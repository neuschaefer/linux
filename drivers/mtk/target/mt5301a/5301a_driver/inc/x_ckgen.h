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


#define CKGENHW_DEBUG   0

//===========================================================================
// Macros for register read/write

#define CKGEN_READ8(offset)			    IO_READ8(CKGEN_BASE, (offset))
#define CKGEN_READ16(offset)			IO_READ16(CKGEN_BASE, (offset))
// #define CKGEN_READ32(offset)			IO_READ32(CKGEN_BASE, (offset))

#define CKGEN_WRITE8(offset, value)	    IO_WRITE8(CKGEN_BASE, (offset), (value))
#define CKGEN_WRITE16(offset, value)	IO_WRITE16(CKGEN_BASE, (offset), (value))
// #define CKGEN_WRITE32(offset, value)	IO_WRITE32(CKGEN_BASE, (offset), (value))

#define CKGEN_REG8(offset)			    IO_REG8(CKGEN_BASE, (offset))
#define CKGEN_REG16(offset)			    IO_REG16(CKGEN_BASE, (offset))
#define CKGEN_REG32(offset)			    IO_REG32(CKGEN_BASE, (offset))

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

//----------------------------------------------------------------------------
// XTAL config
#define XTAL_STRAP_MODE     (CKGEN_READ32(0) & 0x1C00)
    #define XTAL_STRAP_MODE_SHIFT  10
    #define SET_XTAL_27MHZ      (0U << XTAL_STRAP_MODE_SHIFT)   // Normal mode + ICE mode + 27MHz
    #define SET_XTAL_54MHZ      (4U << XTAL_STRAP_MODE_SHIFT)   // Normal mode + ICE mode + 54MHz
    #define SET_XTAL_27MHZ_A    (2U << XTAL_STRAP_MODE_SHIFT)   // Normal mode + No ICE + 27MHz
    #define SET_XTAL_SCAN       (1U << XTAL_STRAP_MODE_SHIFT)   // Scan mode
    #define SET_XTAL_OLT        (3U << XTAL_STRAP_MODE_SHIFT)   // Olt mode

    #define IS_XTAL_54MHZ()     (XTAL_STRAP_MODE == SET_XTAL_54MHZ)
    #define IS_XTAL_27MHZ()     ((XTAL_STRAP_MODE == SET_XTAL_27MHZ) ||(XTAL_STRAP_MODE == SET_XTAL_27MHZ_A))
    #define GET_XTAL_CLK()      (IS_XTAL_54MHZ() ? (CLK_54MHZ) : (CLK_27MHZ))

#define REG_AFECFG4         (0x0110)
    #define RG_SYSPLL_432MHZ_EN (1U << 8)  // On is set 432MHZ.
#define IS_SYSPLL_432()         (CKGEN_READ32(REG_AFECFG4) & RG_SYSPLL_432MHZ_EN)
    
#define REG_PADPUCFG2            (0x0304)
    #define RG_QFP              (1U << 8)      
    
#define REG_VPCLK_STOP		(0x029C)
#define REG_XTAL_CKCFG		(0x0204)
#define REG_CPU_CKCFG		(0x0208)
#define REG_USB_ECLK_CKCFG	(0x0228)

    
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

// x_pinmux.h
#define GPIO_RISE0_OFFSET           REG_RINTEN0
#define GPIO_FALL0_OFFSET           REG_FINTEN0
#define GPIO_EDGE_HIGH0_OFFSET      REG_HINTEN0
#define GPIO_EDGE_LOW0_OFFSET       REG_LINTEN0

#define GPIO_STATUS_WRITE(idx,val)          CKGEN_WRITE32((REG_EDGEINT0 +(4*(idx))), (val))             
#define GPIO_RISE_WRITE(idx,val)            CKGEN_WRITE32((GPIO_RISE0_OFFSET+(4*(idx))), (val))           
#define GPIO_FALL_WRITE(idx,val)            CKGEN_WRITE32((GPIO_FALL0_OFFSET+(4*(idx))), (val))           
#define GPIO_EDGE_HIGH_WRITE(idx,val)       CKGEN_WRITE32((GPIO_EDGE_HIGH0_OFFSET+(4*(idx))), (val)) 
#define GPIO_EDGE_LOW_WRITE(idx,val)        CKGEN_WRITE32((GPIO_EDGE_LOW0_OFFSET+(4*(idx))), (val))   
#define GPIO_STATUS_REG(idx)                CKGEN_REG32(REG_EDGEINT0 +(4*(idx)))                              
#define GPIO_RISE_REG(idx)                  CKGEN_REG32(GPIO_RISE0_OFFSET+(4*(idx)))                      
#define GPIO_FALL_REG(idx)                  CKGEN_REG32(GPIO_FALL0_OFFSET+(4*(idx)))                      
#define GPIO_EDGE_HIGH_REG(idx)             CKGEN_REG32(GPIO_EDGE_HIGH0_OFFSET+(4*(idx)))            
#define GPIO_EDGE_LOW_REG(idx)              CKGEN_REG32(GPIO_EDGE_LOW0_OFFSET+(4*(idx)))              


