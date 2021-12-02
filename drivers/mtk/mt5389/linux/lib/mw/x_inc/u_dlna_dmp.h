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
/*! @file u_dlna_dmp.h
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2012/04/27 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA DMP exported constants, macros,
*         and types.
*/
/*----------------------------------------------------------------------------*/

/*!
* @addtogroup  groupMW_INET_DLNA
* @brief       Provide an external API about DLNA.
* %DLNA API would be used by user who wants to play media stream in local network  
* 
* @see 
*
* @{
* */


#ifndef U_DLNA_DMP_H
#define U_DLNA_DMP_H

#include "u_common.h"
#include "u_dlna_upnp.h"
#include "u_dbg.h"
#include "u_dlna.h"

/* DLNA DMP root container id is 0 */
#define DLNA_DMP_ROOT_CONTAINER_ID "0"

/* DLNA DMP Object debug */
#define DLNA_DMP_OBJECT_DEBUG

/* Return event for complete notify callback */
/*----------------------------------------------------------------------------*/
/*! @enum   DLNA_DMP_OBJECT_EVENT_T 
*  @brief  DLNA DMP action event
*  @code
*  typedef enum                     
*  {                                
*      DLNA_DMP_EVENT_BROWSE_OK = 1,
*      DLNA_DMP_EVENT_BROWSE_FAILED,
*      DLNA_DMP_EVENT_SEARCH_OK,               
*      DLNA_DMP_EVENT_SEARCH_FAILED 
*  } DLNA_DMP_OBJECT_EVENT_T;
*  @endcode
*   
*  @li@c DLNA_DMP_EVENT_BROWSE_OK                      - DLNA DMP browse ok
*  @li@c DLNA_DMP_EVENT_BROWSE_FAILED                  - DLNA DMP browse fail
*  @li@c DLNA_DMP_EVENT_SEARCH_OK                      - DLNA DMP search ok
*  @li@c DLNA_DMP_EVENT_SEARCH_FAILED                  - DLNA DMP search fail              
*/
/*----------------------------------------------------------------------------*/
typedef enum
{
    DLNA_DMP_EVENT_BROWSE_OK = 1,
    DLNA_DMP_EVENT_BROWSE_FAILED,
    DLNA_DMP_EVENT_SEARCH_OK,
    DLNA_DMP_EVENT_SEARCH_FAILED
} DLNA_DMP_OBJECT_EVENT_T;

