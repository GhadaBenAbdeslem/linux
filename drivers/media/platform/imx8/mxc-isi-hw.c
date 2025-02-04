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
#include <soc/imx8/sc/sci.h>
#include <dt-bindings/pinctrl/pads-imx8qxp.h>

#include "mxc-isi-hw.h"
#define	ISI_DOWNSCALE_THRESHOLD		0x4000

#ifdef DEBUG
void dump_isi_regs(struct mxc_isi_dev *mxc_isi)
{
	struct device *dev = &mxc_isi->pdev->dev;

	dev_dbg(dev, "ISI CHNLC register dump, isi%d\n", mxc_isi->id);
	dev_dbg(dev, "CHNL_CTRL              0x0h  = 0x%8x\n", readl(mxc_isi->regs + 0x0));
	dev_dbg(dev, "CHNL_IMG_CTRL          0x4h  = 0x%8x\n", readl(mxc_isi->regs + 0x4));
	dev_dbg(dev, "CHNL_OUT_BUF_CTRL      0x8h  = 0x%8x\n", readl(mxc_isi->regs + 0x8));
	dev_dbg(dev, "CHNL_IMG_CFG           0xCh  = 0x%8x\n", readl(mxc_isi->regs + 0xC));
	dev_dbg(dev, "CHNL_IER               0x10h = 0x%8x\n", readl(mxc_isi->regs + 0x10));
	dev_dbg(dev, "CHNL_STS               0x14h = 0x%8x\n", readl(mxc_isi->regs + 0x14));
	dev_dbg(dev, "CHNL_SCALE_FACTOR      0x18h = 0x%8x\n", readl(mxc_isi->regs + 0x18));
	dev_dbg(dev, "CHNL_SCALE_OFFSET      0x1Ch = 0x%8x\n", readl(mxc_isi->regs + 0x1C));
	dev_dbg(dev, "CHNL_CROP_ULC          0x20h = 0x%8x\n", readl(mxc_isi->regs + 0x20));
	dev_dbg(dev, "CHNL_CROP_LRC          0x24h = 0x%8x\n", readl(mxc_isi->regs + 0x24));
	dev_dbg(dev, "CHNL_CSC_COEFF0        0x28h = 0x%8x\n", readl(mxc_isi->regs + 0x28));
	dev_dbg(dev, "CHNL_CSC_COEFF1        0x2Ch = 0x%8x\n", readl(mxc_isi->regs + 0x2C));
	dev_dbg(dev, "CHNL_CSC_COEFF2        0x30h = 0x%8x\n", readl(mxc_isi->regs + 0x30));
	dev_dbg(dev, "CHNL_CSC_COEFF3        0x34h = 0x%8x\n", readl(mxc_isi->regs + 0x34));
	dev_dbg(dev, "CHNL_CSC_COEFF4        0x38h = 0x%8x\n", readl(mxc_isi->regs + 0x38));
	dev_dbg(dev, "CHNL_CSC_COEFF5        0x3Ch = 0x%8x\n", readl(mxc_isi->regs + 0x3C));
	dev_dbg(dev, "CHNL_ROI_0_ALPHA       0x40h = 0x%8x\n", readl(mxc_isi->regs + 0x40));
	dev_dbg(dev, "CHNL_ROI_0_ULC         0x44h = 0x%8x\n", readl(mxc_isi->regs + 0x44));
	dev_dbg(dev, "CHNL_ROI_0_LRC         0x48h = 0x%8x\n", readl(mxc_isi->regs + 0x48));
	dev_dbg(dev, "CHNL_ROI_1_ALPHA       0x4Ch = 0x%8x\n", readl(mxc_isi->regs + 0x4C));
	dev_dbg(dev, "CHNL_ROI_1_ULC         0x50h = 0x%8x\n", readl(mxc_isi->regs + 0x50));
	dev_dbg(dev, "CHNL_ROI_1_LRC         0x54h = 0x%8x\n", readl(mxc_isi->regs + 0x54));
	dev_dbg(dev, "CHNL_ROI_2_ALPHA       0x58h = 0x%8x\n", readl(mxc_isi->regs + 0x58));
	dev_dbg(dev, "CHNL_ROI_2_ULC         0x5Ch = 0x%8x\n", readl(mxc_isi->regs + 0x5C));
	dev_dbg(dev, "CHNL_ROI_2_LRC         0x60h = 0x%8x\n", readl(mxc_isi->regs + 0x60));
	dev_dbg(dev, "CHNL_ROI_3_ALPHA       0x64h = 0x%8x\n", readl(mxc_isi->regs + 0x64));
	dev_dbg(dev, "CHNL_ROI_3_ULC         0x68h = 0x%8x\n", readl(mxc_isi->regs + 0x68));
	dev_dbg(dev, "CHNL_ROI_3_LRC         0x6Ch = 0x%8x\n", readl(mxc_isi->regs + 0x6C));
	dev_dbg(dev, "CHNL_OUT_BUF1_ADDR_Y   0x70h = 0x%8x\n", readl(mxc_isi->regs + 0x70));
	dev_dbg(dev, "CHNL_OUT_BUF1_ADDR_U   0x74h = 0x%8x\n", readl(mxc_isi->regs + 0x74));
	dev_dbg(dev, "CHNL_OUT_BUF1_ADDR_V   0x78h = 0x%8x\n", readl(mxc_isi->regs + 0x78));
	dev_dbg(dev, "CHNL_OUT_BUF_PITCH     0x7Ch = 0x%8x\n", readl(mxc_isi->regs + 0x7C));
	dev_dbg(dev, "CHNL_IN_BUF_ADDR       0x80h = 0x%8x\n", readl(mxc_isi->regs + 0x80));
	dev_dbg(dev, "CHNL_IN_BUF_PITCH      0x84h = 0x%8x\n", readl(mxc_isi->regs + 0x84));
	dev_dbg(dev, "CHNL_MEM_RD_CTRL       0x88h = 0x%8x\n", readl(mxc_isi->regs + 0x88));
	dev_dbg(dev, "CHNL_OUT_BUF2_ADDR_Y   0x8Ch = 0x%8x\n", readl(mxc_isi->regs + 0x8C));
	dev_dbg(dev, "CHNL_OUT_BUF2_ADDR_U   0x90h = 0x%8x\n", readl(mxc_isi->regs + 0x90));
	dev_dbg(dev, "CHNL_OUT_BUF2_ADDR_V   0x94h = 0x%8x\n", readl(mxc_isi->regs + 0x94));
	dev_dbg(dev, "CHNL_SCL_IMG_CFG       0x98h = 0x%8x\n", readl(mxc_isi->regs + 0x98));
	dev_dbg(dev, "CHNL_FLOW_CTRL         0x9Ch = 0x%8x\n", readl(mxc_isi->regs + 0x9C));
}
#else
void dump_isi_regs(struct mxc_isi_dev *mxc_isi)
{
}
#endif

