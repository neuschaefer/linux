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
 * $SWAuthor: Jun Zuo$
 *
 * Description: 
 *         This file implement the graphics library which are exported.
 *---------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    include files and values declare
 ----------------------------------------------------------------------------*/
#if 0
#include <directfb.h>
#if !defined(__UCLIBC__)
#include <stropts.h>
#endif
#else

#include "gl_dfb_sw_draw.h"

#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
/*#include "_gl.h"*/

#include "gl_dfb.h"
#include "gl_dfb_surf.h"
#include "gl_dfb_pal.h"
#include "gl_dfb_clipper.h"

#include "mtpmx.h"

/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/

 /* Name: x_gl_init
 *
 * Description: This API initializes the Graphics Library. Note that this API 
 *              should be called before invoking any other exported GL APIs.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: GLR_OK             Routine successful.
 *          GLR_INIT_FAILED    Initialization failed.
 *          GLR_ALREADY_INIT   The Graphics Library has already been initialized.
 ----------------------------------------------------------------------------*/
 
INT32 x_gl_get_gpu_capability(
    GL_GPU_CAPS_T       *pt_caps)
{
#if 1 /* MHF - Linux Basara for mmp */    
#define GFX_SCALING_MIN_W   (UINT32)4 
#define GFX_SCALING_MIN_H   (UINT32)1
#define GFX_SCALING_MAX_W   (UINT32)4095 
#define GFX_SCALING_MAX_H   (UINT32)4095

    INT32 i4_ret = 0;

    switch (pt_caps->ui4_type)
    {
        case GL_GPU_SCALING_LIMIT:
            pt_caps->u.t_scaling_limit.ui4_max_width  = GFX_SCALING_MAX_W;
            pt_caps->u.t_scaling_limit.ui4_min_width  = GFX_SCALING_MIN_W;
            pt_caps->u.t_scaling_limit.ui4_max_height = GFX_SCALING_MAX_H;
            pt_caps->u.t_scaling_limit.ui4_min_height = GFX_SCALING_MIN_H;
            break;

        default:
            i4_ret = GLR_INV_ARG;
            break;
    }
            
    return i4_ret;
#else
    return 0;
#endif
} 
 
INT32 x_gl_init(GL_COLORMODE_T e_colormode, UINT32 ui4_width, UINT32 ui4_height)
{
	return gl_dfb_init(e_colormode, ui4_width, ui4_height);
}	

/* obj component API */
INT32 x_gl_obj_free(GL_HOBJ_T h_obj)
{
    gl_dfb_free_handle(h_obj);
	return GLR_OK;
}

