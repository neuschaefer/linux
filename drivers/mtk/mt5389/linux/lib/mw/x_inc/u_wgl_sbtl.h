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
/*----------------------------------------------------------------------------*/
/*! @file u_wgl_sbtl.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_WGL_SBTL_H_
#define _U_WGL_SBTL_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_gl.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_WGL_SBTL ISDB DVB Subtitle Widget
  *  @ingroup groupMW_WGL
  *  @brief Provides external function TV Application
  *  %MW Application API would be used by TV Application, lot of these APIs would
  *   be wrapped from Exported API
  *  @see groupMW_WGL_SBTL
  *  @{ 
  */
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* commands */
/* WGL_CMD_SBTL_FLIP<HANDLE_T h_page_inst, PTS_T *pui8_pts> */
#define WGL_CMD_SBTL_FLIP                   WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_SBTL, 0)    /**< flip data command */

/* WGL_CMD_SBTL_CLEAR_ALL<PTS_T *pui8_pts, NULL> */
#define WGL_CMD_SBTL_CLEAR_ALL              WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_SBTL, 1)    /**< clear all command */

#ifdef DVB_ENABLE_HD_SUBTITLE
/* WGL_CMD_SBTL_UPDATE_LOGI_SIZE<WGL_SBTL_LOGI_T *pt_wgl_sbtl_logi, NULL> */
#define WGL_CMD_SBTL_UPDATE_LOGI_SIZE       WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_SBTL,2)    /**< update logical size command */
#endif /* DVB_ENABLE_HD_SUBTITLE */

/* return values */
#define WGL_SBTLR_WOULD_BLOCK                (WGLR_USER + 1)    /**< successful, but can not use it now */
#define WGL_SBTLR_OK                         ((INT32) 0)        /**< successful */
#define WGL_SBTLR_LOCK_ERROR                 ((INT32) -1000)    /**< lock fail */
#define WGL_SBTLR_PIB_NOT_FOUND              ((INT32) -1001)    /**< can not find the PIB */
#define WGL_SBTLR_PIB_RGN_NOT_FOUND          ((INT32) -1002)    /**< can not find the region in PIB */
#define WGL_SBTLR_PIB_RGN_NOT_VALID          ((INT32) -1003)    /**< the PIB region is not vaild */

/* color mode enumeration */
/*------------------------------------------------------------------*/
/*! @enum   WGL_SBTL_COLOR_MODE_T
 *  @brief  This enumeration contains the color mode
 *  @code
 *  typedef enum _WGL_SBTL_COLOR_MODE_T
 *  {
 *      WGL_SBTL_COLOR_MODE_CLUT_2_BIT,
 *      WGL_SBTL_COLOR_MODE_CLUT_4_BIT,
 *      WGL_SBTL_COLOR_MODE_CLUT_8_BIT
 *  } WGL_SBTL_COLOR_MODE_T;    
 *  @endcode
 *  @li@c  WGL_SBTL_COLOR_MODE_CLUT_2_BIT                    - clut 2 bit
 *  @li@c  WGL_SBTL_COLOR_MODE_CLUT_4_BIT                    - clut 4 bit
 *  @li@c  WGL_SBTL_COLOR_MODE_CLUT_8_BIT                    - clut 8 bit
 */
/*------------------------------------------------------------------*/
typedef enum _WGL_SBTL_COLOR_MODE_T
{
    WGL_SBTL_COLOR_MODE_CLUT_2_BIT,
    WGL_SBTL_COLOR_MODE_CLUT_4_BIT,
    WGL_SBTL_COLOR_MODE_CLUT_8_BIT
} WGL_SBTL_COLOR_MODE_T;    

