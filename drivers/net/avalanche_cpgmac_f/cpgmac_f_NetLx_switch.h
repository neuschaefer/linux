/*
 *
 * cpgmac_f_NetLxCfg.h
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


/** \file   cpmacNetLx.h
    \brief  CPMAC Linux DDA Header file

    This file is a header for  the device driver adaptation for Linux for
    CPMAC/CPGMAC device based upon PSP Framework architecture.

    Acknowledgements: This DDA implementation for CP(G)MAC device is based upon
    Linux device driver for CP(G)MAC written using HAL 2.0.


    @author     Suraj Iyer - Original Linux Driver for CPMAC
                Anant Gole - Updated as per PSPF architecture (converted to DDA)
    @version    1.1
                Sharath Kumar - Incorporated Review comments.
 */

#ifndef _CPMAC_LX_DDA_H_
#define _CPMAC_LX_DDA_H_

#include "ddc_cpgmac_f.h"
#include "cpgmac_f_NetLxCfg.h"

extern int cpmac_debug_mode;

#define dbgPrint(format,args...) if (cpmac_debug_mode) printk(format,##args)
#define errPrint printk

/* Misc Error codes */
#define CPMAC_DDA_INTERNAL_FAILURE  -1

/* LED codes required for controlling LED's */
#define CPMAC_LINK_OFF          0
#define CPMAC_LINK_ON           1
#define CPMAC_SPEED_100         2
#define CPMAC_SPEED_10          3
#define CPMAC_FULL_DPLX         4
#define CPMAC_HALF_DPLX         5
#define CPMAC_TX_ACTIVITY       6
#define CPMAC_RX_ACTIVITY       7

/**
 * \brief  CPMAC (DDA) Private Ioctl Structure
 *
 * Private Ioctl commands provided by the CPMAC Linux Driver use this structure
 */
typedef struct {
    unsigned int cmd;       /**< Command */
    void *data;             /**< Data provided with the command - depending upon command */
} CpmacDrvPrivIoctl;

/**
 * \brief  CPMAC DDA maintained statistics
 *
 * Driver maintained statistics (apart from Hardware statistics)
 */
typedef struct {
    unsigned long tx_discards;      /**< TX Discards */
    unsigned long rx_discards;      /**< RX Discards */
    unsigned long start_tick;       /**< Start tick */
} CpmacDrvStats;

/**
 * \brief CPMAC Private data structure
 *
 * Each CPMAC device maintains its own private data structure and has a pointer
 * to the net_device data structure representing the instance with the kernel.
 * The private data structure contains a "owner" member pointing to the net_device
 * structure and the net_device data structure's "priv" member points back to this
 * data structure.
 */
