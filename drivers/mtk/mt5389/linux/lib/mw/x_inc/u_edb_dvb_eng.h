/*******************************************************************************
 * LEGAL DISCLAIMER
 *    
 * (Header of MediaTek Software/Firmware Release or Documentation)
 *    
 * BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
 * AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK 
 * SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO
 * BUYER ON AN "AS-IS" BASISONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL 
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH 
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO
 * LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES 
 * MADE TO BUYER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN
 * FORUM.
 * 
 * BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE 
 * LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, 
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO 
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 * 
 * THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH
 * THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS 
 * PRINCIPLES.
 ******************************************************************************/
/*------------------------------------------------------------------------------
 * Copyright (c) 2009, Mediatek Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @file u_edb_dvb_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         EDB DVB engine.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_EDB_EDB_DVB_ENG EDB DVB Engine
 *  @ingroup groupMW_EDB
 *  @brief Provides events operate function for DVB
 *  %This module is EDB DVB instance.
 *  @see groupMW_EDB
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


#ifndef _U_EDB_DVB_ENG_H_
#define _U_EDB_DVB_ENG_H_

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/
#include "u_common.h"


/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/
typedef UINT32      EDB_DVB_CONFIG_T;    /**< EDB DVB Engine config type       */

#define EDB_DVB_CONFIG_PARSE_EXTEND_ITEMS                         (MAKE_BIT_MASK_32(0))    /**< Config of parse extended event items*/
#define EDB_DVB_CONFIG_USE_MAX_EVENT_NAME                         (MAKE_BIT_MASK_32(1))    /**< Config of use max length retrive content*/
#define EDB_DVB_CONFIG_SHOW_ALL_LANG                              (MAKE_BIT_MASK_32(2))    /**< Config of show all language*/ /*For X customer show all language event information,other custom only show matched language*/
#define EDB_DVB_CONFIG_COMPRESS_SPACE                             (MAKE_BIT_MASK_32(3))   
#define EDB_DVB_CONFIG_REPLACE_TAB_WITH_SPACE                     (MAKE_BIT_MASK_32(4))   
#define EDB_DVB_CONFIG_USE_FIRST_EXTENDED_CONTROL_CODE            (MAKE_BIT_MASK_32(5))   

#endif /* _U_EDB_DVB_ENG_H_ */


/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_EDB */
/*----------------------------------------------------------------------------*/


