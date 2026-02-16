/*
 *
 * cslr_cpgmac_f.h 
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


/** \file   cslr_cpgmac_f.h
    \brief  CPGMAC_F CSL Header - Register Layer Abstraction

    Register layer abstraction of CPGMAC_F peripheral device.

    @author     Greg Guyotte
    @version    0.1
 */


#ifndef __CSLR_CPGMAC_F_H__
#define __CSLR_CPGMAC_F_H__

#include "csl_defs.h"

/*#define AVALANCHE_CPMAC_HW_MODULE_REV 0x00500100*/ /* Temp location. */

/**
 * \brief CPGMAC_F Peripheral Device Register Memory Layout structure
 *
 * The structure instance variable points to CPGMAC_F register space in
 * SOC memory map directly.
 * This is a template only, no memory is ever allocated for this!
 */
typedef struct  
{
  CSL_Reg32   IdVer;
  CSL_Reg32   Mac_In_Vector;
  CSL_Reg32   Mac_EOI_Vector;
  CSL_Reg32   Mac_IntStat_Raw;
  CSL_Reg32   Mac_IntStat_Masked;
  CSL_Reg32   Mac_IntMask_Set;
  CSL_Reg32   Mac_IntMask_Clear;
  CSL_Reg32   Rx_MBP_Enable;
  CSL_Reg32   Rx_Unicast_Set;
  CSL_Reg32   Rx_Unicast_Clear;
  CSL_Reg32   Rx_Maxlen;
  CSL_Reg32   MacControl;
  CSL_Reg32   MacStatus;
  CSL_Reg32   EMControl;
  CSL_Reg32   FifoControl;
  CSL_Reg32   Mac_Cfig;
  CSL_Reg32   Soft_Reset;
  CSL_Reg32   MacSrcAddr_Lo;
  CSL_Reg32   MacSrcAddr_Hi;
  CSL_Reg32   MacHash1;
  CSL_Reg32   MacHash2;
  CSL_Reg32   BoffTest;
  CSL_Reg32   Tpace_Test;
  CSL_Reg32   Rx_Pause;
  CSL_Reg32   Tx_Pause;
  CSL_Reg32   Port_VLAN;
  CSL_Reg32   Rx_Flowthresh;
  CSL_Reg32   Reserved2[101];      /* Padding for holes in memory map */
  CSL_Reg32   RxGoodFrames;
  CSL_Reg32   RxBroadcastFrames;
  CSL_Reg32   RxMulticastFrames;
  CSL_Reg32   RxPauseFrames;
  CSL_Reg32   RxCRCErrors;
  CSL_Reg32   RxAlignCodeErrors;
  CSL_Reg32   RxOversizedFrames;
  CSL_Reg32   RxJabberFrames;
  CSL_Reg32   RxUndersizedFrames;
  CSL_Reg32   RxFragments;
  CSL_Reg32   RxFilteredFrames;
  CSL_Reg32   Reserved3;           /* Padding for holes in memory map */
  CSL_Reg32   RxOctets;
  CSL_Reg32   TxGoodFrames;
  CSL_Reg32   TxBroadcastFrames;
  CSL_Reg32   TxMulticastFrames;
  CSL_Reg32   TxPauseFrames;
  CSL_Reg32   TxDeferredFrames;
  CSL_Reg32   TxCollisionFrames;
  CSL_Reg32   TxSingleCollFrames;
  CSL_Reg32   TxMultCollFrames;
  CSL_Reg32   TxExcessiveCollisions;
  CSL_Reg32   TxLateCollisions;
  CSL_Reg32   TxUnderrun;
  CSL_Reg32   TxCarrierSenseErrors;
  CSL_Reg32   TxOctets;
  CSL_Reg32   Reg64octetFrames;
  CSL_Reg32   Reg65t127octetFrames;
  CSL_Reg32   Reg128t255octetFrames;
  CSL_Reg32   Reg256t511octetFrames;
  CSL_Reg32   Reg512t1023octetFrames;
  CSL_Reg32   Reg1024tUPoctetFrames;
  CSL_Reg32   NetOctets;
  CSL_Reg32   RxSofOverruns;
  CSL_Reg32   RxMofOverruns;
  CSL_Reg32   RxDmaOverruns;
  CSL_Reg32   Reserved4[28];       /* Padding for holes in memory map */
  CSL_Reg32   RX_FIFO_Processor_Test_Access[64];
  CSL_Reg32   TX_FIFO_Processor_Test_Access[64];
  CSL_Reg32   MacAddr_Lo;
  CSL_Reg32   MacAddr_Hi;
  CSL_Reg32   QueueInfo;
  CSL_Reg32   MacIndex;
  CSL_Reg32   Reserved5[127];      /* Padding for holes in memory map */
} CSL_Cpgmac_f_Regs;

