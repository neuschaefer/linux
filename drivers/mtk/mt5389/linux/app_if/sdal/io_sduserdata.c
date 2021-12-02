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
 * $RCSfile: io_sduserdata.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_sduserdata.c
 *  SDAL SdUserData module ioctl interface of MT538X.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "SdUserData.h"
#include <linux/sdal_ioctl.h>
#include <linux/mt53xx_cb.h> // for Linux driver using callback function

#include "general.h"
#include "nptv_if.h"
#include "video_def.h" // For judge analog video source
#include "mpv_if.h"
#include "cb_low.h"
#include "sd_vdopath.h"
#include "x_assert.h"
#include "x_hal_5381.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MAX_USERDATA_TMPSIZE       256    
#define RATING_ONLY 0x80

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT8 u1NotifyOnOff;
    UINT8 u1EvenFieldOn;
    UINT8 u1OddFieldOn;
} UserData_CC_Notify_T;

typedef struct
{
    UINT8 u1NotifyOnOff; /* VBI Start */
    UINT8 u1FieldOn; /* VBI line enabled, TTX do not distinguish even/odd field */
} UserData_TTX_Notify_T;

typedef struct
{
    UINT8 u1NotifyOnOff; /* VBI Start */
} UserData_WSSB_Notify_T;

typedef struct
{
    UINT8 u1NotifyOnOff; /* VBI Start */
} UserData_VPS_Notify_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define USERDATA_DBG_MSG 0
#define CC_CHECK_PARITY 0

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UserData_CC_Notify_T rCCUDN = {SV_OFF, SV_OFF, SV_OFF};
static UserData_TTX_Notify_T rTTXUDN = {SV_OFF, SV_OFF};
static UserData_WSSB_Notify_T rWSSBUDN = {SV_OFF};
static UserData_VPS_Notify_T rVPSUDN = {SV_OFF};
static UINT8 _u1VChipOn = SV_OFF;

/// Is callback handler installed
static BOOL _fgInstallCallback = FALSE;

static UINT8 _au1UserDataTmpBuf[MAX_USERDATA_TMPSIZE];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _SdDgUserDataCb(SDAL_DGUSERDATA_CB_T* prDgUserdataCb)
{

    if (_fgInstallCallback)
    {
        _CB_PutEvent(CB_SDAL_DGUSERDATA_TRIGGER, sizeof(SDAL_DGUSERDATA_CB_T), 
                        (void*)prDgUserdataCb);
    }

}


#if 1 /* Not do anything in kernel mode */
static SdResult _SdUserData_InstallCallback(SdUserData_CallBack callback, uint32_t uwParam)
{
#if USERDATA_DBG_MSG
    printk("Callback Function: %d\n", callback);
    printk("uwParam: %d\n", uwParam);
    printk("In kernel _UserData_InstallCallback\n");
#endif

    if (MPV_RegUserDataCb(_SdDgUserDataCb))
    {
        _fgInstallCallback = TRUE;
    }
    
    return SD_OK;
}
#endif

