/**
 * Copyright 2002-5, 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Definition of ICE colours and colour types.
 *
 */

#ifndef CABOT_ICE_ICE_OSD_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_OSD_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"
#include "ice/ice_display_types.h"

/**
 * @brief Representation of an RGB colour with transparency.
 *
 * @note Valid values for each channel are from 0 to 255 i.e. 8 bit colour
 *       depth.
 */
typedef struct ICE_Colour
{
    /**
     * The opacity component of the RGB colour.
     * Valid values are from 0 (fully transparent) to 255 (fully opaque).
     */
    frost_uint8 ICE_alpha;

    /**
     * The red component of the RGB colour.
     */
    frost_uint8 ICE_red;

    /**
     * The green component of the RGB colour.
     */
    frost_uint8 ICE_green;

    /**
     * The blue component of the RGB colour.
     */
    frost_uint8 ICE_blue;

} ICE_Colour;


/**
 * @brief The colour model for the OSD plane.
 */
typedef enum ICE_OsdColourModel
{
    /**
     * 8-bit CLUT colour model. Colours are frost_uint8 values that are 
     * indexes to a 256-entry palette.
     */
    ICE_OsdColourModel8BitCLUT,

    /**
     * 16-bit ARGB colour model.  Colours are frost_uint16 true-colour values.
     * The colour values are created by taking 4-bit red, green, blue, and
     * alpha values (alpha 0 is transparent, alpha 15 is opaque), and
     * calculating <code>((alpha<<12)|(red<<8)|(green<<4)|blue)</code>.
     */
    ICE_OsdColourModelARGB4444,

    /**
     * 32-bit ARGB colour model.  Colours are frost_uint32 true-colour values.
     * The colour values are created by taking 8-bit red, green, blue, and
     * alpha values (alpha 0 is transparent, alpha 255 is opaque), and
     * calculating <code>((alpha<<24)|(red<<16|(green<<8)|blue)</code>.
     */
    ICE_OsdColourModelARGB8888
} ICE_OsdColourModel;

/**
 * @brief Supported Screen resolutions for the OSD plane.
 */
typedef enum ICE_OsdScreenResolution
{
    /**
     * Standard TV PAL Screen Resolution. 720 by 576 pixels.
     */
    ICE_Osd_SDTV_PAL_720_by_576,

    /**
     * High-Definition TV 1080p/i /2 Screen Resolution. 960 by 540 pixels.
     */
    ICE_Osd_HDTV_960_by_540,
    
    /**
     * High-Definition TV 720p Screen Resolution. 1280 by 720 pixels.
     */
    ICE_Osd_HDTV_1280_by_720,

    /**
     * High-Definition TV 1080p/i Screen Resolution. 1920 by 1080 pixels.
     */
    ICE_Osd_HDTV_1920_by_1080

} ICE_OsdScreenResolution;

#if (defined ENABLE_3D)
/**
 * @brief Indicates the plane to be used for the Cabot UI Framework.
 * This is three type of the 3D OSD used in the ICE_3DOsdModel.
 * This OSD plane is used for 3D.  
 */
typedef enum ICE_3DOsdModel
{
    ICE_3DOsdTB,
    ICE_3DOsdLR,
    ICE_3DOsdOff
} ICE_3DOsdModel;
#endif

/**
 * @brief Indicates the plane to be used for the Cabot UI Framework.
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 *
 * This OSD plane is used for menus, dialogs etc.  It may be any colour depth
 * and resolution.
 */
#define ICE_OSD_PURPOSE_UI        0x01u

/**
 * @brief Indicates the plane to be used for Cabot MHEG.
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 */
#define ICE_OSD_PURPOSE_MHEG      0x02u

/**
 * @brief Indicates the plane to be used for Cabot Teletext.
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 */
#define ICE_OSD_PURPOSE_TELETEXT  0x04u

/**
 * @brief Indicates the plane to be used for Cabot DVB Subtitles.
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 */
#define ICE_OSD_PURPOSE_SUBTITLES 0x08u

