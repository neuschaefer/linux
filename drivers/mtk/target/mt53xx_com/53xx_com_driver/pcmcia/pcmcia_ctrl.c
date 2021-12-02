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
#include "x_pdwnc.h"
#include "hw_ckgen.h"
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


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#if defined(CC_EXTERNAL_CI) || defined(CC_EXTERNAL_POD)
extern PFN_GPIO_CALLBACK    ar8295_GpioCallback[TOTAL_8295_GPIO_NUM];
extern GPIO_TYPE            ar8295_GpioType[TOTAL_8295_GPIO_NUM];
#endif

static HANDLE_T sema_Mt8295Intrq = NULL;
static HANDLE_T hCmdDoneSema = NULL;
static HANDLE_T hMT8295IntrqThread;
static BOOL     fgCamPwrManualControl = FALSE;
static BOOL     fgThreadNoAssert = FALSE;
static BOOL     fgMt8295Inited = FALSE;
static BOOL     fgMT8295ThreadStarted = FALSE;
static BOOL     fgPcmciaInit = FALSE;
static BOOL     fgMt8295GpioInit = FALSE;
static BOOL     fgMT8295IntrqThreadKeepRunning = FALSE;
static BOOL     fgMT8295IntrqThreadTerminated = TRUE;
static INT32    u4CamPwrStatus = -1;
extern MT8295_VERSION eMt8295Version;

static void     (*vMt8295IsrSifFct)  (UINT16  ui2_vector_id);
static void     (*vMt8295IsrIreqFct) (UINT16  ui2_vector_id);


// Note ! There are two kinds of PCMCIA interrupt mode : 
//  1. External CI/POD Interrupt mode : 
//      Handled in this file : _MT8295_IntrqIsr(), _MT8295_IntrThreadMain()
//      An command done interrupt pin is required to connect from 8295 to main chip.
//  2. Internal CI/POD Interrupt mode : 
//      Handled in pcmcia_isr.c : _PCMCIA_Isr()
//
// By default, we use polling mode for external CI/POD, and interrupt mode for 
// internal CI/POD. This is initialized in PCMCIA_Initialize().
// Of couse, if PCB support, we can enable interrupt mode in external CI/POD.
// In internal CI/POD, we always use interrupt mode.

PCMCIA_DRIVER_CONFIG_T tPcmciaDriverConfig;


static void _MT8295_ResetMmx(void);
static void _MT8295_ResetFramer(void);
static void _MT8295_ResetMDmx(void);
static void _MT8295_ResetCmp(void);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _MT8295_Reset(void);

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

// MT5360+MT8295
static void _PCMCIA_PinMux_8()
{
    UNUSED(_PCMCIA_PinMux_8);
#ifdef CC_MT5360
    LOG(0, "---------------PCMCIA_PinMux: MT5360+MT8295 -------------------\n");
    BSP_PinSet( PARB_I, PINMUX_FUNCTION0 );     //ci_rb
    BSP_PinSet( POWE_O, PINMUX_FUNCTION0 );     //ci_web
    BSP_PinSet( PALE_O , PINMUX_FUNCTION0 );    //ci_ale
    BSP_PinSet( PCLE_O , PINMUX_FUNCTION0 );    //ci_cle

    BSP_PinSet( PDD2_IO , PINMUX_FUNCTION0 );   //ci_data2
    BSP_PinSet( PDD3_IO , PINMUX_FUNCTION0 );   //ci_data3
    BSP_PinSet( PDD4_IO , PINMUX_FUNCTION0 );   //ci_data4
    BSP_PinSet( PDD5_IO , PINMUX_FUNCTION0 );   //ci_data5
    BSP_PinSet( PDD6_IO , PINMUX_FUNCTION0 );   //ci_data6
    BSP_PinSet( PDD7_IO , PINMUX_FUNCTION0 );   //ci_data7

    /* PAD Driving capability Configuration for PBI Control group
        PBI Control group pins are POCE0_, POCE1_, POCE2_, POOE_, POWE_, PAALE, PACLE
        --> 6mA*/
    CKGEN_WRITE32( 0x320, (CKGEN_READ32(0x320) & ~(0x3<<2)) | (0x1<<2) );

    /*PAD Driving capability Configuration for PBI Data group
        PBI Data group pins are PDD7~PDD0
        -->6mA */
    CKGEN_WRITE32( 0x320, (CKGEN_READ32(0x320) & ~(0x3<<4)) | (0x1<<4) );

    /* PAD Driving capability Configuration for GPIO group
        GPIO group pins are GPIO0~GPIO19
        -->6mA */
    CKGEN_WRITE32( 0x320, (CKGEN_READ32(0x320) & ~(0x3<<12)) | (0x2<<12) );

    VERIFY(  (CKGEN_READ32(0x300)&(0x3<<14))==0 );//GPIO0 must NO PULL
#endif
}

