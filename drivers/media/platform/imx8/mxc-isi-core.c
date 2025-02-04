/*
 * Copyright 2017-2018 NXP
 */
/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#include "mxc-media-dev.h"

static const struct of_device_id mxc_isi_of_match[];

static irqreturn_t mxc_isi_irq_handler(int irq, void *priv)
{
	struct mxc_isi_dev *mxc_isi = priv;
	struct device *dev = &mxc_isi->pdev->dev;
	u32 status;

	spin_lock(&mxc_isi->slock);

	status = mxc_isi_get_irq_status(mxc_isi);
	mxc_isi_clean_irq_status(mxc_isi, status);

	if (status & CHNL_STS_FRM_STRD_MASK) {
		if (mxc_isi->is_m2m)
			mxc_isi_m2m_frame_write_done(mxc_isi);
		else
			mxc_isi_cap_frame_write_done(mxc_isi);
	}

	if (status & (CHNL_STS_AXI_WR_ERR_Y_MASK |
					CHNL_STS_AXI_WR_ERR_U_MASK |
					CHNL_STS_AXI_WR_ERR_V_MASK))
		dev_dbg(dev, "%s, IRQ AXI Error stat=0x%X\n", __func__, status);
	if (status & (CHNL_STS_OFLW_PANIC_Y_BUF_MASK |
					CHNL_STS_OFLW_PANIC_U_BUF_MASK |
					CHNL_STS_OFLW_PANIC_V_BUF_MASK))
		dev_dbg(dev, "%s, IRQ Panic OFLW Error stat=0x%X\n", __func__, status);
	if (status & (CHNL_STS_OFLW_Y_BUF_MASK |
					CHNL_STS_OFLW_U_BUF_MASK |
					CHNL_STS_OFLW_V_BUF_MASK))
		dev_dbg(dev, "%s, IRQ OFLW Error stat=0x%X\n", __func__, status);
	if (status & (CHNL_STS_EXCS_OFLW_Y_BUF_MASK |
					CHNL_STS_EXCS_OFLW_U_BUF_MASK |
					CHNL_STS_EXCS_OFLW_V_BUF_MASK))
		dev_dbg(dev, "%s, IRQ EXCS OFLW Error stat=0x%X\n", __func__, status);

	spin_unlock(&mxc_isi->slock);
	return IRQ_HANDLED;
}

static int disp_mix_sft_rstn(struct reset_control *reset, bool enable)
{
	int ret;

	if (!reset)
		return 0;

	ret = enable ? reset_control_assert(reset) :
			 reset_control_deassert(reset);
	return ret;
}

static int disp_mix_clks_enable(struct reset_control *reset, bool enable)
{
	int ret;

	if (!reset)
		return 0;

	ret = enable ? reset_control_assert(reset) :
			 reset_control_deassert(reset);
	return ret;
}

/**
 * mxc_isi_adjust_mplane_format - adjust bytesperline or sizeimage
 */
void mxc_isi_adjust_mplane_format(struct mxc_isi_fmt *fmt, u32 width, u32 height,
			       struct v4l2_pix_format_mplane *pix)
{
	u32 bytesperline = 0;
	int i;

	pix->colorspace	= V4L2_COLORSPACE_JPEG;
	pix->field = V4L2_FIELD_NONE;
	pix->num_planes = fmt->memplanes;
	pix->pixelformat = fmt->fourcc;
	pix->height = height;
	pix->width = width;

	for (i = 0; i < pix->num_planes; ++i) {
		struct v4l2_plane_pix_format *plane_fmt = &pix->plane_fmt[i];
		u32 bpl = plane_fmt->bytesperline;

		if (fmt->colplanes > 1 && (bpl == 0 || bpl < pix->width))
			bpl = pix->width; /* Planar */

		if (fmt->colplanes == 1 && /* Packed */
		    (bpl == 0 || ((bpl * 8) / fmt->depth[i]) < pix->width))
			bpl = (pix->width * fmt->depth[0]) / 8;

		if (i == 0)
			bytesperline = bpl;
		else if (i == 1 && fmt->memplanes == 3)
			bytesperline /= 2;

		plane_fmt->bytesperline = bytesperline;
		plane_fmt->sizeimage = max((pix->width * pix->height *
				   fmt->depth[i]) / 8, plane_fmt->sizeimage);
	}
}

