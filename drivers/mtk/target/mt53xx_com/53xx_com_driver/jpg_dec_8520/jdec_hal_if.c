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
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN
* "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER
* DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF
* ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE
* MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR
* ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION OR TO CONFORM TO
* A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
* LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR
* REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK
* FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH
* THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS
* PRINCIPLES.
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: vsv_input_api.c $
 *
 * Project:
 * --------
 *   MT8105
 *
 * Description:
 * ------------
 *    Jpeg Decoder HAL
 *
 * Author: C.K. Hu
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 03/12/18 3:29p $
 *
 * $Revision: #1 $
****************************************************************************/
// *********************************************************************
// Memo
// *********************************************************************
/*

*/
// *********************************************************************
// TODO
// *********************************************************************
/*
*/
#include "x_hal_5381.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "jdec_hal_if.h"
#include "jdec_hal_errcode.h"
#include "jdec_hal.h"
#include "jpg_if.h"
#include "x_hal_926.h"

#define RO_VLD_VBAR         0xEC
#define RO_VLD_VRPTR        0xFC
#define RW_VLD_BITCOUNT     0x114
#define RW_VLD_WMV_ABS      0x23C


extern UINT32 u4VDecReadVldRPtr(UINT32 u4BSID, UINT32 u4VDecID, UINT32 *pu4Bits, UINT32 u4VFIFOSa);

// Work around for 16M limitation
static BOOL _fgJdecHalAllocate = FALSE;
static void *_pvJdecHalNoneZeroHistoryBuf[3];
static UINT32 _u4JdecHalOutCoefYBufSa[2];
static UINT32 _u4JdecHalOutCoefCbBufSa[2];
static UINT32 _u4JdecHalOutCoefCrBufSa[2];


typedef struct {
    JDEC_HAL_DEC_MODE_T eDecodingMode;          ///< Decoding Mode.
    JDEC_HAL_PIC_INFO_T rPicInfo;               ///< Input Jpeg picture information.
    JDEC_JFIF_SOS_T rScanInfo;                  ///< Input Jpeg picture Scan Infomation
//    JPEG_JFIF_SOS_T rScanInfo;                  ///< Input Jpeg picture Scan Infomation
    JDEC_HAL_INPUT_INFO_T rInputInfo;           ///< Input fifo information
    JDEC_HAL_OUTPUT_INFO_T rOutputInfo;         ///< Output buffer information
    BOOL fgDecoded;                             ///< If decoded after initialization.
    UINT32 u4CurrMCUNum;                        ///< Current MCU Number.
    void *pvNoneZeroHistoryBuf[3];              ///< None Zero History Buffer.  Working buffer for Progressive Scan.
    UINT32 u4RestartIntervalCnt;                ///< Restart Internal Counter.
    UINT32 u4EOBRUN;                            ///< remaining EOBs in EOBRUN
    BOOL fgEOBRUNSkip;                          ///< Current MCU skip by EOBRUN
    UINT8 au1CompNumOfBlkInMCU[10];             ///< The component number of each block in one MCU.
    BOOL fgStartOfScan;                         ///< Reach Start of Scan
    UINT32 u4OutCoefYBufSa;                     ///< Jpeg output progressive coefficient buffer Y start address
    UINT32 u4OutCoefCbBufSa;                    ///< Jpeg output progressive coefficient buffer Cb start address
    UINT32 u4OutCoefCrBufSa;                    ///< Jpeg output progressive coefficient buffer Cr start address
    BOOL fgReachMarker;
    BOOL fgReinit;                              ///< Re-initialization flag.
    JDEC_HAL_HW_STATUS_T rHwStatus;             ///< Hardware status for re-initialization.
#ifdef CC_MT5363
    UINT8 au1HwDcHuffTbl[4];		    	
	UINT8 au1HwAcHuffTbl[4];             
#else
	UINT8 au1HwDcHuffTbl[1024 * 4];		    	///< HW format DC Huffman Table
	UINT8 au1HwAcHuffTbl[1024 * 4];             ///< HW format AC Huffman Table
#endif	
    UINT8 au1HwHuffTblAlign[15];                ///< For HW format Huffman Table align to 16 bytes.
} JDEC_HAL_INFO_T;


JDEC_HAL_INFO_T *_prJdecHalInfo[HW_JDEC_VLD_NUM];   // HAL local information for each HW.


void vHwClkVDecSwitchJpeg(void)
{
    // set mpg & jpg clock
#if defined(CC_MT5387) && !defined(CC_MT5363)        
   	IO_WRITE32(CKGEN_BASE, 0x224, 0x5);//sys 200
#elif defined(JPG_CLK_FROM_GRAPHIC)
    IO_WRITE32(CKGEN_BASE, 0x230, 0x1);//sys 240
#else                                
    IO_WRITE32(CKGEN_BASE, 0x224, 0x4);  //sys 200
#endif
}


