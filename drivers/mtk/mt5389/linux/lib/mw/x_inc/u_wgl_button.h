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
/*! @file u_wgl_button.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains public interfaces, constant and type 
 *         definitions of the Widget Library.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_WGL_BUTTON   Button Widget  
 *
 *  @ingroup groupMW_WGL
 *  @brief Provides definitions and exported function of the widget button library.
 *
 *  %MW Application API would be used by TV Application, lot of these APIs would
 *   be wrapped from Exported API
 *
 *  @see groupMW_WGL
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

 
#ifndef _U_WGL_BUTTON_H_
#define _U_WGL_BUTTON_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_wgl.h"
#include "u_wgl_tools.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define WGL_BTN_MAX_STRING                      ((UINT16) 128) /**<Indicate the max length of buttom string */

/*Specific Widget Style*/
/**If this style is assigned, it means the Button widget switches its state between pushed and 
unpushed when it receives enter-key-down message; otherwise, it switches to pushed state when it 
receives enter-key-down message, and to unpushed state when it receives enter-key-up message.*/
#define WGL_STL_BTN_CHECKBOX                    (((UINT32)(WGL_STL_WGT_SPF_START))<<0)    

/**If this style is assigned, it means the Button read text from left. */
#define WGL_STL_BTN_READ_TEXT_FROM_LEFT         (((UINT32)(WGL_STL_WGT_SPF_START))<<1)    

/**If this style is assigned, it means the Button read text from right. */
#define WGL_STL_BTN_READ_TEXT_FROM_RIGHT        (((UINT32)(WGL_STL_WGT_SPF_START))<<2)    


/*Display Order*/
#define WGL_BTN_DO_IMG_FIRST                    ((UINT8)0x00)    /**<If this style is assigned, Button's string will follow its image. */
#define WGL_BTN_DO_TEXT_FIRST                   ((UINT8)0x01)    /**<If this style is assigned, Button's image will follow its string. */

/*Specific Command*/
/**
This command is used to change a Button to the pushed/unpushed state.

Input arguments

pv_param1	TRUE to change to pushed state and FALSE to change to unpushed state. 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 isn't TRUE or FALSE.
*/
#define WGL_CMD_BTN_SET_PUSHED                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_BUTTON, 0) 

/**
This command is used to check whether a Button is pushed or not.

Input arguments

pv_param1	Pointer to a BOOL variable to receive the result. If it returns TRUE, it means the 
Button is pushed; otherwise, it is unpushed. 

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BTN_GET_PUSHED                  WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_BUTTON, 1)    

/**
This command is used to get minimum widget size, which includes minimum sizes of content and 
border. Note: this command isn't supported in this version.

Input arguments

pv_param1	Pointer of a GL_SIZE_T to receive the size. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BTN_GET_MIN_SIZE                WGL_MAKE_WIDGET_CMD(HT_WGL_WIDGET_BUTTON, 2)    



/*Specific Command Supported by BUTTON-Content*/
/**
This command is used to set Button's foreground alignment.

Input arguments

pv_param1	Specifies the alignment style. It can be from WGL_AS_LEFT_TOP to WGL_AS_RIGHT_BOTTOM. 
About the explanation of these alignment styles.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The style specified by pv_param1 is invalid.
*/
#define WGL_CMD_BTN_SET_ALIGN                   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 0)   

/**
This command returns Button's foreground alignment.

Input arguments

pv_param1	Pointer to a UINT8 to receive its alignment style. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BTN_GET_ALIGN                   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 1)   

/**
This command is used to set the string displayed in Button.

Input arguments

pv_param1	Pointer to a UTF16_T string. It must be a null terminated string and cannot be over 
WGL_BTN_MAX_STRING (128, the null character isn't included). If it is NULL, it means to remove 
the old string.

pv_param2	A UINT16 integer to specify the character length of pv_param1 (the null character isn't 
included).

Returns

WGLR_OK	The function call is successful.

WGLR_BUFFER_NOT_ENOUGH	This error is caused when the length specified by pv_param2 is larger than 
WGL_BTN_MAX_STRING.
*/
#define WGL_CMD_BTN_SET_TEXT                    WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 2)   

/**
This command is used to get Button's string.

Input arguments

pv_param1	Pointer to a buffer, which the length should not be less than the string length stored 
in the Button (the null character isn't included).

pv_param2	A UINT16 integer to specify the length in character of pv_param1.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.

WGLR_BUFFER_NOT_ENOUGH	This error is caused when the length specified by pv_param2 is less than 
the string length stored in the Button.
*/
#define WGL_CMD_BTN_GET_TEXT                    WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 3)   

/**
This command is used to set the horizontal interval between Button's string and image.

Input arguments

pv_param1	An INT32 integer specifies the horizontal interval.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.
*/
#define WGL_CMD_BTN_SET_DISTANCE                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 4)   

/**
This command returns the horizontal interval between Button's string and image.

Input arguments

pv_param1	Pointer to an INT32 variable to receive horizontal interval. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BTN_GET_DISTANCE                WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 5)   

/**
This command is used to set the display margin of Button's foreground. The i4_left is for the 
left margin, the i4_right is for the right margin, the i4_top is for the top margin, and the 
i4_bottom is for the bottom margin. All of them cannot be less than 0.

Input arguments

pv_param1	Pointer to a WGL_INSET_T value to specify the display margin. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	pv_param1 is NULL, or the sum of i4_left and i4_right isn't less than Icon Static's 
width, or the sum of i4_top and i4_bottom isn't less than Button's height.
*/
#define WGL_CMD_BTN_SET_CNT_INSET               WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 6)   

/**
This command returns the display margin of Button's foreground. The i4_left is the left margin, 
the i4_right is the right margin, the i4_top is the top margin, and the i4_bottom is the bottom 
margin. 

Input arguments

pv_param1	Pointer to a WGL_INSET_T value to receive the display margin. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BTN_GET_CNT_INSET               WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 7)   

/**
This command is used to set the display order of a Button.

Input arguments

pv_param1	Specifies the display order. The possible values are WGL_PB_DO_IMG_FIRST and 
WGL_PB_DO_TEXT_FIRST, and WGL_PB_DO_TEXT_FIRST means the display string is followed by display 
image, whereas display image is followed by display string.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The order specified by pv_param1is invalid.
*/
#define WGL_CMD_BTN_SET_ORDER                   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 8)   

/**
This command returns the display order of a Button.

Input arguments

pv_param1	Pointer to a UINT8 variable to receive the display order. It cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CMD_BTN_GET_ORDER                   WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 9)   

/**
This command returns the necessary width of a Button.

Input arguments

pv_param1	The button state.

pv_param2	Point to the width value.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/
#define WGL_CNT_BTN_GET_NECESSARY_SIZE          WGL_MAKE_CONTENT_CMD(HT_WGL_WIDGET_BUTTON, 10)  


#define BTN_CNT_PRIVATE_START                   11      /**<(WGL_CNT_BTN_GET_NECESSARY_SIZE - WGL_CMD_BTN_SET_ALIGN + 1)*/    


/*Specific Notify Code*/
#define WGL_NC_BTN_PUSHED                       ((UINT32)WGL_NC_WIDGET_SPF_START + 0)    /**< This notification is sent when a Button changes to pushed state.*/
#define WGL_NC_BTN_UNPUSHED                     ((UINT32)WGL_NC_WIDGET_SPF_START + 1)    /**< This notification is sent when a Button changes to unpushed state.*/

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif


/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_BUTTON*/
/*----------------------------------------------------------------------------*/