/* screen component API */
INT32 x_gl_screen_enable(
    HANDLE_T    h_screen,
    BOOL        b_enable)
{
	GL_DFB_TRACE_LINE;
	return gl_dfb_screen_enable(h_screen, b_enable);
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_screen_get_status
 *
 * Description: This API retrieves the status of the screen object.
 *
 * Inputs:  h_screen        Contains the handle of the specified screen object. 
 *
 * Outputs: pt_scr_status   References a GL_SCREEN_STATUS_T structure containing the 
 *                          status of the screen object if the routine was 
 *                          successful. In all other cases the value of this 
 *                          argument is undetermined.
 *
 * Returns: GLR_OK                  The routine was successful and the operation is 
 *                                  complete.
 *          GLR_NOT_INIT            The Graphics Library is not initialized properly.
 *          GLR_INV_ARG             pt_scr_status is NULL.
 *          GLR_INV_HANDLE          The screen handle is invalid.
 *          GLR_DRV_ERROR           Error returned while accessing driver components 
 *                                  through the Resource Manager.
 *          GLR_INCOMPATIBLE_OBJ    The object referenced by the screen handle is 
 *                                  not a valid screen object.
----------------------------------------------------------------------------*/
INT32 x_gl_screen_get_status(
    GL_HSCREEN_T        h_screen, 
    GL_SCREEN_STATUS_T* pt_scr_status)
{
    
	GL_DFB_TRACE_LINE;

    return gl_dfb_screen_get_status(h_screen, pt_scr_status);
	
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_screen_open
 *
 * Description: This API creates a screen object and returns its handle.
 *
 * Inputs:  ps_name         Contains the name of the screen. 
 *          pf_screen_nfy   Contains the screen notify function. This function 
 *                          is called whenever the state of the corresponding 
 *                          display device is changed. This argument may be set 
 *                          to NULL.
 *          pv_nfy_tag      Contains a private tag, which is returned in the notify 
 *                          function. This argument may be set to NULL.
 *
 * Outputs: ph_screen       References the returned screen handle if the routine 
 *                          was successful. In all other cases the value of this 
 *                          argument is undetermined.
 *
 * Returns: GLR_OK              The routine was successful and the operation is complete.
 *          GLR_NOT_INIT        The Graphics Library is not initialized properly.
 *          GLR_INV_ARG         One or more arguments passed in are invalid.
 *          GLR_NO_MEMORY       There's not enough memory available to accomplish the 
 *                              operation.
 *          GLR_OUT_OF_HANDLES  There's no handle available.
 *          GLR_DRV_ERROR       Error returned while accessing driver components 
 *                              through the Resource Manager.
----------------------------------------------------------------------------*/
INT32 x_gl_screen_open(
    const CHAR*         ps_name,
    x_gl_nfy_fct        pf_screen_nfy,
    VOID*               pv_nfy_tag,
    GL_HSCREEN_T*       ph_screen)
{
    GL_DFB_TRACE_LINE;

    return gl_dfb_screen_open(ps_name, pf_screen_nfy, pv_nfy_tag, ph_screen);
}	

/* plane component API */
INT32 x_gl_plane_config(
        HANDLE_T            h_plane,
    GL_PLANE_CFG_T*     pt_pln_cfg)
{
    GL_DFB_TRACE_LINE;

    
    return gl_dfb_plane_config(h_plane, pt_pln_cfg);
}	

INT32 x_gl_plane_num(
    UINT16*             pui2_num)
{
	GL_DFB_TRACE_LINE;
    
    
    return gl_dfb_plane_num(pui2_num);
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_enum
 *
 * Description: This API is called by clients to iteratively enumerate OSD 
 *              planes that can be linked to the specified screen device. The 
 *              capability of the ith OSD plane is returned.
 *                
 * Inputs:  h_screen    Contains the handle of the screen device that the OSD 
 *                      planes being queried can be linked to.
 *          ui2_idx     Contains the index of the OSD plane that clients want 
 *                      to query. The legal range of this value starts from 0 
 *                      to the maximum number of available OSD planes of the 
 *                      specified screen device minus 1.
 *
 * Outputs: pt_plane_caps   Contains the capabilities of the OSD plane if the 
 *                          routine was successful. In all other cases, the 
 *                          value of this argument is undetermined
 *
 * Returns: GLR_OK                  The routine was successful and the operation 
 *                                  has been completed.
 *          GLR_NOT_INIT            The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE          The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ    The object referenced by the plane handle is not 
 *                                  a valid plane object.
 *          GLR_INV_ARG             The value specified in the field e_reorder_type 
 *                                  is invalid.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_enum(
    GL_HSCREEN_T        h_screen, 
    UINT16              ui2_idx, 
    GL_PLANE_CAPS_T*    pt_pln_caps)
{
	GL_DFB_TRACE_LINE;
   
    return gl_dfb_plane_enum(h_screen, ui2_idx, pt_pln_caps);
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_open
 *
 * Description: This API opens the OSD plane on the specified screen device and 
 *              returns a corresponding plane handle according to the index 
 *              value. If the specified OSD plane is not capable of displaying 
 *              on multiple screen devices simultaneously and has already been 
 *              opened on another screen device, this routine fails..
 *                
 * Inputs:  h_screen    Contains the handle of the screen device that the OSD 
 *                      planes being queried can be linked to.
 *          ui2_idx     Contains the index of the OSD plane that clients want 
 *                      to query. The legal range of this value starts from 0 
 *                      to the maximum number of available OSD planes of the 
 *                      specified screen device minus 1.
 *
 * Outputs: ph_plane    References the returned plane handle if the routine was 
 *                      successful. In all other cases the value of this argument 
 *                      is undetermined.
 *
 * Returns: GLR_OK                  The routine was successful and the operation 
 *                                  has been completed.
 *          GLR_NOT_INIT            The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE          The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ    The object referenced by the plane handle is not 
 *                                  a valid plane object.
 *          GLR_INV_ARG             The value specified in the field e_reorder_type 
 *                                  is invalid.
 *          GLR_NO_MEMORY           There is not enough memory available to perform 
 *                                  the operation.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_open(
    GL_HSCREEN_T        h_screen, 
    UINT16              ui2_idx,
    GL_HPLANE_T*        ph_plane)
{
	GL_DFB_TRACE_LINE;
   
    return gl_dfb_plane_open(h_screen, ui2_idx, ph_plane);
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_link
 *
 * Description: This API links the plane to the specified screen device. The 
 *              originally linked screen of the plane might be replaced by the 
 *              new one.
 *                
 * Inputs:  h_plane     Contains the handle of the plane object being reordered.
 *          h_screen    Contains the handle of the screen device that the OSD 
 *                      planes being queried can be linked to. This field can 
 *                      be NULL_HANDLE and indicates removing any linkage present.
 *          b_forced    Contains a Boolean value to indicate whether a 
 *                      replacement should be made under the circumstances that 
 *                      the plane has already been linked to another screen.
 *
 * Outputs: pt_plane_caps   Contains the capabilities of the OSD plane if the 
 *                          routine was successful. In all other cases, the 
 *                          value of this argument is undetermined
 *
 * Returns: GLR_OK                      The routine was successful and the operation 
 *                                      has been completed.
 *          GLR_NOT_INIT                The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE              The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ        The object referenced by the plane handle is not 
 *                                      a valid plane object.
 *          GLR_INV_ARG                 The value specified in the field e_reorder_type 
 *                                      is invalid.
 *          GLR_PLANE_ALREADY_LINKED    The plane specified has already been linked to 
 *                                      another screen and b_forced is FALSE.
 *          GLR_DRV_ERROR               Either the Resource Manager or the driver 
 *                                      components being accessed return failure.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_link(
    GL_HPLANE_T         h_plane,
    GL_HSCREEN_T        h_screen,
    BOOL                b_forced)
{
	
    GL_DFB_TRACE_LINE;
    GL_TRACE_API("x_gl_plane_link, h_plane = %x, h_screen = %x\n",h_plane,h_screen);
   
	return gl_dfb_plane_link(h_plane, h_screen, b_forced);
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_reorder
 *
 * Description: This API changes the z order of the specified plane.
 *                
 * Inputs:  h_plane         Contains the handle of the specified plane object.
 *          e_reorder_type  Contains the reordering type.
 *          i4_param        Contains the value for ordering adjustment. If 
 *                          e_reorder_type is specified with GL_REORDER_ABSOLUTE, 
 *                          this field represents the new z order of the plane 
 *                          (0 denotes the topmost level). If e_reorder_type is 
 *                          specified with GL_REORDER_RELATIVE, i4_param represents 
 *                          the offset from the plane's current z order.
 *
 * Outputs: -
 *
 * Returns: GLR_OK                  The routine was successful and the operation 
 *                                  has been completed.
 *          GLR_NOT_INIT            The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE          The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ    The object referenced by the plane handle is not 
 *                                  a valid plane object.
 *          GLR_INV_ARG             The value specified in the field e_reorder_type 
 *                                  is invalid.
 *          GLR_NO_MEMORY           There is not enough memory available to perform 
 *                                  the operation.
 *          GLR_PLANE_NOT_LINKED    The plane does not link to any screen device 
 *                                  and hence cannot be reordered.
 *          GLR_DRV_ERROR           Either the Resource Manager or the driver 
 *                                  components being accessed return error.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_reorder(
    GL_HPLANE_T         h_plane, 
    UINT32              e_reorder_type, 
    INT32               i4_param)
{
    GL_DFB_TRACE_LINE;	   

	return gl_dfb_plane_reorder(h_plane, e_reorder_type, i4_param);
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_enable
 *
 * Description: This API enables/disables the specified plane.
 *                
 * Inputs:  h_plane     Contains the handle of the plane object being reordered.
 *          b_enable    Contains a Boolean value indicating enabling or 
 *                      disabling the plane.
 *
 * Outputs: -
 *
 * Returns: GLR_OK                      The routine was successful and the operation 
 *                                      has been completed.
 *          GLR_NOT_INIT                The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE              The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ        The object referenced by the plane handle is not 
 *                                      a valid plane object.
 *          GLR_DRV_ERROR               Either the Resource Manager or the driver 
 *                                      components being accessed return failure.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_enable(
    GL_HPLANE_T         h_plane,
    BOOL                b_enable)
{
	GL_DFB_TRACE("x_gl_plane_enable\n");
	
	return gl_dfb_plane_enable(
        h_plane,
        b_enable);
}	

INT32 x_gl_plane_clear(GL_HPLANE_T         h_plane)
{
    return gl_dfb_plane_clear(h_plane);
}
INT32 x_gl_plane_change_resolution(
        GL_HPLANE_T h_plane,
        UINT32      ui4_width,
        UINT32      ui4_height)
{
    return gl_dfb_plane_change_resolution(
        h_plane,
        ui4_width,
        ui4_height);
}

INT32 x_gl_plane_change_resolution_cm(
        GL_HPLANE_T    h_plane,
        UINT32         ui4_width,
        UINT32         ui4_height,
        GL_COLORMODE_T e_cm)
{
    return gl_dfb_plane_change_resolution_cm(
           h_plane,
           ui4_width,
           ui4_height,
           e_cm);
}
/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_flip
 *
 * Description: This API makes the specified view the active one of the plane.
 *                
 * Inputs:  h_plane     Contains the handle of the specified plane object.
 *          h_view      Contains the handle of the view object.
 *          b_forced    Contains a Boolean value to indicate whether a replacement 
 *                      should be made under the circumstances that the plane has 
 *                      an active view already.
 *
 * Outputs: -
 *
 * Returns: GLR_OK                  The routine was successful and the operation 
 *                                  has been completed.
 *          GLR_NOT_INIT            The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE          The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ    The object referenced by the plane handle is not 
 *                                  a valid plane object.
 *          GLR_FLIP_FAILED         Flipping views failed.
 *          GLR_OUT_OF_HANDLES      There's no handle available.
 *          GLR_DRV_ERROR           Error returned while accessing driver 
 *                                  components through the Resource Manager.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_flip(
    GL_HPLANE_T     h_plane,
    GL_HVIEW_T      h_view,
    BOOL            b_forced)
{
	GL_DFB_TRACE_LINE;
	
    return gl_dfb_plane_flip(
        h_plane,
        h_view,
        b_forced);
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_get_capability
 *
 * Description: This API retrieves the capability of the specified plane. Note 
 *              that the plane capability may vary depending on which screen 
 *              device it links to, so the handle of the screen device must be 
 *              also specified.
 *                
 * Inputs:  h_plane         Contains the handle of the specified plane object.
 *          h_screen        Contains the handle of the screen device that the 
 *                          OSD planes being queried can be linked to. Depending 
 *                          on the hardware configuration, the returned capability 
 *                          of the same plane may vary while being linked to 
 *                          different screen devices.
 *
 * Outputs: pt_pln_caps     References a structure of type GL_PLANE_CAPS_T if 
 *                          the routine was successful. In all other cases, 
 *                          the content of the structure is undetermined.
 *
 * Returns: GLR_OK                  The routine was successful and the operation 
 *                                  has been completed.
 *          GLR_NOT_INIT            The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE          The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ    The object referenced by the plane handle is not 
 *                                  a valid plane object.
 *          GLR_INV_ARG             The value specified in the field e_reorder_type 
 *                                  is invalid.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_get_capability(
    GL_HPLANE_T         h_plane, 
    GL_HSCREEN_T        h_screen,
    GL_PLANE_CAPS_T*    pt_pln_caps)
{
	GL_DFB_TRACE_LINE;
	
	return  gl_dfb_plane_get_capability(
        h_plane, 
        h_screen,
        pt_pln_caps);
}	

INT32 x_gl_plane_get_linked_screen(
    GL_HPLANE_T         h_plane, 
    x_gl_nfy_fct        pf_screen_nfy,
    VOID*               pv_nfy_tag,
    GL_HSCREEN_T*       ph_screen)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_plane_get_linked_screen(
        h_plane, 
        pf_screen_nfy,
        pv_nfy_tag,
        ph_screen);
}	

INT32 x_gl_plane_get_order(
    GL_HPLANE_T         h_plane, 
    INT32               *pi4_order)
{
    return gl_dfb_plane_get_order(
        h_plane,
        pi4_order);
}

/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_get_status
 *
 * Description: This API retrieves the current status the specified plane.
 *                
 * Inputs:  h_plane     Contains the handle of the specified plane object.
 *          h_view      Contains the handle of the view object.
 *          b_forced    Contains a Boolean value to indicate whether a replacement 
 *                      should be made under the circumstances that the plane has 
 *                      an active view already.
 *
 * Outputs: pt_pln_sts  References a structure of type GL_PLANE_STATUS_T if the 
 *                      routine was successful. In all other cases, the content 
 *                      of the structure is undetermined.
 *
 * Returns: GLR_OK                  The routine was successful and the operation 
 *                                  has been completed.
 *          GLR_NOT_INIT            The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE          The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ    The object referenced by the plane handle is not 
 *                                  a valid plane object.
 *          GLR_INV_ARG             The value specified in the field e_reorder_type 
 *                                  is invalid.
 *          GLR_DRV_ERROR           Error returned while accessing driver 
 *                                  components through the Resource Manager.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_get_status(
    GL_HPLANE_T         h_plane, 
    GL_PLANE_STATUS_T*  pt_pln_sts)
{
	GL_DFB_TRACE_LINE;
	
    return gl_dfb_plane_get_status(
        h_plane, 
        pt_pln_sts);
}	

/*-----------------------------------------------------------------------------
 * Name: x_gl_plane_set_alpha
 *
 * Description: This API changes the z order of the specified plane.
 *                
 * Inputs:  h_plane         Contains the handle of the specified plane object.
 *          ui1_alpha       Contains the alpha value of the plane while mixing 
 *                          multiple OSD planes together.
 *
 * Outputs: -
 *
 * Returns: GLR_OK                  The routine was successful and the operation 
 *                                  has been completed.
 *          GLR_NOT_INIT            The Graphics Library is not initialized properly.
 *          GLR_INV_HANDLE          The plane handle is invalid.
 *          GLR_INCOMPATIBLE_OBJ    The object referenced by the plane handle is not 
 *                                  a valid plane object.
 *          GLR_INV_ARG             The value specified in the field e_reorder_type 
 *                                  is invalid.
 *          GLR_DRV_ERROR           Either the Resource Manager or the driver 
 *                                  components being accessed return error.
 ----------------------------------------------------------------------------*/
INT32 x_gl_plane_set_alpha(
    GL_HPLANE_T         h_plane,
    UINT8               ui1_alpha)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_plane_set_alpha(
        h_plane,
        ui1_alpha);
}	

INT32 x_gl_plane_enable_colorkey(
	GL_HPLANE_T         h_plane,
    BOOL                b_new_enable,
    BOOL*               old_enable_fg)
{
	return gl_dfb_plane_enable_colorkey(h_plane, b_new_enable, old_enable_fg);
}

#ifdef MW_PHOTO_SHOW_ON_VDP
INT32 x_gl_plane_get_resource(HANDLE_T h_plane,HANDLE_T h_on_screen_surf)
{
   return gl_dfb_plane_get_resource( h_plane, h_on_screen_surf);
}
INT32 x_gl_plane_release_resource(HANDLE_T h_plane,HANDLE_T h_on_screen_surf)
{
    return gl_dfb_plane_release_resource( h_plane, h_on_screen_surf);
}

INT32 x_gl_plane_set_vdp_cropping(
    GL_HPLANE_T h_plane, 
    BOOL        b_3d_mode,
    UINT32      ui4_bottom,
    UINT32      ui4_top,
    UINT32      ui4_left,
    UINT32      ui4_right)
{
    return gl_dfb_plane_set_vdp_cropping(h_plane,b_3d_mode,ui4_bottom,ui4_top,ui4_left,ui4_right);
}

INT32 x_gl_surface_create_from_vdp(
    const GL_SURFACE_DESC_T*    pt_surf_desc, 
    GL_HSURFACE_T*              ph_surf)
{
    return gl_dfb_surface_create_from_vdp(pt_surf_desc, ph_surf);
}

INT32 x_gl_surface_vdp_strech_blend(
    GL_HSURFACE_T       h_surf_dst, 
    GL_HSURFACE_T       h_surf_src_pic, 
    GL_HSURFACE_T       h_surf_src_black,
    UINT8               ui1_alpha)
{
	return gl_dfb_surface_vdp_strech_blend(h_surf_dst,h_surf_src_pic,h_surf_src_black,ui1_alpha);
}

#endif

/* view component API */
INT32 x_gl_view_create(
    UINT32                  ui4_w,
    UINT32                  ui4_h,
    BOOL                    b_auto_scale,
    GL_HVIEW_T*             ph_view)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_view_create(
        ui4_w,
        ui4_h,
        b_auto_scale,
        ph_view);
}	

INT32 x_gl_view_create_viewport(
    GL_HVIEW_T              h_view, 
    GL_HSURFACE_T           h_surface, 
    const GL_DISPLAY_OPT_T* pt_disp_opt,
    UINT32*                 pui4_id)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_view_create_viewport(
        h_view, 
        h_surface, 
        pt_disp_opt,
        pui4_id);
}	

INT32 x_gl_view_delete_viewport(
    GL_HVIEW_T      h_view,
    UINT32          ui4_id)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_view_delete_viewport(
        h_view,
        ui4_id);
}	

INT32 x_gl_view_reconfig_viewport(
    GL_HPLANE_T     h_plane,
    GL_HVIEW_T      h_view,
    UINT32          ui4_viewport_id,
    INT32           i4_view_x,
    INT32           i4_view_y,
    UINT32          ui4_width,
    UINT32          ui4_height,
    UINT32          ui4_surf_offset_x,
    UINT32          ui4_surf_offset_y,
    GL_FIX16_T      fx_shrink_x,
    GL_FIX16_T      fx_shrink_y)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_view_reconfig_viewport(
        h_plane,
        h_view,
        ui4_viewport_id,
        i4_view_x,
        i4_view_y,
        ui4_width,
        ui4_height,
        ui4_surf_offset_x,
        ui4_surf_offset_y,
        fx_shrink_x,
        fx_shrink_y);
}	

INT32 x_gl_surface_draw_polyline(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    UINT32              ui4_flag)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_surface_draw_polyline(
        h_surf,
        ui4_count, 
        at_vertex,
        pt_color,
        ui2_line_width,
        ui4_flag);
}	

INT32 x_gl_surface_pltblt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_BYTE_ALIGNMENT_T e_alignment,
    GL_HPALETTE_T       h_palette,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;
    /*return gl_dfb_surface_patblt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        e_alignment,
        h_palette,
        ui4_flag);	*/

    return GLR_OK;
}	

