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
 * $RCSfile: hash_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: d3541f46bb335fd5e307f0e02344135a $
 *
 * Description:
 *         This header file contains Stream Manager related API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "hash/hash_api.h"
#include "hash/_hash.h"

#ifdef SYS_MEM_CHK
#undef x_mem_alloc
#undef x_mem_calloc
#undef x_mem_realloc
#undef x_mem_free

extern VOID* x_mem_alloc (SIZE_T  z_size);

extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);

extern VOID* x_mem_realloc (VOID*        pv_mem_block,
                            SIZE_T       z_new_size);

extern VOID x_mem_free (VOID*  pv_mem_block);
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define HASH_LOCK( h_lock )                               \
{                                                         \
    if ( x_sema_lock( h_lock,                             \
                      X_SEMA_OPTION_WAIT ) != OSR_OK )    \
    {                                                     \
        DBG_ABORT( 0 );                                   \
    }                                                     \
}

#define HASH_UNLOCK( h_lock )                             \
{                                                         \
    if ( x_sema_unlock( h_lock ) != OSR_OK )              \
    {                                                     \
        DBG_ABORT( 0 );                                   \
    }                                                     \
}


typedef struct _ELEMENT_OBJ_T
{
    INT8*                   pi1_key;
    VOID*                   pv_obj;
    SIZE_T                  z_key_size;
    struct _ELEMENT_OBJ_T*  pt_next;
} ELEMENT_OBJ_T;
typedef ELEMENT_OBJ_T* P_ELEMENT_OBJ_T;

typedef struct _HASH_T
{
    P_ELEMENT_OBJ_T*    ppt_table;
    SIZE_T              z_table_size;
    UINT32              ui4_element_num;

    P_ELEMENT_OBJ_T     pt_iter;
    UINT32              ui4_next_bucket;
    HANDLE_T            h_lock;
} HASH_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static P_ELEMENT_OBJ_T   pt_elm_free_list = NULL;
static HANDLE_T          h_elm_free_list_lock = NULL_HANDLE;

static UINT32            ui4_alloc_elm_cnt = 0;
static UINT32            ui4_free_elm_cnt = 0;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static INT32 _hash_init( VOID )
{
    INT32          i4_ret;
    HANDLE_T       h_sema = NULL_HANDLE;
    CRIT_STATE_T   t_crit_stat;

    if ( h_elm_free_list_lock != NULL_HANDLE )
    {
        /* already initialized */
        return HASH_OK;
    }

    i4_ret = x_sema_create( &h_sema,
                            X_SEMA_TYPE_MUTEX,
                            X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT(0);
    }

    t_crit_stat = x_crit_start();

    if ( h_elm_free_list_lock != NULL_HANDLE )
    {
        /* already initialized */
        x_crit_end( t_crit_stat );

        x_sema_delete( h_sema );
        return HASH_OK;
    }

    h_elm_free_list_lock = h_sema;

    x_crit_end( t_crit_stat );

    return HASH_OK;
}

static VOID _hash_elm_free( P_ELEMENT_OBJ_T pt_elm_obj )
{
    if ( !pt_elm_obj )
    {
        return;
    }

    HASH_LOCK( h_elm_free_list_lock );

    pt_elm_obj->pt_next = pt_elm_free_list;
    pt_elm_free_list = pt_elm_obj;

    ++ui4_free_elm_cnt;

    HASH_UNLOCK( h_elm_free_list_lock );
}

