/*****************************************************************************
 * Copyright 2011 Broadcom Corporation.  All rights reserved.
 *
 * Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2, available at
 * http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
 *
 * Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a
 * license other than the GPL, without Broadcom's express prior written
 * consent.
 *****************************************************************************/

#ifndef _VC_FB_DEFS_H_
#define _VC_FB_DEFS_H_

/*
  FROM bcm-2835/2835-kernel-2.6.32/drivers/video/bcm2708_fb.c:

	fb is /dev/fb0. 
	Otherwise ovl<num> maps to /dev/fb<num>. 

	- 0 is the main grahics layer.
	- 1 and 2 are for the grid screen.
	- 3 is another smaller overlay that can be used while
	- games are running. 
	- 4 through 7 are all for cursors. 

  This is currently *NOT* correct on bcm1130, because fb0 is lcd and fb1 is
  hdmi. Case 561590 is open for Broadcom to fix ths, but in the interim we
  could set MAX_NUM_FB to 9 and then do renaming at boot time to work around
  the issue.  This sets MAX_NUM_FB to 9.  A change has to be made in startup
  scripts to handle the renaming.
*/

#define MAX_NUM_FB 8 //9 --agraham (Roku), we don't use LCD

#define VC_FB_VER       1
#define VC_FB_VER_MIN   0

typedef enum 
{
   FB_FORMAT_MIN,
   FB_FORMAT_RGB565,
   FB_FORMAT_RGB888,
   FB_FORMAT_RGBA32,
   FB_FORMAT_BGR565,
   FB_FORMAT_BGR888,
   FB_FORMAT_YUVNV12,
   FB_FORMAT_YUVNV21,
   FB_FORMAT_MAX
} VC_FB_FORMAT;

typedef enum 
{
   CSC_ITUR_BT_601_5 = 1,
   CSC_ITUR_BT_709_3,
   CSC_JPEG_JIF
} VC_FB_COLOR_SPACE;

// List of screens that are currently supported
typedef enum
{
   VC_FB_SCRN_LCD = -1, //0, --agraham (Roku), we don't use LCD
   VC_FB_SCRN_HDMI,
   VC_FB_SCRN_MAX
} VC_FB_SCRN;

// All message types supported for HOST->VC direction
typedef enum
{
   VC_FB_MSG_TYPE_MIN = 0,
   VC_FB_MSG_TYPE_GET_SCRN_INFO,   // Get screen information
   VC_FB_MSG_TYPE_ALLOC,               // Allocate framebuffer memory
   VC_FB_MSG_TYPE_FREE,                // Free framebuffer memory
   VC_FB_MSG_TYPE_PAN,                 // Pan framebuffer
   VC_FB_MSG_TYPE_SWAP,                // Swap active frames in framebuffer
   VC_FB_MSG_TYPE_CFG,                 // Configure framebuffer
   VC_FB_MSG_TYPE_MAX
} VC_FB_MSG_TYPE;

// Request for information for specified screen (HOST->VC)
typedef struct
{
   uint32_t scrn;    // Screen number (VC_FB_SCRN)
} VC_FB_GET_SCRN_INFO_T;

// Requested information for a screen (VC->HOST)
typedef struct
{
   uint32_t width;            // Screen width (pixels)
   uint32_t height;           // Screen height (pixels)
   uint32_t bits_per_pixel;   // Bits per pixel
} VC_FB_SCRN_INFO_T;

// Request to allocate memory (HOST->VC)
typedef struct
{
   uint32_t scrn;            // Screen number (VC_FB_SCRN)
   uint32_t width;           // Screen width (pixels)
   uint32_t height;          // Screen height (pixels)
   uint32_t bits_per_pixel;  // 16 or 32
   uint32_t num_frames;      // Number of frames, i.e., 2 for double buffering
   uint32_t layer;           // Layer which will be used in dispmanx
   uint32_t alpha_per_pixel; // 0=use default for frame, 1=use per-pixel alpha
   uint32_t default_alpha;   // Alpha value to use when alpha_per_pixel is 1.
   uint32_t scale;           // 0 = center on screen. 1 = scale framebuffer
   uint32_t pixel_fmt;
   uint32_t color_space;
} VC_FB_ALLOC_T;

