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

/** @file mttve.h
 *  This header file declares exported APIs of TVE module.
 */
#ifndef MT_TVE_H
#define MT_TVE_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
#include "mtvdecex.h"
  
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
	MTTVE_VBI_DATA_CC = 0, ///< Closed Caption
	MTTVE_VBI_DATA_XDS, ///< Extended Data Service
	MTTVE_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
	MTTVE_VBI_DATA_WSS, ///< Analog SdUserData is WSS	
	MTTVE_VBI_DATA_MAX
} MTTVE_VBIDATATYPE_T; ///< VBI Data Type

typedef enum
{
	MTTVE_DAC1 = 0,
	MTTVE_DAC2,
	MTTVE_ID_MAX
} MTTVE_ID_T;

typedef enum
{
	MTTVE_480i = 0,
	MTTVE_576i,
	MTTVE_FMT_MAX
} MTTVE_FMT_T;
typedef enum
{
	MTTVE_NTSC_358 = 0,
	MTTVE_PAL_M,
	MTTVE_PAL_NC,
	MTTVE_PAL_BDGHI,
	MTTVE_PAL_60,
	MTTVE_NTSC_443,
	MTTVE_MODE_MAX
} MTTVE_MODE_T;

typedef struct 
{
    UINT16 usCurPicWidth;
    UINT16 usCurPicHeight;
    BOOL   fgHavingSideBar;
    BOOL   fgHavingLetterBox;
    BOOL   fgDuringScaling;
} scaling_info_t;

typedef enum
{
    MTTVE_HSCALING_RESET_EFFECTIVE_SIZE = 0xffff,
    MTTVE_HCENTER_RESET_CUT_SIZE = 0xffff,
    MTTVE_LETTERBOX_RESET_CUT_SIZE = 0xffff
} MTTVE_SCALING_RESET_T; 

typedef enum
{
	MTTVE_FIELD_ODD = 0,
	MTTVE_FIELD_EVEN,
	MTTVE_FIELD_MAX
} MTTVE_FIELD_T;

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
EXTERN MT_RESULT_T MTTVE_Init(VOID);

/**
 * @brief 
 It connects the TVE component with the Input Source.
 * @param eTveId
 * @param eSRC input source
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_Connect(MTTVE_ID_T eTveId, MTVDECEX_SRC_T eSRC);
 
 /**
 * @brief 
It releases the TVE connection settings.
 * @param eTveId
 * @param eSRC input source
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_Disconnect(MTTVE_ID_T eTveId, MTVDECEX_SRC_T eSRC);

/**
 * @brief 
 It starts the Video auxiliary outputs (CVBS outputs) 
 * @param eTveId
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_VideoStart(MTTVE_ID_T eTveId);

/**
 * @brief 
 It stops the Video auxiliary outputs (CVBS output).
 * @param eTveId
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_VideoStop(MTTVE_ID_T eTveId);

/** 
* @brief Video 
It makes the Video auxiliary outputs mute On/Off. 
* @param eTveId 
* @param fgOnOff TRUE : Mute On, FALSE : Mute Off 
* @retval MTR_NOT_OK
*/
EXTERN MT_RESULT_T MTTVE_VideoMute(MTTVE_ID_T eTveId, BOOL fgOnOff);

