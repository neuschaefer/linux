/*
 *
 * puma6_cppi.h
 * Description:
 * File containing CPPI configurations for each driver.
 * Put into a single file to (hopefully) avoid configuration
 * clashes.
 */


#ifndef __PUMA6_CPPI_H__
#define __PUMA6_CPPI_H__

/* CPMAC public */
#define CPMAC_RX_EMBEDDED_BD_NUM            64
#define CPMAC_RX_HOST_BD_NUM                64
#define CPMAC_TX_HOST_BD_NUM                512
/* legacy definitions */
#define CPMAC_RX_BD_NUM                     CPMAC_RX_EMBEDDED_BD_NUM
#define CPMAC_TX_BD_NUM                     CPMAC_TX_HOST_BD_NUM

#include "puma6_cppi_prv.h"

#define PAL_CPPI4_CACHE_INVALIDATE(addr, size)              dma_cache_inv ((unsigned long)(addr), (size))
#define PAL_CPPI4_CACHE_WRITEBACK(addr, size)               dma_cache_wback ((unsigned long)(addr), (size))
#define PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE(addr, size)    dma_cache_wback_inv ((unsigned long)(addr), (size))



extern char * qMNGRname[PAL_CPPI41_SR_QMGR_TOTAL_Q_COUNT];

#define PAL_CPPI41_GET_QNAME(num) (qMNGRname[(num)])

#endif /* __PUMA6_CPPI_H__ */