// 5392B+8295
static void _PCMCIA_PinMux_10()
{
    UNUSED(_PCMCIA_PinMux_10);
#if defined(PCMCIA_TYPE_EXTERNAL) && defined(CC_MT5392B)
    LOG(0, "---------------PCMCIA_PinMux: 5392B+8295 -------------------\n");
    UINT32 u4Current=0;
    
    BSP_PinSet( ONDA_03_O, PINMUX_FUNCTION5 );      //ci_oeb, ci_data0, ci_data1
    BSP_PinSet( POWE_O, PINMUX_FUNCTION2 );         //ci_web, ci_ale, ci_cle
    BSP_PinSet( PDD2_IO, PINMUX_FUNCTION2 );        //ci_data2, ci_data3, ci_data4, ci_data5, ci_data6, ci_data7

    BSP_PinSet( TS0_CLK_I, PINMUX_FUNCTION1 );      //TS0_CLK(I)
    BSP_PinSet( TS0_VALID_I , PINMUX_FUNCTION1 );   //TS0_VALID(I)
    BSP_PinSet( TS0_SYNC_I , PINMUX_FUNCTION1 );    //TS0_SYNC(I)
    BSP_PinSet( TS0_DATA_I , PINMUX_FUNCTION1 );    //TS0_DATA(I)

    BSP_PinSet( PINMUX_GPIO_06 , PINMUX_FUNCTION2 );   //TSO_DVBT_DATA(O)
    BSP_PinSet( PINMUX_GPIO_23 , PINMUX_FUNCTION2 );   //TSO_DVBT_CLK(O)
    BSP_PinSet( PINMUX_GPIO_24 , PINMUX_FUNCTION2 );   //TSO_DVBT_VALID(O)
    BSP_PinSet( PINMUX_GPIO_25 , PINMUX_FUNCTION2 );   //TSO_DVBT_SYNC(O)

    /*One Nand Pins -->4mA */
    CKGEN_WRITE32( 0x320, (CKGEN_READ32(0x320) & ~(0x3<<22)) | (0x2<<22) );

    /* PAD Driving capability Configuration for GPIO group
        GPIO group pins are GPIO0~26
        -->default is 2mA */
    if (DRVCUST_OptQuery(eMT8295TSDrivingCurrent, &u4Current) == 0)
    {
        CKGEN_WRITE32( 0x320, (CKGEN_READ32(0x320) & ~(0x3<<14))
                        | (_PCMCIA_DrvCurrentTrans(u4Current)<<14) );
    }

    /* PAD Driving capability Configuration for  PBIC/PBID group
        PBIC group pins are POCE0, POCE1, POOE, POWE, PAALE, PACLE, PARB
        PBI Data group pins are PDD7~PDD0
        -->default is 6mA */
    if (DRVCUST_OptQuery(eMT8295PBIDrivingCurrent, &u4Current) == 0)
    {
        CKGEN_WRITE32( 0x320, (CKGEN_READ32(0x320) & ~(0x3<<2))
                        | (_PCMCIA_DrvCurrentTrans(u4Current)<<2) );
        CKGEN_WRITE32( 0x320, (CKGEN_READ32(0x320) & ~(0x3<<4))
                        | (_PCMCIA_DrvCurrentTrans(u4Current)<<4) );
    }
#endif
    return;
}

// MT5391 Internal CI
static void _PCMCIA_PinMux_11()
{
    UNUSED(_PCMCIA_PinMux_11);
#if defined(PCMCIA_TYPE_INTERNAL) && defined(CC_MT5391)
    LOG(0, "---------------PCMCIA_PinMux: MT5391 Internal CI -------------------\n");
    /* MX1[13:12]
      * CI_CD, CI_REG, CI_RESET, CI_TSO_DATA[7], CI_TSO_DATA[6],
      * CI_TSO_CLK, CI_TSO_DATA[5], CI_TSO_VALID     */
    BSP_PinSet( BO2P, PINMUX_FUNCTION1 );

    /* MX1[15:14]
     * CI_TSO_DATA[4], CI_TSO_DATA[3], CI_WE, CI_TSO_DATA[2],
     * CI_TSO_DATA[1], CI_TSO_DATA[0], CI_TSO_SYNC, CI_IOWR, CI_IORD  */
    BSP_PinSet( BE0P, PINMUX_FUNCTION1 );

    /* MX1[18:16]
      * CI_ADR[0], CI_ADR[1], CI_ADR[2], CI_ADR[3], CI_ADR[4], CI_ADR[5],
     */
    BSP_PinSet( ONDA_00_O, PINMUX_FUNCTION4 );

    /* MX1[22:20]
      * CI_ADR[6], CI_ADR[7],
      */
    BSP_PinSet( ONDA_06_O, PINMUX_FUNCTION4 );

    /* MX1[26:24]
      * CI_ADR[8],
      */
    BSP_PinSet( ONDA_08_O, PINMUX_FUNCTION4 );

    /* MX1[30:28]
      * CI_ADR[9], CI_ADR[10], CI_DATA[0], CI_DATA[1], CI_DATA[4], CI_DATA[5],
      */
    BSP_PinSet( ONDA_09_O, PINMUX_FUNCTION4 );

    /* MX2[2:0]
      * CI_ADR[11], CI_ADR[12], CI_ADR[13],
      */
    BSP_PinSet( ONDA_11_O, PINMUX_FUNCTION4 );

    /* MX2[6:4]
      * CI_ADR[14]
      */
    BSP_PinSet( ONDOE_O, PINMUX_FUNCTION4 );

    /* MX2[26:24]
     * GPIO6=CI_CE
     */
    BSP_PinSet( PINMUX_GPIO_06, PINMUX_FUNCTION1 );

    /* MX2[31:29]
      * GPIO[11]=CI/TS0_DATA[2], GPIO[12]=CI/TS0_DATA[3]
      */
    BSP_PinSet( PINMUX_GPIO_11, PINMUX_FUNCTION1 );

    /* MX4[2:0]
      * GPIO[19]=CI/TS0_DATA[4]
      */
    BSP_PinSet( PINMUX_GPIO_19, PINMUX_FUNCTION1 );

    /* MX4[6:4]
      * GPIO[20]=CI/TS0_DATA[5]
      */
    BSP_PinSet( PINMUX_GPIO_20, PINMUX_FUNCTION1 );

    /* MX4[10:8]
      * GPIO[21]=CI/TS0_DATA[6]
      */
    BSP_PinSet( PINMUX_GPIO_21, PINMUX_FUNCTION1 );

    /* MX4[14:12]
      * GPIO[22]=CI/TS0_DATA[7]
      */
    BSP_PinSet( PINMUX_GPIO_22, PINMUX_FUNCTION1 );

    /* MX3[13:12]
      * GPIO[23]=CI/TS0_DATA[1], GPIO[24]=CI_WAIT , GPIO[25]=CI_IREQ
      */
    BSP_PinSet( PINMUX_GPIO_23, PINMUX_FUNCTION1 );

    //Disable LVDS Channel B , and leave it to BSP
    IO_WRITE32MSK(POST_PROC_BASE, 0x408, 0x7ffc, 0x30007ffc);

    IO_WRITE32( CKGEN_PCMCIA_CKCFG , 0, 0x03 ); //PCMCIA ->108MHz

    //GPIO_SetOut( 114, 0 );
    BSP_PinSet( BE3N, PINMUX_FUNCTION1 );
#endif
    return;
}

