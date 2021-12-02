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
 * $RCSfile: drv_mjc_frmsch.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------*
  * Header Files
  *----------------------------------------------------------------------------*/
#include "hw_vdoin.h"
#include "drv_mjc.h"
#include "drv_mjc_int.h"
#include "drv_mjc_frmsch.h"
#include "nptv_debug.h"
#include "hw_mjc.h"
#include "hw_mjcfd.h"
#include "hw_sw.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_assert.h"
#include "hw_di_int.h"
 
/*----------------------------------------------------------------------------*
* Global/Static variables
*----------------------------------------------------------------------------*/
#define MJC_FRAME_BUFFER 	9
#define PD_INFO_BUF_SIZE    16
	
#define SW_FILM_AUTO_22323  (1<<0)
#define SW_FILM_AUTO_3223   (1<<1)
#define SW_FILM_AUTO_2224   (1<<2)
#define SW_FILM_AUTO_32     (1<<3)
#define SW_FILM_AUTO_22     (1<<4)
	
enum
{
    FRM_SCH_CTRL_HW = 0,    // HW only
    FRM_SCH_CTRL_SW = 1,    // SW only
    FRM_SCH_CTRL_AUTO = 2   // HW/SW switch
};

typedef enum
{	
	FILM_TYPE_22323 = 0x00,
	FILM_TYPE_3223	= 0x01,
	FILM_TYPE_2224	= 0x02,
	FILM_TYPE_32	= 0x03,
	FILM_TYPE_22	= 0x04,
	VIDEO_TYPE		= 0x05,
	FILM_TYPE_MAX
} E_FILM_MODE_TYPE;

typedef struct
{
    UINT8 u1FrmIn;
    UINT8 u1FrmOut;
} MJC_FRMSCH_IO_T;

typedef struct
{
    UINT8 u1W2P;    // Wptr to Rptr Prev diff
    UINT8 u1P2C;    // Prev to Curr diff
    UINT8 u1P2N;    // Prev to Next diff
} MJC_FRMSCH_DPTR_T;

static CODE UINT8 _arPdInfoSize[FILM_TYPE_MAX] =
{
    12, // FILM_TYPE_22323
    10, // FILM_TYPE_3223
    10, // FILM_TYPE_2224
    5,  // FILM_TYPE_32
    2,  // FILM_TYPE_22
    1   // VIDEO_TYPE
};

static CODE UINT16 _arMskSeq[FILM_TYPE_MAX - 1] =
{
    0xFFF, // FILM_TYPE_22323
    0x3FF, // FILM_TYPE_3223
    0x3FF, // FILM_TYPE_2224
    0x3FF, // FILM_TYPE_32
    0x3FF  // FILM_TYPE_22
};

static CODE UINT16 _arRefSeq[FILM_TYPE_MAX - 1] =
{
    0xA94, // FILM_TYPE_22323
    0x254, // FILM_TYPE_3223
    0x055, // FILM_TYPE_2224
    0x0A5, // FILM_TYPE_32
    0x155  // FILM_TYPE_22
};

static CODE UINT8 _arEnterPoint[FILM_TYPE_MAX - 1] =
{
    19, // FILM_TYPE_22323
    29, // FILM_TYPE_3223
    39, // FILM_TYPE_2224
    1,  // FILM_TYPE_32
    6   // FILM_TYPE_22
};

static CODE UINT8 _arEnterIdx[FILM_TYPE_MAX - 1] =
{
    0, // FILM_TYPE_22323
    0, // FILM_TYPE_3223
    0, // FILM_TYPE_2224
    1,  // FILM_TYPE_32
    1   // FILM_TYPE_22
};

static CODE UINT8 _arPdInfoStart[FILM_TYPE_MAX] =
{
    8,  // FILM_TYPE_22323
    20, // FILM_TYPE_3223
    30, // FILM_TYPE_2224
    1,  // FILM_TYPE_32
    6,  // FILM_TYPE_22
    0   // VIDEO_TYPE
};

static CODE MJC_FRMSCH_IO_T _arFrmSchIO[2][FILM_TYPE_MAX] =
{
    // 60Hz
    {
        {5, 12}, // FILM_TYPE_22323
        {2, 5},  // FILM_TYPE_3223
        {2, 5},  // FILM_TYPE_2224
        {2, 5},  // FILM_TYPE_32
        {1, 2},  // FILM_TYPE_22
        {1, 1}   // VIDEO_TYPE
    },
    // 120Hz
    {
        {5, 24}, // FILM_TYPE_22323
        {1, 5},  // FILM_TYPE_3223
        {1, 5},  // FILM_TYPE_2224
        {1, 5},  // FILM_TYPE_32
        {1, 4},  // FILM_TYPE_22
        {1, 2}   // VIDEO_TYPE
    }
};

UINT16 arSeqFilm32[5] =
{
	0x00A5, // A : 0010100101
	0x014A, // A : 0101001010
	0x0294, // A : 1010010100
	0x0129, // B : 0100101001
	0x0252, // B : 1001010010
};

UINT16 arSeqFilm22[2] =
{
	0x0155, // 6 : 0101010101
    0x02AA, // 7 : 1010101010
};

UINT16 arSeqFilm22323[12] =
{
	0x0529, // 8  : 010100101001
	0x0A52, // 9  : 101001010010
	0x04A5, // 10 : 010010100101
	0x094A, // 11 : 100101001010
	0x0295, // 12 : 001010010101
	0x052A, // 13 : 010100101010
	0x0A54, // 14 : 101001010100
	0x04A9, // 15 : 010010101001
	0x0952, // 16 : 100101010010
	0x02A5, // 17 : 001010100101
	0x054A, // 18 : 010101001010
	0x0A94  // 19 : 101010010100
};
	
UINT16 arSeqFilm3223[10] =
{
	0x00A9, // 20 : 0010101001
	0x0152, // 21 : 0101010010
	0x02A4, // 22 : 1010100100
	0x0149, // 23 : 0101001001
	0x0292, // 24 : 1010010010
	0x0125, // 25 : 0100100101
	0x024A, // 26 : 1001001010
	0x0095, // 27 : 0010010101
	0x012A, // 28 : 0100101010
	0x0254  // 29 : 1001010100	
};

UINT16 arSeqFilm2224[10] =
{
	0x00AA, // 37 : 0010101010
	0x0154, // 38 : 0101010100
	0x02A8, // 39 : 1010101000

    0x0151, // 30 : 0101010001
	0x02A2, // 31 : 1010100010
	0x0145, // 32 : 0101000101

	0x028A, // 33 : 1010001010
	0x0115, // 34 : 0100010101
	0x022A, // 35 : 1000101010
	0x0055, // 36 : 0001010101
};

UINT16 * _parSeqFilm[FILM_TYPE_MAX - 1] =
{
    &arSeqFilm22323[0],
    &arSeqFilm3223[0],
    &arSeqFilm2224[0],
    &arSeqFilm32[0],
    &arSeqFilm22[0]
};

