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
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_drv_cli.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_fm.h"
#include "fbm_drvif.h"
#include "drvcust_if.h"

#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#include "vdec_vp8webp_if.h"

#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"

/******************************************************************************
* Function        : 
******************************************************************************/
#ifdef __MODEL_slt__
static INT32 _WebpCmdSlt(INT32 i4Argc, const CHAR ** szArgv);
#endif

/******************************************************************************
* Variable        : cli "webp" command table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(WEBP)

static CLI_EXEC_T _arWebpCmdTbl[] =
{
#if defined (CC_EMULATION) || defined (__MODEL_slt__)
    #ifdef __MODEL_slt__
    {"slt",        NULL,    _WebpCmdSlt, NULL,        "slt", CLI_SUPERVISOR},
    #endif
#endif
    CLIMOD_DEBUG_CLIENTRY(WEBP),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Webp)
{
    "webp",
    NULL,
    NULL,
    _arWebpCmdTbl,
    "Webp command",
    CLI_SUPERVISOR
};

#ifdef __MODEL_slt__
/******************************************************************************
* Typedef
******************************************************************************/

typedef struct
{
  UINT32 u4ColorType;
  UINT32 u4DecMode;
  UINT32 u4TargW;
  UINT32 u4TargH;
  UINT32 u4TarBuffer;
  UINT32 u4SrcBuffer;
  UINT32 u4DecOutYAddr;
  UINT32 u4DecOutCAddr;
  UINT32 u4GldAddr;
  UINT32 u4WorkBufSize;
}WEBP_CFG_T;

/******************************************************************************
* Global variable
******************************************************************************/

static WEBP_CFG_T _rWebCfg=
{
  WEBP_COLOR_AYCBCR8888,
  WEBP_DEC_MODE_PIC,
  480,
  320,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
};

/******************************************************************************
* Implementation        : helper functions for commands parsing
******************************************************************************/
static UINT32 _VdecWebPCallBack(UINT32 u4Tag, void *pvData, WEBP_NOTIFY_TYPE u4CbType)
{
   WEBP_CFG_T *prWebCfg=(WEBP_CFG_T *)u4Tag;

   if(u4CbType==WEBP_NOTIFY_TYPE_REQDATA)
   {
      WEBP_DATA_REQ_INFO *prDataReqInfo=(WEBP_DATA_REQ_INFO *)pvData;
      x_memcpy((void *)prDataReqInfo->u4WritePosition,(void *)(prWebCfg->u4SrcBuffer+prDataReqInfo->u4FileOffset),prDataReqInfo->u4Size);
	  HalFlushInvalidateDCache();
	  return prDataReqInfo->u4Size;
   }
   return 0;
}

