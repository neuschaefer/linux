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
 * $RCSfile: mheg5_os.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all os related interfaces between the MHEG-5
 *         stack from OceanBlue and the OSAI.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "dbg/u_dbg.h"
#include "dbg/x_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "dt/x_dt.h"
#include "dt/u_dt.h"
#include "mheg5/u_mheg5.h"
#include "mheg5/x_mheg5.h"
#include "mheg5/OceanBlue/glue/mheg5_OceanBlue_eng.h"
#include "mheg5/OceanBlue/glue/misc/mheg5_misc.h"

/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_misc.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_audio.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_video.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_control.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_si.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_tune.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_font.h"

#ifdef SYS_MHEG5_HK_SUPPORT
#include "font/x_fe.h"
#endif

#ifdef CLI_LVL_ALL
#ifdef  DBG_LEVEL_MODULE
#undef  DBG_LEVEL_MODULE
#endif

#define DBG_LEVEL_MODULE            mheg5_misc_get_dbg_level()

static UINT16    _ui2_mheg5_misc_dbg_level = DBG_INIT_LEVEL_MW_MHEG5;
#endif

/* Global data */

/*-----------------------------------------------------------------------------
 * Name:  tmMHEG5NotifyComponentProgress
 *
 * Description: This function is used to monitor the progress of the MHEG5 component.
 *
 * Inputs:  - progressMessage MHEG5 component control progress messages.
 *
 * Outputs: -
 *
 * Returns: - MHEG5_OK                           - Success
 *                MHEG5_ERR_BAD_PARAMETER  - Invalid parameter.
 *                MHEG5_ERR_OTHER               - Controlling application specific error.
 ----------------------------------------------------------------------------*/
