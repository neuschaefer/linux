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
/*! @file u_cap.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all capture library API and constant for 
 *         other module using.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CAP Capture Logo
 *
 *  @ingroup groupMW
 *  @brief Provides external function TV Application
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_CAP_H_
#define _U_CAP_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "x_vid_plane.h"
#include "u_img.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#define HT_CAP_TYPE           (HT_GROUP_CAP + 1)

/* Type Define For Capture Source *******************************/
typedef enum
{
    CAP_SRC_TYPE_DEFAULT = 0,
    CAP_SRC_TYPE_TV_VIDEO,
    CAP_SRC_TYPE_MM_VIDEO,
    CAP_SRC_TYPE_MM_IMAGE,
    CAP_SRC_TYPE_MM_IMAGE_ANDROID
}   CAP_SRC_TYPE_T;

/* Event Define For Capture Operation *******************************/
typedef enum
{
    CAP_EVENT_TYPE_OPEN_DONE = 1,
    CAP_EVENT_TYPE_OPEN_ERROR,
    CAP_EVENT_TYPE_CAP_DONE,
    CAP_EVENT_TYPE_CAP_ERR,
    CAP_EVENT_TYPE_SAVE_DONE,
    CAP_EVENT_TYPE_SAVE_ERROR,
    CAP_EVENT_TYPE_DO_CAPTURING,
    CAP_EVENT_TYPE_DO_SAVING 
    /*to be extend*/
}   CAP_EVENT_TYPE_T;

/* Event Define For Capture Format *******************************/
typedef enum
{
    CAP_IMG_FMT_TYPE_DEFAULT = 0,
    CAP_IMG_FMT_TYPE_MPEG,
    CAP_IMG_FMT_TYPE_JPEG,
    CAP_IMG_FMT_TYPE_RAW
    /*to be extend*/ 
}   CAP_IMG_FMT_TYPE_T;


/* Event Define For Capture Output Resolution *******************************/
typedef enum
{
    CAP_OUT_RES_TYPE_SD = 0,
    CAP_OUT_RES_TYPE_HD,
    CAP_OUT_RES_TYPE_USER
    /*to be extend*/ 
}   CAP_OUT_RES_TYPE_T;


/* Capture information definition *******************************/
typedef struct _CAP_CAPTURE_INFO_T
{
    CAP_IMG_FMT_TYPE_T      e_format;  
    UINT32                  ui4_quality;
    UINT32                  ui4_max_size;
    HANDLE_T                h_src_surf;/* For capture photo use*/
    HANDLE_T                h_working_surf;/* For capture photo use*/
    CAP_OUT_RES_TYPE_T      e_res_type;    
    UINT8                   ui1_video_path;
#ifdef APP_CAPTURE_SPECIAL_AREA_SUPPORT
    GL_RECT_T               t_rec;
    UINT32                  ui4_moveable_width;
    UINT32                  ui4_moveable_height;
#endif
	//for android image
	UINT32	ui4_image_width;
	UINT32	ui4_image_height;
	UINT32	ui4_image_pitch;
	CAP_COLOR_FORMAT_T	e_colormode;
	INT32		i4_shm_id;
    union 
    {/* used for IMG_CAP_RESOLUTION(HW & SW) */        
        struct 
        {
            UINT32  ui4_res_width;
            UINT32  ui4_res_height;
        } t_resolution;
    }u;
} CAP_CAPTURE_INFO_T;


typedef struct _CAP_CAPABILITY_INFO_T
{
    BOOL    b_default;      /* enable default boot logo */
    BOOL    b_default_exist;/* is default boot logo exist*/
    UINT8   ui1_cur_logo_index;
    UINT8   ui1_nums_logo_slots;
    UINT16  ui2_logo_valid_tag;
  /*to be extend*/
} CAP_CAPABILITY_INFO_T;

/* Event Define For Capture Output Resolution *******************************/
typedef enum
{
    CAP_DEVICE_TYPE_DEFAULT,
    CAP_DEVICE_TYPE_EXTERNAL,
    CAP_DEVICE_TYPE_INTERNAL
    /*to be extend*/ 
} CAP_DEVICE_TYPE_T;


/* Capture information definition *******************************/
typedef struct _CAP_LOGO_SAVE_INFO_T
{
    CAP_DEVICE_TYPE_T       e_device_type;  
    union 
    {
        UINT32 ui4_logo_id;
        CHAR*  ps_path;
    } u;
} CAP_LOGO_SAVE_INFO_T;

