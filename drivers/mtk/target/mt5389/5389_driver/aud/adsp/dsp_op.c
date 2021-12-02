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
 * $Author: kai.wang $
 * $Date: 2012/09/21 $
 * $RCSfile: dsp_op.c,v $
 * $Revision: #5 $
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
#include "x_chip_id.h"

LINT_EXT_HEADER_END

#ifdef DATA_DISC_WMA_SUPPORT
//WMA Table
#define WMA_TABLE_NORMAL                         0x6CD6
#define WMA_TABLE_CMPT                           0x476A
#endif

static AV_SYNC_MODE_T _aeAvSyncMode[2] = {AV_SYNC_FREE_RUN, AV_SYNC_FREE_RUN};

#ifdef CC_AUD_DATASRAM
static UINT32 _u4DsramGrp[2] = {0, 0};
static BOOL _fgCommFlag = FALSE;
#endif

static void vResetDecInfo (UINT8 u1DecId);
void vTrnsltSpkCfg (UINT16 u2SpkCfg, SPEAKER_SETTING_T * ptSpkCfg);
UINT32 dwDspCalOfst (UINT32 dwOfst);

#ifdef ADSP_CHECK_RAM_CODE
void vCheckDspRamCode(UINT32 *pu4DspRamCodeStatus, UINT32 *pu4DspTableStatus);
UINT32 u4CheckDspDramTable(UINT32 u4Type);
#endif
BOOL fgDspDataComp(UINT8 *pu1Adr1, UINT8 *pu1Adr2, UINT32 u4Size);
void vDspSetSyncMode(UCHAR ucDecId, AV_SYNC_MODE_T eMode);
AV_SYNC_MODE_T eDspGetSyncMode(UCHAR ucDecId);
BOOL fgCheckDspCmd(UINT32 u4DspCmd);
void vDspSetFifoReadPtr(UCHAR ucDecId, UINT32 u4ReadPtr);
BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address);

//extern function
extern void vDSPLockReadSramSema(void);
extern void vDSPUnlockReadSramSema(void);


/***************************************************************************
     Function : vLoadDspDRAMTable(UINT32 u4Type)
  Description : Download Decoders' Table to DRAM
                Set default value at here too.
    Parameter : u4Type
       Author : sammy,PC
    Return    :
***************************************************************************/
void vLoadDspDRAMTable (UINT32 u4Type)
{
    UINT32 dwOfst;
    UINT32 dwSz;
    //UINT32 dwDspData;
    //UINT16 u2DspData;
    BOOL fgGetBitsNotHold = FALSE;

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
    case AUD_AAC_DEC1:
    case AUD_AAC_DEC2:
#ifdef CC_MT5391_AUD_3_DECODER
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
#ifdef CC_AUD_SUPPORT_MS10
        //DDCO
        dwDspCmptBuf = dwGetDSPCSADR (AUD_DDCO);
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + DDCO_TABLE_ADR * 4, dwOfst, dwSz * 4);
        // write table3.bin
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TB3_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TB3_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + DDCO_TABLE3_ADR * 4, dwOfst, dwSz * 4);
         // write table4.bin
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TB4_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TB4_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + DDCO_TABLE4_ADR * 4, dwOfst, dwSz * 4);
#endif
        break;
    case AUD_AC3_DEC1:
    case AUD_AC3_DEC2:
#ifdef CC_MT5391_AUD_3_DECODER
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
    case AUD_MPEG_DEC3:
#endif
        // move MP2 Table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_MP2_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_MP2_TABLE_SZ << 2));

        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_MP3_DEC1:
    case AUD_MP3_DEC2:
        // move MP3 Table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_MP3_TABLE_SA << 2));

        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_MP3_TABLE_SZ << 2));

        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
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
    case AUD_LPCM_DEC2:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_LPCM_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_LPCM_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;

    case AUD_DTS_DEC1:
    case AUD_DTS_DEC2:
        //DTS Huffman Table 08400~08D00h, 0C00*3/4=0900h
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DTS_HUFF_TABLE_SA << 2));  //get array start address
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DTS_HUFF_TABLE_SZ << 2));
        // 1st: Dsp DRAM address
        // 2nd: Table flash rom offset
        // 3rd: Table flash rom size
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);

        //DTS ADPCM&VQ Table 0A000~0E000h
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DTS_ADPCM_VQ_TABLE_SA << 2));  //from byte address to get offset data of _pbD[]
        dwOfst = dwDspCalOfst (dwOfst);                                             //Calculate absolute dram byte alignment address.
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DTS_ADPCM_VQ_TABLE_SZ << 2));    //from byte address to get size data of DSPF_DTS_ADPCM_VQ_TABLE_SA in _pbD[].
        vFlash2DramDma (dwDspBuf + DTS_ADPCM_VQ_TBL_ADR*4, dwOfst, dwSz * 4);

        //DTS DataROM Table 0E400~0E800h,
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DTS_ROM_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DTS_ROM_TABLE_SZ << 2));
        vFlash2DramDma (dwDspBuf + DTS_DTS_ROM_TBL_ADR*4, dwOfst, dwSz * 4);

        //DTS BC Table 0E800~0EC00h,
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DTS_BC_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DTS_BC_SZ << 2));
        vFlash2DramDma (dwDspBuf + DTS_DTS_BC_TBL_ADR*4, dwOfst, dwSz * 4);
        break;
	case AUD_DTSENC:   //zy_dts load to dec3 dram page
            /*   //Disable DTS for code shrink
	    #if 1
                dwDspBuf = _u4DspDramBuf[2]+(0x400<<2);
                dwOfst =  (INT32)(DTSENC_TABLE);
                dwSz = 8640;
                vFlash2DramDma(dwDspBuf, dwOfst, dwSz*4);
		#else
        
                for (dwSz = 0; dwSz < 8640; dwSz++)
                {
                   do{  \
                  *((UINT32 *)(VIRTUAL(_u4DspDramBuf[2]+(0x400<<2)+((dwSz)<<2)))) = DTSENC_TABLE[dwSz];  \
                    HalFlushDCache(); \
                    }while(0) ;
                }
		#endif
        */

	    break;
#ifndef CC_AUD_SUPPORT_MS10
   case AUD_DDCO:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + DDCO_TABLE_ADR * 4, dwOfst, dwSz * 4);
        // write table3.bin
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TB3_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DDCO_TB3_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + DDCO_TABLE3_ADR * 4, dwOfst, dwSz * 4);
        break;
#endif
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    case AUD_POSTRAM:
   //case AUD_ADV: //Daniel, 2009/12/7, ADV code will be automatically loaded with AUD_POSTRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_ADV_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_ADV_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + ADV_TABLE_ADR * 4, dwOfst, dwSz * 4);
        break;
#endif
    case AUD_SBC_DEC1:
        // move MP3 Table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_SBC_TABLE_SA << 2));

        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_SBC_TABLE_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_SBCENC_DEC4:
        // MT5365_Check
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_SBCENC_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_SBCENC_TABLE_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        //Printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DSPF_SBCENC_TABLE_SA: %08x\n", dwDspBuf + dwTabAddr); //SBCENC_TEST
        break;

    case AUD_DRA_DEC1:
    case AUD_DRA_DEC2:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DRA_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DRA_TABLE_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;
    case AUD_G729_DEC1:
    case AUD_G729_DEC2:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_G729DEC_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_G729DEC_TABLE_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;

    case AUD_COOK_DEC1:
    case AUD_COOK_DEC2:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_COOKTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_COOKTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        break;

#ifdef MP3ENC_SUPPORT
	case AUD_MP3ENC_DEC3:
        // move MP3 encoder table to DRAM
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_MP3ENC_TABLE_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_MP3ENC_TABLE_SZ << 2));
        ///[Joel]Table in each decoder DRAMPage start addr is 0x400(dwTabAddr)
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        //[Joel] Load table from compiler *.bin file to CMPT DRAM
        //dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_MP3ENC_TABLE_SA << 2));
        //dwOfst = dwDspCalOfst (dwOfst);
        //dwSz = u4GetFlashData (dwFlashOfst + (DSPF_MP3ENC_TABLE_SZ << 2));
        //vFlash2DramDma (dwGetDSPNSADR (TYPE_AUD_DEC2) + 0x400*4, dwOfst, dwSz * 4);

        // Jessica: load data sram to decoder DRAM 0x9400 address
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_DROM_MPEG_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_DROM_MPEG_SZ << 2));
        vFlash2DramDmaShift (dwDspBuf + dwTabAddr + (0x1CD0*4), dwOfst, dwSz * 4);
        break;
#endif

#if 1///def CC_VORBIS_SUPPORT
    case AUD_VORBIS_DEC1:
    case AUD_VORBIS_DEC2:
        ///DRAM_TABLE_ADR       from Dec dram : 0x8400
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_VORBIS_NTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_VORBIS_NTBL_SZ << 2));
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);

        ///TWIDDLE_TABLE_ADR    from Dec dram : 0x10000 (0x8000+0x8000)
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_VORBIS_CTBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_VORBIS_CTBL_SZ << 2));
        ///vFlash2DramDma (dwDspBuf + 0x8000 * 4, dwOfst, dwSz * 4);
        vFlash2DramDma (dwDspBuf + (0x8000 - 0x3C00) * 4, dwOfst, dwSz * 4);
        fgGetBitsNotHold = TRUE;
      break;
#endif //VORBIS_SUPPORT

#ifdef CC_AUD_SUPPORT_MS10
    case AUD_COMMRAM:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_PTSINFO_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_PTSINFO_SZ << 2));
        vFlash2DramDma (dwDspBuf + ADDR_R2CD_PTSINFO_TABLE*4, dwOfst, dwSz * 4);
        break;
#endif
    case AUD_WMAPRO_DEC1:
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_WMAPRO_N_TABLE_SA << 2));

        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_WMAPRO_N_TABLE_SZ << 2));

//        vFlash2DramDma (dwDspBuf + WMAPRO_TABLE_NORMAL * 4, dwOfst, dwSz * 4);
        vFlash2DramDma (dwDspBuf + dwTabAddr, dwOfst, dwSz * 4);
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_WMAPRO_C_TABLE_SA << 2));

        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_WMAPRO_C_TABLE_SZ << 2));

        vFlash2DramDma (dwDspBuf + WMAPRO_TABLE_CMPT * 4, dwOfst, dwSz * 4);
        break;
    case AUD_AMR_DEC1:
        // word16 table
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AMR_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AMR_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSz * 4);
        // mode table
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AMR_MODE_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AMR_MODE_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr + 0x1B0*4, dwOfst, dwSz * 4);
        break;
    case AUD_AWB_DEC1:
        // word16 table
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AWB_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AWB_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr, dwOfst, dwSz * 4);
        // mode table
        dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AWB_MODE_TBL_SA << 2));
        dwOfst = dwDspCalOfst (dwOfst);
        dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AWB_MODE_TBL_SZ << 2));
        vFlash2DramDma (dwDspCmptBuf + dwTabAddr + 0x200*4 /*[driver related] awb_dram.inc*/, dwOfst, dwSz * 4);
        break;
    default:
        return;
    }

    // Set Get bits hold register accroding to different decoding
    vSetGetBitsNotHold (fgGetBitsNotHold);
}