tmMHEG5Err_t tmMHEG5NotifyComponentProgress(tmMHEG5ProgressMessage_t progressMessage)
{
    MHEG5_MSG_T     t_msg;
    
    /* Dump the progress of the MHEG5 component */
    switch (progressMessage)
    {
        case MHEG5_CI_APPLICATION_START_FAILED:
            DBG_INFO(("{MHEG5 Stack} %s(MHEG5_CI_APPLICATION_STOPPED)  \n", __FUNCTION__));
            break;

        case MHEG5_CI_APPLICATION_STOPPED:
            DBG_INFO(("{MHEG5 Stack} %s(MHEG5_CI_APPLICATION_STOPPED)  \n", __FUNCTION__));
            break;

        case MHEG5_CI_APPLICATION_STARTED:
            DBG_INFO(("{MHEG5 Stack} %s(MHEG5_CI_APPLICATION_STARTED)  \n", __FUNCTION__));
            break;
            
        case MHEG5_DSM_APPLICATION_STARTED:
            DBG_INFO(("{MHEG5 Stack}DBG_INFO %s(MHEG5_DSM_APPLICATION_STARTED) \n", __FUNCTION__));
            DBG_ERROR(("{MHEG5 Stack}DBG_ERROR %s(MHEG5_DSM_APPLICATION_STARTED) \n", __FUNCTION__));
            break;

        case MHEG5_DSM_APPLICATION_STOPPED:
            DBG_INFO(("{MHEG5 Stack} %s(MHEG5_DSM_APPLICATION_STOPPED) \n", __FUNCTION__));
            break;

        default:
            DBG_INFO(("{MHEG5 Stack} %s(Undefined Progress Message)    \n", __FUNCTION__));
            return MHEG5_ERR_BAD_PARAMETER;
    }

    /* Dispatch the progress of the MHEG5 component */
    /* Currently, only CI module may need handle the first three progresses. */
    t_msg.ui1_module   = MHEG5_MOD_CI;
    t_msg.ui2_msg_type = MHEG5_MSG_COMP_PRGS;
    t_msg.pv_nfy_tag   = (VOID*)progressMessage;
    
    x_mheg5_send_msg(MHEG5_OBS_ENG_CODE, &t_msg, 10);

    return MHEG5_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  tmMHEG5NotifyComponentError
 *
 * Description: This function is used to monitor errors from the MHEG5 component.
 *
 * Inputs:  - errorMessage MHEG5 component error messages.
 *
 * Outputs: -
 *
 * Returns: - MHEG5_OK                           - Success
 *                MHEG5_ERR_BAD_PARAMETER  - Invalid parameter.
 *                MHEG5_ERR_OTHER               - Controlling application specific error.
 ----------------------------------------------------------------------------*/
tmMHEG5Err_t tmMHEG5NotifyComponentError(tmMHEG5ErrorMessage_t errorMessage)
{
    if (MHEG5_ERROR_INTERNAL == errorMessage)
    {
        DBG_ERROR(("{MHEG5 Stack} %s(MHEG5_ERROR_INTERNAL) \n", __FUNCTION__));
        return MHEG5_OK;
    }
    else
    {
        DBG_ERROR(("{MHEG5 Stack} %s(Undefined Error Message) \n", __FUNCTION__));
        return MHEG5_ERR_BAD_PARAMETER;
    }
}

/*-----------------------------------------------------------------------------
 * Name:  tmMHEG5GetLocalTime
 *
 * Description: Provide the current local time and date, normally from the system
 *              real time clock, with any local time conversions (if necessary).
 *
 * Inputs:  - 
 *
 * Outputs: - pDateAndTime Current local time and date
 *
 * Returns: - MHEG5_OK                - Success
 *            MHEG5_ERR_BAD_PARAMETER - Invalid parameter.
 *            MHEG5_ERR_OTHER         - Controlling application specific error.
 ----------------------------------------------------------------------------*/
tmMHEG5Err_t tmMHEG5GetLocalTime(ptmMHEG5DateTime_t pDateAndTime)
{
    tmMHEG5Err_t t_err = MHEG5_ERR_BAD_PARAMETER;

    DBG_INFO((
        "{MHEG5 Stack} %s: pDateAndTime = 0x%x\r\n",
        __FUNCTION__,
        (UINT32) pDateAndTime));
    
    if (pDateAndTime)
    {
        DTG_T t_dtg;
    
        if (DTR_OK == x_dt_utc_sec_to_loc_dtg(x_dt_get_utc(NULL, NULL), &t_dtg))
        {
            pDateAndTime->year   = (U32BIT) t_dtg.ui2_yr;
            pDateAndTime->month  = (U32BIT) t_dtg.ui1_mo;
            pDateAndTime->day    = (U32BIT) t_dtg.ui1_day;
            pDateAndTime->hour   = (U32BIT) t_dtg.ui1_hr;
            pDateAndTime->minute = (U32BIT) t_dtg.ui1_min;
            pDateAndTime->second = (U32BIT) t_dtg.ui1_sec;
            
            t_err = MHEG5_OK;
        }
        else
        {
            t_err = MHEG5_ERR_OTHER;
        }
    }
    
    return (t_err);
}


INT32 mheg5_misc_notify_key_press(UINT32 ui4_key_code, BOOL*  pb_in_func_grp, BOOL b_process)
{
    tmMHEG5Err_t            t_err;
    INT32                   i4_ret;

    i4_ret = MHEG5R_OK;

    if (pb_in_func_grp == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    t_err = tmMHEG5NotifyKeyPress(mheg5_obs_convert_key_code(ui4_key_code));

    DBG_INFO(("[MHEG5-MISC]%s,#%d: return value %d. \r\n",__FUNCTION__, __LINE__, (INT8)t_err ));

    if (t_err == MHEG5_OK)
    {
        i4_ret = MHEG5R_OK;
        *pb_in_func_grp  = TRUE;
    }
    else if (t_err == MHEG5_IGNOR_REQUEST)
    {
        i4_ret = MHEG5R_OK;
        *pb_in_func_grp  = FALSE;
    }
    else
    {
        i4_ret = mheg5_obs_convert_err_code(t_err);
    }

    return i4_ret;
}


tmMHEG5Err_t tmMHEG5GetEmbeddedFont(const U8BIT* fontName, tmMHEG5FontData_t** fontData)
{
#ifdef SYS_MHEG5_HK_SUPPORT
    DBG_INFO("[MHEG5-FONT]INFO:#%d,%s(%s, %p): for mheg HK extension\r\n",__LINE__, __FUNCTION__, fontName, fontData);
   
    INT32 i4_ret = 0;
    UINT32 data_len = 0;
    U8BIT  *ps_data = NULL;
    FE_FNT_RSC_TYPE_T font_data_type = FE_FNT_RSC_TYPE_MEM;

    if( x_strcmp( (CHAR*)fontName, "rec://font/hk1" ) != 0 )
    {
        DBG_INFO("[MHEG5-FONT]ERR:%s,#%d: bad parameter. \r\n",__FUNCTION__, __LINE__ );
        return MHEG5_ERR_BAD_PARAMETER;
    }

    *fontData = x_mem_alloc(sizeof(tmMHEG5FontData_t));

    if (*fontData == NULL)
    {
        DBG_ERROR("[MHEG5-FONT]ERR:%s,#%d: alloc memory error. \r\n",__FUNCTION__, __LINE__ );
        return MHEG5_ERR_INTERNAL;
    }

    i4_ret = x_fe_get_fontdata_by_name("fnt_mheg5_hk",  &ps_data, &data_len, &font_data_type);

    if( i4_ret != 0 )
    {
       DBG_ERROR("[MHEG5-FONT]ERR:%s #%d:find font fail ret: %d. \r\n",__FUNCTION__, __LINE__,i4_ret );
       return MHEG5_ERR_INTERNAL;
    }

    if( font_data_type == FE_FNT_RSC_TYPE_MEM )
    {
        (*fontData)->dataType = MHEG5_RAW_FONT_DATA_TTF;
    }
    else
    {
        (*fontData)->dataType = MHEG5_FONT_FILE_PATH_TTF;
    }

    (*fontData)->data       = ps_data;
    (*fontData)->dataLength = (U32BIT)data_len;


    return MHEG5_OK; 

#else

    return MHEG5_ERR_INTERNAL;

#endif
}

BOOL mheg5_misc_is_native_app_enabled(VOID)
{
    return FALSE;
}

tmMHEG5Err_t tmMHEG5SetInputRegister(S32BIT inputReg)
{
    return MHEG5_ERR_INTERNAL;
}

tmMHEG5Err_t tmMHEG5SetInputMask(tmMHEG5KeyPress_t *inputMask,
                                        U16BIT maskSize)
{
    return MHEG5_ERR_INTERNAL;
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: mheg5_misc_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 mheg5_misc_get_dbg_level(VOID)
{
    return _ui2_mheg5_misc_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: mheg5_misc_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
BOOL mheg5_misc_set_dbg_level(UINT16 ui2_level)
{

    _ui2_mheg5_misc_dbg_level = ui2_level;
    return TRUE;

}
#endif


