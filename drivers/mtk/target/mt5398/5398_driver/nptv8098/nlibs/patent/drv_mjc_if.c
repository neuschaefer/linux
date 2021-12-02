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
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_mjc_if.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "x_assert.h"
#include "hw_mjc.h"
#include "hw_sw.h"
#include "hw_vdoin.h"
#include "drv_mjc.h"
#include "drv_mjc_if.h"
#include "drv_mjc_fbck.h"
#include "drv_mjc_int.h"
#include "drv_display.h"
#include "mute_if.h"
#include "vdo_misc.h"
#include "nptv_debug.h"
#include "hw_ospe.h"

#ifdef W_MAX
#undef W_MAX
#endif

#ifdef LERP
#undef LERP
#endif

#define W_MAX(res) (1<<(res))
#define LERP(a, b, wei, res) ((((UINT32)(a))*(wei) + ((UINT32)(b))*(W_MAX(res)-(wei)) + (W_MAX(res)>>1)) >> (res))

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/

UINT8 u1MJCEffectLevel[E_MJC_EFFECT_NUM] =
{
    0, 0x40, 0x80, 0xff
};

static UINT8 _u1FbckRmpTbl[65] =
{
    0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,13,14,15,
    16,18,20,22,24,26,28,30,32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,32,32,32,32,32,33,35,36,
    38,40,41,43,44,46,48,49,51,52,54,56,57,59,60,62,
    64
};

MJC_FBCK_REMAP_DFT_T _rRemapDft;
MJC_FBCK_REMAP_2SEGMENT_T _rRemap2Seg;


static UINT8 _arMJCVerfDramModeIdx[MJC_DRAM_VERF_MODE_NUM] =
{
    0,  //MJC_Y10_C10_422_0RDC_3FRM
    1,  //MJC_Y10_C10_422_1RDC_3FRM
    2,  //MJC_Y10_C10_420_0RDC_3FRM
    3,  //MJC_Y10_C10_420_1RDC_3FRM
    10, //MJC_Y10_C08_422_0RDC_3FRM
    11, //MJC_Y10_C08_422_1RDC_3FRM
    12, //MJC_Y10_C08_420_0RDC_3FRM
    13, //MJC_Y10_C08_420_1RDC_3FRM
    20, //MJC_Y08_C08_422_0RDC_3FRM
    21, //MJC_Y08_C08_422_1RDC_3FRM
    22, //MJC_Y08_C08_420_0RDC_3FRM
    23, //MJC_Y08_C08_420_1RDC_3FRM
    29, //MJC_Y08_C08_420_0RDC_1FRM
    30, //FRC_Y10_C10_444
    31, //FRC_Y08_C08_444
    32, //FRC_Y10_C10_422RPT
    33  //FRC_Y08_C08_422RPT
}; 

EXTERN BOOL fgDrvMJCSetFbckQTable(UINT8 u1TblIdx, const UINT16 u2Array[MJC_FB_QUANT_TBL_NUM]);
EXTERN BOOL fgDrvMJCSetFbckQMap(UINT8 u1TblIdx, const UINT8 u1MapArray[MJC_FB_QUANT_TBL_NUM]);

EXTERN UINT8 arMJCQuantMap[2][MJC_FB_QUANT_TBL_NUM];

UINT32 u4AutoTestCnt = 0;

/*----------------------------------------------------------------------------*
 * Functions
 *----------------------------------------------------------------------------*/
void MJC_SetOnOff(UINT8 u1OnOff)
{
    UINT32 u4IntMsk = 0;

    if (fgDrvMJCIsInitialized() == SV_FALSE)
        return;

    if (arMJCPrm.u1OnOff != u1OnOff)
    {
        arMJCPrm.u1OnOff = u1OnOff;

        vDrvMJCSetDRAMAccessOnOff(SV_OFF, SV_FALSE);
        fgDrvSetMJCToOSTG(u1OnOff != E_MJC_BYPASS);
        bDrvUpdateInternalFrameRate();

        if (u1OnOff == E_MJC_ON)
        {
            vDrvMJCSetDRAMAccessOnOff(SV_ON, arMJCPrm.u1Dram444);
            if (arMJCPrm.u1Dram444 == SV_FALSE)
            {
                vDrvMJCSetPSAutoOnOff(SV_ON);
            }
        }
        else // u1OnOff == E_MJC_BYPASS
        {
            vDrvMJCSetPSAutoOnOff(SV_OFF);
            u4IntMsk = 0xF;
        }

        #ifdef CC_FPGA
        u4IntMsk = 0xF;
        #endif
        MJC_WRITE_FLD(MJC_SYS_00, !u1OnOff, MJC_BYPASS_MJC);
        MJC_WRITE_FLD(MJC_SYS_02, u4IntMsk, MJC_INT_MASK);
		//vRegWriteFldAlign(OSTG_01, (u1OnOff == SV_ON), REG_MJC_IN_EN);
    }
}

