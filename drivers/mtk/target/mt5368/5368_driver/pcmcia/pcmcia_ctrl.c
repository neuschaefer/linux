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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_timer.h"
#include "x_gpio.h"
#include "drvcust_if.h"
#include "x_pinmux.h"
//#include "hw_ckgen.h"
#include "pi_dvbt_if.h"
LINT_EXT_HEADER_END

#include "pcmcia_if.h"
#include "pcmcia_ctrl.h"
#include "pcmcia_hw.h"
#include "pcmcia_isr.h"

#include "CableCard/ci_if.h"
#include "CableCard/pod_if.h"
#include "CableCard/mpod_if.h"
#include "CableCard/podci_ctrl.h"

#include "SPI/spi_if.h"
#include "NFI/nfi_if.h"

#include "x_tuner.h"
#include "tuner_if.h"


// variables for external ic
static BOOL     fgExternalIcInited = FALSE;
static BOOL     fgExternalIcGpioInit = FALSE;
static BOOL     fgDaughterBoardConnected = TRUE;

#define TOTAL_8295_GPIO_NUM 15
PFN_GPIO_CALLBACK ar8295_GpioCallback[TOTAL_8295_GPIO_NUM];
GPIO_TYPE ar8295_GpioType[TOTAL_8295_GPIO_NUM];

#if defined(CC_EXTERNAL_IC_MT5135)// GPIO Control register
// bit 4: input value
// bit 3: 1 - ci gpio enable (For CI GPIO Only, control by ci register)
// bit 2: 1 - treat as gpio
// bit 1: output value
// bit 0: 1 - gpo, 0 - gpi.
#define GPIO_CTRL_REG_BASE      0xf64 
#define TOTAL_5135_GPIO_NUM     30
#define MT5135_INTR_GPIO_NUM      15
#define MT5135_INTR_GPIO_MIN      (MT5135_GPIO3 - MT5135_GPIO0)
#define MT5135_INTR_GPIO_MAX     (MT5135_GPIO17 - MT5135_GPIO0)
#define CI_GPIO_MIN              (MT5135_GPIO3 - MT5135_GPIO0)
#define CI_GPIO_MAX             (MT5135_GPIO18 - MT5135_GPIO0)
PFN_GPIO_CALLBACK ar5135_GpioCallback[MT5135_INTR_GPIO_NUM];
GPIO_TYPE ar5135_GpioType[MT5135_INTR_GPIO_NUM];
#else //if defined(CC_EXTERNAL_IC_MT5175) //
#define GPIO_CTRL_REG_BASE      0x86c  
#define TOTAL_5135_GPIO_NUM     14
#define MT5135_INTR_GPIO_NUM      15
#define MT5135_INTR_GPIO_MIN      (MT5135_GPIO3 - MT5135_GPIO0)
#define MT5135_INTR_GPIO_MAX     (MT5135_GPIO17 - MT5135_GPIO0)
#define CI_GPIO_MIN              (MT5135_GPIO3 - MT5135_GPIO0)
#define CI_GPIO_MAX             (MT5135_GPIO18 - MT5135_GPIO0)
PFN_GPIO_CALLBACK ar5135_GpioCallback[MT5135_INTR_GPIO_NUM];
GPIO_TYPE ar5135_GpioType[MT5135_INTR_GPIO_NUM];
#endif

extern PCMCIA_EXTERNAL_IC_VERSION eExternalIcVersion;
#ifdef CC_MTK_LOADER
static PCMCIA_MAINCHIP eMainchip = PCMCIA_MAINCHIP_UNKNOWN;
#endif

static void     _ExternalIc_Init(void);
static void     _ExternalIc_Reset(void);
#ifdef CC_MTK_LOADER
static INT32    _ExternalIc_CheckAlive(void);
#endif

#ifdef PCMCIA_ENABLED

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
extern BOOL fgPcmciaIsInitedByCli;

//-----------------------------------------------------------------------------
// Exported variables
//-----------------------------------------------------------------------------
PCMCIA_DRIVER_CONFIG_T tPcmciaDriverConfig;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#if 0   // TODO, we need another new mechanism.
#if defined(CC_EXTERNAL_CI) || defined(CC_EXTERNAL_POD)
extern PFN_GPIO_CALLBACK    ar8295_GpioCallback[TOTAL_8295_GPIO_NUM];
extern GPIO_TYPE            ar8295_GpioType[TOTAL_8295_GPIO_NUM];
#endif
#endif

#ifndef CC_MTK_LOADER
static HANDLE_T sema_ExternalIcIntrq = NULL;
static HANDLE_T hCmdDoneSema = NULL;
static HANDLE_T hExternalIcIntrqThread;
static HANDLE_T hPcmciaOcThread;
static BOOL     fgCamPwrManualControl = FALSE;
static BOOL     fgThreadNoAssert = FALSE;
static BOOL     fgExternalIcThreadStarted = FALSE;
static BOOL     fgPcmciaOverCurrentMonitorThreadStarted = FALSE;
static BOOL     fgPcmciaInit = FALSE;
static BOOL     fgExternalIcIntrqThreadKeepRunning = FALSE;
static BOOL     fgExternalIcIntrqThreadTerminated = TRUE;
static INT32    u4CamPwrStatus = -1;    // TRUE:PowerOn, FALSE:PowerOff
static INT32    u4CamPwrOcStatus = FALSE;  // TRUE:OverCurrentHappen, FALSE:NoOverCurrent
static BOOL     u4CamPwrOcStopRetry = FALSE;

static void     (*vExternalIcIsrSifFct)  (UINT16  ui2_vector_id);
static void     (*vExternalIcIsrIreqFct) (UINT16  ui2_vector_id);
static void     (*pPcmciaNfyfp) (PCMCIA_NFY_EVENT eNfyEventCode);

// Note ! There are two kinds of PCMCIA interrupt mode : 
//  1. External CI/POD Interrupt mode : 
//      Handled in this file : _ExternalIc_IntrqIsr(), _ExternalIc_IntrThreadMain()
//      An command done interrupt pin is required to connect from 8295 to main chip.
//  2. Internal CI/POD Interrupt mode : 
//      Handled in pcmcia_isr.c : _PCMCIA_Isr()
//
// By default, we use polling mode for external CI/POD, and interrupt mode for 
// internal CI/POD. This is initialized in PCMCIA_Initialize().
// Of couse, if PCB support, we can enable interrupt mode in external CI/POD.
// In internal CI/POD, we always use interrupt mode.


static void     _PCMCIA_ResetMmx(void);
static void     _PCMCIA_ResetFramer(void);
static void     _PCMCIA_ResetMDmx(void);
static void     _PCMCIA_ResetCmp(void);
static void     _PCMCIA_ConfigCtrlRegister(void);
static void     _PCMCIA_ConfigWindows(void);
static void     _PCMCIA_ConfigTiming(void);
#endif
static void     _ExternalIc_SetPll(void);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static UINT32 _PCMCIA_DrvCurrentTrans(UINT32 u4Current)
{
    UINT32 val;
    UNUSED(_PCMCIA_DrvCurrentTrans);

    switch(u4Current)
    {
        case 4:
            val = 0x1;
            break;
        case 6:
            val = 0x2;
            break;
        case 8:
            val = 0x3;
            break;
        case 2:
        default:
            val = 0x0;
            break;
    }
    return val;
}

static void _PCMCIA_SetCamDrvingCurrent(void)
{
    UINT32 u4Val = 0;

    if (DRVCUST_OptQuery(eMT8295DrivingCurrent, &u4Val) == 0)
    {
        PCMCIA_SetDriving(u4Val);
    }

    return;
}

static void _PCMCIA_OnOffCamStream(BOOL fgOn)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        if( fgOn )
        {
            if ( (eExternalIcVersion == MT5135_VERSION_AE) || (eExternalIcVersion == MT5175_VERSION_AE) )
            {
                TunerSetTsOutput(BOOL_FALSE);
            }
            PCMCIAHW_WRITE32( REG_CTRL, PCMCIAHW_READ32(REG_CTRL) & 0xfff3ffff | 0x000c0000);        //TS Enable (MT8295)
            PCMCIAHW_WRITE32( REG_POD_GPR_CNTL_SET0, (PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & ~(0x80000000)) | 0x00000000);        //Non-Bypass (MT8295)
            if ( (eExternalIcVersion == MT5135_VERSION_AE) || (eExternalIcVersion == MT5175_VERSION_AE) )
            {
                TunerSetTsOutput(BOOL_TRUE);
            }
        }
        else
        {
            if ( (eExternalIcVersion == MT5135_VERSION_AE ) || (eExternalIcVersion == MT5175_VERSION_AE) )
            {
                TunerSetTsOutput(BOOL_FALSE);
            }
            PCMCIAHW_WRITE32( REG_CTRL, PCMCIAHW_READ32(REG_CTRL) & 0xfff3ffff | 0x00000000);        //TS Disable (MT8295)
            PCMCIAHW_WRITE32( REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & ~(0x80000000) | 0x80000000);        //Bypass (MT8295)
            if ( (eExternalIcVersion == MT5135_VERSION_AE) || (eExternalIcVersion == MT5175_VERSION_AE) )
            {
                TunerSetTsOutput(BOOL_TRUE);
            }
        }
    }
    else
    {
        // controlled by _PCMCIA_AutoSetTsRoutingPath()
    }
}

static void _PCMCIA_OverCurrentMonitorThreadMain(void * pvArg)
{
    UINT32 u4OCGpio;
    UINT32 u4OCPolarity;
    static UINT32 u4OcCount = 0;
    
    if ( DRVCUST_OptQuery(eCI5VOcGpio, &u4OCGpio) != 0 ||
         DRVCUST_OptQuery(eCI5VOcPolarity, &u4OCPolarity) != 0)
    {
        return;
    }

    while(1)
    {
        x_thread_delay(1000);
        if ( u4CamPwrOcStopRetry )
        {
            continue;
        }
        
        // OC just happens.
        if ( GPIO_Input((INT32)u4OCGpio) == u4OCPolarity && 
             u4CamPwrOcStatus == FALSE )
        {
            LOG(0, "CI CAM 5V over current. \n");
            u4CamPwrOcStatus = TRUE;
            PCMCIA_AutoConfigCamPwrSwitch();
            PCMCIA_NfyCamStatusCbk(PCMCIA_OVER_CURRENT);
            u4OcCount++;
            
            // The OC happens for 5 seconds, stop retry.
            if ( u4OcCount == 5 )
            {
                LOG(0, "CI CAM 5V over current exists more than 5s, stop retry. \n");
                u4CamPwrOcStopRetry = TRUE;
                u4CamPwrOcStatus = FALSE;
                u4OcCount = 0;
            }
        }
        // OC happens in last second and keep OC in this second.
        else if ( GPIO_Input((INT32)u4OCGpio) == u4OCPolarity && 
                  u4CamPwrOcStatus == TRUE )
        {}
        // OC just disappear.
        else if ( GPIO_Input((INT32)u4OCGpio) != u4OCPolarity && 
                  u4CamPwrOcStatus == TRUE )
        {
            u4CamPwrOcStatus = FALSE;
            PCMCIA_AutoConfigCamPwrSwitch();
        }
        // No OC.
        else
        {
            u4OcCount = 0;
        }
    }
}

static void _PCMCIA_StartOverCurrentMonitorThread(void)
{
    UINT32 u4OCGpio;
    
    if ( DRVCUST_OptQuery(eCI5VOcGpio, &u4OCGpio) == 0 )
    {
        if ( fgPcmciaOverCurrentMonitorThreadStarted == FALSE )
        {
            VERIFY( x_thread_create(&hPcmciaOcThread,
                                    "PCMCIA_OC",
                                    1024,   //stack size
                                    127,    //priority
                                    _PCMCIA_OverCurrentMonitorThreadMain,
                                    0,
                                    NULL) == OSR_OK);
                                   
            fgPcmciaOverCurrentMonitorThreadStarted = TRUE;
        }
    }
}

