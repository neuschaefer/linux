/*
 *
 * modphy_drv.c
 * Description:
 * MODPHY driver
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2017 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */
 
/*! \file modphy_drv.c
  \brief Implementation of Modphy Driver.  
 */
 
/**************************************************************************/
/*      INCLUDES:                                                       */
/**************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>   /* for modules */
#include <linux/fs.h>       /* file_operations */
#include <linux/uaccess.h>  /* copy_(to,from)_user */
#include <linux/init.h>     /* module_init, module_exit */
#include <linux/cdev.h>     /* cdev utilities */
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/byteorder/generic.h>
#include "_tistdtypes.h"
#include "modphy_api.h"
#include "puma7_bootcfg_ctrl.h"
#include "puma7.h"
#include "iosfsb_api.h"
#include "sram_api.h"
#include <linux/mrpc/handshake_server.h>
#include <linux/slab.h>

/**************************************************************************/
/*      LOCAL DECLARATIONS:                                               */
/**************************************************************************/
static long modphy_ioctl(struct file *fd, unsigned int cmd, unsigned long arg);

/**************************************************************************/
/*      DEFINES AND STRUCTS                                               */
/**************************************************************************/

/* #define MODPHY_DEBUG */
#ifdef MODPHY_DEBUG
#define DPRINTK(fmt, args...) printk("%-40s:%5d " fmt, __FUNCTION__,__LINE__, ## args)
#else
#define DPRINTK(fmt, args...)
#endif

/* Sections addr */
/*iosf sb ports*/
#define    MODPHY_MOCA_SB_PORT              0x10
#define    MODPHY_DOCSIS_OFDM_SB_PORT       0x1A
#define    MODPHY_DOCSIS_LEGACY_SB_PORT     0x12
#define    MODPHY_SGMII_1_SB_PORT           0x1B
#define    MODPHY_SGMII_2_SB_PORT           0x1D

#define    MODPHY_PCS_UNIT                  0x0
#define    MODPHY_TX_UNIT                   0x100
#define    MODPHY_CMN_DIG_UNIT              0x8000
#define    MODPHY_CMN_ANA_UNIT              0x8080
#define    MODPHY_PLL_UNIT                  0x8100
#define    MODPHY_RX_0_31_UNIT              0x200
#define    MODPHY_RX_32_64_UNIT             0x300

#define    MODPHY_GET_OFFSET_BY_DWORD_ID(dwordId)  ( (dwordId) * 0x4 )

typedef enum
{
    MODPHY_TYPE_1_SGMII_2_5GB,      /* Type1 = XAUI 3.125Gbps, 6.25GHz PLL VCO frequency, refclk =100MHz */
    MODPHY_TYPE_2_DOCSIS_P6,        /* Type2 = 5.4/2.7Gbps - CAT Mode, refclk=27MHz, for cases P7 works with P6 tuner*/
    MODPHY_TYPE_3_DOCSIS_P7_25MHz,  /* Type3 = 6/3Gbps - TGM Mode, refclk=25Mhz */
    MODPHY_TYPE_4,                  /* Type4 = 5.4/2.7Gbps - CAT Mode, refclk=25MHz (test mode) */
    MODPHY_TYPE_5,                  /* Type5=: MOCA, with 6 GHz PLL VCO freqency, and 50 MHz ref clk */
    MODPHY_TYPE_6_SGMII_1GB,        /* Type6=SGMII, with 5 GHz PLL VCO frequency, and 100 MHz ref clk, identical to usb3 phy settings*/
    MODPHY_TYPE_7_DOCSIS_P7_50MHz   /* Type7 = 6/3Gbps - TGM Mode, refclk=50Mhz */
}ModphyTypes_e;

#define    MODPHY_COMMON_CONFIG_OFFSET   0x00
#define    MODPHY_COMMON_STATUS_OFFSET   0x04

/*modPHY clients have between 1-3 lanes. Only relevant clients will have L1 (lane1) and L2*/
#define    MODPHY_RX_CONFIG_L0_OFFSET    0x08
#define    MODPHY_RX_CONFIG_L1_OFFSET    0x0c
#define    MODPHY_RX_CONFIG_L2_OFFSET    0x10

#define    MODPHY_RX_CONTROL_L0_OFFSET   0x14
#define    MODPHY_RX_CONTROL_L1_OFFSET   0x18
#define    MODPHY_RX_CONTROL_L2_OFFSET   0x1c

#define    MODPHY_RX_STATUS_L0_OFFSET    0x20
#define    MODPHY_RX_STATUS_L1_OFFSET    0x24
#define    MODPHY_RX_STATUS_L2_OFFSET    0x28

#define    MODPHY_TX_CONFIG_L0_OFFSET    0x2c
#define    MODPHY_TX_CONFIG_L1_OFFSET    0x30
#define    MODPHY_TX_CONFIG_L2_OFFSET    0x34


/*bits names*/
/*common config*/
#define I_COM0_CMNRESET_L                   BIT(0)
#define I_COM0_PLL1CORECLKA_REQ_H           BIT(1)
#define I_COM0_PLL1CORECLKB_REQ_H           BIT(2)
#define I_COM0_PLL1EN_H                     BIT(3)
#define I_LANE_SOC2PHY_PWRREQ_H_SUS(lane)   (8 + (lane * 0x4))
#define I_LANE_COMMON_CONFIG_DEFAULT_VALUE  ( BIT(I_LANE_SOC2PHY_PWRREQ_H_SUS(0)) | BIT(I_LANE_SOC2PHY_PWRREQ_H_SUS(1)) | BIT(I_LANE_SOC2PHY_PWRREQ_H_SUS(2)) )

/* lane reset  */
#define MODPHY_COMMON_LANE_RESET_LANE_OFFSET   0 /*bits 0-3*/
#define MODPHY_COMMON_LANE_RESET_LANE_WIDTH    4

/*common status*/
#define O_COM0_PLL1CORECLKA_ACK_H               BIT(0)
#define O_COM0_PLL1CORECLKB_ACK_H               BIT(1)
#define O_COM0_PLL1OK_H_MASK                    BIT(2)
#define O_COM0_REFCOMPDONE                      BIT(3)
#define O_LANE_PHY2SOC_PWRACK_H_SUS_MASK(lane)  BIT((8 +(lane * 0x4)))

/*rx control*/
#define I_LANE_RESET_L            BIT(0)
#define MODPHY_DATA_LANE_POWER_MODE_OFFSET   4 /* lane_powerdown bits 4-6*/
#define MODPHY_DATA_LANE_POWER_MODE_WIDTH    3

#define MODPHY_DATA_LANE_POWER_MODE_PS0       0 /*000*/
#define MODPHY_DATA_LANE_POWER_MODE_PS1       1 /*001*/
#define MODPHY_DATA_LANE_POWER_MODE_PS2       2 /*010*/

#define MODPHY_DATA_LANE_RX_STANDBY_OFFSET   12 /*bit 12*/
#define MODPHY_DATA_LANE_RX_STANDBY_WIDTH    1

/*rx status*/
#define O_LANE_PIPEPHYSTATUS_SUS_MASK  BIT(0)
#define O_LANE_PIPEPHYSTATUS_SUS       0
#define O_LANE_RXSTANDBYSTATUS_H_MASK  BIT(4)

/*rx config*/
#define I_LANE_POWERDOWN_SUS0     4
#define I_LANE_POWERDOWN_SUS1     5
#define I_LANE_POWERDOWN_SUS2     6
#define I_LANE_WIDTH              10
#define I_LANE_RX_K_ALIGNMODE     15
#define I_LANE_RATE               18
#define I_LANE_RXSTANDBY_H_MASK   BIT(12)
#define I_LANE_PHYMODE_SUS        20
#define I_LANE_POWERDOWN_SUS_MASK (BIT(4) | BIT(5) | BIT(6))

/* tx status */
#define I_LANE_TXELECIDLE_MASK      BIT(0)

#define MODPHY_MOCA_NUM_OF_LANES          2
#define MODPHY_DOCSIS_LEGACY_NUM_OF_LANES 2
#define MODPHY_DOCSIS_OFDM_NUM_OF_LANES   3
#define MODPHY_SGMII_NUM_OF_LANES         1

#define MODPHY_POWER_MODE_P0        0 /*000*/
#define MODPHY_POWER_MODE_P0S       1 /*001*/
#define MODPHY_POWER_MODE_P1        2 /*010*/
#define MODPHY_POWER_MODE_P2        3 /*011*/
#define MODPHY_LANE_WIDTH           1 /*01*/
#define MODPHY_FULL_RATE            3 /*11*/
#define MODPHY_SGMII_LANE_WIDTH     0 /*00*/
#define MODPHY_SGMII_1G_RATE        1 /*01*/
#define MODPHY_SGMII_2_5G_RATE      2 /*10*/

#ifndef CONFIG_CPU_LITTLE_ENDIAN
#define CONFIG_CPU_LITTLE_ENDIAN
#endif

#ifdef CONFIG_CPU_LITTLE_ENDIAN
#define LE_SET_ENABLE_BIT(addr,bit_mask)            (*((volatile unsigned int *)(addr)) |= (cpu_to_le32(bit_mask)))
#define BE_SET_ENABLE_BIT(addr,bit_mask)            (*((volatile unsigned int *)(addr)) |= (bit_mask))
#define SET_ENABLE_BIT(addr,bit_mask,isBigEndian)   ((isBigEndian == True)? BE_SET_ENABLE_BIT(addr,bit_mask):LE_SET_ENABLE_BIT(addr,bit_mask))

#define LE_IS_ENABLE_BIT_SET(addr,bit_mask)            ( *( (volatile unsigned int *)(addr) ) & (cpu_to_le32(bit_mask)) )
#define BE_IS_ENABLE_BIT_SET(addr,bit_mask)            ( *( (volatile unsigned int *)(addr) ) & (bit_mask) )
/* bit_mask works only for a single bit!! */
#define IS_ENABLE_BIT_SET(addr,bit_mask,isBigEndian)   ((isBigEndian == True)? BE_IS_ENABLE_BIT_SET(addr,bit_mask):LE_IS_ENABLE_BIT_SET(addr,bit_mask))

#define LE_SET_REGISTER(addr,bit_mask)                (*((volatile unsigned int *)(addr)) = (cpu_to_le32(bit_mask)))
#define BE_SET_REGISTER(addr,bit_mask)                (*((volatile unsigned int *)(addr)) = (bit_mask))
#define SET_REGISTER(addr,bit_mask,isBigEndian)       ((isBigEndian == True)? BE_SET_REGISTER(addr,bit_mask):LE_SET_REGISTER(addr,bit_mask))

#define LE_SET_DISABLE_BIT(addr,bit_mask)            (*((volatile unsigned int *)(addr)) &= (~(cpu_to_le32(bit_mask))))
#define BE_SET_DISABLE_BIT(addr,bit_mask)            (*((volatile unsigned int *)(addr)) &= (~(bit_mask)))
#define SET_DISABLE_BIT(addr,bit_mask,isBigEndian)   ((isBigEndian == True)? BE_SET_DISABLE_BIT(addr,bit_mask):LE_SET_DISABLE_BIT(addr,bit_mask))

