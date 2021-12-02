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
 * $RCSfile: imgrz_sw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_sw.c
 *  image resizer driver software module interfaces
 *  init, get register base, isr, callback, op count and action
 */

/*
    lint inhibition(s)
        971 - Use of 'char' without 'signed' or 'unsigned'
        961 - #undef (used in middleware u_common.h and engine workaround
        740 - this software engine collected bytes to 32bits to accelerate
        826 - pointer to pointer conversion used in frame buffer fill
        701 - yes, here needs several signed shift
        703 - Shift left of signed quantity (long)
        704 - Shift right of signed quantity (long)
        644 - Variable 'ai4ColorCompMax' may not have been initialized
        613 - Possible use of null pointer 'pu1SrcBase' in left argument 
              to operator 'ptr+int'
        522 - Expected void type, assignment, increment or decrement
        834 - Operator '-' followed by operator '-' is confusing
        613 - Possible use of null pointer
        794 - Conceivable use of null pointer
*/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e961 -e971 -e10 -e18 -e19 */
#include "x_debug.h"

#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "imgrz_sw.h"
#include "imgrz_dif.h"
/*lint -restore */


#if defined(IMGRZ_ENABLE_SW_MODE)
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define D_RZ_SW_TMP_BUF_ONE_COMP_SIZE    (720 * 480)


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

#define _pvRzSwCallBackTag  (_imgrz_sw_param[u4Which].pvRzSwCallBackTag)
#define _au4RzSwReg         (_imgrz_sw_param[u4Which].au4RzSwReg)
#define _u4RzSwActionCount  (_imgrz_sw_param[u4Which].u4RzSwActionCount)

#define _pu1SrcDataA        (_imgrz_sw_param[u4Which].pu1SrcDataA)
#define _pu1SrcDataR        (_imgrz_sw_param[u4Which].pu1SrcDataR)
#define _pu1SrcDataG        (_imgrz_sw_param[u4Which].pu1SrcDataG)
#define _pu1SrcDataB        (_imgrz_sw_param[u4Which].pu1SrcDataB)

#define _pu1TgDataA         (_imgrz_sw_param[u4Which].pu1TgDataA)
#define _pu1TgDataR         (_imgrz_sw_param[u4Which].pu1TgDataR)
#define _pu1TgDataG         (_imgrz_sw_param[u4Which].pu1TgDataG)
#define _pu1TgDataB         (_imgrz_sw_param[u4Which].pu1TgDataB)

typedef void (*CB_RzSwCallBack)(void *pvTag);
#define _pfnRzSwCallBack    (_imgrz_sw_param[u4Which].pfnRzSwCallBack)

typedef struct _IMGRZ_SW_PARMA_T
{
    void    *pvRzSwCallBackTag;
    UINT32  au4RzSwReg[IMGRZ_REG_FILE_SIZE];    
    UINT32  u4RzSwActionCount;    
    UINT8   *pu1SrcDataA;
    UINT8   *pu1SrcDataR;
    UINT8   *pu1SrcDataG;
    UINT8   *pu1SrcDataB;    
    UINT8   *pu1TgDataA;
    UINT8   *pu1TgDataR;
    UINT8   *pu1TgDataG;
    UINT8   *pu1TgDataB;

    CB_RzSwCallBack pfnRzSwCallBack;
}IMGRZ_SW_PARMA_T;

static IMGRZ_SW_PARMA_T _imgrz_sw_param[IMGRZ_NUM];

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _RzSwJpgScale
 *  
 */
//-------------------------------------------------------------------------
//static INT32 _RzSwJpgScale(const E_RZ_DIF_FIELD_T *prsReg)
//{
//}


//-------------------------------------------------------------------------
/** _RzSwVdoScale
 *  
 */
//-------------------------------------------------------------------------
//static INT32 _RzSwVdoScale(const E_RZ_DIF_FIELD_T *prsReg)
//{
//}


//-------------------------------------------------------------------------
/** _RzSwOsdScale
 *  
 */