static int mxc_isi_parse_dt(struct mxc_isi_dev *mxc_isi)
{
	struct device *dev = &mxc_isi->pdev->dev;
	struct device_node *node = dev->of_node;
	int ret = 0;

	mxc_isi->id = of_alias_get_id(node, "isi");

	ret = of_property_read_u32_array(node, "interface",
			mxc_isi->interface, 3);
	if (ret < 0)
		return ret;

	mxc_isi->parallel_csi = of_property_read_bool(node, "parallel_csi");

	dev_dbg(dev, "%s, isi_%d,interface(%d, %d, %d)\n", __func__, mxc_isi->id,
			mxc_isi->interface[0], mxc_isi->interface[1], mxc_isi->interface[2]);

	mxc_isi->chain_buf = of_property_read_bool(node, "fsl,chain_buf");
	return 0;
}

static int mxc_imx8_clk_get(struct mxc_isi_dev *mxc_isi)
{
	struct device *dev = &mxc_isi->pdev->dev;

	mxc_isi->clk = devm_clk_get(dev, NULL);

	if (IS_ERR(mxc_isi->clk)) {
		dev_err(dev, "failed to get isi clk\n");
		return PTR_ERR(mxc_isi->clk);
	}

	return 0;
}

static int mxc_imx8_clk_enable(struct mxc_isi_dev *mxc_isi)
{
	struct device *dev = &mxc_isi->pdev->dev;
	int ret;

	ret = clk_prepare_enable(mxc_isi->clk);
	if (ret < 0) {
		dev_err(dev, "%s, enable clk error\n", __func__);
		return ret;
	}

	return 0;
}

static void mxc_imx8_clk_disable(struct mxc_isi_dev *mxc_isi)
{
	clk_disable_unprepare(mxc_isi->clk);
}

static struct mxc_isi_dev_ops mxc_imx8_data = {
	.clk_get     = mxc_imx8_clk_get,
	.clk_enable  = mxc_imx8_clk_enable,
	.clk_disable = mxc_imx8_clk_disable,
};

static int mxc_imx8mn_clk_get(struct mxc_isi_dev *mxc_isi)
{
	struct device *dev = &mxc_isi->pdev->dev;

	mxc_isi->clk_disp_axi = devm_clk_get(dev, "disp_axi");
	if (IS_ERR(mxc_isi->clk_disp_axi)) {
		dev_err(dev, "failed to get disp_axi clk\n");
		return PTR_ERR(mxc_isi->clk_disp_axi);
	}

	mxc_isi->clk_disp_apb = devm_clk_get(dev, "disp_apb");
	if (IS_ERR(mxc_isi->clk_disp_apb)) {
		dev_err(dev, "failed to get disp_apb clk\n");
		return PTR_ERR(mxc_isi->clk_disp_apb);
	}

	mxc_isi->clk_root_disp_axi = devm_clk_get(dev, "disp_axi_root");
	if (IS_ERR(mxc_isi->clk_root_disp_axi)) {
		dev_err(dev, "failed to get disp axi root clk\n");
		return PTR_ERR(mxc_isi->clk_root_disp_axi);
	}

	mxc_isi->clk_root_disp_apb = devm_clk_get(dev, "disp_apb_root");
	if (IS_ERR(mxc_isi->clk_root_disp_apb)) {
		dev_err(dev, "failed to get disp apb root clk\n");
		return PTR_ERR(mxc_isi->clk_root_disp_apb);
	}

	return 0;
}

static int mxc_imx8mn_clk_enable(struct mxc_isi_dev *mxc_isi)
{
	struct device *dev = &mxc_isi->pdev->dev;
	int ret;

	ret = clk_prepare_enable(mxc_isi->clk_disp_axi);
	if (ret < 0) {
		dev_err(dev, "prepare and enable axi clk error\n");
		return ret;
	}

	ret = clk_prepare_enable(mxc_isi->clk_disp_apb);
	if (ret < 0) {
		dev_err(dev, "prepare and enable abp clk error\n");
		return ret;
	}

	ret = clk_prepare_enable(mxc_isi->clk_root_disp_axi);
	if (ret < 0) {
		dev_err(dev, "prepare and enable axi root clk error\n");
		return ret;
	}

	ret = clk_prepare_enable(mxc_isi->clk_root_disp_apb);
	if (ret < 0) {
		dev_err(dev, "prepare and enable apb root clk error\n");
		return ret;
	}

	return 0;
}