/*
 * \brief CPGMAC_F register overlay pointer
 *
 * Can be used in DDC layer directly for performance considerations.
 */
typedef volatile CSL_Cpgmac_f_Regs *CSL_Cpgmac_f_RegsOvly;

/**
 *  \brief CPGMAC_F Peripheral Device Register Enumerations
 *
 *  Register Enumerations for CPGMAC_F peripheral
 */
typedef enum  
{
    IdVer,
    Mac_In_Vector,
    Mac_EOI_Vector,
    Mac_IntStat_Raw,
    Mac_IntStat_Masked,
    Mac_IntMask_Set,
    Mac_IntMask_Clear,
    Rx_MBP_Enable,
    Rx_Unicast_Set,
    Rx_Unicast_Clear,
    Rx_Maxlen,
    MacControl,
    MacStatus,
    EMControl,
    FifoControl,
    Mac_Cfig,
    Soft_Reset,
    MacSrcAddr_Lo,
    MacSrcAddr_Hi,
    MacHash1,
    MacHash2,
    BoffTest,
    Tpace_Test,
    Rx_Pause,
    Tx_Pause,
    Port_VLAN,
    Rx_Flowthresh,
    RxGoodFrames = 128,
    RxBroadcastFrames,
    RxMulticastFrames,
    RxPauseFrames,
    RxCRCErrors,
    RxAlignCodeErrors,
    RxOversizedFrames,
    RxJabberFrames,
    RxUndersizedFrames,
    RxFragments,
    RxFilteredFrames,
    RxOctets = 140,
    TxGoodFrames,
    TxBroadcastFrames,
    TxMulticastFrames,
    TxPauseFrames,
    TxDeferredFrames,
    TxCollisionFrames,
    TxSingleCollFrames,
    TxMultCollFrames,
    TxExcessiveCollisions,
    TxLateCollisions,
    TxUnderrun,
    TxCarrierSenseErrors,
    TxOctets,
    Reg64octetFrames,
    Reg65t127octetFrames,
    Reg128t255octetFrames,
    Reg256t511octetFrames,
    Reg512t1023octetFrames,
    Reg1024tUPoctetFrames,
    NetOctets,
    RxSofOverruns,
    RxMofOverruns,
    RxDmaOverruns,
    RX_FIFO_Processor_Test_Access = 192,
    TX_FIFO_Processor_Test_Access = 256,
    MacAddr_Lo,
    MacAddr_Hi,
    MacIndex
} CSL_Cpgmac_f_RegIds;

/**
 *  \brief CPGMAC_F Addressing Type
 *
 *  Addressing type based upon cfig register. For CPGMAC_F peripheral cfig register 
 *  reads a value of 0
 *  i.e Type 0 addressing
 */
typedef enum
{   
    RX_ADDR_TYPE0 = 0,      /**< Type 0 addressing - old style used in (CPMAC) */
    RX_ADDR_TYPE1 = 1,      /**< Type 1 addressing - new CPGMAC style */
    RX_ADDR_TYPE2 = 2,      /**< Type 2 addressing - new CPGMAC "filtering" style */
    RX_ADDR_TYPE3 = 3       /**< TODO: Type 3 addressing  - new CPGMAC "filtering" style */
} Cpgmac_f_RxAddrType;
    

/* 
 * The following are CPMAC registers which have been removed from the CPGMAC
 * register map. Thus we access them using macros to avoid having more CSL register
 * overlay structures for older CPMAC register map.
 */

/* Make sure REG32_DATA is not defined before */
/*
#undef REG32_DATA

#define REG32_DATA(addr)                        (*(volatile Uint32 *)(addr))
#define CPMAC_MACADDRLO(baseAddr, chNum)        REG32_DATA((baseAddr) + 0x1B0 + ((chNum) * 4))
#define CPMAC_MACADDRMID(baseAddr)              REG32_DATA((baseAddr) + 0x1D0)
#define CPMAC_MACADDRHI(baseAddr)               REG32_DATA((baseAddr) + 0x1D4)
*/

