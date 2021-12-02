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
/*! @defgroup groupMW_DRM_DIVX DIVX Digital Rights Management
 *
 *  @ingroup groupMW_DRM
 *  @brief Provides a unified interface for DIVX DRM
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DIVX_DRM_H_
#define _U_DIVX_DRM_H_
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "u_common.h"
#include "u_drm.h"

#else

#include "u_common.h"
#include "u_drm.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define DRM_TYPE_DIVX_DRM ((DRM_TYPE_T)1) /**< This defines the divx drm type. */ 

/*---------------------------------------------------------------------------*/
/*! @struct DIVX_DRM_INIT_CONFIG_T
 *  @brief  This structure defines the divx drm initial configuration.
 *  @code
 *  typedef struct _DIVX_DRM_INIT_CONFIG_T
 *  {
 *      UINT16 ui2_divx_drm_memory_offset;
 *      UINT16 ui2_divx_drm_memory_size_byte;
 *      UINT8 ui1_divx_drm_model_id_low_byte;
 *      UINT8 ui1_divx_drm_model_id_high_byte;    
 *  } DIVX_DRM_INIT_CONFIG_T;
 *  @endcode
 *  @li@c  ui2_divx_drm_memory_offset      - drm memory offset of the whole 
 *                                           EEPROM.
 *  @li@c  ui2_divx_drm_memory_size_byte   - drm memory size.
 *  @li@c  ui1_divx_drm_model_id_low_byte  - low byte of drm model id.
 *  @li@c  ui1_divx_drm_model_id_high_byte - high byte of drm model id.
 */
/*---------------------------------------------------------------------------*/
typedef struct _DIVX_DRM_INIT_CONFIG_T
{
    UINT16 ui2_divx_drm_memory_offset;
    UINT16 ui2_divx_drm_memory_size_byte;
    UINT8 ui1_divx_drm_model_id_low_byte;
    UINT8 ui1_divx_drm_model_id_high_byte;    
} DIVX_DRM_INIT_CONFIG_T;

/*---------------------------------------------------------------------------*/
/*! @struct DIVX_DRM_REGISTRATION_INFO_T
 *  @brief  This structure defines the divx drm registration info.
 *  @code
 *  typedef struct _DIVX_DRM_REGISTRATION_INFO_T
 *  {
 *      CHAR s_registration_code[11];
 *  } DIVX_DRM_REGISTRATION_INFO_T;
 *  @endcode
 *  @li@c  s_registration_code  - divx drm registration code.
 */
/*---------------------------------------------------------------------------*/
typedef struct _DIVX_DRM_REGISTRATION_INFO_T
{
    CHAR s_registration_code[11/*DRM_REGISTRATION_CODE_BYTES*/];
} DIVX_DRM_REGISTRATION_INFO_T;

/*---------------------------------------------------------------------------*/
/*! @struct DIVX_DRM_DEACTIVATION_INFO_T
 *  @brief  This structure defines the divx drm deactivation info.
 *  @code
 *  typedef struct _DIVX_DRM_DEACTIVATION_INFO_T
 *  {
 *      CHAR s_deactivation_code[9];
 *  } DIVX_DRM_DEACTIVATION_INFO_T;
 *  @endcode
 *  @li@c  s_deactivation_code  - divx drm deactivation code.
 */
/*---------------------------------------------------------------------------*/
typedef struct _DIVX_DRM_DEACTIVATION_INFO_T
{
    CHAR s_deactivation_code[9];
} DIVX_DRM_DEACTIVATION_INFO_T;

/*---------------------------------------------------------------------------*/
/*! @struct DIVX_DRM_UI_HELP_INFO_T
 *  @brief  This structure defines the divx drm UI info.
 *  @code
 *  typedef struct _DIVX_DRM_UI_HELP_INFO_T
 *  {
 *  #define DIVX_DRM_REGISTRATION_CODE_HIDE ((UINT32)1 << 0)
 *  #define DIVX_DRM_DEACTIVATION_CONFIRMATION ((UINT32)1 << 1)
 *      UINT32 ui4_divx_drm_ui_help_info;
 *  } DIVX_DRM_UI_HELP_INFO_T;
 *  @endcode
 *  @li@c  ui4_divx_drm_ui_help_info  - flag to indicate divx drm ui display.
 */
/*---------------------------------------------------------------------------*/
typedef struct _DIVX_DRM_UI_HELP_INFO_T
{
#define DIVX_DRM_REGISTRATION_CODE_HIDE ((UINT32)1 << 0)
#define DIVX_DRM_DEACTIVATION_CONFIRMATION ((UINT32)1 << 1)
#define DIVX_DRM_HAVE_REGISTERED ((UINT32)1 << 2)
    UINT32 ui4_divx_drm_ui_help_info;
} DIVX_DRM_UI_HELP_INFO_T;

