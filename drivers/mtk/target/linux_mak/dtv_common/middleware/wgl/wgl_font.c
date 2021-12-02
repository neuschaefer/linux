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
 * $RCSfile: wgl_font.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/5 $
 * $SWAuthor: Al Hsieh $
 * $MD5HEX: 5c7f9b779bd3e3428d24b3fb6f494fb5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "font/x_fe.h"
#include "dbg/u_dbg.h"
#include "wgl/u_wgl.h"
#include "wgl/u_wgl_common.h"
#include "wgl/wgl_tools.h"
#include "wgl/wgl_sets_cli.h"
#include "wgl/wgl_font.h"


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#undef DBG_LEVEL_MODULE
#ifdef CLI_LVL_ALL
#define DBG_LEVEL_MODULE wgl_sets_cli_dbg_level_get()
#else
#define DBG_LEVEL_MODULE DBG_INIT_LEVEL_MW_WGL_MNGR
#endif

/*max font handle*/
#define _WGL_FONT_HANDLE_DEFAULT_GROW_NUM       ((UINT16) 10)
#define _WGL_FONT_BUF_NUM                       ((UINT8) 504) /* for CC: 3(size) * 4(UL+ITALIC) * 6 (edge_type) * 7 (fonts)  */

#define _WGL_FONT_REC_FLAG_WITH_UNIFORM_ATTR    ((UINT16) 0x0001)
#define _WGL_FONT_REC_FLAG_WITH_UNREC_CHAR      ((UINT16) 0x0002)
#define _WGL_FONT_REC_FLAG_WITH_ATTR            (_WGL_FONT_REC_FLAG_WITH_UNIFORM_ATTR |     \
                                                 _WGL_FONT_REC_FLAG_WITH_UNREC_CHAR)

typedef struct __WGL_FONT_REC_T
{
    WGL_FONT_INFO_T             t_font_info;
    WGL_FONT_ATTR_T             t_attr;
    HANDLE_T                    h_font;
    struct __WGL_FONT_REC_T*    pt_next;
    UINT16                      ui2_ref_count;
    UINT16                      ui2_flag;
}_WGL_FONT_REC_T;

typedef struct __WGL_FONT_MGR_T
{
    _WGL_FONT_REC_T*        at_buf[_WGL_FONT_BUF_NUM];          /*each buffer points to many records*/
    _WGL_FONT_REC_T*        pt_free_head;
    _WGL_FONT_REC_T*        pt_used_head;
    HANDLE_T                h_sema;
    UINT16                  ui2_grow_num;
    UINT8                   ui1_next_free_buf; 
}_WGL_FONT_MGR_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static _WGL_FONT_MGR_T      g_font_mgr;
 
 /*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
static VOID _wgl_font_lock(
    _WGL_FONT_MGR_T*                pt_this);
    
static VOID _wgl_font_unlock(
    _WGL_FONT_MGR_T*                pt_this);

static INT32 _wgl_font_add_rec(
    _WGL_FONT_MGR_T*            pt_this,
    const WGL_FONT_INFO_T*      pt_font_info,
    BOOL                        b_check_exist,
    HANDLE_T*                   ph_font_handle,
    _WGL_FONT_REC_T**           ppt_ref_rec);

static INT32 wgl_font_remove_rec(
    _WGL_FONT_MGR_T*            pt_this,
    HANDLE_T                    h_font_handle);

static INT32 _wgl_font_new_buf(
    _WGL_FONT_MGR_T*                pt_this);
    
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/ 
/*******************************************************************************
 * Name
 *          wgl_font_init
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_font_init(
    UINT16              ui2_font_handle_grow_num)
{
    _WGL_FONT_MGR_T*    pt_this = &g_font_mgr;
    INT32               i4_ret;
    
    /*check init twice*/
    if(NULL_HANDLE != pt_this->h_sema)
    {
        DBG_ERROR(("{WGL(FONT)} ERR: WGL's Font manager has been initialized in %s()\n\n", __FUNCTION__));
        return WGLR_INV_ARG;   
    }
    
    /**/
    i4_ret = x_sema_create(&pt_this->h_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    if(OSR_OK != i4_ret)
    {
        DBG_ERROR(("{WGL(FONT)} ERR: x_sema_create() failed. (%d) at %s()\n\n", i4_ret, __FUNCTION__));
        pt_this->h_sema = NULL_HANDLE;
        return WGLR_OS_ERROR;
    }
    
    if(0 == ui2_font_handle_grow_num)
    {
        ui2_font_handle_grow_num = _WGL_FONT_HANDLE_DEFAULT_GROW_NUM;
    }
    
    i4_ret = WGLR_INIT_FAILED;
    do
    {
        /**/
        pt_this->ui2_grow_num = ui2_font_handle_grow_num;
        pt_this->pt_free_head = NULL;
        pt_this->ui1_next_free_buf = 0;
        
        i4_ret = _wgl_font_new_buf(pt_this);
        if(WGLR_OK != i4_ret)
        {
            break;
        }
                
        return WGLR_OK;
            
    } while(0);
    
    wgl_font_deinit();
    return i4_ret;
}

