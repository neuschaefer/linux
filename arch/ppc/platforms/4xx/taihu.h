/*
 * Support for IBM PPC 405EP evaluation board (Taihu).
 *
 * Author: SAW (IBM), derived from walnut.h.
 *         Maintained by MontaVista Software <source@mvista.com>
 *
 * 2003 (c) MontaVista Softare Inc.  This file is licensed under the
 * terms of the GNU General Public License version 2. This program is
 * licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#ifdef __KERNEL__
#ifndef __TAIHU_H__
#define __TAIHU_H__

/* 405EP */
#include <platforms/4xx/ibm405ep.h>

#ifndef __ASSEMBLY__
/*
 * Data structure defining board information maintained by the boot
 * ROM on IBM's evaluation board. An effort has been made to
 * keep the field names consistent with the 8xx 'bd_t' board info
 * structures.
 */
#define CONFIG_HAS_ETH1 1
typedef struct board_info {
	unsigned long	bi_memstart;	/* start of DRAM memory */
	unsigned long	bi_memsize;	/* size	 of DRAM memory in bytes */
	unsigned long	bi_flashstart;	/* start of FLASH memory */
	unsigned long	bi_flashsize;	/* size	 of FLASH memory */
	unsigned long	bi_flashoffset; /* reserved area for startup monitor */
	unsigned long	bi_sramstart;	/* start of SRAM memory */
	unsigned long	bi_sramsize;	/* size	 of SRAM memory */
	unsigned long	bi_bootflags;	/* boot / reboot flag (for LynxOS) */
	unsigned long	bi_ip_addr;	/* IP Address */
	unsigned char	bi_enetaddr[6];	/* Ethernet adress */
	unsigned short	bi_ethspeed;	/* Ethernet speed in Mbps */
	unsigned long	bi_intfreq;	/* Internal Freq, in MHz */
	unsigned long	bi_busfreq;	/* Bus Freq, in MHz */
	unsigned long	bi_baudrate;	/* Console Baudrate */
#if defined(CONFIG_405)   ||			\
	defined(CONFIG_405GP) ||		\
	defined(CONFIG_405CR) ||		\
	defined(CONFIG_405EP) ||		\
	defined(CONFIG_440)
	unsigned char	bi_s_version[4];	/* Version of this structure */
	unsigned char	bi_r_version[32];	/* Version of the ROM (IBM) */
	unsigned int	bi_pllouta_freq;	/* CPU (Internal) Freq, in Hz */
	unsigned int	bi_plb_busfreq;	/* PLB Bus speed, in Hz */
	unsigned int	bi_pci_busfreq;	/* PCI Bus speed, in Hz */
	unsigned char	bi_pci_enetaddr[6];	/* PCI Ethernet MAC address */
#endif

#ifdef CONFIG_HAS_ETH1
	/* second onboard ethernet port */
	unsigned char   bi_enet1addr[6];
#endif
#ifdef CONFIG_HAS_ETH2
	/* third onboard ethernet port */
	unsigned char	bi_enet2addr[6];
#endif
#ifdef CONFIG_HAS_ETH3
	unsigned char   bi_enet3addr[6];
#endif

#if defined(CONFIG_405GP) || defined(CONFIG_405EP) || defined (CONFIG_440GX) || \
	defined(CONFIG_440EP) || defined(CONFIG_440GR)
	unsigned int	bi_opbfreq;		/* OPB clock in Hz */
	int		bi_iic_fast[2];		/* Use fast i2c mode */
#endif
#if defined(CONFIG_4xx)
#if defined(CONFIG_440GX)
	int 		bi_phynum[4];           /* Determines phy mapping */
	int 		bi_phymode[4];          /* Determines phy mode */
#elif defined(CONFIG_405EP) || defined(CONFIG_440)
	int 		bi_phynum[2];           /* Determines phy mapping */
	int 		bi_phymode[2];          /* Determines phy mode */
#else
	int 		bi_phynum[1];           /* Determines phy mapping */
	int 		bi_phymode[1];          /* Determines phy mode */
#endif
#endif /* defined(CONFIG_4xx) */
} bd_t;

/* Some 4xx parts use a different timebase frequency from the internal clock.
 */
#define bi_tbfreq bi_intfreq


/* The UART clock is based off an internal clock -
 * define BASE_BAUD based on the internal clock and divider(s).
 * Since BASE_BAUD must be a constant, we will initialize it
 * using clock/divider values which OpenBIOS initializes
 * for typical configurations at various CPU speeds.
 * The base baud is calculated as (FWDA / EXT UART DIV / 16)
 */
#define BASE_BAUD      691200

#define PPC4xx_MACHINE_NAME     "AMCC Taihu"

#endif /* !__ASSEMBLY__ */
#endif /* __TAIHU_H__ */
#endif /* __KERNEL__ */