//MT5387 alais
#if 0
#define REG_GPIO_ED2INTEN       (0x0580)    // TWO EDGE TRIGGERED EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_LEVINTEN       (0x0590)    // LEVEL TRIGGERED EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_ENTPOL         (0x05a0)    // EXTERNAL INTERRUPT POLARITY
#define REG_GPIO_EXTINTEN       (0x05b0)    // EXTERNAL INTERRUPT ENABLE
#define REG_GPIO_EXTINT         (0x05c0)    // EXTERNAL INTERRUPT IDENTIFY BIT
#endif

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


//=====================================================================
// Constant definitions Given by ACD analog designer
#define PLL_BAND_NUM           8


//----------------------------------------------------------------------------
// no use apll setting.
#define REG_APLL1_CFG0          (0x0100)    // Audio PLL 1 Configuration 0
    #define RG_APLL1_MODIN      (0x1ffffffU << 0)   // APLL1 modulator input
    #define RG_APLL1_BIASOPTEN  (1U << 26)  // APLL1 bandgap external enable
    #define RG_APLL1_CAL_EN     (1U << 27)  // APLL1 output clkdiv reset
    #define RG_APLL1_CHP_B      (0x0fU << 28)   // APLL1 scf divider select
#define REG_APLL1_CFG1          (0x0104)    // Audio PLL 1 Configuration 1
    #define RG_APLL1_ENABLE     (1U << 0)   // APLL1 enable
    #define RG_APLL1_FREFSEL    (1U << 1)   // APLL1 reference clk select
    #define RG_APLL1_LPF_R_B    (1U << 2)   // APLL1 lpf resistor value
    #define RG_APLL1_PREDIV     (7U << 4)   // APLL1 predivider code select
    #define APLL1_PREDIV_SHIFT  (4)
    #define RG_APLL1_PREDIV_EN  (1U << 7)   // APLL1 predivider enable
    #define RG_APLL1_TEST_B_0   (1U << 8)   // APLL1 test select
    #define RG_APLL1_TEST_EN    (1U << 9)   // APLL1 test mode enable
    #define RG_APLL1_V2I_RB     (3U << 10)  // APLL1 v2i resistor value
    #define RG_APLL1_VCOG_B     (7U << 12)  // APLL1 vco gain control
    #define APLL1_VCOG_B_SHIFT  (12)
    #define DEFAULT_VCOG_B      (2)
    #define RG_APLL1_VCO_BAND   (7U << 16)  // APLL1 vco band select
    #define APLL1_VCO_BAND_SHIFT    (16)
    #define RG_APLL1_VCTR_SEL   (1U << 20)  // APLL1 output control voltage select
    #define RG_APPL1_TESTCLK_EN (1U << 21)  // APPL1 test mode enable
    #define RG_APLL1_VCOINIT_ENB    (1U << 22)  // APLL1 Trigger vco to oscillate by a falling
    #define RG_APLL1_RESET_B_CORE   (1U << 24)  // APLL1 modulator and post divider reset
    #define RG_APLL1_MUTE       (1U << 25)  // APLL1 modulator and post divider mute
    #define RG_APLL1_DELAY      (1U << 26)  // APLL1 post divider delay
    #define RG_APLL1_POSDIVSEL  (1U << 27)  // APLL1 post divider select
    #define RG_APLL1_SCFDIV     (0xfU << 28)    // APLL1 scf divider select