/***************************************************************************
     Function : vDSPClearDecType
  Description : Clear all decoder types to default value
    Parameter :
       Author :
    Return    :
***************************************************************************/
void vDSPClearDecType(void) //  -- DSP Force --
{
    _rDspVars[AUD_DEC_MAIN].dwDspRamCodeType = 0xFF;
    _rDspVars[AUD_DEC_AUX].dwDspRamCodeType = 0xFF;
#ifdef CC_MT5391_AUD_3_DECODER
    _rDspVars[AUD_DEC_THIRD].dwDspRamCodeType = 0xFF;
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    _rDspVars[AUD_DEC_4TH].dwDspRamCodeType = 0xFF;
#endif
}

#ifdef CC_AUD_DATASRAM //Replace DATA ROM to DATA SRAM

#define DATA_SRAM_GRPCOMM               0x0
#define DATA_SRAM_GRP1                  0x770
#define DATA_SRAM_GRP2                  0xBB8
#define DATA_SRAM_MPEG_OFFSET           0x10

UINT32 pAUD2DROM1[] = {
    DSPF_DROM_AC3_SA,       // AUD_AC3_DEC1
    DSPF_DROM_MPEG_SA,      // AUD_MPEG_DEC1
    DSPF_DROM_AAC_SA,       // AUD_AAC_DEC1
    0,                                     // AUD_PCM_DEC1
    DSPF_DROM_MPEG_SA,      // AUD_MP3_DEC1
    0,                                    // AUD_WMA_DEC1
    0,                                    // AUD_PINK_DEC1
    DSPF_DROM_MTS_SA,       // AUD_NTSC_DEC1
    DSPF_DROM_MTS_SA,       // AUD_PAL_DEC1
    0,                                    // AUD_MINER_DEC1
    DSPF_DROM_MTS_SA,       // AUD_A2DEC_DEC1
    DSPF_DROM_MTS_SA,       // AUD_DETECTOR_DEC1
    DSPF_DROM_MTS_SA,       // AUD_FMFM_DEC1
    0,                                    // AUD_LPCM_DEC1
    DSPF_DROM_MTS_SA,       // AUD_FMRDO_DEC1
    DSPF_DROM_MTS_SA,       // AUD_FMRDODET_DEC1
    0,                                    // AUD_SBC_DEC1
    0,                                    // AUD_G711_DEC1
    0,                                    // AUD_G711_ENC1
    0,                                     // AUD_DRA_DEC1
    0,                                    // AUD_COOK_ENC1
    0,                                     // AUD_G729_DEC1
    0,                                    // AUD_DTS_DEC1
    0,                                    //AUD_VORBISDEC1, CC_VORBIS_SUPPORT
    0,                                    //AUD_WMAPRO_DEC1
    0,                                    //AUD_AMR_DEC1
    0,                                    //AUD_AWB_DEC1
};

UINT32 pAUD2DROM2[] = {
    DSPF_DROM_AC3_SA,       // AUD_AC3_DEC2
    DSPF_DROM_MPEG_SA,      // AUD_MPEG_DEC2
    DSPF_DROM_AAC_SA,       // AUD_AAC_DEC2
    0,                                    // AUD_PCM_DEC2
    DSPF_DROM_MTS_SA,       // AUD_NTSC_DEC2
    DSPF_DROM_MTS_SA,
    0,
    DSPF_DROM_MTS_SA,
    DSPF_DROM_MTS_SA,
    DSPF_DROM_MTS_SA,
    DSPF_DROM_MPEG_SA,      // AUD_MP3_DEC2
    0,					    // AUD_LPCM_DEC2
    0,                       // AUD_DRA_DEC2
    0                       // AUD_G729_DEC2
    //DSPF_NICAM_DEC2_SA,
    //DSPF_TTXACC_DEC2_SA,
};

#if 0
UINT32 pAUD2DROM3[] = {     /// MP3ENC_SUPPORT
    0,
    0,
    0,
    0,
    DSPF_DROM_MPEG_SA,      /// AUD_MP3ENC_DEC3
};
#endif

void vDSPLoadDataSram(UINT8 u1DecId, UINT32 u4DataSa)
{
    UINT32 dwOfst, dwSz, dwFlashOfst, dwStart;
    UINT32 i, u4Data;
    UINT32* puSrc;

    if ((u4DataSa == 0) || (_u4DsramGrp[0] == u4DataSa) || (_u4DsramGrp[1] == u4DataSa))
    {
        return;
    }

    if (u4DataSa == DSPF_DROM_COMM_SA)
    {
        if (_fgCommFlag)
        {
            return;
        }
        else
        {
            _fgCommFlag = TRUE;
        }
    }

    //zy_dts
    /*      //Disable DTS for code shrink
    if (u4DataSa == DSPF_DROM_DTSE)  
    {
        puSrc = (UINT32*)dts_tab;
		dwSz = 670;
		dwStart = DATA_SRAM_GRP2;
		_u4DsramGrp[1] = u4DataSa;


		WriteREG (RW_ADSPCR, (DR_RD_EN|PMEM_AUTO_INC|DROM_WR));  // disable dsp
        WriteREG (RW_DNLD_CNTR, dwStart); // reset download counter to 0

        for (i=0; i<(dwSz-1); i++)
        {
            WriteREG (RW_DROM_DNLD, puSrc[i]);
        }
        // Verify
        WriteREG (RW_DNLD_CNTR, dwStart); // reset download counter to 0
        for (i= 0; i<(dwSz-1); i++)
        {
            u4Data = ReadREG (RW_DROM_DNLD);
            VERIFY(u4Data == puSrc[i]);
        }
        WriteREG (RW_ADSPCR, 0);    // clear datarom write

		return;
    }
    */

    dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr
    dwOfst = u4GetFlashData (dwFlashOfst + (u4DataSa << 2));
    dwSz = u4GetFlashData (dwFlashOfst + ((u4DataSa+1) << 2));

    if (dwSz >= 1)
    {
        puSrc= ((UINT32*) (u4GetDspImgAddr () + ((dwOfst)*4)));
        // Src: u4GetDspImgAddr () + (dwOfst * 4)
        // Size: (dwSz - 1) * 4 bytes
        switch(u4DataSa)
        {
        // position 0
        case DSPF_DROM_COMM_SA:
            dwStart = DATA_SRAM_GRPCOMM;
            break;
        // position 1
        case DSPF_DROM_AAC_SA:
        case DSPF_DROM_MTS_SA:
            dwStart = DATA_SRAM_GRP1;
            _u4DsramGrp[0] = u4DataSa;
            break;
        case DSPF_DROM_MPEG_SA:
            dwStart = DATA_SRAM_GRP1+DATA_SRAM_MPEG_OFFSET; // Aligned at 0x4780
            _u4DsramGrp[0] = u4DataSa;
            break;
        //position 2
        case DSPF_DROM_AC3_SA:
        //case DSPF_DROM_DDCO_SA:
        //case DSPF_DROM_DTSD_SA:
        //case DSPF_DROM_DTSE_SA:
            dwStart = DATA_SRAM_GRP2;
            _u4DsramGrp[1] = u4DataSa;
            break;
        default:
            return;
        }

        WriteREG (RW_ADSPCR, DROM_WR);
        WriteREG (RW_ADSPCR, (DR_RD_EN|PMEM_AUTO_INC|DROM_WR));  // disable dsp
        WriteREG (RW_DNLD_CNTR, dwStart); // reset download counter to 0

        for (i=0; i<(dwSz-1); i++)
        {
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            WriteREG (RW_DROM_DNLD, (puSrc[i]<<8));
            #else
            WriteREG (RW_DROM_DNLD, puSrc[i]);
            #endif
        }
        // Verify
        WriteREG (RW_DNLD_CNTR, dwStart); // reset download counter to 0
        for (i= 0; i<(dwSz-1); i++)
        {
            u4Data = ReadREG (RW_DROM_DNLD);
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            VERIFY(u4Data == (puSrc[i]<<8));
            #else
            VERIFY(u4Data == puSrc[i]);
            #endif
        }
        WriteREG (RW_ADSPCR, DROM_WR);
        WriteREG (RW_ADSPCR, 0);    // clear datarom write
    }

}


//-----------------------------------------------------------------------------
/** vDspResetDataSram
 *  Reset data SRAM
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void vDspResetDataSram(void)
{
    _fgCommFlag = FALSE;
    _u4DsramGrp[0] = 0;
    _u4DsramGrp[1] = 0;
}
#endif

//-----------------------------------------------------------------------------
/** fgDSPCheckBonding
 *
 *  @param  u4Type: decoder type
 *
 *  @retval TRUE(supported). FALSE(not supported)
 */
