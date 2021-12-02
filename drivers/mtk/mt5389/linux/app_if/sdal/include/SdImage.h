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
/**
 * @file SdImage.h
 * @brief Image Module header file\n
  Samsung DAL에서 JPEG, GIF, PNG 등의 Image File에 관한 API를 모아 놓음.\n
  Image 디코딩 & 인코딩 기능, 디코딩 된 데이터를 display 하기 위해 Video Frame Buffer에 쓰는 기능을 수행한다.\n
  Collection of APIs releated to Image Files: JPEG, GIF, PNG \n
  The functions include Image decoding and Encoding, Pumping the decoded data to the video frame buffer for display.\n
 * @author S/W Platform Lab
 * @date 2006/08/24
 */

#ifndef	_SDIMAGE_H_
#define	_SDIMAGE_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdUtil.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

typedef enum
{
	SD_IMAGE_AUTO,	///< [IN] For hardware decoder to detect format by reading file header.
	SD_IMAGE_JPEG,	///< [IN] JPEG Type
	SD_IMAGE_GIF,	///< [IN] GIF Type
	SD_IMAGE_PNG,	///< [IN] PNG Type
	SD_IMAGE_MAX	
} SdImage_Format_k;

typedef enum
{	
	SD_IMAGE_STATUS_BASIC 	= 0x0001, ///< Working State and Buffer Pointer info.
	SD_IMAGE_STATUS_DEC		= 0x0002, ///< Decoding Info
	SD_IMAGE_STATUS_ENC		= 0x0004, ///< Encoding Info
} SdImage_StatusMask_k;

typedef struct
{
	uint32_t decoderSubmitSize; 	///< [OUT] The size of buffer which SdImage can deal with. This is same as SdFeeder_AudioGetBufferSize(). If user submits smaller size than decStreamingBufSize, SdImage should pad rest of buffer to null data. This size is not applied in case of SD_IMAGE_RAW
	uint8_t* pUserBuffer; 			///< [OUT] frame buffer which can be used by app while SdImage is being opened.
	uint32_t userBufferSize; 		///< [OUT]
	SdBool_t bRenderRotate;
} SdImage_Attrib_t;

typedef struct {
	SdImage_Format_k eFormat;		///< [IN] Format of Image.
	SdColorFormat_k eColorFormat;	///< [IN] Desired output color format, optional.
	uint16_t depth;					///< [IN] The number of bits per pixel.
	SdSize_t sSize;					///< [IN] Desired output position and size.
	uint32_t rotateDegree;			///< [IN] The rotation degree. Range is 0 ~ 360. If 0˚then set 0, if 90˚then set 90, etc.	
	
	///< Below member data is only valid in SdImage_GetStatus() and when completed 
	///< If you use SW decoder, use pPicBuffer, else HW decoder, use pPicBufferY, pPicBufferCb, pPicBufferCr	
	uint8_t* pPicBuffer;			///< [OUT] The starting address of the decoded image buffer
	uint32_t picBufSize;			///< [OUT] 
	
	uint8_t* pPicBufferY;           ///< [OUT] The starting address of the decoded Y(G) image buffer
    uint8_t* pPicBufferCb;          ///< [OUT] The starting address of the decoded Cb(B) image buffer
    uint8_t* pPicBufferCr;          ///< [OUT] The starting address of the decoded Cr(R) image buffer
} SdImage_DecParam_t;

typedef struct {
	SdColorFormat_k eColorFormat;	///< [IN] Desired output color format, optional.
	uint16_t depth;					///< [IN] The number of bits per pixel.
	SdSize_t sSize;					///< [IN] Source picture size.
	SdGeometry_t sGeom;				///< [IN] Destination geometry on video plane.
	uint32_t rotateDegree;			///< [IN] The rotation degree. Range is 0 ~ 360. If 0˚then set 0, if 90˚then set 90, etc.	
	uint64_t defaultAlpha __attribute__ ((aligned(8)));	///< [IN] Applied alpha value (Shadow's raw image format includes the alpha pixel information). Valid [0..255], optional.
	
	///< If you use SW decoder, use pPicBuffer, else HW decoder, use pPicBufferYT, pPicBufferCbT, pPicBufferCrT
    ///< So first check the pPicBuffer, if pPicBuffer is NULL, you should use pPicBufferY, pPicBufferCb, pPicBufferCr
    uint8_t* pPicBuffer;            ///< [IN] The starting address of the decoded image buffer in video plane.
    uint32_t picBufSize; 

    uint8_t* pPicBufferY;           ///< [IN] The starting address of the decoded Y(G) image buffer
    uint8_t* pPicBufferCb;          ///< [IN] The starting address of the decoded Cb(B) image buffer
    uint8_t* pPicBufferCr;          ///< [IN] The starting address of the decoded Cr(R) image buffer	
} SdImage_SubmitParam_t;