static void mxc_imx8mn_clk_disable(struct mxc_isi_dev *mxc_isi)
{
	clk_disable_unprepare(mxc_isi->clk_root_disp_axi);
	clk_disable_unprepare(mxc_isi->clk_root_disp_apb);
	clk_disable_unprepare(mxc_isi->clk_disp_axi);
	clk_disable_unprepare(mxc_isi->clk_disp_apb);
}

static struct mxc_isi_dev_ops mxc_imx8mn_data = {
	.clk_get     = mxc_imx8mn_clk_get,
	.clk_enable  = mxc_imx8mn_clk_enable,
	.clk_disable = mxc_imx8mn_clk_disable,
};

static int mxc_isi_clk_get(struct mxc_isi_dev *mxc_isi)
{
	const struct mxc_isi_dev_ops *ops = mxc_isi->ops;

	if (!ops && !ops->clk_get)
		return -EINVAL;

	return ops->clk_get(mxc_isi);
}

static int mxc_isi_clk_enable(struct mxc_isi_dev *mxc_isi)
{
	const struct mxc_isi_dev_ops *ops = mxc_isi->ops;

	if (!ops && !ops->clk_enable)
		return -EINVAL;

	return ops->clk_enable(mxc_isi);
}

static void mxc_isi_clk_disable(struct mxc_isi_dev *mxc_isi)
{
	const struct mxc_isi_dev_ops *ops = mxc_isi->ops;

	if (!ops && !ops->clk_disable)
		return;

	ops->clk_disable(mxc_isi);
}

static int mxc_isi_of_parse_resets(struct mxc_isi_dev *mxc_isi)
{
	int ret;
	struct device *dev = &mxc_isi->pdev->dev;
	struct device_node *np = dev->of_node;
	struct device_node *parent, *child;
	struct of_phandle_args args;
	struct reset_control *rstc;
	const char *compat;
	uint32_t len, rstc_num = 0;

	ret = of_parse_phandle_with_args(np, "resets", "#reset-cells",
					 0, &args);
	if (ret)
		return ret;

	parent = args.np;
	for_each_child_of_node(parent, child) {
		compat = of_get_property(child, "compatible", NULL);
		if (!compat)
			continue;

		rstc = of_reset_control_array_get(child, false, false);
		if (IS_ERR(rstc))
			continue;

		len = strlen(compat);
		if (!of_compat_cmp("isi,soft-resetn", compat, len)) {
			mxc_isi->soft_resetn = rstc;
			rstc_num++;
		} else if (!of_compat_cmp("isi,clk-enable", compat, len)) {
			mxc_isi->clk_enable = rstc;
			rstc_num++;
		} else {
			dev_warn(dev, "invalid isi reset node: %s\n", compat);
		}
	}

	if (!rstc_num) {
		dev_err(dev, "no invalid reset control exists\n");
		return -EINVAL;
	}

	of_node_put(parent);
	return 0;
}

static int mxc_isi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct mxc_isi_dev *mxc_isi;
	struct resource *res;
	const struct of_device_id *of_id;
	int ret = 0;

	mxc_isi = devm_kzalloc(dev, sizeof(*mxc_isi), GFP_KERNEL);
	if (!mxc_isi)
		return -ENOMEM;

	mxc_isi->pdev = pdev;
	of_id = of_match_node(mxc_isi_of_match, dev->of_node);
	if (!of_id)
		return -EINVAL;

	mxc_isi->ops = of_id->data;
	if (!mxc_isi->ops) {
		dev_err(dev, "Can't get platform device data\n");
		return -EINVAL;
	}

	ret = mxc_isi_parse_dt(mxc_isi);
	if (ret < 0)
		return ret;

	if (mxc_isi->id >= MXC_ISI_MAX_DEVS || mxc_isi->id < 0) {
		dev_err(dev, "Invalid driver data or device id (%d)\n",
			mxc_isi->id);
		return -EINVAL;
	}

	init_waitqueue_head(&mxc_isi->irq_queue);
	spin_lock_init(&mxc_isi->slock);
	mutex_init(&mxc_isi->lock);
	mutex_init(&mxc_isi->m2m_lock);
	atomic_set(&mxc_isi->open_count, 0);

	if (of_device_is_compatible(dev->of_node, "fsl,imx8mn-isi")) {
		ret = mxc_isi_of_parse_resets(mxc_isi);
		if (ret) {
			dev_warn(dev, "Can not parse reset control\n");
			return ret;
		}
	}

	ret = mxc_isi_clk_get(mxc_isi);
	if (ret < 0) {
		dev_err(dev, "ISI_%d get clocks fail\n", mxc_isi->id);
		return ret;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	mxc_isi->regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(mxc_isi->regs)) {
		dev_err(dev, "Failed to get ISI register map\n");
		return PTR_ERR(mxc_isi->regs);
	}

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res == NULL) {
		dev_err(dev, "Failed to get IRQ resource\n");
		return -ENXIO;
	}

	ret = mxc_isi_clk_enable(mxc_isi);
	if (ret < 0) {
		dev_err(dev, "ISI_%d enable clocks fail\n", mxc_isi->id);
		return ret;
	}
	disp_mix_sft_rstn(mxc_isi->soft_resetn, false);
	disp_mix_clks_enable(mxc_isi->clk_enable, true);

	mxc_isi_clean_registers(mxc_isi);

	ret = devm_request_irq(dev, res->start, mxc_isi_irq_handler,
			       0, dev_name(dev), mxc_isi);
	if (ret < 0) {
		dev_err(dev, "failed to install irq (%d)\n", ret);
		goto err_clk;
	}

	ret = mxc_isi_initialize_capture_subdev(mxc_isi);
	if (ret < 0) {
		dev_err(dev, "failed to init cap subdev (%d)\n", ret);
		goto err_clk;
	}

	platform_set_drvdata(pdev, mxc_isi);

	mxc_isi_channel_set_chain_buf(mxc_isi);
	mxc_isi_clk_disable(mxc_isi);

	pm_runtime_enable(dev);

	dev_dbg(dev, "mxc_isi.%d registered successfully\n", mxc_isi->id);

	return 0;

