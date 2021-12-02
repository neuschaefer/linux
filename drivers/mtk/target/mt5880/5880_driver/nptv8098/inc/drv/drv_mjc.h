/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_mjc.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_MJC_H_
#define _DRV_MJC_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "hw_mjc.h"
#include "sv_const.h"
#include "typedef.h"
#include "video_def.h"
#include "nptv_if.h"
#include "drv_scaler.h"
#include "drv_di.h"
#include "drv_di_int.h"

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
#ifndef CC_FPGA
#define IO_ADDR_OFST IO_VIRT
#else
#define IO_ADDR_OFST 0x20000000
#endif

#define SKIP_BUILD_ERROR    1
#define MJC_SET_BUF_FULL    0
#define MJC_WA_OUT_LR3D_DLY 1
#define MJC_HALF_PR_240     0
#define MJC_FULL_PR         0      // IF : MT5396_AA:0.FULL PRV1 ,1.FULL PRV2  ELSE 0. FULL PRV0,1. FULL PRV1,2. FULL PRV2, 3. FULL PRV3


#ifndef CC_FPGA
#define MJC_FPGA            0
#else
#define MJC_FPGA            0
#endif

#define MJC_SP_LOG_FRMSCH (1 << 0)

//#define MJC_INIT_MBIST_FLOW
#define MJC_2ND_MBIST_CHK   0
#define MJC_DISP_VPOS_120_TARGET 1631 // 28 * (65536/1125)
#define MJC_DISP_VPOS_060_TARGET 1048 // 18 * (65536/1125)

#define MJC_INT_0           (1 << 0) // Adjusted input vsync end
#define MJC_INT_1           (1 << 1) // Output vsync end
#define MJC_INT_2           (1 << 2) // Input vsync end
#define MJC_INT_3           (1 << 3) // MJC frame processing finished

#ifdef CC_FPGA
#define MJC_READ_1B(u4Addr) u1DrvMJCRead1B(((u4Addr) + IO_ADDR_OFST))
#define MJC_READ_2B(u4Addr) u2DrvMJCRead2B(((u4Addr) + IO_ADDR_OFST))
#define MJC_READ_4B(u4Addr) u4DrvMJCRead4B(((u4Addr) + IO_ADDR_OFST))
#define MJC_READ_FLD(u4Addr, u4Fld) u4DrvMJCReadFldAlign(((u4Addr) + IO_ADDR_OFST), (u4Fld))
#else
#define MJC_READ_1B(u4Addr) u1IO32Read1B(((u4Addr) + IO_ADDR_OFST))
#define MJC_READ_2B(u4Addr) u2IO32Read2B(((u4Addr) + IO_ADDR_OFST))
#define MJC_READ_4B(u4Addr) u4IO32Read4B(((u4Addr) + IO_ADDR_OFST))
#define MJC_READ_FLD(u4Addr, u4Fld) IO32ReadFldAlign(((u4Addr) + IO_ADDR_OFST), (u4Fld))
#endif

#define vIO32MJCWriteFldAlign(reg32,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
    (Fld_ac(fld)==AC_FULLDW)?vIO32Write4B((reg32),(val)):( \
     vIO32Write4BMsk((reg32),((UINT32)(val)<<Fld_shft(fld)),Fld2Msk32(fld))) /*lint -restore */


#define vIO32MJCWriteFldMulti(reg32, list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	INT32 msk=(INT32)(list);\
	{UINT8 upk;\
	((UINT32)msk==0xffffffff)?vIO32Write4B(reg32,(list)),0:(\
	((UINT32)msk)?vIO32Write4BMsk(reg32,(list),((UINT32)msk)),0:0\
	);\
	}\
}/*lint -restore */


#define vVal32List(u4Val, list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	INT32 msk=(INT32)(list);\
	{UINT8 upk;\
	((UINT32)msk==0xffffffff)? ((u4Val)=(list)),0:(\
	((UINT32)msk)?((u4Val)=((u4Val)&((UINT32)~msk))|((list)&((UINT32)msk))),0:0\
	);\
	}\
}/*lint -restore */

