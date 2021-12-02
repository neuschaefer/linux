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
 * $RCSfile: util_scdb.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains utility SCDB function for getting stream
 *    info by PID and stream types.
 *         
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  --------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "util/x_util_scdb.h"
#include "scdb/x_scdb_api.h"


/*--------------------------------------------------------------------------
  static function prototypes
  -------------------------------------------------------------------------*/

/*-----------------------------------------------------------------
 * Name: add_pid_value
 *
 * Description: This API add the PID value to specified array.
 *    If the PID value is already in the array, then it is ignore.
 *    Once the array is full, the count will continue to grow, but
 *    no new PID are added to array.
 *
 * Inputs:
 *
 *   ui2_pid:        Specify PID value to add to the array.
 *
 *   ui2_sz_array:   Size of the input array 'pui2_pid_array'.
 *
 *   pui2_pid_count: Number of PID value found so far.
 *
 * Outputs:
 *
 *   pui2_pid_array: Contain PID values.
 *
 *   pui2_pid_count: Number of PID value found after the new PID is
 *                   check for duplicate.
 *
 * Returns:
 *
 *   SCDBR_OK           Success.
 *
 *   SCDBR_FAIL         No PID value for this record.
 *
 *
----------------------------------------------------------------*/
static INT32 add_pid_value
(
    UINT16               ui2_pid,
    UINT16*              pui2_pid_array,
    UINT16               ui2_sz_array,
    UINT16*              pui2_pid_count
)
{
    INT32                i4_rc ;
    UINT16               ui2_i;
    BOOL                 b_new_pid;

    b_new_pid = TRUE;
    i4_rc = SCDBR_OK;

    /*
      check if this PID is already in the array.
    */
    for ( ui2_i = 0 ; ui2_i < (*pui2_pid_count) ; ui2_i++ )
    {
        if ( ui2_i < ui2_sz_array                &&
             pui2_pid_array[ui2_i] == ui2_pid )
        {
            b_new_pid = FALSE;
        }
    }

    if ( b_new_pid == TRUE )
    {
        if (  ui2_sz_array > ui2_i )
        {
            pui2_pid_array[ui2_i] = ui2_pid ;
        }
        else
        {
            i4_rc = SCDBR_BUF_TOO_SMALL;
        }
        
        /*
          Increment the count fo the number of PID values found.
        */
        *pui2_pid_count = (UINT16) ( (*pui2_pid_count) + 1 ) ;
    }
    
    return i4_rc ;
}

