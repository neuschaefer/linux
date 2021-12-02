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
/*! @file u_dlna_dmr.h
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2012/04/27 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA DMR exported constants, macros,
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
#ifndef U_DLNA_DMR_H
#define U_DLNA_DMR_H

#include "u_common.h"
#include "u_dbg.h"
#include "u_dlna.h"

#define DLNA_DMR_HANDLE 0x444d52ff /* DMR */
#define DLNA_DMR_NAME   "MediaTek DMR"

/* Sema macro */
#define X_DLNA_DMR_SEMA_LOCK(x)    X_DLNA_SEMA_LOCK(x)  
#define X_DLNA_DMR_SEMA_UNLOCK(x)  X_DLNA_SEMA_UNLOCK(x) 
#define X_DLNA_DMR_SEMA_DELETE(x)  X_DLNA_SEMA_DELETE(x)
#define X_DLNA_DMR_SEMA_CREATE(h, t, s) X_DLNA_SEMA_CREATE(h, t, s)

/* Check flag */
#define X_DLNA_DMR_CHECK_FLAG(a, b)     X_DLNA_CHECK_FLAG(a, b)
#define X_DLNA_DMR_SET_FLAG(a, b)       X_DLNA_SET_FLAG(a, b) 
#define X_DLNA_DMR_UNSET_FLAG(a, b)     X_DLNA_UNSET_FLAG(a, b)

/* player action event */
/*----------------------------------------------------------------------------*/
/*! @enum  DLNA_DMR_PLAYER_ACTION_EVENT_T  
*  @brief  DLNA DMR player action event
*  @code
*  typedef enum                                                          
*  {                                                                     
*      DLNA_DMR_PLAYER_ACTION_UNKNOWN = 0,                               
*      DLNA_DMR_PLAYER_ACTION_OPEN,                                      
*      DLNA_DMR_PLAYER_ACTION_START,                                     
*      DLNA_DMR_PLAYER_ACTION_PAUSE,                                     
*      DLNA_DMR_PLAYER_ACTION_STOP,                                      
*      DLNA_DMR_PLAYER_ACTION_CLOSE,                                     
*      DLNA_DMR_PLAYER_ACTION_PREVIOUS,                                  
*      DLNA_DMR_PLAYER_ACTION_NEXT,                                      
*      DLNA_DMR_PLAYER_ACTION_FF,                                        
*      DLNA_DMR_PLAYER_ACTION_FR,                                        
*      DLNA_DMR_PLAYER_ACTION_END_OF_MEDIA,                              
*      DLNA_DMR_PLAYER_ACTION_SEEKTIME,      
*      DLNA_DMR_PLAYER_ACTION_SEEKBYTE,      
*      DLNA_DMR_PLAYER_ACTION_CONTROL,       
*      DLNA_DMR_PLAYER_ACTION_UPDATE,        
*      DLNA_DMR_PLAYER_ACTION_ERROR,         
*      DLNA_DMR_PLAYER_ACTION_RECORD,        
*      DLNA_DMR_PLAYER_ACTION_READ_READY,                                
*      DLNA_DMR_PLAYER_ACTION_WRITE_READY,                               
*  } DLNA_DMR_PLAYER_ACTION_EVENT_T;                
*  @endcode
*   
*  @li@c DLNA_DMR_PLAYER_ACTION_UNKNOWN               - DLNA DMR action unknown
*  @li@c DLNA_DMR_PLAYER_ACTION_OPEN                  - DLNA DMR action open
*  @li@c DLNA_DMR_PLAYER_ACTION_START                 - DLNA DMR action start
*  @li@c DLNA_DMR_PLAYER_ACTION_PAUSE                 - DLNA DMR action pause
*  @li@c DLNA_DMR_PLAYER_ACTION_STOP                  - DLNA DMR action stop
*  @li@c DLNA_DMR_PLAYER_ACTION_CLOSE                 - DLNA DMR action close
*  @li@c DLNA_DMR_PLAYER_ACTION_PREVIOUS              - DLNA DMR action previous
*  @li@c DLNA_DMR_PLAYER_ACTION_NEXT                  - DLNA DMR action next
*  @li@c DLNA_DMR_PLAYER_ACTION_FF                    - DLNA DMR action fast forward
*  @li@c DLNA_DMR_PLAYER_ACTION_FR                    - DLNA DMR action fast rewind
*  @li@c DLNA_DMR_PLAYER_ACTION_END_OF_MEDIA          - DLNA DMR action end of media
*  @li@c DLNA_DMR_PLAYER_ACTION_SEEKTIME              - DLNA DMR action time seek
*  @li@c DLNA_DMR_PLAYER_ACTION_SEEKBYTE              - DLNA DMR action byte seek
*  @li@c DLNA_DMR_PLAYER_ACTION_CONTROL               - DLNA DMR action control
*  @li@c DLNA_DMR_PLAYER_ACTION_UPDATE                - DLNA DMR action update
*  @li@c DLNA_DMR_PLAYER_ACTION_ERROR                 - DLNA DMR action error
*  @li@c DLNA_DMR_PLAYER_ACTION_RECORD                - DLNA DMR action record
*  @li@c DLNA_DMR_PLAYER_ACTION_READ_READY            - DLNA DMR action read ready
*  @li@c DLNA_DMR_PLAYER_ACTION_WRITE_READY           - DLNA DMR action write ready
*
*/                 
/*----------------------------------------------------------------------------*/
typedef enum 
{
    DLNA_DMR_PLAYER_ACTION_UNKNOWN = 0,
    DLNA_DMR_PLAYER_ACTION_OPEN,
    DLNA_DMR_PLAYER_ACTION_START,
    DLNA_DMR_PLAYER_ACTION_PAUSE,
    DLNA_DMR_PLAYER_ACTION_STOP,
    DLNA_DMR_PLAYER_ACTION_CLOSE,
    DLNA_DMR_PLAYER_ACTION_PREVIOUS,
    DLNA_DMR_PLAYER_ACTION_NEXT, 
    DLNA_DMR_PLAYER_ACTION_FF,
    DLNA_DMR_PLAYER_ACTION_FR,    
    DLNA_DMR_PLAYER_ACTION_END_OF_MEDIA,
    DLNA_DMR_PLAYER_ACTION_SEEKTIME,      /* time base seek */
    DLNA_DMR_PLAYER_ACTION_SEEKBYTE,      /* byte base seek */
    DLNA_DMR_PLAYER_ACTION_CONTROL,       /* rendering control */
    DLNA_DMR_PLAYER_ACTION_UPDATE,        /* update progress info */  
    DLNA_DMR_PLAYER_ACTION_ERROR,
    DLNA_DMR_PLAYER_ACTION_RECORD,        /* record */ 
    DLNA_DMR_PLAYER_ACTION_READ_READY,
    DLNA_DMR_PLAYER_ACTION_WRITE_READY,
    DLNA_DMR_PLAYER_ACTION_RESUME, /*add by chenkun for resume the player*/
} DLNA_DMR_PLAYER_ACTION_EVENT_T;

