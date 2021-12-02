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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mpv_if.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mpv_if.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef MPV_IF_H
#define MPV_IF_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "u_common.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
	
#define MPV_DRV_OK			       ((INT32) 0)
#define MPV_DRV_SET_FAILED         ((INT32) -262) 
#define MPV_DRV_INV_GET_INFO       ((INT32) -258)  
#define MPV_DRV_INV_SET_INFO       ((INT32) -259) /**<the set operation is invalid or cannot be processed*/

#define MPV_CC_ERR_BUFF_OVER_FLOW       ((UINT32) 1) /**<Buffer over flow used in VID_DEC_CC_COND_ERROR*/


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
/** the aspect ratio of video source.
*/
typedef enum
{
    MPV_ASPECT_RATIO_UNKNOWN = 0,
    MPV_ASPECT_RATIO_4_3,
    MPV_ASPECT_RATIO_16_9,
    MPV_ASPECT_RATIO_2_21_1
} MPV_ASPECT_RATIO_T;

/** the notify conditions.
  This structure match to the one in mw. Can't change it's sequence.
*/
typedef enum
{
    MPV_COND_ERROR = -1,
    MPV_COND_CTRL_DONE,
    MPV_COND_RESOLUTION_CHG,
    MPV_COND_I_FRAME_SET_BUFF_DONE,
    MPV_COND_I_FRAME_DONE,    
    MPV_COND_DECODE_STATUS_CHG
    #ifdef CC_MINI_DRIVER
    ,
    MPV_COND_NO_FBG  
    #endif
}   MPV_DEC_COND_T;

/** the notify condition status.
  This structure match to the one in mw. Can't change it's sequence.
*/
typedef enum
{
    MPV_DEC_DECODE_UNKNOWN = -1,
    MPV_DEC_DECODE_NO_DATA,
    MPV_DEC_DECODE_NORMAL,
    MPV_DEC_DECODE_ERROR,
    MPV_DEC_DECODE_HD_NOT_SUPPORT
}   MPV_DEC_DECODE_STATUS_T;


/**This notification function is called by the video decoder in response to some events.
*/
typedef void (*PFN_DEC_NFY_FCT) (
    void*               pvNfyTag,
    MPV_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2
);


/**This notification function is called by the video decoder to notify the reached PTS.
*/
typedef void (*PFN_PTS_NFY_FCT)
(
    void*   pvPtsNfyTag,
    PTS_T   rPts
);

/**This callback function is called by the video decoder to acquire data for decoding in Pull mode.
*/
typedef void (*PFN_ACQUIRE_FCT)(
    void*           pvTag,
    UCHAR**         ppucBuf,
    UINT32*         pu4BufSize
);

/**This callback function is called by the video decoder to release the acquired data buffer in Pull mode.
*/
typedef void (*PFN_RELEASE_FCT)(
    void*           pvTag,
    UCHAR*          pucBuf,
    UINT32          u4BufSize
);

/** This structure is used on get type VID_DEC_GET_TYPE_CC_DATA, which contains the CC data and related information.
*/
typedef struct
{
    UCHAR*     pucCcData;
    UINT32     u4Size;
    PTS_T      u8Pts;
    BOOL       fgIsScte;   
    BOOL       fgBottomFirst;
} MPV_CC_DATA;

/** CC notify conditions.
*/
typedef enum
{
    MPV_CC_COND_ERROR = -1,
    MPV_CC_COND_DATA_ARRIVAL
}   MPV_CC_COND_T;

/**This notification function is called by the video decoder to notify CC.
*/
typedef void (*PFN_CC_NFY_FCT) 
(
    void*           pvTag,
    MPV_CC_COND_T  	rCcCond,
    void*           pvArg
);

/**This structure contains a video decoders' closed caption condition notify function and tag.
*/
typedef struct
{
    void*           pvTag;
    PFN_CC_NFY_FCT  pfCcNfyFct;
}   MPV_CC_NFY_INFO_T;

