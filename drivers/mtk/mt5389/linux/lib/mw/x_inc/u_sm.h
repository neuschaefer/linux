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
/*! @file u_sm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Stream Manager specific definitions,
 *         which are exported to applications.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_
#define _U_SM_    /**<        */


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_sm_event.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* Handler name **************************************************************/
#define SM_HDLR_NAME_MAX_SIZE         16    /**<        */
#define SM_GNRC_SESS_HDLR_NAME        "generic_sess"    /**<        */
#define SM_1394_SESS_HDLR_NAME        "1394_sess"    /**<        */
#define SM_RCRD_SESS_HDLR_NAME        "record_sess"    /**<        */

/* stream condition **********************************************************/
typedef UINT32 SM_COND_T;    /**<        */

#define SM_COND_IGNORE                ((SM_COND_T) 0)    /**<        */
#define SM_COND_OPENING               ((SM_COND_T) 1)    /**<        */
#define SM_COND_OPENED                ((SM_COND_T) 2)    /**<        */
#define SM_COND_STARTING              ((SM_COND_T) 3)    /**<        */
#define SM_COND_STARTED               ((SM_COND_T) 4)    /**<        */
#define SM_COND_STOPPING              ((SM_COND_T) 5)    /**<        */
#define SM_COND_CLOSING               ((SM_COND_T) 6)    /**<        */
#define SM_COND_CLOSED                ((SM_COND_T) 7)    /**<        */
#define SM_COND_ERROR                 ((SM_COND_T) 8)    /**<        */
#define SM_COND_LAST_ENTRY            ((SM_COND_T) 128)    /**<        */

/* Session condition *********************************************************/
typedef UINT32 SM_SESS_COND_T;    /**<        */

#define SM_SESS_COND_IGNORE           ((SM_SESS_COND_T) SM_COND_IGNORE)    /**<        */
#define SM_SESS_COND_OPENING          ((SM_SESS_COND_T) SM_COND_OPENING)    /**<        */
#define SM_SESS_COND_OPENED           ((SM_SESS_COND_T) SM_COND_OPENED)    /**<        */
#define SM_SESS_COND_STARTING         ((SM_SESS_COND_T) SM_COND_STARTING)    /**<        */
#define SM_SESS_COND_STARTED          ((SM_SESS_COND_T) SM_COND_STARTED)    /**<        */
#define SM_SESS_COND_STOPPING         ((SM_SESS_COND_T) SM_COND_STOPPING)    /**<        */
#define SM_SESS_COND_CLOSING          ((SM_SESS_COND_T) SM_COND_CLOSING)    /**<        */
#define SM_SESS_COND_CLOSED           ((SM_SESS_COND_T) SM_COND_CLOSED)    /**<        */
#define SM_SESS_COND_ERROR            ((SM_SESS_COND_T) SM_COND_ERROR)    /**<        */
#define SM_SESS_COND_LAST_ENTRY       ((SM_SESS_COND_T) 128)    /**<        */


/* Command set definitions ***************************************************/
/* command codes are all handler specific */
typedef UINT32 SM_CMD_CODE_T;    /**<        */

#define SM_CMD_CODE_END               ((SM_CMD_CODE_T) 0)    /**<        */
#define SM_CMD_CODE_PRIMARY_BIT_FLAG  ((SM_CMD_CODE_T) 0x80000000)    /**<        */

/*------------------------------------------------------------------*/
/*! @struct  SM_COMMAND_T
 *  @brief  For client to set command
 *  @code
 *  typedef struct
 *  {
 *      SM_CMD_CODE_T  e_code;
 *      union
 *      {
 *          BOOL       b_boolean;
 *          UINT8      ui1_data;
 *          UINT16     ui2_data;
 *          UINT32     ui4_data;
 *          HANDLE_T   h_obj;
 *          CHAR*      ps_text;
 *          VOID*      pv_data;
 *      } u;
 *  } SM_COMMAND_T;
 *  @endcode
 *  @li@c  e_code                                            -References to the type of command 
 *  @li@c  b_boolean                                         - the data type of command is BOOL
 *  @li@c  ui1_data                                          -the data type of command is  UINT8
 *  @li@c  ui2_data                                          -the data type of command is UINT16
 *  @li@c  ui4_data                                          -the data type of command is UINT32
 *  @li@c  h_obj                                             -the data type of command is HANDL_T
 *  @li@c  ps_text                                           -the data type of command is CHAR*
 *  @li@c  pv_data                                           -the data type of command is VOID*
 */
