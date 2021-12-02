/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_vp6.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp8webp.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_VP8_WEBP_H
#define VDEC_VP8_WEBP_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_debug.h"
LINT_EXT_HEADER_END
;
#define WEBP_DEC0 (0)
#define WEBP_DEC1 (1)

//#define CC_WEBP_EMULATION

#if defined(__MODEL_slt__) || defined(CC_WEBP_EMULATION)
    #define WEBP_ENABLE_CRC
#endif

#define WEBP_CLRFLG(flg,index) ((flg) &= ~(1<<(index)))
#define WEBP_SETFLG(flg,index) ((flg) |= (1<<(index)))
#define WEBP_FLGSET(flg,index) (((flg)>>(index))&1)

typedef enum
{
   WEBP_NOTIFY_TYPE_REQDATA,
   WEBP_NOTIFY_TYPE_ROWNOTIFY,
}WEBP_NOTIFY_TYPE;

typedef enum
{
  WEBP_DEC_FLAG_KEEPRATIO,
  WEBP_DEC_FLAG_MAX
}WEBP_DEC_FLAG;

typedef enum
{
  WEBP_DEC_MODE_MCUROW,
  WEBP_DEC_MODE_PIC,
  WEBP_DEC_MODE_AUTO
}WEBP_DEC_MODE;

typedef enum
{
  WEBP_COLOR_AYCBCR8888,
  WEBP_COLOR_ARGB8888,
  WEBP_COLOR_YCSEP420,
  WEBP_COLOR_YCSEP422
}WEBP_COLOR_MODE;

typedef struct
{
  UINT32 u4FileOffset;
  UINT32 u4Size;
  UINT32 u4WritePosition;
}WEBP_DATA_REQ_INFO;

typedef struct
{
  UINT32 u4Pitch;
  UINT32 u4Height;
  UINT32 u4YAddr;
  UINT32 u4CAddr;
}WEBP_RZROW_INFO;

typedef struct
{
	UINT32 u4DestBuffer;		// Target buffer
	UINT32 u4CbCrOffset;        // address offset for Y/CbCr seperate mode
	UINT32 u4Pitch;				// target buffer pitch
	UINT32 u4StartX;			//start point X
	UINT32 u4StartY;		    //start point Y
	UINT32 u4Width;				//region width
	UINT32 u4Height;            //region height
	UINT32 u4FileOffset;
	UINT32 u4FileSize;
	UINT32 u4Tag;
	UINT32 u4NotifyInfo;
	UINT32 u4Flag;
    WEBP_COLOR_MODE eColorMode;
#ifdef CC_WEBP_EMULATION
    UINT32 u4DecOutY;
    UINT32 u4DecOutC;
#endif
}WEBP_DECIN_PARAM_T;

typedef struct
{
	UINT32 u4CbCrOffset;        // address offset for Y/CbCr seperate mode
	UINT32 u4StartX;			//start point X
	UINT32 u4StartY;		    //start point Y
	UINT32 u4Width;				//region width
	UINT32 u4Height;            //region height
	UINT32 u4SrcWidht;
    UINT32 u4SrcHeight;
    WEBP_DEC_MODE eDecMode;
#ifdef WEBP_ENABLE_CRC
    UINT32 u4PPType;
	UINT32 u4Ycrc[4];
    UINT32 u4Ccrc[4];
    UINT32 u4RzCrc;
#endif
}WEBP_DECOUT_PARAM_T;

typedef UINT32 (*PFN_EWBP_EVENT_NOTIFY)(
	UINT32 u4Tag,
	VOID *pv_data,
	WEBP_NOTIFY_TYPE eType
);

extern BOOL _VDEC_Vp8WebPInit(UINT32 u4DecId,PFN_EWBP_EVENT_NOTIFY pfnCb,
	UINT32 u4BufStart,UINT32 u4BufSize);

extern BOOL _VDEC_Vp8WebPDecStart(UINT32 u4DecId,WEBP_DEC_MODE eDecMode,
    WEBP_DECIN_PARAM_T *prDecInParam,WEBP_DECOUT_PARAM_T *prDecOutParam);

extern BOOL _VDEC_Vp8WebPDecStop(UINT32 u4DecId);
extern UINT32 _VDEC_Vp8WebPGetProgress(UINT32 u4DecId);

#endif  // VDEC_VP8_API_H

