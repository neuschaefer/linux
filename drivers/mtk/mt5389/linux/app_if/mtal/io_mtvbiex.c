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
 * $RCSfile: io_mtvbiex.c,v $
 * $Revision: #2 $
 *---------------------------------------------------------------------------*/
/** @file MTVDECEX.c
 *  APIs of VBI modules.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mtvbiex.h"
#include <linux/mtal_ioctl.h>
#include <linux/mm.h> 
#include "nptv_if.h"
#include "cb_data.h"
#include "cb_low.h"
#include "drv_video.h"
#include "video_timing.h"
#include "mtutil.h"
#include "fbm_drvif.h"
#include "drv_vbi.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define WSS_TYPE MTVBI_WSS625
#define TTX_TYPE MTVBI_EU_WST
#define CC_TYPE  MTVBI_US_CC
#define UNKNOW_WSS 0x0

#define MT_VBI_CC_CHECK_PARITY 0

#if 0
#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                \
    if (!access_ok(VERIFY_READ, (void __user *)arg,        \
                   sizeof(int)))                           \
    {                                                      \
        printk("mtal_ioctl_mtvbiex argument error\n");     \
        return -1;                                         \
    }                                                      \
    
#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)             \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,  \
                       sizeof(int)))                       \
    {                                                      \
        printk("mtal_ioctl_mtvbiex argument error\n");     \
        return -1;                                         \
    }                                                      \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvbiex argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvbiex argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvbiex argument error\n");     \
        return -1;                                          \
    }
#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvbiex argument error\n");      \
        return -1;                                          \
    }
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//static UINT8 _fgAVFieldIVS;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
static NPTV_PFN_VBI_INFO_NFY _MT_VbiInfo_Cb_driver (BOOL fgAVFieldIVS)
{
    MTVBI_CB_T rFeVBICb = {0};

    rFeVBICb.u4Type = MTVBI_CB_INFO_DATA;
    rFeVBICb.CbData.VBIInfo.u1AVFieldIVS = fgAVFieldIVS;

    _CB_PutEvent(CB_MTAL_VBI_TRIGGER, sizeof (MTVBI_CB_T),&rFeVBICb);    

    return 0;
}

static NPTV_PFN_VPS_NFY _MT_VPS_Cb_driver(UINT8 au1Data[])
{
    MTVBI_CB_T rFeVBICb = {0};

    /* no VPS data for nrFramesNoData consecutive frames */
    if ((au1Data[0] == 0xff) && (au1Data[1] == 0xff) && (au1Data[2] == 0xff) && (au1Data[3] == 0xff))
    {
         rFeVBICb.u4Type = MTVBI_CB_NO_VPS_DATA;
    }
    else
    {
         rFeVBICb.u4Type = MTVBI_CB_VPS_DATA;
         memcpy ( (UINT8 *)rFeVBICb.CbData.VPSInfo.u1VPSData, au1Data, 13);
    }
    
    _CB_PutEvent(CB_MTAL_VBI_TRIGGER, sizeof (MTVBI_CB_T),&rFeVBICb);    

    return 0;
}

static NPTV_PFN_WSS_NFY _MT_WSS_Cb_driver(UINT8 u1Path, UINT32 u4Data)
{
    MTVBI_CB_T rFeVBICb = {0};

    /* no WSS data for nrFramesNoData consecutive frames */
    if (u4Data == UNKNOW_WSS)
    {
         rFeVBICb.u4Type = MTVBI_CB_NO_WSS_DATA;
    }
    else
    {
         rFeVBICb.u4Type = MTVBI_CB_WSS_DATA;
         rFeVBICb.CbData.WSSInfo.u4Path = u1Path;
         rFeVBICb.CbData.WSSInfo.u4WSS_Type = WSS_TYPE; 
         rFeVBICb.CbData.WSSInfo.u4WSSData = u4Data;
    }
    
    _CB_PutEvent(CB_MTAL_VBI_TRIGGER, sizeof (MTVBI_CB_T),&rFeVBICb);    

    return 0;
}

