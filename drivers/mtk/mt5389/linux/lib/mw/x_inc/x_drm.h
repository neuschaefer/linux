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
 *         This header file contains DRM specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_DRM DRM
 *
 *  @ingroup groupMW
 *  @brief Provides a unified interface for DRM
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_DRM_H_
#define _X_DRM_H_
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "x_common.h"
#include "x_handle.h"
#include "x_handle_grp.h"
#include "x_drm_drv.h"
#include "u_drm.h"

#else

#include "x_common.h"
#include "x_handle.h"
#include "x_handle_grp.h"
#include "x_drm_drv.h"
#include "u_drm.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* The following lists all error codes for drm module. */
#define DRMR_RENTAL_EXPIRED ((INT32)3)              /**< DRM RETURN RENTAL EXPIRED */
#define DRMR_AUTH_ERROR ((INT32)2)                  /**< DRM RETURN AUTH ERROR */
#define DRMR_UNSPECIFIED_ERROR ((INT32)1)           /**< DRM RETURN UNSPECIFIED ERROR */
#define DRMR_OK ((INT32)0)                          /**< DRM RETURN OK */
#define DRMR_INV_ARG ((INT32)-1)                    /**< DRM RETURN INVALID ARGD */
#define DRMR_INV_HANDLE ((INT32)-2)                 /**< DRM RETURN INVALID HANDLE */
#define DRMR_INV_COMMAND ((INT32)-3)                /**< DRM RETURN INVALID COMMAND */
#define DRMR_OUT_OF_HANDLE ((INT32)-4)              /**< DRM RETURN OUT OF HANDLE */
#define DRMR_OUT_OF_MEMORY ((INT32)-5)              /**< DRM RETURN OUT OF MEMORY */
#define DRMR_INSUFFICIENT_BUFFER ((INT32)-6)        /**< DRM RETURN INSUFFICIENT BUFFER */
#define DRMR_CLI_ERROR ((INT32)-7)                  /**< DRM RETURN CLI ERROR */
#define DRMR_INITIAL_ERROR ((INT32)-8)              /**< DRM RETURN INITIAL ERROR */
#define DRMR_INTERNAL_ERROR ((INT32)-9)             /**< DRM RETURN INTERNAL ERROR */
#define DRMR_FCT_NOT_SUPPORT ((INT32)-10)           /**< DRM RETURN FUNCTION NOT SUPPORT */
#define DRMR_TYPE_NOT_SUPPORT ((INT32)-11)          /**< DRM RETURN TYPE NOT SUPPORT */
#define DRMR_HANDLER_NOT_SUPPORT ((INT32)-12)       /**< DRM RETURN HANDLER NOT SUPPORT */
#define DRMR_HANDLER_NAME_EXIST ((INT32)-13)        /**< DRM RETURN HANDLER NAME EXIST */
#define DRMR_INFO_NOT_SUPPORT ((INT32)-14)          /**< DRM RETURN INFO NOT SUPPORT */

/* This defines the none drm type. */
#define DRM_TYPE_NONE ((DRM_TYPE_T)0)               /**< DRM TYPE NONE */

/*---------------------------------------------------------------------------*/
/*! 
 *  @brief  This defines the type of command code.
 *  @code
 *  typedef UINT32 DRM_CMD_CODE_T;
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
typedef UINT32 DRM_CMD_CODE_T;

/* This defines the end of drm command. */
#define DRM_CMD_CODE_END ((DRM_CMD_CODE_T)0)        /**< DRM CMD CODE END */

/*---------------------------------------------------------------------------*/
/*! 
 *  @brief  This structure defines drm command.
 *  @code
 *  typedef struct
 *  {
 *      DRM_CMD_CODE_T e_code;
 *      union
 *      {
 *          BOOL b_boolean;
 *          UINT8 ui1_data;
 *          UINT16 ui2_data;
 *          UINT32 ui4_data;
 *          HANDLE_T h_obj;
 *          CHAR* ps_text;
 *          VOID* pv_data;
 *      } u;
 *  } DRM_COMMAND_T;
 *  @endcode
 *  @li@c  e_code     -  DRM CMD CODE
 *  @li@c  b_boolean  -  DRM CMD STRUCTURE
 *  @li@c  ui1_data   -  DRM CMD STRUCTURE
 *  @li@c  ui2_data   -  DRM CMD STRUCTURE
 *  @li@c  ui4_data   -  DRM CMD STRUCTURE
 *  @li@c  h_obj      -  DRM CMD STRUCTURE
 *  @li@c  ps_text    -  DRM CMD STRUCTURE
 *  @li@c  pv_data    -  DRM CMD STRUCTURE
 */
/*---------------------------------------------------------------------------*/
typedef struct
{
    DRM_CMD_CODE_T e_code;
    union
    {
        BOOL b_boolean;
        UINT8 ui1_data;
        UINT16 ui2_data;
        UINT32 ui4_data;
        HANDLE_T h_obj;
        CHAR* ps_text;
        VOID* pv_data;
    } u;
} DRM_COMMAND_T;

