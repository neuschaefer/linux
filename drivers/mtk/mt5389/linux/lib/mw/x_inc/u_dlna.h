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
/*! @file u_dlna.h 
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2012/04/27 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA exported constants, macros,
*         and types.
*/
/*----------------------------------------------------------------------------*/

/*!
* @defgroup    groupMW_INET_DLNA DLNA API
* @ingroup     groupMW_INET
* @brief       Provide an external API about DLNA.
* %INET_DLNA API would be used by user who wants to play media stream in local network,
*                The DLNA core stuffs like UPnP Architecture, UPnP AV, SSDP, SOAP, 
*                GENA and HTTP is hide for application engineer, those protocol 
*                is locate at middleware and application engineer should not modify it.
* 
* @see 
*
* @{
* */

#ifndef U_DLNA_H
#define U_DLNA_H

#include "u_common.h"
#include "u_dbg.h"


/* Error code */
/*----------------------------------------------------------------------------*/
/*! @enum   INET_DLNA_RET_T 
*  @brief  DLNA return type
*  @code
*  enum
*  {
*    DLNAR_SUCCESS                                       = 0,
*    DLNAR_FAIL                                          = -128,
*    DLNAR_RW_FAIL                                       = -129,               
*    DLNAR_BROWSE_FAILED                                 = -130,
*    DLNAR_DEVICE_FAILED                                 = -131,
*    DLNAR_CONNECT_SERVER_FAILED                         = -132,
*    DLNAR_SEARCH_FAILED                                 = -133,
*    DLNAR_NOT_ENOUGH_RESOURCE                           = -134,
*    DLNAR_OBJECT_NOT_EXIST                              = -135,
*    DLNAR_ALREADY_START                                 = -136,
*    DLNAR_NOT_START                                     = -137,
*    DLNAR_INVALID_ARGUMENT                              = -138,
*    DLNAR_NOT_INIT                                      = -139,  
*    DLNAR_DEVICE_SORT_CAPABILITY_FAILED                 = -140,
*    DLNAR_DEVICE_SEARCH_CAPABILITY_FAILED               = -141,
*    DLNAR_NOT_SUPPORT                                   = -142, 
*    DLNAR_CLOSED                                        = -143,
*    DLNAR_ERROR                                         = -144,
*    DLNAR_TIME_OUT                                      = -145,
*    DLNAR_OBJECT_EOF                                    = -146,
*    DLNAR_SERVER_ERROR                                  = -147,
*    DLNAR_CLIENT_ERROR                                  = -148,
*    DLNAR_CANCELLED                                     = -149
*  };
*  @endcode
*   
*  @li@c DLNAR_SUCCESS                                  - Return successful
*  @li@c DLNAR_FAIL                                     - Return fail
*  @li@c DLNAR_RW_FAIL                                  - Return R/W fail
*  @li@c DLNAR_BROWSE_FAILED                            - Return browse fail
*  @li@c DLNAR_DEVICE_FAILED                            - Return device error
*  @li@c DLNAR_CONNECT_SERVER_FAILED                    - Return connect to server fail
*  @li@c DLNAR_SEARCH_FAILED                            - Return search fail
*  @li@c DLNAR_NOT_ENOUGH_RESOURCE                      - Return memory not enough
*  @li@c DLNAR_OBJECT_NOT_EXIST                         - Return object not exist
*  @li@c DLNAR_ALREADY_START                            - Return the client already started
*  @li@c DLNAR_NOT_START                                - Return the client not started
*  @li@c DLNAR_INVALID_ARGUMENT                         - Return invalid argument
*  @li@c DLNAR_NOT_INIT                                 - Return the client not init
*  @li@c DLNAR_DEVICE_SORT_CAPABILITY_FAILED            - Return check the device sort capability fail 
*  @li@c DLNAR_DEVICE_SEARCH_CAPABILITY_FAILED          - Return check the device search capability fail 
*  @li@c DLNAR_NOT_SUPPORT                              - Return system not support
*  @li@c DLNAR_CLOSED                                   - Return the client or server closed 
*  @li@c DLNAR_ERROR                                    - Return the client or server error
*  @li@c DLNAR_TIME_OUT                                 - Return time out 
*  @li@c DLNAR_OBJECT_EOF                               - Return file EOF
*  @li@c DLNAR_SERVER_ERROR                             - Return server error
*  @li@c DLNAR_CLIENT_ERROR                             - Return client error
*  @li@c DLNAR_CANCELLED                                - Return the action cancelled
*              
*/
/*----------------------------------------------------------------------------*/
enum
{
    DLNAR_SUCCESS = 0,
    DLNAR_FAIL                                          = -128,
    DLNAR_RW_FAIL                                       = -129,               
    DLNAR_BROWSE_FAILED                                 = -130,
    DLNAR_DEVICE_FAILED                                 = -131,
    DLNAR_CONNECT_SERVER_FAILED                         = -132,
    DLNAR_SEARCH_FAILED                                 = -133,
    DLNAR_NOT_ENOUGH_RESOURCE                           = -134,
    DLNAR_OBJECT_NOT_EXIST                              = -135,
    DLNAR_ALREADY_START                                 = -136,
    DLNAR_NOT_START                                     = -137,
    DLNAR_INVALID_ARGUMENT                              = -138,
    DLNAR_NOT_INIT                                      = -139,  
    DLNAR_DEVICE_SORT_CAPABILITY_FAILED                 = -140,
    DLNAR_DEVICE_SEARCH_CAPABILITY_FAILED               = -141,
    DLNAR_NOT_SUPPORT                                   = -142, 
    DLNAR_CLOSED                                        = -143,
    DLNAR_ERROR                                         = -144,
    DLNAR_TIME_OUT                                      = -145,
    DLNAR_OBJECT_EOF                                    = -146,
    DLNAR_SERVER_ERROR                                  = -147,
    DLNAR_CLIENT_ERROR                                  = -148,
    DLNAR_CANCELLED                                     = -149
};



