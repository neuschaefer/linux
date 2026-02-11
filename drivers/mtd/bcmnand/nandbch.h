/*****************************************************************************
* Copyright 2004 - 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/
/**
 * @file    nandbch.h
 * @brief   NANDBCH low level API
 * @note
 **/

#ifndef __NANDBCH_H__
#define __NANDBCH_H__

// #define NANDBCH_DEBUG 1

#ifndef NANDBCH_DEBUG
#define NANDBCH_DEBUG 0
#endif

#if NANDBCH_DEBUG == 0
#define dprint(fmt, args...)
#else
#define dprint(fmt, args...) printk(fmt, ##args)
#endif

#define NANDBCH_BANKS_MAX	2

#define NANDBCH_UC_CMD_MAX	32

#define NANDBCH_PAGE_MAX_SIZE	0x4000	/* 16K page */
#define NANDBCH_OOB_MAX_SIZE	0x400	/* 1024 bytes */
#define NANDBCH_AUX_MAX_SIZE	0xF8	/* 248 bytes (31*8) */
#define NANDBCH_ECC_MAX_SIZE	0x48	/* 72 bytes (70 ECC bytes + 2 bytes padding) */

/* size of the ID buffer */
#define NANDBCH_ID_BUF_SIZE	8
/* size of the ONFI feature buffer */
#define NANDBCH_FEATURE_SIZE	4

/* Nand ID definitions */
#define NANDBCH_JEDEC_ID_ADDR	0x0
#define NANDBCH_JEDEC_ID_SIZE	8
#define NANDBCH_ONFI_ID_ADDR	0x20
#define NANDBCH_ONFI_ID_SIZE	4

/* Enable ECC for main data (ECC disabled if flag not set) */
#define NANDBCH_FLAG_ECC	(0x01)
/* 
 * Use the data provided in the geometry field of the ni parameter to the 
 * nandbch_init call 
 */
#define NANDBCH_FLAG_GEOMETRY	(0x02)
/*
 * Use the data provided in the timing field of the ni parameter to the
 * nandbch_init call 
 */
#define NANDBCH_FLAG_TIMING	(0x04)

/*
 * Use the ECC configuration data provided in the ecc field of the ni parameter to the
 * nandbch_init call 
 */
#define NANDBCH_FLAG_BCH_CONFIG	(0x08)

/*
 * Use the auxiliary data configuration provided in the aux fields of the ni parameter to the
 * nandbch_init call 
 */
#define NANDBCH_FLAG_AUX_CONFIG	(0x10)

/* NAND function return codes */
enum {
	NANDBCH_RC_SUCCESS = 0x00,
	NANDBCH_RC_FAILURE,	/* Generic failure */
	NANDBCH_RC_NOMEM,	/* UC memory not available */
	NANDBCH_RC_TOUT,	/* UC competion timeout */
	NANDBCH_RC_BANK_ERROR,	/* UC execution error */
	NANDBCH_RC_DMA_ERROR,	/* DMA error */
	NANDBCH_RC_ECC_CFG_ERR,	/* Unsupported BCH ECC configuration */
	NANDBCH_RC_ECC_ERROR,	/* Uncorrectable BCH ECC error */
	NANDBCH_RC_ECC_TOUT,	/* BCH ECC irq timeout */
	NANDBCH_RC_BANK_CFG_ERR,	/* Unsupported bank configuration */
	NANDBCH_RC_BB_NOERASE,	/* Erase bad block not alowed */
	NANDBCH_RC_NON_ONFI,	/* NAND not ONFI compliant */
	NANDBCH_RC_PARAM_ERR,	/* ONFI parameter page error */
	NANDBCH_RC_EXTPARAM_ERR,	/* ONFI extended parameter page error */
	NANDBCH_RC_PINMUX_ERR,	/* PINMUX error */
	NANDBCH_RC_NOCONFIG,	/* Configuration data not found */
	NANDBCH_RC_IF_ERR,	/* Interface configuration error */
	NANDBCH_RC_FAIL_STATUS,	/* NAND status fail */
	NANDBCH_RC_AUX_CFG_ERR,	/* Invalid AUX data configuration */
	NANDBCH_RC_OOB_SIZE_ERR, /* Insufficient OOB bytes for current configuration */ 
	NANDBCH_RC_MAX
};
/* NAND timing select values */
enum { NANDBCH_TIMING_SELECT_DEFAULT = 0x00,	/* ONFI timing mode 0 */
	NANDBCH_TIMING_SELECT_TMODE_SET_FEATURE,	/* Use ONFI timing mode and send */
	/* ONFI SET_FEATURE command to   */
	/* device to change timing mode  */
	NANDBCH_TIMING_SELECT_TMODE_ONLY,	/* Use ONFI timing mode only     */
	NANDBCH_TIMING_SELECT_REG_CAPRI,	/* Use NAND controller timing    */
	/* configuration register values */
	/* (Capri encoding scheme)       */
	NANDBCH_TIMING_SELECT_MAX
};

