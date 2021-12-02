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
 * $RCSfile: dsp_jpg.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_jpg.h
 *  Dsp jpg Image decode
 */
 
#ifndef	_DSP_JPG_H_
#define	_DSP_JPG_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "x_os.h"
#include "x_typedef.h"
#include "img_lib_if.h"
#include "jpg_if.h"


/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
#define REDUCE_COLOR_BUF
//#define CHECK_COLOR_BUF

#define Enhance_PJPEG
//#define FAST_ENHANCE_PJPEG_FOR_DSP
#define MEMORY_128BIT_ARRANGE

#define JPG_SOFTWARE_SPECIAL_LAYOUT
#ifdef JPG_SOFTWARE_SPECIAL_LAYOUT
#define JPG_SOFTWARE_SCALER_LAYOUT
//#define JPG_SOFTWARE_PSCAN_LAYOUT
#define JPG_SOFTWARE_OSD_LAYOUT
#endif

/********************************************************************
	TYPE DEFINITION
********************************************************************/
typedef struct {
  /* These values are fixed over the whole image. */
  /* For compression, they must be supplied by parameter setup; */
  /* for decompression, they are read from the SOF marker. */
  INT16 component_id;   /* identifier for this component (0..255) */
  INT16 component_index;    /* its index in SOF or cinfo->comp_info[] */
  INT16 h_samp_factor;    /* horizontal sampling factor (1..4) */
  INT16 v_samp_factor;    /* vertical sampling factor (1..4) */

} component_info;


typedef enum
{
    ORIGINAL_SIZE,    /* display image in original size */
    RESAMPLE_FIT,     /* Resample image to just fix the maximum display width or height */
    FIXED_SIZE        /* Resample image to fixed size image for digest view */
} resample_mode_enum;
typedef enum
{
    EIGHT_BY_EIGHT,   /* 8x8 IDCT */
    FOUR_BY_FOUR,     /* 4x4 IDCT */
    TWO_BY_TWO,       /* 2x2 IDCT */
    ONE_BY_ONE        /* 1x1 IDCT */
} IDCT_SIZE_ENUM;

typedef enum
{
    FIT_D0,   /* Scale down JPG file to just smaller than display buffer size */
    FIT_D1,   /* Find the just fit scale factor, we further scale down by one level,
               for example, from 8x8 to 4x4 */
    FIT_D2,   /* Find the just fit scale factor, we further scale down by two level,
               for example, from 8x8 to 2x2 */
    FIT_D3    /* Find the just fit scale factor, we further scale down by three level,
               for example, from 8x8 to 1x1 */
} SCALE_DOWN_LEVEL;

typedef enum
{
    JPG_NO_ERR,       /* no Error occur */
    JPG_CORRUPT_ERR, /* Corrupt Errors: Decoding action can resume but decoded image may be corrupted */
    JPG_FATAL_ERR,    /* Fatal Errors: Decoding action must be halted */
    JPG_UNSUPPORTED_FILE
} JPGErrorLevelEnum;

/* JPEG decode states */
typedef enum _JPG_STATE
{
    JPG_END,           /* has decoded JPEG file */
    JPG_START,        /* start decoding JPEG file */
    JPG_BITSTREAM_FETCHING,
    JPG_DECODING,     /* decoding JPEG file */
    //JPG_PRE_TRANSITION, /* Set transition effect */
    //JPG_TRANSITION,   /* perform JPEG file slide show effect */
    JPG_ERROR_HANDLING,
    //WAIT_DSP_DIGEST_START,
    JPG_PROG_RENDER,
} JPG_STATE;


/* JPEG decode states */
typedef enum _DSP_JPG_STATE
{
    DSP_JPG_EXIT,           /* has decoded JPEG file */
    DSP_JPG_INIT,        /* start decoding JPEG file */
    DSP_PARSING_HEADER,
    DSP_JPG_RESIZING,     /* decoding JPEG file */
    WAIT_DSP_DECODING,
    WAIT_PARSING_HEADER,
    DSP_HEADER_DONE,
    DSP_REPARSING,
    DSP_WAIT_FIFO_READY,
    DSP_JPG_TRANSITION,   /* perform JPEG file slide show effect */
    DSP_JPG_PAUSE,        /* pending decoding JPEG file */
    DSPJPG_ERROR_HANDLING,
    DSP_JPG_STOP,
    PARSING_EXIF_STATE
} DSP_JPG_STATE;