// Used for internal CI.
static void _PCMCIA_Reset(void)
{
    PCMCIAHW_WRITE32(REG_RST, PCMCIAHW_READ32(REG_RST) | IRST);
    x_thread_delay(1);
    PCMCIAHW_WRITE32(REG_RST, PCMCIAHW_READ32(REG_RST) & ~IRST);
    x_thread_delay(1);
    
    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_POD )
    {
        _PCMCIA_ResetMmx();
        _PCMCIA_ResetFramer();
        _PCMCIA_ResetMDmx();
        _PCMCIA_ResetCmp();
        x_thread_delay(10);
    }
}

static INT32 _PCMCIA_ConfigRegDefault(void)
{
    /* Configure PCMCIA_POD_CI control interface */
    /* Initially, the HW should be configured to support 16-bit memory only PC card */
    _PCMCIA_ConfigCtrlRegister();
    _PCMCIA_ConfigWindows();
    _PCMCIA_ConfigTiming();

    return POD_OK;
}

static void _PCMCIA_ConfigCtrlRegister(void)
{
    UINT32 u4Reg;

    u4Reg = (UINT32)GCMD_IE         | 
            (UINT32)PC_CD_IE        |
            (UINT32)PC_MEM_WIN_0_EN | 
            (UINT32)PC_MEM_WIN_1_EN | 
            (UINT32)PC_IO_WIN_EN    |
            (UINT32)PC_WP_EN        | 
            (UINT32)SIXTEEN_QW      |
            (UINT32)MEM_ONLY        | 
            (UINT32)EIGHT_BIT_MEM;

    PCMCIAHW_WRITE32(REG_CTRL, u4Reg);
}

static void _PCMCIA_ConfigWindows(void)
{
    /* Configure memory window 0 for attribute memory access */
    PCMCIAHW_WRITE32(REG_PC_MEM_WIN0_1, 0 & (UINT32)ATTRIBUTE_MEM);
    PCMCIAHW_WRITE32(REG_PC_MEM_WIN0_2, ATTRIBUTE_MEM_END_ADDR);

    /* Configure memory window 1 for common memory access */
    PCMCIAHW_WRITE32(REG_PC_MEM_WIN1_1, 0 | (UINT32)COMMON_MEM);
    PCMCIAHW_WRITE32(REG_PC_MEM_WIN1_2, COMMON_MEM_END_ADDR);
}

static void _PCMCIA_ConfigTiming(void)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        PCMCIAHW_WRITE32(REG_MEM_TIMING0, 0x0a162c2c);
        PCMCIAHW_WRITE32(REG_MEM_TIMING1, 0x34440000);
        PCMCIAHW_WRITE32(REG_IO_TIMING0, 0x14042c3a);
        PCMCIAHW_WRITE32(REG_IO_TIMING1, 0x42400000);
    }
    else
    {
        PCMCIAHW_WRITE32(REG_MEM_TIMING0, 0x080e1c1c);
        PCMCIAHW_WRITE32(REG_MEM_TIMING1, 0x222a0000);
        PCMCIAHW_WRITE32(REG_IO_TIMING0, 0x0c021a22);
        PCMCIAHW_WRITE32(REG_IO_TIMING1, 0x28260000);
    }
}
#endif

#ifdef CC_MTK_LOADER
static BOOL _PCMCIA_SetPllOn(PCMCIA_EXTERNAL_IC_VERSION eExternalCiIcVer)
{
    UINT32 PLL_InitSeq[9] = {0};
    UINT32 PLL_ChkSeq[5] = {0};
    UINT8 i;
     
    PLL_InitSeq[0] = REG_PLL_CNTL_SET0; PLL_InitSeq[1] = 0x1892101f;
    PLL_InitSeq[2] = REG_PLL_CNTL_SET1; PLL_InitSeq[3] = 0x00002800;
    PLL_InitSeq[4] = REG_PLL_CNTL_SET0; PLL_InitSeq[5] = 0x0890606f;
    PLL_InitSeq[6] = REG_PLL_CNTL_SET1; PLL_InitSeq[7] = 0x80002800;
    PLL_InitSeq[8] = 0xFFFF;    //termination

    PLL_ChkSeq[0] = REG_PLL_CNTL_SET0;  PLL_ChkSeq[1] = 0x0890606f;
    PLL_ChkSeq[2] = REG_PLL_CNTL_SET1;  PLL_ChkSeq[3] = 0x80002800;
    PLL_ChkSeq[4] = 0xFFFF;     //termination

    if ( eExternalCiIcVer == MT8295_VERSION_AE )
    {}
    else
    {
        PLL_InitSeq[7] = 0x20002800;
        PLL_ChkSeq[3] = 0x20002800;
    }
    
    for( i = 0 ; PLL_InitSeq[i]!=0xFFFF && i < 7 ; i+=2 )     //init
    {
        ASSERT(i<20);
        PCMCIAHW_WRITE32( PLL_InitSeq[i] , PLL_InitSeq[i+1] );
    }
    x_thread_delay(10);     //delay 10 ms
    for( i = 0 ; PLL_ChkSeq[i]!=0xFFFF && i < 3 ; i+=2 )    //check
    {
        ASSERT(i<20);
        if ( PCMCIAHW_READ32( PLL_ChkSeq[i] ) != PLL_ChkSeq[i+1] )
        {
            Printf("PLL Read : 0x%X\n", PCMCIAHW_READ32( PLL_ChkSeq[i] ));
            Printf("PLL Predict : 0x%X\n", PLL_ChkSeq[i+1]);
            return FALSE;
        }
    }
    return TRUE;
}
#endif // CC_MTK_LOADER

static void _PCMCIA_ResetMDmx(void)
{
    PCMCIAHW_WRITE32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER, 
                     PCMCIAHW_READ32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER) | 0x00010000 );
    x_thread_delay(1);
    PCMCIAHW_WRITE32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER, 
                     PCMCIAHW_READ32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER) & 0xFFFEFFFF );
    x_thread_delay(1);
}

static void _PCMCIA_ResetFramer(void)
{
    PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 
                     PCMCIAHW_READ32(MPOD_MUX_FRAMER_CONTROL_REGISTER) | 0x000F0000 );
    x_thread_delay(1);
    PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 
                     PCMCIAHW_READ32(MPOD_MUX_FRAMER_CONTROL_REGISTER) & 0xFFF0FFFF );
    x_thread_delay(1);
}

static void _PCMCIA_ResetMmx(void)
{
    PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_0, 0x80000000 );
    x_thread_delay(1);
    PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_0, 0x40000000 );
    x_thread_delay(1);
}

static void _PCMCIA_ResetCmp(void)
{
    PCMCIAHW_WRITE32(MPOD_CMP_CONFIGURATION_REGISTER, 
                     PCMCIAHW_READ32(MPOD_CMP_CONFIGURATION_REGISTER) | 0x02000000 );
    x_thread_delay(1);
    PCMCIAHW_WRITE32(MPOD_CMP_CONFIGURATION_REGISTER, 
                     PCMCIAHW_READ32(MPOD_CMP_CONFIGURATION_REGISTER) & 0xFDFFFFFF );
    x_thread_delay(1);
}

static void _ExternalIc_SetPll()
{    
    // Set clock in loader. Just print log in TV image.
    if ((eExternalIcVersion == MT5135_VERSION_AE) || (eExternalIcVersion == MT5175_VERSION_AE))
    {
#ifdef CC_MTK_LOADER  
        UINT32 u4Clock = 0;
        //IO_WRITE32(0xf00280dc, 0, 0x0d000084);// 5135 clock src
        DRVCUST_InitQuery(eCISysClock, &u4Clock);
        PCMCIA_SetCiSysClock(u4Clock);
        
        u4Clock = 0;
        DRVCUST_InitQuery(eCITsClock, &u4Clock);
        PCMCIA_SetCiTsClock(u4Clock);
#endif
        return;
    }    

#ifdef CC_MTK_LOADER
    if ( eExternalIcVersion == MT8295_VERSION_BNE)
    {
        if ( _PCMCIA_SetPllOn(MT8295_VERSION_BNE) )
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)|0xC000 );
        }
    }
#endif

    if ( eExternalIcVersion==MT8295_VERSION_NONE )
    {
#ifdef CC_MTK_LOADER
        Printf("Can't find proper PLL setting for External CI\n");
#else
        LOG(0, "Can't find proper PLL setting for External CI\n");
#endif
        ASSERT(0);
    }
}

#ifndef CC_MTK_LOADER
static void _ExternalIc_IntrqIsr(INT32 i4Gpio, BOOL fgStatus)
{
    UINT32 u4DrvCustIntPin;

    VERIFY(DRVCUST_OptQuery(eMT8295IntGpio, &u4DrvCustIntPin) == 0);
    VERIFY( (UINT32)i4Gpio == u4DrvCustIntPin );
    LOG(7, "(_ExternalIc_IntrqIsr) i4Gpio = %d, fgStatus = %d\n", i4Gpio, fgStatus);

        // Handle 8295 GPIOs, CmdDone Interrupt
        VERIFY( x_sema_unlock(sema_ExternalIcIntrq)==OSR_OK);
    }

static void _ExternalIc_IntrThreadHandleSif()
{
    UINT32 u4RegIntSts;
    
    u4RegIntSts = PCMCIAHW_READ32(REG_WRP_INTST);
    if ( (u4RegIntSts & 0x00000003) != 0 )
    {
        if ( vExternalIcIsrSifFct != NULL )
        {
            vExternalIcIsrSifFct(0);
        }
    }
}

static void _ExternalIc_IntrThreadHandleIreq(UINT32 u4RegIntSts)
{
    //UINT32 u4RegIntSts;
    UINT32 u4PbusSts;
    
    //u4RegIntSts = PCMCIAHW_READ32(REG_INT_STS);
    if ( (u4RegIntSts & PC_RDY_IRQ_INT) != 0 )
    {
        if ( vExternalIcIsrIreqFct != NULL )
        {
            u4PbusSts = PCMCIAHW_READ32(REG_PBUS_STS);
            if ( ((u4PbusSts&PC_CD1) == 0) && ((u4PbusSts&PC_CD2) == 0) )
            {
                LOG(5, "PCMCIA IREQ pin status change: %d\n", u4PbusSts&PC_RDY_IRQ?TRUE:FALSE);
                vExternalIcIsrIreqFct( u4PbusSts&PC_RDY_IRQ?TRUE:FALSE );
            }
        }
    }
}

static void _ExternalIc_IntrThreadHandleCd(void)
{
    PCMCIA_CDPinHandler();
}