/* NAND timing parameters */
typedef struct tag_nandbch_timing_t {
	uint8_t select;
	uint8_t mode;
	uint32_t conf1;
	uint32_t conf2;
} nandbch_timing_t;

/* NAND geometry information */
typedef struct tag_nandbch_geometry_t {
	uint32_t bus_width;
	uint32_t addr_cycles;
	uint32_t banks;
	uint32_t page_shift;
	uint32_t block_shift;
	uint32_t bank_shift;
	uint32_t oob_size;
} nandbch_geometry_t;

/* NAND BCH ECC information */
typedef struct tag_nandbch_bch_ecc_t {
	uint32_t data_size;
	uint32_t ecc_bytes;	/* number of ECC bytes */
	uint32_t ecc_size;	/* including padding bytes */
	uint16_t ecc_n;
	uint16_t ecc_k;
	uint8_t ecc_t;
} nandbch_bch_ecc_t;

/* NAND ONFI parameters */
typedef struct tag_nandbch_onfi_param_t {
	uint16_t feature;
	uint16_t opt_cmd;
} nandbch_onfi_param_t;

#define NANDBCH_MAX_SECTORS 32	/* 16K page with 512 byte sectors */
/* NAND ECC statistics */
typedef struct tag_nandbch_eccstats_t {
	uint8_t len;		/* length of statistics filled in by called function */
	uint8_t errs[NANDBCH_MAX_SECTORS];
} nandbch_eccstats_t;

#define BCMCAPRI_BUF_SIZE (NANDBCH_PAGE_MAX_SIZE + NANDBCH_OOB_MAX_SIZE)

struct bcmnand_buf {
	uint8_t buf[BCMCAPRI_BUF_SIZE];
	dma_addr_t phys_addr;
};
struct bcmnand_desc {
	chal_nand_prd_entry_t desc[2];	/* Maximum 2 descriptors needed */
	dma_addr_t phys_addr;
};
struct bcmnand_param_buf {
	uint8_t buf[512];
	dma_addr_t phys_addr;
};
struct bcmnand_ecc_buf {
	uint8_t buf[NANDBCH_ECC_MAX_SIZE];
	dma_addr_t phys_addr;
};

/* NAND information structure */
typedef struct tag_nandbch_info_t {
	uint8_t id[NANDBCH_JEDEC_ID_SIZE];
	uint32_t flags;
	nandbch_geometry_t geometry;
	nandbch_timing_t timing;
	nandbch_bch_ecc_t sector;
	nandbch_bch_ecc_t aux;
	nandbch_onfi_param_t onfi;
	uint16_t uc_offset[NANDBCH_UC_CMD_MAX];
	struct bcmnand_buf buf;
	struct bcmnand_desc desc;
	struct bcmnand_param_buf param_buf;
	struct bcmnand_ecc_buf ecc_buf;
	struct device *dev;
} nandbch_info_t;

#define NANDBCH_ID(ni)			(((nandbch_info_t *)(ni))->id)
#define NANDBCH_FLAGS(ni)		(((nandbch_info_t *)(ni))->flags)
#define NANDBCH_TIMING_SELECT(ni)	(((nandbch_info_t *)(ni))->timing.select)
#define NANDBCH_TIMING_MODE(ni)		(((nandbch_info_t *)(ni))->timing.mode)
#define NANDBCH_TIMING_CONF1(ni)	(((nandbch_info_t *)(ni))->timing.conf1)
#define NANDBCH_TIMING_CONF2(ni)	(((nandbch_info_t *)(ni))->timing.conf2)
#define NANDBCH_UC_OFFSET(ni)		(((nandbch_info_t *)(ni))->uc_offset)