/**This structure contains information for the middleware to set a PTS timer.
*/
typedef struct
{
    PTS_T             rPts;
    void*             pvTag;
    PFN_PTS_NFY_FCT   pfPtsNfyFct;
}MPV_PTS_NFY_INFO_T;

/**the input mode of video decoder.
*/
typedef enum
{
    MPV_INP_MODE_NORMAL,
    MPV_INP_MODE_PULL
}MPV_INP_MODE_T;

/**This structure contains information for the middleware to change the input mode of the Video Decoder.
*/
typedef struct
{
    MPV_INP_MODE_T      eInpMode;
    void*               pvTag;       
    PFN_ACQUIRE_FCT   	pfAcquireNfy; 
    PFN_RELEASE_FCT   	pfReleaseNfy; 
}MPV_INP_NFY_INFO_T;

/**This structure contains a video decoders notify function and tag.
*/
/*
typedef struct
{
    void*  pvTag;
    PFN_DEC_NFY_FCT  pfDecNfy;
}MPV_DEC_NFY_INFO_T;
*/

/**This structure contains I frame buffer addr and size.
*/
typedef struct 
{
    UCHAR*  pucIFrameBufAddr;
    UINT32  u4Size;
}MPV_DEC_I_FRAME_BUFF_T;


/**Video Stream Format.
*/
typedef enum
{
	MPV_FORMAT_NONE,
	MPV_FORMAT_MPEG1,
	MPV_FORMAT_MPEG2,
	MPV_FORMAT_MPEG4,
	MPV_FORMAT_H264,
	MPV_FORMAT_DIVX,
	MPV_FORMAT_WMV3, ///< WMV Ver.9
	MPV_FORMAT_XVID,
	MPV_FORMAT_MAX
} MPV_VIDEO_FORMAT_T; 

/**Mpeg Aspect Ratio.
*/
typedef enum
{
	MPV_ALL_ASPECT_RATIO_NONE,
	MPV_ALL_ASPECT_RATIO_1_1, ///< Aspect ration 1:1
	MPV_ALL_ASPECT_RATIO_4_3, ///< Aspect ration 4:3
	MPV_ALL_ASPECT_RATIO_16_9, ///< Aspect ration 16:9
	MPV_ALL_ASPECT_RATIO_221_1, ///< Aspect ration 2.21:1
	MPV_ALL_ASPECT_RATIO_MAX
} MPV_ALL_ASPECT_RATIO_T;

/**This structure contains video information.
*/
typedef struct
{
	UINT32 u4HResolution; ///< Source horizontal active size
	UINT32 u4VResolution; ///< Source vertical active size
	UINT32 u4HStart; ///< Source horizontal back porch 
	UINT32 u4VStart; ///< Source vertical back porch
	UINT32 u4HTotal; ///< Source horizontal raster size
	UINT32 u4VTotal; ///< Source vertical raster size
	INT32  i4ProgressScan; ///< Source scantype - TRUE:Progressive, FALSE:Interlace
	MPV_ALL_ASPECT_RATIO_T eRatio; ///< Aspect ratio
	UINT32 u4frameRate; ///< Frame rate (Hz * 1000)
}MPV_VIDEO_INFO_T;


typedef struct
{
	INT32 i4TSDLock; ///< TSD lock status 1:Lock, 0:Unlock
	INT32 i4AudioLock; ///< Audio decoding lock status 1:Lock, 0:Unlock
	INT32 i4VideoLock; ///< Video decoding lock status 1:Lock, 0:Unlock
	INT32 i4UnmuteEnable; ///< Unmute Enable status 1:Enable, 0:Disable
	MPV_VIDEO_INFO_T rVideoInfo; ///< Video information
	UINT32 u4HDMIAudioFormat; ///< HDMI Audio Format
} MPV_AVDEC_STATUS_T;