/* DLNA device type.  */
/*----------------------------------------------------------------------------*/
/*! @enum   DLNA_DEVICE_TYPE_T 
*  @brief  DLNA device type
*  @code
*  typedef enum {
*    DLNA_DEVICE_TYPE_START = 0, 
*    DLNA_DEVICE_TYPE_UNKNOWN,
*    DLNA_DEVICE_TYPE_DMS,
*    DLNA_DEVICE_TYPE_M_DMS,
*    DLNA_DEVICE_TYPE_DMR,
*    DLNA_DEVICE_TYPE_DMPr,
*    DLNA_DEVICE_TYPE_DMP,
*    DLNA_DEVICE_TYPE_M_DMP,
*    DLNA_DEVICE_TYPE_DMC,
*    DLNA_DEVICE_TYPE_M_DMC,
*    DLNA_DEVICE_TYPE_M_DMU,
*    DLNA_DEVICE_TYPE_M_DMD,
*    DLNA_DEVICE_TYPE_END 
*  } DLNA_DEVICE_TYPE_T;
*  @endcode
*   
*  @li@c DLNA_DEVICE_TYPE_START                 - DLNA device type start
*  @li@c DLNA_DEVICE_TYPE_UNKNOWN               - DLNA device type unknown
*  @li@c DLNA_DEVICE_TYPE_DMS                   - DLNA DMS
*  @li@c DLNA_DEVICE_TYPE_M_DMS                 - DLNA M-DMS
*  @li@c DLNA_DEVICE_TYPE_DMR                   - DLNA DMR
*  @li@c DLNA_DEVICE_TYPE_DMPr                  - DLNA DMPr
*  @li@c DLNA_DEVICE_TYPE_DMP                   - DLNA DMP
*  @li@c DLNA_DEVICE_TYPE_M_DMP                 - DLNA M-DMP
*  @li@c DLNA_DEVICE_TYPE_DMC                   - DLNA DMC
*  @li@c DLNA_DEVICE_TYPE_M_DMC                 - DLNA M-DMC
*  @li@c DLNA_DEVICE_TYPE_M_DMU                 - DLNA M-DMU
*  @li@c DLNA_DEVICE_TYPE_M_DMD                 - DLNA M-DMD
*  @li@c DLNA_DEVICE_TYPE_END                   - DLNA device type end
*              
*/
/*----------------------------------------------------------------------------*/
typedef enum {
    DLNA_DEVICE_TYPE_START = 0, /* Please keep this one the first one. */
    DLNA_DEVICE_TYPE_UNKNOWN,
    DLNA_DEVICE_TYPE_DMS,
    DLNA_DEVICE_TYPE_M_DMS,
    DLNA_DEVICE_TYPE_DMR,
    DLNA_DEVICE_TYPE_DMPr,
    DLNA_DEVICE_TYPE_DMP,
    DLNA_DEVICE_TYPE_M_DMP,
    DLNA_DEVICE_TYPE_DMC,
    DLNA_DEVICE_TYPE_M_DMC,
    DLNA_DEVICE_TYPE_M_DMU,
    DLNA_DEVICE_TYPE_M_DMD,
    DLNA_DEVICE_TYPE_END /* Please keep this one the last one. */
} DLNA_DEVICE_TYPE_T;



