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
/*! @file u_mtp.h 
 *  $RCSfile: u_mtp.h,v$
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file contains File Manager exported API's.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_MTP_MNGR Multimedia Transfer Protocol
 *  @ingroup groupMW
 *  @brief Provides a unified interface for MTP
 *  %This module implements Media Transfer Protocol Enhanced Revision 0.96 which mount 
 *   the MTP device as normal storage device and provides query functionality, 
 *   properties collection function for applications and provide application a flexible way to 
 *   configure directory appearance in File manager.
 *   - including these sub modules:
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_MTP_H_
#define _U_MTP_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/

#define MAX_ARRAY_ALLOC_SIZE    (0xFFFFFFFF)   /**  */    /**<  Most bytes to allocate for a single array      */
#define MAXSTRINGSIZE           (255)          /**  */    /**<  per MTP spec: most characters a string can have      */

#define OBJECTHANDLE_ALL        (0xFFFFFFFF)   /**  */    /**<  per MTP spec: special value to indicate all object handles      */
#define OBJECTHANDLE_UNDEFINED  (0x0)          /**  */    /**< per MTP spec: special value to indicate an undefined object handle       */
#define OBJECTHANDLE_ROOT       (0xFFFFFFFF)   /**  */    /**< per MTP spec: special value to indicate root object handle       */

#define STORAGEID_ALL           (0xFFFFFFFF)   /**  */    /**< per MTP spec: special value to indicate all storage ids       */
#define STORAGEID_DEFAULT       (0)            /**  */    /**< per MTP spec: special value to indicate default storage id       */
#define STORAGEID_UNDEFINED     (0)            /** */    /**<  per MTP spec: special value to indicate an undefined storage id       */
#define STORAGEID_PHYSICAL      (0xFFFF0000)   /**  */    /**<   per MTP spec: physical portion of storage id     */
#define STORAGEID_LOGICAL       (0x0000FFFF)   /**  */    /**<  per MTP spec: logical portion of storage id      */

#define SESSIONID_ALL           (0)            /**  */    /**< per MTP spec: special value to indicate all session ids       */
#define SESSIONID_NOSESSION     (0)            /**  */    /**<  per MTP spec: special value to indicate no session id      */

#define TRANSACTIONID_ALL       (0xFFFFFFFF)   /**  */    /**<  per MTP spec: special value to indicate all transaction ids      */
#define TRANSACTIONID_NOSESSION (0)            /**  */    /**<  per MTP spec: special value to indicate no session      */
#define TRANSACTIONID_MIN       (1)            /**  */    /**<  per MTP spec: minimum transaction id      */
#define TRANSACTIONID_MAX       (0xFFFFFFFE)   /**  */    /**<  per MTP spec: maximum transaction id, will wrap to min after this      */

#define DATASIZE_UNKNOWN        (0xFFFFFFFF)    /**<  unknown data size      */

#define COMMAND_NUMPARAMS_MAX   (5)            /**  */    /**<  per MTP spec: max parameters a command can have      */
#define RESPONSE_NUMPARAMS_MAX  (5)            /**  */    /**<  per MTP spec: max parameters a response can have      */
#define EVENT_NUMPARAMS_MAX     (3)            /**  */    /**< per MTP spec: max parameters an event can have       */

#define FORMATMASK_IMAGE        (0x0800)    /**< mask image format       */
#define DATATYPE_ARRAYMASK      (0x4000)    /**< mask array data type       */

#define ERROR_MTP_SPECIFIC      (0x80004005)    /**<  specific MTP error      */


/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define MTPR_REQUEST_CONTINUE          ((INT32)    2)    /**<   return request continue     */
#define MTPR_REQUEST_COMPLETED         ((INT32)    1)    /**<   return request completed     */
#define MTPR_OK                        ((INT32)    0)    /**<   return ok     */
#define MTPR_OUT_OF_HANDLE             ((INT32)   -1)    /**<   return out of handle     */
#define MTPR_OUT_OF_MEMORY             ((INT32)   -2)    /**<   return out of memory     */
#define MTPR_INV_ARG                   ((INT32)   -3)    /**<   return invalid argument     */
#define MTPR_DEVICE_ERROR              ((INT32)   -4)    /**<   return device error     */
#define MTPR_NOT_SUPPORT               ((INT32)   -5)    /**<   return not support     */
#define MTPR_BUSY                      ((INT32)   -6)    /**<   return busy     */
#define MTPR_NOT_INIT                  ((INT32)   -7)    /**<   return not init     */
#define MTPR_NOT_CONNECT               ((INT32)   -8)    /**<   return not connect     */
#define MTPR_ALREADY_CONNECT           ((INT32)   -9)    /**<   return already connect    */
#define MTPR_NO_SUCH_DEVICE            ((INT32)   -10)    /**<  return no such device      */
#define MTPR_EXIST                     ((INT32)   -11)    /**<  return exist      */
#define MTPR_CORE                      ((INT32)   -12)    /**<  return core error      */
#define MTPR_NOT_IMPLEMENT             ((INT32)   -13)    /**<  return not implement      */
#define MTPR_CLI_ERROR                 ((INT32)   -14)    /**<  return CLI error      */
#define MTPR_ALIGNMENT_ERROR           ((INT32)   -15)    /**<  return alignment error      */
#define MTPR_USB_ERROR                 ((INT32)   -16)    /**<  return USB error      */
#define MTPR_NOT_IN_DATA_PHASE         ((INT32)   -17)    /**<  return not in data phase      */
#define MTPR_NOT_IN_READY_PHASE        ((INT32)   -18)    /**<  return not in ready phase      */
#define MTPR_RESET_FAIL                ((INT32)   -19)    /**<  return reset fail      */
#define MTPR_MEDIA_REMOVED             ((INT32)   -20)    /**<  return media removed      */
#define MTPR_OP_NOT_SUPPORT            ((INT32)   -21)    /**<  return operation not support      */
#define MTPR_SESSION_NOT_OPEN          ((INT32)   -22)    /**<  return session not open      */
#define MTPR_SESSION_ALREADY_OPEN      ((INT32)   -23)    /**<  return session already open      */
#define MTPR_UNKNOWN                   ((INT32)   -24)    /**<  return unknown      */
#define MTPR_UNEXPECTED_REQUEST        ((INT32)   -25)    /**<  return unexpected request      */
#define MTPR_PARAMETER_ERROR           ((INT32)   -26)    /**<  return parameter error      */

/*------------------------------------------------------------------*/
/*! @enum MTP_ASYNC_COND_T
 *  @brief  This enumeration specifies MTP asynchronous condition code. 
 *  @code
 *  typedef enum
 *  {
 *      MTP_ASYNC_COND_OPERATION_OK,
 *      MTP_ASYNC_COND_OPERATION_CONTINUE,
 *      MTP_ASYNC_COND_OPERATION_ERROR,
 *      MTP_ASYNC_COND_OUT_OF_MEMORY,
 *      MTP_ASYNC_COND_INVALID,
 *      MTP_ASYNC_COND_ABORT_FAIL,
 *      MTP_ASYNC_COND_ABORT_OK,
 *      MTP_ASYNC_COND_RESET_SUCCESS,
 *      MTP_ASYNC_COND_RESET_FAIL,
 *      MTP_ASYNC_COND_SHUTDOWN
 *  } MTP_ASYNC_COND_T;
 *  @endcode
 *  @li@c  MTP_ASYNC_COND_OPERATION_OK              -Specify condition of operation is ok.
 *  @li@c  MTP_ASYNC_COND_OPERATION_CONTINUE   -Specify condition of operation continue.
 *  @li@c  MTP_ASYNC_COND_OPERATION_ERROR        -Specify condition of operation is error.
 *  @li@c  MTP_ASYNC_COND_OUT_OF_MEMORY           -Specify condition of out of memory
 *  @li@c  MTP_ASYNC_COND_INVALID                        -Specify condition of invalid.
 *  @li@c  MTP_ASYNC_COND_ABORT_FAIL                  -Specify condition of abort fail.
 *  @li@c  MTP_ASYNC_COND_ABORT_OK                     -Specify condition of abort ok.
 *  @li@c  MTP_ASYNC_COND_RESET_SUCCESS            -Specify condition of reset success.
 *  @li@c  MTP_ASYNC_COND_RESET_FAIL                   -Specify condition of reset fail.
 *  @li@c  MTP_ASYNC_COND_SHUTDOWN                   -Specify condition of shut down.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MTP_ASYNC_COND_OPERATION_OK,
    MTP_ASYNC_COND_OPERATION_CONTINUE,
    MTP_ASYNC_COND_OPERATION_ERROR,
    MTP_ASYNC_COND_OUT_OF_MEMORY,
    MTP_ASYNC_COND_INVALID,
    MTP_ASYNC_COND_ABORT_FAIL,
    MTP_ASYNC_COND_ABORT_OK,
    MTP_ASYNC_COND_RESET_SUCCESS,
    MTP_ASYNC_COND_RESET_FAIL,
    MTP_ASYNC_COND_SHUTDOWN
} MTP_ASYNC_COND_T;

/** Values for OPCODE_T DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _OPCODE
 *  @brief      This enumeration specifies MTP operation code.
 *  @code
 *  enum _OPCODE
 *  {       
 *      OPCODE_UNDEFINED                 = 0x1000,
 *      OPCODE_GETDEVICEINFO             = 0x1001,
 *      OPCODE_OPENSESSION               = 0x1002,
 *      OPCODE_CLOSESESSION              = 0x1003,
 *      OPCODE_GETSTORAGEIDS             = 0x1004,
 *      OPCODE_GETSTORAGEINFO            = 0x1005,
 *      OPCODE_GETNUMOBJECTS             = 0x1006,
 *      OPCODE_GETOBJECTHANDLES          = 0x1007,
 *      OPCODE_GETOBJECTINFO             = 0x1008,
 *      OPCODE_GETOBJECT                 = 0x1009,
 *      OPCODE_GETTHUMB                  = 0x100a,
 *      OPCODE_DELETEOBJECT              = 0x100b,
 *      OPCODE_SENDOBJECTINFO            = 0x100c,
 *      OPCODE_SENDOBJECT                = 0x100d,
 *      OPCODE_INITIATECAPTURE           = 0x100e,
 *      OPCODE_FORMATSTORE               = 0x100f,
 *      OPCODE_RESETDEVICE               = 0x1010,
 *      OPCODE_SELFTEST                  = 0x1011,
 *      OPCODE_SETOBJECTPROTECTION       = 0x1012,
 *      OPCODE_POWERDOWN                 = 0x1013,
 *      OPCODE_GETDEVICEPROPDESC         = 0x1014,
 *      OPCODE_GETDEVICEPROPVALUE        = 0x1015,
 *      OPCODE_SETDEVICEPROPVALUE        = 0x1016,
 *      OPCODE_RESETDEVICEPROPVALUE      = 0x1017,
 *      OPCODE_TERMINATECAPTURE          = 0x1018,
 *      OPCODE_MOVEOBJECT                = 0x1019,
 *      OPCODE_COPYOBJECT                = 0x101a,
 *      OPCODE_GETPARTIALOBJECT          = 0x101b,
 *      OPCODE_INITIATEOPENCAPTURE       = 0x101c,
 *      OPCODE_GETOBJECTPROPSSUPPORTED   = 0x9801,
 *      OPCODE_GETOBJECTPROPDESC         = 0x9802,
 *      OPCODE_GETOBJECTPROPVALUE        = 0x9803,
 *      OPCODE_SETOBJECTPROPVALUE        = 0x9804,
 *      OPCODE_GETOBJECTPROPLIST         = 0x9805,
 *      OPCODE_SETOBJECTPROPLIST         = 0x9806,
 *      OPCODE_GETINTERDEPENDENTPROPDESC = 0x9807,
 *      OPCODE_SENDOBJECTPROPLIST        = 0x9808,
 *      OPCODE_GETOBJECTREFERENCES       = 0x9810,
 *      OPCODE_SETOBJECTREFERENCES       = 0x9811,
 *      OPCODE_UPDATEDEVICEFIRMWARE      = 0x9812,
 *      OPCODE_RESETOBJECTPROPVALUE      = 0x9813,
 *      OPCODE_RESERVED_FIRST            = 0x1026,
 *      OPCODE_RESERVED_LAST             = 0x1fff,
 *      OPCODE_VENDOREXTENSION_FIRST     = 0x9000,
 *      OPCODE_PLAYBACK_SKIP             = 0x9820,
 *      OPCODE_VENDOREXTENSION_LAST      = 0x9fff
 *  };
 *  @endcode
 *  @li@c  OPCODE_UNDEFINED               -Specifies MTP operation code of undefined.
 *  @li@c  OPCODE_GETDEVICEINFO        -Specifies MTP operation code of get device info.
 *  @li@c  OPCODE_OPENSESSION           -Specifies MTP operation code of open session.
 *  @li@c  OPCODE_CLOSESESSION         -Specifies MTP operation code of close session.
 *  @li@c  OPCODE_GETSTORAGEIDS       -Specifies MTP operation code of get storage id.
 *  @li@c  OPCODE_GETSTORAGEINFO     -Specifies MTP operation code of get storage info.
 *  @li@c  OPCODE_GETNUMOBJECTS       -Specifies MTP operation code of get number of objects.
 *  @li@c  OPCODE_GETOBJECTHANDLES   -Specifies MTP operation code of get object handles.
 *  @li@c  OPCODE_GETOBJECTINFO    -Specifies MTP operation code of get object info.
 *  @li@c  OPCODE_GETOBJECT           -Specifies MTP operation code of get object.
 *  @li@c  OPCODE_GETTHUMB            -Specifies MTP operation code of get thumb.
 *  @li@c  OPCODE_DELETEOBJECT      -Specifies MTP operation code of delete object.
 *  @li@c  OPCODE_SENDOBJECTINFO   -Specifies MTP operation code of send object info.
 *  @li@c  OPCODE_SENDOBJECT          -Specifies MTP operation code of send object.
 *  @li@c  OPCODE_INITIATECAPTURE  -Specifies MTP operation code of init capture.
 *  @li@c  OPCODE_FORMATSTORE       -Specifies MTP operation code of format store.
 *  @li@c  OPCODE_RESETDEVICE        -Specifies MTP operation code of reset device.
 *  @li@c  OPCODE_SELFTEST               -Specifies MTP operation code of self test.
 *  @li@c  OPCODE_SETOBJECTPROTECTION   -Specifies MTP operation code of set object protection.
 *  @li@c  OPCODE_POWERDOWN           -Specifies MTP operation code of power down.
 *  @li@c  OPCODE_GETDEVICEPROPDESC  -Specifies MTP operation code of get device prop description.
 *  @li@c  OPCODE_GETDEVICEPROPVALUE    -Specifies MTP operation code of get device prop value.
 *  @li@c  OPCODE_SETDEVICEPROPVALUE     -Specifies MTP operation code of set device prop value.
 *  @li@c  OPCODE_RESETDEVICEPROPVALUE   -Specifies MTP operation code of reset device prop value.
 *  @li@c  OPCODE_TERMINATECAPTURE    -Specifies MTP operation code of terminate capture.
 *  @li@c  OPCODE_MOVEOBJECT       -Specifies MTP operation code of move object.
 *  @li@c  OPCODE_COPYOBJECT       -Specifies MTP operation code of copy object.
 *  @li@c  OPCODE_GETPARTIALOBJECT    -Specifies MTP operation code of get partial object.
 *  @li@c  OPCODE_INITIATEOPENCAPTURE   -Specifies MTP operation code of init open capture.
 *  @li@c  OPCODE_GETOBJECTPROPSSUPPORTED   -Specifies MTP operation code of get object prop supported.
 *  @li@c  OPCODE_GETOBJECTPROPDESC    -Specifies MTP operation code of get object prop description.
 *  @li@c  OPCODE_GETOBJECTPROPVALUE    -Specifies MTP operation code of get object prop value.
 *  @li@c  OPCODE_SETOBJECTPROPVALUE   -Specifies MTP operation code of set object prop value.
 *  @li@c  OPCODE_GETOBJECTPROPLIST      -Specifies MTP operation code of get object prop list.
 *  @li@c  OPCODE_SETOBJECTPROPLIST      -Specifies MTP operation code of set object prop list.
 *  @li@c  OPCODE_GETINTERDEPENDENTPROPDESC  -Specifies MTP operation code of get interdependent prop description.
 *  @li@c  OPCODE_SENDOBJECTPROPLIST     -Specifies MTP operation code of send object prop list.
 *  @li@c  OPCODE_GETOBJECTREFERENCES    -Specifies MTP operation code of get object references.
 *  @li@c  OPCODE_SETOBJECTREFERENCES     -Specifies MTP operation code of set object references.
 *  @li@c  OPCODE_UPDATEDEVICEFIRMWARE   -Specifies MTP operation code of update device firmware.
 *  @li@c  OPCODE_RESETOBJECTPROPVALUE    -Specifies MTP operation code of reset object prop value.
 *  @li@c  OPCODE_RESERVED_FIRST     -Specifies MTP operation code of reserved first.
 *  @li@c  OPCODE_RESERVED_LAST      -Specifies MTP operation code of reserved last.
 *  @li@c  OPCODE_VENDOREXTENSION_FIRST   -Specifies MTP operation code of vendor extension first.
 *  @li@c  OPCODE_PLAYBACK_SKIP        -Specifies MTP operation code of play back skip.
 *  @li@c  OPCODE_VENDOREXTENSION_LAST    -Specifies MTP operation code of vendor extension last.
 */
