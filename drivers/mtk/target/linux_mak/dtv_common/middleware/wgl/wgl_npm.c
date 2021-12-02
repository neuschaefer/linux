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
 * $RCSfile: wgl_lb_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "wgl/wgl_npm.h"
#include "wgl/wgl_bdr_tools.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define NPM_GET_NODE_DATA_PTR( _pt_node )               \
    ( (VOID *) WGL_ALIGN_4( ((UINT32) ( (_pt_node) + 1)) ) )

#define NPM_GET_NODE_TOTAL_SIZE( _z_node_data_size )    \
    ( WGL_ALIGN_4( WGL_ALIGN_4( sizeof (NPM_NODE_T) ) + (_z_node_data_size) ) )

/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _npm_partn_create( 
    IN      WGL_NPM_CTX_T*              pt_ctx,
    IN      SIZE_T                      z_node_data_size,
    IN      UINT16                      ui2_num_node,
    OUT     NPM_PARTN_T**               ppt_partn,
    OUT     UINT32*                     pui4_alloc_size);
static INT32 _npm_partn_destroy(
    IN      NPM_PARTN_T*                pt_partn);
static SIZE_T _npm_partn_tool_calc_size(
    IN      SIZE_T                      z_node_data_size,
    IN      UINT16                      ui2_num_node );
static BOOL _npm_partn_is_empty( 
    IN      NPM_PARTN_T*                pt_partn );
static INT32 _npm_partn_alloc_node(
    IN OUT  NPM_PARTN_T*                pt_partn,
    OUT     NPM_NODE_T**                ppt_node_free );
static INT32 _npm_partn_free_node(
    IN OUT  NPM_PARTN_T*                pt_partn,
    IN     NPM_NODE_T*                  pt_node);
static INT32 _npm_lock(HANDLE_T h_sema);
static INT32 _npm_unlock(HANDLE_T h_sema);

/*-----------------------------------------------------------------------------
                    public functions
 ----------------------------------------------------------------------------*/

/*****************************************************************************
            NPM - Node Pool Manager 
 *****************************************************************************/
INT32 wgl_npm_init(
    WGL_NPM_CTX_T*              pt_ctx,
    SIZE_T                      z_node_size,
    UINT16                      ui2_num_init,
    UINT16                      ui2_num_expand)
{
    NPM_PARTN_T         *pt_partn_head;
    INT32               i4_ret;
    UINT32              ui4_size = 0;
    
    /* init context */
    pt_ctx->ui2_num_expand   = ui2_num_expand;
    pt_ctx->z_node_data_size = z_node_size;
    pt_ctx->ui4_alloc_size = 0;
    
    /* init node list */
    DLIST_INIT( & pt_ctx->t_dlst_partns );
    
    /* insert a head partition */
    CATCH_FAIL( _npm_partn_create (pt_ctx,z_node_size, ui2_num_init, & pt_partn_head, &ui4_size ) );
    pt_ctx->ui4_alloc_size = ui4_size;

    DLIST_INSERT_HEAD( pt_partn_head, & pt_ctx->t_dlst_partns, t_link );
    
    CATCH_FAIL(x_sema_create(&pt_ctx->h_npm_lock, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK));
    return WGLR_OK;
}   

INT32 wgl_npm_deinit(
    WGL_NPM_CTX_T*              pt_ctx)
{
    NPM_PARTN_T         *pt_partn;
    INT32 i4_ret;
    
    pt_partn = DLIST_HEAD( & pt_ctx->t_dlst_partns );
    while ( pt_partn )
    {
        DLIST_REMOVE( pt_partn, & pt_ctx->t_dlst_partns, t_link );
        
        _npm_partn_destroy( pt_partn );
        
        pt_partn = DLIST_HEAD( & pt_ctx->t_dlst_partns );
    }
    
    CATCH_FAIL(x_sema_delete(pt_ctx->h_npm_lock));
    
    x_memset( pt_ctx, 0, sizeof(WGL_NPM_CTX_T) );
    
    return WGLR_OK;    
}
    
