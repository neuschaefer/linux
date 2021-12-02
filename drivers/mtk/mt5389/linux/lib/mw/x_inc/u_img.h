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
/*! @file u_img.h
 *  $RCSfile: $
 *  $Revision: #6 $
 *  $Date: 2012/08/28 $
 *  $Author: xiuqin.wang $
 *
 *  @par Description:
 *         This file exports all image library specific definitions and structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_IMG Image Lib
 *
 *  @ingroup groupMW
 *  @brief Provides constant,type definitions and function of the  Image Lib
 *   ..  Image Lib is a middleware component that is responsible for image decode
 *
 *  %MW Application API would be used by TV Application, lot of these APIs
 *  would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_IMG_H_
#define _U_IMG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_gl.h"
#include "u_gif.h"
#include "u_vid_plane.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifndef IMG_MORE_EXIF_INFO_SUPPORT
#define IMG_MORE_EXIF_INFO_SUPPORT    /**<define to support exif information */
#endif


//#define CAPTURE_SAVE_IN_USB


/*------------------------------------------------------------------*/
/*! @enum IMG_TYPE_T
 *  @brief type of image support
 *  @code
 *  typedef enum
 *  {
 *      IMG_UNKNOWN = 0,
 *      IMG_PNG,
 *      IMG_GIF,
 *      IMG_BMP,
 *      IMG_MNG,
 *      IMG_JPG
 *  } IMG_TYPE_T;
 *  @endcode
 *  @li@c  IMG_UNKNOWN                                      - unknown image type
 *  @li@c  IMG_PNG                                          - png type
 *  @li@c  IMG_GIF                                          - gif type
 *  @li@c  IMG_BMP                                          - bmp type
 *  @li@c  IMG_MNG                                          - mng type
 *  @li@c  IMG_JPG                                          - jpeg type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IMG_UNKNOWN = 0,
    IMG_PNG,
    IMG_GIF,
    IMG_BMP,
    IMG_MNG,
    IMG_JPG,
    IMG_WEBP,
    IMG_MPO
} IMG_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum IMG_NFY_EVENT_TYPE_T
 *  @brief type of event callback funtion
 *  @code
 *  typedef enum
 *  {
 *      IMG_NFY_HANDLE_CLOSED_TYPE,
 *      IMG_NFY_LOAD_COMPLETE_TYPE,
 *      IMG_NFY_LOAD_COMPLETE_WITH_TAG,
 *      IMG_NFY_GIF_ONE_FRM_LOAD_COMPLETE_TYPE,
 *      IMG_NTY_GIF_LOAD_STOP
 *  } IMG_NFY_EVENT_TYPE_T;
 *  @endcode
 *  @li@c  IMG_NFY_HANDLE_CLOSED_TYPE                 - handle close callback function
 *  @li@c  IMG_NFY_LOAD_COMPLETE_TYPE                 - load complete callback function
 *  @li@c  IMG_NFY_LOAD_COMPLETE_WITH_TAG             - load complete callback function with argvment
 *  @li@c  IMG_NFY_GIF_ONE_FRM_LOAD_COMPLETE_TYPE     - gif, load one frame complete callback function
 *  @li@c  IMG_NTY_GIF_LOAD_STOP                      - stop gif load callback function
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IMG_NFY_HANDLE_CLOSED_TYPE,
    IMG_NFY_LOAD_COMPLETE_TYPE,
    IMG_NFY_LOAD_COMPLETE_WITH_TAG,
    IMG_NFY_GIF_ONE_FRM_LOAD_COMPLETE_TYPE,
    IMG_NTY_GIF_LOAD_STOP
} IMG_NFY_EVENT_TYPE_T;

/*------------------------------------------------------------------*/
/*! @brief This function is the callback function of image library.
 *  @param [in] h_handle Specifies which handle will be notified.
 *  @param [in] pv_tag The client data specified by the client when callback has 
 *				been called, the callback function will send it again..
 *  @param [in] e_event it identifies the event type of notification.
 *  @param [in] pv_data specify the notification data which is followed with the 
 *				notification event.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_img_nfy_fct) ( HANDLE_T                h_handle,
                                const VOID*             pv_tag,
                                IMG_NFY_EVENT_TYPE_T    e_event,
                                const VOID*             pv_data);

/*------------------------------------------------------------------*/
/*! @enum IMG_INTRA_TYPE_T
 *  @brief This structure enumerates all intra-image types.
 *  @code
 *  typedef enum
 *  {
 *      INTRA_IMG_THUMBNAIL_TYPE,
 *      INTRA_IMG_PRIMARY_IMG_TYPE
 *  } IMG_INTRA_TYPE_T;
 *  @endcode
 *  @li@c  INTRA_IMG_THUMBNAIL_TYPE                          - thumbnail type
 *  @li@c  INTRA_IMG_PRIMARY_IMG_TYPE                        - primary type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    INTRA_IMG_THUMBNAIL_TYPE,
    INTRA_IMG_PRIMARY_IMG_TYPE
} IMG_INTRA_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum IMG_STOP_OP_TYPE_T
 *  @brief This structure enumerates all stop operation types.
 *  @code
 *  typedef enum
 *  {
 *      IMG_STOP_PRIMARY_OP_TYPE,
 *      IMG_STOP_THUMBNAIL_OP_TYPE
 *  } IMG_STOP_OP_TYPE_T;
 *  @endcode
 *  @li@c  IMG_STOP_PRIMARY_OP_TYPE                        - stop thumbnail type
 *  @li@c  IMG_STOP_THUMBNAIL_OP_TYPE                      - stop primary type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IMG_STOP_PRIMARY_OP_TYPE,
    IMG_STOP_THUMBNAIL_OP_TYPE
} IMG_STOP_OP_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum IMG_ROTATE_OP_TYPE_T
 *  @brief This structure enumerates all rotation operation types when the image 
 *			is loading.
 *  @code
 *  typedef enum
 *  {
 *      IMG_ROTATE_CLOCKWS_0_OP_TYPE             = 0,
 *      IMG_ROTATE_CLOCKWS_90_OP_TYPE            = 1,
 *      IMG_ROTATE_CLOCKWS_180_OP_TYPE           = 2,
 *      IMG_ROTATE_CLOCKWS_270_OP_TYPE           = 3,
 *      IMG_ROTATE_CLOCKWS_0_OP_TYPE_WITH_FLIP   = 4,
 *      IMG_ROTATE_CLOCKWS_90_OP_TYPE_WITH_FLIP  = 5,
 *      IMG_ROTATE_CLOCKWS_180_OP_TYPE_WITH_FLIP = 6,
 *      IMG_ROTATE_CLOCKWS_270_OP_TYPE_WITH_FLIP = 7
 *  } IMG_ROTATE_OP_TYPE_T;
 *  @endcode
 *  @li@c  IMG_ROTATE_CLOCKWS_0_OP_TYPE                  - ratate 0 degree option
 *  @li@c  IMG_ROTATE_CLOCKWS_90_OP_TYPE                 - ratate 90 degree option
 *  @li@c  IMG_ROTATE_CLOCKWS_180_OP_TYPE                -ratate 180 degree option
 *  @li@c  IMG_ROTATE_CLOCKWS_270_OP_TYPE                -ratate 270 degree option
 *  @li@c  IMG_ROTATE_CLOCKWS_0_OP_TYPE_WITH_FLIP        - ratate 0 degree option with flip
 *  @li@c  IMG_ROTATE_CLOCKWS_90_OP_TYPE_WITH_FLIP       -ratate 90 degree option with flip
 *  @li@c  IMG_ROTATE_CLOCKWS_180_OP_TYPE_WITH_FLIP      -ratate 180 degree option with flip
 *  @li@c  IMG_ROTATE_CLOCKWS_270_OP_TYPE_WITH_FLIP      -ratate 270 degree option with flip
 */