static void _ExternalIc_IntrThreadMain(void * pvArg)
{
    UINT32  u4RegIntEnb, i, u4RegIntSts, u4RegInput, u4RegCtrl, u4RegCmdSts;
    UINT8 u1Count;
    UNUSED(u4RegInput);

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return;
    }
    
    while( TRUE )
    {
        if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )
        {
            //VERIFY( x_sema_lock( sema_ExternalIcIntrq, X_SEMA_OPTION_WAIT )==OSR_OK);
            x_sema_lock_timeout( sema_ExternalIcIntrq, 50 );
        }
        else
        {
            x_thread_delay(100);
        }

        if ( eExternalIcVersion == MT8295_VERSION_BNE )
        {
            // Handle SIF Interrupt
            _ExternalIc_IntrThreadHandleSif();
        }

        // Handle CD Interrupt
        _ExternalIc_IntrThreadHandleCd();

        u1Count = 0;
        u4RegIntSts = PCMCIAHW_READ32(REG_INT_STS);  //Gpio Interrupt Register (voltage high low)
        while (u4RegIntSts && (u1Count++<SEMA_TIMEOUT) )
        {
            PCMCIAHW_WRITE32(REG_INT_STS, u4RegIntSts);             //Clear interrupt bits
            // Handle IREQ Interrupt
            _ExternalIc_IntrThreadHandleIreq(u4RegIntSts);
        
            u4RegIntEnb = PCMCIAHW_READ32(REG_GPIO_INT_ENABLE);     //Inerrupt  Enable Register
            //u4RegIntSts = PCMCIAHW_READ32(REG_INT_STS);           //Interrupt Status Register
            u4RegInput = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET4);    //Gpio Input Register (voltage high low)
            //PCMCIAHW_WRITE32(REG_INT_STS, u4RegIntSts);             //Clear interrupt bits
            u4RegCtrl = PCMCIAHW_READ32(REG_CTRL);
            u4RegCmdSts = PCMCIAHW_READ32(REG_CMD_STS);

            LOG(9, "REG_GPIO_INT_ENABLE0x%02X=0x%08X, REG_INT_STS0x%02X=0x%08X , REG_POD_GPR_CNTL_SET40x%02X=0x%08X\n",
                REG_GPIO_INT_ENABLE , REG_INT_STS,REG_POD_GPR_CNTL_SET4,u4RegIntEnb, u4RegIntSts ,u4RegInput);

            if ( (eExternalIcVersion == MT5135_VERSION_AE) || (eExternalIcVersion == MT5175_VERSION_AE) )
            {
                if (u4RegIntSts & 0x7fff0000)
                {
                    for(i=0;i<MT5135_INTR_GPIO_NUM;i++)
                    {
                        if( u4RegIntEnb & (1U<<i) & (u4RegIntSts>>16) )       //register set
                        {
                            #if defined(CC_EXTERNAL_CI) || defined(CC_EXTERNAL_POD)
                            if ( (ar5135_GpioType[i] == GPIO_TYPE_INTR_BOTH) ||
                                (ar5135_GpioType[i] == GPIO_TYPE_INTR_RISE &&  (u4RegInput&(1U<<i)))    ||  // low to high
                                (ar5135_GpioType[i] == GPIO_TYPE_INTR_FALL &&  !(u4RegInput&(1U<<i))))       //  high to low
                            {
                                LOG(7, "5135 gpio %d 's polarity is %s, callback addr %08X\n", i, (u4RegInput&(1U<<i))?"high":"low", ar5135_GpioCallback[i]);

                                // Invoke Callback Function
                                (*ar5135_GpioCallback[i])( (INT32)(MT5135_GPIO3+i),  u4RegInput&(1U<<i)?TRUE:FALSE );
                            }
                            #endif
                        }
                    }
                }
            }
            else /*-----Handle 8295 Gpio Bits-----*/
            {
                if ( u4RegIntSts & 0xffff0000 )
                {
                    for(i=0;i<TOTAL_8295_GPIO_NUM;i++)
                    {
                        if( u4RegIntEnb & (1U<<i) & (u4RegIntSts>>16) )       //register set
                        {
                            #if defined(CC_EXTERNAL_CI) || defined(CC_EXTERNAL_POD)
                            if ( (ar8295_GpioType[i] == GPIO_TYPE_INTR_BOTH) ||
                                (ar8295_GpioType[i] == GPIO_TYPE_INTR_RISE &&  (u4RegInput&(1U<<i)))    ||  // low to high
                                (ar8295_GpioType[i] == GPIO_TYPE_INTR_FALL &&  !(u4RegInput&(1U<<i))))       //  high to low
                            {
                                LOG(7, "8295 gpio %d 's polarity is %s, callback addr %08X\n", i, (u4RegInput&(1U<<i))?"high":"low", ar8295_GpioCallback[i]);

                                // Invoke Callback Function
                                (*ar8295_GpioCallback[i])( (INT32)(MT8295_GPIO0+i),  u4RegInput&(1U<<i)?TRUE:FALSE );
                            }
                            #endif
                        }
                    }
                }
            }

            /* ----Handle 8295 CI Interrupt Bits -----*/
            if ( u4RegIntSts & 0x0000ffff )
            {
                /* Command done interrupt */
                if ((u4RegCtrl & GCMD_IE) && (u4RegIntSts & CMD_DONE_INT))
                {
                    u4RegCmdSts = PCMCIAHW_READ32(REG_CMD_STS);
                    if ( (UINT32)(u4RegCmdSts & (CMD_STS_DONE | CMD_STS_DONE_OK)) != (UINT32)(CMD_STS_DONE | CMD_STS_DONE_OK))
                    {
                        //CMD_STS_DONE Interrupt Error
                        x_thread_delay(1);
                    }
                    else        //0xXXXXXXX3 :OK
                    {
                        PCMCIA_SetCmdStatusDone();
                    }
                }
            }
            u4RegIntSts = PCMCIAHW_READ32(REG_INT_STS);
        }

        if ( fgExternalIcIntrqThreadKeepRunning == FALSE )
        {
            break;
        }

        if ( fgThreadNoAssert == FALSE )
        {
            if ( u1Count > SEMA_TIMEOUT )
            {
                LOG(0, "_ExternalIc_IntrThreadMain Timeout");
            }
            ASSERT( u1Count <= SEMA_TIMEOUT );          //the events should not be coming so quickly
        }
    }

    fgExternalIcIntrqThreadTerminated = TRUE;
}

static void _ExternalIc_StartIntrThread()
{
    if ( fgExternalIcThreadStarted == TRUE )
    {
        return;
    }

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return;
    }

    if ( sema_ExternalIcIntrq == NULL )
    {
        VERIFY( x_sema_create(&sema_ExternalIcIntrq, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK)==OSR_OK);
    }
    
    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )
    {
        UINT32 u4DrvCustIntPin;
        VERIFY( DRVCUST_OptQuery(eMT8295IntGpio, &u4DrvCustIntPin) == 0);
        VERIFY( GPIO_Reg((INT32)u4DrvCustIntPin, GPIO_TYPE_INTR_BOTH, _ExternalIc_IntrqIsr) ==0);
    }

    VERIFY( x_thread_create(&hExternalIcIntrqThread,
                            "MT8295_IntrqThread",
                            1024,   //stack size
                            127,    //priority
                            _ExternalIc_IntrThreadMain,
                            0,
                           NULL) == OSR_OK);

    fgExternalIcIntrqThreadKeepRunning = TRUE;
    fgExternalIcIntrqThreadTerminated = FALSE;
    fgExternalIcThreadStarted = TRUE;
    return;
}

void PCMCIA_CDPinHandler(void)
{
    UINT32 u4RegIntSts;
    static BOOL fgCamConnected = FALSE;
    static BOOL fgCamStatisInited = FALSE;

    u4RegIntSts = PCMCIAHW_READ32(REG_INT_STS);
    if ( (u4RegIntSts & PC_CD_INT) != 0 )
    {
        // CAM Connected. (Just to print log)
        if ( (PCMCIAHW_QueryPbusSts((UINT32)PC_CD1) == 0) &&
             (PCMCIAHW_QueryPbusSts((UINT32)PC_CD2) == 0) )
        {
            if ( fgCamStatisInited == FALSE || 
                 (fgCamStatisInited == TRUE && fgCamConnected == FALSE) )
            {
                LOG(0, "CI CAM connected\n");
                fgCamStatisInited = TRUE;
                fgCamConnected = TRUE;
            }
        }
        // CAM Not Connected. (Just to print log)
        else
        {
            if ( fgCamStatisInited == FALSE ||
                 (fgCamStatisInited == TRUE && fgCamConnected == TRUE) )
            {
                LOG(0, "CI CAM disconnected\n");
                fgCamStatisInited = TRUE;
                fgCamConnected = FALSE;
            }
        }

        u4CamPwrOcStopRetry = FALSE;
        PCMCIA_AutoConfigCamPwrSwitch();
    }
}
#endif

#ifndef CC_MTK_LOADER
INT32 PCMCIA_Initialize(void)
{
    INT32   i4Return;
    UINT32  u4DrvCustIntPin;

    if( fgPcmciaInit == TRUE )
    {
        return POD_OK;
    }
    
    PODCI_AllocDmaBuf();

    // Init PODCI
    PODCI_Init();

    // Create Semaphores
    if ( hCmdDoneSema == NULL )
    {
        VERIFY(x_sema_create(&hCmdDoneSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }

    // Initialize ISR function pointers
    vExternalIcIsrSifFct = NULL;
    vExternalIcIsrIreqFct = NULL;
    pPcmciaNfyfp = NULL;

    // Hardware Init
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        if ( DRVCUST_OptQuery(eMT8295IntGpio, &u4DrvCustIntPin) == 0 )
        {
            tPcmciaDriverConfig.u1CmdDoneMethod = PCMCIA_CMD_DONE_INTERRUPT;
            LOG(0, "External PCMCIA Interrupt Mode.\n");
        }
        else
        {
            tPcmciaDriverConfig.u1CmdDoneMethod = PCMCIA_CMD_DONE_POLLING;
            LOG(0, "External PCMCIA Polling Mode.\n");
        }
        
        PCMCIA_GPIO_Init();
        _ExternalIc_Init();
        _PCMCIA_SetCamDrvingCurrent();
        if (fgDaughterBoardConnected == FALSE)
        {
            LOG(0, "PCMCIA_Initialize: Daughter board is not connected.\n");
            return POD_OK;
        }
        /*if (eMainchip == PCMCIA_MAINCHIP_MT5366)
        {
            _ExternalIc_Reset();
            _ExternalIc_SetPll();
        }*/
        _ExternalIc_StartIntrThread();
    }
    else
    {
        tPcmciaDriverConfig.u1CmdDoneMethod = PCMCIA_CMD_DONE_INTERRUPT;
        
        PCMCIA_PinMux();

        _PCMCIA_Reset();

        // disable TS from demod to CI CAM
        TunerSetTsTristate(BOOL_TRUE);
    }

    _PCMCIA_StartOverCurrentMonitorThread();

    // Configure hardware controller
    i4Return = _PCMCIA_ConfigRegDefault();
    if (i4Return != POD_OK)
    {
        return i4Return;
    }

    fgPcmciaInit=(BOOL)TRUE;

    PCMCIA_AutoConfigCamPwrSwitch();

    return POD_OK;
}

void PCMCIA_CamPwrCtrl(BOOL fgManualControl)
{
    fgCamPwrManualControl = fgManualControl;
}

void PCMCIA_CamPwrONOFF(BOOL fgPwrOnOff)
{
    UINT32 u4GpioNum, u4PwrOnPolarity;
    if ( fgCamPwrManualControl == TRUE )
    {
        VERIFY( DRVCUST_OptQuery(eCI5VPowerOnGpio, &u4GpioNum) == 0 );
        VERIFY( DRVCUST_OptQuery(eCI5VPowerOnPolarity, &u4PwrOnPolarity) == 0 );
        GPIO_SetOut((INT32)u4GpioNum, (u4PwrOnPolarity?1:0));    //enable CI_VCC_CNTL
        u4CamPwrStatus = fgPwrOnOff;
    }
}

void PCMCIA_ThdNoAsrt(BOOL bNoAssert)
{
    fgThreadNoAssert = bNoAssert;
}

void PCMCIA_StopThd(void)
{
    fgExternalIcIntrqThreadKeepRunning = FALSE;
    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )
    {
        VERIFY( x_sema_unlock(sema_ExternalIcIntrq)==OSR_OK);
    }
}