/**
 * @brief Indicates the plane to be used for displaying images in the Media
 *        Browser.
 *
 * This is the plane used by AnyImageInterface in the EASI API (see
 * eclipse/easi/AnyImageInterface.hpp).
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 */
#define ICE_OSD_PURPOSE_IMAGE     0x10u

/**
 * @brief Indicates the plane to be used by web engine.
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 */
#define ICE_OSD_PURPOSE_HBBTV     0x20u

/**
 * @brief Indicates the plane to be used for MHP engine for SD graphics.
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 */
#define ICE_OSD_PURPOSE_MHP_SD       0x40u

/**
 * @brief Indicates the plane to be used for MHP engine for HD graphics.
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 */
#define ICE_OSD_PURPOSE_MHP_HD       0x80u

/**
 * @brief Indicates the plane to be used for MHP engine to display background image.
 *
 * This is one of the flags used in the ICE_OsdPlaneCapability::purpose bitfield.
 */
#define ICE_OSD_PURPOSE_MHP_BG_IMAGE       0x100u


/**
 * @brief Indicates that a single (group of) OSD planes is to be used for
 *        everything.
 *
 * @note Eclipse will reject purposes that are not specified here.
 * @note If new purposes are added to this file, make sure to update this
 *       define.
 */
#define ICE_OSD_PURPOSE_ALL (ICE_OSD_PURPOSE_UI | \
                             ICE_OSD_PURPOSE_MHEG | \
                             ICE_OSD_PURPOSE_TELETEXT | \
                             ICE_OSD_PURPOSE_SUBTITLES | \
                             ICE_OSD_PURPOSE_IMAGE | \
                             ICE_OSD_PURPOSE_HBBTV | \
                             ICE_OSD_PURPOSE_MHP_HD | \
                             ICE_OSD_PURPOSE_MHP_SD)

/**
 * @brief Representation of a set of OSD plane attributes.
 */
typedef struct ICE_OsdPlaneCapability
{
    /**
     * The number of (identical) planes described by this structure.
     */
    frost_uint num_planes;

    /**
     * The colour model for the OSD plane(s).
     */
    ICE_OsdColourModel colour_model; 

    /**
     * The width of the OSD plane(s), in pixels.
     */
    frost_uint width;

    /**
     * The height of the OSD plane(s), in pixels.
     */
    frost_uint height;

    /**
     * The things that this OSD plane(s) should be used for.
     * This value is specified by ORing together the
     * ICE_OSD_PURPOSE_UI, ICE_OSD_PURPOSE_MHEG etc constants.
     *
     * If there are multiple planes in this group then Eclipse will allocate
     * them dynamically at runtime.  E.g. if you set #num_planes to 2 and
     * #purpose to ICE_OSD_PURPOSE_ALL, then Eclipse will allow the planes
     * to be dynamically reused for MHEG-over-Subtitles, UI-over-subtitles,
     * and UI-over-MHEG.
     */
    frost_uint purpose;

} ICE_OsdPlaneCapability;

/**
 * @brief Representation of planes supported by an OSD device.
 */
typedef struct ICE_OsdCapability
{
    /**
     * The number of entries in the #plane_capabilities array.
     */
    frost_uint num_plane_capabilities;

    /**
     * Array of plane capability definitions.
     *
     * Contains #num_plane_capabilities entries.
     *
     * The first entry in this array represents the front group of OSD
     * planes.  Entries are in Z-order.  The last entry in this array
     * represents the back group of OSD planes.
     *
     * Each plane has a plane_position value that is used in the call to
     * ICE_OsdPlaneOpen().  This is also in Z order, with 0 being the front
     * OSD plane.
     *
     * The first (front) group of OSD planes has plane_position values in the
     * range 0 to (plane_capabilities[0].num_planes - 1), inclusive.
     *
     * The second group of OSD planes has plane_position values in the
     * range (plane_capabilities[0].num_planes)
     * to (plane_capabilities[0].num_planes + plane_capabilities[1].num_planes - 1),
     * inclusive.
     *
     * Each group of planes has a purpose value.  Different groups cannot
     * have the same purpose flags.  E.g. if you have one 8-bit plane and
     * one 32-bit plane, you cannot set the ICE_OSD_PURPOSE_MHEG flag in
     * both ICE_OsdPlaneCapability structures; instead you must decide in
     * advance whether you want MHEG on an 8-bit or 32-bit plane and set
     * the ICE_OsdPlaneCapability::purpose fields appropriately.
     */
    ICE_OsdPlaneCapability const * plane_capabilities;

} ICE_OsdCapability;