INT32 wgl_npm_alloc(
    WGL_NPM_CTX_T*              pt_ctx,
    VOID**                      ppv_node)
{
    INT32               i4_ret = WGLR_OK;
    BOOL                b_found;
    NPM_PARTN_T         *pt_partn;
    NPM_NODE_T          *pt_node_free;
    UINT32              ui4_size = 0;
    
    *ppv_node = NULL;
    
    /* find a free node */
    b_found      = FALSE;
    pt_node_free = NULL;
    CHECK_VALID(WGLR_OK == _npm_lock(pt_ctx->h_npm_lock), WGLR_LOCK_ERROR);
    
    DLIST_FOR_EACH( pt_partn, & pt_ctx->t_dlst_partns, t_link )
    {
        if( ! _npm_partn_is_empty( pt_partn ) )
        {
            i4_ret = _npm_partn_alloc_node( pt_partn, & pt_node_free );
            if( i4_ret != WGLR_OK )
            {
                x_dbg_stmt("{WGL} NPM ERR 1! \n");
                CHECK_VALID(WGLR_OK == _npm_unlock(pt_ctx->h_npm_lock), WGLR_UNLOCK_ERROR);
                 return i4_ret;
            }
            
            b_found      = TRUE;
            
            break;
        }
    }
    
    /* allocate a new partition if not found */
    if ( ! b_found )
    {
        NPM_PARTN_T     *pt_partn_new;
        
        /* create a new partition */
        i4_ret =  _npm_partn_create(pt_ctx,pt_ctx->z_node_data_size, pt_ctx->ui2_num_expand, &pt_partn_new, &ui4_size ) ;
        if(i4_ret != WGLR_OK)
        {
        
        x_dbg_stmt("{WGL} NPM ERR 2! \n");
            CHECK_VALID(WGLR_OK == _npm_unlock(pt_ctx->h_npm_lock), WGLR_UNLOCK_ERROR);
             return i4_ret;
        }
        pt_ctx->ui4_alloc_size += ui4_size;
                
        /* link the partition */
        DLIST_INSERT_TAIL( pt_partn_new, & pt_ctx->t_dlst_partns, t_link );
        
        /* allocate a free node */
        /* directly return on fail */
        i4_ret = _npm_partn_alloc_node( pt_partn_new, & pt_node_free );
        if(i4_ret != WGLR_OK)
        {
        
        x_dbg_stmt("{WGL} NPM ERR 3! \n");
             CHECK_VALID(WGLR_OK == _npm_unlock(pt_ctx->h_npm_lock), WGLR_UNLOCK_ERROR);
             return i4_ret;
        }
 
        
    }
    
    /* make sure ppv_node aligned at 4-bytes boundary */
    *ppv_node = NPM_GET_NODE_DATA_PTR( pt_node_free );
    
    CHECK_VALID(WGLR_OK == _npm_unlock(pt_ctx->h_npm_lock), WGLR_UNLOCK_ERROR);
    return WGLR_OK;
}        

INT32 wgl_npm_free(
    WGL_NPM_CTX_T*              pt_ctx,
    VOID*                       pv_node)
{
    INT32           i4_ret = WGLR_OK;
    BOOL            b_found;
    NPM_PARTN_T     *pt_partn;
    NPM_NODE_T      *pt_node;
    
    CHECK_VALID(WGLR_OK == _npm_lock(pt_ctx->h_npm_lock), WGLR_LOCK_ERROR);
    /* search for the node */
    b_found = FALSE;
    DLIST_FOR_EACH( pt_partn, & pt_ctx->t_dlst_partns, t_link )
    {
        DLIST_FOR_EACH( pt_node, & pt_partn->t_dlst_used, t_link )
        {
            if ( NPM_GET_NODE_DATA_PTR ( pt_node ) == pv_node )
            {
                b_found = TRUE;
                break;
            }
        }
        
        if ( b_found )
        {
            break;
        }
    }
    
    if(b_found)
    {
    /* free the node */
         _npm_partn_free_node( pt_partn, pt_node );
    
    /* TODO: Enhancement: free the partition if there are too many free partitions,
             do not free the head partition */
        if(DLIST_IS_EMPTY(&pt_partn->t_dlst_used) && DLIST_HEAD(& pt_ctx->t_dlst_partns) != pt_partn)
        {
            x_dbg_stmt("{WGL}free partn! \n");
    
            DLIST_REMOVE( pt_partn, & pt_ctx->t_dlst_partns, t_link );
            
            _npm_partn_destroy( pt_partn );
            pt_ctx->ui4_alloc_size -= _npm_partn_tool_calc_size(pt_ctx->z_node_data_size, pt_ctx->ui2_num_expand);
        }
    }
    else
    {   
        x_dbg_stmt("{WGL}Err,wgl_npm_free not found node! \n");
        i4_ret = WGLR_INV_ARG;
    }
    CHECK_VALID(WGLR_OK == _npm_unlock(pt_ctx->h_npm_lock), WGLR_UNLOCK_ERROR);
    return i4_ret;
}        

