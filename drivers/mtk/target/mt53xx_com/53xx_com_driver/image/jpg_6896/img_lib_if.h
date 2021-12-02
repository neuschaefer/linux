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
 * $RCSfile: img_lib_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file img_lib_if.h
 *  Image decode, scale and display interface.
 */

#ifndef	_IMAGE_LIB_H_
#define	_IMAGE_LIB_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "x_os.h"
#include "x_typedef.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
//notify parameter
typedef void* IMGHANDLE;

#define JPG_HW_NFY_FILL_BUF			(0)         /* driver needs more data */
#define JPG_HW_NFY_FINISHED			(1)         /* decoding process finished */
#define JPG_HW_NFY_ERROR				(2)         /* decoding error */
#define JPG_HW_NFY_STOP_DONE			(3)         /* stop command notification */
#define JPG_HW_NFY_ALREADY_STOPPED	(4)     /* stop command notification */
#define JPG_HW_NFY_PROGRESS			(5)         /* driver notifies the progress (%) */
#define JPG_HW_NFY_NONE				(6)

/* Driver error codes */
#define IMG_DRV_HW_ERROR		(-1)
#define IMG_DRV_IO_ERROR		(-2)
#define IMG_DRV_NOT_SUPPORT	    (-3)
#define IMG_DRV_BUSY	        (-4)
#define IMG_DRV_ERR_COMPONENT	(-5)		//code sync

//decode state
#define IMG_INIT				0x1		//state, cmd
#define IMG_DEC_PREPARE	0x2		//state
#define IMG_DECODE			0x3		//state, cmd
#define IMG_STOP			0x4		//cmd
#define IMG_SET				0x5		//state, cmd
#define IMG_IDLE				0x6		//state
#define IMG_RELEASE			0x7		//state, cmd
#define IMG_BSBUF_UNDERFLOW 0x8	//state

#define BS_PKT_SIZE 0x18000 //96kbyte
/* ID for limitation qeury */
#define IMG_LIMIT_PROGRESSIVE   ((UINT32) 0)
#define IMG_LIMIT_RESOLUTION    ((UINT32) 1)
#define IMG_LIMIT_PGSV_RESOLUTION   ((UINT32) 2)
#define IMG_LIMIT_FULLPIC_MODE  ((UINT32) 3)
#define IMG_LIMIT_PNG_RESOLUTION  ((UINT32) 103)

//#define IMG_INPUT_CHECKSUM

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/
/**
 *  Fill buffer callback function.
 */
typedef void (*PFN_JPG_EVENT_NOTIFY)(
    UINT32 u4ImgID,
    VOID *pv_tag,
    VOID *pv_data,
    UINT32 u4State
);

typedef void (*PFN_JPG_FILL_SC_BUF)(
    UINT32 u4DstBufAddr,
    UINT32 u4Len
);

/********************************************************************
	TYPE DEFINITION
********************************************************************/
/**
 *  Image functions return code.
 */
typedef enum
{
    E_IMG_OK,
    E_IMG_DEC_FINISH,
    E_IMG_INIT_FAIL,
    E_IMG_DEC_FAIL,
    E_IMG_FMT_UNSUPPORT,
    E_IMG_DISP_INFO_ERROR,
    E_IMG_DISP_VP_ERROR,
    E_IMG_CANCEL,
    E_IMG_BUF_FILL_FAIL,
    E_IMG_NULL_HANDLER,
    E_IMG_BSBUF_UNDERFLOW,
    E_IMG_HWBUSY,
    E_IMG_ERR_COMPONENT		//code sync
} ENUM_IMG_RET;

/**
 *  Image format.
 */
typedef enum
{
    IMAGE_JPEG,				///< image format
    IMAGE_PNG,
    IMAGE_WEBP,
    IMAGE_MPO,
    IMAGE_MAX
} ENUM_IMG_FORMAT;

/**
 *  Input/Output buffer type.
 */
typedef enum
{
    LINEAR_IN_LINEAR_OUT,		///< linear input buffer with whole image file, linear output buffer with whole decompressed data.
    LINEAR_IN_SINGLE_OUT,	 	///< linear input buffer with whole image file, single output buffer
    LINEAR_IN_DOUBLE_OUT,		///< linear input buffer with whole image file, double output buffer
    RING_IN_SINGLE_OUT,		///< ring input buffer with callback fill buffer mechanism, single output buffer
    RING_IN_DOUBLE_OUT,		///< ring input buffer with callback fill buffer mechanism, doubld output buffer
    EXTERNAL_RING_IN_DOUBLE_OUT, ///< ring buffer provided by upper layer for mjpg playback
    BUF_TYPE_MAX
} ENUM_IMG_BUFFER_TYPE;

