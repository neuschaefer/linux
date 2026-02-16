/*
 *
 * ddc_cpgmac_f.c 
 * Description:
 * see below
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2008-2013 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution 
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */


/** \file   ddc_cpgmac_f.c
    \brief  DDC CPGMAC_F Source file

    This file contains the device driver core for CPGMAC_F device based upon
    PSP Framework architecture.

    @author     Greg Guyotte
*/

#include "ddc_cpgmac_f_Drv.h"
#ifdef CONFIG_ARM_AVALANCHE_PPD

#include <asm-arm/arch-avalanche/puma5/puma5_pp.h>
#endif
/* Version macro */
#define CPMAC_DDC_MAJOR_VERSION         0
#define CPMAC_DDC_MINOR_VERSION         2

/* To debug Teardown Process with SR */
//#define TD_DEBUG

#ifdef CONFIG_ARM_AVALANCHE_PPD
#define DDC_CPMAC_SR_DELAY1                100
#define DDC_CPMAC_SR_DELAY2                200
#endif

const PRIVATE Char CpmacDDCVersionString[] = "CPGMAC_F DDC version 0.2";

/* Static Global Instance Variable */
PRIVATE Bool CpmacDDCInstCreated[CPMAC_MAX_INSTANCES] = { False, False };
PRIVATE CpmacDDCObj *CpmacDDCObject[CPMAC_MAX_INSTANCES] = { NULL, NULL };
PRIVATE Uint32 CpmacDDCNumInst = 0;
Uint32 CpmacDDCDebug = 1;

/* Internal functions */
PRIVATE PAL_Result DDC_cpmacInit(CpmacDDCObj * hDDC, CpmacInitConfig * initCfg);
PRIVATE PAL_Result DDC_cpmacDeInit(CpmacDDCObj * hDDC, Ptr param);
PRIVATE PAL_Result DDC_cpmacOpen(CpmacDDCObj * hDDC, Ptr param);
PRIVATE PAL_Result DDC_cpmacClose(CpmacDDCObj * hDDC, Ptr param);
PRIVATE PAL_Result DDC_cpmacControl(CpmacDDCObj * hDDC, Int cmd, Ptr cmdArg, Ptr param);
PRIVATE PAL_Result DDC_cpmacDeleteInstance(CpmacDDCObj * hDDC, Ptr param);
PRIVATE PAL_Result DDC_cpmacChOpen(CpmacDDCObj * hDDC, CpmacChInfo * chInfo, Ptr chOpenArgs);
PRIVATE PAL_Result DDC_cpmacChClose(CpmacDDCObj * hDDC, Int channel, Int direction, Ptr chCloseArgs);
PRIVATE PAL_Result cpmacEnableChannel(CpmacDDCObj * hDDC, Uint32 channel, Uint32 direction);
PRIVATE PAL_Result cpmacDisableChannel(CpmacDDCObj * hDDC, Uint32 channel, DDC_NetChDir direction);
PRIVATE PAL_Result cpmacInitTxChannel(CpmacDDCObj * hDDC, CpmacChInfo * chInfo, Ptr chOpenArgs);
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacInitEmbRxChannel(CpmacDDCObj * hDDC, CpmacChInfo * chInfo, Ptr chOpenArgs);
PRIVATE PAL_Result cpmacEnableEmbChannel(CpmacDDCObj * hDDC, Uint32 channel,Uint32 direction);
PRIVATE PAL_Result cpmacInitEth2HostProxyTxChannel(CpmacDDCObj * hDDC,CpmacChInfo * chInfo, Ptr chOpenArgs);
PRIVATE PAL_Result cpmacEnableEth2HostProxyChannel(CpmacDDCObj * hDDC, Uint32 channel,Uint32 direction);
PRIVATE PAL_Result cpmacDeInitEmbRxChannel(CpmacDDCObj * hDDC, Uint32 channel,Ptr chCloseArgs);
PRIVATE PAL_Result cpmacDisableEmbChannel(CpmacDDCObj * hDDC, Uint32 channel,DDC_NetChDir direction);
PRIVATE PAL_Result cpmacDisableEth2HostProxyChannel(CpmacDDCObj * hDDC, Uint32 channel,DDC_NetChDir direction);
PRIVATE PAL_Result cpmacDeInitEth2HostProxyTxChannel(CpmacDDCObj * hDDC, Uint32 channel,Ptr chCloseArgs);
#endif
PRIVATE PAL_Result cpmacSetupTxChannel(CpmacDDCObj * hDDC,CpmacChInfo * chInfo, Ptr chOpenArgs);
PRIVATE PAL_Result cpmacSetupRxChannel(CpmacDDCObj * hDDC,CpmacChInfo * chInfo, Ptr chOpenArgs);
PRIVATE PAL_Result cpmacTearDownRxChannel(CpmacDDCObj * hDDC, Int channel, Ptr chCloseArgs);
PRIVATE PAL_Result cpmacTearDownTxChannel(CpmacDDCObj * hDDC, Int channel, Ptr chCloseArgs);
PRIVATE PAL_Result cpmacInitRxChannel(CpmacDDCObj * hDDC, CpmacChInfo * chInfo, Ptr chOpenArgs);
PRIVATE PAL_Result cpmacDeInitTxChannel(CpmacDDCObj * hDDC, Uint32 channel, Ptr chCloseArgs);
PRIVATE PAL_Result cpmacDeInitRxChannel(CpmacDDCObj * hDDC, Uint32 channel, Ptr chCloseArgs);
PRIVATE void cpmacSetMacAddress(CpmacDDCObj * hDDC, Uint32 channel, String macAddr);
PRIVATE void cpmacSetSrcMacAddress(CpmacDDCObj * hDDC, String macAddr);
PRIVATE void cpmacDDCIfcntClear(CpmacDDCObj * hDDC);
PRIVATE void cpmacDDCIfcntUpdt(CpmacDDCObj * hDDC);
PRIVATE void cpmacDDCPhycnt(CpmacDDCObj * hDDC, Uint32 * cmdArg);
PRIVATE PAL_Result cpmacAddRxBd(CpmacDDCObj * hDDC, CpmacChInfo * chInfo, Uint32 numOfBd2Add);



/***************************** HASH SUPPORT FUNCTIONS *************************/

/**
 *  hashGet
 *    - Get hash value using mechainsm in specs
 */
PRIVATE Uint32 hashGet(Uint8 * addr)
{
    Uint32 hash;
    Uint8 tmpval;
    Int cnt;

    hash = 0;
    for (cnt = 0; cnt < 2; cnt++) {
        tmpval = *addr++;
        hash ^= (tmpval >> 2) ^ (tmpval << 4);
        tmpval = *addr++;
        hash ^= (tmpval >> 4) ^ (tmpval << 2);
        tmpval = *addr++;
        hash ^= (tmpval >> 6) ^ (tmpval);
    }

    return (hash & 0x3F);
}

/**
 *  hashAdd
 *    - Adds mac address to hash table and upates hash bits in hardware
 *    - Returns negative if error, 0 if no change to registers,  >0 if hash
 *      registers need to change
 */
PRIVATE Int hashAdd(CpmacDDCObj * hDDC, Uint8 * macAddress)
{
    Uint32 hashValue;
    Uint32 hashBit;
    Uint32 status = 0;

    hashValue = hashGet(macAddress);

    if (hashValue >= CPMAC_NUM_MULTICAST_BITS) {
        CPMAC_DDC_LOGERR("\nERROR:DDC: hashAdd:%d: Invalid Hash Value=%d. Should not be greater than %d",
             hDDC->ddcObj.instId, hashValue,
             (CPMAC_NUM_MULTICAST_BITS - 1));
        return (CPMAC_INVALID_PARAM);
    }

    /* Set the hash bit only if not previously set */
    if (hDDC->multicastHashCnt[hashValue] == 0) {
        status = 1;
        if (hashValue < 32) {
            hashBit = (1 << hashValue);
            hDDC->MacHash1 |= hashBit;
        } else {
            hashBit = (1 << (hashValue - 32));
            hDDC->MacHash2 |= hashBit;
        }
    }

    /* Inc counter of mcast addresses that map to this hash bit */
    ++hDDC->multicastHashCnt[hashValue];

    return (status);
}

/**
 *  hashDel
 *    - Deletes a mac address from hash table and updates hash register bits
 *    - Returns negative if error, 0 if no change to registers, >0 if hash
 *      registers need to change
 */
PRIVATE Int hashDel(CpmacDDCObj * hDDC, Uint8 * macAddress)
{
    Uint32 hashValue;
    Uint32 hashBit;

    hashValue = hashGet(macAddress);

    if (hDDC->multicastHashCnt[hashValue] > 0) {
        /* Decrement counter of multicast address that map to this hash bit  */
        --hDDC->multicastHashCnt[hashValue];
    }

    /* If counter > 0, at least 1 mcast addr refers to this hash bit, return 0 */
    if (hDDC->multicastHashCnt[hashValue] > 0)
        return (0);

    if (hashValue < 32) {
        hashBit = (1 << hashValue);
        hDDC->MacHash1 &= ~hashBit;
    } else {
        hashBit = (1 << (hashValue - 32));
        hDDC->MacHash2 &= ~hashBit;
    }

    /* Return 1 to indicate change in MacHash registers required */
    return (1);
}

/**
 *  cpmacSingleMulti
 *    - Updates hash register bits with single multicast address add/delete
 *      operation
 */
void cpmacSingleMulti(CpmacDDCObj * hDDC, CpmacSingleMultiOper oper,
                      Uint8 * addr)
{
    Int32 status = -1;
    switch (oper) {
    case CPMAC_MULTICAST_ADD:
        status = hashAdd(hDDC, addr);
        break;
    case CPMAC_MULTICAST_DEL:
        status = hashDel(hDDC, addr);
        break;
    default:
        CPMAC_DDC_LOGERR("\nWARN: cpmacSingleMulti:%d: Unhandled Single Multicast operation %d",
             hDDC->ddcObj.instId, oper);
        break;
    }
    /* Write to the hardware only if the register status chances */
    if (status > 0) {
        hDDC->regs->MacHash1 = hDDC->MacHash1;
        hDDC->regs->MacHash2 = hDDC->MacHash2;
    }
}

/**
 *  cpmacAllMulti
 *    - Updates hash register bits for all multi operation (set/clear)
 */
void cpmacAllMulti(CpmacDDCObj * hDDC, CpmacAllMultiOper oper)
{
    switch (oper) {
    case CPMAC_ALL_MULTI_SET:
        hDDC->MacHash1 = CPMAC_ALL_MULTI_REG_VALUE;
        hDDC->MacHash2 = CPMAC_ALL_MULTI_REG_VALUE;
        break;

    case CPMAC_ALL_MULTI_CLR:
        hDDC->MacHash1 = 0;
        hDDC->MacHash2 = 0;
        break;
    default:
        CPMAC_DDC_LOGERR("\nWARN: cpmacAllMulti:%d: Unhandled All multi operation %d",
             hDDC->ddcObj.instId, oper);
        break;
    }

    hDDC->regs->MacHash1 = hDDC->MacHash1;
    hDDC->regs->MacHash2 = hDDC->MacHash2;
}


/*************************** PHY related functions ****************************/

/**
 *  cpmacUpdatePhyStatus
 *    - updates phy status variables in hDDC->status "CpmacDDCStatus" structure
 */
Int cpmacUpdatePhyStatus(CpmacDDCObj * hDDC)
{
    PHY_DEVICE *PhyDev = hDDC->PhyDev;
    Uint32 setPhyMode;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_BUSY_FUNCTION_ENTRY,"\n+ cpmacUpdatePhyStatus %d", hDDC->ddcObj.instId);

    /* Verify proper device state */
    if (hDDC->ddcObj.state != DDC_OPENED) {
        CPMAC_DDC_LOGERR("\nERROR:DDC: cpmacUpdatePhyStatus:%d: Device NOT Open",
             hDDC->ddcObj.instId);
        return (CPMAC_ERR_DEV_NOT_OPEN);
    }

    setPhyMode = hDDC->initCfg.phyMode;

    /* No Phy Condition */
    if (setPhyMode & SNWAY_NOPHY) {
        /* No Phy condition, always linked */
        hDDC->status.PhyLinked = 1;
        hDDC->status.PhySpeed = 1;
        hDDC->status.PhyDuplex = 1;
        hDDC->status.PhyNum = 0xFFFFFFFF;       /* No Phy */
        hDDC->MacControl |= (1 << CSL_CPMAC_MACCONTROL_FULLDUPLEXEN_SHIFT);
        /* Write mac control register from stored value */
        hDDC->regs->MacControl = hDDC->MacControl;
        goto cpmacUpdatePhyStatus_Exit;
    }

    /* If loopback set in hardware, set link to ON */
    if (hDDC->MacControl & CSL_CPMAC_MACCONTROL_LOOPBKEN_MASK) {
        hDDC->status.PhyLinked = 1;
        goto cpmacUpdatePhyStatus_Exit;
    }

    if (setPhyMode & SNWAY_LPBK) {
        hDDC->status.PhyLinked = cpswHalCommonMiiMdioGetLoopback(PhyDev);
    } else {
        hDDC->status.PhyLinked = cpswHalCommonMiiMdioGetLinked(PhyDev);
    }

    if (hDDC->status.PhyLinked) {
        /*  Retreive Duplex and Speed and the Phy Number  */
        if (setPhyMode & SNWAY_LPBK) {
            hDDC->status.PhyDuplex = 1;
        } else {
            hDDC->status.PhyDuplex = cpswHalCommonMiiMdioGetDuplex(PhyDev);
        }

        hDDC->status.PhySpeed = cpswHalCommonMiiMdioGetSpeed(PhyDev);
#if defined (CONFIG_MACH_PUMA5EVM)		
        /* 10th bit represent the speed for 10/100 Mbps thats why  below
         *  code is required for 10/100 Mbps while function returns
         * 2 for 100 Mbps hence for that below code is not rquired
         */
        if (hDDC->status.PhySpeed != 2)
#endif		
        hDDC->status.PhySpeed = hDDC->status.PhySpeed >> 10;
        
        hDDC->status.PhyNum = cpswHalCommonMiiMdioGetPhyNum(PhyDev);

        /* Set the duplex bit in maccontrol */
        if (hDDC->status.PhyDuplex) {
            hDDC->MacControl |=
                (1 << CSL_CPMAC_MACCONTROL_FULLDUPLEXEN_SHIFT);
        } else {
            hDDC->MacControl &=
                ~(1 << CSL_CPMAC_MACCONTROL_FULLDUPLEXEN_SHIFT);
        }
    }

    /* Write mac control register from stored value */
    hDDC->regs->MacControl = hDDC->MacControl;

  cpmacUpdatePhyStatus_Exit:
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_PORT_UPDATE,
                     "\ncpmacUpdatePhyStatus:%d: MacControl=%08X, Status: Phy=%d, Speed=%s, Duplex=%s",
                     hDDC->ddcObj.instId, hDDC->MacControl,
                     hDDC->status.PhyNum,
                     (hDDC->status.PhySpeed) ? "100" : "10",
                     (hDDC->status.PhyDuplex) ? "Full" : "Half");

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_BUSY_FUNCTION_EXIT, "\n- cpmacUpdatePhyStatus:%d:", hDDC->ddcObj.instId);

    return (CPMAC_SUCCESS);
}

/**
 *  cpmacSetPhyMode
 **/
PRIVATE Int cpmacSetPhyMode(CpmacDDCObj * hDDC)
{
    Uint32 setPhyMode;
    Uint32 PhyMode;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_BUSY_FUNCTION_ENTRY,
                     "\n+ cpmacSetPhyMode:%d:", hDDC->ddcObj.instId);

    /* Verify proper device state */
    if (hDDC->ddcObj.state != DDC_OPENED) {
        CPMAC_DDC_LOGERR("\nERROR:DDC: cpmacSetPhyMode:%d: Device NOT Open",
             hDDC->ddcObj.instId);
        return (CPMAC_ERR_DEV_NOT_OPEN);
    }

    /* Detect if MDI/MDIX selection is available in this hardware via PAL_Sys */
    if (1 == avalanche_is_mdix_on_chip()) {
        hDDC->initCfg.phyMode |= SNWAY_AUTOMDIX;
    }

    setPhyMode = hDDC->initCfg.phyMode;
    PhyMode = 0;
    if (setPhyMode & SNWAY_AUTO)
        PhyMode |= NWAY_AUTO;
    if (setPhyMode & SNWAY_FD10)
        PhyMode |= NWAY_FD10;
    if (setPhyMode & SNWAY_FD100)
        PhyMode |= NWAY_FD100;
    if (setPhyMode & SNWAY_FD1000)
        PhyMode |= NWAY_FD1000;
    if (setPhyMode & SNWAY_HD10)
        PhyMode |= NWAY_HD10;
    if (setPhyMode & SNWAY_HD100)
        PhyMode |= NWAY_HD100;
    if (setPhyMode & SNWAY_HD1000)
        PhyMode |= NWAY_HD1000;
    if (setPhyMode & SNWAY_LPBK)
        PhyMode |= NWAY_LPBK;
    if (setPhyMode & SNWAY_AUTOMDIX)
        PhyMode |= NWAY_AUTOMDIX;

    /* Check for CPMAC Bus frequency for correct speed operation */
    if ((setPhyMode & SNWAY_FD10) || (setPhyMode & SNWAY_HD10)) {
        if (hDDC->initCfg.cpmacBusFrequency <=
            CPMAC_MIN_FREQUENCY_FOR_10MBPS)
            CPMAC_DDC_LOGERR("\nERROR: Bus speedcpmacSetPhyMode:%d: CpmacFreq(%d) is less than required %d freq for 10Mbps support. CANNOT SUPPORT 10Mbps",
                 hDDC->ddcObj.instId, hDDC->initCfg.cpmacBusFrequency,
                 CPMAC_MIN_FREQUENCY_FOR_10MBPS);
    } else if ((setPhyMode & SNWAY_FD100) || (setPhyMode & SNWAY_HD100)) {
        if (hDDC->initCfg.cpmacBusFrequency <=
            CPMAC_MIN_FREQUENCY_FOR_100MBPS)
            CPMAC_DDC_LOGERR("\nERROR: Bus speedcpmacSetPhyMode:%d: CpmacFreq(%d) is less than required %d freq for 100Mbps support. CANNOT SUPPORT 100Mbps",
                 hDDC->ddcObj.instId, hDDC->initCfg.cpmacBusFrequency,
                 CPMAC_MIN_FREQUENCY_FOR_100MBPS);
    } else if ((setPhyMode & SNWAY_FD1000) || (setPhyMode & SNWAY_HD1000)) {
        if (hDDC->initCfg.cpmacBusFrequency <=
            CPMAC_MIN_FREQUENCY_FOR_1000MBPS)
            CPMAC_DDC_LOGERR("\nERROR: Bus speedcpmacSetPhyMode:%d: CpmacFreq(%d) is less than required %d freq for 1000Mbps support. CANNOT SUPPORT 1000Mbps",
                 hDDC->ddcObj.instId, hDDC->initCfg.cpmacBusFrequency,
                 CPMAC_MIN_FREQUENCY_FOR_1000MBPS);
    }

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_PORT_UPDATE,
                     "\ncpmacSetPhyMode:%d: MdioPhyMode=%08X, PhyMode=%08d, Auto:%d, FD10:%d, HD10:%d, FD100:%d, HD100:%d",
                     hDDC->ddcObj.instId, setPhyMode, PhyMode,
                     (PhyMode & NWAY_AUTO), (PhyMode & NWAY_FD10),
                     (PhyMode & NWAY_HD10), (PhyMode & NWAY_FD100),
                     (PhyMode & NWAY_HD100));

    cpswHalCommonMiiMdioSetPhyMode(hDDC->PhyDev, PhyMode);
    cpmacUpdatePhyStatus(hDDC);

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_BUSY_FUNCTION_EXIT, "\n- cpmacSetPhyMode:%d:", hDDC->ddcObj.instId);

    return (CPMAC_SUCCESS);
}

/********************** MAC ADDRESSING MODE SUPPORT FUNCTIONS *****************/

/**
 *  cpmacRxUniCast
 *   - This function sets / clears the unicast flag in hardware
 */
PRIVATE void cpmacRxUniCast(CpmacDDCObj * hDDC, Uint32 channel,
                            Bool enable)
{
    /* Update local copy of register to save cycles in reading the register */
    if (enable == True) {

        hDDC->Rx_Unicast_Set |= (1 << channel);
        hDDC->Rx_Unicast_Clear &= ~(1 << channel);
    } else {
        /* Disable Unicast Channel Setting */

        hDDC->Rx_Unicast_Clear |= (1 << channel);
        hDDC->Rx_Unicast_Set &= ~(1 << channel);
    }

    /* Write to hardware if device is open */
    if (hDDC->ddcObj.state == DDC_OPENED) {
        hDDC->regs->Rx_Unicast_Set = hDDC->Rx_Unicast_Set;
        hDDC->regs->Rx_Unicast_Clear = hDDC->Rx_Unicast_Clear;
    }
}