/* Statistics clear value */
#define CPMAC_NUM_STAT_REGS                     36
#define CPMAC_STAT_CLEAR                        0xFFFFFFFF

/* CPMAC All multicast set register value */
#define CPMAC_ALL_MULTI_REG_VALUE               0xFFFFFFFF

/* CPMAC number of Multicast bits that can be set/cleared - currently 64 bits - hash1/2 regs */
#define CPMAC_NUM_MULTICAST_BITS                64

/* CPMAC Teardown Value */
/* GSG 
#define CPMAC_TEARDOWN_VALUE                    0xFFFFFFFC*/

/* TX / RX Control bits */
/* GSG
#define CPMAC_TX_CONTROL_TX_ENABLE_VAL          0x1
#define CPMAC_RX_CONTROL_RX_ENABLE_VAL          0x1
*/

/* Host interrupt bits */
#define CPMAC_MAC_HOST_ERR_INTMASK_VAL          0x2
#define CPMAC_MAC_STAT_INT_INTMASK_VAL          0x1

/* Rx config masks */
#define CPMAC_RX_UNICAST_CLEAR_ALL              0xFF

/* Type 0 Address filtering Macros */
#define CSL_CPMAC_TYPE_0_MACSRCADDR0_MASK                    (0xFF)
#define CSL_CPMAC_TYPE_0_MACSRCADDR0_SHIFT                   0
#define CSL_CPMAC_TYPE_0_MACSRCADDR1_MASK                    (0xFF)
#define CSL_CPMAC_TYPE_0_MACSRCADDR1_SHIFT                   0

#define CSL_CPMAC_TYPE_0_MACSRCADDR2_MASK                    (0xFF<<24)
#define CSL_CPMAC_TYPE_0_MACSRCADDR2_SHIFT                   24
#define CSL_CPMAC_TYPE_0_MACSRCADDR3_MASK                    (0xFF<<16)
#define CSL_CPMAC_TYPE_0_MACSRCADDR3_SHIFT                   16
#define CSL_CPMAC_TYPE_0_MACSRCADDR4_MASK                    (0xFF<<8)
#define CSL_CPMAC_TYPE_0_MACSRCADDR4_SHIFT                   8
#define CSL_CPMAC_TYPE_0_MACSRCADDR5_MASK                    (0xFF)
#define CSL_CPMAC_TYPE_0_MACSRCADDR5_SHIFT                   0

/* Type 1 Address filtering Macros */
#define CSL_CPMAC_TYPE_1_MACSRCADDR0_MASK                    (0xFF<<8)
#define CSL_CPMAC_TYPE_1_MACSRCADDR0_SHIFT                   8
#define CSL_CPMAC_TYPE_1_MACSRCADDR1_MASK                    (0xFF)
#define CSL_CPMAC_TYPE_1_MACSRCADDR1_SHIFT                   0

#define CSL_CPMAC_TYPE_1_MACSRCADDR2_MASK                    (0xFF<<24)
#define CSL_CPMAC_TYPE_1_MACSRCADDR2_SHIFT                   24
#define CSL_CPMAC_TYPE_1_MACSRCADDR3_MASK                    (0xFF<<16)
#define CSL_CPMAC_TYPE_1_MACSRCADDR3_SHIFT                   16
#define CSL_CPMAC_TYPE_1_MACSRCADDR4_MASK                    (0xFF<<8)
#define CSL_CPMAC_TYPE_1_MACSRCADDR4_SHIFT                   8
#define CSL_CPMAC_TYPE_1_MACSRCADDR5_MASK                    (0xFF)
#define CSL_CPMAC_TYPE_1_MACSRCADDR5_SHIFT                   0

/* CP(G)MAC address filtering bit macros */
#define CSL_CPGMAC_VALID_MASK                                   (0x1<<20)
#define CSL_CPGMAC_VALID_SHIFT                                  20
#define CSL_CPGMAC_MATCH_FILTER_MASK                            (0x1<<19)
#define CSL_CPGMAC_MATCH_FILTER_SHIFT                           19
#define CSL_CPGMAC_CHANNEL_MASK                                 (0x7<<16)
#define CSL_CPGMAC_CHANNEL_SHIFT                                16
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR0_MASK                    (0xFF<<8)
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR0_SHIFT                   8
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR1_MASK                    (0xFF)
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR1_SHIFT                   0