static NPTV_PFN_CC_NFY _CC_Cb_driver(UINT8 u1Path, UINT8 u1data1, UINT8 u1data2, UINT8 u1field)
{
    SDAL_USERDATA_CB_T rUserdataCb;
    
    rUserdataCb.u1DataType = 0; /* CC */
    rUserdataCb.u1FieldID = u1field;
    rUserdataCb.u1Data0 = u1data1;
    rUserdataCb.u1Data1 = u1data2;
   
   switch(bDrvVideoGetTiming(u1Path))
   {
	case MODE_525I:
	case MODE_525I_OVERSAMPLE:
	case MODE_625I:
	case MODE_625I_OVERSAMPLE:
		  break;		
		  
	default:
		  return  0;
		  
   };
		
#if CC_CHECK_PARITY
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
   
    if(rCCUDN.u1NotifyOnOff == SV_ON)
    {
        //printf("%X, %X, %d\n", u1data1, u1data2, u1field);
        if((rCCUDN.u1EvenFieldOn==SV_ON) && (u1field==0))
        {
            _CB_PutEvent(CB_SDAL_USERDATA_TRIGGER, sizeof(SDAL_USERDATA_CB_T), &rUserdataCb);
        }
        if(u1field==1)
        {
            if(rCCUDN.u1OddFieldOn==SV_ON)
            {
                _CB_PutEvent(CB_SDAL_USERDATA_TRIGGER, sizeof(SDAL_USERDATA_CB_T), &rUserdataCb);
            }
            else if(_u1VChipOn==SV_ON)
            {
                rUserdataCb.u1FieldID |= RATING_ONLY;
                _CB_PutEvent(CB_SDAL_USERDATA_TRIGGER, sizeof(SDAL_USERDATA_CB_T), &rUserdataCb);
            }
        }
    }
    return 0;
}

static NPTV_PFN_TTX_NFY _TTX_Cb_driver(UINT8 u1Path, UINT32 u4PktsAvail, UINT32 readptr)
{
    SDAL_USERDATA_CB_T rUserDataTTXCb;

    rUserDataTTXCb.u1DataType = 1; /* TTX */
    rUserDataTTXCb.u4NumPktAvail = u4PktsAvail;
    rUserDataTTXCb.u4StartAddr = readptr;
    if((rTTXUDN.u1NotifyOnOff == SV_ON))
    {
        _CB_PutEvent(CB_SDAL_USERDATA_TRIGGER, sizeof(SDAL_USERDATA_CB_T), &rUserDataTTXCb);
    }
    return 0;
}

static NPTV_PFN_WSS_NFY _WSSB_Cb_driver(UINT8 u1Path, UINT32 u4Data)
{
    SDAL_USERDATA_CB_T rUserDataWSSBCb;

    rUserDataWSSBCb.u1DataType = 2; /* WSSB */
    rUserDataWSSBCb.u4NumPktAvail = 1;
    rUserDataWSSBCb.u4WSS= u4Data;
    if(rWSSBUDN.u1NotifyOnOff == SV_ON)
    {
        _CB_PutEvent(CB_SDAL_USERDATA_TRIGGER, sizeof(SDAL_USERDATA_CB_T), &rUserDataWSSBCb);
    }
    return 0;
}

static NPTV_PFN_VPS_NFY _VPS_Cb_driver(UINT8 au1Data[])
{
    SDAL_USERDATA_CB_T rUserDataVPSCb;

    rUserDataVPSCb.u1DataType = 3; /* VPS */
    rUserDataVPSCb.u4NumPktAvail = 1;
    /* Raw VPS data is of 13 bytes */
    x_memcpy(rUserDataVPSCb.au1VPSData, au1Data, 13);
    if(rVPSUDN.u1NotifyOnOff == SV_ON)
    {
        _CB_PutEvent(CB_SDAL_USERDATA_TRIGGER, sizeof(SDAL_USERDATA_CB_T), &rUserDataVPSCb);
    }
    return 0;
}

static SdResult _SdUserData_Init(void)
{
    rCCUDN.u1NotifyOnOff = SV_OFF;
    rCCUDN.u1EvenFieldOn = SV_OFF;
    rCCUDN.u1OddFieldOn = SV_OFF;

    rTTXUDN.u1FieldOn = SV_OFF;
    rTTXUDN.u1NotifyOnOff = SV_OFF;
    rWSSBUDN.u1NotifyOnOff = SV_OFF;
    rVPSUDN.u1NotifyOnOff = SV_OFF;

    NPTV_VBI_SetCcNfy((NPTV_PFN_CC_NFY)_CC_Cb_driver);
    NPTV_VBI_SetTtxNfy((NPTV_PFN_TTX_NFY)_TTX_Cb_driver);
    NPTV_VBI_SetWssNfy((NPTV_PFN_WSS_NFY)_WSSB_Cb_driver);
    NPTV_VBI_SetVpsNfy((NPTV_PFN_VPS_NFY)_VPS_Cb_driver);
    
#if USERDATA_DBG_MSG
    printk("In kernel _UserData_Init\n");
#endif

    return SD_OK;
}