static const u32 coeffs[2][6] = {
	/* A2,A1,      B1, A3,     B3, B2,      C2, C1,     D1, C3,   D3, D2   */
	/* YUV2RGB */
	{ 0x0000012A, 0x012A0198, 0x0730079C, 0x0204012A, 0x01F00000, 0x01800180 },
	/* RGB->YUV */
	{ 0x00810041, 0x07db0019, 0x007007b6, 0x07a20070, 0x001007ee, 0x00800080 },
};

static void printk_pixelformat(char *prefix, int val)
{
	printk("%s %c%c%c%c\n", prefix ? prefix : "pixelformat",
			val & 0xff, (val >> 8) & 0xff, (val >> 16) & 0xff, (val >> 24) & 0xff);
}

static bool is_rgb(u32 pix_fmt)
{
	if ((pix_fmt == V4L2_PIX_FMT_RGB565) ||
		(pix_fmt == V4L2_PIX_FMT_RGB24) ||
		(pix_fmt == V4L2_PIX_FMT_RGB32) ||
		(pix_fmt == V4L2_PIX_FMT_BGR32) ||
		(pix_fmt == V4L2_PIX_FMT_XRGB32) ||
		(pix_fmt == V4L2_PIX_FMT_XBGR32) ||
		(pix_fmt == V4L2_PIX_FMT_BGR24) ||
		(pix_fmt == V4L2_PIX_FMT_RGBA) ||
		(pix_fmt == V4L2_PIX_FMT_ABGR32) ||
	    (pix_fmt == V4L2_PIX_FMT_ARGB32)) {
		return true;
	} else {
		return false;
	}
}

static bool is_yuv(u32 pix_fmt)
{
	if ((pix_fmt == V4L2_PIX_FMT_YUYV) ||
		(pix_fmt == V4L2_PIX_FMT_YUV32) ||
		(pix_fmt == V4L2_PIX_FMT_YUV444M) ||
		(pix_fmt == V4L2_PIX_FMT_NV12)) {
		return true;
	} else {
		return false;
	}
}

static void chain_buf(struct mxc_isi_dev *mxc_isi)
{
	struct mxc_isi_frame *src_f;
	u32 val;

	if (mxc_isi->is_m2m)
		src_f = &mxc_isi->m2m.src_f;
	else
		src_f = &mxc_isi->isi_cap.src_f;

	if (src_f->o_width > 2048) {
		val = readl(mxc_isi->regs + CHNL_CTRL);
		val &= ~CHNL_CTRL_CHAIN_BUF_MASK;
		val |= (CHNL_CTRL_CHAIN_BUF_2_CHAIN << CHNL_CTRL_CHAIN_BUF_OFFSET);
		writel(val, mxc_isi->regs + CHNL_CTRL);
	} else if (!mxc_isi->chain_buf) {
		val = readl(mxc_isi->regs + CHNL_CTRL);
		val &= ~CHNL_CTRL_CHAIN_BUF_MASK;
		writel(val, mxc_isi->regs + CHNL_CTRL);
	}
}

