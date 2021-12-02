/**
 * @file    ddi_gpu_data.h
 * @brief   type definitions for the DDI_GPU APIs 
 * @author  Ku Bong Min
 * @date    Wed Jan 16 16:27:21 KST 2008
 *
 *  Header file which defines various structure based types for the DDI_GPU APIs.
 *  $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/app_if/ddi/ddi_inc/include/ddi_gpu_data.h#1 $\n
 *  $HeadURL: http://sourceforge.lge.com/svn/rpc4dtv/trunk/rpc_ddi_gpu/ddi_gpu_data.h $\n
 */

#ifndef __DDI_GPU_DATA_H__
#define __DDI_GPU_DATA_H__
#ifdef __cplusplus
extern "C" {
#endif

#define GPU_MAX_NUMBER_OF_LAYER 4

#ifndef UINT8 // If not defined, it's stand-alone client.
#ifndef UINT8
typedef	unsigned char			__UINT8;
#define UINT8 __UINT8
#endif

#ifndef SINT8
typedef	signed char				__SINT8;
#define SINT8 __SINT8
#endif

#ifndef CHAR
typedef	char					__CHAR;
#define CHAR __CHAR
#endif

#ifndef UINT16
typedef	unsigned short			__UINT16;
#define UINT16 __UINT16
#endif

#ifndef SINT16
typedef	signed short			__SINT16;
#define SINT16 __SINT16
#endif

#ifndef UINT32
typedef	unsigned long			__UINT32;
#define UINT32 __UINT32
#endif

#ifndef SINT32
typedef signed long				__SINT32;
#define SINT32 __SINT32
#endif

#ifndef BOOLEAN
#ifndef _EMUL_WIN
typedef	unsigned long			__BOOLEAN;
#define BOOLEAN __BOOLEAN
#else
typedef	unsigned char		__BOOLEAN;
#define BOOLEAN __BOOLEAN
#endif
#endif

#ifndef UINT64
typedef	unsigned long long		__UINT64;
#define UINT64 __UINT64
#endif

#ifndef SINT64
typedef	signed long long		__SINT64;
#define SINT64 __SINT64
#endif

/**
 * @brief This enumeration describes the API return type. 
 */
typedef enum 
{
	OK					= 0, 	/**< success (no error) */
	NOT_OK				= -1,	/**< generic error */
	INVALID_PARAMS		= -2, 	/**< input parameter error */
	NOT_ENOUGH_RESOURCE	= -3,	/**< not enough resource */
	NOT_SUPPORTED		= -4,	/**< not supported */
	NOT_PERMITTED		= -5,	/**< not permitted */
	TIMEOUT				= -6,	/**< timeout */
	NO_DATA_RECEIVED	= -7,	/**< no data received */
	DN_BUF_OVERFLOW 	= -8	/**< buffer overflow error */
} DTV_STATUS_T;
#endif


/**
 * @brief This enumeration describes the supported functions.
 */
typedef enum {
	GPU_ACCEL_NONE				= 0x00000000,	/**< none of these */
	GPU_ACCEL_FILLRECTANGLE 	= 0x00000001,	/**< DDI_GPU_FillRectangle is accelerated */
	GPU_ACCEL_DRAWRECTANGLE 	= 0x00000002,	/**< DDI_GPU_DrawRectangle is accelerated */
	GPU_ACCEL_DRAWLINE			= 0x00000004,	/**< DDI_GPU_DrawLine is accelerated */
	GPU_ACCEL_FILLTRIANGLE		= 0x00000008,	/**< reserved */
	GPU_ACCEL_BLIT				= 0x00010000,	/**< DDI_GPU_Blit is accelerated */
	GPU_ACCEL_STRETCHBLIT		= 0x00020000,	/**< DDI_GPU_StretchBlit is accelerated */
	GPU_ACCEL_TEXTRIANGLES		= 0x00040000,	/**< reserved */
	GPU_ACCEL_TRAPEZOIDBLIT 	= 0x00080000,	/**< DDI_GPU_TrapezoidBlit is accelerated */
	GPU_ACCEL_DECODEIMAGE		= 0x00100000,	/**< DDI_GPU_DecodeImage is accelerated */
	GPU_ACCEL_DRAWSTRING		= 0x01000000,	/**< reserved */
	GPU_ACCEL_FLIP				= 0x02000000, 	/**< Flip operation is supported */
	GPU_ACCEL_ALL				= 0x031F000F,  	/**< All functions */
	GPU_ACCEL_ALL_DRAW			= 0x0000000F,	/**< All draw functions */
	GPU_ACCEL_ALL_BLIT			= 0x011F0000,	/**< All blit functions */
} GPU_SUPPORTED_FUNC_MASK_T;

//FIXME : In the furture, It should be intergrated into one
/** 
 * @brief This structure describes surface pool.
 */
typedef struct {
	UINT32	physicalAddr;		/**< physical address of surface pool base */
	UINT32	virtualAddr;		/**< virtual address of surface pool base */
	UINT32	startOffset;		/**< the end of allocated layers (surface pool start with zero offset.) */
	UINT32	size;				/**< size of surface pool (in bytes). It is not from startOffset but zero offset which means base point of surface pool. */
	UINT16	byteOffsetAlign;	/**< bytes alignment constraint for the offset value of each surface allocation */
	UINT16	bytePitchAlign;		/**< bytes alignment constraint for the surface's pitch value */
} GPU_SURFACE_POOL_INFO_T;

/**
 * @brief This enumeration describes the supported pixel formats. 
 */
typedef enum 
{
	GPU_PIXEL_FORMAT_ARGB = 0,	/**< 32 bit ARGB (4 byte, alpha 8\@24, red 8\@16, green 8\@8, blue 8\@0) */
	GPU_PIXEL_FORMAT_LUT8, 		/**< 8 bit LUT (8 bit color and alpha lookup from palette) */
	GPU_PIXEL_FORMAT_ARGB1555, 	/**< 16 bit ARGB (2 byte, alpha 1\@15, red 5\@10, green 5\@5, blue 5\@0) */
	GPU_PIXEL_FORMAT_RGB16, 	/**< 16 bit RGB (2 byte, red 5\@11, green 6\@5, blue 5\@0) */
	GPU_PIXEL_FORMAT_ARGB4444, 	/**< 16 bit ARGB (2 byte, alpha 4\@12, red 4\@8, green 4\@4, blue 4\@0) */
	GPU_PIXEL_FORMAT_A8, 		/**< 8 bit A (1 byte, alpha 8\@0) */
	GPU_PIXEL_FORMAT_MAX		/**< Maximum number of GPU_PIXEL_FORMAT_T */
} GPU_PIXEL_FORMAT_T;


/**
 * @brief This enumeration describes a information about image format.
 */
typedef enum {
	GPU_IMAGE_FORMAT_JPEG		=0x0,
	GPU_IMAGE_FORMAT_PNG,
	GPU_IMAGE_FORMAT_GIF,
	GPU_IMAGE_FORMAT_BMP,
	GPU_IMAGE_FORMAT_BUFFER,
	GPU_IMAGE_FORMAT_MAX,
} GPU_IMAGE_FORMAT_T;


/**
 * @brief This structure describes a rectangle specified by a point and a dimension.
 */
typedef struct
{
	UINT16	x;	/**< x cordinate of its top-letf point */
	UINT16	y;	/**< y cordinate of its top-left point */
	UINT16	w;	/**< width of it */
	UINT16	h;	/**< height of it */
} GPU_RECT_T;


/**
 * @brief This structure describes a line specified by two points.
 */
typedef struct
{
	UINT16	x1;	/**< x cordinate of its top-letf point1 */
	UINT16	y1;	/**< y cordinate of its top-left point1 */
	UINT16	x2;	/**< x cordinate of its top-letf point2 */
	UINT16	y2;	/**< y cordinate of its top-left point2 */
} GPU_LINE_T;


/**
 * @brief This structure describes the palette information of the surface.
 */
typedef struct
{
	UINT32	*pPalette;	/**< point to the palette arry */
	UINT32	length;		/**< size of palette (max: 256) */
} GPU_PALETTE_INFO_T;

#if 0 //FIXME : we will use it(DDI implementation guide v2.7) later
/** 
 * @brief This structure describes a surface specified by video memory offset, pitch and bpp.
 */
typedef struct
{
	UINT32				offset;			/**< bytes offset from the start of video memory */
	UINT32				pitch;			/**< pitch: length of horizontal line */
	UINT32				bpp;			/**< bits per pixel */
	UINT16				width;			/**< width of surface */
	UINT16				height;			/**< height of surface */
	GPU_PIXEL_FORMAT_T	pixelFormat;	/**< pixel format of surface */
	GPU_PALETTE_INFO_T	paletteInfo;	/**< palette information, this field is used when the surface's pixel format is based on indexed color. */
	SINT32				id;				/**< surface identifier */
	UINT32				property;		/**< reserved for future use */
} GPU_SURFACE_INFO_T;
#else
/** 
 * @brief This structure describes a surface specified by video memory offset, pitch and bpp.
 */
typedef struct
{
	UINT32				offset;			/**< bytes offset from the start of video memory */
	UINT16				pitch;			/**< pitch: length of horizontal line */
	UINT16				bpp;			/**< bits per pixel */
	UINT16				width;			/**< width of surface */
	UINT16				height;			/**< height of surface */
	GPU_PIXEL_FORMAT_T	pixelFormat;	/**< pixel format of surface */
	GPU_PALETTE_INFO_T	paletteInfo;	/**< palette information, this field is used when the surface's pixel format is based on indexed color. */
	SINT32				id;				/**< surface identifier */
	UINT32				property;		/**< reserved for future use */
} GPU_SURFACE_INFO_T;
#endif

/**
 * @brief This structure describes screen resolution.
 */
typedef struct {
	UINT16				width;			/**< width of screen */
	UINT16				height; 		/**< height of screen */
} GPU_SCREEN_RESOLUTION_T;


/**
 * @brief This structure describes configuration of layer for initializing GPU device.
 */
typedef struct {
	UINT8 bUseDoubleBuffer;		/**< whether the double buffer should be allocated or not */
	UINT8 bUseCreatedSurface;		/**< whether use createdSurfaceInfo or not. If TRUE, then layer is registered by using createdSurfaceInfo[2]. If FALSE, then layer is created newly by using creatingLayerInfo and createdSurfaceInfo[2] is not used. */
	union
	{
		GPU_SURFACE_INFO_T	createdSurfaceInfo[2];	/**< information of created surface including backbuffer or frontbuffer */
		struct {
			UINT16				width;			/**< width of layer to be created */
			UINT16				height; 		/**< height of layer to be created */
			UINT32				property;		/**< property of layer to be created */
			GPU_PIXEL_FORMAT_T	pixelFormat;	/**< pixelFormat of layer to be created */
		} creatingLayerInfo; /**< information of layer to be created */
	} layerInfo;
} GPU_LAYER_CONFIG_T;


#if 0 //FIXME : we will use it(DDI implementation guide v2.7) later
/** 
 *
 *  * @brief This structure describes OSD information.
 *
 *   */
typedef struct {
 UINT8                 osdId;	              /** < ID of OSD. (This value is valid only for DDI_GPU_GetOSDInfo function.) */
 UINT8                 bUseDoubleBuffer;    /** < If this value is TRUE, then double-buffering is used. If zero, then single-buffering is used. (This value is valid only for DDI_GPU_GetOSDInfo function.) */
 UINT8                 doubleBufferIndex;   /** < Index number of buffer to draw. When double-buffering is used, this value indicates the index number of backbuffer, otherwise frontbuffer (framebuffer). (This value is valid only for DDI_GPU_GetOSDInfo function.) */
 GPU_SURFACE_INFO_T    surfaceInfo[2];      /** < surface information of frontbuffer and backbuffer. For example, if bUseDoubleBuffer is TRUE, you can access to the surface of backbuffer via surfaceInfo[doubleBufferIndex]. (This value is valid only for DDI_GPU_GetOSDInfo function.) */
 UINT16                opacity;             /** < opacity (zero means full transparency) */
 UINT16                width;               /** < width of allocated region */
 UINT16                height;              /** < height of allocated region */
 GPU_PIXEL_FORMAT_T    pixelFormat;         /** < pixelFormat of allocated region */
 GPU_RECT_T            srcRect;             /** < source region on the OSD to copy to the screen. */
 GPU_RECT_T            dstRect;             /** < destination region on the screen for OSD to be copied to. */
 UINT8 	               zOrder;              /** < z-order between layers, i.e., z axis position of a layer. Layer on the level zero is the bottom layer. */
 UINT32                property;            /** < reserved for future use */
} GPU_OSD_INFO_T;
#endif


/** 
 * @brief This structure describes layer.
 */
typedef struct
{
	UINT8				bUseDoubleBuffer;	/**< If this value is 1, then double-buffering is used. If zero, then single-buffering is used. */
	UINT8				doubleBufferIndex;	/**< Index number of buffer to draw. When double-buffering is used, this value indicates the index number of backbuffer otherwise, frontbuffer. */
	GPU_SURFACE_INFO_T	surfaceInfo[2];		/**< surface information of frontbuffer and backbuffer. For example, if bUseDoubleBuffer is TRUE, you can access to the surface of backbuffer via surfaceInfo[doubleBufferIndex]. */
	UINT16				opacity;			/**< opacity (zero means full transparency) */
	GPU_RECT_T			region;				/**< viewable region of the layer */
	UINT8				zOrder;				/**< z-order between layers, i.e., z axis position of a layer. Layer on the level zero is the bottom layer. */
	UINT32				property;			/**< reserved for future use */
} GPU_LAYER_REGION_INFO_T;


/**
 * @brief This enumeration describes the flags controlling blitting commands.
 */
typedef enum
{
	GPU_BLIT_NOFX					= 0x00000000, /**< uses none of the effects */
	GPU_BLIT_BLEND_ALPHACHANNEL		= 0x00000001, /**< enables blending and uses
													  alphachannel from source */
	GPU_BLIT_BLEND_COLORALPHA		= 0x00000002, /**< enables blending and uses
													  alpha value from color */
	GPU_BLIT_COLORIZE				= 0x00000004, /**< modulates source color with
													  the color's r/g/b values */
	GPU_BLIT_SRC_COLORKEY			= 0x00000008, /**< don't blit pixels matching the source color key */
	GPU_BLIT_DST_COLORKEY			= 0x00000010, /**< write to destination only if the destination pixel
													  matches the destination color key */
	GPU_BLIT_SRC_PREMULTIPLY		= 0x00000020, /**< modulates the source color with the (modulated)
													  source alpha */
	GPU_BLIT_DST_PREMULTIPLY		= 0x00000040, /**< modulates the dest. color with the dest. alpha */
	GPU_BLIT_DEMULTIPLY				= 0x00000080, /**< divides the color by the alpha before writing the
													  data to the destination */
	GPU_BLIT_DEINTERLACE			= 0x00000100, /**< deinterlaces the source during blitting by reading
													  only one field (every second line of full
													  image) scaling it vertically by factor two */
	GPU_BLIT_SRC_PREMULTCOLOR		= 0x00000200, /**< modulates the source color with the color alpha */
	GPU_BLIT_XOR					= 0x00000400, /**< bitwise xor the destination pixels with the
													 source pixels after premultiplication */
	GPU_BLIT_INDEX_TRANSLATION		= 0x00000800, /**< do fast indexed to indexed translation,
													 this flag is mutual exclusive with all others */
	GPU_BLIT_ROTATE90				= 0x00001000, /**< rotate the image by 90 degree clockwise */
	GPU_BLIT_ROTATE180				= 0x00002000, /**< rotate the image by 180 degree clockwise */
	GPU_BLIT_ROTATE270				= 0x00004000, /**< rotate the image by 270 degree clockwise */
	GPU_BLIT_COLORKEY_PROTECT		= 0x00010000, /**< make sure written pixels don't match color key (internal only ATM) */
	GPU_BLIT_SRC_MASK_ALPHA			= 0x00100000, /**< modulate source alpha channel with alpha channel from source mask */
	GPU_BLIT_SRC_MASK_COLOR			= 0x00200000, /**< modulate source color channels with color channels from source mask */

	GPU_BLIT_VERTICAL_MIRROR		= 0x00400000,  /**< flip vertically (x positon is not changed)*/
	GPU_BLIT_HORIZONTAL_MIRROR		= 0x00800000,  /**< flip horizontally (y positon is not changed)*/

	GPU_BLIT_BLEND_ALPHACHANNEL_BCM				= 0x01000000, /**< BCM specific mechanism similar to SRC_PREMULTCOLOR | BLEND_ALPHACHANNEL | DST_PREUMLTCOLOR */
	GPU_BLIT_BLEND_ALPHACHANNEL_COLORALPHA_BCM	= 0x02000000, /**< BCM specific mechanism similar to SRC_PREMULTCOLOR | BLEND_ALPHACHANNEL | BLEND_COLORALPHA | DST_PREUMLTCOLOR */
	GPU_BLIT_BLEND_ALPHACHANNEL_COLORIZE_BCM	= 0x04000000, /**< BCM specific mechanism similar to SRC_PREMULTCOLOR | BLEND_ALPHACHANNEL | COLORIZE | DST_PREUMLTCOLOR */

	GPU_BLIT_MAX  /**< the maximum value */

} GPU_BLIT_FLAGS_T;


/**
 * @brief This enumeration describes the flags controlling drawing commands.
 */
typedef enum
{
	GPU_DRAW_NOFX					= 0x00000000, /**< uses none of the effects */
	GPU_DRAW_BLEND					= 0x00000001, /**< uses alpha from color */
	GPU_DRAW_DST_COLORKEY			= 0x00000002, /**< write to destination only if the destination pixel matches the destination color key */
	GPU_DRAW_SRC_PREMULTIPLY		= 0x00000004, /**< muliplies the color's rgb channels by the alpha channel before drawing */
	GPU_DRAW_DST_PREMULTIPLY		= 0x00000008, /**< modulates the dest. color with the dest. alpha */
	GPU_DRAW_DEMULTIPLY				= 0x00000010, /**< divides the color by the alpha before writing the data to the destination */
	GPU_DRAW_XOR					= 0x00000020, /**< bitwise xor the destination pixels with the specified color after premultiplication */
	GPU_DRAW_MAX  /**< the maximum value */
} GPU_DRAW_FLAGS_T;


/**
 * @brief This enumeration describes the blend functions to use for source and destination blending.
 */
typedef enum
{
	GPU_BLEND_UNKNOWN			= 0,  /**< Uknown blending function */
	GPU_BLEND_ZERO				= 1,  /**< zero or, 0 */
	GPU_BLEND_ONE				= 2,  /**< 1 */
	GPU_BLEND_SRCCOLOR			= 3,  /**< source color */
	GPU_BLEND_INVSRCCOLOR		= 4,  /**< inverse source alpha */
	GPU_BLEND_SRCALPHA			= 5,  /**< source alpha */
	GPU_BLEND_INVSRCALPHA		= 6,  /**< inverse source alpha (1-Sa)*/
	GPU_BLEND_DESTALPHA			= 7,  /**< destination alpha */
	GPU_BLEND_INVDESTALPHA		= 8,  /**< inverse destination lapha (1-Da)*/
	GPU_BLEND_DESTCOLOR			= 9,  /**< destination color */
	GPU_BLEND_INVDESTCOLOR		= 10, /**< inverse destination color */
	GPU_BLEND_SRCALPHASAT		= 11, /**< source alpha saturation */
	GPU_BLEND_MAX  /**< the maximum value */
} GPU_BLEND_FUNCTION_T;


#if 0 //FIXME : we will use it(DDI implementation guide v2.7) later
/**
 * @brief This enumeration describes the OSD flags for OSD setting.
 */
typedef enum {
	GPU_OSD_CONFIG_NONE			= 0x00000000,	/**< none */
	GPU_OSD_CONFIG_WIDTH			= 0x00000001,	/**< width (in pixels) of viewable region */
	GPU_OSD_CONFIG_HEIGHT			= 0x00000002,	/**< height (in pixels) of viewable region */
	GPU_OSD_CONFIG_OPACITY			= 0x00001000,	/**< OSD opacity (0x00: invisible(OSD-off), 0x01~0xfe: transparent OSD, 0xff: fully opaque.) */
	GPU_OSD_CONFIG_ZORDER			= 0x00002000,	/**< z-order (or, OSD level) */
	GPU_OSD_CONFIG_PROPERTY			= 0x00004000,	/**< property */
	GPU_OSD_CONFIG_FORMAT			= 0x00008000,	/**< OSD pixel format */
	GPU_OSD_CONFIG_SRCRECT			= 0x00010000,	/**< Source region on the OSD to copy to the Screen. */
	GPU_OSD_CONFIG_DSTRECT			= 0x00020000,	/**< Destination region on the screen for OSD to be copied to. */
	GPU_OSD_CONFIG_ALL			= 0x0003F003	/**< all */
} GPU_OSD_CONFIG_FLAGS_T;
#endif

#if 0 //FIXME : we will use it(DDI implementation guide v2.7) later
/**
 * @brief This enumeration describes the layer region flags for layer setting.
 */
typedef enum 
{
	GPU_LAYER_CONFIG_NONE		= 0x00000000,	/**< none */
	GPU_LAYER_CONFIG_WIDTH		= 0x00000001,	/**< width (in pixels) of viewable region */
	GPU_LAYER_CONFIG_HEIGHT		= 0x00000002,	/**< height (in pixels) of viewable region */
	GPU_LAYER_CONFIG_OPACITY	= 0x00001000,	/**< layer opacity (0x00: invisible(layer-off), 0x01~0xfe: transparent layer, 0xff: fully opaque.) */
	GPU_LAYER_CONFIG_ZORDER		= 0x00002000,	/**< z-order (or layer level) of layer */
	GPU_LAYER_CONFIG_PROPERTY	= 0x00004000,	/**< property of layer */
	GPU_LAYER_CONFIG_FORMAT 	= 0x00008000,	/**< layer pixel format */
	GPU_LAYER_CONFIG_SRCRECT	= 0x00010000,	/**< Source region on the layer to copy to the OSD */
	GPU_LAYER_CONFIG_DSTRECT 	= 0x00020000,	/**< Destination region on the OSD for layer to be copied to. */
	GPU_LAYER_CONFIG_ALL		= 0x0000F003	/**< all */ 
} GPU_LAYER_CONFIG_FLAGS_T;
#else
/**
 * @brief This enumeration describes the layer region flags for layer setting.
 */
typedef enum 
{
	GPU_LAYER_CONFIG_NONE		= 0x00000000,	/**< none */
	GPU_LAYER_CONFIG_WIDTH		= 0x00000001,	/**< width (in pixels) of viewable region */
	GPU_LAYER_CONFIG_HEIGHT		= 0x00000002,	/**< height (in pixels) of viewable region */
	GPU_LAYER_CONFIG_OPACITY	= 0x00001000,	/**< layer opacity (0x00: invisible(layer-off), 0x01~0xfe: transparent layer, 0xff: fully opaque.) */
	GPU_LAYER_CONFIG_ZORDER		= 0x00002000,	/**< z-order (or layer level) of layer */
	GPU_LAYER_CONFIG_PROPERTY	= 0x00004000,	/**< property of layer */
	GPU_LAYER_CONFIG_FORMAT 	= 0x00008000,	/**< layer pixel format */
	GPU_LAYER_CONFIG_ALL		= 0x0000F003	/**< all */ 
} GPU_LAYER_CONFIG_FLAGS_T;
#endif

/**
 * @brief This enumeration describes Flags controlling surface masks set.
 */
typedef enum {
	GPU_SURFACE_MASK_NONE      = 0x00000000,  /**< none of these. */
	GPU_SURFACE_MASK_STENCIL   = 0x00000001,  /**< Take <b>x</b> and <b>y</b> as fixed start coordinates in the mask. */
	GPU_SURFACE_MASK_ALL       = 0x00000001,  /**< all of these. */
} GPU_SURFACE_MASK_FLAGS_T;

/**
 * @brief This enumeration describes supported image format for HW decoding.
 */
typedef enum {
	GPU_IMAGE_FORMAT_NONE_SUPPORTED		= 0x00000000,
	GPU_IMAGE_FORMAT_JPEG_SUPPORTED		= 0x00000001,
	GPU_IMAGE_FORMAT_PNG_SUPPORTED		= 0x00000002,
	GPU_IMAGE_FORMAT_GIF_SUPPORTED		= 0x00000004,
	GPU_IMAGE_FORMAT_BMP_SUPPORTED		= 0x00000008,
	GPU_IMAGE_FORMAT_BUFFER_SUPPORTED	= 0x00000010,
	GPU_IMAGE_FORMAT_ALL				= 0x0000001F,
} GPU_SUPPORTED_IMAGE_FORMAT_FLAGS_T;

#if 0 //FIXME : we will use it(DDI implementation guide v2.7) later
/**
 * @brief This structure describes the 2D graphic capability of device.
 */
typedef struct {
	UINT8						bAllocatorSupported;	/**< whether the allocator is supported, or not (if supported, then this value is TRUE otherwise FALSE.) */
	GPU_BLIT_FLAGS_T			blitFlags;				/**< supported blit flags */
	GPU_DRAW_FLAGS_T			drawFlags;				/**< supported draw flags */
	GPU_SUPPORTED_FUNC_MASK_T	supportedFunctions;		/**< supported functions */
	UINT8						maxNumOfLayers;			/**< the maximum number of supported layers */
	UINT8           maxNumOfOSDs;                   /**< The maximum number of supported OSDs */
	UINT32          physicalAddrOfBase;             /**< Physical address of videomemory start (or base). This value is generally used for memory mapping (mmap). */
	UINT32					virtualAddrOfBase;		/**< virtual address of videomemory start (or base of offset) This value is valid only for the process which initializes GPU module.*/
	UINT32          videoMemorySize;                /**< The total size of videomemory */
	UINT32          startOffsetOfSurfacePool;       /**< the offset of surface pool starting position. Surface pool means the videomemory block to allocate general surfaces. After initializing, OSDs with backbuffer is created. StartOffsetOfSurfacePool indicates the end of allocation of OSDs including backbuffer. LGE can use LGE??s own allocation algorithm to manage general surfaces with [videoMemorySize-startOffsetOfSurfacePool] size and startOffsetOfSurfacePool. When bUseVendorSurfaceAllocator is set to TRUE, this value is the same to zero and not used. */
	GPU_SUPPORTED_IMAGE_FORMAT_FLAGS_T		supportedImageFormat; /**< Image format that can be decoded by HW decoder */
	UINT32		hwDecodeMinLimitWidth;		/**< Minimum limitation of supported width for HW decoding (Zero means no limitation) */
	UINT32		hwDecodeMinLimitHeight;		/**< Minimum limitation of supported height for HW decoding (Zero means no limitation) */
	UINT32		hwDecodeMaxLimitWidth;		/**< Maximum limitation of supported width for HW decoding (Zero means no limitation) */
	UINT32		hwDecodeMaxLimitHeight;		/**< Maximum limitation of supported height for HW decoding (Zero means no limitation) */
	UINT32		hwDecodeMaxLimitArea;		/**< Maximum limitation of supported area (width*height) for HW decoding (Zero means no limitation). This value may mean the size of allocated surface. */
	UINT32		maxScaleUpFactor;		/**< maximum factor on scaling up (stretchblit).  */
	UINT32		minScaleDownFactor; 	/**< minimum factor on scaling down (stretchblit) */
	UINT32		swBlitArea;				/**< if width*height < swBlitArea then, SW blit operation may be faster. */
	UINT32		hwBlitLimitWidth;		/**< Limitation of supported width for HW blit (Zero means no limitation) */
	UINT32		hwBlitLimitHeight;		/**< Limitation of supported height for HW blit (Zero means no limitation) */
	UINT32		hwBlitLimitArea;		/**< Limitation of supported area (width*height) for HW blit (Zero means no limitation) */
} GPU_DEVICE_CAPABILITY_INFO_T;
#else
/**
 * @brief This structure describes the 2D graphic capability of device.
 */
typedef struct {
	UINT8						bAllocatorSupported;	/**< whether the allocator is supported, or not (if supported, then this value is TRUE otherwise FALSE.) */
	GPU_BLIT_FLAGS_T			blitFlags;				/**< supported blit flags */
	GPU_DRAW_FLAGS_T			drawFlags;				/**< supported draw flags */
	GPU_SUPPORTED_FUNC_MASK_T	supportedFunctions;		/**< supported functions */
	UINT8						maxNumOfLayers;			/**< the maximum number of supported layers */
	UINT32						virtualAddrOfBase;		/**< virtual address of videomemory start (or base of offset) */
	GPU_SUPPORTED_IMAGE_FORMAT_FLAGS_T		supportedImageFormat; /**< Image format that can be decoded by HW decoder */
	UINT32		hwDecodeMinLimitWidth;		/**< Minimum limitation of supported width for HW decoding (Zero means no limitation) */
	UINT32		hwDecodeMinLimitHeight;		/**< Minimum limitation of supported height for HW decoding (Zero means no limitation) */
	UINT16		hwDecodeMaxLimitWidth;		/**< Maximum limitation of supported width for HW decoding (Zero means no limitation) */
	UINT16		hwDecodeMaxLimitHeight;		/**< Maximum limitation of supported height for HW decoding (Zero means no limitation) */
	UINT32		hwDecodeMaxLimitArea;		/**< Maximum limitation of supported area (width*height) for HW decoding (Zero means no limitation). This value may mean the size of allocated surface. */
	UINT32		maxScaleUpFactor;		/**< maximum factor on scaling up (stretchblit).  */
	UINT32		minScaleDownFactor; 	/**< minimum factor on scaling down (stretchblit) */
	UINT32		swBlitArea;				/**< if width*height < swBlitArea then, SW blit operation may be faster. */
	UINT16		hwBlitLimitWidth;		/**< Limitation of supported width for HW blit (Zero means no limitation) */
	UINT16		hwBlitLimitHeight;		/**< Limitation of supported height for HW blit (Zero means no limitation) */
	UINT32		hwBlitLimitArea;		/**< Limitation of supported area (width*height) for HW blit (Zero means no limitation) */
} GPU_DEVICE_CAPABILITY_INFO_T;
#endif

/** 
 * @brief This structure describes source mask.
 */
typedef struct
{
	UINT32						offset;	/**< bytes offset from the start of video memory */
	UINT16						x;		/**< x */
	UINT16						y;		/**< y */
	GPU_SURFACE_MASK_FLAGS_T	flags;	/**< STENCIL or not*/
} GPU_SOURCE_MASK_T;


/** 
 * @brief This structure describes settings for draw operation.
 */
typedef struct
{
	GPU_BLEND_FUNCTION_T	srcBlend;		/**< source blend function */
	GPU_BLEND_FUNCTION_T	dstBlend;		/**< destination blend function */
	UINT32					dstColorkey;	/**< destination colorkey */
} GPU_DRAW_SETTINGS_T;


/** 
 * @brief This structure describes settings for blit operation.
 */
typedef struct
{
	GPU_BLEND_FUNCTION_T	srcBlend;		/**< source blend function */
	GPU_BLEND_FUNCTION_T	dstBlend;		/**< destination blend function */
	UINT32					alpha;			/**< global alpha value */
	UINT32					color;			/**< global color value */
	UINT32					srcColorkey;	/**< source colorkey */
	UINT32					dstColorkey;	/**< destination colorkey */
	GPU_SOURCE_MASK_T		srcMask;		/**< source mask information*/
} GPU_BLIT_SETTINGS_T;


/**
 * @brief This structure describes the trapezoid rendering.
 */
typedef struct
{
	GPU_RECT_T	srcRect;			/**< source rectangular which represents source image */
	UINT8		alphaConst;			/**< constant alpha blending (range: 0~255) */
	UINT8		bAntiAliasing;		/**< antialising option for trapezoid edge smoothing (0:off, 1:on) */
	UINT8		trapDir;			/**< Trapezoid direction mode (0: vertical trapezoid rendering mode, 1: horizontal rendering mode */
	UINT8		srcRectRotateDir;	/**< Source rectangular clockwise rotation (0: 0 degree, 1: 90, 2: 180 degree, 3: 270 degree) */
	UINT8		dstTrapRotateDir;	/**< Destination trapezoid clockwise rotation (0: 0 degree, 1: 90 degree, 2: 180 degree, 3: 270 degree) */
	UINT16		dstTrapEdge0Pos;	/**< y-coor for vertical trapezoid, x-coor for horizontal trapezoid */
	UINT16		dstTrapEdge0St;		/**< x-coor for vertical trapezoid, y-coor for horizontal trapezoid */
	UINT16		dstTrapEdge0End;	/**< x-coor for vertical trapezoid, y-coor for horizontal trapezoid */
	UINT16		dstTrapEdge1St;		/**< x-coor for vertical trapezoid, y-coor for horizontal trapezoid */
	UINT16		dstTrapEdge1End;	/**< x-coor for vertical trapezoid, y-coor for horizontal trapezoid */
	UINT16		dstTrapDistance;	/**< the distance between destination parallel edge0 and parallel edge1 */
} GPU_TRAPEZOID_T;


/**
 * @brief This structure describes information about image.
 */
typedef struct
{
	GPU_IMAGE_FORMAT_T 	imageFormat;
	GPU_RECT_T			rect;
	UINT32				imageLength;
	GPU_PIXEL_FORMAT_T	pixelFormat;
	UINT8				bImageFromBuffer;
	CHAR				*imagePath;
	UINT32				*imagePtr;
} GPU_IMAGE_INFO_T;

/**
 * @brief This enumeration describes the flags controlling decoded image.
 */
typedef enum
{
	GPU_DECODEIMAGE_NOFX               = 0x00000000, /**< uses none of the effects */
	GPU_DECODEIMAGE_PREMULTIPLY        = 0x00000001, /**< modulates the image color with the image alpha */
	GPU_DECODEIMAGE_ROTATE90           = 0x00000010, /**< rotate the image by 90 degree clockwise */
	GPU_DECODEIMAGE_ROTATE180          = 0x00000020, /**< rotate the image by 180 degree clockwise */
	GPU_DECODEIMAGE_ROTATE270          = 0x00000040, /**< rotate the image by 270 degree clockwise */
	GPU_DECODEIMAGE_VERTICAL_MIRROR    = 0x00000100, /**< flip vertically (x positon is not changed)*/
	GPU_DECODEIMAGE_HORIZONTAL_MIRROR  = 0x00000200, /**< flip horizontally (y positon is not changed)*/
	GPU_DECODEIMAGE_ALL                = 0x000003b1  /**< all */
} GPU_DECODEIMAGE_FLAGS_T;


/**
 * @brief This enumeration describes property of a surface (or layer).
 */
typedef enum {
	GPU_PROPERTY_NONE				= 0x00000000,
	GPU_PROPERTY_LAYER_FOR_DIRECTFB	= 0x00000001,
	GPU_PROPERTY_LAYER_FOR_UI		= 0x00000002,

	GPU_PROPERTY_ALL				= 0xffffffff
} GPU_PROPERTY_T;

#ifdef __cplusplus
}
#endif
#endif /* __DDI_GPU_DATA_H__ */