#define NANDBCH_GEOMETRY(ni)		(((nandbch_info_t *)(ni))->geometry)
#define NANDBCH_BUS_WIDTH(ni)		(((nandbch_info_t *)(ni))->geometry.bus_width)
#define NANDBCH_ADDR_CYCLES(ni)		(((nandbch_info_t *)(ni))->geometry.addr_cycles)
#define NANDBCH_BANKS(ni)		(((nandbch_info_t *)(ni))->geometry.banks)
#define NANDBCH_PAGE_SHIFT(ni)		(((nandbch_info_t *)(ni))->geometry.page_shift)
#define NANDBCH_BLOCK_SHIFT(ni)		(((nandbch_info_t *)(ni))->geometry.block_shift)
#define NANDBCH_BANK_SHIFT(ni)		(((nandbch_info_t *)(ni))->geometry.bank_shift)
#define NANDBCH_OOB_SIZE(ni)		(((nandbch_info_t *)(ni))->geometry.oob_size)

#define NANDBCH_SECTOR_SIZE(ni)		(((nandbch_info_t *)(ni))->sector.data_size)
#define NANDBCH_ECC_BYTES(ni)		(((nandbch_info_t *)(ni))->sector.ecc_bytes)
#define NANDBCH_ECC_SIZE(ni)		(((nandbch_info_t *)(ni))->sector.ecc_size)
#define NANDBCH_ECC_N(ni)		(((nandbch_info_t *)(ni))->sector.ecc_n)
#define NANDBCH_ECC_K(ni)		(((nandbch_info_t *)(ni))->sector.ecc_k)
#define NANDBCH_ECC_T(ni)		(((nandbch_info_t *)(ni))->sector.ecc_t)

#define NANDBCH_AUX_SIZE(ni)		(((nandbch_info_t *)(ni))->aux.data_size)
#define NANDBCH_AUX_ECC_BYTES(ni)	(((nandbch_info_t *)(ni))->aux.ecc_bytes)
#define NANDBCH_AUX_ECC_SIZE(ni)	(((nandbch_info_t *)(ni))->aux.ecc_size)
#define NANDBCH_AUX_ECC_N(ni)		(((nandbch_info_t *)(ni))->aux.ecc_n)
#define NANDBCH_AUX_ECC_K(ni)		(((nandbch_info_t *)(ni))->aux.ecc_k)
#define NANDBCH_AUX_ECC_T(ni)		(((nandbch_info_t *)(ni))->aux.ecc_t)

#define NANDBCH_PAGE_SIZE(ni)		(0x1U<<NANDBCH_PAGE_SHIFT(ni))
#define NANDBCH_BLOCK_SIZE(ni)		(0x1U<<NANDBCH_BLOCK_SHIFT(ni))

#define NANDBCH_PAGE_MASK(ni)		(~(NANDBCH_PAGE_SIZE(ni)-1))
#define NANDBCH_BLOCK_MASK(ni)		(~(NANDBCH_BLOCK_SIZE(ni)-1))

#define NANDBCH_SECTORS(ni)		(NANDBCH_PAGE_SIZE(ni)/NANDBCH_SECTOR_SIZE(ni))
#define NANDBCH_PAGES(ni)		(0x1U<<(NANDBCH_BLOCK_SHIFT(ni)-NANDBCH_PAGE_SHIFT(ni)))
#define NANDBCH_BLOCKS(ni)		(0x1U<<(NANDBCH_BANK_SHIFT(ni)-NANDBCH_BLOCK_SHIFT(ni)))

/* ONFI parameters */
#define NANDBCH_FEATURE(ni)		(((nandbch_info_t *)(ni))->onfi.feature)
#define NANDBCH_OPT_CMD(ni)		(((nandbch_info_t *)(ni))->onfi.opt_cmd)

#define NANDBCH_FEATURE_EXT_PARAM_PAGE	0x80
#define NANDBCH_OPT_CMD_GET_SET_FEATURE	0x4