static MJC_FM_05_T _arMJCFm05Tbl[PD_INFO_MAX] =
{
//   NoCtl Rescn PrvSh CurSh NxtSh MeIdx MePhs MeFlg ItNum,LRSel
    {    0,    1,    0,    1,    2,    0,    1,    0,    0,    0},
    {    0,    0,    0,    1,    2,    1,    1,    0,    1,    0},

    {    0,    1,    0,    3,    5,    0,    1,    1,    0,    0},
    {    0,    0,    0,    3,    5,    1,    1,    1,    1,    0},
    {    0,    0,    0,    3,    5,    2,    1,    1,    2,    0},
    {    0,    0,    0,    3,    5,    3,    1,    1,    3,    0},
    {    0,    0,    0,    2,    5,    4,    1,    1,    4,    0},
    {    0,    1,    1,    3,    6,    0,    0,    1,    0,    0},
    {    0,    0,    0,    3,    5,    1,    0,    1,    1,    0},
    {    0,    0,    0,    3,    5,    2,    0,    1,    2,    0},
    {    0,    0,    0,    3,    5,    3,    0,    1,    3,    0},
    {    0,    0,    0,    3,    5,    4,    0,    1,    4,    0},

    {    0,    1,    1,    3,    5,    0,    1,    1,    0,    0},
    {    0,    0,    1,    3,    5,    1,    1,    1,    1,    0},
    {    0,    0,    0,    2,    4,    2,    1,    1,    2,    0},
    {    0,    0,    0,    2,    4,    3,    1,    1,    3,    0}
 };

static MJC_FM_06_T _arMJCFm06Tbl[PD_INFO_MAX] =
{
//   Me2En Me3En RdcEn RdcTy McNum MePos PPCMd Mcori PPCEn Prebi Mvexp NoCtl Fb_64 PPCIt PrFst NoCtl LRTog
    {    0,    1,    1,    0, 0x00,    0,    0,    1,    0,    0,    0,    0,    0,    0,    1,    0,    0},
    {    1,    0,    1,    0, 0x80,    0,    0,    0,    0,    1,    0,    0,    0,    1,    0,    0,    0},

    {    0,    1,    1,    0, 0x00,    0,    0,    1,    1,    0,    0,    0,    0,    1,    1,    0,    0},
    {    1,    0,    1,    0, 0x33,    0,    0,    0,    1,    0,    0,    0,    0,    1,    0,    0,    0},
    {    1,    0,    1,    0, 0x66,    2,    1,    0,    1,    0,    0,    0,    0,    1,    0,    0,    0},
    {    0,    1,    1,    2, 0x99,    2,    1,    0,    1,    0,    0,    0,    0,    1,    0,    0,    0},
    {    1,    0,    1,    2, 0xCC,    0,    0,    0,    0,    1,    0,    0,    0,    0,    0,    0,    0},
    {    0,    1,    1,    0, 0x00,    0,    0,    1,    1,    0,    0,    0,    0,    1,    1,    0,    0},
    {    1,    0,    1,    0, 0x33,    0,    0,    0,    1,    0,    0,    0,    0,    1,    0,    0,    0},
    {    1,    0,    1,    0, 0x66,    2,    1,    0,    1,    0,    0,    0,    0,    1,    0,    0,    0},
    {    0,    1,    1,    2, 0x99,    2,    1,    0,    1,    0,    0,    0,    0,    1,    0,    0,    0},
    {    1,    0,    1,    2, 0xCC,    0,    0,    0,    0,    1,    0,    0,    0,    0,    0,    0,    0},

    {    0,    1,    1,    0, 0x00,    1,    0,    1,    1,    0,    0,    0,    0,    0,    0,    0,    0},
    {    1,    0,    1,    0, 0x40,    1,    1,    0,    1,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    1,    1,    2, 0x80,    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0},
    {    1,    0,    1,    2, 0xC0,    1,    0,    0,    0,    1,    0,    0,    0,    0,    1,    0,    0}
};

static MJC_FM_07_T _arMJCFm07Tbl[PD_INFO_MAX] =
{
//   NoCtl PcoEn CcoEn ItDen Fb55p HqChg 2ndHq 1stHq HqLst NoCtl NoInp TtdCh
    {    0,    0,    1,    2,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    1,    2,    0,    0,    0,    0,    0,    0,    0,    0},

    {    0,    0,    1,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    1,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    1,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    1,    0,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    1,    0,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    1,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    1,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    1,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    1,    0,    5,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    1,    0,    5,    0,    0,    0,    0,    0,    0,    0,    0},

    {    0,    0,    1,    4,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    0,    1,    4,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    1,    0,    4,    0,    0,    0,    0,    0,    0,    0,    0},
    {    0,    1,    0,    4,    0,    0,    0,    0,    0,    0,    0,    0}
};

static MJC_FRM_SCH_IN_PARAM_T _arMJCFrmSchIn = {0};
//static MJC_FRM_SCH_OUT_PARAM_T _arMJCFrmSchOut = {0};

static UINT8 _u1SwFrmSchCtrl; // 0: always HW, 1: always SW, 2: HW/SW switch
static UINT8 _u1SwAutoDet; // [0]: 22323, [1]: 3223, [2]: 2224 [3]: 32, [4]: 22, 
static UINT8 _u1Sw3DMode;
static UINT8 _u1SwFrmSchEnCurr; // 0: HW, 1: SW
static UINT8 _u1SwitchLock;
static UINT8 _u1SwForceVdo = 0; // PSAUTO off
//static UINT8 _u1Sw1X = 0; // PSAUTO off
static UINT8 _u1HwPdInfo; // 0: video, 1~5: 32-film, 6~7: 22-film
static UINT8 _u1HwWrtPtr;
static UINT8 _u1SwLPdInfoBuf[PD_INFO_BUF_SIZE] = {0}; // 0: video, 1~5: 32-film, 6~7: 22-film, 8~19: 22323-film, 20~29: 3223-film, 30~39: 2224-film
//static UINT8 _u1SwLFilmTypeBuf[PD_INFO_BUF_SIZE] = {0};
static UINT8 _u1SwLWrtPtrBuf[PD_INFO_BUF_SIZE] = {0};
static UINT8 _u1SwRPdInfoBuf[PD_INFO_BUF_SIZE] = {0}; // 0: video, 1~5: 32-film, 6~7: 22-film, 8~19: 22323-film, 20~29: 3223-film, 30~39: 2224-film
//static UINT8 _u1SwRFilmTypeBuf[PD_INFO_BUF_SIZE] = {0};
static UINT8 _u1SwRWrtPtrBuf[PD_INFO_BUF_SIZE] = {0};
static UINT8 _u1LPdInfoIdx;
static UINT8 _u1RPdInfoIdx;
static UINT8 _u1FwFrmWrtPtr;
UINT32 u4WriteFm05, u4WriteFm06, u4WriteFm07;

UINT8 u1PeriodOfSeq32;
UINT8 u1PeriodOfSeq22;
//UINT8 u1PeriodOfSeq22323;
//UINT8 u1PeriodOfSeq3223;
//UINT8 u1PeriodOfSeq2224;

UINT16 u2RefSeq32;
UINT16 u2RefSeq22; 

UINT16 u2MskSeq32;
UINT16 u2MskSeq22;

UINT8  u1LIdx32, u1RIdx32, u1LIdx22, u1RIdx22;
UINT8  u1Idx22323, u1Idx3223, u1Idx2224;
UINT8  u1LIdx, u1RIdx;

UINT8  u1FilmOrder;

static UINT8 _u1FrmIORatio;
static UINT8 _u1IsInter;
static UINT8 _u1IsInterFlag;
static UINT8 _fgFrmWrtPtrCtrl = SV_FALSE;

