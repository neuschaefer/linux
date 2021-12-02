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
 * $RCSfile: drv_mjc_int.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "hw_vdoin.h"
#include "drv_mjc.h"
#include "drv_mjc_int.h"
#include "drv_mjc_fbck.h"
#include "drv_mjc_if.h"
#include "hw_mjc.h"
#include "hw_sw.h"
#include "x_assert.h"

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
#define MIN_BLK_CNT 0x7E9

typedef struct
{
    UINT32 u4TgtSAD[2];
    UINT32 u4RefSAD[2];
} MJC_VERT_PAD_T;

static MJC_LBOX_STA_T _rMJCLboxSta;
static MJC_VERT_PAD_T _rMJCMVPad = {{0, 0}, {0, 0}};

/*
static CHAR _chMEOthr[4] = 
{
    'F', 'C', 'B', 'A'
};
*/

static UINT8 _u1IntpCtrlUpdate = 0xA;
static UINT8 _u1OutLR = 0;
static UINT8 _u1ClearMVCnt = 1;

UINT8 _u1AbsGmvY; // integer pixel
UINT8 _u1AbsGmvX; // integer pixel
UINT8 _u1AbsBgmvY; // integer pixel
UINT8 _u1AbsBgmvX; // integer pixel
UINT8 _u1AbsGmvYPdct; // integer pixel, predicted gmv for next frame
UINT8 _u1AbsGmvXPdct; // integer pixel, predicted gmv for next frame
INT16 _i2GmvY;
INT16 _i2GmvX;

UINT16  u2MajorCnt0;
UINT8   u1MajorMvX0;
UINT8   u1MajorMvY0;

UINT8  u1FbckLvl;
UINT16 u2StaNrmvsi;
UINT16 u2StaBdrvsi;
UINT16 u2MaxMvErr;
UINT16 u2BadMotCnt;
UINT16 u2OsdErr;
UINT16 u2LrgV;
UINT16 u2StaMEBec;
UINT16 u2StaCustVal;
UINT16 u2TotalCnt;
UINT16 u2TotalMotCnt;
UINT16 u2LBoxBlockCnt;
UINT16 u2ZeroBlkCnt = 0xFFFF;

UINT32 u4NormLboxFct = 1;
UINT32 u4NormTotalFct = 1;

static UINT32 _u4SwMeCtrl = 0;
UINT8 fgMCOnOff = 1;
UINT8 fgClearMVFb = 0;


/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
void vDrvMJCInitQuality(void)
{
    vDrvInitFbck();
	MJC_WRITE_FLD(MJC_ME_10, 0x2, MJC_ME_LPF_EDGE_TH); // for cr DTV00384092 [more edges patten]
    vIO32Write4B(MJC_SW_ME_00, 0x080C10BE);
    vIO32Write4B(MJC_SW_ME_01, 0x6090800F);
    vIO32Write4B(MJC_SW_ME_02, 0x20051E1C);
    vIO32Write4B(MJC_SW_ME_05, 0x15005180);
    vIO32Write4B(MJC_SW_ME_07, 0x0A050210);
    vIO32Write4B(MJC_SW_ME_09, 0x00000001);
    vIO32Write4B(MJC_TRIBALL_00, 0x0A0012F0);
    vIO32Write4B(MJC_TRIBALL_01, 0x01035200);
    vIO32Write4B(MJC_TRIBALL_02, 0x0000000A);
    vIO32Write4B(MJC_BADEDIT_00, 0x00048080);
    vIO32Write4B(MJC_ADPTOC_00, 0x00020366);
    vIO32Write4B(MJC_SW_MVMC_00, 0x425060A2);
    vIO32Write4B(MJC_SW_MVMC_01, 0x18000000);
}

UINT8 u1DrvMJCGetGmvX(void)
{
	return _u1AbsGmvX;
}

UINT8 u1DrvMJCGetGmvY(void)
{
	return _u1AbsGmvY;
}

#if 0
void vDrvMJCSetMVDS(UINT8 u1MvdsVal)
{
    if (u1MvdsVal >= 16)
    {
        MJC_WRITE_FLD_MULTI(MJC_MC_0F,
            P_Fld(0, MJC_MVDS_FRM_EN)|
            P_Fld(0xF, MJC_MVDS_FRM_X_GAIN)|
            P_Fld(0xF, MJC_MVDS_FRM_Y_GAIN));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_MC_0F,
            P_Fld(1, MJC_MVDS_FRM_EN)|
            P_Fld(u1MvdsVal, MJC_MVDS_FRM_X_GAIN)|
            P_Fld(u1MvdsVal, MJC_MVDS_FRM_Y_GAIN));
    }
}
#endif

static void _vDrvMJCUpdateMEStaTotalCnt(void)
{
    UINT32 u4Val = MJC_READ_4B(MJC_ME_06);

    UINT32 u4Blk =
        (((_rMJCLboxSta.u2PosDn - R_Fld(u4Val, MJC_ME_BND_DOWN)*8 + 4) >> 3) -
         ((_rMJCLboxSta.u2PosUp + R_Fld(u4Val, MJC_ME_BND_UP)*8 + 4) >> 3)) *
        (((_rMJCLboxSta.u2PosRt - R_Fld(u4Val, MJC_ME_BND_RIGHT)*8 + 4) >> 3) -
         ((_rMJCLboxSta.u2PosLf + R_Fld(u4Val, MJC_ME_BND_LEFT)*8 + 4) >> 3));

    vIO32WriteFldAlign(MJC_SW_ME_08, MIN(u4Blk, 0xFFFF), STA_MJC_ME_STA_TOTALBLK);
}

UINT32 u4DrvGetStaNormLbox(UINT32 u4Val)
{
    // normalized by LBox count, the normalized factor is limit to 0x20
    UINT32 u4NormVal = 0;

    if (IO32ReadFldAlign(MJC_FBCK_01, FBCK_NORM_FORCE_DIV) || (u4Val >= (1 << 27)))
    {
        u4NormVal = u4Val / ((UINT32) MAX(u2LBoxBlockCnt, MIN_BLK_CNT));
    }
    else
    {
        u4NormVal = (u4Val * u4NormLboxFct) >> 15;
    }

    return u4NormVal;
}

UINT32 u4DrvGetStaNormTotalCnt(UINT32 u4Val)
{
    // normalized by LBox count, the normalized factor is limit to 0x20
    UINT32 u4NormVal = 0;

    if (IO32ReadFldAlign(MJC_FBCK_01, FBCK_NORM_FORCE_DIV) || (u4Val >= (1 << 27)))
    {
        u4NormVal = u4Val / ((UINT32) MAX(u2TotalCnt, MIN_BLK_CNT));
    }
    else
    {
        u4NormVal = (u4Val * u4NormTotalFct) >> 15;
    }

    return u4NormVal;

}

static UINT32 _u4DrvGetCustVal(void)
{
    UINT32  u4CustAddr;
    UINT32  u4CustMsb;
    UINT32  u4CustLsb;
    UINT32  u4CustValue;

    u4CustAddr = IO32ReadFldAlign(MJC_FBCK_01, CUST_ADDR);
    u4CustMsb  = IO32ReadFldAlign(MJC_FBCK_01, CUST_MSB);
    u4CustLsb  = IO32ReadFldAlign(MJC_FBCK_01, CUST_LSB);
    u4CustValue = ((MJC_READ_4B(0x36000 + u4CustAddr)) >> u4CustLsb) & ((UINT32)0xffffffff>>(31-u4CustMsb+u4CustLsb));
    return u4CustValue;
}

static void _vDrvMJCBndHLRAdapWidth(UINT8 u1Enable)
{
    static UINT8 _u1Enable = 0xff;
    UINT32 u4BlkX = _u1AbsGmvX;
    UINT32 u4BlkY = _u1AbsGmvY;
    
    if (u1Enable == 1)
    {
        UINT32 u4MaxX, u4MaxY;
        u4MaxX = R_Fld(_u4SwMeCtrl, MJC_BND_HLR_MAX_H);
        u4MaxY = R_Fld(_u4SwMeCtrl, MJC_BND_HLR_MAX_V);

        u4BlkX = (u4BlkX*2 + 4) >> 3;
        u4BlkY = (u4BlkY*2 + 4) >> 3;
        
        u4BlkX = MAX(MIN(u4BlkX, u4MaxX), 8);
        u4BlkY = MAX(MIN(u4BlkY, u4MaxY), 4);
        
        MJC_WRITE_FLD(MJC_ME_19, u4BlkX, MJC_HLR_BND_H);
        MJC_WRITE_FLD(MJC_ME_0A, u4BlkY, MJC_HLR_BND_V);
    }
    else if ((u1Enable == 0) && _u1Enable)
    {
        MJC_WRITE_FLD(MJC_ME_19, 8, MJC_HLR_BND_H);
        MJC_WRITE_FLD(MJC_ME_0A, 4, MJC_HLR_BND_V);
    }

    _u1Enable = u1Enable;
}

