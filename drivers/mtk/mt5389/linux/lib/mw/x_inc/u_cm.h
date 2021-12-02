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
/*! @file u_cm.h
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *         This header file contains Connection Manager specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CONN_MNGR
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_CM_H_
#define _U_CM_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_sys_name.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Connection Manager handle types */
#define CMT_PIPE        (HT_GROUP_CM + (HANDLE_TYPE_T) 1)        /**< Handle type definition of pipe*/
#define CMT_CONNECTION  (HT_GROUP_CM + (HANDLE_TYPE_T) 2)        /**< Handle type definition of cm connection*/


/* Pipe/Connection condition state */
typedef UINT32 CM_COND_T;    /**< Pipe/Connection condition state       */

#define CM_COND_CLOSED           ((CM_COND_T)  0)                /**< Definition of connection condition CLOSED*/
#define CM_COND_OPENING          ((CM_COND_T)  1)                /**< Definition of connection condition OPENING */
#define CM_COND_OPENED           ((CM_COND_T)  2)                /**< Definition of connection condition OPEND*/
#define CM_COND_CLOSING          ((CM_COND_T)  3)                /**< Definition of connection condition CLOSING*/
#define CM_COND_DISCONNECTED     ((CM_COND_T)  4)                /**< Definition of connection condition DISCONNECTED*/
#define CM_COND_CONNECTING       ((CM_COND_T)  5)                /**< Definition of connection condition CONNECTING*/
#define CM_COND_CONNECTED        ((CM_COND_T)  6)                /**< Definition of connection condition CONNECTED*/
#define CM_COND_DISCONNECTING    ((CM_COND_T)  7)                /**< Definition of connection condition DISCONNECTING*/
#define CM_COND_CONNECTED_TRANS  ((CM_COND_T)  8)                /**< Definition of connection condition CONNECTED TRANS*/
#define CM_COND_CONNECTION_INFO  ((CM_COND_T)  9)                /**< Definition of connection condition CONNECTING INFO*/
#define CM_COND_CONNECTION_ANA_FREQ ((CM_COND_T)  10)             /**< Definition of connection condition CONNECTING ANA INFO*/
//#ifdef MW_DVBS_DEV_ENABLE
#define CM_COND_DISEQC_CMD_DONE  ((CM_COND_T) 11)                  /**< Definition of connection condition CM_COND_DISEQC_CMD_DONE*/
//#endif

#define CM_HANDLER_COND(_x)  ((CM_COND_T) (0x80000000 + (_x)))   /**< Define the own conditions of individual cm handler*/

/* Command code */
typedef UINT32 CM_COMMAND_CODE_T;    /**< Type definition of command code       */

/*----------------------------------------------------------------------------*/
/*! @struct CM_COMMAND_T
 *  @brief  cm command structure
 *  @code
 *  typedef struct
 *  {
 *      CM_COMMAND_CODE_T  e_code;
 *      union
 *      {
 *          HANDLE_T     h_handle;
 *          UINT32       ui4_number;
 *          UINT16       ui2_number;
 *          UINT8        ui1_number;
 *          INT32        i4_number;
 *          INT16        i2_number;
 *          INT8         i1_number;
 *          BOOL         b_boolean;
 *          const CHAR*  ps_name;
 *          VOID*        pv_ptr;
 *      }   u;
 *  }   CM_COMMAND_T;
 *  @endcode
 *  @li@c e_code           - command code
 *  @li@c h_handle         - a handle type
 *  @li@c ui4_number       - a UINT32 value
 *  @li@c ui2_number       - a UINT16 value
 *  @li@c ui1_number       - a UINT8 value
 *  @li@c i4_number        - a INT32 value
 *  @li@c i2_number        - a INT16 value
 *  @li@c i1_number        - a INT8 value
 *  @li@c b_boolean        - a boolean value
 *  @li@c ps_name          - a string   value
 *  @li@c pv_ptr           - a pointer value
 */
/*------------------------------------------------------------------*/
typedef struct
{
    CM_COMMAND_CODE_T  e_code;
    union
    {
        HANDLE_T     h_handle;
        UINT32       ui4_number;
        UINT16       ui2_number;
        UINT8        ui1_number;
        INT32        i4_number;
        INT16        i2_number;
        INT8         i1_number;
        BOOL         b_boolean;
        const CHAR*  ps_name;
        VOID*        pv_ptr;
    }   u;
}   CM_COMMAND_T;

