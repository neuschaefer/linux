/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file       gpu_ddi.h
 *
 *  header of GPU DDI Functions
 *
 *  @author     Yang, Duck-Yong (yducky@lge.com)
 *  @author     Hong Jin-Hyuk (zuel@lge.com)
 *  @version    1.0
 *  @date       2009.3.10
 *  @note       Additional information.
 *  @see
 */

/******************************************************************************
    Header File Guarder
******************************************************************************/
#ifndef _GPU_DDI_H_
#define _GPU_DDI_H_

//#include "general.h"
#include "ddi_gpu_data.h"
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
    Control Constants
******************************************************************************/
#define GPU_OP_BLEND(alphaLevel)        (GPU_OP_BLEND_ZERO|(alphaLevel))
#define GPU_OP_COPY(alphaLevel)     (GPU_OP_COPY_ZERO |(alphaLevel))

/******************************************************************************
    File Inclusions
******************************************************************************/
//#include <osa_api.h>
//#include <common.h>
//#include "global_configurations.h"
/******************************************************************************
    Constant Definitions
******************************************************************************/
#define GPU_COLOR_KEY   0x0000  // 16bpp Only
#define MAX_GWIN_FB_SUPPORT 32
///Defination for GWIN Status
/// GWIN Fail
#define GWIN_FAIL         0x00
/// GWIN OK
#define GWIN_OK           0x01
/// GWIN NOT Available
#define GWIN_NO_AVAILABLE 0xFF
/// GWIN Keep init OSD window
#define GWIN_KEEP_INIT_OSDWIN  0


#define _DEBUG_GOP_BEHAVIOR_        0       //1: Enable CC, 0: Disable
//#define GWIN_OSD_DEFAULT  MAX_GWIN_SUPPORT
#define FB_FMT_AS_DEFAULT   0xFFFF

#define GWIN_SDRAM_NULL 0x30
#define GWIN_SDRAM_PG_UNIT 0x00000010  // 16-byte page-unit for gwin_sdram

//FIXME zuel_091021
#define _MAX_NUM_OF_LAYERS     5


#define CONFIG_4OSD 1  //Arki definition
#define SUBTITLE_IN_IP 0

typedef UINT8 GWINID;
typedef UINT8 Gwin_Map;

/**
 * Frame buffer attributes for GWIN
*/
typedef struct
{
    UINT8 gWinId;      ///< id
    UINT8 enable:1;    ///< 4bits enable
    UINT8 allocated:1; ///< 4bits

    UINT16 x0;         ///< U16
    UINT16 y0;         ///< U16
    UINT16 x1;         ///< U16
    UINT16 y1;         ///< U16
    UINT16 width;      ///< U16
    UINT16 height;     ///< U16
    UINT16 pitch;      ///< U16
    UINT16 fbFmt;      ///< U16

    UINT32 addr;       ///< U32
    UINT32 size;       ///< U32

    UINT8 next;        ///< U8
    UINT8 prev;        ///< U8
    UINT8 string;      //store which AP use this FB
    //U8 flag;      ///< bit 0:scroll
    UINT16 s_x;        ///< U16
    UINT16 s_y;        ///< U16
    UINT16 dispWidth;  ///< U16
    UINT16 dispHeight; ///< U16
} Gwin_FB_Attr;
/******************************************************************************
    Macro Definitions
******************************************************************************/
#if 0
U32 GOP_GWIN_RB_BASE_ADR;
U32 GOP_GWIN_RB_BASE_LEN;
U32 GOP_GWIN_RB_BASE_ADR2;
U32 GOP_GWIN_RB_BASE_LEN2;
U32 gopSdramFree;
U32 gopSdramFree2;
#endif
/******************************************************************************
    Type Definitions
******************************************************************************/

/**
 * pixel format.
 * ARGB888, ARGB4444, 8bpp palette ..etc .
*/
typedef enum {
    GPU_ARGB8888 = 0,       /**< ARGB8888 format */
    GPU_ARGB4444 = 1,       /**< ARGB4444 format */
    GPU_ARGB1555 = 2,       /**< ARGB1555 Pallette format */
    GPU_8BPP_PAL = 3,       /**< 8bit Pallette format */
} GPU_FORMAT_T;