#else
#define SET_ENABLE_BIT(addr,bit_mask,isBigEndian)      (*((volatile unsigned int *)(addr)) |= (bit_mask))
#define IS_ENABLE_BIT_SET(addr,bit_mask,isBigEndian)   ( *( (volatile unsigned int *)(addr) ) & (bit_mask) )
#define SET_REGISTER(addr,bit_mask,isBigEndian)        (*((volatile unsigned int *)(addr)) = (bit_mask))
#define SET_DISABLE_BIT(addr,bit_mask,isBigEndian)      (*((volatile unsigned int *)(addr)) &= (~(bit_mask)))
#endif

#define FIRST_MINOR 0
#define MINOR_CNT   1

#define MODPHY_OK   (0)
#define MODPHY_FAIL (-1)

#define MODPHY_READ_BIT_TIMEOUT_USEC 20000 /*timeout for reading bits in init sequence in usec*/

struct modphyClient
{
    unsigned int iosfPort;
    unsigned int laneId[MODPHY_MAX_NUM_OF_LANES];
    ModphyTypes_e modPhyType;
    unsigned int baseAddr;
    int          numOfLanes;
    Bool         isBigEndian;
    modphyKgregs_t usrKgregs;
};

static struct modphyClient modphyClientArray[MODPHY_NUM_OF_CLIENTS];
static Bool modphyIsInitArrayDone;

/* Common Lane Power Gate is not enabled by design as of now. Workaround is sought after.*/
/* For the timebeing to keep only data lane Power gate capability for all modphy */
static ModphyStatusData_t modphyClientStatusData[] = 
{
    {MODPHY_MOCA_CLIENT_ID,                 (MODPHY_LANE_1 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, true, false},  false, false}},
    {MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID,     (MODPHY_LANE_1 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, true, false},  false, true}},
    {MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID,    (MODPHY_LANE_2 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, true, true},   true, false}},
    {MODPHY_SGMII0_1G_CLIENT_ID,            (MODPHY_LANE_0 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, false, false}, false, false}},
    {MODPHY_SGMII1_1G_CLIENT_ID,            (MODPHY_LANE_0 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, false, false}, false, false}},
    {MODPHY_SGMII0_2_5G_CLIENT_ID,          (MODPHY_LANE_0 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, false, false}, true, false}},
    {MODPHY_SGMII1_2_5G_CLIENT_ID,          (MODPHY_LANE_0 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, false, false}, true, false}},
    {MODPHY_MOCA_50MHZ_CLIENT_ID,           (MODPHY_LANE_1 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, true, false},  true, false}},
    {MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID,     (MODPHY_LANE_1 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, true, false},  false, true}},
    {MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID,    (MODPHY_LANE_2 + 1), MODPHY_KG_POWER_STATE_OFF, {MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF, MODPHY_KG_POWER_STATE_OFF}, MODPHY_KG_POWER_STATE_3, {{true, true, true},   true, false}},
};

/**************************************************************************/
/*      LOCAL VARIABLES:                                                  */
/**************************************************************************/

static dev_t dev;
static struct cdev *c_dev;
static struct class *cl;

/* Structure to map driver functions to kernel */
static struct file_operations modphy_drv_fops =
{
    .owner   = THIS_MODULE,
    .unlocked_ioctl   = modphy_ioctl /*We assume there are no 2 processes in a race condition*/
};

/***************************************************************************************/
/* LOCAL FUNCTIONS                                                                     */
/***************************************************************************************/

int modphy_read_wait_bit(unsigned int addr, int bit_mask, int expected_val, int timeout_usec, ModphyController_e modphyClientId);

/*! \fn void bit_field32_set(uint32 *addr, uint32 data, uint32 offset, uint32 width)
 *  \brief set the value of a specific bit-field in a given 32-bit field
 *  \param[in] addr the address of the 32-bit field that contains the bit-field
 *  \param[in] data the value to be written
 *  \param[in] offset the offset (in bits) of the required bit-field
 *  \param[in] width the number of bits in the required bit-field
 *  \return none.
 */
static inline void be_bit_field32_set(volatile Uint32 *addr, Uint32 data, Uint32 offset, Uint32 width)
{
    Uint32 mask;

    mask = ((0xFFFFFFFF >> (32 - width)) << offset);

    *addr &= ~mask;
    *addr |=  mask & (data << offset);
}

static inline void le_bit_field32_set(volatile Uint32 *addr, Uint32 data, Uint32 offset, Uint32 width)
{
    Uint32 mask;

    mask = cpu_to_le32((0xFFFFFFFF >> (32 - width)) << offset);

    *addr &= ~mask;
    *addr |=  mask & cpu_to_le32(data << offset);
}

static inline void bit_field32_set(volatile Uint32 *addr, Uint32 data, Uint32 offset, Uint32 width, Bool isBigEndian)
{
    if (isBigEndian)
    {
        be_bit_field32_set(addr, data, offset, width);
    } else
    {
        le_bit_field32_set(addr, data, offset, width);
    }
}

static int modphy_common_lane_init(ModphyController_e modphyClientId)
{

    DPRINTK("\nmodphy_init setting common_config bits I_LANE2_SOC2PHY_PWRREQ_H_SUS, I_LANE1_SOC2PHY_PWRREQ_H_SUS, I_LANE0_SOC2PHY_PWRREQ_H_SUS\n");
    SET_REGISTER( modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_LANE_COMMON_CONFIG_DEFAULT_VALUE , modphyClientArray[modphyClientId].isBigEndian); /* Gives data lanes control on Core well power */
    DPRINTK("\nmodphy_init setting common_config bits I_COM0_CMNRESET_L, I_COM0_PLL1EN_H\n");
    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_CMNRESET_L, modphyClientArray[modphyClientId].isBigEndian); /*takes the common lane out of reset. assert bit 0*/

    DPRINTK("\nmodphy_init reading common_status bit O_COM0_REFCOMPDONE\n");
    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_REFCOMPDONE, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*read '1' - PLL OK signal arrived from modPHY*/
    {
        printk(KERN_ERR "\nmodphy_init reading register 0x%x, bit O_COM0_REFCOMPDONE (%lu) - WRONG VALUE (should be '1')\n", (modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET), O_COM0_REFCOMPDONE);
        return MODPHY_FAIL;
    }

    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_PLL1EN_H, modphyClientArray[modphyClientId].isBigEndian); /*enable PLL #1 within the common lane*/
    /* ADD THIS CODE. TO CHANGE TO FUNCTION/Macro */
    DPRINTK("\nmodphy_init reading common_status bit O_COM0_PLL1OK_H_MASK\n");
    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_PLL1OK_H_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*read '1' - COMP done signal arrived from modPHY*/
    {
        printk(KERN_ERR "\nmodphy_init reading register 0x%x, bit O_COM0_PLL1OK_H_MASK (%lu) - WRONG VALUE (should be '1')\n", (modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET), O_COM0_REFCOMPDONE);
        return MODPHY_FAIL;
    }

    /* Waiting for PLL Ok after enabling the PLL */
    DPRINTK("\nmodphy_init reading common_status bit O_COM0_PLL1OK_H_MASK\n");
    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_PLL1OK_H_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*read '1' - COMP done signal arrived from modPHY*/
    {
        printk(KERN_ERR "\nmodphy_init reading register 0x%x, bit O_COM0_PLL1OK_H_MASK (%lu) - WRONG VALUE (should be '1')\n", (modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET), O_COM0_PLL1OK_H_MASK);
        return MODPHY_FAIL;
    }

    DPRINTK("\nmodphy_init setting common_config bit I_COM0_PLL1CORECLKB_REQ_H\n");
    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_PLL1CORECLKB_REQ_H, modphyClientArray[modphyClientId].isBigEndian); /*Request to turn on the clock signal which is a divided clock from the PLL*/

    DPRINTK("\nmodphy_init reading common_status bit O_COM0_PLL1CORECLKB_ACK_H\n");
    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_PLL1CORECLKB_ACK_H, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Read '1' - PLL OK signal arrived from modPHY*/
    {
        printk(KERN_ERR "\nmodphy_init reading bit O_COM0_PLL1CORECLKB_ACK_H - WRONG VALUE (should be '1')\n");
        return MODPHY_FAIL;
    }

    DPRINTK("\nmodphy_init setting common_config bit I_COM0_PLL1CORECLKA_REQ_H\n");
    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_PLL1CORECLKA_REQ_H, modphyClientArray[modphyClientId].isBigEndian); /*Request to turn on the clock signal which is a divided clock from the PLL*/

    DPRINTK("\nmodphy_init reading common_status bit O_COM0_PLL1CORECLKA_ACK_H\n");
    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_PLL1CORECLKA_ACK_H, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Read '1'  - PLL OK signal arrived from modPHY*/
    {
        printk(KERN_ERR "\nmodphy_init reading bit O_COM0_PLL1CORECLKA_ACK_H - WRONG VALUE (should be '1')\n");
        return MODPHY_FAIL;
    }
    DPRINTK("\nmodphy_init reading common_status bit O_COM0_PLL1CORECLKA_ACK_H\n");

    /* Common Lane is up at this point*/
    modphyClientStatusData[modphyClientId].commonLanePowerState = MODPHY_KG_POWER_STATE_0;

    return MODPHY_OK;
}

static void modphy_kg_regs_from_usr(ModphyController_e modphyClientId)
{
    unsigned i;

    for (i = 0; i != modphyClientArray[modphyClientId].usrKgregs.avpsCnt; i++)
    {
        printk(KERN_WARNING "IOSFSB_CRWRITE: Port=0x%x, Addr=0x%x, Val=0x%x\n",
                modphyClientArray[modphyClientId].iosfPort,
                modphyClientArray[modphyClientId].usrKgregs.avps[i].addr,
                modphyClientArray[modphyClientId].usrKgregs.avps[i].val);

        iosfsb_write(modphyClientArray[modphyClientId].iosfPort,
                IOSFSB_OP_CRWRITE,
                modphyClientArray[modphyClientId].usrKgregs.avps[i].addr,
                modphyClientArray[modphyClientId].usrKgregs.avps[i].val,
                0);
    }
}

