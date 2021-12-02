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
/*! @file u_sm_ttx_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Teletext Stream handler specific 
 *         definitions, which are exported to applications.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_TTX_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_TTX_HDLR_H_
#define _U_SM_TTX_HDLR_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sys_name.h"
#include "u_handle.h"
#include "u_sm.h"

#include "u_sm_ttx_common.h"
#include "u_wgl_ttx.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define SM_TTX_ANA_CHN_PID                                  0xFFFF    /**<The analog teletext chanel pid value.  */


/***************************************************************************** 
    Primary command sets 
*****************************************************************************/
 /** This command sets Teletext data source to Teletext stream handler. It is mandatory. 

The used data struct is SM_COMMAND_T:

e_code:         Must set SM_TTX_CMD_CODE_SET_TTX_SOURCE;

ui2_pid:        -Contains the pid value of the Teletext or VBI stream.
                     If the Teletext stream source is analog VBI signal, ui2_pid sets to 0xFFFF.
*/
#define SM_TTX_CMD_CODE_SET_TTX_SOURCE                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))          

 /** This command sets OSD plane handle to Teletext stream handler.It is mandatory. 

The used data struct is SM_COMMAND_T:

e_code:         Must set SM_TTX_CMD_CODE_SET_OSD_HANDLE;

h_obj:           Containing the handle of the OSD driver component.The OSD plane handle is used to create Teletext widget
 */
#define SM_TTX_CMD_CODE_SET_OSD_HANDLE                       \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 1))   

/** This command set Teletext widget properties. It's mandatory.

The used data struct is SM_COMMAND_T:

e_code:         Must set SM_TTX_CMD_CODE_SET_TTX_WGL.

ui1_data:      Indicating the maximum number of Teletext widget.
 */
#define SM_TTX_CMD_CODE_SET_TTX_WGL                         \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 2))   

/**    This command enable or disable support of Teletext 2.5. It's optional.

The used data struct is SM_COMMAND_T:

e_code:         Must set SM_TTX_CMD_CODE_SET_SUPPORT_2_5.

b_boolean:     It is b_enable_2_5 that indicating whether 2.5 system is supported.
    If this command is not used, the Teletext 2.5 support is configured in SM_TTXH_CONFIG_T.
    If teletext 2.5 support is disable in SM_TTXH_CONFIG_T, this command doesn't take effect.
*/
#define SM_TTX_CMD_CODE_SET_SUPPORT_2_5                      \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 3))            

/** This command set preference language of Teletext. It's optional.

The used data struct is SM_COMMAND_T:

e_code:         Must set SM_TTX_CMD_CODE_SET_PREFERENCE_LANGUAGE.

pv_data:        Points to SM_TTX_CHAR_SET_DESIGNATION_T structure indicating default character set designation.
    If this command is not used, the default character set is configured in SM_TTXH_CONFIG_T.
*/
#define SM_TTX_CMD_CODE_SET_PREFERENCE_LANGUAGE               \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 4))    

/** This command set private notify function of Teletext. It is optional. 

The used data struct is SM_COMMAND_T:

e_code:         Must set SM_TTX_CMD_CODE_SET_PRIVATE_NFY_INFO.

pv_data:        Points to SM_TTXH_PRIVATE_NFY_INFO_T structure.
*/
#define SM_TTX_CMD_CODE_SET_PRIVATE_NFY_INFO                  \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 5))    


/**  This command enable or disable cache hex page. It is optional. 

The used data struct is SM_COMMAND_T:

e_code:         Must set SM_TTX_CMD_CODE_SET_CACHE_INFO.

b_boolean:    It indicates whether need to cache hex page or not.
*/
#define SM_TTX_CMD_CODE_SET_CACHE_INFO                  \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 6))    

#define SM_TTX_CMD_CODE_SET_SERVICE_NAME_INF            \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 7))    

/***************************************************************************** 
    Subsequence command sets 
*****************************************************************************/
/**This is subsequnce command of SM_TTXH_WGL_CUST_INIT_T.This command set the created 
Teletext widget attributes. It is mandatory. 

The used data struct is SM_COMMAND_T:

e_code:         Must set SM_TTX_CMD_CODE_SET_CACHE_INFO.

pv_data:       Points to SM_TTXH_WGL_CUST_INIT_T structure,that is used for create teletext widget attributes.
*/
#define SM_TTX_CMD_CODE_SET_TTX_WGL_ATTR  ((SM_CMD_CODE_T) 1)    



/**  
 Teletext stream handler set/get type groups.
*/
#define SM_GRP_TTXH                             (( SM_GRP_LAST_ENTRY + 13 ) << SM_CODE_BITS)   


/***************************************************************************** 
    Set types 
*****************************************************************************/
/** 
    Argument pv_set_data of API (*sm_set_fct) should be set to NULL.
*/
#define SM_TTXH_SET_TYPE_IGNORE                 ((SM_SET_TYPE_T)(SM_GRP_TTXH | 0))    

/** Clear all Teletext pages cached in Teletext handler.The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_TYPE_CLR_PAGES.

pv_set_info:           -Is null.

z_set_info_len:       -Null.
*/
#define SM_TTXH_SET_TYPE_CLR_PAGES              ((SM_SET_TYPE_T)(SM_GRP_TTXH | 1))   

/** Set current view(search) page.The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:         -Must set to SM_TTXH_SET_TYPE_SET_PAGE.

pv_set_info:        -points to SM_TTXH_WGL_VIEW_PAGE_T struct .

z_set_info_len:    -The size of SM_TTXH_WGL_VIEW_PAGE_T.
*/
#define SM_TTXH_SET_TYPE_SET_PAGE               ((SM_SET_TYPE_T)(SM_GRP_TTXH | 2))   

/** Set Teletext widget attributes. The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_TYPE_WGL_ATTR.

pv_set_info:           -Point to SM_TTXH_WGL_ATTR_T structure.

z_set_info_len:       -The size of SM_TTXH_WGL_ATTR_T.
*/
#define SM_TTXH_SET_TYPE_WGL_ATTR               ((SM_SET_TYPE_T)(SM_GRP_TTXH | 3))    

/** Lock Teletext handler. No more packets will be parsing.The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_TYPE_LOCK.

pv_set_info:           -NULL.

z_set_info_len:       -NULL.
*/
#define SM_TTXH_SET_TYPE_LOCK               ((SM_SET_TYPE_T)(SM_GRP_TTXH | 4))   

/** Unlock Teletext handler. Teletext handler starts parsing packet.The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_TYPE_UNLOCK.

pv_set_info:           -NULL.

z_set_info_len:       -NULL.
*/
#define SM_TTXH_SET_TYPE_UNLOCK             ((SM_SET_TYPE_T)(SM_GRP_TTXH | 5))   