void mxc_isi_channel_set_outbuf(struct mxc_isi_dev *mxc_isi, struct mxc_isi_buffer *buf)
{
	struct vb2_buffer *vb2_buf = &buf->v4l2_buf.vb2_buf;
	struct frame_addr *paddr = &buf->paddr;
	struct v4l2_pix_format_mplane *pix = &mxc_isi->pix;
	u32 framecount = buf->v4l2_buf.sequence;
	int val = 0;

	if (buf->discard) {
		paddr->y = mxc_isi->discard_buffer_dma[0];
		if (pix->num_planes == 2)
			paddr->cb = mxc_isi->discard_buffer_dma[1];
		if (pix->num_planes == 3) {
			paddr->cb = mxc_isi->discard_buffer_dma[1];
			paddr->cr = mxc_isi->discard_buffer_dma[2];
		}
	} else {
		paddr->y = vb2_dma_contig_plane_dma_addr(vb2_buf, 0);

		if (vb2_buf->num_planes == 2)
			paddr->cb = vb2_dma_contig_plane_dma_addr(vb2_buf, 1);
		if (vb2_buf->num_planes == 3) {
			paddr->cb = vb2_dma_contig_plane_dma_addr(vb2_buf, 1);
			paddr->cr = vb2_dma_contig_plane_dma_addr(vb2_buf, 2);
		}
	}

	val = readl(mxc_isi->regs + CHNL_OUT_BUF_CTRL);

	if (framecount % 2 == 0) {
		writel(paddr->y, mxc_isi->regs + CHNL_OUT_BUF1_ADDR_Y);
		writel(paddr->cb, mxc_isi->regs + CHNL_OUT_BUF1_ADDR_U);
		writel(paddr->cr, mxc_isi->regs + CHNL_OUT_BUF1_ADDR_V);
		val ^= CHNL_OUT_BUF_CTRL_LOAD_BUF1_ADDR_MASK;
	} else if (framecount % 2 == 1) {
		writel(paddr->y, mxc_isi->regs + CHNL_OUT_BUF2_ADDR_Y);
		writel(paddr->cb, mxc_isi->regs + CHNL_OUT_BUF2_ADDR_U);
		writel(paddr->cr, mxc_isi->regs + CHNL_OUT_BUF2_ADDR_V);
		val ^= CHNL_OUT_BUF_CTRL_LOAD_BUF2_ADDR_MASK;
	}
	writel(val, mxc_isi->regs + CHNL_OUT_BUF_CTRL);
}

void mxc_isi_channel_set_m2m_out_addr(struct mxc_isi_dev *mxc_isi,
			struct mxc_isi_buffer *buf)
{
	mxc_isi_channel_set_outbuf(mxc_isi, buf);
}

void mxc_isi_channel_set_m2m_src_addr(struct mxc_isi_dev *mxc_isi,
			struct mxc_isi_buffer *buf)
{
	struct vb2_buffer *vb2_buf = &buf->v4l2_buf.vb2_buf;
	struct frame_addr *paddr = &buf->paddr;

	/* Only support one plane */
	paddr->y = vb2_dma_contig_plane_dma_addr(vb2_buf, 0);
	writel(paddr->y, mxc_isi->regs + CHNL_IN_BUF_ADDR);
}

void mxc_isi_channel_sw_reset(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = readl(mxc_isi->regs + CHNL_CTRL);
	val |= CHNL_CTRL_SW_RST;
	writel(val, mxc_isi->regs + CHNL_CTRL);
	mdelay(5);
	val &= ~CHNL_CTRL_SW_RST;
	writel(val, mxc_isi->regs + CHNL_CTRL);
}

void mxc_isi_channel_source_config(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = readl(mxc_isi->regs + CHNL_CTRL);
	val &= ~(CHNL_CTRL_MIPI_VC_ID_MASK |
				CHNL_CTRL_SRC_INPUT_MASK | CHNL_CTRL_SRC_TYPE_MASK);

	switch (mxc_isi->interface[IN_PORT]) {
	case ISI_INPUT_INTERFACE_MIPI0_CSI2:
		val |= CHNL_CTRL_SRC_INPUT_MIPI0;
		if (mxc_isi->interface[SUB_IN_PORT] <= CHNL_CTRL_MIPI_VC_ID_VC3 &&
				mxc_isi->interface[SUB_IN_PORT] >= CHNL_CTRL_MIPI_VC_ID_VC0)
			val |= (mxc_isi->interface[SUB_IN_PORT] << CHNL_CTRL_MIPI_VC_ID_OFFSET);
		break;
	case ISI_INPUT_INTERFACE_MIPI1_CSI2:
		val |= CHNL_CTRL_SRC_INPUT_MIPI1;
		if (mxc_isi->interface[SUB_IN_PORT] <= CHNL_CTRL_MIPI_VC_ID_VC3 &&
				mxc_isi->interface[SUB_IN_PORT] >= CHNL_CTRL_MIPI_VC_ID_VC0)
			val |= (mxc_isi->interface[SUB_IN_PORT] << CHNL_CTRL_MIPI_VC_ID_OFFSET);
		break;
	case ISI_INPUT_INTERFACE_DC0:
		val |= CHNL_CTRL_SRC_INPUT_DC0;
		break;
	case ISI_INPUT_INTERFACE_DC1:
		val |= CHNL_CTRL_SRC_INPUT_DC1;
		break;
	case ISI_INPUT_INTERFACE_HDMI:
		val |= CHNL_CTRL_SRC_INPUT_HDMI;
		break;
	case ISI_INPUT_INTERFACE_PARALLEL_CSI:
		val |= CHNL_CTRL_SRC_INPUT_CSI;
		break;
	case ISI_INPUT_INTERFACE_MEM:
		val |= CHNL_CTRL_SRC_INPUT_MEMORY;
		val |= (CHNL_CTRL_SRC_TYPE_MEMORY << CHNL_CTRL_SRC_TYPE_OFFSET);
		break;
	default:
		dev_err(&mxc_isi->pdev->dev, "invalid interface\n");
		break;
	}

	writel(val, mxc_isi->regs + CHNL_CTRL);
}