/*------------------------------------------------------------------*/
typedef enum
{
#if 0
		IMG_ROTATE_CLOCKWS_0_OP_TYPE			 = 0,
		IMG_ROTATE_CLOCKWS_90_OP_TYPE			 = 1,
		IMG_ROTATE_CLOCKWS_180_OP_TYPE			 = 2,
		IMG_ROTATE_CLOCKWS_270_OP_TYPE			 = 3,
		IMG_ROTATE_CLOCKWS_0_OP_TYPE_WITH_FLIP	 = 4,
		IMG_ROTATE_CLOCKWS_90_OP_TYPE_WITH_FLIP  = 5,
		IMG_ROTATE_CLOCKWS_180_OP_TYPE_WITH_FLIP = 6,
		IMG_ROTATE_CLOCKWS_270_OP_TYPE_WITH_FLIP = 7
#endif
		IMG_ROTATE_CLOCKWS_0_OP_TYPE			 = 0,
		IMG_ROTATE_CLOCKWS_0_OP_TYPE_WITH_FLIP	 = 1,
		IMG_ROTATE_CLOCKWS_180_OP_TYPE			 = 2,
		IMG_ROTATE_CLOCKWS_180_OP_TYPE_WITH_FLIP = 3,
		IMG_ROTATE_CLOCKWS_270_OP_TYPE_WITH_FLIP = 4,
		IMG_ROTATE_CLOCKWS_90_OP_TYPE			 = 5,
		IMG_ROTATE_CLOCKWS_90_OP_TYPE_WITH_FLIP  = 6,
		IMG_ROTATE_CLOCKWS_270_OP_TYPE			 = 7
} IMG_ROTATE_OP_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum IMG_LOAD_STATUS_T
 *  @brief This structure enumerates all loading status of the decoder
 *  @code
 *  typedef enum
 *  {
 *      IMG_STAT_INITIAL,
 *      IMG_STAT_LOADING,
 *      IMG_STAT_LOADED,
 *      IMG_STAT_LOAD_ABORT
 *  } IMG_LOAD_STATUS_T;
 *  @endcode
 *  @li@c  IMG_STAT_INITIAL                                  - image load init
 *  @li@c  IMG_STAT_LOADING                                  - image loading
 *  @li@c  IMG_STAT_LOADED                                   - image loaded
 *  @li@c  IMG_STAT_LOAD_ABORT                               - image load abort
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IMG_STAT_INITIAL,
    IMG_STAT_LOADING,
    IMG_STAT_LOADED,
    IMG_STAT_LOAD_ABORT
} IMG_LOAD_STATUS_T;

/*------------------------------------------------------------------*/
/*! @enum IMG_INFO_TYPE_T
 *  @brief   This structure enumerates all information types retrieved by the 
 *			function x_img_get_info().
 *  @code
 *  typedef enum
 *  {
 *      IMG_INFO_PRIMARY_DIMENSION,
 *      IMG_INFO_THUMBNAIL_DIMENSION,
 *      IMG_INFO_PRIMARY_LOAD_PROGRESS,
 *      IMG_INFO_THUMBNAIL_LOAD_PROGRESS,
 *      IMG_INFO_PRIMARY_READY,
 *      IMG_INFO_THUMBNAIL_READY,
 *      IMG_INFO_PRIMARY_TYPE,
 *      IMG_INFO_THUMBNAIL_TYPE,
 *      IMG_INFO_PRIMARY_EXIF_LEN,
 *      IMG_INFO_THUMBNAIL_EXIF_LEN,
 *      IMG_INFO_PRIMARY_EXIF,
 *      IMG_INFO_THUMBNAIL_EXIF,
 *      IMG_INFO_IMG_TYPE,
 *      IMG_INFO_PRIMARY_MEET_ALL_LIMITATION,
 *      IMG_INFO_THUMBNAIL_MEET_ALL_LIMITATION,
 *      IMG_INFO_PLAY_BACK_SUPPORT,
 *      IMG_INFO_END
 *  } IMG_INFO_TYPE_T;
 *  @endcode
 *  @li@c  IMG_INFO_PRIMARY_DIMENSION                - dimension of primary
 *  @li@c  IMG_INFO_THUMBNAIL_DIMENSION              - dimension of thumbnail
 *  @li@c  IMG_INFO_PRIMARY_LOAD_PROGRESS            - primary load progress
 *  @li@c  IMG_INFO_THUMBNAIL_LOAD_PROGRESS          - thumbnail load progress
 *  @li@c  IMG_INFO_PRIMARY_READY                    - primary loaded
 *  @li@c  IMG_INFO_THUMBNAIL_READY                  - thumbnail loaded
 *  @li@c  IMG_INFO_PRIMARY_TYPE                     - information for primary
 *  @li@c  IMG_INFO_THUMBNAIL_TYPE                   - information for thumbnail
 *  @li@c  IMG_INFO_PRIMARY_EXIF_LEN                 - primary exif information length
 *  @li@c  IMG_INFO_THUMBNAIL_EXIF_LEN               - thumbnail  exif information length
 *  @li@c  IMG_INFO_PRIMARY_EXIF                     - primary exif information
 *  @li@c  IMG_INFO_THUMBNAIL_EXIF                   - thumbnail  exif information
 *  @li@c  IMG_INFO_IMG_TYPE                         - image type
 *  @li@c  IMG_INFO_PRIMARY_MEET_ALL_LIMITATION      - primary limitation
 *  @li@c  IMG_INFO_THUMBNAIL_MEET_ALL_LIMITATION    - thumbnail limitation
 *  @li@c  IMG_INFO_PLAY_BACK_SUPPORT                - playback support
 *  @li@c  IMG_INFO_END                              - not use
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IMG_INFO_PRIMARY_DIMENSION,
    IMG_INFO_THUMBNAIL_DIMENSION,
    IMG_INFO_PRIMARY_LOAD_PROGRESS,
    IMG_INFO_THUMBNAIL_LOAD_PROGRESS,
    IMG_INFO_PRIMARY_READY,
    IMG_INFO_THUMBNAIL_READY,
    IMG_INFO_PRIMARY_TYPE,
    IMG_INFO_THUMBNAIL_TYPE,

    IMG_INFO_PRIMARY_EXIF_LEN,
    IMG_INFO_THUMBNAIL_EXIF_LEN,
    IMG_INFO_PRIMARY_EXIF,
    IMG_INFO_THUMBNAIL_EXIF,

    IMG_INFO_IMG_TYPE,

    IMG_INFO_PRIMARY_MEET_ALL_LIMITATION,
    IMG_INFO_THUMBNAIL_MEET_ALL_LIMITATION,
    IMG_INFO_PLAY_BACK_SUPPORT,
    IMG_INFO_END
} IMG_INFO_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum IMG_JPG_TYPE_T
 *  @brief This structure enumerates jpeg decode mode
 *  @code
 *  typedef enum
 *  {
 *      JPG_TYPE_BASELINE,
 *      JPG_TYPE_PROGRESSIVE
 *  } IMG_JPG_TYPE_T;
 *  @endcode
 *  @li@c  JPG_TYPE_BASELINE                         - baseline decode mode
 *  @li@c  JPG_TYPE_PROGRESSIVE                      - progressive decode mode
 */
