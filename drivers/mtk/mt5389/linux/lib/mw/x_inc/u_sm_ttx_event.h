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
/*! @file u_sm_ttx_event.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Subtitle Stream handler specific 
 *         events.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_TTX_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_TTX_EVENT_H_
#define _U_SM_TTX_EVENT_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sm_event.h"
#include "u_sm_ttx_common.h"
#include "u_wgl_ttx.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define SM_EVN_TTX_APP_OPEN_REQ             ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 0)    /**<APP open require event */
#define SM_EVN_TTX_APP_PLAY_REQ             ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 1)    /**<APP play require event */
#define SM_EVN_TTX_APP_STOP_REQ             ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 2)    /**<APP stop require event */
#define SM_EVN_TTX_APP_CLOSE_REQ            ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 3)    /**<APP close require event */
#define SM_EVN_TTX_DRV_OPENED               ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 4)    /**<Drive opened event */
#define SM_EVN_TTX_DRV_PLAYED               ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 5)    /**<Drive played event*/
#define SM_EVN_TTX_DRV_STOPPED              ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 6)    /**<Drive stopped event*/
#define SM_EVN_TTX_DRV_CLOSED               ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 7)    /**<Drive closed event*/
#define SM_EVN_TTX_DRV_ERR                  ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 8)    /**<Drive Error event*/
#define SM_EVN_TTX_SCDB_RECORD_DEL          ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 9)    /**<Scdb record elete event*/
#define SM_EVN_TTX_NOT_AUTHORIZED           ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 10)    /**<Not authorized event*/
#define SM_EVN_TTX_INTERNAL_ERR             ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 11)    /**<Internal error event*/

/* Notify information to upper layer application.The notify function can see:
    typedef INT32 (*x_stream_nfy_fct)( HANDLE_T        h_stream,
                                                       SM_COND_T       e_nfy_cond,
                                                       VOID*           pv_nfy_tag,
                                                       SM_EVN_TYPE_T   e_event,
                                                       UINT32          ui4_data );

ui4_data contains pointer points to structure SM_TTXH_EVN_WGL_UPDATE_T
*/
#define SM_EVN_TTX_WGL_UPDATE_NFY           ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 12)   

/**Notify information if stream status is changed.
    typedef INT32 (*x_stream_nfy_fct)( HANDLE_T        h_stream,
                                                       SM_COND_T       e_nfy_cond,
                                                       VOID*           pv_nfy_tag,
                                                       SM_EVN_TYPE_T   e_event,
                                                       UINT32          ui4_data );

ui4_data contains pointer points to structure SM_TTXH_EVN_STREAM_UPDATE_T
*/
#define SM_EVN_TTX_STREAM_UPDATE_NFY        ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 13)    

/**
    Notify information if below information of SM_TTX_PAGE_BASIC_INFO_T is updated. 
    BOOL                                    b_existed;
    BOOL                                    b_newsflash;
    BOOL                                    b_subtitle;
    BOOL                                    b_suppress_header;
    BOOL                                    b_inhibit_display;
    BOOL                                    b_display_row24;
    BOOL                                    b_has_side_pnl;
    BOOL                                    b_has_flof_info;
    BOOL                                    b_has_top_info;
    UINT8                                   ui1_left_side_pnl_col_num;
    SM_TTX_PAGE_TYPE_T            e_page_type;

    The notify function can see:
    typedef INT32 (*x_stream_nfy_fct)( HANDLE_T        h_stream,
                                                       SM_COND_T       e_nfy_cond,
                                                       VOID*           pv_nfy_tag,
                                                       SM_EVN_TYPE_T   e_event,
                                                       UINT32          ui4_data );
    
    ui4_data contains pointer points to structure SM_TTXH_EVN_CUR_PAGE_UPDATE_T
*/
#define SM_EVN_TTX_CUR_PAGE_UPDATE_NFY      ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 14)    

/**Notify information if last page is received.    The notify function can see:
    typedef INT32 (*x_stream_nfy_fct)( HANDLE_T        h_stream,
                                                       SM_COND_T       e_nfy_cond,
                                                       VOID*           pv_nfy_tag,
                                                       SM_EVN_TYPE_T   e_event,
                                                       UINT32          ui4_data );

    ui4_data contains pointer points to structure SM_TTXH_EVN_SUBPAGE_UPDATE_T
*/
#define SM_EVN_TTX_SUBPAGE_UPDATE_NFY       ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 15)    

/**
    Notify information if newsflash update.The notify function can see:
    typedef INT32 (*x_stream_nfy_fct)( HANDLE_T        h_stream,
                                                       SM_COND_T       e_nfy_cond,
                                                       VOID*           pv_nfy_tag,
                                                       SM_EVN_TYPE_T   e_event,
                                                       UINT32          ui4_data );
                                                       
    ui4_data contains pointer points to structure SM_TTXH_EVN_NEWSFLASH_UPDATE_T
*/
#define SM_EVN_TTX_NEWSFLASH_UPDATE_NFY       ((SM_EVN_TYPE_T) SM_EVN_GRP_TTX + 16)    