/*------------------------------------------------------------------*/
enum _OPCODE
{       
    OPCODE_UNDEFINED                 = 0x1000,
    OPCODE_GETDEVICEINFO             = 0x1001,
    OPCODE_OPENSESSION               = 0x1002,
    OPCODE_CLOSESESSION              = 0x1003,
    OPCODE_GETSTORAGEIDS             = 0x1004,
    OPCODE_GETSTORAGEINFO            = 0x1005,
    OPCODE_GETNUMOBJECTS             = 0x1006,
    OPCODE_GETOBJECTHANDLES          = 0x1007,
    OPCODE_GETOBJECTINFO             = 0x1008,
    OPCODE_GETOBJECT                 = 0x1009,
    OPCODE_GETTHUMB                  = 0x100a,
    OPCODE_DELETEOBJECT              = 0x100b,
    OPCODE_SENDOBJECTINFO            = 0x100c,
    OPCODE_SENDOBJECT                = 0x100d,
    OPCODE_INITIATECAPTURE           = 0x100e,
    OPCODE_FORMATSTORE               = 0x100f,
    OPCODE_RESETDEVICE               = 0x1010,
    OPCODE_SELFTEST                  = 0x1011,
    OPCODE_SETOBJECTPROTECTION       = 0x1012,
    OPCODE_POWERDOWN                 = 0x1013,
    OPCODE_GETDEVICEPROPDESC         = 0x1014,
    OPCODE_GETDEVICEPROPVALUE        = 0x1015,
    OPCODE_SETDEVICEPROPVALUE        = 0x1016,
    OPCODE_RESETDEVICEPROPVALUE      = 0x1017,
    OPCODE_TERMINATECAPTURE          = 0x1018,
    OPCODE_MOVEOBJECT                = 0x1019,
    OPCODE_COPYOBJECT                = 0x101a,
    OPCODE_GETPARTIALOBJECT          = 0x101b,
    OPCODE_INITIATEOPENCAPTURE       = 0x101c,
    OPCODE_GETOBJECTPROPSSUPPORTED   = 0x9801,
    OPCODE_GETOBJECTPROPDESC         = 0x9802,
    OPCODE_GETOBJECTPROPVALUE        = 0x9803,
    OPCODE_SETOBJECTPROPVALUE        = 0x9804,
    OPCODE_GETOBJECTPROPLIST         = 0x9805,
    OPCODE_SETOBJECTPROPLIST         = 0x9806,
    OPCODE_GETINTERDEPENDENTPROPDESC = 0x9807,
    OPCODE_SENDOBJECTPROPLIST        = 0x9808,
    OPCODE_GETOBJECTREFERENCES       = 0x9810,
    OPCODE_SETOBJECTREFERENCES       = 0x9811,
    OPCODE_UPDATEDEVICEFIRMWARE      = 0x9812,
    OPCODE_RESETOBJECTPROPVALUE      = 0x9813,
    OPCODE_RESERVED_FIRST            = 0x1026,
    OPCODE_RESERVED_LAST             = 0x1fff,
    OPCODE_VENDOREXTENSION_FIRST     = 0x9000,
    OPCODE_PLAYBACK_SKIP             = 0x9820,
    OPCODE_VENDOREXTENSION_LAST      = 0x9fff
};

/** Values for RESPONSECODE_T DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _RESPONSECODE
 *  @brief  This enumeration specifies the MTP response code.
 *  @code
 *  enum _RESPONSECODE
 *  {   
 *      RESPONSECODE_UNDEFINED                  = 0x2000,
 *      RESPONSECODE_OK                         = 0x2001,
 *      RESPONSECODE_GENERALERROR               = 0x2002,
 *      RESPONSECODE_SESSIONNOTOPEN             = 0x2003,
 *      RESPONSECODE_INVALIDTRANSACTIONID       = 0x2004,
 *      RESPONSECODE_OPERATIONNOTSUPPORTED      = 0x2005,
 *      RESPONSECODE_PARAMETERNOTSUPPORTED      = 0x2006,
 *      RESPONSECODE_INCOMPLETETRANSFER         = 0x2007,
 *      RESPONSECODE_INVALIDSTORAGEID           = 0x2008,
 *      RESPONSECODE_INVALIDOBJECTHANDLE        = 0x2009,
 *      RESPONSECODE_INVALIDPROPERTYCODE        = 0x200a,
 *      RESPONSECODE_INVALIDOBJECTFORMATCODE    = 0x200b,
 *      RESPONSECODE_STOREFULL                  = 0x200c,
 *      RESPONSECODE_OBJECTWRITEPROTECTED       = 0x200d,
 *      RESPONSECODE_STOREWRITEPROTECTED        = 0x200e,
 *      RESPONSECODE_ACCESSDENIED               = 0x200f,
 *      RESPONSECODE_NOTHUMBNAILPRESENT         = 0x2010,
 *      RESPONSECODE_SELFTESTFAILED             = 0x2011,
 *      RESPONSECODE_PARTIALDELETION            = 0x2012,
 *      RESPONSECODE_STORENOTAVAILABLE          = 0x2013,
 *      RESPONSECODE_NOSPECIFICATIONBYFORMAT    = 0x2014,
 *      RESPONSECODE_NOVALIDOBJECTINFO          = 0x2015,
 *      RESPONSECODE_INVALIDCODEFORMAT          = 0x2016,
 *      RESPONSECODE_UNKNOWNVENDORCODE          = 0x2017,
 *      RESPONSECODE_CAPTUREALREADYTERMINATED   = 0x2018,
 *      RESPONSECODE_DEVICEBUSY                 = 0x2019,
 *      RESPONSECODE_INVALIDPARENT              = 0x201a,
 *      RESPONSECODE_INVALIDPROPFORMAT          = 0x201b,
 *      RESPONSECODE_INVALIDPROPVALUE           = 0x201c,
 *      RESPONSECODE_INVALIDPARAMETER           = 0x201d,
 *      RESPONSECODE_SESSIONALREADYOPENED       = 0x201e,
 *      RESPONSECODE_TRANSACTIONCANCELLED       = 0x201f,
 *      RESPONSECODE_DESTINATIONUNSUPPORTED     = 0x2020,
 *      RESPONSECODE_RESERVED_FIRST             = 0x2021,
 *      RESPONSECODE_RESERVED_LAST              = 0x2fff,
 *      RESPONSECODE_VENDOREXTENSION_FIRST      = 0xa000,
 *      RESPONSECODE_VENDOREXTENSION_LAST       = 0xa7ff,
 *      RESPONSECODE_INVALIDOBJECTPROPCODE      = 0xa801,
 *      RESPONSECODE_INVALIDOBJECTPROPFORMAT    = 0xa802,
 *      RESPONSECODE_INVALIDOBJECTPROPVALUE     = 0xa803,
 *      RESPONSECODE_INVALIDOBJECTREFERENCE     = 0xa804,
 *      RESPONSECODE_INVALIDOBJECTGROUPCODE     = 0xa805,
 *      RESPONSECODE_INVALIDDATASET             = 0xa806,
 *      RESPONSECODE_NOSPECIFICATIONBYGROUP	    = 0xa807,
 *      RESPONSECODE_NOSPECIFICATIONBYDEPTH	    = 0xa808,
 *      RESPONSECODE_OBJECTTOOLARGE             = 0xa809
 *  };
 *  @endcode
 *  @li@c  RESPONSECODE_UNDEFINED     -Specifies the MTP response code of undefined.
 *  @li@c  RESPONSECODE_OK   -Specifies the MTP response code of ok.
 *  @li@c  RESPONSECODE_GENERALERROR  -Specifies the MTP response code of general error.
 *  @li@c  RESPONSECODE_SESSIONNOTOPEN   -Specifies the MTP response code of session not open.
 *  @li@c  RESPONSECODE_INVALIDTRANSACTIONID  -Specifies the MTP response code of invalid transaction id.
 *  @li@c  RESPONSECODE_OPERATIONNOTSUPPORTED  -Specifies the MTP response code of operation not supported.
 *  @li@c  RESPONSECODE_PARAMETERNOTSUPPORTED  -Specifies the MTP response code of parameter not supported.
 *  @li@c  RESPONSECODE_INCOMPLETETRANSFER  -Specifies the MTP response code of incomplete transfer.
 *  @li@c  RESPONSECODE_INVALIDSTORAGEID  -Specifies the MTP response code of invalid storage id.
 *  @li@c  RESPONSECODE_INVALIDOBJECTHANDLE   -Specifies the MTP response code of invalid object handle.
 *  @li@c  RESPONSECODE_INVALIDPROPERTYCODE   -Specifies the MTP response code of invalid property code.
 *  @li@c  RESPONSECODE_INVALIDOBJECTFORMATCODE  -Specifies the MTP response code of invalid object format code.
 *  @li@c  RESPONSECODE_STOREFULL    -Specifies the MTP response code of store full.
 *  @li@c  RESPONSECODE_OBJECTWRITEPROTECTED -Specifies the MTP response code of object write protected.
 *  @li@c  RESPONSECODE_STOREWRITEPROTECTED  -Specifies the MTP response code of store write protected.
 *  @li@c  RESPONSECODE_ACCESSDENIED    -Specifies the MTP response code of access denied.
 *  @li@c  RESPONSECODE_NOTHUMBNAILPRESENT   -Specifies the MTP response code of no thumbnail present.
 *  @li@c  RESPONSECODE_SELFTESTFAILED  -Specifies the MTP response code of self test failed.
 *  @li@c  RESPONSECODE_PARTIALDELETION    -Specifies the MTP response code of partial deletion.
 *  @li@c  RESPONSECODE_STORENOTAVAILABLE   -Specifies the MTP response code of store not available.
 *  @li@c  RESPONSECODE_NOSPECIFICATIONBYFORMAT   -Specifies the MTP response code of no specification by format.
 *  @li@c  RESPONSECODE_NOVALIDOBJECTINFO   -Specifies the MTP response code of no valid object info.
 *  @li@c  RESPONSECODE_INVALIDCODEFORMAT  -Specifies the MTP response code of invalid code format.
 *  @li@c  RESPONSECODE_UNKNOWNVENDORCODE    -Specifies the MTP response code of unknown vendor code.
 *  @li@c  RESPONSECODE_CAPTUREALREADYTERMINATED  -Specifies the MTP response code of capture already terminated.
 *  @li@c  RESPONSECODE_DEVICEBUSY    -Specifies the MTP response code of device busy.
 *  @li@c  RESPONSECODE_INVALIDPARENT    -Specifies the MTP response code of invalid parent.
 *  @li@c  RESPONSECODE_INVALIDPROPFORMAT   -Specifies the MTP response code of invalid prop format.
 *  @li@c  RESPONSECODE_INVALIDPROPVALUE    -Specifies the MTP response code of invalid prop value.
 *  @li@c  RESPONSECODE_INVALIDPARAMETER    -Specifies the MTP response code of invalid parameter.
 *  @li@c  RESPONSECODE_SESSIONALREADYOPENED   -Specifies the MTP response code of session already opened.
 *  @li@c  RESPONSECODE_TRANSACTIONCANCELLED   -Specifies the MTP response code of transaction cancelled.
 *  @li@c  RESPONSECODE_DESTINATIONUNSUPPORTED   -Specifies the MTP response code of destination unsupported.
 *  @li@c  RESPONSECODE_RESERVED_FIRST  -Specifies the MTP response code of reserved first.
 *  @li@c  RESPONSECODE_RESERVED_LAST   -Specifies the MTP response code of reserved last.
 *  @li@c  RESPONSECODE_VENDOREXTENSION_FIRST  -Specifies the MTP response code of vendor extension first.
 *  @li@c  RESPONSECODE_VENDOREXTENSION_LAST    -Specifies the MTP response code of vendor extension last.
 *  @li@c  RESPONSECODE_INVALIDOBJECTPROPCODE   -Specifies the MTP response code of invalid object prop code.
 *  @li@c  RESPONSECODE_INVALIDOBJECTPROPFORMAT -Specifies the MTP response code of invalid object prop format.
 *  @li@c  RESPONSECODE_INVALIDOBJECTPROPVALUE   -Specifies the MTP response code of invalid object prop value.
 *  @li@c  RESPONSECODE_INVALIDOBJECTREFERENCE   -Specifies the MTP response code of invalid object reference.
 *  @li@c  RESPONSECODE_INVALIDOBJECTGROUPCODE  -Specifies the MTP response code of invalid object group code.
 *  @li@c  RESPONSECODE_INVALIDDATASET     -Specifies the MTP response code of invalid dataset.
 *  @li@c  RESPONSECODE_NOSPECIFICATIONBYGROUP   -Specifies the MTP response code of no specification by group.
 *  @li@c  RESPONSECODE_NOSPECIFICATIONBYDEPTH   -Specifies the MTP response code of no specification by depth.
 *  @li@c  RESPONSECODE_OBJECTTOOLARGE    -Specifies the MTP response code of object too large.
 */
/*------------------------------------------------------------------*/
enum _RESPONSECODE
{   
    RESPONSECODE_UNDEFINED                  = 0x2000,
    RESPONSECODE_OK                         = 0x2001,
    RESPONSECODE_GENERALERROR               = 0x2002,
    RESPONSECODE_SESSIONNOTOPEN             = 0x2003,
    RESPONSECODE_INVALIDTRANSACTIONID       = 0x2004,
    RESPONSECODE_OPERATIONNOTSUPPORTED      = 0x2005,
    RESPONSECODE_PARAMETERNOTSUPPORTED      = 0x2006,
    RESPONSECODE_INCOMPLETETRANSFER         = 0x2007,
    RESPONSECODE_INVALIDSTORAGEID           = 0x2008,
    RESPONSECODE_INVALIDOBJECTHANDLE        = 0x2009,
    RESPONSECODE_INVALIDPROPERTYCODE        = 0x200a,
    RESPONSECODE_INVALIDOBJECTFORMATCODE    = 0x200b,
    RESPONSECODE_STOREFULL                  = 0x200c,
    RESPONSECODE_OBJECTWRITEPROTECTED       = 0x200d,
    RESPONSECODE_STOREWRITEPROTECTED        = 0x200e,
    RESPONSECODE_ACCESSDENIED               = 0x200f,
    RESPONSECODE_NOTHUMBNAILPRESENT         = 0x2010,
    RESPONSECODE_SELFTESTFAILED             = 0x2011,
    RESPONSECODE_PARTIALDELETION            = 0x2012,
    RESPONSECODE_STORENOTAVAILABLE          = 0x2013,
    RESPONSECODE_NOSPECIFICATIONBYFORMAT    = 0x2014,
    RESPONSECODE_NOVALIDOBJECTINFO          = 0x2015,
    RESPONSECODE_INVALIDCODEFORMAT          = 0x2016,
    RESPONSECODE_UNKNOWNVENDORCODE          = 0x2017,
    RESPONSECODE_CAPTUREALREADYTERMINATED   = 0x2018,
    RESPONSECODE_DEVICEBUSY                 = 0x2019,
    RESPONSECODE_INVALIDPARENT              = 0x201a,
    RESPONSECODE_INVALIDPROPFORMAT          = 0x201b,
    RESPONSECODE_INVALIDPROPVALUE           = 0x201c,
    RESPONSECODE_INVALIDPARAMETER           = 0x201d,
    RESPONSECODE_SESSIONALREADYOPENED       = 0x201e,
    RESPONSECODE_TRANSACTIONCANCELLED       = 0x201f,
    RESPONSECODE_DESTINATIONUNSUPPORTED     = 0x2020,
    RESPONSECODE_RESERVED_FIRST             = 0x2021,
    RESPONSECODE_RESERVED_LAST              = 0x2fff,
    RESPONSECODE_VENDOREXTENSION_FIRST      = 0xa000,
    RESPONSECODE_VENDOREXTENSION_LAST       = 0xa7ff,
    RESPONSECODE_INVALIDOBJECTPROPCODE      = 0xa801,
    RESPONSECODE_INVALIDOBJECTPROPFORMAT    = 0xa802,
    RESPONSECODE_INVALIDOBJECTPROPVALUE     = 0xa803,
    RESPONSECODE_INVALIDOBJECTREFERENCE     = 0xa804,
    RESPONSECODE_INVALIDOBJECTGROUPCODE     = 0xa805,
    RESPONSECODE_INVALIDDATASET             = 0xa806,
    RESPONSECODE_NOSPECIFICATIONBYGROUP	    = 0xa807,
    RESPONSECODE_NOSPECIFICATIONBYDEPTH	    = 0xa808,
    RESPONSECODE_OBJECTTOOLARGE             = 0xa809
};

/** Values for EVENTCODE DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _EVENTCODE
 *  @brief  This enumeration specifies the MTP device event code.
 *  @code
 *  enum _EVENTCODE
 *  {
 *      EVENTCODE_UNDEFINED             = 0x4000,
 *      EVENTCODE_CANCELTRANSACTION     = 0x4001,
 *      EVENTCODE_OBJECTADDED           = 0x4002,
 *      EVENTCODE_OBJECTREMOVED         = 0x4003,
 *      EVENTCODE_STOREADDED            = 0x4004,
 *      EVENTCODE_STOREREMOVED          = 0x4005,
 *      EVENTCODE_DEVICEPROPCHANGED     = 0x4006,
 *      EVENTCODE_OBJECTINFOCHANGED     = 0x4007,
 *      EVENTCODE_DEVICEINFOCHANGED     = 0x4008,
 *      EVENTCODE_REQUESTOBJECTTRANSFER = 0x4009,
 *      EVENTCODE_STOREFULL             = 0x400a,
 *      EVENTCODE_DEVICERESET           = 0x400b,
 *      EVENTCODE_STORAGEINFOCHANGED    = 0x400c,
 *      EVENTCODE_CAPTURECOMPLETE       = 0x400d,
 *      EVENTCODE_UNREPORTEDSTATUS      = 0x400e,
 *      EVENTCODE_RESERVED_FIRST        = 0x400f,
 *      EVENTCODE_RESERVED_LAST         = 0x4fff,
 *      EVENTCODE_VENDOREXTENSION_FIRST = 0xc000,
 *      EVENTCODE_VENDOREXTENSION_LAST  = 0xcfff
 *  };
 *  @endcode
 *  @li@c  EVENTCODE_UNDEFINED     -Specifies the MTP event code of undefined.
 *  @li@c  EVENTCODE_CANCELTRANSACTION  -Specifies the MTP event code of cancel transaction.
 *  @li@c  EVENTCODE_OBJECTADDED   -Specifies the MTP event code of object added.
 *  @li@c  EVENTCODE_OBJECTREMOVED  -Specifies the MTP event code of object removed.
 *  @li@c  EVENTCODE_STOREADDED    -Specifies the MTP event code of store added.
 *  @li@c  EVENTCODE_STOREREMOVED   -Specifies the MTP event code of store removed.
 *  @li@c  EVENTCODE_DEVICEPROPCHANGED  -Specifies the MTP event code of device prop changed.
 *  @li@c  EVENTCODE_OBJECTINFOCHANGED  -Specifies the MTP event code of object info changed.
 *  @li@c  EVENTCODE_DEVICEINFOCHANGED  -Specifies the MTP event code of device info changed.
 *  @li@c  EVENTCODE_REQUESTOBJECTTRANSFER  -Specifies the MTP event code of request object transfer.
 *  @li@c  EVENTCODE_STOREFULL   -Specifies the MTP event code of store full.
 *  @li@c  EVENTCODE_DEVICERESET   -Specifies the MTP event code of device reset.
 *  @li@c  EVENTCODE_STORAGEINFOCHANGED   -Specifies the MTP event code of storage information changed.
 *  @li@c  EVENTCODE_CAPTURECOMPLETE  -Specifies the MTP event code of capture complete.
 *  @li@c  EVENTCODE_UNREPORTEDSTATUS   -Specifies the MTP event code of unreported status.
 *  @li@c  EVENTCODE_RESERVED_FIRST  -Specifies the MTP event code of reserved first.
 *  @li@c  EVENTCODE_RESERVED_LAST   -Specifies the MTP event code of reserved last.
 *  @li@c  EVENTCODE_VENDOREXTENSION_FIRST   -Specifies the MTP event code of vendor extension first.
 *  @li@c  EVENTCODE_VENDOREXTENSION_LAST   -Specifies the MTP event code of vendor extension last.
 */