/*------------------------------------------------------------------*/
typedef enum
{
    JPG_TYPE_BASELINE,
    JPG_TYPE_PROGRESSIVE
} IMG_JPG_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum IMG_SRC_TYPE_T
 *  @brief This structure enumerates all image source types.
 *  @code
 *  typedef enum
 *  {
 *      IMG_FILE_SRC_TYPE,
 *      IMG_MEMORY_SRC_TYPE,
 *      IMG_DLNA_SRC_TYPE
 *  } IMG_SRC_TYPE_T;
 *  @endcode
 *  @li@c  IMG_FILE_SRC_TYPE                                 - image from file
 *  @li@c  IMG_MEMORY_SRC_TYPE                               - image from memory
 *  @li@c  IMG_DLNA_SRC_TYPE                                 - image from dlna
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IMG_FILE_SRC_TYPE,
    IMG_MEMORY_SRC_TYPE,
    IMG_DLNA_SRC_TYPE
} IMG_SRC_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum EXIF_ID_T
 *  @brief This structure enumerates exif information type.
 *  @code
 *  typedef enum
 *  {
 *      EXIF_ARTIST             =  0,
 *      EXIF_DATE_TIME          =  1,
 *      EXIF_MAKE               =  2,
 *      EXIF_MODEL              =  3,
 *      EXIF_DESCRIPTION        =  4,
 *      EXIF_DATE_TIME_ORIGINAL =  5,
 *      EXIF_ORIENTATION        =  6,
 *      EXIF_EXPOSURE_TIME      =  7,
 *      EXIF_F_NUMBER           =  8,
 *      EXIF_EXPOSURE_PROGRAM   =  9,
 *      EXIF_COLOR_SPACE        = 10,
 *  #ifdef IMG_MORE_EXIF_INFO_SUPPORT
 *      EXIF_GPS_ASSOCIATED     = 11,
 *      EXIF_FOCAL_LENGTH       = 12,
 *      EXIF_EXPOSURE_BIAS      = 13,
 *      EXIF_ISO_SENSITIVITY    = 14,
 *      EXIF_FLASH_MODE         = 15,
 *      EXIF_FOCAL_LENGTH_IN_35MM_FILM = 16,
 *  #endif
 *      EXIF_ID_RESERVED
 *  } EXIF_ID_T;
 *  @endcode
 *  @li@c  EXIF_ARTIST                                   - artist
 *  @li@c  EXIF_DATE_TIME                                - data and time
 *  @li@c  EXIF_MAKE                                     - make
 *  @li@c  EXIF_MODEL                                    - model
 *  @li@c  EXIF_DESCRIPTION                              - descrption
 *  @li@c  EXIF_DATE_TIME_ORIGINAL                       - data and time original
 *  @li@c  EXIF_ORIENTATION                              - orientation
 *  @li@c  EXIF_EXPOSURE_TIME                            - exposure time
 *  @li@c  EXIF_F_NUMBER                                 - f number
 *  @li@c  EXIF_EXPOSURE_PROGRAM                         - exposure program
 *  @li@c  EXIF_COLOR_SPACE                              - color space
 *  @li@c  EXIF_GPS_ASSOCIATED                           - gps associated
 *  @li@c  EXIF_FOCAL_LENGTH                             - focal length
 *  @li@c  EXIF_EXPOSURE_BIAS                            - exposure bias
 *  @li@c  EXIF_ISO_SENSITIVITY                          - ISO sensitivity
 *  @li@c  EXIF_FLASH_MODE                               - flash mode
 *  @li@c  EXIF_FOCAL_LENGTH_IN_35MM_FILM                - focal length in 35mm film
 *  @li@c  EXIF_ID_RESERVED                              - reserved
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EXIF_ARTIST             =  0,       /* pv_data type := (CHAR *)             */
    EXIF_DATE_TIME          =  1,       /* pv_data type := (CHAR *)             */
    EXIF_MAKE               =  2,       /* pv_data type := (CHAR *)             */
    EXIF_MODEL              =  3,       /* pv_data type := (CHAR *)             */
    EXIF_DESCRIPTION        =  4,       /* pv_data type := (CHAR *)             */
    EXIF_DATE_TIME_ORIGINAL =  5,       /* pv_data type := (CHAR *)             */
    EXIF_ORIENTATION        =  6,       /* pv_data type := IMG_ROTATE_OP_TYPE_T */
    EXIF_EXPOSURE_TIME      =  7,       /* pv_data type := EXIF_RATIONAL_DATA   */
    EXIF_F_NUMBER           =  8,       /* pv_data type := EXIF_RATIONAL_DATA   */
    EXIF_EXPOSURE_PROGRAM   =  9,       /* pv_data type := EXIF_CONTEXT_DATA    */
    EXIF_COLOR_SPACE        = 10,       /* pv_data type := EXIF_CONTEXT_DATA    */

#ifdef IMG_MORE_EXIF_INFO_SUPPORT
    EXIF_GPS_ASSOCIATED     = 11,       /* pv_data type := (BOOL)               */
    EXIF_FOCAL_LENGTH       = 12,       /* pv_data type := EXIF_RATIONAL_DATA   */
    EXIF_EXPOSURE_BIAS      = 13,       /* pv_data type := EXIF_SRATIONAL_DATA  */
    EXIF_ISO_SENSITIVITY    = 14,       /* pv_data type := UINT16               */
    EXIF_FLASH_MODE         = 15,       /* pv_data type := EXIF_FLASH_MODE_TYPE_T */
    EXIF_FOCAL_LENGTH_IN_35MM_FILM = 16,/* pv_data_type := UINT16 */
    EXIF_SHUTTER_SPEED 		= 17,		/* pv_data type := EXIF_SRATIONAL_DATA  */
#endif
    EXIF_ID_RESERVED
} EXIF_ID_T;

/*------------------------------------------------------------------*/
/*! @enum EXIF_STATE_T
 *  @brief This structure enumerates availabel of exif information
 *  @code
 *  typedef enum
 *  {
 *      EXIF_AVAILABLE,
 *      EXIF_NOT_AVAILABLE,
 *      EXIF_NOT_SUPPORT
 *  } EXIF_STATE_T;
 *  @endcode
 *  @li@c  EXIF_AVAILABLE                                - exif is available
 *  @li@c  EXIF_NOT_AVAILABLE                            - exif is not available
 *  @li@c  EXIF_NOT_SUPPORT                              - exif not support
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EXIF_AVAILABLE,
    EXIF_NOT_AVAILABLE,
    EXIF_NOT_SUPPORT
} EXIF_STATE_T;

/*------------------------------------------------------------------*/
/*! @enum EXIF_COLOR_SPACE_CTX
 *  @brief This structure enumerates type of color space
 *  @code
 *  typedef enum
 *  {
 *      EXIF_COLOR_SPACE_RESERVED       = 0,
 *      EXIF_COLOR_SPACE_SRGB           = 1,
 *      EXIF_COLOR_SPACE_UNCALIBRATED   = 0xFFFF
 *  } EXIF_COLOR_SPACE_CTX;
 *  @endcode
 *  @li@c  EXIF_COLOR_SPACE_RESERVED                     - color space reserved
 *  @li@c  EXIF_COLOR_SPACE_SRGB                         - SRGB
 *  @li@c  EXIF_COLOR_SPACE_UNCALIBRATED                 - uncalibrated
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EXIF_COLOR_SPACE_RESERVED       = 0,
    EXIF_COLOR_SPACE_SRGB           = 1,
    EXIF_COLOR_SPACE_UNCALIBRATED   = 0xFFFF
} EXIF_COLOR_SPACE_CTX;

/*------------------------------------------------------------------*/
/*! @enum EXIF_EXPOSURE_PROGRAM_CTX
 *  @brief This structure enumerates type pf exposure program
 *  @code
 *  typedef enum
 *  {
 *      EXIF_EXPOSURE_PROGRAM_NOT_DEFINED       = 0,
 *      EXIF_EXPOSURE_PROGRAM_MANUAL            = 1,
 *      EXIF_EXPOSURE_PROGRAM_NORMAL_PROGRAM    = 2,
 *      EXIF_EXPOSURE_PROGRAM_APERTURE_PRIORITY = 3,
 *      EXIF_EXPOSURE_PROGRAM_SHUTTER_PRIORITY  = 4,
 *      EXIF_EXPOSURE_PROGRAM_CREATIVE_PROGRAM  = 5,
 *      EXIF_EXPOSURE_PROGRAM_ACTION_PROGRAM    = 6,
 *      EXIF_EXPOSURE_PROGRAM_PORTRAIT_MODE     = 7,
 *      EXIF_EXPOSURE_PROGRAM_LANDSCAPE_MODE    = 8,
 *      EXIF_EXPOSURE_PROGRAM_RESERVED
 *  } EXIF_EXPOSURE_PROGRAM_CTX;
 *  @endcode
 *  @li@c  EXIF_EXPOSURE_PROGRAM_NOT_DEFINED                 -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_MANUAL                      -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_NORMAL_PROGRAM              -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_APERTURE_PRIORITY           -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_SHUTTER_PRIORITY            -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_CREATIVE_PROGRAM            -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_ACTION_PROGRAM              -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_PORTRAIT_MODE               -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_LANDSCAPE_MODE              -
 *  @li@c  EXIF_EXPOSURE_PROGRAM_RESERVED                    -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EXIF_EXPOSURE_PROGRAM_NOT_DEFINED       = 0,
    EXIF_EXPOSURE_PROGRAM_MANUAL            = 1,
    EXIF_EXPOSURE_PROGRAM_NORMAL_PROGRAM    = 2,
    EXIF_EXPOSURE_PROGRAM_APERTURE_PRIORITY = 3,
    EXIF_EXPOSURE_PROGRAM_SHUTTER_PRIORITY  = 4,
    EXIF_EXPOSURE_PROGRAM_CREATIVE_PROGRAM  = 5,
    EXIF_EXPOSURE_PROGRAM_ACTION_PROGRAM    = 6,
    EXIF_EXPOSURE_PROGRAM_PORTRAIT_MODE     = 7,
    EXIF_EXPOSURE_PROGRAM_LANDSCAPE_MODE    = 8,

    EXIF_EXPOSURE_PROGRAM_RESERVED
} EXIF_EXPOSURE_PROGRAM_CTX;

/*------------------------------------------------------------------*/
/*! @struct EXIF_RATIONAL_DATA
 *  @brief This structure enumerates rational data of exif
 *  @code
 *  typedef struct
 *  {
 *      UINT32      ui4_numerator;
 *      UINT32      ui4_denominator;
 *  } EXIF_RATIONAL_DATA;
 *  @endcode
 *  @li@c  ui4_numerator                             - number of rational data
 *  @li@c  ui4_denominator                           - denominator
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32      ui4_numerator;
    UINT32      ui4_denominator;
} EXIF_RATIONAL_DATA;

/*------------------------------------------------------------------*/
/*! @struct EXIF_SRATIONAL_DATA
 *  @brief This structure enumerates the srational data of exif
 *  @code
 *  typedef struct
 *  {
 *      INT32       i4_numerator;
 *      INT32       i4_denominator;
 *  } EXIF_SRATIONAL_DATA;
 *  @endcode
 *  @li@c  i4_numerator                              - number of srational data
 *  @li@c  i4_denominator                            - denominator
 */