/** Enable or disable rolling header in search mode.The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_TYPE_ROLL_HEADER.

pv_set_info:           -Point to the BOOL data,that indicate whether roll the header or not.

z_set_info_len:       -The size of BOOL.
*/
#define SM_TTXH_SET_TYPE_ROLL_HEADER        ((SM_SET_TYPE_T)(SM_GRP_TTXH | 6))   

/** Set private notify function.  The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_TYPE_PRIVATE_NFY_FCT.

pv_set_info:           -Point to the SM_TTXH_PRIVATE_NFY_INFO_T struct that contains the notify 
api function and tag information.

z_set_info_len:       -The size of SM_TTXH_PRIVATE_NFY_INFO_T.
*/
#define SM_TTXH_SET_TYPE_PRIVATE_NFY_FCT    ((SM_SET_TYPE_T)(SM_GRP_TTXH | 7))   

/** Set hold status. The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_TYPE_HOLD.

pv_set_info:           -Point to the BOOL data,that indicate whether hold or not.

z_set_info_len:       -The size of BOOL.
*/
#define SM_TTXH_SET_TYPE_HOLD               ((SM_SET_TYPE_T)(SM_GRP_TTXH | 8))    

/** Set character set. The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_CHARSET.

pv_set_info:           -Point to the SM_TTX_CHARSET_MAP_TBL_T struct that contains the teletext 
charset mapping table information.

z_set_info_len:       -The size of SM_TTX_CHARSET_MAP_TBL_T.
*/
#define SM_TTXH_SET_CHARSET                 ((SM_SET_TYPE_T)(SM_GRP_TTXH | 9))    


/** Set create / delete Teletext widgets.The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_CREATE_WIDGET.

pv_set_info:           -Point to the BOOL data. TRUE,it create teletext widget;FALSE,it delete 
widget.

z_set_info_len:       -The size of BOOL.
*/
#define SM_TTXH_SET_CREATE_WIDGET           ((SM_SET_TYPE_T)(SM_GRP_TTXH | 10))   

/** Set subpage status. The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_TYPE_SUBPAGE_MODE.

pv_set_info:           -Point to the BOOL data that  Indicate whether in subpage mode.

z_set_info_len:       -The size of BOOL.
*/
#define SM_TTXH_SET_TYPE_SUBPAGE_MODE       ((SM_SET_TYPE_T)(SM_GRP_TTXH | 11))    

/** Set newsflash monitor page. The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_NEWSFLASH_UPDATE_NFY.

pv_set_info:           -Point to the SM_TTXH_MONITOR_NEWSFLASH_PAGE_T struct that contains the 
newflash page need notify\update information.

z_set_info_len:       -The size of SM_TTXH_MONITOR_NEWSFLASH_PAGE_T.
*/
#define SM_TTXH_SET_NEWSFLASH_UPDATE_NFY        ((SM_SET_TYPE_T)(SM_GRP_TTXH | 12))   

#ifdef TTX_L25_ENHANCEMENT
/** Set presentation level.The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_LEVEL.

pv_set_info:           -Point to the BOOL data that indicates the enable 2.5 level or not.

z_set_info_len:       -The size of BOOL.
*/
#define SM_TTXH_SET_LEVEL       ((SM_SET_TYPE_T)(SM_GRP_TTXH | 13))   
#endif


/** Set ttx display brightness degree. The API function is (*sm_set_fct):

typedef INT32 (*sm_set_fct)( HANDLE_T           h_handle,
                                         SM_SET_TYPE_T   e_set_type,
                                         VOID*                 pv_set_info,
                                         SIZE_T                z_set_info_len );

h_handle:               -Teletext stream handlel.

e_set_type:             -Must set to SM_TTXH_SET_BRIGHTNESS_DEGREE.

pv_set_info:           -Point to the SM_TTXH_BRIGHTNESS_DEGREE_T struct that contain the setting 
brightness information.

z_set_info_len:       -The size of SM_TTXH_BRIGHTNESS_DEGREE_T.
*/
#define SM_TTXH_SET_BRIGHTNESS_DEGREE        ((SM_SET_TYPE_T)(SM_GRP_TTXH | 14))    


/******************************************************************************
    Get types 
******************************************************************************/
/**Ignore this ttx get type  */
#define SM_TTXH_GET_TYPE_IGNORE                 ((SM_GET_TYPE_T)(SM_GRP_TTXH | 0))    

/**Get Teletext widget attributes. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_STREAM_INFO.

pv_get_info:           -Point to the SM_TTXH_STREAM_INFO_T struct that contain the teletext stream 
information.

pz_get_info_len:       -The size of SM_TTXH_STREAM_INFO_T.
*/
#define SM_TTXH_GET_TYPE_STREAM_INFO            ((SM_GET_TYPE_T)(SM_GRP_TTXH | 1))    

/** Get Teletext init page address. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_INIT_PAGE_ADDR.

pv_get_info:           -Point to the SM_TTX_PAGE_ADDR_T struct that contain the teletext page addr information.

pz_get_info_len:       -The size of SM_TTX_PAGE_ADDR_T.
*/
#define SM_TTXH_GET_TYPE_INIT_PAGE_ADDR         ((SM_GET_TYPE_T)(SM_GRP_TTXH | 2))    

/**    Get Teletext current page address. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_CUR_PAGE_ADDR.

pv_get_info:           -Point to the SM_TTX_PAGE_ADDR_T struct that contain the teletext page addr information.

pz_get_info_len:       -The size of SM_TTX_PAGE_ADDR_T.
*/
#define SM_TTXH_GET_TYPE_CUR_PAGE_ADDR          ((SM_GET_TYPE_T)(SM_GRP_TTXH | 3))    

/**Get Teletext page information. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_PAGE_BASIC_INFO.

pv_get_info:           -Point to the SM_TTXH_PAGE_BASIC_INFO_T struct that contain the teletext 
page basic information.

pz_get_info_len:       -The size of SM_TTXH_PAGE_BASIC_INFO_T.
*/
#define SM_TTXH_GET_TYPE_PAGE_BASIC_INFO        ((SM_GET_TYPE_T)(SM_GRP_TTXH | 4))   

/**Get Teletext page FLOF information. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_PAGE_FLOF_INFO.

pv_get_info:           -Point to the SM_TTXH_PAGE_FLOF_INFO_T struct that contain the teletext 
page FLOF information.

pz_get_info_len:       -The size of SM_TTXH_PAGE_FLOF_INFO_T.
*/
#define SM_TTXH_GET_TYPE_PAGE_FLOF_INFO         ((SM_GET_TYPE_T)(SM_GRP_TTXH | 5))   