//-----------------------------------------------------------------------------
BOOL fgDspCheckBonding(UINT32 u4Type)
{
    BOOL fgSupport = TRUE;
    UINT32 u4DecType = 0xFFFFFFFF;

    switch(u4Type & 0xe0)
    {
    case TYPE_AUD_DEC1:
        u4DecType = u4Type - TYPE_AUD_DEC1;
        break;
    case TYPE_AUD_DEC2:
        u4DecType = u4Type - TYPE_AUD_DEC2;
        break;
    case TYPE_AUD_DEC3:
        u4DecType = u4Type - TYPE_AUD_DEC3;
        break;
    case TYPE_AUD_DEC4:
        u4DecType = u4Type - TYPE_AUD_DEC4;
        break;
    default:
        break;
    }

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    if ((u4DecType == AUD_AC3_DEC1) ||
        (u4DecType == AUD_AC3_DEC2) ||
        (u4DecType == AUD_AC3_DEC3))
    {
        if ((!(IS_SupportDOLBY())) || (!(IS_SW_SupportAC3() || IS_SW_SupportEAC3() || IS_SW_SupportMS10())))
        {
            LOG(1, "Error: DD/DDP decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
    else if ((u4DecType == AUD_AAC_DEC1) ||
        (u4DecType == AUD_AAC_DEC2) ||
        (u4DecType == AUD_AAC_DEC3))
    {
        if (!(IS_SupportH264() || (IS_SupportDOLBY() && IS_SW_SupportMS10())))
        {
            LOG(1, "Error: HE-AAC decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
    else if (u4Type == AUD_DDCO)
    {
        if (!(IS_SupportDOLBY() || IS_SupportH264() || IS_SW_SupportMS10()))
        {
            LOG(1, "Error: DDCO decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
    else if ((u4DecType == AUD_DTS_DEC1) || (u4DecType == AUD_DTS_DEC2))
    {
        if (!IS_SupportDTS())
        {
            LOG(1, "Error: DTS decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
    else if ((u4DecType == AUD_DRA_DEC1) || (u4DecType == AUD_DRA_DEC2))
    {
        if (!IS_SupportAVS())
        {
            LOG(1, "Error: DRA decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }

#else

    if ((u4DecType == AUD_AC3_DEC1) ||
        (u4DecType == AUD_AC3_DEC2) ||
        (u4DecType == AUD_AC3_DEC3))
    {
        if (!(IS_SupportAC3() || IS_SupportEAC3() || IS_SupportDDCO() || IS_SupportMS10()))
        {
            LOG(1, "Error: DD/DDP decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
    else if ((u4DecType == AUD_AAC_DEC1) ||
        (u4DecType == AUD_AAC_DEC2) ||
        (u4DecType == AUD_AAC_DEC3))
    {
        if (!(IS_SupportDDCO() || IS_SupportMS10() || IS_SupportH264()))
        {
            LOG(1, "Error: HE-AAC decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
    else if (u4Type == AUD_DDCO)
    {
        if (!(IS_SupportDDCO() || IS_SupportMS10()))
        {
            LOG(1, "Error: DDCO decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
    else if ((u4DecType == AUD_DTS_DEC1) || (u4DecType == AUD_DTS_DEC2))
    {
        if (!IS_SupportDTS())
        {
            LOG(1, "Error: DTS decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
    else if ((u4DecType == AUD_DRA_DEC1) || (u4DecType == AUD_DRA_DEC2))
    {
        if (!IS_SupportAVS())
        {
            LOG(1, "Error: DRA decoder is not supported by this IC\n");
            fgSupport = FALSE;
        }
    }
#endif

    return fgSupport;
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
    UINT32 dwOfst, dwSz, dwSz1; // dwSz1 is to prevent the flush of dsp code of ac3dec3.pra
    BOOL fgLoadTable, fgLoadCode;
    UINT32 dwCodIndx, dwCodIndx1; // dwCodIndx1 is to prevent the flush of dsp code of ac3dec3.pra
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
        DSPF_PNOISE_SA,         // AUD_PINK_DEC1
        DSPF_NTSC_DEC1_SA,      // AUD_NTSC_DEC1
        DSPF_PAL_DEC1_SA,       // AUD_PAL_DEC1
        DSPF_MINER_DEC1_SA,     // AUD_MINER_DEC1
        DSPF_A2_DEC1_SA,        // AUD_A2DEC_DEC1
        DSPF_DETECTOR_DEC1_SA,  // AUD_DETECTOR_DEC1
        DSPF_FMFM_DEC1_SA,      // AUD_FMFM_DEC1
        DSPF_LPCM_DEC1_SA,      // AUD_LPCM_DEC1
        DSPF_FMRDODEC1_SA,      // AUD_FMRDO_DEC1
        DSPF_FMRDODETDEC1_SA,   // AUD_FMRDODET_DEC1
        DSPF_SBCDEC1_SA,        // AUD_SBC_DEC1
        DSPF_G711DEC1_SA,       // AUD_G711_DEC1
        DSPF_G711ENC1_SA,       // AUD_G711_ENC1
        DSPF_DRA_DEC1_SA,       // AUD_DRA_DEC1
        DSPF_COOK_DEC1_SA,       // AUD_COOKDEC1
        DSPF_G729DEC1_SA,       // AUD_G729DEC1
        DSPF_DTS_DEC1_SA,      // AUD_DTSDEC1
        DSPF_VORBIS_DEC1_SA,    // AUD_VORBISDEC1, CC_VORBIS_SUPPORT
        DSPF_WMAPRO_DEC1_SA,    // AUD_WMAPRO_DEC1
        DSPF_AMRDEC1_SA,        // AUD_AMR_DEC1
        DSPF_AWBDEC1_SA,        // AUD_AWB_DEC1
    };
    UINT32 pAUD2COD2[] = {
        DSPF_AC3_DEC2_SA,       // AUD_AC3_DEC2
        DSPF_MP2_DEC2_SA,       // AUD_MPEG_DEC2
        DSPF_AAC_DEC2_SA,       // AUD_AAC_DEC2
        DSPF_PCM_DEC2_SA,       // AUD_PCM_DEC2
        DSPF_NTSC_DEC2_SA,      // AUD_NTSC_DEC2
        DSPF_PAL_DEC2_SA,
        DSPF_MINER_DEC2_SA,
        DSPF_A2_DEC2_SA,
        DSPF_DETECTOR_DEC2_SA,
        DSPF_FMFM_DEC2_SA,
        DSPF_MP3_DEC2_SA,        // AUD_MP3_DEC2   0xa
        DSPF_LPCM_DEC2_SA,       // AUD_LPCM_DEC2  0xb
        DSPF_DRA_DEC2_SA,         // AUD_DRA_DEC2
        DSPF_G729DEC2_SA,         // AUD_G729DEC2
        DSPF_DTS_DEC2_SA,      // AUD_DTSDEC2
        DSPF_COOK_DEC1_SA,
        DSPF_VORBIS_DEC2_SA,    // AUD_VORBISDEC2, CC_VORBIS_SUPPORT
        //DSPF_COOK_DEC2_SA        // AUD_COOKDEC2
        //DSPF_NICAM_DEC2_SA,
        //DSPF_TTXACC_DEC2_SA,
    };

#ifdef CC_MT5391_AUD_3_DECODER
    UINT32 pAUD2COD3[] = {
        DSPF_MP2_DEC3_SA,       // AUD_MPEG_DEC3
        DSPF_PCM_DEC3_SA,       // AUD_PCM_DEC3
        DSPF_AAC_DEC3_SA,       // AUD_AAC_DEC3
        DSPF_AC3_DEC3_SA,       // AUD_AC3_DEC3
#ifdef MP3ENC_SUPPORT
        DSPF_MP3ENC_DEC3_SA,
#endif
    };

    UINT32 pAUD2COD3VCT[] = {
        DSPF_VCT_MP2DEC3_SA,
        DSPF_VCT_PCMDEC3_SA,
        DSPF_VCT_AACDEC3_SA,
        DSPF_VCT_AC3DEC3_SA,
#ifdef MP3ENC_SUPPORT
        DSPF_VCT_MP3ENC_DEC3_SA,
#endif
    };
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    UINT32 pAUD2COD4[] =
    {
        DSPF_SBCENC4_SA,        // AUD_SBC_ENC_DEC4
        DSPF_PCM_DEC4_SA        // AUD_PCM_DEC4
    };
#endif

    UINT8 pDspStrTyp[] = {
        AC3_STREAM,             // AUD_AC3_DEC1
        MPEG12_STREAM,          // AUD_MPEG_DEC1
        AAC_STREAM,             // AUD_AAC_DEC1
        PCM_STREAM,             // AUD_PCM_DEC1
        MPEG3_STREAM,           // AUD_MP3_DEC1
        WMA_STREAM,             // AUD_WMA_DEC1
        PINK_NOISE_STREAM,      // AUD_PINK_DEC1
        NTSC_STREAM,
        PAL_STREAM,
        MINER_STREAM,
        A2_STREAM,
        DETECTOR_STREAM,
        FMFM_STREAM,
        LPCM_STREAM,
        FMRDO_STREAM,
        FMRDODET_STREAM,
        SBC_STREAM,              // AUD_SBC_DEC1
        G711DEC_STREAM,          // AUD_G711_DEC1
        G711ENC_STREAM,          // AUD_G711_ENC1
        DRA_STREAM,              // AUD_DRA_DRC1
        COOK_STREAM,              // AUD_COOK_DEC1
        G729DEC_STREAM,              // AUD_G729_DEC1
        DTSDEC_STREAM,              // AUD_DTS_DEC1
        VORBIS_STREAM,          //AUD_VORBISDEC1, CC_VORBIS_SUPPORT
        WMAPRO_STREAM,          //AUD_WMAPRO_DEC1
        AMR_STREAM,             //AUD_AMR_DEC1
        AWB_STREAM,           //AUD_AWB_DEC1
    };
    UINT8 pDspStrTyp2[] = {
        AC3_STREAM,             // AUD_AC3_DEC2
        MPEG12_STREAM,          // AUD_MPEG_DEC2
        AAC_STREAM,             // AUD_AAC_DEC2
        PCM_STREAM,             // AUD_PCM_DEC2
        NTSC_STREAM,            // AUD_NTSC_DEC2
        PAL_STREAM,
        MINER_STREAM,
        A2_STREAM,
        DETECTOR_STREAM,
        FMFM_STREAM,
        MPEG3_STREAM,           // AUD_MP3_DEC2
        LPCM_STREAM,            // AUD_LPCM_DEC2
        DRA_STREAM,             // AUD_DRA_DRC2
        G729DEC_STREAM,             // AUD_G729_DEC2
        COOK_STREAM,            // AUD_COOK_DEC2
        DTSDEC_STREAM,              // AUD_DTS_DEC2
        VORBIS_STREAM,          //AUD_VORBISDEC2, CC_VORBIS_SUPPORT
    };

#ifdef CC_MT5391_AUD_3_DECODER
    UINT8 pDspStrTyp3[] = {
        MPEG12_STREAM,          // AUD_MPEG_DEC3
        PCM_STREAM,             // AUD_PCM_DEC3
        AAC_STREAM,             // AUD_AAC_DEC3
        AC3_STREAM,             // AUD_AC3_DEC3
#ifdef MP3ENC_SUPPORT
        MP3ENC_STREAM,
#endif
    };
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    UINT8 pDspStrTyp4[] = {
        SBCE_STREAM,             // AUD_SBCE_DEC4
        PCM_STREAM,              // AUD_PCM_DEC4
    };
#endif

    // Check capability
    if (!fgDspCheckBonding(u4Type))
    {
        return;
    }

    fgLoadCode  = FALSE;
    fgLoadTable = FALSE;
    dwFlashOfst = u4GetDspImgAddr ();   // set to dsp image addr
    switch (u4Type & 0xe0)
    {
    case TYPE_AUD_DEC1:
        _rDspVars[AUD_DEC_MAIN].bCode3D = CODE3D_NONE;
        _rDspVars[AUD_DEC_MAIN].bCode3DUpmix = CODE3D_NONE;

        if (_rDspStatus[AUD_DEC_MAIN].fgDspPlay)
        {
            vDspReset(AUD_DEC_MAIN);
        }
        if (u4Type > DECODER1_MAX_INDEX)
        {
            ASSERT(0);
        }
        //LOG(0, "...Test.., u4Type=0x%x, (u4Type - TYPE_AUD_DEC1)=%d, size=%d\n", u4Type,(u4Type - TYPE_AUD_DEC1),(sizeof(pAUD2COD1)/sizeof(UINT32)));
        if ((u4Type - TYPE_AUD_DEC1) >= (sizeof(pAUD2COD1)/sizeof(UINT32)))// For Klocwork check
        {
            ASSERT(0);
        }
        if ( u4Type >= (sizeof(pDspStrTyp)/sizeof(UINT8)))
        {
            ASSERT(0);
        }
#ifdef CC_AUD_SUPPORT_MS10
        if (pAUD2COD1[u4Type - TYPE_AUD_DEC1] == DSPF_AAC_DEC1_SA)
        {
            //load DDCO
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SZ<<2));
       #ifdef   DSP_SUPPORT_17BIT
            // ddco is loaded in program space 2, decoder 1
            dwCodeDestAdr =(dwGetDspIcacheAdr (TYPE_DEC1_ICACHE2) + DDCO_PRAM_ADR) * 4;
       #else
            dwCodeDestAdr =(dwGetDspIcacheAdr (TYPE_DEC1_ICACHE) + DDCO_PRAM_ADR) * 4;
       #endif
            dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
            vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
        }
#endif
        fgLoadTable = _fgDecTblFromFlash;
        /*lint -e{661}*/
        dwCodIndx   = pAUD2COD1[u4Type];
        dwOfst      = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
        dwSz        = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));
        vResetDecInfo (AUD_DEC_MAIN);
        u2RamCodeType = TYPE_DEC1_ICACHE;
        vLoadDspDRAMTable (u4Type);
        vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG, 0x1);
        _rDspVars[AUD_DEC_MAIN].dwDspRamCodeType = u4Type;
        /*lint -e{661}*/
        _rDspVars[AUD_DEC_MAIN].bDspStrTyp = pDspStrTyp[u4Type];
        fgLoadCode = TRUE;
#ifdef CC_AUD_DATASRAM //Replace DATA ROM to DATA SRAM
        if ( (u4Type-TYPE_AUD_DEC1) >= (sizeof(pAUD2DROM1)/sizeof(UINT32)))
        {
            ASSERT(0);
        }
        vDSPLoadDataSram(AUD_DEC_MAIN, pAUD2DROM1[u4Type-TYPE_AUD_DEC1]);
#endif
        break;
    case TYPE_AUD_DEC2:
        _rDspVars[AUD_DEC_AUX].bCode3D = CODE3D_NONE;
        _rDspVars[AUD_DEC_AUX].bCode3DUpmix = CODE3D_NONE;
        if (_rDspStatus[AUD_DEC_AUX].fgDspPlay)
        {
            vDspReset(AUD_DEC_AUX);
        }
        if ((u4Type - TYPE_AUD_DEC2) > DECODER2_MAX_INDEX)
        {
            ASSERT(0);
        }
        //LOG(0, "...Test.., u4Type=0x%x, (u4Type - TYPE_AUD_DEC2)=%d, size=%d\n", u4Type,(u4Type - TYPE_AUD_DEC2),(sizeof(pAUD2COD2)/sizeof(UINT32)));
        if ((u4Type - TYPE_AUD_DEC2) >= (sizeof(pAUD2COD2)/sizeof(UINT32)))// For Klocwork check
        {
            ASSERT(0);
        }
        if (pAUD2COD2[u4Type - TYPE_AUD_DEC2] == DSPF_AC3_DEC2_SA)
        {
            // load DDP converter
            dwCodIndx   = DSPF_DDPCONV_SA;
            if (dwCodIndx)
            {
                dwCodIndx1 = DSPF_AC3_DEC3_SA;
                dwSz1 = u4GetFlashData (dwFlashOfst + ((dwCodIndx1 + 1) << 2));
                dwOfst = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2)) + dwSz1;
                dwSz = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2)) - dwSz1;
                if (dwSz)
                {
                    dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC3_ICACHE) + dwSz1;
                    vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
                }
            }
        }
        else if (pAUD2COD2[u4Type - TYPE_AUD_DEC2] == DSPF_AAC_DEC2_SA)
        {
            dwCodIndx = DSPF_AAC_DEC2EXT_SA;
            // decoder 2 extension code exists
            dwOfst = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
            dwSz = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));
            if (dwSz)
            {
                dwDECICAddr = (dwGetDspIcacheAdr (TYPE_DEC3_ICACHE)+AAC_PRAM_DEC2_EXT_ADR);
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }

#ifdef CC_AUD_SUPPORT_MS10
            if (fgDspCheckBonding(AUD_DDCO) == TRUE)
            {
                //load DDCO
                dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SA<<2));
                dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SZ<<2));
       #ifdef   DSP_SUPPORT_17BIT
                // ddco is loaded in program space 2, decoder 1
                dwCodeDestAdr =(dwGetDspIcacheAdr (TYPE_DEC1_ICACHE2) + DDCO_PRAM_ADR) * 4;
       #else
                dwCodeDestAdr =(dwGetDspIcacheAdr (TYPE_DEC1_ICACHE) + DDCO_PRAM_ADR) * 4;
       #endif
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
#endif
        }
        fgLoadTable = _fgDec2TblFromFlash;

         /*lint -e661*/ /*lint -e662*/
        dwCodIndx   = pAUD2COD2[u4Type - TYPE_AUD_DEC2];
        dwOfst      = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
        dwSz        = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));

        vResetDecInfo (AUD_DEC_AUX);
        u2RamCodeType = TYPE_DEC2_ICACHE;
        vLoadDspDRAMTable (u4Type);
        vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC2, 0x1);
        _rDspVars[AUD_DEC_AUX].dwDspRamCodeType = u4Type;
         /*lint -e661*/ /*lint -e662*/
        _rDspVars[AUD_DEC_AUX].bDspStrTyp = pDspStrTyp2[u4Type - TYPE_AUD_DEC2];
        fgLoadCode = TRUE;
#ifdef CC_AUD_DATASRAM //Replace DATA ROM to DATA SRAM
        if ( (u4Type-TYPE_AUD_DEC2) >= (sizeof(pAUD2DROM2)/sizeof(UINT32)))
        {
            ASSERT(0);
        }
        vDSPLoadDataSram(AUD_DEC_AUX, pAUD2DROM2[u4Type-TYPE_AUD_DEC2]);
#endif

        break;
#ifdef CC_MT5391_AUD_3_DECODER
    // gallen, use this as dec3
    case TYPE_AUD_DEC3:
        _rDspVars[AUD_DEC_THIRD].bCode3D = CODE3D_NONE;
        _rDspVars[AUD_DEC_THIRD].bCode3DUpmix = CODE3D_NONE;
        if (_rDspStatus[AUD_DEC_THIRD].fgDspPlay)
        {
            vDspReset(AUD_DEC_THIRD);
        }
        if ((u4Type - TYPE_AUD_DEC3) > DECODER3_MAX_INDEX)
        {
            ASSERT(0);
        }
        //LOG(0, "...Test.., u4Type=0x%x, (u4Type - TYPE_AUD_DEC3)=%d, size=%d\n", u4Type,(u4Type - TYPE_AUD_DEC3),(sizeof(pAUD2COD3)/sizeof(UINT32)));
        if ((u4Type - TYPE_AUD_DEC3) >= (sizeof(pAUD2COD3)/sizeof(UINT32))) // For Klocwork check
        {
            ASSERT(0);
        }
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
                dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC2_ICACHE) - DEC3_VCT_SIZE;
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
#ifdef DSP_SUPPORT_17BIT
				if (dwCodIndx == DSPF_VCT_MP3ENC_DEC3_SA)
				{
					dwDECICAddr = dwGetDspIcacheAdr (TYPE_DEC2_ICACHE2) - DEC3_VCT_SIZE;
                	vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
				}
#endif
            }
        }

        fgLoadTable = _fgDec3TblFromFlash;
         /*lint -e661*/ /*lint -e662*/
        dwCodIndx   = pAUD2COD3[u4Type - TYPE_AUD_DEC3];
        dwOfst      = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
        dwSz        = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));
        vResetDecInfo (AUD_DEC_THIRD);
#ifdef DSP_SUPPORT_17BIT
		if (dwCodIndx == DSPF_MP3ENC_DEC3_SA)
		{
			u2RamCodeType = TYPE_DEC3_ICACHE2;
		}
		else
		{
			u2RamCodeType = TYPE_DEC3_ICACHE;
		}
#else
		u2RamCodeType = TYPE_DEC3_ICACHE;
#endif
        vLoadDspDRAMTable (u4Type);
        vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC3, 0x1);

        _rDspVars[AUD_DEC_THIRD].dwDspRamCodeType = u4Type;
         /*lint -e661*/ /*lint -e662*/
        _rDspVars[AUD_DEC_THIRD].bDspStrTyp = pDspStrTyp3[u4Type - TYPE_AUD_DEC3];
        fgLoadCode = TRUE;
#if 0
        vDSPLoadDataSram(AUD_DEC_THIRD, pAUD2DROM3[u4Type-TYPE_AUD_DEC3]);    /// MP3ENC_SUPPORT
#endif
        break;
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    case TYPE_AUD_DEC4:
        //Printf("vDspLoadRamCode: TYPE_AUD_DEC4: %x\n", u4Type); //SBCENC_TEST
        _rDspVars[AUD_DEC_4TH].bCode3D = CODE3D_NONE;
        _rDspVars[AUD_DEC_4TH].bCode3DUpmix = CODE3D_NONE;
        if (_rDspStatus[AUD_DEC_4TH].fgDspPlay)
        {
            vDspReset(AUD_DEC_4TH);
        }
        if ((u4Type - TYPE_AUD_DEC4) > DECODER4_MAX_INDEX)
        {
            ASSERT(0);
        }
        //LOG(0, "...Test.., u4Type=0x%x, (u4Type - TYPE_AUD_DEC4)=%d, size=%d\n", u4Type,(u4Type - TYPE_AUD_DEC4),(sizeof(pAUD2COD4)/sizeof(UINT32)));
        if ((u4Type - TYPE_AUD_DEC4) >= (sizeof(pAUD2COD4)/sizeof(UINT32)))
        {
            ASSERT(0);
        }

        fgLoadTable = _fgDec4TblFromFlash;
         /*lint -e661*/ /*lint -e662*/
        dwCodIndx   = pAUD2COD4[u4Type - TYPE_AUD_DEC4];
        dwOfst      = u4GetFlashData (dwFlashOfst + (dwCodIndx << 2));
        dwSz        = u4GetFlashData (dwFlashOfst + ((dwCodIndx + 1) << 2));
        vResetDecInfo (AUD_DEC_4TH);
        u2RamCodeType = TYPE_DEC4_ICACHE;
        vLoadDspDRAMTable (u4Type);
        vWriteDspWORD (ADDR_RC2D_DOWNLOAD_TABLE_FLAG_DEC4, 0x1);

        _rDspVars[AUD_DEC_4TH].dwDspRamCodeType = u4Type;
         /*lint -e661*/ /*lint -e662*/
        _rDspVars[AUD_DEC_4TH].bDspStrTyp = pDspStrTyp4[u4Type - TYPE_AUD_DEC4];
        fgLoadCode = TRUE;
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
            if(AUD_IsSupportSRS())
            {
                dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_SRS_TSHD_SA<<2));
                dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_SRS_TSHD_SZ<<2));
            }
            else
            {
                dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_VSURR_POST_SA<<2));
                dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_VSURR_POST_SZ<<2));
            }
            if (dwSz > 0)
            {
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + VIRTUAL_SURROUND_PRAM_ADR) * 4;
                // Rice: check if exceed the down-mix section @20060804
                ASSERT((dwCodeDestAdr+((dwSz-1)*4)) <=
                       ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+SPEAKER_ENHANCEMENT_PRAM_ADR) * 4));
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
                /* move VSURR code from flash */
            }