/* dlna device event */
/*----------------------------------------------------------------------------*/
/*! @enum   DLNA_DEVICE_EVENT_T 
*  @brief  DLNA device event
*  @code
*  typedef enum {  
*   DLNA_DEVICE_EVENT_START = 0,
*   DLNA_DEVICE_EVENT_FOUND_DEVS,
*   DLNA_DEVICE_EVENT_BYEBYE, 
*   DLNA_DEVICE_EVENT_UNAVAILABLE,
*   DLNA_DEVICE_EVENT_SUID_CHG,
*   DLNA_DEVICE_EVENT_CUIDS_CHG,
*   DLNA_DEVICE_EVENT_TIDS_CHG,
*   DLNA_DEVICE_EVENT_LAST_CHG,
*   DLNA_DEVICE_EVENT_DMR_START = 0x100,
*   DLNA_DEVICE_EVENT_DMR_STOP        
*   } DLNA_DEVICE_EVENT_T;
*  @endcode
*   
*  @li@c DLNA_DEVICE_EVENT_START                        - DLNA device event start
*  @li@c DLNA_DEVICE_EVENT_FOUND_DEVS                   - DLNA found device
*  @li@c DLNA_DEVICE_EVENT_BYEBYE                       - DLNA device bye-bye
*  @li@c DLNA_DEVICE_EVENT_UNAVAILABLE                  - DLNA device unavailable
*  @li@c DLNA_DEVICE_EVENT_SUID_CHG                     - DLNA SUID changed
*  @li@c DLNA_DEVICE_EVENT_CUIDS_CHG                    - DLNA CUIDS changed
*  @li@c DLNA_DEVICE_EVENT_TIDS_CHG                     - DLNA TIDS changed
*  @li@c DLNA_DEVICE_EVENT_LAST_CHG                     - DLNA last changed
*  @li@c DLNA_DEVICE_EVENT_DMR_START                    - DLNA DMR start
*  @li@c DLNA_DEVICE_EVENT_DMR_STOP                     - DLNA DMR stop
*/
/*----------------------------------------------------------------------------*/
typedef enum {
    DLNA_DEVICE_EVENT_START = 0,
    DLNA_DEVICE_EVENT_FOUND_DEVS,
    DLNA_DEVICE_EVENT_BYEBYE, 
    DLNA_DEVICE_EVENT_UNAVAILABLE,
    DLNA_DEVICE_EVENT_SUID_CHG,
    DLNA_DEVICE_EVENT_CUIDS_CHG,
    DLNA_DEVICE_EVENT_TIDS_CHG,
    DLNA_DEVICE_EVENT_LAST_CHG,
    DLNA_DEVICE_EVENT_DMR_START = 0x100,
    DLNA_DEVICE_EVENT_DMR_STOP        
} DLNA_DEVICE_EVENT_T;


