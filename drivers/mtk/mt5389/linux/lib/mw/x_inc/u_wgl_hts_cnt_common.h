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
/*! @file u_wgl_hts_cnt_common.h 
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
/*! @addtogroup groupMW_WGL_HTS
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_WGL_HTS_CNT_COMMON_H_
#define _U_WGL_HTS_CNT_COMMON_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* commands */
#define WGL_CMD_HTS_SET_CNT_INSET                   WGL_MAKE_CONTENT_CMD(WGL_CONTENT_HTS_DEF, 100)    
/**<
This command sets the content inset.  If this command changes the size of the viewport and the article is moved, the notification WGL_NC_HTS_VP_MOVED would be arisen.

Input arguments

pv_param1	Contains the content inset. This argument must be of type (WGL_INSET_T *) and cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_GET_CNT_INSET                   WGL_MAKE_CONTENT_CMD(WGL_CONTENT_HTS_DEF, 101) 
/**< 
This command gets the content inset. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single WGL_INSET_T buffer to receive the content inset. Note that this argument must be of type (WGL_INSET_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/

#define WGL_CMD_HTS_SET_PAGE_INDC_IMG               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_HTS_DEF, 102) 
/**<  
This command sets the information for page-up / page-down indication images. 

Input arguments

pv_param1	Contains the information for paging indication images. This argument must be of type (WGL_HTS_PAGE_INDC_INFO_T *) and cannot be NULL.

pv_param2	Reserved. It must be NULL.

Returns

WGLR_OK	The function call is successful.

WGLR_INV_ARG	The argument pv_param1 is NULL. 
*/

#define WGL_CMD_HTS_GET_PAGE_INDC_IMG               WGL_MAKE_CONTENT_CMD(WGL_CONTENT_HTS_DEF, 103) 
/**< 
This command gets the information for page-up / page-down indication images. 

Input arguments

pv_param2	Reserved. It must be NULL.

Output arguments

pv_param1	Contains the pointer of a single WGL_HTS_PAGE_INDC_INFO_T buffer to receive the paging image information. Note that this argument must be of type (WGL_HTS_PAGE_INDC_INFO_T *) and cannot be NULL.

Returns

WGLR_OK	The function call is successful.

WGL_INV_ARG	The argument pv_param1 is NULL.
*/


#define WGL_CLR_HTS_VP_BK                           (WGL_CLR_WGT_SPF_START+5) 
/**< 
Indicates the color is for viewport background. This definition could be used by the following commands: 
 	WGL_CMD_GL_SET_COLOR 
 	WGL_CMD_GL_GET_COLOR 
*/

#define WGL_CLR_HTS_SEC_TEXT                        (WGL_CLR_WGT_SPF_START+6) 
/**< 
Indicates the color is for the text of a section. This definition could be used by the following commands: 

 	WGL_CMD_HTS_DFT_SEC_SET_COLOR

 	WGL_CMD_HTS_DFT_SEC_GET_COLOR

 	WGL_CMD_HTS_SEC_SET_COLOR

 	WGL_CMD_HTS_SEC_GET_COLOR
*/

#define WGL_CLR_HTS_SEC_TEXT_BK                     (WGL_CLR_WGT_SPF_START+7)   
/**<
Indicates the color is for the text background of a section. This definition could be used by the following commands: 
 	WGL_CMD_HTS_DFT_SEC_SET_COLOR

 	WGL_CMD_HTS_DFT_SEC_GET_COLOR

 	WGL_CMD_HTS_SEC_SET_COLOR

 	WGL_CMD_HTS_SEC_GET_COLOR
*/


#define WGL_IMG_HTS_VP_BK                           (WGL_IMG_WGT_SPF_START+10)   
/**< 
Indicates the template image is for drawing the viewport background. This definition could be used by the following commands: 

 	WGL_CMD_GL_SET_IMAGE 

 	WGL_CMD_GL_GET_IMAGE 
*/

/*-----------------------------------------------------------------------------
                    structure
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @struct WGL_HTS_PAGE_INDC_INFO_T
 *  @brief  This structure holds the information for page-up/page-down indication images. This structure is defined as general content structure.   
 *  @code
 *  typedef struct _WGL_HTS_PAGE_INDC_INFO_T
 *  {
 *  	GL_POINT_T      		            t_location_up;
 *  	GL_POINT_T      		            t_location_down;
 *  	WGL_HIMG_TPL_T  		            h_img_up_on;
 *  	WGL_HIMG_TPL_T  		            h_img_up_off;
 *  	WGL_HIMG_TPL_T  		            h_img_down_on;
 *  	WGL_HIMG_TPL_T  		            h_img_down_off;
 *  } WGL_HTS_PAGE_INDC_INFO_T;    
 *  @endcode
 *  @li@c  t_location_up                                     -Indicates the location of the page-up or scroll-up images. 
 *  @li@c  t_location_down                                   -Indicates the location of the page-down or scroll-down images.
 *  @li@c  h_img_up_on                                       -Contains the handle of the page-up or scroll-up image for "on" state. The "on" state indicates there are one or more lines above the current first visible line in the viewport and thus the article could be paged up or scrolled up.
 *  @li@c  h_img_up_off                                      -Contains the handle of the page-up or scroll-up image for "off" state. The "off" state indicates the viewport already reaches the top of the article and thus the article could not be paged up or scrolled up anymore.
 *  @li@c  h_img_down_on                                     -Contains the handle of the page-down or scroll-down image for "on" state. The "on" state indicates there are one or more lines below the current last visible line in the viewport and thus the article could be paged down or scrolled down.
 *  @li@c  h_img_down_off                                    -Contains the handle of the page-down or scroll-down image for "off" state. The "off" state indicates the viewport already reaches the bottom of the article and thus the article could not be paged down or scrolled down anymore.
 */
/*------------------------------------------------------------------*/
typedef struct _WGL_HTS_PAGE_INDC_INFO_T
{
	GL_POINT_T      		            t_location_up;
	GL_POINT_T      		            t_location_down;

	WGL_HIMG_TPL_T  		            h_img_up_on;
	WGL_HIMG_TPL_T  		            h_img_up_off;
	WGL_HIMG_TPL_T  		            h_img_down_on;
	WGL_HIMG_TPL_T  		            h_img_down_off;
} WGL_HTS_PAGE_INDC_INFO_T;    




#ifdef __cplusplus
}
#endif


#endif

/*----------------------------------------------------------------------------*/
/*! @} */   /* end of groupMW_WGL_HTS*/
/*----------------------------------------------------------------------------*/


