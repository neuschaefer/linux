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
 * $RCSfile: mtvdec.h,v $
 * $Revision: #3 $
 *---------------------------------------------------------------------------*/ 

/** @file mtvdec.h
 *  This header file declares exported APIs of Video Decoder (VDEC) module,
 *  which inclued MPEG 1/2/4, H.264(AVC), and VC-1
 */

#ifndef MT_VDEC_H
#define MT_VDEC_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
#include "mtdmx.h"

   
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define ES_ID_0     0
#define ES_ID_1     1
#define ES_ID_MAX   2
#define FIRST_DECODING_FBG_OFFSET 24*1024*1024
#define DECODER_OUTPUT_OFFSET 3*1024*1024
#define INPUT_BUFFER_SIZE 2*1024*1024
#define OUTPUT_BUFFER_SIZE 46*1024*1024

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/**The aspect ratio of video source.
*/
typedef enum
{
    MTVDEC_ASPECT_RATIO_NONE,
    MTVDEC_ASPECT_RATIO_1_1,   ///< Aspect ration 1:1
    MTVDEC_ASPECT_RATIO_4_3,   ///< Aspect ration 4:3
    MTVDEC_ASPECT_RATIO_16_9,  ///< Aspect ration 16:9
    MTVDEC_ASPECT_RATIO_221_1, ///< Aspect ration 2.21:1
    MTVDEC_ASPECT_RATIO_MAX
}MTVDEC_ASPECT_RATIO_T;


typedef enum
{
    MTVDEC_FRAME_RATE_24_ =1,     
    MTVDEC_FRAME_RATE_24,      
    MTVDEC_FRAME_RATE_25,      
    MTVDEC_FRAME_RATE_30_,     
    MTVDEC_FRAME_RATE_30,      
    MTVDEC_FRAME_RATE_50,      
    MTVDEC_FRAME_RATE_60_,     
    MTVDEC_FRAME_RATE_60,      
    MTVDEC_FRAME_RATE_UNKNOWN 
}MTVDEC_FRAME_RATE_T;

/* Thumbnail Color mode */
typedef enum
{
    MTVDEC_COLORMODE_AYUV_CLUT2     = 0,
    MTVDEC_COLORMODE_AYUV_CLUT4     = 1,
    MTVDEC_COLORMODE_AYUV_CLUT8     = 2,
    MTVDEC_COLORMODE_UYVY_16        = 3,
    MTVDEC_COLORMODE_YUYV_16        = 4,
    MTVDEC_COLORMODE_AYUV_D8888     = 5,
    MTVDEC_COLORMODE_ARGB_CLUT2     = 6,
    MTVDEC_COLORMODE_ARGB_CLUT4     = 7,
    MTVDEC_COLORMODE_ARGB_CLUT8     = 8,
    MTVDEC_COLORMODE_RGB_D565       = 9,
    MTVDEC_COLORMODE_ARGB_D1555     = 10,
    MTVDEC_COLORMODE_ARGB_D4444     = 11,
    MTVDEC_COLORMODE_ARGB_D8888     = 12,
    MTVDEC_COLORMODE_YUV_420_BLK    = 13,
    MTVDEC_COLORMODE_YUV_420_RS     = 14,
    MTVDEC_COLORMODE_YUV_422_BLK    = 15,
    MTVDEC_COLORMODE_YUV_422_RS     = 16,
    MTVDEC_COLORMODE_YUV_444_BLK    = 17,
    MTVDEC_COLORMODE_YUV_444_RS     = 18
}MTVDEC_COLORMODE_T;


/**Decoding speed of video decoder
 */
typedef enum
{
    MTVDEC_DEC_SPEED_PAUSE  =    0,
    MTVDEC_DEC_SPEED_SLOW   =  500,
    MTVDEC_DEC_SPEED_NORMAL = 1000,
}MTVDEC_DEC_SPEED_T;

/**Decoding mode of video decoder
 */
typedef enum
{
    MTVDEC_DEC_ALL,
    MTVDEC_DEC_IP,
    MTVDEC_DEC_I
}MTVDEC_DEC_MODE_T;