/**
 * pixel depth.
 * 8bpp, 16bpp, 32bpp.
*/
typedef enum {
    GPU_8BPP    = 0,            /**< 8 bit per pixel */
    GPU_16BPP   = 1,            /**< 16 bit per pixel */
    GPU_32BPP   = 2,            /**< 32 bit per pixel */
} GPU_PIXELDEPTH_T;

//FIXME zuel_091021
/**
 * physical osd id.
*/
typedef enum {
    GOP_0 = 0,  /**< view osd 0(GOP1G,  MUX0)  */ //DVB : UI, ATSC : UI
    GOP_1 = 1,  /**< view osd 1(GOP4G,  MUX2)  */ //DVB : AddOn,MHEG, ATSC : AddOn,ACAP,Ginga
    GOP_2 = 2,  /**< view osd 2(GOP2G,  MUX1)  */ //DVB : Subtitle,TTX,EME, ATSC : ACC,DCC,EME
#if 1
    GOP_3 = 3,  /**< view osd 3(GOP1GX, MUX3)  */ //DVB : Cursor, ATSC : Cursor
#endif    
} GPU_ID_T;

/**
 * Temporary for MSstar.
*/
typedef enum {
    GPU_FRONT_BUFFER = -2,  /**< Temporary */
    GPU_BACK_BUFFER  = -1,  /**< Temporary */
} GPU_VOSD_TEMP_ID_T;

//FIXME zuel_091021
/**
 * virtual osd id.
*/
#if 0
typedef enum {
    GPU_VOSD_0      = 0,    /**< DVB : UI, ATSC : UI (S7)*/
    GPU_VOSD_1      = 1,    /**< DVB : MHEG, ATSC : ACAP (S7)*/
    GPU_VOSD_2      = 2,    /**< DVB : Subtitle, ATSC : ACC/DCC (S7)*/
    GPU_VOSD_3      = 3,    /**< DVB : Teletext/EME, ATSC : EME (S7) */
    GPU_VOSD_4      = 4,    /**< DVB : AddOn, ATSC : AddOn (S7)*/
#if 0    
    GPU_VOSD_5      = 5,    /**< DVB : Cursor, ATSC : Cursor (S7)*/
    GPU_VOSD_6      = 6,    /**< DVB : Ginga, ATSC : Ginga (S7) */    
	GPU_NUM_OF_VOSD = 7,    /**< Number Of VOSDs */
#endif	
	GPU_NUM_OF_VOSD = 5,    /**< Number Of VOSDs */
} GPU_VOSD_ID_T;
#endif
typedef enum {
    GPU_VOSD_0      = 0,    /**< DVB : UI, ATSC : UI (S8)*/
    GPU_VOSD_1      = 1,    /**< DVB : MHEG, ATSC : ACAP (S8)*/
    GPU_VOSD_2      = 2,    /**< DVB : Subtitle, ATSC : ACC/DCC (S8)*/
    GPU_VOSD_3      = 3,    /**< DVB : Teletext/EME, ATSC : EME (S8) */
    GPU_VOSD_4      = 4,    /**< DVB : Buffer for DirectFB and EMF Thumbnail : browser*/
	GPU_VOSD_5		= 5,	/**< DVB : Buffer for DirectFB (Second layer) : flash engine*/
    GPU_VOSD_6      = 6,    /**< DVB : Buffer for DirectFB (Third layer) : app store */
    GPU_VOSD_7      = 7,    /**< DVB : Buffer for DirectFB (Fourth layer) : netcast main (to improve speed of entry) */
    GPU_VOSD_8      = 8,    /** Motion control  Layer. */
	GPU_NUM_OF_VOSD = GPU_VOSD_8,    /**< Number Of VOSDs */
} GPU_VOSD_ID_T;