static SdResult _SdUserData_EnableVbiLine(SdBackEndInID_k eInput, uint16_t Startline, uint16_t Endline, SdMask_t AnalogDataTypes)
{
#if USERDATA_DBG_MSG
    printk("Input Src: %d\n", eInput);
    printk("StartLine: %d, End line: %d\n", Startline, Endline);
    printk("Data Type: %d\n", AnalogDataTypes);
#endif

    if((eInput>=SD_BE_IN_EXTIN0) && (eInput<=SD_BE_IN_EXTIN1))
    {
        /* Only from external source supprted */
        if((sdal_extin[eInput - SD_BE_IN_EXTIN0].vpath_assigned) &&
            (sdal_extin[eInput - SD_BE_IN_EXTIN0].video_mod->out_id ==SD_WINDOW_MAIN))
        {
            /* Check if external is connected to front end decoder && in main path */
            if(AnalogDataTypes == SD_USER_DATA_CAPTION)
            {
                VBI_StartNotifyCCOnOff(SV_VP_MAIN, SV_ON, NULL);

                /* Only single line selection is allowed */
                if((Startline == 21) && (Endline == 21))
                {
                    rCCUDN.u1EvenFieldOn = SV_ON;
                }
                else if((Startline == 284) && (Endline == 284))
                {
                    rCCUDN.u1OddFieldOn = SV_ON;
                }
                else
                {
                    /* line selection is wrong for Closed Caption */
                    printk("Line selection is wrong for Closed Caption, should be 21/284\n");
                    return SD_NOT_OK;
                }
            }
            else if((AnalogDataTypes>=SD_USER_DATA_TTX) && (AnalogDataTypes<=SD_USER_DATA_VPS))
            {
                /* EU UserData in main path and CVBS only */
                if(_rMChannel.bDecType == (UINT8)SV_VD_TVD3D)
                {
                    if(AnalogDataTypes == SD_USER_DATA_TTX)
                    {
                        if(((Startline == 6) && (Endline == 22)) ||
                           ((Startline == 318) && (Endline == 335)))
                        {
                            rTTXUDN.u1NotifyOnOff = SV_ON;
                        }
                        else
                        {
                            /* line selection is wrong for Teletext */
                            printk("Line selection is wrong for Teletext, should be 6-22/318-336\n");
                            return SD_NOT_OK;
                        }
                        VBI_StartNotifyTTXOnOff(SV_VP_MAIN, SV_ON, NULL);
                    }
                    else if(AnalogDataTypes == SD_USER_DATA_WSS)
                    {
                        if((Startline == 23) && (Endline == 23))
                        {
                            rWSSBUDN.u1NotifyOnOff = SV_ON;
                        }
                        else
                        {
                            /* line selection is wrong for WSS */
                            printk("Line selection is wrong for WSS, should be 23\n");
                            return SD_NOT_OK;
                        }
                        VBI_StartNotifyWSSOnOff(SV_VP_MAIN, SV_ON, NULL);
                    }
                    else if(AnalogDataTypes == SD_USER_DATA_VPS)
                    {
                        if((Startline == 16) && (Endline == 16))
                        {
                            rVPSUDN.u1NotifyOnOff = SV_ON;
                        }
                        else
                        {
                            /* line selection is wrong for VPS */
                            printk("Line selection is wrong for VPS, should be 16\n");
                            return SD_NOT_OK;
                        }
                        VBI_StartNotifyVPSOnOff(SV_ON, NULL);
                    }
                }
                else
                {
                #if USERDATA_DBG_MSG
                    printk("EU UserData in CVBS source only\n");
                #endif
                    return SD_NOT_OK;
                }
            }
            else
            {
                /* Closed Caption considered now */
                printk("Please Enable Supported UserData: \n");
                printk("SD_USER_DATA_CAPTION\n");
                printk("SD_USER_DATA_TTX\n");
                printk("SD_USER_DATA_WSS\n");
                printk("SD_USER_DATA_VPS\n");
                return SD_NOT_OK;
            }
        }
        else
        {
            /* Not Analog source */
            printk("Not Analog source or not in main window\n");
            return SD_NOT_OK;
        }
    }
#if USERDATA_DBG_MSG
    printk("In kernel _UserData_EnableVbiLine\n");
#endif
    return SD_OK;
}

