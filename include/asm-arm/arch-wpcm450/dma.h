/* linux/include/asm-arm/arch-bast/dma.h
 *
 * Copyright (C) 2003,2004 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Samsung WPCM450X DMA support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *  ??-May-2003 BJD   Created file
 *  ??-Jun-2003 BJD   Added more dma functionality to go with arch
 *  10-Nov-2004 BJD   Added sys_device support
*/

#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H __FILE__

#include <linux/autoconf.h>
#include <linux/sysdev.h>
#include "hardware.h"


/*
 * This is the maximum DMA address(physical address) that can be DMAd to.
 *
 */
#define MAX_DMA_ADDRESS		0x20000000
#define MAX_DMA_TRANSFER_SIZE   0x100000 /* Data Unit is half word  */


/* we have 4 dma channels */
#define WPCM450_DMA_CHANNELS        (4)

/* types */

typedef enum {
	WPCM450_DMA_IDLE,
	WPCM450_DMA_RUNNING,
	WPCM450_DMA_PAUSED
} wpcm450_dma_state_t;


/* wpcm450_dma_loadst_t
 *
 * This represents the state of the DMA engine, wrt to the loaded / running
 * transfers. Since we don't have any way of knowing exactly the state of
 * the DMA transfers, we need to know the state to make decisions on wether
 * we can
 *
 * WPCM450_DMA_NONE
 *
 * There are no buffers loaded (the channel should be inactive)
 *
 * WPCM450_DMA_1LOADED
 *
 * There is one buffer loaded, however it has not been confirmed to be
 * loaded by the DMA engine. This may be because the channel is not
 * yet running, or the DMA driver decided that it was too costly to
 * sit and wait for it to happen.
 *
 * WPCM450_DMA_1RUNNING
 *
 * The buffer has been confirmed running, and not finisged
 *
 * WPCM450_DMA_1LOADED_1RUNNING
 *
 * There is a buffer waiting to be loaded by the DMA engine, and one
 * currently running.
*/

typedef enum {
	WPCM450_DMALOAD_NONE,
	WPCM450_DMALOAD_1LOADED,
	WPCM450_DMALOAD_1RUNNING,
	WPCM450_DMALOAD_1LOADED_1RUNNING,
} wpcm450_dma_loadst_t;

typedef enum {
	WPCM450_RES_OK,
	WPCM450_RES_ERR,
	WPCM450_RES_ABORT
} wpcm450_dma_buffresult_t;


typedef enum wpcm450_dmasrc_e wpcm450_dmasrc_t;

enum wpcm450_dmasrc_e {
	WPCM450_DMASRC_HW,      /* source is memory */
	WPCM450_DMASRC_MEM      /* source is hardware */
};

/* enum wpcm450_chan_op_e
 *
 * operation codes passed to the DMA code by the user, and also used
 * to inform the current channel owner of any changes to the system state
*/

enum wpcm450_chan_op_e {
	WPCM450_DMAOP_START,
	WPCM450_DMAOP_STOP,
	WPCM450_DMAOP_PAUSE,
	WPCM450_DMAOP_RESUME,
	WPCM450_DMAOP_FLUSH,
	WPCM450_DMAOP_TIMEOUT,           /* internal signal to handler */
};

typedef enum wpcm450_chan_op_e wpcm450_chan_op_t;

/* flags */

#define WPCM450_DMAF_SLOW         (1<<0)   /* slow, so don't worry about
					    * waiting for reloads */
#define WPCM450_DMAF_AUTOSTART    (1<<1)   /* auto-start if buffer queued */

/* dma buffer */

typedef struct wpcm450_dma_buf_s wpcm450_dma_buf_t;

struct wpcm450_dma_client {
	char                *name;
};

typedef struct wpcm450_dma_client wpcm450_dma_client_t;

/* wpcm450_dma_buf_s
 *
 * internally used buffer structure to describe a queued or running
 * buffer.
*/

struct wpcm450_dma_buf_s {
	wpcm450_dma_buf_t   *next;
	int                  magic;        /* magic */
	int                  size;         /* buffer size in bytes */
	dma_addr_t           data;         /* start of DMA data */
	dma_addr_t           ptr;          /* where the DMA got to [1] */
	void                *id;           /* client's id */
};

/* [1] is this updated for both recv/send modes? */