/*------------------------------------------------------------------*/
typedef struct
{
    SM_CMD_CODE_T  e_code;

    union
    {
        BOOL       b_boolean;
        UINT8      ui1_data;
        UINT16     ui2_data;
        UINT32     ui4_data;
        HANDLE_T   h_obj;
        CHAR*      ps_text;
        VOID*      pv_data;
    } u;
} SM_COMMAND_T;

/* set/get types definition ***************************************************
    31                  20 19                                   0
    --------------------------------------------------------------
    |     Group(12 bits)  |         Code(20 bits)                |
    --------------------------------------------------------------
 *****************************************************************************/
typedef UINT32 SM_SET_TYPE_T;    /**<        */
typedef UINT32 SM_GET_TYPE_T;    /**<        */

#define SM_CODE_BITS                  ((UINT32)20)    /**<        */
#define SM_GRP_BITS                   ((UINT32)12)    /**<        */
#define SM_GRP_MASK                   ((UINT32)0xFFF00000)    /**<        */
#define SM_CODE_MASK                  ((UINT32)0x000FFFFF)    /**<        */

#define SM_GET_GRP( type )            ((UINT32)(type & SM_GRP_MASK))    /**<        */
#define SM_GET_CODE( type )           ((UINT32)(type & SM_CODE_MASK))    /**<        */

#define SM_GRP_SESS                   ((UINT32)(1<<SM_CODE_BITS))    /**<        */
#define SM_GRP_STRM                   ((UINT32)(2<<SM_CODE_BITS))    /**<        */
#define SM_GRP_LAST_ENTRY             ((UINT32)64)    /**<        */


/* Session set types *********************************************************/
#define SM_SESS_SET_TYPE_IGNORE       ((SM_SET_TYPE_T)(SM_GRP_SESS | 0))    /**<        */
#define SM_SESS_SET_TYPE_PLAY         ((SM_SET_TYPE_T)(SM_GRP_SESS | 1))    /**<        */
#define SM_SESS_SET_TYPE_STOP         ((SM_SET_TYPE_T)(SM_GRP_SESS | 2))    /**<        */

/* Session get types *********************************************************/
#define SM_SESS_GET_TYPE_IGNORE       ((SM_GET_TYPE_T)(SM_GRP_SESS | 0))    /**<        */
#define SM_SESS_GET_TYPE_STATE        ((SM_GET_TYPE_T)(SM_GRP_SESS | 1))    /**<        */
#define SM_SESS_GET_TYPE_HDLR_NAME    ((SM_GET_TYPE_T)(SM_GRP_SESS | 2))    /**<        */
#define SM_SESS_GET_TYPE_H_CONNECTION ((SM_GET_TYPE_T)(SM_GRP_SESS | 3))    /**<        */
#define SM_SESS_GET_TYPE_STRM_NUM     ((SM_GET_TYPE_T)(SM_GRP_SESS | 4))    /**<        */
#define SM_SESS_GET_TYPE_STRM_HANDLES ((SM_GET_TYPE_T)(SM_GRP_SESS | 5))    /**<        */
#define SM_SESS_GET_TYPE_DEST_GRP_NAME ((SM_GET_TYPE_T)(SM_GRP_SESS | 6))    /**<        */

/* Stream set types **********************************************************/
#define SM_STRM_SET_TYPE_IGNORE        ((SM_SET_TYPE_T)(SM_GRP_STRM | 0))    /**<        */
#define SM_STRM_SET_TYPE_PLAY          ((SM_SET_TYPE_T)(SM_GRP_STRM | 1))    /**<        */
#define SM_STRM_SET_TYPE_STOP          ((SM_SET_TYPE_T)(SM_GRP_STRM | 2))    /**<        */
#define SM_STRM_SET_TYPE_CCI_INFO      ((SM_SET_TYPE_T)(SM_GRP_STRM | 3))    /**<        */

