/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 		gfxosd_ddi.h
 *
 *  header of GFXOSD DDI Functions
 *
 *  @author		Kim, Suk (stonekim@lge.com)
 *  @author		Kwak, wongee (wongeekwak@lge.com)
 *  @version	1.0
 *  @date		2008.4.
 *  @note		Additional information.
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _GFXOSD_DDI_H_
#define _GFXOSD_DDI_H_

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
    Control Constants
******************************************************************************/
#define GFXOSD_OP_BLEND(alphaLevel)		(GFXOSD_OP_BLEND_ZERO|(alphaLevel))
#define GFXOSD_OP_COPY(alphaLevel)		(GFXOSD_OP_COPY_ZERO|(alphaLevel))

/******************************************************************************
	File Inclusions
******************************************************************************/
#include <osa_api.h>
#include <common.h>
#include "global_configurations.h"

#ifdef	INCLUDE_GFXOSD_ON_GPU_DDI
#include "ddi_gpu_data.h"
#endif

/******************************************************************************
 	Constant Definitions
******************************************************************************/
#define GFXOSD_COLOR_KEY	0x0000	// 16bpp Only
#define GFXOSD_SURFACE_HANDLE   unsigned long

#ifdef INCLUDE_MCB
//#define MAX_THUMBNAIL_SURFACE_NUM		15+1	// for 15 thumbnails and just updated thumbnail
#define MAX_THUMBNAIL_SURFACE_NUM		3	// for 15 thumbnails and just updated thumbnail
#define TMP_THUMBNAIL_SURFACE_NUM		2

#endif

/******************************************************************************
    Macro Definitions
******************************************************************************/
#define GFXOSD_ALIGNED_VALUE(value, align)  ( ((value) + ((align)-1)) & ~((align)-1) )
#define GFXOSD_WORDUNIT     256 // 128 bits

/******************************************************************************
	Type Definitions
******************************************************************************/

/**
 * pixel format.
 * ARGB888, ARGB4444, 8bpp palette ..etc .
*/
typedef enum {
	GFXOSD_ARGB8888 = 0,		/**< ARGB8888 format */
	GFXOSD_ARGB4444 = 1,		/**< ARGB4444 format */
	GFXOSD_ARGB1555 = 2,		/**< ARGB1555 Pallette format */
	GFXOSD_8BPP_PAL = 3,		/**< 8bit Pallette format */
	GFXOSD_YUV2		= 4,
} GFXOSD_FORMAT_T;

/**
 * pixel depth.
 * 8bpp, 16bpp, 32bpp.
*/
typedef enum {
	GFXOSD_8BPP 	= 0,			/**< 8 bit per pixel */
	GFXOSD_16BPP 	= 1,			/**< 16 bit per pixel */
	GFXOSD_32BPP 	= 2,			/**< 32 bit per pixel */
} GFXOSD_PIXELDEPTH_T;

//FIXME zuel_091021
/**
 * physical osd id.
*/
typedef enum {
	GFXOSD_0 			= 0,	/**< view osd 0  */
	GFXOSD_1 			= 1,	/**< view osd 1  */
	GFXOSD_2 			= 2,	/**< view osd 2  */
#if (PLATFORM_TYPE==LG_PLATFORM)
	GFXOSD_3			= 3,	/**< view osd 3  */
	GFXOSD_NUM_OF_POSD	= 4,	/**< Number Of POSDs */
#else
	GFXOSD_NUM_OF_POSD	= 3,	/**< Number Of POSDs */
#endif
} GFXOSD_ID_T;


/**
 * Temporary for MSstar.
*/
typedef enum {
	GFXOSD_FRONT_BUFFER = -2,	/**< Temporary */
	GFXOSD_BACK_BUFFER  = -1,	/**< Temporary */
} GFXOSD_VOSD_TEMP_ID_T;

