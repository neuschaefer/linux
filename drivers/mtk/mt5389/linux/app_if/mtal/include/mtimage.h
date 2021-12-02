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
 * $RCSfile: MTIMAGE.h,v $
 * $Revision: #3 $
 *---------------------------------------------------------------------------*/ 
/** @file MTIMAGE.h
 *  This header file declares exported APIs and data structures of Media Player Module.
 */

#ifndef MT_IMAGE_H
#define MT_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
  INCLUDE FILES
********************************************************************/

#include "mttype.h"
#include "mtvdo.h"

/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/
#define MTIMAGE_IMGDEVICE             "/dev/jpg"
#define MTIMAGE_IMGBUFDEVICE       "/dev/feeder"    //fixme
#define MTIMAGE_SOF0                  0xc0
#define MTIMAGE_SOF2                  0xc2
#define MTIMAGE_APP1                  0xe1
#define MTIMAGE_APP2                  0xe2
#define MTIMAGE_EOI                   0xd9
#define MTIMAGE_SOI                   0xd8
#define MTIMAGE_SOS                   0xda
#define MTIMAGE_EXIF_MAKE_TAG         0x010f
#define MTIMAGE_EXIF_MODEL_TAG        0x0110
#define MTIMAGE_EXIF_ORIENTATION_TAG  0x0112
#define MTIMAGE_EXIF_DATE_TAG         0x0132
#define MTIMAGE_EXIF_INTERCHANGE      0x0201
#define MTIMAGE_EXIF_INTERCHANGELEN   0x0202

#define MTIMAGE_PARSING_BUFFER_SIZE   (512 * 1024)
#define MTIMAGE_WEBP_HEADER_PARSING_SIZE (30)

#define MPF_MAX_IMAGE 1
/********************************************************************
  MACRO FUNCTION DEFINITIONS
********************************************************************/
#define MTIMAGE_LOG(ENABLE, MOD, fmt...) \
    do \
    {\
        if((ENABLE)) \
        { \
            printf("["MOD"]:%s\n" , __FUNCTION__); \
            printf(fmt); \
        } \
									} while (0)

#define MTIMAGE_DEBUG(fmt...)       MTIMAGE_LOG(FALSE, "MTIMAGE", fmt)

/********************************************************************
  TYPE DEFINITION
********************************************************************/
/**MTIMAGE handle.
It is a pointer to VOID, which is used as the handle type for MTIMAGE.  
After an MTIMAGE instance has been opened, a handle is returned.  This 
handle will be used by most MTIMAGE APIs to identify the MTIMAGE instance.
*/
typedef VOID* MTIMAGE_HANDLE;

/**Error Type.
It is used for describing the error type when a callback from kernel is an 
error callback which needs to pass an error code to user space.  Possible 
errors include file open error, video decoding error, audio decoding error, 
DRM (for DivX) error, image decoding error and JPEG color format error.
*/
typedef enum
{
    MT_IMAGE_FILE_ERROR,       // Fail in opening File 
    MT_IMAGE_IMG_ERROR,        // Fail in decoding image
    MT_IMAGE_JPEG_COLOR_FORMAT_ERROR, //sharp code sync
    MT_IMAGE_ERROR_MAX,
} MTIMAGE_ERROR_TYPE_T;


/**Call back type.
When some event occurs during the playback of a multimedia file, a 
corresponding callback could be called to pass information to the upper 
layer.  This callback type is used for identifying the type of callback.  
Useful callback types are ERROR, FINISH and ELAPSEDTIME.  Other types are 
reserved for internal usage or non multimedia related.
*/
typedef enum
{
    MT_IMAGE_CALLBACK_NEEDDATA,        // notification of copy buffer
    MT_IMAGE_CALLBACK_ERROR,           // notification of play error
    MT_IMAGE_CALLBACK_FINISH,          // notification of file end
    MT_IMAGE_CALLBACK_STOPDONE,
    MT_IMAGE_CALLBACK_ALREADYSTOPED,
    MT_IMAGE_CALLBACK_MAX
} MTIMAGE_CALLBACK_TYPE_T;

/**Image format.
*/
typedef enum
{
    MT_IMAGE_JPEG_NEW,				///< image format
    MT_IMAGE_PNG_NEW,
    MT_IMAGE_WEBP_NEW,
    MT_IMAGE_MPO_NEW, 
    MT_IMAGE_MAX_NEW
} MTIMAGE_IMG_FORMAT;