typedef struct _CpmacNetDevice {
    void *owner;                            /**< Pointer to the net_device structure */
    unsigned int instanceNum;               /**< Instance Number of the device */

#ifdef CPGMAC_PORTSEG_TASKLET_MODE
    struct tasklet_struct tx_tasklet;       /**< Tasklet structure if processing packets in tasklets */
    struct tasklet_struct rx_tasklet;       /**< Tasklet structure if processing rx packets in tasklets */
#endif

    struct net_device *nextDevice;          /**< Next device pointer - for internal use */
    unsigned int linkSpeed;                 /**< Link Speed */
    unsigned int linkMode;                  /**< Link Mode */
    unsigned long setToClose;               /**< Flag to indicate closing of device */
    void *ledHandle;                        /**< Handle for LED control */

    /* DDC related parameters */
    CpmacDDCObj *hDDC;                      /**< Handle (pointer) to Cpmac DDC object */
    CpmacDDCIf *ddcIf;                      /**< Handle (pointer) to Cpmac DDC function table */
    CpmacDDCStatus ddcStatus;               /**< Cpmac DDC data structure */

    /* Configuration parameters */
    char macAddr[6];                       /**< Mac (ethernet) address */
    CpmacInitConfig initCfg;                /**< Init cfg parameters - contains rx and mac cfg structures */
    /* Not required now as the buffer are allocated at once to form pool and that too by PALCPPI4 code */
    unsigned int rxBufSize;                /**< RX Buffer Size - skb size to be allocated for RX*/
    unsigned int rxBufOffset;              /**> RX Buffer Offset - extra bytes to be reserved before RX data begins in a skb*/
    /* TODO: VLAN TX not supported as of now */
    BOOL vlanEnable;                        /**< Vlan enable (TX: 8 priority ch's, RX: 1514 byte frames accepted) */
    /* Channel configuration - though only 1 TX/RX channel is supported  */
    CpmacChInfo txChInfo;            /**< Tx Channel configuration */
    CpmacChInfo rxChInfo;            /**< Rx Channel configuration */

    /* Periodic Timer required for DDC (MDIO) polling */
    struct timer_list periodicTimer;        /**< Periodic timer required for DDC (MDIO) polling */
    Uint32 periodicTicks;                   /**< Ticks for this timer */
    BOOL timerActive;                       /**< Periodic timer active ??? */

    struct timer_list mibTimer;        /**< Periodic timer required for 64 bit MIB counter support */
    Uint32 mibTicks;                     /**< Ticks for this timer */
    BOOL mibTimerActive;                       /**< Periodic timer active ??? */


    /* Statistics */
    CpmacHwStatistics deviceMib;            /**< Device MIB - CPMAC hardware statistics counters */
    CpmacDrvStats deviceStats;              /**< Device Statstics */
    struct net_device_stats netDevStats[4];         /**< Linux Network Device statistics */

    /* Statistics counters for debugging */
    Uint32 txisrCount;                       /**< Number of Tx interrupts */
    Uint32 rxisrCount;                       /**< Number of Rx interrupts */

    void *virtOwners[4];        /**< Virtual owners */

    Uint32 mpAllocSize[4];                                     /**< Tx BD pool allocated memory size */
    char *mpMem[4];                                          /**< MP Memory pointer */
    MARVELL_INGRESS_PORT_INFO_T *marvell_ingress_port_free_list[4];              /**< Free MP info Pool Head */


    Uint8 deviceStatus;
    Uint8 promiscStatus[4];

    Uint32 Clear_EOI;
    /* TxRxParam struct added */
    RxTxParams napiRxTx;
} CpmacNetDevice;

/* Function prototypes - only those functions that are in different C files and need
 * to be referenced from the other sources\
 */
int cpmac_dev_tx(struct sk_buff *skb, struct net_device *p_dev);

irqreturn_t cpmac_hal_tx_isr(int irq, void *dev_id, struct pt_regs *regs);
irqreturn_t cpmac_hal_rx_isr(int irq, void *dev_id, struct pt_regs *regs);

Ptr DDA_cpmac_net_alloc_rx_buf(CpmacNetDevice * hDDA, Int bufSize,
                               DDC_NetDataToken * dataToken,
                               Uint32 channel, Ptr allocArgs);
PAL_Result DDA_cpmac_net_free_rx_buf(CpmacNetDevice * hDDA, Ptr buffer,
                                     DDC_NetDataToken dataToken,
                                     Uint32 channel, Ptr freeArgs);

PAL_Result DDA_cpmac_net_rx(CpmacNetDevice * hDDA, DDC_NetPktObj * pkt,
                            Ptr rxArgs, Ptr arg);

PAL_Result DDA_cpmac_net_tx_complete(CpmacNetDevice * hDDA,
                                     DDC_NetDataToken * netDataTokens,
                                     Int numTokens, Uint32 channel);

int cpmac_get_port_id(struct net_device *p_dev);

#ifdef CPGMAC_PORTSEG_TASKLET_MODE
void cpmac_handle_tx_tasklet(unsigned long data);
void cpmac_handle_rx_tasklet(unsigned long data);
#endif

#endif /* _CPMAC_LX_DDA_H_ */