void MJC_SetEffect(UINT32 u4Effect)
{
    UINT8 u1FbckCtrl = 0;
    UINT8 u1ForceFbck = 0;
    E_MJC_EFFECT eMJCEffect;
	
    vIO32WriteFldAlign(MJC_FBCK_01, 1, CUST_TGT_TBL_IDX);	
    if (u4Effect == u1MJCEffectLevel[E_MJC_EFFECT_OFF])
    {
        // off == effect
        eMJCEffect = E_MJC_EFFECT_LOW; //E_MJC_EFFECT_OFF;
        u1ForceFbck = 1;
        u1FbckCtrl = 0;
    }
    else if (u4Effect <= u1MJCEffectLevel[E_MJC_EFFECT_LOW])
    {
        // off < effect <= low
        eMJCEffect = E_MJC_EFFECT_LOW;
        u1FbckCtrl = MJC_FB_FLAG_NRMVSI | MJC_FB_FLAG_BDRVSI | MJC_FB_FLAG_BADMOTCNT | MJC_FB_FLAG_LRGV;
        vIO32Write4B(MJC_FBCK_32, 0x11111111);
        vIO32Write4B(MJC_FBCK_33, 0x1C161111);
        vIO32Write4B(MJC_FBCK_34, 0x36322C24);
        vIO32Write4B(MJC_FBCK_35, 0x403E3C3A);
        vIO32WriteFldAlign(MJC_FBCK_01, 1, CUST_TGT_MAP_WRITE);
    }
    else if (u4Effect <= u1MJCEffectLevel[E_MJC_EFFECT_MIDDLE])
    {
        // low < effect <= mid
        eMJCEffect = E_MJC_EFFECT_MIDDLE;
        u1FbckCtrl = MJC_FB_FLAG_NRMVSI | MJC_FB_FLAG_BDRVSI | MJC_FB_FLAG_BADMOTCNT | MJC_FB_FLAG_LRGV;
        vIO32Write4B(MJC_FBCK_32, 0x08000000);
        vIO32Write4B(MJC_FBCK_33, 0x1C16100C);
        vIO32Write4B(MJC_FBCK_34, 0x36322C24);
        vIO32Write4B(MJC_FBCK_35, 0x403E3C3A);
        vIO32WriteFldAlign(MJC_FBCK_01, 1, CUST_TGT_MAP_WRITE);
    }
    else
    {
        // mid < effect <= high
        eMJCEffect = E_MJC_EFFECT_HIGH;
        u1FbckCtrl = MJC_FB_FLAG_BDRVSI |MJC_FB_FLAG_LRGV;
        vIO32Write4B(MJC_FBCK_32, 0x04000000);
        vIO32Write4B(MJC_FBCK_33, 0x1110100A);
        vIO32Write4B(MJC_FBCK_34, 0x18111111);
        vIO32Write4B(MJC_FBCK_35, 0x40201818);
        vIO32WriteFldAlign(MJC_FBCK_01, 1, CUST_TGT_MAP_WRITE);
    }

    arMJCPrm.eMjcEffect = eMJCEffect;
    arMJCPrm.u1EffectParam = u4Effect;
    vIO32WriteFldAlign(MJC_FBCK_00, u1FbckCtrl, FBCK_CTRL);

    vIO32WriteFldAlign(MJC_FBCK_00, (u1ForceFbck?64:0), FBCK_LEVEL);
    vIO32WriteFldAlign(MJC_FBCK_00, u1ForceFbck, FBCK_DEBUG);
    //vIO32WriteFldAlign(MJC_FBCK_13, (u1ForceFbck?0:(eMJCEffect==E_MJC_EFFECT_LOW?7:0)), FBCK_MTHD_32);
    //vIO32WriteFldAlign(MJC_FBCK_13, (eMJCEffect==E_MJC_EFFECT_LOW?2:0), FBCK_MTHD_22);

    vDrvMJCUpdateIntpCtrl();
}

UINT8 arQMapDjdr[2][MJC_FB_QUANT_TBL_NUM] =
{
    {// mid
        0x00, 0x02, 0x04, 0x08, 0x0C, 0x10, 0x16, 0x1C,
        0x24, 0x2C, 0x32, 0x36, 0x3A, 0x3C, 0x3E, 0x40
    },
    {// max
        0x00, 0x00, 0x00, 0x04, 0x0A, 0x10, 0x10, 0x11,
        0x11, 0x11, 0x11, 0x18, 0x18, 0x18, 0x20, 0x40
    }
};