/*******************************************************************************
* nandbch_config(ni, bank, addr_cycles, page, incr, iter) - Configure nand
* interface based on parameters stored on the NAND flash
* @ni:            [out] nand info structure
* @bank:	         [in] bank number
* @addr_cycles:	[in] address cycles
* @page:          [in/out] page number
* @incr:	         [in] page increment
* @iter:	         [in] mamimum number of iterations
*
* Attempts to retrive NAND configuration parameters from the specified bank
* and page. If unsuccessfull, it repeats the attempt up to the maximum number
* specified in the <iter> argument adding <incr> to the page number at each
* attempt. When it finds a valid config page, it updates the page number.
*
* The configuratuion page has the following informations:
* (LSB at lower offset)
* 
* Bytes        Content
* 0-3          "NAND" signature
* 4-7          Version number
* 8-11         Page size
* 12-15        ECC offset
* 16-17        Sector size
* 18           BCH ECC correction requirement (bits/sector)
*
* Additional parameters not used by BOOTROM:
*
* Bytes        Content
* 19           Auxiliary data BCH ECC correction requirement
* 20-23        Block size
* 24-27        Bank size (in MB)
* 28           Timing config selection
*                 0x00    default (ONFI timing mode 0)
*                 0x01    use ONFI timing mode from byte 29 and send ONFI
*                         SET_FEATURE command to device to change timing mode
*                 0x02    use ONFI timing mode from byte 29 only
*                 0x03    use NAND controller timing config register values
*                         (Capri encoding scheme)
* 29           ONFI timing mode [0..5]
* 30-45        NAND controller timing config register values
* 46-47        OOB size
*
* 48-511       Reserved
*
******************************************************************************/
uint32_t nandbch_config(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t addr_cycles,
	uint32_t * page,
	uint32_t incr,
	uint32_t iter);

/*******************************************************************************
* nandbch_init(ni, flags) - Initialize NAND interface
* @ni:            [in/out] nand info structure
* @flags:         [in] options
*
* By default it configures parameters in nandbch_info_t based on ONFI parameter
* page information.
* Default values for the aux_size is 2 bytes / 512 bytes of main data and no
* ECC correction is enabled for auxiliary data in the OOB.
* Calling nandbch_init with the NANDBCH_FLAG_GEOMETRY flag will force the use of the
* parameters passed in the geometry structure (minimum the bus_width and
* page_shift need to be set for NAND read only access).
* Calling nandbch_init with the NANDBCH_FLAG_BCH_CONFIG flag will force the use of
* the ECC parameters passed in the ecc structure (sector data_size and ecc_t
* need to be set).
* Calling nandbch_init with the NANDBCH_FLAG_AUX_CONFIG flag will force the use of
* the auxiliary data parameters passed in the aux structure (aux data_size and
* ecc_t need to be set).
* Calling nandbch_init with the NANDBCH_FLAG_TIMING flag will force the use of the 
* timing parameters passed in the timing structure. By default timing values
* are set as follows:
*     Hold_tH			Hold time:				20ns
*     Setup_tS			Setup time:				70ns
*     wr_cycle_tWH		Write high time:			45ns
*     wr_pulse_width_tWP	Write pulse width:			55ns
*     rd_cycle_tREH		Read high time:				45ns
*     rd_pulse_width_tRP	Read pulse width:			55ns
*     rTHZ			RE high to out imped time		75ns
*     tCEA-tREA			diff between CE and RE access time	60ns
*     tOE:			Time adj for output enable 
*				to control the sampling time:		10ns
*******************************************************************************/
uint32_t nandbch_init(
	nandbch_info_t * ni,
	uint8_t flags);

/*******************************************************************************
* nandbch_reset(ni, bank, buf) - Send reset command to the nand device
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
*******************************************************************************/
uint32_t nandbch_reset(
	nandbch_info_t * ni,
	uint8_t bank);

/*******************************************************************************
* nandbch_id_get(ni, bank, buf) - Get nand id
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @addr:	[in] address (0x0 or 0x20)
* @buf:		[out] buffer to get the id
*******************************************************************************/
uint32_t nandbch_id_get(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t addr,
	uint8_t len,
	uint8_t * buf);

