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
/*****************************************************************************
*  TVE: HAL
*****************************************************************************/

#ifndef _TVE_HAL_C_
#define _TVE_HAL_C_

#include "tve_hw.h"
#include "tve_hal.h"
#include "tve_if.h"

// johnny mark  20080320 #include "../pmx/pmx_hal.h"

#include "x_bim.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "tve_debug.h"
#include "sv_const.h"
#include "hw_vdoin.h"
#include "hw_tvd.h"
#include "hw_ckgen.h"
#include "hw_scpos.h"

#include "drv_hdtv.h"
//#include "source_table.h"
#include "feature.h"
#include "drv_mpeg.h"
#include "drv_vga.h"
#include "drv_video.h"

/******************************************************************************
* Local definition
******************************************************************************/
#define TVE_CC_FIFO_SIZE	    128
#define XDS_VCHIP_DATA_MASK     0x7F    // 7-bits
#define TVE_CC_SYNC_TIMEOUT     60000   // 60 secs

#define TVE_TTX_BUF_NUM 8 
#define TTX_PACKET_SIZE     45       // 45 bytes per packet: field parity+line offset+framing_code+ttx_data_block

/******************************************************************************
* Function prototype
******************************************************************************/
static UCHAR _reverse_endian(UCHAR ucIn);
static void _TveHalCcIdle(UCHAR ucTveId, UCHAR ucIsOddField);
#if 1
static BOOL _TveHalDetectVChipData(const UCHAR *pucData, UCHAR ucField);
static void _TveHalShowVChipInfo(UCHAR ucByte0, UCHAR ucByte1);
#endif

extern UINT16 wDviInputWidth(void);
extern UINT16 wDviInputHeight(void);
extern UINT8 bGetSignalType(UINT8 bPath);
/******************************************************************************
* Local variable
******************************************************************************/
static volatile HAL_5391_MAIN_TVE_FIELD_T rTveMainReg;
//static volatile HAL_5351_AUX_TVE_FIELD_T rTveAuxReg;
static UINT32 _ucTveInputSrc = 0;

static UINT32 _u4MainCcRp = 0;
static UINT32 _u4MainCcWp = 0;

//static UINT32 _u4AuxCcRp = 0;
//static UINT32 _u4AuxCcWp = 0;

static UINT32 _u4IsCcTimerCreated = 0;
//static UINT32 _u4IsMainCcSyncToPts = 0;
//static UINT32 _u4IsAuxCcSyncToPts = 0;

static HANDLE_T _hTveCcTimer;

static TVE_CC_DATA_T _arMainCcFifo[TVE_CC_FIFO_SIZE];
//static TVE_CC_DATA_T _arAuxCcFifo[TVE_CC_FIFO_SIZE];

static BOOL _bVChipHeadAppear = FALSE;
static BOOL _bVChipDataAppear = FALSE;

/* CGMS-A Table (Workaround the MT5351 HW bug) */
static UCHAR _ucCGMSATable[4] =
{
    0x0, 0x1, 0x2, 0x3
};
#if 0
/* Wss 525-Line Aspect Ratio Table (Workaround the MT5351 HW bug) */
static UCHAR _uc525LAsrTable[4] =
{
    0x0, 0x2, 0x1, 0x3
};

/* Wss 625-Line Aspect Ratio Table (Workaround the MT5351 HW bug) */
static UCHAR _uc625LAsrTable[16] =
{
    0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
    0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF
};
#endif
/* Reverse CC Table (Workaround the MT5351 HW bug) */
static UCHAR RCCTable[256] =
{
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
    0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
    0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
    0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
    0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
    0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
    0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
    0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
    0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
    0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
    0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
    0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
    0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
    0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
    0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
    0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
    0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

// (V-chip) MPAA Movie Rating
static const TVE_ENUM2TEXT_T _arMPAAMovieRating[] =
{
	{ 0x0, "NONE" },
	{ 0x1, "G" },
	{ 0x2, "PG" },
	{ 0x3, "PG-13" },
	{ 0x4, "R" },
	{ 0x5, "NC-17" },
	{ 0x6, "X" },
	{ 0x7, "NONE" },
	
	{ 0xFF, NULL }   // end
};

// (V-chip) USA TV Rating
static const TVE_ENUM2TEXT_T _arUSATvRating[] =
{
	{ 0x0, "NONE" },
	{ 0x1, "TV-Y" },
	{ 0x2, "TV-Y7" },
	{ 0x3, "TV-G" },
	{ 0x4, "TV-PG" },
	{ 0x5, "TV-14" },
	{ 0x6, "TV-MA" },
	{ 0x7, "NONE" },
	
	{ 0xFF, NULL }   // end
};

// (V-chip) Canadian English TV Rating
static const TVE_ENUM2TEXT_T _arCanEngTvRating[] =
{
	{ 0x0, "E" },
	{ 0x1, "C" },
	{ 0x2, "C8+" },
	{ 0x3, "G" },
	{ 0x4, "PG" },
	{ 0x5, "14+" },
	{ 0x6, "18+" },
	{ 0x7, "NONE" },
	
	{ 0xFF, NULL }   // end
};

// (V-chip) Canadian French TV Rating
static const TVE_ENUM2TEXT_T _arCanFreTvRating[] =
{
	{ 0x0, "E" },
	{ 0x1, "G" },
	{ 0x2, "8ans+" },
	{ 0x3, "13ans+" },
	{ 0x4, "16ans+" },
	{ 0x5, "18ans+" },
	{ 0x6, "NONE" },
	{ 0x7, "NONE" },
	
	{ 0xFF, NULL }   // end
};

#define _TTX_CTRL_DW_ON 0x00000018
#define _TTX_CTRL_DW_OFF 0x00000000
static UINT32 _u4MainTTXRp = 0;
static UINT32 _u4MainTTXWp = 0;
static TVE_TTX_DATA_T _arTveTTXBuf[TVE_TTX_BUF_NUM];

static UINT8 _u1TveFieldMatchCnt=0;
static UINT8 _u1TveFieldMisMatchCnt=0;
//static BOOL _bTveFieldMatched=FALSE;
//static UCHAR _ucTvePreField=0;
//#define TVE_FIELD_MATCH_WA  1
#define TVE_FIELD_MATCH_TH  8
#define vDrvTveIncFieldCnt(bCnt)    (bCnt = (bCnt == TVE_FIELD_MATCH_TH)?TVE_FIELD_MATCH_TH:(bCnt+1))
//#define vDrvTveDecFieldCnt(bCnt)    (bCnt = (bCnt == 0)?0:(bCnt-1))

UINT16 const TVE_Sync_Time[4][4][2] = {
#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
{
//TVE_FMT_480I
    {0x404, 0x30c},//SV_VD_YPBPR
    {0x404, 0x30c},//SV_VD_VGA
    {0x404, 0x30c},//SV_VD_DVI
    {0x404, 0x30c},//SV_VD_MPEGHD/SV_VD_MPEGSD
},{
//TVE_FMT_576I
    {0x4ca, 0x308},//SV_VD_YPBPR
    {0x4ca, 0x308},//SV_VD_VGA
    {0x4ca, 0x308},//SV_VD_DVI
    {0x4ca, 0x308}//SV_VD_MPEGHD/SV_VD_MPEGSD
},{
//TVE_FMT_480I + TVE_WA1
    {0x404, 0x30c},//SV_VD_YPBPR
    {0x404, 0x30c},//SV_VD_VGA
    {0x404, 0x30c},//SV_VD_DVI
    {0x404, 0x30c},//SV_VD_MPEGHD/SV_VD_MPEGSD
},{
//TVE_FMT_576I + TVE_WA1
    {0x4ca, 0x308},//SV_VD_YPBPR
    {0x4ca, 0x308},//SV_VD_VGA
    {0x4ca, 0x308},//SV_VD_DVI
    {0x4ca, 0x308}//SV_VD_MPEGHD/SV_VD_MPEGSD
}
#else
{
//TVE_FMT_480I
    {0x3d6, 0x304},//SV_VD_YPBPR
    {0x3d6, 0x304},//SV_VD_VGA
    {0x3d6, 0x306},//SV_VD_DVI
    {0x3d6, 0x304},//SV_VD_MPEGHD/SV_VD_MPEGSD
},{
//TVE_FMT_576I
    {0x4a0, 0x2fe},//SV_VD_YPBPR
    {0x4a0, 0x2fe},//SV_VD_VGA
    {0x4a0, 0x302},//SV_VD_DVI
    {0x4a0, 0x2fe}//SV_VD_MPEGHD/SV_VD_MPEGSD
},{
//TVE_FMT_480I + TVE_WA1
    {0x41a, 0x304},//SV_VD_YPBPR
    {0x41a, 0x304},//SV_VD_VGA
    {0x41a, 0x306},//SV_VD_DVI
    {0x41a, 0x304},//SV_VD_MPEGHD/SV_VD_MPEGSD
},{
//TVE_FMT_576I + TVE_WA1
    {0x4e2, 0x2fe},//SV_VD_YPBPR
    {0x4e2, 0x2fe},//SV_VD_VGA
    {0x4e2, 0x302},//SV_VD_DVI
    {0x4e2, 0x2fe}//SV_VD_MPEGHD/SV_VD_MPEGSD
}
#endif
};
BOOL   _fgDescramblerOn=FALSE;//for descramber box issue(loss channel & show no signal)


/******************************************************************************
* static functions
******************************************************************************/
static UCHAR _reverse_endian(UCHAR ucIn)
{
    return RCCTable[ucIn];
}

static UINT32 BitInverse(UINT32 u4Input, UINT8 u1Rotate)
{
    UINT8 u1i;
    UINT32 u4Temp = 0x0;
    UINT32 u4Result;
    
    for ( u1i = 0; u1i<(u1Rotate-1); u1i++)
    {
        u4Temp |= u4Input&1;
        u4Input = u4Input>>1;
        u4Temp = u4Temp<<1;
    }
	
    u4Temp |= u4Input&1;
    u4Result = u4Temp &((1<<u1Rotate)-1);

    return u4Result;
}

/*--------------------------------------------
// CC sync timer function
//-------------------------------------------- */
static VOID _TveHalCcSyncTimeout(HANDLE_T pt_tm_handle, VOID *pv_tag)
{
    // reset CC sync to pts flag
    //_u4IsMainCcSyncToPts = 0;
    //_u4IsAuxCcSyncToPts = 0;
}

/*--------------------------------------------
// set CC data to 0, but still enable CC signal output
//--------------------------------------------*/
static void _TveHalCcIdle(UCHAR ucTveId, UCHAR ucIsOddField)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }

    if (ucTveId == TVE_1)
    {
        if (ucIsOddField)   // TVE_FIELD_ODD
        {
            rTveMainReg.uCc.rCc.u4CcMode = 1; 	// Enable CC (Line 21)
            rTveMainReg.uCc.rCc.u4CcData0 = 0;
            rTveMainReg.uCc.rCc.u4CcData1 = 0;

            // Set TVE HW Registers
            TVE_WRITE32(TVE1_REG_CC, rTveMainReg.uCc.u4Cc);
        }

        if (!ucIsOddField)  // TVE_FIELD_EVEN
        {
            rTveMainReg.uCc.rCc.u4CcMode = 2; 	// Enable CC (Line 284)
            rTveMainReg.uCc.rCc.u4CcData0 = 0;
            rTveMainReg.uCc.rCc.u4CcData1 = 0;

            // Set TVE HW Registers
            TVE_WRITE32(TVE1_REG_CC, rTveMainReg.uCc.u4Cc);
        }
    }
    else        // TVE_2 (Aux)
    {
            LOG(7, "Only 1 TVE\n");
    }
}

/*--------------------------------------------
// set TTX 
//--------------------------------------------*/
static void _TveHalTTXIdle(UCHAR ucTveId, UCHAR ucIsOddField)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }
#if 0
    if (ucTveId == TVE_1)
    {
        if (ucIsOddField)   // TVE_FIELD_ODD
        {
            rTveMainReg.uCc.rCc.u4CcMode = 1; 	// Enable CC (Line 21)
            rTveMainReg.uCc.rCc.u4CcData0 = 0;
            rTveMainReg.uCc.rCc.u4CcData1 = 0;

            // Set TVE HW Registers
            TVE_WRITE32(TVE1_REG_CC, rTveMainReg.uCc.u4Cc);
        }

        if (!ucIsOddField)  // TVE_FIELD_EVEN
        {
            rTveMainReg.uCc.rCc.u4CcMode = 2; 	// Enable CC (Line 284)
            rTveMainReg.uCc.rCc.u4CcData0 = 0;
            rTveMainReg.uCc.rCc.u4CcData1 = 0;

            // Set TVE HW Registers
            TVE_WRITE32(TVE1_REG_CC, rTveMainReg.uCc.u4Cc);
        }
    }
    else        // TVE_2 (Aux)
    {
            LOG(7, "Only 1 TVE\n");
    }
    #endif
}