static void _vDrvMJCHandleExceedSearchRng(void)
{
    static UINT8 _u1Enable = 0xff;
    UINT8 u1Enable = R_Fld(_u4SwMeCtrl, MJC_ME_EXD_SR_PATCH);
    
    if (u1Enable)
    {        
        UINT8 u1GmvY = R_Fld(_u4SwMeCtrl, MJC_ME_EXD_GMV_MODE) ? _u1AbsGmvYPdct : _u1AbsGmvY;
        UINT8 u1GmvTh = IO32ReadFldAlign(MJC_SW_ME_01, MJC_ME_EXD_SR_GMVY_TH);

        if (u1GmvY >= u1GmvTh)
        {
            MJC_WRITE_FLD(MJC_MV_0C, 0, MJC_GMV_BI_USE_TF);
            MJC_WRITE_FLD(MJC_PPC_01, 1, MJC_ME_PPC_DAR_TH);
        }
        else
        {
            MJC_WRITE_FLD(MJC_MV_0C, 1, MJC_GMV_BI_USE_TF);
            MJC_WRITE_FLD(MJC_PPC_01, IO32ReadFldAlign(MJC_SW_ME_02, MJC_ME_PPC_DAR_TH_DFT), MJC_ME_PPC_DAR_TH);
        }
    }
    else if ((u1Enable == 0) && _u1Enable)
    {
        // reset to default
        MJC_WRITE_FLD(MJC_MV_0C, 1, MJC_GMV_BI_USE_TF);
        MJC_WRITE_FLD(MJC_PPC_01, IO32ReadFldAlign(MJC_SW_ME_02, MJC_ME_PPC_DAR_TH_DFT), MJC_ME_PPC_DAR_TH);
    }

    _u1Enable = u1Enable;
}

static void _vDrvMJCAdapBEC(void)
{
    //static UINT16 u2Buf[4] = {0};
    //static UINT8 u1Idx = 0;
    UINT32 u4LocMax = 0;
    UINT32 u4CurBecSadTh = MJC_READ_FLD(MJC_ME_00, MJC_BEC_SAD_TH);
    
    //u2Buf[u1Idx] = IO32ReadFldAlign(MJC_SW_ME_04, MJC_AVG_LES);
    //u4LocMax = MAX(u2Buf[u1Idx], MAX(u2Buf[(u1Idx-1)&0x3], u2Buf[(u1Idx-2)&0x3]));
    u4LocMax = u4LocMax +
        ((IO32ReadFldAlign(MJC_SW_ME_04, MJC_AVG_MINVAR) * IO32ReadFldAlign(MJC_SW_ME_05, MJC_ADAP_BEC_VAR_GAIN)) >> IO32ReadFldAlign(MJC_SW_ME_05, MJC_ADAP_BEC_VAR_SFT));
    u4LocMax = MIN(MAX(u4LocMax, IO32ReadFldAlign(MJC_SW_ME_05, MJC_ADAP_BEC_SAD_TH_MIN)), IO32ReadFldAlign(MJC_SW_ME_05, MJC_ADAP_BEC_SAD_TH_MAX));

    if (u4CurBecSadTh < u4LocMax)
    {
        u4CurBecSadTh += (((u4LocMax - u4CurBecSadTh)*1) >> 4);    
    }
    else
    {
        u4CurBecSadTh -= (((u4CurBecSadTh - u4LocMax)*1) >> 4);   
    }
    
    u4CurBecSadTh = MIN(MAX(u4CurBecSadTh, IO32ReadFldAlign(MJC_SW_ME_05, MJC_ADAP_BEC_SAD_TH_MIN)), 0x7fff);

    //u1Idx = (u1Idx + 1) & 0x3;

    MJC_WRITE_FLD(MJC_ME_00, u4CurBecSadTh, MJC_BEC_SAD_TH);
}

static void _vDrvMJCStillClearMV(void)
{
    if (IO32ReadFldAlign(MJC_SW_ME_09, MJC_ME_STILL_CLR_MV) &&
        (u2ZeroBlkCnt >= IO32ReadFldAlign(MJC_SW_ME_08, STA_MJC_ME_STA_TOTALBLK)) &&
         (IO32ReadFldAlign(MJC_SW_ME_08, STA_MJC_AVG_ZEROSAD) < MJC_READ_FLD(MJC_ME_01, MJC_ZEROMV_SAD_TH)) &&
        !IO32ReadFldAlign(MJC_SW_ME_10, STA_MJC_IS_MOVLBOX))
    {
        if (IO32ReadFldAlign(MJC_SW_ME_09, MJC_ME_STILL_CLR_MV_FB) == SV_ON)
        {
            vDrvMJCSetClearMVTrig();
		}
		else
		{
            fgClearMVFb = 1;
		}
    }
	else
	{
	    if (IO32ReadFldAlign(MJC_SW_ME_09, MJC_ME_STILL_CLR_MV_FB) == SV_OFF)
        {
		    fgClearMVFb = 0;
	    }
	}
}

static void _vDrvMJCMovingLBoxProc(void)
{
    // Disable LBox auto set when LBOX is changing
    static MJC_LBOX_STA_T _rPrevLbox = {0};
    static UINT8 _u1Enable = 0xFF;
    static UINT8 _u1Cnt = 0;

    UINT32 u4Reg = u4IO32Read4B(MJC_SW_ME_07);
    UINT8 u1Enable = R_Fld(_u4SwMeCtrl, MJC_ME_LBOX_CHG_OFF);
    UINT8 u1Detected = 0;

    if (u1Enable)
    {
        UINT32 u4Percentage = u4IO32Read4B(ADAP_LUMA2_08);     // MJC moving box patch of mt5396 changelsit:903459 ,checked to cd lee ,it also do in Cobra
        u4Percentage = R_Fld(u4Percentage, ADL_BLACK_PERCENTAGE_2) + R_Fld(u4Percentage, ADL_WHITE_PERCENTAGE_2);
        
        if (MJC_READ_FLD(MJC_STA_FM_02, MJC_STA_PAIR_FIRST_HW))
        {
            if (AbsDiff(_rMJCLboxSta.u2PosLf, _rPrevLbox.u2PosLf) > R_Fld(u4Reg, MJC_ME_LBOX_CHG_DIF_TH) ||
                AbsDiff(_rMJCLboxSta.u2PosRt, _rPrevLbox.u2PosRt) > R_Fld(u4Reg, MJC_ME_LBOX_CHG_DIF_TH) ||
                AbsDiff(_rMJCLboxSta.u2PosUp, _rPrevLbox.u2PosUp) > R_Fld(u4Reg, MJC_ME_LBOX_CHG_DIF_TH) ||
                AbsDiff(_rMJCLboxSta.u2PosDn, _rPrevLbox.u2PosDn) > R_Fld(u4Reg, MJC_ME_LBOX_CHG_DIF_TH) ||
                u4Percentage >= 0xF8)
            {
                _u1Cnt += ((_u1Cnt < R_Fld(u4Reg, MJC_ME_LBOX_CHG_CNT_UP)) ? 1 : 0);
            }
            else
            {
                _u1Cnt -= ((_u1Cnt > 0) ? 1 : 0);
            }

            _rPrevLbox = _rMJCLboxSta;
        }

        if (_u1Cnt >= R_Fld(u4Reg, MJC_ME_LBOX_CHG_CNT_HI))
        {
            u1Detected = 1;
            MJC_WRITE_FLD(MJC_FB_LBOX_0, 0, MJC_LBOX_AUTO_SET);
            MJC_WRITE_FLD(MJC_MV_2E, 1, MJC_MV_BYPASS_MVC);
            MJC_WRITE_FLD(MJC_ME_19, 0, MJC_NOZERO_MV_EN);
        }
        else if (_u1Cnt < R_Fld(u4Reg, MJC_ME_LBOX_CHG_CNT_LO))
        {
            MJC_WRITE_FLD(MJC_FB_LBOX_0, 1, MJC_LBOX_AUTO_SET);
            MJC_WRITE_FLD(MJC_MV_2E, 0, MJC_MV_BYPASS_MVC);
            MJC_WRITE_FLD(MJC_ME_19, 1, MJC_NOZERO_MV_EN);
        }
    }
    else if ((u1Enable == 0) && _u1Enable)
    {
        // default
        MJC_WRITE_FLD(MJC_FB_LBOX_0, 1, MJC_LBOX_AUTO_SET);
        MJC_WRITE_FLD(MJC_MV_2E, 0, MJC_MV_BYPASS_MVC);
        MJC_WRITE_FLD(MJC_ME_19, 1, MJC_NOZERO_MV_EN);
    }

    vIO32WriteFldAlign(MJC_SW_ME_10, u1Detected, STA_MJC_IS_MOVLBOX);

    _u1Enable = u1Enable;
}