//FIXME zuel_091021
/**
 * virtual osd id.
*/
typedef enum {
	GFXOSD_VOSD_0 		= 0,	/**< DVB : UI, ATSC : UI */
	GFXOSD_VOSD_1 		= 1,	/**< DVB : MHEG, ATSC : ACAP */
	GFXOSD_VOSD_2 		= 2,	/**< DVB : Subtitle, ATSC : ACC/DCC */
	GFXOSD_VOSD_3 		= 3,	/**< DVB : Teletext/EME, ATSC : EME */
	
#if (defined(INCLUDE_DIRECTFB))
#ifdef INCLUDE_DIRECTFB_MULTILAYER
	GFXOSD_VOSD_4		= 4,	/**< DVB : Buffer for DirectFB and EMF Thumbnail : browser*/
	GFXOSD_VOSD_5		= 5,	/**< DVB : Buffer for DirectFB (Second layer) : flash engine*/
	GFXOSD_VOSD_6		= 6,	/**< DVB : Buffer for DirectFB (Third layer) : app store */
	GFXOSD_VOSD_7		= 7,	/**< DVB : Buffer for DirectFB (Fourth layer) : netcast main (to improve speed of entry) */

#if(PLATFORM_TYPE == MSTAR_PLATFORM) //FIXME : temp
	GFXOSD_VOSD_8		= 8,	/**< HW Cursor>*/
#ifdef INCLUDE_MHP
	GFXOSD_VOSD_9		= 9,
	GFXOSD_NUM_OF_VOSD	= 10,	/**< Number Of VOSDs */
#else
	GFXOSD_NUM_OF_VOSD	= 9,	/**< Number Of VOSDs */
#endif

#else

#ifdef INCLUDE_MHP
	GFXOSD_VOSD_8		= 8,
	GFXOSD_NUM_OF_VOSD	= 9,	/**< Number Of VOSDs */
#else
	GFXOSD_NUM_OF_VOSD	= 8,	/**< Number Of VOSDs */
#endif
#endif

#else
	GFXOSD_VOSD_4		= 4,	/**< DVB : Buffer for DirectFB and EMF Thumbnail */
	GFXOSD_NUM_OF_VOSD	= 5,	/**< Number Of VOSDs */
#endif

#else
	GFXOSD_NUM_OF_VOSD	= 4,	/**< Number Of VOSDs */
#endif
} GFXOSD_VOSD_ID_T;

/**
 * Operation Type
 * GFXOSD_OP_BLEND(alphaLevel) : Adst = Asrc*alphaLevel+(1-Asrc*alphaLevel)*Adst
 * GFXOSD_OP_COPY(alphaLevel) : Adst = Asrc*alphaLevel
 */
typedef enum {
	GFXOSD_OP_DEFAULT		= 0,		/**< Default */
	GFXOSD_OP_XOR			= 1,		/**< XOR */
	GFXOSD_OP_BLEND_ZERO	= 0x8000,	/**< Blend with Alpha Level = 0 */
	GFXOSD_OP_BLEND_DEFAULT	= 0x80ff,	/**< Blend with Alpha Level = 0xff */
	GFXOSD_OP_COPY_ZERO		= 0x8100,	/**< Copy with Alpha Level = 0 */
	GFXOSD_OP_COPY_DEFAULT	= 0x81ff,	/**< Copy with Alpha Level = ff : equivalent with GFXOSD_OP_DEFAULT */
} GFXOSD_OP_TYPE_T;

/**
 * virtual osd info.
*/
typedef struct {
	//To be removed --->
	BOOLEAN					bEnable:1;	/**< virtual osd enable/disable */

	GFXOSD_ID_T				osdId;		/**< physical osd id */

	UINT16					viewX;		/**< virtual osd scaling info */
	UINT16					viewY;		/**< virtual osd scaling info */
	UINT16					viewWidth;	/**< virtual osd scaling info */
	UINT16					viewHeight;	/**< virtual osd scaling info */

	UINT16					dispX;		/**< info to display virtual osd */
	UINT16					dispY;		/**< info to display virtual osd */
	UINT16					dispWidth;	/**< info to display virtual osd */
	UINT16					dispHeight;	/**< info to display virtual osd */

	GFXOSD_FORMAT_T			format;		/**< osd format */
	GFXOSD_PIXELDEPTH_T		pxlDepth;	/**< byte per pixel */
	UINT16					width;		/**< virtual osd width */
	UINT16					height;		/**< virtual osd height */
	UINT16					stride;		/**< virtual osd byte per line */
	UINT8					alphaValue;	/**< Global Alpha Value(only when the OSD Base is ARGB1555) */
	void					*pAddr;		/**< virtual osd address */
	//to be removed <---
#if 1//(PLATFORM_TYPE == X86_PLATFORM)
#ifdef INCLUDE_GFXOSD_ON_GPU_DDI
	GFXOSD_VOSD_ID_T vosdId;		/**< Layer ID */
	BOOLEAN bUseDoubleBuffer;		/**< whether doublebuffer is used or not */
	UINT8 doubleBufferIndex;		/**< index of backbuffer (if backbuffer is available) */
	UINT8 opacity;					/**< opacity */
	UINT8 zOrder;					/**< z order (0 means bottom layer) */
	GPU_RECT_T regionRect;			/**< retion of layer */

	// GPU Surface
	GPU_SURFACE_INFO_T surfaceInfo[2];	/**< GPU surface */
#endif
#endif
} GFXOSD_VOSD_INFO_T;