/**
 *  cpmacAddType0Addr
 *    - set mac address for type 0 addressing
 *
 *  \note This is an internal function of the DDC called from channel enable
 *  API which does channel number range checking and hence its not required.
 *  It is assumed that this function will get the correct channel number.
 */
PRIVATE void cpmacAddType0Addr(CpmacDDCObj * hDDC, Uint32 channel,
                               String macAddress)
{
    hDDC->regs->MacAddr_Lo = (macAddress[5] << 8) | macAddress[4];
    hDDC->regs->MacAddr_Hi = (macAddress[3] << 24) | (macAddress[2] << 16)
        | (macAddress[1] << 8) | (macAddress[0]);

    /* Enable Unicast */
    cpmacRxUniCast(hDDC, channel, True);
}

/**
 *  cpmacSetSrcMacAddress
 *    - store the source MAC address of the CPGMAC_F module. To be used in
 *      conjuction with the Type 1 and Type 2 addresses, only. Not suitable for
 *      Type 0 addresses.
 */
PRIVATE void cpmacSetSrcMacAddress(CpmacDDCObj * hDDC, String macAddress)
{

    if ((hDDC->RxAddrType == RX_ADDR_TYPE1)
        || (hDDC->RxAddrType == RX_ADDR_TYPE2)) {
        hDDC->regs->MacSrcAddr_Lo = (macAddress[5] << 8) | macAddress[4];
        hDDC->regs->MacSrcAddr_Hi =
            (macAddress[3] << 24) | (macAddress[2] << 16)
            | (macAddress[1] << 8) | (macAddress[0]);
    }
}

/**
 *  cpmacAddType1Addr
 *    - set mac address for type 1 addressing
 *
 *  \note This is an internal function of the DDC called from channel enable
 *  API which does channel number range checking and hence its not required.
 *  It is assumed that this function will get the correct channel number.
 */
PRIVATE void cpmacAddType1Addr(CpmacDDCObj * hDDC, Uint32 channel,
                               String macAddress)
{
    /* Set MacIndex register with channel number */
    hDDC->regs->MacIndex = channel;

    /* Set MacAddr_Hi register */
    hDDC->regs->MacAddr_Hi = CSL_FMK(CPMAC_TYPE_1_MACSRCADDR2,(macAddress[3])) | 
	                     CSL_FMK(CPMAC_TYPE_1_MACSRCADDR3,(macAddress[2])) |
                             CSL_FMK(CPMAC_TYPE_1_MACSRCADDR4,(macAddress[1])) | 
			     CSL_FMK(CPMAC_TYPE_1_MACSRCADDR5,(macAddress[0]));

    /* Set MacAddr_Lo register */
    hDDC->regs->MacAddr_Lo = CSL_FMK(CPMAC_TYPE_1_MACSRCADDR0,(macAddress[5])) | 
	                     CSL_FMK(CPMAC_TYPE_1_MACSRCADDR1,(macAddress[4]));

    /* Enable Unicast */
    cpmacRxUniCast(hDDC, channel, True);
}

/**
 *  cpmacAddType2Addr
 *    - CPGMAC CFIG 2/3 Type addressing - Filtering
 */
PRIVATE void cpmacAddType2Addr(CpmacDDCObj * hDDC, Uint32 channel,
                               String macAddress, Int index, Bool valid,
                               Int match)
{
    Uint32 macAddrLo;

    hDDC->regs->MacIndex = index;
    hDDC->regs->MacAddr_Hi = CSL_FMK(CPGMAC_TYPE_2_3_MACSRCADDR2,(macAddress[3])) | 
	                     CSL_FMK(CPGMAC_TYPE_2_3_MACSRCADDR3,(macAddress[2])) |
                             CSL_FMK(CPGMAC_TYPE_2_3_MACSRCADDR4,(macAddress[1])) |
			     CSL_FMK(CPGMAC_TYPE_2_3_MACSRCADDR5,(macAddress[0]));
    macAddrLo = CSL_FMK(CPGMAC_VALID, ((valid == True) ? 0x1 : 0));
    macAddrLo |= CSL_FMK(CPGMAC_MATCH_FILTER, ((valid == True) ? 0x1 : 0));
    macAddrLo |= CSL_FMK(CPGMAC_CHANNEL, channel);
    macAddrLo |= CSL_FMK(CPGMAC_TYPE_2_3_MACSRCADDR0, (macAddress[5]));
    macAddrLo |= CSL_FMK(CPGMAC_TYPE_2_3_MACSRCADDR1, (macAddress[4]));

    /* Set MacAddr_Lo register */
    hDDC->regs->MacAddr_Lo = macAddrLo;

    cpmacRxUniCast(hDDC, channel, True);
}

/**
 *  cpmacType2AddrInit
 */
PRIVATE void cpmacType2AddrInit(CpmacDDCObj * hDDC)
{
    Uint32 i = 0;

    /* initializing the address filtering RAM for type2 addresses
     * All entries in the RAM are set to invalid
     */

    for (i = 0; i < 32; i++) {
        hDDC->regs->MacIndex = i;
        hDDC->regs->MacAddr_Lo = CSL_FMK(CPGMAC_VALID, False);
    }
}

/**
 *  cpmacTypeXAddrInit
 */
PRIVATE void cpmacTypeXAddrInit(CpmacDDCObj * hDDC)
{
    /* We handle only the Type 2 addresses, as of now. Moreover, we cannot
     * do anything to initialize the Type 1 and Type 0 addresses in the CPMAC.
     */
    if (hDDC->RxAddrType == RX_ADDR_TYPE2)
        cpmacType2AddrInit(hDDC);
}

/**
 *  cpmacSetMacAddress
 *    - CPMAC address is set in the hardware based on the address type
 *
 *   \note 1. It is assumed that the channel is already "initialized"
 */
PRIVATE void cpmacSetMacAddress(CpmacDDCObj * hDDC, Uint32 channel,
                                String macAddr)
{
    /* Enable Unicast on this channel */
    hDDC->regs->Rx_Unicast_Set = (1 << channel);

    /* Program MAC address for the channel depending upon cpmac/cpgmac */
    if (hDDC->RxAddrType == RX_ADDR_TYPE0)
        cpmacAddType0Addr(hDDC, channel, macAddr);
    else if (hDDC->RxAddrType == RX_ADDR_TYPE1)
        cpmacAddType1Addr(hDDC, channel, macAddr);
    else if (hDDC->RxAddrType == RX_ADDR_TYPE2) {
        cpmacAddType2Addr(hDDC, channel, macAddr, 0, 1, 1);
    } else
        CPMAC_DDC_LOGERR("\nWARN: cpmacSetMacAddress:%d: Wrong Rx Addressing Type - (Type2) detected in hardware",
             hDDC->ddcObj.instId);
}

/****************** HARDWARE CONFIGURATION SUPPORT FUNCTIONS ******************/

/**
 *  cpmacSetRxHwCfg
 *    - set RX Hardware configuration
 */
void cpmacSetRxHwCfg(CpmacDDCObj * hDDC)
{
    CpmacRxConfig *rxCfg;
    Uint32 rxMbpEnable;

    if (hDDC->ddcObj.state != DDC_OPENED) {
        CPMAC_DDC_LOGERR("\nWARN: cpmacSetRxHwCfg:%d: Function called when device is NOT in open state",
             hDDC->ddcObj.instId);
        return;
    }

    rxCfg = &hDDC->initCfg.rxCfg;

    /* Set RX MBP Enable register */
    rxMbpEnable =
        CSL_FMK(CPMAC_RXMBP_HIPRITHRESH, rxCfg->hiPriThresh) |
        CSL_FMK(CPMAC_RXMBP_CMFEN, rxCfg->copyMACControlFramesEnable) |
        CSL_FMK(CPMAC_RXMBP_CSFEN, rxCfg->copyShortFramesEnable) |
        CSL_FMK(CPMAC_RXMBP_CEFEN, rxCfg->copyErrorFramesEnable) |
        CSL_FMK(CPMAC_RXMBP_CAFEN, rxCfg->promiscousEnable) |
        CSL_FMK(CPMAC_RXMBP_PROMCH, rxCfg->promiscousChannel) |
        CSL_FMK(CPMAC_RXMBP_BROADEN, rxCfg->broadcastEnable) |
        CSL_FMK(CPMAC_RXMBP_BROADCH, rxCfg->broadcastChannel) |
        CSL_FMK(CPMAC_RXMBP_MULTIEN, rxCfg->multicastEnable) |
        CSL_FMK(CPMAC_RXMBP_MULTICH, rxCfg->multicastChannel);

    hDDC->Rx_MBP_Enable = rxMbpEnable;
    hDDC->regs->Rx_MBP_Enable = rxMbpEnable;

    /* Set max Rx packet length */
    hDDC->regs->Rx_Maxlen =
        CSL_FMK(CPMAC_RX_MAX_LEN, rxCfg->maxRxPktLength);

    /* Added support for Rx_flowthresh register */
    hDDC->regs->Rx_Flowthresh = rxCfg->rxFlowThreshold;
}

/**
 *  cpmacSetMacHwCfg
 *    - set MAC Configuration - MACControl register
 */
void cpmacSetMacHwCfg(CpmacDDCObj * hDDC)
{
    CpmacMacConfig *macCfg;
    Uint32 macControl;

    if (hDDC->ddcObj.state != DDC_OPENED) {
        CPMAC_DDC_LOGERR("\nWARN: cpmacSetMacHwCfg:%d: Function called when device is NOT in open state",
             hDDC->ddcObj.instId);
        return;
    }

    macCfg = &hDDC->initCfg.macCfg;

    macControl =
        CSL_FMK(CPMAC_MACCONTROL_RXVLANEN, macCfg->rxVlanEn) |
        CSL_FMK(CPMAC_MACCONTROL_EXTEN, macCfg->extEn) |
        CSL_FMK(CPMAC_MACCONTROL_GIGFORCE, ((hDDC->status.PhySpeed == 2)?1:0)) |
        CSL_FMK(CPMAC_MACCONTROL_IFCTLA, macCfg->ifCtlA) |
        CSL_FMK(CPMAC_MACCONTROL_IFCTLB, macCfg->ifCtlB) |
        CSL_FMK(CPMAC_MACCONTROL_CMDIDLE, macCfg->cmdIdle) |
        CSL_FMK(CPMAC_MACCONTROL_TXSHORTGAPEN, macCfg->txShortGapEnable) |
        CSL_FMK(CPMAC_MACCONTROL_GIGABITEN, ((hDDC->status.PhySpeed == 2)?1:0)) |
        CSL_FMK(CPMAC_MACCONTROL_TXPACEEN, macCfg->txPacingEnable) |
        (hDDC->MacControl & CSL_CPMAC_MACCONTROL_MIIEN_MASK) |
        CSL_FMK(CPMAC_MACCONTROL_TXFLOWEN, macCfg->txFlowEnable) |
        CSL_FMK(CPMAC_MACCONTROL_RXFLOWEN, macCfg->rxFlowEnable) |
        CSL_FMK(CPMAC_MACCONTROL_LOOPBKEN, macCfg->loopbackEnable) |
        (hDDC->MacControl & CSL_CPMAC_MACCONTROL_FULLDUPLEXEN_MASK);

    /* Add in features specific to CFIG3 */
    if (hDDC->RxAddrType == RX_ADDR_TYPE2) {
        /* This is applicable only for CFIG3. */
        macControl |=
            CSL_FMK(CPMAC_MACCONTROL_RXFIFOFLOWEN,
                    macCfg->rxFifoFlowEnable);
    }

    /* added to configure port VLAN register */
    hDDC->regs->Port_VLAN = macCfg->portVlan;

    if (hDDC->MacControl != macControl) {
        hDDC->MacControl = macControl;
        hDDC->regs->MacControl = macControl;
    }
}

/**************************** CPMAC DDC FUNCTIONS *****************************/

/* Static CPMAC DDC function table */
PRIVATE CpmacDDCIf CpmacDDCInterface = {
    {                           /* DDC Net Class functions */
     {                          /*   DDC Class functions */
      (DDC_Init) DDC_cpmacInit, /*      CPMAC Init function */
      (DDC_DeInit) DDC_cpmacDeInit,     /*      CPMAC DeInit function */
      (DDC_Open) DDC_cpmacOpen, /*      CPMAC Open function */
      (DDC_Close) DDC_cpmacClose,       /*      CPMAC Close function */
      (DDC_Control) DDC_cpmacControl,   /*      CPMAC Control function */
      (DDC_DeleteInstance) DDC_cpmacDeleteInstance      /* CPMAC delete "instance" function */
      }
     ,
     (DDC_NetSend) DDC_cpmacSend,       /*   CPMAC Send function */
     NULL,                      /*   Multiple packet send not supported */
     NULL,                      /*   Poll RX not supported */
     NULL,                      /*   Rx Return not supported */
     NULL,                      /*   DDC_cpmacIsr - NOT REQUIRED - taken care by PktProcess */
     (DDC_NetChOpen) DDC_cpmacChOpen,   /*   CPMAC channel open function */
     (DDC_NetChClose) DDC_cpmacChClose, /*   CPMAC channel close function */
     }
    ,
    (DDC_CpmacTick) cpmacTick,  /* CPMAC Tick function */
    (DDC_CpmacPktProcessEnd) cpmacPktProcessEnd,        /* CPMAC Packet processing End function */
    (DDC_CpmacTxPktProcessEnd) cpmacTxPktProcessEnd,    /* CPMAC Packet processing End function */
    (DDC_CpmacTxPktProcess) cpmacTxPktProcess,  /* CPMAC Packet processing function */
    (DDC_CpmacRxPktProcess) cpmacRxPktProcess,
    (DDC_CpmacAddRxBd) cpmacAddRxBd
};

/**
 *  DDC_cpmacGetPhyDev
 *    - Returns pointer to an internal PhyDev structure.  This should be moved
 *      to Control().
 */
Ptr DDC_cpmacGetPhyDev(CpmacDDCObj * hDDC)
{
    return (Ptr) hDDC->PhyDev;
}

/**
 *  DDC_cpmacGetVersionInfo
 */
String DDC_cpmacGetVersionInfo(Uint32 * swVer)
{
    if (swVer != NULL)
        *swVer = ((CPMAC_DDC_MAJOR_VERSION << 16) | CPMAC_DDC_MINOR_VERSION);
    return ((String) & CpmacDDCVersionString[0]);
}

/**
 *  DDC_cpmacCreateInstance
 *    - exchange handles and function pointers between DDA/DDC
 *
 *  \note 1. Unless function pointers are exchanged, error log and debug printf
 *           statements cannot be used in this function. Hence we dont log
 *           anything in this function.
 *        2. "param" is not used in this implementation
 */
PAL_Result DDC_cpmacCreateInstance(Uint32 instId, DDA_Handle hDDA,
                                   CpmacDDACbIf * hDDACbIf,
                                   DDC_Handle ** hDDC,
                                   CpmacDDCIf ** hDDCIf, Ptr param)
{
    CpmacDDCObj *cpmacDDCHandle;
    PAL_Result retCode = CPMAC_SUCCESS;

    /* Check CPMAC instance */
    if (CpmacDDCInstCreated[instId] == True) {
        return (CPMAC_ERR_DEV_ALREADY_INSTANTIATED(instId));
    }

    /* Allocate memory for CPMAC DDC Instance Object and set to 0 */
    retCode = PAL_osMemAlloc(0, sizeof(CpmacDDCObj), 0, (Ptr *) & cpmacDDCHandle);
    if (retCode != PAL_SOK)
        return (retCode);

    PAL_osMemSet(cpmacDDCHandle, 0, sizeof(CpmacDDCObj));

    /* Set CPMAC object variables */
    cpmacDDCHandle->ddcObj.versionId = (CPMAC_DDC_MAJOR_VERSION << 16) | CPMAC_DDC_MINOR_VERSION;
    cpmacDDCHandle->ddcObj.instId = instId;
    cpmacDDCHandle->ddcObj.state = DDC_CREATED;

    /* Populate CPMAC DDC Interface Table */
    cpmacDDCHandle->ddcIf = &CpmacDDCInterface;

    /* Save DDA handle and interface table */
    cpmacDDCHandle->ddcObj.hDDA = hDDA;
    cpmacDDCHandle->ddaIf = hDDACbIf;

    /* Pass back DDC Object handle and Interface Table handle */
    *hDDC = (DDC_Handle) cpmacDDCHandle;
    *hDDCIf = cpmacDDCHandle->ddcIf;

    /* Update "instance created" variable */
    CpmacDDCInstCreated[instId] = True;
    CpmacDDCObject[instId] = cpmacDDCHandle;
    ++CpmacDDCNumInst;

    return (retCode);
}

/**
 *  DDC_cpmacDeleteInstance
 *    - delete the "instance" created via CreateInstance
 *
 *  \note "param" is not used in this implementation
 */
PRIVATE PAL_Result DDC_cpmacDeleteInstance(CpmacDDCObj * hDDC, Ptr param)
{
    PAL_Result retCode = CPMAC_SUCCESS;
    Uint32 instId = hDDC->initCfg.instId;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,"\n+ DDC_cpmacDeleteInstance %d", instId);

    /* Check instance created global variable */
    if (CpmacDDCInstCreated[instId] == False) {
        CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacDeleteInstance:%d: Instance NOT created / Already deleted",
             instId);
        return (CPMAC_ERR_DEV_NOT_INSTANTIATED);
    }

    /* Update instance created global variable */
    CpmacDDCInstCreated[instId] = False;
    CpmacDDCObject[instId] = NULL;
    --CpmacDDCNumInst;

    /* Free resources allocated */
    if (hDDC != NULL) {
        retCode = PAL_osMemFree(0, hDDC, sizeof(CpmacDDCObj));
        if (retCode != PAL_SOK) {
            CPMAC_DDC_LOGERR
                ("\nERROR:DDC_cpmacDeleteInstance:%d: Failed to free memory of DDC Instance Object. Error=%08X",
                 instId, retCode);
            hDDC->ddcIf = NULL;
        }
        hDDC = NULL;
    }

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT, "\n- DDC_cpmacDeleteInstance:%d:", instId);
    return (retCode);
}


/************************** CPPI41 Init config info **************************/

/*****************************************************************************/


/**
 *  DDC_cpmacInit
 *    - validates max TX/RX channels and stores initial configuration
 *
 * \note Initial configuration passed by DDA via the "initCfg" parameter
 */
PRIVATE PAL_Result DDC_cpmacInit(CpmacDDCObj * hDDC,
                                 CpmacInitConfig * initCfg)
{
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY, "\n+ DDC_cpmacInit %d",
                     hDDC->ddcObj.instId);

    /* Validate numTx and numRx channels */
    if ((initCfg->numTxChannels > CPMAC_MAX_TX_CHANNELS) ||
        (initCfg->numRxChannels > CPMAC_MAX_RX_CHANNELS)) {
        CPMAC_DDC_LOGERR
            ("\nERROR:DDC_cpmacInit:%d: Invalid number of TX/RX channels",
             hDDC->ddcObj.instId);
        return (CPMAC_INVALID_PARAM);
    }

    /* Save config info for later use */
    hDDC->initCfg = *initCfg;   /* Structure copy */

    /* Connect to the CPPI4 PAL 
     * !@1 Note: Using global init structure instead of the one contained in hDDC
     * since it is overwritten in cpmac_net_get_config.
     */
    /* avalanche_cppi_init(); */
    hDDC->cppi4PAL = PAL_cppi4Init(NULL, NULL);

    if (hDDC->cppi4PAL == NULL) {
        CPMAC_DDC_LOGERR
            ("\nERROR:DDC_cpmacInit:%d: Error from CPPI4 PAL Init()",
             hDDC->ddcObj.instId);

        return (CPMAC_ERR_CPPI_INIT);
    }

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT, "\n- DDC_cpmacInit:%d:",
                     hDDC->ddcObj.instId);
    return (CPMAC_SUCCESS);
}

/**
 *  DDC_cpmacDeInit
 *    - stub code, no implementation needed here
 */
PRIVATE PAL_Result DDC_cpmacDeInit(CpmacDDCObj * hDDC, Ptr param)
{
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n- DDC_cpmacDeInit %d : NULL FUNCTION",
                     hDDC->ddcObj.instId);

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT, "\n- DDC_cpmacDeInit:%d:",
                     hDDC->ddcObj.instId);

    return (CPMAC_SUCCESS);
}

/**
 *  DDC_cpmacOpen
 *    - Brings module out of reset (via PAL_cppi4Init)
 *    - Open's CSL, programs mandatory hardware init registers
 *    - Open's MII_MDIO module and enable poll timer via DDA
 *    - Enables earlier created TX/RX channels
 *    - Enables TX/RX operation in hardware
 *
 *  \note "param" not used in this implementation
 */