/* 
    set/get drm types definition.
    31                        20 19                                          0
    --------------------------------------------------------------
    |     Group(12 bits)  |         Code(20 bits)                |
    --------------------------------------------------------------   
*/
/*---------------------------------------------------------------------------*/
/*! 
 *  @brief  This defines set type of DRM.
 *  @code
 *  typedef UINT32 DRM_SET_TYPE_T;
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
typedef UINT32 DRM_SET_TYPE_T;
/*---------------------------------------------------------------------------*/
/*! 
 *  @brief  This defines get type of DRM.
 *  @code
 *  typedef UINT32 DRM_GET_TYPE_T;
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
typedef UINT32 DRM_GET_TYPE_T;

#define DRM_GRP_BITS 12                             /**< DRM GROUP SIZE IN BIT */
#define DRM_CODE_BITS 20                            /**< DRM CODE SIZE IN BIT */
#define DRM_GRP_MASK 0xFFF00000                     /**< DRM GROUP MASK */
#define DRM_CODE_MASK 0x000FFFFF                    /**< DRM CODE MASK */
#define DRM_GET_GRP(type) (type & DRM_GRP_MASK)     /**< DRM GET GROUP */
#define DRM_GET_CODE(type) (type & DRM_CODE_MASK)   /**< DRM GET CODE */

/* This defines the common group. */
#define DRM_GRP_COMMON (1 << DRM_CODE_BITS)         /**< DRM GROUP COMMON */
#define DRM_GRP_LAST_ENTRY (64)                     /**< DRM GROUP LAST */

/* The followings lists all drm get common types. */
#define DRM_GET_TYPE_OUTPUT_SIGNAL_PROTECTION_INFO ((DRM_GET_TYPE_T)(DRM_GRP_COMMON | 0))   /**< DRM GET TYPE OUTPUT SIGNAL PROTECTION INFO */

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
/*! @brief  This API opens a drm handle according to the drm command set.
 *
 *  @param  [in] pt_drm_cmd_sets  Specify the command set to initialize drm. 
 *  @param  [in] t_drm_type  Specify the drm type.
 *  @param  [in, out] ph_drm  Specify handle pointer contains the drm handle.
 *
 *  @return  INT32
 ----------------------------------------------------------------------------*/
extern INT32 x_drm_open (
    DRM_COMMAND_T* pt_drm_cmd_sets,
    DRM_TYPE_T t_drm_type,
    HANDLE_T* ph_drm);

/*---------------------------------------------------------------------------*/
/*! @brief  This API closes the drm handle.
 *
 *  @param  [in] h_drm  Specify the drm handle to be closed.
 *
 *  @return  INT32
 ----------------------------------------------------------------------------*/ 
extern INT32 x_drm_close (
    HANDLE_T h_drm);

/*---------------------------------------------------------------------------*/
/*! @brief  This API retrieves information of the drm handle.
 *
 *  @param  [in] h_drm  Specify the drm handle.
 *  @param  [in] t_drm_get_type  Specify the get information type of drm.
 *  @param  [in, out] pv_buf  Specify memory buffer to retrive the drm information.
 *  @param  [in] z_buf_leng  Specify length of the pv_buf. If it is smaller than 
                             required, return DRM_INSUFFICENT_MEM.
 *
 *  @return  INT32
 ----------------------------------------------------------------------------*/
extern INT32 x_drm_get_info (
    HANDLE_T h_drm,
    DRM_GET_TYPE_T t_drm_get_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng);

/*---------------------------------------------------------------------------*/
/*! @brief  This API sets information of the drm handle.
 *
 *  @param  [in] h_drm  Specify the drm handle.
 *  @param  [in] t_drm_set_type  Specify the set information type of drm.
 *  @param  [in] pv_buf  Specify memory buffer to set the drm information.
 *  @param  [in] z_buf_leng  Specify length of the pv_buf. 
 *
 *  @return  INT32
 ----------------------------------------------------------------------------*/ 
extern INT32 x_drm_set_info (
    HANDLE_T h_drm,
    DRM_SET_TYPE_T t_drm_set_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng);

/*---------------------------------------------------------------------------*/
/*! @brief  This API retrieves generic information of the specific drm type.
 *
 *  @param  [in] h_drm  Specify the drm handle.
 *  @param  [in] t_drm_get_type  Specify the get information type of drm.
 *  @param  [in, out] pv_buf  Specify memory buffer to retrive the drm information.
 *  @param  [in] z_buf_leng  Specify length of the pv_buf. If it is smaller than 
                             required, return DRM_INSUFFICENT_MEM.
 *
 *  @return  INT32
 ----------------------------------------------------------------------------*/
extern INT32 x_drm_get_generic_info (
    DRM_TYPE_T t_drm_type,
    DRM_GET_TYPE_T t_drm_get_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng);

/*---------------------------------------------------------------------------*/
/*! @brief  This API sets generic information of the specific drm type.
 *
 *  @param  [in] h_drm  Specify the drm handle.
 *  @param  [in] t_drm_set_type  Specify the set information type of drm.
 *  @param  [in] pv_buf  Specify memory buffer to set the drm information.
 *  @param  [in] z_buf_leng  Specify length of the pv_buf. 
 *
 *  @return  INT32
 ----------------------------------------------------------------------------*/  
extern INT32 x_drm_set_generic_info (
    DRM_TYPE_T t_drm_type,
    DRM_SET_TYPE_T t_drm_set_type,
    VOID* pv_buf,
    SIZE_T z_buf_leng);

#ifdef __cplusplus
}
#endif

#endif /* _X_DRM_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_DRM      */
/*----------------------------------------------------------------------------*/