/*------------------------------------------------------------------*/
enum _EVENTCODE
{
    EVENTCODE_UNDEFINED             = 0x4000,
    EVENTCODE_CANCELTRANSACTION     = 0x4001,
    EVENTCODE_OBJECTADDED           = 0x4002,
    EVENTCODE_OBJECTREMOVED         = 0x4003,
    EVENTCODE_STOREADDED            = 0x4004,
    EVENTCODE_STOREREMOVED          = 0x4005,
    EVENTCODE_DEVICEPROPCHANGED     = 0x4006,
    EVENTCODE_OBJECTINFOCHANGED     = 0x4007,
    EVENTCODE_DEVICEINFOCHANGED     = 0x4008,
    EVENTCODE_REQUESTOBJECTTRANSFER = 0x4009,
    EVENTCODE_STOREFULL             = 0x400a,
    EVENTCODE_DEVICERESET           = 0x400b,
    EVENTCODE_STORAGEINFOCHANGED    = 0x400c,
    EVENTCODE_CAPTURECOMPLETE       = 0x400d,
    EVENTCODE_UNREPORTEDSTATUS      = 0x400e,
    EVENTCODE_RESERVED_FIRST        = 0x400f,
    EVENTCODE_RESERVED_LAST         = 0x4fff,
    EVENTCODE_VENDOREXTENSION_FIRST = 0xc000,
    EVENTCODE_VENDOREXTENSION_LAST  = 0xcfff
};

/** Values for FUNCTIONALMODE DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _FUNCTIONALMODE
 *  @brief  This enumeration specifies the MTP device functional mode.
 *  @code
 *  enum _FUNCTIONALMODE
 *  {
 *      FUNCTIONALMODE_STANDARD              = 0x0000,
 *      FUNCTIONALMODE_SLEEP                 = 0x0001,
 *      FUNCTIONALMODE_RESERVED_FIRST        = 0x0002,
 *      FUNCTIONALMODE_RESERVED_LAST         = 0x7FFF,
 *      FUNCTIONALMODE_VENDOREXTENSION_FIRST = 0x8000,
 *      FUNCTIONALMODE_VENDOREXTENSION_LAST  = 0xFFFF
 *  };
 *  @endcode
 *  @li@c  FUNCTIONALMODE_STANDARD   -Specifies the MTP functional mode of standard.
 *  @li@c  FUNCTIONALMODE_SLEEP    -Specifies the MTP functional mode of sleep.
 *  @li@c  FUNCTIONALMODE_RESERVED_FIRST   -Specifies the MTP functional mode of reserved first.
 *  @li@c  FUNCTIONALMODE_RESERVED_LAST    -Specifies the MTP functional mode of reserved last.
 *  @li@c  FUNCTIONALMODE_VENDOREXTENSION_FIRST  -Specifies the MTP functional mode of vendor extension first.
 *  @li@c  FUNCTIONALMODE_VENDOREXTENSION_LAST  -Specifies the MTP functional mode of vendor extension last.
 */
/*------------------------------------------------------------------*/
enum _FUNCTIONALMODE
{
    FUNCTIONALMODE_STANDARD              = 0x0000,
    FUNCTIONALMODE_SLEEP                 = 0x0001,
    FUNCTIONALMODE_RESERVED_FIRST        = 0x0002,
    FUNCTIONALMODE_RESERVED_LAST         = 0x7FFF,
    FUNCTIONALMODE_VENDOREXTENSION_FIRST = 0x8000,
    FUNCTIONALMODE_VENDOREXTENSION_LAST  = 0xFFFF
};

/** Values for ::DEVICEPROPCODE DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _DEVICEPROPCODE
 *  @brief  This enumeration specifies the MTP device property code.
 *  @code
 *  enum _DEVICEPROPCODE
 *  {
 *      DEVICEPROPCODE_NOTUSED                       = 0x0,
 *      DEVICEPROPCODE_ALL                           = 0xFFFF,
 *      DEVICEPROPCODE_UNDEFINED                     = 0x5000,
 *      DEVICEPROPCODE_BATTERYLEVEL                  = 0x5001,
 *      DEVICEPROPCODE_FUNCTIONMODE                  = 0x5002,
 *      DEVICEPROPCODE_IMAGESIZE                     = 0x5003,
 *      DEVICEPROPCODE_COMPRESSIONSETTING            = 0x5004,
 *      DEVICEPROPCODE_WHITEBALANCE                  = 0x5005,
 *      DEVICEPROPCODE_RGBGAIN                       = 0x5006,
 *      DEVICEPROPCODE_FNUMBER                       = 0x5007,
 *      DEVICEPROPCODE_FOCALLENGTH                   = 0x5008,
 *      DEVICEPROPCODE_FOCUSDISTANCE                 = 0x5009,
 *      DEVICEPROPCODE_FOCUSMODE                     = 0x500A,
 *      DEVICEPROPCODE_EXPOSUREMETERINGMODE          = 0x500B,
 *      DEVICEPROPCODE_FLASHMODE                     = 0x500C,
 *      DEVICEPROPCODE_EXPOSURETIME                  = 0x500D,
 *      DEVICEPROPCODE_EXPOSUREPROGRAMMODE           = 0x500E,
 *      DEVICEPROPCODE_EXPOSUREINDEX                 = 0x500F,
 *      DEVICEPROPCODE_EXPOSURECOMPENSATION          = 0x5010,
 *      DEVICEPROPCODE_DATETIME                      = 0x5011,
 *      DEVICEPROPCODE_CAPTUREDELAY                  = 0x5012,
 *      DEVICEPROPCODE_STILLCAPTUREMODE              = 0x5013,
 *      DEVICEPROPCODE_CONTRAST                      = 0x5014,
 *      DEVICEPROPCODE_SHARPNESS                     = 0x5015,
 *      DEVICEPROPCODE_DIGITALZOOM                   = 0x5016,
 *      DEVICEPROPCODE_EFFECTMODE                    = 0x5017,
 *      DEVICEPROPCODE_BURSTNUMBER                   = 0x5018,
 *      DEVICEPROPCODE_BURSTINTERVAL                 = 0x5019,
 *      DEVICEPROPCODE_TIMELAPSENUMBER               = 0x501A,
 *      DEVICEPROPCODE_TIMELAPSEINTERVAL             = 0x501B,
 *      DEVICEPROPCODE_FOCUSMETERINGMODE             = 0x501C,
 *      DEVICEPROPCODE_UPLOADURL                     = 0x501D,
 *      DEVICEPROPCODE_ARTIST                        = 0x501E,
 *      DEVICEPROPCODE_COPYRIGHTINFO                 = 0x501F,
 *      DEVICEPROPCODE_SYNCHRONIZATIONPARTNER        = 0xD401,
 *      DEVICEPROPCODE_DEVICEFRIENDLYNAME            = 0xD402,
 *      DEVICEPROPCODE_RESERVED_FIRST                = 0x5020,
 *      DEVICEPROPCODE_RESERVED_LAST                 = 0x5FFF,
 *      DEVICEPROPCODE_VENDOREXTENSION_FIRST         = 0xD000,
 *      DEVICEPROPCODE_PLAYBACK_VOLUME               = 0xD403,
 *      DEVICEPROPCODE_PLAYBACK_RATE                 = 0xD410,
 *      DEVICEPROPCODE_PLAYBACK_OBJECT               = 0xD411,
 *      DEVICEPROPCODE_PLAYBACK_CONTAINER_INDEX      = 0xD412,
 *      DEVICEPROPCODE_PLAYBACK_POSITION             = 0xD413,
 *      DEVICEPROPCODE_VENDOREXTENSION_LAST          = 0xDFFF
 *  };
 *  @endcode
 *  @li@c  DEVICEPROPCODE_NOTUSED   -Specifies the MTP device property code of not used.
 *  @li@c  DEVICEPROPCODE_ALL   -Specifies the MTP device property code of all.
 *  @li@c  DEVICEPROPCODE_UNDEFINED    -Specifies the MTP device property code of undefined.
 *  @li@c  DEVICEPROPCODE_BATTERYLEVEL    -Specifies the MTP device property code of battery level.
 *  @li@c  DEVICEPROPCODE_FUNCTIONMODE   -Specifies the MTP device property code of function mode.
 *  @li@c  DEVICEPROPCODE_IMAGESIZE    -Specifies the MTP device property code of image size.
 *  @li@c  DEVICEPROPCODE_COMPRESSIONSETTING    -Specifies the MTP device property code of compression setting.
 *  @li@c  DEVICEPROPCODE_WHITEBALANCE  -Specifies the MTP device property code of white balance.
 *  @li@c  DEVICEPROPCODE_RGBGAIN   -Specifies the MTP device property code of RGB gain.
 *  @li@c  DEVICEPROPCODE_FNUMBER    -Specifies the MTP device property code of focus number.
 *  @li@c  DEVICEPROPCODE_FOCALLENGTH    -Specifies the MTP device property code of focus length.
 *  @li@c  DEVICEPROPCODE_FOCUSDISTANCE     -Specifies the MTP device property code of focus distance.
 *  @li@c  DEVICEPROPCODE_FOCUSMODE    -Specifies the MTP device property code of focus mode.
 *  @li@c  DEVICEPROPCODE_EXPOSUREMETERINGMODE   -Specifies the MTP device property code of exposure metering mode.
 *  @li@c  DEVICEPROPCODE_FLASHMODE    -Specifies the MTP device property code of flash mode.
 *  @li@c  DEVICEPROPCODE_EXPOSURETIME    -Specifies the MTP device property code of exposure time.
 *  @li@c  DEVICEPROPCODE_EXPOSUREPROGRAMMODE   -Specifies the MTP device property code of exposure program mode.
 *  @li@c  DEVICEPROPCODE_EXPOSUREINDEX   -Specifies the MTP device property code of exposure index.
 *  @li@c  DEVICEPROPCODE_EXPOSURECOMPENSATION     -Specifies the MTP device property code of exposure compensation.
 *  @li@c  DEVICEPROPCODE_DATETIME     -Specifies the MTP device property code of date time.
 *  @li@c  DEVICEPROPCODE_CAPTUREDELAY    -Specifies the MTP device property code of capture delay.
 *  @li@c  DEVICEPROPCODE_STILLCAPTUREMODE     -Specifies the MTP device property code of still capture mode.
 *  @li@c  DEVICEPROPCODE_CONTRAST     -Specifies the MTP device property code of contrast.
 *  @li@c  DEVICEPROPCODE_SHARPNESS     -Specifies the MTP device property code of sharpness.
 *  @li@c  DEVICEPROPCODE_DIGITALZOOM     -Specifies the MTP device property code of digit zoom.
 *  @li@c  DEVICEPROPCODE_EFFECTMODE     -Specifies the MTP device property code of effect mode.
 *  @li@c  DEVICEPROPCODE_BURSTNUMBER   -Specifies the MTP device property code of burst number.
 *  @li@c  DEVICEPROPCODE_BURSTINTERVAL     -Specifies the MTP device property code of burst interval.
 *  @li@c  DEVICEPROPCODE_TIMELAPSENUMBER    -Specifies the MTP device property code of time lapse number.
 *  @li@c  DEVICEPROPCODE_TIMELAPSEINTERVAL    -Specifies the MTP device property code of time lapse interval.
 *  @li@c  DEVICEPROPCODE_FOCUSMETERINGMODE   -Specifies the MTP device property code of focus metering mode.
 *  @li@c  DEVICEPROPCODE_UPLOADURL    -Specifies the MTP device property code of upload durl.
 *  @li@c  DEVICEPROPCODE_ARTIST   -Specifies the MTP device property code of artist.
 *  @li@c  DEVICEPROPCODE_COPYRIGHTINFO     -Specifies the MTP device property code of copyright info.
 *  @li@c  DEVICEPROPCODE_SYNCHRONIZATIONPARTNER   -Specifies the MTP device property code of synchronization partner.
 *  @li@c  DEVICEPROPCODE_DEVICEFRIENDLYNAME   -Specifies the MTP device property code of device friendly name.
 *  @li@c  DEVICEPROPCODE_RESERVED_FIRST    -Specifies the MTP device property code of reserved first.
 *  @li@c  DEVICEPROPCODE_RESERVED_LAST   -Specifies the MTP device property code of reserved last.
 *  @li@c  DEVICEPROPCODE_VENDOREXTENSION_FIRST   -Specifies the MTP device property code of vendor extension first.
 *  @li@c  DEVICEPROPCODE_PLAYBACK_VOLUME   -Specifies the MTP device property code of playback volume.
 *  @li@c  DEVICEPROPCODE_PLAYBACK_RATE    -Specifies the MTP device property code of playback rate.
 *  @li@c  DEVICEPROPCODE_PLAYBACK_OBJECT    -Specifies the MTP device property code of playback object.
 *  @li@c  DEVICEPROPCODE_PLAYBACK_CONTAINER_INDEX   -Specifies the MTP device property code of playback container index.
 *  @li@c  DEVICEPROPCODE_PLAYBACK_POSITION   -Specifies the MTP device property code of playback position.
 *  @li@c  DEVICEPROPCODE_VENDOREXTENSION_LAST   -Specifies the MTP device property code of vendor extension last.
 */
/*------------------------------------------------------------------*/
enum _DEVICEPROPCODE
{
    DEVICEPROPCODE_NOTUSED                       = 0x0,
    DEVICEPROPCODE_ALL                           = 0xFFFF,
    DEVICEPROPCODE_UNDEFINED                     = 0x5000,
    DEVICEPROPCODE_BATTERYLEVEL                  = 0x5001,
    DEVICEPROPCODE_FUNCTIONMODE                  = 0x5002,
    DEVICEPROPCODE_IMAGESIZE                     = 0x5003,
    DEVICEPROPCODE_COMPRESSIONSETTING            = 0x5004,
    DEVICEPROPCODE_WHITEBALANCE                  = 0x5005,
    DEVICEPROPCODE_RGBGAIN                       = 0x5006,
    DEVICEPROPCODE_FNUMBER                       = 0x5007,
    DEVICEPROPCODE_FOCALLENGTH                   = 0x5008,
    DEVICEPROPCODE_FOCUSDISTANCE                 = 0x5009,
    DEVICEPROPCODE_FOCUSMODE                     = 0x500A,
    DEVICEPROPCODE_EXPOSUREMETERINGMODE          = 0x500B,
    DEVICEPROPCODE_FLASHMODE                     = 0x500C,
    DEVICEPROPCODE_EXPOSURETIME                  = 0x500D,
    DEVICEPROPCODE_EXPOSUREPROGRAMMODE           = 0x500E,
    DEVICEPROPCODE_EXPOSUREINDEX                 = 0x500F,
    DEVICEPROPCODE_EXPOSURECOMPENSATION          = 0x5010,
    DEVICEPROPCODE_DATETIME                      = 0x5011,
    DEVICEPROPCODE_CAPTUREDELAY                  = 0x5012,
    DEVICEPROPCODE_STILLCAPTUREMODE              = 0x5013,
    DEVICEPROPCODE_CONTRAST                      = 0x5014,
    DEVICEPROPCODE_SHARPNESS                     = 0x5015,
    DEVICEPROPCODE_DIGITALZOOM                   = 0x5016,
    DEVICEPROPCODE_EFFECTMODE                    = 0x5017,
    DEVICEPROPCODE_BURSTNUMBER                   = 0x5018,
    DEVICEPROPCODE_BURSTINTERVAL                 = 0x5019,
    DEVICEPROPCODE_TIMELAPSENUMBER               = 0x501A,
    DEVICEPROPCODE_TIMELAPSEINTERVAL             = 0x501B,
    DEVICEPROPCODE_FOCUSMETERINGMODE             = 0x501C,
    DEVICEPROPCODE_UPLOADURL                     = 0x501D,
    DEVICEPROPCODE_ARTIST                        = 0x501E,
    DEVICEPROPCODE_COPYRIGHTINFO                 = 0x501F,
    DEVICEPROPCODE_SYNCHRONIZATIONPARTNER        = 0xD401,
    DEVICEPROPCODE_DEVICEFRIENDLYNAME            = 0xD402,
    DEVICEPROPCODE_RESERVED_FIRST                = 0x5020,
    DEVICEPROPCODE_RESERVED_LAST                 = 0x5FFF,
    DEVICEPROPCODE_VENDOREXTENSION_FIRST         = 0xD000,
    DEVICEPROPCODE_PLAYBACK_VOLUME               = 0xD403,
    DEVICEPROPCODE_PLAYBACK_RATE                 = 0xD410,
    DEVICEPROPCODE_PLAYBACK_OBJECT               = 0xD411,
    DEVICEPROPCODE_PLAYBACK_CONTAINER_INDEX      = 0xD412,
    DEVICEPROPCODE_PLAYBACK_POSITION             = 0xD413,
    DEVICEPROPCODE_VENDOREXTENSION_LAST          = 0xDFFF
};