/* surface component API */
INT32 x_gl_surface_create(
    const GL_SURFACE_DESC_T*    pt_surf_desc, 
    GL_HSURFACE_T*              ph_surf)
{
	GL_DFB_TRACE_LINE;

	return gl_dfb_surface_create(pt_surf_desc, ph_surf);
}	

INT32 x_gl_surface_create_from_fbm(
    const GL_SURFACE_DESC_T*    pt_surf_desc, 
    GL_HSURFACE_T*              ph_surf)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_create_from_fbm(pt_surf_desc, ph_surf);
}	

INT32 x_gl_surface_lock(
    GL_HSURFACE_T       h_surf, 
    const GL_RECT_T*    pt_rect, 
    UINT32              ui4_flag, 
    GL_LOCK_INFO_T*     pt_lock_info)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_surface_lock(
        h_surf, 
        pt_rect, 
        ui4_flag, 
        pt_lock_info);
}	

INT32 x_gl_surface_unlock(
    GL_HSURFACE_T       h_surf, 
    const GL_RECT_T*    pt_rect)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_surface_unlock(
        h_surf, 
        pt_rect);
}	

INT32 x_gl_surface_get_info(
    GL_HSURFACE_T       h_surf, 
    GL_SURF_INFO_T*     pt_surf_info)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_surface_get_info(h_surf, pt_surf_info);
}	