/**Output Color format.
*/
typedef enum
{
    MT_IMAGE_COLOR_FORMAT_AYCBCR8888,         ///< AYCbCr display mode, 32 bit per pixel, for OSD
    MT_IMAGE_COLOR_FORMAT_Y_CBCR422,          ///< Y/CbCr separate 422 display mode, 16 bit per pixel, for video plane
    MT_IMAGE_COLOR_FORMAT_ARGB8888,
    MT_IMAGE_COLOR_FORMAT_ARGB1555,
    MT_IMAGE_COLOR_FORMAT_ARGB565,    
    MT_IMAGE_COLOR_FORMAT_ARGB4444,        
    MT_IMAGE_COLOR_FORMAT_RGBA8888,            
    MT_IMAGE_COLOR_FORMAT_RGB888,        
    MT_IMAGE_COLOR_FORMAT_RGB565,
    MT_IMAGE_COLOR_FORMAT_MAX,
} MTIMAGE_IMGCOLORFORMAT_T;

/**Output pixel format.
*/
typedef enum
{
    MT_IMAGE_PIX_FORMAT_UYVY,         
    MT_IMAGE_PIX_FORMAT_VYUY,         
    MT_IMAGE_PIX_FORMAT_1555,         
    MT_IMAGE_PIX_FORMAT_565,          
    MT_IMAGE_PIX_FORMAT_4444,         
    MT_IMAGE_PIX_FORMAT_8888,         
    MT_IMAGE_PIX_FORMAT_MAX,
} MTIMAGE_PIXFORMAT_T;

/**Rotation Option.
*/
typedef enum
{
    MT_IMAGE_ROTATE_0,                     ///<no rotation
    MT_IMAGE_ROTATE_90,                    ///<clockwise 90 degrees
    MT_IMAGE_ROTATE_180,                   ///<clockwise 180 degrees
    MT_IMAGE_ROTATE_270,                   ///<clockwise 270 degrees
    MT_IMAGE_ROTATE_0_FLIP,                ///<no rotation with flip
    MT_IMAGE_ROTATE_90_FLIP,               ///<clockwise 90 degrees with flip
    MT_IMAGE_ROTATE_180_FLIP,              ///<clockwise 180 degrees with flip
    MT_IMAGE_ROTATE_270_FLIP,              ///<clockwise 270 degrees with flip
    MT_IMAGE_ROTATE_MAX
} MTIMAGE_ROTATE_T;