typedef enum
{
	AVDEC_STATUS_VIDEOLOCK	= 0x04, ///< Mask of video decoding lock status
	AVDEC_STATUS_VIDEOINFO	= 0x08 ///< Mask of video information status
}MPV_AVDEC_STATUS_MASK_T;


typedef void (*PFN_MPV_AVDEC_CB)
(
	MPV_AVDEC_STATUS_T* prStatus, 
	MPV_AVDEC_STATUS_MASK_T   eMask
);

/**This structure contains information of the user data and picture for user data callback function.
*/
typedef struct
{
    UINT32 u4Rptr;
    UINT32 u4Pts;
    UINT16 u2TemporalRef;    
    UCHAR ucDataType;
    UCHAR ucPicType;
    UCHAR ucPicStruct;
    UCHAR ucRepFirstFld;
    UCHAR ucTopFldFirst; 
    UCHAR ucDataLen;        
    UCHAR ucEsId;
    BOOL fgProgressiveFrm;    
}MPV_USERDATA_CB_T;


typedef enum
{
	MPV_SLICE_EXTRA_INFO        = 0x01,
	MPV_PICTURE_EXTRA_INFO      = 0x02,
	MPV_PICTURE_USER_DATA       = 0x04,
	MPV_GOP_USER_DATA           = 0x08,
	MPV_SEQUENCE_USER_DATA      = 0x10,
	MPV_AFD_DATA		           = 0x20
} MPV_USERDATA_TYPE_T;


typedef void (*PFN_MPV_USERDATA_CB)
(
	MPV_USERDATA_CB_T* prUsrData
);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/** Initialize MPV module.
*/
EXTERN void MPV_Init(void);
/** Terminate MPV module.
*/
EXTERN void MPV_Termint(void);
/** Reset MPV module.
*/
EXTERN void MPV_Reset(void);
/** Change to PLAY mode.
*@param ucEsId					ES id.
 */
EXTERN void MPV_Play(UCHAR ucEsId);
EXTERN void MPV_Play_I_Frame(UCHAR ucEsId);
/** Change to STOP mode.
*@param ucEsId 					ES id.
 */
EXTERN void MPV_Stop(UCHAR ucEsId);
/** Change to PAUSE mode.
*@param ucEsId					ES id.
 */
EXTERN void MPV_Pause(UCHAR ucEsId);