/**
 *  Display color mode.
 */
typedef enum
{
    AYCBCR8888_DIRECT32,						///< AYCbCr display mode, 32 bit per pixel, for OSD
    ARGB8888_DIRECT32,
    Y_CBCR420_SEP16,							///< Y/CbCr seperate 420 display mode, 16 bit per pixel, for video plane
    Y_CBCR422_SEP16,							///< Y/CbCr separate 422 display mode, 16 bit per pixel, for video plane
    Y_CBCR444_YCSEP24,
    ARGB4444_DIRECT16,
    ARGB1555_DIRECT16,    
    RGB565_DIRECT16,
    COLOR_MODE_MAX
} ENUM_IMG_DISP_COLOR_MODE;

/**
 *  Rotate degree.
 */
typedef enum
{
#if 0
    CLOCKWISE_0,
    CLOCKWISE_90,
    CLOCKWISE_180,
    CLOCKWISE_270,
    CLOCKWISE_0_WITH_FLIP,					///<no rotation, with flip */
    CLOCKWISE_90_WITH_FLIP,					///<clockwise  90 degrees, with flip */
    CLOCKWISE_180_WITH_FLIP,					///<clockwise 180 degrees, with flip */
    CLOCKWISE_270_WITH_FLIP,					///<clockwise 270 degrees, with flip */
    PHASE_MAX
#endif
	CLOCKWISE_0			 		= 0,
	CLOCKWISE_0_WITH_FLIP	 	= 1,
	CLOCKWISE_180			 	= 2,
	CLOCKWISE_180_WITH_FLIP 	= 3,
	CLOCKWISE_270_WITH_FLIP 	= 4,
	CLOCKWISE_90			 	= 5,
	CLOCKWISE_90_WITH_FLIP  	= 6,
	CLOCKWISE_270			 	= 7,
	PHASE_MAX
}ENUM_IMG_ROTATE_PHASE;

typedef enum
{
  IMG_GETINFO_PROGRESS=0,
  IMG_GETINFO_INVALID
}ENUM_IMG_GETINFO_TYPE;

//--------------------------------------------------
/**
 *  Region description
 */
typedef struct
{
    UINT8* pu1Addr;								///< [IN] start address
    UINT32 u4CbCrOffset;							///< [IN] address offset for Y/CbCr seperate mode
    UINT32 u4Depth;								///< [IN] number of bits per pixel  for Y component.
    UINT32 u4Pitch;								///< [IN] pitch
    UINT32 u4TotalLine;							///< [IN] total height
    UINT32 u4StartX;								///< [IN] start point X
    UINT32 u4StartY;								///< [IN] start point Y
    UINT32 u4Width;								///< [IN] region width
    UINT32 u4Height;								///< [IN] region height
}IMG_REGION_T;


typedef struct
{
    UCHAR ucFbgId;
    UINT32 u4RingOffset;
}IMG_VDEC_T;

typedef struct
{
    UINT32 u4SrcStartX;								///< [IN] start point X
    UINT32 u4SrcStartY;								///< [IN] start point Y
    UINT32 u4SrcWidth;								///< [IN] region width
    UINT32 u4SrcHeight;								///< [IN] region height
}IMG_SRC_REGION_T;

/**
 *  Input compressed image and output destination information.
 */
typedef struct
{
    ENUM_IMG_FORMAT eFormat;					///< [IN] Format of Image.
    UINT8* pu1InBuffer;							///< [IN] compressed data start pointer.Null for using buffer notify mechanism
    UINT64 u8BufSize;							///< [IN] the size of the compressed data. 0 for using buffer notify mechanism
    IMG_REGION_T rDstRegion;						///< [IN] destination information, use internal buffer if pu1Addr == NULL, use default w,h if w,h=0
    BOOL bKeepRatio;							///< [IN] TRUE: keep original image ratio, FALSE: fit u4DstW, u4DstH
    ENUM_IMG_DISP_COLOR_MODE eDispColorMode;	///< [IN] destination color mode
    ENUM_IMG_ROTATE_PHASE eRotateDegree;		///< [IN] rotate degree
    IMG_REGION_T* prDecOut;						///< [IN/OUT] the decompressed image info. Valid: !=NULL, prepare by middleware
    IMG_VDEC_T rMjpg;                              ///used for getting working buffer from MPEG (MJPEG only)
    IMG_SRC_REGION_T rSrcRegion;
    UINT32 u4DecColorMode;
    BOOL fgThumbnailDec;                        ///< [IN] decode thumbnail or full image
} IMG_PARAM_T;