/* player callback event, this event callback by user while action end */
/*----------------------------------------------------------------------------*/
/*! @enum  DLNA_DMR_PLAYER_CALLBACK_EVENT_T  
*  @brief  DLNA DMR player action event
*  @code
*  typedef enum                                                          
*  {                                                                     
*      DLNA_DMR_PLAYER_CALLBACK_UNKNOWN = 0,                               
*      DLNA_DMR_PLAYER_CALLBACK_OPEN,                                      
*      DLNA_DMR_PLAYER_CALLBACK_START,                                     
*      DLNA_DMR_PLAYER_CALLBACK_PAUSE,                                     
*      DLNA_DMR_PLAYER_CALLBACK_STOP,                                      
*      DLNA_DMR_PLAYER_CALLBACK_CLOSE,                                     
*      DLNA_DMR_PLAYER_CALLBACK_PREVIOUS,                                  
*      DLNA_DMR_PLAYER_CALLBACK_NEXT,                                      
*      DLNA_DMR_PLAYER_CALLBACK_FF,                                        
*      DLNA_DMR_PLAYER_CALLBACK_FR,                                        
*      DLNA_DMR_PLAYER_CALLBACK_END_OF_MEDIA,                              
*      DLNA_DMR_PLAYER_CALLBACK_SEEKTIME,      
*      DLNA_DMR_PLAYER_CALLBACK_SEEKBYTE,      
*      DLNA_DMR_PLAYER_CALLBACK_CONTROL,       
*      DLNA_DMR_PLAYER_CALLBACK_UPDATE,        
*      DLNA_DMR_PLAYER_CALLBACK_ERROR,         
*      DLNA_DMR_PLAYER_CALLBACK_RECORD,        
*      DLNA_DMR_PLAYER_CALLBACK_READ_READY,                                
*      DLNA_DMR_PLAYER_CALLBACK_WRITE_READY,                               
*  } DLNA_DMR_PLAYER_CALLBACK_EVENT_T;                
*  @endcode
*   
*  @li@c DLNA_DMR_PLAYER_CALLBACK_UNKNOWN               - DLNA DMR callback unknown
*  @li@c DLNA_DMR_PLAYER_CALLBACK_OPEN                  - DLNA DMR callback open
*  @li@c DLNA_DMR_PLAYER_CALLBACK_START                 - DLNA DMR callback start
*  @li@c DLNA_DMR_PLAYER_CALLBACK_PAUSE                 - DLNA DMR callback pause
*  @li@c DLNA_DMR_PLAYER_CALLBACK_STOP                  - DLNA DMR callback stop
*  @li@c DLNA_DMR_PLAYER_CALLBACK_CLOSE                 - DLNA DMR callback close
*  @li@c DLNA_DMR_PLAYER_CALLBACK_PREVIOUS              - DLNA DMR callback previous
*  @li@c DLNA_DMR_PLAYER_CALLBACK_NEXT                  - DLNA DMR callback next
*  @li@c DLNA_DMR_PLAYER_CALLBACK_FF                    - DLNA DMR callback fast forward
*  @li@c DLNA_DMR_PLAYER_CALLBACK_FR                    - DLNA DMR callback fast rewind
*  @li@c DLNA_DMR_PLAYER_CALLBACK_END_OF_MEDIA          - DLNA DMR callback end of media
*  @li@c DLNA_DMR_PLAYER_CALLBACK_SEEKTIME              - DLNA DMR callback time seek
*  @li@c DLNA_DMR_PLAYER_CALLBACK_SEEKBYTE              - DLNA DMR callback byte seek
*  @li@c DLNA_DMR_PLAYER_CALLBACK_CONTROL               - DLNA DMR callback control
*  @li@c DLNA_DMR_PLAYER_CALLBACK_UPDATE                - DLNA DMR callback update
*  @li@c DLNA_DMR_PLAYER_CALLBACK_ERROR                 - DLNA DMR callback error
*  @li@c DLNA_DMR_PLAYER_CALLBACK_RECORD                - DLNA DMR callback record
*  @li@c DLNA_DMR_PLAYER_CALLBACK_READ_READY            - DLNA DMR callback read ready
*  @li@c DLNA_DMR_PLAYER_CALLBACK_WRITE_READY           - DLNA DMR callback write ready
*
*/                 
/*----------------------------------------------------------------------------*/
typedef enum 
{
    DLNA_DMR_PLAYER_CALLBACK_UNKNOWN = 0,
    DLNA_DMR_PLAYER_CALLBACK_OPEN,
    DLNA_DMR_PLAYER_CALLBACK_START,
    DLNA_DMR_PLAYER_CALLBACK_PAUSE,
    DLNA_DMR_PLAYER_CALLBACK_STOP,
    DLNA_DMR_PLAYER_CALLBACK_CLOSE,
    DLNA_DMR_PLAYER_CALLBACK_PREVIOUS,
    DLNA_DMR_PLAYER_CALLBACK_NEXT, 
    DLNA_DMR_PLAYER_CALLBACK_FF,
    DLNA_DMR_PLAYER_CALLBACK_FR,    
    DLNA_DMR_PLAYER_CALLBACK_END_OF_MEDIA,
    DLNA_DMR_PLAYER_CALLBACK_SEEKTIME,      /* time base seek */
    DLNA_DMR_PLAYER_CALLBACK_SEEKBYTE,      /* byte base seek */
    DLNA_DMR_PLAYER_CALLBACK_CONTROL,       /* rendering control */
    DLNA_DMR_PLAYER_CALLBACK_UPDATE,        /* update progress info */  
    DLNA_DMR_PLAYER_CALLBACK_ERROR,
    DLNA_DMR_PLAYER_CALLBACK_RECORD,        /* record */ 
    DLNA_DMR_PLAYER_CALLBACK_READ_READY,
    DLNA_DMR_PLAYER_CALLBACK_WRITE_READY,
} DLNA_DMR_PLAYER_CALLBACK_EVENT_T;

