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
/*! \file hal_vdec_mpeg_if.h
*
*  \par LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, USER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* ARE PROVIDED TO USER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS
* ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
* RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED
* IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND USER AGREES TO LOOK ONLY TO
* SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL
* ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO USER'S
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* USER HEREBY ACKNOWLEDGES THE CONFIDENTIALITY OF MEDIATEK SOFTWARE AND AGREES
* NOT TO DISCLOSE OR PERMIT DISCLOSURE OF ANY MEDIATEK SOFTWARE TO ANY THIRD
* PARTY OR TO ANY OTHER PERSON, EXCEPT TO DIRECTORS, OFFICERS, EMPLOYEES OF
* USER WHO ARE REQUIRED TO HAVE THE INFORMATION TO CARRY OUT THE PURPOSE OF
* OPENING OR USING THIS FILE.
*
* \par Project
*    MT8520
*
* \par Description
*    Jpeg Decoder Verification Code
*
* \par Author_Name
*    CK Hu
*
* \par Last_Changed
* $Author: dtvbm11 $
* $Modtime: $
* $Revision: #1 $
*
*/

// *********************************************************************
// Memo
// *********************************************************************
/*
*/

//#include <string.h>
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "x_typedef.h"
#include "x_stl_lib.h"

#include "jpg_if.h"
#include "jdec_hal_if.h"
#include "jdec_drv.h"
#include "img_lib_if.h"
#define DMXCMD_READ32(offset)			IO_READ32(DEMUX0_BASE, ((offset) * 4))


extern void vImgReszVfyJpegPicModeInit(UINT32 u4HwId,UINT32 u4SrcBufWidth,
                                              UINT32 u4SrcImgWidth,UINT32 u4SrcImgHeight,
                                              UINT32 u4SrcBuf1SA1,UINT32 u4SrcBuf1SA2,UINT32 u4SrcBuf1SA3,
                                              UINT32 u4SrcBuf2SA1,UINT32 u4SrcBuf2SA2,UINT32 u4SrcBuf2SA3,
                                              UINT32 u4DestBufWidth,UINT32 u4DestBufHeight,
                                              UINT32 u4DestImgWidth,UINT32 u4DestImgHeight,
                                              UINT32 u4DestImgXOff,UINT32 u4DestImgYOff,
                                              UINT32 u4DestBufSA1,UINT32 u4DestBufSA2,
                                              UINT32 u4HSampleFactor0,UINT32 u4VSampleFactor0,
                                              UINT32 u4HSampleFactor1,UINT32 u4VSampleFactor1,
                                              UINT32 u4HSampleFactor2,UINT32 u4VSampleFactor2
);
extern INT32 i4ImgReszVfyJpegPicModeGetResizeStatus(UINT32 u4HwId);
extern void vImgReszVfyJpegPicModeUninit(UINT32 u4HwId);
extern void vImgReszVfyJpegPreloadInit(UINT32 u4HwId,UINT32 u4SrcBufWidth,
                                              UINT32 u4SrcImgWidth,UINT32 u4SrcImgHeight,
                                              UINT32 u4SrcBufSA1,UINT32 u4SrcBufSA2,UINT32 u4SrcBufSA3,
                                              UINT32 u4DestBufWidth,UINT32 u4DestBufHeight,
                                              UINT32 u4DestImgWidth,UINT32 u4DestImgHeight,
                                              UINT32 u4DestImgXOff,UINT32 u4DestImgYOff,
                                              UINT32 u4DestBufSA1,UINT32 u4DestBufSA2,
                                              UINT32 u4HSampleFactor0,UINT32 u4VSampleFactor0,
                                              UINT32 u4HSampleFactor1,UINT32 u4VSampleFactor1,
                                              UINT32 u4HSampleFactor2,UINT32 u4VSampleFactor2,
                                              BOOL fgYExist,BOOL fgCbExist, BOOL fgCrExist
);
extern INT32 i4ImgReszVfyJpegPreloadGetResizeStatus(UINT32 u4HwId);
extern void vImgReszVfyJpegPreloadUninit(UINT32 u4HwId);

extern void vImgReszVfyBufDeBlock(UINT32 u4SrcBufSA,UINT32 u4DestBufSA,UINT32 u4BufWidth,UINT32 u4BufHeight,
                           UINT32 u4BlockWidth,UINT32 u4BlockHeight);
extern void vImgReszVfyBufDeAddrSwap(UINT32 u4SrcBufSA,UINT32 u4DestBufSA,UINT32 u4BufLen);


#ifdef JPEG_AUTO_TEST
#ifndef CC_EMULATION
#define CC_EMULATION
#endif
typedef struct
{
    UINT32 u4ByteSize;
    CHAR *szFileName;
	UINT32 u4rCheckSum[4];
}JPGCLI_PROG_SET_T;
#define T32_TEST
static INT32 i4CurDecIndex=0;
extern INT32 _JpgVerifyCmdSetParam(INT32 i4ParamType,INT32 Param);
#define LIST_MAX 14
static JPGCLI_PROG_SET_T _arProgressive[] =
{
	{8317, "small.jpg",{0xa0c402c,0x47382e03,0xe642018c,0x37866fbb}},
	{181053, "beach.jpg",{0x7f6c4de2,0x8dfa5229,0x5db2f4a4,0x6b1994af}},
	{399917, "girl1.jpg",{0x1bea14d3,0xdf0bdafe,0x30ee1a12,0x2be409e3}},
	{90567, "girl2.jpg",{0xb5c3f9e9,0x8f7d77e8,0xf0707b4e,0x35b1ed1f}},
	{5655, "rose.jpg",{0x8ac677ec,0xb809010c,0x9cef19af,0xdfbe92a7}},
	{19456, "sun.jpg",{0xf8852bc4,0x7ddfde7d,0x9c32e02b,0x1297ea6c}},
	{522, "yellow.jpg",{0xb8b8b880,0x0,0x59595950,0x121211d0}},
	{42150, "pkoala.jpg",{0xce7f55dc,0x1f864712,0xb1658b8c,0x9f6b287a}},
	{106222, "cuore.jpg",{0xf4a42371,0x86a805d3,0xb7007b32,0x324ca476}},
	{111856, "catfrog.jpg",{0xb802c3b3,0x5f619783,0x3898c06c,0x4ffd1ba2}},
	{51409, "autumn.jpg",{0xab6a30f7,0xb6470e42,0xaf9a81fc,0x114bc135}},
	{247300, "Progressive.jpg",{0xa399af2d,0x9764dc76,0xe35be8b7,0x1e5a745a}},
	{1231, "small_kitty.jpg",{0x1c95b960,0x26e6b641,0x7ce04599,0xc05cb53a}},
	{409250, "000430-1210-34-progressive.jpg",{0,0,0,0}},
	{211178,"waterwalk.jpg",{0,0,0,0}},
	{33415,"dawoo.jpg",{0,0,0,0}},
	{11029,"liang.jpg",{0,0,0,0}},
	{82042,"joeunsw.jpg",{0,0,0,0}},
	{247603,"girl3.jpg",{0,0,0,0}},
	{114162,"skyheart.jpg",{0,0,0,0}},
	{283983,"lgp2000.jpg",{0,0,0,0}},
	{766299,"girl4.jpg",{0,0,0,0}},
	{26267,"italy.jpg",{0,0,0,0}},
	{347325,"hiteker.jpg",{0,0,0,0}},
	{0, NULL,{0,0,0,0}}
};
#endif
//#define JDEC_COLOR_BUF_WIDTH   3264
//#define JDEC_COLOR_BUF_HEIGHT  2448
#define JDEC_COLOR_BUF_WIDTH   1280
#define JDEC_COLOR_BUF_HEIGHT  1024
//#define JDEC_FIFO_SIZE (/*4**/1024*1024)
#define JDEC_COLLECT_LIMIT  (1024 * 1024)
#define JDEC_VERIFY_LIMIT  (1024*768)
#define JDEC_FULLNESS   (400*1024)