/*-----------------------------------------------------------------
 * Name: get_pid_value
 *
 * Description: This API get the PID value for SCDB record.
 *
 * Inputs:
 *
 *   pt_scdb_rec:    Ref to a SCDB record.
 *
 * Outputs:
 *
 *   pui2_pid:       PID valus found for this SCDB record.
 *
 * Returns:
 *
 *   SCDBR_OK           Success.
 *
 *   SCDBR_FAIL         No PID value for this record (e.g., analog entry)
 *
 *
----------------------------------------------------------------*/
static INT32  get_pid_value
(
    SCDB_REC_T*     pt_scdb_rec,
    UINT16*         pui2_pid
)
{
    INT32 i4_rc ;
    
    switch(pt_scdb_rec->e_rec_type)
    {
        case SCDB_REC_TYPE_AUDIO_MPEG:
        {
            *pui2_pid = pt_scdb_rec->u.t_audio_mpeg.ui2_pid;
            i4_rc = SCDBR_OK;
        }
        break;

        case SCDB_REC_TYPE_VIDEO_MPEG:
        {
            *pui2_pid = pt_scdb_rec->u.t_video_mpeg.ui2_pid;
            i4_rc = SCDBR_OK;
        }
        break;

        case SCDB_REC_TYPE_SUBTITLE_DVB:
        {
             *pui2_pid = pt_scdb_rec->u.t_dvb_sbtl.ui2_pid;
             i4_rc = SCDBR_OK;
        }
        break;

        case SCDB_REC_TYPE_TELETEXT_DVB:
        {
             *pui2_pid = pt_scdb_rec->u.t_dvb_tltxt.ui2_pid;
             i4_rc = SCDBR_OK;
        }
        break;

        case SCDB_REC_TYPE_VBI_TELETEXT_DVB:
        {
             *pui2_pid = pt_scdb_rec->u.t_dvb_vbi_tltxt.ui2_pid;
             i4_rc = SCDBR_OK;
        }
        break;

        case SCDB_REC_TYPE_CLOSED_CAPTION:
        case SCDB_REC_TYPE_AUDIO_ANALOG:
        case SCDB_REC_TYPE_VIDEO_ANALOG:
        {
            *pui2_pid = 0x1FFF;
            i4_rc = SCDBR_FAIL;
        }
        break;
        
        default:
        {
            *pui2_pid = 0x1FFF;
            i4_rc = SCDBR_FAIL;
        }
        break;
    }

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_util_scdb_get_stream_pid_info
 *
 * Description: This API return the all the PID values associated with a
 *              particular stream type.
 *
 * Inputs:
 *
 *   h_scdb:         Handle to SCDB
 *
 *   e_stream_type:  Specifies the stream type [ST_TELETEXT, ST_SUBTITLE,
 *                   ST_CLOSED_CAPTION, ST_AUDIO, ST_VIDEO]
 *
 *   pui2_sz_array   The size of array (number of elements) in the
 *                   'pui4_pid_array'.
 *
 * Outputs:
 *
 *   pui2_pid_array: Array with PID valus found for specified stream type.
 *
 *   pui2_sz_array:  Contains the number of PID values filled in the
 *                   'pui2_pid_array'.  If the array is too small (return
 *                   code 'SCDBR_BUF_TOO_SMALL') to hold all the PID values
 *                   found for this stream type, then
 *                   it return the size of array needed to store PIDs found
 *                   for this stream type (this value is maximum size needed
 *                   to store the PID value, so it might be bigger than
 *                   actual number of PID found)
 *
 *   pui4_ver_id:    The version id of current SCDB database.
 *
 * Returns:
 *
 *   SCDBR_OK           Success.
 *
 *   SCDBR_INV_HANDLE   h_scdb is invalid handle.
 *
 *   SCDBR_INV_ARG      'pui2_pid_array' is NULL, or 'pui2_sz_array' is 0,
 *                      'pui4_ver_id' is NULL.
 *
 *   SCDBR_DB_MODIFIED   Database object is being modified, caller should try
 *                       again.
 *                 
 *   SCDBR_BUF_TOO_SMALL  'pui2_pid_array' is not large enough to hold the
 *                        all the PID values for this stream type.
 *
 ----------------------------------------------------------------------------*/
INT32 x_util_scdb_get_stream_pid_info
(
    HANDLE_T             h_scdb,
    STREAM_TYPE_T        e_stream_type,
    UINT16*              pui2_pid_array,
    UINT16*              pui2_sz_array,
    UINT32*              pui4_ver_id
)
{
    STREAM_COMP_ID_T     t_comp_id;
    SCDB_REC_T           t_scdb_rec;
    INT32                i4_rc;
    UINT16               ui2_idx;
    UINT16               ui2_nb_pid_found;
    UINT16               ui2_pid;
    UINT16               ui2_sz_array;

    if ( pui2_sz_array  == NULL    ||
         pui2_pid_array == NULL    ||
         pui4_ver_id    == NULL    ||
         *pui2_sz_array == 0
        )
    {
        return SCDBR_INV_ARG;
    }
    
    ui2_idx            = 0;
    *pui4_ver_id       = SCDB_NULL_VER_ID ;
    ui2_nb_pid_found   = 0;
    ui2_sz_array       = *pui2_sz_array;

    /* clear out the input array. */
    x_memset(pui2_pid_array, 0, sizeof(UINT16)*ui2_sz_array);
     
    while (TRUE)
    {
        i4_rc = x_scdb_get_rec_by_idx(h_scdb,
                                      e_stream_type,
                                      ui2_idx,
                                      &t_comp_id,
                                      &t_scdb_rec,
                                      pui4_ver_id);

        if ( i4_rc == SCDBR_OK )
        {
            /*
              increment the index for next record.
            */
            ui2_idx++;
            
            /*
              get the PID value for the specified stream type.
            */
            i4_rc = get_pid_value(&t_scdb_rec, &ui2_pid);

            if ( i4_rc == SCDBR_OK )
            {
                add_pid_value(ui2_pid, pui2_pid_array,
                              ui2_sz_array, &ui2_nb_pid_found);
            }
        }
        else if ( i4_rc == SCDBR_REC_NOT_FOUND )
        {
            /* done all processing. no more records. */
            *pui2_sz_array = ui2_nb_pid_found;

            if ( ui2_nb_pid_found <= ui2_sz_array )
            {
                i4_rc = SCDBR_OK;
            }
            else
            {
                i4_rc = SCDBR_BUF_TOO_SMALL;
            }
            break;
        }
        else if ( i4_rc == SCDBR_DB_MODIFIED )
        {
            /* return to caller and caller can try again. */
            break;
        }
        else
        {
            /* all other error */
            break;
        }
    }
        
    return i4_rc;
} 


/*--------------------------------------------------------------------------
 * Name: x_util_scdb_get_stream_info
 *
 * Description: Return the SCDB records for a specified stream type
 *              and PID. If specified PID is 0x1FFF, then it is considered
 *              as a 'wild card', and all records for that stream type
 *              are returned.
 *
 * Inputs:
 *
 *   h_scdb:         Handle to SCDB
 *
 *   e_stream_type:  Specifies the stream type [ST_TELETEXT, ST_SUBTITLE,
 *                   ST_CLOSED_CAPTION, ST_AUDIO, ST_VIDEO]
 *
 *   ui2_pid:        Specifies the PID value.
 *
 *   pui2_sz_array   The size of array (number of elements) in the
 *                   'at_scdb_records'.
 *
 * Outputs:
 *
 *   at_scdb_records The size of array (number of elements) in the
 *                   'pui4_pid_array'.
 *
 *   pui2_sz_array:  Contains the number of records filled in the
 *                   'at_scdb_records'.  If the array is too small to hold
 *                   all the records found for this stream type, then
 *                   it return the size of array needed to hold all the
 *                   records.
 *
 *   pui4_ver_id:    The version id of current SCDB database.
 *
 * Returns:
 *
 *   SCDBR_OK            Success.
 *
 *   SCDBR_INV_HANDLE    h_scdb is invalid handle.
 *
 *   SCDBR_INV_ARG      'at_scdb_records' is NULL, or 'pui2_sz_array' is 0,
 *                      'pui4_ver_id' is NULL. 
 *                 
 *   SCDBR_DB_MODIFIED   Database object is being modified, caller should try
 *                       again.
 *                   
 *   SCDBR_BUF_TOO_SMALL  'at_scdb_array' is not large enough to hold the
 *                        all the PID values for this stream type.
 --------------------------------------------------------------------------*/
INT32 x_util_scdb_get_stream_info
(
    HANDLE_T             h_scdb,
    STREAM_TYPE_T        e_stream_type,
    UINT16               ui2_pid,
    SCDB_REC_T*          at_scdb_records,
    UINT16*              pui2_sz_array,
    UINT32*              pui4_ver_id
)
{
    STREAM_COMP_ID_T     t_comp_id;
    SCDB_REC_T           t_scdb_rec;
    INT32                i4_rc;
    UINT16               ui2_idx;
    UINT16               ui2_nb_rec_found;
    UINT16               ui2_this_pid;
    UINT16               ui2_sz_array;


    if ( pui2_sz_array    == NULL    ||
         at_scdb_records  == NULL    ||
         pui4_ver_id      == NULL    ||
         *pui2_sz_array   == 0
        )
    {
        return SCDBR_INV_ARG;
    }
    
    ui2_idx           = 0;
    ui2_nb_rec_found  = 0;
    *pui4_ver_id      = SCDB_NULL_VER_ID ;
    ui2_sz_array      = *pui2_sz_array;
    
    while (TRUE)
    {
        i4_rc = x_scdb_get_rec_by_idx(h_scdb,
                                      e_stream_type,
                                      ui2_idx,
                                      &t_comp_id,
                                      &t_scdb_rec,
                                      pui4_ver_id);

        if ( i4_rc == SCDBR_OK )
        {
            /*
              increment the index for next record.
            */
            ui2_idx++;
            
            /*
              get the PID value for the specified stream type.
            */
            if ( get_pid_value(&t_scdb_rec, &ui2_this_pid) == SCDBR_OK )
            {
                if ( ui2_pid == ui2_this_pid   ||
                     ui2_pid == 0x1FFF
                    )
                {
                    /*
                      Add the record to the array.
                    */
                    if ( ui2_nb_rec_found < ui2_sz_array )
                    {
#if 0                        
                        x_dbg_stmt("add pid [%d] to array.\n", ui2_this_pid);
#endif                        
                        at_scdb_records[ui2_nb_rec_found] = t_scdb_rec ;
                    }
                    else
                    {
#if 0                        
                        x_dbg_stmt("buffer too small, can not add pid [%d] "
                                   "to array.\n", ui2_this_pid);
#endif                        
                    }
                
                    ui2_nb_rec_found++;
                }
            }
        }
        else if ( i4_rc == SCDBR_REC_NOT_FOUND )
        {
            /* done all processing. no more records. */
            
            *pui2_sz_array = ui2_nb_rec_found;
            
            if ( ui2_nb_rec_found <= ui2_sz_array )
            {
#if 0                
                x_dbg_stmt("ok, record found: %d, array size %d\n",
                           ui2_nb_rec_found , ui2_sz_array);
#endif                
                i4_rc = SCDBR_OK;
            }
            else
            {
                i4_rc = SCDBR_BUF_TOO_SMALL;
#if 0                
                x_dbg_stmt("buffer to small, record found: %d, "
                           "array size %d\n",
                           ui2_nb_rec_found , ui2_sz_array);
#endif                
            }
            break;
        }
        else if ( i4_rc == SCDBR_DB_MODIFIED )
        {
            /* return to caller and caller can try again. */
            break;
        }
        else
        {
            /* all other error */
            break;
        }
    }
    return i4_rc;
}


/*
  Exported application API for SCDB utility functions.
*/
INT32 c_util_scdb_get_stream_info
(
    HANDLE_T             h_scdb,
    STREAM_TYPE_T        e_stream_type,
    UINT16               ui2_pid,
    SCDB_REC_T*          at_scdb_records,
    UINT16*              pui2_sz_array,
    UINT32*              pui4_ver_id
)
{
    return x_util_scdb_get_stream_info(h_scdb, e_stream_type,
                                       ui2_pid, at_scdb_records,
                                       pui2_sz_array, pui4_ver_id);
}

    
INT32 c_util_scdb_get_stream_pid_info
(
    HANDLE_T             h_scdb,
    STREAM_TYPE_T        e_stream_type,
    UINT16*              pui2_pid_array,
    UINT16*              pui2_sz_array,
    UINT32*              pui4_ver_id
)
{
    return x_util_scdb_get_stream_pid_info(h_scdb,
                                           e_stream_type,
                                           pui2_pid_array,
                                           pui2_sz_array,
                                           pui4_ver_id);
}