SIZE_T wgl_npm_get_node_size(
    const WGL_NPM_CTX_T*              pt_ctx)
{
    return pt_ctx->z_node_data_size;
}        

UINT32 wgl_npm_get_total_size(
    const WGL_NPM_CTX_T*              pt_ctx)
{
    return pt_ctx->ui4_alloc_size;
}        
#ifdef WGL_NPM_WIDGET_SUPPORT
#define WGL_NPM_WIDGET_MEM_CNT 16
#define WGL_NPM_WIDGET_INIT_NODE_SIZE (UINT16)200
#define WGL_NPM_WIDGET_EXPAND_NODE_SIZE (UINT16)200

static WGL_NPM_CTX_T _t_npm_mem[WGL_NPM_WIDGET_MEM_CNT];
static SIZE_T _au1_npm_node_size[WGL_NPM_WIDGET_MEM_CNT] = {136, 60, 56, 52, 84, 12, 428, 100, 812, 20, 244, 388, 508, 224, 216, 92};

static UINT8 _wgl_npm_mem_get_node_size_index(SIZE_T s_size)
{
	  static UINT32 ui4_total = 0;
	  static UINT32 ui4_hit = 0;
	  //x_dbg_stmt("<<<WGL_NPM_MEM_GET_NODE(%d)>>>\n\r", s_size);
	  ui4_total++;
    switch(s_size)
    {
        case 136:
        	  ui4_hit++;
            return 0;
        case 60:
        	  ui4_hit++;
            return 1;
        case 56:
        	  ui4_hit++;
            return 2;
        case 52:
        	  ui4_hit++;
            return 3;
        case 84:
        	  ui4_hit++;
            return 4;
        case 12:
        	  ui4_hit++;
            return 5;
        case 428:
        	  ui4_hit++;
            return 6;
        case 100:
        	  ui4_hit++;
            return 7;
        case 812:
        	  ui4_hit++;
            return 8;
        case 20:
        	  ui4_hit++;
            return 9;
        case 244:
        	  ui4_hit++;
            return 10;
        case 388:
        	  ui4_hit++;
            return 11;
        case 508:
        	  ui4_hit++;
            return 12;
        case 224:
        	  ui4_hit++;
            return 13;
        case 216:
        	  ui4_hit++;
            return 14;
        case 92:
        	  ui4_hit++;
            return 15; 
       default:
            return WGL_NPM_WIDGET_MEM_CNT;
    }
    return WGL_NPM_WIDGET_MEM_CNT;
}
INT32 wgl_npm_mem_init(VOID)
{
    UINT8 idx;
    for(idx=0;idx<WGL_NPM_WIDGET_MEM_CNT;idx++)
    {
        if(wgl_npm_init(&_t_npm_mem[idx],_au1_npm_node_size[idx],WGL_NPM_WIDGET_INIT_NODE_SIZE,WGL_NPM_WIDGET_EXPAND_NODE_SIZE) != WGLR_OK)
        {
           return WGLR_INIT_FAILED;
        }
        x_dbg_stmt("wgl_npm_mem_init() NPM CTX[%d]:0x%x\n",idx,&_t_npm_mem[idx]);
    }
    return WGLR_OK;
}
VOID* wgl_npm_mem_alloc(SIZE_T s_size)
{
    UINT8 idx = _wgl_npm_mem_get_node_size_index(s_size);
    VOID* pv_data = NULL;    
    INT32 i4_ret = WGLR_INV_ARG;
    if(idx < WGL_NPM_WIDGET_MEM_CNT)
    {
        i4_ret = wgl_npm_alloc(&_t_npm_mem[idx],&pv_data);
        if(i4_ret != WGLR_OK || pv_data == NULL)
        {
            x_dbg_stmt("{WGL}ERR:wgl_npm_mem_alloc() ret: %d\n",i4_ret);
        }
        return pv_data;
    }
    else
    {
        pv_data = x_mem_alloc(s_size+1*sizeof(UINT32));
        if( pv_data == NULL)
        {
            x_dbg_stmt("{WGL}ERR:x_mem_alloc() return null \n");            
            return pv_data;
        }
        *((UINT32*)pv_data) = 0x0000;
        pv_data += sizeof(UINT32);
        return pv_data;
    }
}

