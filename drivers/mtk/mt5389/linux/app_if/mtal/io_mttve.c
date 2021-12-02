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
 * $RCSfile: io_mttve.c,v $
 * $Revision: #2 $
 *---------------------------------------------------------------------------*/

/** @file io_mttve.c
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttve.h"
#include <linux/mtal_ioctl.h>
#include "nptv_if.h"
#include "tve_if.h"
#include "x_linux.h"
#include <linux/wait.h>
#include "hw_vdoin.h"
#include "video_def.h"
#ifdef CC_MT5392B
#include "api_video.h"
#endif
#include "vdp_if.h"
#if defined(CC_MT5360) || defined(CC_MT5391)
#include "drv_tve.h"
#include "tve_ctrl.h"
#else /* for mt5387/63, the tve header file is merged into the one */
#include "tve_if.h"
#endif
#include "source_select.h"
#include "source_table.h"
#if defined(CC_MT5360) || defined(CC_MT5391)
#else /* for mt5387/63, the b2r is divided from vdp */
#include "b2r_if.h"
#endif

#if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
#include "vdp_display.h"
#endif
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DAC_1 0
#define DAC_2 1
#define TVENC_COMP_NS                     2

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(INT32)))                            \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(INT32)))                        \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \


#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_mttve valid argument error\n");     \
        return MTR_ERR_INV;                                          \
    }   

#define COPY_FROM_USER_ARG_K(ArgUsr, ArgKernel,size)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,    \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_mttve argument copy error\n");      \
        return MTR_ERR_INV;                                          \
    } 

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                           

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");      \
        return MTR_ERR_INV;                                          \
    }  
    
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                           

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");      \
        return MTR_ERR_INV;                                          \
    }  


#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                           

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");      \
        return MTR_ERR_INV;                                          \
    }  
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_mttve argument error\n");     \
        return MTR_ERR_INV;                                          \
    }    

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static TveUsage_t _rTveUsed = {0xf, 0x0};
#if defined(CC_MT5360) || defined(CC_MT5391) // old style
static BOOL _fgDac2WithPipPath = FALSE;
#endif
static UINT8 _au1ScartOutSource[TVENC_COMP_NS];
static UINT8 _MTTVE_VBIBuffer[35*45];//max TTX buffer
static MTTVE_FMT_T _eTveFmt; 

//-----------------------------------------------------------------------------
// Local functions
//-----------------------------------------------------------------------------
#if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
void _MTTVE_NotifySigChg(UINT8 bPath, UINT8 u1SigState);
#endif

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT8 _MTVDECEX_VdoSrcMap(MTVDECEX_SRC_T eSrc);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/**
 * @brief 
 It initialized the part pertaining to the Video auxiliary outputs. 
 * @param 
 */
static MT_RESULT_T _MTTVE_Init(void)
{
    MTTVE_PRINT("- void\n");

    Bypass_Init();
    
    return MTR_OK;
}


#if defined(CC_MT5360) || defined(CC_MT5391) // old style
static rtve_config_t _tTveConfig[2];
#endif
/**
 * @brief 
 It connects the TVE component with the Input Source.
 * @param eTveId
 * @param eSRC input source
 */