#endif //DOWNMIX_SUPPORT
#ifdef DSP_SUPPORT_SRSVIQ
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_VIQ_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_VIQ_SZ<<2));
            if (dwSz > 0)
            {
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + VOICE_ENHANCEMENT_PRAM_ADR) * 4;
                ASSERT((dwCodeDestAdr+((dwSz-1)*4)) <
                    ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+POST_SECTION_END_PRAM_ADR) * 4));
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
#endif // DSP_SUPPORT_SRSVIQ
#ifdef 	CC_AUD_BBE_SUPPORT
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

#ifdef	CC_AUD_DSP_SUPPORT_CDNOTCH
			dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_CDNOTCH_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_CDNOTCH_SZ<<2));
			if (dwSz > 0)
            {
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + VOICE_ENHANCEMENT_PRAM_ADR) * 4;
                ASSERT((dwCodeDestAdr+((dwSz-1)*4)) <
                    ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+POST_SECTION_END_PRAM_ADR) * 4));
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
			Printf("load cdnotch code sz %d offset %d 0x%X\n", dwSz, dwOfst, dwCodeDestAdr);
#endif //CC_AUD_DSP_SUPPORT_CDNOTCH

#ifdef  KARAOKE_SUPPORT
       // dynamic load keyshift program
       dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_KEYSHIFT_SA<<2));
       dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_KEYSHIFT_SZ<<2));
	    if (dwSz > 0)
        {
            dwCodeDestAdr =
                (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + VOICE_ENHANCEMENT_PRAM_ADR) * 4;
            ASSERT((dwCodeDestAdr+((dwSz-1)*4)) <
                ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+POST_SECTION_END_PRAM_ADR) * 4));
            dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
            vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
        }
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_ADV_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_ADV_SZ<<2));
            if (dwSz > 0)
            {
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + ADV_PRAM_ADR) * 4;
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
            fgLoadTable = TRUE;
