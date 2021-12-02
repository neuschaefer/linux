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
/* ! @file u_vbi_fltr.h  
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description: 
 *         This header file contains public interfaces, constant and type 
 *         definitions of the vbi filter.
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_VBI_FLTR VBI Filter Module
  *  @ingroup groupMW
  *  @brief Provides external function TV Application
  *  %MW Application API would be used by TV Application, lot of these APIs would
  *   be wrapped from Exported API
  *  @see groupMW
  *  @{ 
  */
/*----------------------------------------------------------------------------*/
 

#ifndef _U_VBI_FLTR_H_
#define _U_VBI_FLTR_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_svl.h"
#include "u_common.h"
#include "u_sys_name.h"
#include "u_handle.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
typedef UINT32  VBIF_SET_TYPE_T;
/**<type definition of VBIF_SET_TYPE_T*/

typedef UINT32  VBIF_GET_TYPE_T;
/**<type definition of VBIF_GET_TYPE_T*/



/* set types */
#define VBIF_SET_TYPE_START             ((VBIF_SET_TYPE_T)   0)
/**<Set type of starting the vbi filter*/

#define VBIF_SET_TYPE_STOP              ((VBIF_SET_TYPE_T)   1)
/**<Set type of stopping the vbi filter*/


#define VBIF_SET_TYPE_LAST_ENTRY        ((VBIF_SET_TYPE_T) 256)
/**<The last entry of set type*/

/* get types */
/* VBIF_GET_TYPE_COND <VBIF_COND_T *, sizeof (VBIF_COND_T)> */
#define VBIF_GET_TYPE_COND              ((VBIF_GET_TYPE_T)   0)
/**<Get type of getting the condition of vbi filter*/

#define VBIF_GET_TYPE_LAST_ENTRY        ((VBIF_GET_TYPE_T) 256)
/**<The last entry of get type.*/


/* return value */
#define VBIFR_OK                        ((INT32)  0)    /**<Routine successful.*/
#define VBIFR_INV_ARG                   ((INT32) -1)    /**<Invalid arguments.*/
#define VBIFR_INV_HANDLE                ((INT32) -2)    /**<Invalid VBI filter handle..*/
#define VBIFR_INV_STATE                 ((INT32) -3)    /**<Invalid VBI filter state.*/
#define VBIFR_INV_SET_TYPE              ((INT32) -4)    /**<Invalid set type.*/
#define VBIFR_INV_GET_TYPE              ((INT32) -5)    /**<Invalid get type.*/
#define VBIFR_INV_BUFF                  ((INT32) -6)    /**<Invalid buffer.*/
#define VBIFR_INV_OP                    ((INT32) -7)    /**<Invalid operation.*/
#define VBIFR_SYS_INIT_ERROR            ((INT32) -8)    /**<Fail to initilize system.*/
#define VBIFR_LOCK_ERROR                ((INT32) -9)    /**<Internal lock error.*/
#define VBIFR_INTERNAL_ERROR            ((INT32) -10)   /**<Internal error.*/
#define VBIFR_DRV_ERROR                 ((INT32) -11)   /**<Driver encounters some error.*/
#define VBIFR_SCDB_ERROR                ((INT32) -12)   /**<SCDB encounters some error.*/
#define VBIFR_MSG_Q_OVERFLOW            ((INT32) -13)   /**<Message queue overflow.*/
#define VBIFR_EVENT_Q_OVERFLOW          ((INT32) -14)   /**<Event queue overflow.*/
#define VBIFR_BUFF_OVERFLOW             ((INT32) -15)   /**<Buffer overflow.*/
#define VBIFR_OUT_OF_MEMORY             ((INT32) -16)   /**<Out of memory.*/
#define VBIFR_OUT_OF_HANDLE             ((INT32) -17)   /**<Out of handle.*/
#define VBIFR_OUT_OF_BUFFER             ((INT32) -18)   /**<Out of buffer.*/
#define VBIFR_NOT_SUPPORT_FLTR_TYPE     ((INT32) -19)   /**<Filter type not supported.*/
#define VBIFR_NOT_SUPPORT_SRC_TYPE      ((INT32) -20)   /**<Source type not supported.*/
#define VBIFR_NOT_SUPPORT_FLTR_MODE     ((INT32) -21)   /**<Filter mode not supported.*/
#define VBIFR_NOT_SUPPORT_OP            ((INT32) -22)   /**<Operation is not supported.*/
#define VBIFR_NOT_SUPPORT_MODE          ((INT32) -23)   /**<Mode is not supported.*/
#define VBIFR_DUPLICATED_OP             ((INT32) -24)   /**<Operation is duplicated.*/
#define VBIFR_DATA_ERROR                ((INT32) -25)   /**<Data encounters some error.*/

#define VBIFR_LAST_RETURN_CODE          ((INT32) -100)  /**<The last return code*/

/* constants */
#define VBIF_PTS_IGNORE                 ((PTS_T) 0xFFFFFFFE00000000)
/**<The definition of PTS_IGNORE constants*/


/* (*x_vbif_nfy_fct) : e_cond */
/*------------------------------------------------------------------*/
/*! @brief     Definition of the vbi filter conditions.
  *  @code 
  *  typedef enum _VBIF_COND_T
  *  { 
  *      VBIF_COND_IGNORE, 
  *      VBIF_COND_OPENING,
  *      VBIF_COND_OPENED, 
  *      VBIF_COND_STARTING,
  *      VBIF_COND_STARTED,
  *      VBIF_COND_STOPPING,
  *      VBIF_COND_CLOSING,
  *      VBIF_COND_CLOSED,
  *      VBIF_COND_ERROR
  *  } VBIF_COND_T; 
  *  @endcode 
  *  @li@c  VBIF_COND_IGNORE         -VBI filter is in ingore condition(init status).
  *  @li@c  VBIF_COND_OPENING       -VBI filter is in opening condition.
  *  @li@c  VBIF_COND_OPENED        -VBI filter is in opened condition.
  *  @li@c  VBIF_COND_STARTING     -VBI filter is in starting condition.
  *  @li@c  VBIF_COND_STARTED      -VBI filter is in started condition.
  *  @li@c  VBIF_COND_STOPPING     -VBI filter is in stopping condition.
  *  @li@c  VBIF_COND_CLOSING       -VBI filter is in closing condition.
  *  @li@c  VBIF_COND_CLOSED        -VBI filter is in closed condition.
  *  @li@c  VBIF_COND_ERROR          -VBI filter is in error condition.
  */
/*------------------------------------------------------------------*/
typedef enum _VBIF_COND_T
{
    VBIF_COND_IGNORE,
    VBIF_COND_OPENING,
    VBIF_COND_OPENED,
    VBIF_COND_STARTING,
    VBIF_COND_STARTED,
    VBIF_COND_STOPPING,
    VBIF_COND_CLOSING,
    VBIF_COND_CLOSED,
    VBIF_COND_ERROR
    
} VBIF_COND_T;    

/* (*x_vbif_nfy_fct) : e_reason */
/*------------------------------------------------------------------*/
/*! @brief     Definition of the vbi filter notification reason.
  *  @code 
  *  typedef enum _VBIF_NFY_REASON_T
  *  { 
  *      NFY_REASON_IGNORE, 
  *      NFY_REASON_COND_CHANGED,
  *      NFY_REASON_DATA_AVAIL, 
  *      NFY_REASON_DISCONNECTED,
  *      NFY_REASON_NOT_AUTHORIZED,
  *      NFY_REASON_SCDB_REC_DEL,
  *      NFY_REASON_SCDB_CLOSED,
  *      NFY_REASON_NOT_SUPPORTED,
  *      NFY_REASON_PM_NFY,
  *      NFY_REASON_ERROR
  *  } VBIF_NFY_REASON_T; 
  *  @endcode 
  *  @li@c  NFY_REASON_IGNORE                    -Ingore.
  *  @li@c  NFY_REASON_COND_CHANGED      -Condition has been changed.
  *  @li@c  NFY_REASON_DATA_AVAIL            -Data is available.
  *  @li@c  NFY_REASON_DISCONNECTED       -Disconnected from RM.
  *  @li@c  NFY_REASON_NOT_AUTHORIZED   -Not authorized.
  *  @li@c  NFY_REASON_SCDB_REC_DEL       -The record has been deleted in SCDB.
  *  @li@c  NFY_REASON_SCDB_CLOSED        -The SCDB has been closed.
  *  @li@c  NFY_REASON_NOT_SUPPORTED     -Something is no supported under current condition.
  *  @li@c  NFY_REASON_PM_NFY                   -Get notification from PM.
  *  @li@c  NFY_REASON_ERROR                    -There is error occurs.  
  */
/*------------------------------------------------------------------*/
typedef enum _VBIF_NFY_REASON_T
{
    NFY_REASON_IGNORE = 0,
    NFY_REASON_COND_CHANGED,
    NFY_REASON_DATA_AVAIL,
    NFY_REASON_DISCONNECTED,  /* e.g. RM notify disconnected */
    NFY_REASON_NOT_AUTHORIZED,
    NFY_REASON_BUF_OVERFLOW,
    NFY_REASON_SCDB_REC_DEL,
    NFY_REASON_SCDB_CLOSED,
    NFY_REASON_NOT_SUPPORTED, /* e.g. Teltext Packets capturing is not supported by PIP(SUB) path of TV Decoder */
    NFY_REASON_PM_NFY,        
    NFY_REASON_ERROR          /* e.g. Buffer error */
    
} VBIF_NFY_REASON_T;

/* x_vbif_open() : e_src_type */
/*------------------------------------------------------------------*/
/*! @brief     Definition of the source of vbi filter.
  *  @code 
  *  typedef enum _VBIF_SRC_TYPE_T
  *  { 
  *      SRC_TYPE_ANA_VBI, 
  *      SRC_TYPE_DIG_VBI_ES,
  *      SRC_TYPE_DIG_TTX_ES
  *  } VBIF_SRC_TYPE_T; 
  *  @endcode 
  *  @li@c  SRC_TYPE_ANA_VBI           -Analog VBI data.
  *  @li@c  SRC_TYPE_DIG_VBI_ES      -Digital VBI element stream.
  *  @li@c  SRC_TYPE_DIG_TTX_ES      -Digital VBI element stream.
  */
/*------------------------------------------------------------------*/
typedef enum _VBIF_SRC_TYPE_T
{
    SRC_TYPE_ANA_VBI,
    SRC_TYPE_DIG_VBI_ES,
    SRC_TYPE_DIG_TTX_ES
    
} VBIF_SRC_TYPE_T;    