typedef enum
{
//#if (ENABLE_PHOTO_TRANS_EFFECT_BLOCKS)
    MT_IMAGE_EFFECT_HORZ_BLOCKS_TOPLEFT, //0
    MT_IMAGE_EFFECT_HORZ_BLOCKS_TOPRIGHT,
    MT_IMAGE_EFFECT_HORZ_BLOCKS_BOTTOMLEFT,
    MT_IMAGE_EFFECT_HORZ_BLOCKS_BOTTOMRIGHT,
    MT_IMAGE_EFFECT_VERT_BLOCKS_TOPLEFT,
    MT_IMAGE_EFFECT_VERT_BLOCKS_TOPRIGHT,//05
    MT_IMAGE_EFFECT_VERT_BLOCKS_BOTTOMLEFT,
    MT_IMAGE_EFFECT_VERT_BLOCKS_BOTTOMRIGHT,
    MT_IMAGE_EFFECT_HORZ_ZIGZAG_BLOCKS_TOPLEFT,
    MT_IMAGE_EFFECT_HORZ_ZIGZAG_BLOCKS_TOPRIGHT,
    MT_IMAGE_EFFECT_HORZ_ZIGZAG_BLOCKS_BOTTOMLEFT,//10
    MT_IMAGE_EFFECT_HORZ_ZIGZAG_BLOCKS_BOTTOMRIGHT,
    MT_IMAGE_EFFECT_VERT_ZIGZAG_BLOCKS_TOPLEFT,
    MT_IMAGE_EFFECT_VERT_ZIGZAG_BLOCKS_TOPRIGHT,
    MT_IMAGE_EFFECT_VERT_ZIGZAG_BLOCKS_BOTTOMLEFT,
    MT_IMAGE_EFFECT_VERT_ZIGZAG_BLOCKS_BOTTOMRIGHT,//15
//#endif
//#if (ENABLE_PHOTO_TRANS_EFFECT_DIAGONAL_BLOCKS)
    MT_IMAGE_EFFECT_DIAGONAL_BLOCKS_TOPLEFT,
    MT_IMAGE_EFFECT_DIAGONAL_BLOCKS_TOPRIGHT,
    MT_IMAGE_EFFECT_DIAGONAL_BLOCKS_BOTTOMLEFT, //2
    MT_IMAGE_EFFECT_DIAGONAL_BLOCKS_BOTTOMRIGHT,
//#endif
//#if (ENABLE_PHOTO_TRANS_EFFECT_WIPE)
    MT_IMAGE_EFFECT_BARWIPE_LEFT2RIGHT, //20
    MT_IMAGE_EFFECT_BARWIPE_RIGHT2LEFT,
    MT_IMAGE_EFFECT_BARWIPE_TOP2BOTTOM,//2
    MT_IMAGE_EFFECT_BARWIPE_BOTTOM2TOP,//2
    MT_IMAGE_EFFECT_BOXWIPE_TOPLEFT,
    MT_IMAGE_EFFECT_BOXWIPE_TOPRIGHT,//25
    MT_IMAGE_EFFECT_BOXWIPE_BOTTOMLEFT,//2
    MT_IMAGE_EFFECT_BOXWIPE_BOTTOMRIGHT,//2
    MT_IMAGE_EFFECT_BARNDOORWIPE_HORIZONTAL_OPEN,//3 
    MT_IMAGE_EFFECT_BARNDOORWIPE_HORIZONTAL_CLOSE,//3 
    MT_IMAGE_EFFECT_BARNDOORWIPE_VERTICAL_OPEN,//30
    MT_IMAGE_EFFECT_BARNDOORWIPE_VERTICAL_CLOSE,//3
    MT_IMAGE_EFFECT_BOXWIPE_TOPCENTER,
    MT_IMAGE_EFFECT_BOXWIPE_RIGHTCENTER,
    MT_IMAGE_EFFECT_BOXWIPE_BOTTOMCENTER,//3
    MT_IMAGE_EFFECT_BOXWIPE_LEFTCENTER,//35
    MT_IMAGE_EFFECT_IRISWIPE_CENTER,
    MT_IMAGE_EFFECT_IRISWIPE_OUTER,
    MT_IMAGE_EFFECT_4BARWIPE_HORIZONTAL_FROM_TOP,//4
    MT_IMAGE_EFFECT_4BARWIPE_HORIZONTAL_FROM_BOTTOM,//4
    MT_IMAGE_EFFECT_4BARWIPE_VERTICAL_FROM_LEFT,//40
    MT_IMAGE_EFFECT_4BARWIPE_VERTICAL_FROM_RIGHT,//4
//#endif
//#if (ENABLE_PHOTO_TRANS_EFFECT_SLIDE)
    MT_IMAGE_EFFECT_BARSLIDE_LEFT2RIGHT,
    MT_IMAGE_EFFECT_BARSLIDE_RIGHT2LEFT,
    MT_IMAGE_EFFECT_BARSLIDE_TOP2BOTTOM,//4
    MT_IMAGE_EFFECT_BARSLIDE_BOTTOM2TOP,//45
    MT_IMAGE_EFFECT_BOXSLIDE_TOPLEFT,
    MT_IMAGE_EFFECT_BOXSLIDE_TOPRIGHT,
    MT_IMAGE_EFFECT_BOXSLIDE_BOTTOMLEFT,
    MT_IMAGE_EFFECT_BOXSLIDE_BOTTOMRIGHT,
    MT_IMAGE_EFFECT_BARNDOORSLIDE_HORIZONTAL_OPEN,//50
    MT_IMAGE_EFFECT_BARNDOORSLIDE_HORIZONTAL_CLOSE,
    MT_IMAGE_EFFECT_BARNDOORSLIDE_VERTICAL_OPEN,
    MT_IMAGE_EFFECT_BARNDOORSLIDE_VERTICAL_CLOSE,
    MT_IMAGE_EFFECT_4BARSLIDE_HORIZONTAL_FROM_TOP,
    MT_IMAGE_EFFECT_4BARSLIDE_HORIZONTAL_FROM_BOTTOM,//55
    MT_IMAGE_EFFECT_4BARSLIDE_VERTICAL_FROM_LEFT,
    MT_IMAGE_EFFECT_4BARSLIDE_VERTICAL_FROM_RIGHT,
//#endif
//#if (ENABLE_PHOTO_TRANS_EFFECT_FADE_IN)
    MT_IMAGE_EFFECT_FADE_IN, 
//#endif
    MT_IMAGE_EFFECT_CLOSE,//59
    MT_IMAGE_EFFECT_MAX//60
} MTIMAGE_SLIDETYPE;


typedef enum
{
    MT_IMAGE_CAP_IMAGE_FORMAT      = 0x01, //jpeg, png, bmp..   
    MT_IMAGE_CAP_MAX_RESOLUTION   = 0x02,
    MT_IMAGE_CAP_MIN_RESOLUTION   = 0x04,
    MT_IMAGE_CAP_COLOR_FORMAT      = 0x08,
    MT_IMAGE_CAP_MAX,
} MTIMAGE_CAP_T;