#if 1
/*--------------------------------------------
// Detect V-chip data
//--------------------------------------------*/
static BOOL _TveHalDetectVChipData(const UCHAR *pucData, UCHAR ucField)
{
    UCHAR ucByte0, ucByte1;
    
    if (pucData == NULL)
    {
        return FALSE;
    }

    if (ucField == TVE_FIELD_ODD)
    {
        return FALSE;
    }
        
    ucByte0 = (pucData[0] & XDS_VCHIP_DATA_MASK);
    ucByte1 = (pucData[1] & XDS_VCHIP_DATA_MASK);

    if (TRUE == _bVChipDataAppear)
    {
        if (0x0F == ucByte0)
        {
            _bVChipDataAppear = FALSE;
            LOG(7, "[End]\n");
            return TRUE;
        }
    }
        
    if (TRUE == _bVChipHeadAppear)
    {
        _bVChipHeadAppear = FALSE;
        _bVChipDataAppear = TRUE;
        LOG(7, "[Data (0x%2x)(0x%2x)] ", ucByte0, ucByte1);

        // for debug use
        _TveHalShowVChipInfo(ucByte0, ucByte1);

        return TRUE;
    }
        
    if ((0x01 == ucByte0) && (0x05 == ucByte1))
    {
        _bVChipHeadAppear = TRUE;
        _bVChipDataAppear = FALSE;
        LOG(7, "[V-chip][Head] ");
        return TRUE;
    }

    return FALSE;
}

/*--------------------------------------------
// Interpret V-chip data
//-------------------------------------------- */
static void _TveHalShowVChipInfo(UCHAR ucByte0, UCHAR ucByte1)
{
    UCHAR ucR2_R0, ucR2_R0_Mask = 0x07;
    UCHAR ucA2_A0, ucA2_A0_Mask = 0x38, ucA2_A0_Shift = 3;
    UCHAR ucG2_G0, ucG2_G0_Mask = 0x07;
    UCHAR ucD_A2, ucD_Mask = 0x20, ucD_Shift = 5;
    UCHAR ucL_A3, ucL_Mask = 0x08, ucL_Shift = 3;
    UCHAR ucV, ucV_Mask = 0x20, ucV_Shift = 5;
    UCHAR ucS, ucS_Mask = 0x10, ucS_Shift = 4;
    UCHAR ucA0, ucA0_Mask = 0x08, ucA0_Shift = 3;
    UCHAR ucA1_A0, ucA1_A0_Mask = 0x18, ucA1_A0_Shift = 3;
    TVE_ENUM2TEXT_T *prEnum2Text;

    // byte0
    ucR2_R0 = (ucByte0 & ucR2_R0_Mask);
    ucA2_A0 = ((ucByte0 & ucA2_A0_Mask) >> ucA2_A0_Shift);
    ucD_A2  = ((ucByte0 & ucD_Mask) >> ucD_Shift);
    ucA1_A0 = ((ucByte0 & ucA1_A0_Mask) >> ucA1_A0_Shift);
    ucA0    = ((ucByte0 & ucA0_Mask) >> ucA0_Shift);

    // byte1
    ucG2_G0 = (ucByte1 & ucG2_G0_Mask);
    ucL_A3  = ((ucByte1 & ucL_Mask) >> ucL_Shift);
    ucS     = ((ucByte1 & ucS_Mask) >> ucS_Shift);
    ucV     = ((ucByte1 & ucV_Mask) >> ucV_Shift);
    
    if (ucA0 == 0)
    {
        LOG(7, "[MPAA Movie Rating] ");

        prEnum2Text = (TVE_ENUM2TEXT_T *)_arMPAAMovieRating;
        
        while (prEnum2Text->szText != NULL)
        {
            if (prEnum2Text->ucValue == ucR2_R0)
            {
                break;
            }
            else
            {
                prEnum2Text++;
            }
        }
        
        LOG(7, "%s ", prEnum2Text->szText);
    }

    if (ucA1_A0 == 0x1)
    {
        LOG(7, "[USA TV Rating] ");

        prEnum2Text = (TVE_ENUM2TEXT_T *)_arUSATvRating;
        
        while (prEnum2Text->szText != NULL)
        {
            if (prEnum2Text->ucValue == ucG2_G0)
            {
                break;
            }
            else
            {
                prEnum2Text++;
            }
        }
        
        LOG(7, "%s ", prEnum2Text->szText);
    }
    
    if ((ucL_A3 == 0) && (ucA2_A0 == 0x3))
    {
        LOG(7, "[Can. Eng TV Rating] ");

        prEnum2Text = (TVE_ENUM2TEXT_T *)_arCanEngTvRating;
        
        while (prEnum2Text->szText != NULL)
        {
            if (prEnum2Text->ucValue == ucG2_G0)
            {
                break;
            }
            else
            {
                prEnum2Text++;
            }
        }
        
        LOG(7, "%s ", prEnum2Text->szText);
    }

    if ((ucL_A3 == 0) && (ucA2_A0 == 0x7))
    {
        LOG(7, "[Can. Eng TV Rating] ");

        prEnum2Text = (TVE_ENUM2TEXT_T *)_arCanFreTvRating;
        
        while (prEnum2Text->szText != NULL)
        {
            if (prEnum2Text->ucValue == ucG2_G0)
            {
                break;
            }
            else
            {
                prEnum2Text++;
            }
        }
        
        LOG(7, "%s ", prEnum2Text->szText);
    }

    if (ucV == 1)
    {
        LOG(7, "V");
    }

    if (ucS == 1)
    {
        LOG(7, "S");
    }

    if (ucL_A3== 1)
    {
        LOG(7, "L");
    }

    if (ucD_A2== 1)
    {
        LOG(7, "D");
    }
}
#endif

/******************************************************************************
* public functions
******************************************************************************/
/*--------------------------------------------
// Enable TVE 
//-------------------------------------------- */
void TVE_HalEnable(UCHAR ucTveId)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uEncMode.rEncMode.fgEncOff = 0;
        //rTveMainReg.uDacControl.rDacControl.u4DAC1 = 0x3;
        //rTveMainReg.uDacControl.rDacControl.u4DAC2 = 0x3;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_ENCMOD, rTveMainReg.uEncMode.u4EncMode);
        //TVE_WRITE32(TVE1_REG_DACTRL, rTveMainReg.uDacControl.u4DacCtrl);
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return;
}

/*--------------------------------------------
// Disable TVE & DAC
//-------------------------------------------- */
void TVE_HalDisable(UCHAR ucTveId)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }
	
    if (ucTveId == TVE_1)
    {
        rTveMainReg.uEncMode.rEncMode.fgEncOff = 1;
        //rTveMainReg.uDacControl.rDacControl.u4DAC1 = 0;
        //rTveMainReg.uDacControl.rDacControl.u4DAC2 = 0;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_ENCMOD, rTveMainReg.uEncMode.u4EncMode);
        //TVE_WRITE32(TVE1_REG_DACTRL, rTveMainReg.uDacControl.u4DacCtrl);
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return;
}

/*--------------------------------------------
// Trigger TVE to reset
//-------------------------------------------- */
void TVE_HalReset(UCHAR ucTveId)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }

    if (ucTveId == TVE_1)
    {
        x_memset(((void *)&rTveMainReg), 0, sizeof(HAL_5391_MAIN_TVE_FIELD_T));
		
        rTveMainReg.uEncSyn.rEncSyn.u4Rst = 3;
        TVE_WRITE32(TVE1_REG_ENCSYN, rTveMainReg.uEncSyn.u4EncSyn);
		
        rTveMainReg.uEncSyn.rEncSyn.u4Rst = 0;
        TVE_WRITE32(TVE1_REG_ENCSYN, rTveMainReg.uEncSyn.u4EncSyn);
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return;
}

/*--------------------------------------------
// Turn On/Off ColorBar
//-------------------------------------------- */
void TVE_HalColorBar(UCHAR ucTveId, UCHAR ucOn)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uEncMode.rEncMode.fgCbOn = ucOn;
        rTveMainReg.uSyncCtrl.rSyncCtrl.fgAdjSelfEn = ucOn;//ky add
        //rTveMainReg.uSyncCtrl.rSyncCtrl.fgAdjSyncEn = ucOn;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_ENCMOD, rTveMainReg.uEncMode.u4EncMode);
        TVE_WRITE32(TVE1_REG_SYNC_CTRL, rTveMainReg.uSyncCtrl.u4SyncCtrl);
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return;
}

/*--------------------------------------------
// Set TVE Mode
//-------------------------------------------- */
void TVE_HalSetMode(UCHAR ucTveId, UCHAR ucFmt, UCHAR ucMode)
{
    if ((ucTveId >= TVE_MAX_NS) || 
        (ucFmt >= TVE_MAX_FMT) ||
        (ucMode >= TVE_MAX_MOD))
    {
        return;
    }

    TVE_HalSetFmt(ucTveId, ucFmt, ucMode);

    return;
}

/*--------------------------------------------
// Set TVE Format and related setting
//-------------------------------------------- */
void TVE_HalSetFmt(UCHAR ucTveId, UCHAR ucFmt, UCHAR ucMode)
{
    if ((ucTveId >= TVE_MAX_NS) || 
        (ucFmt >= TVE_MAX_FMT) ||
        (ucMode >= TVE_MAX_MOD))
    {
        return;
    }

    /* IC number = MT5351_BA (IC_VER_B) */
//    if (BSP_GetIcVersion() >= IC_VER_B)
//    rTveMainReg.uDacControl.rDacControl.u4DaCtrlMisc1 = 0;
//    rTveMainReg.uDacControl.rDacControl.u4DaCtrlMisc2 = 0xF0;

    /* Set TVE HW MAIN Registers */
//    TVE_WRITE32(TVE1_REG_DACTRL, rTveMainReg.uDacControl.u4DacCtrl);

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uEncMode.rEncMode.fgSlewOff = 1;

        /* Adjust Gamma to 1.0 V */
        rTveMainReg.uGammaCoeff1.u4GammaCoeff1 = 0x00000000;
        rTveMainReg.uGammaCoeff2.u4GammaCoeff2 = 0x02800140;
        rTveMainReg.uGammaCoeff3.u4GammaCoeff3 = 0x050003C0;
        rTveMainReg.uGammaCoeff4.u4GammaCoeff4 = 0x07800640;
        rTveMainReg.uGammaCoeff5.u4GammaCoeff5 = 0x0A0008C0;
      
        if (ucMode == TVE_MOD_COMPOSITE) /* only 480i & 576i */
        {
            rTveMainReg.uEncMode.rEncMode.fgGSync = 1;
            //rTveMainReg.uEncMode.rEncMode.u4OutMode1 = 0;
            rTveMainReg.uEncMode.rEncMode.u4OutMode0 = 1;	/* CVBS/S-Video */

//            rTveMainReg.uEncMode.rEncMode.fgCavUpOn = 1;
            rTveMainReg.uEncMode.rEncMode.fgYLPOn = 1;
            rTveMainReg.uEncMode.rEncMode.fgCLPOn = 1;

            /* All color-burst advanced for Macrovision */
//            rTveMainReg.uEncMode.rEncMode.fgBurstStart = 1;
            
            /* Disable Y'(709)CbCr -> R'G'B' */
//            rTveMainReg.uEncMode.rEncMode.fgHdRgb = 0;
//            rTveMainReg.uEncMode.rEncMode.fgHMode = 0;	/* HD disable */

            /* YC Delay */
            rTveMainReg.uYScale.u4YScale = 0x00041680; /* 80 => YScale value */
//            rTveMainReg.uCbCrDelay.u4CbCrDelay = 0x0053B30C;

            if ((ucFmt == TVE_FMT_480I) || (ucFmt == TVE_FMT_576I))
            {
                #ifdef CC_TVE6595
                rTveMainReg.uUGain.u4UGain = 0x00000040;
                rTveMainReg.uVGain.u4VGain = 0x00000060;
                #else			
                rTveMainReg.uUGain.u4UGain = 0x40404040;
                rTveMainReg.uVGain.u4VGain = 0x60606060;
                #endif
            
                #ifdef CC_TVE6595
                /* Adjust Y Low Pass Filter Coefficients */
                rTveMainReg.uEncSyn.rEncSyn.u4YLPFCoefSet = 3;
                /* Adjust C Low Pass Filter Coefficients */				
                rTveMainReg.uEncSyn.rEncSyn.u4CLPFCoefSet = 1;
                #else
                /* Adjust Y Low Pass Filter Coefficients */
                rTveMainReg.uYCoeff1.u4YCoeff1 = 0x070905FF;
                rTveMainReg.uYCoeff2.u4YCoeff2 = 0xF7F4F901;
                rTveMainReg.uYCoeff3.u4YCoeff3 = 0x0B100B01;
                rTveMainReg.uYCoeff4.u4YCoeff4 = 0xEFE4ECFC;
                rTveMainReg.uYCoeff5.u4YCoeff5 = 0x7C62380D;

                /* Adjust C Low Pass Filter Coefficients */
                rTveMainReg.uCbCrCoeff0.u4CbCrCoeff0 = 0x1a11e400;
                rTveMainReg.uCbCrCoeff1.u4CbCrCoeff1 = 0x00ff12c3;
                #endif

                /* Color Burst Configuration 1 */
				#ifdef CC_TVE6595
                rTveMainReg.uBurst1.u4Burst1 = 0x8087003C; //set demod_gain
				#else
                rTveMainReg.uBurst1.u4Burst1 = 0x0087003C;
				#endif
                if (ucFmt == TVE_FMT_480I)
                {
                    rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType = 0x0;//NTSC 480i
                    #if defined(CC_MT5363)||defined(CC_TVE6595)
                    rTveMainReg.uChromaGain.rChromaGain.u4Y2ActiveEnd = 0xb;
                    rTveMainReg.uChromaGain.rChromaGain.u4C2ActiveEnd = 0x5;
                    #endif
                }
                else
                {
                    rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType = 0x3;//PAL 576i
                    #if defined(CC_MT5363)||defined(CC_TVE6595)
                    rTveMainReg.uChromaGain.rChromaGain.u4Y2ActiveEnd = 0x29;
                    rTveMainReg.uChromaGain.rChromaGain.u4C2ActiveEnd = 0x19;
                    #endif
                }
            }
            //else /* 480p, 576p, 720p, 1080i */
            //{
                //LOG(7, "Composite, No Support This FMT(%d)\n", (INT32)ucFmt);
                //return;
           // }
        }
        else if (ucMode == TVE_MOD_COMPONENT) /* Y'PbPr Output */
        {
            LOG(7, "Composite, No Support This FMT(%d)\n", (INT32)ucFmt);
            return;
        }
        else if (ucMode == TVE_MOD_VGA) /* R'G'B' Output */
        {
            LOG(7, "Composite, No Support This FMT(%d)\n", (INT32)ucFmt);
            return;
        }

        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_ENCMOD, rTveMainReg.uEncMode.u4EncMode);

