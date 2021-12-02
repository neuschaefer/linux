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
/*! @file x_scc.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Stream Component Contolller specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR_SCC Stream Component Controller
 *
 *  @ingroup groupMW_STRM_MNGR
 *  @brief Provides stream component controller module external function for middeware module.
 *
 *
 *  @see groupMW_STRM_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _X_SCC_H_
#define _X_SCC_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_scc.h"
#include "u_sm_video_hdlr.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief     close has opened scc component
 *  @param [in]   h_comp    component  handle.
 *  @note     
 *  @see     x_scc_comp_close
 *  @return
 *  @retval      -SMR_OK                  Success.
 *  @retval      -SMR_INV_HANDLE     h_comp is an invalid handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_comp_close( 
    HANDLE_T            h_comp );

/*------------------------------------------------------------------*/
/*! @brief     Open scc component
 *  @param [in]   pt_cmd_sets   open component command.
 *  @param [out] ph_comp        specify the opened component.    
 *  @note
 *  @see     x_scc_comp_open
 *  @return
 *  @retval      -SMR_OK                  Success.
 *  @retval      -SMR_INV_HANDLE     h_comp is an invalid handle.
 *  @retval      -SMR_DRV_COMP_NOT_FOUND  component not find to open.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_comp_open( 
    SM_COMMAND_T*       pt_cmd_sets,
    HANDLE_T*           ph_comp );

/*------------------------------------------------------------------*/
/*! @brief    set group command 
 *  @param [in]   pt_cmd_sets   open component command.
 *  @note
 *  @see  x_scc_comp_grp_set
 *  @return
 *  @retval      -SMR_OK                  Success.
 *  @retval      -SMR_INV_HANDLE     h_comp is an invalid handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_comp_grp_set( 
    SM_COMMAND_T*        pt_cmd_sets );

/*------------------------------------------------------------------*/
/*! @brief  set component attribute increasingly
 *  @param [in]    h_comp  component to set.
 *  @param [in]    e_scc_cmd_code  set command     
 *  @param [in]   pv_cmd_arg  auxiliary command  
 *  @param [out]  pui2_curr_value  current  value.
 *  @note
 *  @see    x_scc_comp_att_incr
 *  @return
 *  @retval      -SMR_OK                  Success.
 *  @retval      -SMR_INV_HANDLE     h_comp is an invalid handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_comp_att_incr(
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_curr_value );

/*------------------------------------------------------------------*/
/*! @brief  set component attribute decreasingly
 *  @param [in]    h_comp  component to set.
 *  @param [in]    e_scc_cmd_code  set command     
 *  @param [in]   pv_cmd_arg  auxiliary command  
 *  @param [out]  pui2_curr_value  current  value.
 *  @note
 *  @see    x_scc_comp_att_decr
 *  @return
 *  @retval      -SMR_OK                  Success.
 *  @retval      -SMR_INV_HANDLE     h_comp is an invalid handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_comp_att_decr( 
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_curr_value );

/*------------------------------------------------------------------*/
/*! @brief    get steps numbers form max to min about setting attribute
 *  @param [in]    h_comp  component to set.
 *  @param [in]    e_scc_cmd_code  set command     
 *  @param [in]   pv_cmd_arg  auxiliary command  
 *  @param [out]  pui2_num_steps  the steps numbers.
 *  @note
 *  @see    x_scc_comp_att_get_num_steps
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INV_ARG         The argument pui4_max_steps is invalid.
 *  @retval      -SMR_NOT_ALLOW       The e_scc_cmd_code is not allowed to get MAX steps. 
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_comp_att_get_num_steps( 
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_num_steps );

/* Video related API *********************************************************/
/*------------------------------------------------------------------*/
/*! @brief   set video mode
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_mode              mode to setting.
 *  @note
 *  @see    x_scc_vid_set_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_mode( 
    HANDLE_T                   h_vid,
    SCC_VID_MODE_T             e_mode );

/*------------------------------------------------------------------*/
/*! @brief   set video mode contains display region info.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_mode              mode to setting.
 *  @note
 *  @see    x_scc_vid_set_mode_ex
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_mode_ex( 
    HANDLE_T                   h_vid,
    const SCC_VID_MODE_EX_T*   pt_mode );

/*------------------------------------------------------------------*/
/*! @brief   enable or disable signal booster
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  b_is_on              Whether  to set or not.
 *  @note
 *  @see   x_scc_vid_set_sig_booster
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_sig_booster(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   enable or disable signal booster flag
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  b_is_on              Whether  to set or not.
 *  @note
 *  @see   x_scc_vid_set_sig_booster_flag
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_sig_booster_flag(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief    set video background.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  pt_bg                background value to set
 *  @note
 *  @see    x_scc_vid_set_bg
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_bg( 
    HANDLE_T                   h_vid,
    const SCC_BG_COLOR_T*      pt_bg );

/*------------------------------------------------------------------*/
/*! @brief   set video super blank info.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  pt_blank             blank value to set
 *  @note
 *  @see   x_scc_vid_set_super_blank
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_super_blank( 
    HANDLE_T                   h_vid,
    const SCC_VID_SUPER_BLANK_T* pt_blank );

/*------------------------------------------------------------------*/
/*! @brief   set video screen mode
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_scr_mode        screen mode to set
 *  @note
 *  @see     x_scc_vid_set_scr_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_scr_mode( 
    HANDLE_T                   h_vid,
    SCC_VID_SCREEN_MODE_T      e_scr_mode );

/*------------------------------------------------------------------*/
/*! @brief   set video enhance value.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_enhance       enhance value.
 *  @note
 *  @see    x_scc_vid_set_enhance
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_enhance( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_enhance );

/*------------------------------------------------------------------*/
/*! @brief   set video blur
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_blur            blur value.
 *  @note
 *  @see    x_scc_vid_set_blur
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_blur( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_blur );

/*------------------------------------------------------------------*/
/*! @brief  set overscan value.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  pt_clipper            position value.
 *  @note
 *  @see    x_scc_vid_set_overscan_clipper
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_overscan_clipper( 
    HANDLE_T                   h_vid,
    const SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper );

/*------------------------------------------------------------------*/
/*! @brief   This function not implement currently
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_ble( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_ble );

/*------------------------------------------------------------------*/
/*! @brief   set video cti.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_cti            cti value.
 *  @note
 *  @see    x_scc_vid_set_cti
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_cti( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_cti );

/*------------------------------------------------------------------*/
/*! @brief   set video eti.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_eti            eti value.
 *  @note
 *  @see    x_scc_vid_set_eti
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_eti( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_eti );

/*------------------------------------------------------------------*/
/*! @brief   set video sharpness .
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_sharpness     sharpness value.
 *  @note
 *  @see    x_scc_vid_set_sharpness
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_sharpness( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_sharpness);

/*------------------------------------------------------------------*/
/*! @brief   set video color suppress.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_color_supress     color suppress value.
 *  @note
 *  @see    x_scc_vid_set_color_suppress
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_color_suppress( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_color_supress );

/*------------------------------------------------------------------*/
/*! @brief  Set video NR.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_nr               NR value.
 *  @note
 *  @see   x_scc_vid_set_nr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_nr( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_nr );

/*------------------------------------------------------------------*/
/*! @brief     Set video 3dNR.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_3dnr               3dNR value.
 *  @note
 *  @see   x_scc_vid_set_3dnr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3dnr( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_3dnr );

/*------------------------------------------------------------------*/
/*! @brief     Set video black level 
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_black_lvl_ext     black level value.
 *  @note
 *  @see  x_scc_vid_set_black_lvl_ext
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_black_lvl_ext( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_black_lvl_ext );

/*------------------------------------------------------------------*/
/*! @brief   set video white peak limit.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_white_peak_rdct     white peak limit value.
 *  @note
 *  @see    x_scc_vid_set_white_peak_rdct
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_white_peak_rdct( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_white_peak_rdct );

/*------------------------------------------------------------------*/
/*! @brief  Set video flesh tone.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_flesh_tone     flesh tone value.
 *  @note
 *  @see    x_scc_vid_set_flesh_tone
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_flesh_tone( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_flesh_tone );

/*------------------------------------------------------------------*/
/*! @brief   Set video luma value.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_luma            luma value.
 *  @note
 *  @see   x_scc_vid_set_luma
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_luma( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_luma );

/*------------------------------------------------------------------*/
/*! @brief   Set video letter box mode detect.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  b_is_on             whether set this feature or not.
 *  @note
 *  @see   x_scc_vid_set_letter_box_detect
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_letter_box_detect( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief     Set video letterbox detect config.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  pt_config             reference the config struct.
 *  @note
 *  @see    x_scc_vid_set_lb_detect_config
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_lb_detect_config( 
    HANDLE_T                   h_vid,
    SCC_VID_LB_DETECT_CONFIG_T* pt_config );

/*------------------------------------------------------------------*/
/*! @brief   set video dynamic scaling.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_effect_time     errect time value.
 *  @note
 *  @see   x_scc_vid_set_dynamic_scaling
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_dynamic_scaling( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_effect_time );

/*------------------------------------------------------------------*/
/*! @brief    Set video deblocking.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  pt_deblocking     contains deblocking info.
 *  @note
 *  @see   x_scc_vid_set_deblocking
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_deblocking( 
    HANDLE_T                   h_vid,
    SCC_VID_DEBLOCKING_INFO_T* pt_deblocking );

/*------------------------------------------------------------------*/
/*! @brief   set split screen demo.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_splt_scrn_demo     indicate split screen demo mode.
 *  @note
 *  @see    x_scc_vid_set_split_scrn_demo
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_split_scrn_demo( 
    HANDLE_T                   h_vid,
    SCC_VID_SPLIT_SCRN_DEMO_T  e_splt_scrn_demo );

/*------------------------------------------------------------------*/
/*! @brief    Set video horizontal position
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_offset     indicate position offset.
 *  @note
 *  @see    x_scc_vid_set_h_position
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_h_position( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset );

/*------------------------------------------------------------------*/
/*! @brief   Set video vertical position
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_offset     indicate position offset.
 *  @note
 *  @see    x_scc_vid_set_v_position
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_v_position( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset );

/*------------------------------------------------------------------*/
/*! @brief    set horizontal size.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_offset     indicate size offset.
 *  @note
 *  @see   x_scc_vid_set_h_size
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_h_size( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset );

/*------------------------------------------------------------------*/
/*! @brief    set vertical size.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  ui2_offset     indicate size offset.
 *  @note
 *  @see   x_scc_vid_set_v_size
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_v_size( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset );

/*------------------------------------------------------------------*/
/*! @brief  set video positon size
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_set_type                set type .
 *  @param [in]  ui2_offset     indicate size offset.
 *  @note
 *  @see    x_scc_vid_set_position_size
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_position_size( 
    HANDLE_T                   h_vid,
    UINT32                     e_set_type,
    UINT16                     ui2_offset );

/*------------------------------------------------------------------*/
/*! @brief   set video film mode.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  b_is_on           indicate whether use this setting or not.
 *  @note
 *  @see   x_scc_vid_set_film_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_film_mode( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief    Reset video vga timing option.
 *  @param [in]    h_vid                video component handle.
 *  @note
 *  @see   x_scc_vid_set_reset_vga_timing_option
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_reset_vga_timing_option( 
    HANDLE_T                   h_vid );

/*------------------------------------------------------------------*/
/*! @brief    set vga timing control.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  b_is_on           indicate whether use this setting or not.
 *  @note
 *  @see   x_scc_vid_set_ambig_vga_timing_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_ambig_vga_timing_ctrl( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief  enable/disable thumbnail 
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  b_is_on           indicate whether use this setting or not.
 *  @note
 *  @see      x_scc_vid_set_thumbnail_enable
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_thumbnail_enable(
	HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   set srm mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_srm_mode  specify srm mode.
 *  @note
 *  @see   x_scc_vid_set_srm_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_srm_mode(
    HANDLE_T                   h_vid,
    SCC_VID_SRM_MODE_T         e_srm_mode); 

/*------------------------------------------------------------------*/
/*! @brief   get video mode.
 *  @param [in]  h_vid                video component handle.
 *  @param [out]pe_mode           contain video mode.
 *  @note
 *  @see  x_scc_vid_get_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_mode( 
    HANDLE_T                   h_vid,
    SCC_VID_MODE_T*            pe_mode );

/*------------------------------------------------------------------*/
/*! @brief  get video mode capability.
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  e_mode         contain video mode.
 *  @note
 *  @see   x_scc_vid_get_mode_capability
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_mode_capability( 
    HANDLE_T                   h_vid,
    SCC_VID_MODE_T             e_mode );

/*------------------------------------------------------------------*/
/*! @brief  get video background
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  pt_bg             contain video background.
 *  @note
 *  @see   x_scc_vid_get_bg
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_bg( 
    HANDLE_T                   h_vid,
    SCC_BG_COLOR_T*            pt_bg );

/*------------------------------------------------------------------*/
/*! @brief   get screen mode
 *  @param [in]  h_vid   video component handle.
 *  @param [out]  pe_scr_mode  contain screen mode.
 *  @note
 *  @see   x_scc_vid_get_scr_mide
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_scr_mode( 
    HANDLE_T                   h_vid,
    SCC_VID_SCREEN_MODE_T*     pe_scr_mode );

/*------------------------------------------------------------------*/
/*! @brief   get video enhance value.
 *  @param [in]  h_vid  video component handle.
 *  @param [out] pui2_enhance  retrieval enhance value.
 *  @note
 *  @see  x_scc_vid_get_enhance
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_enhance( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_enhance );

/*------------------------------------------------------------------*/
/*! @brief   get video blur
 *  @param [in] h_vid      video component handle.
 *  @param [out] pui2_blur    contain blur value.
 *  @note
 *  @see   x_scc_vid_get_blur
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_blur( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_blur );
/*------------------------------------------------------------------*/
/*! @brief   get overscan clipper
 *  @param [in] h_vid  video component handle.
 *  @param [out] pt_clipper  contain overscan clipper info.
 *  @note
 *  @see  x_scc_vd_get_overscan_clipper
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_overscan_clipper( 
    HANDLE_T                   h_vid,
    SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper );

/*------------------------------------------------------------------*/
/*! @brief   This function not implement
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_ble( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_ble );

/*------------------------------------------------------------------*/
/*! @brief  get video cti
 *  @param [in] h_vid  video component handle.
 *  @param [out]  pui2_cti  contain cti value.
 *  @note
 *  @see  x_scc_vid_get_cti
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_cti( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_cti );

/*------------------------------------------------------------------*/
/*! @brief  get video eti
 *  @param [in] h_vid  video component handle.
 *  @param [out]  pui2_eti  contain eti value.
 *  @note
 *  @see  x_scc_vid_get_eti
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_eti( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_eti );

/*------------------------------------------------------------------*/
/*! @brief  get video sharpness value.
 *  @param [in]  h_vid  video component handle.
 *  @param [out] pui2_sharpness  retrieval sharpness value.
 *  @note
 *  @see  x_scc_vid_get_sharpness
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_sharpness( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_sharpness );

/*------------------------------------------------------------------*/
/*! @brief   get video color suppress
 *  @param [in]h_vid  video component handle.
 *  @param [out]pui2_color_supress  retrieval color suppress value.
 *  @note
 *  @see  x_scc_vid_get_color_supress
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_color_supress( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_color_supress );

/*------------------------------------------------------------------*/
/*! @brief   get video NR
 *  @param [in]h_vid  video component handle.
 *  @param [out] pui2_nr   retrieval NR value.
 *  @note
 *  @see   x_scc_vid_get_nr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_nr( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_nr );

/*------------------------------------------------------------------*/
/*! @brief  get video 3dNR
 *  @param [in] h_vid video component handle.
 *  @param [out] pui2_2dnr  retrieval 3dnr value.
 *  @note
 *  @see   x_scc_vid_get_3dnr
 *  @return   
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3dnr( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_3dnr );

/*------------------------------------------------------------------*/
/*! @brief  get black level
 *  @param [in]h_vid   video component handle.
 *  @param [out]pui2_black_lvl_ext  retrieval black level value.
 *  @note
 *  @see   x_scc_vid_get_black_lvl_ext
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_black_lvl_ext( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_black_lvl_ext );

/*------------------------------------------------------------------*/
/*! @brief  get white peak limit.
 *  @param [in]h_vid  video component handle.
 *  @param [out] pui2_white_peak_rdct   retrieval peak limit data.
 *  @note
 *  @see   x_scc_vid_get_white_peak_rdct
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_white_peak_rdct( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_white_peak_rdct );

/*------------------------------------------------------------------*/
/*! @brief   get flesh tone.
 *  @param [in]h_vid   video component handle.
 *  @param [out] pui2_flesh_tone  flesh tone value.
 *  @note
 *  @see   x_scc_vid_get_flesh_tone
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_flesh_tone( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_flesh_tone );

/*------------------------------------------------------------------*/
/*! @brief  get video luma.
 *  @param [in]h_vid   video component handle.
 *  @param [out]pui2_luma   retrieval luma value.
 *  @note
 *  @see   x_scc_vid_get_luma
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_luma( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_luma );

/*------------------------------------------------------------------*/
/*! @brief   get letterbox detect info.
 *  @param [in]h_vid video component handle.
 *  @param [out]pb_is_on   Indicate whether has use this setting or not.
 *  @note
 *  @see   x_scc_vid_get_letter_box_detect
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_letter_box_detect( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief   get letterbox detect config info.
 *  @param [in]h_vid    video component handle.
 *  @param [out] pt_config    contain letterbox detect config info.
 *  @note
 *  @see    x_scc_vid_get_lb_detect_config
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_lb_detect_config( 
    HANDLE_T                   h_vid,
    SCC_VID_LB_DETECT_CONFIG_T* pt_config );

/*------------------------------------------------------------------*/
/*! @brief    get dynamic scaling
 *  @param [in]h_vid   video component handle.
 *  @param [out]pui2_effect_time   effect time value.
 *  @note
 *  @see  x_scc_vid_get_dynamic_scaling
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_dynamic_scaling( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_effect_time );

/*------------------------------------------------------------------*/
/*! @brief   get video deblocking info.
 *  @param [in] h_vid    video component handle.
 *  @param [out]pt_deblocking  contain deblocking info.
 *  @note
 *  @see   x_scc_vid_get_deblocking
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_deblocking( 
    HANDLE_T                   h_vid,
    SCC_VID_DEBLOCKING_INFO_T* pt_deblocking );

/*------------------------------------------------------------------*/
/*! @brief   get video split screen demo info.
 *  @param [in]  h_vid           video component handle.
 *  @param [out]pe_splt_scrn_demo    contain split screen demo info.
 *  @note
 *  @see  x_scc_vid_get_split_scrn_demo
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_split_scrn_demo( 
    HANDLE_T                   h_vid,
    SCC_VID_SPLIT_SCRN_DEMO_T* pe_splt_scrn_demo );

/*------------------------------------------------------------------*/
/*! @brief   get video horizon position
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  pui2_min           get video position min value
 *  @param [out]  pui2_max          get video position max value
 *  @param [out]  pui2_curr          get current video position 
 *  @note
 *  @see    x_scc_vid_get_h_position
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_h_position( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr );

/*------------------------------------------------------------------*/
/*! @brief   get video vertical position
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  pui2_min           get video position min value
 *  @param [out]  pui2_max          get video position max value
 *  @param [out]  pui2_curr          get current video position 
 *  @note
 *  @see    x_scc_vid_get_v_position
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_v_position( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr );

/*------------------------------------------------------------------*/
/*! @brief   get video horizon size
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  pui2_min           get video size min value
 *  @param [out]  pui2_max          get video size max value
 *  @param [out]  pui2_curr          get current video size 
 *  @note
 *  @see    x_scc_vid_get_h_size
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_h_size( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr );

/*------------------------------------------------------------------*/
/*! @brief   get video vertical size
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  pui2_min           get video size min value
 *  @param [out]  pui2_max          get video size max value
 *  @param [out]  pui2_curr          get current video size 
 *  @note
 *  @see    x_scc_vid_get_v_size
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_v_size( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr );

/*------------------------------------------------------------------*/
/*! @brief get video position size
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_get_type                get type.
 *  @param [out]  pui2_min           get video size min value
 *  @param [out]  pui2_max          get video size max value
 *  @param [out]  pui2_curr          get current video size 
 *  @note
 *  @see    x_scc_vid_get_position_size
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_position_size( 
    HANDLE_T                   h_vid,
    UINT32                     e_get_type,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr );

/*------------------------------------------------------------------*/
/*! @brief   get film mode
 *  @param [in]  h_vid                video component handle.
 *  @param [out]pb_is_on           retrieval whether has setting this film mode.
 *  @note
 *  @see   x_scc_vid_get_film_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_film_mode( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief   set video iframe notify.
 *  @param [in]  h_vid   video component handle.
 *  @param [in] pf_scc_iframe_nfy   reference the notify function
 *  @param [in] pv_nfy_tag      contain private tag info.
 *  @note
 *  @see   x_scc_vid_set_iframe_nfy
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_iframe_nfy(
    HANDLE_T                     h_vid, 
    x_scc_vid_iframe_nfy_fct     pf_scc_iframe_nfy, 
    VOID*                        pv_nfy_tag );

#ifdef SYS_MHP_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief   set video iframe notify.
 *  @param [in]  h_vid   video component handle.
 *  @param [in] pf_scc_iframe_nfy   reference the notify function
 *  @param [in] pv_nfy_tag      contain private tag info.
 *  @note
 *  @see   x_scc_vid_set_iframe_nfy
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_mhp_iframe_nfy(
    HANDLE_T                     h_vid,
    x_scc_vid_iframe_nfy_fct     pf_scc_iframe_nfy,
    VOID*                        pv_nfy_tag );

/*------------------------------------------------------------------*/
/*! @brief   set video mhp iframe close.
 *  @param [in]  h_vid   video component handle.
 *  @note
 *  @see   x_scc_vid_set_mhp_iframe_close
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_mhp_iframe_close(
    HANDLE_T                     h_vid );
#endif

/*------------------------------------------------------------------*/
/*! @brief   allocate iframe buffer
 *  @param [in]   h_vid   video component handle.
 *  @param [out] pt_iframe_buf  contain allocate buffer info.
 *  @note
 *  @see    x_scc_vid_iframe_alloc_buffer
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_iframe_alloc_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf);


/*------------------------------------------------------------------*/
/*! @brief   set iframe buffer.
 *  @param [in]  h_vid   video component handle.
 *  @param [in]  pt_iframe_buf  specify iframe buffer.
 *  @note
 *  @see   x_scc_vid_set_iframe_buffer
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_iframe_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf);

/*------------------------------------------------------------------*/
/*! @brief   set iframe decode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_iframe_buf  specify iframe buffer.
 *  @note
 *  @see   x_scc_vid_iframe_decode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_iframe_decode(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf);

/*------------------------------------------------------------------*/
/*! @brief    free iframe buffer.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_iframe_buf  specify iframe buffer.
 *  @note
 *  @see   x_scc_vid_iframe_free_buffer
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_iframe_free_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf);

/*------------------------------------------------------------------*/
/*! @brief    set iframe codec info.
 *  @param [in]  h_vid  video component handle.
 *  @note
 *  @see  x_scc_vid_set_iframe_codec
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_iframe_codec(
    HANDLE_T                      h_vid, 
    SCC_VID_IFRAME_CODEC_T        e_codec );

/*------------------------------------------------------------------*/
/*! @brief   set iframe control
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_ctrl_mode  specify iframe control mode.
 *  @note
 *  @see  x_scc_vid_set_iframe_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_iframe_ctrl(
    HANDLE_T                      h_vid, 
    SCC_VID_IFRAME_CTRL_T         e_ctrl_mode ); 

#ifdef SYS_MHP_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief   set iframe control
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_ctrl_mode  specify iframe control mode.
 *  @note
 *  @see  x_scc_vid_set_mhp_iframe_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_mhp_iframe_ctrl(
    HANDLE_T                      h_vid, 
    SCC_VID_IFRAME_CTRL_T         e_ctrl_mode ); 
#endif

/*------------------------------------------------------------------*/
/*! @brief   get iframe resolution info.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui4_width   retrieval width value.
 *  @param [out]  pui4_height   retrieval height value.
 *  @note
 *  @see  x_scc_vid_get_iframe_resolution
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_iframe_resolution(
    HANDLE_T                       h_vid, 
    UINT32*                        pui4_width, 
    UINT32*                        pui4_height);

/*------------------------------------------------------------------*/
/*! @brief  set iframe plane order.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_order_info  contain order info.
 *  @note
 *  @see   x_scc_vid_set_iframe_pla_order
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_iframe_pla_order(
    HANDLE_T                       h_vid, 
    VSH_SET_PLANE_ORDER_INFO_T*    pt_order_info);

/*------------------------------------------------------------------*/
/*! @brief   set iframe source region.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_region_info  contain source region info.
 *  @note
 *  @see   x_scc_vid_set_iframe_src_region
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_iframe_src_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info);
/*------------------------------------------------------------------*/
/*! @brief   set iframe display region.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_region_info  contain display region info.
 *  @note
 *  @see   x_scc_vid_set_iframe_disp_region
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_iframe_disp_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info);

/*------------------------------------------------------------------*/
/*! @brief   get iframe source region.
 *  @param [in]  h_vid  video component handle.
 *  @param [out] pt_region_info  contain source region info.
 *  @note
 *  @see   x_scc_vid_get_iframe_src_region
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_iframe_src_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info);

/*------------------------------------------------------------------*/
/*! @brief   get iframe display region.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]   pt_region_info  contain display region info.
 *  @note
 *  @see   x_scc_vid_get_iframe_disp_region
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_iframe_disp_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info);

/*------------------------------------------------------------------*/
/*! @brief    This function not implement
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_iframe_surface(
    HANDLE_T                        h_vid,
    HANDLE_T                        h_surface);

#ifdef SYS_MHP_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief    set iframe not display.
 *  @param [in]  h_vid  video component handle.
 *  @note
 *  @see  x_scc_vid_set_iframe_not_display
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_iframe_not_display(
    HANDLE_T                      h_vid, 
    BOOL                          b_not_display ); 

/*------------------------------------------------------------------*/
/*! @brief    get thumbnail info.
 *  @param [in]  h_vid  video component handle.
 *  @note
 *  @see  x_scc_vid_get_thumbnail
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_thumbnail(
    HANDLE_T                      h_vid, 
    VSH_THUMBNAIL_INFO_T*         pt_thumbnail_info ); 
#endif

/*------------------------------------------------------------------*/
/*! @brief  This function not implement
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_vdrips_nfy(
    HANDLE_T                     h_vid, 
    x_scc_vid_vdrips_nfy_fct     pf_scc_vdrips_nfy, 
    VOID*                        pv_nfy_tag );

/*------------------------------------------------------------------*/
/*! @brief  This function not implement
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_vdrips_feed(
    HANDLE_T                     h_vid, 
    SCC_VID_VDRIPS_BUFF_T*       pt_vdrips_buf);

/*------------------------------------------------------------------*/
/*! @brief    This function not implement
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_vdrips_play_ctrl(
    HANDLE_T                      h_vid, 
    SCC_VID_VDRIPS_PLAY_CTRL_T    e_ctrl_mode ); 

/*------------------------------------------------------------------*/
/*! @brief   This function not implement
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_vdrips_duration(
    HANDLE_T                       h_vid, 
    UINT32                         ui4_duration );

/*------------------------------------------------------------------*/
/*! @brief   set force unmute on nosignal.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  b_is_on    specify whether use this setting or not.
 *  @note
 *  @see  x_scc_vid_set_force_unmute_on_no_sig
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_force_unmute_on_no_sig( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   set force unmute on nosignal.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  b_is_on    specify whether use this setting or not.
 *  @note
 *  @see  x_scc_vid_get_force_unmute_on_no_sig
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_force_unmute_on_no_sig( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief     set video picture mode.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_pic_mode           indicate picture mode.
 *  @note
 *  @see    x_scc_vid_set_pic_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_pic_mode( 
    HANDLE_T                       h_vid,
    SCC_VID_PIC_MODE_T             e_pic_mode );

/*------------------------------------------------------------------*/
/*! @brief   set video HDMI mode.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_hdmi_mode           indicate HDMI mode.
 *  @note
 *  @see    x_scc_vid_set_hdmi_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_hdmi_mode( 
    HANDLE_T                       h_vid,
    SCC_VID_HDMI_MODE_T            e_hdmi_mode );

/*------------------------------------------------------------------*/
/*! @brief   set video color system.
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  e_color_sys           indicate color system mode.
 *  @note 
 *  @see   x_scc_vid_set_color_sys
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_color_sys( 
    HANDLE_T                       h_vid,
    SCC_VID_COLOR_SYS_T            e_color_sys );

/*------------------------------------------------------------------*/
/*! @brief    Get video color system.
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  pe_color_sys      Contain the video color system.
 *  @note
 *  @see  x_scc_vid_get_color_sys
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_color_sys( 
    HANDLE_T                       h_vid,
    SCC_VID_COLOR_SYS_T*           pe_color_sys );

/*------------------------------------------------------------------*/
/*! @brief   get video HDMI mode.
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  e_hdmi_mode      contain HDMI mode.
 *  @note
 *  @see    x_scc_vid_get_hdmi_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_hdmi_mode( 
    HANDLE_T                       h_vid,
    SCC_VID_HDMI_MODE_T*           pe_hdmi_mode );

/*------------------------------------------------------------------*/
/*! @brief   get video picture mode
 *  @param [in]  h_vid                video component handle.
 *  @param [out]  pe_pic_mode      contain picture mode.
 *  @note
 *  @see    x_scc_vid_get_pic_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_pic_mode( 
    HANDLE_T                       h_vid,
    SCC_VID_PIC_MODE_T*            pe_pic_mode );

/*------------------------------------------------------------------*/
/*! @brief  set digital film mode
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_film_mode    specify film mode.
 *  @note
 *  @see  x_scc_vid_set_di_film_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_di_film_mode( 
    HANDLE_T                       h_vid,
    SCC_VID_DI_FILM_MODE_T         e_film_mode );

/*------------------------------------------------------------------*/
/*! @brief get  digital film mode
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  e_film_mode    specify film mode.
 *  @note
 *  @see  x_scc_vid_get_di_film_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_di_film_mode( 
    HANDLE_T                       h_vid,
    SCC_VID_DI_FILM_MODE_T*        pe_film_mode );

/*------------------------------------------------------------------*/
/*! @brief   set video di ma mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_di_ma    specify di ma mode.
 *  @note
 *  @see  x_scc_vid_set_di_ma
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_di_ma( 
    HANDLE_T                       h_vid,
    SCC_VID_DI_MA_T                e_di_ma );

/*------------------------------------------------------------------*/
/*! @brief   get video di ma mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  e_di_ma    specify di ma mode.
 *  @note
 *  @see  x_scc_vid_get_di_ma
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_di_ma( 
    HANDLE_T                       h_vid,
    SCC_VID_DI_MA_T*               pe_di_ma );

/*------------------------------------------------------------------*/
/*! @brief  set video di edge preserve mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_edge    specify edge preserve  mode.
 *  @note
 *  @see  x_scc_vid_set_di_edge_preserve
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_di_edge_preserve( 
    HANDLE_T                       h_vid,
    SCC_VID_DI_EDGE_PRESERVE_T     e_edge );

/*------------------------------------------------------------------*/
/*! @brief  get video di edge preserve mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  e_edge    specify edge preserve  mode.
 *  @note
 *  @see  x_scc_vid_get_di_edge_preserve
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_di_edge_preserve( 
    HANDLE_T                       h_vid,
    SCC_VID_DI_EDGE_PRESERVE_T*    pe_edge );

/*------------------------------------------------------------------*/
/*! @brief   set video MJC info.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_mjc    contain MJC info.
 *  @note
 *  @see   x_scc_vid_set_mjc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_mjc( 
    HANDLE_T                       h_vid,
    const SCC_VID_MJC_T*           pt_mjc );
/*------------------------------------------------------------------*/
/*! @brief   get video MJC info.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pt_mjc    contain MJC info.
 *  @note
 *  @see   x_scc_vid_get_mjc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_mjc( 
    HANDLE_T                       h_vid,
    SCC_VID_MJC_T*                 pt_mjc );

/*------------------------------------------------------------------*/
/*! @brief    set video xvycc mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_xvycc    specify xvycc mode.
 *  @note
 *  @see  x_scc_vid_set_xvycc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_xvycc( 
    HANDLE_T                       h_vid,
    SCC_VID_XVYCC_T                e_xvycc );

/*------------------------------------------------------------------*/
/*! @brief    get video xvycc mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  e_xvycc    specify xvycc mode.
 *  @note
 *  @see  x_scc_vid_get_xvycc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_xvycc( 
    HANDLE_T                       h_vid,
    SCC_VID_XVYCC_T*               pe_xvycc );

/*------------------------------------------------------------------*/
/*! @brief    set video wcg mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_wcg    specify wcg mode.
 *  @note
 *  @see  x_scc_vid_set_wcg
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_wcg( 
    HANDLE_T                       h_vid,
    SCC_VID_WCG_T                  e_wcg );

/*------------------------------------------------------------------*/
/*! @brief    get video wcg mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_wcg    specify wcg mode.
 *  @note
 *  @see  x_scc_vid_get_wcg
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_wcg( 
    HANDLE_T                       h_vid,
    SCC_VID_WCG_T*                 pe_wcg );

/*------------------------------------------------------------------*/
/*! @brief    set video game mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_game    specify game mode.
 *  @note
 *  @see  x_scc_vid_set_game_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_game_mode( 
    HANDLE_T                       h_vid,
    SCC_VID_GAME_MODE_T            e_game );

/*------------------------------------------------------------------*/
/*! @brief    get video game mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_game    specify game mode.
 *  @note
 *  @see  x_scc_vid_get_game_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_game_mode( 
    HANDLE_T                       h_vid,
    SCC_VID_GAME_MODE_T*           pe_game );

/*------------------------------------------------------------------*/
/*! @brief    set video blue stretch mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_stretch    specify blue stretch mode.
 *  @note
 *  @see  x_scc_vid_set_blue_stretch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_blue_stretch( 
    HANDLE_T                       h_vid,
    SCC_VID_BLUE_STRETCH_T         e_stretch );

/*------------------------------------------------------------------*/
/*! @brief    get video blue stretch mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_stretch    specify blue stretch mode.
 *  @note
 *  @see  x_scc_vid_get_blue_stretch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_blue_stretch( 
    HANDLE_T                       h_vid,
    SCC_VID_BLUE_STRETCH_T*        pe_stretch );

/*------------------------------------------------------------------*/
/*! @brief    set video Mpeg NR mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_nr    specify Mpeg NR mode.
 *  @note
 *  @see  x_scc_vid_set_mpeg_nr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_mpeg_nr( 
    HANDLE_T                       h_vid,
    SCC_VID_MPEG_NR_T              e_nr );

/*------------------------------------------------------------------*/
/*! @brief    get video Mpeg NR mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_nr    specify Mpeg NR mode.
 *  @note
 *  @see  x_scc_vid_get_mpeg_nr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_mpeg_nr( 
    HANDLE_T                       h_vid,
    SCC_VID_MPEG_NR_T*             pe_nr );

/*------------------------------------------------------------------*/
/*! @brief    set video super freeze info.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_freeze    contain super freeze info.
 *  @note
 *  @see  x_scc_vid_set_super_freeze
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_super_freeze( 
    HANDLE_T                   h_vid,
    const SCC_VID_SUPER_FREEZE_T*   pt_freeze );

/*------------------------------------------------------------------*/
/*! @brief   set video force unmute
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_unmute_type  set unmute type.
 *  @param [out]  b_is_on    specify whether use this setting or not.
 *  @note
 *  @see    x_scc_vid_set_force_unmute
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_force_unmute(
    HANDLE_T                    h_vid,
    SCC_VID_FORCE_UNMUTE_TYPE_T e_unmute_type,
    BOOL                        b_is_on);

/*------------------------------------------------------------------*/
/*! @brief    get video whether has set super freeze feature.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pb_is_on    specify whether use this setting or not
 *  @note
 *  @see  x_scc_vid_get_super_freeze
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_super_freeze( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief    get video whether has set hdcp key.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pb_is_exist    specify whether use hdcp key or not
 *  @note
 *  @see  x_scc_vid_get_hdcp_key_exist
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_hdcp_key_exist( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_exist );

/*------------------------------------------------------------------*/
/*! @brief   set video black stretch mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_stretch    specify black stretch mode
 *  @note
 *  @see   x_scc_vid_set_black_stretch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_black_stretch( 
    HANDLE_T                       h_vid,
    SCC_VID_BLACK_STRETCH_T        e_stretch );

/*------------------------------------------------------------------*/
/*! @brief   get video black stretch mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_stretch    specify black stretch mode
 *  @note
 *  @see   x_scc_vid_get_black_stretch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_black_stretch( 
    HANDLE_T                       h_vid,
    SCC_VID_BLACK_STRETCH_T*       pe_stretch );

/*------------------------------------------------------------------*/
/*! @brief   set video white stretch mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_stretch    specify white stretch mode
 *  @note
 *  @see   x_scc_vid_set_white_stretch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_white_stretch( 
    HANDLE_T                       h_vid,
    SCC_VID_WHITE_STRETCH_T        e_stretch );

/*------------------------------------------------------------------*/
/*! @brief   get video white stretch mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_stretch    specify white stretch mode
 *  @note
 *  @see   x_scc_vid_get_white_stretch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_white_stretch( 
    HANDLE_T                       h_vid,
    SCC_VID_WHITE_STRETCH_T*       pe_stretch );

/*------------------------------------------------------------------*/
/*! @brief   set video black stretch val mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui2_stretch    specify black stretch mode
 *  @note
 *  @see   x_scc_vid_set_black_stretch_val
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_black_stretch_val( 
    HANDLE_T                       h_vid,
    UINT16                         ui2_stretch );

/*------------------------------------------------------------------*/
/*! @brief   get video black stretch val mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui2_stretch    specify black stretch mode
 *  @note
 *  @see   x_scc_vid_get_black_stretch_val
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_black_stretch_val( 
    HANDLE_T                       h_vid,
    UINT16*                        pui2_stretch );

/*------------------------------------------------------------------*/
/*! @brief   set video white stretch val mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui2_stretch    specify white stretch mode
 *  @note
 *  @see   x_scc_vid_set_white_stretch_val
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_white_stretch_val( 
    HANDLE_T                       h_vid,
    UINT16                         ui2_stretch );

/*------------------------------------------------------------------*/
/*! @brief   get video white stretch val mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui2_stretch    specify white stretch mode
 *  @note
 *  @see   x_scc_vid_get_white_stretch_val
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_white_stretch_val( 
    HANDLE_T                       h_vid,
    UINT16*                        pui2_stretch );

/*------------------------------------------------------------------*/
/*! @brief   set video display region 
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_region_info    contain display region info.
 *  @note
 *  @see    x_scc_vid_set_disp_region
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_disp_region( 
    HANDLE_T                       h_vid,
    const VSH_REGION_INFO_T*       pt_region_info );

/*------------------------------------------------------------------*/
/*! @brief    set video plane atomic command.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_atomic_cmd    contain atomic command info.
 *  @note
 *  @see  x_scc_vid_set_vdp_atomic_cmd
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_vdp_atomic_cmd( 
    HANDLE_T                       h_vid,
    SM_COMMAND_T*                  pt_atomic_cmd );

/*------------------------------------------------------------------*/
/*! @brief  set video colorspace mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_colorspace    specify colorspace mode
 *  @note
 *  @see  x_scc_vid_set_colorspace
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_colorspace( 
    HANDLE_T                       h_vid,
    SCC_VID_COLORSPACE_T           e_colorspace );

/*------------------------------------------------------------------*/
/*! @brief   set video flip.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  b_is_on    specify whether use video flip or not
 *  @note
 *  @see  x_scc_vid_set_flip
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_flip( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   set video mirror.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  b_is_on    specify whether use video mirror or not
 *  @note
 *  @see  x_scc_vid_set_mirror
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_mirror( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   get video flip.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  b_is_on    specify whether use video flip or not
 *  @note
 *  @see  x_scc_vid_get_flip
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_flip( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief   get video mirror.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  b_is_on    specify whether use video mirror or not
 *  @note
 *  @see  x_scc_vid_get_mirror
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_mirror( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief   set local dimming
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_lcdim_mode    specify local dimming mode
 *  @note
 *  @see  x_scc_vid_set_local_dimming_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_local_dimming_mode(
    HANDLE_T                        h_vid,
    SCC_VID_LOCAL_DIMMING_TYPE_T    e_lcdim_mode );

/*------------------------------------------------------------------*/
/*! @brief   get local dimming mode.
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_lcdim_mode    specify local dimming mode
 *  @note
 *  @see  x_scc_vid_get_local_dimming_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_local_dimming_mode( 
    HANDLE_T                        h_vid,
    SCC_VID_LOCAL_DIMMING_TYPE_T*   pe_lcdim_mode );

/*------------------------------------------------------------------*/
/*! @brief   set source detect notify function.
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pf_scc_vid_src_detect_nfy  notify function pointer
 *  @param [in]  pv_nfy_tag  vnotify function tag.
 *  @note
 *  @see  x_scc_vid_set_src_detect_nfy
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_src_detect_nfy(
    HANDLE_T                        h_vid, 
    x_scc_vid_src_detect_nfy_fct    pf_scc_vid_src_detect_nfy, 
    VOID*                           pv_nfy_tag);

/*------------------------------------------------------------------*/
/*! @brief   get vdp delay time such as after set mode blank
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui4_delay_time  delay time ms.
 *  @note
 *  @see  x_scc_vid_get_vdp_delay_time
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_vdp_delay_time(
    HANDLE_T                   h_vid,
    UINT32*                    pui4_delay_time );

/*------------------------------------------------------------------*/
/*! @brief   set 3d navigation off or on
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_nav  3d navigation off or on
 *  @note
 *  @see  x_scc_vid_set_3d_nav
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_nav( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_T                e_nav );

/*------------------------------------------------------------------*/
/*! @brief   get 3d navigation off or on
 *  @param [in]  h_vid  video component handle.
 *  @param [out] pe_nav  3d navigation off or on
 *  @note
 *  @see  x_scc_vid_get_3d_nav
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_nav( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_T*               pe_nav );

/*------------------------------------------------------------------*/
/*! @brief   set 3d mode 
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_3d_mode  3d mode.
 *  @note
 *  @see  x_scc_vid_set_3d_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_mode( 
    HANDLE_T                    h_vid,
    SCC_3D_MODE_T               e_3d_mode );

/*------------------------------------------------------------------*/
/*! @brief   get 3d mode 
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_3d_mode  3d mode.
 *  @note
 *  @see  x_scc_vid_get_3d_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_mode( 
    HANDLE_T                    h_vid,
    SCC_3D_MODE_T*              pe_3d_mode );

/*------------------------------------------------------------------*/
/*! @brief   set 3d parallax 
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui4_prlx  3d parallax
 *  @note
 *  @see  x_scc_vid_set_3d_parallax
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_parallax( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_prlx );

/*------------------------------------------------------------------*/
/*! @brief   get 3d parallax 
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui4_prlx  3d parallax
 *  @note
 *  @see  x_scc_vid_get_3d_parallax
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_parallax( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_prlx );

/*------------------------------------------------------------------*/
/*! @brief   set 3d depth of field
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui4_fld_depth  3d depth of field
 *  @note
 *  @see  x_scc_vid_set_3d_fld_depth
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_fld_depth( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_fld_depth );

/*------------------------------------------------------------------*/
/*! @brief   get 3d depth of field
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui4_fld_depth  3d depth of field
 *  @note
 *  @see  x_scc_vid_get_3d_fld_depth
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_fld_depth( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_fld_depth );

/*------------------------------------------------------------------*/
/*! @brief   set 3d l-r switch
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_lr_switch  3d l-r switch
 *  @note
 *  @see  x_scc_vid_set_3d_lr_switch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_lr_switch( 
    HANDLE_T                    h_vid,
    SCC_3D_LR_SWITCH_T          e_lr_switch );

/*------------------------------------------------------------------*/
/*! @brief   get 3d l-r switch
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_lr_switch  3d l-r switch
 *  @note
 *  @see  x_scc_vid_get_3d_lr_switch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_lr_switch( 
    HANDLE_T                    h_vid,
    SCC_3D_LR_SWITCH_T*         pe_lr_switch );

/*------------------------------------------------------------------*/
/*! @brief   set 3d to 2d
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_lr_switch  3d to 2d
 *  @note
 *  @see  x_scc_vid_set_3d_to_2d
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_to_2d( 
    HANDLE_T                    h_vid,
    SCC_3D_TO_2D_T              e_3d_to_2d );

/*------------------------------------------------------------------*/
/*! @brief   get 3d to 2d
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_lr_switch  3d to 2d
 *  @note
 *  @see  x_scc_vid_get_3d_to_2d
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_to_2d( 
    HANDLE_T                    h_vid,
    SCC_3D_TO_2D_T*             pe_3d_to_2d );

/*------------------------------------------------------------------*/
/*! @brief   set 3d pfr
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_pfr  3d pfr
 *  @note
 *  @see  x_scc_vid_set_3d_pfr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_fpr( 
    HANDLE_T                    h_vid,
    SCC_3D_FPR_T                e_fpr );

/*------------------------------------------------------------------*/
/*! @brief   get 3d pfr
 *  @param [in]  h_vid   video component handle.
 *  @param [in]  pe_pfr  3d pfr
 *  @note
 *  @see  x_scc_vid_get_3d_pfr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_fpr( 
    HANDLE_T                    h_vid,
    SCC_3D_FPR_T*               pe_fpr );

/*------------------------------------------------------------------*/
/*! @brief   set 3d protrude from screen
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui4_pro_scr  3d protrude from screen
 *  @note
 *  @see  x_scc_vid_set_3d_protrude_scr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_protrude_scr( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_pro_scr );

/*------------------------------------------------------------------*/
/*! @brief   get 3d protrude from screen
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui4_pro_scr  3d protrude from screen
 *  @note
 *  @see  x_scc_vid_get_3d_protrude_scr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_protrude_scr( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_pro_scr );

/*------------------------------------------------------------------*/
/*! @brief   set 3d distance to tv
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui4_dis_2_tv  3d distance to tv
 *  @note
 *  @see  x_scc_vid_set_3d_distance_2_tv
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_distance_2_tv( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_dis_2_tv );

/*------------------------------------------------------------------*/
/*! @brief   get 3d distance to tv
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui4_dis_2_tv  3d distance to tv
 *  @note
 *  @see  x_scc_vid_get_3d_distance_2_tv
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_distance_2_tv( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_dis_2_tv );

/*------------------------------------------------------------------*/
/*! @brief   set 3d osd depth
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui4_osd_depth  3d osd depth
 *  @note
 *  @see  x_scc_vid_set_3d_osd_depth
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_osd_depth( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_osd_depth );

/*------------------------------------------------------------------*/
/*! @brief   get 3d osd depth
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pui4_osd_depth  3d osd depth
 *  @note
 *  @see  x_scc_vid_get_3d_osd_depth
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_osd_depth( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_osd_depth );

/*------------------------------------------------------------------*/
/*! @brief   get 3d fmt capability. 
 *  @param [in]  h_vid  video component handle.
 *  @param [out] pui4_fmt_cap  3d fmt capability.
 *  @note
 *  @see  x_scc_vid_get_3d_fmt_cap
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_fmt_cap( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_fmt_cap );

/*------------------------------------------------------------------*/
/*! @brief   get 3d ctrl capability. 
 *  @param [in]  h_vid  video component handle.
 *  @param [out] pui4_ctrl_cap  3d ctrl capability.
 *  @note
 *  @see  x_scc_vid_get_3d_ctrl_cap
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_ctrl_cap( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_ctrl_cap );

/*------------------------------------------------------------------*/
/*! @brief   set 3d nav auto change enable
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_auto_chg  auto change enable or not.
 *  @note
 *  @see  x_scc_vid_set_3d_nav_auto_chg_en
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_nav_auto_chg_en( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_AUTO_CHG_T       e_auto_chg );

/*------------------------------------------------------------------*/
/*! @brief   get 3d nav auto change enable
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_auto_chg  auto change enable or not.
 *  @note
 *  @see  x_scc_vid_get_3d_nav_auto_chg_en
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_nav_auto_chg_en( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_AUTO_CHG_T*      pe_auto_chg );

/*------------------------------------------------------------------*/
/*! @brief   set 3d image safety enable
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_img_safe  image safety enable.
 *  @note
 *  @see  x_scc_vid_set_3d_image_safety_en
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_3d_image_safety_en( 
    HANDLE_T                    h_vid,
    SCC_3D_IMAGE_SAFETY_T       e_img_safe );

/*------------------------------------------------------------------*/
/*! @brief   get 3d image safety enable
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_img_safe  image safety enable.
 *  @note
 *  @see  x_scc_vid_get_3d_image_safety_en
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_image_safety_en( 
    HANDLE_T                    h_vid,
    SCC_3D_IMAGE_SAFETY_T*      pe_img_safe );

/*------------------------------------------------------------------*/
/*! @brief   get 3d nav tag
 *  @param [in]  h_vid  video component handle.
 *  @param [out]  pe_nav_tag  3d nav tag.
 *  @note
 *  @see  x_scc_vid_get_3d_nav_tag
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_nav_tag( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_TAG_T*           pe_nav_tag );

/*------------------------------------------------------------------*/
/*! @brief   get video decoder capability information
 *  @param [in]     h_vid   video component handle.
 *  @param [out]  pt_vid_dec_cap  video decoder capability information
 *  @note
 *  @see  x_scc_vid_get_vid_dec_cap
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_vid_dec_cap( 
    HANDLE_T                              h_vid,
    SCC_VID_DEC_CAPABILITY_INFO_T*        pt_vid_dec_cap );

/*------------------------------------------------------------------*/
/*! @brief   Set pedestal
 *  @param [in]  h_vid                video component handle.
 *  @param [in]  b_is_on             whether set this feature or not.
 *  @note
 *  @see   x_scc_vid_set_pedestal
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_pedestal(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   set hdmi range
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_pfr  hdmi range
 *  @note
 *  @see  x_scc_vid_set_hdmi_range
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_hdmi_range( 
    HANDLE_T                    h_vid,
    SCC_HDMI_RANGE_T            e_hdmi_range );

/*------------------------------------------------------------------*/
/*! @brief   get overscan enable/disable infomation.
 *  @param [in]  h_vid          video component handle.
 *  @param [out] pb_is_enable   Indicate overscan is enable or disable.
 *  @note
 *  @see   x_scc_vid_get_overscan_enable
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_overscan_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable );

/*------------------------------------------------------------------*/
/*! @brief   get non-linear scaling enable/disable information.
 *  @param [in]  h_vid          video component handle.
 *  @param [out] pb_is_enable   Indicate non-linear scaling is enable or disable.
 *  @note
 *  @see   x_scc_vid_get_nonlinear_scaling_enable
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_nonlinear_scaling_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable );

/*------------------------------------------------------------------*/
/*! @brief   get 3d panel type.
 *  @param [in]  h_vid           video component handle.
 *  @param [out] pe_panel_type   3d panel type.
 *  @note
 *  @see   x_scc_vid_get_3d_panel_type
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_panel_type( 
    HANDLE_T                    h_vid,
    SCC_3D_PANEL_TYPE_T*        pe_panel_type );

/*------------------------------------------------------------------*/
/*! @brief   set vga detect enable/disable
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  e_vga_detect  vga detect enable/disable
 *  @note
 *  @see  x_scc_vid_set_vga_detect
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_vga_detect( 
    HANDLE_T                    h_vid,
    SCC_VGA_DETECT_T            e_vga_detect );

/*------------------------------------------------------------------*/
/*! @brief   get vga status
 *  @param [in]  h_vid           video component handle.
 *  @param [out] pe_vga_status   vga status.
 *  @note
 *  @see   x_scc_vid_get_vga_status
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_vga_status( 
    HANDLE_T                    h_vid,
    SCC_VGA_STATUS_T*           pe_vga_status );

/*------------------------------------------------------------------*/
/*! @brief   set static gamma
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui1_static_gamma  static gamma
 *  @note
 *  @see  x_scc_vid_set_static_gamma
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_static_gamma( 
    HANDLE_T                    h_vid,
    UINT8                       ui1_static_gamma );

/*------------------------------------------------------------------*/
/*! @brief   set angular sce
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  pt_angular_sce  angular sce
 *  @note
 *  @see  x_scc_vid_set_angular_sce
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_angular_sce( 
    HANDLE_T                    h_vid,
    SCC_ANGULAR_SCE_T*          pt_angular_sce );

/*------------------------------------------------------------------*/
/*! @brief   set down bandwidth mode
 *  @param [in]  h_vid  video component handle.
 *  @param [in]  ui1_down_bw_mode  down bandwidth mode
 *  @note
 *  @see  x_scc_vid_set_down_bw_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_down_bw_mode( 
    HANDLE_T                    h_vid,
    UINT8                       ui1_down_bw_mode );

/*------------------------------------------------------------------*/
/*! @brief   get 3d mjc enable/disable infomation.
 *  @param [in]  h_vid          video component handle.
 *  @param [out] pb_is_enable   Indicate 3d mjc is enable or disable.
 *  @note
 *  @see   x_scc_vid_get_3d_mjc_enable
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_3d_mjc_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable );

/*------------------------------------------------------------------*/
/*! @brief   get video v-freqency and h-freqency.
 *  @param [in]  h_vid  video component handle.
 *  @param [out] pt_freq  contain video v-freq and h-freq info.
 *  @note
 *  @see   x_scc_vid_get_freq
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_freq( 
    HANDLE_T                       h_vid,
    SCC_VID_FREQ_T*                pt_freq );

/*------------------------------------------------------------------*/
/*! @brief   get source detect status.
 *  @param [in]  h_vid  video component handle.
 *  @param [out] pt_src_status  contain source status.
 *  @note
 *  @see   x_scc_vid_get_src_detect_status
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_src_detect_status( 
    HANDLE_T                       h_vid,
    SCC_VID_SRC_DET_STATUS_T*      pt_src_status );

/*------------------------------------------------------------------*/
/*! @brief   set super resolution.
 *  @param [in]  h_vid             video component handle.
 *  @param [in]  b_is_on           indicate whether use this setting or not.
 *  @note
 *  @see   x_scc_vid_set_super_resolution
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_set_super_resolution( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   get aspect ration enable
 *  @param [in]  h_vid                video component handle.
 *  @param [out]pb_is_enable   retrieval whether support set aspect ratio.
 *  @note
 *  @see   x_scc_vid_get_asp_ratio_enable
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_asp_ratio_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable );

/*------------------------------------------------------------------*/
/*! @brief  get screen mode enable
 *  @param [in]  h_vid           video component handle.
 *  @param [out] pt_scr_enable   retrieval whether support designated screen mode.
 *  @note
 *  @see   x_scc_vid_get_screen_mode_enable
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_vid_get_screen_mode_enable( 
    HANDLE_T                   h_vid,
    SCC_VID_SCR_MODE_T*        pt_scr_enable );

/* Display related API *******************************************************/
/*------------------------------------------------------------------*/
/*! @brief  set display control mode.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_ctrl    specify control mode.
 *  @note
 *  @see  x_scc_disp_set_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_ctrl( 
    HANDLE_T                        h_disp,
    SCC_DISP_CTRL_TYPE_T            e_ctrl );

/*------------------------------------------------------------------*/
/*! @brief set display attribute value.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_ctrl    specify control mode.
 *  @param [in]  ui2_value    specify attribute value.
 *  @note
 *  @see  x_scc_disp_set_attrib
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_attrib( 
    HANDLE_T                        h_disp,
    SCC_DISP_ATTRIB_T               e_attrib,
    UINT16                          ui2_value );

/*------------------------------------------------------------------*/
/*! @brief    set display range info.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  pt_range    contain display range info.
 *  @note
 *  @see  x_scc_disp_set_adp_back_light_rng
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_adp_back_light_rng( 
    HANDLE_T                        h_disp,
    const SCC_DISP_RANGE_INFO_T*    pt_range );

/*------------------------------------------------------------------*/
/*! @brief  set display resolution
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  pt_resolution    contain display resolution info.
 *  @note
 *  @see  x_scc_disp_set_resolution
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_resolution( 
    HANDLE_T                        h_disp,
    const SCC_DISP_RESOLUTION_T*    pt_resolution );

/*------------------------------------------------------------------*/
/*! @brief   set frame rate.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  ui4_frame_rate  frame rate.
 *  @note
 *  @see  x_scc_disp_set_frame_rate
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32                      ui4_frame_rate );

/*------------------------------------------------------------------*/
/*! @brief   get frame rate.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pui4_frame_rate  frame rate.
 *  @note
 *  @see  x_scc_disp_get_frame_rate
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32*                     pui4_frame_rate );

/*------------------------------------------------------------------*/
/*! @brief   set force frame rate.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  ui4_force_fr force frame rate.
 *  @note
 *  @see  x_scc_disp_set_force_frame_rate
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_force_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32                      ui4_force_fr );

/*------------------------------------------------------------------*/
/*! @brief   set display aspect ratio.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_aspect_ratio    specify ratio mode.
 *  @note
 *  @see  x_scc_disp_set_aspect_ratio
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_aspect_ratio( 
    HANDLE_T                        h_disp,
    SCC_DISP_ASPECT_RATIO_T         e_aspect_ratio );

/*------------------------------------------------------------------*/
/*! @brief  set display format
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_fmt    specify format mode.
 *  @note
 *  @see  x_scc_disp_set_fmt
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_fmt( 
    HANDLE_T                        h_disp,
    SCC_DISP_FMT_T                  e_fmt );

/*------------------------------------------------------------------*/
/*! @brief     set digital output.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  ui4_digit_out_mask    specify digital out mask.
 *  @note
 *  @see  x_scc_disp_set_digit_output
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_digit_output( 
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUT_MASK_T       ui4_digit_out_mask );

/*------------------------------------------------------------------*/
/*! @brief   set display background.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  pt_bg    contain background info.
 *  @note
 *  @see  x_scc_disp_set_bg
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_bg( 
    HANDLE_T                        h_disp,
    const SCC_BG_COLOR_T*           pt_bg );

/*------------------------------------------------------------------*/
/*! @brief  set display tv type
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_tv_type    specify tv type.
 *  @note
 *  @see  x_scc_disp_set_tv_type
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_tv_type( 
    HANDLE_T                        h_disp,
    SCC_DISP_TV_TYPE_T              e_tv_type );

/*------------------------------------------------------------------*/
/*! @brief  set display digital output format.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_digit_out_fmt    specify format mode..
 *  @note
 *  @see  x_scc_disp_set_digit_output_fmt
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_digit_output_fmt( 
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUTPUT_FMT_T     e_digit_out_fmt );

/*------------------------------------------------------------------*/
/*! @brief  set display color gain
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  pt_color_gain    contain color gain info.
 *  @note
 *  @see  x_scc_disp_set_color_gain
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_color_gain( 
    HANDLE_T                        h_disp,
    const SCC_DISP_COLOR_GAIN_T*    pt_color_gain );

/*------------------------------------------------------------------*/
/*! @brief   set display color offset
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  pt_color_offset    contain color offset info.
 *  @note
 *  @see  x_scc_disp_set_color_offset
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_color_offset( 
    HANDLE_T                        h_disp,
    const SCC_DISP_COLOR_GAIN_T*    pt_color_offset );

/*------------------------------------------------------------------*/
/*! @brief   set display tv mode.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_tv_mode    specify tv mode.
 *  @note
 *  @see  x_scc_disp_set_tv_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_tv_mode(
    HANDLE_T                        h_disp,
    SCC_TV_MODE_T                   e_tv_mode );

/*------------------------------------------------------------------*/
/*! @brief   set display mjc
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_mjc    specify mjc mode.
 *  @note
 *  @see  x_scc_disp_set_mjc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_mjc(
    HANDLE_T                        h_disp,
    SCC_DISP_MJC_T                  e_mjc );

/*------------------------------------------------------------------*/
/*! @brief  set display test pattern mode.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_pattern    specify pattern mode.
 *  @note
 *  @see  x_scc_disp_set_test_pattern
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_test_pattern(
    HANDLE_T                        h_disp,
    SCC_DISP_TEST_PATTERN_T         e_pattern );

#ifdef MW_TV_AV_BYPASS_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  set display bypass mode.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_mode    specify bypass mode.
 *  @note
 *  @see  x_scc_disp_set_bypass_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_bypass_mode(
    HANDLE_T                        h_disp,
    SCC_DISP_BYPASS_MODE_T          e_mode );
/*------------------------------------------------------------------*/
/*! @brief  set display tvencode disable flag
 *  @param [out]  b_not_disable_tve   disable or not tvencode.
 *  @note
 *  @see    x_scc_disp_set_tve_not_disable_flag
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern VOID x_scc_disp_set_tve_not_disable_flag( 
    BOOL                            b_not_disable_tve );

/*------------------------------------------------------------------*/
/*! @brief  set display tvencode enable flag
 *  @param [out]  b_enable_tve_as_play   enable or not tvencode.
 *  @note
 *  @see    x_scc_disp_set_tve_enable_flag
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern VOID x_scc_disp_set_tve_enable_flag( 
    BOOL                            b_enable_tve_as_play );

/*------------------------------------------------------------------*/
/*! @brief  set display tvencode ctrol mode
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_tve_ctrl_mode    specify ctrol  mode.
 *  @note
 *  @see      x_scc_disp_set_tve_ctrl_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_tve_ctrl_mode(
    HANDLE_T                        h_disp,
    SCC_DISP_TVE_CTRL_MODE_T        e_tve_ctrl_mode );
#endif

/*------------------------------------------------------------------*/
/*! @brief  get display control.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pe_ctrl   retrieval control mode.
 *  @note
 *  @see  x_scc_disp_get_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_ctrl( 
    HANDLE_T                        h_disp,
    SCC_DISP_CTRL_TYPE_T*           pe_ctrl );

/*------------------------------------------------------------------*/
/*! @brief   get display max gamma value.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pui1_max_gamma   retrieval max gamma value.
 *  @note
 *  @see  x_scc_disp_get_max_gamma
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_max_gamma( 
    HANDLE_T                        h_disp,
    UINT8*                          pui1_max_gamma );

/*------------------------------------------------------------------*/
/*! @brief  get display attribute value.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  e_attrib  specify attribute type
 *  @param [out] pui2_value   attribute value.
 *  @note
 *  @see  x_scc_disp_get_attrib
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_attrib( 
    HANDLE_T                        h_disp,
    SCC_DISP_ATTRIB_T               e_attrib,
    UINT16*                         pui2_value );

/*------------------------------------------------------------------*/
/*! @brief  get display back light range.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pt_range   retrieval range info.
 *  @note
 *  @see  x_scc_disp_get_adp_back_light_rng
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_adp_back_light_rng( 
    HANDLE_T                        h_disp,
    SCC_DISP_RANGE_INFO_T*          pt_range );

/*------------------------------------------------------------------*/
/*! @brief  get display resolution.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pt_resolution   contain resolution info.
 *  @note
 *  @see  x_scc_disp_get_resolution
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_resolution( 
    HANDLE_T                        h_disp,
    SCC_DISP_RESOLUTION_T*          pt_resolution );

/*------------------------------------------------------------------*/
/*! @brief  get display aspect ratio.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pe_aspect_ratio   specify aspect ratio info.
 *  @note
 *  @see  x_scc_disp_get_aspect_ratio
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_aspect_ratio( 
    HANDLE_T                        h_disp,
    SCC_DISP_ASPECT_RATIO_T*        pe_aspect_ratio );

/*------------------------------------------------------------------*/
/*! @brief  get display format
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pe_fmt   specify format mode.
 *  @note
 *  @see  x_scc_disp_get_fmt
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_fmt( 
    HANDLE_T                        h_disp,
    SCC_DISP_FMT_T*                 pe_fmt );

/*------------------------------------------------------------------*/
/*! @brief   get display digit out mask
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pui4_digit_out_mask   specify mask value.
 *  @note
 *  @see  x_scc_disp_get_digit_output
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_digit_output( 
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUT_MASK_T*      pui4_digit_out_mask );

/*------------------------------------------------------------------*/
/*! @brief   get display background.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pt_bg   contain background info.
 *  @note
 *  @see  x_scc_disp_get_bg
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_bg( 
    HANDLE_T                        h_disp,
    SCC_BG_COLOR_T*                 pt_bg );

/*------------------------------------------------------------------*/
/*! @brief  get display tv type.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pe_tv_type   specify tv type.
 *  @note
 *  @see  x_scc_disp_get_tv_type
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_tv_type( 
    HANDLE_T                        h_disp,
    SCC_DISP_TV_TYPE_T*             pe_tv_type );

/*------------------------------------------------------------------*/
/*! @brief  get display format  capability.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pui4_fmt_cap   contain format info.
 *  @note
 *  @see x_scc_disp_get_fmt_cap
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_fmt_cap( 
    HANDLE_T                        h_disp,
    SCC_DISP_FMT_CAP_T*             pui4_fmt_cap );

/*------------------------------------------------------------------*/
/*! @brief  get display digital output format.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pe_digit_out_fmt   specify digital output format mode.
 *  @note
 *  @see  x_scc_disp_get_digit_output_fmt
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_digit_output_fmt( 
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUTPUT_FMT_T*    pe_digit_out_fmt );

/*------------------------------------------------------------------*/
/*! @brief     get display dither cap.
 *  @param [in]  h_disp  display component handle.
 *  @param [out]   pui4_dither_cap  specify dither cap value.
 *  @note
 *  @see  x_scc_disp_get_dither_cap
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_dither_cap( 
    HANDLE_T          h_disp,
    UINT32*           pui4_dither_cap );

/*------------------------------------------------------------------*/
/*! @brief  get display color gain.
 *  @param [in]  h_disp  display component handle.
 *  @param [out]pt_color_gain contain color gain info.
 *  @note
 *  @see   x_scc_disp_get_color_gain
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_color_gain( 
    HANDLE_T                        h_disp,
    SCC_DISP_COLOR_GAIN_T*          pt_color_gain );

/*------------------------------------------------------------------*/
/*! @brief  get display color offset
 *  @param [in]  h_disp  display component handle.
 *  @param [out]pt_color_offset contain color offset info.
 *  @note
 *  @see  x_scc_disp_get_color_offset
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_color_offset( 
    HANDLE_T                        h_disp,
    SCC_DISP_COLOR_GAIN_T*          pt_color_offset );

/*------------------------------------------------------------------*/
/*! @brief   get display tv mode.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pe_tv_mode   specify tv mode.
 *  @note
 *  @see  x_scc_disp_get_tv_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_tv_mode(
    HANDLE_T                        h_disp,
    SCC_TV_MODE_T*                  pe_tv_mode );

/*------------------------------------------------------------------*/
/*! @brief   get display mjc.
 *  @param [in]  h_disp  display component handle.
 *  @param [out]pe_mjc   specify display mjc.
 *  @note
 *  @see  x_scc_disp_get_mjc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_mjc(
    HANDLE_T                        h_disp,
    SCC_DISP_MJC_T*                 pe_mjc );

/*------------------------------------------------------------------*/
/*! @brief   get display test pattern .
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pe_pattern   specify test pattern mode.
 *  @note
 *  @see  x_scc_disp_get_test_pattern
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_test_pattern(
    HANDLE_T                        h_disp,
    SCC_DISP_TEST_PATTERN_T*        pe_pattern );

/*------------------------------------------------------------------*/
/*! @brief   set back light control.
 *  @param [in]  h_disp  display component handle.
 *  @param [in]  b_is_on  specify whether use this setting or not.
 *  @note
 *  @see  x_scc_disp_set_back_light_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_set_back_light_ctrl( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on );

/*------------------------------------------------------------------*/
/*! @brief     get display back light control.
 *  @param [in]  h_disp  display component handle.
 *  @param [out]pb_is_on   specify whether use this setting or not.
 *  @note
 *  @see  x_scc_disp_get_back_light_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_back_light_ctrl( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief  get screen resolution.
 *  @param [in]  h_disp  display component handle.
 *  @param [out] pt_scr_resolution  contain screen resolution info.
 *  @note
 *  @see  x_scc_disp_get_screen_resolution
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_disp_get_screen_resolution( 
    HANDLE_T                        h_disp,
    SCC_DISP_SCREEN_RESOLUTION_T*   pt_scr_resolution );

/* Audio related API *********************************************************/

