/*
<:copyright-BRCM:2013:DUAL/GPL:standard

   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

Unless you and Broadcom execute a separate written software license
agreement governing use of this software, this software is licensed
to you under the terms of the GNU General Public License version 2
(the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
with the following added to such license:

   As a special exception, the copyright holders of this software give
   you permission to link this software with independent modules, and
   to copy and distribute the resulting executable under terms of your
   choice, provided that you also meet, for each linked independent
   module, the terms and conditions of the license of that module.
   An independent module is a module which is not derived from this
   software.  The special exception does not apply to any modifications
   of the software.

Not withstanding the above, under no circumstances may you combine
this software in any way with any other Broadcom software provided
under a license other than the GPL, without Broadcom's express prior
written consent.

:>
*/

#ifdef CONFIG_BRCM_PCIE_PLATFORM

/* current linux kernel doesn't support pci bus rescan if we
 * power-down then power-up pcie.
 *
 * work-around by saving pci configuration after initial scan and
 * restoring it every time we repower pcie (implemented by module
 * init routine)
 *
 * module exit function powers down pcie
 */
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <bcm_map_part.h>
#include <pmc_pcie.h>

extern void bcm63xx_pcie_aloha(int hello);

static __init int bcm_mod_init(void)
{

	/* first invocation: save pci configuration
	 * subsequent: repower and restore configuration
	 */
	bcm63xx_pcie_aloha(1);

	return 0;
}

static void bcm_mod_exit(void)
{
	/* power down pcie */
	bcm63xx_pcie_aloha(0);
}

module_init(bcm_mod_init);
module_exit(bcm_mod_exit);

MODULE_LICENSE("GPL");

#endif