static MT_RESULT_T _MTTVE_Connect(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTVE_ID_T eTveId;
    MTVDECEX_SRC_T eSrc;
#ifdef CC_SUPPORT_TVE
#if defined(CC_MT5360) || defined(CC_MT5391) // old style
    rtve_config_t tTveConfig;
    UINT8 u1DecType;
#endif
#endif

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eTveId = (MTTVE_ID_T)rArg.ai4Arg[0];
    eSrc = (MTVDECEX_SRC_T)rArg.ai4Arg[1];

    if((eTveId >= MTTVE_ID_MAX) || (eSrc >= MTVDECEX_SRC_MAX))
    {
        return MTR_NOT_SUPPORTED;
    }
    
    MTTVE_PRINT("_MTTVE_Connect\n");
    MTTVE_PRINT("- eTveId = %d, eSrc = %d\n",
		(int)eTveId, (int)eSrc);

#ifdef CC_SUPPORT_TVE
    _au1ScartOutSource[eTveId] = _MTVDECEX_VdoSrcMap(eSrc);    
#if defined(CC_MT5360) || defined(CC_MT5391) // old style
    u1DecType = bGetDecodeType(_au1ScartOutSource[eTveId]);

    if(VSS_MAJOR(_au1ScartOutSource[eTveId]) == VSS_DTV) 
    {
        vApiTVEGetConfig(SV_TVE_TUNER_DTV, u1DecType, &tTveConfig);
    }
    else if(VSS_MAJOR(_au1ScartOutSource[eTveId]) == VSS_ATV ||
        VSS_MAJOR(_au1ScartOutSource[eTveId]) == VSS_ATD )
    {
        vApiTVEGetConfig(SV_TVE_TUNER_ATV, u1DecType, &tTveConfig);
    }
    else
    {
        vApiTVEGetConfig(SV_TVE_TUNER_LAST_TYPE, u1DecType, &tTveConfig);
    }

    _tTveConfig[eTveId] = tTveConfig;

    if(eTveId == MTTVE_DAC1)//TV SCART
    {
        vApiTVESetTvScartOutCtrl(_au1ScartOutSource[0], &tTveConfig, &_rTveUsed);
    }
    else//monitor SCART
    {
        //vApiTVEGetConfig(SV_TVE_TUNER_LAST_TYPE, u1DecType, &tTveConfig);
        //if(bGetVideoDecType(SV_VP_MAIN) == bGetVideoDecType(SV_VP_PIP) 
        //   && _rTveUsed.TveIsUsedBy == 0)
       if (_rTveUsed.TveIsUsedBy == 0)   //scart2 set dtv out
        {
            /* sub is already open by tvscart */
            _fgDac2WithPipPath = TRUE;
        }
        else
        {
            _fgDac2WithPipPath = FALSE;
        }
        vApiTVESetMonitorScartOutCtrl(_au1ScartOutSource[1], &tTveConfig, &_rTveUsed, &_fgDac2WithPipPath);
    }

    if(eTveId == _rTveUsed.TveIsUsedBy)
    {
        if(_rTveUsed.TveIsUsedBy != 0xf)
        {
            VDP_REGION_T rOutRegion;
            rOutRegion.u4X = 0;
            rOutRegion.u4Y = 0;
            rOutRegion.u4Width = 1;
            rOutRegion.u4Height = 1;
            VDP_SetOutRegion(VDP_2, 0, rOutRegion);
        }

        VDP_SetEnable(VDP_2, TRUE);
        VDP_SetInput(VDP_2, 0, 0);
        TVE_SetEnable(TVE_1, TRUE); 
    }
#else /* new style for 5387/63 and later */
    vApiTVESetScartOutCtrl((eTveId==MTTVE_DAC1)?(0):(1), &_au1ScartOutSource[((eTveId==MTTVE_DAC1)?(0):(1))], &_rTveUsed);
    
    if(eTveId == _rTveUsed.TveIsUsedBy)
    {
        VDP_SetEnable(VDP_2, TRUE);
        TVE_SetEnable(TVE_1, TRUE); 
    }
#endif

    #if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
    vApiTVESetNotifyFunc(_MTTVE_NotifySigChg, NULL);
    #endif
    
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}
 
 /**
 * @brief 
It releases the TVE connection settings.
 * @param eTveId
 * @param eSRC input source
 */
static MT_RESULT_T _MTTVE_Disconnect(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTVE_ID_T eTveId;
    MTVDECEX_SRC_T eSrc;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eTveId = (MTTVE_ID_T)rArg.ai4Arg[0];
    eSrc = (MTVDECEX_SRC_T)rArg.ai4Arg[1];

    if((eTveId >= MTTVE_ID_MAX) || (eSrc >= MTVDECEX_SRC_MAX))
    {
        return MTR_NOT_SUPPORTED;
    }

    MTTVE_PRINT("_MTTVE_Disconnect\n");
    MTTVE_PRINT("- eTveId = %d, eSrc = %d\n",
		(int)eTveId, (int)eSrc);

#ifdef CC_SUPPORT_TVE
    if(eTveId == _rTveUsed.TveIsUsedBy)
    {
        TVE_SetEnable(TVE_1, FALSE);
        VDP_SetEnable(VDP_2, FALSE);

        if(_au1ScartOutSource[eTveId] == SV_VS_DTV1)    
        {
            VDP_SetInput(VDP_2, 0xff, 0);
        }

        bApiVideoSetVideoSrc(SV_VP_PIP, SV_VS_MAX);
        vApiTVESetOutputFormat(SV_TVE_OFMT_NONE);
        _rTveUsed.TveIsUsedBy = 0xf;
    }
    vApiTVESetDacMuteMask(((eTveId==MTTVE_DAC1)?(0):(1)), TVE_DAC_MUTE_MASK_DRV, TRUE);

    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}
    
/**
 * @brief 
 It starts the Video auxiliary outputs (CVBS outputs) 
 * @param eTveId
 */
static MT_RESULT_T _MTTVE_VideoStart(unsigned long arg)
{
    MTTVE_ID_T eTveId;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,eTveId);
    //eTveId = (MTTVE_ID_T)arg;
    
    if(eTveId >= MTTVE_ID_MAX)
    {
        return MTR_NOT_SUPPORTED;
    }
        
    MTTVE_PRINT(" - eTveId = %d\n",
		(int)eTveId);

#ifdef CC_SUPPORT_TVE
    //TODO
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}

/**
 * @brief 
 It stops the Video auxiliary outputs (CVBS output).
 * @param eTveId
 */
static MT_RESULT_T _MTTVE_VideoStop(unsigned long arg)
{
    MTTVE_ID_T eTveId;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,eTveId);
    //eTveId = (MTTVE_ID_T)arg;
    
    if(eTveId >= MTTVE_ID_MAX)
    {
        return MTR_NOT_SUPPORTED;
    }
        
    MTTVE_PRINT(" - eTveId = %d\n",
		(int)eTveId);
	
