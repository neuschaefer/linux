/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
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

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
#ifdef CC_53XX_JPG_ARGB4444
#define JPG_TOTAL_BUFFER_SIZE 0xb80000  	///< total 7.5M for image library, 5382 64M
#else
#define JPG_TOTAL_BUFFER_SIZE 0x780000  	///< total 7.5M for image library, 5382 64M
#endif
#define JPG_WORKING_BUFFER_SIZE 0x780000  	//JPG_TOTAL_BUFFER_SIZE = JPG_WORKING_BUFFER_SIZE + 0x400000(ARGB temp frame buffer)
//#define JPG_TOTAL_BUFFER_SIZE 0x780000  	///< total 7.5M for image library, 5382 64M
//#define JPG_TOTAL_BUFFER_SIZE 0x300000  	///< total 3M for image library, 5381 32M

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

/**
 *  Image handler.
 */

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

INT32 _IMG_CheckDecParam(IMGHANDLE hCurImg, IMG_PARAM_T *prInfo);

INT32 _IMG_DecPrepare(IMGHANDLE hCurImg, IMG_PARAM_T *prInfo);

INT32 _IMG_DecOneSegment(IMGHANDLE hCurImg, UINT32 u4ImgID,IMG_PARAM_T *prInfo);

INT32 _IMG_BsBufManager(IMGHANDLE hCurImg, UINT32 u4ImgID);

INT32 _IMG_BsGetData(IMGHANDLE hCurImg, UINT32 u4ImgID, UINT32 u4DstAddr, UINT32 u4Offset, UINT32 u4Size);

INT32 _IMG_BsStartTrsData(IMGHANDLE hCurImg, UINT32 u4ImgID,UINT32 u4TrsOfst);

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
    void _IMG_Set(IMG_BUF_PARAM_T rCmd, IMG_REGION_T *rDstRegion);

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
VOID _IMG_GetDecInfo(IMGHANDLE hCurImg,UINT32 u4GetType,UINT32 u4Param1,UINT32 u4Param2);
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

#endif

