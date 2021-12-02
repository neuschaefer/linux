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
 * $RCSfile: drv_scaler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.c
 *  Brief of file drv_scaler.c.
 *  Details of file drv_scaler.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "drv_scaler.h"
#include "scpos_debug.h"
#ifndef CC_COPLAT_MT82
#include "osd_drvif.h"
#include "drvcust_if.h"
#include "tve_if.h"
#endif
#include "fbm_drvif.h"
#include "srm_drvif.h"
#include "hw_ckgen.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "general.h"        // nptv
#include "hw_scpos.h"
#include "hw_ycproc.h"
#include "hw_ckgen.h"
#include "sv_const.h"       // nptv
#include "video_def.h"      // nptv
#include "drv_display.h"    // nptv
#include "drv_lvds.h"    // nptv
#include "vdo_misc.h"       // nptv
#include "drv_di.h"         // nptv
#include "vdp_display.h"
#include "source_table.h"
#include "source_select.h"
#include "drv_video.h"      // nptv
#include "vdp_drvif.h"
#include "nptv_drvif.h"
#include "api_notify.h"
#include "drv_vga.h"
#include "x_assert.h"
#include "drv_scpos.h"
#include "drv_upscaler.h"
#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#include "ir_if.h"
#endif
#include "c_model.h"
#include "x_util.h"
#include "panel.h"
#include "drv_scaler_table.h"
#include "drv_nr.h"
#include "drv_vdoclk.h"
#include "x_bim.h"
#include "mute_if.h"
#include "x_mid.h"
#if SUPPORT_DOT_TO_DOT_PATTERN
#include "hw_tdc.h"
#endif
#ifdef CC_TVE_US_IMAGERESIZER_WA
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#endif

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations : all WA
//-----------------------------------------------------------------------------
#define CC_NONLINEAR_AUTO

// Horizontal Scale Up and 422 to 444 in output stage, ̥k䦳D, +MHL, +DATA_LENGTH
#define CC_SCPOS_WA3

// Delay unmute to avoid seeing video transient in frame buffer
#define CC_SCPOS_WA5

// For VGA two frame mode, there will be garbage when changing scaling factor
// [DTV00140187] The video flash garbage after enter mheg5 page.
#define CC_SCPOS_WA6

// Scaler HW truncation error
//#define CC_SCPOS_WA7
// for removing fraction part of DS and HDS scaling factor, and initial factor
// Case 1: HPS < 0x1000
// Case 2: DS < 0x2000 (both H and V)
#define CC_SCPOS_WA_REMOVE_FRACTION_PART 0

// Modify DS factors for import proection bug when frame number changed
//#define CC_SCPOS_WA10

//#define CC_TOGGLE_W_EN

// 'MVL = MVL + 2'  to avoid up scalar fifo to access unknow DRAM
// PSCAN display mode + down, VL+= line buffer num.
#define CC_SCPOS_WA11 2

// Fix pre-scaling down factor to specific value to avoid truncation error noise in HDMI 1080i/1080p
//#define CC_SCPOS_WA12  0x6000

/* for 1080i pscan dispmode workaround.
   in down scaling pscan dispmode
   1. invert vsync
   2. enter pscan dispmode
   3. wait for N Vsync
   4. invert vsync back to original
 */
#define CC_SCPOS_WA13_PSCAN_DISPMODE_1080I 1
#define CC_SCPOS_WA13_COUNTER_TO_INVERT_VSYNC 3

// Mute will relatch to VSync, and all register settings which will cause video transient have to be delayed.
//#define CC_SCPOS_WA14
// In 5387, OS_1A bit[17] can control if mute will relatch to VSync or not. 0: relatch, 1: bypass
#ifdef CC_SCPOS_WA14
UINT8 bStatusUpdate[2] = {0, 0};
EXTERN UINT32 _u4MainMuteMask;
#if SUPPORT_POP
EXTERN UINT32 _u4PipMuteMask;
#endif
#endif

// New algorithm to auto fine tune non-linear parameters.
#define CC_SCPOS_WA15
#ifdef CC_SCPOS_WA15
UINT32 u4RefWidth;
UINT32 u4RefHeight;
#endif

// Do not use HPS to avoid video transient and mute under VGA progress mode
//#define CC_SCPOS_WA16 

// There is garbage in middle of screen under DRAM write twice mode.
//#define CC_SCPOS_WA17   /// only enable in 5387, 5363 can remove this

// Warkaround for US L1L2 overwrite issue when HTtoal > 2048.
//#define CC_SCPOS_WA18

// Warkaround for US L1L2 fifo can not access the pxiels after 1898-th pixel
//#define CC_SCPOS_WA19 

// New import protection will only take effect when write-related registers updated
// We can use PIP1_09_RLH_OFF_1 in the future instead of toggle V Porch (8223)
// Currently, PIP1_09_RLH_OFF_1 is not protected with Vsync.
#define CC_SCPOS_WA20

// Losing leftest line under PSCAN Dispmode + US
// Under dot-by-dot mode, use PSCAN Dispmode + DS instead
//#define CC_SCPOS_WA21

// Video gitter under PSCAN Dispmode  or Scaler Dispmode+ WXGA panel
// Keep open for 5387, 5363
//#define CC_SCPOS_WA22

//SD timing after H pre-scaling dwon�ARead dram data will be wrong
//Main : Floor(Datalen/2) +1 == Datalen -48, Sub : Floor(Datalen/2) +1 == Datalen -24
//Main DataLenth ==98, 99, Sub Datalen==49,50, Need work around
//#define CC_SCPOS_WA23

// (1)  ECO fixed :displaymode line buffer wrong at right most 1~2 pixels, need extra 8 pixel data from decoder
// (2)  ECo fixed: 444 to 422 wrong color 
// (3)  Scaler display mode, Pack*N+1, the last pixel lost, need extra 2 pixels
//#define CC_SCPOS_WA24

// import will fail when output frame rate > input frame rate
//#define CC_SCPOS_WA25

// HW will wirte HL"+1" data, it will be fixed soon.
//When flip, the "+1" data will over-write correct data of the begining data of last line 
//WA : Data length need to be HL+1
//#define CC_SCPOS_WA26

// There is video transient when switching VDS_Bypass and VUS_Bypass. 
// Move VDS/VUS bypass setting to input ISR
//#define CC_SCPOS_WA27

//Mirror Issue: When input is 444 and width is 5N+1, Read_initp will fail
//#define CC_SCPOS_WA28
// Mirror issue: 5N+1 Set ReadInitP=4
// Need to adjust HSYNC_ADJ & RHL according to upscaling ratio


//Datlen < 48 + VUS, choose 444 path instead of 422.
#ifdef CC_SUPPORT_JPEG
#define CC_SCPOS_WA29
UINT8 IsImportSel = SV_TRUE;
#endif
#ifdef CC_SCPOS_WA29
static UINT8 bForce444_10b[2] = {0,0};
static UINT8 bPreColorDeth[2] = {0,0};
#endif

//input frame rate > output frame rate, set import_sel_2 to 0
//#define CC_SCPOS_WA30

#if PSCAN_DISPMODE_MIRROR_WA
#define CC_SCPOS_WA31
#endif
#ifdef CC_SCPOS_WA31
UINT8 bAddPScanMirWidth =0;
#endif

// Import fail, change Datalen and HL at the same time and do VDS without overscan 
// The last line will appear garbage when change datalen and HL, [DTV00141493]
// 8223 found this and already fix.
//#define CC_SCPOS_WA32

// postpone import to frame 1
#define CC_SCPOS_WA33
//only handle even pixels when mirror
//#define CC_SCPOS_WA34
#ifdef CC_SCPOS_WA34
UINT8 IsDramWidthChange = SV_FALSE;
#endif

#if SUPPORT_DOT_TO_DOT_PATTERN
UINT32 bOri10BitMode; 
UINT32 bOriUsH; 
UINT32 bOriUsV; 
UINT32 bCount=0;
//UINT16 _uDisableFunction=0;
UINT16 bOriDataLen;
UINT16  bOriHL;
UINT16  bOriVL;
UINT16 bOriRHP;
UINT16 bOriRHL;
UINT16 bOriRVP;
UINT16 bOriRVL;
UINT8 bOriEN3D; 
UINT8 bOriDSIs444Mode; 
UINT8 bOriVUSisBypass; 
UINT8 bOriFSEL_1;
UINT8 bOriVOFST_BYPASS;
UINT8 bOriALLBP;
UINT8 bOriYUV2RGB;
extern void vWriteDramB(UINT32 u4Addr, UINT8 bData);
#endif

// queue scaler event when freezing
#define CC_QUEUE_EVENT_IN_FREEZE

// do not enable dynamic scaling while nonlinear enable
//#define CC_SCPOS_NO_DS_NONLINEAR

//#define CC_SCPOS_MIN_PRESCALER              (0x2D80)     // 1920 >> (1366/2)    // this define was removed when porting 5360B F/W. Seems don't have this limitation in current H pre-down scaler

#define CC_SCPOS_FORCE_WOFF 1

#define CC_SCPOS_NEVER_FIX_HPS 1

#define CC_TWO_FRAME_LIMIT 1

#if 0
#define CC_FIFO_H_DELAY_UP 0
#define CC_FIFO_H_DELAY_DOWN 0
#endif

#define CC_SCPOS_WA_PMX_DEALY 0

#ifdef TVE_WA
#ifdef CC_MT5363
#define TVE_WA1 0
#define TVE_WA2 
#else
#define TVE_WA1 1
#endif
#else
#define TVE_WA1 0
#endif

#define CC_NEW_SCALAR_DISPMODE_AUTOTUNE 1
#define CC_SPEED_UP_DISPMODE_TUNE 1  

#define SCPOS_BYPASS_PSCANNR    0xffffffff

#define CC_SPEEDUP_IMPORT   1

#define CC_NEW_DRAM_PROTECTION

#ifdef CC_SCPOS_EN  // for FPGA verification

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define DFT_DS_FACTOR                       (0x8000)
#define DFT_US_FACTOR                       (0x8000)
#define DFT_HPD_FACTOR                      (0x8000)
#define DFT_DS_BIT_NUM                      (15)
#define DFT_US_BIT_NUM                      (15)
#define DFT_NONLINER_DS_BIT_NUM    (11)
#define DFT_NONLINER_US_BIT_NUM    (11)

#define CAL_HDIRECT_FACTOR               (0)
#define CAL_VDIRECT_FACTOR               (1)

#define UNIT_NONLINEAR                      (0xA)
#define MDDI_PP_CLOCK_TIME              (3)
#define SRM_SET_POOL_TIME                (3)
#define VDP_MDDI_RESCALE_TIME               (4)
#ifdef CC_SCPOS_WA33
#define VDP_SCPOS_RESCALE_TIME              (7+3)
#define VDP_SCPOS_IMPORT_PROTECT            (4+3)
#else
#define VDP_SCPOS_RESCALE_TIME              (7)
#define VDP_SCPOS_IMPORT_PROTECT            (4)
#endif // CC_SCPOS_WA33
#define VDP_SCPOS_WEN_WAIT                  (4)

#define VDP_SCPOS_DELAY_UNMUTE_TIME        (10)    //change from 5 to 15 by skywalker.yin at 2007/12/13 to eliminate CR[DTV00111148]
                                        //roll back to 5 for performance task force
#define VDP_DYNAMICE_SCALE_IDLE             (0)
#define VDP_DYNAMICE_SCALE_SCALING          (1)

#define DFT_SCPOS_MAIN_444_FIFO_LENGTH        1920
#define DFT_SCPOS_MAIN_422_FIFO_LENGTH        2100
#define DFT_SCPOS_SUB_444_FIFO_LENGTH        960
#define DFT_SCPOS_SUB_422_FIFO_LENGTH        960
static const UINT32 u4SCPOS_FIFO_LENGTH[2][2] = {{DFT_SCPOS_MAIN_422_FIFO_LENGTH, DFT_SCPOS_MAIN_444_FIFO_LENGTH},//main
                                           {DFT_SCPOS_SUB_422_FIFO_LENGTH, DFT_SCPOS_SUB_444_FIFO_LENGTH}};    //sub

#define MAX_FSYNC_TUNE        0x3ff

//US line delay setting
#define DOWN_SCALING_MODE 0
#define UP_SCALING_MODE 1

//skywalker.todo: should review this value
#define HPD_SIZE_LIMIT 1440

#define CC_MINIMUN_DRAM_RW_UNIT 7

#define BASE_WADDRESS_CHG ((UINT8)1 << 0)
#define BASE_RADDRESS_CHG ((UINT8)1 << 1)
#define FRAME_SIZE_CHG ((UINT8)1 << 2)
#define FRAME_NUM_CHG ((UINT8)1 << 3)
#define COLOR_DEPTH_CHG ((UINT8)1 << 4) 

#define PARAM_IGNORE    (0xffffffff)

#if CC_SPEEDUP_IMPORT
#define SCPIP_UPDATE_W  (0x1)
#define SCPIP_UPDATE_R  (0x2)
UINT32 VDP_SCPOS_SPEEDUP_IMPORT_PROTECT  =  (2);
#endif


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Brief of VDP_DYNAMIC_SCALER_PRM_T
 */
typedef struct
{
    UINT32 u4State;
    UINT32 u4VsyncCount;
    UINT32 u4LastFactorValid;

    UINT32 u4ScaleStep;
    UINT32 u4TotalScaleNs;

    INT32 i4SrcXOffsetStep;
    INT32 i4SrcYOffsetStep;
    INT32 i4SrcWidthStep;
    INT32 i4SrcHeightStep;

    INT32 i4OutXOffsetStep;
    INT32 i4OutYOffsetStep;
    INT32 i4OutWidthStep;
    INT32 i4OutHeightStep;

    UINT32 u4CurSrcWidth;
    UINT32 u4CurSrcHeight;
    UINT32 u4CurSrcXOffset;
    UINT32 u4CurSrcYOffset;

    UINT32 u4CurOutWidth;
    UINT32 u4CurOutHeight;
    UINT32 u4CurOutXOffset;
    UINT32 u4CurOutYOffset;

    UINT32 u4OrgSrcWidth;
    UINT32 u4OrgSrcHeight;
    UINT32 u4OrgSrcXOffset;
    UINT32 u4OrgSrcYOffset;

    UINT32 u4OrgOutWidth;
    UINT32 u4OrgOutHeight;
    UINT32 u4OrgOutXOffset;
    UINT32 u4OrgOutYOffset;

    UINT32 u4LastSrcWidth;
    UINT32 u4LastSrcHeight;
    UINT32 u4LastSrcXOffset;
    UINT32 u4LastSrcYOffset;

    UINT32 u4LastOutWidth;
    UINT32 u4LastOutHeight;
    UINT32 u4LastOutXOffset;
    UINT32 u4LastOutYOffset;
} VDP_DYNAMIC_SCALER_PRM_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define DIVIDE_WITH_CARRIER(dividend, divisor)        (((dividend) + ((divisor) - 1)) / (divisor))
#define DIVIDE_WITH_ROUND(dividend, divisor)        (((dividend) + ((divisor) >> 1)) / (divisor))
#define RSHIFT_WITH_CARRIER(dividend, shift)        (((dividend) + ((1 << (shift)) - 1)) >> (shift))
#define RSHIFT_WITH_ROUND(dividend, shift)        (((dividend) + ((1 << (shift))/2)) >> (shift))

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

// nptv global variable
volatile UINT32 _u4ResetScposDramBuffer[VDP_NS] = {0, 0};
static UINT32 _au4DelayWriteEnable[VDP_NS] = {0, 0};

#ifdef CC_SCPOS_WA10
#define MAX_SCALER_FACTOR 0xffffffff
#define FRAME_NUMEBR_CHANGED_COUNT  2
static UINT32 _au4FrameNumberChanged[VDP_NS] = {0, 0};
static UINT32 _au4OriginalDSFactor[VDP_NS] = {0, 0};
static void vScalerWA10_UnlockImportProtection(UINT32 u4VdpId);
#endif

#ifdef CC_TOGGLE_W_EN
#define FRAME_NUMEBR_CHANGED_COUNT  2
UINT32 _au4FrameNumberChanged[VDP_NS] = {0, 0};
#endif

//UINT32 _au4GameMode[VDP_NS] = {0, 0};
//UINT32 _au4OriFrameNum[VDP_NS] = {3, 3};

#if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
UINT32 _counterToInvertVSync[VDP_NS] = {0,0};
UINT32 _flagToInvertVSync[VDP_NS] = {SV_FALSE,SV_FALSE};
#endif

#if CC_SCPOS_VSYNC_WA2
static UINT32 _u4UpdateDisplayModeState = 0;
static BOOL _fgDisplayModeEnable = FALSE;
#endif

#if CC_SCPOS_DISPMODE_NOT_SUPPORT
BOOL fgIsDispFail = FALSE;
#endif

UINT32 _u4OsDumpEnable = SV_FALSE;

#if CC_SCPOS_WA_PMX_DEALY
UINT32 u4PmxDelay[2] = {0, 0};
#endif

#if CC_SCPOS_WA_FLIP_IMPORT
UINT32 u4UpdateFlipImport[2] = {SV_FALSE, SV_FALSE};
#endif

#ifdef CC_SCPOS_WA27
#define SCPIP_UPDATE_MAIN_VDS   (1<<0)
#define SCPIP_UPDATE_MAIN_VUS   (1<<1)
#if SUPPORT_POP
#define SCPIP_UPDATE_PIP_VDS   (1<<2)
#define SCPIP_UPDATE_PIP_VUS   (1<<3)
#endif
static UINT32 _u4VdsVusDealy=0;
static UINT32 _u4UpdateFifoConfig = 0;
static UINT32 _u4VDSBypass[VDP_NS] = {0xff, 0xff};
static UINT32 _u4VUSBypass[VDP_NS] = {0xff, 0xff};
#endif

#ifdef CC_SCPOS_WA30
static void vScpipUpdateImportSel(UINT8 bPath);
#endif

// Initlaize Dispmode Auto Tune Table
Disp_Tune AddrStack[] = 
{
    // 1920x1080 Panel
    {1920, 1080, 1920, 1080, 60, 0, 0x7800, 0x7900, 0x8000, 0x8000, 0, 0x78}, // 1080p_60
    {1920, 1080, 1920, 1080, 50, 0, 0x7800, 0x7900, 0x8000, 0x8000, 0, 0x78}, // 1080p_50
    {1920, 1080, 1920, 1080, 60, 1, 0x7800, 0x7900, 0x8000, 0x8000, 0, 0x78}, // 1080i_60
    {1920, 1080, 1920, 1080, 50, 1, 0x7800, 0x7900, 0x8000, 0x8000, 0, 0x78}, // 1080i_50
};
UINT32 ADDR_STACK_SIZE_DISP = sizeof(AddrStack)/sizeof(AddrStack[0]);

UINT32 _au4ForcePsDivClkEn[VDP_NS] = {0, 0};
UINT32 _au4ForcePsDivClkM[VDP_NS] = {0, 0};
UINT32 _au4ForcePsDivClkN[VDP_NS] = {0, 0};

UINT32 u4BaseAddress[2]= {0, 0}; // scaler dram base address
UINT32 u4DramBufSize[2]= {0, 0};
static UINT8 bSRMStable[2]= {SV_FALSE, SV_FALSE};
static UINT32 _u4SRMFrameDelay[2]={0,0};// AV sync : Frame dealy number for SRM, the value is Delay*2.

// When TVE on, force dispOff
static UINT32 u4ForceDispOff[2]={SV_FALSE, SV_FALSE};

#ifdef CC_TVE_UPSCALE_WA
UINT32 _u4DoubleInputHeight = 0;
#endif
#ifdef CC_TVE_US_IMAGERESIZER_WA
UINT32 _u4ResizeSubDRAM = 0;
HANDLE_T h_enableImageResizerSema = NULL_HANDLE;
HANDLE_T h_enableImageResizerThread = NULL_HANDLE;
RZ_OSD_SCL_PARAM_SET_T _rResizerScaleParam;
UINT32 _u4ResizeWPos = 0;
UINT32 _u4UpdateDRAMReadAddr = 0;
#endif

#ifdef CC_NEW_DRAM_PROTECTION
HANDLE_T h_dramUpdateSema = NULL;
#endif

#if CC_SPEEDUP_IMPORT
UINT32 _au4ImportUpdate[2] = {0, 0};
#endif

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
//static UINT32 _SquareRoot(UINT32 u4Integer);
#ifdef CC_SCPOS_WA20
static void _vScpipToggleVPorch(UINT32 u4VdpId);
UINT8 Toggle_Later = SV_FALSE;
#endif
static void _vScpipSetUSReg(UINT32 u4VdpId);
static void _vScpipSetDispReg(UINT32 u4VdpId);
static void _vScpipCalNewScale(UINT32 u4VdpId);
static void _vScpipUpdateDispPrm(UINT32 u4VdpId);
static void vUpdateNonlinearUSPrm(UINT8 ucVdpId);
static void _vScpipSetBypassUpScaler(UINT8 bPath,UINT8 bMode);
static void _vScpipSetBypassDownScaler(UINT8 bPath,UINT8 bMode);
static void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 u4Source, UINT32 u4Target, UINT32 u4LimitLength, UINT8 bDircection);
static void _vScpipSetDEAlignmentReg(UINT8 index);
static void _vScpipSetPscanPrelen(UINT8 bPath,UINT32 u4PreLen);
static UINT32 _u4ScpipGetPscanDispmodeHLen(UINT8 bPath);
static UINT32 _u4ScpipGetPscanDispmodeHPorch(UINT8 bPath);
static void _vScpipSetDispModeReg(void);
static void _vScpipDispmodeFreeRunChk(void);
static void _vScpipWriteCTRLCheck(UINT8 bPath);
#ifdef CC_NEW_DRAM_PROTECTION
static void _vScpipUpdateDramParameter(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4FrameSize, UINT32 u4FrameNum);
static void _vScpipSetDram(UINT32 u4VdpId, UINT32 u4WAddr, UINT32 u4RAddr, UINT32 u4FrameSize);
#else
static void _vScpipSetDram(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4FrameSize);
#endif
static void _vScpipSetBufNum(UINT32 u4VdpId, UINT32 u4BufNum);
static UINT8 _u1ScpipIsDRAMParamChanged(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4Size, UINT32 u4FrameNum);
static void vDrvSetSCPIPClock(UINT8 bPath, SCPIP_CLOCK_TYPE eType);
static void _vDrvScpipConfigInputClk(UINT8 bPath);
static UINT8 u1ScpipGetPSCANDispmodeScalingMode(UINT8 bPath);
#if CC_TWO_FRAME_LIMIT
static void _vScpipSetTwoFrameLimit(UINT32 u4VdpId);
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
static void _vScalerConfigFlip(UINT8 bPath, UINT8 bDramMode);
#endif
#if 0
//#ifdef CC_DTV_HD_DOT_BY_DOT
UINT32 u4CurrFullDotByDot= 0xff, u4CurrDispmodeOK= 0xff;
static UINT8 _fgScpipFHDDotByDotTiming(UINT8 bPath);
#endif

#if SUPPORT_DOT_TO_DOT_PATTERN
EXTERN void VDP_DotToDotPattern(UINT8 ucOnOff,UINT32 u4RGBColor);
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static RDispInfo* const _pDispInfo[] =
{
    &_rMDispInfo
#if SUPPORT_POP
    , &_rPDispInfo
#endif
};

static RDispPrm* const _pDispPrm[] =
{
    &_rMDispPrm
#if SUPPORT_POP
    , &_rPDispPrm
#endif
};

static RVChannel* const _pChannel[] =
{
    &_rMChannel
#if SUPPORT_POP
    , &_rPChannel
#endif
};

static RPicInfo* const _pPicInfo[] =
{
    &_rMPicInfo
#if SUPPORT_POP
    , &_rPPicInfo
#endif
};

#if !SUPPORT_POP
VDP_SCALER_PRM_T _arScalePrm[VDP_NS] = {{0, DFT_DS_FACTOR, 0, 0, 250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, /*UNIT_NONLINEAR,*/ 0, 0, /*UINT_NONLINEAR_END_FACTOR,*/ TRUE, {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR}, {0, 0, 0, 0}, {0, 0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0}};
#else
VDP_SCALER_PRM_T _arScalePrm[VDP_NS] =
{
    {0, DFT_DS_FACTOR, 0, 0, 250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*UNIT_NONLINEAR,*/ 0, 0, /*UINT_NONLINEAR_END_FACTOR,*/ TRUE, {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR}, {0, 0, 0, 0}, {0, 0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, DFT_DS_FACTOR, 0, 0, 250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*UNIT_NONLINEAR,*/ 0, 0, /*UINT_NONLINEAR_END_FACTOR,*/ FALSE, {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR}, {0, 0, 0, 0}, {0, 0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, 0}
};
#endif

static VDP_DYNAMIC_SCALER_PRM_T _arDynamicPrm[VDP_NS];
static UINT32 _u4ScposImportNotReady = 0;
static UINT32 _u4Forced10BitMode[VDP_NS] = {0, 0};
static UINT32 _au4ScalingFactorChanged[VDP_NS] = {0, 0};

#define CC_SCPOS_FREERUN_MAX        150
static UINT32 _u4DispModeFreeRunCounter =0;

#if CC_SCPOS_FORCE_WOFF
static UINT32 _au4ForceWriteOff[VDP_NS] = {0, 0};
#endif

#ifdef CC_TV_MEMO_MODE
BOOL bTVMemoOnOff = FALSE;
BOOL bDispmode = FALSE;
BOOL bDisable = FALSE;
BOOL bMemoEnable;
UINT8 u4Counter;
#endif

#ifdef CC_SCPOS_WA33
UINT32 _au4ImportTrigger[VDP_NS] = {0, 0};
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


#ifdef CC_TVE_US_IMAGERESIZER_WA
static VOID _enableImageResizerThread(VOID* pv_arg)
{
    while(1)
    {
        UINT32 u4SetWPos = 0;
        RZ_OSD_SCL_PARAM_SET_T rResizerParam;
        CRIT_STATE_T rCritState;
        
        x_sema_lock(h_enableImageResizerSema, X_SEMA_OPTION_WAIT);

        if (_u4ResizeSubDRAM)
        {
            rCritState = x_crit_start();
            
            u4SetWPos = _u4ResizeWPos + 1;
            if (u4SetWPos > RegReadFldAlign(SCPIP_PIP2_09, PIP2_09_FSEL_2))
            {
                u4SetWPos = 0;
            }
            x_memcpy((void *)(&rResizerParam), (void *)(&_rResizerScaleParam), sizeof(RZ_OSD_SCL_PARAM_SET_T));

            x_crit_end(rCritState);
            LOG(6, "CC_TVE_US_IMAGERESIZER_WA: (%d)\n", _u4ResizeWPos);
        }
#if SUPPORT_POP
        if (_u4UpdateDRAMReadAddr)
        {
            _u4UpdateDRAMReadAddr = 0;

            // update scpip DRAM read address
            #ifdef CC_NEW_DRAM_PROTECTION
             _vScpipUpdateDramParameter(VDP_2, u4BaseAddress[VDP_2], PARAM_IGNORE, PARAM_IGNORE);
            #else
            UINT32 u4OriAddr = RegReadFldAlign(SCPIP_PIP1_05+getPIPOffset(SV_VP_PIP), PIP1_05_WSTARTP_1);
            UINT32 u4Update = 0;
            u4OriAddr = u4OriAddr << 4;		
            if (u4OriAddr != u4BaseAddress[VDP_2])
            {
                u4Update = 1;
            }
            if (u4Update)       
            {
                _u4ResetScposDramBuffer[VDP_2] = _u4ResetScposDramBuffer[VDP_2] | 0x80000000;
            }
            _vScpipSetDram(VDP_2, u4BaseAddress[VDP_2], 0);
            if (u4Update)
            {
                _u4ResetScposDramBuffer[VDP_2] = _u4ResetScposDramBuffer[VDP_2] & 0x7fffffff;
                _u4ResetScposDramBuffer[VDP_2] = (_u4ResetScposDramBuffer[VDP_2] < 2)? 2: _u4ResetScposDramBuffer[VDP_2];		  
            }
            #endif
        }
#endif
         
        if (_u4ResizeSubDRAM)
        {
            // 3. start image resizer        
            LOG(6, "\tSrc Addr: 0x%X (%d x %d)\n", rResizerParam.u4SrcBase, rResizerParam.u4SrcW, rResizerParam.u4SrcH);
            LOG(6, "\tDes Addr: 0x%X (%d x %d)\n", rResizerParam.u4TgBase, rResizerParam.u4TgW, rResizerParam.u4TgH);
            IMGRZ_Lock();
            IMGRZ_ReInit();            
            IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
            IMGRZ_Scale((void *)(&rResizerParam));   
            IMGRZ_Flush();
            IMGRZ_Wait();
            IMGRZ_Unlock();
        
            // 4. set read position
            vRegWriteFldAlign(SCPIP_PIP2_09, u4SetWPos, PIP2_09_SET_W_POS_2);
        }
        
    }
}
#endif

/**
* @brief Integer square root algorithm
* @param u4Integer
* @return Result
*/
#if 0//!OLD_NONLINEAE_US_ONOFF
static UINT32 _SquareRoot(UINT32 u4Integer) 
{
    UINT32 u4Result;
    UINT32 u4Var1;
    UINT32 u4Var2;
    UINT32 u4Var3;

    u4Result = 0;
    u4Var2 = 0x8000;
    u4Var3 = 15;

    do 
    {
        if (u4Integer >= (u4Var1 = (((u4Result << 1) + u4Var2) << u4Var3--))) 
        {
            u4Result += u4Var2;
            u4Integer -= u4Var1;
        }
    } while ((u4Var2 >>= 1) != 0);

    return u4Result;
}
#endif

#ifdef CC_SCPOS_WA20
static void _vScpipToggleVPorch(UINT32 u4VdpId)
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    UINT32 u4VPorch;   
    UINT8 bOffset;
    UINT8 bPath;

    bPath = getScalerPath(u4VdpId);
    bOffset = getPIPOffset(bPath);

    u4VPorch = RegReadFldAlign(SCPIP_PIP1_06+bOffset, PIP1_06_WSTARTP_OF1ST_1);
    if(u4VPorch < 100)
    {
        u4VPorch++;
    }
    else
        u4VPorch=0;
    vRegWriteFldAlign(SCPIP_PIP1_06+bOffset, u4VPorch, PIP1_06_WSTARTP_OF1ST_1);

#if !SUPPORT_POP
#undef u4VdpId
#endif
}

#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


void vScpipForceDispOff(UINT32 u4Path, UINT32 u4OnOff)
{
    if (u4ForceDispOff[u4Path] != u4OnOff)
    {
        u4ForceDispOff[u4Path] = u4OnOff;

#ifdef CC_PSCAN_FIELD_TOGGLE_WA
            if (u4Path == SV_VP_PIP)
            {
#ifdef CC_SCPOS_WA30            
                vScpipUpdateImportSel((u4Path==0)? SV_VP_MAIN : SV_VP_PIP);
#endif
                if (u4OnOff)
                {
                    // turn off field toggle
                    if (IS_IC_5363() && (BSP_GetIcVersion() >= IC_VER_5363_AB))
                    {
                        vRegWrite4BMsk(0x487C, 0x00000000, 0x00000002); //field toggle
                    }
                    // set vPORCH 
#ifndef __MODEL_slt__                
                    vRegWriteFldAlign(SCPIP_PIP2_06, (_rPDispPrm.wVPosOfst & 0xff), PIP2_06_WSTARTP_OF1ST_2); 
                    vRegWriteFldAlign(SCPIP_PIP2_05, (_rPDispPrm.wVPosOfst >> 8), PIP2_05_WSTARTP_OF1ST_2); 
#else                
                    vRegWriteFldAlign(SCPIP_PIP2_06, ((_rPDispPrm.wVPosOfst+1) & 0xff), PIP2_06_WSTARTP_OF1ST_2); 
                    vRegWriteFldAlign(SCPIP_PIP2_05, ((_rPDispPrm.wVPosOfst+1) >> 8), PIP2_05_WSTARTP_OF1ST_2); 
#endif                
                }
                else
                {
                    if (IS_IC_5363() && (BSP_GetIcVersion() >= IC_VER_5363_AB))
                    {
                        vRegWrite4BMsk(0x487C, 0x00000002, 0x00000002); //field toggle
                    }
                }
            }
#endif    

    }

    #ifdef CC_TVE_UPSCALE_WA
    if (IS_IC_5363() && (u4Path == SV_VP_PIP))
    {
        _u4DoubleInputHeight = 0;
        
        if (((bGetVideoDecType(SV_VP_PIP) == (UINT8) SV_VD_DVI) || (bGetVideoDecType(SV_VP_PIP) == (UINT8) SV_VD_VGA)) 
			&& (u4OnOff))
        {
            UINT8 ucFmt;
            UINT32 u4HeightLimit = 480;
            TVE_GetFmt(TVE_1, &ucFmt);
            if (ucFmt == TVE_FMT_480I)
            {
                u4HeightLimit = 480;
            }
            else
            {
                u4HeightLimit = 576;
            }

            if (_pPicInfo[SV_VP_PIP]->wTargetH < u4HeightLimit)
            {
                // SVL x2
                _u4DoubleInputHeight = 1;
            }                
        }            
        _VDP_ScalerReconfig(SV_VP_PIP);
    }
    #endif
}



#if CC_SCPOS_WA_FLIP_IMPORT
void vScpipTriggerFlipImport(UINT32 u4VdpId)
{
    UINT32 u4WPos;
    UINT8 bPath;
    UINT32 u4PipOffset;
    UINT32 u4FrameNum;
    
    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    
    bPath = getScalerPath(u4VdpId);
    u4PipOffset = getPIPOffset(bPath);

    if(u4UpdateFlipImport[u4VdpId]== SV_TRUE)
    {
        u4FrameNum = RegReadFldAlign(SCPIP_PIP1_09+u4PipOffset, PIP1_09_FSEL_1);
        u4WPos = RegReadFldAlign(SCPIP_PIP1_RO_00+u4PipOffset, PIP1_RO_00_W_POS_1);
        LOG(4,"vScpipTriggerFlipImport %d %d %d \n", u4VdpId, u4WPos, u4FrameNum);

        if(u4WPos==u4FrameNum)
        {
            HAL_Delay_us(6000);

            #ifdef CC_NEW_DRAM_PROTECTION
            _vScpipUpdateDramParameter(u4VdpId, u4BaseAddress[u4VdpId], PARAM_IGNORE, PARAM_IGNORE);
            #else            
            _u4ResetScposDramBuffer[u4VdpId] = (_u4ResetScposDramBuffer[u4VdpId] < 2)? 2: _u4ResetScposDramBuffer[u4VdpId];
            _vScpipSetDram(u4VdpId, u4BaseAddress[u4VdpId], 0);
            #endif
            
            #if CC_SCPOS_WA_FLIP_IMPORT
            if(u4VdpId== VDP_1)
            {
                vDrvMainImportOnOff(SV_ON);
                vDrvMainUpdateVBound(_pDispPrm[VDP_1]->wVLen);
            }
#if SUPPORT_POP
            else
                vDrvPIPImportOnOff(SV_ON);
            #endif
#endif
            u4UpdateFlipImport[u4VdpId]= SV_FALSE;
            LOG(4,"u4WPos %d %d\n", u4VdpId, u4WPos);
        }
    }
}

static void _vScpipSetDramProtect(void)
{
    UINT32 u4DramSize;
    UINT32 u4HighAddr;
    UINT32 u4LowAddr;

    u4DramSize= BSP_GetMemSize();
    u4LowAddr = u4DramSize;
    u4HighAddr=0;

    if(u4BaseAddress[VDP_1])
    {
        u4LowAddr = u4BaseAddress[VDP_1];
        u4HighAddr = u4BaseAddress[VDP_1]+ u4DramBufSize[VDP_1];
    }
#if SUPPORT_POP
    if(u4BaseAddress[VDP_2])
    {
            if(u4BaseAddress[VDP_2] < u4LowAddr)
            {
                u4LowAddr= u4BaseAddress[VDP_2];
            }

            if((u4BaseAddress[VDP_2]+u4DramBufSize[VDP_2]) > u4HighAddr)
            {
                u4HighAddr= (u4BaseAddress[VDP_2]+u4DramBufSize[VDP_2]);
            }
    }
#endif
    //mid.sr 0 0x463a000 scaler_pip 5 0xffffff7f
    //mid.sr 0x5F0D98C 0x20F2674 scaler_pip 6 0xffffff7f
    //lower bound address <= protection region < upper bound address
    MID_DisableRegionProtect(5);
    MID_DisableRegionProtect(6);
    MID_SetRegionEx2(5, MID_AGENT_SCPOS, 0, u4LowAddr, 0, 0xffffff7f);
    MID_SetRegionEx2(6, MID_AGENT_SCPOS, u4HighAddr+4, u4DramSize, 0, 0xffffff7f);
    MID_EnableRegionProtect(5);
    MID_EnableRegionProtect(6);
}

#endif

#ifdef CC_NEW_DRAM_PROTECTION
static void _vScpipUpdateDramParameter(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4FrameSize, UINT32 u4FrameNum)
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif

    UINT32 u4UpdateWAddr, u4UpdateRAddr, u4UpdateSize, u4UpdateNum;
    UINT32 u4OriWAddr, u4OriRAddr, u4OriSize, u4OriNum;
    RDispPrm* dispPrm;
    UINT8 bPath;
    UINT8 offset;
    UINT8 u1UpdateParameter;
    UINT8 u1SwProtect = 0;

    #ifdef CC_TV_MEMO_MODE
    if (bTVMemoOnOff && (u4VdpId == VDP_1))
    {
        return;
    }
    #endif

    #if !SUPPORT_POP
    if(u4VdpId==VDP_2)
    {
        return;
    }
    #endif

    // Convert to register-based value
    u4UpdateRAddr = u4Addr >> 4;
    u4UpdateSize = u4FrameSize >> 4;
    u4UpdateNum = u4FrameNum - 1;
    u4UpdateWAddr = u4UpdateRAddr;
    u1UpdateParameter = 0;

    bPath = ((u4VdpId == VDP_1) ? SV_VP_MAIN : SV_VP_PIP);
    dispPrm = getDispPrm(bPath);
    offset = getPIPOffset(bPath);

    //re-calculate write address
    if ((RegReadFldAlign(SCPIP_PIP1_05+offset, PIP1_05_WSTARTP_1)!=0) && (u4Addr != PARAM_IGNORE))
    {
        if(bScpipFlipEnable[u4VdpId])
        {
            u4UpdateWAddr = u4UpdateWAddr +(dispPrm->wDataLen)*(dispPrm->wVLen-1);
            LOG(3,"Change Scaler Dram address (0x%X --> 0x%X) with flip\n", u4UpdateRAddr, u4UpdateWAddr);
        }
        #if 0
        if(bScpipMirrorEnable[u4VdpId])
        {
            #ifdef CC_SCPOS_WA28
            u4UpdateAddr=u4UpdateAddr -bReadAddrAdj;
            LOG(3,"\n\nChange Scaler Dram address with Mirror 0x%x %d\n\n", u4UpdateAddr, bReadAddrAdj);
            #endif

            #ifdef CC_SCPOS_WA26
            if(bScpipFlipEnable[u4VdpId]== SV_FALSE)// mirror only
            {
                u4UpdateAddr -=1;   //HL work around
            }
            #endif
        }
        #endif
    }

    // workaround for read address
    if(bScpipMirrorEnable[u4VdpId]==SV_TRUE)
    {
        u4UpdateRAddr += 1;
    }
    else
    {
        #ifdef CC_TVE_US_IMAGERESIZER_WA
        if ((_u4ResizeSubDRAM) && (u4VdpId == VDP_2))
        {
            UINT32 u4Ofst;
            if ((u4FrameSize != PARAM_IGNORE) && (u4UpdateSize != 0))
            {
                u4Ofst = u4UpdateSize;
            }
            else
            {
                u4Ofst = RegReadFldAlign(SCPIP_PIP1_07+offset, PIP1_07_WINC_1);
            }
            u4Ofst = u4Ofst * 3;
            LOG(3, "CC_TVE_US_IMAGERESIZER_WA: Change R Addr from 0x%X --> 0x%X\n", (u4UpdateRAddr<<4), (u4UpdateRAddr+u4Ofst)<<4);
            u4UpdateRAddr = u4UpdateRAddr+u4Ofst;
        }
        #endif    
    }

    if (h_dramUpdateSema)
    {
        VERIFY(x_sema_lock(h_dramUpdateSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
    
    u4OriWAddr = RegReadFldAlign(SCPIP_PIP1_05+offset, PIP1_05_WSTARTP_1);
    u4OriRAddr = RegReadFldAlign(SCPIP_PIP1_06+offset, PIP1_06_RSTARTP_1);
    u4OriSize = RegReadFldAlign(SCPIP_PIP1_07+offset, PIP1_07_WINC_1);    
    u4OriNum = RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_FSEL_1);

    if ((u4OriWAddr != u4UpdateWAddr) && (u4Addr != PARAM_IGNORE))     
    {
        u1UpdateParameter |= BASE_WADDRESS_CHG;
        u4UpdateWAddr <<= 4;
    }
    else
    {
        u4UpdateWAddr = PARAM_IGNORE;
    }
    if ((u4OriRAddr != u4UpdateRAddr) && (u4Addr != PARAM_IGNORE))     
    {
        u1UpdateParameter |= BASE_RADDRESS_CHG;
        u4UpdateRAddr <<= 4;
    }
    else
    {
        u4UpdateRAddr = PARAM_IGNORE;
    }
    if ((u4OriSize != u4UpdateSize) && (u4FrameSize != PARAM_IGNORE))
    {
        u1UpdateParameter |= FRAME_SIZE_CHG;
        u4UpdateSize <<= 4;
    }
    else
    {
        u4UpdateSize = PARAM_IGNORE;
    }
    if ((u4OriNum != u4UpdateNum) && (u4FrameNum != PARAM_IGNORE))
    {
        u1UpdateParameter |= FRAME_NUM_CHG;
        u4UpdateNum += 1;
    }
    else
    {
        u4UpdateNum = PARAM_IGNORE;
    }

    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | FRAME_NUM_CHG))
    {
        #if CC_SCPOS_WA_FLIP_IMPORT
        if (bScpipFlipEnable[u4VdpId])
        {
            // protect by MID
            _vScpipSetDramProtect();
            u1SwProtect = 0;
        }
        else
        #endif
        {
            // protect by SW
            u1SwProtect = 1;    
            _u4ResetScposDramBuffer[u4VdpId] = _u4ResetScposDramBuffer[u4VdpId] | 0x80000000;
            vRegWriteFldAlign(SCPIP_PIP1_09+offset, 0, PIP1_09_WEN_RLH_1);
            vDrvScpipWriteCtrl(u4VdpId, SV_OFF);
        }
    }

    // update base address and frame size
    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | BASE_RADDRESS_CHG))
    {
        _vScpipSetDram(u4VdpId, u4UpdateWAddr, u4UpdateRAddr, u4UpdateSize);
    }
    // update frame number
    if (u1UpdateParameter & FRAME_NUM_CHG)
    {
        #ifdef CC_SCPOS_WA10
         _au4FrameNumberChanged[u4VdpId] = FRAME_NUMEBR_CHANGED_COUNT;
        #endif
        #ifdef CC_TOGGLE_W_EN
         _au4FrameNumberChanged[u4VdpId] = FRAME_NUMEBR_CHANGED_COUNT;
        #endif

        _vScpipSetBufNum(u4VdpId, u4UpdateNum);            
    }

    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | FRAME_NUM_CHG))
    {
        if (u1SwProtect)
        {
            // protect by SW
            CRIT_STATE_T csState; 
            csState = x_crit_start(); 
            _u4ResetScposDramBuffer[u4VdpId] = _u4ResetScposDramBuffer[u4VdpId] & 0x7fffffff;
            _u4ResetScposDramBuffer[u4VdpId] = (_u4ResetScposDramBuffer[u4VdpId]  < 1)? 1: _u4ResetScposDramBuffer[u4VdpId];
            x_crit_end(csState);
        }
    }

    if (h_dramUpdateSema)
    {
        VERIFY(x_sema_unlock(h_dramUpdateSema) == OSR_OK);
    }
#if !SUPPORT_POP
#undef u4VdpId
#endif
}

static void _vScpipSetDram(UINT32 u4VdpId, UINT32 u4WAddr, UINT32 u4RAddr, UINT32 u4FrameSize) 
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif

    UINT8 offset;
    UINT8 bPath;

    bPath = ((u4VdpId == VDP_1) ? SV_VP_MAIN : SV_VP_PIP);
    offset = getPIPOffset(bPath);

    if (u4WAddr != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) Write Address: 0x%X\n", u4VdpId, u4WAddr);
        #if (!CC_SCPOS_WA_FLIP_IMPORT)
        if (RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_WEN_RLH_1) || RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_W_EN_1))
        {
            ASSERT(0);
        }
        #endif
        vRegWriteFldAlign(SCPIP_PIP1_05+offset, (u4WAddr>>4),PIP1_05_WSTARTP_1);
    }
    if (u4RAddr != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) Read Address: 0x%X\n", u4VdpId, u4RAddr);
        vRegWriteFldAlign(SCPIP_PIP1_06+offset, (u4RAddr>>4), PIP1_06_RSTARTP_1);
    }
    if (u4FrameSize != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) frame size: 0x%X\n", u4VdpId, u4FrameSize);
        #if (!CC_SCPOS_WA_FLIP_IMPORT)
        if (RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_WEN_RLH_1) || RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_W_EN_1))
        {
            ASSERT(0);
        }
        #endif
        vRegWriteFldAlign(SCPIP_PIP1_07+offset, (u4FrameSize>>4), PIP1_07_WINC_1);
    }  
#if !SUPPORT_POP
#undef u4VdpId
#endif  
}

#else
/**
* @brief Set dram related registers
* @param u4VdpId VDP_1/VDP_2
* @param u4Addr Read/Write address
* @param u4FrameSize frame buffer size
*/
static void _vScpipSetDram(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4FrameSize) 
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    RDispPrm* dispPrm;
    UINT32 u4UpdateAddr, u4OriAddr, u4OriSize;
    UINT8 offset;
    UINT8 bPath;
    UINT8 ucTveEnabled;

    #ifdef CC_TV_MEMO_MODE
    if (bTVMemoOnOff && (u4VdpId == VDP_1))
    {
        return;
    }
    #endif

    #if !SUPPORT_POP
        if(u4VdpId==VDP_2)
        {
            return;
        }
    #endif
    
    bPath = ((u4VdpId == VDP_1) ? SV_VP_MAIN : SV_VP_PIP);
    dispPrm = getDispPrm(bPath);
    offset = getPIPOffset(bPath);
    TVE_GetEnable(TVE_1, &ucTveEnabled);

    // 5360
    u4Addr >>= 4;
    u4FrameSize >>= 4;
    u4UpdateAddr=u4Addr;

    //set Read/Write address
    if(RegReadFldAlign(SCPIP_PIP1_05+offset, PIP1_05_WSTARTP_1)!=0)
    {
        if(bScpipFlipEnable[u4VdpId])
        {
            u4UpdateAddr =u4UpdateAddr +(dispPrm->wDataLen)*(dispPrm->wVLen-1);
            LOG(3,"Change Scaler Dram address with flip\n");
        }
        #if 0
        if(bScpipMirrorEnable[u4VdpId])
        {
            #ifdef CC_SCPOS_WA28
            u4UpdateAddr=u4UpdateAddr -bReadAddrAdj;
            LOG(3,"\n\nChange Scaler Dram address with Mirror 0x%x %d\n\n", u4UpdateAddr, bReadAddrAdj);
            #endif

            #ifdef CC_SCPOS_WA26
            if(bScpipFlipEnable[u4VdpId]== SV_FALSE)// mirror only
            {
                u4UpdateAddr -=1;   //HL work around
            }
            #endif
        }
        #endif
    }

    u4OriAddr = RegReadFldAlign(SCPIP_PIP1_05+getPIPOffset(u4VdpId), PIP1_05_WSTARTP_1);
	u4OriSize = RegReadFldAlign(SCPIP_PIP1_07+getPIPOffset(u4VdpId), PIP1_07_WINC_1);    
	if((u4OriAddr != u4UpdateAddr)||((u4OriSize != u4FrameSize)&&(u4FrameSize != 0))) // if new address is same with original address, don't reset write control.    
	{    
        if((bScpipFlipEnable[u4VdpId]==SV_FALSE) || (!CC_SCPOS_WA_FLIP_IMPORT))
        {
            LOG(3, "Disable (%d) Write Relatch\n", bPath);
            if (bPath == SV_VP_MAIN)
            {    
                vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_WEN_RLH_1);
            }
#if !SUPPORT_POP
            else
            {
                vRegWriteFldAlign(SCPIP_PIP2_09, 0, PIP2_09_WEN_RLH_2);
            }
#endif
            vDrvScpipWriteCtrl(bPath, SV_OFF);
            LOG(3, "Disable [%d] Write Control\n", u4VdpId);
        }
        #if CC_SCPOS_WA_FLIP_IMPORT
        else
        {
            _vScpipSetDramProtect();
        }
        #endif
        vRegWriteFldAlign(SCPIP_PIP1_05+offset,u4UpdateAddr,PIP1_05_WSTARTP_1);
    }

    if(bScpipMirrorEnable[u4VdpId]==SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_PIP1_06+offset,u4Addr+1, PIP1_06_RSTARTP_1);
    }
    else
    {
        #ifdef CC_TVE_US_IMAGERESIZER_WA
        if ((_u4ResizeSubDRAM) && (u4VdpId == VDP_2))
        {
            UINT32 u4Ofst;
            if (u4FrameSize!=0)
            {
                u4Ofst = u4FrameSize;
            }
            else
            {
                u4Ofst = RegReadFldAlign(SCPIP_PIP1_07+offset, PIP1_07_WINC_1);
            }
            u4Ofst = u4Ofst * 3;
            LOG(3, "CC_TVE_US_IMAGERESIZER_WA: Change R Addr from 0x%X --> 0x%X\n", (u4Addr<<4), (u4Addr+u4Ofst)<<4);
            vRegWriteFldAlign(SCPIP_PIP1_06+offset, u4Addr+u4Ofst, PIP1_06_RSTARTP_1);
        }
        else
        #endif    
        {
            vRegWriteFldAlign(SCPIP_PIP1_06+offset, u4Addr,PIP1_06_RSTARTP_1);
        }
    }
    //set Frame size, if frame size ==0, no update
    if(u4FrameSize!=0)
    {
        vRegWriteFldAlign(SCPIP_PIP1_07+offset,u4FrameSize,PIP1_07_WINC_1);
    }
#if !SUPPORT_POP
#undef u4VdpId
#endif
}

#endif
/**
* @brief Set frame buffer number
* @param u4VdpId VDP_1/VDP_2
* @param u4BufNum frame buffer number
*/
static void _vScpipSetBufNum(UINT32 u4VdpId, UINT32 u4BufNum)
{
    if (u4BufNum < 1)
        return;
    
    if (u4VdpId == VDP_1)
    {
        vRegWriteFldAlign(SCPIP_PIP1_09, u4BufNum-1, PIP1_09_FSEL_1);
    }
    else
    {
    #if SUPPORT_POP
        vRegWriteFldAlign(SCPIP_PIP2_09, u4BufNum-1, PIP2_09_FSEL_2);    
    #endif
    }
}


/**
* @brief Get frame buffer number
* @param u4VdpId VDP_1/VDP_2
*/
UINT32 u4ScpipGetBufNum(UINT32 u4VdpId)
{
    UINT32 u4FrameNum;

    u4FrameNum = 1;
    if (u4VdpId == VDP_1)
    {
        u4FrameNum = RegReadFldAlign(SCPIP_PIP1_09, PIP1_09_FSEL_1) + 1;
    }
    else
    {
#if SUPPORT_POP
        u4FrameNum = RegReadFldAlign(SCPIP_PIP2_09, PIP2_09_FSEL_2) + 1;
#endif
    }

    return u4FrameNum;    
}


/**
* @brief Set drame related information
* @param u4VdpId VDP_1/VDP_2
* @param prFbmPool parameters from srm
*/
void vScpipSrmSetBuf(UINT32 u4VdpId, const SCALER_FBM_POOL_T * prFbmPool) 
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
//    UINT32 u4Size;
    UINT32 u4FbSize;
    UINT8 u1ParamChanged;
    //UINT32 u4BaseAddress;
    UINT32 u4FrameNum;
    
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);
    _arScalePrm[u4VdpId].u4MddiDramWidth = prFbmPool->u4MddiWidth;

    u1ParamChanged = 0;
    #if 0
    #ifdef CC_FLIP_MIRROR_SUPPORT           
    u4FrameNum=(prFbmPool->fg2FB)?2: 4;       
    #else                                     
    u4FrameNum = 3- (UINT32)(prFbmPool->fg2FB); 
    #endif     
    #else
    #ifdef CC_FLIP_MIRROR_SUPPORT           
    u4FrameNum = 4;       
    #else                                     
    u4FrameNum = 3; 
    #endif     
  
    #endif

    
    if (_u4Forced10BitMode[u4VdpId])
    {
        if ((_u4Forced10BitMode[u4VdpId] == VDP_SCPOS_10BitMode) && (prFbmPool->fg422 == SV_TRUE))
        {
            _arScalePrm[u4VdpId].u410BitMode = 1;
            u1ParamChanged = COLOR_DEPTH_CHG;
            LOG(3, "Force to SCPOS 10 Bit mode\n");   
        }
        else if (_u4Forced10BitMode[u4VdpId] == VDP_SCPOS_8BitMode)
        {
            _arScalePrm[u4VdpId].u410BitMode = 0;
            u1ParamChanged = COLOR_DEPTH_CHG;
            LOG(3, "Force to SCPOS 8 Bit mode\n");     
        }
        _u4Forced10BitMode[u4VdpId] = 0;
    }
    else
    {
        if (((prFbmPool->fg10bit) && (prFbmPool->fg422 == SV_TRUE)) 
            && (_arScalePrm[u4VdpId].u410BitMode == 0))
        {
            _arScalePrm[u4VdpId].u410BitMode = 1;
            u1ParamChanged = COLOR_DEPTH_CHG;
            LOG(3, "SCPOS 10 Bit mode\n");          
        }
        else if (((prFbmPool->fg8bit) || (prFbmPool->fg422==SV_FALSE))
            && (_arScalePrm[u4VdpId].u410BitMode == 1))
        {
            _arScalePrm[u4VdpId].u410BitMode = 0;
            u1ParamChanged = COLOR_DEPTH_CHG;
            LOG(3, "SCPOS 8 Bit mode\n");         
        }
    }
	#ifdef CC_SCPOS_WA29
	 bPreColorDeth[u4VdpId] = _arScalePrm[u4VdpId].u410BitMode;
	#endif
    _arScalePrm[u4VdpId].u4DramWidth = prFbmPool->u4Width;
    _arScalePrm[u4VdpId].u4DramHeight = prFbmPool->u4Height;
    if ((_arScalePrm[u4VdpId].u4DramWidth == 0) || (_arScalePrm[u4VdpId].u4DramHeight == 0))
    {
        return;
    }
    
    u4FbSize = (prFbmPool->u4FbmSize / u4FrameNum);
    u4BaseAddress[u4VdpId] = prFbmPool->u4Addr;
    u4DramBufSize[u4VdpId] = prFbmPool->u4FbmSize;
    _arScalePrm[u4VdpId].u4Is422= prFbmPool->fg422;

            #if 0// move to _vScpipCalDramLimit()
    if (_arScalePrm[u4VdpId].u4Is422==SV_FALSE) 
    {
        #ifdef CC_SCPOS_WA3
            u4Size = (UINT32)((_arScalePrm[u4VdpId].u4DramWidth * 3 + 15)/16 + 1) * 16 * (UINT32)(_arScalePrm[u4VdpId].u4DramHeight + 4); /* 444 */
        #else
            u4Size = (UINT32)((_arScalePrm[u4VdpId].u4DramWidth * 3 + 15)/16) * 16 * (UINT32)(_arScalePrm[u4VdpId].u4DramHeight + 4); /* 444 */
        #endif
    }
    else
    {
        if (_arScalePrm[u4VdpId].u410BitMode)
        {    
            #ifdef CC_SCPOS_WA3
                u4Size = (UINT32)((_arScalePrm[u4VdpId].u4DramWidth + 5)/6 + 1) * (UINT32)(_arScalePrm[u4VdpId].u4DramHeight + 4) * 16; // 422-10 bit mode
            #else
                u4Size = (UINT32)((_arScalePrm[u4VdpId].u4DramWidth + 5)/6) * (UINT32)(_arScalePrm[u4VdpId].u4DramHeight + 4) * 16; // 422-10 bit mode
            #endif
        }
        else
        {
            #ifdef CC_SCPOS_WA3
                u4Size = (UINT32)((_arScalePrm[u4VdpId].u4DramWidth + 7)/8 + 1) * (UINT32)(_arScalePrm[u4VdpId].u4DramHeight + 4) * 16; /* 422 */
            #else
                u4Size = (UINT32)((_arScalePrm[u4VdpId].u4DramWidth + 7)/8) * (UINT32)(_arScalePrm[u4VdpId].u4DramHeight + 4) * 16; /* 422 */
            #endif
        }
    }
        
    if (u4FbSize >= u4Size)
    {
        //u4Size = u4FbSize;
        //LOG(3, "u4FbSize >= u4Size, use u4FbSize as u4Size\n");
    }
    else
    {
        Printf("SRM allocated buffer is too small!!");
        //ASSERT(0);
    }
        
        _arScalePrm[u4VdpId].u4FbAddr1 = prFbmPool->u4Addr;
        _arScalePrm[u4VdpId].u4FbAddr2 = _arScalePrm[u4VdpId].u4FbAddr1 + u4Size;
        if (u4FrameNum <= 2)
        {
            _arScalePrm[u4VdpId].u4FbAddr3 = 0;
        }
        else
        {
            _arScalePrm[u4VdpId].u4FbAddr3 = _arScalePrm[u4VdpId].u4FbAddr2 + u4Size;
        }
#endif


#if 0
    _au4OriFrameNum[u4VdpId] = u4FrameNum;
    if ((_au4GameMode[u4VdpId]) && (u4FrameNum > 2))
    {
        LOG(3, "GameMode: Force scalar to 2 frame mode! (%d)\n", u4FrameNum);
        u4FrameNum = 2;
    }
    #endif

    u1ParamChanged |= _u1ScpipIsDRAMParamChanged(u4VdpId, u4BaseAddress[u4VdpId] , u4FbSize, u4FrameNum);

    if (u1ParamChanged)
    {
      //      if ((u1ParamChanged & BASE_ADDRESS_CHG) || (u1ParamChanged & FRAME_SIZE_CHG) || (u1ParamChanged & COLOR_DEPTH_CHG))
            {
                /* Dram address or size changed => handle mute peggy 20071107 */
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, u4VdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_WEN_WAIT, TRUE);
            }

            #ifdef CC_NEW_DRAM_PROTECTION
            UINT32 u4Addr, u4Size, u4Num;
            u4Addr = PARAM_IGNORE;
            u4Size = PARAM_IGNORE;
            u4Num = PARAM_IGNORE;
            if ((u1ParamChanged & BASE_WADDRESS_CHG) || (u1ParamChanged & FRAME_SIZE_CHG))
            {
                u4Addr = u4BaseAddress[u4VdpId];
                u4Size = u4FbSize;
            }
            if (u1ParamChanged & FRAME_NUM_CHG)
            {
                u4Num = u4FrameNum;
            }
            _vScpipUpdateDramParameter(u4VdpId, u4Addr, u4Size, u4Num);                       
            #else
            _u4ResetScposDramBuffer[u4VdpId] = _u4ResetScposDramBuffer[u4VdpId] | 0x80000000;
            LOG(3, "Disable (%d) Write Relatch\n", u4VdpId);
            if (u4VdpId == VDP_1)
            {    
                vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_WEN_RLH_1);
            }
            else
            {
                vRegWriteFldAlign(SCPIP_PIP2_09, 0, PIP2_09_WEN_RLH_2);
            }
           //LOG(3, "SCPOS MEM_POOL(%d): Addr (0x%x) Size(%d) FrmNum(%d)\n", u4VdpId, u4BaseAddress, u4FbSize, u4FrameNum);

           if (u4VdpId == VDP_1)
           {    
               vDrvScpipWriteCtrl((UCHAR)u4VdpId, SV_OFF);
               LOG(3, "Disable Main Write Control\n");
           }
           else 
           {    
               vDrvScpipWriteCtrl((UCHAR)u4VdpId, SV_OFF);
               LOG(3, "Disable Sub Write Control\n");
           }

            if ((u1ParamChanged & BASE_WADDRESS_CHG) || (u1ParamChanged & FRAME_SIZE_CHG))
            {
                _vScpipSetDram(u4VdpId, u4BaseAddress[u4VdpId] , u4FbSize);
            }
            if (u1ParamChanged & FRAME_NUM_CHG)
            {
                #ifdef CC_SCPOS_WA10
                 _au4FrameNumberChanged[u4VdpId] = FRAME_NUMEBR_CHANGED_COUNT;
                #endif
                #ifdef CC_TOGGLE_W_EN
                 _au4FrameNumberChanged[u4VdpId] = FRAME_NUMEBR_CHANGED_COUNT;
                #endif
            
                _vScpipSetBufNum(u4VdpId, u4FrameNum);        
            }
            _u4ResetScposDramBuffer[u4VdpId] = _u4ResetScposDramBuffer[u4VdpId] & 0x7fffffff;
            _u4ResetScposDramBuffer[u4VdpId] = (_u4ResetScposDramBuffer[u4VdpId] < 2)? 2: _u4ResetScposDramBuffer[u4VdpId];
            #endif
    
            if (u1ParamChanged & COLOR_DEPTH_CHG)
            {
                if (u4VdpId == VDP_1)
                {
                    vSetScposFlg(MAIN_DISP_PRM_CHG);
                }
#if SUPPORT_POP
                else
                {
                    vSetScposFlg(PIP_DISP_PRM_CHG);
                }
#endif
            }
           
    } 
#if !SUPPORT_POP
#undef u4VdpId
#endif
}

/**
* @brief Set initial down scale factor
* @param u4VdpId VDP_1/VDP_2
*/
static void _VDP_vSetDSInitialFactor(UINT32 u4VdpId) 
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    UINT8 dsOffset;
    UINT8 bPath;
    RDispPrm* dispPrm;
    UINT32 u4HInitValue;

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    dispPrm = getDispPrm(bPath);
    u4HInitValue = DFT_DS_FACTOR- dispPrm->u4DSclH;

    #if CC_SCPOS_WA_REMOVE_FRACTION_PART
    if (dispPrm->u4DSclH < 0x2000)
    {
        u4HInitValue = u4HInitValue & 0xfff0;
    }
    #endif

    vRegWriteFldAlign(SCPIP_DS1_01+dsOffset, u4HInitValue, DS1_01_DSCALE_HOFST_1);
    
    // always keep VOFST == 0
    vRegWriteFldAlign(SCPIP_DS1_01+dsOffset, 0, DS1_01_DSCALE_VOFST_1);    
#if !SUPPORT_POP
#undef u4VdpId
#endif
}


/**
* @brief Set pre-scaling down factor
* @param u4VdpId VDP_1/VDP_2
*/
static void _VDP_vSetHPreScalingDownFactor(UINT32 u4VdpId)
{  
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
      
    UINT32 u4HInitValue;
    UINT32 u4HpsFactor; 

    u4HInitValue = DFT_HPD_FACTOR - _arScalePrm[u4VdpId].u4HpsFactor;
    u4HpsFactor = _arScalePrm[u4VdpId].u4HpsFactor;

    #if CC_SCPOS_WA_REMOVE_FRACTION_PART
    if (u4HpsFactor < 0x1000)
    {         //remove fraction part
        u4HInitValue = u4HInitValue & 0xfff0;
        u4HpsFactor = u4HpsFactor & 0xfff0;
    }
    #endif        

    #if SUPPORT_POP
    if (u4VdpId == VDP_2)
    {
        vRegWriteFldAlign(SCPIP_HDS_01, u4HInitValue, SCPIP_HDS_PIP_OFST);
        vRegWriteFldAlign(SCPIP_HDS_01, u4HpsFactor, SCPIP_HDS_PIP_SCALER);
    }
    else
    #endif
    {
        vRegWriteFldAlign(SCPIP_HDS_00, u4HpsFactor, SCPIP_HDS_MAIN_SCALER);
        vRegWriteFldAlign(SCPIP_HDS_00, u4HInitValue, SCPIP_HDS_MAIN_OFST);
    }
#if !SUPPORT_POP
#undef u4VdpId
#endif
}


/**
* @brief Set down scale factor
* @param u4VdpId VDP_1/VDP_2
*/
static void _VDP_vSetDownScalingFactor(UINT32 u4VdpId)
{
    #if SUPPORT_POP
        if(u4VdpId == VDP_2)
        {
            return;
        }
    #else
    #define u4VdpId VDP_1
    #endif

    RDispPrm* dispPrm;
    UINT8 dsOffset;
    UINT8 bPath;
    UINT32 u4HdsFactor;
    UINT32 u4VdsFactor;

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    dispPrm = getDispPrm(bPath);

    u4HdsFactor = dispPrm->u4DSclH;
    u4VdsFactor = dispPrm->u4DSclV;

    #if CC_SCPOS_WA_REMOVE_FRACTION_PART
    //remove fraction part of scaling factor
    if (u4HdsFactor < 0x2000)
    {
        u4HdsFactor = u4HdsFactor & 0xfff0;
    }
    if (u4VdsFactor < 0x2000)
    {
        u4VdsFactor = u4VdsFactor & 0xfff0;
    }
    #endif

    /* Set Down Scaling factor */
    vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, u4HdsFactor, DS1_00_DSCALER_H_1);
    vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, u4VdsFactor, DS1_00_DSCALER_V_1);    
#if !SUPPORT_POP
#undef u4VdpId
#endif
}

UINT16 wScpipGetDispWidth(UINT8 bPath)
{    
        RDispPrm* dispPrm;    
        dispPrm = getDispPrm(bPath);    
        return dispPrm->wRHLen;
}
/**
* @brief Set up scaler related registers
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipSetUSReg(UINT32 u4VdpId) 
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif

    RVChannel* channel;
    RDispPrm* dispPrm;
    UINT8 usOffset;
    UINT8 bPath;

    //if don't support POP function
    #if !SUPPORT_POP
        if(u4VdpId == VDP_2)
        {
            return;
        }
    #endif

    bPath = getScalerPath(u4VdpId);
    usOffset = getUSOffset(bPath);
    channel = getChannel(bPath);
    dispPrm = getDispPrm(bPath);

    /* Up Scaler for Main */
    if (channel->bIsChannelOn != SV_OFF)
    {
        //       LogSD("_arScalePrm[u4VdpId].u4NonlinearCtrl", _arScalePrm[u4VdpId].u4NonlinearCtrl);
        if (_arScalePrm[u4VdpId].u4NonlinearCtrl == 0)
        {
            vRegWriteFldAlign(SCPIP_US1_00 + usOffset, dispPrm->u4USclH, US1_00_USCALE_H_1);
            vRegWriteFldAlign(SCPIP_US1_00 + usOffset, dispPrm->u4USclV, US1_00_USCALE_V_1);
            vRegWriteFldAlign(SCPIP_US1_15, 0, US1_15_NONL_H_STEP_1);
            vRegWriteFldAlign(SCPIP_US1_01 + usOffset, (dispPrm->u4USclH == 0x8000) ? 1 : 0, US1_01_HORIZONTAL_FILTER_OFF_1);
            vRegWriteFldAlign(SCPIP_US1_01 + usOffset, (dispPrm->u4USclV == 0x8000) ? 1 : 0, US1_01_VERTICAL_FILTER_OFF_1);
#if 0
            //   vRegWriteFldAlign(SCPIP_US1_01 + usOffset, 0x80, US1_01_NONLSTOP_1);
            //      vRegWriteFldAlign(SCPIP_US1_01 + usOffset, 0x0, US1_01_SHOFST_1);
#endif
        }
        else    //non-linear scaling
        {
#if 0
            vRegWriteFldAlign(SCPIP_US1_00 + usOffset, (_arScalePrm[u4VdpId].rNonlinearPrm.u4Start << 4), US1_00_USCALE_H_1);
            vRegWriteFldAlign(SCPIP_US1_00 + usOffset, dispPrm->u4USclV, US1_00_USCALE_V_1);
            //    vRegWriteFldAlign(SCPIP_US1_01 + usOffset, (_arScalePrm[u4VdpId].rNonlinearPrm.u4End >> 4), US1_01_NONLSTOP_1);
            //   vRegWriteFldAlign(SCPIP_US1_01 + usOffset, _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint, US1_01_MIDDLEP_1);
            //    vRegWriteFldAlign(SCPIP_US1_01 + usOffset, _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope, US1_01_SHOFST_1);
#endif
            vRegWriteFldAlign(SCPIP_US1_01, 0, US1_01_NONLINEAR_HNEG);
            vRegWriteFldAlign(SCPIP_US1_15, 1, US1_15_NONL_H_STEP_1);
            vRegWriteFldAlign(SCPIP_US1_15, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4MiddleWidth, US1_15_NONL_H_MIDDLE_1);
            vRegWriteFldAlign(SCPIP_US1_15, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StopPoint, US1_15_NONL_H_STOP_POINT_1);
            vRegWriteFldAlign(SCPIP_US1_00 + usOffset, dispPrm->u4USclV, US1_00_USCALE_V_1);
            vRegWriteFldAlign(SCPIP_US1_00, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StartFactor, US1_00_USCALE_H_1);// start factor
            vRegWriteFldAlign(SCPIP_US1_01, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4IncreaseFactor, US1_01_NONL_H_USCALE_1);//increase factor
        }
    }
#if !SUPPORT_POP
#undef u4VdpId
#endif
}


/**
* @brief Set two frame limit
* @param u4VdpId VDP_1/VDP_2
*/
#if CC_TWO_FRAME_LIMIT
static void _vScpipSetTwoFrameLimit(UINT32 u4VdpId)
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    UINT8 pipOffset;
    UINT8 bPath;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    pipOffset = getPIPOffset(bPath);
    dispPrm = getDispPrm(bPath);

    if (RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_FSEL_1) == 1) // two frame
    {
        vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, (dispPrm->wVLen/16)-1, PIP1_09_TWO_FRAME_LIMIT_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, 0x0, PIP1_09_TWO_FRAME_LIMIT_1);
    }
#if !SUPPORT_POP
#undef u4VdpId
#endif
}
#endif


/**
* @brief Set display related registers
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipSetDispReg(UINT32 u4VdpId)
{

#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    //RVChannel* channel;
    RDispPrm* dispPrm;
    UINT8 dsOffset;
    UINT8 pipOffset;
    UINT8 bPath;
	UINT8 bMode;
	UINT8 bCfg;
    UINT8 bLineNum;
    #if !SUPPORT_POP
        if(u4VdpId == VDP_2)
        {
            return;
        }
    #endif

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    pipOffset = getPIPOffset(bPath);
    dispPrm = getDispPrm(bPath);
    //channel = getChannel(bPath);
    bMode = getScalerMode(bPath);	
                
    /* Down Scaler for Main */
    /* DTV00209813, update display window even when no source is selected */
    //if (channel->bIsChannelOn == SV_ON)
    {
        UINT32 u4Val1;
        UINT32 u4RHP, u4RHL, u4RVP, u4RVL;
        #ifdef CC_FLIP_MIRROR_SUPPORT
        UINT8 u1Config = u1GetFlipMirrorConfig();
        #endif

	#ifdef CC_SCPIP_WA_FLIP_2ND_LINE
        vRegWriteFldAlign(SCPIP_PIP1_09+dsOffset, dispPrm->u4ColorFormat, PIP1_09_BLANKSEL_1);
	#endif

        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_VD10BSEL_1) != (_arScalePrm[u4VdpId].u410BitMode))
        {
            _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R |SCPIP_UPDATE_W);
        }
        #endif        
        // 1. set 8 bit/10bit
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, (_arScalePrm[u4VdpId].u410BitMode), DS1_02_VD10BSEL_1);
                
        /*2. Set 444/422 Selector & blanksel*/
        u4Val1 = (bIsScalerInput444(bPath) == SV_TRUE)? 1 :0;
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, u4Val1, DS1_02_SEL422to444_1);
        vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, u4Val1, PIP1_09_BLANKSEL_1);
        #ifdef CC_SCPOS_WA29
        if(((bForce444_10b[u4VdpId])||u4Val1) != RegReadFldAlign(SCPIP_DS1_02+dsOffset,  DS1_02_SEL444_1))
        {
             #ifndef CC_MIRROR_SW_WA//cr[DTV00213821]when in mirror/flip mode,422 force to 444,there is no need to mute
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_WA, u4VdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_IMPORT_PROTECT, TRUE);	
            LOG(6, "MUTE_MODULE_SCPOS_WA: CC_SCPOS_WA29 :mute , Datlen < 48 + VUS\n");
			 #endif
        }
        //vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, (bForce444_10b[u4VdpId])||u4Val1, DS1_02_SEL444_1);
        u4Val1 = (bForce444_10b[u4VdpId]||u4Val1)? 1: 0;
        #endif
        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_SEL444_1) != u4Val1)
        {
            _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R |SCPIP_UPDATE_W);
        }
        #endif        
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, u4Val1, DS1_02_SEL444_1);
	#ifdef CC_MIRROR_SW_WA //force ycbcr 422 to rgb444 in scaler to avoid color shift.
	if(bIsScalerInput444(u4VdpId) != 1)
	{
		 vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_SEL444_1);
         	 vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_SEL422to444_1);
	}
	#endif
        /*3. RMVP/RMHP/RMVL/RMHL -- Display Window Position and Size */
        #if CC_SCPOS_WA_PMX_DEALY
        //vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, ((dispPrm->wRHPos) + u4PmxDelay[u4VdpId]), PIP1_03_RHP_1);
        u4RHP = ((dispPrm->wRHPos) + u4PmxDelay[u4VdpId]);
        #else
        //vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, (dispPrm->wRHPos), PIP1_03_RHP_1);
        u4RHP = (dispPrm->wRHPos);
        #endif
        #if CC_CTI_WA
        if (u4VdpId == VDP_1)
        {
            //vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, dispPrm->wRHLen+CC_CTI_LEFT_MARGIN+CC_CTI_RIGHT_MARGIN, PIP1_03_RHL_1);
            u4RHL = dispPrm->wRHLen+CC_CTI_LEFT_MARGIN+CC_CTI_RIGHT_MARGIN;
        }
        else
    	#endif
    	#ifdef CC_SCPOS_WA31
        if (u4VdpId == VDP_1)
        {
            //vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, dispPrm->wRHLen+bAddPScanMirWidth, PIP1_03_RHL_1);
            u4RHL = dispPrm->wRHLen+bAddPScanMirWidth;
        }
        else
    	#endif
        {
            //vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, dispPrm->wRHLen, PIP1_03_RHL_1);
            u4RHL = dispPrm->wRHLen;
        }

/*        if(bScpipFlipEnable[u4VdpId])
        {
    	    //vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, (wDISPLAY_HEIGHT - dispPrm->wRVPos -dispPrm->wRVLen), PIP1_02_RVP_1);
    	    u4RVP = (wDISPLAY_HEIGHT - dispPrm->wRVPos -dispPrm->wRVLen);
        }
        else*/
        {
           // vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, dispPrm->wRVPos, PIP1_02_RVP_1);
            u4RVP = dispPrm->wRVPos;
        }
        u4RVL = dispPrm->wRVLen;

        #ifdef CC_FLIP_MIRROR_SUPPORT
        if (u1Config & SYS_MIRROR_CONFIG_ON)
        {
            u4RHP = (wDISPLAY_WIDTH > (u4RHP+u4RHL))? (wDISPLAY_WIDTH - u4RHP - u4RHL): 0;
        }
        if (u1Config & SYS_FLIP_CONFIG_ON)
        {
            u4RVP = (wDISPLAY_WIDTH > (u4RVP+u4RVL))? (wDISPLAY_HEIGHT - u4RVP -u4RVL): 0;
        }
        #endif

        #if CC_SPEEDUP_IMPORT
        if ((RegReadFldAlign(SCPIP_PIP1_03+pipOffset, PIP1_03_RHP_1) != u4RHP)
            ||(RegReadFldAlign(SCPIP_PIP1_03+pipOffset, PIP1_03_RHL_1) != u4RHL)
            ||(RegReadFldAlign(SCPIP_PIP1_02+pipOffset, PIP1_02_RVP_1) != u4RVP)
            ||(RegReadFldAlign(SCPIP_PIP1_02+pipOffset, PIP1_02_RVL_1) != u4RVL)
            )
        {
            _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R);
        }
        #endif        
    	 vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, u4RHP, PIP1_03_RHP_1);
    	 vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, u4RHL, PIP1_03_RHL_1);
        vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, u4RVP, PIP1_02_RVP_1);
        vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, u4RVL, PIP1_02_RVL_1);
                
        /*4. MVP/MHP/MVL/MHL -- Position inside the display window controlled by RMVP/RMHP/RMVL/RMHL */
        #if CC_SPEEDUP_IMPORT
        if ((RegReadFldAlign(SCPIP_PIP1_01+pipOffset, PIP1_01_HP_1) != dispPrm->wHPos)
            ||(RegReadFldAlign(SCPIP_PIP1_01+pipOffset, PIP1_01_HL_1) != dispPrm->wHLen)
            ||(RegReadFldAlign(SCPIP_PIP1_00+pipOffset, PIP1_00_VP_1) != dispPrm->wVPos)
            ||(RegReadFldAlign(SCPIP_PIP1_00+pipOffset, PIP1_00_VL_1) != dispPrm->wVLen)
            )
        {
            _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R |SCPIP_UPDATE_W);
        }
        #endif        
        vRegWriteFldAlign(SCPIP_PIP1_01+pipOffset, dispPrm->wHPos, PIP1_01_HP_1);
        vRegWriteFldAlign(SCPIP_PIP1_01+pipOffset, dispPrm->wHLen, PIP1_01_HL_1);
        vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->wVPos, PIP1_00_VP_1);
        vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->wVLen, PIP1_00_VL_1);
        
        /*5. MHP_OFST -- Picture Target X Offset */
        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_PIP1_04+pipOffset, PIP1_04_HP_OFST_1) != dispPrm->wHPosOfst)
        {
            _au4ImportUpdate[u4VdpId] |= SCPIP_UPDATE_W;
        }
        #endif        
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);

        /*6. WSTARTP_OFST -- Picture Target Y Offset */
        #ifdef __MODEL_slt__        
        // to fix error lines at the top of the screen
        u4Val1 = dispPrm->wVPosOfst+1;
        #else
        u4Val1 = dispPrm->wVPosOfst;
        #endif
        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_PIP1_04+pipOffset, PIP1_04_WSTARTP_OFST_1) != u4Val1)
        {
            _au4ImportUpdate[u4VdpId] |= SCPIP_UPDATE_W;
        }
        #endif        
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, u4Val1, PIP1_04_WSTARTP_OFST_1);
        #ifdef CC_PSCAN_FIELD_TOGGLE_WA        
        if ((IS_IC_5363() && (BSP_GetIcVersion() >= IC_VER_5363_AB)) && (u4ForceDispOff[u4VdpId] && (u4VdpId == SV_VP_PIP))) // 5363 ECO & TVE & PIP
        {
            #if CC_SPEEDUP_IMPORT
            if ((RegReadFldAlign(SCPIP_PIP1_06+pipOffset, PIP1_06_WSTARTP_OF1ST_1) != (u4Val1 & 0xff))
                || (RegReadFldAlign(SCPIP_PIP1_05+pipOffset, PIP1_05_WSTARTP_OF1ST_1) != (u4Val1 >> 8)))
            {
                _au4ImportUpdate[u4VdpId] |= SCPIP_UPDATE_W;
            }
            #endif        
            vRegWriteFldAlign(SCPIP_PIP1_06+pipOffset, (u4Val1 & 0xff), PIP1_06_WSTARTP_OF1ST_1); 
            vRegWriteFldAlign(SCPIP_PIP1_05+pipOffset, (u4Val1 >> 8), PIP1_05_WSTARTP_OF1ST_1); 
        }
        #endif        
        
        /*7. DATA_LENGTH */
#ifdef CC_SCPOS_WA32
        if((dispPrm->wDataLen != RegReadFldAlign(SCPIP_PIP1_08+pipOffset,PIP1_08_DATA_LENGTH_1)) &&
            (dispPrm->wVPosOfst ==0) &&   //no overscan
            (dispPrm->u4DSclV !=DFT_DS_FACTOR))// v down 
        {
             _vDrvVideoSetMute(MUTE_MODULE_SCPOS_WA, u4VdpId, 
                                          (VDP_SCPOS_RESCALE_TIME + VDP_MDDI_RESCALE_TIME),
                                          TRUE);
             LOG(6, "MUTE_MODULE_SCPOS_WA : CC_SCPOS_WA32 (Import WA)\n");
        }
#endif
        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_PIP1_08+pipOffset, PIP1_08_DATA_LENGTH_1) != dispPrm->wDataLen)
        {
            _au4ImportUpdate[u4VdpId] |= SCPIP_UPDATE_R;
        }
        #endif 
		 //[DTV00145304] line_cfg need to recal when change aspect 
        if((dispPrm->wDataLen != RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1)) && 
			((bMode == VDP_SCPOS_DISPMODE_AUTO) || (bMode == VDP_SCPOS_DISPMODE_MANUAL)))
    	 {
    	 //   vScpipDispmodeLineCfg(bPath, bMode);
			bLineNum = 1440/dispPrm->wDataLen;
			if(bLineNum <4)
			{
			bCfg = 0; //3 lines
			}
			else if(bLineNum <6)
			{
			bCfg = 1; //4 lines
			}
			else if(bLineNum <8)
			{
			bCfg = 2; //6 lines
			}
			else
			{
			bCfg = 3; //8 lines
			}
			vRegWriteFldAlign(SCPIP_PIP_OA_06, bCfg, PIP_OA_06_CFG_LINE1);
			vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);
			vRegWriteFldAlign(SCPIP_PIP_OA_08, 1, PIP_OA_08_DFIFO_AUTO_TUNE1);
        }
        vRegWriteFldAlign(SCPIP_PIP1_08+pipOffset, dispPrm->wDataLen, PIP1_08_DATA_LENGTH_1);

        /*8. H Boundary protection */
#ifdef CC_SCPOS_WA31
        if (u4VdpId == VDP_1)
        {
            if((dispPrm->u4HBound+bAddPScanMirWidth) < RegReadFldAlign(SCPIP_PIP1_0F+pipOffset,PIP1_0F_BOUNDARY_H_POS_1))
            {
                // if BOUNDARY_H_POS_1 become smaller we should disable H Boundary protection first, then _u4ScposImportNotReady is 0 enable H Boundary protection.
                vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, 0, PIP1_0F_BOUNDARY_H_EN_1);
            }
            vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, dispPrm->u4HBound+bAddPScanMirWidth,PIP1_0F_BOUNDARY_H_POS_1);
        }
        else
#else
        {
            if(dispPrm->u4HBound < RegReadFldAlign(SCPIP_PIP1_0F+pipOffset,PIP1_0F_BOUNDARY_H_POS_1))
    	    {
     	        // if BOUNDARY_H_POS_1 become smaller we should disable H Boundary protection first, then _u4ScposImportNotReady is 0 enable H Boundary protection.
     	        vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, 0, PIP1_0F_BOUNDARY_H_EN_1);
     	    }
            vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, dispPrm->u4HBound -2, PIP1_0F_BOUNDARY_H_POS_1);
        }
#endif//CC_SCPOS_WA31


        // mirror mode settings
        {
#ifdef CC_SCPOS_WA28
            UINT8 bReadIs444= RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_SEL444_1);        
            static UINT32 au4AddDelay[2]; // for 5N+1 work around
#endif 
          //UVINV_1 and Read_initp_1 not import
           if((RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_UVINV_1)!= _pDispPrm[u4VdpId]->u1UVInv) || 
				(RegReadFldAlign(SCPIP_PIP1_0B+pipOffset, PIP1_0B_READ_INITP_1) != dispPrm->u4ReadInitP))
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_WA, u4VdpId, VDP_SCPOS_RESCALE_TIME, TRUE);
            }
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, _pDispPrm[u4VdpId]->u1UVInv, DS1_02_UVINV_1);
            
            if(bScpipMirrorEnable[u4VdpId])
            {
                //vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, (!bReadIs444), PIP1_0F_MIRROR_OPT_YC2YUV1);
                vRegWriteFldAlign(SCPIP_PIP1_0B+pipOffset, dispPrm->u4ReadInitP, PIP1_0B_READ_INITP_1);
                vRegWriteFldAlign(SCPIP_PIP1_06+pipOffset, (u4BaseAddress[u4VdpId]>>4)+1, PIP1_06_RSTARTP_1);
            }
#ifdef CC_SCPOS_WA28// for 5N+1 work around
            if(bScpipMirrorEnable[u4VdpId] && bReadIs444 && (_pDispPrm[u4VdpId]->u4USInputWidth != 0) && (((_pDispPrm[u4VdpId]->u4USInputWidth-1)%5) == 0)) //(444 8bit) && (width is 5N+1)
            {
                au4AddDelay[u4VdpId] = DFT_US_FACTOR / dispPrm->u4USclH;
            }
            else
            {
                au4AddDelay[u4VdpId] = 0;
            }
            
            // main work around
            vRegWriteFldAlign(SCPIP_PIP_OA_02, MAIN_ACTIVE_ADJ+au4AddDelay[VDP_1], PIP_OA_02_HSYNC_ADJ);  
            vRegWriteFldAlign(SCPIP_PIP1_08, MAIN_ACTIVE_ADJ+au4AddDelay[VDP_1], PIP1_08_ACTIVE_ADJ_1); 
            vRegWriteFldAlign(SCPIP_PIP1_0F, _rMDispPrm.u4HBound+((au4AddDelay[VDP_1])?1:0),PIP1_0F_BOUNDARY_H_POS_1);
            // sub work around
            vRegWriteFldAlign(SCPIP_PIP2_08, SUB_ACTIVE_ADJ+au4AddDelay[VDP_1], PIP2_08_ACTIVE_ADJ_2); 
            vRegWriteFldAlign(SCPIP_PIP2_0F, _rPDispPrm.u4HBound+((au4AddDelay[VDP_2])?1:0),PIP2_0F_BOUNDARY_H_POS_2);
            vRegWriteFldAlign(SCPIP_PIP2_08, SUB_READ_DELAY+au4AddDelay[VDP_1]-au4AddDelay[VDP_2], PIP2_08_READ_DEALY_2); 
#endif
        }
    }
    /* DTV00209813, update display window even when no source is selected */
    /*
    else // Channel is not on, turn off Main display
    {
        //SET RVL TO 0
        vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, 0, PIP1_02_RVL_1);
    }
    */
#if SUPPORT_POP
    #if TVE_WA1
    {
        UINT32 offset=0;
        UINT8 ucEnable;
        UINT8 u1VdoIsInterlace = 0;

        if(bPath == SV_VP_PIP && (bScpipFlipEnable[bPath]==SV_FALSE) && (bScpipMirrorEnable[bPath]==SV_FALSE))
        {
            UINT32 u4VL;
            TVE_GetEnable(TVE_1, &ucEnable);
            u1VdoIsInterlace = bDrvVideoIsSrcInterlace(u4VdpId);
            if(ucEnable && u1VdoIsInterlace && ((_pPicInfo[u4VdpId]->wSrcH == 480) || (_pPicInfo[u4VdpId]->wSrcH == 576)))
            {
                offset = 33 * dispPrm->wHLen;
                 #ifdef CC_TVE_US_IMAGERESIZER_WA
                 if ((_u4ResizeSubDRAM) && (u4VdpId == VDP_2))
                 {
                     UINT32 u4Ofst;
                     u4Ofst = RegReadFldAlign(SCPIP_PIP1_07+pipOffset, PIP1_07_WINC_1);
                     u4Ofst = u4Ofst * 3;
                     u4Ofst += RegReadFldAlign(SCPIP_PIP1_05+pipOffset,PIP1_05_WSTARTP_1);
                     u4Ofst -=  offset;
                     vRegWriteFldAlign(SCPIP_PIP1_06+pipOffset, u4Ofst, PIP1_06_RSTARTP_1);
                     LOG(3, "CC_TVE_US_IMAGERESIZER_WA: Change R Addr to 0x%X\n", (u4Ofst<<4));
                 }
                 else
                 #endif
                 {
                    vRegWriteFldAlign(SCPIP_PIP1_06+pipOffset, RegReadFldAlign(SCPIP_PIP1_05+pipOffset,PIP1_05_WSTARTP_1) -offset , PIP1_06_RSTARTP_1);
                 }
                //vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->wVLen+33, PIP1_00_VL_1);
                u4VL = dispPrm->wVLen+33;
            }
            else
            {
                 #ifdef CC_TVE_US_IMAGERESIZER_WA
                 if ((_u4ResizeSubDRAM) && (u4VdpId == VDP_2))
                 {
                     UINT32 u4Ofst;
                     u4Ofst = RegReadFldAlign(SCPIP_PIP1_07+pipOffset, PIP1_07_WINC_1);
                     u4Ofst = u4Ofst * 3;
                     u4Ofst += RegReadFldAlign(SCPIP_PIP1_05+pipOffset,PIP1_05_WSTARTP_1);
                     vRegWriteFldAlign(SCPIP_PIP1_06+pipOffset, u4Ofst, PIP1_06_RSTARTP_1);
                     LOG(3, "CC_TVE_US_IMAGERESIZER_WA: Change R Addr to 0x%X\n", (u4Ofst<<4));
                 }
                 else
                 #endif
                 {
                     vRegWriteFldAlign(SCPIP_PIP1_06+pipOffset, RegReadFldAlign(SCPIP_PIP1_05+pipOffset,PIP1_05_WSTARTP_1), PIP1_06_RSTARTP_1);
                 }
                 //vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->wVLen, PIP1_00_VL_1);
                 u4VL = dispPrm->wVLen;
            }
            #if CC_SPEEDUP_IMPORT
            if (RegReadFldAlign(SCPIP_PIP1_00+pipOffset, PIP1_00_VL_1) != u4VL)
            {
                _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R|SCPIP_UPDATE_W);
            }
            #endif        
            vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, u4VL, PIP1_00_VL_1);
        }
    }
    #endif
    #endif
#if !SUPPORT_POP
#undef u4VdpId
    #endif
}


/**
* @brief Calculate dynamic scale parameters
* @param u4VdpId VDP_1/VDP_2
* @param u4SrcWidth
* @param u4SrcHeight
* @param u4SrcXOffset
* @param u4SrcYOffset
* @param u4OutWidth
* @param u4OutHeight
* @param u4OutXOffset
* @param u4OutYOffset
*/
static UINT32 _VDP_vCalDynamicScale(UINT32 u4VdpId,
                                       UINT32 u4SrcWidth, UINT32 u4SrcHeight,  UINT32 u4SrcXOffset, UINT32 u4SrcYOffset, 
                                       UINT32 u4OutWidth,  UINT32 u4OutHeight,  UINT32 u4OutXOffset,   UINT32 u4OutYOffset)
{
    
#ifdef CC_SCPOS_NO_DS_NONLINEAR
    if ((_arScalePrm[u4VdpId].u4DynamicScale == 0) ||
        (bIsScalerInput444((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP)) ||
        (_arScalePrm[u4VdpId].u4NonlinearEn == 1))
#else
    if ((_arScalePrm[u4VdpId].u4DynamicScale == 0) ||
        (bIsScalerInput444((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP)))
#endif  
    {
        // dead lock protection
        _arDynamicPrm[u4VdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;

        _arDynamicPrm[u4VdpId].u4LastFactorValid = 1;

        _arDynamicPrm[u4VdpId].u4LastSrcWidth = u4SrcWidth;
        _arDynamicPrm[u4VdpId].u4LastSrcHeight = u4SrcHeight;
        _arDynamicPrm[u4VdpId].u4LastSrcXOffset = u4SrcXOffset;
        _arDynamicPrm[u4VdpId].u4LastSrcYOffset = u4SrcYOffset;

        _arDynamicPrm[u4VdpId].u4LastOutWidth = u4OutWidth;
        _arDynamicPrm[u4VdpId].u4LastOutHeight = u4OutHeight;
        _arDynamicPrm[u4VdpId].u4LastOutXOffset = u4OutXOffset;
        _arDynamicPrm[u4VdpId].u4LastOutYOffset = u4OutYOffset;

        if ((_arDynamicPrm[u4VdpId].u4LastSrcWidth == 0) ||
            (_arDynamicPrm[u4VdpId].u4LastSrcHeight == 0) ||
            (_arDynamicPrm[u4VdpId].u4LastOutWidth == 0) ||
            (_arDynamicPrm[u4VdpId].u4LastOutHeight == 0))
        {
            _arDynamicPrm[u4VdpId].u4LastFactorValid = 0;

            LOG(3, "Last Invalid\n");
        }    
    }

    else
    {
        UINT32 u4UseCurrentValue;
    
        // Handle Dynamic Scaling
        u4UseCurrentValue = 0;
        if (_arDynamicPrm[u4VdpId].u4State == VDP_DYNAMICE_SCALE_IDLE)
        {
            if ((_arDynamicPrm[u4VdpId].u4TotalScaleNs > 1) &&
                (_arDynamicPrm[u4VdpId].u4LastFactorValid != 0))
            {
                INT32 i4Var;

                // Backup Original/Final Source/Output Region
                _arDynamicPrm[u4VdpId].u4OrgSrcWidth = u4SrcWidth;
                _arDynamicPrm[u4VdpId].u4OrgSrcHeight = u4SrcHeight;
                _arDynamicPrm[u4VdpId].u4OrgSrcXOffset = u4SrcXOffset;
                _arDynamicPrm[u4VdpId].u4OrgSrcYOffset = u4SrcYOffset;

                _arDynamicPrm[u4VdpId].u4OrgOutWidth = u4OutWidth;
                _arDynamicPrm[u4VdpId].u4OrgOutHeight = u4OutHeight;
                _arDynamicPrm[u4VdpId].u4OrgOutXOffset = u4OutXOffset;
                _arDynamicPrm[u4VdpId].u4OrgOutYOffset = u4OutYOffset;

                // Last Value Protection
                if ((_arDynamicPrm[u4VdpId].u4LastSrcWidth + _arDynamicPrm[u4VdpId].u4LastSrcXOffset) >
                    (_pPicInfo[u4VdpId]->wSrcW))
                {
                    LOG(3, "DS Protection-H (%d, %d)\n", _arDynamicPrm[u4VdpId].u4LastSrcXOffset, _arDynamicPrm[u4VdpId].u4LastSrcWidth);
                    _arDynamicPrm[u4VdpId].u4LastSrcWidth = u4SrcWidth;
                    _arDynamicPrm[u4VdpId].u4LastSrcXOffset = u4SrcXOffset;
                }

                if ((_arDynamicPrm[u4VdpId].u4LastSrcHeight + _arDynamicPrm[u4VdpId].u4LastSrcYOffset) >
                    (_pPicInfo[u4VdpId]->wSrcH))
                {
                    LOG(3, "DS Protection-V (%d, %d)\n", _arDynamicPrm[u4VdpId].u4LastSrcYOffset, _arDynamicPrm[u4VdpId].u4LastSrcHeight);
                    _arDynamicPrm[u4VdpId].u4LastSrcHeight = u4SrcHeight;
                    _arDynamicPrm[u4VdpId].u4LastSrcYOffset = u4SrcYOffset;
                }

                // Calculate Source Region Step (New - Last)
                i4Var = (((INT32) u4SrcWidth) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcWidth));
                _arDynamicPrm[u4VdpId].i4SrcWidthStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4SrcHeight) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcHeight));
                _arDynamicPrm[u4VdpId].i4SrcHeightStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4SrcXOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcXOffset));
                _arDynamicPrm[u4VdpId].i4SrcXOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4SrcYOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcYOffset));
                _arDynamicPrm[u4VdpId].i4SrcYOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);


                // Calculate Output Region Step (New - Last)
                i4Var = (((INT32) u4OutWidth) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutWidth));
                _arDynamicPrm[u4VdpId].i4OutWidthStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4OutHeight) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutHeight));
                _arDynamicPrm[u4VdpId].i4OutHeightStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4OutXOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutXOffset));
                _arDynamicPrm[u4VdpId].i4OutXOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4OutYOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutYOffset));
                _arDynamicPrm[u4VdpId].i4OutYOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                LOG(3, "Step(%d, %d) S(%d %d %d %d) O(%d %d %d %d)\n",
                    _arDynamicPrm[u4VdpId].u4TotalScaleNs,
                    _arScalePrm[u4VdpId].u4ResponseTime,
                    _arDynamicPrm[u4VdpId].i4SrcXOffsetStep,
                    _arDynamicPrm[u4VdpId].i4SrcYOffsetStep,
                    _arDynamicPrm[u4VdpId].i4SrcWidthStep,
                    _arDynamicPrm[u4VdpId].i4SrcHeightStep,
                    _arDynamicPrm[u4VdpId].i4OutXOffsetStep,
                    _arDynamicPrm[u4VdpId].i4OutYOffsetStep,
                    _arDynamicPrm[u4VdpId].i4OutWidthStep,
                    _arDynamicPrm[u4VdpId].i4OutHeightStep);

                // Update Current Source/Output Region
                _arDynamicPrm[u4VdpId].u4CurSrcWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcWidth) + _arDynamicPrm[u4VdpId].i4SrcWidthStep);
                _arDynamicPrm[u4VdpId].u4CurSrcHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcHeight ) + _arDynamicPrm[u4VdpId].i4SrcHeightStep);
                _arDynamicPrm[u4VdpId].u4CurSrcXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcXOffset) + _arDynamicPrm[u4VdpId].i4SrcXOffsetStep);
                _arDynamicPrm[u4VdpId].u4CurSrcYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcYOffset) + _arDynamicPrm[u4VdpId].i4SrcYOffsetStep);

                _arDynamicPrm[u4VdpId].u4CurOutWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutWidth) + _arDynamicPrm[u4VdpId].i4OutWidthStep);
                _arDynamicPrm[u4VdpId].u4CurOutHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutHeight ) + _arDynamicPrm[u4VdpId].i4OutHeightStep);
                _arDynamicPrm[u4VdpId].u4CurOutXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutXOffset) + _arDynamicPrm[u4VdpId].i4OutXOffsetStep);
                _arDynamicPrm[u4VdpId].u4CurOutYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutYOffset) + _arDynamicPrm[u4VdpId].i4OutYOffsetStep);

                // Update Step
                _arDynamicPrm[u4VdpId].u4ScaleStep = 1;

                // Update State
                _arDynamicPrm[u4VdpId].u4State = VDP_DYNAMICE_SCALE_SCALING;

                // Reset Vsync Count
                _arDynamicPrm[u4VdpId].u4VsyncCount = 0;

                u4UseCurrentValue = 1;
            }

            // Update Last Source/Output Region
            _arDynamicPrm[u4VdpId].u4LastFactorValid = 1;

            _arDynamicPrm[u4VdpId].u4LastSrcWidth = u4SrcWidth;
            _arDynamicPrm[u4VdpId].u4LastSrcHeight = u4SrcHeight;
            _arDynamicPrm[u4VdpId].u4LastSrcXOffset = u4SrcXOffset;
            _arDynamicPrm[u4VdpId].u4LastSrcYOffset = u4SrcYOffset;

            _arDynamicPrm[u4VdpId].u4LastOutWidth = u4OutWidth;
            _arDynamicPrm[u4VdpId].u4LastOutHeight = u4OutHeight;
            _arDynamicPrm[u4VdpId].u4LastOutXOffset = u4OutXOffset;
            _arDynamicPrm[u4VdpId].u4LastOutYOffset = u4OutYOffset;

            if ((_arDynamicPrm[u4VdpId].u4LastSrcWidth == 0) ||
                (_arDynamicPrm[u4VdpId].u4LastSrcHeight == 0) ||
                (_arDynamicPrm[u4VdpId].u4LastOutWidth == 0) ||
                (_arDynamicPrm[u4VdpId].u4LastOutHeight == 0))
            {
                _arDynamicPrm[u4VdpId].u4LastFactorValid = 0;

                LOG(3, "Last Invalid\n");
            }            
        }
        else
        {
            if ((_arDynamicPrm[u4VdpId].u4OrgSrcWidth == u4SrcWidth) &&
                (_arDynamicPrm[u4VdpId].u4OrgSrcHeight == u4SrcHeight) &&
                (_arDynamicPrm[u4VdpId].u4OrgSrcXOffset == u4SrcXOffset) &&
                (_arDynamicPrm[u4VdpId].u4OrgSrcYOffset == u4SrcYOffset) &&
                (_arDynamicPrm[u4VdpId].u4OrgOutWidth == u4OutWidth) &&
                (_arDynamicPrm[u4VdpId].u4OrgOutHeight == u4OutHeight) &&
                (_arDynamicPrm[u4VdpId].u4OrgOutXOffset == u4OutXOffset) &&
                (_arDynamicPrm[u4VdpId].u4OrgOutYOffset == u4OutYOffset))
            {
                if (++_arDynamicPrm[u4VdpId].u4ScaleStep < _arDynamicPrm[u4VdpId].u4TotalScaleNs)
                {
                    // Update Current Source/Output Region
                    _arDynamicPrm[u4VdpId].u4CurSrcWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurSrcWidth) + _arDynamicPrm[u4VdpId].i4SrcWidthStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurSrcHeight ) + _arDynamicPrm[u4VdpId].i4SrcHeightStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurSrcXOffset) + _arDynamicPrm[u4VdpId].i4SrcXOffsetStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurSrcYOffset) + _arDynamicPrm[u4VdpId].i4SrcYOffsetStep);

                    _arDynamicPrm[u4VdpId].u4CurOutWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurOutWidth) + _arDynamicPrm[u4VdpId].i4OutWidthStep);
                    _arDynamicPrm[u4VdpId].u4CurOutHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurOutHeight ) + _arDynamicPrm[u4VdpId].i4OutHeightStep);
                    _arDynamicPrm[u4VdpId].u4CurOutXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurOutXOffset) + _arDynamicPrm[u4VdpId].i4OutXOffsetStep);
                    _arDynamicPrm[u4VdpId].u4CurOutYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurOutYOffset) + _arDynamicPrm[u4VdpId].i4OutYOffsetStep);
                
                    u4UseCurrentValue = 1;
                }
                else
                {
                    _arDynamicPrm[u4VdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;

                    // For 121 Mapping

                    // Set source region
                    _arDynamicPrm[u4VdpId].u4CurSrcWidth = u4SrcWidth;
                    _arDynamicPrm[u4VdpId].u4CurSrcHeight = u4SrcHeight;
                    _arDynamicPrm[u4VdpId].u4CurSrcXOffset = u4SrcXOffset;
                    _arDynamicPrm[u4VdpId].u4CurSrcYOffset = u4SrcYOffset;
                
                    // Change display region
                    _arDynamicPrm[u4VdpId].u4CurOutWidth = u4OutWidth;
                    _arDynamicPrm[u4VdpId].u4CurOutHeight = u4OutHeight;
                    _arDynamicPrm[u4VdpId].u4CurOutXOffset = u4OutXOffset;
                    _arDynamicPrm[u4VdpId].u4CurOutYOffset = u4OutYOffset;
                }
            }
            else
            {
                // Upper Layer ask for rescale during dynamic scaling                
                if (_arDynamicPrm[u4VdpId].u4TotalScaleNs > 1)
                {
                    INT32 i4Var;

                    // Last Region = Current Region
                    _arDynamicPrm[u4VdpId].u4LastSrcWidth = _arDynamicPrm[u4VdpId].u4CurSrcWidth;
                    _arDynamicPrm[u4VdpId].u4LastSrcHeight = _arDynamicPrm[u4VdpId].u4CurSrcHeight;
                    _arDynamicPrm[u4VdpId].u4LastSrcXOffset = _arDynamicPrm[u4VdpId].u4CurSrcXOffset;
                    _arDynamicPrm[u4VdpId].u4LastSrcYOffset = _arDynamicPrm[u4VdpId].u4CurSrcYOffset;

                    _arDynamicPrm[u4VdpId].u4LastOutWidth = _arDynamicPrm[u4VdpId].u4CurOutWidth;
                    _arDynamicPrm[u4VdpId].u4LastOutHeight = _arDynamicPrm[u4VdpId].u4CurOutHeight ;
                    _arDynamicPrm[u4VdpId].u4LastOutXOffset = _arDynamicPrm[u4VdpId].u4CurOutXOffset ;
                    _arDynamicPrm[u4VdpId].u4LastOutYOffset = _arDynamicPrm[u4VdpId].u4CurOutYOffset;

                    // Backup Original/Final Source/Output Region
                    _arDynamicPrm[u4VdpId].u4OrgSrcWidth = u4SrcWidth;
                    _arDynamicPrm[u4VdpId].u4OrgSrcHeight = u4SrcHeight;
                    _arDynamicPrm[u4VdpId].u4OrgSrcXOffset = u4SrcXOffset;
                    _arDynamicPrm[u4VdpId].u4OrgSrcYOffset = u4SrcYOffset;

                    _arDynamicPrm[u4VdpId].u4OrgOutWidth = u4OutWidth;
                    _arDynamicPrm[u4VdpId].u4OrgOutHeight = u4OutHeight;
                    _arDynamicPrm[u4VdpId].u4OrgOutXOffset = u4OutXOffset;
                    _arDynamicPrm[u4VdpId].u4OrgOutYOffset = u4OutYOffset;

                    // Last Value Protection
                    if ((_arDynamicPrm[u4VdpId].u4LastSrcWidth + _arDynamicPrm[u4VdpId].u4LastSrcXOffset) >
                        (_pPicInfo[u4VdpId]->wSrcW))
                    {
                        LOG(3, "DS Protection-H (%d, %d)\n", _arDynamicPrm[u4VdpId].u4LastSrcXOffset, _arDynamicPrm[u4VdpId].u4LastSrcWidth);
                        _arDynamicPrm[u4VdpId].u4LastSrcWidth = u4SrcWidth;
                        _arDynamicPrm[u4VdpId].u4LastSrcXOffset = u4SrcXOffset;
                    }

                    if ((_arDynamicPrm[u4VdpId].u4LastSrcHeight + _arDynamicPrm[u4VdpId].u4LastSrcYOffset) >
                        (_pPicInfo[u4VdpId]->wSrcH))
                    {
                        LOG(3, "DS Protection-V (%d, %d)\n", _arDynamicPrm[u4VdpId].u4LastSrcYOffset, _arDynamicPrm[u4VdpId].u4LastSrcHeight);
                        _arDynamicPrm[u4VdpId].u4LastSrcHeight = u4SrcHeight;
                        _arDynamicPrm[u4VdpId].u4LastSrcYOffset = u4SrcYOffset;
                    }

                    // Calculate Source Region Step (New - Last)
                    i4Var = (((INT32) u4SrcWidth) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcWidth));
                    _arDynamicPrm[u4VdpId].i4SrcWidthStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4SrcHeight) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcHeight));
                    _arDynamicPrm[u4VdpId].i4SrcHeightStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4SrcXOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcXOffset));
                    _arDynamicPrm[u4VdpId].i4SrcXOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4SrcYOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcYOffset));
                    _arDynamicPrm[u4VdpId].i4SrcYOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    // Calculate Output Region Step (New - Last)
                    i4Var = (((INT32) u4OutWidth) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutWidth));
                    _arDynamicPrm[u4VdpId].i4OutWidthStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4OutHeight) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutHeight));
                    _arDynamicPrm[u4VdpId].i4OutHeightStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4OutXOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutXOffset));
                    _arDynamicPrm[u4VdpId].i4OutXOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4OutYOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutYOffset));
                    _arDynamicPrm[u4VdpId].i4OutYOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    LOG(3, "Step(%d, %d) S(%d %d %d %d) O(%d %d %d %d)\n",
                        _arDynamicPrm[u4VdpId].u4TotalScaleNs,
                        _arScalePrm[u4VdpId].u4ResponseTime,
                        _arDynamicPrm[u4VdpId].i4SrcXOffsetStep,
                        _arDynamicPrm[u4VdpId].i4SrcYOffsetStep,
                        _arDynamicPrm[u4VdpId].i4SrcWidthStep,
                        _arDynamicPrm[u4VdpId].i4SrcHeightStep,
                        _arDynamicPrm[u4VdpId].i4OutXOffsetStep,
                        _arDynamicPrm[u4VdpId].i4OutYOffsetStep,
                        _arDynamicPrm[u4VdpId].i4OutWidthStep,
                        _arDynamicPrm[u4VdpId].i4OutHeightStep);

                    // Update Current Source/Output Region
                    _arDynamicPrm[u4VdpId].u4CurSrcWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcWidth) + _arDynamicPrm[u4VdpId].i4SrcWidthStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcHeight ) + _arDynamicPrm[u4VdpId].i4SrcHeightStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcXOffset) + _arDynamicPrm[u4VdpId].i4SrcXOffsetStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcYOffset) + _arDynamicPrm[u4VdpId].i4SrcYOffsetStep);

                    _arDynamicPrm[u4VdpId].u4CurOutWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutWidth) + _arDynamicPrm[u4VdpId].i4OutWidthStep);
                    _arDynamicPrm[u4VdpId].u4CurOutHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutHeight ) + _arDynamicPrm[u4VdpId].i4OutHeightStep);
                    _arDynamicPrm[u4VdpId].u4CurOutXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutXOffset) + _arDynamicPrm[u4VdpId].i4OutXOffsetStep);
                    _arDynamicPrm[u4VdpId].u4CurOutYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutYOffset) + _arDynamicPrm[u4VdpId].i4OutYOffsetStep);

                    // Update Step
                    _arDynamicPrm[u4VdpId].u4ScaleStep = 1;

                    // Update State
                    _arDynamicPrm[u4VdpId].u4State = VDP_DYNAMICE_SCALE_SCALING;

                    // Reset Vsync Count
                    _arDynamicPrm[u4VdpId].u4VsyncCount = 0;

                    u4UseCurrentValue = 1;
                }
            }
        }
        return u4UseCurrentValue;
    }
    return 0;
}


/**
* @brief Calculate nonlinear parameters
* @param u4VdpId VDP_1/VDP_2
* @param u4SrcWidth input width
* @param u4OutWidth target width
*/
#if 0//!OLD_NONLINEAE_US_ONOFF

static void _VDP_vCalNonLinear(UINT32 u4VdpId, UINT32 u4SrcWidth, UINT32 u4OutWidth)
{

            UINT32 u4NotSupported;
            UINT32 u4FactorA;
            UINT32 u4FactorB;
            UINT32 u4Input; 
            UINT32 u4Start; 
            UINT32 u4Var1;
            UINT32 u4Var2;

            UINT32 u4Width;
            UINT32 u4TargetW;

            u4NotSupported = 0;

            if ((_pDispPrm[u4VdpId]->u4DSclV != DFT_DS_FACTOR) ||
                (_pDispPrm[u4VdpId]->u4DSclH != DFT_DS_FACTOR))
            {
                u4NotSupported = 1;
            }

#if 1       // TODO, Nonlinear Full Test
            if ((_pPicInfo[u4VdpId]->wTargetW != 0) &&
                (_pPicInfo[u4VdpId]->wTargetH != 0))
            {
#if 0
                // Block square souce (Aspact Ratio < 1.20)
                u4FactorA = (120 * 0x10000) / 100;
                u4FactorB = (_pPicInfo[u4VdpId]->wTargetW * 0x10000) / _pPicInfo[u4VdpId]->wTargetH;

                if (u4FactorA > u4FactorB)
                {
                    // Work Around 
                    // too square, skip
                    u4NotSupported = 5;
                }
#endif
            }
            else
            {
                u4NotSupported = 6;
            }
#endif

            //// TODO, consider aspect ratio
            u4Width = u4OutWidth;

            // consider H-prescale down
            u4TargetW = (u4SrcWidth * _VDP_GetHorizontalPreScaleDownFactor(u4VdpId)) / DFT_DS_FACTOR;
            _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope = _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4Slope;
            _arScalePrm[u4VdpId].rNonlinearPrm.u4End = (_pDispPrm[u4VdpId]->u4USclV >> 4) * _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor/UINT_NONLINEAR_END_FACTOR;                
            _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint = (u4Width >> 3);
            
#ifdef CC_NONLINEAR_AUTO
            // auto mode, find min-slope in case1
            if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope == VDP_NONLINEAR_AUTO)
            {
                UINT32 u4AvgScaleFactor;
                UINT32 u4Temp;                    

                // Average Scaling Factor
                u4AvgScaleFactor = (u4TargetW << DFT_NONLINER_US_BIT_NUM) / u4Width;

                // S = 2A - E
                u4Start = (2 * u4AvgScaleFactor) - _arScalePrm[u4VdpId].rNonlinearPrm.u4End;

                // 2 * (E - S) > O ?
                u4Temp = 2 * (_arScalePrm[u4VdpId].rNonlinearPrm.u4End - u4Start);

                _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope = UNIT_NONLINEAR;
                if (u4Temp > u4Width)
                {
                    while (u4Temp > u4Width)
                    {
                        u4Temp >>= 1;
                        _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope++;
                    }
                }
                else
                {
                    while (u4Width < u4Temp)
                    {
                        u4Temp <<= 1;
                        _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope--;
                    }

                    _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope++;
                }

                LOG(3, "Auto Scope(%d)\n", _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope);
            }
#endif

            // YH Shih's Suggestion
            if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope > 14)
            {
                _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope = 14;
            }

            if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope > UNIT_NONLINEAR)
            {
                u4FactorA = 1 << (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope - UNIT_NONLINEAR);
                u4FactorB = 1;
            }
            else
            {
                u4FactorA = 1;
                u4FactorB = 1 << (UNIT_NONLINEAR - _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope);
            }

            // Hardware Error Term
            if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope >= 12)
            {
                u4FactorA <<= 8;
                u4FactorB <<= 8;

                if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope == 12)
                {
                    u4FactorA += 6;
                }
                else
                if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope == 13)
                {
                    u4FactorA += 25;
                }
                else
                if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope == 14)
                {
                    u4FactorA += 76;
                }
            }

            // scaling factor increase level in half output width
            u4Var1 = u4Width >> 1;
            u4Var1 *= u4FactorA;
            u4Var1 /= u4FactorB;

            // Min start value
            if (_arScalePrm[u4VdpId].rNonlinearPrm.u4End > u4Var1)
            {
                u4Start = _arScalePrm[u4VdpId].rNonlinearPrm.u4End - u4Var1;
            }
            else
            {
                u4Start = 0;
            }

            // average scaling factor
            u4Var2 = (u4Start + _arScalePrm[u4VdpId].rNonlinearPrm.u4End) >> 1;

            // Max input width
            u4Input = (u4Width * u4Var2) >> DFT_NONLINER_US_BIT_NUM;

            // Current input width > Max input width
            if (u4TargetW < u4Input)
            {
                // case 2
                
                u4Var2 = (u4TargetW << DFT_NONLINER_US_BIT_NUM) / u4Width;

                if (u4Var2 <= ((u4Var1>>1)+100))
                {			   
                    u4NotSupported = 6;
                }
                u4Var2 -= (u4Var1 >> 1);

                _arScalePrm[u4VdpId].rNonlinearPrm.u4Start = u4Var2;
				if(_arScalePrm[u4VdpId].rNonlinearPrm.u4Start>0x8000)
				{
					u4NotSupported = 7;
				}

                LOG(3, "[Case 2] Nonlinear(%d) (%d, %d, %d, %d)\n", 
                    u4VdpId,  
                    _arScalePrm[u4VdpId].rNonlinearPrm.u4Start,
                    _arScalePrm[u4VdpId].rNonlinearPrm.u4End,
                    _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope,
                    _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint);
            }
            else
            {
                // case 1

                if ((u4TargetW << DFT_NONLINER_US_BIT_NUM) > (u4Width * _arScalePrm[u4VdpId].rNonlinearPrm.u4End))
                {
                    u4NotSupported = 3;
                }
                
                u4Var2 = (u4Width * _arScalePrm[u4VdpId].rNonlinearPrm.u4End) - (u4TargetW << DFT_NONLINER_US_BIT_NUM);

                u4Var2 *= u4FactorA;
                u4Var2 /= u4FactorB;

                u4Var2 = _SquareRoot(u4Var2);

                if (u4Var2 > _arScalePrm[u4VdpId].rNonlinearPrm.u4End)
                {
                    u4NotSupported = 4;
                }

                _arScalePrm[u4VdpId].rNonlinearPrm.u4Start = _arScalePrm[u4VdpId].rNonlinearPrm.u4End - u4Var2;

                // handle unknown rounding
                _arScalePrm[u4VdpId].rNonlinearPrm.u4Start -= (_arScalePrm[u4VdpId].rNonlinearPrm.u4End - _arScalePrm[u4VdpId].rNonlinearPrm.u4Start) >> 7;
                if(_arScalePrm[u4VdpId].rNonlinearPrm.u4Start>0x8000)
				{
					u4NotSupported = 7;
				}
                LOG(3, "[Case 1] Nonlinear(%d) (%d, %d, %d, %d)\n", 
                    u4VdpId,  
                    _arScalePrm[u4VdpId].rNonlinearPrm.u4Start,
                    _arScalePrm[u4VdpId].rNonlinearPrm.u4End,
                    _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope,
                    _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint);
            }

            if (u4NotSupported ==0)
            {
                _arScalePrm[u4VdpId].u4NonlinearCtrl = 1;
            }
            else
            {
                LOG(3, "Nonlinear NOT Supported! (%d)\n", u4NotSupported);
            }
}
#endif

static void _vScpipCalDramLimit(UINT32 u4VdpId, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4OutWidth, UINT32 u4OutHeight, UINT32 *u4DramW, UINT32 *u4DramH)
{
    UINT32 u4WidthLimitation, u4HeightLimitation, u4DispWidth;//, u4DramWidth;
    
    u4HeightLimitation = wDISPLAY_HEIGHT;
    u4DispWidth = wDISPLAY_WIDTH/(u4VdpId+1);    
    u4WidthLimitation = u4DispWidth;
    
    if ((_arScalePrm[u4VdpId].u4DispMode == 0) && bSRMStable[u4VdpId])  // dram mode
    {
        // vertical limitation
        if(u4HeightLimitation >u4SrcHeight)
                u4HeightLimitation =u4SrcHeight;

        if(u4HeightLimitation >u4OutHeight)
                u4HeightLimitation =u4OutHeight;

        if(u4HeightLimitation> _arScalePrm[u4VdpId].u4DramHeight)
            u4HeightLimitation = _arScalePrm[u4VdpId].u4DramHeight;

        // horizontal limitation
        if(u4WidthLimitation > u4SrcWidth)
                u4WidthLimitation = u4SrcWidth;

        if(u4WidthLimitation > u4OutWidth)
                u4WidthLimitation = u4OutWidth;

        if(u4WidthLimitation> _arScalePrm[u4VdpId].u4DramWidth)
            u4WidthLimitation = _arScalePrm[u4VdpId].u4DramWidth;

        LOG(3, "Scaler Dram Limit Path(%d) W(%d) H(%d)\n", u4VdpId, u4WidthLimitation, u4HeightLimitation);

#if 0
        if(_au4OriFrameNum[u4VdpId] !=0 && u4HeightLimitation !=0)
        {
            UINT32 u4PackPixel;  // real pixel *3
            if(_arScalePrm[u4VdpId].u4Is422) 
            {
                if(_arScalePrm[u4VdpId].u410BitMode)
                    u4PackPixel = 18;//422 mode, 10bit, (6*3)
                else
                    u4PackPixel = 24;//422 mode, 8bit, (8*3)
            }
            else // 444 mode
            {
                if(_arScalePrm[u4VdpId].u410BitMode)
                    u4PackPixel = 12;//444 mode, 10bit,( 4*3)
                else //444 mode, 8bit
                {
                    if(bScpipMirrorEnable[u4VdpId]==SV_FALSE)
                        u4PackPixel = 16; // (16/3*3)
                    else
                        u4PackPixel = 15; // (5*3) 
                }
            }

            u4DramWidth = u4DramBufSize[u4VdpId] /_au4OriFrameNum[u4VdpId] /(u4HeightLimitation+CC_SCPOS_WA11);
            u4DramWidth = u4DramWidth*u4PackPixel/48-1;// (u4DramWidth*u4PackPixel/3/16)
            LOG(3, "Scaler Dram Limit Path(%d) W(%d) H(%d)\n", u4VdpId, u4DramWidth, u4HeightLimitation+CC_SCPOS_WA11);
        }
        else
            u4DramWidth = u4WidthLimitation;
        
        if(u4WidthLimitation >u4DramWidth)
        {
            u4WidthLimitation =u4DramWidth;
            Printf("SRM allocated buffer is too small!!, Please call SRM owner to check this.\n");
            ASSERT(0);
        }
            #endif
    }

    *u4DramW = u4WidthLimitation;
    *u4DramH = u4HeightLimitation;
}


/**
* @brief Calculate new scale parameters
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipCalNewScale(UINT32 u4VdpId) 
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    /* Get New US/DS Factors */
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4TargetW;  // input width after pre-down
    UINT32 u4TargetH;
    UINT32 u4WidthPlus;

    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4SrcXOffset;
    UINT32 u4SrcYOffset;

    UINT32 u4OutWidth;
    UINT32 u4OutHeight;
    UINT32 u4OutXOffset;
    UINT32 u4OutYOffset;

    UINT32 u4MddiFifoMin;
    UINT32 u4MddiFifoMax;
    UINT32 u4WidthLimitation;    
    UINT32 u4HeightLimitation;
    UINT32 u4FifoLength;
    RDispPrm rDispPrm;
    #if 0 //def CC_SCPOS_WA6
    UINT32 i4FrameNum;
    #endif

    UINT32 u4ScalerMode;

    u4ScalerMode = getScalerMode(getScalerPath(u4VdpId));    
                                          
    // Check NR/PSCAN fifo limit
    if (_arScalePrm[u4VdpId].u4MddiDramWidth != SCPOS_BYPASS_PSCANNR)
    {
        u4MddiFifoMin = u4DrvDIMiniInputWidth(u4VdpId);
        u4MddiFifoMax = u4DrvDIMaxInputWidth(u4VdpId);

        if (_arScalePrm[u4VdpId].u4MddiDramWidth < u4MddiFifoMax)
        {
            u4MddiFifoMax = _arScalePrm[u4VdpId].u4MddiDramWidth;
            LOG(3, "DI DRAM Limit (%d)\n", _arScalePrm[u4VdpId].u4MddiDramWidth);        
        }    
    }
    else
    {                
        u4MddiFifoMin = 0;
        u4MddiFifoMax = 2000;
    }      
    LOG(3, "DI Fifo(%d) range (%d - %d)\n", u4VdpId, u4MddiFifoMin, u4MddiFifoMax);

    // First Config #3
    if ((_arScalePrm[u4VdpId].u4SrcHeight != _pPicInfo[u4VdpId]->wSrcH) ||
        (_arScalePrm[u4VdpId].u4SrcWidth != _pPicInfo[u4VdpId]->wSrcW) ||
        (_arScalePrm[u4VdpId].u4MddiFifoMax != u4MddiFifoMax) ||
        (_arScalePrm[u4VdpId].u4MddiFifoMin != u4MddiFifoMin) ||
        (_pPicInfo[u4VdpId]->u1Interlace != bDrvVideoIsSrcInterlace(u4VdpId)))
    {
        _arScalePrm[u4VdpId].u4FirstConfig = 1;

        _arScalePrm[u4VdpId].u4SrcHeight = _pPicInfo[u4VdpId]->wSrcH;
        _arScalePrm[u4VdpId].u4SrcWidth = _pPicInfo[u4VdpId]->wSrcW;
        _arScalePrm[u4VdpId].u4MddiFifoMax = u4MddiFifoMax;
        _arScalePrm[u4VdpId].u4MddiFifoMin = u4MddiFifoMin;

        LOG(3, "1st Config#3\n");
    }

    u4SrcWidth = _pPicInfo[u4VdpId]->wTargetW;
    u4SrcHeight = _pPicInfo[u4VdpId]->wTargetH;
    #ifdef CC_TVE_UPSCALE_WA
    if ((_u4DoubleInputHeight) && (u4VdpId == VDP_2))
    {
        u4SrcHeight = u4SrcHeight << 1;
    }
    else
    #endif    
    #ifdef CC_TVE_US_IMAGERESIZER_WA
    if ((_u4ResizeSubDRAM) && (u4VdpId == VDP_2))
    {
        u4SrcHeight = _u4ResizeSubDRAM;
    }
    else
    #endif
    {
        // do nothing
    }
    #if NEW_5387_SCPOS_PORCH
    u4SrcXOffset = getMixedHPorch(u4VdpId);
    u4SrcYOffset = getMixedVPorch(u4VdpId);
    #else    
    u4SrcXOffset = _pPicInfo[u4VdpId]->wXOffset;
    u4SrcYOffset = _pPicInfo[u4VdpId]->wYOffset;
    #endif
    u4OutWidth = _pDispInfo[u4VdpId]->wRWidth;
    u4OutHeight = _pDispInfo[u4VdpId]->wRHeight;
    u4OutXOffset = _pDispInfo[u4VdpId]->wRXOffset;
    u4OutYOffset = _pDispInfo[u4VdpId]->wRYOffset;
    u4WidthPlus = 0;

    // window width or height is 0, no need to calculate hps factor
    if ((u4OutWidth == 0) || (u4OutHeight == 0)) return;
    
    // handle 121-mapping
    _arScalePrm[u4VdpId].u4121MapEn = 0;
    if ((_arScalePrm[u4VdpId].u4121Map) &&
        (_arScalePrm[u4VdpId].u4DispMode == 0))         // No 121Map at Display Mode
    {
        // Enable 121, If
        // 1) H/V Scaling Up
        // 2) MDDI FIFO > Source Width
        if ((u4OutWidth >= u4SrcWidth) && 
            (u4OutHeight >= u4SrcHeight) &&
            (u4MddiFifoMax >= _pPicInfo[u4VdpId]->wSrcW))
        {
            _arScalePrm[u4VdpId].u4121MapEn = 1;
        }
        
        // [DTV00112299] 720*400 needs to be dot by dot. Peggy 20080122
        else if (bDrvVideoGetWidthOverSample(u4VdpId) &&
            (u4OutWidth >= (u4SrcWidth >> 1)))
        {
            _arScalePrm[u4VdpId].u4121MapEn = 1;
        }
        
        else
        {
            LOG(3, "Skip 121 Mapping\n");
        }
    }
    
    if (_arScalePrm[u4VdpId].u4121MapEn)
    {
        UINT32 u4X;
        UINT32 u4Y;
        UINT32 u4W;
        UINT32 u4H;

        // Change display region
        if (bDrvVideoGetWidthOverSample(u4VdpId))
        {
            u4W = (u4SrcWidth >> 1) - u4WidthPlus;
            u4X = u4OutXOffset + ((u4OutWidth - (u4SrcWidth>>1)) >> 1);
        }
        else
        {
            u4W = u4SrcWidth - u4WidthPlus;
            u4X = u4OutXOffset + ((u4OutWidth - u4SrcWidth) >> 1);
        }
        u4H = u4SrcHeight;
        u4Y = u4OutYOffset + ((u4OutHeight - u4SrcHeight) >> 1);

        LOG(3, "VDP(%d) 121 Display Region (%d, %d, %d, %d)\n", u4VdpId, u4X, u4Y, u4W, u4H);

        u4OutWidth = u4W;
        u4OutHeight = u4H;
        u4OutXOffset = u4X;
        u4OutYOffset = u4Y;

        // do not update current source & output while doing dynamic scaling
        if ((_arScalePrm[u4VdpId].u4DynamicScale == 0) ||(_arDynamicPrm[u4VdpId].u4State == VDP_DYNAMICE_SCALE_IDLE))
        {
            // Set source region
            _arDynamicPrm[u4VdpId].u4CurSrcWidth = u4SrcWidth;
            _arDynamicPrm[u4VdpId].u4CurSrcHeight = u4SrcHeight;
            _arDynamicPrm[u4VdpId].u4CurSrcXOffset = u4SrcXOffset;
            _arDynamicPrm[u4VdpId].u4CurSrcYOffset = u4SrcYOffset;
        
            // Change display region
            _arDynamicPrm[u4VdpId].u4CurOutWidth = u4OutWidth;
            _arDynamicPrm[u4VdpId].u4CurOutHeight = u4OutHeight;
            _arDynamicPrm[u4VdpId].u4CurOutXOffset = u4OutXOffset;
            _arDynamicPrm[u4VdpId].u4CurOutYOffset = u4OutYOffset;
        }
    }


    if (_VDP_vCalDynamicScale(u4VdpId,u4SrcWidth,u4SrcHeight, u4SrcXOffset, u4SrcYOffset,u4OutWidth, u4OutHeight, u4OutXOffset, u4OutYOffset))
    {
        u4SrcWidth = _arDynamicPrm[u4VdpId].u4CurSrcWidth;
        u4SrcHeight = _arDynamicPrm[u4VdpId].u4CurSrcHeight;
        u4SrcXOffset = _arDynamicPrm[u4VdpId].u4CurSrcXOffset;
        u4SrcYOffset = _arDynamicPrm[u4VdpId].u4CurSrcYOffset;

        u4OutWidth = _arDynamicPrm[u4VdpId].u4CurOutWidth;
        u4OutHeight = _arDynamicPrm[u4VdpId].u4CurOutHeight;
        u4OutXOffset = _arDynamicPrm[u4VdpId].u4CurOutXOffset;
        u4OutYOffset = _arDynamicPrm[u4VdpId].u4CurOutYOffset;
    }

    // store original scaling parameters
    rDispPrm = *_pDispPrm[u4VdpId];

    u4Width = u4OutWidth;
    u4Height = u4OutHeight;
    u4TargetW = u4SrcWidth;
    u4TargetH = u4SrcHeight;

    //Calculate dram width and height limit.
    _vScpipCalDramLimit(u4VdpId,  u4SrcWidth, u4SrcHeight, u4OutWidth, u4OutHeight, &u4WidthLimitation, &u4HeightLimitation);

    if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5387_64MB)
    {
        if ((u4VdpId == VDP_1) && (getScalerMode(u4VdpId) == VDP_SCPOS_DISPMODE_OFF))
        {
            if ((_pPicInfo[u4VdpId]->wSrcW == 1920) && (bDrvVideoIsSrcInterlace(u4VdpId) == 0))
            {
                 _VDP_SetVspTune(0, 360);
            }
            else
            {
                 _VDP_SetVspTune(0, 0);
            }
        }
    }
    
    // consider H-prescale down
    // Force to specific pre-scaling down factors for debugging
    _arScalePrm[u4VdpId].u4HpsFactor = DFT_DS_FACTOR;
    
    if (_arScalePrm[u4VdpId].u4ForcedPreScalingactor)
    {
         _arScalePrm[u4VdpId].u4HpsFactor = _arScalePrm[u4VdpId].u4ForcedPreScalingactor;
         u4TargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
         LOG(1, "Force PreScale to %d\n", _arScalePrm[u4VdpId].u4ForcedPreScalingactor);
    }
#ifdef CC_SUPPORT_PRESCALE_DOWN
    else if (((_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_YPBPR) || (_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_DVI)) && bDrvVideoGetWidthOverSample(u4VdpId))
    {
        // 1440 >> 720
        _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR >> 1);
        u4TargetW >>= 1;
    }
#endif

#ifdef CC_SCPOS_WA12 
    else if ((u4VdpId == VDP_1) && (_rMChannel.bDecType == (UINT8) SV_VD_DVI) 
        && (_pPicInfo[u4VdpId]->wSrcW == 1920) && ((SRM_GetTvMode() == SRM_TV_MODE_TYPE_NORMAL) || (SRM_GetTvMode() == SRM_TV_MODE_TYPE_PIP))
        && (wDISPLAY_WIDTH == 1366) && (wDISPLAY_HEIGHT == 768))
    {
       _arScalePrm[u4VdpId].u4HpsFactor = CC_SCPOS_WA12;
       u4TargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);

       LOG(3, "[CC_SCPOS_WA12] Fix pre-scaling down to %d\n", CC_SCPOS_WA12);
    }
#endif

#ifdef CC_SCPOS_WA16 
    else if (((_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_VGA) || (_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_DVI))
        && (bDrvVideoIsSrcInterlace(u4VdpId) == 0)
        && (SRM_GetTvMode() == SRM_TV_MODE_TYPE_NORMAL)) // NR will still use DRAM under progressive timing. The DRAM size is not enough under POP mode/Sub path.
    {
       _arScalePrm[u4VdpId].u4HpsFactor = DFT_DS_FACTOR;

       LOG(3, "[CC_SCPOS_WA15] Fix pre-scaling down to %d\n", DFT_DS_FACTOR);
    }
#endif

    else
    {
        UINT32 u4NewTargetW;
        u4NewTargetW = u4TargetW;

        if (u4Width >= u4TargetW)
        {
            // Up Scale, Compare Picture Size to FIFO Size

            if (_pPicInfo[u4VdpId]->wSrcW > u4MddiFifoMax)
            {
                _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMax)/ _pPicInfo[u4VdpId]->wSrcW;
                LOG(1, "Two Scaling! (HPSDOWN + UP) \n");

#ifdef CC_SCPOS_MIN_PRESCALER
                if (_arScalePrm[u4VdpId].u4HpsFactor < CC_SCPOS_MIN_PRESCALER)
                {
                    // Abnormal, Impossible
                    // This will happen when changing pattern from CROMA2229
                    //ASSERT(0);
                    
                    _arScalePrm[u4VdpId].u4HpsFactor = CC_SCPOS_MIN_PRESCALER;
                }
#endif
                // Calculate Source Width After PreScale Down
                u4NewTargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
            }
        }
        else
        {
            // Down Scale
            UINT32 u4PictureSize;

            // Picture width After Down Scale
            u4PictureSize = DIVIDE_WITH_CARRIER((_pPicInfo[u4VdpId]->wSrcW * u4Width), u4TargetW);

            if (u4MddiFifoMax >= u4PictureSize)
            {
                // Do PreScale Down First       
                if ((u4PictureSize < u4MddiFifoMin) && (u4MddiFifoMin > 0))
                {                                
                    LOG(1, "Two Scaling! (DOWN + DOWN) MIN#0\n"); 

                    if (u4MddiFifoMin >= u4TargetW)
                    {
                    
                        LOG(1, "Warnning: Input width is already more than DI min FIFO limitation! (%d, %d)\n", u4TargetW, u4MddiFifoMin); 
                        _arScalePrm[u4VdpId].u4HpsFactor = DFT_DS_FACTOR;
    
                        u4NewTargetW = u4TargetW;
                    }
                    else
                    {
                        _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMin)/ u4TargetW;
    
                        // Calculate Source Width After PreScale Down
                        u4NewTargetW = u4MddiFifoMin;
                    }

                }
                else
                {
                    _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4Width)/ u4TargetW;
                    // Calculate Source Width After PreScale Down
                    u4NewTargetW = u4Width;
                }

#ifdef CC_SCPOS_MIN_PRESCALER
                if (_arScalePrm[u4VdpId].u4HpsFactor < CC_SCPOS_MIN_PRESCALER)
                {
                    LOG(1, "Two Scaling! (DOWN + DOWN) MIN#1\n");
                
                    _arScalePrm[u4VdpId].u4HpsFactor = CC_SCPOS_MIN_PRESCALER;

                    // Calculate Source Width After PreScale Down
                    u4NewTargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
                }
#endif
            }
            else
            {
                _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMax)/ _pPicInfo[u4VdpId]->wSrcW;
                LOG(1, "Two Scaling! (DOWN + DOWN) \n");

#ifdef CC_SCPOS_MIN_PRESCALER
                if (_arScalePrm[u4VdpId].u4HpsFactor < CC_SCPOS_MIN_PRESCALER)
                {
                    LOG(1, "Two Scaling! (DOWN + DOWN) MIN#2\n");
                
                    _arScalePrm[u4VdpId].u4HpsFactor = CC_SCPOS_MIN_PRESCALER;
                }
#endif
                // Calculate Source Width After PreScale Down
                u4NewTargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
            }
        }

        if (_arScalePrm[u4VdpId].u4FixedPreScaler != 0)
        {
             // Optimized?
            UINT32 u4NewHpsFac;
            UINT32 u4HpsFactor = 0;
            if (u4VdpId == VDP_1)
            {
                u4HpsFactor = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
            }
            else
            {
#if SUPPORT_POP
                u4HpsFactor = RegReadFldAlign(SCPIP_HDS_01, SCPIP_HDS_PIP_SCALER);
#endif
            }

            u4NewHpsFac = _arScalePrm[u4VdpId].u4HpsFactor;
            #if CC_SCPOS_WA_REMOVE_FRACTION_PART
            if (u4HpsFactor < 0x1000)
            {         //remove fraction part
                u4NewHpsFac = u4NewHpsFac & 0xfff0;
            }
            #endif
            
            if (u4HpsFactor != u4NewHpsFac)            
            {
                if (_arScalePrm[u4VdpId].u4FirstConfig == 0)
                {

#if (!CC_SCPOS_NEVER_FIX_HPS)
                    UINT32 u4PictureSize;
                    UINT32 u4CanNotFixed=0;

                    if ((_arScalePrm[u4VdpId].u4HpsFactor >= (u4HpsFactor << 1)) ||
                        (_arScalePrm[u4VdpId].u4HpsFactor <= (u4HpsFactor >> 1)))
                    {
                        // 0.5X~2X
                        u4CanNotFixed = 1;

                        LOG(1, "Fixed PreScaler Fail! Big rescale (0x%x >> 0x%x)\n", u4HpsFactor, _arScalePrm[u4VdpId].u4HpsFactor);
                    }

                    // Picture Size After Down Scale
                    u4PictureSize = DIVIDE_WITH_CARRIER((_pPicInfo[u4VdpId]->wSrcW * u4HpsFactor), DFT_DS_FACTOR);
                   
                    if (u4PictureSize  > u4MddiFifoMax)
                    {
                        // Bigger than MDDI FIFO
                        u4CanNotFixed = 1;

                        LOG(1, "Fixed PreScaler Fail! Pic(%d) > MDDI_MAX(%d)\n", u4PictureSize, u4MddiFifoMax);
                    }
                    else if ((u4PictureSize < u4MddiFifoMin) && (u4MddiFifoMin > 0) && (u4MddiFifoMin < u4SrcWidth))
                    {
                        // smaller than MDDI FIFO min bound
                        u4CanNotFixed = 1;

                        LOG(1, "Fixed PreScaler Fail! Pic(%d) < MDDI_MIN(%d)\n", u4PictureSize, u4MddiFifoMin);
                    }

                    if (u4CanNotFixed == 0)
                    {
                        LOG(1, "PreScaler Not Optimized (0x%x >> 0x%x)\n", _arScalePrm[u4VdpId].u4HpsFactor, u4HpsFactor);
                                            
                        _arScalePrm[u4VdpId].u4HpsFactor  = u4HpsFactor;
                        _arScalePrm[u4VdpId].u4PreScalerNotOptimized = 1;

                        if (_arScalePrm[u4VdpId].u4HpsFactor  != DFT_DS_FACTOR)
                        {
                            // Calculate Source Width After Old PreScale Down
                            u4TargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
                        }                                
                    }
                    else //CanNotFixed==1
#endif//(!CC_SCPOS_NEVER_FIX_HPS)
                    {
                        // Force rescale due to some reason
                        _arScalePrm[u4VdpId].u4PreScalerNotOptimized = 0;
                    
                        u4TargetW = u4NewTargetW;

                        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PRE_DOWN, u4VdpId, 
                                          (VDP_SCPOS_RESCALE_TIME + VDP_MDDI_RESCALE_TIME),
                                          TRUE);
                        LOG(6, "MUTE_MODULE_SCPOS_PRE_DOWN (1)\n");
                    }
                }
                else
                {            
                    LOG(3, "Do 1st Config\n");

                    _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PRE_DOWN, u4VdpId, 
                                      (VDP_SCPOS_RESCALE_TIME + VDP_MDDI_RESCALE_TIME),
                                      TRUE);
                    LOG(6, "MUTE_MODULE_SCPOS_PRE_DOWN (2)\n");

                    _arScalePrm[u4VdpId].u4PreScalerNotOptimized = 0;
                    _arScalePrm[u4VdpId].u4FirstConfig = 0;
                
                    u4TargetW = u4NewTargetW;
                }
                
                //if Pre-scaling down factor changed, fire scaler mode change to enable scaler to recalculate parameter of pscan dispmode
                if(u4ScalerMode==VDP_SCPOS_PSCAN_DISPMODE)
                {
                    LOG(4,"HPS changed ==> fireSCPOSModeChange\n");
                    fireSCPOSModeChange(u4VdpId);
                }                
            }
            else
            {
                _arScalePrm[u4VdpId].u4PreScalerNotOptimized = 0;
            
                u4TargetW = u4NewTargetW;
            }
        }
        else
        {
            u4TargetW = u4NewTargetW;
        }
    }

  //  #ifdef CC_SCPOS_WA28
   //         _pDispPrm[u4VdpId]->u4USInputWidth = u4TargetW;
  //  #endif
    _VDP_vSetHPreScalingDownFactor(u4VdpId);
    
    u4FifoLength = (u4SCPOS_FIFO_LENGTH[u4VdpId][(bIsScalerInput444((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP))]);
    
    if (u4FifoLength > u4WidthLimitation)
    {
         u4FifoLength = u4WidthLimitation;
    }
	#ifdef CC_SCPOS_WA34
	if((bScpipMirrorEnable[u4VdpId]) && (!bIsScalerInput444(u4VdpId)))
	{

	    if(u4TargetW > u4Width)//down
		{
		    _pDispInfo[u4VdpId]->wRWidth -= (u4Width % 2);
		    u4Width = u4Width - (u4Width % 2);
		}
	    else if(u4TargetW < u4Width)//up
		{
		    IsDramWidthChange = (u4TargetW % 2) ? SV_TRUE : SV_FALSE;
		    u4TargetW = u4TargetW - (u4TargetW % 2);
			
		}
		else
		{
			if(u4Width % 2)
			{
			    IsDramWidthChange = SV_TRUE;
				_pDispInfo[u4VdpId]->wRWidth -= 1;
			    u4Width = u4Width - 1;
				 u4TargetW = u4TargetW - 1;
			}
			#if 0
		    IsDramWidthChange = (u4Width % 2) ? SV_TRUE : SV_FALSE;
			_pDispInfo[u4VdpId]->wRWidth -= (u4Width % 2);
		    u4Width = u4Width - (u4Width % 2);
			 u4TargetW = u4TargetW - (u4TargetW % 2);
			 #endif
			 
		}
	}
#endif
#ifdef CC_SCPOS_WA28
        _pDispPrm[u4VdpId]->u4USInputWidth = ( u4TargetW > u4Width ) ? u4Width : u4TargetW;
     // _pDispPrm[u4VdpId]->u4USInputWidth = u4Width;
#endif
    /* Horizontal Direction */
    _vScpipCalScaleFactor(u4VdpId, u4Width, u4TargetW, u4FifoLength, CAL_HDIRECT_FACTOR);
    /* Vertical Direction */
    _vScpipCalScaleFactor(u4VdpId, u4Height , u4TargetH , u4HeightLimitation, CAL_VDIRECT_FACTOR);    
#if 0

    _arScalePrm[u4VdpId].u4NonlinearCtrl = 0;

#ifdef CC_SCPOS_NO_DS_NONLINEAR
    if (_arScalePrm[u4VdpId].u4NonlinearEn != 0)
#else
    if ((_arScalePrm[u4VdpId].u4NonlinearEn != 0) &&
        ((_arScalePrm[u4VdpId].u4DynamicScale == 0) ||
        (_arDynamicPrm[u4VdpId].u4State == VDP_DYNAMICE_SCALE_IDLE)))
#endif        
    {
        if ((u4VdpId == VDP_1) &&
            (u4OutWidth != 0) &&
            (u4SrcWidth != 0))
        {
            if (_arScalePrm[u4VdpId].u4NonlinearAuto) 
            {
#if 0//!OLD_NONLINEAE_US_ONOFF
                _VDP_vCalNonLinear(u4VdpId, u4SrcWidth, (u4OutWidth + u4WidthPlus)); // For CC_SCPOS_WA1
#endif
            }
            else
            {
                _arScalePrm[u4VdpId].u4NonlinearCtrl = 1;
            }
        }
    }
#endif          
    #if 0 //def CC_SCPOS_WA6
    // For VGA two frame mode, there will be garbage when changing scaling factor
    i4FrameNum = u4ScpipGetBufNum(u4VdpId);
    #endif

    // store original scaling parameters
    if ((rDispPrm.u4DSclH != _pDispPrm[u4VdpId]->u4DSclH) || (rDispPrm.u4DSclV != _pDispPrm[u4VdpId]->u4DSclV) ||
        // [DTV00112610] Flash a bright line when change "ASPECT" at 1152*864@85Hz.
        // Up scaling factor changed => Need mute, Peggy20080110
        (rDispPrm.u4USclH != _pDispPrm[u4VdpId]->u4USclH) || (rDispPrm.u4USclV != _pDispPrm[u4VdpId]->u4USclV))
    {
        LOG(3, "Scaler factor changed!: HU(0x%x) VU(0x%x) HD(0x%x) VD(0x%x)\n",
            rDispPrm.u4USclH, rDispPrm.u4USclV,
            rDispPrm.u4DSclH, rDispPrm.u4DSclV);
        _au4ScalingFactorChanged[u4VdpId] = 1;        
        
        #if 0 //def CC_SCPOS_WA6
        //[DTV00019426] [5336/5337] A black frame flashes at MAIN VGA whenever signal is detected in SUB
        // Only mute when scaling factor changed, Peggy20071122
        if (i4FrameNum < 3) // frame number is less than 3
        {
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_WA, u4VdpId, VDP_SCPOS_RESCALE_TIME, TRUE);
            LOG(6, "MUTE_MODULE_SCPOS_WA: CC_SCPOS_WA6 (1): Mute ,2 Frame mode: Factor changed\n");
        }
        #endif

        //if scaling factor changed, inform scaler to re-enter scaler dispmode or pscan dispmode
        u4ScalerMode = getScalerMode(getScalerPath(u4VdpId));
        if(u4ScalerMode==VDP_SCPOS_PSCAN_DISPMODE ||
           u4ScalerMode==VDP_SCPOS_DISPMODE_AUTO||
           u4ScalerMode==VDP_SCPOS_DISPMODE_MANUAL)
        {
            LOG(4,"Scaling factor changed ==>fireSCPOSModeChange()\n");
            fireSCPOSModeChange(u4VdpId);
        }        
    }
    else
    {
        _au4ScalingFactorChanged[u4VdpId] = 0;            
    }
    
    if (_pChannel[u4VdpId]->bIsChannelOn != SV_OFF)
    {
        LOG(3, "SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
            u4VdpId,  
            u4OutXOffset,
            u4OutYOffset,
            u4OutWidth,
            u4OutHeight,
            u4SrcXOffset,
            u4SrcYOffset,
            u4SrcWidth,
            u4SrcHeight);

        LOG(3, "HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n",
            _pDispPrm[u4VdpId]->u4USclH, _pDispPrm[u4VdpId]->u4USclV,
            _arScalePrm[u4VdpId].u4HpsFactor, 
            _pDispPrm[u4VdpId]->u4DSclH, _pDispPrm[u4VdpId]->u4DSclV);
    }
#if !SUPPORT_POP
#undef u4VdpId
#endif
}


/**
* @brief Calculate up/down scale factor
* @param u4VdpId VDP_1/VDP_2
* @param u4Source input width/height
* @param u4Target output width/height
* @param u4LimitLength fifo limit length
* @param bDirection horizontal/vertical direcrion
*/
static void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 u4Source, UINT32 u4Target, UINT32 u4LimitLength, UINT8 bDirection) 
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    if ((u4Source == 0) || (u4Target == 0) || (_pChannel[u4VdpId]->bIsChannelOn == SV_OFF) || ((u4Target <=u4LimitLength) && (u4Source == u4Target)))
    {
        // no scaling
        if (bDirection == CAL_HDIRECT_FACTOR) // horizontal
        {
            _pDispPrm[u4VdpId]->u4USclH = DFT_US_FACTOR;
            _pDispPrm[u4VdpId]->u4DSclH = DFT_DS_FACTOR;
        }
        else // vertical
        {
            _pDispPrm[u4VdpId]->u4USclV = DFT_US_FACTOR;
            _pDispPrm[u4VdpId]->u4DSclV = DFT_DS_FACTOR;
        }
    }
    
    else if ((u4Source < u4Target) && (u4Source <= u4LimitLength))
    {
        // down scaling
        UINT32 u4Factor;
        
        if (bDirection == CAL_HDIRECT_FACTOR)
        {
            _pDispPrm[u4VdpId]->u4USclH = DFT_US_FACTOR;
        }
        else
        {
            _pDispPrm[u4VdpId]->u4USclV = DFT_US_FACTOR;
        }
        
        u4Factor = (u4Source << DFT_DS_BIT_NUM)/ u4Target;

        if (bDirection == CAL_HDIRECT_FACTOR)
        {
            _pDispPrm[u4VdpId]->u4DSclH =u4Factor;
            if (_pDispPrm[u4VdpId]->u4DSclH > DFT_DS_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4DSclH = DFT_DS_FACTOR;
            }
        }
        else
        {
            _pDispPrm[u4VdpId]->u4DSclV = u4Factor;
            if (_pDispPrm[u4VdpId]->u4DSclV > DFT_DS_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4DSclV = DFT_DS_FACTOR;
            }
        }
    }
    
    else   // up scaling or down+up scaling case
    {
         UINT32 u4Factor;
         UINT32 u4OutPutLength;

         if (u4Target > u4LimitLength)
         {
            u4Factor =(u4LimitLength<< DFT_DS_BIT_NUM)/ u4Target;

            if (bDirection == CAL_HDIRECT_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4DSclH = u4Factor;
                if (_pDispPrm[u4VdpId]->u4DSclH > DFT_DS_FACTOR)
                {
                    _pDispPrm[u4VdpId]->u4DSclH = DFT_DS_FACTOR;
                }
            }
            else
            {
                _pDispPrm[u4VdpId]->u4DSclV = u4Factor;
                if (_pDispPrm[u4VdpId]->u4DSclV > DFT_DS_FACTOR)
                {
                    _pDispPrm[u4VdpId]->u4DSclV = DFT_DS_FACTOR;
                }
            }

            u4OutPutLength = u4LimitLength;
            LOG(3, "SCPOS H-Scaling DOWN+UP: %d -> %d -> %d\n", u4Target, u4LimitLength, u4Source); 
         }
         else
         {
             if (bDirection == CAL_HDIRECT_FACTOR)
             {
                 _pDispPrm[u4VdpId]->u4DSclH = DFT_DS_FACTOR;
             }
             else
             {
                 _pDispPrm[u4VdpId]->u4DSclV = DFT_DS_FACTOR;
             }
             u4OutPutLength= u4Target;
         }

         // up scaling
#ifndef CC_SCPIP_WA_FLIP_2ND_LINE
         if (bDirection == CAL_HDIRECT_FACTOR)
#endif
         {        
            // enlarge H outptu length according to up-scaling ratio to prevent right most line thick
            if(u4OutPutLength!=0)
            {
                u4Source += ((u4Source/u4OutPutLength)-1);
            }
         }

         u4Factor =(u4OutPutLength<< DFT_DS_BIT_NUM)/u4Source;

         if (bDirection == CAL_HDIRECT_FACTOR)
         {
            _pDispPrm[u4VdpId]->u4USclH = u4Factor;
            if (_pDispPrm[u4VdpId]->u4USclH > DFT_US_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4USclH = DFT_US_FACTOR;
            }
         }
         else
         {
             _pDispPrm[u4VdpId]->u4USclV = u4Factor;
             if (_pDispPrm[u4VdpId]->u4USclV > DFT_US_FACTOR)
             {
                 _pDispPrm[u4VdpId]->u4USclV = DFT_US_FACTOR;
             }
         }
    }
#if !SUPPORT_POP
#undef u4VdpId
#endif
}

static UINT32 _u4ScpipCalReadInitP(UINT32 u4Width, UINT8 b10bit, UINT8 b444mode)
{
    UINT32 u4Pack;
    UINT32 u4InitP=0;  // default is 0

    #if 0 //def CC_SCPOS_WA28
    bReadAddrAdj[u4VdpId]=0;
    #endif

    if(b444mode==SV_FALSE) 
    {
        u4Pack = (b10bit)?6:8;

        if(u4Width%u4Pack!=0)
        {
            u4InitP = u4Pack -(u4Width%u4Pack);
        }
    }
    else // 444 8bit mode, 444 10bit not support
    {
        u4Pack =5;

        #if 0 //def CC_SCPOS_WA28
        if(u4Width%u4Pack==1)  //5N+1
        {
            u4InitP=0;            
            bReadAddrAdj[u4VdpId]=1;
        }
        #else            
        if(u4Width%u4Pack==1)  //5N+1
        {
            u4InitP=4;            
            //bReadAddrAdj[u4VdpId]=1;
        }
        #endif
        else                
        {
            u4InitP = u4Pack -((u4Width-1)%u4Pack);
        }
    }
    return u4InitP;
}

/**
* @brief Update display paramters
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipUpdateDispPrm(UINT32 u4VdpId) 
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    UINT32 u4TargetW;
    UINT32 u4XOffset;

    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4SrcXOffset;
    UINT32 u4SrcYOffset;

    UINT32 u4OutWidth;
    UINT32 u4OutHeight;
    UINT32 u4OutXOffset;
    UINT32 u4OutYOffset;
    RDispPrm rDispPrm;
    #ifdef CC_SCPOS_WA6
    UINT32 i4FrameNum;
    #endif
    UINT8 bPath;
    UINT8 bMode;
    UINT32 u4WidthDS; // source width after down-scaling

    bPath = getScalerPath(u4VdpId);
    bMode = getScalerMode(bPath);  

    rDispPrm = *_pDispPrm[u4VdpId];
    #ifdef CC_SCPOS_WA6    
    // store original scaling parameters
    i4FrameNum = u4ScpipGetBufNum(u4VdpId);
    #endif

    if (((_arScalePrm[u4VdpId].u4DynamicScale != 0) &&
        (_arDynamicPrm[u4VdpId].u4State == VDP_DYNAMICE_SCALE_SCALING)) ||
        (_arScalePrm[u4VdpId].u4121MapEn != 0))        
    {
        u4SrcWidth = _arDynamicPrm[u4VdpId].u4CurSrcWidth;
        u4SrcHeight = _arDynamicPrm[u4VdpId].u4CurSrcHeight;
        u4SrcXOffset = _arDynamicPrm[u4VdpId].u4CurSrcXOffset;
        u4SrcYOffset = _arDynamicPrm[u4VdpId].u4CurSrcYOffset;

        u4OutWidth = _arDynamicPrm[u4VdpId].u4CurOutWidth;
        u4OutHeight = _arDynamicPrm[u4VdpId].u4CurOutHeight;
        u4OutXOffset = _arDynamicPrm[u4VdpId].u4CurOutXOffset;
        u4OutYOffset = _arDynamicPrm[u4VdpId].u4CurOutYOffset;
    }
    else
    {
        u4SrcWidth = _pPicInfo[u4VdpId]->wTargetW;
        u4SrcHeight = _pPicInfo[u4VdpId]->wTargetH;
        #ifdef CC_TVE_UPSCALE_WA
        if ((_u4DoubleInputHeight) && (u4VdpId == VDP_2))
        {
            u4SrcHeight = u4SrcHeight << 1;
        }
        else            
        #endif        
        #ifdef CC_TVE_US_IMAGERESIZER_WA
        if ((_u4ResizeSubDRAM) && (u4VdpId == VDP_2))
        {
            u4SrcHeight = _u4ResizeSubDRAM;
        }
        else
        #endif
        {
            // do nothing
        }

        #if NEW_5387_SCPOS_PORCH
        u4SrcXOffset = getMixedHPorch(u4VdpId);
        u4SrcYOffset = getMixedVPorch(u4VdpId);
        #else    
        u4SrcXOffset = _pPicInfo[u4VdpId]->wXOffset;
        u4SrcYOffset = _pPicInfo[u4VdpId]->wYOffset;
        #endif
        u4OutWidth = _pDispInfo[u4VdpId]->wRWidth;
        u4OutHeight = _pDispInfo[u4VdpId]->wRHeight;
        u4OutXOffset = _pDispInfo[u4VdpId]->wRXOffset;
        u4OutYOffset = _pDispInfo[u4VdpId]->wRYOffset;
    }

    // move _VDP_vPreAdjustWidth to here
#ifdef SUPPORT_SPECIAL_POP
    if (!_fgSpecialPOP)
#endif
    {
        _pDispInfo[u4VdpId]->wXOffset = 0;
        _pDispInfo[u4VdpId]->wYOffset = 0;
    }
    //1. consider H-prescale down
    u4TargetW = DIVIDE_WITH_CARRIER(u4SrcWidth * _VDP_GetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR);
    u4XOffset = (u4SrcXOffset * _VDP_GetHorizontalPreScaleDownFactor(u4VdpId)) / DFT_HPD_FACTOR;

    /* Horizontal Direction */

    //2. update H Porch
    // Horizontal display offset on the left side 
    // value 1 represent 1 pixels in 5360. 
    // It is used to compensate video front porch.
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
    {   
        //when scaler is in pscan dispmode, overscan was done in PSCAN
        _pDispPrm[u4VdpId]->wHPosOfst = _u4ScpipGetPscanDispmodeHPorch(bPath);
    }
    else
    {
        if (u4XOffset%2) // if hporch is odd, force it to add 1 to be even
        {
            u4XOffset = u4XOffset - 1;  
        }
        _pDispPrm[u4VdpId]->wHPosOfst = u4XOffset;
    }

    /* 3. update HP and HL and initial point
      422 mode: 16-bit Y/C mode  : value 1 represent 8 pixels.
                20-bit Y/C mode  : value 1 represent 6 pixels.
      444 mode: 24-bit R/G/B mode: value 1 represent 16/3 pixels.
    */
    u4WidthDS = RSHIFT_WITH_CARRIER((u4TargetW * _pDispPrm[u4VdpId]->u4DSclH), DFT_DS_BIT_NUM);
    #ifdef CC_SCPOS_WA34
	if(bScpipMirrorEnable[u4VdpId] && IsDramWidthChange)
	  {
		  u4WidthDS = u4WidthDS -1;
		  IsDramWidthChange = SV_FALSE;
	  }
	#endif
    #ifdef CC_SCPOS_WA29
    
    if(bMode == VDP_SCPOS_DISPMODE_OFF)
    {
        if(u4VdpId==VDP_1)
        {
            bForce444_10b[VDP_1] = (u4WidthDS<48*6)? SV_TRUE : SV_FALSE;
        }
#if SUPPORT_POP
        else //VDP_2
        {
            bForce444_10b[VDP_2] = (u4WidthDS<24*8)? SV_TRUE : SV_FALSE;
        }
#endif
    }
    else
    {
        bForce444_10b[u4VdpId] = SV_FALSE;
    }

    _arScalePrm[u4VdpId].u410BitMode = bPreColorDeth[u4VdpId];
    if(bForce444_10b[u4VdpId])
    {
        //mirror doesn't support 444 10bit mode, only 444 8bit
        _arScalePrm[u4VdpId].u410BitMode=(bScpipMirrorEnable[u4VdpId]) ?SV_FALSE : SV_TRUE;
    }
    #endif
	#ifdef CC_MIRROR_SW_WA
		{
        _pDispPrm[u4VdpId]->u4HBound = u4WidthDS;
        
        if(bScpipMirrorEnable[u4VdpId])
        {
            _pDispPrm[u4VdpId]->u4ReadInitP = _u4ScpipCalReadInitP(u4WidthDS, _arScalePrm[u4VdpId].u410BitMode, SV_TRUE);
        }

        if (_arScalePrm[u4VdpId].u410BitMode)
        {       // 10 bit mode
              _pDispPrm[u4VdpId]->wHPos = _pDispInfo[u4VdpId]->wXOffset >>2;         //  ignore rounding
              _pDispPrm[u4VdpId]->wHLen = RSHIFT_WITH_CARRIER(u4WidthDS, 2);
        }
        else
        {
            _pDispPrm[u4VdpId]->wHPos = (_pDispInfo[u4VdpId]->wXOffset * 3) >> 4;   //  ignore rounding
            
            if((bMode == VDP_SCPOS_DISPMODE_OFF)&&(bScpipMirrorEnable[u4VdpId]==SV_FALSE))  
            {   //128 bits pack
                _pDispPrm[u4VdpId]->wHLen = RSHIFT_WITH_CARRIER((u4WidthDS * 3), 4);
            }
            else
            {   // 120 bits pack
                  _pDispPrm[u4VdpId]->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 5);
            }
        }
    }
#else

    #ifdef CC_SCPOS_WA29
    if((!bForce444_10b[u4VdpId]) &&(bIsScalerInput444((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP) == SV_FALSE) )
    #else
    if (bIsScalerInput444((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP) == SV_FALSE)     // 422 Mode
    #endif
    {
            _pDispPrm[u4VdpId]->u4HBound = u4WidthDS+(u4WidthDS%2); // H bound need to be even
            #ifdef CC_SCPIP_WA_FLIP_2ND_LINE
            _pDispPrm[u4VdpId]->u4ColorFormat = 0; 
            #endif

          if(bScpipMirrorEnable[u4VdpId])
          {
              _pDispPrm[u4VdpId]->u4ReadInitP = _u4ScpipCalReadInitP(u4WidthDS, _arScalePrm[u4VdpId].u410BitMode,SV_FALSE);
          }
          
          if (_arScalePrm[u4VdpId].u410BitMode)
          {       // 10 bit mode
              _pDispPrm[u4VdpId]->wHPos = _pDispInfo[u4VdpId]->wXOffset / 6;         //  ignore rounding
              _pDispPrm[u4VdpId]->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 6);
          }
          else  // 8 bit mode
          {          
              _pDispPrm[u4VdpId]->wHPos = _pDispInfo[u4VdpId]->wXOffset >> 3;         //  ignore rounding
              if(bMode== VDP_SCPOS_DISPMODE_OFF)
              {//128 bits pack
                  _pDispPrm[u4VdpId]->wHLen = RSHIFT_WITH_CARRIER(u4WidthDS, 3);
              }
              else
              {// 120 bits pack
                    _pDispPrm[u4VdpId]->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 7);
              }
          }
    }
    else                                            // 444 Mode
    {
        _pDispPrm[u4VdpId]->u4HBound = u4WidthDS;
        #ifdef CC_SCPIP_WA_FLIP_2ND_LINE
        _pDispPrm[u4VdpId]->u4ColorFormat = 1;
        #endif
        
        if(bScpipMirrorEnable[u4VdpId])
        {
            _pDispPrm[u4VdpId]->u4ReadInitP = _u4ScpipCalReadInitP(u4WidthDS, _arScalePrm[u4VdpId].u410BitMode, SV_TRUE);
        }

        if (_arScalePrm[u4VdpId].u410BitMode)
        {       // 10 bit mode
              _pDispPrm[u4VdpId]->wHPos = _pDispInfo[u4VdpId]->wXOffset >>2;         //  ignore rounding
              _pDispPrm[u4VdpId]->wHLen = RSHIFT_WITH_CARRIER(u4WidthDS, 2);
        }
        else
        {
            _pDispPrm[u4VdpId]->wHPos = (_pDispInfo[u4VdpId]->wXOffset * 3) >> 4;   //  ignore rounding
            
            if((bMode == VDP_SCPOS_DISPMODE_OFF)&&(bScpipMirrorEnable[u4VdpId]==SV_FALSE))  
            {   //128 bits pack
                _pDispPrm[u4VdpId]->wHLen = RSHIFT_WITH_CARRIER((u4WidthDS * 3), 4);
            }
            else
            {   // 120 bits pack
                  _pDispPrm[u4VdpId]->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 5);
            }
        }
    }   
#endif
#ifdef SUPPORT_SPECIAL_POP
    if (_fgSpecialPOP)
    {
        if (bIsScalerInput444((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP) == SV_FALSE) // 422 Mode: 16-bit
        {
            if (_arScalePrm[u4VdpId].u410BitMode)
            {       // 10 bit mode
                 _pDispPrm[u4VdpId]->wDataLen = DIVIDE_WITH_CARRIER(u4OutWidth, 6);
            }
            else
            {       //8 bit mode
                _pDispPrm[u4VdpId]->wDataLen = RSHIFT_WITH_CARRIER(u4OutWidth, 3);
            }
        }
        else       // 444 Mode
        {
            _pDispPrm[u4VdpId]->wDataLen = RSHIFT_WITH_CARRIER((u4OutWidth * 3), 4);
        }

        LOG(3, "Special POP (%d, %d, %d, %d), R(%d, %d, %d, %d), DataLen (%d)\n", 
            _pDispInfo[u4VdpId]->wXOffset,
            _pDispInfo[u4VdpId]->wYOffset,
            u4OutWidth,
            u4OutHeight,
            u4OutXOffset,
            u4OutYOffset,
            u4OutWidth,
            u4OutHeight,
            _pDispPrm[u4VdpId]->wDataLen);
    }
    else
#endif
    {
        _pDispPrm[u4VdpId]->wDataLen = _pDispPrm[u4VdpId]->wHLen;

        #ifdef CC_SCPOS_WA3
        #if 0  // in 444 8 bit mode, last pixel will repeat previous pixel under specific condition
                // (Pixel mod 16) ==5 | (Pixel mod 16) ==9 | (Pixel mod 16) ==10
        if((u4VdpId == VDP_1) &&
           (_fgScpipFHDDotByDotTiming((UINT8)u4VdpId) == SV_TRUE))
        {
            _pDispPrm[u4VdpId]->wHLen += 1;
        }
        else
        #endif    
        {
            // When Mirror, HLen & Datalen should set as riginal value
            // When Flip, HLen +=1, DataLen+=2
            // Normal case, HLen +=1, DataLen+=1
            #if 0
            if(bScpipMirrorEnable[u4VdpId]==SV_FALSE)
            {
                _pDispPrm[u4VdpId]->wHLen += 1;
                #ifdef CC_SCPOS_WA26
                if(bScpipFlipEnable[u4VdpId]== SV_TRUE)
                {
                    _pDispPrm[u4VdpId]->wDataLen += 2;
                }
                else
                #endif
                {
                    _pDispPrm[u4VdpId]->wDataLen += 1;
                }
            }
            #else

            #ifdef CC_SCPOS_WA26
            if(((bScpipMirrorEnable[u4VdpId]==SV_FALSE) && (bScpipFlipEnable[u4VdpId]== SV_FALSE)) ||
                ((bScpipMirrorEnable[u4VdpId]==SV_TRUE) && (bScpipFlipEnable[u4VdpId]== SV_TRUE)))
            {  //normal, mirror+flip
                _pDispPrm[u4VdpId]->wHLen += 1;
                _pDispPrm[u4VdpId]->wDataLen += 1;
            }
            else if(bScpipMirrorEnable[u4VdpId]==SV_TRUE)  //mirror only
            {
                //_pDispPrm[u4VdpId]->wHLen += 1;
                _pDispPrm[u4VdpId]->wDataLen += 1;
            }
            else //if(bScpipFlipEnable[u4VdpId]== SV_TRUE)   // flip only
            {
                _pDispPrm[u4VdpId]->wHLen += 1;
                _pDispPrm[u4VdpId]->wDataLen += 2;
            }
            #else
            _pDispPrm[u4VdpId]->wHLen += 1;
            _pDispPrm[u4VdpId]->wDataLen += 1;
            #endif//CC_SCPOS_WA26
            
            #endif
        }
        #endif


        #ifdef CC_SCPOS_WA23
        //48 : main pre data number, 24 Sub pre data number
        //if(bScpipFlipEnable[u4VdpId]== SV_FALSE)
        if (IS_IC_5387() && (BSP_GetIcVersion() <= IC_VER_5387_AA) || 
           (IS_IC_5363() && (BSP_GetIcVersion() <= IC_VER_5363_AA)))  
        {
             if((_pDispPrm[u4VdpId]->wDataLen/2 +1)==( _pDispPrm[u4VdpId]->wDataLen-(48/(UINT16)(u4VdpId+1))))
             {
                if(_pDispPrm[u4VdpId]->wDataLen %2 ==0)
                     _pDispPrm[u4VdpId]->wDataLen +=1;
                else
                    _pDispPrm[u4VdpId]->wDataLen +=2;
             }
        }
        #endif
    }

    // [DTV00111581] mute when dataLen changed and frame number is smaller than 3, Peggy20071226
    if ((rDispPrm.wDataLen!= _pDispPrm[u4VdpId]->wDataLen))
    {
        #ifdef CC_SCPOS_WA6
        if (i4FrameNum < 3) // frame number is less than 3
        {
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_WA, u4VdpId, VDP_SCPOS_RESCALE_TIME, TRUE);
            LOG(6, "MUTE_MODULE_SCPOS_WA: CC_SCPOS_WA6 (2): Set Mute , 2 Frame mode: Factor changed. \n");
        }
        #endif
        #ifdef CC_SCPOS_WA25
        if (bDrvVideoGetRefreshRate(bPath) > vDrvGetLCDFreq())
        {
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_WA, u4VdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_IMPORT_PROTECT, TRUE);
            LOG(6, "MUTE_MODULE_SCPOS_WA :CC_SCPOS_WA25: Set Mute , VDP(%d)\n", u4VdpId);
        }
        #endif
    }

    /* 4. update RHP and RHL. Value 1 represent 1 pixel. */
    _pDispPrm[u4VdpId]->wRHPos = u4OutXOffset;
    _pDispPrm[u4VdpId]->wRHLen = u4OutWidth;

    //=================================================================
    /* Vertical Direction */

    //5. update V Porch
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        if (u1ScpipGetPSCANDispmodeScalingMode(bPath) == DOWN_SCALING_MODE)   //down-scaling
        {
            _pDispPrm[u4VdpId]->wVPosOfst = 0x14;
        }
        else
        {
            //when scaler is in pscan dispmode, overscan was done in PSCAN    
            _pDispPrm[u4VdpId]->wVPosOfst = 0;
        }
    }
    else
    {
        if(_pDispPrm[u4VdpId]->wVPosOfst !=u4SrcYOffset + _pPicInfo[u4VdpId]->wVPorch)
        {
            //u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_V_PORCH;
            //vSwitchFrameTrackOnOff(SV_OFF);
            //Printf("vSwitchFrameTrackOnOff: SCPIP_FRAME_TRACK_OFF_V_PORCH\n\n");
            _pDispPrm[u4VdpId]->wVPosOfst = u4SrcYOffset + _pPicInfo[u4VdpId]->wVPorch;
        }
    }
    
    /*6. update VP. Value 1 represent 1 line */
    _pDispPrm[u4VdpId]->wVPos = _pDispInfo[u4VdpId]->wYOffset;

    //7. update VL. Value 1 represent 1 line
    #ifdef CC_SCPOS_WA11
    _pDispPrm[u4VdpId]->wVLen = RSHIFT_WITH_CARRIER(((UINT32)(u4SrcHeight) * _pDispPrm[u4VdpId]->u4DSclV), DFT_DS_BIT_NUM);

    if(bScpipFlipEnable[u4VdpId]== SV_FALSE)
    {
        if(bMode==VDP_SCPOS_DISPMODE_OFF)
        {
            _pDispPrm[u4VdpId]->wVLen += CC_SCPOS_WA11;
        }
        else if(bMode==VDP_SCPOS_PSCAN_DISPMODE)
        {
            _pDispPrm[u4VdpId]->wVLen += vScpipGetDispmodeLineBufNum(u4VdpId);
        }
    }
    #else    
    // (u4SrcHeight+1) to fix rounding problem, Yulia
    _pDispPrm[u4VdpId]->wVLen = RSHIFT_WITH_CARRIER(((UINT32)(u4SrcHeight+1) * _pDispPrm[u4VdpId]->u4DSclV), DFT_DS_BIT_NUM);
    #endif
    
    _pDispPrm[u4VdpId]->wRVPos = u4OutYOffset;
    if(u4ForceDispOff[u4VdpId])// for TVE use, not to turn on sub window.
    {
        _pDispPrm[u4VdpId]->wRVLen =0;
    }
    else
    {
        _pDispPrm[u4VdpId]->wRVLen = u4OutHeight;
    }
    //8. determine UV inverse should on on/off
   // if(((_pDispPrm[u4VdpId]->wHPosOfst %2) !=0) && (bIsScalerInput444(u4VdpId)==SV_FALSE))
    if(bScpipMirrorEnable[u4VdpId] && (_pDispPrm[u4VdpId]->u4USInputWidth%2))  // mirror croma shift can apply only for even pixels (reg: PIP1_0F_MIRROR_OPT_YC2YUV1)
    {
        _pDispPrm[u4VdpId]->u1UVInv= SV_TRUE;
    }
    else
    {
        _pDispPrm[u4VdpId]->u1UVInv= SV_FALSE;
    }

#if 1//OLD_NONLINEAE_US_ONOFF
    vUpdateNonlinearUSPrm((UINT8)u4VdpId);
#endif
    /* For debug */
    _arScalePrm[u4VdpId].u4Width = RSHIFT_WITH_CARRIER((u4TargetW * _pDispPrm[u4VdpId]->u4DSclH), (DFT_DS_BIT_NUM + 3)) << 3;
    _arScalePrm[u4VdpId].u4Height = _pDispPrm[u4VdpId]->wVLen;
#if !SUPPORT_POP
#undef u4VdpId
#endif
}


/**
* @brief Calculate output vtotal
* @return Vtotal
*/
UINT16 wCalculateOutputVTotal(void) 
{
    UINT16 u2VOutputTotal;
    UINT32 u4InVTotal;

    //get input VTotal
    u4InVTotal = wDrvVideoGetVTotal(SV_VP_MAIN);    
    u2VOutputTotal = u4InVTotal;
    
    if(_pPicInfo[VDP_1]->wTargetH != 0)
    {
        u2VOutputTotal = DIVIDE_WITH_CARRIER(u4InVTotal * wDISPLAY_HEIGHT , _pPicInfo[VDP_1]->wTargetH);
    }
    return u2VOutputTotal;
}

/**
* @brief Switch to dispmode
* @param u4OnOff On/Off
*/
void vSwitchDispMode(UINT32 u4OnOff) 
{
    UINT8 bScalerMode;
    bScalerMode = getScalerMode(SV_VP_MAIN);

    if (u4OnOff == SV_ON)
    {
        vDrvScpipWriteCtrl(VDP_1,SV_OFF);

        /* Enable Display Mode */
        //vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_F_SYNC_1);
        vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_FMODESEL_1);
    }
    else
    {
        if (_arScalePrm[VDP_1].u4Enable)
        {
            CRIT_STATE_T csState = x_crit_start(); 
            // avoid other thread turn on write control before all related registers are set.          
            _u4ResetScposDramBuffer[VDP_1] = (_u4ResetScposDramBuffer[VDP_1] < 2)? 2: _u4ResetScposDramBuffer[VDP_1];
            x_crit_end(csState);
        }    
    
        /* Disable Display Mode */
#ifdef SUPPORT_SW_FRAME_CHECK
        vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
#endif
        
        if (bScalerMode == VDP_SCPOS_PSCAN_DISPMODE)
        {
            //If current mode is pscan dispmode, to prevent FMODESEL_1 from keeping toggling between 0 and 1.
            //Just keep it as original value.
            //vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_FMODESEL_1);               
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_FMODESEL_1);
         }

         // disable auto tune
         #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new AutoTune
         vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
         #else
         vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);       
         #endif
    }
}


/**
* @brief Search dispmode tune record
* @return table index
*/
UINT32 _VDP_SearchRecord(void)
{
    UINT32 index;

    for (index = 0; index < ADDR_STACK_SIZE_DISP; index ++)
    {
        if ((AddrStack[index].u4PanelW == wDISPLAY_WIDTH) &&
            (AddrStack[index].u4PanelH == wDISPLAY_HEIGHT) &&
            (AddrStack[index].u4SrcW == _rMPicInfo.wSrcW) &&
            (AddrStack[index].u4SrcH == _rMPicInfo.wSrcH) && 
            (AddrStack[index].u4FrameRate == bDrvVideoGetRefreshRate(SV_VP_MAIN)) &&
            (AddrStack[index].u4Interlace == bDrvVideoIsSrcInterlace(SV_VP_MAIN)) &&
            (AddrStack[index].u4USclVMin <= _rMDispPrm.u4USclV) && 
            (AddrStack[index].u4USclVMax >= _rMDispPrm.u4USclV) && 
            (AddrStack[index].u4DSclVMin <= _rMDispPrm.u4DSclV) && 
            (AddrStack[index].u4DSclVMax >= _rMDispPrm.u4DSclV))
        {
            return index;
        }
    }
    return ADDR_STACK_SIZE_DISP;
}


/**
* @brief _vScpipSetDispModeReg
*/
static void _vScpipSetDispModeReg(void) 
{
    if (_arScalePrm[VDP_1].u4DispMode == VDP_SCPOS_DISPMODE_AUTO)    // enable adative fine-tune mechanism in Display Mode
    {
        #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE //5387, use new auto tune
        UINT32 index;

        index = _VDP_SearchRecord();
        if (index != ADDR_STACK_SIZE_DISP) // find record
        {
            _VDP_SetVspTune(AddrStack[index].VSP, AddrStack[index].TUNE);
            _arScalePrm[VDP_1].u4DispMode = VDP_SCPOS_DISPMODE_MANUAL;
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_RESET_AUTO_TUNE_1);
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
        }        
        else
        {
            UINT32 u4TuneStep;
            if ( (wDISPLAY_WIDTH == 1440) && (wDISPLAY_HEIGHT == 900)
            && ((bDrvVideoGetTiming(SV_VP_MAIN) == MODE_1080i_50) || (bDrvVideoGetTiming(SV_VP_MAIN) == MODE_1080i)) )
            {
                 u4TuneStep = 0x03;        // for 538x 1080i display mode status error
            }
            else
            {
                 u4TuneStep = 0x08;
            }
    
            // set tune step
            vRegWriteFldAlign(SCPIP_PIP1_0A, u4TuneStep, PIP1_0A_TUNE_STEP_1);        
    
            // reset VSP and TUNE
            if ((wDISPLAY_WIDTH == 1440) && (wDISPLAY_HEIGHT == 900) &&
                (_rMPicInfo.wSrcW == 1440) && (_rMPicInfo.wSrcH == 900))
            {
                 // Auto tune workaround
                 _VDP_SetVspTune(1, 0);
            }
            else
            {
                 _VDP_SetVspTune(0, 0);
            }
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
            vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_RESET_AUTO_TUNE_1);
        }        
        #else
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);       
#ifdef SUPPORT_SW_FRAME_CHECK
        vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
#endif
        #endif
    }    
    else if (_arScalePrm[VDP_1].u4DispMode == VDP_SCPOS_DISPMODE_MANUAL)
    {
        UINT32 index;
        index = _VDP_SearchRecord();

        if (index != ADDR_STACK_SIZE_DISP) // find record
        {
            _VDP_SetVspTune(AddrStack[index].VSP, AddrStack[index].TUNE);
        }
        else
        {        
            UINT32 u4Vsp;
            UINT32 u4Tune;
        
            u4Vsp = 0;
            u4Tune = 0;
    
            // disable auto tune
            #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new auto tune
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
            #else
            vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);       
#ifdef SUPPORT_SW_FRAME_CHECK
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
#endif
            #endif
            
            if ((_rMDispPrm.u4USclH != DFT_US_FACTOR) || (_rMDispPrm.u4USclV != DFT_US_FACTOR) ||
                (_rMDispPrm.u4DSclH != DFT_DS_FACTOR) || (_rMDispPrm.u4DSclV != DFT_DS_FACTOR))
            {
                if ((_rMDispPrm.u4USclV == DFT_US_FACTOR) && (_rMDispPrm.u4DSclV == DFT_DS_FACTOR))
                {   
                    /* Vertical No Scaling: DISP_VSP = 2, DISP_TUNE = 0 */
                    u4Vsp = 2;
                    u4Tune = 0;                
                }
                else if (_rMDispPrm.u4DSclV != DFT_DS_FACTOR)
                {
                    /* Vertical Down Scaling: DISP_VSP = 0, DISP_TUNE = 0 */
                    u4Vsp = 0;
                    u4Tune = 0x10;             
                }                
                else
                if ((_rMDispPrm.u4USclV >= 1900) && (_rMDispPrm.u4USclV < DFT_US_FACTOR))
                { 
                    /* Vertical Up Scaling Factor 1900~2048: DISP_VSP = 2, DISP_TUNE = 0 */
                    u4Vsp = 2;
                    if ((_rMPicInfo.wTargetW == 1280) && (_rMPicInfo.wTargetH == 960) &&
                            (_rMDispInfo.wWidth == 1280) && (_rMDispInfo.wHeight == 1024))
                    {
                        u4Tune = 0x0a;             
                    }
                    else
                    {
                        u4Tune = 0;             
                    }
                }        
                else 
                if ((_rMDispPrm.u4USclV >= 1024) && (_rMDispPrm.u4USclV <= 1899))
                { 
                    /* Vertical Up Scaling Factor 1024~1899: DISP_VSP = 3, DISP_TUNE = 0 */
                    u4Vsp = 0x03;
                    u4Tune = 0;             
                }        
                else 
                if (_rMDispPrm.u4USclV < 1024)
                {
                    /* Vertical Up Scaling Factor < 1024: DISP_VSP = 3, DISP_TUNE = ?? Need to Fine Tune */
                    u4Vsp = 3;
                    if ((_rMPicInfo.wTargetW == 640) && (_rMPicInfo.wTargetH == 480) &&
                            (_rMDispInfo.wWidth == 1280) && (_rMDispInfo.wHeight == 1024))
                    {
                        u4Tune = 0x32;             
                    }
                }            
            }// end of scaling mode
            
            else
            {                       
                // No Scaling: DISP_VSP = 2, DISP_TUNE = 0 
                u4Vsp = 2;
                u4Tune = 0;                     
            }
            _VDP_SetVspTune(u4Vsp, u4Tune);
        }
    }
}


/**
* @brief Get dispmode status
* @param bPath VDP_1/VDP_2
* @return Dispmode type
*/
UINT8 _VDP_vGetDispModeOnOff(UINT8 bPath)
{
#if SUPPORT_POP
    return _arScalePrm[bPath].u4DispMode;
#else
    return _arScalePrm[0].u4DispMode;
#endif
}

UINT32 _VDP_GetMinSrcRegionWidth(UINT8 u4VdpId)
{
    UINT32 ret = 0;
    UINT16 inputWidth;
    inputWidth = wDrvVideoInputWidth(u4VdpId);
    
    if (bIsScalerInput444((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP) == SV_FALSE)     // 422 Mode
    {
        //if (_arScalePrm[u4VdpId].u410BitMode) // 10 bit mode
        //{       
            ret = ((CC_MINIMUN_DRAM_RW_UNIT*6)*VDP_MAX_REGION_WIDTH) /inputWidth;
        //}
        //else // 8 bit mode
        //{          
        //   ret = ((CC_MINIMUN_DRAM_RW_UNIT*8)*VDP_MAX_REGION_WIDTH) /inputWidth;
        //}
    }
    else // 444 Mode
    {
        ret = ((CC_MINIMUN_DRAM_RW_UNIT*16/3)*VDP_MAX_REGION_WIDTH) /inputWidth;
    }
    if(ret > VDP_MAX_REGION_WIDTH)//Not support zoom when input width < CC_MINIMUN_DRAM_RW_UNIT*6 or CC_MINIMUN_DRAM_RW_UNIT*16/3
	{
	    return VDP_MAX_REGION_WIDTH;
	}
    return ret;
}


/**
* @brief Force to turn Off Channel Write Control 
* @param u4VdpId VDP_1/VDP_2
* @return u1OnOff On/Off
*/
void _VDP_vDrvForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff) 
{
#if CC_SCPOS_FORCE_WOFF
    CRIT_STATE_T csState;
    
    if (u1OnOff)
    {
        LOG(9, "Enable SCPOS(%d) Force-write-control-off\n", u4VdpId);
    }
    else
    {
        LOG(9, "Cancel SCPOS(%d) Force-write-control-off\n", u4VdpId);
    }
    csState = x_crit_start();    
    _au4ForceWriteOff[u4VdpId] = u1OnOff;
    x_crit_end(csState);
#else
#ifndef CC_UP8032_ATV
    UNUSED(u4VdpId);
    UNUSED(u1OnOff);
#endif       
#endif       
}

static void vScpipSetDramWrite(UCHAR ucVdpId, UCHAR ucOnOff)
{
    vRegWriteFldAlign(SCPIP_PIP1_09, ucOnOff, PIP1_09_W_EN_1);
    LOG(9, "vScpipSetDramWrite(Path:%d, OnOff %d)\n", ucVdpId,ucOnOff );
}

static void vScpipSetDramRead(UCHAR ucVdpId, UCHAR ucOnOff)
{
    vRegWriteFldAlign(SCPIP_PIP1_09, ucOnOff, PIP1_09_R_EN_1);
    LOG(9, "vScpipSetDramRead(Path:%d, OnOff:%d)\n", ucVdpId,ucOnOff);
}

/**
* @brief Turn On/Off Main Channel Write Control 
* @param u1OnOff On/Off
*/
//void vDrvMainWriteCtrl(UCHAR ucVdpId, UCHAR ucOnOff)
void vDrvScpipWriteCtrl(UCHAR ucVdpId, UCHAR ucOnOff)
{
    CRIT_STATE_T csState;
    RVChannel *prChannel;
    UINT8 pipOffset;
    
#if SUPPORT_POP
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
#else
#define ucVdpId VDP_1
#endif    
    pipOffset = getPIPOffset(ucVdpId);
    if (RegReadFldAlign(SCPIP_PIP1_05+pipOffset, PIP1_05_WSTARTP_1) == 0)
    {
        return;
    }

    prChannel = getChannel(ucVdpId);

    csState = x_crit_start();    
    if (((ucOnOff == SV_ON) && 
#if CC_SCPOS_FORCE_WOFF
        (_au4ForceWriteOff[ucVdpId] == 0) &&
#endif       
        (prChannel->bIsChannelOn == SV_ON) && 
        (prChannel->bIsFreeze == SV_OFF) &&
        (_arScalePrm[ucVdpId].u4NotReady == 0) &&
        (_arScalePrm[ucVdpId].u4DispModeEn == 0) &&
        (_arScalePrm[ucVdpId].u4DispMode == 0))
        )
    {
        if (_u4ResetScposDramBuffer[ucVdpId] != 0)
        {
            _au4DelayWriteEnable[ucVdpId] = 1;
        }
        else
        {
            if(ucVdpId==VDP_1)
            {
                vRegWriteFldAlign(SCPIP_PIP_OA_06, 1, PIP_OA_06_RFIFO_PRO1);   
            }
            vScpipSetDramWrite(ucVdpId, SV_ON);
            vScpipSetDramRead(ucVdpId, SV_ON);
        }
    } 
    else
    {        
        _au4DelayWriteEnable[ucVdpId] = 0;
        vScpipSetDramWrite(ucVdpId, SV_OFF);
#ifndef __MODEL_slt__
        //if not in freeze mode, turn off read control
        if(prChannel->bIsFreeze == SV_OFF)
        {
            //if ((ucVdpId==VDP_1)|| 
            //    ((ucVdpId==VDP_2) &&(SRM_GetTvMode() == SRM_TV_MODE_TYPE_NORMAL)))
            //{  // Single View: do not read to turn on Read control
                vScpipSetDramRead(ucVdpId, SV_OFF);    
           // }
                
            if(ucVdpId==VDP_1)
            {
                vRegWriteFldAlign(SCPIP_PIP_OA_06, 0, PIP_OA_06_RFIFO_PRO1);   
            }
        }
#endif        
        if (_arScalePrm[ucVdpId].u4NotReady > 0)
        {
            _arScalePrm[ucVdpId].u4NotReady--;
        }
    }
    x_crit_end(csState);
#if !SUPPORT_POP
#undef ucVdpId
#endif
}


/**
* @brief Check write control status
* @param bPath VDP_1/VDP_2
*/
static void _vScpipWriteCTRLCheck(UINT8 bPath)
{
    RVChannel *prChannel;
    UINT8 bSigStatus;
    UINT8 bWriteOnOff= SV_OFF;
       
    bSigStatus = bDrvVideoSignalStatus(bPath);
    prChannel = getChannel(bPath);

    if (prChannel->bIsChannelOn == (UINT8)SV_ON)
    {
        #if SUPPORT_ATV_SNOWSCREEN
        if(fgIsSrcAtv(bPath))
        {
            bWriteOnOff = ((prChannel->bIsFreeze == SV_OFF)? SV_ON : SV_OFF);
        }
        else // non-atv input
        #endif//  #if SUPPORT_ATV_SNOWSCREEN
        {
            if ((bSigStatus == (UINT8)SV_VDO_STABLE) && (prChannel->bIsFreeze == (UINT8)SV_OFF))
            {
                bWriteOnOff = SV_ON;
            }
            else
            {
                bWriteOnOff = SV_OFF;
            }
        }
    }

    vDrvScpipWriteCtrl(bPath, bWriteOnOff);
}

/**
* @brief Set game mode
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
#if 0//!REMOVE_UNCALLED_FUNCTION
void _VDP_VideoSetGameMode(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_au4GameMode[ucVdpId] != ucOnOff)
    {
        LOG(3, "VDP(%d) Game Mode(%d)\n", ucVdpId, ucOnOff);
        _au4GameMode[ucVdpId] = ucOnOff;

        if (ucOnOff)
        {
            UINT32 u4FrameNum; 
    
            // force frame number to be less than 2.
            u4FrameNum =  u4ScpipGetBufNum((UINT32)ucVdpId);
            if (u4FrameNum > 2)
            {
                _vScpipSetBufNum((UINT32)ucVdpId, 2);
            }
        }
        else
        {
            _vScpipSetBufNum((UINT32)ucVdpId, _au4OriFrameNum[ucVdpId]);
        }
    }
}
#endif
#ifndef CC_COPLAT_MT82
/**
* @brief Get nonlinear customization table
* @param ucVdpId VDP_1/VDP_2
* @return NONLINEAR_PRM_T nonlinear customization table
*/
NONLINEAR_PRM_T _VDP_GetNonLinearTable(UCHAR ucVdpId) 
{
    UINT32 u4Idx, u4Size, u4Width;
    NONLINEAR_PRM_T* Nonlinear_Prm_Tbl_1;
    
    Nonlinear_Prm_Tbl_1 = DRVCUST_NonLinearTblGet();
    u4Size = DRVCUST_NonLinearTblSizeGet();
    u4Width = DIVIDE_WITH_CARRIER((wDrvVideoInputWidth(ucVdpId) * _arScalePrm[ucVdpId].u4HpsFactor), DFT_DS_FACTOR);

    if (Nonlinear_Prm_Tbl_1 == NULL)
    {
        NONLINEAR_PRM_T Nonlinear_Tbl;

        x_memset(&Nonlinear_Tbl, 0, sizeof(Nonlinear_Tbl));
        Nonlinear_Tbl.bAuto = TRUE;
        return Nonlinear_Tbl;
    }

    for (u4Idx = 1; u4Idx < u4Size; u4Idx++)
    {
        if ((Nonlinear_Prm_Tbl_1[u4Idx].bDecType == _pChannel[ucVdpId]->bDecType) &&
        (Nonlinear_Prm_Tbl_1[u4Idx].u4PanelW == wDISPLAY_WIDTH) && 
        (Nonlinear_Prm_Tbl_1[u4Idx].u4PanelH == wDISPLAY_HEIGHT) &&
        (Nonlinear_Prm_Tbl_1[u4Idx].u4Width == u4Width) &&
        (Nonlinear_Prm_Tbl_1[u4Idx].u4Height == wDrvVideoInputHeight(ucVdpId)) && 
        (Nonlinear_Prm_Tbl_1[u4Idx].u4Interlace == bDrvVideoIsSrcInterlace(ucVdpId)))
        {
            return Nonlinear_Prm_Tbl_1[u4Idx];
        }
    }
    return Nonlinear_Prm_Tbl_1[0];
}
#endif //CC_COPLAT_MT82

/**
* @brief Enable/Disable nonlinear up scaler
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void _VDP_VideoSetNonLinearUpScaler(UCHAR ucVdpId, UCHAR ucOnOff) 
{
    if (ucVdpId > SV_VP_MAIN)
        return;
    if (_arScalePrm[ucVdpId].u4NonlinearCtrl  != ucOnOff)
    {
        _arScalePrm[ucVdpId].u4NonlinearEn = ucOnOff;
        _arScalePrm[ucVdpId].u4NonlinearCtrl  = ucOnOff;
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    
#if 0
    UINT32 u4EndFactor = 0;
    UINT32 u4SideRatio = 0;
    UINT32 u4StopPoint = 0;
    UINT32 u4IncreaseFactor = 0;
    UINT32 u4StartFactor = 0;
    UINT32 Trigger = 0;

    UINT32 u4InputWidth = _pPicInfo[ucVdpId]->wTargetW;
    UINT32 u4InputMidWidth = 50;
    UINT32 u4OutputMidWidth = 70;
//  LogSD("in _VDP_VideoSetNonLinearUpScaler 11111111111",_arScalePrm[ucVdpId].u4NonlinearEn);
//   LogSD("in _VDP_VideoSetNonLinearUpScaler ucOnOff",ucOnOff);
    if (_arScalePrm[ucVdpId].u4NonlinearEn != ucOnOff) 
    {
        // 121-map & nonlinear scaling can NOT enable together
        if (ucOnOff)
        {
            _arScalePrm[ucVdpId].u4121Map = 0;
        }
        if ((PANEL_GetPanelWidth() == 0) || (u4InputWidth == u4InputMidWidth) || (u4InputMidWidth == 0))
        {
            return;
        }
        u4EndFactor = (u4InputMidWidth * 32768) / u4OutputMidWidth;
        u4SideRatio = ((u4InputWidth - u4InputMidWidth) * 32768) / (PANEL_GetPanelWidth() - u4OutputMidWidth);

        if (u4EndFactor <= u4SideRatio)
        {
            Printf("The upscaling factor of middle part larger than side part\n");
            return;
        }
        u4StopPoint = (PANEL_GetPanelWidth() - 50) / 2;
        u4IncreaseFactor = (PANEL_GetPanelWidth() * u4EndFactor - 32768 * u4InputWidth);
        u4IncreaseFactor = u4IncreaseFactor / (u4StopPoint * u4StopPoint);//a=(OE-32768I)/(stop_point.^2)
        u4StartFactor = u4EndFactor - u4IncreaseFactor * u4StopPoint;//S=E-a*stop_point
        u4EndFactor = (u4IncreaseFactor * u4StopPoint * u4StopPoint + 32768 * u4InputWidth) / PANEL_GetPanelWidth();
        u4StartFactor = u4EndFactor - u4IncreaseFactor * u4StopPoint;
        if ( u4StartFactor > 0x8000)
        {
            Printf("Start Factor more than 0x8000!\n");
            return;
        }
        
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4MiddleWidth = PANEL_GetPanelWidth() / 2;
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4StopPoint = u4StopPoint;
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4EndFactor = u4EndFactor;
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4IncreaseFactor = u4IncreaseFactor;
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4StartFactor = u4StartFactor;
        _arScalePrm[ucVdpId].u4NonlinearAuto = FALSE;
        _arScalePrm[ucVdpId].u4NonlinearEn = ucOnOff;
    }
#endif
        
}

#if !REMOVE_UNCALLED_FUNCTION
/**
* @brief Set auto nonlinear
* @param u4VdpId VDP_1/VDP_2
* @param rAutoNonlinearPrm auto nonlinear paramters
*/
void _VDP_SetAutoNonLinear(UINT32 u4VdpId, VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm) 
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    LOG(3, "Set Auto NonLinear (%d) (%d, %d, %d)\n", 
        u4VdpId,
        rAutoNonlinearPrm.u4Adaptive, 
        rAutoNonlinearPrm.u4EndFactor, 
        rAutoNonlinearPrm.u4Slope);

    if ((_arScalePrm[u4VdpId].rAutoNonlinearPrm.u4Slope != rAutoNonlinearPrm.u4Slope) ||
        (_arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor != rAutoNonlinearPrm.u4EndFactor) ||
        (_arScalePrm[u4VdpId].u4NonlinearAuto != TRUE))     
    {
        #ifdef CC_SCPOS_WA15
        UINT32 u4Width, u4Height;
        UINT32 u4Stop;
        #endif
        _arScalePrm[u4VdpId].rAutoNonlinearPrm = rAutoNonlinearPrm;
        _arScalePrm[u4VdpId].u4NonlinearAuto= TRUE;
        

        #ifdef CC_SCPOS_WA15 
        u4Width = wDrvVideoInputWidth(u4VdpId);
        u4Height = wDrvVideoInputHeight(u4VdpId);
        u4Stop = _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor;

        switch (u4Height)
        {
            case 480:
                u4RefWidth = 720;
                u4RefHeight = 480;
                break;
            case 576:
                u4RefWidth = 720;
                u4RefHeight = 576;
                break;
            case 720:
                u4RefWidth = 1280;
                u4RefHeight = 720;
                break;
            case 1080:
                u4RefWidth = 1920;
                u4RefHeight = 1080;
                break;
            default:
                u4RefWidth = 720;
                u4RefHeight = 480;
                break;
        }
        
        if (u4Width && u4Height)
        {
            _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor = (u4Stop * u4RefHeight * u4Width) / (u4RefWidth * u4Height);
        }
        #endif

        if (u4VdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
#if SUPPORT_POP
        else
        {
            vSetScposFlg(PIP_DISP_PRM_CHG);
        }
#endif
    }

    _arScalePrm[u4VdpId].u4NonlinearAuto= TRUE;
}

/**
* @brief Set manual nonlinear
* @param u4VdpId VDP_1/VDP_2
* @param rNonlinearPrm manual nonlinear paramters
*/
void _VDP_SetManualNonLinear(UINT32 u4VdpId, VDP_NONLINEAR_PRM_T rNonlinearPrm)
{
        if (u4VdpId >= VDP_NS)
        {
            return;
        }

        LOG(3, "Set Manual NonLinear(%d) (%d, %d, %d, %d)\n", 
                    u4VdpId,  
                    rNonlinearPrm.u4Start,
                    rNonlinearPrm.u4End,
                    rNonlinearPrm.u4Slope,
                    rNonlinearPrm.u4MiddlePoint);

        if ((_arScalePrm[u4VdpId].rNonlinearPrm.u4End != rNonlinearPrm.u4End) ||
            (_arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint != rNonlinearPrm.u4MiddlePoint) ||
            (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope != rNonlinearPrm.u4Slope) ||
            (_arScalePrm[u4VdpId].rNonlinearPrm.u4Start!= rNonlinearPrm.u4Start) ||
            (_arScalePrm[u4VdpId].u4NonlinearAuto))
        {
            _arScalePrm[u4VdpId].u4NonlinearAuto= FALSE;
            _arScalePrm[u4VdpId].rNonlinearPrm = rNonlinearPrm;

            if (u4VdpId == VDP_1)
            {
                vSetScposFlg(MAIN_DISP_PRM_CHG);
            }
#if SUPPORT_POP
            else
            {
                vSetScposFlg(PIP_DISP_PRM_CHG);
            }
#endif
        }

        _arScalePrm[u4VdpId].u4NonlinearAuto= FALSE;
}
#endif


/**
* @brief Set dot by dot mode
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void _VDP_VideoSet121Map(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4121Map != (UINT32) ucOnOff) 
    {
        // 121-map & nonlinear scaling can NOT enable together
        if (ucOnOff)
        {
            _arScalePrm[ucVdpId].u4NonlinearEn = 0;
        }
    
        _arScalePrm[ucVdpId].u4121Map = (UINT32) ucOnOff;

        if (ucVdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
        else
        {
            vSetScposFlg(PIP_DISP_PRM_CHG);
        }
    }
}

/**
* @brief Set dot by dot mode and bypass VDS/VUS
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void vDrvScpipForceBypass(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4ForceBypass != (UINT32) ucOnOff) 
    {
        _arScalePrm[ucVdpId].u4ForceBypass = (UINT32) ucOnOff;
        _VDP_VideoSet121Map(ucVdpId, ucOnOff);    

        if (ucVdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
        else
        {
            vSetScposFlg(PIP_DISP_PRM_CHG);
        }
    }
}

#if !REMOVE_UNCALLED_FUNCTION
/**
* @brief Fix pre-scaling down factor
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
* @param uc1stConfig configure paramter
*/
void vDrvVideoFixedPreScaler(UCHAR ucVdpId, UCHAR ucOnOff, UCHAR uc1stConfig)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4FixedPreScaler != (UINT32) ucOnOff) 
    {
        if (_arScalePrm[ucVdpId].u4FixedPreScaler != 0)
        {
            if (_arScalePrm[ucVdpId].u4PreScalerNotOptimized != 0)
            {
                // re-scale if NOT optimized!
                if (ucVdpId == VDP_1)
                {
                    vSetScposFlg(MAIN_DISP_PRM_CHG);
                }
                else
                {
                    vSetScposFlg(PIP_DISP_PRM_CHG);
                }                
            }
        }

        _arScalePrm[ucVdpId].u4FixedPreScaler = (UINT32) ucOnOff;
    }

    _arScalePrm[ucVdpId].u4FirstConfig = (UINT32) uc1stConfig;
}
#endif


/**
* @brief Set pre-scaling down factor
* @param u4VdpId VDP_1/VDP_2
* @param u4Factor pre-scaling down factor
*/
void _VDP_SetHorizontalPreScaleDownFactor(UINT32 u4VdpId, UINT32 u4Factor)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    _arScalePrm[u4VdpId].u4ForcedPreScalingactor = u4Factor;
    
    if (u4VdpId == VDP_1)
    {
           vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else
    {
          vSetScposFlg(PIP_DISP_PRM_CHG);
    }                

}


/**
* @brief Set 10/8 bit mode
* @param u4VdpId VDP_1/VDP_2
* @param u4Mode 10/8 bit
*/
void _VDP_Set10bitMode(UINT32 u4VdpId, UINT32 u4Mode)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    if (_u4Forced10BitMode[u4VdpId] != u4Mode)
    {
        _u4Forced10BitMode[u4VdpId] = u4Mode;
        if (u4VdpId == VDP_1)
        {
               vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
        else
        {
              vSetScposFlg(PIP_DISP_PRM_CHG);
        }                
    }

}


/**
* @brief Set dynamic scaler
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
* @param u4ResponseTime response time
*/
void _VDP_VideoSetDynamiceScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime)
{
    UINT32 u4OutputRate;
	
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4DynamicScale != (UINT32) ucOnOff)
    {
        // Protection, Stop dynamic Scaler while scaling
        if (_arDynamicPrm[ucVdpId].u4State != VDP_DYNAMICE_SCALE_IDLE)
        {
            if (ucVdpId == VDP_1)
            {
                vSetScposFlg(MAIN_DISP_PRM_CHG);
            }
            else
            {
                vSetScposFlg(PIP_DISP_PRM_CHG);
            }
        }
    
        _arDynamicPrm[ucVdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;
    }

    _arScalePrm[ucVdpId].u4DynamicScale = (UINT32) ucOnOff;
    _arScalePrm[ucVdpId].u4ResponseTime = u4ResponseTime;
    
    u4OutputRate = (UINT32)((vDrvGetLCDFreq() > 0) ? vDrvGetLCDFreq(): 60);
    _arDynamicPrm[ucVdpId].u4TotalScaleNs = 1 + DIVIDE_WITH_CARRIER((u4ResponseTime * u4OutputRate), 
        (1000 * VDP_SCPOS_RESCALE_TIME));

    if (_arScalePrm[ucVdpId].u4DynamicScale != 0)
    {
        if (_arScalePrm[ucVdpId].u4FixedPreScaler == 0)
        {
            LOG(1, "Warning: Dynamic Scaling but PreScale Not Fixed!\n");
        }
    }
}


/**
* @brief Enable/Disable scaler dispmode
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void _VDP_DispModeEnable(UCHAR ucVdpId, UCHAR ucOnOff)
{
#if SUPPORT_POP
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
#else
#define ucVdpId VDP_1
#endif
    
    #if CC_SCPOS_VSYNC_WA2
        if (LVDS_MUTE_IN_DISPLAY_MODE == LVDS_MUTE_IN_DISPLAY_MODE_ON)
        {
            // DE channel: need to check (SCPOS_LVDS_PD_2?)           
            if (ucOnOff && (RegReadFldAlign(OUTSTG_OS_15, OS_15_DEN_EN) == 0)
            && (_arScalePrm[ucVdpId].u4DispMode != ucOnOff))
            {
                PMX_SetDisplayMode(SV_TRUE);
                LOG(1, "PMX_SetDisplayMode(TRUE)\n");
              return;
            }
            else if (!ucOnOff)
            {
                PMX_SetDisplayMode(SV_FALSE);
                LOG(1, "PMX_SetDisplayMode(FALSE)\n");
            }
        }
    #endif

    LOG(4,"Set scaler mode by SRM. VDP(%d) mode(%d)\n",ucVdpId,ucOnOff);
    
    if (_arScalePrm[ucVdpId].u4DispMode != ucOnOff) 
    {
        LOG(4,"Scaler mode changed. VDP(%d) mode(%d->%d)\n",ucVdpId,_arScalePrm[ucVdpId].u4DispMode,ucOnOff);
        _arScalePrm[ucVdpId].u4DispMode = ucOnOff;

        if (ucVdpId == VDP_1)
        {
            //vSetScposFlg(MAIN_DISP_PRM_CHG);
            vSetScposFlg(SCALER_MAIN_MODE_CHG);
        }
#if SUPPORT_POP
        else
        {
            //vSetScposFlg(PIP_DISP_PRM_CHG);
            vSetScposFlg(SCALER_PIP_MODE_CHG);            
            if(ucOnOff == VDP_SCPOS_DISPMODE_AUTO || ucOnOff == VDP_SCPOS_DISPMODE_MANUAL)
            {
                //Hardware Limitation, sub path can't get into scaler dispmode
            ASSERT(0);
            }
        }
#endif
    }
#if !SUPPORT_POP
#undef ucVdpId
#endif
}


#ifdef CC_CLI
/**
* @brief Set vsp and tune
* @param u4Vsp
* @param u4Tune
*/
void _VDP_SetVspTune(UINT32 u4Vsp, UINT32 u4Tune)
{
    vRegWriteFldAlign(SCPIP_PIP1_0A, u4Vsp , PIP1_0A_FSYNC_VSP_1);
    vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);
}


/**
* @brief Set scaler dispmode related registers
* @param u4Auto Auto/Manual
* @param u4Step Tune step
* @param u4Print Log information
*/
void _VDP_DispModeTune(UINT32 u4Auto, UINT32 u4Step, UINT32 u4Print)
{
    UINT32 u4Vsp;
    UINT32 u4Tune;
    UINT32 u4Down;
    UINT32 u4SingleStep;

    u4Down = u4Step & 0x80000000;
    u4SingleStep = u4Step & 0x7fffffff;

    //1. get current tune and vsp value
    u4Tune = RegReadFldAlign(SCPIP_PIP1_0A,PIP1_0A_FSYNC_TUNE_1);
    u4Vsp = RegReadFldAlign(SCPIP_PIP1_0A,PIP1_0A_FSYNC_VSP_1);
    
    if (u4Print != 0)
    {
        UINT32 u4FifoOver1 = 0;
        UINT32 u4FifoOver2 = 0;
        UINT32 u4FifoOverLine1 = 0;
        UINT32 u4FifoOverLine2 = 0;
        UINT32 u4WAP = 0;

        //2. get overflow information
        u4FifoOver1 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_FIFO_OVER1_1);
        u4FifoOver2 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_FIFO_OVER2_1);       
        u4FifoOverLine1 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_OVERLINE1_1);
        u4FifoOverLine2 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_OVERLINE2_1);
        u4WAP = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_WAP_RA_1);
               
        LOG(1, "Vsp(%d) Tune(%d) O1(%d, %d) O2(%d, %d) WAP_RA(%d)\n", 
            u4Vsp, u4Tune, 
            u4FifoOver1, u4FifoOverLine1,
            u4FifoOver2, u4FifoOverLine2, u4WAP);

        UNUSED(u4FifoOver1);
        UNUSED(u4FifoOver2);
        UNUSED(u4FifoOverLine1);
        UNUSED(u4FifoOverLine2);
    }
    else if (u4Auto != 0)
    {
    }
    else
    {
        if (u4Down == 0)
        {
            if ((u4Tune + u4SingleStep) <= MAX_FSYNC_TUNE)
            {
                u4Tune += u4SingleStep;
                //set tune value
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);
            }
            else
            {
                u4Tune = 0;
                u4Vsp++;
                //set vsp and tune value
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Vsp , PIP1_0A_FSYNC_VSP_1);
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);                
            }
        }
        else
        {
            if (u4Tune >= u4SingleStep)
            {
                u4Tune -= u4SingleStep;
                //set tune value
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);                                
            }
            else
            if (u4Vsp > 0)
            {
                u4Tune = MAX_FSYNC_TUNE;
                u4Vsp--;
                //set vsp and tune value
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Vsp , PIP1_0A_FSYNC_VSP_1);
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);                
            }
        }
    }
}
#endif


/**
* @brief Set scpos delay enable time
* @param ucVdpId VDP_1/VDP_2
* @param ucEnable Enable/Disable
* @param u4DelayCount Delay time
*/
void _VDP_ScposDelayEnable(UCHAR ucVdpId, UCHAR ucEnable, UINT32 u4DelayCount)
{
#if SUPPORT_POP
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
#else
#define ucVdpId VDP_1
#endif
    LOG(3, "_VDP_ScposDelayEnable(%d) %d\n", ucVdpId, ucEnable);
    
    if (ucEnable)
    {
        // still keep mute, so enable memory read/write is fine
        #ifndef CC_SCPOS_WA5
        _VDP_ScposEnable(ucVdpId, TRUE);
        #endif
        _pDispInfo[ucVdpId]->u4VsyncDelay = VDP_VD_TURN_ENABLE + u4DelayCount;
    }
    else
    {
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PLA_EN, ucVdpId, FOREVER_MUTE, FALSE);
        _VDP_ScposEnable(ucVdpId, FALSE);
        _pDispInfo[ucVdpId]->u4VsyncDelay = VDP_VD_DISABLE;
    }
#if !SUPPORT_POP
#undef ucVdpId
#endif
}


/**
* @brief Check scpos delay enable
* @param ucVdpId VDP_1/VDP_2
*/
void _VDP_ScposChkDelayEnable(UCHAR ucVdpId)
{
#if SUPPORT_POP
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
#else
#define ucVdpId VDP_1
#endif
    if ((_pDispInfo[ucVdpId]->u4VsyncDelay == VDP_VD_DISABLE) ||
        (_pDispInfo[ucVdpId]->u4VsyncDelay == VDP_VD_ENABLE))
    {
        return;
    }
    if ((bDrvVideoSignalStatus(ucVdpId) == (UINT8) SV_VDO_STABLE)
    #if SUPPORT_ATV_SNOWSCREEN
    || (fgIsSrcAtv(ucVdpId))
    #endif
    )
    {
        // fgApiVideoIsEvtFlgSet will not be set when analog tv change channel!
        if (((ucVdpId == VDP_1) && (fgIsMainTvd3d() || fgApiVideoIsEvtFlgSet(VDO_FLG_MAIN_MODECHG_DONE))) 
            #if SUPPORT_POP
            ||((ucVdpId == VDP_2) && (fgIsPipTvd3d() || fgApiVideoIsEvtFlgSet(VDO_FLG_PIP_MODECHG_DONE)))
            #endif
            )
        {
        #ifdef CC_SCPOS_WA5
            if (_pDispInfo[ucVdpId]->u4VsyncDelay == VDP_VD_TURN_ENABLE + VDP_VD_DELAY_COUNT)
            {
                _VDP_ScposEnable(ucVdpId, TRUE);
            }        
        #endif
            // input source is ready, we could count down or unmute
            if (_pDispInfo[ucVdpId]->u4VsyncDelay > VDP_VD_TURN_ENABLE)
            {
                _pDispInfo[ucVdpId]->u4VsyncDelay--;
            }
            else if (_pDispInfo[ucVdpId]->u4VsyncDelay == VDP_VD_TURN_ENABLE)
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PLA_EN, ucVdpId, 0, FALSE);
                _pDispInfo[ucVdpId]->u4VsyncDelay = VDP_VD_ENABLE;

                #ifdef CHANNEL_CHANGE_LOG
                HAL_TIME_T dt, dt_press_key, dt_channel;
                static HAL_TIME_T dt_max = {0, 0}, dt_min = {1000, 1000}, dt_avg = {0, 0};
                static UINT32 u32_test_times = 0;
                
                HAL_GetTime(&dt);
                LOG(0, " %u.%06u s [AV SYNC] 9 SCPOS UNMUTE VDP(%d)\n", dt.u4Seconds, dt.u4Micros, ucVdpId);

                IRRX_GetPressTime(&dt_press_key);
                if(dt_press_key.u4Seconds > 0)
                {
                    HAL_GetDeltaTime(&dt_channel, &dt_press_key, &dt);
                    LOG(0, " Current Channel Change time: %u.%u\n", dt_channel.u4Seconds, dt_channel.u4Micros);                    
                    if(dt_channel.u4Seconds > dt_max.u4Seconds)
                    {
                        dt_max.u4Seconds = dt_channel.u4Seconds;
                        dt_max.u4Micros = dt_channel.u4Micros;
                    }
                    else if ((dt_channel.u4Seconds == dt_max.u4Seconds) && (dt_channel.u4Micros > dt_max.u4Micros))
                    {
                        dt_max.u4Micros = dt_channel.u4Micros;
                    }
                    if(dt_channel.u4Seconds < dt_min.u4Seconds)
                    {
                        dt_min.u4Seconds = dt_channel.u4Seconds;
                        dt_min.u4Micros = dt_channel.u4Micros;
                    }
                    else if ((dt_channel.u4Seconds == dt_min.u4Seconds) && (dt_channel.u4Micros < dt_min.u4Micros))
                    {
                        dt_min.u4Micros = dt_channel.u4Micros;
                    }
                    dt_avg.u4Seconds += dt_channel.u4Seconds;
                    dt_avg.u4Micros += dt_channel.u4Micros/1000;
                    u32_test_times++;
                    LOG(0, " Avg Channel Change time: %u.%u\n", dt_avg.u4Seconds/u32_test_times, dt_avg.u4Micros/u32_test_times);
                    LOG(0, " Max Channel Change time: %u.%u\n", dt_max.u4Seconds, dt_max.u4Micros);
                    LOG(0, " Min Channel Change time: %u.%u\n", dt_min.u4Seconds, dt_min.u4Micros);

                    IRRX_RstPressTime();
                }
                #endif
                
            }
        }
    }
#if !SUPPORT_POP
#undef ucVdpId
#endif
}

#if SUPPORT_POP
static void vScpipPowerCtrl(UINT8 bPath, UINT8 bOnOff)
{
    if(bPath ==SV_VP_PIP)
    {
        #ifdef CC_MT5363
        if(bOnOff==0)
        {
            vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, 0, FLD_SCPIP_PIP_CLK_EN);
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, 1, FLD_SCPIP_PIP_CLK_EN);
        }
        #else//5387
        if(bOnOff)
        {
                 _vDrvScpipConfigInputClk(bPath);
        }
        else
        {
            // use tve clk and power down tve clk
            vIO32WriteFldAlign(CKGEN_PSWCLK_CFG, 1, FLD_TVE54_PD);
            vDrvSetSCPIPClock(bPath, SCPIP_CLK_TYPE_TVE27_CK);
        }
        #endif
    }
}
#endif
/**
* @brief Set scpos enable/disable
* @param ucVdpId VDP_1/VDP_2
* @param ucEnable Enable/Disable
*/
void _VDP_ScposEnable(UCHAR ucVdpId, UCHAR ucEnable)
{
#if !SUPPORT_POP
#define ucVdpId VDP_1
#endif
    _arScalePrm[ucVdpId].u4Enable = ucEnable;    

    if (ucVdpId == VDP_1)
    {
        if (ucEnable)
        {
            UINT32 u4OutputRate;
            u4OutputRate = (UINT32)((vDrvGetLCDFreq() > 0) ? vDrvGetLCDFreq(): 60);
            _arDynamicPrm[ucVdpId].u4TotalScaleNs = 1 + DIVIDE_WITH_CARRIER((_arScalePrm[ucVdpId].u4ResponseTime * u4OutputRate), 
                (1000 * VDP_SCPOS_RESCALE_TIME));
        
           // vDrvScpipWriteCtrl(VDP_1, SV_ON); 
        }
        else
        {
            // Reset Dynamic Scaling State
            _arDynamicPrm[ucVdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;
            _arDynamicPrm[ucVdpId].u4LastFactorValid = 0;
                    
#ifdef CC_TV_MEMO_MODE
            if (!bTVMemoOnOff)
#endif
            {
                vDrvScpipWriteCtrl(VDP_1, SV_OFF);

                /* DTV00209813, update display window even when no source is selected */
                //set RVL to 0
                //vRegWriteFldAlign(SCPIP_PIP1_02,0,PIP1_02_RVL_1);

                // Disable Display Mode when video plane disable
                #if 0 // 5387
                if (_arScalePrm[VDP_1].u4DispModeEn != 0)
                {
                    LOG(1, "[DM] Off (vdp off)\n");

                    vSwitchDispMode(SV_OFF);
                    vDrvSetDynamicDisplay(SV_FAIL);
                    _VDP_ScposDispmodeFreeRun(SV_OFF);
                    _u4DispModeFreeRunCounter=0;
                    _arScalePrm[VDP_1].u4DispModeEn = 0;
                }
                #endif
            }
        }
    }
#if SUPPORT_POP
    else
    {
        if (ucEnable)
        {
            UINT32 u4OutputRate;
            u4OutputRate = (UINT32)((vDrvGetLCDFreq() > 0) ? vDrvGetLCDFreq(): 60);
            _arDynamicPrm[ucVdpId].u4TotalScaleNs = 1 + DIVIDE_WITH_CARRIER((_arScalePrm[ucVdpId].u4ResponseTime * u4OutputRate), 
                (1000 * VDP_SCPOS_RESCALE_TIME));
            vScpipPowerCtrl(SV_VP_PIP, SV_ON);
            //vDrvScpipWriteCtrl(SV_VP_PIP, SV_ON);
        }
        else
        {
            // Reset Dynamic Scaling State
            _arDynamicPrm[ucVdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;
            _arDynamicPrm[ucVdpId].u4LastFactorValid = 0;

#ifdef CC_TV_MEMO_MODE
            if (!bTVMemoOnOff)
#endif
            {
                vDrvScpipWriteCtrl(SV_VP_PIP, SV_OFF);
                vScpipPowerCtrl(SV_VP_PIP, SV_OFF);
                /* DTV00209813, update display window even when no source is selected */
                //set RVL to 0
                //vRegWriteFldAlign(SCPIP_PIP2_02, 0, PIP2_02_RVL_2);
            }
        }
    }
#endif    
#if !SUPPORT_POP
#undef ucVdpId
#endif
} 

/**
* @brief Get pre-scaling down factor
* @param ucVdpId VDP_1/VDP_2
* @return pre-scaling down factor
*/
UINT32 _VDP_GetHorizontalPreScaleDownFactor(UCHAR ucVdpId)
{
#if SUPPORT_POP
    return _arScalePrm[ucVdpId].u4HpsFactor;
#else
    return _arScalePrm[0].u4HpsFactor;
#endif
}

/**
* @brief Set frame delay
* @param u4VdpId VDP_1/VDP_2
* @return delay time
*/
UINT32 u4DrvGetScalarFrameDelay(UINT32 u4VdpId)
{
    UINT32 u4Delay;
    UINT32 u4RPos;
    UINT32 u4WPos;
    UINT8 bPath;
    UINT32 u4PipOffset;
    UINT32 u4FrameNum;
    
    bPath = getScalerPath(u4VdpId);
    u4PipOffset = getPIPOffset(bPath);
    
    if (u4VdpId >= VDP_NS)
    {
        return 0;
    }

    if (_arScalePrm[u4VdpId].u4DispModeEn)
    {
        u4Delay = 0;
    }
    else
    {
        u4FrameNum = RegReadFldAlign(SCPIP_PIP1_09+u4PipOffset, PIP1_09_FSEL_1);
        u4FrameNum += 1;
        
        u4WPos = RegReadFldAlign(SCPIP_PIP1_RO_00+u4PipOffset, PIP1_RO_00_W_POS_1);
        u4RPos = RegReadFldAlign(SCPIP_PIP1_RO_00+u4PipOffset, PIP1_RO_00_R_POS_1);

        if (u4RPos > u4WPos)
        {
            u4WPos += u4FrameNum;
        }

        u4Delay = u4WPos + 1 - u4RPos;    
    }

    return u4Delay;    
}

/**
* @brief Check if dram paramters change
* @param u4VdpId VDP_1/VDP_2
* @param u4Addr base address
* @param u4Size frame buffer size
* @param u4FrameNum frame buffer number
*/
static UINT8 _u1ScpipIsDRAMParamChanged(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4Size, UINT32 u4FrameNum)
{
    UINT8 u1Changed = 0;
    UINT32 u4OriFrameNum;
    UINT32 u4OriAdr;
    UINT32 u4OriSize;

    u4Addr >>= 4;
    u4Size >>= 4;
    u4FrameNum -= 1;

    #if !SUPPORT_POP
    if(u4VdpId == VDP_2)
    {
        return 0;
    }
    #endif
#if SUPPORT_POP
    //get frame number
    u4OriFrameNum = RegReadFldAlign(SCPIP_PIP1_09+getPIPOffset(u4VdpId), PIP1_09_FSEL_1);
    //get base address
    u4OriAdr = RegReadFldAlign(SCPIP_PIP1_05+getPIPOffset(u4VdpId), PIP1_05_WSTARTP_1);
    //get frame size
    u4OriSize = RegReadFldAlign(SCPIP_PIP1_07+getPIPOffset(u4VdpId), PIP1_07_WINC_1);
#else
    //get frame number
    u4OriFrameNum = RegReadFldAlign(SCPIP_PIP1_09, PIP1_09_FSEL_1);
    //get base address
    u4OriAdr = RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1);
    //get frame size
    u4OriSize = RegReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1);
#endif
    if (u4OriFrameNum != u4FrameNum)
    {
        u1Changed |= FRAME_NUM_CHG;
    }
    if (u4OriAdr != u4Addr)
    {
        u1Changed |= BASE_WADDRESS_CHG;
    }
    if (u4OriSize != u4Size)
    {
        u1Changed |= FRAME_SIZE_CHG;
    }
    if (u1Changed > 0)
    {
        LOG(3, "VDP(%d) DRAM parameters are changed! Original:(0x%X, %d, %d), New:(0x%X, %d, %d)\n", 
            u4VdpId, u4OriAdr<<4, u4OriSize<<4, u4OriFrameNum+1, u4Addr<<4, u4Size<<4, u4FrameNum+1);
    }

    return u1Changed;
}
    

/**
* @brief Set scpos status
* @param ucVdpId VDP_1/VDP_2
* @param u4NotReady
*/
void _VDP_ScposNotReady(UCHAR ucVdpId, UINT32 u4NotReady)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    _arScalePrm[ucVdpId].u4NotReady = u4NotReady;
}


/**
* @brief Enable/Disable scaler dispmode freerun mechanism
* @param bEnable On/Off
*/
void _VDP_ScposDispmodeFreeRun(BOOL bEnable)
{
#ifndef CC_UP8032_ATV
    vRegWriteFldAlign(SCPIP_PIP1_0B, bEnable, PIP1_0B_SP_PRO_1);
#else
    if(bEnable)
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_SP_PRO_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_SP_PRO_1);
    }
#endif
}


/**
* @brief Scaler dispmode freerun check
*/
static void _vScpipDispmodeFreeRunChk(void)
{
    if(((_u4DispModeFreeRunCounter == 1) || ( fgGetFrameTrackStatus()==SCPIP_FRAME_TRACK_LOCK))&&
        _u4DispModeFreeRunCounter!=0)
    {
        LOG(3, "_vScpipDispmodeFreeRunChk %d %d\n\n", _u4DispModeFreeRunCounter, fgGetFrameTrackStatus());
            
        _u4DispModeFreeRunCounter = 0;
        if (_arScalePrm[VDP_1].u4DispMode == VDP_SCPOS_DISPMODE_AUTO)    // enable adative fine-tune mechanism in Display Mode
        {
            #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new AutoTune
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_RESET_AUTO_TUNE_1);       
            vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_AUTO_TUNE_EN_1);
            #else
#ifdef SUPPORT_SW_FRAME_CHECK
            vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_F_SYNC_1);
#endif
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DISP_TUNE, SV_VP_MAIN, 10, FALSE);
            vRegWriteFldAlign(SCPIP_PIP_OA_08, 1, PIP_OA_08_DFIFO_AUTO_TUNE1);       
            #endif
        }
    }
    else if (_u4DispModeFreeRunCounter)
    {
          _u4DispModeFreeRunCounter--;
    }
}


/**
* @brief Check if scaler dispmode fifo overflow
* @return overflow status
*/
UINT32 _VDP_ScposDispModeFIFOOverflow(void)
{
    UINT32 u4DispActive;
    UINT32 u4FifoOver1 = 0;
    UINT32 u4FifoOver2 = 0;
    UINT32 u4FifoOverLine1 = 0;
    UINT32 u4FifoOverLine2 = 0;
    UINT32 u4WapRA;

    if (_rMDispPrm.u4USclV != DFT_US_FACTOR)        // US
    {
        u4DispActive = _rMPicInfo.wTargetH;
    }
    else                                                                            // DS
    {
        u4DispActive = _rMDispInfo.wHeight; //wDISPLAY_HEIGHT;
    }

    u4FifoOver1 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_FIFO_OVER1_1);
    u4FifoOver2 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_FIFO_OVER2_1);       
    u4FifoOverLine1 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_OVERLINE1_1);
    u4FifoOverLine2 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_OVERLINE2_1);
    u4WapRA = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_WAP_RA_1);
    
    if ((u4FifoOver1) && (u4FifoOverLine1 > _rMPicInfo.wYOffset) && (u4FifoOverLine1 < u4DispActive))
    {
        return 1;
    }
    else if ((u4FifoOver2) && (u4FifoOverLine2 > _rMPicInfo.wYOffset) && (u4FifoOverLine2 < u4DispActive))
    {
        return 2;
    }
    else
    {
        if(u4WapRA<=2)
        {
            return 0;
        }
        else
        {
            return 3;
        }
    }
 }


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#ifdef CC_CLI
/**
* @brief Print current scale paramters
* @param ucVdpId VDP_1/VDP_2
*/
void VDP_PrintCurrentScale(UCHAR ucVdpId)
{
    if (_pChannel[ucVdpId]->bIsChannelOn != SV_OFF)
    {
        //LOG(1, "SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
        Printf("SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
            ucVdpId,  
            _pDispInfo[ucVdpId]->wRXOffset,
            _pDispInfo[ucVdpId]->wRYOffset,
            _pDispInfo[ucVdpId]->wRWidth,
            _pDispInfo[ucVdpId]->wRHeight,
            #if NEW_5387_SCPOS_PORCH
            getMixedHPorch(ucVdpId),
            getMixedVPorch(ucVdpId),
            #else    
            _pPicInfo[ucVdpId]->wXOffset,
            _pPicInfo[ucVdpId]->wYOffset,
            #endif
            _pPicInfo[ucVdpId]->wTargetW,
            _pPicInfo[ucVdpId]->wTargetH);

        //LOG(1, "HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n\n",
        Printf("HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n\n",
            _pDispPrm[ucVdpId]->u4USclH, _pDispPrm[ucVdpId]->u4USclV,
            _arScalePrm[ucVdpId].u4HpsFactor, 
            _pDispPrm[ucVdpId]->u4DSclH, _pDispPrm[ucVdpId]->u4DSclV);
    }
}
#endif

#if (!NEW_5387_SCPOS_PORCH)    
typedef struct {
    INT16 Hdef,HOffset;
    INT16 Vdef,VOffset;
} rScposPorchState;

static rScposPorchState ScposPorch[2];
#endif

/**
* @brief Update scpos porch
* @param bPath VDP_1/VDP_2
* @param bEvent porch update type
*/
void  _VDP_ScposPorchUpdate(UINT8 bPath,UINT8 bEvent)
{
#if !SUPPORT_POP
#define bPath VDP_1
#define u4VdpId VDP_1
#endif
   #if !NEW_5387_SCPOS_PORCH
    UINT32 u4VdpId=bPath; /* use u4VdpId makes _pPicInfo[u4VdpId] search easier*/
    INT16 pos,max;
    #endif
    switch(bEvent)
    {
     #if NEW_5387_SCPOS_PORCH
        default:
        case 0:
            _VDP_ScposSetPorch(bPath, SV_HPORCH_CURRENT, 1000);
            _VDP_ScposSetPorch(bPath, SV_VPORCH_CURRENT, 1000);
            break;
        case 1:
        case 2:        
            break;	
     #else 
        default:
        case 0:    /* reset all rScposPorchState*/
            ScposPorch[u4VdpId].HOffset=0;
            ScposPorch[u4VdpId].VOffset=0;
            break;
        case 1:
            /* scpos Hdef/Vdef has been re-calculated */        
            ScposPorch[u4VdpId].Hdef=1000+_pPicInfo[u4VdpId]->wXOffset;
            ScposPorch[u4VdpId].Vdef=1000+_pPicInfo[u4VdpId]->wYOffset;
            break;
        case 2:        
            /* ScposPorch[u4VdpId].#Offset is only update thru HV api & mode detectdone*/
            pos=ScposPorch[u4VdpId].Hdef-1000+ScposPorch[u4VdpId].HOffset;
            pos=(pos>0)?pos:0;
            max=_pPicInfo[u4VdpId]->wSrcW-_pPicInfo[u4VdpId]->wTargetW;
            pos=(pos>max)?max:pos;
            _pPicInfo[u4VdpId]->wXOffset=pos;
            pos=ScposPorch[u4VdpId].Vdef-1000+ScposPorch[u4VdpId].VOffset;
            pos=(pos>0)?pos:0;
            max=_pPicInfo[u4VdpId]->wSrcH -_pPicInfo[u4VdpId]->wTargetH;
            pos=(pos>max)?max:pos;
            _pPicInfo[u4VdpId]->wYOffset=pos;
            break;
    #endif
    }
#if !SUPPORT_POP
#undef bPath
#undef u4VdpId
#endif
}


/**
* @brief Get scpos porch
* @param bPath VDP_1/VDP_2
* @param bPorchType porch type
* @return scpos porch
*/
UINT16 _VDP_ScposGetPorch(UINT8 bPath, UINT8 bPorchType)
{
    UINT32 u4VdpId=(UINT32)getScalerVDP(bPath); /* use u4VdpId makes _pPicInfo[u4VdpId] search easier*/
    switch(bPorchType)
    {
     #if NEW_5387_SCPOS_PORCH    
        case SV_HPORCH_CURRENT:
//            return _pPicInfo[u4VdpId]->wXOffset + _pPicInfo[u4VdpId]->wUIHPorch;
            return _pPicInfo[u4VdpId]->wUIHPorch;
        case SV_HPORCH_DEFAULT:
            return 1000;
        case SV_VPORCH_CURRENT:
            return _pPicInfo[u4VdpId]->wUIVPorch;
        case SV_VPORCH_DEFAULT:
            return 1000;
        case SV_HPORCH_MAX:
            return 1000+_pPicInfo[u4VdpId]->wSrcW-_pPicInfo[u4VdpId]->wTargetW - _pPicInfo[u4VdpId]->wXOffset;
        case SV_VPORCH_MAX:
            return 1000+_pPicInfo[u4VdpId]->wSrcH -_pPicInfo[u4VdpId]->wTargetH - _pPicInfo[u4VdpId]->wYOffset;
        case SV_HPORCH_MIN:
            return  1000 - _pPicInfo[u4VdpId]->wXOffset;
        case SV_VPORCH_MIN:
            return  1000 - _pPicInfo[u4VdpId]->wYOffset;
        default: /*MinMax*/
            return 1000;
     #else
        case SV_HPORCH_CURRENT:
            return ScposPorch[u4VdpId].Hdef+ScposPorch[u4VdpId].HOffset;
        case SV_HPORCH_DEFAULT:
            return ScposPorch[u4VdpId].Hdef;
        case SV_VPORCH_CURRENT:
            return ScposPorch[u4VdpId].Vdef+ScposPorch[u4VdpId].VOffset;
        case SV_VPORCH_DEFAULT:
            return ScposPorch[u4VdpId].Vdef;
        case SV_HPORCH_MAX:
            return 1000+_pPicInfo[u4VdpId]->wSrcW-_pPicInfo[u4VdpId]->wTargetW;
        case SV_VPORCH_MAX:
            return 1000+_pPicInfo[u4VdpId]->wSrcH -_pPicInfo[u4VdpId]->wTargetH;
        case SV_HPORCH_MIN:
        case SV_VPORCH_MIN:
        default: /*MinMax*/
            return 1000;
    #endif

    }
}


/**
* @brief Set scpos porch
* @param bPath VDP_1/VDP_2
* @param bPorchType porch type
* @param wValue porch value
*/
void _VDP_ScposSetPorch(UINT8 bPath,UINT8 bPorchType,UINT16 wValue)
{
#if SUPPORT_POP
    /* trigger from UI*/
    UINT8 u4VdpId = (UINT32)getScalerVDP(bPath); /* use u4VdpId makes _pPicInfo[u4VdpId] search easier*/
#else
#define bPath VDP_1
#define u4VdpId VDP_1
#endif
    #if (!NEW_5387_SCPOS_PORCH)
    INT16 i2Porch;
    #endif
    UINT32 u4Update;

    u4Update = 0;
    if (bPorchType==SV_HPORCH_CURRENT)
    {
        #if NEW_5387_SCPOS_PORCH    
        if (wValue != _pPicInfo[u4VdpId]->wUIHPorch) 
        {
            _pPicInfo[u4VdpId]->wUIHPorch = wValue;
            u4Update = 1;
        }
        #else
        i2Porch = (INT16)wValue - (INT16)ScposPorch[u4VdpId].Hdef; /* for overscan chg*/
        if (ScposPorch[u4VdpId].HOffset != i2Porch)
        {
            ScposPorch[u4VdpId].HOffset = i2Porch;
            u4Update = 1;
        }
        #endif
    }
    else
    {
        #if NEW_5387_SCPOS_PORCH    
        if (wValue != _pPicInfo[u4VdpId]->wUIVPorch) 
        {
            _pPicInfo[u4VdpId]->wUIVPorch = wValue;
            u4Update = 1;
        }
        #else
        i2Porch = (INT16)wValue - (INT16)ScposPorch[u4VdpId].Vdef; /* for overscan chg*/
        if (ScposPorch[u4VdpId].VOffset != i2Porch)
        {
            ScposPorch[u4VdpId].VOffset = i2Porch;
            u4Update = 1;
        }
        #endif
    }

    if (u4Update)
    {
        if (bPath == VDP_1)
        {
            vSetScposFlg(MAIN_HV_CHG);
        }
#if SUPPORT_POP
        else
        {
            vSetScposFlg(PIP_HV_CHG);
        }
#endif
    }
#if !SUPPORT_POP
#undef bPath
#undef u4VdpId
#endif
}
    
/**
* @brief Initialize tve related global paramters
*/
void vDrvScpipInit(void)
{

    vDrvDISetOutputClock(SV_VP_MAIN, DI_OCLK_TYPE_ROOT);
    vDrvSetSCPIPClock(SV_VP_MAIN, SCPIP_CLK_TYPE_VDOIN);
#if SUPPORT_POP
    vDrvDISetOutputClock(SV_VP_PIP, DI_OCLK_TYPE_ROOT);
    vDrvSetSCPIPClock(SV_VP_PIP, SCPIP_CLK_TYPE_VDOIN);
#endif

    //always use internal fifo, won't borrow sram from Pscan anymore
    vRegWriteFldAlign(SCPIP_PIP_OA_06,  1, PIP_OA_06_USE_INTF_EN1);
    vRegWriteFldAlign(SCPIP_PIP_OA_06,  0, PIP_OA_06_USE_EXTF_EN1);

    //when boot ,set PIP_OA_01_AUTO_UP_ADJUST to 0 .(default is 1 when boot)
    vRegWriteFldAlign(SCPIP_PIP_OA_01, 0, PIP_OA_01_AUTO_UP_ADJUST);

#ifdef CC_SCPOS_WA17
    vRegWriteFldAlign(SCPIP_PIP1_09, 0x1, PIP1_09_MACCWSEL_1);
#if SUPPORT_POP
    vRegWriteFldAlign(SCPIP_PIP2_09, 0x1, PIP2_09_MACCWSEL_2);
#endif
#else
    // Enable dram req delay to replace CC_SCPOS_WA17
    vRegWriteFldAlign(SCPIP_PIP1_0F, 1, PIP1_0F_WDRAM_DEL_SEL1);
#if SUPPORT_POP
    vRegWriteFldAlign(SCPIP_PIP2_0F, 1, PIP2_0F_WDRAM_DEL_SEL2);
#endif
#endif
   
    vFrameTrackConfig(SCPIP_FRAME_TRACK_LOCK_RANGE, SCPIP_FRAME_TRACK_LOSS_LOCK_RANGE, SCPIP_FRAME_TRACK_STEP_SMALL);//( u4LockRange,  u4LossLockRange,  u4Step) 
    #ifdef SUPPORT_SW_FRAME_CHECK
    //vSetSWFrameTrackLockRange(0x800 + SCPIP_FRAME_TRACK_LOCK_RANGE, 0x800);
    vSetSWFrameTrackLockRange(0x800 , 0x800);
    vSetSWFrameTrackStep(SCPIP_FRAME_TRACK_STEP_SMALL);
    #endif
        

    #ifdef CC_TVE_US_IMAGERESIZER_WA
    if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5363_64MBx2)
    {
        if (x_sema_create(&h_enableImageResizerSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
        {
                ASSERT(0);
        }
        VERIFY(x_thread_create(&h_enableImageResizerThread, "h_enableImageResizerInt", 2048, 20, _enableImageResizerThread, 0, NULL)==OSR_OK);
    }
    #endif

    #ifdef CC_NEW_DRAM_PROTECTION
    if (x_sema_create(&h_dramUpdateSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
            ASSERT(0);
    }
    #endif    
}
#if 0
void vDrvScpipForce2PSDivClock(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4N, UINT32 u4M)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    if ((_au4ForcePsDivClkEn[u4VdpId] == u4OnOff) && 
    	((u4OnOff == 0) || ((_au4ForcePsDivClkM[u4VdpId] == u4M) && (_au4ForcePsDivClkM[u4VdpId] == u4N))))
    {
        // do nothing
        return;
    }

    _au4ForcePsDivClkEn[u4VdpId] = u4OnOff;
    if (u4OnOff)
    {
        _au4ForcePsDivClkM[u4VdpId] = u4M;
        _au4ForcePsDivClkN[u4VdpId] = u4N;
    }

    _vDrvScpipConfigInputClk(getScalerPath(u4VdpId));

}
#endif

static void _vDrvScpipConfigInputClk(UINT8 bPath)
{
#if SUPPORT_POP
    UINT8 u4VdpId = getScalerVDP(bPath);
#else
#define bPath VDP_1
#endif
    UINT8 bMode = getScalerMode(bPath);
    DI_OUTPUT_CLOCK_TYPE eDiOClk;
    SCPIP_CLOCK_TYPE eScpipClk;

    if (bMode == VDP_SCPOS_PSCAN_DISPMODE)  //pscan dispmode
    {
        if (u1ScpipGetPSCANDispmodeScalingMode(bPath) == DOWN_SCALING_MODE)   //down-scaling
        {
            eDiOClk = DI_OCLK_TYPE_ROOT;
            eScpipClk = SCPIP_CLK_TYPE_PS_ROOT;
        }
        else
        {
            eDiOClk = DI_OCLK_TYPE_OCLK;
            eScpipClk = SCPIP_CLK_TYPE_OCLK;
        }
    }
    else
    {
        eDiOClk = DI_OCLK_TYPE_ROOT;
        if (bDrvVideoIsSrcInterlace(bPath))
        {
            eScpipClk = SCPIP_CLK_TYPE_PS_ROOT;
        }
        else
        {
            eScpipClk = SCPIP_CLK_TYPE_VDOIN;
        }
    }

    vDrvDISetPSCANClock(bPath, eDiOClk);
    vDrvSetSCPIPClock(bPath, eScpipClk);

#if !SUPPORT_POP
#undef bPath
#endif
}


void vDrvScpipToggleForceOff(UINT32 u4VdpId)
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif

    CRIT_STATE_T csState; 
    LOG(3, "vDrvScpipForceWENOff: Disable (%d) Write Relatch\n", u4VdpId); 
    if (u4VdpId == VDP_1)
    {          
        csState = x_crit_start(); 
        _u4ResetScposDramBuffer[VDP_1] = (_u4ResetScposDramBuffer[VDP_1] < 1)? 1: _u4ResetScposDramBuffer[VDP_1];
        vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_WEN_RLH_1);
        x_crit_end(csState);
        vDrvScpipWriteCtrl(VDP_1, SV_OFF);            
    }
#if SUPPORT_POP
    else
    {
        csState = x_crit_start(); 
        _u4ResetScposDramBuffer[VDP_2] = (_u4ResetScposDramBuffer[VDP_2] < 1)? 1: _u4ResetScposDramBuffer[VDP_2];
        vRegWriteFldAlign(SCPIP_PIP2_09, 0, PIP2_09_WEN_RLH_2);
        x_crit_end(csState);
        vDrvScpipWriteCtrl(VDP_2, SV_OFF);            
    }
#endif

#if !SUPPORT_POP
#undef u4VdpId
#endif
}

#endif // CC_SCPOS_EN


/**
* @brief Get scaler h porch
* @param bPath VDP_1/VDP_2
* @return h porch
*/
UINT16 _VDP_wGetHPorch(UINT8 bPath)
{
    UINT16 wRet = 0;
    
#ifdef CC_SCPOS_EN
    #if !SUPPORT_POP    
    if(bPath == SV_VP_PIP)
    {
        return wRet;
    }
    #endif

    //get H Proch value
    wRet = RegReadFldAlign(SCPIP_PIP1_04+getPIPOffset(bPath), PIP1_04_HP_OFST_1);
    
#else
    (void)(bPath);
#endif // CC_SCPOS_EN

    return wRet;
}

/**
* @brief Set mode change flag
* @param ucVdpId VDP_1/VDP_2
*/
void _VDP_ScalerReconfig(UCHAR ucVdpId)
{
    if (ucVdpId == VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
#if SUPPORT_POP
    else if (ucVdpId == VDP_2)
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }
#endif
}

#if 0//!REMOVE_UNCALLED_FUNCTION
/**
* @brief Get pre-scaling down DE width
* @param bPath VDP_1/VDP_2
* @return pre-scaling down DE width
*/
UINT32 wDrvVideoGetPreScaleDEWidth(UINT8 bPath)
{
    UINT16 u2Width;
    UINT32 u4Factor;

    if (bPath == VDP_1) // Main
    {
        u4Factor = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    }
#if SUPPORT_POP
    else
    {
        u4Factor = RegReadFldAlign(SCPIP_HDS_01, SCPIP_HDS_PIP_SCALER);
    }
#endif
    u2Width = DIVIDE_WITH_CARRIER(((UINT32)wDrvVideoInputDEWidth(bPath) * u4Factor), DFT_DS_FACTOR);
    
    return u2Width;
}
#endif


/**
* @brief Get pre-scaling down width
* @param bPath VDP_1/VDP_2
* @return pre-scaling down width
*/
UINT32 wDrvVideoGetPreScaleWidth(UINT8 bPath)
{
    UINT16 u2Width;
    UINT32 u4Factor;

#if SUPPORT_POP
    if (bPath == VDP_1) // Main
#endif
    {
        u4Factor = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    }
#if SUPPORT_POP
    else
    {
        u4Factor = RegReadFldAlign(SCPIP_HDS_01, SCPIP_HDS_PIP_SCALER);
    }
#endif
    u2Width = DIVIDE_WITH_CARRIER(((UINT32)wDrvVideoInputWidth(bPath) * u4Factor), DFT_DS_FACTOR);
    return u2Width;
}


/**
* @brief Handle dynamic scaling
* @param u4VdpId VDP_1/VDP_2
*/
static void _VDP_HandleDynamicScaling(UINT32 u4VdpId)
{
//    RVChannel* prChannel;
//    prChannel = ((u4VdpId == VDP_1) ? &_rMChannel : &_rPChannel);

    if ((_arScalePrm[u4VdpId].u4DynamicScale != 0) &&
        (_pChannel[u4VdpId]->bIsChannelOn != SV_OFF) &&
        (!_pChannel[u4VdpId]->bIsFreeze) &&
        (_arDynamicPrm[u4VdpId].u4State != VDP_DYNAMICE_SCALE_IDLE))
    {
        if (++_arDynamicPrm[u4VdpId].u4VsyncCount >= VDP_SCPOS_RESCALE_TIME)
        {
            _arDynamicPrm[u4VdpId].u4VsyncCount = 0;
            vSetScposFlg(((u4VdpId == VDP_1) ? MAIN_DISP_PRM_CHG : PIP_DISP_PRM_CHG));
        }
    }
}


/**
* @brief Handle channel freeze
* @param u4VdpId VDP_1/VDP_2
*/
static void _VDP_HandleChannelFreeze(UINT32 u4VdpId)
{
   // RVChannel* prChannel;
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
  //  prChannel = ((u4VdpId == VDP_1) ? &_rMChannel : &_rPChannel);
    if ( _pChannel[u4VdpId]->bIsGoingToFreeze && (_pChannel[u4VdpId]->bIsChannelOn == SV_ON) && (_arScalePrm[u4VdpId].u4Enable))
    {    // wait for some VSync to fill the whole scaler frame buffers before freezing
        UINT8 bMute;
        LOG(3, "Channel is going to freeze: %d\n", _pChannel[u4VdpId]->bIsGoingToFreeze);
        bMute =  ((u4VdpId == VDP_1) ? fgMainMuteOn() : fgPIPMuteOn());

        if (bMute)
        {
            LOG(3, "Channel is still in mute mode\n");
        }
        else
        {
            _pChannel[u4VdpId]->bIsGoingToFreeze --;
            if (_pChannel[u4VdpId]->bIsGoingToFreeze == 0)
            {
                LOG(3, "Channel freeze on\n");
                _pChannel[u4VdpId]->bIsFreeze = SV_ON;
            }
        }
    }    
#if !SUPPORT_POP
#undef u4VdpId
#endif
}


/**
* @brief Configure scaling mode
*/
void vScpipConfigScalingMode(UINT32 u4VdpId)
{
    UINT8 bMode = 0; // for main path only
    UINT8 bTap = 0;  // for main path only
    UINT8 bVUSBypass = SV_OFF;
    UINT8 bVDSBypass = SV_OFF;

#if !SUPPORT_POP
#define u4VdpId VDP_1
#define bPath VDP_1
    UINT8 bDispMode = getScalerMode(bPath);
#else
    UINT8 bPath = getScalerPath(u4VdpId);
    UINT8 bDispMode = getScalerMode(bPath);

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    if (bPath == SV_VP_MAIN)
#endif
    {
        if (bDispMode == VDP_SCPOS_DISPMODE_AUTO || bDispMode == VDP_SCPOS_DISPMODE_MANUAL) //scaler dispmode
        {
#if CC_SPEED_UP_DISPMODE_TUNE
            vScpipFrameTrackStep(SCPIP_FRAME_TRACK_STEP_LARGE);
#endif

            if (_pDispPrm[u4VdpId]->u4USclV == DFT_US_FACTOR)   //down-scaling
            {
                bMode = SCALER_DISPMODE_DOWN_SCALING_MODE;
                bVUSBypass = SV_ON;  // can be removed??
            }
            else    //up-scaling
            {
                bMode = SCALER_DISPMODE_UP_SCALING_MODE;
                bVDSBypass = SV_ON;  // can be removed??
            }
            bTap = UPSCALER_4TAP;
        }
        else if (bDispMode == VDP_SCPOS_PSCAN_DISPMODE)  //pscan dispmode
        {
#if CC_SPEED_UP_DISPMODE_TUNE
            vScpipFrameTrackStep(SCPIP_FRAME_TRACK_STEP_SMALL);
#endif

            if (u1ScpipGetPSCANDispmodeScalingMode(bPath) == UP_SCALING_MODE)   //up-scaling
            {
#ifdef CC_SCPOS_WA18
#if !CONFIG_MT8225_E1
                if ((((UINT32)wDrvGetOutputHTotal() * _pDispPrm[u4VdpId]->u4USclH / DFT_US_FACTOR) > 1998))
                {
                    bMode = DRAM_MODE_4_TAPS;
                    bTap = UPSCALER_4TAP;

                }
                else
#endif
#endif
#ifdef CC_SCPOS_WA19
#if !CONFIG_MT8225_E1
                    if ((_pPicInfo[u4VdpId]->wTargetW > 1898))
                    {
                        bMode = DRAM_MODE_4_TAPS;
                        bTap = UPSCALER_4TAP;
                    }
                    else
#endif
#endif
                    {
                        bMode = PSCAN_DISPMODE_UP;
                        bVDSBypass = SV_ON;
                        bTap = UPSCALER_6TAP;
                    }
            }
            else
            {
                bMode = PSCAN_DISPMODE_DOWN;
                bVUSBypass = SV_ON;  // can be removed??
                bTap = UPSCALER_4TAP;
            }
        }
        else //scalar DRAM mode
        {
#if CC_SPEED_UP_DISPMODE_TUNE
            vScpipFrameTrackStep(SCPIP_FRAME_TRACK_STEP_SMALL);
#endif

#ifdef CC_SUPPORT_JPEG
            if (fgIsSrcMM() && (!fgIsScposFlgSet(SCALER_MAIN_MODE_CHG)) &&  IsImportSel)
            {
                vRegWriteFldAlign(SCPIP_PIP1_04, 0, PIP1_04_IMPORT_SEL_1);
                IsImportSel = SV_FALSE;
            }
#endif
            if (_pDispPrm[u4VdpId]->u4USclV == DFT_US_FACTOR) //down-scaling
            {
                bMode = DRAM_MODE_BYPASS_UPSCALER;
                bVUSBypass = SV_ON; // can be removed??
                bTap = UPSCALER_4TAP;
            }
            else // up scaling
            {
#ifdef CC_SCPOS_WA18
                if (((UINT32)wDrvGetOutputHTotal() * _pDispPrm[u4VdpId]->u4USclH / DFT_US_FACTOR) > 1998)
                {
                    bMode = DRAM_MODE_4_TAPS;
                    bTap = UPSCALER_4TAP;
                }
                else
#endif
#ifdef CC_SCPOS_WA19
                    if (_pPicInfo[u4VdpId]->wTargetW > 1898)
                    {
                        bMode = DRAM_MODE_4_TAPS;
                        bTap = UPSCALER_4TAP;
                    }
                    else
#endif
                        if (bDrvVideoIsSrcInterlace(bPath) || bIsScalerInput444(bPath) || (_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_MPEGHD))
                        {
                            bMode = DRAM_MODE_4_TAPS;
                            bTap = UPSCALER_4TAP;
                        }
                        else
                        {
#ifdef CC_SCPOS_WA27
                            {
                                bMode = DRAM_MODE_4_TAPS;
                                bTap = UPSCALER_4TAP;
                            }
#else
                            {
                                bMode = DRAM_MODE_6_TAPS;
                                bTap = UPSCALER_6TAP;
                                bVDSBypass = SV_ON;
                            }
#endif
                        }
            }
        }



    }
#if SUPPORT_POP
    else  // sub path
    {
        if (bDispMode == VDP_SCPOS_PSCAN_DISPMODE)  //pscan dispmode
        {
            if (_pDispPrm[u4VdpId]->u4USclV == DFT_US_FACTOR) //down-scaling
            {
                bVUSBypass = SV_ON;
            }
            else
            {
                bVDSBypass = SV_ON;
            }
        }
        else //scalar DRAM mode
        {
            if (_pDispPrm[u4VdpId]->u4USclV == DFT_US_FACTOR) //down-scaling
            {
                bVUSBypass = SV_ON;
            }
            else // up scaling
            {
                bVDSBypass = SV_ON;
            }
        }


    }
#endif
    if (_arScalePrm[u4VdpId].u4ForceBypass)
    {
        bVUSBypass = SV_ON;
        bVDSBypass = SV_ON;
        if (bPath == SV_VP_MAIN)
        {
            bMode = DRAM_MODE_BYPASS_UPSCALER;
        }
    }

#if SUPPORT_POP
#ifdef CC_SCPOS_WA27
    if (bPath == SV_VP_PIP)
    {
        if (_u4VDSBypass[bPath] != bVDSBypass)
        {
            _u4VDSBypass[bPath] = bVDSBypass;
            _u4UpdateFifoConfig |= SCPIP_UPDATE_PIP_VDS;
        }

        if (_u4VUSBypass[bPath] != bVUSBypass)
        {
            _u4VUSBypass[bPath] = bVUSBypass;
            _u4UpdateFifoConfig |= SCPIP_UPDATE_PIP_VUS;
        }
    }
    else
#endif
#endif
    {
        //set UP-SCALER BYPASS
        _vScpipSetBypassUpScaler(bPath, bVUSBypass);
        _vScpipSetBypassDownScaler(bPath, bVDSBypass);
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvSetUpScalerVTap(bPath, bTap);
        //mtk01140 20091221 improve edge smooth quality
        /*
                if (bTap == UPSCALER_4TAP)
                {
                    vDrv2DEdgeSmoothOnOff(SV_OFF);
                }
                else
                {
                    vDrv2DEdgeSmoothOnOff(bDrvIs2DEdgeSmoothEnabled());
                }*/
        _vScpipSetDEAlignmentReg(bMode);
    }
#if !SUPPORT_POP
#undef bPath
#undef u4VdpId
#endif
}


/**
* @brief Set DE alignment related registers
* @param index bypass status
*/
static void _vScpipSetDEAlignmentReg(UINT8 index)
{

    switch (index)
    {
        case SCALER_DISPMODE_DOWN_SCALING_MODE :
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  1, PIP_OA_03_US_VOFST_BYPASS1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  2, PIP_OA_03_PRE_VOFST_BYPASS1);
            vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL2);
            vRegWriteFldAlign(OUTSTG_OS_75,     3, OS_75_VSYNC_DELSEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  0, PIP_OA_02_VSOUT_SEL);
            vRegWriteFldAlign(SCPIP_PIP1_0B,    0, PIP1_0B_VS_EDGE_SEL_1);
            vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
            vRegWriteFldAlign(SCPIP_PIP1_0A,    0, PIP1_0A_VS_DATA_THP_1);
            break;

        case SCALER_DISPMODE_UP_SCALING_MODE :
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  2, PIP_OA_03_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  1, PIP_OA_03_PRE_VOFST1);
            vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL2);
            vRegWriteFldAlign(OUTSTG_OS_75,     3, OS_75_VSYNC_DELSEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  0, PIP_OA_02_VSOUT_SEL);
            vRegWriteFldAlign(SCPIP_PIP1_0B,	0, PIP1_0B_VS_EDGE_SEL_1);
            vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
            vRegWriteFldAlign(SCPIP_PIP1_0A,    0, PIP1_0A_VS_DATA_THP_1);
            break;

        case DRAM_MODE_4_TAPS :
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  2, PIP_OA_03_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  3, PIP_OA_03_PRE_VOFST1);
            vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL2);
            vRegWriteFldAlign(OUTSTG_OS_75,     3, OS_75_VSYNC_DELSEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  2, PIP_OA_02_VSOUT_SEL);
            vRegWriteFldAlign(SCPIP_PIP1_0B,    0, PIP1_0B_VS_EDGE_SEL_1);
            vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
            vRegWriteFldAlign(SCPIP_PIP1_0A,    0, PIP1_0A_VS_DATA_THP_1);
            break;

        case DRAM_MODE_6_TAPS :
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  3, PIP_OA_03_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  2, PIP_OA_03_PRE_VOFST1);
            vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL2);
            vRegWriteFldAlign(OUTSTG_OS_75,     3, OS_75_VSYNC_DELSEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  2, PIP_OA_02_VSOUT_SEL);
            vRegWriteFldAlign(SCPIP_PIP1_0B,    0, PIP1_0B_VS_EDGE_SEL_1);
            vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
            vRegWriteFldAlign(SCPIP_PIP1_0A,    0, PIP1_0A_VS_DATA_THP_1);
            break;

        case PSCAN_DISPMODE_UP:
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  3, PIP_OA_03_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  2, PIP_OA_03_PRE_VOFST1);
            vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL2);
            vRegWriteFldAlign(OUTSTG_OS_75,     3, OS_75_VSYNC_DELSEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  2, PIP_OA_02_VSOUT_SEL);
            vRegWriteFldAlign(SCPIP_PIP1_0B,    0, PIP1_0B_VS_EDGE_SEL_1);
            vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
            vRegWriteFldAlign(SCPIP_PIP1_0A,    0, PIP1_0A_VS_DATA_THP_1);
            break;

        case DRAM_MODE_BYPASS_UPSCALER:
        case PSCAN_DISPMODE_DOWN:
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  1, PIP_OA_03_US_VOFST_BYPASS1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  4, PIP_OA_03_PRE_VOFST_BYPASS1);
            vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL2);
            vRegWriteFldAlign(OUTSTG_OS_75,     3, OS_75_VSYNC_DELSEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  2, PIP_OA_02_VSOUT_SEL);
            vRegWriteFldAlign(SCPIP_PIP1_0B,    0, PIP1_0B_VS_EDGE_SEL_1);
            vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
            vRegWriteFldAlign(SCPIP_PIP1_0A,    0, PIP1_0A_VS_DATA_THP_1);
            break;

            /*        case SCALER_DISPMODE_BYPASS_UPSCALER:
                        vRegWriteFldAlign(SCPIP_PIP_OA_03,  1, PIP_OA_03_US_VOFST_BYPASS1);
                        vRegWriteFldAlign(SCPIP_PIP_OA_03,  2, PIP_OA_03_PRE_VOFST_BYPASS1);
                        vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL2);
                        vRegWriteFldAlign(OUTSTG_OS_75,     3, OS_75_VSYNC_DELSEL);
                        vRegWriteFldAlign(SCPIP_PIP_OA_02,  0, PIP_OA_02_VSOUT_SEL);
                        vRegWriteFldAlign(SCPIP_PIP1_0B,    0, PIP1_0B_VS_EDGE_SEL_1);
                        vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
                        vRegWriteFldAlign(SCPIP_PIP1_0A,    1, PIP1_0A_VS_DATA_THP_1);
                    break;
                        vRegWriteFldAlign(SCPIP_PIP_OA_03,  3, PIP_OA_03_US_VOFST_BYPASS1);
                        vRegWriteFldAlign(SCPIP_PIP_OA_03,  3, PIP_OA_03_PRE_VOFST_BYPASS1);
                        vRegWriteFldAlign(OUTSTG_OS_75,     5, OS_75_VSYNC_DELSEL2);
                        vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL);
                        vRegWriteFldAlign(SCPIP_PIP_OA_02,  2, PIP_OA_02_VSOUT_SEL);
                        vRegWriteFldAlign(SCPIP_PIP1_0B,    0, PIP1_0B_VS_EDGE_SEL_1);
                        vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
                        vRegWriteFldAlign(SCPIP_PIP1_0A,    1, PIP1_0A_VS_DATA_THP_1);
                    break;
                */

        default:
            break;
    }
}

#if 0//!REMOVE_UNCALLED_FUNCTION
/**
 * @brief  Set main and sub scale mode
 * @param  wMainSrcV main source Height
 * @param  wSubSrcV sub source Height
 * @return scale mode
 *         E_V_M_UPSCALE = 0x01,
 *         E_V_M_DOWNSCALE = 0x02,
 *           E_V_M_NOSCALE = 0x04,
 *         E_V_S_UPSCALE = 0x10,
 *         E_V_S_DOWNSCALE = 0x20,
 *           E_V_S_NOSCALE = 0x40
 */
UINT8 bDrvScalerGetVscale()
{
    UINT8  ret;
    UINT16 mainSrcV;
    UINT16 mainDispV;
    UINT16 subSrcV;    
    UINT16 subDispV;
    
    ret = 0;
    mainSrcV = _rMPicInfo.wTargetH;
    mainDispV = _rMDispInfo.wHeight;
#if SUPPORT_POP
    subSrcV = _rPPicInfo.wTargetH;
    subDispV = _rPDispInfo.wHeight;
#endif
    //main channel
    if ((mainSrcV == mainDispV) || (_rMChannel.bIsChannelOn == SV_OFF) || (mainSrcV == 0) || (mainDispV == 0))
    {
        //NO SCALING
        ret |= E_V_M_NOSCALE;
    }
    else if(mainSrcV > mainDispV)
    {
        //DOWN SCALING    
        ret |= E_V_M_DOWNSCALE;
    }
    else if(mainSrcV < mainDispV)
    {
        ret |= E_V_M_UPSCALE;
    }
    
#if SUPPORT_POP
    if ((subSrcV == subDispV) || (_rPChannel.bIsChannelOn == SV_OFF) || (subSrcV == 0) || (subDispV == 0))
    {
        ret |= E_V_S_NOSCALE;   
    }
    else if(subSrcV > subDispV)
    {
        ret |= E_V_S_DOWNSCALE;
    }
    else if(subSrcV < subDispV)
    {
        ret |= E_V_S_UPSCALE;
    }
#endif
    return ret;
}

#endif

/**
* @brief Get pre-scaling down factor
* @param bPath VDP_1/VDP_2
* @return pre-scaling down factor
*/
UINT32 u4ScalerGetHPS(UINT8 bPath)
{
#if SUPPORT_POP
    if (bPath == SV_VP_MAIN)
#endif
    {
        return RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    }
#if SUPPORT_POP
    else
    {
        return RegReadFldAlign(SCPIP_HDS_01, SCPIP_HDS_PIP_SCALER);
    }
#endif
}


/**
* @brief Set scaler dispmode
* @param bPath VDP_1/VDP_2
* @param bMode mode type
*/
static void vScposSetScalerDispmode(UINT8 bPath,UINT8 bMode)
{
    //only main will enter dispmode
    if(bPath == SV_VP_PIP)
    {
        return;
    }

    #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new auto tune
    vRegWriteFldAlign(SCPIP_PIP1_0A, 0x8, PIP1_0A_TUNE_STEP_1);
    #else    
    if(_rMPicInfo.wHTotal * _rMPicInfo.wVTotal * bDrvVideoGetRefreshRate(SV_VP_MAIN) >100000000)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 0xF, PIP_OA_08_DFIFO_TUNE_STEP1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 0x10, PIP_OA_08_DFIFO_TUNE_STEP1);
    }
    #endif
    
    if(bMode == SV_ON)
    {
        #if CC_SPEED_UP_DISPMODE_TUNE
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DISP_TUNE, SV_VP_MAIN, 60, TRUE);
        #endif
        /* set DDDS and MOVLEN,MOHLEN,... */
        vDrvSetDynamicDisplay(SV_SUCCESS);                
        /* turn ON disp mode */
        vSwitchDispMode(SV_ON);
        /* Set tune parameter */
        _vScpipSetDispModeReg();
        /* set disp mode tuning register */
        #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new auto tune
        vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_RESET_AUTO_TUNE_1);
        vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);       
#ifdef SUPPORT_SW_FRAME_CHECK
        vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
#endif
        #endif

        _u4DispModeFreeRunCounter = CC_SCPOS_FREERUN_MAX;
        _arScalePrm[getScalerVDP(bPath)].u4DispModeEn = VDP_SCPOS_DISPMODE_AUTO;
    }
    else if(bMode == SV_OFF)
    {
        /* set dispmode tuning register to default value */
        /* turn OFF disp mode */
        vSwitchDispMode(SV_OFF);
        vDrvSetDynamicDisplay(SV_FAIL);                
        _VDP_ScposDispmodeFreeRun(SV_OFF);
        _u4DispModeFreeRunCounter=0;
    }
}


/**
* @brief Check if Scaler dispmode can be supported or not
* @param bPath VDP_1/VDP_2
*/
static UINT8 isScalerDispmodeSupported(UINT8 bPath)
{
    if(bDrvVideoSignalStatus(bPath) == SV_VDO_STABLE &&
       bDrvDispSupportFlag(wCalculateOutputVTotal()) == SV_SUCCESS)
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}

#ifdef CC_SCPOS_WA31
UINT8 bScpipPScanDispMirrorWA(UINT8 bPath, UINT8 bOnOff)
{
    RDispPrm* dispPrm;

    if(bPath > VDP_1)
        return 0; // only work around for main

    dispPrm =getDispPrm(VDP_1);
    vRegWriteFldAlign(HSHARP_MAIN_0F, bOnOff, MIRROR_OPTION_IN_CTI);
    
    //if(bOnOff && ((wDISPLAY_WIDTH != 1920) || (wDISPLAY_HEIGHT != 1080)))
    if(bOnOff && (dispPrm->u4DSclV !=DFT_US_FACTOR))  // WA on and Scaler down
    {
          bAddPScanMirWidth =2;
          vRegWriteFldAlign(SCPIP_DS1_02, 1, DS1_02_HSYNC_DELAY_EN1);
          vRegWriteFldAlign(SCPIP_PIP1_0F, 1, PIP1_0F_HSYNC_DELAY_EN1);
    }
    else // WA off or FHD panel
    {
        bAddPScanMirWidth =0;
        vRegWriteFldAlign(SCPIP_DS1_02, 0, DS1_02_HSYNC_DELAY_EN1);
        vRegWriteFldAlign(SCPIP_PIP1_0F, 0, PIP1_0F_HSYNC_DELAY_EN1);
    }

    vRegWriteFldAlign(SCPIP_PIP_OA_02, MAIN_ACTIVE_ADJ+bAddPScanMirWidth, PIP_OA_02_HSYNC_ADJ); 
    vRegWriteFldAlign(SCPIP_PIP1_0F, dispPrm->u4HBound+bAddPScanMirWidth,PIP1_0F_BOUNDARY_H_POS_1);
    vRegWriteFldAlign(SCPIP_PIP1_03, dispPrm->wRHLen+bAddPScanMirWidth, PIP1_03_RHL_1);

    LOG(3, "bScpipPScanDispMirrorWA : bAddPScanMirWidth(%d)\n", bAddPScanMirWidth);

    return bAddPScanMirWidth;
}
#endif

/**
* @brief For pscan to get overscan information. Especially in pscan dispmode.
* @param bPath VDP_1/VDP_2
* @param clipInfo structure to describe clip dimension.
* @return status
*/
UINT8 bScalerGetClipInfo(UINT8 bPath,PSCAN_CLIP_INFO_T* clipInfo)
{
    UINT32 u4Factor;
    RPicInfo* picInfo;
    UINT8 bMode;
#if !SUPPORT_POP
#define bPath VDP_1
#endif

 //   if (!bScalerModeChangeDone(bPath)) return SV_FAIL;

    picInfo = getPicInfo(bPath);
    u4Factor = u4ScalerGetHPS(bPath);
    bMode = getScalerMode(bPath);
        
    if(picInfo->wTargetH ==0 || picInfo->wTargetW==0 || u4Factor == 0)
    {
        return SV_FAIL;
    }
    
    if(bMode!= VDP_SCPOS_PSCAN_DISPMODE)
    {
        //If not in pscan dispmode, pscan should not do clip
        clipInfo->wX = 0;
        clipInfo->wWidth = wDrvVideoGetPreScaleWidth(bPath);
        clipInfo->wY = 0;
        clipInfo->wHeight = wDrvVideoInputHeight(bPath);
    }
    else    //pscan dispmode
    {
        #if NEW_5387_SCPOS_PORCH
        clipInfo->wX = (UINT32)getMixedHPorch(bPath) * u4Factor / DFT_HPD_FACTOR;
        #else    
        clipInfo->wX = (UINT32)picInfo->wXOffset * u4Factor / DFT_HPD_FACTOR;
        #endif
        clipInfo->wX = (clipInfo->wX%2==1)?clipInfo->wX+1:clipInfo->wX;
        clipInfo->wWidth = DIVIDE_WITH_CARRIER(((UINT32)picInfo->wTargetW * u4Factor),DFT_HPD_FACTOR);        
        #if NEW_5387_SCPOS_PORCH
        clipInfo->wY = getMixedVPorch(bPath);        
        #else
        clipInfo->wY = picInfo->wYOffset;        
        #endif
        clipInfo->wHeight = picInfo->wTargetH;
    }

    if(bPath==SV_VP_MAIN)// work around only for single mode due to di fifo length
    {
        #ifdef CC_MT5363
        if ((BSP_GetIcVersion() >= IC_VER_5363_AB)  && (bMode!= VDP_SCPOS_DISPMODE_AUTO) &&(bMode!= VDP_SCPOS_DISPMODE_MANUAL)) 
        #else// CC_MT5387
        if ((BSP_GetIcVersion() >= IC_VER_5387_AB) && (bMode!= VDP_SCPOS_DISPMODE_AUTO) &&(bMode!= VDP_SCPOS_DISPMODE_MANUAL)) 
        #endif
        {
            #ifdef CC_SCPOS_WA31
            clipInfo->wWidth += bAddPScanMirWidth;
            #endif
        }
        else
        {
            #ifdef CC_SCPOS_WA24
            clipInfo->wWidth += DECODER_ADD_WIDTH;
            #endif
        }
    }

#ifdef PSCAN_8223_PORTING_FINISH //MARK FOR PSCAN 8223 PROTING JOB
    #ifdef MDDI_WA1
    if (BSP_GetIcVersion() >= IC_VER_5387_AB) // After ECO
    {
        clipInfo->wWidth = vDrvDISetWAWidth(clipInfo->wWidth);
    }
    #endif
#endif

    return SV_SUCCESS;    
#if !SUPPORT_POP
#undef bPath
#endif
}


/**
* @brief Check if scaler mode change done
* @param bPath VDP_1/VDP_2
* @return status
*/
UINT8 bScalerModeChangeDone(UINT8 bPath)
{    
    if (bPath == SV_VP_MAIN)
    {
        if (!fgApiVideoIsEvtFlgSet(VDO_FLG_MAIN_MODECHG_DONE)) return FALSE;
                   
        if (fgIsScposFlgSet(SCALE_PRM_CHG | MAIN_HV_CHG|MAIN_DISP_PRM_CHG) || _bMainState == VDO_STATE_WAIT_MODE_DET)
        {
            return FALSE;
        }
        else
        {
            return TRUE;        
        }    
    }
    else
    {
        if (!fgApiVideoIsEvtFlgSet(VDO_FLG_PIP_MODECHG_DONE)) return FALSE;
    
        if (fgIsScposFlgSet(SCALE_PRM_CHG | PIP_HV_CHG|PIP_DISP_PRM_CHG) || _bPipState == VDO_STATE_WAIT_MODE_DET)
        {
            return FALSE;
        }
        else
        {
            return TRUE;        
        }        
    }
}

/**
* @brief Get H clip width
* @param bPath VDP_1/VDP_2
* @return clip width
*/
UINT16 wScalerGetHClipWidth(UINT8 bPath)
{
#if !SUPPORT_POP
#define bPath VDP_1
#endif
    UINT32 u4Factor;
    RPicInfo* picInfo;
    UINT16 HClipWidth;

    //if not in pscan dispmode, just return pre-scale downed width
    if(getScalerMode(bPath)!= VDP_SCPOS_PSCAN_DISPMODE)
    {
        return wDrvVideoGetPreScaleWidth(bPath);
    }

    //get input info
    picInfo = getPicInfo(bPath);
    //get pre-scale down factor
    u4Factor = u4ScalerGetHPS(bPath);
    
    //get the width pscan will read and write after doing overscan in pscan module.
    HClipWidth = (UINT32)picInfo->wTargetW * u4Factor / DFT_HPD_FACTOR;
    
    return HClipWidth;
#if !SUPPORT_POP
#undef bPath
#endif
}

/**
* @brief Set scpip clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
static void vDrvSetSCPIPClock(UINT8 bPath, SCPIP_CLOCK_TYPE eType)
{
#if !SUPPORT_POP
#define bPath VDP_1
#endif

    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, eType, FLD_SCPIP_MAIN_CLK_SEL);
    }
#if SUPPORT_POP
    else if (bPath == SV_VP_PIP)
    {
        vIO32WriteFldAlign(CKGEN_DISP_CKCFG1, eType, FLD_SCPIP_PIP_CLK_SEL);
    }
#endif

#if !SUPPORT_POP
#undef bPath
#endif
}

/**
* @brief Configure Pscan dispmode
* @param bPath VDP_1/VDP_2
* @param bScalingMode down/up/bypass
* @param bOnOff On/Off
*/
static void vScposConfigPSCANDispmode(UINT8 bPath,UINT8 bScalingMode,UINT8 bOnOff)
{
#if !SUPPORT_POP
#define bPath VDP_1
#endif
    UINT8 offset = getPIPOffset(bPath);
    UINT8 bmode = getScalerMode(bPath);
    UINT8 dsOffset = getDSOffset(bPath);            

    if (bPath >= SV_VP_NA)
    {
        return;
    }

    //turn off pscan dispmode
    if(bOnOff == SV_OFF)
    {
        LOG(3, "VDP(%d) Turn off dispmode", (UINT32)bPath);
        
        vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 0, PIP1_0C_PSCAN_IN_1);
        vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 0, PIP1_0C_PSCAN_SYNC_SEL_1);
        
        if(bmode == VDP_SCPOS_DISPMODE_AUTO || bmode == VDP_SCPOS_DISPMODE_MANUAL)
        {
            //If current mode is scaler dispmode, to prevent FMODESEL_1 from keep toggling between 0 and 1.
            //Just keep it as original value.
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP1_0B+offset, 0, PIP1_0B_FMODESEL_1);
        }
        
        //free run at h_total/v_total
        vDrvDISetOutputTimeGen(bPath, DI_TG_FREE_RUN);
        
        //switch pscan clock to video in
        _vDrvScpipConfigInputClk(bPath);

        vRegWriteFldAlign(SCPIP_PIP1_0A+offset, 0, PIP1_0A_VS_DATA_THP_1);
        //#if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNCP_1);
        //#endif

        if(bPath == SV_VP_MAIN)
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_00, 0, PIP_OA_00_INPUT_VSYNC_SEL_1);
        }
#if SUPPORT_POP
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_00, 0, PIP_OA_00_INPUT_VSYNC_SEL_2);
        }
#endif
    }
    else if(bOnOff == SV_ON)
    {
        //set global variable to pscan dispmode
        _arScalePrm[getScalerVDP(bPath)].u4DispModeEn = VDP_SCPOS_PSCAN_DISPMODE;
        
        if(bScalingMode == DOWN_SCALING_MODE)   //for down scaling
        {
            LOG(3, "VDP(%d) PSCAN Dispmode + DS", (UINT32)bPath);

            #if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DISP_TUNE, SV_VP_MAIN, CC_SCPOS_WA13_COUNTER_TO_INVERT_VSYNC+1, TRUE);
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNCP_1);
            _counterToInvertVSync[bPath] = CC_SCPOS_WA13_COUNTER_TO_INVERT_VSYNC;
            _flagToInvertVSync[bPath] = SV_TRUE;
            #else
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNCP_1);
            #endif
            
            //SCPOS_DbgMsgS("Turn on pscan dispmode [Down scaling]");
            //PSCAN dispmode -> down scaling
            vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 0, PIP1_0C_PSCAN_IN_1);
            vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 1, PIP1_0C_PSCAN_SYNC_SEL_1);
            
            //switch pscan clock to video in
            _vDrvScpipConfigInputClk(bPath);

           //PSCAN vsync prefetch
            _vScpipSetPscanPrelen(bPath,0x14);  

            vRegWriteFldAlign(SCPIP_PIP1_0A+offset, 1, PIP1_0A_VS_DATA_THP_1);    
            if(bPath == SV_VP_MAIN)
            {
                vRegWriteFldAlign(SCPIP_PIP_OA_00, 1, PIP_OA_00_INPUT_VSYNC_SEL_1);
            }
            #if SUPPORT_POP
            else
            {
                vRegWriteFldAlign(SCPIP_PIP_OA_00, 1, PIP_OA_00_INPUT_VSYNC_SEL_2);
            }
            #endif
            //Write data to line buffer
            vRegWriteFldAlign(SCPIP_PIP1_0B+offset, 1, PIP1_0B_FMODESEL_1);                 
        }
        else if(bScalingMode == UP_SCALING_MODE)    //for up scaling
        {
            LOG(3, "VDP(%d) PSCAN Dispmode + US", (UINT32)bPath);

            //PSCAN dispmode -> up scaling            
            vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 1, PIP1_0C_PSCAN_IN_1);
            vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 0, PIP1_0C_PSCAN_SYNC_SEL_1);            

            //switch pscan clock to output clock
            _vDrvScpipConfigInputClk(bPath);

            //PSCAN vsync prefetch
            _vScpipSetPscanPrelen(bPath, 0x14);
          
            vRegWriteFldAlign(SCPIP_PIP1_0A+offset, 0, PIP1_0A_VS_DATA_THP_1);   
            //#if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNCP_1);
            //#endif

            if(bPath == SV_VP_MAIN)
            {
                vRegWriteFldAlign(SCPIP_PIP_OA_00, 0, PIP_OA_00_INPUT_VSYNC_SEL_1);
            }
            #if SUPPORT_POP
            else
            {
                vRegWriteFldAlign(SCPIP_PIP_OA_00, 0, PIP_OA_00_INPUT_VSYNC_SEL_2);
            }
            #endif

            //Disable Write data to line buffer
            vRegWriteFldAlign(SCPIP_PIP1_0B+offset, 0, PIP1_0B_FMODESEL_1);     
        }
        
        //Horizontal pixel per line = pre-scale down size + pscan HBLANK       
        vRegWriteFldAlign(SCPIP_PIP1_0C+offset, _u4ScpipGetPscanDispmodeHLen(bPath), PIP1_0C_HLEN_PSCAN_1);        
        
        //lock to scaler
        vDrvDISetOutputTimeGen(bPath, DI_TG_LOCK_TO_SCALER);

    }
#if !SUPPORT_POP
#undef bPath
#endif
}


/**
* @brief Get Pscan dispmode horizontal length
* @param bPath VDP_1/VDP_2
* @param horizontal length
*/
static UINT32 _u4ScpipGetPscanDispmodeHLen(UINT8 bPath)
{
#if !SUPPORT_POP
#define bPath VDP_1
#endif

    UINT32 ret;
//    UINT16 u2RegOfst = getPSCANOffset(bPath);
//    UINT32 inputWidth;
    
    ret = ((wScalerGetHClipWidth(bPath)+15)/16)*16 + dwDrvDIGetHBlank(bPath);
    /*
    inputWidth = wDrvVideoInputWidth(bPath);
    if(inputWidth == 1280)
    {
        ret = 1680;
    }
    else if(inputWidth == 1440)
    {
        ret = 1696;
    }
    else if(inputWidth == 1920)
    {
        if (bDrvVideoIsSrcInterlace(bPath) && (bDrvVideoGetRefreshRate(bPath) == 50)) //1080i_50
        {
            return 2000;
        }
        else
        {
            return 2300;
        }
    }
*/
    return ret;
#if !SUPPORT_POP
#undef bPath
#endif
}


/**
* @brief Get Pscan dispmode h porch
* @param bPath VDP_1/VDP_2
* @return h porch
*/
static UINT32 _u4ScpipGetPscanDispmodeHPorch(UINT8 bPath)
{    
    #if 0 //5387 need this?
    UINT32 ret = 0;
    
    if (u1ScpipGetPSCANDispmodeScalingMode(bPath) == DOWN_SCALING_MODE)
    {
        //down scaling 
        if(bPath == SV_VP_MAIN)
        {
            ret =  CC_FIFO_H_DELAY_DOWN;
        }
        else
        {
            ret =  CC_FIFO_H_DELAY_DOWN;
        }
    }
    else
    {
        //up-scaling mode 
        if(bPath == SV_VP_MAIN)
        {
            ret = CC_FIFO_H_DELAY_UP;
        }
        else
        {
            ret = CC_FIFO_H_DELAY_UP;
        }        
    }

    return ret;
    #else
    return 0;
    #endif
}


/**
* @brief Set Pscan prelength
* @param bPath VDP_1/VDP_2
* @param u4PreLen
*/
static void _vScpipSetPscanPrelen(UINT8 bPath,UINT32 u4PreLen)
{
#if SUPPORT_POP
    UINT8 pipOffset;
    pipOffset = getPIPOffset(bPath);

    vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, (u4PreLen & 0xff), PIP1_0C_VS_PSCAN_PRELEN_1);
    vRegWriteFldAlign(SCPIP_PIP1_0B+pipOffset, ((u4PreLen >>8) & 0x7), PIP1_0B_VS_PSCAN_PRELEN_1);    
#else
    vRegWriteFldAlign(SCPIP_PIP1_0C , (u4PreLen & 0xff), PIP1_0C_VS_PSCAN_PRELEN_1);
    vRegWriteFldAlign(SCPIP_PIP1_0B , ((u4PreLen >> 8) & 0x7), PIP1_0B_VS_PSCAN_PRELEN_1);
#endif
}

#if 0
/**
* @brief Configure dispmode fifo
* @param bPath VDP_1/VDP_2
* @param fifoMode
*/
static void _vScpipConfigDispmodeFIFO(UINT8 bPath, E_IPC_FIFO_CONFIG_MODE fifoMode)
{
    UINT8 pipOffset;
    pipOffset = getPIPOffset(bPath);

    if(fifoMode == E_FIFO_MJC_DIPMODE_UP || fifoMode == E_FIFO_PSCAN_DISPMODE_UP)
    {
    #ifdef MT5387_TODO
        //1. set VDE_SEL
        if(bPath == SV_VP_MAIN)
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_MAIN_VDE_SEL_1);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_SUB_VDE_SEL_1);      
        }
        /*2. when VDE_SEL is 1, up scaler will send vsync earlier for N lines. N was determined by DISPMODE_V_DE_SEL.
             MJC dispmode + up   => DISPMODE_V_DE_SEL = 2
             PSCAN dispmode + up => DISPMODE_V_DE_SEL = 1
         */
        if(fifoMode==E_FIFO_MJC_DIPMODE_UP)
        {
            vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 2, PIP1_0C_DISPMODE_V_DE_SEL_1);
        }
        else if(fifoMode == E_FIFO_PSCAN_DISPMODE_UP)
        {
            vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 1, PIP1_0C_DISPMODE_V_DE_SEL_1);
        }

        /*3. Set DISPMODE_FIFO_CTRL to 1*/
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 1, PIP1_0C_DISPMODE_FIFO_CTRL_1);        

        /*4. Set DISPMODE_H_DE_SEL to 1. 
             When this bit was set to 1, the data request speed from up scaler to MJC or PSCAN was controled by register DISPMODE_DATA_RATE
         */
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 1, PIP1_0C_DISPMODE_H_DE_SEL_1);
   #endif        
        /*5. set DATA_RATE to 1024 (default value).*/
        vRegWriteFldAlign(SCPIP_PIP1_0E+pipOffset, 1024, PIP1_0E_DISPMODE_DATA_RATE_1);
   #ifdef MT5387_TODO
        /*6. set DISPMODE_WITH_FIFO to 1.
             When this bit was set to 1,data send out from MJC or PSCAN will be wrote into FIFO between MJC(or PSCAN) and SCALER.
         */
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 1, PIP1_0C_DISPMODE_WITH_FIFO_1);
   #endif
        /*7. set DISPMODE_FIFO_SEP_REG to 1. */
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 1, PIP1_0C_DISPMODE_FIFO_SEP_REG_1);

        /*8. When DISPMODE_FIFO_SEP_REG was set to 1. 
             SCALER will take HP_DUMP as HP
                              DATA_LENGTH_DUMP as DATA_LENGTH
                              VP_DUMP as VP
                              VL_DUMP as VL
                              SEL444_DUMP as SEL444
                              10B_SEL_DUMP as 10B_SEL
         */
         _vScpipSetDispmodeMirrorPara(bPath);
    }
    else if(fifoMode == E_FIFO_OTHERS)
    {
        #ifdef MT5387_TODO
        //1. set VDE_SEL to 0
        #ifdef MT5360B_WA3
        if(bPath == SV_VP_MAIN)
        {
             if (getScalerMode(bPath) != VDP_SCPOS_PSCAN_DISPMODE)
             {
                 vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_MAIN_VDE_SEL_1); // only reset if not pscan dispmode
             }
        }
        else
        {
             if (getScalerMode(bPath) != VDP_SCPOS_PSCAN_DISPMODE)
             {
                 vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_SUB_VDE_SEL_1); // only reset if not pscan dispmode
             }
        }
        
        #else
        if(bPath == SV_VP_MAIN)
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_MAIN_VDE_SEL_1);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_SUB_VDE_SEL_1);      
        }
        #endif

        /*2. Set VDE_SEL is 0*/
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 0, PIP1_0C_DISPMODE_V_DE_SEL_1);         

        /*3. Set DISPMODE_FIFO_CTRL to 0*/
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 0, PIP1_0C_DISPMODE_FIFO_CTRL_1);

        /*4. Set DISPMODE_H_DE_SEL to 0.*/
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 0, PIP1_0C_DISPMODE_H_DE_SEL_1);
        #endif
        /*5. set DATA_RATE to 1024 (default value).*/
        vRegWriteFldAlign(SCPIP_PIP1_0E+pipOffset, 1024, PIP1_0E_DISPMODE_DATA_RATE_1);
        #ifdef MT5387_TODO 
        /*6. set DISPMODE_WITH_FIFO to 0 */
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 0, PIP1_0C_DISPMODE_WITH_FIFO_1);
        #endif       
        /*7. set DISPMODE_FIFO_SEP_REG to 0. */
        vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 0, PIP1_0C_DISPMODE_FIFO_SEP_REG_1);
    }
}
#endif
#if 0
/**
* @brief Set dispmode mirror parameters
* @param bPath VDP_1/VDP_2
*/
static void _vScpipSetDispmodeMirrorPara(UINT8 bPath)
{
    UINT8 scalerMode;
    UINT8 pipOffset;
    UINT8 dsOffset;
    UINT32 vp;
    scalerMode = getScalerMode(bPath);    
    pipOffset = getPIPOffset(bPath);
    dsOffset = getDSOffset(bPath);

    //1. mirror HP
    if(scalerMode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        vRegWriteFldAlign(SCPIP_PIP1_0E+pipOffset, CC_FIFO_H_DELAY_UP, PIP1_0E_HP_DUMP);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_0E+pipOffset, RegReadFldAlign(SCPIP_PIP1_01+pipOffset, PIP1_01_HP_1), PIP1_0E_HP_DUMP);
    }    
    //2. mirror DATA_LENGTH
    if(bPath == SV_VP_MAIN && 
       _fgScpipFHDDotByDotTiming(bPath) == SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_PIP1_0D+pipOffset, RegReadFldAlign(SCPIP_PIP1_08+pipOffset, PIP1_08_DATA_LENGTH_1) + 1, PIP1_0D_DATA_LENGTH_DUMP);    
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_0D+pipOffset, RegReadFldAlign(SCPIP_PIP1_08+pipOffset, PIP1_08_DATA_LENGTH_1), PIP1_0D_DATA_LENGTH_DUMP);    
    }
    //3. mirror VP
    vp = RegReadFldAlign(SCPIP_PIP1_00+pipOffset, PIP1_00_VP_1);
    vRegWriteFldAlign(SCPIP_PIP1_0D+pipOffset, (vp & 0xff) , PIP1_0D_VP_DUMP_7_0);    
    vRegWriteFldAlign(SCPIP_PIP1_0E+pipOffset, (vp >>8) & 0xf, PIP1_0E_VP_DUMP_11_8);
    //4. mirror VL
    vRegWriteFldAlign(SCPIP_PIP1_0D+pipOffset, RegReadFldAlign(SCPIP_PIP1_00+pipOffset, PIP1_00_VL_1), PIP1_0D_VL_DUMP);    
    //5. mirror SEL444
    vRegWriteFldAlign(SCPIP_PIP1_0E+pipOffset, RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_SEL444_1), PIP1_0E_SEL444_DUMP);            
    //6. mirror 10B_SEL
    vRegWriteFldAlign(SCPIP_PIP1_0E+pipOffset, RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_VD10BSEL_1), PIP1_0E_10B_SEL_DUMP);
}
#endif

static UINT8 u1ScpipGetPSCANDispmodeScalingMode(UINT8 bPath)
{
#if SUPPORT_POP
    RDispPrm* dispPrm;
    UINT8 bMode;

    dispPrm = getDispPrm(bPath);
            
    if (dispPrm->u4DSclV != DFT_DS_FACTOR)
    {
        bMode = DOWN_SCALING_MODE;
    }
    else if(dispPrm->u4USclV != DFT_US_FACTOR)
#else
    UINT8 bMode;
    if (_rMDispPrm.u4DSclV != DFT_DS_FACTOR)
    {
        bMode = DOWN_SCALING_MODE;
    }
    else if (_rMDispPrm.u4USclV != DFT_US_FACTOR)
#endif
    {
        bMode = UP_SCALING_MODE;
    }
    else  // dot by dot
    {
        #ifdef CC_SCPOS_WA21
        bMode = DOWN_SCALING_MODE;
        #else
        bMode = UP_SCALING_MODE;
        #endif
    }

    return bMode;    
}


/**
* @brief set pscan display mode on/off
* @param bPath VDP_1/VDP_2
* @param bMode what mode to set
*/
static void vScposSetPSCANDispmode(UINT8 bPath,UINT8 bMode)
{
    UINT8 bScalingMode;

    bScalingMode = u1ScpipGetPSCANDispmodeScalingMode(bPath);
            
    vScposConfigPSCANDispmode(bPath, bScalingMode, bMode);    
}

static void vScpipDispmodeLineCfg(UINT8 bPath, UINT8 bmode)
{
    UINT8 bCfg;
	UINT8 bLineNum;
    
    if(bPath == SV_VP_MAIN)
    {
        if(bmode==VDP_SCPOS_DISPMODE_OFF)
        {
            bCfg=0;
        }
		else if(bmode==VDP_SCPOS_PSCAN_DISPMODE) //pscan  display mode
        {
            // main display mode line buf 1920x3x30
            // 444 intput samaller than 1800 can be configed to 4 lines (1800x24x4) 
            // 422 intput: all can config to 4 lines (2160x20x4)
            if(bIsScalerInput444(bPath) && _pPicInfo[bPath]->wTargetW >1800)
                bCfg=0;
            else
                bCfg=1;
        }
		else  // scaler display mode
        {
            bLineNum = 1440/_pDispPrm[SV_VP_MAIN]->wDataLen;
			if(bLineNum<4)
				bCfg = 0;
			else if(bLineNum<6)
				bCfg = 1;
			else if(bLineNum<8)
				bCfg = 2;
			else 
				bCfg = 3;
				
        }
		#if 0
        else //pscan / scaler display mode
        {
            // main display mode line buf 1920x3x30
            // 444 intput samaller than 1800 can be configed to 4 lines (1800x24x4) 
            // 422 intput: all can config to 4 lines (2160x20x4)
            if(bIsScalerInput444(bPath) && _pPicInfo[bPath]->wTargetW >1800)
                bCfg=0;
            else
                bCfg=1;
        }
		#endif
        vRegWriteFldAlign(SCPIP_PIP_OA_06, bCfg, PIP_OA_06_CFG_LINE1); 
    }
    else // sub path, todo
    {
            // sub display mode line buf 400x2x64
           // vRegWriteFldAlign(SCPIP_PIP_OA_07, 0, PIP_OA_07_CFG_LINE2); 
    }

}

#ifdef CC_FLIP_MIRROR_SUPPORT
static void _vScalerConfigFlip(UINT8 bPath, UINT8 bDramMode)
{
    if(bDramMode ==SV_TRUE) // do flip only when dram mode
    {
        UINT8 u1Config ;
        UINT8 ucEnable;
        u1Config = u1GetFlipMirrorConfig();
        TVE_GetEnable(TVE_1, &ucEnable);

        if((u1Config & SYS_MIRROR_CONFIG_ON) && !(ucEnable != SV_FALSE && bPath == SV_VP_PIP))
        {
            vScpipSetMirror(bPath, SV_ON);
            LOG(4, "vScalerConfigFlip: MIRROR ON\n");
        }	
        else 
        {
            vScpipSetMirror(bPath, SV_OFF);
            LOG(4, "vScalerConfigFlip: MIRROR OFF\n");
        }
        
        if((u1Config & SYS_FLIP_CONFIG_ON) && !(ucEnable != SV_FALSE && bPath == SV_VP_PIP))
        {
            vScpipSetFlip(bPath, SV_ON);
            LOG(4, "vScalerConfigFlip: FLIP ON\n");
        }
        else
        {
            vScpipSetFlip(bPath, SV_OFF);
            LOG(4, "vScalerConfigFlip: FLIP OFF\n");
        }
    }
    else
    {
        vScpipSetMirror(bPath, SV_OFF);
        vScpipSetFlip(bPath, SV_OFF);
        LOG(4, "vScalerConfigFlip: Both FLIP and MIRROR OFF\n");
    }
    //_vScpipSetDramProtect();
}
#endif

#if SUPPORT_POP
#ifdef CC_SCPOS_WA30
static void vScpipUpdateImportSel(UINT8 bPath)
{
    if (bPath == SV_VP_PIP)
    {
        #ifdef CC_FORCE_OLD_IMPORT
        vRegWriteFldAlign(SCPIP_PIP2_04, 0, PIP2_04_IMPORT_SEL_2);
        #else
        if ((bDrvVideoGetRefreshRate(bPath) > vDrvGetLCDFreq()) || u4ForceDispOff[bPath])
        {
            vRegWriteFldAlign(SCPIP_PIP2_04, 0, PIP2_04_IMPORT_SEL_2);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP2_04, 3, PIP2_04_IMPORT_SEL_2);
        }
        #endif
    }
}
#endif
#endif

/**
* @brief Set scaler mode
* @param bPath VDP_1/VDP_2
*/
static UINT8 vScalerDoScalerModeSwitch(UINT8 bPath)
{
#if !SUPPORT_POP
#define bPath VDP_1
#endif
    UINT8 mode;

#if CC_SCPOS_DISPMODE_NOT_SUPPORT
     fgIsDispFail = FALSE;
#endif    
    //get scaler current mode by path
    mode = getScalerMode(bPath);
    //PIP path don't support Scaler dispmode
#if SUPPORT_POP
    if (bPath == SV_VP_PIP && 
       (mode == VDP_SCPOS_DISPMODE_AUTO || mode == VDP_SCPOS_DISPMODE_MANUAL))
    {
        return SV_TRUE;
    }
#endif
    vScpipDispmodeLineCfg(bPath, mode);
                
    if (mode == VDP_SCPOS_DISPMODE_AUTO || mode == VDP_SCPOS_DISPMODE_MANUAL)
    {
        //set fast blank on for N VSyncs
        //skywalker.todo: check if this is needed?
        //vDrvSetFastBlank(SV_ON, 20);

        /* have to turn off PSCAN DISPMODE first
         * To test if scaler dispmode was supported or not, DS1_INVLEN_1 value is needed.
         * When scaler is in up-scaling pscan dispmode, DS1_INVLEN_1 won't be correct.
         * So before testing scaler dispmode, we have to turn off pscan dispmode first.
         */
        vRegWriteFldAlign(SCPIP_PIP1_04, 0, PIP1_04_IMPORT_SEL_1);
        
        vScposSetPSCANDispmode(bPath,SV_OFF);
        vDrvScpipWriteCtrl(bPath, SV_OFF);
        #ifdef CC_SCPOS_WA31
        //bScpipPScanDispMirrorWA(bPath, SV_OFF);
        #endif

        //if scaler dispmode can be supported 
        if (isScalerDispmodeSupported(bPath)==SV_TRUE)
        {
            vScposSetScalerDispmode(bPath,SV_ON);
        }
        else
        {
            vScposSetScalerDispmode(bPath,SV_OFF);
            #if CC_SCPOS_DISPMODE_NOT_SUPPORT
             fgIsDispFail = TRUE;
            #endif

            #if 0
            #ifdef CC_DTV_HD_DOT_BY_DOT
            #ifdef CC_SRM_ON
            if((u4CurrFullDotByDot !=0) || (u4CurrDispmodeOK!=0))
            {
                LOG(1,"SRM_SCPOS_EVENT_ZOOM_MODE : FALSE, FALSE\n");
                SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_ZOOM_MODE, SV_FALSE, SV_FALSE);
                u4CurrFullDotByDot = 0;
                u4CurrDispmodeOK =0;
            }
            #endif
            #endif
            #else
            SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_ZOOM_MODE, SV_FALSE, SV_FALSE);
            #endif
            LOG(1, "VDP(%d) [DM] fail to enter scaler dispmode\n", bPath);
        }
    }
    else if (mode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        vRegWriteFldAlign(SCPIP_PIP1_04, 0, PIP1_04_IMPORT_SEL_1);
        vScposSetScalerDispmode(bPath,SV_OFF);
        vDrvScpipWriteCtrl(bPath, SV_OFF);
        vScposSetPSCANDispmode(bPath,SV_ON);
        #ifdef CC_SCPOS_WA31
        //bScpipPScanDispMirrorWA(bPath, SV_ON);
        #endif
    }
    else if (mode == VDP_SCPOS_DISPMODE_OFF)
    {    
        #ifdef CC_FORCE_OLD_IMPORT
        vRegWriteFldAlign(SCPIP_PIP1_04, 0, PIP1_04_IMPORT_SEL_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04, 3, PIP1_04_IMPORT_SEL_1);
        #endif
        vScposSetScalerDispmode(bPath,SV_OFF);
        vScposSetPSCANDispmode(bPath,SV_OFF);
        //vDrvScpipWriteCtrl(bPath, SV_ON);
        #ifdef CC_SCPOS_WA31
        //bScpipPScanDispMirrorWA(bPath, SV_OFF);
        #endif

        _arScalePrm[getScalerVDP(bPath)].u4DispModeEn = VDP_SCPOS_DISPMODE_OFF;
    }
    #if CC_SCPOS_DISPMODE_NOT_SUPPORT
    if (fgIsMainFlgSet(MAIN_FLG_MODE_DET_DONE))
    {
        if (fgIsDispFail)
        {
            vApiNotifyResolutionChg(SV_VP_MAIN, SV_VDO_NOSUPPORT);
        }
        else
        {        
            vApiNotifyResolutionChg(SV_VP_MAIN, SV_VDO_STABLE);
        }
    }
    #endif

    return SV_TRUE;
#if !SUPPORT_POP
#undef bPath
#endif
}


/**
* @brief Set up scaler bypass
* @param bPath SV_VP_MAIN/SV_VP_PIP
* @param bMode
*/
static void _vScpipSetBypassUpScaler(UINT8 bPath,UINT8 bMode)
{
//When DS->US, import as normal at DS domain.when US->DS,vtap and vus_bypass should follow DS domain
    vRegWriteFldAlign(SCPIP_PIP1_04, (bMode == SV_ON) ? 1 : 0, PIP1_04_REG_VUS_BYPASS_SEL_1);
    vRegWriteFldAlign(SCPIP_PIP1_04, (bMode == SV_ON) ? 1 : 0, PIP1_04_REG_VTAP_SEL_DOMAIN_1);
    vRegWriteFldAlign(SCPIP_US1_01, (bMode == SV_ON) ? 1 : 0, US1_01_VUS_BYPASS_1);
}

/**
* @brief Set down scaler bypass
* @param bPath SV_VP_MAIN/SV_VP_PIP
* @param bMode
*/
static void _vScpipSetBypassDownScaler(UINT8 bPath,UINT8 bMode)
{
#if SUPPORT_POP
    UINT8 offset = getDSOffset(bPath);

    vRegWriteFldAlign(SCPIP_DS1_02 + offset, (bMode == SV_ON) ? 1 : 0, DS1_02_VDS_BYPASS_1);
#else
    vRegWriteFldAlign(SCPIP_DS1_02, (bMode == SV_ON) ? 1 : 0, DS1_02_VDS_BYPASS_1);
#endif
}

/**
* @brief Set DRAM r/w fifo
* @param bPath SV_VP_MAIN/SV_VP_PIP
* @param bMode
*/
#if 0
static void _vScpipSetDRAMFifo(UINT8 bPath, UINT8 bExtFifo, UINT8 bIntFifo)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_06, (bIntFifo==SV_ON)?1:0, PIP_OA_06_USE_INTF_EN1);
        vRegWriteFldAlign(SCPIP_PIP_OA_06, (bExtFifo==SV_ON)?1:0, PIP_OA_06_USE_EXTF_EN1);
    }
    #if SUPPORT_POP
    else if (bPath == SV_VP_PIP)    	
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_07, 1, PIP_OA_07_USE_INTF_EN2);
    }
    #endif
}
#endif

#if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
/**
* @brief Inverse scale vsync
* @param ucVdpId VDP_1/VDP_2
*/
static void vWA13_SetVSyncInvert(UCHAR ucVdpId)
{
    UINT8 dsOffset = getDSOffset(getScalerPath(ucVdpId));

    if (ucVdpId >= VDP_NS)
    {
        return;
    }
    
    if(_flagToInvertVSync[ucVdpId] == SV_TRUE)
    {
        if(_counterToInvertVSync[ucVdpId] !=0)
        {
            _counterToInvertVSync[ucVdpId]--;
        }
        if (_counterToInvertVSync[ucVdpId] == 1)
        {
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNCP_1);
        }
        else if (_counterToInvertVSync[ucVdpId] == 0)
        {
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNCP_1);
            _flagToInvertVSync[ucVdpId] = SV_FALSE;
        }
    }
}
#endif

/**
* @brief Check if video is freezable
* @param bPath VDP_1/VDP_2
*/
UINT8 fgScposIsVideoFreezable(UINT8 bPath)
{
    UINT8 scalerMode;
    UINT8 ret;
    
    ret = SV_TRUE;
    scalerMode = getScalerMode(bPath);

    // If scaler is in scaler dispmode or pscan dispmode, can't support freeze function.
    if(scalerMode == VDP_SCPOS_DISPMODE_AUTO || 
       scalerMode == VDP_SCPOS_DISPMODE_MANUAL ||
       scalerMode == VDP_SCPOS_PSCAN_DISPMODE)
    {       
        ret = SV_FALSE;
    }

    return ret;
}

UINT32 u4ScposGetInOutVDistance(void)
{
    return RegReadFldAlign(SCPIP_PIP1_RO_02, PIP1_RO_02_VCOUNTER_OUT_1);
}


/**
* @brief Check if upscaler enabled.
* @param bPath VDP_1/VDP_2
* @param direction horizontal/vertical
* @return status
*/
UINT8 fgScposIsUpscalerEnabled(UINT8 bPath,E_DIRECTION direction)
{
    UINT8 ret;
    RDispPrm* dispPrm;
    dispPrm = getDispPrm(bPath);

    ret = SV_TRUE;
    
    if(direction == E_DIRECTION_H)
    {
        if(dispPrm->u4USclH == DFT_US_FACTOR)
        {
            ret = SV_FALSE;
        }
    }
    else if(direction == E_DIRECTION_V)
    {
        if(dispPrm->u4USclV == DFT_US_FACTOR)
        {
            ret = SV_FALSE;
        }    
    }
    return ret;
}


/**
* @brief Check if FHD dot by dot
* @param bPath VDP_1/VDP_2
* @return status
*/

#if 0
//#ifdef CC_DTV_HD_DOT_BY_DOT
static UINT8 _fgScpipFHDDotByDotTiming(UINT8 bPath)
{
    if(_pPicInfo[bPath]->wTargetW == 1920 && 
        _pPicInfo[bPath]->wTargetH == 1080 &&
        (wDISPLAY_WIDTH == 1920) && 
        (wDISPLAY_HEIGHT == 1080))
    {   
        //FHD dot-by-dot
        return SV_TRUE;
    }
    return SV_FALSE;
}
#endif

/**
* @brief Update scaler parameters protected by import
* @param u4VdpId VDP_1/VDP_2
*/
void vUpdateRegister(UINT32 u4VdpId)
{
#if !SUPPORT_POP
#define u4VdpId VDP_1
#endif
    /* Set DS horizontal initial offset for Main channel. */
    _VDP_vSetDSInitialFactor(u4VdpId);
    _VDP_vSetDownScalingFactor(u4VdpId);
    _vScpipSetUSReg(u4VdpId);                
    _vScpipSetDispReg(u4VdpId);    

#ifdef CC_SCPOS_WA20
    if (RegReadFldAlign(SCPIP_PIP1_RO_00 + getPIPOffset(u4VdpId), PIP1_RO_00_W_POS_1) <= RegReadFldAlign(SCPIP_PIP1_09 + getPIPOffset(u4VdpId), PIP1_09_FSEL_1))
    {
        _vScpipToggleVPorch(u4VdpId);
        #if CC_SPEEDUP_IMPORT
        if (u4VdpId < VDP_NS)
        {
            _au4ImportUpdate[u4VdpId] |= SCPIP_UPDATE_W;
        }
        #endif
    }
    else
        Toggle_Later = SV_TRUE;
#endif
#if !SUPPORT_POP
#undef u4VdpId
#endif
}


#ifdef CC_SCPOS_WA14
/**
* @brief Check update status to update scaler parameters protected by import
* @param u4VdpId VDP_1/VDP_2
*/
void _VDP_ScposRegisterRelatch(UINT32 u4VdpId)
{
    if (u4DrvScposGetHwMuteStatus(u4VdpId)) // mute works
    {
        if (bStatusUpdate[u4VdpId])
        {
            (u4VdpId == VDP_1) ? vDrvMainImportOnOff(SV_OFF) : vDrvPIPImportOnOff(SV_OFF);
            vUpdateRegister(u4VdpId);
            vScpipConfigScalingMode(u4VdpId); 
            (u4VdpId == VDP_1) ? vDrvMainImportOnOff(SV_ON) : vDrvPIPImportOnOff(SV_ON);
            bStatusUpdate[u4VdpId] = 0;
        }
    }
}
#endif


/**
* @brief Scpos normal procedure
*/
void vScposNormProc(void) 
{
    CRIT_STATE_T csState;

    vOstgNormProc();
#ifdef CC_SCPOS_WA20
    //always toggle vporch for sclaer register take effect.
    if (Toggle_Later)
    {
        if (RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1) <= RegReadFldAlign(SCPIP_PIP1_09 , PIP1_09_FSEL_1))
        {
            _vScpipToggleVPorch(VDP_1);
            Toggle_Later = SV_FALSE;
        }
#if 0
#if SUPPORT_POP
        _vScpipToggleVPorch(VDP_2);
#endif
#endif

    }
#endif

    #ifndef CC_COPLAT_MT82
    OSD_PLA_Unmute();
    // temporily mark since the osd_dma is not ready, Tadd 2008/03/24
    //OSD_DMA_OnOutputVsync();
    #endif //CC_COPLAT_MT82
    
    #ifdef DRV_SUPPORT_EXTMJC
    if(IS_COMPANION_CHIP_ON())
    {
    	OSD_DMA_OnOutputVsync();
    }
    #endif

    #if CC_SCPOS_FW_MUTE_WA
        if (dwDrvGetMuteMask(VDP_1) && (!u4DrvScposGetHwMuteStatus(VDP_1)))
        {
            return;
        }
#if SUPPORT_POP
        if (dwDrvGetMuteMask(VDP_2) && (!u4DrvScposGetHwMuteStatus(VDP_2)))
        {
            return;
        }
    #endif
#endif
    
#if CC_SCPOS_MOVE_WRITECTRL
    if(_u4OsDumpEnable==SV_FALSE)
    {
        if (RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1) != 0) // srm not ready
        {
    #ifdef CC_TV_MEMO_MODE
            if(!bTVMemoOnOff)
    #endif        
            {
                _vScpipWriteCTRLCheck(VDP_1);
            }
        }
    }
    #if SUPPORT_POP
    if(_u4OsDumpEnable==SV_FALSE)
    {
        if (RegReadFldAlign(SCPIP_PIP2_05, PIP2_05_WSTARTP_2) != 0) // srm not ready
        {
            _vScpipWriteCTRLCheck(VDP_2);
        }
    }
    #endif
#endif

#ifdef CC_TV_MEMO_MODE
    if(!bTVMemoOnOff)
#endif    
    {
        _VDP_ScposChkDelayEnable(VDP_1);
    }
#if SUPPORT_POP
    _VDP_ScposChkDelayEnable(VDP_2);
#endif
    _vScpipDispmodeFreeRunChk();

    #if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
    #ifdef CC_TV_MEMO_MODE
    if(!bTVMemoOnOff)
    #endif    
    {
        vWA13_SetVSyncInvert(VDP_1);
    }
#if SUPPORT_POP
    vWA13_SetVSyncInvert(VDP_2);
    #endif
#endif

    if (_u4ScposImportNotReady != 0)
    {
        _u4ScposImportNotReady--;
        /*
        if(RegReadFldAlign(SCPIP_PIP1_09, PIP1_09_W_EN_1)==0 && _u4ScposImportNotReady > 1)
        {
            _u4ScposImportNotReady = 1;
        }
        */
        return;
    }
    else
    {
        //u4ForceFrameTrackCtrl &= (~SCPIP_FRAME_TRACK_OFF_V_PORCH);
        // if BOUNDARY_H_POS_1 become smaller we should disable H Boundary protection first, then _u4ScposImportNotReady is 0 enable H Boundary protection.
        vRegWriteFldAlign(SCPIP_PIP1_0F, 1, PIP1_0F_BOUNDARY_H_EN_1);
        #if SUPPORT_POP
        vRegWriteFldAlign(SCPIP_PIP2_0F, 1, PIP2_0F_BOUNDARY_H_EN_2);
        #endif

        #ifdef CC_SCPOS_WA33
        if (_au4ImportTrigger[VDP_1])
        {
            // give up workaround        
            _au4ImportTrigger[VDP_1] = 0;
            vDrvMainImportOnOff(SV_ON);
        }
        #if SUPPORT_POP
        if (_au4ImportTrigger[VDP_2])
        {
            // give up workaround        
            _au4ImportTrigger[VDP_2] = 0;
            vDrvPIPImportOnOff(SV_ON);
        }
        #endif //SUPPORT_POP
        #endif
        
        #if CC_SPEEDUP_IMPORT
        // clean import update flag
        _au4ImportUpdate[VDP_1] = 0;
        _au4ImportUpdate[VDP_2] = 0;
        #endif
    }


#ifdef CC_TV_MEMO_MODE
    if (bDisable)
    {
        if (u4Counter == 0)
        {
            _VDP_SetMemoMode(bMemoEnable);
            bDisable = FALSE;
        }
        else
        {
            u4Counter --;
        }
    }    
#endif

    // Handle Dynamic Scaling
#ifdef CC_TV_MEMO_MODE
    if(!bTVMemoOnOff)
#endif    
    {
        _VDP_HandleDynamicScaling(VDP_1);
    }
#if SUPPORT_POP
    _VDP_HandleDynamicScaling(VDP_2);
#endif
#ifdef CC_TV_MEMO_MODE
    if(!bTVMemoOnOff)
#endif
    {
        _VDP_HandleChannelFreeze(VDP_1);
    }
#if SUPPORT_POP
    _VDP_HandleChannelFreeze(VDP_2);
#endif
#ifdef CC_SCPOS_WA14
#ifdef CC_TV_MEMO_MODE
    if(!bTVMemoOnOff)
#endif
    {
        _VDP_ScposRegisterRelatch(VDP_1);
    }
#if SUPPORT_POP
    _VDP_ScposRegisterRelatch(VDP_2);
#endif
#endif

    if (fgIsScposFlgSet(SCALE_PRM_CHG | MAIN_HV_CHG|PIP_HV_CHG|MAIN_DISP_PRM_CHG | PIP_DISP_PRM_CHG))
    {
        UINT32 u4UpdateMain;
        UINT32 u4UpdatePip;
        u4UpdateMain = 0;
        u4UpdatePip = 0;

#ifdef CC_TV_MEMO_MODE
    if(!bTVMemoOnOff)
#endif
    {
        if (fgIsScposFlgSet(SCALE_PRM_CHG | MAIN_DISP_PRM_CHG|MAIN_HV_CHG)
#ifdef CC_QUEUE_EVENT_IN_FREEZE
            && (!_rMChannel.bIsFreeze)
#endif            
            )
        {
#ifdef __MODEL_slt__
            if ((_rMChannel.bIsChannelOn == SV_ON) && (!_rMChannel.bIsFreeze))
#else
           /* DTV00209813, update display window even when no source is selected */
           // if (_rMChannel.bIsChannelOn == SV_ON)
#endif
            {   
                #if 0
                #ifdef CC_DTV_HD_DOT_BY_DOT
                UINT32 u4IsFullDotByDot= SV_FALSE, u4DispmodeOK= SV_FALSE;
                #endif
                #endif
                //re-calculate scaling factor
                _vScpipCalNewScale(VDP_1);

                #if 0
                #ifdef CC_DTV_HD_DOT_BY_DOT
                u4IsFullDotByDot = _fgScpipFHDDotByDotTiming(VDP_1);
                if(u4IsFullDotByDot)
                {
                    u4DispmodeOK = isScalerDispmodeSupported(VDP_1);
                }

                #ifdef CC_SRM_ON
                if((u4CurrFullDotByDot !=u4IsFullDotByDot) || (u4CurrDispmodeOK!=u4DispmodeOK))
                {
                    LOG(1,"SRM_SCPOS_EVENT_ZOOM_MODE : u4IsFullDotByDot(%d), u4DispmodeOK(%d)\n",u4IsFullDotByDot, u4DispmodeOK);
                    SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_ZOOM_MODE, u4IsFullDotByDot, u4DispmodeOK);
                    u4CurrFullDotByDot = u4IsFullDotByDot;
                    u4CurrDispmodeOK =u4DispmodeOK;
                }
                #endif//CC_SRM_ON
                #endif//CC_DTV_HD_DOT_BY_DOT
                #endif

                if ((!fgMainWriteOn()) && _au4ScalingFactorChanged[VDP_1])
                {
                    _vDrvVideoSetMute(MUTE_MODULE_SCPOS_MISC, VDP_1, VDP_SCPOS_DELAY_UNMUTE_TIME, FALSE);
                    LOG(6, "MUTE_MODULE_SCPOS_MISC : scaling factor change when write off\n");
                }
        
                _VDP_ScposPorchUpdate(VDP_1, 2); //update offset.
                _vScpipUpdateDispPrm(VDP_1);  // _VDP_vUpdateMainDispPrm();
                
                _pPicInfo[VDP_1]->u1Interlace = bDrvVideoIsSrcInterlace(VDP_1);

                u4UpdateMain = 1;    
            }
            //switch pscan clock to video in
            _vDrvScpipConfigInputClk(VDP_1);

            vClrScposFlg(MAIN_DISP_PRM_CHG);
            vClrScposFlg(MAIN_HV_CHG);
        }
    }
    
#if SUPPORT_POP
        if (fgIsScposFlgSet(SCALE_PRM_CHG | PIP_DISP_PRM_CHG|PIP_HV_CHG)
#ifdef CC_QUEUE_EVENT_IN_FREEZE 
            && (!_rPChannel.bIsFreeze)
#endif
            )
        {        
#ifdef CC_SCPOS_WA30
		    vScpipUpdateImportSel(SV_VP_PIP);
#endif             
            #ifdef __MODEL_slt__
            if ((_rPChannel.bIsChannelOn == SV_ON) && (!_rPChannel.bIsFreeze))
            #else
            /* DTV00209813, update display window even when no source is selected */
            //if (_rPChannel.bIsChannelOn == SV_ON)
            #endif
            {
                _vScpipCalNewScale(VDP_2);
                _VDP_ScposPorchUpdate(VDP_2,2); //update offset.
                _vScpipUpdateDispPrm(VDP_2);  // _VDP_vUpdatePipDispPrm();

                _pPicInfo[VDP_2]->u1Interlace = bDrvVideoIsSrcInterlace(VDP_2);

                u4UpdatePip = 1;
            }

            vClrScposFlg(PIP_DISP_PRM_CHG);
            vClrScposFlg(PIP_HV_CHG);   //[DTV00113962]
        }
#endif

        if ((u4UpdateMain) || (u4UpdatePip))
        {            
            #ifdef CC_FORCE_OLD_IMPORT
            _u4ScposImportNotReady = CC_FORCE_OLD_IMPORT;
            #else
            _u4ScposImportNotReady = VDP_SCPOS_IMPORT_PROTECT;
            #endif

            #ifdef CC_TV_MEMO_MODE
            if(!bTVMemoOnOff)
            #endif
            {                       
            if (u4UpdateMain)
            {
                vDrvMainImportOnOff(SV_OFF);
                    //   u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_V_PORCH;
                    //   vSwitchFrameTrackOnOff(SV_OFF);
                #ifdef CC_SCPOS_WA14
                if (_u4MainMuteMask && (!u4DrvScposGetHwMuteStatus(VDP_1))) // mute not works
                {
                    bStatusUpdate[VDP_1] = 1;
                }
                else
                #endif
                {
                    vUpdateRegister(VDP_1);
                    vScpipConfigScalingMode(VDP_1); 
                    //vDrvScpipSetClk(VDP_1); 
                }       

                if (bScpipFlipEnable[VDP_1])
                {
                  //  LOG(0,"in normproc");
                    #if CC_SCPOS_WA_FLIP_IMPORT
                    if(_pPicInfo[VDP_1]->u1Interlace)
                    {
                        vDrvDIChangeISRType(VDP_1, E_DI_OUT_V_F);
                    }
                    else
                    {
                        vDrvDIChangeISRType(VDP_1, E_DI_IN_V_F);
                    }
					

                    if(_vDrvVideoGetMuteDelay(VDP_1, MUTE_MODULE_MODECHG))
                    {                  
                        #ifdef CC_NEW_DRAM_PROTECTION
                        _vScpipUpdateDramParameter(VDP_1, u4BaseAddress[VDP_1], PARAM_IGNORE, PARAM_IGNORE);
                        #else
                        _vScpipSetDram(VDP_1, u4BaseAddress[VDP_1], 0);
                        #endif
                        vDrvMainUpdateVBound(_pDispPrm[VDP_1]->wVLen);
                        vDrvMainImportOnOff(SV_ON);
                    }
                    else
                        u4UpdateFlipImport[VDP_1] = SV_TRUE;
                    #else
                    ASSERT(0);
               //     _vScpipSetDram(VDP_1, u4BaseAddress[VDP_1], 0);
                 //   vDrvMainUpdateVBound(_pDispPrm[VDP_1]->wVLen);
                //    vDrvMainImportOnOff(SV_ON);
                    #endif
                }
                else
                {
                    #if CC_SCPOS_WA_FLIP_IMPORT
                    vDrvDIChangeISRType(VDP_1, E_DI_OUT_V_F);
                    u4UpdateFlipImport[VDP_1] = SV_FALSE;
                    #ifdef CC_NEW_DRAM_PROTECTION
                    _vScpipUpdateDramParameter(VDP_1, u4BaseAddress[VDP_1], PARAM_IGNORE, PARAM_IGNORE);
                    #else
                    _vScpipSetDram(VDP_1, u4BaseAddress[VDP_1], 0);
                    #endif
                    #endif
                    #ifdef CC_SCPOS_WA33 
                    if ((_u4ResetScposDramBuffer[VDP_1] == 0) 
                        #if CC_SPEEDUP_IMPORT
                        && ((_au4ImportUpdate[VDP_1] & (SCPIP_UPDATE_W|SCPIP_UPDATE_R)) == (SCPIP_UPDATE_W|SCPIP_UPDATE_R))
                        #endif                       
                        && (RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1) != 1))
                    {
                        _au4ImportTrigger[VDP_1] = 1;
                    }
                    else
                    #endif    
                    {
                        #ifdef CC_SCPOS_WA33    
                         _au4ImportTrigger[VDP_1] = 0;
                        #endif
                        vDrvMainImportOnOff(SV_ON);
                    }
                }

                #if CC_TWO_FRAME_LIMIT
                _vScpipSetTwoFrameLimit(VDP_1);
                #endif
                
    #ifdef CC_SCPOS_WA10
                if ((_au4FrameNumberChanged[VDP_1]  > 0) && (_au4FrameNumberChanged[VDP_1] < FRAME_NUMEBR_CHANGED_COUNT))
                {
                    _au4FrameNumberChanged[VDP_1] = FRAME_NUMEBR_CHANGED_COUNT;
                    LOG(3, "DS factors changed! Reset _au4FrameNumberChanged[VDP_1]\n");
                }
    #endif            
                
                #ifdef CC_TOGGLE_W_EN
                if ((_au4FrameNumberChanged[VDP_1]  > 0) && (_au4FrameNumberChanged[VDP_1] < FRAME_NUMEBR_CHANGED_COUNT))
                {
                    UINT8 bMainWEN;
                    bMainWEN = RegReadFldAlign(SCPIP_PIP1_09, PIP1_09_W_EN_1);
                    vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_W_EN_1);
                    vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_W_EN_1);
                    vRegWriteFldAlign(SCPIP_PIP1_09, bMainWEN, PIP1_09_W_EN_1);        
                    _au4FrameNumberChanged[VDP_1] = 0;
                }
                #endif
                
            }
    }
#if SUPPORT_POP
            if (u4UpdatePip)
            {
                vDrvPIPImportOnOff(SV_OFF);
                #ifdef CC_SCPOS_WA14
                if (_u4PipMuteMask && (!u4DrvScposGetHwMuteStatus(VDP_2))) // mute not works
                {
                    bStatusUpdate[VDP_2] = 1;
                }
                else
                #endif
                {
                    vUpdateRegister(VDP_2);
                    vScpipConfigScalingMode(VDP_2); 
                    //vDrvScpipSetClk(VDP_2); 
                }
                
                if (bScpipFlipEnable[VDP_2])
                {
                    #if CC_SCPOS_WA_FLIP_IMPORT
                    if(_pPicInfo[VDP_2]->u1Interlace)
                    {
                        vDrvDIChangeISRType(VDP_2, E_DI_OUT_V_F);
                    }
                    else
                    {
                        vDrvDIChangeISRType(VDP_2, E_DI_IN_V_F);
                    }

                    if(_vDrvVideoGetMuteDelay(VDP_2, MUTE_MODULE_MODECHG))
                    {
                        #ifdef CC_NEW_DRAM_PROTECTION
                        _vScpipUpdateDramParameter(VDP_2, u4BaseAddress[VDP_2], PARAM_IGNORE, PARAM_IGNORE);
                        #else
                        _vScpipSetDram(VDP_2, u4BaseAddress[VDP_2], 0);
                        #endif
                        vDrvPIPImportOnOff(SV_ON);
                    }
                    else
                        u4UpdateFlipImport[VDP_2] = SV_TRUE;
                    #else
                    ASSERT(0);
//                    _vScpipSetDram(VDP_2, u4BaseAddress[VDP_2], 0);
//                    vDrvPIPImportOnOff(SV_ON);
                    #endif
                }
                else
                {
                    #if CC_SCPOS_WA_FLIP_IMPORT
                    vDrvDIChangeISRType(VDP_2, E_DI_OUT_V_F);
                    u4UpdateFlipImport[VDP_2] = SV_FALSE;
                    #ifdef CC_NEW_DRAM_PROTECTION
                    _vScpipUpdateDramParameter(VDP_2, u4BaseAddress[VDP_2], PARAM_IGNORE, PARAM_IGNORE);
                    #else
                    _vScpipSetDram(VDP_2, u4BaseAddress[VDP_2], 0);
                    #endif
                    #endif
                    vDrvPIPImportOnOff(SV_ON);
                }

                #if CC_TWO_FRAME_LIMIT
                _vScpipSetTwoFrameLimit(VDP_2);
                #endif
                
    #ifdef CC_SCPOS_WA10
                if ((_au4FrameNumberChanged[VDP_2]  > 0) && (_au4FrameNumberChanged[VDP_2] < FRAME_NUMEBR_CHANGED_COUNT))
                {
                    _au4FrameNumberChanged[VDP_2] = FRAME_NUMEBR_CHANGED_COUNT;
                    LOG(3, "DS factors changed! Reset _au4FrameNumberChanged[VDP_2]\n");
                }
    #endif
                #ifdef CC_TOGGLE_W_EN
                if ((_au4FrameNumberChanged[VDP_2]  > 0) && (_au4FrameNumberChanged[VDP_2] < FRAME_NUMEBR_CHANGED_COUNT))
                {
                    UINT8 bSubWEN;
                    bSubWEN = RegReadFldAlign(SCPIP_PIP2_09, PIP2_09_W_EN_2);
                    vRegWriteFldAlign(SCPIP_PIP2_09, 0, PIP2_09_W_EN_2);
                    vRegWriteFldAlign(SCPIP_PIP2_09, 1, PIP2_09_W_EN_2);
                    vRegWriteFldAlign(SCPIP_PIP2_09, bSubWEN, PIP2_09_W_EN_2);
                    _au4FrameNumberChanged[VDP_2] = 0;
                }
                #endif
            }
            #endif // SUPPORT_POP
        }
        vClrScposFlg(SCALE_PRM_CHG);

        #if CC_SPEEDUP_IMPORT
        LOG(3, "ImportUpdate(Main) (%d), (Sub) (%d)\n", _au4ImportUpdate[VDP_1], _au4ImportUpdate[VDP_2]);
        if (((_au4ImportUpdate[VDP_1] & (SCPIP_UPDATE_W|SCPIP_UPDATE_R)) != (SCPIP_UPDATE_W|SCPIP_UPDATE_R))
            && ((_au4ImportUpdate[VDP_2] & (SCPIP_UPDATE_W|SCPIP_UPDATE_R)) != (SCPIP_UPDATE_W|SCPIP_UPDATE_R)))
        {
            LOG(3, "Spped up import\n");
            _u4ScposImportNotReady = VDP_SCPOS_SPEEDUP_IMPORT_PROTECT;
        }
        #endif                       

    }

    //update scaler mode of main path
    if (fgIsScposFlgSet(SCALER_MAIN_MODE_CHG)
        #ifdef CC_SCPOS_WA14    
        && (bStatusUpdate[VDP_1] == 0) 
        #endif
       )
    {
        #if CC_SCPOS_FW_MUTE_WA
        if (dwDrvGetMuteMask(VDP_1) && (!u4DrvScposGetHwMuteStatus(VDP_1)))
        {
            return;
        }
        #endif
        vScalerDoScalerModeSwitch(getScalerPath(VDP_1));   
#ifdef CC_SCPOS_WA20
        _vScpipToggleVPorch(VDP_1);
#endif  
        vSetScposFlg(MAIN_DISP_PRM_CHG);   
        vClrScposFlg(SCALER_MAIN_MODE_CHG);        
    }
    //update scaler mode of pip path
#if SUPPORT_POP
    if (fgIsScposFlgSet(SCALER_PIP_MODE_CHG)
        #ifdef CC_SCPOS_WA14    
        && (bStatusUpdate[VDP_2] == 0) 
        #endif
       )
    {
        #if CC_SCPOS_FW_MUTE_WA
        if (dwDrvGetMuteMask(VDP_2) && (!u4DrvScposGetHwMuteStatus(VDP_2)))
        {
            return;
        }
        #endif
        vScalerDoScalerModeSwitch(getScalerPath(VDP_2));
#ifdef CC_SCPOS_WA20
        _vScpipToggleVPorch(VDP_2);
#endif
        vSetScposFlg(PIP_DISP_PRM_CHG);   
        vClrScposFlg(SCALER_PIP_MODE_CHG);        
    }
#endif

// added by peggy 20071108
#ifdef CC_QUEUE_EVENT_IN_FREEZE
    if ((!_rMChannel.bIsFreeze) && (
#if SUPPORT_POP
                (!_rPChannel.bIsFreeze) ||
#endif
                (SRM_GetTvMode() == SRM_TV_MODE_TYPE_NORMAL)))
#endif            
        {
           vClrScposFlg(SCALE_PRM_CHG);
        }
#if SUPPORT_POP
#ifdef CC_QUEUE_EVENT_IN_FREEZE
        else if ((SRM_GetTvMode() == SRM_TV_MODE_TYPE_PIP) || (SRM_GetTvMode() == SRM_TV_MODE_TYPE_POP))
        {
            if ((!_rMChannel.bIsFreeze) && (_rPChannel.bIsFreeze))
            {  // clean main event and keep sub event
               vClrScposFlg(SCALE_PRM_CHG);
               vSetScposFlg(PIP_DISP_PRM_CHG);               
            }
            else if ((_rMChannel.bIsFreeze) && (!_rPChannel.bIsFreeze))
            {  // keep main event and clean sub event
               vClrScposFlg(SCALE_PRM_CHG);
               vSetScposFlg(MAIN_DISP_PRM_CHG);               
            }
        }
#endif            
#endif

    //??
    csState = x_crit_start();
    #ifdef CC_TV_MEMO_MODE
    if(!bTVMemoOnOff)
    #endif
    {    
        #ifdef CC_SCPOS_WA14    
        if (bStatusUpdate[VDP_1] == 0) 
        #endif
        {
            if ((_u4ResetScposDramBuffer[VDP_1]) && ((_u4ResetScposDramBuffer[VDP_1] & 0x80000000) == 0))
            {
                _u4ResetScposDramBuffer[VDP_1]--;
                if (_u4ResetScposDramBuffer[VDP_1] == 0)
                {
                    vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_WEN_RLH_1); 
                    if (_au4DelayWriteEnable[VDP_1])
                    {
                //        vDrvScpipWriteCtrl(VDP_1, SV_ON);
                        _au4DelayWriteEnable[VDP_1] = 0;
                    }                    
                }
            }
        }
    }    
#if SUPPORT_POP
    #ifdef CC_SCPOS_WA14    
    if (bStatusUpdate[VDP_2] == 0) 
    #endif
    {
        if ((_u4ResetScposDramBuffer[VDP_2]) && ((_u4ResetScposDramBuffer[VDP_2] & 0x80000000) == 0))
        {
            _u4ResetScposDramBuffer[VDP_2]--;
            if (_u4ResetScposDramBuffer[VDP_2] == 0)
            {
                #if SUPPORT_POP    
                vRegWriteFldAlign(SCPIP_PIP2_09, 1, PIP2_09_WEN_RLH_2);    
                #endif
                if (_au4DelayWriteEnable[VDP_2])
                {
            //        vDrvScpipWriteCtrl(VDP_2, SV_ON);
                    _au4DelayWriteEnable[VDP_2] = 0;
                }                    
            }
        }

    }
#endif
    x_crit_end(csState);

#ifdef CC_SCPOS_WA10
    #ifdef CC_TV_MEMO_MODE
    if(!bTVMemoOnOff)
    #endif
    {
        #ifdef CC_SCPOS_WA14    
        if (bStatusUpdate[VDP_1] == 0) 
        #endif
        {
            vScalerWA10_UnlockImportProtection(VDP_1);
        }
    }
    #if SUPPORT_POP
    #ifdef CC_SCPOS_WA14    
    if (bStatusUpdate[VDP_2] == 0) 
    #endif
    {
        vScalerWA10_UnlockImportProtection(VDP_2);
    }
    #endif    
#endif
}


/**
* @brief Unlock scaler import protection
* @param u4VdpId VDP_1/VDP_2
*/
#ifdef CC_SCPOS_WA10
static void vScalerWA10_UnlockImportProtection(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT8 dsOffset;
    UINT8 pipOffset;
    RVChannel* channel;
    
    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    pipOffset = getPIPOffset(bPath);
    channel = getChannel(bPath);
    
    if (_au4FrameNumberChanged[u4VdpId])
    {
        if (_au4FrameNumberChanged[u4VdpId] == FRAME_NUMEBR_CHANGED_COUNT)
        {
            if (RegReadFldAlign(SCPIP_DS1_00+dsOffset, DS1_00_DSCALER_V_1) == 0x8000)
            {
                 _au4OriginalDSFactor[u4VdpId] = MAX_SCALER_FACTOR;
                 vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, 0x7fff, DS1_00_DSCALER_V_1);
            }
            else
            {
                 _au4OriginalDSFactor[u4VdpId] = RegReadFldAlign(SCPIP_DS1_00+dsOffset, DS1_00_DSCALER_V_1);
                 vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, _au4OriginalDSFactor[u4VdpId]+1, DS1_00_DSCALER_V_1);
            }            
            LOG(3, "VDP(%d), FrameNumber Changed! Modify LSB of scaling factor to reset import R_POS!\n", u4VdpId);
            LOG(3, "W_POS = %d\n", RegReadFldAlign(SCPIP_PIP1_RO_00+pipOffset, PIP1_RO_00_W_POS_1));
        }
        _au4FrameNumberChanged[u4VdpId] --;
        if (_au4FrameNumberChanged[u4VdpId] == 0)
        {
            // check current W_POS and recover DS factors
            if ((RegReadFldAlign(SCPIP_PIP1_RO_00+pipOffset, PIP1_RO_00_W_POS_1) > RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_FSEL_1)) && (channel->bIsChannelOn == SV_ON))
            {
                 // W_POS is still in wrong position
                 _au4FrameNumberChanged[u4VdpId] ++;
                 LOG(3, "VDP(%d) W_POS is still in wrong position: %d\n",u4VdpId, RegReadFldAlign(SCPIP_PIP1_RO_00+pipOffset, PIP1_RO_00_W_POS_1));
            }
            else
            {        
                if (_au4OriginalDSFactor[u4VdpId] == MAX_SCALER_FACTOR)
                {
                    vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, 0x8000, DS1_00_DSCALER_V_1);
                }
                else
                {
                    vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, _au4OriginalDSFactor[u4VdpId], DS1_00_DSCALER_V_1);
                }
                LOG(3, "VDP(%d), FrameNumber Changed! Recover original LSB of scaling factor!\n", u4VdpId);
                LOG(3, "W_POS = %d\n", RegReadFldAlign(SCPIP_PIP1_RO_00+pipOffset,PIP1_RO_00_W_POS_1));
            }
        }
    }
}
#endif 


/**
* @brief Switch DE on/off when entering scaler dispmode
*/
void vSwitchDEOnOff(void)
{
#if CC_SCPOS_VSYNC_WA2
    if (LVDS_MUTE_IN_DISPLAY_MODE == LVDS_MUTE_IN_DISPLAY_MODE_ON)
    {
        if (_u4UpdateDisplayModeState == 1)
        {
            LVDS_SwitchDataEnable(SV_FALSE);
            _u4UpdateDisplayModeState = 2;
        }
        else if (_u4UpdateDisplayModeState == 2)
        {
            _VDP_DispModeEnable(VDP_1, _fgDisplayModeEnable);
            _u4UpdateDisplayModeState = 3;
        }
        else if (_u4UpdateDisplayModeState >= 3)
        {
            // check if ddds lock or lock timeout
            if (_u4UpdateDisplayModeState == 22)
            {
                LVDS_SwitchDataEnable(SV_TRUE);
                _u4UpdateDisplayModeState = 0; // initilaze
            }
            else
            {
                _u4UpdateDisplayModeState++;
            }
        }
    }
#endif
}


/**
* @brief Enable/Disable scaler dispmode
* @param fgEnable On/Off
*/
void PMX_SetDisplayMode(BOOL fgEnable)
{
#if CC_SCPOS_VSYNC_WA2
    if (fgEnable)
    {
        _u4UpdateDisplayModeState = 1;
        _fgDisplayModeEnable = fgEnable;
    }
    else if (_fgDisplayModeEnable && (_u4UpdateDisplayModeState<3))
    {
        _u4UpdateDisplayModeState = 3;
        _fgDisplayModeEnable = fgEnable;
    }    
#endif
}


/**
* @brief Notify plane order changed
* @param u4VdpId VDP_1/VDP_2
* @param u4PlaneOrder
*/
void _VDP_NotifyPlaneOrderChanged(UINT32 u4VdpId, UINT32 u4PlaneOrder)
{
#if CC_SCPOS_WA_PMX_DEALY
    UINT32 u4Delay;

    if ((u4VdpId > 1) || (u4PlaneOrder > 3))
    {
        return;
    }
    
    
    if (u4PlaneOrder == 0)
    {
        u4Delay = 0;
    }
    else if (u4PlaneOrder == 1)
    {
        u4Delay = 0;
    }
    else if (u4PlaneOrder == 2)
    {
        u4Delay = 0;
    }
    else //if (u4PlaneOrder == 3)
    {
        u4Delay = 0;
    }

    if (u4PmxDelay[u4VdpId] != u4Delay)
    {
        UINT32 u4PipOffset;
        UINT8 bPath;
        RDispPrm* pDispPrm;

        bPath = getScalerPath(u4VdpId);
        u4PipOffset = getPIPOffset(bPath);
        pDispPrm = getDispPrm(bPath);

        u4PmxDelay[u4VdpId] = u4Delay;    
                
        // update MHP/SHP
        vRegWriteFldAlign(SCPIP_PIP1_03+u4PipOffset, ((pDispPrm->wRHPos) + u4PmxDelay[u4VdpId]), PIP1_03_RHP_1);
    }
    
#endif


}


/**
* @brief Enable/Disable channel preview mode
* @param fgEnable On/Off
*/
#ifdef CC_TV_MEMO_MODE
void _VDP_SetMemoMode(BOOL fgEnable)
{
    SRM_FREEZE_MODE_T eFreezeMode;
    
    eFreezeMode = SRM_GetFreezeMode(VDP_1);
    bMemoEnable = fgEnable;

    if (eFreezeMode == SRM_FREEZE_MODE_MPEG)
    {
        eFreezeMode = SRM_FREEZE_MODE_SCPOS;
    }
    
    if (eFreezeMode == SRM_FREEZE_MODE_SCPOS)
    {
        if (fgScposIsVideoFreezable(VDP_1) == SV_TRUE) // dram mode
        {
            if (fgEnable)
            {
                vDrvScpipWriteCtrl(VDP_1, SV_OFF); // freeze main window
                _rMChannel.bIsFreeze = SV_ON;
#if SUPPORT_POP
                vRegWriteFldAlign(SCPIP_PIP2_06, RegReadFldAlign(SCPIP_PIP2_05,PIP2_05_WSTARTP_2), PIP2_06_RSTARTP_2);
#endif
                bTVMemoOnOff = SV_ON;
            }
            else
            {
               if (bDispmode)
               {
                    _VDP_DispModeEnable(SV_VP_MAIN, SV_ON); // entering dispmode
                    #ifdef CC_MT5391
                        vDrvMJCOnOff(SV_ON); // enable MJC
                    #endif
                    bDispmode = FALSE;
                    u4Counter = 5;
                    bDisable = TRUE;
               }
               else
               {
                   bTVMemoOnOff = SV_OFF;
                   _rMChannel.bIsFreeze = SV_OFF;
                }
            }
        }
        else // scaler dispmode
        {
            #ifdef CC_MT5391
                vDrvMJCOnOff(SV_OFF); // disable MJC
            #endif
            _VDP_DispModeEnable(SV_VP_MAIN, SV_OFF); // force entering dram
            bDispmode = TRUE;
            u4Counter = 5;
            bDisable = TRUE;
        }
    }
    else
    {
        if (eFreezeMode == SRM_FREEZE_MODE_MDDI) // pscan dispmode
        {
            vDrvDIFreezeOnOff(VDP_1, fgEnable);
        }
        
        #ifdef CC_MT5391
        else if (eFreezeMode == SRM_FREEZE_MODE_MJC) // mjc dispmode
        {
            vDrvMJCFreeze(fgEnable);
        }
        #endif
        #if 0
        else if (eFreezeMode == SRM_FREEZE_MODE_MPEG)
        {
            VDP_FrcSetFreeze(VDP_1, fgEnable);
        }
        #endif
        bTVMemoOnOff = fgEnable;
    }
}
#endif


#ifdef CC_COPLAT_MT82
UINT8  _bBlankMuteCnt;

UINT8 bDrvIsScalerFlagCleared(UINT8 bPath)
{
	if(bPath == SV_VP_MAIN){
		if(fgIsScposFlgSet(SCALE_PRM_CHG | MAIN_DISP_PRM_CHG | SCALER_MAIN_MODE_CHG))
		{
			return SV_FALSE;
		}
		else
		{
			return SV_TRUE;
		}
	}
#if SUPPORT_POP
	else
	{
		if(fgIsScposFlgSet(SCALE_PRM_CHG | PIP_DISP_PRM_CHG | SCALER_PIP_MODE_CHG))
		{
			return SV_FALSE;
		}
		else
		{
			return SV_TRUE;
		}		
	}
#endif
	
}

void vDrvSetFastBlank(UINT8 bOnOff, UINT8 bCount)
{
    if(bOnOff == SV_ON && bCount > 0)
    {
    	if(bCount > _bBlankMuteCnt)
    	{
        	_bBlankMuteCnt = bCount;
        }
        vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_FAST_BLANK);
    }
    else
    {
        _bBlankMuteCnt = 0;
        vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_FAST_BLANK);
    }
}

/**
 * @brief  to know if video can be freezed
 * @param  bPath path info.(SV_VP_MAIN/SV_VP_PIP)
 * @retval void
 */
UINT8 bIsVideoFreezable(UINT8 bPath)
{
    UINT8 mode;
    mode = getScalerMode(bPath);
    if((mode==VDP_SCPOS_DISPMODE_AUTO)||(mode==VDP_SCPOS_PSCAN_DISPMODE))
    {
        return SV_FALSE;
    }
    else
    {
    	 UINT8 bTiming = bDrvVideoGetTiming(bPath);
	//DTV00121401	The video can be freezed in PIP/POP sub window in HDMI/YPbPr 1080P source.
	 if ((bTiming == MODE_1080p_50) || (bTiming == MODE_1080p_60))
	 {
	 	return SV_FALSE;
	 }
	 else
	 {
	        return SV_TRUE;
	 }
    }
}

#else  //#ifdef CC_COPLAT_MT82

static void vScpipSRMCalFrameDealy(UINT32 u4VdpId, UINT32 u4Mode)
{
    UINT32 u4Delay=0;
    
    if((u4Mode & FBM_POOL_MODE_SCPOS_DISP) || (u4Mode&FBM_POOL_MODE_SCPOS_PSCAN_DISP) || (u4Mode &FBM_POOL_MODE_SCPOS_MJC_DISP))
    {
        u4Delay= 0;
    }
    else //Scaler dram mode
    {
        if(u4Mode & FBM_POOL_MODE_SCPOS_2FB)
        {
            u4Delay= 2;  //delay 1 frame
        }
        else
        {
            u4Delay= 3; // delay 1.5 frame
        }

        if(bScpipFlipEnable[u4VdpId])
             u4Delay+=0;  // need to change to +=2 if flip use extra 1 more buf.
    }

    _u4SRMFrameDelay[u4VdpId]=u4Delay;
}

UINT32 u4ScpipSRMGetFrameDelay(UINT32 u4VdpId)
{
    return _u4SRMFrameDelay[u4VdpId];
}

/**
 *  scaler control call back
 *
 * @param void
 * @return void
 */
void vDrvScpipSrmDispCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode)
{
    #ifdef CC_SCPOS_EN
    FBM_POOL_T* prPool;
    FBM_POOL_T* prMddiPool;
    #endif

    LOG(3, "[SRM] DISP CTRL vdp%d mode=0x%x, pds=%d, sram=%d\n", u4VdpId, u4Mode, u4PDSize, u4SramMode);


    #ifdef CC_SCPOS_EN

    vScpipSRMCalFrameDealy(u4VdpId, u4Mode);

    if (u4VdpId == VDP_1)
    {
        if (u4Mode & FBM_POOL_MODE_UNSTABLE)
        {
            LOG(3, "[SRM] Unstable callback! Disable Dispmode!\n");
            _VDP_DispModeEnable((UINT8)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
        }
        #if 0 // defined(CC_MT5360) && !defined(CC_MT5360B)  //MT5360A
        VDP_SetSramPool();
        #endif
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_MAIN, NULL);
        prMddiPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_MAIN, NULL);

    }
    else
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_PIP, NULL);
        prMddiPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_PIP, NULL);
    }
    
    if (u4Mode & FBM_POOL_MODE_UNSTABLE)
    {
		_VDP_vDrvForceWriteOff(u4VdpId, SV_ON);    
		vDrvScpipWriteCtrl(u4VdpId, SV_OFF);
		bSRMStable[u4VdpId]=SV_FALSE;
    }
    if ((prPool != NULL) && 
        (((u4Mode & FBM_POOL_MODE_UNSTABLE) == 0) || (u4Mode & FBM_POOL_MODE_SCPOS_MM_BUFCHG)))
    {
        #if 0
        _VdpSetScalerBuffer(u4VdpId, prPool->u4Addr, prPool->u4Width, prPool->u4Height, prMddiPool->u4Width, prPool->u4Size,
        (u4Mode & FBM_POOL_MODE_SCPOS_2FB) > 0,
        (u4Mode & FBM_POOL_MODE_10BIT) > 0,
        (u4Mode & FBM_POOL_MODE_8BIT) > 0,
        (u4Mode & FBM_POOL_MODE_422_FB) > 0);

        #else
        UINT32 u4Address;
        UINT32 u4Width, u4Height, u4MddiWidth, u4FbmSize;
        SCALER_FBM_POOL_T rScalerDramInfo;

        u4Address = prPool->u4Addr;
        u4Width = prPool->u4Width;
        u4Height = prPool->u4Height;
        u4MddiWidth = prMddiPool->u4Width;
        u4FbmSize = prPool->u4Size;

        rScalerDramInfo.u4Addr = u4Address;
        rScalerDramInfo.u4Width = u4Width;
        rScalerDramInfo.u4Height = u4Height;
        if (prMddiPool->u4Mode & FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR)
        {
            rScalerDramInfo.u4MddiWidth = SCPOS_BYPASS_PSCANNR;
        }
        else
        {
            rScalerDramInfo.u4MddiWidth = u4MddiWidth;
        }
        rScalerDramInfo.u4FbmSize = u4FbmSize;
        rScalerDramInfo.fg2FB = ((u4Mode & FBM_POOL_MODE_SCPOS_2FB) > 0)? 1: 0;
        rScalerDramInfo.fg10bit = ((u4Mode & FBM_POOL_MODE_10BIT) > 0)? 1: 0;
        rScalerDramInfo.fg8bit = ((u4Mode & FBM_POOL_MODE_8BIT) > 0)? 1: 0;

        LOG(3, "[SRM Callback]Set Scaler Buffer: a=0x%x, w=%d, h=%d, 2FB=%d, 10bit=%d, DIWidth=%d\n", 
            u4Address, u4Width, u4Height, (UINT32)rScalerDramInfo.fg2FB, (UINT32)rScalerDramInfo.fg10bit, rScalerDramInfo.u4MddiWidth);
        
        rScalerDramInfo.fg422 = ((u4Mode & FBM_POOL_MODE_422_FB) > 0)? 1: 0;
        vScpipSrmSetBuf(u4VdpId,&rScalerDramInfo);
        bSRMStable[u4VdpId]=SV_TRUE;
        #endif
		_VDP_vDrvForceWriteOff(u4VdpId, SV_OFF);
    }
    else
    {
        if (prPool == NULL)
        {
            LOG(1, "_VdpSrmScalerWeCb: fail to get pool\n");
        }
    }

    if (u4Mode & FBM_POOL_MODE_UNSTABLE)
    {
        return;
    }

    #ifdef CC_FLIP_MIRROR_SUPPORT
    if (u4Mode & FBM_POOL_MODE_SCPOS_FULL)
    {
        _vScalerConfigFlip(u4VdpId, SV_TRUE);
    }
    else
    {
        _vScalerConfigFlip(u4VdpId, SV_FALSE);
    }
    #endif

    // u4Arg3 is pre-scale size
    if (u4Mode & FBM_POOL_MODE_SCPOS_FULL)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
        if (bVideoUpdateSrcRegion(u4VdpId) != 1)
        {
        }
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_AUTO);
        if (bVideoUpdateSrcRegion(u4VdpId) != 1)
        {
        }
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_PSCAN_DISPMODE);
        if (bVideoUpdateSrcRegion(u4VdpId) != 1)
        {
        }
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_MJC_DISP)
    {
        #if 0 // def CC_MT5392B
        _arVdpConf[VDP_1].ucDispMode = VDP_SCPOS_MJC_DISPMODE;

        _VDP_DispModeEnable((UCHAR)u4Arg1, (UCHAR)VDP_SCPOS_MJC_DISPMODE);

        if (bApiVideoSetSrcOverScan((UINT8)u4Arg1,
            _arVdpConf[u4Arg1].rOverScan.u4Top,
            _arVdpConf[u4Arg1].rOverScan.u4Bottom,
            _arVdpConf[u4Arg1].rOverScan.u4Left,
            _arVdpConf[u4Arg1].rOverScan.u4Right) != 1)
        {
        }
        #endif
    }
    else
    {
        //// TODO
    }
    #endif
}
#endif//#ifdef CC_COPLAT_MT82

/**
 * @brief vDrvSetScalarFreeze
 
 * Freeze Video in scalar
 
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         bOnOff: SV_ON/SV_OFF
 * @retval void
 */
void vDrvSetScalarFreeze(UINT8 bPath, UINT8 bOnOff) 
{
    CRIT_STATE_T csState;
    csState = x_crit_start();

    // Use Main/Pip Memory Write Enable to Perform Freeze
//#ifndef __MODEL_slt__    
#if 1
    if (bPath == SV_VP_MAIN) 
    {

        #if (MAIN_FREEZE_DELAY)
        {
            if(bOnOff)
            {
                _rMChannel.bIsGoingToFreeze = MAIN_FREEZE_DELAY;
            }
            else
            {
                _rMChannel.bIsGoingToFreeze = 0;
                _rMChannel.bIsFreeze = 0;
            }            
        }
        #else
        {
            _rMChannel.bIsFreeze = bOnOff;
        }
        #endif
    } 
#if SUPPORT_POP
    else if (bPath == SV_VP_PIP) 
    {
        #if (PIP_FREEZE_DELAY)
        {
            if(bOnOff)
            {
                _rPChannel.bIsGoingToFreeze = PIP_FREEZE_DELAY;
            }
            else
            {
                _rPChannel.bIsGoingToFreeze = 0;
                _rPChannel.bIsFreeze = 0;
            }
        }
        #else
        {
            _rPChannel.bIsFreeze = bOnOff;
        }
        #endif
    }
#endif
#else
    if (bPath == SV_VP_MAIN) 
    {
        _rMChannel.bIsFreeze = bOnOff;
    } 
    else 
    {
        _rPChannel.bIsFreeze = bOnOff;
    }
#endif

  //  vDrvAutoContrastFreeze(_rMChannel.bIsFreeze ||_rPChannel.bIsFreeze || (_rMChannel.bIsGoingToFreeze > 0) || (_rPChannel.bIsGoingToFreeze > 0));
    x_crit_end(csState);
}


void vDrvSetDSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4HTotal, UINT32 u4Width, UINT32 u4VTotal, UINT32 u4Height)
{
    UINT8 bPath;
    UINT8 dsOffset;
  //  UINT8 pipOffset;
  //  RVChannel* channel;

    if (u4VdpId >= VDP_NS) 
    { 
        return;
    }
   
    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
//  pipOffset = getPIPOffset(bPath);
  //  channel = getChannel(bPath);

    if (u4OnOff == VDP_DS_PTN_AUTO)
    {
        UINT32 u4SrcWidth, u4SrcHeight, u4SrcXOffset, u4SrcYOffset;
        UINT32 u4YUV;
        
        u4SrcWidth = _pPicInfo[u4VdpId]->wTargetW;
        u4SrcHeight = _pPicInfo[u4VdpId]->wTargetH;
        #if NEW_5387_SCPOS_PORCH
        u4SrcXOffset = getMixedHPorch(u4VdpId);
        u4SrcYOffset = getMixedVPorch(u4VdpId);
        #else    
        u4SrcXOffset = _pPicInfo[u4VdpId]->wXOffset;
        u4SrcYOffset = _pPicInfo[u4VdpId]->wYOffset;
        #endif

        u4SrcWidth = DIVIDE_WITH_CARRIER(u4SrcWidth * _VDP_GetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR);
        u4SrcXOffset = (u4SrcXOffset * _VDP_GetHorizontalPreScaleDownFactor(u4VdpId)) / DFT_HPD_FACTOR;

        LOG(3, "VDP(%d), SRC after HPS (%d, %d, %d, %d)\n",  u4VdpId, u4SrcXOffset, u4SrcYOffset, u4SrcWidth, u4SrcHeight);

        u4HTotal = wDrvVideoGetHTotal(bPath);
        u4VTotal = wDrvVideoGetVTotal(bPath);
            
        LOG(3, "VDP(%d), HTotal/VTotal (%d, %d)\n",  u4VdpId, u4HTotal, u4VTotal);

        u4Width =  u4SrcWidth + u4SrcXOffset;
        u4Height =  u4SrcHeight + u4SrcYOffset;

        u4YUV = (bIsScalerInput444(bPath)? 0: 1);
        
        vRegWriteFldAlign(SCPIP_DS1_03+dsOffset, u4Width, DS1_03_PATGEN_H_ACTIVE_1);
        vRegWriteFldAlign(SCPIP_DS1_03+dsOffset, u4HTotal, DS1_03_PATGEN_H_TOTAL_1);
        vRegWriteFldAlign(SCPIP_DS1_04+dsOffset, u4Height, DS1_04_PATGEN_V_ACTIVE_1);
        vRegWriteFldAlign(SCPIP_DS1_04+dsOffset, u4VTotal, DS1_04_PATGEN_V_TOTAL_1);

        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, u4YUV, DS1_02_PTCOLOR_1);

        // disable frame track
        u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_DS_PATTERN;

        // turn on pattern
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_PTGEN_1);
    }
    else if (u4OnOff == VDP_DS_PTN_MANUAL)
    {
        vRegWriteFldAlign(SCPIP_DS1_03+dsOffset, u4Width, DS1_03_PATGEN_H_ACTIVE_1);
        vRegWriteFldAlign(SCPIP_DS1_03+dsOffset, u4HTotal, DS1_03_PATGEN_H_TOTAL_1);
        vRegWriteFldAlign(SCPIP_DS1_04+dsOffset, u4Height, DS1_04_PATGEN_V_ACTIVE_1);
        vRegWriteFldAlign(SCPIP_DS1_04+dsOffset, u4VTotal, DS1_04_PATGEN_V_TOTAL_1);

        // disable frame track
         u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_DS_PATTERN;

        // turn on pattern
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_PTGEN_1);
    }
    else // off
    {
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_PTGEN_1);
        u4ForceFrameTrackCtrl &= (~SCPIP_FRAME_TRACK_OFF_DS_PATTERN);    
    }
}


void vDrvSetUSPattern(UINT32 u4VdpId, UINT32 u4OnOff, UINT32 u4Width, UINT32 u4Height)
{
    UINT8 bPath;
    UINT32 u4PipOffset;
    UINT32 u4Offset;
  //  RVChannel* channel;

    if (u4VdpId >= VDP_NS) 
    { 
        return;
    }

    if (u4VdpId == VDP_1)
    {
        u4Offset = 0;
    }
    else
    {
        u4Offset = 4;
    }
    bPath = getScalerPath(u4VdpId);
    u4PipOffset = getPIPOffset(bPath);

    if ((u4OnOff == VDP_US_PTN_AUTO) || (u4OnOff == VDP_US_PTN_MANUAL))
    {
        UINT32 u4PreLen; //, u4YOfst;

        if (u4OnOff == VDP_US_PTN_AUTO)        
        {
            u4Width = _pPicInfo[u4VdpId]->wTargetW;
            u4Height = _pPicInfo[u4VdpId]->wTargetH;

            u4Width = DIVIDE_WITH_CARRIER(u4Width * _VDP_GetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR);
            u4Width = RSHIFT_WITH_CARRIER((u4Width * _pDispPrm[u4VdpId]->u4DSclH), DFT_DS_BIT_NUM);
            u4Height = RSHIFT_WITH_CARRIER((u4Height * _pDispPrm[u4VdpId]->u4DSclV), DFT_DS_BIT_NUM);

            LOG(3, "VDP(%d), Src after DS(%d, %d)\n", u4Width, u4Height);
        }

        u4PreLen = RegReadFldAlign(SCPIP_PIP1_0B+u4PipOffset, PIP1_0B_VS_PSCAN_PRELEN_1);
        u4PreLen = u4PreLen << 8;
        u4PreLen += RegReadFldAlign(SCPIP_PIP1_0C+u4PipOffset, PIP1_0C_VS_PSCAN_PRELEN_1);
  //      u4YOfst = RegReadFldAlign(SCPIP_PIP1_02+u4PipOffset, PIP1_02_RVP_1);
            
        //LOG(3, "VDP(%d), PreLen/RVP (%d, %d)\n",  u4VdpId, u4PreLen, u4YOfst);
            
        vRegWriteFldAlign(SCPIP_PIP_OA_04+u4Offset, u4Width, PIP_OA_04_MAIN_PAT_HTOTAL);
        vRegWriteFldAlign(SCPIP_PIP_OA_04+u4Offset, u4Height, PIP_OA_04_MAIN_PAT_VTOTAL);
        vRegWriteFldAlign(SCPIP_PIP_OA_04+u4Offset, u4PreLen+1, PIP_OA_04_MAIN_PAT_V_START);

        // turn on pattern
        if (u4VdpId == VDP_1)
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_MAIN_PAT_EN);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_SUB_PAT_EN);
        }
    }
    else // off
    {
        if (u4VdpId == VDP_1)
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_MAIN_PAT_EN);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_SUB_PAT_EN);
        }
    }
}

#if !REMOVE_UNCALLED_FUNCTION
UINT8 bGetScalerFrameNum(UINT8 bPath)
{
    UINT8 bDramMode=getScalerMode(VDP_1);

    if(bDramMode == VDP_SCPOS_DISPMODE_OFF)
    {
    	return (UINT8)(u4ScpipGetBufNum(VDP_1));
    }
    else
    	return 0; // scaler no delay when display mode
}
#endif
void vScpipOnInputVSync(UINT8 bPath)
{
    if (bPath >= SV_VP_NA)
    {
        return;
    }

    if (bPath == SV_VP_MAIN) 
    { 
        #ifdef CC_SCPOS_WA33
        if (_au4ImportTrigger[VDP_1])
        {
            UINT32 u4FrameIdx = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1);
            LOG(4, "WA33: W_POS %d\t _au4ImportTrigger : 0x%X\n", u4FrameIdx, _au4ImportTrigger[VDP_1]);
            if ((_au4ImportTrigger[VDP_1] & 0xfff0) == 0)
            {
                if (u4FrameIdx == 1)
                {
                    _au4ImportTrigger[VDP_1] = 0;
                    vDrvMainImportOnOff(SV_ON);
                }
                else
                {
                    _au4ImportTrigger[VDP_1] ++;
                }

                if ((_au4ImportTrigger[VDP_1] & 0xf) > 4)
                {
                    // give up workaround        
                    _au4ImportTrigger[VDP_1] = 0;
                    if(u4FrameIdx == 0)
                     {
                       vRegWriteFldAlign(SCPIP_PIP1_05, 1, PIP1_05_REG_WEN_SEL_1);
                     }
                     else
                     {
                         vRegWriteFldAlign(SCPIP_PIP1_05, 0, PIP1_05_REG_WEN_SEL_1);
                     }
                    vDrvMainImportOnOff(SV_ON);
                }
            }
       }
       #endif
    }
#if SUPPORT_POP
    else // sub
    {
        #ifdef CC_SCPOS_WA27
        if(_u4VdsVusDealy)
        {
            if(_u4VdsVusDealy>1)
                _u4VdsVusDealy--;
            else  //_u4VdsVusDealy==1
            {
                // Vds_bypass and Vus_bypass cannot be 0 at the same time,
                // Therefore, check the bypass status and change the bypass order.
                if(_u4VUSBypass[bPath])
                {
                    if (_u4UpdateFifoConfig & SCPIP_UPDATE_PIP_VUS)
                    {
                        _vScpipSetBypassUpScaler(bPath, _u4VUSBypass[bPath]);    
                        _u4UpdateFifoConfig &= (~SCPIP_UPDATE_PIP_VUS);
                    }
                    if (_u4UpdateFifoConfig & SCPIP_UPDATE_PIP_VDS)
                    {
                        _vScpipSetBypassDownScaler(bPath, _u4VDSBypass[bPath]);    
                        _u4UpdateFifoConfig &= (~SCPIP_UPDATE_PIP_VDS);
                    }
                }
                else
                {
                    if (_u4UpdateFifoConfig & SCPIP_UPDATE_PIP_VDS)
                    {
                        _vScpipSetBypassDownScaler(bPath, _u4VDSBypass[bPath]);    
                        _u4UpdateFifoConfig &= (~SCPIP_UPDATE_PIP_VDS);
                    }
                    if (_u4UpdateFifoConfig & SCPIP_UPDATE_PIP_VUS)
                    {
                        _vScpipSetBypassUpScaler(bPath, _u4VUSBypass[bPath]);    
                        _u4UpdateFifoConfig &= (~SCPIP_UPDATE_PIP_VUS);
                    }
                }
                _u4VdsVusDealy=0;
            }
        }

        #endif

        #ifdef CC_TVE_US_IMAGERESIZER_WA
        if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5363_64MBx2)
        {
            // check if TVE is turnned on
            UINT8 ucEnable;
            UINT32 u4ResizeSubDRAM = 0;
            TVE_GetEnable(TVE_1, &ucEnable);
            if (ucEnable)
            {
                UINT8 ucFmt;
                UINT32 u4TargetHeight = 480;
                TVE_GetFmt(TVE_1, &ucFmt);
                if (ucFmt == TVE_FMT_480I)
                {
                    u4TargetHeight = 480;    
                }
                else
                {
                    u4TargetHeight = 576;
                }
                // check input height
                if ((bGetVideoDecType(SV_VP_PIP) == SV_VD_MPEGHD) 
                    && (_pPicInfo[SV_VP_PIP]->wTargetH < u4TargetHeight) 
                    && (RegReadFldAlign(SCPIP_PIP2_09, PIP2_09_W_EN_2))
                    && (wDrvVideoInputHeight(SV_VP_PIP) <= 576))
                {
                    u4ResizeSubDRAM = u4TargetHeight;
                }
                else
                {
                    u4ResizeSubDRAM = 0;
                }
            }

            if (_u4ResizeSubDRAM != u4ResizeSubDRAM)
            {
                _u4ResizeSubDRAM = u4ResizeSubDRAM;
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_WA, VDP_2, VDP_SCPOS_DELAY_UNMUTE_TIME, TRUE);

                if (u4ResizeSubDRAM)
                {
                    LOG(3, "CC_TVE_US_IMAGERESIZER_WA on, Resize sub to %d --> %d\n", _pPicInfo[SV_VP_PIP]->wTargetH, u4ResizeSubDRAM);   
                    // start control read pointer
                    vRegWriteFldAlign(SCPIP_PIP2_09, 0, PIP2_09_SET_W_POS_2);
                    vRegWriteFldAlign(SCPIP_PIP2_09, 1, PIP2_09_SET_W_POS_EN_2);            
                }
                else
                {
                    LOG(3, "CC_TVE_US_IMAGERESIZER_WA off\n");
                    // stop control read pointer
                    vRegWriteFldAlign(SCPIP_PIP2_09, 0, PIP2_09_SET_W_POS_EN_2);            
                }
                // re-config SVL and PIP2_06_RSTARTP_2
                 _u4UpdateDRAMReadAddr = 1;
                 _VDP_ScalerReconfig(SV_VP_PIP);
            }

            if (_u4UpdateDRAMReadAddr || ((u4ResizeSubDRAM)&& (RegReadFldAlign(SCPIP_PIP2_09, PIP2_09_W_EN_2)) && (wDrvVideoInputHeight(SV_VP_PIP) <= 576)))
            {
                UINT32 u4SrcAddr;
                UINT32 u4Ofst;
                UINT32 u4DesAddr;  
                UINT32 u4Width;
            
                // 1. get current write position
                _u4ResizeWPos = RegReadFldAlign(SCPIP_PIP2_10, PIP2_10_W_POS_2);
                
                if (_u4ResizeWPos == 0)
                {
                    _u4ResizeWPos = RegReadFldAlign(SCPIP_PIP2_09, PIP2_09_FSEL_2);
                }
                else
                {
                    _u4ResizeWPos = _u4ResizeWPos - 1;
                }

                u4SrcAddr = RegReadFldAlign(SCPIP_PIP2_05, PIP2_05_WSTARTP_2);
                u4Ofst = RegReadFldAlign(SCPIP_PIP2_07, PIP2_07_WINC_2);

                u4SrcAddr += (u4Ofst * _u4ResizeWPos);
                u4DesAddr = u4SrcAddr + (u4Ofst * 3);

                u4SrcAddr = u4SrcAddr << 4;
                u4DesAddr = u4DesAddr << 4;

                u4Width = RegReadFldAlign(SCPIP_PIP2_01, PIP2_01_HL_2);
                u4Width = u4Width * 16;

                if ((u4SrcAddr > 0) && (u4DesAddr > 0) &&  (u4Width > 0) &&  (_pPicInfo[SV_VP_PIP]->wTargetH > 0))   
                {
                    // 2. set up image resizer
                    _rResizerScaleParam.u4SrcBase = u4SrcAddr;
                    _rResizerScaleParam.u4IsSrcDirCM = 1;
                    _rResizerScaleParam.u4SrcCM = E_RZ_OSD_DIR_CM_AYCbCr8888;
                    _rResizerScaleParam.u4SrcHOffset = 0;
                    _rResizerScaleParam.u4SrcVOffset = 0;
                    _rResizerScaleParam.u4SrcW = u4Width;
                    _rResizerScaleParam.u4SrcH = _pPicInfo[SV_VP_PIP]->wTargetH;
                    _rResizerScaleParam.u4SrcBufLen = u4Width;

                    _rResizerScaleParam.u4TgBase = u4DesAddr;
                    _rResizerScaleParam.u4TgCM = E_RZ_OSD_DIR_CM_AYCbCr8888;  
                    _rResizerScaleParam.u4TgHOffset = 0;
                    _rResizerScaleParam.u4TgVOffset = 0;
                    _rResizerScaleParam.u4TgW = u4Width;
                    _rResizerScaleParam.u4TgH = u4ResizeSubDRAM;
                    _rResizerScaleParam.u4TgBufLen = u4Width;

                    _rResizerScaleParam.pu4CPTData = 0;
                    _rResizerScaleParam.u4CPTDataCount = 0;
                
                    //trigger image resizer thread    
                    x_sema_unlock(h_enableImageResizerSema);
                }
            }
        }
        #endif
    }
    #endif //SUPPORT_POP

}

static void vUpdateNonlinearUSPrm(UINT8 ucVdpId)
{
    UINT32 u4EndFactor = 0;
    UINT32 u4SideRatio = 0;
    UINT32 u4StopPoint = 0;
    UINT32 u4IncreaseFactor = 0;
    UINT32 u4StartFactor = 0;
    //UINT32 Trigger = 0;

    UINT32 u4InputWidth = _pPicInfo[ucVdpId]->wTargetW;

    UINT32 u4InputMidWidth ;
    UINT32 u4OutputMidWidth ;

    if (PANEL_GetPanelWidth() != 1920)
    {
        u4InputMidWidth = 50;
        u4OutputMidWidth = 70;
    }
    else
    {
        u4InputMidWidth = 50;
        u4OutputMidWidth = 100;
    }

    if (_arScalePrm[ucVdpId].u4NonlinearCtrl == SV_TRUE)
    {
        // 121-map & nonlinear scaling can NOT enable together
        if (_arScalePrm[ucVdpId].u4NonlinearEn)
        {
            _arScalePrm[ucVdpId].u4121Map = 0;
        }
        if ((PANEL_GetPanelWidth() == 0) || (u4InputWidth == u4InputMidWidth) || (u4InputMidWidth == 0))
        {
            _arScalePrm[ucVdpId].u4NonlinearEn = 0;
            _arScalePrm[ucVdpId].u4NonlinearCtrl  = 0;
            return;
        }
        u4EndFactor = (u4InputMidWidth * 32768) / u4OutputMidWidth;
        u4SideRatio = ((u4InputWidth - u4InputMidWidth) * 32768) / (PANEL_GetPanelWidth() - u4OutputMidWidth);

        if (u4EndFactor <= u4SideRatio)
        {
            _arScalePrm[ucVdpId].u4NonlinearEn = 0;
            _arScalePrm[ucVdpId].u4NonlinearCtrl  = 0;
            Printf("The upscaling factor of middle part larger than side part\n");
            return;
        }
        u4StopPoint = (PANEL_GetPanelWidth() - 50) / 2;
        u4IncreaseFactor = (PANEL_GetPanelWidth() * u4EndFactor - 32768 * u4InputWidth);
        u4IncreaseFactor = u4IncreaseFactor / (u4StopPoint * u4StopPoint);//a=(OE-32768I)/(stop_point.^2)
        u4StartFactor = u4EndFactor - u4IncreaseFactor * u4StopPoint;//S=E-a*stop_point
        u4EndFactor = (u4IncreaseFactor * u4StopPoint * u4StopPoint + 32768 * u4InputWidth) / PANEL_GetPanelWidth();
        u4StartFactor = u4EndFactor - u4IncreaseFactor * u4StopPoint;
        if ( u4StartFactor > 0x8000)
        {
            _arScalePrm[ucVdpId].u4NonlinearEn = 0;
            _arScalePrm[ucVdpId].u4NonlinearCtrl  = 0;
            Printf("Start Factor more than 0x8000!\n");
            return;
        }
        if (u4InputMidWidth > u4OutputMidWidth)
        {
            Printf("Nonlinear not support down scaler mode\n");
            _arScalePrm[ucVdpId].u4NonlinearEn = 0;
            _arScalePrm[ucVdpId].u4NonlinearCtrl  = 0;
            return;
        }

        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4MiddleWidth = PANEL_GetPanelWidth() / 2;
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4StopPoint = u4StopPoint;
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4EndFactor = u4EndFactor;
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4IncreaseFactor = u4IncreaseFactor;
        _arScalePrm[ucVdpId].rNewNonlinearPrm.u4StartFactor = u4StartFactor;
       // _arScalePrm[ucVdpId].u4NonlinearAuto = FALSE;

    }

}

#if SUPPORT_DOT_TO_DOT_PATTERN
void VDP_DotToDotPattern(UINT8 ucOnOff,UINT32 u4RGBColor)
{	     
	static UINT8 _uDisableFunction=0;

       if((_uDisableFunction ==0) && (ucOnOff==0))
        return;

        _uDisableFunction = ucOnOff;
        
       if(ucOnOff == 1)
	{
        UINT32 TempAddress;
		UINT16 bDate_Length;
		UINT16 VerticalCounter=0;
		UINT16 HorizontalConuter=0;
		
		INT8 bTemp = 15;
		UINT8 uGValue;
		UINT8 uBValue;
		UINT8 uRValue;
		UINT8 bPackCount=0;
		
		uGValue=(UINT8)(u4RGBColor&0x000000ff);
		uBValue=(UINT8)((u4RGBColor>>8)&0x000000ff);
		uRValue=(UINT8)((u4RGBColor>>16)&0x000000ff);
		bDate_Length=(PANEL_GetPanelWidth()*3%16)?(PANEL_GetPanelWidth()*3/16+ 1):(PANEL_GetPanelWidth()*3/16+0);
		_rMChannel.bIsFreeze=1;
		bCount++;
		_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1, FOREVER_MUTE, TRUE);
	      vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_W_EN_1);
		TempAddress= (RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1))<<4;
		
		if(bCount==1)
		{
		    bOriEN3D = RegReadFldAlign(C_COMB_2A, EN3D); 
			bOriDSIs444Mode=RegReadFldAlign(SCPIP_DS1_02, DS1_02_SEL444_1); 
			bOri10BitMode = RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
			bOriUsH= RegReadFldAlign(SCPIP_US1_00, US1_00_USCALE_H_1);
			bOriUsV= RegReadFldAlign(SCPIP_US1_00, US1_00_USCALE_V_1);
			bOriVUSisBypass= RegReadFldAlign(SCPIP_US1_01, US1_01_VUS_BYPASS_1);
			bOriDataLen= RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1);
			bOriHL  = RegReadFldAlign(SCPIP_PIP1_01, PIP1_01_HL_1);
			bOriVL= RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1);
			bOriRHP  = RegReadFldAlign(SCPIP_PIP1_03, PIP1_03_RHP_1);
			bOriRHL  = RegReadFldAlign(SCPIP_PIP1_03, PIP1_03_RHL_1);
			bOriRVP  = RegReadFldAlign(SCPIP_PIP1_02, PIP1_02_RVP_1);	
			bOriRVL  = RegReadFldAlign(SCPIP_PIP1_02, PIP1_02_RVL_1);
			bOriFSEL_1 = RegReadFldAlign(SCPIP_PIP1_09, PIP1_09_FSEL_1);
			bOriVOFST_BYPASS = RegReadFldAlign(SCPIP_PIP_OA_03, PIP_OA_03_US_VOFST_BYPASS1);
			bOriALLBP = RegReadFldAlign(SCE_CFG_MAIN, ALLBP);
			bOriYUV2RGB = RegReadFldAlign(OS_84, YUV2RGB_BYPASS_MAIN);	
		}
	       vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_FSEL_1);
		vScposSetScalerDispmode(SV_VP_MAIN,SV_OFF);
		vScposSetPSCANDispmode(SV_VP_MAIN,SV_OFF);
		vRegWriteFldAlign(C_COMB_2A, SV_OFF, EN3D);
		vRegWriteFldAlign(SCPIP_DS1_02, 1, DS1_02_SEL444_1);
		vRegWriteFldAlign(SCPIP_DS1_02, 0, DS1_02_VD10BSEL_1);
		vRegWriteFldAlign(SCPIP_US1_00, 0x8000, US1_00_USCALE_H_1);
		vRegWriteFldAlign(SCPIP_US1_00, 0x8000, US1_00_USCALE_V_1); 
		vRegWriteFldAlign(SCPIP_US1_01, 1, US1_01_VUS_BYPASS_1);
		vRegWriteFldAlign(SCPIP_PIP1_08, bDate_Length, PIP1_08_DATA_LENGTH_1);
		vRegWriteFldAlign(SCPIP_PIP1_01, bDate_Length, PIP1_01_HL_1);
		vRegWriteFldAlign(SCPIP_PIP1_00, PANEL_GetPanelHeight(), PIP1_00_VL_1);
		vRegWriteFldAlign(SCPIP_PIP1_03, 0, PIP1_03_RHP_1);
		vRegWriteFldAlign(SCPIP_PIP1_03, PANEL_GetPanelWidth(), PIP1_03_RHL_1);
		vRegWriteFldAlign(SCPIP_PIP1_02, 0, PIP1_02_RVP_1);
		vRegWriteFldAlign(SCPIP_PIP1_02, PANEL_GetPanelHeight(), PIP1_02_RVL_1);
		vRegWriteFldAlign(SCE_CFG_MAIN, 1, ALLBP);// bypass SCE
		vRegWriteFldAlign(OS_84, 1, YUV2RGB_BYPASS_MAIN);
		vRegWriteFldAlign(SCPIP_PIP_OA_03, 0, PIP_OA_03_US_VOFST_BYPASS1);

		for(VerticalCounter = 0; VerticalCounter < PANEL_GetPanelHeight()*2; VerticalCounter ++)//Write Dram Data
		{
			bTemp = 15;
			for(HorizontalConuter = 0; HorizontalConuter < bDate_Length; HorizontalConuter ++)
			{
				switch(HorizontalConuter%3)
				{				
				       case 0: //left one byte
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uGValue);
						vWriteDramB(TempAddress+(bTemp--),uBValue);
						vWriteDramB(TempAddress+(bTemp--),uRValue);
					}
					vWriteDramB(TempAddress+(bTemp--),uGValue);
					TempAddress += 16;
					bTemp = 15;	
					break;
					
					case 1://left two byte
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uBValue);
						vWriteDramB(TempAddress+(bTemp--),uRValue);
						vWriteDramB(TempAddress+(bTemp--),uGValue);
					}					
					vWriteDramB(TempAddress+(bTemp--),uBValue);
					TempAddress += 16;
					bTemp = 15;					
					break;
					
					case 2:
					for(bPackCount=0;bPackCount<5;bPackCount++)
					{
						vWriteDramB(TempAddress+(bTemp--),uRValue);
						vWriteDramB(TempAddress+(bTemp--),uGValue);
						vWriteDramB(TempAddress+(bTemp--),uBValue);
					}					
					vWriteDramB(TempAddress+(bTemp--),uRValue);
					TempAddress += 16;
					bTemp = 15;
					break;
					
					default:
					    break;
				}
			}
		}
		_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1,10, FALSE);  
	   }	 
	   else
	   { 	  
	   	//if(_uDisableFunction==1)
		{ 
		    bCount=0;
			//_uDisableFunction=0;	
			_rMChannel.bIsFreeze=0;
			_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PATTERN, VDP_1, 10, TRUE);
			vDrvMainImportOnOff(SV_OFF);
			vRegWriteFldAlign(C_COMB_2A, bOriEN3D, EN3D);
			vRegWriteFldAlign(SCPIP_DS1_02, bOriDSIs444Mode, DS1_02_SEL444_1);
			vRegWriteFldAlign(SCPIP_DS1_02, bOri10BitMode, DS1_02_VD10BSEL_1);
			vRegWriteFldAlign(SCPIP_US1_00, bOriUsH, US1_00_USCALE_H_1);
			vRegWriteFldAlign(SCPIP_US1_00, bOriUsV, US1_00_USCALE_V_1);
			vRegWriteFldAlign(SCPIP_US1_01, bOriVUSisBypass, US1_01_VUS_BYPASS_1);
			vRegWriteFldAlign(SCPIP_PIP1_08, bOriDataLen, PIP1_08_DATA_LENGTH_1);
			vRegWriteFldAlign(SCPIP_PIP1_01, bOriHL, PIP1_01_HL_1);
			vRegWriteFldAlign(SCPIP_PIP1_00, bOriVL, PIP1_00_VL_1);
			vRegWriteFldAlign(SCPIP_PIP1_03, bOriRHP, PIP1_03_RHP_1);
			vRegWriteFldAlign(SCPIP_PIP1_03, bOriRHL, PIP1_03_RHL_1);
			vRegWriteFldAlign(SCPIP_PIP1_02, bOriRVP, PIP1_02_RVP_1);
			vRegWriteFldAlign(SCPIP_PIP1_02, bOriRVL, PIP1_02_RVL_1);
			vRegWriteFldAlign(SCPIP_PIP1_09, bOriFSEL_1, PIP1_09_FSEL_1);
			vRegWriteFldAlign(SCPIP_PIP_OA_03, bOriVOFST_BYPASS, PIP_OA_03_US_VOFST_BYPASS1);
			vRegWriteFldAlign(SCE_CFG_MAIN, bOriALLBP, ALLBP);
			vRegWriteFldAlign(OS_84, bOriYUV2RGB, YUV2RGB_BYPASS_MAIN);
			vDrvMainImportOnOff(SV_ON);
		}
	}
 }
#endif