/* seek type supported by dms */
/*----------------------------------------------------------------------------*/
/*! @enum   DLNA_SEEK_TYPE_T 
*  @brief  DLNA server seek type
*  @code
*  typedef enum {
*    DLNA_SEEK_NONSUPPORT = 0,               
*    DLNA_SEEK_FULL_BYTE = 1,                
*    DLNA_SEEK_FULL_TIME = 2,                
*    DLNA_SEEK_FULL_BYTE_AND_TIME = 3,       
*    DLNA_SEEK_LIMITED_BYTE = 4,             
*    DLNA_SEEK_LIMITED_TIME = 8,             
*    DLNA_SEEK_LIMITED_BYTE_AND_TIME = 12,   
*    DLNA_SEEK_UNKNOWN,
*  } DLNA_SEEK_TYPE_T;  
*  @endcode
*   
*  @li@c DLNA_SEEK_NONSUPPORT                          - DLNA not support seek
*  @li@c DLNA_SEEK_FULL_BYTE                           - DLNA full byte seek
*  @li@c DLNA_SEEK_FULL_TIME                           - DLNA full time seek
*  @li@c DLNA_SEEK_FULL_BYTE_AND_TIME                  - DLNA full byte and time seek
*  @li@c DLNA_SEEK_LIMITED_BYTE                        - DLNA limited byte seek
*  @li@c DLNA_SEEK_LIMITED_TIME                        - DLNA limited time seek
*  @li@c DLNA_SEEK_LIMITED_BYTE_AND_TIME               - DLNA limited byte and time seek
*  @li@c DLNA_SEEK_UNKNOWN                             - DLNA unknown
*/
/*----------------------------------------------------------------------------*/
typedef enum {
    DLNA_SEEK_NONSUPPORT = 0,            /* nonsupport seek */     
    DLNA_SEEK_FULL_BYTE = 1,                 /* full byte seek supported by dms */
    DLNA_SEEK_FULL_TIME = 2,                 /* full time seek supported by dms */
    DLNA_SEEK_FULL_BYTE_AND_TIME = 3,        /* full byte and time seek supported by dms */
    DLNA_SEEK_LIMITED_BYTE = 4,              /* full byte seek supported by dms */
    DLNA_SEEK_LIMITED_TIME = 8,              /* full time seek supported by dms */
    DLNA_SEEK_LIMITED_BYTE_AND_TIME = 12,      /* full byte and time seek supported by dms */
    DLNA_SEEK_UNKNOWN,
} DLNA_SEEK_TYPE_T;

/* protection type supported by dms */
/*----------------------------------------------------------------------------*/
/*! @enum   DLNA_DRM_TYPE_T 
*  @brief  DLNA drm type
*  @code
*  typedef enum {
*    DLNA_DRM_NONE = 0,          
*    DLNA_DRM_DTCP = 1,          
*    DLNA_DRM_WMDRM = 2,         
*    DLNA_DRM_UNKNOWN,
*  } DLNA_DRM_TYPE_T;
*  @endcode
*   
*  @li@c DLNA_DRM_NONE                - DLNA not DRM
*  @li@c DLNA_DRM_DTCP                - DLNA DRM DTCP
*  @li@c DLNA_DRM_WMDRM               - DLNA DRM WMDRM
*  @li@c DLNA_DRM_UNKNOWN             - DLNA DRM unknown
*/
/*----------------------------------------------------------------------------*/
typedef enum {
    DLNA_DRM_NONE = 0,            /* non content protection */     
    DLNA_DRM_DTCP = 1,            /* protected by dtcp */
    DLNA_DRM_WMDRM = 2,           /* protected by wmdrm  */
    DLNA_DRM_UNKNOWN,
} DLNA_DRM_TYPE_T;


/*General type */
typedef VOID * DLNA_DEVICE_T;