#ifdef INCLUDE_GFXOSD_ON_GPU_DDI
/**
 * physical osd info.
 */
typedef struct {
	BOOLEAN					bEnable;			/**< osd enable/disable */

	GFXOSD_FORMAT_T			format;				/**< osd format */
	GFXOSD_PIXELDEPTH_T		pxlDepth; 			/**< byte per pixel */
	UINT16					width;				/**< physical osd width */
	UINT16					height;				/**< physical osd height */

	UINT32					nNumOfVirtualOsds;	/**< the number of virtual osd */
#if (PLATFORM_TYPE == BCM_PLATFORM || PLATFORM_TYPE == MTK_PLATFORM || PLATFORM_TYPE == LG_PLATFORM )
	GPU_SURFACE_INFO_T		surfaceInfo[2];		/**< surfaceInfo of virtual osd */
#endif
} GFXOSD_INFO_T;
#else
/**
 * physical osd info.
 */
typedef struct {
	BOOLEAN					bEnable;			/**< osd enable/disable */

	GFXOSD_FORMAT_T			format;				/**< osd format */
	GFXOSD_PIXELDEPTH_T		pxlDepth; 			/**< byte per pixel */
	UINT16					width;				/**< physical osd width */
	UINT16					height;				/**< physical osd height */

	UINT32					nNumOfVirtualOsds;	/**< the number of virtual osd */
} GFXOSD_INFO_T;
#endif

/**
 * BLIT frame
 */
typedef struct {
	UINT16					x;			/**< x */
	UINT16					y;			/**< y */
	UINT16					width;		/**< width */
	UINT16					height;		/**< height */

	void 					*pAddr;		/**< Frame Base Addr */
	UINT16					stride;		/**< available when pAddr!=NULL */
	GFXOSD_FORMAT_T			format;		/**< available when pAddr!=NULL */
	GFXOSD_PIXELDEPTH_T		pxlDepth; 	/**< available when pAddr!=NULL */
	GFXOSD_VOSD_ID_T		vosdId;		/**< available when pAddr==NULL */
#ifdef	INCLUDE_GFXOSD_ON_GPU_DDI
#if (PLATFORM_TYPE != X86_PLATFORM) || (PLATFORM_TYPE == MSTAR_PLATFORM)
	GPU_SURFACE_INFO_T 		surfaceInfo;
#endif
#endif
} GFXOSD_FRAME_T;

/**
 * gfxosd rect.
 *
 * @see : cpplover adapt from gdvr for thumbnail
*/
typedef struct {
	UINT16	x;		/**< x coordinate of rect */
	UINT16	y;		/**< y coordinate of rect */
	UINT16	w;		/**< w coordinate of rect */
	UINT16	h;		/**< h coordinate of rect */
} GFXOSD_RECT_T;

/**
 * gfxosd rect.
 *
 * @see : output information for thumbnail
*/
typedef struct {
	UINT16	x;				/**< x coordinate of rect */
	UINT16	y;				/**< y coordinate of rect */
	UINT16	w;				/**< w coordinate of rect */
	UINT16	h;				/**< h coordinate of rect */
	UINT16	rotate_flag;	/**< rotate the image by 0(0),90(1),180(2),270(3) degree clockwise */
} GFXOSD_OUTPUT_INFO_T;

/**
 * display update mode
 */
typedef enum {
	GFXOSD_UPDATE_MODE_NORMAL	= 0,
	GFXOSD_UPDATE_MODE_FLIPONLY
} GFXOSD_UPDATE_MODE_T;


#ifdef INCLUDE_MOUSE

/**
 * gesture type info.
 */
typedef enum tagGESTURE_TYPE_T
{
	GESTURE_NONE = 0,
	GESTURE_POINT,
	GESTURE_DRAG_N,
	GESTURE_DRAG_S,
	GESTURE_CLICK,
	GESTURE_WRITING,
	GESTURE_WRITING_P,
	GESTURE_WRITING_INIT,
	GESTURE_WRITING_TIMEOUT
} GESTURE_TYPE_T;


/**
 * gesture surface.
 */