PRIVATE PAL_Result DDC_cpmacOpen(CpmacDDCObj * hDDC, Ptr param)
{
    PAL_Result retCode;
    Uint32 miiModId, miiRevMaj, miiRevMin;
    PAL_Result retVal;
    CpmacInitConfig *initCfg;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY, "\n+ DDC_cpmacOpen:%d:",
                     hDDC->ddcObj.instId);

    if (hDDC->ddcObj.state == DDC_OPENED) {
        CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacOpen:%d: Device already open",
                         hDDC->ddcObj.instId);
        return (CPMAC_ERR_DEV_ALREADY_OPEN);
    }

    /* If not already out of reset, bring MII out of reset */
    PAL_sysResetCtrl(hDDC->initCfg.mdioResetLine, OUT_OF_RESET);
    PAL_osWaitMsecs(1);

    /* Get init config info structure pointer for easy access */
    initCfg = &hDDC->initCfg;
    hDDC->regs = (CSL_Cpgmac_f_RegsOvly) initCfg->baseAddress;

    /* Enable Adapter check interrupts - disable stats interupt */
    hDDC->regs->Mac_IntMask_Set = CPMAC_MAC_HOST_ERR_INTMASK_VAL;

    /* Set device state - Opened - useful when opening channels */
    hDDC->ddcObj.state = DDC_OPENED;

    /* Read RX Address matching/filtering Type (0/1/2) */
    /* This line was moved here because the MacControl fields used in
       cpmacSetMacHwCfg() are now dependent upon the RxAddrType */
    /* For the old CPMAC - the Cfig register doesn't really exist and the
       implementor has mapped it to a reserved register - not a good practice
       and may not always be safe to do so.  At a minimum this should be
       documented. */
    hDDC->RxAddrType = (hDDC->regs->Mac_Cfig >> 8) & 0xFF;

    /* Set the MacControl register */
    cpmacSetMacHwCfg(hDDC);

    /* Start MDIO Autonegotiation and set Phy mode */
    cpswHalCommonMiiMdioGetVer(initCfg->mdioBaseAddress, &miiModId,
                               &miiRevMaj, &miiRevMin);

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_PORT_UPDATE,
                     "\nDDC_cpmacOpen:%d: MII Module Id=%d, MII Base Address=%08X, Major Rev=%d, Minor Rev=%d",
                     hDDC->ddcObj.instId, miiModId, miiRevMaj, miiRevMin);

    /* Allocate memory for the PHY device and initialize the module */
    retCode = PAL_osMemAlloc(0, cpswHalCommonMiiMdioGetPhyDevSize(), 0,
                             (Ptr *) & hDDC->PhyDev);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:DDC_cpmacOpen:%d: Failed to allocate memory for Phy Device",
             hDDC->ddcObj.instId);
        return (retCode);
    }

    /* \note No failure code returned from this function */
    cpswHalCommonMiiMdioInit(hDDC->PhyDev,
                             initCfg->mdioBaseAddress,
                             hDDC->ddcObj.instId,
                             initCfg->PhyMask,
                             initCfg->MLinkMask,
                             initCfg->PhyMask,  /* this is new parameter mdix mask same as phymask */
                             0,     /* ResetBase not required */
                             initCfg->mdioResetLine,    /* Reset line not required */
                             initCfg->MdioBusFrequency,
                             initCfg->MdioClockFrequency,
                             (CpmacDDCDebug & CPMAC_DEBUG_MII),
                             (OsPrintf) hDDC->ddaIf->ddaPrintf);

    /* Set the PHY to a given mode per config parameters and update DDA layer */
    cpmacSetPhyMode(hDDC);
    hDDC->ddaIf->ddaNetIf.ddaFuncTable.ddaControlCb(hDDC->ddcObj.hDDA,
                                                    CPMAC_DDA_IOCTL_STATUS_UPDATE,
                                                    (Ptr) & hDDC->status,
                                                    NULL);

    /* Start the tick timer via DDA */
    hDDC->ddaIf->ddaNetIf.ddaFuncTable.ddaControlCb(hDDC->ddcObj.hDDA,
                                                    CPMAC_DDA_IOCTL_TIMER_START,
                                                    (Ptr) initCfg->
                                                    MdioTickMSec, NULL);

    /* Enable Opened TX Channel */
    if (hDDC->txCppi != NULL) {
        retVal = cpmacEnableChannel(hDDC, 0, DDC_NET_CH_DIR_TX);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:DDC_cpmacOpen:%d: Error enabling TX channel",
                 hDDC->ddcObj.instId);

            return (retVal);
        }
        hDDC->txCppi->listEntryPtr = PAL_cppi4AccChGetNextList(hDDC->txCppi->txAccChHnd);
    }

    /* Disable unicast on all channels first - enabled if channel is configured
       & enabled below */
    hDDC->regs->Rx_Unicast_Clear = CPMAC_RX_UNICAST_CLEAR_ALL;

    /* Set MAC Hash register */
    hDDC->regs->MacHash1 = hDDC->MacHash1;
    hDDC->regs->MacHash2 = hDDC->MacHash2;

    /* RX MBP, RX pkt length and RX buffer offset registers configured here */
    cpmacSetRxHwCfg(hDDC);

    /* Use external DMA Configuration (default) */
    hDDC->regs->QueueInfo = (3<<14) | 0xFFF;

#if 0 
    /* Set the Tx Cell FIFO threshold to 128 bytes instead of default 
     * value of 64 bytes  This required to get 100% performance  in 
     * SR mode with 1518 bytes packet size.
     */
    hDDC->regs->FifoControl = (hDDC->regs->FifoControl & ~0x1F) | 0x12;
#endif
    
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_GENERAL,"Queue info @ %x value %x\n", 
	    &hDDC->regs->QueueInfo, hDDC->regs->QueueInfo);

    /* Initialize the RX Address Logic. */
    cpmacTypeXAddrInit(hDDC);
    
#ifdef CONFIG_ARM_AVALANCHE_PPD
    /* Enable Opened Inframode Tx Channel */
    if (hDDC->infraTxCppi != NULL) {
        retVal = cpmacEnableEth2HostProxyChannel(hDDC, 0, DDC_NET_CH_DIR_TX);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:DDC_cpmacOpen:%d: Error enabling TX channel",
                 hDDC->ddcObj.instId);
            return (retVal);
        }
        
    }
    /* Enable Opened RX Endpoint Channel */
    if (hDDC->epRxCppi != NULL) {
        retVal = cpmacEnableEmbChannel(hDDC, 0, DDC_NET_CH_DIR_RX);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacOpen:%d: Error enabling RX channel",hDDC->ddcObj.instId);
            return (retVal);
        }
        
    }
#endif	
    /* Enable Opened RX Channel */
    if (hDDC->rxCppi != NULL) {
        retVal = cpmacEnableChannel(hDDC, 0, DDC_NET_CH_DIR_RX);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacOpen:%d: Error enabling RX channel",hDDC->ddcObj.instId);
            return (retVal);
        }
        hDDC->rxCppi->listEntryPtr = PAL_cppi4AccChGetNextList(hDDC->rxCppi->rxAccChHnd);        
    }
    /* Finally Set MAC Control register, Enable MII */
    hDDC->MacControl |= (1 << CSL_CPMAC_MACCONTROL_MIIEN_SHIFT);
    hDDC->regs->MacControl = hDDC->MacControl;

    /* Start the MIB cnt tick timer via DDA */
    hDDC->ddaIf->ddaNetIf.ddaFuncTable.ddaControlCb(hDDC->ddcObj.hDDA,
                                                    CPMAC_DDA_IOCTL_MIB64_CNT_TIMER_START,
                                                    (Ptr) initCfg->
                                                    Mib64CntMsec, NULL);

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT, "\n- DDC_cpmacOpen:%d:",
                     hDDC->ddcObj.instId);



    return (CPMAC_SUCCESS);
}

/**
 *  DDC_cpmacClose
 *    - Disables poll timer via DDA
 *    - Disable and Close all open TX/RX channels
 *    - Closes CSL
 *    - Puts module in reset (via PAL_cppi4Exit)
 *
 *  \note "param" not used in this implementation
 */
PRIVATE PAL_Result DDC_cpmacClose(CpmacDDCObj * hDDC, Ptr param)
{
    PAL_Result retVal;
    PAL_Result errVal = CPMAC_SUCCESS;
    Uint32 modeDescType;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY, "\n+ DDC_cpmacClose:%d:",
                     hDDC->ddcObj.instId);

    if (hDDC->ddcObj.state == DDC_CLOSED) {
        CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacClose:%d: Device already closed",
             hDDC->ddcObj.instId);
        return (CPMAC_ERR_DEV_ALREADY_CLOSED);
    }

    /* Stop the tick timer via DDA */
    hDDC->ddaIf->ddaNetIf.ddaFuncTable.ddaControlCb(hDDC->ddcObj.hDDA,
                                                    CPMAC_DDA_IOCTL_TIMER_STOP,
                                                    NULL, NULL);

    /* Stop the mib timer via DDA */
    hDDC->ddaIf->ddaNetIf.ddaFuncTable.ddaControlCb(hDDC->ddcObj.hDDA,
                                                    CPMAC_DDA_IOCTL_MIB64_CNT_TIMER_STOP,
                                                    NULL, NULL);

    /* Close TX Channel */
#ifdef CONFIG_ARM_AVALANCHE_PPD
    /* In Case of PPD Disable the Rx Endpoint Channel */
    if (hDDC->epRxCppi != NULL) {
        modeDescType = CPPI41_DMA_MODE_ENDPOINT | (CPPI41_DESC_TYPE_EMBEDDED << 0x8);
        retVal = DDC_cpmacChClose(hDDC, 0, DDC_NET_CH_DIR_RX, (Ptr)modeDescType);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacClose:%d: Error closing TX channel",
                 hDDC->ddcObj.instId);
            errVal = retVal;
        }
    }
    /* Wait for some time so all the received packets which are in MPDSP or CPDSP 
     * are drained out from the Infra mode Queue
     * Below delay needs to be tuned if required
     */
    PAL_osWaitMsecs(DDC_CPMAC_SR_DELAY1);
    /* Now Disable the Inframode Tx and Rx channels.
     * Also drain the Inframode Rx queue if there are any 
     * Embedded descriptors to be transmitted to host
     */
    if(hDDC->rxCppi != NULL)
    {
        modeDescType = (CPPI41_DMA_MODE_INFRA | (CPPI41_DESC_TYPE_HOST << 0x8));
        retVal = DDC_cpmacChClose(hDDC, 0, DDC_NET_CH_DIR_RX, (Ptr)modeDescType);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacClose:%d: Error closing TX channel",
                 hDDC->ddcObj.instId);
            errVal = retVal;
        }
    }
    if(hDDC->infraTxCppi != NULL)
    {
        modeDescType = (CPPI41_DMA_MODE_INFRA | (CPPI41_DESC_TYPE_EMBEDDED << 0x8));
        retVal = DDC_cpmacChClose(hDDC, 0, DDC_NET_CH_DIR_TX, (Ptr)modeDescType);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacClose:%d: Error closing TX channel",
                 hDDC->ddcObj.instId);
            errVal = retVal;
        }
    }
#endif	

    /*  Disable Tx Channel */	
    if (hDDC->txCppi != NULL) {
        modeDescType = (CPPI41_DMA_MODE_ENDPOINT | (CPPI41_DESC_TYPE_HOST << 0x8));
        retVal = DDC_cpmacChClose(hDDC, 0, DDC_NET_CH_DIR_TX, (Ptr)modeDescType);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacClose:%d: Error closing TX channel",
                 hDDC->ddcObj.instId);
            errVal = retVal;
        }
    }
#ifndef CONFIG_ARM_AVALANCHE_PPD
    /* Close RX Channel */
    if (hDDC->rxCppi != NULL) {
        modeDescType = (CPPI41_DMA_MODE_ENDPOINT | (CPPI41_DESC_TYPE_HOST << 0x8));
        retVal = DDC_cpmacChClose(hDDC, 0, DDC_NET_CH_DIR_RX, (Ptr)modeDescType);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacClose:%d: Error closing RX channel",
                 hDDC->ddcObj.instId);
            errVal = retVal;
        }
    }
#endif
    /* If this is the last CPMAC instance, reset MII module */
    if (CpmacDDCNumInst == 1) {
        PAL_sysResetCtrl(hDDC->initCfg.mdioResetLine, IN_RESET);
        PAL_osWaitMsecs(1);
    }

#if 0
    /* Exit the CPPI4 instance, placing NWSS in reset if I'm the last one out */
    retVal = PAL_cppi4Exit(hDDC->cppi4PAL, NULL);
    if (retVal != CPMAC_SUCCESS) {
        CPMAC_DDC_LOGERR("\nERROR:DDC_cpmacClose: Error %08X from CPPI4 PAL_cppi4Exit()",
             retVal);
        errVal = retVal;
    }
#endif

    /* closed all channels successfully. Mark the DDC as closed */
    if (errVal == CPMAC_SUCCESS) {
        hDDC->ddcObj.state = DDC_CLOSED;
    }

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT, "\n- DDC_cpmacClose:%d:",
                     hDDC->ddcObj.instId);

    return (errVal);
}


/**
 *  DDC_cpmacControl
 *    - Get Software (DDC) and Hardware Versions
 *    - Set/Modify RX and MAC configuration
 *    - Get DDC/module status
 *    - Read/Write MII registers (via PHY)
 *    - Get/Clr Statistics (hardware counters)
 *    - Add/Del/ Multicast operations AllMulti Set/Clear operations
 *    - Type2/3 Filtering operation
 *
 *  \note "param" not used in this implementation
 */
PRIVATE PAL_Result DDC_cpmacControl(CpmacDDCObj * hDDC, Int cmd,
                                    Ptr cmdArg, Ptr param)
{
    switch (cmd) {
    case CPMAC_DDC_IOCTL_GET_SWVER:
        /* cmdArg is an ptr to an integer that will contain the integer version
           id and param is a double ptr to a string which will point to the
           static version string */
        *((Uint32 *) cmdArg) = (Uint32) ((CPMAC_DDC_MAJOR_VERSION << 16)
                                         | CPMAC_DDC_MINOR_VERSION);
        *((Char **) param) = (Char *) & CpmacDDCVersionString[0];
        break;

    case CPMAC_DDC_IOCTL_GET_HWVER:
        /* Read hardware versions only if device is in open state */
        /* cmdArg is a ptr to an integer that will contain Tx Id ver and param is
           a pointer to an integer that will contain Rx Id ver after this call */
        if (hDDC->ddcObj.state == DDC_OPENED) {
            *((Uint32 *) param) = hDDC->regs->IdVer;
        } else {
            return (CPMAC_ERR_DEV_NOT_OPEN);
        }
        break;

    case CPMAC_DDC_IOCTL_SET_RXCFG:
        /* Rx configuration structure passed in structure pointed by cmdArg,
           params not used */
        if (cmdArg != NULL) {
            hDDC->initCfg.rxCfg = *((CpmacRxConfig *) cmdArg);
            cpmacSetRxHwCfg(hDDC);
        } else {
            return (CPMAC_INVALID_PARAM);
        }
        break;

    case CPMAC_DDC_IOCTL_SET_MACCFG:
        /* Mac configuration structure passed in a structure pointed by cmdArg,
           params not used */
        if (cmdArg != NULL) {
            hDDC->initCfg.macCfg = *((CpmacMacConfig *) cmdArg);
            cpmacSetMacHwCfg(hDDC);
        } else {
            return (CPMAC_INVALID_PARAM);
        }
        break;

    case CPMAC_DDC_IOCTL_GET_STATUS:
        /* Returns CpmacDDCStatus structure back via cmdArg */
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
		    ("\nWARN: DDC_cpmacControl:%d: DDC_CPMAC_IOCTL_GET_STATUS Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        } else {
            CpmacDDCStatus *status = (CpmacDDCStatus *) cmdArg;
            *status = hDDC->status;     /* structure copy */
        }
        break;

    case CPMAC_DDC_IOCTL_READ_PHY_REG:
        /* cmdArg = pointer to CpmacPhyParams struct. data read back into "data"
           parameter in the structure */
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_READ_PHY_REG Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        } else {
            /* \warning: Read to the phy registers - Note that this code loops
               on a completion bit in the phy so there are chances of hanging" */
            CpmacPhyParams *phyParams = (CpmacPhyParams *) cmdArg;
            phyParams->data =
                _cpswHalCommonMiiMdioUserAccessRead(hDDC->PhyDev,
                                                    phyParams->regAddr,
                                                    phyParams->phyNum);
        }
        break;

    case CPMAC_DDC_IOCTL_WRITE_PHY_REG:
        /* cmdArg = pointer to CpmacPhyParams struct. data to be written is in
           "data" parameter in the structure */
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_WRITE_PHY_REG Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        } else {
            CpmacPhyParams *phyParams = (CpmacPhyParams *) cmdArg;

            /* \warning: Write to the phy registers - Note that this code loops
               on a completion bit in the phy so there are chances of hanging" */
            _cpswHalCommonMiiMdioUserAccessWrite(hDDC->PhyDev,
                                                 phyParams->regAddr,
                                                 phyParams->phyNum,
                                                 phyParams->data);
        }
        break;

    case CPMAC_DDC_IOCTL_GET_STATISTICS:
        /* cmdArg points to the user provided structure for statistics which
           match with hardware 36 regs, param is not used */
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_GET_STATISTICS Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        } else {
            Uint32 cnt;
            Uint32 *userStats = (Uint32 *) cmdArg;
            volatile Uint32 *addr = (Uint32 *) & hDDC->regs->RxGoodFrames;

            for (cnt = 0; cnt < CPMAC_NUM_STAT_REGS;
                 cnt++, userStats++, addr++) {
                *userStats = *addr;
            }
        }
        break;

    case CPMAC_DDC_IOCTL_CLR_STATISTICS:
        /* cmdArg or param is not used */
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_CLR_STATISTICS Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        } else {
            Uint32 cnt;
            volatile Uint32 *addr = (Uint32 *) & hDDC->regs->RxGoodFrames;

            for (cnt = 0; cnt < CPMAC_NUM_STAT_REGS; cnt++, addr++) {
                *addr = CPMAC_STAT_CLEAR;       /* 0xFFFFFFFF value */
            }
            cpmacDDCIfcntClear(hDDC);
        }
        break;

    case CPMAC_DDC_IOCTL_MULTICAST_ADDR:
        /* cmdArg= CpmacMulticastOper enum, param= ptr to mcast address - Uint8 */
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_MULTICAST_ADDR Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        } else {
            Uint8 *addr = (Uint8 *) param;
            cpmacSingleMulti(hDDC, (CpmacSingleMultiOper) cmdArg, addr);
        }
        break;

    case CPMAC_DDC_IOCTL_ALL_MULTI:
        /* cmdArg= CpmacAllMultiOper enum, param=not used */
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_ALL_MULTI Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        }
        cpmacAllMulti(hDDC, (CpmacAllMultiOper) cmdArg);
        break;

    case CPMAC_DDC_IOCTL_TYPE2_3_FILTERING:
        {
            /* cmdArg = Pointer to CpmacType2_3_AddrFilterParams structure,
               param=not used */
            CpmacType2_3_AddrFilterParams *addrParams;
            if (hDDC->ddcObj.state != DDC_OPENED) {
                CPMAC_DDC_LOGERR
                    ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_TYPE2_3_FILTERING Ioctl called when device is NOT in open state",
                     hDDC->ddcObj.instId);
                return (CPMAC_ERR_DEV_NOT_OPEN);
            }
            addrParams = (CpmacType2_3_AddrFilterParams *) cmdArg;
            cpmacAddType2Addr(hDDC, addrParams->channel,
                              addrParams->macAddress, addrParams->index,
                              addrParams->valid, addrParams->match);
        }
        break;

    case CPMAC_DDC_IOCTL_SET_MAC_ADDRESS:
        {
            /* cmdArg = Pointer to CpmacType2_3_AddrFilterParams structure,
               param=not used */
            CpmacAddressParams *addrParams;
            Uint32 cnt;

            if (hDDC->ddcObj.state != DDC_OPENED) {
                CPMAC_DDC_LOGERR
                    ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_SET_MAC_ADDRESS Ioctl called when device is NOT in open state",
                     hDDC->ddcObj.instId);
                return (CPMAC_ERR_DEV_NOT_OPEN);
            }
            addrParams = (CpmacAddressParams *) cmdArg;

            for (cnt = 0; cnt < 6; cnt++)
                hDDC->macAddr[addrParams->channel][cnt] = addrParams->macAddress[cnt];

            /* Set interface MAC address */

            cpmacSetMacAddress(hDDC, addrParams->channel, addrParams->macAddress);
        }
        break;

        /* For now, all mac address stuff is required to be done via Control */
    case CPMAC_DDC_IOCTL_SET_SRC_MAC_ADDRESS:
        {
            Uint32 cnt;

            if (hDDC->ddcObj.state != DDC_OPENED) {
                CPMAC_DDC_LOGERR
                    ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_SET_SRC_MAC_ADDRESS Ioctl called when device is NOT in open state",
                     hDDC->ddcObj.instId);
                return (CPMAC_ERR_DEV_NOT_OPEN);
            }

            for (cnt = 0; cnt < 6; cnt++)
                hDDC->macSrcAddr[cnt] = ((Char *) cmdArg)[cnt];

            cpmacSetSrcMacAddress(hDDC, hDDC->macSrcAddr);
        }
        break;

    case CPMAC_DDC_IOCTL_IF_COUNTERS:
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_IF_COUNTERS Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        }
        cpmacDDCIfcntUpdt(hDDC);
        PAL_osMemCopy((char *) cmdArg,(char *) &hDDC->Mib2IfHCCounter.Mib2IfCounter, sizeof(struct mib2_ifCounters));
        break;

    case CPMAC_DDC_IOCTL_ETHER_COUNTERS:
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_ETHER_COUNTERS Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        }
        cpmacDDCPhycnt(hDDC, cmdArg);
        break;

    case CPMAC_DDC_IOCTL_IF_PARAMS_UPDT:
        if (hDDC->ddcObj.state != DDC_OPENED) {
            CPMAC_DDC_LOGERR
                ("\nWARN: DDC_cpmacControl:%d: CPMAC_DDC_IOCTL_IF_PARAMS_UPDT Ioctl called when device is NOT in open state",
                 hDDC->ddcObj.instId);
            return (CPMAC_ERR_DEV_NOT_OPEN);
        }
        cpmacDDCIfcntUpdt(hDDC);
        break;

    default:
        CPMAC_DDC_LOGERR
            ("\nWARN: DDC_cpmacControl:%d: Unhandled ioctl code %d",
             hDDC->ddcObj.instId, cmd);
        break;


    }

    return (CPMAC_SUCCESS);
}