static void _vDrvMJCSetMELBoxSA(E_MJC_LBOX_ME_SA eMjcLboxMeSa, UINT32 u4GmvTh)
{
    static E_MJC_LBOX_ME_SA eMjcLboxMeSaPrev = E_ME_LBOX_SA_AUTO;

    if (eMjcLboxMeSa == E_ME_LBOX_SA_AUTO)
    {
        UINT32 u4Tmp1, u4Tmp2 = 0;

        // top and bottom letter box
        u4Tmp1 = _rMJCLboxSta.u2PosUp & 0x7; // line pos % 8
        if (u4Tmp1 >= 4 && _u1AbsGmvY < u4GmvTh)
        {
            u4Tmp2 |= ((4-u4Tmp1-1) & 0x000F);
        }

        u4Tmp1 = _rMJCLboxSta.u2PosDn & 0x7; // line pos % 8
        if (u4Tmp1 < 4 && _u1AbsGmvY < u4GmvTh)
        {
            u4Tmp2 |= (((u4Tmp1-4) << 4) & 0x00F0);
        }

        // todo: left and right
        u4Tmp1 = _rMJCLboxSta.u2PosLf & 0x7; // pix pos % 8
        if (u4Tmp1 >= 4 && _u1AbsGmvX < u4GmvTh)
        {
            u4Tmp2 |= (((4-u4Tmp1) << 8) & 0x0F00);
        }

        u4Tmp1 = _rMJCLboxSta.u2PosRt & 0x7; // pix pos % 8
        if (u4Tmp1 < 4 && _u1AbsGmvX < u4GmvTh)
        {
            u4Tmp2 |= (((u4Tmp1-6) << 12) & 0xF000);
        }

        MJC_WRITE_FLD(MJC_FB_LBOX_4, u4Tmp2, MJC_LBOX_SA_ME_ALL);
    }
    else if (eMjcLboxMeSa == E_ME_LBOX_SA_MANUAL)
    {
        if (eMjcLboxMeSa != eMjcLboxMeSaPrev)
        {
            // trigger to set default value
            MJC_WRITE_FLD_MULTI(MJC_FB_LBOX_4,
                P_Fld(0, MJC_LBOX_SA_ME_RT)|
                P_Fld(0, MJC_LBOX_SA_ME_LF)|
                P_Fld(0, MJC_LBOX_SA_ME_DN)|
                P_Fld(0, MJC_LBOX_SA_ME_UP));
        }
    }
    else
    {
        ASSERT(0);
    }

    eMjcLboxMeSaPrev = eMjcLboxMeSa;
}

void vDrvMJCTmprPnltyProc(void)
{
    static UINT8 _u1PrevEn = 0xFF;
    
    UINT8 u1En = R_Fld(_u4SwMeCtrl, MJC_ME_TMPR_PNLTY_64_PATCH);
    
    UINT32 u4Tmp = MJC_READ_4B(MJC_STA_FM_02);
    UINT32 u4Idx = IO32ReadFldAlign(MJC_FBCK_11, MVDS_APPLY_DELAY) + 
                        ((R_Fld(u4Tmp, MJC_STA_ME_FILM_IDX_HW)) + ((R_Fld(u4Tmp, MJC_STA_ME_FILM_PHS_HW)) == 0 ? 6 : 0));

    if (u4Idx >= 10)
        u4Idx = u4Idx - 10;

    if (u1En == 1)
    {
        if (MJC_FM_32 == R_Fld(u4Tmp, MJC_STA_ME_FILM_FLG_HW) && (u4Idx == 6))
        {
            MJC_WRITE_FLD(MJC_ME_03, 0, MJC_TMPR_SAD_PENLTY_S);
            MJC_WRITE_FLD(MJC_ME_0A, 0, MJC_TMPR_SAD_PENLTY_M);
        }
        else
        {
            MJC_WRITE_FLD(MJC_ME_03, 0xC8, MJC_TMPR_SAD_PENLTY_S);
            MJC_WRITE_FLD(MJC_ME_0A, 0x88, MJC_TMPR_SAD_PENLTY_M);
        }
    }
    else if ((u1En == 0) && (_u1PrevEn != u1En))
    {
        MJC_WRITE_FLD(MJC_ME_03, 0xC8, MJC_TMPR_SAD_PENLTY_S);
        MJC_WRITE_FLD(MJC_ME_0A, 0x88, MJC_TMPR_SAD_PENLTY_M);
    }
    _u1PrevEn = u1En;
}

EXTERN void vDrvMJCSetProgME(UINT8 u1PanelType, UINT8 fgIs55p2);
void vDrvMJCMVEXPProc(void)
{
    static UINT8 u1Freerun = 0;
    static UINT8 _u1PrevEn = 0xFF;

    UINT8 u1MVEXP;

    UINT32 u4Idx_vdo;
    UINT32 u4Idx_32;
    UINT32 u4Idx_22;

    UINT32 u4Tmp = MJC_READ_4B(MJC_STA_FM_02);
    UINT32 u4Delay = IO32ReadFldAlign(MJC_FBCK_11, MVDS_APPLY_DELAY);

    u4Idx_32 = u4Delay + ((R_Fld(u4Tmp, MJC_STA_ME_FILM_IDX_HW)) + ((R_Fld(u4Tmp, MJC_STA_ME_FILM_PHS_HW)) == 0 ? 6 : 0));
                        
    u4Idx_22 = u4Delay + (R_Fld(u4Tmp, MJC_STA_ME_FILM_IDX_HW));

    if (u4Idx_32 >= 10)
    {
        u4Idx_32 = u4Idx_32 - 10;
    }

    if (u4Idx_22 >= 4)
    {
        u4Idx_22 = u4Idx_22 - 4;
    }

    u4Idx_vdo = u4Idx_22;
    if ((u4Idx_vdo & 1) == 0)
        u1Freerun = !u1Freerun;

    u4Idx_vdo = u4Idx_vdo + (u1Freerun ? 0 : 2);
    if (u4Idx_vdo >= 4)
    {
        u4Idx_vdo = u4Idx_vdo - 4;
    }

    u1MVEXP = IO32ReadFldAlign(MJC_MISC_01, MJC_FW_MVEXP_EN);
    u1MVEXP = (R_Fld(u4Tmp, MJC_STA_ME_FILM_FLG_HW) == MJC_FM_VDO) ?
        (u1MVEXP && IO32ReadFldAlign(MJC_MISC_01, MJC_VDO_ME2ME3)) : u1MVEXP;
    
    if (u1MVEXP == 1)
    {
        MJC_WRITE_FLD(MJC_FM_15, 1, MJC_FW_FRM_MVEXP);

        if (R_Fld(u4Tmp, MJC_STA_ME_FILM_FLG_HW) == MJC_FM_32)
        {
            if ((u4Idx_32 == 1) || (u4Idx_32 == 6))
            {
                MJC_WRITE_FLD(MJC_FM_06, 1, MJC_FW_MVEXP_FLAG);
            }
            else
            {
                MJC_WRITE_FLD(MJC_FM_06, 0, MJC_FW_MVEXP_FLAG);
            }
        }
        else if (R_Fld(u4Tmp, MJC_STA_ME_FILM_FLG_HW) == MJC_FM_22)
        {
            if (u4Idx_22 == 1) 
            {
                MJC_WRITE_FLD(MJC_FM_06, 1, MJC_FW_MVEXP_FLAG);
            }
            else
            {
                MJC_WRITE_FLD(MJC_FM_06, 0, MJC_FW_MVEXP_FLAG);
            }
        }
        else   //(u1FilmStatus == MJC_FM_VDO)
        {
            if (u1Freerun == 0)
            {
                MJC_WRITE_FLD(MJC_FM_06, ((u4Idx_vdo == 3) ? 1 : 0), MJC_FW_MVEXP_FLAG);
                MJC_WRITE_FLD_MULTI(MJC_FM_02, P_Fld(1, MJC_PROG_ME2EN_0_4)|P_Fld(0, MJC_PROG_ME2EN_1_4));
                MJC_WRITE_FLD_MULTI(MJC_FM_03, P_Fld(0, MJC_PROG_ME3EN_0_4)|P_Fld(1, MJC_PROG_ME3EN_1_4));
            }
            else 
            {
                MJC_WRITE_FLD(MJC_FM_06, 0, MJC_FW_MVEXP_FLAG);
                MJC_WRITE_FLD_MULTI(MJC_FM_02, P_Fld(0, MJC_PROG_ME2EN_0_4)|P_Fld(1, MJC_PROG_ME2EN_1_4));
                MJC_WRITE_FLD_MULTI(MJC_FM_03, P_Fld(1, MJC_PROG_ME3EN_0_4)|P_Fld(0, MJC_PROG_ME3EN_1_4));
            }
        }
    }
    else if (u1MVEXP == 0 && _u1PrevEn)
    {
        MJC_WRITE_FLD(MJC_FM_15, 0, MJC_FW_FRM_MVEXP);
        vDrvMJCSetProgME(arMJCPrm.arFRCConfig.u1PanelType, arMJCPrm.arFRCConfig.u1FrmSchMode == FALLBACK_MODE_55P2);
    }

    _u1PrevEn = u1MVEXP;
}


void vDrvMJCSetClearMVTrig(void)
{
    _u1ClearMVCnt = 1;
}

static void _vDrvMJCHandleClearMV(void)
{
    static UINT8 _u1PrevCnt = 0xFF;
    UINT8 u1CurCnt = _u1ClearMVCnt;
    
    if (u1CurCnt)
    {
        _u1ClearMVCnt --;
        MJC_WRITE_FLD(MJC_MV_00, 1, MJC_MV_DRAM_WRITE_ZERO);
    }
    else if (_u1PrevCnt != 0 /*_u1PrevCnt != _u1ClearMVCnt && _u1ClearMVCnt == 0*/)
    {
        MJC_WRITE_FLD(MJC_MV_00, 0, MJC_MV_DRAM_WRITE_ZERO);
    }

    _u1PrevCnt = u1CurCnt;
}

