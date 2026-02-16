/*
 * sdhci-puma6.c
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2012-2014 Intel Corporation. All rights reserved.
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


/*
 * SDHCI support for Puma6 SoC
 *
 * 
 */

#include <asm/io.h>
#include <linux/mmc/host.h>
#include <linux/module.h>
#include "sdhci.h"
#include "sdhci-pltfm.h"


static inline void sdhci_puma6_writeb(struct sdhci_host *host, u8 val, int reg)
{
    int align_reg;
    u32 align_val;

    /* WA for Puma6 A0.
      In Puma6 A0 we must write to HC Registers within width of 32 bits (4 bytes alingment) 
      Solution: Read the 32 bit register, modify the reqiured byte (8 bit), and write back 32 bits
    */

    /* align the register to 32 bit */
    align_reg = reg & 0xFFFFFFFC;      // Clear bit #0,#1

    // Read
    align_val = readl(host->ioaddr + align_reg);    
    
    // Modify 
    align_val = align_val &~ (((u32)0x000000FF) << ((reg & 0x00000003) << 3));  // Reset the 0,1,2 or 3 byte
    align_val = align_val | ((u32)val) << ((reg & 0x00000003) << 3);         // Add the 0,1,2 or 3 byte.

    // Write
    writel(align_val, host->ioaddr + align_reg);
}


static inline void sdhci_puma6_writew(struct sdhci_host *host, u16 val, int reg)
{
    int align_reg;
    u32 align_val;
    static u32 shadow_value;
    static u32 shadow_valid = 0;

    /* In Puma6 we must write to HC Registers within width of 32 bits (4 bytes alingment) 
      Solution: Read the 32 bit register, modify the High Word (16 bit), or Low Word, and write back 32 bits
    */

    /* In Puma6 A special case is wiriting to TRANSFER MODE register.
      Writing to TRANSFER MODE Register with Read/modify/write solution (as above), will trigger the 
      COMMAND Register, to send a message to eMMC card 
      Solution: write the value of TRANSFER MODE to 'a shadow' register, and next time the user write 
      to COMMAND Register, then write the shadow register to TRANSFER MODE register.
     
      Note: According to spec after each writing to TRANSFER MODE register, and user will also write
      to COMMAND Register. 
    */

    /* align the register to 32 bit */
    align_reg = reg & 0xFFFFFFFC;      // Clear bit #0,#1

    /* Save Transfer Mode to a shadow register */
    if (unlikely(reg == SDHCI_TRANSFER_MODE))
    {
        shadow_value = val;
        shadow_valid = 1;
        return;
    }

    /* Restore the Transfer Mode from a shadow register */
    if (unlikely((reg == SDHCI_COMMAND) && (shadow_valid == 1)))
    {
        // Read from shadow
        align_val = shadow_value;
        shadow_valid = 0;
    }
    else
    {
        // Read form Register
        align_val = readl(host->ioaddr + align_reg); 
    }

    // Modify 
    align_val = align_val &~ (((u32)0x0000FFFF) << ((reg & 0x00000003) << 3));  // Reset the first or second word (16 bits)
    align_val = align_val | ((u32)val) << ((reg & 0x00000003) << 3);            // Add the first or second word (16 bits)

    
    // Write
    writel(align_val, host->ioaddr + align_reg);
}


static struct sdhci_ops sdhci_puma6_ops = {
    .write_b = sdhci_puma6_writeb,
    .write_w = sdhci_puma6_writew,
};

struct sdhci_pltfm_data sdhci_puma6_pdata = {
	.ops = &sdhci_puma6_ops,
	.quirks = // SDHCI_QUIRK_BROKEN_CARD_DETECTION |
              // SDHCI_QUIRK_NO_CARD_NO_RESET      |
              // SDHCI_QUIRK_DELAY_AFTER_POWER     |
              // SDHCI_QUIRK_BROKEN_DMA            |
              // SDHCI_QUIRK_BROKEN_ADMA           |
              // SDHCI_QUIRK_32BIT_DMA_ADDR        |
              // SDHCI_QUIRK_32BIT_DMA_SIZE        |
              // SDHCI_QUIRK_32BIT_ADMA_SIZE       |  
              // SDHCI_QUIRK_NO_HISPD_BIT          |
              // SDHCI_QUIRK_FORCE_1_BIT_DATA      |
              0,
};


static int sdhci_puma6_probe(struct platform_device *pdev)
{
    return sdhci_pltfm_register(pdev, &sdhci_puma6_pdata, 0);
}

static int sdhci_puma6_remove(struct platform_device *pdev)
{
    return sdhci_pltfm_unregister(pdev);
}

static struct platform_driver sdhci_puma6_driver = {
	.driver		= {
		.name	= "sdhci-puma6",
		.owner	= THIS_MODULE,
		.pm	= SDHCI_PLTFM_PMOPS,
	},
	.probe		= sdhci_puma6_probe,
	.remove		= sdhci_puma6_remove,
};

module_platform_driver(sdhci_puma6_driver);

MODULE_DESCRIPTION("SDHCI driver for Puma6");
MODULE_AUTHOR("Intel Corporation");
MODULE_LICENSE("GPL v2");
