/*
 *
 * puma5_cppi.h
 * Description:
 * File containing CPPI configurations for each driver.
 * Put into a single file to (hopefully) avoid configuration
 * clashes.
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


#ifndef __PUMA5_CPPI_H__
#define __PUMA5_CPPI_H__

/* CPMAC public */
#define CPMAC_RX_EMBEDDED_BD_NUM            64
#define CPMAC_RX_HOST_BD_NUM                64
#define CPMAC_TX_HOST_BD_NUM                512
/* legacy definitions */
#define CPMAC_RX_BD_NUM                     CPMAC_RX_EMBEDDED_BD_NUM
#define CPMAC_TX_BD_NUM                     CPMAC_TX_HOST_BD_NUM

#include "puma5_cppi_prv.h"

#define PAL_CPPI4_CACHE_INVALIDATE(addr, size)              dma_cache_inv ((unsigned long)(addr), (size))
#define PAL_CPPI4_CACHE_WRITEBACK(addr, size)               dma_cache_wback ((unsigned long)(addr), (size))
#define PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE(addr, size)    dma_cache_wback_inv ((unsigned long)(addr), (size))


extern char * qMNGRname[PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT];

#define PAL_CPPI41_GET_QNAME(num) (qMNGRname[(num)])

#endif /* __PUMA5_CPPI_H__ */
