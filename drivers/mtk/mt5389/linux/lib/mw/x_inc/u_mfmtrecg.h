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
/*! @file u_mfmtrecg.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all media format recognizer specific definitions and structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_MUTIL_MFMTRECG
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_MFMTRECG_H_
#define _U_MFMTRECG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__ 

#include "u_mm_common.h"

#else

#include "u_mm_common.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MP3_RECOGNIZER_NAME "mp3 recognizer"    /**<        */
#define MP3_EXTENSION_NAME  "mp3"    /**<        */

#if 0 
/* This structure enumerates all types for media format. */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_TYPE_UNKNOWN = 0,
 *      MEDIA_TYPE_AUDIO_ES,
 *      MEDIA_TYPE_VIDEO_ES,
 *      MEDIA_TYPE_IMAGE,
 *      MEDIA_TYPE_CONTAINER,
 *      MEDIA_TYPE_PS,
 *      MEDIA_TYPE_TS
 *  } MEDIA_TYPE_T;
 *  @endcode
 *  @li@c  MEDIA_TYPE_UNKNOWN                                -
 *  @li@c  MEDIA_TYPE_AUDIO_ES                               -
 *  @li@c  MEDIA_TYPE_VIDEO_ES                               -
 *  @li@c  MEDIA_TYPE_IMAGE                                  -
 *  @li@c  MEDIA_TYPE_CONTAINER                              -
 *  @li@c  MEDIA_TYPE_PS                                     -
 *  @li@c  MEDIA_TYPE_TS                                     -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MEDIA_TYPE_UNKNOWN = 0,
    MEDIA_TYPE_AUDIO_ES,
    MEDIA_TYPE_VIDEO_ES,
    MEDIA_TYPE_IMAGE,
    MEDIA_TYPE_CONTAINER,
    MEDIA_TYPE_PS,
    MEDIA_TYPE_TS
} MEDIA_TYPE_T;

/* This structure enumerates all audio subtype types for media format. */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_AUD_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_AUD_SUBTYPE_MP2,
 *      MEDIA_AUD_SUBTYPE_MP3
 *  } MEDIA_AUD_SUBTYPE_T;
 *  @endcode
 *  @li@c  MEDIA_AUD_SUBTYPE_UNKNOWN                         -
 *  @li@c  MEDIA_AUD_SUBTYPE_MP2                             -
 *  @li@c  MEDIA_AUD_SUBTYPE_MP3                             -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MEDIA_AUD_SUBTYPE_UNKNOWN = 0,
    MEDIA_AUD_SUBTYPE_MP2,
    MEDIA_AUD_SUBTYPE_MP3
/*    MEDIA_AUD_SUBTYPE_ES,   */
} MEDIA_AUD_SUBTYPE_T;

/* This structure enumerates all audio subtype types for media format. */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_IMG_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_IMG_SUBTYPE_PNG,
 *      MEDIA_IMG_SUBTYPE_JPG
 *  } MEDIA_IMG_SUBTYPE_T;
 *  @endcode
 *  @li@c  MEDIA_IMG_SUBTYPE_UNKNOWN                         -
 *  @li@c  MEDIA_IMG_SUBTYPE_PNG                             -
 *  @li@c  MEDIA_IMG_SUBTYPE_JPG                             -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MEDIA_IMG_SUBTYPE_UNKNOWN = 0,
    MEDIA_IMG_SUBTYPE_PNG,
    MEDIA_IMG_SUBTYPE_JPG
} MEDIA_IMG_SUBTYPE_T;

/* This structure enumerates all video  subtype types for media format. */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_VID_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_VID_SUBTYPE_M1V,
 *      MEDIA_VID_SUBTYPE_M2V,
 *      MEDIA_VID_SUBTYPE_M4V
 *  } MEDIA_VID_SUBTYPE_T;
 *  @endcode
 *  @li@c  MEDIA_VID_SUBTYPE_UNKNOWN                         -
 *  @li@c  MEDIA_VID_SUBTYPE_M1V                             -
 *  @li@c  MEDIA_VID_SUBTYPE_M2V                             -
 *  @li@c  MEDIA_VID_SUBTYPE_M4V                             -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MEDIA_VID_SUBTYPE_UNKNOWN = 0,
    MEDIA_VID_SUBTYPE_M1V,
    MEDIA_VID_SUBTYPE_M2V,
    MEDIA_VID_SUBTYPE_M4V
} MEDIA_VID_SUBTYPE_T;