// MT5363+MT8295
static void _PCMCIA_PinMux_12()
{
    UNUSED(_PCMCIA_PinMux_12);
#if defined(PCMCIA_TYPE_EXTERNAL) && defined(CC_MT5363)
    LOG(0, "---------------PCMCIA_PinMux: MT5363+MT8295 -------------------\n");
    BSP_PinSet( CI_MDO0, PINMUX_FUNCTION0 );
//    BSP_PinSet( CI_MOVAL, PINMUX_FUNCTION0 );
    BSP_PinSet( CI_MOSTRT, PINMUX_FUNCTION0 );
    BSP_PinSet( CI_MCLKO, PINMUX_FUNCTION0 );
    
    BSP_PinSet( CI_MISTRT, PINMUX_FUNCTION0 );
    BSP_PinSet( CI_MDI0, PINMUX_FUNCTION0 );
    BSP_PinSet( CI_MCLKI, PINMUX_FUNCTION0 );
    BSP_PinSet( CI_MIVAL, PINMUX_FUNCTION0 );

    BSP_PinSet( PINMUX_GPIO_00, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_01, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_02, PINMUX_FUNCTION1 );

    // Demod TS output to 8295
    CKGEN_WRITE32( 0x24c, ((CKGEN_READ32(0x24c)&0xFCFFFFFF)|0x01000000));

#if 0 // External CI Case 2
    BSP_PinSet( PINMUX_GPIO_22, PINMUX_FUNCTION3 );
    BSP_PinSet( PINMUX_GPIO_23, PINMUX_FUNCTION3 );
    BSP_PinSet( PINMUX_GPIO_24, PINMUX_FUNCTION3 );
    BSP_PinSet( PINMUX_GPIO_25, PINMUX_FUNCTION3 );

    BSP_PinSet( PINMUX_GPIO_33, PINMUX_FUNCTION2 );
    BSP_PinSet( PINMUX_GPIO_34, PINMUX_FUNCTION2 );
    BSP_PinSet( PINMUX_GPIO_35, PINMUX_FUNCTION2 );
    BSP_PinSet( PINMUX_GPIO_36, PINMUX_FUNCTION2 );
#endif

#endif
    return;
}

// MT5363 Internal CI
static void _PCMCIA_PinMux_13()
{
    UNUSED(_PCMCIA_PinMux_13);
#if defined(PCMCIA_TYPE_INTERNAL) && defined(CC_MT5363)
    LOG(0, "---------------PCMCIA_PinMux: MT5363 Internal CI -------------------\n");
    /* Clock selection */
    CKGEN_WRITE32( 0x294, (CKGEN_READ32(0x294) & 0xFFFFFFF8) | 0x2 ); // [2:0] = 2, 60Mhz

    BSP_PinSet( CI_MDO0, PINMUX_FUNCTION2 );
//    BSP_PinSet( CI_MOVAL, PINMUX_FUNCTION2 );
    BSP_PinSet( CI_MOSTRT, PINMUX_FUNCTION2 );
    BSP_PinSet( CI_MCLKO, PINMUX_FUNCTION2 );

    BSP_PinSet( CI_MISTRT, PINMUX_FUNCTION1 );
    BSP_PinSet( CI_MDI0, PINMUX_FUNCTION1 );
    BSP_PinSet( CI_MCLKI, PINMUX_FUNCTION1 );
    BSP_PinSet( CI_MIVAL, PINMUX_FUNCTION1 );

    BSP_PinSet( MII_PHY_TXER, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_MDC_O, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_MDIO, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_RXER, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_CRS, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_RXDV, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_TXER, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_RXD0, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_RXD1, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_RXD2, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_RXD3, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_RXCLK, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_TXCLK, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_TXD0, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_TXD1, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_TXD2, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_TXD3, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_TXEN, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_COL, PINMUX_FUNCTION2 );
    BSP_PinSet( MII_PHY_REFCLK, PINMUX_FUNCTION2 );

    BSP_PinSet( PINMUX_GPIO_03, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_04, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_05, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_06, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_07, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_08, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_09, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_10, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_11, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_12, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_13, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_14, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_15, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_16, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_17, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_18, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_19, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_20, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_21, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_22, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_23, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_24, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_25, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_26, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_27, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_28, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_29, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_30, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_31, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_32, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_33, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_34, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_35, PINMUX_FUNCTION1 );
    BSP_PinSet( PINMUX_GPIO_36, PINMUX_FUNCTION1 );


    IO_WRITE32( 0x2000d404 , 0, 0x09054940); 
    IO_WRITE32( 0x2000d408 , 0, 0x00004a00); 
    IO_WRITE32( 0x2000d414 , 0, 0x002aaaaa); 

#endif    
    return;
}