/*------------------------------------------------------------------*/
typedef struct
{
    INT32       i4_numerator;
    INT32       i4_denominator;
} EXIF_SRATIONAL_DATA;

/*------------------------------------------------------------------*/
/*! @enum EXIF_FLASH_MODE_TYPE_T
 *  @brief This structure enumerates type pf flash
 *  @code
 *  typedef enum
 *  {
 *      EXIF_FLASH_ON               = 0,
 *      EXIF_FLASH_OFF              = 1,
 *      EXIF_FLASH_FORCE_ON         = 2,
 *      EXIF_FLASH_FORCE_OFF        = 3,
 *      EXIF_FLASH_AUTO_ON          = 4,
 *      EXIF_FLASH_AUTO_OFF         = 5,
 *      EXIF_FLASH_RESERVED
 *  } EXIF_FLASH_MODE_TYPE_T;
 *  @endcode
 *  @li@c  EXIF_FLASH_ON                                     - on
 *  @li@c  EXIF_FLASH_OFF                                    - off
 *  @li@c  EXIF_FLASH_FORCE_ON                               - force on
 *  @li@c  EXIF_FLASH_FORCE_OFF                              - force off
 *  @li@c  EXIF_FLASH_AUTO_ON                                - auto on
 *  @li@c  EXIF_FLASH_AUTO_OFF                               - auto off
 *  @li@c  EXIF_FLASH_RESERVED                               - reserved
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EXIF_FLASH_ON               = 0,
    EXIF_FLASH_OFF              = 1,
    EXIF_FLASH_FORCE_ON         = 2,
    EXIF_FLASH_FORCE_OFF        = 3,
    EXIF_FLASH_AUTO_ON          = 4,
    EXIF_FLASH_AUTO_OFF         = 5,

    EXIF_FLASH_RESERVED
} EXIF_FLASH_MODE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct EXIF_INFO_T
 *  @brief This structure enumerates exif information
 *  @code
 *  typedef struct
 *  {
 *      EXIF_ID_T       e_exif_id;
 *      EXIF_STATE_T    e_property;
 *      SIZE_T          z_size;
 *      VOID            *pv_data;
 *  } EXIF_INFO_T;
 *  @endcode
 *  @li@c  e_exif_id                                         - EXIF type
 *  @li@c  e_property                                        - property
 *  @li@c  z_size                                            - data size
 *  @li@c  *pv_data                                          - EXIF data
 */
/*------------------------------------------------------------------*/
typedef struct
{
    EXIF_ID_T       e_exif_id;      /* [IN]     EXIF type */
    EXIF_STATE_T    e_property;     /* [OUT]    property  */
    SIZE_T          z_size;         /* [IN/OUT] data size */
    VOID            *pv_data;       /* [IN]     EXIF data */
} EXIF_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct EXIF_CONTEXT_DATA
 *  @brief This structure enumerates exif context information
 *  @code
 *  typedef struct
 *  {
 *      UINT16                          ui2_value;
 *      union {
 *          EXIF_COLOR_SPACE_CTX        e_color_space;
 *          EXIF_EXPOSURE_PROGRAM_CTX   e_exposure_program;
 *      } u;
 *  } EXIF_CONTEXT_DATA;
 *  @endcode
 *  @li@c  ui2_value                                         - value
 *  @li@c  e_color_space                                     - color space
 *  @li@c  e_exposure_program                                - exposure program
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16                          ui2_value;

    union {
        EXIF_COLOR_SPACE_CTX        e_color_space;
        EXIF_EXPOSURE_PROGRAM_CTX   e_exposure_program;
    } u;
} EXIF_CONTEXT_DATA;

/*------------------------------------------------------------------*/
/*! @struct IMG_ENG_DATA_T
 *  @brief  This structure enumerates imahe engine information
 *  @code
 *  typedef struct
 *  {
 *      IMG_SRC_TYPE_T      e_src_type;
 *      VOID                *pv_data;
 *      VOID                *pv_data2;
 *      SIZE_T              z_mem_len;
 *      VOID                *pv_type_data;
 *  } IMG_ENG_DATA_T;
 *  @endcode
 *  @li@c  e_src_type                                - Image source type
 *  @li@c  *pv_data                                  - file name
 *  @li@c  *pv_data2                                 -  image data
 *  @li@c  z_mem_len                                 - length of filename or image data
 *  @li@c  *pv_type_data                             - type of image
 */
/*------------------------------------------------------------------*/
typedef struct
{
    IMG_SRC_TYPE_T      e_src_type;
    VOID                *pv_data;
    VOID                *pv_data2;
    SIZE_T              z_mem_len;
    VOID                *pv_type_data;
} IMG_ENG_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct IMG_ANIMATE_T
 *  @brief This structure enumerates information for gif animation
 *  @code
 *  typedef struct
 *  {
 *      BOOL              b_animated;
 *      UINT32            ui4_loop_cnt;
 *      UINT32              ui4_frame_index;
 *      GL_HSURFACE_T     h_surface_temp;
 *  }IMG_ANIMATE_T;
 *  @endcode
 *  @li@c  b_animated                                - if gif support animation
 *  @li@c  ui4_loop_cnt                              - loop count
 *  @li@c  ui4_frame_index                           - start frame index
 *  @li@c  h_surface_temp                            - temp surface for decode
 */
/*------------------------------------------------------------------*/
typedef struct
{
    BOOL              b_animated;
    UINT32            ui4_loop_cnt;
    UINT32              ui4_frame_index;
    GL_HSURFACE_T     h_surface_temp;
}IMG_ANIMATE_T;