/**
 * Operation Type
 * GPU_OP_BLEND(alphaLevel) : Adst = Asrc*alphaLevel+(1-Asrc*alphaLevel)*Adst
 * GPU_OP_COPY(alphaLevel) : Adst = Asrc*alphaLevel
 */
typedef enum {
    GPU_OP_DEFAULT          = 0,        /**< Default */
    GPU_OP_XOR              = 1,        /**< XOR */
    GPU_OP_BLEND_ZERO       = 0x8000,   /**< Blend with Alpha Level = 0 */
    GPU_OP_BLEND_DEFAULT    = 0x80ff,   /**< Blend with Alpha Level = 0xff */
    GPU_OP_COPY_ZERO        = 0x8100,   /**< Copy with Alpha Level = 0 */
    GPU_OP_COPY_DEFAULT     = 0x81ff,   /**< Copy with Alpha Level = ff : equivalent with GPU_OP_DEFAULT */
} GPU_OP_TYPE_T;


/**
 * Layer Setting Information
*/
typedef struct
{
    UINT32                         surfacePoolSize;
    UINT16                         surfacePoolAlignment;
    UINT16                         numberOfLayers;
    UINT16                         useVendorSurfaceAllocator;
    GPU_LAYER_CONFIG_T             layerConfig[_MAX_NUM_OF_LAYERS];
} GPU_INIT_SETTINGS_T;

/**
 * Blend of Source Type
*/
typedef enum
{
    ORGSRC,
    DSTSRC
} BlendSource;

#if 0 //yducky
/******************************** LG2 Enum Start ******************************/
//This enumeration describes the supported pixel formats.
typedef enum
{
    GPU_PIXEL_FORMAT_ARGB = 0,
    GPU_PIXEL_FORMAT_LUT8 ,
    GPU_PIXEL_FORMAT_ARGB1555 ,
    GPU_PIXEL_FORMAT_RGB16 ,
    GPU_PIXEL_FORMAT_ARGB4444 ,
    GPU_PIXEL_FORMAT_A8,
    GPU_PIXEL_FORMAT_MAX,
} GPU_PIXEL_FORMAT_T;

//This enumeration describes the flags controlling blitting commands.
typedef enum {
    GPU_BLIT_NOFX                   = 0x00000000,
    GPU_BLIT_BLEND_ALPHACHANNEL     = 0x00000001,
    GPU_BLIT_BLEND_COLORALPHA       = 0x00000002,
    GPU_BLIT_BLEND_COLORIZE         = 0x00000004,
    GPU_BLIT_BLEND_SRC_COLORKEY     = 0x00000008,
    GPU_BLIT_BLEND_DST_COLORKEY     = 0x00000010,
    GPU_BLIT_BLEND_SRC_PREMULTIPLY  = 0x00000020,
    GPU_BLIT_BLEND_DST_PREMULTIPLY  = 0x00000040,
    GPU_BLIT_BLEND_DEMULTIPLY       = 0x00000080,
    GPU_BLIT_BLEND_DEINTERLACE      = 0x00000100,
    GPU_BLIT_BLEND_SRC_PREMULTCOLOR = 0x00000200,
    GPU_BLIT_XOR                    = 0x00000400,
    GPU_BLIT_INDEX_TRANSLATION      = 0x00000800,
    GPU_BLIT_ROTATE90               = 0x00001000,
    GPU_BLIT_ROTATE180              = 0x00002000,
    GPU_BLIT_ROTATE270              = 0x00004000,
    GPU_BLIT_COLORKEY_PROTECT       = 0x00010000,
    GPU_BLIT_SRC_MASK_ALPHA         = 0x00100000,
    GPU_BLIT_SRC_MASK_COLOR         = 0x00200000,
} GPU_BLIT_FLAGS_T;