static UINT8 _u1InputFrameDelay;
static UINT8 _u1PdInfoDelay;
//static UINT8 _u1TestFrameDelay;

static UINT32 _u4FrmSchCtrl;
static char* _fbStr[4] = {"fw", "bi", "bw", "na"};

UINT8  u1UserDefineFilm = 0;

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
static void _vDrvMJCFrmSchHWLog(void)
{
	UINT32 u4StaFm01, u4StaFm02, u4StaFm03;

    u4StaFm01 = MJC_READ_4B(MJC_STA_FM_01);
    u4StaFm02 = MJC_READ_4B(MJC_STA_FM_02);
    u4StaFm03 = MJC_READ_4B(MJC_STA_FM_03);

    Printf("%d ", R_Fld(u4StaFm01, MJC_STA_PS_FILM_ID_HW));
    Printf("%s ", R_Fld(u4StaFm03, MJC_STA_WRT_FRM_PTR_HW) & 0x10 ? "R" : "L");
    Printf("%d ", R_Fld(u4StaFm03, MJC_STA_WRT_FRM_PTR_HW) & 0xF);
    Printf("%s ", R_Fld(u4StaFm02, MJC_STA_DA_FRM_PREV_HW) & 0x10 ? "R" : "L");
    Printf("%x ", R_Fld(u4StaFm02, MJC_STA_DA_FRM_PREV_HW) & 0xF);
    Printf("%x ", R_Fld(u4StaFm02, MJC_STA_DA_FRM_CURR_HW) & 0xF);    
    Printf("%x ", R_Fld(u4StaFm02, MJC_STA_DA_FRM_NEXT_HW) & 0xF);

    Printf("%d/%02d/%d/%02d/%02d ",
        R_Fld(u4StaFm02, MJC_STA_ME_FILM_PHS_HW),
        R_Fld(u4StaFm02, MJC_STA_ME_FILM_IDX_HW),
        R_Fld(u4StaFm02, MJC_STA_ME_FILM_FLG_HW),
        R_Fld(u4StaFm02, MJC_STA_MEMC_NUM_HW),
        R_Fld(u4StaFm02, MJC_STA_MEMC_DEN_HW));

    Printf("%d(%d) ",
        R_Fld(u4StaFm03, MJC_STA_MC_ORI_EN_HW),
        R_Fld(u4StaFm03, MJC_STA_RESCAN_FRM_EN_HW));
    
    Printf("[%d%d%d%d%d%d] ",
        R_Fld(u4StaFm02, MJC_STA_PAIR_FIRST_HW),
        R_Fld(u4StaFm03, MJC_STA_HQRPT_CHG_HW),
        R_Fld(u4StaFm03, MJC_STA_HQRPT_LST_HW),
        R_Fld(u4StaFm03, MJC_STA_SEC_HQRPT_CHG_HW),
        R_Fld(u4StaFm03, MJC_STA_FST_HQRPT_GP_HW),
        R_Fld(u4StaFm03, MJC_STA_MVEXP_FLAG_HW));

    Printf("%d/%d/%d/%d ",
        R_Fld(u4StaFm03, MJC_STA_ME_24_60_PREBI_HW),
        R_Fld(u4StaFm03, MJC_STA_PPC_FRM_EN_HW),
        R_Fld(u4StaFm03, MJC_STA_PPC_MODE_SEL_HW),
        R_Fld(u4StaFm03, MJC_STA_PPC_INTLV_SEL_HW));

    Printf("%d/%d/%s ",
        R_Fld(u4StaFm03, MJC_STA_ME2_EN_HW),
        R_Fld(u4StaFm03, MJC_STA_ME3_EN_HW),
        _fbStr[R_Fld(u4StaFm03, MJC_STA_PROG_ME_POS_OTHR_SEL_HW)]);

    Printf("[%d%d%d%d] ",
        R_Fld(u4StaFm03, MJC_STA_DA_BWRDC_EN_HW),
        R_Fld(u4StaFm03, MJC_STA_DA_RDCTYPE_HW),
        R_Fld(u4StaFm03, MJC_STA_MEMC_PCO_DECI_EN_HW),
        R_Fld(u4StaFm03, MJC_STA_MEMC_CCO_DECI_EN_HW));

    Printf("3D[%d%d] ",
        R_Fld(u4StaFm03, MJC_STA_TD_LR_TOGGLE_HW),
        R_Fld(u4StaFm03, MJC_STA_TD_LR_SEL_OUT));
}

static void _vDrvMJCFrmSchFWLog(void)
{
    Printf("%d ", _arMJCFrmSchIn.u1PdInfo);
    Printf("%s ", R_Fld(u4WriteFm06, MJC_FW_WRT_FRM_PTR) & 0x10 ? "R" : "L");
    Printf("%d ", R_Fld(u4WriteFm06, MJC_FW_WRT_FRM_PTR) & 0x0F);

    Printf("%s ", R_Fld(u4WriteFm05, MJC_FW_FRM_PREV) & 0x10 ? "R" : "L");
    Printf("%x ", R_Fld(u4WriteFm05, MJC_FW_FRM_PREV) & 0x0F);
    Printf("%x ", R_Fld(u4WriteFm05, MJC_FW_FRM_CURR) & 0x0F);    
    Printf("%x ", R_Fld(u4WriteFm05, MJC_FW_FRM_NEXT) & 0x0F);

    Printf("%d/%02d/%d/%02d/%02d ",
        R_Fld(u4WriteFm05, MJC_FW_ME_FILM_PHS),
        R_Fld(u4WriteFm05, MJC_FW_ME_FILM_IDX),
        R_Fld(u4WriteFm05, MJC_FW_ME_FILM_FLG),
        R_Fld(u4WriteFm05, MJC_FW_INT_NUM),
        R_Fld(u4WriteFm07, MJC_FW_INT_DEN));
        
    Printf("%d(%d) ", 
        R_Fld(u4WriteFm06, MJC_FW_MC_ORI_EN),
        R_Fld(u4WriteFm05, MJC_FW_RESCAN_FRM_EN));
    
    Printf("[%d%d%d%d%d%d] ",
        R_Fld(u4WriteFm06, MJC_FW_PAIR_FIRST),
        R_Fld(u4WriteFm07, MJC_FW_HQRPT_CHG),
        R_Fld(u4WriteFm07, MJC_FW_HQRPT_LST),
        R_Fld(u4WriteFm07, MJC_FW_SEC_HQRPT_CHG),
        R_Fld(u4WriteFm07, MJC_FW_FST_HQRPT_GP),
        R_Fld(u4WriteFm06, MJC_FW_MVEXP_FLAG));

    Printf("%d/%d/%d/%d ",
        R_Fld(u4WriteFm06, MJC_FW_ME_24_60_PREBI),
        R_Fld(u4WriteFm06, MJC_FW_PPC_FRM_EN),
        R_Fld(u4WriteFm06, MJC_FW_PPC_MODE_SEL),
        R_Fld(u4WriteFm06, MJC_FW_PPC_INTLV_SEL));

    Printf("%d/%d/%s ",
        R_Fld(u4WriteFm06, MJC_FW_ME2_EN),
        R_Fld(u4WriteFm06, MJC_FW_ME3_EN),
        _fbStr[R_Fld(u4WriteFm06, MJC_FW_ME_POS_OTHR_SEL)]);

    Printf("[%d%d%d%d] ",
        R_Fld(u4WriteFm06, MJC_FW_DA_BWRDC_EN),
        R_Fld(u4WriteFm06, MJC_FW_DA_RDCTYPE),
        R_Fld(u4WriteFm06, MJC_FW_MEMC_PCO_DECI_EN),
        R_Fld(u4WriteFm06, MJC_FW_MEMC_CCO_DECI_EN));

    Printf("3D[%d%d] ",
        R_Fld(u4WriteFm06, MJC_FW_TD_LR_TOGGLE),
        R_Fld(u4WriteFm05, MJC_FW_TD_LR_SEL_OUT));
}

