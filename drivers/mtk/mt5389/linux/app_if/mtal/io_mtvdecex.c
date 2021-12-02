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
 * $RCSfile: io_mtvdecex.c,v $
 * $Revision: #4 $
 *---------------------------------------------------------------------------*/
/** @file MTVDECEX.c
 *  APIs of external VDEC modules.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mtvdecex.h"
#include "cb_low.h"
#include <linux/mtal_ioctl.h>

#include "nptv_if.h"
#include "sv_const.h"
#include "source_table.h"
#include "video_def.h"
#include "video_timing.h"
#include "drv_adcset.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "drv_tvd.h"
#include "vdo_misc.h"
#include "b2r_if.h"
#include "drv_video.h"
#include "drv_auto.h"
#include "vdec_if.h"

extern UINT8 bHDMIGetAudioBitWidth(void);
extern void vHDMIAudChannelStatus(BOOL* pfgCopyright, UINT8* pu1CategoryCode, 
                                UINT8* p1WordLength, BOOL* pfgIsPcm);
 extern UINT8 bHDMI_IsPCM(void);    
 extern UINT8 bDrvVideoSignalStatus(UINT8 bPath);
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
#if defined (CC_MT5395) || defined (CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389)
#define CC_MT5365
#endif
#define MTVDECEX_PRINT_AMBIG_TIMINGT(rAmbigVgaTimingInfo)                       \
    do{\
        if(IS_MTVDECEX_LOG_ENABLE)\
        {                                        \
            printf("rAmbigVgaTimingInfo.u1RefreshRate = %d\n",(int)rAmbigVgaTimingInfo.u1RefreshRate);\
            printf("rAmbigVgaTimingInfo.u2Width = %d\n",(int)rAmbigVgaTimingInfo.u2Width);\
            printf("rAmbigVgaTimingInfo.u2Height= %d\n",(int)rAmbigVgaTimingInfo.u2Height);\
         }\
    }while(0)

#define MTVDECEX_PRINT_INFO_FRAME(rInfoFrame)                       \
    do{\
        if(IS_MTVDECEX_LOG_ENABLE)\
        {                                        \
            printf("rFrame.HDMI_FrameSize = %d\n",(int)rInfoFrame.HDMI_FrameSize);\
            for(i=0; i <10; i++)\
           {                             \
            printf("rFrame.HDMI_FrameData[%d] = %x\n",i,(int)rInfoFrame.HDMI_FrameData[i]);\
         }\
         }\
    }while(0)

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdecex argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdecex argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdecex argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdecex argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdecex argument error\n");     \
        return MTR_ERR_INV;                                          \
    }
#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdecex argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
    
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdecex argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

UINT32 _u4OversampleDependent = 1;
UINT32 _fgIsSupportVGAToYpbpr = 0;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


UINT8 _MTVDECEX_VdoSrcMap(MTVDECEX_SRC_T eSrc)
{
	switch(eSrc)
	{
		case	MTVDECEX_SRC_ATV1:
#ifdef USE_ATD_IF_DEMOD
                return SV_VS_ATD1; // not using ATD currently
#else
               return SV_VS_ATV1;
#endif
		case 	MTVDECEX_SRC_DTV1:
				return SV_VS_DTV1;
		case	MTVDECEX_SRC_CVBS1:
				return SV_VS_CVBS1;
		case	MTVDECEX_SRC_CVBS2:
				return SV_VS_CVBS2;
		case	MTVDECEX_SRC_CVBS3:
				return SV_VS_CVBS3;
		case	MTVDECEX_SRC_CVBS4:
				return SV_VS_CVBS4;				
		case	MTVDECEX_SRC_SV1:
				return SV_VS_S1;
		case	MTVDECEX_SRC_SV2:
				return SV_VS_S2;
		case	MTVDECEX_SRC_SV3:
				return SV_VS_S3;
		case	MTVDECEX_SRC_COMP1:
                if(_fgIsSupportVGAToYpbpr == 1)
                {
                    return SV_VS_VGACOMP1;//SV_VS_YPbPr1;
                }
                else
                {   
                    return SV_VS_YPbPr1;
                }            
		case	MTVDECEX_SRC_COMP2:
				return SV_VS_YPbPr2;
		case	MTVDECEX_SRC_COMP3:
				return SV_VS_YPbPr3;
		case	MTVDECEX_SRC_COMP4:
				return SV_VS_YPbPr4;
		case 	MTVDECEX_SRC_PC1:
				return SV_VS_VGA1;
		case	MTVDECEX_SRC_HDMI1:
				return SV_VS_HDMI1;
		case	MTVDECEX_SRC_HDMI2:
				return SV_VS_HDMI2;
		case	MTVDECEX_SRC_HDMI3:
				return SV_VS_HDMI3;
		case	MTVDECEX_SRC_HDMI4:
				return SV_VS_HDMI4;
		case	MTVDECEX_SRC_HDMI5:
				return SV_VS_HDMI5;				
		case	MTVDECEX_SRC_SCART1:
				return SV_VS_SCART1;
		case	MTVDECEX_SRC_SCART2:
				return SV_VS_SCART2;
		case	MTVDECEX_SRC_SCART3:
				return SV_VS_SCART3;
		case	MTVDECEX_SRC_SCART4:
				return SV_VS_SCART4;
		default:
			return SV_VS_MAX;		
	};
};

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
static MT_RESULT_T _MTVDECEX_IsPCTiming(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    BOOL *pfgPcTiming;
    BOOL fgPcTiming;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pfgPcTiming = (BOOL *)rArg.ai4Arg[1];
    
    fgPcTiming = (BOOL) fgApiVideoIsVgaTiming(ePath);

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgPcTiming,BOOL);
    COPY_TO_USER_ARG(pfgPcTiming,fgPcTiming,BOOL);

    MTVDECEX_PRINT(" - ePath = %d, fgPcTiming = %d\n",
		(int)ePath,
		(int)fgPcTiming);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_IsOversampleTiming(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    BOOL *pfgOversampleTiming;
    BOOL fgOversampleTiming;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    pfgOversampleTiming = (BOOL *)rArg.ai4Arg[1];
    
    fgOversampleTiming = (BOOL) bDrvVideoGetWidthOverSample(ePath);

    USR_SPACE_ACCESS_VALIDATE_ARG(pfgOversampleTiming,BOOL);
    COPY_TO_USER_ARG(pfgOversampleTiming,fgOversampleTiming,BOOL);

    MTVDECEX_PRINT(" - ePath = %d, fgOversampleTiming = %d\n",
		(int)ePath,
		(int)fgOversampleTiming);
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_SetOversampleDependent(unsigned long arg)
{
    UINT32 u4Value;

    u4Value = (UINT32)arg;
    MTVDECEX_PRINT(" - u4Value = %d\n",
		(int)u4Value);

    _u4OversampleDependent = u4Value;

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_GetOversampleDependent(unsigned long arg)
{
    UINT32 *pu4Value;

    pu4Value = (UINT32 *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Value, UINT32);
    COPY_TO_USER_ARG(pu4Value, _u4OversampleDependent, UINT32);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_SetVideoSrc(unsigned long arg)
{
    UINT8 u1Ret;
    MTVDO_PATH_T ePath;
    MTVDECEX_SRC_T eSrc;

    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    eSrc = (MTVDECEX_SRC_T)rArg.ai4Arg[1];
    
    MTVDECEX_PRINT(" - ePath = %d, eSrc = %d\n",
		(int)ePath,
		(int)eSrc);
    
    u1Ret= bApiVideoSetVideoSrc((UINT8)ePath, _MTVDECEX_VdoSrcMap(eSrc));

    if (_MTVDECEX_VdoSrcMap(eSrc) == SV_VS_DTV1)
    {
        VDP_SetInput((UCHAR)ePath, 0, 0);
    }
	else
    {
        VDP_SetInput((UCHAR)ePath, 0xFF, 0);
    }

    if((u1Ret == SV_NO_CHANGE) ||(u1Ret == SV_SUCCESS))
    	return MTR_OK;
    else   	//if(u1Ret == SV_FAIL)
    	return MTR_NOT_OK;

}

static MT_RESULT_T _MTVDECEX_SetMainSubSrc(unsigned long arg)
{
    UINT8 u1Ret;
    MTVDECEX_SRC_T eMainSrc;
    MTVDECEX_SRC_T eSubSrc;

    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eMainSrc = (MTVDECEX_SRC_T )rArg.ai4Arg[0];
    eSubSrc = (MTVDECEX_SRC_T )rArg.ai4Arg[1];
    MTVDECEX_PRINT(" - eMainSrc = %d, eSubSrc = %d\n",
		(int)eMainSrc,
		(int)eSubSrc);

    u1Ret=bApiVideoMainSubSrc(_MTVDECEX_VdoSrcMap(eMainSrc), _MTVDECEX_VdoSrcMap(eSubSrc));

   // if(u1Ret == SV_NO_CHANGE)
   // 	return MTR_PARAMETER_NOCHANGE;
   // else if(u1Ret == SV_SUCCESS)
   if((u1Ret == SV_NO_CHANGE) ||(u1Ret == SV_SUCCESS))
    	return MTR_OK;
    else   	//if(u1Ret == SV_FAIL)
    	return MTR_NOT_OK;
}

static MT_RESULT_T _MTVDECEX_SetColorSystem(unsigned long arg)
{
    //UINT8 u1Ret;
    MTVDECEX_CS_T eColSys;
    eColSys = (MTVDECEX_CS_T)arg;

    MTVDECEX_PRINT(" - eColSys = %d\n",(int)eColSys);

    if( fgApiVideoColorSystem(eColSys) != SV_TRUE)
    {
    	return MTR_NOT_OK;
    }	
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_GetColorSystem(unsigned long arg)
{
    //MTVDO_PATH_T ePath;
    MTVDECEX_CS_T *peColSys;
    MTVDECEX_CS_T eColSys;

    peColSys =  (MTVDECEX_CS_T *)arg;
    
    eColSys = bApiVideoGetColorSystem();
    
    USR_SPACE_ACCESS_VALIDATE_ARG(peColSys,MTVDECEX_CS_T );
    COPY_TO_USER_ARG(peColSys,eColSys,MTVDECEX_CS_T );

    MTVDECEX_PRINT(" - eColSys = %d\n",(int)eColSys);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_SetPorch(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    MTVDECEX_PORCH_TYPE_T ePorchType;
    UINT16 u2Value;

    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    ePorchType = (MTVDECEX_PORCH_TYPE_T)rArg.ai4Arg[1];
    u2Value = (UINT16)rArg.ai4Arg[2];
    MTVDECEX_PRINT(" - ePath = %d, ePorchType = %d, u2Value = %d\n",
		(int)ePath,
		(int)ePorchType,
		(int)u2Value);

    if (_u4OversampleDependent == 0)
    {
        if (bDrvVideoGetWidthOverSample(ePath) && (ePorchType == MTVDECEX_HPORCH_CURRENT))
        {
            u2Value = u2Value<<1;
        }
    }

    wDrvVideoSetPorch(ePath, ePorchType,u2Value);
    
    return MTR_OK;
}
static MT_RESULT_T _MTVDECEX_GetTvdNoiseLevel(unsigned long arg)
{
    MTVDECEX_NOISE_MODE eNmode;    
    UINT16 *pu2Value;
    UINT16  u2Value;

    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eNmode = (MTVDECEX_NOISE_MODE)rArg.ai4Arg[0];   
    pu2Value = (UINT16 *)rArg.ai4Arg[1];

    u2Value = bTvdSignalStrengthLevel(eNmode);

    MTVDECEX_PRINT(" - noise_mode = %d, u2Value = %d\n",
		(int)eNmode,		
		(int)u2Value);
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu2Value,UINT16);
    COPY_TO_USER_ARG(pu2Value,u2Value,UINT16);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_GetPorch(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    MTVDECEX_PORCH_TYPE_T ePorchType;
    UINT16 *pu2Value;
    UINT16  u2Value;

    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    ePorchType = (MTVDECEX_PORCH_TYPE_T)rArg.ai4Arg[1];
    pu2Value = (UINT16 *)rArg.ai4Arg[2];

    u2Value = wDrvVideoGetPorch(ePath, ePorchType);

    MTVDECEX_PRINT(" - ePath = %d, ePorchType = %d, u2Value = %d\n",
		(int)ePath,
		(int)ePorchType,
		(int)u2Value);

    if (_u4OversampleDependent == 0)
    {
        if (bDrvVideoGetWidthOverSample(ePath) && 
        	((ePorchType == MTVDECEX_HPORCH_CURRENT) || (ePorchType == MTVDECEX_HPORCH_DEFAULT) || (ePorchType == MTVDECEX_HPORCH_MIN) || (ePorchType == MTVDECEX_HPORCH_MAX)))
        {
            u2Value = u2Value >> 1;
        }
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu2Value,UINT16);
    COPY_TO_USER_ARG(pu2Value,u2Value,UINT16);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_StartVgaAuto(VOID)
{
       MTVDECEX_PRINT(" - void\n");
	if(fgApiVideoVgaAuto()==SV_FALSE)
	{
		return MTR_NOT_OK;
	}
	return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_StopVgaAuto(VOID)
{
       MTVDECEX_PRINT(" - void\n");
	vDrvVgaAutoStop();
	return MTR_OK;
}


static MT_RESULT_T _MTVDECEX_StartAutoColor(VOID)
{
       MTVDECEX_PRINT(" - void\n");
	if(fgApiVideoAutoColor()==SV_FALSE)
	{
		return MTR_NOT_OK;
	}
	return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_SetPhase(unsigned long arg)
{
    UINT8 u1Value;
    u1Value = (UINT8)arg;
    MTVDECEX_PRINT(" - u1Value = %d\n",u1Value);

    if(fgApiVideoVgaSetPhase(u1Value)==SV_FALSE)
    {
    	return MTR_NOT_OK;
    }
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_GetPhase(unsigned long arg)
{
    MTVDECEX_PHASE_TYPE_T ePhaseType;
    UINT8 *pu1Value;
    UINT8  u1Value;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    ePhaseType = (MTVDECEX_PHASE_TYPE_T )rArg.ai4Arg[0];
    pu1Value = (UINT8 *)rArg.ai4Arg[1];

    u1Value =u1DrvVGAGetPhase();//bDrvVGAGetPhase();

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Value,UINT8);
    COPY_TO_USER_ARG(pu1Value,u1Value,UINT8);

    MTVDECEX_PRINT(" - ePhaseType = %d, u1Value = %d\n",
		(int)ePhaseType,
		(int)u1Value);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_SetClock(unsigned long arg)
{
    UINT16 u2Value;
    u2Value = (UINT16)arg;
    MTVDECEX_PRINT(" - u2Value = %d\n",(int)u2Value);

    if(fgApiVideoVgaSetClock(u2Value)==SV_FALSE)
    {
    	return MTR_NOT_OK;
    }

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_GetClock(unsigned long arg)
{
    MTVDECEX_CLOCK_TYPE_T eClockType;
    UINT16 *pu2Value;
    UINT16  u2Value;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eClockType = (MTVDECEX_CLOCK_TYPE_T)rArg.ai4Arg[0];
    pu2Value = (UINT16 *)rArg.ai4Arg[1];

    if(eClockType == MTVDECEX_VGA_CLOCK_CURRENT)
    {
    	u2Value= wDrvVGAGetClock();
    }
    else if(eClockType == MTVDECEX_VGA_CLOCK_MAX)
    {
    	u2Value =255;
    }
    else //if(eClockType == MTVDECEX_VGA_CLOCK_MIN)
    {
    	u2Value =0;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pu2Value,UINT16);
    COPY_TO_USER_ARG(pu2Value,u2Value,UINT16);

    MTVDECEX_PRINT(" - eClockType = %d, u2Value = %d\n",
		(int)eClockType,
		(int)u2Value);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VGA_SetAmbiguousUserSel(unsigned long arg)
{
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
	BOOL fgUserSel;

	fgUserSel = (BOOL)arg;

	MTVDECEX_PRINT(" - fgUserSel = %d\n",(int)fgUserSel);
	vVgaSetAmbiguousUserSel(fgUserSel);
#endif	

       return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VGA_GetAmbiguousTimingNum(unsigned long arg)
{
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT
    UINT8 *pu1AmbiguousTimingNum;
    UINT8  u1AmbiguousTimingNum;
    pu1AmbiguousTimingNum = (UINT8 *)arg;

    //add driver implementation code here
    u1AmbiguousTimingNum = bVgaGetAmbiguousTimingNum();

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1AmbiguousTimingNum,UINT8);
    COPY_TO_USER_ARG(pu1AmbiguousTimingNum,u1AmbiguousTimingNum,UINT8);

    MTVDECEX_PRINT(" - u1AmbiguousTimingNum = %d\n",(int)u1AmbiguousTimingNum);
#endif
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VGA_QueryAmbiguousTiming(unsigned long arg)
{
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT    
    UINT8 i, u1OptionIdx, bTimingNum;
    MTVDECEX_VGA_AMBIG_TIMING_T* prAmbigVgaTimingInfo;
    MTVDECEX_VGA_AMBIG_TIMING_T  rAmbigVgaTimingInfo[3];
    VGAAmbTimingInfo_t rTmpVgaTimingInfo[3];
    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1OptionIdx = (UINT8)rArg.ai4Arg[0];
    prAmbigVgaTimingInfo = (MTVDECEX_VGA_AMBIG_TIMING_T *)rArg.ai4Arg[1];
    bTimingNum = (UINT8)rArg.ai4Arg[2];

    //add driver implementation code here
    vVgaQueryAmbiguousTiming(u1OptionIdx+1, rTmpVgaTimingInfo, bTimingNum);
    MTVDECEX_PRINT(" - u1OptionIdx = %d, bTimingNum = %d\n",(int)u1OptionIdx, (int)bTimingNum);

    for(i=0; i<bTimingNum; i++)
    {
	    rAmbigVgaTimingInfo[i].u2Width =rTmpVgaTimingInfo[i].wHor_res;
	    rAmbigVgaTimingInfo[i].u2Height=rTmpVgaTimingInfo[i].wVer_res;
	    rAmbigVgaTimingInfo[i].u1RefreshRate= rTmpVgaTimingInfo[i].bVer_freq; 
	    USR_SPACE_ACCESS_VALIDATE_ARG(prAmbigVgaTimingInfo+i*sizeof(MTVDECEX_VGA_AMBIG_TIMING_T), MTVDECEX_VGA_AMBIG_TIMING_T);
    	    COPY_TO_USER_ARG(prAmbigVgaTimingInfo+i*sizeof(MTVDECEX_VGA_AMBIG_TIMING_T), rAmbigVgaTimingInfo[i], MTVDECEX_VGA_AMBIG_TIMING_T);
	    MTVDECEX_PRINT_AMBIG_TIMINGT(rAmbigVgaTimingInfo[i]);
    }
#endif    


    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VGA_SetAmbiguousIndex(unsigned long arg)
{
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT    
    UINT8 u1OptionIdx;
    BOOL fgSetAsDefault;

    MTAL_IOCTL_2ARG_T    rArg;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1OptionIdx = rArg.ai4Arg[0];
    fgSetAsDefault = (BOOL) rArg.ai4Arg[1];
    
    MTVDECEX_PRINT(" - u1OptionIdx = %d, fgSetAsDefault = %d\n",(int)u1OptionIdx, (int)fgSetAsDefault);
    //add driver implementation code here
    vVgaSetAmbiguousTiming(u1OptionIdx+1, fgSetAsDefault);
#endif
    
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VGA_GetCurrentAmbiguousIndex(unsigned long arg)
{
#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT    
    UINT8 *pu1CurrentAmbiguousIndex;
    UINT8  u1CurrentAmbiguousIndex;
    VGAAmbTimingInfo_t VGAInfo;
    pu1CurrentAmbiguousIndex= (UINT8 *)arg;

    //add driver implementation code here
    vVgaQueryAmbiguousTiming(QUERY_CUR_AMBIGUOUS_TIMING, &VGAInfo, 1);
    u1CurrentAmbiguousIndex = VGAInfo.bCandiIdx-1;

    USR_SPACE_ACCESS_VALIDATE_ARG(pu1CurrentAmbiguousIndex,UINT8);
    COPY_TO_USER_ARG(pu1CurrentAmbiguousIndex,u1CurrentAmbiguousIndex,UINT8);
    MTVDECEX_PRINT(" - u1CurrentAmbiguousIndex = %d\n",(int)u1CurrentAmbiguousIndex);
#endif    
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_RegSigChgNfy( MTVDO_PATH_T ePath, MTVDECEX_SIG_STATUS eSigStatus, UINT8 u1SigSkip)
{
    static  UINT16 u2SigStatus[2][5] = {{0, 0xffff, 0xffff, 0xffff, 0xffff},//main
                                                                 {0, 0xffff, 0xffff, 0xffff, 0xffff}};    //sub
    static UINT8 u1SigSrc[2]={0xff, 0xff};

    MTVDECEX_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = ePath;
    rVdoCbT.u4Arg2 = eSigStatus;
    rVdoCbT.u4Arg3 = wApiVideoGetInputWidth(ePath);    
    rVdoCbT.u4Arg4 = wApiVideoGetInputHeight(ePath);
    rVdoCbT.u4Arg5 = (UINT16)bApiVideoGetRefreshRate(ePath);
    rVdoCbT.u4Arg6 = (UINT16)bDrvVideoIsSrcInterlace(ePath);

   // if the input signal status is no change, don't call back.
   if((u2SigStatus[ePath][0] != (UINT16)rVdoCbT.u4Arg2) ||
   	(u2SigStatus[ePath][1] != (UINT16)rVdoCbT.u4Arg3) ||
   	(u2SigStatus[ePath][2] != (UINT16)rVdoCbT.u4Arg4) ||
   	(u2SigStatus[ePath][3] != (UINT16)rVdoCbT.u4Arg5) ||
   	(u2SigStatus[ePath][4] != (UINT16)rVdoCbT.u4Arg6) ||
   	(u1SigSrc[ePath] != bGetVdoSrcNew(ePath)))
   {
    _CB_PutEvent(CB_MTAL_VDECEX_SIG_CHG, sizeof(MTVDECEX_CB_T), (void*)&rVdoCbT);
   	 u2SigStatus[ePath][0] = (UINT16)rVdoCbT.u4Arg2;
   	 u2SigStatus[ePath][1] = (UINT16)rVdoCbT.u4Arg3;
   	 u2SigStatus[ePath][2] = (UINT16)rVdoCbT.u4Arg4;
   	 u2SigStatus[ePath][3] = (UINT16)rVdoCbT.u4Arg5;
   	 u2SigStatus[ePath][4] = (UINT16)rVdoCbT.u4Arg6;
   	 u1SigSrc[ePath] = bGetVdoSrcNew(ePath);   	 	
   }
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_VGAAutoNfy( MTVDO_PATH_T ePath, UINT8 u1Progress)
{
    MTVDECEX_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = ePath;
    rVdoCbT.u4Arg2 = u1Progress;
    
    _CB_PutEvent(CB_MTAL_VDECEX_VGA_AUTO, sizeof(MTVDECEX_CB_T), (void*)&rVdoCbT);
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_AutoColorNfy( MTVDO_PATH_T ePath, UINT8 u1Success)
{
    MTVDECEX_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = ePath;
    rVdoCbT.u4Arg2 = u1Success;
    
    _CB_PutEvent(CB_MTAL_VDECEX_AUTO_COLOR, sizeof(MTVDECEX_CB_T), (void*)&rVdoCbT);
    return MTR_OK;
}
#if 1
static MT_RESULT_T _MTVDECEX_AFDNfy( MTVDO_PATH_T ePath, UINT32 u4Aspection)
{
    MTVDECEX_CB_T  rVdoCbT;

    rVdoCbT.u4Arg1 = ePath;
    rVdoCbT.u4Arg2 = u4Aspection;
    printf("AFD, ePath(%d), u4Aspection(%d)\n", ePath, u4Aspection);
    _CB_PutEvent(CB_MTAL_VDECEX_AFD, sizeof(MTVDECEX_CB_T), (void*)&rVdoCbT);
    return MTR_OK;
}

#endif 

extern VOID VDEC_SetAFDNfy(const AFD_VDEC_STATUS_CB AFDNfy);

static MT_RESULT_T _MTVDECEX_RegCbFunc(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    MTVDECEX_CB_FUNC_ENUM_T eFuncType;
    UINT32 u4FuncPtr;
    UINT32 u4Arg1;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    eFuncType = (MTVDECEX_CB_FUNC_ENUM_T)rArg.ai4Arg[0];
    u4FuncPtr = (UINT32)rArg.ai4Arg[1];
    u4Arg1 = (UINT32)rArg.ai4Arg[2];

    MTVDECEX_PRINT(" -IO: eFuncType = %d, u4FuncPtr = %ul, u4Arg1 = %ul\n",
        eFuncType, u4FuncPtr, u4Arg1);

    switch (eFuncType)
    {
        case MTVDECEX_CB_FUNC_SIG_CHG:
            NPTV_SetSigChgNfyFct( (NPTV_PFN_SIG_NFY_FCT)_MTVDECEX_RegSigChgNfy);
            break;
        case MTVDECEX_CB_FUNC_VGA_AUTO:
            NPTV_SetVgaAutoNfy( (NPTV_PFN_VGA_AUTO_NFY)_MTVDECEX_VGAAutoNfy);
            break;
        case MTVDECEX_CB_FUNC_AUTO_COLOR:
            NPTV_SetVgaAutocolorNfy( (NPTV_PFN_VGA_AUTOCOLOR_NFY)_MTVDECEX_AutoColorNfy);
        case MTVDECEX_CB_FUNC_AFD:
            printf("AFD, set AFD callback function\n");
            VDEC_SetAFDNfy( (AFD_VDEC_STATUS_CB)_MTVDECEX_AFDNfy);
            break;
        default:
            return MTR_NOT_OK;
            break;
    }
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_HDMI_GetAspectRatio(unsigned long arg)
{
    MTVDECEX_HDMI_ARATIO_T* peRatio;
    MTVDECEX_HDMI_ARATIO_T  eRatio;
    peRatio = (MTVDECEX_HDMI_ARATIO_T* )arg;

    //add driver implementation code here
   if( bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_NOSIGNAL) {
       eRatio = MTVDECEX_HDMI_ARATIO_NODATA;
   }
   else {
    	 if(_bHdmiMode){
  	      switch (bHDMIAspectRatio()) {
  	          case 0: 
  	          	    eRatio = MTVDECEX_HDMI_ARATIO_NODATA;
  	                 break;
  	          case 1: 
  	          	    eRatio = MTVDECEX_HDMI_ARATIO_4_3;
  	                 break;
  	          case 2: 
  	          	    eRatio = MTVDECEX_HDMI_ARATIO_16_9;
  	                 break;
  	          case 3: 
  	          	    eRatio = MTVDECEX_HDMI_ARATIO_RESERVED;
  	                 break;
  	          default:
  	          	    eRatio = MTVDECEX_HDMI_ARATIO_NODATA;
  	                 break;    
  	      }
  	 }
    	 else {
   	     eRatio = MTVDECEX_HDMI_ARATIO_NODATA;
   	 }
   }	
    
    USR_SPACE_ACCESS_VALIDATE_ARG(peRatio, MTVDECEX_HDMI_ARATIO_T);
    COPY_TO_USER_ARG(peRatio, eRatio, MTVDECEX_HDMI_ARATIO_T);

    MTVDECEX_PRINT(" - eRatio = %d\n",(int)eRatio);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_HDMI_SetHPDAdjust(unsigned long arg)
{
    UINT32 u4Value;
    u4Value = (UINT32)arg;
    MTVDECEX_PRINT(" - u4Value = %d\n",(int)u4Value);
    vHDMIHPDAdjust(u4Value);
    //add driver implementation code here
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_HDMI_GetInfoFrame(unsigned long arg)
{
    MTVDECEX_HDMI_INFOFRAME* prFrame ;
    MTVDECEX_HDMI_INFOFRAME  rFrame ;
    MTVDECEX_HDMI_INFOFRAME* frame;
    UINT8 u1FrameType;

    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    prFrame = (MTVDECEX_HDMI_INFOFRAME*)rArg.ai4Arg[0];
    u1FrameType = (UINT8)rArg.ai4Arg[1];

   frame=(MTVDECEX_HDMI_INFOFRAME *) API_HDMI_Get_InfoFrame(u1FrameType);
   rFrame=*frame;

    USR_SPACE_ACCESS_VALIDATE_ARG(prFrame, MTVDECEX_HDMI_INFOFRAME);
    COPY_TO_USER_ARG(prFrame, rFrame,MTVDECEX_HDMI_INFOFRAME);

//    MTVDECEX_PRINT(" - u1FrameType = %x\n",(int)u1FrameType);
//    MTVDECEX_PRINT_INFO_FRAME(rFrame);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_HDMI_GetMode(unsigned long arg)
{
    MTVDECEX_HDMI_MODE_T* peMode;
    MTVDECEX_HDMI_MODE_T  eMode;
    peMode = (MTVDECEX_HDMI_MODE_T* )arg;

    //add driver implementation code here
 	if (fgIsMainDVI())
 	{
   		if( bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_NOSIGNAL)
   		{
   			eMode=MTVDECEX_HDMI_NO_SIGNAL;
   		}
   		else if(bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_UNKNOWN)
   		{
   		   eMode=MTVDECEX_HDMI_NO_SIGNAL;
   		}
   		else
   		{
   			if(_bHdmiMode)
   			{
   				eMode =MTVDECEX_HDMI_HDMI;
   			}
   			else
   			{
   				eMode =MTVDECEX_HDMI_DVI;
   			}
   		}	
   	} 
	else if (fgIsPipDVI())
	{
		if( bDrvVideoSignalStatus(SV_VP_PIP)==SV_VDO_NOSIGNAL)
   		{
   			eMode=MTVDECEX_HDMI_NO_SIGNAL;
   		}
   	    else if(bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_UNKNOWN)
   		{
   		   eMode=MTVDECEX_HDMI_NO_SIGNAL;
   		}
   		else
   		{
   			if(_bHdmiMode)
   			{
   				eMode =MTVDECEX_HDMI_HDMI;
   			}
   			else
   			{
   				eMode =MTVDECEX_HDMI_DVI;
   			}
   		}	
	}

    USR_SPACE_ACCESS_VALIDATE_ARG(peMode,MTVDECEX_HDMI_MODE_T);
    COPY_TO_USER_ARG(peMode,eMode,MTVDECEX_HDMI_MODE_T);

//  MTVDECEX_PRINT(" - eMode = %d\n",(int)eMode);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_HDMI_GetDeepColorMode(unsigned long arg)
{
    MTVDECEX_HDMI_DEEPCOLOR_MODE_T* peDeepColorMode;
    MTVDECEX_HDMI_DEEPCOLOR_MODE_T  eDeepColorMode;
    peDeepColorMode = (MTVDECEX_HDMI_DEEPCOLOR_MODE_T* )arg;

    if(bHDMIDeepColorStatus()==0x1)
    {
    	eDeepColorMode=MTVDECEX_HDMI_DEEPCOLOR_10BIT;
    }
    else  if(bHDMIDeepColorStatus()==0x2)
    {
    	eDeepColorMode=MTVDECEX_HDMI_DEEPCOLOR_12BIT;
    }
    else  if(bHDMIDeepColorStatus()==0x3)
    {
    	eDeepColorMode=MTVDECEX_HDMI_DEEPCOLOR_16BIT;
    }
    else
    {
    	eDeepColorMode=MTVDECEX_HDMI_DEEPCOLOR_8BIT;
    }
    USR_SPACE_ACCESS_VALIDATE_ARG(peDeepColorMode,MTVDECEX_HDMI_DEEPCOLOR_MODE_T);
    COPY_TO_USER_ARG(peDeepColorMode,eDeepColorMode,MTVDECEX_HDMI_DEEPCOLOR_MODE_T);

 //   MTVDECEX_PRINT(" - eDeepColorMode = %d\n",(int)eDeepColorMode);

    return MTR_OK;
}
static MT_RESULT_T _MTVDECEX_HDMI_IsPCM(unsigned long arg)
{
    MTVDECEX_HDMI_AUDIOTYPE_T* pePCM;
    MTVDECEX_HDMI_AUDIOTYPE_T  ePCM;
    pePCM = (MTVDECEX_HDMI_AUDIOTYPE_T* )arg;

    if(bHDMI_IsPCM()==0x0)
    {
    	ePCM=MTVDECEX_HDMI_AUDIOTYPE_PCM;
    }
    else  
    {
    	ePCM=MTVDECEX_HDMI_AUDIOTYPE_NONEPCM;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(pePCM,MTVDECEX_HDMI_AUDIOTYPE_T);
    COPY_TO_USER_ARG(pePCM,ePCM,MTVDECEX_HDMI_AUDIOTYPE_T);

    return MTR_OK;
}
static MT_RESULT_T _MTVDECEX_HDMI_GetAudioFrequency(unsigned long arg)
{
    UINT8 AUDIOFreq;
    MTVDECEX_HDMI_AUDIOFREQUENCY_T* peAUDIOFreq;
    MTVDECEX_HDMI_AUDIOFREQUENCY_T  eAUDIOFreq;
    peAUDIOFreq = (MTVDECEX_HDMI_AUDIOFREQUENCY_T* )arg;
    //AUDIOFreq=bHDMIAUDIOSampleRateCal();
    AUDIOFreq=RegReadFld(CHST1, AUD_SAMPLE_F);
    switch (AUDIOFreq)
    {
    	case AUD_FS_32K:
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_32K;
    	break;
    	case AUD_FS_44K:
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_44K;
    	break;
    	case AUD_FS_48K:
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_48K;
    	break;
    	case AUD_FS_88K:
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_88K;
    	break;
    	case AUD_FS_96K:
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_96K;
    	break;
    	case AUD_FS_176K:
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_176K;
    	break;
    	case AUD_FS_192K:
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_192K;
    	break;
    	case AUD_FS_UNKNOWN:
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_NOTINDICATED;
    	break;
    	default :
    		eAUDIOFreq=	MTVDECEX_HDMI_AUDIOFREQUENCY_UNKNOWN;
    	break;
    }


    USR_SPACE_ACCESS_VALIDATE_ARG(peAUDIOFreq,MTVDECEX_HDMI_AUDIOFREQUENCY_T);
    COPY_TO_USER_ARG(peAUDIOFreq,eAUDIOFreq,MTVDECEX_HDMI_AUDIOFREQUENCY_T);

    return MTR_OK;
}
static MT_RESULT_T _MTVDECEX_HDMI_GetAudioBitWidth(unsigned long arg)
{
    UINT8 BitWidth;
    MTVDECEX_HDMI_AUDIOLENGTH_T* peBitWidth;
    MTVDECEX_HDMI_AUDIOLENGTH_T  eBitWidth;
    peBitWidth = (MTVDECEX_HDMI_AUDIOLENGTH_T* )arg;
    BitWidth=bHDMIGetAudioBitWidth();
    switch (BitWidth)
    {
    	case HDMI_AUD_Length_16bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_16BITS;
    	break;
    	case HDMI_AUD_Length_17bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_17BITS;
    	break;
       case HDMI_AUD_Length_18bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_18BITS;
    	break;   
    	case HDMI_AUD_Length_19bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_19BITS;
    	break;
    	case HDMI_AUD_Length_20bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_20BITS;
    	break;
    	case HDMI_AUD_Length_21bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_21BITS;
    	break;
    	case HDMI_AUD_Length_22bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_22BITS;
    	break;
    	case HDMI_AUD_Length_23bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_23BITS;
    	break;
    	case HDMI_AUD_Length_24bits:
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_24BITS;
    	break;
    	default :
    		eBitWidth=MTVDECEX_HDMI_AUDIOLENGTH_UNKNOWN;
    	break;
    }


    USR_SPACE_ACCESS_VALIDATE_ARG(peBitWidth,MTVDECEX_HDMI_AUDIOLENGTH_T);
    COPY_TO_USER_ARG(peBitWidth,eBitWidth,MTVDECEX_HDMI_AUDIOLENGTH_T);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_HDMI_GetAudioCategoryCode(unsigned long arg)
{
    UINT8 u1CategoryCode;
    BOOL  fgCopyright;
    UINT8 u1WordLength;
    BOOL  fgIsPcm;
    
    UINT8* pu1CategoryCode;
   
    pu1CategoryCode = (UINT8* )arg;

    vHDMIAudChannelStatus(&fgCopyright, &u1CategoryCode, 
                                &u1WordLength, &fgIsPcm);
                                
    USR_SPACE_ACCESS_VALIDATE_ARG(pu1CategoryCode,UINT8);
    COPY_TO_USER_ARG(pu1CategoryCode,u1CategoryCode,UINT8);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetMAGC(unsigned long arg)
{
    UINT8 u1OnOff;
    u1OnOff = (UINT8)arg;
    MTVDECEX_PRINT(" - u1OnOff = 0x%x\n",u1OnOff);

    vTvd3dSetFixAgc(u1OnOff);
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetAGCAnalogGain(unsigned long arg)
{
    MTVDEC_TVD_CHANNEL_T* eChannelSel;
    UINT8 u1Value;

    MTAL_IOCTL_2ARG_T    rArg;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eChannelSel = (MTVDEC_TVD_CHANNEL_T*)rArg.ai4Arg[0];
    u1Value = (UINT8)rArg.ai4Arg[1];

    MTVDECEX_PRINT(" - eChannelSel = 0x%x\n"
    " - u1Value = 0x%x\n",
    eChannelSel,
    u1Value);

    u1Value =(u1Value>0x3f)?0x3f:u1Value; //limit 6 bits to meet PGA's spec
    if(eChannelSel == TVDADC_CHANNEL_A)
    {
        vDrvSetTVDADCGain(u1Value<<1);
    }
    else
    {
        vDrvSetTVDCHBGain(u1Value<<1);
    }
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetAGCDigitalGain(unsigned long arg)
{
    UINT16 u2Value;
    u2Value = (UINT16)arg;
    MTVDECEX_PRINT(" - u2Value = 0x%x\n",u2Value);

    vDrvSetTVDADCDGain(u2Value);
    return MTR_OK;
}
static MT_RESULT_T _MTVDECEX_TVD_SetClampLevel(unsigned long arg)
{
    MTVDEC_TVD_CHANNEL_T* eChannelSel;
    UINT16 u1Value;

    MTAL_IOCTL_2ARG_T    rArg;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eChannelSel = (MTVDEC_TVD_CHANNEL_T*)rArg.ai4Arg[0];
    u1Value = (UINT16)rArg.ai4Arg[1];

    MTVDECEX_PRINT(" - eChannelSel = 0x%x\n"
    " - u1Value = 0x%x\n",
    eChannelSel,
    u1Value);

    if(eChannelSel == TVDADC_CHANNEL_A)
    {
    	if((u1Value>(256+12))||(u1Value<(256-12))) //range:10bit 256 +- 12 code
    		return  MTR_PARAMETER_ERROR; 
    	else
		vDrvDFE(u1Value);
    }
    else
    {
        return MTR_NOT_SUPPORTED;
    }
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetAGCMode(unsigned long arg)
{
    MTVDEC_TVD_CHANNEL_T* eChannelSel;
    UINT8 u1Value;

    MTAL_IOCTL_2ARG_T    rArg;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eChannelSel = (MTVDEC_TVD_CHANNEL_T*)rArg.ai4Arg[0];
    u1Value = (UINT8)rArg.ai4Arg[1];

    MTVDECEX_PRINT(" - eChannelSel = 0x%x\n"
    " - u1Value = 0x%x\n",
    eChannelSel,
    u1Value);

    if(eChannelSel == TVDADC_CHANNEL_A)
    {
    	vTvd3dSetFixAgc(u1Value==0);
    }
    else
    {
        	vDrvSetTVDCHBSrcMux((u1Value==0)?2:0);
    }
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetCAGC(unsigned long arg)
{
    MTVDECEX_CAGC_PRM_T *prCAGCParam;
    MTVDECEX_CAGC_PRM_T rCAGCParam;/*kernel data*/
    prCAGCParam = (MTVDECEX_CAGC_PRM_T *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prCAGCParam, MTVDECEX_CAGC_PRM_T);
    COPY_FROM_USER_ARG(prCAGCParam, rCAGCParam, MTVDECEX_CAGC_PRM_T);

    MTVDO_PRINT(" rCAGCParam.u1MCAGC_EN = 0x%x\n"
        "rCAGCParam.u1MCAG = 0x%x\n"
        "rCAGCParam.u4Bound = 0x%lx\n"
        "rCAGCParam.u1L_Ste = 0x%x\n"
        "rCAGCParam.u1S_Step = 0x%x\n",
        rCAGCParam.u1MCAGC_EN,
        rCAGCParam.u1MCAGC,
        rCAGCParam.u4Bound,
        rCAGCParam.u1L_Step,
        rCAGCParam.u1S_Step);

    _rTvd3dCAGCInfo.MCAGC_EN = (UINT8) rCAGCParam.u1MCAGC_EN;
    _rTvd3dCAGCInfo.MCAGC = (UINT8) rCAGCParam.u1MCAGC;
    _rTvd3dCAGCInfo.Bound = (UINT32)rCAGCParam.u4Bound;
    _rTvd3dCAGCInfo.L_Step = (UINT8) rCAGCParam.u1L_Step;
    _rTvd3dCAGCInfo.S_Step = (UINT8) rCAGCParam.u1S_Step;
	
	vTvd3dSetCAGCbyAP();   
	
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetUVDelay(unsigned long arg)
{
    UINT8 u1YDelay;
    UINT8 u1UVDelay;

    MTAL_IOCTL_2ARG_T    rArg;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1YDelay = (UINT8)rArg.ai4Arg[0];
    u1UVDelay = (UINT8) rArg.ai4Arg[1];
    if((u1YDelay > 3) || (u1UVDelay > 3))
    {
        MTVDECEX_PRINT(" u1YDelay or u1UVDelay error input value (> 3).\n");
        return MTR_NOT_OK;
    }

    MTVDECEX_PRINT(" - u1YDelay = 0x%x, u1UVDelay = 0x%x\n",(int)u1YDelay, (int)u1UVDelay);
    vTvd3dSetYCDelaybyAP(u1YDelay, u1UVDelay);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_GetStatus(unsigned long arg)
{
    MTVDECEX_TVD_STATUS_T eType;
    UINT8 *pu1Value;
    UINT8  u1Value;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eType = (MTVDECEX_TVD_STATUS_T )rArg.ai4Arg[0];
    pu1Value = (UINT8 *)rArg.ai4Arg[1];

    switch(eType)
    {
        case MTVDECEX_TVD_VPRES:
            u1Value = fgHwTvdVPres();
        break;
        case MTVDECEX_TVD_VLOCK:
            u1Value = fgHwTvdVLock();
        break;
        case MTVDECEX_TVD_HLOCK: 
            u1Value = fgHwTvdVLock();
        break;
        case MTVDECEX_TVD_LLOCK: 
            u1Value = fgHwTvdLLock();
        break;
        case MTVDECEX_TVD_NRLVL: 
            u1Value = bHwTvdNRLevel();
        break;
        case MTVDECEX_TVD_NASTA: 
            u1Value = bHwTvdNAState();
        break;
	 case MTVDECEX_TVD_CHSCAN_LOCK:
            u1Value = fgDrvTvdCheckVPresOn();
        break;	
        default:
            u1Value = 0;
        break;
    }    
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu1Value,UINT8);
    COPY_TO_USER_ARG(pu1Value,u1Value,UINT8);

    MTVDECEX_PRINT(" - eType = %d, u1Value = %d\n",
		(int)eType,
		(int)u1Value);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetAAF(unsigned long arg)
{
    UINT8 u1OnOff, u1Value;

    MTAL_IOCTL_2ARG_T    rArg;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1OnOff = (UINT8)rArg.ai4Arg[0];
    u1Value = (UINT8)rArg.ai4Arg[1];

    MTVDECEX_PRINT(" - u1OnOff = 0x%x\n"
    " - u1Value = 0x%x\n",
    u1OnOff,
    u1Value);
    if(u1Value > 7)
    {
        MTVDECEX_PRINT(" u1Value error input value (> 7).\n");
        return MTR_NOT_OK;
    }
    vTvd3dSetAAFbyAP(u1OnOff, u1Value);
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetSFSetting(unsigned long arg)
{
    MTVDECEX_TVD_SECAM_T *prSecamParam;
    MTVDECEX_TVD_SECAM_T rSecamParam;/*kernel data*/
    prSecamParam = (MTVDECEX_TVD_SECAM_T *)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prSecamParam, MTVDECEX_TVD_SECAM_T);
    COPY_FROM_USER_ARG(prSecamParam, rSecamParam, MTVDECEX_TVD_SECAM_T);

    vTvdSetSFSetting(rSecamParam.uSecam01.u4Secam01);
	
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_SetAutoSearch(unsigned long arg)
{
    UINT8 u1Value;
    u1Value = (UINT8)arg;
    MTVDECEX_PRINT(" - u1Value = %d\n",u1Value);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_StartCVBSAutoColor(VOID)
{
       MTVDECEX_PRINT(" - void\n");
	if(fgApiVideoCVBSAutoColor()==SV_FALSE)
	{
		return MTR_NOT_OK;
	}
	return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_HoldMode(unsigned long arg)
{
    UINT8 u1OnOff;

    MTAL_IOCTL_2ARG_T    rArg;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1OnOff = (UINT8)rArg.ai4Arg[0];

    MTVDECEX_PRINT(" - u1OnOff = 0x%x\n", u1OnOff);
    
    vTvdHoldMode(u1OnOff);
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_AGCPedEn(unsigned long arg)
{
    UINT8 u1OnOff;

    MTAL_IOCTL_2ARG_T    rArg;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1OnOff = (UINT8)rArg.ai4Arg[0];

    MTVDECEX_PRINT(" - u1OnOff = 0x%x\n", u1OnOff);
    
    vTVD3dAGCPedEn(u1OnOff);
    return MTR_OK;
}

/*static MT_RESULT_T _MTVDECEX_HDMI_GetColorDomain(unsigned long arg)
{
	MTPQL_HDMI_COLOR_DOMAIN_T *pColorDomain;
	MTPQL_HDMI_COLOR_DOMAIN_T ColorDomain;
	pColorDomain=(MTPQL_HDMI_COLOR_DOMAIN_T*)arg;
	ColorDomain=bGetHDMIColorDomain(); 
	USR_SPACE_ACCESS_VALIDATE_ARG(pColorDomain,MTPQL_HDMI_COLOR_DOMAIN_T);
	COPY_TO_USER_ARG(pColorDomain,ColorDomain,MTPQL_HDMI_COLOR_DOMAIN_T);
	return MTR_OK;
}*/

static MT_RESULT_T _MTVDECEX_GetVideoDecType(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    MTVDO_DECODER_TYPE *peType;
    MTVDO_DECODER_TYPE  eType;
    UINT32 eDecType = SV_VD_NA;
    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    peType = (MTVDO_DECODER_TYPE *)rArg.ai4Arg[1];

    if(ePath == MTVDO_MAIN)
    {
    	eDecType = bGetVideoDecType(SV_VP_MAIN);
    }
    else if(ePath == MTVDO_SUB)
    {
    	eDecType = bGetVideoDecType(SV_VP_PIP);
    }
    else
    {
        return MTR_NOT_OK;
    }
    switch(eDecType)
    {
        case SV_VD_NA:
            eType = MTVDO_DEC_NA;
            break;
        case SV_VD_TVD3D:
            eType = MTVDO_DEC_TVD3D;
            break;
        case SV_VD_YPBPR:
            eType = MTVDO_DEC_YPBPR;
            break;
        case SV_VD_VGA:
            eType = MTVDO_DEC_VGA;
            break;
        case SV_VD_DVI:
            eType = MTVDO_DEC_DVI;
            break;
        case SV_VD_MPEGHD:
            eType = MTVDO_DEC_MPEGHD;
            break;
        case SV_VD_MPEGSD:
            eType = MTVDO_DEC_MPEGSD;
            break;
        default:
            eType = MTVDO_DEC_MAX;
            break;
    }


    USR_SPACE_ACCESS_VALIDATE_ARG(peType,MTVDO_DECODER_TYPE);
    COPY_TO_USER_ARG(peType,eType,MTVDO_DECODER_TYPE);

    MTVDECEX_PRINT(" - ePath = %d, eType = %d\n",
		(int)ePath,
		(int)eType);

    return MTR_OK;

}



static MT_RESULT_T _MTVDECEX_TVD_GetDefaultPorch(unsigned long arg)
{
    MTVDO_PATH_T ePath;
    MTVDECEX_PORCH_TYPE_T ePorchType;
    UINT16 *pu2Value;
    UINT16  u2Value;

    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    ePath = (MTVDO_PATH_T)rArg.ai4Arg[0];
    ePorchType = (MTVDECEX_PORCH_TYPE_T)rArg.ai4Arg[1];
    pu2Value = (UINT16 *)rArg.ai4Arg[2];

    u2Value = wTvdGetDefaultPorch(ePath, ePorchType);

    MTVDECEX_PRINT(" - ePath = %d, ePorchType = %d, u2Value = %d\n",
		(int)ePath,
		(int)ePorchType,
		(int)u2Value);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu2Value,UINT16);
    COPY_TO_USER_ARG(pu2Value,u2Value,UINT16);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetChannelChange(unsigned long arg)
{
    UINT8 u1Value;
    u1Value = (UINT8)arg;
    MTVDECEX_PRINT(" - u1Value = %d\n",u1Value);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetEnabledColorSystem(unsigned long arg)
{
    UINT32 u4DrvEnabledColSys=0,u4EnabledColSys=0;
    u4EnabledColSys = (MTVDECEX_CS_T)arg;

    MTVDECEX_PRINT(" - u4EnabledColSys = %d\n",(int)u4EnabledColSys);

    
    if(u4EnabledColSys & (1<<MTVDECEX_CS_PAL_N))
        u4DrvEnabledColSys |= (1<<SV_CS_PAL_N);
    
    if(u4EnabledColSys & (1<<MTVDECEX_CS_PAL))
        u4DrvEnabledColSys |= (1<<SV_CS_PAL);

    if(u4EnabledColSys & (1<<MTVDECEX_CS_PAL_M))
        u4DrvEnabledColSys |= (1<<SV_CS_PAL_M);
    
    if(u4EnabledColSys & (1<<MTVDECEX_CS_NTSC358))
        u4DrvEnabledColSys |= (1<<SV_CS_NTSC358);

    if(u4EnabledColSys & (1<<MTVDECEX_CS_SECAM))
        u4DrvEnabledColSys |= (1<<SV_CS_SECAM);

    if(u4EnabledColSys & (1<<MTVDECEX_CS_PAL_60))
        u4DrvEnabledColSys |= (1<<SV_CS_PAL_60); //PAL with pedestal

    if(u4EnabledColSys & (1<<MTVDECEX_CS_NTSC443))
        u4DrvEnabledColSys |= (1<<SV_CS_NTSC443); //PAL with pedestal

    if( vDrvTvd3dSetEnabledColorSystem(u4DrvEnabledColSys) != SV_TRUE)
    {
    	return MTR_NOT_OK;
    }	
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetVPresOnOffGain(unsigned long arg)
{
    UINT8 u1OffGain;
    UINT8 u1OnGain;
    //UINT8 u1OnOffGain;

    MTAL_IOCTL_2ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u1OffGain = (UINT8)rArg.ai4Arg[0];
    u1OnGain = (UINT8)rArg.ai4Arg[1];
    //u1OnOffGain = ((u1OnGain & 0x0F)<<4) | (u1OffGain & 0x0F);
    
    vDrvTvd3dSetVPresOnOffGain(u1OffGain, u1OnGain, DFE_NORMAL_ONOFF_THRD);

    MTVDECEX_PRINT(" - u1OffGain = %d, u1OnGain = %d\n",
		(int)u1OffGain,
		(int)u1OnGain);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_TVD_SetHPLL(unsigned long arg)
{
    UINT8 u1KPGain;
    UINT8 u1KIGain;
    UINT32 u4ColorSys;

    MTAL_IOCTL_3ARG_T    rArg;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u1KPGain = (UINT8)rArg.ai4Arg[0];
    u1KIGain = (UINT8)rArg.ai4Arg[1];
    u4ColorSys = (UINT32)rArg.ai4Arg[2];

    vDrvTvd3dSetHPLL(u1KPGain, u1KIGain, u4ColorSys);

    MTVDECEX_PRINT(" - u1KPGain = %d, u1KIGain = %d, u4ColorSys = %d\n",
		(int)u1KPGain,
		(int)u1KIGain,
		(int)u4ColorSys);

    return MTR_OK;
}
static MT_RESULT_T _MTVDECEX_SetCompentToVGASupport(unsigned long arg)
{
     UINT32 u4Support = (UINT32)arg;

    _fgIsSupportVGAToYpbpr = u4Support;
    printf(" - _fgIsSupportVGAToYpbpr ================== %d\n",(int)_fgIsSupportVGAToYpbpr);
    return MTR_OK;
}


int mtal_ioctl_mtvdecex(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_OK;

	switch (cmd)
	{
        case MTAL_IO_VDECEX_ISPCTIMING:
        	i4Ret = _MTVDECEX_IsPCTiming(arg);
            break;
        case MTAL_IO_VDECEX_ISOVERSAMPLETIMING:
        	i4Ret = _MTVDECEX_IsOversampleTiming(arg);
            break;
        case MTAL_IO_VDECEX_SETOVERSAMPLEDEPENDENT:
        	i4Ret = _MTVDECEX_SetOversampleDependent(arg);
            break;
        case MTAL_IO_VDECEX_GETOVERSAMPLEDEPENDENT:
        	i4Ret = _MTVDECEX_GetOversampleDependent(arg);
            break;
        case MTAL_IO_DECEX_SETVIDEOSRC:
        	i4Ret = _MTVDECEX_SetVideoSrc(arg);
            break;
        case MTAL_IO_VDECEX_SETMAINSUBSRC:
        	i4Ret = _MTVDECEX_SetMainSubSrc(arg);
            break;
        case MTAL_IO_VDECEX_SETCOLORSYSTEM:
        	i4Ret = _MTVDECEX_SetColorSystem(arg);
            break;
        case MTAL_IO_VDECEX_GETCOLORSYSTEM:
        	i4Ret = _MTVDECEX_GetColorSystem(arg);
            break;
        case MTAL_IO_VDECEX_SETPORCH:
        	i4Ret = _MTVDECEX_SetPorch(arg);
            break;
        case MTAL_IO_VDECEX_GETPORCH:
        	i4Ret = _MTVDECEX_GetPorch(arg);
            break;
        case MTAL_IO_VDECEX_STARTVGAAUTO:
        	i4Ret = _MTVDECEX_StartVgaAuto();
            break;
        case MTAL_IO_VDECEX_STOPVGAAUTO:
        	i4Ret = _MTVDECEX_StopVgaAuto();
            break;

        case MTAL_IO_VDECEX_STARTAUTOCOLOR:
        	i4Ret = _MTVDECEX_StartAutoColor();
            break;
        case MTAL_IO_VDECEX_SETPHASE:
        	i4Ret = _MTVDECEX_SetPhase(arg);
            break;
        case MTAL_IO_VDECEX_GETPHASE:
        	i4Ret = _MTVDECEX_GetPhase(arg);
            break;
        case MTAL_IO_VDECEX_SETCLOCK:
        	i4Ret = _MTVDECEX_SetClock(arg);
            break;
        case MTAL_IO_VDECEX_GETCLOCK:
        	i4Ret = _MTVDECEX_GetClock(arg);
            break;
        case MTAL_IO_VDECEX_VGA_SETAMBIGUOUSUSRSEL:
        	i4Ret = _MTVDECEX_VGA_SetAmbiguousUserSel(arg);
            break;
        case MTAL_IO_VDECEX_VGA_GETAMBIGUOUSTIMINGNUM:
        	i4Ret = _MTVDECEX_VGA_GetAmbiguousTimingNum(arg);
            break;
        case MTAL_IO_VDECEX_VGA_QUERYAMBIGUOUSTIMING:
        	i4Ret = _MTVDECEX_VGA_QueryAmbiguousTiming(arg);
            break;
        case MTAL_IO_VDECEX_VGA_SETAMBIGUOUSINDEX:
        	i4Ret = _MTVDECEX_VGA_SetAmbiguousIndex(arg);
            break;
        case MTAL_IO_VDECEX_VGA_GETCURRENTAMBIGUOUSINDEX:
        	i4Ret = _MTVDECEX_VGA_GetCurrentAmbiguousIndex(arg);
            break;
        case MTAL_IO_VDECEX_REGCB:
        	i4Ret = _MTVDECEX_RegCbFunc(arg);
            break;
        case MTAL_IO_VDECEX_HDMI_GETASPECTRATIO:
        	i4Ret = _MTVDECEX_HDMI_GetAspectRatio(arg);
            break;            
        case MTAL_IO_VDECEX_HDMI_SETHPDADJUST:
        	i4Ret = _MTVDECEX_HDMI_SetHPDAdjust(arg);
            break;
        case MTAL_IO_VDECEX_HDMI_GETINFOFRAME:
        	i4Ret = _MTVDECEX_HDMI_GetInfoFrame(arg);
            break;
        case MTAL_IO_VDECEX_HDMI_GETMODE:
        	i4Ret = _MTVDECEX_HDMI_GetMode(arg);
            break;
        case MTAL_IO_VDECEX_HDMI_GETDEEPCOLORMODE:
        	 i4Ret = _MTVDECEX_HDMI_GetDeepColorMode(arg);
            break;
        case MTAL_IO_VDECEX_HDMI_ISPCM:
        	 i4Ret = _MTVDECEX_HDMI_IsPCM(arg);
            break;
        case MTAL_IO_VDECEX_HDMI_GETAUDIOFREQUENCY:
        	 i4Ret = _MTVDECEX_HDMI_GetAudioFrequency(arg);
            break;
        case MTAL_IO_VDECEX_HDMI_GETDAUDIOBITWIDTH:
        	 i4Ret = _MTVDECEX_HDMI_GetAudioBitWidth(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SETMAGC:
        	 i4Ret = _MTVDECEX_TVD_SetMAGC(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SET2CHPGAGAIN:
        	 i4Ret = _MTVDECEX_TVD_SetAGCAnalogGain(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SETAGCDIGITALGAIN:
        	 i4Ret = _MTVDECEX_TVD_SetAGCDigitalGain(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SETCAGC:
        	 i4Ret = _MTVDECEX_TVD_SetCAGC(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SETUVDELAY:
        	 i4Ret = _MTVDECEX_TVD_SetUVDelay(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SET2CHCLAMPLEVEL:
             i4Ret = _MTVDECEX_TVD_SetClampLevel(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SET2CHAGCMODE:
             i4Ret = _MTVDECEX_TVD_SetAGCMode(arg);
            break;
         case MTAL_IO_VDECEX_TVD_GETSTATUS:
             i4Ret = _MTVDECEX_TVD_GetStatus(arg);
            break;
         case MTAL_IO_VDECEX_TVD_SETAAF:
             i4Ret = _MTVDECEX_TVD_SetAAF(arg);
            break;
         case MTAL_IO_VDECEX_TVD_SETSECAMSETTING:
             i4Ret = _MTVDECEX_TVD_SetSFSetting(arg);
            break;     
		case MTAL_IO_VDECEX_TVD_GETTVDNOISELEVEL:
        	i4Ret = _MTVDECEX_GetTvdNoiseLevel(arg);
            break;
            case MTAL_IO_VDECEX_YPBPR_SETYPBPRTOVGA:
                i4Ret = _MTVDECEX_SetCompentToVGASupport(arg);
                break;
        case MTAL_IO_VDECEX_SETAUTOSEARCH:
             i4Ret = _MTVDECEX_SetAutoSearch(arg);
            break;            
        case MTAL_IO_VDECEX_STARTCVBSAUTOCOLOR:
        	i4Ret = _MTVDECEX_StartCVBSAutoColor();
            break;
        case MTAL_IO_VDECEX_TVD_HOLDMODE:
             i4Ret = _MTVDECEX_TVD_HoldMode(arg);
             break;
        case MTAL_IO_VDECEX_HDMI_GETDAUDIOCATEGORYCODE:
        	 i4Ret = _MTVDECEX_HDMI_GetAudioCategoryCode(arg);
            break;

        case MTAL_IO_VDECEX_TVD_PEDESTAL_EN:
            i4Ret = _MTVDECEX_TVD_AGCPedEn(arg);
            break;
        case MTAL_IO_VDECEX_GETVIDEODECODERTYPE:
            i4Ret = _MTVDECEX_GetVideoDecType(arg);
            break;
        case MTAL_IO_VDECEX_TVD_GETDEFAULTPORCH:
        	i4Ret = _MTVDECEX_TVD_GetDefaultPorch(arg);
        	break;
        case MTAL_IO_VDECEX_TVD_SETCHANNELCHANGE:
            i4Ret = _MTVDECEX_TVD_SetChannelChange(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SETENABLEDCOLORSYSTEM:
            i4Ret = _MTVDECEX_TVD_SetEnabledColorSystem(arg);
            break;
        case MTAL_IO_VDECEX_TVD_SETVPRESONOFFGAIN:
            i4Ret = _MTVDECEX_TVD_SetVPresOnOffGain(arg); 
            break;
        case MTAL_IO_VDECEX_TVD_SETHPLL:
            i4Ret = _MTVDECEX_TVD_SetHPLL(arg); 
            break;
      default:
            i4Ret = MTR_NOT_OK;
            break;

	}
	return i4Ret;
}