/**Picture header information for user data callback.
*/
typedef struct 
{
    UINT32 u4Pts;                       ///< Presentation Time Stamps (90KHz) -> CC & Video synchonization
    UINT16 u2TemporalRef;               ///< Temporal Reference (MPEG picture header) -> debug only
    UCHAR ucPicType;                    ///< I/P/B type -> debug only
    UCHAR ucPicStruct;                  ///< frame/field encode picture -> debug only
    UCHAR ucRepFirstFld;                ///< repeat first field -> debug only
    UCHAR ucTopFldFirst;                ///< top field first -> debug only
    UCHAR ucEsId;                       ///< decoder ID = ES_ID_0
    BOOL fgProgressiveFrame;            ///< progressice frame  
}MTVDEC_USERDATA_CB_T;

/**Video information in bitstream header.
*/
typedef struct 
{
    BOOL fgProgressiveSeq;              ///<progressice_sequence in Sequence extension
    BOOL fgConstrainedParameterFlag;    ///<constrained_parameters_flag in Sequence Header
    BOOL fgLowDelay;                    ///<low_delay in Sequence extension
    BOOL fgMPEG2;                       ///<True if sequence extension present
    BOOL fgSeqDispExternValid;          ///<True if display extension present
    BOOL fgColorDescription;            ///<colour_description in Sequence display extension
    UCHAR ucAfd;                        ///<AFD in Picture user data
    UCHAR ucVideoFmt;                   ///<video_format in Sequence display extension   
    UCHAR ucProfileLevelIndication;     ///<profile_and_level_indication in Sequence extension
    UCHAR ucColorPrimaries;             ///<colour_primaries in Sequence display extension
    UCHAR ucTransferCharacteristics;    ///<transfer_characteristics in Sequence display extension
    UCHAR ucMatrixCoefficients;         ///<matrix_coefficients in Sequence display extension
    UINT16 u2Height;                    ///<vertical_size_value in Sequence Header 
    UINT16 u2Width;                     ///<horizontal_size_value in Sequence Header 
    INT16 i2DHS;                        ///<display_horizontal_size in Sequence display extension 
    INT16 i2DVS;                        ///<display_vertical_size in Sequence display extension 
    UINT16 u2FrmRate;                   ///<frame_rate_code in Sequence header
    UINT32 u4BitRate;                   ///<bit_rate_value in Sequence header
    UINT32 u4VbvBufSize;                ///<vbv_buffer_size_extension in Sequence extension
    UINT32 u4ChromaFormat;              ///<chroma_format in Sequence extension
    
    MTVDEC_ASPECT_RATIO_T eAspectRatio;
} MTVDEC_HDR_INFO_T;

/**Frame/field deblocking information in bitstream.
*/
typedef struct 
{
    BOOL fgDeblocking;                   ///is deblocking
    UINT32 u4Dbk1PixelCnt;               ///Strong deblocking method Pixel count
    UINT32 u4Dbk2PixelCnt;               ///Middle deblocking method Pixel count
    UINT32 u4Dbk3PixelCnt;               ///Weak deblocking method Pixel count
} MTVDEC_DBK_INFO_T;

/**Video decoder status.
* fgLock = TRUE, when the 1st picture decode ready.
* scramble channel (CQAM)  =>  fgLock = FALSE
* non scramble =>  fgLock = TRUE
* u4DecOkNs <= u4ReceiveNs
* When signal is weak, u4DecOkNs < u4ReceiveNs.
* picture type => ucPicType
* top field first => fgTopFldFirst
* repeat first field => fgRepFirstFld
* progressive frame => fgProgressiveFrm
* picture struct => ucPicStruture
* temporal reference => ucTemporalRef
*/
typedef struct 
{
    BOOL fgLock;                        ///<video decoder is lock.
    BOOL fgIsDisplaying;           ///<video start displaying
    BOOL fgTopFldFirst;                 ///<top field first
    BOOL fgRepFirstFld;                 ///<repeat first field
    BOOL fgProgressiveFrm;              ///<progressive frame
    UCHAR ucPicType;                    ///<picture type decoded ok by video decoder.
    UCHAR ucPicStruture;                ///<picture struct
    UINT32 ucTemporalRef;               ///<temporal reference
    UINT32 u4ReceiveNs;                 ///<picture count received by video decoder.
    UINT32 u4DecOkNs;                   ///<picture count decoded ok by video decoder.
    UINT32 u4CurPTS;                     ///<current displaying frame PTS
    UINT32 u4ESBufDataSize;             ///<current es buffer data size in bytes
    UINT32 u4DisplayQNum;               ///<current decoded frame number
    UINT32 u4DecodingPTS;               ///<current decoded frame pts
}MTVDEC_DEC_STATUS_T;