// Result of a requested memory allocation (VC->HOST)
typedef struct
{
   uint32_t res_handle;    // Resource handle
   void    *res_mem;       // Pointer to resource buffer
   uint32_t line_bytes;    // Nbr of bytes to advance from one line to another
   uint32_t frame_bytes;   // Nbr of bytes to advance from one frame to another
} VC_FB_ALLOC_RESULT_T;

// Request to free a previously allocated memory (HOST->VC)
typedef struct
{
   uint32_t res_handle;    // Resource handle (returned from alloc)
} VC_FB_FREE_T;

// Request to pan a framebuffer (HOST->VC)
typedef struct
{
   uint32_t res_handle;    // Resource handle (returned from alloc)
   uint32_t src_x;
   uint32_t src_y;
   uint32_t src_w;
   uint32_t src_h;
   uint32_t dst_x;
   uint32_t dst_y;
   uint32_t dst_w;
   uint32_t dst_h;
   uint32_t opacity;
} VC_FB_PAN_T;

// Request to swap active frames in a framebuffer (HOST->VC)
typedef struct
{
   uint32_t res_handle;    // Resource handle (returned from alloc)
   uint32_t active_frame;  // Active frame to display (0-indexed)
} VC_FB_SWAP_T;

// Request to configure the framebuffer (HOST->VC)
typedef struct
{
   uint32_t res_handle;       // Resource handle (returned from alloc)
   uint32_t blank;
   uint32_t width;           // Screen width (pixels)
   uint32_t height;          // Screen height (pixels)
   uint32_t bits_per_pixel;   // Bits per pixel (leave as 0 for unchanged)
   uint32_t alpha_per_pixel;  // 0=alpha for whole frame, 1=use per-pixel alpha
   uint32_t default_alpha;    // Alpha value to use when alpha_per_pixel is 1
   uint32_t scale;           // 0 = center on screen. 1 = scale framebuffer
   uint32_t pixel_fmt;
   uint32_t color_space;   
} VC_FB_CFG_T;

// Result of a requested configuration of framebuffer (VC->HOST)
typedef struct
{
   uint32_t res_handle;    // resource handle on videocode
   uint32_t line_bytes;    // Nbr of bytes to advance from one line to another
   uint32_t frame_bytes;   // Nbr of bytes to advance from one frame to another
   uint16_t update_count;
} VC_FB_CFG_RESULT_T;

/*
 * Generic result for a request (VC->HOST). This is used from free, pan and swap
 * A type field is required for the callback to decide whether to call a driver
 * function that processes update_count's on ARM and vc. it is to be processed
 * only for pan and not for free and swap. Pan operation is asynchronous to the
 * user app to implement FBIO_WAITFORVSYNC
 */
typedef struct
{
   uint32_t res_handle; // resource handle on videocode
   uint16_t update_count;
} VC_FB_RESULT_T;

// Union of ALL messages
typedef union
{
   VC_FB_GET_SCRN_INFO_T get_scrn_info;   
   VC_FB_ALLOC_T         alloc;   
   VC_FB_FREE_T          free;
   VC_FB_PAN_T           pan;
   VC_FB_SWAP_T          swap;
   VC_FB_CFG_T           cfg;
} VC_FB_REQUEST_UNION_T;

typedef union
{
   VC_FB_SCRN_INFO_T     scrn_info;
   VC_FB_ALLOC_RESULT_T  alloc_result;
   VC_FB_CFG_RESULT_T    cfg_result;
   VC_FB_RESULT_T        result;
}VC_FB_MSG_REPLY_UINON_T;

typedef struct
{
   int32_t type;     // Message type (VC_FB_MSG_TYPE)
   int32_t success;
   VC_FB_MSG_REPLY_UINON_T u;  // Pointer to message body (if exists)
} VC_FB_MSG_REPLY_T;

typedef struct
{
   int32_t type;     // Message type (VC_FB_MSG_TYPE)
   VC_FB_REQUEST_UNION_T u;  // Pointer to message body (if exists)
} VC_FB_MSG_RQUEST_T;

#endif // _VC_FB_DEFS_H_
