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

/******************************************************************************
    Control Constants
******************************************************************************/
#define GFXOSD_OP_BLEND(alphaLevel)		(GFXOSD_OP_BLEND_ZERO|(alphaLevel))
#define GFXOSD_OP_COPY(alphaLevel)		(GFXOSD_OP_COPY_ZERO|(alphaLevel))

/******************************************************************************
	File Inclusions
******************************************************************************/
#ifndef __KERNEL__
#include <osa_api.h>
#include <common.h>
#include "global_configurations.h"
#else
#include "common.h"
#endif
/******************************************************************************
 	Constant Definitions
******************************************************************************/
#define GFXOSD_COLOR_KEY	0x0000	// 16bpp Only

/******************************************************************************
    Macro Definitions
******************************************************************************/

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

/**
 * physical osd id.
*/
typedef enum {
	GFXOSD_0 = 0,	/**< view osd 0  */
	GFXOSD_1 = 1,	/**< view osd 1  */
	GFXOSD_2 = 2	/**< view osd 2  */
} GFXOSD_ID_T;

/**
 * Temporary for MSstar.
*/
typedef enum {
	GFXOSD_FRONT_BUFFER = -2,	/**< Temporary */
	GFXOSD_BACK_BUFFER  = -1,	/**< Temporary */
} GFXOSD_VOSD_TEMP_ID_T;

/**
 * virtual osd id.
*/
typedef enum {
	GFXOSD_VOSD_0 		= 0,	/**< DVB : UI, ATSC : UI */
	GFXOSD_VOSD_1 		= 1,	/**< DVB : MHEG, ATSC : ACAP */
	GFXOSD_VOSD_2 		= 2,	/**< DVB : Subtitle, ATSC : ACC/DCC */
	GFXOSD_VOSD_3 		= 3,	/**< DVB : Teletext/EME, ATSC : EME */
	GFXOSD_NUM_OF_VOSD	= 4,	/**< Number Of VOSDs */
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
	BOOLEAN					bEnable;	/**< virtual osd enable/disable */

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
} GFXOSD_VOSD_INFO_T;

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
} GFXOSD_FRAME_T;

typedef struct 
{
    UINT32  u4BufferSize;
    UINT32 u4BufferSizeOrder;
    UINT32  u4PhysicalAddr;
} GFXOSD_BUFFER_INFO_T;

/******************************************************************************
	Function Declaration
******************************************************************************/
extern void DDI_GFXOSD_Lock(void);
extern void DDI_GFXOSD_Unlock(void);
extern DTV_STATUS_T DDI_GFXOSD_Initialize(void);
extern DTV_STATUS_T DDI_GFXOSD_Finalize(void);
extern DTV_STATUS_T DDI_GFXOSD_GetOSDInfo(GFXOSD_ID_T osdId, GFXOSD_INFO_T *pGfxOsdInfo);
extern DTV_STATUS_T DDI_GFXOSD_Enable(GFXOSD_ID_T osdId);
extern DTV_STATUS_T DDI_GFXOSD_Disable(GFXOSD_ID_T osdId);

extern DTV_STATUS_T DDI_GFXOSD_GetVirtualOSDInfo(GFXOSD_VOSD_ID_T vosdId, GFXOSD_VOSD_INFO_T *pGfxVosdInfo);
extern DTV_STATUS_T DDI_GFXOSD_SetVirtualOSDInfo(GFXOSD_VOSD_ID_T vosdId, GFXOSD_VOSD_INFO_T *pGfxVosdInfo);
extern DTV_STATUS_T DDI_GFXOSD_EnableVirtualOSD(GFXOSD_VOSD_ID_T vosdId);
extern DTV_STATUS_T DDI_GFXOSD_DisableVirtualOSD(GFXOSD_VOSD_ID_T vosdId);
extern BOOLEAN DDI_GFXOSD_IsVirtualOSDEnabled(GFXOSD_VOSD_ID_T vosdId);

extern DTV_STATUS_T DDI_GFXOSD_SetVirtualOSDPalette(GFXOSD_VOSD_ID_T vosdId, UINT32 *pPalette);

extern DTV_STATUS_T DDI_GFXOSD_WritePixel(GFXOSD_VOSD_ID_T vosdId, UINT32 x, UINT32 y, UINT32 color);
extern DTV_STATUS_T DDI_GFXOSD_ReadPixel(GFXOSD_VOSD_ID_T vosdId, UINT32 x, UINT32 y, UINT32 *pColor);
extern DTV_STATUS_T DDI_GFXOSD_FillRect(GFXOSD_VOSD_ID_T vosdId, UINT32 x, UINT32 y,
	UINT32 width, UINT32 height, UINT32 color, GFXOSD_OP_TYPE_T opType);
extern DTV_STATUS_T DDI_GFXOSD_Clear(GFXOSD_VOSD_ID_T vosdId);
extern DTV_STATUS_T DDI_GFXOSD_UpdateScreen(GFXOSD_VOSD_ID_T vosdId,
	UINT32 x, UINT32 y, UINT32 width, UINT32 height, BOOLEAN bImmediate);
extern DTV_STATUS_T DDI_GFXOSD_AllocateFrame(GFXOSD_FRAME_T *pFrame);
extern DTV_STATUS_T DDI_GFXOSD_FreeFrame(GFXOSD_FRAME_T *pFrame);
extern DTV_STATUS_T DDI_GFXOSD_Blit(GFXOSD_FRAME_T *pDst, GFXOSD_FRAME_T *pSrc, GFXOSD_OP_TYPE_T opType);

//extern void DDI_GFXOSD_StartTrueMotionDemo(int step, int y);
EXTERN  void DDI_GFXOSD_StartTrueMotionDemo(int step, int y, int imgWidth, int imgHeight, UINT8 *pImage);

extern void DDI_GFXOSD_StopTrueMotionDemo(void);

#endif /*_GFXOSD_DDI_H_ */