/* This structure enumerates all audio subtype types for media format. */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_CONTNR_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_CONTNR_SUBTYPE_AVI,
 *      MEDIA_CONTNR_SUBTYPE_ASF,    
 *      MEDIA_CONTNR_SUBTYPE_MP4   
 *  } MEDIA_CONTNR_SUBTYPE_T;
 *  @endcode
 *  @li@c  MEDIA_CONTNR_SUBTYPE_UNKNOWN                      -
 *  @li@c  MEDIA_CONTNR_SUBTYPE_AVI                          -
 *  @li@c  MEDIA_CONTNR_SUBTYPE_ASF                          -
 *  @li@c  MEDIA_CONTNR_SUBTYPE_MP4                          -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MEDIA_CONTNR_SUBTYPE_UNKNOWN = 0,
    MEDIA_CONTNR_SUBTYPE_AVI,
    MEDIA_CONTNR_SUBTYPE_ASF,    
    MEDIA_CONTNR_SUBTYPE_MP4   
} MEDIA_CONTNR_SUBTYPE_T;

/* This structure enumerates all PS  subtype types for media format. */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_PS_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_PS_SUBTYPE_MPG,
 *      MEDIA_PS_SUBTYPE_VOB,
 *      MEDIA_PS_SUBTYPE_DAT
 *  } MEDIA_PS_SUBTYPE_T;
 *  @endcode
 *  @li@c  MEDIA_PS_SUBTYPE_UNKNOWN                          -
 *  @li@c  MEDIA_PS_SUBTYPE_MPG                              -
 *  @li@c  MEDIA_PS_SUBTYPE_VOB                              -
 *  @li@c  MEDIA_PS_SUBTYPE_DAT                              -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MEDIA_PS_SUBTYPE_UNKNOWN = 0,
    MEDIA_PS_SUBTYPE_MPG,
    MEDIA_PS_SUBTYPE_VOB,
    MEDIA_PS_SUBTYPE_DAT
/*  MEDIA_PS_SUBTYPE_CAM, */
} MEDIA_PS_SUBTYPE_T;

/* This structure enumerates all TS  subtype types for media format. */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MEDIA_TS_SUBTYPE_UNKNOWN = 0,
 *      MEDIA_TS_SUBTYPE_TS_188,
 *      MEDIA_TS_SUBTYPE_TS_192,
 *      MEDIA_TS_SUBTYPE_TS_192_ZERO
 *   } MEDIA_TS_SUBTYPE_T;
 *  @endcode
 *  @li@c  MEDIA_TS_SUBTYPE_UNKNOWN                          -
 *  @li@c  MEDIA_TS_SUBTYPE_TS_188                           -
 *  @li@c  MEDIA_TS_SUBTYPE_TS_192                           -
 *  @li@c  MEDIA_TS_SUBTYPE_TS_192_ZERO                      -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MEDIA_TS_SUBTYPE_UNKNOWN = 0,
    MEDIA_TS_SUBTYPE_TS_188,
    MEDIA_TS_SUBTYPE_TS_192,
    MEDIA_TS_SUBTYPE_TS_192_ZERO
 } MEDIA_TS_SUBTYPE_T;