void mxc_isi_channel_set_flip(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val &= ~(CHNL_IMG_CTRL_VFLIP_EN_MASK | CHNL_IMG_CTRL_HFLIP_EN_MASK);

	if (mxc_isi->vflip)
		val |= (CHNL_IMG_CTRL_VFLIP_EN_ENABLE << CHNL_IMG_CTRL_VFLIP_EN_OFFSET);
	if (mxc_isi->hflip)
		val |= (CHNL_IMG_CTRL_HFLIP_EN_ENABLE << CHNL_IMG_CTRL_HFLIP_EN_OFFSET);

	writel(val, mxc_isi->regs + CHNL_IMG_CTRL);
}

void mxc_isi_channel_set_csc(struct mxc_isi_dev *mxc_isi)
{
	struct mxc_isi_fmt *dst_fmt = mxc_isi->isi_cap.dst_f.fmt;
	struct mxc_isi_fmt *src_fmt = mxc_isi->isi_cap.src_f.fmt;
	u32 val, csc = 0;

	if (mxc_isi->is_m2m) {
		src_fmt = mxc_isi->m2m.src_f.fmt;
		dst_fmt = mxc_isi->m2m.dst_f.fmt;
	}

	val = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val &= ~(CHNL_IMG_CTRL_FORMAT_MASK |
			CHNL_IMG_CTRL_YCBCR_MODE_MASK |
			CHNL_IMG_CTRL_CSC_BYPASS_MASK |
			CHNL_IMG_CTRL_CSC_MODE_MASK);

	/* set outbuf format */
	val |= dst_fmt->color << CHNL_IMG_CTRL_FORMAT_OFFSET;

	mxc_isi->cscen = 1;

	if (is_yuv(src_fmt->fourcc) && is_rgb(dst_fmt->fourcc)) {
		/* YUV2RGB */
		csc = YUV2RGB;
		/* YCbCr enable???  */
		val |= (CHNL_IMG_CTRL_CSC_MODE_YCBCR2RGB << CHNL_IMG_CTRL_CSC_MODE_OFFSET);
		val |= (CHNL_IMG_CTRL_YCBCR_MODE_ENABLE << CHNL_IMG_CTRL_YCBCR_MODE_OFFSET);
	} else if (is_rgb(src_fmt->fourcc) && is_yuv(dst_fmt->fourcc)) {
		/* RGB2YUV */
		csc = RGB2YUV;
		val |= (CHNL_IMG_CTRL_CSC_MODE_RGB2YCBCR << CHNL_IMG_CTRL_CSC_MODE_OFFSET);
	} else {
		/* Bypass CSC */
		printk("bypass csc\n");
		mxc_isi->cscen = 0;
		val |= CHNL_IMG_CTRL_CSC_BYPASS_ENABLE;
	}

	printk_pixelformat("input fmt", src_fmt->fourcc);
	printk_pixelformat("output fmt", dst_fmt->fourcc);

	if (mxc_isi->cscen) {
		writel(coeffs[csc][0], mxc_isi->regs + CHNL_CSC_COEFF0);
		writel(coeffs[csc][1], mxc_isi->regs + CHNL_CSC_COEFF1);
		writel(coeffs[csc][2], mxc_isi->regs + CHNL_CSC_COEFF2);
		writel(coeffs[csc][3], mxc_isi->regs + CHNL_CSC_COEFF3);
		writel(coeffs[csc][4], mxc_isi->regs + CHNL_CSC_COEFF4);
		writel(coeffs[csc][5], mxc_isi->regs + CHNL_CSC_COEFF5);
	}

	writel(val, mxc_isi->regs + CHNL_IMG_CTRL);
}
void mxc_isi_channel_set_alpha_roi0(struct mxc_isi_dev *mxc_isi,
				struct v4l2_rect *rect)
{
	u32 val0, val1;
	val0 = (rect->left << 16) | rect->top;
	writel(val0, mxc_isi->regs + CHNL_ROI_0_ULC);
	val1 = (rect->width << 16) | rect->height;
	writel(val0 + val1, mxc_isi->regs + CHNL_ROI_0_LRC);
}

