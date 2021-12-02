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

//add by david.hsieh 20090504
//----------------------------------------------------------------------------
// XTAL config
EXTERN BOOL BSP_IsFPGA(void);

//#define XTAL_STRAP_MODE     (CKGEN_READ32(0) & 0x1c00)
#define XTAL_STRAP_MODE (IO32ReadFldAlign(CKGEN_STRAP_OP, FLD_STRAP_2_0) & 0x2)
    #define SET_XTAL_27MHZ  (0)
    #define SET_XTAL_54MHZ  (2)
    #define IS_XTAL_54MHZ() (0)
    #define IS_XTAL_60MHZ() (0)
    #define IS_XTAL_27MHZ() (1)
    #define IS_XTAL_30MHZ() (0)
    #define GET_XTAL_CLK()  (CLK_27MHZ)

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
#define CLK_200MHZ              200000000
#define CLK_250MHZ              250000000
#define CLK_300MHZ              300000000
#define CLK_400MHZ              400000000
#define CLK_500MHZ              500000000
#define CLK_550MHZ              550000000
#define CLK_600MHZ              600000000



//===========================================================================
// PLL default clock settings

#define APLL1_DEFAULT_CLOCK     CLK_APLL294MHZ  // 270.9504M
#define VPLL_DEFAULT_CLOCK      CLK_81MHZ   // 81 MHz
#define APLL2_DEFAULT_CLOCK     CLK_APLL270MHZ  // 294.912M

//=====================================================================
// Type definitions