/*------------------------------------------------------------------*/
/*! @struct IMG_TYPE_DATA_T
 *  @brief This structure enumerates information for image
 *  @code
 *  typedef struct
 *  {
 *      UINT32              ui4_data_flag;
 *      VOID                *pv_tag;
 *      IMG_ENG_DATA_T      t_eng_data;
 *      GL_HPALETTE_T       h_palette;
 *      IMG_ANIMATE_T       t_animated;
 *  } IMG_TYPE_DATA_T;
 *  @endcode
 *  @li@c  ui4_data_flag                                 - data flag
 *  @li@c  *pv_tag                                       - data tag
 *  @li@c  t_eng_data                                    - image engine data
 *  @li@c  h_palette                                     - palette handle
 *  @li@c  t_animated                                    - animation information
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32              ui4_data_flag;
    VOID                *pv_tag;
    IMG_ENG_DATA_T      t_eng_data;
    GL_HPALETTE_T       h_palette;
    IMG_ANIMATE_T       t_animated;
    BOOL				b_load_more_frame;
	GL_HSURFACE_T		h_animation_surf;

	UINT32				buffer;
	UINT32				ui4_width;
	UINT32				ui4_height;
	UINT32				ui4_pitch;
	UINT32				shm_id;
} IMG_TYPE_DATA_T;


/*------------------------------------------------------------------*/
/*! @struct IMG_ENG_TYPE_DATA_T
 *  @brief This structure enumerates information for image engine data
 *  @code
 *  typedef struct
 *  {
 *      GL_HPALETTE_T       h_palette;
 *  } IMG_ENG_TYPE_DATA_T;
 *  @endcode
 *  @li@c  h_palette                                         - palette handle
 */
/*------------------------------------------------------------------*/
typedef struct
{
    GL_HPALETTE_T       h_palette;
} IMG_ENG_TYPE_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct IMG_CAPS_T
 *  @brief This structure enumerates of image capability
 *  @code
 *  typedef struct
 *  {
 *      UINT32          ui4_type;
 *      union {
 *          BOOL        b_progressive_support;
 *          struct {
 *              UINT32  ui4_max_width;
 *              UINT32  ui4_min_width;
 *              UINT32  ui4_max_height;
 *              UINT32  ui4_min_height;
 *          } t_resolution;
 *          struct {
 *              UINT32  ui4_max_area;
 *              UINT32  ui4_min_width;
 *              UINT32  ui4_min_height;
 *          } t_pgsv_resolution;
 *          BOOL        b_full_mode_support;
 *      } u;
 *  } IMG_CAPS_T;
 *  @endcode
 *  @li@c  ui4_type                              - image type
 *  @li@c  b_progressive_support                 - if progressive supported
 *  @li@c  ui4_max_width                         - max width (IMG_CAP_RESOLUTION)
 *  @li@c  ui4_min_width                         - min width (IMG_CAP_RESOLUTION)
 *  @li@c  ui4_max_height                        - max height (IMG_CAP_RESOLUTION)
 *  @li@c  ui4_min_height                        - min height (IMG_CAP_RESOLUTION)
 *  @li@c  ui4_max_area                          - max area (IMG_CAP_PGSV_RESOLUTION)
 *  @li@c  ui4_min_width                         - min width (IMG_CAP_PGSV_RESOLUTION)
 *  @li@c  ui4_min_height                        - min height (IMG_CAP_PGSV_RESOLUTION)
 *  @li@c  b_full_mode_support                   - if full mode support
 */
/*------------------------------------------------------------------*/
typedef struct
{
    /* INPUT */
    UINT32          ui4_type;

    /* OUTPUT */
    union {
        /* used for IMG_CAP_PROGRESSIVE */
        BOOL        b_progressive_support;

        /* used for IMG_CAP_RESOLUTION(HW & SW) */
        struct {
            UINT32  ui4_max_width;
            UINT32  ui4_min_width;
            UINT32  ui4_max_height;
            UINT32  ui4_min_height;
            UINT32  ui4_max_area;
        } t_resolution;

        /* used for IMG_CAP_PGSV_RESOLUTION */
        struct {
            UINT32  ui4_max_area;
            UINT32  ui4_min_width;
            UINT32  ui4_min_height;
        } t_pgsv_resolution;

        /* used for IMG_HW_JPG_CAP_FULL_MODE */
        BOOL        b_full_mode_support;
    } u;
} IMG_CAPS_T;

/*------------------------------------------------------------------*/
/*! @struct IMG_NFY_LOAD_WITH_TAG_T
 *  @brief  This structure enumerates notify information
 *  @code
 *  typedef struct
 *  {
 *      INT32               i4_ret;
 *      VOID                *pv_tag;
 *  } IMG_NFY_LOAD_WITH_TAG_T;
 *  @endcode
 *  @li@c  i4_ret                                            - return value
 *  @li@c  *pv_tag                                           - tag information
 */
/*------------------------------------------------------------------*/
typedef struct
{
    INT32               i4_ret;
    VOID                *pv_tag;
} IMG_NFY_LOAD_WITH_TAG_T;

/*------------------------------------------------------------------*/
/*! @struct IMG_NFY_GIF_LOAD_PARAM_T
 *  @brief This structure enumerates load complete information for gif
 *  @code
 *  typedef struct
 *  {
 *      INT32               i4_ret;
 *      UINT32              ui4_frm_idx;
 *      UINT32              ui4_loop_cnt;
 *  } IMG_NFY_GIF_LOAD_PARAM_T;
 *  @endcode
 *  @li@c  i4_ret                                            - return value
 *  @li@c  ui4_frm_idx                                       - current frame
 *  @li@c  ui4_loop_cnt                                      - current loop
 */
/*------------------------------------------------------------------*/
typedef struct
{
    INT32               i4_ret;
    UINT32              ui4_frm_idx;
    UINT32              ui4_loop_cnt;
    VOID                *pv_tag;
} IMG_NFY_GIF_LOAD_PARAM_T;

#if 0
/* GIF playback commands */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum _IMG_GIF_PLAYBACK_CMD_TYPE_T
 *  {
 *  	IMG_GIF_PLAYBACK_CMD_OPEN   = 0,
 *      IMG_GIF_PLAYBACK_CMD_PLAY   = 1,
 *      IMG_GIF_PLAYBACK_CMD_CLOSE  = 2,
 *      IMG_GIF_PLAYBACK_CMD_STOP   = 3
 *  } IMG_GIF_PLAYBACK_CMD_TYPE_T ;
 *  @endcode
 *  @li@c  IMG_GIF_PLAYBACK_CMD_OPEN                         -
 *  @li@c  IMG_GIF_PLAYBACK_CMD_PLAY                         -
 *  @li@c  IMG_GIF_PLAYBACK_CMD_CLOSE                        -
 *  @li@c  IMG_GIF_PLAYBACK_CMD_STOP                         -
 */
/*------------------------------------------------------------------*/
typedef enum _IMG_GIF_PLAYBACK_CMD_TYPE_T
{
	IMG_GIF_PLAYBACK_CMD_OPEN   = 0,
    IMG_GIF_PLAYBACK_CMD_PLAY   = 1,
    IMG_GIF_PLAYBACK_CMD_CLOSE  = 2,
    IMG_GIF_PLAYBACK_CMD_STOP   = 3
} IMG_GIF_PLAYBACK_CMD_TYPE_T ;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      HANDLE_T       h_load_sema;
 *      INT32          i4_ret;
 *  } IMG_GIF_PLAYBACK_TAG_T;
 *  @endcode
 *  @li@c  h_load_sema                                       -
 *  @li@c  i4_ret                                            -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    HANDLE_T       h_load_sema;
    INT32          i4_ret;
} IMG_GIF_PLAYBACK_TAG_T;

/* The maximum time(ms) can be standed when async load a frame */
#define MAX_LOAD_TIME_FOR_A_FRAME  500    /**<        */

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _IMG_GIF_PLAYBACK_CMD_OPEN_DATA_T
 *  {
 *      IMG_SRC_TYPE_T        e_type;
 *      const VOID*           pv_src;
 *      SIZE_T                z_len;
 *      x_img_nfy_fct         pf_nfy;
 *      VOID*                 pv_tag;
 *      HANDLE_T              h_img;
 *      UINT32                ui4_width;
 *      UINT32                ui4_height;
 *  } IMG_GIF_PLAYBACK_CMD_OPEN_DATA_T;
 *  @endcode
 *  @li@c  e_type                                            -
 *  @li@c  pv_src                                            -
 *  @li@c  z_len                                             -
 *  @li@c  pf_nfy                                            -
 *  @li@c  pv_tag                                            -
 *  @li@c  h_img                                             -
 *  @li@c  ui4_width                                         -
 *  @li@c  ui4_height                                        -
 */