typedef struct
{
    UINT32 u4_trsn_id;         /* transaction id */
    INT32 i4_ret;                   /* MW buffer-filling return codes */
    UINT32 u4_filled_len;     /* filled length */
} MTIMAGE_BUF_FILLED_T;


/**Parameters of image display.
*/
typedef struct 
{
    MTVDO_REGION_T rDstRegion;                  // Destination region
    MTIMAGE_ROTATE_T eRotation;            // Rotation phase for display
    UINT32 u1PlaneId;                           // the id of the OSD/Video plane for display (image only)
    BOOL fgClean;
    BOOL fgDisplay;
    BOOL fgIsSlideShow;
} MTIMAGE_PARAM_T;


/**Parameters of image meta data.
*/
typedef struct
{
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4BPP;
    UINT32 u4ThumbnailLength;
    UINT32 u4Orientation;
    UINT32 u4APP1Length;
    UINT32 u4APP2Length;
    UINT8 au1Maker[20];
    UINT8 au1Model[30];
    UINT8 au1Date[30];
    UINT8 *pu1Thumbnail;
    UINT8 *pu1APP0; 
    UINT8 *pu1APP1; 
    UINT8 *pu1APP2; 
    BOOL fgProgressive;
    BOOL fgIsMPF;
    BOOL fgSOF;
    UINT32 u4NumOfImages;
    UINT8 *apu1MPEntry[MPF_MAX_IMAGE];
} MTIMAGE_IMGMETA_T;

typedef struct
{
    MTIMAGE_IMGCOLORFORMAT_T eColorFormat;  // Desired output color format (image only)
} MTIMAGE_SETTING_T;


typedef struct
{
    UINT32 u4CurImageAddrOffset;
} MTIMAGE_STATUS_T;


/**MTIMAGE callback function prototype.
It is the callback prototype used by MTIMAGE's callback mechanism.  The 
first argument is callback type.  The second one is a pointer to a data 
structure of callback parameters.  The last one is an additional parameter 
which is assigned when registering a specific callback function.
*/

typedef VOID (*MTIMAGE_CALLBACK)(MTIMAGE_CALLBACK_TYPE_T eCallbackType, 
                                     UINT32 u4Param);