void PCMCIA_ResetPcmciaHW(void)
{
    fgPcmciaInit = FALSE;
    fgExternalIcGpioInit = FALSE;
    fgExternalIcInited = FALSE;
    fgExternalIcThreadStarted = FALSE;

    // Terminate MT8295IntrqThread.
    fgExternalIcIntrqThreadKeepRunning = FALSE;
    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )
    {
        VERIFY( x_sema_unlock(sema_ExternalIcIntrq)==OSR_OK);
    }

    // Wait until thread terminated.
    while ( TRUE )
    {
        if ( fgExternalIcIntrqThreadTerminated == TRUE )
        {
            break;
        }
    }
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        _ExternalIc_Reset();
    }
    else
    {
        _PCMCIA_Reset();
    }
}


BOOL PCMCIA_GetCmdStatusDone(void)
{
    UINT32    u4CmdSts , u4PollCount;
    INT32             i4ret;

    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )   // Interrupt Mode
    {
        i4ret = x_sema_lock_timeout(hCmdDoneSema, SEMA_TIMEOUT);
        if( i4ret != OSR_OK )
        {
                LOG(0, "0x4 : 0x%x\n", PCMCIAHW_READ32(0x4));
                LOG(0, "0x8 : 0x%x\n", PCMCIAHW_READ32(0x8));
                LOG(0, "0xC : 0x%x\n", PCMCIAHW_READ32(0xC));
                LOG(0, "0x10 : 0x%x\n", PCMCIAHW_READ32(0x10));
                LOG(0, "REG_SDA_DATA_READ: 0x%x=0x%x\n", REG_SDA_DATA_READ,PCMCIAHW_READ32(REG_SDA_DATA_READ));
            LOG(0, "PCMCIA_GetCmdStatusDone Fail\n");
            return FALSE;
        }
    }
    else
    {                                    //Polling
        u4PollCount = 0;
        while( TRUE )
        {
            /* main code */
            u4CmdSts  = PCMCIAHW_READ32(REG_CMD_STS);
            if(  (UINT32)(u4CmdSts  & (CMD_STS_DONE | CMD_STS_DONE_OK) ) == (UINT32)(CMD_STS_DONE | CMD_STS_DONE_OK) )
                break;

            x_thread_delay( 1 );        //delay 1 ms

            /* exception: escape if consume too much time */
            if( ++u4PollCount > SEMA_TIMEOUT )
            {
                LOG(0, "PCMCIA_GetCmdStatusDone Timeout\n");
                LOG(0, "0x4 : 0x%x\n", PCMCIAHW_READ32(0x4));
                LOG(0, "0x8 : 0x%x\n", PCMCIAHW_READ32(0x8));
                LOG(0, "0xC : 0x%x\n", PCMCIAHW_READ32(0xC));
                LOG(0, "0x10 : 0x%x\n", PCMCIAHW_READ32(0x10));
                return FALSE;
            }
        }
        LOG( 9, "POD_GetCmdDone(), Polling Mode, u4PollCount=%d \n", u4PollCount );
    }
    return TRUE;
}

void PCMCIA_SetCmdStatusDone(void)
{
    VERIFY(x_sema_unlock(hCmdDoneSema) == OSR_OK);
}

INT32 PCMCIA_PcmciaToPodCi(BOOL fgPcmciaToPod)
{
    UINT32 u4Reg;

    if (fgPcmciaToPod)  /* POD mode */
    {
        if ( ( PCMCIAHW_READ32(REG_CTRL)&0xC0000000) != (UINT32)PODCI )
        {
            // change to pod/ci mode
            PCMCIAHW_WRITE32(REG_CTRL, (PCMCIAHW_READ32(REG_CTRL) & 0x3FFFFFFF) | (UINT32)PODCI);

            // enable IREQ interrupt (should not enable this in initialization flow : _PCMCIA_ConfigCtrlRegister)
            {
                u4Reg = PCMCIAHW_READ32(REG_CTRL);
                u4Reg |= (UINT32)PC_RDY_IRQ_IE;
                PCMCIAHW_WRITE32(REG_CTRL, u4Reg);
            }
        }
    }
    else                /* PCMCIA mode */
    {
        if ( (PCMCIAHW_READ32(REG_CTRL) &0xC0000000) != (UINT32)EIGHT_BIT_MEM )
        {
            // change to pcmcia mode
            PCMCIAHW_WRITE32(REG_CTRL, (PCMCIAHW_READ32(REG_CTRL) & 0x3FFFFFFF) | (UINT32)EIGHT_BIT_MEM);

            // disable IREQ interrupt
            {
                u4Reg = PCMCIAHW_READ32(REG_CTRL);
                u4Reg &= (UINT32)(~PC_RDY_IRQ_IE);
                PCMCIAHW_WRITE32(REG_CTRL, u4Reg);
            }
        }
    }

    return POD_OK;
}

INT32 PCMCIA_EnableTS(BOOL fgPcmciaEnableTS)
{
    static INT32 i4Prev = -1;

    if (fgPcmciaEnableTS) /* POD mode */
    {
        if (1 != i4Prev)
        {
            _PCMCIA_OnOffCamStream(TRUE);
            i4Prev = 1;
        }
    }
    else /* PCMCIA mode */
    {
        if (0 != i4Prev)
        {
            _PCMCIA_OnOffCamStream(FALSE);
            i4Prev = 0;
        }
    }

    return POD_OK;
}

INT32 PCMCIA_DetectCableCard()
{
    if ( fgPcmciaInit == FALSE )
    {
        return 0;
    }

    if ( u4CamPwrStatus == TRUE &&
         (PCMCIAHW_QueryPbusSts((UINT32)PC_CD1) == 0) &&
         (PCMCIAHW_QueryPbusSts((UINT32)PC_CD2) == 0) )
    {
        // CAM connected. (Only when power switch is on.)
        return 1;
    }
    else
    {
        // CAM not connected.
        return 0;
    }
}

BOOL PCMCIA_DetectPodCiActive(void)
{
    if ( PCMCIAHW_READ32(REG_CTRL) & PODCI )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void PCMCIA_ResetCableCard(void)
{
    /* Reset PCMCIA_POD_CI control interface */
    PCMCIAHW_WRITE32(REG_RST, PCMCIAHW_READ32(REG_RST) | PCRST);
    x_thread_delay(10);                //10ms
    PCMCIAHW_WRITE32(REG_RST, PCMCIAHW_READ32(REG_RST) & ~PCRST);
    x_thread_delay(50);                //50ms
}

INT32 PCMCIA_ResetCableCardOnOff(BOOL b_set)
{
    if (b_set)
    {
        PCMCIAHW_WRITE32(REG_RST, PCMCIAHW_READ32(REG_RST) | PCRST);
    }
    else
    {
        PCMCIAHW_WRITE32(REG_RST, PCMCIAHW_READ32(REG_RST) & ~PCRST);
        x_thread_delay(300);    // Work around for CAM005.
    }

    x_thread_delay(1);

    return POD_OK;
}

void PCMCIA_SetExtDemodOnOff(BOOL fgOnOff)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        if (fgOnOff)
        {
            if ( eExternalIcVersion == MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)|0x4000 );
            }
            else
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)&(~0xC000));
            }
        }
        else
        {
            if ( eExternalIcVersion == MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)&(~0x4000));
            }
            else
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)|0xC000 );
            }
        }
    }
    else
    {}
}

void PCMCIA_SetParallelTsOnOff(BOOL fgOnOff)
{

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        if (fgOnOff)
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)|0x40000000 );
            
            if ( eExternalIcVersion == MT8295_VERSION_BNE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)&0xFFFF3FFF );
            }
        }
        else
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&(~0x40000000));
            
            if ( eExternalIcVersion == MT8295_VERSION_BNE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)|0x0000C000 );
            }
        }
    }
    else
    {
        // TODO
    }
}

void PCMCIA_ExternalICSetTsClkRate(UINT32 u4ClkRate)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return;
    }

    if ( eExternalIcVersion == MT8295_VERSION_AE )
    {
        switch (u4ClkRate)
        {
            case 63:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890606f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x80002800);
                break;
            case 72:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x089070ff);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x80002800);
                break;
            case 81:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890506f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x80001400);
                break;
            case 94:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x089061ff);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x80001400);
                break;
            case 108:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890303f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x80000400);
                break;
            case 135:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890407f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x80000400);
                break;
            case 162:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x089050ff);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x80000400);
                break;
        }
    }
    else  if ( eExternalIcVersion == MT8295_VERSION_BNE )
    {
        switch (u4ClkRate)
        {
            case 63:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890606f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x20002800);
                break;
            case 72:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x089070ff);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x20002800);
                break;
            case 81:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890506f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x20001400);
                break;
            case 94:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890100f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x20001400);
                break;
            case 108:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890303f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x20000400);
                break;
            case 135:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x0890407f);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x20000400);
                break;
            case 162:
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET0, 0x089050ff);
                PCMCIAHW_WRITE32(REG_PLL_CNTL_SET1, 0x20000400);
                break;
        }
    }
    else//5135 or 5175
    {
        UINT8 u1Val;
        switch (u4ClkRate)
        {
            case 48:
                u1Val = 0x7; // 144 MHz
                break;
            case 62: //61.7
                u1Val = 0x6;
                break;
            case 72: 
                u1Val = 0x5;
                break;
            case 86:  //86.4
                u1Val = 0x4;
                break;
            case 108:
                u1Val = 0x3;
                break;
            case 144:
                u1Val = 0x2;
                break;
            case 54:
                u1Val = 0x1;
                break;
            case 36:
                u1Val = 0xf;
                break;
            case 27:
                u1Val = 0x13;
                break;
            default:
                u1Val = 0x5;
                break;
        }
        SPI_DemodSingleWrite(&u1Val, 0xe5b);   // CI TS CLK
        u1Val = 0x1;
        SPI_DemodSingleWrite(&u1Val, 0xe5c);
    }
}

void PCMCIA_RegISR(UINT16 u2IntNo, x_os_isr_fct pfnIsr, x_os_isr_fct *ppfnOld)
{
    UNUSED(ppfnOld);

    if ( fgPcmciaInit == FALSE )
    {
        LOG(0, "PCMCIA_RegIsr be invoked before PCMCIA initialization !\n");
        ASSERT(0);
        return;
    }
    
    if ( PCMCIA_ISR_SIF == u2IntNo )
    {
        if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
        {
            LOG(0, "Sif ISR on MT8925 should not be enabled when using Internal CI.\n");
            ASSERT(0);
        }
        else
        {
            vExternalIcIsrSifFct = pfnIsr;
        }
    }
    else if ( PCMCIA_ISR_IREQ == u2IntNo )  // This is used for CI+.
    {
        if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
        {
            PCMCIA_RegIreqCallback(pfnIsr);
        }        
        else
        {
            vExternalIcIsrIreqFct = pfnIsr;
        }
    }
}

void PCMCIA_RegCAMStatusCbk(x_pcmcia_nfy_fct pfn)
{
    pPcmciaNfyfp = pfn;
}

void PCMCIA_NfyCamStatusCbk(PCMCIA_NFY_EVENT eNfyEvent)
{
    if ( pPcmciaNfyfp != NULL )
    {
        pPcmciaNfyfp(eNfyEvent);
    }
}