/* player rendering control */
/*----------------------------------------------------------------------------*/
/*! @enum  DLNA_DMR_PLAYER_CONTROL_T  
*  @brief  DLNA DMP player control
*  @code
*  typedef enum                                            
*  {                                                       
*      DLNA_DMR_PLAYER_CONTROL_UNKNOWN = 0,                 
*      DLNA_DMR_PLAYER_CONTROL_BRIGHTNESS,                  
*      DLNA_DMR_PLAYER_CONTROL_CONTRAST,                          
*      DLNA_DMR_PLAYER_CONTROL_SHARPNESS,                   
*      DLNA_DMR_PLAYER_CONTROL_RED_VIDEO_GAIN,              
*      DLNA_DMR_PLAYER_CONTROL_GREED_VIDEO_GAIN,            
*      DLNA_DMR_PLAYER_CONTROL_BLUE_VIDEO_GAIN,             
*      DLNA_DMR_PLAYER_CONTROL_RED_VIDEO_BLACK_LEVEL,       
*      DLNA_DMR_PLAYER_CONTROL_GREED_VIDEO_BLACK_LEVEL,     
*      DLNA_DMR_PLAYER_CONTROL_BLUE_VIDEO_BLACK_LEVEL,      
*      DLNA_DMR_PLAYER_CONTROL_COLOR_TEMPERATURE,           
*      DLNA_DMR_PLAYER_CONTROL_HORIZONTAL_KEYSTORE,         
*      DLNA_DMR_PLAYER_CONTROL_VERTICAL_KEYSTORE,           
*      DLNA_DMR_PLAYER_CONTROL_MUTE,                        
*      DLNA_DMR_PLAYER_CONTROL_VOLUME,                      
*      DLNA_DMR_PLAYER_CONTROL_VOLUMEDB,                    
*      DLNA_DMR_PLAYER_CONTROL_LOUDNESS,                    
*      DLNA_DMR_PLAYER_CONTROL_CHANNEL                      
*  } DLNA_DMR_PLAYER_CONTROL_T;         
*  @endcode
*   
*  @li@c DLNA_DMR_PLAYER_CONTROL_UNKNOWN                       - DLNA DMP player control unknown
*  @li@c DLNA_DMR_PLAYER_CONTROL_BRIGHTNESS                    - DLNA DMP player control brightness
*  @li@c DLNA_DMR_PLAYER_CONTROL_CONTRAST                      - DLNA DMP player control contrast
*  @li@c DLNA_DMR_PLAYER_CONTROL_SHARPNESS                     - DLNA DMP player control sharpness
*  @li@c DLNA_DMR_PLAYER_CONTROL_RED_VIDEO_GAIN                - DLNA DMP player control read video gain
*  @li@c DLNA_DMR_PLAYER_CONTROL_GREED_VIDEO_GAIN              - DLNA DMP player control green video gain
*  @li@c DLNA_DMR_PLAYER_CONTROL_BLUE_VIDEO_GAIN               - DLNA DMP player control blue video gain
*  @li@c DLNA_DMR_PLAYER_CONTROL_RED_VIDEO_BLACK_LEVEL         - DLNA DMP player control red video black level
*  @li@c DLNA_DMR_PLAYER_CONTROL_GREED_VIDEO_BLACK_LEVEL       - DLNA DMP player control green video black level
*  @li@c DLNA_DMR_PLAYER_CONTROL_BLUE_VIDEO_BLACK_LEVEL        - DLNA DMP player control blue video black level
*  @li@c DLNA_DMR_PLAYER_CONTROL_COLOR_TEMPERATURE             - DLNA DMP player control color temperature
*  @li@c DLNA_DMR_PLAYER_CONTROL_HORIZONTAL_KEYSTORE           - DLNA DMP player control horizontal keystore
*  @li@c DLNA_DMR_PLAYER_CONTROL_VERTICAL_KEYSTORE             - DLNA DMP player control verticaol keystore
*  @li@c DLNA_DMR_PLAYER_CONTROL_MUTE                          - DLNA DMP player control mute
*  @li@c DLNA_DMR_PLAYER_CONTROL_VOLUME                        - DLNA DMP player control volume
*  @li@c DLNA_DMR_PLAYER_CONTROL_VOLUMEDB                      - DLNA DMP player control volume DB
*  @li@c DLNA_DMR_PLAYER_CONTROL_LOUDNESS                      - DLNA DMP player control loudness
*  @li@c DLNA_DMR_PLAYER_CONTROL_CHANNEL                       - DLNA DMP player control channel
*/                 
/*----------------------------------------------------------------------------*/
typedef enum 
{
    DLNA_DMR_PLAYER_CONTROL_UNKNOWN = 0, 
    DLNA_DMR_PLAYER_CONTROL_BRIGHTNESS,
    DLNA_DMR_PLAYER_CONTROL_CONTRAST,
    DLNA_DMR_PLAYER_CONTROL_SHARPNESS,
    DLNA_DMR_PLAYER_CONTROL_RED_VIDEO_GAIN,
    DLNA_DMR_PLAYER_CONTROL_GREED_VIDEO_GAIN,
    DLNA_DMR_PLAYER_CONTROL_BLUE_VIDEO_GAIN,
    DLNA_DMR_PLAYER_CONTROL_RED_VIDEO_BLACK_LEVEL,
    DLNA_DMR_PLAYER_CONTROL_GREED_VIDEO_BLACK_LEVEL,
    DLNA_DMR_PLAYER_CONTROL_BLUE_VIDEO_BLACK_LEVEL,
    DLNA_DMR_PLAYER_CONTROL_COLOR_TEMPERATURE,
    DLNA_DMR_PLAYER_CONTROL_HORIZONTAL_KEYSTORE,
    DLNA_DMR_PLAYER_CONTROL_VERTICAL_KEYSTORE,
    DLNA_DMR_PLAYER_CONTROL_MUTE,
    DLNA_DMR_PLAYER_CONTROL_VOLUME,
    DLNA_DMR_PLAYER_CONTROL_VOLUMEDB,
    DLNA_DMR_PLAYER_CONTROL_LOUDNESS,
    DLNA_DMR_PLAYER_CONTROL_CHANNEL
} DLNA_DMR_PLAYER_CONTROL_T;