/* Error code */
/*----------------------------------------------------------------------------*/
/*! @enum   INET_DLNAR_DMP_T 
*  @brief  DLNA DMP return type
*  @code
*  enum                                                                
*  {                                                                   
*      DLNAR_DMP_OK                                            = 0,    
*      DLNAR_DMP_FAIL                                          = -128, 
*      DLNAR_DMP_RW_FAIL                                       = -129,             
*      DLNAR_DMP_BROWSE_FAILED                                 = -130, 
*      DLNAR_DMP_DEVICE_FAILED                                 = -131, 
*      DLNAR_DMP_CONNECT_SERVER_FAILED                         = -132, 
*      DLNAR_DMP_SEARCH_FAILED                                 = -133, 
*      DLNAR_DMP_NOT_ENOUGH_RESOURCE                           = -134, 
*      DLNAR_DMP_OBJECT_NOT_EXIST                              = -135, 
*      DLNAR_DMP_ALREADY_START                                 = -136, 
*      DLNAR_DMP_NOT_START                                     = -137, 
*      DLNAR_DMP_INVALID_ARGUMENT                              = -138, 
*      DLNAR_DMP_NOT_INIT                                      = -139, 
*      DLNAR_DMP_DEVICE_SORT_CAPABILITY_FAILED                 = -140, 
*      DLNAR_DMP_DEVICE_SEARCH_CAPABILITY_FAILED               = -141, 
*      DLNAR_DMP_NOT_SUPPORT                                   = -142, 
*      DLNAR_DMP_CLOSED                                        = -143, 
*      DLNAR_DMP_TIME_OUT                                      = -144  
*  };
*  @endcode
*   
*  @li@c DLNAR_DMP_OK                                     - Return successful
*  @li@c DLNAR_DMP_FAIL                                   - Return fail
*  @li@c DLNAR_DMP_RW_FAIL                                - Return R/W fail
*  @li@c DLNAR_DMP_BROWSE_FAILED                          - Return browse fail
*  @li@c DLNAR_DMP_DEVICE_FAILED                          - Return device error
*  @li@c DLNAR_DMP_CONNECT_SERVER_FAILED                  - Return connect to server fail
*  @li@c DLNAR_DMP_SEARCH_FAILED                          - Return search fail
*  @li@c DLNAR_DMP_NOT_ENOUGH_RESOURCE                    - Return memory not enough
*  @li@c DLNAR_DMP_OBJECT_NOT_EXIST                       - Return object not exist
*  @li@c DLNAR_DMP_ALREADY_START                          - Return the client already started
*  @li@c DLNAR_DMP_NOT_START                              - Return the client not started
*  @li@c DLNAR_DMP_INVALID_ARGUMENT                       - Return invalid argument
*  @li@c DLNAR_DMP_NOT_INIT                               - Return the client not init
*  @li@c DLNAR_DMP_DEVICE_SORT_CAPABILITY_FAILED          - Return check the device sort capability fail 
*  @li@c DLNAR_DMP_DEVICE_SEARCH_CAPABILITY_FAILED        - Return check the device search capability fail 
*  @li@c DLNAR_DMP_NOT_SUPPORT                            - Return system not support
*  @li@c DLNAR_DMP_CLOSED                                 - Return the client or server closed 
*  @li@c DLNAR_DMP_TIME_OUT                               - Return time out            
*/
/*----------------------------------------------------------------------------*/
enum
{
    DLNAR_DMP_OK                                            = 0,
    DLNAR_DMP_FAIL                                          = -128,
    DLNAR_DMP_RW_FAIL                                       = -129,               
    DLNAR_DMP_BROWSE_FAILED                                 = -130,
    DLNAR_DMP_DEVICE_FAILED                                 = -131,
    DLNAR_DMP_CONNECT_SERVER_FAILED                         = -132,
    DLNAR_DMP_SEARCH_FAILED                                 = -133,
    DLNAR_DMP_NOT_ENOUGH_RESOURCE                           = -134,
    DLNAR_DMP_OBJECT_NOT_EXIST                              = -135,
    DLNAR_DMP_ALREADY_START                                 = -136,
    DLNAR_DMP_NOT_START                                     = -137,
    DLNAR_DMP_INVALID_ARGUMENT                              = -138,
    DLNAR_DMP_NOT_INIT                                      = -139,  
    DLNAR_DMP_DEVICE_SORT_CAPABILITY_FAILED                 = -140,
    DLNAR_DMP_DEVICE_SEARCH_CAPABILITY_FAILED               = -141,
    DLNAR_DMP_NOT_SUPPORT                                   = -142, 
    DLNAR_DMP_CLOSED                                        = -143,
    DLNAR_DMP_TIME_OUT                                      = -144
};

/* browse object flag */
/*----------------------------------------------------------------------------*/
/*! @enum   DLNA_DMP_BROWSE_FLAG_T 
*  @brief  DLNA browse flag 
*  @code
*  typedef enum                                                       
*  {                                                                  
*      DLNA_DMP_BROWSE_METADATA = 0,      
*      DLNA_DMP_BROWSE_DIRECT_CHILDREN    
*  } DLNA_DMP_BROWSE_FLAG_T; 
*  @endcode
*   
*  @li@c DLNA_DMP_BROWSE_METADATA                       - DLNA DMP browse metadata
*  @li@c DLNA_DMP_BROWSE_DIRECT_CHILDREN                - DLNA DMP browse dirent children
*              
*/
/*----------------------------------------------------------------------------*/
typedef enum
{
    DLNA_DMP_BROWSE_METADATA = 0,      /* browse object self */
    DLNA_DMP_BROWSE_DIRECT_CHILDREN    /* browse object children */
} DLNA_DMP_BROWSE_FLAG_T;



