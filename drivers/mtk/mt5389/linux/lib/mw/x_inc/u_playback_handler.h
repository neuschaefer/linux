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
/*! @file u_playback_handler.h
 *  $RCSfile: $
 *  $Revision: #2 $
 *  $Date: 2012/06/28 $
 *  $Author: wengao.han $
 *
 *  @par Description:
 *       This header file contains the Playback Connection Handler specific
 *       definitions that are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CONN_MNGR_PLAYBACK_HDLR Playback Handler
 *  @ingroup  groupMW_CONN_MNGR
 *  @brief    Provides a unified interface for playback connection
 *  @see      groupMW_CONN_MNGR
 *
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_PLAYBACK_HANDLER_H_
#define _U_PLAYBACK_HANDLER_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_cm.h"
#include "u_handle.h"
#include "u_scdb.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define PLAYBACK_HANDLER_DEFAULT_NAME "playback"  /**< Default name of playback handler */

#define PLAYBACK_CONN_OFFSET                ((CM_COMMAND_CODE_T) 0x80004000)    /**< Command codes offset */
#define PLAYBACK_CONN_BUFFER_AGENT_NAME     (PLAYBACK_CONN_OFFSET +  0)         /**< Connection command codes - Buffer agent name */
#define PLAYBACK_CONN_DISC_IF_COMP_BUSY     (PLAYBACK_CONN_OFFSET +  1)         /**< Connection command codes - Disconnect if components are busy */
#define PLAYBACK_CONN_DISC_IF_FILE_ERROR    (PLAYBACK_CONN_OFFSET +  2)         /**< Connection command codes - Disconnect if file is busy */
#define PLAYBACK_CONN_2_LANGUAGES_1_STREAM  (PLAYBACK_CONN_OFFSET +  3)         /**< Connection command codes - 2 languages to 1 stream */
#define PLAYBACK_CONN_DUAL_MONO_FAKE_STEREO (PLAYBACK_CONN_OFFSET +  4)         /**< Connection command codes - Dual mono fack stereo */
#define PLAYBACK_CONN_VIRTUAL_SOURCE_NAME   (PLAYBACK_CONN_OFFSET +  5)         /**< Connection command codes - Virtual source name */
#define PLAYBACK_CONN_OPERATION_MODE        (PLAYBACK_CONN_OFFSET +  6)         /**< Connection command codes - Operation Mode */
#define PLAYBACK_CONN_SHARED_MEMORY         (PLAYBACK_CONN_OFFSET +  7)         /**< Connection command codes - Use shared memory or not */
#define PLAYBACK_CONN_SOURCE_TYPE           (PLAYBACK_CONN_OFFSET +  8)         /**< Connection command codes - determine bagt source type */

#define PLAYBACK_HANDLER_OFFSET                ((CM_CTRL_TYPE_T) 0x02000000) 
#define PLAYBACK_HANDLER_SET_GET(_x)           (CM_CTRL_HANDLER_PRIVATE_OFFSET + PLAYBACK_HANDLER_OFFSET + (_x))      /**< Playback-specific commands to use with x_cm_set/get */

#define PLAYBACK_CTRL_GET_ITEM_ATTRIBUTES      PLAYBACK_HANDLER_SET_GET(1)    /**< Get commands - Get attributes of item */
#define PLAYBACK_CTRL_GET_ITEM_ATTRIBUTES_BY_INDEX  PLAYBACK_HANDLER_SET_GET(2)    /**< Get commands - Get attributes of item by index */
#ifdef TIME_SHIFT_SUPPORT
#define PLAYBACK_CTRL_GET_CURR_UTC             PLAYBACK_HANDLER_SET_GET(3)    /**< Get commands - Get current UTC */
#endif

#define PLAYBACK_CTRL_GET_BUFFER_FULLNESS      PLAYBACK_HANDLER_SET_GET(4)    /**< Get commands - Get Buffer fullness */
#define PLAYBACK_CTRL_GET_SHARED_MEMORY        PLAYBACK_HANDLER_SET_GET(5)    /**< Get commands - Get shared memory */
#define PLAYBACK_CTRL_GET_TIME_TO_SIZE         PLAYBACK_HANDLER_SET_GET(6)    /**< Get commands - Get TIME TO SIZE */
#define PLAYBACK_CTRL_GET_BUFFER_STATUS        PLAYBACK_HANDLER_SET_GET(7)    /**< Get commands - Get Buffer Status */
#define PLAYBACK_CTRL_GET_HTTP_DOWNLOAD_SPEED  PLAYBACK_HANDLER_SET_GET(8)    /**< Get commands - Get Buffer Status */

#define PLAYBACK_CTRL_SET_QUEUE_FILE           PLAYBACK_HANDLER_SET_GET(101)    /**< Set commands - Queue file */
#define PLAYBACK_CTRL_SET_QUEUE_BUFFER         PLAYBACK_HANDLER_SET_GET(102)    /**< Set commands - Queue buffer */
#define PLAYBACK_CTRL_SET_UNQUEUE              PLAYBACK_HANDLER_SET_GET(103)    /**< Set commands - Unqueue */
#define PLAYBACK_CTRL_SET_EMPTY_QUEUE          PLAYBACK_HANDLER_SET_GET(104)    /**< Set commands - Empty quque */
#define PLAYBACK_CTRL_SET_ITEM_ATTRIBUTES      PLAYBACK_HANDLER_SET_GET(105)    /**< Set commands - Set attributes of item */
#define PLAYBACK_CTRL_SET_ITEM_SVC_INFO_BY_ID  PLAYBACK_HANDLER_SET_GET(106)    /**< Set commands - Set SVC information by ID */

