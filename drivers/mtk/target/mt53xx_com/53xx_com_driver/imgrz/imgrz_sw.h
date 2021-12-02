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

extern void IMGRZ_SwInit(void);

extern void IMGRZ_SwGetRegBase(UINT32 **ppu4RegBase);

extern void IMGRZ_SwISR(void);

extern INT32 IMGRZ_SwSetCallBack(void (*pfnCallBack)(void *pvTag), void *pvTag);

extern UINT32 IMGRZ_SwGetOpCount(void);

extern void IMGRZ_SwAction(void);

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