/* dmp object type */
/*----------------------------------------------------------------------------*/
/*! @enum  DLNA_DMP_OBJECT_TYPE_T 
*  @brief  DLNA dmp object type
*  @code
*  typedef enum                                   
*  {                                              
*      DLNA_DMP_OBJECT_TYPE_UNKNOWN = -1,         
*      DLNA_DMP_OBJECT_TYPE_DEVICE,               
*      DLNA_DMP_OBJECT_TYPE_MUSIC_ALBUM,                
*      DLNA_DMP_OBJECT_TYPE_PHOTO_ALBUM,          
*      DLNA_DMP_OBJECT_TYPE_ALBUM,                
*      DLNA_DMP_OBJECT_TYPE_PLAYLIST_CONTAINER,   
*      DLNA_DMP_OBJECT_TYPE_IMAGE_ITEM,           
*      DLNA_DMP_OBJECT_TYPE_PHOTO,                
*      DLNA_DMP_OBJECT_TYPE_MUSIC_VIDEO_CLIP,     
*      DLNA_DMP_OBJECT_TYPE_MUSIC_TRACK,          
*      DLNA_DMP_OBJECT_TYPE_AUDIO_ITEM,           
*      DLNA_DMP_OBJECT_TYPE_MOVIE,                
*      DLNA_DMP_OBJECT_TYPE_VIDEO_ITEM,           
*      DLNA_DMP_OBJECT_TYPE_PLAYLIST_ITEM,        
*      DLNA_DMP_OBJECT_TYPE_STORAGE_FOLDER,       
*      DLNA_DMP_OBJECT_TYPE_CONTAINER,            
*      DLNA_DMP_OBJECT_TYPE_ITEM,                 
*      DLNA_DMP_OBJECT_TYPE_END                   
*  } DLNA_DMP_OBJECT_TYPE_T;
*  @endcode
*   
*  @li@c DLNA_DMP_OBJECT_TYPE_UNKNOWN                    - DLNA DMP unknown
*  @li@c DLNA_DMP_OBJECT_TYPE_DEVICE                     - DLNA DMP device
*  @li@c DLNA_DMP_OBJECT_TYPE_MUSIC_ALBUM                - DLNA DMP music album
*  @li@c DLNA_DMP_OBJECT_TYPE_PHOTO_ALBUM                - DLNA DMP photo album
*  @li@c DLNA_DMP_OBJECT_TYPE_ALBUM                      - DLNA DMP album
*  @li@c DLNA_DMP_OBJECT_TYPE_PLAYLIST_CONTAINER         - DLNA DMP playlist container
*  @li@c DLNA_DMP_OBJECT_TYPE_IMAGE_ITEM                 - DLNA DMP image item
*  @li@c DLNA_DMP_OBJECT_TYPE_PHOTO                      - DLNA DMP photo
*  @li@c DLNA_DMP_OBJECT_TYPE_MUSIC_VIDEO_CLIP           - DLNA DMP music video clip
*  @li@c DLNA_DMP_OBJECT_TYPE_MUSIC_TRACK                - DLNA DMP music track
*  @li@c DLNA_DMP_OBJECT_TYPE_AUDIO_ITEM                 - DLNA DMP audio item
*  @li@c DLNA_DMP_OBJECT_TYPE_MOVIE                      - DLNA DMP movie
*  @li@c DLNA_DMP_OBJECT_TYPE_VIDEO_ITEM                 - DLNA DMP video item
*  @li@c DLNA_DMP_OBJECT_TYPE_PLAYLIST_ITEM              - DLNA DMP playlist item
*  @li@c DLNA_DMP_OBJECT_TYPE_STORAGE_FOLDER             - DLNA DMP storage folder
*  @li@c DLNA_DMP_OBJECT_TYPE_CONTAINER                  - DLNA DMP container
*  @li@c DLNA_DMP_OBJECT_TYPE_ITEM                       - DLNA DMP item
*  @li@c DLNA_DMP_OBJECT_TYPE_END                        - DLNA DMP end
*              
*/
/*----------------------------------------------------------------------------*/
typedef enum
{
    DLNA_DMP_OBJECT_TYPE_UNKNOWN = -1,
    DLNA_DMP_OBJECT_TYPE_DEVICE,             
    DLNA_DMP_OBJECT_TYPE_MUSIC_ALBUM,        
    DLNA_DMP_OBJECT_TYPE_PHOTO_ALBUM,        
    DLNA_DMP_OBJECT_TYPE_ALBUM,              
    DLNA_DMP_OBJECT_TYPE_PLAYLIST_CONTAINER, 
    DLNA_DMP_OBJECT_TYPE_IMAGE_ITEM,         
    DLNA_DMP_OBJECT_TYPE_PHOTO,              
    DLNA_DMP_OBJECT_TYPE_MUSIC_VIDEO_CLIP,   
    DLNA_DMP_OBJECT_TYPE_MUSIC_TRACK,        
    DLNA_DMP_OBJECT_TYPE_AUDIO_ITEM,         
    DLNA_DMP_OBJECT_TYPE_MOVIE,              
    DLNA_DMP_OBJECT_TYPE_VIDEO_ITEM,         
    DLNA_DMP_OBJECT_TYPE_PLAYLIST_ITEM,      
    DLNA_DMP_OBJECT_TYPE_STORAGE_FOLDER,     
    DLNA_DMP_OBJECT_TYPE_CONTAINER,          
    DLNA_DMP_OBJECT_TYPE_ITEM, 
    DLNA_DMP_OBJECT_TYPE_END
} DLNA_DMP_OBJECT_TYPE_T;

