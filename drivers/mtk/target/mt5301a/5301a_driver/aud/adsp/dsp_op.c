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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dsp_op.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_op.c
 *  Brief of file dsp_op.c.
 */
 
#define _DSP_OP_C

// KERNEL
#include "x_util.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "dsp_general.h"
#include "adsp_task.h"
#include "dsp_intf.h"
#include "dsp_data.h"
#include "dsp_reg.h"
#include "dsp_shm.h"
#include "dsp_uop.h"
#include "dsp_table.h"
#include "dsp_rg_ctl.h"
#include "dsp_func.h"
#include "d2rc_shm.h"

#include "aud_if.h"
#include "aud_debug.h"
#include "aud_drvif.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_hal_926.h"

LINT_EXT_HEADER_END

#ifdef DATA_DISC_WMA_SUPPORT
//WMA Table
#define WMA_TABLE_NORMAL                         0x6CD6
#define WMA_TABLE_CMPT                           0x476A
#endif

#define fgBond2Ch()           ((_u4Bond & BOND_2CH) > 0)
#define fgBondInvalid()       ((_u4Bond & BOND_INVALID) > 0)

static AV_SYNC_MODE_T _aeAvSyncMode[2] = {AV_SYNC_FREE_RUN, AV_SYNC_FREE_RUN};

static void vResetDecInfo (BOOL fgFirstDecoder);
BOOL fgIsOverADCFreq (UINT32 u4Freq, BOOL fgFirstDecoder);
void vTrnsltSpkCfg (UINT16 u2SpkCfg, SPEAKER_SETTING_T * ptSpkCfg);

UINT32 dwDspCalOfst (UINT32 dwOfst);

#ifdef ADSP_CHECK_RAM_CODE
void vCheckDspRamCode(UINT32 *pu4DspRamCodeStatus, UINT32 *pu4DspTableStatus);
UINT32 u4CheckDspDramTable(UINT32 u4Type);
#endif
BOOL fgDspDataComp(UINT8 *puAdr1, UINT8 *puAdr2, UINT32 u4Size);
void vDspSetSyncMode(UCHAR ucDecId, AV_SYNC_MODE_T eMode);
AV_SYNC_MODE_T eDspGetSyncMode(UCHAR ucDecId);
BOOL fgCheckDspCmd(UINT32 u4DspCmd);
void vDspSetFifoReadPtr(UCHAR ucDecId, UINT32 u4ReadPtr);
BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address);
void vDspDec2Des (void);

#ifdef CC_MT5391_AUD_3_DECODER
    extern void vDspResetDec3 (void);
#endif

    
#ifdef DSP_RAM_CODE_CHKSUM
/****************************************************************************
 FUNCTION: fgBuildCRCTable
 DESCRIPTION: build CRC table to speedup computation
*****************************************************************************/
static UINT32 CRCTable[256];      // Table constructed for fast lookup.

#define CRC32_POLYNOMIAL		 0xEDB88320L

// Initialize the CRC calculation table
//
BOOL fgBuildCRCTable (void)
{
    int i, j;
    UINT32 dwCRC;

    for (i = 0; i <= 255; i++)
    {
        dwCRC = i;
        for (j = 8; j > 0; j--)
        {
            if (dwCRC & 1)
                dwCRC = (dwCRC >> 1) ^ CRC32_POLYNOMIAL;
            else
                dwCRC >>= 1;
        }
        CRCTable[i] = dwCRC;
    }
    return 0;
}

/****************************************************************************
 FUNCTION: dwCRC32
 DESCRIPTION: calculate 32 bit CRC checksum
*****************************************************************************/
UINT32 dwCRC32 (UINT32 * pdwBuffer, UINT32 dwCount, UINT32 dwCRC)
{
    UINT32 dwTmp, dwTmp1, dwTmp2, dwInput;
    int i, index;
    static BOOL fgFirstTime = TRUE;

    if (fgFirstTime)
    {
        if (fgBuildCRCTable ())
            return -1;
        fgFirstTime = FALSE;
    }

    for (index = dwCount - 1; index >= 0; index--)
    {
        ReadIntfDram ((UINT32) ((UINT32) (pdwBuffer) + index * 4), dwInput);
//  ^^^^^^^^^used to be compatible with ADSP simulation program
        for (i = 0; i < 4; i++)
        {
            dwTmp = ((dwInput << (i * 8)) & 0xFF000000L) >> 24;
            dwTmp1 = (dwCRC >> 8) & 0x00FFFFFFL;
            dwTmp2 = CRCTable[(dwCRC ^ dwTmp) & 0xFF];
            dwCRC = dwTmp1 ^ dwTmp2;
        }
    }
    return dwCRC;
}
#endif

/***************************************************************************
     Function : vLoadDspDRAMTable(UINT32 u4Type)
  Description : Download Decoders' Table to DRAM
                Set default value at here too.
    Parameter : u4Type
       Author : sammy,PC
    Return    :
***************************************************************************/
#ifdef DSP_RAM_CODE_CHKSUM
#define DSP_RAM_CODE_RETRY_CNT   100
UINT32 _dwDspTblChkSum;
UINT32 _dwDspTblRetryCnt;
UINT32 _dwDspTblChkSum1;
#endif

void vLoadDspDRAMTable (UINT32 u4Type)
{
    UINT32 dwOfst;
    UINT32 dwSz;
    //UINT32 dwDspData;
    //UINT16 u2DspData;
    BOOL fgGetBitsNotHold = FALSE;

#ifdef DSP_RAM_CODE_CHKSUM
    UINT32 *pdwDspTbl;
#endif
    UINT32 dwTabAddr = DRAM_DECODING_TABLE_ADDR * 4;  // Byte address
    UINT32 dwDspBuf;
    UINT32 dwDspCmptBuf;
    UINT32 dwFlashOfst;

    // copy Table to DRAM from flash
    dwDspBuf = dwGetDSPNSADR (u4Type);
    dwDspCmptBuf = dwGetDSPCSADR (u4Type);

    dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr
    switch (u4Type)
    {
    #ifdef ADSP_JPEG_DEC2       ///[Joel]
    case AUD_JPEG_DEC2:
        // JPEG Table in DRAM 08400~0844Ah
        // JPEG_DRAM_ZIGZAG_ADR     08400~08420h
        // JPEG_DRAM_AANSCAL_ADR    08420~08440h
        // JPEG_DRAM_FIX_MxM_ADR    08440~08448h
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_JPEG_TBL_SA<< 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_JPEG_TBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    #endif

    case AUD_AAC_DEC1:
    case AUD_AAC_DEC2:
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    case AUD_AAC_DEC3:
#endif
        // move AAC Table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AAC_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AAC_TABLE_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);

		//cmpt table
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AAC_CMPT_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AAC_CMPT_TABLE_SZ << 2));
        vFlash2DramDma (dwDspBuf + (AAC_CMPT_TABLE_ADR*4), dwOfst, dwSz * 4);
		
        break;
    case AUD_AC3_DEC1:
    case AUD_AC3_DEC2:
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    case AUD_AC3_DEC3:
#endif
        // move AC3 Table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AC3_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AC3_TABLE_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_MPEG_DEC1:
    case AUD_MPEG_DEC2:
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    case AUD_MPEG_DEC3:
#endif
        // move MP2 Table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_MP2_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_MP2_TABLE_SZ << 2));

        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_MP3_DEC1:
        // move MP3 Table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_MP3_TABLE_SA << 2));

        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_MP3_TABLE_SZ << 2));

        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_VORBIS_DEC1:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_VORBIS_C_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_VORBIS_C_TABLE_SZ << 2));
        vFlash2DramDma ((dwDspBuf + (0x8000 * 4)), dwOfst, (dwSz * 4));
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_VORBIS_N_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_VORBIS_N_TABLE_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        fgGetBitsNotHold = TRUE;
        break;
#ifdef DATA_DISC_WMA_SUPPORT
    case AUD_WMA_DEC1:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_WMA_N_TABLE_SA << 2));

        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_WMA_N_TABLE_SZ << 2));

        vFlash2DramDma (dwDspBuf + WMA_TABLE_NORMAL * 4, dwOfst, dwSz * 4);
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_WMA_C_TABLE_SA << 2));

        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_WMA_C_TABLE_SZ << 2));

        vFlash2DramDma (dwDspBuf + WMA_TABLE_CMPT * 4, dwOfst, dwSz * 4);
        break;
#endif
#ifdef DSP_SUPPORT_NPTV
    case AUD_MTS_DEC1:
    //case AUD_DSP_MINER_DEC1:
    case AUD_MTS_DEC2:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_NTSCTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_NTSCTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        //the following is needed for MT8223 ECO IC.
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_ATV_COSTBL_SA << 2));
        dwOfst = dwDspCalOfst(dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_ATV_COSTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + (0x800*4), dwOfst, dwSz * 4);
        break;
#if 1   
    case AUD_FMFM_DEC1:
    case AUD_FMFM_DEC2:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_FMFMTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_FMFMTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_A2DEC_DEC1:
    case AUD_A2DEC_DEC2:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_A2TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_A2TBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
         //the following is needed for MT8223 ECO IC.
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_ATV_COSTBL_SA << 2));
        dwOfst = dwDspCalOfst(dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_ATV_COSTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + (0x800*4), dwOfst, dwSz * 4);
        break;
    case AUD_DETECTOR_DEC1:
    case AUD_DETECTOR_DEC2:
    case AUD_FMRDODET_DEC1: //detector shares table with FM radio detector 
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DETECTORTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_DETECTORTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_PAL_DEC1:
    case AUD_PAL_DEC2:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PALTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_PALTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        //the following is needed for MT8223 ECO IC.
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_ATV_COSTBL_SA << 2));
        dwOfst = dwDspCalOfst(dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_ATV_COSTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + (0x800*4), dwOfst, dwSz * 4);	
        break;
    case AUD_FMRDO_DEC1:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_FMRDOTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_FMRDOTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_MINER_DEC1: 
    case AUD_MINER_DEC2: 
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_MINERTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz   = u4GetFlashData (dwFlashOfst + (DSPF_MINERTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;    
    #endif
#endif

    case AUD_LPCM_DEC1:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_LPCM_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_LPCM_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
   case AUD_DDCO:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TBL_SZ << 2));
	  vFlash2DramDma (dwDspBuf + DDCO_TABLE_ADR * 4, dwOfst, dwSz * 4);	
        break;
    case AUD_SBC_DEC1:
        // move MP3 Table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_SBC_TABLE_SA << 2));

        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_SBC_TABLE_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;        
#ifdef CC_MT5360B
        //for MT5360B, need to load down-mix table when load common code
    case AUD_COMMRAM:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DOWNMIX_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DOWNMIX_TBL_SZ << 2));
	  vFlash2DramDma (dwDspBuf + DRAM_DOWNMIX_TABLE_ADR * 4, dwOfst, dwSz * 4);	    
#endif
        
        
    default:
        return;
    }

    // Set Get bits hold register accroding to different decoding
    vSetGetBitsNotHold (fgGetBitsNotHold);

#ifdef DSP_RAM_CODE_CHKSUM
    switch (u4Type)
    {
    case AUD_AAC_DEC1:
    case AUD_AAC_DEC2:
        _dwDspTblRetryCnt = 0;
        pdwDspTbl = (UINT32 *) (dwDspBuf + dwTabAddr);

        do
        {
            vFlash2DramDma (pdwDspTbl, dwOfst, dwSz * 4);
            _dwDspTblChkSum =
                dwCRC32 ((UINT32 *) CACHE (pdwDspTbl), dwSz - 1, -1L);
            _dwDspTblChkSum1 = u4GetFlashData (dwOfst + (dwSz - 1) * 4);

            _dwDspTblRetryCnt++;
        }
        while (_dwDspTblChkSum1 != _dwDspTblChkSum &&
               _dwDspTblRetryCnt < DSP_RAM_CODE_RETRY_CNT);
    }
#endif /* no check checksum */
}

#ifdef CC_MT5391_AUD_3_DECODER
/***************************************************************************
     Function : vResetDec3Info
  Description : Reset decoding information, decoding dynamic parameter,
                decoding fixed parameters
                It is invoked when decoding is changed.
    Parameter :
       Author : sammy
    Return    :
***************************************************************************/
void vResetDec3Info (void)
{
    UINT32 i;   
    for (i = 0; i < DSP_D2RC_INFO_AREA_SZ; i++)
    {
        WriteDspCommDram (ADDR_D2RC_DECODING_INFO_BASE_DEC3 + i, 0);
    }
    for (i = 0; i < DSP_RC2D_SETTING_AREA_SZ; i++)
    {
        WriteDspCommDram (ADDR_RC2D_DECODER_INFO_BASE_DEC3 + i, 0);
    }
    // Initial ctrl page 5 for second decoder
    for (i = 0; i < DSP_RC2D_CTRL8_AREA_SZ; i++)
    {
        WriteDspCommDram (ADDR_RC2D_CTRL8_BASE + i, 0);
    }
}
#endif

/***************************************************************************
     Function : vDSPClearDecType
  Description : Clear all decoder types to default value
    Parameter :
       Author : 
    Return    :
***************************************************************************/
void vDSPClearDecType(void) //  -- DSP Force --
{
    _dwDspRamCodeType = 0xFF;
    _dwDspRamCodeTypeDec2 = 0xFF;
#ifdef CC_MT5391_AUD_3_DECODER    
    _dwDspRamCodeTypeDec3 = 0xFF;
#endif
}