INT32 x_gl_surface_blt_ex(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    const GL_BLT_EX_T*  pt_blt_ex,
    UINT8               ui1_blt_ex_opt,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;
	
	return GLR_OK;
}	
    
INT32 x_gl_surface_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;
	return gl_dfb_surface_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha_assigned,
        ui4_flag);
}	

INT32 x_gl_surface_flip(GL_HSURFACE_T       h_surf, UINT32              ui4_flag)
{
	return gl_dfb_surface_flip(h_surf, ui4_flag);
}	


INT32 x_gl_surface_rop_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_ROP_TYPE_T       e_rop_type,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_rop_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        e_rop_type,
        ui4_flag);
}	

INT32 x_gl_surface_ycbcr_to_rgb(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_y_surf,
    GL_HSURFACE_T       h_cbcr_surf,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    GL_YCBCR_MODE_T     e_ycbcr_mode,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return GLR_OK;
}	

INT32 x_gl_surface_ycbcr_to_rgb_3buf(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_y_surf,
    GL_HSURFACE_T       h_cb_surf,
    GL_HSURFACE_T       h_cr_surf,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    GL_JPEG_MODE_T      e_jpeg_mode,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return GLR_OK;
}	

INT32 x_gl_surface_draw_line(
    GL_HSURFACE_T       h_surf,
    const GL_POINT_T*   pt_start, 
    const GL_POINT_T*   pt_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_draw_line(
        h_surf,
        pt_start, 
        pt_end,
        pt_color,
        ui2_line_width,
        ui4_flag);
}	