#define V_Fld(u4Val, fld) ((u4Val)<<Fld_shft(fld))

// read field
#define R_Fld(u4Val, fld) (((u4Val)&Fld2Msk32(fld))>>Fld_shft(fld))

#define MJC_WRITE_1B(u4Addr, u1Value) vIO32Write1B(((u4Addr) + IO_ADDR_OFST), (u1Value))
#define MJC_WRITE_2B(u4Addr, u2Value) vIO32Write2B(((u4Addr) + IO_ADDR_OFST), (u2Value))
#define MJC_WRITE_4B(u4Addr, u4Value) vIO32Write4B(((u4Addr) + IO_ADDR_OFST), (u4Value))
#define MJC_WRITE_1BMSK(u4Addr, u1Value, u1Msk) vIO32Write1BMsk(((u4Addr) + IO_ADDR_OFST), (u1Value), (u1Msk))
#define MJC_WRITE_2BMSK(u4Addr, u2Value, u2Msk) vIO32Write2BMsk(((u4Addr) + IO_ADDR_OFST), (u2Value), (u2Msk))
#define MJC_WRITE_4BMSK(u4Addr, u4Value, u4Msk) vIO32Write4BMsk(((u4Addr) + IO_ADDR_OFST), (u4Value), (u4Msk))
//#define MJC_WRITE_FLD(u4Addr, u4Value, u4Fld) vIO32WriteFldAlign(((u4Addr) + IO_ADDR_OFST), (u4Value), (u4Fld))
//#define MJC_WRITE_FLD_MULTI(u4Addr, list) vIO32WriteFldMulti(((u4Addr) + IO_ADDR_OFST), (list))
#define MJC_WRITE_FLD(u4Addr, u4Value, u4Fld) vIO32MJCWriteFldAlign(((u4Addr) + IO_ADDR_OFST), (u4Value), (u4Fld))
#define MJC_WRITE_FLD_MULTI(u4Addr, list) vIO32MJCWriteFldMulti(((u4Addr) + IO_ADDR_OFST), (list))

#define MJC_REGTBL_END 0xffffffff

#define ALIGN_MASK(VAL, MSK) (((VAL) + (MSK)) & (~(MSK)))
#define ALIGN_64BYTE(val) (((val) + 0x0000003f) & (~0x0000003f))
#define ALIGN_128_CEIL(val) ALIGN_MASK((val), 127)
#define ALIGN_64_CEIL(val) ALIGN_MASK((val), 63)
#define ALIGN_32_CEIL(val) ALIGN_MASK((val), 31)
#define ALIGN_16_CEIL(val) ALIGN_MASK((val), 15)
#define ALIGN_8_CEIL(val) ALIGN_MASK((val), 7)
#define ALIGN_4_CEIL(val) ALIGN_MASK((val), 3)
#define ALIGN_2_CEIL(val) ALIGN_MASK((val), 1)

#define MJC_CTRL_RT_ADAPT   (1 << 0)
#define MJC_CTRL_LT_ADAPT   (1 << 1)
#define MJC_CTRL_RT_HFBACK  (1 << 2)
#define MJC_CTRL_LT_HFBACK  (1 << 3)
#define MJC_CTRL_RT_ON      (1 << 4)
#define MJC_CTRL_LT_ON      (1 << 5)
#define MJC_CTRL_FM_ON      (1 << 6)

#define MJC_MC_LF_HALF_FBK_ON  (1 << 1)
#define MJC_MC_RT_HALF_FBK_ON  (1 << 0)
#define MJC_MC_LF_HALF_FBK_OFF (0 << 1)
#define MJC_MC_RT_HALF_FBK_OFF (0 << 0)

enum
{
	MJC_FRAME_NO_DROP,
	MJC_PSEUDO_DROP_ALL,
	MJC_PSEUDO_DROP_ON_SCN_CHG, // default frame No drop
	MJC_FRAME_DROP_LIKE			// default frame drop
};

enum
{
	E_MJC_SELF_MODE,
	E_MJC_DISP_MODE
};