typedef enum
{
	GPU_ACCEL_NONE 					= 0x00000000,
	GPU_ACCEL_FILLRECTANGLE         = 0x00000001,
	GPU_ACCEL_DRAWRECTANGLE         = 0x00000002,
	GPU_ACCEL_DRAWLINE 			    = 0x00000004,
	GPU_ACCEL_FILLTRIANGLE 	        = 0x00000008,
	GPU_ACCEL_BLIT 					= 0x00010000,
	GPU_ACCEL_STRETCHBLIT 	        = 0x00020000,
	GPU_ACCEL_TEXTRIANGLES 	        = 0x00040000,
	GPU_ACCEL_TRAPEZOIDBLIT         = 0x00080000,
	GPU_ACCEL_IMAGEDECODE 	        = 0x00100000,
	GPU_ACCEL_DRAWSTRING 		    = 0x01000000,
	GPU_ACCEL_ALL 					= 0x011F000F,
	GPU_ACCEL_ALL_DRAW 			    = 0x0000000F,
	GPU_ACCEL_ALL_BLIT 			    = 0x011F0000,
} GPU_SUPPORTED_FUNC_MASK_T;

//This enumeration describes flags controlling drawing commands.
typedef enum
{
    GPU_DRAW_NOFX                   = 0x00000000,
    GPU_DRAW_BLEND                  = 0x00000001,
    GPU_DRAW_DST_COLORKEY           = 0x00000002,
    GPU_DRAW_SRC_PREMULTIPLY        = 0x00000004,
    GPU_DRAW_DST_PREMULTIPLY        = 0x00000008,
    GPU_DRAW_DEMULTIPLY             = 0x00000010,
    GPU_DRAW_XOR                    = 0x00000020,
    GPU_DRAW_MAX,
} GPU_DRAWING_FLAGS_T;

//This enumeration describes the surface blend function s to use for source and destination blending.
typedef enum
{
    GPU_BLEND_ZERO                  = 1,
    GPU_BLEND_ONE                   = 2,
    GPU_BLEND_SRCOLOR               = 3,
    GPU_BLEND_INVSRCCOLOR           = 4,
    GPU_BLEND_SRCALPHA              = 5,
    GPU_BLEND_INVSRCALPHA           = 6,
    GPU_BLEND_DESTALPHA             = 7,
    GPU_BLEND_INVDESTALPHA          = 8,
    GPU_BLEND_DESTCOLOR             = 9,
    GPU_BLEND_INVDESTCOLOR          = 10,
    GPU_BLEND_SRCALPHASAT           = 11,
    GPU_BLEND_MAX,
} GPU_BLEND_FUNCTION_T;//GPU_DFB_SURFACE_BLEND_FUNCTION_T;

//This enumeration describes the layer region flags for layer setting.
typedef enum {
    GPU_LAYER_CONFIG_NONE           = 0x00000000,
    GPU_LAYER_CONFIG_WIDTH          = 0x00000001,
    GPU_LAYER_CONFIG_HEIGHT         = 0x00000002,
    GPU_LAYER_CONFIG_OPACITY        = 0x00001000,
    GPU_LAYER_CONFIG_ZORDER         = 0x00002000,
    GPU_LAYER_CONFIG_PROPERTY       = 0x00004000,
    GPU_LAYER_CONFIG_ALL            = 0xFFFFFFFF
} GPU_LAYER_CONFIG_FLAGS_T;

//This enumeration describes Flags controlling surface masks set.
typedef enum
{
    GPU_SURFACE_MASK_NONE           = 0x00000000,
    GPU_SURFACE_MASK_STENCIL        = 0x00000001,
    GPU_SURFACE_MASK_ALL            = 0x00000001,
} GPU_SURFACE_MASK_FLAGS_T;

typedef enum
{
    ORGSRC,
    DSTSRC
} BlendSource;



/******************************** LG2 Enum End   ******************************/

/***************************** Structure *********************************/

/******************************** LG2 Start ******************************/
typedef struct {
	BOOLEAN                     bAllocatorSupported;
	GPU_BLIT_FLAGS_T            blitFlags;
	GPU_DRAWING_FLAGS_T         drawFlags;
    GPU_SUPPORTED_FUNC_MASK_T   supportedFunctions;
	UINT8                       maxNumOfLayers;
    UINT32                      virtualAddrOfBase;
} GPU_DEVICE_CAPABILITY_INFO_T;