#define REG_APLL1_CFG2          (0x0108)    // Audio PLL 1 Configuration 2
    #define RG_APLL1_K1            (1U << 0)   // APLL1 K1 select, clk_k1 is APLL_CK/2(0), APLL_CK/3(1)
    #define RG_APLL1_K2            (0x3fU << 8)    // APLL1 K2 select, clk_k2 is clk_k1/apll_k2.
    #define RG_APLL1_BP            (1U << 30)  // APLL1 Bypass, 0:normal, 1:bypass(from pin)
    #define RG_APLL1_DIV_RST       (1U << 31)  // APLL1 Divider counter reset, 0:normal, 1:reset
#define REG_APLL1_CFG3          (0x010c)    // Audio PLL 1 Configuration 3
    #define RG_APPL1_MODDIV     (0xffU << 0)    // APPL1 post divider code select
    #define RG_APLL1_LPFDIV     (0xffU << 8)    // APLL1 lpf clk select
    #define RG_APLL1_RESET_B_CLKDIV (1U << 16)  // APLL1 output clkdiv reset
    #define RG_APLL1_TCLKSEL    (1U << 17)  // APLL1 ADAC lpf clk select
    #define RG_APLL1_MODCLK_SEL (1U << 18)  // APLL1 mod clk select
    #define RG_APLL1_POWERDOWN  (1U << 24)  // APLL1 power down bit
    #define RG_APLL1_BANDGAP    (3U << 27)  // APLL1 bandgap select
    #define DEFAULT_BANDGAP_CUR    (2)     //default bandgap
    #define DEF_APLL1_BANDGAP   (2U << 27)  // default APLL1 bandgap select
    #define RG_APPL1_REV        (0xffU << 24)   // APPL1 reserved register
    #define RG_APLL1_BGPOWER    (1U << 29)  // APLL1 Band gap power down/up
#define REG_APLL2_CFG0          (0x01a0)    // APLL2 Configuration 0
    #define RG_APLL2_MODIN      (0x1ffffffU << 0)   // APLL2 modulator input
    #define RG_APLL2_BIASOPTEN  (1U << 26)  // APLL2 bandgap external enable
    #define RG_APLL2_CAL_EN     (1U << 27)  // APLL2 output clkdiv reset
    #define RG_APLL2_CHP_B      (0x0fU << 28)   // APLL2 scf divider select
#define REG_APLL2_CFG1          (0x01a4)    // APLL2 Configuration 1
    #define RG_APLL2_ENABLE     (1U << 0)   // APLL2 enable
    #define RG_APLL2_FREFSEL    (1U << 1)   // APLL2 reference clk select
    #define RG_APLL2_LPF_R_B    (1U << 2)   // APLL2 lpf resistor value
    #define RG_APLL2_PREDIV     (7U << 4)   // APLL2 predivider code select
    #define APLL2_PREDIV_SHIFT  (4)
    #define RG_APLL2_PREDIV_EN  (1U << 7)   // APLL2 predivider enable
    #define RG_APLL2_TEST_B_0   (1U << 8)   // APLL2 test select
    #define RG_APLL2_TEST_EN    (1U << 9)   // APLL2 test mode enable
    #define RG_APLL2_V2I_RB     (3U << 10)  // APLL2 v2i resistor value
    #define RG_APLL2_VCOG_B     (7U << 12)  // APLL2 vco gain control
    #define APLL2_VCOG_B_SHIFT  (12)
    #define RG_APLL2_VCO_BAND   (7U << 16)  // APLL2 vco band select
    #define APLL2_VCO_BAND_SHIFT    (16)
    #define RG_APLL2_VCTR_SEL   (1U << 20)  // APLL2 output control voltage select
    #define RG_APLL2_TESTCLK_EN (1U << 21)  // APLL2 test mode enable
    #define RG_APLL2_VCOINIT_ENB    (1U << 22)  // APLL2 Trigger vco to oscillate by a falling
    #define RG_APLL2_RESET_B_CORE   (1U << 24)  // APLL2 modulator and post divider reset
    #define RG_APLL2_MUTE       (1U << 25)  // APLL2 modulator and post divider mute
    #define RG_APLL2_DELAY      (1U << 26)  // APLL2 post divider delay
    #define RG_APLL2_POSDIVSEL  (1U << 27)  // APLL2 post divider select
    #define RG_APLL2_SCFDIV     (0xfU << 28)    // APLL2 scf divider select