VOID wgl_npm_mem_free(VOID* pv_mem)
{
    
    VOID* pv_data = NULL;
    UINT32 ui4_data = 0;

    if(pv_mem == NULL)
    {
        return;
    }
    pv_data = pv_mem-4;
    ui4_data = *((UINT32*)pv_data);
    if(ui4_data == 0x0000)
    {
        x_mem_free(pv_data);
    }
    else
    {
       /*this is dangeuous here:pv_mem-4, must ensure that struct NPM_NODE_T is 4 bytes alignment*/ 
       if(ui4_data != (UINT32)(&_t_npm_mem[0]) &&
          ui4_data != (UINT32)(&_t_npm_mem[1]) &&
          ui4_data != (UINT32)(&_t_npm_mem[2]) &&
          ui4_data != (UINT32)(&_t_npm_mem[3]) &&
          ui4_data != (UINT32)(&_t_npm_mem[4]) &&
          ui4_data != (UINT32)(&_t_npm_mem[5]) &&
          ui4_data != (UINT32)(&_t_npm_mem[6]) &&
          ui4_data != (UINT32)(&_t_npm_mem[7]) &&
          ui4_data != (UINT32)(&_t_npm_mem[8]) &&
          ui4_data != (UINT32)(&_t_npm_mem[9]) &&
          ui4_data != (UINT32)(&_t_npm_mem[10]) &&
          ui4_data != (UINT32)(&_t_npm_mem[11]) &&
          ui4_data != (UINT32)(&_t_npm_mem[12]) &&
          ui4_data != (UINT32)(&_t_npm_mem[13]) &&
          ui4_data != (UINT32)(&_t_npm_mem[14]) &&
          ui4_data != (UINT32)(&_t_npm_mem[15]))
        {
          x_dbg_stmt("{WGL}ERR:wgl_npm_mem_free() pv_data: 0x%x\n",ui4_data);
        }
       
       wgl_npm_free((WGL_NPM_CTX_T*)ui4_data,pv_mem);
    }
}
#endif
/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _npm_partn_create( 
    IN      WGL_NPM_CTX_T*              pt_ctx,
    IN      SIZE_T                      z_node_data_size,
    IN      UINT16                      ui2_num_node,
    OUT     NPM_PARTN_T**               ppt_partn,
    OUT     UINT32*                     pui4_alloc_size)
{
    NPM_PARTN_T         *pt_partn_new;
    UINT16              ui2_i;
    UINT8               *pui1_ptr;
    NPM_NODE_T          *pt_node_new;
    UINT32              ui4_size;
    
    if(ppt_partn == NULL)
    {
        return WGLR_INV_ARG; 
    }
    *ppt_partn  = NULL;
    
    /* allocate memory for partition */
    ui4_size = _npm_partn_tool_calc_size( z_node_data_size, ui2_num_node );
    pt_partn_new = (NPM_PARTN_T *) x_mem_alloc( ui4_size );
    CHECK_VALID ( pt_partn_new != NULL, WGLR_OUT_OF_MEMORY );
    
    /* init partition */
    DLIST_INIT( & pt_partn_new->t_dlst_used );
    DLIST_INIT( & pt_partn_new->t_dlst_free );

    /* setup free node list */
    pui1_ptr = (UINT8 *) WGL_ALIGN_4( (UINT32) ( pt_partn_new + 1 ) );
    for ( ui2_i = 0; ui2_i < ui2_num_node; ui2_i ++ )
    {
        pt_node_new = (NPM_NODE_T *) pui1_ptr;
        
        pt_node_new->pt_ctx = (VOID*)pt_ctx;
            
        DLIST_INSERT_TAIL( pt_node_new, & pt_partn_new->t_dlst_free, t_link );
        
        pui1_ptr += NPM_GET_NODE_TOTAL_SIZE( z_node_data_size ) ;
    }
    
    *ppt_partn = pt_partn_new;
    if( pui4_alloc_size != NULL )
    {
        *pui4_alloc_size = ui4_size;
    }
    
    return WGLR_OK;
}        

