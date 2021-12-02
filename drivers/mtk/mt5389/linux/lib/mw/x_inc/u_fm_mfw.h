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
/*! @file u_fm_mfw.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Multi-File-Wrapper exported constants, macros,
 *         and types.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_FM_MFW_H_
#define _U_FM_MFW_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_fm.h"


/*!
 * @defgroup    groupMW_FM_MFW Multi File Wrapper
 * @ingroup     groupMW_FM
 * @brief    This header file contains Multi-File-Wrapper exported constants, macros, and types.
 * %MFW is a wrapper of multi files, user can access it as a visual file.
 * 
 * @see groupMW_FM
 *
 * @{
 * */



/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @enum   FM_MFW_ASYNC_COND_T
 *  @brief  Async condition
 *  @code
 *  typedef enum _FM_MFW_ASYNC_COND_T
 *  {
 *      FM_MFW_ASYNC_COND_CREATE_FAIL,
 *      FM_MFW_ASYNC_COND_ANLS_FAIL,
 *      FM_MFW_ASYNC_COND_ABORT_FAIL,
 *      FM_MFW_ASYNC_COND_ABORT_OK,
 *      FM_MFW_ASYNC_COND_ANLS_OK,
 *      FM_MFW_ASYNC_COND_CREATE_OK,
 *      FM_MFW_ASYNC_COND_PRGS,  
 *      FM_MFW_ASYNC_COND_STATUS,
 *  } FM_MFW_ASYNC_COND_T;  
 *  @endcode
 *  @li@c  FM_MFW_ASYNC_COND_CREATE_FAIL                    - Failed to create MFW
 *  @li@c  FM_MFW_ASYNC_COND_ANLS_FAIL                       - Analyse the device failed
 *  @li@c  FM_MFW_ASYNC_COND_ABORT_FAIL                     - Failed to abort
 *  @li@c  FM_MFW_ASYNC_COND_ABORT_OK                        - Abort OK
 *  @li@c  FM_MFW_ASYNC_COND_ANLS_OK                         - Analysed OK
 *  @li@c  FM_MFW_ASYNC_COND_CREATE_OK                     - Created OK
 *  @li@c  FM_MFW_ASYNC_COND_PRGS                               - Notify progress 
 *  @li@c  FM_MFW_ASYNC_COND_STATUS                           - Notify status
 */
/*------------------------------------------------------------------*/
typedef enum _FM_MFW_ASYNC_COND_T
{
    FM_MFW_ASYNC_COND_CREATE_FAIL,
    FM_MFW_ASYNC_COND_ANLS_FAIL,
    FM_MFW_ASYNC_COND_ABORT_FAIL,
    FM_MFW_ASYNC_COND_ABORT_OK,
    FM_MFW_ASYNC_COND_ANLS_OK,
    FM_MFW_ASYNC_COND_CREATE_OK,
    FM_MFW_ASYNC_COND_PRGS,
    FM_MFW_ASYNC_COND_STATUS,
} FM_MFW_ASYNC_COND_T;  

/* (*x_fm_async_fct) (e_async_cond = FM_ASYNC_COND_READ_OK, ui4_data: (done bytes) ) */
/* (*x_fm_async_fct) (e_async_cond = FM_ASYNC_COND_WRITE_OK, ui4_data: (done bytes) ) */
/* (*x_fm_async_fct) (e_async_cond = FM_MFW_ASYNC_COND_FAIL, ui4_data: (N/A) ) */

/* (*x_fm_mfw_async_fct) (e_async_cond = FM_MFW_ASYNC_COND_ABORT_OK, ui4_data: (N/A) ) */
/* (*x_fm_mfw_async_fct) (e_async_cond = FM_MFW_ASYNC_COND_ANLS_OK, ui4_data: (FM_MFW_ANLS_RES_T *) ) */
/* (*x_fm_mfw_async_fct) (e_async_cond = FM_MFW_ASYNC_COND_CREATE_OK, ui4_data: (N/A) ) */
/* (*x_fm_mfw_async_fct) (e_async_cond = FM_MFW_ASYNC_COND_ABORT_FAIL, ui4_data: FM_MFW_NFY_FAIL_* ) */
/* (*x_fm_mfw_async_fct) (e_async_cond = FM_MFW_ASYNC_COND_CREATE_FAIL, ui4_data: FM_MFW_NFY_FAIL_* ) */
/* (*x_fm_mfw_async_fct) (e_async_cond = FM_MFW_ASYNC_COND_ANLS_FAIL, ui4_data: FM_MFW_NFY_FAIL_* ) */
#define FM_MFW_NFY_FAIL_CORE_ERROR                  ((UINT32) 1)   /**< Internal error or OSAI error */
#define FM_MFW_NFY_FAIL_DEVICE_ERROR                ((UINT32) 2)   /**< IO error */
#define FM_MFW_NFY_FAIL_NO_ENOUGH_SPACE             ((UINT32) 3)   /**< No enough free space in device */
#define FM_MFW_NFY_FAIL_NO_ENOUGH_CONTINS_SPACE     ((UINT32) 4)   /**< For FM_MFW_VF_POLICY_PHYSIC_CONTINS_FORCE, when there is no more continuous space */
#define FM_MFW_NFY_FAIL_ABORTED                     ((UINT32) 5)   /**< For FM_MFW_ASYNC_COND_ANLS_FAIL and FM_MFW_ASYNC_COND_CREATE_FAIL, when the analyse request is aborted */
#define FM_MFW_NFY_FAIL_ALREADY_DONE                ((UINT32) 6)   /**< For FM_MFW_ASYNC_COND_ABORT_FAIL, when the request is already done */
#define FM_MFW_NFY_FAIL_ALREADY_EXISTED             ((UINT32) 7)   /**< For FM_MFW_ASYNC_COND_ABORT_FAIL, when the request is already done */
#define FM_MFW_NFY_FAIL_UMOUNTED                    ((UINT32) 8)   /**< Internal error or OSAI error */
#define FM_MFW_NFY_FAIL_NOT_SUPPORTED               ((UINT32) 9)   /**<  Not supported now */