/* DLNA object DRM type */
/*----------------------------------------------------------------------------*/
/*! @enum   DLNA_DMP_OBJECT_DRM_TYPE_T 
*  @brief  DLNA DMP object drm type
*  @code
*  typedef enum {                                                           
*      DLNA_DMP_OBJECT_DRM_TYPE_UNKNOWN = -1,                               
*      DLNA_DMP_OBJECT_DRM_TYPE_NONE,                                       
*      DLNA_DMP_OBJECT_DRM_TYPE_DTCP,                                       
*      DLNA_DMP_OBJECT_DRM_TYPE_WMDRM,                                                  
*      DLNA_DMP_OBJECT_DRM_TYPE_END
*  } DLNA_DMP_OBJECT_DRM_TYPE_T;
*  @endcode
*   
*  @li@c DLNA_DMP_OBJECT_DRM_TYPE_UNKNOWN             - DLNA DMP unknown
*  @li@c DLNA_DMP_OBJECT_DRM_TYPE_NONE                - DLNA DMP none
*  @li@c DLNA_DMP_OBJECT_DRM_TYPE_DTCP                - DLNA DMP dtcp
*  @li@c DLNA_DMP_OBJECT_DRM_TYPE_WMDRM               - DLNA DMP wmdrm
*  @li@c DLNA_DMP_OBJECT_DRM_TYPE_END                 - DLNA DMP end
*              
*/
/*----------------------------------------------------------------------------*/
typedef enum {
    DLNA_DMP_OBJECT_DRM_TYPE_UNKNOWN = -1,
    DLNA_DMP_OBJECT_DRM_TYPE_NONE,
    DLNA_DMP_OBJECT_DRM_TYPE_DTCP,
    DLNA_DMP_OBJECT_DRM_TYPE_WMDRM,
    DLNA_DMP_OBJECT_DRM_TYPE_END /* Please keep this one the last one. */
} DLNA_DMP_OBJECT_DRM_TYPE_T;