/** Enable closed caption.
*@param ucEsId					ES id.
*/
EXTERN void MPV_PlayCc(UCHAR ucEsId);
/** Disable closed caption.
*@param ucEsId 					ES id.
*/
EXTERN void MPV_StopCc(UCHAR ucEsId);
/** Allocate buffer for closed caption .
*@param ucEsId 					ES id.
*@param u4CcBufSize 			the buffer size of closed caption.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_AllocCcBuf(UCHAR ucEsId, UINT32 u4CcBufSize);
/** Free closed caption buffer.
*@param ucEsId 					ES id.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_FreeCcBuf(UCHAR ucEsId);
/** Flush closed caption buffer.
*@param ucEsId 					ES id.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_FlushCcBuf(UCHAR ucEsId);
/** Get closed caption data.
*@param ucEsId 					ES id.
*@param prCcData 				the CC data and related information
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_GET_INFO  	Fail.
*/
EXTERN INT32 MPV_GetCc(UCHAR ucEsId, MPV_CC_DATA* prCcData);
/** Set closed caption notify function.
*@param ucEsId 					ES id.
*@param prCcNfyInfo 			Closed caption condition notify function and tag.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_SetCcNfyFct(UCHAR ucEsId, const MPV_CC_NFY_INFO_T* prCcNfyInfo);
/** Set PTS notify function.
*@param ucEsId 					ES id.
*@param prPtsNfyInfo 			Information for the middleware to set a PTS timer.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_SetPtsNfyFct(UCHAR ucEsId, const MPV_PTS_NFY_INFO_T* prPtsNfyInfo);
/** Set input mode notify function.
*@param ucEsId 					ES id.
*@param prInpNfyInfo 			Information for the middleware to change the input mode of the Video Decoder.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_SetInpNfyFct(UCHAR ucEsId, const MPV_INP_NFY_INFO_T* prInpNfyInfo);
/** Set decode notify function.
*@param ucEsId 					ES id.
*@param prDecNfyInfo 			Video decoders notify function and tag.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
//EXTERN INT32 MPV_SetDecNfy(UCHAR ucEsId, const MPV_DEC_NFY_INFO_T* prDecNfyInfo);
/** Allocate I frame buffer.
*@param ucEsId	 				ES id.
*@param prIFrameBuff			I frame buffer addr and size.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_AllocIFrameBuf(UCHAR ucEsId, MPV_DEC_I_FRAME_BUFF_T* prIFrameBuff);
/** Free I frame buffer.
*@param ucEsId 					ES id.
*@retval MPV_DRV_OK  			Success.
*/
EXTERN INT32 MPV_FreeIFrameBuf(UCHAR ucEsId);
/** Parse I frame resolution info.
*@param ucEsId		 			ES id.
*@param prIFrameBuff			I frame buffer addr and size.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_SetIFrameBuf(UCHAR ucEsId, const MPV_DEC_I_FRAME_BUFF_T* prIFrameBuf);
/** Decode I frame.
*@param ucEsId	 				ES id.
*@param prIFrameBuff			I frame buffer addr and size.
*@retval MPV_DRV_OK  			Success.
*@retval MPV_DRV_INV_SET_INFO  	Fail.
*/
EXTERN INT32 MPV_SetIFrameDec(UCHAR ucEsId, const MPV_DEC_I_FRAME_BUFF_T* prIFrameBuff);

/** Get Video Fromat.
*@param ucEsId	 			ES id.
*@retval Video Format.
*/
EXTERN MPV_VIDEO_FORMAT_T MPV_GetVideoFormat(UCHAR ucEsId);

/** Check Video lock or not.
*@param ucEsId	 			ES id.
*@retval TRUE               Video is lock. MPV has received the first I picture.
*@retval FALSE              Video is Not lock. MPV has not received the first I picture.
*/
EXTERN BOOL MPV_IsLock(UCHAR ucEsId);

/** Get Video Information.
*@param ucEsId	 			ES id.
*@param peVideoInfo	 		Address of video info structure.
*/
EXTERN void MPV_GetVideoInfo(UCHAR ucEsId, UINT32* pu4HResolution, UINT32* pu4VResolution,
                            INT32* pi4PrgScan, MPV_ALL_ASPECT_RATIO_T* peRatio, UINT32* pu4FrmRat);

/** Register AVDec callback function.
*@param ucEsId	 			ES id.
*@param pfSdalCb	 		Sdal AVDec callback function.
*/
EXTERN BOOL  MPV_RegAVDecCb(UCHAR ucEsId, PFN_MPV_AVDEC_CB pfAVDecCb);

/** AVDec callback function.
*@param ucEsId	 			ES id.
*@param prStatus	 		AVDec status.
*@param eMask	 		AVDec callback mask.
*/
EXTERN void MPV_AVDecCb(UCHAR ucEsId, MPV_AVDEC_STATUS_T* prStatus, MPV_AVDEC_STATUS_MASK_T eMask);

/** Register digital user data callback function.
*@param ucEsId	 		ES id.
*@param pfUserDataCb	 	Digital user data callback function.
*/
EXTERN BOOL  MPV_RegUserDataCb(PFN_MPV_USERDATA_CB pfUserDataCb);

/** Enable digital user data.
*@param ucEsId				ES id.
 */
EXTERN void MPV_StartUserData(UCHAR ucEsId);

/** Disable digital user data.
*@param ucEsId				ES id.
 */
EXTERN void MPV_StopUserData(UCHAR ucEsId);