/** Values for FORMATCODE DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _FORMATCODE
 *  @brief  This enumeration specifies the MTP device format code.
 *  @code
 *  enum _FORMATCODE
 *  {
 *      FORMATCODE_NOTUSED                      = 0x0000,
 *      FORMATCODE_ALLIMAGES                    = 0xFFFF,
 *      FORMATCODE_UNDEFINED                    = 0x3000,
 *      FORMATCODE_ASSOCIATION                  = 0x3001,
 *      FORMATCODE_SCRIPT                       = 0x3002,
 *      FORMATCODE_EXECUTABLE                   = 0x3003,
 *      FORMATCODE_TEXT                         = 0x3004,
 *      FORMATCODE_HTML                         = 0x3005,
 *      FORMATCODE_DPOF                         = 0x3006,
 *      FORMATCODE_AIFF                         = 0x3007,
 *      FORMATCODE_WAVE                         = 0x3008,
 *      FORMATCODE_MP3                          = 0x3009,
 *      FORMATCODE_AVI                          = 0x300A,
 *      FORMATCODE_MPEG                         = 0x300B,
 *      FORMATCODE_ASF                          = 0x300C,
 *      FORMATCODE_RESERVED_FIRST               = 0x300D,
 *      FORMATCODE_RESERVED_LAST                = 0x37FF,
 *      FORMATCODE_IMAGE_UNDEFINED              = 0x3800,
 *      FORMATCODE_IMAGE_EXIF                   = 0x3801,
 *      FORMATCODE_IMAGE_TIFFEP                 = 0x3802,
 *      FORMATCODE_IMAGE_FLASHPIX               = 0x3803,
 *      FORMATCODE_IMAGE_BMP                    = 0x3804,
 *      FORMATCODE_IMAGE_CIFF                   = 0x3805,
 *      FORMATCODE_IMAGE_GIF                    = 0x3807,
 *      FORMATCODE_IMAGE_JFIF                   = 0x3808,
 *      FORMATCODE_IMAGE_PCD                    = 0x3809,
 *      FORMATCODE_IMAGE_PICT                   = 0x380A,
 *      FORMATCODE_IMAGE_PNG                    = 0x380B,
 *      FORMATCODE_IMAGE_TIFF                   = 0x380D,
 *      FORMATCODE_IMAGE_TIFFIT                 = 0x380E,
 *      FORMATCODE_IMAGE_JP2                    = 0x380F,
 *      FORMATCODE_IMAGE_JPX                    = 0x3810,
 *      FORMATCODE_IMAGE_RESERVED_FIRST         = 0x3811,
 *      FORMATCODE_IMAGE_RESERVED_LAST          = 0x3FFF,
 *      FORMATCODE_UNDEFINEDFIRMWARE            = 0xB802,
 *      FORMATCODE_WINDOWSIMAGEFORMAT           = 0xB881,
 *      FORMATCODE_UNDEFINEDAUDIO               = 0xB900,
 *      FORMATCODE_WMA                          = 0xB901,
 *      FORMATCODE_UNDEFINEDVIDEO               = 0xB980,
 *      FORMATCODE_WMV                          = 0xB981,
 *      FORMATCODE_UNDEFINEDCOLLECTION          = 0xBA00,
 *      FORMATCODE_ABSTRACTMULTIMEDIAALBUM      = 0xBA01,
 *      FORMATCODE_ABSTRACTIMAGEALBUM           = 0xBA02,
 *      FORMATCODE_ABSTRACTAUDIOALBUM           = 0xBA03,
 *      FORMATCODE_ABSTRACTVIDEOALBUM           = 0xBA04,
 *      FORMATCODE_ABSTRACTAUDIOVIDEOPLAYLIST   = 0xBA05,
 *      FORMATCODE_ABSTRACTCONTACTGROUP         = 0xBA06,
 *      FORMATCODE_ABSTRACTMESSAGEFOLDER        = 0xBA07,
 *      FORMATCODE_ABSTRACTCHAPTEREDPRODUCTION  = 0xBA08,
 *      FORMATCODE_WPLPLAYLIST                  = 0xBA10,
 *      FORMATCODE_M3UPLAYLIST                  = 0xBA11,
 *      FORMATCODE_MPLPLAYLIST                  = 0xBA12,
 *      FORMATCODE_ASXPLAYLIST                  = 0xBA13,
 *      FORMATCODE_PLSPLAYLIST                  = 0xBA14,
 *      FORMATCODE_UNDEFINEDDOCUMENT            = 0xBA80,
 *      FORMATCODE_ABSTRACTDOCUMENT             = 0xBA81,
 *      FORMATCODE_UNDEFINEDMESSAGE             = 0xBB00,
 *      FORMATCODE_ABSTRACTMESSAGE              = 0xBB01,
 *      FORMATCODE_UNDEFINEDCONTACT             = 0xBB80,
 *      FORMATCODE_ABSTRACTCONTACT              = 0xBB81,
 *      FORMATCODE_VCARD2                       = 0xBB82,
 *      FORMATCODE_VCARD3                       = 0xBB83,
 *      FORMATCODE_UNDEFINEDCALENDARITEM        = 0xBE00,
 *      FORMATCODE_ABSTRACTCALENDARITEM         = 0xBE01,
 *      FORMATCODE_VCALENDAR1                   = 0xBE02,
 *      FORMATCODE_UNDEFINEDWINDOWSEXECUTABLE   = 0xBE80
 *  };
 *  @endcode
 *  @li@c  FORMATCODE_NOTUSED      -Specifies the MTP device format code of not used.
 *  @li@c  FORMATCODE_ALLIMAGES   -Specifies the MTP device format code of all images.
 *  @li@c  FORMATCODE_UNDEFINED    -Specifies the MTP device format code of undefined.
 *  @li@c  FORMATCODE_ASSOCIATION  -Specifies the MTP device format code of association.
 *  @li@c  FORMATCODE_SCRIPT  -Specifies the MTP device format code of script.
 *  @li@c  FORMATCODE_EXECUTABLE   -Specifies the MTP device format code of executable.
 *  @li@c  FORMATCODE_TEXT     -Specifies the MTP device format code of test.
 *  @li@c  FORMATCODE_HTML     -Specifies the MTP device format code of HTML.
 *  @li@c  FORMATCODE_DPOF     -Specifies the MTP device format code of DPOF.
 *  @li@c  FORMATCODE_AIFF       -Specifies the MTP device format code of AIFF.
 *  @li@c  FORMATCODE_WAVE    -Specifies the MTP device format code of WAVE.
 *  @li@c  FORMATCODE_MP3      -Specifies the MTP device format code of MP3.
 *  @li@c  FORMATCODE_AVI       -Specifies the MTP device format code of AVI.
 *  @li@c  FORMATCODE_MPEG    -Specifies the MTP device format code of MPEG.
 *  @li@c  FORMATCODE_ASF      -Specifies the MTP device format code of ASF.
 *  @li@c  FORMATCODE_RESERVED_FIRST   -Specifies the MTP device format code of reserved first.
 *  @li@c  FORMATCODE_RESERVED_LAST    -Specifies the MTP device format code of reserved last.
 *  @li@c  FORMATCODE_IMAGE_UNDEFINED   -Specifies the MTP device format code of image undefined.
 *  @li@c  FORMATCODE_IMAGE_EXIF    -Specifies the MTP device format code of image EXIF.
 *  @li@c  FORMATCODE_IMAGE_TIFFEP    -Specifies the MTP device format code of image TIFFEP.
 *  @li@c  FORMATCODE_IMAGE_FLASHPIX    -Specifies the MTP device format code of image FLASHPIX.
 *  @li@c  FORMATCODE_IMAGE_BMP    -Specifies the MTP device format code of image BMP.
 *  @li@c  FORMATCODE_IMAGE_CIFF    -Specifies the MTP device format code of image CIFF.
 *  @li@c  FORMATCODE_IMAGE_GIF    -Specifies the MTP device format code of image GIF.
 *  @li@c  FORMATCODE_IMAGE_JFIF      -Specifies the MTP device format code of image JFIF.
 *  @li@c  FORMATCODE_IMAGE_PCD      -Specifies the MTP device format code of image PCD.
 *  @li@c  FORMATCODE_IMAGE_PICT     -Specifies the MTP device format code of image PICT.
 *  @li@c  FORMATCODE_IMAGE_PNG       -Specifies the MTP device format code of image PNG.
 *  @li@c  FORMATCODE_IMAGE_TIFF       -Specifies the MTP device format code of image TIFF.
 *  @li@c  FORMATCODE_IMAGE_TIFFIT     -Specifies the MTP device format code of image TIFFIT.
 *  @li@c  FORMATCODE_IMAGE_JP2     -Specifies the MTP device format code of image JP2.
 *  @li@c  FORMATCODE_IMAGE_JPX      -Specifies the MTP device format code of JPX.
 *  @li@c  FORMATCODE_IMAGE_RESERVED_FIRST      -Specifies the MTP device format code of image reserved first.
 *  @li@c  FORMATCODE_IMAGE_RESERVED_LAST    -Specifies the MTP device format code of image reserved last.
 *  @li@c  FORMATCODE_UNDEFINEDFIRMWARE     -Specifies the MTP device format code of undefined firmware.
 *  @li@c  FORMATCODE_WINDOWSIMAGEFORMAT   -Specifies the MTP device format code of windows image format.
 *  @li@c  FORMATCODE_UNDEFINEDAUDIO   -Specifies the MTP device format code of undefined audio.
 *  @li@c  FORMATCODE_WMA        -Specifies the MTP device format code of WMA.
 *  @li@c  FORMATCODE_UNDEFINEDVIDEO     -Specifies the MTP device format code of undefined video.
 *  @li@c  FORMATCODE_WMV        -Specifies the MTP device format code of WMV.
 *  @li@c  FORMATCODE_UNDEFINEDCOLLECTION   -Specifies the MTP device format code of undefined collection.
 *  @li@c  FORMATCODE_ABSTRACTMULTIMEDIAALBUM    -Specifies the MTP device format code of abstract multimedia album.
 *  @li@c  FORMATCODE_ABSTRACTIMAGEALBUM  -Specifies the MTP device format code of abstract image album.
 *  @li@c  FORMATCODE_ABSTRACTAUDIOALBUM   -Specifies the MTP device format code of abstract audio album.
 *  @li@c  FORMATCODE_ABSTRACTVIDEOALBUM    -Specifies the MTP device format code of abstract video album.
 *  @li@c  FORMATCODE_ABSTRACTAUDIOVIDEOPLAYLIST   -Specifies the MTP device format code of abstract audio video play list.
 *  @li@c  FORMATCODE_ABSTRACTCONTACTGROUP    -Specifies the MTP device format code of abstract contact group.
 *  @li@c  FORMATCODE_ABSTRACTMESSAGEFOLDER    -Specifies the MTP device format code of abstract message folder.
 *  @li@c  FORMATCODE_ABSTRACTCHAPTEREDPRODUCTION   -Specifies the MTP device format code of abstract chaptered production.
 *  @li@c  FORMATCODE_WPLPLAYLIST    -Specifies the MTP device format code of WPL play list.
 *  @li@c  FORMATCODE_M3UPLAYLIST    -Specifies the MTP device format code of M3U play list.
 *  @li@c  FORMATCODE_MPLPLAYLIST    -Specifies the MTP device format code of MPL play list.
 *  @li@c  FORMATCODE_ASXPLAYLIST    -Specifies the MTP device format code of ASX play list.
 *  @li@c  FORMATCODE_PLSPLAYLIST     -Specifies the MTP device format code of PLS play list.
 *  @li@c  FORMATCODE_UNDEFINEDDOCUMENT   -Specifies the MTP device format code of undefined document.
 *  @li@c  FORMATCODE_ABSTRACTDOCUMENT   -Specifies the MTP device format code of abstract document.
 *  @li@c  FORMATCODE_UNDEFINEDMESSAGE    -Specifies the MTP device format code of undefined message.
 *  @li@c  FORMATCODE_ABSTRACTMESSAGE   -Specifies the MTP device format code of abstract message.
 *  @li@c  FORMATCODE_UNDEFINEDCONTACT    -Specifies the MTP device format code of undefined contact.
 *  @li@c  FORMATCODE_ABSTRACTCONTACT     -Specifies the MTP device format code of abstract contact.
 *  @li@c  FORMATCODE_VCARD2     -Specifies the MTP device format code of VCARD2.
 *  @li@c  FORMATCODE_VCARD3     -Specifies the MTP device format code of VCARD3.
 *  @li@c  FORMATCODE_UNDEFINEDCALENDARITEM   -Specifies the MTP device format code of undefined calendar item.
 *  @li@c  FORMATCODE_ABSTRACTCALENDARITEM   -Specifies the MTP device format code of abstract calendar item.
 *  @li@c  FORMATCODE_VCALENDAR1    -Specifies the MTP device format code of v calendar 1.
 *  @li@c  FORMATCODE_UNDEFINEDWINDOWSEXECUTABLE    -Specifies the MTP device format code of undefined windows executable.
 */
/*------------------------------------------------------------------*/
enum _FORMATCODE
{
    FORMATCODE_NOTUSED                      = 0x0000,
    FORMATCODE_ALLIMAGES                    = 0xFFFF,
    FORMATCODE_UNDEFINED                    = 0x3000,
    FORMATCODE_ASSOCIATION                  = 0x3001,
    FORMATCODE_SCRIPT                       = 0x3002,
    FORMATCODE_EXECUTABLE                   = 0x3003,
    FORMATCODE_TEXT                         = 0x3004,
    FORMATCODE_HTML                         = 0x3005,
    FORMATCODE_DPOF                         = 0x3006,
    FORMATCODE_AIFF                         = 0x3007,
    FORMATCODE_WAVE                         = 0x3008,
    FORMATCODE_MP3                          = 0x3009,
    FORMATCODE_AVI                          = 0x300A,
    FORMATCODE_MPEG                         = 0x300B,
    FORMATCODE_ASF                          = 0x300C,
    FORMATCODE_RESERVED_FIRST               = 0x300D,
    FORMATCODE_RESERVED_LAST                = 0x37FF,

    FORMATCODE_IMAGE_UNDEFINED              = 0x3800,
    FORMATCODE_IMAGE_EXIF                   = 0x3801,
    FORMATCODE_IMAGE_TIFFEP                 = 0x3802,
    FORMATCODE_IMAGE_FLASHPIX               = 0x3803,
    FORMATCODE_IMAGE_BMP                    = 0x3804,
    FORMATCODE_IMAGE_CIFF                   = 0x3805,
    FORMATCODE_IMAGE_GIF                    = 0x3807,
    FORMATCODE_IMAGE_JFIF                   = 0x3808,
    FORMATCODE_IMAGE_PCD                    = 0x3809,
    FORMATCODE_IMAGE_PICT                   = 0x380A,
    FORMATCODE_IMAGE_PNG                    = 0x380B,
    FORMATCODE_IMAGE_TIFF                   = 0x380D,
    FORMATCODE_IMAGE_TIFFIT                 = 0x380E,
    FORMATCODE_IMAGE_JP2                    = 0x380F,
    FORMATCODE_IMAGE_JPX                    = 0x3810,
    FORMATCODE_IMAGE_RESERVED_FIRST         = 0x3811,
    FORMATCODE_IMAGE_RESERVED_LAST          = 0x3FFF,

    FORMATCODE_UNDEFINEDFIRMWARE            = 0xB802,
    FORMATCODE_WINDOWSIMAGEFORMAT           = 0xB881,
    FORMATCODE_UNDEFINEDAUDIO               = 0xB900,
    FORMATCODE_WMA                          = 0xB901,
    FORMATCODE_UNDEFINEDVIDEO               = 0xB980,
    FORMATCODE_WMV                          = 0xB981,
    FORMATCODE_UNDEFINEDCOLLECTION          = 0xBA00,
    FORMATCODE_ABSTRACTMULTIMEDIAALBUM      = 0xBA01,
    FORMATCODE_ABSTRACTIMAGEALBUM           = 0xBA02,
    FORMATCODE_ABSTRACTAUDIOALBUM           = 0xBA03,
    FORMATCODE_ABSTRACTVIDEOALBUM           = 0xBA04,
    FORMATCODE_ABSTRACTAUDIOVIDEOPLAYLIST   = 0xBA05,
    FORMATCODE_ABSTRACTCONTACTGROUP         = 0xBA06,
    FORMATCODE_ABSTRACTMESSAGEFOLDER        = 0xBA07,
    FORMATCODE_ABSTRACTCHAPTEREDPRODUCTION  = 0xBA08,
    FORMATCODE_WPLPLAYLIST                  = 0xBA10,
    FORMATCODE_M3UPLAYLIST                  = 0xBA11,
    FORMATCODE_MPLPLAYLIST                  = 0xBA12,
    FORMATCODE_ASXPLAYLIST                  = 0xBA13,
    FORMATCODE_PLSPLAYLIST                  = 0xBA14,
    FORMATCODE_UNDEFINEDDOCUMENT            = 0xBA80,
    FORMATCODE_ABSTRACTDOCUMENT             = 0xBA81,
    FORMATCODE_UNDEFINEDMESSAGE             = 0xBB00,
    FORMATCODE_ABSTRACTMESSAGE              = 0xBB01,
    FORMATCODE_UNDEFINEDCONTACT             = 0xBB80,
    FORMATCODE_ABSTRACTCONTACT              = 0xBB81,
    FORMATCODE_VCARD2                       = 0xBB82,
    FORMATCODE_VCARD3                       = 0xBB83,
    FORMATCODE_UNDEFINEDCALENDARITEM        = 0xBE00,
    FORMATCODE_ABSTRACTCALENDARITEM         = 0xBE01,
    FORMATCODE_VCALENDAR1                   = 0xBE02,
    FORMATCODE_UNDEFINEDWINDOWSEXECUTABLE   = 0xBE80
};