static P_ELEMENT_OBJ_T _hash_elm_alloc(
    VOID*           pv_obj,
    const INT8*     pi1_key,
    SIZE_T          z_key_size )
{
    P_ELEMENT_OBJ_T pt_elm = NULL;
    INT8*           pi1_key_tmp = NULL;

    HASH_LOCK( h_elm_free_list_lock );

    if ( !pt_elm_free_list )
    {
        /* construct the element */
        pt_elm = (P_ELEMENT_OBJ_T) x_mem_alloc( sizeof(ELEMENT_OBJ_T) );
        if ( !pt_elm )
        {
            HASH_UNLOCK( h_elm_free_list_lock );
            return NULL;
        }
        pt_elm->pt_next = NULL;
        pt_elm->pv_obj = NULL;
        pt_elm->pi1_key = NULL;
        pt_elm->z_key_size = 0;

        ++ui4_alloc_elm_cnt;
    }
    else
    {
        pt_elm = pt_elm_free_list;
        pt_elm_free_list = pt_elm_free_list->pt_next;
        pt_elm->pt_next = NULL;

        --ui4_free_elm_cnt;
    }

    pt_elm->z_key_size = z_key_size;
    /* make MIN z_key_size to multiple of 16 */
    z_key_size = (z_key_size & 0xFFFFFFF0) +
                 ((z_key_size & 0x0000000F )? 0x10: 0);

    pi1_key_tmp = (INT8*) x_mem_realloc( pt_elm->pi1_key,
                                         sizeof(INT8) * z_key_size );
    if ( !pi1_key_tmp )
    {
        _hash_elm_free( pt_elm );
        HASH_UNLOCK( h_elm_free_list_lock );
        return NULL;
    }

    pt_elm->pi1_key = pi1_key_tmp;
    x_memcpy( pt_elm->pi1_key, pi1_key, pt_elm->z_key_size);
    pt_elm->pv_obj = pv_obj;

    HASH_UNLOCK( h_elm_free_list_lock );

    return pt_elm;
}


/*-----------------------------------------------------------------------------
 * Name: _hash_code_gen
 *
 * Description: This API is a hash function which generates hash codes.
 *
 * Inputs:  pt_hash_obj    Contains pointer to a hash object.
 *          pi1_key         Contains the key value.
 *          z_key_size     Contains the size of the key value.
 *
 * Outputs: None
 *
 * Returns: The code generate from pi1_key.
 *
 ----------------------------------------------------------------------------*/
static UINT32 _hash_code_gen( HASH_T*        pt_hash_obj,
                              const INT8*    pi1_key,
                              SIZE_T         z_key_size )
{
    SIZE_T i4_i = 0;
    UINT32 ui4_code = 0;

    /* FNV hash algorithm */
    for( i4_i = 0; i4_i < z_key_size; ++i4_i )
    {
        ui4_code ^= (UINT32)pi1_key[i4_i];

        ui4_code += ( (ui4_code<<1) + (ui4_code<<4) + (ui4_code<<7)
                      + (ui4_code<<8) + (ui4_code<<24) );
    }

    return (ui4_code % pt_hash_obj->z_table_size);
}

/*-----------------------------------------------------------------------------
 * Name: _hash_look_up
 *
 * Description: This API lookup the hash for a specific key, and returns the
 *              referenced element object pointer.
 *
 * Inputs:  pt_bucket    Contains pointer to the first element object of a bucket.
 *          pi1_key       Contains the key value to be found.
 *          z_key_size   Contains the size of the key value.
 *
 * Outputs: ppt_previous Contains pointer to the previous element object of
 *                         the founded element object.
 *
 * Returns: NULL         if not found.
 *          None NULL    Contains pointer to the founded element object.
 *
 ----------------------------------------------------------------------------*/