/*******************************************************************************
 * Name
 *          wgl_font_deinit
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
VOID wgl_font_deinit(VOID)
{
    _WGL_FONT_MGR_T*    pt_this = &g_font_mgr;
    _WGL_FONT_REC_T*    pt_rec;
    INT32               i4_ret, i;
    
    /*free used font handle*/
    pt_rec = pt_this->pt_used_head;
    while(pt_rec)
    {
        if(x_handle_valid(pt_rec->h_font))
        {
            i4_ret = x_handle_free(pt_rec->h_font);
            WGL_ASSERT(HR_OK == i4_ret);
        }
        pt_rec->h_font = NULL_HANDLE;
        pt_rec = pt_rec->pt_next;
    }
    
    /*free buffer*/
    for(i = 0; i < pt_this->ui1_next_free_buf; i++)
    {
        if(pt_this->at_buf[i])
        {
            WGL_MEM_FREE(pt_this->at_buf[i]);
            pt_this->at_buf[i] = NULL;
        }
    }
    pt_this->ui1_next_free_buf = 0;
    
    /*free semaphore*/
    if(x_handle_valid(pt_this->h_sema))
    {
        i4_ret = x_handle_free(pt_this->h_sema);
        WGL_ASSERT(HR_OK == i4_ret);
    }
    pt_this->h_sema = NULL_HANDLE;
    
    /*reset manager*/
    x_memset(pt_this, 0, sizeof(_WGL_FONT_MGR_T));
}

/*******************************************************************************
 * Name
 *          wgl_font_create
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_font_create(
    const WGL_FONT_INFO_T*      pt_font_info,
    HANDLE_T*                   ph_font_handle)
{
    _WGL_FONT_MGR_T*            pt_this = &g_font_mgr;
    INT32                       i4_ret;
    
    /*check if init*/
    if(NULL_HANDLE == pt_this->h_sema)
    {
        DBG_ERROR(("{WGL(FONT)} ERR: WGL's Font manager isn't initialized in %s()\n\n", __FUNCTION__));
        return WGLR_INV_ARG;
    }
    
    /*check param*/
    if(NULL == pt_font_info || NULL == ph_font_handle)
    {
        DBG_ERROR(("{WGL(FONT)} ERR: Arguments of %s() cannot be NULL\n\n", __FUNCTION__));
        return WGLR_INV_ARG;
    }
    
    _wgl_font_lock(pt_this);

    i4_ret = _wgl_font_add_rec(pt_this, pt_font_info, TRUE, ph_font_handle, NULL);
    
    _wgl_font_unlock(pt_this);
    return i4_ret;
}

/*******************************************************************************
 * Name
 *          wgl_font_free
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_font_free(
    HANDLE_T                    h_font_handle)
{
    _WGL_FONT_MGR_T*            pt_this = &g_font_mgr;
    INT32                       i4_ret;
    
    /*check if init*/
    if(NULL_HANDLE == pt_this->h_sema)
    {
        DBG_ERROR(("{WGL(FONT)} ERR: WGL's Font manager isn't initialized in %s()\n\n", __FUNCTION__));
        return WGLR_INV_ARG;
    }
    
    /*check param*/
    if(FALSE == x_handle_valid(h_font_handle))
    {
        WGL_ASSERT(0);
        return WGLR_INV_HANDLE;
    }
    
    _wgl_font_lock(pt_this);
    
    i4_ret = wgl_font_remove_rec(pt_this, h_font_handle);
    
    _wgl_font_unlock(pt_this);
    return i4_ret;
}