void PCMCIA_AutoConfigCamPwrSwitch(void)
{
    UINT32 u4GpioNum=0, u4PwrOnPolarity=0, u4RegValue=0;
    UNUSED(u4GpioNum);UNUSED(u4PwrOnPolarity);UNUSED(u4RegValue);
//    VERIFY( DRVCUST_OptQuery(eCI5VPowerOnGpio, &u4GpioNum) == 0 );
//    VERIFY( DRVCUST_OptQuery(eCI5VPowerOnPolarity, &u4PwrOnPolarity) == 0 );

    if ( fgPcmciaInit == FALSE )
    {
        return;
    }
    if (fgDaughterBoardConnected == FALSE)
    {
        return;
    }

    // CAM Connected.
    if ( (PCMCIAHW_QueryPbusSts((UINT32)PC_CD1) == 0) &&
         (PCMCIAHW_QueryPbusSts((UINT32)PC_CD2) == 0) &&
         u4CamPwrOcStatus == FALSE)
    {
        if ( fgCamPwrManualControl == FALSE )
        {
            if ( u4CamPwrStatus != TRUE )
            {
                u4RegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3);
                // Set default ts
                if ( eExternalIcVersion == MT8295_VERSION_AE )
                {
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, u4RegValue & 0xFFFF7BFF);
                }
                else  if ( eExternalIcVersion == MT8295_VERSION_BNE )
                {
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, u4RegValue & 0xFFFFFBFF);
                }
                // Set power on
                if (eExternalIcVersion == MT8295_VERSION_BNE)
                {
                    VERIFY( DRVCUST_OptQuery(eCI5VPowerOnGpio, &u4GpioNum) == 0 );
                    VERIFY( DRVCUST_OptQuery(eCI5VPowerOnPolarity, &u4PwrOnPolarity) == 0 );
                    GPIO_SetOut((INT32)u4GpioNum, (u4PwrOnPolarity?1:0));    // turn off ci power switch
                }
                else
                {
                    UINT8 u1Val = 7;
                    SPI_DemodSingleWrite(&u1Val, 0x86c);
#if 1
                    // CI_PWR_En use PAD_TP
                    SPI_DemodSingleRead(&u1Val, 0xEBF); // bit[1:0] = 11;
                    u1Val |= 0x3;
                    SPI_DemodSingleWrite(&u1Val, 0xEBF); 

                    SPI_DemodSingleRead(&u1Val, 0xEC1);
                    u1Val |= 1;
                    SPI_DemodSingleWrite(&u1Val, 0xEC1);
#else
                    SPI_DemodSingleWrite(&u1Val, 0xf81);   // CI_PWR_En use CHIP_CTRL
#endif
                }
                
                u4CamPwrStatus = TRUE;
                LOG(0, "CI 5V Power On\n");
                PCMCIA_NfyCamStatusCbk(PCMCIA_CAM_CONNECT);
            }
        }
    }
    else    // CAM Not Connected.
    {
        if ( fgCamPwrManualControl == FALSE )
        {
            if ( u4CamPwrStatus != FALSE )
            {
                // Set power off
                if (eExternalIcVersion == MT8295_VERSION_BNE)
                {
                    VERIFY( DRVCUST_OptQuery(eCI5VPowerOnGpio, &u4GpioNum) == 0 );
                    VERIFY( DRVCUST_OptQuery(eCI5VPowerOnPolarity, &u4PwrOnPolarity) == 0 );
                    GPIO_SetOut((INT32)u4GpioNum, (u4PwrOnPolarity?0:1));    // turn off ci power switch
                }
                else
                {
                    UINT8 u1Val=5;
                    SPI_DemodSingleWrite(&u1Val, 0x86c);
#if 1
                    SPI_DemodSingleRead(&u1Val, 0xEBF); // bit[1:0] = 11;
                    u1Val |= 0x3;
                    SPI_DemodSingleWrite(&u1Val, 0xEBF);
 
                    SPI_DemodSingleRead(&u1Val, 0xEC1);
                    u1Val &= 0xFE;
                    SPI_DemodSingleWrite(&u1Val, 0xEC1);
#else
                    SPI_DemodSingleWrite(&u1Val, 0xf81);   // CI_PWR_En
#endif
                }
                
                u4RegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3);
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, u4RegValue | 0x400);
                u4CamPwrStatus = FALSE;
                LOG(0,"CI 5V Power Off\n");
                PCMCIA_NfyCamStatusCbk(PCMCIA_CAM_DISCONNECT);
            }
        }
    }
    return;
}

void PCMCIA_SetDriving(UINT32 u4Driving)
{
    UINT8 u1Val;
    
    // MT5135 Setting
    if ( (eExternalIcVersion == MT5135_VERSION_AE) || (eExternalIcVersion == MT5175_VERSION_AE) )
    {
        switch (u4Driving)
        {
            case 8:
                u1Val = 0x60;
                break;
            case 6:
                u1Val = 0x20;
                break;
            case 4:
                u1Val = 0x40;
                break;
            case 2:
                u1Val = 0x0;
                break;
            default:
                LOG(0, "PCMCIA_SetDrving(): Current value error in drv-cust file\n");
                u4Driving = 2;
                u1Val = 0;
                break;
        }
        LOG(0, "MT5135 Driving Setting %dmA\n", u4Driving);
        
        // CI TS : 0xf04, 0xf05, 0xf06, 0xf07
        // Demod TS : 0xf08, 0xf09, 0xf0a, 0xf0b
        // CPU Interface : 0xf17 ~ 0xf2e, 0xf33 ~ 0xf41, 0xf44 ~
        SPI_DemodSingleWrite(&u1Val, 0xf4f);
        SPI_DemodSingleWrite(&u1Val, 0xf51);
        SPI_DemodSingleWrite(&u1Val, 0xf53);
        SPI_DemodSingleWrite(&u1Val, 0xf19);
        SPI_DemodSingleWrite(&u1Val, 0xf1b);
        SPI_DemodSingleWrite(&u1Val, 0xf1d);
        SPI_DemodSingleWrite(&u1Val, 0xf1f);
        SPI_DemodSingleWrite(&u1Val, 0xf21);
        SPI_DemodSingleWrite(&u1Val, 0xf4d);
        SPI_DemodSingleWrite(&u1Val, 0xf4b);
        SPI_DemodSingleWrite(&u1Val, 0xf40);
        SPI_DemodSingleWrite(&u1Val, 0xf3b);
        SPI_DemodSingleWrite(&u1Val, 0xf39);
        SPI_DemodSingleWrite(&u1Val, 0xf2a);
        SPI_DemodSingleWrite(&u1Val, 0xf2c);
        SPI_DemodSingleWrite(&u1Val, 0xf2e);
        SPI_DemodSingleWrite(&u1Val, 0xf34);
        SPI_DemodSingleWrite(&u1Val, 0xf36);
        SPI_DemodSingleWrite(&u1Val, 0xf38);
        SPI_DemodSingleWrite(&u1Val, 0xf3a);
        SPI_DemodSingleWrite(&u1Val, 0xf3c);
        SPI_DemodSingleWrite(&u1Val, 0xf3e); 
        return;
    }

    // MT8295 Setting
    switch( u4Driving )
    {
        case 0 /*MT8295_IO_0mA*/:
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x00000000 );

            if ( eExternalIcVersion != MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000000 );    
                PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000000 );    
            }
            break;
        case 4 /*MT8295_IO_4mA*/:
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x04104040 );
                            
            if ( eExternalIcVersion != MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000001 );    
                PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00040000 );
            }
            break;
        case 8 /*MT8295_IO_8mA*/:
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x08208080 );
                            
            if ( eExternalIcVersion != MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000002 );    
                PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00080000 );
            }
            break;
        case 12 /*MT8295_IO_12mA*/:
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x0c30c0c0 );
                            
            if ( eExternalIcVersion != MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000003 );
                PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x000C0000 );
            }
            break;
        default:
            LOG(0, "SetDrvingCurrent default 4mA , u4Driving = %d\n",u4Driving);
            //ASSERT(0);
             PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x04104040 );
                            
            if ( eExternalIcVersion != MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000001 );    
                PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00040000 );
            }
            break;
    }
    LOG(0, "MT8295 Driving Setting %dmA\n", u4Driving);
}

#ifdef CC_FAST_INIT
void PCMCIA_Suspend(void)
{
    fgPcmciaInit = FALSE;
    SPI_pm_suspend();
    PCMCIA_GPIO_Suspend();
    return;
}
#endif
#endif

void PCMCIA_SetCiSysClock(UINT32 u4Clock)
{
    UINT8 u1Val;
    switch (u4Clock)
    {
        case 144:
            u1Val = 0x7; // 144 MHz
            break;
        case 108:
            u1Val = 0x6;
            break;
        case 86: // 86.4
            u1Val = 0x5;
            break;
        case 62: // 61.7
            u1Val = 0x4;
            break;
        case 54:
            u1Val = 0x3;
            break;
        case 48:
            u1Val = 0x2;
            break;
        case 72:
            u1Val = 0x1;
            break;
        case 36:
            u1Val = 0xf;
            break;
        case 27:
            u1Val = 0x13;
            break;
        default:
            u1Val = 0x1;
            break;
    }
    SPI_DemodSingleWrite(&u1Val, 0xe59);   // CI SYS CLK
    u1Val = 0x1;
    SPI_DemodSingleWrite(&u1Val, 0xe5a);
}

void PCMCIA_SetCiTsClock(UINT32 u4Clock)
{
    UINT8 u1Val;
    switch (u4Clock)
    {
        case 48:
            u1Val = 0x7; // 144 MHz
            break;
        case 62: //61.7
            u1Val = 0x6;
            break;
        case 72: 
            u1Val = 0x5;
            break;
        case 86:  //86.4
            u1Val = 0x4;
            break;
        case 108:
            u1Val = 0x3;
            break;
        case 144:
            u1Val = 0x2;
            break;
        case 54:
            u1Val = 0x1;
            break;
        case 36:
            u1Val = 0xf;
            break;
        case 27:
            u1Val = 0x13;
            break;
        default:
            u1Val = 0x5;
            break;
    }
    SPI_DemodSingleWrite(&u1Val, 0xe5b);   // CI TS CLK
    u1Val = 0x1;
    SPI_DemodSingleWrite(&u1Val, 0xe5c);

    switch (u4Clock)
    {
        case 72:
            u1Val = 0x9;
            break;
        case 108:
            u1Val = 0x7;
            break;
        case 144:
            u1Val = 0x5;
            break;
        default:
            u1Val = 0x9;
            break;
    }
    SPI_DemodSingleWrite(&u1Val, 0xe5f);
    u1Val = 0x1;
    SPI_DemodSingleWrite(&u1Val, 0xe60);
}

static INT32 PCMCIA_MT8295_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity/*0:input, 1:output, null:query*/)
{
    //ASSERT(i4GpioNum>= MT8295_GPIO0);
    //ASSERT(i4GpioNum<=MT8295_GPIO_MAX);
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=14);
    ASSERT( pi4Polarity==NULL || *pi4Polarity==1 || *pi4Polarity==0 );    
    PCMCIA_GPIO_Init();
    
    INT32 i4Ret;
    UINT32 u4EnbValue , u4Val;
    UINT32 u4EnbMask = (1<<i4GpioNum)<<15;
    UINT32 u4RegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET1);
    if( pi4Polarity == NULL )// just query
    {
        i4Ret = ( (u4RegValue & u4EnbMask)>0?1:0 );
        return i4Ret;
    }

    u4EnbValue = (*pi4Polarity<<i4GpioNum)<<15;     
    u4Val = u4RegValue & ~u4EnbMask | u4EnbValue;
    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET1,  u4Val );
    return (*pi4Polarity);
}

static INT32 PCMCIA_MT8295_GPIO_Input(INT32 i4GpioNum)
{
    //ASSERT(i4GpioNum>= MT8295_GPIO0);
    //ASSERT(i4GpioNum<=MT8295_GPIO_MAX);
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=14);
    INT32 i4Ret;
    PCMCIA_GPIO_Init();

    UINT32 u4InMask = (1<<i4GpioNum);
    UINT32 u4InRegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET4);

    i4Ret = ((u4InRegValue & u4InMask)>0?1:0);
    return i4Ret;  
}