typedef struct
{
    HANDLE_T  h_rm_comp;
    VOID*     pv_stream_tag;
}   CM_CTRL_RM_SRC_COMP_T;
/* Commands from 0 to ((2^31)-1) are reserved for the Connection Manager */
/* Handler-specific commands shall start at (2^31), i.e 0x80000000 */
#define CONN_SRC_TYPE  ((CM_COMMAND_CODE_T) 1)   /**< Command code of source type*/
#define CONN_SRC_NAME  ((CM_COMMAND_CODE_T) 2)   /**< Command code of source name*/
#define CONN_SNK_NAME  ((CM_COMMAND_CODE_T) 3)   /**< Command code of sink name*/
#define CONN_END       ((CM_COMMAND_CODE_T) 0)   /**< Command code of command end*/

#define CONN_SRC_TYPE_MAX_STRLEN  SYS_NAME_LEN   /**< Maximum string length of source type*/
#define CONN_SRC_NAME_MAX_STRLEN  SYS_NAME_LEN   /**< Maximum string length of source name*/
#define CONN_SNK_NAME_MAX_STRLEN  SYS_NAME_LEN   /**< Maximum string length of sink name*/

/* Notification function */
/*------------------------------------------------------------------*/
/*! @brief This notify function is called by the connection manager to notify
 *         of changes in the condition state of a connection or of a pipe.
 *  @param [in]  h_handler   -   Contains the connection handle that was created by the API x_cm_connect,
 *                               or the pipe handle that was created by the API x_cm_open_pipe.
 *  @param [in]  e_nfy_cond  -   Contains the connection or pipe condition state
 *  @param [in]  pv_nfy_tag  -   Contains the tag value that was specified in the API x_cm_connect or x_cm_open_pipe.
 *  @param [in]  ui4_data    -   Unused, currently always set to 0
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_cm_nfy_fct) (HANDLE_T  h_handle,
                              CM_COND_T e_nfy_cond,
                              VOID*     pv_nfy_tag,
                              UINT32    ui4_data);

/* Connection Manager return values */
#define CMR_ASYNC_NFY             ((INT32)   1)   /**< Return value of async notification*/
#define CMR_OK                    ((INT32)   0)   /**< Return value of ok*/
#define CMR_FAIL                  ((INT32)  -1)   /**< Return value of failure*/
#define CMR_INV_ARG               ((INT32)  -2)   /**< Return value of invalid argument*/
#define CMR_INV_HANDLE            ((INT32)  -3)   /**< Return value of invalid handle*/
#define CMR_OUT_OF_HANDLES        ((INT32)  -4)   /**< Return value of out of handles*/
#define CMR_ALREADY_INIT          ((INT32)  -5)   /**< Return value of already init*/
#define CMR_NOT_INIT              ((INT32)  -6)   /**< Return value of not init*/
#define CMR_INV_NAME              ((INT32)  -7)   /**< Return value of invalid name*/
#define CMR_INV_COMMAND           ((INT32)  -8)   /**< Return value of invalid command*/
#define CMR_NOT_ENOUGH_MEM        ((INT32)  -9)   /**< Return value of not enough memory*/
#define CMR_ALREADY_EXISTS        ((INT32) -10)   /**< Return value of already exists*/
#define CMR_OUT_OF_HANDLERS       ((INT32) -11)   /**< Return value of out of handlers*/
#define CMR_NO_HANDLER            ((INT32) -12)   /**< Return value of no handler*/
#define CMR_NO_COMP_FOUND         ((INT32) -13)   /**< Return value of no component found*/
#define CMR_AEE_NO_RIGHTS         ((INT32) -14)   /**< Return value of AEE no rights*/
#define CMR_AEE_OUT_OF_RESOURCES  ((INT32) -15)   /**< Return value of out of resource*/
#define CMR_EOS                   ((INT32) -16)   /**< Return value of end of stream*/

/* Control (for set and get functions) */
typedef UINT32 CM_CTRL_TYPE_T;                    /**< CM control type code*/

