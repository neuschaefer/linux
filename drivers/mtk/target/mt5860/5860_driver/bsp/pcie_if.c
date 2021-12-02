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
 * $RCSfile$
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pcie_if.c
 *  This C file implements the pcie initial hardware protocol functions.  
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#if defined(CC_MT5396) && defined(CC_MTK_LOADER)

#include "c_model.h"
#include "drvcust_if.h"

#include "x_typedef.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_gpio.h"
#include "x_printf.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define PCIE_VIRT                      (IO_VIRT + 0x64000)

/*
 * PCIe PHY register.
 */
#define PCIE_PHY_BASE                    (PCIE_VIRT + 0x1500)
    #define PCIE_PHYD08                        (0x08)    /* PHY register. Tx amplitude */
    #define PCIE_LTSSM_OFFSET            (0x68)    /* PHY register. LTSSM register */
    #define PCIE_RESET_CTRL                 (0x64)    /* PHY register. RESET Control register */
    #define PCIE_TX_RX_PHASE_SEL     (0x5C)    /* PHY register. TX/RX phase select Control register */
    #define PCIE_PHYA90                        (0x90)    /* PHY register. CLK SSC on/off */
    #define PCIE_PHYA9C                        (0x9C)    /* PHY register. Tx pre-emphasis */
    #define PCIE_PHYAB4                        (0xB4)    /* PHY register. PLL calibration register */
    
/*
 * PCIe unit register offsets.
 */
#define PLDA_WDMA_PCI_ADDR0       (0x0000)
#define PLDA_WDMA_PCI_ADDR1       (0x0004)
#define PLDA_RDMA_PCI_ADDR0       (0x0020)
#define PLDA_RDMA_PCI_ADDR1       (0x0024)
    #define CONF_REG(r)                        (r & 0xFFC)
    #define CONF_BUS(b)                        ((b & 0xFF) << 24)
    #define CONF_DEV(d)                        ((d & 0x1F) << 19)
    #define CONF_FUNC(f)                       ((f & 0x7) << 16)

#define PLDA_WDMA_AHB_ADDR       (0x0008)
#define PLDA_RDMA_AHB_ADDR       (0x0028)

#define PLDA_WDMA_CONTROL          (0x000C)
#define PLDA_RDMA_CONTROL          (0x002C)
    #define CONF_SIZE                       ((4) << 12)
    #define CONF_BYTE1                    ((1) << 8)
    #define CONF_BYTE2                    ((3) << 8)    
    #define CONF_BYTE3                    ((7) << 8)    
    #define CONF_BYTE4                    ((0xF) << 8)    
    #define CONF_WCMD                     ((5) << 4)
    #define CONF_RCMD                      ((4) << 4)
    #define CONF_STOP                       ((1) << 1)
    #define CONF_START                     ((1) << 0)
    

#define PLDA_IMASK                         (0x0040)
#define PLDA_ISTATUS                     (0x0044)
#define PLDA_ICMD                           (0x0048)
#define PLDA_INFO                           (0x004C)
#define PLDA_IMSISTATUS               (0x0050)
    /* Write DMA */
    #define INT_WDMAEND                   (1 << 0)
    #define INT_WPCIERR                     (1 << 1)
    #define INT_WAHBERR                    (1 << 2)
    /* Read DMA */
    #define INT_RDMAEND                     (1 << 4)
    #define INT_RPCIERR                       (1 << 5)
    #define INT_RAHBERR                      (1 << 6)
    /* PCI-AHB window */
    #define INT_PAPOSTERR                       (1 << 8)
    #define INT_PAFETCHERR                     (1 << 9)
    #define INT_PAWIN0DB                         (1 << 10)
    #define INT_PAWIN1DB                         (1 << 11)   
    /* AHB-PCI window */
    #define INT_APDISCARD                       (1 << 13)
    #define INT_APFETCHERR                     (1 << 14)
    /* Legacy PCI Interrupt */    
    #define INT_INTA                            (1 << 16)
    #define INT_INTB                            (1 << 17)
    #define INT_INTC                            (1 << 18)    
    #define INT_INTD                            (1 << 19)
    /* MISC */        
    #define INT_SYSERR                       (1 << 20)
    #define INT_PMEVENT                     (1 << 21)
    #define INT_AER                              (1 << 22)
    #define INT_MSI                              (1 << 23)
    #define INT_PMSTATE                      (1 << 29)
    /* All error flag */
    #define INT_ALL_ERR                     (INT_WPCIERR |INT_WAHBERR |INT_RPCIERR|INT_RPCIERR| \
                                                              INT_PAPOSTERR|INT_PAFETCHERR|INT_APDISCARD|INT_APFETCHERR |\
                                                              INT_SYSERR)
    