/* x_vbif_open() : e_fltr_type */
/*------------------------------------------------------------------*/
/*! @brief     Definition of the type of vbi filter.
  *  @code 
  *  typedef enum _VBIF_FLTR_TYPE_T
  *  { 
  *      FLTR_TYPE_TTX_PKT, 
  *      FLTR_TYPE_TTX_MIP,
  *      FLTR_TYPE_NI, 
  *      FLTR_TYPE_ACI,
  *      FLTR_TYPE_EU_EVN,
  *      FLTR_TYPE_EPG,
  *      FLTR_TYPE_EU_WSS,
  *      FLTR_TYPE_EU_VBI_D2A
  *  } VBIF_FLTR_TYPE_T; 
  *  @endcode 
  *  @li@c  FLTR_TYPE_TTX_PKT         -Teletext packet.
  *  @li@c  FLTR_TYPE_TTX_MIP         -Teletext MIP.
  *  @li@c  FLTR_TYPE_NI                  -NI.
  *  @li@c  FLTR_TYPE_ACI                -ACI.
  *  @li@c  FLTR_TYPE_EU_EVN          -EU Evn.
  *  @li@c  FLTR_TYPE_EPG               -EPG.
  *  @li@c  FLTR_TYPE_EU_WSS         -EU Wss.
  *  @li@c  FLTR_TYPE_EU_VBI_D2A   -EU Vbi D2a.
  */
/*------------------------------------------------------------------*/
typedef enum _VBIF_FLTR_TYPE_T
{
    FLTR_TYPE_TTX_PKT,
    FLTR_TYPE_TTX_MIP,
    FLTR_TYPE_NI,
    FLTR_TYPE_ACI,
    FLTR_TYPE_EU_EVN,
    FLTR_TYPE_EPG,
    FLTR_TYPE_EU_WSS,
    FLTR_TYPE_EU_VBI_D2A
    
} VBIF_FLTR_TYPE_T;

/* x_vbif_open() : e_fltr_mode */
/*------------------------------------------------------------------*/
/*! @brief     Definition of the mode of vbi filter.
  *  @code 
  *  typedef enum _VBIF_FLTR_MODE_T
  *  { 
  *      FLTR_MODE_LOAD, 
  *      FLTR_MODE_MONITOR,
  *      FLTR_MODE_PUMP
  *  } VBIF_FLTR_MODE_T; 
  *  @endcode 
  *  @li@c  FLTR_MODE_LOAD         -Load the data, notify ONLY once and notify ONLY when data is first received.
  *  @li@c  FLTR_MODE_MONITOR   -Monitor the content of data, notify whenever data is changed
  *  @li@c  FLTR_MODE_PUMP         -pump the data to client whenever data is received.
  */
/*------------------------------------------------------------------*/
typedef enum _VBIF_FLTR_MODE_T
{
    FLTR_MODE_LOAD,
    FLTR_MODE_MONITOR,
    FLTR_MODE_PUMP
    
} VBIF_FLTR_MODE_T;     


/*-----------------------------------------------------------------------------
                    notify functions
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: (*x_vbif_nfy_fct)
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------*/
/*! @brief  This type defines the vbi filter notify function function prototype.
  *  @param [in] h_filter    Contain the handle of vbi filter.
  *  @param [in] e_cond    Contain the condition of the vbi filter.
  *  @param [in] e_reason Contain the reason of the notification.
  *  @param [in] pv_tag     Contain the tag specificed by user.
  *  @param [in] ui4_data  Contain the data.
  */
/*------------------------------------------------------------------*/
typedef VOID (*x_vbif_nfy_fct)(
    HANDLE_T                    h_filter,
    VBIF_COND_T                 e_cond,
    VBIF_NFY_REASON_T           e_reason,
    VOID*                       pv_tag,
    UINT32                      ui4_data);


/*-----------------------------------------------------------------------------
                    structures (generic)
 ----------------------------------------------------------------------------*/

/* x_vbif_open ( e_src_type == SRC_TYPE_ANA_VBI,    */
/*               pv_src_info => (VBIF_SRC_ANA_VBI_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Contains the analog vbi data source info. It is used whent he source type is SRC_TYPE_ANA_VBI.
 *  @code
 *  typedef struct _VBIF_SRC_ANA_VBI_T
 *  {
 *      BOOL                    b_sel_src_or_conn_handle;
 *      HANDLE_T             h_ref;
 *  } VBIF_SRC_ANA_VBI_T;
 *  @endcode
 *  @li@c b_sel_src_or_conn_handle  - Indicates h_ref is connection handle or the source component handle.
 *  @li@c h_ref                                 -The handle of source.
 */
/*----------------------------------------------------------------------------*/
typedef struct _VBIF_SRC_ANA_VBI_T
{
    /* FALSE: h_ref=>connection_handle TRUE: h_ref=>src_component_handle */
    BOOL                    b_sel_src_or_conn_handle;
    
    /* connection handle or source component handle */
    HANDLE_T                h_ref;
    
} VBIF_SRC_ANA_VBI_T;

/*  x_vbif_open( e_src_type == SRC_TYPE_DIG_VBI_ES,    */
/*               pv_src_info => (VBIF_SRC_DIG_VBI_ES_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Contains the digital vbi element stream source info. It is used whent he source type is SRC_TYPE_DIG_VBI_ES.
 *  @code
 *  typedef struct _VBIF_SRC_DIG_VBI_ES_T
 *  {
 *      HANDLE_T                h_connection;
 *      HANDLE_T                h_pm_service;
 *      MPEG_2_PID_T         t_pid;
 *  } VBIF_SRC_DIG_VBI_ES_T;
 *  @endcode
 *  @li@c h_connection   - The handle of connection for opening SCDB.
 *  @li@c h_pm_service - The handle of PM service 
 *  @li@c t_pid              -The pid for identifing the VBI ES related records in scdb.
 */
/*----------------------------------------------------------------------------*/
typedef struct _VBIF_SRC_DIG_VBI_ES_T
{
    /* connection handle : for opening SCDB */
    HANDLE_T                h_connection;
    
    /* PM service handle */
    HANDLE_T                h_pm_service;

    /* pid: identify the VBI ES related records in scdb */
    MPEG_2_PID_T            t_pid;
    
} VBIF_SRC_DIG_VBI_ES_T;

/* x_vbif_open( e_src_type == SRC_TYPE_DIG_TTX_ES,    */
/*              pv_src_info => (VBIF_SRC_DIG_TTX_ES_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Contains the digital teletext element stream source info. It is used whent he source type is SRC_TYPE_DIG_TTX_ES.
 *  @code
 *  typedef struct _VBIF_SRC_DIG_TTX_ES_T
 *  {
 *      HANDLE_T                h_connection;
 *      HANDLE_T                h_pm_service;
 *      MPEG_2_PID_T         t_pid;
 *  } VBIF_SRC_DIG_TTX_ES_T;
 *  @endcode
 *  @li@c h_connection   - The handle of connection for opening SCDB.
 *  @li@c h_pm_service - The handle of PM service 
 *  @li@c t_pid              -The pid for identifing the TTX ES related records in scdb.
 */
/*----------------------------------------------------------------------------*/
typedef struct _VBIF_SRC_DIG_TTX_ES_T
{
    /* connection handle : for opening SCDB */
    HANDLE_T                h_connection;

    /* PM service handle */
    HANDLE_T                h_pm_service;
    
    /* pid: identify the TTX ES related records in scdb */
    MPEG_2_PID_T            t_pid;

} VBIF_SRC_DIG_TTX_ES_T;


/* (*x_vbif_nfy_fct) ( e_reason == NFY_REASON_PM_NFY,  */
/*                     ui4_data => (VBIF_PM_NFY_T *) ) */
/*   ==> Refer to vbi_fltr.h                           */


/*-----------------------------------------------------------------------------
                macros, defines, typedefs, enums, struct (filters)
 ----------------------------------------------------------------------------*/



/* Teletext Packet Filter (FLTR_TYPE_TTX_PKT) ****************************************/



/* specific set types */

/* <FLTR_TTX_PKT_BUFF_LOCK_INFO_T *> */
#define VBIF_SET_TYPE_TTX_PKT_LOCK_BUFF     (VBIF_SET_TYPE_LAST_ENTRY + ((VBIF_SET_TYPE_T) 0))
/**<VBI Filter set type to lock teletext packet buffer*/


/* <FLTR_TTX_PKT_BUFF_TAG_T *> */
#define VBIF_SET_TYPE_TTX_PKT_UNLOCK_BUFF   (VBIF_SET_TYPE_LAST_ENTRY + ((VBIF_SET_TYPE_T) 1))
/**<VBI Filter set type to unlock teletext packet buffer*/



/* <FLTR_TTX_PKT_BUFF_TAG_T *> */
#define VBIF_SET_TYPE_TTX_PKT_FREE_BUFF     (VBIF_SET_TYPE_LAST_ENTRY + ((VBIF_SET_TYPE_T) 2))
/**<VBI Filter set type to free teletext packet buffer*/


/* <NULL> */
#define VBIF_SET_TYPE_TTX_PKT_FLUSH_BUFF    (VBIF_SET_TYPE_LAST_ENTRY + ((VBIF_SET_TYPE_T) 3))
/**<VBI Filter set type to free teletext packet buffer*/


/* specific get types */
/* <FLTR_TTX_PKT_BUFF_ATTR_T *> */
#define VBIF_GET_TYPE_TTX_PKT_BUFF_ATTR     (VBIF_GET_TYPE_LAST_ENTRY + ((VBIF_GET_TYPE_T) 0))
/**<VBI Filter get type to get  teletext packet buffer attribute*/



/* x_vbif_open (e_fltr_type == FLTR_TYPE_TTX_PKT,        */
/*              pv_fltr_info : (FLTR_INIT_TTX_PKT_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet initialization info when do vbi filter opening, if the filter type is FLTR_TYPE_TTX_PKT. 
 *  @code
 *  typedef struct _FLTR_INIT_TTX_PKT_T
 *  {
 *      UINT32                  ui4_reserved;
 *  } FLTR_INIT_TTX_PKT_T;
 *  @endcode
 *  @li@c ui4_reserved   - Currently not used.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_INIT_TTX_PKT_T
{
    /* currently not used */
    UINT32                  ui4_reserved;
    
} FLTR_INIT_TTX_PKT_T;

/* compatible with TV_DEC_TTX_PKT_T */
/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet initialization info when do vbi filter opening, if the filter type is FLTR_TYPE_TTX_PKT. 
 *  @code
 *  typedef struct _TTX_PKT_INFO_T
 *  {
 *      UINT32                 ui4_padding;
 *      UINT8                   aui1_data[42];
 *  } TTX_PKT_INFO_T;
 *  @endcode
 *  @li@c ui4_padding       - Padding variable.
 *  @li@c aui1_data[42]    - The teletext packet data.
 */