#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR2_MASK                    (0xFF<<24)
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR2_SHIFT                   24
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR3_MASK                    (0xFF<<16)
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR3_SHIFT                   16
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR4_MASK                    (0xFF<<8)
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR4_SHIFT                   8
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR5_MASK                    (0xFF)
#define CSL_CPGMAC_TYPE_2_3_MACSRCADDR5_SHIFT                   0


/* RX MBP register bit positions */
#define CSL_CPMAC_RXMBP_HIPRITHRESH_SHIFT           25
#define CSL_CPMAC_RXMBP_HIPRITHRESH_MASK            (0x7 << 25)
/*
#define CSL_CPMAC_RXMBP_PASSCRC_SHIFT               30
#define CSL_CPMAC_RXMBP_PASSCRC_MASK                (0x1 << 30)
#define CSL_CPMAC_RXMBP_QOSEN_SHIFT                 29
#define CSL_CPMAC_RXMBP_QOSEN_MASK                  (0x1 << 29)
#define CSL_CPMAC_RXMBP_NOCHAIN_SHIFT               28
#define CSL_CPMAC_RXMBP_NOCHAIN_MASK                (0x1 << 28)
*/
#define CSL_CPMAC_RXMBP_CMFEN_SHIFT                 24
#define CSL_CPMAC_RXMBP_CMFEN_MASK                  (0x1 << 24)
#define CSL_CPMAC_RXMBP_CSFEN_SHIFT                 23
#define CSL_CPMAC_RXMBP_CSFEN_MASK                  (0x1 << 23)
#define CSL_CPMAC_RXMBP_CEFEN_SHIFT                 22
#define CSL_CPMAC_RXMBP_CEFEN_MASK                  (0x1 << 22)
#define CSL_CPMAC_RXMBP_CAFEN_SHIFT                 21
#define CSL_CPMAC_RXMBP_CAFEN_MASK                  (0x1 << 21)
#define CSL_CPMAC_RXMBP_PROMCH_SHIFT                16
#define CSL_CPMAC_RXMBP_PROMCH_MASK                 (0x7 << 16)
#define CSL_CPMAC_RXMBP_BROADEN_SHIFT               13
#define CSL_CPMAC_RXMBP_BROADEN_MASK                (0x1 << 13)
#define CSL_CPMAC_RXMBP_BROADCH_SHIFT               8
#define CSL_CPMAC_RXMBP_BROADCH_MASK                (0x7 << 8)
#define CSL_CPMAC_RXMBP_MULTIEN_SHIFT               5
#define CSL_CPMAC_RXMBP_MULTIEN_MASK                (0x1 << 5)
#define CSL_CPMAC_RXMBP_MULTICH_SHIFT               0
#define CSL_CPMAC_RXMBP_MULTICH_MASK                0x7

/* Mac Control register bit fields */
#define CSL_CPMAC_MACCONTROL_RXVLANEN_SHIFT         19
#define CSL_CPMAC_MACCONTROL_RXVLANEN_MASK          (0x1 << 19)
#define CSL_CPMAC_MACCONTROL_EXTEN_SHIFT            18
#define CSL_CPMAC_MACCONTROL_EXTEN_MASK             (0x1 << 18)
#define CSL_CPMAC_MACCONTROL_GIGFORCE_SHIFT         17 
#define CSL_CPMAC_MACCONTROL_GIGFORCE_MASK          (0x1 << 17)
#define CSL_CPMAC_MACCONTROL_IFCTLB_SHIFT           16 
#define CSL_CPMAC_MACCONTROL_IFCTLB_MASK            (0x1 << 16)
#define CSL_CPMAC_MACCONTROL_IFCTLA_SHIFT            15 
#define CSL_CPMAC_MACCONTROL_IFCTLA_MASK             (0x1 << 15)

