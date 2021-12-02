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
/*! @file u_mheg5.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:This header file contains MHEG5 exported constants, macros, and types.
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_MHEG5_H_
#define _U_MHEG5_H_
/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "x_dbg.h"
#include "x_scc.h"
#include "u_scc.h"
#include "x_gl.h"
#include "u_scdb.h"
#include "u_wgl.h"


/*!
 * @addtogroup    groupMW_MHEG5
 * @brief    This header file contains MHEG5 exported constants, macros, and types.
 * 
 * @see groupMW_MHEG5
 *
 * @{
 * */


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define MHEG5R_ASYNC_RESPONSE       ((INT32)    1)    /**<Indicate the request will be responsed asynchronously        */

#define MHEG5R_OK                   ((INT32)    0)    /**<OK        */
#define MHEG5R_INV_ARG              ((INT32) -  1)    /**<Invalid arguments        */
#define MHEG5R_INV_HANDLE           ((INT32) -  2)    /**<Invalid handle        */
#define MHEG5R_OUT_OF_MEMORY        ((INT32) -  3)    /**<Out of memory        */
#define MHEG5R_OUT_OF_HANDLE        ((INT32) -  4)    /**<Out of handle        */
#define MHEG5R_INTERNAL_ERROR       ((INT32) -  5)    /**<Internal error        */
#define MHEG5R_NOT_INIT             ((INT32) -  6)    /**<Not initialized       */
#define MHEG5R_ALREADY_INIT         ((INT32) -  7)    /**<Already initialized        */
#define MHEG5R_INIT_FAIL            ((INT32) -  8)    /**<Failed to initialized        */
#define MHEG5R_INV_STATE            ((INT32) -  9)    /**<Invalid state        */
#define MHEG5R_SVC_NOT_FOUND        ((INT32) - 10)    /**<Service is not found       */
#define MHEG5R_ENG_NOT_FOUND        ((INT32) - 11)    /**<Engine is not found        */
#define MHEG5R_CONNECT_FAIL         ((INT32) - 12)    /**<Failed to connect        */
#define MHEG5R_LOAD_NIT_FAIL        ((INT32) - 13)    /**<Failed to load NIT        */
#define MHEG5R_LOAD_SDT_FAIL        ((INT32) - 14)    /**<Failed to load SDT        */
#define MHEG5R_LOAD_PAT_FAIL        ((INT32) - 15)    /**<Failed to load PAT        */
#define MHEG5R_LOAD_PMT_FAIL        ((INT32) - 16)    /**<Failed to LOAD PMT        */
#define MHEG5R_CREATE_TIMER_FAIL    ((INT32) - 17)    /**<Failed to create a timer        */
#define MHEG5R_START_TIMER_FAIL     ((INT32) - 18)    /**<Failed to start a timer        */
#define MHEG5R_MISMATCHED_RUN       ((INT32) - 19)    /**<Mismatched run        */
#define MHEG5R_OUT_OF_RESOURCES     ((INT32) - 20)    /**<Out of semaphores        */
#define MHEG5R_PARSE_PMT_FAIL       ((INT32) - 21)    /**<Failed to parse PMT        */
#define MHEG5R_SVC_UNAVAILABLE      ((INT32) - 22)    /**<Service is unavailable        */
#define MHEG5R_CONFIG_OSD_FAIL      ((INT32) - 23)    /**<Failed to config OSD        */
#define MHEG5R_COMP_NOT_OPEN        ((INT32) - 24)    /**<The MHEG-5 component has not been opened by a call to tmMHEG5Open()        */
#define MHEG5R_COMP_ALREADY_OPEN    ((INT32) - 25)    /**<The MHEG-5 component has already been opened by a call to tmMHEG5Open()        */
#define MHEG5R_COMP_NOT_STARTED     ((INT32) - 26)    /**<The MHEG-5 component has not been started by a call to tmMHEG5Start()         */
#define MHEG5R_COMP_ALREADY_STARTED ((INT32) - 27)    /**<The MHEG-5 component has already been started by a call to tmMHEG5Start()        */
#define MHEG5R_REG_CI_NFY_FAIL      ((INT32) - 28)    /**<Failed to register CI notify function        */
#define MHEG5R_CI_START_APP_BUSY    ((INT32) - 29)    /**<A CI application has already been started        */
#define MHEG5R_CI_NO_APP_STARTED    ((INT32) - 29)    /**<No CI applications has been started        */
#define MHEG5R_NO_SUPPORTED_PROFILE ((INT32) - 30)    /**<No supported profile        */
#define MHEG5R_CI_NOT_SUPPORTED     ((INT32) - 31)
#define MHEG5R_ICS_REG_NFY_FAIL     ((INT32) - 32)    /**<the input paremeter in registering notify function is bad */
#define MHEG5R_ICS_ENG_NFY_FAIL     ((INT32) - 33)    /**<notify the ICS's guidance result failed */

#define MHEG5_DBG_INFO(x)    /**<Debug info        */
#if 0
#define MHEG5_DBG_APP(x)            x_dbg_stmt x    /**<Debug info        */
#define MHEG5_DBG_ERROR(x)          x_dbg_stmt x    /**<Debug info        */
#else
#define MHEG5_DBG_APP(x)    /**<Debug info        */
#define MHEG5_DBG_ERROR(x)    /**<Debug info        */
#endif

#if 1
    #define MHEG5_KEEP_SYS_VIDEO_MODE    /**<keep system video mode       */
#endif

#if 1
    #define MHEG5_PAUSE_RESUME_MODE    /**<OSD arbitration mode     */
#endif

#ifdef DEBUG
    #define MHEG5_ASSERT(cond)                                              \
    {                                                                       \
        if (!(cond))                                                        \
        {                                                                   \
            x_dbg_stmt("\n File : %s, Line : %d ", __FILE__, __LINE__);     \
            DBG_ABORT(DBG_MOD_MHEG_5);                                       \
        }                                                                   \
    }    /**<        */
#else
    #define MHEG5_ASSERT(cond)    /**<Assert        */
#endif

#define MHEG5_SCREEN_WIDTH  ((INT32) 720)    /**<Width of MHEG5 graphics plane        */
#define MHEG5_SCREEN_HEIGHT ((INT32) 576)    /**<Height of MHEG5 graphics plane        */



#define MHEG5_MSG_GRD_RENDERED      ((UINT32) 0)    /**<????        */
#define MHEG5_MSG_GRD_KEY_DOWN      ((UINT32) 1)    /**<????        */
#define MHEG5_MSG_GRD_DRAWN         ((UINT32) 2)    /**<????        */

#define MHEG5_STOP_SVC_PARAM_NONE       ((UINT32) 0x0)    /**<Operation flag, do nothing when stop MHEG5 service   */
#define MHEG5_STOP_SVC_PARAM_ADJ_VDP    ((UINT32) 0x1)    /**<Operation flag, adjust vdp when stop MHEG5 service        */

#define MHEG5_VIDEO_UPDATE_PARAM_NONE       ((UINT32) 0x0)    /**<Video update notification flag, N/A       */
#define MHEG5_VIDEO_UPDATE_PARAM_FREEZE     ((UINT32) 0x1)    /**<Video update notification flag, video froze/unfroze      */
#define MHEG5_VIDEO_UPDATE_PARAM_SCR_MDOE   ((UINT32) 0x2)    /**<Video update notification flag,  screen mode controled by MHEG5 or not    */
#define MHEG5_VIDEO_UPDATE_PARAM_IFRAME     ((UINT32) 0x4)    /**< Video update notification flag, Iframe played/stopped       */

/* Minimum section buffers */
#define MHEG5_MIN_SEC_BUF           (100)       /**<MHEG5_MIN_SECTION_BUFFERS        */

/* Section buffer, buffer size (bytes) */
#define MHEG5_SEC_BUF_SIZE          (4*1024)    /**<MHEG5_SECTION_BUFFER_SIZE        */


typedef struct _STREAM_PID_INFO_T
{
    MPEG_2_PID_T    t_pid;
    MPEG_2_PID_T    t_pcr_pid;
    BOOL            b_found;
    BOOL            b_waiting;
    UINT8           ui1_enc_type;

} STREAM_PID_INFO_T;