#define PLAYBACK_CTRL_SET_QUEUE_URI            PLAYBACK_HANDLER_SET_GET(107)    /**< Set commands - Queue URI */
#define PLAYBACK_CTRL_SET_PRELOAD              PLAYBACK_HANDLER_SET_GET(108)    /**< Set commands - Preload */
#define PLAYBACK_CTRL_SET_QUEUE_PULL           PLAYBACK_HANDLER_SET_GET(109)    /**< Set commands - Queue PULL */
#define PLAYBACK_CTRL_SET_MODIFY_SCDB          PLAYBACK_HANDLER_SET_GET(110)    /**< Set commands - Modify SCDB */
#define PLAYBACK_CTRL_SET_RELEASE_TABLES       PLAYBACK_HANDLER_SET_GET(111)    /**< Set commands - Release All Tables*/

#define PLAYBACK_DISC_AS_REQUESTED                   0U    /**< Disconnection reasons - As requested */
#define PLAYBACK_DISC_OTHER_REASON                   1U    /**< Disconnection reasons - Other reason */
#define PLAYBACK_DISC_NEW_CONNECTION                 2U    /**< Disconnection reasons - New connection */
#define PLAYBACK_DISC_NO_COMP_FOUND                  3U    /**< Disconnection reasons - No component is found */
#define PLAYBACK_DISC_COMP_BUSY                      4U    /**< Disconnection reasons - Components is busy */
#define PLAYBACK_DISC_COULD_NOT_SET_COMP_PROPERTIES  5U    /**< Disconnection reasons - Could not set properties of components */
#define PLAYBACK_DISC_COULD_NOT_CONNECT_COMP         6U    /**< Disconnection reasons - Could not connect components */
#define PLAYBACK_DISC_FILE_ERROR                     7U    /**< Disconnection reasons - File error */

#define PLAYBACK_HANDLER_START_OFFSET_FLAG  MAKE_BIT_MASK_32(0)    /**< To use with PLAYBACK_CTRL_GET/SET_ITEM_ATTRIBUTES(_BY_INDEX) - start offset */
#define PLAYBACK_HANDLER_END_OFFSET_FLAG    MAKE_BIT_MASK_32(1)    /**< To use with PLAYBACK_CTRL_GET/SET_ITEM_ATTRIBUTES(_BY_INDEX) - end offset */
#define PLAYBACK_HANDLER_POSITION_FLAG      MAKE_BIT_MASK_32(2)    /**< To use with PLAYBACK_CTRL_GET/SET_ITEM_ATTRIBUTES(_BY_INDEX) - position */

#define PLAYBACK_HANDLER_INVALID_ITEM_ID    (0)    /**< Invalid item ID */

typedef enum
{
    PLAYBACK_CONN_OPERATION_MODE_DEFAULT = 0,
    PLAYBACK_CONN_OPERATION_MODE_PULL,
    PLAYBACK_CONN_OPERATION_MODE_CIRCULAR,
    PLAYBACK_CONN_OPERATION_MODE_PUSH_SHARE_MEM
} PLAYBACK_CONN_OPERATION_MODE_T;

typedef enum
{
    PLAYBACK_CONN_SOURCE_TYPE_DEFAULT = 0,
    PLAYBACK_CONN_SOURCE_TYPE_SKYPE
} PLAYBACK_CONN_SOURCE_TYPE_T;

typedef enum
{
    PLAYBACK_CTRL_PULL_ABORT_FAIL            =   -2,     /* Pull Model Read Abort Fail */
    PLAYBACK_CTRL_PULL_FAIL                  =   -1,     /* Pull Model Read Fail */
    PLAYBACK_CTRL_PULL_READ_OK               =   0,      /* Pull Model Read OK */
    PLAYBACK_CTRL_PULL_ABORT_OK              =   1,      /* Pull Model Read Abort OK */ 
    PLAYBACK_CTRL_PULL_READ_EOS              =   2       /* Pull Model Read EOS */
} PLAYBACK_CTRL_PULL_EVENT_T;

typedef INT32 (*x_playback_pull_nfy_fct)(PLAYBACK_CTRL_PULL_EVENT_T     e_event,
                                         VOID*                          pv_tag,
                                         HANDLE_T                       h_req,
                                         UINT32                         ui4_data);

typedef INT32 (*x_playback_pull_open_fct)(HANDLE_T      h_pb,
                                          HANDLE_T*     ph_pull,
                                          VOID*         pv_app_tag);

typedef INT32 (*x_playback_pull_associate_fct)(HANDLE_T     h_pull,
                                               VOID*        pv_app_tag,
                                               UINT32       ui4_phy_addr,
                                               UINT32       ui4_size);

typedef INT32 (*x_playback_pull_dissociate_fct)(HANDLE_T    h_pull,
                                                VOID*       pv_app_tag);


typedef INT32 (*x_playback_pull_close_fct)(HANDLE_T     h_pull,
                                           VOID*        pv_app_tag);