typedef struct
{
    UINT16 width;
    UINT16 height;
} GPU_SCREEN_RESOLUTION_T;

typedef struct
{
    UINT32 *pPalette;
    UINT32 length;
    //GPU_SUPPORTED_FUNC_MASK_T   supportedFunctions;
} GPU_PALETTE_INFO_T;

//This structure describes a rectangle specified by a point and a dimension.
typedef struct
{
    UINT16 x;
    UINT16 y;
    UINT16 w;
    UINT16 h;
} GPU_RECT_T;

typedef struct
{
    UINT32                      offset;
    UINT32                      pitch;
    UINT32                      bpp;
	UINT16                      width;
	UINT16                      height;
	GPU_PIXEL_FORMAT_T 	        pixelFormat;
	GPU_PALETTE_INFO_T 	        paletteInfo;
	UINT32                      property;
	SINT32                      id;
} GPU_SURFACE_INFO_T;

#if 0
typedef struct {
	BOOLEAN                     bUseDoubleBuffer;
	BOOLEAN                     bUseCreatedSurface;
	GPU_SURFACE_INFO_T 	        surfaceInfo[2];
} GPU_LAYER_CONFIG_T;
#else
typedef struct {
    BOOLEAN bUseDoubleBuffer;
	BOOLEAN bUseCreatedSurface;
	union
	{
        GPU_SURFACE_INFO_T createdSurfaceInfo[2];
        struct
        {
            UINT16 width;
            UINT16 height;
            GPU_PIXEL_FORMAT_T pixelFormat;
        } creatingLayerInfo;
	}layerInfo;
} GPU_LAYER_CONFIG_T;
#endif

typedef struct
{
    UINT32                         physicalAddr;
    UINT32                         virtualAddr;
    UINT32                         startOffset;
    UINT32                         size;           //Arki
    UINT16                         byteOffsetAlign;
    UINT16                         bytePitchAlign;
} GPU_SURFACE_POOL_INFO_T;

typedef struct
{
    BOOLEAN                     bUseDoubleBuffer;
    UINT8                       doubleBufferIndex;
    GPU_SURFACE_INFO_T          surfaceInfo[2];
    UINT16                      opacity;
    GPU_RECT_T                  region;
    UINT8                       zOrder;
    UINT32                      property;
} GPU_LAYER_REGION_INFO_T;

typedef struct
{
    UINT32                         surfacePoolSize;
    UINT16                         surfacePoolAlignment;
    UINT16                         numberOfLayers;
    UINT16                         useVendorSurfaceAllocator;
    GPU_LAYER_CONFIG_T             layerConfig[_MAX_NUM_OF_LAYERS];
} GPU_INIT_SETTINGS_T;

typedef struct _GRAPHIC_DC
{
    UINT8  u8FbID;
    UINT8  u8ConstantAlpha; //0xFF for disable alpha...
} GRAPHIC_DC;

//This structure describes settings for draw operation.
typedef struct
{
    UINT32 offset;
    UINT16 x;
    UINT16 y;
    GPU_SURFACE_MASK_FLAGS_T flags;
} GPU_SOURCE_MASK_T;

//This structure describes a line specified by two points.
typedef struct
{
    UINT16 x1;
    UINT16 y1;
    UINT16 x2;
    UINT16 y2;
} GPU_LINE_T;

//This structure describes settings for draw operation.
typedef struct
{
    GPU_BLEND_FUNCTION_T srcBlend;
    GPU_BLEND_FUNCTION_T dstBlend;
    UINT32 dstColorkey;
} GPU_DRAW_SETTINGS_T;

//This structure describes settings for blit operation.
typedef struct
{
    GPU_BLEND_FUNCTION_T srcBlend;
    GPU_BLEND_FUNCTION_T dstBlend;
    UINT32 alpha;
    UINT32 srcColorkey;
    UINT32 dstColorkey;
    GPU_SOURCE_MASK_T srcMask;
} GPU_BLIT_SETTINGS_T;