static INT32 PCMCIA_MT8295_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow/*0:drive0, 1:drive 1, null:query*/) 
{
    //ASSERT(i4GpioNum>= MT8295_GPIO0);
    //ASSERT(i4GpioNum<=MT8295_GPIO_MAX);
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=14);
    ASSERT( pi4DriveHighLow==NULL || *pi4DriveHighLow==1 || *pi4DriveHighLow==0 );    
    PCMCIA_GPIO_Init();
    
    INT32 i4Ret;
    INT32 i4Idx;
    UINT32 u4OutValue, u4Val;
    UINT32 u4OutMask = (1<<i4GpioNum);    
    UINT32 u4RegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET1);
    if( pi4DriveHighLow == NULL)
    {
        i4Ret = ( (u4RegValue & u4OutMask)>0?1:0 );
        return i4Ret;
    }
    u4OutValue = (*pi4DriveHighLow<<i4GpioNum);  
    u4Val = u4RegValue & ~u4OutMask | u4OutValue;
    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET1,  u4Val);

    // Set the output mode.
    i4Idx = 1;
    VERIFY(i4Idx==PCMCIA_MT8295_GPIO_Enable(i4GpioNum, &i4Idx));

    return (*pi4DriveHighLow);
}

static INT32 PCMCIA_MT8295_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Reg;
    //INT32 i4GpioIndex;
    /*---check parameters----*/
    //ASSERT(i4GpioNum>= MT8295_GPIO0);
    //ASSERT(i4GpioNum<=MT8295_GPIO_MAX);
    ASSERT(i4Gpio>= 0);
    ASSERT(i4Gpio<=14);
    ASSERT( pfgSet ==NULL  || *pfgSet==1 || *pfgSet==0 );
    PCMCIA_GPIO_Init();


    /*-----manipulate registers------*/
    u4Reg  = PCMCIAHW_READ32(REG_GPIO_INT_ENABLE);
    if( pfgSet ==NULL)  //query
    {
        return ( u4Reg & (1U<<i4Gpio) );
    }
    else
    {
        PCMCIAHW_WRITE32( REG_GPIO_INT_ENABLE, u4Reg&~(1U<<i4Gpio)|(*pfgSet<<i4Gpio) );        //clean bit&set bit
    }
#ifndef CC_MTK_LOADER
    LOG(7, "PCMCIA_MT8295_GPIO_Intrq: (REG_GPIO_INT_ENABLE)=%08X\n", PCMCIAHW_READ32(REG_GPIO_INT_ENABLE));
#endif
    return (*pfgSet);    
}

static INT32 PCMCIA_MT8295_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    INT32 fgSet; 
    /*----check parameters-----*/
    //ASSERT(i4GpioNum>= MT8295_GPIO0);
    //ASSERT(i4GpioNum<=MT8295_GPIO_MAX);
    ASSERT(i4Gpio>= 0);
    ASSERT(i4Gpio<=14);
    ASSERT(eType==GPIO_TYPE_INTR_FALL || eType==GPIO_TYPE_INTR_RISE \
        || eType==GPIO_TYPE_INTR_BOTH || eType==GPIO_TYPE_INTR_BOTH ||  eType==GPIO_TYPE_NONE );
    PCMCIA_GPIO_Init();

    if( i4Gpio>14 || i4Gpio<0 )
    {
        return -1;
    }
    
    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
    case GPIO_TYPE_INTR_RISE:
    case GPIO_TYPE_INTR_BOTH:
        ar8295_GpioType[i4Gpio]=eType;
        fgSet = 1;
        PCMCIA_MT8295_GPIO_Intrq( i4Gpio, &fgSet );
        break;
    case GPIO_TYPE_NONE:
        ar8295_GpioType[i4Gpio]=GPIO_TYPE_NONE; //clean
        ar8295_GpioCallback[i4Gpio] = NULL;                
        fgSet = 0;
        PCMCIA_MT8295_GPIO_Intrq( i4Gpio, &fgSet );
        break;
//    case GPIO_TYPE_INTR_LEVEL:
        //not support
    default:
        return -2;
    }

    if (pfnCallback)
    {
        ar8295_GpioCallback[i4Gpio] = pfnCallback;
    }
    return 0;
}

INT32 PCMCIA_MT8295_GPIO_Range_Check(INT32 i4GpioNum)
{
    PCMCIA_GPIO_Init();

    if (eExternalIcVersion == MT8295_VERSION_BNE) // MT5135
    {
        if ((i4GpioNum >= MT8295_GPIO0) && (i4GpioNum <= MT8295_GPIO_MAX))
        {
            return 1;
        }
    }
    return 0;
}

#ifdef CC_FAST_INIT
INT32 PCMCIA_GPIO_Suspend(void)
{
    fgExternalIcGpioInit = 0;
    return 0;
}

INT32 PCMCIA_GPIO_Resume(void)
{
    PCMCIA_GPIO_Init();
}

#endif

#if 0
static void PCMCIA_SetParallelTsIn(BOOL fgParallel)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        if (fgParallel)
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)|0x40000000 );
        }
        else // Serial TS in, 0x94 [15:14] = 0, 0x88[30] = 0
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3) & 0xFFFD3FFF);
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&(~0x40000000));
        }
    }
    else // Internal CI
    {
    }
}

static void PCMCIA_SetTsOut(BOOL fgParallel)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        if (fgParallel)
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)|0x40000000 );
            
            if ( eExternalIcVersion != MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)&0xFFFF3FFF );
            }
        }
        else
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&(~0x40000000));
            
            if ( eExternalIcVersion != MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)|0x0000C000 );
            }
        }
    }
    else
    {
    }
}
#endif

#endif // PCMCIA_ENABLED


#ifndef CC_MTK_LOADER
static void PCMCIA_SetTsRouting_5135_2bit(BOOL fgTsGoThroughCam, TS_FMT_T *prTsFmt)
{
#if defined(CC_EXTERNAL_IC_MT5135) || defined(CC_EXTERNAL_IC_MT5175)
    UINT8  u1RegValue;
    UINT32 u4PositiveEdge = 1, u4TsFreq, u4RegValue, u4Clock;
    UINT8  u1BaseReg = 0;

    if (eExternalIcVersion == MT5135_VERSION_AE)
    {
        u1BaseReg = 0xa80;
    }
    else // mt5175
    {
        u1BaseReg = 0xdd0;
    }
    u4Clock = 0;
    DRVCUST_OptQuery(eCITsClock, &u4Clock);
    switch (u4Clock)
    {
        case 48:
            u1RegValue = 0xc; // 144 MHz
            break;
        case 62: //61.7
            u1RegValue = 0xa;
            break;
        case 72: 
            u1RegValue = 0x9;
            break;
        case 86:  //86.4
            u1RegValue = 0x8;
            break;
        case 108:
            u1RegValue = 0x7;
            break;
        case 144:
            u1RegValue = 0x5;
            break;
        case 54:
            u1RegValue = 0xb;
            break;
        case 36:
            u1RegValue = 0xf;
            break;
        case 27:
            u1RegValue = 0x13;
            break;
        default:
            u4Clock = 72;
            u1RegValue = 0x9;
            break;
    }
    // Set spectrun spread clock
    SPI_DemodSingleWrite(&u1RegValue, 0xe5f);
    u1RegValue = 0x1;
    SPI_DemodSingleWrite(&u1RegValue, 0xe60);
    
    DRVCUST_OptQuery(eDmxFramerSet0PostiveEdge, &u4PositiveEdge);

    // mt5135 demod
    if (prTsFmt->SerOrPar == SP_SERIAL)
    {
#ifdef PCMCIA_ENABLED
        if (fgTsGoThroughCam)
        {
            //PCMCIA_EnableTS(TRUE);
            _PCMCIA_OnOffCamStream(TRUE);
        }
        else
        {
            //PCMCIA_EnableTS(FALSE);
            _PCMCIA_OnOffCamStream(FALSE);
        }
#endif

        // Set CI TS In/Out. 
        // 0x94[15:14] = 0 -- serial 0 input 
        // 0x94[17] = 1 -- parallel output
        PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3) & 0xFFFF3FFF | 0x20000);
        LOG(1, "Set CI TS In: Serial 0, TS Out: Parallel.\n");
        // 0x88[30] = 0 -- serial in
        PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & 0xBFFFFFFF);

        // Serial TS go through 2 bit normal path to main chip
        SPI_DemodSingleRead(&u1RegValue, 0xe00);
        u1RegValue |= 0x12;
        LOG(1, "Set CI Ts to main chip.\n");
        SPI_DemodSingleWrite(&u1RegValue, 0xe00);

        // Set 0xa80 = 0x26 -- positive, 0x6 -- negative
        if (u4PositiveEdge == 1)
        {
            u1RegValue = 0x26;
        }
        else
        {
            u1RegValue = 0x6;
        }
        SPI_DemodSingleWrite(&u1RegValue, u1BaseReg);
        
        // Set 0xa81 = 0x3
        u1RegValue = 0x3;
        SPI_DemodSingleWrite(&u1RegValue, u1BaseReg+1);

        // Set 0xa82 = 0
        u1RegValue = 0x40;
        SPI_DemodSingleWrite(&u1RegValue, u1BaseReg+2);
        LOG(1, "Set CI Ts In: tsif.\n");

        // Set 0xa83 = 0x80
        u1RegValue = 0x80;
        SPI_DemodSingleWrite(&u1RegValue, u1BaseReg+3);

        LOG(1, "Dmx Set Front End EX(1, %d).\n", DMX_FE_EXT_2BIT);
        DMX_SetFrontEndEx(0, DMX_FE_EXT_2BIT);
    }
    else // SP_PARALLEL -- DVBS ... 3rd party demod
    {
        if (prTsFmt->ClockPolarity == POL_LOW_ACTIVE)
        {
            SPI_DemodSingleRead(&u1RegValue, 0xE87);
            u1RegValue |= 0x2;
            SPI_DemodSingleWrite(&u1RegValue, 0xE87);
        }

        if (prTsFmt->fgExternalDemod == BOOL_TRUE)
        {
        u4TsFreq = prTsFmt->u4TsClockInKHz*8;

        // Freq in TsFmt is higher than in customization header file,
        // rewrite the spectrum spread clock.
        if (u4TsFreq > (u4Clock * 10))
        {
            if (u4TsFreq <=72000)
            {
                u1RegValue = 0x9;
            }
            else if (u4TsFreq <=108000)
            {
                u1RegValue = 0x7;
            }
            else 
            {
                u1RegValue = 0x5;
            }
            SPI_DemodSingleWrite(&u1RegValue, 0xe5f);
            u1RegValue = 0x1;
            SPI_DemodSingleWrite(&u1RegValue, 0xe60);
        }
        }
            
        if (fgTsGoThroughCam)
        {
// 0xa82=0x80
// 0x88[30]=1
// 0x94[17]=1
// 0xe00[4,1] = 0x12
// 0xa80 = 0            
            u4RegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0);
            // Set CI parallel TS In . 0x88[30] = 1
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, u4RegValue | 0x40000000);

            u4RegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3);
            // Set CI parallel TS Out. 0x94[17] = 1
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, u4RegValue | 0x20000);

            SPI_DemodSingleRead(&u1RegValue, u1BaseReg+2);
            if (prTsFmt->fgExternalDemod == BOOL_TRUE)
            {
                u1RegValue = 0x88;
            }
            else if(prTsFmt->fgExternalDemod == BOOL_FALSE)
            {
                u1RegValue = 0x40;
            }
            SPI_DemodSingleWrite(&u1RegValue, u1BaseReg+2);
            
            SPI_DemodSingleRead(&u1RegValue, 0xe00);
            u1RegValue |= 0x12;
            SPI_DemodSingleWrite(&u1RegValue, 0xe00);
        }
        else
        {
            //0xe00[4, 1] = 0x10
            SPI_DemodSingleRead(&u1RegValue, 0xe00);
            if (prTsFmt->fgExternalDemod == BOOL_TRUE)
            {
                u1RegValue = (u1RegValue | 0x10) & 0xFD;
            }
            else if(prTsFmt->fgExternalDemod == BOOL_FALSE)
            {
                u1RegValue = (u1RegValue & 0xED) | 0x2;
            }
            SPI_DemodSingleWrite(&u1RegValue, 0xe00);   

            // Power down spectrum spread clock.
            u1RegValue = 0x80;
            SPI_DemodSingleWrite(&u1RegValue, 0xe66);
            u1RegValue = 0x1;
            SPI_DemodSingleWrite(&u1RegValue, 0xe67);
            LOG(1, "Power down spectrum spread clock, 0xe66=0x80.\n");
            // 0xa82[5:4] = 1, select spectrum spread clock to disable ts output.
            u1RegValue = 0x10;
            SPI_DemodSingleWrite(&u1RegValue, u1BaseReg+2);
            LOG(1, "Select spectrum spread clock to disable ts output, 0xa82=0x10.\n");
        }

        if (u4PositiveEdge)
        {
            u1RegValue = 0x26;
        }
        else
        {
            u1RegValue = 0x6;
        }
        SPI_DemodSingleWrite(&u1RegValue, 0xa80);
        LOG(1, "Dmx Set Front End EX(1, %d).\n", DMX_FE_EXT_2BIT);
        DMX_SetFrontEndEx(0, DMX_FE_EXT_2BIT);
    }   