/**
 *  Decoding status.
 */
typedef struct
{
    UINT32 u4MaxImgW;							///< [OUT] max support source image width
    UINT32 u4MaxImgH;							///< [OUT] max support source image height
    UINT32 u4MaxScaleFactor;					///< [OUT] max scaling factor
    UINT32 u4MaxScaleBufSize;					///< [OUT] max scaled output buffer size
    UINT32 u4ScaleBufAddr;						///< [OUT] scaled buffer addr
    UINT32 u4WorkingAddr;						///< [OUT] working buffer addr
    UINT32 u4RsvAddr;							///<[OUT] available addr when sdImage module open
    UINT32 u4RsvSizeL;							///<[OUT] available mem size when sdImage module open (large)
    UINT32 u4RsvSizeS;							///<[OUT] available mem size when sdImage module open (small)
    UINT32 u4RsvTotalSz;
}IMG_CAP_T;

/**
 *  scaling operation, src, dst information
 */
typedef struct
{
    ENUM_IMG_DISP_COLOR_MODE eColorFormat;		///< [IN] Color format.
    UINT32 srcWidth;								///< [IN] The width of the source image.
    UINT32 srcHeight;							///< [IN] The height of the source image.
    UINT32 dstWidth;								///< [IN] The width of the source image.
    UINT32 dstHeight;							///< [IN] The height of the source image.
    UINT32 depth;								///< [IN] The number of bits per pixel.
    UINT8* pu1OutBuffer;							///< [IN] The starting address of the scaled image buffer.
} IMG_SCALE_INFO_T;

/**
 *  Set operation and src, dst information
 */
typedef struct
{
    ENUM_IMG_DISP_COLOR_MODE eDispColorMode;	///< [IN] color mode
    ENUM_IMG_ROTATE_PHASE eRotateDegree;		///< [IN] rotate degree
    IMG_REGION_T rSrcRegion;						///< [IN] source information
    IMG_REGION_T rDstRegion;						///< [IN] destination information
    BOOL bKeepRatio;							///< [IN] TRUE: keep original image ratio, FALSE: fit u4DstW, u4DstH
    BOOL bKeepOriResolution;                   ///< [IN] TRUE: keep original image resolution, FALSE: enlarge to u4DstW, u4DstH
    IMG_REGION_T* prDstRegion;					///< [OUT] destination result
}IMG_BUF_PARAM_T;

/**
 *  Video plane information.
 */
typedef struct
{
    UINT32 u4Width;								///< [OUT] video plane width
    UINT32 u4Height;								///< [OUT] video plane height
    UINT32 u4PitchAlign;							///< [OUT] pitch alignment
    UINT8* pu1CurFrameAddr[2];					///< [OUT] current display frame buffer address, [1] for Y, [2] for CbCr
    UINT8* pu1BackFrameAddr[2];					///< [OUT] background display frame buffer address, [1] for Y, [2] for CbCr
    ENUM_IMG_DISP_COLOR_MODE eDispColorMode;	///< [OUT] color mode
}IMG_VP_INFO_T;

/**
 *  Buffer filling mechanism. Information of each entry.
 */
typedef struct
{
    BOOL bValid;									///< valid flag for buffer feed, 1: need feeding, 0: no need feeding
    VOID* pvStartAddr;							///< address for buffer write pointer
    UINT32 u4RequiredLen;						///< required bytes from start_addr
    UINT32 u4FillLen;								///< actually filled length
} IMG_LIB_ENTRY_BUF_T;

/**
 *  Buffer filling mechanism. 2 buffer entries to simulate ring buffer by linear buffer.
 */
typedef struct
{
    IMG_LIB_ENTRY_BUF_T rEntry1;					///< entry 1 for linear buffer
    IMG_LIB_ENTRY_BUF_T rEntry2;					///< entry 2 for wrap back linear buffer
    BOOL bEOF;									/// < check if end of file
} IMG_LIB_FILL_RING_BUF_T;

/**
 *  Internal buffer layout.
 */
typedef struct
{
    UINT8* pu1ImageBuf;							///< buffer for raw image
    UINT8* pu1WorkingBuf;						///< working area
    UINT32 u4ImageBufSize;						///< image buffer size
    UINT32 u4WorkingBufSize;						///< working buffer size
}IMG_BUF_INFO_T;

/**
 * Debug information
 */