/***************************************************************************
     Function : vDspLoadRamCode(UINT32 u4Type)
  Description : Initialize Load DSP Ram Code
    Parameter : u4Type: AC3_CODE/DTS_CODE/MP3_CODE/CDDA_CODE/MP2_CODE
    Return    : None

**  Caution   : Some setting is NOT involved in this function, like Bistream
                buffer initialization.

***************************************************************************/
#ifdef DSP_RAM_CODE_CHKSUM
#define DSP_RAM_CODE_RETRY_CNT   100
UINT32 _dwDspChkSum;
UINT32 _dwDspLoadRetryCnt;
UINT32 _dwDspChkSum1;
#endif
void vDspLoadRamCode (UINT32 u4Type)
{
    // Down Load From Flash
    UINT32 dwDECICAddr;
    UINT16 u2RamCodeType;    // Dyanmic load by Shawn. ID is over 7, need to extend to 16 bits
    UINT32 dwOfst, dwSz;
    BOOL fgLoadTable, fgLoadCode;
    UINT32 dwCodIndx;
//    BOOL fgWaitDsp;
    UINT32 dwFlashOfst;
    UINT32 dwCodeDestAdr,dwCodeSrcAdr;

    UINT32 pAUD2COD1[] = {
        DSPF_AC3_DEC1_SA,       // AUD_AC3_DEC1
        DSPF_MP2_DEC1_SA,       // AUD_MPEG_DEC1
        DSPF_AAC_DEC1_SA,       // AUD_AAC_DEC1
        DSPF_PCM_DEC1_SA,       // AUD_PCM_DEC1
        DSPF_MP3_DEC1_SA,       // AUD_MP3_DEC1
        DSPF_WMA_DEC1_SA,       // AUD_WMA_DEC1
        DSPF_VORBIS_DEC1_SA,    // AUD_VORBIS_DEC1
        DSPF_PNOISE_SA,         // AUD_PINK_DEC1
#ifdef DSP_SUPPORT_NPTV
        DSPF_NTSC_DEC1_SA,      // AUD_NTSC_DEC1
        DSPF_PAL_DEC1_SA,       // AUD_PAL_DEC1
        DSPF_MINER_DEC1_SA,     // AUD_MINER_DEC1
        DSPF_A2_DEC1_SA,        // AUD_A2DEC_DEC1
        DSPF_DETECTOR_DEC1_SA,  // AUD_DETECTOR_DEC1
        DSPF_FMFM_DEC1_SA,      // AUD_FMFM_DEC1
        //DSPF_NICAM_DEC1_SA,     // AUD_NICAM_DEC1
        //DSPF_TTXACC_DEC1_SA,    // AUD_TTXACC_DEC1
        DSPF_LPCM_DEC1_SA,      // AUD_LPCM_DEC1  
        DSPF_FMRDODEC1_SA,      //AUD_FMRDO_DEC1
        DSPF_FMRDODETDEC1_SA,  // AUD_FMRDODET_DEC1
        DSPF_SBCDEC1_SA             // AUD_SBC_DEC1
        
#endif
    };
    UINT32 pAUD2COD2[] = {
        DSPF_AC3_DEC2_SA,       // AUD_AC3_DEC2
        DSPF_MP2_DEC2_SA,       // AUD_MPEG_DEC2
        DSPF_AAC_DEC2_SA,       // AUD_AAC_DEC2
        DSPF_PCM_DEC2_SA,       // AUD_PCM_DEC2
#ifdef DSP_SUPPORT_NPTV
        DSPF_NTSC_DEC2_SA,      // AUD_NTSC_DEC2
        DSPF_PAL_DEC2_SA,
        DSPF_MINER_DEC2_SA,
        DSPF_A2_DEC2_SA,
        DSPF_DETECTOR_DEC2_SA,
        DSPF_FMFM_DEC2_SA,
        //DSPF_NICAM_DEC2_SA,
        //DSPF_TTXACC_DEC2_SA,
#endif
#ifdef ADSP_JPEG_DEC2       ///[Joel]
        DSPF_JPEG_DEC2_SA,
#endif   

    };


#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    UINT32 pAUD2COD3[] = {
        DSPF_MP2_DEC3_SA,       // AUD_MPEG_DEC3
        DSPF_PCM_DEC3_SA,       // AUD_PCM_DEC3 
        DSPF_AAC_DEC3_SA,       // AUD_PCM_DEC3 
        DSPF_AC3_DEC3_SA,       // AUD_PCM_DEC3 
    };
#endif 

#ifdef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT
#ifdef CC_MT5391_AUD_3_DECODER
    // dynamic load by Shawn
    // decoder 3 vector flash info table
    UINT32 pAUD2COD3VCT[] = {
        DSPF_VCT_MP2DEC3_SA,
        0,
        DSPF_VCT_AACDEC3_SA,
        DSPF_VCT_AC3DEC3_SA,        
        
    };     
    
#endif 

#ifdef CC_NEW_PROGRAM_DEF 
    // MT5360B/MT5393
    // decoder 2 extension code  flash info table
    UINT32 pAUD2COD2EXT[] = {
        DSPF_AC3_DEC2EXT_SA,       // AUD_AC3_DEC2
        0,       // AUD_MPEG_DEC2
        DSPF_AAC_DEC2EXT_SA,       // AUD_AAC_DEC2
        0,       // AUD_PCM_DEC2
#ifdef DSP_SUPPORT_NPTV
        0,      // AUD_NTSC_DEC2
        0,
        0,
        0,
        0,
        0
        //0,
        //0,
#endif
    };     
#else
    // MT5360A/MT5391
    // decoder 2 extension 0 code  flash info table
    UINT32 pAUD2COD2EXT0[] = {
        DSPF_AC3_DEC2EXT0_SA,       // AUD_AC3_DEC2
        0,       // AUD_MPEG_DEC2
        DSPF_AAC_DEC2EXT0_SA,       // AUD_AAC_DEC2
        0,       // AUD_PCM_DEC2
#ifdef DSP_SUPPORT_NPTV
        0,      // AUD_NTSC_DEC2
        0,
        0,
        0,
        0,
        0
        //0,
        //0,
#endif
    };
    // decoder 2 extension 1 code  flash info table
    UINT32 pAUD2COD2EXT1[] = {
        DSPF_AC3_DEC2EXT1_SA,       // AUD_AC3_DEC2
        0,       // AUD_MPEG_DEC2
        DSPF_AAC_DEC2EXT1_SA,       // AUD_AAC_DEC2
        0,       // AUD_PCM_DEC2
#ifdef DSP_SUPPORT_NPTV
        0,      // AUD_NTSC_DEC2
        0,
        0,
        0,
        0,
        0
        //0,
        //0,
#endif
    };  
#endif    
#endif
    
    UINT8 pDspStrTyp[] = {
        AC3_STREAM,             // AUD_AC3_DEC1
        MPEG12_STREAM,          // AUD_MPEG_DEC1
        AAC_STREAM,             // AUD_AAC_DEC1
        PCM_STREAM,             // AUD_PCM_DEC1
        MPEG3_STREAM,           // AUD_MP3_DEC1
        WMA_STREAM,             // AUD_WMA_DEC1
        VORBIS_STREAM,          // AUD_VORBIS_DEC1
        PINK_NOISE_STREAM,      // AUD_PINK_DEC1
#ifdef DSP_SUPPORT_NPTV        
        NTSC_STREAM,         
        PAL_STREAM,        
        MINER_STREAM,
        A2_STREAM,
        DETECTOR_STREAM,
        FMFM_STREAM,
        LPCM_STREAM,
        //NICAM_STREAM,
        //TTXACC_STREAM,
        FMRDO_STREAM,
        FMRDODET_STREAM,
#endif
        SBC_STREAM              // AUD_SBC_DEC1
    };
    UINT8 pDspStrTyp2[] = {
        AC3_STREAM,             // AUD_AC3_DEC2
        MPEG12_STREAM,          // AUD_MPEG_DEC2
        AAC_STREAM,             // AUD_AAC_DEC2
        PCM_STREAM,             // AUD_PCM_DEC2
#ifdef DSP_SUPPORT_NPTV
        NTSC_STREAM,            // AUD_NTSC_DEC2
        PAL_STREAM,        
        MINER_STREAM,
        A2_STREAM,
        DETECTOR_STREAM,
        FMFM_STREAM,
        //NICAM_STREAM,
        //TTXACC_STREAM,
#endif
#ifdef ADSP_JPEG_DEC2       ///[Joel]
        JPEG_STREAM,            //0x10
#endif
    };

#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    UINT8 pDspStrTyp3[] = {
        MPEG12_STREAM,          // AUD_MPEG_DEC3
        PCM_STREAM,             // AUD_PCM_DEC3
        AAC_STREAM,             // AUD_PCM_DEC3
        AC3_STREAM,             // AUD_PCM_DEC3
        
    };        
#endif
    

    //fgWaitDsp = TRUE;

    fgLoadCode  = FALSE;
    fgLoadTable = FALSE;
    dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr
    switch (u4Type & 0xC0)
    {
    case TYPE_AUD_DEC1:
        _bCode3D      = CODE3D_NONE;
        _bCode3DUpmix = CODE3D_NONE;

        if (_fgDspPlay)
        {
            vDspResetDec1 ();
        }
        if (u4Type > DECODER1_MAX_INDEX)
        {
            vShowError (ERR_ADSP_LOADCODE_OUTBOUND);
        }
        fgLoadTable = _fgDecTblFromFlash;
        /*lint -e{661}*/
        dwCodIndx   = pAUD2COD1[u4Type];
        dwOfst      = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
        dwSz        = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));
        vResetDecInfo (TRUE);
        u2RamCodeType = TYPE_DEC1_ICACHE;
        if (_dwDspRamCodeType == u4Type)
        {
            vLoadDspDRAMTable (u4Type);
            vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG, 0x1);
            return;
        }
        _dwDspRamCodeType = u4Type;
        /*lint -e{661}*/
        _bDspStrTyp       = pDspStrTyp[u4Type];
        fgLoadCode        = TRUE;
        break;
    case TYPE_AUD_DEC2:
        _bCode3DDec2      = CODE3D_NONE;
        _bCode3DUpmixDec2 = CODE3D_NONE;
        if (_fgDspPlayDec2)
        {
            vDspResetDec2 ();
        }
        if ((u4Type - TYPE_AUD_DEC2) > DECODER2_MAX_INDEX)
        {
            vShowError (ERR_ADSP_LOADCODE_OUTBOUND);
        }
      
#ifdef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT
#ifdef CC_NEW_PROGRAM_DEF 
        // MT5360B/MT5393
        // dynamic load by Shawn
        // load decoder 2 extension code        
        dwCodIndx   = pAUD2COD2EXT[u4Type - TYPE_AUD_DEC2];
        if (dwCodIndx)
        {
            // decoder 2 extension code exists
            dwOfst = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
            dwSz = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2)); 
            if (dwSz)
            {
                dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC2_EXT0_ICACHE);  
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }
        }
#else
        // MT5360A/MT5391
        // dynamic load by Shawn
        // load decoder 2 extension 0 code
        dwCodIndx   = pAUD2COD2EXT0[u4Type - TYPE_AUD_DEC2];
        if (dwCodIndx)
        {
            // decoder 2 extension 0 code exists
            dwOfst = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
            dwSz = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2)); 
            if (dwSz)
            {
                dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC2_EXT0_ICACHE);  
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }
        }
        
        // load decoder 2 extension 1 code
        dwCodIndx   = pAUD2COD2EXT1[u4Type - TYPE_AUD_DEC2];
        if (dwCodIndx)
        {
            // decoder 2 extension 1 code exists
            dwOfst = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
            dwSz = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));
            if (dwSz)
            {
                dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC2_EXT1_ICACHE);   
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }
        }
#endif

        //MT5360A, for AAC dec2, use dec3 section code
        if (u4Type == AUD_AAC_DEC2) 
        {
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AAC_DEC2X_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AAC_DEC2X_SZ << 2)); 
            if (dwSz)
            {
			    //dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC1_ICACHE) + 8192/4;			    
                dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC3_ICACHE);
                #ifdef CC_MT5391_AUD_3_DECODER
                _dwDspRamCodeTypeDec3 = u4Type; //HE-AAC use decoder3 space, need to clean the flag or decoder3 won't be loaded again                
                #endif
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }
        }

    

#endif
        
        fgLoadTable = _fgDec2TblFromFlash;
        
         /*lint -e661*/ /*lint -e662*/
        dwCodIndx   = pAUD2COD2[u4Type - TYPE_AUD_DEC2];
        dwOfst      = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
        dwSz        = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));

#ifdef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT
#ifdef CC_NEW_PROGRAM_DEF 
#define DECODER2_PROGRAM_SIZE (DSP_DEC2_RAM_SZ / 4)
        // dynamic load by Shawn
        // load decoder 2 ain part
        // Because decoder 2 program is divied into two parts, ain part (0xa000) must be loaded.
        if (dwSz > DECODER2_PROGRAM_SIZE)
        {
            // decoder 2 ain part code exists
            vFlash2DramDma ((dwGetDspIcacheAdr (TYPE_DEC2_EXT1_ICACHE) * 4), 
                	                      u4GetDspImgAddr () + ((dwOfst + DECODER2_PROGRAM_SIZE) * 4), 
                	                      (dwSz - DECODER2_PROGRAM_SIZE - 1) * 4);
            dwSz = DECODER2_PROGRAM_SIZE + 1;
        }
#endif
#endif
        
        vResetDecInfo (FALSE);
        u2RamCodeType = TYPE_DEC2_ICACHE;
        if (_dwDspRamCodeTypeDec2 == u4Type)
        {
            vLoadDspDRAMTable (u4Type);
            vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC2, 0x1);
            return;
        }
        _dwDspRamCodeTypeDec2 = u4Type;
         /*lint -e661*/ /*lint -e662*/
        _bDspStrTypDec2       = pDspStrTyp2[u4Type - TYPE_AUD_DEC2];
        fgLoadCode            = TRUE;
        break;
#ifdef CC_MT5391_AUD_3_DECODER
// gallen, use this as dec3
    case TYPE_AUD_MIX:
        _bCode3DDec3      = CODE3D_NONE;
        _bCode3DUpmixDec3 = CODE3D_NONE;
        if (_fgDspPlayDec3)
        {
            vDspResetDec3 ();
        }
        if ((u4Type - TYPE_AUD_MIX) > DECODER3_MAX_INDEX)
        {
            vShowError (ERR_ADSP_LOADCODE_OUTBOUND);
        }