typedef enum
{
    CAL_SRC_SAWLESSPLL = 0,             // 0    
    CAL_SRC_CPUPLL_D2,                  // 1
    CAL_SRC_SYSPLL_D2,                  // 2
    CAL_SRC_APLL1,                      // 3
    CAL_SRC_APLL_D3,                    // 4
    CAL_SRC_APLL2,                      // 5
    CAL_SRC_APLL2_D3,                   // 6
    CAL_SRC_ADCPLL,                     // 7
    CAL_SRC_M_CK,                       // 8
    CAL_SRC_VDECPLL,                    // 9
    CAL_SRC_FBOUT,                      // 0x0A
    CAL_SRC_USBPLL,                     // 0x0B
    CAL_SRC_ETHPLL_CLK675M = 13,        // 0x0D
    CAL_SRC_XTAL,                       // 0x0E
    CAL_SRC_ADC_CKTOUT,                 // 0x0F
    CAL_SRC_CVBSADC_CKIND,              // 0x10
    CAL_SRC_HDMI_PLL340M,
    CAL_SRC_HDMI_PIX340M,
    CAL_SRC_HDMI_DEEP170M,
    CAL_SRC_DDDS1_VSP,                  // 0x14
    CAL_SRC_DDDS2_VSP,                  // 0x15
    CAL_SRC_VPLL_TCON_CK,               // 0x16
    CAL_SRC_LVDS_DPIX_CK,               // 0x17
    CAL_SRC_PIXPLL,                     // 0x18

    CAL_SRC_CLKDIG_CTS_D11,             // 0x19
    CAL_SRC_TCLK,                       // 0x1A
    CAL_SRC_OCLK,                       // 0x1B                       
    CAL_SRC_LVDS_CLK_CTS,               // 0x1C
    CAL_SRC_CVBSADC_CKOUT_2,            // 0x1D

    CAL_SRC_CVBSADC_CKOUT = 0x1F,       // 0x1F
    CAL_SRC_DA_SRV_SPL,                 // 0x20
    CAL_SRC_CKOUT_DEMOD,                // 0x21
    CAL_SRC_HDMI_0_DEEP340M_CK,         // 0x22
    CAL_SRC_HDMI_0_PIX340M_CK,          // 0x23
    CAL_SRC_HDMI_0_PLL340M_CK,          // 0x24
    CAL_SRC_HDMI_1_DEEP340M_CK,         // 0x25
    CAL_SRC_HDMI_1_PIX340M_CK,          // 0x26
    CAL_SRC_HDMI_1_PLL340M_CK,          // 0x27
    CAL_SRC_HDMI_2_DEEP340M_CK,         // 0x28
    CAL_SRC_HDMI_2_PIX340M_CK,          // 0x29
    CAL_SRC_HDMI_2_PLL340M_CK,          // 0x2A
    CAL_SRC_HDMI_3_DEEP340M_CK,         // 0x2B
    CAL_SRC_HDMI_3_PIX340M_CK,          // 0x2C
    CAL_SRC_HDMI_3_PLL340M_CK,          // 0x2D
    CAL_SRC_LVDSB_EVEN_CLK_DIV_125M_D11,// 0x2E
    CAL_SRC_LVDSB_ODD_CLK_DIV_125M_D11, // 0x2F
    CAL_SRC_LVDSA_EVEN_CLK_DIV_125M_D11,// 0x30
    CAL_SRC_LVDSA_ODD_CLK_DIV_125M_D11, // 0x31
    CAL_SRC_CLKDIG_D11,                 // 0x32
    CAL_SRC_VOPLL_TEST_CK,              // 0x33
    CAL_SRC_SYNC_R_CK,                  // 0x34
    CAL_SRC_SYNC_L_CK,                  // 0x35
    CAL_SRC_CVBSADC_CKIND_2,            // 0x36

    CAL_SRC_VDECPLL_FBCLK_BUF = 58,     // 0x3A
    CAL_SRC_VDECPLL_REFCLK_BUF,         // 0x3B
    CAL_SRC_PL_RCLK250,                 // 0x3C
    CAL_SRC_HADDS2_FBCLK_BUF,           // 0x3D
    CAL_SRC_PLLGP_TSTCK,                // 0x3E
    CAL_SRC_MJCPLL_FBCLK_BUF,           // 0x3F
    CAL_SRC_MJCPLL_REFCLK_BUF,          // 0x40
    CAL_SRC_CKT_B,                      // 0x41
    CAL_SRC_CKT_A,                      // 0x42
    CAL_SRC_USB20_MONCLK_3P,            // 0x43
    CAL_SRC_USB20_MONCLK_2P,            // 0x44
    CAL_SRC_USB20_MONCLK_1P,            // 0x45
    CAL_SRC_USB20_MONCLK,               // 0x46
    CAL_SRC_MEMPLL_MONCLK_PRE,          // 0x47
    CAL_SRC_MEMFB_D16_CK,               // 0x48
    CAL_SRC_MEMREF_D16_CK,              // 0x49
    CAL_SRC_GRA3D_ROSC_0,               // 0x4A
    CAL_SRC_PCIE11_DDS_MON_CK,          // 0x4B
    CAL_SRC_PCIE11_CDR_FB25M_CK,        // 0x4C
    CAL_SRC_PCIE11_RX250M_CK,           // 0x4D
    CAL_SRC_PCIE11_PLL_FB25M_CK,        // 0x4E
    CAL_SRC_PCIE11_TX250M_CK,           // 0x4F
    CAL_SRC_CA9_ROSC_CK,                // 0x50
    CAL_SRC_MON_TCLK_DIV,               // 0x51
    CAL_SRC_MON_OCLK_DIV2,              // 0x52
    CAL_SRC_MON_OCLK_DIV,               // 0x53
    CAL_SRC_MON_CLK75M_CK,              // 0x54
    CAL_SRC_DDDS2_CK,                   // 0x55
    CAL_SRC_CA9_CPM_OUT,                // 0x56
    CAL_SRC_MAIN2_H,                    // 0x57
    CAL_SRC_BLK2RS_HS,                  // 0x58
    CAL_SRC_MIB_OUT_M_HS,               // 0x59
    CAL_SRC_MIO_CK_P0_DIV4,             // 0x5A
    CAL_SRC_MIO_CK_P0_DIV8,             // 0x5B
    CAL_SRC_MEMPLL_D8 = 91,             // 0x5B
    
    CAL_SRC_ADC_CKOUT = 96,             // 0x60
    CAL_SRC_AD_SOGY_OUT_MON,            // 0x61
    CAL_SRC_AD_SOGY_OUT = 98,           // 0x62
    CAL_SRC_AD_VSYNC_OUT = 99,          // 0x63
    CAL_SRC_AD_HSYNC_OUT,               // 0x64
    CAL_SRC_AD_FB_OUT,                  // 0x65 
    CAL_SRC_AD_SOGY_ADC_CKOUT,          // 0x66

    CAL_SRC_AD_GPIO_PWM_0 = 104,        // 0x68
    CAL_SRC_AD_GPIO_PWM_1,              // 0x69
    CAL_SRC_AD_GPIO_PWM_2,              // 0x6A
    CAL_SRC_AD_GPIO_PWM_3,              // 0x6B
    CAL_SRC_AD_GPIO_PWM_4,              // 0x6C
    CAL_SRC_AD_GPIO_PWM_5,              // 0x6D
    CAL_SRC_AD_GPIO_PWM_6,              // 0x6E
    CAL_SRC_AD_GPIO_PWM_7,              // 0x6F

    SRC_CPU_CLK = 112,                  // 0x70
    SRC_MEM_CLK,                        // 0x71
    SRC_BUS_CLK,                        // 0x72
    SRC_TIMER_CLK,                      // 0x73
    SRC_SYSPLL_CLK,                     // 0x74
    SRC_APLL1_CLK,                      // 0x75
    SRC_APLL2_CLK,                      // 0x76
    SRC_SAWLESSPLL_CLK,                 // 0x77
    SRC_ADCPLL_CLK,                     // 0x78
    SRC_ETHPLL_CLK,                     // 0x79
    SRC_ETHNETPLL_CLK,                  // 0x7A
    SRC_VDECPLL_CLK,                    // 0x7B
    SRC_XPLL_CLK,                       // 0x7C
    SRC_VOPLL_CLK,                      // 0x7D

    SRC_POCLK = 128,                    // 0x80
    SRC_VDOIN_MPCLK,                    // 0x81
    SRC_MIB_OCLK,                       // 0x82
    SRC_POCLK_DPLL,                     // 0x83

    SRC_MJC_CLK = 144,                  // 0x90
    SRC_UNKNOWN
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