#define PLDA_IMSIADDR                   (0x0054)

#define PLDA_SLOTCAP                      (0x005C)
#define PLDA_DV                                 (0x0060)
#define PLDA_SUB                               (0x0064)
#define PLDA_CREV                             (0x0068)
#define PLDA_SLOTCSR                       (0x006C)
#define PLDA_PRMCSR                        (0x0070)
#define PLDA_DEVCSR                        (0x0074)
#define PLDA_LINKCSR                       (0x0078)
#define PLDA_ROOTCSR                      (0x007C)

#define PLDA_PCIAHB0_BASE            (0x0080)
#define PLDA_PCIAHB1_BASE            (0x0084)

#define PLDA_AHBPCI0_BASE0            (0x0090)
#define PLDA_AHBPCI0_BASE1            (0x0094)
#define PLDA_AHBPCI1_BASE0            (0x0098)
#define PLDA_AHBPCI1_BASE1            (0x009C)

#define PLDA_AHBPCI_TIMER            (0x00A0)

#define PLDA_GEN2_CONF            (0x00A8)
#define PLDA_ASPM_CONF            (0x00AC)
#define PLDA_PM_STATUS            (0x00B0)
#define PLDA_PM_CONF0              (0x00B4)
#define PLDA_PM_CONF1             (0x00B8)
#define PLDA_PM_CONF2             (0x00BC)

// MSI communication address: 32 bits. Just use the msi register address as msi communication address.
// Must be 64 bits alignment, bit 0:2 = 000.
#define PLDA_MSI_ADDR_SETTING  (PCIE_VIRT + 0xC0)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** PCIE_PreInit
 *  Initialize pcie to set pcie_reset low
 *  @retval void
 */
//-------------------------------------------------------------------------
void PCIE_PreInit(void)
{
#ifdef PCIE_RESET_GPIO
    UINT32 regval32;
    UINT32 u4Gpio;
    UINT32 u4GpioPolarity;

    u4Gpio = PCIE_RESET_GPIO;
    u4GpioPolarity = 0;
    (void)GPIO_Output((INT32)u4Gpio, (INT32 *)&u4GpioPolarity);            

    Printf("PCIE_PreInit, reset gpio=%d.\n", u4Gpio);

    regval32 = HAL_READ32(PCIE_VIRT + PLDA_INFO);
    if (regval32 != 0x0152)
    {
        return;
    }

    /* Disable power down analog circuit  */
    regval32 = HAL_READ32(PCIE_PHY_BASE);
    regval32 &= ~0x20;
    HAL_WRITE32(PCIE_PHY_BASE, regval32);

    /* Enable SSC CLK  */
    regval32 = HAL_READ32(PCIE_PHY_BASE + PCIE_PHYA90);
    regval32 |= 0x800;
    HAL_WRITE32(PCIE_PHY_BASE + PCIE_PHYA90, regval32);

    /* Disable tx pre-emphasis  */
    regval32 = HAL_READ32(PCIE_PHY_BASE + PCIE_PHYA9C);
    regval32 &= ~0xF0;
    HAL_WRITE32(PCIE_PHY_BASE + PCIE_PHYA9C, regval32);

    /* Add tx amplitude  */
    regval32 = HAL_READ32(PCIE_PHY_BASE + PCIE_PHYD08);
    regval32 &= ~0x0F000000;
    regval32 |= 0x0A000000;    
    HAL_WRITE32(PCIE_PHY_BASE + PCIE_PHYD08, regval32);
#else
    Printf("No PCIE on this board.\n");
#endif    
}

//-------------------------------------------------------------------------
/** PCIE_PostInit
 *  Initialize pcie to set pcie_reset high
 *  @retval void
 */
//-------------------------------------------------------------------------
void PCIE_PostInit(void)
{
#ifdef PCIE_RESET_GPIO
    UINT32 regval32;
    UINT32 u4Gpio;
    UINT32 u4GpioPolarity;

    u4Gpio = PCIE_RESET_GPIO;
    u4GpioPolarity = 1;
    (void)GPIO_Output((INT32)u4Gpio, (INT32 *)&u4GpioPolarity);

    Printf("PCIE_PostInit, reset gpio=%d.\n", u4Gpio);

    regval32 = HAL_READ32(PCIE_PHY_BASE + PCIE_PHYAB4);
    if (regval32 & 0x3)
    {
        Printf("PCI-Express PLL calibration fail, status = 0x%04X.\n", regval32);
        return;
    }

    /* Active MAC/PHY */
    HAL_WRITE32(PCIE_PHY_BASE + PCIE_RESET_CTRL, 0xF);
#endif    
}

#endif /* #if defined(CC_MT5396) && defined(CC_MTK_LOADER) */

