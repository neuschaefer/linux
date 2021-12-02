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
 * $RCSfile: sm_drv_map.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/7 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 17c5a042aea0e30a1e8dca15868ca8c2 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "hash/hash_api.h"
#include "rwlock/rwl_api.h"
#include "strm_mngr/x_sm.h"
#include "strm_mngr/_sm_utils.h"
#include "strm_mngr/sm_drv_map.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_DRV_MAP_DEFAULT_HASH_TBL_SIZE       ((SIZE_T) 32)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T h_drv_map = NULL_HANDLE;
static HANDLE_T h_drv_map_lock = NULL_HANDLE;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

SIZE_T _sm_drv_map_get_hash_key_size( const SM_DRV_COMP_INFO_T* pt_drv_infor )
{
    if ( pt_drv_infor->b_shared_comp_id )
    {
        return (SIZE_T)(sizeof( SM_DRV_COMP_INFO_T ));
    }
    else
    {
        return (SIZE_T)(sizeof( SM_DRV_COMP_INFO_T ) - SM_DRV_LOGICAL_NAME_LEN);
    }
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 sm_drv_map_init(VOID)
{
    INT32 i4_ret;
    
    if ( h_drv_map != NULL_HANDLE )
    {
        return SMDMR_ALREADY_INIT;   
    }
    
    i4_ret = hash_create( SM_DRV_MAP_DEFAULT_HASH_TBL_SIZE, &h_drv_map );
    if ( i4_ret != HASH_OK )
    {
        return SMDMR_NOT_ENOUGH_RESOURCE;   
    }
    
    i4_ret = x_sema_create( &h_drv_map_lock,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || h_drv_map_lock==NULL_HANDLE )
    {
        return SMDMR_NOT_ENOUGH_RESOURCE;   
    }
    
    return SMDMR_OK;
}

INT32 sm_drv_map_destory(VOID)
{
    hash_delete( h_drv_map, NULL );
    h_drv_map = NULL_HANDLE;
    return SMR_OK;
}

INT32 sm_drv_map_add( const SM_DRV_COMP_INFO_T* pt_drv_infor )
{
    INT32      i4_ret;
    UINT32     ui4_ref_cnt = 1;
    VOID*      pv_obj = NULL;

    if ( h_drv_map == NULL_HANDLE )
    {
        return SMDMR_NOT_INIT_YET;
    }
    if ( !pt_drv_infor )
    {
        return SMDMR_INV_ARG;
    }
    
    sm_drv_map_lock();

    i4_ret = hash_get_obj( h_drv_map,
                           (INT8*)pt_drv_infor,
                           _sm_drv_map_get_hash_key_size( pt_drv_infor ),
                           (VOID**)&ui4_ref_cnt );
    if ( i4_ret == HASH_OK )
    {
        ui4_ref_cnt++;
    }
    
    i4_ret = hash_add_obj( h_drv_map,
                           (INT8*)pt_drv_infor,
                           _sm_drv_map_get_hash_key_size( pt_drv_infor ),
                           (VOID*)ui4_ref_cnt,
                           (VOID**)&pv_obj );

    sm_drv_map_unlock();;
    
    if ( i4_ret == HASH_OK && pv_obj )
    {
        return SMDMR_DRV_ALREADY_EXIST;   
    }
    else if ( i4_ret != HASH_OK )
    {
        return SMDMR_NOT_ENOUGH_RESOURCE;
    }
    
    return SMDMR_OK;
}

INT32 sm_drv_map_del( const SM_DRV_COMP_INFO_T* pt_drv_infor )
{
    INT32      i4_ret;
    UINT32     ui4_ref_cnt = 1;
    VOID*      pv_obj = NULL;
    
    if ( h_drv_map == NULL_HANDLE )
    {
        return SMDMR_NOT_INIT_YET;
    }
    if ( !pt_drv_infor )
    {
        return SMDMR_INV_ARG;
    }
    
    sm_drv_map_lock();
    
    i4_ret = hash_get_obj( h_drv_map,
                           (INT8*)pt_drv_infor,
                           _sm_drv_map_get_hash_key_size( pt_drv_infor ),
                           (VOID**)&ui4_ref_cnt );
    if ( i4_ret == HASH_OK )
    {
        if ( --ui4_ref_cnt == 0 )
        {
            i4_ret = hash_remove_obj( h_drv_map,
                                      (INT8*)pt_drv_infor,
                                      _sm_drv_map_get_hash_key_size( pt_drv_infor ),
                                      (VOID**)&pv_obj );            
        }
        else
        {
            i4_ret = hash_add_obj( h_drv_map,
                                   (INT8*)pt_drv_infor,
                                   _sm_drv_map_get_hash_key_size( pt_drv_infor ),
                                   (VOID*)ui4_ref_cnt,
                                   (VOID**)&pv_obj );
        }
    }
    
    sm_drv_map_unlock();;

    if ( i4_ret == HASH_NOT_EXIST )
    {
        return SMDMR_DRV_NOT_EXIST;   
    }
    else if ( i4_ret != HASH_OK )
    {
        return SMDMR_NOT_ENOUGH_RESOURCE;
    }
    
    return SMDMR_OK;
}

BOOL sm_drv_map_exists( const SM_DRV_COMP_INFO_T* pt_drv_infor )
{
    BOOL b_exists = FALSE;

    if ( h_drv_map == NULL_HANDLE )
    {
        return FALSE;
    }
    if ( !pt_drv_infor )
    {
        return FALSE;
    }
    
    sm_drv_map_lock();
    
    b_exists =  hash_is_key_exist( h_drv_map,
                                   (INT8*)pt_drv_infor,
                                   _sm_drv_map_get_hash_key_size( pt_drv_infor ) );
    
    sm_drv_map_unlock();;

    return b_exists;
}

VOID _sm_drv_map_lock(VOID)
{
    SM_LOCK( h_drv_map_lock )
}

VOID _sm_drv_map_unlock(VOID)
{
    SM_UNLOCK( h_drv_map_lock )
}