/*------------------------------------------------------------------*/
/*! @enum MHEG5_COEXIST_DISPLAY_T
 *  @brief MHEG5 UI display mode
 *  @code
 *  typedef enum _MHEG5_COEXIST_DISPLAY_T
 *  {
 *      MHEG5_HIDE_GUI,
 *      MHEG5_OVERLAY_GUI,
 *      MHEG5_OVERLAY_GUI_LIGHTER,
 *      MHEG5_OVERLAY_GUI_BOTTOMMOST
 *  } MHEG5_COEXIST_DISPLAY_T;
 *  @endcode
 *  @li@c  MHEG5_HIDE_GUI                                    - Hide MHEG5 UI
 *  @li@c  MHEG5_OVERLAY_GUI                              - Overlay MHEG5 UI
 *  @li@c  MHEG5_OVERLAY_GUI_LIGHTER                - Overlay MHEG5 UI lighter
 *  @li@c  MHEG5_OVERLAY_GUI_BOTTOMMOST        - Overlay MHEG5 UI bottommost
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_COEXIST_DISPLAY_T
{
    MHEG5_HIDE_GUI,
    MHEG5_OVERLAY_GUI,
    MHEG5_OVERLAY_GUI_LIGHTER,
    MHEG5_OVERLAY_GUI_BOTTOMMOST
} MHEG5_COEXIST_DISPLAY_T;

/*------------------------------------------------------------------*/
/*! @enum MHEG5_DISPLAY_ASPECT_RATIO_T
 *  @brief The default display aspect ratio
 *  @code
 *  typedef enum _MHEG5_DISPLAY_ASPECT_RATIO_T
 *  {
 *      MHEG5_DISPLAY_ASPECT_RATIO_16_9,
 *      MHEG5_DISPLAY_ASPECT_RATIO_4_3
 *  } MHEG5_DISPLAY_ASPECT_RATIO_T;
 *  @endcode
 *  @li@c  MHEG5_DISPLAY_ASPECT_RATIO_16_9                  - 16:9 display aspect ratio
 *  @li@c  MHEG5_DISPLAY_ASPECT_RATIO_4_3                    - 4:3 display aspect ratio
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_DISPLAY_ASPECT_RATIO_T
{
    MHEG5_DISPLAY_ASPECT_RATIO_16_9,
    MHEG5_DISPLAY_ASPECT_RATIO_4_3
} MHEG5_DISPLAY_ASPECT_RATIO_T;

/*------------------------------------------------------------------*/
/*! @enum MHEG5_ASPECT_RATIO_T
 *  @brief Aspect ratio
 *  @code
 *  typedef enum _MHEG5_ASPECT_RATIO_T
 *  {
 *      MHEG5_ASPECT_RATIO_4_3,
 *      MHEG5_ASPECT_RATIO_16_9,
 *      MHEG5_ASPECT_RATIO_UNKNOWN
 *  } MHEG5_ASPECT_RATIO_T;
 *  @endcode
 *  @li@c  MHEG5_ASPECT_RATIO_4_3                            - 4:3 aspect ratio
 *  @li@c  MHEG5_ASPECT_RATIO_16_9                          - 16:9 aspect ratio
 *  @li@c  MHEG5_ASPECT_RATIO_UNKNOWN                  - unknown aspect ratio
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_ASPECT_RATIO_T
{
    MHEG5_ASPECT_RATIO_16_9,
    MHEG5_ASPECT_RATIO_4_3,
    MHEG5_ASPECT_RATIO_UNKNOWN
} MHEG5_ASPECT_RATIO_T;

/*------------------------------------------------------------------*/
/*! @enum MHEG5_WSS_T
 *  @brief MHEG5_WSS_T
 *  @code
 *  typedef enum _MHEG5_WSS_T
 *  {
 *      MHEG5_WSS_NA_0 = 0,     
 *      MHEG5_WSS_14_9,         
 *      MHEG5_WSS_14_9T,        
 *      MHEG5_WSS_NA_3,         
 *      MHEG5_WSS_16_9T,        
 *      MHEG5_WSS_NA_5,         
 *      MHEG5_WSS_NA_6,         
 *      MHEG5_WSS_16_9FF,       
 *      MHEG5_WSS_4_3FF,        
 *      MHEG5_WSS_NA_9,         
 *      MHEG5_WSS_NA_10,        
 *      MHEG5_WSS_16_9,         
 *      MHEG5_WSS_NA_12,        
 *      MHEG5_WSS_GREATER_16_9, 
 *      MHEG5_WSS_4_3_SP_14_9,  
 *      MHEG5_WSS_NA_15         
 *  } MHEG5_WSS_T;
 *  @endcode
 *  @li@c  MHEG5_WSS_NA_0                                    - NA, 0000
 *  @li@c  MHEG5_WSS_14_9                                    - 1000 14:9 Letterbox Centre            (504 lines) 
 *  @li@c  MHEG5_WSS_14_9T                                   - 0100 14:9 Letterbox Top               (504 lines)
 *  @li@c  MHEG5_WSS_NA_3                                    - NA, 1100
 *  @li@c  MHEG5_WSS_16_9T                                   - 0010 16:9 Letterbox Top               (430 lines) 
 *  @li@c  MHEG5_WSS_NA_5                                    - NA, 0101
 *  @li@c  MHEG5_WSS_NA_6                                    - NA, 0110
 *  @li@c  MHEG5_WSS_16_9FF                                 - 1110 16:9 Full height 16:9 anamorphic (576 lines)
 *  @li@c  MHEG5_WSS_4_3FF                                   - 0001 4:3 Full                         (576 lines)
 *  @li@c  MHEG5_WSS_NA_9                                    - NA, 1001
 *  @li@c  MHEG5_WSS_NA_10                                  - NA, 1010
 *  @li@c  MHEG5_WSS_16_9                                    - 1101 16:9 Letterbox Centre            (430 lines)
 *  @li@c  MHEG5_WSS_NA_12                                  - NA, 0011
 *  @li@c  MHEG5_WSS_GREATER_16_9                     - 1011 > 16:9 Letterbox deeper than 16:9 
 *  @li@c  MHEG5_WSS_4_3_SP_14_9                        - 0111 14:9 Full-height 4:3, S&P 14:9   (576 lines)
 *  @li@c  MHEG5_WSS_NA_15                                  - NA, 1111
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_WSS_T
{
    MHEG5_WSS_NA_0 = 0,
    MHEG5_WSS_14_9,
    MHEG5_WSS_14_9T,
    MHEG5_WSS_NA_3,
    MHEG5_WSS_16_9T,
    MHEG5_WSS_NA_5,
    MHEG5_WSS_NA_6,
    MHEG5_WSS_16_9FF,
    MHEG5_WSS_4_3FF,
    MHEG5_WSS_NA_9,
    MHEG5_WSS_NA_10,
    MHEG5_WSS_16_9,
    MHEG5_WSS_NA_12,
    MHEG5_WSS_GREATER_16_9,
    MHEG5_WSS_4_3_SP_14_9,
    MHEG5_WSS_NA_15
} MHEG5_WSS_T;

/*------------------------------------------------------------------*/
/*! @enum MHEG5_VIDEO_MODE_T
 *  @brief MHEG5 video mode
 *  @code
 *  typedef enum _MHEG5_VIDEO_MODE_T
 *  {
 *      MHEG5_VIDEO_MODE_AUTO = 0,
 *      MHEG5_VIDEO_MODE_FULLSCREEN,
 *      MHEG5_VIDEO_MODE_LETTER_BOX,
 *      MHEG5_VIDEO_MODE_PAN_SCAN,
 *      MHEG5_VIDEO_MODE_PILLAR_BOX,
 *      MHEG5_VIDEO_MODE_GEN_LETTER_BOX,
 *      MHEG5_VIDEO_MODE_GEN_PAN_SCAN,
 *      MHEG5_VIDEO_MODE_FORCE_4x3,
 *      MHEG5_VIDEO_MODE_FORCE_16x9,
 *      MHEG5_VIDEO_MODE_PANORAMA,
 *      MHEG5_VIDEO_MODE_DOT_BY_DOT,
 *      MHEG5_VIDEO_MODE_ZOOM
 *  } MHEG5_VIDEO_MODE_T;
 *  @endcode
 *  @li@c  MHEG5_VIDEO_MODE_AUTO                            - Auto
 *  @li@c  MHEG5_VIDEO_MODE_FULLSCREEN                 - Full screen
 *  @li@c  MHEG5_VIDEO_MODE_LETTER_BOX                 - letter box
 *  @li@c  MHEG5_VIDEO_MODE_PAN_SCAN                    - PAN scan
 *  @li@c  MHEG5_VIDEO_MODE_PILLAR_BOX                  - pallar box
 *  @li@c  MHEG5_VIDEO_MODE_GEN_LETTER_BOX          - gen letter box
 *  @li@c  MHEG5_VIDEO_MODE_GEN_PAN_SCAN             - gen pan scan
 *  @li@c  MHEG5_VIDEO_MODE_FORCE_4x3                    - Force 4:3
 *  @li@c  MHEG5_VIDEO_MODE_FORCE_16x9                  - Force 16:9
 *  @li@c  MHEG5_VIDEO_MODE_PANORAMA                    - Panorama
 *  @li@c  MHEG5_VIDEO_MODE_DOT_BY_DOT                 - Dot by dot
 *  @li@c  MHEG5_VIDEO_MODE_ZOOM                            - Zoom
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_VIDEO_MODE_T
{
    MHEG5_VIDEO_MODE_AUTO = 0,
    MHEG5_VIDEO_MODE_FULLSCREEN,

    /* 16:9 video on 4:3 display */
    MHEG5_VIDEO_MODE_LETTER_BOX,
    MHEG5_VIDEO_MODE_PAN_SCAN,

    /* 4:3 video on 16:9 display */
    MHEG5_VIDEO_MODE_PILLAR_BOX,
    
    MHEG5_VIDEO_MODE_GEN_LETTER_BOX,
    MHEG5_VIDEO_MODE_GEN_PAN_SCAN,
    
    MHEG5_VIDEO_MODE_FORCE_4x3,
    MHEG5_VIDEO_MODE_FORCE_16x9,

    MHEG5_VIDEO_MODE_PANORAMA,
    MHEG5_VIDEO_MODE_DOT_BY_DOT,

    MHEG5_VIDEO_MODE_ZOOM
} MHEG5_VIDEO_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum MHEG5_DECFC_T
 *  @brief MHEG5_DECFC_T
 *  @code
 *  typedef enum _MHEG5_DECFC_T
 *  {
 *      MHEG5_DECFC_UNKNOWN,
 *      MHEG5_DECFC_IGNORE,
 *      MHEG5_DECFC_PAN_SCAN,
 *      MHEG5_DECFC_LETTER_BOX,
 *      MHEG5_DECFC_LETTER_BOX_14_9,
 *      MHEG5_DECFC_PILLAR_BOX,
 *      MHEG5_DECFC_ZOOM_4_3,
 *      MHEG5_DECFC_ZOOM_14_9
 *  } MHEG5_DECFC_T;
 *  @endcode
 *  @li@c  MHEG5_DECFC_UNKNOWN                            - Unknown
 *  @li@c  MHEG5_DECFC_IGNORE                                - DECFC_IGNORE
 *  @li@c  MHEG5_DECFC_PAN_SCAN                            - DECFC_PAN_SCAN  
 *  @li@c  MHEG5_DECFC_LETTER_BOX                         - DECFC_LETTER_BOX
 *  @li@c  MHEG5_DECFC_LETTER_BOX_14_9                - DECFC_LETTER_BOX_14_9  
 *  @li@c  MHEG5_DECFC_PILLAR_BOX                          - DECFC_PILLAR_BOX 
 *  @li@c  MHEG5_DECFC_ZOOM_4_3                            - DECFC_ZOOM_4_3 
 *  @li@c  MHEG5_DECFC_ZOOM_14_9                          - DECFC_ZOOM_14_9  
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_DECFC_T
{
    MHEG5_DECFC_UNKNOWN,
    MHEG5_DECFC_IGNORE,
    
    /* 16:9 video on 4:3 display */
    MHEG5_DECFC_PAN_SCAN,
    MHEG5_DECFC_LETTER_BOX,
    MHEG5_DECFC_LETTER_BOX_14_9,

    /* 4:3 video on 16:9 display */
    MHEG5_DECFC_PILLAR_BOX,
    MHEG5_DECFC_ZOOM_4_3,
    MHEG5_DECFC_ZOOM_14_9
} MHEG5_DECFC_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_MSG_T
 *  @brief MHEG5 message structure
 *  @code
 *  typedef struct _MHEG5_MSG_T
 *  {
 *      UINT8       ui1_module;
 *      UINT16      ui2_msg_type;
 *      UINT32      ui4_seq_num;
 *      HANDLE_T    h_obj;
 *      UINT16      ui2_on_id;
 *      UINT16      ui2_ts_id;
 *      UINT16      ui2_svc_id;
 *      VOID*       pv_nfy_tag;
 *  } MHEG5_MSG_T;
 *  @endcode
 *  @li@c  ui1_module                                   - Module
 *  @li@c  ui2_msg_type                               - Message type
 *  @li@c  ui4_seq_num                                - Sequence number
 *  @li@c  h_obj                                            - Object handle
 *  @li@c  ui2_on_id                                      - Original network  ID
 *  @li@c  ui2_ts_id                                       - Transport stream ID
 *  @li@c  ui2_svc_id                                     - Service ID
 *  @li@c  pv_nfy_tag                                    - Tag
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_MSG_T
{
    UINT8       ui1_module;
    UINT16      ui2_msg_type;
    UINT32      ui4_seq_num;
    HANDLE_T    h_obj;
    UINT16      ui2_on_id;
    UINT16      ui2_ts_id;
    UINT16      ui2_svc_id;
    VOID*       pv_nfy_tag;
} MHEG5_MSG_T;