static BYTE *_EmuJpgFifo=NULL;
static INT32 _i4JpegFileSize = 0;
static JPEG_JFIF_HEADER_T *_prJPEG_JFIF_HEADER = NULL;
static UINT32 _u4ErrMCUNum = 0;


#define JpegFileListMax 64

static UINT32 _u4ColorBufY;
static UINT32 _u4ColorBufCb;
static UINT32 _u4ColorBufCr;
static UINT32 _u4ColorBufY2;
static UINT32 _u4ColorBufCb2;
static UINT32 _u4ColorBufCr2;
//static UINT32 _u4FrameBufY;
//static UINT32 _u4FrameBufC;
//static UINT32 _u4EnhanceBufY;
//static UINT32 _u4EnhanceBufCb;
//static UINT32 _u4EnhanceBufCr;

static UINT32 _u4ColorBufWidth;
static UINT32 _u4ImageWidth;
static UINT32 _u4ImageHeight;

static UINT32 _u4XSampleY,_u4YSampleY;
static UINT32 _u4XSampleCb,_u4YSampleCb;
static UINT32 _u4XSampleCr,_u4YSampleCr;

static UINT32 _u4DiffCnt;

static BOOL _fgStop = FALSE;

//#define JDEC_VFY_TEST_PROGRESSIVE_PRELOAD 1
//static BOOL _fgReinitTest = TRUE;

static BOOL _fgVerifyFlow;

VOID vJpegSetProgressiveStop(void)
{
    _fgStop = TRUE;
}