/** Values for STORAGETYPE DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _STORAGETYPE
 *  @brief  This enumeration specifies the MTP device storage type.
 *  @code
 *  enum _STORAGETYPE
 *  {
 *      STORAGETYPE_UNDEFINED      = 0x0000,
 *      STORAGETYPE_FIXEDROM       = 0x0001,
 *      STORAGETYPE_REMOVABLEROM   = 0x0002,
 *      STORAGETYPE_FIXEDRAM       = 0x0003,
 *      STORAGETYPE_REMOVABLERAM   = 0x0004,
 *      STORAGETYPE_RESERVED_FIRST = 0x0005,
 *      STORAGETYPE_RESERVED_LAST  = 0xFFFF
 *  };
 *  @endcode
 *  @li@c  STORAGETYPE_UNDEFINED    -Specifies the MTP device storage type of undefined.
 *  @li@c  STORAGETYPE_FIXEDROM    -Specifies the MTP device storage type of fixed rom.
 *  @li@c  STORAGETYPE_REMOVABLEROM  -Specifies the MTP device storage type of removable rom.
 *  @li@c  STORAGETYPE_FIXEDRAM    -Specifies the MTP device storage type of fixed ram.
 *  @li@c  STORAGETYPE_REMOVABLERAM   -Specifies the MTP device storage type of removable ram.
 *  @li@c  STORAGETYPE_RESERVED_FIRST   -Specifies the MTP device storage type of reserved first.
 *  @li@c  STORAGETYPE_RESERVED_LAST  -Specifies the MTP device storage type of reserved last.
 */
/*------------------------------------------------------------------*/
enum _STORAGETYPE
{
    STORAGETYPE_UNDEFINED      = 0x0000,
    STORAGETYPE_FIXEDROM       = 0x0001,
    STORAGETYPE_REMOVABLEROM   = 0x0002,
    STORAGETYPE_FIXEDRAM       = 0x0003,
    STORAGETYPE_REMOVABLERAM   = 0x0004,
    STORAGETYPE_RESERVED_FIRST = 0x0005,
    STORAGETYPE_RESERVED_LAST  = 0xFFFF
};

/** Values for FILESYSTEMTYPE DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _FILESYSTEMTYPE
 *  @brief   This enumeration specifies the MTP device file system type.
 *  @code
 *  enum _FILESYSTEMTYPE
 *  {
 *      FILESYSTEMTYPE_UNDEFINED             = 0x0000,
 *      FILESYSTEMTYPE_FLAT                  = 0x0001,
 *      FILESYSTEMTYPE_HIERARCHICAL          = 0x0002,
 *      FILESYSTEMTYPE_DCF                   = 0x0003,
 *      FILESYSTEMTYPE_RESERVED_FIRST        = 0x0004,
 *      FILESYSTEMTYPE_RESERVED_LAST         = 0x7FFF,
 *      FILESYSTEMTYPE_VENDOREXTENSION_FIRST = 0x8000,
 *      FILESYSTEMTYPE_VENDOREXTENSION_LAST  = 0xFFFF
 *  };
 *  @endcode
 *  @li@c  FILESYSTEMTYPE_UNDEFINED    -Specifies the MTP device file system type of undefined.
 *  @li@c  FILESYSTEMTYPE_FLAT     -Specifies the MTP device file system type of flat.
 *  @li@c  FILESYSTEMTYPE_HIERARCHICAL   -Specifies the MTP device file system type of hierarchical.
 *  @li@c  FILESYSTEMTYPE_DCF    -Specifies the MTP device file system type of DCF.
 *  @li@c  FILESYSTEMTYPE_RESERVED_FIRST    -Specifies the MTP device file system type of reserved first.
 *  @li@c  FILESYSTEMTYPE_RESERVED_LAST   -Specifies the MTP device file system type of reserved last.
 *  @li@c  FILESYSTEMTYPE_VENDOREXTENSION_FIRST   -Specifies the MTP device file system type of vendor extension first.
 *  @li@c  FILESYSTEMTYPE_VENDOREXTENSION_LAST    -Specifies the MTP device file system type of vendor extension last.
 */
/*------------------------------------------------------------------*/
enum _FILESYSTEMTYPE
{
    FILESYSTEMTYPE_UNDEFINED             = 0x0000,
    FILESYSTEMTYPE_FLAT                  = 0x0001,
    FILESYSTEMTYPE_HIERARCHICAL          = 0x0002,
    FILESYSTEMTYPE_DCF                   = 0x0003,
    FILESYSTEMTYPE_RESERVED_FIRST        = 0x0004,
    FILESYSTEMTYPE_RESERVED_LAST         = 0x7FFF,
    FILESYSTEMTYPE_VENDOREXTENSION_FIRST = 0x8000,
    FILESYSTEMTYPE_VENDOREXTENSION_LAST  = 0xFFFF
};

/** Values for STORAGEACCESSCAPABILITY DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _STORAGEACCESSCAPABILITY
 *  @brief  This enumeration specifies the MTP device access capability.
 *  @code
 *  enum _STORAGEACCESSCAPABILITY
 *  {
 *      STORAGEACCESSCAPABILITY_READ_WRITE     = 0x0000,
 *      STORAGEACCESSCAPABILITY_READ_DELETE    = 0x0001,
 *      STORAGEACCESSCAPABILITY_READ_NO_DELETE = 0x0002,
 *      STORAGEACCESSCAPABILITY_RESERVED_FIRST = 0x0003,
 *      STORAGEACCESSCAPABILITY_RESERVED_LAST  = 0xFFFF
 *  };
 *  @endcode
 *  @li@c  STORAGEACCESSCAPABILITY_READ_WRITE    -Specifies the MTP device access capability of read write.
 *  @li@c  STORAGEACCESSCAPABILITY_READ_DELETE   -Specifies the MTP device access capability of delete.
 *  @li@c  STORAGEACCESSCAPABILITY_READ_NO_DELETE  -Specifies the MTP device access capability of no delete.
 *  @li@c  STORAGEACCESSCAPABILITY_RESERVED_FIRST   -Specifies the MTP device access capability of reserved first.
 *  @li@c  STORAGEACCESSCAPABILITY_RESERVED_LAST  -Specifies the MTP device access capability of reserved last.
 */
/*------------------------------------------------------------------*/
enum _STORAGEACCESSCAPABILITY
{
    STORAGEACCESSCAPABILITY_READ_WRITE     = 0x0000,
    STORAGEACCESSCAPABILITY_READ_DELETE    = 0x0001,
    STORAGEACCESSCAPABILITY_READ_NO_DELETE = 0x0002,
    STORAGEACCESSCAPABILITY_RESERVED_FIRST = 0x0003,
    STORAGEACCESSCAPABILITY_RESERVED_LAST  = 0xFFFF
};

/** Values for PROTECTIONSTATUS DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _PROTECTIONSTATUS
 *  @brief  This enumeration specifies the MTP device protection status.
 *  @code
 *  enum _PROTECTIONSTATUS
 *  {
 *      PROTECTIONSTATUS_NONE            = 0x0000,
 *      PROTECTIONSTATUS_READONLY        = 0x0001,
 *      PROTECTIONSTATUS_READONLYDATA    = 0x8002,
 *      PROTECTIONSTATUS_RESERVED_FIRST  = 0x8003,
 *      PROTECTIONSTATUS_RESERVED_LAST   = 0xFFFF
 *  };
 *  @endcode
 *  @li@c  PROTECTIONSTATUS_NONE    -Specifies the MTP device protection status of none.
 *  @li@c  PROTECTIONSTATUS_READONLY   -Specifies the MTP device protection status of read only.
 *  @li@c  PROTECTIONSTATUS_READONLYDATA   -Specifies the MTP device protection status of read only data.
 *  @li@c  PROTECTIONSTATUS_RESERVED_FIRST  -Specifies the MTP device protection status of reserved first.
 *  @li@c  PROTECTIONSTATUS_RESERVED_LAST   -Specifies the MTP device protection status of reserved last.
 */
/*------------------------------------------------------------------*/
enum _PROTECTIONSTATUS
{
    PROTECTIONSTATUS_NONE            = 0x0000,
    PROTECTIONSTATUS_READONLY        = 0x0001,
    PROTECTIONSTATUS_READONLYDATA    = 0x8002,
    PROTECTIONSTATUS_RESERVED_FIRST  = 0x8003,
    PROTECTIONSTATUS_RESERVED_LAST   = 0xFFFF
};

/** Values for ASSOCIATIONTYPE DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _ASSOCIATIONTYPE
 *  @brief  This enumeration specifies the MTP device association type.
 *  @code
 *  enum _ASSOCIATIONTYPE
 *  {
 *      ASSOCIATIONTYPE_UNDEFINED             = 0x0000,
 *      ASSOCIATIONTYPE_GENERICFOLDER         = 0x0001,
 *      ASSOCIATIONTYPE_ALBUM                 = 0x0002,
 *      ASSOCIATIONTYPE_TIMESEQUENCE          = 0x0003,
 *      ASSOCIATIONTYPE_HPANORAMIC            = 0x0004,
 *      ASSOCIATIONTYPE_VPANORAMIC            = 0x0005,
 *      ASSOCIATIONTYPE_2DPANORAMIC           = 0x0006,
 *      ASSOCIATIONTYPE_ANCILLARYDATA         = 0x0007,
 *      ASSOCIATIONTYPE_RESERVED_FIRST        = 0x0008,
 *      ASSOCIATIONTYPE_RESERVED_LAST         = 0x7FFF,
 *      ASSOCIATIONTYPE_VENDOREXTENSION_FIRST = 0x8000,
 *      ASSOCIATIONTYPE_VENDOREXTENSION_LAST  = 0xFFFF
 *  };
 *  @endcode
 *  @li@c  ASSOCIATIONTYPE_UNDEFINED    -Specifies the MTP device association type of undefined.
 *  @li@c  ASSOCIATIONTYPE_GENERICFOLDER  -Specifies the MTP device association type of generic folder.
 *  @li@c  ASSOCIATIONTYPE_ALBUM -Specifies the MTP device association type of album.
 *  @li@c  ASSOCIATIONTYPE_TIMESEQUENCE   -Specifies the MTP device association type of time sequence.
 *  @li@c  ASSOCIATIONTYPE_HPANORAMIC   -Specifies the MTP device association type of H panoramic.
 *  @li@c  ASSOCIATIONTYPE_VPANORAMIC   -Specifies the MTP device association type of V panoramic.
 *  @li@c  ASSOCIATIONTYPE_2DPANORAMIC   -Specifies the MTP device association type of 2D panoramic.
 *  @li@c  ASSOCIATIONTYPE_ANCILLARYDATA  -Specifies the MTP device association type of ancillary data.
 *  @li@c  ASSOCIATIONTYPE_RESERVED_FIRST  -Specifies the MTP device association type of reserved first.
 *  @li@c  ASSOCIATIONTYPE_RESERVED_LAST    -Specifies the MTP device association type of reserved last.
 *  @li@c  ASSOCIATIONTYPE_VENDOREXTENSION_FIRST  -Specifies the MTP device association type of vendor extension first.
 *  @li@c  ASSOCIATIONTYPE_VENDOREXTENSION_LAST   -Specifies the MTP device association type of vendor extension last.
 */
/*------------------------------------------------------------------*/
enum _ASSOCIATIONTYPE
{
    ASSOCIATIONTYPE_UNDEFINED             = 0x0000,
    ASSOCIATIONTYPE_GENERICFOLDER         = 0x0001,
    ASSOCIATIONTYPE_ALBUM                 = 0x0002,
    ASSOCIATIONTYPE_TIMESEQUENCE          = 0x0003,
    ASSOCIATIONTYPE_HPANORAMIC            = 0x0004,
    ASSOCIATIONTYPE_VPANORAMIC            = 0x0005,
    ASSOCIATIONTYPE_2DPANORAMIC           = 0x0006,
    ASSOCIATIONTYPE_ANCILLARYDATA         = 0x0007,
    ASSOCIATIONTYPE_RESERVED_FIRST        = 0x0008,
    ASSOCIATIONTYPE_RESERVED_LAST         = 0x7FFF,
    ASSOCIATIONTYPE_VENDOREXTENSION_FIRST = 0x8000,
    ASSOCIATIONTYPE_VENDOREXTENSION_LAST  = 0xFFFF
};

/** Values for DATATYPE_T DATATYPE 
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _DATATYPE
 *  @brief  This enumeration specifies the MTP device data type.
 *  @code
 *  enum _DATATYPE
 *  {
 *      DATATYPE_UNDEFINED   = 0x0000,
 *      DATATYPE_INT8        = 0x0001,
 *      DATATYPE_UINT8       = 0x0002,
 *      DATATYPE_INT16       = 0x0003,
 *      DATATYPE_UINT16      = 0x0004,
 *      DATATYPE_INT32       = 0x0005,
 *      DATATYPE_UINT32      = 0x0006,
 *      DATATYPE_INT64       = 0x0007,
 *      DATATYPE_UINT64      = 0x0008,
 *      DATATYPE_INT128      = 0x0009,
 *      DATATYPE_UINT128     = 0x000A,
 *      DATATYPE_AINT8       = 0x4001,
 *      DATATYPE_AUINT8      = 0x4002,
 *      DATATYPE_AINT16      = 0x4003,
 *      DATATYPE_AUINT16     = 0x4004,
 *      DATATYPE_AINT32      = 0x4005,
 *      DATATYPE_AUINT32     = 0x4006,
 *      DATATYPE_AINT64      = 0x4007,
 *      DATATYPE_AUINT64     = 0x4008,
 *      DATATYPE_AINT128     = 0x4009,
 *      DATATYPE_AUINT128    = 0x400A,
 *      DATATYPE_STRING      = 0xFFFF
 *  };
 *  @endcode
 *  @li@c  DATATYPE_UNDEFINED -Specifies the MTP device data type of undefined.
 *  @li@c  DATATYPE_INT8     -Specifies the MTP device data type of INT8.
 *  @li@c  DATATYPE_UINT8   -Specifies the MTP device data type of UINT8.
 *  @li@c  DATATYPE_INT16    -Specifies the MTP device data type of INT16.
 *  @li@c  DATATYPE_UINT16   -Specifies the MTP device data type of UINT16.
 *  @li@c  DATATYPE_INT32    -Specifies the MTP device data type of INT32.
 *  @li@c  DATATYPE_UINT32   -Specifies the MTP device data type of UINT32.
 *  @li@c  DATATYPE_INT64   -Specifies the MTP device data type of INT64.
 *  @li@c  DATATYPE_UINT64   -Specifies the MTP device data type of UINT64.
 *  @li@c  DATATYPE_INT128   -Specifies the MTP device data type of INT128.
 *  @li@c  DATATYPE_UINT128    -Specifies the MTP device data type of UINT128.
 *  @li@c  DATATYPE_AINT8    -Specifies the MTP device data type of AINT8.
 *  @li@c  DATATYPE_AUINT8   -Specifies the MTP device data type of AUINT8.
 *  @li@c  DATATYPE_AINT16     -Specifies the MTP device data type of AINT16.
 *  @li@c  DATATYPE_AUINT16    -Specifies the MTP device data type of AUINT16.
 *  @li@c  DATATYPE_AINT32   -Specifies the MTP device data type of AINT32.
 *  @li@c  DATATYPE_AUINT32     -Specifies the MTP device data type of AUINT32.
 *  @li@c  DATATYPE_AINT64   -Specifies the MTP device data type of AINT64.
 *  @li@c  DATATYPE_AUINT64   -Specifies the MTP device data type of AUINT64.
 *  @li@c  DATATYPE_AINT128     -Specifies the MTP device data type of AINT128.
 *  @li@c  DATATYPE_AUINT128   -Specifies the MTP device data type of AUINT128.
 *  @li@c  DATATYPE_STRING    -Specifies the MTP device data type of STRING.
 */
/*------------------------------------------------------------------*/
enum _DATATYPE
{
    DATATYPE_UNDEFINED   = 0x0000,
    DATATYPE_INT8        = 0x0001,
    DATATYPE_UINT8       = 0x0002,
    DATATYPE_INT16       = 0x0003,
    DATATYPE_UINT16      = 0x0004,
    DATATYPE_INT32       = 0x0005,
    DATATYPE_UINT32      = 0x0006,
    DATATYPE_INT64       = 0x0007,
    DATATYPE_UINT64      = 0x0008,
    DATATYPE_INT128      = 0x0009,
    DATATYPE_UINT128     = 0x000A,
    DATATYPE_AINT8       = 0x4001,
    DATATYPE_AUINT8      = 0x4002,
    DATATYPE_AINT16      = 0x4003,
    DATATYPE_AUINT16     = 0x4004,
    DATATYPE_AINT32      = 0x4005,
    DATATYPE_AUINT32     = 0x4006,
    DATATYPE_AINT64      = 0x4007,
    DATATYPE_AUINT64     = 0x4008,
    DATATYPE_AINT128     = 0x4009,
    DATATYPE_AUINT128    = 0x400A,
    DATATYPE_STRING      = 0xFFFF
};