void mxc_isi_channel_set_alpha(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val &= ~(CHNL_IMG_CTRL_GBL_ALPHA_VAL_MASK | CHNL_IMG_CTRL_GBL_ALPHA_EN_MASK);
	val |= ((mxc_isi->alpha << CHNL_IMG_CTRL_GBL_ALPHA_VAL_OFFSET) |
			(CHNL_IMG_CTRL_GBL_ALPHA_EN_ENABLE << CHNL_IMG_CTRL_GBL_ALPHA_EN_OFFSET));

	writel(val, mxc_isi->regs + CHNL_IMG_CTRL);
}

void mxc_isi_channel_set_chain_buf(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	if (mxc_isi->chain_buf) {
		printk("%s\n", __func__);
		val = readl(mxc_isi->regs + CHNL_CTRL);
		val &= ~CHNL_CTRL_CHAIN_BUF_MASK;
		val |= (CHNL_CTRL_CHAIN_BUF_2_CHAIN << CHNL_CTRL_CHAIN_BUF_OFFSET);

		writel(val, mxc_isi->regs + CHNL_CTRL);
	}
}

void mxc_isi_channel_deinterlace_init(struct mxc_isi_dev *mxc_isi)
{
	/* Config for Blending deinterlace */
}

void mxc_isi_channel_set_deinterlace(struct mxc_isi_dev *mxc_isi)
{
	/* de-interlacing method
	 * Weaving-------------Yes
	 * Line Doubling-------No
	 * Blending -----------TODO*/
	u32 val;

	val = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val &= ~CHNL_IMG_CTRL_DEINT_MASK;
	if (mxc_isi->deinterlace)
		val |= mxc_isi->deinterlace << CHNL_IMG_CTRL_DEINT_OFFSET;
	if (mxc_isi->deinterlace == CHNL_IMG_CTRL_DEINT_LDOUBLE_ODD_EVEN ||
			mxc_isi->deinterlace == CHNL_IMG_CTRL_DEINT_LDOUBLE_EVEN_ODD)
		mxc_isi_channel_deinterlace_init(mxc_isi);

	writel(val, mxc_isi->regs + CHNL_IMG_CTRL);
}


void mxc_isi_channel_set_crop(struct mxc_isi_dev *mxc_isi)
{
	struct mxc_isi_frame *src_f = &mxc_isi->isi_cap.src_f;
	struct v4l2_rect crop;
	u32 val, val0, val1, temp;

	val = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val &= ~CHNL_IMG_CTRL_CROP_EN_MASK;

	if ((src_f->o_height == src_f->height) &&
			(src_f->o_width == src_f->width)) {
		mxc_isi->crop = 0;
		writel(val, mxc_isi->regs + CHNL_IMG_CTRL);
		return;
	}

	if (mxc_isi->scale) {
		temp = (src_f->h_off << 12) / mxc_isi->xfactor;
		crop.left = temp >> mxc_isi->pre_dec_x;
		temp = (src_f->v_off << 12) / mxc_isi->yfactor;
		crop.top = temp >> mxc_isi->pre_dec_y;
		temp = (src_f->width << 12) / mxc_isi->xfactor;
		crop.width = temp >> mxc_isi->pre_dec_x;
		temp = (src_f->height << 12) / mxc_isi->yfactor;
		crop.height = temp >> mxc_isi->pre_dec_y;
	} else {
		crop.left = src_f->h_off;
		crop.top = src_f->v_off;
		crop.width = src_f->width;
		crop.height = src_f->height;
	}

	mxc_isi->crop = 1;
	val |= (CHNL_IMG_CTRL_CROP_EN_ENABLE << CHNL_IMG_CTRL_CROP_EN_OFFSET);
	val0 = crop.top | (crop.left << CHNL_CROP_ULC_X_OFFSET);
	val1 = crop.height | (crop.width << CHNL_CROP_LRC_X_OFFSET);

	writel(val0, mxc_isi->regs + CHNL_CROP_ULC);
	writel((val1 + val0), mxc_isi->regs + CHNL_CROP_LRC);
	writel(val, mxc_isi->regs + CHNL_IMG_CTRL);
}

static void mxc_isi_channel_clear_scaling(struct mxc_isi_dev *mxc_isi)
{
	u32 val0;

	writel(0x10001000, mxc_isi->regs + CHNL_SCALE_FACTOR);

	val0 = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val0 &= ~(CHNL_IMG_CTRL_DEC_X_MASK | CHNL_IMG_CTRL_DEC_Y_MASK);
	writel(val0, mxc_isi->regs + CHNL_IMG_CTRL);
}