INT32 x_gl_surface_draw_rect(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_draw_rect(
        h_surf,
        pt_rect,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag);
}	

INT32 x_gl_surface_draw_round_rect(
    GL_HSURFACE_T           h_surf,
    const GL_RECT_T*        pt_rect,
    const GL_ARC_DESC_T*    pt_arc,
    const GL_COLOR_T*       pt_color,
    UINT16                  ui2_line_width,
    GL_STYLE_T              t_style,
    UINT32                  ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_draw_round_rect(
        h_surf,
        pt_rect,
        pt_arc,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag);
}	

INT32 x_gl_surface_draw_ellipse(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_draw_ellipse(
        h_surf,
        pt_rect,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag);
}	

INT32 x_gl_surface_draw_polygon(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_count, 
    const GL_POINT_T    at_vertex[],
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_draw_polygon(
        h_surf,
        ui4_count, 
        at_vertex,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag);
}	

INT32 x_gl_surface_draw_arc(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_draw_arc(
        h_surf,
        pt_rect,
        i4_deg_start, 
        i4_deg_end,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag);
}	

INT32 x_gl_surface_draw_pie(
    GL_HSURFACE_T       h_surf,
    const GL_RECT_T*    pt_rect,
    INT32               i4_deg_start, 
    INT32               i4_deg_end,
    const GL_COLOR_T*   pt_color,
    UINT16              ui2_line_width,
    GL_STYLE_T          t_style,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_draw_pie(
        h_surf,
        pt_rect,
        i4_deg_start, 
        i4_deg_end,
        pt_color,
        ui2_line_width,
        t_style,
        ui4_flag);
}	

INT32 x_gl_surface_patblt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_pat_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        ui1_alpha_assigned,
        ui4_flag);
}	

INT32 x_gl_surface_rotate_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    UINT32              ui4_rot_op,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_rotate_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha_assigned,
        ui4_rot_op,
        ui4_flag);
}	

INT32 x_gl_surface_stretch_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_stretch_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        ui4_flag);
}	

INT32 x_gl_surface_adv_stretch_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;
    return gl_dfb_surface_stretch_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        ui4_flag);
	/*return GLR_OK;*/
}	

INT32 x_gl_surface_transp_blt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_transp_blt(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha_assigned,
        b_off_mask,
        pt_color_min,
        pt_color_max,
        ui4_flag);
}	

INT32 x_gl_surface_transp_patblt(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_alpha_assigned,
    BOOL                b_off_mask,
    const GL_COLOR_T*   pt_color_min,
    const GL_COLOR_T*   pt_color_max,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE("[X_GL_API] %s, \
dst:%d, %d, %d, %d, \
src:%d, %d, %d, %d, \
alpha:%d, b_off_mask:%d, \
colormin:%d, colormax:%d\n",

            __FUNCTION__,
            i4_dst_x,
            i4_dst_y,
            ui4_dst_w,
            ui4_dst_h,
            i4_src_x,
            i4_src_y,
            ui4_src_w,
            ui4_src_h,
            ui1_alpha_assigned,
            b_off_mask,
            GL_COLOR_ARGB8888(pt_color_min->a, pt_color_min->u1.r, pt_color_min->u2.g, pt_color_min->u3.b),
            GL_COLOR_ARGB8888(pt_color_max->a, pt_color_max->u1.r, pt_color_max->u2.g, pt_color_max->u3.b));

    return gl_dfb_surface_transp_pat_blt(
            h_dst_surf, 
            h_src_surf, 
            i4_dst_x,
            i4_dst_y,
            ui4_dst_w,
            ui4_dst_h,
            i4_src_x,
            i4_src_y,
            ui4_src_w,
            ui4_src_h,
            ui1_alpha_assigned,
            b_off_mask,
            pt_color_min,
            pt_color_max,
            ui4_flag);

	return GLR_OK;
}	

INT32 x_gl_surface_blend(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_blend(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_alpha,
        ui4_flag);
}	

INT32 x_gl_surface_compose(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_surface_compose(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        i4_src_x,
        i4_src_y,
        ui4_width,
        ui4_height,
        ui1_pre_alpha,
        e_rule,
        ui4_flag);
}	

INT32 x_gl_surface_pat_compose(
    GL_HSURFACE_T       h_dst_surf, 
    GL_HSURFACE_T       h_src_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_dst_w,
    UINT32              ui4_dst_h,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_src_w,
    UINT32              ui4_src_h,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

    return gl_dfb_surface_pat_compose(
        h_dst_surf, 
        h_src_surf, 
        i4_dst_x,
        i4_dst_y,
        ui4_dst_w,
        ui4_dst_h,
        i4_src_x,
        i4_src_y,
        ui4_src_w,
        ui4_src_h,
        ui1_pre_alpha,
        e_rule,
        ui4_flag);
	return GLR_OK;
}	

INT32 x_gl_surface_rect_src_compose(
    GL_HSURFACE_T       h_dst_surf, 
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_pre_alpha,
    GL_COMPOSE_RULE_T   e_rule,
    GL_COLORMODE_T      e_src_colormode,
    GL_COLOR_T          *pt_src_color,
    UINT32              ui4_flag)
{
    return gl_dfb_surface_fillrect_blend(   h_dst_surf, 
                                            i4_dst_x,
                                            i4_dst_y,
                                            ui4_width,
                                            ui4_height,
                                            ui1_pre_alpha,
                                            e_rule,
                                            e_src_colormode,
                                            pt_src_color,
                                            ui4_flag);
}	

/*INT32 x_gl_surface_color_compose(
    GL_HSURFACE_T       h_surf,
    const GL_COLOR_T*   pt_color,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COMPOSE_RULE_T   e_rule,
    UINT32              ui4_flag)
{
	return GLR_OK;
}	

INT32 x_gl_surface_draw_uchars(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T             aw2_chars[],
    UINT16              ui2_len,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
	GL_RECT_T*          pt_rc_str)
{
	return GLR_OK;
}*/	

#if defined (GL_NO_MW_FONT)	

