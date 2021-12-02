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
 * $RCSfile: _sm_sess_tbl.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: b875de372b9ad1022018ac43de87f8e4 $
 *
 * Description: 
 *         This file implements the Stream Manager's Session Control Table.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "dbg/dbg.h"
#include "handle/x_handle.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/_sm_utils.h"
#include "hash/hash_api.h"

#include "strm_mngr/_sm_sess_tbl.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_SESS_CTRL_TBL_SIZE           32

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/* Session Control Table,
   maintain a hash that uses connection handle as key, and session handle as 
   value. */
HANDLE_T     gh_sess_ctrl_tbl = NULL_HANDLE;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _sm_sess_tbl_get_sess_tbl_hash_cb
 *
 * Description: <function description>
 *
 * Inputs:  h_hash       References.
 *          ui4_counter  References.
 *          pv_obj       References.
 *          pv_parse_tag References.
 *
 * Outputs: - 
 *
 * Returns: HASH_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _sm_sess_tbl_get_sess_tbl_hash_cb( 
    HANDLE_T          h_hash,
    UINT32            ui4_counter,
    VOID*             pv_obj,
    VOID*             pv_parse_tag )
{
    HANDLE_T     h_sess = (HANDLE_T)pv_obj;
    HANDLE_T*    ph_sess_tbl = (HANDLE_T*) pv_parse_tag;

    SM_ASSERT( h_sess != NULL_HANDLE );
    SM_ASSERT( ph_sess_tbl ); 
    
    ph_sess_tbl[ui4_counter] = h_sess;
    
    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_tbl_init
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: SMR_OK                   Success.
 *          SMR_INSUFFICIENT_MEMORY  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_tbl_init( VOID )
{
    INT32  i4_ret;
    
    i4_ret = hash_create( SM_SESS_CTRL_TBL_SIZE, &gh_sess_ctrl_tbl );
    if ( i4_ret != HASH_OK || gh_sess_ctrl_tbl == NULL_HANDLE )
    {
        return SMR_INSUFFICIENT_MEMORY;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_tbl_lock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_sess_tbl_lock( VOID )
{
    if ( hash_lock( gh_sess_ctrl_tbl ) != HASH_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_LOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_tbl_unlock
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID sm_sess_tbl_unlock( VOID )
{
    if ( hash_unlock( gh_sess_ctrl_tbl ) != HASH_OK )
    {
        SM_ABORT( SM_DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE );
    }
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_tbl_find
 *
 * Description: <function description>
 *
 * Inputs:  h_connection References.
 *          ph_sess      References.
 *
 * Outputs: - 
 *
 * Returns: SMR_SESS_NOT_EXIST  Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_tbl_find( HANDLE_T            h_connection,
                        HANDLE_T*           ph_sess )
{
    INT32     i4_ret;
    
    i4_ret = hash_get_obj( gh_sess_ctrl_tbl,
                           (const INT8*)&h_connection,
                           sizeof(h_connection),
                           (VOID**)(&ph_sess) );
    if ( i4_ret == HASH_NOT_EXIST )
    {
        return SMR_SESS_NOT_EXIST;
    }
    else if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_tbl_get_sess_tbl
 *
 * Description: <function description>
 *
 * Inputs:  h_connection References.
 *          ph_sess      References.
 *
 * Outputs: - 
 *
 * Returns: SMR_SESS_NOT_EXIST  Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_tbl_get_sess_tbl( HANDLE_T*           ph_sess_tbl,
                                SIZE_T*             pz_size )
{
    INT32     i4_ret;
    SIZE_T    z_tbl_size = sm_sess_tbl_get_size();
    
    if ( !ph_sess_tbl || !pz_size )
    {
        return SMR_INV_ARG;
    }
    
    if ( (*pz_size / sizeof(HANDLE_T)) < z_tbl_size )
    {
        return SMR_INSUFFICIENT_SIZE;
    }
    
    *pz_size = z_tbl_size * sizeof(HANDLE_T);
    
    i4_ret = hash_parse( gh_sess_ctrl_tbl,
                         _sm_sess_tbl_get_sess_tbl_hash_cb,
                         (VOID*)ph_sess_tbl );

    if ( i4_ret == HASH_NOT_EXIST )
    {
        return SMR_SESS_NOT_EXIST;
    }
    else if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    return SMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: sm_sess_tbl_add
 *
 * Description: <function description>
 *
 * Inputs:  h_connection References.
 *          h_sess       References.
 *
 * Outputs: - 
 *
 * Returns: SMR_SESS_ALREADY_EXIST  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *          SMR_OK                  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_tbl_add( HANDLE_T             h_connection,
                       HANDLE_T             h_sess )
{
    INT32     i4_ret;
    HANDLE_T  h_sess_tmp;
    
    if ( sm_sess_tbl_find( h_connection, &h_sess_tmp )==SMR_OK )
    {
        return SMR_SESS_ALREADY_EXIST;    
    }
    
    i4_ret = hash_add_obj( gh_sess_ctrl_tbl,
                           (const INT8*)&h_connection,
                           sizeof(h_connection),
                           (VOID*)(h_sess),
                           NULL );
     if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
       
    return SMR_OK;
}

                       
/*-----------------------------------------------------------------------------
 * Name: sm_sess_tbl_remove
 *
 * Description: <function description>
 *
 * Inputs:  h_connection References.
 *
 * Outputs: - 
 *
 * Returns: SMR_SESS_NOT_EXIST  Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *          SMR_OK              Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 sm_sess_tbl_remove( HANDLE_T          h_connection )
{
    INT32     i4_ret;
    HANDLE_T* ph_sess_tmp;

    i4_ret = hash_remove_obj( gh_sess_ctrl_tbl,
                              (const INT8*)&h_connection,
                              sizeof(HANDLE_T),
                              (VOID**)(&ph_sess_tmp) );
    if ( i4_ret == HASH_NOT_EXIST )
    {
        return SMR_SESS_NOT_EXIST;
    }
    else if ( i4_ret != HASH_OK )
    {
        return SMR_INTERNAL_ERROR;
    }
    
    return SMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: sm_sess_tbl_get_size
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: Current size of session table.
 *
 ----------------------------------------------------------------------------*/
SIZE_T sm_sess_tbl_get_size( VOID )
{
    SIZE_T    z_size = 0;
    INT32     i4_ret;
    
    i4_ret = hash_get_size( gh_sess_ctrl_tbl, &z_size );
    if ( i4_ret != HASH_OK )
    {
        return 0;
    }
    
    return z_size;
}