/** Get Teletext page TOP information. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_PAGE_TOP_INFO.

pv_get_info:           -Point to the SM_TTXH_PAGE_TOP_INFO_T struct that contain the teletext 
page TOP information.

pz_get_info_len:       -The size of SM_TTXH_PAGE_TOP_INFO_T.
*/
#define SM_TTXH_GET_TYPE_PAGE_TOP_INFO          ((SM_GET_TYPE_T)(SM_GRP_TTXH | 6))    

/** Get Teletext page subpage information. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_PAGE_SUB_PAGE_INFO.

pv_get_info:           -Point to the SM_TTXH_PAGE_SUB_PAGE_INFO_T struct that contain the teletext 
subpage information.

pz_get_info_len:       -The size of SM_TTXH_PAGE_SUB_PAGE_INFO_T.
*/
#define SM_TTXH_GET_TYPE_PAGE_SUB_PAGE_INFO     ((SM_GET_TYPE_T)(SM_GRP_TTXH | 7))    

/** Get Teletext TOP block information. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_TOP_BLOCK_INFO.

pv_get_info:           -Point to the SM_TTXH_TOP_BLOCK_INFO_T struct that contain the teletext 
block information of the top.

pz_get_info_len:       -The size of SM_TTXH_TOP_BLOCK_INFO_T.
*/
#define SM_TTXH_GET_TYPE_TOP_BLOCK_INFO         ((SM_GET_TYPE_T)(SM_GRP_TTXH | 8))    

/**Get Teletext TOP group information.The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_TOP_GROUP_INFO.

pv_get_info:           -Point to the SM_TTXH_TOP_BLOCK_INFO_T struct that contain the teletext 
group information of the top.

pz_get_info_len:       -The size of SM_TTXH_TOP_BLOCK_INFO_T.
*/
#define SM_TTXH_GET_TYPE_TOP_GROUP_INFO         ((SM_GET_TYPE_T)(SM_GRP_TTXH | 9))   

/**    Get Teletext TOP group page information. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_TOP_GROUP_PAGE_INFO.

pv_get_info:           -Point to the SM_TTXH_TOP_GROUP_PAGE_INFO_T struct that contain the teletext 
page information of the special group in the top.

pz_get_info_len:       -The size of SM_TTXH_TOP_GROUP_PAGE_INFO_T.
*/
#define SM_TTXH_GET_TYPE_TOP_GROUP_PAGE_INFO    ((SM_GET_TYPE_T)(SM_GRP_TTXH | 10))    

/**   
Get Teletext TOP programme page information. This type is not used now.
*/
#define SM_TTXH_GET_TYPE_TOP_PROGRAMME_INFO     ((SM_GET_TYPE_T)(SM_GRP_TTXH | 11))    


/**    Get Teletext TOP programme page information.The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_TOP_AIT_INFO.

pv_get_info:           -Point to the SM_TTXH_TOP_AIT_INFO_T struct that contain the teletext 
AIT page information of the special top.

pz_get_info_len:       -The size of SM_TTXH_TOP_AIT_INFO_T.
*/
#define SM_TTXH_GET_TYPE_TOP_AIT_INFO     ((SM_GET_TYPE_T)(SM_GRP_TTXH | 12))   


/** Get Teletext subtitle page information. The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_SUBTITLE_INFO.

pv_get_info:           -Point to the SM_TTXH_SUBTITLE_PAGE_INFO_T struct that contain the teletext 
subtitle page information .

pz_get_info_len:       -The size of SM_TTXH_SUBTITLE_PAGE_INFO_T.
*/
#define SM_TTXH_GET_TYPE_SUBTITLE_INFO          ((SM_GET_TYPE_T)(SM_GRP_TTXH | 13))    

/**    Get Teletext EPG page information.The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_TYPE_EPG_PAGE_INFO.

pv_get_info:           -Point to the SM_TTX_EPG_PAGE_INFO_T struct that contain the teletext 
epg page information .

pz_get_info_len:       -The size of SM_TTX_EPG_PAGE_INFO_T.
*/
#define SM_TTXH_GET_TYPE_EPG_PAGE_INFO          ((SM_GET_TYPE_T)(SM_GRP_TTXH | 14))    

/**  Get sibling existed \sequence or not sibling pages address.The API function is (*sm_get_fct):

typedef INT32 (*sm_get_fct)( HANDLE_T       h_handle,
                             SM_GET_TYPE_T  e_get_type,
                             VOID*          pv_get_info,       
                             SIZE_T*        pz_get_info_len );

h_handle:               -Teletext stream handlel.

e_get_type:             -Must set to SM_TTXH_GET_SIBLING_PAGE.

pv_get_info:           -Point to the SM_TTX_SIBLING_PAGE_INFO_T struct that contain the teletext 
sibling page information of the special page.

pz_get_info_len:       -The size of SM_TTX_SIBLING_PAGE_INFO_T.
*/
#define SM_TTXH_GET_SIBLING_PAGE                ((SM_SET_TYPE_T)(SM_GRP_TTXH | 15))   

/*-----------------------------------------------------------------------------
                    Structure
-----------------------------------------------------------------------------*/

/* Widget init configuration */



/* WGL_TTX_CUST_INIT_T */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_WGL_CUST_INIT_T
 *  @brief This structure defines customer initial widget data information
 *  @code
 *  typedef struct _SM_TTXH_WGL_CUST_INIT_T
 *  {
 *      UINT32                              ui4_wgl_type_border;
 *      WGL_TTX_CUST_INIT_T                 t_wgl_ttx_cust_init;
 *  } SM_TTXH_WGL_CUST_INIT_T;
 *  @endcode
 *  @li@c  ui4_wgl_type_border                -Contains the border of widget type information
 *  @li@c  t_wgl_ttx_cust_init                   -Contains the customer initial widget data information
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_WGL_CUST_INIT_T
{
    UINT32                              ui4_wgl_type_border;
    
    WGL_TTX_CUST_INIT_T                 t_wgl_ttx_cust_init;
} SM_TTXH_WGL_CUST_INIT_T;


