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
 * $RCSfile: u_dmx_util.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/

#ifndef _U_DMX_UTIL_H_
#define _U_DMX_UTIL_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_DMX_UT DMX Utility
 *
 *  @ingroup groupMW
 *  @brief For load section
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could change input source, 
 *   change channel, select multi media and so on.
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
 
/*------------------------------------------------------------------*/
/*! @struct DMX_UT_SF_INFO_T
 *  @brief  Setion Filter Information, Demux is according to this info to filter some sections
 *  @code
 *  typedef struct _DMX_UT_SF_INFO_T
 *  {
 *      UINT8  ui1_tbl_id_pattern;
 *      UINT8  ui1_tbl_id_mask;

 *      UINT8  ui1_pattern_offset;
 *      UINT8  ui1_pattern_len;

 *      UINT8*  pui1_pos_pattern;
 *      UINT8*  pui1_pos_mask;

 *      UINT8*  pui1_neg_pattern;
 *      UINT8*  pui1_neg_mask;

 *      BOOL  b_check_crc;
 *  }   DMX_UT_SF_INFO_T; 
 *  @endcode
 *  @li@c  ui1_tbl_id_pattern                                  ui1_tbl_id_pattern - table id pattern
 *  @li@c  ui1_tbl_id_mask                                     ui1_tbl_id_mask -table id mask
 *  @li@c  ui1_pattern_offset                                  ui1_pattern_offset - offset where start filter pattern 
 *  @li@c  ui1_pattern_len                                      ui1_pattern_len - len which filter pattern length
 *  @li@c  pui1_pos_pattern                                     pui1_pos_pattern - local array for filter pattern
 *  @li@c  pui1_pos_mask                                        pui1_pos_mask - local mask for filter pattern
 *  @li@c  pui1_neg_pattern                                     pui1_neg_pattern - against pattern for above
 *  @li@c  pui1_neg_mask                                        pui1_neg_mask - against pattern mask
 */
/*------------------------------------------------------------------*/
typedef struct _DMX_UT_SF_INFO_T
{
    UINT8  ui1_tbl_id_pattern;
    UINT8  ui1_tbl_id_mask;

    UINT8  ui1_pattern_offset;
    UINT8  ui1_pattern_len;

    UINT8*  pui1_pos_pattern;
    UINT8*  pui1_pos_mask;

    UINT8*  pui1_neg_pattern;
    UINT8*  pui1_neg_mask;

    BOOL  b_check_crc;
}   DMX_UT_SF_INFO_T;

/* service context states */
/*------------------------------------------------------------------*/
/*! @enum   DMX_UT_COND_T
 *  @brief  section notify condition
 *  @code
 *  typedef enum
 *  {
 *      DMX_UT_COND_DATA_REC = 0,
 *      DMX_UT_COND_DATA_OVERFLOW
 *  } DMX_UT_COND_T;
 *  @endcode
 *  @li@c  DMX_UT_COND_DATA_REC                         DMX_UT_COND_DATA_REC - section data is arrived
 *  @li@c  DMX_UT_COND_DATA_OVERFLOW              DMX_UT_COND_DATA_OVERFLOW - ready use, only init field.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DMX_UT_COND_DATA_REC = 0,
    DMX_UT_COND_DATA_OVERFLOW
} DMX_UT_COND_T;

/*------------------------------------------------------------------*/
/*! @brief  prototype for section notify
 *  @param  [in]  e_nfy_cond - notify current section arrived condition.
 *  @param  [in]  pv_nfy_tag - tag for notify.
 *  @param  [in]  z_sec_len - section length.
 *  @param  [in]  ui4_nfy_data - section data.
 *  @return TRUE or FALSE
 */
/*------------------------------------------------------------------*/
typedef BOOL (*x_dmx_ut_nfy_fct) (DMX_UT_COND_T e_nfy_cond,
                                  VOID*         pv_src_tag,
                                  SIZE_T        z_sec_len,
                                  UINT32        ui4_nfy_data);


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#define DMXR_UT_OK                               ((INT32)    0) /**< operate OK */
#define DMXR_UT_FAIL                             ((INT32)   -1) /**< operate Fail */
#define DMXR_UT_NOT_INIT                         ((INT32)   -2) /**< demux utility moduel dont' initialize */
#define DMXR_UT_INV_ARGS                         ((INT32)   -3) /**< input invalid arguments */
#define DMXR_UT_INV_OP                           ((INT32)   -4) /**< current call is invalid */
#define DMXR_UT_INV_HANDLE                       ((INT32)   -5) /**< input handle is invalid */
#define DMXR_UT_NO_RESOURCE                      ((INT32)   -6) /**< demux utility module resource is not enough */
#define DMXR_UT_INTERNAL_ERR                     ((INT32)   -7) /**< demux utility module has a internal error */
#define DMXR_UT_RM_ERR                           ((INT32)   -8) /**< resource manager return a error */

#define DMXR_UT_CLI_ERR                        ((INT32)   -11)  /**< cli command has a error */
#endif /* _U_DMX_UTIL_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_DMX_UT */
/*----------------------------------------------------------------------------*/


