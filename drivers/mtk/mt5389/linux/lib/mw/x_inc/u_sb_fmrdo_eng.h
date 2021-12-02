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
/*! @file u_sb_fmrdo_eng.h
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *       This header file contains exported definitions and delcarations of
 *       SVL Builder FM radio engine.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVL_BLDR_SB_FMRDO_ENG FM Radio Engine
 *  @ingroup  groupMW_SVL_BLDR
 *  @brief    Provides a unified interface for service list building of FM radio
 *  @see      groupMW_SVL_BLDR
 *
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SB_FMRDO_ENG_H_
#define _U_SB_FMRDO_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_common.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define SB_FMRDO_ENG_NAME    "sb_fmrdo"    /**< Default name of SVL builder FM radio engine */

/*---------------------------------------------------------------------------*/
/*! @struct SB_FMRDO_OPEN_DATA_T
 *  @brief  This structure is used as the argument pv_open_data of the API
 *          c_sb_open_builder() / x_sb_open_builder() when opening an SVL
 *          Builder with FM radio engine.
 *
 *  @code
 *  typedef struct _SB_FMRDO_OPEN_DATA_T
 *  {
 *      UINT16    ui2_svl_id;
 *      CHAR*     ps_svl_name;
 *      CHAR*     ps_file_name;
 *      BOOL      b_use_orig;
 *      UINT32    ui4_first_freq;
 *      UINT32    ui4_last_freq;
 *  } SB_FMRDO_OPEN_DATA_T;
 *  @endcode
 *
 *  @li@c   ui2_svl_id   - The ID of SVL to be loaded
 *  @li@c   ps_svl_name  - The name of SVL to be loaded
 *  @li@c   ps_file_name - The file name from which the SVL will be loaded
 *  @li@c   b_use_orig   - A flag, When set to true, use the existed SVL instead
 *                         of reloading it. When set to false, overwrite the
 *                         SVL (if exists) by reloading.
 *  @li@c   ui4_first_freq - The variable to specifiy range of FM Radio frequency
 *                           (lower bound), ex.  87500000 Hz
 *  @li@c   ui4_last_freq  - The variable to specifiy range of FM Radio frequency
 *                           (upper bound), ex. 108000000 Hz
 */
/*---------------------------------------------------------------------------*/

typedef struct _SB_FMRDO_OPEN_DATA_T
{
    UINT16      ui2_svl_id;
    CHAR*       ps_svl_name;
    CHAR*       ps_file_name;
    BOOL        b_use_orig;
    UINT32      ui4_first_freq;
    UINT32      ui4_last_freq;
} SB_FMRDO_OPEN_DATA_T;

/*---------------------------------------------------------------------------*/
/*! @enum  SB_FMRDO_SCAN_TYPE_T
 *  @brief This structure enumerates all scan types of SVL builder FM radio
 *         engine
 *
 *  @code
 *  typedef enum
 *  {
 *      SB_FMRDO_SCAN_TYPE_UNKNOWN = 0,
 *      SB_FMRDO_SCAN_TYPE_FULL_MODE,
 *      SB_FMRDO_SCAN_TYPE_ONCE_UP,
 *      SB_FMRDO_SCAN_TYPE_ONCE_DOWN,
 *      SB_FMRDO_SCAN_TYPE_NUM,
 *  }   SB_FMRDO_SCAN_TYPE_T;
 *  @endcode
 *
 *  @li@c  SB_FMRDO_SCAN_TYPE_UNKNOWN   - Unknow
 *  @li@c  SB_FMRDO_SCAN_TYPE_FULL_MODE - Full mode
 *  @li@c  SB_FMRDO_SCAN_TYPE_ONCE_UP   - Scan up
 *  @li@c  SB_FMRDO_SCAN_TYPE_ONCE_DOWN - Scan down
 *  @li@c  SB_FMRDO_SCAN_TYPE_NUM       - Number of scan types
 */
/*---------------------------------------------------------------------------*/

typedef enum
{
    SB_FMRDO_SCAN_TYPE_UNKNOWN = 0,
    SB_FMRDO_SCAN_TYPE_FULL_MODE,
    SB_FMRDO_SCAN_TYPE_ONCE_UP,
    SB_FMRDO_SCAN_TYPE_ONCE_DOWN,
    SB_FMRDO_SCAN_TYPE_NUM    /*  */
} SB_FMRDO_SCAN_TYPE_T;

/*---------------------------------------------------------------------------*/
/*! @struct SB_FMRDO_SCAN_DATA_T
 *  @brief  This structure is used as the argument pv_scan_data of the API
 *          c_sb_start_scan() / x_sb_start_scan() when scaning a media.
 *  @code
 *  typedef struct _SB_FMRDO_SCAN_DATA_T
 *  {
 *      SB_FMRDO_SCAN_TYPE_T    e_scan_type;
 *      UINT32                  ui4_start_freq;
 *  } SB_FMRDO_SCAN_DATA_T;
 *  @endcode
 *
 *  @li@c   e_scan_type    - Scan type
 *  @li@c   ui4_start_freq - Start frequency of scanning
 */
/*---------------------------------------------------------------------------*/

typedef struct _SB_FMRDO_SCAN_DATA_T
{
    SB_FMRDO_SCAN_TYPE_T    e_scan_type;
    UINT32                  ui4_start_freq;
} SB_FMRDO_SCAN_DATA_T;

/*! @} */

#endif /* _U_SB_FMRDO_ENG_H_ */