/*------------------------------------------------------------------*/
/*! @enum MHEG5_NFY_COND_T
 *  @brief MHEG5 notify condition
 *  @code
 *  typedef enum _MHEG5_NFY_COND_T
 *  {
 *      MHEG5_NFY_COND_TABLE_LOADED,
 *      MHEG5_NFY_COND_TABLE_FAILED,    
 *      MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_RETURNS,
 *      MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_GONE,
 *      MHEG5_NFY_COND_NB_ACTION_INITIATE_AUTOBOOT,
 *      MHEG5_NFY_COND_CAROUSEL_ID_CHANGED,
 *      MHEG5_NFY_COND_SERVICE_GONE,
 *      MHEG5_NFY_COND_SERVICE_RETURNS,
 *      MHEG5_NFY_COND_SERVICE_CHANGED
 *  } MHEG5_NFY_COND_T;
 *  @endcode
 *  @li@c  MHEG5_NFY_COND_TABLE_LOADED                       - SI table loaded well
 *  @li@c  MHEG5_NFY_COND_TABLE_FAILED                        - SI table loaded failed
 *  @li@c  MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_RETURNS     - Broadcast ID descriptor returns
 *  @li@c  MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_GONE        - Broadcast ID descriptor gone
 *  @li@c  MHEG5_NFY_COND_NB_ACTION_INITIATE_AUTOBOOT        - initiate autoboot
 *  @li@c  MHEG5_NFY_COND_CAROUSEL_ID_CHANGED                - Carousel ID chanaged
 *  @li@c  MHEG5_NFY_COND_SERVICE_GONE                       - Service gone
 *  @li@c  MHEG5_NFY_COND_SERVICE_RETURNS                    - Service returns
 *  @li@c  MHEG5_NFY_COND_SERVICE_CHANGED                    - Service changed
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_NFY_COND_T
{
    MHEG5_NFY_COND_TABLE_LOADED,
    MHEG5_NFY_COND_TABLE_FAILED,    
    MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_RETURNS,
    MHEG5_NFY_COND_DATA_BROADCASD_ID_DESC_GONE,
    MHEG5_NFY_COND_NB_ACTION_INITIATE_AUTOBOOT,
    MHEG5_NFY_COND_CAROUSEL_ID_CHANGED,
    MHEG5_NFY_COND_SERVICE_GONE,
    MHEG5_NFY_COND_SERVICE_RETURNS,
    MHEG5_NFY_COND_SERVICE_CHANGED
} MHEG5_NFY_COND_T;

/*------------------------------------------------------------------*/
/*! @enum MHEG5_IFRAME_SATTE_T
 *  @brief Iframe states
 *  @code
 *  typedef enum _MHEG5_IFRAME_STATE_T
 *  {
 *      MHEG5_IFRAME_PLAY,
 *      MHEG5_IFRAME_STOP
 *  } MHEG5_IFRAME_SATTE_T;
 *  @endcode
 *  @li@c  MHEG5_IFRAME_PLAY                                 - Iframe plyed
 *  @li@c  MHEG5_IFRAME_STOP                                 - Iframe stopped
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_IFRAME_STATE_T
{
    MHEG5_IFRAME_PLAY,
    MHEG5_IFRAME_STOP
} MHEG5_IFRAME_SATTE_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_STREAM_DESC_T
 *  @brief MHEG5 stream descriptor
 *  @code
 *  typedef struct _MHEG5_STREAM_DESC_T
 *  {
 *      UINT16          ui2_on_id;
 *      UINT16          ui2_ts_id;
 *      UINT16          ui2_svc_id;
 *      INT32           i4_comp_tag;
 *      STREAM_TYPE_T   e_strm_type;
 *      ISO_639_LANG_T  s_lang;
 *      ISO_639_LANG_T  s_gui_lang;
 *      AUD_TYPE_T      e_aud_type;
 *  } MHEG5_STREAM_DESC_T;
 *  @endcode
 *  @li@c  ui2_on_id                                        - Original network  ID
 *  @li@c  ui2_ts_id                                         - Transport stream ID
 *  @li@c  ui2_svc_id                                       - Service ID
 *  @li@c  i4_comp_tag                                    - Component Tag
 *  @li@c  e_strm_type                                     - Stream type
 *  @li@c  s_lang                                              - Audio language
 *  @li@c  s_gui_lang                                        - ????
 *  @li@c  e_aud_type                                       - Audio type
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_STREAM_DESC_T
{
    UINT16          ui2_on_id;
    UINT16          ui2_ts_id;
    UINT16          ui2_svc_id;
    INT32           i4_comp_tag;
    STREAM_TYPE_T   e_strm_type;
    ISO_639_LANG_T  s_lang;
    ISO_639_LANG_T  s_gui_lang;
    AUD_TYPE_T      e_aud_type;
} MHEG5_STREAM_DESC_T;

typedef enum _MHEG5_WAM_T
{
    MHEG5_WAM_CENTRE_CUT_OUT,
    MHEG5_WAM_LETTERBOX
} MHEG5_WAM_T;


/*------------------------------------------------------------------*/
/*! @struct MHEG5_CNV_CFG_T
 *  @brief
 *  @code
 *  typedef struct _MHEG5_CNV_CFG_T
 *  {
 *      MHEG5_DECFC_T                   e_decfc;
 *      MHEG5_WSS_T                     e_wss;
 *      MHEG5_VIDEO_MODE_T              e_vid_mode;
 *      MHEG5_DISPLAY_ASPECT_RATIO_T    e_display_ar;
 *  } MHEG5_CNV_CFG_T;
 *  @endcode
 *  @li@c  e_decfc                                           - ????
 *  @li@c  e_wss                                             - ?????
 *  @li@c  e_vid_mode                                    - video mode
 *  @li@c  e_display_ar                                   - display aspect ratio 
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_CNV_CFG_T
{
    HANDLE_T                        h_scc_video;
    MHEG5_DECFC_T                   e_decfc;
    MHEG5_WSS_T                     e_wss;
    MHEG5_VIDEO_MODE_T              e_vid_mode;
    MHEG5_DISPLAY_ASPECT_RATIO_T    e_display_ar;
    MHEG5_WAM_T                     e_wam;
    MHEG5_ASPECT_RATIO_T            e_scene_ar;
    MHEG5_ASPECT_RATIO_T            e_src_aspect_ratio;
    
} MHEG5_CNV_CFG_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_DISPLAY_PARAM_T
 *  @brief MHEG5 display parameters
 *  @code
 *  typedef struct _MHEG5_DISPLAY_PARAM_T
 *  {
 *      UINT8                       ui1_afd;
 *      MHEG5_ASPECT_RATIO_T        e_src_aspect_ratio;
 *      SCC_VID_SCREEN_MODE_T       e_screen_mode;
 *      MHEG5_VIDEO_MODE_T          e_vid_mode;
 *      UINT16                      ui2_video_width;
 *      UINT16                      ui2_video_height;
 *  } MHEG5_DISPLAY_PARAM_T;
 *  @endcode
 *  @li@c  ui1_afd                                          - AFD
 *  @li@c  e_src_aspect_ratio                          - aspect ratio
 *  @li@c  e_screen_mode                               - screen mode
 *  @li@c  e_vid_mode                                    - video mode
 *  @li@c  ui2_video_width                              - video width
 *  @li@c  ui2_video_height                             - video height
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_DISPLAY_PARAM_T
{
    UINT8                       ui1_afd;
    MHEG5_ASPECT_RATIO_T        e_src_aspect_ratio;
    SCC_VID_SCREEN_MODE_T       e_screen_mode;
    MHEG5_VIDEO_MODE_T          e_vid_mode;
    UINT16                      ui2_video_width;
    UINT16                      ui2_video_height;
} MHEG5_DISPLAY_PARAM_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_STOP_SVC_PARAM_T
 *  @brief MHEG5 stop service parameters
 *  @code
 *  typedef struct
 *  {
 *      UINT32          ui4_op_flag;
 *      BOOL            b_adjust_vdp;
 *  } MHEG5_STOP_SVC_PARAM_T;
 *  @endcode
 *  @li@c  ui4_op_flag                                       - Operation flag
 *  @li@c  b_adjust_vdp                                    - when ui4_op_flag ==MHEG5_STOP_SVC_PARAM_ADJ_VDP, indicate if need adjust vdp
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32          ui4_op_flag;
    BOOL            b_adjust_vdp;
} MHEG5_STOP_SVC_PARAM_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_VIDEO_UPDATE_NFY_T
 *  @brief Notification when video update
 *  @code
 *  typedef struct
 *  {
 *      UINT32                  ui4_nfy_flag;
 *      BOOL                    b_freeze;
 *      MHEG5_IFRAME_SATTE_T    e_iframe_state;
 *      BOOL                    b_internal_scrn_mode;
 *  } MHEG5_VIDEO_UPDATE_NFY_T;
 *  @endcode
 *  @li@c  ui4_nfy_flag                                    - Notification flag
 *  @li@c  b_freeze                                          - Video froze or unfroze
 *  @li@c  e_iframe_state                                 - Iframe played or stopped
 *  @li@c  b_internal_scrn_mode                       - Internal screen mode or not
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32                  ui4_nfy_flag;
    
    BOOL                    b_freeze;
    MHEG5_IFRAME_SATTE_T    e_iframe_state;
    BOOL                    b_internal_scrn_mode;
} MHEG5_VIDEO_UPDATE_NFY_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_DVB_LOCATOR_T
 *  @brief Locate to s specified service
 *  @code
 *  typedef struct 
 *  {
 *      UINT16 ui2_on_id;
 *      UINT16 ui2_ts_id;
 *      UINT16 ui2_svc_id;
 *      INT32  i4_comp_tag;
 *  } MHEG5_DVB_LOCATOR_T;
 *  @endcode
 *  @li@c  ui2_on_id                                        - Original network ID
 *  @li@c  ui2_ts_id                                         - Transport stream ID
 *  @li@c  ui2_svc_id                                       - Service ID
 *  @li@c  i4_comp_tag                                    - Component Tag
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    UINT16 ui2_on_id;
    UINT16 ui2_ts_id;
    UINT16 ui2_svc_id;
    INT32  i4_comp_tag;
} MHEG5_DVB_LOCATOR_T;

/*------------------------------------------------------------------*/
/*! @brief Callback AP to tune to specified service
 *  @param [in] i4_svc_id   - Service index
 *  @param [out] pb_status -True: The specified service is available.
 *                                         False: The specified service is not available.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_tune_svc_fct)(
    INT32 i4_svc_id,
    VOID *pv_tag,
    BOOL *pb_status);

/*------------------------------------------------------------------*/
typedef enum
{
   MHEG5_PIN_SUPPORT_NONE,
   MHEG5_PIN_SUPPORT_DISABLED,
   MHEG5_PIN_SUPPORT_ENABLED
} MHEG5_ICS_PIN_SUPPORT_T;

