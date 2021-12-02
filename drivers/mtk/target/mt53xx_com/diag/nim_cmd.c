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
#include "x_lint.h"

LINT_SAVE_AND_DISABLE


#include "x_printf.h"
#include "x_stl_lib.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_timer.h"
#include "nim_drvif.h"
#include "x_tuner.h"
#include "tuner_if.h"

#ifdef CC_MT5381
//---------------------------------------------------------------------
// LOG macro related

#define DEFINE_IS_LOG       NIM_IsLog
#include "x_debug.h"

CLIMOD_DEBUG_FUNCTIONS(NIM)
#endif

/******************************************************************************
* Macro         : helper macros to reduce code
******************************************************************************/

#define MA_CHK_NUM_MIN_PARAM(N)                \
    do                                         \
    {                                          \
        if (i4Argc < ((N)+1))                  \
        {                                      \
            return -(INT32)E_CLIGFX_ERR_PARAM; \
        }                                      \
    } while(0)

typedef enum _ENUM_CLINIM_ERR_CODE_T
{
    E_CLINIM_OK = 0,
    E_CLINIM_ERR_GENERAL = 1000,
    E_CLINIM_ERR_PARAM = 1001,
    // add here
    E_CLINIM_UNKNOWN = 1999
}

ENUM_CLINIM_ERR_CODE_T;

typedef struct _TEXT2ENUM_T
{
    CHAR *szText;
    INT32 i4Value;
} TEXT2ENUM_T;

typedef enum
{
    DEMOD_ATSC,
    DEMOD_CABLE,
    DEMOD_DVBT,
    DEMOD_ISDBT,
    DEMOD_T_ANA,
    DEMOD_C_ANA,
    DEMOD_DVBS,
    DEMOD_S_ANA,
    DEMOD_DTMB,
    DEMOD_NS,                           // ATSC & CABLE & DVBT & ISDBT
} DEMOD_TYPE_T;

#define ATSC_CH_NS              70          // 0~69
#define CABLE_CH_NS             160         // 0~159
#define DVBT_CH_NS              69          // 0~68
#define ISDBT_CH_NS             69          // 0~68
#define NTSCRF_CH_NS            69
#define NTSCCABLE_CH_NS         125
#define DVBS_CH_NS              160
#define NTSCDVBS_CH_NS          160
#define DTMB_CH_NS              69

#define ATSC_CH_DEFAULT         24
#define CABLE_CH_DEFAULT        83
#define DVBT_CH_DEFAULT         53          // 730MHz
#define ISDBT_CH_DEFAULT        16          // 491MHz
#define T_ANA_CH_DEFAULT        24
#define C_ANA_CH_DEFAULT        83
#define DVBS_CH_DEFAULT         83
#define S_ANA_CH_DEFAULT        83
#define DTMB_CH_DEFAULT         53 
#define MIN_FREQ                50000
#define MAX_FREQ                1050000

/******************************************************************************
* Prototype     : cli "nim" commands
******************************************************************************/

CLI_EXEC_T *GetNimCmdTbl(void);
extern UINT8 DynamicSwitch_DTD_DVBT_Mode;/*BRDCST_IF= DTDDVBT_ALL_P_analog for SLT, DTD: DynamicSwitchMode=0,DVBT: DynamicSwitchMode=1(default)*/
extern UINT16 NIM_GetTunerID(void);

extern void NIM_SetTunerID(UINT16 TunerID);

extern void NIM_CmdVer(void);

extern void NIM_SetConnType(DRV_CONN_TYPE_T  e_conn_type);

extern void NIM_SetMod(TUNER_MODULATION_T Mod);

extern UCHAR NIM_GetDbgLvl(void);

extern void NIM_SetDbgLvl(UCHAR dbg_lvl);

extern void NIM_CmdHostCmd(INT32 i4Argc, const CHAR ** szArgv);
extern void NIM_DetachMW(BOOL bDetach);

