/*****************************************************************************
*  Copyright 2006 - 2011 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/**
 * @file    chal_nandbch_uc.h
 * @brief   CHAL NANDBCH microcode.
 * @note
 **/

#ifndef __CHAL_NANDBCH_UC_H__
#define __CHAL_NANDBCH_UC_H__

#include "chal_nand_cmd.h"                /* nand command definitions */
#include "chal_nand_uasm.h"               /* micro-assembly definitions */

/* timeouts definition
 * using steps of ~320 ns
 */
#define uTIMEOUT_TWB         2
#define uTIMEOUT_TCCS        1
#define uTIMEOUT_TWHR        1
#define uTIMEOUT_TADL        1

/* UC command definitions */
typedef enum tag_chal_nand_uc_cmd_t {
  CHAL_NAND_UC_RESET,
  CHAL_NAND_UC_ID_GET,
  CHAL_NAND_UC_PARAM_READ_PRE,
  CHAL_NAND_UC_GET_FEATURE_PRE,
  CHAL_NAND_UC_SET_FEATURE,
  CHAL_NAND_UC_READ_PRE,
  CHAL_NAND_UC_READ,
  CHAL_NAND_UC_READ_RANDOM,
  CHAL_NAND_UC_READ_ECC,
  CHAL_NAND_UC_READ_RANDOM_ECC,
  CHAL_NAND_UC_STATUS_GET,
  CHAL_NAND_UC_BLOCK_ERASE,
  CHAL_NAND_UC_WRITE_PRE,
  CHAL_NAND_UC_WRITE_RANDOM,
  CHAL_NAND_UC_WRITE,
  CHAL_NAND_UC_WRITE_COMPLETE,
  CHAL_NAND_UC_UC_MAX
} chal_nand_uc_cmd_t;



/* send reset command */
static const uint32_t uc_RESET[]  = {
  uA_WCI(NAND_CMD_RESET),           /* send command 0xFF */
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWB),	/* wait tWB	*/
  uA_END
};
  
/* read ID
 * address: 0x00 to read the NAND ID 
 * address: 0x20 to read the ONFI identification code 
 * the ID size is in (R4) argument 0 low (need to be multiple of 2)
 */
static const uint32_t uc_ID_GET[]  = {
  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */
  uA_MV(uR2,uRB), 	               /* load address ( 0x0 or 0x20 ) */
  uA_WCI(NAND_CMD_ID),              /* send command 0x90 */
  uA_WAI(0x1),                      /* one address cycle	*/
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWHR),	/* wait tWHR	*/
  uA_RD(uR4),                       /* execute [R4] read cycles */
  uA_BRA(-7),                       /* jump to wait for DMA start event */
  uA_END
};

/* ONFI parameters read preamble */
static const uint32_t uc_PARAM_READ_PRE[]  = {
  uA_ANDI(uRB,0x0),                 /* zero the adderess */
  uA_WCI(NAND_CMD_READ_PARAM),      /* send command 0xEC	*/
  uA_WAI(0x1),                      /* one address cycle	*/
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWB),	/* wait tWB	*/
  uA_END
};

/* get feature preamble
 * address: feature address
 */
static const uint32_t uc_GET_FEATURE_PRE[]  = {
  uA_MV(uR2,uRB), 	               /* load feature address */
  uA_WCI(NAND_CMD_GET_FEATURE),     /* send command 0xEE */
  uA_WAI(0x1),                      /* one address cycle	*/
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWB),   /* wait tWB	*/
  uA_END
};

/* set feature
 * address: feature address
 */
static const uint32_t uc_SET_FEATURE[]  = {
  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */
  uA_MV(uR2,uRB), 	               /* load feature address */
  uA_WCI(NAND_CMD_SET_FEATURE),     /* send command 0xEF */
  uA_WAI(0x1),                      /* one address cycle	*/
  uA_WTS(uEVT_TOUT,uTIMEOUT_TADL),  /* wait tADL */
  uA_WDI(0x4),                      /* write 4 bytes */
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWB),   /* wait tWB	*/
  uA_BRA(-8),                       /* jump to wait for DMA start event */
  uA_END
};