#ifdef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT
        // dynamic load by Shawn
        // load decoder 3 vector
        dwCodIndx   = pAUD2COD3VCT[u4Type - TYPE_AUD_MIX];
        if (dwCodIndx)
        {
            // decoder 3 vector exists
            dwOfst = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
            dwSz = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2)); 
            if (dwSz)
            {
                dwDECICAddr = dwGetDspIcacheAdr (TYPE_VCT_DEC3_ICACHE);        
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }
        }
#endif
        
        fgLoadTable = _fgDec2TblFromFlash;
         /*lint -e661*/ /*lint -e662*/
        dwCodIndx   = pAUD2COD3[u4Type - TYPE_AUD_MIX];
        dwOfst      = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
        dwSz        = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));
        vResetDec3Info ();
#ifdef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT        
        u2RamCodeType = TYPE_DEC3_ICACHE;
#else
        u2RamCodeType = TYPE_ROM_ICACHE;
#endif
        vLoadDspDRAMTable (u4Type);
        vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC3, 0x1);
        
        _dwDspRamCodeTypeDec3 = u4Type;
         /*lint -e661*/ /*lint -e662*/
        _bDspStrTypDec3       = pDspStrTyp3[u4Type - TYPE_AUD_MIX];
        fgLoadCode            = TRUE;
        break;
#endif
#if (0) // Light: Remove mixsound program, it's used for AinIntRAM in MT5371
    case TYPE_AUD_MIX:
        // Only Comm MIX
        fgLoadTable = _fgMixTblFromFlash;
        dwOfst      = u4GetFlashData (dwFlashOfst + (DSPF_MIX_SOUND_SA << 2));
        dwSz        = u4GetFlashData (dwFlashOfst + (DSPF_MIX_SOUND_SZ << 2));
        uRamCodeType = TYPE_MIX_ICACHE;
        _uDspMixStrTyp = COMMMIX_STREAM;
        fgLoadCode  = TRUE;
        break;
#endif
    case TYPE_AUD_CMM:
        switch (u4Type)
        {
        case AUD_POSTRAM:
#ifdef DOWNMIX_SUPPORT
            /* move VSURR code from flash */
            //Rice: because no dynamic virtual surround is supported,
            //you could only choose one VS to support.
    #ifdef DSP_SUPPORT_SRSWOW  
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_SRS_WOW_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_SRS_WOW_SZ<<2));
    #elif defined(DSP_SUPPORT_SRSTSXT)
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_SRS_TSXT_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_SRS_TSXT_SZ<<2));
    #elif defined(DSP_SUPPORT_SRSTSHD) //sunman_tshd
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_SRS_TSHD_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_SRS_TSHD_SZ<<2));              
    #else
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_VSURR_POST_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_VSURR_POST_SZ<<2));
    #endif
            dwCodeDestAdr =
                (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + VIRTUAL_SURROUND_PRAM_ADR) * 4;
            // Rice: check if exceed the down-mix section @20060804
            ASSERT((dwCodeDestAdr+((dwSz-1)*4)) <
                   ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+SPEAKER_ENHANCEMENT_PRAM_ADR) * 4));
            dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
            vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            /* move VSURR code from flash */
#endif //DOWNMIX_SUPPORT
#ifdef CC_AUD_BBE_SUPPORT
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_BBE_VIVA4_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_BBE_VIVA4_SZ<<2));
            if (dwSz > 0)
            {
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + SPEAKER_ENHANCEMENT_PRAM_ADR) * 4;
                ASSERT((dwCodeDestAdr+((dwSz-1)*4)) <
                    ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+VOICE_ENHANCEMENT_PRAM_ADR) * 4));                    
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);           
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
#endif // CC_AUD_BBE_SUPPORT
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SZ << 2));
            u2RamCodeType = TYPE_POST_ICACHE;
            fgLoadCode = TRUE;
            break;
	    case AUD_DDCO:
             dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SZ<<2));
            dwCodeDestAdr =
                (dwGetDspIcacheAdr (TYPE_DEC1_ICACHE) + DDCO_PRAM_ADR) * 4;        
            dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
            vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);	     
			
	       dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SA_2<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SZ_2<<2));
            dwCodeDestAdr =
                (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + DDCO_PRAM_ADR_2) * 4;        
            dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
            vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);	
		dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SZ << 2));
            u2RamCodeType = TYPE_POST_ICACHE;
            fgLoadCode = TRUE;
	      fgLoadTable = TRUE;	
            break;		
        case AUD_COMMRAM:
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_COMM_CODE_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_COMM_CODE_SZ << 2));

            u2RamCodeType = TYPE_COMM_ICACHE;
            fgLoadCode = TRUE;
#ifdef CC_MT5360B   
            //for MT5360B, need to load down-mix table when load common code
            fgLoadTable = TRUE;
#endif            
//            fgWaitDsp = FALSE;
            break;
        case AUD_ROMCOD:
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_ROM_CODE_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_ROM_CODE_SZ << 2));

            u2RamCodeType = TYPE_ROM_ICACHE;
            fgLoadCode = TRUE;
//            fgWaitDsp = FALSE;
            break;
        // Light: Add load code for AinIntRAM
        case AUD_AININTRAM:

#ifdef CC_MT5360B_AUD_FIX_PROM
        // dynamic load by Shawn
        // load temp Ain vector every loading to make sure Ain vector is in DRAM
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_TEMP_AIN_CODE_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_TEMP_AIN_CODE_SZ << 2)); 
            dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC2_ICACHE);        
            vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4); 
#endif

#if (defined(CC_NEW_PROGRAM_DEF) || defined(CC_MT5360B))
// MT5360B/MT5393
            // dynamic load by Shawn
            // load critical code
            // In current stage, 
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_CRITICAL_CODE_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_CRITICAL_CODE_SZ << 2)); 
            if (dwSz)
            {
                dwDECICAddr = dwGetDspIcacheAdr (TYPE_CRITICAL_ICACHE);        
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }
#endif

#ifdef __MODEL_slt__
#ifdef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_VCT_MP2DEC3_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_VCT_MP2DEC3_SZ << 2)); 
            if (dwSz)
            {
            dwDECICAddr = dwGetDspIcacheAdr (TYPE_VCT_DEC3_ICACHE);        
            vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }
#endif
#endif

            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AIN_INTRAM_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AIN_INTRAM_SZ << 2));

            u2RamCodeType = TYPE_AIN_ICACHE;
            fgLoadCode = TRUE;
//            fgWaitDsp = FALSE;
            break;
       #ifdef SIF_ADC     
         case AUD_AININTRAM_MINER:
              dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AIN_INTRAM_MINER_SA << 2));
              dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AIN_INTRAM_MINER_SZ << 2)); 
              u2RamCodeType = TYPE_AIN_ICACHE;
              fgLoadCode = TRUE; 
            break;
        #endif   //SIF_ADC 
        default:
            fgLoadCode = FALSE;
//            fgWaitDsp = FALSE;
            return;
        }
        break;
    default:
        return;
    }
    if (fgLoadTable)
    {
        vLoadDspDRAMTable (u4Type);
        vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG, 0x1);
    }

    dwDECICAddr = dwGetDspIcacheAdr (u2RamCodeType);

#ifndef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT
#ifdef CC_MT5391_AUD_3_DECODER
    // Need to check if dec3.pra here, by gallen. If it's dec3.pra
    // then the DRAM addr should be 0xa00 Dwords offset to _u4DspICacheAddr[TYPE_ROM_ICACHE]. 
    if ( (u4Type & 0xC0) == TYPE_AUD_MIX )
    {
        dwDECICAddr += ((DSP_ROM_SZ>>8)<<6);
    }
#endif    
#endif

    // Check dwSz > 0 for preventing take empty entry code
    /*lint -e{774}*/
    if ((fgLoadCode == TRUE) && (dwSz >= 1))
    {
#ifdef CC_MT5391_AUD_DYNAMICLOAD_SUPPORT 
// dynamic load by Shawn
// do not check dynamic loading region
// maybe implement later...
        if (u2RamCodeType < TYPE_BASIC_END_ID)
#endif
        // Rice: check if exceed to next section. @20060804

#ifdef CC_MT5360B_DSP_ROM_BOOT        
        if (u4Type!= AUD_PAL_DEC2)
        {
#endif        
        if (u2RamCodeType < ICACHE_MAX_IDX) // except the last section
        {      	
            if (((dwDECICAddr+(dwSz-1))*4) >= (dwGetDspIcacheAdr((UINT16)(u2RamCodeType+1))*4))
            {
                LOG(1, "Error -> Not enough space for RAM code type %d\n", u2RamCodeType);
                //LOG(1, "DRAM addr1 : %d\n",  (((UINT32) _u4DspICacheAddr[uRamCodeType]) <<6));
                //LOG(1, "DRAM addr2 : %d\n",  (((UINT32) _u4DspICacheAddr[uRamCodeType+1]) <<6));
                //LOG(1, "ram code len : %d\n", dwSz);
                ASSERT(0);
            }
        }
#ifdef CC_MT5360B_DSP_ROM_BOOT         
        }
#endif        
        //  Flush Cache
        vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4),
                        (dwSz - 1) * 4);
#ifdef DSP_RAM_CODE_CHKSUM
        _dwDspLoadRetryCnt = 0;
        do
        {
            vFlash2DramDma ((dwDECICAddr * 4),
                            u4GetDspImgAddr () + (dwOfst * 4),
                            (dwSz - 1) * 4);
            _dwDspChkSum =
                dwCRC32 ((UINT32 *) (dwDECICAddr * 4 + DRAMANC_SA), dwSz - 1,
                         -1L);
            _dwDspChkSum1 =
                u4GetFlashData (u4GetDspImgAddr () + dwOfst * 4 +
                             (dwSz - 1) * 4);

            _dwDspLoadRetryCnt++;
        }
        while (_dwDspChkSum1 != _dwDspChkSum &&
               _dwDspLoadRetryCnt <= DSP_RAM_CODE_RETRY_CNT);
#endif
        /* if (fgWaitDsp)
           {
           _uDspState = DSP_FLUSH_CACHE;
           }
           else                        // Not wait DSP
           { */
        vFlushDspICache (FALSE);
        //}
    }
    else if (fgLoadCode == FALSE)
    {
        LOG(1, "Error -> Not matched RAM code type!!");
        //vShowError (ERR_ADSP_LOADCODE_ERR);
    }
    else
    {
        LOG(1, "Error -> No RAM code type %d exists!!\n", u2RamCodeType);
    }

}



#ifdef __MODEL_slt__
void vDspSLTLoadRamCode (void)
{
    UINT32 dwDECICAddr;
    UINT8 uRamCodeType;
    UINT32 dwOfst, dwSz;
    BOOL fgLoadTable, fgLoadCode;
    UINT32 dwCodIndx;
    UINT32 dwFlashOfst;
    UINT32 dwCodeDestAdr,dwCodeSrcAdr;

    fgLoadCode  = FALSE;
    fgLoadTable = FALSE;
    dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr

    // Put common pra in AAC_DEC2 area, now load to dram 
    dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AAC_DEC2X_SA << 2));
    dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AAC_DEC2X_SZ << 2));             

    uRamCodeType = TYPE_COMM_ICACHE;
    fgLoadCode = TRUE;
    dwDECICAddr = dwGetDspIcacheAdr (uRamCodeType);
    if ((fgLoadCode == TRUE) && (dwSz >= 1))
    {
        if (uRamCodeType < ICACHE_MAX_IDX) // except the last section
        {      	
            if (((dwDECICAddr+(dwSz-1))*4) >= (dwGetDspIcacheAdr((uRamCodeType+1))*4))
            {
                LOG(1, "Error -> Not enough space for RAM code type %d\n", uRamCodeType);
                ASSERT(0);
            }
        }
        //  Flush Cache
        vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
        vFlushDspICache (FALSE);
    }
    
    // Put ain pra in MP2_DEC3 area, now load to dram 
    dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_VCT_MP2DEC3_SA << 2));
    dwSz = u4GetFlashData (dwFlashOfst + (DSPF_VCT_MP2DEC3_SZ << 2));             
    
    uRamCodeType = TYPE_AIN_ICACHE;
    fgLoadCode = TRUE;
    dwDECICAddr = dwGetDspIcacheAdr (uRamCodeType); 
    if ((fgLoadCode == TRUE) && (dwSz >= 1))
    {
        if (uRamCodeType < ICACHE_MAX_IDX) // except the last section
        {      	
            if (((dwDECICAddr+(dwSz-1))*4) >= (dwGetDspIcacheAdr((uRamCodeType+1))*4))
            {
                LOG(1, "Error -> Not enough space for RAM code type %d\n", uRamCodeType);
                ASSERT(0);
            }
        }
        //  Flush Cache
        vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
        vFlushDspICache (FALSE);
    }
}
#endif //SLT





/***************************************************************************
     Function : vSetDRCInfo
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vSetDRCInfo (void)
{
    //UINT16 u2DspData;
    //UINT32 dwDspInfo;

    /* for first decoder */
    vWriteDspWORD (ADDR_RC2D_DRC_MAX_AVE_ORDER,
                   uReadDspShmBYTE (B_DRC_AVEORDER));
    vWriteDspWORD (ADDR_RC2D_DRC_ATTACK_ORDER,
                   uReadDspShmBYTE (B_DRC_ATTACKORDER));
    vWriteDspWORD (ADDR_RC2D_DRC_VALUE_THRESHOLD,
                   u2ReadDspShmWORD (W_DRC_THRESHOLD));
    vWriteDspWORD (ADDR_RC2D_DRC_VALUE_MARGIN,
                   u2ReadDspShmWORD (W_DRC_MARGIN));
    WriteDspCommDram (ADDR_RC2D_DRC_RELEASE_GAIN,
                      u4ReadDspShmDWRD (D_DRC_RE_GAIN));

    /* for second decoder */
    vWriteDspWORD (ADDR_RC2D_DRC_MAX_AVE_ORDER_DEC2,
                   uReadDspShmBYTE (B_DRC_AVEORDER));
    vWriteDspWORD (ADDR_RC2D_DRC_ATTACK_ORDER_DEC2,
                   uReadDspShmBYTE (B_DRC_ATTACKORDER));
    vWriteDspWORD (ADDR_RC2D_DRC_VALUE_THRESHOLD_DEC2,
                   u2ReadDspShmWORD (W_DRC_THRESHOLD));
    vWriteDspWORD (ADDR_RC2D_DRC_VALUE_MARGIN_DEC2,
                   u2ReadDspShmWORD (W_DRC_MARGIN));
    WriteDspCommDram (ADDR_RC2D_DRC_RELEASE_GAIN_DEC2,
                      u4ReadDspShmDWRD (D_DRC_RE_GAIN));
}