typedef struct
{
    UINT32 u4OrigImgW;
    UINT32 u4OrigImgH;
    UINT32 u4ScW;
    UINT32 u4ScH;
    UINT32 u4ScImgAddr[2];						///<Y/C
    UINT32 u4DecWorkBufStart[3];
    UINT32 u4DecWorkBufEnd[3];
    UINT32 u4ScWorkBufStart[3];
    UINT32 u4ScWorkBufEnd[3];
    IMG_BUF_INFO_T rBufInfo;
    IMG_CAP_T rCapInfo;
    IMG_VP_INFO_T rVPInfo;
}IMG_DEBUG_INFO_T;

//--------------------------------------------------
/**
 * data passed with IMG_NFY_FILL_BUF
 */
typedef struct
{
    UINT32 ui4_trsn_id;		// transaction id
    VOID *pv_start_add;		// starting address
    UINT32 ui4_required_len;	// required length
    BOOL b_reset_pos;		// position-resetting flag
    UINT32 ui4_position;		// the position to be resettd
} IMG_FILL_BUF_MW_T;

/* passed with IMG_GET_TYPE_LIMITATION */
typedef struct
{
    /* INPUT */
    UINT32    u4Type;

    /* OUTPUT */
    union
    {
        /* used for IMG_LIMIT_PROGRESSIVE */
        BOOL fgProgressiveSupport;

        /* used for IMG_LIMIT_RESOLUTION */
        struct
        {
            UINT32  u4MaxWidth;
            UINT32  u4MinWidth;
            UINT32  u4MaxHeight;
            UINT32  u4MinHeight;
            UINT32  u4MaxArea;
        } t_resolution;

        /* used for IMG_LIMIT_PGSV_RESOLUTION */
        struct
        {
            UINT32  u4MaxArea;
            UINT32  u4MinWidth;
            UINT32  u4MinHeight;
        } t_pgsv_resolution;

        /* used for IMG_LIMIT_FULL_MODE */
        BOOL        fgFullModeSpport;
    } u;
} IMG_LIMIT_T;



typedef void* (*PFN_IMG_INIT)(
    ENUM_IMG_BUFFER_TYPE eMemClass,
    void *pvTag
);

typedef INT32 (*PFN_IMG_DecPrepare)(
    void* hCurImg,
    IMG_PARAM_T *prInfo
);

typedef INT32 (*PFN_IMG_DecOneSegment)(
    void* hCurImg,
    UINT32 u4ImgID,
    IMG_PARAM_T *prInfo
);

typedef INT32 (*PFN_IMG_BsBufManager)(
    VOID* hCurImg,
    UINT32 u4ImgID
);

typedef void (*PFN_IMG_SetFillBsBufEnd)(
    void* hCurImg,
    UINT32 u4Entry1Size,
    UINT32 u4Entry2Size,
    BOOL bIsEOF
);

typedef INT32 (*PFN_IMG_MwFillBuffer)(
    void* hDec,
void **ppvBuf,
UINT32 *pu4Size, 
UINT32 u4AddSize
);

typedef void (*PFN_IMG_SetFileSize)(
    void* hDec,
    UINT32 u4ImageSize
);

typedef INT32 (*PFN_IMG_CheckDecParam)(
    void* hCurImg,
    IMG_PARAM_T *prInfo
);

typedef void (*PFN_IMG_FreeInstance)(
    void* hDec
);

typedef ENUM_IMG_RET (*PFN_IMG_Stop)(
    void* hCurImg
);

typedef void (*PFN_IMG_GetInfo)(
    void* hCurImg,
    UINT32 u4GetType,
    UINT32 u4Param1,
    UINT32 u4Param2
);

typedef struct
{
PFN_IMG_INIT pfnImgInit;
PFN_IMG_DecPrepare pfnImgDecPrepare;
PFN_IMG_DecOneSegment pfnImgDecOneSegment;
PFN_IMG_BsBufManager pfnImgBsBufManager;
PFN_IMG_SetFillBsBufEnd pfnImgSetFillBsBufEnd;
PFN_IMG_MwFillBuffer pfnImgMwFillBuffer;
PFN_IMG_SetFileSize pfnImgSetFileSize;
PFN_IMG_CheckDecParam pfnImgCheckDecParam;
PFN_IMG_FreeInstance pfnImgFreeInstance;
PFN_IMG_Stop pfnImgStop;
PFN_IMG_GetInfo pfnImgGetInfo;
ENUM_IMG_FORMAT eImgType;
}IMG_INFO_T;