void MJC_SetDejudderLvl(UINT32 u4Lvl)
{
    UINT32 i = 0;
    UINT8 u1FbckCtrl = 0;
    E_MJC_EFFECT eMJCEffect;
    
    // u4Lvl = 0 (min) ~ 8 (mid) ~ 16 (max)
    if (u4Lvl <= 8)
    {
        if (u4Lvl == 0)
        {
            eMJCEffect = E_MJC_EFFECT_OFF;
            u1FbckCtrl = 0;
        }
        else
        {
            eMJCEffect = E_MJC_EFFECT_LOW;
            u1FbckCtrl = MJC_FB_FLAG_NRMVSI | MJC_FB_FLAG_BDRVSI | MJC_FB_FLAG_BADMOTCNT | MJC_FB_FLAG_LRGV;
        }
        
        for (i = 0; i < 16; i++)
        {
            arMJCQuantMap[1][i] = LERP(arQMapDjdr[0][i], 64, u4Lvl, 3);
        }
    }
    else // u4Lvl > 8
    {
        if (u4Lvl <= 12)
        {
            eMJCEffect = E_MJC_EFFECT_MIDDLE;
            u1FbckCtrl = MJC_FB_FLAG_NRMVSI | MJC_FB_FLAG_BDRVSI | MJC_FB_FLAG_BADMOTCNT | MJC_FB_FLAG_LRGV;
        }
        else
        {
            eMJCEffect = E_MJC_EFFECT_HIGH;
            u1FbckCtrl = MJC_FB_FLAG_BDRVSI | MJC_FB_FLAG_LRGV;
        }
    
        for (i = 0; i < 16; i++)
        {
            arMJCQuantMap[1][i] = LERP(arQMapDjdr[1][i], arQMapDjdr[0][i], (u4Lvl-8), 3);
        }
    }

    arMJCPrm.eMjcEffect = eMJCEffect;
    arMJCPrm.u1EffectParam = u1MJCEffectLevel[eMJCEffect];
    vIO32WriteFldAlign(MJC_FBCK_00, u1FbckCtrl, FBCK_CTRL);

    vDrvMJCUpdateIntpCtrl();
}

UINT8 arQMapDblr[2][MJC_FB_QUANT_TBL_NUM] =
{
    {// mid
        0x00, 0x02, 0x04, 0x08, 0x0C, 0x10, 0x16, 0x1C,
        0x24, 0x2C, 0x32, 0x36, 0x3A, 0x3C, 0x3E, 0x40
    },
    {// max
        0x00, 0x00, 0x00, 0x04, 0x0A, 0x10, 0x10, 0x11,
        0x11, 0x11, 0x11, 0x18, 0x18, 0x18, 0x20, 0x40
    }
};
void MJC_SetDeblurLvl(UINT32 u4Lvl)
{
    UINT32 i = 0;
    UINT8 u1FbckCtrl = 0;
    E_MJC_EFFECT eMJCEffect;
    
    // u4Lvl = 0 (min) ~ 8 (mid) ~ 16 (max)
    if (u4Lvl <= 8)
    {
        if (u4Lvl == 0)
        {
            eMJCEffect = E_MJC_EFFECT_OFF;
            u1FbckCtrl = 0;
        }
        else
        {
            eMJCEffect = E_MJC_EFFECT_LOW;
            u1FbckCtrl = MJC_FB_FLAG_NRMVSI | MJC_FB_FLAG_BDRVSI | MJC_FB_FLAG_BADMOTCNT | MJC_FB_FLAG_LRGV;
        }
        
        for (i = 0; i < 16; i++)
        {
            arMJCQuantMap[0][i] = LERP(arQMapDblr[0][i], 64, u4Lvl, 3);
        }
    }
    else // u4Lvl > 8
    {
        if (u4Lvl <= 12)
        {
            eMJCEffect = E_MJC_EFFECT_MIDDLE;
            u1FbckCtrl = MJC_FB_FLAG_NRMVSI | MJC_FB_FLAG_BDRVSI | MJC_FB_FLAG_BADMOTCNT | MJC_FB_FLAG_LRGV;
        }
        else
        {
            eMJCEffect = E_MJC_EFFECT_HIGH;
            u1FbckCtrl = MJC_FB_FLAG_BDRVSI | MJC_FB_FLAG_LRGV;
        }
    
        for (i = 0; i < 16; i++)
        {
            arMJCQuantMap[0][i] = LERP(arQMapDblr[1][i], arQMapDblr[0][i], (u4Lvl-8), 3);
        }
    }

    arMJCPrm.eMjcEffect = eMJCEffect;
    arMJCPrm.u1EffectParam = u1MJCEffectLevel[eMJCEffect];
    vIO32WriteFldAlign(MJC_FBCK_00, u1FbckCtrl, FBCK_CTRL_VDO);

    vDrvMJCUpdateIntpCtrl();
}

void MJC_SetClearLCD(UINT32 u4OnOff)
{
    if (!u4OnOff)
    {
        //arMJCPrm.u1CtrlParam |= ( MJC_CTRL_RT_HFBACK |  MJC_CTRL_LT_HFBACK);
        MJC_SetDemo(MJC_DEMO_OFF, MJC_INTP_HALF, MJC_INTP_HALF);
    }
    else
    {
        //arMJCPrm.u1CtrlParam &= (~MJC_CTRL_RT_HFBACK & ~MJC_CTRL_LT_HFBACK);
        MJC_SetDemo(MJC_DEMO_OFF, MJC_INTP_FULL, MJC_INTP_FULL);
    }
}