//        TVE_WRITE32(TVE1_REG_CBSCALE, rTveMainReg.uCbGain.u4CbGain);
//        TVE_WRITE32(TVE1_REG_CRSCALE, rTveMainReg.uCrGain.u4CrGain);

        TVE_WRITE32(TVE1_REG_GAMMA1, rTveMainReg.uGammaCoeff1.u4GammaCoeff1);
        TVE_WRITE32(TVE1_REG_GAMMA2, rTveMainReg.uGammaCoeff2.u4GammaCoeff2);
        TVE_WRITE32(TVE1_REG_GAMMA3, rTveMainReg.uGammaCoeff3.u4GammaCoeff3);
        TVE_WRITE32(TVE1_REG_GAMMA4, rTveMainReg.uGammaCoeff4.u4GammaCoeff4);
        TVE_WRITE32(TVE1_REG_GAMMA5, rTveMainReg.uGammaCoeff5.u4GammaCoeff5);

        TVE_WRITE32(TVE1_REG_YSCALE, rTveMainReg.uYScale.u4YScale);
//        TVE_WRITE32(TVE1_REG_CBCRDELAY, rTveMainReg.uCbCrDelay.u4CbCrDelay);

        /* For Composite */
        TVE_WRITE32(TVE1_REG_USCALE, rTveMainReg.uUGain.u4UGain);
        TVE_WRITE32(TVE1_REG_VSCALE, rTveMainReg.uVGain.u4VGain);
        TVE_WRITE32(TVE1_REG_BURST, rTveMainReg.uBurst1.u4Burst1);

        //for TV type
        TVE_WRITE32(TVE1_REG_MUX_CTRL, rTveMainReg.uMuxCtrl.u4MuxCtrl);
        #if defined(CC_MT5363)||defined(CC_TVE6595)
        TVE_WRITE32(TVE1_REG_CHROMAGAIN, rTveMainReg.uChromaGain.u4ChromaGain);
        #endif
        
        #ifdef CC_TVE6595
        TVE_WRITE32(TVE1_REG_ENCSYN, rTveMainReg.uEncSyn.u4EncSyn);			
        #else
        if (rTveMainReg.uEncMode.rEncMode.fgYLPOn == 1)
        {
            TVE_WRITE32(TVE1_REG_YCOEFF0, rTveMainReg.uYCoeff1.u4YCoeff1);
            TVE_WRITE32(TVE1_REG_YCOEFF1, rTveMainReg.uYCoeff2.u4YCoeff2);
            TVE_WRITE32(TVE1_REG_YCOEFF2, rTveMainReg.uYCoeff3.u4YCoeff3);
            TVE_WRITE32(TVE1_REG_YCOEFF3, rTveMainReg.uYCoeff4.u4YCoeff4);
            TVE_WRITE32(TVE1_REG_YCOEFF4, rTveMainReg.uYCoeff5.u4YCoeff5);
        }
        
        if (rTveMainReg.uEncMode.rEncMode.fgCLPOn == 1)
        {
            TVE_WRITE32(TVE1_REG_CBCRCOEFF0, rTveMainReg.uCbCrCoeff0.u4CbCrCoeff0);
            TVE_WRITE32(TVE1_REG_CBCRCOEFF1, rTveMainReg.uCbCrCoeff1.u4CbCrCoeff1);
        }
        #endif
    }
    else /* TVE_2 (Aux) MOD = Composite/Component, only 525i & 625i */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return;
}

/*--------------------------------------------
// Set TVE Format and related setting
//-------------------------------------------- */
void TVE_HalGetFmt(UCHAR ucTveId, UCHAR* ucFmt)
{
    if ((ucTveId >= TVE_MAX_NS) || (ucFmt == NULL))
    {
        return;
    }

    if(rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType == 0)
    {
        *ucFmt = TVE_FMT_480I;
    }
    else if(rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType == 3)
    {
        *ucFmt = TVE_FMT_576I;
    }
    else 
    {
        *ucFmt = TVE_MAX_FMT;
    }
}

/*--------------------------------------------
// Set low impedence to DAC
//-------------------------------------------- */
void TVE_HalSetImpedence(UCHAR ucTveId, UCHAR ucImpedence)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }
	
    /* Set low impedence to DAC */
#ifdef MT5351_M1_BOARD
    if (ucImpedence == TVE_HIGH_IMPEDENCE)
    {
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
        vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_IBAND_X); 
        vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_IBAND_Y);
        #else
        rTveMainReg.uDacControl.rDacControl.u4PdBcd = 0x3;
        rTveMainReg.uDacControl.rDacControl.u4PdCd = 0x3;
        #endif
    }
    else
    {
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
        vIO32WriteFldAlign(REG_VDAC_CFG0, 3, RG_IBAND_X); 
        vIO32WriteFldAlign(REG_VDAC_CFG0, 3, RG_IBAND_Y;
        #else
        rTveMainReg.uDacControl.rDacControl.u4PdBcd = 0;
        rTveMainReg.uDacControl.rDacControl.u4PdCd = 0;
        #endif
    }        
#else   /* Demo Board */
    if (ucImpedence == TVE_LOW_IMPEDENCE)
    {
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
        vIO32WriteFldAlign(REG_VDAC_CFG0, 3, RG_IBAND_X); 
        vIO32WriteFldAlign(REG_VDAC_CFG0, 3, RG_IBAND_Y);
        #else
        rTveMainReg.uDacControl.rDacControl.u4PdBcd = 0;
        rTveMainReg.uDacControl.rDacControl.u4PdCd = 0;
        #endif
    }
    else
    {
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
        vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_IBAND_X); 
        vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_IBAND_Y);
        #else
        rTveMainReg.uDacControl.rDacControl.u4PdBcd = 0x3;
        rTveMainReg.uDacControl.rDacControl.u4PdCd = 0x3;
        #endif
    }
#endif

    /* Set TVE HW MAIN Registers */
    TVE_WRITE32(TVE1_REG_DACTRL, rTveMainReg.uDacControl.u4DacCtrl);
}

/*--------------------------------------------
// Set VDAC current
//-------------------------------------------- */
void TVE_HalSetCurrent(UCHAR ucDacId, UCHAR ucCurrent)
{
    if(ucDacId >= DAC_MAX)
        return;
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    if (ucDacId == DAC_1)
    {
        switch(ucCurrent)
        {
            case DAC_1_4_CURRENT:
                vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_IBAND_X);
                break;
            case DAC_1_2_CURRENT:
                vIO32WriteFldAlign(REG_VDAC_CFG0, 1, RG_IBAND_X);
                break;
            case DAC_3_4_CURRENT:
                vIO32WriteFldAlign(REG_VDAC_CFG0, 2, RG_IBAND_X);
                break;
            case DAC_FULL_CURRENT:
            default:
                vIO32WriteFldAlign(REG_VDAC_CFG0, 3, RG_IBAND_X);
                break;
        }
    }
    else
    {
        switch(ucCurrent)
        {
            case DAC_1_4_CURRENT:
                vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_IBAND_Y);
                break;
            case DAC_1_2_CURRENT:
                vIO32WriteFldAlign(REG_VDAC_CFG0, 1, RG_IBAND_Y);
                break;
            case DAC_3_4_CURRENT:
                vIO32WriteFldAlign(REG_VDAC_CFG0, 2, RG_IBAND_Y);
                break;
            case DAC_FULL_CURRENT:
            default:
                vIO32WriteFldAlign(REG_VDAC_CFG0, 3, RG_IBAND_Y);
                break;
        }

    }
    #endif
    return;
}

    
/*--------------------------------------------
// 
//-------------------------------------------- */
void TVE_HalCcInit(UCHAR ucTveId)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }
	
    if (ucTveId == TVE_1)
    {
        _u4MainCcRp = 0;
        _u4MainCcWp = 0;
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    if (!_u4IsCcTimerCreated)
    {
        // create CC timer for PTS sync use
        VERIFY(x_timer_create(&_hTveCcTimer) == OSR_OK);

        VERIFY(x_timer_start(_hTveCcTimer, TVE_CC_SYNC_TIMEOUT, 
            X_TIMER_FLAG_REPEAT, _TveHalCcSyncTimeout, NULL) == OSR_OK);

        _u4IsCcTimerCreated = 1;
    }

    return;
}

/*--------------------------------------------
// 
//-------------------------------------------- */
void TVE_HalEnableCc(UCHAR ucTveId, UCHAR ucEnable)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }
	
    if (ucTveId == TVE_1)   // TVE_1 (Main)
    {
        if (ucEnable == (UCHAR)TRUE)
        {
            rTveMainReg.uCc.rCc.u4CcMode = 0x3;
            rTveMainReg.uCc.rCc.u4CcData0 = 0x0;
            rTveMainReg.uCc.rCc.u4CcData1 = 0x0;
            TVE_WRITE32(TVE1_REG_CC, rTveMainReg.uCc.u4Cc);
            LOG(5, "(Main) CC ouput On");
        }
        else
        {
            rTveMainReg.uCc.rCc.u4CcMode = 0x0;
            rTveMainReg.uCc.rCc.u4CcData0 = 0x0;
            rTveMainReg.uCc.rCc.u4CcData1 = 0x0;
            TVE_WRITE32(TVE1_REG_CC, rTveMainReg.uCc.u4Cc);
            LOG(5, "(Main) CC output Off");
        }
    }
    else                    // TVE_2 (Aux)
    {
        LOG(7, "Only 1 TVE\n");
    }

    return;
}

/*--------------------------------------------
// Send to TVE CC queue
//-------------------------------------------- */
UCHAR TVE_HalSend2CcQue(UCHAR ucTveId, UCHAR ucField, const UCHAR *pucData, PTS_T u8Pts)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }
    
    if (ucTveId == TVE_1)
    {
        // copy data to fifo
        _arMainCcFifo[_u4MainCcWp].u8Pts = u8Pts;
        _arMainCcFifo[_u4MainCcWp].ucData1 = pucData[1];    // hw problem
        _arMainCcFifo[_u4MainCcWp].ucData2 = pucData[0];    // hw problem
        _arMainCcFifo[_u4MainCcWp].ucField = ucField;
        
        _u4MainCcWp++;      // update write ptr
        
        // check write ptr boundary
        if (_u4MainCcWp >= TVE_CC_FIFO_SIZE)
        {
            _u4MainCcWp = 0;
        }
    }
    else            // TVE_2 (Aux)
    {
        LOG(7, "Only 1 TVE\n");
    }

//    PMX_HalSetCc(ucTveId, 1);   // enable ISR callback

    return TVE_SET_OK;
}