static void _vDrvMJCGeneralMEISR(void)
{
    E_MJC_LBOX_ME_SA eMjcLboxMeSa;
    UINT8 u1AdapBndHlrEn;
    UINT8 u1GmvTh;

    _u4SwMeCtrl = u4IO32Read4B(MJC_SW_ME_00);

    eMjcLboxMeSa = R_Fld(_u4SwMeCtrl, MJC_LBOX_ME_SA_MODE) ?
        E_ME_LBOX_SA_MANUAL : E_ME_LBOX_SA_AUTO;
    u1AdapBndHlrEn = R_Fld(_u4SwMeCtrl, MJC_BND_HLR_ADAP_EN);
    u1GmvTh = R_Fld(_u4SwMeCtrl, MJC_LBOX_ME_SA_GMVTH);
    
    _vDrvMJCSetMELBoxSA(eMjcLboxMeSa, u1GmvTh);
    vDrvMJCTmprPnltyProc();
    vDrvMJCMVEXPProc();

    if (u1DrvMJCGetOutLR() == 0)
    {
        _vDrvMJCBndHLRAdapWidth(u1AdapBndHlrEn);
        _vDrvMJCMovingLBoxProc();
        _vDrvMJCHandleExceedSearchRng();
        _vDrvMJCAdapBEC();
        _vDrvMJCStillClearMV();
        _vDrvMJCHandleClearMV();
    }
}

static void _vDrvMJCMVPadProc(void)
{
    static UINT8 _u1Cnt[2] = {0, 0};
    static UINT8 _u1PadEn[2] = {0, 0};

    UINT32 u4Tmp = u4IO32Read4B(MJC_SW_MVMC_00);
    UINT8 i = 0;    

    for (i = 0; i < 2; i++)
    {
        UINT8 u1Disable = ((_rMJCMVPad.u4TgtSAD[i] + 200) < _rMJCMVPad.u4RefSAD[i]); //Letter box with OSD

        if (u1Disable)
        {
            _u1Cnt[i] = 0;
        }
        else if (
            ((_rMJCMVPad.u4TgtSAD[i] > MIN(((UINT32)R_Fld(u4Tmp, MJC_PAD_GAIN_INC) * _rMJCMVPad.u4RefSAD[i]), 0x120000)) && _u1AbsGmvY >= 7)||
            (R_Fld(u4Tmp, MJC_PAD_MVY_EN) && (_u1AbsGmvY >= IO32ReadFldAlign(MJC_SW_MVMC_01, MJC_PAD_MVY_THR)))
            )
        {
            _u1Cnt[i] += ((_u1Cnt[i] < R_Fld(u4Tmp, MJC_PAD_CNT_MAX_THR)) ? 1 : 0);
        }
        else if (
            AbsDiff(_rMJCMVPad.u4TgtSAD[i], _rMJCMVPad.u4RefSAD[i]) < 
            MAX((((UINT32)R_Fld(u4Tmp, MJC_PAD_RATIO_DEC) * _rMJCMVPad.u4RefSAD[i]) >> 4), 200)
            )
        {
            _u1Cnt[i] -= ((_u1Cnt[i] > 0) ? 1 : 0);
        }

        if ((_u1Cnt[i] >= R_Fld(u4Tmp, MJC_PAD_CNT_HI_THR)) || R_Fld(u4Tmp, MJC_PAD_FORCE_ON))
        {
            // enable padding
            _u1PadEn[i] = 1;
        }
        else if (_u1Cnt[i] < R_Fld(u4Tmp, MJC_PAD_CNT_LO_THR))
        {
            // disable padding
            _u1PadEn[i] = 0;
        }
    }    

    MJC_WRITE_FLD_MULTI(MJC_MV_PAD, P_Fld(_u1PadEn[0], MJC_MV_PAD_UP_EN)|P_Fld(_u1PadEn[1], MJC_MV_PAD_DN_EN));
}

static void _vDrvMJCUpdateGMV(void)
{
    UINT32 u4GmvY, u4GmvX;
    INT16 i2CurGmvY, i2CurGmvX;
    INT16 i2PrdctY, i2PrdctX;

    u4GmvY = MJC_READ_FLD(MJC_STA_GMV_BI, MJC_ME2_STA_GMV_Y_FIX);
    u4GmvX = MJC_READ_FLD(MJC_STA_GMV_BI, MJC_ME2_STA_GMV_X);
    i2CurGmvY = (u4GmvY & 0x80) ? (-(((~u4GmvY) + 1) & 0xff)) : u4GmvY;
    i2CurGmvX = (u4GmvX & 0x100) ? (-(((~u4GmvX) + 1) & 0x1ff)) : u4GmvX;

    // simple extrapolation: x(n+1) = x(n) + ( x(n) - x(n-1) )
    i2PrdctY = i2CurGmvY*2 - _i2GmvY;
    i2PrdctX = i2CurGmvX*2 - _i2GmvX;
    
    _u1AbsGmvY = (UINT8)(ABS(i2CurGmvY) >> 1);
    _u1AbsGmvX = (UINT8)(ABS(i2CurGmvX) >> 1);
    _u1AbsGmvYPdct = (UINT8)(ABS(i2PrdctY) >> 1);
    _u1AbsGmvXPdct = (UINT8)(ABS(i2PrdctX) >> 1);

    _i2GmvY = i2CurGmvY;
    _i2GmvX = i2CurGmvX;

    vIO32WriteFldAlign(MJC_SW_ME_06, _u1AbsGmvX, MJC_STA_GMV_X);
    vIO32WriteFldAlign(MJC_SW_ME_06, _u1AbsGmvY, MJC_STA_GMV_Y);
}
    
static void _vDrvMJCGetSta(void)
{
    UINT32 u4GmvY, u4GmvX;
    UINT16 u2MajorMv0;

#if 0
    u4GmvY = MJC_READ_FLD(MJC_STA_GMV, MJC_ME2_STA_GMV_Y_FIX);
    u4GmvX = MJC_READ_FLD(MJC_STA_GMV, MJC_ME2_STA_GMV_X);
    u4GmvY = (u4GmvY & 0x80) ? (((~u4GmvY) + 1) & 0xff) : u4GmvY;
    u4GmvX = (u4GmvX & 0x100) ? (((~u4GmvX) + 1) & 0x1ff) : u4GmvX;
    _u1AbsGmvY = (UINT8)(u4GmvY >> 1);
    _u1AbsGmvX = (UINT8)(u4GmvX >> 1);
#endif
    _vDrvMJCUpdateGMV();

    u4GmvY = MJC_READ_FLD(MJC_STA_BG, MJC_STA_MV_BG_Y);
    u4GmvX = MJC_READ_FLD(MJC_STA_BG, MJC_STA_MV_BG_X);
    u4GmvY = (u4GmvY & 0x40) ? (((~u4GmvY) + 1) & 0x7f) : u4GmvY;
    u4GmvX = (u4GmvX & 0x100) ? (((~u4GmvX) + 1) & 0x1ff) : u4GmvX;
    _u1AbsBgmvY = (UINT8)(u4GmvY >> 1);
    _u1AbsBgmvX = (UINT8)(u4GmvX >> 1);

    u2MajorCnt0 = MJC_READ_FLD(MJC_STA_MV_00, MJC_STA_MAJOR_CNT0);
    u2MajorMv0  = MJC_READ_FLD(MJC_STA_MV_00, MJC_STA_MAJOR_MV0);
    u4GmvX = u2MajorMv0 >> 7;
    u4GmvY = u2MajorMv0 & 0x7F;
    u1MajorMvX0 = (u4GmvX >= 0x100 ? ((~u4GmvX) + 1) : u4GmvX) & 0x1FF;
    u1MajorMvY0 = (u4GmvY >= 0x40 ? ((~u4GmvY) + 1) : u4GmvY) & 0x7F;
    
    vIO32WriteFldAlign(MJC_SW_ME_06, u1MajorMvX0, MJC_STA_MAJ_X);
    vIO32WriteFldAlign(MJC_SW_ME_06, u1MajorMvY0, MJC_STA_MAJ_Y);
}