/* read preamble
 * the row address is in (R2,R3) address register
 * the collumn address is in (R5) argument 0 high
 * the number of address cycles is in (R6) argument 1 low
 */
static const uint32_t uc_READ_PRE[]  = {
  uA_MV(uR5,uRB), 	               /* load the 2 bytes of column address  	*/
  uA_MV(uR2,uRC),                   /* load first 2 bytes of address (lower) */
  uA_MV(uR3,uRD),                   /* load 3rd byte of address */
  uA_WCI(NAND_CMD_READ_1ST),        /* send command 0x00 */
  uA_WA(uR6),                       /* 4 or 5 address cycles */
  uA_WCI(NAND_CMD_READ_2ND),	      /* send command 0x30	*/
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWB),	/* wait tWB	*/
  uA_END
};

/* raw read
 * the data size is in (R4) argument 0 low (need to be multiple of 2)
 */
static const uint32_t uc_READ[]  = {
  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */
  uA_RD(uR4),                       /* execute [R4] read cycles */
  uA_BRA(-3),                       /* jump to wait for DMA start event when done */
  uA_END
};

/* raw random read
 * the data size is in (R4) argument 0 low (need to be multiple of 2)
 * the data offset is in (R5) argument 0 high
 */
static const uint32_t uc_READ_RANDOM[]  = {
  uA_MV(uR5,uRB), 	               /* load the 2 bytes of column address  	*/

  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */

  uA_WCI(NAND_CMD_READ_RAND_1ST),   /* start random output */
  uA_WAI(0x2),			               /* 2 address cycles */
  uA_WCI(NAND_CMD_READ_RAND_2ND),

  uA_WTS(uEVT_TOUT,uTIMEOUT_TCCS),  /* wait tCCS */

  uA_RD(uR4),                       /* execute [R4] read cycles */

  uA_BRA(-7),                       /* jump to wait for DMA start event when done */
  uA_END
};


/* read with ECC
 * the data size is in (R4) argument 0 low (need to be multiple of 2)
 * the ECC size is in (R6) argument 1 low (need to be multiple of 2)
 * the ECC offset is in (R7) argument 1 high
 */
static const uint32_t uc_READ_ECC[]  = {
  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */

  uA_RD(uR4),                       /* execute [R4] read cycles to read data */

  uA_MV(uR7,uRB), 	               /* load the 2 bytes of ECC offset */

  uA_WCI(NAND_CMD_READ_RAND_1ST),   /* start random output */
  uA_WAI(0x2),			               /* 2 address cycles */
  uA_WCI(NAND_CMD_READ_RAND_2ND),

  uA_WTS(uEVT_TOUT,uTIMEOUT_TCCS),  /* wait tCCS */

  uA_RD(uR6),                       /* execute [R6] read cycles to read ECC bytes */

  uA_BRA(-9),                       /* jump to wait for DMA start event when done */
  uA_END
};


/* random read with ECC
 * the data size is in (R4) argument 0 low (need to be multiple of 2)
 * the data offset is in (R5) argument 0 high
 * the ECC size is in (R6) argument 1 low (need to be multiple of 2)
 * the ECC offset is in (R7) argument 1 high
 */
static const uint32_t uc_READ_RANDOM_ECC[]  = {
  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */

  uA_MV(uR5,uRB), 	               /* load the 2 bytes of data offset */
  uA_WCI(NAND_CMD_READ_RAND_1ST),   /* start random output */
  uA_WAI(0x2),			               /* 2 address cycles */
  uA_WCI(NAND_CMD_READ_RAND_2ND),

  uA_WTS(uEVT_TOUT,uTIMEOUT_TCCS),  /* wait tCCS */

  uA_RD(uR4),                       /* execute [R4] read cycles to read data */

  uA_MV(uR7,uRB), 	               /* load the 2 bytes of ECC offset */

  uA_WCI(NAND_CMD_READ_RAND_1ST),   /* start random output */
  uA_WAI(0x2),			               /* 2 address cycles */
  uA_WCI(NAND_CMD_READ_RAND_2ND),

  uA_WTS(uEVT_TOUT,uTIMEOUT_TCCS),  /* wait tCCS */

  uA_RD(uR6),                       /* execute [R6] read cycles to read ECC bytes */

  uA_BRA(-14),                      /* jump to wait for DMA start event when done */
  uA_END
};