typedef CAP_LOGO_SAVE_INFO_T CAP_LOGO_SELECT_INFO_T;
/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define CAPR_OK                     ((INT32)     0)
#define CAPR_INSUFFICIENT_BUFFER    ((INT32)    -1)
#define CAPR_INV_HANDLE             ((INT32)    -2)
#define CAPR_OUT_OF_HANDLE          ((INT32)    -3)
#define CAPR_OUT_OF_MEMORY          ((INT32)    -4)
#define CAPR_FCT_NOT_SUPPORT        ((INT32)    -5)
#define CAPR_NAME_EXIST             ((INT32)    -6)
#define CAPR_INV_ARG                ((INT32)    -7)
#define CAPR_ENGINE_NOT_FOUND       ((INT32)    -8)
#define CAPR_INTERNAL_ERR           ((INT32)    -9)
#define CAPR_ENCODE_FAIL            ((INT32)   -10)
#define CAPR_INITIAL_ERR            ((INT32)   -11)
#define CAPR_MSG_Q_ERR              ((INT32)   -12)
#define CAPR_IO_ERROR               ((INT32)   -13)
#define CAPR_DRV_BUSY               ((INT32)   -14)
#define CAPR_CLI_ERROR              ((INT32)   -15)


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief      x_cap_nfy_fct
 *              This function is the callback function of capture library.
 *  @param [in] h_handle     Specifies which handle will be notified. 
 *  @param [in] pv_tag       The client data specified by the client when callback has been called, the callback function will send it again.. 
 *  @param [in] e_event      It identifies the event type of notification. 
 *  @param [in] pv_data      Specified the notification data which is followed with the notification event. 
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_cap_nfy_fct) (
                                HANDLE_T                h_handle,  /* (I) Specifies which handle will be notified. */
                                const VOID*             pv_tag,    /* (I) The client data specified by the client when callback has been called, the callback function will send it again.. */
                                CAP_EVENT_TYPE_T        e_event,   /* (I) It identifies the event type of notification. */
                                const VOID*             pv_data);   /* (I) specify the notification data which is followed with the notification event. */


/*------------------------------------------------------------------*/
/*! @brief      x_cap_cust_fct
 *              This function is the function of capture library.
 *  @param [in] e_source     Specifies the source which will be captured. 
 *  @param [in] e_event      It identifies the event type of notification. 
 *  @param [in] pv_data      Specified the notification data which is followed with the notification event. 
 */
/*------------------------------------------------------------------*/
typedef BOOL (*x_cap_cust_fct) (
                                CAP_SRC_TYPE_T          e_source,  /* (I) Specifies the source which will be captured. */
                                CAP_EVENT_TYPE_T        e_event,     /* (I) It identifies the event type of notification. */
                                const VOID*             pv_data);     /* (I) specify the notification data which is followed with the notification event. */




typedef struct
{
    HANDLE_T            h_cap;
    HANDLE_T            h_src_surf;/* For capture photo use*/    
    HANDLE_T            h_working_surf;/* For capture photo use*/    
    HANDLE_T            h_cap_cancel_exe_sema;/* For sync stop use*/    
    BOOL                b_cancel_exe_flag;/* For sync stop use*/  
    CAP_OUT_RES_TYPE_T  e_res_type; 
    CAP_EVENT_TYPE_T    e_cur_event;
    CAP_SRC_TYPE_T      e_source;           /* (I) specifies the open mode of the open operation*/ 
    x_cap_nfy_fct       pf_nfy;             /* (I) specify the callback function to receive the notification of image library. */ 
    const VOID*         pv_nfy_tag;         /* (I) The client data specified by the client and the callback function will send it again. */ 
    x_cap_cust_fct      pf_cust;            /* (I) Specified the function to customize user's operation when creating handle. */ 
    const VOID*         pv_cust_tag; 

    CAP_LOGO_SAVE_INFO_T t_save_info;
    CAP_INPUT_T         t_cap_input;
    CAP_OUTPUT_T        t_cap_output;

    UINT32                  ui4_moveable_width;
    UINT32                  ui4_moveable_height;
} CAP_OBJ_T;


#ifdef __cplusplus
}
#endif

#endif /* _U_CAP_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_CAP*/
/*----------------------------------------------------------------------------*/