/*----------------------------------------------------------------------------*/
typedef struct _TTX_PKT_INFO_T
{
    UINT32                  ui4_padding;  
    UINT8                   aui1_data[42];
    
} TTX_PKT_INFO_T;    



/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet data which source is analog vbi data.
 *  @code
 *  typedef struct _FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T
 *  {
 *      TTX_PKT_INFO_T*      pat_pkt_buf;
 *      UINT16                      ui2_num_pkt;
 *  } FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T;
 *  @endcode
 *  @li@c pat_pkt_buf        - The buffer contaions the teletext packet data.
 *  @li@c ui2_num_pkt       - The number of teletext packet.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T
{
    TTX_PKT_INFO_T*         pat_pkt_buf;
    UINT16                  ui2_num_pkt;
    
} FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T;


/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet buffer attribute which source is analog vbi data.
 *  @code
 *  typedef struct _FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T
 *  {
 *      UINT16                      ui2_num_pkt;
 *  } FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T;
 *  @endcode
 *  @li@c ui2_num_pkt       - The number of teletext packet.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T
{
    UINT16                  ui2_num_pkt;
    
} FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T;


/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet data which source is digital vbi element stream.
 *  @code
 *  typedef struct _FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T
 *  {
 *      PTS_T                   t_pts;
 *      VOID*                   pv_data_pes_pkt;
 *      SIZE_T                  z_len_data;
 *  } FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T;
 *  @endcode
 *  @li@c t_pts                      -The pts of teletext packet.
 *  @li@c pv_data_pes_pkt    -The pes data packet.
 *  @li@c z_len_data             -The length of pes data packet. 
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T
{
    PTS_T                   t_pts;
    
    VOID*                   pv_data_pes_pkt;
    SIZE_T                  z_len_data;

} FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T;


/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet buffer attribute which source is digital vbi element stream.
 *  @code
 *  typedef struct _FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T
 *  {
 *      PTS_T                   t_pts;
 *      SIZE_T                  z_len_data;
 *  } FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T;
 *  @endcode
 *  @li@c t_pts                      -The pts of teletext packet.
 *  @li@c z_len_data             -The length of pes data packet. 
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T
{
    PTS_T                   t_pts;
    SIZE_T                  z_len_data;
    
} FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T;


/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet data which source is digital teletext element stream.
 *  @code
 *  typedef struct _FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T
 *  {
 *      PTS_T                   t_pts;
 *      VOID*                   pv_data_pes_pkt;
 *      SIZE_T                  z_len_data;
 *  } FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T;
 *  @endcode
 *  @li@c t_pts                      -The pts of teletext packet.
 *  @li@c pv_data_pes_pkt    -The pes data packet.
 *  @li@c z_len_data             -The length of pes data packet. 
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T
{
    PTS_T                   t_pts;
    
    VOID*                   pv_data_pes_pkt;
    SIZE_T                  z_len_data;
    
} FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T;


/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet buffer attribute which source is digital teletext element stream.
 *  @code
 *  typedef struct _FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T
 *  {
 *      PTS_T                   t_pts;
 *      SIZE_T                  z_len_data;
 *  } FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T;
 *  @endcode
 *  @li@c t_pts                      -The pts of teletext packet.
 *  @li@c z_len_data             -The length of pes data packet. 
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T
{
    PTS_T                   t_pts;
    SIZE_T                  z_len_data;
    
} FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T;



/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet buffer information.
 *  @code
 *  typedef struct _FLTR_TTX_PKT_BUFF_INFO_T
 *  {
 *      VBIF_SRC_TYPE_T         e_src_type;
 *   
 *      union 
 *      {
 *          FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T      t_src_ana_vbi;
 *          FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T   t_src_dig_vbi_es;
 *          FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T   t_src_dig_ttx_es;
 *      } u;
 *  } FLTR_TTX_PKT_BUFF_INFO_T;
 *  @endcode
 *  @li@c e_src_type                      -Indicates the teletext packet source type.
 *  @li@c t_src_ana_vbi                 -The analog vbi data buffer information.
 *  @li@c t_src_dig_vbi_es             -The digital vbi element stream buffer information.
 *  @li@c t_src_dig_ttx_es             -The digital teletext element stream buffer information.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_TTX_PKT_BUFF_INFO_T
{
    VBIF_SRC_TYPE_T         e_src_type;
    
    union 
    {
        FLTR_NFY_TTX_PKT_SRC_ANA_VBI_T      t_src_ana_vbi;
        FLTR_NFY_TTX_PKT_SRC_DIG_VBI_ES_T   t_src_dig_vbi_es;
        FLTR_NFY_TTX_PKT_SRC_DIG_TTX_ES_T   t_src_dig_ttx_es;
    } u;
    
} FLTR_TTX_PKT_BUFF_INFO_T;

/* (1) x_vbif_set ( e_set_type == VBIF_SET_TYPE_TTX_PKT_UNLOCK_BUFF,  */
/*                  pv_set_info : (FLTR_TTX_PKT_BUFF_TAG_T *) )       */
/* (2) x_vbif_set ( e_set_type == VBIF_SET_TYPE_TTX_PKT_FREE_BUFF,    */
/*                  pv_set_info : (FLTR_TTX_PKT_BUFF_TAG_T *) )       */
#define FLTR_TTX_PKT_BUFF_TAG_SIZE  32 
/**<The definition of teletext packet buffer tag size*/


/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet buffer tag information.
 *  @code
 *  typedef struct _FLTR_TTX_PKT_BUFF_TAG_T
 *  {
 *      UINT32       aui4_tag[(FLTR_TTX_PKT_BUFF_TAG_SIZE + 3) / 4];
 *  } FLTR_TTX_PKT_BUFF_TAG_T;
 *  @endcode
 *  @li@c aui4_tag                      -Indicates the tag of teletext packet buffer.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_TTX_PKT_BUFF_TAG_T
{
    UINT32       aui4_tag[(FLTR_TTX_PKT_BUFF_TAG_SIZE + 3) / 4];
    
} FLTR_TTX_PKT_BUFF_TAG_T;

/* x_vbif_set ( e_set_type == VBIF_SET_TYPE_TTX_PKT_LOCK_BUFF,     */
/*              pv_set_info : (FLTR_TTX_PKT_BUFF_LOCK_INFO_T *) )  */
/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet buffer attribute which source is digital teletext element stream.
 *  @code
 *  typedef struct _FLTR_TTX_PKT_BUFF_LOCK_INFO_T
 *  {
 *      const FLTR_TTX_PKT_BUFF_TAG_T*      pt_buf_tag;
 *      FLTR_TTX_PKT_BUFF_INFO_T*           pt_buf_info;
 *  } FLTR_TTX_PKT_BUFF_LOCK_INFO_T;
 *  @endcode
 *  @li@c pt_buf_tag              -The buffer tag as input parameter.
 *  @li@c pt_buf_info             -The buffer contains the actuall data. 
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_TTX_PKT_BUFF_LOCK_INFO_T
{
    const FLTR_TTX_PKT_BUFF_TAG_T*      pt_buf_tag;  /* in */
    
    FLTR_TTX_PKT_BUFF_INFO_T*           pt_buf_info; /* out */
    
} FLTR_TTX_PKT_BUFF_LOCK_INFO_T;


/* x_vbif_get (e_get_type == VBIF_GET_TYPE_TTX_PKT_BUFF_ATTR, */
/*             pv_get_info : (FLTR_TTX_PKT_BUFF_ATTR_T *)   ) */

/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet buffer attribute.
 *  @code
 *  typedef struct _FLTR_TTX_PKT_BUFF_ATTR_T
 *  {
 *      const FLTR_TTX_PKT_BUFF_TAG_T*     pt_buf_tag;
 *      VBIF_SRC_TYPE_T                             e_src_type;
 *      union           
 *      {
 *          FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T      t_ana_vbi;
 *          FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T   t_dig_vbi_es;
 *          FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T   t_dig_ttx_es;
 *      } u;
 *  } FLTR_TTX_PKT_BUFF_ATTR_T;
 *  @endcode
 *  @li@c pt_buf_tag              -The buffer tag as input parameter.
 *  @li@c e_src_type              -The output parameter to indicates the actual source type.
 *  @li@c t_ana_vbi                -The attribute info of analog vbi data. 
 *  @li@c t_dig_vbi_es            -The attribute info of digital vbi element stream.
 *  @li@c t_dig_ttx_es            -The attribute info of digital teletext element stream. 
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_TTX_PKT_BUFF_ATTR_T
{
    const FLTR_TTX_PKT_BUFF_TAG_T*      pt_buf_tag;  /* in */
    
    VBIF_SRC_TYPE_T                     e_src_type;  /* out */
    
    union           
    {
        FLTR_TTX_PKT_BUFF_ATTR_ANA_VBI_T      t_ana_vbi;
        FLTR_TTX_PKT_BUFF_ATTR_DIG_VBI_ES_T   t_dig_vbi_es;
        FLTR_TTX_PKT_BUFF_ATTR_DIG_TTX_ES_T   t_dig_ttx_es;
    } u;                                            /* out */
    
} FLTR_TTX_PKT_BUFF_ATTR_T;    

/* (*x_vbif_nfy_fct) ( e_cond   == VBIF_COND_STARTED,       */
/*                     e_reason == NFY_REASON_DATA_AVAIL,   */
/*                     ui4_data => (FLTR_NFY_TTX_PKT_T *) ) */


/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext packet buffer attribute.
 *  @code
 *  typedef struct _FLTR_NFY_TTX_PKT_T
 *  {
 *      FLTR_TTX_PKT_BUFF_TAG_T    t_buf_tag;
 *  } FLTR_NFY_TTX_PKT_T;
 *  @endcode
 *  @li@c t_buf_tag              -The teletext packet buffer tag.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_TTX_PKT_T
{
    FLTR_TTX_PKT_BUFF_TAG_T    t_buf_tag;
    
} FLTR_NFY_TTX_PKT_T;
    







/* Teletext MIP Filter (FLTR_TYPE_TTX_MIP)  ********************************************/
/* specific get types */
#define VBIF_GET_TYPE_TTX_MIP_DATA       (VBIF_GET_TYPE_LAST_ENTRY + ((VBIF_GET_TYPE_T) 0))
/**<Get type of getting the teletext mip data */