/* Stream get types **********************************************************/
#define SM_STRM_GET_TYPE_IGNORE        ((SM_GET_TYPE_T)(SM_GRP_STRM | 0))    /**<        */
#define SM_STRM_GET_TYPE_STATE         ((SM_GET_TYPE_T)(SM_GRP_STRM | 1))    /**<        */
#define SM_STRM_GET_TYPE_ID            ((SM_GET_TYPE_T)(SM_GRP_STRM | 2))    /**<        */
#define SM_STRM_GET_TYPE_SCDB_REC      ((SM_GET_TYPE_T)(SM_GRP_STRM | 3))    /**<        */


/* event notify callback functions *******************************************/
/*------------------------------------------------------------------*/
/*! @brief  This type defines the session notify callback function. It will be executed from Stream Selector
 *              whenever the condition of a session is changed or an event occurred.
 *  @param [in] h_sess Indicates the handle of the session.
 *  @param [in] e_sess_cond Indicates the condition of the session.
 *  @param [in] pv_sess_tag Contains the user's private tag.
 *  @param [in] e_event  Contains a newly occurred event associated with current condition.
 *  @param [in] ui4_data  Contains data passed by under layer for application.
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sess_nfy_fct)( HANDLE_T          h_sess,
                                 SM_SESS_COND_T    e_sess_cond,
                                 VOID*             pv_sess_tag,
                                 SM_EVN_TYPE_T     e_event,
                                 UINT32            ui4_data );

/*------------------------------------------------------------------*/
/*! @brief   This type defines the session notify callback function. It will be executed from Stream Selector
 *              whenever the condition of a stream is changed or an event occurred.
 *  @param [in] h_steam Indicates the handle of the steam.
 *  @param [in] e_nfy_cond Indicates the condition of the steam.
 *  @param [in] pv_nfy_tag Contains the user's private tag.
 *  @param [in] e_event  Contains a newly occurred event associated with current condition.
 *  @param [in] ui4_data  Contains data passed by under layer for application.
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_stream_nfy_fct)( HANDLE_T        h_stream,
                                   SM_COND_T       e_nfy_cond,
                                   VOID*           pv_nfy_tag,
                                   SM_EVN_TYPE_T   e_event,
                                   UINT32          ui4_data );


/* Stream Manager API return values ******************************************/
#define SMR_PENDING                             ((INT32)  3)    /**<        */
#define SMR_STOP_DATA_FLOWING                   ((INT32)  2)    /**<        */
#define SMR_ASYNC_NFY                           ((INT32)  1)    /**<        */
#define SMR_OK                                  ((INT32)  0)    /**<        */
#define SMR_INV_ARG                             ((INT32) -1)    /**<        */
#define SMR_INV_HANDLE                          ((INT32) -2)    /**<        */
#define SMR_OUT_OF_HANDLES                      ((INT32) -3)    /**<        */
#define SMR_ALREADY_INIT                        ((INT32) -4)    /**<        */
#define SMR_NOT_INIT                            ((INT32) -5)    /**<        */
#define SMR_INTERNAL_ERROR                      ((INT32) -6)    /**<        */
#define SMR_NOT_OPEN_YET                        ((INT32) -7)    /**<        */
#define SMR_DRV_COMP_ERROR                      ((INT32) -8)    /**<        */
#define SMR_NOT_SUPPORT                         ((INT32) -9)    /**<        */
#define SMR_INSUFFICIENT_MEMORY                 ((INT32) -10)    /**<        */
#define SMR_ALREADY_PLAYED                      ((INT32) -11)    /**<        */
#define SMR_ALREADY_STOPPED                     ((INT32) -12)    /**<        */
#define SMR_INV_SET_INFO                        ((INT32) -13)    /**<        */
#define SMR_INV_GET_INFO                        ((INT32) -14)    /**<        */
#define SMR_INCONSISTENCE_FORMAT                ((INT32) -15)    /**<        */
#define SMR_HDLR_ALREADY_REGISTERED             ((INT32) -16)    /**<        */
#define SMR_HDLR_NOT_REGISTERED_YET             ((INT32) -17)    /**<        */
#define SMR_SESS_ALREADY_EXIST                  ((INT32) -18)    /**<        */
#define SMR_SESS_NOT_EXIST                      ((INT32) -19)    /**<        */
#define SMR_INSUFFICIENT_SIZE                   ((INT32) -20)    /**<        */
#define SMR_NOT_ALLOW                           ((INT32) -21)    /**<        */
#define SMR_CAN_NOT_OP_IN_THIS_STATE            ((INT32) -22)    /**<        */
#define SMR_INV_COMMAND                         ((INT32) -23)    /**<        */
#define SMR_DRV_COMP_NOT_FOUND                  ((INT32) -24)    /**<        */
#define SMR_DRV_ALREADY_USED_BY_OTHER_STRM      ((INT32) -25)    /**<        */
#define SMR_STRM_COMP_ID_NOT_FOUND              ((INT32) -26)    /**<        */
#define SMR_LAST_ENTRY                          ((INT32) -256)    /**<        */