static void _PCMCIA_SetCamDrvingCurrent(void)
{

#if defined(CC_8295_FPGA_EMU)
    {
        LOG(1, "8295 driving no use in FPGA\n");
        return;
    }
#endif
    UINT32 u4Val;
    ASSERT( DRVCUST_OptQuery(eMT8295DrivingCurrent, &u4Val) == 0 );
    if (DRVCUST_OptQuery(eMT8295DrivingCurrent, &u4Val) == 0)
    {
        LOG(0, "MT8295 driving current %d mA\n",u4Val);
        switch( u4Val )
        {
            case 0 /*MT8295_IO_0mA*/:
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x00000000 );

                    if ( eMt8295Version == MT8295_VERSION_BNE )
                    {
                        PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000000 );    
                        PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000000 );    
                    }
                break;
            case 4 /*MT8295_IO_4mA*/:
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x04104040 );
                                    
                    if ( eMt8295Version == MT8295_VERSION_BNE )
                    {
                        PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000001 );    
                        PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00040000 );
                    }
                break;
            case 8 /*MT8295_IO_8mA*/:
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x08208080 );
                                    
                    if ( eMt8295Version == MT8295_VERSION_BNE )
                    {
                        PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000002 );    
                        PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00080000 );
                    }
                break;
            case 12 /*MT8295_IO_12mA*/:
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET2,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET2)&(~0x0c30c0c0)|0x0c30c0c0 );
                                    
                    if ( eMt8295Version == MT8295_VERSION_BNE )
                    {
                        PCMCIAHW_WRITE32(REG_CKGEN_GPR7,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x00000003 );
                        PCMCIAHW_WRITE32(REG_CKGEN_GPR10,PCMCIAHW_READ32(REG_CKGEN_GPR10)|0x000C0000 );
                    }
                break;
            default:
                    LOG(0, "SetDrvingCurrent drv-cust error\n");
                    ASSERT(0);
                break;
        }
    }

    return;
}

static BOOL _PCMCIA_SetPllOn(MT8295_VERSION eMt8295Ver)
{
#if defined(CC_8295_FPGA_EMU)
    {
        LOG(1, "_PCMCIA_SetPllOn no use in FPGA\n");
        return;
    }
#endif

    UINT32 PLL_InitSeq[9] = {0};
    UINT32 PLL_ChkSeq[5] = {0};
     
    PLL_InitSeq[0] = REG_PLL_CNTL_SET0; PLL_InitSeq[1] = 0x1892101f;
    PLL_InitSeq[2] = REG_PLL_CNTL_SET1; PLL_InitSeq[3] = 0x00002800;
    PLL_InitSeq[4] = REG_PLL_CNTL_SET0; PLL_InitSeq[5] = 0x0890606f;
    PLL_InitSeq[6] = REG_PLL_CNTL_SET1; PLL_InitSeq[7] = 0x80002800;
    PLL_InitSeq[8] = 0xFFFF;    //termination

    PLL_ChkSeq[0] = REG_PLL_CNTL_SET0;  PLL_ChkSeq[1] = 0x0890606f;
    PLL_ChkSeq[2] = REG_PLL_CNTL_SET1;  PLL_ChkSeq[3] = 0x80002800;
    PLL_ChkSeq[4] = 0xFFFF;     //termination
    
    UINT8 i;

    if ( eMt8295Ver == MT8295_VERSION_AE )
    {}
    else if ( eMt8295Ver == MT8295_VERSION_BNE )
    {
        PLL_InitSeq[7] = 0x20002800;
        PLL_ChkSeq[3] = 0x20002800;
    }
    else
    {
        ASSERT(0);
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
            LOG(0, "PLL Read : 0x%X\n", PCMCIAHW_READ32( PLL_ChkSeq[i] ));
            LOG(0, "PLL Predict : 0x%X\n", PLL_ChkSeq[i+1]);
            return FALSE;
        }
    }

    LOG(3, "Reg[0x94]=%08x\n",PCMCIAHW_READ32(0x94*4) );
    LOG(3, "Reg[0xA0]=%08x\n",PCMCIAHW_READ32(0xA0*4) );
    LOG(3, "Reg[0xA4]=%08x\n",PCMCIAHW_READ32(0xA4*4) );

    return TRUE;
}

static void _PCMCIA_OnOffCamStream(BOOL fgOn)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        if( fgOn )
        {
            PCMCIAHW_WRITE32( REG_CTRL, PCMCIAHW_READ32(REG_CTRL) & 0xfff3ffff | 0x000c0000);		//TS Enable (MT8295)
            PCMCIAHW_WRITE32( REG_POD_GPR_CNTL_SET0, (PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & ~(0x80000000)) | 0x00000000);		//Non-Bypass (MT8295)
        }
        else
        {
            PCMCIAHW_WRITE32( REG_CTRL, PCMCIAHW_READ32(REG_CTRL) & 0xfff3ffff | 0x00000000);		//TS Disable (MT8295)
            PCMCIAHW_WRITE32( REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & ~(0x80000000) | 0x80000000);		//Bypass (MT8295)
        }
    }
    else
    {
        if ( fgOn )
        {
            // enable TS from demod to CI CAM
            IO_WRITE32(CKGEN_BASE, 0x408, IO_READ32(CKGEN_BASE, 0x408)|0x00004000);
        }
        else
        {
            // disable TS from demod to CI CAM
            IO_WRITE32(CKGEN_BASE, 0x408, IO_READ32(CKGEN_BASE, 0x408)&0xffffbfff); 
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
        _MT8295_ResetMmx();
        _MT8295_ResetFramer();
        _MT8295_ResetMDmx();
        _MT8295_ResetCmp();
        x_thread_delay(10);
    }
}

static void _MT8295_SetPll()
{
    if ( eMt8295Version == MT8295_VERSION_NONE )
    {
        if ( _PCMCIA_SetPllOn(MT8295_VERSION_BNE) )
        {
            eMt8295Version = MT8295_VERSION_BNE;
            LOG(0, "MT8295 Version : BNE\n");
        }
        else
        {
            _MT8295_Reset();
        }
    }
    if ( eMt8295Version == MT8295_VERSION_NONE )
    {
        if ( _PCMCIA_SetPllOn(MT8295_VERSION_AE) )
        {
            eMt8295Version = MT8295_VERSION_AE;
            LOG(0, "MT8295 Version : AE\n");
        }
        else
        {
            _MT8295_Reset();
        }
    }

    if ( eMt8295Version==MT8295_VERSION_NONE )
    {
        LOG(0, "Cna't find proper PLL setting for MT8295\n");
        ASSERT(0);
    }
}