void MJC_SetDemo(UINT32 u4Mode, UINT32 u4Param1, UINT32 u4Param2)
{
    MJC_DEMO_CFG_T rDemo;

    // set up and position
    rDemo.u2BndTop = arMJCPrm.arPicSize.u2InHeight / 2;
    rDemo.u2BndBottom = rDemo.u2BndTop * 2;
    rDemo.u2BndLeft = arMJCPrm.arPicSize.u2InWidth / 2;
    rDemo.u2BndRight = rDemo.u2BndLeft * 2;

    rDemo.arRgn[2] = E_MJC_RGN_NF;
    
    switch (u4Mode)
    {
    default:
    case MJC_DEMO_OFF:
        rDemo.eDemoMode = E_MJC_DEMO_FULL;
        rDemo.arRgn[0] = rDemo.arRgn[1] =
            (u4Param1 == MJC_INTP_OFF ? E_MJC_RGN_RPT : (u4Param1 == MJC_INTP_HALF ? E_MJC_RGN_HF : E_MJC_RGN_NF));
        break;
    case MJC_DEMO_SIDEBYSIDE:
        rDemo.eDemoMode = E_MJC_DEMO_COLUMN;
        if (u4Param1 == MJC_INTP_OFF)
        {
            rDemo.arRgn[0] = E_MJC_RGN_RPT;
        }
        else if (u4Param1 == MJC_INTP_HALF)
        {
            rDemo.arRgn[0] = E_MJC_RGN_HF;
        }
        else
        {
            rDemo.arRgn[0] = E_MJC_RGN_NF;
        }

        if (u4Param2 == MJC_INTP_OFF)
        {
            rDemo.arRgn[1] = E_MJC_RGN_RPT;
        }
        else if (u4Param2 == MJC_INTP_HALF)
        {
            rDemo.arRgn[1] = E_MJC_RGN_HF;
        }
        else
        {
            rDemo.arRgn[1] = E_MJC_RGN_NF;
        }
        break;
    case MJC_DEMO_WINDOW:
        rDemo.eDemoMode = E_MJC_DEMO_WINDOW;
        rDemo.u2BndTop = arMJCPrm.arPicSize.u2InHeight / 3;
        rDemo.u2BndBottom = rDemo.u2BndTop * 2;
        rDemo.u2BndLeft = arMJCPrm.arPicSize.u2InWidth / 3;
        rDemo.u2BndRight = rDemo.u2BndLeft * 2;
        if (u4Param1 == MJC_INTP_OFF) // Outside
        {
            rDemo.arRgn[0] = E_MJC_RGN_NF;
            rDemo.arRgn[1] = E_MJC_RGN_RPT;
        }
        else
        {
            rDemo.arRgn[0] = E_MJC_RGN_RPT;
            rDemo.arRgn[1] = E_MJC_RGN_NF;
        }
        break;
    }

    vDrvMJCSetDemoCfg(&rDemo);
}