/*
  Control codes from 0 to 0x7fffffff are reserved for the Connection Manager.
  Codes from 0x80000000 to 0xbfffffff are defined by the Connection Manager,
  but are routed to the connection handler's set and get functions.
  Codes from 0xC0000000 to 0xffffffff are defined by individual handlers and
  are routed to the connection handler's set and get functions.
*/
#define CM_CTRL_CM_OFFSET               ((CM_CTRL_TYPE_T) 0x00000000)   /**< Control code of cm offset*/
#define CM_CTRL_HANDLER_CM_OFFSET       ((CM_CTRL_TYPE_T) 0x80000000)   /**< Control code of cm handler offset*/
#define CM_CTRL_HANDLER_PRIVATE_OFFSET  ((CM_CTRL_TYPE_T) 0xC0000000)   /**< Control code of private handler offset*/

#define CM_CTRL_COND     (CM_CTRL_CM_OFFSET + 1)                        /**< Offset of cm control condition*/
#define CM_CTRL_RM_SRC_COMP  (CM_CTRL_HANDLER_CM_OFFSET + 0)

/*
  Information codes from 0 to 0x7fffffff are reserved for the Connection
  Manager.
  Connection handlers shall define their own codes starting at 0x80000000.
  These codes are used in x_cm_nfy_fct when e_nfy_cond is set to
  CM_COND_CONNECTION_INFO.
*/
#define CM_CONN_INFO_GOT_TV_SIGNAL                1    /**< Information code of got TV signal                             */
#define CM_CONN_INFO_LOST_TV_SIGNAL               2    /**< Information code of lost TV signal                            */
#define CM_CONN_INFO_COMP_BUSY                    3    /**< Information code of component busy                            */
#define CM_CONN_INFO_GOT_TUNER_SIGNAL             4    /**< Information code of got tuner signal                          */
#define CM_CONN_INFO_LOST_TUNER_SIGNAL            5    /**< Information code of lost tuner signal                         */
#define CM_CONN_INFO_PAT_TS_ID_CHANGED            6    /**< Information code of PAT TS ID changed                         */
#define CM_CONN_INFO_TV_AUD_SYS_DETECTED          7    /**< Information code of TV audio system detected                  */
#define CM_CONN_INFO_EWS_ON                       8    /**< Information code of EWS on                                    */
#define CM_CONN_INFO_EWS_OFF                      9    /**< Information code of EWS off                                   */
#define CM_CONN_INFO_ISDB_BRDCST_SVC              10   /**< Information code of ISDB broadcasting service detected        */
#define CM_CONN_INFO_ISDB_NON_BRDCST_SVC          11   /**< Information code of ISDB non-broadcasting service detected    */
#define CM_CONN_INFO_ISDB_UNKNOWN_BRDCST          12   /**< Information code of ISDB broadcasting service unable detected */
/* Riks-TV requirement - Begin */
#define CM_CONN_INFO_SIGNAL_LOST_TIMER_EXPIRED    13  /**< Information code of start alternate frequency tuning           */
/* Riks-TV requirement - End */
#define CM_CONN_INFO_DVBS_AUTOSCAN_END            14  /**< Information code of dvbs auto scan end           */
#define CM_CONN_INFO_PRE_CONNECTION_END           15  /**< Information code of pre connection end */
/*
  Flags that can be used by individual handlers
*/
#define CM_FLAG_DVB                   ((UINT32) 0x00000001)           /**< Indicate DVB flag*/
#define CM_FLAG_ATSC                  ((UINT32) 0x00000002)           /**< Indicate ATSC flag*/
#define CM_FLAG_SCTE                  ((UINT32) 0x00000004)           /**< Indicate SCTE flag*/
#define CM_FLAG_2_LANGUAGES_1_STREAM  ((UINT32) 0x00000010)           /**< Indicate 2 audio languages exist in 1 stream*/
#define CM_FLAG_N_LANGUAGES_1_STREAM  ((UINT32) 0x00000020)           /**< Indicate N audio languages exist in 1 stream*/
#define CM_FLAG_DUAL_MONO_FAKE_STEREO ((UINT32) 0x00000040)           /**< Indicate to make dual mono to a fake stereo stream */
#define CM_FLAG_APPEND_SCDB_SUPPORT   ((UINT32) 0x00000100)           /**< Indicate to support append the same pid different section pmt>**/

#define CM_FLAG_ATSC_SCTE             (CM_FLAG_ATSC | CM_FLAG_SCTE)   /**< Indicate ATSC SCTE flag*/

#endif /*  _U_CM_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                         /* end of groupMW_CONN_MNGR */
/*----------------------------------------------------------------------------*/