#ifdef CC_SUPPORT_TVE
    //TODO
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}

/** * @brief Video 
It makes the Video auxiliary outputs mute On/Off. 
* @param eTveId 
* @param bMuteOn TRUE : Mute On, FALSE : Mute Off 
*/
static MT_RESULT_T _MTTVE_VideoMute(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTVE_ID_T eTveId;
    BOOL fgOnOff; 

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eTveId = (MTTVE_ID_T)rArg.ai4Arg[0];
    fgOnOff = (UINT8)rArg.ai4Arg[1];
    
    if(eTveId >= MTTVE_ID_MAX)
    {
        return MTR_NOT_SUPPORTED;
    }
        
    MTTVE_PRINT("- eTveId = %d, fgOnOff = %d\n",
		(int)eTveId, (int)fgOnOff);
		
#ifdef CC_SUPPORT_TVE
    if (eTveId == MTTVE_DAC1)//TV SCART
    {
        vApiTVESetDacMuteMask(DAC_1, TVE_DAC_MUTE_MASK_MW, fgOnOff);
    }
    else //Monitor Scart
    {
        vApiTVESetDacMuteMask(DAC_2, TVE_DAC_MUTE_MASK_MW, fgOnOff);
    }
    
     return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
   
}
/**
 * @brief Video format.
 It sets the Video auxiliary outputs format(480i or 576i) 
 * @param eTveId
 */
static MT_RESULT_T _MTTVE_VideoFmt(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTVE_ID_T eTveId;
    MTTVE_FMT_T eTveFmt;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eTveId = (MTTVE_ID_T)rArg.ai4Arg[0];
    eTveFmt = (MTTVE_FMT_T)rArg.ai4Arg[1];

    if((eTveId >= MTTVE_ID_MAX) || (eTveFmt >= MTTVE_FMT_MAX))
    {
        return MTR_NOT_SUPPORTED;
    }
        
    if(eTveId != _rTveUsed.TveIsUsedBy)
    {
        return MTR_NOT_OK;
    }
    MTTVE_PRINT("[MTAL_TVE]_MTTVE_VideoFmt = %d\n", eTveFmt);
    _eTveFmt = eTveFmt;
    MTTVE_PRINT("- eTveId = %d, eTveFmt = %d\n",
		(int)eTveId, (int)eTveFmt);

#ifdef CC_SUPPORT_TVE
    if(eTveFmt == MTTVE_480i)
    {
        TVE_SetFmt(TVE_1, TVE_FMT_480I);
    }
    else 
    {
        TVE_SetFmt(TVE_1, TVE_FMT_576I);
    }
    
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}
/**
 * @brief Video output mode.
 It sets the Video auxiliary outputs format(ntsc or pal) 
 * @param eTveId
 */
static MT_RESULT_T _MTTVE_VideoMode(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTVE_ID_T eTveId;
    MTTVE_MODE_T eTveMode;
    UCHAR mode_index;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eTveId = (MTTVE_ID_T)rArg.ai4Arg[0];
    eTveMode= (MTTVE_MODE_T)rArg.ai4Arg[1];
     if((eTveId >= MTTVE_ID_MAX) || (eTveMode>= MTTVE_MODE_MAX))
    {
        return MTR_NOT_SUPPORTED;
    }
        
  /*  if(eTveId != _rTveUsed.TveIsUsedBy)
    {
        return MTR_NOT_OK;
    }*/
MTTVE_PRINT("- eTveId = %d, eTveMode = %d\n",
		(int)eTveId, (int)eTveMode);
#ifdef CC_SUPPORT_TVE  
 switch(eTveMode)
 	{
        case MTTVE_NTSC_358 :
		  mode_index=0;
		  break;
	 case MTTVE_PAL_M:
	 	  mode_index=1;
		  break;
	 case MTTVE_PAL_NC:
	 	  mode_index=2;
		  break;
	 case MTTVE_PAL_BDGHI:
	 	  mode_index=3;
		  break;
	 case MTTVE_PAL_60:
	 	  mode_index=4;
		  break;
	 case MTTVE_NTSC_443:
	 	  mode_index=5;
		  break;
	 case MTTVE_MODE_MAX:
	 	break;
	 default :
	 	break;

     }

    TVE_SetTVMode(TVE_1,mode_index);
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif


}
/**
 * @brief 
 It sends the VBI Data to the Video auxiliary outputs from the input parameter buffer which contains the vbi Data. 
 It inserts the VBI Data in the monitoring output.
 * @param eTveId
 * @param eType Closed Caption or Extended Data Service or etc.
 * @param pData Data buffer pointer
 * @param length Data Length
 * @code
		typedef enum
		{
			MTTVE_VBI_DATA_CC = 0, ///< Closed Caption
			MTTVE_VBI_DATA_XDS, ///< Extended Data Service
			MTTVE_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
			MTTVE_VBI_DATA_WSS, ///< Analog SdUserData is WSS	
			MTTVE_VBI_DATA_MAX
		} MTTVE_VBIDATATYPE_T; ///< VBI Data Type
 * @endcode
 */