static void modphy_kg_regs_post(ModphyController_e modphyClientId)
{
    unsigned int siliconStepping;

    DPRINTK("START KG_REGS client ID = %d\n", modphyClientId);

    if (modphyClientArray[modphyClientId].usrKgregs.avps)
    {
        printk(KERN_WARNING "modphy_kg_regs_post: Using user-configured kgregs for ClientId=%d\n", modphyClientId);
        modphy_kg_regs_from_usr(modphyClientId);
        return;
    }

    if (SRAM_Get_Boot_Param(SILICON_STEPPING_ID, &siliconStepping))
    {
        printk("%s:%d ERROR .... silicon stepping retrieval unsuccessful \n", __FUNCTION__, __LINE__);
        BUG_ON(1);
    }

    if (siliconStepping <= SILICON_STEPPING_ID_A_2)
    {
        DPRINTK("\nm KG-regs POST settings for A\n");

        switch (modphyClientId)
        {
            case MODPHY_MOCA_CLIENT_ID:
                {
                    /* soc.moca.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.moca.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.moca.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.moca.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.moca.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2228, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2224, 0xa40d0716, 0);
                }
                break;
            case MODPHY_SGMII0_1G_CLIENT_ID:
                {
                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.ethernet.sgmii0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_SGMII1_1G_CLIENT_ID:
                {
                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.ethernet.sgmii0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_SGMII0_2_5G_CLIENT_ID:
                {
                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.ethernet.sgmii0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_SGMII1_2_5G_CLIENT_ID:
                {
                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.ethernet.sgmii0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_MOCA_50MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2228, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2224, 0xa40d0716, 0);
                }
                break;
            default:
                DPRINTK("Wrong modphy client ID %d \n", modphyClientId);
                break;
        }
    }
    else
    {
        DPRINTK("\nm KG-regs POST settings for B\n");

        switch (modphyClientId)
        {
            case MODPHY_MOCA_CLIENT_ID:
                {
                    /* soc.moca.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.moca.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.moca.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.moca.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.moca.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2228, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2224, 0xa40d0716, 0);
                }
                break;
            case MODPHY_SGMII0_1G_CLIENT_ID:
                {
                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.ethernet.sgmii0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_SGMII1_1G_CLIENT_ID:
                {
                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.ethernet.sgmii0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_SGMII0_2_5G_CLIENT_ID:
                {
                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.ethernet.sgmii0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_SGMII1_2_5G_CLIENT_ID:
                {
                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.ethernet.sgmii0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.ethernet.sgmii0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_MOCA_50MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);
                }
                break;
            case MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID:
                {
                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2228, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword10.reg_cfg_data_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x10221e0, 0);

                    /* soc.docsis3p0.modphy.clane.cmn_dig.cmn_dig_dword15.o_cfg_cmn_dynclkgate_mode_1_0=0x3 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x6, 0);

                    /* soc.docsis3p0.modphy.dlane0.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0xa40d0716, 0);

                    /* soc.docsis3p0.modphy.dlane1.pcs.pcs_dword9.reg_enable_pwr_gating=0x1 */
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2224, 0xa40d0716, 0);
                }
                break;
            default:
                DPRINTK("Wrong modphy client ID %d \n", modphyClientId);
                break;
        }
    }
}

static void modphy_kg_regs(ModphyController_e modphyClientId)
{
    unsigned int siliconStepping;

    DPRINTK("START KG_REGS client ID = %d\n", modphyClientId);

    if (modphyClientArray[modphyClientId].usrKgregs.avps)
    {
        printk(KERN_WARNING "modphy_kg_regs: Using user-configured kgregs for ClientId=%d\n", modphyClientId);
        modphy_kg_regs_from_usr(modphyClientId);
        return;
    }

    if (SRAM_Get_Boot_Param(SILICON_STEPPING_ID, &siliconStepping))
    {
        printk("%s:%d ERROR .... silicon stepping retrieval unsuccessful \n", __FUNCTION__, __LINE__);
        BUG_ON(1);
    }

    if (siliconStepping <= SILICON_STEPPING_ID_A_2)
    {
        DPRINTK("\nm KG-regs settings for A\n");

        switch (modphyClientId)
        {
            case MODPHY_MOCA_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x78, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10804, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x1030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x35347a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x35347a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_MOCA_50MHZ_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x3c, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10904, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x2030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70012, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x35347a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x35347a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x78, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10804, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x1030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x3C, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10904, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x2030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70012, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }

            case MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x78, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10804, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x1030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2230, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x230c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2300, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2390, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2210, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2220, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2218, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2264, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2290, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x22b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x22ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2294, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x228c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2340, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2344, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2348, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2314, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x23cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2368, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x23ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x236c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2378, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2310, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x230c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2318, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2324, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x237c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2358, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2354, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2304, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x233c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2350, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2308, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x238c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2390, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2384, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2370, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2204, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2228, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2224, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    break;
                }
            case MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x3c, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10904, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x2030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70012, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2230, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x230c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2300, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2390, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2210, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2220, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2218, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2264, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2290, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x22b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x22ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2294, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x228c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2340, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2344, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2348, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2314, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x23cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2368, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x23ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x236c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2378, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2310, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x230c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2318, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2324, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x237c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2358, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2354, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2304, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x233c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2350, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2308, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x238c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2390, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2384, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2370, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2204, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2228, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2224, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    break;
                }
            case MODPHY_SGMII0_1G_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x0521b414, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x404, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x19, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x30c05, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x78003c00, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x3030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x2521b414, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_SGMII1_1G_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x0521b414, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x404, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x19, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x30c05, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x78003c00, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x3030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x2521b414, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_SGMII0_2_5G_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x05096254, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10803, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x100000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x10002, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x3030a, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x25096254, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020704, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x24200024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_SGMII1_2_5G_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x05096254, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10803, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x100000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x10002, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x3030a, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x25096254, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020704, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x24200024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            default:
                printk(KERN_ERR "Wrong modphy client ID %d \n", modphyClientId);
                break;
        }
    } else
    {
        DPRINTK("\nm KG-regs settings for B\n");

        switch (modphyClientId)
        {
            case MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x78, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10804, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x78003c00, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x1030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_MOCA_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x78, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10904, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x1030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x35347a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x35347a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x3C, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10804, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x78003c00, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x2030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70012, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_MOCA_50MHZ_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x3c, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10904, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x2030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70012, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x35347a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x35347a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x78, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10804, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x78003c00, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x1030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2230, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x230c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2300, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2390, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250ba194, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2210, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2220, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2218, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2264, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2290, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x22b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x22ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2294, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x228c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2340, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2344, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2348, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2314, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x23cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2368, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x23ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x236c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2378, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2310, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x230c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2318, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2324, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x237c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2358, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2354, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2304, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x233c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2350, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2308, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x238c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2390, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2384, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2370, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2204, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2228, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2224, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    break;
                }
            case MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x050d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x3c, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10804, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x78003c00, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x2030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70012, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2230, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x30, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x230c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2300, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2390, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x250d7200, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2210, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10, 0x723f3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2220, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2218, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2264, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x64, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2290, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x90, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x22b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xb8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x22ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0xac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2294, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x94, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x228c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2340, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2344, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2348, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2314, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x23cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2368, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x23ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x1ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x236c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x16c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2378, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2310, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x230c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x10c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2318, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2324, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x237c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x17c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2358, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2354, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2304, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x233c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x13c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2350, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2308, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x238c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x18c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2390, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2384, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2370, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2204, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x4, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2228, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x28, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2224, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x24, 0x840d0716, 0);

                    break;
                }
            case MODPHY_SGMII0_1G_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x0521b414, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x404, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x19, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x30c05, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x78003c00, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x3030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x2521b414, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_SGMII1_1G_CLIENT_ID:
                {
                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x0521b414, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x404, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x19, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x30c05, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x78003c00, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x3030b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x2521b414, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x200000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_SGMII0_2_5G_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x05096254, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10803, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x100000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x10002, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x3030a, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x25096254, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020704, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x24200024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            case MODPHY_SGMII1_2_5G_CLIENT_ID:
                {

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x05096254, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8004, 0x21000060, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8084, 0x504, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x80a4, 0x8240540e, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8100, 0x1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8118, 0x10803, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8128, 0x70000000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8108, 0x100000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x810c, 0x10002, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8124, 0x3030a, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8120, 0x70009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2030, 0x8080b0b0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x98121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0016, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2100, 0xa5001455, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8020, 0x25096254, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8008, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2010, 0x723f3400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2020, 0xd071624, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2018, 0xb90009, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2064, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2090, 0x2b404d55, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20b8, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x20ac, 0x3000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2094, 0x8000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x208c, 0xc782040, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2140, 0x1020304, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2144, 0x1020704, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2148, 0x1021110, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2114, 0xa3816300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21cc, 0x1e020007, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2168, 0x38009608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x21ac, 0x7a13608, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x216c, 0x10200100, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2178, 0x243f84, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2110, 0x6a7, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x210c, 0x97121b03, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2118, 0x2c2c7a7b, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2124, 0x40000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x217c, 0x10030000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2158, 0x404000, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2154, 0x24200024, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2104, 0x642bf2, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x213c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2150, 0x271f1f1f, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2108, 0x34, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x218c, 0x250d2701, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2190, 0x4c0f0022, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2184, 0x3f271300, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2170, 0xff4c3d04, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2004, 0x25, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x803c, 0x0, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2028, 0x1022180, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x8030, 0x900400, 0);

                    iosfsb_write(modphyClientArray[modphyClientId].iosfPort, IOSFSB_OP_CRWRITE, 0x2024, 0x840d0716, 0);

                    break;
                }
            default:
                printk(KERN_ERR "Wrong modphy client ID %d \n", modphyClientId);
                break;
        }
    }
}

/*********************************************************************************************************/
/*! \fn int modphy_tx_disable(ModphyLanes_e lane, ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function disable TX lanes
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_tx_disable(ModphyLanes_e lane, ModphyController_e modphyClientId)
{
    int modphyReadFail = MODPHY_OK;

    DPRINTK("\nmodphy_init clear rx_status bit O_LANE_PIPEPHYSTATUS_SUS, lane:%d\n", lane);
    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian); /*Clears the Status bit.*/

    DPRINTK("\nmodphy_init setting rx_config bit I_LANE_POWERDOWN_SUS[0-2], lane:%d\n", lane);

    bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_POWER_MODE_P0S, I_LANE_POWERDOWN_SUS0, 3, modphyClientArray[modphyClientId].isBigEndian); /*P0s - Tx off operation*/

    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Verify that the pipephystatus signal goes from high to low*/
    {
        modphyReadFail = MODPHY_FAIL;
        printk(KERN_ERR "\nmodphy_init reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
    }

    if (MODPHY_FAIL == modphyReadFail)
    {
        return MODPHY_FAIL; /*one of the values in the loop was wrong*/
    }

    return MODPHY_OK;
}
EXPORT_SYMBOL(modphy_tx_disable);
/*********************************************************************************************************/
/*! \fn int modphy_go2_fast_recovery(ModphyLanes_e lane, ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function resets ModPhy client. Used for ext.switches WA. Can be scheduled out.
 *  \param[in] lane :           lane index
 *  \param[in] modphyClientId : Client ID
 *  \return void
 ********************************************************************************************************/
int modphy_go2_fast_recovery(ModphyLanes_e lane, ModphyController_e modphyClientId)
{

    DPRINTK("\nmodphy_go2_fast_recovery configure lane_powerdown lane:%d\n", lane);
    bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)),
            MODPHY_DATA_LANE_POWER_MODE_PS2, MODPHY_DATA_LANE_POWER_MODE_OFFSET, MODPHY_DATA_LANE_POWER_MODE_WIDTH, modphyClientArray[modphyClientId].isBigEndian);

    DPRINTK("\nmodphy_go2_fast_recovery clear rxStandby lane:%d\n", lane);
    bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)),
            0, MODPHY_DATA_LANE_RX_STANDBY_OFFSET, MODPHY_DATA_LANE_RX_STANDBY_WIDTH, modphyClientArray[modphyClientId].isBigEndian);
    return MODPHY_OK;

}
EXPORT_SYMBOL(modphy_go2_fast_recovery);
/*********************************************************************************************************/
/*! \fn int modphy_go2_full_functonal(ModphyLanes_e lane, ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function resets ModPhy client. Used for ext.switches WA. Can be scheduled out.
 *  \param[in] lane :           lane index
 *  \param[in] modphyClientId : Client ID
 *  \return void
 ********************************************************************************************************/