/* x_vbif_open ( e_fltr_type == FLTR_TYPE_TTX_MIP,        */
/*               pv_fltr_info : (FLTR_INIT_TTX_MIP_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext mip initialization info when do vbi filter opening, if the filter type is FLTR_TYPE_TTX_MIP. 
 *  @code
 *  typedef struct _FLTR_INIT_TTX_MIP_T
 *  {
 *      UINT32                  ui4_reserved;
 *  } FLTR_INIT_TTX_MIP_T;
 *  @endcode
 *  @li@c ui4_reserved   - Currently not used.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_INIT_TTX_MIP_T
{
    /* currently not used */
    UINT32                  ui4_reserved;
    
} FLTR_INIT_TTX_MIP_T;


/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext mip node information. 
 *  @code
 *  typedef struct _TTX_MIP_NODE_T
 *  {
 *      UINT8                   ui1_page_code;
 *      UINT16                  ui2_num_subpage;
 *  } TTX_MIP_NODE_T;
 *  @endcode
 *  @li@c ui1_page_code         -Reference to the page code.
 *  @li@c ui2_num_subpage    -The number of subpage. 
 */
/*----------------------------------------------------------------------------*/
typedef struct _TTX_MIP_NODE_T
{
    UINT8                   ui1_page_code;          
    
    UINT16                  ui2_num_subpage;
    
} TTX_MIP_NODE_T;    

/* x_vbif_get ( e_get_type == VBIF_GET_TYPE_TTX_MIP_DATA,      */
/*              pv_get_info => (FLTR_NFY_TTX_MIP_T *) )        */

/*----------------------------------------------------------------------------*/
/*! @brief  structure of teletext mip node information. 
 *  @code
 *  typedef struct _FLTR_NFY_TTX_MIP_T
 *  {
 *      VOID*                       pv_tag_mip; 
 *      UINT8                       ui1_id_mgz; 
 *      TTX_MIP_NODE_T       at_nodes[256]; 
 *  } FLTR_NFY_TTX_MIP_T;
 *  @endcode
 *  @li@c pv_tag_mip    -The mip tag as input parameter.
 *  @li@c ui1_id_mgz     -The id of maganize as output parameter. 
 *  @li@c at_nodes        -The node information as output parameter.  
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_TTX_MIP_T
{
    /* MIP data tag  (*x_vbif_nfy_fct) : ui4_data -> pv_tag_mip */
    VOID*                   pv_tag_mip;         /* IN */
    
    /* Magazine ID: 1~8 */
    UINT8                   ui1_id_mgz;         /* OUT */
    
    /* nodes info */
    TTX_MIP_NODE_T          at_nodes[256];      /* OUT */
    
} FLTR_NFY_TTX_MIP_T;










/* CNI / NI Filter (FLTR_TYPE_NI) **************************************************/

/* FLTR_TYPE_NI filter specific return values */
#define VBIFR_INV_DB_VERSION                            (VBIFR_LAST_RETURN_CODE - ((INT32) 1))
/**<The return value that databace version is invalid.*/

#define VBIFR_DB_NOT_READY                              (VBIFR_LAST_RETURN_CODE - ((INT32) 2))
/**<The return value that database is not ready.*/


/* FLTR_NI_T.e_src_mask */
typedef UINT8   FLTR_NI_SRC_TYPE_T;
/**<The type definition of NI source type*/


#define FLTR_NI_SRC_TYPE_NI_TTX_8_30_FMT_1      MAKE_BIT_MASK_8(0)
/**<NI source type is NI_TTX_8_30_FMT_1*/

#define FLTR_NI_SRC_TYPE_CNI_VPS                MAKE_BIT_MASK_8(1)
/**<NI source type is CNI_VPS*/

#define FLTR_NI_SRC_TYPE_CNI_TTX_8_30_FMT_2     MAKE_BIT_MASK_8(2)
/**<NI source type is CNI_TTX_8_30_FMT_2*/

#define FLTR_NI_SRC_TYPE_TTX_HDR_CH_NAME        MAKE_BIT_MASK_8(3)
/**<NI source type is TTX_HDR_CH_NAME*/

#define FLTR_NI_SRC_TYPE_VPS_DATA               MAKE_BIT_MASK_8(4)
/**<NI source type is VPS_DATA*/

#define FLTR_NI_SRC_TYPE_8_30_FMT_1_PRG_TTL     MAKE_BIT_MASK_8(5)
/**<NI source type is 8_30_FMT_1_PRG_TTL*/

#define FLTR_NI_SRC_TYPE_TTX_DATE_TIME          MAKE_BIT_MASK_8(6)
/**<NI source type is NI_TTX_DATA_TIME*/

#define FLTR_NI_SRC_TYPE_ACI                    MAKE_BIT_MASK_8(7)
/**<NI source type is ACI*/


/* get types range */
#define VBIF_NI_GET_TYPE_START_ACI                 (VBIF_GET_TYPE_LAST_ENTRY + 0)
/**<NI get type of starting ACI*/



/* ACI specific get types (FLTR_NI_SRC_TYPE_ACI) */

/* IN/OUT: <FLTR_NI_ACI_DB_INFO_T *> (Only valid when DB is ready) */
#define VBIF_GET_TYPE_NI_ACI_DB_INFO                    (VBIF_NI_GET_TYPE_START_ACI + ((VBIF_GET_TYPE_T) 0))
/**<ACI specific get type of database info, only valid when DB is ready*/

/* IN/OUT: <FLTR_NI_ACI_CITY_LIST_INFO_T *> (Only valid when DB is ready and in Multi-package mode) */
#define VBIF_GET_TYPE_NI_ACI_CITY_LIST_INFO             (VBIF_NI_GET_TYPE_START_ACI + ((VBIF_GET_TYPE_T) 1))
/**<ACI specific get type of city list info, only valid when DB is ready and in Multi-package mode*/

/* IN/OUT: <FLTR_NI_ACI_CITY_INFO_T *> (Only valid when DB is ready and in Multi-package mode) */
#define VBIF_GET_TYPE_NI_ACI_CITY_INFO                  (VBIF_NI_GET_TYPE_START_ACI + ((VBIF_GET_TYPE_T) 2))
/**<ACI specific get type of city info, only valid when DB is ready and in Multi-package mode*/

/* IN/OUT: <FLTR_NI_ACI_CH_NUM_T *> (Only valid when DB is ready) */
#define VBIF_GET_TYPE_NI_ACI_CH_NUM                     (VBIF_NI_GET_TYPE_START_ACI + ((VBIF_GET_TYPE_T) 3))
/**<ACI specific get type of channel number, only valid when DB is ready*/

/* IN/OUT: <FLTR_NI_ACI_CH_INFO_T *> (Only valid when DB is ready) */
#define VBIF_GET_TYPE_NI_ACI_CH_INFO                    (VBIF_NI_GET_TYPE_START_ACI + ((VBIF_GET_TYPE_T) 4))
/**<ACI specific get type of cyannel info, only valid when DB is ready*/


/* ACI originator-type */
typedef UINT8   FLTR_NI_ACI_ORIGR_TYPE_T;
/**<typedef of ACI originator-type*/


#define FLTR_NI_ACI_ORIGR_TYPE_CABLE            ((FLTR_NI_ACI_ORIGR_TYPE_T) 0x00)
/**<ACI originator-type of cable*/

#define FLTR_NI_ACI_ORIGR_TYPE_SATELLITE        ((FLTR_NI_ACI_ORIGR_TYPE_T) 0x01)
/**<ACI originator-type of satellite*/

#define FLTR_NI_ACI_ORIGR_TYPE_TERRESTRIAL      ((FLTR_NI_ACI_ORIGR_TYPE_T) 0x02)
/**<ACI originator-type of terrestrial*/

#define FLTR_NI_ACI_ORIGR_TYPE_IGNORE           ((FLTR_NI_ACI_ORIGR_TYPE_T) 0xFF)
/**<ACI originator-type ingore*/



/* ACI originator-type mask */
typedef UINT8   FLTR_NI_ACI_ORIGR_TYPE_MASK_T;  
/**<typedef of ACI originator-type mask */

#define MAKE_ORIGR_TYPE_MASK(_e_origr)          MAKE_BIT_MASK_8(_e_origr)
/**<Make the mask of ACI originator-type mask*/


#define FLTR_NI_ACI_ORIGR_TYPE_MASK_CABLE       MAKE_ORIGR_TYPE_MASK(FLTR_NI_ACI_ORIGR_TYPE_CABLE)
/**<Cable mask of ACI originator-type */

#define FLTR_NI_ACI_ORIGR_TYPE_MASK_SATELLITE   MAKE_ORIGR_TYPE_MASK(FLTR_NI_ACI_ORIGR_TYPE_SATELLITE)
/**<Satellite mask of ACI originator-type */

#define FLTR_NI_ACI_ORIGR_TYPE_MASK_TERRESTRIAL MAKE_ORIGR_TYPE_MASK(FLTR_NI_ACI_ORIGR_TYPE_TERRESTRIAL)
/**<Terrestrial mask of ACI originator-type */


#define FLTR_NI_ACI_ORIGR_TYPE_MASK_ALL         (FLTR_NI_ACI_ORIGR_TYPE_MASK_CABLE      |   \
                                                 FLTR_NI_ACI_ORIGR_TYPE_MASK_SATELLITE  |   \
                                                 FLTR_NI_ACI_ORIGR_TYPE_MASK_TERRESTRIAL)
/**<All mask of ACI originator-type */

/* ACI network-type */
typedef UINT8   FLTR_NI_ACI_NETWORK_TYPE_T;
/**<typedef of ACI network-type */


#define FLTR_NI_ACI_NETWORK_TYPE_CABLE          ((FLTR_NI_ACI_NETWORK_TYPE_T) 0x00)
/**<ACI network-type of cable*/

#define FLTR_NI_ACI_NETWORK_TYPE_SATELLITE      ((FLTR_NI_ACI_NETWORK_TYPE_T) 0x01)
/**<ACI network-type of satellite*/

#define FLTR_NI_ACI_NETWORK_TYPE_TERRESTRIAL    ((FLTR_NI_ACI_NETWORK_TYPE_T) 0x02)
/**<ACI network-type of terrestrial*/

#define FLTR_NI_ACI_NETWORK_TYPE_IGNORE         ((FLTR_NI_ACI_NETWORK_TYPE_T) 0xFF)
/**<ACI network-type of ingore*/



/* ACI network-type mask */
typedef UINT8   FLTR_NI_ACI_NETWORK_TYPE_MASK_T;
/**<typedef of ACI network-type mask */


#define MAKE_NETWORK_TYPE_MASK(_e_network)          MAKE_BIT_MASK_8(_e_network)
/**<Make the mask of ACI network-type mask*/


#define FLTR_NI_ACI_NETWORK_TYPE_MASK_CABLE         MAKE_NETWORK_TYPE_MASK(FLTR_NI_ACI_NETWORK_TYPE_CABLE)
/**<Cable mask of ACI network-type */