typedef INT32 (*x_mheg5_pin_support_fct)(
    VOID *                      pv_tag,
    MHEG5_ICS_PIN_SUPPORT_T *   pb_status);
/*------------------------------------------------------------------*/
/*! @brief Callback AP to tune to specified service
 *  @param [in] i4_svc_id   - Service index
 *  @param [out] pb_status -True: The specified service is available.
 *                                         False: The specified service is not available.
 */
/*------------------------------------------------------------------*/
typedef enum
{
   MHEG5_AUDIO_DESC_NORMAL,
   MHEG5_AUDIO_DESC_HEARING_IMPAIRED,
   MHEG5_AUDIO_DESC_VISUALLY_IMPAIRED
} MHEG5_ICS_AUDIO_DESC_T;

typedef INT32 (*x_mheg5_ad_fct)(
    VOID *                  pv_tag,
    MHEG5_ICS_AUDIO_DESC_T *pb_status);
/*! @brief Callback AP of the tune method
 *  @param [in] e_tune_method   - 0: Subsequent tuning shall be "normal"
 *                                1: No banner, A/V stops and after is restarted, original channel info
 *                                2: No banner, A/V continues, new channel info
 *                                3: No banner, A/V continues, original channel info.
 *  @param [out] None.
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_TUNE_METHOD_T
{
    MHEG5_TUNE_ALL_NORMALLY        = 0x00,
    MHEG5_TUNE_ALL_QUIETLY         = 0x01,
    MHEG5_TUNE_QUIET_THEN_NORMAL   = 0x02,
    MHEG5_TUNE_COMPLETELY_QUIET    = 0x03
    
} MHEG5_TUNE_METHOD_T;

typedef VOID (*x_mheg5_tune_method_update_nfy)(
    MHEG5_TUNE_METHOD_T e_tune_method,
    VOID*               pv_tag);

/*------------------------------------------------------------------*/
/*! @struct MHEG5_SETUP_T
 *  @brief MHEG5 setup structure
 *  @code
 *  typedef struct
 *  {
 *      UINT8 ui1_ver;
 *      VOID*  pv_mem_rgn;
 *      SIZE_T z_mem_rgn;
 *      UINT8  ui1_task_pri;
 *      UINT32 ui4_sec_buf_num;
 *      BOOL   b_sub_with_gfx;
 *      CHAR   s_dvp_name[10];
 *      x_mheg5_tune_svc_nfy pf_tune_svc_nfy;
 *      x_mheg5_tune_method_update_nfy pf_tune_method_update_nfy;
 *  } MHEG5_SETUP_T;
 *  @endcode
 *  @li@c  ui1_ver                                            - Version
 *  @li@c  pv_mem_rgn                                   - Pointer to the memory region
 *  @li@c  z_mem_rgn                                     - Size of the mmeory region
 *  @li@c  ui1_task_pri                                    - MHEG5 task priority
 *  @li@c  ui4_sec_buf_num                            - Number of the section buffers
 *  @li@c  b_sub_with_gfx                               - Indicates if DVB subtitles and MHEG5 Visibles can displayed simultaneously
 *  @li@c  s_dvp_name[10]                             - Null terminated hardware, software and manufacturer specific
 *  @li@c  pf_tune_svc_nfy                              - Callback function for tuning service
 *  @li@c  pf_tune_method_update_nfy            - Callback function for updating tune method
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8 ui1_ver;

    VOID*  pv_mem_rgn;
    SIZE_T z_mem_rgn;
    UINT8  ui1_task_pri;
    UINT32 ui4_sec_buf_num;
    BOOL   b_sub_with_gfx;
    CHAR   s_dvp_name[10];

} MHEG5_SETUP_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_START_EXEC_T
 *  @brief Start MHEG5 component instance
 *  @code
 *  typedef struct
 *  {
 *      UINT8 ui1_ver;
 *      MHEG5_DVB_LOCATOR_T* pt_dvb_loc;
 *  } MHEG5_START_EXEC_T;
 *  @endcode
 *  @li@c  ui1_ver                                           - Version
 *  @li@c  pt_dvb_loc                                      - DVB locator
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8 ui1_ver;
    
    MHEG5_DVB_LOCATOR_T* pt_dvb_loc;
} MHEG5_START_EXEC_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_STOP_EXEC_T
 *  @brief Stop MHEG5 component instance
 *  @code
 *  typedef struct
 *  {
 *      UINT8 ui1_ver;
 *      BOOL  b_clean;
 *  } MHEG5_STOP_EXEC_T;
 *  @endcode
 *  @li@c  ui1_ver                                           - Version
 *  @li@c  b_clean                                          - Clean cached DSMCC data nor not
 */