int modphy_go2_full_functonal(ModphyLanes_e lane, ModphyController_e modphyClientId)
{
    DPRINTK("\nmodphy_go2_full_functonal configure lane_powerdown lane:%d\n", lane);
    bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)),
            MODPHY_DATA_LANE_POWER_MODE_PS0, MODPHY_DATA_LANE_POWER_MODE_OFFSET, MODPHY_DATA_LANE_POWER_MODE_WIDTH, modphyClientArray[modphyClientId].isBigEndian);

    DPRINTK("\nmodphy_go2_full_functonal clear rxStandby lane:%d\n", lane);
    bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)),
            0, MODPHY_DATA_LANE_RX_STANDBY_OFFSET, MODPHY_DATA_LANE_RX_STANDBY_WIDTH, modphyClientArray[modphyClientId].isBigEndian);

    return MODPHY_OK;

}
EXPORT_SYMBOL(modphy_go2_full_functonal);
/*********************************************************************************************************/
/*! \fn int modphy_common_lane_reset(ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function resets ModPhy client. Used for ext.switches WA. Can be scheduled out.
 *  \param[in] modphyClientId : Client ID
 *  \return void
 ********************************************************************************************************/
int modphy_common_lane_reset(ModphyController_e modphyClientId)
{
    DPRINTK("\nmodphy_common_lane_reset ClientId=%d addr ***** 0x%x ***** \n", modphyClientId, (modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET));

    bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET), 0, MODPHY_COMMON_LANE_RESET_LANE_OFFSET, MODPHY_COMMON_LANE_RESET_LANE_WIDTH, modphyClientArray[modphyClientId].isBigEndian);

    return MODPHY_OK;

}
EXPORT_SYMBOL(modphy_common_lane_reset);
/*********************************************************************************************************/
/*! \fn int modphy_reset_k_align(ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function reset K alignment in modphy
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_reset_k_align(ModphyController_e modphyClientId)
{
    int i = 0;

    for (i = 0; i < modphyClientArray[modphyClientId].numOfLanes; i++)
    {
        /*cri_reset_kalignlck*/
        iosfsb_regReadModifyWrite(modphyClientArray[modphyClientId].iosfPort,
                modphyClientArray[modphyClientId].laneId[i] + MODPHY_PCS_UNIT + MODPHY_GET_OFFSET_BY_DWORD_ID(1), 0x40000000, 0, 0x40000000);
        iosfsb_regReadModifyWrite(modphyClientArray[modphyClientId].iosfPort,
                modphyClientArray[modphyClientId].laneId[i] + MODPHY_PCS_UNIT + MODPHY_GET_OFFSET_BY_DWORD_ID(1), 0x0, 0, 0x40000000);
    }
    return MODPHY_OK;
}
EXPORT_SYMBOL(modphy_reset_k_align);

/*********************************************************************************************************/
/*! \fn void modphy_reset_client(ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function resets ModPhy client. Used for ext.switches WA. Can be scheduled out.
 *  \param[in] modphyClientId : Client ID
 *  \return void
 ********************************************************************************************************/
void modphy_reset_client(ModphyController_e modphyClientId)
{
    SET_REGISTER((modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONTROL_L0_OFFSET), 0, modphyClientArray[modphyClientId].isBigEndian);

    msleep(10);
    SET_REGISTER((modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONTROL_L0_OFFSET), 1, modphyClientArray[modphyClientId].isBigEndian);
}
EXPORT_SYMBOL(modphy_reset_client);

/*********************************************************************************************************/
/*! \fn int modphy_init(ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function inits the modphy for a specific client.
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_init(ModphyController_e modphyClientId)
{
    int i;
    int retVal = MODPHY_FAIL;

    if (modphyIsInitArrayDone != True)
    {
        printk(KERN_ERR "\n Modphy driver need to be initialize before trying to init modphy %d \n"
                "(0=MOCA, 1=LEGACY DOCSIS, 2=OFDM DOCSIS, 3=SGMII, 4=SGMII2)\n", modphyClientId);
        return MODPHY_FAIL;
    }
    DPRINTK("\nStarting modphy_init for client = %d (0=MOCA, 1=LEGACY DOCSIS, 2=OFDM DOCSIS, 3=SGMII, 4=SGMII2)\n", modphyClientId);

    if ((modphyClientId == MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID) || (modphyClientId == MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID) ||
            (modphyClientId == MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID) || (modphyClientId == MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID))
    {
        /*Change Docsis lanes Power state to '010'. Reset value is '000' in the Phy Modphy*/
        for (i = 0; i < modphyClientArray[modphyClientId].numOfLanes; i++)
        {
            DPRINTK("\nmodphy_init setting rx_config bit I_LANE_POWERDOWN_SUS[0-2], lane:%d\n", i);
            bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (i * 0x4)), MODPHY_POWER_MODE_P1, I_LANE_POWERDOWN_SUS0, 3, modphyClientArray[modphyClientId].isBigEndian); /*P1 - normal operation*/
        }
    }

    /* Calibration and configuration of KG regs */
    modphy_kg_regs(modphyClientId);

    /* Common Lane reset, PLL enable and Clocks acquired */
    retVal = modphy_common_lane_init(modphyClientId);

    /* Post Configuration of KG regs */
    modphy_kg_regs_post(modphyClientId);

    return retVal;
}
EXPORT_SYMBOL(modphy_init);

/*********************************************************************************************************/
/*! \fn int modphy_lane_configure(ModphyLanes_e lane, ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function configure the modphy for a specific client.
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_lane_configure(ModphyLanes_e lane, ModphyController_e modphyClientId)
{
    int modphyReadFail = MODPHY_OK;
    if (modphyIsInitArrayDone != True)
    {
        printk(KERN_ERR "\n Modphy driver need to be initialized before trying to init modphy %d \n"
                "(0=MOCA, 1=LEGACY DOCSIS, 2=OFDM DOCSIS, 3=SGMII, 4=SGMII2)\n", modphyClientId);
        return MODPHY_FAIL;
    }

    if ((modphyClientId == MODPHY_SGMII0_1G_CLIENT_ID) || (modphyClientId == MODPHY_SGMII1_1G_CLIENT_ID))
    {
        bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_SGMII_1G_RATE, I_LANE_RATE, 2, modphyClientArray[modphyClientId].isBigEndian); /*Configure lane rate - '10' for the default SerDes Full rate*/
    } else if ((modphyClientId == MODPHY_SGMII0_2_5G_CLIENT_ID) || (modphyClientId == MODPHY_SGMII1_2_5G_CLIENT_ID))
    {
        bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_SGMII_2_5G_RATE, I_LANE_RATE, 2, modphyClientArray[modphyClientId].isBigEndian); /*Configure lane rate - '10' for the default SerDes Full rate*/
    } else
    {
        bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_FULL_RATE, I_LANE_RATE, 2, modphyClientArray[modphyClientId].isBigEndian); /*Configure lane rate - '10' for the default SerDes Full rate*/
    }

    DPRINTK("\nmodphy_lane_configure set rx lane :%d K alignment\n", lane);
    bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), 0, I_LANE_RX_K_ALIGNMODE, 1, modphyClientArray[modphyClientId].isBigEndian); /*Set Rx K_align mode = 0 (Dynamic)*/

    DPRINTK("\nmodphy_lane_configure set rx lane:%d width  \n ", lane);
    if ((modphyClientId == MODPHY_SGMII0_1G_CLIENT_ID) || (modphyClientId == MODPHY_SGMII1_1G_CLIENT_ID)
            || (modphyClientId == MODPHY_SGMII0_2_5G_CLIENT_ID) || (modphyClientId == MODPHY_SGMII1_2_5G_CLIENT_ID))
    {
        bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_SGMII_LANE_WIDTH, I_LANE_WIDTH, 2, modphyClientArray[modphyClientId].isBigEndian); /*Configure lane width to 20 bits*/
    } else
    {
        bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_LANE_WIDTH, I_LANE_WIDTH, 2, modphyClientArray[modphyClientId].isBigEndian); /*Configure lane width to 20 bits*/
    }

    DPRINTK("\nmodphy_lane_configure clear rx_status bit O_LANE_PIPEPHYSTATUS_SUS, lane:%d \n", lane);
    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian); /*Clears the Status bit.*/

    mdelay(100);

    DPRINTK("\nmodphy_lane_configure setting rx_control bit I_LANE_RESET_L, lane:%d \n", lane);
    SET_REGISTER(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONTROL_L0_OFFSET + (lane * 0x4), I_LANE_RESET_L, modphyClientArray[modphyClientId].isBigEndian); /*Takes the data lane out of reset.*/
    DPRINTK("\nmodphy_lane_configure reading rx_status bit O_LANE_PIPEPHYSTATUS_SUS for each lane\n");

    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Verify that the pipephystatus signal goes from high to low*/
    {
        modphyReadFail = MODPHY_FAIL;
        printk(KERN_ERR "\nERROR modphy_lane_configure reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
    }

    if (MODPHY_FAIL == modphyReadFail)
    {
        return MODPHY_FAIL; /*one of the values in the loop was wrong*/
    }

    /* Require delay between setting Data lane power mode to PS0 and performing Data Lane reset */
    mdelay(100);

    DPRINTK("\nmodphy_lane_configure clear rx_status bit O_LANE_PIPEPHYSTATUS_SUS, lane:%d\n", lane);
    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian); /*Clears the Status bit.*/

    DPRINTK("\nmodphy_lane_configure setting rx_config bit I_LANE_POWERDOWN_SUS[0-2], lane:%d\n", lane);
    bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_POWER_MODE_P0, I_LANE_POWERDOWN_SUS0, 3, modphyClientArray[modphyClientId].isBigEndian); /*P0s - Tx off operation*/

    DPRINTK("\nmodphy_lane_configure reading rx_status bit O_LANE_PIPEPHYSTATUS_SUS for each lane\n");

    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Verify that the pipephystatus signal goes from high to low*/
    {
        modphyReadFail = MODPHY_FAIL;
        printk(KERN_ERR "\nmodphy_lane_configure reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
    }

    if (MODPHY_FAIL == modphyReadFail)
    {
        return MODPHY_FAIL; /*one of the values in the loop was wrong*/
    }

    /* Data Lane for the specific modphy is up at this point*/
    modphyClientStatusData[modphyClientId].dataLanePowerState[lane] = MODPHY_KG_POWER_STATE_0;

    return MODPHY_OK;

}
EXPORT_SYMBOL(modphy_lane_configure);