/* Observer related definitions **********************************************/
/*------------------------------------------------------------------*/
/*! @enum   SM_OBJ_TYPE_T
 *  @brief     Indicates the type of object.
 *  @code
 *  typedef enum
 *  {
 *      SM_OBJ_TYPE_SESSION = 0,
 *      SM_OBJ_TYPE_STRM
 *  } SM_OBJ_TYPE_T;
 *  @endcode
 *  @li@c  SM_OBJ_TYPE_SESSION                               -the obj is session
 *  @li@c  SM_OBJ_TYPE_STRM                                  -the obj is stream.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SM_OBJ_TYPE_SESSION = 0,
    SM_OBJ_TYPE_STRM
} SM_OBJ_TYPE_T;

typedef UINT32 SM_SESS_TYPE_T;    /**<        */
#define SM_SESS_TYPE_GNRC                       ((UINT32) 0)    /**<        */
#define SM_SESS_TYPE_1394                       ((UINT32) 1)    /**<        */
#define SM_SESS_TYPE_RCRD                       ((UINT32) 2)    /**<        */

#define SM_OBSVR_OBJ_TYPE_FILTER_NONE           ((UINT32) 0x0)    /**<        */
#define SM_OBSVR_OBJ_TYPE_FILTER_ANY            ((UINT32) 0xFFFFFFFF)    /**<        */
#define SM_OBSVR_OBJ_TYPE_FILTER_SESS           MAKE_BIT_MASK_32(SM_OBJ_TYPE_SESSION)    /**<        */
#define SM_OBSVR_OBJ_TYPE_FILTER_STRM           MAKE_BIT_MASK_32(SM_OBJ_TYPE_STRM)    /**<        */

#define SM_OBSVR_SESS_TYPE_FILTER_NONE          ((UINT32) 0x0)    /**<        */
#define SM_OBSVR_SESS_TYPE_FILTER_ANY           ((UINT32) 0xFFFFFFFF)    /**<        */
#define SM_OBSVR_SESS_TYPE_FILTER_GNRC          MAKE_BIT_MASK_32(SM_SESS_TYPE_GNRC)    /**<        */
#define SM_OBSVR_SESS_TYPE_FILTER_1394          MAKE_BIT_MASK_32(SM_SESS_TYPE_1394)    /**<        */
#define SM_OBSVR_SESS_TYPE_FILTER_RCRD          MAKE_BIT_MASK_32(SM_SESS_TYPE_RCRD)    /**<        */

#define SM_OBSVR_STRM_TYPE_FILTER_NONE          ((UINT32) 0x0)    /**<        */
#define SM_OBSVR_STRM_TYPE_FILTER_ANY           ((UINT32) 0xFFFFFFFF)    /**<        */
#define SM_OBSVR_STRM_TYPE_FILTER_AUDIO         ST_MASK_AUDIO    /**<        */
#define SM_OBSVR_STRM_TYPE_FILTER_VIDEO         ST_MASK_VIDEO    /**<        */
#define SM_OBSVR_STRM_TYPE_FILTER_CLOSED_CAP    ST_MASK_CLOSED_CAPTION    /**<        */
#define SM_OBSVR_STRM_TYPE_FILTER_TELETEXT      ST_MASK_TELETEXT    /**<        */
#define SM_OBSVR_STRM_TYPE_FILTER_SUBTITLE      ST_MASK_SUBTITLE    /**<        */
#define SM_OBSVR_STRM_TYPE_FILTER_DATA          ST_MASK_DATA    /**<        */