#ifdef MTIMAGE_ANDROID_SUPPORT
typedef UINT32 (*MTIMAGE_READDATA)(UINT32 u4Addr,UINT32 u4Len);
typedef VOID (*MTIMAGE_SETCINFORES)(UINT32 u4Width,UINT32 u4Height);
#endif
/********************************************************************
  EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**Initialize IMAGE.  This API opens an MTIMAGE instance.
*@param phHandle        - Pointer to the handle for holding the returning 
                          instance handle.
*@param pucFileName     - File name of the multimedia file to be played back 
                          by this MTIMAGE instance.
                        - Set NULL for push mode.
                        - File instance when MTIMAGE_SRCTYPE is 
                          MTIMAGE_SRCTYPE_TIME_SHIFT
*@param eInputInstance  - Input instance.  For multimedia Audio+Video files, 
                          use MTIMAGE_PLAYMGR.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_Open(MTIMAGE_HANDLE* phHandle, CHAR* pucFileName);

/**This API closes an MTIMAGE instance.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_Close(MTIMAGE_HANDLE hHandle);

/**This API sets playback parameters.
*@param hHandle    - Instance handle.
*@param prSettings - Pointer to the data structure that holds the playback 
                     parameters to be set.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_Set(MTIMAGE_HANDLE hHandle, 
                                     MTIMAGE_SETTING_T* prSettings);

/**This API retrieves the current playback settings.
*@param hHandle       - Instance handle.
*@param prSettings    - Pointer to the data structure for holding the 
                        returning playback settings.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_Get(MTIMAGE_HANDLE hHandle, 
                                     MTIMAGE_SETTING_T* prSettings);

/**This API starts the playback.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_Start(MTIMAGE_HANDLE hHandle);

/**This API stops the playback.
*@param hHandle     - Instance handle.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_Stop(MTIMAGE_HANDLE hHandle);

/**Get the buffer address and size of the input local buffer
*@param hHandle             - Instance handle.
*@param ppi1Buffer          - Avaiable buffer pointer.
*@param pu4Size             - Size of avaiable buffer.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T  MTIMAGE_GetBuffer(MTIMAGE_HANDLE hHandle,
                            INT8** ppi1Buffer, UINT32* pu4Size);

/**This API retrieves the current playback status.
*@param hHandle   - Instance handle.
*@param prStatus  - Pointer to the data structure for holding the returning 
                    playback status.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_GetInfo(MTIMAGE_HANDLE hHandle,
                                     MTIMAGE_STATUS_T *prStatus);

/**This API registers a callback function.
IMAGE can uses the registered callback function to notify the upper layer. 
*@param hHandle       - Instance handle.
*@param eCallbackType - Callback type.
*@param fnCallback    - Callback function body.
*@param u4Param       - Additional parameter to be passed when calling the 
                        callback.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_RegCb(MTIMAGE_HANDLE hHandle, 
                                     MTIMAGE_CALLBACK fnCallback, 
                                     UINT32 u4Param);



#ifdef MTIMAGE_ANDROID_SUPPORT
EXTERN MT_RESULT_T MTIMAGE_RegReadFuc(MTIMAGE_HANDLE hHandle,
                               MTIMAGE_READDATA fnReadData,MTIMAGE_SETCINFORES fnSetCinfoRes);
#endif

/**Set image parameters for display.
*@param hHandle      - Instance handle.
*@param prImgSetting - Image setting for display.
*@param fgPartial    - Partial flag.
*@param prRegion     - Pointer to region.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_Display(MTIMAGE_HANDLE hHandle,
                                     MTIMAGE_PARAM_T *prImgSetting, 
                                     BOOL fgPartial, MTVDO_REGION_T *prRegion);

/**Push bitstream to kernel.
*@param hHandle                   - Instance handle.
*@param pu1Buf                    - Buffer address of bitstream data.
*@param u4Size                    - Size of the buffer.
*@param fgEof                     - Whether last data of the file/bitstream.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_PushData(MTIMAGE_HANDLE hHandle, UINT8 *pu1Buf, 
                                                UINT32 u4Size, BOOL fgEof);


/**Get result image from kernel.
*@param hHandle                   - Instance handle.
*@param pu4Buf                    - Buffer address of jpeg output (virtual).
*@param pu4PhyBuf                 - Buffer address of jpeg output (physical).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_GetImg(MTIMAGE_HANDLE hHandle,
                                     UINT32 *pu4Buf, UINT32 *pu4PhyBuf, UINT32* pu4CbCrOffset, MTVDO_REGION_T *prRegion);

/**Get image meta data.
*@param hHandle                   - Instance handle.
*@param pMeta                     - Meta data.
*@param pu1InBuf                  - Input buf for push mode (ex: thumbnail).
*@param u4InSize                  - Input size for push mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_GetImgMetaData(MTIMAGE_HANDLE hHandle, 
                                        MTIMAGE_IMGMETA_T *pMeta, UINT8 *pu1InBuf, UINT32 u4InSize);

/**Get meida type.
*@param hHandle            - Instance handle.
*@param peInstance         - Pointer for holding the returning instance type.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTIMAGE_SetDataMode(MTIMAGE_HANDLE  hHandle, BOOL fgPush);

EXTERN MT_RESULT_T MTIMAGE_SlideShow(MTIMAGE_HANDLE  hHandle, MTIMAGE_SLIDETYPE eType);

EXTERN MT_RESULT_T MTIMAGE_CleanFrameBuffer(MTIMAGE_HANDLE  hHandle);

//EXTERN MT_RESULT_T MTIMAGE_SetDecResolution(MTIMAGE_HANDLE  hHandle, UINT32 u4W, UINT32 u4H);

EXTERN MT_RESULT_T MTIMAGE_SetDecResolution(MTIMAGE_HANDLE  hHandle, UINT32 u4W, UINT32 u4H, UINT32 u4Pitch);
EXTERN MT_RESULT_T MTIMAGE_SetDecRatio(MTIMAGE_HANDLE  hHandle, UINT16 u2DecRatio);

EXTERN MT_RESULT_T MTIMAGE_WaitDecFinish(void);
EXTERN MT_RESULT_T MTIMAGE_Filldata_Libjpeg(void);

EXTERN MT_RESULT_T MTIMAGE_Connect(BOOL fgConnect, UINT32 eColorFMT);
EXTERN MT_RESULT_T MTIMAGE_DataFromBuffer(MTIMAGE_HANDLE* phHandle, UINT8 *pu1Buf, UINT32 u4Size, UINT32 format);
EXTERN MT_RESULT_T MTIMAGE_SetDecAddr(MTIMAGE_HANDLE  hHandle, UINT32 u4DecAddr);
#ifdef MTIMAGE_ANDROID_SUPPORT
EXTERN MT_RESULT_T MTIMAGE_SetStopFlag(BOOL fgStop);
#endif
#ifdef __cplusplus
}
#endif

#endif // MT_IMAGE_H

