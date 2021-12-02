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
/*-----------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $RCSfile: io_sdtuner_eu.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------*/

/** @file io_sdtuner.c
 *  SDAL ioctl interface of MT538X tuner driver.
 */

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/

#include "x_linux.h"
#include <linux/sdal_ioctl.h>

//#include "PI_Def.h"
//#include "drvapi_tuner.h"
//#include "TDIS.h"
#include "tuner_if.h"
#include "x_tuner.h"
#include "u_tuner.h"
#include "io_sdtuner.h"
#include "sd_audio.h"
#include "hw_vdoin.h"
#include "drv_tvd.h"

#define DEFINE_IS_LOG       NIM_IsLog
#include "x_debug.h"

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
//-----------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------
#define cTUNER_DEFAULT_FREQ     533000      // kHz
/***********************************************************************/
#define fcTIMING_MEASURE        0

//-----------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------
//#define mcPRINTF(fmt...)        printk(fmt)
//#define mcPRINT_DBG(fmt...)     mcPRINTF(fmt)
#define mcPRINT_DBG(fmt...)     LOG_I(fmt)  // print level
#define mcPRINT_ERR(fmt...)     LOG_E(fmt)  // print level

/***********************************************************************/
/*              Static variables                                       */
/***********************************************************************/
static BOOL fgTunerInited = FALSE;
static BOOL afgBreakFlag[SD_TUNER_MAX];
static TUNER_ALL_INFO_T tConnInfo;

/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
/***********************************************************************/
/*              Static functions                                       */
/***********************************************************************/
static INT32 i4CheckBreak(void* pvArg)
{
SdTunerID_k *peTuner = (SdTunerID_k *) pvArg;

    return afgBreakFlag[*peTuner];
}

static UINT16 u2GetTunerIdDig(SdTunerID_k eTuner)
{
    if (eTuner == SD_TUNER0)
        return MTK_DVBT_0;
//    else if (eTuner == SD_TUNER1)
//        return MTK_DTD_1;
    return MTK_DVBT_0;
}