/****************** DDC NET DEVICE INTERFACE TABLE FUNCTIONS ******************/
/**
 *  DDC_cpmacChOpen
 *    - Verify channel info (range checking etc)
 *    - Allocate memory for the channel
 *    - Book-keep operations for the channel - ready to be enabled in hardware
 *
 *  \note 1. If DDC instance is in "Opened" state, the channel is enabled in
 *           hardware
 */
PRIVATE PAL_Result DDC_cpmacChOpen(CpmacDDCObj * hDDC,
                                   CpmacChInfo * chInfo, Ptr chOpenArgs)
{
    PAL_Result retVal;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ DDC_cpmacChOpen:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, chInfo->chNum,
                     ((chInfo->chDir == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
                     
      /* If the channel state is not DDC_NET_CH_UNINITIALIZED, return error */
    if (chInfo->chState != DDC_NET_CH_UNINITIALIZED) {
        CPMAC_DDC_LOGERR
            ("\nERROR:DDC_cpmacChOpen:%d: %s ch %d  should be in DDC_NET_CH_UNINITIALIZED state",
             hDDC->ddcObj.instId,
             ((chInfo->chDir == DDC_NET_CH_DIR_TX) ? "TX" : "RX"),
             chInfo->chNum);
        return (CPMAC_INVALID_PARAM);
    }
    if (chInfo->chDir == DDC_NET_CH_DIR_TX)
    {
        /* Initialized the data structures for channel depending upon the
         * channel type and descriptor type
         */
	    
        retVal =  cpmacSetupTxChannel(hDDC,chInfo,chOpenArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:DDC_cpmacChOpen:%d: Error in setting up ch %d in %d direction",
                 hDDC->ddcObj.instId, chInfo->chNum, chInfo->chDir);
            return (retVal);
        }
    }
    if (chInfo->chDir == DDC_NET_CH_DIR_RX)
    {
        /* Initialized the data structures for channel depending upon the
         * channel type and descriptor type
         */
	    
        retVal =  cpmacSetupRxChannel(hDDC,chInfo,chOpenArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:DDC_cpmacChOpen:%d: Error in setting up ch %d in %d direction",
                 hDDC->ddcObj.instId, chInfo->chNum, chInfo->chDir);
            return (retVal);
        }
    }
   
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- DDC_cpmacChOpen:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, chInfo->chNum,
                     ((chInfo->chDir == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    return (CPMAC_SUCCESS);
}

PRIVATE PAL_Result cpmacTearDownRxChannel(CpmacDDCObj * hDDC, Int channel,
                                         Ptr chCloseArgs)
{
    PAL_Result retVal;
    Uint32 dmaMode;
    Uint32 descType;
    Int direction = DDC_NET_CH_DIR_RX;
    dmaMode = (Uint32)chCloseArgs & 0xFF;
    descType = ((Uint32)chCloseArgs & 0xFF00)>>8;


	
    if(((dmaMode == CPPI41_DMA_MODE_ENDPOINT) || (dmaMode == CPPI41_DMA_MODE_INFRA)) 
        && (descType == CPPI41_DESC_TYPE_HOST))
    {
        if (hDDC->rxIsOpen == False)
            return (CPMAC_ERR_RX_CH_ALREADY_CLOSED);	    
        if (hDDC->ddcObj.state == DDC_OPENED)
        {
            if(dmaMode == CPPI41_DMA_MODE_ENDPOINT )
            {
                retVal = cpmacDisableChannel(hDDC, channel, direction);
               if (retVal != CPMAC_SUCCESS) {
                   CPMAC_DDC_LOGERR
                     ("\nERROR:cpmacTearDownRxChannel:%d: Error disabling ch %d in %d direction",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
                   return (retVal);
                }
            }
#ifdef CONFIG_ARM_AVALANCHE_PPD
			if(dmaMode == CPPI41_DMA_MODE_INFRA )
            {
                retVal = cpmacDisableEth2HostProxyChannel(hDDC, channel, direction);
               if (retVal != CPMAC_SUCCESS) {
                   CPMAC_DDC_LOGERR
                     ("\nERROR:cpmacTearDownRxChannel:%d: Error disabling ch %d in %d direction",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
                   return (retVal);
                }
            }
#endif
            
        }
        retVal = cpmacDeInitRxChannel(hDDC, channel, chCloseArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacTearDownRxChannel:%d: Error in DeInit of RX ch %d",
                 hDDC->ddcObj.instId, channel);
            return (retVal);
        }                  
    }
#ifdef CONFIG_ARM_AVALANCHE_PPD
    if((dmaMode == CPPI41_DMA_MODE_ENDPOINT)  && 
	    (descType == CPPI41_DESC_TYPE_EMBEDDED))
    {
         if (hDDC->ddcObj.state == DDC_OPENED) {
             retVal = cpmacDisableEmbChannel(hDDC, channel, direction);
            if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                    ("\nERROR:cpmacTearDownRxChannel:%d: Error disabling ch in %d direction",
                     hDDC->ddcObj.instId,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
                return (retVal);
            }
        }
        retVal = cpmacDeInitEmbRxChannel(hDDC, channel, chCloseArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacTearDownRxChannel:%d: Error in DeInit of Embedded RX channel",
                 hDDC->ddcObj.instId);
            return (retVal);
        }

    }
#endif
    return CPMAC_SUCCESS;
}

PRIVATE PAL_Result cpmacTearDownTxChannel(CpmacDDCObj * hDDC, Int channel,
                                         Ptr chCloseArgs)
{
    PAL_Result retVal;
    Uint32 dmaMode;
    Uint32 descType;
    Int direction = DDC_NET_CH_DIR_TX; 
    dmaMode = (Uint32)chCloseArgs & 0xFF;
    descType = ((Uint32)chCloseArgs & 0xFF00)>>8;

    if((dmaMode == CPPI41_DMA_MODE_ENDPOINT) 
	    && (descType == CPPI41_DESC_TYPE_HOST))
    {
        if (hDDC->txIsOpen == False)
            return (CPMAC_ERR_TX_CH_ALREADY_CLOSED);
        if (hDDC->ddcObj.state == DDC_OPENED) {
            retVal = cpmacDisableChannel(hDDC, channel, direction);
            if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                    ("\nERROR:cpmacTearDownTxChannel:%d: Error disabling ch %d in %d direction",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
                return (retVal);
            }
        }
        retVal = cpmacDeInitTxChannel(hDDC, channel, chCloseArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacTearDownTxChannel:%d: Error in DeInit of TX ch %d",
                 hDDC->ddcObj.instId, channel);
            return (retVal);
        }
    }
#ifdef CONFIG_ARM_AVALANCHE_PPD
    if((dmaMode == CPPI41_DMA_MODE_INFRA)
	    && (descType == CPPI41_DESC_TYPE_EMBEDDED))
    {
        if (hDDC->ddcObj.state == DDC_OPENED) {
            retVal = cpmacDisableEth2HostProxyChannel(hDDC, channel, direction);
            if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                    ("\nERROR:cpmacTearDownTxChannel:%d: Error disabling ch %d in %d direction",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
                return (retVal);
            }
        }
        retVal = cpmacDeInitEth2HostProxyTxChannel(hDDC, channel, chCloseArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacTearDownTxChannel:%d: Error in DeInit of Eth2HostProxy TX ch %d",
                 hDDC->ddcObj.instId, channel);
            return (retVal);
        }
        
        
    }
#endif
    return CPMAC_SUCCESS;
}


/**
 *  DDC_cpmacChClose
 *    - If DDC instance is in "Opened" state, disable the channel in hardware
 *    - De-initialize the channel (free memory previously allocated)
 *    - chCloseArgs is used to pass the mode of the channel and descriptor
 *      types used by channel to close the channel in appropriate way
 */
PRIVATE PAL_Result DDC_cpmacChClose(CpmacDDCObj * hDDC, Int channel,
                                    Int direction, Ptr chCloseArgs)
{
    PAL_Result retVal;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ DDC_cpmacChClose:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    if (direction == DDC_NET_CH_DIR_TX) {
	    retVal = cpmacTearDownTxChannel(hDDC,channel,chCloseArgs);
	    if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                    ("\nERROR:DDC_cpmacChClose:%d: Error Destroying ch %d in %d direction",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
                return (retVal);
            }
    }
    else if (direction == DDC_NET_CH_DIR_RX) {   
        retVal = cpmacTearDownRxChannel(hDDC,channel,chCloseArgs);
	    if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                    ("\nERROR:DDC_cpmacChClose:%d: Error Destroying ch %d in %d direction",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
                return (retVal);
            }
    }

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- DDC_cpmacChClose:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    return (CPMAC_SUCCESS);
}
/**
 *  cpmacInitEth2HostProxyTxChannel
 *    - Allocates memory for Eth2host proxy inframode TX Ch Control structure, 
 *    - Stores the cppi specific configuration provided in 'chInfo' for quick
 *    access at txCppi level.
 *
 *  \note 1. "chOpenArgs" not used in this implementation
 *        2. This function assumes that the channel number passed is valid and
 *           the hDDC->infraTxCppi pointer is NULL.
 *        3. This function will not do any error check on these initialization
 *        parameters to avoid duplicate error checks (done in caller function
 *        or init parse functions)..
 */
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacInitEth2HostProxyTxChannel(CpmacDDCObj * hDDC,
                                      CpmacChInfo * chInfo, Ptr chOpenArgs)
{
    PAL_Result retCode;
    CpmacTxCppiCh *infraTxCppi = NULL;
    
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacInitEth2HostProxyTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);

    retCode = PAL_osMemAlloc(0, sizeof(CpmacTxCppiCh), 0, (Ptr *) & infraTxCppi);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacInitEth2HostProxyTxChannel:%d: Failed to allocate memory for TX CPPI Ch %d",
             hDDC->ddcObj.instId, chInfo->chNum);
        return (retCode);
    }
	PAL_osMemSet(infraTxCppi, 0, sizeof(CpmacTxCppiCh));

    hDDC->infraTxCppi = infraTxCppi;

#ifdef CPMAC_INCLUDE_ASSERT
    infraTxCppi->hDDC = hDDC;
#endif
    infraTxCppi->chInfo = *chInfo;  
    infraTxCppi->chInfo.chState = DDC_NET_CH_INITIALIZED;

    
    infraTxCppi->txCmplQueue = chInfo->cppi4TxChInfo.txCompQueue;
    hDDC->txIsCreated = True;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacInitEth2HostProxyTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);
    
    return (CPMAC_SUCCESS);
}
#endif
/**
 *  cpmacInitTxChannel
 *    - Allocates memory for TX Ch Control structure, Buffer descriptors
 *    - Initializes the above data structures as per channel configuration
 *    - Stores the cppi specific configuration provided in 'chInfo' for quick
 *    access at txCppi level.
 *    - Sets up accumulator list memory for tx complete as per the number of
 *    pages.  - Initializes each descriptor and chain the TX BD list ready to
 *    be given to hardware
 *
 *  \note 1. "chOpenArgs" not used in this implementation
 *        2. This function assumes that the channel number passed is valid and
 *           the hDDC->txCppi pointer is NULL.
 *        3. This function will not do any error check on these initialization
 *        parameters to avoid duplicate error checks (done in caller function
 *        or init parse functions). Also numBD value and alignment requirement
 *        should be validated as per the specs during init time (eg. in
 *        cpmac_net_get_config).
 */
PRIVATE PAL_Result cpmacInitTxChannel(CpmacDDCObj * hDDC,
                                      CpmacChInfo * chInfo, Ptr chOpenArgs)
{
    PAL_Result retCode;
    Uint32 cnt;
    CpmacHostDesc* currBD;
    CpmacTxCppiCh *txCppi = NULL;
    Cppi4AccumulatorCfg* accChCfg;
    
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacInitTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);

    retCode =
        PAL_osMemAlloc(0, sizeof(CpmacTxCppiCh), 0, (Ptr *) & txCppi);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacInitTxChannel:%d: Failed to allocate memory for TX CPPI Ch %d",
             hDDC->ddcObj.instId, chInfo->chNum);
        return (retCode);
    }
    PAL_osMemSet(txCppi, 0, sizeof(CpmacTxCppiCh));

    hDDC->txCppi = txCppi;

#ifdef CPMAC_INCLUDE_ASSERT
    txCppi->hDDC = hDDC;
#endif
    txCppi->chInfo = *chInfo;  
    txCppi->chInfo.chState = DDC_NET_CH_INITIALIZED;

    txCppi->numBD = chInfo->numBD;
    txCppi->numTxQs = chInfo->numTxQs;
    for (cnt = 0; cnt < chInfo->numTxQs; cnt++) {
        txCppi->txQueue [cnt] = chInfo->cppi4TxChInfo.txInQueue [cnt];
    }    
    txCppi->txCmplQueue = chInfo->cppi4TxChInfo.txCompQueue;

    
    accChCfg = &txCppi->chInfo.txAccChInfo;
    accChCfg->list.listEntrySize     = CPGMAC_ACC_ENTRY_SIZE_VAL;
    accChCfg->list.listCountMode     = CPGMAC_ACC_LIST_MODE;
    accChCfg->list.maxPageEntry      = CPGMAC_ACC_TX_MAXENTRIES;
    accChCfg->list.pacingMode        = CPGMAC_ACC_INTR_MODE;
    accChCfg->pacingTickCnt     = CPGMAC_ACC_INTR_DELAY;
    accChCfg->list.maxPageCnt        = CPGMAC_ACC_LIST_DIV;
    accChCfg->list.stallAvoidance    = CPGMAC_ACC_STALL_AVOID;
    accChCfg->queue             = txCppi->txCmplQueue; 
    accChCfg->mode              = 0;
    /*accChCfg->entryCntThrsh = CPGMAC_ACC_ENTRY_THRSH;*/

    txCppi->accChNum    = accChCfg->accChanNum;
    txCppi->accVecNum   = chInfo->accVecNum;
    
    /* !@0 TODO: Do we need to specify alignment for list region as
     * CPGMAC_ACC_ENTRY_SIZE ?
     */ 
    txCppi->accListSize = (CPGMAC_ACC_LIST_DIV) * ((CPGMAC_ACC_TX_MAXENTRIES) 
                            * (CPGMAC_ACC_ENTRY_SIZE));
    retCode = PAL_osMemAlloc(0, txCppi->accListSize, 0, &accChCfg->list.listBase);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR ("\nERROR:cpmacInitTxChannel:%d: Failed to allocate "
                "%d bytes for accumulator list for ch %d", hDDC->ddcObj.instId,
                txCppi->accListSize, chInfo->chNum); 
        
        /* !@0 TODO: Before returning we should do the cleanup for previous
         * successful allocations (applicable to most such error cases!)
         */
        
        return (retCode);
    }
    
    for (cnt = 0; cnt < CPGMAC_ACC_LIST_DIV; cnt++)
        txCppi->listBuffBase[cnt] = (Ptr) ((Uint32)accChCfg->list.listBase + 
                    (cnt * (CPGMAC_ACC_TX_MAXENTRIES) * (CPGMAC_ACC_ENTRY_SIZE)));
    
    for (cnt  = 0; cnt < CPGMAC_ACC_LIST_DIV; ++cnt)
        *(Uint32*)txCppi->listBuffBase[cnt] = 0;
    
    PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE(accChCfg->list.listBase, txCppi->accListSize);

    txCppi->chInfo.cppi4TxChInfo.tdQueue = txCppi->txCmplQueue;

    hDDC->txTeardownPending = False;    /* Clear the TX teardown pending flag */

    /* !@0 TODO: We may need to check numBD here since CPPI4.1 requires the
     * desc count to be * multiple of 2 power 5 (ie 32). Similarly, the desc
     * size should also be multiple of 32.
     */
    txCppi->allocSize = sizeof(CpmacHostDesc) * chInfo->numBD;
    txCppi->bdMem = PAL_cppi4AllocDesc (hDDC->cppi4PAL, 
                        txCppi->txQueue[0].qMgr, txCppi->numBD, 
                        txCppi->chInfo.descAlignment);
    if (txCppi->bdMem == NULL) { 
        CPMAC_DDC_LOGERR ("\nERROR:cpmacInitTxChannel: Failed to allocate Tx descriptors");
        return (CPMAC_ERR_CPPI_DESC_REGN_FAIL);
    }

    currBD = (CpmacHostDesc *) txCppi->bdMem;
    txCppi->bdPoolHead = 0;
    for (cnt = 0; cnt < chInfo->numBD; cnt++) {
        currBD->hwDesc.descInfo    = PAL_CPPI4_HOSTDESC_DESC_TYPE_HOST << PAL_CPPI4_HOSTDESC_DESC_TYPE_SHIFT;
        currBD->hwDesc.tagInfo     = 0;
        currBD->hwDesc.pktInfo     = (PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH << PAL_CPPI4_HOSTDESC_PKT_TYPE_SHIFT) 
                                | (PAL_CPPI4_HOSTDESC_PKT_RETPLCY_LINKED << PAL_CPPI4_HOSTDESC_PKT_RETPLCY_SHIFT)
                                | (PAL_CPPI4_HOSTDESC_DESC_LOC_OFFCHIP << PAL_CPPI4_HOSTDESC_DESC_LOC_SHIFT)
                                | (txCppi->txCmplQueue.qMgr << PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT)
                                | (txCppi->txCmplQueue.qNum << PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT);
        currBD->hwDesc.buffLen     = 0;
        currBD->hwDesc.bufPtr      = 0;
        currBD->hwDesc.nextBDPtr   = 0;
        currBD->hwDesc.orgBuffLen  = 0;
        currBD->hwDesc.orgBufPtr   = 0;
       
        currBD->nextSwBDPtr = (Ptr) txCppi->bdPoolHead;
        txCppi->bdPoolHead = currBD;
        currBD = (CpmacHostDesc*) ((Uint32)currBD + txCppi->chInfo.descAlignment);
    }


    hDDC->txIsCreated = True;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacInitTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);

    return (CPMAC_SUCCESS);
}
PRIVATE PAL_Result cpmacSetupRxChannel(CpmacDDCObj * hDDC,
                                   CpmacChInfo * chInfo, Ptr chOpenArgs)
{
    PAL_Result retVal;
    Uint16 pwr;
    
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacSetupRxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);
    
    if(((chInfo->dmaMode == CPPI41_DMA_MODE_ENDPOINT) || (chInfo->dmaMode == CPPI41_DMA_MODE_INFRA))
        && chInfo->cppi4RxChInfo.defDescType == CPPI41_DESC_TYPE_HOST)
    {
        if (chInfo->chNum >= hDDC->initCfg.numRxChannels) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupRxChannel:%d: Invalid RX Ch=%d specified",
                 hDDC->ddcObj.instId, chInfo->chNum);
            return (CPMAC_ERR_RX_CH_INVALID);
        }
        
        if (hDDC->rxIsOpen == True)
            return (CPMAC_ERR_RX_CH_ALREADY_OPENED);
    
        if (hDDC->rxIsCreated == True) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupRxChannel:%d: RX Ch %d already open",
                 hDDC->ddcObj.instId, chInfo->chNum);
            return (CPMAC_ERR_RX_CH_ALREADY_INIT);
        }
        /* Calculate the descriptor alignment as smallest power of 2 that is equal to
         * or greater than the descriptor size. Note that if we already have
         * confirmed the desc size to be multiple of 2^32, we can directly use the
         * desc size as alignment value.
         */
        chInfo->descAlignment = CPMAC_CPPI4x_RX_HOST_BD_SIZE; 
        if(sizeof(CpmacHostDesc) > chInfo->descAlignment) 
        {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupRxChannel:%d: BD size configured to be less than actual CPPI host BD size.",
                hDDC->ddcObj.instId, chInfo->chNum);            
            return (CPMAC_ERR_TX_CH_INVALID);
        } else {
               for (pwr = 0; (1UL << pwr) < chInfo->descAlignment; pwr++);
               if((1UL << pwr) != chInfo->descAlignment) {
                   CPMAC_DDC_LOGERR
                   ("\nERROR:cpmacSetupRxChannel:%d: BD size configured not a power of two.",
                    hDDC->ddcObj.instId, chInfo->chNum);            
                  return (CPMAC_ERR_TX_CH_INVALID);
    
               }
          }
        /* Allocate memory and perform other book-keep functions for the channel */
        retVal = cpmacInitRxChannel(hDDC, chInfo, chOpenArgs);
            if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                    ("\nERROR:cpmacSetupRxChannel:%d: Error in initializing RX ch %d",
                     hDDC->ddcObj.instId, chInfo->chNum);
                return (retVal);
            }
        /* If device is opened already, enable this channel for use */
        if (hDDC->ddcObj.state == DDC_OPENED) {
	        retVal = cpmacEnableChannel(hDDC, chInfo->chNum, chInfo->chDir);
            if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupRxChannel:%d: Error enabling ch %d in %d direction",
                 hDDC->ddcObj.instId, chInfo->chNum, chInfo->chDir);
                return (retVal);
            }
        }
        
    }