typedef INT32 (*x_playback_pull_read_fct)(HANDLE_T  h_pull,
                                          VOID*     pv_app_tag,
                                          UINT8*    pu1_dst,
                                          UINT32    ui4_count,                                           
                                          UINT32*   pui4_read);

typedef INT32 (*x_playback_pull_read_async_fct)(HANDLE_T                    h_pull,
                                                VOID*                       pv_app_tag,
                                                UINT8*                      pu1_dst,
                                                UINT32                      ui4_count,
                                                x_playback_pull_nfy_fct     pf_nfy,
                                                VOID*                       pv_tag,
                                                HANDLE_T*                   ph_req);

typedef INT32 (*x_playback_pull_abort_read_async_fct)(HANDLE_T              h_pull,
                                                      VOID*                 pv_app_tag,
                                                      HANDLE_T              h_req);

typedef INT32 (*x_playback_pull_byteseek_fct)(HANDLE_T      h_pull,
                                              VOID*         pv_app_tag,
                                              UINT64        u8SeekPos,
                                              UINT8         u1Whence,
                                              UINT64*       pu8CurPos);

typedef INT32 (*x_playback_pull_get_input_length_fct)(HANDLE_T      h_pull,
                                                      VOID*         pv_app_tag,
                                                      UINT64*       pui8_len);


/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_CTRL_PULL_INFO_T
 *  @brief  This structure contains pull info function pointer table
 *
 *  @code
 *  typedef struct _PLAYBACK_CTRL_PULL_INFO_T
 *  {
 *      HANDLE_T                               h_pb;
 *      x_playback_pull_read_fct               pf_read;
 *      x_playback_pull_read_Async_Fct         pf_read_async;
 *      x_playback_pull_abort_read_async_fct   pf_abort_read_async;
 *      x_playback_pull_byteseek_fct           pf_byteseek;

 *  } PLAYBACK_CTRL_PULL_INFO_T;
 *  @endcode
 *
 *  @li@c   h_pb                    - Pull Connection Handle
 *  @li@c   pf_read                 - read function pointer
 *  @li@c   pf_read_async           - async read function pointer
 *  @li@c   pf_abort_read_async     - abort async read function pointer
 *  @li@c   pf_byteseek             - seek function pointer
 */
/*---------------------------------------------------------------------------*/
typedef struct _PLAYBACK_CTRL_PULL_INFO_T
{
    HANDLE_T                               h_pb;
    VOID*                                  pv_app_tag;
    x_playback_pull_open_fct               pf_open;
    x_playback_pull_associate_fct          pf_associate;
    x_playback_pull_dissociate_fct         pf_dissociate;
    x_playback_pull_close_fct              pf_close;
    x_playback_pull_read_fct               pf_read;
    x_playback_pull_read_async_fct         pf_read_async;
    x_playback_pull_abort_read_async_fct   pf_abort_read_async;
    x_playback_pull_byteseek_fct           pf_byteseek;
    x_playback_pull_get_input_length_fct   pf_get_input_len;
} PLAYBACK_CTRL_PULL_INFO_T;


/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_FLOW_CTRL_INFO_T
 *  @brief  This structure contains URI flow control attributes information
 *
 *  @code
 *  typedef struct _PLAYBACK_FLOW_CTRL_INFO_T
 *  {
 *      UINT32  ui4_buffer_len;
 *      UINT32  ui4_keep_buffer_threshold;
 *      UINT32  ui4_re_buferf_threshold;
 *  } PLAYBACK_FLOW_CTRL_INFO_T;
 *
 *  @endcode
 *
 *  @li@c   ui4_buffer_len              - Buffer size in Bytes. 
 *  @li@c   ui4_keep_buffer_threshold   - Keep threshold in Bytes, at least keep this size of data which can used by seek backward. 
 *  @li@c   ui4_re_buferf_threshold     - Re-Buffer threshold in Bytes, at least this size of space before pulling data from Application
 */
/*---------------------------------------------------------------------------*/
typedef struct _PLAYBACK_FLOW_CTRL_INFO_T
{
    UINT32  ui4_buffer_len;
    UINT32  ui4_keep_buffer_threshold;
    UINT32  ui4_re_buferf_threshold;
} PLAYBACK_FLOW_CTRL_INFO_T;


/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_MODE_CTRL_T
 *  @brief  This structure contains playback handler operation mode
 *
 *  @code
 *  typedef struct _PLAYBACK_MODE_CTRL_T
 *  {
 *      PLAYBACK_CONN_OPERATION_MODE_T  e_op_mode;
 *      union 
 *      {
 *          PLAYBACK_FLOW_CTRL_INFO_T   t_flow_ctrl;
 *      } u;
 *  } PLAYBACK_MODE_CTRL_T;
 *
 *
 *  @endcode
 *
 *  @li@c   e_op_mode                   - operation mode. 
 *  @li@c   t_flow_ctrl                 - used when mode is PLAYBACK_CONN_OPERATION_MODE_CIRCULAR. 
 */
/*---------------------------------------------------------------------------*/
typedef enum
{
    PLAYBACK_FEEDER_BUF_MODE_TIME = 0,
    PLAYBACK_FEEDER_BUF_MODE_BYTE,
    PLAYBACK_FEEDER_BUF_MODE_PIDX,
} PLAYBACK_FEEDER_BUF_MODE_T;

