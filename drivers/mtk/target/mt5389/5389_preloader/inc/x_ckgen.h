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

#ifndef X_CKGEN_H
#define X_CKGEN_H

#include "x_hal_5381.h"
#include "hw_ckgen.h"


#define CKGENHW_DEBUG   0

//===========================================================================
// Macros for register read/write

#define CKGEN_READ8(offset)             IO_READ8(CKGEN_BASE, (offset))
#define CKGEN_READ16(offset)            IO_READ16(CKGEN_BASE, (offset))
// #define CKGEN_READ32(offset)         IO_READ32(CKGEN_BASE, (offset))

#define CKGEN_WRITE8(offset, value)     IO_WRITE8(CKGEN_BASE, (offset), (value))
#define CKGEN_WRITE16(offset, value)    IO_WRITE16(CKGEN_BASE, (offset), (value))
// #define CKGEN_WRITE32(offset, value) IO_WRITE32(CKGEN_BASE, (offset), (value))

#define CKGEN_REG8(offset)              IO_REG8(CKGEN_BASE, (offset))
#define CKGEN_REG16(offset)             IO_REG16(CKGEN_BASE, (offset))
#define CKGEN_REG32(offset)             IO_REG32(CKGEN_BASE, (offset))

#if CKGENHW_DEBUG

// implement at ckgen.c
EXTERN void CKGENDB_IoWr(UINT32 u4Offset, UINT32 u4Value, INT8 *szFile, INT32 Line);
EXTERN UINT32 CKGENDB_IoRd(UINT32 u4Offset, INT8 *szFile, INT32 Line);

#define CKGEN_WRITE32(offset, value)      CKGENDB_IoWr(offset, value, __FILE__, __LINE__)
#define CKGEN_READ32(offset)              CKGENDB_IoRd(offset, __FILE__, __LINE__)

#else /* CKGENHW_DEBUG */

#define CKGEN_WRITE32(offset, value)      IO_WRITE32(CKGEN_BASE, (offset), (value))
#define CKGEN_READ32(offset)              IO_READ32(CKGEN_BASE, (offset))

#endif /* CKGENHW_DEBUG */


//----------------------------------------------------------------------------
// PLL Register definitions
//----------------------------------------------------------------------------
//add by david.hsieh 20090504

#define REG_PINMUX_SEL0         (0x0400)    // Pin function multiplexer selection 0
#define REG_PINMUX_SEL1         (0x0404)    // Pin function multiplexer selection 1
#define REG_PINMUX_SEL2         (0x0408)    // Pin function multiplexer selection 2
#define REG_PINMUX_SEL3         (0x040c)    // Pin function multiplexer selection 3
#define REG_PINMUX_MISC         (0x0410)    // Pin function multiplexer misc
#if 0

#define REG_GPIO_OUT0           (0x0500)    // GPIOOUT0
#define REG_GPIO_OUT1           (0x0504)    // GPIOOUT1
#define REG_GPIO_OUT2           (0x0508)    // GPIOOUT2
#define REG_GPIO_OUT3           (0x050c)    // GPIOOUT3
#define REG_GPIO_OUT4           (0x0530)    // GPIOOUT4

#define REG_GPIO_EN0            (0x0510)    // GPIOEN0
#define REG_GPIO_EN1            (0x0514)    // GPIOEN1
#define REG_GPIO_EN2            (0x0518)    // GPIOEN2
#define REG_GPIO_EN3            (0x051c)    // GPIOEN3
#define REG_GPIO_EN4            (0x0534)    //GPIOEN4

#define REG_GPIO_IN0            (0x0520)    // GPIOIN0
#define REG_GPIO_IN1            (0x0524)    // GPIOIN1
#define REG_GPIO_IN2            (0x0528)    // GPIOIN2
#define REG_GPIO_IN3            (0x052c)    // GPIOIN3
#define REG_GPIO_IN4            (0x0538)    // GPIOIN3


#define REG_PMUX0 0x0400
#define REG_PMUX1 0x0404
#define REG_PMUX2 0x0408
#define REG_PMUX3 0x040c
#define REG_PMUX4 0x0410
#define REG_PMUX5 0x0414
#define REG_PMUX_MISC 0x041c