typedef enum {
    E_VDO_MJC_2D    = 0,
    E_VDO_MJC_FS    = 1,
    E_VDO_MJC_SBS   = 2,
    E_VDO_MJC_TAB   = 3,
    E_VDO_MJC_TTD   = 4, 
    E_VDO_MJC_SS    = 5,
    E_VDO_MJC_OTHER = 6
}E_VDO_MJC_IN;

typedef enum
{
    E_VD0_MJC_OUT_NoPR  = 0,
    E_VD0_MJC_OUT_HPR   = 1,
    E_VD0_MJC_OUT_FPR   = 2,
    E_VD0_MJC_OUT_OTHER = 3
}E_VDO_MJC_OUT;


#define MJC_DES_WIDTH       (704)   // user define : must 64 byte align
#define MJC_DES_HEIGHT      (16)    // user define
#define MJC_SUBSAMPLE       (32)    // user define

#define MJC_FRM_SCH_MODE	MJC_PSEUDO_DROP_ON_SCN_CHG


#define MJC_BIT_SEL Fld(3, 8, AC_MSKW10) //[10:8]
#define MJC_FLPMRR_SEL Fld(2, 6, AC_MSKB0) //[7:6]
#define MJC_COLOR_SEL Fld(2, 4, AC_MSKB0) //[5:4]
#define MJC_RSCNRDC_SEL Fld(2, 2, AC_MSKB0) //[3:2]
#define MJC_NFRM_SEL Fld(2, 0, AC_MSKB0) //[1:0]

#define MJC_RPT_MODE    (1 << 11)

#define MJC_DYN_MODE    (1 << 10)
#define MJC_Y10_MODE	(0 << 0) 	
#define MJC_Y08_MODE	(1 << 9) 	
#define MJC_C10_MODE	(0 << 0) 	
#define MJC_C08_MODE	(1 << 8) 	

#define MJC_422_MODE	(0 << 0) 	
#define MJC_420_MODE	(1 << 5) 	
#define MJC_444_MODE	(1 << 4) 	
#define MJC_422RPT_MODE	(1 << 6) 

#define MJC_NRSCN_MODE  (0 << 0)
#define MJC_RSCN_MODE   (1 << 3)
#define MJC_0RDC_MODE	(0 << 0) 	
#define MJC_1RDC_MODE	(1 << 2) 	

#define MJC_3FRM_MODE	(0 << 0) 	
#define MJC_2FRM_MODE	(1 << 0) 	
#define MJC_1FRM_MODE	(1 << 1) 	

enum
{
    MJC_Y10_C10_422_0RDC_3FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C10_422_1RDC_3FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C10_420_0RDC_3FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C10_420_1RDC_3FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C10_422_0RDC_2FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C10_422_1RDC_2FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C10_420_0RDC_2FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C10_420_1RDC_2FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C10_422_0RDC_1FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y10_C10_420_0RDC_1FRM = MJC_Y10_MODE | MJC_C10_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y10_C08_422_0RDC_3FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C08_422_1RDC_3FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C08_420_0RDC_3FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C08_420_1RDC_3FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y10_C08_422_0RDC_2FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C08_422_1RDC_2FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C08_420_0RDC_2FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C08_420_1RDC_2FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y10_C08_422_0RDC_1FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y10_C08_420_0RDC_1FRM = MJC_Y10_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y08_C08_422_0RDC_3FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y08_C08_422_1RDC_3FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y08_C08_420_0RDC_3FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_3FRM_MODE,
    MJC_Y08_C08_420_1RDC_3FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_3FRM_MODE,
    MJC_Y08_C08_422_0RDC_2FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y08_C08_422_1RDC_2FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y08_C08_420_0RDC_2FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_2FRM_MODE,
    MJC_Y08_C08_420_1RDC_2FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_1RDC_MODE | MJC_2FRM_MODE,
    MJC_Y08_C08_422_0RDC_1FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_422_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    MJC_Y08_C08_420_0RDC_1FRM = MJC_Y08_MODE | MJC_C08_MODE | MJC_420_MODE | MJC_0RDC_MODE | MJC_1FRM_MODE,
    FRC_Y10_C10_444 = MJC_Y10_MODE | MJC_C10_MODE | MJC_444_MODE,
    FRC_Y08_C08_444 = MJC_Y08_MODE | MJC_C08_MODE | MJC_444_MODE,
    FRC_Y10_C10_422RPT = MJC_Y10_MODE | MJC_C10_MODE | MJC_422RPT_MODE,
    FRC_Y08_C08_422RPT = MJC_Y08_MODE | MJC_C08_MODE | MJC_422RPT_MODE
};