int modphy_common_lane_bringdown( ModphyController_e modphyClientId )
{
    DPRINTK("\nmodphy_common_lane_bringdown Full Common Lane Power gate for Client %d - Not Implemented(only PLL is powered UP/DOWN)\n", modphyClientId);

    /* Set: (keep it by now)  i_com0_pll1en_h=0 */
    SET_DISABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_PLL1EN_H, modphyClientArray[modphyClientId].isBigEndian); /* disable PLL #1 within the common lane*/

    /* Read/Ensure o_com0_pll1ok_h = 0 */
    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_PLL1OK_H_MASK, 0, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
    {
        DPRINTK("\nERROR modphy_common_lane_bringdown reading bit O_LANE_PIPEPHYSTATUS_SUS on common lane - WRONG VALUE (should be '1')\n");
        return MODPHY_FAIL;
    }
    DPRINTK("\nmodphy_common_lane_bringdown Client = %d, common Lane PLL DISABLED \n", modphyClientId);

    DPRINTK("\nmodphy_common_lane_bringdown Client = %d, common Lane RESET \n", modphyClientId);
    SET_DISABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_CMNRESET_L, modphyClientArray[modphyClientId].isBigEndian); /*takes the common lane out of reset. assert bit 0*/

    /* Send mbox message to PMIC for Common Lane PG Entry */
    /* TODO: Add code here */

    /* Read/Ensure o_com0_iosf_side_clkreq to be 0 */
    /* Read/Ensure i_com0_iosf_side_clkack to be 0 */
    /* Read/Ensure i_com0_ck_iosf_sideclk to be 0 (This one is part of the handshaking, it is not part of the SW but can be monitored) */
    /* Read/Ensure o_ com0_pll1ok_h =0 */
    /* Set i_com0_soc2phy_pwrreq_h_sus=0 (because it is tied off to 0 already then it is ready for SERDES) */

    modphyClientStatusData[modphyClientId].commonLanePowerState = MODPHY_KG_POWER_STATE_0_PLL_OFF;
    return MODPHY_OK;
}

int modphy_common_lane_bringup( ModphyController_e modphyClientId )
{
    DPRINTK( "\nmodphy_common_lane_bringup Client %d, common lane : Bringing back from reset\n", modphyClientId );
    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_CMNRESET_L, modphyClientArray[modphyClientId].isBigEndian); /*takes the common lane out of reset. assert bit 0*/
    /* Send mbox message to PMIC for Common Lane PG Exit */
    /* TODO: Add code here */

    /* Commonlane Power Gating Exit -> Controller has to define when it is needed to exit */
    /* Set i_com0_soc2phy_pwrreq_h_sus = 1 */
    /* i_com0_ck_iosf_sideclk = toggling */
    /* Set: (keep it by now)  i_com0_pll1en_h = 1 */

    DPRINTK( "\nmodphy_common_lane_bringup Client %d, common lane : Enabling PLL\n", modphyClientId );
    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_PLL1EN_H, modphyClientArray[modphyClientId].isBigEndian); /*enable PLL #1 within the common lane*/

    /* Read/Ensure o_com0_pll1ok_h = 1 */
    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_PLL1OK_H_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
    {
        DPRINTK("\nERROR modphy_common_lane_bringup reading bit O_COM0_PLL1OK_H_MASK - WRONG VALUE (should be '1')\n");
        return MODPHY_FAIL;
    }

    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_PLL1CORECLKB_REQ_H, modphyClientArray[modphyClientId].isBigEndian); /*Request to turn on the clock signal which is a divided clock from the PLL*/
    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_PLL1CORECLKB_ACK_H, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Read '1' - PLL OK signal arrived from modPHY*/
    {
        printk(KERN_ERR "\nmodphy_common_lane_bringup reading bit O_COM0_PLL1CORECLKB_ACK_H - WRONG VALUE (should be '1')\n");
        return MODPHY_FAIL;
    }

    SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET, I_COM0_PLL1CORECLKA_REQ_H, modphyClientArray[modphyClientId].isBigEndian); /*Request to turn on the clock signal which is a divided clock from the PLL*/

    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_COM0_PLL1CORECLKA_ACK_H, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Read '1'  - PLL OK signal arrived from modPHY*/
    {
        printk(KERN_ERR "\nmodphy_common_lane_bringup reading bit O_COM0_PLL1CORECLKA_ACK_H - WRONG VALUE (should be '1')\n");
        return MODPHY_FAIL;
    }
    modphyClientStatusData[modphyClientId].commonLanePowerState = MODPHY_KG_POWER_STATE_0;

    /* Add minimum 100 us delay before moving to PS2 */
    mdelay(10);

    DPRINTK("\nmodphy_common_lane_bringup Client = %d, common lane(PLL) ENABLED \n", modphyClientId);

    return MODPHY_OK;
}