/// Initialize Jpeg decoder HAL
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Init(
    UINT32 u4JDecID                                     ///< [IN] Jpeg decoder hardware ID
)
{
    //Printf("i4JDEC_HAL_Init()\n");
    
    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    // Set clock for Jpeg.
    vHwClkVDecSwitchJpeg();
//#ifndef CC_MT5392B
    // Power on VLD
    vHwJdecVLDPowerOn(u4JDecID,TRUE);
//#endif
    // VLD reset.
    vHwJdecHwReset(u4JDecID);

    // Clear SRAM
    vHwJdecSRAMClear(u4JDecID);

    vHwJdecSetVLDBarrelShifterToJpegMode(u4JDecID,TRUE);

    // Adjust this register to correctly decode.
    vHwJdecSetMiscReg(u4JDecID);

    // Set output to color buffer as raster scan mode.
    vHwJdecSetReadWriteOutputBufMode(u4JDecID);

    // Set VLD wait time out value.
    vHwJdecSetVLDWaitTimeoutValue(u4JDecID,0x3000);

#if 0
    // Variable initialization
    _prJdecHalInfo[u4JDecID] = (JDEC_HAL_INFO_T *)BSP_AllocAlignedDmaMemory(sizeof(JDEC_HAL_INFO_T),2048);
    x_memset(_prJdecHalInfo[u4JDecID],0,sizeof(JDEC_HAL_INFO_T));
#else
    if(!_fgJdecHalAllocate)
    {
        _prJdecHalInfo[0] = (JDEC_HAL_INFO_T *)BSP_AllocAlignedDmaMemory(sizeof(JDEC_HAL_INFO_T),2048);
        x_memset(_prJdecHalInfo[0],0,sizeof(JDEC_HAL_INFO_T));

        _prJdecHalInfo[1] = (JDEC_HAL_INFO_T *)BSP_AllocAlignedDmaMemory(sizeof(JDEC_HAL_INFO_T),2048);
        x_memset(_prJdecHalInfo[1],0,sizeof(JDEC_HAL_INFO_T));

#ifdef CHIPVERIFY
        _pvJdecHalNoneZeroHistoryBuf[0] = (void *)0x2bbc800;
        _pvJdecHalNoneZeroHistoryBuf[1] = (void *)0x2be4800;
        _pvJdecHalNoneZeroHistoryBuf[2] = (void *)0x2c0c800;
        x_memset(_pvJdecHalNoneZeroHistoryBuf[0],0, 0x28000);
        x_memset(_pvJdecHalNoneZeroHistoryBuf[1],0, 0x28000);
        x_memset(_pvJdecHalNoneZeroHistoryBuf[2],0, 0x28000);
        _u4JdecHalOutCoefYBufSa[0] = 0x2c34800;
        _u4JdecHalOutCoefCbBufSa[0] = 0x2eb4800;
        _u4JdecHalOutCoefCrBufSa[0] = 0x3134800;   
        x_memset((void *)_u4JdecHalOutCoefYBufSa[0], 0, 0x280000);
        x_memset((void *)_u4JdecHalOutCoefCbBufSa[0], 0, 0x280000);
        x_memset((void *)_u4JdecHalOutCoefCrBufSa[0], 0, 0x280000);
#else
        {
            UINT32 u4Comp;

            for(u4Comp=0; u4Comp<3; u4Comp++)
            {
                _pvJdecHalNoneZeroHistoryBuf[u4Comp] =
                    BSP_AllocAlignedDmaMemory(2000 * 2000/8,1);
                x_memset(_pvJdecHalNoneZeroHistoryBuf[u4Comp],0,2000 * 2000/8);
            }
        }

        _u4JdecHalOutCoefYBufSa[0] =
            BSP_AllocAlignedDmaMemory(2000 * 2000 * 2,2048);
        x_memset(_u4JdecHalOutCoefYBufSa[0],0,
                                   2000 * 2000 * 2);

        _u4JdecHalOutCoefCbBufSa[0] =
            BSP_AllocAlignedDmaMemory(2000 * 2000 * 2,2048);
        x_memset(_u4JdecHalOutCoefCbBufSa[0],0,
                                   2000 * 2000 * 2);

        _u4JdecHalOutCoefCrBufSa[0] =
            BSP_AllocAlignedDmaMemory(2000 * 2000 * 2,2048);
        x_memset(_u4JdecHalOutCoefCrBufSa[0],0,
                                   2000 * 2000 * 2);

        _u4JdecHalOutCoefYBufSa[1] =
            BSP_AllocAlignedDmaMemory(2000 * 2000 * 2,2048);
        x_memset(_u4JdecHalOutCoefYBufSa[0],0,
                                   2000 * 2000 * 2);

        _u4JdecHalOutCoefCbBufSa[1] =
            BSP_AllocAlignedDmaMemory(2000 * 2000 * 2,2048);
        x_memset(_u4JdecHalOutCoefCbBufSa[0],0,
                                   2000 * 2000 * 2);

        _u4JdecHalOutCoefCrBufSa[1] =
            BSP_AllocAlignedDmaMemory(2000 * 2000 * 2,2048);
        x_memset(_u4JdecHalOutCoefCrBufSa[0],0,
                                   2000 * 2000 * 2);
#endif
        _fgJdecHalAllocate = TRUE;
    }

    x_memset(_prJdecHalInfo[u4JDecID],0,sizeof(JDEC_HAL_INFO_T));
#endif

    return S_JDEC_HAL_OK;
}


/// Uninitialize Jpeg decoder HAL
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Uninit(
    UINT32 u4JDecID                                     ///< [IN] Jpeg decoder hardware ID
)
{
    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

#if 0
    // Free none zero history buffer.
    if(_prJdecHalInfo[u4JDecID]->rPicInfo.prSofInfo->eJpegFormat == E_JPG_PROGRESSIVE_HUFFMAN)
    {
        UINT32 u4Comp;

        for(u4Comp=0; u4Comp<3; u4Comp++)
        {
            if(_prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp] != NULL)
            {
                x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp]);
                _prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp] = NULL;
            }
        }

        if(_prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa != NULL)
        {
            x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa);
            _prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa = NULL;
        }

        if(_prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa != NULL)
        {
            x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa);
            _prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa = NULL;
        }

        if(_prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa != NULL)
        {
            x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa);
            _prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa = NULL;
        }

    }

    x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]);
#endif


    vHwJdecSetVLDBarrelShifterToJpegMode(u4JDecID,FALSE);

    // Software reset VLD for other module
    vHwJdecHwReset(u4JDecID);

    // Power off
    vHwJdecVLDPowerOn(u4JDecID,FALSE);

    return S_JDEC_HAL_OK;
}


/// Set Jpeg decoding mode
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Decoding_Mode(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_DEC_MODE_T eDecodingMode                   ///< [IN] Jpeg decoding mode
)
{
    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    _prJdecHalInfo[u4JDecID]->eDecodingMode = eDecodingMode;

    return S_JDEC_HAL_OK;
}