//-------------------------------------------------------------------------
static INT32 _RzSwOsdScale(const E_RZ_DIF_FIELD_T *prsReg)
{
    RZ_SW_OSDMD_INFO_T rRzOsd;

    // source buffer information
    rRzOsd.u4RsIn       = prsReg->fg_RSIN;
    rRzOsd.u4SrcYBase1  = RZ_SW_16B_EXTENT(prsReg->fg_SRCYBASE1);
    rRzOsd.u4DCMD       = prsReg->fg_DCMD;
    rRzOsd.u4InType     = prsReg->fg_INTYPE;
    rRzOsd.u4SrcBufL    = RZ_SW_16B_EXTENT(prsReg->fg_SRCBUFL);
    rRzOsd.u4SrcHOftY   = prsReg->fg_SRC_H_OFTY;
    rRzOsd.u4SrcVOftY   = prsReg->fg_SRC_V_OFTY;
    rRzOsd.u4SrcWidthY  = prsReg->fg_SRC_WIDTHY;
    rRzOsd.u4SrcHeightY = prsReg->fg_SRC_HEIGHTY;

    // target buffer information
    rRzOsd.u4RsOut    = prsReg->fg_RSOUT;
    rRzOsd.u4TgYBase  = RZ_SW_16B_EXTENT(prsReg->fg_TGYBASE);
    rRzOsd.u4OutType  = prsReg->fg_OUTTYPE;
    rRzOsd.u4TgBufL   = RZ_SW_16B_EXTENT(prsReg->fg_TGBUFL);
    rRzOsd.u4TgHOft   = prsReg->fg_TG_H_OFT;
    rRzOsd.u4TgVOft   = prsReg->fg_TG_V_OFT;
    rRzOsd.u4TgWidth  = prsReg->fg_TG_WIDTH;
    rRzOsd.u4TgHeight = prsReg->fg_TG_HEIGHT;

    // sram information
    rRzOsd.u4Ext16 = prsReg->fg_EXT16;
    rRzOsd.u4LBufL = prsReg->fg_LBUFL;

    // hori-8-tap filter information
    rRzOsd.u4H8TapFacY = prsReg->fg_H8TAP_FAC_Y;
    rRzOsd.u4H8TapOftY = prsReg->fg_H8TAP_OFT_Y;

    // hori-src-accumulator information
    rRzOsd.u4HsaFacY = prsReg->fg_HSA_FAC_Y;
    rRzOsd.u4HsaOftY = prsReg->fg_HSA_OFT_Y;
        
    // vert-4-tap filter information
    rRzOsd.u4V4TapMd = prsReg->fg_V4TAPMD;
    rRzOsd.u4V4TapFacY = prsReg->fg_V4TAP_FAC_Y;
    rRzOsd.u4V4TapOftY = prsReg->fg_V4TAP_OFT_Y;

    // vert-src-accumulator information
    rRzOsd.u4VupY = prsReg->fg_VUP_Y;
    rRzOsd.u4VFactorY = prsReg->fg_V_FACTORY;
    rRzOsd.u4VSclOftY = prsReg->fg_V_SCLOFTY;

    // virtual address
    rRzOsd.u4SrcYBase1 = VIRTUAL(rRzOsd.u4SrcYBase1);
    // virtual address
    rRzOsd.u4TgYBase = VIRTUAL(rRzOsd.u4TgYBase);

    return IMGRZ_SwOsdScale(&rRzOsd);
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** IMGRZ_SwInit
 *
 */
//-------------------------------------------------------------------------
void IMGRZ_SwInit(UINT32 u4Which)
{
    _pfnRzSwCallBack   = NULL;
    _pvRzSwCallBackTag = NULL;
    _u4RzSwActionCount = 0;
    
    IMGRZ_UNUSED_RET(x_memset(_au4RzSwReg, 0, sizeof(_au4RzSwReg)))


    // allocate SrcDataA/R/G/B
    if (_pu1SrcDataA == NULL)
    {
        _pu1SrcDataA = (UINT8 *)BSP_AllocAlignedDmaMemory(D_RZ_SW_TMP_BUF_ONE_COMP_SIZE, 4);
        VERIFY(_pu1SrcDataA != NULL);
    }
    
    if (_pu1SrcDataR == NULL)
    {
        _pu1SrcDataR = (UINT8 *)BSP_AllocAlignedDmaMemory(D_RZ_SW_TMP_BUF_ONE_COMP_SIZE, 4);
        VERIFY(_pu1SrcDataR != NULL);
    }

    if (_pu1SrcDataG == NULL)
    {
        _pu1SrcDataG = (UINT8 *)BSP_AllocAlignedDmaMemory(D_RZ_SW_TMP_BUF_ONE_COMP_SIZE, 4);
        VERIFY(_pu1SrcDataG != NULL);
    }

    if (_pu1SrcDataB == NULL)
    {
        _pu1SrcDataB = (UINT8 *)BSP_AllocAlignedDmaMemory(D_RZ_SW_TMP_BUF_ONE_COMP_SIZE, 4);
        VERIFY(_pu1SrcDataB != NULL);
    }

    // allocate TgDataA/R/G/B
    if (_pu1TgDataA == NULL)
    {
        _pu1TgDataA = (UINT8 *)BSP_AllocAlignedDmaMemory(D_RZ_SW_TMP_BUF_ONE_COMP_SIZE, 4);
        VERIFY(_pu1TgDataA != NULL);
    }

    if (_pu1TgDataR == NULL)
    {
        _pu1TgDataR = (UINT8 *)BSP_AllocAlignedDmaMemory(D_RZ_SW_TMP_BUF_ONE_COMP_SIZE, 4);
        VERIFY(_pu1TgDataR != NULL);
    }

    if (_pu1TgDataG == NULL)
    {
        _pu1TgDataG = (UINT8 *)BSP_AllocAlignedDmaMemory(D_RZ_SW_TMP_BUF_ONE_COMP_SIZE, 4);
        VERIFY(_pu1TgDataG != NULL);
    }

    if (_pu1TgDataB == NULL)
    {
        _pu1TgDataB = (UINT8 *)BSP_AllocAlignedDmaMemory(D_RZ_SW_TMP_BUF_ONE_COMP_SIZE, 4);
        VERIFY(_pu1TgDataB != NULL);
    }
}


//-------------------------------------------------------------------------
/** IMGRZ_SwGetRegBase
 *
 */
//-------------------------------------------------------------------------
void IMGRZ_SwGetRegBase(UINT32 u4Which, UINT32 **ppu4RegBase)
{
    IMGRZ_ASSERT(ppu4RegBase != NULL);
    IMGRZ_ASSERT(*ppu4RegBase != NULL);

    *ppu4RegBase = (UINT32 *)_au4RzSwReg;
}


//-------------------------------------------------------------------------
/** IMGRZ_SwISR
 *
 */
//-------------------------------------------------------------------------
void IMGRZ_SwISR(UINT32 u4Which)
{
    if (_pfnRzSwCallBack != NULL)
    {
        _pfnRzSwCallBack(_pvRzSwCallBackTag);
    }
}


//-------------------------------------------------------------------------
/** IMGRZ_SwSetCallBack
 *
 */
//-------------------------------------------------------------------------
INT32 IMGRZ_SwSetCallBack(UINT32 u4Which, void (*pfnCallBack)(void *pvTag), void *pvTag)
{
    if (pfnCallBack != NULL)
    {
        _pfnRzSwCallBack   = pfnCallBack;
        _pvRzSwCallBackTag = pvTag;
        return (INT32)E_RZ_OK;
    }

    return -(INT32)E_RZ_INV_ARG;
}


//-------------------------------------------------------------------------
/** IMGRZ_SwGetOpCount
 *
 */
//-------------------------------------------------------------------------
UINT32 IMGRZ_SwGetOpCount(UINT32 u4Which)
{
    return _u4RzSwActionCount;
}


//-------------------------------------------------------------------------
/** IMGRZ_SwAction
 *
 */
//-------------------------------------------------------------------------
/*lint -save -e740 -e826 */
void IMGRZ_SwAction(UINT32 u4Which)
{
    INT32 i4Ret;
    E_RZ_DIF_FIELD_T *prRzReg;

    prRzReg = (E_RZ_DIF_FIELD_T *)_au4RzSwReg;

    if (prRzReg->fg_OSDMD == IMGRZ_ENABLE)
    {
        i4Ret = _RzSwOsdScale(prRzReg);
    }
    else 
    {
        if (prRzReg->fg_JPGMD == IMGRZ_ENABLE)
        {
//            i4Ret = _RzSwJpgScale(prRzReg);
        }
        else
        {
//            i4Ret = _RzSwVdoScale(prRzReg);
        }
    }

    VERIFY(i4Ret == (INT32)E_RZ_OK);
}
/*lint -restore */


//-------------------------------------------------------------------------
/** IMGRZ_SwOsdSetColorComponent
 *  set color components to frame buffer, 
 *  advance frame buffer point for 1 pixel
 *      rgb565
 *      argb1555
 *      argb4444
 *      argb8888/aycbcr8888
 */
//-------------------------------------------------------------------------
void IMGRZ_SwOsdSetDirColorComp(UINT8 **ppu1DestFb, UINT32 u4DirCM, 
    const UINT32 *pu4sDirColorComp)
{
    INT32  u4A, u4R, u4G, u4B;
    UINT32 u4Color;
    UINT16 u2Color;
//    UINT8  u1Color;

    if ((ppu1DestFb == NULL) || (pu4sDirColorComp == NULL))
    {
        return;
    }

    u4A = pu4sDirColorComp[0];
    u4R = pu4sDirColorComp[1];
    u4G = pu4sDirColorComp[2];
    u4B = pu4sDirColorComp[3];

    u4A = (u4A > 255) ? 255 :
          (u4A <   0) ?   0 : u4A;

    u4R = (u4R > 255) ? 255 :
          (u4R <   0) ?   0 : u4R;

    u4G = (u4G > 255) ? 255 :
          (u4G <   0) ?   0 : u4G;

    u4B = (u4B > 255) ? 255 :
          (u4B <   0) ?   0 : u4B;
          
    switch (u4DirCM)
    {
    case E_RZ_OSD_DIR_CM_RGB565:
        u2Color = ((u4R & 0xf8) << 8)   |
                  ((u4G & 0xfc) << 3)   |
                  (u4B >> 3);
        *((UINT16 *)*ppu1DestFb) = u2Color;
        *ppu1DestFb += 2;
        break;

    case E_RZ_OSD_DIR_CM_ARGB1555:
        u2Color = (u4A ? 0x8000 : 0)     |
                  ((u4R & 0xf8) << 7)    |
                  ((u4G & 0xf8) << 2)    |
                  (u4B >> 3);
        *((UINT16 *)*ppu1DestFb) = u2Color;
        *ppu1DestFb += 2;
        break;

    case E_RZ_OSD_DIR_CM_ARGB4444:
        u2Color = ((u4A & 0xf0) << 8)   |
                  ((u4R & 0xf0) << 4)   |
                  (u4G & 0xf0)          |
                  (u4B >> 4);
        *((UINT16 *)*ppu1DestFb) = u2Color;
        *ppu1DestFb += 2;
        break;

    case E_RZ_OSD_DIR_CM_ARGB8888:
        u4Color = (u4A << 24) |
                  (u4R << 16) |
                  (u4G <<  8) |
                  u4B;
        *((UINT32 *)*ppu1DestFb) = u4Color;
        *ppu1DestFb += 4;
        break;

    default:
        return;
    }

    return;
}


//-------------------------------------------------------------------------
/** IMGRZ_SwOsdetDirColorComp
 *  set color components to frame buffer, 
 *  advance frame buffer point for 1 pixel
 *      rgb565
 *      argb1555
 *      argb4444
 *      argb8888/aycbcr8888
 */
//-------------------------------------------------------------------------
void IMGRZ_SwOsdGetDirColorComp(UINT8 **ppu1DestFb, UINT32 u4DirCM, 
    UINT32 *pu4DirColorComp)
{
    UINT32 u4A, u4R, u4G, u4B;
    UINT32 u4Color;
    UINT16 u2Color;
 //   UINT8  u1Color;

    if ((ppu1DestFb == NULL) || (pu4DirColorComp == NULL))
    {
        return;
    }

    switch (u4DirCM)
    {
    case E_RZ_OSD_DIR_CM_RGB565:
        u2Color = *((UINT16 *)*ppu1DestFb);
        u4A = 0xff;
        u4R = (u2Color >> 8) & 0xf8;
        u4G = (u2Color >> 3) & 0xfc;
        u4B = (u2Color << 3) & 0xf8;

        u4R = u4R | (u4R >> 5);
        u4G = u4G | (u4G >> 6);
        u4B = u4B | (u4B >> 5);

        *ppu1DestFb += 2;
        break;

    case E_RZ_OSD_DIR_CM_ARGB1555:
        u2Color = *((UINT16 *)*ppu1DestFb);
        u4A = (u2Color & 0x8000) ? 0xff : 0;
        u4R = (u2Color >> 7) & 0xf8;
        u4G = (u2Color >> 2) & 0xf8;
        u4B = (u2Color << 3) & 0xf8;

        u4R = u4R | (u4R >> 5);
        u4G = u4G | (u4G >> 5);
        u4B = u4B | (u4B >> 5);

        *ppu1DestFb += 2;
        break;

    case E_RZ_OSD_DIR_CM_ARGB4444:
        u2Color = *((UINT16 *)*ppu1DestFb);
        u4A = (u2Color >> 8) & 0xf0;
        u4R = (u2Color >> 4) & 0xf0;
        u4G = u2Color & 0xf0;
        u4B = (u2Color << 4) & 0xf0;

        u4A = u4A | (u4A >> 4);
        u4R = u4R | (u4R >> 4);
        u4G = u4G | (u4G >> 4);
        u4B = u4B | (u4B >> 4);

        *ppu1DestFb += 2;
        break;

    case E_RZ_OSD_DIR_CM_ARGB8888:
        u4Color = *((UINT32 *)*ppu1DestFb);
        u4A = u4Color >> 24;
        u4R = (u4Color >> 16) & 0xff;
        u4G = (u4Color >>  8) & 0xff;
        u4B = u4Color & 0xff;
        *ppu1DestFb += 4;
        break;

    default:
        return;
    }

    pu4DirColorComp[0] = u4A;
    pu4DirColorComp[1] = u4R;
    pu4DirColorComp[2] = u4G;
    pu4DirColorComp[3] = u4B;

    return;
}


//-------------------------------------------------------------------------
/** IMGRZ_SwOsdDma
 *      rgb565
 *      argb1555
 *      argb4444
 *      argb8888/aycbcr8888
 */
//-------------------------------------------------------------------------
void IMGRZ_SwOsdDma(UINT8 *pu1SrcYBase1, UINT8 *pu1TgYBase, UINT32 u4Type, 
    UINT32 u4Pitch, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 x, y;
    UINT8 *pu1SrcLine, *pu1TgLine;
    UINT8 *pu1Read, *pu1Write;
    UINT32 au4DirColorComp[4];

    pu1Read    = pu1SrcYBase1;
    pu1SrcLine = pu1SrcYBase1;
    pu1Write   = pu1TgYBase;
    pu1TgLine  = pu1TgYBase;

    // u4Height must be equal to 1
    for (y = 0; y < u4Height; y++)
    {
        for (x = 0; x < u4Width; x++)
        {
            IMGRZ_SwOsdGetDirColorComp(&pu1Read, u4Type, au4DirColorComp);

            IMGRZ_SwOsdSetDirColorComp(&pu1Write, u4Type, au4DirColorComp);
        } // ~for

        pu1SrcLine += u4Pitch;
        pu1TgLine  += u4Pitch;
        pu1Read     = pu1SrcLine;
        pu1Write    = pu1TgLine;
    } // ~for
}

    
//-------------------------------------------------------------------------
/** IMGRZ_SwOsdScale
 *  
 *
 */
//-------------------------------------------------------------------------
INT32 IMGRZ_SwOsdScale(const RZ_SW_OSDMD_INFO_T *prsData)
{
//    UINT32 x, y;
//    UINT8 *pu1SrcLine, *pu1TgLine;
 //   UINT8 *pu1Read, *pu1Write;
 //   UINT32 au4DirColorComp[4];
    
    UINT8 *pu1SrcYBase1, *pu1TgYBase;
    UINT32 u4DCMD, u4InType, u4SrcBufL;//UINT32 u4RsIn, u4DCMD, u4InType, u4SrcBufL;
    UINT32 u4SrcWidthY, u4SrcHeightY;//UINT32 u4SrcHOftY, u4SrcVOftY, u4SrcWidthY, u4SrcHeightY;
//    UINT32 u4RsOut, u4OutType, u4TgBufL;
    UINT32 u4TgWidth, u4TgHeight;//UINT32 u4TgHOft, u4TgVOft, u4TgWidth, u4TgHeight;
//    UINT32 u4Ext16, u4LBufL;
//    UINT32 u4H8TapFacY, u4H8TapOftY;
//    UINT32 u4HsaFacY, u4HsaOftY;
//    UINT32 u4V4TapMd, u4V4TapFacY, u4V4TapOftY;
//    UINT32 u4VupY, u4VFactorY, u4VSclOftY;

    
    // check parameters' error
    VERIFY(prsData != NULL);
    VERIFY(prsData->u4RsIn == 1);
    VERIFY(prsData->u4SrcYBase1 != NULL);
    VERIFY(prsData->u4SrcWidthY > 0);
    VERIFY(prsData->u4SrcHeightY > 0);
    VERIFY(prsData->u4RsOut == 1);
    VERIFY(prsData->u4TgYBase != NULL);
    VERIFY(prsData->u4TgWidth > 0);
    VERIFY(prsData->u4TgHeight > 0);

    // source buffer information
   // u4RsIn       = prsData->u4RsIn;
    pu1SrcYBase1 = (UINT8 *)prsData->u4SrcYBase1;
    u4DCMD       = prsData->u4DCMD;
    u4InType     = prsData->u4InType;
    u4SrcBufL    = prsData->u4SrcBufL;
   // u4SrcHOftY   = prsData->u4SrcHOftY;
   // u4SrcVOftY   = prsData->u4SrcVOftY;
    u4SrcWidthY  = prsData->u4SrcWidthY;
    u4SrcHeightY = prsData->u4SrcHeightY;

    // target buffer information
    //u4RsOut    = prsData->u4RsOut;
    pu1TgYBase = (UINT8 *)prsData->u4TgYBase;
    //u4OutType  = prsData->u4OutType;
    //u4TgBufL   = prsData->u4TgBufL;
    //u4TgHOft   = prsData->u4TgHOft;
    //u4TgVOft   = prsData->u4TgVOft;
    u4TgWidth  = prsData->u4TgWidth;
    u4TgHeight = prsData->u4TgHeight;

    // sram information
    //u4Ext16 = prsData->u4Ext16;
    //u4LBufL = prsData->u4LBufL;

    // hori-8-tap filter information
    //u4H8TapFacY = prsData->u4H8TapFacY;
    //u4H8TapOftY = prsData->u4H8TapOftY;

    // hori-src-accumulator information
    //u4HsaFacY = prsData->u4HsaFacY;
    //u4HsaOftY = prsData->u4HsaOftY;
        
    // vert-4-tap filter information
    //u4V4TapMd   = prsData->u4V4TapMd;
    //u4V4TapFacY = prsData->u4V4TapFacY;
    //u4V4TapOftY = prsData->u4V4TapOftY;

    // vert-src-accumulator information
    //u4VupY     = prsData->u4VupY;
    //u4VFactorY = prsData->u4VFactorY;
    //u4VSclOftY = prsData->u4VSclOftY;


    //pu1Read    = pu1SrcYBase1;
    //pu1SrcLine = pu1SrcYBase1;
    //pu1Write   = pu1TgYBase;
    //pu1TgLine  = pu1TgYBase;

    // DMA
    if ((u4SrcWidthY  == u4TgWidth)     && 
        (u4SrcHeightY == u4TgHeight)    && 
        (u4DCMD == E_RZ_OSD_IN_CM_DIRECT))
    {
        IMGRZ_SwOsdDma(pu1SrcYBase1, pu1TgYBase, u4InType, 
            u4SrcBufL, u4SrcWidthY, u4SrcHeightY);
        return (INT32)E_RZ_OK;
    }

    return (INT32)E_RZ_OK;
}


#if 0
void IMGRZ_SwConvertSrcTo32bppTmpData(
    UINT32 u4DCMD, UINT32 u4InType, UINT32 *srcData, 
    UINT8 *srcDataA, UINT8 *srcDataR,
    UINT8 *srcDataG, UINT8 *srcDataB)
{
    UINT32 i, j, oft, srcOft;
    UINT32 srcBufHgh, srcBufLen, width;
    
	if ((E_RZ_OSD_IN_CM_INDEX == u4DCMD) && 
	    (E_RZ_OSD_CM_8BPP == u4InType))
	{
	    //------------------------
	    // 256 color index mode
	    //------------------------

		for (j = 0; j < srcBufHgh; j++)
		{
			oft = (j * srcBufLen);
			srcOft = (j * width);

			for (i = 0; i < width; i++)
			{
				srcDataA[oft + i] = (UINT8)((cpt_256[srcData[srcOft + i]] & 0xff000000) >> 24);
				srcDataR[oft + i] = (UINT8)((cpt_256[srcData[srcOft + i]] & 0x00ff0000) >> 16);
				srcDataG[oft + i] = (UINT8)((cpt_256[srcData[srcOft + i]] & 0x0000ff00) >> 8);
				srcDataB[oft + i] = (UINT8)((cpt_256[srcData[srcOft + i]] & 0x000000ff));
			}

			for( ; i < srcBufLen; i++)
			{
				srcDataA[oft + i] = 0;
				srcDataB[oft + i] = 0;
				srcDataG[oft + i] = 0;
				srcDataR[oft + i] = 0;
			}
		}
	}
	else if ((E_RZ_OSD_IN_CM_INDEX == u4DCMD) && 
	         (E_RZ_OSD_CM_4BPP == u4InType)) 
	{
	    //------------------------
        // 16 color index mode
	    //------------------------

		for(j = 0; j < srcBufHgh; j++)
		{
			oft = (j * srcBufLen);
			srcOft = (j * width);
			
			for(i=0; i < width; i+=2)
			{
				idx0 = (srcData[srcOft + i] & 0xf0) >> 4;
				idx1 = (srcData[srcOft + i] & 0xf) >> 4;

				srcDataA[oft + i] = (UINT8)((cpt_256[idx0] & 0xff000000) >> 24);
				srcDataR[oft + i] = (UINT8)((cpt_256[idx0] & 0x00ff0000) >> 16);
				srcDataG[oft + i] = (UINT8)((cpt_256[idx0] & 0x0000ff00) >> 8);
				srcDataB[oft + i] = (UINT8)((cpt_256[idx0] & 0x000000ff));

				srcDataA[oft + i + 1] = (UINT8)((cpt_256[idx1] & 0xff000000) >> 24);
				srcDataR[oft + i + 1] = (UINT8)((cpt_256[idx1] & 0x00ff0000) >> 16);
				srcDataG[oft + i + 1] = (UINT8)((cpt_256[idx1] & 0x0000ff00) >> 8);
				srcDataB[oft + i + 1] = (UINT8)((cpt_256[idx1] & 0x000000ff));
			}

			for( ; i < srcBufLen; i++)
			{
				srcDataA[oft + i] = 0;
				srcDataB[oft + i] = 0;
				srcDataG[oft + i] = 0;
				srcDataR[oft + i] = 0;
			}
		}
	}
	else if ((E_RZ_OSD_IN_CM_DIRECT == u4DCMD)      && 
	         ((E_RZ_OSD_CM_RGB565   == u4InType) || 
 	          (E_RZ_OSD_CM_ARGB1555 == u4InType) ||
              (E_RZ_OSD_CM_ARGB4444 == u4InType)))
	{
		for(j=0; j < srcBufHgh; j++)
		{
			oft = j*srcBufLen;
			srcOft = j*width;
			for(i=0; i < width; i++)
			{
				byte0 = srcData[(srcOft + i) * 2];
				byte1 = srcData[(srcOft + i) * 2 + 1];
				if(inType == 4) // 565
				{
					srcDataA[oft + i] = 0xff;
                    srcDataR[oft + i] = static_cast<UINT8>(byte1 & 0xf8); // 5
-bit
                    srcDataG[oft + i] = static_cast<UINT8>(((byte1 & 0x7) << 5
) + ((byte0 & 0xe0) >> 3)); // 6-bit
                    srcDataB[oft + i] = static_cast<UINT8>((byte0 & 0x1f) << 3
); // 5-bit
				}
				else if(inType == 5) // 1555
				{
					srcDataA[oft + i] = ((byte1 & 0x80) == 0x80) ? ((a_table & 0xff00) >> 8) 
: (a_table & 0xff);
                    srcDataR[oft + i] = (UINT8) ((byte1 & 0x7c) << 1); // 5-bit
                    srcDataG[oft + i] = (UINT8) (((byte1 & 0x3) << 6) + ((
byte0 & 0xe0) >> 2)); // 5-bit
                    srcDataB[oft + i] = (UINT8) ((byte0 & 0x1f) << 3); // 5-bit
				}
				else  // 4444
				{
					srcDataA[oft + i] = ((byte1 & 0xf0) == 0xf0) ? 0xff : byte1 & 0xf0; // 4-
bit
                    srcDataR[oft + i] = (UINT8) ((byte1 & 0x0f) << 4); // 4-bit
                    srcDataG[oft + i] = (UINT8) (byte0 & 0xf0); // 4-bit
                    srcDataB[oft + i] = (UINT8) ((byte0 & 0x0f) << 4); // 4-bit
				}
			}
			for(;i < srcBufLen; i++)
			{
				srcDataA[oft + i] = 
				srcDataB[oft + i] = 
				srcDataG[oft + i] = 
				srcDataR[oft + i] = 0;
			}
		}
	}
	else  // 32bpp
	{
		for(j=0; j < srcBufHgh; j++)
		{
			oft = j*srcBufLen;
			srcOft = j*width;
			for(i=0; i < width; i++)
			{
				srcDataB[oft + i] = srcData[(srcOft + i)*3 ];
				srcDataG[oft + i] = srcData[(srcOft + i)*3 +1];
				srcDataR[oft + i] = srcData[(srcOft + i)*3 +2];
				srcDataA[oft + i] = 0;
			}
			for(;i < srcBufLen; i++)
			{
				srcDataA[oft + i] = 
				srcDataB[oft + i] = 
				srcDataG[oft + i] = 
				srcDataR[oft + i] = 0;
			}
		}
	}
}
#endif


//-------------------------------------------------------------------------
/** IMGRZ_SwClipping
 *  Clip value within 0 ~ 255
 *
 */
//-------------------------------------------------------------------------
INT32 IMGRZ_SwClipping(INT32 i4Value)
{
    if (i4Value > 255)
    {
        return 255;
    }
    else if (i4Value < 0)
    {
        return 0;
    }
    else
    {
        return i4Value;
    }
}


#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)