extern void vDrvMJCSetPsAutoTrigOn(void);

static void vDrvMJCFrmSchFlushBuf(void)
{
    UINT32 i;
    for (i = 0; i < PD_INFO_BUF_SIZE; i++)
    {
        _u1SwLPdInfoBuf[i] = 0;
        //_u1SwLFilmTypeBuf[i] = VIDEO_TYPE;
        _u1SwRPdInfoBuf[i] = 0;
        //_u1SwRFilmTypeBuf[i] = VIDEO_TYPE;
    }

    _arMJCFrmSchIn.u1PdInfo = 0;
    _arMJCFrmSchIn.u1FilmType = VIDEO_TYPE;
    _arMJCFrmSchIn.u1LDetFilmMode = VIDEO_TYPE;
    _arMJCFrmSchIn.u1RDetFilmMode = VIDEO_TYPE;
}

static void vDrvMJCFwHwSwitch(void)
{
    //#ifndef CC_FPGA
    UINT8  u1HwFilmMode;
	
    _u1HwPdInfo = MJC_READ_FLD(MJCFD_08, MJCFD_FD_INFO);

	if (_u1HwPdInfo > 5)
	{
		u1HwFilmMode = FILM_TYPE_22;
	}
	else if (_u1HwPdInfo > 0)
	{
		u1HwFilmMode = FILM_TYPE_32;
	}
	else
	{
		u1HwFilmMode = VIDEO_TYPE;
	}

    vIO32WriteFldAlign(MJC_FW_SCH_00, u1HwFilmMode, MJC_RO_FRM_SCH_HW_DET_FILM_TYPE);

	if (_u1SwFrmSchCtrl == FRM_SCH_CTRL_AUTO) // HW/SW frm sch switch
	{
        if (_u1SwitchLock == 0)
        {
            if (((u1HwFilmMode == VIDEO_TYPE) && (_arMJCFrmSchIn.u1LDetFilmMode <= FILM_TYPE_22323))||
                ((u1HwFilmMode == FILM_TYPE_22) && (_arMJCFrmSchIn.u1LDetFilmMode == FILM_TYPE_2224)))
    		{
    		    if (!(_u1SwFrmSchEnCurr & 0x01))
                {   
                    _u1SwFrmSchEnCurr = 0x03;   // trigger to enable
                    _u1SwitchLock = 1;
                    _u1SwForceVdo = 1;
                    // turn off hw and sw film mode
                    // reset sw film mode buf
                    vDrvMJCSetPSAutoOnOff(SV_OFF);
                    vDrvMJCFrmSchFlushBuf();
                }
    		}
            else
    		{
    		    if (_u1SwFrmSchEnCurr & 0x01)
                {
                    _u1SwFrmSchEnCurr = 0x02;   // trigger to disable
                    _u1SwitchLock = 1;
                    _u1SwForceVdo = 1;
                    // turn off hw and sw film mode
                    // reset sw film mode buf
                    vDrvMJCSetPSAutoOnOff(SV_OFF);
                    vDrvMJCFrmSchFlushBuf();
                }
    		}
        }			
	}
	//#endif
}