#define REG_GPIOOUT0 0x0500
#define REG_GPIOOUT1 0x0504
#define REG_GPIOOUT2 0x0508
#define REG_GPIOOUT3 0x050c
#define REG_GPIOEN0 0x0510
#define REG_GPIOEN1 0x0514
#define REG_GPIOEN2 0x0518
#define REG_GPIOEN3 0x051c
#define REG_GPIOIN0 0x0520
#define REG_GPIOIN1 0x0524
#define REG_GPIOIN2 0x0528
#define REG_GPIOIN3 0x052c
#define REG_GPIOOUT4 0x0530
#define REG_GPIOEN4 0x0534
#define REG_GPIOIN4 0x0538
#endif

/*
#define REG_GPIO_ED2INTEN       (0x0560)    // TWO EDGE TRIGGERED EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_LEVINTEN       (0x056c)    // LEVEL TRIGGERED EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_ENTPOL         (0x0578)    // EXTERNAL INTERRUPT POLARITY
#define REG_GPIO_EXTINTEN       (0x0584)    // EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_EXTINT         (0x0590)    // EXTERNAL INTERRUPT IDENTIFY BIT
*/
/*
#define REG_RINTEN0 0x0560
#define REG_RINTEN1 0x0564
#define REG_FINTEN0 0x0570
#define REG_FINTEN1 0x0574
#define REG_HINTEN0 0x0580
#define REG_HINTEN1 0x0584
#define REG_LINTEN0 0x0590
#define REG_LINTEN1 0x0594
#define REG_EDGEINT0 0x05a0
#define REG_EDGEINT1 0x05a4
#define REG_MBIST_CTRL 0x05e0
*/
//alias...
/*
#define REG_GPIO_LEV_HIGH_INTR0          (0x0580)    // Level triggered GPIO interrupt enable 0
#define REG_GPIO_LEV_LOW_INTR0          (0x0590)    // Level triggered GPIO interrupt enable 0
#define REG_GPIO_RISING_INTR0   (0x0560)    // Rising edge triggered interrupt Enable 0
#define REG_GPIO_FALLING_INTR0  (0x0570)    // Falling edge triggered interrupt Enable 0
*/
//add by david.hsieh 20090504
//----------------------------------------------------------------------------
// XTAL config
EXTERN BOOL BSP_IsFPGA(void);

//#define XTAL_STRAP_MODE     (CKGEN_READ32(0) & 0x1c00)
#define XTAL_STRAP_MODE (IO32ReadFldAlign(CKGEN_STRAP_OP, FLD_STRAP_2_0) & 0x2)
    #define SET_XTAL_27MHZ  (0)
    #define SET_XTAL_54MHZ  (2)
    #define IS_XTAL_54MHZ() (0)
    #define IS_XTAL_60MHZ() 0
    #define IS_XTAL_27MHZ() 1
    #define IS_XTAL_30MHZ() 0
    #define GET_XTAL_CLK()  (BSP_IsFPGA() ? (CLK_27MHZ) :    \
                            (IS_XTAL_54MHZ() ? (CLK_54MHZ) : (CLK_27MHZ) ))

//#define REG_CPU_CKCFG       (0x0208)

/* marked by david 20090504
#define REG_PINMUX_SEL0         (0x0400)    // Pin function multiplexer selection 0
#define REG_PINMUX_SEL1         (0x0404)    // Pin function multiplexer selection 1
#define REG_PINMUX_SEL2         (0x0408)    // Pin function multiplexer selection 2
#define REG_PINMUX_SEL3         (0x040c)    // Pin function multiplexer selection 3
#define REG_PINMUX_MISC         (0x0410)    // Pin function multiplexer misc
#define REG_PINMUX_SEL4         (0x0414)    // Pin function multiplexer selection 4

#define REG_GPIO_OUT0           (0x0500)    // GPIOOUT0
#define REG_GPIO_OUT1           (0x0504)    // GPIOOUT1
#define REG_GPIO_OUT2           (0x0508)    // GPIOOUT2
#define REG_GPIO_OUT3           (0x050c)    // GPIOOUT3

#define REG_GPIO_EN0            (0x0510)    // GPIOEN0
#define REG_GPIO_EN1            (0x0514)    // GPIOEN1
#define REG_GPIO_EN2            (0x0518)    // GPIOEN2
#define REG_GPIO_EN3            (0x051c)    // GPIOEN3

#define REG_GPIO_IN0            (0x0520)    // GPIOIN0
#define REG_GPIO_IN1            (0x0524)    // GPIOIN1
#define REG_GPIO_IN2            (0x0528)    // GPIOIN2
#define REG_GPIO_IN3            (0x052c)    // GPIOIN3

//MT5387 alais
#define REG_GPIO_ED2INTEN       (0x0580)    // TWO EDGE TRIGGERED EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_LEVINTEN       (0x0590)    // LEVEL TRIGGERED EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_ENTPOL         (0x05a0)    // EXTERNAL INTERRUPT POLARITY
#define REG_GPIO_EXTINTEN       (0x05b0)    // EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_EXTINT         (0x05c0)    // EXTERNAL INTERRUPT IDENTIFY BIT
marked by david 20090504 */