/* read status */
static const uint32_t uc_STATUS_GET[]  = {
  uA_WCI(NAND_CMD_STATUS),          /* send command 0x70 (read status) */
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWHR),  /* Wait tWHR */
  uA_RS,                            /* read status */
  uA_MV(uRA,uRF),                   /* move status to RF */
  uA_END
};


/* block erase
 * the block address is in (R2,R3) address register
 * the number of address cycles is in (R6) argument 1 low
 */
static const uint32_t uc_BLOCK_ERASE[]  = {
  uA_MV(uR2,uRB),                   /* load the first 2 bytes of address (lower) */
  uA_MV(uR3,uRC),                   /* load 3rd byte of address */
  uA_WCI(NAND_CMD_BERASE_1ST),      /* send command 0x60 (block erase) */
  uA_WA(uR6),                       /* 2 or 3 address cycles */
  uA_WCI(NAND_CMD_BERASE_2ND),      /* send command 0xD0 (confirm the erase operation) */ 
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWB),	/* wait tWB	*/
  uA_END
};


/* write preamble
 * page address (row) is in the address register (R2, R3)
 * the data size is in (R4) argument 0 low (need to be multiple of 2)
 * the data offset (column) is in (R5) argument 0 high
 * the number of address cycles is in (R6) argument 1 low
 */
static const uint32_t uc_WRITE_PRE[]  = {
  uA_MV(uR5,uRB),                   /* load the 2 bytes of column */
  uA_MV(uR2,uRC),                   /* load the first 2 bytes of address ( lower )  	*/
  uA_MV(uR3,uRD),                   /* load the 3rd byte of address			*/
  uA_WCI(NAND_CMD_PROG_1ST),        /* send command 0x80 */
  uA_WA(uR6),                       /* 4 or 5 address cycles */
  uA_WTS(uEVT_TOUT,uTIMEOUT_TADL),  /* wait tADL */

  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */

  uA_WD(uR4),                       /* execute [R4] write cycles */

  uA_BRA(-3),                       /* jump to wait for DMA start event when done */
  uA_END
};


/* write random
 * the data size is in (R4) argument 0 low (need to be multiple of 2)
 * the column address is in (R5) argument 0 high
 */
static const uint32_t uc_WRITE_RANDOM[]  = {
  uA_MV(uR5,uRB), 	               /* load the 2 bytes of column address  	*/
  uA_WCI(NAND_CMD_PROG_RAND),       /* send command 0x85 */
  uA_WAI(0x2),			               /* 2 address cycles */

  uA_WTS(uEVT_TOUT,uTIMEOUT_TCCS),  /* wait tCCS */
   
  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */
   
  uA_WD(uR4),                       /* execute [R4] write cycles */

  uA_BRA(-3),                       /* jump to wait for DMA start event when done */
  uA_END
};


/* write
 * the data size is in (R4) argument 0 low (need to be multiple of 2)
 */
static const uint32_t uc_WRITE[]  = {
  uA_WTS(uEVT_DMA_START,0),         /* wait for DMA start event */
  uA_WD(uR4),                       /* execute [R4] write cycles */
  uA_BRA(-3),                       /* jump to wait for DMA start event when done */
  uA_END
};


/* write buffer
 */
static const uint32_t uc_WRITE_COMPLETE[]  = {
  uA_WCI(NAND_CMD_PROG_2ND),        /* send command 0x10 */
  uA_WTS(uEVT_TOUT,uTIMEOUT_TWB),	/* wait tWB	*/
  uA_END
};


#endif