static INT32 _MT8295_CheckAlive(void)        //check hw status
{
    UINT32 u4Reg = (UINT32)0xffffffff;
    INT32 i4Ret  = POD_OK ;
    UINT8 u1Cnt  = 10;

    if ( eMt8295Version == MT8295_VERSION_AE )
    {
        for( ;u1Cnt>0;u1Cnt--)            //read mutiple times, should be the same
        {
            u4Reg = PCMCIAHW_READ32(REG_VERSION_CODE);
            i4Ret = ((u4Reg&0x000000ff)==0x08)? POD_OK : POD_INIT_FAILED ;
            if(i4Ret !=POD_OK)
            {
                break;
            }
        }
    }

    if ( eMt8295Version == MT8295_VERSION_BNE )
    {
        for( ;u1Cnt>0;u1Cnt--)            //read mutiple times, should be the same
        {
            u4Reg = PCMCIAHW_READ32(REG_CKGEN_CHIP_VERSION);
            i4Ret = ((u4Reg&0x0000ffff)==0xdead)? POD_OK : POD_INIT_FAILED ;
            if(i4Ret !=POD_OK)
            {
                break;
            }
        }        
    }

    if(i4Ret !=POD_OK)
    {
        LOG(0, "MT8295 is dead !\n"  );
        LOG(0, "Version Code : 0x%x\n", u4Reg);
        return i4Ret;
    }

    return i4Ret;
}

static void _MT8295_ResetMDmx(void)
{
    PCMCIAHW_WRITE32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER, 
                     PCMCIAHW_READ32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER) | 0x00010000 );
    x_thread_delay(1);
    PCMCIAHW_WRITE32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER, 
                     PCMCIAHW_READ32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER) & 0xFFFEFFFF );
    x_thread_delay(1);
}

static void _MT8295_ResetFramer(void)
{
    PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 
                     PCMCIAHW_READ32(MPOD_MUX_FRAMER_CONTROL_REGISTER) | 0x000F0000 );
    x_thread_delay(1);
    PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 
                     PCMCIAHW_READ32(MPOD_MUX_FRAMER_CONTROL_REGISTER) & 0xFFF0FFFF );
    x_thread_delay(1);
}

static void _MT8295_ResetMmx(void)
{
    PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_0, 0x80000000 );
    x_thread_delay(1);
    PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_0, 0x40000000 );
    x_thread_delay(1);
}

static void _MT8295_ResetCmp(void)
{
    PCMCIAHW_WRITE32(MPOD_CMP_CONFIGURATION_REGISTER, 
                     PCMCIAHW_READ32(MPOD_CMP_CONFIGURATION_REGISTER) | 0x02000000 );
    x_thread_delay(1);
    PCMCIAHW_WRITE32(MPOD_CMP_CONFIGURATION_REGISTER, 
                     PCMCIAHW_READ32(MPOD_CMP_CONFIGURATION_REGISTER) & 0xFDFFFFFF );
    x_thread_delay(1);
}

static void _MT8295_Reset(void)
{
    UINT32 u4GpioNum;
    ASSERT( DRVCUST_OptQuery(eMT8295ResetGpio, &u4GpioNum) == 0 );
    if (DRVCUST_OptQuery(eMT8295ResetGpio, &u4GpioNum) == 0)
    {
       LOG(3, "_MT8295_Reset: u4GpioNum: %d \n", u4GpioNum);
       GPIO_SetOut((INT32)u4GpioNum, 0);       //reset
       x_thread_delay(10);
       GPIO_SetOut((INT32)u4GpioNum, 1);       //~reset
       x_thread_delay(10);
    }

    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_POD )
    {
        _MT8295_ResetMmx();
        _MT8295_ResetFramer();
        _MT8295_ResetMDmx();
        _MT8295_ResetCmp();
        x_thread_delay(10);
    }
}

static void _MT8295_Init(void)
{
    /*----make sure 8295 only inited one time -----*/

    if ( fgMt8295Inited )
    {
        return;
    }
    
    fgMt8295Inited = TRUE;

    #ifdef CC_MT5363
        PCMCIAHW_SetIfType(PCMCIA_IF_TYPE_SPI);
        SPI_Init();
    #else
        PCMCIAHW_SetIfType(PCMCIA_IF_TYPE_NFI);
        NFI_Init();
    #endif

    _MT8295_Reset();

    /*-------------8295 should be readable so far---------------*/
    _MT8295_SetPll();

    if ( _MT8295_CheckAlive() != 0 )
    {
        LOG(0, "ERROR : MT8295 is not mounted !!!\n"); 
        LOG(0, "ERROR : Please turn off CC_INTERNAL_CI or CC_EXTERNAL_CI.\n");
        ASSERT(0);
    }

    _PCMCIA_SetCamDrvingCurrent();

    PCMCIAHW_WRITE32(  REG_POD_GPR_CNTL_SET0, 0x34 );		//clear bypass mode -->which means no bypass TS stream
    LOG(0, "5381/5382: serial TS + no-bypass \n");
    LOG(0, "8295 initilization done\n");
}

static void _MT8295_IntrqIsr(INT32 i4Gpio, BOOL fgStatus)
{
    UINT32 u4DrvCustIntPin;

    VERIFY(DRVCUST_OptQuery(eMT8295IntGpio, &u4DrvCustIntPin) == 0);
    VERIFY( (UINT32)i4Gpio == u4DrvCustIntPin );
    LOG(7, "(_MT8295_IntrqIsr) i4Gpio = %d, fgStatus = %d\n", i4Gpio, fgStatus);

    //double0-edged, for there are chances isr slower than waveform, check both edge-triggered
    if ( 0 == fgStatus )
    {
        // Handle 8295 GPIOs, CmdDone Interrupt
        VERIFY( x_sema_unlock(sema_Mt8295Intrq)==OSR_OK);
    }
}


static void _MT8295_IntrThreadHandleSif()
{
    UINT32 u4RegIntSts;
    
    u4RegIntSts = PCMCIAHW_READ32(REG_WRP_INTST);
    if ( (u4RegIntSts & 0x00000003) != 0 )
    {
        if ( vMt8295IsrSifFct != NULL )
        {
            vMt8295IsrSifFct(0);
        }
    }
}