#ifdef CONFIG_ARM_AVALANCHE_PPD
    if((chInfo->dmaMode == CPPI41_DMA_MODE_ENDPOINT)
        && (chInfo->cppi4RxChInfo.defDescType == CPPI41_DESC_TYPE_EMBEDDED))
    {

        retVal = cpmacInitEmbRxChannel(hDDC, chInfo, chOpenArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupRxChannel:%d: Error in initializing RX ch %d",
                 hDDC->ddcObj.instId, chInfo->chNum);
            return (retVal);
        }
        if (hDDC->ddcObj.state == DDC_OPENED) {
            retVal = cpmacEnableEmbChannel(hDDC, chInfo->chNum, chInfo->chDir);
            if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupRxChannel:%d: Error enabling ch %d in %d direction",
                 hDDC->ddcObj.instId, chInfo->chNum, chInfo->chDir);
                return (retVal);
            }
        }
    }
#endif
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacSetupRxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);
    return CPMAC_SUCCESS;
}
PRIVATE PAL_Result cpmacSetupTxChannel(CpmacDDCObj * hDDC,
                                   CpmacChInfo * chInfo, Ptr chOpenArgs)
{
    PAL_Result retVal;
    Uint16 pwr;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacSetupTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);
    if((chInfo->dmaMode == CPPI41_DMA_MODE_ENDPOINT)
	    && (chInfo->cppi4TxChInfo.defDescType == CPPI41_DESC_TYPE_HOST))
    {
        if (hDDC->txIsOpen == True)
            return (CPMAC_ERR_TX_CH_ALREADY_OPENED);                         
    
        if (chInfo->chNum >= hDDC->initCfg.numTxChannels) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupTxChannel:%d: Invalid TX Ch=%d specified",
                hDDC->ddcObj.instId, chInfo->chNum);
            return (CPMAC_ERR_TX_CH_INVALID);
        }
    
        if (hDDC->txIsCreated == True) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupTxChannel:%d: TX Ch %d already open",
                 hDDC->ddcObj.instId, chInfo->chNum);
            return (CPMAC_ERR_TX_CH_ALREADY_INIT);
        }
        /* Calculate the descriptor alignment as smallest power of 2 that is equal to
         * or greater than the descriptor size. Note that if we already have
         * confirmed the desc size to be multiple of 2^32, we can directly use the
         * desc size as alignment value.
         */
        chInfo->descAlignment = CPMAC_CPPI4x_TX_HOST_BD_SIZE;  
        if(sizeof(CpmacHostDesc) > chInfo->descAlignment) 
        {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupTxChannel:%d: BD size configured to be less than actual CPPI host BD size.",
                hDDC->ddcObj.instId, chInfo->chNum);            
            return (CPMAC_ERR_TX_CH_INVALID);
        } else {
               for (pwr = 0; (1UL << pwr) < chInfo->descAlignment; pwr++);
               if((1UL << pwr) != chInfo->descAlignment) {
                   CPMAC_DDC_LOGERR
                   ("\nERROR:cpmacSetupTxChannel:%d: BD size configured not a power of two.",
                    hDDC->ddcObj.instId, chInfo->chNum);            
                  return (CPMAC_ERR_TX_CH_INVALID);
    
               }
          }
    
        /* Allocate memory and perform other book-keep functions for the channel */
        retVal = cpmacInitTxChannel(hDDC, chInfo, chOpenArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupTxChannel:%d: Error in initializing TX ch %d",
                 hDDC->ddcObj.instId, chInfo->chNum);
            return (retVal);
        }
        /* If device is opened already, enable this channel for use */
        if (hDDC->ddcObj.state == DDC_OPENED) {
            retVal = cpmacEnableChannel(hDDC, chInfo->chNum, chInfo->chDir);
            if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupTxChannel:%d: Error enabling ch %d in %d direction",
                 hDDC->ddcObj.instId, chInfo->chNum, chInfo->chDir);
                return (retVal);
            }
        }
    }
#ifdef CONFIG_ARM_AVALANCHE_PPD
    else if((chInfo->dmaMode== CPPI41_DMA_MODE_INFRA)
	    && (chInfo->cppi4TxChInfo.defDescType == CPPI41_DESC_TYPE_EMBEDDED))
    {
        retVal = cpmacInitEth2HostProxyTxChannel(hDDC, chInfo, chOpenArgs);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupTxChannel:%d: Error in initializing TX ch %d",
                hDDC->ddcObj.instId, txChInfo->chNum);
            return (retVal);
        }
        /* If device is opened already, enable this channel for use */
        if (hDDC->ddcObj.state == DDC_OPENED) {
            retVal = cpmacEnableEth2HostProxyChannel(hDDC, chInfo->chNum, chInfo->chDir);
            if (retVal != CPMAC_SUCCESS) {
                CPMAC_DDC_LOGERR
                ("\nERROR:cpmacSetupTxChannel:%d: Error enabling ch %d in %d direction",
                 hDDC->ddcObj.instId, chInfo->chNum, chInfo->chDir);
                return (retVal);
            }
        }
    }
#endif
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacSetupTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);
    return CPMAC_SUCCESS;
}
/**
 *  cpmacDeInitEth2HostProxyTxChannel
 *    - Frees memory previously allocated in InitEth2HostProxyTxChannel
 *
 *  \note 1. "chCloseArgs" not used in this implementation
 *        2. This function assumes that the channel number passed is valid and
 *           this function will not do any error check to avoid duplicate error
 *           checks (done in caller function).
 */
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacDeInitEth2HostProxyTxChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                        Ptr chCloseArgs)
{
    PAL_Result retCode;
    CpmacTxCppiCh *infraTxCppi;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacDeInitEth2HostProxyTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, channel);

    infraTxCppi = hDDC->infraTxCppi;
    /* Free the TX Channel structure */
    retCode = PAL_osMemFree(0, infraTxCppi, sizeof(CpmacTxCppiCh));
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitEth2HostProxyTxChannel:%d: Failed to free TX CPPI channel structure for Ch %d",
             hDDC->ddcObj.instId, channel);
    }
 
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacDeInitEth2HostProxyTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, channel);

    return (CPMAC_SUCCESS);
}
#endif
/**
 *  cpmacDeInitTxChannel
 *    - Frees memory previously allocated for Ch Control structure, buffer
 *      descriptors
 *
 *  \note 1. "chCloseArgs" not used in this implementation
 *        2. This function assumes that the channel number passed is valid and
 *           this function will not do any error check to avoid duplicate error
 *           checks (done in caller function).
 */
PRIVATE PAL_Result cpmacDeInitTxChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                        Ptr chCloseArgs)
{
    PAL_Result retCode;
    CpmacTxCppiCh *txCppi;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacDeInitTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, channel);
    /* Check if channel structure is already de-allocated */
    if (hDDC->txIsCreated == False) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitTxChannel:%d: TX CPPI Ch %d structure already freed",
             hDDC->ddcObj.instId, channel);
        return (CPMAC_ERR_TX_CH_ALREADY_CLOSED);
    }
    txCppi = hDDC->txCppi;

    retCode = PAL_cppi4DeallocDesc(hDDC->cppi4PAL, txCppi->txQueue[0].qMgr, txCppi->bdMem);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitTxChannel:%d: Failed to free tx desc region for Ch %d",
             hDDC->ddcObj.instId, channel);
    }

#ifdef CPGMAC_USE_ACC_LIST
    retCode = PAL_osMemFree(0, txCppi->chInfo.txAccChInfo.list.listBase, txCppi->accListSize);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitTxChannel:%d: Failed to free TX accumulator list for Ch %d",
             hDDC->ddcObj.instId, channel);
    }
#endif

    /* Free the TX Channel structure */
    retCode = PAL_osMemFree(0, txCppi, sizeof(CpmacTxCppiCh));
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitTxChannel:%d: Failed to free TX CPPI channel structure for Ch %d",
             hDDC->ddcObj.instId, channel);
    }

    hDDC->txCppi = NULL;
    hDDC->txIsCreated = False;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacDeInitTxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, channel);

    return (CPMAC_SUCCESS);
}
/**
 *  cpmacInitEmbRxChannel
 *    - Allocates memory for endpoint Rx Ch Control structure with embedded descriptors
 *    - Stores the cppi specific configuration provided in 'chInfo' for quick
 *    access at epRxCppi level.
 *
 *  \note 1. "chOpenArgs" not used in this implementation
 *        2. This function assumes that the channel number passed is valid and
 *           the hDDC->epRxCppi pointer is NULL.
 *        3. This function will not do any error check on these initialization
 *        parameters to avoid duplicate error checks (done in caller function
 *        or init parse functions)..
 */
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacInitEmbRxChannel(CpmacDDCObj * hDDC,
                                      CpmacChInfo * chInfo, Ptr chOpenArgs)
{
    PAL_Result retCode;
    CpmacRxCppiCh *epRxCppi = NULL;
    Cppi4RxChEmbeddedPktCfg *embeddedPktCfgPtr;
    
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacInitEmbRxChannel:%d: ",
                     hDDC->ddcObj.instId);
    
    retCode = PAL_osMemAlloc(0, sizeof(CpmacRxCppiCh), 0, (Ptr *) & epRxCppi);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacInitEmbRxChannel:%d: Failed to allocate memory for RX CPPI Ch",
             hDDC->ddcObj.instId);
        goto allocRxCppiFail;
    }
    PAL_osMemSet(epRxCppi, 0, sizeof(CpmacRxCppiCh));

    hDDC->epRxCppi = epRxCppi;
#ifdef CPMAC_INCLUDE_ASSERT
    epRxCppi->hDDC = hDDC;
#endif

    epRxCppi->chInfo = *chInfo;
    epRxCppi->chInfo.chState = DDC_NET_CH_INITIALIZED;
	
    epRxCppi->rxQueue = chInfo->cppi4RxChInfo.rxCompQueue;
    epRxCppi->fdbQueue[0] = chInfo->cppi4RxChInfo.u.embeddedPktCfg.fdQueue;
    embeddedPktCfgPtr = &(chInfo->cppi4RxChInfo.u.embeddedPktCfg);
  
    return (CPMAC_SUCCESS);
    
allocRxCppiFail:
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
            "\n- cpmacInitEmbRxChannel:%d: Ch=%d",
            hDDC->ddcObj.instId, chInfo->chNum);
    return (retCode);
}
#endif
/**
 *  cpmacInitRxChannel
 *    - Allocates memory for RX Ch structure and other internal structs 
 *    - Stores the cppi specific configuration provided in 'chInfo' for quick
 *    access at rxCppi level.
 *    - Sets up accumulator list memory for rx as per the number of pages.
 *    - prepares the rx packet list
 *
 *  \note 1. "chOpenArgs" unused
 *        2. This function assumes that the channel number passed is valid and
 *           the  hDDC->rxCppi pointer is NULL.
 *        3. This function will not do any error check on these initialization
 *        parameters to avoid duplicate error checks (done in caller function
 *        or init parse functions). Also numBD value and alignment requirement
 *        should be validated as per the specs during init time (eg. in
 *        cpmac_net_get_config).
 */
PRIVATE PAL_Result cpmacInitRxChannel(CpmacDDCObj * hDDC,
                                      CpmacChInfo * chInfo, Ptr chOpenArgs)
{
    PAL_Result retCode;
    CpmacRxCppiCh *rxCppi = NULL;
    Char *macAddr;
    CpmacHostDesc *currBD;
    Cppi4AccumulatorCfg* accChCfg;
    BdMemChunk *MemChunk;
    Uint32 cnt, lockKey;


    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacInitRxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);

    retCode = PAL_osMemAlloc(0, sizeof(CpmacRxCppiCh), 0, (Ptr *) & rxCppi);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacInitRxChannel:%d: Failed to allocate memory for RX CPPI Ch %d",
             hDDC->ddcObj.instId, chInfo->chNum);
        goto allocRxCppiFail;
    }
    PAL_osMemSet(rxCppi, 0, sizeof(CpmacRxCppiCh));

    hDDC->rxCppi = rxCppi;

#ifdef CPMAC_INCLUDE_ASSERT
    rxCppi->hDDC = hDDC;
#endif

    rxCppi->chInfo = *chInfo;
    rxCppi->chInfo.chState = DDC_NET_CH_INITIALIZED;
    
    rxCppi->rxQueue = chInfo->cppi4RxChInfo.rxCompQueue;
    for (cnt = 0; cnt < CPGMAC_NUM_RXFDB_QS; cnt++) {
        rxCppi->fdbQueue [cnt] = chInfo->cppi4RxChInfo.u.hostPktCfg.fdbQueue [cnt];
    }
    
    accChCfg = &rxCppi->chInfo.rxAccChInfo;
    accChCfg->list.listEntrySize     = CPGMAC_ACC_ENTRY_SIZE_VAL;
    accChCfg->list.listCountMode     = CPGMAC_ACC_LIST_MODE;
    accChCfg->list.maxPageEntry      = CPGMAC_ACC_RX_MAXENTRIES;
    accChCfg->list.pacingMode        = CPGMAC_ACC_INTR_MODE;
    accChCfg->pacingTickCnt     = CPGMAC_ACC_INTR_DELAY;
    accChCfg->list.maxPageCnt        = CPGMAC_ACC_LIST_DIV;
    accChCfg->list.stallAvoidance    = CPGMAC_ACC_STALL_AVOID;
    accChCfg->queue             = rxCppi->rxQueue;
    accChCfg->mode              = 0;
    /*accChCfg->entryCntThrsh = CPGMAC_ACC_ENTRY_THRSH;*/
    
    rxCppi->accChNum    = accChCfg->accChanNum;
    rxCppi->accVecNum   = chInfo->accVecNum;
    
    /* !@0 TODO: Do we need to specify alignment for list region as
     * CPGMAC_ACC_ENTRY_SIZE ?
     */ 
    rxCppi->accListSize = (CPGMAC_ACC_LIST_DIV) * ((CPGMAC_ACC_RX_MAXENTRIES) 
                            * (CPGMAC_ACC_ENTRY_SIZE));
    retCode = PAL_osMemAlloc(0, rxCppi->accListSize, 0, &accChCfg->list.listBase);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR ("\nERROR:cpmacInitRxChannel:%d: Failed to allocate "
                "%d bytes for accumulator list for ch %d", hDDC->ddcObj.instId,
                rxCppi->accListSize, chInfo->chNum); 
        
        /* !@0 TODO: Before returning we should do the cleanup for previous
         * successful allocations (applicable to most such error cases!)
         */
        
        return (retCode);
    }

    for (cnt = 0; cnt < CPGMAC_ACC_LIST_DIV; cnt++)
        rxCppi->listBuffBase[cnt] = (Ptr) ((Uint32)accChCfg->list.listBase + 
                    (cnt * (CPGMAC_ACC_RX_MAXENTRIES) * (CPGMAC_ACC_ENTRY_SIZE)));
    
    *(Uint32*)rxCppi->listBuffBase[0] = 0; 

    PAL_CPPI4_CACHE_WRITEBACK_INVALIDATE(accChCfg->list.listBase, rxCppi->accListSize);
    
    hDDC->rxTeardownPending = False;    /* Clear the RX teardown pending flag */

    macAddr = (Char *) chOpenArgs;
    for (cnt = 0; cnt < 6; cnt++)
        rxCppi->macAddr[cnt] = macAddr[cnt];


    retCode = PAL_osMemAlloc(0, sizeof(BdMemChunk), 0, (Ptr *) & MemChunk);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacInitRxChannel:%d: Failed to allocate mem chunk for RX CPPI Ch %d",
             hDDC->ddcObj.instId, chInfo->chNum);
        goto errorOut;
    }

    PAL_osMemSet(MemChunk, 0, sizeof(BdMemChunk));

    /* !@0 TODO: We may need to check numBD here since CPPI4.1 requires the
     * desc count to be * multiple of 2 power 5 (ie 32). Similarly, the desc
     * size should also be multiple of 32.
     */

    MemChunk->bdMemSize = (sizeof(CpmacHostDesc) * chInfo->numBD);

    MemChunk->bdMemPtr = PAL_cppi4AllocDesc (hDDC->cppi4PAL, 
                            rxCppi->rxQueue.qMgr, chInfo->numBD, 
                            rxCppi->chInfo.descAlignment);
    if (MemChunk->bdMemPtr == NULL) { 
        CPMAC_DDC_LOGERR ("\nERROR:cpmacInitRxChannel: Failed to allocate Rx descriptor");
        retCode = CPMAC_ERR_CPPI_DESC_REGN_FAIL;
        goto errorFreeMemChunk;
    }

    currBD = (CpmacHostDesc *) MemChunk->bdMemPtr;
    MemChunk->numBd = chInfo->numBD;

    retCode = PAL_osMemAlloc(0, (chInfo->serviceMax * sizeof(DDC_NetPktObj)), 
                                0, (Ptr *) &rxCppi->pktQueue);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacInitRxChannel:%d: Failed to allocate memory for "
             "RX packet queue %d", hDDC->ddcObj.instId, chInfo->chNum);
        goto errorFreeMemChunk;
    }
    PAL_osMemSet(rxCppi->pktQueue, 0, 
                    (chInfo->serviceMax * sizeof(DDC_NetPktObj)));

    /* Allocate memory for buffer queue on a 4 byte boundry and set to 0 */
    retCode = PAL_osMemAlloc(0, (chInfo->serviceMax * sizeof(DDC_NetBufObj) * hDDC->initCfg.rxMaxFrags), 0, (Ptr *) & rxCppi->bufQueue);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacInitRxChannel:%d: Failed to allocate memory for RX buffer queue %d",
             hDDC->ddcObj.instId, chInfo->chNum);
        goto allocBufQueueFail;
    }
    PAL_osMemSet(rxCppi->bufQueue, 0, (chInfo->serviceMax * sizeof(DDC_NetBufObj)
                  * hDDC->initCfg.rxMaxFrags));

    /* Build the packet-buffer structures */
    {
        Int cnt;
        DDC_NetPktObj *currPkt = &rxCppi->pktQueue[0];
        DDC_NetBufObj *currBuf = &rxCppi->bufQueue[0];

        /* Bind pkt and buffer queue data structures */
        for (cnt = 0; cnt < chInfo->serviceMax; cnt++) {
            currPkt->bufList = currBuf;
            ++currPkt;
            currBuf += hDDC->initCfg.rxMaxFrags;
        }
    }

    /* "Slice" BD's one-by-one from the chunk, allocate a buffer and token, */
    for (cnt = 0; cnt < chInfo->numBD; cnt++) {
        char *newBuffer;
        DDC_NetDataToken newBufToken;
        newBuffer = (Ptr) (hDDC->ddaIf->ddaNetIf.ddaNetAllocRxBufCb(hDDC->ddcObj.hDDA, chInfo->bufSize,
                                      (DDC_NetDataToken *) & newBufToken,
                                      (Ptr) currBD));
        if (newBuffer == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:DDC: cpmacInitRxChannel: Error in RX Buffer allocation for channel %d",
                 chInfo->chNum);
            return (CPMAC_ERR_RX_BUFFER_ALLOC_FAIL);
        }

        /* Update the hardware descriptor */
        currBD->hwDesc.descInfo    = PAL_CPPI4_HOSTDESC_DESC_TYPE_HOST << PAL_CPPI4_HOSTDESC_DESC_TYPE_SHIFT;
        currBD->hwDesc.tagInfo     = 0;
        currBD->hwDesc.pktInfo     = (PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH << PAL_CPPI4_HOSTDESC_PKT_TYPE_SHIFT) 
                                | (PAL_CPPI4_HOSTDESC_PKT_RETPLCY_LINKED << PAL_CPPI4_HOSTDESC_PKT_RETPLCY_SHIFT)
                                | (PAL_CPPI4_HOSTDESC_DESC_LOC_OFFCHIP << PAL_CPPI4_HOSTDESC_DESC_LOC_SHIFT)
                                | (rxCppi->fdbQueue[0].qMgr << PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT)
                                | (rxCppi->fdbQueue[0].qNum << PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT);
        currBD->hwDesc.buffLen     = 0;
        currBD->hwDesc.bufPtr      = 0;
        currBD->hwDesc.nextBDPtr   = 0;
        currBD->hwDesc.orgBuffLen  = chInfo->bufSize;
        currBD->hwDesc.orgBufPtr   = PAL_CPPI4_VIRT_2_PHYS(newBuffer);
        currBD->dataPtr     = (Ptr) newBuffer;
        currBD->bufToken    = newBufToken;
        PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
        currBD = (CpmacHostDesc*) ((Uint32)currBD + rxCppi->chInfo.descAlignment);
    }

        /**** Start of Critical Section ****/
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);

    /* Update the memory list while protected */
    if (rxCppi->BdMemList == NULL) {
        rxCppi->BdMemList = MemChunk;
    } else {
        MemChunk->ptrNext = (struct BdMemChunk *) rxCppi->BdMemList;
        rxCppi->BdMemList = MemChunk;

    }

  /**** End of Critical Section ****/
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);


    hDDC->rxIsCreated = True;

    return (CPMAC_SUCCESS);

  allocBufQueueFail:
    /* Free pkt queue */
    PAL_osMemFree(0, rxCppi->pktQueue,
                  (chInfo->serviceMax * sizeof(DDC_NetPktObj)));
  errorFreeMemChunk:
    /* Free BD MemChunck */
    PAL_osMemFree(0, (Ptr *) MemChunk, sizeof(BdMemChunk));
  errorOut:
    /* Free rx cppi channel memory */
    PAL_osMemFree(0, rxCppi, sizeof(CpmacRxCppiCh));
  allocRxCppiFail:

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacInitRxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, chInfo->chNum);

    return (retCode);
}
/**
 *  cpmacDeInitEmbRxChannel
 *    - Frees memory previously allocated in InitEmbRxChannel
 *
 *  \note 1. "chCloseArgs" not used in this implementation
 *        2. This function assumes that the channel number passed is valid and
 *           this function will not do any error check to avoid duplicate error
 *           checks (done in caller function).
 */
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacDeInitEmbRxChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                        Ptr chCloseArgs)
{
    PAL_Result retCode;
    CpmacRxCppiCh *epRxCppi; 

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacDeInitEmbRxChannel:%d: Ch",
                     hDDC->ddcObj.instId);
    
    epRxCppi = hDDC->epRxCppi;

    /* Free the RX Channel structure */
    retCode = PAL_osMemFree(0, epRxCppi, sizeof(CpmacRxCppiCh));
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitEmbRxChannel:%d: Failed to free RX CPPI channel structure for Emb Ch ",
             hDDC->ddcObj.instId);
    }

    hDDC->epRxCppi = NULL;
    		
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacDeInitEmbRxChannel:%d: Ch",
                     hDDC->ddcObj.instId);
					 

    return (CPMAC_SUCCESS);
}
#endif
/**
 *  cpmacDeInitRxChannel
 *    - Frees memory previously allocated in InitRxChannel
 *
 *  \note 1. "chCloseArgs" not used in this implementation
 *        2. This function assumes that the channel number passed is valid and
 *           this function will not do any error check to avoid duplicate error
 *           checks (done in caller function).
 */