#if 1 //Vestel's mods to ice_osd interfce - may conflict with Cabot ice_osd
/**
 * @brief The rules to be used when blitting/drawing.
 */
typedef enum ICE_OsdBlitStrategy
{
    /**
     * No rule. Overwrite the destination pixel with the source pixel.
     */
    ICE_Osd_Blit_Strategy_None,

    /**
     * Use alpha component to determine the destination pixel colour.
     * Source over rule is applied when this option is given.
     *
     * SRC_OVER rule: Cd = Cs + Cd*(1-As)
     *                Ad = As + Ad*(1-As)
     *
     * where Cd: Destination colour
     *       Cs: Source colour
     *       Ad: Destination alpha
     *       As: Source alpha
     */
    ICE_Osd_Blit_Strategy_Blend,

} ICE_OsdBlitStrategy;
#endif

/**
 * @brief Rectangle
 */
typedef struct ICE_OsdRect
{
    frost_int32 x; /*!< x coordinate of top-left corner */
    frost_int32 y; /*!< Y coordinate of top-left corner */
    frost_uint32 width; /*!< width of the rectangle in pixels */
    frost_uint32 height; /*!< height of the rectangle in pixels */
} ICE_OsdRect;

/**
 * @brief Surface properties
 *
 * @todo The 'palette' field is going to be removed.
 */
typedef struct ICE_OsdSurfaceProperties
{
    frost_uint32 width; /*!< Width of the surface in pixels */
    frost_uint32 height; /*!< Height of the surface in pixels */
    ICE_OsdColourModel colour_model;  /*!< The colour model of the surface */
} ICE_OsdSurfaceProperties;

/**
 *@brief Surface memory description
 */
typedef struct ICE_OsdSurfaceMemory
{
    void * pixels; /*!< Pointer to surface's pixel buffer. */
    frost_uint32 pitch; /*!< Row size in bytes. */
    ICE_OsdSurfaceProperties properties; /*!< Properties of the surface. */
} ICE_OsdSurfaceMemory;

/**
 * @brief Bit flags for ICE_OsdSurfaceBlit function.
 */
typedef enum ICE_OsdBlitFlag
{
    ICE_OsdBlitCopy     = 0x01, /*!<  perform simple memcpy-like blit */
    ICE_OsdBlitConvert  = 0x02, /*!<  perform colour space conversion */
    ICE_OsdBlitBlend    = 0x04, /*!<  perform alpha-blending */
    ICE_OsdBlitStretch  = 0x08, /*!<  stretch the source surface*/
    ICE_OsdBlitRotate   = 0x10  /*!<  rotate the source surface */
} ICE_OsdBlitFlag;

/**
 * @brief Roration angles for blit function.
 */
typedef enum ICE_OsdRotation
{
    ICE_OsdRotate0 = 0,                    /*!< do not rotate */
    ICE_OsdRotate90CW,                     /*!< rotate by 90 degees clock-wise */
    ICE_OsdRotate180,                      /*!< rotate by 180 degees */
    ICE_OsdRotate270CW,                    /*!< rotate by 270 degees clock-wise */
    ICE_OsdRotate90CCW = ICE_OsdRotate270CW/*!< synonym for #ICE_OsdRotate270CW */
} ICE_OsdRotation;

#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_OSD_TYPES_H_INCLUDED*/