static void _MT8295_IntrThreadHandleIreq()
{
    UINT32 u4RegIntSts;
    
    u4RegIntSts = PCMCIAHW_READ32(REG_INT_STS);
    if ( (u4RegIntSts & PC_RDY_IRQ_INT) != 0 )
    {
        if ( vMt8295IsrIreqFct != NULL )
        {
            vMt8295IsrIreqFct(PCMCIAHW_READ32(REG_PBUS_STS) & PC_RDY_IRQ);
        }
        PCMCIAHW_WRITE32(REG_INT_STS, (u4RegIntSts & PC_RDY_IRQ_INT));
    }
}

static void _MT8295_IntrThreadMain(void * pvArg)
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
            VERIFY( x_sema_lock( sema_Mt8295Intrq, X_SEMA_OPTION_WAIT )==OSR_OK);
        }
        else
        {
            x_thread_delay(100);
        }

        // Handle SIF Interrupt
        _MT8295_IntrThreadHandleSif();

        // Handle IREQ Interrupt
        _MT8295_IntrThreadHandleIreq();        

        u1Count = 0;
        u4RegIntSts = PCMCIAHW_READ32(REG_INT_STS);  //Gpio Input Register (voltage high low)
        while (u4RegIntSts && (u1Count++<SEMA_TIMEOUT) )
        {
            u4RegIntEnb = PCMCIAHW_READ32(REG_GPIO_INT_ENABLE);     //Inerrupt  Enable Register
            //u4RegIntSts = PCMCIAHW_READ32(REG_INT_STS);           //Interrupt Status Register
            u4RegInput = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET4);    //Gpio Input Register (voltage high low)
            PCMCIAHW_WRITE32(REG_INT_STS, u4RegIntSts);             //Clear interrupt bits
            u4RegCtrl = PCMCIAHW_READ32(REG_CTRL);
            u4RegCmdSts = PCMCIAHW_READ32(REG_CMD_STS);

            LOG(9, "REG_GPIO_INT_ENABLE0x%02X=0x%08X, REG_INT_STS0x%02X=0x%08X , REG_POD_GPR_CNTL_SET40x%02X=0x%08X\n",
                REG_GPIO_INT_ENABLE , REG_INT_STS,REG_POD_GPR_CNTL_SET4,u4RegIntEnb, u4RegIntSts ,u4RegInput);

            /*-----Handle 8295 Gpio Bits-----*/
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

        if ( fgMT8295IntrqThreadKeepRunning == FALSE )
        {
            break;
        }

        if ( fgThreadNoAssert == FALSE )
        {
            if ( u1Count > SEMA_TIMEOUT )
            {
                LOG(0, "_MT8295_IntrThreadMain Timeout");
            }
            ASSERT( u1Count <= SEMA_TIMEOUT );          //the events should not be coming so quickly
        }
    }

    fgMT8295IntrqThreadTerminated = TRUE;
}

static void _MT8295_StartIntrqPollingThread()
{
    if ( fgMT8295ThreadStarted == TRUE )
    {
        return;
    }

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return;
    }

    if ( sema_Mt8295Intrq == NULL )
    {
        VERIFY( x_sema_create(&sema_Mt8295Intrq, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK)==OSR_OK);
    }
    
    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )
    {
        UINT32 u4DrvCustIntPin;
        VERIFY( DRVCUST_OptQuery(eMT8295IntGpio, &u4DrvCustIntPin) == 0);
        VERIFY( GPIO_Reg((INT32)u4DrvCustIntPin, GPIO_TYPE_INTR_BOTH, _MT8295_IntrqIsr) ==0);
    }

    VERIFY( x_thread_create(&hMT8295IntrqThread,
                            "MT8295_IntrqThread",
                            1024,   //stack size
                            128,    //priority
                            _MT8295_IntrThreadMain,
                            0,
                           NULL) == OSR_OK);

    fgMT8295IntrqThreadKeepRunning = TRUE;
    fgMT8295IntrqThreadTerminated = FALSE;
    fgMT8295ThreadStarted = TRUE;
    return;
}

static void _MT8295_ConfigCtrlRegister(void)
{
    UINT32 u4Reg;
    UINT32  u4DrvCustIntPin;

    u4Reg = (UINT32)GCMD_IE  | (UINT32)PC_MEM_WIN_0_EN | (UINT32)PC_MEM_WIN_1_EN | (UINT32)PC_IO_WIN_EN |
            (UINT32)PC_WP_EN | (UINT32)SIXTEEN_QW      |
            (UINT32)MEM_ONLY | (UINT32)EIGHT_BIT_MEM;

    if ( (DRVCUST_OptQuery(eMT8295IntGpio, &u4DrvCustIntPin) == 0) || (ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL) )
    {
        u4Reg |= (UINT32)PC_RDY_IRQ_IE;
    }

    PCMCIAHW_WRITE32(REG_CTRL, u4Reg);
}

static void _MT8295_ConfigWindows(void)
{
    /* Configure memory window 0 for attribute memory access */
    PCMCIAHW_WRITE32(REG_PC_MEM_WIN0_1, 0 & (UINT32)ATTRIBUTE_MEM);
    PCMCIAHW_WRITE32(REG_PC_MEM_WIN0_2, ATTRIBUTE_MEM_END_ADDR);

    /* Configure memory window 1 for common memory access */
    PCMCIAHW_WRITE32(REG_PC_MEM_WIN1_1, 0 | (UINT32)COMMON_MEM);
    PCMCIAHW_WRITE32(REG_PC_MEM_WIN1_2, COMMON_MEM_END_ADDR);
}

static void _MT8295_ConfigTiming(void)
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