#if 0
#define PAD_NO_PULL             0
#define PAD_PULL_DOWN           1
#define PAD_PULL_UP             2
#define PAD_BUS_HOLD            3
#define PADDRV_2MA              0
#define PADDRV_4MA              1
#define PADDRV_6MA              2
#define PADDRV_8MA              3


#define REG_PADDRV_CFG0         0x0320      // PAD Drive Configuration 0, Set PAD Driving capability
    #define ASPDF_SHFT          (30)
    #define AOSD_SHFT           (28)
    #define AOLRCK_SHFT         (26)
    #define AOBCK_SHFT          (24)
    #define AOMCLK_SHFT         (22)
    #define PADDRV_ASPDF        (3U << ASPDF_SHFT)
    #define PADDRV_AOSD         (3U << AOSD_SHFT)
    #define PADDRV_AOLRCK       (3U << AOLRCK_SHFT)
    #define PADDRV_AOBCK        (3U << AOBCK_SHFT)
    #define PADDRV_AOMCLK       (3U << AOMCLK_SHFT)
    #define PADDRV_GPIO         (3U << 12)  // GPIO group
#endif

//=====================================================================
// Constant definitions Given by ACD analog designer
#define PLL_BAND_NUM           8


//----------------------------------------------------------------------------
// clock const definitions
#define CLK_13_5MHZ             13500000    // 13.5 MHz
#define CLK_20MHZ               20000000    // 20 MHz
#define CLK_27MHZ               27000000    // 27 MHz
#define CLK_30MHZ               30000000    // 30 MHz
#define CLK_40MHZ               40000000    // 40 MHz
#define CLK_50MHZ               50000000    // 50 MHz
#define CLK_54MHZ               54000000    // 54 MHz
#define CLK_60MHZ               60000000    // 60 MHz
#define CLK_81MHZ               81000000    // 81 MHz
#define CLK_100MHZ              100000000   // 100 MHz
#define CLK_120MHZ              120000000   // 120 MHz
#define CLK_231_43MHZ           231430000   // 231.43 MHz
#define CLK_243MHZ              243000000   // 243 MHz
#define CLK_246_86MHZ           246860000   // 246.86 MHz
#define CLK_259_2MHZ            259200000   // 259.2 MHz
#define CLK_324MHZ              324000000   // 324 MHz
#define CLK_337_5MHZ            337500000   // 337.5 MHz
#define CLK_351MHZ              351000000   // 351 MHz
#define CLK_405MHZ              405000000   // 405 MHz
#define CLK_432MHZ              432000000   // 432 MHz
#define CLK_APLL294MHZ          294912000   // 294.912 MHz
#define CLK_APLL270MHZ          270950400   // 270.9404 MHz
#define CLK_200MHZ          200000000
#define CLK_250MHZ          250000000
#define CLK_300MHZ          300000000
#define CLK_400MHZ          400000000
#define CLK_500MHZ          500000000
#define CLK_550MHZ          550000000
#define CLK_600MHZ          600000000



//===========================================================================
// PLL default clock settings

#define APLL1_DEFAULT_CLOCK     CLK_APLL294MHZ  // 270.9504M
#define VPLL_DEFAULT_CLOCK      CLK_81MHZ   // 81 MHz
#define APLL2_DEFAULT_CLOCK     CLK_APLL270MHZ  // 294.912M

//=====================================================================
// Type definitions