/***************************************************************************
     Function : wReadDSPPROMVer
  Description : Read DSP Program ROM Version
    Parameter : None
    Return    : wDSPPROMVer - XYZZ (Hex)
                  X: ROM/RAM Code Compatible Version
                  Y: New Feature Version
                 ZZ: Bug Fixed Version
***************************************************************************/
UINT16 wReadDspPROMVer (void)
{
    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    return ((UINT16) wReadDspWORD (ADDR_D2RC_ROM_CODE_VERSION));
}

/***************************************************************************
     Function : vDspSetSyncMode
  Description : Set PTS mode
    Parameter : ucDecId : Decoder ID
                eMode :   AV_SYNC_FREE_RUN = 0,     // no need to syn
                          AV_SYNC_SLAVE = 1,        // syn to STC
                          AV_SYNC_AUDIO_MASTER = 2, // update A-PTS to A-STC, may use in PCR or HardDisk playback
    Return    :
***************************************************************************/
void vDspSetSyncMode(UCHAR ucDecId, AV_SYNC_MODE_T eMode)
{
    UINT16   u2SystemSetupValue;
	
     u2SystemSetupValue = (u2ReadDspShmWORD(W_SYSTEM_SETUP) & 0x7fff );
 
     //AV sync mode
     if (eMode == AV_SYNC_FREE_RUN)
     {
            u2SystemSetupValue = (u2SystemSetupValue | (1<<15) ) ;
		//printf("########AV_SYNC_FREE_RUN #######\n");
     }
     else if (eMode == AV_SYNC_SLAVE)
     {
           u2SystemSetupValue = (u2SystemSetupValue | (1<<15) ) ;
	    //printf("########### AV_SYNC_SLAVE #######\n");	   
     }
     else //AV_SYNC_AUDIO_MASTER
     {
          u2SystemSetupValue = u2SystemSetupValue  ;
		 // printf("########## AV_SYNC_AUDIO_MASTER #########\n");
      }    

	 
      vWriteDspShmWORD(W_SYSTEM_SETUP, (UINT16)u2SystemSetupValue);	
	vWriteDspWORD (ADDR_RC2D_SYSTEM_SETUP,   u2ReadDspShmWORD (W_SYSTEM_SETUP));

   _aeAvSyncMode[ucDecId] = eMode;
}

/***************************************************************************
     Function : rDspGetSyncMode
  Description : Get PTS mode
    Parameter : ucDecId : Decoder ID
                
    Return    : eMode :   AV_SYNC_FREE_RUN = 0,     // no need to syn
                          AV_SYNC_SLAVE = 1,        // syn to STC
                          AV_SYNC_AUDIO_MASTER = 2, // update A-PTS to A-STC, may use in PCR or HardDisk playback
***************************************************************************/
AV_SYNC_MODE_T eDspGetSyncMode(UCHAR ucDecId)
{
   return _aeAvSyncMode[ucDecId];
}

/***************************************************************************
     Function : i4DspSendPts
  Description :
    Parameter :
    Return    :
***************************************************************************/
INT32 i4DspSendPts (UINT32 u4Pts, UINT32 u4ParPnt)
{
    // Send 1 out of 2 Pts to DSP
#if (0)
    _fgSendPts2Dsp = !_fgSendPts2Dsp;
    if (!_fgSendPts2Dsp)
    {
        return DSP_OK;
    }
#endif

    if (fgDspBusy ())
    {
        return DSP_BUSY;
    }
    
    if (_fgDspStopIssue)
    {
        return DSP_FAIL;
    }
    
    vSendDspLongInt (INT_RC2D_PTS, u4ParPnt, u4Pts);
    //u4ParPnt = 0 -> don't care!
    _u4PrsPts = 0;              // clear _u4PrsPts

    return DSP_OK;
}
/***************************************************************************
     Function : i4DspSendPtsDec2
  Description :
    Parameter :
    Return    :
***************************************************************************/
INT32 i4DspSendPtsDec2 (UINT32 u4Pts, UINT32 u4ParPnt)
{
    // Send 1 out of 2 Pts to DSP
#if (0)    
    _fgSendPts2DspDec2 = !_fgSendPts2DspDec2;
    if (!_fgSendPts2DspDec2)
    {
        return DSP_OK;
    }
#endif

    if (fgDspBusy ())
    {
        return DSP_BUSY;
    }
    
    if (_fgDspStopIssueDec2)
    {
        return DSP_FAIL;
    }
    
    vSendDspLongInt (INT_RC2D_PTS_DEC2, u4ParPnt, u4Pts);
    //u4ParPnt = 0 -> don't care!
    _u4PrsPtsDec2 = 0;              // clear _u4PrsPtsDec2

    return DSP_OK;
}
#ifdef CC_MT5391_AUD_3_DECODER
/***************************************************************************
     Function : i4DspSendPtsDec3
  Description :
    Parameter :
    Return    :
***************************************************************************/
INT32 i4DspSendPtsDec3 (UINT32 u4Pts, UINT32 u4ParPnt)
{
    // Send 1 out of 2 Pts to DSP
#if (0)
    _fgSendPts2DspDec3 = !_fgSendPts2DspDec3;
    if (!_fgSendPts2DspDec3)
    {
        return DSP_OK;
    }
#endif

    if (fgDspBusy ())
    {
        return DSP_BUSY;
    }
    
    if (_fgDspStopIssueDec3)
    {
        return DSP_FAIL;
    }
    
    vSendDspLongInt (INT_RC2D_PTS_DEC3, u4ParPnt, u4Pts);
    LOG(9, "INT_RC2D_PTS_DEC3...\n");
    //u4ParPnt = 0 -> don't care!
    _u4PrsPtsDec3 = 0;              // clear _u4PrsPtsDec3

    return DSP_OK;
}
#endif
/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspCmd (UINT32 u4Cmd)
{
    vSendADSPTaskMessage(u4Cmd);
}

BOOL fgCheckDspCmd(UINT32 u4DspCmd)
{
    INT32 i4Ret = TRUE;

    if (u4DspCmd == DSP_PLAY)
    {
        AUD_UopCommandDone(AUD_DEC_MAIN, DSP_PLAY);
        if (_fgDspPlay)
        {
            i4Ret = FALSE;
        }
        else 
        {
            _fgDspPtsSet = FALSE;  
            _fgDspPlay = TRUE;
            _fgDspPause = FALSE;
            _fgDspStop = FALSE;
            _fgDspStopIssue = FALSE;
            _fgSendPts2Dsp = FALSE;
        }    
    }
    else if (u4DspCmd == DSP_STOP)
    {
        if (_fgDspStopIssue && (!_fgDspFlush))
        {
            AUD_UopCommandDone(AUD_DEC_MAIN, DSP_STOP);
            LOG(1, "_fgDspStopIssue return \n");
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSet = FALSE;
            _fgDspPlay = FALSE;
            _fgDspFlush = FALSE;
            _fgDspStopIssue = TRUE;
        }
    }
    else if (u4DspCmd == DSP_FLUSH)
    {
        if ((_fgDspStop) || (_fgDspFlush))
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspFlush = TRUE;
            _fgDspPtsSet = FALSE;
            _fgDspStopIssue = TRUE;
            _fgDspPlay = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PAUSE)
    {
        AUD_UopCommandDone(AUD_DEC_MAIN, DSP_PAUSE);
        
        // assume DSP_RESUME is only issued in DSP_PAUSE state
        if (!_fgDspPlay || _fgDspStop)	//koro: should not pause when DSP stop
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSet = FALSE;
            _fgDspPlay = FALSE;
            _fgDspPause = TRUE;
        }
    }

    else if (u4DspCmd == DSP_RESUME)
    {
        AUD_UopCommandDone(AUD_DEC_MAIN, DSP_RESUME);

        if (!_fgDspPause)
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSet = FALSE;  
            _fgDspPlay = TRUE;
            _fgDspPause = FALSE;
        }
    }
    else if (u4DspCmd == DSP_WAKEUP)
    {
        // If we send wakeup before.. do not send again
        if (_fgDspWakeUp)
        {
            i4Ret = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PLAY_DEC2)
    {
        AUD_UopCommandDone(AUD_DEC_AUX, DSP_PLAY_DEC2);
        if (_fgDspPlayDec2)
        {
            i4Ret = FALSE;
        }
        else 
        {
            _fgDspPtsSetDec2 = FALSE;
            _fgDspPlayDec2 = TRUE;
            _fgDspStopDec2 = FALSE;
            _fgDspStopIssueDec2 = FALSE;
            _fgSendPts2DspDec2 = FALSE;
        }    
    }
    else if (u4DspCmd == DSP_STOP_DEC2)
    {
        if (_fgDspStopIssueDec2 && (!_fgDspFlushDec2))
        {
            AUD_UopCommandDone(AUD_DEC_AUX, DSP_STOP_DEC2);
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSetDec2 = FALSE;
            _fgDspPlayDec2 = FALSE;
            _fgDspFlushDec2 = FALSE;
            _fgDspStopIssueDec2 = TRUE;
        }
    }
    else if (u4DspCmd == DSP_FLUSH_DEC2)
    {
        if ((_fgDspStopDec2) || (_fgDspFlushDec2))
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspFlushDec2 = TRUE;
            _fgDspPtsSetDec2 = FALSE;
            _fgDspStopIssueDec2 = TRUE;
            _fgDspPlayDec2 = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PAUSE_DEC2)
    {
        AUD_UopCommandDone(AUD_DEC_AUX, DSP_PAUSE_DEC2);
        // assume DSP_RESUME is only issued in DSP_PAUSE state
        if (!_fgDspPlayDec2)
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSetDec2 = FALSE;
            _fgDspPlayDec2 = FALSE;
            _fgDspPauseDec2 = TRUE;
        }
    }

    else if (u4DspCmd == DSP_RESUME_DEC2)
    {
        AUD_UopCommandDone(AUD_DEC_AUX, DSP_RESUME_DEC2);

        if (!_fgDspPauseDec2)
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSetDec2 = FALSE;  
            _fgDspPlayDec2 = TRUE;
            _fgDspPauseDec2 = FALSE;
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    else if (u4DspCmd == DSP_PLAY_DEC3)
    {
        AUD_UopCommandDone(AUD_DEC_THIRD, DSP_PLAY_DEC3);
        if (_fgDspPlayDec3)
        {
            i4Ret = FALSE;
        }
        else 
        {
            _fgDspPtsSetDec3 = FALSE;
            _fgDspPlayDec3 = TRUE;
            _fgDspStopDec3 = FALSE;
            _fgDspStopIssueDec3 = FALSE;
            _fgSendPts2DspDec3 = FALSE;
        }    
    }
    else if (u4DspCmd == DSP_STOP_DEC3)
    {
        if (_fgDspStopIssueDec3 && (!_fgDspFlushDec3))
        {
            AUD_UopCommandDone(AUD_DEC_THIRD, DSP_STOP_DEC3);
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSetDec3 = FALSE;
            _fgDspPlayDec3 = FALSE;
            _fgDspFlushDec3 = FALSE;
            _fgDspStopIssueDec3 = TRUE;
        }
    }
    else if (u4DspCmd == DSP_FLUSH_DEC3)
    {
        if ((_fgDspStopDec3) || (_fgDspFlushDec3))
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspFlushDec3 = TRUE;
            _fgDspPtsSetDec3 = FALSE;
            _fgDspStopIssueDec3 = TRUE;
            _fgDspPlayDec3 = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PAUSE_DEC3)
    {
        AUD_UopCommandDone(AUD_DEC_THIRD, DSP_PAUSE_DEC3);
        // assume DSP_RESUME is only issued in DSP_PAUSE state
        if (!_fgDspPlayDec3)
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSetDec3 = FALSE;
            _fgDspPlayDec3 = FALSE;
            _fgDspPauseDec3 = TRUE;
        }
    }

    else if (u4DspCmd == DSP_RESUME_DEC3)
    {
        AUD_UopCommandDone(AUD_DEC_THIRD, DSP_RESUME_DEC3);

        if (!_fgDspPauseDec3)
        {
            i4Ret = FALSE;
        }
        else
        {
            _fgDspPtsSetDec3 = FALSE;  
            _fgDspPlayDec3 = TRUE;
            _fgDspPauseDec3 = FALSE;
        }
    }
#endif
    
    return i4Ret;
}

/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspSetFifoReadPtr(UCHAR ucDecId, UINT32 u4ReadPtr)
{
    UINT32 u4FifoStartDspInterAddr;
    UINT32 u4FifoEndDspInterAddr;
    UINT32 u4FifoStartPhysicalAddr;
    UINT32 u4FifoEndPhysicalrAddr;
    UINT32 u4ReadPtrDspInterAddr;
    
    DSP_GetAFIFOPhysicalAddr(ucDecId, &u4FifoStartPhysicalAddr, &u4FifoEndPhysicalrAddr);
    u4FifoStartDspInterAddr = DSP_INTERNAL_ADDR(u4FifoStartPhysicalAddr);
    u4FifoEndDspInterAddr = DSP_INTERNAL_ADDR(u4FifoEndPhysicalrAddr);
    u4ReadPtrDspInterAddr = DSP_INTERNAL_ADDR(u4ReadPtr);

    if ((u4FifoStartDspInterAddr > u4ReadPtrDspInterAddr) || 
    	(u4FifoEndDspInterAddr < u4ReadPtrDspInterAddr))
    {
        LOG(1, "Invalid Read Pointer\n");
        return;
    }
#ifndef CC_MT5391_AUD_3_DECODER
//#if 1
    if (ucDecId == AUD_DEC_MAIN)
    {
        vWriteDspShmDWRD (D_DECODING_STR_PNT, u4ReadPtrDspInterAddr);
    }
    else
    {
    	vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC2, u4ReadPtrDspInterAddr);
    }	