/*------------------------------------------------------------------*/
typedef struct _IMG_GIF_PLAYBACK_CMD_OPEN_DATA_T
{
    /* Input */
    IMG_SRC_TYPE_T        e_type;  /* Load from file or memory */
    const VOID*           pv_src;  /* File path or memory address */
    SIZE_T                z_len;   /* Size of memory data */
    x_img_nfy_fct         pf_nfy;  /* callback */
    VOID*                 pv_tag;  /* tag returned with callback */

    /* Output */
    HANDLE_T              h_img;  /* IMG handle returned by IMG library */
    UINT32                ui4_width;
    UINT32                ui4_height;
} IMG_GIF_PLAYBACK_CMD_OPEN_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _IMG_GIF_PLAYBACK_CMD_PLAY_DATA_T
 *  {
 *      HANDLE_T              h_img;
 *      BOOL                  b_animated;
 *      BOOL                  b_async;
 *      IMG_ROTATE_OP_TYPE_T  e_rotate_type;
 *      GL_HSURFACE_T         h_dest_surf;
 *      UINT32                ui4_src_x;
 *      UINT32                ui4_src_y;
 *      UINT32                ui4_src_w;
 *      UINT32                ui4_src_h;
 *      UINT32                ui4_dst_x;
 *      UINT32                ui4_dst_y;
 *      UINT32                ui4_dst_w;
 *      UINT32                ui4_dst_h;
 *      UINT32                ui4_loop_cnt;
 *  } IMG_GIF_PLAYBACK_CMD_PLAY_DATA_T;
 *  @endcode
 *  @li@c  h_img                                             -
 *  @li@c  b_animated                                        -
 *  @li@c  b_async                                           -
 *  @li@c  e_rotate_type                                     -
 *  @li@c  h_dest_surf                                       -
 *  @li@c  ui4_src_x                                         -
 *  @li@c  ui4_src_y                                         -
 *  @li@c  ui4_src_w                                         -
 *  @li@c  ui4_src_h                                         -
 *  @li@c  ui4_dst_x                                         -
 *  @li@c  ui4_dst_y                                         -
 *  @li@c  ui4_dst_w                                         -
 *  @li@c  ui4_dst_h                                         -
 *  @li@c  ui4_loop_cnt                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _IMG_GIF_PLAYBACK_CMD_PLAY_DATA_T
{
    HANDLE_T              h_img;
    BOOL                  b_animated;    /* If FALSE, only load the first frame */
    BOOL                  b_async;       /* If FALSE, use sync play method */
    IMG_ROTATE_OP_TYPE_T  e_rotate_type;
    GL_HSURFACE_T         h_dest_surf;   /* The dest surface, in where will show the GIF */
    UINT32                ui4_src_x;
    UINT32                ui4_src_y;
    UINT32                ui4_src_w;
    UINT32                ui4_src_h;
    UINT32                ui4_dst_x;
    UINT32                ui4_dst_y;
    UINT32                ui4_dst_w;
    UINT32                ui4_dst_h;
    UINT32                ui4_loop_cnt; /* Display Loop Count */
} IMG_GIF_PLAYBACK_CMD_PLAY_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _IMG_GIF_PLAYBACK_CMD_STOP_DATA_T
 *  {
 *      HANDLE_T              h_img;
 *  } IMG_GIF_PLAYBACK_CMD_STOP_DATA_T;
 *  @endcode
 *  @li@c  h_img                                             -
 */
/*------------------------------------------------------------------*/
typedef struct _IMG_GIF_PLAYBACK_CMD_STOP_DATA_T
{
    HANDLE_T              h_img;
} IMG_GIF_PLAYBACK_CMD_STOP_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _IMG_GIF_PLAYBACK_CMD_CLOSE_DATA_T
 *  {
 *      HANDLE_T              h_img;
 *  } IMG_GIF_PLAYBACK_CMD_CLOSE_DATA_T;
 *  @endcode
 *  @li@c  h_img                                             -
 */
/*------------------------------------------------------------------*/
typedef struct _IMG_GIF_PLAYBACK_CMD_CLOSE_DATA_T
{
    HANDLE_T              h_img;
} IMG_GIF_PLAYBACK_CMD_CLOSE_DATA_T;


/* GIF playback command data*/
#if 0
/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _IMG_GIF_PLAYBACK_CMD_DATA_T
 *  {
 *      union
 *      {
 *          IMG_GIF_PLAYBACK_CMD_OPEN_DATA_T  t_open_data;
 *          IMG_GIF_PLAYBACK_CMD_PLAY_DATA_T  t_play_data;
 *          IMG_GIF_PLAYBACK_CMD_STOP_DATA_T  t_stop_data;
 *          IMG_GIF_PLAYBACK_CMD_CLOSE_DATA_T t_close_data;
 *      } u;
 *  } IMG_GIF_PLAYBACK_CMD_DATA_T;
 *  @endcode
 *  @li@c  t_open_data                                       -
 *  @li@c  t_play_data                                       -
 *  @li@c  t_stop_data                                       -
 *  @li@c  t_close_data                                      -
 */
/*------------------------------------------------------------------*/
typedef struct _IMG_GIF_PLAYBACK_CMD_DATA_T
{
    union
    {
        IMG_GIF_PLAYBACK_CMD_OPEN_DATA_T  t_open_data;
        IMG_GIF_PLAYBACK_CMD_PLAY_DATA_T  t_play_data;
        IMG_GIF_PLAYBACK_CMD_STOP_DATA_T  t_stop_data;
        IMG_GIF_PLAYBACK_CMD_CLOSE_DATA_T t_close_data;
    } u;

} IMG_GIF_PLAYBACK_CMD_DATA_T;
#endif
#endif
#if 0
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      GIF_NOT_SPECIFIED       = 0,
 *      GIF_NOT_DISPOSE         = 1,
 *      GIF_RESTORE_TO_BG       = 2,
 *      GIF_RESTORE_TO_PREVIOUS = 3,
 *      GIF_DISPOSE_NOT_DEFINED
 *  } IMG_GIF_DISPOSAL_T;
 *  @endcode
 *  @li@c  GIF_NOT_SPECIFIED                                 -
 *  @li@c  GIF_NOT_DISPOSE                                   -
 *  @li@c  GIF_RESTORE_TO_BG                                 -
 *  @li@c  GIF_RESTORE_TO_PREVIOUS                           -
 *  @li@c  GIF_DISPOSE_NOT_DEFINED                           -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    GIF_NOT_SPECIFIED       = 0,
    GIF_NOT_DISPOSE         = 1,
    GIF_RESTORE_TO_BG       = 2,
    GIF_RESTORE_TO_PREVIOUS = 3,
    GIF_DISPOSE_NOT_DEFINED
} IMG_GIF_DISPOSAL_T;

/* [20080710] This sturcture provides GIF frame rectangle info for disposal method area */
/* frame rectangle is in unit of pixel and relative to the edge of the logical screen */
/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct
 *  {
 *      UINT16              ui2_frame_x;
 *      UINT16              ui2_frame_y;
 *      UINT16              ui2_frame_width;
 *      UINT16              ui2_frame_height;
 *  } IMG_GIF_FRM_RECT_T;
 *  @endcode
 *  @li@c  ui2_frame_x                                       -
 *  @li@c  ui2_frame_y                                       -
 *  @li@c  ui2_frame_width                                   -
 *  @li@c  ui2_frame_height                                  -
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT16              ui2_frame_x;
    UINT16              ui2_frame_y;
    UINT16              ui2_frame_width;
    UINT16              ui2_frame_height;
} IMG_GIF_FRM_RECT_T;
#endif

/*------------------------------------------------------------------*/
/*! @struct CAP_OUTPUT_T
 *  @brief This structure is  JPEG/MPEG encoding output information for others.
 *  @code
 *  typedef struct
 *  {
 *      UCHAR*                              puac_dst_buffer;
 *      UINT32                               ui4_buffer_len;
 *      CAP_OUTPUT_PIC_FMT_T   e_out_pic_fmt;
 *      UINT32                              ui4_dst_height;
 *      UINT32                              ui4_dst_width;
 *  } CAP_OUTPUT_T;
 *  @endcode
 *  @li@c puac_dst_buffer                     - data buffer after encoding
 *  @li@c  ui4_buffer_len                     - encoded data length
 *  @li@c  e_out_pic_fmt                      - MPEG or JPEG format
 *  @li@c  ui4_dst_height                     - image height after encoding
 *  @li@c  ui4_dst_width                      - image width after encoding
 */