/*---------------------------------------------------------------------------*/
/*! 
 *  @brief  This defines the divx drm memory read function pointer.
 *  @code
 *  typedef BOOL (*divx_drm_memory_read_func)(UINT8* pui1_divx_drm_mem);
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
typedef BOOL (*divx_drm_memory_read_func)(UINT8* pui1_divx_drm_mem);

/*---------------------------------------------------------------------------*/
/*! 
 *  @brief  This defines the divx drm memory write function pointer.
 *  @code
 *  typedef BOOL (*divx_drm_memory_write_func)(UINT8* pui1_divx_drm_mem);
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
typedef BOOL (*divx_drm_memory_write_func)(UINT8* pui1_divx_drm_mem);

/*---------------------------------------------------------------------------*/
/*! @struct DIVX_DRM_MEMORY_RW_FUNC_INFO_T
 *  @brief  This structure defines the divx drm memory r/w functions.
 *  @code
 *  typedef struct _DIVX_DRM_MEMORY_RW_FUNC_INFO_T
 *  {
 *     divx_drm_memory_read_func pfn_divx_drm_memory_read_func;
 *     divx_drm_memory_read_func pfn_divx_drm_memory_write_func;
 *  } DIVX_DRM_MEMORY_RW_FUNC_INFO_T;
 *  @endcode
 *  @li@c  pfn_divx_drm_memory_read_func   - divx drm memory read function pointer.
 *  @li@c  pfn_divx_drm_memory_write_func  - divx drm memory write function pointer.
 */
/*---------------------------------------------------------------------------*/
typedef struct _DIVX_DRM_MEMORY_RW_FUNC_INFO_T
{
    divx_drm_memory_read_func pfn_divx_drm_memory_read_func;
    divx_drm_memory_read_func pfn_divx_drm_memory_write_func;
} DIVX_DRM_MEMORY_RW_FUNC_INFO_T;

/*---------------------------------------------------------------------------*/
/*! @struct DIVX_DRM_BASIC_INFO_T
 *  @brief  This structure defines the divx drm basic info.
 *  @code
 *  typedef struct _DIVX_DRM_BASIC_INFO_T
 *  {
 *  #define DIVX_DRM_RENTAL ((UINT8)1 << 0)
 *  #define DIVX_DRM_RENTAL_EXPIRED ((UINT8)1 << 1)
 *  #define DIVX_DRM_AUTH_ERROR ((UINT8)1 << 2)
 *      UINT8 ui1_flag;
 *      UINT8 ui1_use_count;
 *      UINT8 ui1_use_limit;
 *  } DIVX_DRM_BASIC_INFO_T;
 *  @endcode
 *  @li@c  ui1_flag       - flag to indicate divx drm ui display.
 *  @li@c  ui1_use_count  - use count of the dive drm clip.
 *  @li@c  ui1_use_limit  - use limit of the dive drm clip.
 */
/*---------------------------------------------------------------------------*/
typedef struct _DIVX_DRM_BASIC_INFO_T
{
#define DIVX_DRM_RENTAL ((UINT8)1 << 0)
#define DIVX_DRM_RENTAL_EXPIRED ((UINT8)1 << 1)
#define DIVX_DRM_AUTH_ERROR ((UINT8)1 << 2)
    UINT8 ui1_flag;
    UINT8 ui1_use_count;
    UINT8 ui1_use_limit;
} DIVX_DRM_BASIC_INFO_T;

/*---------------------------------------------------------------------------*/
/*! 
 *  @brief  This union defines divx drm generic info.
 *  @code
 *  typedef union _DIVX_DRM_UNION_GENERIC_INFO_T
 *  {
 *      DIVX_DRM_REGISTRATION_INFO_T t_divx_drm_registration_info;
 *      DIVX_DRM_DEACTIVATION_INFO_T t_divx_drm_deactivation_info;
 *      DIVX_DRM_UI_HELP_INFO_T t_divx_drm_ui_help_info;
 *      DIVX_DRM_MEMORY_RW_FUNC_INFO_T t_divx_drm_memory_rw_func_info; 
 *  } DIVX_DRM_UNION_GENERIC_INFO_T;
 *  @endcode
 *  @li@c  t_divx_drm_registration_info  - divx drm registration info.
 *  @li@c  t_divx_drm_deactivation_info  - divx drm deactivation info.
 *  @li@c  t_divx_drm_ui_help_info       - divx drm ui help info.
 *  @li@c  t_divx_drm_eeprom_rw_func_info   - divx drm memory r/w func info. 
 */
/*---------------------------------------------------------------------------*/
typedef union _DIVX_DRM_UNION_GENERIC_INFO_T
{
    DIVX_DRM_REGISTRATION_INFO_T t_divx_drm_registration_info;
    DIVX_DRM_DEACTIVATION_INFO_T t_divx_drm_deactivation_info;
    DIVX_DRM_UI_HELP_INFO_T t_divx_drm_ui_help_info;
    DIVX_DRM_MEMORY_RW_FUNC_INFO_T t_divx_drm_memory_rw_func_info;
} DIVX_DRM_UNION_GENERIC_INFO_T;

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _U_DIVX_DRM_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_DRM_DIVX */
/*----------------------------------------------------------------------------*/