enum
{
    FALLBACK_MODE_55,
    FALLBACK_MODE_64,
    FALLBACK_MODE_55P2
};

enum
{
    E_MJC_60HZ = 0,
    E_MJC_120HZ = 1,
    E_MJC_240HZ = 2
};

enum
{
    E_MJC_BYPASS = 0,
    E_MJC_ON = 1,
    E_MJC_OFF = 2
};

enum
{
    E_MJC_NoPR  = 0,
    E_MJC_HPR   = 1,
    E_MJC_FPR   = 2
};

enum
{
    E_MJC_NOHPR   = 0,
    E_MJC_HPR120  = 1,
    E_MJC_HPR60   = 2,
    E_MJC_HPR240  = 3,
    E_MJC_HPR2160 = 4
};

enum
{
    E_MJC_FPRV0 = 0,
	E_MJC_FPRV1 = 1,
	E_MJC_FPRV2 = 2,
	E_MJC_FPRV3 = 3
};

typedef enum
{
    E_MJC_INTP_UNKNOWN = 0,
    E_MJC_2D_60        = 1,
    E_MJC_2D_120       = 2,
    E_MJC_2D_240       = 3,
    E_MJC_3D_60        = 4, // 50Hz (2x), 60Hz (2.5x)
    E_MJC_3D_120       = 5, // 100Hz 44 mode (4x), 120Hz 64 mode (5x)
    E_MJC_TTD_120      = 6,
    E_MJC_TTD_240      = 7    
} MJC_FBK_INTP_MODE_T;

typedef enum
{
    E_MJC_EFFECT_OFF = 0,
    E_MJC_EFFECT_LOW,
    E_MJC_EFFECT_MIDDLE,
    E_MJC_EFFECT_HIGH,
    E_MJC_EFFECT_NUM
} E_MJC_EFFECT;

typedef enum
{
    E_MJC_DELAY_PSAUTO = 0,
    E_MJC_DELAY_UNMUTE,
    E_MJC_DELAY_DM_BW_CHK,
    E_MJC_DELAY_DM_CRC_CHK,
    E_MJC_DELAY_DM_START,
    E_MJC_DELAY_MAX
} E_MJC_DELAY_SET;

typedef enum
{
    E_MJC_DEMO_FULL = 0,    // demo off
    E_MJC_DEMO_COLUMN,      // split demo mode = 1
    E_MJC_DEMO_ROW,         // split demo mode = 2
    E_MJC_DEMO_WINDOW       // split demo mode = 3
} E_MJC_DEMO;

typedef enum
{
    E_MJC_RGN_NF = 0,       // intp = 6, hqf = 0
    E_MJC_RGN_HF = 1,       // intp = 6, hqf = 1
    E_MJC_RGN_QF = 2,       // intp = 6, hqf = 2
    E_MJC_RGN_RPT = 3       // intp = 0    
} E_MJC_DEMO_RGN_TYPE;

typedef struct
{
    UINT32  u2Addr;
    UINT32  u4Value;
    UINT32  u4Mask;
} MJC_REGTBL_T;

typedef struct
{
    UINT32  u4StartAddr;
    UINT32  u4EndAddr;
    UINT32  u4Width;
    UINT32  u4Height;
    UINT16  u2Mode;
    UINT16  u2DebugMode;
} MJC_DRAM_ALLOC_T;