#define FLTR_NI_ACI_NETWORK_TYPE_MASK_SATELLITE     MAKE_NETWORK_TYPE_MASK(FLTR_NI_ACI_NETWORK_TYPE_SATELLITE)
/**<Satellite mask of ACI network-type */

#define FLTR_NI_ACI_NETWORK_TYPE_MASK_TERRESTRIAL   MAKE_NETWORK_TYPE_MASK(FLTR_NI_ACI_NETWORK_TYPE_TERRESTRIAL)
/**<Terrestrial mask of ACI network-type */

#define FLTR_NI_ACI_NETWORK_TYPE_MASK_ALL           (FLTR_NI_ACI_NETWORK_TYPE_MASK_CABLE     |      \
                                                     FLTR_NI_ACI_NETWORK_TYPE_MASK_SATELLITE |      \
                                                     FLTR_NI_ACI_NETWORK_TYPE_MASK_TERRESTRIAL )
/**<All mask of ACI network-type */

/* ACI package modes */
typedef UINT8   FLTR_NI_ACI_PKG_MODE_T; 
/**<typedef of ACI package modes*/

#define FLTR_NI_ACI_PKG_MODE_SINGLE             ((FLTR_NI_ACI_PKG_MODE_T) 0x00)
/**<SINGLE mode of ACI package*/

#define FLTR_NI_ACI_PKG_MODE_MULTIPLE           ((FLTR_NI_ACI_PKG_MODE_T) 0x01)
/**<MULTIPLE mode of ACI package*/

#define FLTR_NI_ACI_PKT_MODE_INVALID            ((FLTR_NI_ACI_PKG_MODE_T) 0xFF)
/**<INVALID mode of ACI package*/


/* ACI service type */
typedef UINT8   FLTR_NI_ACI_SVC_TYPE_T; 
/**<typedef of ACI service type*/

#define FLTR_NI_ACI_SVC_TYPE_PAL_SECAM_BG       ((FLTR_NI_ACI_SVC_TYPE_T) 0x00)
/**<PAL_SECAM_BG type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_SECAM_L            ((FLTR_NI_ACI_SVC_TYPE_T) 0x01)
/**<SECAM_L type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_PAL_M              ((FLTR_NI_ACI_SVC_TYPE_T) 0x02)
/**<PAL_M type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_NTSC_M             ((FLTR_NI_ACI_SVC_TYPE_T) 0x03)
/**<NTSC_M type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_PAL_I              ((FLTR_NI_ACI_SVC_TYPE_T) 0x04)
/**<PAL_I type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_SECAM_DK           ((FLTR_NI_ACI_SVC_TYPE_T) 0x05)
/**<SECAM_DK type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_UNKNOWN            ((FLTR_NI_ACI_SVC_TYPE_T) 0x06)
/**<Unknown type of ACI service*/


#define FLTR_NI_ACI_SVC_TYPE_RADIO_FM           ((FLTR_NI_ACI_SVC_TYPE_T) 0x40)
/**<RAIDO_FM type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_RADIO_DSR          ((FLTR_NI_ACI_SVC_TYPE_T) 0x41)
/**<RADIO_DSR type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_RADIO_DAB          ((FLTR_NI_ACI_SVC_TYPE_T) 0x42)
/**<RADIO_DAB type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_RADIO_DCR          ((FLTR_NI_ACI_SVC_TYPE_T) 0x43)
/**<RADIO_DCR type of ACI service*/

#define FLTR_NI_ACI_SVC_TYPE_RADIO_UNKNOWN      ((FLTR_NI_ACI_SVC_TYPE_T) 0x44)
/**<Radio unknown type of ACI service*/



/* ACI notification code */
typedef UINT8   FLTR_NI_NFY_CODE_ACI_T; 
/**<typedef of ACI notification code*/

#define FLTR_NI_ACI_NFY_CODE_DETECTED           ((FLTR_NI_NFY_CODE_ACI_T) 0x01)
/**<ACI notification of DETECTED*/

#define FLTR_NI_ACI_NFY_CODE_SLCTN_PAGE_NUM     ((FLTR_NI_NFY_CODE_ACI_T) 0x02)
/**<ACI notification of SLCTN_PAGE_NUM*/

#define FLTR_NI_ACI_NFY_CODE_DB_UPDATED         ((FLTR_NI_NFY_CODE_ACI_T) 0x03)
/**<ACI notification of DB_UPDATED*/


/* callback for parsing channel name in teletext header */
/* using VBI filter's thread context, should return as quickly as possible */
/*------------------------------------------------------------------*/
/*! @brief This notify function is called by the VBI Filter and allow AP 
 *         to judge of a received teletext header title contains a suitable 
 *         key word that can be adopted as a candidate of channel name.
 *
 *  @param [in]  pv_tag           - Contains the tag prepared by AP.
 *  @param [in]  ui1_mag_num      - Contains the magazine number.
 *  @param [in]  ui1_page_num     - Contains the page number.
 *  @param [in]  ui4_sub_code     - Contains the subcode.
 *  @param [in]  aui1_data_32     - Contains the 32 byte length title info.
 *  @param [out] pb_valid         - Returns the judgement result.
 *  @param [out] s_ch_name        - Returns the retrieved channel name. Valid only of *pb_valid=TRUE.
 *  @param [out] pui1_len_ch_name - Returns the length of the retrieved channel name.
 
 *  @note  This callback is called in VBI Filter's thread context and thus should return as soon as possible.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_vbif_ttx_hdr_ch_name_parse_fct) (
    VOID*               pv_tag,             /* in */
    UINT8               ui1_mag_num,        /* in */
    UINT8               ui1_page_num,       /* in */
    UINT32              ui4_sub_code,       /* in */
    const UINT8*        aui1_data_32,       /* in */
    BOOL*               pb_valid,           /* out */
    CHAR                s_ch_name[33],      /* out */
    UINT8*              pui1_len_ch_name);  /* out */


/* FLTR_INIT_NI_T.t_ttx_hdr_ch_name */
/*----------------------------------------------------------------------------*/
/*! @brief  Contains the initialization info of the source type mask FLTR_NI_SRC_TYPE_TTX_HDR_CH_NAME is setup.
 *  @code
 *  typedef struct _FLTR_NI_INIT_TTX_HDR_CH_NAME_T
 *  {
 *      x_vbif_ttx_hdr_ch_name_parse_fct    pf_parse_ch_name;
 *      const VOID*                         pv_tag_parse_ch_name;
 *  } FLTR_NI_INIT_TTX_HDR_CH_NAME_T;
 *  @endcode
 *  @li@c pf_parse_ch_name     - Specifies the teletext header parse function for channel name.
 *  @li@c pv_tag_parse_ch_name - Specifies caller prepared tag.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_INIT_TTX_HDR_CH_NAME_T
{
    x_vbif_ttx_hdr_ch_name_parse_fct    pf_parse_ch_name;
    const VOID*                         pv_tag_parse_ch_name;
    
} FLTR_NI_INIT_TTX_HDR_CH_NAME_T;


/* FLTR_INIT_NI_T.t_aci */
/*----------------------------------------------------------------------------*/
/*! @brief  Contains the initialization info of the ACI.
 *  @code
 *  typedef struct _FLTR_NI_INIT_ACI_T
 *  {
 *      FLTR_NI_ACI_ORIGR_TYPE_MASK_T           e_originator_mask;
 *      FLTR_NI_ACI_NETWORK_TYPE_MASK_T      e_network_mask;
 *      ISO_639_LANG_T                                      t_lang;
 *  } FLTR_NI_INIT_ACI_T;
 *  @endcode
 *  @li@c e_originator_mask  - Specifies the originator mask.
 *  @li@c e_network_mask    - Specifies the network mask.
 *  @li@c t_lang                    - Specifies the language information.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_INIT_ACI_T
{
    /* filter condition: originator, could be FLTR_NI_ACI_ORIGR_TYPE_IGNORE */
    FLTR_NI_ACI_ORIGR_TYPE_MASK_T       e_originator_mask;
    
    /* filter condition: network, could be FLTR_NI_ACI_NETWORK_TYPE_IGNORE */
    FLTR_NI_ACI_NETWORK_TYPE_MASK_T     e_network_mask;

    /* filter condition: language */
    ISO_639_LANG_T                      t_lang;
    
} FLTR_NI_INIT_ACI_T;    


/* x_vbif_open ( e_fltr_type == FLTR_TYPE_NI,       */
/*               pv_fltr_info -> (FLTR_INIT_NI_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Contains the initialization info of NI.
 *  @code
 *  typedef struct _FLTR_INIT_NI_T
 *  {
 *      FLTR_NI_SRC_TYPE_T                          e_src_mask;
 *      FLTR_NI_INIT_TTX_HDR_CH_NAME_T    t_ttx_hdr_ch_name;
 *      FLTR_NI_INIT_ACI_T                            t_aci;
 *  } FLTR_INIT_NI_T;
 *  @endcode
 *  @li@c e_src_mask               - Specifies the source mask.
 *  @li@c t_ttx_hdr_ch_name    - Specifies the channel name.
 *  @li@c t_aci                         - Specifies the aci information.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_INIT_NI_T
{
    FLTR_NI_SRC_TYPE_T      e_src_mask;
    
    /* FLTR_NI_SRC_TYPE_NI_TTX_8_30_FMT_1 */
    /* nothing */
    
    /* FLTR_NI_SRC_TYPE_CNI_VPS */
    /* nothing */

    /* FLTR_NI_SRC_TYPE_CNI_TTX_8_30_FMT_2 */
    /* nothing */
    
    /* FLTR_NI_SRC_TYPE_TTX_HDR_CH_NAME */
    /* (valid if (e_src_mask & FLTR_NI_SRC_TYPE_TTX_HDR_CH_NAME) ) */
    FLTR_NI_INIT_TTX_HDR_CH_NAME_T      t_ttx_hdr_ch_name;
    
    /* FLTR_NI_SRC_TYPE_VPS_DATA */
    /* nothing */
    
    /* FLTR_NI_SRC_TYPE_8_30_FMT_1_PRG_TTL */
    /* nothing */
    
    /* FLTR_NI_SRC_TYPE_TTX_DATE_TIME */
    /* nothing */
    
    /* FLTR_NI_SRC_TYPE_ACI */
    /* (valid if (e_src_mask & FLTR_NI_SRC_TYPE_ACI) */
    FLTR_NI_INIT_ACI_T                  t_aci;
     
} FLTR_INIT_NI_T;