/*********************************************************************************************************/
/*! \fn int modphy_lane_power_state_transition(ModphyLanes_e lane, ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief  This function can cause a specific lane to enter or exit from different low power modes 
 *  \param[in] lane : Lane for the Client ID
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_lane_power_state_transition(ModphyLanes_e lane, ModphyController_e modphyClientId, ModphyPowerState_e powerStateReq)
{
    int modphyReadFail = MODPHY_OK;
    Uint8 powerState;
    Bool alldataLanesinPowerGate = true;
    int i;

    if (modphyIsInitArrayDone != True)
    {
        DPRINTK("\n Modphy driver need to be initialized before trying to init modphy %d \n"
                "(0=MOCA, 1=LEGACY DOCSIS, 2=OFDM DOCSIS, 3=SGMII, 4=SGMII2)\n", modphyClientId);
        return MODPHY_FAIL;
    }

    /* Validate parameters to powergate function */
    if( (lane != MODPHY_COMMON_LANE) && (modphyClientId < MODPHY_NUM_OF_CLIENTS) && (lane < modphyClientStatusData[modphyClientId].maxLanes) ) 
    {
        if ( (powerStateReq == MODPHY_POWER_STATE_ACTIVE) && (modphyClientStatusData[modphyClientId].dataLanePowerState[lane] == MODPHY_POWER_STATE_ACTIVE) ) {
            DPRINTK("\nInvalid Power transition Requested : Already in Active state. Lane %d, Client %d\n", lane, modphyClientId);
            modphyReadFail = MODPHY_FAIL;
        } else if ( (powerStateReq == MODPHY_POWER_STATE_POWERGATE) && ( (modphyClientStatusData[modphyClientId].dataLanePowerState[lane] == modphyClientStatusData[modphyClientId].powerGateState) || (modphyClientStatusData[modphyClientId].dataLanePowerState[lane] == MODPHY_KG_POWER_STATE_0_RXSTANDBY) ) ) {
            DPRINTK("\nInvalid Power transition Requested : Already in Power gated state OR Rx Standby State. Lane %d, Client %d\n", lane, modphyClientId);
            modphyReadFail = MODPHY_FAIL;
        } else if ( (powerStateReq == MODPHY_POWER_STATE_RXSTANDBY) && ( (modphyClientStatusData[modphyClientId].dataLanePowerState[lane] == MODPHY_KG_POWER_STATE_0_RXSTANDBY) || (modphyClientStatusData[modphyClientId].dataLanePowerState[lane] == modphyClientStatusData[modphyClientId].powerGateState) ) ) {
            DPRINTK("\nInvalid Power transition Requested : Already in Rx Standby state OR Power gated State. Lane %d, Client %d\n", lane, modphyClientId);
            modphyReadFail = MODPHY_FAIL;
        }
    } 
    else if (lane == MODPHY_COMMON_LANE)
    {
        if ((powerStateReq == MODPHY_POWER_STATE_ACTIVE) && (modphyClientStatusData[modphyClientId].commonLanePowerState == MODPHY_POWER_STATE_ACTIVE)) 
        {
            DPRINTK("\nInvalid Power transition Requested : Already in Active state. common lane of Client %d\n", modphyClientId);
            modphyReadFail = MODPHY_FAIL;
        } else if ( (powerStateReq == MODPHY_POWER_STATE_COMMON_LANE) && (modphyClientStatusData[modphyClientId].commonLanePowerState == MODPHY_POWER_STATE_COMMON_LANE) ) 
        {
            DPRINTK("\nInvalid Power transition Requested : Already in power gate common lane(PLL) state Client %d\n", modphyClientId);
            modphyReadFail = MODPHY_FAIL;
        }
    }
    else
    {
        DPRINTK("\nInvalid Parameters to powergate function: Lane %d, Client %d\n", lane, modphyClientId);
        modphyReadFail = MODPHY_FAIL;
    }

    DPRINTK("\nEntering power gate function : lane = %d, clientId = %d, powerstateReq = %d\n", (unsigned int)lane, (unsigned int)modphyClientId, (unsigned int)powerStateReq);

    if (modphyReadFail == MODPHY_FAIL) {
        return MODPHY_FAIL;
    }

    switch(powerStateReq) 
    {
        case MODPHY_POWER_STATE_ACTIVE : 
            {
                /**** Exit common lane power gate state****/
                if (modphyClientStatusData[modphyClientId].commonLanePowerState == MODPHY_KG_POWER_STATE_0_PLL_OFF) 
                {
                    if (lane != MODPHY_COMMON_LANE) 
                    {
                        modphyReadFail = MODPHY_FAIL;
                        DPRINTK("\nERROR modphy_lane_power_state_transition can't power on client %d lane:%d,  becouse common lane(PLL) is off!\n", modphyClientId, lane);
                        return MODPHY_FAIL;
                    }
                    if (modphyClientStatusData[modphyClientId].powerGateAccess.clanePowerGateAllowed) 
                    {
                        return modphy_common_lane_bringup( modphyClientId );
                    }
                }

                /**** Exit from data lane rx standby state if so ****/
                if ( modphyClientStatusData[modphyClientId].dataLanePowerState[lane] == MODPHY_KG_POWER_STATE_0_RXSTANDBY )
                {
                    /* Read/Ensure current state is PS0. i_lane0_powerdown_sus (or lane 1) == 0 */
                    if ((powerState = IS_ENABLE_BIT_SET((modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), I_LANE_POWERDOWN_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian)) != 0)
                    {
                        modphyReadFail = MODPHY_FAIL;
                        DPRINTK("\nERROR modphy_lane_power_state_transition power state for rx standby should be PS0 instead it is PS%d! (lane:%d, client %d)\n", powerState, lane, modphyClientId);
                        return MODPHY_FAIL;
                    }
                    /* Check that rxstandbyStatus == 1. (RX active) - o_lane0_standbystatus( or lane 1) == 1 */
                    if (IS_ENABLE_BIT_SET((modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4)), O_LANE_RXSTANDBYSTATUS_H_MASK, modphyClientArray[modphyClientId].isBigEndian) == 0)
                    {
                        modphyReadFail = MODPHY_FAIL;
                        DPRINTK("\nERROR modphy_lane_power_state_transition rx standby for lane:%d, client %d is allready disabled(Rx is on)!\n", lane, modphyClientId);
                        return MODPHY_FAIL;
                    }
                    /* Set  i_rxstandby = 0 */
                    SET_DISABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4), I_LANE_RXSTANDBY_H_MASK, modphyClientArray[modphyClientId].isBigEndian);

                    /* Check that rxstandbyStatus == 0 (RX off) - o_lane0_standbystatus( or lane 1) == 0 */
                    if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_RXSTANDBYSTATUS_H_MASK, 0, 10, modphyClientId)) == MODPHY_FAIL) 
                    {
                        modphyReadFail = MODPHY_FAIL;
                        DPRINTK("\nERROR modphy_lane_power_state_transition rx standby for lane:%d, client %d was not disabled (Rx is still off)!\n", lane);
                        return MODPHY_FAIL;
                    }

                    modphyClientStatusData[modphyClientId].dataLanePowerState[lane] = MODPHY_KG_POWER_STATE_0;

                    DPRINTK("\nClient = %d, Lane = %d POWERSTATE = 0x%08x \n", modphyClientId, lane, modphyClientStatusData[modphyClientId].dataLanePowerState[lane]);

                    return MODPHY_OK;
                }

                /**** Exit from data lane power gate state, if so ****/

                SET_REGISTER(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONTROL_L0_OFFSET + (lane * 0x4), 1, modphyClientArray[modphyClientId].isBigEndian); /*Takes the data lane out of reset.*/

                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 0     , MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Verify that the pipephystatus signal goes from high to low*/
                {
                    modphyReadFail = MODPHY_FAIL;
                    printk(KERN_ERR "\nERROR modphy_lane_configure reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                }
                SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian); /*Clears the Status bit.*/

                /* Check if Power gating is allowed by design for this client and this lane */
                if ( !(modphyClientStatusData[modphyClientId].powerGateAccess.dlanePowerGateAllowed[lane]) )
                {
                    DPRINTK("\nERROR modphy_lane_power_state_transition power gate not allowed for lane:%d, client %d\n", lane, modphyClientId);
                    return MODPHY_FAIL;
                }

                /* Set i_powerdown_sus[2:0]=0x2 */
                bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_KG_POWER_STATE_2, I_LANE_POWERDOWN_SUS0, 3, modphyClientArray[modphyClientId].isBigEndian); /*P0s - Tx off operation*/

                /* Read/Ensure o_phystatus = 1 */
                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                    return MODPHY_FAIL;
                }

                /* Wait 1 pclk  100 us should more than suffice. */
                //udelay(100);
                SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian);

                /* Read/Ensure o_phystatus = 0 */
                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 0, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                    return MODPHY_FAIL;
                }

                /* Set: i_lane0_txelecidle_h_sus = 0 */
                SET_DISABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_TX_CONFIG_L0_OFFSET + (lane * 0x4), I_LANE_TXELECIDLE_MASK, modphyClientArray[modphyClientId].isBigEndian); 

                /* Read/Ensure o_lane0_rxelecidle_h_sus=0 . Instead wait 10 ms */
                mdelay(10); 

                /* Set i_powerdown_sus[2:0] = 0x0 */
                bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), MODPHY_KG_POWER_STATE_0, I_LANE_POWERDOWN_SUS0, 3, modphyClientArray[modphyClientId].isBigEndian); /*P0s - Tx off operation*/

                /* Read/Ensure o_phystatus = 1 */
                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                    return MODPHY_FAIL;
                }

                /* Wait 1 pclk  100 us should more than suffice. */
                //udelay(100);
                SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian);

                /* Read/Ensure o_phystatus = 0 */
                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 0, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                    return MODPHY_FAIL;
                }

                modphyClientStatusData[modphyClientId].dataLanePowerState[lane] = MODPHY_KG_POWER_STATE_0;

                DPRINTK("\nClient = %d, Lane = %d POWERSTATE = 0x%08x \n", modphyClientId, lane, modphyClientStatusData[modphyClientId].dataLanePowerState[lane]);
            }
            break;

        case MODPHY_POWER_STATE_POWERGATE : 
            {

                /* Check if Power gating is allowed by design for this client and this lane */
                if ( !(modphyClientStatusData[modphyClientId].powerGateAccess.dlanePowerGateAllowed[lane]) )
                {
                    DPRINTK("\nERROR modphy_lane_power_state_transition power gate not allowed for lane:%d, client %d\n", lane, modphyClientId);
                    return MODPHY_FAIL;
                }

                /**** Entry into data lane power gate state ****/

                /* Read/Ensure o_phystatus = 0 */
                SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian);

                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 0, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '0')\n", lane);
                    return MODPHY_FAIL;
                }

                /* Set: i_lane0_txelecidle_h_sus=1 */
                SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_TX_CONFIG_L0_OFFSET + (lane * 0x4), I_LANE_TXELECIDLE_MASK, modphyClientArray[modphyClientId].isBigEndian); 

                /* Read/Ensure o_lane0_rxelecidle_h_sus=1 . Instead wait 10 ms */
                mdelay(10); 

                /* Set i_powerdown_sus[2:0]=0x3  Can be 0x7 for SGMII */
                bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), modphyClientStatusData[modphyClientId].powerGateState, I_LANE_POWERDOWN_SUS0, 3, modphyClientArray[modphyClientId].isBigEndian); /*P0s - Tx off operation*/

                /* Read/Ensure o_phystatus = 1 */
                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 1, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                    return MODPHY_FAIL;
                }

                /* Wait 1 pclk  100 us should more than suffice. */
                //udelay(100);
                SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian);

                /* Read/Ensure o_phystatus = 0 */
                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 0, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                    return MODPHY_FAIL;
                }

                /* Set i_lane0_soc2phy_pwrreq_h_sus=0 */
                bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_CONFIG_OFFSET), 0, I_LANE_SOC2PHY_PWRREQ_H_SUS(lane), 1, modphyClientArray[modphyClientId].isBigEndian); /*P0s - Tx off operation*/

                /* Read/Ensure o_lane_phy2soc_pwrack_h_sus = 0 */
                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_COMMON_STATUS_OFFSET, O_LANE_PHY2SOC_PWRACK_H_SUS_MASK(lane), 0, MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                    return MODPHY_FAIL;
                }

                /* Puts the DLANE back in reset.*/
                SET_REGISTER(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONTROL_L0_OFFSET + (lane * 0x4), 0, modphyClientArray[modphyClientId].isBigEndian); 

                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, 0     , MODPHY_READ_BIT_TIMEOUT_USEC, modphyClientId)) == MODPHY_FAIL) /*Verify that the pipephystatus signal goes from high to low*/
                {
                    modphyReadFail = MODPHY_FAIL;
                    printk(KERN_ERR "\nERROR modphy_lane_configure reading bit O_LANE_PIPEPHYSTATUS_SUS on lane:%d - WRONG VALUE (should be '1')\n", lane);
                }
                SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_PIPEPHYSTATUS_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian);
                modphyClientStatusData[modphyClientId].dataLanePowerState[lane] = modphyClientStatusData[modphyClientId].powerGateState;

                DPRINTK("\nClient = %d, Lane = %d POWERSTATE = 0x%08x \n", modphyClientId, lane, modphyClientStatusData[modphyClientId].dataLanePowerState[lane]);
            }
            break;

        case MODPHY_POWER_STATE_RXSTANDBY:
            {
                /* Check if Rx standby is allowed by design for this client and this lane */
                if ( !(modphyClientStatusData[modphyClientId].powerGateAccess.rxStandbyAllowed) )
                {
                    DPRINTK("\nERROR modphy_lane_power_state_transition rx standby not allowed for lane:%d, client %d\n", lane, modphyClientId);
                    return MODPHY_FAIL;
                }

                /* Set PHY MODE = SERDES - i_lane0_phymode_sus = 0 */
                //bit_field32_set((Uint32 *)(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET), 4, I_LANE_PHYMODE_SUS, 3, modphyClientArray[modphyClientId].isBigEndian);

                /* Read/Ensure current state is PS0. i_lane0_powerdown_sus (or lane 1) == 0 */
                if ((powerState = IS_ENABLE_BIT_SET((modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4)), I_LANE_POWERDOWN_SUS_MASK, modphyClientArray[modphyClientId].isBigEndian)) != 0)
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition power state for rx standby should be PS0 instead it is PS%d! (lane:%d, client %d)\n", powerState, lane, modphyClientId);
                    return MODPHY_FAIL;
                }
                /* Check that rxstandbyStatus == 0. (RX active) - o_lane0_standbystatus( or lane 1) == 0 */
                if (IS_ENABLE_BIT_SET((modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4)), O_LANE_RXSTANDBYSTATUS_H_MASK, modphyClientArray[modphyClientId].isBigEndian) != 0)
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition rx standby for lane:%d, client %d is allready active(Rx is off)!\n", lane, modphyClientId);
                    return MODPHY_FAIL;
                }
                /* Set  i_rxstandby = 1 */
                SET_ENABLE_BIT(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_CONFIG_L0_OFFSET + (lane * 0x4), I_LANE_RXSTANDBY_H_MASK, modphyClientArray[modphyClientId].isBigEndian);
                /* Wait 1-2 us : usleep(2) */
                /* Check that rxstandbyStatus == 1 (RX off) - o_lane0_standbystatus( or lane 1) == 1 */
                if ((modphy_read_wait_bit(modphyClientArray[modphyClientId].baseAddr + MODPHY_RX_STATUS_L0_OFFSET + (lane * 0x4), O_LANE_RXSTANDBYSTATUS_H_MASK, 1, 10, modphyClientId)) == MODPHY_FAIL) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition rx standby for lane:%d, client %d was not activated (Rx is still on)!\n", lane, modphyClientId);
                    return MODPHY_FAIL;
                }

                modphyClientStatusData[modphyClientId].dataLanePowerState[lane] = MODPHY_KG_POWER_STATE_0_RXSTANDBY;

                DPRINTK("\nClient = %d, Lane = %d POWERSTATE = 0x%08x \n", modphyClientId, lane, modphyClientStatusData[modphyClientId].dataLanePowerState[lane]);
            }
            break;

        case MODPHY_POWER_STATE_COMMON_LANE:
            {
                /**** Enter common lane power gate state if all data lanes for a modphy client is power gated ****/
                for ( i = 0; i < modphyClientStatusData[modphyClientId].maxLanes; i++) 
                {
                    alldataLanesinPowerGate &= (modphyClientStatusData[modphyClientId].dataLanePowerState[i] == modphyClientStatusData[modphyClientId].powerGateState);
                    if (!alldataLanesinPowerGate)
                    {
                        break;
                    }
                }
                if (!alldataLanesinPowerGate) 
                {
                    modphyReadFail = MODPHY_FAIL;
                    DPRINTK("\nERROR modphy_lane_power_state_transition common lane power gate for client %d can't be performed becouse lane:%d,  is still powered on!\n", modphyClientId, i);
                    return MODPHY_FAIL;
                }

                if (modphyClientStatusData[modphyClientId].powerGateAccess.clanePowerGateAllowed) 
                {
                    modphyReadFail = modphy_common_lane_bringdown( modphyClientId );
                }
            }
            break;

        default :
            {
                DPRINTK("\n Wrong transition requested for modphy_lane_power_state_transition for Client = %d, Lane = %d\n", modphyClientId, lane);
                modphyReadFail = MODPHY_FAIL;
            }
            break;
    }

    return modphyReadFail;
}
EXPORT_SYMBOL(modphy_lane_power_state_transition);

/*********************************************************************************************************/
/*! \fn int modphy_proc_dump_regs(char* buf, char **start, off_t offset, int count, int *eof, void *data)
 *********************************************************************************************************
 *  \brief  This function inits the modphy for a specific client.
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
int modphy_proc_dump_regs(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    int len = 0;

    if (offset != 0)
    {
        return 0;
    }

    printk("modphy_proc_dump_regs :: \n");
    /*MOCA*/
#if 0
    len += sprintf(buf + len, "MODPHY_MOCA_CLIENT_ID:\n");
    len += sprintf(buf + len, "MODPHY_COMMON_CONFIG_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_COMMON_CONFIG_OFFSET)));

    len += sprintf(buf + len, "MODPHY_COMMON_STATUS_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_COMMON_STATUS_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_RX_CONFIG_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_RX_CONFIG_L1_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_RX_CONTROL_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_RX_CONTROL_L1_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_STATUS_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_RX_STATUS_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_STATUS_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_RX_STATUS_L1_OFFSET)));

    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_TX_CONFIG_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_MOCA_BASE + MODPHY_TX_CONFIG_L1_OFFSET)));