/*------------------------------------------------------------------*/
typedef struct _CAP_OUTPUT_T
{
    UCHAR*                  puac_dst_buffer;
    UINT32                  ui4_buffer_len;
    CAP_OUTPUT_PIC_FMT_T    e_out_pic_fmt;
    UINT32                  ui4_dst_height;
    UINT32                  ui4_dst_width;
}CAP_OUTPUT_T;


#define IMG_READ_FILE_MODE      ((UINT8) 0x01)    /**<  read file mode      */
#define IMG_RW_FILE_MODE        ((UINT8) 0x02)    /**<  read and write mode */
#define IMG_FOPEN_AGAIN			((UINT8) 0x04)

#define IMG_TYPE_DATA_NONE      ((UINT32)   0x00)    /**<  data none      */
#define IMG_TYPE_DATA_GIF_INDEX ((UINT32)   0x01)    /**<  gif index      */
#define IMG_TYPE_DATA_TAG       ((UINT32)   0x02)    /**<  tag data     */
#define IMG_TYPE_DATA_ENGINE    ((UINT32)   0x04)    /**<  image engine data */
#define IMG_TYPE_DATA_PALETTE   ((UINT32)   0x08)    /**<  palette data      */


#define JPEG_SW_ENG_NAME        "JPEG SW decoder"    /**< jpeg decoder name sw decoder         */
#define JPEG_HW_ENG_NAME        "JPEG HW decoder"    /**< jpeg decoder name hw decoder        */
#define JPEG_PM_HW_ENG_NAME     "JPG PM HW"         /**< jpeg picture mode hw decoder        */
#define JPEG_PM_HW_ENG_NAME2    "JPG PM HW2"        /**< jpeg picture mode hw decoder duplicated        */
#define JPEG_EXTENSION_NAME     "JPG;JPEG;JPE"    /**< jpeg extension name is valid       */

#define MPO_HW_ENG_NAME			"MPO HW decoder"
#define MPO_EXTENSION_NAME		"MPO"
#define BMP_HW_ENG_NAME         "BMP HW decoder"    /**<bitmap picture mode hw decoder         */
#define BMP_SW_ENG_NAME         "BMP SW decoder"    /**<bitmap picture mode sw decoder         */
#define BMP_EXTENSION_NAME      "BMP"    /**<bitmap extension name is valid         */
#define GIF_HW_ENG_NAME         "GIF HW decoder"    /**<gif picture mode hw decoder         */
#define GIF_SW_ENG_NAME         "GIF SW decoder"    /**<gif picture mode sw decoder         */
#define GIF_EXTENSION_NAME      "GIF"    /**<gif picture extension name is valid         */
#define PNG_HW_ENG_NAME         "PNG HW decoder"    /**<png picture mode hw decoder          */
#define PNG_SW_ENG_NAME         "PNG SW decoder"    /**<png picture mode sw decoder        */
#define PNG_EXTENSION_NAME      "PNG"    /**<png picture extension name is valid          */
#define WEBP_HW_ENG_NAME		"WEBP_HW_decoder"
#define WEBP_SW_ENG_NAME		"WEBP_SW_decoder"
#define WEBP_EXTENSION_NAME		"WEBP"
#define MNG_HW_ENG_NAME         "MNG HW decoder"    /**<mng picture mode hw decoder        */
#define MNG_EXTENSION_NAME      "MNG"    /**<mng picture extension name is valid        */
#define RLE_HW_ENG_NAME         "RLE HW decoder"    /**<RLE hw decoder         */
#define RLE_SW_ENG_NAME         "RLE SW decoder"    /**<RLE swdecode       */
#define RLE_EXTENSION_NAME      "RLE"    /**<RLE extension name is valid         */
#define RLE_DVDSPU_ENG_NAME     "DVDSPU HW dec"    /**<DVDSPU hw decoder        */
#define RLE_HDDVDSPU_ENG_NAME   "HDDVDSPU HW dec"    /**<HDDVDSPU hw decoder         */
#define RLE_BDPGIG_ENG_NAME     "BDPGIG HW dec"    /**<BDPGIG hw decoder        */

#define HT_JPG_IMG_TYPE         (HT_GROUP_IMG)    /**<JPEG handle        */
#define HT_TFX_IMG_TYPE         (HT_GROUP_IMG + 1)    /**<transition effect handle        */
#define HT_EXIF_IMG_TYPE        (HT_GROUP_IMG + 2)    /**<JPEG EXIF handle        */
#define HT_IMG_TYPE             (HT_GROUP_IMG + 3)    /**<IMG handle       */
#define HT_ENGINE_TYPE          (HT_GROUP_IMG + 4)    /**<IMG engine handle        */
#define HT_END_IMG_TYPE         (HT_GROUP_IMG + 5)    /**<No use, just indicate the end of handle        */
#define HT_MPO_IMG_TYPE			(HT_GROUP_IMG + 6)

/* used for ui4_type of IMG_CAPS_T */
/* NOTE: When add new capabilities, we have to cache it in IMG_JPG_HW_DEC_LIMITATION_T */
#define IMG_HW_JPG_CAP_PROGRESSIVE      ((UINT32) 0)    /**<progressive capabilities        */
#define IMG_HW_JPG_CAP_RESOLUTION       ((UINT32) 1)    /**<resolution capabilities        */
#define IMG_HW_JPG_CAP_PGSV_RESOLUTION  ((UINT32) 2)    /**<progressive/resolution capabilities          */
#define IMG_HW_JPG_CAP_FULL_MODE        ((UINT32) 3)    /**<full mode        */
#define IMG_SW_JPG_CAP_PROGRESSIVE      ((UINT32) 4)    /**<progressive capabilities        */
#define IMG_SW_JPG_CAP_RESOLUTION       ((UINT32) 5)    /**<resolution capabilities        */
#define IMG_SW_JPG_CAP_PGSV_RESOLUTION  ((UINT32) 6)    /**<progressive/resolution capabilities          */
#define IMG_SW_JPG_CAP_FULL_MODE        ((UINT32) 7)    /**<full mode        */

#define IMG_SW_BMP_CAP_RESOLUTION       ((UINT32) 100)    /**<bmp resolution capabilities        */
#define IMG_SW_PNG_CAP_RESOLUTION       ((UINT32) 101)    /**<png resolution capabilities        */
#define IMG_SW_GIF_CAP_RESOLUTION       ((UINT32) 102)    /**<gif resolution capabilities        */
#define IMG_HW_PNG_CAP_RESOLUTION		((UINT32) 103)
#define IMG_HW_WEBP_CAP_RESOLUTION		((UINT32) 104)
#define IMG_SW_WEBP_CAP_RESOLUTION		((UINT32) 105)