/* DLNA MIME type */
/*----------------------------------------------------------------------------*/
/*! @struct   DLNA_PROTOCOL_INFO_T 
*  @brief  DLNA protocol info
*  @code
*  typedef struct
*  {
*    CHAR *ps_mime;            
*    CHAR *ps_host;            
*    CHAR *ps_port;            
*    CHAR *ps_rate;            
*    CHAR *ps_channels;        
*    CHAR *ps_profile_id;      
*    CHAR *ps_content_format; 
*    CHAR *ps_operations_parameter; 
*    CHAR *ps_play_speeds;
*    CHAR *ps_conversion_indicator;
*    CHAR *ps_flags;
*  } DLNA_PROTOCOL_INFO_T;  
*  @endcode
*   
*  @li@c ps_mime                         - DLNA protocol MIME info 
*  @li@c ps_host                         - DLNA host URI
*  @li@c ps_port                         - DLNA server port
*  @li@c ps_rate                         - DLNA audio rate
*  @li@c ps_channels                     - DLNA audio channels
*  @li@c ps_profile_id                   - DLNA profile ID
*  @li@c ps_content_format               - DLNA content format 
*  @li@c ps_operations_parameter         - DLNA operation parameter
*  @li@c ps_play_speeds                  - DLNA play speeds
*  @li@c ps_conversion_indicator         - DLNA conversion indicator
*  @li@c ps_flags                        - DLNA flags
*/
/*----------------------------------------------------------------------------*/
typedef struct
{
    CHAR *ps_mime;             /* the object MIME type */
    CHAR *ps_host;             /* the ake host if media protected*/
    CHAR *ps_port;             /* the port of host */
    CHAR *ps_rate;             /* audio rate */
    CHAR *ps_channels;          /* audio channel */
    CHAR *ps_profile_id;       /* the object profile id, get value in DLNA.ORG_PN */
    CHAR *ps_content_format;   /* the object content format, if the object is CP */

    /* MM protocolInfo values: 4th Field */
    /* Operations Parameter 
    this value for HTTP, time-base seek and byte-base seek, such as '11' means supporting all, '01' means support byte-base see;  
    this value for RTP, byte-base seek and '0', such as '10' means supporting byte-seek*/
    CHAR *ps_operations_parameter; 
    /* Server-Side PlaySpeeds Parameter */
    CHAR *ps_play_speeds;
    /*Conversion Indicator Flag*/
    CHAR *ps_conversion_indicator;
    /* Flags */
    CHAR *ps_flags;
} DLNA_PROTOCOL_INFO_T;


/* Device list change notify prototype the pv_arg is DLNA_DEVICE_T */
/*------------------------------------------------------------------*/
/*! @brief  callback of device found or bye-bye
*  @param [in]t_event             - The device event
*  @param [in]pv_tag              - The callback tag
*  @param [in]pv_arg              - The callback argument, the type is DLNA_DEVICE_T
*  @note
*  @see
*  @return
*  @retval      -
*/
/*------------------------------------------------------------------*/
typedef VOID (*x_dlna_device_nfy) (DLNA_DEVICE_EVENT_T t_event, 
              VOID * pv_tag,
              VOID * pv_arg);

/* Sema macro */
#define X_DLNA_SEMA_LOCK(x)    { if (x_sema_lock(x, X_SEMA_OPTION_WAIT) != OSR_OK) DBG_ASSERT(0, DBG_MOD_ACCESS_DLNA); }
#define X_DLNA_SEMA_UNLOCK(x)       { if (x_sema_unlock(x) != OSR_OK) DBG_ASSERT(0, DBG_MOD_ACCESS_DLNA); }
#define X_DLNA_SEMA_DELETE(x)       { if (x_sema_delete(x) != OSR_OK) DBG_ASSERT(0, DBG_MOD_ACCESS_DLNA); }
#define X_DLNA_SEMA_CREATE(h, t, s) { if(x_sema_create(h, t, s) != OSR_OK) DBG_ASSERT(0, DBG_MOD_ACCESS_DLNA); }

/* Check flag */
#define X_DLNA_CHECK_FLAG(a, b)     ((a) & (b))
#define X_DLNA_SET_FLAG(a, b)       ((a) |= (b))
#define X_DLNA_UNSET_FLAG(a, b)     ((a) &= (~(b)))

#endif

/*! @} */