static INT32 _NimCmdInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdOpen(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdClose(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdSetCable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdGetCableLevel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdGetCableSignal(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdGetCableSync(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdDbgLvl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdHostCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdDtdCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdAtdCmd(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdGetTsFmt(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdSetTsSP(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdSetTsTristate(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdSetTuner(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdGetTuner(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdGetTunerEx(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdScanTuner(INT32 i4Argc, const CHAR **szArgv);

#if !defined(__linux__)
static void _NimConfig(void);
#else
static BOOL _NimConfig(void);
#endif
static INT32 _NimCmdSetID(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdVer(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdGo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdChannel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdFreq(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdUp(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdDown(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdDetachI2C(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdSetBW(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdDetachMW(INT32 i4Argc, const CHAR ** szArgv);
#if defined(__linux__)
static INT32 _NimCmdSetConnType(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdSetMod(INT32 i4Argc, const CHAR **szArgv);
static INT32 _NimCmdSetFineTune(INT32 i4Argc, const CHAR **szArgv);
#endif

/******************************************************************************
* Variable      : cli "nim" command table
******************************************************************************/

static UINT32 _au4ChannelNs[DEMOD_NS] =
{
    ATSC_CH_NS, CABLE_CH_NS, DVBT_CH_NS, ISDBT_CH_NS, NTSCRF_CH_NS, NTSCCABLE_CH_NS, DVBS_CH_NS, NTSCDVBS_CH_NS,DTMB_CH_NS
};

static UINT32 _au4ChannelDefault[DEMOD_NS] =
{
    ATSC_CH_DEFAULT, CABLE_CH_DEFAULT, DVBT_CH_DEFAULT, ISDBT_CH_DEFAULT, T_ANA_CH_DEFAULT, C_ANA_CH_DEFAULT, DVBS_CH_DEFAULT ,S_ANA_CH_DEFAULT, DTMB_CH_DEFAULT
};

static CHAR _azDemodName[DEMOD_NS][8] =
{
    "ATSC", "CABLE", "DVBT", "ISDBT", "T_ANA", "C_ANA", "DVBS", "S_ANA","DTMB"
};

static UINT32 _au4AtscChannelInKhz[ATSC_CH_NS] =
{
    0,      51000,  57000,  63000,  69000,  79000,  85000,  177000, 183000, 189000, 
    195000, 201000, 207000, 213000, 473000, 479000, 485000, 491000, 497000, 503000, 
    509000, 515000, 521000, 527000, 533000, 539000, 545000, 551000, 557000, 563000, 
    569000, 575000, 581000, 587000, 593000, 599000, 605000, 611000, 617000, 623000, 
    629000, 635000, 641000, 647000, 653000, 659000, 665000, 671000, 677000, 683000, 
    689000, 695000, 701000, 707000, 713000, 719000, 725000, 731000, 737000, 743000, 
    749000, 755000, 761000, 767000, 773000, 779000, 785000, 791000, 797000, 803000
};


static UINT32 _au4CableChannelInKhz[CABLE_CH_NS] =
{
    0,      57000,  63000,  69000,  75000,  79000,  85000,  93000,  99000,  105000, 
    111000, 117000, 123000, 129000, 135000, 141000, 147000, 153000, 159000, 165000, 
    171000, 177000, 183000, 189000, 195000, 201000, 207000, 213000, 219000, 225000, 
    231000, 237000, 243000, 249000, 255000, 261000, 267000, 273000, 279000, 285000, 
    291000, 297000, 303000, 309000, 315000, 321000, 327000, 333000, 339000, 345000, 
    351000, 357000, 363000, 369000, 375000, 381000, 387000, 393000, 399000, 405000, 
    411000, 417000, 423000, 429000, 435000, 441000, 447000, 453000, 459000, 465000, 
    471000, 477000, 483000, 489000, 495000, 501000, 507000, 513000, 519000, 525000, 
    531000, 537000, 543000, 549000, 555000, 561000, 567000, 573000, 579000, 585000, 
    591000, 597000, 603000, 609000, 615000, 621000, 627000, 633000, 639000, 645000, 
    651000, 657000, 663000, 669000, 675000, 681000, 687000, 693000, 699000, 705000, 
    711000, 717000, 723000, 729000, 735000, 741000, 747000, 753000, 759000, 765000, 
    771000, 777000, 783000, 789000, 795000, 801000, 807000, 813000, 819000, 825000, 
    831000, 837000, 843000, 849000, 855000, 861000, 867000, 873000, 879000, 885000, 
    891000, 897000, 903000, 909000, 915000, 921000, 927000, 933000, 939000, 945000, 
    951000, 957000, 963000, 969000, 975000, 981000, 987000, 993000, 999000, 1005000
};

static UINT32 _au4DvbtChannelInKhz[DVBT_CH_NS] =
{
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 
    0,      474000, 482000, 490000, 498000, 506000, 514000, 522000, 530000, 538000, 
    546000, 554000, 562000, 570000, 578000, 586000, 594000, 602000, 610000, 618000, 
    626000, 634000, 642000, 650000, 658000, 666000, 674000, 682000, 690000, 698000, 
    706000, 714000, 722000, 730000, 738000, 746000, 754000, 762000, 770000, 778000, 
    786000, 794000, 802000, 810000, 818000, 826000, 834000, 842000, 850000
};

static UINT32 _au4IsdbtChannelInKhz[ISDBT_CH_NS] =
{ 
    0,      57000,  63000,  69000,  79000,  85000,  177000, 183000, 189000, 195000, 
    201000, 207000, 213000, 473000, 479000, 485000, 491000, 497000, 503000, 509000, 
    515000, 521000, 527000, 533000, 539000, 545000, 551000, 557000, 563000, 569000, 
    575000, 581000, 587000, 593000, 599000, 605000, 0,      617000, 623000, 629000, 
    635000, 641000, 647000, 653000, 659000, 665000, 671000, 677000, 683000, 689000, 
    695000, 701000, 707000, 713000, 719000, 725000, 731000, 737000, 743000, 749000, 
    755000, 761000, 767000, 773000, 779000, 785000, 791000, 797000, 803000
};
    
UINT32 _au4NTSCRFCh[NTSCRF_CH_NS] = /* RTMA M/N System US Off RF Channel */
{
    73250,  55250,  61250,  67250,  77250,  83250,  175250, 181250, 187250, 193250,
    199250, 205250, 211250, 471250, 477250, 483250, 489250, 495250, 501250, 507250,
    513250, 519250, 525250, 531250, 537250, 543250, 549250, 555250, 561250, 567250,
    573250, 579250, 585250, 591250, 597250, 603250, 609250, 615250, 621250, 627250,
    633250, 639250, 645250, 651250, 657250, 663250, 669250, 675250, 681250, 687250,
    693250, 699250, 705250, 711250, 717250, 723250, 729250, 735250, 741250, 747250,
    753250, 759250, 765250, 771250, 777250, 783250, 789250, 795250, 801250
};

UINT32 _au4NTSCCABLECh[NTSCCABLE_CH_NS] = /* RTMA M/N System US Channel CATV Channel */
{ 
    73250,  55250,  61250,  67250,  77250,  83250,  175250, 181250, 187250, 193250,
    199250, 205250, 211250, 121250, 127250, 133250, 139250, 145250, 151250, 157250,
    163250, 169250, 217250, 223250, 229250, 235250, 241250, 247250, 253250, 259250,
    265250, 271250, 277250, 283250, 289250, 295250, 301250, 307250, 313250, 319250,
    325250, 331250, 337250, 343250, 349250, 355250, 361250, 367250, 373250, 379250,
    385250, 391250, 397250, 403250, 409250, 415250, 421250, 427250, 433250, 439250,
    445250, 451250, 457250, 463250, 469250, 475250, 481250, 487250, 493250, 499250,
    505250, 511250, 517250, 523250, 529250, 535250, 541250, 547250, 553250, 559250,
    565250, 571250, 577250, 583250, 589250, 595250, 601250, 607250, 613250, 619250,
    625250, 631250, 637250, 643250, 91250,  97250,  103250, 109250, 115250, 649250,
    655250, 661250, 667250, 673250, 679250, 685250, 691250, 697250, 703250, 709250,
    715250, 721250, 727250, 733250, 739250, 745250, 751250, 757250, 763250, 769250,
    775250, 781250, 787250, 793250, 799250
};

static UINT32 _au4DvbsChannelInKhz[DVBS_CH_NS] =
{
    0,      57000,  63000,  69000,  75000,  79000,  85000,  93000,  99000,  105000, 
    111000, 117000, 123000, 129000, 135000, 141000, 147000, 153000, 159000, 165000, 
    171000, 177000, 183000, 189000, 195000, 201000, 207000, 213000, 219000, 225000, 
    231000, 237000, 243000, 249000, 255000, 261000, 267000, 273000, 279000, 285000, 
    291000, 297000, 303000, 309000, 315000, 321000, 327000, 333000, 339000, 345000, 
    351000, 357000, 363000, 369000, 375000, 381000, 387000, 393000, 399000, 405000, 
    411000, 417000, 423000, 429000, 435000, 441000, 447000, 453000, 459000, 465000, 
    471000, 477000, 483000, 489000, 495000, 501000, 507000, 513000, 519000, 525000, 
    531000, 537000, 543000, 549000, 555000, 561000, 567000, 573000, 579000, 585000, 
    591000, 597000, 603000, 609000, 615000, 621000, 627000, 633000, 639000, 645000, 
    651000, 657000, 663000, 669000, 675000, 681000, 687000, 693000, 699000, 705000, 
    711000, 717000, 723000, 729000, 735000, 741000, 747000, 753000, 759000, 765000, 
    771000, 777000, 783000, 789000, 795000, 801000, 807000, 813000, 819000, 825000, 
    831000, 837000, 843000, 849000, 855000, 861000, 867000, 873000, 879000, 885000, 
    891000, 897000, 903000, 909000, 915000, 921000, 927000, 933000, 939000, 945000, 
    951000, 957000, 963000, 969000, 975000, 981000, 987000, 993000, 999000, 1005000
};

static UINT32 _au4NTSCDVBSCh[NTSCDVBS_CH_NS] =
{
    0,      57000,  63000,  69000,  75000,  79000,  85000,  93000,  99000,  105000, 
    111000, 117000, 123000, 129000, 135000, 141000, 147000, 153000, 159000, 165000, 
    171000, 177000, 183000, 189000, 195000, 201000, 207000, 213000, 219000, 225000, 
    231000, 237000, 243000, 249000, 255000, 261000, 267000, 273000, 279000, 285000, 
    291000, 297000, 303000, 309000, 315000, 321000, 327000, 333000, 339000, 345000, 
    351000, 357000, 363000, 369000, 375000, 381000, 387000, 393000, 399000, 405000, 
    411000, 417000, 423000, 429000, 435000, 441000, 447000, 453000, 459000, 465000, 
    471000, 477000, 483000, 489000, 495000, 501000, 507000, 513000, 519000, 525000, 
    531000, 537000, 543000, 549000, 555000, 561000, 567000, 573000, 579000, 585000, 
    591000, 597000, 603000, 609000, 615000, 621000, 627000, 633000, 639000, 645000, 
    651000, 657000, 663000, 669000, 675000, 681000, 687000, 693000, 699000, 705000, 
    711000, 717000, 723000, 729000, 735000, 741000, 747000, 753000, 759000, 765000, 
    771000, 777000, 783000, 789000, 795000, 801000, 807000, 813000, 819000, 825000, 
    831000, 837000, 843000, 849000, 855000, 861000, 867000, 873000, 879000, 885000, 
    891000, 897000, 903000, 909000, 915000, 921000, 927000, 933000, 939000, 945000, 
    951000, 957000, 963000, 969000, 975000, 981000, 987000, 993000, 999000, 1005000
};

static UINT32 _au4DtmbChannelInKhz[ISDBT_CH_NS] =
    {
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 
        0,      474000, 482000, 490000, 498000, 506000, 514000, 522000, 530000, 538000, 
        546000, 554000, 562000, 570000, 578000, 586000, 594000, 602000, 610000, 618000, 
        626000, 634000, 642000, 650000, 658000, 666000, 674000, 682000, 690000, 698000, 
        706000, 714000, 722000, 730000, 738000, 746000, 754000, 762000, 770000, 778000, 
        786000, 794000, 802000, 810000, 818000, 826000, 834000, 842000, 850000
    };


static UINT32 *_pu4DemodChannel[DEMOD_NS] =
{
    _au4AtscChannelInKhz, 
    _au4CableChannelInKhz, 
    _au4DvbtChannelInKhz, 
    _au4IsdbtChannelInKhz,
    _au4NTSCRFCh, 
    _au4NTSCCABLECh,
    _au4DvbsChannelInKhz,
    _au4NTSCDVBSCh,
    _au4DtmbChannelInKhz
};

static TEXT2ENUM_T _arNimDemodEnumTbl [] =
{
    { "atsc", DEMOD_ATSC },
    { "cableauto", DEMOD_CABLE },
    { "cable256", DEMOD_CABLE },
    { "cable64", DEMOD_CABLE },
    { "cable", DEMOD_CABLE },
    { "dvbt", DEMOD_DVBT },
    { "isdbt", DEMOD_ISDBT },
    { "t_ana", DEMOD_T_ANA },
    { "c_ana", DEMOD_C_ANA },
    { "dvbc4", DEMOD_CABLE },
    { "dvbc16", DEMOD_CABLE },
    { "dvbc32", DEMOD_CABLE },
    { "dvbc64", DEMOD_CABLE },
    { "dvbc128", DEMOD_CABLE },
    { "dvbc256", DEMOD_CABLE },
    { "dvbs", DEMOD_DVBS },
    { "dtmb", DEMOD_DTMB },
    { NULL, 0xFFFFFFFF }
};

static TEXT2ENUM_T _arNimDemodConnTypeEnumTbl [] =
{
    { "atsc", TUNER_CONN_TYPE_TER_DIG },
    { "cableauto", TUNER_CONN_TYPE_CAB_DIG },
    { "cable256", TUNER_CONN_TYPE_CAB_DIG },
    { "cable64", TUNER_CONN_TYPE_CAB_DIG },
    { "cable", TUNER_CONN_TYPE_CAB_DIG },
    { "dvbt", TUNER_CONN_TYPE_TER_DIG },
    { "isdbt", TUNER_CONN_TYPE_TER_DIG },
    { "t_ana", TUNER_CONN_TYPE_TER_ANA },
    { "c_ana", TUNER_CONN_TYPE_CAB_ANA },
    { "dvbc4", TUNER_CONN_TYPE_CAB_DIG },
    { "dvbc16", TUNER_CONN_TYPE_CAB_DIG },
    { "dvbc32", TUNER_CONN_TYPE_CAB_DIG },
    { "dvbc64", TUNER_CONN_TYPE_CAB_DIG },
    { "dvbc128", TUNER_CONN_TYPE_CAB_DIG },
    { "dvbc256", TUNER_CONN_TYPE_CAB_DIG },
    { "dvbs", TUNER_CONN_TYPE_SAT_DIG },
    { "dtmb", TUNER_CONN_TYPE_TER_DIG },
    { NULL, 0xFFFFFFFF }
};

static TEXT2ENUM_T _arNimDemodModEnumTbl [] =
{
    { "atsc", MOD_VSB_8 },
    { "cableauto", MOD_UNKNOWN },
    { "cable256", MOD_QAM_256 },
    { "cable64", MOD_QAM_64 },
    { "cable", MOD_QAM_64 },
    { "dvbt", MOD_QAM_64 },
    { "isdbt", MOD_QAM_64 },
    { "t_ana", MOD_QAM_64 },
    { "c_ana", MOD_QAM_64 },
    { "dvbc4", MOD_QAM_4_NR },
    { "dvbc16", MOD_QAM_16 },
    { "dvbc32", MOD_QAM_32 },
    { "dvbc64", MOD_QAM_64 },
    { "dvbc128", MOD_QAM_128 },
    { "dvbc256", MOD_QAM_256 },
    { "dvbs", MOD_QAM_64 },
    { "dtmb", MOD_QAM_64 },
    { NULL, 0xFFFFFFFF }
};

static TEXT2ENUM_T _arNimTunerIDEnumTbl [] =
{
    { "atsc", 0x01 },
    { "cableauto", 0x01 },
    { "cable256", 0x01 },
    { "cable64", 0x01 },
    { "cable", 0x01 },
    { "dvbt",  0x01 },
    { "isdbt", 0x01 },
    { "t_ana", 0x0 },
    { "c_ana", 0x0 },
    { "dvbc4", 0x01 },
    { "dvbc16", 0x01 },
    { "dvbc32", 0x01 },
    { "dvbc64", 0x01 },
    { "dvbc128", 0x01 },
    { "dvbc256", 0x01 },
    { "dvbs",  0x01 },
    { "dtmb",  0x01 },
    { NULL, 0xFFFFFFFF }
};

static TEXT2ENUM_T _arNimDemodArgEnumTbl [] =
{
    { "atsc", 8 },
    { "cableauto", 999 },
    { "cable256", 256 },
    { "cable64", 64 },
    { "cable", 64 },
    { "dvbt", 64 },
    { "isdbt", 64 },
    { "t_ana", 64 },
    { "c_ana", 64 },
    { "dvbc4", 4 },
    { "dvbc16", 16 },
    { "dvbc32", 32 },
    { "dvbc64", 64 },
    { "dvbc128", 128 },
    { "dvbc256", 256 },
    { "dvbs", 64 },
    { "dtmb", 64 },
    { NULL, 0xFFFFFFFF }
};

static UINT32 _u4Demod = DEMOD_ATSC;
static UINT32 _u4Channel = ATSC_CH_DEFAULT;
static UINT32 _u4DemodArg = 8; // 8 for ATSC (8VSB)
static UINT32 _u4NimInit = 0;

CLI_EXEC_T _arNimCmdTbl [] =
{
    { "id"              , NULL      , _NimCmdSetID          , NULL, "Set Tuner ID", CLI_GUEST },
    { "ver"             , NULL      , _NimCmdVer            , NULL, "Tuner version", CLI_GUEST },
    { "hc"              , NULL      , _NimCmdHostCmd        , NULL, "Tuner Host Command", CLI_ADMIN },
    { "dl"              , NULL      , _NimCmdDbgLvl         , NULL, "Set Debug Level", CLI_GUEST },
    { "go"              , NULL      , _NimCmdGo             , NULL, "Start Nim", CLI_GUEST },
    { "freq"            , NULL      , _NimCmdFreq           , NULL, "Freq Set", CLI_GUEST },
    { "setcable"        , "sc"      , _NimCmdSetCable       , NULL, "Nim set cable parameters", CLI_GUEST },
    { "getcablelevel"   , "gclv"    , _NimCmdGetCableLevel  , NULL, "Nim get cable signal level", CLI_GUEST },
    { "getcablelock"    , "gclk"    , _NimCmdGetCableSync   , NULL, "Nim get cable lock status", CLI_GUEST },
    { "detachmw"        , "dm"      , _NimCmdDetachMW       , NULL, "Nim Detach MW", CLI_GUEST },
    { "dtd"             , NULL      , _NimCmdDtdCmd         , NULL, "Nim dtd", CLI_GUEST },    
    { "atd"             , NULL      , _NimCmdAtdCmd         , NULL, "Nim atd", CLI_GUEST },
    { "gettsfmt"        , NULL      , _NimCmdGetTsFmt       , NULL, "Get Ts format", CLI_GUEST },
    { "settssp"         , NULL      , _NimCmdSetTsSP        , NULL, "Set Ts serial/prallel", CLI_GUEST },
    { "settstristate"   , NULL      , _NimCmdSetTsTristate  , NULL, "Set Ts tristate", CLI_GUEST },
    { "settuner"        , "st"      , _NimCmdSetTuner       , NULL, "Set Tuner by Control-Word", CLI_SUPERVISOR },
    { "gettuner"        , "gt"      , _NimCmdGetTuner       , NULL, "Get Tuner register value", CLI_SUPERVISOR },
    { "gettunerEx"      , "gtEx"    , _NimCmdGetTunerEx     , NULL, "Get Tuner Reg-Val with register address", CLI_SUPERVISOR },
    { "scan"            , "s"       , _NimCmdScanTuner      , NULL, "Scan all Tuner device", CLI_GUEST },
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
    { "up"              , NULL      , _NimCmdUp             , NULL, "Channel Up", CLI_GUEST },
    { "down"            , NULL      , _NimCmdDown           , NULL, "Channel Down", CLI_GUEST },
    { "channel"         , "ch"      , _NimCmdChannel        , NULL, "Channel Set", CLI_GUEST },
    { "init"            , "i"       , _NimCmdInit           , NULL, "Nim init", CLI_GUEST },
    { "open"            , "o"       , _NimCmdOpen           , NULL, "Nim open", CLI_GUEST },
    { "close"           , "c"       , _NimCmdClose          , NULL, "Nim close", CLI_GUEST },
    { "setBW"           , "sbw"     , _NimCmdSetBW          , NULL, "Nim set BW", CLI_GUEST },
    { "getcablesignal"  , "gcsc"    , _NimCmdGetCableSignal , NULL, "Nim get cable signal parameter", CLI_GUEST },
    { "detachi2c"       , "dei2c"   , _NimCmdDetachI2C      , NULL, "Detach Tuner I2C", CLI_GUEST },
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
#if defined(__linux__)
    {"setconntype"      , "sctype"  , _NimCmdSetConnType    , NULL, "Set Conn Type", CLI_SUPERVISOR },
    {"setmod"           , "sm"      , _NimCmdSetMod         , NULL, "Set mod", CLI_SUPERVISOR },
    {"setfinetune"      , "sft"     , _NimCmdSetFineTune    , NULL, "Set fine tune", CLI_SUPERVISOR },
#endif
// LOG macro support
#ifdef CC_MT5381
    CLIMOD_DEBUG_CLIENTRY(NIM),
#endif
    { NULL, NULL, NULL, NULL, NULL, CLI_GUEST }
};

CLI_MAIN_COMMAND_ITEM(Nim)
{
    "nim", NULL, NULL, _arNimCmdTbl, "Nim command", CLI_GUEST
};

/******************************************************************************
* Variable      : cli "jpg" enums
******************************************************************************/

/******************************************************************************
* Flag(s)       :
******************************************************************************/

/******************************************************************************
* Implementation        : helper functions for commands parsing
******************************************************************************/

#define D_NIM_DONT_BREAK 0
#define D_NIM_BREAK          1

static UINT32 _u4NimTimeoutMs = 0;
static HAL_TIME_T _rHalTimeStart;

INT32 _nim_SetTimeout(UINT32 u4TimeoutMs)
{
    HAL_RAW_TIME_T rRawTime;

    HAL_GetRawTime(&rRawTime);
    HAL_RawToTime(&rRawTime, &_rHalTimeStart);
    _u4NimTimeoutMs = u4TimeoutMs;

    return E_CLINIM_OK;
}

INT32 _nim_Breaker(void)
{
#if defined(__linux__)
    return FALSE;
#else
    HAL_RAW_TIME_T rRawTime;

    HAL_TIME_T rHalTime, rHalTimeDuration;
    UINT32 u4MilliSec;

    if (_u4NimTimeoutMs == 0)
    {
        // run endlessly
        return D_NIM_DONT_BREAK;
    }

    // get raw time
    HAL_GetRawTime(&rRawTime);
    HAL_RawToTime(&rRawTime, &rHalTime);
    // get time difference
    HAL_GetDeltaTime(&rHalTimeDuration, &_rHalTimeStart, &rHalTime);

    u4MilliSec = (rHalTimeDuration.u4Seconds *1000) + (rHalTimeDuration.u4Micros /1000);

    if (u4MilliSec >= _u4NimTimeoutMs)
    {
        Printf("Timeout ... break\n");
        return D_NIM_BREAK;
    }

    return D_NIM_DONT_BREAK;
#endif
}

static INT32 _GetEnum(const CHAR *szText, const TEXT2ENUM_T *prText2Enum)
{
    if ((NULL == szText) || (NULL == prText2Enum))
    {
        return -(INT32)E_CLINIM_ERR_PARAM;
    }

    while (prText2Enum->szText)
    {
        if (x_strncmp(prText2Enum->szText, szText, x_strlen(prText2Enum->szText)) == 0)
        {
            break;
        }
        else
        {
            prText2Enum++;
        }
    }

    return prText2Enum->i4Value;
}

/******************************************************************************
* Implementation        : cli "nim" commands
******************************************************************************/
static INT32 _NimCmdInit(INT32 i4Argc, const CHAR **szArgv)
{
    NIM_MonitorStart();

    return E_CLINIM_OK;
}

static INT32 _NimCmdOpen(INT32 i4Argc, const CHAR **szArgv)
{
    return NIM_Open();
}

static INT32 _NimCmdClose(INT32 i4Argc, const CHAR **szArgv) \
{
    return NIM_Close();
}

static INT32 _NimCmdSetCable(INT32 i4Argc, const CHAR **szArgv)
{
    // nim.setcable {freq_mhz} {k_symbol_sec} {qam_size}
    UINT32 u4FreqK, u4SymbolK, uAcqType=ACQ_ACTION_DEFAULT, uLoopCounts=1, u4DelayMs=0;

    UINT32 i4QamSize;

    if (i4Argc < 4)
    {
        Printf("usage :\n\tnim.setcable {freq_khz} {k_symbol_sec} {qam_size} \n");
        return -E_CLINIM_ERR_PARAM;
    }

    u4FreqK = (UINT32)StrToInt(szArgv[1]);
    u4SymbolK = (UINT32)StrToInt(szArgv[2]);
    i4QamSize = (UINT32)StrToInt(szArgv[3]);

    if(i4Argc >= 5){
        uAcqType = (INT32)StrToInt(szArgv[4]);
    }
    if(i4Argc >= 6){
        uLoopCounts = (INT32)StrToInt(szArgv[5]);
    }
    if(i4Argc >= 7){
        u4DelayMs = (INT32)StrToInt(szArgv[6]);
    }
    if (i4QamSize != 4 && i4QamSize != 8 && i4QamSize != 16 && i4QamSize != 32 && i4QamSize != 64 && i4QamSize != 128
            && i4QamSize != 256 && i4QamSize != 999)
    {
        Printf("QAM size should be one of 8(VSB), 16, 32, 64, 128 , 256 or 999(autoQAM)\n");
        return -E_CLINIM_ERR_PARAM;
    }

     Printf("Try to lock ... %uKHz %u k-sym/sec %dQAM (10 seconds)\n", u4FreqK, u4SymbolK, i4QamSize);
    printf("uAcqType=%d uLoopCounts=%d u4DelayMs=%d\n",uAcqType, uLoopCounts, u4DelayMs);

#if !defined(__linux__)
    NIM_SetCable(u4FreqK, u4SymbolK, i4QamSize, TRUE, _nim_Breaker, (TUNER_ACQ_ACTION_T) uAcqType, uLoopCounts, u4DelayMs);
    return E_CLINIM_OK;
#else
    return NIM_SetCable(u4FreqK, u4SymbolK, i4QamSize, TRUE, _nim_Breaker, (TUNER_ACQ_ACTION_T) uAcqType, uLoopCounts, u4DelayMs);
#endif
}

static INT32 _NimCmdSetBW(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 2)
    {
        Printf("BW_6_MHz = %d, BW_7_MHz = %d, BW_8_MHz = %d\n", BW_6_MHz, BW_7_MHz, BW_8_MHz);
        return E_CLINIM_ERR_PARAM;
    }

    NIM_SetBW(StrToInt(szArgv[1]));

    return E_CLINIM_OK;
}

static INT32 _NimCmdDetachMW(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 2)
    {
        return E_CLINIM_ERR_PARAM;
    }

    NIM_DetachMW(StrToInt(szArgv[1]));

    return E_CLINIM_OK;
}

static INT32 _NimCmdGetCableLevel(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1SignalLevel;

    UINT16 u2SignalSNR;
    UINT16 u2SignalPER;

    u1SignalLevel = NIM_GetCableSignalLevel();
    Printf("cable signal level : %d\n", (INT32)u1SignalLevel);

    u2SignalSNR = NIM_GetCableSignalSNR();
    Printf("cable SNR : %d\n", u2SignalSNR);


    u2SignalPER = NIM_GetCableSignalPER();
    Printf("cable PER : %d\n", u2SignalPER);

#if defined(__linux__)
    return (INT32)u1SignalLevel;
#else
    return E_CLINIM_OK;
#endif
}

static INT32 _NimCmdGetCableSignal(INT32 i4Argc, const CHAR **szArgv)
{
    SIGNAL rSignal;

    NIM_GetCableSignal(&rSignal);

    Printf("freq        : %8u\n", (UINT32)rSignal.Frequency);
    Printf("symbol rate : %8u\n", (UINT32)rSignal.SymbolRate);
    Printf("qam size    : %8dQAM\n", (INT32)rSignal.QAMSize);

    return E_CLINIM_OK;
}

static INT32 _NimCmdGetCableSync(INT32 i4Argc, const CHAR **szArgv)
{
    INT16 i2Sync;

    i2Sync = NIM_GetCableSync();

    Printf("Sync status : %d\n", (INT32)i2Sync);
#if defined(__linux__)
    return (INT32)i2Sync;
#else
    return E_CLINIM_OK;
#endif
}

#if !defined(__linux__)
static void _NimConfig(void)
#else
static BOOL  _NimConfig(void)
#endif
{
    UINT32 u4Freq;

    u4Freq = _pu4DemodChannel[_u4Demod][_u4Channel];
    Printf("Demod(%s) Channel(%d) Freq(%d KHz)\n", _azDemodName[_u4Demod], _u4Channel, u4Freq);

    if ((u4Freq > MAX_FREQ) || (u4Freq < MIN_FREQ))
    {   
        //lei_code_dB110525 for build warning
        //Printf("Freq Invalid!\n");
        //return;
        goto lbUsage;
    }

#if !defined(__linux__)
    CLI_Parser_Arg("nim.sc %d 5057 %d", u4Freq, _u4DemodArg);
    Printf("\n");
    // Only For Digital Tuner
    if(NIM_GetTunerID() == 1)
    {
        _NimCmdGetCableLevel(0, NULL);
        _NimCmdGetCableSignal(0, NULL);
    }
#else
    return CLI_Parser_Arg("nim.sc %d 5057 %d", u4Freq, _u4DemodArg);
#endif

lbUsage:  //lei_code_dB110525 for build warning
#if !defined(__linux__)
	Printf("Freq Invalid!\n");
#else
	Printf("Freq Invalid!\n");
	return 1;
#endif

}

static INT32 _NimCmdSetID(INT32 i4Argc, const CHAR **szArgv)
{
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: TunerID.\n");

        Printf("Current ID = %d\n", NIM_GetTunerID());
        return 0;
    }

    NIM_SetTunerID((UINT16)StrToInt(szArgv[1]));

    return E_CLINIM_OK;
}

static INT32 _NimCmdDbgLvl(INT32 i4Argc, const CHAR **szArgv)
{
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: Debug_Level.\n");
    }
    else
    {
        NIM_SetDbgLvl((UCHAR) StrToInt(szArgv[1]));
    }

    Printf("Current Debug_Level = 0x%02X\n", NIM_GetDbgLvl());

    return E_CLINIM_OK;
}

static INT32 _NimCmdVer(INT32 i4Argc, const CHAR **szArgv)
{
    NIM_CmdVer();

    return E_CLINIM_OK;
}

static INT32 _NimCmdHostCmd(INT32 i4Argc, const CHAR **szArgv)
{
    NIM_CmdHostCmd(i4Argc, szArgv);

    return E_CLINIM_OK;
}

static INT32 _NimCmdDtdCmd(INT32 i4Argc, const CHAR **szArgv)
{
    NIM_CmdDtdCmd(i4Argc, szArgv);

    return E_CLINIM_OK;
}

static INT32 _NimCmdAtdCmd(INT32 i4Argc, const CHAR **szArgv)
{
    NIM_CmdAtdCmd(i4Argc, szArgv);

    return E_CLINIM_OK;
}

#if defined(__linux__)
static INT32 _NimCmdSetConnType(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4e_conn_type;
    u4e_conn_type = (UINT32)_GetEnum(szArgv[1], _arNimDemodConnTypeEnumTbl);
    NIM_SetConnType(u4e_conn_type);
    return 0;
}

static INT32 _NimCmdSetMod(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mod;
    u4Mod = (UINT32)_GetEnum(szArgv[1], _arNimDemodModEnumTbl);
    NIM_SetMod((TUNER_MODULATION_T)u4Mod);
    _u4DemodArg = (UINT32)_GetEnum(szArgv[1], _arNimDemodArgEnumTbl);
	return 0;//lei_code_dB110525 for build warning
}

static INT32 _NimCmdSetFineTune(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 2)
    {
        return E_CLINIM_ERR_PARAM;
    }
    //lei_code_dB110525 for build warning
    return NIM_SetFineTune(StrToInt(szArgv[1]));
    //return E_CLINIM_OK;
}

#endif
static INT32 _NimCmdGo(INT32 i4Argc, const CHAR **szArgv)
{
UINT32 u4Demod;

UINT32 u4e_conn_type;
UINT32 u4Mod;
UINT16 u4TunerId;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("Arg: atsc/cable/cable256/cableauto/dvbt/dvbs/isdbt/t_ana/c_ana \r\n /dvbc4/dvbc16/dvbc32/dvbc64/dvbc128/dvbc256\n");
        return 0;
    }

    u4Demod = (UINT32)_GetEnum(szArgv[1], _arNimDemodEnumTbl);
    if((u4Demod == DEMOD_ATSC) || (u4Demod == DEMOD_CABLE))
    {
        DynamicSwitch_DTD_DVBT_Mode = 0; //for SLT
    }
    
    if(u4Demod == DEMOD_DVBT)
    {
        DynamicSwitch_DTD_DVBT_Mode = 1; //for SLT
    }
    
    u4e_conn_type = (UINT32)_GetEnum(szArgv[1], _arNimDemodConnTypeEnumTbl);
    NIM_SetConnType(u4e_conn_type);
    u4Mod = (UINT32)_GetEnum(szArgv[1], _arNimDemodModEnumTbl);
    NIM_SetMod((TUNER_MODULATION_T)u4Mod);
    u4TunerId = (UINT16)_GetEnum(szArgv[1],_arNimTunerIDEnumTbl);
    NIM_SetTunerID(u4TunerId);

    if (u4Demod >= DEMOD_NS)
    {
        Printf("Arg: atsc/cable/cable256/cableauto/dvbt/isdbt/t_ana/c_ana \r\n /dvbc4/dvbc16/dvbc32/dvbc64/dvbc128/dvbc256\n");
        return -1;
    }
    
    if (u4Demod < DEMOD_NS)
    { 
        _u4Demod = u4Demod;
        _u4DemodArg = (UINT32)_GetEnum(szArgv[1], _arNimDemodArgEnumTbl);
        if (u4Demod > 0 || u4Demod == 0)
            _u4Channel = _au4ChannelDefault[u4Demod];
    }

    if (_u4NimInit == 0)
    {
        CLI_Parser("sif.i");

        NIM_MonitorStart();
        NIM_Open();
    }

    _NimConfig();

    return E_CLINIM_OK;
}

static INT32 _NimCmdFreq(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4FreqK, uAcqType,uLoopCounts, u4DelayMs=0;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("nim.freq  freq_in_khz \n");
        return 0;
    }

    u4FreqK = (UINT32)StrToInt(szArgv[1]);
    Printf("Demod(%s) Freq(%d KHz)\n", _azDemodName[_u4Demod], u4FreqK);

    if(i4Argc >4 ){
        uAcqType = (INT32)StrToInt(szArgv[2]);
        uLoopCounts = (INT32)StrToInt(szArgv[3]);
        u4DelayMs = (INT32)StrToInt(szArgv[4]);
    }
    else{
        uAcqType = 2; //ACQ_ACTION_DEFAULT
        uLoopCounts = 1;
    }

    printf("_NimCmdFreq i4Argc=%d, uAcqType=%d, uLoopCounts=%d u4DelayMs=%d\n",i4Argc,uAcqType, uLoopCounts, u4DelayMs);
#if !defined(__linux__)
    CLI_Parser_Arg("nim.sc %d 5057 %d %d %d %d", u4FreqK, _u4DemodArg, uAcqType, uLoopCounts, u4DelayMs);
    Printf("\n");
    // Only For Digital Tuner
    if(NIM_GetTunerID() == 1)
    {    
        _NimCmdGetCableLevel(0, NULL);
        _NimCmdGetCableSignal(0, NULL);
    }

    return E_CLINIM_OK;
#else
    return CLI_Parser_Arg("nim.sc %d 5057 %d", u4FreqK, _u4DemodArg);
#endif
}

static INT32 _NimCmdChannel(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Channel;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        UINT32 u4Idx;

        UINT32 u4Min;
        UINT32 u4Max;

        Printf("Demod(%s) Current Channel is %d\n", _azDemodName[_u4Demod], _u4Channel);

        Printf("Channel List:\n");

        for (u4Channel = 0; u4Channel < _au4ChannelNs[_u4Demod]; )
        {
            u4Min = u4Channel;

            u4Max = (u4Channel + 10);

            if (u4Max > _au4ChannelNs[_u4Demod])
            {
                u4Max = _au4ChannelNs[_u4Demod];
            }

            Printf("Channel %3d~%3d: ", u4Min, u4Max - 1);

            for (u4Idx = u4Min; u4Idx < u4Max; u4Idx++)
            {
                Printf("%6d, ", _pu4DemodChannel[_u4Demod][u4Idx]);
            }

            Printf("\n");

            u4Channel = u4Max;
        }
        return 0;
    }

    u4Channel = (UINT32)StrToInt(szArgv[1]);

    if (u4Channel >= _au4ChannelNs[_u4Demod])
    {
        Printf("Demod(%s) Max Channel is %d\n", _azDemodName[_u4Demod],
               _au4ChannelNs[_u4Demod] - 1);
        return 0;
    }

    _u4Channel = u4Channel;

#if !defined(__linux__)
    _NimConfig();

    return E_CLINIM_OK;
#else
    return _NimConfig();
#endif
}

static INT32 _NimCmdUp(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Channel;

    u4Channel = _u4Channel + 1;

    if (u4Channel >= _au4ChannelNs[_u4Demod])
    {
        Printf("Demod(%s) Max Channel is %d\n", _azDemodName[_u4Demod],
               _au4ChannelNs[_u4Demod] - 1);
        return 0;
    }

    _u4Channel = u4Channel;

    _NimConfig();

    return E_CLINIM_OK;
}

static INT32 _NimCmdDown(INT32 i4Argc, const CHAR **szArgv)
{
    if (_u4Channel == 0)
    {
        Printf("Demod(%s) Min Channel is 0\n", _azDemodName[_u4Demod]);
        return 0;
    }

    _u4Channel--;

    _NimConfig();

    return E_CLINIM_OK;
}

static INT32 _NimCmdDetachI2C(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 2)
    {
        return E_CLINIM_ERR_PARAM;
    }

    NIM_DetachI2C(StrToInt(szArgv[1]));

    return E_CLINIM_OK;
}

static INT32 _NimCmdGetTsFmt(INT32 i4Argc, const CHAR **szArgv)
{
    UCHAR ucSerorPal, ucClockPol;
    UINT32 u4TsClock;
    BOOL fgGatedClock, fgExternalDemod;

    Printf("0/1/2(SP_UNKNOWN/SP_SERIAL/SP_PARALLEL) 0/1/2(POL_UNKNOWN_ACTIVE/POL_HIGH_ACTIVE/POL_LOW_ACTIVE) \n");
    
    NIM_GetTsFmt(&ucSerorPal, &ucClockPol, &u4TsClock, &fgGatedClock, &fgExternalDemod);
    
    Printf("Current TS format: = (SIG_SP_T)%d (SIG_POL_T)%d (Ts_Clock(KHz))%d (Gated_Clock_Flag)%d (External_Demod_Flag)%d\n", ucSerorPal, ucClockPol, u4TsClock, fgGatedClock, fgExternalDemod);
    return E_CLINIM_OK;
}

static INT32 _NimCmdSetTsSP(INT32 i4Argc, const CHAR **szArgv)
{
    UCHAR ucSerorPal;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("nim.settssp  0/1/2(SP_UNKNOWN/SP_SERIAL/SP_PARALLEL) \n");
        return 0;
    }

    ucSerorPal = (UINT32)StrToInt(szArgv[1]);
    NIM_SetTsSP(ucSerorPal);
    return E_CLINIM_OK;
}

static INT32 _NimCmdSetTsTristate(INT32 i4Argc, const CHAR **szArgv)
{
    UCHAR ucTristate;

    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("nim.settstristate  0/1/2(BOOL_UNKNOWN/BOOL_FALSE/BOOL_TRUE) \n");
        return 0;
    }

    ucTristate = (UINT32)StrToInt(szArgv[1]);
    NIM_SetTsTristate(ucTristate);
    return E_CLINIM_OK;
}

static INT32 _NimCmdSetTuner(INT32 i4Argc, const CHAR **szArgv)
{
    UCHAR  ucAddr, ucLen, ii;
    UCHAR  aucBuf[8];

    if ((i4Argc < 3) || (szArgv == NULL) || (szArgv[1] == NULL))
    {
        Printf("nim.st i2cAddr CB1 CB2 CB3 ... \n");
        return 0;
    }

    ucAddr = (UCHAR)StrToInt(szArgv[1]);
    ucLen  = (UCHAR)(i4Argc-2);

    if(ucLen > 8)
    {
        Printf("%d exceed max(8) command length!\n", ucLen);
        return 0;
    }

    Printf("settuner Contol-Word: ");
    for(ii=0; ii<ucLen; ii++)
    {
        aucBuf[ii] = (UCHAR)StrToInt(szArgv[ii+2]);
        Printf("0x%02X ", aucBuf[ii]);
    }
    Printf("\n");

    NIM_SetTuner(ucAddr, aucBuf, ucLen);

    return E_CLINIM_OK;
}

static INT32 _NimCmdGetTuner(INT32 i4Argc, const CHAR **szArgv)
{
    UCHAR  ucAddr, ucLen, ii;
    UCHAR  aucBuf[8];

    if (i4Argc != 3)
    {
        Printf("nim.gt i2cAddr Len\n");
        return 0;
    }

    ucAddr = (UCHAR)StrToInt(szArgv[1]);
    ucLen  = (UCHAR)StrToInt(szArgv[2]);

    if(ucLen > 8)
    {
        Printf("%d exceed max(8) command length!\n", ucLen);
        return 0;
    }

    NIM_GetTuner(ucAddr, aucBuf, ucLen);

    Printf("Tuner value: ");
    for(ii=0; ii<ucLen; ii++)
    {
        Printf("0x%02X ", aucBuf[ii]);
    }
    Printf("\n");

    return E_CLINIM_OK;
}

static INT32 _NimCmdGetTunerEx(INT32 i4Argc, const CHAR **szArgv)
{
    UCHAR  ucAddr, ucReg, ucLen, ii;
    UCHAR  aucBuf[8];

    if (i4Argc != 4)
    {
        Printf("nim.gtEx i2cAddr regAddr Len\n");
        return 0;
    }

    ucAddr = (UCHAR)StrToInt(szArgv[1]);
    ucReg  = (UCHAR)StrToInt(szArgv[2]);
    ucLen  = (UCHAR)StrToInt(szArgv[3]);

    if(ucLen > 8)
    {
        Printf("%d exceed max(8) command length!\n", ucLen);
        return 0;
    }

    NIM_GetTunerEx(ucAddr, ucReg, aucBuf, ucLen);

    Printf("Tuner register 0x%2X value: ", ucReg);
    for(ii=0; ii<ucLen; ii++)
    {
        Printf("0x%02X ", aucBuf[ii]);
    }
    Printf("\n");

    return E_CLINIM_OK;
}

static INT32 _NimCmdScanTuner(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2Addr;
    UCHAR  ucValue;

    if (i4Argc != 1)
    {
        Printf("nim.scan\n");
        return 0;
    }

    for(u2Addr=0; u2Addr<=0xFF; u2Addr+=2)
    {
        ucValue=0xFF;
        NIM_GetTuner((UCHAR)u2Addr, &ucValue, 1);

        if(ucValue != 0xFF)
            Printf("\rFound Device at Addr 0x%02X\n", u2Addr);
    }
    Printf("    Done\n");

    return E_CLINIM_OK;
}

LINT_RESTORE