/*--------------------------------------------
// 
//-------------------------------------------- */
void TVE_HalSendCcIsr(UCHAR ucTveId, UCHAR ucIsOddField, UINT32 u4PmxPts)
{
    UCHAR ucField;
    BOOL bIsVChipData;
    UCHAR ucCcData[2];

    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }

    if (ucTveId == TVE_1)
    {
        // fifo empty
        if (_u4MainCcRp == _u4MainCcWp)
        {
//            PMX_HalSetCc(ucTveId, 0);     // disable ISR callback
            _TveHalCcIdle(ucTveId, ucIsOddField);
            return;
        }

        ucField = _arMainCcFifo[_u4MainCcRp].ucField;

        if (ucField == TVE_FIELD_ODD)
        {
            if (!ucIsOddField)
            {
                return;
            }

            rTveMainReg.uCc.rCc.u4CcMode = 1; 	// Enable CC (Line 21)
        }

        if (ucField == TVE_FIELD_EVEN)
        {
            if (ucIsOddField)
            {
                return;
            }

            rTveMainReg.uCc.rCc.u4CcMode = 2; 	// Enable CC (Line 284)
        }


    #if 0
        //-----------------------------------------------------------------
        // Todo: enable and verify the following code
        // CC sync to pts
        if (!_u4IsMainCcSyncToPts)
        {
            UINT32 u4Rp = _u4MainCcRp;
    		UINT32 u4MpvPts;
    		INT32 i;
            for (i = 0; i < TVE_CC_FIFO_SIZE; i++)
            {
                u4MpvPts = (UINT32)(_arMainCcFifo[u4Rp].u8Pts & 0xFFFFFFFF);

                // pts fail, free run
                if ((u4MpvPts == 0) || (u4PmxPts == 0))
                {
                    _u4IsMainCcSyncToPts = 1;
                    break;
                }

                // succeed to sync CC to pts ?
                if (u4MpvPts == u4PmxPts)
                {
                    _u4MainCcRp = u4Rp;
                    _u4IsMainCcSyncToPts = 1;
                    break;
                }

                // protection: read ptr can NOT exceed to write ptr
                if (u4Rp == _u4MainCcWp)
                {
                    _u4IsMainCcSyncToPts = 1;
                    break;
                }

                // Update read ptr and check read ptr boundary
                u4Rp++;
                if (u4Rp >= TVE_CC_FIFO_SIZE)
                {
                    u4Rp = 0;
                }
            } //~for
        } // ~if
        //-----------------------------------------------------------------
    #endif


        rTveMainReg.uCc.rCc.u4CcData0 = _reverse_endian(_arMainCcFifo[_u4MainCcRp].ucData1);
        rTveMainReg.uCc.rCc.u4CcData1 = _reverse_endian(_arMainCcFifo[_u4MainCcRp].ucData2);
	//TODO odd parity-bit

        //---------------------------------------------------
        // todo: refine the following code, output all data
        // Is V-chip data or not ?
        // only v-chip data can output; disable others
        
        ucCcData[1] = _arMainCcFifo[_u4MainCcRp].ucData1;
        ucCcData[0] = _arMainCcFifo[_u4MainCcRp].ucData2;
        bIsVChipData = _TveHalDetectVChipData(ucCcData, ucField);
        if (TRUE == bIsVChipData)
        {
            //rTveMainReg.uCc.rCc.u4CcMode = 2;   // Enable CC (Line 284)
        }
        else
        {
            // still output zero CC
            //rTveMainReg.uCc.rCc.u4CcMode = 3;
            //rTveMainReg.uCc.rCc.u4CcData0 = 0;
            //rTveMainReg.uCc.rCc.u4CcData1 = 0;
        }
        
        //---------------------------------------------------


        // Set TVE HW Registers
        TVE_WRITE32(TVE1_REG_CC, rTveMainReg.uCc.u4Cc);

        _u4MainCcRp++;     // update read ptr
        
        // check read ptr boundary
        if (_u4MainCcRp >= TVE_CC_FIFO_SIZE)
        {
            _u4MainCcRp = 0;
        }
    }
    else            // TVE_2 (Aux)
    {
        LOG(7, "Only 1 TVE\n");
    }
}

#ifdef LINUX_TURNKEY_SOLUTION
void TVE_HalTTXInit(void)
{
    UINT8 i = 0;
    UINT32 u4Addr=0;
    for(i=0; i<TVE_TTX_BUF_NUM; i++)
    {
        u4Addr = (UINT32)BSP_AllocAlignedDmaMemory(TVE_TTX_BUF_LINENUM*TVE_TTX_BUF_DATAPERLINE*4, 16);
        if (u4Addr == 0)
        {
            LOG(3, "%s:%d: Can't allocate TVE buffer!\n", __FILE__, __LINE__);
            return;
        }
        _arTveTTXBuf[i].ucData = (UINT32 *)VIRTUAL(u4Addr);
    }
}

void TVE_HalTTXUnInit(void)
{
    UINT8 i = 0;
    UINT32 u4Addr=0;
    for(i=0; i<TVE_TTX_BUF_NUM; i++)
    {
        u4Addr = PHYSICAL((UINT32)(_arTveTTXBuf[i].ucData));
        if(u4Addr!=0)
        {
            if(!BSP_FreeAlignedDmaMemory(u4Addr))
            {
                LOG(3, "Fail to free TVE buffer!, addr: 0x%08x\n", u4Addr);
            }
        }
    }
}
#endif

/*--------------------------------------------
// 
//-------------------------------------------- */
void TVE_HalEnableTTX(UCHAR ucTveId, UCHAR ucEnable)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }
	
    if (ucTveId == TVE_1)   // TVE_1 (Main)
    {
        if (ucEnable == (UCHAR)TRUE)
        {
            rTveMainReg.uTT_ctrl.rTT_ctrl.fgTeltxtEn = 0x1;
            TVE_WRITE32(TVE1_REG_TT_CTL, rTveMainReg.uTT_ctrl.u4TT_ctrl);
            LOG(5, "TTX ouput On");
        }
        else
        {
            rTveMainReg.uEncMode.rEncMode.fgTTRst = 0x1;
            TVE_WRITE32(TVE1_REG_ENCMOD, rTveMainReg.uEncMode.u4EncMode);
            
            rTveMainReg.uTT_ctrl.rTT_ctrl.fgTeltxtEn = 0x0;
            TVE_WRITE32(TVE1_REG_TT_CTL, rTveMainReg.uTT_ctrl.u4TT_ctrl);

            rTveMainReg.uEncMode.rEncMode.fgTTRst = 0x0;
            TVE_WRITE32(TVE1_REG_ENCMOD, rTveMainReg.uEncMode.u4EncMode);
            LOG(5, "TTX output Off");
        }
    }
    else                    // TVE_2 (Aux)
    {
        LOG(7, "Only 1 TVE\n");
    }

    return;
}

/*--------------------------------------------
// Send to TVE TTX queue
//-------------------------------------------- */
UCHAR TVE_HalSend2TTXQue(UCHAR ucTveId, const UCHAR *pu1Buf, UINT32 length)
{
    UINT32 i, j, idx;
    const UCHAR *pucData;
    UINT8 ucLineOffset;
    UINT32 u4DataOffset;
    UINT32 u4ValidPackets, u4TotalPackets;
    UINT8 _pre_field_parity;
#if 1
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }
    
    if (ucTveId == TVE_1)
    {
        u4ValidPackets = 0;
        
        u4TotalPackets = length / TTX_PACKET_SIZE;
        _pre_field_parity = *pu1Buf;//keep field info
        _arTveTTXBuf[_u4MainTTXWp].ucField = *pu1Buf;
        
        for (j = 0; j < u4TotalPackets; j++)
        {
            idx = (j * TTX_PACKET_SIZE);
            u4ValidPackets++;
            pucData = pu1Buf + idx;
    
            LOG(8, "field_parity = %d\n",*pucData);
            LOG(8, "line_offset = %d\n",*(pucData+1));
            LOG(8, "framing_code = %d\n",*(pucData+2));
        
            if(_pre_field_parity != *pucData)
            {
                _pre_field_parity = *pucData;

                _u4MainTTXWp++;      // update write ptr   
                LOG(8, "_u4MainTTXWp=%d!!!: _pre_field_parity\n",_u4MainTTXWp);
        // check write ptr boundary
                if (_u4MainTTXWp >= TVE_TTX_BUF_NUM)
        {
                    _u4MainTTXWp = 0;
                }
                if(_u4MainTTXRp == _u4MainTTXWp)
                {
                    LOG(3, "TTX buffer overflow!!!\n");
                    return TVE_SET_VBI_OVERFLOW;
                }
            
                _arTveTTXBuf[_u4MainTTXWp].ucField = *pucData;
                //clean TTX buf
                for(i = 0; i < TVE_TTX_BUF_LINENUM; i++)
                {
                    #ifdef LINUX_TURNKEY_SOLUTION
                    *(_arTveTTXBuf[_u4MainTTXWp].ucData + (i*TVE_TTX_BUF_DATAPERLINE)) = _TTX_CTRL_DW_OFF;
                    #else
                    _arTveTTXBuf[_u4MainTTXWp].ucData[i * TVE_TTX_BUF_DATAPERLINE] = _TTX_CTRL_DW_OFF;
                    #endif
                }
            }
            //even field-> line offset+1 for HW design
            //ucLineOffset = (*pucData == 0) ? *(pucData + 1) + 1: *(pucData + 1);
            ucLineOffset = *(pucData + 1);//5363 fix line offset issue
            if((ucLineOffset - rTveMainReg.uTeltxt3.rTeltxt3.u4TTBgnLine) < TVE_TTX_BUF_LINENUM)
            {
                //line_offset info
                u4DataOffset = (UINT32)(ucLineOffset - rTveMainReg.uTeltxt3.rTeltxt3.u4TTBgnLine) * TVE_TTX_BUF_DATAPERLINE;
                //copy data to fifo; +1 DW is for HW control byte
                #ifdef LINUX_TURNKEY_SOLUTION
                {
                    UINT32* pTveDmaAddr = _arTveTTXBuf[_u4MainTTXWp].ucData+u4DataOffset+1;
                    x_memcpy(pTveDmaAddr, pucData + 3, 42);
                    *(_arTveTTXBuf[_u4MainTTXWp].ucData+u4DataOffset) = _TTX_CTRL_DW_ON;
                }
                #else
                x_memcpy(&_arTveTTXBuf[_u4MainTTXWp].ucData[u4DataOffset+1], pucData + 3, 42);
               _arTveTTXBuf[_u4MainTTXWp].ucData[u4DataOffset] = _TTX_CTRL_DW_ON;
                #endif
            }
            //for(i = 0; i < TVE_TTX_BUF_DATAPERLINE; i++)
            //{
            //    LOG(9, "_arTveTTXBuf[%d].ucData[%x]\n",_u4MainTTXWp, _arTveTTXBuf[_u4MainTTXWp].ucData[data_offset+i]);
            //}
        }
        LOG(8, "u4ValidPackets = %d\n",u4ValidPackets);
        
        _u4MainTTXWp++;      // update write ptr   
        LOG(8, "_u4MainTTXWp=%x!!!\n",_u4MainTTXWp);
        // check write ptr boundary
        if (_u4MainTTXWp >= TVE_TTX_BUF_NUM)
        {
            _u4MainTTXWp = 0;
        }
        if(_u4MainTTXRp == _u4MainTTXWp)
        {
            LOG(3, "TTX buffer overflow!!!\n");
            return TVE_SET_VBI_OVERFLOW;
        }
            
        //clean TTX buf
        for(i = 0; i < TVE_TTX_BUF_LINENUM; i++)
        {
            #ifdef LINUX_TURNKEY_SOLUTION
            *(_arTveTTXBuf[_u4MainTTXWp].ucData+(i*TVE_TTX_BUF_DATAPERLINE)) = _TTX_CTRL_DW_OFF;
            #else
            _arTveTTXBuf[_u4MainTTXWp].ucData[i * TVE_TTX_BUF_DATAPERLINE] = _TTX_CTRL_DW_OFF;
            #endif
        }
    }
    else            // TVE_2 (Aux)
    {
        LOG(7, "Only 1 TVE\n");
    }

//    PMX_HalSetCc(ucTveId, 1);   // enable ISR callback
#endif
    return TVE_SET_OK;
}