/*----------------------------------------------------------------------------*/
/*! @enum  DLNA_DMR_PLAYER_RENDERING_CONTROL_CHANNEL_T  
*  @brief  DLNA DMR player rendering control channel
*  @code
*  typedef enum {                                 
*      DLNA_DMR_PLAYER_RC_CHANNEL_UNKNOWN = 0,    
*      DLNA_DMR_PLAYER_RC_CHANNEL_MASTER,          
*      DLNA_DMR_PLAYER_RC_CHANNEL_LF,              
*      DLNA_DMR_PLAYER_RC_CHANNEL_RF,                    
*      DLNA_DMR_PLAYER_RC_CHANNEL_CF,              
*      DLNA_DMR_PLAYER_RC_CHANNEL_LFE,             
*      DLNA_DMR_PLAYER_RC_CHANNEL_LS,              
*      DLNA_DMR_PLAYER_RC_CHANNEL_RS,              
*      DLNA_DMR_PLAYER_RC_CHANNEL_LFC,             
*      DLNA_DMR_PLAYER_RC_CHANNEL_RFC,             
*      DLNA_DMR_PLAYER_RC_CHANNEL_SD,              
*      DLNA_DMR_PLAYER_RC_CHANNEL_SL,              
*      DLNA_DMR_PLAYER_RC_CHANNEL_SR,              
*      DLNA_DMR_PLAYER_RC_CHANNEL_T,               
*      DLNA_DMR_PLAYER_RC_CHANNEL_B                
*  } DLNA_DMR_PLAYER_RENDERING_CONTROL_CHANNEL_T; 
*  @endcode
*   
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_UNKNOWN             - DLNA DMR player rendering control channel unknown
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_MASTER              - DLNA DMR player rendering control channel master
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_LF                  - DLNA DMR player rendering control channel LF
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_RF                  - DLNA DMR player rendering control channel RF
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_CF                  - DLNA DMR player rendering control channel CF
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_LFE                 - DLNA DMR player rendering control channel LFE
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_LS                  - DLNA DMR player rendering control channel LS
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_RS                  - DLNA DMR player rendering control channel RS
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_LFC                 - DLNA DMR player rendering control channel LFC
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_RFC                 - DLNA DMR player rendering control channel RFC
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_SD                  - DLNA DMR player rendering control channel SD
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_SL                  - DLNA DMR player rendering control channel SL
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_SR                  - DLNA DMR player rendering control channel SR
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_T                   - DLNA DMR player rendering control channel T
*  @li@c DLNA_DMR_PLAYER_RC_CHANNEL_B                   - DLNA DMR player rendering control channel B
*/                 
/*----------------------------------------------------------------------------*/
typedef enum {
    DLNA_DMR_PLAYER_RC_CHANNEL_UNKNOWN = 0,
    DLNA_DMR_PLAYER_RC_CHANNEL_MASTER,
    DLNA_DMR_PLAYER_RC_CHANNEL_LF,
    DLNA_DMR_PLAYER_RC_CHANNEL_RF,
    DLNA_DMR_PLAYER_RC_CHANNEL_CF,
    DLNA_DMR_PLAYER_RC_CHANNEL_LFE,
    DLNA_DMR_PLAYER_RC_CHANNEL_LS,
    DLNA_DMR_PLAYER_RC_CHANNEL_RS,
    DLNA_DMR_PLAYER_RC_CHANNEL_LFC,
    DLNA_DMR_PLAYER_RC_CHANNEL_RFC,
    DLNA_DMR_PLAYER_RC_CHANNEL_SD,
    DLNA_DMR_PLAYER_RC_CHANNEL_SL,
    DLNA_DMR_PLAYER_RC_CHANNEL_SR,
    DLNA_DMR_PLAYER_RC_CHANNEL_T,
    DLNA_DMR_PLAYER_RC_CHANNEL_B        
} DLNA_DMR_PLAYER_RENDERING_CONTROL_CHANNEL_T;