INT32 PCMCIA_ConfigRegDefault(void)
{
    /* Configure PCMCIA_POD_CI control interface */
    /* Initially, the HW should be configured to support 16-bit memory only PC card */
    _MT8295_ConfigCtrlRegister();
    _MT8295_ConfigWindows();
    _MT8295_ConfigTiming();

    return POD_OK;
}

INT32 PCMCIA_MT8295_GPIO_Init()
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return 0;
    }

    if( fgMt8295GpioInit == TRUE)
    {
        return 0;
    }
    
    fgMt8295GpioInit = TRUE;

    VERIFY(GPIO_Init()==0);

    return 0;
}

void PCMCIA_PinMux()
{   
    #if defined(CC_8295_FPGA_EMU)
    {
        LOG(1, "pinmux no use in FPGA\n");
        return;
    }
    #endif

    LOG(0, "PINMUX0:0x%08X\n", PINMUX_REG(0) );
    LOG(0, "PINMUX1:0x%08X\n", PINMUX_REG(1) );
    LOG(0, "PINMUX2:0x%08X\n", PINMUX_REG(2) );
    LOG(0, "PINMUX3:0x%08X\n", PINMUX_REG(3) );
    LOG(0, "PINMUX4:0x%08X\n", PINMUX_REG(4) );

    #ifdef CC_MT5360
        _PCMCIA_PinMux_8();
    #elif defined(CC_MT5392B) && defined(CC_EXTERNAL_CI)
        _PCMCIA_PinMux_10();
    #elif defined(CC_MT5391) && defined(CC_INTERNAL_CI)
        _PCMCIA_PinMux_11();
    #elif defined(CC_MT5363) && defined(CC_EXTERNAL_CI)
        _PCMCIA_PinMux_12();
    #elif defined(CC_MT5363) && defined(CC_INTERNAL_CI)
        _PCMCIA_PinMux_13();
    #endif

    LOG(0, "PINMUX0:0x%08X\n", PINMUX_REG(0) );
    LOG(0, "PINMUX1:0x%08X\n", PINMUX_REG(1) );
    LOG(0, "PINMUX2:0x%08X\n", PINMUX_REG(2) );
    LOG(0, "PINMUX3:0x%08X\n", PINMUX_REG(3) );
    LOG(0, "PINMUX4:0x%08X\n", PINMUX_REG(4) );
}

INT32 PCMCIA_Initialize(void)
{
    INT32   i4Return;
    UINT32  u4DrvCustIntPin;

    if( fgPcmciaInit == TRUE )
    {
        return POD_OK;
    }
    
#if defined(CC_8295_FPGA_EMU)
    // currently was called in ci_mw.c pod_mw.c
    PODCI_AllocDmaBuf();
#endif

    // Init PODCI
    PODCI_Init();

    // Create Semaphores
    if ( hCmdDoneSema == NULL )
    {
        VERIFY(x_sema_create(&hCmdDoneSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }

    // Initialize ISR function pointers
    vMt8295IsrSifFct = NULL;

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
        
        PCMCIA_MT8295_GPIO_Init();

        PCMCIA_PinMux();

        _MT8295_Init();

        _MT8295_StartIntrqPollingThread();
    }
    else
    {
        tPcmciaDriverConfig.u1CmdDoneMethod = PCMCIA_CMD_DONE_INTERRUPT;
        
        PCMCIA_PinMux();

        _PCMCIA_Reset();

        // disable TS from demod to CI CAM
        IO_WRITE32(CKGEN_BASE, 0x408, IO_READ32(CKGEN_BASE, 0x408)&0xffffbfff); 
    }

    // Configure hardware controller
    i4Return = PCMCIA_ConfigRegDefault();
    if (i4Return != POD_OK)
    {
        return i4Return;
    }

    fgPcmciaInit=(BOOL)TRUE;

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
        GPIO_SetOut((INT32)u4GpioNum, (u4PwrOnPolarity?1:0));	//enable CI_VCC_CNTL
        u4CamPwrStatus = (u4PwrOnPolarity?1:0);
    }
}

void PCMCIA_ThdNoAsrt(BOOL bNoAssert)
{
    fgThreadNoAssert = bNoAssert;
}

void PCMCIA_StopThd(void)
{
    fgMT8295IntrqThreadKeepRunning = FALSE;
    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )
    {
        VERIFY( x_sema_unlock(sema_Mt8295Intrq)==OSR_OK);
    }
}

void PCMCIA_ResetPcmciaHW(void)
{
    fgPcmciaInit = FALSE;
    fgMt8295GpioInit = FALSE;
    fgMt8295Inited = FALSE;
    fgMT8295ThreadStarted = FALSE;

    // Terminate MT8295IntrqThread.
    fgMT8295IntrqThreadKeepRunning = FALSE;
    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )
    {
        VERIFY( x_sema_unlock(sema_Mt8295Intrq)==OSR_OK);
    }

    // Wait until thread terminated.
    while ( TRUE )
    {
        if ( fgMT8295IntrqThreadTerminated == TRUE )
        {
            break;
        }
    }
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        _MT8295_Reset();
    }
    else
    {
        _PCMCIA_Reset();
    }
}


void PCMCIA_GetCmdStatusDone(void)
{
    UINT32    u4CmdSts , u4PollCount;
    INT32             i4ret;

    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_INTERRUPT )   // Interrupt Mode
    {
        i4ret = x_sema_lock_timeout(hCmdDoneSema, SEMA_TIMEOUT);
        if( i4ret != OSR_OK )
        {
            LOG(0, "PCMCIA_GetCmdStatusDone Fail");
        }
        ASSERT( i4ret == OSR_OK );
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

            x_thread_delay( 1 );		//delay 1 ms

            /* exception: escape if consume too much time */
            if( ++u4PollCount > SEMA_TIMEOUT )
            {
        	    LOG(0, "PCMCIA_GetCmdStatusDone Timeout");
        	    ASSERT( u4PollCount <SEMA_TIMEOUT );
        	    break;
            }
        }
        LOG( 9, "POD_GetCmdDone(), Polling Mode, u4PollCount=%d \n", u4PollCount );
    }
}