static void vDrvMJCFrmSchFilmDetect(void)
{
	UINT32 u4StaLr3dInput = 0;
	UINT32 u4DetFilmMode;
	UINT16 u2MskSeq, u2RefSeq;

	UINT32 u4FilmModeType;
	UINT32 u4FilmModeSwitch;
	UINT32 u4FilmModeIdx = 0;

	UINT8  u1TempPdInfo = 0;
	UINT16 u2MotionSeq;

    #if 0
	UINT16 u2PMotSeq, u2VSubSeq, u2HSubSeq;

    u2PMotSeq = MJC_READ_FLD(MJC_FD_0A, MJC_STA_DIF_SEQ);   
    u2VSubSeq = MJC_READ_FLD(MJC_FD_09, MJC_STA_VST_SEQ);   
    u2HSubSeq = MJC_READ_FLD(MJC_FD_09, MJC_STA_SAW_SEQ);

	u2MotionSeq = u2PMotSeq | u2VSubSeq | u2HSubSeq;
	#endif

	if (arMJCPrm.fgIsInterlace)
    {
        u2MotionSeq   = RegReadFldAlign(MCVP_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
	    u4DetFilmMode = _arMJCFrmSchIn.u1RDetFilmMode;
    }
    else
    {
        u4StaLr3dInput = MJC_READ_FLD(MJC_STA_FM_03, MJC_STA_LR3D_I) & _u1Sw3DMode;
	    if (u4StaLr3dInput)
	    {
	        u2MotionSeq   = RegReadFldAlign(MJCFD_SEQ, MJCFD_FD_DET_SEQ_R);
	        u4DetFilmMode = _arMJCFrmSchIn.u1RDetFilmMode;
	    }
	    else
	    {
    	    u2MotionSeq   = RegReadFldAlign(MJCFD_SEQ, MJCFD_FD_DET_SEQ_L);
	        u4DetFilmMode = _arMJCFrmSchIn.u1LDetFilmMode;
        }
    }
	// FW film mode detection
	switch (u4DetFilmMode)
	{
	default:
	case VIDEO_TYPE:
	    #if SUPPORT_UNIVERSAL_CADENCE
	    for (u4FilmModeType = FILM_TYPE_22323; u4FilmModeType < VIDEO_TYPE; u4FilmModeType++)
        #else
	    for (u4FilmModeType = FILM_TYPE_32; u4FilmModeType < VIDEO_TYPE; u4FilmModeType++)
	    #endif
	    {
	        u2MskSeq = _arMskSeq[u4FilmModeType];
	        u2RefSeq = _arRefSeq[u4FilmModeType];
	        u4FilmModeSwitch = _u1SwAutoDet & (1 << u4FilmModeType);
	        if (u4FilmModeSwitch == 0)
	        {
	            continue;
	        }

	        if ((u2MotionSeq & u2MskSeq) == u2RefSeq)
	        {
	            u4DetFilmMode = u4FilmModeType;
	            u1TempPdInfo  = _arEnterPoint[u4FilmModeType];
	            u4FilmModeIdx = _arEnterIdx[u4FilmModeType];
	            break;
	        }
	    }

	    if (u4FilmModeType == VIDEO_TYPE)
	    {
            u4DetFilmMode = VIDEO_TYPE;
            u1TempPdInfo  = 0;
            u4FilmModeIdx = 0;
	    }
	    break;

    #if SUPPORT_UNIVERSAL_CADENCE		
	case FILM_TYPE_22323:
	case FILM_TYPE_3223:
	case FILM_TYPE_2224:
    #endif
	case FILM_TYPE_32:
	case FILM_TYPE_22:
	    u4FilmModeIdx = u4StaLr3dInput ? u1RIdx : u1LIdx;
	    u2MskSeq = _arMskSeq[u4DetFilmMode];
	    u2RefSeq = _parSeqFilm[u4DetFilmMode][u4FilmModeIdx];
	    u2RefSeq = (~(u2RefSeq)) & u2MskSeq;
        u4FilmModeSwitch = _u1SwAutoDet & (1 << u4DetFilmMode);
		
		if (((u2MotionSeq & u2MskSeq) & u2RefSeq) || (u4FilmModeSwitch == 0))
		{
			u4DetFilmMode = VIDEO_TYPE;
			u1TempPdInfo  = 0;
			u4FilmModeIdx = 0;
		}
		else
		{
			u1TempPdInfo = _arPdInfoStart[u4DetFilmMode] + u4FilmModeIdx;
			
			u4FilmModeIdx++;
			
			if (u4FilmModeIdx >= _arPdInfoSize[u4DetFilmMode])
			{
				u4FilmModeIdx = 0;
			}
		}
		break;
	}

    _u1HwWrtPtr = MJC_READ_FLD(MJC_STA_FM_03, MJC_STA_WRT_FRM_PTR_HW) & 0xF;
    _u1FwFrmWrtPtr  = (_u1HwWrtPtr == 8) ? 0 : (_u1HwWrtPtr + 1);

	if (u4StaLr3dInput)
	{
	    _arMJCFrmSchIn.u1RDetFilmMode = (UINT8)u4DetFilmMode;
	    u1RIdx                        = (UINT8)u4FilmModeIdx;
    	_u1SwRPdInfoBuf[_u1RPdInfoIdx]   = u1TempPdInfo;
        _u1SwRWrtPtrBuf[_u1RPdInfoIdx]   = _u1HwWrtPtr | 0x10;
        //_u1SwRFilmTypeBuf[_u1RPdInfoIdx] = u4DetFilmMode;
        _u1RPdInfoIdx++;
    	if (_u1RPdInfoIdx >= PD_INFO_BUF_SIZE)
    	{
    		_u1RPdInfoIdx = 0;
    	}
        vIO32WriteFldAlign(MJC_FW_SCH_02, u1TempPdInfo, MJC_FW_FRM_SCH_PD_R);
	}
	else
	{
	    _arMJCFrmSchIn.u1LDetFilmMode    = u4DetFilmMode;
	    u1LIdx                           = (UINT8)u4FilmModeIdx;
    	_u1SwLPdInfoBuf[_u1LPdInfoIdx]   = u1TempPdInfo;
        _u1SwLWrtPtrBuf[_u1LPdInfoIdx]   = _u1HwWrtPtr;
        //_u1SwLFilmTypeBuf[_u1LPdInfoIdx] = u4DetFilmMode;
        _u1LPdInfoIdx++;
    	if (_u1LPdInfoIdx >= PD_INFO_BUF_SIZE)
    	{
    		_u1LPdInfoIdx = 0;
    	}
    	if (_u1Sw3DMode)
    	{
    	    _u1FwFrmWrtPtr = _u1HwWrtPtr | 0x10;
    	}
        vIO32WriteFldAlign(MJC_FW_SCH_02, u1TempPdInfo, MJC_FW_FRM_SCH_PD_L);
	}
}


static void vDrvMJCFrmSchInputPara(UINT8 u1FrmRateMultiple)
{
    UINT32 u4FilmType = _arMJCFrmSchIn.u1FilmType;
    UINT32 u4FrcIdx = u1FrmRateMultiple >> 1;

    //_arMJCFrmSchIn.u1InWrpt = MJC_READ_FLD(MJC_STA_FM_03, MJC_STA_WRT_FRM_PTR_HW);

    _arMJCFrmSchIn.u1PdInfoNormalize = _arMJCFrmSchIn.u1PdInfo - _arPdInfoStart[u4FilmType];
    _arMJCFrmSchIn.u1FrmIn = _arFrmSchIO[u4FrcIdx][u4FilmType].u1FrmIn;
    _arMJCFrmSchIn.u1FrmOut = _arFrmSchIO[u4FrcIdx][u4FilmType].u1FrmOut;
   // _arMJCFrmSchOut.u1IntpDen = _arMJCFrmSchIn.u1FrmOut;

    _arMJCFrmSchIn.u1ParamRdy = 1;
}

static void vDrvMJCFrmSchOutputPara(UINT8 u1FrmRateMultiple, UINT8 u1Idx)
{
    UINT32 u4WrtPtrBufIdx, u4PdInfoBufIdx;
    UINT32 u4FrmSchTblIdx;
    UINT32 u4StaLr3dOutput;
    UINT32 u4PdInfoIdx;
    UINT32 u4FrameDelay;
    UINT32 u43DPdInfoDelay;
    
	MJC_FM_05_U unFm05;
	MJC_FM_06_U unFm06;
	MJC_FM_07_U unFm07;
	UINT8 u1BufSize ;
	
    u4StaLr3dOutput = MJC_READ_FLD(MJC_STA_FM_03, MJC_STA_LR3D_I);
    //if (_u1Sw3DMode)
    if (0)
    {
        u4StaLr3dOutput = (u4StaLr3dOutput) ? 0 : 1;
    }
    u4PdInfoIdx     = u4StaLr3dOutput ? _u1RPdInfoIdx : _u1LPdInfoIdx;
    if (_u1Sw3DMode)
    {
        u43DPdInfoDelay = _u1PdInfoDelay >> 1;
        u43DPdInfoDelay = u4StaLr3dOutput ? u43DPdInfoDelay : (u43DPdInfoDelay + (_u1PdInfoDelay & 0x1));
    }
    else
    {
        u43DPdInfoDelay = _u1PdInfoDelay;
    }
    //u4FrameDelay    = (_u1Sw3DMode && (u4StaLr3dOutput == 0)) ? (_u1InputFrameDelay - 1) : _u1InputFrameDelay;
    if (_u1Sw3DMode)
    {
        u4FrameDelay = _u1InputFrameDelay >> 1;
        u4FrameDelay = u4StaLr3dOutput ? u4FrameDelay :  (u4FrameDelay + (_u1InputFrameDelay & 0x1));
    }
    else
    {
        u4FrameDelay = _u1InputFrameDelay;
    }
    
    u4WrtPtrBufIdx = (u4PdInfoIdx < u4FrameDelay) ? (u4PdInfoIdx + PD_INFO_BUF_SIZE) : u4PdInfoIdx;
    u4WrtPtrBufIdx = u4WrtPtrBufIdx - u4FrameDelay;
    u4PdInfoBufIdx = u4WrtPtrBufIdx + u43DPdInfoDelay;
    u4PdInfoBufIdx = (u4PdInfoBufIdx >= PD_INFO_BUF_SIZE) ? (u4PdInfoBufIdx - PD_INFO_BUF_SIZE) : u4PdInfoBufIdx;

    if (_u1SwForceVdo)
    {
        _arMJCFrmSchIn.u1PdInfo = 0;
        _arMJCFrmSchIn.u1InWrpt = u4StaLr3dOutput ? _u1SwRWrtPtrBuf[u4WrtPtrBufIdx] : _u1SwLWrtPtrBuf[u4WrtPtrBufIdx];
    }
    else
    {
        _arMJCFrmSchIn.u1PdInfo = u4StaLr3dOutput ? _u1SwRPdInfoBuf[u4PdInfoBufIdx] : _u1SwLPdInfoBuf[u4PdInfoBufIdx];
        _arMJCFrmSchIn.u1InWrpt = u4StaLr3dOutput ? _u1SwRWrtPtrBuf[u4WrtPtrBufIdx] : _u1SwLWrtPtrBuf[u4WrtPtrBufIdx];
    }

    if (IO32ReadFldAlign(MJC_FW_SCH_00, MJC_FRM_SCH_1X))
    {
        u4FrmSchTblIdx = (_arMJCFrmSchIn.u1PdInfo << 1);
    }
    else
    {
        u4FrmSchTblIdx = (_arMJCFrmSchIn.u1PdInfo << 1) + u1Idx;
    }
    #if 0
    _arMJCFrmSchOut.u1MeFilmFlg   = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_ME_FILM_FLAG];
    _arMJCFrmSchOut.u1MeFilmIdx   = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_ME_FILM_IDX];
    _arMJCFrmSchOut.u1MeFilmPhs   = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_ME_FILM_PHS];
    _arMJCFrmSchOut.u1RescanFrmEn = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_RESCAN_FRM_EN];
    _arMJCFrmSchOut.u1PPCFrmEn    = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_PPC_FRM_EN];
    _arMJCFrmSchOut.u1PPCModeSel  = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_PPC_MODE_SEL];
    _arMJCFrmSchOut.u1Me2En       = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_ME2_EN];
    _arMJCFrmSchOut.u1Me3En       = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_ME3_EN];
    _arMJCFrmSchOut.u1DaBwrdcEn   = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_DA_BWRDC_EN];
    _arMJCFrmSchOut.u1DaRdctype   = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_DA_RDCTYPE];
    _arMJCFrmSchOut.u1IntpNum     = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_INTP_NUM];
    _arMJCFrmSchOut.u1IntpDen     = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_INTP_DEN];
    _arMJCFrmSchOut.u1McOriEn     = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_MC_ORI_EN];
    _arMJCFrmSchOut.u1MemcCcoDeci = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_MEMC_CC0_DECI];
    _arMJCFrmSchOut.u1MemcPcoDeci = arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_MEMC_PCO_DECI];
    _arMJCFrmSchOut.u1FrmPrev     = _arMJCFrmSchIn.u1InWrpt + arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_PREV_OFST];
    _arMJCFrmSchOut.u1FrmCurr     = _arMJCFrmSchIn.u1InWrpt + arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_CURR_OFST];
    _arMJCFrmSchOut.u1FrmNext     = _arMJCFrmSchIn.u1InWrpt + arFrmSchSetting[u4FrmSchTblIdx][FRM_SCH_NEXT_OFST];
    _arMJCFrmSchOut.u1FrmPrev     = (_arMJCFrmSchOut.u1FrmPrev >= 9) ? (_arMJCFrmSchOut.u1FrmPrev - 9) : _arMJCFrmSchOut.u1FrmPrev;
    _arMJCFrmSchOut.u1FrmCurr     = (_arMJCFrmSchOut.u1FrmCurr >= 9) ? (_arMJCFrmSchOut.u1FrmCurr - 9) : _arMJCFrmSchOut.u1FrmCurr;
    _arMJCFrmSchOut.u1FrmNext     = (_arMJCFrmSchOut.u1FrmNext >= 9) ? (_arMJCFrmSchOut.u1FrmNext - 9) : _arMJCFrmSchOut.u1FrmNext;
    
    _arMJCFrmSchOut.u1MePosSel = 1; // currently bi-direction only
    _arMJCFrmSchOut.u1TdLrSelOut = 0; // force 2D
    _arMJCFrmSchOut.u1TdLrToggle = 0; // force 2D
    _arMJCFrmSchOut.u1WrtCtrlEn = 0; // force 2D
    _arMJCFrmSchOut.u1WrtFrmEn = 0; // force 2D
    _arMJCFrmSchOut.u1WrtFrmPtr = 0; // force 2D
    _arMJCFrmSchOut.u1PairFirst = 0;
    _arMJCFrmSchOut.u1PPCIntlvSel = 0;
    _arMJCFrmSchOut.u1Fallback64 = 0;
    _arMJCFrmSchOut.u1MvexpFlag = 0;
    _arMJCFrmSchOut.u1Me2460Prebi = 0;
    #endif
    u1BufSize = u4StaLr3dOutput ? 25 : 9;
	
    unFm05.arFm05 = _arMJCFm05Tbl[u4FrmSchTblIdx];
    unFm06.arFm06 = _arMJCFm06Tbl[u4FrmSchTblIdx];
    unFm07.arFm07 = _arMJCFm07Tbl[u4FrmSchTblIdx];
    
    unFm05.arFm05.u1FwPrevShift += _arMJCFrmSchIn.u1InWrpt;
    unFm05.arFm05.u1FwCurrShift += _arMJCFrmSchIn.u1InWrpt;
    unFm05.arFm05.u1FwNextShift += _arMJCFrmSchIn.u1InWrpt;
    unFm05.arFm05.u1FwPrevShift  = (unFm05.arFm05.u1FwPrevShift >= u1BufSize) ? (unFm05.arFm05.u1FwPrevShift - 9) : unFm05.arFm05.u1FwPrevShift;
    unFm05.arFm05.u1FwCurrShift  = (unFm05.arFm05.u1FwCurrShift >= u1BufSize) ? (unFm05.arFm05.u1FwCurrShift - 9) : unFm05.arFm05.u1FwCurrShift;
    unFm05.arFm05.u1FwNextShift  = (unFm05.arFm05.u1FwNextShift >= u1BufSize) ? (unFm05.arFm05.u1FwNextShift - 9) : unFm05.arFm05.u1FwNextShift;

    unFm05.arFm05.u1FwTdLrSelOut = u4StaLr3dOutput;
    unFm06.arFm06.u1FwTdLrToggle = _u1Sw3DMode;

    if (IO32ReadFldAlign(MJC_FW_SCH_00, MJC_FRM_SCH_1X))
    {
        unFm05.arFm05.u1FwMeFilmIdx = unFm05.arFm05.u1FwMeFilmIdx >> 1;
        unFm05.arFm05.u1FwMeFilmPhs = unFm05.arFm05.u1FwMeFilmPhs ? 0 : 1;
    }

    u4WriteFm05 = unFm05.u4Fm05Reg;
    u4WriteFm06 = unFm06.u4Fm06Reg;
    u4WriteFm07 = unFm07.u4Fm07Reg;

    MJC_WRITE_4BMSK(MJC_FM_05, u4WriteFm05, 0xFFFFFFF8);
    MJC_WRITE_4BMSK(MJC_FM_06, u4WriteFm06, 0x80EFFFFF);
    MJC_WRITE_4BMSK(MJC_FM_07, u4WriteFm07, 0xCFFE0000);
}