//This structure describes settings for trapezoid blit operation.
typedef struct
{
    GPU_RECT_T      srcRect;
    UINT8           alphaConst;
    BOOLEAN         bAntiAliasing;
    UINT8           trapDir;
    UINT8           srcRectRotateDir;
    UINT8           dstTrapRotateDir;
    UINT16          dstTrapEdge0Pos;
    UINT16          dstTrapEdge0St;
    UINT16          dstTrapEdge0End;
    UINT16          dstTrapEdge1St;
    UINT16          dstTrapEdge1End;
    UINT16          dstTrapDistance;
} GPU_TRAPEZOID_T;

#if 0
//This structure describes display.
typedef struct
{
    U32 physicalAddr;
    U32 virtualAddr;
    U16 width;
    U16 height;
    U16 numberOfLayers;
} GPU_DISPLAY_INFO_T;
#endif
/******************************** LG2 End   ******************************/
#endif //yducky




/**
 * virtual osd info.
*/
typedef struct {
    UINT8             bEnable;    /**< virtual osd enable/disable */
    GPU_ID_T            osdId;      /**< physical osd id */

    UINT16              viewX;      /**< virtual osd scaling info */
    UINT16              viewY;      /**< virtual osd scaling info */
    UINT16              viewWidth;  /**< virtual osd scaling info */
    UINT16              viewHeight; /**< virtual osd scaling info */

    UINT16              dispX;      /**< info to display virtual osd */
    UINT16              dispY;      /**< info to display virtual osd */
    UINT16              dispWidth;  /**< info to display virtual osd */
    UINT16              dispHeight; /**< info to display virtual osd */

    GPU_PIXEL_FORMAT_T  format;     /**< osd format */
    GPU_PIXELDEPTH_T    pxlDepth;   /**< byte per pixel */
    UINT16              width;      /**< virtual osd width */
    UINT16              height;     /**< virtual osd height */
    UINT16              stride;     /**< virtual osd byte per line */
    UINT8               alphaValue; /**< Global Alpha Value(only when the OSD Base is ARGB1555) */
    UINT16              opacity;
    UINT8               zOrder;
    UINT32              property;
    void                *pAddr;     /**< virtual osd address */
} GPU_VOSD_INFO_T;

/**
 * physical osd info.
 */
typedef struct {
    UINT8             bEnable;            /**< osd enable/disable */
    GPU_PIXEL_FORMAT_T  format;             /**< osd format */
    GPU_PIXELDEPTH_T    pxlDepth;           /**< byte per pixel */
    UINT16                 width;              /**< physical osd width */
    UINT16                 height;             /**< physical osd height */
    UINT32                 nNumOfVirtualOsds;  /**< the number of virtual osd */
} GPU_INFO_T;

/**
 * BLIT frame
 */
typedef struct {
    UINT16                 x;          /**< x */
    UINT16                 y;          /**< y */
    UINT16                 width;      /**< width */
    UINT16                 height;     /**< height */
    void                *pAddr;     /**< Frame Base Addr */
    UINT16                 stride;     /**< available when pAddr!=NULL */
    GPU_PIXEL_FORMAT_T  format;     /**< available when pAddr!=NULL */
    GPU_PIXELDEPTH_T    pxlDepth;   /**< available when pAddr!=NULL */
    GPU_VOSD_ID_T       vosdId;     /**< available when pAddr==NULL */
} GPU_FRAME_T;

#if 0 //FIXME : we will use it(DDI implementation guide v2.7) later
/**
 * GPU display handler
 */
typedef void* GPU_DISPLAY_T;
#endif

/******************************************************************************
    Function Declaration
******************************************************************************/
extern void DDI_GPU_Lock(void);
extern void DDI_GPU_Unlock(void);

//extern DTV_STATUS_T DDI_GPU_GetOSDInfo(GPU_ID_T osdId, GPU_INFO_T *pGfxOsdInfo);
extern DTV_STATUS_T GPU_Enable(GPU_ID_T osdId);
extern DTV_STATUS_T GPU_Disable(GPU_ID_T osdId);