PRIVATE PAL_Result cpmacDeInitRxChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                        Ptr chCloseArgs)
{
    PAL_Result retCode;
    CpmacRxCppiCh *rxCppi;
    BdMemChunk *currMemChunk;
    Uint32 lockKey;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacDeInitRxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, channel);
    if (hDDC->rxIsCreated == False) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitRxChannel:%d: RX CPPI Ch %d structure already freed",
             hDDC->ddcObj.instId, channel);
        return (CPMAC_ERR_RX_CH_ALREADY_CLOSED);
    }
    rxCppi = hDDC->rxCppi;

/**** Start of Critical Section ****/
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);

    currMemChunk = rxCppi->BdMemList;

    if (currMemChunk != NULL) {
        while (currMemChunk) {
            CpmacHostDesc *currBD;
            BdMemChunk *lastMemChunk;
            Uint32 numBd;

            currBD = (CpmacHostDesc *) currMemChunk->bdMemPtr;
            numBd = currMemChunk->numBd;

            /* Free the Receive buffers previously allocated */
            while (numBd > 0) {
                /*retCode = PAL_cppi4NetFreeRxBuf(hPAL, (PAL_NetDataToken)
                   &currBD->bufToken); */
                if (hDDC->ddaIf->ddaNetIf.
                    ddaNetFreeRxBufCb(hDDC->ddcObj.hDDA, currBD->dataPtr,
                                      (DDC_NetDataToken) currBD->bufToken,
                                      NULL) != CPMAC_SUCCESS) {
                    CPMAC_DDC_LOGERR
                        ("\nERROR:DDC: cpmacDeInitRxChannel:%d: Failed to free RX buffer Ch %d",
                         hDDC->ddcObj.instId, channel);
                }

                numBd--;
                currBD = (CpmacHostDesc*) ((Uint32)currBD + rxCppi->chInfo.descAlignment);
            }

            /* save this chunk address for freeing */
            lastMemChunk = currMemChunk;

            retCode = PAL_cppi4DeallocDesc(hDDC->cppi4PAL, rxCppi->rxQueue.qMgr, currMemChunk->bdMemPtr);
            if (retCode != PAL_SOK) {
                CPMAC_DDC_LOGERR
                    ("\nERROR:cpmacDeInitRxChannel:%d: Failed to free rx desc region for Ch %d",
                     hDDC->ddcObj.instId, channel);
            }

            /* Move to next mem chunk */
            currMemChunk = (BdMemChunk *) currMemChunk->ptrNext;

            /* Free the BD mem chunk structure */
            PAL_osMemFree(0, lastMemChunk, sizeof(BdMemChunk));
        }
    }
 /**** End of Critical Section ****/
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

#ifdef CPGMAC_USE_ACC_LIST
    retCode = PAL_osMemFree(0, rxCppi->chInfo.rxAccChInfo.list.listBase, rxCppi->accListSize);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitRxChannel:%d: Failed to free RX accumulator list for Ch %d",
             hDDC->ddcObj.instId, channel);
    }
#endif

    /* Free the RX buffer queue */
    retCode = PAL_osMemFree(0, rxCppi->bufQueue, 
                    (rxCppi->chInfo.serviceMax * sizeof(DDC_NetBufObj) * hDDC->initCfg.rxMaxFrags));

    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitRxChannel:%d: Failed to free RX buffer queue for Ch %d",
             hDDC->ddcObj.instId, channel);
    }

    /* Free the RX packet queue */
    retCode = PAL_osMemFree(0, rxCppi->pktQueue, (rxCppi->chInfo.serviceMax * sizeof(DDC_NetPktObj)));
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitRxChannel:%d: Failed to free RX packet queue for Ch %d",
             hDDC->ddcObj.instId, channel);
    }


    /* Free the RX Channel structure */
    retCode = PAL_osMemFree(0, rxCppi, sizeof(CpmacRxCppiCh));
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacDeInitRxChannel:%d: Failed to free RX CPPI channel structure for Ch %d",
             hDDC->ddcObj.instId, channel);
    }

    hDDC->rxCppi = NULL;
    hDDC->rxIsCreated = False;

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacDeInitRxChannel:%d: Ch=%d",
                     hDDC->ddcObj.instId, channel);

    return (CPMAC_SUCCESS);
}

PRIVATE PAL_Result cpmacAddRxBd(CpmacDDCObj * hDDC, CpmacChInfo * chInfo,
                                unsigned int numOfBd2Add)
{
    PAL_Result retCode;
    Uint32 cnt, lockKey;
    CpmacHostDesc *currBD;
    BdMemChunk *MemChunk;
    CpmacRxCppiCh *rxCppi;

    rxCppi = hDDC->rxCppi;
    retCode = PAL_osMemAlloc(0, sizeof(BdMemChunk), 0, (Ptr *) & MemChunk);
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacAddRxBd: Failed to allocate mem chunk");
        goto errorOut;
    }

    /* Set memory to 0 */
    PAL_osMemSet(MemChunk, 0, sizeof(BdMemChunk));

    MemChunk->bdMemSize = (sizeof(CpmacHostDesc) * numOfBd2Add);
    /* Save the BD count in the mem chunk, will be used for freeing later */
    MemChunk->numBd = numOfBd2Add;

    /* Allocate BD Pool */
    MemChunk->bdMemPtr = PAL_cppi4AllocDesc (hDDC->cppi4PAL, 
                            rxCppi->rxQueue.qMgr, MemChunk->numBd, 
                            rxCppi->chInfo.descAlignment);
    if (MemChunk->bdMemPtr == NULL) { 
        CPMAC_DDC_LOGERR ("\nERROR:cpmacAddRxBd: Failed to allocate Rx descriptor");
        retCode = CPMAC_ERR_CPPI_DESC_REGN_FAIL;
        goto errorFreeMemChunk;
    }

    currBD = (CpmacHostDesc *) MemChunk->bdMemPtr;

    /* "Slice" BD's one-by-one from the chunk, allocate a buffer and token, and
       provide to hardware */
    for (cnt = 0; cnt < numOfBd2Add; cnt++) {
        Char *newBuffer;
        DDC_NetDataToken newBufToken;

        /* Allocate a buffer from the buffer pool indicated by 'queue' */
        newBuffer =
            (Ptr) (hDDC->ddaIf->ddaNetIf.
                   ddaNetAllocRxBufCb(hDDC->ddcObj.hDDA, chInfo->bufSize,
                                      (DDC_NetDataToken *) & newBufToken,
                                      (Ptr) currBD));
        if (newBuffer == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacAddRxBd: cnt = %d Error in RX Buffer allocation for queue ",
                 cnt);
            /* If this happens, we can't free the buffer descriptor block or mem
               chunk if we have already committed one or more BDs to the HW */
                goto errorOut;
        }

        /* Update the hardware descriptor */
        currBD->hwDesc.descInfo    = PAL_CPPI4_HOSTDESC_DESC_TYPE_HOST << PAL_CPPI4_HOSTDESC_DESC_TYPE_SHIFT;
        currBD->hwDesc.tagInfo     = 0;
        currBD->hwDesc.pktInfo     = (PAL_CPPI4_HOSTDESC_PKT_TYPE_ETH << PAL_CPPI4_HOSTDESC_PKT_TYPE_SHIFT) 
                                | (PAL_CPPI4_HOSTDESC_PKT_RETPLCY_LINKED << PAL_CPPI4_HOSTDESC_PKT_RETPLCY_SHIFT)
                                | (PAL_CPPI4_HOSTDESC_DESC_LOC_OFFCHIP << PAL_CPPI4_HOSTDESC_DESC_LOC_SHIFT)
                                | (rxCppi->fdbQueue[0].qMgr << PAL_CPPI4_HOSTDESC_PKT_RETQMGR_SHIFT)
                                | (rxCppi->fdbQueue[0].qNum << PAL_CPPI4_HOSTDESC_PKT_RETQNUM_SHIFT);
        currBD->hwDesc.buffLen     = 0;
        currBD->hwDesc.bufPtr      = 0;
        currBD->hwDesc.nextBDPtr   = 0;

        /* Put descriptor on hardware Rx free queue */
        cpmacAddBDToRxQueue(hDDC, rxCppi, currBD, (char *) newBuffer,
                            newBufToken);

        currBD = (CpmacHostDesc*) ((Uint32)currBD + rxCppi->chInfo.descAlignment);
    }

  /**** Start of Critical Section ****/
    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &lockKey);

    /* Update the memory list while protected */
    if (rxCppi->BdMemList == NULL) {
        rxCppi->BdMemList = MemChunk;
    } else {
        MemChunk->ptrNext = (struct BdMemChunk *) rxCppi->BdMemList;
        rxCppi->BdMemList = MemChunk;
    }
    chInfo->numBD += numOfBd2Add;
  /**** End of Critical Section ****/
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, lockKey);

    /* Normal function return point */
    return (PAL_SOK);

  /**** Error cleanup code follows ****/
  errorFreeMemChunk:
    retCode = PAL_osMemFree(0, (Ptr *) MemChunk, sizeof(BdMemChunk));
    if (retCode != PAL_SOK) {
        CPMAC_DDC_LOGERR
            ("\nERROR:cpmacAddRxBd: Failed to Free MemChunk\n");
    }
  errorOut:


    /* Function return point in case of error */
    return (CPMAC_ERR_RX_BUFFER_ALLOC_FAIL);
}

/**
 *  cpmacEnableEth2HostPrxoyChannel
 *    - Channel is enabled in hardware.  Data transfer ready.
 *
 *  \note 1. It is assumed that the channel is already "initialized"
 *        2. To enable a channel after its disabled, it needs to be
 *           initialized again
 */
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacEnableEth2HostProxyChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                      Uint32 direction)
{
    Uint32 retVal;


    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacEnableEth2HostProxyChannel:%d: Ch=%d, Direction=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    if (direction == DDC_NET_CH_DIR_TX) {
        CpmacTxCppiCh *infraTxCppi;
        Ptr cppi4TxChHnd;
        infraTxCppi = hDDC->infraTxCppi;
        if (infraTxCppi == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableEth2HostProxyChannel:%d: Invalid Ch %d. TX CPPI structure NULL",
                 hDDC->ddcObj.instId, channel);
            return (CPMAC_ERR_TX_CH_INVALID);
        }
        infraTxCppi->palCppi4Hnd = hDDC->cppi4PAL;
        
        
        /* Open the channel in the CPPI4 PAL */
        cppi4TxChHnd = PAL_cppi4TxChOpen(hDDC->cppi4PAL, &infraTxCppi->chInfo.cppi4TxChInfo, NULL);
        if (cppi4TxChHnd == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableEth2HostProxyChannel: Error %08X from CPPI4 PAL TxChOpen()");
            return (CPMAC_ERR_CPPI_TX_CH);
        }
        infraTxCppi->cppi4TxChHnd = cppi4TxChHnd;
        
        retVal = PAL_cppi4EnableTxChannel(infraTxCppi->cppi4TxChHnd, NULL);
		if (retVal != PAL_SOK) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from CPPI4 PAL EnableTxChannel()",
                 retVal);
            return (retVal);
        }
        infraTxCppi->chInfo.chState = DDC_NET_CH_OPENED;

    }
    else if (direction == DDC_NET_CH_DIR_RX) {
        
    }

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacEnableEth2HostProxyChannel:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    return (CPMAC_SUCCESS);
}
#endif
/**
 *  cpmacEnableEmbChannel
 *    - Channel is enabled in hardware.  Data transfer ready.
 *
 *  \note 1. It is assumed that the channel is already "initialized"
 *        2. To enable a channel after its disabled, it needs to be
 *           initialized again
 */
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacEnableEmbChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                      Uint32 direction)
{
    Uint32 retVal;


    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacEnableEmbChannel:%d: Ch=%d, Direction=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    if (direction == DDC_NET_CH_DIR_TX) {
	
    }
    else if (direction == DDC_NET_CH_DIR_RX) {
        CpmacRxCppiCh *epRxCppi;
        
        epRxCppi = hDDC->epRxCppi;
	if (epRxCppi == NULL) {
             CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel:%d: Invalid Channel %d. RX CPPI structure NULL",
                 hDDC->ddcObj.instId, channel);
            return (CPMAC_ERR_RX_CH_INVALID);
        }
        epRxCppi->palCppi4Hnd = hDDC->cppi4PAL;

        epRxCppi->cppi4RxChHnd = PAL_cppi4RxChOpen(hDDC->cppi4PAL, &epRxCppi->chInfo.cppi4RxChInfo, NULL);
        if (epRxCppi->cppi4RxChHnd == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableEmbChannel: Error %08X from CPPI4 PAL RxChOpen()");
            return (CPMAC_ERR_CPPI_RX_CH);
        }
		
        retVal = PAL_cppi4EnableRxChannel(epRxCppi->cppi4RxChHnd, NULL);

        if (retVal != PAL_SOK) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableEmbChannel: Error %08X from CPPI4 PAL EnableRxChannel()",
                 retVal);
            return (retVal);
        }

        /* Mark channel open */
        epRxCppi->chInfo.chState = DDC_NET_CH_OPENED;
    }
	CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n+ cpmacEnableEmbChannel:%d: Ch=%d, Direction=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
    return (CPMAC_SUCCESS);
}
#endif
/**
 *  cpmacEnableChannel
 *    - Channel is enabled in hardware.  Data transfer ready.
 *
 *  \note 1. It is assumed that the channel is already "initialized"
 *        2. To enable a channel after its disabled, it needs to be
 *           initialized again
 */