void mxc_isi_channel_set_scaling(struct mxc_isi_dev *mxc_isi)
{
	struct mxc_isi_frame *dst_f = &mxc_isi->isi_cap.dst_f;
	struct mxc_isi_frame *src_f = &mxc_isi->isi_cap.src_f;
	u32 decx, decy;
	u32 xscale, yscale;
	u32 xdec = 0, ydec = 0;
	u32 val0, val1;

	if (mxc_isi->is_m2m) {
		src_f = &mxc_isi->m2m.src_f;
		dst_f = &mxc_isi->m2m.dst_f;
	}

	if (dst_f->height == src_f->height ||
			dst_f->width == src_f->width) {
		mxc_isi->scale = 0;
		mxc_isi_channel_clear_scaling(mxc_isi);
		dev_dbg(&mxc_isi->pdev->dev, "%s: no scale\n", __func__);
		return;
	}

	dev_info(&mxc_isi->pdev->dev, "input_size(%d,%d), output_size(%d,%d)\n",
			src_f->width, src_f->height, dst_f->width, dst_f->height);

	mxc_isi->scale = 1;

	decx = src_f->width / dst_f->width;
	decy = src_f->height / dst_f->height;

	if (decx > 1) {
		/* Down */
		if (decx >= 2 && decx < 4) {
			decx = 2;
			xdec = 1;
		} else if (decx >= 4 && decx < 8) {
			decx = 4;
			xdec = 2;
		} else if (decx >= 8) {
			decx = 8;
			xdec = 3;
		}
		xscale = src_f->width * 0x1000 / (dst_f->width * decx);
	} else
		/* Up  */
		xscale = src_f->width * 0x1000 / dst_f->width;

	if (decy > 1) {
		if (decy >= 2 && decy < 4) {
			decy = 2;
			ydec = 1;
		} else if (decy >= 4 && decy < 8) {
			decy = 4;
			ydec = 2;
		} else if (decy >= 8) {
			decy = 8;
			ydec = 3;
		}
		yscale = src_f->height * 0x1000 / (dst_f->height * decy);
	} else
		yscale = src_f->height * 0x1000 / dst_f->height;

	val0 = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val0 |= CHNL_IMG_CTRL_YCBCR_MODE_MASK;//YCbCr  Sandor???
	val0 &= ~(CHNL_IMG_CTRL_DEC_X_MASK | CHNL_IMG_CTRL_DEC_Y_MASK);
	val0 |= (xdec << CHNL_IMG_CTRL_DEC_X_OFFSET) |
			(ydec << CHNL_IMG_CTRL_DEC_Y_OFFSET);
	writel(val0, mxc_isi->regs + CHNL_IMG_CTRL);

	if (xscale > ISI_DOWNSCALE_THRESHOLD)
		xscale = ISI_DOWNSCALE_THRESHOLD;
	if (yscale > ISI_DOWNSCALE_THRESHOLD)
		yscale = ISI_DOWNSCALE_THRESHOLD;

	val1 = xscale | (yscale << CHNL_SCALE_FACTOR_Y_SCALE_OFFSET);

	writel(val1, mxc_isi->regs + CHNL_SCALE_FACTOR);

	/* Update scale config if scaling enabled */
	val1 = dst_f->o_width | (dst_f->o_height << CHNL_SCL_IMG_CFG_HEIGHT_OFFSET);
	writel(val1, mxc_isi->regs + CHNL_SCL_IMG_CFG);

	writel(0, mxc_isi->regs + CHNL_SCALE_OFFSET);

	return;
}

void mxc_isi_channel_init(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	/* sw reset */
	mxc_isi_channel_sw_reset(mxc_isi);

	/* Init channel clk first */
	val = readl(mxc_isi->regs + CHNL_CTRL);
	val |= (CHNL_CTRL_CLK_EN_ENABLE << CHNL_CTRL_CLK_EN_OFFSET);
	writel(val, mxc_isi->regs + CHNL_CTRL);
}

void mxc_isi_channel_deinit(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	/* sw reset */
	mxc_isi_channel_sw_reset(mxc_isi);

	/* deinit channel clk first */
	val = (CHNL_CTRL_CLK_EN_ENABLE << CHNL_CTRL_CLK_EN_OFFSET);
	writel(val, mxc_isi->regs + CHNL_CTRL);
}

void mxc_isi_channel_config(struct mxc_isi_dev *mxc_isi)
{
	struct mxc_isi_frame *dst_f = &mxc_isi->isi_cap.dst_f;
	struct mxc_isi_frame *src_f = &mxc_isi->isi_cap.src_f;
	u32 val;

	/* images having higher than 2048 horizontal resolution */
	chain_buf(mxc_isi);

	/* config output frame size and format */
	val = src_f->o_width | (src_f->o_height << CHNL_IMG_CFG_HEIGHT_OFFSET);
	writel(val, mxc_isi->regs + CHNL_IMG_CFG);

	/* scale size need to equal input size when scaling disabled*/
	writel(val, mxc_isi->regs + CHNL_SCL_IMG_CFG);

	/* check csc and scaling  */
	mxc_isi_channel_set_csc(mxc_isi);

	mxc_isi_channel_set_scaling(mxc_isi);

	/* select the source input / src type / virtual channel for mipi*/
	mxc_isi_channel_source_config(mxc_isi);

	/* line pitch */
	val = dst_f->bytesperline[0];
	writel(val, mxc_isi->regs + CHNL_OUT_BUF_PITCH);

	/* TODO */
	mxc_isi_channel_set_flip(mxc_isi);

	if (mxc_isi->alphaen)
		mxc_isi_channel_set_alpha(mxc_isi);
#if 0
	mxc_isi_channel_set_crop(mxc_isi);
#endif

	val = readl(mxc_isi->regs + CHNL_CTRL);
	val &= ~CHNL_CTRL_CHNL_BYPASS_MASK;

	/*  Bypass channel */
	if (!mxc_isi->cscen && !mxc_isi->scale)
		val |= (CHNL_CTRL_CHNL_BYPASS_ENABLE << CHNL_CTRL_CHNL_BYPASS_OFFSET);

	writel(val, mxc_isi->regs + CHNL_CTRL);
}