typedef enum tagGESTURE_SURFACE_T
{
	GESTURE_SURFACE_POINT_N=0,
	GESTURE_SURFACE_POINT_S,
	GESTURE_SURFACE_POINT_BG,
	GESTURE_SURFACE_POINT_BG4,
	GESTURE_SURFACE_PEN,
	GESTURE_SURFACE_PEN_P,
	GESTURE_SURFACE_CH_POINT,
	GESTURE_SURFACE_PBG,
	GESTURE_SURFACE_PBG4,
	GESTURE_SURFACE_CH_HELP,
	GESTURE_SURFACE_DRAG_N,
	GESTURE_SURFACE_DRAG_S,
	GESTURE_SURFACE_MAX
} GESTURE_SURFACE_T;

/**
 * gesture surface size type
 */
typedef struct tagGESTURE_SURFACE_SIZE_T
{
	UINT16 Width;
	UINT16 Height;
}GESTURE_SURFACE_SIZE_T;

/**
 * gesture surface info.
 */
typedef struct tagGESTURE_SURFACE_INFO_T
{
	void 	*pAddr;
	UINT16 	width;
	UINT16 	height;
}GESTURE_SURFACE_INFO_T;


#endif



/**
 * gfxosd rect.
 *
 * @see : cpplover adapt from gdvr for thumbnail
*/
typedef struct {
	UINT8	*pBuffer;
	UINT16	width;
	UINT16	height;
} GFXOSD_ARGB_IMAGE_T;

/**
 *
 *
 * @see : added by minsup
*/

typedef struct {
	UINT8			*pBuffer;
	GFXOSD_RECT_T	rect;
} GFXOSD_ARGB_EX_IMAGE_T;

//#ifdef INCLUDE_GFXOSD_ON_GPU_DDI

/**
 * @brief This structure describes a information about imageFormat.
 */
typedef enum {
    GFXOSD_IMAGE_FORMAT_JPEG           = 0x00000000,
    GFXOSD_IMAGE_FORMAT_PNG,
    GFXOSD_IMAGE_FORMAT_GIF,
    GFXOSD_IMAGE_FORMAT_BMP,
    GFXOSD_IMAGE_FORMAT_BUFFER,
    GFXOSD_IMAGE_FORMAT_MAX
} GFXOSD_IMAGE_FORMAT_T;

/**
 * @brief This structure describes a information about image.
 */
typedef struct {
	UINT32		imageLength;
	GFXOSD_FORMAT_T	pixelFormat;
	BOOLEAN		bimageFromBuffer;
	CHAR 		*imagePath;
	UINT32		*imagePtr;
	GFXOSD_IMAGE_FORMAT_T	imageFormat;
	GFXOSD_RECT_T rect;
} GFXOSD_IMAGE_INFO_T;
//#endif

//#ifdef INCLUDE_TRIDTV
/**
 * 3DTV OSD mode
 */
typedef enum {
	GFXOSD_3D_MODE_OFF	=	0,
	GFXOSD_3D_MODE_ON	=	1,
} GFXOSD_3D_MODE_T;

/**
 * 3DTV OSD type
 */
typedef enum {
	GFXOSD_3D_TYPE_TOP_BOTTOM		=	0,
	GFXOSD_3D_TYPE_SIDE_BY_SIDE		=	1,
	GFXOSD_3D_TYPE_CHECKER_BOX		=	2,
	GFXOSD_3D_TYPE_FULL_HD_3D		=	3,
//	GFXOSD_3D_TYPE_MODE_EXIT		=	4,
	GFXOSD_3D_TYPE_OFF_2D_3D		=	5,
	GFXOSD_3D_TYPE_2D_3D			=	6,
	GFXOSD_3D_1080P_FRAME_PACKED 	=	7,
	GFXOSD_3D_720P_FRAME_PACKED 	=	8,
	GFXOSD_3D_MAX					=	GFXOSD_3D_720P_FRAME_PACKED,
} GFXOSD_3D_TYPE_T;

/**
 * 3DTV OSD LR
 */
typedef enum {
	GFXOSD_3D_FRAME_R_L	=	0,
	GFXOSD_3D_FRAME_L_R	=	1,
} GFXOSD_2D_FRAME_LR_T;

/**
 * 3DTV POSD type
 */
typedef enum {
	GFXOSD_POSD_2D						=	0,
	GFXOSD_POSD_3D_1080P_FRAME_PACKED	=	1,
	GFXOSD_POSD_3D_720P_FRAME_PACKED	=	2,
	GFXOSD_POSD_2D_TO_3D				=	3,
	GFXOSD_POSD_NONE					=	4,
} GFXOSD_POSD_TYPE_T;