PRIVATE PAL_Result cpmacEnableChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                      Uint32 direction)
{
    Uint32 retVal;


    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacEnableChannel:%d: Ch=%d, Direction=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    if (direction == DDC_NET_CH_DIR_TX) {
        CpmacTxCppiCh *txCppi;
        Uint32 indx;
        Ptr cppi4TxChHnd;
        PAL_Cppi4AccChHnd txAccChHnd;
        txCppi = hDDC->txCppi;
        if (txCppi == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel:%d: Invalid Ch %d. TX CPPI structure NULL",
                 hDDC->ddcObj.instId, channel);
            return (CPMAC_ERR_TX_CH_INVALID);
        }
        txCppi->palCppi4Hnd = hDDC->cppi4PAL;
        
        for (indx = 0; indx < txCppi->numTxQs; indx++) {
            txCppi->txQueueHnd[indx] = PAL_cppi4QueueOpen (hDDC->cppi4PAL, 
                                                txCppi->txQueue[indx]);
        }
        txCppi->txCmplQueueHnd = PAL_cppi4QueueOpen (hDDC->cppi4PAL, 
                                                txCppi->txCmplQueue);

        /* Open the channel in the CPPI4 PAL */
        cppi4TxChHnd = PAL_cppi4TxChOpen(hDDC->cppi4PAL, &txCppi->chInfo.cppi4TxChInfo, NULL);
        if (cppi4TxChHnd == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from CPPI4 PAL TxChOpen()");
            return (CPMAC_ERR_CPPI_TX_CH);
        }
        txCppi->cppi4TxChHnd = cppi4TxChHnd;

        txAccChHnd = PAL_cppi4AccChOpen(hDDC->cppi4PAL, &txCppi->chInfo.txAccChInfo);
        if (txAccChHnd == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from CPPI4 PAL PAL_cppi4AccChOpen()");
            return (CPMAC_ERR_CPPI_TX_CH);
        }
        txCppi->txAccChHnd = txAccChHnd;
        

        retVal = PAL_cppi4EnableTxChannel(txCppi->cppi4TxChHnd, NULL);
        if (retVal != PAL_SOK) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from CPPI4 PAL EnableTxChannel()",
                 retVal);
            return (retVal);
        }

        retVal = enableTxIntr(txCppi);
        if (retVal) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from enableTxIntr()",
                 retVal);
            return (retVal);
        }

        /* Mark channel open */
        hDDC->txIsOpen = True;
        txCppi->chInfo.chState = DDC_NET_CH_OPENED;
    } else if (direction == DDC_NET_CH_DIR_RX) {
        CpmacRxCppiCh *rxCppi;
        BdMemChunk *MemChunk;
        Uint32 cnt;
        CpmacHostDesc *currBD;
        Ptr cppi4RxChHnd;
        PAL_Cppi4AccChHnd rxAccChHnd;

        rxCppi = hDDC->rxCppi;
        if (rxCppi == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel:%d: Invalid Channel %d. RX CPPI structure NULL",
                 hDDC->ddcObj.instId, channel);
            return (CPMAC_ERR_RX_CH_INVALID);
        }
        rxCppi->palCppi4Hnd = hDDC->cppi4PAL;

        cpmacSetMacAddress(hDDC, channel, rxCppi->macAddr);
        cpmacSetSrcMacAddress(hDDC, rxCppi->macAddr);
        MemChunk = rxCppi->BdMemList;

        rxCppi->rxQueueHnd = PAL_cppi4QueueOpen (hDDC->cppi4PAL, 
                                                rxCppi->rxQueue);
        rxCppi->fdbQueueHnd[0] = PAL_cppi4QueueOpen (hDDC->cppi4PAL, 
                                                rxCppi->fdbQueue[0]);

        /* Open the channel in the CPPI4 PAL */
        cppi4RxChHnd = PAL_cppi4RxChOpen(hDDC->cppi4PAL, &rxCppi->chInfo.cppi4RxChInfo, NULL);
        if (cppi4RxChHnd == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from CPPI4 PAL RxChOpen()");
            return (CPMAC_ERR_CPPI_RX_CH);
        }

        rxCppi->cppi4RxChHnd = cppi4RxChHnd;

        rxAccChHnd = PAL_cppi4AccChOpen(hDDC->cppi4PAL, &rxCppi->chInfo.rxAccChInfo);
        if (rxAccChHnd == NULL) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from CPPI4 PAL PAL_cppi4AccChOpen()");
            return (CPMAC_ERR_CPPI_RX_CH);
        }
        rxCppi->rxAccChHnd = rxAccChHnd;
        

        currBD = (CpmacHostDesc *) MemChunk->bdMemPtr;
        for (cnt = 0; cnt < (MemChunk->numBd); cnt++) {
            PAL_CPPI4_CACHE_WRITEBACK(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
            PAL_cppi4QueuePush(rxCppi->fdbQueueHnd[0], (Ptr) PAL_CPPI4_VIRT_2_PHYS(currBD),
                                CPPI41_QM_HDESC_SIZE_VAL/*rxCppi->chInfo.descAlignment*/, 
                                0/*!@@*/);
            currBD = (CpmacHostDesc*) ((Uint32)currBD + rxCppi->chInfo.descAlignment);
        }

        retVal = PAL_cppi4EnableRxChannel(rxCppi->cppi4RxChHnd, NULL);

        if (retVal != PAL_SOK) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from CPPI4 PAL EnableRxChannel()",
                 retVal);
            return (retVal);
        }

        retVal = enableRxIntr (rxCppi);
        if (retVal) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacEnableChannel: Error %08X from enableRxIntr)",
                 retVal);
            return (retVal);
        }

        /* Mark channel open */
        hDDC->rxIsOpen = True;
        rxCppi->chInfo.chState = DDC_NET_CH_OPENED;
    }

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacEnableChannel:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    return (CPMAC_SUCCESS);
}
/**
 *  cpmacDisableEth2HostProxyChannel
 *    - Channel is disabled in hardware. Data transfer is finished.
 *
 *  \note 1. It is assumed that the channel number passed is valid
 *        2. Resources for the channel will be released only when its closed
 */
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacDisableEth2HostProxyChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                       DDC_NetChDir direction)
{
    Uint32 retVal;
    CpmacHostDesc *currBD;
    CpmacHostDesc *tempBD;
    Uint32 ret = False;
    Uint32 indx;
#ifdef CPGMAC_USE_ACC_LIST
    Uint32 lockKey;
#endif
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacDisableEth2HostProxyChannel:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    /* Set teardown pending flags */
     if (direction == DDC_NET_CH_DIR_RX)
        hDDC->rxTeardownPending = True;

    if (direction == DDC_NET_CH_DIR_TX) {
        CpmacTxCppiCh *infraTxCppi;
        infraTxCppi = hDDC->infraTxCppi;
 
        /* Teardown the Inframode Tx channel
         * Teardown Descriptor for the Inframode Tx channel 
         * is routed to Teardown Free Descriptor queue
         */
        retVal = PAL_cppi4TxChClose(infraTxCppi->cppi4TxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEth2HostProxyChannel: Error %08X from CPPI4 PAL ChClose()",
                 retVal);
            return (retVal);
        }
        /* Since Teardown descriptor is routed to Teardown Free Descriptor queue
         * wait here and poll the status bit to make sure teardown is complete
         */
        retVal = PAL_cppi4TxChStatus(infraTxCppi->cppi4TxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEth2HostProxyChannel: Error %08X from CPPI4 PAL DisableInfraTxChannel()",
                 retVal);
            return (retVal);
        }
        
        
        retVal = PAL_cppi4DisableTxChannel(infraTxCppi->cppi4TxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEth2HostProxyChannel: Error %08X from CPPI4 PAL DisableTxChannel()",
                 retVal);
            return (retVal);
        }

        retVal = PAL_cppi4TxChDestroy(infraTxCppi->cppi4TxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEth2HostProxyChannel: Error %08X from CPPI4 PAL PAL_cppi4DestroyTxChannel()",
                 retVal);
            return (retVal);
        }
        /* Mark channel closed */
        
    } else if (direction == DDC_NET_CH_DIR_RX) {
 
        CpmacRxCppiCh *rxCppi;
        PAL_Cppi4QueueHnd ethProxyQHnd, ethUsbFDQhnd;
        Cppi4Queue ethProxyQ, ethUsbFDQ;
        CpmacEmbdDesc *tempBDEmb, *currBDEmb;
        Cppi4BufPool pool;
        Uint32 i,bufValid;
        
        rxCppi = hDDC->rxCppi;   
        retVal = disableRxIntr (rxCppi);
		retVal = PAL_cppi4AccChClose(rxCppi->rxAccChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEth2HostProxyChannel: Error %08X from CPPI4 PAL PAL_cppi4AccChClose()",
                 retVal);
            return (retVal);
        }
         
	/* Pop out all the descriptors from the free host buffer descriptor queue         
         * so all the descriptor in the proxyqueue will be recycled
         * automatically
         */
        while ((currBD = (CpmacHostDesc *) PAL_cppi4QueuePop (rxCppi->fdbQueueHnd[0] /*!@0 */)));
#ifdef CPGMAC_USE_ACC_LIST
            /* Pop the received descriptors from the Accumulator channel */
        indx = avalanche_intd_get_interrupt_count (CPGMAC_INTD_HOST_NUM, rxCppi->accChNum);
        if (indx)
        {
            for (; (indx <= CPGMAC_ACC_LIST_DIV) && indx; --indx) {
                currBD = NULL;
                PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lockKey);
                while ((tempBD = (CpmacHostDesc *) ((Uint32)(*rxCppi->listEntryPtr) & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK))) {
                    rxCppi->listEntryPtr = (Uint32 *)((Uint32)rxCppi->listEntryPtr + CPGMAC_ACC_ENTRY_SIZE); 
                    currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);

                    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"%s %d currBD %x\n"
					    , __FUNCTION__, __LINE__, currBD); 
                    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"Popped BD from acc list is not TD\n");
                        /* !@0 TODO - Return the buffer to net */
                  }
                  rxCppi->listEntryPtr = PAL_cppi4AccChGetNextList(rxCppi->rxAccChHnd);

                  if (currBD) avalanche_intd_set_interrupt_count (CPGMAC_INTD_HOST_NUM, rxCppi->accChNum, 1);
                  PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lockKey);
            } /* for All pages */
            doRxEOI (rxCppi); 
        }
        
        /* Recycle the descriptors from Rx queue */      
        while (1 /* !@0 */) { 
            tempBD = (CpmacHostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd);
            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);
            if (tempBD == NULL)
                break;

        }
        /* TODO Magic value Need to set according to Requirement */
        /* Wait for some time if there are more descriptors that are still not forwarded by SR
         */
        PAL_osWaitMsecs(DDC_CPMAC_SR_DELAY2);
        /* Recycle Those descriptors as well */
        while (1 /* !@0 */) { 
            tempBD = (CpmacHostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd);
            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);
            if (tempBD == NULL)
                break;

        }
		
#else
        while ((tempBD = (CpmacHostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd))) {
            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);
			if (((currBD->hwDesc.descInfo & PAL_CPPI4_TDDESC_DESC_TYPE_MASK) 
                        >> PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT) == PAL_CPPI4_TDDESC_DESC_TYPE_TD) {
                ret = True;
                break;		
			}
        }
            /* check for teardown condition here.  This allows packets to
               be processed normally up until the teardown descriptor shows up */
        PAL_osWaitMsecs(DDC_CPMAC_SR_DELAY2);
		while ((tempBD = (CpmacHostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd))) {
            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);
            if (((currBD->hwDesc.descInfo & PAL_CPPI4_TDDESC_DESC_TYPE_MASK) 
                        >> PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT) == PAL_CPPI4_TDDESC_DESC_TYPE_TD) {
                ret = True;
                break;		
			}
           
        }
#endif /* !CPGMAC_USE_ACC_LIST */
        /* Teardown the Rx Host channel */
        retVal = PAL_cppi4RxChClose(rxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEth2HostProxyChannel: Error %08X from CPPI4 PAL ChClose()",
                 retVal);
            return (retVal);
        }

        /* Look for the Teardown descriptor 
         * if not found and queue is empty
         * Come out with error
         */
        while ((tempBD = (CpmacHostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd))) {
            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);

            /* check for teardown condition here.  This allows packets to
               be processed normally up until the teardown descriptor shows up */
            if(tempBD == NULL)
                continue;

            PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
            if (((currBD->hwDesc.descInfo & PAL_CPPI4_TDDESC_DESC_TYPE_MASK) 
                       >> PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT) == PAL_CPPI4_TDDESC_DESC_TYPE_TD) 
            {
                PAL_cppi4GetTdInfo (rxCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
#ifdef TD_DEBUG 
                printk("Rx Channel TearDown Descriptor Found %x\n",(unsigned int)currBD);
#endif
                ret = True;
                break;
            }
        }
		
        if (ret != True) {
#ifdef TD_DEBUG
            /*CPMAC_DDC_LOGERR*/ printk("\nERROR:Teardown buffer descriptor not found.");
#endif
            CPMAC_DDC_LOGERR("\nERROR:Teardown buffer descriptor not found.");
        }
        hDDC->rxTeardownPending = False;

        retVal = PAL_cppi4DisableRxChannel(rxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEth2HostProxyChannel: Error %08X from CPPI4 PAL DisableRxChannel()",
                 retVal);
            return (retVal);
        }


        retVal = PAL_cppi4RxChDestroy(rxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEth2HostProxyChannel: Error %08X from CPPI4 PAL PAL_cppi4RxChDestroy()",
                 retVal);
            return (retVal);
        }        
        /* Drain out the Proxy queue if it is having any embedded descriptors forwarded by SR
         * Read the proxy queue if found any embedded descriptors
         * push back the buffers to respective pools and 
         * descriptor to the Free Embedded descriptor queue shared  by Eth and USB
         */ 
        ethProxyQ.qNum  = CPMAC_CPPI4x_ETH_TO_HOST_PRXY_QNUM(0);
        ethProxyQ.qMgr  = CPMAC_CPPI4x_ETH_TO_HOST_PRXY_QMGR;
        ethUsbFDQ.qNum = CPMAC_CPPI4x_FD_QNUM(0);
        ethUsbFDQ.qMgr = CPMAC_CPPI4x_FD_QMGR;
        ethProxyQHnd = PAL_cppi4QueueOpen(hDDC->cppi4PAL,ethProxyQ);
        ethUsbFDQhnd =   PAL_cppi4QueueOpen(hDDC->cppi4PAL,ethUsbFDQ);
         
        /* Find if any buffer descriptors are left in the Proxy queue after channel is disabled. */
        while((tempBDEmb = (CpmacEmbdDesc *)PAL_cppi4QueuePop(ethProxyQHnd)))
        {
            currBDEmb = PAL_CPPI4_PHYS_2_VIRT(tempBDEmb);
            PAL_CPPI4_CACHE_INVALIDATE(currBDEmb, CPPI4_BD_LENGTH_FOR_CACHE);
#ifdef TD_DEBUG
            printk("\n\n\nEmbedded Descriptor found\n\n\n");
#endif
            /* TODO Is number of buffers ok */
            /* find the Buffers attached to the Embedded descripors  and push it back to respective pools */
            for(i=0;i<4;i++)
            {
                bufValid = (currBDEmb->hwDesc.Buf[i].BufInfo & CPPI41_EM_BUF_VALID_MASK);
                if(bufValid)
                {
                    pool.bMgr = (currBDEmb->hwDesc.Buf[i].BufInfo & CPPI41_EM_BUF_MGR_MASK) >> CPPI41_EM_BUF_MGR_SHIFT;
                    pool.bPool =(currBDEmb->hwDesc.Buf[i].BufInfo & CPPI41_EM_BUF_POOL_MASK) >> CPPI41_EM_BUF_POOL_SHIFT;                         
                    PAL_cppi4BufDecRefCnt(hDDC->cppi4PAL,pool,(Ptr)currBDEmb->hwDesc.Buf[i].BufPtr);
                }
            }
            /* Push back the embedded descriptor the the Free descriptor queue shared between USB and ETH */
            tempBDEmb = (CpmacEmbdDesc *)PAL_CPPI4_VIRT_2_PHYS((Uint32)currBDEmb);
            PAL_cppi4QueuePush(ethUsbFDQhnd,(Ptr)tempBDEmb,(CPMAC_CPPI4x_RX_EMB_BD_SIZE - 24)/4,0);

        } 
        PAL_cppi4QueueClose(hDDC->cppi4PAL,ethProxyQHnd);
        PAL_cppi4QueueClose(hDDC->cppi4PAL,ethUsbFDQhnd);
		
        /* Mark channel closed */
        hDDC->rxIsOpen = False;
        
        PAL_cppi4QueueClose (hDDC->cppi4PAL, rxCppi->rxQueueHnd);
        PAL_cppi4QueueClose (hDDC->cppi4PAL, rxCppi->fdbQueueHnd[0]);
    }

    /* !@@ TODO: Free list memeory for both tx and rx */

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacDisableEth2HostProxyChannel:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    return (CPMAC_SUCCESS);
}
#endif
/**
 *  cpmacDisableEmbChannel
 *    - Channel is disabled in hardware. Data transfer is finished.
 *
 *  \note 1. It is assumed that the channel number passed is valid
 *        2. Resources for the channel will be released only when its closed
 */
#ifdef CONFIG_ARM_AVALANCHE_PPD
PRIVATE PAL_Result cpmacDisableEmbChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                       DDC_NetChDir direction)
{
    Uint32 retVal;
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacDisableEmbChannel:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    
    if (direction == DDC_NET_CH_DIR_TX) {
       
    } 
	else if (direction == DDC_NET_CH_DIR_RX) {
       
        CpmacRxCppiCh *epRxCppi;
        epRxCppi = hDDC->epRxCppi;
        /* Teardown the RxEmbedded Channel
         * Teardown descriptor is routed back to Free Teardown Descriptor queue
         * by SR firmware
         */
        retVal = PAL_cppi4RxChClose(epRxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEmbChannel: Error %08X from CPPI4 PAL ChClose()",
                 retVal);
            return (retVal);
        }
        /* Since we are not getting Teardown descriptor wait for teardown to
         * complete by observing the Teardown status bit
         */
        retVal = PAL_cppi4RxChStatus(epRxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEmbChannel: Error %08X from CPPI4 PAL ChClose()",
                 retVal);
            return (retVal);
        }
        retVal = PAL_cppi4DisableRxChannel(epRxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEmbChannel: Error %08X from CPPI4 PAL DisableRxChannel()",
                 retVal);
            return (retVal);
        }
        retVal = PAL_cppi4RxChDestroy(epRxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableEmbChannel: Error %08X from CPPI4 PAL PAL_cppi4RxChDestroy()",
                 retVal);
            return (retVal);
        }          
        epRxCppi->chInfo.chState = DDC_NET_CH_CLOSED;
    }

    /* !@@ TODO: Free list memeory for both tx and rx */

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacDisableEmbChannel:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    return (CPMAC_SUCCESS);
}
#endif
/**
 *  cpmacDisableChannel
 *    - Channel is disabled in hardware. Data transfer is finished.
 *
 *  \note 1. It is assumed that the channel number passed is valid
 *        2. Resources for the channel will be released only when its closed
 */
PRIVATE PAL_Result cpmacDisableChannel(CpmacDDCObj * hDDC, Uint32 channel,
                                       DDC_NetChDir direction)
{
    Uint32 retVal;
    CpmacHostDesc *currBD, *tempBD1;
    CpmacHostDesc *tempBD;
    Uint32 ret = False;
    Uint32 indx;
#ifdef CPGMAC_USE_ACC_LIST
    Uint32 lockKey;
#endif
    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_ENTRY,
                     "\n+ cpmacDisableChannel:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));
    /* Set teardown pending flags */
    if (direction == DDC_NET_CH_DIR_TX)
        hDDC->txTeardownPending = True;
    else if (direction == DDC_NET_CH_DIR_RX)
        hDDC->rxTeardownPending = True;

    if (direction == DDC_NET_CH_DIR_TX) {
        CpmacTxCppiCh *txCppi;
        txCppi = hDDC->txCppi;
        
        retVal = disableTxIntr (txCppi);
        if (retVal) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from disableTxIntr()",
                 retVal);
            return (retVal);
        }
        retVal = PAL_cppi4AccChClose(txCppi->txAccChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from CPPI4 PAL PAL_cppi4AccChClose()",
                 retVal);
            return (retVal);
        }
        retVal = PAL_cppi4TxChClose(txCppi->cppi4TxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from CPPI4 PAL ChClose()",
                 retVal);
            return (retVal);
        }
        /* There might be packets on the Tx queue that weren't sent.  They need to
           be recycled properly. */
        while ((currBD = (CpmacHostDesc *) PAL_cppi4QueuePop (txCppi->txQueueHnd[0] /*!@0 */))) {
            currBD = PAL_CPPI4_PHYS_2_VIRT(currBD);
            hDDC->ddaIf->ddaNetIf.ddaNettxCompleteCb(hDDC->ddcObj.hDDA,
                                                     (DDC_NetDataToken) &
                                                     currBD->bufToken, 1,
                                                     0);
			txCppi->numBD += 1;
#ifdef TD_DEBUG
			printk("numBD = %d\n",txCppi->numBD);
#endif
        }
#ifdef CPGMAC_USE_ACC_LIST
            indx = avalanche_intd_get_interrupt_count (CPGMAC_INTD_HOST_NUM, txCppi->accChNum);
            if (indx)
            {
                for (; (indx <= CPGMAC_ACC_LIST_DIV) && indx; --indx) {
                    currBD = NULL;
                    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lockKey);
                    while ((tempBD1 = (CpmacHostDesc *) ((Uint32)(*txCppi->listEntryPtr) & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK))) {
                        txCppi->listEntryPtr = (Uint32 *)((Uint32)txCppi->listEntryPtr + CPGMAC_ACC_ENTRY_SIZE); 
                        currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD1);
                        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
			txCppi->numBD += 1;

                        if (((currBD->hwDesc.descInfo & PAL_CPPI4_TDDESC_DESC_TYPE_MASK) 
                                >> PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT) == PAL_CPPI4_TDDESC_DESC_TYPE_TD) {
                            ret = True;
                            txCppi->numBD -= 1;
                            break;
                        }
                        else
                            CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"Popped BD from acc list is not TD\n");

                        /* !@0 TODO - Return the buffer to net */
                    }

                    txCppi->listEntryPtr = PAL_cppi4AccChGetNextList(txCppi->txAccChHnd);
                    
                    if (currBD) avalanche_intd_set_interrupt_count (CPGMAC_INTD_HOST_NUM, txCppi->accChNum, 1);
                    
                    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lockKey);
                } /* for All pages */
                doTxEOI (txCppi); 
            }
       
        while (1 /* !@0 */) {
            tempBD1 = (CpmacHostDesc *) PAL_cppi4QueuePop(txCppi->txCmplQueueHnd);
            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD1);
						
            if (tempBD1 == NULL)
                continue;
            txCppi->numBD += 1;
#ifdef TD_DEBUG
            printk("numBD = %d\n",txCppi->numBD);