extern DTV_STATUS_T DDI_GPU_EnableVirtualOSD(GPU_VOSD_ID_T vosdId);
extern DTV_STATUS_T DDI_GPU_DisableVirtualOSD(GPU_VOSD_ID_T vosdId);
extern BOOLEAN      DDI_GPU_IsVirtualOSDEnabled(GPU_VOSD_ID_T vosdId);

//extern DTV_STATUS_T DDI_GPU_SetVirtualOSDPalette(GPU_VOSD_ID_T vosdId, UINT32 *pPalette);

extern DTV_STATUS_T DDI_GPU_WritePixel(GPU_VOSD_ID_T vosdId, UINT32 x, UINT32 y, UINT32 color);
extern DTV_STATUS_T DDI_GPU_ReadPixel(GPU_VOSD_ID_T vosdId, UINT32 x, UINT32 y, UINT32 *pColor);
extern DTV_STATUS_T DDI_GPU_Clear(GPU_VOSD_ID_T vosdId);

extern UINT16  GPU_GetPitchFromFmt(UINT8 u8Fmt, UINT16 width);
extern UINT16  GPU_GetPxlDepthFromFmt(UINT8 u8Fmt);
extern UINT8   GPU_GetBppFromFmt(UINT8 u8Fmt);
extern UINT16  GPU_GetAlignWidthFromFmt(UINT8 u8Fmt, UINT16 width);
extern UINT8   GPU_BufFmtMap(UINT8 u8Type , UINT8 u8Fmt);
extern void GPU_InitLayerConfig(GPU_INIT_SETTINGS_T *initSettings);
extern void GPU_SetVOSD(SINT8 LayerID);
extern void GPU_DumpSurfaceList(int dumpLevel);
/*************************************** GPU DDI Funcition Start *************************************************/
#if 0 //yducky
//extern DTV_STATUS_T DDI_GPU_Initialize(GPU_INIT_SETTINGS_T initSettings, GPU_DEVICE_INFO_T *pDeviceInfo);
extern DTV_STATUS_T DDI_GPU_GetDeviceCapability(GPU_DEVICE_CAPABILITY_INFO_T *pDeviceCapability);

extern DTV_STATUS_T DDI_GPU_Initialize(UINT32 width, UINT32 height, GPU_PIXEL_FORMAT_T pixelFormat, GPU_SCREEN_RESOLUTION_T *pScreeenResolution);

extern DTV_STATUS_T DDI_GPU_Finalize(void);

extern DTV_STATUS_T DDI_GPU_GetLayerRegionInfo(SINT32 layer, GPU_LAYER_REGION_INFO_T *pLayerRegionInfo);

extern DTV_STATUS_T DDI_GPU_SetLayerRegion(SINT32 layer, GPU_LAYER_CONFIG_FLAGS_T layerConfigFlags, GPU_RECT_T regionRect, UINT32 regionAlpha, UINT8 zOrder, UINT32 property);

extern DTV_STATUS_T DDI_GPU_FlipLayerRegion(SINT32 layer, SINT32 bufferOffset);

extern DTV_STATUS_T DDI_GPU_UpdateLayerRegion(SINT32 layer, GPU_RECT_T updateRect);

extern DTV_STATUS_T DDI_GPU_AllocSurface(UINT16 width, UINT16 height, GPU_PIXEL_FORMAT_T pixelFormat, GPU_SURFACE_INFO_T *pSurfaceInfo);

extern DTV_STATUS_T DDI_GPU_SetSurfacePalette(GPU_SURFACE_INFO_T surface, UINT32 palette[256], SINT32 paletteLegnth);

extern void DDI_GPU_DeallocSurface(GPU_SURFACE_INFO_T surface);