/** Values for OBJECTPROPCODE DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _OBJECTPROPCODE
 *  @brief  This enumeration specifies the MTP device object prop code.
 *  @code
 *  enum _OBJECTPROPCODE
 *  {
 *      OBJECTPROPCODE_NOTUSED                          = 0x0000,
 *      OBJECTPROPCODE_ALL                              = 0xFFFF,
 *      OBJECTPROPCODE_UNDEFINED                        = 0xD000,
 *      OBJECTPROPCODE_STORAGEID                        = 0xDC01,
 *      OBJECTPROPCODE_OBJECTFORMAT                     = 0xDC02,
 *      OBJECTPROPCODE_PROTECTIONSTATUS                 = 0xDC03,
 *      OBJECTPROPCODE_OBJECTSIZE                       = 0xDC04,
 *      OBJECTPROPCODE_ASSOCIATIONTYPE                  = 0xDC05,
 *      OBJECTPROPCODE_ASSOCIATIONDESC                  = 0xDC06,
 *      OBJECTPROPCODE_OBJECTFILENAME                   = 0xDC07,
 *      OBJECTPROPCODE_DATECREATED                      = 0xDC08,
 *      OBJECTPROPCODE_DATEMODIFIED                     = 0xDC09,
 *      OBJECTPROPCODE_KEYWORDS                         = 0xDC0A,
 *      OBJECTPROPCODE_PARENT                           = 0xDC0B,
 *      OBJECTPROPCODE_PERSISTENTUNIQUEOBJECTIDENTIFIER = 0xDC41,
 *      OBJECTPROPCODE_SYNCID                           = 0xDC42,
 *      OBJECTPROPCODE_PROPERTYBAG                      = 0xDC43,
 *      OBJECTPROPCODE_NAME                             = 0xDC44,
 *      OBJECTPROPCODE_CREATEDBY                        = 0xDC45,
 *      OBJECTPROPCODE_ARTIST                           = 0xDC46,
 *      OBJECTPROPCODE_DATEAUTHORED                     = 0xDC47,
 *      OBJECTPROPCODE_DESCRIPTION                      = 0xDC48,
 *      OBJECTPROPCODE_URLREFERENCE                     = 0xDC49,
 *      OBJECTPROPCODE_LANGUAGELOCALE                   = 0xDC4A,
 *      OBJECTPROPCODE_COPYRIGHTINFORMATION             = 0xDC4B,
 *      OBJECTPROPCODE_SOURCE                           = 0xDC4C,
 *      OBJECTPROPCODE_ORIGINLOCATION                   = 0xDC4D,
 *      OBJECTPROPCODE_DATEADDED                        = 0xDC4E,
 *      OBJECTPROPCODE_NONCONSUMABLE                    = 0xDC4F,
 *      OBJECTPROPCODE_CORRUPTUNPLAYABLE                = 0xDC4F,
 *      OBJECTPROPCODE_REPRESENTATIVESAMPLEFORMAT       = 0xDC81,
 *      OBJECTPROPCODE_REPRESENTATIVESAMPLESIZE         = 0xDC82,
 *      OBJECTPROPCODE_REPRESENTATIVESAMPLEHEIGHT       = 0xDC83,
 *      OBJECTPROPCODE_REPRESENTATIVESAMPLEWIDTH        = 0xDC84,
 *      OBJECTPROPCODE_REPRESENTATIVESAMPLEDURATION     = 0xDC85,
 *      OBJECTPROPCODE_REPRESENTATIVESAMPLEDATA         = 0xDC86,
 *      OBJECTPROPCODE_WIDTH                            = 0xDC87,
 *      OBJECTPROPCODE_HEIGHT                           = 0xDC88,
 *      OBJECTPROPCODE_DURATION                         = 0xDC89,
 *      OBJECTPROPCODE_USERRATING                       = 0xDC8A,
 *      OBJECTPROPCODE_TRACK                            = 0xDC8B,
 *      OBJECTPROPCODE_GENRE                            = 0xDC8C,
 *      OBJECTPROPCODE_CREDITS                          = 0xDC8D,
 *      OBJECTPROPCODE_LYRICS                           = 0xDC8E,
 *      OBJECTPROPCODE_SUBSCRIPTIONCONTENTID            = 0xDC8F,
 *      OBJECTPROPCODE_PRODUCEDBY                       = 0xDC90,
 *      OBJECTPROPCODE_USECOUNT                         = 0xDC91,
 *      OBJECTPROPCODE_SKIPCOUNT                        = 0xDC92,
 *      OBJECTPROPCODE_LASTACCESSED                     = 0xDC93,
 *      OBJECTPROPCODE_PARENTALRATING                   = 0xDC94,
 *      OBJECTPROPCODE_METAGENRE                        = 0xDC95,
 *      OBJECTPROPCODE_COMPOSER                         = 0xDC96,
 *      OBJECTPROPCODE_EFFECTIVERATING                  = 0xDC97,
 *      OBJECTPROPCODE_SUBTITLE                         = 0xDC98,
 *      OBJECTPROPCODE_ORIGINALRELEASEDATE              = 0xDC99,
 *      OBJECTPROPCODE_ALBUMNAME                        = 0xDC9A,
 *      OBJECTPROPCODE_ALBUMARTIST                      = 0xDC9B,
 *      OBJECTPROPCODE_MOOD                             = 0xDC9C,
 *      OBJECTPROPCODE_DRMSTATUS                        = 0xDC9D,
 *      OBJECTPROPCODE_SUBDESCRIPTION                   = 0xDC9E,
 *      OBJECTPROPCODE_ISCROPPED                        = 0xDCD1,
 *      OBJECTPROPCODE_ISCOLOURCORRECTED                = 0xDCD2,
 *      OBJECTPROPCODE_TOTALBITRATE                     = 0xDE91,
 *      OBJECTPROPCODE_BITRATETYPE                      = 0xDE92,
 *      OBJECTPROPCODE_SAMPLERATE                       = 0xDE93,
 *      OBJECTPROPCODE_NUMBEROFCHANNELS                 = 0xDE94,
 *      OBJECTPROPCODE_AUDIOBITDEPTH                    = 0xDE95,
 *      OBJECTPROPCODE_BLOCKALIGNMENT                   = 0xDE96,
 *      OBJECTPROPCODE_SCANTYPE                         = 0xDE97,
 *      OBJECTPROPCODE_COLOURRANGE                      = 0xDE98,
 *      OBJECTPROPCODE_AUDIOFORMATCODE                  = 0xDE99,
 *      OBJECTPROPCODE_AUDIOBITRATE                     = 0xDE9A,
 *      OBJECTPROPCODE_VIDEOFORMATCODE                  = 0xDE9B,
 *      OBJECTPROPCODE_VIDEOBITRATE                     = 0xDE9C,
 *      OBJECTPROPCODE_FRAMESTHOUSANDSECONDS            = 0xDE9D,
 *      OBJECTPROPCODE_KEYFRAMEDISTANCE                 = 0xDE9E,
 *      OBJECTPROPCODE_BUFFERSIZE                       = 0xDE9F,
 *      OBJECTPROPCODE_QUALITY                          = 0xDEA0
 *  };
 *  @endcode
 *  @li@c  OBJECTPROPCODE_NOTUSED   -Specifies the MTP device object prop code of not used.
 *  @li@c  OBJECTPROPCODE_ALL    -Specifies the MTP device object prop code of all.
 *  @li@c  OBJECTPROPCODE_UNDEFINED  -Specifies the MTP device object prop code of undefined.
 *  @li@c  OBJECTPROPCODE_STORAGEID  -Specifies the MTP device object prop code of storage id.
 *  @li@c  OBJECTPROPCODE_OBJECTFORMAT    -Specifies the MTP device object prop code of object format.
 *  @li@c  OBJECTPROPCODE_PROTECTIONSTATUS -Specifies the MTP device object prop code of protection status.
 *  @li@c  OBJECTPROPCODE_OBJECTSIZE   -Specifies the MTP device object prop code of object size.
 *  @li@c  OBJECTPROPCODE_ASSOCIATIONTYPE    -Specifies the MTP device object prop code of association type.
 *  @li@c  OBJECTPROPCODE_ASSOCIATIONDESC   -Specifies the MTP device object prop code of association description.
 *  @li@c  OBJECTPROPCODE_OBJECTFILENAME   -Specifies the MTP device object prop code of object file name.
 *  @li@c  OBJECTPROPCODE_DATECREATED    -Specifies the MTP device object prop code of date created.
 *  @li@c  OBJECTPROPCODE_DATEMODIFIED     -Specifies the MTP device object prop code of date modified.
 *  @li@c  OBJECTPROPCODE_KEYWORDS   -Specifies the MTP device object prop code of keywords.
 *  @li@c  OBJECTPROPCODE_PARENT   -Specifies the MTP device object prop code of parent.
 *  @li@c  OBJECTPROPCODE_PERSISTENTUNIQUEOBJECTIDENTIFIER  -Specifies the MTP device object prop code of persistent unique object identifier.
 *  @li@c  OBJECTPROPCODE_SYNCID     -Specifies the MTP device object prop code of sync id.
 *  @li@c  OBJECTPROPCODE_PROPERTYBAG    -Specifies the MTP device object prop code of property bag.
 *  @li@c  OBJECTPROPCODE_NAME     -Specifies the MTP device object prop code of name.
 *  @li@c  OBJECTPROPCODE_CREATEDBY    -Specifies the MTP device object prop code of created by.
 *  @li@c  OBJECTPROPCODE_ARTIST      -Specifies the MTP device object prop code of artist.
 *  @li@c  OBJECTPROPCODE_DATEAUTHORED   -Specifies the MTP device object prop code of date authored.
 *  @li@c  OBJECTPROPCODE_DESCRIPTION    -Specifies the MTP device object prop code of description.
 *  @li@c  OBJECTPROPCODE_URLREFERENCE    -Specifies the MTP device object prop code of URL reference.
 *  @li@c  OBJECTPROPCODE_LANGUAGELOCALE     -Specifies the MTP device object prop code of language locale.
 *  @li@c  OBJECTPROPCODE_COPYRIGHTINFORMATION    -Specifies the MTP device object prop code of copy right information.
 *  @li@c  OBJECTPROPCODE_SOURCE    -Specifies the MTP device object prop code of source .
 *  @li@c  OBJECTPROPCODE_ORIGINLOCATION    -Specifies the MTP device object prop code of origin location.
 *  @li@c  OBJECTPROPCODE_DATEADDED     -Specifies the MTP device object prop code of date added.
 *  @li@c  OBJECTPROPCODE_NONCONSUMABLE     -Specifies the MTP device object prop code of non consumable.
 *  @li@c  OBJECTPROPCODE_CORRUPTUNPLAYABLE    -Specifies the MTP device object prop code of corrupt unplayable.
 *  @li@c  OBJECTPROPCODE_REPRESENTATIVESAMPLEFORMAT    -Specifies the MTP device object prop code of representative sample format.
 *  @li@c  OBJECTPROPCODE_REPRESENTATIVESAMPLESIZE   -Specifies the MTP device object prop code of representative sample size.
 *  @li@c  OBJECTPROPCODE_REPRESENTATIVESAMPLEHEIGHT    -Specifies the MTP device object prop code of representative sample height.
 *  @li@c  OBJECTPROPCODE_REPRESENTATIVESAMPLEWIDTH    -Specifies the MTP device object prop code of representative sample width.
 *  @li@c  OBJECTPROPCODE_REPRESENTATIVESAMPLEDURATION  -Specifies the MTP device object prop code of representative sample duration.
 *  @li@c  OBJECTPROPCODE_REPRESENTATIVESAMPLEDATA    -Specifies the MTP device object prop code of representative sample data.
 *  @li@c  OBJECTPROPCODE_WIDTH    -Specifies the MTP device object prop code of width.
 *  @li@c  OBJECTPROPCODE_HEIGHT   -Specifies the MTP device object prop code of height.
 *  @li@c  OBJECTPROPCODE_DURATION    -Specifies the MTP device object prop code of duration.
 *  @li@c  OBJECTPROPCODE_USERRATING  -Specifies the MTP device object prop code of user rating.
 *  @li@c  OBJECTPROPCODE_TRACK   -Specifies the MTP device object prop code of track.
 *  @li@c  OBJECTPROPCODE_GENRE   -Specifies the MTP device object prop code of genre.
 *  @li@c  OBJECTPROPCODE_CREDITS    -Specifies the MTP device object prop code of credits
 *  @li@c  OBJECTPROPCODE_LYRICS    -Specifies the MTP device object prop code of lyrics.
 *  @li@c  OBJECTPROPCODE_SUBSCRIPTIONCONTENTID    -Specifies the MTP device object prop code of subscription content id.
 *  @li@c  OBJECTPROPCODE_PRODUCEDBY     -Specifies the MTP device object prop code of produced by.
 *  @li@c  OBJECTPROPCODE_USECOUNT     -Specifies the MTP device object prop code of use count.
 *  @li@c  OBJECTPROPCODE_SKIPCOUNT     -Specifies the MTP device object prop code of skip count.
 *  @li@c  OBJECTPROPCODE_LASTACCESSED    -Specifies the MTP device object prop code of last accessed.
 *  @li@c  OBJECTPROPCODE_PARENTALRATING    -Specifies the MTP device object prop code of parental rating.
 *  @li@c  OBJECTPROPCODE_METAGENRE    -Specifies the MTP device object prop code of meta genre.
 *  @li@c  OBJECTPROPCODE_COMPOSER      -Specifies the MTP device object prop code of composer.
 *  @li@c  OBJECTPROPCODE_EFFECTIVERATING   -Specifies the MTP device object prop code of effective rating.
 *  @li@c  OBJECTPROPCODE_SUBTITLE    -Specifies the MTP device object prop code of subtitle.
 *  @li@c  OBJECTPROPCODE_ORIGINALRELEASEDATE   -Specifies the MTP device object prop code of original release date.
 *  @li@c  OBJECTPROPCODE_ALBUMNAME     -Specifies the MTP device object prop code of album name.
 *  @li@c  OBJECTPROPCODE_ALBUMARTIST   -Specifies the MTP device object prop code of album artist.
 *  @li@c  OBJECTPROPCODE_MOOD      -Specifies the MTP device object prop code of mood.
 *  @li@c  OBJECTPROPCODE_DRMSTATUS     -Specifies the MTP device object prop code of DRM status.
 *  @li@c  OBJECTPROPCODE_SUBDESCRIPTION     -Specifies the MTP device object prop code of sub description.
 *  @li@c  OBJECTPROPCODE_ISCROPPED   -Specifies the MTP device object prop code of is cropped.
 *  @li@c  OBJECTPROPCODE_ISCOLOURCORRECTED   -Specifies the MTP device object prop code of is color corrected. 
 *  @li@c  OBJECTPROPCODE_TOTALBITRATE    -Specifies the MTP device object prop code of total bit rate.
 *  @li@c  OBJECTPROPCODE_BITRATETYPE    -Specifies the MTP device object prop code of bit rate type.
 *  @li@c  OBJECTPROPCODE_SAMPLERATE     -Specifies the MTP device object prop code of sample rate.
 *  @li@c  OBJECTPROPCODE_NUMBEROFCHANNELS   -Specifies the MTP device object prop code of number of channels.
 *  @li@c  OBJECTPROPCODE_AUDIOBITDEPTH    -Specifies the MTP device object prop code of audio bit depth.
 *  @li@c  OBJECTPROPCODE_BLOCKALIGNMENT    -Specifies the MTP device object prop code of block alignment.
 *  @li@c  OBJECTPROPCODE_SCANTYPE    -Specifies the MTP device object prop code of scan type.
 *  @li@c  OBJECTPROPCODE_COLOURRANGE       -Specifies the MTP device object prop code of color range.
 *  @li@c  OBJECTPROPCODE_AUDIOFORMATCODE    -Specifies the MTP device object prop code of audio format code.
 *  @li@c  OBJECTPROPCODE_AUDIOBITRATE    -Specifies the MTP device object prop code of audio bit rate.
 *  @li@c  OBJECTPROPCODE_VIDEOFORMATCODE    -Specifies the MTP device object prop code of video format code.
 *  @li@c  OBJECTPROPCODE_VIDEOBITRATE   -Specifies the MTP device object prop code of video bit rate.
 *  @li@c  OBJECTPROPCODE_FRAMESTHOUSANDSECONDS   -Specifies the MTP device object prop code of frames thousand seconds.
 *  @li@c  OBJECTPROPCODE_KEYFRAMEDISTANCE    -Specifies the MTP device object prop code of key frame distance.
 *  @li@c  OBJECTPROPCODE_BUFFERSIZE    -Specifies the MTP device object prop code of buffer size.
 *  @li@c  OBJECTPROPCODE_QUALITY    -Specifies the MTP device object prop code of quality.
 */
/*------------------------------------------------------------------*/
enum _OBJECTPROPCODE
{
    OBJECTPROPCODE_NOTUSED                          = 0x0000,
    OBJECTPROPCODE_ALL                              = 0xFFFF,
    OBJECTPROPCODE_UNDEFINED                        = 0xD000,
    OBJECTPROPCODE_STORAGEID                        = 0xDC01,
    OBJECTPROPCODE_OBJECTFORMAT                     = 0xDC02,
    OBJECTPROPCODE_PROTECTIONSTATUS                 = 0xDC03,
    OBJECTPROPCODE_OBJECTSIZE                       = 0xDC04,
    OBJECTPROPCODE_ASSOCIATIONTYPE                  = 0xDC05,
    OBJECTPROPCODE_ASSOCIATIONDESC                  = 0xDC06,
    OBJECTPROPCODE_OBJECTFILENAME                   = 0xDC07,
    OBJECTPROPCODE_DATECREATED                      = 0xDC08,
    OBJECTPROPCODE_DATEMODIFIED                     = 0xDC09,
    OBJECTPROPCODE_KEYWORDS                         = 0xDC0A,
    OBJECTPROPCODE_PARENT                           = 0xDC0B,
    OBJECTPROPCODE_PERSISTENTUNIQUEOBJECTIDENTIFIER = 0xDC41,
    OBJECTPROPCODE_SYNCID                           = 0xDC42,
    OBJECTPROPCODE_PROPERTYBAG                      = 0xDC43,
    OBJECTPROPCODE_NAME                             = 0xDC44,
    OBJECTPROPCODE_CREATEDBY                        = 0xDC45,
    OBJECTPROPCODE_ARTIST                           = 0xDC46,
    OBJECTPROPCODE_DATEAUTHORED                     = 0xDC47,
    OBJECTPROPCODE_DESCRIPTION                      = 0xDC48,
    OBJECTPROPCODE_URLREFERENCE                     = 0xDC49,
    OBJECTPROPCODE_LANGUAGELOCALE                   = 0xDC4A,
    OBJECTPROPCODE_COPYRIGHTINFORMATION             = 0xDC4B,
    OBJECTPROPCODE_SOURCE                           = 0xDC4C,
    OBJECTPROPCODE_ORIGINLOCATION                   = 0xDC4D,
    OBJECTPROPCODE_DATEADDED                        = 0xDC4E,
    OBJECTPROPCODE_NONCONSUMABLE                    = 0xDC4F,
    OBJECTPROPCODE_CORRUPTUNPLAYABLE                = 0xDC4F,
    OBJECTPROPCODE_REPRESENTATIVESAMPLEFORMAT       = 0xDC81,
    OBJECTPROPCODE_REPRESENTATIVESAMPLESIZE         = 0xDC82,
    OBJECTPROPCODE_REPRESENTATIVESAMPLEHEIGHT       = 0xDC83,
    OBJECTPROPCODE_REPRESENTATIVESAMPLEWIDTH        = 0xDC84,
    OBJECTPROPCODE_REPRESENTATIVESAMPLEDURATION     = 0xDC85,
    OBJECTPROPCODE_REPRESENTATIVESAMPLEDATA         = 0xDC86,
    OBJECTPROPCODE_WIDTH                            = 0xDC87,
    OBJECTPROPCODE_HEIGHT                           = 0xDC88,
    OBJECTPROPCODE_DURATION                         = 0xDC89,
    OBJECTPROPCODE_USERRATING                       = 0xDC8A,
    OBJECTPROPCODE_TRACK                            = 0xDC8B,
    OBJECTPROPCODE_GENRE                            = 0xDC8C,
    OBJECTPROPCODE_CREDITS                          = 0xDC8D,
    OBJECTPROPCODE_LYRICS                           = 0xDC8E,
    OBJECTPROPCODE_SUBSCRIPTIONCONTENTID            = 0xDC8F,
    OBJECTPROPCODE_PRODUCEDBY                       = 0xDC90,
    OBJECTPROPCODE_USECOUNT                         = 0xDC91,
    OBJECTPROPCODE_SKIPCOUNT                        = 0xDC92,
    OBJECTPROPCODE_LASTACCESSED                     = 0xDC93,
    OBJECTPROPCODE_PARENTALRATING                   = 0xDC94,
    OBJECTPROPCODE_METAGENRE                        = 0xDC95,
    OBJECTPROPCODE_COMPOSER                         = 0xDC96,
    OBJECTPROPCODE_EFFECTIVERATING                  = 0xDC97,
    OBJECTPROPCODE_SUBTITLE                         = 0xDC98,
    OBJECTPROPCODE_ORIGINALRELEASEDATE              = 0xDC99,
    OBJECTPROPCODE_ALBUMNAME                        = 0xDC9A,
    OBJECTPROPCODE_ALBUMARTIST                      = 0xDC9B,
    OBJECTPROPCODE_MOOD                             = 0xDC9C,
    OBJECTPROPCODE_DRMSTATUS                        = 0xDC9D,
    OBJECTPROPCODE_SUBDESCRIPTION                   = 0xDC9E,
    OBJECTPROPCODE_ISCROPPED                        = 0xDCD1,
    OBJECTPROPCODE_ISCOLOURCORRECTED                = 0xDCD2,
    OBJECTPROPCODE_TOTALBITRATE                     = 0xDE91,
    OBJECTPROPCODE_BITRATETYPE                      = 0xDE92,
    OBJECTPROPCODE_SAMPLERATE                       = 0xDE93,
    OBJECTPROPCODE_NUMBEROFCHANNELS                 = 0xDE94,
    OBJECTPROPCODE_AUDIOBITDEPTH                    = 0xDE95,
    OBJECTPROPCODE_BLOCKALIGNMENT                   = 0xDE96,
    OBJECTPROPCODE_SCANTYPE                         = 0xDE97,
    OBJECTPROPCODE_COLOURRANGE                      = 0xDE98,
    OBJECTPROPCODE_AUDIOFORMATCODE                  = 0xDE99,
    OBJECTPROPCODE_AUDIOBITRATE                     = 0xDE9A,
    OBJECTPROPCODE_VIDEOFORMATCODE                  = 0xDE9B,
    OBJECTPROPCODE_VIDEOBITRATE                     = 0xDE9C,
    OBJECTPROPCODE_FRAMESTHOUSANDSECONDS            = 0xDE9D,
    OBJECTPROPCODE_KEYFRAMEDISTANCE                 = 0xDE9E,
    OBJECTPROPCODE_BUFFERSIZE                       = 0xDE9F,
    OBJECTPROPCODE_QUALITY                          = 0xDEA0
};

