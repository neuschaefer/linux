/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*
 * remove memory debugger for lint free
 */
#define __MEMDBG_H

#include "x_hal_5381.h"
#include "x_bim.h"
#include "jpg_common.h"
#include "jpg_if.h"
#include "jpg_hw.h"
#include "jpg_debug.h"
#include "x_hal_926.h"
#include "x_bim.h"

#ifdef JPEG_AUTO_TEST
extern INT32 _JpgVerifyCmdGetParam(INT32 i4ParamType, INT32 Param);
#endif

#define D_VLD_POLL_DELAY			5
#define D_VLD_POLL_TIMEOUT			300


/* vld register clear value, by default, zero */
#define VLD_CLEAR					0

/* barrel shifter base, and bit shift address */
#define VLD_BARREL_SHIFTER_BITS(BITS) ((BITS) << 2)

/* vld register address/value definition*/
#define VLDREG_RISC_PROCESS		0x8C
	#define VLD_INIT_FETCH_DRAM		(1 << 20)
	#define VLD_INIT_FILL_BS		(1 << 23)

#define VLDREG_PWRSAVE		0xC4

#define VLDREG_FETCH_FINISH		0xE8
	#define VLD_INIT_FETCH_FIN		1

#define VLDREG_BS_PARSED_FLAG		0x104
	#define VLD_USERDEF_QMATRIX	0x300

#define VLDREG_INPUT_WINDOW     (0xF0)

#define VLDREG_VID_BUF_CTL_SIGNAL   (0x0F4)
#ifdef JPEG_VLD_V3
	#define VLD_SRAM_AA_FIT_TAR_SCLK    1
	#define VLD_SRAM_AA_FIT_TAR_D       (1 << 1)
#else
	#define VLD_SRAM_AA_FIT_TAR_SCLK    (1 << 16)
	#define VLD_SRAM_AA_FIT_TAR_D       (1 << 17)
#endif	

#define VLDREG_VIDEO_RPTR       (0xFC)

#define VLDREG_WAIT_THRES       (0x100)

#define VLDREG_MCU_RESTART		0x014C
	#define VLD_MCU_RESTART			0x1
	#define VLD_RES_DC_PRED			0x101
	#define VLD_RES_MKR_FND			0x10101

/* huffman related registers */
#define VLDREG_LA_HUFF_INDEX	0x0124
#define VLDREG_LA_HUFF_DATA		0x0128
#define VLDREG_MAXCODE			0x0118

/* component info */
#define VLDREG_BLK_PARAM_LIST	0x0138
	#define VLD_DCNEED_MASK			0x000003FF
	#define VLD_DCNEED_SHIFT		0
	#define VLD_ACNEED_MASK			0x000003FF
	#define VLD_ACNEED_SHIFT		10
	#define VLD_MCUBLK_MASK			0x0000000F
	#define VLD_MCUBLK_SHIFT		24

/* block membership (blongs to which component) */
#define VLDREG_MCU_MEMBERSHIP	0x013C

#define VLDREG_DC_TBL_LIST		0x0130
#define VLDREG_AC_TBL_LIST		0x0134

/* q table for each component */
#define VLDREG_Q_TBL_LIST		0x0120

#define VLDREG_JPEG_SWITCH		0x0144
	#define VLD_MPEG_MODE		0
	#define VLD_JPEG_MODE		1	
	#define VLD_INTMASK         4
	#define VLD_ERRINT_OFF		6		

#define VLDREG_FAST_START_DECODE	0x0148
	#define VLD_CLEAN_INTERNAL_BUFFER		0
	#define VLD_DONT_CLEAN_INTERNAL_BUFFER	1

#define VLDREG_PROGRESSIVE_MODE		0x0150
	#define VLD_PROG_NONE		    0
	#define VLD_PROG_DC_REFINE		1
	#define VLD_PROG_AC_FIRST		2
	#define VLD_PROG_AC_REFINE		3
	#define VLD_PROG_DC_FIRST		4
	#define VLD_PROG_MASK			0x7
	#define VLD_PROG_SHIFT			0
	/* progressive mode write out (write to idct if last scan) */
	#define VLD_PROG_COEF_WRITE		0
	#define VLD_PROG_IDCT_WRITE		1
	#define VLD_PROG_WRITE_MASK		1
	#define VLD_PROG_WRITE_SHIFT	3
	/* select baseline or progressive mode */
	#define VLD_DECODE_BASELINE		0
	#define VLD_DECODE_PROGRESSIVE	1
	#define VLD_DECODE_MODE_MASK	1
	#define VLD_DECODE_MODE_SHIFT	8

#define VLDREG_PROGRESSIVE_NONZERO_ONLY		0x0154
	#define VLD_PROG_BUFF_ALL		0
	#define VLD_PROG_BUFF_NZ_ONLY	1

#define VLDREG_PROGRESSIVE_ALAH		0x0158
	#define VLD_PROG_AL_MASK		0xf
	#define VLD_PROG_AL_SHIFT		0
	#define VLD_PROG_AH_MASK		0xf
	#define VLD_PROG_AH_SHIFT		8

#define VLDREG_PROGRESSIVE_SESS		0x015C
	#define VLD_PROG_SE_MASK		0x3f
	#define VLD_PROG_SE_SHIFT		0
	#define VLD_PROG_SS_MASK		0x3f
	#define VLD_PROG_SS_SHIFT		8

#define VLDREG_NONE_ZERO_HISTORY	0x0160

#define VLDREG_CURR_MCU				0x0164
#define VLDREG_EOB_RUN_WR			0x0168
#define VLDREG_EOB_RUN_RD			0x01A0

#define VLDREG_READ_START_COMP(X)	(0x016C + (4 * (X)))

#define VLDREG_WRITE_START_COMP(X)	(0x0178 + (4 * (X)))

#define VLDREG_X_ADDR_LIST			0x0184
#define VLDREG_Y_ADDR_LIST			0x0188

#define VLDREG_COMP_PITCH1			0x018C
#define VLDREG_COMP_PITCH2			0x0190
#define VLDREG_COMP_PITCH3			0x0194
	#define VLD_PITCH_LO_MASK		0x3FFF
	#define VLD_PITCH_LO_SHIFT		0
	#define VLD_PITCH_HI_MASK		0x3FFF
	#define VLD_PITCH_HI_SHIFT		14

#define VLDREG_START_DECODE			0x140
	#define VLD_DECODE_MCU			1

#define VLDREG_MCU_STATUS			0x198
	#define VLD_MCU_DECODE_FINISH	1

#define VLDREG_RISC_CTRL_WP		0x0110
    #define VLD_CTRL_PROG_EN    0x1
	#define VLD_RISC_CTRL_WP_EN	0x2
	#define VLD_WP_LATCH_EN		0x4
#define VLDREG_READ_POINTER		0x0B0
#define VLDREG_START_ADDRESS		0x0B4
#define VLDREG_END_ADDRESS		0x0B8

#define VLDREG_LATCH2				0x0D0
	#define LATCH2_SET				0x1
	
#define VLDREG_WATCH_RP			0x0FC
#define VLDREG_WATCH_WP			0x0F8

#define VLDREG_LOAD_INTRA_Q_MATRIX		0x0A0
#define VLDREG_LOAD_NONINTRA_Q_MATRIX	0x0A4

#define VLDREG_SOFT_RESET			0x0108
	#define VLD_RESET				1

#define VLDREG_DRAM_HUFF			0x011C
	#define VLD_AC_HUFF_FLAG		0x80000000

#define VLDREG_RESTART				0x014C
	#define VLD_RESTART				1
	#define VLD_RESTART_ALL			0x101
	#define VLD_RESTART_WORLD		0x10101
	#define VLD_JSTOR               24
	#define VLD_JSTOP_RDY           25

#define VLDREG_JPEG_FSM				0x0208
	#define VLD_JPEG_IDLE			0

#define VLDREG_DEC_MB_ROW           (0x2A8)
    #define VLD_DEC_FIRST_ROW       0x1
    #define VLD_DEC_NEXT_ROW        0x100

#define VLDREG_MB_ROW_DEC_SWITCH        (0x2AC)
    #define VLD_ROW_DEC_WR_ADDR                 0x100
    #define VLD_ROW_DEC_WR_BANK1_ADDR           0x200
    #define VLD_ROW_DEC_MCU_LEVEL               0x0
    #define VLD_ROW_DEC_MCU_ROW_LEVEL           0x1
    #define VLD_ROW_DEC_PIC_LEVEL               0x3
#define VLDREG_ROW_DEC_COMP0_ADDR       (0x2B0)
#define VLDREG_ROW_DEC_COMP1_ADDR       (0x2B4)
#define VLDREG_ROW_DEC_COMP2_ADDR       (0x2B8)
#define VLDREG_ROW_DEC_COMP0_ADDR_OFST  (0x2BC)
#define VLDREG_ROW_DEC_COMP1_ADDR_OFST  (0x2C0)
#define VLDREG_ROW_DEC_COMP2_ADDR_OFST  (0x2C4)

#define VLDREG_JPEG_PIC_SIZE            (0x2C8)
    #define VLD_ROW_DEC_WIDTH_SHIFT     16
    #define VLD_ROW_DEC_HEIGHT_SHIFT    0
    
#define VLDREG_ROW_PIC_DEC_FINISH       (0x2CC)
    #define VLD_ROW_DEC_PIC_FINISH      (0x1 << 8)
    #define VLD_ROW_DEC_MCU_ROW_FINISH  (0x1 << 0)

#define VLDREG_RESTART_INTERVAL     (0x2D0)
    #define VLD_RST_ENABLE          16

#define VLDREG_JPEG_SRAM_CLEAR      (0x2EC)