/*------------------------------------------------------------------*/
/* Values given to c_mheg5_stop_execution for 'action', which can be bitwise OR'ed */
/*      
 * MHEG5_STOP_ACTION_CLEAR_DSMCC:
 *       Unloads DSM-CC carousel(s) and any data in SSF cache. This should not be
 *       set when setting MHEG5_STOP_ACTION_MHEG_TUNE to indicating an MHEG tune.
 *
 * MHEG5_STOP_ACTION_MHEG_TUNE:
 *       Indicates to MHEG engine that this call is a direct result of a call by
 *       the engine to tmMHEG5TuneIndex(). This bit must be set in this
 *       circumstance, so that Voyager can support the LifecycleExtension.
 */
#define MHEG5_STOP_ACTION_CLEAR_DSMCC  (0x01)
#define MHEG5_STOP_ACTION_MHEG_TUNE    (0x02)

typedef struct
{
    UINT8 ui1_ver;
    
    UINT8 ui1_acts;
} MHEG5_STOP_EXEC_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_PAUSE_EXEC_T
 *  @brief Active/Inactive MHEG5 component instance
 *  @code
 *  typedef struct
 *  {
 *      UINT8 ui1_ver;
 *      BOOL  b_on;
 *  } MHEG5_PAUSE_EXEC_T;
 *  @endcode
 *  @li@c  ui1_ver                                          - Version
 *  @li@c  b_on                                              - Active or inactive
 */
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8 ui1_ver;
    
    BOOL  b_on;
} MHEG5_PAUSE_EXEC_T;

typedef struct _MHEG5_VID_INFO_NFY_T
{

}MHEG5_VID_INFO_UPDATE_NFY_T;

typedef struct _MHEG5_VID_INFO_T
{
    BOOL                        b_mheg5_running;
    UINT8                       ui1_afd;
    MHEG5_ASPECT_RATIO_T        e_src_aspect_ratio;
    MHEG5_WAM_T                 e_wam;
    MHEG5_ASPECT_RATIO_T        e_scene_ar;

}MHEG5_VID_INFO_T;

#ifdef CI_PLUS_SUPPORT
/*------------------------------------------------------------------*/
/*! @enum MHEG5_PRGS_TYPE_T
 *  @brief Notify APP of the MHEG5 component progress
 *  @code
 *  typedef enum
 *  {
 *      MHEG5_PRGS_CI_APPLICATION_START_FAILED,
 *      MHEG5_PRGS_CI_APPLICATION_STOPPED,
 *      MHEG5_PRGS_CI_APPLICATION_STARTED
 *  } MHEG5_IFRAME_SATTE_T;
 *  @endcode
 *  @li@c  MHEG5_PRGS_CI_APPLICATION_START_FAILED                        - Start CI application failed
 *  @li@c  MHEG5_PRGS_CI_APPLICATION_STOPPED                                 - CI application stopped
 *  @li@c  MHEG5_PRGS_CI_APPLICATION_STARTED                                 - CI application started
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MHEG5_PRGS_CI_APPLICATION_START_FAILED = 0,

    MHEG5_PRGS_CI_APPLICATION_STOPPED,

    MHEG5_PRGS_CI_APPLICATION_STARTED,

} MHEG5_PRGS_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_CI_FILE_ACK_T
 *  @brief The MMI FileAcknowledge message, which is a reply from the CI module to an MMI FileRequest sent using tmMHEG5CiFileRequest.
 *  @code
 * typedef struct _MHEG5_CI_FILE_ACK_T
 * {
 *      BOOL              b_file_ok;
 *      UINT8*            pui1_file_data;
 *      UINT32            ui4_file_size;
 *      VOID*             pv_tag;
 * } MHEG5_CI_FILE_ACK_T;
 *  @endcode
 *  @li@c  b_file_ok                                        - OK?
 *  @li@c  pui1_file_data                                - File data
 *  @li@c  ui4_file_size                                   - File size 
 *  @li@c  pv_tag                                           - Tag
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_CI_FILE_ACK_T
{
    BOOL              b_file_ok;
    UINT8*            pui1_file_data;
    UINT32            ui4_file_size;

    VOID*             pv_tag;
} MHEG5_CI_FILE_ACK_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_CI_START_APP_T
 *  @brief Used in x_mheg5_start_ci_application()
 *  @code
 *  typedef struct _MHEG5_CI_START_APP_T
 *  {
 *      UINT8             ui1_ver;
 *      const CHAR*       s_ci_app_name;
 *  } MHEG5_CI_START_APP_T;
 *  @endcode
 *  @li@c  ui1_ver                                        - Version
 *  @li@c  s_ci_app_name                            - CI application name
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_CI_START_APP_T
{
    UINT8             ui1_ver;
    
    const CHAR*       s_ci_app_name;

} MHEG5_CI_START_APP_T;

/*------------------------------------------------------------------*/
/*! @struct MHEG5_CI_START_APP_T
 *  @brief Used in x_mheg5_stop_ci_applications()
 *  @code
 * typedef struct _MHEG5_CI_STOP_APPS_T
 * {
 *     UINT8             ui1_ver; 
 * } MHEG5_CI_STOP_APPS_T;
 *  @endcode
 *  @li@c  ui1_ver                                        - Version
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_CI_STOP_APPS_T
{
    UINT8             ui1_ver;

} MHEG5_CI_STOP_APPS_T;

#define MHEG5_CI_NFY_COMP_PRGS    ((UINT32) 0x0) /**<Data flags for MHEG5 CI notifications         */