/*******************************************************************************
 * Name
 *          wgl_font_change_attr
 * Description
 * Input arguments
 * Output arguments
 * Returns
 ******************************************************************************/
INT32 wgl_font_change_attr(
    HANDLE_T*                   ph_font_handle,
    const WGL_FONT_ATTR_T*      pt_attr,
    UINT32                      ui4_mask)
{
    _WGL_FONT_MGR_T*            pt_this = &g_font_mgr;
    _WGL_FONT_REC_T             *pt_rec, *pt_attr_rec, *pt_new_rec;
    HANDLE_T                    h_org_font, h_new_font;
    INT32                       i4_ret;
    UINT16                      ui2_new_attr_flag;

    if(NULL == ph_font_handle || NULL_HANDLE == *ph_font_handle || NULL == pt_attr || 0 == ui4_mask)
    {
        return WGLR_INV_ARG;
    }

    /*check if init*/
    if(NULL_HANDLE == pt_this->h_sema)
    {
        DBG_ERROR(("{WGL(FONT)} ERR: WGL's Font manager isn't initialized in %s()\n\n", __FUNCTION__));
        return WGLR_INV_ARG;
    }
    
    /*check param*/
    if(FALSE == x_handle_valid(*ph_font_handle))
    {
        WGL_ASSERT(0);
        return WGLR_INV_HANDLE;
    }
    
    _wgl_font_lock(pt_this);

    pt_rec = pt_this->pt_used_head;
    while(pt_rec)
    {
        if(pt_rec->h_font == *ph_font_handle)
        {
            break;
        }
        
        pt_rec = pt_rec->pt_next;
    }
    
    if(pt_rec)
    {
        h_new_font = h_org_font = NULL_HANDLE;
        ui2_new_attr_flag = 0;

        /*collect new flag*/
        if(WGL_FONT_ATTR_MASK_UNIFORM & ui4_mask)
        {
            ui2_new_attr_flag |= _WGL_FONT_REC_FLAG_WITH_UNIFORM_ATTR;
        }
        if(WGL_FONT_ATTR_MAKS_UNREC_CHAR & ui4_mask)
        {
            ui2_new_attr_flag |= _WGL_FONT_REC_FLAG_WITH_UNREC_CHAR;
        }


        /*check if there's another font handle with the same attributes*/
        pt_attr_rec = pt_this->pt_used_head;
        while(pt_attr_rec)
        {
            if(pt_rec != pt_attr_rec &&
               pt_attr_rec->t_font_info.e_font_size == pt_rec->t_font_info.e_font_size &&
               pt_attr_rec->t_font_info.e_font_style == pt_rec->t_font_info.e_font_style &&
               pt_attr_rec->t_font_info.e_font_cmap == pt_rec->t_font_info.e_font_cmap &&
               pt_attr_rec->t_font_info.i2_width == pt_rec->t_font_info.i2_width &&
               0 == x_strncmp(pt_attr_rec->t_font_info.a_c_font_name, pt_rec->t_font_info.a_c_font_name, WGL_MAX_FONT_NAME))
            {
                /*are their attribute flags the same?*/
                if((_WGL_FONT_REC_FLAG_WITH_ATTR & pt_attr_rec->ui2_flag) == (ui2_new_attr_flag | (_WGL_FONT_REC_FLAG_WITH_ATTR & pt_rec->ui2_flag)))
                {
                    /*are their attributes the same?*/
                    if((WGL_FONT_ATTR_MASK_UNIFORM & ui4_mask) && (pt_attr_rec->t_attr.t_uniform.ui2_width == pt_attr->t_uniform.ui2_width))
                    {
                        break;  /*yes, they're the same.*/
                    }

                    if((WGL_FONT_ATTR_MAKS_UNREC_CHAR & ui4_mask) && (pt_attr_rec->t_attr.w2c_unrec_char == pt_attr->w2c_unrec_char))
                    {
                        break;  /*yes, they're the same.*/
                    }
                }
            }
            
            pt_attr_rec = pt_attr_rec->pt_next;
        }

        /*there's another font handle with the same attributes*/
        if(pt_attr_rec)
        {
            if(pt_attr_rec == pt_rec) /*same rec*/
            {
                /*do nothing*/
            }
            else
            {
                /*free old rec*/
                wgl_font_remove_rec(pt_this, pt_rec->h_font);

                /*increase new rec's reference count*/
                pt_attr_rec->ui2_ref_count++;
                *ph_font_handle = pt_attr_rec->h_font;
            }

            i4_ret = WGLR_OK;
        }
        else
        {
            do 
            {
                h_org_font = pt_rec->h_font;
                
                if(1 != pt_rec->ui2_ref_count) /*Others are using this font handle*/
                {
                    /*new a font handle*/
                    i4_ret = _wgl_font_add_rec(pt_this, &pt_rec->t_font_info, FALSE, &h_new_font, &pt_new_rec);
                    if(WGLR_OK != i4_ret)
                    {
                        h_org_font = h_new_font = NULL_HANDLE;
                        break;
                    }
                }
                else
                {
                    h_new_font = pt_rec->h_font;
                    pt_new_rec = pt_rec;
                }

                /*update font attribute*/
                if(WGL_FONT_ATTR_MASK_UNIFORM & ui4_mask)
                {
                    i4_ret = x_fe_set_uniform_width(h_new_font, pt_attr->t_uniform.ui2_width);
                    if(FER_OK != i4_ret)
                    {
                        i4_ret = WGLR_SYS_RC_CREATE_FAIL;
                        break;
                    }

                    pt_new_rec->ui2_flag |= _WGL_FONT_REC_FLAG_WITH_UNIFORM_ATTR;
                    pt_new_rec->t_attr.t_uniform = pt_attr->t_uniform;
                }

                if(WGL_FONT_ATTR_MAKS_UNREC_CHAR & ui4_mask)
                {
                    i4_ret = x_fe_set_unrecognizable_char(h_new_font, pt_attr->w2c_unrec_char);
                    if(FER_OK != i4_ret)
                    {
                        i4_ret = WGLR_SYS_RC_CREATE_FAIL;
                        break;
                    }

                    pt_new_rec->ui2_flag |= _WGL_FONT_REC_FLAG_WITH_UNREC_CHAR;
                    pt_new_rec->t_attr.w2c_unrec_char = pt_attr->w2c_unrec_char;
                }
                
                pt_rec = pt_new_rec;
                *ph_font_handle = pt_rec->h_font;
                i4_ret = WGLR_OK;

            } while(0);

            /*free original font handle if not used.*/
            if(NULL_HANDLE != h_org_font && h_org_font != pt_rec->h_font)
            {
                wgl_font_remove_rec(pt_this, h_org_font);
            }

            /*free new font handle if not used.*/
            if(NULL_HANDLE != h_new_font && h_new_font != pt_rec->h_font)
            {
                wgl_font_remove_rec(pt_this, h_new_font);
            }
        }
    }
    else
    {
        DBG_ERROR(("{WGL(FONT)} ERR: WGL's Font manager cannot find the font handle %d.\n\n", *ph_font_handle));
        i4_ret = WGLR_INV_HANDLE;
        WGL_ASSERT(0);
    }

    _wgl_font_unlock(pt_this);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * private VIEW methods implementations
 *---------------------------------------------------------------------------*/ 
/*-----------------------------------------------------------------------------
 * Name: _wgl_font_lock
 *
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
static VOID _wgl_font_lock(
    _WGL_FONT_MGR_T*       pt_this)
{
    INT32                  i4_ret;
    
    i4_ret = x_sema_lock(pt_this->h_sema, X_SEMA_OPTION_WAIT);
    if(OSR_OK != i4_ret)
    {
        x_dbg_stmt("Fail to lock Widget's Font handle manager. i4_ret=%d\r\n", i4_ret);
        WGL_ASSERT(0);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _wgl_font_unlock
 *
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
static VOID _wgl_font_unlock(
    _WGL_FONT_MGR_T*         pt_this)
{
    INT32                    i4_ret;
    
    i4_ret = x_sema_unlock(pt_this->h_sema);
    if(OSR_OK != i4_ret)
    {
        x_dbg_stmt("Fail to unlock Widget's Font handle manager. i4_ret=%d\r\n", i4_ret);
        WGL_ASSERT(0);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _wgl_font_add_rec
 *
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _wgl_font_add_rec(
    _WGL_FONT_MGR_T*            pt_this,
    const WGL_FONT_INFO_T*      pt_font_info,
    BOOL                        b_check_exist,
    HANDLE_T*                   ph_font_handle,
    _WGL_FONT_REC_T**           ppt_ref_rec)
{
    _WGL_FONT_REC_T*            pt_rec;
    const CHAR*                 ps_font_name;
    INT32                       i4_ret;

    if(pt_font_info == NULL || pt_font_info->e_font_size>=FE_FNT_SIZE_NUMBER || 
       (pt_font_info->e_font_size==FE_FNT_SIZE_CUSTOM && pt_font_info->ui1_custom_size==0)
      )
    {
        DBG_ERROR(("{WGL(FONT)} ERR: _wgl_font_add_rec() failed because of invalid argument \n\n"));
        return WGLR_INV_ARG;
    }
    /**/
    if(b_check_exist)
    {
        pt_rec = pt_this->pt_used_head;
        while(pt_rec)
        {
            if(pt_rec->t_font_info.e_font_size == pt_font_info->e_font_size &&
               pt_rec->t_font_info.e_font_size!=FE_FNT_SIZE_CUSTOM &&
               pt_rec->t_font_info.e_font_style == pt_font_info->e_font_style &&
               pt_rec->t_font_info.e_font_cmap == pt_font_info->e_font_cmap &&
               pt_rec->t_font_info.i2_width == pt_font_info->i2_width &&
               0 == x_strncmp(pt_rec->t_font_info.a_c_font_name, pt_font_info->a_c_font_name, WGL_MAX_FONT_NAME) &&
               0 == (_WGL_FONT_REC_FLAG_WITH_ATTR & pt_rec->ui2_flag))
            {
                break;
            }
        
            pt_rec = pt_rec->pt_next;
        }
    }
    else
    {
        pt_rec = NULL;
    }
    
    if(pt_rec) /*found*/
    {
        pt_rec->ui2_ref_count++;
        *ph_font_handle = pt_rec->h_font;
  
        i4_ret = WGLR_OK;

        if(ppt_ref_rec)
        {
            *ppt_ref_rec = pt_rec;
        }
    }
    else
    {
        do
        {
            /*get a new record*/
            pt_rec = pt_this->pt_free_head;
            if(NULL == pt_rec)
            {
                /*new a buffer*/
                i4_ret = _wgl_font_new_buf(pt_this);
                if(WGLR_OK != i4_ret)
                {
                    break;
                }
                
                pt_rec = pt_this->pt_free_head;
            }
            
            /*create font handle*/
            ps_font_name = (0 != x_strlen(pt_font_info->a_c_font_name)) ? pt_font_info->a_c_font_name : NULL;
            i4_ret = x_fe_create_font(ps_font_name, 
                                      pt_font_info->e_font_size, 
                                      pt_font_info->e_font_style, 
                                      pt_font_info->e_font_cmap, 
                                      &pt_rec->h_font);
            if(FER_OK != i4_ret)
            {
                DBG_ERROR(("{WGL(FONT)} ERR: x_fe_create_font() failed in %s().i4_ret=%d\n\n", __FUNCTION__, i4_ret));
                i4_ret = WGLR_SYS_RC_CREATE_FAIL;
                pt_rec->h_font = NULL_HANDLE;
                break;
            }
            if( pt_font_info->e_font_size == FE_FNT_SIZE_CUSTOM )
            {
              i4_ret = x_fe_set_custom_size(pt_rec->h_font,pt_font_info->ui1_custom_size);
              if(FER_OK != i4_ret)
              {
                DBG_ERROR(("{WGL(FONT)} ERR: x_fe_set_custom_size() failed in %s().\n\n", __FUNCTION__));
                /*free font handle*/
                i4_ret = x_handle_free(pt_rec->h_font);
                WGL_ASSERT(HR_OK == i4_ret);
                i4_ret = WGLR_INTERNAL_ERROR;
                pt_rec->h_font = NULL_HANDLE;
                break;
              }
            }
            /*init rec*/
            x_memcpy(&pt_rec->t_font_info, pt_font_info, sizeof(WGL_FONT_INFO_T));
            pt_rec->ui2_ref_count = 1;
            
            /*remove from free list*/
            pt_this->pt_free_head = pt_rec->pt_next;
            
            /*add to used list*/
            pt_rec->pt_next = pt_this->pt_used_head;
            pt_this->pt_used_head = pt_rec;
            
            /**/
            *ph_font_handle = pt_rec->h_font;
            i4_ret = WGLR_OK;

            if(ppt_ref_rec)
            {
                *ppt_ref_rec = pt_rec;
            }
            
        } while(0);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: wgl_font_remove_rec
 *
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 wgl_font_remove_rec(
    _WGL_FONT_MGR_T*            pt_this,
    HANDLE_T                    h_font_handle)
{
    _WGL_FONT_REC_T             *pt_rec, *pt_prev_rec, *pt_new_next;
    INT32                       i4_ret;
    
    pt_prev_rec = NULL;
    pt_rec = pt_this->pt_used_head;
    while(pt_rec)
    {
        if(pt_rec->h_font == h_font_handle)
        {
            break;
        }
        
        pt_prev_rec = pt_rec;
        pt_rec = pt_rec->pt_next;
    }
    
    if(pt_rec)
    {
        if(pt_rec->ui2_ref_count > 1)
        {
            pt_rec->ui2_ref_count--;
        }
        else /*last reference*/
        {
            /*remove from used list*/
            if(pt_prev_rec)
            {
                pt_prev_rec->pt_next = pt_rec->pt_next;
            }
            else /*head*/
            {
                pt_this->pt_used_head = pt_rec->pt_next;
            }
            
            /*add to free list*/
            pt_rec->pt_next = pt_this->pt_free_head;
            pt_this->pt_free_head = pt_rec;
            
            /*free font handle*/
            i4_ret = x_handle_free(pt_rec->h_font);
            WGL_ASSERT(HR_OK == i4_ret);
            
            /*zero record*/
            pt_new_next = pt_rec->pt_next;
            x_memset(pt_rec, 0, sizeof(_WGL_FONT_REC_T));
            pt_rec->pt_next = pt_new_next;
        }
        
        i4_ret = WGLR_OK;
    }
    else
    {
        DBG_ERROR(("{WGL(FONT)} ERR: WGL's Font manager cannot find the font handle %d.\n\n", h_font_handle));
        i4_ret = WGLR_INV_HANDLE;
        WGL_ASSERT(0);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _wgl_font_new_buf
 *
 * Description: 
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _wgl_font_new_buf(
    _WGL_FONT_MGR_T*           pt_this)
{
    _WGL_FONT_REC_T*           pt_rec;
    SIZE_T                     z_size;
    INT32                      i4_ret, i, i4_new_buf;
    
    
    if(pt_this->ui1_next_free_buf >= _WGL_FONT_BUF_NUM)
    {
        return WGLR_OVER_SUPPORT_LIMIT;    
    }
    
    do
    {
        /**/
        i4_new_buf = pt_this->ui1_next_free_buf;
        z_size = pt_this->ui2_grow_num * sizeof(_WGL_FONT_REC_T);
        pt_this->at_buf[i4_new_buf] = (_WGL_FONT_REC_T*) WGL_MEM_ALLOC(z_size);
        if(NULL == pt_this->at_buf[i4_new_buf])
        {
            DBG_ERROR(("{WGL(FONT)} ERR: WGL_MEM_ALLOC(size=%d) failed in %s()\n\n", z_size, __FUNCTION__));
            i4_ret = WGLR_OUT_OF_MEMORY;
            break;
        }
        
        /*zero memory*/
        x_memset(pt_this->at_buf[i4_new_buf], 0, z_size);
        
        /**/
        pt_rec = pt_this->at_buf[i4_new_buf];
        for(i = 0; i < (INT32) pt_this->ui2_grow_num; i++, pt_rec++)
        {
            pt_rec->pt_next = pt_rec + 1;
        }
        
        /*append to the head of the free list*/
        (pt_this->at_buf[i4_new_buf] + pt_this->ui2_grow_num - 1)->pt_next = pt_this->pt_free_head;
        
        /**/
        pt_this->pt_free_head = pt_this->at_buf[i4_new_buf];
        pt_this->ui1_next_free_buf++;
        i4_ret = WGLR_OK;
        
    } while(0);
    
    return i4_ret;
}