#endif // CC_AUD_DSP_SUPPORT_AUDYSSEY

#ifdef CC_AUD_VBASS_SUPPORT
			dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_VBASS_SA<<2));
			dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_VBASS_SZ<<2));
			if (dwSz > 0)
            {
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + VOICE_ENHANCEMENT_PRAM_ADR) * 4;
                ASSERT((dwCodeDestAdr+((dwSz-1)*4)) <
                    ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+POST_SECTION_END_PRAM_ADR) * 4));
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
#endif // CC_AUD_VBASS_SUPPORT

#ifdef CC_AUD_SPEAKER_HEIGHT
			dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_SPKHEIGHT_SA<<2));
			dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_SPKHEIGHT_SZ<<2));
			if (dwSz > 0)
            {
                dwCodeDestAdr =
                    (dwGetDspIcacheAdr (TYPE_POST_ICACHE) + SPEAKER_ENHANCEMENT_PRAM_ADR) * 4;
                ASSERT((dwCodeDestAdr+((dwSz-1)*4)) <
                    ((dwGetDspIcacheAdr (TYPE_POST_ICACHE)+VOICE_ENHANCEMENT_PRAM_ADR) * 4));
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            }
#endif

            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SZ << 2));
            u2RamCodeType = TYPE_POST_ICACHE;
            fgLoadCode = TRUE;
            break;
#ifndef CC_AUD_SUPPORT_MS10
	    case AUD_DDCO:
                dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SA<<2));
                dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_DDCO_SZ<<2));
                dwCodeDestAdr =(dwGetDspIcacheAdr (TYPE_DEC1_ICACHE) + DDCO_PRAM_ADR) * 4;
                dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
                vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
                dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SA << 2));
                dwSz = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SZ << 2));
                u2RamCodeType = TYPE_POST_ICACHE;
                fgLoadCode = TRUE;
                fgLoadTable = TRUE;
            break;
#endif
	   case AUD_DTSENC:   //zy_dts
			dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_DTSENC_SA<<2));
			dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_DTSENC_SZ<<2));
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
			dwCodeDestAdr =(dwGetDspIcacheAdr (TYPE_DEC3_ICACHE)) * 4; //load @ dec3 page
			#else
			dwCodeDestAdr =(dwGetDspIcacheAdr (TYPE_DEC3_ICACHE) + 0x100/2) * 4; //load @ dec3 page, offset 0x100
			#endif
			dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
			vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
			vDSPLoadDataSram(AUD_DEC_MAIN, DSPF_DROM_DTSE);
			dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SA << 2));
			dwSz = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SZ << 2));
			u2RamCodeType = TYPE_POST_ICACHE;
			fgLoadCode = TRUE;
			fgLoadTable = TRUE;
			break;
#if 0 //def CC_AUD_DSP_SUPPORT_AUDYSSEY
        //Daniel, 2009/12/7, ADV code will be automatically loaded with AUD_POSTRAM.
	    case AUD_ADV:
            dwOfst= u4GetFlashData(u4GetDspImgAddr()+(DSPF_ADV_SA<<2));
            dwSz  = u4GetFlashData(u4GetDspImgAddr()+(DSPF_ADV_SZ<<2));
            dwCodeDestAdr =(dwGetDspIcacheAdr (TYPE_POST_ICACHE) + ADV_PRAM_ADR) * 4;
            dwCodeSrcAdr = u4GetDspImgAddr () + (dwOfst * 4);
            vFlash2DramDma (dwCodeDestAdr, dwCodeSrcAdr, (dwSz - 1) * 4);
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_POST_VECT_SZ << 2));
            u2RamCodeType = TYPE_POST_ICACHE;
            fgLoadCode = TRUE;
            fgLoadTable = TRUE;
            break;
#endif

        case AUD_COMMRAM:
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_COMM_CODE_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_COMM_CODE_SZ << 2));

            u2RamCodeType = TYPE_COMM_ICACHE;
            fgLoadCode = TRUE;
#ifdef CC_AUD_SUPPORT_MS10
            fgLoadTable = TRUE;
#endif
#ifdef CC_AUD_DATASRAM //Replace DATA ROM to DATA SRAM
            vDSPLoadDataSram(AUD_DEC_MAIN, DSPF_DROM_COMM_SA);
            #if defined(CC_FPGA) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) //load the MTS_SA first for test pattern
            vDSPLoadDataSram(AUD_DEC_MAIN, DSPF_DROM_MTS_SA);
            #endif
#endif
            break;
        case AUD_ROMCOD:
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_ROM_CODE_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_ROM_CODE_SZ << 2));

            u2RamCodeType = TYPE_ROM_ICACHE;
            fgLoadCode = TRUE;
            break;
        // Light: Add load code for AinIntRAM
        case AUD_AININTRAM:
#if 0
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_AIN_INTRAM_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_AIN_INTRAM_SZ << 2));
            if (dwSz)
            {
                dwDECICAddr = dwGetDspIcacheAdr (TYPE_CRI_ICACHE);
                vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4), (dwSz - 1) * 4);
            }