static INT32 _WebpCmdSlt(INT32 i4Argc, const CHAR ** szArgv)
{
    char aFileName[] = "//mnt//usb_0//slt.webp";
    UINT32 u4FileSize = 3632;
    UINT32 aCrc[] = {0xC5526D56, 0xECA4E2BE, 0xE8973CD6, 0x9A6A7391,
                     0xFDF494A6, 0xB48EC26E, 0x1E93BA88, 0xDC1859B0};
    UINT32 u4RzCrc = 0x0055F4C7;
    HANDLE_T hImgFile;
    FBM_POOL_T* pFbmPool;
    WEBP_DECIN_PARAM_T rInParam;
    WEBP_DECOUT_PARAM_T rOutParam;
    WEBP_CFG_T *prWebCfg=&_rWebCfg;
    UINT32 u4ByteRead;
    INT32 i4Rtn;
    UINT8 *pbTarBuf, *pbSrcBuf;
    
    // Init other module
    CLI_Parser("fbm.i");
    CLI_Parser("vdec.i");
    
    // Init working buffer
    pbTarBuf = BSP_AllocAlignedDmaMemory(prWebCfg->u4TargW * prWebCfg->u4TargH * 4, 1024);
    pbSrcBuf = BSP_AllocAlignedDmaMemory(u4FileSize, 1024);
    //pbTarBuf = x_mem_alloc(prWebCfg->u4TargW * prWebCfg->u4TargH * 4 + 1024);
    //pbSrcBuf = x_mem_alloc(u4FileSize + 1024);
        
    if (!pbTarBuf || !pbSrcBuf)
    {
        Printf("Alloc memory for target buffer or src buffer fail\n");
        return -1;
    }
       
    // Load webp file
    if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, aFileName, FM_READ_ONLY, 0777, FALSE, &hImgFile))
    {
        Printf("Open file fail!\n");
        return -1;
    }
    if (FMR_OK != x_fm_read(hImgFile, pbSrcBuf, u4FileSize, &u4ByteRead))
    {
        Printf("Read file fail!\n");
        return -1;
    }
    x_fm_close(hImgFile);
    
    // Init parameter for WebP decoder
    prWebCfg->u4SrcBuffer = (UINT32)pbSrcBuf;
    prWebCfg->u4TarBuffer = (UINT32)pbTarBuf;
    x_memset(&rInParam, 0, sizeof(WEBP_DECIN_PARAM_T));
    x_memset(&rOutParam, 0, sizeof(WEBP_DECOUT_PARAM_T));
    
    rInParam.u4Tag = (UINT32)prWebCfg;
    rInParam.u4DestBuffer = prWebCfg->u4TarBuffer;
    rInParam.eColorMode = (WEBP_COLOR_MODE)prWebCfg->u4ColorType;
    rInParam.u4StartX = 0;
    rInParam.u4StartY = 0;
    rInParam.u4Width = prWebCfg->u4TargW;
    rInParam.u4Height = prWebCfg->u4TargH;
    rInParam.u4FileOffset = 20;
    rInParam.u4FileSize = u4FileSize;
    rInParam.u4Pitch = ((rInParam.u4Width+15)/16)*16;
    rInParam.u4NotifyInfo = 0;

    if((prWebCfg->u4ColorType == WEBP_COLOR_AYCBCR8888) || (prWebCfg->u4ColorType == WEBP_COLOR_ARGB8888))
    {
        rInParam.u4Pitch = rInParam.u4Pitch<<2;
    }

    // Init WebP decoder
    pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_JPEG);
    if(pFbmPool == NULL)
    { 
        Printf("Can't get jpeg fbm type \n");
        return -1;
    }
    else
    { 
        Printf("WorkingAdd=0x%x,Size=0x%x,ChangeSize=0x%x\n", pFbmPool->u4Addr, pFbmPool->u4Size, _rWebCfg.u4WorkBufSize);
        if(_rWebCfg.u4WorkBufSize)
        {
            pFbmPool->u4Size = _rWebCfg.u4WorkBufSize;
        }
        _VDEC_Vp8WebPInit(0, _VdecWebPCallBack, pFbmPool->u4Addr, pFbmPool->u4Size);
    }
    
    i4Rtn = 0;
    
    // WebP decode
    if(_VDEC_Vp8WebPDecStart(0, (WEBP_DEC_MODE)prWebCfg->u4DecMode, &rInParam, &rOutParam))
    {
        if (rOutParam.u4Ycrc[0]==aCrc[0] && rOutParam.u4Ycrc[1]==aCrc[1]
         && rOutParam.u4Ycrc[2]==aCrc[2] && rOutParam.u4Ycrc[3]==aCrc[3]
         && rOutParam.u4Ccrc[0]==aCrc[4] && rOutParam.u4Ccrc[1]==aCrc[5]
         && rOutParam.u4Ccrc[2]==aCrc[6] && rOutParam.u4Ccrc[3]==aCrc[7]
         && rOutParam.u4RzCrc == u4RzCrc)
        {
            Printf("\n\nWEBPOK\n");
        }
        else
        {
            Printf("WEBPFAIL\n");
            i4Rtn = -1;
        }
    }
    else
    {
        Printf("WEBPFAIL\n");
        i4Rtn = -1;
    }
       
    //x_mem_free(pbTarBuf);
    //x_mem_free(pbSrcBuf);
    
    return i4Rtn;
}
#endif // __MODEL_slt__

#endif // defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