/* Default character set */
/*------------------------------------------------------------------*/
/*! @struct SM_TTX_CHAR_SET_DESIGNATION_T
 *  @brief This structure defines ttx charset designation data
 *  @code
 *  typedef struct _SM_TTX_CHAR_SET_DESIGNATION_T
 *  {
 *      UINT8                           ui1_default_designation;
 *      UINT8                           ui1_second_designation;
 *      UINT8                           ui1_modified_designation;
 *  } SM_TTX_CHAR_SET_DESIGNATION_T;
 *  @endcode
 *  @li@c  ui1_default_designation            -Default character set designation, the coding is defined in ETSI EN 300 706, Table 32.
 *  @li@c  ui1_second_designation            -Second character set designation, the coding is defined in ETSI EN 300 706, Table 33
 *  @li@c  ui1_modified_designation          -Modified character set designation, the coding is defined in ETSI EN 300 706, Table 34. 
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTX_CHAR_SET_DESIGNATION_T
{
    /* Default character set designation, the coding is defined in ETSI EN 300 706, Table 32. 
       Only bit 14~11 is used and bit 3~0 of this field is bit 14~11 in the table*/
    UINT8                           ui1_default_designation;

    /* Second character set designation, the coding is defined in ETSI EN 300 706, Table 33. */
    UINT8                           ui1_second_designation;

    /* Modified character set designation, the coding is defined in ETSI EN 300 706, Table 34. */
    UINT8                           ui1_modified_designation;
} SM_TTX_CHAR_SET_DESIGNATION_T;

/* Init configuration Structure  */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_CONFIG_T
 *  @brief This structure define the ttx global information and is initialed at the system start 
 *  @code
 *  typedef struct _SM_TTXH_CONFIG_T
 *  {
 *      THREAD_DESCR_T                  t_thrd_descr;               
 *      HANDLE_T                        h_partition;
 *      HANDLE_T                        h_cache_part;
 *      UINT32                          ui4_max_part_size;
 *      UINT8                           ui1_ttx_service_max_cnt;
 *      UINT8                           ui1_wgl_order;
 *      SM_TTX_CACHE_POLICY_T           t_cache_policy;      
 *      BOOL                            b_enable_top;
 *      BOOL                            b_enable_2_5;
 *      BOOL                            b_parse_mip;
 *      BOOL                            b_rolling_header;
 *      SM_TTX_CHAR_SET_DESIGNATION_T   t_default_char_set_designation;
 *  } SM_TTXH_CONFIG_T;
 *  @endcode
 *  @li@c  t_thrd_descr                               -Contain the ttx thread decription information
 *  @li@c  h_partition                                 -The ttx partition handle
 *  @li@c  h_cache_part                              -The ttx cache partition handle
 *  @li@c  ui4_max_part_size                      -The max partition size
 *  @li@c  ui1_ttx_service_max_cnt              -The max count of the ttx services
 *  @li@c  ui1_wgl_order                            -TTX widget order type
 *  @li@c  t_cache_policy                            -TTX cache policy data information
 *  @li@c  b_enable_top                              -True:enable the top function;other ,disable
 *  @li@c  b_enable_2_5                             -True:enable the ttx 2.5 level;Ohter:disable
 *  @li@c  b_parse_mip                               -This field indicates whether the engine parse magazine inventory page packets
 *  @li@c  b_rolling_header                         -Enable rolling header or not
 *  @li@c  t_default_char_set_designation     -Preference language information
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_CONFIG_T
{
    /* Teletext handler thread */
    THREAD_DESCR_T                  t_thrd_descr;               

    /* Handle of memory partition */
    HANDLE_T                        h_partition;
    
    /* Handle of cache memory partition */
    HANDLE_T                        h_cache_part;

    /* Maximum size of partition */
    UINT32                          ui4_max_part_size;

    /* Maximum number of Teletext service */
    UINT8                           ui1_ttx_service_max_cnt;

    /* Widget App Order */
    UINT8                           ui1_wgl_order;

    /* Teletext engine cache policy */
    SM_TTX_CACHE_POLICY_T           t_cache_policy;      

    /* Whether this system support TOP */
    BOOL                            b_enable_top;

    /* Whether this system support 2.5 */
    BOOL                            b_enable_2_5;

    /* This field indicates whether the engine parse magazine inventory page packets. 
    If this field is ture, the engine parses magazine inventory page packets. Otherwise, 
    the engine gets whole magazine inventory page by sm_ttx_engine_set_magazine_inventory_page */
    BOOL                            b_parse_mip;

    /* Enable rolling header */
    BOOL                            b_rolling_header;

    /* Preference language */
    SM_TTX_CHAR_SET_DESIGNATION_T   t_default_char_set_designation;
} SM_TTXH_CONFIG_T;

/* contain information of stream */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_STREAM_INFO_T
 *  @brief This struct contains the ttx stream information
 *  @code
 *  typedef struct _SM_TTXH_STREAM_INFO_T
 *  {
 *      BOOL                                    b_rec_pages;
 *      BOOL                                    b_top_stream;
 *  } SM_TTXH_STREAM_INFO_T;
 *  @endcode
 *  @li@c  b_rec_pages             -True:received ttx page.Otherwise,not received ttx  page.
 *  @li@c  b_top_stream           -True:this stream has top information;Otherwise,not has top information
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_STREAM_INFO_T
{
    /* received pages */
    BOOL                                    b_rec_pages;

    /* Whether got TOP information */
    BOOL                                    b_top_stream;
} SM_TTXH_STREAM_INFO_T;

/* Contains information to set a view page */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_WGL_VIEW_PAGE_T
 *  @brief This struct contains the widget information for the viewing page.
 *  @code
 *  typedef struct _SM_TTXH_WGL_VIEW_PAGE_T
 *  {
 *      UINT8                   ui1_wgl_id;
 *      SM_TTX_PAGE_ADDR_T      t_page_addr;
 *      SM_TTX_LANG_CODE_T      e_preference_lang;
 *      BOOL                    b_existed;
 *      BOOL                    b_flof_first;
 *  } SM_TTXH_WGL_VIEW_PAGE_T;
 *  @endcode
 *  @li@c  ui1_wgl_id                    -ID of the created Teletext widget. The value is from 0.
 *  @li@c  t_page_addr                  -View page addr
 *  @li@c  e_preference_lang          - Render preference language
 *  @li@c  b_existed                      -Whether the page is existed.
 *  @li@c  b_flof_first                    -Whether the flof is first priority.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_WGL_VIEW_PAGE_T
{
    /* ID of the created Teletext widget. The value is from 0 */
    UINT8                   ui1_wgl_id;

    /* Page address */
    SM_TTX_PAGE_ADDR_T      t_page_addr;
    
    /* Render preference language */
    SM_TTX_LANG_CODE_T      e_preference_lang;

    /* Whether the page is existed */
    BOOL                    b_existed;

    /* Whether the flof is first priority. */
    BOOL                    b_flof_first;

} SM_TTXH_WGL_VIEW_PAGE_T;