void mxc_isi_clean_registers(struct mxc_isi_dev *mxc_isi)
{
	u32 status;

	status = mxc_isi_get_irq_status(mxc_isi);
	mxc_isi_clean_irq_status(mxc_isi, status);
}

void mxc_isi_channel_enable(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = readl(mxc_isi->regs + CHNL_CTRL);
	val |= (CHNL_CTRL_CHNL_EN_ENABLE << CHNL_CTRL_CHNL_EN_OFFSET);
	val |= 0xff << CHNL_CTRL_BLANK_PXL_OFFSET;
	writel(val, mxc_isi->regs + CHNL_CTRL);

	mxc_isi_clean_registers(mxc_isi);
	mxc_isi_enable_irq(mxc_isi);
	msleep(300);
	dump_isi_regs(mxc_isi);
}

void mxc_isi_channel_disable(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	mxc_isi_disable_irq(mxc_isi);

	val = readl(mxc_isi->regs + CHNL_CTRL);
	val &= ~(CHNL_CTRL_CHNL_EN_MASK | CHNL_CTRL_CLK_EN_MASK);
	val |= (CHNL_CTRL_CHNL_EN_DISABLE << CHNL_CTRL_CHNL_EN_OFFSET);
	val |= (CHNL_CTRL_CLK_EN_DISABLE << CHNL_CTRL_CLK_EN_OFFSET);
	writel(val, mxc_isi->regs + CHNL_CTRL);
}

void  mxc_isi_enable_irq(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = CHNL_IER_FRM_RCVD_EN_MASK |
			CHNL_IER_OFLW_Y_BUF_EN_MASK |
			CHNL_IER_AXI_WR_ERR_U_EN_MASK |
			CHNL_IER_AXI_WR_ERR_V_EN_MASK |
			CHNL_IER_AXI_WR_ERR_Y_EN_MASK |
			CHNL_IER_OFLW_PANIC_V_BUF_EN_MASK |
			CHNL_IER_EXCS_OFLW_V_BUF_EN_MASK |
			CHNL_IER_OFLW_V_BUF_EN_MASK |
			CHNL_IER_OFLW_PANIC_U_BUF_EN_MASK |
			CHNL_IER_EXCS_OFLW_U_BUF_EN_MASK |
			CHNL_IER_OFLW_U_BUF_EN_MASK |
			CHNL_IER_OFLW_PANIC_Y_BUF_EN_MASK |
			CHNL_IER_EXCS_OFLW_Y_BUF_EN_MASK |
			CHNL_IER_OFLW_Y_BUF_EN_MASK;
	writel(val, mxc_isi->regs + CHNL_IER);
}

void mxc_isi_disable_irq(struct mxc_isi_dev *mxc_isi)
{
	writel(0, mxc_isi->regs + CHNL_IER);
}

u32 mxc_isi_get_irq_status(struct mxc_isi_dev *mxc_isi)
{
	return readl(mxc_isi->regs + CHNL_STS);
}

void mxc_isi_clean_irq_status(struct mxc_isi_dev *mxc_isi, u32 val)
{
	writel(val, mxc_isi->regs + CHNL_STS);
}

void mxc_isi_m2m_channel_set_filp(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val &= ~(CHNL_IMG_CTRL_VFLIP_EN_MASK | CHNL_IMG_CTRL_HFLIP_EN_MASK);

	if (mxc_isi->m2m.vflip)
		val |= (CHNL_IMG_CTRL_VFLIP_EN_ENABLE << CHNL_IMG_CTRL_VFLIP_EN_OFFSET);
	if (mxc_isi->m2m.hflip)
		val |= (CHNL_IMG_CTRL_HFLIP_EN_ENABLE << CHNL_IMG_CTRL_HFLIP_EN_OFFSET);

	writel(val, mxc_isi->regs + CHNL_IMG_CTRL);
}

void mxc_isi_m2m_channel_set_alpha(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val &= ~(CHNL_IMG_CTRL_GBL_ALPHA_VAL_MASK | CHNL_IMG_CTRL_GBL_ALPHA_EN_MASK);
	val |= ((mxc_isi->m2m.alpha << CHNL_IMG_CTRL_GBL_ALPHA_VAL_OFFSET) |
			(CHNL_IMG_CTRL_GBL_ALPHA_EN_ENABLE << CHNL_IMG_CTRL_GBL_ALPHA_EN_OFFSET));

	writel(val, mxc_isi->regs + CHNL_IMG_CTRL);
}