INT32 vJpegDecode(UINT32 u4JDecID,JDEC_HAL_DEC_MODE_T eDecodingMode,UINT32 u4ColorBufY,UINT32 u4ColorBufCb,UINT32 u4ColorBufCr,
                                                                    UINT32 u4ColorBufY2,UINT32 u4ColorBufCb2,UINT32 u4ColorBufCr2)
                                                                   //UINT32 u4FrameBufY,UINT32 u4FrameBufC,
                                                                   //UINT32 u4FrameBufWidth,UINT32 u4FrameBufHeight)
{
    JPEG_JFIF_HEADER_T *prJPEG_JFIF_HEADER = _prJPEG_JFIF_HEADER;
    JPEGHANDLE hParser;
    JDEC_HAL_PIC_INFO_T rPicInfo = { 0 };
    JDEC_HAL_INPUT_INFO_T rInputInfo = { 0 };
    JDEC_HAL_OUTPUT_INFO_T rOutputInfo = { 0 };
    UINT32 au4MCUWidth[3] = {0},au4MCUHeight[3] = {0};
    UINT32 u4CombMCUWidth,u4CombMCUHeight;
    UINT32 u4MCUWidth,u4MCUHeight;
    UINT32 u4MCUx,u4MCUy;
    UINT32 u4YBufSa,u4CbBufSa,u4CrBufSa,u4PitchPerRow;
    UINT32 u4WaitCnt = 0;
    UINT32 u4MCUCnt;
    UINT32 u4ReadAddrBytes,u4ReadAddrBits;
    UINT32 u4ScanCnt;
    JDEC_HAL_DEC_PROG_T eProgMode;
    INT32 i4Ret = E_IMG_OK;
    UINT32 u4TotalComp = 1;
    BOOL fgPreload = FALSE;
    BOOL fgLastScan[3] = {FALSE};

    if (_fgVerifyFlow)
    {
        eProgMode = JDEC_HAL_DEC_PROG_VERIFY;
    }
    else if (eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT)
    {
        eProgMode = JDEC_HAL_DEC_MULTI_COLLECT;
    }
    else
    {
        eProgMode = JDEC_HAL_DEC_ENHANCE;
    }

    i4JDEC_HAL_Init(u4JDecID);
    i4JDEC_HAL_Set_Decoding_Mode(u4JDecID,eDecodingMode);
    rPicInfo.prSofInfo = (JDEC_JFIF_SOF_T *)&(_prJPEG_JFIF_HEADER->rSOF);
    rPicInfo.prHuffTblInfo = (JDEC_JFIF_DHT_T *)&(_prJPEG_JFIF_HEADER->rDHT);
    rPicInfo.prQTblInfo = (JDEC_JFIF_DQT_T *)&(_prJPEG_JFIF_HEADER->rDQT);
    rPicInfo.u4RestartInterval = _prJPEG_JFIF_HEADER->u2RestartInterval;
    i4JDEC_HAL_Set_Pic_Info(u4JDecID,&rPicInfo);
    x_memcpy(&(_prJPEG_JFIF_HEADER->rSOS), &(_prJPEG_JFIF_HEADER->arSOS[0]), 
        sizeof(JPEG_JFIF_SOS_T));
    i4JDEC_HAL_Set_Scan_Info(u4JDecID,&(_prJPEG_JFIF_HEADER->rSOS));
    rInputInfo.u4InFifoSa = (UINT32)_EmuJpgFifo;
#if 1
    rInputInfo.u4InFifoSz = JDEC_FIFO_SIZE;
#else
    rInputInfo.u4InFifoSz = _prJPEG_JFIF_HEADER->u4ImageByteSize;
#endif
    rInputInfo.u4BitstreamSa = (UINT32)(_prJPEG_JFIF_HEADER->pu1CurrRead);

    i4JDEC_HAL_Set_Input_Info(u4JDecID,&rInputInfo);

    {
        UINT32 u4WidthPerMCU = (UINT32)(rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor * 8);
        u4PitchPerRow = ((rPicInfo.prSofInfo->u2ImageWidth +
                                   u4WidthPerMCU - 1) /
                                   u4WidthPerMCU) * u4WidthPerMCU; // Align to u4WidthPerMCU
        u4PitchPerRow = ((u4PitchPerRow + 15) / 16) * 16; // Align to 16's
        _u4ColorBufWidth = u4PitchPerRow;
    }
	
    rOutputInfo.u4OutYBufPitch = u4PitchPerRow;
    rOutputInfo.u4OutCbBufPitch = u4PitchPerRow;
    rOutputInfo.u4OutCrBufPitch = u4PitchPerRow;

    {
        UINT32 u4CompId;

        if (rPicInfo.prSofInfo->u1NumComponents > (UINT8)3)
        {
            rPicInfo.prSofInfo->u1NumComponents = (UINT8)3;
        }
        for(u4CompId = 0; u4CompId<rPicInfo.prSofInfo->u1NumComponents; u4CompId++)
        {
            UINT32 u4WidthPerMCU = (UINT32)((rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor /
                                    rPicInfo.prSofInfo->arSofComp[u4CompId].u1HSampFactor) * 8);
            UINT32 u4HeightPerMCU = (UINT32)((rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor /
                                     rPicInfo.prSofInfo->arSofComp[u4CompId].u1VSampFactor) * 8);

            au4MCUWidth[u4CompId] = (rPicInfo.prSofInfo->u2ImageWidth + u4WidthPerMCU - 1) / u4WidthPerMCU;
            au4MCUHeight[u4CompId] = (rPicInfo.prSofInfo->u2ImageHeight + u4HeightPerMCU - 1) / u4HeightPerMCU;
        }

        u4CombMCUWidth = (UINT32)((rPicInfo.prSofInfo->u2ImageWidth + rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor * 8 - 1) /
                         (rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor * 8));
        u4CombMCUHeight = (UINT32)((rPicInfo.prSofInfo->u2ImageHeight + rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor * 8 - 1) /
                          (rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor * 8));
    }
    if (rPicInfo.prSofInfo->u1NumComponents != 1)
    {
        u4TotalComp = (UINT32)rPicInfo.prSofInfo->u1NumComponents;
    }

    /*if(eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE)
    {
#ifdef JDEC_VFY_TEST_PROGRESSIVE_PRELOAD
        x_memset(u4FrameBufY,0x80,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        x_memset(u4FrameBufC,0x80,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
#else
        x_memset((void *)_u4EnhanceBufY,0x80,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        x_memset((void *)_u4EnhanceBufCb,0x80,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        x_memset((void *)_u4EnhanceBufCr,0x80,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
#endif
    }*/

    u4ScanCnt = 0;
    do
    {
        rOutputInfo.u4OutYBufSa = u4ColorBufY;
        rOutputInfo.u4OutCbBufSa = u4ColorBufCb;
        rOutputInfo.u4OutCrBufSa = u4ColorBufCr;
        i4JDEC_HAL_Set_Output_Info(u4JDecID,&rOutputInfo);
#if 1
/*        Printf("[Jpeg][Verify] Scan cnt %d, %d component in this scan\n",u4ScanCnt,_prJPEG_JFIF_HEADER->rSOS.u1CompInScan);
        if(_prJPEG_JFIF_HEADER->rSOS.u1CompInScan == 1)
        {
            Printf("[Jpeg][Verify] Component id is %d\n",_prJPEG_JFIF_HEADER->rSOS.au1CompId[0]);
        }
        Printf("[Jpeg][Verify] Ss = %d, Se = %d, Ah = %d, Al = %d\n",_prJPEG_JFIF_HEADER->rSOS.u1Ss,
            _prJPEG_JFIF_HEADER->rSOS.u1Se,
            (_prJPEG_JFIF_HEADER->rSOS.u1AhAl & 0xf0) >> 4,
            _prJPEG_JFIF_HEADER->rSOS.u1AhAl & 0xf);


        if (((_prJPEG_JFIF_HEADER->rSOS.u1AhAl & 0xf0) >> 4) == 0)
        {
            if (_prJPEG_JFIF_HEADER->rSOS.u1Ss == 0)
                Printf("[Jpeg][Verify] JPG_DC_FIRST\n");
            else
                Printf("[Jpeg][Verify] JPG_AC_FIRST\n");
        }
        else
        {
            if (_prJPEG_JFIF_HEADER->rSOS.u1Ss == 0)
                Printf("[Jpeg][Verify] JPG_DC_REFINE\n");
            else
                Printf("[Jpeg][Verify] JPG_AC_REFINE\n");
        }*/
#endif
        /*if(eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE)
        {
            x_memset((void *)u4ColorBufY,0x80,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
            x_memset((void *)u4ColorBufCb,0x80,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
            x_memset((void *)u4ColorBufCr,0x80,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        }*/

        if(_prJPEG_JFIF_HEADER->rSOS.u1CompInScan == 1)
        {
            UINT32 u4CompId = _prJPEG_JFIF_HEADER->rSOS.au1CompId[0];
            if (u4CompId >= (UINT32)3)
            {
                 u4CompId = (UINT32)2;
            }
            u4MCUWidth = au4MCUWidth[u4CompId];
            u4MCUHeight = au4MCUHeight[u4CompId];
        } else
        {
            u4MCUWidth = u4CombMCUWidth;
            u4MCUHeight = u4CombMCUHeight;
        }

        //Printf("[Jpeg][Verify] MCU (%d,%d)",u4MCUWidth,u4MCUHeight);

        u4MCUCnt = 0;
        for(u4MCUy=0; u4MCUy<u4MCUHeight; u4MCUy++)
        {
            u4YBufSa = u4ColorBufY;
            u4CbBufSa = u4ColorBufCb;
            u4CrBufSa = u4ColorBufCr;

            if(eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE)
            {
                //clear color buffer 
                //CR DTV00098636, we change u1HSampFactor to u1VSampFactor when H:V = 2:1, otherwise 2 set color buffer will be clean to 0x80, 
                //result in RZ output error at last line in the MUC row
                x_memset(VIRTUAL((void *)rOutputInfo.u4OutYBufSa), 0x80, (rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor << 3)
                                                                * rPicInfo.prSofInfo->u2ImageWidth);
                x_memset(VIRTUAL((void *)rOutputInfo.u4OutCbBufSa), 0x80, (rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor << 3)
                                                                * rPicInfo.prSofInfo->u2ImageWidth);
                x_memset(VIRTUAL((void *)rOutputInfo.u4OutCrBufSa), 0x80, (rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor << 3)
                                                                * rPicInfo.prSofInfo->u2ImageWidth);
                #if defined(CC_MT5365) || defined(CC_MT5395)
                    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL((void *)rOutputInfo.u4OutYBufSa), (rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor << 3)
                                                                * rPicInfo.prSofInfo->u2ImageWidth);                
                    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL((void *)rOutputInfo.u4OutCbBufSa), (rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor << 3)
                                                                * rPicInfo.prSofInfo->u2ImageWidth);  
                    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL((void *)rOutputInfo.u4OutCrBufSa), (rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor << 3)
                                                                * rPicInfo.prSofInfo->u2ImageWidth);  
                #endif                    
            }

            //Printf("[Jpeg][Verify] MCU Row %d",u4MCUy);

            if(_prJPEG_JFIF_HEADER->rSOS.u1CompInScan == 1)
            {
                u4YBufSa += u4MCUy*u4PitchPerRow*8;
                u4CbBufSa += u4MCUy*u4PitchPerRow*8;
                u4CrBufSa += u4MCUy*u4PitchPerRow*8;
            } else
            {
                u4YBufSa += u4MCUy*u4PitchPerRow*(rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor * 8);
                u4CbBufSa += u4MCUy*u4PitchPerRow*(rPicInfo.prSofInfo->arSofComp[1].u1VSampFactor * 8);
                u4CrBufSa += u4MCUy*u4PitchPerRow*(rPicInfo.prSofInfo->arSofComp[2].u1VSampFactor * 8);
            }

            if((eDecodingMode == JDEC_HAL_DEC_MODE_BASELINE_MCU) ||
               (eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE) ||
               (eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT))
            {                
                UINT32 u4NewRP, u4UsedLen;
                
                for(u4MCUx=0; u4MCUx<u4MCUWidth; u4MCUx++)
                {                    
                    i4JDEC_HAL_Decode(u4JDecID, eProgMode);

                    for(u4WaitCnt=0; u4WaitCnt<0x10000; u4WaitCnt++)
                    {
                        if(i4JDEC_HAL_Get_Decode_Status(u4JDecID) >= 0)
                            break;
                    }

                    if(u4WaitCnt==0x10000)
                    {
                        _u4ErrMCUNum = u4MCUCnt;
                        i4Ret = E_IMG_DEC_FAIL;
                        break;
                    }

                    u4MCUCnt++;

                    if(_prJPEG_JFIF_HEADER->rSOS.u1CompInScan == 1)
                    {
                        u4YBufSa += 8;
                        u4CbBufSa += 8;
                        u4CrBufSa += 8;
                    } 
					else
                    {
                        u4YBufSa += (rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor * 8);
                        u4CbBufSa += (rPicInfo.prSofInfo->arSofComp[1].u1HSampFactor * 8);
                        u4CrBufSa += (rPicInfo.prSofInfo->arSofComp[2].u1HSampFactor * 8);
                    }
                }               

            #if 0  // winton
                if(_fgReinitTest) // Re-initialization test.
                {
                    JDEC_HAL_HW_STATUS_T rHwStatus = { 0 };

                    i4JDEC_HAL_Get_HW_Status(u4JDecID,&rHwStatus);
                    i4JDEC_HAL_Uninit(u4JDecID);
                    i4JDEC_HAL_Init(u4JDecID);
                    i4JDEC_HAL_Set_Decoding_Mode(u4JDecID,eDecodingMode);
                    i4JDEC_HAL_Set_Pic_Info(u4JDecID,&rPicInfo);
                    x_memcpy(&(_prJPEG_JFIF_HEADER->rSOS), &(_prJPEG_JFIF_HEADER->arSOS[u4ScanCnt + 1]), 
                        sizeof(JPEG_JFIF_SOS_T));
                    i4JDEC_HAL_Set_Scan_Info(u4JDecID,&(_prJPEG_JFIF_HEADER->rSOS));
                    i4JDEC_HAL_Set_Input_Info(u4JDecID,&rInputInfo);
                    i4JDEC_HAL_Set_Output_Info(u4JDecID,&rOutputInfo);
                    i4JDEC_HAL_Set_HW_Status(u4JDecID,&rHwStatus);
                }
            #endif
            
                if(u4WaitCnt==0x10000)
                {
                    break;
                }

                if (!_fgVerifyFlow)                     //BC adds for driver
                {
                     if (((_prJPEG_JFIF_HEADER->rSOS.u1Se == 63) && ((_prJPEG_JFIF_HEADER->rSOS.u1AhAl & 0x0f) == 0) && 
                        (eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT)) ||
                        (eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE)) //enhance or last scan of collection 
                     {
                        UINT32 u4ColorCmp;

                        if(_prJPEG_JFIF_HEADER->rSOS.u1CompInScan == 1)
                        {
						    UINT32 u4CompId = _prJPEG_JFIF_HEADER->rSOS.au1CompId[0];
							u4CompId=u4CompId>=3 ? 2 : u4CompId;
							
                            switch(u4CompId)
                            {
                            case 0:
                                //If first scan only has Y, it's gray level and need CbCr padding.
                                u4ColorCmp = (u4ScanCnt) ? (UINT32)0x4 : (UINT32)0xF;
                                if ((_prJPEG_JFIF_HEADER->rSOS.u1Se == 63) && ((_prJPEG_JFIF_HEADER->rSOS.u1AhAl & 0x0f) == 0))
                                {
                                    fgLastScan[0] = TRUE;
                                }
                                break;
                            case 1:
                                u4ColorCmp = 0x2;
                                if ((_prJPEG_JFIF_HEADER->rSOS.u1Se == 63) && ((_prJPEG_JFIF_HEADER->rSOS.u1AhAl & 0x0f) == 0))
                                {
                                    fgLastScan[1] = TRUE;
                                }
                                break;
                            case 2:
                            default:    
                                u4ColorCmp = 0x1;
                                if ((_prJPEG_JFIF_HEADER->rSOS.u1Se == 63) && ((_prJPEG_JFIF_HEADER->rSOS.u1AhAl & 0x0f) == 0))
                                {
                                    fgLastScan[2] = TRUE;
                                }
                                break;
                            }
                        }
                        else
                        {
                            u4ColorCmp = 0x7;
                        }

                        if ((eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE) && u4ScanCnt)
                        {
                            fgPreload = TRUE;
                        }
                        else
                        {
                            fgPreload = FALSE;
                        }
                        if (JDEC_RzRow(rOutputInfo.u4OutYBufSa, rOutputInfo.u4OutCbBufSa, rOutputInfo.u4OutCrBufSa, 
                                    u4MCUy, u4MCUHeight, u4ColorCmp, fgPreload))
                        {
                            i4Ret = E_IMG_HWBUSY;
                        }
                    
                        rOutputInfo.u4OutYBufPitch = u4PitchPerRow;
                        rOutputInfo.u4OutCbBufPitch = u4PitchPerRow;
                        rOutputInfo.u4OutCrBufPitch = u4PitchPerRow;
                        if (u4MCUy % 2)                     //next output buffer
                        {
                            rOutputInfo.u4OutYBufSa = u4ColorBufY;
                            rOutputInfo.u4OutCbBufSa = u4ColorBufCb;
                            rOutputInfo.u4OutCrBufSa = u4ColorBufCr;
                        }
                        else
                        {
                            rOutputInfo.u4OutYBufSa = u4ColorBufY2;
                            rOutputInfo.u4OutCbBufSa = u4ColorBufCb2;
                            rOutputInfo.u4OutCrBufSa = u4ColorBufCr2;
                        }
                        i4JDEC_HAL_Set_Output_Info(u4JDecID,&rOutputInfo);
                    }
                }

                if (!(u4MCUy % 10)) //check buffer after each 10 mcu rows
                {
                    UINT32 u4EmptySize;
                    if (prJPEG_JFIF_HEADER->fgRingBuffer && (!prJPEG_JFIF_HEADER->fgEOF) && 
                        (!_fgVerifyFlow))
                    {
                        i4JDEC_HAL_Get_Read_Pointer(u4JDecID,&u4ReadAddrBytes,&u4ReadAddrBits);
                        u4NewRP = u4ReadAddrBytes - (UINT32)prJPEG_JFIF_HEADER->pu1RingBufStart;
                        if (u4NewRP < prJPEG_JFIF_HEADER->u4RingRP)
            	    	{
	            	    	u4UsedLen = (prJPEG_JFIF_HEADER->u4RingBufSize + u4NewRP) - 
	            	    	            prJPEG_JFIF_HEADER->u4RingRP;
    		            }
	    	            else
    		            {
	    		            u4UsedLen = u4NewRP - prJPEG_JFIF_HEADER->u4RingRP;
            		    }
    	        	    if(prJPEG_JFIF_HEADER->u4RingData < u4UsedLen)
	    	            {
		    	        //LOG(3, "BS Buffer data 0x%x less than used data 0x%x.\n", prJPEG_JFIF_HEADER->u4RingData, u4UsedLen);
    		            }
    	    	        else
        		        {
	        		        prJPEG_JFIF_HEADER->u4RingData -= u4UsedLen;
    	        	    }	
	    	            prJPEG_JFIF_HEADER->u4RingRP = u4NewRP;   
	    	            u4EmptySize = prJPEG_JFIF_HEADER->u4RingBufSize - prJPEG_JFIF_HEADER->u4RingData;                        
                        if (u4EmptySize > JDEC_FULLNESS)
                        {
                            IMG_BsBufManager();                        
                        }
#ifdef LINUX_TURNKEY_SOLUTION
						else if(_u4ImageWidth>3500)
						{
                           x_thread_delay(1);
						}
#endif
                    }
                }
            } else if(eDecodingMode == JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW)
            {
                rOutputInfo.u4OutYBufSa = u4YBufSa;
                rOutputInfo.u4OutCbBufSa = u4CbBufSa;
                rOutputInfo.u4OutCrBufSa = u4CrBufSa;

                rOutputInfo.u4OutBank0YBufSa = u4YBufSa;
                rOutputInfo.u4OutBank0CbBufSa = u4CbBufSa;
                rOutputInfo.u4OutBank0CrBufSa = u4CrBufSa;

                rOutputInfo.u4OutBank1YBufSa = u4YBufSa;
                rOutputInfo.u4OutBank1CbBufSa = u4CbBufSa;
                rOutputInfo.u4OutBank1CrBufSa = u4CrBufSa;

                i4JDEC_HAL_Set_Output_Info(u4JDecID,&rOutputInfo);

                i4JDEC_HAL_Decode(u4JDecID, JDEC_HAL_DEC_PROG_VERIFY);
                for(u4WaitCnt=0; u4WaitCnt<5000; u4WaitCnt++)
                {
                    if((i4Ret = i4JDEC_HAL_Get_Decode_Status(u4JDecID)) >= 0)
                        break;
                }
                if(u4WaitCnt==5000)
                {
                    _u4ErrMCUNum = u4MCUCnt;
                    break;
                }
            } else if(eDecodingMode == JDEC_HAL_DEC_MODE_BASELINE_PIC)
            {
                rOutputInfo.u4OutYBufSa = u4YBufSa;
                rOutputInfo.u4OutCbBufSa = u4CbBufSa;
                rOutputInfo.u4OutCrBufSa = u4CrBufSa;

                rOutputInfo.u4OutBank0YBufSa = u4ColorBufY;
                rOutputInfo.u4OutBank0CbBufSa = u4ColorBufCb;
                rOutputInfo.u4OutBank0CrBufSa = u4ColorBufCr;

                rOutputInfo.u4OutBank1YBufSa = u4ColorBufY + 16*rOutputInfo.u4OutYBufPitch;
                rOutputInfo.u4OutBank1CbBufSa = u4ColorBufCb + 16*rOutputInfo.u4OutYBufPitch;
                rOutputInfo.u4OutBank1CrBufSa = u4ColorBufCr + 16*rOutputInfo.u4OutYBufPitch;

                i4JDEC_HAL_Set_Output_Info(u4JDecID,&rOutputInfo);

#if 0 // winton
                vImgReszVfyJpegPicModeInit(u4JDecID,rOutputInfo.u4OutYBufPitch,
                                                    rPicInfo.prSofInfo->u2ImageWidth,rPicInfo.prSofInfo->u2ImageHeight,
                                                    rOutputInfo.u4OutBank0YBufSa,rOutputInfo.u4OutBank0CbBufSa,rOutputInfo.u4OutBank0CrBufSa,
                                                    rOutputInfo.u4OutBank1YBufSa,rOutputInfo.u4OutBank1CbBufSa,rOutputInfo.u4OutBank1CrBufSa,
                                                    u4FrameBufWidth,u4FrameBufHeight,
                                                    u4FrameBufWidth,u4FrameBufHeight,
                                                    0,0,
                                                    u4FrameBufY,u4FrameBufC,
                                                    rPicInfo.prSofInfo->arSofComp[0].u1HSampFactor,
                                                    rPicInfo.prSofInfo->arSofComp[0].u1VSampFactor,
                                                    rPicInfo.prSofInfo->arSofComp[1].u1HSampFactor,
                                                    rPicInfo.prSofInfo->arSofComp[1].u1VSampFactor,
                                                    rPicInfo.prSofInfo->arSofComp[2].u1HSampFactor,
                                                    rPicInfo.prSofInfo->arSofComp[2].u1VSampFactor);
#endif

                i4JDEC_HAL_Decode(u4JDecID, JDEC_HAL_DEC_PROG_VERIFY);
                for(u4WaitCnt=0; u4WaitCnt < (UINT32)0xFFFFFFFF; u4WaitCnt++)
                {
                    if((i4Ret = i4JDEC_HAL_Get_Decode_Status(u4JDecID)) >= 0)
                        break;
                }
                if(u4WaitCnt == (UINT32)0xFFFFFFFF)
                {
                    _u4ErrMCUNum = 2266;
                    break;
                }

#if 0 // winton
                for(u4WaitCnt=0; u4WaitCnt<0xFFFFFFFF; u4WaitCnt++)
                {
                    if((i4Ret = i4ImgReszVfyJpegPicModeGetResizeStatus(u4JDecID)) >= 0)
                        break;
                }
                vImgReszVfyJpegPicModeUninit(u4JDecID);
                if(u4WaitCnt==0xFFFFFFFF)
                {
                    _u4ErrMCUNum = 3388;
                    break;
                }
#endif
                break;
            }
        }

        if (i4Ret != E_IMG_OK)
        {
            break;
        }

        // For progressvie picture, search next start of scan (SOS)
        if((eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE) ||
           (eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT))
        {
            i4JDEC_HAL_Get_Read_Pointer(u4JDecID,&u4ReadAddrBytes,&u4ReadAddrBits);

            if(u4ReadAddrBits > 0)
                u4ReadAddrBytes++;

            if(u4ReadAddrBytes >= (rInputInfo.u4InFifoSa + rInputInfo.u4InFifoSz))
            {
                u4ReadAddrBytes -= rInputInfo.u4InFifoSa;
            }

            if(((UINT32)(prJPEG_JFIF_HEADER->pu1CurrRead) - rInputInfo.u4InFifoSa) > (UINT32)_i4JpegFileSize)
            {
                Printf("(prJPEG_JFIF_HEADER->pu1CurrRead) - rInputInfo.u4InFifoSa > _i4JpegFileSize\n");
                break;
            }
            //if(u4ScanCnt == 2)
            //    break;

            prJPEG_JFIF_HEADER->fgParsed = 0;   // winton
            hParser = (JPEGHANDLE)prJPEG_JFIF_HEADER;
            if (JFIF_New(hParser, (void *)u4ReadAddrBytes, (UINT32)_i4JpegFileSize))
            {
                if ((fgLastScan[0] & fgLastScan[1] & fgLastScan[2] & u4TotalComp == 3) ||
                    (fgLastScan[0] & u4TotalComp == 1))  //last scan done, finished
                {
                    break;
                }
                i4Ret = E_IMG_DEC_FAIL;
                break;
            }

            // When reach end of image, quit.
            if(prJPEG_JFIF_HEADER->fgEOI)
            {
                if ((!(fgLastScan[0] & fgLastScan[1] & fgLastScan[2]) && u4TotalComp == 3 )||
                    (!fgLastScan[0] && u4TotalComp == 1))
                {
                    i4Ret = E_IMG_DEC_FAIL;       //no last scan but EOI ==> error
                }
                break;
            }

            if (_fgStop)
            {
                _fgStop = FALSE;
                i4Ret = E_IMG_CANCEL;
                break;
            }
            if ((fgLastScan[0] & fgLastScan[1] & fgLastScan[2] & u4TotalComp == 3) ||
                (fgLastScan[0] & u4TotalComp == 1))  //last scan done, finished
            {
                break;
            }

            i4JDEC_HAL_Set_HuffmanTable_Info(u4JDecID,(JDEC_JFIF_DHT_T *)&(prJPEG_JFIF_HEADER->rDHT));

            x_memcpy(&(prJPEG_JFIF_HEADER->rSOS), &(prJPEG_JFIF_HEADER->arSOS[u4ScanCnt + 1]), 
                sizeof(JPEG_JFIF_SOS_T));
            i4JDEC_HAL_Set_Scan_Info(u4JDecID,&(prJPEG_JFIF_HEADER->rSOS));
            
            i4JDEC_HAL_Set_Read_Pointer(u4JDecID,(UINT32)(prJPEG_JFIF_HEADER->pu1CurrRead));
            u4ScanCnt++;
        }

    } while((eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE) ||
            (eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT));