/*--------------------------------------------
// 
//-------------------------------------------- */
void TVE_HalSendTTXIsr(UCHAR ucTveId, UCHAR ucIsOddField, UINT32 u4PmxPts)
{
    UCHAR ucField;
    UINT32 u4address;
    if (ucTveId >= TVE_MAX_NS)
    {
        return;
    }

    if (ucTveId == TVE_1)
    {
        // fifo empty
        if (_u4MainTTXRp == _u4MainTTXWp)
        {
           //TODO
            _TveHalTTXIdle(ucTveId, ucIsOddField);
            return;
        }

        //field_parity in TTX spec. 1: ODD; 0: EVEN
        ucField = (_arTveTTXBuf[_u4MainTTXRp].ucField == 1) ? TVE_FIELD_ODD : TVE_FIELD_EVEN;
#if 1
        if (ucField == TVE_FIELD_ODD)
        {
            if (!ucIsOddField)
            {
                LOG(3, "ISR Not ODD (%d)(%d)\n", ucIsOddField, ucField);
                vDrvTveIncFieldCnt(_u1TveFieldMisMatchCnt);
                if(_u1TveFieldMisMatchCnt==TVE_FIELD_MATCH_TH)
                {
                    LOG(3, "Ignore TVE_ISR()\n");
                    _u1TveFieldMatchCnt = 0;
                    return;
                }
            }
            else
            {
                vDrvTveIncFieldCnt(_u1TveFieldMatchCnt);
                if(_u1TveFieldMatchCnt==TVE_FIELD_MATCH_TH)
                {
                    _u1TveFieldMisMatchCnt = 0;
                }
            }
        }

        if (ucField == TVE_FIELD_EVEN)
        {
            if (ucIsOddField)
            {
                LOG(3, "ISR_NOT EVEN (%d)(%d)\n", ucIsOddField, ucField);
                vDrvTveIncFieldCnt(_u1TveFieldMisMatchCnt);
                if(_u1TveFieldMisMatchCnt==TVE_FIELD_MATCH_TH)
                {
                    LOG(3, "Ignore TVE_ISR()\n");
                    _u1TveFieldMatchCnt = 0;
                    return;
                }
            }
            else
            {
                vDrvTveIncFieldCnt(_u1TveFieldMatchCnt);
                if(_u1TveFieldMatchCnt==TVE_FIELD_MATCH_TH)
                {
                    _u1TveFieldMisMatchCnt = 0;
                }
            }
        }
#endif
        // Set TVE HW Registers
        #ifdef LINUX_TURNKEY_SOLUTION
        u4address = (UINT32)(PHYSICAL((UINT32)(_arTveTTXBuf[_u4MainTTXRp].ucData)));
        LOG(8, "Read _arTveTTXBuf[%d] address = %x\n",_u4MainTTXRp, PHYSICAL((UINT32)(_arTveTTXBuf[_u4MainTTXRp].ucData)));
        #else
        u4address = (UINT32)_arTveTTXBuf[_u4MainTTXRp].ucData;
        LOG(8, "Read _arTveTTXBuf[%d] address = %x\n",_u4MainTTXRp, _arTveTTXBuf[_u4MainTTXRp].ucData);
        #endif
        rTveMainReg.uTeltxt1.rTeltxt1.u4TeltxtAddr = u4address>>2;
        TVE_WRITE32(TVE1_REG_TELTXT1, rTveMainReg.uTeltxt1.u4Teltxt1);

        _u4MainTTXRp++;     // update read ptr
        
        // check read ptr boundary
        if (_u4MainTTXRp >= TVE_TTX_BUF_NUM)
        {
            _u4MainTTXRp = 0;
        }
    }
    else            // TVE_2 (Aux)
    {
        LOG(7, "Only 1 TVE\n");
    }
}

/*--------------------------------------------
// Enable WSS
//-------------------------------------------- */
UINT32 TVE_HalEnableWss(UCHAR ucTveId, UCHAR ucTveFmt)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        if (ucTveFmt == TVE_FMT_480I)
        {
            rTveMainReg.uWss480i.rWss480i.u4WssLvl = 0xC8;     /* 500 mv */
            rTveMainReg.uWss480i.rWss480i.u4Word1 = 0xf;	//default
            rTveMainReg.uWss480i.rWss480i.fgWssOn = 1;

            TVE_WRITE32(TVE1_REG_WSSI, rTveMainReg.uWss480i.u4Wss480i);
        }
        else if (ucTveFmt == TVE_FMT_576I)
        {
            rTveMainReg.uWss576i.rWss576i.u4WssLvl = 0xC8;     /* 500 mv */
            rTveMainReg.uWss576i.rWss576i.fgWssOn = 1;

            TVE_WRITE32(TVE1_REG_WSSI, rTveMainReg.uWss576i.u4Wss576i);
        }
        else
        {
            u4RetVal = TVE_SET_ERROR;
        }
    }
    else    /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}

/*--------------------------------------------
// Disable WSS
//-------------------------------------------- */
UINT32 TVE_HalDisableWss(UCHAR ucTveId, UCHAR ucTveFmt)
{
    UINT32 u4RetVal = TVE_SET_OK;

    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        if (ucTveFmt == TVE_FMT_480I)
        {
            rTveMainReg.uWss480i.rWss480i.u4WssLvl = 0x0;     /* 0 mv */
            rTveMainReg.uWss480i.rWss480i.fgWssOn = 0;

            TVE_WRITE32(TVE1_REG_WSSI, rTveMainReg.uWss480i.u4Wss480i);
        }
        else if (ucTveFmt == TVE_FMT_576I)
        {
            rTveMainReg.uWss576i.rWss576i.u4WssLvl = 0x0;     /* 0 mv */
            rTveMainReg.uWss576i.rWss576i.fgWssOn = 0;

            TVE_WRITE32(TVE1_REG_WSSI, rTveMainReg.uWss576i.u4Wss576i);
        }
        else
        {
            u4RetVal = TVE_SET_ERROR;
        }
    }
    else    /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}

/*--------------------------------------------
// Set WSS Aspect Ratio
//-------------------------------------------- */
UINT32 TVE_HalSetAspectRatio(UCHAR ucTveId, UCHAR ucTveFmt,
                             UCHAR ucAspectRatio)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        if (ucTveFmt == TVE_FMT_480I)
        {
            rTveMainReg.uWss480i.rWss480i.u4Word0 = ucAspectRatio;//_uc525LAsrTable[ucAspectRatio];

            TVE_WRITE32(TVE1_REG_WSSI, rTveMainReg.uWss480i.u4Wss480i);
        }
        else if (ucTveFmt == TVE_FMT_576I)
        {
            rTveMainReg.uWss576i.rWss576i.u4Wss = ucAspectRatio;//_uc625LAsrTable[ucAspectRatio];

            TVE_WRITE32(TVE1_REG_WSSI, rTveMainReg.uWss576i.u4Wss576i);
        }
        else
        {
            u4RetVal = TVE_SET_ERROR;
        }
    }
    else    /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}

/*--------------------------------------------
// Set CGMS-A Value
//-------------------------------------------- */
UINT32 TVE_HalSetCgmsaValue(UCHAR ucTveId, UCHAR ucTveFmt, UCHAR ucValue)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        if (ucTveFmt == TVE_FMT_480I)
        {
            rTveMainReg.uWss480i.rWss480i.u4Word1 = 0x0;
            rTveMainReg.uWss480i.rWss480i.u4Word2_b6b7 = _ucCGMSATable[ucValue&0x3];//CGMS-A
            rTveMainReg.uWss480i.rWss480i.u4Word2_b8b9 = _ucCGMSATable[(ucValue>>2)&0x3]; //MV PSP
            rTveMainReg.uWss480i.rWss480i.u4Word2_b10 = 0;

            TVE_WRITE32(TVE1_REG_WSSI, rTveMainReg.uWss480i.u4Wss480i);
        }
        else if (ucTveFmt == TVE_FMT_576I)
        {
            rTveMainReg.uWss576i.rWss576i.u4Cgmsa= _ucCGMSATable[ucValue&0x3];

            TVE_WRITE32(TVE1_REG_WSSI, rTveMainReg.uWss576i.u4Wss576i);
        }
        else
        {
            u4RetVal = TVE_SET_ERROR;
        }
    }
    else    /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}

/*--------------------------------------------
// Set WSS Value
//-------------------------------------------- */
UINT32 TVE_HalSetWSSValue(UCHAR ucTveId, UCHAR ucTveFmt, const UCHAR *pucData)
{
    UINT32 u4RetVal = TVE_SET_OK;
    UINT32 u4Value;
    
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        if (ucTveFmt == TVE_FMT_480I)
        {
            u4Value = (UINT32)(*pucData) |((UINT32)(*(pucData+1))<<8) |((UINT32)(*(pucData+2))<<16);  
            if(DRVCUST_OptGet(eTVESupportAFD))
            {
                u4Value = BitInverse(u4Value, 20);
            }
            u4Value = u4Value | (TVE_READ32(TVE1_REG_WSSI) & 0xfff00000);
            rTveMainReg.uWss480i.u4Wss480i = u4Value;
            TVE_WRITE32(TVE1_REG_WSSI, u4Value);
        }
        else if (ucTveFmt == TVE_FMT_576I)
        { 
            u4Value = (UINT32)(*pucData) |((UINT32)(*(pucData+1)))<<8;  
            if(DRVCUST_OptGet(eTVESupportAFD))
            {
                u4Value = BitInverse(u4Value, 14);
            }
            u4Value = u4Value | (TVE_READ32(TVE1_REG_WSSI) & 0xffffc000);
            rTveMainReg.uWss576i.u4Wss576i = u4Value;
            TVE_WRITE32(TVE1_REG_WSSI, u4Value);
        }
        else
        {
            u4RetVal = TVE_SET_ERROR;
        }
    }
    else    /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}

#ifdef CC_TVE_INSERT_VPS//  for vps

UINT32 TVE_HalEnableVPS(UCHAR ucTveId, UCHAR ucEnable)
{
	UINT32 u4RetVal = TVE_SET_OK;
	if(ucTveId == TVE_1)
	{
		if(ucEnable == TRUE)
		{
			rTveMainReg.uVpsCtrl1.rVpsCtrl1.fgVpsOn = 0x01;
			rTveMainReg.uVpsCtrl1.rVpsCtrl1.fgWaveformType = 0x0;
			rTveMainReg.uVpsCtrl1.rVpsCtrl1.u4VpsP1 = 0xbd ;
			rTveMainReg.uVpsCtrl1.rVpsCtrl1.u4VpsP2 = 0x5ce ;
			rTveMainReg.uVpsData1.u4VpsData1 = 0x0;
			rTveMainReg.uVpsData2.u4VpsData2 = 0x0;
			rTveMainReg.uVpsData3.u4VpsData3 = 0x0;
			rTveMainReg.uVpsCtrl2.rVpsCtrl2.u4VpsData4 = 0x0;	
			rTveMainReg.uVpsCtrl2.rVpsCtrl2.u4VpsBgnPxl = 0x0 ;
			rTveMainReg.uVpsCtrl2.rVpsCtrl2.u4VpsEndPxl = 0x13 ;
			rTveMainReg.uVpsCtrl2.rVpsCtrl2.u4VpsBgnLine = 0x10 ;
			TVE_WRITE32(TVE1_REG_VPS_CTRL1, rTveMainReg.uVpsCtrl1.u4VpsCtrl1);
			TVE_WRITE32(TVE1_REG_VPS_DATA1, rTveMainReg.uVpsData1.u4VpsData1);	
			TVE_WRITE32(TVE1_REG_VPS_DATA2, rTveMainReg.uVpsData2.u4VpsData2);	
			TVE_WRITE32(TVE1_REG_VPS_DATA3, rTveMainReg.uVpsData3.u4VpsData3);	
			TVE_WRITE32(TVE1_REG_VPS_DATA4_VPS_CTRL2, rTveMainReg.uVpsCtrl2.u4VpsCtrl2);
			LOG(0, "TVE VPS ON\n");
		}
		else
		{
			rTveMainReg.uVpsCtrl1.rVpsCtrl1.fgVpsOn = 0x0;		
			TVE_WRITE32(TVE1_REG_VPS_CTRL1, rTveMainReg.uVpsCtrl1.u4VpsCtrl1);	
			LOG(5, "TVE VPS OFF\n");
		}
	}
	else
	{
		LOG(7, "Only one tve\n");
	}
	return u4RetVal;
}


void TVE_BitInverse(UINT32 u4InputData, UINT32 *pu4OutputData);

UINT32 TVE_HalSendVPSValue(UCHAR uTveId, const UCHAR *pucData)
{
	UINT32 u4RetVal = TVE_SET_OK;
	UINT32 u4TempValue = 0, u4InverseRealData = 0;
	
	if(uTveId >= TVE_MAX_NS)
	{
		return TVE_SET_ERROR;
	}
	if(uTveId == TVE_1)
	{
		UINT8 i = 0, j = 0;
		for(; i<4; i++)
		{
			if(j == 12)
			{
				u4TempValue = (UINT32)pucData[j];
				TVE_BitInverse(u4TempValue, &u4InverseRealData);
				u4InverseRealData = u4InverseRealData | (TVE_READ32(TVE1_REG_VPS_DATA4_VPS_CTRL2) & 0x00ffffff);
				TVE_WRITE32(TVE1_REG_VPS_DATA4_VPS_CTRL2,u4InverseRealData);
				//u4InverseRealData = u4InverseRealData >> 24;
				//TVE_WRITE8((TVE1_REG_VPS_DATA4_VPS_CTRL2 + 3), (UINT8)u4InverseRealData);  // wirte fail: the result is 0x30303030. why?
				break;
			}
			u4TempValue = (UINT32)(pucData[j]) | (UINT32)(pucData[j+1] << 8) | (UINT32)(pucData[j+2] << 16) | (UINT32)(pucData[j+3] << 24);
			TVE_BitInverse(u4TempValue, &u4InverseRealData);
			TVE_WRITE32(TVE1_REG_VPS_DATA1 + i*4, u4InverseRealData);
			j = j + 4;
		}
	}
	else
	{
		LOG(7, "Only One TVE \n");
	}
	return u4RetVal;

}