#endif
            dwOfst = u4GetFlashData (dwFlashOfst + (DSPF_CRITICAL_CODE_SA << 2));
            dwSz = u4GetFlashData (dwFlashOfst + (DSPF_CRITICAL_CODE_SZ << 2));
            u2RamCodeType = TYPE_CRI_ICACHE;
            fgLoadCode = TRUE;
            break;
        default:
            fgLoadCode = FALSE;
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

    // Check dwSz > 0 for preventing take empty entry code
    /*lint -e{774}*/
    if ((fgLoadCode == TRUE) && (dwSz >= 1))
    {
        // Rice: check if exceed to next section. @20060804

        if ((u2RamCodeType < ICACHE_MAX_IDX) &&     // Except the last section
            (u2RamCodeType != TYPE_DEC1_ICACHE))    // Dec1 may share code with Dec3
        {
            if (((dwDECICAddr+(dwSz-1))*4) > (dwGetDspIcacheAdr((UINT16)(u2RamCodeType+1))*4))
            {
                LOG(0, "Error -> Not enough space for RAM code type %d\n", u2RamCodeType);
                LOG(1, "DRAM addr1 : %d\n",  (((UINT32) _u4DspICacheAddr[u2RamCodeType]) <<6));
                LOG(1, "DRAM addr2 : %d\n",  (((UINT32) _u4DspICacheAddr[u2RamCodeType+1]) <<6));
                LOG(1, "RAM code len : %d\n", dwSz);
                ASSERT(0);
            }
        }
        //  Flush Cache
        vFlash2DramDma ((dwDECICAddr * 4), u4GetDspImgAddr () + (dwOfst * 4),
                        (dwSz - 1) * 4);
        #ifdef DSP_SUPPORT_17BIT
        if ((u2RamCodeType == TYPE_COMM_ICACHE) ||
            (u2RamCodeType == TYPE_ROM_ICACHE) ||
            (u2RamCodeType == TYPE_CRI_ICACHE) ||
            (u2RamCodeType == TYPE_POST_ICACHE))
        {
            Printf("!!!! Load to 2nd page: %d\n", u2RamCodeType);
            vFlash2DramDma ((dwDECICAddr * 4)+0x20000, u4GetDspImgAddr () + (dwOfst * 4),
                            (dwSz - 1) * 4);
        }
        #endif

        vFlushDspICache (FALSE);
    }
    else if (fgLoadCode == FALSE)
    {
        LOG(1, "Error -> Not matched RAM code type!!");
    }
    else
    {
        LOG(1, "Error -> No RAM code type %d exists!!\n", u2RamCodeType);

        if (u2RamCodeType == TYPE_ROM_ICACHE)
        {
            LOG(1, "Error -> No ROM code pra for RAM boot\n");
            ASSERT(0);
        }
    }

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
INT32 i4DspSendPts (UINT8 u1DecId, UINT32 u4Pts, UINT32 u4ParPnt)
{
    if (fgDspBusy ())
    {
        return DSP_BUSY;
    }

    if (_rDspStatus[u1DecId].fgDspStopIssue)
    {
        return DSP_FAIL;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        vSendDspLongInt (INT_RC2D_PTS, u4ParPnt, u4Pts);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        vSendDspLongInt (INT_RC2D_PTS_DEC2, u4ParPnt, u4Pts);
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        vSendDspLongInt (INT_RC2D_PTS_DEC3, u4ParPnt, u4Pts);
    }
#endif

    return DSP_OK;
}

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
        if (_rDspStatus[AUD_DEC_MAIN].fgDspPlay)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPlay = TRUE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPause = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspStop = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspStopIssue = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgSendPts2Dsp = FALSE;
        }
    }
    else if (u4DspCmd == DSP_STOP)
    {
        if (_rDspStatus[AUD_DEC_MAIN].fgDspStopIssue && (!_rDspStatus[AUD_DEC_MAIN].fgDspFlush))
        {
            AUD_UopCommandDone(AUD_DEC_MAIN, DSP_STOP);
            LOG(1, "_fgDspStopIssue return \n");
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPlay = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspFlush = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspStopIssue = TRUE;
        }
    }
    else if (u4DspCmd == DSP_FLUSH)
    {
        if ((_rDspStatus[AUD_DEC_MAIN].fgDspStop) || (_rDspStatus[AUD_DEC_MAIN].fgDspFlush))
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_MAIN].fgDspFlush = TRUE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspStopIssue = TRUE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPlay = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PAUSE)
    {
        AUD_UopCommandDone(AUD_DEC_MAIN, DSP_PAUSE);

        // assume DSP_RESUME is only issued in DSP_PAUSE state
        if (!_rDspStatus[AUD_DEC_MAIN].fgDspPlay || _rDspStatus[AUD_DEC_MAIN].fgDspStop)	//koro: should not pause when DSP stop
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPlay = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPause = TRUE;
        }
    }

    else if (u4DspCmd == DSP_RESUME)
    {
        AUD_UopCommandDone(AUD_DEC_MAIN, DSP_RESUME);

        if (!_rDspStatus[AUD_DEC_MAIN].fgDspPause)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_MAIN].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPlay = TRUE;
            _rDspStatus[AUD_DEC_MAIN].fgDspPause = FALSE;
        }
    }
    else if (u4DspCmd == DSP_WAKEUP)
    {
        // If we send wakeup before.. do not send again
        if (_rDspStatus[AUD_DEC_MAIN].fgDspWakeUp)
        {
            i4Ret = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PLAY_DEC2)
    {
        AUD_UopCommandDone(AUD_DEC_AUX, DSP_PLAY_DEC2);
        if (_rDspStatus[AUD_DEC_AUX].fgDspPlay)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_AUX].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspPlay = TRUE;
            _rDspStatus[AUD_DEC_AUX].fgDspStop = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspStopIssue = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgSendPts2Dsp = FALSE;
        }
    }
    else if (u4DspCmd == DSP_STOP_DEC2)
    {
        if (_rDspStatus[AUD_DEC_AUX].fgDspStopIssue && (!_rDspStatus[AUD_DEC_AUX].fgDspFlush))
        {
            AUD_UopCommandDone(AUD_DEC_AUX, DSP_STOP_DEC2);
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_AUX].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspPlay = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspFlush = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspStopIssue = TRUE;
        }
    }
    else if (u4DspCmd == DSP_FLUSH_DEC2)
    {
        if ((_rDspStatus[AUD_DEC_AUX].fgDspStop) || (_rDspStatus[AUD_DEC_AUX].fgDspFlush))
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_AUX].fgDspFlush = TRUE;
            _rDspStatus[AUD_DEC_AUX].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspStopIssue = TRUE;
            _rDspStatus[AUD_DEC_AUX].fgDspPlay = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PAUSE_DEC2)
    {
        AUD_UopCommandDone(AUD_DEC_AUX, DSP_PAUSE_DEC2);
        // assume DSP_RESUME is only issued in DSP_PAUSE state
        if (!_rDspStatus[AUD_DEC_AUX].fgDspPlay)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_AUX].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspPlay = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspPause = TRUE;
        }
    }

    else if (u4DspCmd == DSP_RESUME_DEC2)
    {
        AUD_UopCommandDone(AUD_DEC_AUX, DSP_RESUME_DEC2);

        if (!_rDspStatus[AUD_DEC_AUX].fgDspPause)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_AUX].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_AUX].fgDspPlay = TRUE;
            _rDspStatus[AUD_DEC_AUX].fgDspPause = FALSE;
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u4DspCmd == DSP_PLAY_DEC3)
    {
        AUD_UopCommandDone(AUD_DEC_THIRD, DSP_PLAY_DEC3);
        if (_rDspStatus[AUD_DEC_THIRD].fgDspPlay)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_THIRD].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspPlay = TRUE;
            _rDspStatus[AUD_DEC_THIRD].fgDspStop = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspStopIssue = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgSendPts2Dsp = FALSE;
        }
    }
    else if (u4DspCmd == DSP_STOP_DEC3)
    {
        if (_rDspStatus[AUD_DEC_THIRD].fgDspStopIssue && (!_rDspStatus[AUD_DEC_THIRD].fgDspFlush))
        {
            AUD_UopCommandDone(AUD_DEC_THIRD, DSP_STOP_DEC3);
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_THIRD].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspPlay = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspFlush = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspStopIssue = TRUE;
        }
    }
    else if (u4DspCmd == DSP_FLUSH_DEC3)
    {
        if ((_rDspStatus[AUD_DEC_THIRD].fgDspStop) || (_rDspStatus[AUD_DEC_THIRD].fgDspFlush))
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_THIRD].fgDspFlush = TRUE;
            _rDspStatus[AUD_DEC_THIRD].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspStopIssue = TRUE;
            _rDspStatus[AUD_DEC_THIRD].fgDspPlay = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PAUSE_DEC3)
    {
        AUD_UopCommandDone(AUD_DEC_THIRD, DSP_PAUSE_DEC3);
        // assume DSP_RESUME is only issued in DSP_PAUSE state
        if (!_rDspStatus[AUD_DEC_THIRD].fgDspPlay)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_THIRD].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspPlay = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspPause = TRUE;
        }
    }

    else if (u4DspCmd == DSP_RESUME_DEC3)
    {
        AUD_UopCommandDone(AUD_DEC_THIRD, DSP_RESUME_DEC3);

        if (!_rDspStatus[AUD_DEC_THIRD].fgDspPause)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_THIRD].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_THIRD].fgDspPlay = TRUE;
            _rDspStatus[AUD_DEC_THIRD].fgDspPause = FALSE;
        }
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u4DspCmd == DSP_PLAY_DEC4)
    {
        //Printf("fgCheckDspCmd - DSP_PLAY_DEC4\n"); //SBCENC_TEST
        AUD_UopCommandDone(AUD_DEC_4TH, DSP_PLAY_DEC4);
        if (_rDspStatus[AUD_DEC_4TH].fgDspPlay)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_4TH].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspPlay = TRUE;
            _rDspStatus[AUD_DEC_4TH].fgDspStop = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspStopIssue = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgSendPts2Dsp = FALSE;
        }
    }
    else if (u4DspCmd == DSP_STOP_DEC4)
    {
        if (_rDspStatus[AUD_DEC_4TH].fgDspStopIssue && (!_rDspStatus[AUD_DEC_4TH].fgDspFlush))
        {
            AUD_UopCommandDone(AUD_DEC_4TH, DSP_STOP_DEC4);
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_4TH].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspPlay = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspFlush = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspStopIssue = TRUE;
        }
    }
    else if (u4DspCmd == DSP_FLUSH_DEC4)
    {
        if ((_rDspStatus[AUD_DEC_4TH].fgDspStop) || (_rDspStatus[AUD_DEC_4TH].fgDspFlush))
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_4TH].fgDspFlush = TRUE;
            _rDspStatus[AUD_DEC_4TH].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspStopIssue = TRUE;
            _rDspStatus[AUD_DEC_4TH].fgDspPlay = FALSE;
        }
    }
    else if (u4DspCmd == DSP_PAUSE_DEC4)
    {
        AUD_UopCommandDone(AUD_DEC_4TH, DSP_PAUSE_DEC4);
        // assume DSP_RESUME is only issued in DSP_PAUSE state
        if (!_rDspStatus[AUD_DEC_4TH].fgDspPlay)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_4TH].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspPlay = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspPause = TRUE;
        }
    }
    else if (u4DspCmd == DSP_RESUME_DEC4)
    {
        AUD_UopCommandDone(AUD_DEC_4TH, DSP_RESUME_DEC4);

        if (!_rDspStatus[AUD_DEC_4TH].fgDspPause)
        {
            i4Ret = FALSE;
        }
        else
        {
            _rDspStatus[AUD_DEC_4TH].fgDspPtsSet = FALSE;
            _rDspStatus[AUD_DEC_4TH].fgDspPlay = TRUE;
            _rDspStatus[AUD_DEC_4TH].fgDspPause = FALSE;
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
    if (ucDecId == AUD_DEC_MAIN)
    {
        vWriteDspShmDWRD (D_DECODING_STR_PNT, u4ReadPtrDspInterAddr);
    }
    else
    {
    	vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC2, u4ReadPtrDspInterAddr);
    }
#else
  #ifdef CC_AUD_DEC3_USE_FIFO1 //ndef CC_AUD_SKYPE_SUPPORT
    // MAIN and THIRD both use Fifo1
    if (ucDecId == AUD_DEC_AUX)
    {
        vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC2, u4ReadPtrDspInterAddr);
    }
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (ucDecId != AUD_DEC_4TH)
#else
    else
#endif
    {
    	vWriteDspShmDWRD (D_DECODING_STR_PNT, u4ReadPtrDspInterAddr);
    }
  #else
if (ucDecId == AUD_DEC_MAIN)
    {
        vWriteDspShmDWRD (D_DECODING_STR_PNT, u4ReadPtrDspInterAddr);
    }
    else if (ucDecId == AUD_DEC_AUX)
    {
    	vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC2, u4ReadPtrDspInterAddr);
    }
    else if (ucDecId == AUD_DEC_THIRD)
    {
    	vWriteDspShmDWRD (D_DECODING_STR_PNT_DEC3, u4ReadPtrDspInterAddr);
    }
  #endif
#endif
}

/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
UINT32 u4DspGetSampleRate(UINT8 u1DecId)
{
    UINT32 dwSFreq;

    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    if (u1DecId == AUD_DEC_MAIN)
    {
        dwSFreq = dReadDspCommDram(ADDR_D2RC_RISC_INFO_SAMPLING_RATE);
    }
    else
    {
        dwSFreq = dReadDspCommDram(ADDR_D2RC_RISC_INFO_SAMPLING_RATE_DEC2);
    }

    dwSFreq = (dwSFreq & 0xff00) >> 8;
    return (dwSFreq);
}