typedef struct _PLAYBACK_FEEDER_BUF_INFO_T
{
  PLAYBACK_FEEDER_BUF_MODE_T e_mode;
  union
  {
      UINT32 ui4_time_ms;	
  }  u;
} PLAYBACK_FEEDER_BUF_INFO_T;

typedef struct _PLAYBACK_MODE_CTRL_T
{
    PLAYBACK_CONN_OPERATION_MODE_T  e_op_mode; /*buffer agent operation mode.*/
    PLAYBACK_FEEDER_BUF_INFO_T      t_feeder_buf_info; 
    union 
    {
        PLAYBACK_FLOW_CTRL_INFO_T   t_flow_ctrl;
    } u;
    
    UINT32                          ui4_access_unit;
    UINT32                          ui4_pid;
} PLAYBACK_MODE_CTRL_T;



/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_GET_BUFFER_FULLNESS_T
 *  @brief  This structure contains URI buffer fullness status
 *
 *  @code
 * typedef struct _PLAYBACK_GET_BUFFER_FULLNESS_T
 * {
 *     UINT32  ui4_length;
 *     UINT32  ui4_percentage;
 *} PLAYBACK_GET_BUFFER_FULLNESS_T;
 *
 *  @endcode
 *
 *  @li@c   ui4_length                  - Buffer size in Bytes. 
 *  @li@c   ui4_percentage              - Buffer fullness percentage. 
 */
/*---------------------------------------------------------------------------*/
typedef struct _PLAYBACK_GET_BUFFER_FULLNESS_T
{
    UINT32  ui4_buf_agt_index;
    UINT32  ui4_length;
    UINT32  ui4_percentage;
} PLAYBACK_GET_BUFFER_FULLNESS_T;


typedef struct _PLAYBACK_GET_BUFFER_STATUS_T
{
    UINT32  ui4_buf_agt_index;
    UINT64  ui8_begin_pos;
    UINT64  ui8_end_pos;
} PLAYBACK_GET_BUFFER_STATUS_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T
 *  @brief  This structure contains the item attributes information and to be
 *          used with PLAYBACK_CTRL_GET/SET_ITEM_ATTRIBUTES
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T
 *  {
 *      UINT32    ui4_id;
 *      UINT32    ui4_flags;
 *      UINT64    ui8_start_offset;
 *      UINT64    ui8_end_offset;
 *      UINT64    ui8_transfer_position;
 *      UINT64    ui8_consumed_position;
 *  } PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T;
 *  @endcode
 *
 *  @li@c   ui4_id                - ID
 *  @li@c   ui4_flags             - Flags
 *  @li@c   ui8_start_offset      - Start offset
 *  @li@c   ui8_end_offset        - End offset
 *  @li@c   ui8_transfer_position - Transfer position
 *  @li@c   ui8_consumed_position - Consumed position
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T
{
    UINT32    ui4_id;
    UINT32    ui4_flags;
    UINT64    ui8_start_offset;
    UINT64    ui8_end_offset;
    UINT64    ui8_transfer_position;
    UINT64    ui8_consumed_position;
}   PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T
 *  @brief  This structure contains the item attributes information and to be
 *          used with PLAYBACK_CTRL_GET/SET_ITEM_ATTRIBUTES_BY_INDEX
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T
 *  {
 *      UINT32                                ui4_index;
 *      UINT32                                ui4_buf_agt_index;
 *      BOOL                                  b_ignore_unqueued;
 *      PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T    t_attributes;
 *  }   PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T;
 *  @endcode
 *
 *  @li@c   ui4_index         - Index of item
 *  @li@c   ui4_buf_agt_index - Index of buffer agent
 *  @li@c   b_ignore_unqueued - To be ignored if unqueued (TRUE or FALSE)
 *  @li@c   t_attributes      - Attributes
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T
{
    UINT32                                ui4_index;
    UINT32                                ui4_buf_agt_index;
    BOOL                                  b_ignore_unqueued;
    PLAYBACK_HANDLER_ITEM_ATTRIBUTES_T    t_attributes;
}   PLAYBACK_HANDLER_ITEM_ATTRIBUTES_BY_INDEX_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_SHARED_MEMORY_T
 *  @brief  This structure contains the shared memory information
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_SHARED_MEMORY_T
 *  {
 *      UINT32  ui4_buf_agt_index;
 *      VOID*   pv_buffer;
 *      SIZE_T  z_buffer_size;
 *  } PLAYBACK_HANDLER_SHARED_MEMORY_T
 *  @endcode
 *
 *  @li@c   ui4_buf_agt_index  - Buffer agent index
 *  @li@c   pv_buffer          - Pointer to shared memory buffer
 *  @li@c   z_buffer_size      - Shared memory buffer size
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_SHARED_MEMORY_T
{
    UINT32  ui4_buf_agt_index;
    VOID*   pv_buffer;
    VOID*   pv_private;
    VOID*   pv_private2;
    SIZE_T  z_buffer_size;
}   PLAYBACK_HANDLER_SHARED_MEMORY_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_TIME_TO_SIZE_T
 *  @brief  This structure gets information of time to size
 *
 *  @code
 *  typedef struct  _PLAYBACK_HANDLER_TIME_TO_SIZE_T
 *  {
 *      UINT32  ui4_ms;
 *      UINT32  ui4_size;
 *  }   PLAYBACK_HANDLER_TIME_TO_SIZE_T;
 *  @endcode
 *
 *  @li@c   ui4_ms      - IN: time in miliseconds
 *  @li@c   ui4_size    - OUT: size in bytes
 */