/* PIB locking function */
/*------------------------------------------------------------------*/
/*! @brief This API is called by the Subtitle widget and locks the page instance buffer (PIB) associated by the page instance handle, which is indicated in the argument h_page_inst. The PIB lock is acquired in no-wait manner. If the lock has been acquired by other threads, this API immediately returns WGL_SBTLR_WOULD_BLOCK. If the specified PIB fails to be locked, this API returns WGL_SBTLR_LOCK_ERROR
 *  @param [in]     h_page_inst                     - Contains the subtitle page instance handle
 *  @param [in]     pv_tag                          - Contains the host application private data
 *  @param [out]
 *  @return     Return the error code.
 *  @retval      WGL_SBTLR_OK                   - The specified PIB is successfully locked
 *  @retval      WGL_SBTLR_WOULD_BLOCK          - The specified PIB has been locked by other threads
 *  @retval      WGL_SBTLR_LOCK_ERROR           - The specified PIB fails to be locked
 *  @retval      WGL_SBTLR_PIB_NOT_FOUND        - The specified PIB is not found
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_sbtl_lock_pib_fct)(
    HANDLE_T                h_page_inst,
    VOID                    *pv_tag);

/* PIB unlocking function */
/*------------------------------------------------------------------*/
/*! @brief This API is called by the Subtitle widget and unlocks the page instance buffer (PIB) associated by the page instance handle, which is indicated in the argument h_page_inst.
 *  @param [in]     h_page_inst                     - Contains the subtitle page instance handle
 *  @param [in]     pv_tag                          - Contains the host application private data
 *  @param [out]
 *  @return     Return the error code.
 *  @retval      WGL_SBTLR_OK                   - The specified PIB is successfully unlocked.
 *  @retval      WGL_SBTLR_PIB_NOT_FOUND        - The specified PIB is not found.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_sbtl_unlock_pib_fct)(
    HANDLE_T                h_page_inst,
    VOID                    *pv_tag);

typedef struct _WGL_SBTL_REGION_T WGL_SBTL_REGION_T;
/*------------------------------------------------------------------*/
/*! @brief This API is called by the Subtitle widget and get a region indicated by ui2_rgn_idx from  the page instance buffer (PIB) associated by the page instance handle, which is indicated in the argument h_page_inst
 *  @param [in]     h_page_inst                     - Contains the subtitle page instance handle
 *  @param [in]     pv_tag                          - Contains the host application private data
 *  @param [in]     ui2_rgn_idx                     - Contains the region index that subtitle widget need
 *  @param [in]     pt_rgn                          - Contains the returned region structure
 *  @param [out]
 *  @return     Return the error code.
 *  @retval      WGL_SBTLR_OK                   - The specified PIB is successfully unlocked.
 *  @retval      WGL_SBTLR_PIB_NOT_FOUND        - The specified PIB is not found.
 *  @retval      WGL_SBTLR_PIB_RGN_NOT_FOUND    - The specified region is not found.
 *  @retval      WGL_SBTLR_PIB_RGN_NOT_VALID    - The specified region is not valid.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wgl_sbtl_get_page_inst_rgn_fct)(
    HANDLE_T                h_page_inst,
    VOID                    *pv_tag,
    UINT16                  ui2_rgn_idx,
    WGL_SBTL_REGION_T       *pt_rgn);

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/
/* region structure */
/*------------------------------------------------------------------*/
/*! @struct _WGL_SBTL_REGION_T
 *  @brief  This structure holds the information of a region
 *  @code
 *  struct _WGL_SBTL_REGION_T
 *  {
 *      UINT32                      ui4_rid;
 *      BOOL                        b_dirty;
 *      INT32                       i4_left;
 *      INT32                       i4_top;
 *      UINT32                      ui4_width;
 *      UINT32                      ui4_height;
 *      WGL_SBTL_COLOR_MODE_T       e_clr_mode;
 *      VOID                        *pv_bitmap;
 *      GL_COLOR_T                  *at_clut;
 *      GL_RECT_T                   t_rc_dirty;
 *  };    
 *  @endcode
 *  @li@c  ui4_rid                                           - Contains the region id for identifying the region
 *  @li@c  b_dirty                                           - Contains a BOOL flag indicating the region is dirty or not. If TRUE, the region has to be redrawn and the associated cache buffer (if has) has to be updated
 *  @li@c  i4_left                                           - Contains the location of the left boundary of the region
 *  @li@c  i4_top                                            - Contains the location of the top boundary of the region
 *  @li@c  ui4_width                                         - Contains the width of the region
 *  @li@c  ui4_height                                        - Contains the height of the region
 *  @li@c  e_clr_mode                                        - Contains the color mode of the region
 *  @li@c  *pv_bitmap                                        - Contains the pointer to the bitmap data buffer, which is prepared by the host application of the Subtitle widget (e.g. the Subtitle handler). The size of this buffer must be at least (ui4_width * ui4_height * bits_per_pixel / 8 + 1)
 *  @li@c  *at_clut                                          - Contains the pointer to the region's color lookup table. The size of the color table must be at least 2 ^ (bits_per_pixel).
 *  @li@c  t_rc_dirty                                        - Contains the rectangle of the dirty area in the region. This field is meaningful only when the b_dirty field is TRUE
 */
/*------------------------------------------------------------------*/
struct _WGL_SBTL_REGION_T
{
    UINT32                      ui4_rid;
    BOOL                        b_dirty;
    INT32                       i4_left;
    INT32                       i4_top;
    UINT32                      ui4_width;
    UINT32                      ui4_height;
    WGL_SBTL_COLOR_MODE_T       e_clr_mode;
    VOID                        *pv_bitmap;
    GL_COLOR_T                  *at_clut;
    GL_RECT_T                   t_rc_dirty;
};    