/* Contains information of private notifucation */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_PRIVATE_NFY_INFO_T
 *  @brief This struct contains the private notify information
 *  @code
 *  typedef struct _SM_TTXH_PRIVATE_NFY_INFO_T
 *  {
 *      x_stream_nfy_fct    pf_private_nfy;
 *      VOID*               pv_private_nfy_tag;
 *  } SM_TTXH_PRIVATE_NFY_INFO_T;
 *  @endcode
 *  @li@c  pf_private_nfy               -TTX stream privated notify function
 *  @li@c  pv_private_nfy_tag         -TTX stream privated notify function tag information
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_PRIVATE_NFY_INFO_T
{
    x_stream_nfy_fct    pf_private_nfy;

    VOID*               pv_private_nfy_tag;
} SM_TTXH_PRIVATE_NFY_INFO_T;

/* Contains information of current page in the widget */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_WGL_GET_CUR_PAGE_T
 *  @brief This struct contains the getting current displaying page information
 *  @code
 *  typedef struct _SM_TTXH_WGL_GET_CUR_PAGE_T
 *  {
 *      UINT8                   ui1_wgl_id;
 *      SM_TTX_PAGE_ADDR_T      t_page_addr;
 *  } SM_TTXH_WGL_GET_CUR_PAGE_T;
 *  @endcode
 *  @li@c  ui1_wgl_id                      -ID of the created Teletext widget. The value is from 0
 *  @li@c  t_page_addr                    -Page address
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_WGL_GET_CUR_PAGE_T
{
    /* ID of the created Teletext widget. The value is from 0 */
    UINT8                   ui1_wgl_id;

    /* Page address */
    SM_TTX_PAGE_ADDR_T      t_page_addr;
} SM_TTXH_WGL_GET_CUR_PAGE_T;


/* Contains information for monitoring newsflash page update */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_MONITOR_NEWSFLASH_PAGE_T
 *  @brief This struct defines the newflash page to be monitored data information
 *  @code
 *  typedef struct _SM_TTXH_MONITOR_NEWSFLASH_PAGE_T
 *  {
 *      BOOL                            b_newsflash_nfy_enable;
 *      SM_TTX_PAGE_ADDR_T                  t_page_addr;
 *  } SM_TTXH_MONITOR_NEWSFLASH_PAGE_T;
 *  @endcode
 *  @li@c  b_newsflash_nfy_enable              -Indicate whether newsflash update notify enable or not
 *  @li@c  t_page_addr                              -The page to be monitored
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_MONITOR_NEWSFLASH_PAGE_T
{
    /* Indicate whether newsflash update notify enable or not */
    BOOL                            b_newsflash_nfy_enable;
    /*the page to be monitored*/
    SM_TTX_PAGE_ADDR_T                  t_page_addr;
} SM_TTXH_MONITOR_NEWSFLASH_PAGE_T;


/* Contains information for set Teletext widget */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_WGL_ATTR_T
 *  @brief This struct contains the widget command information that widget will be to do 
 *  @code
 *  typedef struct _SM_TTXH_WGL_ATTR_T
 *  {
 *      UINT8                   ui1_wgl_id;
 *      UINT32                  ui4_wgl_cmd;
 *      VOID*                   pv_wgl_do_cmd_param1;
 *      VOID*                   pv_wgl_do_cmd_param2;
 *  } SM_TTXH_WGL_ATTR_T;
 *  @endcode
 *  @li@c  ui1_wgl_id                               -ID of the created Teletext widget. The value is from 0
 *  @li@c  ui4_wgl_cmd                            -Widget command ID 
 *  @li@c  pv_wgl_do_cmd_param1            -Points data for Teletext widget do command
 *  @li@c  pv_wgl_do_cmd_param2            -Points data for Teletext widget do command
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_WGL_ATTR_T
{
    /* ID of the created Teletext widget. The value is from 0 */
    UINT8                   ui1_wgl_id;

    /* Command ID */
    UINT32                  ui4_wgl_cmd;
    
    /* Points data for Teletext widget do command */
    VOID*                   pv_wgl_do_cmd_param1;
    
    /* Points data for Teletext widget do command */
    VOID*                   pv_wgl_do_cmd_param2;
} SM_TTXH_WGL_ATTR_T;

/* Contains information of SM_TTXH_GET_TYPE_PAGE_BASIC_INFO */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_PAGE_BASIC_INFO_T
 *  @brief This struct contains ttx page basic information
 *  @code
 *  typedef struct _SM_TTXH_PAGE_BASIC_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                      t_page_addr;
 *      SM_TTX_PAGE_BASIC_INFO_T             t_basic_info;
 *  } SM_TTXH_PAGE_BASIC_INFO_T;
 *  @endcode
 *  @li@c  t_page_addr              -Page addr
 *  @li@c  t_basic_info               -The basic information of the special page
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_PAGE_BASIC_INFO_T
{
    SM_TTX_PAGE_ADDR_T                      t_page_addr;
    SM_TTX_PAGE_BASIC_INFO_T                t_basic_info;
} SM_TTXH_PAGE_BASIC_INFO_T;

/* Contains information of SM_TTXH_GET_TYPE_PAGE_FLOF_INFO */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_PAGE_FLOF_INFO_T
 *  @brief This struct contains the FLOF information for the special page
 *  @code
 *  typedef struct _SM_TTXH_PAGE_FLOF_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                      t_page_addr;
 *      SM_TTX_FLOF_INFO_T                      t_flof_info;
 *  } SM_TTXH_PAGE_FLOF_INFO_T;
 *  @endcode
 *  @li@c  t_page_addr                 -Page addr
 *  @li@c  t_flof_info                    -The flof information of the special page
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_PAGE_FLOF_INFO_T
{
    SM_TTX_PAGE_ADDR_T                      t_page_addr;
    SM_TTX_FLOF_INFO_T                      t_flof_info;
} SM_TTXH_PAGE_FLOF_INFO_T;

