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
 * $RCSfile: io_sdtuner_na.c,v $
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
#define AIR_CH_NS               69          // 1 ~ 69
#define CABLE_CH_NS             159         // 1 ~ 159
#define MIN_CH_FREQ             50
#define MAX_CH_FREQ             1050
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
#define mcPRINT_DBG(fmt...)     LOG_I(fmt)
#define mcPRINT_ERR(fmt...)     LOG_E(fmt)

/***********************************************************************/
/*              Static variables                                       */
/***********************************************************************/
static BOOL fgTunerInited = FALSE;
static BOOL afgBreakFlag[SD_TUNER_MAX];

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
        return MTK_DTD_0;
//    else if (eTuner == SD_TUNER1)
//        return MTK_DTD_1;
    return MTK_DTD_0;
}

static UINT16 u2GetTunerIdAna(SdTunerID_k eTuner)
{
    if (eTuner == SD_TUNER0)
        return MTK_ANALOG_0;
//    else if (eTuner == SD_TUNER1)
//        return MTK_ANALOG_1;
    return MTK_ANALOG_0;
}

#define cCHAN_BAND_NUM          (sizeof(au2ChanBand) / sizeof(*au2ChanBand))
#define cCHAN_BAND_NUM_CABLE    (sizeof(au2ChanBandCable) / sizeof(*au2ChanBandCable))
#define cCHAN_BAND_WID          6           // 6 MHz

static UINT16 au2ChanBand[] = {51, 79, 177, 473};
static UINT16 au2ChanNum[] = {4, 2, 7, 67};

static UINT16 au2ChanBandCable[] = {75, 57, 79, 93, 123, 177, 219, 651};
static UINT16 au2ChanNumBegin[] = {1, 2, 5, 95, 14, 7, 23, 100};
static UINT16 au2ChanNumEnd[] = {1, 4, 6, 99, 22, 13, 94, 135};