#endif
}

void PCMCIA_SetTsRouting(BOOL fgTsGoThroughCam, TS_FMT_T *prTsFmt)
{
    if (fgDaughterBoardConnected == FALSE)
    {
        return;
    }
    // External Demod: DVBC, DVBT, DTMB, DVBS
    PCMCIA_SetTsRouting_5135_2bit(fgTsGoThroughCam, prTsFmt);
}
#endif

void PCMCIA_PinMux()
{   
}

#ifdef CC_MTK_LOADER
static INT32 _ExternalIc_CheckAlive(void)        //check hw status
{
    UINT32 u4Reg = (UINT32)0xffffffff;
    INT32 i4Ret  = POD_OK ;
    UINT8 u1Cnt  = 10;

    for( ;u1Cnt>0;u1Cnt--)            //read mutiple times, should be the same
    {
        if ( eExternalIcVersion == MT5135_VERSION_AE )
        {
            u4Reg = PCMCIAHW_READ32(REG_VERSION_CODE);
            i4Ret = ((u4Reg&0x000000ff)==0x35)? POD_OK : POD_INIT_FAILED ;
        }
        else if ( eExternalIcVersion == MT5175_VERSION_AE )
        {
            u4Reg = PCMCIAHW_READ32(REG_VERSION_CODE);
            i4Ret = ((u4Reg&0x000000ff)==0x75)? POD_OK : POD_INIT_FAILED ;
        }
#ifdef PCMCIA_ENABLED
        else if ( eExternalIcVersion == MT8295_VERSION_BNE )
        {
            u4Reg = PCMCIAHW_READ32(REG_CKGEN_CHIP_VERSION);
            i4Ret = ((u4Reg&0x0000ffff)==0xdead)? POD_OK : POD_INIT_FAILED ;
        }
        if ( eExternalIcVersion == MT5135_VERSION_AE )
        {
            u4Reg = PCMCIAHW_READ32(REG_VERSION_CODE);
            i4Ret = ((u4Reg&0x000000ff)==0x35)? POD_OK : POD_INIT_FAILED ;
        }
        else if ( eExternalIcVersion == MT8295_VERSION_AE )
        {
            u4Reg = PCMCIAHW_READ32(REG_VERSION_CODE);
            i4Ret = ((u4Reg&0x000000ff)==0x08)? POD_OK : POD_INIT_FAILED ;
        }
#endif
        else
        {
            Printf("_ExternalIc_CheckAlive: unknown external IC\n");
            i4Ret = POD_INIT_FAILED;
        }
        
        if(i4Ret !=POD_OK)
        {
            break;
        }
    }

    if(i4Ret !=POD_OK)
    {
        Printf("External CI is dead !\n"  );
        Printf("Version Code : 0x%x\n", u4Reg);
        return i4Ret;
    }

    return i4Ret;
}
#endif

static void _ExternalIc_Reset(void)
{
    UINT32 u4GpioNum;
    UNUSED(u4GpioNum);

#if defined(CC_EXTERNAL_IC_MT5135) || defined(CC_MT5135CI)
    eExternalIcVersion = MT5135_VERSION_AE;
    Printf("External CI Version : MT5135\n");
    #ifdef CC_MTK_LOADER
    if (DRVCUST_InitQuery(eDemodResetGpio, &u4GpioNum) == 0)
    {
        GPIO_SetOut(u4GpioNum, 0);
        HAL_Delay_us(20000); //delay 20ms
        GPIO_SetOut(u4GpioNum, 1);
        Printf("PCMCIA Reset Demod GPIO=%d, 0->1\r\n",u4GpioNum);
    }
    else
    {
        Printf("_ExternalIc_Reset: MT5135 Reset Gpio not set\n");
    }
    #endif //CC_MTK_LOADER
#elif defined(CC_EXTERNAL_IC_MT5175) || defined(CC_MT5175CI)
    eExternalIcVersion = MT5175_VERSION_AE;
    Printf("External CI Version : MT5175\n");
    #ifdef CC_MTK_LOADER
    if (DRVCUST_InitQuery(eDemodResetGpio, &u4GpioNum) == 0)
    {
        GPIO_SetOut(u4GpioNum, 0);
        HAL_Delay_us(20000); //delay 20ms
        GPIO_SetOut(u4GpioNum, 1);
        Printf("PCMCIA Reset Demod GPIO=%d, 0->1\r\n",u4GpioNum);
    }
    #else
    {
        Printf("_ExternalIc_Reset: MT5175 Reset Gpio not set\n");
    }
    #endif //CC_MTK_LOADER
#elif defined(PCMCIA_ENABLED) && defined(CC_EXTERNAL_IC_MT8295)
    eExternalIcVersion = MT8295_VERSION_BNE;
    Printf("External CI Version : MT8295 BNE\n");
    #ifdef CC_MTK_LOADER
    if (DRVCUST_InitQuery(eMT8295ResetGpio, &u4GpioNum) == 0)
    {
        //LOG(3, "_ExternalIc_Reset: u4GpioNum: %d \n", u4GpioNum);
        GPIO_SetOut((INT32)u4GpioNum, 0);       //reset
        x_thread_delay(10);
        GPIO_SetOut((INT32)u4GpioNum, 1);       //~reset
        x_thread_delay(10);
        Printf("PCMCIA Reset Demod GPIO=%d, 0->1\r\n",u4GpioNum);
    }     
    else
    {
        Printf("_ExternalIc_Reset: MT8295 Reset Gpio not set\n");
    }
    #endif // CC_MTK_LOADER
#else // PCMCIA_ENABLED
    Printf("_ExternalIC_Reset: unknown external IC\n");
#endif

#ifdef PCMCIA_ENABLED
    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_POD )
    {
        _PCMCIA_ResetMmx();
        _PCMCIA_ResetFramer();
        _PCMCIA_ResetMDmx();
        _PCMCIA_ResetCmp();
        x_thread_delay(10);
    }
#endif
}

static void _ExternalIc_Init(void)
{
    if ( fgExternalIcInited )
    {
        return;
    }
    
    fgExternalIcInited = TRUE;

    #if defined(CC_MT5360) || defined(CC_MT5391) || defined(CC_MT5392B)
        PCMCIAHW_SetIfType(PCMCIA_IF_TYPE_NFI);
        NFI_Init();
    #else
        PCMCIAHW_SetIfType(PCMCIA_IF_TYPE_SPI);
        SPI_Init();
    #endif
    
    _ExternalIc_Reset();
    /*-------------External Ic should be readable so far---------------*/
#ifdef PCMCIA_ENABLED
    _ExternalIc_SetPll();
#endif
#ifdef CC_MTK_LOADER
    SPI_SetPll();
#endif

#ifdef CC_MTK_LOADER
    if ( _ExternalIc_CheckAlive() != 0 )
    {
        //ASSERT(0);
        fgDaughterBoardConnected = FALSE;
        Printf("ExternalIc_Init: Daughter board is not connected.\n");
        return;
    }
    Printf("External IC Initilization done\n");
#else
    LOG(0, "External IC Initilization done\n");
#endif
}

static INT32 PCMCIA_MT5135_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity/*0:input, 1:output, null:query*/)
{
    INT32  i4Ret;
    UINT8  u1EnbMask; 
    UINT32 u4RegAddr = GPIO_CTRL_REG_BASE + i4GpioNum;
    UINT8  u1RegValue, u1Val, u1Polarity;
    
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<= 30);    
    ASSERT( pi4Polarity==NULL || *pi4Polarity==1 || *pi4Polarity==0 );   
    PCMCIA_GPIO_Init(); 
    
        u1EnbMask = 1<<2; 
    SPI_DemodSingleRead(&u1RegValue, u4RegAddr);
    if( pi4Polarity == NULL )// just query
    {
        i4Ret = ( (u1RegValue & u1EnbMask)>0?1:0 );
        return i4Ret;
    }
    u1Polarity = (UINT8)(*pi4Polarity);
    u1Val = u1RegValue | u1EnbMask;
    u1Val &= 0xFE;
    u1Val |= u1Polarity;
    SPI_DemodSingleWrite(&u1Val, u4RegAddr);
    
    return (*pi4Polarity);
}

static INT32 PCMCIA_MT5135_GPIO_Input(INT32 i4GpioNum)
{
    UINT32 u4RegAddr = GPIO_CTRL_REG_BASE + i4GpioNum;
    UINT8  u1InMask = (1<<4); // bit 4: input value
    UINT8  u1InRegValue;
    
    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=30);
    INT32 i4Ret;
    PCMCIA_GPIO_Init();

    SPI_DemodSingleRead(&u1InRegValue, u4RegAddr);
    i4Ret = ((u1InRegValue & u1InMask)>0?1:0);
    return i4Ret;  
}

static INT32 PCMCIA_MT5135_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow/*0:drive0, 1:drive 1, null:query*/) 
{
    INT32 i4Ret;
    INT32 i4Idx;
    UINT8 u1OutValue, u1Val;
    UINT8  u1RegValue, u1OutMask = (1<<1); // bit 1: output value
    UINT32 u4RegAddr = GPIO_CTRL_REG_BASE + i4GpioNum;    

    ASSERT(i4GpioNum>= 0);
    ASSERT(i4GpioNum<=30);
    ASSERT( pi4DriveHighLow==NULL || *pi4DriveHighLow==1 || *pi4DriveHighLow==0  );    
    PCMCIA_GPIO_Init();
    
    // Set the output mode.
    i4Idx = 1;
    VERIFY(i4Idx==PCMCIA_MT5135_GPIO_Enable(i4GpioNum, &i4Idx));
    
    SPI_DemodSingleRead(&u1RegValue, u4RegAddr);
    if( pi4DriveHighLow == NULL)
    {
        i4Ret = ( (u1RegValue & u1OutMask)>0?1:0 );
        return i4Ret;
    }
    
    u1OutValue = (UINT8)((*pi4DriveHighLow)<<1);  
    u1Val = u1RegValue & ~u1OutMask | u1OutValue;
    SPI_DemodSingleWrite(&u1Val, u4RegAddr);
    return (*pi4DriveHighLow);
}