/*******************************************************************************
* nandbch_page_read(ni, bank, page, buf, stats) - Reads one page from nand into buf
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[out] target buffer
* @stats:	[out] ECC stats (number of bad bits per sector)
*
* Note: If the caller wants ECC correction stats it needs to provide a pointer
* to an array stats[number of sectors per page]. The number of bits corrected
* by ECC in every sector will be returned in the corresponding array element.
* If stats are not needed caller should provide a NULL pointer argument.
*******************************************************************************/
uint32_t nandbch_page_read(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	nandbch_eccstats_t * stats);

/*******************************************************************************
* nandbch_page_read_ecc(ni, bank, page, buf, stats) - Reads one page from nand into
*                                                  buf with BCH-ECC correction
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[out] target buffer
* @stats:	[out] ECC stats (number of bad bits per sector)
*
* Note: If the caller wants ECC correction stats it needs to provide a pointer
* to an array stats[number of sectors per page]. The number of bits corrected
* by ECC in every sector will be returned in the corresponding array element.
* If stats are not needed caller should provide a NULL pointer argument.
*******************************************************************************/
uint32_t nandbch_page_read_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	nandbch_eccstats_t * stats);

/*******************************************************************************
* nandbch_page_read_raw(ni, bank, page, buf) - Reads one page from nand into buf
*                                           witout ECC corection
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[out] target buffer
*******************************************************************************/
uint32_t nandbch_page_read_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_enable(void) - Setup the pins and enables the NAND block
*******************************************************************************/
uint32_t nandbch_enable(
	void
    );

/*******************************************************************************
* nandbch_disable(void) - Disables the NAND block and releases the pins
*******************************************************************************/
uint32_t nandbch_disable(
	void
    );

/*******************************************************************************
* nandbch_aux_read_raw(ni, bank, page, buf) - Reads the auxiliary data from oob
*                                          without ECC correction
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[out] target buffer
*******************************************************************************/
uint32_t nandbch_aux_read_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_block_isbad(ni, bank, block, is_bad) - Checks if block is marked bad
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @block:	[in] block number
* @is_bad:	[out] 1 if block is marked bad, 0 if good
*******************************************************************************/
uint32_t nandbch_block_isbad(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t block,
	uint8_t * is_bad);

/*******************************************************************************
 * nandbch_status_get - gets the status for an individual bank
 * @ni:		[in] nand info structure
 * @bank:	[in] bank to get status
 * @status:	[out] nand status
*******************************************************************************/
uint32_t nandbch_status_get(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t * status);

/*******************************************************************************
* nandbch_aux_read(ni, bank, page, buf, stats) - Reads the auxiliary data from oob
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[out] target buffer
* @stats:	[out] ECC stats (number of bad bits)
*
* Note: If ECC correction stats are not needed caller should provide a NULL
* pointer argument.
*******************************************************************************/
uint32_t nandbch_aux_read(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	uint8_t * stats);

/*******************************************************************************
* nandbch_aux_read_ecc(ni, bank, page, buf, stats) - Reads the auxiliary data from
*                                                 oob with BCH-ECC correction
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[out] target buffer
* @stats:	[out] ECC stats (number of bad bits)
*
* Note: If ECC correction stats are not needed caller should provide a NULL
* pointer argument.
*******************************************************************************/
uint32_t nandbch_aux_read_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	uint8_t * stats);

/*******************************************************************************
* nandbch_page_write(ni, bank, page, buf) - Writes the page data from buf to the
*                                        nand device
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
* @raw:		[in] write ECC?
*******************************************************************************/
uint32_t nandbch_page_write(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf,
	int raw);