static MT_RESULT_T _MTTVE_SendVbiData(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    MTTVE_ID_T eTveId;
    MTTVE_VBIDATATYPE_T eType;
    UINT8 *pData;
    UINT32 length;
    UINT32 i4RetValue;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg,rArg);

    eTveId = (MTTVE_ID_T)rArg.ai4Arg[0];
    eType = (MTTVE_VBIDATATYPE_T)rArg.ai4Arg[1];
    pData = (UINT8*)rArg.ai4Arg[2];
    length = (UINT32)rArg.ai4Arg[3];

    if((eTveId >= MTTVE_ID_MAX) || (eType >= MTTVE_VBI_DATA_MAX))
    {
        return MTR_NOT_SUPPORTED;
    }
    
    if(eTveId != _rTveUsed.TveIsUsedBy)
    {
        return MTR_NOT_OK;
    }
    
    MTTVE_PRINT("- eTveId = %d, eType = %d, length = %d\n",
		(int)eTveId, (int)eType, (int)length);

#ifdef CC_SUPPORT_TVE
     if(eType == MTTVE_VBI_DATA_TTX)
     {
        if(length > 35) return MTR_NOT_SUPPORTED;
        
         USR_SPACE_ACCESS_VALIDATE_ARG_K(pData, length*45*sizeof(UINT8));
         COPY_FROM_USER_ARG_K(pData, _MTTVE_VBIBuffer, length*45*sizeof(UINT8));
         if(TVE_SendTTX(TVE_1, _MTTVE_VBIBuffer, length * 45) != TVE_SET_OK)
         {
            i4RetValue = MTR_NOT_OK;
         }
     }
     else if(eType == MTTVE_VBI_DATA_WSS)
     {
		if(length >2) return MTR_NOT_SUPPORTED;

         USR_SPACE_ACCESS_VALIDATE_ARG_K(pData, length*sizeof(UINT8));
         COPY_FROM_USER_ARG_K(pData, _MTTVE_VBIBuffer, length*sizeof(UINT8));
         if(TVE_SendWss(TVE_1, _MTTVE_VBIBuffer, length) != TVE_SET_OK)
         {
             i4RetValue= MTR_NOT_OK;
         }
     }
     else if(eType == MTTVE_VBI_DATA_CC)
     {
         if(length >2) return MTR_NOT_SUPPORTED;
         
         USR_SPACE_ACCESS_VALIDATE_ARG_K(pData, length*sizeof(UINT8));
         COPY_FROM_USER_ARG_K(pData, _MTTVE_VBIBuffer, length*sizeof(UINT8));
         if(TVE_SendCc(TVE_1, TVE_FIELD_ODD, _MTTVE_VBIBuffer, length) != TVE_SET_OK)
         {
             i4RetValue= MTR_NOT_OK;
         }
     }
     else if(eType == MTTVE_VBI_DATA_XDS)
     {
         if(length >2) return MTR_NOT_SUPPORTED;

         USR_SPACE_ACCESS_VALIDATE_ARG_K(pData, length*sizeof(UINT8));
         COPY_FROM_USER_ARG_K(pData, _MTTVE_VBIBuffer, length*sizeof(UINT8));
         if(TVE_SendCc(TVE_1, TVE_FIELD_EVEN, _MTTVE_VBIBuffer, length) != TVE_SET_OK)
         {
             i4RetValue= MTR_NOT_OK;
         }
     }

    return MTR_OK;    
#else
    return MTR_NOT_SUPPORTED;
#endif
}
/**
 * @brief VBI Data
 It starts the VBI function of the Video auxiliary outputs. 
 * @param eTveId
 * @param eType Closed Caption or Extended Data Service or etc.
 * @code
 		typedef enum
		{
			MTTVE_VBI_DATA_CC = 0, ///< Closed Caption
			MTTVE_VBI_DATA_XDS, ///< Extended Data Service
			MTTVE_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
			MTTVE_VBI_DATA_WSS, ///< Analog SdUserData is WSS	
			MTTVE_VBI_DATA_MAX
		} MTTVE_VBIDATATYPE_T; ///< VBI Data Type
 * @endcode
 */
static MT_RESULT_T _MTTVE_VbiStart(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTVE_ID_T eTveId;
    MTTVE_VBIDATATYPE_T eType;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eTveId = (MTTVE_ID_T)rArg.ai4Arg[0];
    eType = (MTTVE_VBIDATATYPE_T)rArg.ai4Arg[1];
    
    if((eTveId >= MTTVE_ID_MAX) || (eType >= MTTVE_VBI_DATA_MAX))
    {
        return MTR_NOT_SUPPORTED;
    }
    
    if(eTveId != _rTveUsed.TveIsUsedBy)
    {
        return MTR_NOT_OK;
    }
    
    MTTVE_PRINT("- eTveId = %d, eType = %d\n",
		(int)eTveId, (int)eType);

#ifdef CC_SUPPORT_TVE
    if(eType == MTTVE_VBI_DATA_TTX)
    {
        if(TVE_SetTTXEnable(TVE_1, 1) != TVE_SET_OK)
        {
            return MTR_NOT_OK;
        }
    }
    else if(eType == MTTVE_VBI_DATA_WSS)
    {
        if(TVE_SetWssEnable(TVE_1, 1) != TVE_SET_OK)
        {
            return MTR_NOT_OK;
        }
    }
    else
    {
        if(TVE_SetCcEnable(TVE_1, 1) != TVE_SET_OK)
        {
            return MTR_NOT_OK;
        }
    }
    
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}