void TVE_BitInverse(UINT32 u4InputData, UINT32 *pu4OutputData)
{
	UINT8 i = 0; //j = 0;
	UINT32 u4TempValue = 0, u4TempResult = 0;

	for(; i<32; i++)
	{
		u4TempValue = u4InputData & 1;
		u4InputData = u4InputData >> 1;
		u4TempResult =  u4TempResult | (u4TempValue << (31 - i));	
	}
	*pu4OutputData = u4TempResult;
}
#endif

/*
VOID TVE_HalAnalogVDACPowerOn(UCHAR ucDAC, UCHAR ucOn)
{
    #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    if(ucDAC == DAC_1)
    {
        if(ucOn)
        {
            vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_BGREF_PWD);
            vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_DACX_PWD);
        }
        else
        {
            vIO32WriteFldAlign(REG_VDAC_CFG0, 1, RG_DACX_PWD);
        }
    }
    else
    {        
        if(ucOn)
        {
            vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_BGREF_PWD);
            vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_DACX_PWD);
        }
        else
        {
            vIO32WriteFldAlign(REG_VDAC_CFG0, 1, RG_DACX_PWD);
        }
    }
    if(IO32ReadFldAlign(REG_VDAC_CFG0, RG_DACX_PWD) && IO32ReadFldAlign(REG_VDAC_CFG0, RG_DACX_PWD))
    {
        vIO32WriteFldAlign(REG_VDAC_CFG0, 1, RG_BGREF_PWD);
    }
    #endif
    return;
}
*/

VOID TVE_HalVDACClkPowerOn(UCHAR ucOn)
{
    #if defined(CC_MT5396)
    if(ucOn)
    {
        vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG0, 1, FLD_VDAC_CKEN);
        vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG0, 0, FLD_VDAC_RST);
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG0, 1, FLD_VDAC_RST);
        vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG0, 0, FLD_VDAC_CKEN);
    }
    #elif (defined(CC_MT5368) || defined(CC_MT5389))
    if(ucOn)
    {
        vIO32WriteFldAlign(CKGEN_CLK_PDCFG2, 1, FLD_VDAC_CKEN);
        vIO32WriteFldAlign(CKGEN_CLK_PDCFG0, 0, FLD_VDAC_RST);
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_CLK_PDCFG0, 1, FLD_VDAC_RST);
        vIO32WriteFldAlign(CKGEN_CLK_PDCFG2, 0, FLD_VDAC_CKEN);
    }
    #endif
    return;
}

VOID TVE_HalTVEClkPowerOn(UCHAR ucOn)
{
    #if defined(CC_MT5396)
    if(ucOn)
    {
        vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG0, 1, FLD_TVE_CKEN);
        vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG0, 0, FLD_TVE_RST);
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG0, 1, FLD_TVE_RST);
        vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG0, 0, FLD_TVE_CKEN);
    }
    #elif (defined(CC_MT5368) || defined(CC_MT5389))
    if(ucOn)
    {
        vIO32WriteFldAlign(CKGEN_CLK_PDCFG0, 1, FLD_TVE_CKEN);
        vIO32WriteFldAlign(CKGEN_CLK_PDCFG2, 0, FLD_TVE_RST);
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_CLK_PDCFG2, 1, FLD_TVE_RST);
        vIO32WriteFldAlign(CKGEN_CLK_PDCFG0, 0, FLD_TVE_CKEN);
    }
    #endif
    return;
}


/*--------------------------------------------
// Power on/off DAC
//-------------------------------------------- */
UINT32 TVE_HalDACPowerOn(UCHAR ucTveId, UCHAR ucDAC, UCHAR ucOn)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
        vIO32WriteFldAlign(REG_VDAC_CFG0, 0x10, RG_TRIM_VAL); //about 1000mV
        #endif
        if(ucDAC == DAC_1)
        {
            rTveMainReg.uDacControl.rDacControl.u4DAC1 = ucOn ? 0x3 : 0x0; 
           // #if !defined(CC_MT5368) && !defined(CC_MT5396) && !defined(CC_MT5389)           
            if(_fgDescramblerOn)
            rTveMainReg.uDacControl.rDacControl.u4DAC1 = 0x03; 
            //#endif
            //rTveMainReg.uDacControl.rDacControl.u4DaTrim = 0x1F;  //Trim value
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            if(_fgDescramblerOn || ucOn)
            {
                vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_DACX_PWD);
            }
            else
            {
                vIO32WriteFldAlign(REG_VDAC_CFG0, 1, RG_DACX_PWD);
            }
            #endif
        }
        else
        {
            rTveMainReg.uDacControl.rDacControl.u4DAC2 = ucOn ? 0x3 : 0x0;//use DAC1 as monitor scart
           // #if !defined(CC_MT5368) && !defined(CC_MT5396) && !defined(CC_MT5389)            
            if(_fgDescramblerOn)
            rTveMainReg.uDacControl.rDacControl.u4DAC2 =0x03; 
            //#endif
            //rTveMainReg.uDacControl.rDacControl.u4DaTrim = 0x1F;  //Trim value
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            if(_fgDescramblerOn || ucOn)
            {
                vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_DACY_PWD);
            }
            else
            {
                vIO32WriteFldAlign(REG_VDAC_CFG0, 1, RG_DACY_PWD);
            }
            #endif
        }
	if(( rTveMainReg.uDacControl.rDacControl.u4DAC1 == 0) && ( rTveMainReg.uDacControl.rDacControl.u4DAC2 == 0))
	{
	       #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
		vIO32WriteFldAlign(REG_VDAC_CFG0, 1, RG_BGREF_PWD);
		#else
		rTveMainReg.uDacControl.rDacControl.fgBGRef_off = 0x1;
              #endif
	}
	else
	{
		#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
		vIO32WriteFldAlign(REG_VDAC_CFG0, 0, RG_BGREF_PWD);
              #else
              rTveMainReg.uDacControl.rDacControl.fgBGRef_off = 0x0;
		#endif
	}
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_DACTRL, rTveMainReg.uDacControl.u4DacCtrl);
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}

UINT32 TVE_HalResetSyncEn(UCHAR ucTveId)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uSyncCtrl.rSyncCtrl.fgSyncEn = 0;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_SYNC_CTRL, rTveMainReg.uSyncCtrl.u4SyncCtrl);
        rTveMainReg.uSyncCtrl.rSyncCtrl.fgSyncEn = 1;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_SYNC_CTRL, rTveMainReg.uSyncCtrl.u4SyncCtrl);
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}

#if defined(CC_MT5363)||defined(CC_TVE6595)
/*--------------------------------------------
// Enable/Disable PIP mixer
//-------------------------------------------- */
UINT32 TVE_HalPIPMixerOn(UCHAR ucTveId, UCHAR ucOn)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uMuxCtrl.rMuxCtrl.fgPipMixerOn = ucOn ? 0x1 : 0x0; 
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_MUX_CTRL, rTveMainReg.uMuxCtrl.u4MuxCtrl);
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}
#endif

#if 0
/*--------------------------------------------
// Power down DAC
//-------------------------------------------- */
UINT32 TVE_HalDACPowerDown(UCHAR ucTveId)
{
    UINT32 u4RetVal = TVE_SET_OK;
    
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uDacControl.rDacControl.u4DaMain = 0x0;

        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_DACTRL, rTveMainReg.uDacControl.u4DacCtrl);
    }
    else /* TVE_2 (Aux) */
    {
        LOG(7, "Only 1 TVE\n");
    }

    return u4RetVal;
}
#endif

UINT32 TVE_HalGetFieldInfo(UCHAR ucTveId)
{
    //UINT8 FieldInfo;

    if (ucTveId == TVE_1)
    {
        return (TVE_READ32(TVE1_REG_ENCMOD) & 0x00400000);
    }

    return TVE_SET_ERROR;
}

UINT32 TVE_HalGetFieldInvStatus(void)
{
    LOG(9, "TVE1_REG_ENCSYN=0x%8X\n", TVE_READ32(TVE1_REG_ENCSYN));
    return (TVE_READ32(TVE1_REG_ENCSYN) & FIELD_INV_STATUS);
}


/*--------------------------------------------
// Reset TVE CC queue Read and Write ptrs
//--------------------------------------------*/
UINT32 TVE_HalResetCcFifo(UCHAR ucTveId)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        _u4MainCcRp = 0;
        _u4MainCcWp = 0;
        //_u4IsMainCcSyncToPts = 0;
    }
    else
    {
        LOG(7, "Only 1 TVE\n");
    }

    return TVE_SET_OK;
}

/*--------------------------------------------
// Reset V-Chip setting
//--------------------------------------------*/
void TVE_HalResetVChip(void)
{
    _bVChipHeadAppear = FALSE;
    _bVChipDataAppear = FALSE;
}

/*--------------------------------------------
// src select 0: PIP 1:Vdoin
//-------------------------------------------- */
UINT32 TVE_HalSrcSelect(UCHAR ucTveSrc)
{
    UINT32 u4RetVal = TVE_SET_OK;

//    if (ucTveId >= TVE_MAX_NS)
//    {
//        return TVE_SET_ERROR;
//    }
    if (ucTveSrc == 0)
	{
        rTveMainReg.uMuxCtrl.rMuxCtrl.fgCVBSMuxEn = 0x00;
        rTveMainReg.uMuxCtrl.rMuxCtrl.fgCVBSP2IEn = 0x01;
        rTveMainReg.uMuxCtrl.rMuxCtrl.fgPipSyncOff = 0x00;
	}
    else
	{
      rTveMainReg.uMuxCtrl.rMuxCtrl.fgCVBSMuxEn = 0x01;
        rTveMainReg.uMuxCtrl.rMuxCtrl.fgCVBSP2IEn = 0x00;
        rTveMainReg.uMuxCtrl.rMuxCtrl.fgPipSyncOff = 0x00;
	}

    /* Set TVE HW MAIN Registers */
    TVE_WRITE32(TVE1_REG_MUX_CTRL, rTveMainReg.uMuxCtrl.u4MuxCtrl);

    return u4RetVal;
	}

/*--------------------------------------------
// Set TVE sync_time0, adj_h_total & adj_v_total
//--------------------------------------------*/
UINT32 TVE_HalSetSyncTime0(UCHAR ucTveId, UCHAR ucOn, UINT16 vtotal, UINT16 htotal)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uSyncCtrl.rSyncCtrl.fgAdjTotalEn = ucOn;
        rTveMainReg.uSyncTime0.rSyncTime0.u4AdjHTotal = htotal;
        rTveMainReg.uSyncTime0.rSyncTime0.u4AdjVTotal = vtotal;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_SYNC_CTRL, rTveMainReg.uSyncCtrl.u4SyncCtrl);
        TVE_WRITE32(TVE1_REG_SYNC_TIME0, rTveMainReg.uSyncTime0.u4SyncTime0);
    }
    else
    {
        LOG(7, "Only 1 TVE\n");
    }

    return TVE_SET_OK;
}

#if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
/*--------------------------------------------
// Get TVE sync_time1, adj_hsync & adj_vsync
//--------------------------------------------*/
UINT32 TVE_HalGetSyncTime1(UCHAR ucTveId, UCHAR ucFmt, UCHAR bSrc, UINT16* vsync, UINT16* hsync)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        UINT8 idx1, idx2;
        UINT8 u1VdoIsInterlace;
        UINT16 u2VdoInputHeight;
    
        if(ucFmt == TVE_FMT_480I)
        {
            idx1 = 0;
        }
        else
        {
            idx1 = 1;
        }
        #ifdef TVE_WA
        u1VdoIsInterlace = bDrvVideoIsSrcInterlace(SV_VP_PIP);
        u2VdoInputHeight = wDrvVideoInputHeight(SV_VP_PIP);
        if (u1VdoIsInterlace && (u2VdoInputHeight == 480 || u2VdoInputHeight == 576))
        {
            if(ucFmt == TVE_FMT_480I)
            {
                idx1 = 2;
            }
            else
            {
                idx1 = 3;
            }
        }
        #endif
        switch(bSrc)
        {
            case SV_VD_YPBPR:
                idx2 = 0;
            break;
            case SV_VD_VGA:
                idx2 = 1;
                break;
            case SV_VD_DVI:
                idx2 = 2;
                break;
            case SV_VD_MPEGHD:
            case SV_VD_MPEGSD:
            default:
                idx2 = 3;
                break;
        }
    
        *vsync = TVE_Sync_Time[idx1][idx2][0];
        *hsync = TVE_Sync_Time[idx1][idx2][1];
    }
    else
    {
        LOG(7, "Only 1 TVE\n");
    }

    return TVE_SET_OK;
}
#endif