static void vDrvMJCFrmSchApply(UINT8 u1IdxInter)
{
    static UINT8 _u1FrmSchCtrlEn = 0xff;
    UINT8 u1En, u1Idx;
    //UINT32 u4WriteFm05, u4WriteFm06, u4WriteFm07;

    //return;
	
    MJC_WRITE_FLD(MJC_FM_05, 0, MJC_FW_SET_OK);
    //MJC_WRITE_FLD(MJC_FM_07, _arMJCFrmSchOut.u1FrmDysw, MJC_FW_FRM_DYSW);

    #if 0
	u4WriteFm05  = 0x0;
	u4WriteFm05 |= (_arMJCFrmSchOut.u1RescanFrmEn << 3);
	u4WriteFm05 |= (_arMJCFrmSchOut.u1FrmPrev     << 4);
	u4WriteFm05 |= (_arMJCFrmSchOut.u1FrmCurr     << 9);
	u4WriteFm05 |= (_arMJCFrmSchOut.u1FrmNext     << 14);
	u4WriteFm05 |= (_arMJCFrmSchOut.u1MeFilmIdx   << 19);
	u4WriteFm05 |= (_arMJCFrmSchOut.u1MeFilmPhs   << 23);
    u4WriteFm05 |= (_arMJCFrmSchOut.u1MeFilmFlg   << 24);
    u4WriteFm05 |= (_arMJCFrmSchOut.u1IntpNum     << 26);
    u4WriteFm05 |= (_arMJCFrmSchOut.u1TdLrSelOut  << 31);
    //MJC_WRITE_4BMSK(MJC_FM_05, u4WriteFm05, 0xFFFFFFF8);
	
    u4WriteFm06  = 0x0;
    u4WriteFm06 |= (_arMJCFrmSchOut.u1Me2En       << 0);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1Me3En       << 1);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1DaBwrdcEn   << 2);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1DaRdctype   << 3);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1McNum       << 05);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1MePosSel    << 13);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1PPCModeSel  << 15);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1McOriEn     << 16);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1PPCFrmEn    << 17);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1Me2460Prebi << 18);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1MvexpFlag   << 19);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1Fallback64  << 21);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1PPCIntlvSel << 22);
	u4WriteFm06 |= (_arMJCFrmSchOut.u1PairFirst   << 23);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1WrtFrmEn    << 24);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1WrtFrmPtr   << 25);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1WrtCtrlEn   << 30);
    u4WriteFm06 |= (_arMJCFrmSchOut.u1TdLrToggle  << 31);
    //MJC_WRITE_4BMSK(MJC_FM_06, u4WriteFm06, 0xFFEFFFFF);

    u4WriteFm07  = 0x0;
    u4WriteFm07 |= (_arMJCFrmSchOut.u1MemcPcoDeci  << 17);
    u4WriteFm07 |= (_arMJCFrmSchOut.u1MemcCcoDeci  << 18);
    u4WriteFm07 |= (_arMJCFrmSchOut.u1IntpDen      << 19);
    u4WriteFm07 |= (_arMJCFrmSchOut.u1Fallback55P2 << 23);
    u4WriteFm07 |= (_arMJCFrmSchOut.u1HqrptChg     << 24);
    u4WriteFm07 |= (_arMJCFrmSchOut.u1SecHqrptChg  << 25);
    u4WriteFm07 |= (_arMJCFrmSchOut.u1FstHqrptGp   << 26);
    u4WriteFm07 |= (_arMJCFrmSchOut.u1HqrptLst     << 27);
     u4WriteFm07 |= (_arMJCFrmSchOut.u1VideoNoIntp  << 30);
    u4WriteFm07 |= (_arMJCFrmSchOut.u1FrmChgTtd    << 31);
    //MJC_WRITE_4BMSK(MJC_FM_07, u4WriteFm07, 0xFFFE0000);
    #endif

	MJC_WRITE_FLD(MJC_FM_03, 1, MJC_FW_SET_OK);

    if (_u1SwFrmSchCtrl != FRM_SCH_CTRL_AUTO)
    {
        u1En = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_CTRL_EN);
        u1Idx = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_CTRL_IDX);

        if ((u1En != _u1FrmSchCtrlEn) && (u1Idx == u1IdxInter))
        {
            _u1FrmSchCtrlEn = u1En;
            MJC_WRITE_FLD(MJC_FM_05, u1En, MJC_FW_FRM_SCH_EN);
            MJC_WRITE_FLD(MJC_MC_21, u1En, MJC_INTP_POS_FW_NUM_EN);

            LOG(1, "MJC Frm Sch: SW(%d)\n", u1En);

            if (R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_DBG_LOG))
            {
                _vDrvMJCFrmSchFWLog();
                Printf("\n");
            }
        }
    }
    else
    {
        u1En = _u1SwFrmSchEnCurr & 0x01;

        if ((_u1SwFrmSchEnCurr & 0x02) &&
            (MJC_READ_FLD(MJC_STA_FM_02, MJC_STA_ME_FILM_FLG_HW) == 0) &&
            (MJC_READ_FLD(MJC_FM_05, MJC_FW_ME_FILM_FLG) == 0)) // ?????
        {
            _u1SwFrmSchEnCurr &= ~0x02; // remove trig
            MJC_WRITE_FLD(MJC_FM_05, u1En, MJC_FW_FRM_SCH_EN);
            MJC_WRITE_FLD(MJC_MC_21, u1En, MJC_INTP_POS_FW_NUM_EN);
            // turn on hw and sw film mode
			vDrvMJCSetPsAutoTrigOn();
            _u1SwForceVdo = 0;
            _u1SwitchLock = 0;

            LOG(1, "MJC Frm Sch: SW(%d)\n", u1En);

            if (R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_DBG_LOG))
            {
                _vDrvMJCFrmSchFWLog();
                Printf("\n");
            }
        }
    }

    // update to read only status
    vIO32WriteFldMulti(MJC_FW_SCH_00,
        P_Fld(_arMJCFrmSchIn.u1LDetFilmMode, MJC_RO_FRM_SCH_SW_DET_FILM_TYPE)|
        P_Fld(_arMJCFrmSchIn.u1FilmType, MJC_RO_FRM_SCH_SW_FILM_TYPE)|
        P_Fld(_u1SwForceVdo, MJC_RO_FRM_SCH_SW_FORCE_VDO)|
        P_Fld(_u1SwitchLock, MJC_RO_FRM_SCH_SW_SWITCH_LOCK)|
        P_Fld(_u1SwFrmSchEnCurr, MJC_RO_FRM_SCH_SW_TRIG_EN));
}