/**
 * @brief VBI Data
 It stops the VBI function of the Video auxiliary outputs. 
 * @param eTveId
 * @param eType Closed Caption or Extended Data Service or etc.
 * @code
 		typedef enum
		{
			MTTVE_VBI_DATA_CC = 0, ///< Closed Caption
			MTTVE_VBI_DATA_XDS, ///< Extended Data Service
			MTTVE_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
			MTTVE_VBI_DATA_WSS, ///< Analog SdUserData is WSS	
			MTTVE_VBI_DATA_MAX
		} MTTVE_VBIDATATYPE_T; ///< VBI Data Type
 * @endcode
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
static MT_RESULT_T _MTTVE_VbiStop(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTTVE_ID_T eTveId;
    MTTVE_VBIDATATYPE_T eType;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eTveId = (MTTVE_ID_T)rArg.ai4Arg[0];
    eType = (MTTVE_VBIDATATYPE_T)rArg.ai4Arg[1];

    if((eTveId >= MTTVE_ID_MAX) || (eType >= MTTVE_VBI_DATA_MAX))
    {
        return MTR_NOT_SUPPORTED;
    }
    
    if(eTveId != _rTveUsed.TveIsUsedBy)
    {
        return MTR_NOT_OK;
    }
    
    MTTVE_PRINT("- eTveId = %d, eType = %d\n",
		(int)eTveId, (int)eType);

#ifdef CC_SUPPORT_TVE
    if(eType == MTTVE_VBI_DATA_TTX)
    {
        if(TVE_SetTTXEnable(TVE_1, 0) != TVE_SET_OK)
        {
            return MTR_NOT_OK;
        }
    }
    else if(eType == MTTVE_VBI_DATA_WSS)
    {
        if(TVE_SetWssEnable(TVE_1, 0) != TVE_SET_OK)
        {
            return MTR_NOT_OK;
        }
    }
    else
    {
        if(TVE_SetCcEnable(TVE_1, 0) != TVE_SET_OK)
        {
            return MTR_NOT_OK;
        }
    }
    
    return MTR_OK;
#else
    return MTR_NOT_SUPPORTED;
#endif
}

//#if defined(CC_MT5391) && defined(CC_SUPPORT_TVE)
#if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
static UINT16 usTveCurWidth = 720;
static UINT16 usTveCurHeight= 576;
static UINT16 _usTveCurXOff = 0;
static UINT16 _usTveCurYOff = 0;

#if defined(CC_MT5391)
static VDP_REGION_T _rTveSrcRgn = {0, 0, 1000, 1000};
#else
static VDP_REGION_T _rTveSrcRgn = {0, 0, 10000, 10000};
#endif

static MT_RESULT_T _mttve_setdtvhscalingforcvbs(
    UINT16 usOrigWidth,
    UINT16 usFrameWidth,
    UINT16 usCutWidth,
    UINT16 usDestWidth
)
{
    MTTVE_PRINT("\n_mttve_setdtvhscalingforcvbs Input(%d, %d)\n", usOrigWidth, usFrameWidth, usCutWidth, usDestWidth);

    if(usFrameWidth < usDestWidth)
    {
        /* reset case */
        usDestWidth = usFrameWidth;
    }

    usTveCurWidth = usDestWidth;

    MTTVE_PRINT("\n_mttve_setdtvhscalingforcvbs _vApiTveSetSclPara(%d, %d)\n", usTveCurWidth, usTveCurHeight);
    #if defined(CC_MT5391)
    _SCPIP_SetTVEEnable(VDP_2, TRUE, usTveCurWidth, usTveCurHeight);
    #else
    _vApiTveSetSclPara(TRUE, usTveCurWidth, usTveCurHeight);
    #endif

    _usTveCurXOff = (usFrameWidth - usTveCurWidth) / 2;

    if(usOrigWidth < usCutWidth)
    {
        /* reset case */
        usCutWidth = usOrigWidth;
    }

    #if defined(CC_MT5391)
    _rTveSrcRgn.u4X = (1000 * (usOrigWidth - usCutWidth)/2) / usOrigWidth; 
    _rTveSrcRgn.u4Width = 1000 * usCutWidth / usOrigWidth; 
    #else
    _rTveSrcRgn.u4X = (10000 * (usOrigWidth - usCutWidth)/2) / usOrigWidth; 
    _rTveSrcRgn.u4Width = 10000 * usCutWidth / usOrigWidth; 
    #endif

    MTTVE_PRINT("\n_mttve_setdtvhscalingforcvbs VDP_SetSrcRegion(%d, %d, %d, %d)\n",_rTveSrcRgn.u4X, _rTveSrcRgn.u4Width,  _rTveSrcRgn.u4Y, _rTveSrcRgn.u4Height);
    VDP_SetSrcRegion(VDP_2, 0, _rTveSrcRgn); 

    MTTVE_PRINT("\n_mttve_setdtvhscalingforcvbs TVE_SetHDispArea(%d, %d, %d, %d)\n", _eTveFmt, bGetVideoDecType(SV_VP_PIP),  _usTveCurXOff, _usTveCurXOff+usTveCurWidth);
    TVE_SetHDispArea(_eTveFmt, bGetVideoDecType(SV_VP_PIP),  _usTveCurXOff, _usTveCurXOff+usTveCurWidth);
    return MTR_OK;
}