#define SM_OBSVR_STATE_FLTER_NONE               ((UINT32) 0x0)    /**<        */
#define SM_OBSVR_STATE_FLTER_ANY                ((UINT32) 0xFFFFFFFF)    /**<        */
#define SM_OBSVR_STATE_FLTER_IGNORE             MAKE_BIT_MASK_32(SM_COND_IGNORE)    /**<        */
#define SM_OBSVR_STATE_FLTER_OPENING            MAKE_BIT_MASK_32(SM_COND_OPRNING)    /**<        */
#define SM_OBSVR_STATE_FLTER_OPENED             MAKE_BIT_MASK_32(SM_COND_OPENED)    /**<        */
#define SM_OBSVR_STATE_FLTER_STARTING           MAKE_BIT_MASK_32(SM_COND_STARTING)    /**<        */
#define SM_OBSVR_STATE_FLTER_STARTED            MAKE_BIT_MASK_32(SM_COND_STARTED)    /**<        */
#define SM_OBSVR_STATE_FLTER_STOPPING           MAKE_BIT_MASK_32(SM_COND_STOPPING)    /**<        */
#define SM_OBSVR_STATE_FLTER_CLOSING            MAKE_BIT_MASK_32(SM_COND_CLOSING)    /**<        */
#define SM_OBSVR_STATE_FLTER_CLOSED             MAKE_BIT_MASK_32(SM_COND_CLOSED)    /**<        */

#define SM_OBSVR_EVN_FLTER_ANY                  ((UINT32) 0xFFFFFFFF)    /**<        */

/* Filter matching rule is as following,
Session:
        ( (ui4_obj_type_fltr == Any or session) and
          (ui4_sess_type_fltr == Any or match) )
        and
        ( ( ui4_sess_state_fltr == Any or match) or
          ( pui4_event_fltr == Any or match ) )
Stream:
        ( (ui4_obj_type_fltr == Any or session) and
          (ui4_strm_type_fltr == Any or match) )
        and
        ( ( ui4_strm_state_fltr == Any or match) or
          ( pui4_event_fltr == Any or match ) )
 */
/*------------------------------------------------------------------*/
/*! @struct  SM_OBSVR_FILTER_T
 *  @brief  Contains observer info.
 *  @code
 *  typedef struct _SM_OBSVR_FILTER_T
 *  {
 *      UINT32     ui4_obj_type_fltr;
 *      UINT32     ui4_sess_type_fltr;
 *      UINT32     ui4_strm_type_fltr;
 *      UINT32     ui4_sess_state_fltr;
 *      UINT32     ui4_strm_state_fltr;
 *      UINT32*    pui4_event_fltr;      
 *  } SM_OBSVR_FILTER_T;
 *  @endcode
 *  @li@c  ui4_obj_type_fltr                                 -
 *  @li@c  ui4_sess_type_fltr                                -
 *  @li@c  ui4_strm_type_fltr                                -
 *  @li@c  ui4_sess_state_fltr                               -
 *  @li@c  ui4_strm_state_fltr                               -
 *  @li@c  pui4_event_fltr                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _SM_OBSVR_FILTER_T
{
    UINT32     ui4_obj_type_fltr;
    UINT32     ui4_sess_type_fltr;
    UINT32     ui4_strm_type_fltr;
    UINT32     ui4_sess_state_fltr;
    UINT32     ui4_strm_state_fltr;
    UINT32*    pui4_event_fltr;      /* events array that end with SM_EVN_IGNORE */
} SM_OBSVR_FILTER_T;

/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sm_obsvr_sess_nfy_fct)(
    HANDLE_T          h_obsvr,
    SM_SESS_TYPE_T    e_sess_type,
    HANDLE_T          h_sess,
    SM_COND_T         e_cond,
    VOID*             pv_tag,
    SM_EVN_TYPE_T     e_event,
    UINT32            ui4_data );

/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sm_obsvr_strm_nfy_fct)(
    HANDLE_T          h_obsvr,
    STREAM_TYPE_T     e_strm_type,
    HANDLE_T          h_steam,
    SM_COND_T         e_cond,
    VOID*             pv_tag,
    SM_EVN_TYPE_T     e_event,
    UINT32            ui4_data );

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/


#endif /* _U_SM_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_STRM_MNGR */
/*----------------------------------------------------------------------------*/