void vDrvMJCFrmSchHWLog(void* pvArgv)
{
    _vDrvMJCFrmSchHWLog();
}

void vDrvMJCFrmSchFWLog(void* pvArgv)
{
    _vDrvMJCFrmSchFWLog();
}

static BOOL fgDrvMJCFrmSchISREnProc(void)
{
    static UINT8 _u1FrmSchIsrEn = 0xFF;
    UINT8 u1En = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_ISR_EN);

    if (u1En != _u1FrmSchIsrEn)
    {
        _u1FrmSchIsrEn = u1En;
    }

    return u1En;
}

static void vDrvMJCFrmSchGetCtrl(void)
{
    _u4FrmSchCtrl = u4IO32Read4B(MJC_FW_SCH_01);

    _u1FrmIORatio = 2;//arMJCPrm.arFRCConfig.u1Is120HzPanel ? 2 : 1;

    _fgFrmWrtPtrCtrl = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_WRT_PTR_CTRL);

    _u1SwAutoDet = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_AUTO_DET);
    _u1Sw3DMode  = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_3D_MODE);
    _u1InputFrameDelay = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_IN_DLY);
    _u1PdInfoDelay = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_PD_INFO_DLY);
    //_u1TestFrameDelay = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_TST_DLY);
    _u1SwFrmSchCtrl = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_SWITCH);
    //_arMJCFrmSchOut.u1FrmDysw = R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_DYSW);
}