static MT_RESULT_T _MTTVE_SetDtvHScalingForCvbs(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    UINT16 usOrigWidth;
    UINT16 usFrameWidth;
    UINT16 usEffectiveWidth;
    UINT16 usDestWidth;
    UINT16 usCutWidth;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg,rArg);

    usOrigWidth = (UINT16)rArg.ai4Arg[0];
    usEffectiveWidth = (UINT16)rArg.ai4Arg[1];
    usFrameWidth = (UINT16)rArg.ai4Arg[2];

    MTTVE_PRINT("\nMTTVE_SetDtvHScalingForCvbs(%d, %d, %d)\n", usOrigWidth, usEffectiveWidth , usFrameWidth);

    usCutWidth = usOrigWidth;

    if(usEffectiveWidth == MTTVE_HSCALING_RESET_EFFECTIVE_SIZE)
    {
        /* reset case */
        usDestWidth = usFrameWidth;
    }
    else
    {
        usDestWidth = usOrigWidth * usFrameWidth / usEffectiveWidth;
    }
    
    return _mttve_setdtvhscalingforcvbs(usOrigWidth, usFrameWidth, usCutWidth, usDestWidth);
}

static MT_RESULT_T _MTTVE_SetDtvHCenterCutForCvbs(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    UINT16 usOrigWidth;
    UINT16 usFrameWidth;
    UINT16 usCutWidth;
    UINT16 usDestWidth;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg,rArg);

    usOrigWidth = (UINT16)rArg.ai4Arg[0];
    usFrameWidth = (UINT16)rArg.ai4Arg[1];
    usCutWidth = (UINT16)rArg.ai4Arg[2];
    usDestWidth = (UINT16)rArg.ai4Arg[3];

    MTTVE_PRINT("\nMTTVE_SetDtvHCenterCutForCvbs(%d, %d, %d, %d)\n", usOrigWidth, usFrameWidth, usCutWidth, usDestWidth);

    return _mttve_setdtvhscalingforcvbs(usOrigWidth, usFrameWidth, usCutWidth, usDestWidth);
}

static MT_RESULT_T _MTTVE_SetDtvLetterBoxForCvbs(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    UINT16 usOrigHeight;
    UINT16 usFrameHeight;
    UINT16 usCutHeight;
    UINT16 usDestHeight;
    UINT16 usSourceHeight;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg,rArg);

    usOrigHeight = (UINT16)rArg.ai4Arg[0];
    usFrameHeight = (UINT16)rArg.ai4Arg[1];
    usCutHeight = (UINT16)rArg.ai4Arg[2];
    usDestHeight = (UINT16)rArg.ai4Arg[3];

    MTTVE_PRINT("\nMTTVE_SetDtvLetterBoxForCvbs Input(%d, %d, %d, %d)\n", usOrigHeight, usFrameHeight, usCutHeight, usDestHeight);

    if(usFrameHeight < usDestHeight)
    {
        /* reset case */
        usDestHeight = usFrameHeight;
    }

    usTveCurHeight = usDestHeight;

    MTTVE_PRINT("\nMTTVE_SetDtvLetterBoxForCvbs _vApiTveSetSclPara(%d, %d)\n", usTveCurWidth, usTveCurHeight);
    #if defined(CC_MT5391)
    _SCPIP_SetTVEEnable(VDP_2, TRUE, usTveCurWidth, usTveCurHeight);
    #else
    _vApiTveSetSclPara(TRUE, usTveCurWidth, usTveCurHeight);
    #endif

    _usTveCurYOff = (usFrameHeight - usTveCurHeight) / 2;
    
    usSourceHeight = wApiVideoGetInputHeight(SV_VP_PIP);
    if(usSourceHeight <= 0)
    {
        usSourceHeight = usTveCurHeight;
    }

    #if defined(CC_MT5391)
    bApiVideoSetDispPosSize(SV_VP_PIP, 
                           0, 
                           (_usTveCurYOff>6)?_usTveCurYOff-6:0,
                           1000, 
                           1000-_usTveCurYOff*2);
    #else
    bApiVideoSetDispPosSize(SV_VP_PIP, 
                           0, 
                           (_usTveCurYOff>6)?_usTveCurYOff-6:0,
                           10000, 
                           10000-_usTveCurYOff*2);
    #endif

    if(usOrigHeight < usCutHeight)
    {
        /* reset case */
        usCutHeight = usOrigHeight;
    }

    #if defined(CC_MT5391)
    _rTveSrcRgn.u4Y = (1000 * (usOrigHeight - usCutHeight)/2) / usOrigHeight; 
    _rTveSrcRgn.u4Height = 1000 * usCutHeight / usOrigHeight; 
    #else
    _rTveSrcRgn.u4Y = (10000 * (usOrigHeight - usCutHeight)/2) / usOrigHeight; 
    _rTveSrcRgn.u4Height = 10000 * usCutHeight / usOrigHeight; 
    #endif

    MTTVE_PRINT("\nMTTVE_SetDtvLetterBoxForCvbs VDP_SetSrcRegion(%d, %d, %d, %d)\n",_rTveSrcRgn.u4X, _rTveSrcRgn.u4Width,  _rTveSrcRgn.u4Y, _rTveSrcRgn.u4Height);

    VDP_SetSrcRegion(VDP_2, 0, _rTveSrcRgn); 

    MTTVE_PRINT("\nMTTVE_SetDtvLetterBoxForCvbs TVE_SetVDispArea(%d, %d, %d)\n", _eTveFmt , _usTveCurYOff, _usTveCurYOff+usTveCurHeight);
    TVE_SetVDispArea(_eTveFmt , _usTveCurYOff, _usTveCurYOff+usTveCurHeight);

    return MTR_OK;
}