/*---------------------------------------------------------------------------*/
typedef struct  _PLAYBACK_HANDLER_TIME_TO_SIZE_T
{
    UINT32  ui4_buf_agt_index;
    UINT32  ui4_ms;
    UINT32  ui4_size;
}   PLAYBACK_HANDLER_TIME_TO_SIZE_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_ITEM_SCDB_INFO_T
 *  @brief  This structure contains the SCDB information
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_ITEM_SCDB_INFO_T
 *  {
 *      SCDB_REC_T          t_scdb_rec;
 *      STREAM_COMP_ID_T    t_stream_id;
 *  } PLAYBACK_HANDLER_ITEM_SCDB_INFO_T;
 *  @endcode
 *
 *  @li@c   t_scdb_rec  - SCDB record
 *  @li@c   t_stream_id - Stream ID
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_ITEM_SCDB_INFO_T
{
    SCDB_REC_T          t_scdb_rec;
    STREAM_COMP_ID_T    t_stream_id;
}  PLAYBACK_HANDLER_ITEM_SCDB_INFO_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_ITEM_SVC_INFO_T
 *  @brief  This structure contains the SCDB information
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_ITEM_SVC_INFO_T
 *  {
 *      UINT16    ui2_svl_id;
 *      UINT16    ui2_svl_rec_id;
 *      UINT16    ui2_svc_id;
 *      UINT16    ui2_index;
 *  } PLAYBACK_HANDLER_ITEM_SVC_INFO_T;
 *  @endcode
 *
 *  @li@c   ui2_svl_id     - SVL ID
 *  @li@c   ui2_svl_rec_id - SVL record ID
 *  @li@c   ui2_svc_id     - Servic ID
 *  @li@c   ui2_index      - Index
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_ITEM_SVC_INFO_T
{
    UINT16  ui2_svl_id;
    UINT16  ui2_svl_rec_id;
    UINT16  ui2_svc_id;
    UINT16  ui2_svc_pid;
    UINT16  ui2_index;
} PLAYBACK_HANDLER_ITEM_SVC_INFO_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T
 *  @brief  This structure contains the SCDB information by ID
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T
 *  {
 *      UINT32                              ui4_id;
 *      PLAYBACK_HANDLER_ITEM_SVC_INFO_T    t_svc_info;
 *  } PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T;
 *  @endcode
 *
 *  @li@c   ui4_id     - ID
 *  @li@c   t_svc_info - Service information
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T
{
    UINT32                              ui4_id;
    PLAYBACK_HANDLER_ITEM_SVC_INFO_T    t_svc_info;
}  PLAYBACK_HANDLER_ITEM_SVC_INFO_BY_ID_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T
 *  @brief  This structure contains the MPEG2 information
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T
 *  {
 *      PLAYBACK_HANDLER_ITEM_SVC_INFO_T    t_svc_info;
 *      UINT32                              ui4_flags;
 *  } PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T;
 *  @endcode
 *
 *  @li@c   t_svc_info - Serivce information
 *  @li@c   ui4_flags  - Flags (CM_FLAG_...)
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T
{
    PLAYBACK_HANDLER_ITEM_SVC_INFO_T    t_svc_info;
    UINT32                              ui4_flags;
}  PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_ITEM_ES_INFO_T
 *  @brief  This structure contains the elementary stream information
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_ITEM_ES_INFO_T
 *  {
 *      PTS_T    t_pts;
 *  } PLAYBACK_HANDLER_ITEM_ES_INFO_T;
 *  @endcode
 *
 *  @li@c   t_pts - PTS (Presentation time stamp)
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_ITEM_ES_INFO_T
{
    PTS_T    t_pts;
}  PLAYBACK_HANDLER_ITEM_ES_INFO_T;


typedef enum
{
    PLAYBACK_CTRL_STREAM_TYPE_AUDIO,
    PLAYBACK_CTRL_STREAM_TYPE_VIDEO
} PLAYBACK_CTRL_STREAM_TYPE_T;

typedef struct
{
    PLAYBACK_CTRL_STREAM_TYPE_T     e_stream_type;      /* the stream type whose packet info is to be modified */
    UINT32                          ui4_stream_id;      /* the stream ID used by the ASF demuxer, associated with this stream type */
    UINT64                          ui8_preroll;        /* preroll */
    UINT32                          ui4_packet_size;    /* size of the packet associated with this stream type */
} PLAYBACK_CTRL_IBC_PARAM_SET_ASF_PACKET_INFO_T;

typedef enum
{
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_NONE = 0,             
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_DES_ECB,              
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_DES_CBC,              
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_3DES_ECB,             
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_3DES_CBC,             
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_DVB,                  
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_DVB_CONF,             
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_MULTI2_BIG,           
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_MULTI2_LITTLE,        
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_AES_ECB,              
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_AES_CBC               
} PLAYBACK_CTRL_IBC_ENCRYPT_MODE_T;