typedef struct {
	uint8_t* pInBuffer; 			///< [IN] input data
	uint32_t imageWidth; 			///< [IN] image width, in pixels
	uint32_t imageHeight; 			///< [IN] image height, in pixels
	uint32_t lnOffset;				///< [IN] src Image Line Offset	
	SdColorFormat_k eColorFormat;	///< [IN] Color format
	SdBool_t bInterlaced;
	
	int32_t quality; 				///< [IN] compress quality
	uint32_t inputComponents; 		///< [IN] # of color components per pixel

	uint32_t YTopAddr;				///< [IN] base address of top field of Y component to be encoded
	uint32_t YBotAddr;				///< [IN] base address of bottom field of Y component to be encoded
	uint32_t CTopAddr;				///< [IN] base address of top field of C component to be encoded
	uint32_t CBotAddr;				///< [IN] base address of bottom field of C component to be encoded
} SdImage_EncInputParam_t;

typedef struct {
	SdColorFormat_k eColorFormat;	///< [OUT] The color format of the encoded image
	uint32_t encXSize;				///< [OUT] The width of the encoded image
	uint32_t encYSize;				///< [OUT] The height of the encoded image
	uint32_t encBufSize;			///< [OUT] The size of the encoded image buffer
	uint32_t* pEncBufStart;			///< [OUT] The starting address of the encoded image buffer
} SdImage_EncOutputParam_t;

typedef struct {
	SdBool_t bOnGoing; 				///< On Decoding or Encoding
	SdImage_DecParam_t sDecParam; 	///< Hardware might not be able to decode according to user desire because of hardware limitation, read final result again.	
	SdImage_EncOutputParam_t sEncParam;
} SdImage_Status_t;

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief It initialize Image Engine and gets the Buffer information.
 * @remarks 
 * @param eVideoId Video Component ID
 * @param pAttrib
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_Open(SdVideoID_k eVideoId, SdImage_Attrib_t *pAttrib);

/**
 * @brief It closes Image Engine.
 * @remarks
 * @param eVideoId Video Component ID
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_Close(SdVideoID_k eVideoId);

/**
 * @brief Submit to encoder buffer
 * @remarks
 * @param eVideoId Video Component ID
 * @param pSettings
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_EncoderSubmit(SdVideoID_k eVideoId, SdImage_EncInputParam_t* pSettings);

/**
 * @brief Initilize the decoder
 * @remarks
 * @param eVideoId Video Component ID
 * @param pSettings
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_DecoderInit(SdVideoID_k eVideoId, SdImage_DecParam_t* pSettings);

/**
 * @brief Submit to decoder buffer
 * @remarks
 * @param eVideoId Video Component ID
 * @param pBuffer
 * @param size
 * @param bWait
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_DecoderSubmit(SdVideoID_k eVideoId, uint8_t* pBuffer, uint32_t size, SdBool_t bWait);

/**
 * @brief Stop decoding
 * @remarks
 * @param eVideoId Video Component ID
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_DecoderStop(SdVideoID_k eVideoId);

/**
 * @brief Submit data to the Image Engine for Decoding or Encoding.
 * @remarks
 * @param eVideoId Video Component ID
 * @param pSettings
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_Submit(SdVideoID_k eVideoId, SdImage_SubmitParam_t* pSettings);

/**
 * @brief Submit data to video plane with specific color and geometry.
 * @remarks
 * @param eVideoId Video Component ID
 * @param sGeom position and size
 * @param sColor red, green, blue color
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_SubmitFillRect(SdVideoID_k eVideoId, SdGeometry_t sGeom, SdColor_t sColor);

/**
 * @brief Get the status of Image Engine.
 * @remarks
 * @param eVideoId Video Component ID
 * @param mask
 * @param pState
 * @param timeout
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_GetStatus(SdVideoID_k eVideoId, SdMask_t mask, SdImage_Status_t *pState, uint32_t timeout);

 /**
 * @brief 비디오 영역의 메모리 사용여부를 설정하는 함수
 Decide whether it uses video memory.
 * @remarks User Level 에서 비디오 영역의 메모리를 사용하고자 하는 경우 bSet 을 TRUE로 설정하고, 해당 영역에대한 정보는 pAttrib를 통해 얻어온다.
 Set bSet=TRUE if it uses video memory at user level. The property of the memory is returned by pAttrib.
 * @param eVideoId Video Component ID
 * @param bSet Video Memory 사용 여부. True if it uses video memory
 * @param pAttrib
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdImage_UseVideoMemory(SdVideoID_k eVideoId, SdBool_t bSet, SdImage_Attrib_t *pAttrib);

/**
 * @brief SdImage API의 Manual Debug 함수. Manual Debug function for SdImage API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdImage_Debug(void);

#endif