static INT32 _npm_partn_destroy(
    IN      NPM_PARTN_T*                pt_partn)
{
    x_mem_free( (VOID *) pt_partn );
    
    return WGLR_OK;
}        

static SIZE_T _npm_partn_tool_calc_size(
    IN      SIZE_T                      z_node_data_size,
    IN      UINT16                      ui2_num_node )
{
    return ( WGL_ALIGN_4( sizeof( NPM_PARTN_T ) ) + 
        ((SIZE_T) ui2_num_node) * NPM_GET_NODE_TOTAL_SIZE( z_node_data_size ) );
}    

static BOOL _npm_partn_is_empty( 
    IN      NPM_PARTN_T*                pt_partn )
{
    return DLIST_IS_EMPTY ( & pt_partn->t_dlst_free ) ? TRUE : FALSE ;
}

static INT32 _npm_partn_alloc_node(
    IN OUT  NPM_PARTN_T*                pt_partn,
    OUT     NPM_NODE_T**                ppt_node_free )
{
    NPM_NODE_T      *pt_node_free = NULL;
    
    *ppt_node_free = NULL;
    
    CHECK_VALID( ! _npm_partn_is_empty ( pt_partn ), WGLR_INV_ARG );
    
    /* get a free node */
    pt_node_free = DLIST_HEAD( & pt_partn->t_dlst_free );
    
    /* remove from free list */
    DLIST_REMOVE( pt_node_free, & pt_partn->t_dlst_free, t_link );
    
    /* append into used list */
    DLIST_INSERT_TAIL( pt_node_free, & pt_partn->t_dlst_used, t_link );
    
    *ppt_node_free = pt_node_free;
    
    return WGLR_OK;
}    

static INT32 _npm_partn_free_node(
    IN OUT  NPM_PARTN_T*                pt_partn,
    IN     NPM_NODE_T*                  pt_node)
{
    
    /* removed from used list */
    DLIST_REMOVE( pt_node, & pt_partn->t_dlst_used, t_link );
    
    /* append into free list */
    DLIST_INSERT_TAIL( pt_node, & pt_partn->t_dlst_free, t_link );
    
    return WGLR_OK;
}        

static INT32 _npm_lock(HANDLE_T h_sema)
{
    INT32 i4_ret = x_sema_lock(h_sema, X_SEMA_OPTION_WAIT);
    if(i4_ret != OSR_OK)
    {
        x_dbg_stmt("{wgl}Err:_npm_lock error %d \n",i4_ret);
    }
    return (i4_ret == OSR_OK ? WGLR_OK : WGLR_OS_ERROR);
}

static INT32 _npm_unlock(HANDLE_T h_sema)
{
    INT32 i4_ret = x_sema_unlock(h_sema);

    if(i4_ret != OSR_OK)
    {
        x_dbg_stmt("{wgl}Err:_npm_unlock error %d \n",i4_ret);
    }
   return (i4_ret == OSR_OK ? WGLR_OK : WGLR_OS_ERROR);
}