UINT16 u4ConvChan2Freq(UINT16 u2Chan, UINT16 u2ChanMode)
{
UINT16  u2Freq = 0;
UINT16  u2ChanCnt = 0, ii;

    if (u2Chan <= 0)
        u2Chan = 1;
    if (u2ChanMode == SD_RF_AIR)
    {
        if (u2Chan > AIR_CH_NS)
            u2Chan = AIR_CH_NS;
        for (ii = 0; ii < cCHAN_BAND_NUM; ii ++)
        {
            if (u2Chan <= (u2ChanCnt + au2ChanNum[ii]))
                break;
            u2ChanCnt += au2ChanNum[ii];
        }
        u2ChanCnt ++;
        u2Freq += au2ChanBand[ii];
    }
    else
    {
        if (u2Chan > CABLE_CH_NS)
            u2Chan = CABLE_CH_NS;
        for (ii = 0; ii < cCHAN_BAND_NUM_CABLE; ii ++)
            if ((u2Chan >= au2ChanNumBegin[ii]) && (u2Chan <= au2ChanNumEnd[ii]))
                break;

        u2ChanCnt = au2ChanNumBegin[ii];
        u2Freq += au2ChanBandCable[ii];
    }

    u2Freq += (u2Chan - u2ChanCnt) * cCHAN_BAND_WID;
    return u2Freq;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
INT32 _Tuner_Init_NA(SdTunerID_k eTuner, SdPowerState_k ePowerState)
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

//    if (fgTunerInited)
    {
        if (TunerClose() != DRVAPI_TUNER_OK)
        {
            if (fgTunerInited)
                mcPRINT_ERR("Tuner %d driver API shutdown failed!\n", eTuner);
        //    return SD_ERR_RESOURCE;
        }
        fgTunerInited = FALSE;
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

INT32 _Tuner_Tune_NA(SdTunerID_k eTuner, SdTuner_Parameter_t *pTunerParam, UINT32 u4Frequency)
{
UINT16 u2TunerId;
TUNER_ALL_INFO_T tConnInfo;
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

//    u4TmpFreq = (UINT32) (sTunerParam.frequency);
    u4TmpFreq = *((UINT32 *) &(sTunerParam.frequency));
    mcPRINT_DBG(" (%d)\n", u4TmpFreq);

    if (!fgTunerInited)
        if (_Tuner_Init_NA(eTuner, SD_PWROFF_TO_NORMAL) != SD_OK)
            return SD_ERR_UNINIT;

    u4TmpFreq = u4Frequency;
    if (sTunerParam.eTuneMode == SD_TUNE_CHANNEL)
    {
        if ((sTunerParam.eRfMode == SD_RF_AIR) || (sTunerParam.eRfMode == SD_RF_CABLE))
        {
            u4TmpFreq = u4ConvChan2Freq(sTunerParam.channel, sTunerParam.eRfMode) * 1000;
        }
        else
        {
            mcPRINT_ERR("Invalid eRfMode: %d\n", sTunerParam.eRfMode);
            return SD_ERR_INV;
        }
        if (sTunerParam.eModFormat == SD_MOD_NTSC)
            u4TmpFreq -= 1750;
    }
    else if (sTunerParam.eTuneMode != SD_TUNE_FREQUENCY)
    {
        mcPRINT_ERR("Invalid eTuneMode: %d\n", sTunerParam.eTuneMode);
        return SD_ERR_INV;
    }
    u4TmpFreq += sTunerParam.iOffset;

    u2TunerId = u2GetTunerIdDig(eTuner);
    if (sTunerParam.eModFormat == SD_MOD_8VSB)
    {
    TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info;

        eConntype = TUNER_CONN_TYPE_TER_DIG;
        pt_tuner_info = (TUNER_TER_DIG_TUNE_INFO_T *) (&tConnInfo);
        pt_tuner_info->ui4_freq = u4TmpFreq * 1000;
        pt_tuner_info->e_mod = MOD_VSB_8;
    }
    else if ((sTunerParam.eModFormat == SD_MOD_64QAM)
          || (sTunerParam.eModFormat == SD_MOD_256QAM)
          || (sTunerParam.eModFormat == SD_MOD_QAM))
    {
    TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info;

        eConntype = TUNER_CONN_TYPE_CAB_DIG;
        pt_tuner_info = (TUNER_CAB_DIG_TUNE_INFO_T *) (&tConnInfo);
        pt_tuner_info->ui4_freq = u4TmpFreq * 1000;
        pt_tuner_info->e_mod = MOD_QAM_256;
        if (sTunerParam.eModFormat == SD_MOD_64QAM)
            pt_tuner_info->e_mod = MOD_QAM_64;
        else if (sTunerParam.eModFormat == SD_MOD_QAM)
            pt_tuner_info->e_mod = MOD_UNKNOWN;
    }
    else if (sTunerParam.eModFormat == SD_MOD_NTSC)
    {
    TUNER_CAB_ANA_TUNE_INFO_T *pt_tuner_info;

        u2TunerId = u2GetTunerIdAna(eTuner);
        eConntype = TUNER_CONN_TYPE_CAB_ANA;
        pt_tuner_info = (TUNER_CAB_ANA_TUNE_INFO_T *) (&tConnInfo);
        pt_tuner_info->ui4_freq = u4TmpFreq * 1000;      
    }
    else
        return SD_ERR_INV;

    if (sTunerParam.eModFormat != SD_MOD_NTSC)
    {                                       /* acq digital, disconnect analog */
        TunerDisc(MTK_ANALOG_0, TUNER_CONN_TYPE_TER_ANA);
    }
    else
    {                                       /* acq analog, disconnect digital */
        TunerDisc(MTK_DTD_0, TUNER_CONN_TYPE_TER_DIG);
    }

    //if (sTunerParam.eModFormat != SD_MOD_NTSC)
    {
        eActionType = TUNE_FREQ_ONLY;
        TunerSetAttribute(u2TunerId, TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS,
                          (void *) &eActionType, sizeof(TUNER_ACQ_ACTION_T));
    }
   
    TunerAcq(u2TunerId, eConntype, &tConnInfo, sizeof(TUNER_ALL_INFO_T),
             i4CheckBreak, &eTuner);

    return SD_OK;
}

INT32 _Tuner_StartChannel_NA(SdTunerID_k eTuner, SdTuner_ModFormat_k eMode)
{
UINT16 u2TunerId;
TUNER_ALL_INFO_T tConnInfo;
DRV_CONN_TYPE_T eConntype;
TUNER_ACQ_ACTION_T  eActionType;

    mcPRINT_DBG("In kernel %s: %d, %d\n", __FUNCTION__, eTuner, eMode);

    if (!fgTunerInited)
        if (_Tuner_Init_NA(eTuner, SD_PWROFF_TO_NORMAL) != SD_OK)
            return SD_ERR_UNINIT;

    u2TunerId = u2GetTunerIdDig(eTuner);
    if (eMode == SD_MOD_8VSB)
    {
    TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info;

        eConntype = TUNER_CONN_TYPE_TER_DIG;
        pt_tuner_info = (TUNER_TER_DIG_TUNE_INFO_T *) (&tConnInfo);
        pt_tuner_info->ui4_freq = cTUNER_DEFAULT_FREQ * 1000;   // Valid range, ignore value
        pt_tuner_info->e_mod = MOD_VSB_8;
    }
    else if ((eMode == SD_MOD_64QAM)
          || (eMode == SD_MOD_256QAM)
          || (eMode == SD_MOD_QAM))
    {
    TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info;

        eConntype = TUNER_CONN_TYPE_CAB_DIG;
        pt_tuner_info = (TUNER_CAB_DIG_TUNE_INFO_T *) (&tConnInfo);
        pt_tuner_info->ui4_freq = cTUNER_DEFAULT_FREQ * 1000;   // Valid range, ignore value
        pt_tuner_info->e_mod = MOD_QAM_256;
        if (eMode == SD_MOD_64QAM)
            pt_tuner_info->e_mod = MOD_QAM_64;
        else if (eMode == SD_MOD_QAM)
            pt_tuner_info->e_mod = MOD_UNKNOWN;
    }
    else if (eMode == SD_MOD_NTSC)
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

    eActionType = ACQ_DEMOD_ONLY;
    TunerSetAttribute(u2TunerId, TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS,
                      (void *) &eActionType, sizeof(TUNER_ACQ_ACTION_T));

    TunerAcq(u2TunerId, eConntype, &tConnInfo, sizeof(TUNER_ALL_INFO_T),
             i4CheckBreak, &eTuner);

    return SD_OK;
}

INT32 _Tuner_Get_NA(SdTunerID_k eTuner, SdTuner_Settings_t *pSettings)
{
SdTuner_Settings_t sTunerSettings;

// Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *) pSettings, sizeof(SdTuner_Settings_t)))
    {
        mcPRINT_ERR("%s arg access error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }
// Copy user space argument
    if (copy_from_user(&sTunerSettings, (void __user *) pSettings, sizeof(SdTuner_Settings_t)))
    {
        mcPRINT_ERR("%s copy_from_user error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    mcPRINT_DBG("In kernel %s: %d, %d\n", __FUNCTION__, eTuner, sTunerSettings.bLnaOn);

    return SD_OK;
}

INT32 _Tuner_Set_NA(SdTunerID_k eTuner, SdTuner_Settings_t *pSettings)
{
SdTuner_Settings_t sTunerSettings;

// Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *) pSettings, sizeof(SdTuner_Settings_t)))
    {
        mcPRINT_ERR("%s arg access error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }
// Copy user space argument
    if (copy_from_user(&sTunerSettings, (void __user *) pSettings, sizeof(SdTuner_Settings_t)))
    {
        mcPRINT_ERR("%s copy_from_user error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    mcPRINT_DBG("In kernel %s: %d, %d\n", __FUNCTION__, eTuner, sTunerSettings.bLnaOn);

    return SD_OK;
}

INT32 _Tuner_SplitterGet_NA(SdTuner_SplitterSettings_t *pSettings)
{
SdTuner_SplitterSettings_t sSplitterSettings;

// Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *) pSettings, sizeof(SdTuner_SplitterSettings_t)))
    {
        mcPRINT_ERR("%s arg access error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }
// Copy user space argument
    if (copy_from_user(&sSplitterSettings, (void __user *) pSettings, sizeof(SdTuner_SplitterSettings_t)))
    {
        mcPRINT_ERR("%s copy_from_user error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    mcPRINT_DBG("In kernel %s: %d\n", __FUNCTION__, sSplitterSettings.eSwType);

    return SD_OK;
}

INT32 _Tuner_SplitterSet_NA(SdTuner_SplitterSettings_t *pSettings)
{
SdTuner_SplitterSettings_t sSplitterSettings;

// Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *) pSettings, sizeof(SdTuner_SplitterSettings_t)))
    {
        mcPRINT_ERR("%s arg access error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }
// Copy user space argument
    if (copy_from_user(&sSplitterSettings, (void __user *) pSettings, sizeof(SdTuner_SplitterSettings_t)))
    {
        mcPRINT_ERR("%s copy_from_user error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    mcPRINT_DBG("In kernel %s: %d\n", __FUNCTION__, sSplitterSettings.eSwType);

    return SD_OK;
}

#define cHIGH_RES_SNR_SCALE     512

INT32 _Tuner_GetStatus_NA(SdTunerID_k eTuner, SdTuner_ModFormat_k eMode,
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
        if (_Tuner_Init_NA(eTuner, SD_PWROFF_TO_NORMAL) != SD_OK)
        {
            mcPRINT_ERR("In kernel %s return: SD_ERR_UNINIT\n", __FUNCTION__);
        //    return SD_ERR_UNINIT;
            i4RetSts = SD_ERR_UNINIT;
            goto lbExit;
        }

    u2TunerId = u2GetTunerIdDig(eTuner);
    if (eMode == SD_MOD_8VSB)
    {
        eConntype = TUNER_CONN_TYPE_TER_DIG;
    }
    else if ((eMode == SD_MOD_64QAM)
          || (eMode == SD_MOD_256QAM)
          || (eMode == SD_MOD_QAM))
    {
        eConntype = TUNER_CONN_TYPE_CAB_DIG;
    }
    else if (eMode == SD_MOD_NTSC)
    {
        u2TunerId = u2GetTunerIdAna(eTuner);
        eConntype = TUNER_CONN_TYPE_CAB_ANA;
    }
    else
    {
        mcPRINT_ERR("In kernel %s return: SD_ERR_INV\n", __FUNCTION__);
    //    return SD_ERR_INV;
        i4RetSts = SD_ERR_INV;
        goto lbExit;
    }

    if (eMode == SD_MOD_NTSC)
    {
        sTunerState.bLock = fgHwTvdVPres();
        sTunerState.signalStrength = 0;
        u4Snr = 0;
        if (sTunerState.bLock)
        {
            sTunerState.signalStrength = 10;
            u4Snr = 30 * cHIGH_RES_SNR_SCALE;
        }
    }
    else
    {
        sTunerState.bLock = TunerGetSync(u2TunerId, eConntype);
        sTunerState.signalStrength = TunerGetSignalLevel(u2TunerId, eConntype) / 10;
        u4Snr = TunerGetSignalSNR(u2TunerId, eConntype);

        TunerGetSignal(u2TunerId, eConntype, &sSignal);
        if (sSignal.QAMSize == 8)
            sTunerState.eAutoQAM = SD_MOD_8VSB;
        if (sSignal.QAMSize == 64)
            sTunerState.eAutoQAM = SD_MOD_64QAM;
        else if (sSignal.QAMSize == 256)
            sTunerState.eAutoQAM = SD_MOD_256QAM;
    }

lbExit:
//    sTunerState.snr = u4Snr;
    mcPRINT_DBG("In kernel %s return: %d, %d, %d, %d\n", __FUNCTION__,
                sTunerState.bLock, u4Snr,
                sTunerState.signalStrength, sTunerState.eAutoQAM);

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

#define AFT_AD_NUM              0           // For the Servo AD in Pin0

EXTERN UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);

INT32 _Tuner_GetAFTVol_NA(UINT8 *pu1AFT_Vol)
{
UINT8 u1AFT_Vol;

    mcPRINT_DBG("In kernel %s\n", __FUNCTION__);

    u1AFT_Vol = PDWNC_ReadServoADCChannelValue(AFT_AD_NUM);  //ServoADCNum for Tuner S-curve  :ADC IN0

    mcPRINT_DBG("--> u1AFT_Vol = %X\n", u1AFT_Vol);

// Check access of user space
    if (!access_ok(VERIFY_WRITE, (void __user *) pu1AFT_Vol,
                   sizeof(UINT8)))
    {
        mcPRINT_ERR("%s arg access error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

//    *pu1AFT_Vol = u1AFT_Vol;
    if (copy_to_user((void __user *) pu1AFT_Vol, &u1AFT_Vol,
                     sizeof(UINT8)))
    {
        mcPRINT_ERR("%s copy_to_user error\n", __FUNCTION__);
        return SD_ERR_SYS;
    }

    return 0;
}

