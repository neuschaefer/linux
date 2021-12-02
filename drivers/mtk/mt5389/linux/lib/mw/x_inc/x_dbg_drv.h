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
 * $RCSfile: x_dbg_drv.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains debug driver related definitions, which
 *         are exported.
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_DBG
  *  @{  
  */
/*----------------------------------------------------------------------------*/

#ifndef _X_DBG_DRV_H_
#define _X_DBG_DRV_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_dbg_drv.h"
#include "x_dbg.h"
#include "x_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define SYS_EXIT  ((UINT32) 0)
/**<This definition of SYS_EXIT*/


/*------------------------------------------------------------------*/
/*! @brief  This type defines the data receive callback function prototype.
  *  @param [in] ui1_data        Contain the input data.
  */
/*------------------------------------------------------------------*/
typedef VOID (*x_dbg_data_rec_fct) (UINT8  ui1_data);


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* The following API's must ONLY be implemented by the manufacturer */
/* if the debug output is not routed via a driver registered with   */
/* the Resource Manager. This means, the debug library has been     */
/* compiled with the define EXT_DBG_DRV set.                        */

/*------------------------------------------------------------------*/
/*! @brief  This API open the oputput device.
*  @param [in]   ps_output_name     References the name of output device.
*  @param [in]   e_output_type        References the type of output device.
*  @param [in]   pv_output_info       References the info to open output device.
*  @retval      -DBGR_OK                Routine successful.
*  @retval      -DBG_FAIL                 Debug fail.
*/
/*------------------------------------------------------------------*/
extern INT32 x_dbg_open_output (const CHAR*        ps_output_name,
                                DBG_OUTPUT_TYPE_T  e_output_type,
                                const VOID*        pv_output_info);
                                


/*------------------------------------------------------------------*/
/*! @brief  This API output the log information.
*  @param [in]   ps_stmt                 References the log information.
*  @retval      -DBGR_OK                Routine successful.
*  @retval      -DBG_FAIL                 Debug fail.
*/
/*------------------------------------------------------------------*/
extern VOID x_dbg_put_stmt (const CHAR* ps_stmt);


/*------------------------------------------------------------------*/
/*! @brief  This API set the data receive callback function.
*  @param [in]   pf_data_rec           References the pointer of data receive callback function.
*  @retval      -DBGR_OK                Routine successful.
*  @retval      -DBG_FAIL                 Debug fail.
*/
/*------------------------------------------------------------------*/
extern VOID x_dbg_reg_data_rec (x_dbg_data_rec_fct  pf_data_rec);

#endif /* _X_DBG_DRV_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                                 /* end of groupMW_DBG*/
/*----------------------------------------------------------------------------*/