/*----------------------------------------------------------------------------*/
/*! @struct DLNA_DMR_PLAYER_RENDERING_CONTROL_T 
*  @brief   DLNA DMR player rendering control
*  @code
*  typedef struct                                                                                  
*  {                                                                                               
*      UINT16 ui2_brightness;                                 
*      UINT16 ui2_contrast;                                   
*      UINT16 ui2_sharpness;                                      
*      UINT16 ui2_red_video_gain;                            
*      UINT16 ui2_greed_video_gain;                           
*      UINT16 ui2_blue_video_gain;                            
*      UINT16 ui2_red_video_black_level;                      
*      UINT16 ui2_greed_video_black_level;                   
*      UINT16 ui2_blue_video_black_level;                    
*      UINT16 ui2_color_temperature;                         
*      UINT16 ui2_horizontal_keystore;                       
*      UINT16 ui2_vertical_keystore;                         
*      BOOL   b_mute;                                        
*      UINT16 ui2_volume;                                    
*      INT16  i2_volumeDB;                                   
*      BOOL   b_loudness;                                    
*      DLNA_DMR_PLAYER_RENDERING_CONTROL_CHANNEL_T e_channel;
*  } DLNA_DMR_PLAYER_RENDERING_CONTROL_T;                   
*
*  @endcode
*  
*  @li@c     ui2_brightness                                       - DLNA DMR rendering control brightness
*  @li@c     ui2_contrast                                         - DLNA DMR rendering control contrast
*  @li@c     ui2_sharpness                                        - DLNA DMR rendering control sharpness
*  @li@c     ui2_red_video_gain                                   - DLNA DMR rendering control read video gain            
*  @li@c     ui2_greed_video_gain                                 - DLNA DMR rendering control green video gain           
*  @li@c     ui2_blue_video_gain                                  - DLNA DMR rendering control blue video gain            
*  @li@c     ui2_red_video_black_level                            - DLNA DMR rendering control red video black level      
*  @li@c     ui2_greed_video_black_level                          - DLNA DMR rendering control green video black level    
*  @li@c     ui2_blue_video_black_level                           - DLNA DMR rendering control blue video black level     
*  @li@c     ui2_color_temperature                                - DLNA DMR rendering control color temperature          
*  @li@c     ui2_horizontal_keystore                              - DLNA DMR rendering control horizontal keystore        
*  @li@c     ui2_vertical_keystore                                - DLNA DMR rendering control verticaol keystore         
*  @li@c     b_mute                                               - DLNA DMR rendering control mute                       
*  @li@c     ui2_volume                                           - DLNA DMR rendering control volume                     
*  @li@c     i2_volumeDB                                          - DLNA DMR rendering control volume DB                  
*  @li@c     b_loudness                                           - DLNA DMR rendering control loudness                   
*  @li@c     e_channel                                            - DLNA DMR rendering control channel                    
*  
*/                   
/*----------------------------------------------------------------------------*/
typedef struct
{
    UINT16 ui2_brightness;
    UINT16 ui2_contrast;
    UINT16 ui2_sharpness;
    UINT16 ui2_red_video_gain;
    UINT16 ui2_greed_video_gain;
    UINT16 ui2_blue_video_gain;
    UINT16 ui2_red_video_black_level;
    UINT16 ui2_greed_video_black_level;
    UINT16 ui2_blue_video_black_level;
    UINT16 ui2_color_temperature;
    UINT16 ui2_horizontal_keystore;
    UINT16 ui2_vertical_keystore;
    BOOL   b_mute;
    UINT16 ui2_volume;
    INT16  i2_volumeDB;
    BOOL   b_loudness;
    DLNA_DMR_PLAYER_RENDERING_CONTROL_CHANNEL_T e_channel;
} DLNA_DMR_PLAYER_RENDERING_CONTROL_T;

     
/*------------------------------------------------------------------*/
/*! @brief  DMR player callback
*  @param [in]t_event             - The player callback event
*  @param [in]pv_tag              - The callback tag
*  @note
*  @see
*  @return
*  @retval      -
*/
/*------------------------------------------------------------------*/
typedef VOID (*x_dlna_dmr_player_callback) (DLNA_DMR_PLAYER_CALLBACK_EVENT_T e_event, 
                                            VOID * pv_tag);