// i4Gpio is the 0-based index, not absolute index.
// Set GPIO Interrupt Enable bit of register REG_GPIO_INT_ENABLE
static INT32 PCMCIA_MT5135_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet)
{
    UINT32 u4Reg;
    INT32  i4GpioInt = i4Gpio - MT5135_INTR_GPIO_MIN;
    /*---check parameters----*/
    ASSERT(i4Gpio>= 0);
    ASSERT(i4Gpio<= 30);
    ASSERT( pfgSet ==NULL  || *pfgSet==1 || *pfgSet==0 );
    PCMCIA_GPIO_Init();

    if( i4Gpio>MT5135_INTR_GPIO_MAX || i4Gpio<MT5135_INTR_GPIO_MIN)
    {
        return -1;
    }

    /*-----manipulate registers------*/
    u4Reg  = PCMCIAHW_READ32(REG_GPIO_INT_ENABLE);
    
    if( pfgSet ==NULL)  //query
    {
        return ( u4Reg & (1U<<i4GpioInt) );
    }
    else
    {
        PCMCIAHW_WRITE32( REG_GPIO_INT_ENABLE, u4Reg&~(1U<<i4GpioInt)|(*pfgSet<<i4GpioInt) );        //clean bit&set bit
    }
#ifndef CC_MTK_LOADER
    LOG(7, "PCMCIA_GPIO_Intrq: (REG_GPIO_INT_ENABLE)=%08X\n", PCMCIAHW_READ32(REG_GPIO_INT_ENABLE));
#endif
    // GPIO Interrupt Enable
    if (*pfgSet == 1)
    {
        INT32 i4Polarity = 0;
        PCMCIA_MT5135_GPIO_Enable(i4Gpio, &i4Polarity);
    }
   
    return (*pfgSet);    
}

// i4Gpio is the 0-based index, not absolute index.
static INT32 PCMCIA_MT5135_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    INT32 fgSet; 
    /*----check parameters-----*/
    ASSERT(i4Gpio>= 0);
    ASSERT(i4Gpio<= 30);
    ASSERT(eType==GPIO_TYPE_INTR_FALL || eType==GPIO_TYPE_INTR_RISE \
        || eType==GPIO_TYPE_INTR_BOTH || eType==GPIO_TYPE_INTR_BOTH ||  eType==GPIO_TYPE_NONE );
    PCMCIA_GPIO_Init();

    if( i4Gpio>MT5135_INTR_GPIO_MAX || i4Gpio<MT5135_INTR_GPIO_MIN )
    {
        return -1;
    }

    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
    case GPIO_TYPE_INTR_RISE:
    case GPIO_TYPE_INTR_BOTH:
        ar5135_GpioType[i4Gpio - MT5135_INTR_GPIO_MIN]=eType;
        fgSet = 1;
        PCMCIA_MT5135_GPIO_Intrq(i4Gpio, &fgSet );
        break;
    case GPIO_TYPE_NONE:
        ar5135_GpioType[i4Gpio - MT5135_INTR_GPIO_MIN]=GPIO_TYPE_NONE; //clean
        ar5135_GpioCallback[i4Gpio - MT5135_INTR_GPIO_MIN] = NULL;                
        fgSet = 0;
        PCMCIA_MT5135_GPIO_Intrq(i4Gpio, &fgSet );
        break;
//    case GPIO_TYPE_INTR_LEVEL:
        //not support
    default:
        return -2;
    }

    if (pfnCallback)
    {
        ar5135_GpioCallback[i4Gpio - MT5135_INTR_GPIO_MIN] = pfnCallback;
    }
    return 0;
}

// i4GpioNum is the absolute gpio index.
INT32 PCMCIA_MT5135_GPIO_Range_Check(INT32 i4GpioNum)
{
    PCMCIA_GPIO_Init();

    // Use assert for checking bug caused by daughter board connection error.
    // Print log and return 0 for running without daughter board.
    //ASSERT(fgDaughterBoardConnected);
    if (!fgDaughterBoardConnected)
    {
#ifdef CC_MTK_LOADER
        Printf("Daughter board is not connected!!\n");
#else
        LOG(0, "Daughter board is not connected!!\n");
#endif
        return 0;
    }
#ifdef PCMCIA_ENABLED
    if (eExternalIcVersion == MT5135_VERSION_AE) // MT5135
#endif
    {
        if ((i4GpioNum >= MT5135_GPIO0) && (i4GpioNum <= MT5135_GPIO_MAX))
        {
            return 1;
        }
    }
    return 0;
}


INT32 PCMCIA_GPIO_Init()
{
#ifdef PCMCIA_ENABLED
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return 0;
    }
#endif
    if( fgExternalIcGpioInit == TRUE)
    {
        return 0;
    }
    
    fgExternalIcGpioInit = TRUE;

    //VERIFY(GPIO_Init()==0);

    // The following two invocation should be placed here if some other module 
    // controls MT8295 GPIO before PCMCIA driver init.
#ifdef CC_MTK_LOADER
    PCMCIA_PinMux();  
    if (DRVCUST_InitGet(eFlagDDRQfp))
    {
        // QFP -- mt5365
        eMainchip = PCMCIA_MAINCHIP_MT5365;
    }
    else
    {
        // BGA -- mt5366
        eMainchip = PCMCIA_MAINCHIP_MT5366;
    }
#endif
    _ExternalIc_Init();
    
    if (fgDaughterBoardConnected == FALSE)
    {
        Printf("PCMCIA_GPIO_Init: Daughter board is not connected.\n");
        return -1;
    }
#ifdef CC_MTK_LOADER    


    if (eExternalIcVersion == MT5135_VERSION_AE && 
        eMainchip == PCMCIA_MAINCHIP_MT5365     &&
        fgDaughterBoardConnected )
    {
        // Set GPIO 3,4,5,6,8,9,10,11 as GPI
        INT32 i, i4Polarity = 0;
        //INT32 ai4GpioNum[] = {MT5135_GPIO3,MT5135_GPIO4,MT5135_GPIO5,MT5135_GPIO6,
        //                                   MT5135_GPIO8,MT5135_GPIO9,MT5135_GPIO10,MT5135_GPIO11};
        INT32 ai4GpioNum[] = {3,4,5,6,8,9,10,11};
        for (i = 0; i < 8; i++)
        {
            PCMCIA_GPIO_Enable(ai4GpioNum[i], &i4Polarity);
        }
    }
#endif
    return 0;
}


INT32 PCMCIA_GPIO_Range_Check(INT32 i4GpioNum)
{
    PCMCIA_GPIO_Init();

    // Use assert for checking bug caused by daughter board connection error.
    // Print log and return 0 for running without daughter board.
    //ASSERT(fgDaughterBoardConnected);
    if (!fgDaughterBoardConnected)
    {
        Printf("[PCMCIA]Daughter board is not connected!!\n");
        return 0;
    }
#ifdef PCMCIA_ENABLED
    if (eExternalIcVersion == MT5135_VERSION_AE) // MT5135
    {
        return PCMCIA_MT5135_GPIO_Range_Check(i4GpioNum);
        }
    else if (eExternalIcVersion == MT8295_VERSION_BNE)// MT8295
        {
        return PCMCIA_MT8295_GPIO_Range_Check(i4GpioNum);
        }
            return 0;
#else
    return PCMCIA_MT5135_GPIO_Range_Check(i4GpioNum);
#endif
        }


INT32 PCMCIA_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity)
{
    PCMCIA_GPIO_Init();
    // Use assert for checking bug caused by daughter board connection error.
    // Print log and return -1 for running without daughter board.
    //ASSERT(fgDaughterBoardConnected);
    if (!fgDaughterBoardConnected)
    {
        Printf("[PCMCIA]Daughter board is not connected!!\n");
        return -1;
    }
#ifdef PCMCIA_ENABLED
    if (eExternalIcVersion == MT5135_VERSION_AE)
    {
        return PCMCIA_MT5135_GPIO_Enable(i4GpioNum, pi4Polarity);
    }
    else if (eExternalIcVersion == MT8295_VERSION_BNE)
    {
        return PCMCIA_MT8295_GPIO_Enable(i4GpioNum, pi4Polarity);
    }

    return -1;
#else
    return PCMCIA_MT5135_GPIO_Enable(i4GpioNum, pi4Polarity);
#endif
}

INT32 PCMCIA_GPIO_Input(INT32 i4GpioNum)
{
    PCMCIA_GPIO_Init();
    // Use assert for checking bug caused by daughter board connection error.
    // Print log and return -1 for running without daughter board.
    //ASSERT(fgDaughterBoardConnected);
    if (!fgDaughterBoardConnected)
    {
        Printf("[PCMCIA]Daughter board is not connected!!\n");
        return -1;
    }
#ifdef PCMCIA_ENABLED
    if (eExternalIcVersion == MT5135_VERSION_AE)
    {
        return PCMCIA_MT5135_GPIO_Input(i4GpioNum);
    }
    else if (eExternalIcVersion == MT8295_VERSION_BNE)
    {
        return PCMCIA_MT8295_GPIO_Input(i4GpioNum);
    }
    return -1;
#else
    return PCMCIA_MT5135_GPIO_Input(i4GpioNum);
#endif
}

INT32 PCMCIA_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow)
{
    PCMCIA_GPIO_Init();
    // Use assert for checking bug caused by daughter board connection error.
    // Print log and return -1 for running without daughter board.
    //ASSERT(fgDaughterBoardConnected);
    if (!fgDaughterBoardConnected)
    {
        Printf("[PCMCIA]Daughter board is not connected!!\n");
        return -1;
    }
#ifdef PCMCIA_ENABLED
    if (eExternalIcVersion == MT5135_VERSION_AE)
    {
        return PCMCIA_MT5135_GPIO_Output(i4GpioNum, pi4DriveHighLow);
    }
    else if (eExternalIcVersion == MT8295_VERSION_BNE)
    {
        return PCMCIA_MT8295_GPIO_Output(i4GpioNum, pi4DriveHighLow);
    }
    return -1;
#else
    return PCMCIA_MT5135_GPIO_Output(i4GpioNum, pi4DriveHighLow);
#endif
}

INT32 PCMCIA_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet)
{
    PCMCIA_GPIO_Init();
    // Use assert for checking bug caused by daughter board connection error.
    // Print log and return -1 for running without daughter board.
    //ASSERT(fgDaughterBoardConnected);
    if (!fgDaughterBoardConnected)
    {
        Printf("[PCMCIA]Daughter board is not connected!!\n");
        return -1;
    }
#ifdef PCMCIA_ENABLED
    if (eExternalIcVersion == MT5135_VERSION_AE)
    {
        return PCMCIA_MT5135_GPIO_Intrq(i4Gpio, pfgSet);
    }
    else if (eExternalIcVersion == MT8295_VERSION_BNE)
    {
        return PCMCIA_MT8295_GPIO_Intrq(i4Gpio, pfgSet);
    }
    return -1;
#else
    return PCMCIA_MT5135_GPIO_Intrq(i4Gpio, pfgSet);
#endif
}

INT32 PCMCIA_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    PCMCIA_GPIO_Init();
    // Use assert for checking bug caused by daughter board connection error.
    // Print log and return -1 for running without daughter board.
    //ASSERT(fgDaughterBoardConnected);
    if (!fgDaughterBoardConnected)
    {
        Printf("[PCMCIA]Daughter board is not connected!!\n");
        return -1;
    }
#ifdef PCMCIA_ENABLED
    if (eExternalIcVersion == MT5135_VERSION_AE)
    {
        return PCMCIA_MT5135_GPIO_Reg(i4Gpio, eType, pfnCallback);
    }
    else if (eExternalIcVersion == MT8295_VERSION_BNE)
    {
        return PCMCIA_MT8295_GPIO_Reg(i4Gpio, eType, pfnCallback);
    }
    return -1;
#else
    return PCMCIA_MT5135_GPIO_Reg(i4Gpio, eType, pfnCallback);
#endif
}

INT32 PCMCIA_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    UNUSED(i4Gpio);
    UNUSED(*pi4Intr);
    UNUSED(*pi4Fall);
    UNUSED(*pi4Rise);
    return 0;
}