/***************************************************************************
     Function :
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspBlockPtrReset(UINT8 u1DecId)
{
    UINT32 dwTryCnt = 0;
    BOOL fgDspPlay;
    UINT32 dwAFifo;
    UINT32 dwBufPNTReg;

    fgDspPlay = _rDspStatus[u1DecId].fgDspPlay;

    if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_THIRD))
    {
        dwAFifo = _u4AFIFO[AUD_DEC_MAIN][0];
        dwBufPNTReg = REG_DSP_BUF_PNT;
    }
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        dwAFifo = _u4AFIFO[AUD_DEC_4TH][0];
        dwBufPNTReg = REG_DSP_BUF_PNT_DEC4;
    }
#endif
    else
    {
        dwAFifo = _u4AFIFO[AUD_DEC_AUX][0];
        dwBufPNTReg = REG_DSP_BUF_PNT_DEC2;
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
    BOOL fgDspStop = FALSE;
    UINT32 dwAFifo;
    UINT32 dwBufPNTReg;
    BOOL fgRet = FALSE;
    CRIT_STATE_T rCritState;

    AUD_DEC_ID_VALIDATE(ucDecId);

    if (ucDecId < AUD_DEC_MAX)
    {
        fgDspStop = _rDspStatus[ucDecId].fgDspStop;
    }
    dwAFifo = PHYSICAL(u4Address);

    if (ucDecId == AUD_DEC_MAIN)
    {
        dwBufPNTReg = REG_DSP_BUF_PNT;
    }
    else if (ucDecId == AUD_DEC_AUX)
    {
        dwBufPNTReg = REG_DSP_BUF_PNT_DEC2;
    }
    else if (ucDecId == AUD_DEC_THIRD)
    {
        dwBufPNTReg = REG_DSP_BUF_PNT_DEC3;
    }
    else
    {
        dwBufPNTReg = REG_DSP_BUF_PNT_DEC4; //default value.
    }
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

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
void vSetSpeakerConfig(UINT8 u1DecId)
{
    //UINT16 u2DspData;
    UINT32 dwDspInfo;
//    UINT8 bStrType;
    UINT32 u4Addr;

    if (u1DecId == AUD_DEC_MAIN)
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
    WriteDspCommDram (u4Addr, dwDspInfo);
}

extern UINT32 fgAUD_DspProbeHalt(void);
extern UINT32 AUD_DspProbeWrite(UINT32 u4Addr, UINT32 u4Data);

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

    if (fgAUD_DspProbeHalt())
    {
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
        AUD_DspProbeWrite(u4Addr, (u4Value<<8));
        #else
        AUD_DspProbeWrite(u4Addr, u4Value);
        #endif
        return;
    }

    while (TRUE)
    {
        u4TryCnt++;
        rCritState = x_crit_start();
        if (!fgDspBusy ())
        {
            #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
            vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, (u4Addr<<8),((u4Value)<<8));
            #else
            vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, ((u4Value)),u4Addr);
            #endif
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

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))

/***************************************************************************
     Function : vWriteDspSram32
  Description : write dsp sram through interrupt
    Parameter : u4Addr: address,u4Value: value in 24 bits
    Return    :
***************************************************************************/
void vWriteDspSram32 (UINT32 u4Addr, UINT32 u4Value)
{
    UINT32 u4TryCnt = 0;
    CRIT_STATE_T rCritState;

    if (fgAUD_DspProbeHalt())
    {
        AUD_DspProbeWrite(u4Addr, u4Value);
        return;
    }

    while (TRUE)
    {
        u4TryCnt++;
        rCritState = x_crit_start();
        if (!fgDspBusy ())
        {
            vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, (u4Addr<<8),
				         ((u4Value)));
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
#endif

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
        #if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
		vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, (u4Addr<<8),((u4Value<<8)));
        #else
		vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, ((u4Value)),u4Addr);
        #endif
        fgRet = TRUE;
    }
    else
    {
        fgRet = FALSE;
    }

    return fgRet;
}

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
/***************************************************************************
     Function : vWriteDspSramInISR32
  Description : write dsp sram through interrupt in RISC ISR context
    Parameter : u4Addr: address,u4Value: value in 24 bits
    Return    :
***************************************************************************/
BOOL fgWriteDspSramInISR32 (UINT32 u4Addr, UINT32 u4Value)
{
    BOOL fgRet = TRUE;
    if (!fgDspBusy ())
    {
		vSendDspLongInt (INT_RC2D_WRITE_DSP_MEMORY, (u4Addr<<8),
						((u4Value)));
        fgRet = TRUE;
    }
    else
    {
        fgRet = FALSE;
    }

    return fgRet;
}
#endif

extern UINT32 fgAUD_DspProbeHalt(void);
extern UINT32 u4AUD_DspProbeRead(UINT32 u4Addr);

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
	 
	if (fgAUD_DspProbeHalt())
	{
		 u4Value = u4AUD_DspProbeRead(u4Addr);
	#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
		 u4Value >>= 8;
	#endif
		 return(u4Value);
	}

    vDSPLockReadSramSema();
	 
	while (TRUE)
	{
		 u4TryCnt++;
	 
		 if (!fgDspBusy ())
		 {
			 vSendDspLongInt (INT_RC2D_READ_DSP_MEMORY, ((u4Addr) << 8), 0);
			 break;
		 }
		 else
		 {
			 x_thread_delay(1);
			 if (u4TryCnt == DSP_TIMEOUT)
			 {
				 break;
			 }
		 }
	}
	x_thread_delay(1);
	u4Value = dwReadDspLongData();  //here may comes the racing condition ..we use sema to protect.
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
	u4Value >>= 8;
#endif

	vDSPUnlockReadSramSema();

    return(u4Value);
}

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
/***************************************************************************
     Function : u4ReadDspSram32
  Description : Read Dsp Sram through interrupt
    Parameter : u4Addr : address
    Return    : value in UIN32
***************************************************************************/
UINT32 u4ReadDspSram32(UINT32 u4Addr)
{
    UINT32 u4TryCnt = 0;
    UINT32 u4Value = 0;

    if (fgAUD_DspProbeHalt())
    {
        u4Value = u4AUD_DspProbeRead(u4Addr);
        return(u4Value);
    }

    vDSPLockReadSramSema();

    while (TRUE)
    {
        u4TryCnt++;

        if (!fgDspBusy ())
        {
            vSendDspLongInt (INT_RC2D_READ_DSP_MEMORY, ((u4Addr) << 8), 0);
            break;
        }
        else
        {
            x_thread_delay(1);
            if (u4TryCnt == DSP_TIMEOUT)
            {
                break;
            }
        }
    }
    x_thread_delay(1);
    u4Value = dwReadDspLongData();
	
	vDSPUnlockReadSramSema();
	
    return(u4Value);
}
#endif

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
    UINT32 u4IecChannelCfg0, u4IecChannelCfg1;
    UINT32 u4WordLen, u4IecFlag, u4CopyRight, u4CategoryCode, u4SampleRate;

    /* Select IEC output */
    u1ChSel = (uReadDspShmBYTE (B_IEC_PCMCH) & 0x07);
    if (u1ChSel == (UINT8)AUD_IEC_AUX)
    {
        u1IecDecSel = SPOUT_SPDF_SEL_AUX;
        u1ChSel = (UINT8)AUD_IEC_PCM_CH_L_R;
    }

    /* Set IEC driver registers */

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

    /* Set IEC output status */

    // get RAW or PCM information
    u4IecFlag = uReadDspShmBYTE (B_IECFLAG);

    // get word length
    switch (uReadDspShmBYTE (B_IEC_WORD_LENGTH))
    {
    case IEC_WORDLENGTH_24BITS:
    	u4WordLen = IEC_CFG0_WORDLENGTH_24BITS;
    	break;
    case IEC_WORDLENGTH_20BITS:
    	u4WordLen = IEC_CFG0_WORDLENGTH_20BITS;
    	break;
    case IEC_WORDLENGTH_16BITS:
    	u4WordLen = IEC_CFG0_WORDLENGTH_16BITS;
    	break;
    default:
    	u4WordLen = IEC_CFG0_WORDLENGTH_DEFAULT;
    	break;
    }

    // get category code
    u4CategoryCode = uReadDspShmBYTE (B_IEC_CATEGORY_CODE);

    // get sample rate, fix me!!!!
    if (u4IecFlag == AUD_IEC_CFG_RAW)
    {
        switch (u4FreqIdx)
        {
        case FS_32K:
    	    u4SampleRate = IEC_CFG1_SAMPLERATE_32K;
    	    break;
        case FS_44K:
    	    u4SampleRate = IEC_CFG1_SAMPLERATE_44K;
    	    break;
        default:
    	    u4SampleRate = IEC_CFG1_SAMPLERATE_48K;
    	    break;
        }
    }
    else
    {
        u4SampleRate = IEC_CFG1_SAMPLERATE_48K;
    }

    // get copy right (C-bit)
    u4CopyRight = (uReadDspShmBYTE (B_IEC_COPYRIGHT) & 0x1) << IEC_CFG1_CBIT_BIT;

    // set iec channel config 0
    if (u4IecFlag == AUD_IEC_CFG_RAW)
    {
        u4IecChannelCfg0 = (u4CategoryCode | IEC_CFG0_PCM_RAW_ISRAW | u4WordLen);
    }
    else
    {
        u4IecChannelCfg0 = (u4CategoryCode | IEC_CFG0_PCM_RAW_ISPCM | u4WordLen);
    }
    vWriteDspWORD (ADDR_RC2D_IEC_CHANNEL_CFG0, (UINT16) u4IecChannelCfg0);

    // set iec channel config 1
    if (u4IecFlag == AUD_IEC_CFG_RAW)
    {
        u4IecChannelCfg1 = (IEC_CFG1_DIGITAL_RAW | u4CopyRight | u4SampleRate);
    }
    else
    {
        u4IecChannelCfg1 = (IEC_CFG1_DIGITAL_PCM | u4CopyRight | u4SampleRate);
    }
    vWriteDspWORD (ADDR_RC2D_IEC_CHANNEL_CFG1, (UINT16) u4IecChannelCfg1);

    // set burst info
    vWriteDspWORD (ADDR_RC2D_IEC_BURST_INFO, u2ReadDspShmWORD (W_IEC_BURST_INFO));

    // set next sample number (nsnum)
    vWriteDspWORD (ADDR_RC2D_IEC_NSNUM, u2ReadDspShmWORD (W_IEC_NSNUM));

    vSetIecConfigReg(u1DecId, &rAudCfg);
}