static void _vDrvMJCGetPSG(void)
{
    UINT32 u4Val;

    u2StaBdrvsi   = MJC_READ_FLD(MJC_STA_PSG_00, MJC_BDR_VSI_SUM);
    u2StaNrmvsi   = MJC_READ_FLD(MJC_STA_PSG_00, MJC_NRM_VSI_SUM);
    u2TotalCnt    = MJC_READ_FLD(MJC_STA_PSG_01, MJC_TOTAL_CNT);
    u2TotalMotCnt = MJC_READ_FLD(MJC_STA_PSG_02, MJC_TOTAL_MOT_CNT);
    u2BadMotCnt   = MJC_READ_FLD(MJC_STA_PSG_02, MJC_BAD_MOT_CNT);
    u2MaxMvErr    = MJC_READ_FLD(MJC_STA_PSG_08, MJC_MV_ERR4);
    u2StaMEBec    = MJC_READ_FLD(MJC_STA_ME_02, MJC_BEC);
    u2ZeroBlkCnt  = MJC_READ_FLD(MJC_STA_ME_0B, MJC_SUM_BLKZERO);
    u2StaCustVal  = (UINT16) MIN(_u4DrvGetCustVal(), 0xffff);
    //u2MvErr1      = MJC_READ_FLD(MJC_STA_PSG_07, MJC_MV_ERR1);
    //u2MvErr2      = MJC_READ_FLD(MJC_STA_PSG_07, MJC_MV_ERR2);
    //u2MvErr3      = MJC_READ_FLD(MJC_STA_PSG_08, MJC_MV_ERR3);
    //u2MvErr4      = MJC_READ_FLD(MJC_STA_PSG_08, MJC_MV_ERR4);

    u2LBoxBlockCnt = ((_rMJCLboxSta.u2PosDn - _rMJCLboxSta.u2PosUp +4 )>>3)*((_rMJCLboxSta.u2PosRt - _rMJCLboxSta.u2PosLf +4 )>>3);

    u4NormLboxFct = (UINT32) 0x8000 / MAX(u2LBoxBlockCnt, MIN_BLK_CNT);
    u4NormTotalFct = (UINT32) 0x8000 / MAX(u2TotalCnt, MIN_BLK_CNT);

    // avg min sad
    u4Val = u4DrvGetStaNormLbox(MJC_READ_FLD(MJC_STA_ME_01, MJC_LES_SEL));
    u4Val = MIN(u4Val, 0xffff);
    vIO32WriteFldAlign(MJC_SW_ME_04, u4Val, MJC_AVG_LES);

    // avg var
    u4Val = u4DrvGetStaNormLbox(MJC_READ_FLD(MJC_STA_ME_08, MJC_SUM_MIN_VAR_CURR));
    u4Val = MIN(u4Val, 0xffff);
    vIO32WriteFldAlign(MJC_SW_ME_04, u4Val, MJC_AVG_MINVAR);

    // avg zero sad
    u4Val = u4DrvGetStaNormLbox(MJC_READ_FLD(MJC_STA_ME_00, MJC_SUM_ZERO_SAD_SEL));
    u4Val = MIN(u4Val, 0xffff);
    vIO32WriteFldAlign(MJC_SW_ME_08, u4Val, STA_MJC_AVG_ZEROSAD);

    _vDrvMJCUpdateMEStaTotalCnt();
    
    vIO32WriteFldAlign(MJC_SW_ME_03, u2LBoxBlockCnt, MJC_ATV4LINE_LBOX_BLOCKSUM);
    //vIO32WriteFldAlign(MJC_SW_ME_03, bDrvGetAPL(), MJC_ATV4LINE_SWAPL);

    // Normalize
    u2StaNrmvsi = (UINT16)u4DrvGetStaNormTotalCnt(((UINT32)u2StaNrmvsi) << 15);
    u2StaBdrvsi = (UINT16)u4DrvGetStaNormTotalCnt(((UINT32)u2StaBdrvsi) << 15);
    u2MaxMvErr = (UINT16)u4DrvGetStaNormTotalCnt(((UINT32)u2MaxMvErr) << 15);
    u2BadMotCnt = (UINT16)u4DrvGetStaNormTotalCnt(((UINT32)u2BadMotCnt) << 15);
    u2OsdErr = (UINT16)u4DrvGetStaNormTotalCnt(((UINT32)u2OsdErr) << 15);
    u2StaMEBec = (UINT16)u4DrvGetStaNormTotalCnt(((UINT32)u2StaMEBec) << 15);
    u2StaCustVal = (UINT16)u4DrvGetStaNormTotalCnt(((UINT32)u2StaCustVal) << 15);

    vIO32WriteFldMulti(MJC_FBCK_28, P_Fld(u2StaNrmvsi, STA_FBK_NRM_VSI)|P_Fld(u2StaBdrvsi, STA_FBK_BDR_VSI));
    vIO32WriteFldMulti(MJC_FBCK_29, P_Fld(u2MaxMvErr, STA_FBK_MVERR)|P_Fld(u2BadMotCnt, STA_FBK_BAD_MOTCNT));
    vIO32WriteFld(MJC_FBCK_30, u2OsdErr, STA_FBK_OSDERR);
    vIO32WriteFldMulti(MJC_FBCK_31, P_Fld(u2StaMEBec, STA_FBK_BEC)|P_Fld(u2StaCustVal, STA_FBK_CUST));
}
void vDrvMJCSetMEScnChgTh_Mthd1(UINT32 u4Slope, UINT32 u4LowerBnd, UINT32 u4UpperBnd)
{
    UINT32 u4SelTh = 0;

    ASSERT(u4Slope < 8);
    ASSERT(u4UpperBnd >= u4LowerBnd);

    u4SelTh = (u4UpperBnd * 4) / (u4Slope + 1);

    // write registers
    MJC_WRITE_FLD(MJC_ME_02, u4SelTh, MJC_SCNCHG_SEL_TH);
    MJC_WRITE_FLD_MULTI(MJC_ME_07,
        P_Fld(0, MJC_SCNCHG_SEL)|
        P_Fld(u4Slope, MJC_SCNCHG_TH1)|
        P_Fld(u4UpperBnd, MJC_SCNCHG_TH3));
    MJC_WRITE_FLD(MJC_ME_08, u4LowerBnd, MJC_SCNCHG_TH4);
}

void vDrvMJCSetMEScnChgTh_Mthd2(UINT32 u4Slope, UINT32 u4LowerBnd, UINT32 u4SelTh)
{
    UINT32 u4UpperBnd = 0;

    ASSERT(u4Slope < 8);

    u4UpperBnd = (u4SelTh * (u4Slope + 1)) >> 2;
    
    ASSERT(u4UpperBnd >= u4LowerBnd);

    // write registers
    MJC_WRITE_FLD(MJC_ME_02, u4SelTh, MJC_SCNCHG_SEL_TH);
    MJC_WRITE_FLD_MULTI(MJC_ME_07,
        P_Fld(0, MJC_SCNCHG_SEL)|
        P_Fld(u4Slope, MJC_SCNCHG_TH1)|
        P_Fld(u4UpperBnd, MJC_SCNCHG_TH3));
    MJC_WRITE_FLD(MJC_ME_08, u4LowerBnd, MJC_SCNCHG_TH4);
}

static void _vDrvMJCGetLBoxSta(MJC_LBOX_STA_T* prLBoxSta)
{
    ASSERT(prLBoxSta != NULL);

    if (u1DrvMJCGetOutLR() == 0)
    {
        prLBoxSta->u2PosUp = MJC_READ_FLD(MJC_STA_FB_00, MJC_STA_LBOX_UP_LINE);
        prLBoxSta->u2PosDn = MJC_READ_FLD(MJC_STA_FB_00, MJC_STA_LBOX_LOW_LINE);
        prLBoxSta->u2PosLf = MJC_READ_FLD(MJC_STA_FB_02, MJC_STA_LBOX_LEFT_PIXEL);
        prLBoxSta->u2PosRt = MJC_READ_FLD(MJC_STA_FB_02, MJC_STA_LBOX_RIGHT_PIXEL);
        prLBoxSta->fgIsYSmall = MJC_READ_FLD(MJC_STA_FB_00, MJC_STA_LBOX_Y_IS_SMALL);
    }
    else
    {
        prLBoxSta->u2PosUp = MJC_READ_FLD(MJC_STA_LBOX_R1, MJC_STA_LBOX_UP_LINE_R);
        prLBoxSta->u2PosDn = MJC_READ_FLD(MJC_STA_LBOX_R1, MJC_STA_LBOX_LOW_LINE_R);
        prLBoxSta->u2PosLf = MJC_READ_FLD(MJC_STA_LBOX_R0, MJC_STA_LBOX_LEFT_PIXEL_R);
        prLBoxSta->u2PosRt = MJC_READ_FLD(MJC_STA_LBOX_R0, MJC_STA_LBOX_RIGHT_PIXEL_R);
        prLBoxSta->fgIsYSmall = MJC_READ_FLD(MJC_STA_LBOX_R1, MJC_STA_LBOX_Y_IS_SMALL_R);
    }
}

static void _vDrvMJCGetPadSADSta(void)
{
    _rMJCMVPad.u4TgtSAD[0] = MJC_READ_FLD(MJC_STA_ME_13, MJC_STA_PA_TAR_UP_SAD);
    _rMJCMVPad.u4RefSAD[0] = MJC_READ_FLD(MJC_STA_ME_14, MJC_STA_PA_REF_UP_SAD);
    _rMJCMVPad.u4TgtSAD[1] = MJC_READ_FLD(MJC_STA_ME_15, MJC_STA_PA_TAR_DN_SAD);
    _rMJCMVPad.u4RefSAD[1] = MJC_READ_FLD(MJC_STA_ME_16, MJC_STA_PA_REF_DN_SAD);
}