/* DMP object info */
/*----------------------------------------------------------------------------*/
/*! @enum  DLNA_DMP_OBJECT_PROP_T  
*  @brief  DLNA DMP object property info
*  @code
*  typedef enum                                            
*  {                                                       
*      DLNA_DMP_OBJECT_PROP_ID	                  = 0,      
*      DLNA_DMP_OBJECT_PROP_PATH	              ,         
*      DLNA_DMP_OBJECT_PROP_TITLE	              ,               
*      DLNA_DMP_OBJECT_PROP_CREATOR	              ,         
*      DLNA_DMP_OBJECT_PROP_WRITE_STATUS          ,         
*      DLNA_DMP_OBJECT_PROP_RES_URI	              ,         
*      DLNA_DMP_OBJECT_PROP_RES_SIZE	          ,         
*      DLNA_DMP_OBJECT_PROP_RES_DURATION	      ,         
*      DLNA_DMP_OBJECT_PROP_RES_RESOLUTION	      ,         
*      DLNA_DMP_OBJECT_PROP_LONG_DESCRIPTION	  ,         
*      DLNA_DMP_OBJECT_PROP_DESCRIPTION	          ,         
*      DLNA_DMP_OBJECT_PROP_PUBLISHER	          ,         
*      DLNA_DMP_OBJECT_PROP_CONTRIBUTOR	          ,         
*      DLNA_DMP_OBJECT_PROP_DATE	              ,         
*      DLNA_DMP_OBJECT_PROP_COPYRIGHTS	          ,         
*      DLNA_DMP_OBJECT_PROP_ARTIST	              ,         
*      DLNA_DMP_OBJECT_PROP_ALBUM	              ,         
*      DLNA_DMP_OBJECT_PROP_GENRE	              ,         
*      DLNA_DMP_OBJECT_PROP_PRODUCER	          ,         
*      DLNA_DMP_OBJECT_PROP_ALBUM_ART_URI	      ,         
*      DLNA_DMP_OBJECT_PROP_LANGUAGE	          ,         
*      DLNA_DMP_OBJECT_PROP_ACTOR	              ,         
*      DLNA_DMP_OBJECT_PROP_CONTAINER_CHILD_COUNT ,         
*      DLNA_DMP_OBJECT_PROP_PARENT_ID	          ,         
*      DLNA_DMP_OBJECT_PROP_RES_BITRATE	          ,         
*      DLNA_DMP_OBJECT_PROP_RES_NRAUDIOCHANNELS   ,         
*      DLNA_DMP_OBJECT_PROP_RES_SAMPLEFREQUENCY   ,         
*      DLNA_DMP_OBJECT_PROP_RES_COLOR_DEPTH	      ,         
*      DLNA_DMP_OBJECT_PROP_RES_BITS_PER_SAMPLE   ,     
*      DLNA_DMP_OBJECT_PROP_PROTOCOL_INFO         ,         
*      DLNA_DMP_OBJECT_PROP_END                            
*  } DLNA_DMP_OBJECT_PROP_T;                               
*  @endcode
*   
*  @li@c DLNA_DMP_OBJECT_PROP_ID	                      - DLNA DMP property ID
*  @li@c DLNA_DMP_OBJECT_PROP_PATH	                      - DLNA DMP property path
*  @li@c DLNA_DMP_OBJECT_PROP_TITLE	                      - DLNA DMP property title
*  @li@c DLNA_DMP_OBJECT_PROP_CREATOR	                  - DLNA DMP property creator
*  @li@c DLNA_DMP_OBJECT_PROP_WRITE_STATUS                - DLNA DMP property write status
*  @li@c DLNA_DMP_OBJECT_PROP_RES_URI	                  - DLNA DMP property URI
*  @li@c DLNA_DMP_OBJECT_PROP_RES_SIZE	                  - DLNA DMP property size
*  @li@c DLNA_DMP_OBJECT_PROP_RES_DURATION	              - DLNA DMP property duration
*  @li@c DLNA_DMP_OBJECT_PROP_RES_RESOLUTION	          - DLNA DMP property resolution
*  @li@c DLNA_DMP_OBJECT_PROP_LONG_DESCRIPTION	          - DLNA DMP property long description
*  @li@c DLNA_DMP_OBJECT_PROP_DESCRIPTION	              - DLNA DMP property description
*  @li@c DLNA_DMP_OBJECT_PROP_PUBLISHER	                  - DLNA DMP property publisher
*  @li@c DLNA_DMP_OBJECT_PROP_CONTRIBUTOR	              - DLNA DMP property contributor
*  @li@c DLNA_DMP_OBJECT_PROP_DATE	                      - DLNA DMP property date
*  @li@c DLNA_DMP_OBJECT_PROP_COPYRIGHTS	              - DLNA DMP property copyright
*  @li@c DLNA_DMP_OBJECT_PROP_ARTIST	                  - DLNA DMP property artist
*  @li@c DLNA_DMP_OBJECT_PROP_ALBUM	                      - DLNA DMP property album
*  @li@c DLNA_DMP_OBJECT_PROP_GENRE	                      - DLNA DMP property genre
*  @li@c DLNA_DMP_OBJECT_PROP_PRODUCER	                  - DLNA DMP property producer
*  @li@c DLNA_DMP_OBJECT_PROP_ALBUM_ART_URI	              - DLNA DMP property album art URI
*  @li@c DLNA_DMP_OBJECT_PROP_LANGUAGE	                  - DLNA DMP property language
*  @li@c DLNA_DMP_OBJECT_PROP_ACTOR	                      - DLNA DMP property actor
*  @li@c DLNA_DMP_OBJECT_PROP_CONTAINER_CHILD_COUNT       - DLNA DMP property container child count
*  @li@c DLNA_DMP_OBJECT_PROP_PARENT_ID	                  - DLNA DMP property parent ID
*  @li@c DLNA_DMP_OBJECT_PROP_RES_BITRATE	              - DLNA DMP property bitrate
*  @li@c DLNA_DMP_OBJECT_PROP_RES_NRAUDIOCHANNELS         - DLNA DMP property NR audio channels
*  @li@c DLNA_DMP_OBJECT_PROP_RES_SAMPLEFREQUENCY         - DLNA DMP property sample frequency 
*  @li@c DLNA_DMP_OBJECT_PROP_RES_COLOR_DEPTH	          - DLNA DMP property color depth
*  @li@c DLNA_DMP_OBJECT_PROP_RES_BITS_PER_SAMPLE         - DLNA DMP property bits per sample
*  @li@c DLNA_DMP_OBJECT_PROP_PROTOCOL_INFO               - DLNA DMP property protocol info
*  @li@c DLNA_DMP_OBJECT_PROP_END                         - DLNA DMP property end
*/                 
/*----------------------------------------------------------------------------*/
typedef enum
{
    DLNA_DMP_OBJECT_PROP_ID	                  = 0,
    DLNA_DMP_OBJECT_PROP_PATH	              ,
    DLNA_DMP_OBJECT_PROP_TITLE	              ,
    DLNA_DMP_OBJECT_PROP_CREATOR	          ,
    DLNA_DMP_OBJECT_PROP_WRITE_STATUS         ,  
    DLNA_DMP_OBJECT_PROP_RES_URI	          ,
    DLNA_DMP_OBJECT_PROP_RES_SIZE	          ,
    DLNA_DMP_OBJECT_PROP_RES_DURATION	      ,
    DLNA_DMP_OBJECT_PROP_RES_RESOLUTION	      ,
    DLNA_DMP_OBJECT_PROP_LONG_DESCRIPTION	  ,    
    DLNA_DMP_OBJECT_PROP_DESCRIPTION	      ,
    DLNA_DMP_OBJECT_PROP_PUBLISHER	          ,
    DLNA_DMP_OBJECT_PROP_CONTRIBUTOR	      ,
    DLNA_DMP_OBJECT_PROP_DATE	              ,
    DLNA_DMP_OBJECT_PROP_COPYRIGHTS	          ,
    DLNA_DMP_OBJECT_PROP_ARTIST	              ,
    DLNA_DMP_OBJECT_PROP_ALBUM	              ,
    DLNA_DMP_OBJECT_PROP_GENRE	              ,
    DLNA_DMP_OBJECT_PROP_PRODUCER	          ,
    DLNA_DMP_OBJECT_PROP_ALBUM_ART_URI	      ,
    DLNA_DMP_OBJECT_PROP_LANGUAGE	          ,
    DLNA_DMP_OBJECT_PROP_ACTOR	              ,    
    DLNA_DMP_OBJECT_PROP_CONTAINER_CHILD_COUNT,
    DLNA_DMP_OBJECT_PROP_PARENT_ID	          ,
    DLNA_DMP_OBJECT_PROP_RES_BITRATE	      ,
    DLNA_DMP_OBJECT_PROP_RES_NRAUDIOCHANNELS  ,
    DLNA_DMP_OBJECT_PROP_RES_SAMPLEFREQUENCY  ,
    DLNA_DMP_OBJECT_PROP_RES_COLOR_DEPTH	  ,
    DLNA_DMP_OBJECT_PROP_RES_BITS_PER_SAMPLE  ,
    DLNA_DMP_OBJECT_PROP_PROTOCOL_INFO        ,
    DLNA_DMP_OBJECT_PROP_END
} DLNA_DMP_OBJECT_PROP_T;