typedef struct wpcm450_dma_chan_s wpcm450_dma_chan_t;

/* wpcm450_dma_cbfn_t
 *
 * buffer callback routine type
*/

typedef void (*wpcm450_dma_cbfn_t)(wpcm450_dma_chan_t *, void *buf, int size,
				   wpcm450_dma_buffresult_t result);

typedef int  (*wpcm450_dma_opfn_t)(wpcm450_dma_chan_t *,
				   wpcm450_chan_op_t );

struct wpcm450_dma_stats_s {
	unsigned long          loads;
	unsigned long          timeout_longest;
	unsigned long          timeout_shortest;
	unsigned long          timeout_avg;
	unsigned long          timeout_failed;
};

typedef struct wpcm450_dma_stats_s wpcm450_dma_stats_t;

/* struct wpcm450_dma_chan_s
 *
 * full state information for each DMA channel
*/

struct wpcm450_dma_chan_s {
	/* channel state flags and information */
	unsigned char          number;      /* number of this dma channel */
	unsigned char          in_use;      /* channel allocated */
	unsigned char          irq_claimed; /* irq claimed for channel */
	unsigned char          irq_enabled; /* irq enabled for channel */
	unsigned char          xfer_unit;   /* size of an transfer */

	/* channel state */

	wpcm450_dma_state_t    state;
	wpcm450_dma_loadst_t   load_state;
	wpcm450_dma_client_t  *client;

	/* channel configuration */
	wpcm450_dmasrc_t       source;
	unsigned long          dev_addr;
	unsigned long          load_timeout;
	unsigned int           flags;        /* channel flags */

	/* channel's hardware position and configuration */
	void __iomem           *regs;        /* channels registers */
	void __iomem           *addr_reg;    /* data address register */
	unsigned int           irq;          /* channel irq */
	unsigned long          dcon;         /* default value of DCON */

	/* driver handles */
	wpcm450_dma_cbfn_t     callback_fn;  /* buffer done callback */
	wpcm450_dma_opfn_t     op_fn;        /* channel operation callback */

	/* stats gathering */
	wpcm450_dma_stats_t   *stats;
	wpcm450_dma_stats_t    stats_store;

	/* buffer list and information */
	wpcm450_dma_buf_t      *curr;        /* current dma buffer */
	wpcm450_dma_buf_t      *next;        /* next buffer to load */
	wpcm450_dma_buf_t      *end;         /* end of queue */

	/* system device */
	struct sys_device	dev;
};

/* the currently allocated channel information */
extern wpcm450_dma_chan_t wpcm450_chans[];

/* note, we don't really use dma_device_t at the moment */
typedef unsigned long dma_device_t;

/* functions --------------------------------------------------------------- */

/* wpcm450_dma_request
 *
 * request a dma channel exclusivley
*/

extern int wpcm450_dma_request(dmach_t channel,
			       wpcm450_dma_client_t *, void *dev);


/* wpcm450_dma_ctrl
 *
 * change the state of the dma channel
*/

extern int wpcm450_dma_ctrl(dmach_t channel, wpcm450_chan_op_t op);

/* wpcm450_dma_setflags
 *
 * set the channel's flags to a given state
*/

extern int wpcm450_dma_setflags(dmach_t channel,
				unsigned int flags);

/* wpcm450_dma_free
 *
 * free the dma channel (will also abort any outstanding operations)
*/

extern int wpcm450_dma_free(dmach_t channel, wpcm450_dma_client_t *);

/* wpcm450_dma_enqueue
 *
 * place the given buffer onto the queue of operations for the channel.
 * The buffer must be allocated from dma coherent memory, or the Dcache/WB
 * drained before the buffer is given to the DMA system.
*/

extern int wpcm450_dma_enqueue(dmach_t channel, void *id,
			       dma_addr_t data, int size);

/* wpcm450_dma_config
 *
 * configure the dma channel
*/

extern int wpcm450_dma_config(dmach_t channel, int xferunit, int dcon);

/* wpcm450_dma_devconfig
 *
 * configure the device we're talking to
*/

extern int wpcm450_dma_devconfig(int channel, wpcm450_dmasrc_t source,
				 int hwcfg, unsigned long devaddr);

/* wpcm450_dma_getposition
 *
 * get the position that the dma transfer is currently at
*/

