/*----------------------------------------------------------------------------
 * No Warranty :  Except  as  may  be  otherwise  agreed  to  in  writing, no *
 * warranties  of  any  kind,  whether  express or  implied, are given by MTK *
 * with  respect  to  any  Confidential  Information  or any use thereof, and *
 * the  Confidential  Information  is  provided  on  an  "AS IS"  basis.  MTK *
 * hereby  expressly  disclaims  all  such  warranties, including any implied *
 * warranties   of  merchantability ,  non-infringement  and  fitness  for  a *
 * particular purpose and any warranties arising out of course of performance *
 * course  of dealing or usage of trade.  Parties further acknowledge that in *
 * connection  with  the Purpose, Company may, either presently and/or in the *
 * future,   instruct   MTK   to   assist  it  in  the  development  and  the *
 * implementation,  in accordance with Company's designs, of certain software *
 * relating  to  Company's  product(s)  (the  "Services").   Except as may be *
 * otherwise agreed to in writing, no warranties of any kind, whether express *
 * or  implied,  are  given by MTK with respect to the Services provided, and *
 * the  Services  are  provided  on  an  "AS  IS"  basis.   Company   further *
 * acknowledges  that  the  Services  may  contain  errors,  which testing is *
 * important  and  it  is  solely  responsible for fully testing the Services *
 * and/or   derivatives   thereof   before  they  are  used,  sublicensed  or *
 * distributed.   Should  there  be any third party action brought again MTK, *
 * arising  out  of  or  relating  to  the  Services,  Company agree to fully *
 * indemnify and hold MTK harmless.                                           *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: imgrz_sw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file imgrz_sw.h
 *  image resizer driver software module interfaces
 *  init, get register base, isr, callback, op count and action
 */

#ifndef IMGRZ_SW_H
#define IMGRZ_SW_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "imgrz_if.h"
#include "imgrz_drvif.h"


#if defined(IMGRZ_ENABLE_SW_MODE)
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef struct _IMGRZ_YCBCR
{
    UINT8 y;
    UINT8 cb;
    UINT8 cr;
} IMGRZ_YCBCR_T;


// OSDMD information structure
typedef struct _RZ_SW_OSDMD_INFO_T
{
    // source
    UINT32 u4RsIn;          // raster scan input (always T)
    UINT32 u4SrcYBase1;     // src buffer addr (16-byte align)
    UINT32 u4DCMD;          // direct(T)/index(F) input color mode
    UINT32 u4InType;        // input color mode (direct/index)
    UINT32 u4SrcBufL;       // src buffer length (16-byte align)
    UINT32 u4SrcHOftY;      // sx coordinate
    UINT32 u4SrcVOftY;      // sy coordinate
    UINT32 u4SrcWidthY;     // src width
    UINT32 u4SrcHeightY;    // src height

    // target
    UINT32 u4RsOut;         // raster scan output (always T)
    UINT32 u4TgYBase;       // tg buffer addr (16-byte align)
    UINT32 u4OutType;       // output color mode (direct/index)
    UINT32 u4TgBufL;        // tg buffer length (16-byte align)
    UINT32 u4TgHOft;        // tx coordinate
    UINT32 u4TgVOft;        // ty coordinate
    UINT32 u4TgWidth;       // tg width
    UINT32 u4TgHeight;      // tg height

    // sram
    UINT32 u4Ext16;         // 16(T)/8(F) bpp pixel precision
    UINT32 u4LBufL;         // tmp line buffer length (32-byte align) (***)

    // hori-8-tap filter
    UINT32 u4H8TapFacY;     // factor
    UINT32 u4H8TapOftY;     // offset

    // hori-src-accumulator
    UINT32 u4HsaFacY;       // factor
    UINT32 u4HsaOftY;       // offset  
        
    // vert-4-tap filter
    UINT32 u4V4TapMd;       // 4-tap(T)/src-accum(F)
    UINT32 u4V4TapFacY;     // factor
    UINT32 u4V4TapOftY;     // offset

    // vert-src-accumulator
    UINT32 u4VupY;          // up(T)/down(F) scaling
    UINT32 u4VFactorY;      // factor
    UINT32 u4VSclOftY;      // offset
    
    // misc
//    UINT32 u4BitExtRepeat;              // 0 (default)
//    UINT32 u4AlphaChgScaleType;         // refer to the nearest (default)
//    UINT32 u4SwitchCbCrPosition;        // 0 (default)
//    UINT32 u4AlphaBlendLevel;           // 0 (default)
//    UINT32 u4UsrDefinedAlpha;           // 0xFF (default)
    
} RZ_SW_OSDMD_INFO_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

// do 16-byte extent
#define RZ_SW_16B_EXTENT(num)     (num << 4)

// do 32-byte extent
#define RZ_SW_32B_EXTENT(num)     (num << 5)


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern void IMGRZ_SwInit(UINT32 u4Which);

extern void IMGRZ_SwGetRegBase(UINT32 u4Which, UINT32 **ppu4RegBase);

extern void IMGRZ_SwISR(UINT32 u4Which);

extern INT32 IMGRZ_SwSetCallBack(UINT32 u4Which, void (*pfnCallBack)(void *pvTag), void *pvTag);

extern UINT32 IMGRZ_SwGetOpCount(UINT32 u4Which);

extern void IMGRZ_SwAction(UINT32 u4Which);

extern void IMGRZ_SwOsdSetDirColorComp(UINT8 **ppu1DestFb, UINT32 u4DirCM, 
    const UINT32 *pu4sDirColorComp);

extern void IMGRZ_SwOsdGetDirColorComp(UINT8 **ppu1DestFb, UINT32 u4DirCM, 
    UINT32 *pu4DirColorComp);

extern void IMGRZ_SwOsdDma(UINT8 *pu1SrcYBase1, UINT8 *pu1TgYBase, UINT32 u4Type, 
    UINT32 u4Pitch, UINT32 u4Width, UINT32 u4Height);

extern INT32 IMGRZ_SwOsdScale(const RZ_SW_OSDMD_INFO_T *prsData);

extern INT32 IMGRZ_SwClipping(INT32 i4Value);


#endif //#if defined(IMGRZ_ENABLE_SW_MODE)


#endif // IMGRZ_SW_H