typedef enum
{
    CAL_SRC_SAWLESSPLL                = 0  ,
    CAL_SRC_CPUPLL                    = 1  ,
    CAL_SRC_SIF_CK                    = 2  ,
    CAL_SRC_AD_APLL1_CKOUT300M_CK     = 3  ,
    CAL_SRC_AD_APLL1_CKOUT108M_CK     = 4  ,
    CAL_SRC_AD_APLL2_CKOUT300M_CK     = 5  ,
    CAL_SRC_AD_APLL2_CKOUT108M_CK     = 6  ,
    CAL_SRC_AD_PCLK                   = 7  ,
    CAL_SRC_M_CK                      = 8  ,
    CAL_SRC_TCLK                      = 9  ,
    CAL_SRC_AD_APLL2_FBOUT27M_CK      = 10 ,
    CAL_SRC_AD_USBPLL_480M            = 11 ,
    CAL_SRC_AD_USBPLL_480M_2P         = 12 ,
    CAL_SRC_AD_ETHPLL_CLK675M         = 13 ,
    CAL_SRC_XTAL                      = 14 ,
    CAL_SRC_AD_ADC_CKOUT              = 15 ,
    CAL_SRC_AD_CVBSADC_CKIND          = 16 ,
    CAL_SRC_AD_HDMI_1_PLL340M_CK      = 17 ,
    CAL_SRC_AD_HDMI_1_PIX340M_CK      = 18 ,
    CAL_SRC_AD_HDMI_1_DEEP170M_CK     = 19 ,
    CAL_SRC_AD_DDDS2_VSP              = 20 ,
    CAL_SRC_AD_DDDS1_VSP              = 21 ,
    CAL_SRC_TCLK_DIV                  = 22 ,
    CAL_SRC_OCLK                      = 23 ,
    CAL_SRC_OCLK_DIV                  = 24 ,
    CAL_SRC_LVDS_CLK_CTS              = 25 ,
    CAL_SRC_CPUPLL_D2_CLK             = 26 ,
    CAL_SRC_AD_SOGY_ADC_CKOUT         = 30 ,
    CAL_SRC_AD_CKT_B                  = 31 ,
    CAL_SRC_AD_FB_OUT                 = 32 ,
    CAL_SRC_AD_SOGY_OUT_MON           = 33 ,
    CAL_SRC_AD_SOGY_OUT               = 34 ,
    CAL_SRC_AD_VSYNC_OUT              = 35 ,
    CAL_SRC_AD_HSYNC_OUT              = 36 ,
    CAL_SRC_AD_GPIO_PWM_0_0           = 37 ,
    CAL_SRC_AD_GPIO_PWM_0_1           = 38 ,
    CAL_SRC_AD_GPIO_PWM_0_2           = 39 ,
    CAL_SRC_AD_GPIO_PWM_0_3           = 40 ,
    CAL_SRC_AD_GPIO_PWM_0_4           = 41 ,
    CAL_SRC_AD_GPIO_PWM_0_5           = 42 ,
    CAL_SRC_AD_GPIO_PWM_0_6           = 43 ,
    CAL_SRC_AD_GPIO_PWM_0_7           = 44 ,
    CAL_SRC_AD_VOPLL_TEST_CLK         = 45 ,
    CAL_SRC_AD_EVEN_CLK_DIV_125M_D11  = 46 ,
    CAL_SRC_AD_ODD_CLK_DIV_125M_D11   = 47 ,
    CAL_SRC_AD_CLKDIG_D11             = 48 ,
    CAL_SRC_AD_CKOUT_DEMOD            = 49 ,
    CAL_SRC_AD_SRV_SPL                = 50 ,
    CAL_SRC_AD_CKT_A                  = 51 ,
    CAL_SRC_AD_HDMI_2_PLL340M_CK      = 52 ,
    CAL_SRC_AD_HDMI_2_DEEP170M_CK     = 53 ,
    CAL_SRC_AD_HDMI_2_PIX340M_CK      = 54 ,
    CAL_SRC_AD_HDMI_3_PLL340M_CK      = 55 ,
    CAL_SRC_AD_HDMI_4_DEEP170M_CK     = 56 ,
    CAL_SRC_AD_HDMI_5_PIX340M_CK      = 57 ,
    CAL_SRC_AD_CLKDIG_CTS_D11         = 58 ,
    CAL_SRC_AD_CVBSADC_CKOUT          = 60 ,
    CAL_SRC_AD_VOPLL_DPIX_CK          = 64 ,
    CAL_SRC_AD_VOPLL_TCON_CK          = 65 ,
    CAL_SRC_AD_PPLL_DPIX_CK           = 66 ,
    CAL_SRC_AD_XDDS_CLK               = 67 ,
    CAL_SRC_AD_MJCPLL_REFCLK_BUF      = 68 ,
    CAL_SRC_AD_MJCPLL_FBCLK_BUF       = 69 ,
    CAL_SRC_AD_PLLGP_TSTCK            = 70 ,
    CAL_SRC_AD_HADDS2_FBOUT27M_CK     = 71 ,
    CAL_SRC_AD_VDECPLL_FBCLK_BUF      = 72 ,
    CAL_SRC_AD_VDECPLL_REFCLK_BUF     = 73 ,
    CAL_SRC_AD_PL_RCLK250             = 74 ,
    CAL_SRC_AD_MEMPLL_MONCLK_PRE      = 75 ,
    CAL_SRC_AD_MEMPLL_FBCLK_BUF_D16   = 76 ,
    CAL_SRC_AD_MEMPLL_REFCLK_BUF_D16  = 77 ,
    CAL_SRC_MIO_CK_P0_DIV8            = 78 ,
    CAL_SRC_MIO_CK_P0_DIV4            = 79 ,
    CAL_SRC_AD_SYNC_R_CK              = 80 ,
    CAL_SRC_AD_SYNC_L_CK              = 81 ,
    CAL_SRC_AD_CVBSADC_CKIND_2        = 82 ,
    CAL_SRC_AD_USB20_MONCLK_3P        = 83 ,
    CAL_SRC_AD_USB20_SESSEND_2P       = 84 ,
    CAL_SRC_AD_USB20_MONCLK           = 85 ,
    CAL_SRC_AD_USB20_MONCLK_1P        = 86 ,
    CAL_SRC_AD_USB20_MONCLK_2P        = 87 ,
    CAL_SRC_SPL2CORE                  = 88 ,
    CAL_SRC_MON_CLK                   = 111,

    SRC_CPU_CLK,
    SRC_MEM_CLK,
    SRC_BUS_CLK,
    SRC_TIMER_CLK,
    SRC_FRAC_DIV_CLK,
    SRC_SYSPLL_CLK,
    SRC_APLL1_CLK,
    SRC_APLL2_CLK,
    SRC_SAWLESSPLL_CLK,
    SRC_ADCPLL_CLK,
    SRC_ETHPLL_CLK,
    SRC_ETHNETPLL_CLK,
    
    SRC_VDOIN_MPCLK                   = 129,
    SRC_MIB_OCLK                      = 130,

    // Following for build pass
    CAL_SRC_VOPLL,
    CAL_SRC_APLL1,
    CAL_SRC_APLL2,
    CAL_SRC_USBPLL,
    CAL_SRC_PIXPLL,
    CAL_SRC_DMPLL,
    CAL_SRC_ADCPLL,
} CAL_SRC_T;

//=====================================================================
// Interface

#define BSP_GetDomainClock(eSrc)    BSP_GetClock(eSrc, NULL, NULL, NULL, NULL)
#define BSP_GetPLLClock(eSrc)       BSP_GetClock(eSrc, NULL, NULL, NULL, NULL)

extern void BSP_CkgenInit(void);

extern BOOL BSP_Calibrate(CAL_SRC_T eSource, UINT32 u4Clock);

extern UINT32 BSP_GetClock(CAL_SRC_T eSrc, UINT8* pu1Band, UINT16* pu2MS,
    UINT16* pu2NS, UINT16* pu2Counter);

extern void vDrvVOPLLFreqSet(UINT32 u4CLK);
extern void vDrvVOPLLSet(void);
extern UINT32 _CalGetPLLCounter(UINT32 u4CalSel);

extern UINT32 BSP_GetBand(CAL_SRC_T ePll);
extern UINT32 BSP_SetBand(CAL_SRC_T ePll, UINT32 u4CalEn, UINT32 u4Band);
extern UINT32 BSP_SetVConDebugOut(CAL_SRC_T ePll);



#endif  // X_CKGEN_H