#endif
    /*DOCSIS LEGACY*/
    len += sprintf(buf + len, "\nMODPHY_DOCSIS_LEGACY_CLIENT_ID:\n");
    len += sprintf(buf + len, "MODPHY_COMMON_CONFIG_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_COMMON_CONFIG_OFFSET)));
    len += sprintf(buf + len, "MODPHY_COMMON_STATUS_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_COMMON_STATUS_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_RX_CONFIG_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_RX_CONFIG_L1_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_RX_CONTROL_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_RX_CONTROL_L1_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_STATUS_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_RX_STATUS_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_STATUS_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_RX_STATUS_L1_OFFSET)));

    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_TX_CONFIG_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE + MODPHY_TX_CONFIG_L1_OFFSET)));

    /*DOCSIS OFDM*/
    len += sprintf(buf + len, "\nMODPHY_DOCSIS_OFDM_CLIENT_ID:\n");
    len += sprintf(buf + len, "MODPHY_COMMON_CONFIG_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_COMMON_CONFIG_OFFSET)));
    len += sprintf(buf + len, "MODPHY_COMMON_STATUS_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_COMMON_STATUS_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_CONFIG_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_CONFIG_L1_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L2_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_CONFIG_L2_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_CONTROL_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_CONTROL_L1_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L2_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_CONTROL_L2_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_STATUS_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_STATUS_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_STATUS_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_STATUS_L1_OFFSET)));
    len += sprintf(buf + len, "MODPHY_RX_STATUS_L2_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_RX_STATUS_L2_OFFSET)));

    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_TX_CONFIG_L0_OFFSET)));
    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L1_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_TX_CONFIG_L1_OFFSET)));
    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L2_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_DOCSIS_OFDM_BASE + MODPHY_TX_CONFIG_L2_OFFSET)));