typedef struct
{
    UINT16  u2InHStart;
    UINT16  u2InVStart;
    UINT16  u2InWidth;
    UINT16  u2InHeight;
    UINT16  u2OutHStart;
    UINT16  u2OutVStart;
    UINT16  u2OutWidth;
    UINT16  u2OutHeight;
    UINT16  u2OutHTotal;
    UINT16  u2OutVTotal;
    UINT8   fgUdp4;
    UINT8   u1DispVpos;
    MJC_CLIP_INFO_T rClipInfo;
} MJC_PIC_SIZE_T;

typedef struct
{
    UINT8   u1InFrmRate;
    UINT8   u1OutFrmRate;
    UINT8   u13DMode;
	UINT8   u1FHPRMode;
    UINT8   u1FrmSchMode;
    UINT8   u1PanelType;
} MJC_FRC_CONFIG_T;

typedef struct
{
    E_MJC_DEMO          eDemoMode;
    E_MJC_DEMO_RGN_TYPE arRgn[3];
    UINT16  u2BndLeft;
    UINT16  u2BndRight;
    UINT16  u2BndTop;
    UINT16  u2BndBottom;
} MJC_DEMO_CFG_T;

typedef struct
{
    UINT8   u1InPRFrameRate;
    UINT16  u2InPRHTotal;
    UINT16  u2InPRVTotal;
    UINT8   u1OutPRFrameRate;
    UINT16  u2OutPRHTotal;
    UINT16  u2OutPRVTotal;
}MJC_PR_CONFIG_T;


typedef struct
{
    UINT8   u1OnOff;

    UINT8   u1Dyn;
    UINT8   u1Rpt;
    
    UINT8   u1Dram444;
	UINT8   u1Dram422RPT;
    UINT8   u1Dram420;
    UINT8   u1Y8;
    UINT8   u1C8;

    UINT8   u1YDithEn;
    UINT8   u1CDithEn;
    UINT8   u1Flip;
    UINT8   u1Mirror;

    UINT8   u1TwoFrm;
    UINT8   u1OneFrm;
    UINT8   u1Rdc;
    UINT8   u1Rscn;
    
    UINT8   u1CtrlParam;
    UINT8   u1EffectParam;
    UINT8   u1UIOnOff;
    UINT8   u1PSAuto22OnOff;
    UINT8   u1PSAuto32OnOff;
    UINT8   fgIsInterlace;
    UINT8   fgPSCANDispMode;
	UINT8	fgFreeze;

    UINT16  u2Center;
    
    E_MJC_EFFECT eMjcEffect;
    MJC_PIC_SIZE_T arPicSize;
    MJC_FRC_CONFIG_T arFRCConfig;
	MJC_PR_CONFIG_T  arMJCPRConfig;
}MJC_PRM_T;


//typedef UINT32  HANDLE_T;

//typedef void (*PThreadFunc)(void *pvArgv);

typedef struct{
    PThreadFunc pThreadFunc;
    CHAR*       szThreadDescription;
} MJC_SP_THREAD;

typedef struct
{
    HANDLE_T* pVsyncSema;
    CHAR*     szVsyncSema;
} MJC_SP_VSYNC;

typedef struct
{
    PThreadFunc* ptrPfnVSyncFunc;
    CHAR*     szVsyncDesc;
} MJC_SP_VSYNC_T;

typedef struct
{
    UINT32 u4Addr;
    UINT8  u1Msb;
    UINT8  u1Lsb;
} MJC_SP_REG_T;

typedef struct
{
    UINT16 u2DramMode;
    char* strDramMode;
} MJC_DRAM_MODE_T;

typedef struct
{
    UINT16 u4SrcWidth;
    UINT16 u4SrcHeight;    
    UINT32 u4SrcAddr;    
    UINT16 u4DesWidth;
    UINT16 u4DesHeight;    
    UINT32 u4DesAddrL;    
    UINT32 u4DesAddrR;	
    UINT32 u4SrcStartX;
    UINT32 u4SrcStartY;
    UINT32 u4Subsample;
    UINT32 u4DumpSize;
    UINT8 u4Index;
} MJC_DUMP_INFO_T;