static BOOL _fgTrigRstTriball = 0;
UINT8 u1DrvMJCTriball(void)
{
    static UINT8 u1TriballFrmCnt = 0;
    static UINT8 u1FbckLevel = 0;
    UINT16 u2MvCntThr;
    UINT8  u1MvXThr;
    UINT8  u1MvYThr;
    UINT8  u1UpperBound;
    UINT8  u1FrmCntThr;
    UINT8  u1IncCnt;
    UINT8  u1FbckSpeed;

    if (_fgTrigRstTriball)
    {
        u1FbckLevel = 0;
        _fgTrigRstTriball = 0;
    }

    if (IO32ReadFldAlign(MJC_TRIBALL_00, TRIBALL_EN))
    {
        u1UpperBound = IO32ReadFldAlign(MJC_TRIBALL_00, TRIBALL_UPPER_BOUND);
        u1FrmCntThr  = IO32ReadFldAlign(MJC_TRIBALL_00, TRIBALL_FRM_CNT_THR);
        u2MvCntThr   = IO32ReadFldAlign(MJC_TRIBALL_01, TRIBALL_MVCNT_THR);
        u1MvXThr     = IO32ReadFldAlign(MJC_TRIBALL_01, TRIBALL_MVX_THR);
        u1MvYThr     = IO32ReadFldAlign(MJC_TRIBALL_01, TRIBALL_MVY_THR);
        u1IncCnt     = u1MajorMvX0 >> (IO32ReadFldAlign(MJC_TRIBALL_00, TRIBALL_INC_SEL));

        if ((u2MajorCnt0 >= u2MvCntThr) && (u1MajorMvX0 > u1MvXThr) && (u1MajorMvY0 > u1MvYThr))
        {
            u1TriballFrmCnt = (u1TriballFrmCnt > u1UpperBound)? u1UpperBound : u1TriballFrmCnt + u1IncCnt;
        }
        else
        {
            //if ((u1MajorMvX0 != 0) || (u1MajorMvY0 != 0))
            {
                u1TriballFrmCnt = (u1TriballFrmCnt == 0)? 0 : u1TriballFrmCnt - 1;
            }
        }

        u1FbckSpeed = IO32ReadFldAlign(MJC_TRIBALL_00, TRIBALL_FBCK_SPEED);
        if (u1TriballFrmCnt >= u1FrmCntThr)
        {
            u1FbckLevel = (u1FbckLevel >= MAX_FBCK_LVL) ? MAX_FBCK_LVL : (u1FbckLevel + u1FbckSpeed);
        }
        else if (u1TriballFrmCnt < IO32ReadFldAlign(MJC_TRIBALL_02, TRIBALL_FRM_CNT_THR_LO))
        {
            u1FbckLevel = (u1FbckLevel < u1FbckSpeed)? 0x00 : (u1FbckLevel - u1FbckSpeed);
        }
        
        vIO32WriteFldAlign(MJC_FBCK_10, u1FbckLevel, STA_FBCK_TRIBALL_LEVEL);
        
    }
    else
    {
        u1FbckLevel = 0;
    }

    return u1FbckLevel;
}

static void _vDrvMJCGetStatus(void)
{
    _vDrvMJCGetLBoxSta(&_rMJCLboxSta);

    if (u1DrvMJCGetOutLR() == 0)
    {
        _vDrvMJCGetPadSADSta();
        _vDrvMJCGetSta();
        _vDrvMJCGetPSG();
    }
}

static void _vDrvMJCOCCWtEnProc(void)
{
    //UINT8 u1Lvl0 = IO32ReadFldAlign(MJC_FBCK_10, STA_FBCK_CALC_LEVEL);
    UINT8 u1Lvl1 = IO32ReadFldAlign(MJC_FBCK_10, STA_FBCK_RMP_LEVEL);

    if (IO32ReadFldAlign(MJC_FBCK_13, OCC_ADAP_OFF))
    {
        if (/*(u1Lvl0 > 58) || */(u1Lvl1 > IO32ReadFldAlign(MJC_FBCK_13, OCC_ADAP_OFF_FBCK_TH)))
        {
            MJC_WRITE_FLD(MJC_MC_05, SV_OFF, MJC_OCC_WT_EN);
        }
        else
        {
            MJC_WRITE_FLD(MJC_MC_05, SV_ON, MJC_OCC_WT_EN);
        }
    }
}

static void _vDrvMJCAdpToc(void)
{
    UINT32 u4DiffRatioX;
    UINT32 u4DiffRatioY;
    UINT32 u4OccErrRatio;
    UINT32 u4OccCnt;
    UINT32 u4OccErr;
    UINT32 u4DiffThrX;
    UINT32 u4DiffThrY;
    
    if (IO32ReadFldAlign(MJC_ADPTOC_00, ADPTOC_EN))
    {
        u4DiffRatioX = IO32ReadFldAlign(MJC_ADPTOC_00, ADPTOC_DIFFX_RATIO);
        u4DiffRatioY = IO32ReadFldAlign(MJC_ADPTOC_00, ADPTOC_DIFFY_RATIO);
        u4OccErrRatio = IO32ReadFldAlign(MJC_ADPTOC_00, ADPTOC_OCCERR_RATIO);

        if (IO32ReadFldAlign(MJC_ADPTOC_00, ADPTOC_OCCERR_CHK_EN))
        {
            u4OccCnt = MJC_READ_FLD(MJC_STA_PSG_06, MJC_OCC_CNT);
            u4OccErr = MJC_READ_FLD(MJC_STA_PSG_07, MJC_OCC_ERR1);
            u4OccCnt = (u4OccCnt * u4OccErrRatio) >> 5;
            if (u4OccErr > u4OccCnt)
            {
                u4DiffThrX = 8;
                u4DiffThrY = 8;
            }
            else
            {
                u4DiffThrX = 4;
                u4DiffThrY = 4;
            }
        }
        else
        {
            u4DiffThrX = (_u1AbsGmvX * u4DiffRatioX) >> 4;
            u4DiffThrY = (_u1AbsGmvY * u4DiffRatioY) >> 4;
            u4DiffThrX = (u4DiffThrX > 0xF) ? 0xF : ((u4DiffThrX < 4) ? 4 : u4DiffThrX);
            u4DiffThrY = (u4DiffThrY > 0xF) ? 0xF : ((u4DiffThrY < 4) ? 4 : u4DiffThrY);
        }

        MJC_WRITE_FLD(MJC_MV_2F, u4DiffThrX, MJC_MV_TOC_BIDF_X_THR);
        MJC_WRITE_FLD(MJC_MV_2F, u4DiffThrY, MJC_MV_TOC_BIDF_Y_THR);
        MJC_WRITE_FLD(MJC_MV_31, u4DiffThrX, MJC_MV_TOC_TF_X_THR);
        MJC_WRITE_FLD(MJC_MV_31, u4DiffThrY, MJC_MV_TOC_TF_Y_THR);
    }
}

void vDrvVdoMEISR(void)
{
}

void vDrvVdoMVISR(void)
{
}

void vDrvVdoMCISR(void)
{
    UINT8 u1TgtFbckLvl = 0;
    UINT8 u1Tmp = 0;

    if (IO32ReadFldAlign(MJC_FBCK_00, FBCK_SW_EN) == SV_FALSE)
    {
        return;
    }

    u1TgtFbckLvl = u1DrvMJCTriball();
    //u1Tmp = u1DrvMJCATV4Line();
    //u1TgtFbckLvl = MAX(u1TgtFbckLvl, u1Tmp);
    u1Tmp = u1DrvCalPartialFbck(0);
    u1TgtFbckLvl = MAX(u1TgtFbckLvl, u1Tmp);

    if ((fgClearMVFb) &&  (IO32ReadFldAlign(MJC_SW_ME_09, MJC_ME_STILL_CLR_MV_FB) == SV_OFF))
    {
        u1TgtFbckLvl = 64;
	}
	 
	// moving CZP fallback by MDDi detection
    u1Tmp = u1DrvErrorFeedBack(u1TgtFbckLvl);
    
    vIO32WriteFldAlign(MJC_FBCK_10, u1TgtFbckLvl, STA_FBCK_TGT_LEVEL);

    vDrvSetPartialFbck(0, u1Tmp);
}    

void vDrvFilmMEISR(void)
{
}

void vDrvFilmMVISR(void)
{
    _vDrvMJCAdpToc();
}

void vDrvFilmMCISR(void)
{
    static UINT8 _u1CurFbckLvl = 0;
    UINT8 u1TgtFbckLvl;
    
    if (IO32ReadFldAlign(MJC_FBCK_00, FBCK_SW_EN) == SV_FALSE)
    {
        return;
    }

    if (IO32ReadFldAlign(MJC_FBCK_00, FBCK_MODE) == 0)
    {
        u1TgtFbckLvl = u1DrvCalPartialFbck(1);
    }
    else
    {
        u1TgtFbckLvl = u1DrvCalPartialFbckAG();
    }

	if ((fgClearMVFb) &&  (IO32ReadFldAlign(MJC_SW_ME_09, MJC_ME_STILL_CLR_MV_FB) == SV_OFF))
    {
        u1TgtFbckLvl = 64;
	}
	 
    if (IO32ReadFldAlign(MJC_FBCK_00, FBCK_TRANSITION_MODE) == 0)
    {
        _u1CurFbckLvl = u1DrvErrorFeedBack(u1TgtFbckLvl);
    }
    else
    {
        if (u1TgtFbckLvl > _u1CurFbckLvl)
            _u1CurFbckLvl ++;
        else if (u1TgtFbckLvl < _u1CurFbckLvl)
            _u1CurFbckLvl --;
    }

    vIO32WriteFldAlign(MJC_FBCK_10, u1TgtFbckLvl, STA_FBCK_TGT_LEVEL);
    
    vDrvSetPartialFbck(1, _u1CurFbckLvl);
}