static NPTV_PFN_TTX_NFY _MT_TTX_Cb_driver(UINT8 u1Path, UINT32 u4PktsAvail, UINT32 readptr)
{
    MTVBI_CB_T rFeVBICb = {0};
    
    /* no TTX data for nrFramesNoData consecutive frames */
    if (u4PktsAvail == 0)
    {
         rFeVBICb.u4Type = MTVBI_CB_NO_TTX_DATA;
    }
    else        
    {    
         //HalFlushInvalidateDCache(); //??
         HalInvalidateDCacheMultipleLine(VIRTUAL(readptr), u4PktsAvail*48);
         rFeVBICb.u4Type = MTVBI_CB_TTX_DATA;
         rFeVBICb.CbData.TTXInfo.u4TTX_Type = TTX_TYPE;
         rFeVBICb.CbData.TTXInfo.u4Path = u1Path;
         rFeVBICb.CbData.TTXInfo.u4PktsAvail = u4PktsAvail;
         rFeVBICb.CbData.TTXInfo.u4ReadPtr = readptr;
         //VBI_StartNotifyTTXOnOff(SV_VP_MAIN, FALSE, 0); ??
    }
    _CB_PutEvent(CB_MTAL_VBI_TRIGGER, sizeof (MTVBI_CB_T),&rFeVBICb);         

    return 0;
}

static NPTV_PFN_CC_NFY _MT_CC_Cb_driver(UINT8 u1Path, UINT8 u1data1, UINT8 u1data2, UINT8 u1field)
{

    MTVBI_CB_T rFeVBICb = {0};

    //Check Timing
    switch (bDrvVideoGetTiming(u1Path))
    {
        case MODE_525I:
        case MODE_525I_OVERSAMPLE:
        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
        	  break;		
        	  
        default:
        	  return  0;
    	  
    };

    #if MT_VBI_CC_CHECK_PARITY
    {
        UINT8 fgResult;
        
        fgResult = u1data1>>7;
        fgResult ^= (u1data1>>6)&0x01;
        fgResult ^= (u1data1>>5)&0x01;
        fgResult ^= (u1data1>>4)&0x01;
        fgResult ^= (u1data1>>3)&0x01;
        fgResult ^= (u1data1>>2)&0x01;
        fgResult ^= (u1data1>>1)&0x01;
        fgResult ^= (u1data1>>0)&0x01;
        if(fgResult == 0)
        {
            //printf("CC 1st Parity check error!!!\n");
            return 0;
        }
        fgResult = u1data2>>7;
        fgResult ^= (u1data2>>6)&0x01;
        fgResult ^= (u1data2>>5)&0x01;
        fgResult ^= (u1data2>>4)&0x01;
        fgResult ^= (u1data2>>3)&0x01;
        fgResult ^= (u1data2>>2)&0x01;
        fgResult ^= (u1data2>>1)&0x01;
        fgResult ^= (u1data2>>0)&0x01;
        if(fgResult == 0)
        {
            //printf("CC 2nd Parity check error!!!\n");
            return 0;
        }
    }
    #endif

    rFeVBICb.u4Type = MTVBI_CB_CC_DATA;
    rFeVBICb.CbData.CCInfo.u4CC_Type = CC_TYPE;
    rFeVBICb.CbData.CCInfo.u4Path = u1Path;
    rFeVBICb.CbData.CCInfo.u4Field   = u1field;
    rFeVBICb.CbData.CCInfo.u1CCData0 = u1data1;
    rFeVBICb.CbData.CCInfo.u1CCData1 = u1data2;

    _CB_PutEvent(CB_MTAL_VBI_TRIGGER, sizeof (MTVBI_CB_T),&rFeVBICb);         
   
    return 0;
}