#else
    // MAIN and THIRD both use Fifo1
    if (ucDecId == AUD_DEC_AUX)
    {
        vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC2, u4ReadPtrDspInterAddr);
    }
    else
    {
    	vWriteDspShmDWRD (D_DECODING_STR_PNT, u4ReadPtrDspInterAddr);
    }	
#endif
}

/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
UINT32 dwDspSetTableAddr (BOOL fgFirstDecoder)
{
    UINT32 dwSFreq;

    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    dwSFreq =
        dReadDspCommDram (fgFirstDecoder ? ADDR_D2RC_RISC_INFO_SAMPLING_RATE :
                          ADDR_D2RC_RISC_INFO_SAMPLING_RATE_DEC2);
    dwSFreq = (dwSFreq & 0xff00) >> 8;
    return (dwSFreq);
}

/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspBlockPtrReset (BOOL fgFirstDecoder)
{
    UINT32 dwTryCnt = 0;
    BOOL fgDspPlay;
    UINT32 dwAFifo;
    UINT32 dwBufPNTReg;

    if (fgFirstDecoder)
    {
        fgDspPlay = _fgDspPlay;
        dwAFifo = _u4AFIFO[0];
        dwBufPNTReg = REG_BUF_PNT;
    }
    else
    {
        fgDspPlay = _fgDspPlayDec2;
        dwAFifo = _u4AFIFODec2[0];
        dwBufPNTReg = REG_BUF_PNT_DEC2;
    }
    while (fgDspPlay)
    {
        dwTryCnt++;
        if (!fgDspBusy ())
        {
            vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, dwAFifo, dwBufPNTReg);
            break;
        }

        if (dwTryCnt == DSP_TIMEOUT)
        {
            vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, dwAFifo, dwBufPNTReg);
            break;
        }
    }
}

/***************************************************************************
     Function : fgDspReadPtrSet
  Description : Update DSP read pointer when DSP is stopped to avoid blocking
                parser channel data input
    Parameter :
    Return    :
***************************************************************************/
BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address)
{
    UINT32 dwTryCnt = 0;
    BOOL fgDspStop;
    UINT32 dwAFifo;
    UINT32 dwBufPNTReg;
    BOOL fgRet = FALSE;
    CRIT_STATE_T rCritState;

    VERIFY((ucDecId == AUD_DEC_MAIN) || (ucDecId == AUD_DEC_AUX));

    if (ucDecId == AUD_DEC_MAIN)
    {
        fgDspStop = _fgDspStop;
        //dwAFifo = NONCACHE(u4Address); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        dwAFifo = PHYSICAL(u4Address);
        dwBufPNTReg = REG_BUF_PNT;
    }
    else
    {
        fgDspStop = _fgDspStopDec2;
        //dwAFifo = NONCACHE(u4Address);
        dwAFifo = PHYSICAL(u4Address);
        dwBufPNTReg = REG_BUF_PNT_DEC2;
    }
    HalFlushInvalidateDCache();

    while (fgDspStop)
    {
        dwTryCnt++;
        rCritState = x_crit_start();
        if (!fgDspBusy ())
        {
            vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, dwAFifo, dwBufPNTReg);
            fgRet = TRUE;
            x_crit_end(rCritState);
            break;
        }
        else
        {
            x_crit_end(rCritState);
        }

        if (dwTryCnt == DSP_TIMEOUT)
        {
            fgRet = FALSE;
            break;
        }
    }

    if (!fgRet)
    {
        if (fgDspStop)
        {
            LOG(7, "[DSP] Error: Dec(%d) _fgDspStop = TRUE. Can not update RP\n");
        }
        else
        {
            LOG(7, "[DSP] Error: Dec(%d) DSP is busy. Can not update RP\n");
        }
    }
    
    return fgRet;
}


/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vSetSpeakerConfig (BOOL fgFirstDecoder)
{
    //UINT16 u2DspData;
    UINT32 dwDspInfo;
//    UINT8 bStrType;
    UINT32 u4Addr;

    if (fgFirstDecoder)
    {
//        bStrType = _bDspStrTyp;
        dwDspInfo = u4ReadDspShmDWRD (D_SPKCFG);
        u4Addr = ADDR_RC2D_SPEAKER_CONFIG;
#if 0        
        switch (bStrType)
        {
        case AC3_STREAM:
            //Set Auto Down Mix Mode for Version 3
            vWriteDspWORD (ADDR_RC2D_AC3_DOWNMIX,
                           uReadDspShmBYTE (B_AC3AUTODNMIX));
            u2DspData = u2ReadDspShmWORD (W_AC3_SPKCFG);
            break;
        case MPEG12_STREAM:
            u2DspData = u2ReadDspShmWORD (W_MP2_SPKCFG);
            break;
        default:
            u2DspData = 0xFFFF;
        }        
#endif        
    }
    else
    {
//        bStrType = _bDspStrTypDec2;
        dwDspInfo = u4ReadDspShmDWRD (D_SPKCFG_DEC2);
        u4Addr = ADDR_RC2D_SPEAKER_CONFIG_DEC2;
#if 0        
        switch (bStrType)
        {
        case AC3_STREAM:
            //Set Auto Down Mix Mode for Version 3
            vWriteDspWORD (ADDR_RC2D_AC3_DOWNMIX_DEC2,
                           uReadDspShmBYTE (B_AC3AUTODNMIX_DEC2));
            u2DspData = u2ReadDspShmWORD (W_AC3_SPKCFG_DEC2);
            break;
        case MPEG12_STREAM:
            u2DspData = u2ReadDspShmWORD (W_MP2_SPKCFG_DEC2);
            break;
        default:
            u2DspData = 0xFFFF;
        }        
#endif        
        // bond the speaker config to 2 ch for Aux decoder
        dwDspInfo = dwDspInfo & 0x7007;
        //Check if setting is greater than 2 ch
        if ((dwDspInfo & 0x7) > 2)
        {
            dwDspInfo = (dwDspInfo & 0x7000) + 2;
        }
    }
#if 0
    if (u2DspData != 0xFFFF)
        dwDspInfo = (dwDspInfo & 0xFFF000) | (u2DspData & 0xFFF);
      
    if (fgBond2Ch ())
    {
        //Remove subfwoofer setting
        dwDspInfo = dwDspInfo & 0x7007;
        //Check if setting is greater than 2 ch
        if ((dwDspInfo & 0x7) > 2)
        {
            dwDspInfo = (dwDspInfo & 0x7000) + 2;
        }
    }
#endif
    WriteDspCommDram (u4Addr, dwDspInfo);
}

/***************************************************************************
     Function : vWriteDspSram 
  Description : write dsp sram through interrupt
    Parameter : u4Addr: address,u4Value: value in 24 bits
    Return    :
***************************************************************************/
void vWriteDspSram (UINT32 u4Addr, UINT32 u4Value)
{
    UINT32 u4TryCnt = 0;
    CRIT_STATE_T rCritState;

    while (TRUE)
    {
        u4TryCnt++;
        rCritState = x_crit_start();
        if (!fgDspBusy ())
        {
            vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, ((u4Value)),
                             u4Addr);
            x_crit_end(rCritState);
            break;
        }
        else
        {
            x_crit_end(rCritState);
            x_thread_delay(1);
            if (u4TryCnt == DSP_TIMEOUT)
            {
                break;
            }
        }
    }
}

/***************************************************************************
     Function : vWriteDspSramInISR 
  Description : write dsp sram through interrupt in RISC ISR context
    Parameter : u4Addr: address,u4Value: value in 24 bits
    Return    :
***************************************************************************/
BOOL fgWriteDspSramInISR (UINT32 u4Addr, UINT32 u4Value)
{
    BOOL fgRet = TRUE;
    if (!fgDspBusy ())
    {
        vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, ((u4Value)),
                         u4Addr);
        fgRet = TRUE;
    }
    else
    {
        fgRet = FALSE;
    }

    return fgRet;
}

/***************************************************************************
     Function : u4ReadDspSram
  Description : Read Dsp Sram through interrupt
    Parameter : u4Addr : address
    Return    : value in UIN32
***************************************************************************/
UINT32 u4ReadDspSram(UINT32 u4Addr)
{
    UINT32 u4TryCnt = 0;
    UINT32 u4Value = 0;
    CRIT_STATE_T rCritState;

    while (TRUE)
    {
        u4TryCnt++;

        rCritState = x_crit_start();
        if (!fgDspBusy ())
        {
            vSendDspLongInt (INT_RC2D_READ_DSP_MEMORY, ((u4Addr) << 8),
                             0);
            x_crit_end(rCritState);
            break;
        }
        else
        {
            x_crit_end(rCritState);
            x_thread_delay(1);
            if (u4TryCnt == DSP_TIMEOUT)
            {
                break;
            }
        }
    }
    x_thread_delay(1);
    u4Value = dwReadDspLongData();    
    return(u4Value);
}
#ifdef VOL_CHG_FIX
/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
UINT32 dwVolumeSoftChange (BOOL fgFirstDecoder)
{
    UINT32 dwVolTimeOut;
    UINT32 dwCurrSet;
    BOOL fgNextDspCmd = FALSE;
    UINT32 dwDspFreq;
    UINT32 *pdwVolPrevUsr;
    UINT32 *pdwVolLasSetting;
    UINT32 *pdwVolSettingSetp;

    if (fgFirstDecoder)
    {
        dwDspFreq = _dwDspFreq;
        pdwVolPrevUsr = &_dwVolPrevUsr;
        pdwVolSettingSetp = &_dwVolSettingSetp;
        pdwVolLasSetting = &_dwVolLasSetting;
    }
    else
    {
        dwDspFreq = _dwDspFreqDec2;
        pdwVolPrevUsr = &_dwVolPrevUsrDec2;
        pdwVolSettingSetp = &_dwVolSettingSetpDec2;
        pdwVolLasSetting = &_dwVolLasSettingDec2;
    }

    dwCurrSet = u4ReadDspShmDWRD (D_VOL);

    if (*pdwVolPrevUsr == dwCurrSet)
    {
        if (*pdwVolLastSetting != *pdwVolPrevUsr)
        {
            switch (dwDspFreq)
            {
            case SFREQ_176K:   // 176K
            case SFREQ_192K:   // 192K
                dwVolTimeOut = VOL_STEP_TIMEOUT_4X;
                break;

            case SFREQ_88K:    // 88K
            case SFREQ_96K:    // 96K
                dwVolTimeOut = VOL_STEP_TIMEOUT_2X;
                break;

            case SFREQ_16K:    // 16K
            case SFREQ_22K:    // 22K
            case SFREQ_24K:    // 24K
            case SFREQ_32K:    // 32K
            case SFREQ_44K:    // 44K
            case SFREQ_48K:    // 48K
            default:
                dwVolTimeOut = 0;
                break;
            }                   /* switch (_dwDspFreq) */

            if ((dwTimer1 (T1_VOL_IDX) != 0)
                && (dwTimer1 (T1_VOL_IDX) != 0xffffffff))
            {
                if (dwVolTimeOut > 0)   // wait timer up
                {
                    vDspCmd (UOP_DSP_MASTER_VOLUME);
                    return (INVALID_VOL_SETTING);
                }
            }

            if (*pdwVolLastSetting > *pdwVolPrevUsr)
            {
                dwCurrSet = *pdwVolLastSetting - *pdwVolSettingStep;

                if (dwCurrSet < *pdwVolPrevUsr)
                {
                    dwCurrSet = *pdwVolPrevUsr;
                }
            }
            else if (*pdwVolLastSetting < *pdwVolPrevUsr)
            {
                dwCurrSet = *pdwVolLastSetting + *pdwVolSettingStep;

                if (dwCurrSet > *pdwVolPrevUsr)
                {
                    dwCurrSet = *pdwVolPrevUsr;
                }
            }

            *pdwVolLastSetting = dwCurrSet;

            if (*pdwVolPrevUsr == *pdwVolLastSetting)
            {
                *pdwVolSettingStep = 0;
                vSetTimer1 (T1_VOL_IDX, 0);
            }
            else
            {
                // still need another step
                fgNextDspCmd = TRUE;
            }
        }                       /* _dwVolLastSetting != _dwVolPrevUsr */
    }
    else
    {                           /* _dwVolPrevUsr != dwCurrSet */

        // reset stepping
        if (dwCurrSet > *pdwVolPrevUsr)
        {
            *pdwVolSettingStep = dwCurrSet - *pdwVolPrevUsr;
        }
        else
        {                       /* dwCurrSet < _dwVolPrevUsr */

            *pdwVolSettingStep = *pdwVolPrevUsr - dwCurrSet;
        }

        switch (dwDspFreq)
        {
        case SFREQ_176K:       // 176K
        case SFREQ_192K:       // 192K
            dwVolTimeOut = VOL_STEP_TIMEOUT_4X;

            *pdwVolSettingStep = *pdwVolSettingStep >> 2;   // divide to 4 steps
            if (*pdwVolSettingStep > VOL_CHG_MAX_STEP_4X)
            {
                *pdwVolSettingStep = VOL_CHG_MAX_STEP_4X;
            }
            break;

        case SFREQ_88K:        // 88K
        case SFREQ_96K:        // 96K
            dwVolTimeOut = VOL_STEP_TIMEOUT_2X;

            *pdwVolSettingStep = *pdwVolSettingStep >> 1;   // divide to 2 steps
            if (*pdwVolSettingStep > VOL_CHG_MAX_STEP_2X)
            {
                *pdwVolSettingStep = VOL_CHG_MAX_STEP_2X;
            }
            break;

        case SFREQ_16K:        // 16K
        case SFREQ_22K:        // 22K
        case SFREQ_24K:        // 24K
        case SFREQ_32K:        // 32K
        case SFREQ_44K:        // 44K
        case SFREQ_48K:        // 48K
        default:
            /*
               no need stepping
             */
            dwVolTimeOut = 0;
            *pdwVolSettingStep = 0;
            break;
        }

        if ((*pdwVolSettingStep > 0) && (*pdwVolSettingStep <= VOL_MAX))
        {
            if (dwCurrSet > *pdwVolPrevUsr)
            {
                *pdwVolLastSetting = *pdwVolPrevUsr + *pdwVolSettingStep;
            }
            else
            {                   /* dwCurrSet < _dwVolPrevUsr */

                *pdwVolLastSetting = *pdwVolPrevUsr - *pdwVolSettingStep;
            }

            // for next step
            fgNextDspCmd = TRUE;
        }
        else
        {                       /* _dwVolSettingStep == 0 */

            // no need next step
            *pdwVolLastSetting = dwCurrSet;
            vSetTimer1 (T1_VOL_IDX, 0);
        }

        *pdwVolPrevUsr = dwCurrSet;
        dwCurrSet = *pdwVolLastSetting;
    }

    if (dwCurrSet > VOL_MAX)    // error condition, overflow
    {
        // reset all related variables
        dwCurrSet = _dwVolPrevUsr;
        *pdwVolLastSetting = *pdwVolPrevUsr;
        *pdwVolSettingStep = 0;
        vSetTimer1 (T1_VOL_IDX, 0);
        fgNextDspCmd = FALSE;
    }

    if (fgNextDspCmd == TRUE)
    {
        vDspCmd (UOP_DSP_MASTER_VOLUME);
        vSetTimer1 (T1_VOL_IDX, dwVolTimeOut);
    }

    return (dwCurrSet);
}
#endif /* VOL_CHG_FIX */