void mxc_isi_m2m_channel_init(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	/* sw reset */
	mxc_isi_channel_sw_reset(mxc_isi);

	/* Init channel clk first */
	val = readl(mxc_isi->regs + CHNL_CTRL);
	val |= (CHNL_CTRL_CLK_EN_ENABLE << CHNL_CTRL_CLK_EN_OFFSET);
	writel(val, mxc_isi->regs + CHNL_CTRL);
}

void mxc_isi_m2m_channel_config(struct mxc_isi_dev *mxc_isi)
{
	struct mxc_isi_frame *src_f, *dst_f;
	u32 reg;

	src_f = &mxc_isi->m2m.src_f;
	dst_f = &mxc_isi->m2m.dst_f;

	chain_buf(mxc_isi);

	/* scale size need to equal input size when scaling disabled*/
	reg = src_f->o_width | (src_f->o_height << CHNL_IMG_CFG_HEIGHT_OFFSET);
	writel(reg, mxc_isi->regs + CHNL_SCL_IMG_CFG);

	/* CSC */
	mxc_isi_channel_set_csc(mxc_isi);

	/* Scaling */
	mxc_isi_channel_set_scaling(mxc_isi);

	/* Horizonal and Vertical flip */
	mxc_isi_m2m_channel_set_filp(mxc_isi);

	if (mxc_isi->m2m.alphaen)
		mxc_isi_m2m_channel_set_alpha(mxc_isi);
}

void mxc_isi_m2m_channel_enable(struct mxc_isi_dev *mxc_isi)
{
	u32 reg;

	/* Read from memory and enable ISI channel */
	reg = readl(mxc_isi->regs + CHNL_CTRL);
	reg &= ~(CHNL_CTRL_SRC_TYPE_MASK |
			 CHNL_CTRL_SRC_INPUT_MASK |
			 CHNL_CTRL_CHNL_EN_MASK);
	reg |= (CHNL_CTRL_SRC_INPUT_MEMORY << CHNL_CTRL_SRC_INPUT_OFFSET |
			CHNL_CTRL_SRC_TYPE_MEMORY << CHNL_CTRL_SRC_TYPE_OFFSET|
			CHNL_CTRL_CHNL_EN_ENABLE << CHNL_CTRL_CHNL_EN_OFFSET);
	writel(reg, mxc_isi->regs + CHNL_CTRL);

	mxc_isi_clean_registers(mxc_isi);
	mxc_isi_enable_irq(mxc_isi);
	mxc_isi_m2m_start_read(mxc_isi);

	dump_isi_regs(mxc_isi);
}

void mxc_isi_m2m_config_src(struct mxc_isi_dev *mxc_isi)
{
	struct mxc_isi_frame *src_f = &mxc_isi->m2m.src_f;
	u32 val;

	/* source format */
	val = readl(mxc_isi->regs + CHNL_MEM_RD_CTRL);
	val &= ~CHNL_MEM_RD_CTRL_IMG_TYPE_MASK;
	val |= src_f->fmt->color << CHNL_MEM_RD_CTRL_IMG_TYPE_OFFSET;
	writel(val, mxc_isi->regs + CHNL_MEM_RD_CTRL);

	/* source image width and height */
	val = (src_f->width << CHNL_IMG_CFG_WIDTH_OFFSET |
		   src_f->height << CHNL_IMG_CFG_HEIGHT_OFFSET);
	writel(val, mxc_isi->regs + CHNL_IMG_CFG);

	/* source pitch */
	val = src_f->bytesperline[0] << CHNL_IN_BUF_PITCH_LINE_PITCH_OFFSET;
	writel(val, mxc_isi->regs + CHNL_IN_BUF_PITCH);
}

void mxc_isi_m2m_config_dst(struct mxc_isi_dev *mxc_isi)
{
	struct mxc_isi_frame *dst_f = &mxc_isi->m2m.dst_f;
	u32 val;

	/* out format */
	val = readl(mxc_isi->regs + CHNL_IMG_CTRL);
	val &= ~CHNL_IMG_CTRL_FORMAT_MASK;
	val |= dst_f->fmt->color << CHNL_IMG_CTRL_FORMAT_OFFSET;
	writel(val, mxc_isi->regs + CHNL_IMG_CTRL);

	/* out pitch */
	val = readl(mxc_isi->regs + CHNL_OUT_BUF_PITCH);
	val &= ~CHNL_IN_BUF_PITCH_LINE_PITCH_MASK;
	val |= dst_f->bytesperline[0] << CHNL_OUT_BUF_PITCH_LINE_PITCH_OFFSET;
	writel(val, mxc_isi->regs + CHNL_OUT_BUF_PITCH);
}

void mxc_isi_m2m_start_read(struct mxc_isi_dev *mxc_isi)
{
	u32 val;

	val = readl(mxc_isi->regs + CHNL_MEM_RD_CTRL);
	val &= ~ CHNL_MEM_RD_CTRL_READ_MEM_MASK;
	writel(val, mxc_isi->regs + CHNL_MEM_RD_CTRL);
	udelay(300);

	val |= CHNL_MEM_RD_CTRL_READ_MEM_ENABLE << CHNL_MEM_RD_CTRL_READ_MEM_OFFSET;
	writel(val, mxc_isi->regs + CHNL_MEM_RD_CTRL);
}