static SdResult _SdUserData_DisableVbiLine(SdBackEndInID_k eInput, uint16_t Startline, uint16_t Endline)
{
#if USERDATA_DBG_MSG
    printk("Input Src: %d\n", eInput);
    printk("StartLine: %d, End line: %d\n", Startline, Endline);
#endif

    /* Consider only main path is available case */
    if((eInput>=SD_BE_IN_EXTIN0) && (eInput<=SD_BE_IN_EXTIN1))
    {
        /* Only from external source supprted */
        if((sdal_extin[eInput - SD_BE_IN_EXTIN0].vpath_assigned) &&
            (sdal_extin[eInput - SD_BE_IN_EXTIN0].video_mod->out_id ==SD_WINDOW_MAIN))
        {
            /* Check if external is connected to front end decoder && in main path */            
            if((Startline == 21) && (Endline == 21))
            {
                rCCUDN.u1EvenFieldOn = SV_OFF;
            }
            else if((Startline == 284) && (Endline == 284))
            {
                rCCUDN.u1OddFieldOn = SV_OFF;
            }
            
            /* EU UserData in main path and CVBS only */
            else if(_rMChannel.bDecType == (UINT8)SV_VD_TVD3D)
            {
                if(((Startline == 6) && (Endline == 22)) ||
                   ((Startline == 318) && (Endline == 335)))
                {
                    rTTXUDN.u1NotifyOnOff = SV_OFF;
                    VBI_StartNotifyTTXOnOff(SV_VP_MAIN, SV_OFF, NULL);
                }
                else if((Startline == 23) && (Endline == 23))
                {
                    rWSSBUDN.u1NotifyOnOff = SV_OFF;
                    VBI_StartNotifyWSSOnOff(SV_VP_MAIN, SV_OFF, NULL);
                }
                else if((Startline == 16) && (Endline == 16))
                {
                    rVPSUDN.u1NotifyOnOff = SV_OFF;
                    VBI_StartNotifyVPSOnOff(SV_OFF, NULL);
                }
                else
                {
                    /* line selection is wrong for EU userdata */
                    printk("Line selection is wrong for userdata\n");
                    return SD_NOT_OK;
                }
            }
            else
            {
                /* line selection is wrong for Closed Caption */
                printk("Line selection is wrong for userdata\n");
                return SD_NOT_OK;
            }
        }
        else
        {
            /* Not Analog source */
            printk("Not Analog source or not in main window\n");
            return SD_NOT_OK;
        }

    }
#if USERDATA_DBG_MSG
    printk("In kernel _UserData_DisableVbiLine\n");
#endif

    return SD_OK;
}

static SdResult _SdUserData_Start(SdBackEndInID_k eInput)
{
    /* Consider only main path is available case */
    if((eInput>=SD_BE_IN_EXTIN0) && (eInput<=SD_BE_IN_EXTIN1))
    {
        /* Only from external source supprted */
        if((sdal_extin[eInput - SD_BE_IN_EXTIN0].vpath_assigned) &&
            (sdal_extin[eInput - SD_BE_IN_EXTIN0].video_mod->out_id ==SD_WINDOW_MAIN))
        /* Check if external is connected to front end decoder && in main path */
        {
            rCCUDN.u1NotifyOnOff = SV_ON;
            if(GetTargetCountry() == COUNTRY_US)
            {
                _u1VChipOn = SV_ON;
            }
        }
    }
    else if (eInput == SD_BE_IN_AVDEC0)
    {
        MPV_StartUserData(SD_BE_IN_AVDEC0);
    }
    else
    {
        return SD_NOT_SUPPORTED;
    }
    
#if USERDATA_DBG_MSG    
    printk("Start source %d \n", eInput);
#endif
    return SD_OK;
}