/* FLTR_NFY_NI_T.u.t_ttx_hdr_ch_name */
/*----------------------------------------------------------------------------*/
/*! @brief  Contains the channel name info of teletext handler.
 *  @code
 *  typedef struct _FLTR_NI_NFY_TTX_HDR_CH_NAME_T
 *  {
 *      CHAR*           ps_ch_name;
 *      UINT8           ui1_len_ch_name;  
 *  } FLTR_NI_NFY_TTX_HDR_CH_NAME_T;
 *  @endcode
 *  @li@c ps_ch_name           - Specifies the string of channel name.
 *  @li@c ui1_len_ch_name    - Specifies the length of channel name.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_NFY_TTX_HDR_CH_NAME_T
{
    CHAR*           ps_ch_name;             /* OUT */
    UINT8           ui1_len_ch_name;        /* OUT */    
} FLTR_NI_NFY_TTX_HDR_CH_NAME_T;


/* FLTR_NFY_NI_T.u.t_vps_data */
#define NI_VPS_DATA_LEN    13       
/**<The definition of VPS data length*/

typedef UINT8 NI_VPS_DATA_T [NI_VPS_DATA_LEN];
/**<The type definition of VPS data*/


/*----------------------------------------------------------------------------*/
/*! @brief  Contains the VPS data of NI notification.
 *  @code
 *  typedef struct _FLTR_NI_NFY_VPS_DATA_T
 *  {
 *      NI_VPS_DATA_T           t_vps_data;
 *  } FLTR_NI_NFY_VPS_DATA_T;
 *  @endcode
 *  @li@c t_vps_data           - Specifies the vps data.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_NFY_VPS_DATA_T
{
    NI_VPS_DATA_T           t_vps_data;     /* OUT, VPS byte 3~15 */
    
} FLTR_NI_NFY_VPS_DATA_T;

/* FLTR_NFY_NI_T.u.t_prg_ttl_8_30_f1 */
#define FLTR_NI_PRG_TTL_LEN     20  
/**<The definition of program title length*/

/*----------------------------------------------------------------------------*/
/*! @brief  Contains the program title of NI notification.
 *  @code
 *  typedef struct _FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T
 *  {
 *      CHAR                    s_prg_ttl[FLTR_NI_PRG_TTL_LEN + 1];
 *  } FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T;
 *  @endcode
 *  @li@c s_prg_ttl           - Specifies the program title.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T 
{
    CHAR                    s_prg_ttl[FLTR_NI_PRG_TTL_LEN + 1];  /* OUT */
    
} FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T;

/* FLTR_NFY_NI_T.u.t_ttx_date_time */
#define NI_TTX_DATE_TIME_LEN    7   
/**<The definition of teletext date time length*/

typedef UINT8 NI_TTX_DATE_TIME_T [NI_TTX_DATE_TIME_LEN];
/**<The definition of teletext date time*/


/*----------------------------------------------------------------------------*/
/*! @brief  Contains the teletext date time of NI notification.
 *  @code
 *  typedef struct _FLTR_NI_NFY_TTX_DATE_TIME_T
 *  {
 *      NI_TTX_DATE_TIME_T      aui1_data;
 *  } FLTR_NI_NFY_TTX_DATE_TIME_T;
 *  @endcode
 *  @li@c aui1_data           - Specifies the teletext date time.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_NFY_TTX_DATE_TIME_T
{
    NI_TTX_DATE_TIME_T      aui1_data;         /* OUT, Teletext packet 8/30 format 1, byte 15~21 */
    
} FLTR_NI_NFY_TTX_DATE_TIME_T;    


/* FLTR_NI_NFY_ACI_T.u.t_detected */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information required by client when ACI is detected.
 *  @code
 *  typedef struct _FLTR_NI_NFY_ACI_DETECTED_T 
 *  {
 *      FLTR_NI_ACI_ORIGR_TYPE_T    e_originator;
 *  } FLTR_NI_NFY_ACI_DETECTED_T;
 *  @endcode
 *  @li@c e_originator - Contains the ACI originator.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_NFY_ACI_DETECTED_T 
{
    FLTR_NI_ACI_ORIGR_TYPE_T    e_originator;
    
} FLTR_NI_NFY_ACI_DETECTED_T;

/* FLTR_NI_NFY_ACI_T.u.t_slctn_page_update */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information required by client when selection page is changed.
 *  @code
 *  typedef struct _FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T 
 *  {
 *      UINT16                      ui2_page_slctn;
 *  } FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T;    
 *  @endcode
 *  @li@c ui2_page_slctn - Contains the new page number of selection page.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T 
{
    
    UINT16                      ui2_page_slctn;
    
} FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T;    

/* FLTR_NI_NFY_ACI_T.u.t_db_ready */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information required by client when ACI DB is ready.
 *  @code
 *  typedef struct _FLTR_NI_NFY_ACI_DB_READY_T 
 *  {
 *      UINT32                      ui4_db_version;
 *  } FLTR_NI_NFY_ACI_DB_READY_T;    
 *  @endcode
 *  @li@c ui4_db_version - Contains the ACI DB version.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_NFY_ACI_DB_READY_T 
{
    UINT32                      ui4_db_version;
    
} FLTR_NI_NFY_ACI_DB_READY_T;    

/* FLTR_NFY_NI_T.u.t_aci */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the structure of ACI notification.
 *  @code
 *  typedef struct _FLTR_NI_NFY_ACI_T 
 *  {
 *      FLTR_NI_NFY_CODE_ACI_T      e_nfy_code;
 *      union
 *      {
 *          FLTR_NI_NFY_ACI_DETECTED_T                  t_detected;
 *          FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T  t_slctn_page_update;
 *          FLTR_NI_NFY_ACI_DB_READY_T                  t_db_ready;
 *      } u;
 *  } FLTR_NI_NFY_ACI_T;    
 *  @endcode
 *  @li@c e_nfy_code                - Contains the code fo ACI notification.
 *  @li@c t_detected                 - Contains the notification info of ACI detected.
 *  @li@c t_slctn_page_update   - Contains the notification info of ACI selecten page update.
 *  @li@c t_db_ready                - Contains the notification info of ACI database reade.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_NFY_ACI_T
{
    FLTR_NI_NFY_CODE_ACI_T      e_nfy_code;
    
    union
    {
        /* FLTR_NI_ACI_NFY_CODE_DETECTED */
        FLTR_NI_NFY_ACI_DETECTED_T           t_detected;
        
        /* FLTR_NI_ACI_NFY_CODE_SLCTN_PAGE_NUM */
        FLTR_NI_NFY_ACI_SLCTN_PAGE_UPDATE_T  t_slctn_page_update;
        
        /* FLTR_NI_ACI_NFY_CODE_DB_UPDATED */
        FLTR_NI_NFY_ACI_DB_READY_T           t_db_ready;
        
    } u;
    
} FLTR_NI_NFY_ACI_T;    


/* (*x_vbif_nfy_fct) : (FLTR_NFY_NI_T *) ui4_data */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the NI notification.
 *  @code
 *  typedef struct _FLTR_NFY_NI_T 
 *  {
 *      FLTR_NI_SRC_TYPE_T      e_src_type;
 *      union
 *      {
 *          UINT16                                                  ui2_id;
 *          FLTR_NI_NFY_TTX_HDR_CH_NAME_T       t_ttx_hdr_ch_name;  
 *          FLTR_NI_NFY_VPS_DATA_T                      t_vps_data;
 *          FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T     t_prg_ttl_8_30_f1;
 *          FLTR_NI_NFY_TTX_DATE_TIME_T             t_ttx_date_time;
 *          FLTR_NI_NFY_ACI_T                               t_aci;
 *      } u;  
 *  } FLTR_NFY_NI_T;    
 *  @endcode
 *  @li@c e_src_type                - Contains the source type information.
 *  @li@c ui2_id                        - Contains the ID information.
 *  @li@c t_ttx_hdr_ch_name    - Contains the teletext channel name.
 *  @li@c t_vps_data                - Contains the VPS data.
 *  @li@c t_prg_ttl_8_30_f1       - Contains the program title.
 *  @li@c t_ttx_date_time          - Contains the teletext date time.
 *  @li@c t_aci                         - Contains the ACI information.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_NI_T
{
    FLTR_NI_SRC_TYPE_T      e_src_type;         /* OUT */
    
    union
    {
        /* FLTR_NI_SRC_TYPE_NI_TTX_8_30_FMT_1 */
        /* FLTR_NI_SRC_TYPE_CNI_VPS */
        /* FLTR_NI_SRC_TYPE_CNI_TTX_8_30_FMT_2 */
        UINT16                              ui2_id;
        
        /* FLTR_NI_SRC_TYPE_TTX_HDR_CH_NAME */
        FLTR_NI_NFY_TTX_HDR_CH_NAME_T       t_ttx_hdr_ch_name;  
        
        /* FLTR_NI_SRC_TYPE_VPS_DATA */
        FLTR_NI_NFY_VPS_DATA_T              t_vps_data;
        
        /* FLTR_NI_SRC_TYPE_8_30_FMT_1_PRG_TTL */
        FLTR_NI_NFY_8_30_FMT_1_PRG_TTL_T    t_prg_ttl_8_30_f1;
        
        /* FLTR_NI_SRC_TYPE_TTX_DATE_TIME */
        FLTR_NI_NFY_TTX_DATE_TIME_T         t_ttx_date_time;
        
        /* FLTR_NI_SRC_TYPE_ACI */
        FLTR_NI_NFY_ACI_T                   t_aci;
        
    } u;                                        /* OUT */
    
} FLTR_NFY_NI_T;



/* EU Event Filter (FLTR_TYPE_EU_EVN) ******************************************/


/* x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_DB_INFO,  */
/*             pv_get_info : (FLTR_NI_ACI_DB_INFO_T *)) */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the NI ACI database information.
 *  @code
 *  typedef struct _FLTR_NI_ACI_DB_INFO_T 
 *  {
 *      UINT32                                          ui4_db_version;
 *      FLTR_NI_ACI_ORIGR_TYPE_T          e_originator; 
 *      FLTR_NI_ACI_NETWORK_TYPE_T     e_network;
 *      ISO_639_LANG_T                           t_lang; 
 *      FLTR_NI_ACI_PKG_MODE_T            e_pkt_mode; 
 *  } FLTR_NI_ACI_DB_INFO_T;    
 *  @endcode
 *  @li@c ui4_db_version      - Contains the version of database.
 *  @li@c e_originator           - Contains the originator type.
 *  @li@c e_network             - Contains the network type.
 *  @li@c t_lang                   - Contains the language.
 *  @li@c e_pkt_mode          - Contains the ACI packet mode.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_ACI_DB_INFO_T
{
    UINT32                          ui4_db_version;     /* IN */
    
    FLTR_NI_ACI_ORIGR_TYPE_T        e_originator;       /* OUT */
    
    FLTR_NI_ACI_NETWORK_TYPE_T      e_network;          /* OUT */
    
    ISO_639_LANG_T                  t_lang;             /* OUT */
    
    FLTR_NI_ACI_PKG_MODE_T          e_pkt_mode;         /* OUT */
    
} FLTR_NI_ACI_DB_INFO_T;    