/* Contains information of SM_TTXH_GET_TYPE_PAGE_TOP_INFO */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_PAGE_TOP_INFO_T
 *  @brief This struct contains the TOP information for the special page
 *  @code
 *  typedef struct _SM_TTXH_PAGE_TOP_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                      t_page_addr;
 *      UTF16_T                                 awc_cur_block_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_next_block_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_prev_block_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_cur_group_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_next_group_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_prev_group_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_next_normal_page_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_prev_normal_page_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_next_page_name[SM_TTX_TOP_NAME_SIZE];
 *      UTF16_T                                 awc_prev_page_name[SM_TTX_TOP_NAME_SIZE];
 *      BOOL                                    ab_has_name[SM_TTX_PAGE_TOP_MAX_LINK_CNT];
 *      SM_TTX_PAGE_ADDR_T                      at_link_addr[SM_TTX_PAGE_TOP_MAX_LINK_CNT];
 *  } SM_TTXH_PAGE_TOP_INFO_T;
 *  @endcode
 *  @li@c  t_page_addr                                      -Page addr of the top information 
 *  @li@c  awc_cur_block_name[SM_TTX_TOP_NAME_SIZE]          -Contains current block name information
 *  @li@c  awc_next_block_name[SM_TTX_TOP_NAME_SIZE]         -Contains next block name information
 *  @li@c  awc_prev_block_name[SM_TTX_TOP_NAME_SIZE]         -Contains previous block name information
 *  @li@c  awc_cur_group_name[SM_TTX_TOP_NAME_SIZE]          -Contains current group name information
 *  @li@c  awc_next_group_name[SM_TTX_TOP_NAME_SIZE]         -Contain next group name information
 *  @li@c  awc_prev_group_name[SM_TTX_TOP_NAME_SIZE]         -Contain previous group name information
 *  @li@c  awc_next_normal_page_name[SM_TTX_TOP_NAME_SIZE]   -Contain next normal page name information
 *  @li@c  awc_prev_normal_page_name[SM_TTX_TOP_NAME_SIZE]   -Contain previous normal page name infromation
 *  @li@c  awc_next_page_name[SM_TTX_TOP_NAME_SIZE]          -Contain next page information
 *  @li@c  awc_prev_page_name[SM_TTX_TOP_NAME_SIZE]          -
 *  @li@c  ab_has_name[SM_TTX_PAGE_TOP_MAX_LINK_CNT]         -
 *  @li@c  at_link_addr[SM_TTX_PAGE_TOP_MAX_LINK_CNT]        -
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_PAGE_TOP_INFO_T
{
    /* input */
    SM_TTX_PAGE_ADDR_T                      t_page_addr;
    
    /* output */
    UTF16_T                                 awc_cur_block_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_next_block_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_prev_block_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_cur_group_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_next_group_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_prev_group_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_next_normal_page_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_prev_normal_page_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_next_page_name[SM_TTX_TOP_NAME_SIZE];
    UTF16_T                                 awc_prev_page_name[SM_TTX_TOP_NAME_SIZE];
    BOOL                                    ab_has_name[SM_TTX_PAGE_TOP_MAX_LINK_CNT];
    SM_TTX_PAGE_ADDR_T                      at_link_addr[SM_TTX_PAGE_TOP_MAX_LINK_CNT];
} SM_TTXH_PAGE_TOP_INFO_T;

/* Contains information of SM_TTXH_PAGE_SUB_PAGE_INFO_T */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_PAGE_SUB_PAGE_INFO_T
 *  @brief This struct contains the subpage information for the special page.
 *  @code
 *  typedef struct _SM_TTXH_PAGE_SUB_PAGE_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                      t_page_addr;
 *      BOOL                                    b_prev_subpage;
 *      SM_TTX_SUBPAGE_INFO_T                   t_subpage_info;
 *  } SM_TTXH_PAGE_SUB_PAGE_INFO_T; 
 *  @endcode
 *  @li@c  t_page_addr                    -The current page num.
 *  @li@c  b_prev_subpage              -True,the subpage information is for previous of current 
 page. Fale,the next subpage information of current page.
 *  @li@c  t_subpage_info               -Contain the subpage information for current page.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_PAGE_SUB_PAGE_INFO_T
{
    SM_TTX_PAGE_ADDR_T                      t_page_addr;
    BOOL                                    b_prev_subpage;
    SM_TTX_SUBPAGE_INFO_T                   t_subpage_info;
} SM_TTXH_PAGE_SUB_PAGE_INFO_T; 

/* Contains information of SM_TTXH_TOP_BLOCK_INFO_T */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_TOP_BLOCK_INFO_T
 *  @brief This struct contains the top block information.
 *  @code
 *  typedef struct _SM_TTXH_TOP_BLOCK_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
 *      SM_TTX_TOP_BLOCK_TYPE_T                 e_block_type;
 *      UINT8                                   ui1_block_num;
 *      SM_TTX_PAGE_ADDR_T                      at_block_addr[SM_TTX_TOP_BLOCK_MAX_LINK_CNT];
 *      BOOL                                    ab_has_name[SM_TTX_TOP_BLOCK_MAX_LINK_CNT];
 *      UTF16_T                                 awc_block_name[SM_TTX_TOP_BLOCK_MAX_LINK_CNT]
 *                                                  [SM_TTX_TOP_NAME_SIZE];
 *  } SM_TTXH_TOP_BLOCK_INFO_T; 
 *  @endcode
 *  @li@c  t_idx_page_addr                                   -The index page address.
 *  @li@c  e_block_type                                      -The block type.
 *  @li@c  ui1_block_num                                     -Indicate the blocks number.
 *  @li@c  at_block_addr[SM_TTX_TOP_BLOCK_MAX_LINK_CNT]      -The block page addr information.
 *  @li@c  ab_has_name[SM_TTX_TOP_BLOCK_MAX_LINK_CNT]        -Contain the associated blcok has name 
 or not.
  *  @li@c awc_block_name[SM_TTX_TOP_BLOCK_MAX_LINK_CNT][SM_TTX_TOP_NAME_SIZE]   -Contain the 
  associated block name information.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_TOP_BLOCK_INFO_T
{
    /* input */
    SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
    SM_TTX_TOP_BLOCK_TYPE_T                 e_block_type;

    /* output */
    UINT8                                   ui1_block_num;
    SM_TTX_PAGE_ADDR_T                      at_block_addr[SM_TTX_TOP_BLOCK_MAX_LINK_CNT];
    BOOL                                    ab_has_name[SM_TTX_TOP_BLOCK_MAX_LINK_CNT];
    UTF16_T                                 awc_block_name[SM_TTX_TOP_BLOCK_MAX_LINK_CNT][SM_TTX_TOP_NAME_SIZE];
} SM_TTXH_TOP_BLOCK_INFO_T; 