/* This structure contains the subtype of the media. This is a union structure to specify the subtype of the media and it could be audio, video and so on. */
/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct 
 *  {
 *      union
 *      {  
 *           MEDIA_AUD_SUBTYPE_T         e_aud_subtype;
 *           MEDIA_VID_SUBTYPE_T         e_vid_subtype;
 *           MEDIA_IMG_SUBTYPE_T         e_img_subtype;
 *           MEDIA_CONTNR_SUBTYPE_T      e_contnr_subtype;
 *           MEDIA_PS_SUBTYPE_T          e_ps_subtype;
 *           MEDIA_TS_SUBTYPE_T          e_ts_subtype;
 *      }u; 
 *  } MEDIA_SUBTYPE_T;  
 *  @endcode
 *  @li@c  e_aud_subtype                                     -
 *  @li@c  e_vid_subtype                                     -
 *  @li@c  e_img_subtype                                     -
 *  @li@c  e_contnr_subtype                                  -
 *  @li@c  e_ps_subtype                                      -
 *  @li@c  e_ts_subtype                                      -
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    union
    {  
         MEDIA_AUD_SUBTYPE_T         e_aud_subtype;
         MEDIA_VID_SUBTYPE_T         e_vid_subtype;
         MEDIA_IMG_SUBTYPE_T         e_img_subtype;
         MEDIA_CONTNR_SUBTYPE_T      e_contnr_subtype;
         MEDIA_PS_SUBTYPE_T          e_ps_subtype;
         MEDIA_TS_SUBTYPE_T          e_ts_subtype;
    }u; 
} MEDIA_SUBTYPE_T;  

/* This structure contains the format of the media. */
/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MEDIA_FORMAT_T
 *  {
 *      MEDIA_TYPE_T            e_media_type;
 *      MEDIA_SUBTYPE_T         t_media_subtype; 
 *  }   MEDIA_FORMAT_T;
 *  @endcode
 *  @li@c  e_media_type                                      -
 *  @li@c  t_media_subtype                                   -
 */
/*------------------------------------------------------------------*/
typedef struct _MEDIA_FORMAT_T
{
    MEDIA_TYPE_T            e_media_type;
    MEDIA_SUBTYPE_T         t_media_subtype; 
}   MEDIA_FORMAT_T;
#endif

/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define MFMTRECGR_OK                        ((INT32)    0)    /**<        */
#define MFMTRECGR_INSUFFICIENT_BUFFER       ((INT32)   -1)    /**<        */
#define MFMTRECGR_FILE_OR_DIR_MISSING       ((INT32)   -2)    /**<        */
#define MFMTRECGR_INV_HANDLE                ((INT32)   -3)    /**<        */
#define MFMTRECGR_OUT_OF_HANDLE             ((INT32)   -4)    /**<        */
#define MFMTRECGR_OUT_OF_MEMORY             ((INT32)   -5)    /**<        */
#define MFMTRECGR_FCT_NOT_SUPPORT           ((INT32)   -6)    /**<        */
#define MFMTRECGR_NAME_EXIST                ((INT32)   -7)    /**<        */
#define MFMTRECGR_INV_ARG                   ((INT32)   -8)    /**<        */
#define MFMTRECGR_HANDLER_NOT_FOUND          ((INT32)   -9)    /**<        */
#define MFMTRECGR_INTERNAL_ERR              ((INT32)   -10)    /**<        */
#define MFMTRECGR_REENTRY_LOADING           ((INT32)   -11)    /**<        */
#define MFMTRECGR_INITIAL_ERR               ((INT32)   -12)    /**<        */
#define MFMTRECGR_MSG_Q_ERR                 ((INT32)   -13)    /**<        */
#define MFMTRECGR_FILE_ERR                  ((INT32)   -14)    /**<        */
#define MFMTRECGR_CLI_ERROR                 ((INT32)   -15)    /**<        */
#define MFMTRECGR_AUTOFREE_ERROR            ((INT32)   -16)    /**<        */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_MUTIL_MFMTRECG */
/*----------------------------------------------------------------------------*/

/*! @page pageMW_MUTIL_MFMTRECG Multi Media Format Recognizer
 *  @par
 *  @section secMW_MUTIL_MFMTRECG_1  1. Introduction
 *      This function is to get the media format of the specified file or memery.
 *  @par
 *  @section secMW_MUTIL_MFMTRECG_2  2. Concept
 *      It will check the media format by the extention first. If not found, it will check all the format.
 *  @par
 *  @section secMW_MUTIL_MFMTRECG_3  3. Brief Design
 *      The regular format will be first and then the others, such as video ES and audio ES. By this way, it will speed up the recognize process.
 *  @par
 *      @include mfmtrecg_sample_code.c
 *      
 */

#endif /* _U_MFMTRECG_H_ */