extern int wpcm450_dma_getposition(dmach_t channel,
				   dma_addr_t *src, dma_addr_t *dest);

extern int wpcm450_dma_set_opfn(dmach_t, wpcm450_dma_opfn_t rtn);
extern int wpcm450_dma_set_buffdone_fn(dmach_t, wpcm450_dma_cbfn_t rtn);

/* DMA Register definitions */

#define WPCM450_DMA_DISRC       (0x00)
#define WPCM450_DMA_DISRCC      (0x04)
#define WPCM450_DMA_DIDST       (0x08)
#define WPCM450_DMA_DIDSTC      (0x0C)
#define WPCM450_DMA_DCON        (0x10)
#define WPCM450_DMA_DSTAT       (0x14)
#define WPCM450_DMA_DCSRC       (0x18)
#define WPCM450_DMA_DCDST       (0x1C)
#define WPCM450_DMA_DMASKTRIG   (0x20)

#define WPCM450_DISRCC_INC	(1<<0)
#define WPCM450_DISRCC_APB	(1<<1)

#define WPCM450_DMASKTRIG_STOP   (1<<2)
#define WPCM450_DMASKTRIG_ON     (1<<1)
#define WPCM450_DMASKTRIG_SWTRIG (1<<0)

#define WPCM450_DCON_DEMAND     (0<<31)
#define WPCM450_DCON_HANDSHAKE  (1<<31)
#define WPCM450_DCON_SYNC_PCLK  (0<<30)
#define WPCM450_DCON_SYNC_HCLK  (1<<30)

#define WPCM450_DCON_INTREQ     (1<<29)

#define WPCM450_DCON_CH0_XDREQ0	(0<<24)
#define WPCM450_DCON_CH0_UART0	(1<<24)
#define WPCM450_DCON_CH0_SDI	(2<<24)
#define WPCM450_DCON_CH0_TIMER	(3<<24)
#define WPCM450_DCON_CH0_USBEP1	(4<<24)

#define WPCM450_DCON_CH1_XDREQ1	(0<<24)
#define WPCM450_DCON_CH1_UART1	(1<<24)
#define WPCM450_DCON_CH1_I2SSDI	(2<<24)
#define WPCM450_DCON_CH1_SPI	(3<<24)
#define WPCM450_DCON_CH1_USBEP2	(4<<24)

#define WPCM450_DCON_CH2_I2SSDO	(0<<24)
#define WPCM450_DCON_CH2_I2SSDI	(1<<24)
#define WPCM450_DCON_CH2_SDI	(2<<24)
#define WPCM450_DCON_CH2_TIMER	(3<<24)
#define WPCM450_DCON_CH2_USBEP3	(4<<24)

#define WPCM450_DCON_CH3_UART2	(0<<24)
#define WPCM450_DCON_CH3_SDI	(1<<24)
#define WPCM450_DCON_CH3_SPI	(2<<24)
#define WPCM450_DCON_CH3_TIMER	(3<<24)
#define WPCM450_DCON_CH3_USBEP4	(4<<24)

#define WPCM450_DCON_SRCSHIFT   (24)
#define WPCM450_DCON_SRCMASK	(7<<24)

#define WPCM450_DCON_BYTE       (0<<20)
#define WPCM450_DCON_HALFWORD   (1<<20)
#define WPCM450_DCON_WORD       (2<<20)

#define WPCM450_DCON_AUTORELOAD (0<<22)
#define WPCM450_DCON_NORELOAD   (1<<22)
#define WPCM450_DCON_HWTRIG     (1<<23)

#ifdef CONFIG_CPU_S3C2440
#define S3C2440_DIDSTC_CHKINT	(1<<2)

#define S3C2440_DCON_CH0_I2SSDO	(5<<24)
#define S3C2440_DCON_CH0_PCMIN	(6<<24)

#define S3C2440_DCON_CH1_PCMOUT	(5<<24)
#define S3C2440_DCON_CH1_SDI	(6<<24)

#define S3C2440_DCON_CH2_PCMIN	(5<<24)
#define S3C2440_DCON_CH2_MICIN	(6<<24)

#define S3C2440_DCON_CH3_MICIN	(5<<24)
#define S3C2440_DCON_CH3_PCMOUT	(6<<24)
#endif

#endif /* __ASM_ARCH_DMA_H */