/**
 * 3DTV Picture Size Type
 */
typedef enum {
	GFXOSD_3D_PICSIZE_JUSTSCAN	=	0,
	GFXOSD_3D_PICSIZE_OVERSCAN	=	1,
} GFXOSD_3D_PICTURE_SIZE_T;

//#endif
/******************************************************************************
	Function Declaration
******************************************************************************/
extern void DDI_GFXOSD_Lock(void);
extern void DDI_GFXOSD_Unlock(void);
extern DTV_STATUS_T DDI_GFXOSD_Initialize(void);
//#if (PLATFORM_TYPE == MSTAR_PLATFORM)
//extern DTV_STATUS_T DDI_GFXOSD_ChangeGOPSetting(void);
//#endif
extern DTV_STATUS_T DDI_GFXOSD_Finalize(void);
extern DTV_STATUS_T DDI_GFXOSD_GetOSDInfo(GFXOSD_ID_T osdId, GFXOSD_INFO_T *pGfxOsdInfo);
extern DTV_STATUS_T DDI_GFXOSD_Enable(GFXOSD_ID_T osdId);
extern DTV_STATUS_T DDI_GFXOSD_Disable(GFXOSD_ID_T osdId);

extern DTV_STATUS_T DDI_GFXOSD_GetVirtualOSDInfo(GFXOSD_VOSD_ID_T vosdId, GFXOSD_VOSD_INFO_T *pGfxVosdInfo);
extern void * DDI_GFXOSD_GetVirtualOSDSurface(GFXOSD_VOSD_ID_T vosdId);
extern DTV_STATUS_T DDI_GFXOSD_SetVirtualOSDInfo(GFXOSD_VOSD_ID_T vosdId, GFXOSD_VOSD_INFO_T *pGfxVosdInfo);
extern DTV_STATUS_T DDI_GFXOSD_EnableVirtualOSD(GFXOSD_VOSD_ID_T vosdId);
extern DTV_STATUS_T DDI_GFXOSD_DisableVirtualOSD(GFXOSD_VOSD_ID_T vosdId);
extern BOOLEAN DDI_GFXOSD_IsVirtualOSDEnabled(GFXOSD_VOSD_ID_T vosdId);

extern DTV_STATUS_T DDI_GFXOSD_SetVirtualOSDPalette(GFXOSD_VOSD_ID_T vosdId, UINT32 *pPalette);

extern DTV_STATUS_T DDI_GFXOSD_WritePixel(GFXOSD_VOSD_ID_T vosdId, UINT32 x, UINT32 y, UINT32 color);
extern DTV_STATUS_T DDI_GFXOSD_ReadPixel(GFXOSD_VOSD_ID_T vosdId, UINT32 x, UINT32 y, UINT32 *pColor);
extern DTV_STATUS_T DDI_GFXOSD_FillRect(GFXOSD_VOSD_ID_T vosdId, UINT32 x, UINT32 y, UINT32 width, UINT32 height, UINT32 color, GFXOSD_OP_TYPE_T opType);
extern DTV_STATUS_T DDI_GFXOSD_FillRectToFrame(GFXOSD_FRAME_T *pFrame, UINT32 x, UINT32 y, UINT32 width, UINT32 height, UINT32 color, GFXOSD_OP_TYPE_T opType);
extern DTV_STATUS_T DDI_GFXOSD_Clear(GFXOSD_VOSD_ID_T vosdId);
extern DTV_STATUS_T DDI_GFXOSD_UpdateScreen(GFXOSD_VOSD_ID_T vosdId, UINT32 x, UINT32 y, UINT32 width, UINT32 height, BOOLEAN bImmediate);
extern DTV_STATUS_T DDI_GFXOSD_FreezeScreen(void);
extern DTV_STATUS_T DDI_GFXOSD_AllocateFrame(GFXOSD_FRAME_T *pFrame);
extern DTV_STATUS_T DDI_GFXOSD_FreeFrame(GFXOSD_FRAME_T *pFrame);
extern DTV_STATUS_T DDI_GFXOSD_StretchMemoryFrame(GFXOSD_FRAME_T *pDst, GFXOSD_FRAME_T *pSrc);
extern DTV_STATUS_T DDI_GFXOSD_Blit(GFXOSD_FRAME_T *pDst, GFXOSD_FRAME_T *pSrc, GFXOSD_OP_TYPE_T opType);
extern void DDI_GFXOSD_SetScreenUpdateMode(GFXOSD_UPDATE_MODE_T updateMode);
// coding by jaehak.lee
extern DTV_STATUS_T DDI_GFXOSD_IsValidAllocateFrameMemory(GFXOSD_FRAME_T *pFrame);
//Get  frame  handle - coding by jaehak.lee, below code must be replaced by other method.
extern void* DDI_GFXOSD_GetHandleFromFrame(GFXOSD_FRAME_T *pFrame);
extern GFXOSD_UPDATE_MODE_T DDI_GFXOSD_GetScreenUpdateMode(void);