extern DTV_STATUS_T DDI_GPU_Blit(GPU_SURFACE_INFO_T srcSurface, GPU_RECT_T srcRect, GPU_SURFACE_INFO_T dstSurface, UINT16 dx, UINT16 dy,
                                        GPU_BLIT_FLAGS_T blitFlags, GPU_BLIT_SETTINGS_T blitSettings);

extern DTV_STATUS_T DDI_GPU_StretchBlit(GPU_SURFACE_INFO_T srcSurface, GPU_RECT_T srcRect, GPU_SURFACE_INFO_T dstSurface,
                            GPU_RECT_T destRect, GPU_BLIT_FLAGS_T blitFlags, GPU_BLIT_SETTINGS_T blitSettings);

extern DTV_STATUS_T DDI_GPU_TrapezoidBlit(GPU_SURFACE_INFO_T srcSurface, GPU_SURFACE_INFO_T dstSurface,
                        GPU_TRAPEZOID_T trapezoidSet, GPU_BLIT_FLAGS_T blitFlags, GPU_BLIT_SETTINGS_T blitSettings);

extern DTV_STATUS_T DDI_GPU_FillRectangle(GPU_SURFACE_INFO_T dstSurface, GPU_RECT_T destRect, UINT32 color,
                                        GPU_DRAW_FLAGS_T drawingFlags, GPU_DRAW_SETTINGS_T drawSettings);

extern DTV_STATUS_T DDI_GPU_DrawRectangle(GPU_SURFACE_INFO_T dstSurface, GPU_RECT_T destRect, UINT32 color,
                                    GPU_DRAW_FLAGS_T drawingFlags, GPU_DRAW_SETTINGS_T drawSettings);

extern DTV_STATUS_T DDI_GPU_DrawLine(GPU_SURFACE_INFO_T dstSurface, GPU_LINE_T destLine,
                                        UINT32 color, GPU_DRAW_FLAGS_T drawingFlags, GPU_BLEND_FUNCTION_T srcBlend,
                                        GPU_BLEND_FUNCTION_T dstBlend, UINT32 dstColorkey);
#endif //yducky
/*************************************** GPU DDI Funcition End *************************************************/

//extern DTV_STATUS_T DDI_GPU_Clear(GPU_VOSD_ID_T vosdId);
extern DTV_STATUS_T _GPU_UpdateScreen(GPU_VOSD_ID_T vosdId,
    UINT32 x, UINT32 y, UINT32 width, UINT32 height, BOOLEAN bImmediate);
DTV_STATUS_T DDI_GPU_FreezeScreen(void);

extern DTV_STATUS_T GPU_FreeFrame(GPU_FRAME_T *pFrame);


extern DTV_STATUS_T DDI_GPU_StartTrueMotionDemo(int step, int y, int imgWidth, int imgHeight, UINT8 *pImage);
extern DTV_STATUS_T DDI_GPU_StopTrueMotionDemo(void);

extern DTV_STATUS_T DDI_GPU_StartVCOMPattern(void);
extern DTV_STATUS_T DDI_GPU_StopVCOMPattern(void);

extern DTV_STATUS_T DDI_GPU_SyncGraphic(void);

#ifdef INCLUDE_REMOTESVC
DTV_STATUS_T DDI_GPU_CaptureVideo(GPU_SURFACE_INFO_T surface, BOOLEAN bDigitalSrc);
#endif

#if 0 //FIXME : we will use it(DDI implementation guide v2.7) later
extern DTV_STATUS_T DDI_GPU_GetOSDInfo(SINT32 osdId, GPU_OSD_INFO_T *pOsdInfo);
extern DTV_STATUS_T DDI_GPU_SetOSD(SINT32 osdId, GPU_OSD_CONFIG_FLAGS_T osdFlags, GPU_OSD_INFO_T osdInfo);
extern DTV_STATUS_T DDI_GPU_UpdateSurfaceFromOffset(UINT32 offset, SINT32 sock, SINT32 cpid);
extern DTV_STATUS_T DDI_GPU_DeallocSurfacesFromSocket (SINT32 sock);
#endif

#ifdef __cplusplus
}
#endif

#endif /*_GPU_DDI_H_ */