INT32 x_gl_surface_draw_ustring(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
	GL_RECT_T*          pt_rc_str)
{
    GL_DFB_TRACE_LINE;

    gl_dfb_surface_draw_ustring(
        h_surf, 
        i4_x,
        i4_y,
        w2s_str,
        ui2_strlen,
        h_fnt,
        pt_fg_clr,
        pt_bg_clr,
        pt_rc_str);
    return GLR_OK;
}	
 INT32 x_gl_surface_draw_ustring_ex(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    INT32               i4_bg_y,
    VOID*               pv_str,
    UINT16              ui2_strlen,
    VOID*               pv_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    const GL_COLOR_T*   pt_bg_clr,
    const GL_COLOR_T*   pt_edge_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    UINT16              ui2_bg_h,
    INT32               i4_bg_adj_x,
    GL_DRAW_STR_MODE_T  e_draw_str_mode)
{
    GL_DFB_TRACE_LINE;

    gl_dfb_surface_draw_ustring(
        h_surf, 
        i4_x,
        i4_y,
        pv_str,
        ui2_strlen,
        h_fnt,
        pt_fg_clr,
        pt_bg_clr,
        NULL);
	return GLR_OK;
}	
INT32 x_gl_surface_draw_subset_ustring(
    GL_HSURFACE_T       h_surf,
    INT32               i4_x,
    INT32               i4_y,
    UTF16_T*            w2s_str,
    UINT16              ui2_strlen,
    UTF16_T*            w2s_skip,
    UINT16              ui2_skiplen,
    HFONT_T             h_fnt,
    const GL_COLOR_T*   pt_fg_clr,
    BOOL                b_fixed,
    UINT16              ui2_ch_w,
    UINT16              ui2_skip_w,
    GL_DRAW_STR_MODE_T  e_draw_str_mode,
    UINT16              ui2_index_num,
    UINT16*             aui2_index)
{
    GL_DFB_TRACE_LINE;

    return GLR_OK;
}


#endif

INT32 x_gl_surface_fill(
    GL_HSURFACE_T       h_surf, 
    const GL_COLOR_T*   pt_color,
    UINT32              ui4_flag)
{
	GL_DFB_TRACE_LINE;
	gl_dfb_surface_fill(h_surf, pt_color, ui4_flag);
	return GLR_OK;
}	

INT32 x_gl_surface_reconfig(
    GL_HSURFACE_T       h_surf,
    UINT32              ui4_width,
    UINT32              ui4_height,
    GL_COLORMODE_T      e_mode)
{
    GL_DFB_TRACE_LINE;
    
	return gl_dfb_surface_reconfig(h_surf, ui4_width, ui4_height, e_mode);
}	

INT32 x_gl_surface_set_palette(
    GL_HSURFACE_T       h_surf, 
    GL_HPALETTE_T       h_palette)
{
	GL_DFB_TRACE_LINE;
	
	return gl_dfb_surface_set_palette(
        h_surf, 
        h_palette);
}	

INT32 x_gl_surface_set_clipper(
    GL_HSURFACE_T       h_surf, 
    GL_HCLIPPER_T       h_clipper)
{
	GL_DFB_TRACE_LINE;
		

	return gl_dfb_surface_set_clipper(
        h_surf, 
        h_clipper);
}	

INT32 x_gl_surface_replace_color_blt(
    GL_HSURFACE_T       h_dst_surf,
    GL_HSURFACE_T       h_src_surf,
    INT32               i4_dst_x,
    INT32               i4_dst_y,
    INT32               i4_src_x,
    INT32               i4_src_y,
    UINT32              ui4_width,
    UINT32              ui4_height,
    UINT8               ui1_alpha_assigned,
    GL_COLOR_T          at_original_color[],
    GL_COLOR_T          at_new_color[],
    UINT8               ui1_color_num,
    UINT32              ui4_flag)
{
    GL_DFB_TRACE_LINE;

	return GLR_OK;
}	

/* clipper component API */
INT32 x_gl_clipper_create(
    UINT16              ui2_count, 
    const GL_RECT_T*    pt_rect, 
    GL_HCLIPPER_T*      ph_clipper)
{

	GL_DFB_TRACE_LINE;
	
	return gl_dfb_clipper_create(
        ui2_count, 
        pt_rect, 
        ph_clipper);
}	

INT32 x_gl_clipper_set_clip_list(
    GL_HCLIPPER_T       h_clipper, 
    UINT16              ui2_count, 
    const GL_RECT_T*    pt_rect)
{
	GL_DFB_TRACE_LINE;
	return gl_dfb_clipper_set_clip_list(
        h_clipper, 
        ui2_count, 
        pt_rect);
}

INT32 x_gl_clipper_get_clip_list(
    GL_HCLIPPER_T       h_clipper, 
    UINT16*             pui2_count, 
    GL_RECT_T*          pt_rect)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_clipper_get_clip_list(
        h_clipper, 
        pui2_count, 
        pt_rect);
}	

/* palette component API */
INT32 x_gl_palette_create(
    GL_COLORMODE_T      e_clrmode, 
    UINT16              ui2_count,
    const GL_COLOR_T    at_color[], 
    GL_HPALETTE_T*      ph_palette)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_palette_create(
        e_clrmode, 
        ui2_count,
        at_color,
        ph_palette); 
}	

INT32 x_gl_palette_create_with_id(
    GL_COLORMODE_T      e_clrmode, 
    UINT16              ui2_count,
    const GL_COLOR_T    at_color[], 
    UINT32              ui4_id,
    GL_HPALETTE_T*      ph_palette)
{
	GL_DFB_TRACE_LINE;
	
    return gl_dfb_palette_create(
        e_clrmode, 
        ui2_count,
        at_color,
        ph_palette); 
}

INT32 x_gl_palette_set_entries(
    GL_HPALETTE_T       h_palette, 
    UINT16              ui2_base, 
    UINT16              ui2_count, 
    const GL_COLOR_T    at_color[])
{
    GL_DFB_TRACE_LINE;

    return gl_dfb_palette_set_entries(
        h_palette, 
        ui2_base, 
        ui2_count, 
        at_color);
}	

INT32 x_gl_palette_get_entries(
    GL_HPALETTE_T       h_palette, 
    UINT16              ui2_base, 
    UINT16              ui2_count, 
    GL_COLOR_T          at_color[])
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_palette_get_entries(
        h_palette, 
        ui2_base, 
        ui2_count, 
        at_color);
}	

