/*
 *  linux/include/asm-arm/arch-mt5391/hardware.h
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <mach/memory.h>
#include <mach/platform.h>

#define pcibios_assign_all_busses()	1

#define PCIBIOS_MIN_IO		(0)
#define PCIBIOS_MIN_MEM		(PCIE_MMIO_LENGTH)
#define PCIMEM_BASE		(PCIE_MMIO_BASE)

#ifdef CONFIG_PCI_MSI
/* MSI MTK arch hooks */
#define arch_setup_msi_irqs MTK_setup_msi_irqs
#define arch_teardown_msi_irqs MTK_teardown_msi_irqs
#define arch_msi_check_device MTK_msi_check_device
#endif /* #ifdef CONFIG_PCI_MSI */

#endif