void PCMCIA_SetCmdStatusDone(void)
{
    VERIFY(x_sema_unlock(hCmdDoneSema) == OSR_OK);
}

INT32 PCMCIA_PcmciaToPodCi(BOOL fgPcmciaToPod)
{

    if (fgPcmciaToPod) /* POD mode */
    {
        /* Disable BVD1 and BVD2 interrupts since the pins are used as TS data pins after personality change */
        if ( ( PCMCIAHW_READ32(REG_CTRL)&0xC0000000) != (UINT32)POD )
        {
            PCMCIAHW_WRITE32(REG_CTRL, (PCMCIAHW_READ32(REG_CTRL) & 0x3FFFFFFF) | (UINT32)POD);
        }
    }
    else /* PCMCIA mode */
    {
        if ( (PCMCIAHW_READ32(REG_CTRL) &0xC0000000) != (UINT32)EIGHT_BIT_MEM )
        {
            PCMCIAHW_WRITE32(REG_CTRL, (PCMCIAHW_READ32(REG_CTRL) & 0x3FFFFFFF) | (UINT32)EIGHT_BIT_MEM);
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
    UINT32 u4GpioNum, u4PwrOnPolarity, u4RegValue;
    VERIFY( DRVCUST_OptQuery(eCI5VPowerOnGpio, &u4GpioNum) == 0 );
    VERIFY( DRVCUST_OptQuery(eCI5VPowerOnPolarity, &u4PwrOnPolarity) == 0 );

    if ( fgPcmciaInit == FALSE )
    {
        return 0;
    }

    if ( !(PCMCIAHW_QueryPbusSts((UINT32)PC_CD1) ? 1 : 0) &&
        !(PCMCIAHW_QueryPbusSts((UINT32)PC_CD2) ? 1 : 0))
    {
        if ( fgCamPwrManualControl == FALSE )
        {
            if ( u4CamPwrStatus != (u4PwrOnPolarity?1:0) )
            {
                u4RegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3);
                if ( eMt8295Version == MT8295_VERSION_AE )
                {
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, u4RegValue & 0x7BFF);
                }
                else if ( eMt8295Version == MT8295_VERSION_BNE )
                {
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, u4RegValue & 0xFBFF);
                }
                GPIO_SetOut((INT32)u4GpioNum, (u4PwrOnPolarity?1:0));	//enable CI_VCC_CNTL
                u4CamPwrStatus = (u4PwrOnPolarity?1:0);
            }
        }
        
        LOG(9, "PCMCIA_DetectCard : CI 5V Power On\n");
        return 1;
    }
    else
    {
        if ( fgCamPwrManualControl == FALSE )
        {
            if ( u4CamPwrStatus != (u4PwrOnPolarity?0:1) )
            {
                GPIO_SetOut((INT32)u4GpioNum, (u4PwrOnPolarity?0:1));    //disable CI_VCC_CNTL
                u4RegValue = PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3);
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, u4RegValue | 0x400);
                u4CamPwrStatus = (u4PwrOnPolarity?0:1);
            }
        }
        
        LOG(9,"PCMCIA_DetectCard : CI 5V Power Off\n");
    }
    return 0;
}

BOOL PCMCIA_DetectPodCiActive(void)
{
    if ( PCMCIAHW_READ32(REG_CTRL) & POD )
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
            if ( eMt8295Version == MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)|0x4000 );
            }
            else if ( eMt8295Version == MT8295_VERSION_BNE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)&(~0xC000));
            }
        }
        else
        {
            if ( eMt8295Version == MT8295_VERSION_AE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)&(~0x4000));
            }
            else if ( eMt8295Version == MT8295_VERSION_BNE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3,PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)|0xC000 );
            }
        }
    }
    else
    {
        // TODO 
    }
}

void PCMCIA_SetParallelTsOnOff(BOOL fgOnOff)
{

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        if (fgOnOff)
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)|0x40000000 );
            
            if ( eMt8295Version == MT8295_VERSION_BNE )
            {
                PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3)&0xFFFF3FFF );
            }
        }
        else
        {
            PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&(~0x40000000));
            
            if ( eMt8295Version == MT8295_VERSION_BNE )
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

void PCMCIA_Mt8295PinSet(UINT32 i4PinSel, UINT32 i4Func)
{
    UINT32 u4RegAddr;
    UINT32 u4RegValue;
    UINT32 u4BitIndex;

    if ( i4Func > PCMCIA_PINMUX_FUNCTION3 )
    {
        LOG(0, "PCMCIA_Mt8295PinSet invalid function number !\n");
        return;
    }

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        LOG(0, "PCMCIA_Mt8295PinSet be invoked with Internal CI function !\n");
        return;
    }
    
    if ( i4PinSel < 32 )
    {
        u4RegAddr = REG_CKGEN_PAD_CFG0;
        u4BitIndex = i4PinSel;
    }
    else
    {
        u4RegAddr = REG_CKGEN_PAD_CFG1;
        u4BitIndex = i4PinSel - 32;
    }

    u4RegValue = (i4Func & 0x3)<<u4BitIndex;
    u4RegValue = (PCMCIAHW_READ32(u4RegAddr)&(~(0x3<<u4BitIndex)))|u4RegValue;
    PCMCIAHW_WRITE32(u4RegAddr, u4RegValue);
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
    
    if ( MT8295_ISR_SIF == u2IntNo )
    {
        if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
        {
            LOG(0, "Sif ISR on MT8925 should not be enabled when using Internal CI.\n");
            ASSERT(0);
        }
        else
        {
            vMt8295IsrSifFct = pfnIsr;
        }
    }
    else if ( MT8295_ISR_IREQ == u2IntNo )  // This is used for CI+.
    {
        if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
        {
            PCMCIA_RegIreqCallback(pfnIsr);
        }        
        else
        {
            vMt8295IsrIreqFct = pfnIsr;
        }
    }
}