void MJC_SetDemoNew(E_MJC_NEW_DEMO_MODE eMode, UINT32 u4Param) //u4Param == 0 for two window, u4Param == 1 for three window
{
    MJC_DEMO_CFG_T rDemo;
	UINT8 u1Dum;

    if (eMode >= E_NEW_DEMO_MAX)
        return;

    // set up mode and position
    u1Dum = ((u4Param != 0) || (eMode == E_NEW_DEMO_WINDOW_INSIDE) || (eMode == E_NEW_DEMO_WINDOW_OUTSIDE)) ? 3 : 2;
    rDemo.u2BndTop = arMJCPrm.arPicSize.u2InHeight / u1Dum;
    rDemo.u2BndBottom = rDemo.u2BndTop * 2;
    rDemo.u2BndLeft = arMJCPrm.arPicSize.u2InWidth / u1Dum;
    rDemo.u2BndRight = rDemo.u2BndLeft * 2;
    
    switch (eMode)
    {
    default:
    case E_NEW_DEMO_OFF:
        rDemo.eDemoMode = E_MJC_DEMO_FULL;
        rDemo.u2BndLeft = rDemo.u2BndRight = rDemo.u2BndTop = rDemo.u2BndBottom = 0;
        break;
    case E_NEW_DEMO_COL_LEFT_120:
    case E_NEW_DEMO_COL_RIGHT_120:
    case E_NEW_DEMO_COL_LEFT_240:
    case E_NEW_DEMO_COL_RIGHT_240:
        rDemo.eDemoMode = E_MJC_DEMO_COLUMN;
        break;
    case E_NEW_DEMO_ROW_TOP_120:
    case E_NEW_DEMO_ROW_BOT_120:
    case E_NEW_DEMO_ROW_TOP_240:
    case E_NEW_DEMO_ROW_BOT_240:
        rDemo.eDemoMode = E_MJC_DEMO_ROW;
        break;
    case E_NEW_DEMO_WINDOW_INSIDE:
    case E_NEW_DEMO_WINDOW_OUTSIDE:
        rDemo.eDemoMode = E_MJC_DEMO_WINDOW;
        break;
    }

    // setup region
    switch (eMode)
    {
    default:
    case E_NEW_DEMO_OFF:
        rDemo.arRgn[0] = rDemo.arRgn[1] = rDemo.arRgn[2] = E_MJC_RGN_NF;
        break;
    case E_NEW_DEMO_COL_LEFT_120:
    case E_NEW_DEMO_ROW_TOP_120:
        rDemo.arRgn[0] = E_MJC_RGN_NF;
        rDemo.arRgn[1] = (u4Param == 0)? E_MJC_RGN_RPT  : E_MJC_RGN_HF;
        rDemo.arRgn[2] = E_MJC_RGN_RPT;
        break;
    case E_NEW_DEMO_COL_RIGHT_120:
    case E_NEW_DEMO_ROW_BOT_120:
        rDemo.arRgn[0] = E_MJC_RGN_RPT;
        rDemo.arRgn[1] = (u4Param == 0)?  E_MJC_RGN_NF : E_MJC_RGN_HF;
        rDemo.arRgn[2] = E_MJC_RGN_NF;
        break;
    case E_NEW_DEMO_COL_LEFT_240:
    case E_NEW_DEMO_ROW_TOP_240:
        rDemo.arRgn[0] = E_MJC_RGN_NF;
        rDemo.arRgn[1] = E_MJC_RGN_HF;
        rDemo.arRgn[2] = E_MJC_RGN_QF;
        break;
    case E_NEW_DEMO_COL_RIGHT_240:
    case E_NEW_DEMO_ROW_BOT_240:
        rDemo.arRgn[0] = (u4Param == 0)? E_MJC_RGN_HF : E_MJC_RGN_QF;
        rDemo.arRgn[1] = (u4Param == 0)? E_MJC_RGN_NF : E_MJC_RGN_HF;
        rDemo.arRgn[2] = E_MJC_RGN_NF;
        break;
    case E_NEW_DEMO_WINDOW_INSIDE:
        rDemo.arRgn[0] = E_MJC_RGN_NF;
        rDemo.arRgn[1] = E_MJC_RGN_RPT;
        rDemo.arRgn[2] = E_MJC_RGN_NF;
        break;
    case E_NEW_DEMO_WINDOW_OUTSIDE:
        rDemo.arRgn[0] = E_MJC_RGN_RPT;
        rDemo.arRgn[1] = E_MJC_RGN_NF;
        rDemo.arRgn[2] = E_MJC_RGN_NF;
        break;
    }

    vDrvMJCSetDemoCfg(&rDemo);
}

void MJC_SetBypassWindow(UINT8 u1Window, UINT32 u4Width, UINT32 u4Height, UINT32 u4StartX, UINT32 u4StartY)
{
    vDrvMJCSetOSDMask(u1Window, u4StartX, u4StartY, u4StartX + u4Width, u4StartY + u4Height);
}

void MJC_SetDemoBar(UINT8 fgOnOff, UINT8 r, UINT8 g, UINT8 b)
{
    vDrvMJCSetDemoBar(fgOnOff, r, g, b);
}

void MJC_SetMCOnOff(UINT8 fgOnOff)
{
    vDrvMJCSetMCOnOff(fgOnOff);
}

void vDrvMJCSetRemapDFT(UINT8 u1Coring, UINT8 u1Offset, UINT8 u1Weight)
{
    _rRemapDft.u1Coring = u1Coring;
    _rRemapDft.u1Offset = u1Offset;
    _rRemapDft.u1Weight = u1Weight;
}

UINT8 u1DrvMJCFbckLvlRemapDFT(UINT8 u1Lvl, UINT8 u1Effect)
{
    UINT32 u4OutLvl;
    UINT32 coring, offset, weight;

    coring = _rRemapDft.u1Coring;
    offset = _rRemapDft.u1Offset;
    weight = _rRemapDft.u1Weight;

    if ((UINT32) u1Lvl < coring)
    {
        u4OutLvl = (UINT32) u1Lvl + offset - (offset * u1Lvl / coring);
    }
    else
    {
        u4OutLvl = (UINT32) u1Lvl + ((255 - u1Effect) * weight * (u1Lvl - coring) >> 14);
    }
    
    return u4OutLvl;
}