/* (*x_fm_mfw_async_fct) (e_nfy_code = FM_MFW_NFY_CODE_STATUS, ui4_data: FM_MFW_NFY_STATUS_* ) */
#define FM_MFW_NFY_STATUS_NO_ENOUGH_CONTINS_SPACE   ((UINT32) 1)   /*  */    /**<  For FM_MFW_VF_POLICY_PHYSIC_CONTINS_PREFER, when there is no more continuous space, AP may do sth for this. */

/* (*x_fm_mfw_async_fct) (e_nfy_code = FM_MFW_ASYNC_COND_PRGS, ui4_data: (FM_MFW_PROGRESS_T *) ) */
/*------------------------------------------------------------------*/
/*! @struct FM_MFW_PRGS_T
 *  @brief Analyse or create progress info.
 *  @code
 *  typedef struct _FM_MFW_PRGS_T
 *  {
 *      UINT8               ui1_total_phases;
 *      UINT8               ui1_cur_phase;
 *      UINT32              ui4_prgs;
 *  } FM_MFW_PRGS_T;
 *  @endcode
 *  @li@c  ui1_total_phases                                  - Total phases
 *  @li@c  ui1_cur_phase                                     - Current phase
 *  @li@c  ui4_prgs                                             - Current progress 
 */
/*------------------------------------------------------------------*/
typedef struct _FM_MFW_PRGS_T
{
    UINT8               ui1_total_phases;
    UINT8               ui1_cur_phase;
    UINT32              ui4_prgs;
} FM_MFW_PRGS_T;


/*------------------------------------------------------------------*/
/*! @brief async callback to tell AP the result of the analysing or creating operation.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_fm_mfw_async_fct)(
    HANDLE_T            h_req,
    VOID                *pv_tag,
    FM_MFW_ASYNC_COND_T e_async_cond,
    UINT32              ui4_data);


/*------------------------------------------------------------------*/
/*! @enum   FM_MFW_VF_POLICY_T
 *  @brief  Policy applied when create visual files
 *  @code
 *  typedef enum _FM_MFW_VF_POLICY_T
 *  {
 *      FM_MFW_VF_POLICY_NO_CONSTRAINT,
 *      FM_MFW_VF_POLICY_PHYSIC_CONTINS_FORCE,
 *      FM_MFW_VF_POLICY_PHYSIC_CONTINS_PREFER    
 *  } FM_MFW_VF_POLICY_T;
 *  @endcode
 *  @li@c  FM_MFW_VF_POLICY_NO_CONSTRAINT                    -
 *  @li@c  FM_MFW_VF_POLICY_PHYSIC_CONTINS_FORCE             -
 *  @li@c  FM_MFW_VF_POLICY_PHYSIC_CONTINS_PREFER            -
 */
/*------------------------------------------------------------------*/
typedef enum _FM_MFW_VF_POLICY_T
{
    FM_MFW_VF_POLICY_NO_CONSTRAINT,
    FM_MFW_VF_POLICY_PHYSIC_CONTINS_FORCE,
    FM_MFW_VF_POLICY_PHYSIC_CONTINS_PREFER    
} FM_MFW_VF_POLICY_T;

/*------------------------------------------------------------------*/
/*! @enum   FM_MFW_VF_POLICY_T
 *  @brief  Meta file policy applied when create visual files
 *  @code
 *  typedef enum _FM_MFW_VF_META_POLICY_T
 *  {
 *      FM_MFW_VF_META_POLICY_VISIBLE_META_FILE,
 *      FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE   
 *  } FM_MFW_VF_META_POLICY_T;
 *  @endcode
 *  @li@c  FM_MFW_VF_META_POLICY_VISIBLE_META_FILE                 - Visible meta file
 *  @li@c  FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE             - Invisible meta file
 */
/*------------------------------------------------------------------*/
typedef enum _FM_MFW_VF_META_POLICY_T
{
    FM_MFW_VF_META_POLICY_VISIBLE_META_FILE,
    FM_MFW_VF_META_POLICY_INVISIBLE_META_FILE
} FM_MFW_VF_META_POLICY_T;