/* DIDL keyword */
#define DLNA_DMP_SORT_ASCENDING_CRITERIA(prop)  ("+"##prop)   /* sort result by dlna prop ascending criteria */
#define DLNA_DMP_SORT_DESCENDING_CRITERIA(prop) ("-"##prop)   /*sort result  by dlna prop descending criteria*/
/* the sample of basic prop */
#define DLNA_DMP_SORT_ASCENDING_CRITERIA_TITLE      ("+dc:title")
#define DLNA_DMP_SORT_DESCENDING_CRITERIA_TITLE     ("-dc:title")
#define DLNA_DMP_SORT_ASCENDING_CRITERIA_ARTIST     ("+upnp:artist")
#define DLNA_DMP_SORT_DESCENDING_CRITERIA_ARTIST    ("-upnp:artist")
#define DLNA_DMP_SORT_ASCENDING_CRITERIA_DATE       ("+dc:date")
#define DLNA_DMP_SORT_DESCENDING_CRITERIA_DATE      ("-dc:date")


/* Filter */
#define DLNA_DMP_FILTER_BASIC_INFO (CHAR*)"res,res@resolution,res@protocolInfo,res@size,res@duration,res@bitrate,res@sampleFrequency,res@bitsPerSample,res@nrAudioChannels,res@protection,dc:creator,dc:date,upnp:genre,upnp:album,upnp:originalTrackNumber,upnp:channelNr,upnp:scheduledStartTime,upnp:scheduledEndTime,upnp:icon,upnp:albumArtURI,upnp:artist,container@childCount"
#define DLNA_DMP_FILTER_ALL_INFO   (CHAR*)"*"