/*------------------------------------------------------------------*/
/*! @struct MHEG5_CI_NFY_T
 *  @brief Notifications to APP 
 *  @code
 * typedef struct _MHEG5_CI_NFY_T
 * {
 *     UINT32   ui4_nfy_flag;
 *     union
 *     {
 *         MHEG5_PRGS_TYPE_T     e_prgs;
 *     } u;
 * } MHEG5_CI_NFY_T;
 *  @endcode
 *  @li@c  ui4_nfy_flag                                - Notification flag 
 *  @li@c  e_prgs                                        - MHEG5 component progress
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_CI_NFY_T
{
    UINT32   ui4_nfy_flag;

    union
    {
        MHEG5_PRGS_TYPE_T     e_prgs;
    } u;

} MHEG5_CI_NFY_T;

/*------------------------------------------------------------------*/
/*! @brief Callback to notify APP of the CI application status
 *  @param [in] pt_data - Notification of the CI application status
 *  @param [out] None.
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_ci_nfy)(MHEG5_CI_NFY_T* pt_data, VOID* pv_tag);

#endif

/*------------------------------------------------------------------*/
/*! @enum MHEG5_PROFILE_ID_T
 *  @brief MHEG5 profile ID
 *  @code
 * typedef enum _MHEG5_PROFILE_ID_T
 * {
 *     MHEG5_PROFILE_UK,
 *     MHEG5_PROFILE_NZ,
 *     MHEG5_PROFILE_NONE
 * } MHEG5_PROFILE_ID_T;
 *  @li@c  MHEG5_PROFILE_UK                        - MHEG5 UK profile
 *  @li@c  MHEG5_PROFILE_NZ                        - MHEG5 NZ profile
 *  @li@c  MHEG5_PROFILE_NONE                    - None
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_PROFILE_ID_T
{
    MHEG5_PROFILE_UK,
    MHEG5_PROFILE_NZ,
    MHEG5_PROFILE_IRL,
    MHEG5_PROFILE_AUS,
    MHEG5_PROFILE_HK,
    
    MHEG5_PROFILE_NONE
} MHEG5_PROFILE_ID_T;

#ifdef MHEG5_IC_SUPPORT
/* MHEG5 Interaction Channel Status */
typedef enum _MHEG5_IC_STATE_T
{
    MHEG5_IC_STATE_ACTIVE,
    MHEG5_IC_STATE_INACTIVE,
    MHEG5_IC_STATE_DISABLED
} MHEG5_IC_STATE_T;
#endif

/* For simultaneous MHEG5 and subtilte */
typedef struct _MHEG5_SUBTITLE_CFG_T
{
    BOOL             b_on;
    ISO_639_LANG_T   s639_lang_1st;
    ISO_639_LANG_T   s639_lang_2nd;
    DVB_SBTL_TYPE_T  e_type;
} MHEG5_SUBTITLE_CFG_T;

/*------------------------------------------------------------------*/
/*! @enum MHEG5_SBTL_STATE_T
 *  @brief Notify APP of subtitle's states reuired by MHEG5
 *  @code
 *typedef enum _MHEG5_SBTL_STATE_T
 *{
 *   MHEG5_SBTL_HIDE,
 *   MHEG5_SBTL_SHOW,
 *    MHEG5_SBTL_START,
 *    MHEG5_SBTL_STOP
 *}MHEG5_SBTL_STATE_T;

 *  @endcode
 *  @  MHEG5_SBTL_HIDE                        - MHEG5 wants to hide subtitle
 *  @  MHEG5_SBTL_SHOW                       - MHEG5 wants to show subtitle, but this behavior related to user's setup
 *  @  MHEG5_SBTL_START                      - MHEG5 request subtitle module to playback subtitle stream
 *  @  MHEG5_SBTL_STOP                        - MHEG5 request subtitle module to stop subtitle stream
 */
/*------------------------------------------------------------------*/
typedef enum _MHEG5_SBTL_STATE_T
{
    MHEG5_SBTL_HIDE,
    MHEG5_SBTL_SHOW,
    MHEG5_SBTL_START,
    MHEG5_SBTL_STOP
}MHEG5_SBTL_STATE_T;

typedef INT32 (*x_mheg5_eng_set_sbtl_cfg_info)(
    MHEG5_SUBTITLE_CFG_T* pt_cfg);