#if 0
    /*SGMII*/
    len += sprintf(buf + len, "\nMODPHY_SGMII_CLIENT_ID:\n");
    len += sprintf(buf + len, "MODPHY_COMMON_CONFIG_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII_BASE + MODPHY_COMMON_CONFIG_OFFSET)));
    len += sprintf(buf + len, "MODPHY_COMMON_STATUS_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII_BASE + MODPHY_COMMON_STATUS_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII_BASE + MODPHY_RX_CONFIG_L0_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII_BASE + MODPHY_RX_CONTROL_L0_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_STATUS_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII_BASE + MODPHY_RX_STATUS_L0_OFFSET)));

    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII_BASE + MODPHY_TX_CONFIG_L0_OFFSET)));

    /*SGMII2*/
    len += sprintf(buf + len, "\nMODPHY_SGMII2_CLIENT_ID:\n");
    len += sprintf(buf + len, "MODPHY_COMMON_CONFIG_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII2_BASE + MODPHY_COMMON_CONFIG_OFFSET)));
    len += sprintf(buf + len, "MODPHY_COMMON_STATUS_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII2_BASE + MODPHY_COMMON_STATUS_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII2_BASE + MODPHY_RX_CONFIG_L0_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_CONTROL_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII2_BASE + MODPHY_RX_CONTROL_L0_OFFSET)));

    len += sprintf(buf + len, "MODPHY_RX_STATUS_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII2_BASE + MODPHY_RX_STATUS_L0_OFFSET)));

    len += sprintf(buf + len, "MODPHY_TX_CONFIG_L0_OFFSET = 0x%x\n", *((volatile unsigned int *)(AVALANCHE_MODPHY_SGMII2_BASE + MODPHY_TX_CONFIG_L0_OFFSET)));
#endif

    *eof = 1;
    return len;
}


/*********************************************************************************************************/
/*! \fn void modphy_init_client_array(void)
 *********************************************************************************************************
 *  \brief  This function inits the modphy for a specific client.
 *  \param[in] modphyClientId : Client ID
 *  \return MODPHY_OK if writing succeed otherwise MODPHY_FAIL 
 ********************************************************************************************************/
void modphy_init_client_array(void)
{
    modphyClientArray[MODPHY_MOCA_CLIENT_ID].iosfPort                = MODPHY_MOCA_SB_PORT;
    modphyClientArray[MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID].iosfPort    = MODPHY_DOCSIS_LEGACY_SB_PORT;
    modphyClientArray[MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID].iosfPort   = MODPHY_DOCSIS_OFDM_SB_PORT;
    modphyClientArray[MODPHY_SGMII0_1G_CLIENT_ID].iosfPort           = MODPHY_SGMII_1_SB_PORT;
    modphyClientArray[MODPHY_SGMII1_1G_CLIENT_ID].iosfPort           = MODPHY_SGMII_2_SB_PORT;
    modphyClientArray[MODPHY_SGMII0_2_5G_CLIENT_ID].iosfPort         = MODPHY_SGMII_1_SB_PORT;
    modphyClientArray[MODPHY_SGMII1_2_5G_CLIENT_ID].iosfPort         = MODPHY_SGMII_2_SB_PORT;
    modphyClientArray[MODPHY_MOCA_50MHZ_CLIENT_ID].iosfPort          = MODPHY_MOCA_SB_PORT;
    modphyClientArray[MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID].iosfPort    = MODPHY_DOCSIS_LEGACY_SB_PORT;
    modphyClientArray[MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID].iosfPort   = MODPHY_DOCSIS_OFDM_SB_PORT;

    modphyClientArray[MODPHY_MOCA_CLIENT_ID].modPhyType                = MODPHY_TYPE_3_DOCSIS_P7_25MHz;
    modphyClientArray[MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID].modPhyType    = MODPHY_TYPE_3_DOCSIS_P7_25MHz;
    modphyClientArray[MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID].modPhyType   = MODPHY_TYPE_3_DOCSIS_P7_25MHz;
    modphyClientArray[MODPHY_SGMII0_1G_CLIENT_ID].modPhyType           = MODPHY_TYPE_6_SGMII_1GB;
    modphyClientArray[MODPHY_SGMII1_1G_CLIENT_ID].modPhyType           = MODPHY_TYPE_6_SGMII_1GB;
    modphyClientArray[MODPHY_SGMII0_2_5G_CLIENT_ID].modPhyType         = MODPHY_TYPE_1_SGMII_2_5GB;
    modphyClientArray[MODPHY_SGMII1_2_5G_CLIENT_ID].modPhyType         = MODPHY_TYPE_1_SGMII_2_5GB;
    modphyClientArray[MODPHY_MOCA_50MHZ_CLIENT_ID].modPhyType          = MODPHY_TYPE_7_DOCSIS_P7_50MHz;
    modphyClientArray[MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID].modPhyType    = MODPHY_TYPE_7_DOCSIS_P7_50MHz;
    modphyClientArray[MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID].modPhyType   = MODPHY_TYPE_7_DOCSIS_P7_50MHz;

    modphyClientArray[MODPHY_MOCA_CLIENT_ID].baseAddr                = AVALANCHE_MODPHY_MOCA_BASE;
    modphyClientArray[MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID].baseAddr    = AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE;
    modphyClientArray[MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID].baseAddr   = AVALANCHE_MODPHY_DOCSIS_OFDM_BASE;
    modphyClientArray[MODPHY_SGMII0_1G_CLIENT_ID].baseAddr           = AVALANCHE_MODPHY_SGMII_BASE;
    modphyClientArray[MODPHY_SGMII1_1G_CLIENT_ID].baseAddr           = AVALANCHE_MODPHY_SGMII2_BASE;
    modphyClientArray[MODPHY_SGMII0_2_5G_CLIENT_ID].baseAddr         = AVALANCHE_MODPHY_SGMII_BASE;
    modphyClientArray[MODPHY_SGMII1_2_5G_CLIENT_ID].baseAddr         = AVALANCHE_MODPHY_SGMII2_BASE;
    modphyClientArray[MODPHY_MOCA_50MHZ_CLIENT_ID].baseAddr          = AVALANCHE_MODPHY_MOCA_BASE;
    modphyClientArray[MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID].baseAddr    = AVALANCHE_MODPHY_DOCSIS_LEGACY_BASE;
    modphyClientArray[MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID].baseAddr   = AVALANCHE_MODPHY_DOCSIS_OFDM_BASE;

    modphyClientArray[MODPHY_MOCA_CLIENT_ID].numOfLanes                = MODPHY_MOCA_NUM_OF_LANES;
    modphyClientArray[MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID].numOfLanes    = MODPHY_DOCSIS_LEGACY_NUM_OF_LANES;
    modphyClientArray[MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID].numOfLanes   = MODPHY_DOCSIS_OFDM_NUM_OF_LANES;
    modphyClientArray[MODPHY_SGMII0_1G_CLIENT_ID].numOfLanes           = MODPHY_SGMII_NUM_OF_LANES;
    modphyClientArray[MODPHY_SGMII1_1G_CLIENT_ID].numOfLanes           = MODPHY_SGMII_NUM_OF_LANES;
    modphyClientArray[MODPHY_SGMII0_2_5G_CLIENT_ID].numOfLanes         = MODPHY_SGMII_NUM_OF_LANES;
    modphyClientArray[MODPHY_SGMII1_2_5G_CLIENT_ID].numOfLanes         = MODPHY_SGMII_NUM_OF_LANES;
    modphyClientArray[MODPHY_MOCA_50MHZ_CLIENT_ID].numOfLanes          = MODPHY_MOCA_NUM_OF_LANES;
    modphyClientArray[MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID].numOfLanes    = MODPHY_DOCSIS_LEGACY_NUM_OF_LANES;
    modphyClientArray[MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID].numOfLanes   = MODPHY_DOCSIS_OFDM_NUM_OF_LANES;

    modphyClientArray[MODPHY_MOCA_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x200;
    modphyClientArray[MODPHY_MOCA_CLIENT_ID].laneId[MODPHY_LANE_1] = 0x0;
    modphyClientArray[MODPHY_MOCA_50MHZ_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x200;
    modphyClientArray[MODPHY_MOCA_50MHZ_CLIENT_ID].laneId[MODPHY_LANE_1] = 0x0;

    modphyClientArray[MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x2000;
    modphyClientArray[MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID].laneId[MODPHY_LANE_1] = 0x0;
    modphyClientArray[MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x2000;
    modphyClientArray[MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID].laneId[MODPHY_LANE_1] = 0x0;

    modphyClientArray[MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x200;
    modphyClientArray[MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID].laneId[MODPHY_LANE_1] = 0x300;
    modphyClientArray[MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID].laneId[MODPHY_LANE_2] = 0x0;
    modphyClientArray[MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x200;
    modphyClientArray[MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID].laneId[MODPHY_LANE_1] = 0x300;
    modphyClientArray[MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID].laneId[MODPHY_LANE_2] = 0x0;

    modphyClientArray[MODPHY_SGMII0_1G_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x200;
    modphyClientArray[MODPHY_SGMII1_1G_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x200;
    modphyClientArray[MODPHY_SGMII0_2_5G_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x200;
    modphyClientArray[MODPHY_SGMII1_2_5G_CLIENT_ID].laneId[MODPHY_LANE_0] = 0x200;

    modphyClientArray[MODPHY_MOCA_CLIENT_ID].isBigEndian                   = False;
    modphyClientArray[MODPHY_DOCSIS_QAM_25MHZ_CLIENT_ID].isBigEndian       = True;
    modphyClientArray[MODPHY_DOCSIS_OFDM_25MHZ_CLIENT_ID].isBigEndian      = True;
    modphyClientArray[MODPHY_SGMII0_1G_CLIENT_ID].isBigEndian              = True;
    modphyClientArray[MODPHY_SGMII1_1G_CLIENT_ID].isBigEndian              = True;
    modphyClientArray[MODPHY_SGMII0_2_5G_CLIENT_ID].isBigEndian            = True;
    modphyClientArray[MODPHY_SGMII1_2_5G_CLIENT_ID].isBigEndian            = True;
    modphyClientArray[MODPHY_MOCA_50MHZ_CLIENT_ID].isBigEndian             = False;
    modphyClientArray[MODPHY_DOCSIS_QAM_50MHZ_CLIENT_ID].isBigEndian       = True;
    modphyClientArray[MODPHY_DOCSIS_OFDM_50MHZ_CLIENT_ID].isBigEndian      = True;

    modphyIsInitArrayDone = True;
}

/*********************************************************************************************************/
/*! \fn int modphy_read_wait_bit(unsigned int addr, int bit_mask, int expected_val, int timeout_usec, ModphyController_e modphyClientId)
 *********************************************************************************************************
 *  \brief This function reads a specific bit, waiting for it to be expected_val
 *  \If the value doesn't match after timeout_usec, the function returns.
 *  \param[in] addr: Register address
 *  \param[in] bit_mask: The required bit (only 1 bit at a time!)
 *  \param[in] expected_val: The expected bit value
 *  \param[in] timeout_usec: Timeout in usec after which the function returns, regardless of the bit's value
 *  \return MODPHY_OK - if the value matched; MODPHY_FAIL - if the value didn't match 
 ********************************************************************************************************/
int modphy_read_wait_bit(unsigned int addr, int bit_mask, int expected_val, int timeout_usec, ModphyController_e modphyClientId)
{
    unsigned int waitingTimeInJiffies = 0;
    waitingTimeInJiffies = jiffies + usecs_to_jiffies(timeout_usec);

    do
    {
        if (0 == expected_val)
        {
            if (IS_ENABLE_BIT_SET(addr, bit_mask, modphyClientArray[modphyClientId].isBigEndian) == 0)
            {
                return MODPHY_OK;
            }
        } else
        {
            if (IS_ENABLE_BIT_SET(addr, bit_mask, modphyClientArray[modphyClientId].isBigEndian) != 0)
            {
                return MODPHY_OK;
            }
        }
        udelay(5);
    }while (time_after(waitingTimeInJiffies, jiffies));

    /*Read again before return failure*/
    if (0 == expected_val)
    {
        if (IS_ENABLE_BIT_SET(addr, bit_mask, modphyClientArray[modphyClientId].isBigEndian) == 0)
        {
            return MODPHY_OK;
        }
    } else
    {
        if (IS_ENABLE_BIT_SET(addr, bit_mask, modphyClientArray[modphyClientId].isBigEndian) != 0)
        {
            return MODPHY_OK;
        }
    }
    return MODPHY_FAIL;
}

/**************************************************************************/
/*! \fn static long modphy_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
 **************************************************************************
 *  \brief This function is called from the user space to oprate function in the driver.
 *  \param[in] fd - The character device of the modphy driver.
 *  \param[in] cmd - The wanted command.
 *  \param[in] arg - The arguments from and to the user space.
 *  \return    MODPHY_OK  = ok, MODPHY_FAIL = operation faild.
 *************************************************************************/
static long modphy_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{
    ModphyController_e modphyClientId;
    ModphyClient_t ModphyClient;
    ModphyClientPowerGate_t ModphyClientPowerGate;
    modphyClientKgregs_t usrKgregs;
    modphyKgregs_t * kernKgregs;
    unsigned kgregsBuffSize;

    int ret = MODPHY_OK;

    if (fd == NULL)
    {
        printk(KERN_ERR "\nMODPHY Error value\n");
        return MODPHY_FAIL;
    }
    switch (cmd)
    {
        case  MODPHY_SET_KGREGS:
            {
                if (copy_from_user(&usrKgregs, (void __user *)arg, sizeof(modphyClientKgregs_t)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                kernKgregs = &modphyClientArray[usrKgregs.clientId].usrKgregs;
                if (kernKgregs->avps)
                {
                    kfree(kernKgregs->avps);
                }

                kernKgregs->avpsCnt = usrKgregs.kgRegs.avpsCnt;
                kgregsBuffSize = kernKgregs->avpsCnt * sizeof(*kernKgregs->avps);
                kernKgregs->avps = kmalloc(kgregsBuffSize, GFP_KERNEL);

                if (copy_from_user(kernKgregs->avps,
                            (void __user *) usrKgregs.kgRegs.avps,
                            kgregsBuffSize))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy KGREGS from user data. IOCTL=%d\n", cmd);
                    kfree(kernKgregs->avps);
                    kernKgregs->avps = 0;
                    kernKgregs->avpsCnt = 0;

                    return -EFAULT;
                }

                break;
            }

        case  MODPHY_INIT:
            {

                if (copy_from_user(&modphyClientId, (void __user *)arg, sizeof(modphyClientId)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_init(modphyClientId);
                break;
            }

        case  MODPHY_CFG:
            {
                if (copy_from_user(&ModphyClient, (void __user *)arg, sizeof(ModphyClient)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_lane_configure(ModphyClient.laneId, ModphyClient.clientId);
                break;
            }

        case MODPHY_DISABLE_TX:
            {
                if (copy_from_user(&ModphyClient, (void __user *)arg, sizeof(ModphyClient)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_tx_disable(ModphyClient.laneId, ModphyClient.clientId);
                break;
            }
        case MODPHY_GO2_FAST_RECOVERY:
            {
                if (copy_from_user(&ModphyClient, (void __user *)arg, sizeof(ModphyClient)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_go2_fast_recovery(ModphyClient.laneId, ModphyClient.clientId);
                break;
            }

        case MODPHY_GO2_FULL_FUNCTIONAL:
            {
                if (copy_from_user(&ModphyClient, (void __user *)arg, sizeof(ModphyClient)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_go2_full_functonal(ModphyClient.laneId, ModphyClient.clientId);
                break;
            }

        case  MODPHY_COMMON_LANE_RESET:
            {

                if (copy_from_user(&modphyClientId, (void __user *)arg, sizeof(modphyClientId)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_common_lane_reset(modphyClientId);
                break;
            }

        case  MODPHY_COMMON_LANE_INIT:
            {

                if (copy_from_user(&modphyClientId, (void __user *)arg, sizeof(modphyClientId)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_common_lane_init(modphyClientId);
                break;
            }
        case MODPHY_RESET_K_ALIGN:
            {
                if (copy_from_user(&modphyClientId, (void __user *)arg, sizeof(modphyClientId)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_reset_k_align(modphyClientId);
                break;
            }

        case MODPHY_RESET_LANE:
            {
                if (copy_from_user(&modphyClientId, (void __user *)arg, sizeof(modphyClientId)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                modphy_reset_client(modphyClientId);
                break;
            }

        case  MODPHY_LANE_POWER_STATE_TRANSITION:
            {
                if (copy_from_user(&ModphyClientPowerGate, (void __user *)arg, sizeof(ModphyClientPowerGate_t)))
                {
                    printk(KERN_ERR"\n MODPHY failed to copy from user data. IOCTL=%d\n", cmd);
                    return -EFAULT;
                }

                ret = modphy_lane_power_state_transition(ModphyClientPowerGate.modphyClient.laneId, 
                        ModphyClientPowerGate.modphyClient.clientId, 
                        ModphyClientPowerGate.powerStateReq );
                break;
            }

        default:
            printk(KERN_ERR "%s:%d Invalid IOCTL(0x%08X) has been received \n", __FUNCTION__, __LINE__, cmd);
            return -ENOTTY;
    }

    return ret;
}

/**************************************************************************/
/*! \fn static int __init modphy_drv_init(void)
 **************************************************************************
 *  \brief     driver init routine 
 *  \param[in] none
 *  \return    MODPHY_OK  = ok, MODPHY_FAIL = operation faild.
 *************************************************************************/
static int __init modphy_drv_init(void)
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "MODPHY: Initializing Intel(R) MODPHY driver\n");

    modphy_init_client_array();

    /*Allocating the modphy_driver */
    if ((ret = alloc_chrdev_region(&dev, 0, 1, MODPHY_NAME)) < 0)
    {
        printk(KERN_ERR "MODPHY: Failed alloc_chrdev_region\n");
        return ret;
    }
    DPRINTK("MODPHY: alloc_chrdev_region - Done\n");

    if (!(c_dev = cdev_alloc()))
    {
        printk(KERN_ERR "%s:%d Failed to allocate character device modphy_driver\n", __FUNCTION__, __LINE__);
        unregister_chrdev_region(dev, MINOR_CNT);
        return (-1);
    }
    DPRINTK("MODPHY: cdev_alloc - Done\n");

    /* Init the driver */
    cdev_init(c_dev, &modphy_drv_fops);
    DPRINTK("MODPHY: cdev_init - Done\n");

    if ((ret = cdev_add(c_dev, dev, MINOR_CNT)) < 0)
    {
        printk(KERN_ERR "%s:%d Failed to add character device modphy_driver\n", __FUNCTION__, __LINE__);
        cdev_del(c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return ret;
    }
    DPRINTK("MODPHY: cdev_add - Done\n");

    if (IS_ERR(cl = class_create(THIS_MODULE, MODPHY_NAME)))
    {
        printk(KERN_ERR "MODPHY: class_create -Fail\n");
        cdev_del(c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    DPRINTK("MODPHY: class_create - Done\n");

    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, MODPHY_NAME)))
    {
        class_destroy(cl);
        cdev_del(c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    DPRINTK("MODPHY: Create device: /dev/%s\n", MODPHY_NAME);
#ifdef MODPHY_DEBUG
    if (NULL == (proc_create("modphy_dump_regs", 0, NULL, modphy_proc_dump_regs))) //TBD BEN - Change proc_create to fit the new kernel
    {
        printk(KERN_ERR "%s:%d ERROR ....\n", __FUNCTION__, __LINE__);
        return -1;
    }
#endif
#ifdef CONFIG_MRPC_HANDSHAKE_SERVER
    boot_status_report(HANDSHAKE_ID_MODPHY);
#endif
    return MODPHY_OK;
}

/**************************************************************************/
/*! \fn static void __exit modphy_drv_exit(void)
 **************************************************************************
 *  \brief     driver exit routine 
 *  \param[in] none
 *  \return    MODPHY_OK  = ok, MODPHY_FAIL = operation faild.
 *************************************************************************/
static void __exit modphy_drv_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}


subsys_initcall(modphy_drv_init);
module_exit(modphy_drv_exit);

/* Driver identification */
MODULE_DESCRIPTION("MODPHY Device Driver");
MODULE_LICENSE("GPL");