// - FOR JPEG PLAYER.
typedef struct _JPG_PLAYER
{
    // - mode and state control.
   // JPG_OPMODE    eOpMode;            // - operation mode
   JPG_STATE eState;
    DSP_JPG_STATE     eDspState;             // - decoding state.
    //JPG_DECMODE eDecMode;
} JPG_PLAYER;


enum
{
    BLOCK_420_MODE,
    RASTER_420_MODE,
    SCALER_422_MDOE,
    PSCAN_420_MODE,
    OSD_444_MODE
};

enum
{
	Y,
	Cb,
	Cr,
	ALL
};

typedef enum _SCALING_INPUT_MODE
{
    COLOR_BUFFER_FORMAT,         
    SLOGO_SCALER_FORMAT,                 
} SCALING_INPUT_MODE;

/* JPEG decode states */
#define JPG_ROTATION_TEMP_BUFFER_SIZE     0x3c000 //1920*16*4*2(driver rotation buffer), 240KB
#define JPG_WORKING_BUFFER_SIZE                 0x2800  //driver working buffer, e.g. sizeof(comp_info)*3....etc.     

// - TV related definition
#define JPG_BUFFER_NTSC      //set display buffer size to NTSC size, else PAL size

#define TV_SCREEN_WIDTH    3//4       //TV Screen Geometrical Aspect Ratio 4:3
#define TV_SCREEN_HEIGHT   2//3       //TV Screen Geometrical Aspect Ratio 4:3
#define PAL_H_FACTOR   (720/144)   //PAL MAX Horizontal Resolution 720 /(downscaling factor)
#define PAL_V_FACTOR   (576/144)   //PAL MAX Vertical Resolution 576 /(downscaling factor)
#define NTSC_H_FACTOR   (720/48)   //NTSC MAX Horizontal Resolution 720 /(downscaling factor)
#define NTSC_V_FACTOR   (480/48)   //NTSC MAX Vertical Resolution 480 /(downscaling factor)

#define PAL_H_RESOLUTION  720      //PAL MAX Horizontal Resolution 720
#define PAL_Y_RESOLUTION  576      //PAL MAX Vertical Resolution 576
#define NTSC_H_RESOLUTION  720      //NTSC MAX Horizontal Resolution 720
#define NTSC_Y_RESOLUTION  480      //NTSC MAX Vertical Resolution 480

//#define PANEL_WIDTH 1368
//#define PANEL_HEIGHT 768

#ifdef JPG_BUFFER_NTSC
#define TV_RESO_H_FACTOR  NTSC_H_FACTOR  //TV Resolution Aspect Ratio (Horizontal)
#define TV_RESO_Y_FACTOR  NTSC_V_FACTOR  //TV Resolution Aspect Ratio (Vertical)
#else
#define TV_RESO_H_FACTOR  PAL_H_FACTOR  //TV Resolution Aspect Ratio (Horizontal)
#define TV_RESO_Y_FACTOR  PAL_V_FACTOR  //TV Resolution Aspect Ratio (Vertical)
#endif

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
//(MWIF) img_if_lib.c
void fgJpeg_Play(void);
INT32 u4ImageDecode(void* hCurImg, UINT32 u4ImgID, ENUM_IMG_ROTATE_PHASE eRotateDegree);
void vGetDTVJpgInterface(JPEGHANDLE hFlow);
void vGetDTVJpgInterface_2(IMG_LIB_INFO_T *prImg, ENUM_IMG_ROTATE_PHASE eRotateDegree);
void vSetDTVJpgInterface(IMG_LIB_INFO_T *p);

void vRetDTVJpgInterface(void);

//dsp_uop.c
UINT32 dwGetDJpgAreaSa(void);
UINT32 dwGetDJpgAreaEa(void);

//dsp_irq.c
 BOOL fgIsDspJpg(void);
 BOOL bSetDspJpgHeaderInfo(UINT16 _image_width, UINT16 _image_height, BOOL fgProgressive, UINT16  uYformat, UINT16  uCformat );
 void vDspJpgStart(void );
 BOOL fgDspJpgHeadDone(void);
 void vCopyDspJpgColorBuf(UINT32 *_color_buf);
 void vSetFetchDspColorBufFlag(void);
 void vDspJpgErrStatus(void);
 void vGetProgressiveSos(UINT8 n, UINT8 cc);
 void vDspSetComID(UINT8 Com1, UINT8 Com2, UINT8 Com3);

#endif