/* x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_CITY_LIST_INFO,  */
/*             pv_get_info : (FLTR_NI_ACI_CITY_LIST_INFO_T *)) */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the city list page information. 
 *              x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_CITY_LIST_INFO,
 *                          pv_get_info : (FLTR_NI_ACI_CITY_LIST_INFO_T *))
 *  @code
 *  typedef struct _FLTR_NI_ACI_CITY_LIST_INFO_T
 *  {
 *      UINT32                      ui4_db_version;       
 *      UINT16                      ui2_page_clst;        
 *      UINT8                       ui1_num_city;         
 *      UINT8                       ui1_idx_default;      
 *  } FLTR_NI_ACI_CITY_LIST_INFO_T;
 *  @endcode
 *  @li@c ui4_db_version  - Contains the ACI DB version.
 *  @li@c ui2_page_clst   - Contains the page number of the city list page.
 *  @li@c ui1_num_city    - Contains the number of items in the city list.
 *  @li@c ui1_idx_default - Contains the index of the default city.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_ACI_CITY_LIST_INFO_T
{
    UINT32                      ui4_db_version;         /* IN */
    
    UINT16                      ui2_page_clst;          /* IN */
    
    UINT8                       ui1_num_city;           /* OUT */
    
    UINT8                       ui1_idx_default;        /* OUT */
    
} FLTR_NI_ACI_CITY_LIST_INFO_T;

/* City List Entry Type */
/*------------------------------------------------------------------*/
/*! @brief     Definition of the entry type.
  *  @code 
  *  typedef enum _FLTR_NI_ACI_CLST_ENTRY_TYPE_T
  *  { 
  *      ACI_CLST_ENTRY_TYPE_CH_MAP_LINK, 
  *      ACI_CLST_ENTRY_TYPE_CLIST_LINK,
  *      ACI_CLST_ENTRY_TYPE_UNKNOWN,
  *      ACI_CLST_ENTRY_TYPE_INVALID_CH_MAP_LINK
  *  } FLTR_NI_ACI_CLST_ENTRY_TYPE_T; 
  *  @endcode 
  *  @li@c  ACI_CLST_ENTRY_TYPE_CH_MAP_LINK                -linkage to a channel map.
  *  @li@c  ACI_CLST_ENTRY_TYPE_CLIST_LINK                   -linkage to the next level city list.
  *  @li@c  ACI_CLST_ENTRY_TYPE_UNKNOWN                     -unknown type due to incomplete ACI data.
  *  @li@c  ACI_CLST_ENTRY_TYPE_INVALID_CH_MAP_LINK  -linkage to an invalid channel map due to incomplete ACI data.
  */
/*------------------------------------------------------------------*/
typedef enum _FLTR_NI_ACI_CLST_ENTRY_TYPE_T
{
    ACI_CLST_ENTRY_TYPE_CH_MAP_LINK,                    /* linkage to a channel map */
    ACI_CLST_ENTRY_TYPE_CLIST_LINK,                     /* linkage to the next level city list */
    ACI_CLST_ENTRY_TYPE_UNKNOWN,                        /* unknown type due to incomplete ACI data */
    ACI_CLST_ENTRY_TYPE_INVALID_CH_MAP_LINK             /* linkage to an invalid channel map due to incomplete ACI data */
    
} FLTR_NI_ACI_CLST_ENTRY_TYPE_T;    


/* x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_CITY_INFO,  */
/*             pv_get_info : (FLTR_NI_ACI_CITY_INFO_T *)) */
#define FLTR_NI_ACI_CITY_DESC_TEXT_LEN      18          
/**<Defines the ACI city descriptor text length*/


/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information of a city item.
 *              x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_CITY_INFO,
 *                          pv_get_info : (FLTR_NI_ACI_CITY_INFO_T *)) 
 *  @code
 *  typedef struct _FLTR_NI_ACI_CITY_INFO_T
 *  {
 *      UINT32                      ui4_db_version; 
 *      UINT16                      ui2_page_clst;  
 *      UINT8                       ui1_idx_city;   
 *      UINT8                       ui1_ttx_row;    
 *      UINT8                       ui1_desc_start; 
 *      UINT8                       ui1_desc_end;   
 *      UINT8                       ui1_digit_start;
 *      UINT8                       ui1_digit_end;    
 *      CHAR                        s_desc_text[FLTR_NI_ACI_CITY_DESC_TEXT_LEN + 1]; 
 *      FLTR_NI_ACI_CLST_ENTRY_TYPE_T  e_type;              
 *      UINT16                      ui2_page_sub_clst;      
 *  } FLTR_NI_ACI_CITY_INFO_T;    
 *  @endcode
 *  @li@c ui4_db_version    - (IN)  Specifies the ACI DB version.
 *  @li@c ui2_page_clst     - (IN)  Specifies the page number of the desired city list.
 *  @li@c ui1_idx_city      - (IN)  Specifies the index id of the desired city item.
 *  @li@c ui1_ttx_row       - (IN)  Contains the teletext row id the city item.
 *  @li@c ui1_desc_start    - (OUT) Contains the start column of city item description text.
 *  @li@c ui1_desc_end      - (OUT) Contains the end column of city item description text.
 *  @li@c ui1_digit_start   - (OUT) Contains the start column of city item index id.
 *  @li@c ui1_digit_end     - (OUT) Contains the end column of city item index id.
 *  @li@c s_desc_text       - (OUT) Contains the description text of the city item.
 *  @li@c e_type            - (OUT) Contains the entry type of the city item.
 *  @li@c ui2_page_sub_clst - (OUT) Contains the page number of the linked sub city list page. Valid if e_type=ACI_CLST_ENTRY_TYPE_CLIST_LINK.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_ACI_CITY_INFO_T
{
    UINT32                      ui4_db_version;         /* IN */
    
    UINT16                      ui2_page_clst;          /* IN */
    
    UINT8                       ui1_idx_city;           /* IN */

    UINT8                       ui1_ttx_row;            /* OUT */
    
    UINT8                       ui1_desc_start;         /* OUT */
    
    UINT8                       ui1_desc_end;           /* OUT */
    
    UINT8                       ui1_digit_start;        /* OUT */
        
    UINT8                       ui1_digit_end;          /* OUT */   
    
    CHAR                        s_desc_text[FLTR_NI_ACI_CITY_DESC_TEXT_LEN + 1]; /* IN/OUT */
    
    FLTR_NI_ACI_CLST_ENTRY_TYPE_T  e_type;              /* OUT */
                                
    UINT16                      ui2_page_sub_clst;      /* OUT, only available when e_type == ACI_CLST_ENTRY_TYPE_CLIST_LINK */    

} FLTR_NI_ACI_CITY_INFO_T;    


/* x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_CH_NUM, */
/*             pv_get_info : (FLTR_NI_ACI_CH_NUM_T *)   ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information of ACI channel num.
 *              x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_CH_NUM, 
 *                          pv_get_info : (FLTR_NI_ACI_CH_NUM_T *)   )
 *  @code
 *  typedef struct _FLTR_NI_ACI_CH_NUM_T
 *  {
 *      UINT32                      ui4_db_version; 
 *      UINT16                      ui2_page_clst;  
 *      UINT8                        ui1_idx_city;   
 *      UINT16                      ui2_num_ch;       
 *  } FLTR_NI_ACI_CH_NUM_T;    
 *  @endcode
 *  @li@c ui4_db_version    - (IN)  Specifies the ACI DB version.
 *  @li@c ui2_page_clst      - (IN)  Specifies the page number of the desired city list.
 *  @li@c ui1_idx_city         - (IN)  Specifies the index id of the desired city item.
 *  @li@c ui2_num_ch         - (OUT)  Contains the channel number.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_ACI_CH_NUM_T
{
    UINT32                              ui4_db_version;         /* IN */

    UINT16                              ui2_page_clst;          /* IN */
    
    UINT8                               ui1_idx_city;           /* IN */
    
    UINT16                              ui2_num_ch;             /* OUT */

} FLTR_NI_ACI_CH_NUM_T;


/* x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_CH_INFO, */
/*             pv_get_info : (FLTR_NI_ACI_CH_INFO_T *)   ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information of ACI channel information.
 *              x_vbif_get (e_get_type == VBIF_GET_TYPE_NI_ACI_CH_INFO, 
 *                          pv_get_info : (FLTR_NI_ACI_CH_INFO_T *)   ) 
 *  @code
 *  typedef struct _FLTR_NI_ACI_CH_INFO_T
 *  {
 *      UINT32                              ui4_db_version; 
 *      UINT16                              ui2_page_clst;  
 *      UINT8                               ui1_idx_city;   
 *      UINT16                              ui2_idx_ch; 
 *      UINT16                              ui2_num_ch;       
 *      UINT32                              ui4_freq;
 *      CHAR                                ac_ch_name[MAX_PROG_NAME_LEN+1]; 
 *      FLTR_NI_ACI_SVC_TYPE_T e_svc_type;     
 *      UINT16                              ui2_page_prog_guide;
 *  } FLTR_NI_ACI_CH_INFO_T;    
 *  @endcode
 *  @li@c ui4_db_version          - (IN)  Specifies the ACI DB version.
 *  @li@c ui2_page_clst            - (IN)  Specifies the page number of the desired city list.
 *  @li@c ui1_idx_city               - (IN)  Specifies the index id of the desired city item.
 *  @li@c ui1_idx_ch                 - (IN)  Specifies the index id of channel. 
 *  @li@c ui2_ch_num               - (OUT)  Contains the channel number.
 *  @li@c ui4_freq                    - (OUT)  Specifies the frequency.
 *  @li@c ac_ch_name              - (OUT)  Specifies the channel name.
 *  @li@c e_svc_type                - (OUT)  Specifies the service type.
 *  @li@c ui2_page_prog_guide  - (OUT)  Contains page of program guide. 
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NI_ACI_CH_INFO_T
{
    UINT32                              ui4_db_version;         /* IN */

    UINT16                              ui2_page_clst;          /* IN */
    
    UINT8                               ui1_idx_city;           /* IN */
    
    UINT16                              ui2_idx_ch;             /* IN */
    
    UINT16                              ui2_ch_num;             /* OUT */
    
    UINT32                              ui4_freq;               /* OUT */
    
    CHAR                                ac_ch_name[MAX_PROG_NAME_LEN+1];  /* OUT */

    FLTR_NI_ACI_SVC_TYPE_T              e_svc_type;             /* OUT */
    
    UINT16                              ui2_page_prog_guide;    /* OUT */
    
} FLTR_NI_ACI_CH_INFO_T;    












