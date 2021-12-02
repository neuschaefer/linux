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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: img_flw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file img_lib_if.h
 *  Image decode, scale and display interface.
 */
 
#ifndef	_IMAGE_FLW_H_
#define	_IMAGE_FLW_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "x_os.h"
#include "x_typedef.h"
#include "img_lib_if.h"
#include "jpg_if.h"
/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
//#define JPG_TOTAL_BUFFER_SIZE 0x780000  	///< total 7.5M for image library, 5382 64M
//#define JPG_TOTAL_BUFFER_SIZE 0x300000  	///< total 3M for image library, 5381 32M
#define JPG_TOTAL_BUFFER_SIZE 0x200000  	///< total 2M for dsp working space, 8223 nucleus

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

/**
 *  Image handler.
 */
typedef void* IMGHANDLE;
/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
/**
 * Initialize image module.
 *
 * @param eMemClass in/out buffer type.
 * @return IMGHANDLE image handler
 */
IMGHANDLE _IMG_Init(ENUM_IMG_BUFFER_TYPE eMemClass, void *pvTag);

INT32 _IMG_CheckDecParam(IMGHANDLE hCurImg, IMG_PARAM_T rInfo);

INT32 _IMG_DecPrepare(IMGHANDLE hCurImg, IMG_PARAM_T rInfo);

INT32 _IMG_DecOneSegment(IMGHANDLE hCurImg, UINT32 u4ImgID, ENUM_IMG_ROTATE_PHASE eRotateDegree);

INT32 _IMG_BsBufManager(IMGHANDLE hCurImg, UINT32 u4ImgID);

void _IMG_NotifyStatus(IMGHANDLE hCurImg, UINT32 u4ImgID, INT32 i4Data, UINT32 u4Status);

/**
 * Scale operation.
 *
 * @param _IMGFillScBufCB fill scaling src buffer callback
 * @param  prScOptInfo operation parameter
 * @return void
 */
void _IMG_Scale(PFN_JPG_FILL_SC_BUF pfnFillScBufCB, IMG_SCALE_INFO_T* prScOptInfo);

/**
 * Apply operations on decoded raw image.
 *
 * @param rCmd command for image operation(zoom and rotate).
 * @return ENUM_IMG_RET
 */
void _IMG_Set(IMG_BUF_PARAM_T rCmd);

/**
 * Query decoding information.
 *
 * @param rStatus status and information of decoding process.
 * @return void.
 */
void _IMG_Get(IMG_CAP_T* prCap);


/**
 * Set File Size
 *
 * @File Size.
 * @return void
 */
void _IMG_SetImgSize(IMGHANDLE hCurImg, UINT32 u4ImageSize);

/**
 * Query decoding progress.
 *
 * @param decoding process.
 * @return void
 */
void _IMG_GetProgess(IMGHANDLE hCurImg, UINT32* pu4Status);

/**
 * Stop decoding procedure.
 *
 * @param hCurImg image handler
 * @param void.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET _IMG_Stop(IMGHANDLE hCurImg);

/**
 * Release handler.
 *
 * @param hCurImg image handler
 * @param hCurImg.
 * @return void
 */
void _IMG_Release(IMGHANDLE* phCurImg);

/**
 * Connect video plane.
 *
 * @param u4VideoId video plane ID.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET _IMG_VDP_Connect(UINT32 u4VideoId);

/**
 * Query video plane info.
 *
 * @param video plane information.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET _IMG_GetVideoPlaneInfo(IMG_VP_INFO_T *prVpInfo);

/**
 * Flip double buffer for video plane, swap pu1CurFrameAddr and pu1BackFrameAddr.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET _IMG_Flip(void);

/**
 * Register input image buffer filling callback function.
 *
 * @param pfnReg callback function.
 * @return void
 */	
void _IMG_CBreg(PFN_JPG_EVENT_NOTIFY pfnReg);

/**
 * Set status while buffer filling operation finish.
 *
 * @param u4Entry1Size buffer entry1 filled size.
 * @param u4Entry2Size buffer entry2 filled size.
 * @param u4IsEOF flag if end of image file.
 * @return void
 */	
void _IMG_SetFillBsBufEnd(IMGHANDLE hCurImg, UINT32 u4Entry1Size, UINT32 u4Entry2Size, BOOL bIsEOF);

/**
 * Get internal buffer info.
 *
 * @param prImgBufInfo buffer description.
 * @return void.
 */
void _IMG_GetBufInfo(IMG_BUF_INFO_T* prImgBufInfo);

UINT32 _IMG_GetBSBufInfo(IMGHANDLE hCurImg, IMG_LIB_FILL_RING_BUF_T *prStreamBufInfo);

INT32 _IMG_GetOutputBuffer(void *hCurImg, void **ppvmem);

BOOL _IMG_IsFullImgDecoding(void);

VOID _IMG_RotateJpg(ENUM_IMG_ROTATE_PHASE eRotatePhase, IMG_LIB_DISP_INFO_T* prDispInfo, IMG_LIB_MEM_LAYOUT_T* prImgLibMem);

#endif