static MT_RESULT_T _MTTVE_GetDtvScalingStatusForCvbs(unsigned long arg)
{
    scaling_info_t tScalingInfo;

    tScalingInfo.usCurPicWidth = usTveCurWidth;
    tScalingInfo.usCurPicHeight = usTveCurHeight;
    tScalingInfo.fgHavingSideBar = (_usTveCurXOff > 0)?(TRUE):(FALSE); 
    tScalingInfo.fgHavingLetterBox = (_usTveCurYOff > 0)?(TRUE):(FALSE);
    tScalingInfo.fgDuringScaling = FALSE; /* todo */

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_TO_USER_ARG(arg, tScalingInfo, scaling_info_t);

    return MTR_OK;
}


#ifdef CC_SUPPORT_TVE
#if defined(CC_MT5360) || defined(CC_MT5391) // old style
void _TveNotifySigChg(UINT8 bPath, UINT8 u1SigState)
{
    if(u1SigState == SV_VDO_NOSIGNAL)
    {
        if( bGetVideoDecType(bPath) == bGetDecodeType(_au1ScartOutSource[MTTVE_DAC1]) &&
	   (_tTveConfig[MTTVE_DAC1].tve_out1.type != SV_TVE_DAC_BYPASS_DEMOD &&
	    _tTveConfig[MTTVE_DAC1].tve_out1.type != SV_TVE_HW_BYPASS))
        {
            vApiTVESetDacMuteMask(MTTVE_DAC1, TVE_DAC_MUTE_MASK_DRV, TRUE);
        }
        if( bGetVideoDecType(bPath) == bGetDecodeType(_au1ScartOutSource[MTTVE_DAC2]) &&
	   (_tTveConfig[MTTVE_DAC2].tve_out2.type != SV_TVE_DAC_BYPASS_DEMOD &&
	    _tTveConfig[MTTVE_DAC2].tve_out2.type != SV_TVE_HW_BYPASS))
        {
            vApiTVESetDacMuteMask(MTTVE_DAC2, TVE_DAC_MUTE_MASK_DRV, TRUE);
        }
    }

    if(bPath == VDP_2) /* tve path */
    {
        if(u1SigState == SV_VDO_STABLE)
        {
            if(_rTveUsed.TveIsUsedBy != 0xf)
            {
                if(//wApiVideoGetInputHeight(bPath) < 576 ||
                   bApiVideoGetRefreshRate(bPath) == 60 ||
                   bApiVideoGetRefreshRate(bPath) == 30 ||
                   bApiVideoGetRefreshRate(bPath) == 24)
                {
                    _eTveFmt = TVE_FMT_480I;
		    vApiTVESetOutputFormat(SV_TVE_OFMT_NTSC);
                    if(DRVCUST_OptGet(eTVESupportAFD) == 1)
                    {
                        TVE_SetWssEnable(TVE_1, SV_OFF);
                    }
                }
                else
                {
                    _eTveFmt = TVE_FMT_576I;
		    vApiTVESetOutputFormat(SV_TVE_OFMT_PAL);
		    
                    if((VSS_MAJOR(_au1ScartOutSource[_rTveUsed.TveIsUsedBy]) == VSS_DTV) &&
                      (DRVCUST_OptGet(eTVESupportAFD) == 1))
                    {
                        TVE_SetWssEnable(TVE_1, SV_ON);
                    }
                }
            }
        }
    }
}