/*------------------------------------------------------------------*/
/*! @struct FM_MFW_ANLS_PARAM_T
 *  @brief  Analysing parameters
 *  @code
 *  typedef struct _FM_MFW_ANLS_PARAM_T
 *  {
 *      UINT8                   ui1_strct_ver;
 *      CHAR*                   s_mp_path;
 *      x_fm_mfw_async_fct      pf_nfy;
 *      VOID*                   pv_tag;
 *  } FM_MFW_ANLS_PARAM_T;
 *  @endcode
 *  @li@c  ui1_strct_ver                                  - Version
 *  @li@c  s_mp_path                                     - Mount point path
 *  @li@c  pf_nfy                                            - Callback function
 *  @li@c  pv_tag                                           - Tag
 */
/*------------------------------------------------------------------*/
typedef struct _FM_MFW_ANLS_PARAM_T
{
    UINT8                   ui1_strct_ver;
    CHAR*                   s_mp_path;
    x_fm_mfw_async_fct      pf_nfy;
    VOID*                   pv_tag;
} FM_MFW_ANLS_PARAM_T;


/*------------------------------------------------------------------*/
/*! @struct FM_MFW_ANLS_RESULT_T
 *  @brief  Analysing results
 *  @code
 *  typedef struct _FM_MFW_ANLS_RESULT_T
 *  {
 *      UINT64  ui8_continuous_size;
 *      UINT64  ui8_avail_size;
 *  } FM_MFW_ANLS_RESULT_T;
 *  @endcode
 *  @li@c  ui8_continuous_size  -   Size of all the continuous regions in device;
 *                                  Continuous region is a region whose size is not less than 32MB
 *  @li@c  ui8_avail_size       -   Available space in the device
 */
/*------------------------------------------------------------------*/
typedef struct _FM_MFW_ANLS_RESULT_T
{
    /* Size of all the continuous regions in device;
       Continuous region is a region whose size is not less than 32MB */
    UINT64  ui8_continuous_size;

    /* Available space in the device */
    UINT64  ui8_avail_size;
} FM_MFW_ANLS_RESULT_T;


/*------------------------------------------------------------------*/
/*! @struct FM_MFW_VF_CFG_T
 *  @brief  Creating configs
 *  @code
 *  typedef struct _FM_MFW_VF_CFG_T
 *  {
 *      UINT8                   ui1_strct_ver;
 *      FM_MFW_VF_POLICY_T      e_vf_policy;
 *      FM_MFW_VF_META_POLICY_T e_meta_policy;
 *      UINT64                  ui8_init_size;
 *      x_fm_mfw_async_fct      pf_nfy;
 *      VOID*                   pv_tag; 
 *      CHAR*                   s_dir;
 *      CHAR*                   s_meta_file;
 *      CHAR*                   s_fn_pattern;
 *  } FM_MFW_VF_CFG_T;  
 *  @endcode
 *  @li@c  ui1_strct_ver                                 - Version
 *  @li@c  e_vf_policy                                    - VF policy
 *  @li@c  e_meta_policy                               - Meta policy
 *  @li@c  ui8_init_size                                  - Init size
 *  @li@c  pf_nfy                                           - Callback function
 *  @li@c  pv_tag                                          - Tag
 *  @li@c  s_dir                                             - The path of the directory to gengrate the wrapper files.
 *  @li@c  s_meta_file                                   - s_meta_file is valid only when e_meta_policy equals to FM_MFW_VF_META_POLICY_VISIBLE_META_FILE 
 *                                                                    s_meta_file is file name only, should not contain any path.
 *  @li@c  s_fn_pattern                                 -  s_fn_pattern is used for generate the name of the files in the wrapper.
 *                                                                    s_fn_pattern should like "xxxxxx%xdxxx"(for example, "Timeshift_PRG1_%03d.ts"), otherwise the result is unknown.
 *                                                                    s_fn_pattern is file name only, should not contain any path.
 */
/*------------------------------------------------------------------*/
typedef struct _FM_MFW_VF_CFG_T
{
    UINT8                   ui1_strct_ver;

    FM_MFW_VF_POLICY_T      e_vf_policy;
    
    FM_MFW_VF_META_POLICY_T e_meta_policy;
    
    UINT64                  ui8_init_size;
    
    x_fm_mfw_async_fct      pf_nfy;
    
    VOID*                   pv_tag;

    /* The path of the directory to gengrate the wrapper files.  */
    CHAR*                   s_dir;
    
    /* 
           s_meta_file is valid only when e_meta_policy equals to FM_MFW_VF_META_POLICY_VISIBLE_META_FILE 
           s_meta_file is file name only, should not contain any path.
       */
    CHAR*                   s_meta_file;

    /* 
           s_fn_pattern is used for generate the name of the files in the wrapper.
           s_fn_pattern should like "xxxxxx%xdxxx"(for example, "Timeshift_PRG1_%03d.ts"), otherwise the result is unknown.
           s_fn_pattern is file name only, should not contain any path.
       */
    CHAR*                   s_fn_pattern;

} FM_MFW_VF_CFG_T;  


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*! @}  */

#endif /* _U_FM_MFW_H */
