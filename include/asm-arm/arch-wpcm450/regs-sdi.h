/* linux/include/asm/arch-wpcm450/regs-sdi.h
 *
 * Copyright (c) 2004 Simtec Electronics <linux@simtec.co.uk>
 *		      http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 MMC/SDIO register definitions
 *
 *  Changelog:
 *    18-Aug-2004 Ben Dooks      Created initial file
 *    29-Nov-2004 Koen Martens   Added some missing defines, fixed duplicates
 *    29-Nov-2004 Ben Dooks	 Updated Koen's patch
*/

#ifndef __ASM_ARM_REGS_SDI
#define __ASM_ARM_REGS_SDI "regs-sdi.h"

#define WPCM450_SDICON                (0x00)
#define WPCM450_SDIPRE                (0x04)
#define WPCM450_SDICMDARG             (0x08)
#define WPCM450_SDICMDCON             (0x0C)
#define WPCM450_SDICMDSTAT            (0x10)
#define WPCM450_SDIRSP0               (0x14)
#define WPCM450_SDIRSP1               (0x18)
#define WPCM450_SDIRSP2               (0x1C)
#define WPCM450_SDIRSP3               (0x20)
#define WPCM450_SDITIMER              (0x24)
#define WPCM450_SDIBSIZE              (0x28)
#define WPCM450_SDIDCON               (0x2C)
#define WPCM450_SDIDCNT               (0x30)
#define WPCM450_SDIDSTA               (0x34)
#define WPCM450_SDIFSTA               (0x38)
#define WPCM450_SDIDATA               (0x3C)
#define WPCM450_SDIIMSK               (0x40)

#define WPCM450_SDICON_BYTEORDER      (1<<4)
#define WPCM450_SDICON_SDIOIRQ        (1<<3)
#define WPCM450_SDICON_RWAITEN        (1<<2)
#define WPCM450_SDICON_FIFORESET      (1<<1)
#define WPCM450_SDICON_CLOCKTYPE      (1<<0)

#define WPCM450_SDICMDCON_ABORT       (1<<12)
#define WPCM450_SDICMDCON_WITHDATA    (1<<11)
#define WPCM450_SDICMDCON_LONGRSP     (1<<10)
#define WPCM450_SDICMDCON_WAITRSP     (1<<9)
#define WPCM450_SDICMDCON_CMDSTART    (1<<8)
#define WPCM450_SDICMDCON_INDEX       (0xff)

#define WPCM450_SDICMDSTAT_CRCFAIL    (1<<12)
#define WPCM450_SDICMDSTAT_CMDSENT    (1<<11)
#define WPCM450_SDICMDSTAT_CMDTIMEOUT (1<<10)
#define WPCM450_SDICMDSTAT_RSPFIN     (1<<9)
#define WPCM450_SDICMDSTAT_XFERING    (1<<8)
#define WPCM450_SDICMDSTAT_INDEX      (0xff)

#define WPCM450_SDIDCON_IRQPERIOD     (1<<21)
#define WPCM450_SDIDCON_TXAFTERRESP   (1<<20)
#define WPCM450_SDIDCON_RXAFTERCMD    (1<<19)
#define WPCM450_SDIDCON_BUSYAFTERCMD  (1<<18)
#define WPCM450_SDIDCON_BLOCKMODE     (1<<17)
#define WPCM450_SDIDCON_WIDEBUS       (1<<16)
#define WPCM450_SDIDCON_DMAEN         (1<<15)
#define WPCM450_SDIDCON_STOP          (1<<14)
#define WPCM450_SDIDCON_DATMODE	      (3<<12)
#define WPCM450_SDIDCON_BLKNUM        (0x7ff)

/* constants for WPCM450_SDIDCON_DATMODE */
#define WPCM450_SDIDCON_XFER_READY    (0<<12)
#define WPCM450_SDIDCON_XFER_CHKSTART (1<<12)
#define WPCM450_SDIDCON_XFER_RXSTART  (2<<12)
#define WPCM450_SDIDCON_XFER_TXSTART  (3<<12)

#define WPCM450_SDIDCNT_BLKNUM_SHIFT  (12)

#define WPCM450_SDIDSTA_RDYWAITREQ    (1<<10)
#define WPCM450_SDIDSTA_SDIOIRQDETECT (1<<9)
#define WPCM450_SDIDSTA_FIFOFAIL      (1<<8)	/* reserved on 2440 */
#define WPCM450_SDIDSTA_CRCFAIL       (1<<7)
#define WPCM450_SDIDSTA_RXCRCFAIL     (1<<6)
#define WPCM450_SDIDSTA_DATATIMEOUT   (1<<5)
#define WPCM450_SDIDSTA_XFERFINISH    (1<<4)
#define WPCM450_SDIDSTA_BUSYFINISH    (1<<3)
#define WPCM450_SDIDSTA_SBITERR       (1<<2)	/* reserved on 2410a/2440 */
#define WPCM450_SDIDSTA_TXDATAON      (1<<1)
#define WPCM450_SDIDSTA_RXDATAON      (1<<0)

#define WPCM450_SDIFSTA_TFDET          (1<<13)
#define WPCM450_SDIFSTA_RFDET          (1<<12)
#define WPCM450_SDIFSTA_TXHALF         (1<<11)
#define WPCM450_SDIFSTA_TXEMPTY        (1<<10)
#define WPCM450_SDIFSTA_RFLAST         (1<<9)
#define WPCM450_SDIFSTA_RFFULL         (1<<8)
#define WPCM450_SDIFSTA_RFHALF         (1<<7)
#define WPCM450_SDIFSTA_COUNTMASK      (0x7f)

#define WPCM450_SDIIMSK_RESPONSECRC    (1<<17)
#define WPCM450_SDIIMSK_CMDSENT        (1<<16)
#define WPCM450_SDIIMSK_CMDTIMEOUT     (1<<15)
#define WPCM450_SDIIMSK_RESPONSEND     (1<<14)
#define WPCM450_SDIIMSK_READWAIT       (1<<13)
#define WPCM450_SDIIMSK_SDIOIRQ        (1<<12)
#define WPCM450_SDIIMSK_FIFOFAIL       (1<<11)
#define WPCM450_SDIIMSK_CRCSTATUS      (1<<10)
#define WPCM450_SDIIMSK_DATACRC        (1<<9)
#define WPCM450_SDIIMSK_DATATIMEOUT    (1<<8)
#define WPCM450_SDIIMSK_DATAFINISH     (1<<7)
#define WPCM450_SDIIMSK_BUSYFINISH     (1<<6)
#define WPCM450_SDIIMSK_SBITERR        (1<<5)	/* reserved 2440/2410a */
#define WPCM450_SDIIMSK_TXFIFOHALF     (1<<4)
#define WPCM450_SDIIMSK_TXFIFOEMPTY    (1<<3)
#define WPCM450_SDIIMSK_RXFIFOLAST     (1<<2)
#define WPCM450_SDIIMSK_RXFIFOFULL     (1<<1)
#define WPCM450_SDIIMSK_RXFIFOHALF     (1<<0)

#endif /* __ASM_ARM_REGS_SDI */