/*------------------------------------------------------------------*/
/*! @brief   get audio Pro Logic II switch
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out] pe_value             retrieve mode value
 *  @note
 *  @see   x_scc_aud_get_pl2_switch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_pl2_switch( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_SWITCH_T*           pe_value );

/*------------------------------------------------------------------*/
/*! @brief   get audio Pro Logic II mode
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out]  pe_value             retrieve mode value
 *  @note
 *  @see     x_scc_aud_get_pl2_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_pl2_mode( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_MODE_T*             pe_value );

/*------------------------------------------------------------------*/
/*! @brief    get audio Pro Logic II panorama
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out] pe_value             retrieve mode value
 *  @note
 *  @see    x_scc_aud_get_pl2_panorama
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_pl2_panorama( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_PANORAMA_T*         pe_value );

/*------------------------------------------------------------------*/
/*! @brief   get audio Pro Logic II dimension
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out]  pe_value             retrieve mode value
 *  @note
 *  @see   x_scc_aud_get_pl2_dimension
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_pl2_dimension( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_DIMENSION_T*        pe_value );   

/*------------------------------------------------------------------*/
/*! @brief     get audio Pro Logic II center width
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out] pe_value             retrieve mode value
 *  @note
 *  @see    x_scc_aud_get_pl2_c_width
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_pl2_c_width( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_C_WIDTH_T*          pe_value );

/*------------------------------------------------------------------*/
/*! @brief    set audio Pro Logic II switch
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pe_value             specify mode value
 *  @param [out]
 *  @note
 *  @see   x_scc_aud_set_pl2_switch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_pl2_switch( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_SWITCH_T            e_value );

/*------------------------------------------------------------------*/
/*! @brief   set audio Pro Logic II mode
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pe_value             specify mode value
 *  @param [out]
 *  @note
 *  @see   x_scc_aud_set_pl2_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_pl2_mode( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_MODE_T              e_value );


/*------------------------------------------------------------------*/
/*! @brief     set audio Pro Logic II panorama
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pe_value             specify mode value
 *  @note
 *  @see   x_scc_aud_set_pl2_panorama
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_pl2_panorama( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_PANORAMA_T          e_value );

/*------------------------------------------------------------------*/
/*! @brief   set audio Pro Logic II dimension
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pe_value             specify mode value
 *  @note
 *  @see   x_scc_aud_set_pl2_dimension
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_pl2_dimension( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_DIMENSION_T         e_value );   

/*------------------------------------------------------------------*/
/*! @brief   set audio Pro Logic II center width
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pe_value             specify mode value
 *  @note
 *  @see  x_scc_aud_set_pl2_c_width
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_pl2_c_width( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_C_WIDTH_T           e_value );

/*------------------------------------------------------------------*/
/*! @brief    The linear gain converted to a fractional fixed point unsigned integer, as described 
 *              in [BD-ROM3] part 1 I.4. The value to pass is the linear gain multiplied by 8192, and 
 *              rounded to the closest integer. 
 *  @param [in]  h_aud       Audio component handle.
 *  @param [in]  ui4_gain    b0 ~b15 => fraction portion, b16~b31 => integer portion
 *  @note
 *  @see   x_scc_aud_set_clip_gain
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_clip_gain (
    HANDLE_T                     h_aud, 
    UINT32                          ui4_gain );
    
    /*------------------------------------------------------------------*/
    /*! @brief    set audio clip playback data
     *  @param [in]  h_aud                Audio component handle.
     *  @param [in]   pt_aud_clip          Audio clip data structure     
     *  @param [in]  pf_scc_aud_clip_nfy  SCC Audio clip notify function callback
     *  @param [in]   pv_nfy_tag           notify callback tag
     *  @note
     *  @see    x_scc_aud_set_clip
     *  @return
     *  @retval      -SMR_OK              Success.
     *  @retval      -SMR_INTERNAL_ERROR  Fail.
     */
    /*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_clip(
    HANDLE_T                     h_aud, 
    SCC_AUD_CLIP_T*              pt_aud_clip, 
    x_scc_aud_clip_nfy_fct       pf_scc_aud_clip_nfy, 
    VOID*                        pv_nfy_tag );

/*------------------------------------------------------------------*/
/*! @brief   trigger audio clip play control
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_ctrl_mode          play control mode(play/pause/stop)     
 *  @note
 *  @see   x_scc_aud_set_clip_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_clip_ctrl(
    HANDLE_T                      h_aud, 
    SCC_AUD_CTRL_MODE_T           e_ctrl_mode ); 

/*------------------------------------------------------------------*/
/*! @brief    get audio clip play control
 *  @param [in] h_aud                Audio component handle.
 *  @param [out]  pe_ctrl_mode         retrieve play control mode(play/pause/stop)  
 *  @note
 *  @see   x_scc_aud_get_clip_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_clip_ctrl(
    HANDLE_T                      h_aud, 
    SCC_AUD_CTRL_MODE_T*          pe_ctrl_mode ); 

/*------------------------------------------------------------------*/
/*! @brief   get audio clip duration
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out] pui4_duration        retrieve the duration of the audio clip(ms)
 *  @note
 *  @see   x_scc_aud_get_clip_duration
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_clip_duration(
    HANDLE_T                       h_aud, 
    UINT32*                        pui4_duration );

/*------------------------------------------------------------------*/
/*! @brief  get audio clip duration
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out]  pui4_duration        retrieve the current position of the audio clip(ms)
 *  @note
 *  @see   x_scc_aud_get_clip_position
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_clip_position(
    HANDLE_T                        h_aud, 
    UINT32*                         pui4_position ); 

/*------------------------------------------------------------------*/
/*! @brief  get audio clip duration
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  ui4_duration         the current position of the audio clip(ms)
 *  @note
 *  @see  x_scc_aud_set_clip_position
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_clip_position(
    HANDLE_T                        h_aud, 
    UINT32                          ui4_position ); 

/*------------------------------------------------------------------*/
/*! @brief  set audio clip volume.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pt_volume         the volume value of audio clip
 *  @note
 *  @see  x_scc_aud_set_clip_volume
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_clip_volume( 
    HANDLE_T                        h_aud,
    const SCC_AUD_VOLUME_INFO_T*    pt_volume );

/*------------------------------------------------------------------*/
/*! @brief   set audio volume.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pt_volume           the  audio volume info.
 *  @note
 *  @see   x_scc_aud_set_volume
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_volume( 
    HANDLE_T                        h_aud,
    const SCC_AUD_VOLUME_INFO_T*    pt_volume );

/*------------------------------------------------------------------*/
/*! @brief  set audio clip custom setting.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_setting_type         indicate this operation is about power on or power off 
 *  @param [in]  e_setting_mode         indicate audio clip custom setting mode. 
 *  @note
 *  @see  x_scc_aud_set_clip_custom_setting
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_clip_custom_setting(
    HANDLE_T                        h_aud,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T              e_setting_type,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T              e_setting_mode );

/*------------------------------------------------------------------*/
/*! @brief  get audio clip custom setting.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_setting_type         indicate this operation is about power on or power off 
 *  @param [out]  pe_setting_mode         Get the audio clip setting mode.
 *  @note
 *  @see  x_scc_aud_get_clip_custom_setting
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_clip_custom_setting(
    HANDLE_T                        h_aud,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T              e_setting_type,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T*              pe_setting_mode );


/*------------------------------------------------------------------*/
/*! @brief   set volume function include set port.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pt_volume           the  audio volume info.
 *  @note
 *  @see   x_scc_aud_set_volume_ex
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_volume_ex( 
    HANDLE_T                        h_aud,
    const SCC_AUD_VOLUME_INFO_T*    pt_volume );

/*------------------------------------------------------------------*/
/*! @brief   mute/unmute the audio
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  b_on                specify mute audio or not.
 *  @note
 *  @see x_scc_aud_switch
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_switch(
    HANDLE_T                        h_aud,
    BOOL                            b_on);

/*------------------------------------------------------------------*/
/*! @brief   mute the audio
 *  @param [in]  h_aud                Audio component handle.
 *  @note
 *  @see x_scc_aud_mute
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_mute( 
    HANDLE_T                        h_aud );

/*------------------------------------------------------------------*/
/*! @brief    unmute audio.
 *  @param [in]  h_aud                Audio component handle.
 *  @note
 *  @see    x_scc_aud_unmute
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_unmute( 
    HANDLE_T                        h_aud );

/*------------------------------------------------------------------*/
/*! @brief   set spdif output format
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_spdif_fmt        set format information.
 *  @note
 *  @see   x_scc_aud_set_spdif_fmt
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_spdif_fmt( 
    HANDLE_T                        h_aud,
    SCC_AUD_SPDIF_FMT_T             e_spdif_fmt );

/*------------------------------------------------------------------*/
/*! @brief    set hdcd filter
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_hdcd_fltr       set hdcd filter information.
 *  @note
 *  @see   x_scc_aud_set_hdcd_filter
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_hdcd_filter( 
    HANDLE_T                        h_aud,
    SCC_AUD_HDCD_FLTR_T             e_hdcd_fltr );

/*------------------------------------------------------------------*/
/*! @brief    set speaker mode.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pt_ls_type       set speaker information.
 *  @note
 *  @see    x_scc_aud_set_loudspeaker
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_loudspeaker( 
    HANDLE_T                        h_aud,
    const SCC_AUD_LOUD_SPEAKER_T*   pt_ls_type );

/*------------------------------------------------------------------*/
/*! @brief   set sound effect trim information.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pt_se_trim       set trim information.
 *  @note
 *  @see   x_scc_aud_set_se_trim
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_trim( 
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_TRIM_T*        pt_se_trim );

/*------------------------------------------------------------------*/
/*! @brief    set super bass
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  b_is_on              Indicate whether use this setting.
 *  @note
 *  @see    x_scc_aud_set_se_superbase
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_superbase( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect equalizer.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_equalizer         Set equalizer information.
 *  @note
 *  @see  x_scc_aud_set_se_equalizer
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_equalizer( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_EQ_T                 e_equalizer );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect reverb.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_reverb         Set reverb mode.
 *  @note
 *  @see  x_scc_aud_set_se_reverb
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_reverb( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_REVERB_MODE_T        e_reverb );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect base.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  ui1_base         sound effect base value.
 *  @note
 *  @see  x_scc_aud_set_se_base
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_base( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_base );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect treble.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  ui1_treble         sound effect treble value.
 *  @note
 *  @see  x_scc_aud_set_se_treble
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_treble( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_treble );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect balance.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  ui1_balance         sound effect balance value.
 *  @note
 *  @see  x_scc_aud_set_se_balance
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_balance( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_balance );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect postdr.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  b_is_on              Indicate whether use this setting.
 *  @note
 *  @see  x_scc_aud_set_se_postdr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_postdr( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   set sound effect volume
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pt_volume              sound effect balance volume.
 *  @note
 *  @see   x_scc_aud_set_se_volume
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_volume( 
    HANDLE_T                        h_aud,
    const SCC_AUD_VOLUME_INFO_T*    pt_volume );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect surround.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  b_is_on              Indicate whether use this setting.
 *  @note
 *  @see  x_scc_aud_set_se_surround
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_surround( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect superwoofer.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  b_is_on              Indicate whether use this setting.
 *  @note
 *  @see  x_scc_aud_set_se_superwoofer
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_superwoofer( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );

/*------------------------------------------------------------------*/
/*! @brief    set pan/fade.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  b_is_on              Indicate whether use this setting.
 *  @note
 *  @see  x_scc_aud_set_se_superwoofer
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_panfade( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect sound_mode_start.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  b_is_on              Indicate whether use this setting.
 *  @note
 *  @see  x_scc_aud_set_se_sound_mode_start
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_sound_mode_start( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );

/*------------------------------------------------------------------*/
/*! @brief    set aud srs bypass
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  b_is_on              Indicate whether use this setting.
 *  @note
 *  @see  x_scc_aud_set_srs_bypass
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_srs_bypass(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );

#if 0
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_surround_ex( 
    HANDLE_T                 h_aud,
    UINT32                          ui4_surround_mode );
#endif
/*------------------------------------------------------------------*/
/*! @brief    set surround mode
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  pt_surround_ex     sound effect surround info. 
 *  @note
 *  @see    x_scc_aud_set_se_surround_ex
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_surround_ex( 
    HANDLE_T                 h_aud,
    SCC_AUD_SE_SURROUND_INFO_T*                pt_surround_ex );

/*------------------------------------------------------------------*/
/*! @brief   set sound effect sound mode
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_sound_mode     sound effect sound mode 
 *  @note
 *  @see   x_scc_aud_set_se_sound_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_sound_mode( 
    HANDLE_T                h_aud,
    SCC_AUD_SE_SOUND_MODE_T  e_sound_mode );

/*------------------------------------------------------------------*/
/*! @brief   set audio downmix mode
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_dm_mode        downmix mode 
 *  @note
 *  @see    x_scc_aud_set_downmix_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_downmix_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_DOWNMIX_MODE_T          e_dm_mode );

/*------------------------------------------------------------------*/
/*! @brief     set audio dolby drc mode
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in]  e_drc             dolby drc mode
 *  @note
 *  @see    x_scc_aud_set_dolby_drc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_dolby_drc( 
    HANDLE_T                        h_aud,
    SCC_AUD_DOLBY_DRC_T             e_drc );
 
 /*------------------------------------------------------------------*/
 /*! @brief     set audio compression drc mode
  *  @param [in]  h_aud            Audio component handle.
  *  @param [in]  e_cmpss_drc      Compression drc mode
  *  @note
  *  @see    x_scc_aud_set_cmpss_drc
  *  @return
  *  @retval      -SMR_OK              Success.
  *  @retval      -SMR_INTERNAL_ERROR  Fail.
  */
 /*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_cmpss_drc( 
    HANDLE_T                            h_aud,
    SCC_AUD_CMPSS_DRC_MODE_T            e_cmpss_drc );

/*------------------------------------------------------------------*/
/*! @brief     set audio mpeg level adjustment
 *  @param [in]  h_aud            Audio component handle.
 *  @param [in]  i1_level         adjustment level
 *  @note
 *  @see    x_scc_aud_set_mpeg_level_adj
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_mpeg_level_adj(
    HANDLE_T                        h_aud,
    INT8                            i1_level );

/*------------------------------------------------------------------*/
/*! @brief   set output port whether is adjust or not
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   e_port               specify set output  
 *  @param [in]   b_is_fixed           specify is fixed or not for the port
 *  @note
 *  @see    x_scc_aud_set_output_port_adj
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_output_port_adj( 
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL                            b_is_fixed );

/*------------------------------------------------------------------*/
/*! @brief   get output port whether is adjust or not
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   e_port               specify set output 
 *  @param [out]  pb_is_fixed          retrieve is fixed or not for the port
 *  @note
 *  @see   x_scc_aud_get_output_port_adj
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_output_port_adj( 
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL*                           pb_is_fixed );

/*------------------------------------------------------------------*/
/*! @brief   get audio capability
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   pt_capability        retrieve the audio capability
 *  @note
 *  @see  x_scc_aud_get_capability
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_capability( 
    HANDLE_T                        h_aud,
    SCC_AUD_CAPABILITY_T*           pt_capability );
    
    /*------------------------------------------------------------------*/
    /*! @brief   set output port 
     *  @param [in]   h_aud                Audio component handle.
     *  @param [in]   e_port               specify set output  
     *  @param [in]   b_is_fixed           specify is active or not for the port
     *  @note
     *  @see    x_scc_aud_set_output_port
     *  @return
     *  @retval      -SMR_OK              Success.
     *  @retval      -SMR_INTERNAL_ERROR  Fail.
     */
    /*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_output_port( 
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL                            b_is_active );

/*------------------------------------------------------------------*/
/*! @brief   set output port 
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   ui4_enable_mask    specify set output mask 
 *  @note
 *  @see    x_scc_aud_set_output_port_ex
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_output_port_ex( 
    HANDLE_T                        h_aud,
    UINT32                          ui4_enable_mask );

/*------------------------------------------------------------------*/
/*! @brief   set audio compression mode
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   e_cmpss_mode    audio compression mode
 *  @note
 *  @see    x_scc_aud_set_cmpss_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_cmpss_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_CMPSS_MDOE_T            e_cmpss_mode );

/*------------------------------------------------------------------*/
/*! @brief    set audio agc
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   b_agc                specify is set or not for the port
 *  @note
 *  @see     x_scc_aud_set_agc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_agc( 
    HANDLE_T                        h_aud,
    BOOL                            b_agc );

/*------------------------------------------------------------------*/
/*! @brief    set vdo picture off
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   b_is_on              specify is set or not for the picture off
 *  @note
 *  @see     x_scc_aud_set_vdo_picture_off
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_vdo_picture_off(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );

/*------------------------------------------------------------------*/
/*! @brief   set audio mts mode
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   e_mts                specify mts mode
 *  @note
 *  @see   x_scc_aud_set_mts
 *  @return 
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_mts( 
    HANDLE_T                        h_aud,
    SCC_AUD_MTS_T                   e_mts );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect equation user level
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   pt_eq_user_lvl                specify user level information
 *  @note
 *  @see    x_scc_aud_set_se_eq_user_lvl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_eq_user_lvl( 
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_QA_USER_LVL_T* pt_eq_user_lvl );

/*------------------------------------------------------------------*/
/*! @brief    set sound effect equation user level
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   pt_eq_user_lvl                specify user level information
 *  @note
 *  @see   x_scc_aud_set_se_eq_user_lvl_ex
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_eq_user_lvl_ex(
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_QA_USER_LVL_T* pt_eq_user_lvl );

/*------------------------------------------------------------------*/
/*! @brief   set audio channel delay
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   pt_chnl_delay                specify delay information
 *  @note
 *  @see   x_scc_aud_set_chnl_delay
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_chnl_delay( 
    HANDLE_T                        h_aud,
    SCC_AUD_CHNL_DELAY_T*           pt_chnl_delay );

/*------------------------------------------------------------------*/
/*! @brief   set audio headphone detect notify
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   pt_hdph_nfy_info                specify notify information
 *  @note
 *  @see   x_scc_aud_set_hdphone_detect_nfy
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_hdphone_detect_nfy( 
    HANDLE_T                        h_aud,
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T* pt_hdph_nfy_info );

/*------------------------------------------------------------------*/
/*! @brief   set audio mute
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   ui4_muted_out_port_msk                specify mute port mask
 *  @note
 *  @see   x_scc_aud_set_mute_ex
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_mute_ex( 
    HANDLE_T                        h_aud,
    UINT32                          ui4_muted_out_port_msk );

/*------------------------------------------------------------------*/
/*! @brief   set audio mute ,this function is use for svctx.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   ui4_muted_out_port_msk                specify mute port mask
 *  @note
 *  @see   x_scc_aud_switch_ex
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_switch_ex(
    HANDLE_T                        h_aud,
    UINT32                          ui4_muted_out_port_msk );

/*------------------------------------------------------------------*/
/*! @brief   set mute or unmute port 
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   e_port               specify set output  
 *  @param [in]   b_is_fixed           specify is active or not for the port
 *  @note
 *  @see    x_scc_aud_set_mute_single_port
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_mute_single_port( 
    HANDLE_T                 h_aud,
    SCC_AUD_OUT_PORT_T       e_port,
    BOOL                     b_is_active );

/*------------------------------------------------------------------*/
/*! @brief   get audio volume 
 *  @param [in]    h_aud                Audio component handle.
 *  @param [out]  pt_volume          specify get volume information
 *  @note
 *  @see   x_scc_aud_get_volume
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_volume( 
    HANDLE_T                        h_aud,
    SCC_AUD_VOLUME_INFO_T*          pt_volume );

/*------------------------------------------------------------------*/
/*! @brief    specify whether audio is mute or not.
 *  @param [in]    h_aud                Audio component handle.
 *  @param [out]   pb_is_muted       specify whether audio is mute or not.
 *  @note
 *  @see    x_scc_aud_is_muted
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_is_muted( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_muted );

/*------------------------------------------------------------------*/
/*! @brief   get audio spdif format
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pe_spdif_fmt      specify spdif format
 *  @note
 *  @see   x_scc_aud_get_spdif_fmt
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_spdif_fmt( 
    HANDLE_T                        h_aud,
    SCC_AUD_SPDIF_FMT_T*            pe_spdif_fmt );

/*------------------------------------------------------------------*/
/*! @brief   get audio hdcd filter 
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pe_hdcd_fltr      specify hdcd filter
 *  @note
 *  @see   x_scc_aud_get_hdcd_filter
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_hdcd_filter( 
    HANDLE_T                        h_aud,
    SCC_AUD_HDCD_FLTR_T*            pe_hdcd_fltr );

/*------------------------------------------------------------------*/
/*! @brief   get speaker information
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pt_ls_type          specify speaker information.
 *  @note
 *  @see    x_scc_aud_get_loudspeaker
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_loudspeaker( 
    HANDLE_T                        h_aud,
    SCC_AUD_LOUD_SPEAKER_T*         pt_ls_type );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect trim.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pt_se_trim          specify trim info.
 *  @note
 *  @see  x_scc_aud_get_se_trim
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_trim( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_TRIM_T*              pt_se_trim );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect trim.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pb_is_on            whether audio is setting super bass or not
 *  @note
 *  @see  x_scc_aud_get_se_trim
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_superbase( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect equalizer.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pe_equalizer          specify equalizer info.
 *  @note
 *  @see  x_scc_aud_get_se_equalizer
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_equalizer( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_EQ_T*                pe_equalizer );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect reverb.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pe_reverb          specify reverb info.
 *  @note
 *  @see  x_scc_aud_get_se_reverb
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_reverb( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_REVERB_MODE_T*       pe_reverb );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect base.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pui1_base          specify base info.
 *  @note
 *  @see  x_scc_aud_get_se_base
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_base( 
    HANDLE_T                        h_aud,
    UINT8*                          pui1_base );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect treble.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pui1_treble          specify treble info.
 *  @note
 *  @see  x_scc_aud_get_se_treble
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_treble( 
    HANDLE_T                        h_aud,
    UINT8*                          pui1_treble );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect balance.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pui1_balance         specify balance info.
 *  @note
 *  @see  x_scc_aud_get_se_balance
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_balance( 
    HANDLE_T                        h_aud,
    UINT8*                          pui1_balance );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect postdr.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pb_is_on        specify whether have setting postdr.
 *  @note
 *  @see  x_scc_aud_get_se_postdr
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_postdr( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect volume.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pt_volume        specify volume info.
 *  @note
 *  @see  x_scc_aud_get_se_volume
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_volume( 
    HANDLE_T                        h_aud,
    SCC_AUD_VOLUME_INFO_T*          pt_volume );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect surround.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pb_is_on        specify whether have setting surround.
 *  @note
 *  @see  x_scc_aud_get_se_surround
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_surround( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief   get sound effect superwoofer.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pb_is_on        specify whether have setting superwoofer.
 *  @note
 *  @see  x_scc_aud_get_se_superwoofer
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_superwoofer( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on );

/*------------------------------------------------------------------*/
/*! @brief   get audio downmix mode
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out] pe_dm_mode     specify downmix mode
 *  @note
 *  @see   x_scc_aud_get_downmix_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_downmix_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_DOWNMIX_MODE_T*         pe_dm_mode );

/*------------------------------------------------------------------*/
/*! @brief   get audio dolby drc
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pe_drc              specify dolby drc mode.
 *  @note
 *  @see  x_scc_aud_get_dolby_drc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_dolby_drc( 
    HANDLE_T                        h_aud,
    SCC_AUD_DOLBY_DRC_T*            pe_drc );

/*------------------------------------------------------------------*/
/*! @brief   get audio compression drc
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pe_cmpss_drc    specify compression drc mode.
 *  @note
 *  @see  x_scc_aud_get_cmpss_drc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_cmpss_drc( 
    HANDLE_T                   h_aud,
    SCC_AUD_CMPSS_DRC_MODE_T*  pe_cmpss_drc );

/*------------------------------------------------------------------*/
/*! @brief   get audio mpeg level adjustment
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out] pi1_level             specify adjustment level.
 *  @note
 *  @see  x_scc_aud_get_mpeg_level_adj
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_mpeg_level_adj( 
    HANDLE_T                        h_aud,
    INT8*                           pi1_level );

/*------------------------------------------------------------------*/
/*! @brief    get whether output is active or not
 *  @param [in]   h_aud                Audio component handle.
 *  @param [in]   e_port              specify audio output
 *  @param [out]  pb_is_active     specify whether output is active or not
 *  @note
 *  @see   x_scc_aud_get_output_port
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_output_port( 
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL*                           pb_is_active );

/*------------------------------------------------------------------*/
/*! @brief   get audio compression mode
 *  @param [in]    h_aud                Audio component handle.
 *  @param [out]  pe_cmpss_mode   specify audio compression mode.
 *  @note
 *  @see   x_scc_aud_get_cmpss_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_cmpss_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_CMPSS_MDOE_T*           pe_cmpss_mode );

/*------------------------------------------------------------------*/
/*! @brief  get whether audio setting agc or not.
 *  @param [in] h_aud                Audio component handle.
 *  @param [out]   pb_agc          specify  whether audio setting agc or not.
 *  @note
 *  @see   c_scc_aud_get_agc
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_agc( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_agc );

/*------------------------------------------------------------------*/
/*! @brief  get audio channel delay information.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out] pt_chnl_delay    specify delay information.
 *  @note
 *  @see  x_scc_aud_get_chnl_delay
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_chnl_delay( 
    HANDLE_T                        h_aud,
    SCC_AUD_CHNL_DELAY_T*           pt_chnl_delay );

/*------------------------------------------------------------------*/
/*! @brief   get max channel delay value
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pui2_max_delay   specify max delay value.
 *  @note
 *  @see   x_scc_aud_get_max_chnl_delay
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_max_chnl_delay( 
    HANDLE_T                        h_aud,
    UINT16*                         pui2_max_delay );

/*------------------------------------------------------------------*/
/*! @brief   get audio mts.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out] pe_mts             specify audio mts mode.
 *  @note
 *  @see  x_scc_aud_get_mts
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_mts( 
    HANDLE_T                        h_aud,
    SCC_AUD_MTS_T*                  pe_mts );

/*------------------------------------------------------------------*/
/*! @brief   get audio sound effect equalizer user level.
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pt_eq_user_lvl   specify level information.
 *  @note
 *  @see  x_scc_aud_get_se_eq_user_lvl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_eq_user_lvl( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_QA_USER_LVL_T*       pt_eq_user_lvl );

/*------------------------------------------------------------------*/
/*! @brief   get audio sound effect equalizer user band numberl.
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pui1_num_band   specify band number.
 *  @note
 *  @see  x_scc_aud_get_se_eq_user_num_band
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_eq_user_num_band( 
    HANDLE_T                        h_aud,
    UINT8*                          pui1_num_band );

/*------------------------------------------------------------------*/
/*! @brief    get audio sound effect equalizer user information.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [out]  pt_eq_user_info  specify user information.
 *  @note
 *  @see  x_scc_aud_get_se_eq_user_info
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_se_eq_user_info( 
    HANDLE_T                        h_aud,
    SCC_AU_SE_QA_USER_BAND_INFO_T*  pt_eq_user_info );
    
    /*------------------------------------------------------------------*/
    /*! @brief  get whether audio set dolby certification mode.
     *  @param [in]  h_aud                Audio component handle.
     *  @param [out] pb_mode         specify dolby certification mode.        
     *  @note
     *  @see  x_scc_aud_get_dolby_certi_mode
     *  @return
     *  @retval      -SMR_OK              Success.
     *  @retval      -SMR_INTERNAL_ERROR  Fail.
     */
    /*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_dolby_certi_mode( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_mode );

/*------------------------------------------------------------------*/
/*! @brief  get whether audio set dolby certification mode.
 *  @param [in]  h_aud                Audio component handle.
 *  @param [in] b_mode         specify dolby certification mode.  
 *  @note
 *  @see  x_scc_aud_set_dolby_certi_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_dolby_certi_mode( 
    HANDLE_T                        h_aud,
    BOOL                            b_mode );

/*------------------------------------------------------------------*/
/*! @brief  set audio post-processing mode
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] e_mode               specify audio post-processing mode.
 *  @note
 *  @see  x_scc_aud_set_bbe_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_bbe_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_BBE_MODE_T              e_mode );

/*------------------------------------------------------------------*/
/*! @brief  get audio post-processing mode
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] e_mode               specify audio post-processing mode.
 *  @note
 *  @see  x_scc_aud_get_bbe_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_bbe_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_BBE_MODE_T*             pe_mode );

/*------------------------------------------------------------------*/
/*! @brief  get audio headphone detect notify.
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pt_hdph_nfy_info  notify information.
 *  @note
 *  @see  x_scc_aud_get_hdphone_detect_nfy
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_hdphone_detect_nfy( 
    HANDLE_T                           h_aud,
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T* pt_hdph_nfy_info );
    
    /*------------------------------------------------------------------*/
    /*! @brief    get audio headphone condition.
     *  @param [in] h_aud                Audio component handle.
     *  @param [out] pe_hdphone_cond  headphone condition.
     *  @note
     *  @see x_scc_aud_get_hdphone_cond
     *  @return
     *  @retval      -SMR_OK              Success.
     *  @retval      -SMR_INTERNAL_ERROR  Fail.
     */
    /*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_hdphone_cond( 
    HANDLE_T                        h_aud,
    SCC_AUD_HDPHONE_COND_T*         pe_hdphone_cond );

/*------------------------------------------------------------------*/
/*! @brief   get mute output mask information.
 *  @param [in]   h_aud                Audio component handle.
 *  @param [out]  pui4_muted_out_port_msk  specify the mask information.
 *  @note
 *  @see  x_scc_aud_get_mute_ex
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_mute_ex( 
    HANDLE_T                        h_aud,
    UINT32*                         pui4_muted_out_port_msk );

/*------------------------------------------------------------------*/
/*! @brief   set audio lineout volume mode.
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] pt_lineout_vol_mode   specify the lineout mode.
 *  @note
 *  @see x_scc_aud_set_lineout_vol_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_lineout_vol_mode( 
    HANDLE_T                 h_aud,
   SCC_AUD_DEC_LINEOUT_VOL_T*  pt_lineout_vol_mode);

/*------------------------------------------------------------------*/
/*! @brief  set sound effect speaker height mode.
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] e_mode              specify speaker height mode.
 *  @note
 *  @see  x_scc_aud_set_se_sph_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_sph_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_DEC_SE_SPH_MODE_T        e_mode );

/*------------------------------------------------------------------*/
/*! @brief  set sound effect volume offset
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] ui1_offset              specify offset
 *  @note
 *  @see x_scc_aud_set_se_volume_offset
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_volume_offset(
    HANDLE_T                        h_aud,
    UINT8                           ui1_offset );


/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_hp_volume_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_balance         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_se_hp_volume_offset(
    HANDLE_T                        h_aud,
    UINT8                           ui1_offset );


/*------------------------------------------------------------------*/
/*! @brief  set audio parametric eq information.
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] pt_peq_info              specify parametric eq information.
 *  @note
 *  @see  x_scc_aud_set_peq_info
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_peq_info( 
    HANDLE_T                 h_aud,
   SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T*  pt_peq_info);

/*------------------------------------------------------------------*/
/*! @brief  set audio over-modulation
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] ui4_value              specify over-modulation value.
 *  @note
 *  @see   x_scc_aud_set_ovm
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_ovm(
    HANDLE_T                        h_aud,
    UINT32                            ui4_value );

/*------------------------------------------------------------------*/
/*! @brief  set audio button sound mode
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] e_btnsnd_mode              specify button sound mode.
 *  @note
 *  @see   x_scc_aud_set_btnsnd_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_btnsnd_mode( 
    HANDLE_T                  h_aud,
    SCC_AUD_BTNSND_MODE_T             e_btnsnd_mode );

/*------------------------------------------------------------------*/
/*! @brief  get audio button sound mode
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pe_btnsnd_mode              specify button sound mode.
 *  @note
 *  @see   x_scc_aud_get_btnsnd_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_btnsnd_mode( 
    HANDLE_T                  h_aud,
    SCC_AUD_BTNSND_MODE_T*             pe_btnsnd_mode );

/*------------------------------------------------------------------*/
/*! @brief  set audio button sound enable or disable
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] e_btnsnd_enable              specify button sound enable or disable.
 *  @note
 *  @see   x_scc_aud_set_btnsnd_enable
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_btnsnd_enable( 
    HANDLE_T                  h_aud,
    SCC_AUD_BTNSND_ENABLE_T             e_btnsnd_enable );

/*------------------------------------------------------------------*/
/*! @brief  get audio button sound enable or disable
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pe_btnsnd_enable              specify button sound enable or disable.
 *  @note
 *  @see   x_scc_aud_get_btnsnd_enable
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_btnsnd_enable( 
    HANDLE_T                  h_aud,
    SCC_AUD_BTNSND_ENABLE_T*             pe_btnsnd_enable );

/*------------------------------------------------------------------*/
/*! @brief  set audio button sound play
 *  @param [in] h_aud                Audio component handle.
 *  @note
 *  @see   x_scc_aud_set_btnsnd_play
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_btnsnd_play( HANDLE_T                  h_aud );

/*------------------------------------------------------------------*/
/*! @brief  set audio av sync mode
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] e_av_sync_mode              specify av sync mode.
 *  @note
 *  @see   x_scc_aud_set_av_sync_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_av_sync_mode( 
    HANDLE_T                  h_aud,
    SCC_AUD_DEC_AV_SYNC_INFO_T             e_av_sync_mode );

/*------------------------------------------------------------------*/
/*! @brief  get audio av sync mode
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pe_av_sync_mode             specify av sync mode.
 *  @note
 *  @see   x_scc_aud_get_av_sync_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_av_sync_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_DEC_AV_SYNC_INFO_T*             pe_av_sync_mode );

/*------------------------------------------------------------------*/
/*! @brief  get power on/off music volume
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pt_volume             contain volume info.
 *  @note
 *  @see   x_scc_aud_get_power_on_off_music_volume
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_power_on_off_music_volume( 
    HANDLE_T                      h_aud,
    SCC_AUD_VOLUME_INFO_T*        pt_volume );

/*------------------------------------------------------------------*/
/*! @brief  set upload data enable or not.
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] b_is_on             enable/disable.
 *  @note
 *  @see   x_scc_aud_set_upload_ctrl
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_upload_ctrl(
    HANDLE_T                        h_aud,
    BOOL                            b_is_on );
/*------------------------------------------------------------------*/
/*! @brief  get upload data buffer info.
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pt_buffer_info             buffer info.
 *  @note
 *  @see   x_scc_aud_get_upload_data_buffer_info
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_upload_data_buffer_info(
    HANDLE_T                        h_aud,
    SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T*           pt_buffer_info );

/*------------------------------------------------------------------*/
/*! @brief  set upload data callback function.
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] pt_upload_data_nfy_info             specify callback function information.
 *  @note
 *  @see   x_scc_aud_set_upload_data_nfy
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_upload_data_nfy(
    HANDLE_T                        h_aud,
    SCC_AUD_UPLOAD_DATA_NFT_INFO_T* pt_upload_data_nfy_info );

/*------------------------------------------------------------------*/
/*! @brief   get upload pcm information.
 *  @param [in] h_aud          Audio component handle.
 *  @param [out] pt_pcm_info   specify upload pcm information.
 *  @note
 *  @see  x_scc_aud_get_upload_pcm_info
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_get_upload_pcm_info( 
    HANDLE_T                        h_aud,
    SCC_AUD_UPLOAD_PCM_INFO_T*      pt_pcm_info );

/*------------------------------------------------------------------*/
/*! @brief  set audio protect information
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] e_protect_info       specify audio protect information.
 *  @note
 *  @see   x_scc_aud_set_protect_info
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_protect_info(
    HANDLE_T                        h_aud,
    SCC_PROTECT_INFO_T              e_protect_info );
/*------------------------------------------------------------------*/
/*! @brief  Inform audio decoder video is scrambled or not
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] b_scrambled            The video is scrambled or not.
 *  @note
 *  @see   x_scc_aud_set_video_scramble_status
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_video_scramble_status(
    HANDLE_T                        h_aud,
    BOOL                            b_scrambled );

/*------------------------------------------------------------------*/
/*! @brief  set audio karaoke info
 *  @param [in] h_aud                Audio component handle.
 *  @param [out] pt_karaoke_info     contain karaoke info.
 *  @note
 *  @see   x_scc_aud_set_karaoke
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_karaoke(
    HANDLE_T                        h_aud,
    SCC_AUD_KARAOKE_INFO_T*         pt_karaoke_info );

/*------------------------------------------------------------------*/
/*! @brief  set out port link enable or disable
 *  @param [in] h_aud                Audio component handle.
 *  @param [in] b_enable             link enable or not.
 *  @note
 *  @see   x_scc_aud_set_out_port_link
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
INT32 x_scc_aud_set_out_port_link(
    HANDLE_T                        h_aud,
    BOOL                            b_enable );

/*------------------------------------------------------------------*/
/*! @brief  set dc power off enable or disable
 *  @param [in] h_aud               Audio component handle.
 *  @param [in] b_enable            link enable or not.
 *  @note
 *  @see   x_scc_aud_set_dc_power_off
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_dc_power_off(
    HANDLE_T                        h_aud,
    BOOL                            b_enable );

/*------------------------------------------------------------------*/
/*! @brief  set little endian or big endian
 *  @param [in] h_aud                                        Audio component handle.
 *  @param [in] SCC_AUD_ENDIAN_MODE_T            little endian or big endian.
 *  @note
 *  @see   x_scc_aud_set_endian_mode
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_endian_mode(
    HANDLE_T                           h_aud,
    SCC_AUD_ENDIAN_MODE_T              e_mode );

/*------------------------------------------------------------------*/
/*! @brief  set audio out device
 *  @param [in] h_aud                                        Audio component handle.
 *  @param [in] SCC_AUD_OUT_DEVICE_T              Out device.
 *  @note
 *  @see   x_scc_aud_set_out_device
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_aud_set_out_device(
    HANDLE_T                           h_aud,
    SCC_AUD_OUT_DEVICE_T              e_out_device );

/*------------------------------------------------------------------*/
/*! @brief   display pmx sephore lock
 *  @note
 *  @see   x_scc_disp_pmx_lock
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern VOID x_scc_disp_pmx_lock( VOID );

/*------------------------------------------------------------------*/
/*! @brief   display pmx sephore unlock
 *  @note
 *  @see
 *  @return    x_scc_disp_pmx_unlock
 *  @retval      -
 */
/*------------------------------------------------------------------*/
extern VOID x_scc_disp_pmx_unlock( VOID );

/*------------------------------------------------------------------*/
/*! @brief  query bootup status
 *  @param [in] ui1_mode                query mode, sound, logo etc
 *  @param [out] pui1_status            query status
 *  @note
 *  @see   x_scc_query_bootup_status
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_query_bootup_status(UINT8  ui1_mode, UINT8* pui1_status);
/*------------------------------------------------------------------*/
/*! @brief  set current input source info to driver
 *  @param [in] e_inp_src_type                current input source type
 *  @param [in] ui1_port_num                  current input source port num
 *  @note
 *  @see   x_scc_set_crnt_inp_src_info
 *  @return
 *  @retval      -SMR_OK              Success.
 *  @retval      -SMR_INTERNAL_ERROR  Fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_scc_set_crnt_inp_src_info(    
                     SCC_CRNT_INP_SRC_TYPE_T                e_inp_src_type,     
                     UINT8                                  ui1_port_num);


#endif /* _X_SCC_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                              /* end of groupMW_STRM_MNGR_SCC*/
/*----------------------------------------------------------------------------*/