extern UINT8 fgMJCStatusPolling;
extern UINT8 u1TestDelayCnt;
extern MJC_SP_THREAD MJCSPThread[];
extern MJC_SP_VSYNC MJCSPVsync[];
extern MJC_SP_VSYNC_T MJCSPVsyncIsr[];

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
extern MJC_DRAM_ALLOC_T arMJCDramAlloc;
extern MJC_PRM_T arMJCPrm;
extern MJC_DEMO_CFG_T arMJCDemo;
extern UINT8 u1MJCFbckCtrl;
extern UINT32 u4MJCStatusPollingCount;
extern UINT8 u1MJCStatusCheck;

#ifdef CC_SCPOS_3DTV_SUPPORT 
extern UINT16 u2Delay2usTimes;
extern UINT8 u1LRSwitch;
#endif

extern UINT8 fgIsSupport120HzPanel(void);

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
UINT32 u4DrvMJCGetSpEn(void);
void vDrvMJCSetSpEn(UINT32 u4Msk, UINT32 u4Count);
void vDrvMJCClearSpEn(UINT32 u4Msk);
BOOL fgDrvMJCIsInitialized(void);
void vDrvMJCSetPsAutoTrigOn(void);
void vDrvMJCInit(UINT8 bForceInit);
void vDrvMJCResetSW(void);
void vDrvMJCInitISR(void);
void vDrvMJCInitPR(void);

void vDrvMJCISR(UINT16 u2Vector);
void vDrvMJCLoadRegTbl(MJC_REGTBL_T * prRegTbl);
void vDrvMJCSetClock(void);
UINT32 u4DrvMJCRemapMJCFbmMode(UINT32 u4FbmMode);
void vDrvMJCRunMbist(void);
void vDrvMJCSetSACtrlMode(UINT8 u1Mode);

void vDrvMJCSetDRAMAccessOnOff(BOOL fgOnOff, BOOL fgDRAM444);
void vDrvMJCSrmCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode);
void vDrvMJCSetBuf(const MJC_DRAM_ALLOC_T* prDramAlloc);
void vDrvMJCSetSystemMode(UINT32 u4MjcSystemMode);

void vDrvMJCSetDispMode(UINT32 fgOnOff);
void vDrvMJCSetFlipMirror(UINT32 u4FlipOnOff, UINT32 u4MirrorOnOff);
void vDrvMJCSet32PdInfoShift(UINT32 fgIsInterlace, UINT32 u4PSCANDisplay, UINT32 fgNRSDMode);

void vDrvMJCSetPSAutoOnOff(BOOL fgOnOff);
void vDrvMJCSetFrmSch(MJC_FRC_CONFIG_T * arMJCFRC);
void vDrvMJCFrmSchPRTCPatch( MJC_PIC_SIZE_T * arMJCPicSize);

void vDrvMJCSetPicSize(const MJC_PIC_SIZE_T * arMjcPicSize);
void vDrvMJCModeChangeDone(UINT32 u4Path);
void vDrvMJCModeChgProc(UINT32 u4Path);
void vDrvMJCSetOutputTiming(const MJC_PIC_SIZE_T * arMjcPicSize);
void vDrvMJCSetVsyncDelay(UINT32 u4InVStart, UINT32 u4OutVact);
UINT16 u2DrvMJCGetOutputHTotal(void);
UINT16 u2DrvMJCGetOutputVTotal(void);
UINT32 u4DrvMJCBufferInit(void);
void vDrvMJCGetSPReg(void);
UINT32 u4DrvMJCSetSPReg(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Msb, UINT32 u4Lsb);
void vDrvMJCSetMVSortOnOff(UINT32 u4OnOff);
void vDrvMJCRegStatusPolling(void *pvArgv);
void vDrvMJCSetDramStaMonitorCount(UINT32 u4Val);
UINT32 u4DrvMJCGetDramStaMonitorCount(void);
UINT32 u4DrvMJCGetVDist(void);
void vDrvMJCEnablePatGen(UINT32 u4InOut, UINT32 u4OnOff);
void vDrvMJCClearDramStatus(void);
UINT8 u1DrvMJCChkDramOvfUdf(UINT32 * pu4StaFB01, UINT32 * pu4StaFB02, UINT32 * pu4StaSys00);
void vDrvMJCDramOvfUdfPolling(void *pvArgv);
void vDrvMJCInOutCRCCheck(void *pvArgv);
void vDrvMJCInputLRCheck(void *pvArgv);
void vDrvMJCDelaySetProc(void);
void vDrvMJCSetFrmFreeze(UINT32 u4OnOff);
void vDrvMJCSetFBOneFrmOnOff(BOOL fgOnOff);
void vDrvMJCSetDispVpos(UINT32 u4PscanPrelen);
UINT32 fgDrvMJCGetDispMode(void);
UINT32 fgDrvMJCGetFlipMirror(void);
void vDrvCliCallPME(BOOL fgIs120Hz,UINT8 fgIs55p2);