/// Set Jpeg input picture information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Pic_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_PIC_INFO_T *prPicInfo                      ///< [IN] Jpeg input picture information
)
{
    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    x_memcpy(&(_prJdecHalInfo[u4JDecID]->rPicInfo),prPicInfo,sizeof(JDEC_HAL_PIC_INFO_T));

#if 0
    // Allocate none zero history buffer when picture size is update.
    if(_prJdecHalInfo[u4JDecID]->rPicInfo.prSofInfo->eJpegFormat == E_JPG_PROGRESSIVE_HUFFMAN)
    {
        UINT32 u4Comp;

        for(u4Comp=0; u4Comp<3; u4Comp++)
        {
            if(_prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp] != NULL)
                x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp]);
        }

        if(_prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa != NULL)
            x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa);
        if(_prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa != NULL)
            x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa);
        if(_prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa != NULL)
            x_free_aligned_nc_mem(_prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa);

        {
            UINT32 u4Width,u4Height;
            UINT32 u4WidthPerMCU,u4HeightPerMCU;

            for(u4Comp=0; u4Comp<3; u4Comp++)
            {
                u4WidthPerMCU = (_prJdecHalInfo[u4JDecID]->rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor /
                                 _prJdecHalInfo[u4JDecID]->rPicInfo.prSofInfo->arSofComp[u4Comp].u1HSampFactor) * 8;
                u4HeightPerMCU = (_prJdecHalInfo[u4JDecID]->rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor /
                                  _prJdecHalInfo[u4JDecID]->rPicInfo.prSofInfo->arSofComp[u4Comp].u1VSampFactor) * 8;
                u4Width = ((_prJdecHalInfo[u4JDecID]->rPicInfo.prSofInfo->u2ImageWidth + u4WidthPerMCU - 1) / u4WidthPerMCU) * u4WidthPerMCU;
                u4Height = ((_prJdecHalInfo[u4JDecID]->rPicInfo.prSofInfo->u2ImageHeight + u4HeightPerMCU - 1) / u4HeightPerMCU) * u4HeightPerMCU;
                _prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp] =
                    BSP_AllocAlignedDmaMemory(u4Width * u4Height/8,1);
                x_memset(_prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp],0,u4Width * u4Height/8);
            }
        }

        _prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa =
            BSP_AllocAlignedDmaMemory(prPicInfo->prSofInfo->u2ImageWidth * prPicInfo->prSofInfo->u2ImageHeight * 2,2048);
        x_memset(_prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa,0,
                                   prPicInfo->prSofInfo->u2ImageWidth * prPicInfo->prSofInfo->u2ImageHeight * 2);

        _prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa =
            BSP_AllocAlignedDmaMemory(prPicInfo->prSofInfo->u2ImageWidth * prPicInfo->prSofInfo->u2ImageHeight * 2,2048);
        x_memset(_prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa,0,
                                   prPicInfo->prSofInfo->u2ImageWidth * prPicInfo->prSofInfo->u2ImageHeight * 2);

        _prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa =
            BSP_AllocAlignedDmaMemory(prPicInfo->prSofInfo->u2ImageWidth * prPicInfo->prSofInfo->u2ImageHeight * 2,2048);
        x_memset(_prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa,0,
                                   prPicInfo->prSofInfo->u2ImageWidth * prPicInfo->prSofInfo->u2ImageHeight * 2);
    }
#else
        {
            UINT32 u4Comp;

            for(u4Comp=0; u4Comp<3; u4Comp++)
            {
                _prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp] = _pvJdecHalNoneZeroHistoryBuf[u4Comp];
                //x_memset(_prJdecHalInfo[u4JDecID]->pvNoneZeroHistoryBuf[u4Comp],0,2000 * 2000/8);
            }
        }

        _prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa = _u4JdecHalOutCoefYBufSa[u4JDecID];
//        x_memset(_prJdecHalInfo[u4JDecID]->u4OutCoefYBufSa,0,
    //                               2000 * 2000 * 2);

        _prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa = _u4JdecHalOutCoefCbBufSa[u4JDecID];
        //x_memset(_prJdecHalInfo[u4JDecID]->u4OutCoefCbBufSa,0,
            //                       2000 * 2000 * 2);

        _prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa = _u4JdecHalOutCoefCrBufSa[u4JDecID];
//        x_memset(_prJdecHalInfo[u4JDecID]->u4OutCoefCrBufSa,0,
    //                               2000 * 2000 * 2);
#endif

    return S_JDEC_HAL_OK;
}


/// Set Huffman Table information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_HuffmanTable_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_JFIF_DHT_T *prHuffTblInfo                      ///< [IN] Huffman Table information
)
{
    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    //x_memcpy(_prJdecHalInfo[u4JDecID]->rPicInfo.prHuffTblInfo,prHuffTblInfo,sizeof(JDEC_JFIF_DHT_T));

    return S_JDEC_HAL_OK;
}


/// Set Jpeg scan information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Scan_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JPEG_JFIF_SOS_T *prScanInfo                         ///< [IN] Input Jpeg scan information
)
{
    UINT32 i;

    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

//    x_memcpy(&(_prJdecHalInfo[u4JDecID]->rScanInfo),prScanInfo,sizeof(JDEC_JFIF_SOS_T));

    _prJdecHalInfo[u4JDecID]->rScanInfo.u1CompInScan = prScanInfo->u1CompInScan;

    for (i = 0; i < D_MAX_JPEG_COMPONENT; i++)
    {
        _prJdecHalInfo[u4JDecID]->rScanInfo.au1CompId[i] = prScanInfo->au1CompId[i];
        _prJdecHalInfo[u4JDecID]->rScanInfo.au1DcId[i] = prScanInfo->au1DcId[i];
        _prJdecHalInfo[u4JDecID]->rScanInfo.au1AcId[i] = prScanInfo->au1AcId[i];
    }
    
    _prJdecHalInfo[u4JDecID]->rScanInfo.u1Ss = prScanInfo->u1Ss;
    _prJdecHalInfo[u4JDecID]->rScanInfo.u1Se = prScanInfo->u1Se;
    _prJdecHalInfo[u4JDecID]->rScanInfo.u1Ah = (prScanInfo->u1AhAl & 0xf0) >> 4;
    _prJdecHalInfo[u4JDecID]->rScanInfo.u1Al = (prScanInfo->u1AhAl & 0x0f);
	
    _prJdecHalInfo[u4JDecID]->u4EOBRUN = 0;
    _prJdecHalInfo[u4JDecID]->u4RestartIntervalCnt = 0;
    _prJdecHalInfo[u4JDecID]->fgStartOfScan = TRUE;
    _prJdecHalInfo[u4JDecID]->fgReachMarker = FALSE;

    return S_JDEC_HAL_OK;
}