IMG_INFO_T* _IMG_GetInfo(void);
HANDLE_T IMG_GetPngSema(void);

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
/**
 * Initialize image library with async mode.
 *
 * @param void.
 * @return ENUM_IMG_RET.
 */
ENUM_IMG_RET IMG_Lib_Init(void);

/**
 * Release image library with async mode.
 *
 * @param void.
 * @return void.
 */
void IMG_Lib_End(void);

/**
 * Initialize image module.
 *
 * @param eMemClass in/out buffer type.
 * @param pvTag private data.
 * @param pfnReg callback function pointer.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Init(ENUM_IMG_BUFFER_TYPE eMemClass, void *pvTag,  UINT32 u4ImgID,
                      PFN_JPG_EVENT_NOTIFY pfnReg);

/**
 * Decode image and scale output raw data.
 *
 * @param rInfo input compressed image and output destination information.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Decode(IMG_PARAM_T rInfo);

/**
 * Scale operation.
 *
 * @param _IMGFillScBufCB fill scaling src buffer callback
 * @param  prScOptInfo operation parameter
 * @return void
 */
void IMG_Scale(PFN_JPG_FILL_SC_BUF _IMGFillScBufCB, IMG_SCALE_INFO_T* prScOptInfo);

/**
 * Apply operations on decoded raw image.
 *
 * @param rCmd command for image operation(zoom and rotate).
 * @return void
 */
 void IMG_Set(IMG_BUF_PARAM_T rCmd, IMG_REGION_T *rDstRegion);

/**
 * Query module capability.
 *
 * @param rCap max supported image size and buffer size.
 * @return void.
 */
void IMG_Get(IMG_CAP_T* prCap);

/**
 * Stop decoding procedure.
 *
 * @param void.
 * @return void
 */

/**
 * Set Driver if use mw_if or mtal.
 *
 * @param Use or not.
 * @if set ok, return 0;
 */
ENUM_IMG_RET IMG_SetUseMw(BOOL fgUseMw);

void IMG_Stop(void);

/**
 * Release handler.
 *
 * @param void.
 * @return void.
 */
void IMG_Release(void);

/**
 * Connect video path.
 *
 * @param video plane id.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_VDP_Connect(UINT32 u4VideoId);

/**
 * Query video plane info.
 *
 * @param video plane information.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_GetVideoPlaneInfo(IMG_VP_INFO_T *prVpInfo);

/**
 * Flip double buffer for video plane, swap pu1CurFrameAddr and pu1BackFrameAddr.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Flip(void);


/**
 * Set status while buffer filling operation finish.
 *
 * @param u4Entry1Size buffer entry1 filled size.
 * @param u4Entry2Size buffer entry2 filled size.
 * @param u4IsEOF flag if end of image file.
 * @return void
 */
void IMG_SetFillBsBufEnd(UINT32 u4Entry1Size, UINT32 u4Entry2Size, BOOL bIsEOF);

/**
 * Get internal buffer info.
 *
 * @param prImgBufInfo buffer description.
 * @return void.
 */
void IMG_GetBufInfo(IMG_BUF_INFO_T* prImgBufInfo);

/**
 * Get bitstream buffer info.
 *
 * @param prStreamBufInfo bitsteam buffer description.
 * @return UINT32 size of available space.
 */
UINT32 IMG_GetBSBufInfo(IMG_LIB_FILL_RING_BUF_T *prStreamBufInfo);

/**
 * Get current decoding state.
 *
 * @param void.
 * @return UINT32 decoding state.
 */
UINT32 IMG_GetDecState(void);


/**
 * Set Image File Size.
 *
 * @Image File Size.
 * @return void
 */

void IMG_Set_ImgSize(UINT32 u4ImgSize);

/**
 * Query decoding progress.
 *
 * @param decoding process.
 * @return void
 */
void IMG_GetProg(UINT32* pu4Status);

INT32 IMG_BsBufManager(VOID);

INT32 IMG_BsBufStartTrsData(UINT32 u4TrsOfst);

/**
 * Query decoding capabilities.
 *
 * @param ptr of decoding capabilities.
 * @return void
 */
void IMG_GetCap(VOID *pv_get_info);


INT32 IMG_BsGetData(UINT32 u4DstAddr, UINT32 u4Offset, UINT32 u4Size);

INT32 IMG_Config(ENUM_IMG_FORMAT img_type);
ENUM_IMG_FORMAT IMG_GetImageType(void);
void IMG_SeDirectFBMem(UINT32 u4PhyAddr, UINT32 u4VirAddr,UINT32 u4Size);

#endif