void vDrvMJCFrmSchSetPdInfoDly(UINT32 u4Delay)
{
    vIO32WriteFldAlign(MJC_FW_SCH_01, u4Delay, MJC_FW_FRM_SCH_PD_INFO_DLY);
}

void vDrvMJCFrmSchInputVsyncProc(void)
{
    static UINT8 fgFrmWrtPtrCtrlPrev = 0xFF;
	_u1IsInterFlag = 0;

    if (!fgDrvMJCFrmSchISREnProc())
        return;

    if (fgFrmWrtPtrCtrlPrev != _fgFrmWrtPtrCtrl)
    {
        MJC_WRITE_FLD(MJC_FM_06, _fgFrmWrtPtrCtrl, MJC_FW_WRT_CTRL_EN);
        fgFrmWrtPtrCtrlPrev = _fgFrmWrtPtrCtrl;
    }

	vDrvMJCFrmSchFilmDetect();
	vDrvMJCFwHwSwitch();
	vDrvMJCFrmSchInputPara(_u1FrmIORatio);
}

void vDrvMJCFrmSchOutputVsyncProc(void)
{
	UINT8 u1StaFwIsTooLate;

    vDrvMJCFrmSchGetCtrl();
    
    if ((fgDrvMJCFrmSchISREnProc() == SV_OFF) || (_arMJCFrmSchIn.u1ParamRdy == SV_FALSE))
        return;

    if (R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_LOG) && R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_LOG_HW))
    {
        Printf("HW: ");
        _vDrvMJCFrmSchHWLog();
        Printf("\n");
    }
    
	if (_u1IsInterFlag)
	{
		_u1IsInter = 1;
	}
	else
	{
		_u1IsInter = 0;
		_u1IsInterFlag = 1;
	}

    //if (_u1IsInter)
    {
        MJC_WRITE_FLD(MJC_FM_06, _u1FwFrmWrtPtr, MJC_FW_WRT_FRM_PTR);
    }
    
    // Read FW frame scheduler control value
	vDrvMJCFrmSchOutputPara(_u1FrmIORatio, _u1IsInter);
    if (R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_LOG) && R_Fld(_u4FrmSchCtrl, MJC_FW_FRM_SCH_LOG_SW))
    {
        Printf("FW: ");
        _vDrvMJCFrmSchFWLog();
        Printf("\n");
    }

	vDrvMJCFrmSchApply(_u1IsInter);

	u1StaFwIsTooLate = MJC_READ_FLD(MJC_STA_FM_03, MJC_STA_FW_IS_TOO_LATE);
	
	if (u1StaFwIsTooLate)
	{
		//Printf("MJC FW Frm sch not finish !\n");
		MJC_WRITE_FLD(MJC_FM_05, 1, MJC_CLEAR_FW_TOO_LATE);
        MJC_WRITE_FLD(MJC_FM_05, 0, MJC_CLEAR_FW_TOO_LATE);
	}
}

void vDrvMJCFrmSchInit(void)
{
	//UINT8  bi, bj;
	//UINT16 u2Temp32[5], u2Temp22[2];

    vIO32Write4B(MJC_FW_SCH_01, 0x7E010602);

	_u1SwFrmSchCtrl = 0;// 0: always HW, 1: always SW, 2: HW/SW switch
	_u1SwitchLock = 0;

	_u1SwFrmSchEnCurr = 0;
	_u1SwAutoDet = 0x1F; // [0]: 32, [1]: 22, [2]: 22323, [3]: 3223, [4]: 2224

    _arMJCFrmSchIn.u1FrmOut = 1;   // video
	_u1HwPdInfo = 0;
    vDrvMJCFrmSchFlushBuf();		

    
	u1PeriodOfSeq32 = 2;
	u1PeriodOfSeq22 = 6;
	//u1PeriodOfSeq22323 = 1;
	//u1PeriodOfSeq3223 = 1;
	//u1PeriodOfSeq2224 = 1;

	//u2RefSeq32 = 0x12;// 5 : 10010
	//u2RefSeq22 = 0x2 ;// 7 : 10 
	
	u2MskSeq32 = 0x1F; // 11111
	u2MskSeq22 = 0x3;  // 11
	
	//u1Idx32 = 0;
	//u1Idx22 = 0;

	u1Idx22323 = 0;
	u1Idx3223 = 0;
	u1Idx2224 = 0;

	_u1LPdInfoIdx = 0;
	_u1RPdInfoIdx = 0;
	u1FilmOrder = 0;
	
#if 1 //((MJC_FPGA_OUTPUT_TIMING == FPGA_OUTPUT_480P120) || (MJC_FPGA_OUTPUT_TIMING == FPGA_OUTPUT_576P100))
	_u1FrmIORatio = 2; // 1: 60->60, 2:60->120, 4: 60->240
#else
	_u1FrmIORatio = 1; // 1: 60->60, 2:60->120, 4: 60->240
#endif

	_u1IsInter = 0;
	_u1IsInterFlag = 0;

	_u1InputFrameDelay = 6;
	_u1PdInfoDelay = 1;
    //_u1TestFrameDelay = 2;

	if (u1PeriodOfSeq22 > 8)
	{
		u1PeriodOfSeq22 = 8;
	}

	if (u1PeriodOfSeq32 > 3)
	{
		u1PeriodOfSeq32 = 3;
	}

    #if 0
	for (bj = 0; bj < 5; bj++)
	{
		u2Temp32[bj] = arSeqFilm32[bj];
	}

	for (bj = 0; bj < 2; bj++)
	{
		u2Temp22[bj] = arSeqFilm22[bj];
	}

	for (bi = 1; bi < u1PeriodOfSeq32; bi++)
	{
		//u2RefSeq32 = (u2RefSeq32 << 5) + 0x12;
		
		for (bj = 0; bj < 5; bj++)
		{
			arSeqFilm32[bj] = (arSeqFilm32[bj] << 5) + u2Temp32[bj];
		}
		
		u2MskSeq32 = (u2MskSeq32 << 5) + 0x1F;		
	}
	
	//u2RefSeq32 = arSeqFilm32[4];
	u2RefSeq32 = arSeqFilm32[0]; // 32 enter

	for (bi = 1; bi < u1PeriodOfSeq22; bi++)
	{
		//u2RefSeq22 = (u2RefSeq22 << 2) + 0x2;

		for (bj = 0; bj < 2; bj++)
		{
			arSeqFilm22[bj] = (arSeqFilm22[bj] << 2) + u2Temp22[bj];
		}		

		u2MskSeq22 = (u2MskSeq22 << 2) + 0x3;		
	}
	#endif
	//u2RefSeq22 = arSeqFilm22[1];
	u2RefSeq22 = arSeqFilm22[0]; // 22 enter	
	_fgFrmWrtPtrCtrl = 0;
	MJC_WRITE_FLD(MJC_FM_07, 2, MJC_FW_FRM_DYSW); // Output Vsync
	MJC_WRITE_FLD(MJC_FM_15, 0x0, MJC_FW_FRM_SCH_CTRL);
	MJC_WRITE_FLD(MJC_FM_06, 0x0, MJC_FW_WRT_CTRL_EN);
	MJC_WRITE_FLD(MJC_FM_06, 0x1, MJC_FW_WRT_FRM_EN);
}