/// Set Jpeg input information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Input_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_INPUT_INFO_T *prInputInfo                  ///< [IN] Input Jpeg input information
)
{
    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    x_memcpy(&(_prJdecHalInfo[u4JDecID]->rInputInfo),prInputInfo,sizeof(JDEC_HAL_INPUT_INFO_T));

    return S_JDEC_HAL_OK;
}


/// Set Jpeg output information
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Output_Info(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_OUTPUT_INFO_T *prOutputInfo                ///< [IN] Input Jpeg output information
)
{
    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    x_memcpy(&(_prJdecHalInfo[u4JDecID]->rOutputInfo),prOutputInfo,sizeof(JDEC_HAL_OUTPUT_INFO_T));

    return S_JDEC_HAL_OK;
}


/// Trigger Jpeg HAL decoding
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Decode(
    UINT32 u4JDecID,                                     ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_DEC_PROG_T eFlow                           ///< [IN] Jpeg flow, driver uses row buffer
)
{
    JDEC_HAL_INFO_T *prJdecHalInfo;
    INT32 i4Ret;

    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    prJdecHalInfo = _prJdecHalInfo[u4JDecID];

    // EOBRUN skip
    if((prJdecHalInfo->rScanInfo.u1Ah == 0) && (prJdecHalInfo->rScanInfo.u1Ss != 0) && // AC first
       (prJdecHalInfo->u4EOBRUN != 0) && // EOBRUN
       !((prJdecHalInfo->rScanInfo.u1Se == 63) && (prJdecHalInfo->rScanInfo.u1Al == 0))) // Not Last Scan
    {
        // 8. Status Update
        UINT32 u4YBufSa,u4CbBufSa,u4CrBufSa;
        UINT32 u4YCoeffBufSa,u4CbCoeffBufSa,u4CrCoeffBufSa;

        i4HwJdecCalculateOutputBufAddr(u4JDecID,prJdecHalInfo->u4CurrMCUNum,
                                                prJdecHalInfo->rPicInfo.prSofInfo,
                                                &(prJdecHalInfo->rScanInfo),
                                                &(prJdecHalInfo->rOutputInfo),
                                                prJdecHalInfo->u4OutCoefYBufSa,
                                                prJdecHalInfo->u4OutCoefCbBufSa,
                                                prJdecHalInfo->u4OutCoefCrBufSa,
                                                &u4YBufSa,
                                                &u4CbBufSa,
                                                &u4CrBufSa,
                                                &u4YCoeffBufSa,
                                                &u4CbCoeffBufSa,
                                                &u4CrCoeffBufSa, eFlow);
        prJdecHalInfo->u4CurrMCUNum++;
        prJdecHalInfo->u4EOBRUN--;
        prJdecHalInfo->fgEOBRUNSkip = TRUE;
        return S_JDEC_HAL_OK;
    } else
    {
        prJdecHalInfo->fgEOBRUNSkip = FALSE;
    }

    // Initialize setting
    if((!prJdecHalInfo->fgDecoded) || prJdecHalInfo->fgReinit)
    {
        // 0. Buffer clean
        if(!prJdecHalInfo->fgDecoded)
        {
            if((prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT) ||
               (prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE))
            {
                UINT32 u4Comp;
                UINT32 u4Width,u4Height;
                UINT32 u4Tmp;
                UINT32 u4CleanSize;

                u4Tmp = (UINT32)(prJdecHalInfo->rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor * 8);
                u4Width = ((prJdecHalInfo->rPicInfo.prSofInfo->u2ImageWidth + (u4Tmp - 1)) / u4Tmp) * u4Tmp;
                u4Tmp = (UINT32)(prJdecHalInfo->rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor * 8);
                u4Height = ((prJdecHalInfo->rPicInfo.prSofInfo->u2ImageHeight + (u4Tmp - 1)) / u4Tmp) * u4Tmp;

                // Clean none-zero history buffer
                for(u4Comp=0; u4Comp<3; u4Comp++)
                {
                    x_memset((void *)VIRTUAL(prJdecHalInfo->pvNoneZeroHistoryBuf[u4Comp]),0,
                                               (u4Width * u4Height + 7)/8);
                    
                    #if defined(CC_MT5365) || defined(CC_MT5395)
                    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL((prJdecHalInfo->pvNoneZeroHistoryBuf[u4Comp])), (UINT32)((u4Width * u4Height + 7)/8));
                    #endif
                }


                if (prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT)
                {
                    u4CleanSize = (u4Width * u4Height) << 1;
                }
                else
                {
                    u4CleanSize = (u4Width * prJdecHalInfo->rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor << 3) << 1;                    
                }
                // Clean coefficient buffer
                //if(!_fgJdecHalResume)
                    x_memset(VIRTUAL((void *)prJdecHalInfo->u4OutCoefYBufSa),0,
                                           u4CleanSize);

                //if(!_fgJdecHalResume)
                    x_memset(VIRTUAL((void *)prJdecHalInfo->u4OutCoefCbBufSa),0,
                                           u4CleanSize);

                //if(!_fgJdecHalResume)
                    x_memset(VIRTUAL((void *)prJdecHalInfo->u4OutCoefCrBufSa),0,
                                           u4CleanSize);
                
                #if defined(CC_MT5365) || defined(CC_MT5395)
                    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((prJdecHalInfo->u4OutCoefYBufSa)), (UINT32)u4CleanSize);
                    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((prJdecHalInfo->u4OutCoefCbBufSa)), (UINT32)u4CleanSize);
                    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((prJdecHalInfo->u4OutCoefCrBufSa)), (UINT32)u4CleanSize);                
                #else
                    HalFlushInvalidateDCache();
                #endif
                
            }
        }

        // 1. Set Decodeing Mode to HW
        vHwJdecSetDecodeMode(u4JDecID,prJdecHalInfo->eDecodingMode);

        // 2. Set Picture Information to HW
        // Set picture information
        switch((ENUM_JPEG_FORMAT_T)prJdecHalInfo->rPicInfo.prSofInfo->eJpegFormat)
        {
            case E_JPG_BASELINE:
            case E_JPG_EX_SEQ_HUFFMAN:
                vHwJdecSetDecodeModeBaseline(u4JDecID);
                break;
            case E_JPG_PROGRESSIVE_HUFFMAN:
                vHwJdecSetDecodeModeProgressive(u4JDecID);
                break;
            default:
                return E_JDEC_HAL_FAIL;
        }

        switch(prJdecHalInfo->eDecodingMode)
        {
            case JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW:
            case JDEC_HAL_DEC_MODE_BASELINE_PIC:
                // Set Picture Size
                vHwJdecSetPictureSize(u4JDecID,prJdecHalInfo->rPicInfo.prSofInfo);

                // Restart Interval Processing.
                if(prJdecHalInfo->rPicInfo.u4RestartInterval>0)
                {
                    vHwJdecSetRestartInterval(u4JDecID,prJdecHalInfo->rPicInfo.u4RestartInterval);
                }
                break;
        }

        // Set Huffman Table
        if((i4Ret = i4HwJdecSetHuffmanTable(u4JDecID,prJdecHalInfo->rPicInfo.prHuffTblInfo,
                                           (UINT8 *)((((UINT32)prJdecHalInfo->au1HwDcHuffTbl+15)>>4)<<4),
                                           (UINT8 *)((((UINT32)prJdecHalInfo->au1HwAcHuffTbl+15)>>4)<<4))) < 0)
            return i4Ret;

        // Set Quantization Table
        if((i4Ret = i4HwJdecSetQuantTable(u4JDecID,prJdecHalInfo->rPicInfo.prQTblInfo)) < 0)
            return i4Ret;

        // Initialize Restart Interval Counter
        if(prJdecHalInfo->rPicInfo.u4RestartInterval>0)
        {
            prJdecHalInfo->u4RestartIntervalCnt = prJdecHalInfo->rPicInfo.u4RestartInterval;
        }

        // 4. Set Input Buffer Information to HW
        vHwJdecSetInputBuffer(u4JDecID,prJdecHalInfo->rInputInfo.u4InFifoSa,prJdecHalInfo->rInputInfo.u4InFifoSz);
        if(!prJdecHalInfo->fgReinit)
            i4HwJdecSetBarrelShifterAddr(u4JDecID,prJdecHalInfo->rInputInfo.u4BitstreamSa,0); // Set VLD read pointer to bitstream start address.
        else
            i4HwJdecSetBarrelShifterAddr(u4JDecID,prJdecHalInfo->rHwStatus.u4ReadAddrBytes,
                                                  prJdecHalInfo->rHwStatus.u4ReadAddrBits);

        // 5. Set Output Buffer Information to HW
        i4HwJdecSetPitchPerRow(u4JDecID,prJdecHalInfo->rOutputInfo.u4OutYBufPitch,
                                        prJdecHalInfo->rOutputInfo.u4OutCbBufPitch,
                                        prJdecHalInfo->rOutputInfo.u4OutCrBufPitch,
                                        (prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE),
                                        (prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE));
        if(prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT)
        {
            i4HwJdecSetCoefPitchPerRow(u4JDecID,prJdecHalInfo->rPicInfo.prSofInfo);
        }

    }

    // Scan info update
    if(prJdecHalInfo->fgStartOfScan || prJdecHalInfo->fgReinit)
    {
        // Initialize VLD marker_found and encounter_marker register
        vHwJdecInitScan(u4JDecID);
    
        // 2. Set Picture Information to HW
        if((i4Ret = i4HwJdecSetHuffmanTable(u4JDecID,prJdecHalInfo->rPicInfo.prHuffTblInfo,
                                           (UINT8 *)((((UINT32)prJdecHalInfo->au1HwDcHuffTbl+15)>>4)<<4),
                                           (UINT8 *)((((UINT32)prJdecHalInfo->au1HwAcHuffTbl+15)>>4)<<4))) < 0)
            return i4Ret;
        i4HwJdecSetCurrUsedQuantTable(u4JDecID,prJdecHalInfo->rPicInfo.prSofInfo,&(prJdecHalInfo->rScanInfo));

        // 3. Set Scan Information to HW
        // Set Ah, Al, Ss, Se information
        vHwJdecSetAhAl(u4JDecID,(UINT32)prJdecHalInfo->rScanInfo.u1Ah,(UINT32)prJdecHalInfo->rScanInfo.u1Al);
        vHwJdecSetDcAc(u4JDecID,(UINT32)prJdecHalInfo->rScanInfo.u1Ah,(UINT32)prJdecHalInfo->rScanInfo.u1Ss);
        if(prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT)
        {
            if ((prJdecHalInfo->rScanInfo.u1Se == 63) && (prJdecHalInfo->rScanInfo.u1Al == 0))
                vHwJdecSetLastScan(u4JDecID,TRUE);
            else
                vHwJdecSetLastScan(u4JDecID,FALSE);
        } else
        {
            vHwJdecSetLastScan(u4JDecID,TRUE);
        }

        // Set DC Huffman List of Each block in MCU
        // Set AC Huffman List of Each block in MCU
        i4HwJdecSetBlkAndDcAcTblList(u4JDecID,prJdecHalInfo->rPicInfo.prSofInfo->arSofComp,
                                              &(prJdecHalInfo->rScanInfo));
        // Set Component number of Each block in MCU
        i4HwJdecSetBlkAndCompMembership(u4JDecID,prJdecHalInfo->rPicInfo.prSofInfo->arSofComp,
                                                 &(prJdecHalInfo->rScanInfo));
        i4HwJdecSetDcAcNeededList(u4JDecID,prJdecHalInfo->rPicInfo.prSofInfo->arSofComp,&(prJdecHalInfo->rScanInfo));

        // Set Block Position in MCU
        i4HwJdecSetBlockPostition(u4JDecID,prJdecHalInfo->rPicInfo.prSofInfo,&(prJdecHalInfo->rScanInfo));

        i4HwJdecSetBlockCount(u4JDecID,prJdecHalInfo->rPicInfo.prSofInfo->arSofComp,&(prJdecHalInfo->rScanInfo));

        // 5. Set Output Buffer Information to HW
        if ((prJdecHalInfo->rScanInfo.u1Se == 63) && (prJdecHalInfo->rScanInfo.u1Al == 0)) // Last scan
            i4HwJdecSetPitchPerRow(u4JDecID,prJdecHalInfo->rOutputInfo.u4OutYBufPitch,
                                            prJdecHalInfo->rOutputInfo.u4OutCbBufPitch,
                                            prJdecHalInfo->rOutputInfo.u4OutCrBufPitch,
                                            TRUE,TRUE);

        // 6. HW related setting
        vHwJdecSetNoneZeroHistoryBuf(u4JDecID,(UINT32)(prJdecHalInfo->pvNoneZeroHistoryBuf[prJdecHalInfo->rScanInfo.au1CompId[0]]));

        if(!prJdecHalInfo->fgReinit)
            prJdecHalInfo->u4CurrMCUNum = 0;
        vHwJdecCleanInterBuf(u4JDecID,prJdecHalInfo->fgStartOfScan);
        prJdecHalInfo->fgStartOfScan = FALSE;
    }
    else
    {
        vHwJdecCleanInterBuf(u4JDecID,prJdecHalInfo->fgStartOfScan);
    }

    // 2. Picture Information
    // Restart Interval Processing.
    if(prJdecHalInfo->fgDecoded && (prJdecHalInfo->eDecodingMode != JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW))
    {
        if(prJdecHalInfo->rPicInfo.u4RestartInterval>0)
        {
            prJdecHalInfo->u4RestartIntervalCnt--;
            if(prJdecHalInfo->u4RestartIntervalCnt == 0)
            {
                i4HwJdecProcRestart(u4JDecID);
                prJdecHalInfo->u4EOBRUN = 0;
                prJdecHalInfo->u4RestartIntervalCnt = prJdecHalInfo->rPicInfo.u4RestartInterval;
            }
        }
    }

    // 3. Set Scan Information to HW
    // Set Ah, Al, Ss, Se information
    if((prJdecHalInfo->rScanInfo.u1Ah == 0) && (prJdecHalInfo->rScanInfo.u1Ss != 0) && // AC first
       (prJdecHalInfo->u4EOBRUN != 0) && // EOBRUN
       ((prJdecHalInfo->rScanInfo.u1Se == 63) && (prJdecHalInfo->rScanInfo.u1Al == 0))) // Last Scan
        vHwJdecSetSsSe(u4JDecID,64,63);
    else
        vHwJdecSetSsSe(u4JDecID,(UINT32)prJdecHalInfo->rScanInfo.u1Ss,(UINT32)prJdecHalInfo->rScanInfo.u1Se);

    // 5. Set Output Buffer Information to HW
    {
        UINT32 u4YBufSa,u4CbBufSa,u4CrBufSa;
        UINT32 u4YCoeffBufSa,u4CbCoeffBufSa,u4CrCoeffBufSa;

        i4HwJdecCalculateOutputBufAddr(u4JDecID,prJdecHalInfo->u4CurrMCUNum,
                                                prJdecHalInfo->rPicInfo.prSofInfo,
                                                &(prJdecHalInfo->rScanInfo),
                                                &(prJdecHalInfo->rOutputInfo),
                                                prJdecHalInfo->u4OutCoefYBufSa,
                                                prJdecHalInfo->u4OutCoefCbBufSa,
                                                prJdecHalInfo->u4OutCoefCrBufSa,
                                                &u4YBufSa,
                                                &u4CbBufSa,
                                                &u4CrBufSa,
                                                &u4YCoeffBufSa,
                                                &u4CbCoeffBufSa,
                                                &u4CrCoeffBufSa, eFlow);

        if ((prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT) && // Multi-collect mode
            !((prJdecHalInfo->rScanInfo.u1Se == 63) && (prJdecHalInfo->rScanInfo.u1Al == 0))) // Not Last Scan
        {
            vHwJdecSetOutputBufAddr(u4JDecID,u4YCoeffBufSa,
                                             u4CbCoeffBufSa,
                                             u4CrCoeffBufSa,
                                             &(prJdecHalInfo->rScanInfo));
        } else
        {
            vHwJdecSetOutputBufAddr(u4JDecID,u4YBufSa,
                                             u4CbBufSa,
                                             u4CrBufSa,
                                             &(prJdecHalInfo->rScanInfo));
        }

        if((prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT) ||
           (prJdecHalInfo->eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE))
        {
            vHwJdecSetOutputCoefBufAddr(u4JDecID,u4YCoeffBufSa,
                                                 u4CbCoeffBufSa,
                                                 u4CrCoeffBufSa,
                                                 &(prJdecHalInfo->rScanInfo));
        }
    }

    switch(prJdecHalInfo->eDecodingMode)
    {
        case JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW:
        case JDEC_HAL_DEC_MODE_BASELINE_PIC:
            {
                vHwJdecSetOutputBank0BufAddr(u4JDecID,prJdecHalInfo->rOutputInfo.u4OutBank0YBufSa,
                                                      prJdecHalInfo->rOutputInfo.u4OutBank0CbBufSa,
                                                      prJdecHalInfo->rOutputInfo.u4OutBank0CrBufSa);
                vHwJdecSetOutputBank1BufAddr(u4JDecID,prJdecHalInfo->rOutputInfo.u4OutBank1YBufSa,
                                                      prJdecHalInfo->rOutputInfo.u4OutBank1CbBufSa,
                                                      prJdecHalInfo->rOutputInfo.u4OutBank1CrBufSa);
                vHwJdecSetMCUWidth(u4JDecID, (UINT32)((prJdecHalInfo->rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor)*8),
                                             (UINT32)((prJdecHalInfo->rPicInfo.prSofInfo->arSofComp[1].u1HSampFactor)*8),
                                             (UINT32)((prJdecHalInfo->rPicInfo.prSofInfo->arSofComp[2].u1HSampFactor)*8));
            }
            break;
    }

    // 6. HW related setting
    //vHwJdecCleanInterBuf(u4JDecID,prJdecHalInfo->fgStartOfScan);
    vHwJdecSetCurrMCUNum(u4JDecID,prJdecHalInfo->u4CurrMCUNum);
    i4HwJdecSetEOBRUN(u4JDecID,prJdecHalInfo->u4EOBRUN);
    vHwJdecSetOutputNoneZeroHistoryBufOnly(u4JDecID,FALSE);

    if(prJdecHalInfo->fgReinit)
        i4HwJdecSetHwStatus(u4JDecID,&(prJdecHalInfo->rHwStatus));


    // 7. Trigger Decoding
    switch(prJdecHalInfo->eDecodingMode)
    {
        case JDEC_HAL_DEC_MODE_BASELINE_MCU:
        case JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT:
        case JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE:
            vHwJdecDecodeMCU(u4JDecID);
            break;
        case JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW:
            vHwJdecDecodeMCURow(u4JDecID,!(prJdecHalInfo->fgDecoded));
            break;
        case JDEC_HAL_DEC_MODE_BASELINE_PIC:
            vHwJdecDecodePicture(u4JDecID);
            break;
    }

    // 8. Status Update
    prJdecHalInfo->fgDecoded = TRUE;
    prJdecHalInfo->fgReinit = FALSE;
    switch(prJdecHalInfo->eDecodingMode)
    {
        case JDEC_HAL_DEC_MODE_BASELINE_MCU:
        case JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT:
        case JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE:
            prJdecHalInfo->u4CurrMCUNum++;
            break;
    }

    return S_JDEC_HAL_OK;
}