#if (PLATFORM_TYPE==BCM_PLATFORM)
extern DTV_STATUS_T DDI_GFXOSD_LockExclusiveUpdateFlag(GFXOSD_VOSD_ID_T vosdId);
extern DTV_STATUS_T DDI_GFXOSD_UnlockExclusiveUpdateFlag(void);
#endif

#ifdef INCLUDE_DIRECTFB
extern void* DDI_GFXOSD_GetDFBHandler(void);
#endif

extern DTV_STATUS_T DDI_GFXOSD_StartTrueMotionDemo(int step, int y, int imgWidth, int imgHeight, UINT8 *pImage, BOOLEAN reverse);
extern DTV_STATUS_T DDI_GFXOSD_StopTrueMotionDemo(void);

extern DTV_STATUS_T DDI_GFXOSD_StartVCOMPattern(void);
extern DTV_STATUS_T DDI_GFXOSD_StopVCOMPattern(void);
extern DTV_STATUS_T DDI_GFXOSD_DrawImageOnOSD(GFXOSD_VOSD_ID_T vosdId, GFXOSD_IMAGE_INFO_T *pImage, GFXOSD_OUTPUT_INFO_T *pOut_Info);



extern DTV_STATUS_T DDI_GFXOSD_ScaleImage (
	GFXOSD_ID_T 	osdId,
	GFXOSD_VOSD_ID_T srcSubId,
	UINT32 			srcX,
	UINT32 			srcY,
	UINT32 			srcW,
	UINT32 			srcH,
	GFXOSD_VOSD_ID_T destSubId,
	UINT32			destX,
	UINT32			destY,
	UINT32 			destW,
	UINT32 			destH
);

extern DTV_STATUS_T DDI_GFXOSD_StoreImage(
	GFXOSD_ID_T 	osdId,
	GFXOSD_VOSD_ID_T subId,
	UINT32			xPosSrc,
	UINT32			yPosSrc,
	UINT32			xPosDest,
	UINT32			yPosDest,
	UINT32 			width,
	UINT32 			height,
	UINT32			srcPitch, /* it means just source pitch not (pitch * bpp) */
	UINT8			*pImage
);

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
extern DTV_STATUS_T	DDI_GFXOSD_ShowJpeg_HW(unsigned char *jpgData, int jpgLength, GFXOSD_RECT_T *srcrect, GFXOSD_VOSD_ID_T destosdId, GFXOSD_RECT_T *dstrect);
#endif

DTV_STATUS_T	DDI_GFXOSD_InitHWDecoder(void);

#ifdef INCLUDE_MCB
void 			DDI_GFXOSD_FinalizeHWDecoder(void);
DTV_STATUS_T 	DDI_GFXOSD_CreateMCBSurface(int surfaceIndex, int width, int height);
DTV_STATUS_T 	DDI_GFXOSD_RemoveMCBSurface(int surfaceIndex);

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
DTV_STATUS_T	DDI_GFXOSD_DecodeJpgAndShow(unsigned char *jpegData, int jpegLength, GFXOSD_RECT_T *srcRect, GFXOSD_VOSD_ID_T dstOsdId, GFXOSD_RECT_T *dstRect);
#endif

DTV_STATUS_T 	DDI_GFXOSD_DecodeJpgFile(int surfaceIndex, char *filename);
DTV_STATUS_T 	DDI_GFXOSD_DecodeJpgBuffer(int surfaceIndex, UINT8 *pJpegData, UINT32 jpegLength);
DTV_STATUS_T 	DDI_GFXOSD_DrawScanImageOnUI(int surfaceIndex, GFXOSD_RECT_T srcRect, GFXOSD_VOSD_ID_T dstOsdId, GFXOSD_RECT_T dstRect);

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
DTV_STATUS_T	DDI_GFXOSD_CaptureVideo(int surfaceIndex, UINT16 width, UINT16 height);
DTV_STATUS_T 	DDI_GFXOSD_CaptureAnalogVideo( GFXOSD_ARGB_IMAGE_T *pImage);
#endif

