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
 * $Author: cheng.chen $
 * $Date: 2012/10/19 $
 * $RCSfile: osd_region_hw.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_region_hw.c
 *  This header file includes function definitions of OSD region hardware.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"

#ifdef CC_MTK_LOADER
#define HalFlushDCache()
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define OSD_RGN_ALLOCATED 1


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define OSD_REGION_FLUSH(REGION) \
do \
{ \
HalFlushInvalidateDCacheMultipleLine((UINT32)(&_prOsdRegionReg[REGION]), sizeof(OSD_RGN_UNION_T)); \
} \
while (0)

	


/*lint -save -e960 */
#define OSD_REGION_SETGET_TMPL(NAME, FIELD) \
    INT32 _OSD_RGN_Set##NAME(UINT32 u4Region, UINT32 u4Value) \
    { \
        OSD_VERIFY_REGION(u4Region); \
        _prOsdRegionReg[u4Region].rField.FIELD = u4Value; \
        OSD_REGION_FLUSH(u4Region); \
        return (INT32)OSD_RET_OK; \
    } \
    INT32 _OSD_RGN_Get##NAME(UINT32 u4Region, UINT32 *pu4Value) \
    { \
        OSD_VERIFY_REGION(u4Region); \
        if (pu4Value == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
        *pu4Value = _prOsdRegionReg[u4Region].rField.FIELD; \
        return (INT32)OSD_RET_OK; \
    } \

/* above line is intendedly left blanc */
/*lint -restore */

/*lint -save -e960 */
#define OSD_REGION_SETGET_ADDRESS_TMPL(NAME, FIELD) \
    INT32 _OSD_RGN_Set##NAME(UINT32 u4Region, UINT32 u4Value) \
    { \
        OSD_VERIFY_REGION(u4Region); \
        _prOsdRegionReg[u4Region].rField.FIELD = PHYSICAL(u4Value) >> 4; \
        OSD_REGION_FLUSH(u4Region); \
        return (INT32)OSD_RET_OK; \
    } \
    INT32 _OSD_RGN_Get##NAME(UINT32 u4Region, UINT32 *pu4Value) \
    { \
        OSD_VERIFY_REGION(u4Region); \
        if (pu4Value == NULL) \
        { \
            return -(INT32)OSD_RET_INV_ARG; \
        } \
        *pu4Value = VIRTUAL(_prOsdRegionReg[u4Region].rField.FIELD << 4); \
        return (INT32)OSD_RET_OK; \
    } \

/* above line is intendedly left blanc */
/*lint -restore */

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//static UINT8 _au1OsdRegion[(OSD_MAX_NUM_RGN * sizeof(OSD_RGN_UNION_T)) + 15];
static UINT32 _au4OsdRegionStatus[OSD_MAX_NUM_RGN + 1];
static OSD_RGN_UNION_T * _prOsdRegionReg;


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _OSD_RGN_InitApi(void)
{
    INT32 i4Unused;
    // just init once
    if (_prOsdRegionReg == NULL)
    {
#ifdef CC_MTK_LOADER
        UINT32 _au1OsdRegion = (UINT32)x_mem_alloc((OSD_MAX_NUM_RGN * sizeof(OSD_RGN_UNION_T)) + 15);
        _au1OsdRegion = (_au1OsdRegion + 0xf) & ~0xf;
#else
        UINT32 _au1OsdRegion = BSP_AllocAlignedDmaMemory(
                               (OSD_MAX_NUM_RGN * sizeof(OSD_RGN_UNION_T)), 16);
#endif
        _prOsdRegionReg = (OSD_RGN_UNION_T*)VIRTUAL(_au1OsdRegion);
    }
    i4Unused = (INT32)x_memset(_prOsdRegionReg, 0, sizeof(OSD_RGN_UNION_T) *
                               OSD_MAX_NUM_RGN);
    i4Unused = (INT32)x_memset(_au4OsdRegionStatus, 0, sizeof(UINT32) *
                               (OSD_MAX_NUM_RGN + 1));
    UNUSED(i4Unused);
}

OSD_REGION_SETGET_ADDRESS_TMPL(NextRegion, u4NextOsdAddr)
#if !(defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)) 
OSD_REGION_SETGET_TMPL(FifoEx, fgFifoEx)
#endif
OSD_REGION_SETGET_TMPL(NextEnable, fgNextOsdEn)
//OSD_REGION_SETGET_ADDRESS_TMPL(DataAddr, u4DataAddr)
OSD_REGION_SETGET_TMPL(Alpha, u4MixWeight)
OSD_REGION_SETGET_TMPL(HClip, u4HClip)
OSD_REGION_SETGET_TMPL(VClip, u4VClip)
OSD_REGION_SETGET_TMPL(LineSize9, fgLineSize9)
#if !(defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)) 
OSD_REGION_SETGET_TMPL(VbSel, u4VbSel)
#endif
OSD_REGION_SETGET_TMPL(UgSel, u4UgSel)
OSD_REGION_SETGET_TMPL(YrSel, u4YrSel)
OSD_REGION_SETGET_TMPL(ASel, u4AlphaSel)
//OSD_REGION_SETGET_ADDRESS_TMPL(PaletteAddr, u4PaletteAddr)
OSD_REGION_SETGET_TMPL(PaletteLen, u4PaletteLen)
OSD_REGION_SETGET_TMPL(LoadPalette, fgNewPalette)
OSD_REGION_SETGET_TMPL(InputWidth, u4Ihw)
OSD_REGION_SETGET_TMPL(InputHeight, u4Ivw)
OSD_REGION_SETGET_TMPL(LineSize, u4LineSize)
OSD_REGION_SETGET_TMPL(HStep, u4HStep)
OSD_REGION_SETGET_TMPL(VStep, u4VStep)
OSD_REGION_SETGET_TMPL(OutputHeight, u4Ovw)
OSD_REGION_SETGET_TMPL(OutputPosY, u4Ovs)
OSD_REGION_SETGET_TMPL(OutputWidth, u4Ohw)
OSD_REGION_SETGET_TMPL(OutputPosX, u4Ohs)
OSD_REGION_SETGET_TMPL(ColorKeyEnable, fgColorKeyEn)
#if !(defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)) 
OSD_REGION_SETGET_TMPL(FrameMode, fgAcsFrame)
OSD_REGION_SETGET_TMPL(AutoMode, fgAcsAuto)
OSD_REGION_SETGET_TMPL(TopField, fgAcsTop)
#endif
OSD_REGION_SETGET_TMPL(BlendMode, u4MixSel)
OSD_REGION_SETGET_TMPL(HMirror, fgHMirror)
OSD_REGION_SETGET_TMPL(VFlip, fgVFlip)
OSD_REGION_SETGET_TMPL(DecMode, fgDecMode)
OSD_REGION_SETGET_TMPL(DecLine, fgDecLine)
OSD_REGION_SETGET_TMPL(DecEn, fgDecEn)
// for 539x
//OSD_REGION_SETGET_TMPL(HpxExt, fgHpxExt)
//OSD_REGION_SETGET_TMPL(VIntpMode, u4VIntpMode)

INT32 _OSD_RGN_SetPaletteAddr(UINT32 u4Region, UINT32 u4Value)
{
	OSD_VERIFY_REGION(u4Region);
        _prOsdRegionReg[u4Region].rField.u4PaletteAddr = PHYSICAL(u4Value) >> 4; 
        _prOsdRegionReg[u4Region].rField.pal_adr28= PHYSICAL(u4Value) >> 28; 
        OSD_REGION_FLUSH(u4Region); 
        return (INT32)OSD_RET_OK; 
	
}

INT32 _OSD_RGN_GetPaletteAddr(UINT32 u4Region, UINT32 *pu4Value)
{
	OSD_VERIFY_REGION(u4Region); 
        if (pu4Value == NULL) 
        { 
            return -(INT32)OSD_RET_INV_ARG; 
        } 
        *pu4Value = VIRTUAL( (_prOsdRegionReg[u4Region].rField.u4PaletteAddr |
        					((_prOsdRegionReg[u4Region].rField.pal_adr28)<<24))<< 4 ); 
        return (INT32)OSD_RET_OK; 
}

INT32 _OSD_RGN_SetDataAddr(UINT32 u4Region, UINT32 u4Value)
{
	OSD_VERIFY_REGION(u4Region);
        _prOsdRegionReg[u4Region].rField.u4DataAddr = PHYSICAL(u4Value) >> 4; 
        _prOsdRegionReg[u4Region].rField.data_adr28= PHYSICAL(u4Value) >> 28; 
        OSD_REGION_FLUSH(u4Region); 
        return (INT32)OSD_RET_OK; 
	
}
INT32 _OSD_RGN_GetDataAddr(UINT32 u4Region, UINT32 *pu4Value)
{
	OSD_VERIFY_REGION(u4Region); 
        if (pu4Value == NULL) 
        { 
            return -(INT32)OSD_RET_INV_ARG; 
        } 
        *pu4Value = VIRTUAL( (_prOsdRegionReg[u4Region].rField.u4DataAddr |
        					((_prOsdRegionReg[u4Region].rField.data_adr28)<<24))<< 4 ); 
        return (INT32)OSD_RET_OK; 
}

INT32 _OSD_RGN_GetDataHWAddr(UINT32 u4Region, UINT32 *pu4Value)
{
	OSD_VERIFY_REGION(u4Region); 
        if (pu4Value == NULL) 
        { 
            return -(INT32)OSD_RET_INV_ARG; 
        } 
        *pu4Value = (_prOsdRegionReg[u4Region].rField.u4DataAddr << 4 ); 
        return (INT32)OSD_RET_OK; 
}


INT32 _OSD_RGN_SetEndian(UINT32 u4Region, UINT32 u4Value)
{
    OSD_VERIFY_REGION(u4Region);
    _prOsdRegionReg[u4Region].rField.u4PaletteLen &= ~0x2;
    _prOsdRegionReg[u4Region].rField.u4PaletteLen |= (u4Value << 1);
    OSD_REGION_FLUSH(u4Region);
    return (INT32)OSD_RET_OK;
}

INT32 _OSD_RGN_SetHIntMode(UINT32 u4Region, UINT32 u4Value)
{
    //OSD_VERIFY_REGION(u4Region);
    //_prOsdRegionReg[u4Region].rField.u4HintMode = u4Value & 0x3;
    //_prOsdRegionReg[u4Region].rField.u4PaletteLen &= ~0x1;
    //_prOsdRegionReg[u4Region].rField.u4PaletteLen |= ((u4Value >> 2) & 1);
    //OSD_REGION_FLUSH(u4Region);
    return (INT32)OSD_RET_OK;
}

INT32 _OSD_RGN_SetVStepOffset(UINT32 u4Region, UINT32 u4Value)
{
    OSD_VERIFY_REGION(u4Region);
    _prOsdRegionReg[u4Region].rField.u4PaletteAddr &= ~0xfff000;
    _prOsdRegionReg[u4Region].rField.u4PaletteAddr |= ((u4Value & 0xfff) << 12);
    OSD_REGION_FLUSH(u4Region);
    return (INT32)OSD_RET_OK;
}

INT32 _OSD_RGN_SetHStepOffset(UINT32 u4Region, UINT32 u4Value)
{
    OSD_VERIFY_REGION(u4Region);
    _prOsdRegionReg[u4Region].rField.u4PaletteAddr &= ~0xfff;
    _prOsdRegionReg[u4Region].rField.u4PaletteAddr |= (u4Value & 0xfff);
    OSD_REGION_FLUSH(u4Region);
    return (INT32)OSD_RET_OK;
}

INT32 _OSD_RGN_GetEndian(UINT32 u4Region, UINT32 *pu4Value)
{
    OSD_VERIFY_REGION(u4Region);
    if (pu4Value == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    *pu4Value = (_prOsdRegionReg[u4Region].rField.u4PaletteLen >> 1);
    return (INT32)OSD_RET_OK;
}

INT32 _OSD_RGN_GetHIntMode(UINT32 u4Region, UINT32 *pu4Value)
{
    //OSD_VERIFY_REGION(u4Region);
    //if (pu4Value == NULL)
    //{
    //    return -(INT32)OSD_RET_INV_ARG;
    //}
    //*pu4Value = (_prOsdRegionReg[u4Region].rField.u4HintMode |
    //             ((_prOsdRegionReg[u4Region].rField.u4PaletteLen & 1) << 2));
    return (INT32)OSD_RET_OK;
}

INT32 _OSD_RGN_GetVStepOffset(UINT32 u4Region, UINT32 *pu4Value)
{
    OSD_VERIFY_REGION(u4Region);
    if (pu4Value == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    *pu4Value = (_prOsdRegionReg[u4Region].rField.u4PaletteAddr >> 12);
    return (INT32)OSD_RET_OK;
}

INT32 _OSD_RGN_GetHStepOffset(UINT32 u4Region, UINT32 *pu4Value)
{
    OSD_VERIFY_REGION(u4Region);
    if (pu4Value == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    *pu4Value = (_prOsdRegionReg[u4Region].rField.u4PaletteAddr & 0xfff);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_RGN_SetColorMode(UINT32 u4Region, UINT32 u4Value)
{
    OSD_VERIFY_REGION(u4Region);
#if !(defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)) 

    if ((u4Value == 0) ||
        (u4Value == 1) ||
        (u4Value == 2) ||
        (u4Value == 8) ||
        (u4Value == 9) ||
        (u4Value == 10))
    {
        IGNORE_RET(_OSD_RGN_SetFifoEx(u4Region, 0));
    }
    else
    {
        IGNORE_RET(_OSD_RGN_SetFifoEx(u4Region, 1));
    }
#endif

    // luis@2007/4/24, to utilize dram, osd use short fifo
    // luis@2007/5/25, in 5380, OSD will suffer by dram starvation
//    IGNORE_RET(_OSD_RGN_SetFifoEx(u4Region, 0));
    _prOsdRegionReg[u4Region].rField.u4ColorMode = u4Value;
    OSD_REGION_FLUSH(u4Region);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_RGN_GetColorMode(UINT32 u4Region, UINT32 *pu4Value)
{
    OSD_VERIFY_REGION(u4Region);
    if (pu4Value == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    *pu4Value = _prOsdRegionReg[u4Region].rField.u4ColorMode;
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_RGN_SetColorKey(UINT32 u4Region, UINT32 u4Value)
{
    UINT32 u4ColorMode = _prOsdRegionReg[u4Region].rField.u4ColorMode;
    OSD_VERIFY_REGION(u4Region);

    if (((u4ColorMode >= (UINT32)OSD_CM_CBYCRY422_DIRECT16) &&
         (u4ColorMode <= (UINT32)OSD_CM_AYCBCR8888_DIRECT32)) ||
        ((u4ColorMode >= (UINT32)OSD_CM_RGB565_DIRECT16) &&
         (u4ColorMode <= (UINT32)OSD_CM_ARGB8888_DIRECT32)))
    {
        _prOsdRegionReg[u4Region].rField.u4PaletteAddr = u4Value >> 8;
    }
    _prOsdRegionReg[u4Region].rField.u4ColorKey = u4Value & 0xff;
    OSD_REGION_FLUSH(u4Region);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_RGN_GetColorKey(UINT32 u4Region, UINT32 *pu4Value)
{
    OSD_VERIFY_REGION(u4Region);
    if (pu4Value == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    *pu4Value = (_prOsdRegionReg[u4Region].rField.u4PaletteAddr << 8) |
                _prOsdRegionReg[u4Region].rField.u4ColorKey;
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_RGN_GetHandle(UINT32 u4Addr, UINT32 *pu4Region)
{
    if ((u4Addr < (UINT32)_prOsdRegionReg) ||
        (u4Addr >= ((OSD_MAX_NUM_RGN * sizeof(OSD_RGN_UNION_T)) +
        (UINT32)_prOsdRegionReg)))
    {
        return -(INT32)OSD_RET_UNDEF_ERR;
    }

    if (pu4Region == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    *pu4Region = (u4Addr - (UINT32)_prOsdRegionReg) / sizeof(OSD_RGN_UNION_T);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_RGN_GetAddress(UINT32 u4Region , UINT32 *pu4Addr)
{
    OSD_VERIFY_REGION(u4Region);
    if (pu4Addr == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    *pu4Addr = (UINT32)&_prOsdRegionReg[u4Region];
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_RGN_Alloc(UINT32 *pu4Region)
{
    INT32 i4Rgn;
    if (pu4Region == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }

    for (i4Rgn = 0; i4Rgn < OSD_MAX_NUM_RGN; i4Rgn++)
    {
        if ((_au4OsdRegionStatus[i4Rgn] & OSD_RGN_ALLOCATED) == 0)
        {
            *pu4Region = (UINT32)i4Rgn;
            _au4OsdRegionStatus[i4Rgn] |= OSD_RGN_ALLOCATED;
            return (INT32)OSD_RET_OK;
        }
    }

    return -(INT32)OSD_RET_OUT_OF_REGION;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
INT32 _OSD_RGN_Free(UINT32 u4Region)
{
    OSD_VERIFY_REGION(u4Region);

    _au4OsdRegionStatus[u4Region] &= ~OSD_RGN_ALLOCATED;
    _prOsdRegionReg[u4Region].rField.fgNextOsdEn = FALSE;
    return (INT32)OSD_RET_OK;
}