/* Search */
#define DLNA_DMP_SEARCH_ALL_INFO   (CHAR*)"*" 

/*General type */
typedef DLNA_DEVICE_T DLNA_DMP_DEVICE_T;
typedef VOID * DLNA_DMP_CONTAINER_T;
typedef VOID * DLNA_DMP_OBJECT_T;
typedef VOID * DLNA_DMP_WIRE_INFO_T;



typedef DLNA_PROTOCOL_INFO_T DLNA_DMP_PROFILE_INFO_T;

#define DLNA_DMP_DEVICE_LIST_SIZE 100

/* Object device info */
/*----------------------------------------------------------------------------*/
/*! @struct   DLNA_DMP_DEVICE_INFO_LIST_T 
*  @brief  DLNA DMP device list
*  @code
*  typedef struct _DLNA_DMP_DEVICE_INFO_LIST_T                          
*  {                                                                    
*      INT32      i4_num;               
       DLNA_DMP_DEVICE_T t_list[DLNA_DMP_DEVICE_LIST_SIZE];             
*  } DLNA_DMP_DEVICE_INFO_LIST_T;                                       
*  @endcode
*   
*  @li@c i4_num                                  - DLNA DMP device number
*  @li@c t_list                                  - DLNA DMP device list
*              
*/
/*----------------------------------------------------------------------------*/
typedef struct _DLNA_DMP_DEVICE_INFO_LIST_T
{
    INT32      i4_num;               /* number of object in t_list */
    DLNA_DMP_DEVICE_T t_list[DLNA_DMP_DEVICE_LIST_SIZE];
} DLNA_DMP_DEVICE_INFO_LIST_T;

#define DLNA_DMP_OBJECT_LIST_SIZE 100 



/* DLNA object info */
/*----------------------------------------------------------------------------*/
/*! @struct DLNA_DMP_OBJECT_INFO_T 
*  @brief   DLNA DMP object info
*  @code
*  typedef struct _DLNA_DMP_OBJECT_INFO_T                                                         
*  {                                                                                              
*      INT32      i4_start_index;                            
*      INT32      i4_request_cnt;                            
*      DLNA_DMP_DEVICE_T t_dms;                                  
*      CHAR *     ps_object_id;                           
*      UINT32     ui4_total_matches;                         
*      INT32      i4_update_id;                              
*      INT32      i4_num;                                    
*      DLNA_DMP_OBJECT_T t_list[DLNA_DMP_OBJECT_LIST_SIZE]; 
*  } DLNA_DMP_OBJECT_INFO_T;
*  @endcode
*   
*  @li@c i4_start_index                       - DLNA DMP start index
*  @li@c i4_request_cnt                       - DLNA DMP request count
*  @li@c t_dms                                - DLNA DMP DMS
*  @li@c ps_object_id	                      - DLNA DMP object ID
*  @li@c ui4_total_matches                    - DLNA DMP total matches
*  @li@c i4_update_id	                      - DLNA DMP update ID
*  @li@c i4_num;                              - DLNA DMP number
*  @li@c t_list                               - DLNA DMP return list
*              
*/
/*----------------------------------------------------------------------------*/
typedef struct _DLNA_DMP_OBJECT_INFO_T   
{                
    INT32      i4_start_index;       /* start index */
    INT32      i4_request_cnt;       /* request count */
    DLNA_DMP_DEVICE_T t_dms;         /* internal use, my DMS server */
    CHAR *     ps_object_id;         /* the object id which browsed*/  

    UINT32     ui4_total_matches;    /* total match in current request*/
    INT32      i4_update_id;         /* the update id */ 
    INT32      i4_num;               /* number of object in t_list */
    DLNA_DMP_OBJECT_T t_list[DLNA_DMP_OBJECT_LIST_SIZE]; /* object direct children or itself */
} DLNA_DMP_OBJECT_INFO_T;