/*------------------------------------------------------------------*/
/*! @brief Callback to notify the GUI states
 *  @param [in] ui4_msg - MHEG5_MSG_GRD_RENDERED, MHEG5_MSG_GRD_KEY_DOWN, 
 *                                      or MHEG5_MSG_GRD_DRAWN 
 *  @param [in] pv_param - Parameters
 *  @param [out] None.
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_mheg5_eng_state_nfy_fct)(
    UINT32  ui4_msg, 
    VOID*   pv_param,
    VOID*   pv_tag);

/*------------------------------------------------------------------*/
/*! @brief Callback to notify the SI loading status
 *  @param [in] e_cond - MHEG5 notify condition
 *  @param [in] pv_tag - User's tag
 *  @param [out] None.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_load_nfy_fct)(
    MHEG5_NFY_COND_T    e_cond,
    VOID*               pv_tag);

/*------------------------------------------------------------------*/
/*! @brief Callback to notify the PID/PCR PID resolved
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_mheg5_resolve_nfy)(
    BOOL            b_found, 
    MPEG_2_PID_T    t_pid, 
    MPEG_2_PID_T    t_pcr_pid,
    UINT8           ui1_strm_type,
    VOID*           pv_tag);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Init osd
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_init_osd)(
    HANDLE_T               h_mheg5_canvas,
    HANDLE_T               h_sbtl_canvas,
    GL_HPALETTE_T          h_palette,
    const GL_COLOR_T*      pt_color_key);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Load SI
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_load_si)(
    HANDLE_T                    h_svctx,
    UINT16                      ui2_svl_id,
    UINT16                      ui2_svl_rec_id,
    x_mheg5_eng_load_nfy_fct    pf_load_nfy,
    VOID*                       pv_tag);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Process message
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_msg_proc_fct)(MHEG5_MSG_T* pt_msg);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Get display adjustment
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_get_display_adjustment)(
    UINT8                       ui1_afd,
    MHEG5_ASPECT_RATIO_T        e_src_aspect_ratio,
    SCC_VID_SCREEN_MODE_T       e_screen_mode,
    MHEG5_VIDEO_MODE_T          e_vid_mode,
    VSH_REGION_INFO_T*          pt_src_region, 
    VSH_REGION_INFO_T*          pt_dst_region);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Query display adjustment
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_query_display_adjustment)(
    const MHEG5_DISPLAY_PARAM_T*    pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Query display adjustment
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_query_display_adjustment_ex)(
    const MHEG5_DISPLAY_PARAM_T*    pt_dspy_param,
    VSH_REGION_INFO_T*              pt_src_region, 
    VSH_REGION_INFO_T*              pt_dst_region);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Query display adjustment
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef BOOL (*x_mheg5_eng_query_display_b_auto_mode)(VOID);


/*------------------------------------------------------------------*/
/*! @brief Engine function: Evaluate audio and video
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_evaluate)(VOID);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Query the PID and PCR PID
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_resolve)(
    MHEG5_STREAM_DESC_T*        pt_strm_desc,
    x_mheg5_resolve_nfy         pf_resolve_nfy,
    VOID*                       pv_tag,
    STREAM_PID_INFO_T*          pt_pid_info);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Show MHEG5's UI
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_show)(
    MHEG5_COEXIST_DISPLAY_T     e_display,
    BOOL                        b_get_focus);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Adjust OSD
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_adjust_osd)(
    const MHEG5_CNV_CFG_T*      pt_cnv_cfg);

typedef INT32 (*x_mheg5_eng_adjust_osd_ex)(
    const MHEG5_CNV_CFG_T*        pt_cnv_cfg,
    const VSH_REGION_INFO_T*      pt_orig_src_reg,
    const VSH_REGION_INFO_T*      pt_full_src_reg,
    const VSH_REGION_INFO_T*      pt_full_dst_reg,
    const VSH_REGION_INFO_T*      pt_xlat_src_reg,
    const VSH_REGION_INFO_T*      pt_xlat_dst_reg);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Pause MHEG5 service
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_pause_service)(VOID);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Resume MHEG5 service
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_resume_service)(VOID);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Set default audio language
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_set_def_lang)(
    ISO_639_LANG_T s639_lang);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Start MHEG5 service
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_start_service)(
    HANDLE_T                    h_svctx,
    UINT16                      ui2_svl_id,
    UINT16                      ui2_svl_rec_id);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Stop MHEG5 service
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_stop_service)(MHEG5_STOP_SVC_PARAM_T *pt_param);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Unload SI
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_unload_si)(VOID);

/*------------------------------------------------------------------*/
/*! @brief Callback to notify the video update info
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef VOID  (*x_mheg5_video_update_nfy)(MHEG5_VIDEO_UPDATE_NFY_T *pt_data, VOID* pv_tag);

/*------------------------------------------------------------------*/
/*! @brief Callback to notify the video update info
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef VOID  (*x_mheg5_vid_info_update_nfy)(MHEG5_VID_INFO_UPDATE_NFY_T *pt_data, VOID* pv_tag);


/*------------------------------------------------------------------*/
/*! @brief Callback to notify the subtitle visibility info
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef VOID  (*x_mheg5_sbtl_visibility_nfy)(MHEG5_SBTL_STATE_T e_state, VOID* pv_tag);

/*------------------------------------------------------------------*/
/*! @brief Callback to notify the overscan setting info
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_comp_set_overscan_nfy)(SCC_VID_OVER_SCAN_CLIPPER_T *pt_clipper);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Set vdp mode
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef VOID  (*x_mheg5_eng_set_vdp_mode)(SCC_VID_MODE_T e_mode, BOOL b_resume);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Set video info
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef VOID  (*x_mheg5_eng_set_video_info)(VOID);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Set MHEG5 UI visibility
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_set_visibility_func)(WGL_SW_CMD_T e_sw_cmd);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Update screen mode
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef VOID  (*x_mheg5_eng_update_scr_mode)(SCC_VID_SCREEN_MODE_T e_screen_mode);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Adjust freeze mode
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_adjust_freeze_mode)(VOID);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Set the default dvb locator
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_set_dvb_locator)(UINT16 ui2_on_id, UINT16 ui2_ts_id, UINT16 ui2_svc_id);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Adjust the Z-order of MHEG5 UI frame 
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_mheg5_eng_adjust_order)(HANDLE_T h_ref_widget, WGL_INSERT_MODE_T e_insert);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Open MHEG5 component
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_open)(MHEG5_SETUP_T* pt_setup);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Close MHEG5 component
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_close)(VOID);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Process key press
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_key_press)(UINT32 ui4_key_code, BOOL*  pb_in_func_grp, BOOL b_process);
    
/*------------------------------------------------------------------*/
/*! @brief Engine function: Start MHEG5 component
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_start_execution)(MHEG5_START_EXEC_T* pt_start_exec);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Stop MHEG5 component
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_stop_execution)(MHEG5_STOP_EXEC_T* pt_stop_exec);
    
/*------------------------------------------------------------------*/
/*! @brief Engine function: Pause/Resume MHEG5 component
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_pause_execution)(MHEG5_PAUSE_EXEC_T* pt_pause_exec);

#ifdef CI_PLUS_SUPPORT

typedef INT32 (*x_mheg5_eng_reg_ci_nfy)(x_mheg5_ci_nfy pf_nfy, VOID* pv_tag);

/*------------------------------------------------------------------*/
/*! @brief Engine function: start CI application
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_start_ci_app)(MHEG5_CI_START_APP_T* pt_start_ci_app);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Stop CI appliaction
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_stop_ci_apps)(MHEG5_CI_STOP_APPS_T*  pt_stop_ci_apps);

#endif

/*------------------------------------------------------------------*/
/*! @brief Engine function: notify AP weather the guidence should be showed or not
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
#ifdef MHEG5_IC_SUPPORT
typedef INT32 (*x_mheg5_ics_guid_visibility_nfy)(UINT8 *p_restriction, BOOL b_show,VOID* pv_tag);
#endif

/*------------------------------------------------------------------*/
/*! @brief Callback to notify APP of the video info update
 *  @param [in] pt_data - Notification of the CI application status
 *  @param [out] None.
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_vid_info_update_nfy)(x_mheg5_vid_info_update_nfy pf_func, VOID* pv_tag);

typedef INT32 (*x_mheg5_eng_get_vid_info)(MHEG5_VID_INFO_T*  pt_vid_info);

/*------------------------------------------------------------------*/
/*! @brief Engine function: Apply supported MHEG5 profile for the specific country
 *  @note Internel use
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_mheg5_eng_apply_supported_prof)(ISO_3166_COUNT_T s3166_country, MHEG5_PROFILE_ID_T* pe_profile_id, BOOL b_apply);

typedef INT32 (*x_mheg5_eng_reg_eng_state_nfy)(x_mheg5_eng_state_nfy_fct pf_nfy, VOID* pv_tag);

typedef INT32 (*x_mheg5_eng_reg_video_update_nfy)(x_mheg5_video_update_nfy pf_nfy, VOID* pv_tag);

typedef INT32 (*x_mheg5_eng_reg_sbtl_visibility_nfy)(x_mheg5_sbtl_visibility_nfy pf_nfy, VOID* pv_tag);

typedef INT32 (*x_mheg5_eng_reg_tune_method_update_nfy)(x_mheg5_tune_method_update_nfy pf_nfy, VOID* pv_tag);

typedef INT32 (*x_mheg5_eng_reg_tune_svc_cb_fct)(x_mheg5_tune_svc_fct pf_fct, VOID* pv_tag);

#ifdef MHEG5_IC_SUPPORT
typedef INT32 (*x_mheg5_eng_set_ic_status)(MHEG5_IC_STATE_T  e_new_ic_status);

typedef INT32 (*x_mheg5_eng_reg_ics_guid_nfy)(x_mheg5_ics_guid_visibility_nfy pf_nfy, VOID* pv_tag);

typedef INT32 (*x_mheg5_eng_set_ics_guid_result)(BOOL b_result);

typedef INT32 (*x_mheg5_eng_reg_pin_support_cb_fct)(x_mheg5_pin_support_fct pf_fct, VOID* pv_tag);
typedef INT32 (*x_mheg5_eng_notify_ad_pref_changed)(VOID);
typedef INT32 (*x_mheg5_eng_reg_ad_cb_fct)(x_mheg5_ad_fct pf_fct, VOID* pv_tag);

#endif

/*------------------------------------------------------------------*/
/*! @struct MHEG5_FUNC_TBL_T
 *  @brief MHEG5 engine function table
 *  @code
 *  typedef struct _MHEG5_FUNC_TBL_T
 *  {
 *      x_mheg5_eng_load_si                     pf_load_si;
 *      x_mheg5_eng_unload_si                   pf_unload_si;
 *      x_mheg5_eng_evaluate                    pf_evaluate;
 *      x_mheg5_eng_get_display_adjustment      pf_get_display_adjustment;
 *      x_mheg5_eng_query_display_adjustment    pf_query_display_adjustment;
 *      x_mheg5_eng_resolve                     pf_resolve;
 *      x_mheg5_eng_start_service               pf_start_service;
 *      x_mheg5_eng_resume_service              pf_resume_service;
 *      x_mheg5_eng_stop_service                pf_stop_service;
 *      x_mheg5_eng_pause_service               pf_pause_service;
 *      x_mheg5_eng_init_osd                    pf_init_osd;
 *      x_mheg5_eng_show                        pf_show;
 *      x_mheg5_eng_adjust_osd                  pf_adjust_osd;
 *      x_mheg5_eng_set_def_lang                pf_set_def_lang;
 *      x_mheg5_eng_set_def_lang                pf_set_gui_lang;
 *      x_mheg5_eng_msg_proc_fct                pf_msg_proc;
 *      x_mheg5_eng_set_vdp_mode                pf_set_vdp_mode;
 *      x_mheg5_eng_set_video_info              pf_set_video_info;
 *      x_mheg5_set_visibility_func             pf_set_visibility;
 *      x_mheg5_eng_update_scr_mode             pf_update_screen_mode;
 *      x_mheg5_eng_adjust_freeze_mode          pf_adjust_freeze_mode;
 *      x_mheg5_eng_set_dvb_locator             pf_set_dvb_locator;
 *      x_mheg5_eng_adjust_order                pf_adjust_order;
 *      x_mheg5_eng_open                        pf_mheg5_open;
 *      x_mheg5_eng_close                       pf_mheg5_close;
 *      x_mheg5_eng_key_press                   pf_key_press;
 *      x_mheg5_eng_start_execution             pf_start_exec;
 *      x_mheg5_eng_stop_execution              pf_stop_exec;
 *      x_mheg5_eng_pause_execution             pf_pause_exec;
 *      x_mheg5_eng_start_ci_app                pf_start_ci_app;
 *      x_mheg5_eng_stop_ci_apps                pf_stop_ci_apps;
 *      x_mheg5_eng_apply_supported_prof        pf_apply_supported_prof;
 *  } MHEG5_FUNC_TBL_T;
 *  @endcode
 *  @li@c  pf_load_si                                          - Engine function: Load SI
 *  @li@c  pf_unload_si                                      - Engine function: Unload SI
 *  @li@c  pf_evaluate                                       - Engine function: Evaluate audio and video
 *  @li@c  pf_get_display_adjustment                - Engine function: Get display adjustment
 *  @li@c  pf_query_display_adjustment            - Engine function: Query display adjustment
 *  @li@c  pf_resolve                                        - Engine function: Query the PID and PCR PID
 *  @li@c  pf_start_service                                - Engine function: Start MHEG5 service
 *  @li@c  pf_resume_service                            - Engine function: Resume MHEG5 service
 *  @li@c  pf_stop_service                                - Engine function: Stop MHEG5 service
 *  @li@c  pf_pause_service                             - Engine function: Pause MHEG5 service
 *  @li@c  pf_init_osd                                       - Engine function: Init osd
 *  @li@c  pf_show                                           - Engine function: Show MHEG5's UI
 *  @li@c  pf_adjust_osd                                   - Engine function: Adjust OSD
 *  @li@c  pf_set_def_lang                                - Engine function: Set default audio language
 *  @li@c  pf_set_gui_lang                                 -Engine function: Set gui language
 *  @li@c  pf_msg_proc                                     - Engine function: Process message
 *  @li@c  pf_set_vdp_mode                              - Engine function: Set vdp mode
 *  @li@c  pf_set_video_info                               - Engine function: Set video info
 *  @li@c  pf_set_visibility                                 - Engine function: Set MHEG5 UI visibility
 *  @li@c  pf_update_screen_mode                     - Engine function: Update screen mode
 *  @li@c  pf_adjust_freeze_mode                      - Engine function: Adjust freeze mode
 *  @li@c  pf_set_dvb_locator                             - Engine function: Set the default dvb locator
 *  @li@c  pf_adjust_order                                  - Engine function: Adjust the Z-order of MHEG5 UI frame 
 *  @li@c  pf_mheg5_open                                  - Engine function: Open MHEG5 component
 *  @li@c  pf_mheg5_close                                  - Engine function: Close MHEG5 component
 *  @li@c  pf_key_press                                      - Engine function: Process key press
 *  @li@c  pf_start_exec                                     - Engine function: Start MHEG5 component
 *  @li@c  pf_stop_exec                                     - Engine function: Stop MHEG5 component
 *  @li@c  pf_pause_exec                                   - Engine function: Pause/Resume MHEG5 component
 *  @li@c  pf_start_ci_app                                  - Engine function: Start CI application
 *  @li@c  pf_stop_ci_apps                                 - Engine function: Stop CI application
 *  @li@c  pf_apply_supported_prof                     - Engine function: Apply supported MHEG5 profile for the specific country
 */