UINT8 u1DrvMJCFbckLvlRemap2Sgmt(UINT8 in, UINT8 u1Effect)
{
    UINT32 out;
    UINT32 x, y0, y1, y2, y3;

    y0 = _rRemap2Seg.u1Y0;
    y1 = _rRemap2Seg.u1Y1;
    y2 = _rRemap2Seg.u1Y2;
    y3 = _rRemap2Seg.u1Y3;
    x = _rRemap2Seg.u1X;

    switch (arMJCPrm.eMjcEffect)
    {
    default:
    case E_MJC_EFFECT_HIGH:
        {
            UINT32 u4Tmp1 = u1MJCEffectLevel[E_MJC_EFFECT_HIGH] - u1MJCEffectLevel[E_MJC_EFFECT_MIDDLE];
            UINT32 u4Tmp2 = u1MJCEffectLevel[E_MJC_EFFECT_HIGH] - u1Effect;
            y3 = y3 + ((64-y3)*u4Tmp2) / u4Tmp1;
            y0 = 0;
        }
        break;
    case E_MJC_EFFECT_MIDDLE:
        {
            UINT32 u4Tmp1 = u1MJCEffectLevel[E_MJC_EFFECT_MIDDLE] - u1MJCEffectLevel[E_MJC_EFFECT_LOW];
            UINT32 u4Tmp2 = u1MJCEffectLevel[E_MJC_EFFECT_MIDDLE] - u1Effect;
            y3 = 64;
            y0 = y0 + ((y1-y0)*u4Tmp2) / u4Tmp1;
        }
        break;
    case E_MJC_EFFECT_LOW:
        {
            UINT32 u4Tmp1 = u1MJCEffectLevel[E_MJC_EFFECT_LOW] - u1MJCEffectLevel[E_MJC_EFFECT_OFF];
            UINT32 u4Tmp2 = u1MJCEffectLevel[E_MJC_EFFECT_LOW] - u1Effect;
            y3 = 64;
            y2 = y2 + ((64-y2)*u4Tmp2) / u4Tmp1;
            y0 = y2;
            y1 = y2;
        }
        break;
    case E_MJC_EFFECT_OFF:
        y0 = y1 = y2 = y3 = 64;
        break;
    }

    if (in < x)
    {
        ASSERT(x != 0);
        out = y0 + ((y1-y0)*in) / x;
    }
    else if (in < 64) // x <= in < 64
    {
        ASSERT(x < 64);
        out = y2 + ((y3-y2)*(in-x)) / (64-x);
    }
    else // in >= 64
    {
        out = y3;
    }

    return out;
}

UINT8 u1DrvMJCFbckLvlRemapLUT(UINT8 in, UINT8 u1Effect)
{
    UNUSED(u1Effect);
    return _u1FbckRmpTbl[in];
}


void MJC_SetEffectRemapMode(E_MJC_EFFECT_REMAP_MODE eMode)
{
    switch (eMode)
    {
    case E_MJC_EFFECT_MODE_NO_REMAP:
        vDrvSetFallbackRemapFunctor(NULL);
        break;
    case E_MJC_EFFECT_MODE_DFT:
        vDrvSetFallbackRemapFunctor(u1DrvMJCFbckLvlRemapDFT);
        break;
    case E_MJC_EFFECT_MODE_2SEGMENT:
        vDrvSetFallbackRemapFunctor(u1DrvMJCFbckLvlRemap2Sgmt);
        break;
    case E_MJC_EFFECT_MODE_LUT:
        vDrvSetFallbackRemapFunctor(u1DrvMJCFbckLvlRemapLUT);
        break;
    }
}


void MJC_SetPicSizeDbg(const MJC_PIC_SIZE_T * prMjcPicSize)
{
    if (prMjcPicSize == NULL)   // restore the original setting
    {
        vDrvMJCSetPicSize(&(arMJCPrm.arPicSize));
    }
    else
    {
        vDrvMJCSetPicSize(prMjcPicSize);
    }
}

void MJC_SetFRCDbg(MJC_FRC_CONFIG_T * prMJCFRC)
{
    if (prMJCFRC == NULL)   // restore the original setting
    {
        vDrvMJCSetFrmSch(&(arMJCPrm.arFRCConfig));
    }
    else
    {
        vDrvMJCSetFrmSch(prMJCFRC);
    }
}

void MJC_SetFrmSchDbg(UINT32 u4Mode)
{
    MJC_FRC_CONFIG_T rMJCFrc;

    rMJCFrc = arMJCPrm.arFRCConfig;

    if (u4Mode < 4)
    {
        rMJCFrc.u1FrmSchMode = u4Mode;
    }

    vDrvMJCSetFrmSch(&rMJCFrc);
}

void MJC_SetBufDbg(const MJC_DRAM_ALLOC_T * prMJCDram)
{
}