/*------------------------------------------------------------------*/
/*! @struct WGL_SBTL_INIT_T
 *  @brief  This structure holds the information for initializing a Subtitle widget
 *  @code
 *  typedef struct _WGL_SBTL_INIT_T
 *  {
 *      UINT32                              ui4_page_width;
 *      UINT32                              ui4_page_height;
 *      wgl_sbtl_lock_pib_fct               pf_lock_pib;
 *      VOID*                               pv_tag_lock_pib;
 *      wgl_sbtl_unlock_pib_fct             pf_unlock_pib;
 *      VOID*                               pv_tag_unlock_pib;
 *      wgl_sbtl_get_page_inst_rgn_fct      pf_get_page_inst_rgn;
 *      VOID*                               pv_tag_get_rgn;
 *  } WGL_SBTL_INIT_T;    
 *  @endcode
 *  @li@c  ui4_page_width                                    - Contains the logical width of a subtitling page. Note that if the logical width is not the same as the physical width of the Subtitle widget, the page content would be automatically scaled to fit into the physical widget area
 *  @li@c  ui4_page_height                                   - Contains the logical height of a subtitling page. Note that if the logical height is not the same as the physical height of the Subtitle widget, the page content would be automatically scaled to fit into the physical widget area
 *  @li@c  pf_lock_pib                                       - Contains the pointer of a lock function that is used to acquire and lock the page instance buffer. This function is provided by the host application of the Subtitle widget and will be called before a PIB is to be used. For more information about this function, refer to (*wgl_sbtl_lock_pib_fct)
 *  @li@c  pv_tag_lock_pib                                   - Contains the host application private data, when pf_lock_pib is called,  it will be transferred to subtitle handler as parameter
 *  @li@c  pf_unlock_pib                                     - Contains the pointer of an unlock function that is used to unlock the page instance buffer. This function is provided by the host application of the Subtitle widget and will be called if a PIB can be freed. For more information about this function, refer to (*wgl_sbtl_unlock_pib_fct)(*wgl_sbtl_lock_pib_fct).
 *  @li@c  pv_tag_unlock_pib                                 - Contains the host application private data, when pf_unlock_pib is called,  it will be transferred to subtitle handler as parameter
 *  @li@c  pf_get_page_inst_rgn                              - Contains the pointer of an get region function that is used to get region list from one page instance. This function is provided by the host application of the Subtitle widget and will be called if a PIB can be freed. For more information about this function
 *  @li@c  pv_tag_get_rgn                                    - Contains the host application private data, when pf_get_page_inst_rgn is called,  it will be transferred to subtitle handler as parameter
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_SBTL_INIT_T
{
    UINT32                              ui4_page_width;
    UINT32                              ui4_page_height;
    wgl_sbtl_lock_pib_fct               pf_lock_pib;
    VOID*                               pv_tag_lock_pib;
    wgl_sbtl_unlock_pib_fct             pf_unlock_pib;
    VOID*                               pv_tag_unlock_pib;
    wgl_sbtl_get_page_inst_rgn_fct      pf_get_page_inst_rgn;
    VOID*                               pv_tag_get_rgn;
} WGL_SBTL_INIT_T;    

#ifdef DVB_ENABLE_HD_SUBTITLE
/*------------------------------------------------------------------*/
/*! @struct WGL_SBTL_LOGI_SZ_T
 *  @brief  This structure contains the DDS information for logical size display
 *  @code
 *  typedef struct _WGL_SBTL_LOGI_SZ_T
 *  {
 *      UINT16      ui2_logi_width;
 *      UINT16      ui2_logi_height;
 *  } WGL_SBTL_LOGI_SZ_T;
 *  @endcode
 *  @li@c  ui2_logi_width                                    - Contains new logical width of a subtitling page. Note that if the logical width is not the same as the physical width of the Subtitle widget, the page content would be automatically scaled to fit into the physical widget area.
 *  @li@c  ui2_logi_height                                   - Contains new logical height of a subtitling page. Note that if the logical height is not the same as the physical height of the Subtitle widget, the page content would be automatically scaled to fit into the physical widget area.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_SBTL_LOGI_SZ_T
{
    UINT16      ui2_logi_width;
    UINT16      ui2_logi_height;
} WGL_SBTL_LOGI_SZ_T;
#endif /* DVB_ENABLE_HD_SUBTITLE */

#ifdef __cplusplus
}
#endif

#endif

/*----------------------------------------------------------------------------*/
/*! @} */                                /* end of groupMW_WGL_SBTL */
/*----------------------------------------------------------------------------*/


