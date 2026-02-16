/*
 *
 * pal_sysHw.c
 * Description:
 * see below
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

/*
 * pal_sysHw.c
 */

#include <asm-arm/arch-avalanche/generic/pal.h>

BOARD_INFO_T soc[] = 
{
        /* 
         *  The structure should be filled as:
         *
         *  { MODULE_ID_AND_VERSION,    BASE_ADDRESS }
         *  
         *  Also the unused modules should be zero filled.
         */

        /* Puma5 VOLCANO emulation*/
        { 
            {
        		{AVALANCHE_NWSS_HW_MODULE_REV,  	    AVALANCHE_NWSS_SLAVE_BASE},
                {AVALANCHE_MDIO_HW_MODULE_REV,         	AVALANCHE_MDIO_BASE},
                {AVALANCHE_USB20_OTG_HW_MODULE_REV,  	AVALANCHE_USB20_OTG_SLAVE_BASE},
                {AVALANCHE_SPI_HW_MODULE_REV,           AVALANCHE_SPI_BASE},
		{AVALANCHE_TDM_HW_MODULE_REV,	        AVALANCHE_TDM_BASE},
		{AVALANCHE_CPMAC_HW_MODULE_REV,	        AVALANCHE_HIGH_CPMAC_BASE},	
                {0, 0}
            }
        },
};

static int avalanche_puma5_variant( void )
{
#ifdef CONFIG_MACH_PUMA5_VOLCANO
    return PUMA5_VOLCANO;
#endif

#ifdef CONFIG_MACH_PUMA5EVM3
    return PUMA5_EVM;
#endif

    return -1;
}

/*  Purpose: Get the BOARD variant
    Note: Currently this is implemented in h/w for titan boards only.
    For other boards it will return BOARD_TYPE_UNKNOWN.
*/
int avalanche_get_board_variant( void )
{
    static int init = 0;
    static int board_type;

    if ( !init )
    {
        board_type = avalanche_puma5_variant();
        init = 1;
    }

    return board_type;
}

/* 
 * Here the revision number of the module can be used for some version 
 * specific initialization 
 */
PAL_Result avalanche_device_prepare(Uint32 module_id, Uint32 base_addr, BOARD_ID board_variant, void *param)
{

    switch (module_id)
    {
        case AVALANCHE_NWSS_HW_MODULE_REV:
	    case AVALANCHE_MDIO_HW_MODULE_REV:
	    case AVALANCHE_USB20_OTG_HW_MODULE_REV:
    	case AVALANCHE_SPI_HW_MODULE_REV:
        case AVALANCHE_TDM_HW_MODULE_REV: 
	case AVALANCHE_CPMAC_HW_MODULE_REV:		
        break;

        default:
            return -1;
    };

    return 0;
}