/******************************************************************************
* nandbch_aux_write (ni, bank, page, buf) - Writes the auxiliary data to oob
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
*******************************************************************************/
uint32_t nandbch_aux_write(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_page_aux_write(ni, bank, page, buf) - Write page data and auxiliary data
*                                            from buf to nand device
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
*******************************************************************************/
uint32_t nandbch_page_aux_write(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_page_write_raw(ni, bank, page, buf) - Writes page from buf to the nand
*                                            device, without ECC
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
*******************************************************************************/
uint32_t nandbch_page_write_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/******************************************************************************
* nandbch_aux_write_raw (ni, bank, page, buf) - Writes the auxiliary data to oob,
*                                            without ECC
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
*******************************************************************************/
uint32_t nandbch_aux_write_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_page_aux_write_raw(ni, bank, page, buf) - Write page and aux data from
*                                                buf to nand device, without ECC
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
*******************************************************************************/
uint32_t nandbch_page_aux_write_raw(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_page_write_ecc(ni, bank, page, buf) - Writes page from buf to the nand
*                                            device with BCH_ECC
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
*******************************************************************************/
uint32_t nandbch_page_write_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_aux_write_ecc(ni, bank, page, buf) - Writes auxiliary data from buf to
*                                           to the nand device with BCH_ECC
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
*******************************************************************************/
uint32_t nandbch_aux_write_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_page_aux_write_ecc(ni, bank, page, buf) - Writes page and auxiliary data
*                                                from buf to the nand device
*                                                with BCH_ECC
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @page:	[in] page number
* @buf:		[in] source buffer
*******************************************************************************/
uint32_t nandbch_page_aux_write_ecc(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t page,
	uint8_t * buf);

/*******************************************************************************
* nandbch_block_force_erase(ni, bank, block) - Erase block ignoring bad block marks
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @block:	[in] block number
*******************************************************************************/
uint32_t nandbch_block_force_erase(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t block);

/*******************************************************************************
* nandbch_block_erase(ni, bank, block)  - Erase a block
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @block:	[in] block number
*******************************************************************************/
uint32_t nandbch_block_erase(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t block);

/*******************************************************************************
* nandbch_block_markbad(ni, bank, block) - Erase block and write the bad block
*                                       marker in oob
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @block:	[in] block number
*******************************************************************************/
uint32_t nandbch_block_markbad(
	nandbch_info_t * ni,
	uint8_t bank,
	uint32_t block);

/*******************************************************************************
* nandbch_feature_get(ni, bank, feature, buf) - Get ONFI feature
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @feature:	[in] feature address
* @buf:		[out] buffer to get the feature value (4 bytes)
*******************************************************************************/
uint32_t nandbch_feature_get(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t feature,
	uint8_t * buf);

/*******************************************************************************
* nandbch_feature_set(ni, bank, feature, buf) - Set ONFI feature
* @ni:		[in] nand info structure
* @bank:	[in] bank number
* @feature:	[in] feature address
* @buf:		[out] buffer containing the feature value (4 bytes)
*******************************************************************************/
uint32_t nandbch_feature_set(
	nandbch_info_t * ni,
	uint8_t bank,
	uint8_t feature,
	uint8_t * buf);

/*******************************************************************************
* nandbch_oob_read(ni, bank, page, buf) - Reads used oob bytes (aux data, aux data
*                                      ECC bytes and main data ECC bytes) from
*                                      nand into buf without ECC correction
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [out] target buffer
*******************************************************************************/ 
uint32_t nandbch_oob_read(nandbch_info_t * ni, uint8_t bank, uint32_t page, uint8_t * buf);

/*******************************************************************************
* nandbch_oob_write(ni, bank, page, buf) - Write oob bytes (aux data, aux data
*                                      ECC bytes and main data ECC bytes) to
*                                      nand from buf without ECC correction
* @ni:      [in] nand info structure
* @bank:	   [in] bank number
* @page:    [in] page number
* @buf:     [in] target buffer
*******************************************************************************/ 
uint32_t nandbch_oob_write(nandbch_info_t * ni, uint8_t bank, uint32_t page, uint8_t * buf);

/*******************************************************************************
* nandbch_page_read_ileaved(ni, page0, page1, buf0, buf1, stats0, stats1) - Reads
* one page from each bank (CS iterleaved mode)
* @ni:		[in] nand info structure
* @page0:	[in] page number bank 0
* @page1:	[in] page number bank 1
* @buf0:	[out] target page buffer bank 0
* @buf1:	[out] target page buffer bank 1
* @stats0:	[out] ECC stats bank 0 (number of bad bits per sector)
* @stats0:	[out] ECC stats bank 1 (number of bad bits per sector)
*
* Note: If the caller wants ECC correction stats it needs to provide a pointer
* to an array stats[number of sectors per page]. The number of bits corrected
* by ECC in every sector will be returned in the corresponding array element.
* If stats are not needed caller should provide a NULL pointer argument.
*******************************************************************************/
uint32_t nandbch_page_read_ileaved(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1,
	nandbch_eccstats_t * stats0,
	nandbch_eccstats_t * stats1);

/*******************************************************************************
* nandbch_page_read_ileaved_ecc(ni, page0, page1, buf0, buf1, stats0, stats1) - Reads
* one page from each bank with ECC correction (CS iterleaved mode)
* @ni:	[in] nand info structure
* @page0:	[in] page number bank 0
* @page1:	[in] page number bank 1
* @buf0:	[out] target page buffer bank 0
* @buf1:	[out] target page buffer bank 1
* @stats0:	[out] ECC stats bank 0 (number of bad bits per sector)
* @stats0:	[out] ECC stats bank 1 (number of bad bits per sector)
*
* Note: If the caller wants ECC correction stats it needs to provide a pointer
* to an array stats[number of sectors per page]. The number of bits corrected
* by ECC in every sector will be returned in the corresponding array element.
* If stats are not needed caller should provide a NULL pointer argument.
*******************************************************************************/
uint32_t nandbch_page_read_ileaved_ecc(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1,
	nandbch_eccstats_t * stats0,
	nandbch_eccstats_t * stats1);

/*******************************************************************************
* nandbch_page_read_ileaved_raw(ni, page0, page1, buf0, buf1 - Reads one page from
* each bank without ECC correction (CS iterleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] target page buffer bank 0
* @buf1:    [out] target page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_read_ileaved_raw(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1);

/*******************************************************************************
* nandbch_block_force_erase_ileaved(ni, block0, block1) - Erase one block from each
* bank ignoring bad block marks (CS interleaved mode)
* @ni:		[in] nand info structure
* @block0:	[in] block number from bank 0
* @block1:	[in] block number from bank 1
*******************************************************************************/
uint32_t nandbch_block_force_erase_ileaved(
	nandbch_info_t * ni,
	uint32_t block0,
	uint32_t block1);

/*******************************************************************************
* nandbch_block_erase_ileaved(ni, block0, block1)  - Erase a block from each bank
* (CS interleaved mode)
* @ni:		[in] nand info structure
* @block0:	[in] block number from bank 0
* @block1:	[in] block number from bank 1
*******************************************************************************/
uint32_t nandbch_block_erase_ileaved(
	nandbch_info_t * ni,
	uint32_t block0,
	uint32_t block1);

/*******************************************************************************
* nandbch_page_write_ileaved(ni, page0, page1, buf0, buf1) - Writes one page to
* each bank (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_write_ileaved(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1);

/*******************************************************************************
* nandbch_page_write_ileaved_raw(ni, page0, page1, buf0, buf1) - Writes one page to
* each bank without ECC (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_write_ileaved_raw(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1);

/*******************************************************************************
* nandbch_page_write_ileaved_ecc(ni, page0, page1, buf0, buf1) - Writes one page to
* each bank with ECC (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_write_ileaved_ecc(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1);

/*******************************************************************************
* nandbch_page_aux_write_ileaved(ni, page0, page1, buf0, buf1) - Writes one page 
* and aux data to each bank (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_aux_write_ileaved(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1);

/*******************************************************************************
* nandbch_page_aux_write_ileaved_raw(ni, page0, page1, buf0, buf1) - Writes one
* page and aux data to each bank without ECC (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_aux_write_ileaved_raw(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1);

/*******************************************************************************
* nandbch_page_aux_write_ileaved_ecc(ni, page0, page1, buf0, buf1) - Writes one
* page and aux data to each bank with ECC (CS interleaved mode)
* @ni:      [in] nand info structure
* @page0:   [in] page number bank 0
* @page1:   [in] page number bank 1
* @buf0:    [out] source page buffer bank 0
* @buf1:    [out] source page buffer bank 1
*******************************************************************************/
uint32_t nandbch_page_aux_write_ileaved_ecc(
	nandbch_info_t * ni,
	uint32_t page0,
	uint32_t page1,
	uint8_t * buf0,
	uint8_t * buf1);

#endif