void vDrvMJCSetMuteColor(UINT8 u1R, UINT8 u1G, UINT8 u1B);
void vDrvMJCSetAutoMute(UINT8 u1Count);
void vDrvMJCDelaySet(E_MJC_DELAY_SET eDelayType, UINT8 u1Count);
UINT8 u1DrvMJCGetMuteCount(void);
void vDrvMJCSetMuteCount(UINT32 u4MuteCount);
void vDrvMJCMainloop(void);
void vDrvMJCFrmDlyChkOnOff(UINT32 u4OnOff);
void vDrvMJCFrmDlyChk(UINT32 fgInVsync);
void vDrvMJCQueryInOutCtrl(void);
void vDrvMJCQueryFrmSchCtrl(void);
void vDrvMJCQueryDramCtrl(void);
void vDrvMJCQuery3DCtrl(void);
void vDrvMJCPRQueryCtrl(void);
void vDrvMJCImportProtection(UINT8 u1OnOff);
void vDrvMJCFireImportProtection(void);
void vDrvMJC1W_PR1RCtrl(void);
void vDrvMJCSwitchFrameTrackOnOff(UINT32 u4OnOff);
void vDrvMJCSetFrmMask(UINT32 u4FrmMaskI, UINT32 u4FrmMaskO);
UINT8 u1DrvMJCIsOnOff(void);
UINT8 u1DrvMJCGetFrmDelay(void);
UINT8 u1DrvMJCGetFrmDelay10x(void);
UINT8 u1DrvMJCGetPRMode(void);
void vDrvMJCSetPRMode(UINT8 u1PRMode , UINT8 u1FHPRMode);
void vDrvMJCSetPRCtrl(MJC_FRC_CONFIG_T* arMJCFRC ,  MJC_PIC_SIZE_T * arMJCPicSize);
void vDrvMJCSet1W1Rmode(UINT8 u111Rmode);
UINT8 u1DrvMJCGetPROnOffInfo(void);
void vDrvMJCSetPRModeOnOff(UINT8 fgPROnOff);
UINT8 u1DrvMJCGetHPRMode(void);
UINT8 u1DrvMJCGetFPRMode(void);
UINT32 u4DrvMJCGetFilmModeStatus(void);
UINT8 u1DrvMJCGet1W1Rmode(void);
void vDrvMJCSetDbgInk(UINT8 u1INKIdex);

const MJC_DRAM_MODE_T * rDrvMJCGetDramMode(UINT32 u4DramModeIdx);

void vDrvMJCSetDemoCfg(const MJC_DEMO_CFG_T* prDemo);


#if 1
void vDrvMJCSetTTDOnOff(UINT8 u1OnOff);
#endif
void MJC_Set3DModeDbg(UINT8 u13DMode);

EXTERN UINT8 u1DrvMJCGetIntpMode(void);

#ifdef CC_FPGA
UINT8  u1DrvMJCRead1B(UINT32 u4Addr);
UINT16 u2DrvMJCRead2B(UINT32 u4Addr);
UINT32 u4DrvMJCRead4B(UINT32 u4Addr);
UINT32 u4DrvMJCReadFldAlign(UINT32 u4Addr, UINT32 u4Fld);
#endif

#endif //#ifndef _DRV_MJC_H_