static UINT16 u2GetTunerIdAna(SdTunerID_k eTuner)
{
    if (eTuner == SD_TUNER0)
        return MTK_ANALOG_0;
//    else if (eTuner == SD_TUNER1)
//        return MTK_ANALOG_1;
    return MTK_ANALOG_0;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
INT32 _Tuner_Init_EU(SdTunerID_k eTuner, SdPowerState_k ePowerState)
{
UINT8 ii;
#if fcTIMING_MEASURE
TIME_TICK_T u4TickStart, u4TickCur;
TIME_DIFF_T u4TickCnt;
TIME_CONV_T u4TickCnt0;
#endif

    mcPRINT_DBG("In kernel %s: %d, %d\n", __FUNCTION__, eTuner, ePowerState);

#if fcTIMING_MEASURE
    mcGET_SYS_TIME(u4TickStart);
#endif

    fgTunerInited = FALSE;
    if (TunerClose() != DRVAPI_TUNER_OK)
    {
        if (fgTunerInited)
            mcPRINT_ERR("Tuner %d driver API shutdown failed!\n", eTuner);
        //return SD_ERR_RESOURCE;
    }

    for (ii = 0; ii < SD_TUNER_MAX; ii ++)
        afgBreakFlag[ii] = FALSE;
    if (TunerOpen() != DRVAPI_TUNER_OK)
        return SD_ERR_RESOURCE;
    fgTunerInited = TRUE;

#if fcTIMING_MEASURE
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
    u4TickCnt0 = mcCONV_SYS_TIME(u4TickCnt);
    mcSHOW_DBG_MSG((" %s (%3u ms) ", __FUNCTION__, u4TickCnt0));
#endif

    return SD_OK;
}

INT32 _Tuner_Tune_EU(SdTunerID_k eTuner, SdTuner_Parameter_t *pTunerParam, UINT32 u4Frequency)
{
    UINT16 u2TunerId;
    //TUNER_ALL_INFO_T tConnInfo;   // move to global scope for _Tuner_StartChannel_EU (2nd TunerAcq)
    DRV_CONN_TYPE_T eConntype;
    TUNER_ACQ_ACTION_T  eActionType;
    UINT32 u4TmpFreq;
    SdTuner_Parameter_t sTunerParam;
#if 0
    UINT8   ii;
    UINT32  *pData;

        pData = (UINT32 *) pTunerParam;
        mcPRINT_DBG("DumpKernel:");
        for (ii = 0; ii < 8; ii ++)
            mcPRINT_DBG(" %08X (%d)", pData[ii]);
        mcPRINT_DBG("\n");
#endif

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *) pTunerParam, sizeof(SdTuner_Parameter_t)))
    {
        mcPRINT_ERR("%s arg access error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }
    // Copy user space argument
    if (copy_from_user(&sTunerParam, (void __user *) pTunerParam, sizeof(SdTuner_Parameter_t)))
    {
        mcPRINT_ERR("%s copy_from_user error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    mcPRINT_DBG("In kernel %s: %d, %d; %d, %d, %d, %d", __FUNCTION__,
                eTuner, sTunerParam.eModFormat, sTunerParam.eTuneMode,
                sTunerParam.eRfMode, sTunerParam.channel, u4Frequency);

    mcPRINT_DBG(" (%d)\n", *((UINT32 *) &(sTunerParam.frequency)));
    //u4TmpFreq = *((UINT32 *) &(sTunerParam.frequency));
    //mcPRINT_DBG(" (%d)\n", u4TmpFreq);

    if (!fgTunerInited)
        if (_Tuner_Init_EU(eTuner, SD_PWROFF_TO_NORMAL) != SD_OK)
            return SD_ERR_UNINIT;

    // channel number not support so far [Justin]
    if (sTunerParam.eTuneMode != SD_TUNE_FREQUENCY)
    {
        mcPRINT_ERR("Invalid eTuneMode: %d\n", sTunerParam.eTuneMode);
        return SD_ERR_INV;
    }
    u4TmpFreq = u4Frequency + sTunerParam.iOffset; //kHz

    u2TunerId = u2GetTunerIdDig(eTuner);
    
    // Justin TODO
    if (sTunerParam.eModFormat == SD_MOD_OFDM)
    {
        TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info;

        eConntype = TUNER_CONN_TYPE_TER_DIG;

        pt_tuner_info = (TUNER_TER_DIG_TUNE_INFO_T *) (&tConnInfo);
        // set frequency
        pt_tuner_info->ui4_freq = u4TmpFreq * 1000;
        // set bandwidth
        switch (sTunerParam.bandwidth) {
        case 6:  pt_tuner_info->e_bandwidth = BW_6_MHz; break;
        case 7:  pt_tuner_info->e_bandwidth = BW_7_MHz; break;
        case 8:  pt_tuner_info->e_bandwidth = BW_8_MHz; break;
        default: pt_tuner_info->e_bandwidth = BW_8_MHz; break;
        }
        // set hierarchy
        if (sTunerParam.eHP == SD_H_PRIORITY_LOW){
            pt_tuner_info->e_hierarchy_priority = HIR_PRIORITY_LOW;
        }
        else{
            pt_tuner_info->e_hierarchy_priority = HIR_PRIORITY_HIGH;
        }
    }
    else if (sTunerParam.eModFormat == SD_MOD_PAL)
    {
        TUNER_TER_ANA_TUNE_INFO_T *pt_tuner_info;

        u2TunerId = u2GetTunerIdAna(eTuner);
        eConntype = TUNER_CONN_TYPE_TER_ANA;

        pt_tuner_info = (TUNER_TER_ANA_TUNE_INFO_T *) (&tConnInfo);
        pt_tuner_info->ui4_freq = u4TmpFreq * 1000;  
        pt_tuner_info->ui4_freq_bound = pt_tuner_info->ui4_freq;
        pt_tuner_info->e_mod = MOD_PSK_8;     // MOD_PSK_8 --> Channel Change; _fgAutoSearch = FALSE. Please refer to GetAnaConnMode()
        switch (sTunerParam.eSoundSys){
        case SD_TUNER_SOUND_SYS_BG:
            pt_tuner_info->ui4_tv_sys_mask = (TV_SYS_MASK_B | TV_SYS_MASK_G);
            break;
        case SD_TUNER_SOUND_SYS_DK:
            pt_tuner_info->ui4_tv_sys_mask = (TV_SYS_MASK_D | TV_SYS_MASK_K);
            break;
        case SD_TUNER_SOUND_SYS_I:
            pt_tuner_info->ui4_tv_sys_mask = (TV_SYS_MASK_I);
            break;
        case SD_TUNER_SOUND_SYS_L:
            pt_tuner_info->ui4_tv_sys_mask = (TV_SYS_MASK_L);
            break;
        default:
            pt_tuner_info->ui4_tv_sys_mask = TV_SYS_MASK_NONE;
            break;
        }

    }
    else
        return SD_ERR_INV;

    if (sTunerParam.eModFormat != SD_MOD_PAL)
    {                                       /* acq digital, disconnect analog */
        TunerDisc(MTK_ANALOG_0, TUNER_CONN_TYPE_TER_ANA);
    }
    else
    {                                       /* acq analog, disconnect digital */
        TunerDisc(MTK_DVBT_0, TUNER_CONN_TYPE_TER_DIG);
    }

    //if (sTunerParam.eModFormat != SD_MOD_NTSC)
    {        
        eActionType = TUNE_FREQ_ONLY; // tune tuner only
        TunerSetAttribute(u2TunerId, TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS,
                          (void *) &eActionType, sizeof(TUNER_ACQ_ACTION_T));
    }
   
    TunerAcq(u2TunerId, eConntype, &tConnInfo, sizeof(TUNER_ALL_INFO_T),
             i4CheckBreak, &eTuner);

    return SD_OK;
}

INT32 _Tuner_StartChannel_EU(SdTunerID_k eTuner, SdTuner_ModFormat_k eMode)
{
    UINT16 u2TunerId;
    //TUNER_ALL_INFO_T tConnInfo;   // move to global scope for _Tuner_StartChannel_EU (2nd TunerAcq)
    DRV_CONN_TYPE_T eConntype;
    TUNER_ACQ_ACTION_T  eActionType;

    mcPRINT_DBG("In kernel %s: %d, %d\n", __FUNCTION__, eTuner, eMode);

    if (!fgTunerInited)
        if (_Tuner_Init_EU(eTuner, SD_PWROFF_TO_NORMAL) != SD_OK)
            return SD_ERR_UNINIT;

    // Justin TODO
    u2TunerId = u2GetTunerIdDig(eTuner);
    if (eMode == SD_MOD_OFDM)
    {
        TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info;

        eConntype = TUNER_CONN_TYPE_TER_DIG;
        pt_tuner_info = (TUNER_TER_DIG_TUNE_INFO_T *) (&tConnInfo);
        pt_tuner_info->ui4_freq = cTUNER_DEFAULT_FREQ * 1000;   // Valid range, ignore value
        // Justin TODO, we need BW & Hier parameters
        //pt_tuner_info->e_bandwidth = BW_8_MHz;
        //pt_tuner_info->e_hierarchy_priority == HIR_PRIORITY_HIGH;
    }
    else if (eMode == SD_MOD_PAL)
    {
        // _SD_AUD_AtvPlay(0);              // --> move to _sdExtIn_Start
#if 0
    TUNER_CAB_ANA_TUNE_INFO_T *pt_tuner_info;

        u2TunerId = u2GetTunerIdAna(eTuner);
        eConntype = TUNER_CONN_TYPE_CAB_ANA;
        pt_tuner_info = (TUNER_CAB_ANA_TUNE_INFO_T *) (&tConnInfo);
        pt_tuner_info->ui4_freq = cTUNER_DEFAULT_FREQ * 1000;   // Valid range, ignore value
#else
    /* Connect analog --> disconnect digital first */
        TunerDisc(MTK_DTD_0, TUNER_CONN_TYPE_TER_DIG);

        return SD_OK;
#endif
    }
    else
        return SD_ERR_INV;

    eActionType = ACQ_DEMOD_ONLY;  // set demod only
    TunerSetAttribute(u2TunerId, TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS,
                      (void *) &eActionType, sizeof(TUNER_ACQ_ACTION_T));

    TunerAcq(u2TunerId, eConntype, &tConnInfo, sizeof(TUNER_ALL_INFO_T),
             i4CheckBreak, &eTuner);

    return SD_OK;
}

INT32 _Tuner_Get_EU(SdTunerID_k eTuner, SdTuner_Settings_t *pSettings)
{
    return SD_OK;
}

INT32 _Tuner_Set_EU(SdTunerID_k eTuner, SdTuner_Settings_t *pSettings)
{
    return SD_OK;
}

INT32 _Tuner_SplitterGet_EU(SdTuner_SplitterSettings_t *pSettings)
{
    return SD_OK;
}

INT32 _Tuner_SplitterSet_EU(SdTuner_SplitterSettings_t *pSettings)
{
    return SD_OK;
}

INT32 _Tuner_GetStatus_EU(SdTunerID_k eTuner, SdTuner_ModFormat_k eMode,
                              SdMask_t mask, SdTuner_Status_t *pState, uint32_t timeout, UINT32 *pu4Snr)
{
    UINT16 u2TunerId;
    DRV_CONN_TYPE_T eConntype;
    SIGNAL sSignal;
    SdTuner_Status_t sTunerState;
    UINT32  u4Snr;
    INT32   i4RetSts = SD_OK;

    mcPRINT_DBG("In kernel %s: %d, %d, %04X, %d\n", __FUNCTION__,
                eTuner, eMode, mask, timeout);

    sTunerState.eLnaSensitive = SD_LNA_NOT_CHECK;
    sTunerState.bLock = 0;
    sTunerState.signalStrength = 0;
    sTunerState.snr = 0;
    sTunerState.eAutoQAM = SD_MOD_UNKNOWN;
    u4Snr = 0;

    if (!fgTunerInited)
        if (_Tuner_Init_EU(eTuner, SD_PWROFF_TO_NORMAL) != SD_OK)
        {
            mcPRINT_ERR("In kernel %s return: SD_ERR_UNINIT\n", __FUNCTION__);
        //    return SD_ERR_UNINIT;
            i4RetSts = SD_ERR_UNINIT;
            goto lbExit;
        }

    // Justin TODO
    u2TunerId = u2GetTunerIdDig(eTuner);
    if (eMode == SD_MOD_OFDM)
    {
        eConntype = TUNER_CONN_TYPE_TER_DIG;

        sTunerState.bLock = TunerGetSync(u2TunerId, eConntype);
        sTunerState.signalStrength = TunerGetSignalLevel(u2TunerId, eConntype) / 10;
        sTunerState.snr = TunerGetSignalSNR(u2TunerId, eConntype);  // snr*10 => CC_SDAL in pd_glue_dvbt.c
        u4Snr = sTunerState.snr;
        
        TunerGetSignal(u2TunerId, eConntype, &sSignal);
        if (sSignal.sHierInfo.eInDepth) {
            switch(sSignal.sHierInfo.eAlpha)
            {
            case ALPHA_0:  sTunerState.eHI = SD_H_IN_DEPTH_ALPHA_NONE;  break;
            case ALPHA_1:  sTunerState.eHI = SD_H_IN_DEPTH_ALPHA_1;     break;
            case ALPHA_2:  sTunerState.eHI = SD_H_IN_DEPTH_ALPHA_2;     break;
            case ALPHA_4:  sTunerState.eHI = SD_H_IN_DEPTH_ALPHA_4;     break;
            default:       sTunerState.eHI = SD_H_IN_DEPTH_ALPHA_NONE;  break;
            }
        }
        else{
            switch(sSignal.sHierInfo.eAlpha)
            {
            case ALPHA_0:  sTunerState.eHI = SD_H_NATIVE_ALPHA_NONE;  break;
            case ALPHA_1:  sTunerState.eHI = SD_H_NATIVE_ALPHA_1;     break;
            case ALPHA_2:  sTunerState.eHI = SD_H_NATIVE_ALPHA_2;     break;
            case ALPHA_4:  sTunerState.eHI = SD_H_NATIVE_ALPHA_4;     break;
            default:       sTunerState.eHI = SD_H_NATIVE_ALPHA_NONE;  break;
            }
        }        
    }
    else if (eMode == SD_MOD_PAL)
    {
        u2TunerId = u2GetTunerIdAna(eTuner);
        eConntype = TUNER_CONN_TYPE_CAB_ANA;

        sTunerState.bLock = fgHwTvdVPres();
        sTunerState.signalStrength = 0;
        
        if (sTunerState.bLock)
        {
            sTunerState.signalStrength = 10;
            sTunerState.snr = 300;
            u4Snr = sTunerState.snr;
        }
    }
    else
    {
        mcPRINT_ERR("In kernel %s return: SD_ERR_INV\n", __FUNCTION__);
    //    return SD_ERR_INV;
        i4RetSts = SD_ERR_INV;
        goto lbExit;
    }

lbExit:
//    sTunerState.snr = u4Snr;
    mcPRINT_DBG("In kernel %s return: %d, %d, %d, %d\n", __FUNCTION__,
                sTunerState.bLock, sTunerState.snr,
                sTunerState.signalStrength, sTunerState.eHI);

// Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *) pState,
                   sizeof(SdTuner_Status_t)))
    {
        mcPRINT_ERR("%s arg access error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    if (copy_to_user((void __user *) pState, &sTunerState,
                     sizeof(SdTuner_Status_t)))
    {
        mcPRINT_ERR("%s copy_to_user error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

// Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *) pu4Snr,
                   sizeof(UINT32)))
    {
        mcPRINT_ERR("%s arg access error2\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    if (copy_to_user((void __user *) pu4Snr, &u4Snr,
                     sizeof(UINT32)))
    {
        mcPRINT_ERR("%s copy_to_user error2\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    return i4RetSts;
}

INT32 _Tuner_GetAFTVol_EU(UINT8 *pu1AFT_Vol)
{
    return SD_OK;
}

