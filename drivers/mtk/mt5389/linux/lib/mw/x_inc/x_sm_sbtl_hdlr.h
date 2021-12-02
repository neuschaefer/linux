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
/*! @file x_sm_sbtl_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Subtitle Stream handler specific 
 *         definitions, which are exported to applications.
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_SM_SBTL_HDLR_H_
#define _X_SM_SBTL_HDLR_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_sm_sbtl_hdlr.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_SBTL_HDLR
*  @{  
*/
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief Function prototype of subtitle ROM object render function
 *  @param [in]     ui2_obj_id                  - Object ID
 *  @param [in]     ui1_obj_type                - Object type
 *  @param [in]     pt_sbtl_render_info         - Object position and color
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*sm_sbtl_rom_render_fct)(
    UINT16                      ui2_obj_id, 
    UINT8                       ui1_obj_type,
    SM_SBTLH_RENDER_INFO_T*     pt_sbtl_render_info);


/*------------------------------------------------------------------*/
/*! @brief Function prototype of subtitle text object
 *  @param [in]     pui2_text                   - Text array
 *  @param [in]     ui2_text_len                - Text array length
 *  @param [in]     pt_sbtl_render_info         - Object position and color
 *  @param [out]    
 *  @note
 *  @see
 *  @return     Return the error code.
 *  @retval      SMR_OK                 - The routine was successful.
 *  @retval      SMR_INTERNAL_ERROR     - The routine was fail.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*sm_sbtl_text_render_fct)(
    UINT16*                     pui2_text,
    UINT16                      ui2_text_len,
    SM_SBTLH_RENDER_INFO_T*     pt_sbtl_render_info);

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief Initialize the DVB subtitle configuration
 *  @param [in]     pt_config                   - Configuration context
 *  @param [out]
 *  @note
 *  @see
 *  @return     Return the error code.
 *  @retval      SMR_OK                 - The routine was successful.
 *  @retval      SMR_INTERNAL_ERROR     - The routine was fail.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_sbtl_hdlr_init(const SM_SBTLH_CONFIG_T* pt_config);

/*------------------------------------------------------------------*/
/*! @brief Register rendering function for object provided by a ROM in the IRD
 *  @param [in]     pf_sbtl_rom_render_fct      - Function pointer
 *  @param [out]
 *  @note
 *  @see
 *  @return     Return the error code.
 *  @retval      SBTLR_OK               - The routine was successful.
 *  @retval      SBTLR_INV_ARG          - pf_sbtl_rom_render_fct is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_sbtl_hdlr_reg_rom_render(
    sm_sbtl_rom_render_fct pf_sbtl_rom_render_fct);

/*------------------------------------------------------------------*/
/*! @brief Register rendering function for text type object
 *  @param [in]     pf_sbtl_text_render_fct     - Function pointer
 *  @param [out]    
 *  @note
 *  @see
 *  @return     Return the error code.
 *  @retval      SBTLR_OK               - The routine was successful.
 *  @retval      SBTLR_INV_ARG          - pf_sbtl_text_render_fct is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_sm_sbtl_hdlr_reg_text_render(
    sm_sbtl_text_render_fct pf_sbtl_text_render_fct);

#endif /* _X_SM_SBTL_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                            /* end of groupMW_STRM_MNGR_SBTL_HDLR */
/*----------------------------------------------------------------------------*/