/// Get Jpeg HAL decode status
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Get_Decode_Status(
    UINT32 u4JDecID                                     ///< [IN] Jpeg decoder hardware ID
)
{
    BOOL fgFinish = FALSE;

    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    // Skip by EOBRun
    if(_prJdecHalInfo[u4JDecID]->fgEOBRUNSkip)
        return S_JDEC_HAL_OK;

    switch(_prJdecHalInfo[u4JDecID]->eDecodingMode)
    {
        case JDEC_HAL_DEC_MODE_BASELINE_MCU:
        case JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT:
        case JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE:
            fgFinish = fgHwJdecIsDecodeFinish(u4JDecID);
            break;
        case JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW:
            fgFinish = fgHwJdecIsDecodeMCURowFinish(u4JDecID);
            break;
        case JDEC_HAL_DEC_MODE_BASELINE_PIC:
            fgFinish = fgHwJdecIsDecodePictureFinish(u4JDecID);
            break;
    }

    if(fgFinish)
    {
        if(_prJdecHalInfo[u4JDecID]->u4EOBRUN != 0)
            _prJdecHalInfo[u4JDecID]->u4EOBRUN--;
        else
            _prJdecHalInfo[u4JDecID]->u4EOBRUN = u4HwJdecGetEOBRUN(u4JDecID);

        // Process marker except RST marker
        if(fgHwJdecIsFoundMarker(u4JDecID))
        {
            UINT32 u4JpegMarker = u4HwJdecGetFoundMarker(u4JDecID);
            if (((u4JpegMarker >= 0xD0) && (u4JpegMarker <= 0xD7)) || (u4JpegMarker == 0x00))
            {
                //do nothing
            }
            else
        {
#if 0
            UINT32 u4Bytes;
            UINT32 u4Bits;

            u4Bytes = u4VDecReadVldRPtr(0,u4JDecID,&u4Bits,PHYSICAL(_prJdecHalInfo[u4JDecID]->rInputInfo.u4InFifoSa));

            if(u4Bytes >= 2)
                u4Bytes -= 2;
            else
                u4Bytes += _prJdecHalInfo[u4JDecID]->rInputInfo.u4InFifoSz - 2;

            i4HwJdecSetBarrelShifterAddr(u4JDecID,_prJdecHalInfo[u4JDecID]->rInputInfo.u4InFifoSa + u4Bytes,0);

            // Initialize VLD marker_found and encounter_marker register
            vHwJdecInitScan(u4JDecID);
#endif
                _prJdecHalInfo[u4JDecID]->fgReachMarker = TRUE;
            }
        }
        return S_JDEC_HAL_OK;
    }
    else
        return E_JDEC_HAL_FAIL;
}