void _TveNotifyUnmute(UINT8 bPath)
{ 
    UINT8 bVideoDecType;

    bVideoDecType = bGetVideoDecType(bPath);

    /* if the pipe is not connected, ignore the unmute signal */
    if(bVideoDecType != SV_VD_NA && bVideoDecType != SV_VD_MAX)
    {
        if( bVideoDecType == bGetDecodeType(_au1ScartOutSource[MTTVE_DAC1]) )
        {
            vApiTVESetDacMuteMask(MTTVE_DAC1, TVE_DAC_MUTE_MASK_DRV, FALSE);
        }

        if( bVideoDecType == bGetDecodeType(_au1ScartOutSource[MTTVE_DAC2]) )
        {
            vApiTVESetDacMuteMask(MTTVE_DAC2, TVE_DAC_MUTE_MASK_DRV, FALSE);
        }
    }
}
#else
void _MTTVE_NotifySigChg(UINT8 bPath, UINT8 u1SigState)
{    
    UINT8 ucEnable;
    if(bPath == VDP_2)
    {
        if(u1SigState == SV_VDO_STABLE)
        {
            TVE_GetEnable(TVE_1, &ucEnable);
            if(ucEnable != FALSE)
            {
                if(//wApiVideoGetInputHeight(bPath) < 576 ||
                   bApiVideoGetRefreshRate(bPath) == 120 ||
                   bApiVideoGetRefreshRate(bPath) == 60 ||
                   bApiVideoGetRefreshRate(bPath) == 30 ||
                   bApiVideoGetRefreshRate(bPath) == 48 ||
                   bApiVideoGetRefreshRate(bPath) == 24)
                {
                    MTTVE_PRINT("[MTAL_TVE]SigChg TVE_FMT_480I\n");
                    _eTveFmt = TVE_FMT_480I;
                }
                else
                {
                    MTTVE_PRINT("[MTAL_TVE]SigChg TVE_FMT_576I\n");
                    _eTveFmt = TVE_FMT_576I;
                }
            }
        }
    }
}
#if 0
void _TveNotifyUnmute(UINT8 bPath)
{ 
    UINT8 bVideoDecType;

    bVideoDecType = bGetVideoDecType(bPath);

    /* if the pipe is not connected, ignore the unmute signal */
    if(bVideoDecType != SV_VD_NA && bVideoDecType != SV_VD_MAX)
    {
        if( bVideoDecType == bGetDecodeType(_au1ScartOutSource[MTTVE_DAC1]) )
        {
            vApiTVESetDacMuteMask(MTTVE_DAC1, TVE_DAC_MUTE_MASK_DRV, FALSE);
        }

        if( bVideoDecType == bGetDecodeType(_au1ScartOutSource[MTTVE_DAC2]) )
        {
            vApiTVESetDacMuteMask(MTTVE_DAC2, TVE_DAC_MUTE_MASK_DRV, FALSE);
        }
    }
}
#endif
#endif
#endif
#endif

int mtal_ioctl_mttve(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
       int i4Ret = MTR_OK;
	   
	switch (cmd) 
	{				
		case MTAL_IO_TVE_INIT:
			return _MTTVE_Init();
		case MTAL_IO_TVE_CONNECT:
			return _MTTVE_Connect(arg);
		case MTAL_IO_TVE_DISCONNECT:
			return _MTTVE_Disconnect(arg);
		case MTAL_IO_TVE_VIDEOSTART:
			return _MTTVE_VideoStart(arg);
		case MTAL_IO_TVE_VIDEOSTOP:
			return _MTTVE_VideoStop(arg);
		case MTAL_IO_TVE_VIDEOMUTE:
			return _MTTVE_VideoMute(arg);
		case MTAL_IO_TVE_VIDEOFMT:
			return _MTTVE_VideoFmt(arg);
		case MTAL_IO_TVE_SENDVBIDATA:
			return _MTTVE_SendVbiData(arg);
		case MTAL_IO_TVE_VBISTART:
			return _MTTVE_VbiStart(arg);
		case MTAL_IO_TVE_VBISTOP:
			return _MTTVE_VbiStop(arg);	
		#if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
		case MTAL_IO_TVE_SETHSCALING:
			return _MTTVE_SetDtvHScalingForCvbs(arg);
		case MTAL_IO_TVE_SETHCENTERCUT:
			return _MTTVE_SetDtvHCenterCutForCvbs(arg);
		case MTAL_IO_TVE_SETLETTERBOX:
			return _MTTVE_SetDtvLetterBoxForCvbs(arg);
		case MTAL_IO_TVE_GETSCALINGSTATUS:
			return _MTTVE_GetDtvScalingStatusForCvbs(arg);
		#endif
		case MTAL_IO_TVE_VIDEOMODE:
			return _MTTVE_VideoMode(arg);
              default:
			return MTR_NOT_SUPPORTED;
	}
	return i4Ret;
}