#endif

            /* check for teardown condition here.  This allows packets to
               be processed normally up until the teardown descriptor shows up */

            PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
            if (((currBD->hwDesc.descInfo & PAL_CPPI4_TDDESC_DESC_TYPE_MASK) 
                        >> PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT) == PAL_CPPI4_TDDESC_DESC_TYPE_TD) {
#ifdef TD_DEBUG
                printk ("Tx Channel Teardown Descriptor found! %x\n ",(unsigned int)currBD);
#endif
                PAL_cppi4GetTdInfo (txCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
                ret = True;
                txCppi->numBD -= 1;
                break;
            }
            else
            {
                CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"Popped BD(%x) is not TD\n", currBD);
                CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"raw descInfo = %x\n",
                    currBD->descInfo);
                CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"descInfo = %d\n", 
                    ((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD) >> CPPI4_DESC_TYPE_SHIFT_TD));
            }

        }
#else
        while ((tempBD1 = (CpmacHostDesc *) PAL_cppi4QueuePop(txCppi->txCmplQueueHnd))) {
            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD1);
            /* check for teardown condition here.  This allows packets to
               be processed normally up until the teardown descriptor shows up */

            PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
            if (((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD) 
                        >> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD) {
                ret = True;
                break;
            }
        }
#endif /* !CPGMAC_USE_ACC_LIST */
#ifdef TD_DEBUG
        if(txCppi->numBD < 64)
        {
            printk("Error Tx Descriptors missing\n");
        }
#endif	
        if (ret != True) {
            CPMAC_DDC_LOGERR
                ("\nERROR:Teardown buffer descriptor not found.");
        }
        retVal = PAL_cppi4DisableTxChannel(txCppi->cppi4TxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from CPPI4 PAL DisableTxChannel()",
                 retVal);
            return (retVal);
        }

        retVal = PAL_cppi4TxChDestroy(txCppi->cppi4TxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from CPPI4 PAL PAL_cppi4DestroyTxChannel()",
                 retVal);
            return (retVal);
        }
        
        hDDC->txTeardownPending = False;

        for (indx = 0; indx < txCppi->numTxQs; indx++) {
            PAL_cppi4QueueClose (hDDC->cppi4PAL, txCppi->txQueueHnd[indx]);
        }
        PAL_cppi4QueueClose (hDDC->cppi4PAL, txCppi->txCmplQueueHnd);

        /* Mark channel closed */
        hDDC->txIsOpen = False;
    } else if (direction == DDC_NET_CH_DIR_RX) {
        CpmacRxCppiCh *rxCppi;
        rxCppi = hDDC->rxCppi;
        retVal = disableRxIntr (rxCppi);
        if (retVal) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from disableRxIntr()",
                 retVal);
            return (retVal);
        }
        
        retVal = PAL_cppi4AccChClose(rxCppi->rxAccChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from CPPI4 PAL PAL_cppi4AccChClose()",
                 retVal);
            return (retVal);
        }

        retVal = PAL_cppi4RxChClose(rxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from CPPI4 PAL ChClose()",
                 retVal);
            return (retVal);
        }

        while ((currBD = (CpmacHostDesc *) PAL_cppi4QueuePop (rxCppi->fdbQueueHnd[0] /*!@0 */)));
        
#ifdef CPGMAC_USE_ACC_LIST
            indx = avalanche_intd_get_interrupt_count (CPGMAC_INTD_HOST_NUM, rxCppi->accChNum);
            if (indx)
            {
                for (; (indx <= CPGMAC_ACC_LIST_DIV) && indx; --indx) {
                    currBD = NULL;
                    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lockKey);
                    while ((tempBD = (CpmacHostDesc *) ((Uint32)(*rxCppi->listEntryPtr) & QMGR_QUEUE_N_REG_D_DESC_ADDR_MASK))) {
                        rxCppi->listEntryPtr = (Uint32 *)((Uint32)rxCppi->listEntryPtr + CPGMAC_ACC_ENTRY_SIZE); 
                        currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);
						
                        CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN, "%s %d currBD %x\n", __FUNCTION__, __LINE__, currBD); 

                        PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);

                        if (((currBD->hwDesc.descInfo & PAL_CPPI4_TDDESC_DESC_TYPE_MASK) 
                                >> PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT) == PAL_CPPI4_TDDESC_DESC_TYPE_TD) {
                            ret = True;
                            break;
                        }
                        else
                            CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"Popped BD from acc list is not TD\n");


                        /* !@0 TODO - Return the buffer to net */
                    }
                    rxCppi->listEntryPtr = PAL_cppi4AccChGetNextList(rxCppi->rxAccChHnd);

                    if (currBD) avalanche_intd_set_interrupt_count (CPGMAC_INTD_HOST_NUM, rxCppi->accChNum, 1);
                    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lockKey);
                } /* for All pages */
                doRxEOI (rxCppi); 
            }
              
        while (1 /* !@0 */) { 
            tempBD = (CpmacHostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd);
            
            if (tempBD == NULL)
                continue;

            CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"Rx BD popped!\n");

            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);

            /* check for teardown condition here.  This allows packets to
               be processed normally up until the teardown descriptor shows up */

            PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
            if (((currBD->hwDesc.descInfo & PAL_CPPI4_TDDESC_DESC_TYPE_MASK) 
                        >> PAL_CPPI4_TDDESC_DESC_TYPE_SHIFT) == PAL_CPPI4_TDDESC_DESC_TYPE_TD) {

                CPMAC_DDC_LOGMSG(CPMAC_DEBUG_TEARDOWN,"Rx TD BD(%x) popped!\n", currBD);

                PAL_cppi4GetTdInfo (rxCppi->palCppi4Hnd, (PAL_Cppi4BD*)currBD, NULL, NULL, NULL);
                ret = True;
                break;
            }
        }
#else
        while ((tempBD = (CpmacHostDesc *) PAL_cppi4QueuePop(rxCppi->rxQueueHnd))) {
            currBD = PAL_CPPI4_PHYS_2_VIRT(tempBD);

            /* check for teardown condition here.  This allows packets to
               be processed normally up until the teardown descriptor shows up */

            PAL_CPPI4_CACHE_INVALIDATE(currBD, CPPI4_BD_LENGTH_FOR_CACHE);
            if (((currBD->descInfo & CPPI4_DESC_TYPE_MASK_TD) 
                        >> CPPI4_DESC_TYPE_SHIFT_TD) == CPPI4_DESC_TYPE_TD) {
                ret = True;
                break;
            }
        }
#endif /* !CPGMAC_USE_ACC_LIST */

        if (ret != True) {
            CPMAC_DDC_LOGERR
                ("\nERROR:Teardown buffer descriptor not found.");
        }
        hDDC->rxTeardownPending = False;

        retVal = PAL_cppi4DisableRxChannel(rxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from CPPI4 PAL DisableRxChannel()",
                 retVal);
            return (retVal);
        }


        retVal = PAL_cppi4RxChDestroy(rxCppi->cppi4RxChHnd, NULL);
        if (retVal != CPMAC_SUCCESS) {
            CPMAC_DDC_LOGERR
                ("\nERROR:cpmacDisableChannel: Error %08X from CPPI4 PAL PAL_cppi4RxChDestroy()",
                 retVal);
            return (retVal);
        }        

        /* Mark channel closed */
        hDDC->rxIsOpen = False;
        
        PAL_cppi4QueueClose (hDDC->cppi4PAL, rxCppi->rxQueueHnd);
        PAL_cppi4QueueClose (hDDC->cppi4PAL, rxCppi->fdbQueueHnd[0]);
    }

    /* !@@ TODO: Free list memeory for both tx and rx */

    CPMAC_DDC_LOGMSG(CPMAC_DEBUG_FUNCTION_EXIT,
                     "\n- cpmacDisableChannel:%d: Ch=%d, Dir=%s",
                     hDDC->ddcObj.instId, channel,
                     ((direction == DDC_NET_CH_DIR_TX) ? "TX" : "RX"));

    return (CPMAC_SUCCESS);
}

PRIVATE void cpmacDDCPhycnt(CpmacDDCObj * hDDC, Uint32 * cmdArg)
{
    int result;
    CpmacHwStatistics stats;
    struct mib2_phyCounters *mib2PhyCounters =
        (struct mib2_phyCounters *) cmdArg;
    result = DDC_cpmacControl(hDDC, CPMAC_DDC_IOCTL_GET_STATISTICS, (Uint32 *) & stats, NULL);
    if (result != 0) {
        CPMAC_DDC_LOGERR
            ("\ncpmacStats: Error from ioctl for DDC CPMAC_DDC_IOCTL_GET_STATISTICS \n");
        return;
    }

    mib2PhyCounters->ethAlignmentErrors = stats.ifInAlignCodeErrors;
    mib2PhyCounters->ethFCSErrors = stats.ifInCRCErrors;
    mib2PhyCounters->ethSingleCollisions = stats.ifSingleCollisionFrames;
    mib2PhyCounters->ethMultipleCollisions =
        stats.ifMultipleCollisionFrames;
    mib2PhyCounters->ethSQETestErrors = 0;      /*Hardware doesn't support this */
    mib2PhyCounters->ethDeferredTxFrames = stats.ifDeferredTransmissions;
    mib2PhyCounters->ethLateCollisions = stats.ifLateCollisions;
    mib2PhyCounters->ethExcessiveCollisions =
        stats.ifExcessiveCollisionFrames;
    mib2PhyCounters->ethInternalMacTxErrors = 0;        /*Hardware doesn't support this */
    mib2PhyCounters->ethCarrierSenseErrors = stats.ifCarrierSenseErrors;
    mib2PhyCounters->ethTooLongRxFrames = stats.ifInOversizedFrames;
    mib2PhyCounters->ethInternalMacRxErrors = 0;        /*Hardware doesn't support this */
    mib2PhyCounters->ethSymbolErrors = 0;       /*Hardware doesn't support this */
    return;
}

unsigned int ts_ddc_stat_dbg = 0;

PRIVATE void cpmacDDCIfcntClear(CpmacDDCObj * hDDC)
{
    PAL_osMemSet((char *) &hDDC->Mib2IfHCCounter, 0,
                 sizeof(hDDC->Mib2IfHCCounter));
}


PRIVATE void cpmacDDCIfcntUpdt(CpmacDDCObj * hDDC)
{
    int result;
    CpmacHwStatistics stats;

    result =
        DDC_cpmacControl(hDDC, CPMAC_DDC_IOCTL_GET_STATISTICS,
                         (Uint32 *) & stats, NULL);

    if (result != 0) {
        CPMAC_DDC_LOGERR
            ("\ncpmacStats: Error from ioctl for DDC CPMAC_DDC_IOCTL_GET_STATISTICS \n");
        return;
    }
    if (stats.ifInOctets >= hDDC->Mib2IfHCCounter.inBytes) {
        hDDC->Mib2IfHCCounter.inBytesHC += (stats.ifInOctets - hDDC->Mib2IfHCCounter.inBytes);
    } else {
        hDDC->Mib2IfHCCounter.inBytesHC +=  0xffffffff - (hDDC->Mib2IfHCCounter.inBytes - stats.ifInOctets);
    }
    hDDC->Mib2IfHCCounter.inBytes = stats.ifInOctets;

    if (stats.ifInGoodFrames >= hDDC->Mib2IfHCCounter.inMulticastPkts + hDDC->Mib2IfHCCounter.inBroadcastPkts + 
        hDDC->Mib2IfHCCounter.inUnicastPkts) {
        hDDC->Mib2IfHCCounter.inUnicastPktsHC += ((stats.ifInGoodFrames - (stats.ifInBroadcasts + stats.ifInMulticasts))
                                                  - hDDC->Mib2IfHCCounter.inUnicastPkts);
    } else {

        hDDC->Mib2IfHCCounter.inUnicastPktsHC += 0xffffffff - (hDDC->Mib2IfHCCounter.inUnicastPkts - (stats.ifInGoodFrames 
				                 - (stats.ifInBroadcasts + stats.ifInMulticasts)));
    }
    hDDC->Mib2IfHCCounter.inUnicastPkts = (stats.ifInGoodFrames - (stats.ifInBroadcasts + stats.ifInMulticasts));


    if (stats.ifInMulticasts >= hDDC->Mib2IfHCCounter.inMulticastPkts) {
        hDDC->Mib2IfHCCounter.inMulticastPktsHC += (stats.ifInMulticasts - hDDC->Mib2IfHCCounter.inMulticastPkts);
    } else { 
        hDDC->Mib2IfHCCounter.inMulticastPktsHC += 0xffffffff - (hDDC->Mib2IfHCCounter.inMulticastPkts 
			                            - stats.ifInMulticasts);
    }
    hDDC->Mib2IfHCCounter.inMulticastPkts = stats.ifInMulticasts;

    if (stats.ifInBroadcasts >= hDDC->Mib2IfHCCounter.inBroadcastPkts) {
        hDDC->Mib2IfHCCounter.inBroadcastPktsHC +=
            (stats.ifInBroadcasts - hDDC->Mib2IfHCCounter.inBroadcastPkts);
    } else {

        hDDC->Mib2IfHCCounter.inBroadcastPktsHC += 0xffffffff - (hDDC->Mib2IfHCCounter.inBroadcastPkts 
			                           - stats.ifInBroadcasts);
    }
    hDDC->Mib2IfHCCounter.inBroadcastPkts = stats.ifInBroadcasts;

    if (stats.ifOutOctets >= hDDC->Mib2IfHCCounter.outBytes) {
        hDDC->Mib2IfHCCounter.outBytesHC +=  (stats.ifOutOctets - hDDC->Mib2IfHCCounter.outBytes);
    } else {

        hDDC->Mib2IfHCCounter.outBytesHC += 0xffffffff - (hDDC->Mib2IfHCCounter.outBytes - stats.ifOutOctets);
    }
    hDDC->Mib2IfHCCounter.outBytes = stats.ifOutOctets;

    if (stats.ifOutGoodFrames >= hDDC->Mib2IfHCCounter.outMulticastPkts +
        hDDC->Mib2IfHCCounter.outBroadcastPkts +
        hDDC->Mib2IfHCCounter.outUnicastPkts) {
        hDDC->Mib2IfHCCounter.outUnicastPktsHC += ((stats.ifOutGoodFrames - (stats.ifOutBroadcasts + stats.ifOutMulticasts))
                                                   - hDDC->Mib2IfHCCounter.outUnicastPkts);
    } else {
        hDDC->Mib2IfHCCounter.outUnicastPktsHC += 0xffffffff - (hDDC->Mib2IfHCCounter.outUnicastPkts - (stats.ifOutGoodFrames                                                  - (stats.ifOutBroadcasts + stats.ifOutMulticasts)));
    }
    hDDC->Mib2IfHCCounter.outUnicastPkts = (stats.ifOutGoodFrames - (stats.ifOutBroadcasts + stats.ifOutMulticasts));

    if (stats.ifOutMulticasts >= hDDC->Mib2IfHCCounter.outMulticastPkts) {
        hDDC->Mib2IfHCCounter.outMulticastPktsHC += (stats.ifOutMulticasts - hDDC->Mib2IfHCCounter.outMulticastPkts);
    } else {
        hDDC->Mib2IfHCCounter.outMulticastPktsHC += 0xffffffff - (hDDC->Mib2IfHCCounter.outMulticastPkts 
			                            - stats.ifOutMulticasts);
    }
    hDDC->Mib2IfHCCounter.outMulticastPkts = stats.ifOutMulticasts;

    if (stats.ifOutBroadcasts >= hDDC->Mib2IfHCCounter.outBroadcastPkts) {
        hDDC->Mib2IfHCCounter.outBroadcastPktsHC += (stats.ifOutBroadcasts - hDDC->Mib2IfHCCounter.outBroadcastPkts);
    } else {
        hDDC->Mib2IfHCCounter.outBroadcastPktsHC += 0xffffffff - (hDDC->Mib2IfHCCounter.outBroadcastPkts 
			                            - stats.ifOutBroadcasts);
    }
    hDDC->Mib2IfHCCounter.outBroadcastPkts = stats.ifOutBroadcasts;

    /* low 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inBytesLow =
        (unsigned long) hDDC->Mib2IfHCCounter.inBytesHC;
    /* high 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inBytesHigh =
        (hDDC->Mib2IfHCCounter.inBytesHC >> 32);


    /* low 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inUnicastPktsLow =
        (unsigned long) hDDC->Mib2IfHCCounter.inUnicastPktsHC;
    /* high 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inUnicastPktsHigh =
        (hDDC->Mib2IfHCCounter.inUnicastPktsHC >> 32);


    /* low 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inMulticastPktsLow =
        (unsigned long) hDDC->Mib2IfHCCounter.inMulticastPktsHC;
    /* high 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inMulticastPktsHigh =
        hDDC->Mib2IfHCCounter.inMulticastPktsHC >> 32;

    /* low 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inBroadcastPktsLow =
        (unsigned long) hDDC->Mib2IfHCCounter.inBroadcastPktsHC;
    /* high 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inBroadcastPktsHigh =
        hDDC->Mib2IfHCCounter.inBroadcastPktsHC >> 32;

    /* packets discarded due to resource limit */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inDiscardPkts =
        stats.ifRxDMAOverruns + stats.ifRxMofOverruns +
        stats.ifRxSofOverruns + stats.ifInCRCErrors +
        stats.ifInAlignCodeErrors + stats.ifInJabberFrames +
        stats.ifInFragments + stats.ifInOversizedFrames +
        stats.ifInUndersizedFrames + stats.ifInFilteredFrames +
        stats.ifInQosFilteredFrames;
    /* packets discarded due to format errors */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.inErrorPkts = stats.ifInCRCErrors
        + stats.ifInAlignCodeErrors
        + stats.ifInJabberFrames + stats.ifInFragments;

    hDDC->Mib2IfHCCounter.Mib2IfCounter.inUnknownProtPkts = 0;  /* Hardware doesn't support this.
                                                                   packets for unknown protocols */

    /* low 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outBytesLow =
        (unsigned long) hDDC->Mib2IfHCCounter.outBytesHC;
    /* high 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outBytesHigh =
        hDDC->Mib2IfHCCounter.outBytesHC >> 32;


    /* low 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outUnicastPktsLow =
        (unsigned long) hDDC->Mib2IfHCCounter.outUnicastPktsHC;
    /* high 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outUnicastPktsHigh =
        hDDC->Mib2IfHCCounter.outUnicastPktsHC >> 32;

    /* low 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outMulticastPktsLow =
        (unsigned long) hDDC->Mib2IfHCCounter.outMulticastPktsHC;
    /* high 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outMulticastPktsHigh =
        hDDC->Mib2IfHCCounter.outMulticastPktsHC >> 32;

    /* low 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outBroadcastPktsLow =
        (unsigned long) hDDC->Mib2IfHCCounter.outBroadcastPktsHC;
    /* high 32-bit of total octets received from media */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outBroadcastPktsHigh =
        hDDC->Mib2IfHCCounter.outBroadcastPktsHC >> 32;


    /* packets discarded due to format errors */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outErrorPkts =
        (stats.ifExcessiveCollisionFrames + stats.ifLateCollisions +
         stats.ifCarrierSenseErrors);

    /* packets discarded due to resource limit */
    hDDC->Mib2IfHCCounter.Mib2IfCounter.outDiscardPkts =
        stats.ifOutUnderrun +
        hDDC->Mib2IfHCCounter.Mib2IfCounter.outErrorPkts;
    return;
}

/* Debug functions */
#ifdef CPMAC_DDC_GETSTATS

extern int printf();

/* DDC specific debug functions */
void cpmacDDCStats(Uint32 instId, Uint8 txQueue, Uint8 rxQueue)
{
    CpmacDDCObj *hDDC;
    CpmacRxCppiCh *rxCppi;
    CpmacTxCppiCh *txCppi;

    if (instId >= CpmacDDCNumInst) {
        printf
            ("\ncpmacDumpRxStat: Invalid instance id %d. Currently only %d instances available",
             instId, CpmacDDCNumInst);
        return;
    }

    hDDC = CpmacDDCObject[instId];
    if (hDDC == NULL) {
        printf("\ncpmacDumpRxStat: Invalid DDC handle for instance id %d.",
               instId);
        return;
    }

    rxCppi = hDDC->rxCppi;
    txCppi = hDDC->txCppi;

    printf("\nCPMAC Instance %d Statistics", instId);
    if (hDDC->txTeardownPending == True)
        printf("\nTX Teardown pending ...");
    else {
        printf("\n");
        printf("\nTX Int Count                  = %u",
               hDDC->txIntCount[txQueue]);
        printf("\nTX Int with no active pkts    = %u",
               hDDC->txEmptyIntCount[txQueue]);
        printf("\nTX Out of BD's                = %u", txCppi->outOfTxBD);
    }

    if (hDDC->rxTeardownPending == True)
        printf("\nRX Teardown pending ...");
    else {
        printf("\n");
        printf("\nRX Int Count                  = %u",
               hDDC->rxIntCount[rxQueue]);
        printf("\nRX Out of Buffers             = %u",
               rxCppi->outOfRxBuffers);
        printf("\nRX Int with no active pkts    = %u",
               hDDC->rxEmptyIntCount[rxQueue]);
    }
    printf("\n");

}
#endif
