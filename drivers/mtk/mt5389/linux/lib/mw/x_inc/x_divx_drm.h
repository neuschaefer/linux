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
/*! @file 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains DIVX DRM specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_DRM_DIVX
 *  @{  
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_DIVX_DRM_H_
#define _X_DIVX_DRM_H_
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "x_common.h"
#include "x_drm.h"
#include "u_divx_drm.h"

#else

#include "x_common.h"
#include "x_drm.h"
#include "u_divx_drm.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* This defines the divx drm group. */
#define DRM_GRP_DIVX_DRM (2 << DRM_CODE_BITS)                                               /**< DIVX DRM GROUP */

/* The following lists all divx drm commands. */
#define DRM_CMD_CODE_DIVX_DRM_STRD_CHUNK  ((DRM_CMD_CODE_T) (DRM_GRP_DIVX_DRM | 0))         /**< DIVX DRM CMD STRD CHUNK */
#define DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE  ((DRM_CMD_CODE_T) (DRM_GRP_DIVX_DRM | 1))  /**< DIVX DRM CMD REGISTRATION CODE */
#define DRM_CMD_CODE_DIVX_DRM_DEACTIVATION_CODE  ((DRM_CMD_CODE_T) (DRM_GRP_DIVX_DRM | 2))  /**< DIVX DRM CMD DEACTIVATION CODE */

/* The following lists all drm set divx drm types. */
#define DRM_SET_TYPE_DIVX_DRM_PLAYBACK ((DRM_SET_TYPE_T)(DRM_GRP_DIVX_DRM | 0))             /**< DIVX DRM SET TYPE PLAYBACK */

/* The following lists all drm get divx drm types. */
#define DRM_GET_TYPE_DIVX_DRM_BASIC_INFO ((DRM_GET_TYPE_T)(DRM_GRP_DIVX_DRM | 0))           /**< DIVX DRM GET TYPE BASIC INFO */
#define DRM_GET_TYPE_DIVX_DRM_CRYPTO_INFO ((DRM_GET_TYPE_T)(DRM_GRP_DIVX_DRM | 1))          /**< DIVX DRM GET TYPE CRYPTO INFO */

/*---------------------------------------------------------------------------*/
/*! 
 *  @brief  This union defines divx drm info.
 *  @code
 *  typedef union _DIVX_DRM_UNION_INFO_T
 *    DIVX_DRM_REGISTRATION_INFO_T* pt_divx_drm_registration_info;
 *    DIVX_DRM_DEACTIVATION_INFO_T* pt_divx_drm_deactivation_info;    
 *    DIVX_DRM_BASIC_INFO_T* pt_divx_drm_basic_info;
 *    DIVX_DRM_CRYPTO_INFO_T* pt_divx_drm_crypto_info;
 *    DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T* pt_drm_output_signal_protection_info;
 *  } DIVX_DRM_UNION_INFO_T;
 *  @endcode
 *  @li@c  pt_divx_drm_registration_info            - divx drm registration info.
 *  @li@c  pt_divx_drm_deactivation_info            - divx drm deactivation info.
 *  @li@c  pt_divx_drm_basic_info                   - divx drm basic info.
 *  @li@c  pt_divx_drm_crypto_info                  - divx drm crypto info.
 *  @li@c  pt_drm_output_signal_protection_info     - divx drm signal protection info. 
 */
/*---------------------------------------------------------------------------*/
typedef union _DIVX_DRM_UNION_INFO_T{
    DIVX_DRM_REGISTRATION_INFO_T* pt_divx_drm_registration_info;
    DIVX_DRM_DEACTIVATION_INFO_T* pt_divx_drm_deactivation_info;    
    DIVX_DRM_BASIC_INFO_T* pt_divx_drm_basic_info;
    DIVX_DRM_CRYPTO_INFO_T* pt_divx_drm_crypto_info;
    DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T* pt_drm_output_signal_protection_info;
} DIVX_DRM_UNION_INFO_T;

/* This defines the divx drm generic group. */
#define DRM_GRP_DIVX_DRM_GENERIC (3 << DRM_CODE_BITS)                                                      /**< DIVX DRM GENERIC GROUP */

/* The following lists all drm set divx drm generic types. */
#define DRM_SET_TYPE_DIVX_DRM_GENERIC_DEACTIVATION ((DRM_SET_TYPE_T)(DRM_GRP_DIVX_DRM_GENERIC | 0))        /**< DIVX DRM GENERIC SET TYPE DEACTIVATION */
#define DRM_SET_TYPE_DIVX_DRM_GENERIC_MEMORY_RW_FUNC ((DRM_SET_TYPE_T)(DRM_GRP_DIVX_DRM_GENERIC | 1))      /**< DIVX DRM GENERIC SET MEMORY RW FUNC */


/* The following lists all drm get divx drm generic types. */
#define DRM_GET_TYPE_DIVX_DRM_GENERIC_REGISTRATION_INFO ((DRM_GET_TYPE_T)(DRM_GRP_DIVX_DRM_GENERIC | 0))   /**< DIVX DRM GENERIC GET TYPE REGISTRATION INFO */
#define DRM_GET_TYPE_DIVX_DRM_GENERIC_UI_HELP_INFO ((DRM_GET_TYPE_T)(DRM_GRP_DIVX_DRM_GENERIC | 1))        /**< DIVX DRM GENERIC UI HELP INFO */

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*! @brief  This API initializes the divx drm.
 *
 *  @param  [in] pt_divx_drm_init_config  Reference to the divx drm 
 *               configuration structure.
 *
 *  @return  INT32
 */
/*---------------------------------------------------------------------------*/
extern INT32 x_divx_drm_init (
    DIVX_DRM_INIT_CONFIG_T* pt_divx_drm_init_config);

#ifndef __KERNEL__
/*---------------------------------------------------------------------------*/
/*! @brief  This API provides the divx drm memory info.
 *
 *  @param  [in, out] pui2_divx_drm_memory_offset  Reference to the divx drm 
 *                    offset in whole eeprom.
 *  @param  [in, out] pui2_divx_drm_memory_size_byte  Reference to the divx 
 *                    drm size in byte.
 *
 *  @return  VOID
 */
/*---------------------------------------------------------------------------*/
extern VOID x_divx_drm_mem_info (
    UINT16* pui2_divx_drm_memory_offset,
    UINT16* pui2_divx_drm_memory_size_byte);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _X_DIVX_DRM_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_DRM_DIVX */
/*----------------------------------------------------------------------------*/