/***************************************************************************
     Function : vDspSetFreq
  Description :
    Parameter :
    Return    :
***************************************************************************/
void vDspSetFreq (UINT8 u1DecId, UINT32 u4FreqIdx, BOOL fgResetDAC)
{
    UINT32 u4TargetADCFmt;

    u4TargetADCFmt = u4FreqIdx;
    vSetIecConfig(u1DecId, u4FreqIdx);
    if (fgResetDAC)
    {
        vDspAdacFmt (u1DecId, u4TargetADCFmt);
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
void vResetDecInfo (UINT8 u1DecId)
{
    UINT32 i;

    if (u1DecId == AUD_DEC_MAIN)
    {
        // first decoder
        for (i = 0; i < DSP_D2RC_INFO_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_D2RC_DECODING_INFO_BASE + i, 0);
        }

        // Don't clear this information in EU model
        // due to Main and Aux decoder use the same address
#ifndef CC_MT5391_AUD_3_DECODER
        for (i = 0; i < DSP_RC2D_SETTING_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_DECODER_INFO_BASE + i, 0);
        }
#endif

        // Initial ctrl page 3 for first decoder
        for (i = 0; i < DSP_RC2D_CTRL3_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_CTRL3_BASE + i, 0);
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        for (i = 0; i < DSP_D2RC_INFO_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_D2RC_DECODING_INFO_BASE_DEC3 + i, 0);
        }
        for (i = 0; i < DSP_RC2D_SETTING_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_DECODER_INFO_BASE_DEC3 + i, 0);
        }
        // Initial ctrl page 6 dec part for third decoder
        for (i = 0; i < DSP_RC2D_CTRL6_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_CTRL6_DEC_BASE + i, 0);
        }
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        for (i = 0; i < DSP_D2RC_INFO_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_D2RC_DECODING_INFO_BASE_DEC4 + i, 0);
        }
        for (i = 0; i < DSP_RC2D_SETTING_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_DECODER_INFO_BASE_DEC4 + i, 0);
        }
        // Initial ctrl page 7 for 4th decoder
        for (i = 0; i < DSP_RC2D_CTRL7_AREA_SZ; i++)
        {
            WriteDspCommDram (ADDR_RC2D_CTRL7_DEC_BASE + i, 0);
        }
    }
#endif
    else
    {   // second decoder
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
void vDspDecDes(UINT8 u1DecId)
{
    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteDspShmBYTE (W_SPEED, 0x0);
    }
    else
    {
    }
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

    if (ptADSPErrLog != NULL)
    {
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
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
            ptADSPErrLog->dwInputUnderRunDec3 = dReadDspCommDram(ADDR_D2RC_INPUT_UNDERRUN_DEC3);
            ptADSPErrLog->dwOutputUnderRun = dReadDspCommDram(ADDR_D2RC_OUTPUT_UNDERRUN);
            ptADSPErrLog->dwOutputUnderRunDec2 = dReadDspCommDram(ADDR_D2RC_OUTPUT_UNDERRUN_DEC2);
            ptADSPErrLog->dwOutputUnderRunDec3 = dReadDspCommDram(ADDR_D2RC_OUTPUT_UNDERRUN_DEC3);
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
        DSPF_PNOISE_SA,                     // AUD_PINK_DEC1
        DSPF_NTSC_DEC1_SA,	                // AUD_NTSC_DEC1
        DSPF_PAL_DEC1_SA,                   // AUD_PAL_DEC1
        DSPF_MINER_DEC1_SA,                 // AUD_MINER_DEC1
        DSPF_A2_DEC1_SA,                    // AUD_A2DEC_DEC1
        DSPF_DETECTOR_DEC1_SA,              // AUD_DETECTOR_DEC1
        DSPF_FMFM_DEC1_SA,                  // AUD_FMFM_DEC1
        DSPF_LPCM_DEC1_SA,                  // DSPF_LPCM_DEC1_SA
        DSPF_VORBIS_DEC1_SA,                // AUD_VORBISDEC1, CC_VORBIS_SUPPORT
        DSPF_AMRDEC1_SA,                    // AUD_AMR
    };

    UINT32 u4AudSaDec2[] =
    {
        DSPF_AC3_DEC2_SA,                   // AUD_AC3_DEC2
        DSPF_MP2_DEC2_SA,                   // AUD_MPEG_DEC2
        DSPF_AAC_DEC2_SA,                   // AUD_AAC_DEC2
        DSPF_PCM_DEC2_SA,                   // AUD_PCM_DEC2
        DSPF_NTSC_DEC2_SA,                  // AUD_NTSC_DEC2
        DSPF_PAL_DEC2_SA,                   // AUD_PAL_DEC2
        DSPF_MINER_DEC2_SA,                 // AUD_MINER_DEC2
        DSPF_A2_DEC2_SA,                    // AUD_A2DEC_DEC2
        DSPF_DETECTOR_DEC2_SA,              // AUD_DETECTOR_DEC2
        DSPF_FMFM_DEC2_SA,                  // AUD_FMFM_DEC2
        DSPF_MP3_DEC2_SA,                   // AUD_MP3_DEC2
        DSPF_VORBIS_DEC2_SA,                // AUD_VORBISDEC2, CC_VORBIS_SUPPORT
    };

#ifdef CC_MT5391_AUD_3_DECODER
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
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_ROM_ICACHE);

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
    if(AUD_IsSupportSRS())
    {
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_SRS_TSHD_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_SRS_TSHD_SZ << 2));
    }
    else
    {
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_VSURR_POST_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_VSURR_POST_SZ << 2));
    }

    puFlashRead = (UINT8*) u4Adr;

    // Get DRAM address
    u4Adr = (dwGetDspIcacheAdr(TYPE_POST_ICACHE) + VIRTUAL_SURROUND_PRAM_ADR) * 4;
    puDramRead = (UINT8*)(u4Adr);
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_POST_ICACHE);

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

#ifdef DSP_SUPPORT_SRSVIQ
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_VIQ_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_VIQ_SZ << 2));

    puFlashRead = (UINT8*) u4Adr;

    // Get DRAM address
    u4Adr = (dwGetDspIcacheAdr(TYPE_POST_ICACHE) + VOICE_ENHANCEMENT_PRAM_ADR) * 4;
    puDramRead = (UINT8*)(u4Adr);
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_POST_ICACHE);

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
#endif // DSP_SUPPORT_SRSVIQ

#ifdef CC_AUD_BBE_SUPPORT
    u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_BBE_VIVA4_SA << 2));
    u4Adr = u4FlashStart + (u4FlashOfst * 4);
    u4Size = u4GetFlashData (u4FlashStart + (DSPF_BBE_VIVA4_SZ << 2));

    puFlashRead = (UINT8*) u4Adr;

    // Get DRAM address
    u4Adr = (dwGetDspIcacheAdr(TYPE_POST_ICACHE) + SPEAKER_ENHANCEMENT_PRAM_ADR) * 4;
    puDramRead = (UINT8*)(u4Adr);
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_POST_ICACHE);

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
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_POST_ICACHE);

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
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_COMM_ICACHE);

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
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_DEC3_ICACHE);

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
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_DEC2_ICACHE);
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
    u4Adr = dwGetDspIcacheAdr(TYPE_CTR_ICACHE) * 4;
    puDramRead = (UINT8*)(u4Adr);
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_CTR_ICACHE);

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
    if (((_rDspVars[AUD_DEC_MAIN].dwDspRamCodeType - TYPE_AUD_DEC1) <= DECODER1_MAX_INDEX))
    {
        // Get section's start address and size in Flash
        u4FlashCodIndx = u4AudSaDec1[_rDspVars[AUD_DEC_MAIN].dwDspRamCodeType - TYPE_AUD_DEC1];
        u4FlashOfst = u4GetFlashData (u4FlashStart + (u4FlashCodIndx << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + ((u4FlashCodIndx + 1) << 2));

        // Get DRAM address
        u4Adr = dwGetDspIcacheAdr(TYPE_DEC1_ICACHE) * 4;
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_ICACHE, TYPE_DEC1_ICACHE);
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
                *pu4DspTableStatus |= u4CheckDspDramTable(_rDspVars[AUD_DEC_MAIN].dwDspRamCodeType);
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
    case AUD_AAC_DEC3:
#endif
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_AAC_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_AAC_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + u4TabAddr;
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCacheFree(u4Adr, u4Size*4);
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
    case AUD_AC3_DEC3:
#endif
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_AC3_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_AC3_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspCmptBuf + u4TabAddr;
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCacheFree(u4Adr, u4Size*4);
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
    case AUD_MPEG_DEC3:
#endif
        // Flash setting
        u4FlashOfst = u4GetFlashData (u4FlashStart + (DSPF_MP2_TABLE_SA << 2));
        u4Adr = u4FlashStart + (u4FlashOfst * 4);
        puFlashRead = (UINT8*) u4Adr;
        u4Size = u4GetFlashData (u4FlashStart + (DSPF_MP2_TABLE_SZ << 2));
        // DRAM setting
        u4Adr = u4DspBuf + u4TabAddr;
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCacheFree(u4Adr, u4Size*4);
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
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCacheFree(u4Adr, u4Size*4);
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
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCacheFree(u4Adr, u4Size*4);
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
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCacheFree(u4Adr, u4Size*4);
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
        puDramRead = (UINT8*)(u4Adr);
        DSP_FlushInvalidateDCacheFree(u4Adr, u4Size*4);
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
BOOL fgDspDataComp(UINT8 *pu1Adr1, UINT8 *pu1Adr2, UINT32 u4Size)
{
    BOOL fgMathFlag = TRUE;                 // Match flag
    UINT32 u4Index;                         // Loop index
    UINT8* puAdr1=VIRTUAL(pu1Adr1);
    UINT8* puAdr2=VIRTUAL(pu1Adr2);

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
    //DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    DSP_FlushInvalidateDCacheSmall(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX,
                                    ((ADDR_D2RC_RISC_INFO_APUT_BANK_DEC1 + u1DecId)<<2), 1);

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
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

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
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    if (u1DecId == AUD_DEC_MAIN)
    {
        return (UINT32)dReadDspCommDram(ADDR_D2RC_RISC_INFO_REG_BUF_PNT);
    }
    else
    {
        return (UINT32)dReadDspCommDram(ADDR_D2RC_RISC_INFO_REG_BUF_PNT_DEC2);
    }
}

//-----------------------------------------------------------------------------
/** u4ReadD2RInfo_StcDiff
 *
 *
 *  @param
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4ReadD2RInfo_StcDiff(UINT8 u1DecId)
{
    UINT32 u4Addr;

    UNUSED(u4ReadD2RInfo_StcDiff);
    u4Addr = ADDR_RC2D_STC_DIFF;

    if (u1DecId == AUD_DEC_AUX)
    {
        u4Addr = ADDR_RC2D_STC_DIFF_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Addr = ADDR_RC2D_STC_DIFF_DEC3;
    }

    DSP_FlushInvalidateDCacheSmall(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX,
                                    ((u4Addr)<<2), 1);

    return (UINT32)dReadDspCommDram(u4Addr);
}


