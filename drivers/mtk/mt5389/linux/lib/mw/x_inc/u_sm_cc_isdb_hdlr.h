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
/*! @file u_sm_cc_isdb_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains ISDB closed caption Stream handler specific 
 *         definitions, which are exported to applications.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_CC_ISDB_HDLR_H_
#define _U_SM_CC_ISDB_HDLR_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR_CCI_HDLR ISDB Closed Caption Handler
  *  @ingroup groupMW_STRM_MNGR
  *  @brief Provides external function TV Application
  *  %MW Application API would be used by TV Application, lot of these APIs would
  *   be wrapped from Exported API
  *  @see groupMW_STRM_MNGR_CCI_HDLR
  *  @{ 
  */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define SM_CCI_CMD_CODE_SET_OSD_HANDLE      ((SM_CMD_CODE_T)    1)    /**< set OSD canvas handle */
#define SM_CCI_CMD_CODE_SET_LANG_ID         ((SM_CMD_CODE_T)    2)    /**< set language data */


/* set/get type groups *******************************************************/
#define SM_GRP_CCI                          (( SM_GRP_LAST_ENTRY + 13 ) << SM_CODE_BITS)    /**< get/set command group id */
    
/* set types *****************************************************************/
#define SM_CCI_SET_TYPE_IGNORE          ((SM_SET_TYPE_T)(SM_GRP_CCI | 0))    /**< setting type is IGNORE */
    
/* get types *****************************************************************/
#define SM_CCI_GET_TYPE_IGNORE          ((SM_GET_TYPE_T)(SM_GRP_CCI | 0))    /**< getting type is IGNORE */


/*------------------------------------------------------------------*/
/*! @brief     Indicates the ISDB closed caption justify type.
  *  @code 
  *  typedef enum
  *  {
  *      SM_CCI_ATTRIBS_UNKNOWN           = 0,
  *      SM_CCI_ATTRIBS_EXTERN_SIZE,
  *      SM_CCI_ATTRIBS_EQUAL_WIDTH,
  *      SM_CCI_ATTRIBS_AUTO_LINE_FEED,
  *      SM_CCI_ATTRIBS_ROLL_UP_MODE
  *  }   SM_CCI_ATTRIBS_TYPE_T;
  *  @endcode 
  *  @li@c  SM_CCI_ATTRIBS_UNKNOWN          -The justify type is invalid.
  *  @li@c  SM_CCI_ATTRIBS_EXTERN_SIZE      -The justify type is switch of EXTERN_SIZE.
  *  @li@c  SM_CCI_ATTRIBS_EQUAL_WIDTH      -The justify type is switch of EQUAL_WIDTH.
  *  @li@c  SM_CCI_ATTRIBS_AUTO_LINE_FEED   -The justify type is switch of AUTO_LINE_FEED.
  *  @li@c  SM_CCI_ATTRIBS_ROLL_UP_MODE     -The justify type is switch of ROLL_UP_MODE.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CCI_ATTRIBS_UNKNOWN           = 0,
    SM_CCI_ATTRIBS_EXTERN_SIZE,
    SM_CCI_ATTRIBS_EQUAL_WIDTH,
    SM_CCI_ATTRIBS_AUTO_LINE_FEED,
    SM_CCI_ATTRIBS_ROLL_UP_MODE
}   SM_CCI_ATTRIBS_TYPE_T;


/*------------------------------------------------------------------*/
/*! @brief the structure of ISDB closed caption attribute information.
  *  @code 
  *  typedef struct  _SM_CCI_ATTRIBS_T
  *  {
  *      SM_CCI_ATTRIBS_TYPE_T       t_cci_attrib_type;
  *      BOOL                        fg_enable_flag;
  *  }   SM_CCI_ATTRIBS_T;
  *  @endcode 
  *  @li@c  t_cci_attrib_type           -Contains the attribute type.
  *  @li@c  fg_enable_flag              -Contains the attribute flag to indicates it is valid or not.
  */  
/*------------------------------------------------------------------*/
typedef struct  _SM_CCI_ATTRIBS_T
{
    SM_CCI_ATTRIBS_TYPE_T       t_cci_attrib_type;
    BOOL                        fg_enable_flag;
}   SM_CCI_ATTRIBS_T;


/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/


#endif /* _U_SM_CC_ISDB_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                /* end of groupMW_STRM_MNGR_CCI_HDLR */
/*----------------------------------------------------------------------------*/