//-----------------------------------------------------------------------------
/** vSetIecConfig
 *  Set IEC Configuration.
 *
 *  @param  u1DecId  decoder ID.
 *  @param  u4FreqIdx  frequency index. 
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void vSetIecConfig(UINT8 u1DecId, UINT32 u4FreqIdx)
{
    UINT8 u1ChSel;
    UINT8 u1DownSample = SPOUT_DOWNSAMPLE_0;
    UINT8 u1IecDecSel = SPOUT_SPDF_SEL_MAIN;
    AUDIO_CFG_T rAudCfg;

    u1ChSel = (uReadDspShmBYTE (B_IEC_PCMCH) & 0x07);
        
    if (u1ChSel == (UINT8)AUD_IEC_AUX)
    {
        u1IecDecSel = SPOUT_SPDF_SEL_AUX;
        u1ChSel = (UINT8)AUD_IEC_PCM_CH_L_R;
    }

    // For high sampling rate IEC config
    // LPCM will do downsample filtering, so IEC ch will be redirected to CH7/8
    switch (u4FreqIdx)
    {
    case FS_88K:               // 88K
    case FS_96K:               // 96K
        if ((uReadDspShmBYTE (B_IEC_MAX_FREQ)) == SV_48K)
        {
            u1DownSample = SPOUT_DOWNSAMPLE_2;
        }
        break;
    case FS_176K:              // 176K
    case FS_192K:              // 192k
        if ((uReadDspShmBYTE (B_IEC_MAX_FREQ)) == SV_48K)
        {
            u1DownSample = SPOUT_DOWNSAMPLE_4;
        }
        else if ((uReadDspShmBYTE (B_IEC_MAX_FREQ)) == SV_96K)
        {
            u1DownSample = SPOUT_DOWNSAMPLE_2;
        }
        break;
    default:
        break;
    }
        
    rAudCfg.u1IecDownSamp = u1DownSample;
    rAudCfg.u1IecChSel = u1ChSel;
    rAudCfg.fgIecMute = (BOOL)(uReadDspShmBYTE (B_IEC_MUTE) & 0x01);
    rAudCfg.u1IecOutBitNum = (UINT8)(uReadDspShmBYTE(B_IEC_WORD_LENGTH) & 0x03);
    rAudCfg.u1IecDecSel = u1IecDecSel;

    // Set copyright information
    vWriteDspWORD (ADDR_RC2D_CGMS_INFO, (uReadDspShmBYTE (B_IEC_COPYRIGHT)));

    // Set category code
    vWriteDspWORD (ADDR_RC2D_DISC_TYPE, (uReadDspShmBYTE (B_IEC_CATEGORY_CODE)));

    // Set wordlength
    vWriteDspWORD (ADDR_RC2D_SPDIF_WORD_LENGTH, (uReadDspShmBYTE (B_IEC_WORD_LENGTH)));

    vSetIecConfigReg(u1DecId, &rAudCfg);
}

/***************************************************************************
     Function : vDspSetFreq
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspSetFreq (UINT32 u4FreqIdx, BOOL fgResetDAC, BOOL fgFirstDecoder)
{
    UINT32 u4TargetADCFmt;

    u4TargetADCFmt = u4FreqIdx;
    vSetIecConfig((UINT8)fgFirstDecoder, u4FreqIdx);
    if (fgResetDAC)
    {
        vDspAdacFmt (u4TargetADCFmt, fgFirstDecoder);
    }
}

/***************************************************************************
     Function : fgDspGetMpgTyp
  Description :
    Parameter :
       Author : sammy
    Return    :
***************************************************************************/
BOOL fgDspGetMpgTyp (UINT32 * pu4MpgTyp, BOOL fgFirstDecoder)
{
    if (pu4MpgTyp != NULL)
    {
        if (fgFirstDecoder)
        {                           // first decoder
            if (_fgDspMpgTypGot && _fgDspStop)
            {
                *pu4MpgTyp = _dwDspMpgTyp;
                // 20030810 sammy modified
                // self clear bitrate got to prevent
                // cheating next get bitrate command
                _fgDspMpgTypGot = FALSE;
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {                           // second decoder
            if (_fgDspMpgTypGotDec2 && _fgDspStopDec2)
            {
                *pu4MpgTyp = _dwDspMpgTypDec2;
                // 20030810 sammy modified
                // self clear bitrate got to prevent
                // cheating next get bitrate command
                _fgDspMpgTypGotDec2 = FALSE;
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }

}

/***************************************************************************
     Function : vResetDecInfo
  Description : Reset decoding information, decoding dynamic parameter,
                decoding fixed parameters
                It is invoked when decoding is changed.
    Parameter :
       Author : sammy
    Return    :
***************************************************************************/
void vResetDecInfo (BOOL fgFirstDecoder)
{
    UINT32 i;

    if (fgFirstDecoder)
    {                 
         // first decoder
        for (i = 0; i < DSP_D2RC_INFO_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_D2RC_DECODING_INFO_BASE + i, 0);
        }
        // Don't clear this information in EU model
        // due to Main and Aux decoder use the same address
//#ifndef CC_AUD_DVBT_SUPPORT   
    	if(AUD_GetTargetCountry() != (UINT32)COUNTRY_EU)
        {
            for (i = 0; i < DSP_RC2D_SETTING_AREA_SZ; i++)
            {
                WriteDspCommDram (ADDR_RC2D_DECODER_INFO_BASE + i, 0);
            }
    	}
//#endif
        // Initial ctrl page 3 for first decoder
        for (i = 0; i < DSP_RC2D_CTRL3_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_CTRL3_BASE + i, 0);
        }
    }
    else
    {                           // second decoder
        for (i = 0; i < DSP_D2RC_INFO_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_D2RC_DECODING_INFO_BASE_DEC2 + i, 0);
        }
        for (i = 0; i < DSP_RC2D_SETTING_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_DECODER_INFO_BASE_DEC2 + i, 0);
        }
        // Initial ctrl page 5 for second decoder
        for (i = 0; i < DSP_RC2D_CTRL5_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_CTRL5_BASE + i, 0);
        }
    }
}



UINT32 dwDspCalOfst (UINT32 dwOfst)
{
    return (u4GetDspImgAddr () + (dwOfst * 4));
}

/*
 * dwGetDspAckPTS
 * if valid is 1 -> copy new
 * else -> use old
*/
UINT32 dwGetDspAckPTS (BOOL fgFirstDecoder)
{
    UINT32 dwPtsValid;
    UINT32 dwPtsHigh;
    UINT32 dwPtsLow;

    if (fgFirstDecoder)
    {                           // first decoder
        HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        dwPtsValid = wReadDspWORD (ADDR_D2RC_DRAM_STC_VALID);
        dwPtsHigh = wReadDspWORD (ADDR_D2RC_DRAM_STC_HIGH);
        dwPtsLow = wReadDspWORD (ADDR_D2RC_DRAM_STC_LOW);
        if (dwPtsValid != 0)
        {
            _dwDspAckPTS = ((dwPtsHigh & 0xFFFF) << 16) | (dwPtsLow & 0xFFFF);
        }
        return _dwDspAckPTS;
    }
    // second decoder
    dwPtsValid = wReadDspWORD (ADDR_D2RC_DRAM_STC_VALID_DEC2);
    dwPtsHigh = wReadDspWORD (ADDR_D2RC_DRAM_STC_HIGH_DEC2);
    dwPtsLow = wReadDspWORD (ADDR_D2RC_DRAM_STC_LOW_DEC2);
    if (dwPtsValid != 0)
    {
        _dwDspAckPTSDec2 = ((dwPtsHigh & 0xFFFF) << 16) | (dwPtsLow & 0xFFFF);
    }
    return _dwDspAckPTSDec2;
}

/***************************************************************************
     Function : fgCheckReinitLock
  Description : Check if dsp is doing fast reinit
    Parameter :
       Author : sammy
    Return    :
***************************************************************************/
BOOL fgCheckReinitLock (void)
{
    UINT32 dData;

    HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    dData = wReadDspWORD (ADDR_D2RC_DRAM_LOCK);
    return ((dData & REINIT_LOCK) > 0);
}

/***************************************************************************
     Function : vDspDecDes
  Description : Dsp decoder destructor ( after receive stop )
                Risc side information reset routine
                NOTE: Do not send command to DSP because dsp may be power off
                      right after
    Parameter :
       Author : sammy
    Return    :
***************************************************************************/
void vDspDecDes (void)
{
    vWriteDspShmBYTE (W_SPEED, 0x0);
}

/***************************************************************************
     Function : vDspDecDes
  Description : Dsp decoder destructor ( after receive stop )
                Risc side information reset routine
                NOTE: Do not send command to DSP because dsp may be power off
                      right after
    Parameter :
       Author : sammy
    Return    :
***************************************************************************/
void vDspDec2Des (void)
{
    // vWriteDspShm(W_SPEED_DEC2,0x0);
}

/***************************************************************************
     Function : fgIsOverADCFreq
       Author : sammy
  Description : return if input freq is adc capable freq
    Parameter :
    Return    :
***************************************************************************/
BOOL fgIsOverADCFreq (UINT32 u4Freq, BOOL fgFirstDecoder)
{
    UINT32 *pdwDspFreq = &_dwDspFreq;

    if (!fgFirstDecoder)
    {
        pdwDspFreq = &_dwDspFreqDec2;
    }
    if ((*pdwDspFreq == SFREQ_88K) ||
        (*pdwDspFreq == SFREQ_176K) ||
        (*pdwDspFreq == SFREQ_96K) || (*pdwDspFreq == SFREQ_192K))
    {
        return TRUE;
    }

    UNUSED(u4Freq);
    return FALSE;
}

/***************************************************************************
     Function : vTrnsltSpkCfg
       Author : sammy
  Description : Translate speaker config to internal speaker config structure
    Parameter : u2SpkCfg : Dsp speaker config
                rSpkCfg : translated speaker config
    Return    : 
***************************************************************************/
void vTrnsltSpkCfg (UINT16 u2SpkCfg, SPEAKER_SETTING_T * ptSpkCfg)
{
    UINT8 nFSpeakers = 0;
    UINT8 nSSpeakers = 0;
    UINT8 fgSWExist;
    if (ptSpkCfg != NULL)
    {
        if (u2SpkCfg != ptSpkCfg->wDspSpkCfg)
        {
            switch (u2SpkCfg & 0x7)
            {
            case 0:                // 2/0 (LT/RT)
                nFSpeakers = 2;
                nSSpeakers = 0;
                break;
            case 1:                // 1/0
                nFSpeakers = 1;
                nSSpeakers = 0;
                break;
            case 2:                // 2/0
                nFSpeakers = 2;
                nSSpeakers = 0;
                break;
            case 3:                // 3/0
                nFSpeakers = 3;
                nSSpeakers = 0;
                break;
            case 4:                // 2/1
                nFSpeakers = 2;
                nSSpeakers = 1;
                break;
            case 5:                // 3/1
                nFSpeakers = 3;
                nSSpeakers = 1;
                break;
            case 6:                // 2/2
                nFSpeakers = 2;
                nSSpeakers = 2;
                break;
            case 7:                // 3/2
                nFSpeakers = 3;
                nSSpeakers = 2;
                break;
            default:
                break;
            }
            fgSWExist = (u2SpkCfg & 0x20) ? TRUE : FALSE;
            if (u2SpkCfg & 0x8)
            {
                nSSpeakers++;
            }
            if (u2SpkCfg & 0x10)
            {
                nSSpeakers++;
            }
            ptSpkCfg->bFrntSpkrNm = nFSpeakers;
            ptSpkCfg->bSrndSpkrNm = nSSpeakers;
            ptSpkCfg->fgIsSWExst = fgSWExist;
            ptSpkCfg->bChNm = nFSpeakers + nSSpeakers + fgSWExist;
            ptSpkCfg->wDspSpkCfg = u2SpkCfg;
        }
    }
}
/***************************************************************************
     Function : fgGetDspErrorLogs
  Description : Get DSP related error logs
    Parameter : struct address
    Return    : value in struct
***************************************************************************/
/*
    // Error log related 
    // common logs
#define ADDR_D2RC_COMMON_LOG_FLAG                          (ADDR_D2RC_COMMON_LOG)
#define ADDR_D2RC_INPUT_UNDERRUN                           (ADDR_D2RC_COMMON_LOG+1)
#define ADDR_D2RC_INPUT_UNDERRUN_DEC2                      (ADDR_D2RC_COMMON_LOG+2)
#define ADDR_D2RC_INPUT_UNDERRUN_MIXSOUND                  (ADDR_D2RC_COMMON_LOG+3)
#define ADDR_D2RC_OUTPUT_UNDERRUN                          (ADDR_D2RC_COMMON_LOG+4)
#define ADDR_D2RC_OUTPUT_UNDERRUN_DEC2                     (ADDR_D2RC_COMMON_LOG+5)
#define ADDR_D2RC_OUTPUT_UNDERRUN_MIXSOUND                 (ADDR_D2RC_COMMON_LOG+6)
    // primary decoder
#define ADDR_D2RC_DECODER_LOG_FLAG                         (ADDR_D2RC_DECODER_LOG)
#define ADDR_D2RC_FRAME_REPEAT_COUNT                       (ADDR_D2RC_DECODER_LOG+1)
#define ADDR_D2RC_FRAME_DROP_COUNT                         (ADDR_D2RC_DECODER_LOG+2)
#define ADDR_D2RC_TOTAL_ERROR_COUNT                        (ADDR_D2RC_DECODER_LOG+3)
#define ADDR_D2RC_TOTAL_FRAME_COUNT                        (ADDR_D2RC_DECODER_LOG+4)
    // detail logs total 16 DWORD
#define ADDR_D2RC_DECODER_DETAIL_LOG                       (ADDR_D2RC_DECODER_LOG+8)
    // secondary decoder
#define ADDR_D2RC_DECODER_LOG_FLAG_DEC2                    (ADD2_D2RC_DECODER_LOG_DEC2)
#define ADDR_D2RC_FRAME_REPEAT_COUNT_DEC2                  (ADD2_D2RC_DECODER_LOG_DEC2+1)
#define ADDR_D2RC_FRAME_DROP_COUNT_DEC2                    (ADD2_D2RC_DECODER_LOG_DEC2+2)
#define ADDR_D2RC_TOTAL_ERROR_COUNT_DEC2                   (ADD2_D2RC_DECODER_LOG_DEC2+3)
#define ADDR_D2RC_TOTAL_FRAME_COUNT_DEC2                   (ADD2_D2RC_DECODER_LOG_DEC2+4)
    // detail logs total 16 DWORD
#define ADDR_D2RC_DECODER_DETAIL_LOG_DEC2                  (ADD2_D2RC_DECODER_LOG_DEC2+8)

*/