/**
 * @brief Video format.
 It sets the Video auxiliary outputs format(480i or 576i) 
 * @param eTveId
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_VideoFmt(MTTVE_ID_T eTveId, MTTVE_FMT_T eTveFmt);

/**
 * @brief 
 It sends the VBI Data to the Video auxiliary outputs from the input parameter buffer which contains the vbi Data. 
 It inserts the VBI Data in the monitoring output.
 * @param eTveId
 * @param eType Closed Caption or Extended Data Service or etc.
 * @param pData Data buffer pointer
 * @param length Data Length
 * @code
		typedef enum
		{
			MTTVE_VBI_DATA_CC = 0, ///< Closed Caption
			MTTVE_VBI_DATA_XDS, ///< Extended Data Service
			MTTVE_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
			MTTVE_VBI_DATA_WSS, ///< Analog SdUserData is WSS	
			MTTVE_VBI_DATA_MAX
		} MTTVE_VBIDATATYPE_T; ///< VBI Data Type
 * @endcode
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_SendVbiData(MTTVE_ID_T eTveId, MTTVE_VBIDATATYPE_T eType, UINT8 *pData, UINT32 length);

/**
 * @brief VBI Data
 It starts the VBI function of the Video auxiliary outputs. 
 * @param eTveId
 * @param eType Closed Caption or Extended Data Service or etc.
 * @code
 		typedef enum
		{
			SD_VBI_DATA_CC = 0, ///< Closed Caption
			SD_VBI_DATA_XDS, ///< Extended Data Service
			SD_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
			SD_VBI_DATA_WSS, ///< Analog SdUserData is WSS
		} MTTVE_VbiDataType_k; ///< VBI Data Type
 * @endcode
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_VbiStart(MTTVE_ID_T eTveId, MTTVE_VBIDATATYPE_T eType);

/**
 * @brief VBI Data
 It stops the VBI function of the Video auxiliary outputs. 
 * @param eTveId
 * @param eType Closed Caption or Extended Data Service or etc.
 * @code
 		typedef enum
		{
			SD_VBI_DATA_CC = 0, ///< Closed Caption
			SD_VBI_DATA_XDS, ///< Extended Data Service
			SD_VBI_DATA_TTX, ///< Analog SdUserData is Teletext
			SD_VBI_DATA_WSS, ///< Analog SdUserData is WSS
		} MTTVE_VbiDataType_k; ///< VBI Data Type
 * @endcode
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_VbiStop(MTTVE_ID_T eTveId, MTTVE_VBIDATATYPE_T eType);

#ifdef CC_MT5391
/**
 * @brief 
   set the horizontal sidebar/scaling of cvbs out
 * @param usOriginalSize original horizontal input size
   @param usEffectiveSize effective horizontal size
   @param usOutputSize horizontal size to output
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_SetDtvHScalingForCvbs(UINT16 usOriginalSize,
                                               UINT16 usEffectiveSize,
                                               UINT16 usOutputSize);

/**
 * @brief 
   set the horizontal cut size of cvbs out
 * @param usOriginalSize original horizontal input size
   @param usFrameSize CVBS horizontal output size
   @param usPictureCutSize horizontal size to center cut
   @param usPictureDestSize horizontal size to down scaling
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_SetDtvHCenterCutForCvbs(UINT16 usOriginalSize,
                                                 UINT16 usFrameSize,
                                                 UINT16 usPictureCutSize,
                                                 UINT16 usPictureDestSize);

EXTERN MT_RESULT_T MTTVE_VideoMode(MTTVE_ID_T eTveId, MTTVE_MODE_T eTveMode);


/**
 * @brief 
   set the vertical blackbar/scaling of cvbs out
 * @param usOriginalSize original vertical input size
   @param usFrameSize CVBS vertical output size
   @param usPictureCutSize vertical size to center cut
   @param usPictureDestSize vertical size to down scaling
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_SetDtvLetterboxForCvbs(UINT16 usOriginalSize,
                                                UINT16 usFrameSize,
                                                UINT16 usPictureCutSize,
                                                UINT16 usPictureDestSize);

/**
 * @brief 
   get the scaling information of cvbs out
   @param ptScalingInfo the scaling information
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
EXTERN MT_RESULT_T MTTVE_GetDtvScalingStatusForCvbs(scaling_info_t *ptScalingInfo);
#endif
/**
 * @brief 
 The Manual Debug function of MTTVE API.
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
//void MTTVE_Debug(void);

#ifdef __cplusplus
}
#endif

#endif