void MJC_SetDramModeDbg(UINT32 u4DramModeIdx)
{
    MJC_DRAM_ALLOC_T rMJCDram;
    const MJC_DRAM_MODE_T * prMJCDramMode;

    if (u1DrvMJCIsOnOff() == SV_FALSE)
    {
        LOG(0, "MJC is bypassed!\n");
        return;
    }

    if (u4DramModeIdx >= MJC_DRAM_ALL_MODE_NUM)
    {
        LOG(0, "Restore DRAM Mode: (0x%02X)\n", arMJCDramAlloc.u2DebugMode);

        rMJCDram = arMJCDramAlloc;
        rMJCDram.u2Mode = arMJCDramAlloc.u2DebugMode;
        arMJCDramAlloc.u2DebugMode = 0xFFFF;
    }
    else
    {
        prMJCDramMode = rDrvMJCGetDramMode(u4DramModeIdx);
        LOG(0, "Set DRAM Mode: idx(%d), %s(0x%2X)\n",
            u4DramModeIdx,
            prMJCDramMode->strDramMode,
            prMJCDramMode->u2DramMode);

        rMJCDram = arMJCDramAlloc;
        rMJCDram.u2Mode = prMJCDramMode->u2DramMode;
        arMJCDramAlloc.u2DebugMode = arMJCDramAlloc.u2Mode;
        arMJCDramAlloc.u2Mode      = rMJCDram.u2Mode;
    }

    _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, u1TestDelayCnt, SV_TRUE);
    vDrvMJCSetDRAMAccessOnOff(SV_OFF, 0);    
    vDrvMJCSetSystemMode(rMJCDram.u2Mode);
    vDrvMJCSetDRAMAccessOnOff(SV_ON, (rMJCDram.u2Mode & MJC_444_MODE)? 1 : 0);
}

void MJC_SetRemapDFTDbg(UINT8 u1Coring, UINT8 u1Offset, UINT8 u1Weight)
{
    vDrvMJCSetRemapDFT(u1Coring, u1Offset, u1Weight);
}

void MJC_VrfDramMode(UINT32 u4DramModeIdx)
{
    u4DramModeIdx %= MJC_DRAM_VERF_MODE_NUM;
    
    MJC_SetDramModeDbg(_arMJCVerfDramModeIdx[u4DramModeIdx]);
}

void MJC_SetSystemModeAutoTest(void)
{
    UINT8 u1FrmSel;
    UINT8 u1RdcRscnSel;
    UINT8 u1FlipMrrSel;
    UINT8 u1BitSel;
    UINT8 u1ColorSel;

    UINT32 u4SystemMode = 0;

    u1FrmSel = u4AutoTestCnt % NFRM_MAX;
    u1RdcRscnSel = (u4AutoTestCnt / NFRM_MAX) % RDCRSCN_MAX;
    u1FlipMrrSel = (u4AutoTestCnt / (NFRM_MAX*RDCRSCN_MAX)) % FLPMRR_MAX;
    u1BitSel = (u4AutoTestCnt / (NFRM_MAX*RDCRSCN_MAX*FLPMRR_MAX)) % BIT_MAX;
    u1ColorSel = (u4AutoTestCnt / (NFRM_MAX*RDCRSCN_MAX*FLPMRR_MAX*BIT_MAX)) % COLOR_MAX;

    u4SystemMode = V_Fld(u1RdcRscnSel, MJC_RSCNRDC_SEL);

    if (u1FrmSel == 1)
        u4SystemMode |= MJC_1FRM_MODE;

    switch (u1BitSel)
    {
    case 0:
        u4SystemMode |= (MJC_Y10_MODE|MJC_C10_MODE);
        break;
    case 1:
        u4SystemMode |= (MJC_Y10_MODE|MJC_C08_MODE);
        break;
    default:
    case 2:
        u4SystemMode |= (MJC_Y08_MODE|MJC_C08_MODE);
        break;
    case 3:
        u4SystemMode |= (MJC_Y10_MODE|MJC_C10_MODE|MJC_DYN_MODE);
        break;
    case 4:
        u4SystemMode |= (MJC_Y10_MODE|MJC_C08_MODE|MJC_DYN_MODE);
        break;
    case 5:
        u4SystemMode |= (MJC_Y08_MODE|MJC_C08_MODE|MJC_DYN_MODE);
        break;
    }

    switch (u1ColorSel)
    {
    default:
    case 0:
        u4SystemMode |= MJC_422_MODE;
        break;
    case 1:
        u4SystemMode |= MJC_420_MODE;
        break;
    case 2:
        u4SystemMode |= (MJC_444_MODE|MJC_RPT_MODE);
        break;
    case 3:
        u4SystemMode |= (MJC_422_MODE|MJC_RPT_MODE);
        break;
    }

    vDrvMJCSetSystemMode(u4SystemMode);
    vDrvMJCSetFlipMirror((u1FlipMrrSel&2) >> 1, (u1FlipMrrSel&1));

    LOG(0, "Current Test Count = %d\n", u4AutoTestCnt);
    LOG(0, "u1Dyn = %d\n", arMJCPrm.u1Dyn);
    LOG(0, "u1Rpt = %d\n", arMJCPrm.u1Rpt);
    LOG(0, "u1Dram444 = %d\n", arMJCPrm.u1Dram444);
    LOG(0, "u1Dram420 = %d\n", arMJCPrm.u1Dram420);
    LOG(0, "u1Y8 = %d\n", arMJCPrm.u1Y8);
    LOG(0, "u1C8 = %d\n", arMJCPrm.u1C8);
    LOG(0, "u1YDithEn = %d\n", arMJCPrm.u1YDithEn);
    LOG(0, "u1CDithEn = %d\n", arMJCPrm.u1CDithEn);
    LOG(0, "u1Flip = %d\n", arMJCPrm.u1Flip);
    LOG(0, "u1Mirror = %d\n", arMJCPrm.u1Mirror);
    LOG(0, "u1TwoFrm = %d\n", arMJCPrm.u1TwoFrm);
    LOG(0, "u1OneFrm = %d\n", arMJCPrm.u1OneFrm);
    LOG(0, "u1Rdc = %d\n", arMJCPrm.u1Rdc);
    LOG(0, "u1Rscn = %d\n", arMJCPrm.u1Rscn);

    vDrvMJCDelaySet(E_MJC_DELAY_DM_BW_CHK, 60);
    vDrvMJCDelaySet(E_MJC_DELAY_DM_CRC_CHK, 60);
    vDrvMJCDelaySet(E_MJC_DELAY_DM_START, 20);

    u4AutoTestCnt ++;
}

