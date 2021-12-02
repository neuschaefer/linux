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
 * $RCSfile: x_wgl_cc.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 112905fbb1168f6cafcaec33132fd0c9 $
 *
 * Description: 
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 *---------------------------------------------------------------------------*/
 
 

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_WGL_CC Closed Caption Widget
  *  @ingroup groupMW_WGL
  *  @brief Provides x_api function TV Application
  *  %MW Application API would be used by TV Application, lot of these APIs would
  *   be wrapped from Exported API
  *  @see groupMW_WGL
  *  @{ 
  */
/*----------------------------------------------------------------------------*/
     
#ifdef MW_CC_SUPPORT_CUST_1
     /*here we include the .h files in cc_c1 directory*/
#include "x_wgl_cc.h"
#else

#ifndef _X_WGL_CC_H_
#define _X_WGL_CC_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl_cc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_CC_INIT_CTRL_LOGIC          ((UINT32)0x00000001)
/**<The CC init control logic flag.*/

#define WGL_CC_INIT_DEFAULT_CNT         ((UINT32)0x00000002)
/**<The CC init default content flag.*/

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
#define WGL_CC_SYS_INIT_FLAG_FONT_TBL           MAKE_BIT_MASK_32(0)
/**<The CC init flag indicating that the font table is valid.*/

#define WGL_CC_SYS_INIT_FLAG_SMTH_SCRL_SPEED    MAKE_BIT_MASK_32(1)
/**<The CC init flag indicating that the smooth scroll speed is valid.*/

#define WGL_CC_SYS_INIT_FLAG_FLASH_INTERVAL     MAKE_BIT_MASK_32(2)
/**<The CC init flag indicating that the falsh interval is valid.*/

#define WGL_CC_SYS_INIT_FLAG_SYS_MAX_CC_SIZE    MAKE_BIT_MASK_32(3) 
/**<The CC init flag indicating that the system max CC size is valid.*/

#define WGL_CC_SYS_INIT_FLAG_CANVAS_CLR_MODE    MAKE_BIT_MASK_32(4) 
/**<The CC init flag indicating that canvas color mode is valid.*/

#define WGL_CC_SYS_INIT_FLAG_DIS_SMALL_BUFF     MAKE_BIT_MASK_32(5) 
/**<The CC init flag indicating that the disable small buffer flag is valid.(No used)*/

#define WGL_CC_SYS_INIT_FLAG_LAST_FLAG          MAKE_BIT_MASK_32(5)
/**<The CC init flag indicating it is last.*/



/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption resclale information.
  *  @code 
  *  typedef struct _WGL_CC_SYS_INIT_T 
  *  { 
  *         UINT32                              ui4_flags;
  *         WGL_CC_FONT_REG_INFO_T              *at_font_reg_table; 
  *         UINT32                              ui4_scrl_step;
  *         UINT32                              ui4_scrl_delay;
  *         UINT32                              ui4_flash_interval;
  *         UINT32                              ui4_sys_max_cc_width; 
  *         UINT32                              ui4_sys_max_cc_height;   
  *         GL_COLORMODE_T                      e_clr_mode_canvas;
  *  } WGL_CC_SYS_INIT_T; 
  *  @endcode 
  *  @li@c  ui4_flags                    -Reference to the flag indicating which parameters are valid.
  *  @li@c  at_font_reg_table            -Reference to font register table.
  *  @li@c  ui4_scrl_step                -Reference to scroll step.
  *  @li@c  ui4_scrl_delay               -Reference to scroll delay time.
  *  @li@c  ui4_flash_interval           -Reference to flash interval time. 
  *  @li@c  ui4_sys_max_cc_width         -Reference to the system max CC width.
  *  @li@c  ui4_sys_max_cc_height        -Reference to the system max CC height.
  *  @li@c  e_clr_mode_canvas            -Reference to the colormode of CC canvas.
  */
/*------------------------------------------------------------------*/
typedef struct _WGL_CC_SYS_INIT_T
{
    UINT32                              ui4_flags;
    
    /* WGL_CC_SYS_INIT_FLAG_FONT_TBL: [mandate] */
    WGL_CC_FONT_REG_INFO_T              *at_font_reg_table; 
    
    /* WGL_CC_SYS_INIT_FLAG_SMTH_SCRL_SPEED */
    UINT32                              ui4_scrl_step;
    UINT32                              ui4_scrl_delay;
    
    /* WGL_CC_SYS_INIT_FLAG_FLASH_INTERVAL */
    UINT32                              ui4_flash_interval;
    
    /* WGL_CC_SYS_INIT_FLAG_SYS_MAX_CC_SIZE: [mandate] */
    UINT32                              ui4_sys_max_cc_width;       
    UINT32                              ui4_sys_max_cc_height;      
    
    /* WGL_CC_SYS_INIT_FLAG_CANVAS_CLR_MODE: [mandate] */
    GL_COLORMODE_T                      e_clr_mode_canvas;
   
} WGL_CC_SYS_INIT_T;    


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
 
/*------------------------------------------------------------------*/
/*! @brief  init the attribute of Closed Caption widget.
 *  @param  [in]    ui4_init_type   - Contains the type of initialization.
 *  @param  [in]    pt_sys_init     - Contains the init parameters.
 *  @note 
 *  @return return the error code
 *  @retval    WGLR_OK                  The routine was successful. 
 *  @retval    WGLR_INV_ARG             The argument is invalid.
 *  @retval    WGLR_OUT_OF_RESOURCE     There isn't sufficient memory.
 */
/*------------------------------------------------------------------*/
extern INT32 x_wgl_cc_init(
    UINT32                              ui4_init_type,
    WGL_CC_SYS_INIT_T                   *pt_sys_init);


#ifdef __cplusplus
}
#endif

#endif
#endif



/*----------------------------------------------------------------------------*/
/*! @} */                                /* end of MW_WGL_CC */
/*----------------------------------------------------------------------------*/