/* Descrambling residual termination block(RTB)*/
typedef enum
{
    PLAYBACK_CTRL_IBC_RTB_MODE_CLEAR = 0,
    PLAYBACK_CTRL_IBC_RTB_MODE_CTS,
    PLAYBACK_CTRL_IBC_RTB_MODE_SCTE52,
    PLAYBACK_CTRL_IBC_RTB_MODE_MAX,
} PLAYBACK_CTRL_IBC_RTB_MODE_T;

#define PLAYBACK_CTRL_IBC_ENCRYPT_IV        1
#define PLAYBACK_CTRL_IBC_ENCRYPT_UNIQUE    2
#define PLAYBACK_CTRL_IBC_ENCRYPT_EVEN      4
#define PLAYBACK_CTRL_IBC_ENCRYPT_ODD       8
typedef struct
{
    UINT8       ui1_mask;
    UINT8       ui1_key_len; /*specify key length of aui1_even_key and aui1_odd_key*/
    BOOL        b_warp_key;
    BOOL        b_warp_IV;
    UINT8       aui1_IV[16];
    UINT8       aui1_uniqueKey[16];
    UINT8       aui1_even_key[32];
    UINT8       aui1_odd_key[32];
} PLAYBACK_CTRL_IBC_AES_ENCRYPT_INFO_T;

typedef struct
{
    PLAYBACK_CTRL_IBC_ENCRYPT_MODE_T e_encrypt_mode;

    union
    {
        PLAYBACK_CTRL_IBC_AES_ENCRYPT_INFO_T t_aes;
    }u_encrypt_info;
    
    PLAYBACK_CTRL_IBC_RTB_MODE_T e_rtb_mode;
    
} PLAYBACK_CTRL_IBC_PARAM_ENCRYPT_INFO_T;

typedef enum
{
	PLAYBACK_CTRL_IBC_TYPE_SET_ASF_PACKET_INFO = 1,
    PLAYBACK_CTRL_IBC_TYPE_EOS,
    PLAYBACK_CTRL_IBC_TYPE_ENCRYPT
} PLAYBACK_CTRL_IBC_TYPE_T;

typedef union
{
	PLAYBACK_CTRL_IBC_PARAM_SET_ASF_PACKET_INFO_T   t_set_asf_packet_info_param;
	PLAYBACK_CTRL_IBC_PARAM_ENCRYPT_INFO_T          t_set_encrypt_info_param;
} PLAYBACK_CTRL_IBC_PARAM_T;

typedef struct _PLAYBACK_HANDLER_ITEM_IBC_INFO_T
{
    PLAYBACK_CTRL_IBC_TYPE_T    e_cmd_type;
    UINT32                      ui4_id;           /*< reserved for future use*/
    PLAYBACK_CTRL_IBC_PARAM_T   t_param;
} PLAYBACK_HANDLER_ITEM_IBC_INFO_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T
 *  @brief  This structure contains the extra information
 *
 *  @code
 *  typedef union _PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T
 *  {
 *    PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T    t_mpeg2_info;
 *    PLAYBACK_HANDLER_ITEM_ES_INFO_T        t_es_info;
 *  } PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T;
 *  @endcode
 *
 *  @li@c   t_mpeg2_info - MPEG2 information
 *  @li@c   t_es_info    - Elementary stream information
 */
/*---------------------------------------------------------------------------*/

typedef union _PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T
{
    PLAYBACK_HANDLER_ITEM_MPEG_2_INFO_T    t_mpeg2_info;
    PLAYBACK_HANDLER_ITEM_ES_INFO_T        t_es_info;
    PLAYBACK_HANDLER_ITEM_IBC_INFO_T       t_ibc_info;
} PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_QUEUE_URI_T
 *  @brief  This structure contains the queue URI information and to be used
 *          with PLAYBACK_CTRL_SET_QUEUE_FILE
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_QUEUE_URI_T
 *  {
 *      UINT32                                ui4_id
 *      UINT32                                ui4_flags;
 *      DATA_FMT_T                            e_format;
 *      UINT64                                ui8_start_offset;
 *      UINT64                                ui8_end_offset;
 *      const CHAR*                           ps_path;
 *      PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*    pt_scdb_info;
 *      UINT32                                ui4_nb_records;
 *      UINT32                                ui4_buf_agt_index;
 *      PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T    t_extra;
 *  }   PLAYBACK_HANDLER_QUEUE_URI_T;
 *  @endcode
 *
 *  @li@c   ui4_id            - ID of queued item (Out)
 *  @li@c   ui4_flags         - Flags
 *  @li@c   e_format          - Format
 *  @li@c   ui8_start_offset  - Start offset
 *  @li@c   ui8_end_offset    - End offset
 *  @li@c   ps_path           - Path of file
 *  @li@c   pt_scdb_info      - SCDB information
 *  @li@c   ui4_nb_records    - Number of records
 *  @li@c   ui4_buf_agt_index - Index of buffer agent index
 *  @li@c   t_extra           - Extra information
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_QUEUE_URI_T
{
    UINT32                                ui4_id;
    UINT32                                ui4_flags;
    DATA_FMT_T                            e_format;
    UINT64                                ui8_start_offset;
    UINT64                                ui8_end_offset;
    const CHAR*                           ps_path;
    PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*    pt_scdb_info;
    UINT32                                ui4_nb_records;
    UINT32                                ui4_buf_agt_index;
    HANDLE_T                              h_uri;
    PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T    t_extra;
    const CHAR*                           ps_proxy_url;
    const CHAR*                           ps_agent_url;
    const CHAR*                           ps_cookie_url;
}   PLAYBACK_HANDLER_QUEUE_URI_T;