BOOL fgDisableHf = 0;
EXTERN void vDrvMJCSetRdcType(BOOL fgDft);

static void _vDrvMJCIntpCtrl(void)
{
    UINT8 i = 0;
    UINT8 u1IntpRgn[3] = {0};
    UINT8 u1HqfRgn[3] = {0};
    UINT8 u1MCRgnNum = 3;

    if (!arMJCPrm.u1OneFrm)
    {
        for (i = 0; i < 3; i++)
        {
            switch (arMJCDemo.arRgn[i])
            {
            default:
            case E_MJC_RGN_NF:
                u1IntpRgn[i] = 6;
                u1HqfRgn[i] = 0;
                break;
            case E_MJC_RGN_HF:
                u1IntpRgn[i] = 6;
                u1HqfRgn[i] = fgDisableHf ? 0 : 1;
                break;
            case E_MJC_RGN_QF:
                u1IntpRgn[i] = 6;
                u1HqfRgn[i] = 2;
                break;
            case E_MJC_RGN_RPT:
                u1IntpRgn[i] = 0;
                u1HqfRgn[i] = 0;
                u1MCRgnNum --;
                break;
            }
        }
    }

    if (arMJCPrm.eMjcEffect == E_MJC_EFFECT_OFF)
    {
        u1IntpRgn[0] = u1IntpRgn[1] = u1IntpRgn[2] = 0;
        u1HqfRgn[0] = u1HqfRgn[1] = u1HqfRgn[2] = 0;
        u1MCRgnNum = 0;
    }

    vDrvMJCSetRdcType(u1MCRgnNum == 3 && arMJCPrm.u1EffectParam != 0);

    MJC_WRITE_FLD_MULTI(MJC_MC_00, 
        P_Fld(u1IntpRgn[0], MJC_MC_INTP_MODE_RGN0) |
        P_Fld(u1IntpRgn[1], MJC_MC_INTP_MODE_RGN1) |
        P_Fld(u1IntpRgn[2], MJC_MC_INTP_MODE_RGN2));
    MJC_WRITE_FLD_MULTI(MJC_MC_03,
        P_Fld(u1HqfRgn[0], MJC_HQF_MODE_RGN0) |
        P_Fld(u1HqfRgn[1], MJC_HQF_MODE_RGN1) |
        P_Fld(u1HqfRgn[2], MJC_HQF_MODE_RGN2));

    MJC_WRITE_FLD_MULTI(MJC_MC_0B,
        P_Fld(arMJCDemo.eDemoMode, MJC_SPLIT_MODE) |
        P_Fld(arMJCDemo.u2BndLeft, MJC_SPLIT_BND_LF) |
        P_Fld(arMJCDemo.u2BndRight, MJC_SPLIT_BND_RT));
    MJC_WRITE_FLD_MULTI(MJC_MC_0D,
        P_Fld(arMJCDemo.u2BndTop, MJC_SPLIT_BND_UP) |
        P_Fld(arMJCDemo.u2BndBottom, MJC_SPLIT_BND_DN));
}

void vDrvMJCUpdateIntpCtrl(void)
{
    if (_u1IntpCtrlUpdate < 0xff)
        _u1IntpCtrlUpdate ++;
}

UINT32 u4FilmModeFlag(void)
{
    if (MJC_READ_FLD(MJC_FM_05, MJC_FW_FRM_SCH_EN))
    {
        if (IO32ReadFldAlign(MJC_FW_SCH_03, MJC_50TO120_EN))
            return MJC_FM_32;
        else
            return MJC_READ_FLD(MJC_FM_05, MJC_FW_ME_FILM_FLG);
    }
    else
    {
        if (MJC_READ_FLD(MJC_SYS_04, MJC_PROG_FRM_EN))
            return MJC_FM_32;
        else
            return MJC_READ_FLD(MJC_STA_FM_02, MJC_STA_ME_FILM_FLG_HW);
    }
}

UINT32 u4DrvMJCGetFilmModeStatus(void)
{
    UINT32 u4Ret = 0;
    UINT32 u4HWInfo = 0;

    if (MJC_READ_FLD(MJC_FM_05, MJC_FW_FRM_SCH_EN))
    {
        switch (IO32ReadFldAlign(MJC_FW_SCH_00, MJC_RO_FRM_SCH_SW_DET_FILM_TYPE))
        {
        case 0:
            u4Ret = MJC_FM_32322;
            break;
        case 1:
            u4Ret = MJC_FM_2332;
            break;
        case 2:
            u4Ret = MJC_FM_2224;
            break;
        case 3:
            u4Ret = MJC_FM_32;
            break;
        case 4:
            u4Ret = MJC_FM_22;
            break;
        default:
        case 5:
            u4Ret = MJC_FM_VDO;
            break;
        }
    }
    else
    {
        u4HWInfo = MJC_READ_FLD(MJC_STA_FM_01, MJC_STA_PS_FILM_ID_HW);
        
        if (u4HWInfo > 5)
        {
            u4Ret = MJC_FM_22;
        }
        else if (u4HWInfo > 0)
        {
            u4Ret = MJC_FM_32;
        }
        else
        {
            u4Ret = MJC_FM_VDO;
        }
    }
    
    return u4Ret;
}

UINT8 u1DrvMJCGetOutLR(void)
{
    return _u1OutLR;
}


UINT32 u4DrvMJCGetCurScnChgTh(UINT32 u4CurLes)
{
    UINT32 u4SelTh, u4Slope, u4UpperBnd, u4LowerBnd, u4CurTh;

    u4SelTh = MJC_READ_FLD(MJC_ME_02, MJC_SCNCHG_SEL_TH);
    u4Slope = MJC_READ_FLD(MJC_ME_07, MJC_SCNCHG_TH1);
    u4UpperBnd = MJC_READ_FLD(MJC_ME_07, MJC_SCNCHG_TH3);
    u4LowerBnd = MJC_READ_FLD(MJC_ME_07, MJC_SCNCHG_TH4);

    if (u4CurLes >= u4SelTh)
    {
        u4CurTh = u4UpperBnd;
    }
    else
    {
        u4CurTh = ((u4Slope + 1)*u4CurLes) >> 2;
    }

    if (u4CurTh < u4LowerBnd)
    {
        u4CurTh = u4LowerBnd;
    }

    return u4CurTh;
}

BOOL u4DrvMJCIsLesPrevTiming(void)
{
    UINT32 u4Idx = MJC_READ_FLD(MJC_STA_FM_02, MJC_STA_ME_FILM_IDX_HW);
    UINT32 u4Phs = MJC_READ_FLD(MJC_STA_FM_02, MJC_STA_ME_FILM_PHS_HW);

    if ((u4Idx == 1 && u4Phs == 0) || (u4Idx == 0 && u4Phs == 1))
        return SV_TRUE;
    else
        return SV_FALSE;
}

void vDrvMJCMEStaPolling(void *pvArgv)
{
    static UINT32 u4LESPrev = 0;
    UINT32 u4LESCur = 0;
    UINT32 u4LESDiff = 0;
    BOOL fgSWScnChg;

    u4LESCur = MJC_READ_FLD(MJC_STA_ME_01, MJC_LES_SEL) >> 6;
            
    u4LESDiff = u4LESCur - u4LESPrev;
    u4LESDiff = (u4LESDiff & 0x80000000) ? ((~u4LESDiff)+1) : u4LESDiff;
    
    fgSWScnChg = (u4LESCur > (u4LESPrev + u4DrvMJCGetCurScnChgTh(u4LESPrev)));
        
    Printf("(LESCur, LESPrev, Diff, ScnChg, SW) = (0x%08X, 0x%08X, 0x%08X, %d, %d)",
        u4LESCur, u4LESPrev, u4LESDiff, MJC_READ_FLD(MJC_STA_ME_02, MJC_SCNCHG_OK), fgSWScnChg);

    u4LESPrev = u4LESCur;
}

BOOL fgDrvMJCIsMEScnChg(void)
{
    static UINT8 _u1Cnt = 0;
    
    if (MJC_READ_FLD(MJC_STA_ME_02, MJC_SCNCHG_OK))
    {
        _u1Cnt = 5;
    }
    else if (_u1Cnt)
    {
        _u1Cnt--;
    }

    return _u1Cnt ? 1 : 0;
}

void vDrvMJCQueryMEProgMode(void)
{
    // TODO 
}



void vDrvMJCSetOSDIntp(UINT32 u4OnOff, UINT32 u4IntpCtrl)
{
    if (u4OnOff)
    {
        MJC_WRITE_FLD(MJC_MC_16, (u4IntpCtrl == MJC_INTP_OFF)? 0x0 : 0x6, MJC_MC_INTP_OSD);
        MJC_WRITE_FLD(MJC_MC_17, SV_ON, MJC_OSD_BAR_EN);
        MJC_WRITE_FLD(MJC_MC_16, 0x5A0, MJC_OSD_BOUND_RIGHT_0);
        MJC_WRITE_FLD(MJC_MC_16, 0x1E0, MJC_OSD_BOUND_LEFT_0);
        MJC_WRITE_FLD(MJC_MC_17, 0x10E, MJC_OSD_BOUND_DOWN_0);
        MJC_WRITE_FLD(MJC_MC_17, 0x32A, MJC_OSD_BOUND_UP_0);
    }
    else
    {
        MJC_WRITE_FLD(MJC_MC_16, 0x6, MJC_MC_INTP_OSD);
        MJC_WRITE_FLD(MJC_MC_17, SV_OFF, MJC_OSD_BAR_EN);
        MJC_WRITE_FLD(MJC_MC_16, 0x0, MJC_OSD_BOUND_RIGHT_0);
        MJC_WRITE_FLD(MJC_MC_16, 0x0, MJC_OSD_BOUND_LEFT_0);
        MJC_WRITE_FLD(MJC_MC_17, 0x0, MJC_OSD_BOUND_DOWN_0);
        MJC_WRITE_FLD(MJC_MC_17, 0x0, MJC_OSD_BOUND_UP_0);
    }
}