err_clk:
	mxc_isi_clk_disable(mxc_isi);
	mxc_isi_unregister_capture_subdev(mxc_isi);
	return ret;
}

static int mxc_isi_remove(struct platform_device *pdev)
{
	struct mxc_isi_dev *mxc_isi = platform_get_drvdata(pdev);
	struct device *dev = &pdev->dev;

	mxc_isi_unregister_capture_subdev(mxc_isi);
	pm_runtime_disable(dev);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int mxc_isi_pm_suspend(struct device *dev)
{
	struct mxc_isi_dev *mxc_isi = dev_get_drvdata(dev);

	if (mxc_isi->is_streaming) {
		dev_warn(dev, "running, prevent entering suspend.\n");
		return -EAGAIN;
	}

	return pm_runtime_force_suspend(dev);
}

static int mxc_isi_pm_resume(struct device *dev)
{
	int ret;

	ret = pm_runtime_force_resume(dev);
	if (ret < 0)
		return ret;

	return 0;
}
#endif

static int mxc_isi_runtime_suspend(struct device *dev)
{
	struct mxc_isi_dev *mxc_isi = dev_get_drvdata(dev);

	disp_mix_clks_enable(mxc_isi->clk_enable, false);
	mxc_isi_clk_disable(mxc_isi);

	return 0;
}

static int mxc_isi_runtime_resume(struct device *dev)
{
	struct mxc_isi_dev *mxc_isi = dev_get_drvdata(dev);
	int ret;

	ret = mxc_isi_clk_enable(mxc_isi);
	if (ret) {
		dev_err(dev, "%s clk enable fail\n", __func__);
		return ret;
	}
	disp_mix_sft_rstn(mxc_isi->soft_resetn, false);
	disp_mix_clks_enable(mxc_isi->clk_enable, true);

	return 0;
}

static const struct dev_pm_ops mxc_isi_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(mxc_isi_pm_suspend, mxc_isi_pm_resume)
	SET_RUNTIME_PM_OPS(mxc_isi_runtime_suspend, mxc_isi_runtime_resume, NULL)
};

static const struct of_device_id mxc_isi_of_match[] = {
	{.compatible = "fsl,imx8-isi", .data = &mxc_imx8_data },
	{.compatible = "fsl,imx8mn-isi", .data = &mxc_imx8mn_data },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(of, mxc_isi_of_match);

static struct platform_driver mxc_isi_driver = {
	.probe		= mxc_isi_probe,
	.remove		= mxc_isi_remove,
	.driver = {
		.of_match_table = mxc_isi_of_match,
		.name		= MXC_ISI_DRIVER_NAME,
		.pm		= &mxc_isi_pm_ops,
	}
};

module_platform_driver(mxc_isi_driver);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("MXC Image Subsystem driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ISI");
MODULE_VERSION("1.0");