/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_QUEUE_FILE_T
 *  @brief  This structure contains the queue file information and to be used
 *          with PLAYBACK_CTRL_SET_QUEUE_FILE
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_QUEUE_FILE_T
 *  {
 *      UINT32                                ui4_id
 *      UINT32                                ui4_flags;
 *      DATA_FMT_T                            e_format;
 *      UINT64                                ui8_start_offset;
 *      UINT64                                ui8_end_offset;
 *      const CHAR*                           ps_path;
 *      PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*    pt_scdb_info;
 *      UINT32                                ui4_nb_records;
 *      UINT32                                ui4_buf_agt_index;
 *      PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T    t_extra;
 *  }   PLAYBACK_HANDLER_QUEUE_FILE_T;
 *  @endcode
 *
 *  @li@c   ui4_id            - ID of queued item (Out)
 *  @li@c   ui4_flags         - Flags
 *  @li@c   e_format          - Format
 *  @li@c   ui8_start_offset  - Start offset
 *  @li@c   ui8_end_offset    - End offset
 *  @li@c   ps_path           - Path of file
 *  @li@c   pt_scdb_info      - SCDB information
 *  @li@c   ui4_nb_records    - Number of records
 *  @li@c   ui4_buf_agt_index - Index of buffer agent index
 *  @li@c   t_extra           - Extra information
 */
/*---------------------------------------------------------------------------*/

typedef struct _PLAYBACK_HANDLER_QUEUE_FILE_T
{
    UINT32                                ui4_id;
    UINT32                                ui4_flags;
    DATA_FMT_T                            e_format;
    UINT64                                ui8_start_offset;
    UINT64                                ui8_end_offset;
    const CHAR*                           ps_path;
    PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*    pt_scdb_info;
    UINT32                                ui4_nb_records;
    UINT32                                ui4_buf_agt_index;
    PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T    t_extra;
}   PLAYBACK_HANDLER_QUEUE_FILE_T;

/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_QUEUE_BUFFER_T
 *  @brief  This structure contains the queue buffer information and to be used
 *          with PLAYBACK_CTRL_SET_QUEUE_BUFFER
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_QUEUE_BUFFER_T
 *  {
 *      UINT32                                ui4_id;
 *      UINT32                                ui4_flags;
 *      DATA_FMT_T                            e_format;
 *      UINT64                                ui8_start_offset;
 *      UINT64                                ui8_end_offset;
 *      UINT64                                ui8_size;
 *      VOID*                                 pv_data;
 *      PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*    pt_scdb_info;
 *      UINT32                                ui4_nb_records;
 *      UINT32                                ui4_buf_agt_index;
 *      PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T    t_extra;
 *  } PLAYBACK_HANDLER_QUEUE_BUFFER_T;
 *  @endcode
 *
 *  @li@c   ui4_id            - ID of queued item (Out)
 *  @li@c   ui4_flags         - Flags
 *  @li@c   e_format          - Format
 *  @li@c   ui8_start_offset  - Start offset
 *  @li@c   ui8_end_offset    - End offset
 *  @li@c   ui8_size          - Size of buffer
 *  @li@c   pv_data           - Address of buffer
 *  @li@c   pt_scdb_info      - SCDB information
 *  @li@c   ui4_nb_records    - Number of records
 *  @li@c   ui4_buf_agt_index - Index of buffer agent index
 *  @li@c   t_extra           - Extra information
 */
/*---------------------------------------------------------------------------*/
typedef struct _PLAYBACK_HANDLER_QUEUE_BUFFER_T
{
    UINT32                                ui4_id;
    UINT32                                ui4_flags;
    DATA_FMT_T                            e_format;
    UINT64                                ui8_start_offset;
    UINT64                                ui8_end_offset;
    UINT64                                ui8_size;
    VOID*                                 pv_data;
    PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*    pt_scdb_info;
    UINT32                                ui4_nb_records;
    UINT32                                ui4_buf_agt_index;
    PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T    t_extra;
} PLAYBACK_HANDLER_QUEUE_BUFFER_T;



/*---------------------------------------------------------------------------*/
/*! @struct PLAYBACK_HANDLER_QUEUE_PULL_T
 *  @brief  This structure contains the queue PULL information and to be used
 *          with PLAYBACK_CTRL_SET_QUEUE_PULL
 *
 *  @code
 *  typedef struct _PLAYBACK_HANDLER_QUEUE_PULL_T
 *  {
 *      UINT32                                ui4_id;
 *      UINT32                                ui4_flags;
 *      DATA_FMT_T                            e_format;
 *      UINT64                                ui8_start_offset;
 *      UINT64                                ui8_end_offset;
 *      PLAYBACK_CTRL_PULL_INFO_T             t_pull_info;
 *      UINT64                                ui8_size;
 *      VOID*                                 pv_data;
 *      PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*    pt_scdb_info;
 *      UINT32                                ui4_nb_records;
 *      UINT32                                ui4_buf_agt_index;
 *      PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T    t_extra;
 *  } PLAYBACK_HANDLER_QUEUE_PULL_T;
 *  @endcode
 *
 *  @li@c   ui4_id            - ID of queued item (Out)
 *  @li@c   ui4_flags         - Flags
 *  @li@c   e_format          - Format
 *  @li@c   ui8_start_offset  - Start offset
 *  @li@c   ui8_end_offset    - End offset
 *  @li@c   t_pull_info       - pull info function pointer table
 *  @li@c   ui8_size          - Size of buffer
 *  @li@c   pv_data           - Address of buffer
 *  @li@c   pt_scdb_info      - SCDB information
 *  @li@c   ui4_nb_records    - Number of records
 *  @li@c   ui4_buf_agt_index - Index of buffer agent index
 *  @li@c   t_extra           - Extra information
 */