#define CSL_CPMAC_MACCONTROL_RXFIFOFLOWEN_SHIFT     12
#define CSL_CPMAC_MACCONTROL_RXFIFOFLOWEN_MASK      (0x1 << 12)
#define CSL_CPMAC_MACCONTROL_CMDIDLE_SHIFT          11 
#define CSL_CPMAC_MACCONTROL_CMDIDLE_MASK           (0x1 << 11)
#define CSL_CPMAC_MACCONTROL_TXSHORTGAPEN_SHIFT     10
#define CSL_CPMAC_MACCONTROL_TXSHORTGAPEN_MASK      (0x1 << 10)
#define CSL_CPMAC_MACCONTROL_TXPTYPE_SHIFT          9
#define CSL_CPMAC_MACCONTROL_TXPTYPE_MASK           (0x1 << 9)
#define CSL_CPMAC_MACCONTROL_GIGABITEN_SHIFT        7
#define CSL_CPMAC_MACCONTROL_GIGABITEN_MASK         (0x1 << 7)
#define CSL_CPMAC_MACCONTROL_TXPACEEN_SHIFT         6
#define CSL_CPMAC_MACCONTROL_TXPACEEN_MASK          (0x1 << 6)
#define CSL_CPMAC_MACCONTROL_MIIEN_SHIFT            5
#define CSL_CPMAC_MACCONTROL_MIIEN_MASK             (0x1 << 5)
#define CSL_CPMAC_MACCONTROL_TXFLOWEN_SHIFT         4
#define CSL_CPMAC_MACCONTROL_TXFLOWEN_MASK          (0x1 << 4)
#define CSL_CPMAC_MACCONTROL_RXFLOWEN_SHIFT         3
#define CSL_CPMAC_MACCONTROL_RXFLOWEN_MASK          (0x1 << 3)
#define CSL_CPMAC_MACCONTROL_LOOPBKEN_SHIFT         1
#define CSL_CPMAC_MACCONTROL_LOOPBKEN_MASK          (0x1 << 1)
#define CSL_CPMAC_MACCONTROL_FULLDUPLEXEN_SHIFT     0
#define CSL_CPMAC_MACCONTROL_FULLDUPLEXEN_MASK      (0x1)

/* MacStatus register */
/* GSG - errors no longer reported through Status reg
#define CSL_CPMAC_MACSTATUS_TXERRCODE_MASK          0xF00000
#define CSL_CPMAC_MACSTATUS_TXERRCODE_SHIFT         20
#define CSL_CPMAC_MACSTATUS_TXERRCH_MASK            0x7
#define CSL_CPMAC_MACSTATUS_TXERRCH_SHIFT           16
#define CSL_CPMAC_MACSTATUS_RXERRCODE_MASK          0xF000
#define CSL_CPMAC_MACSTATUS_RXERRCODE_SHIFT         12
#define CSL_CPMAC_MACSTATUS_RXERRCH_MASK            0x7
#define CSL_CPMAC_MACSTATUS_RXERRCH_SHIFT           8
*/

/* CPMAC RX Max packet length mask */
#define CSL_CPMAC_RX_MAX_LEN_SHIFT              0
#define CSL_CPMAC_RX_MAX_LEN_MASK               0xFFFF

/* CPMAC RX Max packet length mask */
#define CSL_CPMAC_RX_BUFFER_OFFSET_SHIFT        0
#define CSL_CPMAC_RX_BUFFER_OFFSET_MASK         0xFFFF

/* MAC_IN_VECTOR (0x180) register bit fields */
#define CPMAC_MAC_IN_VECTOR_STATUS_INT          (1 << 19)
#define CPMAC_MAC_IN_VECTOR_HOST_INT            (1 << 18)
#define CPMAC_MAC_IN_VECTOR_RX_INT_OR           (1 << 17)
#define CPMAC_MAC_IN_VECTOR_TX_INT_OR           (1 << 16)
#define CPMAC_MAC_IN_VECTOR_RX_INT_VEC          (7 << 8)
#define CPMAC_MAC_IN_VECTOR_TX_INT_VEC          (7)

/* CPPI bit positions */
#define CPMAC_CPPI_SOP_BIT                      0x80000000  /*(1 << 31)*/
#define CPMAC_CPPI_EOP_BIT                      0x40000000  /*(1 << 30*/
#define CPMAC_CPPI_OWNERSHIP_BIT                0x20000000  /*(1 << 29)*/
#define CPMAC_CPPI_EOQ_BIT                      0x10000000  /*(1 << 28)*/
#define CPMAC_CPPI_TEARDOWN_COMPLETE_BIT        0x8000000   /*(1 << 27)*/
#define CPMAC_CPPI_PASS_CRC_BIT                 0x4000000   /*(1 << 26)*/


#endif /* __CSLR_CPGMAC_F_H__ */