void vDrvMJCSetOSDMask(UINT32 u4Mask, UINT32 u4Left, UINT32 u4Top, UINT32 u4Right, UINT32 u4Bottom)
{
    switch (u4Mask)
    {
    case 0:
        MJC_WRITE_FLD(MJC_MC_16, u4Right, MJC_OSD_BOUND_RIGHT_0);
        MJC_WRITE_FLD(MJC_MC_16, u4Left, MJC_OSD_BOUND_LEFT_0);
        MJC_WRITE_FLD(MJC_MC_17, u4Bottom, MJC_OSD_BOUND_DOWN_0);
        MJC_WRITE_FLD(MJC_MC_17, u4Top, MJC_OSD_BOUND_UP_0);
        break;
    case 1:
        MJC_WRITE_FLD(MJC_MC_18, u4Right, MJC_OSD_BOUND_RIGHT_1);
        MJC_WRITE_FLD(MJC_MC_18, u4Left, MJC_OSD_BOUND_LEFT_1);
        MJC_WRITE_FLD(MJC_MC_19, u4Bottom, MJC_OSD_BOUND_DOWN_1);
        MJC_WRITE_FLD(MJC_MC_19, u4Top, MJC_OSD_BOUND_UP_1);
        break;
    case 2:
        MJC_WRITE_FLD(MJC_MC_1A, u4Right, MJC_OSD_BOUND_RIGHT_2);
        MJC_WRITE_FLD(MJC_MC_1A, u4Left, MJC_OSD_BOUND_LEFT_2);
        MJC_WRITE_FLD(MJC_MC_1A, u4Bottom & 0xF, MJC_OSD_BOUND_DOWN_2);
        MJC_WRITE_FLD(MJC_MC_19, u4Bottom >> 4, MJC_OSD_BOUND_DOWN_2_11_4);
        MJC_WRITE_FLD(MJC_MC_1A, u4Top & 0xF, MJC_OSD_BOUND_UP_2_3_0);
        MJC_WRITE_FLD(MJC_MC_18, u4Top >> 4, MJC_OSD_BOUND_UP_2_11_4);
        break;
    case 3:
        MJC_WRITE_FLD(MJC_MC_1B, u4Right, MJC_OSD_BOUND_RIGHT_3);
        MJC_WRITE_FLD(MJC_MC_1B, u4Left, MJC_OSD_BOUND_LEFT_3);
        MJC_WRITE_FLD(MJC_MC_1C, u4Bottom, MJC_OSD_BOUND_DOWN_3);
        MJC_WRITE_FLD(MJC_MC_1C, u4Top, MJC_OSD_BOUND_UP_3);
        break;
    default:
    case 4:
        MJC_WRITE_FLD(MJC_MC_1D, u4Right, MJC_OSD_BOUND_RIGHT_4);
        MJC_WRITE_FLD(MJC_MC_1D, u4Left, MJC_OSD_BOUND_LEFT_4);
        MJC_WRITE_FLD(MJC_MC_1D, u4Bottom & 0xF, MJC_OSD_BOUND_DOWN_4);
        MJC_WRITE_FLD(MJC_MC_1C, u4Bottom >> 4, MJC_OSD_BOUND_DOWN_4_11_4);
        MJC_WRITE_FLD(MJC_MC_1D, u4Top & 0xF, MJC_OSD_BOUND_UP_4);
        MJC_WRITE_FLD(MJC_MC_1B, u4Top >> 4, MJC_OSD_BOUND_UP_4_11_4);
    }
}

void vDrvMJCSetDemoBar(UINT8 fgOnOff, UINT8 u1R, UINT8 u1G, UINT8 u1B)
{
    MJC_WRITE_FLD(MJC_MC_0B, fgOnOff, MJC_DEMO_BAR_EN);
    MJC_WRITE_FLD_MULTI(MJC_MC_0C, 
        P_Fld(u1R, MJC_BAR_U) |
        P_Fld(u1G, MJC_BAR_Y) |
        P_Fld(u1B, MJC_BAR_V) |
        P_Fld(2, MJC_BAR_WIDTH));
}

void vDrvMJCSetMCOnOff(UINT8 fgOnOff)
{
    fgMCOnOff = fgOnOff;
}

void vDrvMJCBadEdit(void)
{
    static UINT8 _fgPrevOnOff = 0x0;
    static UINT32 fgUnstable = SV_OFF;
    static UINT32 u4UnstableCnt = 0;
    static UINT32 u4UnstableTimer = 0;
    static UINT32 fgPrevFilmMode = 0;
    UINT32 fgCurrFilmMode;
    UINT32 fgCurrOnOff = IO32ReadFldAlign(MJC_BADEDIT_00, BADEDIT_EN);

    fgCurrFilmMode = (u4DrvMJCGetFilmModeStatus() != MJC_FM_VDO) ? SV_TRUE : SV_FALSE;

    if (fgUnstable == SV_OFF)
    {
        if (u4UnstableTimer == 0)
        {
            u4UnstableTimer = 0;
        }
        else if (u4UnstableTimer == IO32ReadFldAlign(MJC_BADEDIT_00, BADEDIT_ENTER_TH))
        {
            u4UnstableCnt = 0;
            u4UnstableTimer = 0;
        }
        else
        {
            u4UnstableTimer ++;
        }

        if (fgCurrFilmMode != fgPrevFilmMode)
        {
            u4UnstableCnt ++;

            if (u4UnstableTimer == 0)
            {
                u4UnstableTimer = 1;
            }

            if (u4UnstableCnt > IO32ReadFldAlign(MJC_BADEDIT_00, BADEDIT_UNSTABLE_CNT))
            {
                fgUnstable = SV_ON;
                u4UnstableCnt = 0;
                u4UnstableTimer = 0;
            }
        }
    }
    else
    {
        u4UnstableTimer ++;
        if (fgCurrFilmMode != fgPrevFilmMode)
        {
            u4UnstableTimer = 0;
        }

        if (u4UnstableTimer >= IO32ReadFldAlign(MJC_BADEDIT_00, BADEDIT_EXIT_TH))
        {
            u4UnstableTimer = 0;
            fgUnstable = SV_OFF;
        }
    }

    fgPrevFilmMode = fgCurrFilmMode;

    if (fgCurrOnOff)
    {
        if (fgUnstable)
        {
            vDrvMJCSetPSAutoOnOff(SV_OFF);
        }
        else
        {
            vDrvMJCSetPSAutoOnOff(SV_ON);
        }
    }
    else if (_fgPrevOnOff)
    {
        vDrvMJCSetPSAutoOnOff(SV_ON);
    }

    _fgPrevOnOff = fgCurrOnOff;
}

void vDrvMJCSetQualityISR(void)
{
    static UINT32 u4PreFilmStatus = 0xFF;
    UINT32 u4CurFilmStatus = MJC_FM_VDO;

    _u1OutLR = MJC_READ_FLD(MJC_STA_LR3D, MJC_STA_LR3D_INFO_0);

    if ((arMJCPrm.eMjcEffect != E_MJC_EFFECT_OFF) || (fgMCOnOff == 0))
    {
        _vDrvMJCGetStatus();
        _vDrvMJCGeneralMEISR();

        if (u1DrvMJCGetOutLR() == 0)
        {
            u4CurFilmStatus = u4FilmModeFlag();
            if (u4PreFilmStatus != u4CurFilmStatus) // reset isr status
            {
                u4PreFilmStatus = u4CurFilmStatus;
                _fgTrigRstTriball = 1;
                MJC_WRITE_FLD(MJC_ME_02,
                    ((IO32ReadFldAlign(MJC_SW_ME_00, MJC_ME_SCNCHG_VDO_OFF) && (u4CurFilmStatus == MJC_FM_VDO)) ? 0 : 1),
                    MJC_SCNCHG_EN);
                //vDrvSetPartialFbck(0);
                vDrvInitAvgFilter();
                vDrvInitErrorFeedBack();
            }

            _vDrvMJCMVPadProc();

            if(u4CurFilmStatus == MJC_FM_VDO)
            {
                vDrvVdoMEISR();
                vDrvVdoMVISR();
                vDrvVdoMCISR();
            }
            else
            {
                vDrvFilmMEISR();
                vDrvFilmMVISR();
                vDrvFilmMCISR();
            }
            _vDrvMJCOCCWtEnProc();
        }
    }

    if (_u1IntpCtrlUpdate)
    {
        _u1IntpCtrlUpdate --;
        _vDrvMJCIntpCtrl();
    }
}