/*------------------------------------------------------------------*/
typedef struct _MHEG5_FUNC_TBL_T
{
    x_mheg5_eng_load_si                     pf_load_si;
    x_mheg5_eng_unload_si                   pf_unload_si;
    x_mheg5_eng_evaluate                    pf_evaluate;
    x_mheg5_eng_get_display_adjustment      pf_get_display_adjustment;
    x_mheg5_eng_query_display_adjustment    pf_query_display_adjustment;
    x_mheg5_eng_query_display_adjustment_ex pf_query_display_adjustment_ex;
    x_mheg5_eng_query_display_b_auto_mode   pf_query_display_b_auto_mode;
    x_mheg5_eng_resolve                     pf_resolve;
    x_mheg5_eng_start_service               pf_start_service;
    x_mheg5_eng_resume_service              pf_resume_service;
    x_mheg5_eng_stop_service                pf_stop_service;
    x_mheg5_eng_pause_service               pf_pause_service;
    x_mheg5_eng_init_osd                    pf_init_osd;
    x_mheg5_eng_show                        pf_show;
    x_mheg5_eng_adjust_osd                  pf_adjust_osd;
    x_mheg5_eng_adjust_osd_ex               pf_adjust_osd_ex;
    x_mheg5_eng_set_def_lang                pf_set_def_lang;
    x_mheg5_eng_set_def_lang                pf_set_gui_lang;
    x_mheg5_eng_msg_proc_fct                pf_msg_proc;
    x_mheg5_eng_set_vdp_mode                pf_set_vdp_mode;
    x_mheg5_eng_set_video_info              pf_set_video_info;
    x_mheg5_set_visibility_func             pf_set_visibility;
    x_mheg5_eng_update_scr_mode             pf_update_screen_mode;
    x_mheg5_eng_adjust_freeze_mode          pf_adjust_freeze_mode;
    x_mheg5_eng_set_dvb_locator             pf_set_dvb_locator;
    x_mheg5_eng_adjust_order                pf_adjust_order;
    x_mheg5_eng_open                        pf_mheg5_open;
    x_mheg5_eng_close                       pf_mheg5_close;
    x_mheg5_eng_key_press                   pf_key_press;
    x_mheg5_eng_start_execution             pf_start_exec;
    x_mheg5_eng_stop_execution              pf_stop_exec;
    x_mheg5_eng_pause_execution             pf_pause_exec;
    x_mheg5_eng_reg_eng_state_nfy           pf_reg_eng_state_nfy;
    x_mheg5_eng_reg_video_update_nfy        pf_reg_video_update_nfy;
    x_mheg5_eng_reg_sbtl_visibility_nfy     pf_reg_sbtl_visibility_nfy; /** to suport simultaneous MHEG5 and subtitle*/
    x_mheg5_eng_set_sbtl_cfg_info           pf_set_sbtl_cfg_info;
    x_mheg5_eng_reg_tune_method_update_nfy  pf_reg_tune_method_update_nfy;
    x_mheg5_eng_reg_tune_svc_cb_fct         pf_reg_tune_svc_cb_fct;
    
#ifdef CI_PLUS_SUPPORT
    x_mheg5_eng_reg_ci_nfy                  pf_reg_ci_nfy;
    x_mheg5_eng_start_ci_app                pf_start_ci_app;
    x_mheg5_eng_stop_ci_apps                pf_stop_ci_apps;
#endif
    x_mheg5_eng_apply_supported_prof        pf_apply_supported_prof;
#ifdef MHEG5_IC_SUPPORT
    x_mheg5_eng_set_ic_status               pf_set_ic_status;
    x_mheg5_eng_reg_ics_guid_nfy            pf_reg_ics_guid_visibility_nfy;
    x_mheg5_eng_set_ics_guid_result         pf_set_ics_guid_result;
    x_mheg5_eng_reg_pin_support_cb_fct      pf_reg_pin_support_cb_fct;
    x_mheg5_eng_notify_ad_pref_changed      pf_notify_audio_desc_pref_changed;
    x_mheg5_eng_reg_ad_cb_fct               pf_reg_ad_cb_fct;
#endif/*MHEG5_IC_SUPPORT*/
    x_mheg5_eng_vid_info_update_nfy         pf_reg_vid_info_update_nfy;
    x_mheg5_eng_get_vid_info                pf_get_vid_info;
} MHEG5_FUNC_TBL_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

/*! @}  */

#endif /* _U_MHEG5_H_ */