/// Get Jpeg VLD read pointer
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Get_Read_Pointer(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    UINT32 *pu4AddrBytes,                               ///< [IN] Read pointer address (bytes)
    UINT32 *pu4AddrBits                                 ///< [IN] Read pointer address (bytes)
)
{
    UINT32 u4Bytes;
    UINT32 u4Bits;

    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;
    if(pu4AddrBytes == NULL)
        return E_JDEC_HAL_FAIL;
    if(pu4AddrBits == NULL)
        return E_JDEC_HAL_FAIL;

    u4Bytes = u4VDecReadVldRPtr(0,u4JDecID,&u4Bits,PHYSICAL(_prJdecHalInfo[u4JDecID]->rInputInfo.u4InFifoSa));

    if(_prJdecHalInfo[u4JDecID]->fgReachMarker)
    {
        if(u4Bytes >= 2)
            u4Bytes -= 2;
        else
            u4Bytes += _prJdecHalInfo[u4JDecID]->rInputInfo.u4InFifoSz - 2;
    }

    if(u4Bytes < _prJdecHalInfo[u4JDecID]->rInputInfo.u4InFifoSz)
    {
        *pu4AddrBytes = _prJdecHalInfo[u4JDecID]->rInputInfo.u4InFifoSa + u4Bytes;
        *pu4AddrBits = u4Bits;
    } else
    {
        return E_JDEC_HAL_FAIL;
    }

    return S_JDEC_HAL_OK;
}


