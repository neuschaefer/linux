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
/*! @file u_str_buf.h
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description: 
 *   This header file contains exported API for to
 *   store and retrive character string from the mini-buffer.
 *
 *   The mini-buffer provides API to store char string (NULL terminated)
 *   string in the buffer (x_str_buf_write_string()).  On success,
 *   this API returns string ID (integer value) which application can
 *   use to retrive the string (get a copy) or to delete this string from
 *   the mini-buffer.
 *
 *   
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_UTIL
  *  @{  
  */
/*----------------------------------------------------------------------------*/ 

#ifndef _U_STR_BUF_H_
#define _U_STR_BUF_H_

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/
    
#define STRBUFR_OK                  ((INT32)   0)       /**<Routine successful*/
#define STRBUFR_INV_ARG             ((INT32)  -1)       /**<One or more of the arguments contain invalid data.*/
#define STRBUFR_OUT_OF_MEM          ((INT32)  -2)       /**<There is no enough memory*/
#define STRBUFR_INV_STR_LEN         ((INT32)  -3)       /**<The string length is invalid*/
#define STRBUFR_INV_STR_ID          ((INT32)  -4)       /**<The object id is invalid*/
#define STRBUFR_INPUT_BUF_TOO_SMALL ((INT32)  -5)       /**<The input buffer is too small*/
#define STRBUFR_FAIL                ((INT32)  -6)       /**<Routine fail*/
#define STRBUFR_EMPTY               ((INT32)  -7)       /**<The memory buffer is empty*/


#define STR_BUF_MIN_BUF_SZ          (8)                 /**<Definition of minimize buffer size*/
#define STR_BUF_MIN_ARY_LEN         (1)                 /**<Definition of minimize array length*/
#define STR_BUF_MAX_STR_LEN         (255)               /**<Definition of maxmium object length*/
#define STR_BUF_NULL_STR_ID         (0)                 /**<Definition the ID of null object*/
#define STR_BUF_MEM_GROW_INC        (256)               /**<Definition the memory grow increase step*/
#define STR_BUF_ARY_GROW_INC        (16)                /**<Definition the array grow increase step*/
#define STR_BUF_MAX_SZ              (65536)             /**<Definition the buffer's maxnium size*/
#define STR_BUF_MAX_NB_STR_OBJ      ((65536)>>2)        /**<Definition the maxmium number of string object*/

/* ------------------------------------------------------------------------
                   ABORT code
 -------------------------------------------------------------------------*/
/*  Abort Macro and Abort code. */
#ifdef ABORT
#undef ABORT
#endif

/* Abort stuff. */
#define ABORT(_cat,_val)    DBG_ABORT((DBG_MOD_UTIL | _cat | _val))
/**<Definition of Util abort stuff*/

/* Error code. */
#define DBG_ABRT_STR_BUF_CORRUPTED         ((UINT32) 0x00000012)
/**<The error code of memory buffer corrupted*/

/*---------------------------------------------------------------------------
 Type definition
----------------------------------------------------------------------------*/

/* 
  Typedef for the string-buffer control structure       
  This type definition should keep the same as mem_buf's for type conversion
*/
/*------------------------------------------------------------------*/
/*! @brief typedef for the string-buffer control structure   
*  @code 
*  typedef struct _STR_BUF_T_ 
*  { 
*      UINT8*        pui1_str_buf;
*      UINT32*       pui4_ary; 
*      UINT32        ui4_obj_buf_sz;
*      UINT32        ui4_mem_used; 
*      UINT16        ui2_ary_len; 
*      UINT16        ui2_nb_str.   
*  } MEM_BUF_T; 
*  @endcode 
*  @li@c  pui1_str_buf      -address of string buffer. 
*  @li@c  pui4_ary               -array containing object's location (offset) in the buffer 'pui1_str_buf'. 
*  @li@c  ui4_obj_buf_sz      -size of 'pui1_str_buf'
*  @li@c  ui4_mem_used      -total memory used by objects. 
*  @li@c  ui2_ary_len           -nb of entries in 'pui4_ary'.
*  @li@c  ui2_nb_str           -number of strings. 
*/
/*------------------------------------------------------------------*/

typedef struct _STR_BUF_T_
{
                             
    UINT8*        pui1_str_buf;     /* address of memory buffer.  */
    UINT32*       pui4_ary;         /* array containing object's
                                       location (offset) in the buffer
                                       'pui1_mem_buf'. */
    UINT32        ui4_str_buf_sz;   /* size of 'pui1_mem_buf'. */
    UINT32        ui4_mem_used;     /* total memory used by objects. */
    
    UINT16        ui2_ary_len;      /* nb of entries in 'pui4_ary'.*/
    UINT16        ui2_nb_str;       /* number of objects. */
    
} STR_BUF_T ;


typedef  UINT16    STRING_ID_T;         /**<The typedef definition of STRING_ID_T*/

#endif   /*  _U_STR_BUF_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                               /* end of groupMW_UTIL*/
/*----------------------------------------------------------------------------*/