BOOL fgTriggerDspErrorLogs(void)
{
    UINT32 u4TryCnt = 0;
    CRIT_STATE_T rCritState;
    
    // Issue interrupt to DSP
    WriteDspCommDram(ADDR_D2RC_COMMON_LOG_FLAG, 0x0);
    while (TRUE)
    {
        u4TryCnt++;
        rCritState = x_crit_start();
        if (!fgDspBusy ())
        {
            vSendDspLongInt (INT_RC2D_OR, ((LOG_DRAM_REFRESH) << 8),SRAM_SYSTEM_CTRL);
            x_crit_end(rCritState);
            return TRUE;
        }
        else
        {
            x_crit_end(rCritState);
            if (u4TryCnt == 10000)
            {
                return FALSE;
            }
        }
    }
}

BOOL fgGetDspErrorLogs(ADSP_ERROR_LOG_T* ptADSPErrLog)
{
    UINT8  uIndex;

    if(ptADSPErrLog != NULL)
    {
        HalFlushInvalidateDCache(); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        if (dReadDspCommDram(ADDR_D2RC_COMMON_LOG_FLAG) == 0) // data not ready
        {
            return FALSE;
        }
        else // (dReadDspCommDram(ADDR_D2RC_COMMON_LOG_FLAG) != 0) //data ready
        {
            // Get latest error log
            // fill error log structure
            ptADSPErrLog->dwInputUnderRun = dReadDspCommDram(ADDR_D2RC_INPUT_UNDERRUN);
            ptADSPErrLog->dwInputUnderRunDec2 = dReadDspCommDram(ADDR_D2RC_INPUT_UNDERRUN_DEC2);
#ifndef CC_MT5391_AUD_3_DECODER        
//#if 1
            ptADSPErrLog->dwInputUnderRunMixSound = dReadDspCommDram(ADDR_D2RC_INPUT_UNDERRUN_MIXSOUND);
#else
            ptADSPErrLog->dwInputUnderRunDec3 = dReadDspCommDram(ADDR_D2RC_INPUT_UNDERRUN_DEC3);
#endif
            ptADSPErrLog->dwOutputUnderRun = dReadDspCommDram(ADDR_D2RC_OUTPUT_UNDERRUN);
            ptADSPErrLog->dwOutputUnderRunDec2 = dReadDspCommDram(ADDR_D2RC_OUTPUT_UNDERRUN_DEC2);
#ifndef CC_MT5391_AUD_3_DECODER   
//#if 1
            ptADSPErrLog->dwOutputUnderRunMixSound = dReadDspCommDram(ADDR_D2RC_OUTPUT_UNDERRUN_MIXSOUND);   
#else
            ptADSPErrLog->dwOutputUnderRunDec3 = dReadDspCommDram(ADDR_D2RC_OUTPUT_UNDERRUN_DEC3); 
#endif
            // first decoder's log
            ptADSPErrLog->dwFrameRepeatCount = dReadDspCommDram(ADDR_D2RC_FRAME_REPEAT_COUNT);
            ptADSPErrLog->dwFrameDropCount = dReadDspCommDram(ADDR_D2RC_FRAME_DROP_COUNT);
            ptADSPErrLog->dwTotalErrorCount = dReadDspCommDram(ADDR_D2RC_TOTAL_ERROR_COUNT);
            ptADSPErrLog->dwTotalFrameCount = dReadDspCommDram(ADDR_D2RC_TOTAL_FRAME_COUNT);
            // first decoder's detail log
            for(uIndex=0;uIndex<16;uIndex++)
            {
                ptADSPErrLog->dwDetailLogs[uIndex]=dReadDspCommDram(ADDR_D2RC_DECODER_DETAIL_LOG+uIndex);
            }
            // second decoder's log
            ptADSPErrLog->dwFrameRepeatCountDec2 = dReadDspCommDram(ADDR_D2RC_FRAME_REPEAT_COUNT_DEC2);
            ptADSPErrLog->dwFrameDropCountDec2 = dReadDspCommDram(ADDR_D2RC_FRAME_DROP_COUNT_DEC2);
            ptADSPErrLog->dwTotalErrorCountDec2 = dReadDspCommDram(ADDR_D2RC_TOTAL_ERROR_COUNT_DEC2);
            ptADSPErrLog->dwTotalFrameCountDec2 = dReadDspCommDram(ADDR_D2RC_TOTAL_FRAME_COUNT_DEC2);
            // first decoder's detail log
            for(uIndex=0;uIndex<16;uIndex++)
            {
                ptADSPErrLog->dwDetailLogsDec2[uIndex]=dReadDspCommDram(ADDR_D2RC_DECODER_DETAIL_LOG_DEC2+uIndex);
            }          
        }
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

#ifdef ADSP_CHECK_RAM_CODE
//-----------------------------------------------------------------------------
/** fgCheckDspData
 *   Check DSP RAM code and table
 *
 *  @param  void
 *  @retval TRUE(pass), FALSE(fail)
 */
//-----------------------------------------------------------------------------
BOOL fgCheckDspData(void)
{
    UINT32 u4DspRamCodeStatus = 0;
    UINT32 u4DspTableStatus = 0;

    vCheckDspRamCode(&u4DspRamCodeStatus, &u4DspTableStatus);

    if ((u4DspRamCodeStatus == 0) && (u4DspTableStatus == 0))
    {   
        return TRUE;
    }
    else
    {   
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/** vCheckDspRamCode
 *   Check DSP RAM Code
 *
 *  @param  *pu4DspRamCodeStatus: RAM code status
  *  @param  *pu4DspTableStatus: table status
 *  @retval void
 */
//-----------------------------------------------------------------------------
void vCheckDspRamCode(UINT32 *pu4DspRamCodeStatus, UINT32 *pu4DspTableStatus)
{
    UINT32 u4FlashStart;                    // Flash start address (flash header)
    UINT32 u4FlashCodIndx;                  // Audio codec index	
    UINT32 u4FlashOfst;                     // Flash offset in DWORD
    UINT32 u4Adr;                           // Temp byte address
    UINT32 u4Size;
    UINT8 *puFlashRead;                     // Read pointer to flash file
    UINT8 *puDramRead;                      // Read pointer to DRAM
    BOOL fgMatchFlag;                       // Match flag	

    UINT32 u4AudSaDec1[] =
    {
        DSPF_AC3_DEC1_SA,                   // AUD_AC3_DEC1
        DSPF_MP2_DEC1_SA,                   // AUD_MPEG_DEC1
        DSPF_AAC_DEC1_SA,                   // AUD_AAC_DEC1
        DSPF_PCM_DEC1_SA,                   // AUD_PCM_DEC1
        DSPF_MP3_DEC1_SA,                   // AUD_MP3_DEC1
        DSPF_WMA_DEC1_SA,                   // AUD_WMA_DEC1
        DSPF_VORBIS_DEC1_SA,                // AUD_VORBIS_DEC1
        DSPF_PNOISE_SA,                     // AUD_PINK_DEC1
#ifdef DSP_SUPPORT_NPTV    
        DSPF_NTSC_DEC1_SA,	                // AUD_NTSC_DEC1
        DSPF_PAL_DEC1_SA,                 // AUD_PAL_DEC1
        DSPF_MINER_DEC1_SA,               // AUD_MINER_DEC1
        DSPF_A2_DEC1_SA,                  // AUD_A2DEC_DEC1
        DSPF_DETECTOR_DEC1_SA,            // AUD_DETECTOR_DEC1
        DSPF_FMFM_DEC1_SA,                // AUD_FMFM_DEC1
        //DSPF_NICAM_DEC1_SA,               // AUD_NICAM_DEC1
        //DSPF_TTXACC_DEC1_SA,              // AUD_TTXACC_DEC1		
        DSPF_LPCM_DEC1_SA,                  // DSPF_LPCM_DEC1_SA
#endif
    };

    UINT32 u4AudSaDec2[] =
    {
        DSPF_AC3_DEC2_SA,                   // AUD_AC3_DEC2
        DSPF_MP2_DEC2_SA,                   // AUD_MPEG_DEC2
        DSPF_AAC_DEC2_SA,                   // AUD_AAC_DEC2
        DSPF_PCM_DEC2_SA,                   // AUD_PCM_DEC2
#ifdef DSP_SUPPORT_NPTV    
        DSPF_NTSC_DEC2_SA,                  // AUD_NTSC_DEC2
        DSPF_PAL_DEC2_SA,                   // AUD_PAL_DEC2
        DSPF_MINER_DEC2_SA,                 // AUD_MINER_DEC2
        DSPF_A2_DEC2_SA,                    // AUD_A2DEC_DEC2
        DSPF_DETECTOR_DEC2_SA,              // AUD_DETECTOR_DEC2
        DSPF_FMFM_DEC2_SA,                  // AUD_FMFM_DEC2
#endif
#ifdef ADSP_JPEG_DEC2                       ///[Joel]
        DSPF_JPEG_DEC2_SA,                  //0x0A, AUD_JPEG_DEC2
#endif
    };

#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    UINT32 u4AudSaDec2[] = {
        DSPF_MP2_DEC3_SA,       // AUD_MPEG_DEC3
        DSPF_PCM_DEC3_SA,       // AUD_PCM_DEC3 
        DSPF_AAC_DEC3_SA,       // AUD_PCM_DEC3 
        DSPF_AC3_DEC3_SA,       // AUD_PCM_DEC3 
    };        
#endif    

    u4FlashStart = u4GetDspImgAddr ();          // Get Flash header address
    
    //=============== Compare ROM Section ===============
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_ROM_CODE_SZ << 2));
    /*lint -e{506}*/
    /*lint -e{774}*/
    if (_fgRamBoot && (u4Size > 0))
    {   // RAM boot
        // Get section's start address and size in Flash
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_ROM_CODE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;           // Convert to byte pointer

        // Get DRAM address
        u4Adr = dwGetDspIcacheAdr(TYPE_ROM_ICACHE) * 4; // Byte address
        puDramRead = (UINT8*)(u4Adr);  // Access DRAM directly
        HalFlushInvalidateDCache();
        
        // Compare
        fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
        if (!fgMatchFlag)
        {
            *pu4DspRamCodeStatus |= DSP_NC_RAM_ROM;
        }
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: ROM boot, no RAM code exist\n");
    }

    //=============== Compare POST Section ===============
    // Compare Post Code
#ifdef DOWNMIX_SUPPORT
    // Get section's start address and size in Flash
#ifdef DSP_SUPPORT_SRSWOW  
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_SRS_WOW_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_SRS_WOW_SZ << 2));
#elif defined(DSP_SUPPORT_SRSTSXT)
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_SRS_TSXT_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_SRS_TSXT_SZ << 2));
#elif defined(DSP_SUPPORT_SRSTSHD) //sunman_tshd
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_SRS_TSHD_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_SRS_TSHD_SZ << 2));    
#else
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_VSURR_POST_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_VSURR_POST_SZ << 2));
#endif  // DSP_SUPPORT_SRSWOW

    puFlashRead = (UINT8*) u4Adr;
	
    // Get DRAM address
    u4Adr = (dwGetDspIcacheAdr(TYPE_POST_ICACHE) + VIRTUAL_SURROUND_PRAM_ADR) * 4;
    puDramRead = (UINT8*)(u4Adr);
    HalFlushInvalidateDCache();
    
    // Compare
    if (u4Size > 0)
    {
        fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
        if (!fgMatchFlag)
        {
            *pu4DspRamCodeStatus |= DSP_NC_RAM_POST;
        }
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: No Post RAM code exist\n");
    }
#endif  // DOWNMIX_SUPPORT

