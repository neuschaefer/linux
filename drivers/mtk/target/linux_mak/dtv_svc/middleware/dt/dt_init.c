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
 * $RCSfile: dt_init.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains Date Time library function for
 *         interfacing with the RTC driver.
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "dt/_dt.h"
#include "dt/dt_cli.h"
#include "dt/dt_dbg.h"
#include "dt/dt_clock.h"

/*---------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------*/

/*
  Function table to load and unload broadcast table processing.
*/
static const DT_BRDCST_API_T*         pt_brdcst_api        = NULL;

static const DT_ANALOG_BRDCST_API_T*  pt_analog_brdcst_api = NULL;

UINT16    DT_CLOCK_BRDCST_UTC ;
UINT16    DT_CLOCK_USER_UTC ;

/*--------------------------------------------------------------------------
 * Name: dt_rtc_set_time
 *
 * Description - Set the current time on the RTC component.
 *
 * Input  - t_sec:  current system time as the number of seconds
 *                  since 00:00:00Z, Jan 1, 1970.
 *
 * Return - DTR_OK:  system current time is set.
 *          DTR_FAIL: Can not set current time on the RTC component.
 *          DTR_NO_RTC: No RTC component is registered with resource
 *                      manager.
 --------------------------------------------------------------------------*/

INT32 dt_init(DT_CONFIG_T*  pt_config)
{
    INT32              i4_rc;
    TIME_T             t_utc_sec = 0;
    
    i4_rc = DTR_OK;

    /*
      Initialize CLI support
    */
#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST)
    
    dt_cli_init();
    
#endif

    /*
      Reserve 2 clock (broadcast and user) for used by
      Datetime library.
    */
    if ( os_reserve_sys_td(&DT_CLOCK_BRDCST_UTC) != OSR_OK )
    {
        return DTR_FAIL;
    }

    if ( os_reserve_sys_td(&DT_CLOCK_USER_UTC) != OSR_OK )
    {
        return DTR_FAIL;
    }
    
    /*
      Initialize client notification mechanism.
    */
    dt_init_nfy(DT_MAX_NUM_CLIENTS);
    
    /*
      Set up handle to the RTC component. We will ignore the
      return status DTR_NO_RTC if no RTC is registered.
      The middleware will use os tick to implement the system
      clock.

      NOTE: If there is no RTC, we DON'T return an error. Just
      continue.
      
    */
    if ( dt_rtc_get_handle() == DTR_OK )
    {
        /* Get the RTC time. */
        if ( dt_rtc_get_time(&t_utc_sec) == DTR_OK )
        {
            /*
              Set the user time clock.
            */
            os_set_sys_td(t_utc_sec, 0, DT_CLOCK_USER_UTC);
            dt_set_cond ((INT32)DT_CLOCK_USER_UTC, DT_IS_RUNNING);
        }
    }

    /*
      If there is a valid UTC time from the RTC clock, then
      we will initialize the Broadcast clock with this UTC.
    */
    if ( t_utc_sec != 0 )
    {
        os_set_sys_td(t_utc_sec, 0, DT_CLOCK_BRDCST_UTC);
        dt_set_cond ((INT32)DT_CLOCK_BRDCST_UTC, DT_IS_RUNNING);
    }

    /* Init the new clock API support. */
    dt_clock_init();
    
    /*
      Set up the synchronization source for the current broadcast time.
    */
    x_dt_set_sync_src(pt_config->t_src_type,
                      pt_config->t_src_desc,
                      pt_config->pv_src_info);
        
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: x_dt_set_sync_src
 *
 * Description: This API sets the synchronization source for the current
 *              time.
 *
 * Input  -
 *    t_sync_src_type:  Specifies the types of synchronization source.
 *
 *    t_src_type_desc:  Specifies the source characteristic.
 *
 *    pv_src_info:      Pointer to a structure containing information
 *                      about the synchronization source. 
 *
 * Return - DTR_OK:   synchronization source is set.
 *          DTR_FAIL: Can not set the source of the synchronization for
 *                    the current time. 
 *          
 --------------------------------------------------------------------------*/
INT32 x_dt_set_sync_src
(
   DT_SYNC_SRC_TYPE_T          t_sync_src_type,
   DT_SRC_TYPE_DESC_T          t_src_type_desc,
   const VOID*                 pv_src_info    
)
{
    INT32           i4_rc;
    
/*    TM_SRC_TYPE_T   t_tbm_src_type; */

    i4_rc = DTR_FAIL;

    /*
      Determine the source type to obtain the
      current system time, and call the corresponding table
      engine.
    */
    switch (t_sync_src_type)
    {
        case DT_SYNC_SRC_NONE:
        {
            if ( pt_brdcst_api != NULL )
            {
                i4_rc = pt_brdcst_api->pf_unload_brdcst_time_info();
            }
        }
        break;

        case DT_SYNC_SRC_ANALOG_NONE:
        {
            if ( pt_analog_brdcst_api != NULL )
            {
                i4_rc = pt_analog_brdcst_api->pf_unload_brdcst_time_info();
            }
        }
        break;
        
        case DT_SYNC_WITH_DVB_TDT:
        case DT_SYNC_WITH_DVB_TOT:
        case DT_SYNC_WITH_DVB_TDT_OR_TOT:
        case DT_SYNC_WITH_DVB_TOT_OR_TDT:
        {
            switch (t_src_type_desc)
            {
                /*
                   For MPEG2 Broadcast source, set up the parameters
                   to connect to the tbm manager for DVB TDT/TOT table.
                */
                case DT_SRC_TYPE_MPEG_2_BRDCST:
                {
                    if ( pv_src_info != NULL )
                    {
                        /* t_tbm_src_type = TM_SRC_TYPE_MPEG_2_BRDCST; */
                        
                        /*
                          Set up connection and callback function
                          with DVB engine.
                        */
                        if ( pt_brdcst_api != NULL )
                        {
                            i4_rc = pt_brdcst_api->pf_load_brdcst_time_info(
                                t_sync_src_type,
                                t_src_type_desc, /* t_tbm_src_type,*/
                                pv_src_info);
                        }
                    }
                }
                break;
        
                default:
                {
                    /*
                      Currently, we only support DVB TDT and TOT table
                      from MPEG2 broadcast.
                    */
                    i4_rc = DTR_FAIL;
                }
                break;
            }
        }
        break;
        
        /* For cable out-of-band STT data. */
        case DT_SYNC_WITH_SCTE_STT:
        {
            switch (t_src_type_desc)
            {
                /*
                   For OOB Cable source, the format type can be
                   anything.
                */
                case DT_SRC_TYPE_FORMAT_UNKNOWN:
                {
                    /*
                      For Cable OOB data source,
                      the 'pv_src_info' should be set to NULL.
                    */
                    if ( pv_src_info == NULL )
                    {
                        /*
                          For the table manager, we need to specify
                          MPEG_2, OOB source type.
                        */
                        
                        /* t_tbm_src_type = TM_SRC_TYPE_MPEG_2_OOB; */
                        
                        /*
                          Set up connection and callback function
                          with ATSC engine.
                        */
                        if ( pt_brdcst_api != NULL )
                        {
                            i4_rc = pt_brdcst_api->pf_load_brdcst_time_info(
                                t_sync_src_type,
                                t_src_type_desc, /* t_tbm_src_type,*/
                                pv_src_info);
                        }
                    }
                    else
                    {
                        DBG_ERROR(("{DT} Error: tuner name parameter, pv_src_info, should be set to NULL for Cable OOB connection.\n"));
                    }
                }
                break;
        
                default:
                {
                    i4_rc = DTR_FAIL;
                }
                break;
            }
        }
        break;
            
        case DT_SYNC_WITH_ATSC_STT:
        {
            switch (t_src_type_desc)
            {
                /*
                   For MPEG2 Broadcast source, set up the parameters
                   to connect to the tbm manager for ATSC STT table.
                */
                case DT_SRC_TYPE_MPEG_2_BRDCST:
                {
                    if ( pv_src_info != NULL )
                    {
                        /* t_tbm_src_type = TM_SRC_TYPE_MPEG_2_BRDCST; */
                        
                        /*
                          Set up connection and callback function
                          with ATSC engine.
                        */
                        if ( pt_brdcst_api != NULL )
                        {
                            i4_rc = pt_brdcst_api->pf_load_brdcst_time_info(
                                t_sync_src_type,
                                t_src_type_desc,  /* t_tbm_src_type */
                                pv_src_info);
                        }
                    }
                }
                break;
        
                default:
                {
                    /*
                      Currently, we only support ATSC STT table from
                      MPEG2 broadcast.
                    */
                    i4_rc = DTR_FAIL;
                }
                break;
            }
        }
        break;

        case DT_SYNC_WITH_ANALOG_TELETEXT:
        {
            switch (t_src_type_desc)
            {
                /*
                   For VBI teletext source, set up the parameters
                   to receive teletext data from VBI filter.
                */
                case DT_SRC_TYPE_VBI_ANA_TER:
                case DT_SRC_TYPE_VBI_ANA_CAB:
                case DT_SRC_TYPE_VBI_ANA_SAT:
                {
                    /*
                       Open VBI filter and register callback function
                       with VBI filter engine.
                    */
                    if ( pt_analog_brdcst_api != NULL )
                    {
                        i4_rc = pt_analog_brdcst_api->pf_load_brdcst_time_info(
                                t_sync_src_type,
                                t_src_type_desc,  
                                pv_src_info);
                    }
                }
                break;
        
                default:
                {
                    /*
                      Currently, we only support teletext data from
                      VBI filter (analog terrestrial, analog cable,
                      or analog satellite.)
                    */
                    i4_rc = DTR_FAIL;
                    DBG_ERROR(("{DT} Invalid analog source type.\n"));
                }
                break;
            }
        }
        break;

        default:
        {
            /* unknown synchronization source, no action needed. */
            i4_rc = DTR_OK;
        }
        break;
    }
    return i4_rc;
}

/*
  GPS to UTC offset second.
  As of march, 2005, the gps to utc offset is 13 seconds.
*/
static INT32                    i4_gps_utc_offset = 13 ;

VOID dt_set_gps_utc_offset(INT32 i4_offset)
{
    i4_gps_utc_offset = i4_offset;
    return ;
}

INT32 dt_get_gps_utc_offset(VOID)
{
    return i4_gps_utc_offset;
}

/*****************************************************************
 * Name: dt_reg_brdcst_functions
 *
 * Description:  This function registers DVB function table.
 *
 * Inputs:
 *   pt_table     DVB function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 dt_reg_brdcst_functions(const DT_BRDCST_API_T* pt_table)
{
    INT32   i4_rc = DTR_OK;
    
    pt_brdcst_api = pt_table;
    
    return i4_rc;
}

/*****************************************************************
 * Name: dt_reg_analog_brdcst_functions
 *
 * Description:  This function registers analog broadcast function table.
 *
 * Inputs:
 *   pt_table    Analog function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 dt_reg_analog_brdcst_functions(const DT_ANALOG_BRDCST_API_T* pt_table)
{
    INT32   i4_rc = DTR_OK;
    
    pt_analog_brdcst_api = pt_table;
    
    return i4_rc;
}


/*****************************************************************
 * Name: dt_get_analog_brdcst_functions
 *
 * Description:  This function registers Analog broadcast function table.
 *
 * Inputs:
 *   pt_table     Analog broadcast function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
DT_ANALOG_BRDCST_API_T*  dt_get_analog_brdcst_functions(void)
{
    return (DT_ANALOG_BRDCST_API_T*)pt_analog_brdcst_api;
}