/// Set Jpeg VLD read pointer
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_Read_Pointer(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    UINT32 u4Addr                                       ///< [IN] Read pointer address (bytes)
)
{
    INT32 i4Ret;

    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    i4Ret = i4HwJdecSetBarrelShifterAddr(u4JDecID,u4Addr,0);

    return i4Ret;
}


/// Get Jpeg hardware status
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Get_HW_Status(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    JDEC_HAL_HW_STATUS_T *prHwStatus                    ///< [OUT] Hardware status
)
{
    INT32 i4Ret;

    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    if(prHwStatus == NULL)
        return E_JDEC_HAL_FAIL;

    i4Ret = i4HwJdecGetHwStatus(u4JDecID,prHwStatus);
    prHwStatus->u4CurrMCUNum = _prJdecHalInfo[u4JDecID]->u4CurrMCUNum;
    i4JDEC_HAL_Get_Read_Pointer(u4JDecID,&(prHwStatus->u4ReadAddrBytes),&(prHwStatus->u4ReadAddrBits));

    return i4Ret;
}


/// Set Jpeg hardware status
/// \return If return value < 0, it's failed. Please reference hal_jdec_errcode.h.
INT32 i4JDEC_HAL_Set_HW_Status(
    UINT32 u4JDecID,                                    ///< [IN] Jpeg decoder hardware ID
    const JDEC_HAL_HW_STATUS_T *prHwStatus              ///< [IN] Hardware status
)
{
    JDEC_HAL_INFO_T *prJdecHalInfo;

    if(u4JDecID >= HW_JDEC_VLD_NUM)
        return E_JDEC_HAL_FAIL;

    if(prHwStatus == NULL)
        return E_JDEC_HAL_FAIL;

    prJdecHalInfo = _prJdecHalInfo[u4JDecID];

    prJdecHalInfo->u4CurrMCUNum = prHwStatus->u4CurrMCUNum;
    prJdecHalInfo->fgReinit = TRUE;
    prJdecHalInfo->fgDecoded = TRUE;
    x_memcpy(&(prJdecHalInfo->rHwStatus),prHwStatus,sizeof(JDEC_HAL_HW_STATUS_T));

    return S_JDEC_HAL_OK;
}