/**Digital user data callback function.
*/
typedef VOID (*MTVDEC_PFN_USERDATA_CB)
(
    MTVDEC_USERDATA_CB_T* prUserDataInfo, UINT8* pu1UserData, UINT16 u2DataLen
);


/**Decode error notify function.
*@param ucEsId                     ES id.
*@param u4DecErrNs                 Decode error picture count.       
*/
typedef VOID (*MTVDEC_PFN_DECERR_CB)
(
    UCHAR ucEsId, UINT32 u4DecErrNs
);


/**Decode picture notify notify function.
*@param ucEsId                     ES id.
*@param ucPicType                  Decode picture type.       
*/
typedef VOID (*MTVDEC_PFN_PICTYPE_CB)
(
    UCHAR ucEsId, UCHAR ucPicType
);


/**Decode picture status notify function.
*@param ucEsId                     ES id.
*@param u4Status                  Decode status.       
*/
typedef VOID (*MTVDEC_PFN_STATUS_CB)
(
    UCHAR ucEsId, UINT32 u4Status
);

/* Thumbnail Info */
typedef struct
{
    UCHAR*                    pucCanvasBuffer;
    MTVDEC_COLORMODE_T        eCanvasColormode;
    UINT32                    u4CanvasWidth;     /* in pixels */
    UINT32                    u4CanvasHeight;    /* in pixels */
    UINT32                    u4CanvasPitch;     /* in bytes */

    UINT32                    u4ThumbnailX;      /* in pixels */
    UINT32                    u4ThumbnailY;      /* in pixels */
    UINT32                    u4ThumbnailWidth;  /* in pixels */
    UINT32                    u4ThumbnailHeight; /* in pixels */
}MTVDEC_THUMBNAIL_INFO_T;

/* Thumbnail cap Info */
typedef struct
{
    UINT32 u4AddrY;
    UINT32 u4AddrC;
    UINT32 u4MemHSize;
    UINT32 u4MemVSize;
    UINT32 u4HSize;
    UINT32 u4VSize;
    UINT32 u4RRMode;
} MTVDEC_CAPTURE_INTO_T;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**Initialize VDEC module.
*@param VOID
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_Init(VOID);