/** Enable digital user data in PES header.
*@param ucEsId				ES id.
 */
EXTERN void MPV_StartPesUserData(UCHAR ucEsId);

/** Disable digital user data in Pes header.
*@param ucEsId				ES id.
 */
EXTERN void MPV_StopPesUserData(UCHAR ucEsId);

EXTERN void MPV_EnableAfd(UCHAR ucEsId);

EXTERN void MPV_DisableAfd(UCHAR ucEsId);

EXTERN void MPV_SetDbk(BOOL fgEnable, BOOL fgEngrMode);

EXTERN BOOL MPV_SupportDbk(void);

/** Get mpeg2 deblocking function.
*@param ucEsId	 			ES id.
*@param fgDbkEnable	 		Deblocking enable/disable.
*@param u4XStart            Deblocking region horizontal start.
*@param u4YStart	 		Deblocking region vertical start.
*@param u4Width             Deblocking region width.
*@param u4Height	 		Deblocking region height.
*/                            
EXTERN void MPV_GetDbkInfo(UCHAR ucEsId, BOOL* pfgDbkEnable, UINT16* pu4XStart, 
                            UINT16* pu4YStart, UINT16* pu4Width, UINT16* pu4Height);

/** Set mpeg2 deblocking function.
*@param ucEsId	 			ES id.
*@param fgDbkEnable	 		Enable/disable deblocking.
*@param u4XStart            Deblocking region horizontal start.
*@param u4YStart	 		Deblocking region vertical start.
*@param u4Width             Deblocking region width.
*@param u4Height	 		Deblocking region height.
*/                            
EXTERN void MPV_SetDbkInfo(UCHAR ucEsId, BOOL fgDbkEnable, 
                             UINT16 u4XStart, UINT16 u4YStart, UINT16 u4Width, UINT16 u4Height);
                           
/** Set mpeg2 deblocking function.
*@param ucEsId	 			ES id.
*@param u2Level             Set the level in 0~100.
                            0: weak, 50: default, 100: strong
*/                            
EXTERN BOOL MPV_SetDbkLevel(UCHAR ucEsId, UINT16 u2Level);
   
/** Get mpeg2 deblocking function.
*@param ucEsId	 			ES id.
*@param pu2Level            Return the current setting in 0~100.
                            0: weak, 50: default, 100: strong
*/
EXTERN VOID MPV_GetDbkLevel(UCHAR ucEsId, UINT16* pu2Level);

                       
/** Set mpeg2 deblocking value function.
*@param ucEsId	 			ES id.
*@param u4Value             Set the level in 0~3.
                            0: disable, 1: weak, 2: middle, 3: strong
*/                            
EXTERN BOOL MPV_SetDbkValue(UCHAR ucEsId, UINT32 u4Value);
   
/** Get mpeg2 deblocking value function.
*@param ucEsId	 			ES id.
*@param pu4Value            Return the current setting in 0~3.
                            0: disable, 1: weak, 2: middle, 3: strong
*/
EXTERN VOID MPV_GetDbkValue(UCHAR ucEsId, UINT32* pu4Value);

/** Get digital user data and copy to pucUserDataTmpBuf.
*@param pucUserDataTmpBuf   Buffer for the user data.
*@param u4Rptr	 		    The Rptr of the user data.
*@param u4DataLen	 		The size of the user data.
*@retval TRUE               Success.
*@retval FALSE              Fail.
*/                            
EXTERN BOOL MPV_GetUserData(UCHAR* pucUserDataTmpBuf, UINT32 u4Rptr, UINT32 u4DataLen);

/** Get Video ES FIFO address and size.
*@param pu4FifoAddr         Video ES FIFO start address.
*@param pu4FifoSize	 		Video ES FIFO size.
*/
EXTERN void MPV_GetVidFifo(UINT32* pu4FifoAddr, UINT32* pu4FifoSize);


#endif //MPV_IF_H