/* Complete notify prototype, the pv_arg is DLNA_DMP_OBJECT_INFO_T pointer */
/*------------------------------------------------------------------*/
/*! @brief  callback of device found or bye-bye
*  @param [in]t_event             - The device event
*  @param [in]pv_tag              - The callback tag
*  @param [in]pv_arg              - The callback argument, the type is DLNA_DMP_OBJECT_INFO_T
*  @note
*  @see
*  @return
*  @retval      -
*/
/*------------------------------------------------------------------*/
typedef VOID (*x_dlna_dmp_nfy) (DLNA_DMP_OBJECT_EVENT_T t_event, 
                                VOID * pv_tag,
                                VOID * pv_arg);

/* Sema macro */
#define X_DLNA_DMP_SEMA_LOCK(x) X_DLNA_SEMA_LOCK(x)    
#define X_DLNA_DMP_SEMA_UNLOCK(x) X_DLNA_SEMA_UNLOCK(x)      
#define X_DLNA_DMP_SEMA_DELETE(x) X_DLNA_SEMA_DELETE(x)      
#define X_DLNA_DMP_SEMA_CREATE(h, t, s)  X_DLNA_SEMA_CREATE(h, t, s) 

/* Check flag */
#define X_DLNA_DMP_CHECK_FLAG(a, b)     X_DLNA_CHECK_FLAG(a, b)
#define X_DLNA_DMP_SET_FLAG(a, b)       X_DLNA_SET_FLAG(a, b) 
#define X_DLNA_DMP_UNSET_FLAG(a, b)     X_DLNA_UNSET_FLAG(a, b)

/* dmp object seek type supported by dms */
/*----------------------------------------------------------------------------*/
/*! @enum   DLNA_DMP_FM_SEEK_TYPE_T 
*  @brief  DLNA DMP FM seek type
*  @code
*  typedef enum {                                                                                      
*      DLNA_DMP_FM_SEEK_NONSUPPORT = 0,            
*      DLNA_DMP_FM_SEEK_FULL_BYTE = 1,             
*      DLNA_DMP_FM_SEEK_FULL_TIME = 2,             
*      DLNA_DMP_FM_SEEK_FULL_BYTE_AND_TIME = 3,               
*      DLNA_DMP_FM_SEEK_LIMITED_BYTE = 4,          
*      DLNA_DMP_FM_SEEK_LIMITED_TIME = 8,          
*      DLNA_DMP_FM_SEEK_LIMITED_BYTE_AND_TIME = 12,
*      DLNA_DMP_FM_SEEK_UNKNOWN,                                                                       
*  } DLNA_DMP_FM_SEEK_TYPE_T;
*  @endcode
*   
*  @li@c DLNA_DMP_FM_SEEK_NONSUPPORT                - DLNA DMP not support
*  @li@c DLNA_DMP_FM_SEEK_FULL_BYTE                 - DLNA DMP full byte seek
*  @li@c DLNA_DMP_FM_SEEK_FULL_TIME                 - DLNA DMP full time seek
*  @li@c DLNA_DMP_FM_SEEK_FULL_BYTE_AND_TIME        - DLNA DMP full byte and time seek
*  @li@c DLNA_DMP_FM_SEEK_LIMITED_BYTE              - DLNA DMP limited byte seek
*  @li@c DLNA_DMP_FM_SEEK_LIMITED_TIME              - DLNA DMP limited time seek
*  @li@c DLNA_DMP_FM_SEEK_LIMITED_BYTE_AND_TIME     - DLNA DMP limited byte and time seek
*  @li@c DLNA_DMP_FM_SEEK_UNKNOWN                   - DLNA DMP unknown
*              
*/
/*----------------------------------------------------------------------------*/
typedef enum {
    DLNA_DMP_FM_SEEK_NONSUPPORT = 0,            /* nonsupport seek */     
    DLNA_DMP_FM_SEEK_FULL_BYTE = 1,                 /* full byte seek supported by dms */
    DLNA_DMP_FM_SEEK_FULL_TIME = 2,                 /* full time seek supported by dms */
    DLNA_DMP_FM_SEEK_FULL_BYTE_AND_TIME = 3,        /* full byte and time seek supported by dms */
    DLNA_DMP_FM_SEEK_LIMITED_BYTE = 4,              /* full byte seek supported by dms */
    DLNA_DMP_FM_SEEK_LIMITED_TIME = 8,              /* full time seek supported by dms */
    DLNA_DMP_FM_SEEK_LIMITED_BYTE_AND_TIME = 12,      /* full byte and time seek supported by dms */
    DLNA_DMP_FM_SEEK_UNKNOWN,
} DLNA_DMP_FM_SEEK_TYPE_T;

#endif

/*! @} */