static P_ELEMENT_OBJ_T _hash_look_up( P_ELEMENT_OBJ_T  pt_bucket,
                                      const INT8*      pi1_key,
                                      SIZE_T           z_key_size,
                                      P_ELEMENT_OBJ_T* ppt_previous )
{
    if ( ppt_previous )
        *ppt_previous = NULL;

    while( pt_bucket != NULL )
    {
        if( x_memcmp( pt_bucket->pi1_key, pi1_key, z_key_size) == 0 )
        {
            return pt_bucket;
        }

        if ( ppt_previous )
            *ppt_previous  = pt_bucket;
        pt_bucket = pt_bucket->pt_next;
    }

    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: _hash_free_fct
 *
 * Description: This API releases the resources which are occupied by the hash.
 *
 * Inputs:  h_handle      Contains the handle to the hash.
 *          e_type        Contains the hash handle type and must be set to
 *                        HT_HASH.
 *          pv_obj        References the hash control object structure.
 *          pv_tag        Ignored.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs:
 *
 * Returns: FALSE        if parameter error or it's initiated by
 *                       x_handle_free().
 *          TRUE         if success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _hash_free_fct( HANDLE_T       h_handle,
                            HANDLE_TYPE_T  e_type,
                            VOID*          pv_obj,
                            VOID*          pv_tag,
                            BOOL           b_req_handle)
{
    HASH_T*     pt_hash_obj = pv_obj;

    if ( !pv_obj || b_req_handle || e_type != HT_HASH )
    {
        return FALSE;
    }

    x_sema_delete( pt_hash_obj->h_lock );
    x_mem_free( pt_hash_obj->ppt_table );
    x_mem_free( pt_hash_obj );

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: hash_create
 *
 * Description: This API create a hash with $z_init_size as initial table size.
 *
 * Inputs:  z_hash_tbl_size  Initial hash table size
 *
 * Outputs: ph_hash  Handle to the hash object.
 *
 * Returns: HASH_OK                        Hash is created.
 *          HASH_OUT_OF_MEMORY             System run out of memory resource.
 *          HASH_INTERNAL_ERROR            Internal error.
 *          HASH_OUT_OF_HANDLES            System run out of handle resource.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_create( SIZE_T                 z_hash_tbl_size,
                   HANDLE_T*              ph_hash )
{
    INT32           i4_ret;
    HASH_T*         pt_hash_obj = NULL;
    SIZE_T          z_tbl_size = sizeof(P_ELEMENT_OBJ_T) * z_hash_tbl_size;

    _hash_init();

    pt_hash_obj = (HASH_T*)x_mem_alloc( sizeof(struct _HASH_T) );
    if ( !pt_hash_obj )
    {
        return HASH_OUT_OF_MEMORY;
    }

    pt_hash_obj->ppt_table = (P_ELEMENT_OBJ_T*)x_mem_alloc( z_tbl_size );
    if ( !pt_hash_obj->ppt_table )
    {
        x_mem_free( pt_hash_obj );
        return HASH_OUT_OF_MEMORY;
    }

    x_memset( pt_hash_obj->ppt_table, 0, z_tbl_size );

    pt_hash_obj->z_table_size = z_hash_tbl_size;
    pt_hash_obj->ui4_element_num = 0;
    pt_hash_obj->pt_iter = NULL;
    pt_hash_obj->ui4_next_bucket = 0;
    pt_hash_obj->h_lock = NULL_HANDLE;

    i4_ret = x_sema_create( &pt_hash_obj->h_lock,
                         X_SEMA_TYPE_MUTEX,
                         X_SEMA_STATE_UNLOCK );
    if ( i4_ret != OSR_OK || pt_hash_obj->h_lock == NULL_HANDLE )
    {
        x_mem_free( pt_hash_obj->ppt_table );
        x_mem_free( pt_hash_obj );
        return HASH_INTERNAL_ERROR;
    }

    i4_ret = handle_alloc( HT_HASH,
                        pt_hash_obj,
                        (VOID*)NULL,
                        &_hash_free_fct,
                        ph_hash );
    if ( i4_ret != HR_OK || !ph_hash )
    {
        x_sema_delete( pt_hash_obj->h_lock );
        x_mem_free( pt_hash_obj->ppt_table );
        x_mem_free( pt_hash_obj );
        return HASH_OUT_OF_HANDLES;
    }

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: hash_delete
 *
 * Description: This API delete a hash.
 *
 * Inputs:  h_hash           Handle to the hash object.
 *          pf_obj_free_fct  Object free callback function.
 *
 * Outputs: None
 *
 * Returns: None
 *
 ----------------------------------------------------------------------------*/
VOID hash_delete( HANDLE_T                h_hash,
                  hash_obj_free_fct       pf_obj_free_fct )
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_hdl_type;
    HASH_T*         pt_hash_obj = NULL;
    SIZE_T          i4_i = 0;

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return;
    }

    hash_lock( h_hash );

    for ( i4_i = 0; i4_i < pt_hash_obj->z_table_size; ++i4_i )
    {
        P_ELEMENT_OBJ_T    pt_elm;

        for ( pt_elm = pt_hash_obj->ppt_table[i4_i]; pt_elm != NULL; )
        {
            P_ELEMENT_OBJ_T    pt_elm_tmp;

            if ( pf_obj_free_fct && pt_elm->pv_obj )
            {
                pf_obj_free_fct( pt_elm->pv_obj );
            }

            pt_elm_tmp = pt_elm;
            pt_elm = pt_elm->pt_next;

            _hash_elm_free( pt_elm_tmp );
        }
    }

    hash_unlock( h_hash );

    handle_free( h_hash, FALSE );

    return;
}

/*-----------------------------------------------------------------------------
 * Name: hash_add_obj
 *
 * Description: This API add a object to the hash with pi1_key as its key.
 *
 * Inputs:  h_hash           Contains the handle to the hash.
 *          pi1_key           Contains the key value to be found.
 *          z_key_size       Contains the size of the key value.
 *          pv_obj           References the element control object structure.
 *
 * Outputs: ppv_old_obj      Contains reference to the old object with the same
 *                           key.
 *
 * Returns: HASH_OK             If success.
 *          HASH_INV_ARG        Argument pi1_key contains a NULL value.
 *          HASH_INV_HANDLE     Argument h_hash is invalid.
 *          HASH_OUT_OF_MEMORY  System run out of memory resource.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_add_obj( HANDLE_T              h_hash,
                    const INT8*           pi1_key,
                    SIZE_T                z_key_size,
                    VOID*                 pv_obj,
                    VOID**                ppv_old_obj )    /* OUT */
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_hdl_type;
    HASH_T*         pt_hash_obj = NULL;
    P_ELEMENT_OBJ_T pt_elm, pt_previous;
    UINT32          ui4_code = 0;

    if ( !pi1_key )
    {
        return HASH_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return HASH_INV_HANDLE;
    }

    ui4_code = _hash_code_gen( pt_hash_obj, pi1_key, z_key_size );

    /* lookup if an old element exist */
    pt_elm = _hash_look_up( pt_hash_obj->ppt_table[ui4_code],
                            pi1_key,
                            z_key_size,
                            &pt_previous );
    if ( pt_elm != NULL )
    {
        /* found an old object with the same key */
        if ( ppv_old_obj )
        {
            *ppv_old_obj = pt_elm->pv_obj;
        }
        pt_elm->pv_obj = pv_obj;

        return HASH_OK;
    }
    /* this is a new key */
    if ( ppv_old_obj )
    {
        *ppv_old_obj = NULL;
    }

    pt_elm = _hash_elm_alloc( pv_obj, pi1_key, z_key_size );
    if ( !pt_elm )
    {
        return HASH_OUT_OF_MEMORY;
    }

    /* put the element to the head of the bucket */
    pt_elm->pt_next = pt_hash_obj->ppt_table[ui4_code];
    pt_hash_obj->ppt_table[ui4_code] = pt_elm;
    ++(pt_hash_obj->ui4_element_num);

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: hash_is_key_exist
 *
 * Description: This API query the hash to see if the pi1_key is already exist.
 *
 * Inputs:  h_hash           Contains the handle to the hash.
 *          pi1_key           Contains the key value to be found.
 *          z_key_size       Contains the size of the key value.
 *
 * Outputs: -
 *
 * Returns: TRUE  if pi1_key is already exist.
 *          FALSE if pi1_key is not exist.
 *
 ----------------------------------------------------------------------------*/
BOOL hash_is_key_exist( HANDLE_T              h_hash,
                        const INT8*           pi1_key,
                        SIZE_T                z_key_size )
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_hdl_type;
    HASH_T*         pt_hash_obj = NULL;
    P_ELEMENT_OBJ_T pt_elm, pt_previous;
    UINT32          ui4_code = 0;

    if ( !pi1_key )
    {
        return FALSE;
    }

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return FALSE;
    }

    ui4_code = _hash_code_gen( pt_hash_obj, pi1_key, z_key_size );

    /* lookup if an old element exist */
    pt_elm = _hash_look_up( pt_hash_obj->ppt_table[ui4_code],
                            pi1_key,
                            z_key_size,
                            &pt_previous );
    if ( pt_elm != NULL )
    {
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: hash_remove_obj
 *
 * Description: This API removes a object from the hash with pi1_key as its key.
 *
 * Inputs:  h_hash           Contains the handle to the hash.
 *          pi1_key           Contains the key value to be found.
 *          z_key_size       Contains the size of the key value.
 *
 * Outputs: ppv_obj          Contains reference to the object with key equal to
 *                           pi1_key.
 *
 * Returns: HASH_OK          If success.
 *          HASH_INV_ARG     Argument pi1_key contains a NULL value.
 *          HASH_INV_HANDLE  Argument h_hash is invalid.
 *          HASH_NOT_EXIST   There is no object with the key value pi1_key.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_remove_obj( HANDLE_T           h_hash,
                       const INT8*        pi1_key,
                       SIZE_T             z_key_size,
                       VOID**             ppv_obj )        /* OUT */
{
    INT32           i4_ret;
    UINT32          ui4_code = 0;
    HANDLE_TYPE_T   e_hdl_type;
    HASH_T*         pt_hash_obj = NULL;
    P_ELEMENT_OBJ_T pt_elm, pt_previous;

    if ( !pi1_key )
    {
        return HASH_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return HASH_INV_HANDLE;
    }

    ui4_code = _hash_code_gen( pt_hash_obj, pi1_key, z_key_size );

    /* lookup if an old element exist */
    pt_elm = _hash_look_up( pt_hash_obj->ppt_table[ui4_code],
                      pi1_key,
                      z_key_size,
                      &pt_previous );
    if ( pt_elm != NULL )
    {
        /* found an old object with the same key */
        if ( ppv_obj )
        {
            *ppv_obj = pt_elm->pv_obj;
        }

        if ( pt_previous )
        {
            pt_previous->pt_next = pt_elm->pt_next;
        }
        else /* pt_elm is the first element */
        {
            pt_hash_obj->ppt_table[ui4_code] = pt_elm->pt_next;
        }

        _hash_elm_free( pt_elm );
        --(pt_hash_obj->ui4_element_num);

        return HASH_OK;
    }

    return HASH_NOT_EXIST;
}

/*-----------------------------------------------------------------------------
 * Name: hash_get_obj
 *
 * Description: This API get a object from the hash with pi1_key as its key.
 *
 * Inputs:  h_hash           Contains the handle to the hash.
 *          pi1_key           Contains the key value to be found.
 *          z_key_size       Contains the size of the key value.
 *
 * Outputs: ppv_obj          Contains reference to the object with key equal to
 *                           pi1_key.
 *
 * Returns: HASH_OK          If success.
 *          HASH_INV_ARG     Argument pi1_key contains a NULL value.
 *          HASH_INV_HANDLE  Argument h_hash is invalid.
 *          HASH_NOT_EXIST   There is no object with the key value pi1_key.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_get_obj( HANDLE_T              h_hash,
                    const INT8*           pi1_key,
                    SIZE_T                z_key_size,
                    VOID**                ppv_obj )        /* OUT */
{
    INT32           i4_ret;
    UINT32          ui4_code = 0;
    HANDLE_TYPE_T   e_hdl_type;
    HASH_T*         pt_hash_obj = NULL;
    P_ELEMENT_OBJ_T pt_elm, pt_previous;

    if ( !pi1_key || !ppv_obj )
    {
        return HASH_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return HASH_INV_HANDLE;
    }

    ui4_code = _hash_code_gen( pt_hash_obj, pi1_key, z_key_size );

    /* lookup if an old element exist */
    pt_elm = _hash_look_up( pt_hash_obj->ppt_table[ui4_code],
                      pi1_key,
                      z_key_size,
                      &pt_previous );
    if ( pt_elm != NULL )
    {
        *ppv_obj = pt_elm->pv_obj;
        return HASH_OK;
    }

    return HASH_NOT_EXIST;
}

/*-----------------------------------------------------------------------------
 * Name: hash_get_size
 *
 * Description: This API gets current number of objects in the hash.
 *
 * Inputs:  h_hash           Contains the handle to the hash.
 *
 * Outputs: pz_size          Contains current number of objects in the hash.
 *
 * Returns: HASH_OK          If success.
 *          HASH_INV_ARG     Argument pz_size contains a NULL value.
 *          HASH_INV_HANDLE  Argument h_hash is invalid.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_get_size( HANDLE_T             h_hash,
                     SIZE_T*              pz_size )        /* OUT */
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_hdl_type;
    HASH_T*         pt_hash_obj = NULL;

    if ( !pz_size )
    {
        return HASH_INV_ARG;
    }

    i4_ret = x_handle_get_type( h_hash, &e_hdl_type );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return HASH_INV_HANDLE;
    }

    i4_ret = handle_get_obj( h_hash, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || !pt_hash_obj )
    {
        return HASH_INV_HANDLE;
    }

    *pz_size = pt_hash_obj->ui4_element_num;

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: hash_parse
 *
 * Description: This API calls for all objects attached to the hash the parse
 *              function.
 *
 * Inputs:  h_hash           Contains the handle to the hash.
 *          pf_parse_fct     References the parse function.
 *          pv_parse_tag     References some data which is interpreted by the
 *                           parse function.
 *
 * Outputs: -
 *
 * Returns: HASH_OK             If success.
 *          HASH_INV_ARG        Argument pf_parse_fct contains a NULL value.
 *          HASH_INV_HANDLE     Argument h_hash is invalid.
 *          HASH_PARSE_BREAK    If break by parse function.
 *          HASH_INTERNAL_ERROR If parse function returns an error.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_parse( HANDLE_T                h_hash,
                  hash_parse_fct          pf_parse_fct,
                  VOID*                   pv_parse_tag )
{
    UINT32            ui4_i = 0, ui4_counter = 0;
    INT32             i4_ret;
    P_ELEMENT_OBJ_T   pt_elm;
    HANDLE_TYPE_T     e_hdl_type;
    HASH_T*           pt_hash_obj = NULL;

    if ( !pf_parse_fct )
    {
        return HASH_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return HASH_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            i4_ret = (pf_parse_fct)(
                h_hash,
                ui4_counter++,
                pt_elm->pv_obj,
                pv_parse_tag );
            if ( i4_ret==HASH_PARSE_BREAK || i4_ret==HASH_INTERNAL_ERROR )
            {
                return i4_ret;
            }

            pt_elm = pt_elm->pt_next;
        }
    }

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: hash_del_obj_on_cond
 *
 * Description: This API lock the hash for some operation.
 *
 * Inputs:  h_hash              Contains the handle to the hash.
 *          pf_cond_fct         References the condition callback function.
 *                              If pf_cond_fct() returns TRUE, this API will
 *                              call pf_obj_free_fct() to free the object
 *                              memory.
 *          pf_obj_free_fct     Object free callback function.
 *          pv_tag              Reference to the user tag to pass to
 *                              pf_cond_fct().
 *
 * Outputs: -
 *
 * Returns: HASH_OK             If success.
 *          HASH_INV_HANDLE     Argument h_hash is invalid.
 *          HASH_INV_ARG        Argument pf_cond_fct is NULL.
 *          HASH_INTERNAL_ERROR If semaphone lock fail.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_del_obj_on_cond(
    HANDLE_T                h_hash,
    hash_cond_fct           pf_cond_fct,
    hash_obj_free_fct       pf_obj_free_fct,
    VOID*                   pv_tag )
{
    UINT32            ui4_i;
    INT32             i4_ret;
    P_ELEMENT_OBJ_T   pt_elm;
    HANDLE_TYPE_T     e_hdl_type;
    HASH_T*           pt_hash_obj = NULL;

    if ( !pf_cond_fct )
    {
        return HASH_INV_ARG;
    }

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return HASH_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        P_ELEMENT_OBJ_T     pt_elm_next = NULL;
        P_ELEMENT_OBJ_T     pt_elm_prev = NULL;

        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            BOOL b_cond;
            pt_elm_next = pt_elm->pt_next;

            b_cond = (pf_cond_fct)( pt_elm->pi1_key,
                                    pt_elm->z_key_size,
                                    pt_elm->pv_obj,
                                    pv_tag );
            if ( b_cond )
            {
                if ( pt_elm_prev )
                {
                    pt_elm_prev->pt_next = pt_elm_next;
                    /* pt_elm_prev remains the same */
                }
                else /* pt_elm is the first element */
                {
                    pt_hash_obj->ppt_table[ui4_i] = pt_elm_next;
                    pt_elm_prev = NULL;
                }

                if ( pf_obj_free_fct )
                {
                    pf_obj_free_fct( (VOID*)pt_elm->pv_obj );
                }

                _hash_elm_free( pt_elm );
            }
            else
            {
                pt_elm_prev = pt_elm;
            }

            pt_elm = pt_elm_next;
        }
    }

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: hash_lock
 *
 * Description: This API lock the hash for some operation.
 *
 * Inputs:  h_hash              Contains the handle to the hash.
 *
 * Outputs: -
 *
 * Returns: HASH_OK             If success.
 *          HASH_INV_HANDLE     Argument h_hash is invalid.
 *          HASH_INTERNAL_ERROR If semaphone lock fail.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_lock( HANDLE_T                 h_hash )
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_hdl_type;
    HASH_T*         pt_hash_obj = NULL;

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return HASH_INV_HANDLE;
    }

    i4_ret = x_sema_lock( pt_hash_obj->h_lock,
                       X_SEMA_OPTION_WAIT );
    if ( i4_ret != OSR_OK )
    {
        return HASH_INTERNAL_ERROR;
    }

    return HASH_OK;
}

/*-----------------------------------------------------------------------------
 * Name: hash_unlock
 *
 * Description: This API unlock the locked hash.
 *
 * Inputs:  h_hash              Contains the handle to the hash.
 *
 * Outputs: -
 *
 * Returns: HASH_OK             If success.
 *          HASH_INV_HANDLE     Argument h_hash is invalid.
 *          HASH_INTERNAL_ERROR If semaphone unlock fail.
 *
 ----------------------------------------------------------------------------*/
INT32 hash_unlock( HANDLE_T               h_hash )
{
    INT32           i4_ret;
    HANDLE_TYPE_T   e_hdl_type;
    HASH_T*         pt_hash_obj = NULL;

    i4_ret = handle_get_type_obj( h_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != HT_HASH )
    {
        return HASH_INV_HANDLE;
    }

    i4_ret = x_sema_unlock( pt_hash_obj->h_lock );
    if ( i4_ret != OSR_OK )
    {
        return HASH_INTERNAL_ERROR;
    }

    return HASH_OK;
}