//extern UINT32 _MPV_VldRead32(UCHAR ucMpvId, UINT16 u2Offset);
extern INLINE UINT32 u4HwJdecRead32(UINT32 u4HwId, UINT32 u4Offset);

UINT32 u4VDecReadVldRPtr(UINT32 u4BSID, UINT32 u4VDecID, UINT32 *pu4Bits, 
UINT32 u4VFIFOSa)
{
    UINT32 u4DramRptr;
    UINT32 u4SramRptr, u4SramWptr;
    UINT32 u4SramDataSz;
    UINT32 u4ByteAddr;
#ifdef CC_MT5363
    UINT32 u4Reg;
    u4Reg = u4HwJdecRead32(u4VDecID, RO_VLD_VBAR + (u4BSID << 10));
    u4SramRptr =  ((u4Reg & 0xf) << 2) + ((u4Reg >> 24) & 0x3);
    u4SramWptr = ((u4Reg >> 8) & 0xf) << 2;
#else      
    u4SramRptr = u4HwJdecRead32(u4VDecID, (UINT32)(RO_VLD_VBAR + (u4BSID << 10))) & 0xff;
    u4SramWptr = (u4HwJdecRead32(u4VDecID, (UINT32)(RO_VLD_VBAR + (u4BSID << 10))) >> 16) & 0xff;
#endif
    u4DramRptr = u4HwJdecRead32(u4VDecID, (UINT32)(RO_VLD_VRPTR + (u4BSID << 10)));
    if (u4SramWptr > u4SramRptr)
    {
        u4SramDataSz = u4SramWptr - u4SramRptr;
    }
    else
    {
        u4SramDataSz = 64 - (u4SramRptr - u4SramWptr);
    }
  
    *pu4Bits = u4HwJdecRead32(u4VDecID, (UINT32)(RW_VLD_BITCOUNT + (u4BSID << 10))) & 0x3f;
  
    if (u4HwJdecRead32(u4VDecID, (UINT32)(RW_VLD_WMV_ABS + (u4BSID << 10))) & 0x1) // 03 replacement enable!
    {
        u4ByteAddr = u4DramRptr - (u4SramDataSz + 4) * 4 + (*pu4Bits / 8);
    }
    else
    {
        u4ByteAddr = u4DramRptr - (u4SramDataSz + 3)* 4 + (*pu4Bits / 8);
    }
    *pu4Bits &= 0x7;
    
    u4ByteAddr -= ((UINT32)u4VFIFOSa);
  
    return (u4ByteAddr);
}


INT32 i4JDEC_HAL_Set_Buf(UINT32 u4JDecID, VOID **paCoefBuf, VOID **paNzBuf)
{
    _u4JdecHalOutCoefYBufSa[u4JDecID] = (UINT32)paCoefBuf[0];
    _u4JdecHalOutCoefCbBufSa[u4JDecID] = (UINT32)paCoefBuf[1];
    _u4JdecHalOutCoefCrBufSa[u4JDecID] = (UINT32)paCoefBuf[2];
    _pvJdecHalNoneZeroHistoryBuf[0] = paNzBuf[0];
    _pvJdecHalNoneZeroHistoryBuf[1] = paNzBuf[1];    
    _pvJdecHalNoneZeroHistoryBuf[2] = paNzBuf[2];    
    _prJdecHalInfo[0] = VIRTUAL((JDEC_HAL_INFO_T *)BSP_AllocAlignedDmaMemory(sizeof(JDEC_HAL_INFO_T),2048));
    x_memset(_prJdecHalInfo[0],0,sizeof(JDEC_HAL_INFO_T));
    _fgJdecHalAllocate = TRUE;
    return 0;
}

INT32 i4JDEC_HAL_Unset_Buf(UINT32 u4JDecID)
{
    _u4JdecHalOutCoefYBufSa[u4JDecID] = 0;
    _u4JdecHalOutCoefCbBufSa[u4JDecID] = 0;
    _u4JdecHalOutCoefCrBufSa[u4JDecID] = 0;
    _pvJdecHalNoneZeroHistoryBuf[0] = 0;
    _pvJdecHalNoneZeroHistoryBuf[1] = 0;    
    _pvJdecHalNoneZeroHistoryBuf[2] = 0;    
    BSP_FreeAlignedDmaMemory((UINT32)_prJdecHalInfo[0]);
    _fgJdecHalAllocate = FALSE;
    return 0;
}