#define MCREG_SWAP_MODE				0x0090
	#define MC_SWAP_RASTER			3
#define MCREG_DRAM_TYPE				0x01C8
	#define MC_DRAM_RASTER			3

#define MCREG_STATUS				0x0894

#define VLDREG_JNEWBASE		        (0x3C4)
    #define VLD_NEW_JENGINE           0x7FF
    #define VLD_OLD_JENGINE           0x707

#define VLDREG_JFILLH                   (0x3D4)
    #define VLD_HUFF_DC_AC_SELECTION        12
    #define VLD_HUFF_TABLE_IDENTIFIER       8
    #define VLD_HUFF_TABLE_ADDR             0
#define VLDREG_JHVAL                    (0x3D8)

#define VLDREG_JED_EN    		    (0x3CC)
#define VLDREG_JEC_EN               (0x3D0)
#define VLDREG_JERR_STATUS		    (0x3DC)
    #define VLD_ERR_RSTMKR	            4			
    #define VLD_ERR_DCRR	            3
    #define VLD_ERR_ACSR	            2    
    #define VLD_ERR_GT64	            1    
    #define VLD_ERR_ACLEN	            0

#define BIG_LOOP 5000

#ifdef JPEG_VLD_V3
#ifdef VLD0_BASE
#undef VLD0_BASE
#define VLD0_BASE   JPG_VLD_BASE
#endif
#endif

#if !defined(D_JPEG_REG_DEBUG)
	#define VLD0_READ32(offset)				IO_READ32	(VLD0_BASE,	(offset))
	#define MC0_READ32(offset)				IO_READ32	(MC0_BASE,	(offset))
	#define VLD0_WRITE32(offset, value)		IO_WRITE32	(VLD0_BASE,	(offset), (value))
	#define MC0_WRITE32(offset, value)		IO_WRITE32	(MC0_BASE,	(offset), (value))
#else
	static __inline UINT32 VLD0_READ32(UINT32 u4Offset)
	{
		UINT32 u4Reg;
		u4Reg = IO_READ32(VLD0_BASE, (u4Offset));
		Printf("  - 0x%08x = VLD0_READ32(0x%03x)\n", u4Reg, (u4Offset));
//		Printf("r 0x%08x 1\n", (VLD0_BASE + (u4Offset - 0x400)));
		return u4Reg;
	}
	static __inline UINT32 MC0_READ32(UINT32 u4Offset)
	{
		UINT32 u4Reg;
		u4Reg = IO_READ32(MC0_BASE,	u4Offset);
		Printf("  - 0x%08x = MC0_READ32(0x%08x)\n", u4Reg, u4Offset);
		return u4Reg;
	}
	static __inline void VLD0_WRITE32(UINT32 u4Offset, UINT32 u4Value)
	{
		Printf("  - VLD0_WRITE32(0x%03x, 0x%08x)\n", (u4Offset), u4Value);
		IO_WRITE32(VLD0_BASE, (u4Offset), u4Value);
//		Printf("w 0x%08x 0x%08x\n", (VLD0_BASE + (u4Offset - 0x400)), u4Value);
	}
	static __inline void MC0_WRITE32(UINT32 u4Offset, UINT32 u4Value)
	{
		Printf("  - MC0_WRITE32(0x%8x, 0x%08x)\n", u4Offset, u4Value);
		IO_WRITE32(MC0_BASE, u4Offset, u4Value);
	}
#endif

#if defined(D_JPEG_HWFUNC_DEBUG)
	#define TRACE(MSG) Printf(MSG)
#else //D_JPEG_HWFUNC_DEBUG
	#define TRACE(MSG)
#endif //D_JPEG_HWFUNC_DEBUG

#if defined(D_JPEG_REG_DUMP)
	static void _jpg_DumpReg(void);
	#define DUMP_REG() _jpg_DumpReg()
#else
	#define DUMP_REG() \
	/* a safe do while (0)  */ \
	LINT_SUPPRESS_NEXT_STATEMENT(717) \
	do {} while (0)
#endif //D_JPEG_REG_DUMP


/***********************************************************
* static variable
***********************************************************/
static INT32 _i4JpgCurDecMode5391 = (INT32)E_JPG_DEC_MODE_BASELINE_MCU;
static BOOL _fgJpgHwDetectError = FALSE;

/***********************************************************
* local function prototypes
***********************************************************/

static INT32 _PollWaitFetchFinish(void);