/* Contains information of SM_TTXH_GET_TYPE_TOP_GROUP_INFO */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_TOP_GROUP_INFO_T
 *  @brief  This struct contains the group information for the top.
 *  @code
 *  typedef struct _SM_TTXH_TOP_GROUP_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
 *      UINT8                                   ui1_group_num;
 *      SM_TTX_PAGE_ADDR_T                      at_group_addr[SM_TTX_TOP_GROUP_MAX_LINK_CNT];
 *      BOOL                                    ab_has_name[SM_TTX_TOP_GROUP_MAX_LINK_CNT];
 *      UTF16_T                                 awc_group_name[SM_TTX_TOP_GROUP_MAX_LINK_CNT]
 *                                                      [SM_TTX_TOP_NAME_SIZE];
 *  } SM_TTXH_TOP_GROUP_INFO_T; 
 *  @endcode
 *  @li@c  t_idx_page_addr                                   -The special page addr.
 *  @li@c  ui1_group_num                                     -Contains the group numbers.
 *  @li@c  at_group_addr[SM_TTX_TOP_GROUP_MAX_LINK_CNT]      -Contains group page addr information.
 *  @li@c  ab_has_name[SM_TTX_TOP_GROUP_MAX_LINK_CNT]        -Contians the associated group has 
 name or not.
  *  @li@c awc_group_name[SM_TTX_TOP_GROUP_MAX_LINK_CNT][SM_TTX_TOP_NAME_SIZE]  -Contains the group 
  name information.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_TOP_GROUP_INFO_T
{
    /* input */
    SM_TTX_PAGE_ADDR_T           t_idx_page_addr;

    /* output */
    UINT8                        ui1_group_num;
    SM_TTX_PAGE_ADDR_T           at_group_addr[SM_TTX_TOP_GROUP_MAX_LINK_CNT];
    BOOL                         ab_has_name[SM_TTX_TOP_GROUP_MAX_LINK_CNT];
    UTF16_T                      awc_group_name[SM_TTX_TOP_GROUP_MAX_LINK_CNT][SM_TTX_TOP_NAME_SIZE];
} SM_TTXH_TOP_GROUP_INFO_T; 

/* Contains information of SM_TTXH_GET_TYPE_TOP_GROUP_PAGE_INFO */
/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_TOP_GROUP_PAGE_INFO_T
 *  @brief This struct contains the pages information of special top group.
 *  @code
 *  typedef struct _SM_TTXH_TOP_GROUP_PAGE_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
 *      UINT8                                   ui1_page_num;
 *      SM_TTX_PAGE_ADDR_T                      at_page_addr[SM_TTX_TOP_GROUP_PAGE_LINK_CNT];
 *      BOOL                                    ab_has_name[SM_TTX_TOP_GROUP_PAGE_LINK_CNT];
 *      UTF16_T                                 awc_page_name[SM_TTX_TOP_GROUP_PAGE_LINK_CNT]
 *                                                  [SM_TTX_TOP_NAME_SIZE];
 *  } SM_TTXH_TOP_GROUP_PAGE_INFO_T; 
 *  @endcode
 *  @li@c  t_idx_page_addr                                   -The page addr of special group.
 *  @li@c  ui1_page_num                                      -The page numbers in the special group.
 *  @li@c  at_page_addr[SM_TTX_TOP_GROUP_PAGE_LINK_CNT]      -Contains the page addr information 
 of the special group.
 *  @li@c  ab_has_name[SM_TTX_TOP_GROUP_PAGE_LINK_CNT]       -Contains the associates page has name 
 information or not.
  *  @li@c awc_page_name[SM_TTX_TOP_GROUP_PAGE_LINK_CNT][SM_TTX_TOP_NAME_SIZE]    -Contains the 
  page name information of the special group.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_TOP_GROUP_PAGE_INFO_T
{
    /* input */
    SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
    
    /* output */
    UINT8                                   ui1_page_num;
    SM_TTX_PAGE_ADDR_T                      at_page_addr[SM_TTX_TOP_GROUP_PAGE_LINK_CNT];
    BOOL                                    ab_has_name[SM_TTX_TOP_GROUP_PAGE_LINK_CNT];
    UTF16_T                                 awc_page_name[SM_TTX_TOP_GROUP_PAGE_LINK_CNT][SM_TTX_TOP_NAME_SIZE];
} SM_TTXH_TOP_GROUP_PAGE_INFO_T; 


/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_TOP_AIT_INFO_T
 *  @brief This struct contains AIT information of the special top.The max number of AIT is 
 SM_TTX_TOP_AIT_MAX_LINK_CNT.
 *  @code
 *  typedef struct _SM_TTXH_TOP_AIT_INFO_T
 *  {
 *      UINT16                                  ui2_ait_idx;
 *      UINT8                                   ui1_page_num;
 *      SM_TTX_PAGE_ADDR_T                      at_page_addr[SM_TTX_TOP_AIT_MAX_LINK_CNT];
 *      UTF16_T                                 awc_page_name[SM_TTX_TOP_AIT_MAX_LINK_CNT]
 *                                                  [SM_TTX_TOP_NAME_SIZE];
 *  } SM_TTXH_TOP_AIT_INFO_T; 
 *  @endcode
 *  @li@c  ui2_ait_idx                                       -The index of AIT.
 *  @li@c  ui1_page_num                                      -The AIT page numbers of the specail 
 AIT.
 *  @li@c  at_page_addr[SM_TTX_TOP_AIT_MAX_LINK_CNT]         -Contains the page addr information of 
 the associate AIT.
 *  @li@c awc_page_name[SM_TTX_TOP_AIT_MAX_LINK_CNT][SM_TTX_TOP_NAME_SIZE]    -Contains the page 
 names of the associated AIT page.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_TOP_AIT_INFO_T
{
    /* input / output */
    UINT16                                  ui2_ait_idx;
    
    /* output */
    UINT8                                   ui1_page_num;
    SM_TTX_PAGE_ADDR_T                      at_page_addr[SM_TTX_TOP_AIT_MAX_LINK_CNT];
    UTF16_T                                 awc_page_name[SM_TTX_TOP_AIT_MAX_LINK_CNT][SM_TTX_TOP_NAME_SIZE];
} SM_TTXH_TOP_AIT_INFO_T; 



/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_SUBTITLE_PAGE_INFO_T
 *  @brief This struct contains subtitle page information .
 *  @code
 *  typedef struct _SM_TTXH_SUBTITLE_PAGE_INFO_T
 *  {
 *      SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
 *      SM_TTX_SUBTITLE_PAGE_INFO_T             t_subtitle_page_info;
 *  } SM_TTXH_SUBTITLE_PAGE_INFO_T; 
 *  @endcode
 *  @li@c  t_idx_page_addr                                   -The indext page addr.
 *  @li@c  t_subtitle_page_info                              -The subtitle page information.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_SUBTITLE_PAGE_INFO_T
{
    SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
    SM_TTX_SUBTITLE_PAGE_INFO_T             t_subtitle_page_info;
} SM_TTXH_SUBTITLE_PAGE_INFO_T; 