/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define IMGR_NO_DATA                ((INT32)     3)    /**< image no data       */
#define IMGR_ALREADY_STOP           ((INT32)     2)    /**< image stop       */
#define IMGR_NO_EXIF_DATA           ((INT32)     1)    /**< no exif information       */
#define IMGR_OK                     ((INT32)     0)    /**< image ok       */
#define IMGR_INSUFFICIENT_BUFFER    ((INT32)    -1)    /**< insufficient buffer       */
#define IMGR_FILE_OR_DIR_MISSING    ((INT32)    -2)    /**< file or directory is missing       */
#define IMGR_INV_HANDLE             ((INT32)    -3)    /**< invalid handle       */
#define IMGR_OUT_OF_HANDLE          ((INT32)    -4)    /**< out of handle       */
#define IMGR_OUT_OF_MEMORY          ((INT32)    -5)    /**< out of memory       */
#define IMGR_FCT_NOT_SUPPORT        ((INT32)    -6)    /**< function is not support       */
#define IMGR_NAME_EXIST             ((INT32)    -7)    /**< name exist       */
#define IMGR_INV_ARG                ((INT32)    -8)    /**< invalid argument       */
#define IMGR_ENGINE_NOT_FOUND       ((INT32)    -9)    /**< image engine not found      */
#define IMGR_IMG_NOT_SUPPORT        ((INT32)   -10)    /**< image type not support       */
#define IMGR_INTERNAL_ERR           ((INT32)   -11)    /**< internal error       */
#define IMGR_REENTRY_LOADING        ((INT32)   -12)    /**< retry loading       */
#define IMGR_ABORT_LOADING          ((INT32)   -13)    /**< abort loading       */
#define IMGR_DECODE_FAIL            ((INT32)   -14)    /**< decode fail       */
#define IMGR_INITIAL_ERR            ((INT32)   -15)    /**< inital error       */
#define IMGR_MSG_Q_ERR              ((INT32)   -16)    /**< message queue error       */
#define IMGR_FILE_ERR               ((INT32)   -17)    /**< file opration error       */
#define IMGR_CLI_ERROR              ((INT32)   -18)    /**< cli error       */
#define IMGR_AUTOFREE_ERROR         ((INT32)   -19)    /**< free resource error      */
#define IMGR_IO_ERROR               ((INT32)   -20)    /**< IO error       */
#define IMGR_DRV_BUSY               ((INT32)   -21)    /**< driver busy       */
#define IMGR_IMG_CRASHED            ((INT32)   -22)    /**< image crashed       */
#define IMGR_ENCODE_FAIL            ((INT32)   -23)
#define IMGR_ENCODE_STOP            ((INT32)   -24)
#define IMGR_DLNA_SERVER_OFF		((INT32)   -25)
#define IMGR_MPF_NOT_SUPPORT		((INT32)   -26)
#define IMGR_FILE_PERM_DENY			((INT32)   -27)
#define IMGR_RPC_ERROR				((INT32)   -28)
typedef struct{

    /** Size of the file in bytes. */
    UINT64       ui8_file_size;

    /** The creation time of the file - in UCT.*/
    UINT32    	 ui4_creation_time;

    /** Time of last access to this file
    */
    UINT32 		ui4_last_access_time;
    /** Time of last modification made to this file
    */
    UINT32 		ui4_last_modify_time;
} IMAGE_FILE_INFO_T;


typedef INT32 (*imageFmOpenFunc)(
									CHAR 	*puFileName, 
									UINT32 	*pui4FmHandle, 
									VOID	*pv_tag);

typedef INT32 (*imageFmReadFunc)(
									UINT32 	ui4FmHandle, 
									UINT8 	*pu1Dst, 
									UINT32 	ui4RequestLen, 
									UINT32 	*pui4ReadLen, 
									VOID	*pv_tag);

typedef INT32 (*imageFmWriteFunc)(
									UINT32 	ui4FmHandle, 
									UINT8 	*pu1Src, 
									UINT32 	ui4RequestLen, 
									UINT32 	*pui4WriteLen, 
									VOID	*pv_tag);

typedef INT32 (*imageFmSeekFunc)(
									UINT32 	ui4FmHandle, 
									INT64 	i8SeekPos,
									UINT8 	u1Whence,
									UINT64 	*pu8CurPos, 
									VOID	*pv_tag);

typedef INT32 (*imageFmCloseFunc)(
									UINT32 	ui4FmHandle, 
									VOID	*pv_tag);

typedef INT32 (*imageFmAssaciateFunc)(
                                            UINT32    	ui4FmHandle,
                                            VOID		*pv_app_tag,
                                            UINT32      ui4_phy_addr,
                                            UINT32      ui4_size);

typedef INT32 (*imageFmDisAssaciateFunc)(
                                            UINT32    	ui4FmHandle,
                                            VOID		*pv_app_tag);

typedef INT32 (*imageFmGetInfoFunc)(
									CHAR 				*puFileName,
									IMAGE_FILE_INFO_T 	*pt_file_info,
									VOID				*pv_app_tag);


typedef struct
{
	VOID*							pv_app_tag;
	imageFmAssaciateFunc            pfAssociate;
    imageFmDisAssaciateFunc         pfDissociate;
    imageFmOpenFunc					pfnOpen;                ///<  Open function
    imageFmCloseFunc              	pfnClose;               ///<  Close function
    imageFmReadFunc               	pfnRead;                ///<  Read function
    imageFmWriteFunc         		pfnWrite;              ///<  Wrtie function
    imageFmSeekFunc           		pfnSeek;               ///<  Seek function
    imageFmGetInfoFunc				pfnGetInfo;
} IMAGE_PULL_FM_FUNC_T;

extern IMAGE_PULL_FM_FUNC_T gt_fm_func;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#endif /* _U_IMG_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_IMG*/
/*----------------------------------------------------------------------------*/



/*!@page pageMW_IMG IMG Library Detailed Description
  * @par
  * @section secMW_IMG_1 1.Introduction
  * @par
  *      There are three components in the image library - image library interface 
  * layer, image engine selector and decoder/
  * encoder engines.
  * @par
  *     The main mission of this image library is to provide <b>image-decoding</b> 
  *	and <b>image-encoding</b> functions
  * for MW or AP. So far, IMG can support <b>jpg/png/bmp/gif</b> decoding and 
  *	jpg/mpg encoding. Except for jpg format
  * decoding,  the others only have <b>SW</b> decoding method.
  *
  * @par
  * @section secMW_IMG_2 2.Concept
  *
  * @par
  * @subsection ssecMW_IMG_2_1 2.1.Componets Diagram
  *     Following Figure is the components diagram of the image library.
  * @par
  *     @image html Img_Lib_Component.gif "Figure 1: Components diagram of the 
  *	image library"
  * @par
  *     At the top of the component diagram, there is an image library interface, 
  *	which wraps all interface APIs and the interface logic in this layer.
  * @par
  *     The image engine selector manages all image decoder engines. All image 
  *	decoder engines will be added into the image engine selector during system 
  *	start-up time. The selector will pass the file path or memory address of the 
  *	image file to each decoder engine, then ask it "Can you decode this file?". 
  *	If the image decoder answers "yes", the image engine selector will return the 
  *	function table of this engine, and image library interface layer will manipulate 
  *	the function table for following APIs.
  * @par
  *     The File Manager is the most common used middleware component in image 
  *	library, because every engine will directly access the File Manager to open, 
  *	read and close the image file.
  * @par
  *     HW decoder engine will use the HW JPEG driver through Resource Manager 
  *	to accelerate decoding process time.
  * @par
  *     JPEG/MPEG encoder are added latest. They directly  offer APIs for others, 
  *	not through image engine selector.
  *
  * @subsection secMW_IMG_2_2 2.2.Interface layer APIs
  *     Following Figure is the relation between image engine and interface layer APIs.
  * @par
  *     @image html Img_Lib_Call.gif "Figure 2: X- APIs and Engine"
  *
  * @subsection secMW_IMG_2_3 2.3.Program Flow
  *     Following Figure is the decoding one jpg file with JPG engine.
  * @par
  *     @image html Img_Lib_Seq.gif "Figure 3: Decdoing Flow"
  *
  * @par
  * @section secMW_IMG_3 3.Typical Case
  *      @dontinclude img_lib_samplecode.c
  * @par
  * @subsection secMW_IMG_3_1 3.1.Open an image file and get its handle from disk
  *      @skip c_img_fopen
  *      @skip {
  *      @until }
  * @par
  * @subsection secMW_IMG_3_2 3.2.Open an image file and get its handle from memory
  *      @skip c_img_mopen
  *      @skip {
  *      @until }
  * @par
  * @subsection secMW_IMG_3_3 3.3.Load an image file by handle
  *      @skip c_img_sync/async_load
  *      @skip {
  *      @until }
  * c_img_async_load usage is same as it.
  * @par
  * @subsection secMW_IMG_3_4 3.4.Get image information by handle
  *      @skip c_img_get_info
  *      @skip {
  *      @until }
  * @par
  * @subsection secMW_IMG_3_5 3.5.Get image dimension by handle
  *      @skip c_img_get_dimension
  *      @skip {
  *      @until }
  * @par
  * @subsection secMW_IMG_3_6 3.6.Stop the operation when it is loading
  *      @skip c_img_stop
  *      @skip {
  *      @until }
  * @par
  * @subsection secMW_IMG_3_7 3.7.Close the handle
  *      @skip c_img_close
  *      @skip {
  *      @until }
  *
  *
*/