INT32 x_gl_palette_match_color(
    GL_HPALETTE_T       h_palette,
    BOOL                b_cmp_alpha,
    const GL_COLOR_T*   pt_to_match,
    GL_COLOR_T*         pt_matched,
    UINT16*             pui2_index)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_palette_match_color(
        h_palette,
        b_cmp_alpha,
        pt_to_match,
        pt_matched,
        pui2_index);
}	

INT32 x_gl_palette_match_color_ex(
    GL_HPALETTE_T       h_palette,
    UINT16              ui2_base,
    UINT16              ui2_count,
    BOOL                b_cmp_alpha,
    const GL_COLOR_T*   pt_to_match,
    GL_COLOR_T*         pt_matched,
    UINT16*             pui2_index)
{
    GL_DFB_TRACE_LINE;

	return gl_dfb_palette_match_color_ex(
        h_palette,
        ui2_base,
        ui2_count,
        b_cmp_alpha,
        pt_to_match,
        pt_matched,
        pui2_index);
}

/*INT32 x_gl_sw_nhv_pattern_paint(
    GL_HGRAPHICS_T      h_graphics,
    GL_COLORMODE_T      e_colormode,
    GL_COLOR_T          *pt_colorkey,
    UINT8               *pui1_compressed_img,
    GL_RECT_T           *pt_work_rect)
{
	return GLR_OK;
}*/	

#if 0
/* cursor-related API */
INT32 x_gl_cursor_enable(
    BOOL                b_enable)
{
}	

INT32 x_gl_cursor_get_capability(
    GL_PLANE_CAPS_T*    pt_pln_caps)
{
}	

INT32 x_gl_cursor_get_pos(
    INT32*              pi4_x,
    INT32               pi4_y)
{
}	

INT32 x_gl_cursor_load(
    GL_HSURFACE_T       h_surf)
{
}	

INT32 x_gl_cursor_load_stocked(
    UINT32              ui4_id)
{
}	

INT32 x_gl_cursor_set_pos(
    INT32               i4_x,
    INT32               i4_y)
{
}	
#endif



INT32 gl_plane_notify_drv_update(GL_DRV_UPDATE_INFO_T *pt_info)
{
    return 0;
}

INT32 x_gl_screen_get_panel_size(
    GL_HSCREEN_T        h_screen,
    UINT32              *pui4_w,
    UINT32              *pui4_h)
{
    return gl_dfb_screen_get_panel_size(h_screen, pui4_w, pui4_h);

}

INT32 x_gl_surface_get_bitmap_max_leng(
    GL_HSURFACE_T       h_surf,
    UINT32*             pui4_max_leng)
{
    return 0;
}

#if 1 /* MHF Linux - Bin Zhang */
INT32 x_gl_surface_compressed_blt(
    GL_HSURFACE_T           h_dst_surf,
    GL_HSURFACE_T           h_src_surf,
    INT32                   i4_src_x,
    INT32                   i4_src_y,
    UINT32                  ui4_src_w,
    UINT32                  ui4_src_h,
    GL_COMPRESSION_RATIO_T  e_ratio,
    BOOL                    b_line_separate,
    UINT32                  ui4_flag)
{
    return 0;
}

UINT32 gl_compose_packed_color(const GL_COLOR_T* pt_color, GL_COLORMODE_T e_mode)
{
    return 0;
}

/// Color mode 
typedef enum 
{
    CM_Reserved0 = 0,
    CM_Reserved1,
    CM_YCbCr_CLUT8,
    CM_Reserved2,
    CM_CbYCrY422_DIRECT16,
    CM_YCbYCr422_DIRECT16,
    CM_AYCbCr8888_DIRECT32,
    CM_Reserved3,
    CM_Reserved4,
    CM_Reserved5,
    CM_RGB_CLUT8,
    CM_RGB565_DIRECT16,
    CM_ARGB1555_DIRECT16,
    CM_ARGB4444_DIRECT16,
    CM_ARGB8888_DIRECT32,
    CM_Reserved6
} GFX_COLOR_MODE_T;



INT32 gl_gpu_fill_rect(
    GL_COLORMODE_T  e_mode,
    VOID*           pv_bits,
    UINT32          ui4_pitch,
    INT32           i4_x,
    INT32           i4_y,
    UINT32          ui4_width,
    UINT32          ui4_height,
    UINT32          ui4_color)
{
#if 0
    mw_gl_op_t t_op;
    int         fd;
    
    t_op.u4Op = MW_GL_OP_FILL_RECT;
    
    t_op.u.fill_rect.pv_bits    = pv_bits;
    t_op.u.fill_rect.ui4_pitch  = ui4_pitch;
    t_op.u.fill_rect.i4_x       = i4_x;
    t_op.u.fill_rect.i4_y       = i4_y;
    t_op.u.fill_rect.ui4_width  = ui4_width;
    t_op.u.fill_rect.ui4_height = ui4_height;
    t_op.u.fill_rect.ui4_color  = ui4_color;

    switch (e_mode)
    {
        case GL_COLORMODE_ARGB_D4444:
            t_op.u.fill_rect.e_mode  = CM_ARGB4444_DIRECT16;
            break;
            
        case GL_COLORMODE_ARGB_D8888:
            t_op.u.fill_rect.e_mode  = CM_ARGB8888_DIRECT32;
            break;
            
        case GL_COLORMODE_RGB_D565:
            t_op.u.fill_rect.e_mode  = CM_RGB565_DIRECT16;
            break;
            
        case GL_COLORMODE_ARGB_D1555:
            t_op.u.fill_rect.e_mode  = CM_ARGB1555_DIRECT16;
            break;
            
        case GL_COLORMODE_AYUV_D8888:
            t_op.u.fill_rect.e_mode  = CM_AYCbCr8888_DIRECT32;
            break;
            
        case GL_COLORMODE_AYUV_CLUT8:
            t_op.u.fill_rect.e_mode  = CM_YCbCr_CLUT8;
            break;
            
        case GL_COLORMODE_ARGB_CLUT8:
            t_op.u.fill_rect.e_mode  = CM_RGB_CLUT8;
            break;
            
        default:
            t_op.u.fill_rect.e_mode  = e_mode;
            break;    
    }
    
    fd = gl_dfb_get_fb_fd();
    
    ioctl(fd, FBIO_MW_OP, &t_op);
#else
	gl_dfb_locked_fill_rect(e_mode, pv_bits, ui4_pitch, i4_x,i4_y,ui4_width,ui4_height,ui4_color);
#endif
    return 0;
}