/**Terminate VDEC module.
*@param VOID
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_Termint(VOID);

/** Reset VDEC module.
*@param VOID
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_Reset(VOID);

/**Change VDEC to PLAY mode.
*@param ucEsId                     ES id.
*@param u4Fmt                      Video codec format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_Play(UCHAR ucEsId, MTVDEC_FMT_T eVdecFmt);

/**Change VDEC to PLAY_I_FRAME mode.
*@param ucEsId                     ES id.
*@param u4Fmt                      Video codec format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_PlayIFrame(UCHAR ucEsId, MTVDEC_FMT_T eVdecFmt, UINT8* pucData, UINT32 u4Size);

/**Change VDEC to DECODE_I_FRAME mode.
*@param ucEsId                     ES id.
*@param u4Fmt                      Video codec format.
*@param pucData                    input data address
*@param u4Size                     input data size(in byte)
*@param pucDestData                outoput data address
*@param pu4DestSize                outoput data size
*@param pu4Width                   outoput picture width
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_DecodeIFrame(UCHAR ucEsId, MTVDEC_FMT_T eVdecFmt, UINT8* pucData, UINT32 u4Size,
                                       UINT8** ppucDestData, UINT32 *pu4DestSize,UINT32 *pu4Width);

/**Change VDEC to PAUSE mode.
*@param ucEsId                     ES id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_Pause(UCHAR ucEsId);

/**Change VDEC to STOP mode.
*@param ucEsId                     ES id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_Stop(UCHAR ucEsId);

/**Change VDEC to STOP I Frame mode.
*@param ucEsId                     ES id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_StopIFrame(UCHAR ucEsId);

/** Get src state. clear or scramble
*@param ucEsId                     ES id.
*@param pbScramble                 scramble state
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_GetSrcState(UCHAR ucEsId, BOOL *pbScramble);

/**Get video resolution and information.
*@param ucEsId                     ES id.
*@param prVdecInfo                 Bitstream header information.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_GetInfo(UCHAR ucEsId, MTVDEC_HDR_INFO_T* prVdecInfo);

/**Get deblocking information.
*@param ucEsId                     ES id.
*@param prDbkInfo                  Frame/field deblocking information.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_GetDbkInfo(UCHAR ucEsId, MTVDEC_DBK_INFO_T* prDbkInfo);
    
/**Set AV sync mode and STC id.
*@param ucEsId                     ES id.
*@param ucMode                     AV sync mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_SetAvSynMode(UCHAR ucEsId, MTAV_SYNC_MODE_T eMode);

/**Get STC value.
*@param ucEsId                     ES id.
*@param pu8Stc                     Stc value.(90K)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_GetStc(UCHAR ucEsId, UINT64 *pu8Stc);

/** Check VDEC decode status.
*@param ucEsId                     ES id.
*@param prDecStatus                Video decoder status.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_GetDecStatus(UCHAR ucEsId, MTVDEC_DEC_STATUS_T *prDecStatus);


/**Register VDEC decode error callback function.
*@param pfDecErrCb                 Decode error callback function.
*@param u4Duration                 Duration of continuous decode error (ms).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_RegDecErrCb(MTVDEC_PFN_DECERR_CB pfDecErrCb, UINT32 u4Duration);

/**Register VDEC picture type callback function.
*@param pfPicTypeCb                 Decode picture type callback function.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_RegPicTypeCb(MTVDEC_PFN_PICTYPE_CB pfPicTypeCb);

/**Register VDEC status callback function.
*@param pfStatusCb                 Decode status callback function.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_RegStatusCb(MTVDEC_PFN_STATUS_CB pfStatusCb);

/**Register digital user data callback function.
*@param pfUserDataCb               Digital user data callback function.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_RegUserDataCb(MTVDEC_PFN_USERDATA_CB pfUserDataCb);

/**Enable digital user data.
*@param ucEsId                     ES id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_StartUserData(UCHAR ucEsId);

/**Disable digital user data.
*@param ucEsId                     ES id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_StopUserData(UCHAR ucEsId);

/**Disable digital user data.
*@param ucEsId                     ES id.
*@param bIsOnePas                  Is one pass?
*@param prDstInfo                  destination information.
*@param prSrcInfo                  source information.
*@param ucEsId                     ES id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
EXTERN MT_RESULT_T MTVDEC_GetVideoThumbnail(UCHAR ucEsId, BOOL bIsOnePass, 
    MTVDEC_THUMBNAIL_INFO_T* prDstInfo, MTVDEC_CAPTURE_INTO_T* prSrcInfo);

/**Set speed
*@param ucEsId                     ES id.
*@param eSpeed                     speed
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T MTVDEC_SetSpeed(UCHAR ucEsId, MTVDEC_DEC_SPEED_T eSpeed);

/**Advance a frame (without wait complete)
*@param ucEsId                     ES id.
*@param fgSetFrameAdvance          Is advance a frame or not?
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T MTVDEC_SetFrameAdvance(UCHAR ucEsId, BOOL fgSetFrameAdvance);

/**Flush ES data
*@param ucEsId                     ES id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T MTVDEC_SetFlushEsData(UCHAR ucEsId);

/**Flush ES data
*@param ucEsId                     ES id.
*@param eMode                      Decode mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T MTVDEC_SetDecodeMode(UCHAR ucEsId, MTVDEC_DEC_MODE_T eMode);
#ifdef CC_ITIMING_ALWAYS
EXTERN MT_RESULT_T MTVDEC_SetFixFHDDisplay(UCHAR ucEsId, BOOL fgIsInterlace);
#endif
#ifdef __cplusplus
}
#endif

#endif //MT_VDEC_H
