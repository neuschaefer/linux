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
/*-----------------------------------------------------------------------------
 *---------------------------------------------------------------------------*/

/** @file mtvss.h
 *  This header file declares exported APIs of VSS module.
 */
#ifndef MT_VSS_H
#define MT_VSS_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifndef MT_TYPE_H
typedef char           CHAR;
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned long  UINT32;
typedef UINT8          BOOL; 

/**MTAL error code
*/
typedef enum
{
    MTR_BUFFER_FULL           =  5,
    MTR_CARD_READY            =  4,
    MTR_POD_CI_ACTIVE         =  3,
    MTR_DA_HIGH               =  2,
    MTR_CARD_DETECTED         =  1,
    MTR_OK                    =  0,   ///< Success
    MTR_NOT_OK                = -1,   ///< Not ok
    MTR_NOT_SUPPORTED         = -2,   ///< Not support
    MTR_PARAMETER_ERROR       = -3,   ///< Parameter error
    MTR_DONT_CARE             = -4,   ///< Don't care
    MTR_ERR_INV               = -5,   ///< Invalid argument passed
    MTR_ERR_TIMEDOUT          = -6,   ///< Timeout occured
    MTR_PARAMETER_NOCHANGE    = -7,   ///< Paramenter no change

    MTR_OSD_NO_REGION         = -11,  ///< No region attach
    MTR_OSD_INV_REGION        = -12,  ///< Invalid region
    MTR_OSD_INV_LIST          = -13,  ///< Invalid list
    MTR_OSD_INV_PLANE         = -14,  ///< Invalid plane
    MTR_OSD_INV_SCALER        = -15,  ///< Invalid scaler
    MTR_OSD_INV_ATTRIBUTE     = -16   ///< Invalid region attribute
} MT_RESULT_T;

/** Video Path
*/
typedef enum {
  MTVDO_MAIN,
  MTVDO_SUB,
  MTVDO_MAX
} MTVDO_PATH_T;

#ifdef __cplusplus
    #define EXTERN          extern "C"
#else
    #define EXTERN          extern
#endif
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    MTVSS_CID_BT709 = 0,
    MTVSS_CID_BT601 = 1,
	MTVSS_CID_MAX
} MTVSS_COLOR_SPACE_T;

typedef enum
{
	MTVSS_INPUT_TYPE_HDMI = 0,
    MTVSS_INPUT_TYPE_COMPOSITE = 1,
    MTVSS_INPUT_TYPE_COMPONENT = 2,
    MTVSS_INPUT_TYPE_COAXIAL   = 3,
    MTVSS_INPUT_TYPE_SVIDEO    = 4,
    MTVSS_INPUT_TYPE_VGA       = 5,
	MTVSS_INPUT_TYPE_MAX
} MTVSS_INPUT_TYPE_T;

typedef struct 
{
    CHAR *make;
    CHAR *model_id;
    CHAR *model_description;
    CHAR *firmware_version;
} platform_info_t;

EXTERN BOOL DRV_Init(void);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
/**
 * @brief 
 It initialized the part pertaining to the Video auxiliary outputs. 
 * @param 
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTVSS_Init(void);

EXTERN MT_RESULT_T MTVSS_SetColorFormat(UINT32 ColorFmt);
/**
 * @brief 
 It connects the VSS connection settings.
 * @param eVdpId
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTVSS_Connect(MTVDO_PATH_T eVdpId);
 
/**
 * @brief 
 It releases the VSS connection settings.
 * @param eVdpId
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */

EXTERN MT_RESULT_T MTVSS_Disconnect(MTVDO_PATH_T eVdpId);

EXTERN MT_RESULT_T MTVSS_GetVideoInfo(UINT16 *width, UINT16 *height, UINT16 *color, UINT16 *source);

EXTERN MT_RESULT_T MTVSS_GetPlatformInfo(platform_info_t *info);

EXTERN MT_RESULT_T MTVSS_GetFrameBuffer(UINT16 roi_x, UINT16 roi_y, UINT16 roi_width, UINT16 roi_height, UINT8 *buffer);

EXTERN MT_RESULT_T MTVSS_SetFrameCallback(void (*newframe)(void));

/**
 * @brief 
 The Manual Debug function of MTVSS API.
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
//void MTVSS_Debug(void);

#ifdef __cplusplus
}
#endif

#endif