/*--------------------------------------------
// Set TVE sync_time1, adj_hsync & adj_vsync
//--------------------------------------------*/
UINT32 TVE_HalSetSyncTime1(UCHAR ucTveId, UINT16 vsync, UINT16 hsync)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uSyncTime1.rSyncTime1.u4AdjVsync = vsync;
        rTveMainReg.uSyncTime1.rSyncTime1.u4AdjHsync = hsync;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_SYNC_TIME1, rTveMainReg.uSyncTime1.u4SyncTime1);
    }
    else
    {
        LOG(7, "Only 1 TVE\n");
    }

    return TVE_SET_OK;
}

#if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
/*--------------------------------------------
// Set TVE sync_time2, adj_line_oend & adj_lin_ostart
//--------------------------------------------*/
UINT32 TVE_HalSetSyncTime2(UCHAR ucTveId, UCHAR ucOn, UINT16 oend, UINT16 ostart)
{
    UINT32 synctime2;
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    synctime2 = ((UINT32)oend<<16) | (UINT32)ostart;
    if (ucTveId == TVE_1)
    {
        rTveMainReg.uSyncCtrl.rSyncCtrl.fgSelfYActiveEn = ucOn;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_SYNC_CTRL, rTveMainReg.uSyncCtrl.u4SyncCtrl);
        TVE_WRITE32(TVE1_REG_SYNC_TIME2, synctime2);
    }
    else
    {
        LOG(7, "Only 1 TVE\n");
    }

    return TVE_SET_OK;
}
#endif

/*--------------------------------------------
// Set TVE disp area, disp_end_pxl, disp_bgn_pxl and new_din_en
//--------------------------------------------*/
UINT32 TVE_HalSetDispArea(UCHAR ucTveId, UCHAR ucOn, UINT16 bgn, UINT16 end)
{
    if (ucTveId >= TVE_MAX_NS)
    {
        return TVE_SET_ERROR;
    }

    if (ucTveId == TVE_1)
    {
        rTveMainReg.uDispArea.rDispArea.fgNewDinSel = ucOn;
        rTveMainReg.uDispArea.rDispArea.u4DispBgnPxl = bgn;
        rTveMainReg.uDispArea.rDispArea.u4DispEndPxl = end;
        /* Set TVE HW MAIN Registers */
        TVE_WRITE32(TVE1_REG_DISP_AREA, rTveMainReg.uDispArea.u4DispArea);
    }
    else
    {
        LOG(7, "Only 1 TVE\n");
    }

    return TVE_SET_OK;
}

/*--------------------------------------------
// Set TVE Mixer data source
// Mixer 0: Mixer1; 1: Mixer2
// bSrc 0: CVBS ADC; 1: Demod ADC 
//--------------------------------------------*/
UINT32 TVE_HalSetMixerSrc(UCHAR Mixer, UCHAR bSrc)
{
    if(Mixer == TVE_Mixer1)
    {
        if(bSrc == TVE_VDOIN_MIXER)
        {
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgMixer1Sel = 0;
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFMixer1Sel = 0;
        }
        else if(bSrc == TVE_DEMOD_MIXER)
        {
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgMixer1Sel = 1;
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFMixer1Sel = 0;
        }
        else
        {
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFMixer1Sel = 1;
            #if defined(CC_MT5363) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFClockInv=0;
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFDataIdxInv=0;
            #endif
        }
    }
    else
    {
        if(bSrc == TVE_VDOIN_MIXER)
        {
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgMixer2Sel = 1;
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFMixer2Sel = 0;
        }
        else if(bSrc == TVE_DEMOD_MIXER)
        {
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgMixer2Sel = 0;
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFMixer2Sel = 0;
        }
        else
        {
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFMixer2Sel = 1;
            #if defined(CC_MT5363) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFClockInv=0;
            rTveMainReg.uMuxCtrl.rMuxCtrl.fgSIFDataIdxInv=0;
            #endif
        }
    }
    /* Set TVE HW MAIN Registers */
    TVE_WRITE32(TVE1_REG_MUX_CTRL, rTveMainReg.uMuxCtrl.u4MuxCtrl);

    return TVE_SET_OK;
}

/*--------------------------------------------
// Set TVE Mixer DAC sel
// DAC 0: DAC1; 1: DAC2
// bSrc 0: TVE; 1: Bypass
//--------------------------------------------*/
UINT32 TVE_HalSetMixerDacSel(UCHAR DAC, UCHAR bSrc)
{
    if(DAC == DAC_1)
    {
        rTveMainReg.uMuxCtrl.rMuxCtrl.fgMixerDac1Sel = bSrc;
    }
    else
    {
        rTveMainReg.uMuxCtrl.rMuxCtrl.fgMixerDac2Sel = bSrc;
    }
    /* Set TVE HW MAIN Registers */
    TVE_WRITE32(TVE1_REG_MUX_CTRL, rTveMainReg.uMuxCtrl.u4MuxCtrl);

    return TVE_SET_OK;
}

/*--------------------------------------------
// Get TVE PIP/VDOIN Input Src
// bSrc:  SV_VD_YPBPR, SV_VD_DVI, SV_VD_VGA, SV_VD_MPEGHD
//--------------------------------------------*/
UINT32 TVE_HalSetInputSrc(UCHAR* bSrc)
{
    *bSrc = _ucTveInputSrc;
    
    return TVE_SET_OK;
}

/*TVEM_SEL
0000 Select TVD to tvem output path.
0001 Select 0 to tvem output path.
0010 Select HDTV to tvem output path.
0011 Select VGA to tvem output path.
0100 Select CCIR to tvem output path.
0101 Select CCIR to tvem output path.
0110 Select DVI to tvem output path.
0111 Select DVI to tvem output path.
1xx0 Select MPEG1 to tvem output path
1xx1 Select MPEG2 to tvem output path
*/

void Bypass_HalTVEMSel(UCHAR bSrc)
{
    switch(bSrc)
    {
        case SV_VD_TVD3D:
	     vRegWriteFldAlign(OMUX_00, 0, OMUX_TVEM_SEL);
            break;
        case SV_VD_YPBPR:
	     vRegWriteFldAlign(OMUX_00, 0x02, OMUX_TVEM_SEL);
            break;
        case SV_VD_DVI:
	     vRegWriteFldAlign(OMUX_00, 0x06, OMUX_TVEM_SEL);
            break;
        case SV_VD_VGA:
	     vRegWriteFldAlign(OMUX_00, 0x03, OMUX_TVEM_SEL);
            break;
        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:
	 default:
	     vRegWriteFldAlign(OMUX_00, 0x08, OMUX_TVEM_SEL);
	     break;
    }
}

void TVELoadRegDwrdTbl(TVE_RRegDwrdTbl const *pRegDwrdTbl)
{
    if (pRegDwrdTbl == NULL)
    {
        return;
    }

    while (pRegDwrdTbl->wReg != 0xFFFF)
    {
        TVE_WRITE32(pRegDwrdTbl->wReg, pRegDwrdTbl->dwValue);
        pRegDwrdTbl++;
    }
}

void Bypass_HalTVESyncTime(UCHAR bSrc, UCHAR ucFmt)
{
    UINT8 idx1, idx2;
    #ifdef TVE_WA
    UINT8 u1VdoIsInterlace;
    UINT16 u2VdoInputHeight;
    #endif
    
    if(ucFmt == TVE_FMT_480I)
    {
        idx1 = 0;
    }
    else
    {
        idx1 = 1;
    }
    #ifdef TVE_WA
    u1VdoIsInterlace = bDrvVideoIsSrcInterlace(SV_VP_PIP);
    u2VdoInputHeight = wDrvVideoInputHeight(SV_VP_PIP);
    if (u1VdoIsInterlace && (u2VdoInputHeight == 480 || u2VdoInputHeight == 576))
    {
        if(ucFmt == TVE_FMT_480I)
        {
            idx1 = 2;
        }
        else
        {
            idx1 = 3;
        }
    }
    #endif
    switch(bSrc)
    {
        case SV_VD_YPBPR:
            idx2 = 0;
        break;
        case SV_VD_VGA:
            idx2 = 1;
            break;
        case SV_VD_DVI:
            idx2 = 2;
            break;
        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:
        default:
            idx2 = 3;
            break;
    }
    
    TVE_HalSetSyncTime1(TVE_1, TVE_Sync_Time[idx1][idx2][0], TVE_Sync_Time[idx1][idx2][1]);
}

void Bypass_HalTVEDispArea(UCHAR bSrc, UCHAR ucFmt)
{
    UINT16 wRet1;
    UINT16 u2Bgn = 0x77;
    
    wRet1 = u2MpegHdInputValidDataWidth();
    #if VIDEO_ALIGNMENT_SUPPORT        
        switch(bSrc)
        {
            case SV_VD_YPBPR:
                wRet1 = wHdtvInputWidth();
                break;

            case SV_VD_VGA:
                wRet1 = wVgaInputWidth();
                break;

            #if SUPPORT_DVI
            case SV_VD_DVI:
                wRet1 = wDviInputWidth();
                break;
            #endif

            case SV_VD_MPEGHD:
            case SV_VD_MPEGSD:
                wRet1 = u2MpegHdInputValidDataWidth();
                break;
            default:
                wRet1 = 0;
        }
        if(wRet1 == 0 || ucFmt != TVE_FMT_576I)
            TVE_HalSetDispArea(TVE_1, SV_OFF, 0, 0);
        //else if(wRet1<720-VIDEO_ALIGNMENT_OFFSET)
        //    TVE_HalSetDispArea(TVE_1, SV_ON, u2Bgn+VIDEO_ALIGNMENT_OFFSET, u2Bgn + wRet1 + 1);
        else
            TVE_HalSetDispArea(TVE_1, SV_ON, u2Bgn+VIDEO_ALIGNMENT_OFFSET, u2Bgn + 720-VIDEO_ALIGNMENT_OFFSET + 1);
    #else
        #if 0
        if(((bSrc == SV_VD_MPEGHD) || (bSrc == SV_VD_MPEGSD)) && (wRet1 < 720))
        {
            TVE_HalSetDispArea(TVE_1, SV_ON, u2Bgn, u2Bgn + wRet1 + 1);
        }
        else
        #endif
        {
            TVE_HalSetDispArea(TVE_1, SV_OFF, 0, 0);
        }
    #endif
}

/*--------------------------------------------
// TVE HW default value
//-------------------------------------------- */
TVE_RRegDwrdTbl const REGTBL_BYPASS_DEFAULT[] = {
    {TVE1_REG_ENCSYN, 0x4020034B},
    {TVE1_REG_ENCMOD, 0x00000001},
#ifdef CC_TVE_POWERDOWN_HALF
    {TVE1_REG_DACTRL, 0xDF040010},
#else
    {TVE1_REG_DACTRL, 0xDF000010},
#endif
    {TVE1_REG_WSSI, 0x00000000},
#ifdef CC_MT5363
    {TVE1_REG_MUX_CTRL, 0x00002000},
#else
    {TVE1_REG_MUX_CTRL, 0x00000000}, //switch to external REF resistor for ECO IC
#endif
    {TVE1_REG_SYNC_CTRL, 0x00540005},
    {TVE1_REG_SYNC_TIME1, 0x00010001},
    {TVE1_REG_SYNC_TIME2, 0x0106041c},
    {0xFFFF, 0x00000000},
};

void Bypass_HalClear2Default(void)
{
    TVELoadRegDwrdTbl(REGTBL_BYPASS_DEFAULT);
    rTveMainReg.uEncSyn.u4EncSyn = TVE_READ32(TVE1_REG_ENCSYN);
    rTveMainReg.uEncMode.u4EncMode = TVE_READ32(TVE1_REG_ENCMOD);
    rTveMainReg.uDacControl.u4DacCtrl = TVE_READ32(TVE1_REG_DACTRL);
    rTveMainReg.uWss576i.u4Wss576i = TVE_READ32(TVE1_REG_WSSI);
    rTveMainReg.uWss480i.u4Wss480i = TVE_READ32(TVE1_REG_WSSI);
    rTveMainReg.uMuxCtrl.u4MuxCtrl = TVE_READ32(TVE1_REG_MUX_CTRL);
    rTveMainReg.uSyncCtrl.u4SyncCtrl = TVE_READ32(TVE1_REG_SYNC_CTRL);
    rTveMainReg.uSyncTime1.u4SyncTime1 = TVE_READ32(TVE1_REG_SYNC_TIME1);
#if defined(CC_MT5363)||defined(CC_TVE6595)
    rTveMainReg.uChromaGain.u4ChromaGain = TVE_READ32(TVE1_REG_CHROMAGAIN);
#endif
}