INT32 x_gl_get_fbm_surface_info(
    UINT8*              pui1_cnt, 
    UINT32*             pui4_mem_size)
{
#if 0

#ifdef MW_PHOTO_SHOW_ON_VDP
	GL_SURFACE_DESC_T    t_surf_desc; 
    GL_HSURFACE_T        h_surf = NULL_HANDLE;
	INT32                i4_ret;
	GL_LOCK_INFO_T       t_lock_info;
	
	t_surf_desc.e_clrmode = GL_COLORMODE_ARGB_D8888;
	t_surf_desc.h_clipper = NULL_HANDLE;
	t_surf_desc.h_palette = NULL_HANDLE;
	t_surf_desc.ui4_height = 1080;
	t_surf_desc.ui4_width  = 1920;

	i4_ret = x_gl_surface_create(&t_surf_desc, &h_surf);
	if (i4_ret != GLR_OK || h_surf == NULL_HANDLE)
	{
		*pui4_mem_size = 0;
    	*pui1_cnt = 0;
		return GLR_NO_MEMORY;
	}

	i4_ret = x_gl_surface_lock(h_surf, NULL, 0, &t_lock_info);
	if (i4_ret != GLR_OK)
	{
		*pui4_mem_size = 0;
    	*pui1_cnt = 0;
		x_gl_surface_unlock(h_surf, NULL);
		x_gl_obj_free(h_surf);
		return GLR_NO_MEMORY;
	}

	x_gl_surface_unlock(h_surf, NULL);
	x_gl_obj_free(h_surf);

	*pui4_mem_size = 1920*1080*4;
    *pui1_cnt = 1;
#else
    mw_gl_op_t t_op;
    int         fd;
    
    t_op.u4Op = MW_GL_OP_GET_FBM_BITMAP_INFO;

    t_op.u.get_fbm_bitmap_info.ui4_size = 0;
    t_op.u.get_fbm_bitmap_info.pv_bits  = NULL;
    
    fd = gl_dfb_get_fb_fd();
   
    ioctl(fd, FBIO_MW_OP, &t_op);

    *pui4_mem_size = t_op.u.get_fbm_bitmap_info.ui4_size - 256;
    *pui1_cnt = 1;
#endif 
#endif

    return 0;
}


INT32 gl_surface_flush(VOID* pv_surf, UINT32 ui4_flag)
{
    return 0;
}

INT32 x_gl_osd_wait_vdp(VOID)
{
#if 0
    ioctl(gl_dfb_get_fb_fd(), FBIO_GFX_WAIT_VDP, NULL);
#endif
    return GLR_OK;
}

VOID* x_gl_dfb_get_native_engine(VOID)
{
    return gl_dfb_get_native_engine();
}
#endif


#if 1
/** get plane z-order
 *  @param  u4PlaneId               plane id:{MTPMX_MAIN, MTPMX_PIP, MTPMX_OSD1, MTPMX_OSD2}
 *  @param  pu4PlaneOrder           plane order:{MTPMX_ORDER1/2/3/4}
 *  @retval GLR_OK                  Success
 *  @retval other              Fail
 */
extern INT32 x_gl_GetPlaneOrder(UINT32 u4PlaneId, UINT32* pu4PlaneOrder)
{
	return MTPMX_GetPlaneOrder(u4PlaneId, pu4PlaneOrder);
}

/** set plane z-order
 *  @param  u4PlaneId               plane id:{MTPMX_MAIN, MTPMX_PIP, MTPMX_OSD1, MTPMX_OSD2}
 *  @param  u4PlaneOrder            plane order:{MTPMX_ORDER1/2/3/4}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *   Pmx can control the order of Video/Graphic planes and merge into one to output stage.
 *   The sources of Pmx are Main, PIP, OSD1, OSD2.
 *   For example, through pmx, we can assign OSD1 as top-most plane, and then OSD2, PIP, and Main as the bottom plane.
 */
extern INT32 x_gl_SetPlaneOrder(UINT32 u4PlaneId, UINT32 u4PlaneOrder)
{
	return MTPMX_SetPlaneOrder(u4PlaneId, u4PlaneOrder);
}

/** set plane z-order array
 *  @param  pu4PlaneOrder               plane order array addr
 *  @param  u4PlaneOrder            plane order:{MTPMX_ORDER1/2/3/4}
 *  @retval MTR_OK                  Success
 *  @retval MTR_NOT_OK              Fail
 *   Pmx can control the order of Video/Graphic planes and merge into one to output stage.
 *   The sources of Pmx are Main, PIP, OSD1, OSD2.
 *   In mt5395, OSD planes must be on top of video planes.
 *   For example, through pmx, we can assign OSD2 as top-most plane, and then OSD1, PIP, and Main as the bottom plane.
 *   Then the plane order array will be set as 
 *   const UINT32 au4PlaneArray[4] = { PMX_OSD2, PMX_OSD1, MTPMX_PIP, MTPMX_MAIN };
 */
extern INT32 x_gl_SetPlaneOrderArray(UINT32* pu4PlaneOrder)
{
	return MTPMX_SetPlaneOrderArray(pu4PlaneOrder);
}

extern INT32 x_gl_GetPlaneOrderArray(UINT32* pu4PlaneOrder, INT32 i4Count)
{
	UINT32 PlaneOrder = 0;
	INT32  i4_ret;
	
	if (i4Count != 4)
	{
		return GLR_NO_MEMORY;
	}

	i4_ret = x_gl_GetPlaneOrder(GLPMX_MAIN, &PlaneOrder);
	if (i4_ret != GLR_OK && PlaneOrder > 4)
	{
		return GLR_DRV_ERROR;
	}
	pu4PlaneOrder[PlaneOrder] = GLPMX_MAIN;

	
	i4_ret = x_gl_GetPlaneOrder(GLPMX_PIP, &PlaneOrder);
	if (i4_ret != GLR_OK && PlaneOrder > 4)
	{
		return GLR_DRV_ERROR;
	}
	pu4PlaneOrder[PlaneOrder] = GLPMX_PIP;


	
	i4_ret = x_gl_GetPlaneOrder(GLPMX_OSD1, &PlaneOrder);
	if (i4_ret != GLR_OK && PlaneOrder > 4)
	{
		return GLR_DRV_ERROR;
	}
	pu4PlaneOrder[PlaneOrder] = GLPMX_OSD1;

	
	i4_ret = x_gl_GetPlaneOrder(GLPMX_OSD2, &PlaneOrder);
	if (i4_ret != GLR_OK && PlaneOrder > 4)
	{
		return GLR_DRV_ERROR;
	}
	pu4PlaneOrder[PlaneOrder] = GLPMX_OSD2;


	return GLR_OK;
	
}


#endif