#ifdef CC_AUD_BBE_SUPPORT
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_BBE_VIVA4_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_BBE_VIVA4_SZ << 2));
    
    puFlashRead = (UINT8*) u4Adr;
	
    // Get DRAM address
    u4Adr = (dwGetDspIcacheAdr(TYPE_POST_ICACHE) + SPEAKER_ENHANCEMENT_PRAM_ADR) * 4;
    puDramRead = (UINT8*)(u4Adr);
    HalFlushInvalidateDCache();
    
    // Compare
    if (u4Size > 0)
    {
        fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
        if (!fgMatchFlag)
        {
            *pu4DspRamCodeStatus |= DSP_NC_RAM_POST;
        }
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: No Post RAM code exist\n");
    }
#endif // CC_AUD_BBE_SUPPORT

    // Compare Post Vector
    // Get section's start address and size in Flash
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_POST_VECT_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    puFlashRead = (UINT8*) u4Adr;
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_POST_VECT_SZ << 2));

    // Get DRAM address
    u4Adr = dwGetDspIcacheAdr(TYPE_POST_ICACHE) * 4;
    puDramRead = (UINT8*)(u4Adr); 
    HalFlushInvalidateDCache();

    // Compare
    if (u4Size > 0)
    {
        fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
        if (!fgMatchFlag)
        {
            *pu4DspRamCodeStatus |= DSP_NC_RAM_POST;
        }
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: No Post Vectort RAM code exist\n");
    }

    //=============== Compare COMM Section ===============
    // Get section's start address and size in Flash
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_COMM_CODE_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    puFlashRead = (UINT8*) u4Adr;
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_COMM_CODE_SZ << 2));

    // Get DRAM address
    u4Adr = dwGetDspIcacheAdr(TYPE_COMM_ICACHE) * 4;
    puDramRead = (UINT8*)(u4Adr);
    HalFlushInvalidateDCache();

    // Compare
    if (u4Size > 0)
    {
        fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
        if (!fgMatchFlag)
        {
            *pu4DspRamCodeStatus |= DSP_NC_RAM_COMM;
        }
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: No Comm RAM code exist\n");
    }

#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    //=============== Compare DEC3 Section ===============
    if (((_dwDspRamCodeTypeDec3 - TYPE_AUD_MIX) <= DECODER3_MAX_INDEX))
    {
        // Get section's start address and size in Flash
        u4FlashCodIndx = u4AudSaDec3[_dwDspRamCodeTypeDec3 - TYPE_AUD_MIX];
        u4FlashOfst = u4GetFlashData (u4FlashStart + (u4FlashCodIndx << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + ((u4FlashCodIndx + 1) << 2));

        // Get DRAM address
        u4Adr = dwGetDspIcacheAdr(TYPE_DEC3_ICACHE) * 4;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();

        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
            if (!fgMatchFlag)
            {
                *pu4DspRamCodeStatus |= DSP_NC_RAM_DEC3;
            }

            /*lint -e{506}*/
            /*lint -e{774}*/
            // Check table
            if (_fgDecTblFromFlash)
            {
                *pu4DspTableStatus |= u4CheckDspDramTable(_dwDspRamCodeTypeDec3);
            }
        }
        else
        {
            LOG(7, "[DSP] vCheckDspRamCode: No Dec3 RAM code exist\n");
        }
  
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: No Dec3 RAM code exist\n");
    }
#endif

    //=============== Compare DEC2 Section ===============
    if (((_dwDspRamCodeTypeDec2 - TYPE_AUD_DEC2) <= DECODER2_MAX_INDEX))
    {
        // Get section's start address and size in Flash
        u4FlashCodIndx = u4AudSaDec2[_dwDspRamCodeTypeDec2 - TYPE_AUD_DEC2];
        u4FlashOfst = u4GetFlashData (u4FlashStart + (u4FlashCodIndx << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + ((u4FlashCodIndx + 1) << 2));

        // Get DRAM address
        u4Adr = dwGetDspIcacheAdr(TYPE_DEC2_ICACHE) * 4;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();

        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
            if (!fgMatchFlag)
            {
                *pu4DspRamCodeStatus |= DSP_NC_RAM_DEC2;
            }

            /*lint -e{506}*/
            /*lint -e{774}*/
            // Check table
            if (_fgDecTblFromFlash)
            {
                *pu4DspTableStatus |= u4CheckDspDramTable(_dwDspRamCodeTypeDec2);
            }
        }
        else
        {
            LOG(7, "[DSP] vCheckDspRamCode: No Dec2 RAM code exist\n");
        }
  
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: No Dec2 RAM code exist\n");
    }

    //=============== Compare AININT Section ===============
    // Get section's start address and size in Flash
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_AIN_INTRAM_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    puFlashRead = (UINT8*) u4Adr;
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_AIN_INTRAM_SZ << 2));

    // Get DRAM address
    u4Adr = dwGetDspIcacheAdr(TYPE_AIN_ICACHE) * 4;
    //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
    puDramRead = (UINT8*)(u4Adr);
    HalFlushInvalidateDCache();

    // Compare
    if (u4Size > 0)
    {
        fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
        if (!fgMatchFlag)
        {
            *pu4DspRamCodeStatus |= DSP_NC_RAM_AIN;
        }
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: No AinInt RAM code exist\n");
    }

    //=============== Compare DEC1 Section ===============
    if (((_dwDspRamCodeType - TYPE_AUD_DEC1) <= DECODER1_MAX_INDEX))
    {
        // Get section's start address and size in Flash
        u4FlashCodIndx = u4AudSaDec1[_dwDspRamCodeType - TYPE_AUD_DEC1];
        u4FlashOfst = u4GetFlashData (u4FlashStart + (u4FlashCodIndx << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + ((u4FlashCodIndx + 1) << 2));        

        // Get DRAM address
        u4Adr = dwGetDspIcacheAdr(TYPE_DEC1_ICACHE) * 4;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();

        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, (u4Size - 1) * 4);
            if (!fgMatchFlag)
            {
                *pu4DspRamCodeStatus |= DSP_NC_RAM_DEC1;
            }

            /*lint -e{506}*/
            /*lint -e{774}*/
            // Check table
            if (_fgDecTblFromFlash)
            {
                *pu4DspTableStatus |= u4CheckDspDramTable(_dwDspRamCodeType);
            }
        }
        else
        {
            LOG(7, "[DSP] vCheckDspRamCode: No Dec1 RAM code exist\n");
        }
    
    }
    else
    {
        LOG(7, "[DSP] vCheckDspRamCode: No Dec1 RAM code exist\n");
    }    
}

//-----------------------------------------------------------------------------
/** u4CheckDspDramTable
 *   Check DSP Table
 *
 *  @param  u4Type: type
 *  @retval Status bit
 */
//-----------------------------------------------------------------------------
UINT32 u4CheckDspDramTable(UINT32 u4Type)
{
    UINT32 u4FlashStart;                    // Flash start address (flash header)
    UINT32 u4FlashOfst;                     // Flash offset in DWORD
    UINT32 u4Adr;                           // Temp byte address
    UINT32 u4Size;
    UINT8 *puFlashRead;                     // Read pointer to flash file
    UINT8 *puDramRead;                      // Read pointer to DRAM
    UINT32 u4TabAddr = DRAM_DECODING_TABLE_ADDR * 4;    // Byte address
    UINT32 u4DspBuf;
    UINT32 u4DspCmptBuf;
    BOOL fgMatchFlag = TRUE;                // Match flag
    UINT32 u4RetStatus = 0;

    u4FlashStart = u4GetDspImgAddr ();

    // Get DSP buffer start address (Normal and Compact)
    u4DspBuf = dwGetDSPNSADR (u4Type);
    u4DspCmptBuf = dwGetDSPCSADR (u4Type);

    switch (u4Type)
    {
    case AUD_AAC_DEC1:
    case AUD_AAC_DEC2:
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    case AUD_AAC_DEC3:
#endif
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_AAC_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_AAC_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + u4TabAddr;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                if (u4Type == AUD_AAC_DEC1)
                {
                    u4RetStatus = DSP_NC_TBL_AAC_DEC1;
                }
                else
                {
                    u4RetStatus = DSP_NC_TBL_AAC_DEC2;
                }
            }							
        }
        break;

    case AUD_AC3_DEC1:
    case AUD_AC3_DEC2:
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    case AUD_AC3_DEC3:
#endif
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_AC3_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_AC3_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspCmptBuf + u4TabAddr;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                if (u4Type == AUD_AC3_DEC1)
                {
                    u4RetStatus = DSP_NC_TBL_AC3_DEC1;
                }
                else
                {
                    u4RetStatus = DSP_NC_TBL_AC3_DEC2;
                }			
            }										
        }
        break;

    case AUD_MPEG_DEC1:
    case AUD_MPEG_DEC2:
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    case AUD_MPEG_DEC3:
#endif
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_MP2_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_MP2_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + u4TabAddr;
        //puDramRead = (UINT8*) NONCACHE(u4Adr);  //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                if (u4Type == AUD_MPEG_DEC1)
                {
                    u4RetStatus = DSP_NC_TBL_MPEG_DEC1;
                }
                else
                {
                    u4RetStatus = DSP_NC_TBL_MPEG_DEC2;
                }						
            }										
        }
        break;

    case AUD_MP3_DEC1:
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_MP3_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_MP3_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + u4TabAddr;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                u4RetStatus = DSP_NC_TBL_MP3_DEC1;
            }			                
        }
        break;

    case AUD_VORBIS_DEC1:
        // Compare normal table
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_VORBIS_N_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_VORBIS_N_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + u4TabAddr;
        //puDramRead = (UINT8*) NONCACHE(u4Adr);  //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                u4RetStatus = DSP_NC_TBL_VORBIS_N_DEC1;
            }			                
        }

        // Compare compact table
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_VORBIS_C_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_VORBIS_C_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + (0x8000 * 4);
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                u4RetStatus = DSP_NC_TBL_VORBIS_C_DEC1;
            }							
        }
        break;

#ifdef DATA_DISC_WMA_SUPPORT
    case AUD_WMA_DEC1:
        // Compare normal table
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_WMA_N_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_WMA_N_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + WMA_TABLE_NORMAL * 4;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                u4RetStatus = DSP_NC_TBL_WMA_N_DEC1;
            }									
        }

        // Compare compact table
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_WMA_C_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_WMA_C_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + WMA_TABLE_CMPT * 4;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                u4RetStatus = DSP_NC_TBL_WMA_C_DEC1;
            }								
        }
        break;
#endif

#ifdef DSP_SUPPORT_NPTV
        case AUD_MTS_DEC1:
        case AUD_MTS_DEC2:
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_NTSCTBL_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_NTSCTBL_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + u4TabAddr;
        //puDramRead = (UINT8*) NONCACHE(u4Adr); //Andrew Wen : DTV00005752 Remove usage of NONCACHE macro
        puDramRead = (UINT8*)(u4Adr);
        HalFlushInvalidateDCache();
        // Compare
        if (u4Size > 0)
        {
            fgMatchFlag = fgDspDataComp(puFlashRead, puDramRead, u4Size*4);
            if (!fgMatchFlag)
            {
                if (u4Type == AUD_MTS_DEC1)
                {
                    u4RetStatus = DSP_NC_TBL_MTS_DEC1;
                }
                else
                {
                    u4RetStatus = DSP_NC_TBL_MTS_DEC2;
                }						
            }										
        }
        break;
#endif
    default:
        LOG(7, "[DSP] Error: vCheckDspDramTable: invalid table type\n");
        break;
    }

    return u4RetStatus;
}

//-----------------------------------------------------------------------------
/** fgDspDataComp
 *   Compare Flash and DSP Data
 *
 *  @param  *puAdr1: address 1
 *  @param  *puAdr2: address 2
 *  @param  u4Size: size
 *  @retval TRUE(match), FALSE(not match)
 */
//-----------------------------------------------------------------------------
/*lint -e{818}*/
BOOL fgDspDataComp(UINT8 *puAdr1, UINT8 *puAdr2, UINT32 u4Size)
{
    BOOL fgMathFlag = TRUE;                 // Match flag
    UINT32 u4Index;                         // Loop index

    if ((puAdr1 == NULL) || (puAdr2 == NULL))
    {
        LOG(7, "[DSP] Error: fgDspDataComp: invalid pointer\n");
        return FALSE;
    }

    u4Index = 0;
    while ((u4Index < u4Size) && (fgMathFlag != FALSE))
    {
        if (puAdr1[u4Index] != puAdr2[u4Index])
        {
            fgMathFlag = FALSE;
        }
        u4Index++;
    }

    return (fgMathFlag);
}
#endif

//-----------------------------------------------------------------------------
/** u4ReadD2RInfo_AputBank
 *   
 *
 *  @param  
 *  @retval 
 */
//-----------------------------------------------------------------------------
UINT32 u4ReadD2RInfo_AputBank(UINT8 u1DecId)
{    
    HalFlushInvalidateDCache();
    return (UINT32)dReadDspCommDram(ADDR_D2RC_RISC_INFO_APUT_BANK_DEC1 + u1DecId);
}

//-----------------------------------------------------------------------------
/** u4ReadD2RInfo_InputUnderRun
 *   
 *
 *  @param  
 *  @retval 
 */
//-----------------------------------------------------------------------------
UINT32 u4ReadD2RInfo_InputUnderRun(UINT8 u1DecId)   //  -- DSP Force --
{
    HalFlushInvalidateDCache();
    return (UINT32)dReadDspCommDram(ADDR_D2RC_INPUT_UNDERRUN + u1DecId);
}

//-----------------------------------------------------------------------------
/** u4ReadD2RInfo_ReadPtr
 *   
 *
 *  @param  
 *  @retval 
 */
//-----------------------------------------------------------------------------
UINT32 u4ReadD2RInfo_ReadPtr(UINT8 u1DecId) //  -- DSP Force --
{
    HalFlushInvalidateDCache();
    if (u1DecId == AUD_DEC_MAIN)
    {
        return (UINT32)dReadDspCommDram(ADDR_D2RC_RISC_INFO_REG_BUF_PNT);
    }
    else
    {
        return (UINT32)dReadDspCommDram(ADDR_D2RC_RISC_INFO_REG_BUF_PNT_DEC2);        
    }
}