/*---------------------------------------------------------------------------*/
typedef struct _PLAYBACK_HANDLER_QUEUE_PULL_T
{
    UINT32                              ui4_id; /* out */
    UINT32                              ui4_flags;
    DATA_FMT_T                          e_format;
    UINT64                              ui8_start_offset;
    UINT64                              ui8_end_offset;
    PLAYBACK_CTRL_PULL_INFO_T           t_pull_info;
    PLAYBACK_HANDLER_ITEM_SCDB_INFO_T*  pt_scdb_info;
    UINT32                              ui4_nb_records;
    UINT32                              ui4_buf_agt_index;
    PLAYBACK_HANDLER_ITEM_EXTRA_INFO_T  t_extra;
}   PLAYBACK_HANDLER_QUEUE_PULL_T;


#define SUPPORT_PMT_MAX     16
typedef struct 
{
    MPEG_2_PID_T    t_pid;
    UINT16          ui2_svc_id;
}   PSI_PID_SVC_ID_MAP_INFO_T;

typedef struct
{
    UINT8                       ui1_pmt_num;
    PSI_PID_SVC_ID_MAP_INFO_T   at_pmt_map[SUPPORT_PMT_MAX];
}   PLAYBACK_PMT_LIST_T;



#define CM_PLAYBACK_FLAG_TRANSFER_WHEN_CONSUMED  ((UINT32) 0x00000001)    /**< Flags to use when queuing items - transfer when consumed */
#define CM_PLAYBACK_FLAG_KEEP_UNTIL_UNQUEUED     ((UINT32) 0x00000002)    /**< Flags to use when queuing items - keep until unqueud */
#define CM_PLAYBACK_FLAG_STICKY_SCDB             ((UINT32) 0x00000004)    /**< Flags to use when queuing items - sticky SCDB */
#define CM_PLAYBACK_FLAG_CANNOT_SET_OFFSET       ((UINT32) 0x00000008)    /**< Flags to use when queuing items - cannot set offset */
#define CM_PLAYBACK_FLAG_SHARED_MEMORY           ((UINT32) 0x00000010)    /**< Flags to use when queuing items - shared memory */

#define CM_PLAYBACK_CONN_INFO_ITEM_QUEUED          CM_HANDLER_COND(1)    /**< Playback handler's own conditions (CM_COND_T) - Item is queued */
#define CM_PLAYBACK_CONN_INFO_ITEM_UNQUEUED        CM_HANDLER_COND(2)    /**< Playback handler's own conditions (CM_COND_T) - Item is unqueued */
#define CM_PLAYBACK_CONN_INFO_ITEM_TRANSFER_START  CM_HANDLER_COND(3)    /**< Playback handler's own conditions (CM_COND_T) - Transfer item starts */
#define CM_PLAYBACK_CONN_INFO_ITEM_TRANSFER_END    CM_HANDLER_COND(4)    /**< Playback handler's own conditions (CM_COND_T) - Transfer item ends */
#define CM_PLAYBACK_CONN_INFO_ITEM_CONSUMED        CM_HANDLER_COND(5)    /**< Playback handler's own conditions (CM_COND_T) - Item is consumed */
#define CM_PLAYBACK_CONN_INFO_QUEUE_EMPTY          CM_HANDLER_COND(6)    /**< Playback handler's own conditions (CM_COND_T) - Queue is empty */
#define CM_PLAYBACK_CONN_INFO_FILE_ERROR           CM_HANDLER_COND(7)    /**< Playback handler's own conditions (CM_COND_T) - File error */
#define CM_PLAYBACK_CONN_INFO_UNQUEUE_INVALID_ID   CM_HANDLER_COND(8)    /**< Playback handler's own conditions (CM_COND_T) - Unqueue invalid ID */

#define CM_PLAYBACK_CONN_INFO_BUFFER_OVERFLOW      CM_HANDLER_COND(9)    /**< Playback handler's own conditions (CM_COND_T) - Buffer Overflow */
#define CM_PLAYBACK_CONN_INFO_BUFFER_UNDERFLOW     CM_HANDLER_COND(10)   /**< Playback handler's own conditions (CM_COND_T) - Buffer Underflow */
#define CM_PLAYBACK_CONN_INFO_EOF                  CM_HANDLER_COND(11)   /**< Playback handler's own conditions (CM_COND_T) - END OF FILE */
#define CM_PLAYBACK_CONN_INFO_PAT_GOT              CM_HANDLER_COND(12)   /**< Playback handler's own conditions (CM_COND_T) - Pat Got */


/*! @} */

#endif /* _U_PLAYBACK_HANDLER_H_ */