static SdResult _SdUserData_Stop(SdBackEndInID_k eInput)
{
    /* Consider only main path is available case */
    if((eInput>=SD_BE_IN_EXTIN0) && (eInput<=SD_BE_IN_EXTIN1))
    {
        /* Only from external source supprted */
        if((sdal_extin[eInput - SD_BE_IN_EXTIN0].vpath_assigned) &&
           (sdal_extin[eInput - SD_BE_IN_EXTIN0].video_mod->out_id ==SD_WINDOW_MAIN))
        /* Check if external is connected to front end decoder && in main path */
        {
            rCCUDN.u1NotifyOnOff = SV_OFF;
            if(GetTargetCountry() == COUNTRY_US)
            {
                _u1VChipOn = SV_OFF;
            }
        }
    }
    else if (eInput == SD_BE_IN_AVDEC0)
    {
        MPV_StopUserData(SD_BE_IN_AVDEC0);
    }
    else
    {
        return SD_NOT_SUPPORTED;
    }
    
#if USERDATA_DBG_MSG
    printk("Stop source %d \n", eInput);
#endif
    return SD_OK;
}


static SdResult _SdUserData_EnableAfd(SdBackEndInID_k eInput)
{
    if (eInput != SD_BE_IN_AVDEC0)
    {
        // Only SD_BE_IN_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

    MPV_EnableAfd(eInput);
    
#if USERDATA_DBG_MSG
    printk("Enable AFD %d \n", eInput);
#endif
    return SD_OK;
}


static SdResult _SdUserData_DisableAfd(SdBackEndInID_k eInput)
{
    if (eInput != SD_BE_IN_AVDEC0)
    {
        // Only SD_BE_IN_AVDEC0 is supported
        return SD_NOT_SUPPORTED;
    }

    MPV_DisableAfd(eInput);

#if USERDATA_DBG_MSG
    printk("Disable AFD %d \n", eInput);
#endif
    return SD_OK;

}


#if 0 /* SDAL 3.4.7 renamed as Enable/Disable digital data */
static SdResult _SdUserData_Get(SdBackEndInID_k eInput, SdUserData_Settings_t* sSettings)
{
    if((eInput>=SD_BE_IN_EXTIN0) && (eInput<=SD_BE_IN_EXTIN1))
    {
        /* Only from external source supprted */
        if((sdal_extin[eInput - SD_BE_IN_EXTIN0].vpath_assigned) &&
           (sdal_extin[eInput - SD_BE_IN_EXTIN0].video_mod->out_id ==SD_WINDOW_MAIN))
        /* Check if external is connected to front end decoder && in main path */
        {
            if (copy_to_user((void __user *)sSettings, &rFilterSetting, sizeof(SdUserData_Settings_t)))
            {
                printk("_SdUserData_Get: copy_to_user fail!!\n");
                return -1;
            }
        }
    }
    else if (eInput < SD_BE_IN_EXTIN0)
    {
        MPV_GetSdUserDataFilter(eInput, &(rFilterSetting.bFilterOn), &(rFilterSetting.DigitalFilter));
        
        if (copy_to_user((void __user *)sSettings, &rFilterSetting, sizeof(SdUserData_Settings_t)))
        {
            printk("_SdUserData_Get: copy_to_user fail!!\n");
            return -1;
        }    
    }
    
    return SD_OK;
}

static SdResult _SdUserData_Set(SdBackEndInID_k eInput, SdUserData_Settings_t* sSettings)
{
    printk("_SdUserData_Set\n");
    if((eInput>=SD_BE_IN_EXTIN0) && (eInput<=SD_BE_IN_EXTIN1))
    {
        /* Only from external source supprted */
        if((sdal_extin[eInput - SD_BE_IN_EXTIN0].vpath_assigned) &&
           (sdal_extin[eInput - SD_BE_IN_EXTIN0].video_mod->out_id ==SD_WINDOW_MAIN))
        /* Check if external is connected to front end decoder && in main path */
        {
            // Copy user space argument
            if (copy_from_user(&rFilterSetting, (void __user *)sSettings, sizeof(SdUserData_Settings_t)))
            {
                printk("_SdUserData_CopyData argument error\n");
                return -1;
            }
        }
    }
    else if (eInput < SD_BE_IN_EXTIN0)
    {
        // Copy user space argument
        if (copy_from_user(&rFilterSetting, (void __user *)sSettings, sizeof(SdUserData_Settings_t)))
        {
            printk("_SdUserData_CopyData argument error\n");
            return -1;
        }

        MPV_SetSdUserDataFilter(eInput, rFilterSetting.bFilterOn, rFilterSetting.DigitalFilter);
    }
    

    printk("_SdUserData_Set: bFilterOn = %d\n", rFilterSetting.bFilterOn);
    printk("_SdUserData_Set: DigitalFilter = %d\n", rFilterSetting.DigitalFilter);
    printk("_SdUserData_Set: AnalogFilter = %d\n", rFilterSetting.AnalogFilter);
    return SD_OK;
}
#endif

static SdResult _SdUserData_Debug(void)
{
    return SD_OK;
}


static INT32 _SdUserData_CopyData(UINT32 u4Arg)
{
    SDAL_IOCTL_3ARG_T rArg; 
    UINT32 u4Rptr;
    UINT32 u4Len;    
    UINT8 *pu1Buf;
            
    if (!access_ok(VERIFY_READ, (void __user *)u4Arg,
                      sizeof(SDAL_IOCTL_3ARG_T)))

    {
        printk("_SdUserData_CopyData argument error\n");
        return -1;
    }

    // Copy user space argument
    if (copy_from_user(&rArg, (void __user *)u4Arg,
                       sizeof(SDAL_IOCTL_3ARG_T)))
    {
        printk("_SdUserData_CopyData argument error\n");
        return -1;
    }

    pu1Buf = (UINT8*)rArg.au4Arg[0];
    u4Len = rArg.au4Arg[1];
    u4Rptr = rArg.au4Arg[2];

    ASSERT(pu1Buf);
    ASSERT(u4Len);

    if (!MPV_GetUserData(_au1UserDataTmpBuf, u4Rptr, u4Len))
    {
        return -1;
    }

    // Copy to user buffer
    if (copy_to_user((void __user *)pu1Buf, _au1UserDataTmpBuf, u4Len))
    {
        return -1;
    }

    return SD_OK;

}

static INT32 _SdUserData_CopyData_TTX(UINT32 u4Arg)
{
    SDAL_IOCTL_4ARG_T rArg; 
    UINT32 u4Len;
    UINT8 *pu1Buf;
    UINT8 *pu1UserData;
            
    if (!access_ok(VERIFY_READ, (void __user *)u4Arg,
                      sizeof(SDAL_IOCTL_4ARG_T)))

    {
        printk("_SdUserData_CopyData_TTX argument error\n");
        return -1;
    }

    // Copy user space argument
    if (copy_from_user(&rArg, (void __user *)u4Arg,
                       sizeof(SDAL_IOCTL_4ARG_T)))
    {
        printk("_SdUserData_CopyData argument error\n");
        return -1;
    }

    pu1UserData = (UINT8*)rArg.au4Arg[0];
    u4Len = rArg.au4Arg[1];
    pu1Buf = (UINT8*)rArg.au4Arg[2];

    pu1UserData = (UINT8*)VIRTUAL((UINT32)pu1UserData);
    
    // Copy to user buffer
    if (copy_to_user((void __user *)pu1Buf, pu1UserData, u4Len))
    {
        return -1;
    }
    
    return SD_OK;

}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int sdal_ioctl_sduserdata(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    SDAL_IOCTL_4ARG_T rArg12;    //current use SDAL_IOCTL_4ARG_T instead of SDAL_IOCTL_12ARG_T

    if (_IOC_SIZE(cmd)>sizeof(unsigned long))
    {    
        // Check access of user space
        if (!access_ok(VERIFY_READ, (void __user *)arg,  _IOC_SIZE(cmd) ))
        {
            printk("sdal_ioctl_sduserdata arg access error\n");
            return SD_ERR_SYS;
        }
        // Copy user space argument
        if(copy_from_user(&rArg12, (void __user *)arg,
               _IOC_SIZE(cmd)))
        {
            printk("sdal_ioctl_sduserdata copy_from_user error\n");
            return SD_ERR_SYS;
        }
    }

    switch (cmd)
    {
        case SDAL_IO_USERDATA_INSTALLCALLBACK:
        #if 1
            return _SdUserData_InstallCallback((SdUserData_CallBack) rArg12.au4Arg[0]
                ,(uint32_t) rArg12.au4Arg[1]);
        #endif
            break;
            
        case SDAL_IO_USERDATA_INIT:
            _SdUserData_Init();
            break;
            
        case SDAL_IO_USERDATA_ENABLEVBILINE:
            return _SdUserData_EnableVbiLine((SdBackEndInID_k) rArg12.au4Arg[0]
                , (uint16_t) rArg12.au4Arg[1]
                , (uint16_t) rArg12.au4Arg[2]
                , (SdMask_t) rArg12.au4Arg[3]);
            break;
            
        case SDAL_IO_USERDATA_DISABLEVBILINE:
            return _SdUserData_DisableVbiLine((SdBackEndInID_k) rArg12.au4Arg[0]
                , (uint16_t) rArg12.au4Arg[1]
                , (uint16_t) rArg12.au4Arg[2]);
            break;
            
        case SDAL_IO_USERDATA_START:
            return _SdUserData_Start((SdBackEndInID_k) arg);
            break;
            
        case SDAL_IO_USERDATA_STOP:
            return _SdUserData_Stop((SdBackEndInID_k) arg);
            break;
            
        case SDAL_IO_USERDATA_ENABLEAFD:
            return _SdUserData_EnableAfd((SdBackEndInID_k) arg);
            break;
            
        case SDAL_IO_USERDATA_DISABLEAFD:
            return _SdUserData_DisableAfd((SdBackEndInID_k) arg);
            break;

        case SDAL_IO_USERDATA_DEBUG:
            return _SdUserData_Debug();
            break;

        case SDAL_IO_USERDATA_COPYDATA:
            return _SdUserData_CopyData(arg);
            break;
            
        case SDAL_IO_USERDATA_COPYDATA_TTX:
            return _SdUserData_CopyData_TTX(arg);
            break;
#if 0 /* SDAL 3.4.7 removed*/
        case SDAL_IO_USERDATA_GET:    
        	return _SdUserData_Get((SdBackEndInID_k)rArg12.au4Arg[0]
        		, (SdUserData_Settings_t*)rArg12.au4Arg[1]);
        	break;
        case SDAL_IO_USERDATA_SET:    
        	return _SdUserData_Set((SdBackEndInID_k)rArg12.au4Arg[0]
        		, (SdUserData_Settings_t*)rArg12.au4Arg[1]);
        	break;
#endif 	
        	
        default:
            printk("SdUserData: unknown IO command %u\n", cmd);
            return SD_NOT_SUPPORTED;
    }

    return SD_OK;
}