#define REG_APLL2_CFG2          (0x01a8)    // APLL2 Configuration 2
    #define RG_APLL2_K1            (1U << 0)   // APLL2 K1 select, clk_k1 is APLL_CK/2(0), APLL_CK/3(1)
    #define RG_APLL2_K2            (0x3fU << 8)    // APLL2 K2 select, clk_k2 is clk_k1/apll_k2.
    #define RG_APLL2_BP            (1U << 30)  // APLL2 Bypass, 0:normal, 1:bypass(from pin)
    #define RG_APLL2_DIV_RST       (1U << 31)  // APLL2 Divider counter reset, 0:normal, 1:reset
#define REG_APLL2_CFG3          (0x01ac)    // APLL2 Configuration 3
    #define RG_APLL2_MODDIV     (0xffU << 0)    // APLL2 post divider code select
    #define RG_APLL2_LPFDIV     (0xffU << 8)    // APLL2 lpf clk select
    #define RG_APLL2_RESET_B_CLKDIV (1U << 16)  // APLL2 output clkdiv reset
    #define RG_APLL2_TCLKSEL    (1U << 17)  // APLL2 ADAC lpf clk select
    #define RG_APLL2_MODCLK_SEL (1U << 18)  // APLL2 mod clk select
    #define RG_APLL2_POWERDOWN  (1U << 24)  // APLL2 power down bit
    #define RG_APLL2_BANDGAP    (3U << 27)  // APLL2 bandgap select
    #define DEF_APLL2_BANDGAP   (2U << 27)  // default APLL2 bandgap select
    #define RG_APLL2_REV        (0xffU << 24)   // APLL2 reserved register


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
#define CLK_200MHZ			200000000
#define CLK_250MHZ			250000000
#define CLK_300MHZ			300000000
#define CLK_400MHZ			400000000
#define CLK_500MHZ			500000000
#define CLK_550MHZ			550000000
#define CLK_600MHZ			600000000



//===========================================================================
// PLL default clock settings

#define APLL1_DEFAULT_CLOCK     CLK_APLL294MHZ  // 270.9504M
#define PSPLL_DEFAULT_CLOCK     148500000   // 148.5 MHz
#define VPLL_DEFAULT_CLOCK      CLK_81MHZ   // 81 MHz
#define ADCPLL_DEFAULT_CLOCK    CLK_81MHZ   // 81 MHz
#define B2RPLL_DEFAULT_CLOCK    148352000   // 148.352M
#define APLL2_DEFAULT_CLOCK     CLK_APLL270MHZ  // 294.912M

//=====================================================================
// Type definitions

typedef enum
{
    CAL_SRC_ATDPLL = 0,
    CAL_SRC_SYSPLL,
    CAL_SRC_ADCPLL,
    CAL_SRC_APLL1,
    CAL_SRC_APLL2,
    CAL_SRC_APLL_108M,
    CAL_SRC_CKOUT_CKC,
    CAL_SRC_DMPLL,
    CAL_SRC_TCK_DIV2,
    CAL_SRC_LVDS_DPIX_CLK,
    CAL_SRC_VOPLL,
    CAL_SRC_HDMIPLL,
    CAL_SRC_MEM_CLK = 0xf,
    CAL_SRC_TVD_CLK = 0x1a,
    CAL_SRC_VGA_CKO = 0x1b,
    CAL_SRC_USBPLL_480 = 0x16,
    CAL_SRC_USBPLL_CK = 0x18,
    CAL_SRC_YPBPR_CK54 = 0x20,
    CAL_SRC_NCPO_CK = 0x21,
    // ...
    SRC_CPU_CLK = 0x30,
    SRC_MEM_CLK,
    SRC_BUS_CLK,
    SRC_TIMER_CLK
} CAL_SRC_T;

//=====================================================================
// Interface

#define BSP_GetDomainClock(eSrc)    BSP_GetClock(eSrc, NULL, NULL, NULL, NULL)
#define BSP_GetPLLClock(eSrc)       BSP_GetClock(eSrc, NULL, NULL, NULL, NULL)

extern void BSP_CkgenInit(void);

extern BOOL BSP_Calibrate(CAL_SRC_T eSource, UINT32 u4Clock);

extern UINT32 BSP_GetClock(CAL_SRC_T eSrc, UINT8* pu1Band, UINT16* pu2MS,
    UINT16* pu2NS, UINT16* pu2Counter);

extern void vDrvVOPLLSet(UINT32 u4CLK);

#endif  // X_CKGEN_H