void MJC_SetFilmModeTypeOnOff(UINT8 u1Path, UINT8 u1FilmType, UINT8 u1OnOff)
{
    if (u1Path == VDP_1)
    {
        switch (u1FilmType)
        {
    	    case 0:
                arMJCPrm.u1PSAuto22OnOff = u1OnOff;
                break;
    	    case 1:
                arMJCPrm.u1PSAuto32OnOff = u1OnOff;
                break;
            case 2:
                MJC_WRITE_FLD(MJC_ME_15, (u1OnOff ? 2 : 0), MJC_FW_FRM_SCH_SWITCH);
                break;
            default:
                break;
        }
    }    
}

void MJC_SetFilmMode(BOOL fgOnOff)
{
    if (fgOnOff)
    {
        arMJCPrm.u1CtrlParam |= MJC_CTRL_FM_ON;
        vDrvMJCSetPSAutoOnOff(SV_ON);
        vIO32WriteFldAlign(MJC_FBCK_13, 3, FBCK_MTHD_32);
        vIO32WriteFldAlign(MJC_FBCK_13, 0, FBCK_MTHD_22);
    }
    else
    {
        arMJCPrm.u1CtrlParam &= ~MJC_CTRL_FM_ON;
        vDrvMJCSetPSAutoOnOff(SV_OFF);
        vIO32WriteFldAlign(MJC_FBCK_13, 6, FBCK_MTHD_32);
        vIO32WriteFldAlign(MJC_FBCK_13, 3, FBCK_MTHD_22);
    }
    vIO32WriteFldAlign(MJC_MISC_01, arMJCPrm.u1CtrlParam, MJC_CTRL_PARAM);
    vIO32WriteFldAlign(MJC_FBCK_01, (fgOnOff?1:0), FBCK_55_MODE);
}

BOOL MJC_Set_FbckQTable(UINT8 u1TblIdx, const UINT16 u2Array[16])
{
    return fgDrvMJCSetFbckQTable(u1TblIdx, u2Array);
}

BOOL MJC_Set_FbckQMap(UINT8 u1TblIdx, const UINT8 u1MapArray[16])
{
    return fgDrvMJCSetFbckQMap(u1TblIdx, u1MapArray);
}

void MJC_SetLRSwitch(BOOL fgSwap)
{
    UINT8 u1LR3DDly;
    u1LR3DDly = arMJCPrm.u1DispRDly;
    arMJCPrm.fgDispRSwp = fgSwap;
    if(fgSwap)
    {
        MJC_WRITE_FLD(MJC_PR_00 , ((LVDS_DISP_3D == LVDS_DISP_3D_POLARIZED_RLRL)? 0 : 1), MJC_PR_LR_SWITCH);
	    if(LVDS_DISP_3D == LVDS_DISP_3D_SHUTTER)
	    {
            LOG(3,"MJC_SetLRSwitch(%d)::Set MJC LR 3D dely to (%d)\n",fgSwap,u1LR3DDly+1);
	        MJC_WRITE_FLD(MJC_SYS_D2, ((u1LR3DDly + 1) % 4), MJC_OUT_LR3D_DLY);
	    }
    }
    else
    {
        MJC_WRITE_FLD(MJC_PR_00 , ((LVDS_DISP_3D == LVDS_DISP_3D_POLARIZED_RLRL)? 1 : 0), MJC_PR_LR_SWITCH);
	    if(LVDS_DISP_3D == LVDS_DISP_3D_SHUTTER)
	    {
            LOG(3,"MJC_SetLRSwitch(%d)::Set MJC LR 3D dely to (%d)\n",fgSwap,((u1LR3DDly) % 4));
	        MJC_WRITE_FLD(MJC_SYS_D2, ((u1LR3DDly) % 4), MJC_OUT_LR3D_DLY);
	    }
    }
}