/* input argument */
#define IN_PARAMETER_T
/*output argument */
#define OUT_PARAMETER_T

/*----------------------------------------------------------------------------*/
/*! @struct DLNA_DMR_PLAYER_ACTION_PARAMETER_T 
*  @brief   DLNA DMR player action parameter
*  @code
*  typedef struct _DLNA_DMR_PLAYER_ACTION_PARAMETER_T{                                                                                                      
*      IN_PARAMETER_T UINT32 ui4_version;                                                      
*      IN_PARAMETER_T CHAR * ps_uri;                              
*      IN_PARAMETER_T CHAR * ps_protocol_info;                                                                                   
*      IN_PARAMETER_T INT32 i4_play_speed;                        
*      IN_PARAMETER_T INT64 i8_offset;                            
*      IN_PARAMETER_T INT32 i4_seconds;                                                                                       
*      IN_PARAMETER_T DLNA_DMR_PLAYER_CONTROL_T   e_control;      
*      IN_PARAMETER_T INT32                       i4_value;                                                                        
*      IN_PARAMETER_T UINT8* pui1_buf;                            
*      IN_PARAMETER_T UINT32 ui4_length;                                                                                        
*      IN_PARAMETER_T x_dlna_dmr_player_callback    pf_callback;  
*      IN_PARAMETER_T VOID * pv_tag;                                                                                            
*      OUT_PARAMETER_T INT32    i4_progress;                      
*      OUT_PARAMETER_T UINT64   ui8_position;                     
*      OUT_PARAMETER_T UINT16   ui2_current_hour;                 
*      OUT_PARAMETER_T UINT8    ui1_current_minute;                                                                          
*      OUT_PARAMETER_T UINT8    ui1_current_second;                                                                          
*      OUT_PARAMETER_T UINT32   ui4_current_microsecond;                                                                     
*      OUT_PARAMETER_T DLNA_DMR_PLAYER_RENDERING_CONTROL_T t_current_rendering;                                                                                                                                        
*      VOID * pv_data;                                                                         
*  } DLNA_DMR_PLAYER_ACTION_PARAMETER_T;           
*  @endcode
*   
*  @li@c        ui4_version                            - DLNA DMR action version
*  @li@c        ps_uri                                 - DLNA DMR action URI
*  @li@c        ps_protocol_info                       - DLNA DMR action protocol
*  @li@c        i4_play_speed                          - DLNA DMR action play speed
*  @li@c        i8_offset                              - DLNA DMR action offset
*  @li@c        i4_seconds                             - DLNA DMR action second
*  @li@c        e_control                      		   - DLNA DMR action control
*  @li@c        i4_value                       		   - DLNA DMR action value
*  @li@c        pui1_buf                               - DLNA DMR action buffer
*  @li@c        ui4_length                             - DLNA DMR action length
*  @li@c        pf_callback                            - DLNA DMR action callback
*  @li@c        pv_tag                                 - DLNA DMR action tag
*  @li@c        i4_progress                            - DLNA DMR action progress
*  @li@c        ui8_position                           - DLNA DMR action position
*  @li@c        ui2_current_hour                       - DLNA DMR action current hour
*  @li@c        ui1_current_minute                     - DLNA DMR action current minute
*  @li@c        ui1_current_second                     - DLNA DMR action current second
*  @li@c        ui4_current_microsecond                - DLNA DMR action current microsecond
*  @li@c        t_current_rendering                    - DLNA DMR action current rendering
*  @li@c        pv_data                                - DLNA DMR action data
*              
*/
/*----------------------------------------------------------------------------*/
typedef struct _DLNA_DMR_PLAYER_ACTION_PARAMETER_T{  
    IN_PARAMETER_T UINT32 ui4_version;                         /* notify version, increased every action */ 
    IN_PARAMETER_T CHAR * ps_uri;                              /* content uri */           
    IN_PARAMETER_T CHAR * ps_protocol_info;                    /* content protocol info */
             
    IN_PARAMETER_T INT32 i4_play_speed;                        /* play speed */         
    IN_PARAMETER_T INT64 i8_offset;                            /* byte seek */
    IN_PARAMETER_T INT32 i4_seconds;                           /* time seek */
    
    IN_PARAMETER_T DLNA_DMR_PLAYER_CONTROL_T   e_control;      /* render control event */
    IN_PARAMETER_T INT32                       i4_value;       /* render control message */ 
     
    IN_PARAMETER_T UINT8* pui1_buf;                            /* upload or download buffer */
    IN_PARAMETER_T UINT32 ui4_length;                          /* upload or download buffer size */
    
    IN_PARAMETER_T x_dlna_dmr_player_callback    pf_callback;  /* callback to dmr while player action has been executed */
    IN_PARAMETER_T VOID * pv_tag;
    
    OUT_PARAMETER_T INT32    i4_progress;                       /* percent of content, update by player */
    OUT_PARAMETER_T UINT64   ui8_position;                      /* current byte position */
    OUT_PARAMETER_T UINT16   ui2_current_hour;                  /* current time position */
    OUT_PARAMETER_T UINT8    ui1_current_minute;
    OUT_PARAMETER_T UINT8    ui1_current_second;
    OUT_PARAMETER_T UINT32   ui4_current_microsecond; 
    OUT_PARAMETER_T DLNA_DMR_PLAYER_RENDERING_CONTROL_T t_current_rendering; /* current rendering info */
         
    VOID * pv_data;                             /* reserve for future */ 
} DLNA_DMR_PLAYER_ACTION_PARAMETER_T;