static MT_RESULT_T _MTVDECEX_VBI_StartNotifyCCOnOff(unsigned long arg)
{
	MTVDO_PATH_T ePath;
	UINT8 u1OnOff;
    MTVDECEX_PFN_CC_NFY pfnCCnotify;

	MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    ePath = (MTVDO_PATH_T )rArg.ai4Arg[0];
    u1OnOff = (UINT8)rArg.ai4Arg[1];
    pfnCCnotify = (MTVDECEX_PFN_CC_NFY)rArg.ai4Arg[2];
    MTVBIEX_PRINT(" - ePath = %d, u1OnOff = %d, pfnCCnotify = 0x%x\n",
		(int)ePath,
		(int)u1OnOff,
		(int)pfnCCnotify);

    VBI_StartNotifyCCOnOff((UINT8)ePath, u1OnOff, (NPTV_PFN_CC_NFY)pfnCCnotify);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VBI_StartNotifyTTXOnOff(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    UINT8 u1OnOff;
    MTVDECEX_PFN_TTX_NFY pfnTTXnotify;

    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    u1OnOff = (UINT8)rArg.ai4Arg[1];
    pfnTTXnotify= (MTVDECEX_PFN_TTX_NFY)rArg.ai4Arg[2];
    MTVBIEX_PRINT(" - ePath = %d, u1OnOff = %d, pfnTTXnotify = 0x%x\n",
		(int)ePath,
		(int)u1OnOff,
		(int)pfnTTXnotify);

    VBI_StartNotifyTTXOnOff( (UINT8)ePath, u1OnOff, (NPTV_PFN_TTX_NFY)pfnTTXnotify);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VBI_StartNotifyWSSOnOff(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    UINT8 u1OnOff;
    MTVDECEX_PFN_WSS_NFY pfnWSSnotify;

    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    u1OnOff = (UINT8)rArg.ai4Arg[1];
    pfnWSSnotify = (MTVDECEX_PFN_WSS_NFY)rArg.ai4Arg[2];
    MTVBIEX_PRINT(" - ePath = %d, u1OnOff = %d, pfnWSSnotify = 0x%x\n",
		(int)ePath,
		(int)u1OnOff,
		(int)pfnWSSnotify);

    VBI_StartNotifyWSSOnOff((UINT8)ePath, u1OnOff, (NPTV_PFN_WSS_NFY)pfnWSSnotify);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VBI_StartNotifyVPSOnOff(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    UINT8 u1OnOff;
    MTVDECEX_PFN_VPS_NFY pfnVPSnotify;

    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    ePath = (MTVDO_PATH_T )rArg.ai4Arg[0];
    u1OnOff = (UINT8)rArg.ai4Arg[1];
    pfnVPSnotify = (MTVDECEX_PFN_VPS_NFY)rArg.ai4Arg[2];
    MTVBIEX_PRINT(" - ePath = %d, u1OnOff = %d, pfnVPSnotify = 0x%x\n",
		(int)ePath,
		(int)u1OnOff,
		(int)pfnVPSnotify);

    VBI_StartNotifyVPSOnOff(u1OnOff, pfnVPSnotify);
        
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VBI_SetVbiInfoNfy(unsigned long arg)
{
    INT32 i4Arg;
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, i4Arg);

    MTVBIEX_PRINT(" - Set VBI Info Nfy = 0x%x\n", (int)i4Arg);

    if (i4Arg == TRUE)
        NPTV_VBI_SetVbiInfoNfy((const NPTV_PFN_VBI_INFO_NFY) _MT_VbiInfo_Cb_driver);
    else
        NPTV_VBI_SetVbiInfoNfy(NULL);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VBI_SetTtxNfy(unsigned long arg)
{
    INT32 i4Arg;
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, i4Arg);

    MTVBIEX_PRINT(" - Set TTX Nfy = 0x%x\n", (int)i4Arg);

    if (i4Arg == TRUE)
        NPTV_VBI_SetTtxNfy((const NPTV_PFN_TTX_NFY) _MT_TTX_Cb_driver);
    else
        NPTV_VBI_SetTtxNfy(NULL);
    
    return MTR_OK;
    
}

static MT_RESULT_T _MTVDECEX_VBI_SetCcNfy(unsigned long arg)
{
    INT32 i4Arg;
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, i4Arg);

    MTVBIEX_PRINT(" - Set CC Nfy = 0x%x\n", (int)i4Arg);

    if (i4Arg == TRUE)
        NPTV_VBI_SetCcNfy((const NPTV_PFN_CC_NFY) _MT_CC_Cb_driver);
    else
        NPTV_VBI_SetCcNfy(NULL);

    return MTR_OK;
    
}

static MT_RESULT_T _MTVDECEX_VBI_SetVpsNfy(unsigned long arg)
{
    INT32 i4Arg;
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, i4Arg);
    
    MTVBIEX_PRINT(" - Set Vps Nfy = 0x%x\n", (int)i4Arg);

    if (i4Arg == TRUE)
        NPTV_VBI_SetVpsNfy((const NPTV_PFN_VPS_NFY) _MT_VPS_Cb_driver);
    else
        NPTV_VBI_SetVpsNfy(NULL);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VBI_SetWssNfy(unsigned long arg)
{
    INT32 i4Arg;
    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, i4Arg);

    MTVBIEX_PRINT(" - Set Wss Nfy = 0x%x\n", (int)i4Arg);

    if (i4Arg == TRUE)
        NPTV_VBI_SetWssNfy((const NPTV_PFN_WSS_NFY)_MT_WSS_Cb_driver);
    else
        NPTV_VBI_SetWssNfy(NULL);
 
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VBI_GetVBIFifoInfo(unsigned long arg)
{
    FBM_POOL_T* pVBI_Pool_Info;
    MTAL_IOCTL_2ARG_T    rArg;

    pVBI_Pool_Info = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_VBI);

    rArg.ai4Arg[0] = pVBI_Pool_Info->u4Addr;
    rArg.ai4Arg[1] = pVBI_Pool_Info->u4Size;

    MTVBIEX_PRINT("VBI FIFO BaseAddr = 0x%x, VBI FIFO Size = 0x%x\n", (int)rArg.ai4Arg[0], (int)rArg.ai4Arg[1]);

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_TO_USER_2ARG(arg, rArg);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VBI_SetVBILine(unsigned long arg)
{
    UINT8 bIsNTSC;
    MTVBI_SERVICE_T eMode;
    UINT8 u1LineNum;

    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    bIsNTSC = (UINT8)rArg.ai4Arg[0];
    eMode = (MTVBI_SERVICE_T)rArg.ai4Arg[1];
    u1LineNum = (UINT8)rArg.ai4Arg[2];
    MTVBIEX_PRINT(" - bIsNTSC = %d, eMode = %d, u1LineNum = %d\n",
		(int)bIsNTSC,
		(int)eMode,
		(int)u1LineNum);

    switch(eMode)
    {
        case MTVBI_TTX_SERVICE:
            break;
        case MTVBI_CC_SERVICE:
            VBI_SetLineInfoByAP(bIsNTSC, VBI_CC_SERVICE, u1LineNum);
            break;
        case MTVBI_CC2X_SERVICE:
            break;
        case MTVBI_WSS525_SERVICE:
            VBI_SetLineInfoByAP(bIsNTSC, VBI_WSS525_SERVICE, u1LineNum);
            break;
        case MTVBI_WSS625_SERVICE:
            VBI_SetLineInfoByAP(bIsNTSC, VBI_WSS625_SERVICE, u1LineNum);
            break;
        case MTVBI_VPS_SERVICE:
            VBI_SetLineInfoByAP(bIsNTSC, VBI_VPS_SERVICE, u1LineNum);
            break;
        default:
            break;
    }

    return MTR_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int mtal_ioctl_mtvbiex(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_OK;

	switch (cmd)
	{
        case MTAL_IO_VDECEX_VBI_STARTNOTIFYCCONOFF:
            return  _MTVDECEX_VBI_StartNotifyCCOnOff(arg);                    
            break;
        case MTAL_IO_VDECEX_VBI_STARTNOTIFYTTXONOFF:
            return _MTVDECEX_VBI_StartNotifyTTXOnOff(arg);                   
            break;
        case MTAL_IO_VDECEX_VBI_STARTNOTIFYWSSONOFF:
            return _MTVDECEX_VBI_StartNotifyWSSOnOff(arg);                    
            break;
        case MTAL_IO_VDECEX_VBI_STARTNOTIFYVPSONOFF:
            return _MTVDECEX_VBI_StartNotifyVPSOnOff(arg);                   
            break;
        case MTAL_IO_VDECEX_VBI_SETTTXNFY:
            return _MTVDECEX_VBI_SetTtxNfy(arg);                             
            break;
        case MTAL_IO_VDECEX_VBI_SETCCNFY:
            return _MTVDECEX_VBI_SetCcNfy(arg);                              
            break;
        case MTAL_IO_VDECEX_VBI_SETVPSNFY:
            return _MTVDECEX_VBI_SetVpsNfy(arg);                              
            break;
        case MTAL_IO_VDECEX_VBI_SETWSSNFY:
            return _MTVDECEX_VBI_SetWssNfy(arg);  
			break;                            

        case MTAL_IO_VDECEX_VBI_SETINFONFY:
            return _MTVDECEX_VBI_SetVbiInfoNfy(arg);  
			break;                            

        case MTAL_IO_VDECEX_VBI_GETVBIFIFOINFO:
            return _MTVDECEX_VBI_GetVBIFifoInfo(arg);
            break;
        case MTAL_IO_VDECEX_VBI_SETVBILINE:
            return _MTVDECEX_VBI_SetVBILine(arg);
            break;
        default:
			return MTR_NOT_SUPPORTED;
		       break;
	}
	return i4Ret;
}