/** Values for PROPGETSET_T DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _PROPGETSET
 *  @brief  This enumeration specifies the MTP device prop get set.
 *  @code
 *  enum _PROPGETSET
 *  {
 *      PROPGETSET_GETONLY  = 0x00,
 *      PROPGETSET_GETSET   = 0x01
 *  };
 *  @endcode
 *  @li@c  PROPGETSET_GETONLY    -Specifies the MTP device prop get set of get only.
 *  @li@c  PROPGETSET_GETSET     -Specifies the MTP device prop get set of get set.
 */
/*------------------------------------------------------------------*/
enum _PROPGETSET
{
    PROPGETSET_GETONLY  = 0x00,
    PROPGETSET_GETSET   = 0x01
};

/** Values for FORMFLAGS DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _FORMFLAGS
 *  @brief  This enumeration specifies the MTP device form flags.
 *  @code
 *  enum _FORMFLAGS
 *  {
 *      FORMFLAGSNONE      = 0,
 *      FORMFLAGSRANGE     = 1,
 *      FORMFLAGSENUM      = 2
 *  };
 *  @endcode
 *  @li@c  FORMFLAGSNONE      -Specifies the MTP device form flags of none.
 *  @li@c  FORMFLAGSRANGE    -Specifies the MTP device form flags of range.
 *  @li@c  FORMFLAGSENUM   -Specifies the MTP device form flags of number.
 */
/*------------------------------------------------------------------*/
enum _FORMFLAGS
{
    FORMFLAGSNONE      = 0,
    FORMFLAGSRANGE     = 1,
    FORMFLAGSENUM      = 2
};

/** Values for PROPERTYGROUP_T DATATYPE
 *
 */
/*------------------------------------------------------------------*/
/*! @enum _PROPERTYGROUP
 *  @brief  This enumeration specifies the MTP device property group.
 *  @code
 *  enum _PROPERTYGROUP
 *  {
 *      PROPERTYGROUP_UNDEFINED     = 0x0,
 *      PROPERTYGROUP_NOTUSED       = 0x0
 *  };
 *  @endcode
 *  @li@c  PROPERTYGROUP_UNDEFINED    -Specifies the MTP device property group of undefined.
 *  @li@c  PROPERTYGROUP_NOTUSED   -Specifies the MTP device property group of not used.
 */
/*------------------------------------------------------------------*/
enum _PROPERTYGROUP
{
    PROPERTYGROUP_UNDEFINED     = 0x0,
    PROPERTYGROUP_NOTUSED       = 0x0
};

/** Values for HACKMODEL DATATYPE
 *  @note 
 *  Right now, Sony devices do not provide model in device info. m_HackVersion is used to
 *  differentiate newer and older firmware of Sony devices
 */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief  This enumeration specifies the MTP device hack model.
 *  @code
 *  enum
 *  {
 *      HACKMODEL_NONE          = 0,
 *      HACKMODEL_DC4800        = 1,
 *      HACKMODEL_NIKON_E2500   = 2,
 *      HACKMODEL_SONY          = 3,
 *      HACKMODEL_KODAK_MC3     = 4,
 *      HACKMODEL_SIGMATEL128   = 5,
 *      HACKMODEL_CANON         = 6
 *  };
 *  @endcode
 *  @li@c  HACKMODEL_NONE      -Specifies the MTP device hack model of none.
 *  @li@c  HACKMODEL_DC4800   -Specifies the MTP device hack model of DC4800.
 *  @li@c  HACKMODEL_NIKON_E2500  -Specifies the MTP device hack model of NIKON E2500.
 *  @li@c  HACKMODEL_SONY   -Specifies the MTP device hack model of SONY.
 *  @li@c  HACKMODEL_KODAK_MC3   -Specifies the MTP device hack model of KODAK MC3.
 *  @li@c  HACKMODEL_SIGMATEL128  -Specifies the MTP device hack model of SIGMATEL128.
 *  @li@c  HACKMODEL_CANON   -Specifies the MTP device hack model of CANON.
 */
/*------------------------------------------------------------------*/
enum
{
    HACKMODEL_NONE          = 0,
    HACKMODEL_DC4800        = 1,
    HACKMODEL_NIKON_E2500   = 2,
    HACKMODEL_SONY          = 3,
    HACKMODEL_KODAK_MC3     = 4,
    HACKMODEL_SIGMATEL128   = 5,
    HACKMODEL_CANON         = 6
};

/*------------------------------------------------------------------*/
/*! @brief  This function is called when free an object.
 *  @param [in]     pv_obj    Specify the object to be freed.
 *  @param [out]    -
 *  @note
 *  @see
 *  @return     -
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*mtp_obj_free_fct)(VOID *pv_obj);

/*------------------------------------------------------------------*/
/*! @struct MTP_INT128_T
 *  @brief  This structure realize a long type of signed number which include 128 bits.
 *  @code
 *  typedef struct { 
 *      INT32 ai4_part[4];
 *  }MTP_INT128_T;
 *  @endcode
 *  @li@c  ai4_part[4]           -array to store 128 bits' signed number.
 */
/*------------------------------------------------------------------*/
typedef struct { 
    INT32 ai4_part[4];
}MTP_INT128_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_UINT128_T
 *  @brief  This structure realize a long type of unsigned number which include 128 bits.
 *  @code
 *  typedef struct { 
 *      UINT32 aui4_part[4];
 *  }MTP_UINT128_T;
 *  @endcode
 *  @li@c  aui4_part[4]        -array to store 128 bits' unsigned number.
 */
/*------------------------------------------------------------------*/
typedef struct { 
    UINT32 aui4_part[4];
}MTP_UINT128_T;

typedef union _U_MTP_VARIANTVALUE_T
{
    INT8            i1_int8;    /**<   BYTE     */
    UINT8           ui1_uint8;
    INT16           i2_int16;
    UINT16          ui2_uint16;
    INT32           i4_int32;
    UINT32          ui4_uint32;
    INT64           i8_int64;
    UINT64          ui8_uint64;
    MTP_INT128_T    t_int128;
    MTP_UINT128_T   t_uint128;
    UTF16_T*        w2s_str;
    INT8*           ai1_int8;
    UINT8*          aui1_uint8;
    INT16*          ai2_int16;
    UINT16*         aui2_uint16;
    INT32*          ai4_int32;
    UINT32*         aui4_uint32;
    INT64*          ai8_int64;
    UINT64*         aui8_uint64;
    MTP_INT128_T*   at_int128;
    MTP_UINT128_T*  at_uint128;
}U_MTP_VARIANTVALUE_T;


/*------------------------------------------------------------------*/
/*! @struct MTP_VARIANT_T
 *  @brief  This structure include variant values.
 *  @code
 *  typedef struct _MTP_VARIANT_T
 *  {
 *      UINT16                  ui2_type; 
 *      UINT32                  ui4_element_size;
 *      UINT32                  ui4_num_elements;
 *      U_MTP_VARIANTVALUE_T    u_v;
 *  } MTP_VARIANT_T;
 *  @endcode
 *  @li@c  ui2_type                -Specify data type.
 *  @li@c  ui4_element_size    -Specify element size.
 *  @li@c  ui4_num_elements    -Specify element number.
 *  @li@c  u_v      -Specify variant value.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_VARIANT_T
{
    UINT16                  ui2_type; /*_DATATYPE */
    UINT32                  ui4_element_size;
    UINT32                  ui4_num_elements;
    U_MTP_VARIANTVALUE_T    u_v;
} MTP_VARIANT_T;

/** holds property lists for use with GetObjectPropList, etc.
 *
 */
/*------------------------------------------------------------------*/
/*! @struct PROPLIST_ITEM_T
 *  @brief  This structure hold an item's prop list info.
 *  @code
 *  typedef struct _PROPLIST_ITEM_T
 *  {
 *      UINT32              ui4_object_handle;
 *      UINT16              ui2_prop_code; 
 *      UINT16              ui2_data_type; 
 *      MTP_VARIANT_T       t_prop_value;
 *  } PROPLIST_ITEM_T;
 *  @endcode
 *  @li@c  ui4_object_handle   -Specify object handle.
 *  @li@c  ui2_prop_code     -Specify prop code.
 *  @li@c  ui2_data_type      -Specify data type.
 *  @li@c  t_prop_value      -Specify prop value.
 */
/*------------------------------------------------------------------*/
typedef struct _PROPLIST_ITEM_T
{
    UINT32              ui4_object_handle;
    UINT16              ui2_prop_code; /*_OBJECTPROPCODE*/
    UINT16              ui2_data_type; /*_DATATYPE */
    MTP_VARIANT_T       t_prop_value;
} PROPLIST_ITEM_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_COMMAND_T
 *  @brief  This structure hold a command.
 *  @code
 *  typedef struct _MTP_COMMAND_T
 *  {
 *      UINT16      ui2_opcode; 
 *      UINT32      ui4_transaction_id;
 *      UINT32      ui4_nparams;
 *      UINT32      aui4_params[COMMAND_NUMPARAMS_MAX];
 *  } MTP_COMMAND_T;
 *  @endcode
 *  @li@c  ui2_opcode             -Specify operation code of a command.
 *  @li@c  ui4_transaction_id    -Specify transaction id of a command.
 *  @li@c  ui4_nparams     -Specify parameter numbers of a command.
 *  @li@c  aui4_params[COMMAND_NUMPARAMS_MAX]   -Specify parameters of a command.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_COMMAND_T
{
    UINT16      ui2_opcode; /*_OPCODE */
    UINT32      ui4_transaction_id;
    UINT32      ui4_nparams;
    UINT32      aui4_params[COMMAND_NUMPARAMS_MAX];
} MTP_COMMAND_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_RESPONSE_T
 *  @brief  This structure hold a response.
 *  @code
 *  typedef struct _MTP_RESPONSE_T
 *  {
 *      UINT16          ui2_response_code; 
 *      UINT32          ui4_transaction_id;
 *      UINT32          ui4_nparams;
 *      UINT32          ui4_params[RESPONSE_NUMPARAMS_MAX];
 *  } MTP_RESPONSE_T;
 *  @endcode
 *  @li@c  ui2_response_code      -Specify response code of a response.
 *  @li@c  ui4_transaction_id     -Specify transaction id of a response.
 *  @li@c  ui4_nparams       -Specify parameter numbers of a response.
 *  @li@c  ui4_params[RESPONSE_NUMPARAMS_MAX]    -Specify parameters of a response.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_RESPONSE_T
{
    UINT16          ui2_response_code; /*_RESPONSECODE */
    UINT32          ui4_transaction_id;
    UINT32          ui4_nparams;
    UINT32          ui4_params[RESPONSE_NUMPARAMS_MAX];
} MTP_RESPONSE_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_EVENT
 *  @brief  This structure hold an event.
 *  @code
 *  typedef struct _MTP_EVENT
 *  {
 *      UINT16      ui2_event_code; 
 *      UINT32      ui4_transaction_id;
 *      UINT32      ui4_nparams;
 *      UINT32      ui4_params[EVENT_NUMPARAMS_MAX];
 *  } MTP_EVENT;
 *  @endcode
 *  @li@c  ui2_event_code       -Specify event code of an event.
 *  @li@c  ui4_transaction_id    -Specify transaction id of an event.
 *  @li@c  ui4_nparams       -Specify parameter numbers of an event.
 *  @li@c  ui4_params[EVENT_NUMPARAMS_MAX]    -Specify parameters of an event.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_EVENT
{
    UINT16      ui2_event_code; /*_EVENTCODE */
    UINT32      ui4_transaction_id;
    UINT32      ui4_nparams;
    UINT32      ui4_params[EVENT_NUMPARAMS_MAX];
} MTP_EVENT;