UINT8 * 		DDI_GFXOSD_GetThumbnailImage(UINT32 surfaceIndex);

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
DTV_STATUS_T 	DDI_GFXOSD_DrawImage( GFXOSD_ARGB_IMAGE_T *pImage );
#endif

#endif
UINT8 * 		DDI_GFXOSD_CopyOsdImage(GFXOSD_VOSD_ID_T vosdId, GFXOSD_RECT_T rect);

//FIXME jinhyuk : This function is not standard. and This function is not use in GP3
//extern DTV_STATUS_T	DDI_GFXOSD_DrawImageFile(unsigned char *imagePath, GFXOSD_VOSD_ID_T vosdId, GFXOSD_RECT_T *dstRect, GFXOSD_IMAGE_FORMAT_T format);

//3DTV 관련
extern DTV_STATUS_T	DDI_GFXOSD_Set3DMode(GFXOSD_3D_MODE_T gfx3DMode, GFXOSD_3D_TYPE_T gfx3DType);
extern DTV_STATUS_T DDI_GFXOSD_Set3DModeEx(TRIDTV_INPUT_CONFIG_INFO_T* p3DModeInfo);

extern DTV_STATUS_T DDI_GFXOSD_SetPOSD(GFXOSD_POSD_TYPE_T posd_type);
extern DTV_STATUS_T DDI_GFXOSD_GetOSDState(UINT32 *pPosd_type, UINT32 *pGfx3DMode, UINT32 *pGfx3DType);
//_________/

extern void  *DDI_GFXOSD_AllocAndDumpVOSD(GFXOSD_VOSD_ID_T vosdId, UINT32 *x, UINT32 *y, UINT32 *w, UINT32 *h);
extern void  *DDI_GFXOSD_AllocAndDumpVOSDMem(GFXOSD_VOSD_ID_T vosdId, UINT32 *pitch, UINT32 *width, UINT32 *height);

extern DTV_STATUS_T DDI_GFXOSD_CreateAcrodeaSurface(GFXOSD_FRAME_T *pFrame, UINT32 *pHandle);
extern DTV_STATUS_T DDI_GFXOSD_DestroyAcrodeaSurface(GFXOSD_FRAME_T *pFrame);
extern DTV_STATUS_T DDI_GFXOSD_UpdateAcrodea( UINT32 x, UINT32 y, UINT32 width, UINT32 height);
extern DTV_STATUS DDI_GFXOSD_ClearOSDAcrodea(UINT32 x, UINT32 y, UINT32 width, UINT32 height);
extern void DDI_GFXOSD_BlitBufferToUIOSD(void *surfaceIndex,
	UINT16 srcX, UINT16 srcY, UINT16 srcW, UINT16 srcH,
	UINT16 x, UINT16 y, UINT16 width, UINT16 height, BOOLEAN blitCombine, UINT32 constantAlpha ,BOOLEAN withColorkey);
/////////지우자
extern void DDI_GFXOSD_FadeEffect(
		void *surfaceIndex,
		UINT16 srcX, UINT16 srcY, UINT16 srcW, UINT16 srcH,
		UINT16 x, UINT16 y, UINT16 width, UINT16 height, BOOLEAN blitCombine, UINT32 constantAlpha );

// 2D Effect의 투명도 Effect 를 위해 추가됨.
void DDI_GFXOSD_OpacityEffect(
		void *pSrcSurface, void *pDstSurface,
		UINT16 srcX, UINT16 srcY, UINT16 srcW, UINT16 srcH,
		UINT16 dstX, UINT16 dstY, UINT16 dstW, UINT16 dstH, UINT8 opacity, BOOLEAN alpha );
//2D Effect에서 Pixmap으로 부터 Surface 생성을 위해 추가됨.
void* DDI_GFXOSD_CreateSurfaceFromFrame(GFXOSD_FRAME_T *pSrcFrame);

extern
	void DDI_GFXOSD_BlitBufferToBuffer(
		void *surfaceIndex, void *dstsurfaceindex,
		UINT16 srcX, UINT16 srcY, UINT16 srcW, UINT16 srcH,
		UINT16 x, UINT16 y, UINT16 width, UINT16 height, BOOLEAN blitCombine, UINT32 constantAlpha );

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
extern DTV_STATUS_T DDI_GFXOSD_Blit0( void *surfaceIndex, GFXOSD_RECT_T srcRect, void *dstsurfaceIndex ,GFXOSD_RECT_T dstRect, BOOLEAN blitCombine, UINT32 constantAlpha );
#endif