/*--------------------------------------------
// TVE HW common value
//-------------------------------------------- */
void Bypass_HalSetCommon(void)
{
    //0x04
    rTveMainReg.uEncMode.rEncMode.fgFulW = 1;
    rTveMainReg.uEncMode.rEncMode.u4OutMode0 = 1;
    rTveMainReg.uEncMode.rEncMode.u4SYDelay = 0x2;
    rTveMainReg.uEncMode.rEncMode.u4YDelay = 0x1;
    //0x34
    #if !defined(CC_MT5368) && !defined(CC_MT5396)  && !defined(CC_MT5389)    
    rTveMainReg.uDacControl.rDacControl.u4VCOD = 0x03;
    rTveMainReg.uDacControl.rDacControl.u4PlugDetect = 0x03;
    #endif

    TVE_WRITE32(TVE1_REG_ENCMOD, rTveMainReg.uEncMode.u4EncMode);
    TVE_WRITE32(TVE1_REG_DACTRL, rTveMainReg.uDacControl.u4DacCtrl);

    //TTX system B
    rTveMainReg.uTeltxt3.rTeltxt3.u4TTAddrNum = 0xC;//
    rTveMainReg.uTeltxt3.rTeltxt3.u4TTBufrWidth = 0xC;
    rTveMainReg.uTeltxt3.rTeltxt3.u4TTBgnLine = 0x5;//for line 318
    rTveMainReg.uTeltxt3.rTeltxt3.u4TTEndLine = 0x16;//line 22 and 335
    rTveMainReg.uTeltxt4.rTeltxt4.u4TTP1 = 0x107;//coarse tune
    rTveMainReg.uTeltxt4.rTeltxt4.u4TTP2 = 0x2D8;//fine tune
    rTveMainReg.uTeltxt5.rTeltxt5.u4TTSyn = 0x27;//framing_code
    rTveMainReg.uTeltxt5.rTeltxt5.u4TTCodeEnd = 0x170;//46 bytes per line
    TVE_WRITE32(TVE1_REG_TELTXT3, rTveMainReg.uTeltxt3.u4Teltxt3);
    TVE_WRITE32(TVE1_REG_TELTXT4, rTveMainReg.uTeltxt4.u4Teltxt4);
    TVE_WRITE32(TVE1_REG_TELTXT5, rTveMainReg.uTeltxt5.u4Teltxt5);
}

void Bypass_HalPIPClk(UCHAR ucOn)
{
    UCHAR ucFmt;

    TVE_HalGetFmt(TVE_1, &ucFmt); 
    #if defined(CC_MT5387) && !defined(CC_MT5363)
/*    if(ucFmt == TVE_FMT_480I)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_TVE_PAL);//PAL
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_TVE_PAL);//0x20025048[9];//PAL
    }
*/
    if(ucOn)
    {
//       vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 1, FLD_OCLK_TVE_SEL);
       vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 1, FLD_TVE2FS_SEL );
//        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1,PIP_OA_02_TVE_EN);//0x20025048[8];//PIP scaler 27MHz
    }
    else
    {
//       vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0, FLD_OCLK_TVE_SEL);
       vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0, FLD_TVE2FS_SEL );
//        vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_TVE_EN);//PIP scaler 27MHz
    }
    #else //CC_MT5363 or CC_TVE6595
    if(ucFmt == TVE_FMT_480I)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_TVE_PAL);//PAL
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_TVE_PAL);//0x20025048[9];//PAL
    }

    if(ucOn)
    {
        #ifdef CC_MT5396
        vIO32WriteFldAlign(CKGEN_OCLK_TEST, 4, FLD_OCLK_TVE_SEL);
        #else
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 1, FLD_OCLK_TVE_SEL);
        #endif
        
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 1, FLD_TVE2FS_SEL );
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1,PIP_OA_02_TVE_EN);//0x20025048[8];//PIP scaler 27MHz
        #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1 , PIP_OA_02_TVE_UP_EN);
        #else
        vRegWriteFldAlign(SCPIP_PIP_OA_01, 1 , PIP_OA_01_TVE_UP_EN);
        #endif
    }
    else
    {
        #if defined(CC_MT5396)
        //when TVE off, set OCLK_TVE same as POCLK
        vIO32WriteFldAlign(CKGEN_OCLK_TEST, IO32ReadFldAlign(CKGEN_DISP_CKCFG, FLD_POCLK_SEL), FLD_OCLK_TVE_SEL);
        #else
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0, FLD_OCLK_TVE_SEL);
        #endif
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0, FLD_TVE2FS_SEL );
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_TVE_EN);//PIP scaler 27MHz
        #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 0 , PIP_OA_02_TVE_UP_EN);
        #else        
        vRegWriteFldAlign(SCPIP_PIP_OA_01, 0 , PIP_OA_01_TVE_UP_EN);
        #endif
    }
    #endif

}

/*--------------------------------------------
// SCART bypass test pattern
//-------------------------------------------- */
void Bypass_HalSrcTestPattern(void)
{
    TVE_HalSrcSelect(1);
    TVE_HalColorBar(TVE_1, 1);
    TVE_HalSetSyncTime1(TVE_1, 0x006, 0x600);
    Bypass_HalPIPClk(0);
}


/*--------------------------------------------
// YPbPr/HDMI/DTV HD or non-standard SD to TVE
//-------------------------------------------- */
void Bypass_HalSrcPIP(UCHAR DAC, UCHAR bSrc)
{
    UCHAR ucFmt;
    //UINT16 adj_vsync, adj_hsync;
    _ucTveInputSrc = bSrc;//used by TVE_HalSetFmt

    //printk("Bypass_HalSrcPIP=%x\n",bSrc);
    TVE_HalSetMixerDacSel(DAC, 0);//TVE, not bypass
    
    TVE_HalSrcSelect(0);//PIP
    TVE_HalColorBar(TVE_1, 0);
    TVE_HalGetFmt(TVE_1, &ucFmt); 
    
    Bypass_HalTVEMSel(0xff);//set default TVEM_SEL
    Bypass_HalPIPClk(1);
    Bypass_HalTVESyncTime(bSrc, ucFmt);
    #if defined(CC_MT5387) && !defined(CC_MT5363)
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x4, FLD_VDAC1_SEL); 
    #else //MT5363 or TVE6596
    if(DAC == DAC_1)
    {
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x4, FLD_VDAC1_SEL); 
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x4, FLD_VDAC2_SEL); 
    }
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    if(DAC == DAC_1)
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x0, FLD_VDAC1_SIF_SEL); 
    else
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x0, FLD_VDAC2_SIF_SEL); 
    #endif
    #endif
}

/*--------------------------------------------
// YPbPr/HDMI/DTV SD to TVE
//-------------------------------------------- */
void Bypass_HalSrcVdoin(UCHAR DAC, UCHAR bSrc)
{
    _ucTveInputSrc = bSrc;//used by TVE_HalSetFmt
    TVE_HalSetMixerDacSel(DAC, 0);//TVE, not bypass

    TVE_HalSrcSelect(1);//Vdoin
    TVE_HalColorBar(TVE_1, 0);
    //PAL 1728-2; NTSC 1716-2
    TVE_HalSetSyncTime0(TVE_1, 1, 0x480, 0x6Be);
    Bypass_HalTVEMSel(bSrc);
    Bypass_HalPIPClk(0);
    switch(bSrc)
    {
        case SV_VD_YPBPR:
            TVE_HalSetSyncTime1(TVE_1, 0x006, 0x5D0);
            break;
        case SV_VD_DVI:
            TVE_HalSetSyncTime1(TVE_1, 0x206, 0x5C0);
            break;
        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:
            TVE_HalSetSyncTime1(TVE_1, 0x006, 0x5C0);
            break;
    }
    #if defined(CC_MT5387) && !defined(CC_MT5363)
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x4, FLD_VDAC1_SEL); 
    #else //MT5363 or TVE6596
    if(DAC == DAC_1)
    {
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x4, FLD_VDAC1_SEL); 
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x4, FLD_VDAC2_SEL); 
    }
    #endif
}

/*--------------------------------------------
// ATV/AV/S-Video bypass to DAC
//-------------------------------------------- */
void Bypass_HalSrcBypass(UCHAR DAC, UCHAR bSrc)
{
    //Bypass_HalPIPClk(0);
    if(DAC == DAC_1)
    {
        TVE_HalSetMixerSrc(TVE_Mixer1, bSrc);
        TVE_HalSetMixerDacSel(DAC_1, 1);//bypass, not TVE
        if(bSrc == TVE_VDOIN_MIXER) //BYPASS_CVBS
        {
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x5, FLD_VDAC1_SEL); 
            #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x0, FLD_VDAC1_SIF_SEL); 
            #endif
        }
        else if(bSrc == TVE_DEMOD_MIXER)//BYPASS_DEMOD
        {
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x6, FLD_VDAC1_SEL); 
            #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x0, FLD_VDAC1_SIF_SEL); 
            #endif
        }
        #if defined(CC_MT5363) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
        else if(bSrc == TVE_SIF_MIXER)//BYPASS_SIF
        {
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x6, FLD_VDAC1_SEL); 
            #ifdef CC_MT5363
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x3, FLD_VDAC1_DTDCK_SEL); 
            #elif defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x1, FLD_VDAC1_SIF_SEL); 
            #endif
        }
        #endif
    }
    else
    {
        TVE_HalSetMixerSrc(TVE_Mixer2, bSrc);
        TVE_HalSetMixerDacSel(DAC_2, 1);//bypass, not TVE
#if defined(CC_MT5363) || defined(CC_TVE6595)
        if(bSrc == TVE_VDOIN_MIXER) //BYPASS_VDOIN
        {
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x5, FLD_VDAC2_SEL); 
            #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x0, FLD_VDAC2_SIF_SEL); 
            #endif
        }
        else if(bSrc == TVE_DEMOD_MIXER)//BYPASS_DEMOD
        {
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x6, FLD_VDAC2_SEL); 
            #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x0, FLD_VDAC2_SIF_SEL); 
            #endif
        }
        #if defined(CC_MT5363) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
        else if(bSrc == TVE_SIF_MIXER)//BYPASS_SIF
        {
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x6, FLD_VDAC2_SEL); 
            #ifdef CC_MT5363
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x3, FLD_VDAC2_DTDCK_SEL); 
            #elif defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 0x1, FLD_VDAC2_SIF_SEL); 
            #endif
        }
        #endif
#endif
    }
}

/*--------------------------------------------
// Set TVE output mode and related setting
//-------------------------------------------- */
UINT32 TVE_HalSetTVMode(UCHAR ucTveId,UCHAR eTveMode)
{
    INT32 i4RetVal = TVE_SET_OK;
    if (ucTveId == TVE_1)
    	{
        switch (eTveMode)
       	{
       	case 0:
			 rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType=0X0;
			 rTveMainReg.uBurst1.rBurst1.fgBf443=0x0;
			 	break;
		case 1:
			 rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType=0X1;
			 rTveMainReg.uBurst1.rBurst1.fgBf443=0x0;
			 	break;
             case 2:
			 rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType=0X2;
			 rTveMainReg.uBurst1.rBurst1.fgBf443=0x0;
			 	break;				 	
	       case 3:
		   	 rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType=0X3;
			 rTveMainReg.uBurst1.rBurst1.fgBf443=0x0;
			 	break;
	       case 4:
		   	 rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType=0X1;
			 rTveMainReg.uBurst1.rBurst1.fgBf443=0x1;
			 	break;
	       case 5:
		   	 rTveMainReg.uMuxCtrl.rMuxCtrl.u4TvType=0X0;
			 rTveMainReg.uBurst1.rBurst1.fgBf443=0x1;
			 	break;
	       default :
		   	       break;
		}
     TVE_WRITE32(TVE1_REG_MUX_CTRL, rTveMainReg.uMuxCtrl.u4MuxCtrl);
     TVE_WRITE32(TVE1_REG_BURST, rTveMainReg.uBurst1.u4Burst1);

	}
	else /* TVE_2 (Aux) MOD = Composite/Component, only 525i & 625i */
    {
        LOG(7, "Only 1 TVE\n");
    }
   return (UINT32)i4RetVal;
}

/*--------------------------------------------
// Check YPbPr/HDMI/DTV is HD or SD
//-------------------------------------------- */
UINT32 Bypass_HalTVEMode(UCHAR bSrc)
{
    UINT16 wRet1,wRet2;

    wRet1 = wRet2 = 0;

    switch(bSrc)
    {
        case SV_VD_YPBPR:
            wRet1 = wHdtvInputWidth();
            wRet2 = wHdtvInputHeight();
            break;

        //case SV_VD_VGA:
            //wRet1 = wVgaInputWidth();
            //wRet2 = wVgaInputHeight();
            //break;

        #if SUPPORT_DVI
        case SV_VD_DVI:
            wRet1 = wDviInputWidth();
            wRet2 = wDviInputHeight();
            break;
        #endif
        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:
            wRet1 = u2MpegHdInputValidDataWidth();
            wRet2 = u2MpegHdInputHeight();
            break;
    }
    //if((wRet1== 720) && ((wRet2 == 480) || (wRet2 == 576)))
    if(((UINT32)wRet1 * (UINT32)wRet2) < 1280*720)
    {
        return 1;//SD
    }
    else
    {
        return 0;//HD
    }

}

#endif /* _TVE_HAL_C_ */