#if 0
    if(eDecodingMode == JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE)
    {
        x_memcpy((void *)u4ColorBufY, (void *)_u4EnhanceBufY,JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT);
        x_memcpy((void *)u4ColorBufCb, (void *)_u4EnhanceBufCb,JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT);
        x_memcpy((void *)u4ColorBufCr, (void *)_u4EnhanceBufCr,JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT);
    }
#endif    

    UNUSED(_u4ErrMCUNum);
    UNUSED(i4Ret);
    i4JDEC_HAL_Uninit(u4JDecID);

    return i4Ret;
}

INT32 i4JpegCompareBuf(void)
{
    UINT32 u4X,u4Y;
    UINT32 u4ColorBufY = _u4ColorBufY;
    UINT32 u4ColorBufCb = _u4ColorBufCb;
    UINT32 u4ColorBufCr = _u4ColorBufCr;
    UINT32 u4ColorBufWidth = _u4ColorBufWidth;
    UINT32 u4GoldenBufY = _u4ColorBufY2;
    UINT32 u4GoldenBufCb = _u4ColorBufCb;
    UINT32 u4GoldenBufCr = _u4ColorBufCr2;
    UINT32 u4GoldenBufWidth = _u4ColorBufWidth;
    BYTE bColor,bGoldenColor;
    UINT32 u4XSample,u4YSample;
    UINT32 u4GoldenXSample,u4GoldenYSample;
    const UINT32 u4YThreshold = 0;
    const UINT32 u4CbThreshold = 0;
    const UINT32 u4CrThreshold = 0;
    UINT32 u4MaxErr;
    UINT32 u4MaxErrX=0,u4MaxErrY=0;
    UINT32 u4Err;

    _u4DiffCnt = 0;

    // Compare Y
    u4XSample = _u4XSampleY;
    u4YSample = _u4YSampleY;
    u4GoldenXSample = u4XSample;
    u4GoldenYSample = u4YSample;
    u4MaxErr = 0;
    for(u4Y=0; u4Y<_u4ImageHeight; u4Y++)
        for(u4X=0; u4X<_u4ImageWidth; u4X++)
        {
            // Get HW decoded color
            bColor = *(BYTE *)(u4ColorBufY + (u4Y>>u4YSample)*u4ColorBufWidth + (u4X>>u4XSample));

            // Get Goldedn color
            bGoldenColor = *(BYTE *)(u4GoldenBufY + (u4Y>>u4GoldenYSample)*u4GoldenBufWidth + (u4X>>u4GoldenXSample));

            if(((UINT32)bColor > ((UINT32)bGoldenColor+u4YThreshold)) || ((UINT32)bColor < ((UINT32)bGoldenColor-u4YThreshold)))
            {
                //Printf("[Jpeg][Verify] Compare Y, bColor(%d,%d) = %d, bGoldenColor(%d,%d) = %d",u4X,u4Y,bColor,u4X,u4Y,bGoldenColor);
                _u4DiffCnt++;
                if(bColor > bGoldenColor)
                    u4Err = bColor - bGoldenColor;
                else
                    u4Err = bGoldenColor - bColor;

                if(u4Err > u4MaxErr)
                {
                    u4MaxErr = u4Err;
                    u4MaxErrX = u4X;
                    u4MaxErrY = u4Y;
                }
            }
        }
    if(u4MaxErr>0)
        Printf("[Jpeg][Verify] Compare Y, Max err = %d, pixel(%d,%d)",u4MaxErr,u4MaxErrX,u4MaxErrY);


    // Compare Cb
    u4XSample = _u4XSampleCb;
    u4YSample = _u4YSampleCb;
    u4GoldenXSample = u4XSample;
    u4GoldenYSample = u4YSample;
    u4MaxErr = 0;
    for(u4Y=0; u4Y<_u4ImageHeight; u4Y++)
        for(u4X=0; u4X<_u4ImageWidth; u4X++)
        {
            // Get HW decoded color
            bColor = *(BYTE *)(u4ColorBufCb + (u4Y>>u4YSample)*u4ColorBufWidth + (u4X>>u4XSample));

            // Get Goldedn color
            bGoldenColor = *(BYTE *)(u4GoldenBufCb + (u4Y>>u4GoldenYSample)*u4GoldenBufWidth + (u4X>>u4GoldenXSample));

            if(((UINT32)bColor > ((UINT32)bGoldenColor+u4CbThreshold)) || ((UINT32)bColor < ((UINT32)bGoldenColor-u4CbThreshold)))
            {
                //Printf("[Jpeg][Verify] Compare Cb, bColor(%d,%d) = %d, bGoldenColor(%d,%d) = %d",u4X,u4Y,bColor,u4X,u4Y,bGoldenColor);
                _u4DiffCnt++;

                if(bColor > bGoldenColor)
                    u4Err = bColor - bGoldenColor;
                else
                    u4Err = bGoldenColor - bColor;

                if(u4Err > u4MaxErr)
                {
                    u4MaxErr = u4Err;
                    u4MaxErrX = u4X;
                    u4MaxErrY = u4Y;
                }
            }
        }
    if(u4MaxErr>0)
        Printf("[Jpeg][Verify] Compare Cb, Max err = %d, pixel(%d,%d)",u4MaxErr,u4MaxErrX,u4MaxErrY);


    // Compare Cr
    u4XSample = _u4XSampleCr;
    u4YSample = _u4YSampleCr;
    u4GoldenXSample = u4XSample;
    u4GoldenYSample = u4YSample;
    u4MaxErr = 0;
    for(u4Y=0; u4Y<_u4ImageHeight; u4Y++)
        for(u4X=0; u4X<_u4ImageWidth; u4X++)
        {
            // Get HW decoded color
            bColor = *(BYTE *)(u4ColorBufCr + (u4Y>>u4YSample)*u4ColorBufWidth + (u4X>>u4XSample));

            // Get Goldedn color
            bGoldenColor = *(BYTE *)(u4GoldenBufCr + (u4Y>>u4GoldenYSample)*u4GoldenBufWidth + (u4X>>u4GoldenXSample));

            if(((UINT32)bColor > ((UINT32)bGoldenColor+u4CrThreshold)) || ((UINT32)bColor < ((UINT32)bGoldenColor-u4CrThreshold)))
            {
                //Printf("[Jpeg][Verify] Compare Cr, bColor(%d,%d) = %d, bGoldenColor(%d,%d) = %d",u4X,u4Y,bColor,u4X,u4Y,bGoldenColor);
                _u4DiffCnt++;

                if(bColor > bGoldenColor)
                    u4Err = bColor - bGoldenColor;
                else
                    u4Err = bGoldenColor - bColor;

                if(u4Err > u4MaxErr)
                {
                    u4MaxErr = u4Err;
                    u4MaxErrX = u4X;
                    u4MaxErrY = u4Y;
                }
            }
        }
    if(u4MaxErr>0)
        Printf("[Jpeg][Verify] Compare Cr, Max err = %d, pixel(%d,%d)",u4MaxErr,u4MaxErrX,u4MaxErrY);

    if(_u4DiffCnt>0)
        return -1;
    else
        return 0;
}

