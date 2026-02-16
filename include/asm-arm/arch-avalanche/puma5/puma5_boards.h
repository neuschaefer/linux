/*
 *
 * puma5_boards.h
 * Description:
 * board specific macros
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


#ifndef _PUMA5_BOARDS_H
#define _PUMA5_BOARDS_H

#define REFCLK_FREQ                                25000000
/* PHY MASK is same for both Volcano and EVM */
/* CPMAC with low base address */
#define AVALANCHE_LOW_CPMAC_PHY_MASK               0x00000002
/* CPMAC with high base address*/
/* There is no high cpmac in PUMA5 */
#define AVALANCHE_HIGH_CPMAC_PHY_MASK              0x00000004
#define VLYNQ0_RESET_GPIO_NUM                      0x7
#define MARVELL_PHY_OFFSET                         0x10
#define MARVELL_PORT_OFFSET                        0x18

#if defined(CONFIG_MACH_PUMA5EVM)
/* According to schematics PUMA-5 does not have external switch */
#define AVALANCHE_LOW_CPMAC_HAS_EXT_SWITCH         0
#define AVALANCHE_HIGH_CPMAC_HAS_EXT_SWITCH        0
/* GPIO number 14 is used to reset the External phy
 */
#define EXTPHY_RESET_DEFAULT_INDEX                  0
#define EXTPHY_RESET_TNETC550_GPIO_NUM              14
#define EXTPHY_RESET_TNETC950_GPIO_NUM              32
#define EXTPHY_RESET_TNETC958_GPIO_NUM              23
#define EXTPHY_RESET_TNETC552_GPIO_NUM              9

#endif

#if defined (CONFIG_MACH_PUMA5_VOLCANO)

#define AVALANCHE_LOW_CPMAC_HAS_EXT_SWITCH         0
#define AVALANCHE_HIGH_CPMAC_HAS_EXT_SWITCH        0

#endif

/* Puma5 variants detection */
typedef enum {
	PUMA5_VOLCANO = 0,
	PUMA5_UR8EVM,
	MAX_BOARDS /* For count */
} BOARD_ID;

int avalanche_get_board_variant( void );

#define MAX_MODULES 8


#endif /* _PUMA5_BOARDS_H */