/* Contains information of SM_TTXH_GET_SIBLING_PAGE */
/*------------------------------------------------------------------*/
/*! @enum SM_TTXH_SIBLING_PAGE_TYPE
 *  @brief This contains the slbling page type.
 *  @code
 *  typedef enum _SM_TTXH_SIBLING_PAGE_TYPE
 *  {
 *      SM_TTXH_SIBLING_PAGE_TYPE_SEQ,
 *      SM_TTXH_SIBLING_PAGE_TYPE_EXIST,
 *      SM_TTXH_SIBLING_PAGE_TYPE_EXIST_SEQ,
 *      SM_TTXH_SIBLING_PAGE_TYPE_TOP_SEQ,
 *      SM_TTXH_SIBLING_PAGE_TYPE_TOP_EXIST_SEQ
 *  } SM_TTXH_SIBLING_PAGE_TYPE;
 *  @endcode
 *  @li@c  SM_TTXH_SIBLING_PAGE_TYPE_SEQ                     -The sequence type of sibling page.The 
 page maybe not exist in the stream.
 *  @li@c  SM_TTXH_SIBLING_PAGE_TYPE_EXIST                   -The exist type of the sibling page 
 that is exist in the stream.
 *  @li@c  SM_TTXH_SIBLING_PAGE_TYPE_EXIST_SEQ               -The sequence and exist sibling page 
 type.
 *  @li@c  SM_TTXH_SIBLING_PAGE_TYPE_TOP_SEQ                 -The sequence top page type of the 
 sibling.
 *  @li@c  SM_TTXH_SIBLING_PAGE_TYPE_TOP_EXIST_SEQ           -The sequence and exist top sibling 
 page type.
 */
/*------------------------------------------------------------------*/
typedef enum _SM_TTXH_SIBLING_PAGE_TYPE
{
    SM_TTXH_SIBLING_PAGE_TYPE_SEQ,
    SM_TTXH_SIBLING_PAGE_TYPE_EXIST,
    SM_TTXH_SIBLING_PAGE_TYPE_EXIST_SEQ,
    SM_TTXH_SIBLING_PAGE_TYPE_TOP_SEQ,
    SM_TTXH_SIBLING_PAGE_TYPE_TOP_EXIST_SEQ
} SM_TTXH_SIBLING_PAGE_TYPE;

/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_SIBLING_PAGE_INFO_T
 *  @brief This struct contains the sibling page informtion for the special page.
 *  @code
 *  typedef struct _SM_TTXH_SIBLING_PAGE_INFO_T
 *  {
 *      SM_TTXH_SIBLING_PAGE_TYPE               e_sibling_page_type;
 *      SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
 *      SM_TTX_PAGE_ADDR_T                      t_next_page_addr;
 *      SM_TTX_PAGE_ADDR_T                      t_prev_page_addr;
 *  } SM_TTXH_SIBLING_PAGE_INFO_T;
 *  @endcode
 *  @li@c  e_sibling_page_type                 -The slbling page type ,can see  SM_TTXH_SIBLING_PAGE_TYPE.
 *  @li@c  t_idx_page_addr                      -The special index page addr.
 *  @li@c  t_next_page_addr                     -The next sibling page.
 *  @li@c  t_prev_page_addr                     -The previous sibling page.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_SIBLING_PAGE_INFO_T
{
    SM_TTXH_SIBLING_PAGE_TYPE               e_sibling_page_type;
    SM_TTX_PAGE_ADDR_T                      t_idx_page_addr;
    SM_TTX_PAGE_ADDR_T                      t_next_page_addr;
    SM_TTX_PAGE_ADDR_T                      t_prev_page_addr;
} SM_TTXH_SIBLING_PAGE_INFO_T;

/**It define the max num degree of brightness,if it using SM_TTXH_BRIGHTNESS_TYPE_INDX type*/
#define SM_TTX_BRIGHTNESS_DEGREE_MAX_NUM      10    

/*------------------------------------------------------------------*/
/*! @enum SM_TTXH_SET_BRIGHTNESS_TYPE
 *  @brief This struct contains the brightness command type.
 *  @code
 *  typedef enum _SM_TTXH_SET_BRIGHTNESS_TYPE
 *  {
 *      SM_TTXH_BRIGHTNESS_TYPE_INDX, 
 *      SM_TTXH_BRIGHTNESS_TYPE_CAL
 *  } SM_TTXH_SET_BRIGHTNESS_TYPE;
 *  @endcode
 *  @li@c  SM_TTXH_BRIGHTNESS_TYPE_INDX                      -The brightness value is by 
 index type.
 *  @li@c  SM_TTXH_BRIGHTNESS_TYPE_CAL                       -The brightness value is by calculated.
 */
/*------------------------------------------------------------------*/
typedef enum _SM_TTXH_SET_BRIGHTNESS_TYPE
{
    SM_TTXH_BRIGHTNESS_TYPE_INDX, 
    SM_TTXH_BRIGHTNESS_TYPE_CAL
} SM_TTXH_SET_BRIGHTNESS_TYPE;

/*------------------------------------------------------------------*/
/*! @struct SM_TTXH_BRIGHTNESS_DEGREE_T
 *  @brief This struct contains the brightness degree informtion to set.
 *  @code
 *  typedef struct _SM_TTXH_BRIGHTNESS_DEGREE_T
 *  {
 *      SM_TTXH_SET_BRIGHTNESS_TYPE             e_brightness_type;
 *      UINT8                                   ui1_wgl_id;
 *      UINT8                                   ui1_brightness;
 *      BOOL                                    b_update_page;
 *  } SM_TTXH_BRIGHTNESS_DEGREE_T;
 *  @endcode
 *  @li@c  e_brightness_type                                 -The set brightness command type.It 
 can see the SM_TTXH_SET_BRIGHTNESS_TYPE.
 *  @li@c  ui1_wgl_id                                        -The setting  teletext widget id.
 *  @li@c  ui1_brightness                                    -If using SM_TTXH_BRIGHTNESS_TYPE_INDX,ui1_brightness max value is SM_TTX_BRIGHTNESS_DEGREE_MAX_NUM.
          else ui1_brightness value is from 0~100%*.
 *  @li@c  b_update_page                                     -If TRUE,the engine will do update page action; other,it do not update 
 page.
 */
/*------------------------------------------------------------------*/
typedef struct _SM_TTXH_BRIGHTNESS_DEGREE_T
{
    SM_TTXH_SET_BRIGHTNESS_TYPE             e_brightness_type;

    UINT8                                   ui1_wgl_id;

    UINT8                                   ui1_brightness;

    BOOL                                    b_update_page;
} SM_TTXH_BRIGHTNESS_DEGREE_T;


#endif /* _U_SM_TTX_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                /* end of groupMW_STRM_MNGR_TTX_HDLR */
/*----------------------------------------------------------------------------*/