/***********************************************************
* implementation
***********************************************************/
static INT32 _PollWaitFetchFinish(void)
{
	//UINT32 u4TimeAccu = 0;
	//UINT32 u4TimeAccu2 = 0;

	TRACE(("_PollWaitFetchFinish()\n"));

	while (! (VLD0_READ32(VLDREG_FETCH_FINISH) & VLD_INIT_FETCH_FIN));
	while ((! (VLD0_READ32(VLDREG_VID_BUF_CTL_SIGNAL) & VLD_SRAM_AA_FIT_TAR_SCLK)) && 
	       (! (VLD0_READ32(VLDREG_VID_BUF_CTL_SIGNAL) & VLD_SRAM_AA_FIT_TAR_D)));	
/*
	while (! (VLD0_READ32(VLDREG_FETCH_FINISH) & VLD_INIT_FETCH_FIN))
	{
		x_thread_delay(D_VLD_POLL_DELAY);
		u4TimeAccu += D_VLD_POLL_DELAY;
		if (u4TimeAccu >= D_VLD_POLL_TIMEOUT)
		{
			return -(INT32)E_HWJPG_ERR_TIMEOUT;
		}
	}

    // temp check -- to be deleted
	while ((! (VLD0_READ32(VLDREG_VID_BUF_CTL_SIGNAL) & VLD_SRAM_AA_FIT_TAR_SCLK)) && 
	       (! (VLD0_READ32(VLDREG_VID_BUF_CTL_SIGNAL) & VLD_SRAM_AA_FIT_TAR_D)))
	{
		x_thread_delay(D_VLD_POLL_DELAY);
		u4TimeAccu2 += D_VLD_POLL_DELAY;
		if (u4TimeAccu2 >= D_VLD_POLL_TIMEOUT)
		{
			return -(INT32)E_HWJPG_ERR_TIMEOUT;
		}
	}
*/	
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_BarrelShifterSkipBytes(UINT32 u4Bytes)
{
	UINT32 u4Dummy = 0;

	TRACE(("VLDJPG_BarrelShifterSkipBytes(%u)\n", u4Bytes));

	if (u4Bytes)
	{
		while (u4Bytes >= 4)
		{
			u4Dummy = VLD0_READ32(VLD_BARREL_SHIFTER_BITS(32));
			u4Bytes -= 4;
		}

		if (u4Bytes)
		{
			u4Dummy = VLD0_READ32(VLD_BARREL_SHIFTER_BITS(u4Bytes << 3));
		}
	}

	UNUSED(u4Dummy);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_SetRingBuffer(void *pvStartAddr, UINT32 u4ByteSize)
{
	UINT32 u4StartAddr;
	u4StartAddr = PHYSICAL((UINT32)pvStartAddr);
	if ((u4StartAddr & 0x3f) != 0)
	{
		return -(INT32)E_HWJPG_ERR_PARAM;
	}
	if ((u4ByteSize & 0x3f) != 0)
	{
		return -(INT32)E_HWJPG_ERR_PARAM;
	}

	VLD0_WRITE32(VLDREG_START_ADDRESS, (UINT32)(u4StartAddr >> 6));
	VLD0_WRITE32(VLDREG_END_ADDRESS, (UINT32)((u4StartAddr + u4ByteSize) >> 6));

	return (INT32)E_JPG_OK;
}

void VLDJPG_SetPowerOn(void)
{
	VLD0_WRITE32(VLDREG_PWRSAVE, (UINT32)0);
}

void VLDJPG_SetPowerOff(void)
{
	VLD0_WRITE32(VLDREG_PWRSAVE, (UINT32)3);
}

void VLDJPG_SetLinearBuffer(void)
{
	VLD0_WRITE32(VLDREG_START_ADDRESS, (UINT32)0);
	VLD0_WRITE32(VLDREG_END_ADDRESS, (UINT32)~0);
}

INT32 VLDJPG_SetBarrelShifter(UINT8 *pu1MemAddr)
{
	UINT32 u4NonCacheAddr;
	UINT32 u4NonAlignByteCount;

	TRACE(("VLDJPG_SetBarrelShifter(0x%08x)\n", pu1MemAddr));

	u4NonCacheAddr = PHYSICAL((UINT32)pu1MemAddr);
	VLD0_WRITE32(VLDREG_READ_POINTER, u4NonCacheAddr);
	VLD0_WRITE32(VLDREG_RISC_PROCESS, (UINT32)VLD_INIT_FETCH_DRAM);
	if ((INT32)E_HWJPG_OK != _PollWaitFetchFinish())
	{
		return -(INT32)E_HWJPG_ERR_SET_BS;
	}
	VLD0_WRITE32(VLDREG_RISC_PROCESS, (UINT32)VLD_INIT_FILL_BS);

	/* shift barrel shifter to non-128bit align address */
	u4NonAlignByteCount = (UINT32)pu1MemAddr & ALIGN_MASK(128);

	return VLDJPG_BarrelShifterSkipBytes(u4NonAlignByteCount);
}


void VLDJPG_SetRiscCtrlWP(UINT32 u4VldWptr)
{
	UINT32 u4RegVal;
//#ifdef CC_MT5387
#ifdef JPEG_VLD_V2
    u4RegVal = (u4VldWptr << 3) | (VLD_CTRL_PROG_EN);
#else
	u4RegVal = (u4VldWptr << 4) | (VLD_RISC_CTRL_WP_EN | VLD_WP_LATCH_EN);
#endif

#ifdef JPEG_VLD_V3
    VLD0_WRITE32(VLDREG_RISC_CTRL_WP, u4VldWptr);
    UNUSED(u4RegVal);
#else
	VLD0_WRITE32(VLDREG_RISC_CTRL_WP, u4RegVal);
#endif
}

INT32 VLDJPG_LoadTblQ(UINT8 u1Qn, UINT8 *pu1Qtbl)
{
	UINT32 u4Dummy;

	TRACE(("VLDJPG_LoadTblQ(%u, 0x%08x)\n", u1Qn, pu1Qtbl));

	if (u1Qn > 1)
	{
		// vld hardware currently supports only 2 quantization table
		// 5351A1 (351AB) 2005/1/16
		return -(INT32)E_HWJPG_ERR_QTBL_INDEX;
	}

	VLDJPG_SetLinearBuffer();

	if((INT32)E_HWJPG_OK != VLDJPG_SetBarrelShifter(pu1Qtbl))
	{
		return -(INT32)E_HWJPG_ERR_LOAD_Q;
	}

	if (u1Qn== 0)
	{
		u4Dummy = VLD0_READ32(VLDREG_LOAD_INTRA_Q_MATRIX);
	}
	else
	{
		u4Dummy = VLD0_READ32(VLDREG_LOAD_NONINTRA_Q_MATRIX);
	}

	UNUSED(u4Dummy);

	return (INT32)E_HWJPG_OK;
}


void VLDJPG_SetUserDefQMatrix(void)
{
	VLD0_WRITE32(VLDREG_BS_PARSED_FLAG, VLD_USERDEF_QMATRIX);
}


INT32 VLDJPG_ResetDcPrediction(void)
{
	TRACE(("VLDJPG_ResetDcPrediction()\n"));
	VLD0_WRITE32(VLDREG_MCU_RESTART, VLD_CLEAR);
	VLD0_WRITE32(VLDREG_MCU_RESTART, VLD_RES_DC_PRED);
	VLD0_WRITE32(VLDREG_MCU_RESTART, VLD_CLEAR);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_CalcDerivedHuffTable(const UINT8 *au1Bits, const UINT8 *au1HuffVal, DERIVED_HUFF_TABLE_T *prDerivedHuffTbl)
{
	//int numsymbols;
	INT32	i4BitLen, i4Symbol, i4LookBits, i4Ctr, i4Size, i4Loop1, i4Loop2;
	INT16	i2Bits;

	INT8	ai1HuffSize[257];
	UINT16	au2HuffCode[257];
	UINT16	u2Code;

	ASSERT(au1Bits != NULL);
	ASSERT(au1HuffVal != NULL);
	ASSERT(prDerivedHuffTbl != NULL);
	TRACE(("VLDJPG_CalcDerivedHuffTable(bits(0x%08x),val(0x%08x),dhufftbl(0x%08x))\n",au1Bits,au1HuffVal,prDerivedHuffTbl));

	/* Note that huffsize[] and huffcode[] are filled in code-length order,
	 * paralleling the order of the symbols themselves in htbl->huffval[].
	 */

	//x_memset(ai1HuffSize, 0, sizeof(ai1HuffSize));
	//x_memset(au2HuffCode, 0, sizeof(au2HuffCode));

	for (i4Loop1 = 0; i4Loop1 < 257; i4Loop1++)
	{
		ai1HuffSize[i4Loop1]=0;
		au2HuffCode[i4Loop1]=0;
	}	
	/* Figure C.1: make table of Huffman code length for each symbol */

	i4Symbol = 0;
	for (i4BitLen = 1; i4BitLen <= 16; i4BitLen++)
	{
		i2Bits = (INT16)au1Bits[i4BitLen];

		if ((i4Symbol + i2Bits) > 256) /* protect against table overrun */
		{
			// Error handler:
			// set to nearest value
			i2Bits = 255 - i4Symbol;	/* p + i = 255 */
		}

		while (i2Bits--)  // safe while
		{
			ai1HuffSize[i4Symbol++] = (INT8) i4BitLen;
		}
	}

	ai1HuffSize[i4Symbol] = 0;
	//numsymbols = p;

	/* Figure C.2: generate the codes themselves */
	/* We also validate that the counts represent a legal Huffman code tree. */

	i4Symbol	= 0;
	u2Code		= 0;
	i4Size		= ai1HuffSize[0];


	for (i4Loop1 = 0; i4Loop1 < BIG_LOOP; i4Loop1++)
	{	
	    if (i4Symbol > 256)
	    {
	        i4Symbol = 256;
	    }	    
		if (ai1HuffSize[i4Symbol])
		{
			for (i4Loop2 = 0; i4Loop2 < BIG_LOOP; i4Loop2++)
			{
				if (((INT32) ai1HuffSize[i4Symbol]) == i4Size)
				{
					au2HuffCode[i4Symbol++] = u2Code;
					u2Code++;
				}
				else
				{
					break;
				}
			}
			/* code is now 1 more than the last code used for codelength si; but
			* it must still fit in si bits, since no code is allowed to be all ones.
			*/

			if (((INT32) u2Code) >= (((INT32) 1) << i4Size))
			{
				// Error handler:
				// set to 0
				u2Code = 0;
			}

			u2Code <<= 1;
			i4Size++;
		}// huffsize[9]
		else
		{
			break;
		}
	}// BIG_LOOP

	/* Figure F.15: generate decoding tables for bit-sequential decoding */

	i4Symbol = 0;
	for (i4BitLen = 1; i4BitLen <= 16; i4BitLen++)
	{
		if (au1Bits[i4BitLen])
		{
			/* valoffset[l] = huffval[] index of 1st symbol of code length l,
			 * minus the minimum code of length l
			 */
			prDerivedHuffTbl->ai4Valoffset[i4BitLen] =
				(INT32)i4Symbol - (INT32)au2HuffCode[i4Symbol];
			i4Symbol += au1Bits[i4BitLen];
			if (!i4Symbol)
			{
			    i4Symbol = 1;
			    LOG(0, "Huffmantable warning");
			}
			prDerivedHuffTbl->ai4Maxcode[i4BitLen] =
				(INT32)au2HuffCode[i4Symbol - 1];		/* maximum code of length l */
		}
		else
		{
			prDerivedHuffTbl->ai4Valoffset[i4BitLen] = 0;
				/* clear to zero if no codes of this length */
			prDerivedHuffTbl->ai4Maxcode[i4BitLen] = -1;
				/* -1 if no codes of this length */
		}
	}
	prDerivedHuffTbl->ai4Maxcode[17] = (INT32)0xFFFFF;
		/* ensures jpeg_huff_decode terminates */

	/* Compute lookahead tables to speed up decoding.
	 * First we set all the table entries to 0, indicating "too long";
	 * then we iterate through the Huffman codes that are short enough and
	 * fill in all the entries that correspond to bit sequences starting
	 * with that code.
	 */

	for (i4Loop1 = 0; i4Loop1 < (1 << D_HUFF_LOOKAHEAD_BITS); i4Loop1++)
	{
		prDerivedHuffTbl->ai4LookNbits[i4Loop1] = 0;
	}

	i4Symbol = 0;
	for (i4BitLen = 1; i4BitLen <= D_HUFF_LOOKAHEAD_BITS; i4BitLen++)
	{
		for (i2Bits = 1; i2Bits <= (INT16) au1Bits[i4BitLen]; i2Bits++, i4Symbol++)
		{
			/* i4BitLen = current code's length, i4Symbol = its index in huffcode[] & huffval[]. */
			/* Generate left-justified code followed by all possible bit sequences */
			i4LookBits = (INT32)au2HuffCode[i4Symbol] << (D_HUFF_LOOKAHEAD_BITS - i4BitLen);
			for (i4Ctr = 1 << (D_HUFF_LOOKAHEAD_BITS - i4BitLen); i4Ctr > 0; i4Ctr--)
			{
				prDerivedHuffTbl->ai4LookNbits[i4LookBits] = i4BitLen;
				prDerivedHuffTbl->au1LookSym[i4LookBits] = au1HuffVal[i4Symbol];
				i4LookBits++;
			}
		}
	}

	return (INT32)E_HWJPG_OK;
}

// load ac look ahead table
INT32 VLDJPG_LoadAcLookAheadTable(UINT8 u1HuffTblNo, const DERIVED_HUFF_TABLE_T *prDerivedHuffTbl)
{
	INT32 i4LookAhead;

	ASSERT(prDerivedHuffTbl != NULL);
	
	TRACE(("VLDJPG_LoadAcLookAheadTable(%u, 0x%08x)\n", u1HuffTblNo, prDerivedHuffTbl));

	VLD0_WRITE32(VLDREG_LA_HUFF_INDEX, (UINT32) u1HuffTblNo);

	for (i4LookAhead = 0; i4LookAhead < 64; i4LookAhead++)
	{
		VLD0_WRITE32(VLDREG_LA_HUFF_DATA,
			(((UINT32)prDerivedHuffTbl->ai4LookNbits[i4LookAhead] << 8) |
			(UINT32)prDerivedHuffTbl->au1LookSym[i4LookAhead]));
	}

	VERIFY(VLDJPG_ResetDcPrediction() == (INT32)E_HWJPG_OK);

	return (INT32)E_HWJPG_OK;
}

// maxcode and valoffset
INT32 VLDJPG_LoadMaxcode(const INT32 *ai4Value)
{
	// decoder ap should load maxcode and valoffset follow this procedure

	INT32 i4Index;

	ASSERT(ai4Value != NULL);
	
	TRACE(("VLDJPG_LoadMaxcode(1..16)\n"));

	for (i4Index = 1; i4Index <= 16; i4Index++)
	{
		VLD0_WRITE32(VLDREG_MAXCODE, (UINT32)(ai4Value[i4Index]));
	}
	return (INT32)E_HWJPG_OK;
}

void VLDJPG_TransHuffTblToHwFormat(UINT8 *pu1DstAddr, const UINT8 *au1Huffval)
{
	UINT8 *pu1Huff;
	INT32 i4Index;

	ASSERT(au1Huffval != NULL);
	ASSERT(pu1DstAddr != NULL);

    TRACE(("VLDJPG_TransHuffTblToHwFormat(Dst:0x%08x, Huff:0x%08x)\n", pu1DstAddr, au1Huffval));

	pu1Huff = pu1DstAddr;

	for (i4Index = 0; i4Index < 256; (i4Index += 4), (pu1Huff += 4))
	{
		// every 128 bits, write only 32 bits data, rest 96 bits fill zero
		//x_memcpy(pu1Huff, &au1Huffval[i4Index], 4);
		pu1Huff[0] = au1Huffval[i4Index + 0];
		pu1Huff[1] = au1Huffval[i4Index + 1];
		pu1Huff[2] = au1Huffval[i4Index + 2];
		pu1Huff[3] = au1Huffval[i4Index + 3];
		//x_memset(pu1Huff + 4, 0, 12);
	}
	//HalFlushDCache();
#if defined(CC_MT5365) || defined(CC_MT5395)
    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(pu1DstAddr), (UINT32)(256));
#else
    HalFlushInvalidateDCache();
#endif
}

// huff value address
INT32 VLDJOG_SetHuffAddr(UINT8 *pu1DcHuff, UINT8 *pu1AcHuff)
{
	ASSERT(pu1DcHuff != NULL);
	ASSERT(pu1AcHuff != NULL);
	
	TRACE(("VLDJOG_SetHuffAddr(dc(0x%08x),ac(0x%08x))\n", pu1DcHuff, pu1AcHuff));
    
      if (((UINT32)pu1DcHuff & 15) || ((UINT32)pu1AcHuff & 15))
      {
            return -(INT32)E_HWJPG_ERR_HUFF_ADDR;
      }
      
	//dump huffman value
	/*{
		INT32 i4Index;

		LOG(6, "Dc huffval - \n");
		for (i4Index = 0; i4Index < 4096; i4Index++)
		{
			LOG(6, (const CHAR *)((i4Index != 0 && (i4Index % 16 == 0)) ? "\n" : " "));
			LOG(6, "%2x ", (UINT32)*((UINT8*)((UINT32)pu1DcHuff + i4Index)));
		}
		LOG(6, "\n\nAc huffval - \n");
		for (i4Index = 0; i4Index < 4096; i4Index++)
		{
			LOG(6, (const CHAR *)((i4Index != 0 && (i4Index % 16 == 0)) ? "\n" : " "));
			LOG(6, "%2x ", (UINT32)*((UINT8*)((UINT32)pu1AcHuff + i4Index)));
		}
		LOG(6, "\n\n");
	}*/
	//Christie 07/0307, huffman table limitation, 64M bound
      if(((UINT32)pu1DcHuff > (UINT32)0x4000000) ||((UINT32)pu1AcHuff > (UINT32)0x4000000)) //64M
      {
          Printf("Huffman Tbl Addr out of 64M\n");
      }

//	VLD0_WRITE32(VLDREG_DRAM_HUFF, (UINT32)pu1DcHuff >> 2);
//	VLD0_WRITE32(VLDREG_DRAM_HUFF, ((UINT32)pu1AcHuff >> 2) | VLD_AC_HUFF_FLAG);
	VLD0_WRITE32(VLDREG_DRAM_HUFF, (UINT32)pu1DcHuff);
	VLD0_WRITE32(VLDREG_DRAM_HUFF, ((UINT32)pu1AcHuff) | VLD_AC_HUFF_FLAG);

	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_SetHuffTblVal(UINT8 *au1Huffval, BOOL fgDCTbl)
{
    UINT32 u4Val, u4TblNum, u4I;
    UINT8 *pu1Huffval;

#ifdef __KERNEL__
    au1Huffval=(UINT8*)(VIRTUAL(au1Huffval));
#endif

    for (u4TblNum = 0; u4TblNum < D_MAX_JPEG_HUFF_TBL; u4TblNum++)
    {
        u4Val = u4TblNum << VLD_HUFF_TABLE_IDENTIFIER;
        if (!fgDCTbl)
        {
            u4Val |= (1 << VLD_HUFF_DC_AC_SELECTION);
        }
    
        VLD0_WRITE32(VLDREG_JFILLH, u4Val);

        pu1Huffval = au1Huffval + (u4TblNum << 8);

        for (u4I = 0; u4I < 256; u4I += 4)
        {
            u4Val = ((pu1Huffval[u4I + 3] << 24) | (pu1Huffval[u4I + 2] << 16) |
                    (pu1Huffval[u4I + 1] << 8) | pu1Huffval[u4I]);
            VLD0_WRITE32(VLDREG_JHVAL, u4Val);
        }
    }
    
    return (INT32)E_HWJPG_OK;
}	

// set dc needed flag (1bit) for each block (maximum 10)
INT32 VLDJPG_SetDcNeededList(UINT32 u4DcNeededList)
{
	UINT32 u4Reg;

	TRACE(("VLDJPG_SetDcNeededList(0x%08x)\n", u4DcNeededList));

	u4Reg = VLD0_READ32(VLDREG_BLK_PARAM_LIST);
	u4Reg = (u4Reg & ~(VLD_DCNEED_MASK << VLD_DCNEED_SHIFT)) |
		((u4DcNeededList & VLD_DCNEED_MASK) << VLD_DCNEED_SHIFT);
	VLD0_WRITE32(VLDREG_BLK_PARAM_LIST, u4Reg);

	return (INT32)E_HWJPG_OK;
}

// set ac needed flag (1bit) for each block (maximum 10)
INT32 VLDJPG_SetAcNeededList(UINT32 u4AcNeededList)
{
	UINT32 u4Reg;

	TRACE(("VLDJPG_SetAcNeededList(0x%08x)\n", u4AcNeededList));

	u4Reg = VLD0_READ32(VLDREG_BLK_PARAM_LIST);
	u4Reg = (u4Reg & ~(VLD_ACNEED_MASK << VLD_ACNEED_SHIFT)) | ((u4AcNeededList & VLD_ACNEED_MASK) << VLD_ACNEED_SHIFT);
	VLD0_WRITE32(VLDREG_BLK_PARAM_LIST, u4Reg);

	return (INT32)E_HWJPG_OK;
}

// set block count in an MCU
INT32 VLDJPG_SetBlockCount(UINT32 u4BlockCount)
{
	UINT32 u4Reg;

	TRACE(("VLDJPG_SetBlockCount(%u)\n", u4BlockCount));

	u4Reg = VLD0_READ32(VLDREG_BLK_PARAM_LIST);
	u4Reg = (u4Reg & ~(VLD_MCUBLK_MASK << VLD_MCUBLK_SHIFT)) | ((u4BlockCount & VLD_MCUBLK_MASK) << VLD_MCUBLK_SHIFT);
	VLD0_WRITE32(VLDREG_BLK_PARAM_LIST, u4Reg);

	return (INT32)E_HWJPG_OK;
}

// set membership register
INT32 VLDJPG_SetMemberShip(UINT32 u4Membership)
{
	// per block has 2 bits
	// member = 0;
	// for each block (index blkno, blongs to blk_comp[blkno])
	// 	member |= blk_comp[blkno]

	TRACE(("VLDJPG_SetMemberShip(0x%08x)\n", u4Membership));

	VLD0_WRITE32(VLDREG_MCU_MEMBERSHIP, u4Membership);
	return (INT32)E_HWJPG_OK;
}

// set dc & ac table for each block
// maximum 10 blocks (4,4,2) for mtk vld hardware
// each block can be one of table 0 1 2 3
INT32 VLDJPG_SetDcTableList(UINT32 u4DcTableList)
{
	TRACE(("VLDJPG_SetDcTableList(0x%08x)\n", u4DcTableList));

	VLD0_WRITE32(VLDREG_DC_TBL_LIST, u4DcTableList);
	return (INT32)E_HWJPG_OK;
}
INT32 VLDJPG_SetAcTableList(UINT32 u4AcTableList)
{
	TRACE(("VLDJPG_SetAcTableList(0x%08x)\n", u4AcTableList));

	VLD0_WRITE32(VLDREG_AC_TBL_LIST, u4AcTableList);
	return (INT32)E_HWJPG_OK;
}

// set q table for each component
INT32 VLDJPG_SetQuanTableList(UINT32 u4QuanTableList)
{
	TRACE(("VLDJPG_SetQuanTableList(0x%08x)\n", u4QuanTableList));

	VLD0_WRITE32(VLDREG_Q_TBL_LIST, u4QuanTableList);
	return (INT32)E_HWJPG_OK;
}

// switch VLD to jpeg decoding mode only when start jpeg decoding,
// after jpeg operation, switch back to mpeg
INT32 VLDJPG_Switch2Jpeg(void)
{
	TRACE(("VLDJPG_Switch2Jpeg()\n"));
	VLD0_WRITE32(VLDREG_JPEG_SWITCH, VLD_JPEG_MODE);
	return (INT32)E_HWJPG_OK;
}
INT32 VLDJPG_Switch2Mpeg(void)
{
	TRACE(("VLDJPG_Switch2Mpeg()\n"));
	VLD0_WRITE32(VLDREG_JPEG_SWITCH, VLD_MPEG_MODE);
	return (INT32)E_HWJPG_OK;
}

// clean buffer only when decode 1st mcu
// after 1st mcu, we can keep buffer for fast decode
INT32 VLDJPG_Switch2CleanBuffer(void)
{
	TRACE(("VLDJPG_Switch2CleanBuffer()\n"));
	VLD0_WRITE32(VLDREG_FAST_START_DECODE, VLD_CLEAN_INTERNAL_BUFFER);
	return (INT32)E_HWJPG_OK;
}
INT32 VLDJPG_Switch2KeepBuffer(void)
{
	TRACE(("VLDJPG_Switch2KeepBuffer()\n"));
	VLD0_WRITE32(VLDREG_FAST_START_DECODE, VLD_DONT_CLEAN_INTERNAL_BUFFER);
	return (INT32)E_HWJPG_OK;
}

// set decoding mode
INT32 VLDJPG_Switch2Baseline(void)
{
	TRACE(("VLDJPG_Switch2Baseline()\n"));
	VLD0_WRITE32(VLDREG_PROGRESSIVE_MODE, VLD_DECODE_BASELINE << VLD_DECODE_MODE_SHIFT);
	return (INT32)E_HWJPG_OK;
}
INT32 VLDJPG_Switch2Progressive(void)
{
	UINT32 u4Reg;
	TRACE(("VLDJPG_Switch2Progressive()\n"));
	u4Reg = VLD0_READ32(VLDREG_PROGRESSIVE_MODE);
	u4Reg = (u4Reg & (~(VLD_DECODE_MODE_MASK << VLD_DECODE_MODE_SHIFT))) | (VLD_DECODE_PROGRESSIVE << VLD_DECODE_MODE_SHIFT);
	VLD0_WRITE32(VLDREG_PROGRESSIVE_MODE, u4Reg);
	return (INT32)E_HWJPG_OK;
}


// set one of four progressive mode:
// ac_first, ac_refine, dc_first, dc_refine
INT32 VLDJPG_SetProgressiveMode(UINT8 u1ProgMode)
{
	UINT32 u4Reg;
	TRACE(("VLDJPG_SetProgressiveMode(%u)\n", u1ProgMode));
	u4Reg = VLD0_READ32(VLDREG_PROGRESSIVE_MODE);
	u4Reg = (u4Reg & (~(VLD_PROG_MASK << VLD_PROG_SHIFT))) | ((UINT32)u1ProgMode << VLD_PROG_SHIFT);
	VLD0_WRITE32(VLDREG_PROGRESSIVE_MODE, u4Reg);
	return (INT32)E_HWJPG_OK;
}


// indicate this is the last scan of a progressive image
INT32 VLDJPG_SetProgressiveLastScan(BOOL fgLastScan)
{
	UINT32 u4Reg;

	TRACE(("VLDJPG_SetProgressiveLastScan(%d)\n", fgLastScan));

	u4Reg = VLD0_READ32(VLDREG_PROGRESSIVE_MODE);
	u4Reg = (u4Reg & (~(VLD_PROG_WRITE_MASK << VLD_PROG_WRITE_SHIFT))) |
		((UINT32)fgLastScan << VLD_PROG_WRITE_SHIFT);
	VLD0_WRITE32(VLDREG_PROGRESSIVE_MODE, u4Reg);
	return (INT32)E_HWJPG_OK;
}

// TRUE to write only none zero history buffer (1 bit per pixel)
// FALSE to write to IDCT output buffer
INT32 VLDJPG_SetProgressiveNzOnly(BOOL fgWriteNzOnly)
{
	TRACE(("VLDJPG_SetProgressiveNzOnly(%d)\n", fgWriteNzOnly));
	VLD0_WRITE32(VLDREG_PROGRESSIVE_NONZERO_ONLY, (UINT32)fgWriteNzOnly);
	return (INT32)E_HWJPG_OK;
}

// specify Ah and Al in progressive decoding process
INT32 VLDJPG_SetProgressiveAhAl(UINT8 u1AhAl)
{
	TRACE(("VLDJPG_SetProgressiveAhAl(0x%x)\n", u1AhAl));
	VLD0_WRITE32(VLDREG_PROGRESSIVE_ALAH, ((u1AhAl & 0xf) << VLD_PROG_AL_SHIFT) | ((u1AhAl >> 4) << VLD_PROG_AH_SHIFT));
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_SetProgressiveSeSs(UINT8 u1Se, UINT8 u1Ss)
{
	TRACE(("VLDJPG_SetProgressiveSeSs(0x%x,0x%x)\n", u1Se, u1Ss));
	VLD0_WRITE32(VLDREG_PROGRESSIVE_SESS,
		((u1Se & VLD_PROG_SE_MASK) << VLD_PROG_SE_SHIFT) |
		((u1Ss & VLD_PROG_SS_MASK) << VLD_PROG_SS_SHIFT));
	return (INT32)E_HWJPG_OK;
}

// set none zero history buffer for progressive decoding process
INT32 VLDJPG_SetProgressiveNoneZeroHistory(UINT8 *pu1NoneZeroHistory)
{
	UINT32 u4NzHist;

	TRACE(("VLDJPG_SetProgressiveNoneZeroHistory(0x%08x)\n", pu1NoneZeroHistory));

	u4NzHist = (UINT32)pu1NoneZeroHistory;
	VLD0_WRITE32(VLDREG_NONE_ZERO_HISTORY, u4NzHist);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_SetCurrMCUNum(UINT32 u4CurrMCUNum)
{
	TRACE(("VLDJPG_SetCurrMCUNum(%u)\n", u4CurrMCUNum));

	VLD0_WRITE32(VLDREG_CURR_MCU, u4CurrMCUNum);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_SetCurrMcuRowNum5391(UINT32 u4CurrMcuRowNum)
{
	TRACE(("---- VLDJPG_SetCurrMcuRowNum5391(%u) ----\n", u4CurrMcuRowNum));

//	VLD0_WRITE32(VLDREG_CURR_MCU, u4CurrMcuRowNum);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_SetEndOfBandRun(UINT32 u4EndOfBandRun)
{
	TRACE(("VLDJPG_SetEndOfBandRun(%u)\n", u4EndOfBandRun));

	VLD0_WRITE32(VLDREG_EOB_RUN_WR, u4EndOfBandRun);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_GetEndOfBandRun(UINT32 *pu4EndOfBandRun)
{
	if (pu4EndOfBandRun == NULL)
	{
		return -(INT32)E_HWJPG_ERR_PARAM;
	}
	*pu4EndOfBandRun = VLD0_READ32(VLDREG_EOB_RUN_RD);

	TRACE(("VLDJPG_GetEndOfBandRun(%u)\n", *pu4EndOfBandRun));

	return (INT32)E_HWJPG_OK;
}

// read address of components
INT32 VLDJPG_SetCompReadAddr(UINT32 u4Comp, void *avCompReadAddr)
{
	ASSERT(u4Comp < D_MAX_JPEG_HW_COMP);

	TRACE(("VLDJPG_SetCompReadAddr(%u, 0x%08x)\n", u4Comp, avCompReadAddr));

	VLD0_WRITE32(VLDREG_READ_START_COMP(u4Comp), (UINT32)avCompReadAddr >> 2);
	return (INT32)E_HWJPG_OK;
}

// write address of components
INT32 VLDJPG_SetCompWriteAddr(UINT32 u4Comp, void *avCompWriteAddr)
{
	ASSERT(u4Comp < D_MAX_JPEG_HW_COMP);

	TRACE(("VLDJPG_SetCompWriteAddr(%u, 0x%08x)\n", u4Comp, avCompWriteAddr));

	VLD0_WRITE32(VLDREG_WRITE_START_COMP(u4Comp), (UINT32)avCompWriteAddr >> 2);
	return (INT32)E_HWJPG_OK;
}

// write x address list and y address list for each block
// each block has 3 bits
INT32 VLDJPG_SetBlockAddrList(const UINT32 *au4XAddrList, const UINT32 *au4YAddrList)
{
	INT32 i4Blk;
	UINT32 u4XAddrList = 0, u4YAddrList = 0;
	
	ASSERT(au4XAddrList != NULL);
	ASSERT(au4YAddrList != NULL);

	for (i4Blk = 0; i4Blk < 10; i4Blk++)
	{
		u4XAddrList |= (au4XAddrList[i4Blk] & 0x7) << (i4Blk * 3);
		u4YAddrList |= (au4YAddrList[i4Blk] & 0x7) << (i4Blk * 3);
	}

	TRACE(("VLDJPG_SetBlockAddrList(0x%08x, 0x%08x)\n", u4XAddrList, u4YAddrList));
	VLD0_WRITE32(VLDREG_X_ADDR_LIST, u4XAddrList);
	VLD0_WRITE32(VLDREG_Y_ADDR_LIST, u4YAddrList);

	return (INT32)E_HWJPG_OK;
}

// write idct pitch of each component
INT32 VLDJPG_SetCompIdctPitch(const UINT32 *au4CompPitch)
{
	UINT32 u4Reg;

	ASSERT(au4CompPitch != NULL);

if ((VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ) ||
    (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ))
{
	TRACE(("VLDJPG_SetCompIdctPitch(%u, %u, %u)\n", 
	    au4CompPitch[0], au4CompPitch[0], au4CompPitch[0]));

	VLD0_WRITE32(VLDREG_COMP_PITCH1,
		(((au4CompPitch[0] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT) |
		(((au4CompPitch[0] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT));

	u4Reg = VLD0_READ32(VLDREG_COMP_PITCH2);
	u4Reg = (u4Reg & ~(VLD_PITCH_LO_MASK << VLD_PITCH_LO_SHIFT)) |
		(((au4CompPitch[0] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT);
	VLD0_WRITE32(VLDREG_COMP_PITCH2, u4Reg);
}
else
{
	TRACE(("VLDJPG_SetCompIdctPitch(%u, %u, %u)\n", au4CompPitch[0], au4CompPitch[1], au4CompPitch[2]));

	VLD0_WRITE32(VLDREG_COMP_PITCH1,
		(((au4CompPitch[0] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT) |
		(((au4CompPitch[1] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT));

	u4Reg = VLD0_READ32(VLDREG_COMP_PITCH2);
	u4Reg = (u4Reg & ~(VLD_PITCH_LO_MASK << VLD_PITCH_LO_SHIFT)) |
		(((au4CompPitch[2] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT);
	VLD0_WRITE32(VLDREG_COMP_PITCH2, u4Reg);
}
	return (INT32)E_HWJPG_OK;
}

// write coef pitch of each component
INT32 VLDJPG_SetCompCoefPitch(const UINT32 *au4CoefPitch)
{
	UINT32 u4Reg;

	ASSERT(au4CoefPitch != NULL);

if ((VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ) ||
    (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ))
{
	TRACE(("VLDJPG_SetCompCoefPitch(%u, %u, %u)\n", 
	    au4CoefPitch[0], au4CoefPitch[0], au4CoefPitch[0]));

	u4Reg = VLD0_READ32(VLDREG_COMP_PITCH2);
	u4Reg = (u4Reg & (~(VLD_PITCH_HI_MASK << VLD_PITCH_HI_SHIFT))) |
		(((au4CoefPitch[0] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT);
	VLD0_WRITE32(VLDREG_COMP_PITCH2, u4Reg);

	VLD0_WRITE32(VLDREG_COMP_PITCH3,
		(((au4CoefPitch[0] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT) |
		(((au4CoefPitch[0] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT));
}
else
{
	TRACE(("VLDJPG_SetCompCoefPitch(%u, %u, %u)\n", au4CoefPitch[0], au4CoefPitch[1], au4CoefPitch[2]));

	u4Reg = VLD0_READ32(VLDREG_COMP_PITCH2);
	u4Reg = (u4Reg & (~(VLD_PITCH_HI_MASK << VLD_PITCH_HI_SHIFT))) |
		(((au4CoefPitch[0] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT);
	VLD0_WRITE32(VLDREG_COMP_PITCH2, u4Reg);

	VLD0_WRITE32(VLDREG_COMP_PITCH3,
		(((au4CoefPitch[1] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT) |
		(((au4CoefPitch[2] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT));
}
	return (INT32)E_HWJPG_OK;
}

#define D_JPEG_HW_WAIT	0x200000
#define D_JPEG_HW_WAITPIC	(0x200000 * 10)
#define LED_ADDR		0x11000000      // hardcoded, fixme
// start decode process (mcu mode)
INT32 VLDJPG_DecodeMcu(void)
{
	UINT32 u4Wait;

	TRACE(("VLDJPG_DecodeMcu()\n"));

	DUMP_REG();
	VLD0_WRITE32(VLDREG_START_DECODE, VLD_DECODE_MCU);
	/*
	while((IO_READ32(VLD0_BASE, VLDREG_MCU_STATUS) & 1) != 0)
	{
		SetLed((u2Loop++ & 0xff));
	}
	*/
	VLD0_WRITE32(VLDREG_START_DECODE, VLD_CLEAR);
	for (u4Wait = 1;
		(u4Wait < D_JPEG_HW_WAIT) &&
			((VLD0_READ32(VLDREG_MCU_STATUS) & 1) == 0);
		u4Wait++)
	{
		//HAL_WRITE8(LED_ADDR, u4Wait >> 8);
	}
	DUMP_REG();
	if (u4Wait != D_JPEG_HW_WAIT)
	{
		//BIM_WRITE32(REG_SEG7, (UINT16)_u2McuCnt++);
		return (INT32)E_HWJPG_OK;
	}

	return -(INT32)E_JPG_ERR_FATAL;
}

// start decode process (mcu-ROW mode)
INT32 VLDJPG_DecodeFirstMcuRow5391(void)
{
	UINT32 u4Wait;

	TRACE(("VLDJPG_DecodeFirstMcuRow5391()\n"));

	DUMP_REG();
	VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_DEC_FIRST_ROW);
	/*
	while((IO_READ32(VLD0_BASE, VLDREG_MCU_STATUS) & 1) != 0)
	{
		SetLed((u2Loop++ & 0xff));
	}
	*/
	VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_CLEAR);
	for (u4Wait = 1;
		(u4Wait < D_JPEG_HW_WAIT) &&
			((VLD0_READ32(VLDREG_ROW_PIC_DEC_FINISH) & VLD_ROW_DEC_MCU_ROW_FINISH) == 0);
		u4Wait++)
	{
		//HAL_WRITE8(LED_ADDR, u4Wait >> 8);
	}
	DUMP_REG();
	if (u4Wait != D_JPEG_HW_WAIT)
	{
		//BIM_WRITE32(REG_SEG7, (UINT16)_u2McuCnt++);
		return (INT32)E_HWJPG_OK;
	}

	return -(INT32)E_JPG_ERR_FATAL;
}

// start decode process (mcu-ROW mode)
INT32 VLDJPG_DecodeNextMcuRow5391(void)
{
	UINT32 u4Wait;

	TRACE(("VLDJPG_DecodeNextMcuRow5391()\n"));

	DUMP_REG();
	VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_DEC_NEXT_ROW);
	/*
	while((IO_READ32(VLD0_BASE, VLDREG_MCU_STATUS) & 1) != 0)
	{
		SetLed((u2Loop++ & 0xff));
	}
	*/
	VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_CLEAR);
	for (u4Wait = 1;
		(u4Wait < D_JPEG_HW_WAIT) &&
			((VLD0_READ32(VLDREG_ROW_PIC_DEC_FINISH) & VLD_ROW_DEC_MCU_ROW_FINISH) == 0);
		u4Wait++)
	{
		//HAL_WRITE8(LED_ADDR, u4Wait >> 8);
	}
	DUMP_REG();
	if (u4Wait != D_JPEG_HW_WAIT)
	{
		//BIM_WRITE32(REG_SEG7, (UINT16)_u2McuCnt++);
		return (INT32)E_HWJPG_OK;
	}

	return -(INT32)E_JPG_ERR_FATAL;
}

// start decode process (pic mode)
INT32 VLDJPG_DecodePic5391(void)
{
	UINT32 u4Wait;

	TRACE(("VLDJPG_DecodePic5391()\n"));

	DUMP_REG();
	VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_DEC_FIRST_ROW);
	/*
	while((IO_READ32(VLD0_BASE, VLDREG_MCU_STATUS) & 1) != 0)
	{
		SetLed((u2Loop++ & 0xff));
	}
	*/
	VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_CLEAR);
	for (u4Wait = 1;
		(u4Wait < D_JPEG_HW_WAITPIC) &&
			((VLD0_READ32(VLDREG_ROW_PIC_DEC_FINISH) & VLD_ROW_DEC_PIC_FINISH) == 0);
		u4Wait++)
	{
		//HAL_WRITE8(LED_ADDR, u4Wait >> 8);
	}
	DUMP_REG();
	if (u4Wait != D_JPEG_HW_WAITPIC)
	{
		//BIM_WRITE32(REG_SEG7, (UINT16)_u2McuCnt++);
		return (INT32)E_HWJPG_OK;
	}

	return -(INT32)E_JPG_ERR_FATAL;
}

// start decode process (pic mode, not polling in this api)
void VLDJPG_DecodePic(void)
{
	TRACE(("VLDJPG_DecodePic()\n"));

	DUMP_REG();
	VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_DEC_FIRST_ROW);

	VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_CLEAR);
}

// check decode finish status
// return 0 if finish
// return err if unfinish
INT32 VLDJPG_CheckDecodeFinish(void)
{
    if ((VLD0_READ32(VLDREG_ROW_PIC_DEC_FINISH) & VLD_ROW_DEC_PIC_FINISH))
    {
	return (INT32)E_HWJPG_OK;
    }
    else
    {
        return (INT32)E_HWJPG_BUSY;
    }
}

INT32 VLDJPG_SoftwareReset(void)
{
	TRACE(("VLDJPG_SoftwareReset()\n"));

	VLD0_WRITE32(VLDREG_SOFT_RESET, VLD_RESET);
	VLD0_WRITE32(VLDREG_SOFT_RESET, VLD_CLEAR);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_SramClear(void)
{
	TRACE(("VLDJPG_SramClear()\n"));

	VLD0_WRITE32(VLDREG_JPEG_SRAM_CLEAR, 0x0);
	VLD0_WRITE32(VLDREG_JPEG_SRAM_CLEAR, 0x100);
	VLD0_WRITE32(VLDREG_JPEG_SRAM_CLEAR, 0x0);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_SetRiscWaitThreshold(void)
{
	TRACE(("VLDJPG_SetRiscWaitThreshold()\n"));

	VLD0_WRITE32(VLDREG_WAIT_THRES, 0x4000);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_Restart()
{
	TRACE(("VLDJPG_Restart()\n"));

	VLD0_WRITE32(VLDREG_RESTART, VLD_RESTART);
	VLD0_WRITE32(VLDREG_RESTART, VLD_CLEAR);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_AdvanceRestartMarker(UINT8 *pu1Marker)
{
	UINT8 u1RestartMarker;

	TRACE(("VLDJPG_AdvanceRestartMarker()\n"));

	u1RestartMarker = (UINT8)((VLD0_READ32(VLDREG_MCU_STATUS) >> 8) & 0xff);
	if (pu1Marker)
	{
		*pu1Marker = u1RestartMarker;
	}
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_InitScan(void)
{
	TRACE(("VLDJPG_InitScan()\n"));

	VLD0_WRITE32(VLDREG_MCU_RESTART, VLD_CLEAR);
	VLD0_WRITE32(VLDREG_MCU_RESTART, VLD_RES_MKR_FND);
	VLD0_WRITE32(VLDREG_MCU_RESTART, VLD_CLEAR);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_CheckVideoInputWindow(void)
{
	TRACE(("VLDJPG_CheckVideoInputWindow()\n"));

	VLD0_READ32(VLDREG_INPUT_WINDOW);
	return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_CheckVideoReadPointer(void)
{
	TRACE(("VLDJPG_CheckVideoReadPointer()\n"));

	VLD0_READ32(VLDREG_VIDEO_RPTR);
	return (INT32)E_HWJPG_OK;
}

//==========================================================

// allocate an vld jpeg hardware instance
JPEGHANDLE VLDJPG_AllocInstance(void)
{
	JPEGHANDLE hInstance;
	JPEG_BASE_CLASS_T *prRootObject;
	JPEG_VLDJPG_DATA_T *prVldObject;
	#ifndef __KERNEL__
	static UINT8 au1DCTbl[(1024*4) + 15];
	static UINT8 au1ACTbl[(1024*4) + 15];
	#endif

	// allocate vld information instance
	hInstance = (JPEGHANDLE)x_mem_alloc(sizeof(JPEG_VLDJPG_DATA_T));
	if (hInstance == NULL)
	{
		x_mem_free(hInstance);
		return hInstance;
	}
	x_memset(hInstance, 0, sizeof(JPEG_VLDJPG_DATA_T));
	// init its root object
	prRootObject = (JPEG_BASE_CLASS_T *)hInstance;
	prRootObject->eJpegClass = E_JC_VLD_DECODER;
	prRootObject->zObjectSize = sizeof(JPEG_VLDJPG_DATA_T);

	// init huffman table buffer
	prVldObject = (JPEG_VLDJPG_DATA_T *)hInstance;
	
	#ifdef __KERNEL__		//for linux system	
	prVldObject->pu1HwDcHuffTbl = (UINT8 *)BSP_AllocAlignedDmaMemory(1024*4, 16);
	prVldObject->pu1HwAcHuffTbl = (UINT8 *)BSP_AllocAlignedDmaMemory(1024*4, 16);
	if((prVldObject->pu1HwDcHuffTbl == NULL) || (prVldObject->pu1HwAcHuffTbl == NULL))
	{
		x_mem_free(hInstance);
		return NULL;
	}
	
	#else					//for nucleus system
	prVldObject->pu1HwDcHuffTbl = (UINT8*)PHYSICAL(((UINT32)au1DCTbl + 15) & (~15));
	prVldObject->pu1HwAcHuffTbl = (UINT8*)PHYSICAL(((UINT32)au1ACTbl + 15) & (~15));	
	#endif
	
	return hInstance;
}

// release an vld jpeg hardware instance
void VLDJPG_FreeInstance(JPEGHANDLE hInstance)
{
	#ifdef __KERNEL__		//for linux system	
	JPEG_VLDJPG_DATA_T *prVldObject;
	
	prVldObject = (JPEG_VLDJPG_DATA_T *)hInstance;
	if(prVldObject == NULL)
	{
		return;
	}
	if(prVldObject->pu1HwDcHuffTbl)
	{
		VERIFY(BSP_FreeAlignedDmaMemory(prVldObject->pu1HwDcHuffTbl));
	}
	if(prVldObject->pu1HwAcHuffTbl)
	{
		VERIFY(BSP_FreeAlignedDmaMemory(prVldObject->pu1HwAcHuffTbl));
	}
	#endif
	
	x_mem_free(hInstance);
}

// start hardware decoder, init data structure (allocate vld0)
INT32 VLDJPG_New(const JPEGHANDLE hInstance)
{
  UINT32 u4SysClock=5;
#ifdef JPEG_VLD_V2
  u4SysClock=5;//200
#elif defined(JPG_CLK_FROM_GRAPHIC)
  u4SysClock=1; // 240
#else
  u4SysClock=4;//162
#endif
#ifdef JPEG_AUTO_TEST
  u4SysClock=_JpgVerifyCmdGetParam(9,0);
#endif
  u4SysClock=u4SysClock&0x0f;
    // set mpg & jpg clock
  //IO_WRITE32(CKGEN_BASE, 0x224, u4SysClock);         //162 mhz for baseline-->1
#if defined(JPG_CLK_FROM_GRAPHIC)  //joshua
  IO_WRITE32(CKGEN_BASE, 0x230, u4SysClock);        //from gra_clk    
#else
  IO_WRITE32(CKGEN_BASE, 0x224, u4SysClock);         //from sys_clk    
#endif

//#ifndef CC_MT5392B
	VLDJPG_SetPowerOn();
//#endif
	VERIFY(VLDJPG_SoftwareReset() == (INT32)E_HWJPG_OK);
    VLDJPG_SramClear();

    // ????
//    VLD0_WRITE32((0x12C + 0x400), 0xffffffff);
    
	MC0_WRITE32(MCREG_SWAP_MODE, 0x0);
	MC0_WRITE32(MCREG_DRAM_TYPE, MC_DRAM_RASTER);
	
	// ????
//	MC0_WRITE32(0x98, 1);
//	VLD0_WRITE32(VLDREG_LATCH2, LATCH2_SET);
//#ifdef CC_MT5387
#ifdef JPEG_VLD_V2
    VLD0_WRITE32(VLDREG_LATCH2, 0x01000001);
#else
    //LOG(3,"--- 536x test ---\n");
	VLD0_WRITE32(VLDREG_LATCH2, 0x010E0103);
#endif	

    VLDJPG_SetRiscWaitThreshold();
	UNUSED(hInstance);
	return VLDJPG_Switch2Jpeg();
}


#if defined(D_JPEG_REG_DUMP)
static void _jpg_DumpReg(void)
{
	UINT8  ucIdx;
  	UINT32  u4RegVal;


  for (ucIdx = 34; ucIdx <= 39; ucIdx++)
  {
	u4RegVal = VLD0_READ32(4*ucIdx);
	Printf("VLD Reg %4d (0x%03x)= %08x \n", ucIdx , 4*ucIdx, u4RegVal);
  }


  for (ucIdx = 42; ucIdx <= 112; ucIdx++)
  {
	u4RegVal = VLD0_READ32(4*ucIdx);
	Printf("VLD Reg %4d (0x%03x)= %08x \n", ucIdx , 4*ucIdx, u4RegVal);
  }

  for (ucIdx = 0; ucIdx <= 43; ucIdx++)
  {
  	u4RegVal = MC0_READ32(4*ucIdx);
	Printf(" MC Reg %4d (0x%03x)= %08x \n", ucIdx, 4*ucIdx, u4RegVal);
  }

    u4RegVal = MC0_READ32(4*114);
    Printf(" MC Reg  114 (0x1c8) = %08x \n", u4RegVal);

}
#endif //D_JPEG_REG_DUMP


//////////////////////////////////////////////////////////////
//
// Picture Mode (MCU Row Mode) Related Function
//
//////////////////////////////////////////////////////////////
// read address of components
INT32 VLDJPG_SetCompReadAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr)
{
	TRACE(("VLDJPG_SetCompReadAddr5391(%u, 0x%08x)\n", 0, u4Comp0Addr));
	TRACE(("VLDJPG_SetCompReadAddr5391(%u, 0x%08x)\n", 1, u4Comp1Addr));
	TRACE(("VLDJPG_SetCompReadAddr5391(%u, 0x%08x)\n", 2, u4Comp2Addr));

	VLD0_WRITE32(VLDREG_READ_START_COMP(0), (u4Comp0Addr >> 2));
	VLD0_WRITE32(VLDREG_READ_START_COMP(1), (u4Comp1Addr >> 2));
	VLD0_WRITE32(VLDREG_READ_START_COMP(2), (u4Comp2Addr >> 2));
	return (INT32)E_HWJPG_OK;
}

// write address of components
INT32 VLDJPG_SetCompWriteAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr)
{
	TRACE(("VLDJPG_SetCompWriteAddr5391(%u, 0x%08x)\n", 0, u4Comp0Addr));
	TRACE(("VLDJPG_SetCompWriteAddr5391(%u, 0x%08x)\n", 1, u4Comp1Addr));
	TRACE(("VLDJPG_SetCompWriteAddr5391(%u, 0x%08x)\n", 2, u4Comp2Addr));

	VLD0_WRITE32(VLDREG_WRITE_START_COMP(0), (u4Comp0Addr >> 2));
	VLD0_WRITE32(VLDREG_WRITE_START_COMP(1), (u4Comp1Addr >> 2));
	VLD0_WRITE32(VLDREG_WRITE_START_COMP(2), (u4Comp2Addr >> 2));
	return (INT32)E_HWJPG_OK;
}

void VLDJPG_SetOutputBank0BufAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr)
{
    UINT32 u4Value;

    TRACE(("VLDJPG_SetOutputBank0BufAddr5391(%u, 0x%08x)\n", 0, u4Comp0Addr));
    TRACE(("VLDJPG_SetOutputBank0BufAddr5391(%u, 0x%08x)\n", 1, u4Comp1Addr));
    TRACE(("VLDJPG_SetOutputBank0BufAddr5391(%u, 0x%08x)\n", 2, u4Comp2Addr));

    u4Comp0Addr = PHYSICAL(u4Comp0Addr);
    u4Comp1Addr = PHYSICAL(u4Comp1Addr);
    u4Comp2Addr = PHYSICAL(u4Comp2Addr);

    u4Value = VLD0_READ32(VLDREG_MB_ROW_DEC_SWITCH);
    u4Value |= VLD_ROW_DEC_WR_ADDR;
    u4Value &= ~(VLD_ROW_DEC_WR_BANK1_ADDR);

    VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP0_ADDR, (u4Comp0Addr >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP1_ADDR, (u4Comp1Addr >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP2_ADDR, (u4Comp2Addr >> 2));
}

void VLDJPG_SetOutputBank1BufAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr)
{
    UINT32 u4Value;

    TRACE(("VLDJPG_SetOutputBank1BufAddr5391(%u, 0x%08x)\n", 0, u4Comp0Addr));
    TRACE(("VLDJPG_SetOutputBank1BufAddr5391(%u, 0x%08x)\n", 1, u4Comp1Addr));
    TRACE(("VLDJPG_SetOutputBank1BufAddr5391(%u, 0x%08x)\n", 2, u4Comp2Addr));
    
    u4Comp0Addr = PHYSICAL(u4Comp0Addr);
    u4Comp1Addr = PHYSICAL(u4Comp1Addr);
    u4Comp2Addr = PHYSICAL(u4Comp2Addr);

    u4Value = VLD0_READ32(VLDREG_MB_ROW_DEC_SWITCH);
    u4Value |= VLD_ROW_DEC_WR_ADDR;
    u4Value |= VLD_ROW_DEC_WR_BANK1_ADDR;

    VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP0_ADDR, (u4Comp0Addr >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP1_ADDR, (u4Comp1Addr >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP2_ADDR, (u4Comp2Addr >> 2));
}

void VLDJPG_SetMCUWidth5391(UINT32 u4Comp0MCUWidth, UINT32 u4Comp1MCUWidth, UINT32 u4Comp2MCUWidth)
{
    UINT32 u4Value;

    TRACE(("VLDJPG_SetMCUWidth5391(%u, %u)\n", 0, u4Comp0MCUWidth));
    TRACE(("VLDJPG_SetMCUWidth5391(%u, %u)\n", 1, u4Comp1MCUWidth));
    TRACE(("VLDJPG_SetMCUWidth5391(%u, %u)\n", 2, u4Comp2MCUWidth));
    
    //set each component MCU width
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP0_ADDR_OFST, (u4Comp0MCUWidth >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP1_ADDR_OFST, (u4Comp1MCUWidth >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP2_ADDR_OFST, (u4Comp2MCUWidth >> 2));

    u4Value = VLD0_READ32(VLDREG_MB_ROW_DEC_SWITCH);
    u4Value &= ~(VLD_ROW_DEC_WR_ADDR | VLD_ROW_DEC_WR_BANK1_ADDR);
    VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
}

void VLDJPG_SetPicSize5391(UINT32 u4MCUsPerRow, UINT32 u4TotalRows)
{
    UINT32 u4Value = 0;

    TRACE(("VLDJPG_SetPicSize5391(%u, %u)\n", u4MCUsPerRow, u4TotalRows));

    u4Value = ((u4MCUsPerRow << VLD_ROW_DEC_WIDTH_SHIFT) | (u4TotalRows));
    VLD0_WRITE32(VLDREG_JPEG_PIC_SIZE, u4Value);
}

void VLDJPG_SetRestartInterval5391(UINT32 u4RestartInterval)
{
    TRACE(("VLDJPG_SetRestartInterval5391(%u)\n", u4RestartInterval));

#ifdef JPEG_VLD_V3
    VLD0_WRITE32(VLDREG_RESTART_INTERVAL, ((u4RestartInterval-1) & 0xFFFF) | (1 << VLD_RST_ENABLE));
#else
    VLD0_WRITE32(VLDREG_RESTART_INTERVAL, ((u4RestartInterval-1) & 0xFFFF));
#endif
}

void JPG_SetDecMode5391(INT32 i4DecMode)
{
    _i4JpgCurDecMode5391 = i4DecMode;
}

void VLDJPG_SetDecMode5391(INT32 i4DecMode)
{
    UINT32 u4Value;
    
    u4Value = VLD0_READ32(VLDREG_MB_ROW_DEC_SWITCH);
    u4Value &= ~(VLD_ROW_DEC_MCU_LEVEL | VLD_ROW_DEC_MCU_ROW_LEVEL | VLD_ROW_DEC_PIC_LEVEL);

    switch((ENUM_JPEG_DEC_MODE_T)i4DecMode)
    {
        case E_JPG_DEC_MODE_BASELINE_MCU:
        case E_JPG_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT:
        case E_JPG_DEC_MODE_PROGRESSIVE_MCU_ENHANCE:
            TRACE(("VLDJPG_SetDecMode5391( BASELINE_MCU )\n"));
            u4Value |= VLD_ROW_DEC_MCU_LEVEL;
            VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
            break;
        case E_JPG_DEC_MODE_BASELINE_MCU_ROW:
        case E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ:
            TRACE(("VLDJPG_SetDecMode5391( BASELINE_MCU_ROW )\n"));
            u4Value |= VLD_ROW_DEC_MCU_ROW_LEVEL;
            VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
            break;
        case E_JPG_DEC_MODE_BASELINE_PIC:
        case E_JPG_DEC_MODE_BASELINE_PIC_RZ:
            TRACE(("VLDJPG_SetDecMode5391( BASELINE_PIC )\n"));
            u4Value |= VLD_ROW_DEC_PIC_LEVEL;
            VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
            break;
        default:
            Printf("Error: No support i4DecMode = %d\n", i4DecMode);
            ASSERT(0);
            return;
    }
}

INT32 VLDJPG_GetDecMode5391(void)
{
    return _i4JpgCurDecMode5391;
}

void VLDJPG_UseNewEngine(void)
{
#ifdef JPEG_VLD_V3
    VLD0_WRITE32(VLDREG_JNEWBASE, VLD_NEW_JENGINE);
#endif
}

void VLDJPG_HwIsr(UINT16 u2Vector)
{
#ifdef JPEG_VLD_V3
    UINT32 u4Value;

    ASSERT(u2Vector == (UINT16)VECTOR_JPGDEC);
    if (BIM_ClearIrq((UINT32)VECTOR_JPGDEC))
    {
        LOG(9, "JPG IRQ\n");
        if (VLD0_READ32(VLDREG_ROW_PIC_DEC_FINISH) == 0)
        {
            u4Value = VLD0_READ32(VLDREG_JERR_STATUS);
            if (u4Value & (UINT32)(1 << VLD_ERR_ACLEN))
            {
                LOG(3, "*E* VLD_ERR_ACLEN\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_GT64))
            {
                LOG(3, "*E* VLD_ERR_GT64\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_ACSR))
            {
                LOG(3, "*E* VLD_ERR_ACSR\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_DCRR))
            {
                LOG(3, "*E* VLD_ERR_RSTMKR\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_RSTMKR))
            {
                LOG(3, "*E* VLD_ERR_RSTMKR\n");
            }
            _fgJpgHwDetectError = TRUE;
        }
    }
#endif   
    UNUSED(u2Vector);
}

void VLDJPG_SetErrorConcealment(void)
{
#ifdef JPEG_VLD_V3
    VLD0_WRITE32(VLDREG_JEC_EN, (1 << VLD_ERR_RSTMKR) | (1 << VLD_ERR_GT64));
#endif
}

void VLDJPG_SetErrorDetection(void)
{
#ifdef JPEG_VLD_V3
    x_os_isr_fct pfnOldIsr;
    UINT32 u4Value;
    static BOOL fgIRQ = FALSE;

    if (!fgIRQ)
    {
        VERIFY(x_reg_isr((UINT16)VECTOR_JPGDEC, VLDJPG_HwIsr, &pfnOldIsr) == OSR_OK);
        fgIRQ = TRUE;
    }
    
    u4Value = VLD0_READ32(VLDREG_JPEG_SWITCH);
    VLD0_WRITE32(VLDREG_JPEG_SWITCH, u4Value | (1 << VLD_INTMASK));

    VLD0_WRITE32(VLDREG_JED_EN, (1 << VLD_ERR_DCRR) | (1 << VLD_ERR_ACSR) | (1 << VLD_ERR_ACLEN));
#endif
}

INT32 VLDJPG_DetectError(void)
{
    if (_fgJpgHwDetectError)
    {
        _fgJpgHwDetectError = FALSE;
        return -(INT32)E_JPG_ERR_FATAL;
    }

    return (INT32)E_HWJPG_OK;
}

BOOL VLDJPG_CheckHwStop(void)
{
    UINT32 u4Value;

    u4Value = VLD0_READ32(VLDREG_WATCH_RP);
    x_thread_delay(1);
    if (u4Value == VLD0_READ32(VLDREG_WATCH_RP))        //wr ptr stop moving
    {
        return TRUE;
    }

    return FALSE;
}

INT32 VLDJPG_StopHw(void)
{    
#ifdef JPEG_VLD_V3    
    UINT32 u4Value;
    UINT32 u2Cnt = 0;

    u4Value = VLD0_READ32(VLDREG_RESTART);
    VLD0_WRITE32(VLDREG_RESTART, u4Value | (1 << VLD_JSTOR));

    while (!(VLD0_READ32(VLDREG_RESTART) & (1 << VLD_JSTOP_RDY)))
    {
        u2Cnt++;
        x_thread_delay(1);
        if (u2Cnt > (UINT16)100)
        {
            LOG(3, "*E* decoder still running\n");
            break;
        }
    }
#else           
    while (VLD0_READ32(VLDREG_WATCH_RP) != VLD0_READ32(VLDREG_WATCH_WP))
    {               
        if (VLDJPG_CheckHwStop())
        {
            break;
        }
    }
#endif
    return (INT32)E_HWJPG_OK;
}

INT32 VLDJPG_GetChipType(void)
{
#ifdef JPEG_VLD_V2
   return 1;
#else
   return 2;
#endif
}