#ifdef JPEG_AUTO_TEST
static UINT32 _8520CheckSum(void* pvData, UINT32 u4ByteLen)
{
	UINT32* pu4Data;
	UINT32 u4Sum = 0;

	ASSERT(pvData!=NULL);
	pu4Data = (UINT32*)pvData;
	while (u4ByteLen > 3)
	{
		u4Sum += *pu4Data;
		if (u4ByteLen >= 4)
		{
			u4ByteLen -= 4;
		}
		else
		{
			u4ByteLen = 0;
		}
		pu4Data++;
	}

	return u4Sum;
}

INT32 i4JpegDecode(void)
{
    JPEGHANDLE hParser;
    UINT32 u4Sum[3];
    //static UINT32 u4FirstHwId = 0;
#ifdef JDEC_VFY_TEST_SPEED
    UINT32 u4I,u4StcStart,u4StcEnd,u4StcTotal;
    UINT32 u4StartStcL,u4StartStcH,u4Stc;
#endif
    _fgVerifyFlow = TRUE;

    _prJPEG_JFIF_HEADER = JFIF_AllocInstance();
	if(!_prJPEG_JFIF_HEADER)
	  return -1;

    hParser = (JPEGHANDLE)_prJPEG_JFIF_HEADER;

//    x_memset((void *)_prJPEG_JFIF_HEADER,0,sizeof(JPEG_JFIF_HEADER_T));

    JFIF_New(hParser,_EmuJpgFifo,JDEC_FIFO_SIZE);

    if((_prJPEG_JFIF_HEADER->rSOF.u2ImageWidth == 0) || (_prJPEG_JFIF_HEADER->rSOF.u2ImageHeight == 0))
        return -1;

    _u4ImageWidth = _prJPEG_JFIF_HEADER->rSOF.u2ImageWidth;
    _u4ImageHeight = _prJPEG_JFIF_HEADER->rSOF.u2ImageHeight;

    _u4XSampleY = 1;
    _u4YSampleY = 1;
    if(_prJPEG_JFIF_HEADER->rSOF.u1NumComponents>1)
    {
        _u4XSampleCb = _prJPEG_JFIF_HEADER->rSOF.arSofComp[0].u1HSampFactor/_prJPEG_JFIF_HEADER->rSOF.arSofComp[1].u1HSampFactor;
        _u4YSampleCb = _prJPEG_JFIF_HEADER->rSOF.arSofComp[0].u1VSampFactor/_prJPEG_JFIF_HEADER->rSOF.arSofComp[1].u1VSampFactor;
    }
    if(_prJPEG_JFIF_HEADER->rSOF.u1NumComponents>2)
    {
        _u4XSampleCr = _prJPEG_JFIF_HEADER->rSOF.arSofComp[0].u1HSampFactor/_prJPEG_JFIF_HEADER->rSOF.arSofComp[2].u1HSampFactor;
        _u4YSampleCr = _prJPEG_JFIF_HEADER->rSOF.arSofComp[0].u1VSampFactor/_prJPEG_JFIF_HEADER->rSOF.arSofComp[2].u1VSampFactor;
    }

    {
        JDEC_HAL_DEC_MODE_T eDecodingMode;

        if(_prJPEG_JFIF_HEADER->rSOF.eJpegFormat == E_JPG_PROGRESSIVE_HUFFMAN)
        {
            //eDecodingMode = JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE;
            eDecodingMode = JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT;
        }
        else
        {
            eDecodingMode = JDEC_HAL_DEC_MODE_BASELINE_MCU;
        }
		
        x_memset((void *)_u4ColorBufY,0x0,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        x_memset((void *)_u4ColorBufCb,0x0,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        x_memset((void *)_u4ColorBufCr,0x0,(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
#ifdef CC_MT5395
        HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(_u4ColorBufY), (UINT32)(_u4ColorBufY + JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(_u4ColorBufCb), (UINT32)(_u4ColorBufCb + JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(_u4ColorBufCr), (UINT32)(_u4ColorBufCr + JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
#else
		HalFlushInvalidateDCache();
#endif
        vJpegDecode(0,eDecodingMode,_u4ColorBufY,_u4ColorBufCb,_u4ColorBufCr,_u4ColorBufY2,_u4ColorBufCb2,_u4ColorBufCr2);
        u4Sum[0] = _8520CheckSum((void *)_u4ColorBufY, (UINT32)(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        u4Sum[1] = _8520CheckSum((void *)_u4ColorBufCb, (UINT32)(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        u4Sum[2] = _8520CheckSum((void *)_u4ColorBufCr, (UINT32)(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT));
        printf(" Y Checksum: 0x%x\n", u4Sum[0]);
        printf("Cr Checksum: 0x%x\n", u4Sum[1]);
        printf("Cb Checksum: 0x%x\n", u4Sum[2]);
        //printf("Total Checksum: 0x%x\n", u4Sum[0] + u4Sum[1] + u4Sum[2]);
#ifdef JPEG_AUTO_TEST
       {
        
		INT32 arCheckFileParam[10];
		INT32 arCheckSum[4],i4Index=0;
		UINT32 fmt=0;
		BOOL fgCheckPass=TRUE;
		arCheckSum[0]=3;
		arCheckSum[1]=u4Sum[0];
		arCheckSum[2]=u4Sum[1];
		arCheckSum[3]=u4Sum[2];
 		_JpgVerifyCmdSetParam(1,(INT32)arCheckSum);
		if(i4CurDecIndex>=0)
		{
			if(_arProgressive[i4CurDecIndex].u4rCheckSum[0]==0 &&
			   _arProgressive[i4CurDecIndex].u4rCheckSum[1]==0	&&
			   _arProgressive[i4CurDecIndex].u4rCheckSum[2]==0)
			{
			   printf("No golden checksum to do compare\n");
			}
			else
			{
				for(i4Index=0;i4Index<3;i4Index++)
				{
				   if(_arProgressive[i4CurDecIndex].u4rCheckSum[i4Index]!=u4Sum[i4Index])
				   {
					  printf("Checksum=(Golden:0x%x,Result:0x%x\n",_arProgressive[i4CurDecIndex].u4rCheckSum[i4Index],u4Sum[i4Index]);
					  fgCheckPass=FALSE;
				   }
				}
				
				if(!fgCheckPass)
				{
				   printf("Progressive:%s Checksum Fail\n",_arProgressive[i4CurDecIndex].szFileName);
				}
				else
				{
				   printf("Progressive Checksum Pass\n");
				}
			}
		}

        arCheckFileParam[0]=3;
	    arCheckFileParam[1]=5;
	    arCheckFileParam[2]=_u4ColorBufY;
	    arCheckFileParam[3]=(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT);

		arCheckFileParam[4]=6;
		arCheckFileParam[5]=_u4ColorBufCb;
		arCheckFileParam[6]=(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT);

		arCheckFileParam[7]=7;
		arCheckFileParam[8]=_u4ColorBufCr;
		arCheckFileParam[9]=(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT);
		
        _JpgVerifyCmdSetParam(2,(INT32)arCheckFileParam);
			
            if(_u4XSampleCb==2)
            {
               if(_u4YSampleCb==2)
			   	  fmt=3;
			   else if(_u4YSampleCb==1)
			   	  fmt=2;
            }
			else if(_u4XSampleCb==1)
		    {
			  if(_u4YSampleCb==2)
			    fmt=1;
			  else if(_u4YSampleCb==1)
			    fmt=0;
			}
	        _JpgVerifyCmdSetParam(22,fmt);
       }
#endif
    }
    JFIF_FreeInstance(_prJPEG_JFIF_HEADER);
	_prJPEG_JFIF_HEADER=NULL;
    return 0;
}

INT32 i4LoadJpegFile(UINT32 u4Idx)
{
    printf("d.load.binary \"%s/%s\" 0x%08x /ny\n", "D:/jpg_test/progressive", 
        _arProgressive[u4Idx].szFileName, (UINT32)_EmuJpgFifo);
#ifdef T32_TEST  
    HalFlushInvalidateDCache();
    UNUSED(T32_HostExec(1, "d.load.binary \"%s/%s\" 0x%08x /ny", "D:/jpg_test/progressive", 
        _arProgressive[u4Idx].szFileName, (UINT32)_EmuJpgFifo));
    HalFlushInvalidateDCache();
#endif
    return _arProgressive[u4Idx].u4ByteSize;
}

VOID vJpegVertifyAutoCreateBuffer(UCHAR *pBufferAddr)
{
    if(!_EmuJpgFifo)
    {
       _EmuJpgFifo=pBufferAddr;
		//_EmuJpgFifo = BSP_AllocAlignedDmaMemory(JDEC_FIFO_SIZE,2048);
		//x_memset(_EmuJpgFifo,0,JDEC_FIFO_SIZE);
    }
	   
	//if(pBufferAddr)
    //     *pBufferAddr=_EmuJpgFifo;
       return;
}

INT32 vJpegVerifyAuto(INT32 index,INT32 i4JpgSize)
{
     static BOOL sfgFirst = TRUE;
     //UINT32 u4I,i4FileSize;
	 
    if (sfgFirst)
    {
        sfgFirst = FALSE;
        _u4ColorBufY = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
        _u4ColorBufCb = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
        _u4ColorBufCr = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);

        _u4ColorBufY2 = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
        _u4ColorBufCb2 = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
        _u4ColorBufCr2 = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
   }
   
   if(index>=0)
   {
       _i4JpegFileSize = i4LoadJpegFile(index);
   }
   else
   {
      _i4JpegFileSize=i4JpgSize;
   }
  
   if(_i4JpegFileSize < 0)
   {
       Printf("[Jpeg][Verify] Load File fail");
       return -1;
   }
   
   i4CurDecIndex=index;
   if(i4JpegDecode() <0)
   {
       Printf("[Jpeg][Verify] Decode %s fail",_arProgressive[index].szFileName);
	return -1;
   }
   
   Printf("[Jpeg][Verify] Verify complete.\n");
   return 0;
}

INT32 vJpegVerify(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Idx, u4I, u4Times;
    static BOOL sfgFirst = TRUE;
    if (i4Argc < 3)
    {
        for (u4I = 0; u4I < LIST_MAX; u4I++)
        {
            printf("file list: (%d) \"%s/%s\" /ny\n", u4I, "D:/jpg_test/progressive", 
                _arProgressive[u4I].szFileName);
        }
        return -1;
    }
#ifdef CC_EMULATION    
    u4Idx = StrToInt(szArgv[1]);
    u4Times = StrToInt(szArgv[2]);
#else
    u4Idx = (UINT32)0;
    u4Times = (UINT32)1;
#endif    
    if (sfgFirst)
{
        sfgFirst = FALSE;
    // Allocate Memory
    _EmuJpgFifo = BSP_AllocAlignedDmaMemory(JDEC_FIFO_SIZE,2048);

    _u4ColorBufY = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
    _u4ColorBufCb = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
    _u4ColorBufCr = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);

    _u4ColorBufY2 = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
    _u4ColorBufCb2 = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
    _u4ColorBufCr2 = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);

    //_u4FrameBufY = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
    //_u4FrameBufC = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);

    //_u4EnhanceBufY = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
    //_u4EnhanceBufCb = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
    //_u4EnhanceBufCr = (UINT32)BSP_AllocAlignedDmaMemory(JDEC_COLOR_BUF_WIDTH*JDEC_COLOR_BUF_HEIGHT,2048);
    }
    x_memset(_EmuJpgFifo,0,JDEC_FIFO_SIZE);

    for(u4I=0; u4I < u4Times; u4I++)
    {
        //Printf("[Jpeg][Verify] Load File %s",_abJpegFileList[_u4CurJpegFileListNum]);
        _i4JpegFileSize = i4LoadJpegFile(u4Idx);
        if(_i4JpegFileSize < 0)
        {
            Printf("[Jpeg][Verify] Load File fail");
            continue;
        }

        Printf("[Jpeg][Verify] Decode Jpeg by HW");
        if(i4JpegDecode() >= 0)
        {
/*#if JDEC_VFY_TEST_PIC_MODE
            i4SaveFrameBuf();
#elif JDEC_VFY_TEST_PROGRESSIVE_PRELOAD
            i4SaveFrameBuf();
#else*/
            /*if(i4JpegCompareBuf()<0)
                Printf("[Jpeg][Verify] Hw1 does not match Hw2");

            i4SaveColorBuf();*/
//#endif
        } else
        {
        }
    }

    Printf("[Jpeg][Verify] Verify complete.");
#ifndef CHIPVERIFY
    // Free Memory
    BSP_FreeAlignedDmaMemory((void *)_u4ColorBufY);
    BSP_FreeAlignedDmaMemory((void *)_u4ColorBufCb);
    BSP_FreeAlignedDmaMemory((void *)_u4ColorBufCr);

    BSP_FreeAlignedDmaMemory((void *)_u4ColorBufY2);
    BSP_FreeAlignedDmaMemory((void *)_u4ColorBufCb2);
    BSP_FreeAlignedDmaMemory((void *)_u4ColorBufCr2);

    BSP_FreeAlignedDmaMemory((void *)_u4FrameBufY);
    BSP_FreeAlignedDmaMemory((void *)_u4FrameBufC);

    BSP_FreeAlignedDmaMemory((void *)_EmuJpgFifo);

    BSP_FreeAlignedDmaMemory((void *)_u4EnhanceBufY);
    BSP_FreeAlignedDmaMemory((void *)_u4EnhanceBufCb);
    BSP_FreeAlignedDmaMemory((void *)_u4EnhanceBufCr);
#endif  
    return 0;
}

#endif 

INT32 vJpegProgDec(JPEGHANDLE hParser, void **pvCoefBuffer, void **pvOutBuffer, void **pvNzBuffer)
{
    JDEC_HAL_DEC_MODE_T eDecodingMode = JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT;
    INT32 i4Ret;

    _fgVerifyFlow = FALSE;
    _fgStop = FALSE;

    _prJPEG_JFIF_HEADER = (JPEG_JFIF_HEADER_T *)hParser;

    if((_prJPEG_JFIF_HEADER->rSOF.u2ImageWidth == 0) || (_prJPEG_JFIF_HEADER->rSOF.u2ImageHeight == 0))
        return -1;

    _u4ImageWidth = _prJPEG_JFIF_HEADER->rSOF.u2ImageWidth;
    _u4ImageHeight = _prJPEG_JFIF_HEADER->rSOF.u2ImageHeight;
    
      if ((_u4ImageWidth * _u4ImageHeight) > JDEC_COLLECT_LIMIT)
      {
        eDecodingMode = JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE;
      }
	
	
    _u4XSampleY = 1;
    _u4YSampleY = 1;
    if(_prJPEG_JFIF_HEADER->rSOF.u1NumComponents > 1)
    {
        _u4XSampleCb = _prJPEG_JFIF_HEADER->rSOF.arSofComp[0].u1HSampFactor/_prJPEG_JFIF_HEADER->rSOF.arSofComp[1].u1HSampFactor;
        _u4YSampleCb = _prJPEG_JFIF_HEADER->rSOF.arSofComp[0].u1VSampFactor/_prJPEG_JFIF_HEADER->rSOF.arSofComp[1].u1VSampFactor;
    }
    if(_prJPEG_JFIF_HEADER->rSOF.u1NumComponents > 2)
    {
        _u4XSampleCr = _prJPEG_JFIF_HEADER->rSOF.arSofComp[0].u1HSampFactor/_prJPEG_JFIF_HEADER->rSOF.arSofComp[2].u1HSampFactor;
        _u4YSampleCr = _prJPEG_JFIF_HEADER->rSOF.arSofComp[0].u1VSampFactor/_prJPEG_JFIF_HEADER->rSOF.arSofComp[2].u1VSampFactor;
    }

    _u4ColorBufY = (UINT32)pvOutBuffer[0];
    _u4ColorBufCb = (UINT32)pvOutBuffer[1];
    _u4ColorBufCr = (UINT32)pvOutBuffer[2];

    _u4ColorBufY2 = (UINT32)pvOutBuffer[6];
    _u4ColorBufCb2 = (UINT32)pvOutBuffer[7];
    _u4ColorBufCr2 = (UINT32)pvOutBuffer[8];

    _i4JpegFileSize = (INT32)_prJPEG_JFIF_HEADER->u4ImageByteSize;
    _EmuJpgFifo = (BYTE *)_prJPEG_JFIF_HEADER->pu1JpegImage;
    
    i4JDEC_HAL_Set_Buf(0, pvCoefBuffer, pvNzBuffer);

    i4Ret = vJpegDecode(0,eDecodingMode,_u4ColorBufY,_u4ColorBufCb,_u4ColorBufCr,_u4ColorBufY2,_u4ColorBufCb2,_u4ColorBufCr2);
//                                                           _u4FrameBufY,_u4FrameBufC,
    //                                                       JDEC_COLOR_BUF_WIDTH,JDEC_COLOR_BUF_HEIGHT);

    i4JDEC_HAL_Unset_Buf(0);

    return i4Ret;
}