/*------------------------------------------------------------------*/
/*! @struct MTP_DEVICE_INFO_T
 *  @brief  This type specifies the data phase content of device info.
 *  @code
 *  typedef struct _MTP_DEVICE_INFO_T
 *  {
 *      UINT16              ui2_version;     
 *      UINT32              ui4_vendor_extId;     
 *      UINT16              ui2_vendor_ext_version;
 *      UTF16_T*            w2s_vendor_ext_desc;
 *      UINT16              ui2_funcMode;                   
 *      UINT32              ui4_nsupported_ops;
 *      UINT16*             aui2_supportedOps;             
 *      UINT32              ui4_nsupported_events;
 *      UINT16*             aui2_supported_events;          
 *      UINT32              ui4_nsupported_props;
 *      UINT16*             aui2_supported_props;           
 *      UINT32              ui4_nsupported_capture_fmts;
 *      UINT16*             aui2_supported_capture_fmts;    
 *      UINT32              ui4_nsupported_object_fmts;
 *      UINT16*             aui2_supported_object_fmts;     
 *      UTF16_T*            w2s_manufacturer;
 *      UTF16_T*            w2s_model;
 *      UTF16_T*            w2s_device_version;
 *      UTF16_T*            w2s_serial_number;
 *  } MTP_DEVICE_INFO_T;
 *  @endcode
 *  @li@c  ui2_version            -Specify device version.
 *  @li@c  ui4_vendor_extId       -Specify device vendor extern id.
 *  @li@c  ui2_vendor_ext_version    -Specify device vendor extern version.
 *  @li@c  w2s_vendor_ext_desc    -Specify device vendor extern desc.
 *  @li@c  ui2_funcMode        -Specify device function mode.
 *  @li@c  ui4_nsupported_ops    -Specify device supported operations' number.
 *  @li@c  aui2_supportedOps     -Specify device supported operations.
 *  @li@c  ui4_nsupported_events     -Specify device supported events' number.
 *  @li@c  aui2_supported_events    -Specify device supported events.
 *  @li@c  ui4_nsupported_props      -Specify device supported props' number.
 *  @li@c  aui2_supported_props      -Specify device supported props.
 *  @li@c  ui4_nsupported_capture_fmts    -Specify device supported capture formats' number.
 *  @li@c  aui2_supported_capture_fmts    -Specify device supported capture formats.
 *  @li@c  ui4_nsupported_object_fmts      -Specify device supported object formats' number.
 *  @li@c  aui2_supported_object_fmts       -Specify device supported object formats.
 *  @li@c  w2s_manufacturer       -Specify device manufacturer.
 *  @li@c  w2s_model         -Specify device model.
 *  @li@c  w2s_device_version        -Specify device version name.
 *  @li@c  w2s_serial_number        -Specify device serial number.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_DEVICE_INFO_T
{
    UINT16              ui2_version;     
    UINT32              ui4_vendor_extId;     
    UINT16              ui2_vendor_ext_version;
    UTF16_T*            w2s_vendor_ext_desc;
    UINT16              ui2_funcMode;                   /*_FUNCTIONALMODE */
    
    UINT32              ui4_nsupported_ops;
    UINT16*             aui2_supportedOps;             /*_OPCODE */

    UINT32              ui4_nsupported_events;
    UINT16*             aui2_supported_events;          /*_RESPONSECODE */       

    UINT32              ui4_nsupported_props;
    UINT16*             aui2_supported_props;           /*_DEVICEPROPCODE*/

    UINT32              ui4_nsupported_capture_fmts;
    UINT16*             aui2_supported_capture_fmts;    /*_FORMATCODE*/

    UINT32              ui4_nsupported_object_fmts;
    UINT16*             aui2_supported_object_fmts;     /*_FORMATCODE*/
    
    UTF16_T*            w2s_manufacturer;
    UTF16_T*            w2s_model;
    UTF16_T*            w2s_device_version;
    UTF16_T*            w2s_serial_number;
} MTP_DEVICE_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_STORAGE_INFO_T
 *  @brief  This type specifies the data phase content of storage info.
 *  @code
 *  typedef struct _MTP_STORAGE_INFO_T
 *  {
 *      UINT32              ui4_storage_id;             
 *      UINT16              ui2_storage_type;           
 *      UINT16              ui2_file_system_type;       
 *      UINT16              ui2_access_capability;      
 *      UINT64              ui8_max_capacity;           
 *      UINT64              ui8_free_space_in_bytes;    
 *      UINT32              ui4_free_space_in_objects;  
 *      UTF16_T*            w2s_storage_desc;           
 *      UTF16_T*            w2s_storage_label;          
 *  } MTP_STORAGE_INFO_T;
 *  @endcode
 *  @li@c  ui4_storage_id                  -Specify the "id" for this store
 *  @li@c  ui2_storage_type              -Specify storage type, see _STORAGETYPE
 *  @li@c  ui2_file_system_type         -Specify file system type, see _FILESYSTEMTYPE 
 *  @li@c  ui2_access_capability         -Specify access capability
 *  @li@c  ui8_max_capacity              -Specify maximum capacity in bytes
 *  @li@c  ui8_free_space_in_bytes     -Specify free space in bytes 
 *  @li@c  ui4_free_space_in_objects  -Specify free space in objects
 *  @li@c  w2s_storage_desc          -Specify description
 *  @li@c  w2s_storage_label          -Specify volume label 
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_STORAGE_INFO_T
{
    UINT32              ui4_storage_id;             /* the "id" for this store */
    UINT16              ui2_storage_type;           /* storage type, see _STORAGETYPE */
    UINT16              ui2_file_system_type;       /* file system type, see _FILESYSTEMTYPE */
    UINT16              ui2_access_capability;      /* access capability (e.g. read/write), see _STORAGEACCESSCAPABILITY */
    UINT64              ui8_max_capacity;           /* maximum capacity in bytes */
    UINT64              ui8_free_space_in_bytes;    /* free space in bytes */
    UINT32              ui4_free_space_in_objects;  /* free space in objects */
    UTF16_T*            w2s_storage_desc;           /* description */
    UTF16_T*            w2s_storage_label;          /* volume label */
} MTP_STORAGE_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct MTP_OBJECT_INFO_T
 *  @brief      This type specifies the data phase content of object info.
 *  @code
 *  typedef struct _MTP_OBJECT_INFO_T
 *  {
 *      UINT32              ui4_object_handle;          
 *      UINT32              ui4_storage_id;             
 *      UINT16              ui2_format_code;            
 *      UINT16              ui2_protection_status;      
 *      UINT32              ui4_compressed_size;        
 *      UINT16              ui2_thumb_format;           
 *      UINT32              ui4_thumb_compressed_size;  
 *      UINT32              ui4_thumb_pixwidth;         
 *      UINT32              ui4_thumb_pixheight;        
 *      UINT32              ui4_image_pixwidth;         
 *      UINT32              ui4_image_pixheight;        
 *      UINT32              ui4_image_bitdepth;         
 *      UINT32              ui4_parent_handle;          
 *      UINT16              ui2_association_type;       
 *      UINT32              ui4_association_desc;       
 *      UINT32              ui4_sequence_number;        
 *      UTF16_T*            w2s_file_name;              
 *      UTF16_T*            w2s_capture_date;           
 *      UTF16_T*            w2s_modification_date;      
 *      UTF16_T*            w2s_keywords;               
 *  } MTP_OBJECT_INFO_T;
 *  @endcode
 *  @li@c  ui4_object_handle                      -Specify the "handle" for this object 
 *  @li@c  ui4_storage_id                           -Specify the storage the object resides
 *  @li@c  ui2_format_code                        -Specify object format code, see _FORMATCODE
 *  @li@c  ui2_protection_status                  -Specify object protection status, see _PROTECTIONSTATUS
 *  @li@c  ui4_compressed_size                  -Specify object compressed size
 *  @li@c  ui2_thumb_format                      -Specify thumbnail format(image object only)
 *  @li@c  ui4_thumb_compressed_size        -Specify thumbnail compressed size
 *  @li@c  ui4_thumb_pixwidth               -Specify thumbnail width in pixels
 *  @li@c  ui4_thumb_pixheight             -Specify thumbnail height in pixels
 *  @li@c  ui4_image_pixwidth              -Specify image width in pixels
 *  @li@c  ui4_image_pixheight             -Specify image height in pixels
 *  @li@c  ui4_image_bitdepth              -Specify image color depth 
 *  @li@c  ui4_parent_handle                -Specify parent object handle
 *  @li@c  ui2_association_type             -Specify association type
 *  @li@c  ui4_association_desc             -Specify association description
 *  @li@c  ui4_sequence_number           -Specify sequence number
 *  @li@c  w2s_file_name                      -Specify optional file name
 *  @li@c  w2s_capture_date                  -Specify Captured date
 *  @li@c  w2s_modification_date           -Specify when it was last modified.
 *  @li@c  w2s_keywords                  -Specify optional keywords
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_OBJECT_INFO_T
{
    UINT32              ui4_object_handle;          /* the "handle" for this object */
    UINT32              ui4_storage_id;             /* The storage the object resides */
    UINT16              ui2_format_code;            /* object format code, see _FORMATCODE*/
    UINT16              ui2_protection_status;      /* object protection status, see _PROTECTIONSTATUS */
    UINT32              ui4_compressed_size;        /* object compressed size */
    UINT16              ui2_thumb_format;           /* thumbnail format(image object only), see _FORMATCODE */
    UINT32              ui4_thumb_compressed_size;  /* thumbnail compressed size */
    UINT32              ui4_thumb_pixwidth;         /* thumbnail width in pixels */
    UINT32              ui4_thumb_pixheight;        /* thumbnail height in pixels */
    UINT32              ui4_image_pixwidth;         /* image width in pixels */
    UINT32              ui4_image_pixheight;        /* image height in pixels */
    UINT32              ui4_image_bitdepth;         /* image color depth */
    UINT32              ui4_parent_handle;          /* parent object handle */
    UINT16              ui2_association_type;       /* association type, see _ASSOCIATIONTYPE */
    UINT32              ui4_association_desc;       /* association description */
    UINT32              ui4_sequence_number;        /* sequence number */
    UTF16_T*            w2s_file_name;              /* optional file name */
    UTF16_T*            w2s_capture_date;           /* Captured date */
    UTF16_T*            w2s_modification_date;      /* when it was last modified. */
    UTF16_T*            w2s_keywords;               /* optional keywords */
} MTP_OBJECT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_DESC_RANGE_FORM_T
 *  @brief  This type specifies the data phase content of range form  info.
 *  @code
 *  typedef struct _MTP_DESC_RANGE_FORM_T
 *  {
 *      MTP_VARIANT_T       t_range_min;
 *      MTP_VARIANT_T       t_range_max;
 *      MTP_VARIANT_T       t_range_step;
 *  }MTP_DESC_RANGE_FORM_T;
 *  @endcode
 *  @li@c  t_range_min            -Specify min range.
 *  @li@c  t_range_max           -Specify max range.
 *  @li@c  t_range_step           -Specify range step.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_DESC_RANGE_FORM_T
{
    MTP_VARIANT_T       t_range_min;
    MTP_VARIANT_T       t_range_max;
    MTP_VARIANT_T       t_range_step;
}MTP_DESC_RANGE_FORM_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_DESC_ENUM_FORM_T
 *  @brief  This type specifies the data phase content of description form info.
 *  @code
 *  typedef struct _MTP_DESC_ENUM_FORM_T
 *  {
 *      INT16               i2_num_enum_values;
 *      MTP_VARIANT_T**     apt_enum_values;    
 *  }MTP_DESC_ENUM_FORM_T;
 *  @endcode
 *  @li@c  i2_num_enum_values    -Specify numbers of description.
 *  @li@c  apt_enum_values          -Specify description values.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_DESC_ENUM_FORM_T
{
    INT16               i2_num_enum_values;
    MTP_VARIANT_T**     apt_enum_values;    /*array*/
}MTP_DESC_ENUM_FORM_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_DEVICE_PROP_DESC_T
 *  @brief  This type specifies the data phase content of device prop description info.
 *  @code
 *  typedef struct _MTP_DEVICE_PROP_DESC_T
 *  {
 *      UINT16              ui2_prop_code;      
 *      UINT16              ui2_mtp_data_type;  
 *      UINT8               ui1_access_rights;  
 *      MTP_VARIANT_T       t_current;
 *      MTP_VARIANT_T       t_default;
 *      UINT8               ui1_form_flag;      
 *      union
 *      {
 *          MTP_DESC_RANGE_FORM_T   t_desc_range_form;
 *          MTP_DESC_ENUM_FORM_T    t_desc_enum_form;
 *      }u_form;
 *  } MTP_DEVICE_PROP_DESC_T;
 *  @endcode
 *  @li@c  ui2_prop_code               -Specify device prop code.
 *  @li@c  ui2_mtp_data_type        -Specify data type.
 *  @li@c  ui1_access_rights          -Specify access rights.
 *  @li@c  t_current                      -Specify current device info.
 *  @li@c  t_default                      -Specify default device info.
 *  @li@c  ui1_form_flag               -Specify form flags.
 *  @li@c  t_desc_range_form      -Specify range form.
 *  @li@c  t_desc_enum_form      -Specify description enumeration form.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_DEVICE_PROP_DESC_T
{
    UINT16              ui2_prop_code;      /*_DEVICEPROPCODE*/
    UINT16              ui2_mtp_data_type;  /*_DATATYPE */
    UINT8               ui1_access_rights;  /*_PROPGETSET*/

    MTP_VARIANT_T       t_current;
    MTP_VARIANT_T       t_default;

    UINT8               ui1_form_flag;      /*_FORMFLAGS*/

    union
    {
        MTP_DESC_RANGE_FORM_T   t_desc_range_form;
        MTP_DESC_ENUM_FORM_T    t_desc_enum_form;
    }u_form;
    
} MTP_DEVICE_PROP_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_DESC_DATATIME_FORM_T
 *  @brief  This type specifies the data phase content of data time.
 *  @code
 *  typedef struct _MTP_DESC_DATATIME_FORM_T
 *  {
 *      UINT32 ui4_invalid_value;
 *  }MTP_DESC_DATATIME_FORM_T;
 *  @endcode
 *  @li@c  ui4_invalid_value   -Specify invalid value.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_DESC_DATATIME_FORM_T
{
    /* invalid member for build*/
    UINT32 ui4_invalid_value;
}MTP_DESC_DATATIME_FORM_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_DESC_REGULAR_EXPRESSION_FORM
 *  @brief      This type specifies the data phase content of regular expression.
 *  @code
 *  typedef struct _MTP_DESC_REGULAR_EXPRESSION_FORM
 *  {
 *      UINT8       ui1_string_length;
 *      UTF16_T*    w2s_desc_regular_expression_form;
 *  }MTP_DESC_REGULAR_EXPRESSION_FORM;
 *  @endcode
 *  @li@c  ui1_string_length         -Specify string length.
 *  @li@c  w2s_desc_regular_expression_form    -Specify regular expression form.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_DESC_REGULAR_EXPRESSION_FORM
{
    UINT8       ui1_string_length;
    UTF16_T*    w2s_desc_regular_expression_form;
}MTP_DESC_REGULAR_EXPRESSION_FORM;

/*------------------------------------------------------------------*/
/*! @struct MTP_DESC_BYTE_ARRAY_FORM
 *  @brief      This type specifies the array of byte.
 *  @code
 *  typedef struct _MTP_DESC_BYTE_ARRAY_FORM
 *  {
 *      UINT32  ui4_num;
 *      UINT8*  aui1_byte_array;
 *  }MTP_DESC_BYTE_ARRAY_FORM;
 *  @endcode
 *  @li@c  ui4_num       -Specify byte number.
 *  @li@c  aui1_byte_array     -Specify the byte array.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_DESC_BYTE_ARRAY_FORM
{
    UINT32  ui4_num;
    UINT8*  aui1_byte_array;
}MTP_DESC_BYTE_ARRAY_FORM;

/*------------------------------------------------------------------*/
/*! @struct MTP_DESC_LONG_STRING_FORM
 *  @brief      This type specifies the long string form.
 *  @code
 *  typedef struct _MTP_DESC_LONG_STRING_FORM
 *  {
 *      UINT8       ui1_string_length;
 *      UTF16_T*    w2s_long_string_form;
 *  }MTP_DESC_LONG_STRING_FORM;
 *  @endcode
 *  @li@c  ui1_string_length       -Specify string length.
 *  @li@c  w2s_long_string_form    -Specify long string form data.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_DESC_LONG_STRING_FORM
{
    UINT8       ui1_string_length;
    UTF16_T*    w2s_long_string_form;
}MTP_DESC_LONG_STRING_FORM;

/*------------------------------------------------------------------*/
/*! @struct MTP_OBJECT_PROP_DESC_T
 *  @brief      This type specifies the object prop description info.
 *  @code
 *  typedef struct _MTP_OBJECT_PROP_DESC_T
 *  {
 *      UINT16              ui2_prop_code;      
 *      UINT16              ui2_mtp_data_type;  
 *      UINT8               ui1_access_rights;  
 *      MTP_VARIANT_T       t_default;
 *      UINT8               ui1_form_flag;      
 *      UINT32              ui4_group_code;
 *      union
 *      {
 *          MTP_DESC_RANGE_FORM_T               t_desc_range_form;
 *          MTP_DESC_ENUM_FORM_T                t_desc_enum_form;
 *          MTP_DESC_DATATIME_FORM_T            t_desc_datatime_form;
 *          UINT16                              ui2_desc_fixed_length_array_form;
 *          MTP_DESC_REGULAR_EXPRESSION_FORM    t_desc_regular_expression_form;
 *          MTP_DESC_BYTE_ARRAY_FORM            t_desc_byte_array_form;
 *          MTP_DESC_LONG_STRING_FORM           t_desc_long_string_form;
 *      }u_form;
 *  } MTP_OBJECT_PROP_DESC_T;
 *  @endcode
 *  @li@c  ui2_prop_code          -Specify object prop code.
 *  @li@c  ui2_mtp_data_type     -Specify data type.
 *  @li@c  ui1_access_rights    -Specify access rights.
 *  @li@c  t_default              -Specify default description.
 *  @li@c  ui1_form_flag        -Specify form flag.
 *  @li@c  ui4_group_code       -Specify group code.
 *  @li@c  t_desc_range_form    -Specify range form.
 *  @li@c  t_desc_enum_form      -Specify enumeration form.
 *  @li@c  t_desc_datatime_form     -Specify data time form.
 *  @li@c  ui2_desc_fixed_length_array_form    -Specify fixed length array form.
 *  @li@c  t_desc_regular_expression_form    -Specify regular expression form.
 *  @li@c  t_desc_byte_array_form   -Specify byte array form.
 *  @li@c  t_desc_long_string_form   -Specify long string form.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_OBJECT_PROP_DESC_T
{
    UINT16              ui2_prop_code;      /*_OBJECTPROPCODE*/
    UINT16              ui2_mtp_data_type;  /*_DATATYPE */
    UINT8               ui1_access_rights;  /*_PROPGETSET*/
    MTP_VARIANT_T       t_default;
    UINT8               ui1_form_flag;      /*_FORMFLAGS*/
    UINT32              ui4_group_code;

    union
    {
        MTP_DESC_RANGE_FORM_T               t_desc_range_form;
        MTP_DESC_ENUM_FORM_T                t_desc_enum_form;
        MTP_DESC_DATATIME_FORM_T            t_desc_datatime_form;
        UINT16                              ui2_desc_fixed_length_array_form;
        MTP_DESC_REGULAR_EXPRESSION_FORM    t_desc_regular_expression_form;
        MTP_DESC_BYTE_ARRAY_FORM            t_desc_byte_array_form;
        MTP_DESC_LONG_STRING_FORM           t_desc_long_string_form;
    }u_form;
    
} MTP_OBJECT_PROP_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_INTERDEPENDENT_PROP_DESC_ELEMENT_T
 *  @brief      This type specifies the interdependent prop description element info.
 *  @code
 *  typedef struct _MTP_INTERDEPENDENT_PROP_DESC_ELEMENT_T
 *  {
 *      UINT16                      ui2_num_desc;
 *      MTP_OBJECT_PROP_DESC_T**    apt_list;
 *  }MTP_INTERDEPENDENT_PROP_DESC_ELEMENT_T;
 *  @endcode
 *  @li@c  ui2_num_desc    -Specify description number.
 *  @li@c  apt_list           -Specify object prop description list.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_INTERDEPENDENT_PROP_DESC_ELEMENT_T
{
    UINT16                      ui2_num_desc;
    MTP_OBJECT_PROP_DESC_T**    apt_list;
}MTP_INTERDEPENDENT_PROP_DESC_ELEMENT_T;

/*------------------------------------------------------------------*/
/*! @struct MTP_INTERDEPENDENT_PROP_DESC_LIST_T
 *  @brief      This type specifies the interdependent prop description list info.
 *  @code
 *  typedef struct _MTP_INTERDEPENDENT_PROP_DESC_LIST_T
 *  {
 *      UINT16                                      ui2_num_interdependencies;
 *      MTP_INTERDEPENDENT_PROP_DESC_ELEMENT_T**    apt_list;
 *  }MTP_INTERDEPENDENT_PROP_DESC_LIST_T;
 *  @endcode
 *  @li@c  ui2_num_interdependencies     -Specify interdependent prop description elements' number.
 *  @li@c  apt_list          -Specify interdependent prop description list.
 */
/*------------------------------------------------------------------*/
typedef struct _MTP_INTERDEPENDENT_PROP_DESC_LIST_T
{
    UINT16                                      ui2_num_interdependencies;
    MTP_INTERDEPENDENT_PROP_DESC_ELEMENT_T**    apt_list;
}MTP_INTERDEPENDENT_PROP_DESC_LIST_T;

#endif /* _U_MTP_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                         /* end of groupMW_MTP_MNGR */
/*----------------------------------------------------------------------------*/

