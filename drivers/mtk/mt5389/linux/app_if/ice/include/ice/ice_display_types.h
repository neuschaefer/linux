/**
 * Copyright 2007 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief  Definition of display data types that are common to OSD and video.
 *
 */

#ifndef CABOT_ICE_ICE_DISPLAY_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_DISPLAY_TYPES_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "frost/frost_basictypes.h"

/**
 * @brief Representation of a window onto an area of video, I-frame or OSD.
 *
 * The coordinates are in pixels, relative to the top-left of the screen.
 */
typedef struct ICE_DisplayWindow
{
    /**
     * The x-coordinate of the leftmost edge of the window.
     */
    frost_int x_position;

    /**
     * The y-coordinate of the topmost edge of the window.
     */
    frost_int y_position;

    /**
     * The width of the window in pixels.
     */
    frost_uint width;

    /**
     * The height of the window in pixels.
     */
    frost_uint height;

} ICE_DisplayWindow;

/**
*@ brief Structure of VideoWall that includes row , coloumn, cell  counts and offset value.
*/
typedef struct ICE_VideoWall
{
    frost_uint row_count;
    frost_uint column_count;
    frost_uint cell;
    frost_uint offset;
}ICE_VideoWall;

#ifdef __cplusplus
} /* end of extern "C" block */
#endif

#endif /* CABOT_ICE_ICE_DISPLAY_TYPES_H_INCLUDED */