extern void DDI_GFXOSD_GetSurfaceInfo(void *pSurface, GFXOSD_FRAME_T *pSurfaceInfo);

extern void *DDI_GFXOSD_CreateSurface(UINT16 width, UINT16 height);
/////지우자
extern DTV_STATUS DDI_GFXOSD_FillRectToSurface(void *pSurface, UINT32 x, UINT32 y,
	UINT32 width, UINT32 height, UINT32 color, GFXOSD_OP_TYPE_T opType);

//return address of decoded buffer
extern void *DDI_GFXOSD_DecodePNGImgfromDBEx(UINT8* imgBuffer, UINT32 imgSize, UINT16 width, UINT16 height);
//return address of surface handler
extern void* DDI_GFXOSD_DecodePNGImgfromDB(UINT8* imgBuffer, UINT32 imgSize,UINT16 *width, UINT16 *height);

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
extern void DDI_GFXOSD_BlitBuffer(UINT16 x, UINT16 y, UINT16 width, UINT16 height, void *surfaceIndex, BOOLEAN blitCombine );
#endif

extern void DDI_GFXOSD_DestroySurface(void* surfaceHandle);

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
DTV_STATUS DDI_GFXOSD_UpdateScreenUsingOneFrameBuffer( UINT32 x, UINT32 y, UINT32 width, UINT32 height);
#endif

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
DTV_STATUS_T 	DDI_GFXOSD_EMP_Blit(void *src, GFXOSD_RECT_T srcRect, GFXOSD_RECT_T dstRect);	// spmarine
#endif

void			DDI_GFXOSD_Set3DPictureSize(GFXOSD_3D_PICTURE_SIZE_T eScanType);
void 			DDI_GFXOSD_ConvertOverscan(GFXOSD_RECT_T inRect, GFXOSD_RECT_T *pOutRect);


#ifdef INCLUDE_M3D	// hskang add 20091019 : for 3D Schedule management
void* DDI_GFXOSD_GetGrp2DHandleForM3d(void);
void* DDI_GFXOSD_GetVOSDHandleForM3d(UINT32 vOsdId);
void DDI_GFXOSD_GetVOSDSizeForM3d(UINT32 vOsdId, UINT32* pWidth, UINT32* pHeight);
void DDI_GFXOSD_EnableVOSD1(void);
void DDI_GFXOSD_DisableVOSD1(void);
void DDI_GFXOSD_UpdateVOSD1ForM3D(void);
#endif


#ifdef INCLUDE_MOUSE
DTV_STATUS_T DDI_GFXOSD_SetGestureSurface( UINT32 SurfaceIndex, void* SurfaceHandle, UINT32 Width, UINT32 Height );
DTV_STATUS_T DDI_GFXOSD_SetGestureType( UINT32 nGestureType );
DTV_STATUS_T DDI_GFXOSD_DrawCursor( UINT32 x, UINT32 y, int nDrawCursorType, BOOLEAN bDrawGestureHelp );

#if 0 //FIXME jinhyuk : This function is not standard. and This function is not use in GP3
void DDI_GFXOSD_BlitBuffer2Screen(UINT16 x, UINT16 y, UINT16 width, UINT16 height);
#endif

// #ifdef INCLUDE_AEL과 중복: 정리 필요.
//void DDI_GFXOSD_GetSurfaceInfo(void *pSurface, GFXOSD_FRAME_T *pSurfaceInfo);
//void *DDI_GFXOSD_CreateSurface(UINT16 width, UINT16 height);
//void* DDI_GFXOSD_DecodePNGImgfromDB(UINT8* imgBuffer, UINT32 imgSize,UINT16 *width, UINT16 *height);
//void DDI_GFXOSD_DestroySurface(void* surfaceHandle);
//void DDI_GFXOSD_BlitBufferToBuffer(
//	void *surfaceIndex, void *dstsurfaceindex,
//	UINT16 srcX, UINT16 srcY, UINT16 srcW, UINT16 srcH,
//	UINT16 x, UINT16 y, UINT16 width, UINT16 height, BOOLEAN blitCombine, UINT32 constantAlpha );
//void DDI_GFXOSD_BlitBuffer(UINT16 x, UINT16 y, UINT16 width, UINT16 height, void *surfaceIndex, BOOLEAN blitCombine );
//지운거 다시 풀자
//void DDI_GFXOSD_BlitBuffer2Screen(UINT16 x, UINT16 y, UINT16 width, UINT16 height);
#endif

#ifdef __cplusplus
}
#endif

#endif /*_GFXOSD_DDI_H_ */