/*-----------------------------------------------------------------------------
                    structure
-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_EVN_WGL_UPDATE_T
 *  @brief This struct contains the ttx widget update information 
 *  @code
 *  typedef struct _SM_TTXH_EVN_WGL_UPDATE_T
 *  {
 *      UINT8                   ui1_wgl_id;
 *      WGL_TTX_NFY_REASON_T    e_reason;
 *      UINT32                  ui4_data;
 *  } SM_TTXH_EVN_WGL_UPDATE_T;
 *  @endcode
 *  @li@c  ui1_wgl_id                      -The widget id to update,from 0
 *  @li@c  e_reason                         -The widget to update reason type.
 *  @li@c  ui4_data                          -Contains the notify data
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_EVN_WGL_UPDATE_T
{
    /* Widget id */
    UINT8                   ui1_wgl_id;

    /* Widget notify reason */
    WGL_TTX_NFY_REASON_T    e_reason;

    /* Widget notify data */
    UINT32                  ui4_data;
} SM_TTXH_EVN_WGL_UPDATE_T;


#define SM_TTXH_EVN_STREAM_UPDATE_MASK_REC_PAGES        0x01    /**<Recieved normal page notify mask flag*/
#define SM_TTXH_EVN_STREAM_UPDATE_MASK_REC_SUBTITLE     0x02    /**<Recieved subtitle page notify mask flag*/
#define SM_TTXH_EVN_STREAM_UPDATE_MASK_TOP_UPDATE       0x04    /**<Recieved TOP page notify mask flag */

/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_EVN_STREAM_UPDATE_T
 *  @brief This struct contains the ttx stream notify information
 *  @code
 *  typedef struct _SM_TTXH_EVN_STREAM_UPDATE_T
 *  {
 *      UINT8                   ui1_mask;
 *      BOOL                    b_rec_pages;
 *      BOOL                    b_rec_subtitle;
 *      BOOL                    b_top_update;
 *  } SM_TTXH_EVN_STREAM_UPDATE_T;
 *  @endcode
 *  @li@c  ui1_mask                           -Contains the update type information
 *  @li@c  b_rec_pages                       -True :it recieved the normal page
 *  @li@c  b_rec_subtitle                     -True:it recieved the subtitle page
 *  @li@c  b_top_update                      -True:it recieved the top page
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_EVN_STREAM_UPDATE_T
{
    /* Mask of update stream */
    UINT8                   ui1_mask;

    /* received pages */
    BOOL                    b_rec_pages;

    /* received subtitle pages */
    BOOL                    b_rec_subtitle;

    /* Whether TOP information is updated */
    BOOL                    b_top_update;
} SM_TTXH_EVN_STREAM_UPDATE_T;

/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_EVN_CUR_PAGE_UPDATE_T
 *  @brief This struct contains the updating page event information
 *  @code
 *  typedef struct _SM_TTXH_EVN_CUR_PAGE_UPDATE_T
 *  {
 *      UINT8                   ui1_wgl_id;
 *  } SM_TTXH_EVN_CUR_PAGE_UPDATE_T;
 *  @endcode
 *  @li@c  ui1_wgl_id                 -The widget that is to update
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_EVN_CUR_PAGE_UPDATE_T
{
    /* Widget id */
    UINT8                   ui1_wgl_id;
} SM_TTXH_EVN_CUR_PAGE_UPDATE_T;

/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_EVN_SUBPAGE_UPDATE_T
 *  @brief This struct contains the update subpage event information
 *  @code
 *  typedef struct _SM_TTXH_EVN_SUBPAGE_UPDATE_T
 *  {
 *      UINT8                   ui1_wgl_id;
 *      SM_TTX_PAGE_ADDR_T      t_page_addr;
 *  } SM_TTXH_EVN_SUBPAGE_UPDATE_T;
 *  @endcode
 *  @li@c  ui1_wgl_id                       -The widget id,from 0
 *  @li@c  t_page_addr                     -The page address that is to update
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_EVN_SUBPAGE_UPDATE_T
{
    /* Widget id */
    UINT8                   ui1_wgl_id;

    /* Page address */
    SM_TTX_PAGE_ADDR_T      t_page_addr;
} SM_TTXH_EVN_SUBPAGE_UPDATE_T;

/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_EVN_NEWSFLASH_UPDATE_T
 *  @brief This struct contains the newflash page update information
 *  @code
 *  typedef struct _SM_TTXH_EVN_NEWSFLASH_UPDATE_T
 *  {
 *      SM_TTX_PAGE_ADDR_T      t_page_addr;
 *  }SM_TTXH_EVN_NEWSFLASH_UPDATE_T;
 *  @endcode
 *  @li@c  t_page_addr                -Page addr that is newflash type and to update
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_EVN_NEWSFLASH_UPDATE_T
{
    /*Page address*/
    SM_TTX_PAGE_ADDR_T      t_page_addr;
}SM_TTXH_EVN_NEWSFLASH_UPDATE_T;

#endif /* _U_SM_TTX_EVENT_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                /* end of groupMW_STRM_MNGR_TTX_HDLR */
/*----------------------------------------------------------------------------*/