/*add by kun.chen:NOTE THE CMD STATUS*/
typedef enum _DLNA_DMR_CMD_STATUS_T
{
	DLNA_DMR_CMD_STATUS_NONE,
	DLNA_DMR_CMD_STATUS_PLAY, /*send the play cmd to ap*/
	DLNA_DMR_CMD_STATUS_PLAYED,/*ap return played*/
	DLNA_DMR_CMD_STATUS_PAUSE, /*send the pause cmd to ap*/
	DLNA_DMR_CMD_STATUS_PAUSED, /*this status not in using current*/
	DLNA_DMR_CMD_STATUS_RESUME, /*send the play cmd to ap*/
	DLNA_DMR_CMD_STATUS_RESUMED, /*this status not in using current*/
	DLNA_DMR_CMD_STATUS_STOP,/*send the stop cmd to ap*/
	DLNA_DMR_CMD_STATUS_STOPPED,/*ap return stopped*/
	DLNA_DMR_CMD_STATUS_END_OF_MEDIA /*ap return end of media*/
}DLNA_DMR_CMD_STATUS_T;


/*------------------------------------------------------------------*/
/*! @brief  callback of player action
*  @param [in]t_event             - The player action event
*  @param [in]pv_tag              - The callback tag
*  @param [in]pv_param            - The callback parameter
*  @note
*  @see
*  @return
*  @retval      -
*/
/*------------------------------------------------------------------*/
typedef VOID (*x_dlna_dmr_player_nfy) (DLNA_DMR_PLAYER_ACTION_EVENT_T e_event, 
                                   VOID * pv_tag,
                                   DLNA_DMR_PLAYER_ACTION_PARAMETER_T * pt_param);

/*------------------------------------------------------------------*/
/*! @brief  DMR file handler, set by user
*  @param [in]ps_uri              - The media content URI
*  @param [in]pv_protocol         - The protocol info
*  @note
*  @see
*  @return
*  @retval      -
*/
/*------------------------------------------------------------------*/                                  
typedef CHAR* (*x_dlna_dmr_file_hdlr) (CHAR*ps_uri, CHAR*ps_protocol);

/* DLNA MIME type */
typedef DLNA_PROTOCOL_INFO_T DLNA_DMR_PROTOCOL_INFO_T;

#endif

/*! @} */