/* EU Event Filter (FLTR_TYPE_EU_EVN) ******************************************/
/* data types for EU Event */
/*------------------------------------------------------------------*/
/*! @brief     Definition of data ype of EU event.
  *  @code 
  *  typedef enum _FLTR_EU_EVN_DATA_TYPE_T
  *  { 
  *      FLTR_EU_EVN_DATA_TYPE_PRG_TTL = 0
  *  } FLTR_EU_EVN_DATA_TYPE_T; 
  *  @endcode 
  *  @li@c  FLTR_EU_EVN_DATA_TYPE_PRG_TTL        -program title.
  */
/*------------------------------------------------------------------*/
typedef enum _FLTR_EU_EVN_DATA_TYPE_T
{
    FLTR_EU_EVN_DATA_TYPE_PRG_TTL = 0
    
} FLTR_EU_EVN_DATA_TYPE_T;

/* x_vbif_open ( e_fltr_type == FLTR_TYPE_EU_EVN,          */
/*               pv_fltr_info => (FLTR_INIT_EU_EVN_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information of ACI channel information.
 *              x_vbif_open ( e_fltr_type == FLTR_TYPE_EU_EVN, 
 *                          pv_fltr_info => (FLTR_INIT_EU_EVN_T *) )
 *  @code
 *  typedef struct _FLTR_INIT_EU_EVN_T
 *  {
 *      UINT32              ui4_reserved;
 *  } FLTR_INIT_EU_EVN_T;    
 *  @endcode
 *  @li@c ui4_reserved          - Unused currently.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_INIT_EU_EVN_T
{
    /* init info for FLTR_EU_EVN_DATA_TYPE_PRG_TTL */
    UINT32              ui4_reserved;
    
} FLTR_INIT_EU_EVN_T;

/* format of data with type FLTR_EU_EVN_DATA_TYPE_PRG_TTL */
#define VBIF_EU_EVN_PRG_TTL_LEN     20
/**<Defines the lenght of program title*/


/*----------------------------------------------------------------------------*/
/*! @brief  Defines the structure of program title.
 *  @code
 *  typedef struct _FLTR_EU_EVN_DATA_PRG_TTL_T
 *  {
 *      CHAR                            s_prg_ttl[VBIF_EU_EVN_PRG_TTL_LEN + 1];
 *  } FLTR_EU_EVN_DATA_PRG_TTL_T;    
 *  @endcode
 *  @li@c s_prg_ttl          - The program title string.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_EU_EVN_DATA_PRG_TTL_T
{
    CHAR                            s_prg_ttl[VBIF_EU_EVN_PRG_TTL_LEN + 1];  /* OUT */
    
} FLTR_EU_EVN_DATA_PRG_TTL_T;

/* (*x_vbif_nfy_fct) ( e_cond   == VBIF_COND_STARTED,       */
/*                     e_reason == NFY_REASON_DATA_AVAIL,   */
/*                     ui4_data => (FLTR_NFY_EU_EVN_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information of EU event notification.
 *               (*x_vbif_nfy_fct) ( e_cond   == VBIF_COND_STARTED,
 *                          e_reason == NFY_REASON_DATA_AVAIL, 
 *                          ui4_data => (FLTR_NFY_EU_EVN_T *) )
 *  @code
 *  typedef struct _FLTR_NFY_EU_EVN_T
 *  {
 *      FLTR_EU_EVN_DATA_TYPE_T         e_data_type;
 *      union
 *      {
 *          FLTR_EU_EVN_DATA_PRG_TTL_T     t_prg_ttl;
 *      } u_data;
 *  } FLTR_NFY_EU_EVN_T;    
 *  @endcode
 *  @li@c e_data_type    - Specified the data type.
 *  @li@c t_prg_ttl          - Contains the program title.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_EU_EVN_T
{
    FLTR_EU_EVN_DATA_TYPE_T         e_data_type;
    
    union
    {
        FLTR_EU_EVN_DATA_PRG_TTL_T     t_prg_ttl;
        
    } u_data;
    
} FLTR_NFY_EU_EVN_T;










/* EPG Filter (FLTR_TYPE_EPG) *******************************************************/
/* specific get types */
#define VBIF_GET_TYPE_EPG_DATA          (VBIF_GET_TYPE_LAST_ENTRY + ((VBIF_GET_TYPE_T) 0))
/**<The get type of geting EPG data*/


/* x_vbif_open ( e_fltr_type == FLTR_TYPE_EPG,         */
/*               pv_fltr_info => (FLTR_INIT_EPG_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the init info of EPG filter.
 *              x_vbif_open ( e_fltr_type == FLTR_TYPE_EPG,  
 *                          pv_fltr_info => (FLTR_INIT_EPG_T *) ) 
 *  @code
 *  typedef struct _FLTR_INIT_EPG_T
 *  {
 *      UINT32                          ui4_reserved;
 *  } FLTR_INIT_EPG_T;    
 *  @endcode
 *  @li@c ui4_reserved    - Unused currently.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_INIT_EPG_T
{
    UINT32                          ui4_reserved;
    
} FLTR_INIT_EPG_T;


/* x_vbif_get ( e_get_type == VBIF_GET_TYPE_EPG_DATA,  */
/*              pv_get_info => (FLTR_NFY_EPG_T *) )    */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the information of EPG notification.
 *              x_vbif_get ( e_get_type == VBIF_GET_TYPE_EPG_DATA,
 *                          pv_get_info => (FLTR_NFY_EPG_T *) ) 
 *  @code
 *  typedef struct _FLTR_NFY_EPG_T
 *  {
 *      VOID*                           pv_tag_epg;
 *      SIZE_T                          z_offset;  
 *      SIZE_T                          z_sz_buf;
 *      SIZE_T                          z_len_total;
 *      SIZE_T                          z_len_data; 
 *      UINT8*                          paui1_data; 
 *  } FLTR_NFY_EPG_T;    
 *  @endcode
 *  @li@c pv_tag_epg     -[IN]The epg tag.
 *  @li@c z_offset          -[IN]The offset.
 *  @li@c z_sz_buf         -[IN]The size of buffer.
 *  @li@c z_len_total      -[OUT]The total length.
 *  @li@c z_len_data      -[OUT]The data length.
 *  @li@c paui1_data      -[IN/OUT]The data.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_EPG_T
{
    /* parameter part */
    /* EPG data tag  (*x_vbif_nfy_fct) : ui4_data -> pv_tag_epg */
    VOID*                           pv_tag_epg;     /* IN */
    
    SIZE_T                          z_offset;       /* IN */
    
    SIZE_T                          z_sz_buf;       /* IN */
    
    /* payload part */
    SIZE_T                          z_len_total;    /* OUT */
    
    SIZE_T                          z_len_data;     /* OUT */
    
    UINT8*                          paui1_data;     /* IN/OUT */
    
} FLTR_NFY_EPG_T;    











/* EU WSS Filter (FLTR_TYPE_EU_WSS) *****************************************************/
/* x_vbif_open ( e_fltr_type == FLTR_TYPE_EU_WSS,       */
/*               pv_fltr_info => (FLTR_INIT_EU_WSS_T *) )  */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the init EU WSS information.
 *              x_vbif_open ( e_fltr_type == FLTR_TYPE_EU_WSS, 
 *                          pv_fltr_info => (FLTR_INIT_EU_WSS_T *) )
 *  @code
 *  typedef struct _FLTR_INIT_EU_WSS_T
 *  {
 *      UINT32                          ui4_reserved;
 *  } FLTR_INIT_EU_WSS_T;    
 *  @endcode
 *  @li@c ui4_reserved     -Unused currently.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_INIT_EU_WSS_T
{
    UINT32                          ui4_reserved;
    
} FLTR_INIT_EU_WSS_T;

/* (*x_vbif_nfy_fct) ( e_cond   == VBIF_COND_STARTED,       */
/*                     e_reason == NFY_REASON_DATA_AVAIL,   */
/*                     ui4_data => (FLTR_NFY_EU_WSS_T *) )     */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the EU WSS notification.
 *               (*x_vbif_nfy_fct) ( e_cond   == VBIF_COND_STARTED,
 *                          e_reason == NFY_REASON_DATA_AVAIL,
 *                          ui4_data => (FLTR_NFY_EU_WSS_T *) )
 *  @code
 *  typedef struct _FLTR_NFY_EU_WSS_T
 *  {
 *      UINT16                          ui2_eu_wss; 
 *  } FLTR_NFY_EU_WSS_T;    
 *  @endcode
 *  @li@c ui2_eu_wss     -[OUT]The eu wss.
 */
/*----------------------------------------------------------------------------*/
typedef struct _FLTR_NFY_EU_WSS_T
{
    UINT16                          ui2_eu_wss;        /* OUT */
    
} FLTR_NFY_EU_WSS_T;













/* Digital EU VBI to Analog EU VBI Pumper (FLTR_TYPE_EU_VBI_D2A) **************************************/
/* x_vbif_open (e_fltr_type == FLTR_TYPE_EU_VBI_D2A,        */
/*              pv_fltr_info : (FLTR_INIT_EU_VBI_D2A_T *) ) */
/*----------------------------------------------------------------------------*/
/*! @brief  Defines the Digital EU VBI to Analog EU VBI Pumper.
 *               x_vbif_open (e_fltr_type == FLTR_TYPE_EU_VBI_D2A,
 *                          pv_fltr_info : (FLTR_INIT_EU_VBI_D2A_T *) ) 
 *  @code
 *  typedef struct _FLTR_INIT_EU_VBI_D2A_T
 *  {
 *      HANDLE_T        h_connection_enc;
 *  } FLTR_INIT_EU_VBI_D2A_T;    
 *  @endcode
 *  @li@c h_connection_enc     -The handle of the connection that encodes the analog signal, could be the same connection as VBIF_SRC_DIG_VBI_ES_T.h_connection in some cases.
 */
/*----------------------------------------------------------------------------*/

typedef struct _FLTR_INIT_EU_VBI_D2A_T  
{
    /* the handle of the connection that encodes the analog signal */
    /* could be the same connection as VBIF_SRC_DIG_VBI_ES_T.h_connection in some cases */
    HANDLE_T        h_connection_enc;
    
} FLTR_INIT_EU_VBI_D2A_T;


#ifdef __cplusplus
}
#endif


#endif

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_VBI_FLTR*/
/*----------------------------------------------------------------------------*/






