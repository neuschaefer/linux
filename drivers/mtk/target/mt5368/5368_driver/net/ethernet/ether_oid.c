/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/********************************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
* 
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: ether_oid.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *  Ethernet driver OID(ioctl) API source file
 *
 * Author:
 * -------
 *   Hs Huang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/
		
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "ether_com.h"
#include "ether_oid.h"
#include "ether_drv.h"

#include "eeprom_if.h"
#include "eepdef.h"


INT32 EEPNPTV_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
INT32 EEPNPTV_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
LINT_EXT_HEADER_END

/****************************************************************************
** Global variable
****************************************************************************/
//MISC_PARTITION_INFO                 _rMiscPartInfo;

/****************************************************************************
** Local definitions
****************************************************************************/
#define ETHER_DEVICE_NAME 			"ethernet device"
#define ETHER_DESCRIPTION 			"ethernet 10/100M"

/****************************************************************************
** Local structures and enumerations
****************************************************************************/

/****************************************************************************
** External function prototypes
****************************************************************************/
INT32 i4CmnEthGetNicTest(NET_DRV_IF_T *prDev, INT32 *pu4Result);

/****************************************************************************
** Local variable
****************************************************************************/

/****************************************************************************
** Local functions
****************************************************************************/
#define MISC_MAC_ADDR_SIZE      7

/*******************************************************************************
**  ucCalChksumByte
**
**  descriptions:
**      This routine is called to calculate two's complement checksum byte 
**      mass production procedure. Only this API can write mac to flash.
**  parameters:
**      pucArg - pointer to the argument 
**      u4Size - checksum size 
**  return:
**      two's complement checksum byte value
*******************************************************************************/
UINT8 ucCalChksumByte(UINT8 *pucArg, UINT32 u4Size)
{
    UINT8 ucChksum;
    UINT32 i;
    
    ASSERT(pucArg != NULL);

    ucChksum = 0;
    for(i = 0; i < u4Size; i++)
    {
        ucChksum += (UINT8)*(pucArg + i);
    }
    ucChksum = (ucChksum ^ 0xff) + 1;     // Get the two's complement

    return(ucChksum);
}

/*******************************************************************************
**  i4MiniCliSetMacAddr
**
**  descriptions:
**      This routine is used for mini cli to set mac address into eeprom at 
**      mass production procedure. Only this API can write mac to eeprom.
**      Both 1st and 2nd MAC will be overwrite.
**  parameters:
**      pMacAddr - the new MACAddress (a structure)
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
INT32 i4MiniCliSetMacAddr(MAC_ADDRESS_T *pMacAddr)
{

    INT32 i4Ret;
    UINT8 aMiscMac[MISC_MAC_ADDR_SIZE];
	UINT8 au1Addr[6];

    ASSERT(pMacAddr != NULL);

    x_memcpy(au1Addr, (UINT8*)pMacAddr, 6);

    
    if (((au1Addr[0] & 0x01) == 1) || 
    	((au1Addr[0] == 0) && (au1Addr[1] == 0) && (au1Addr[2] == 0) && (au1Addr[3] == 0) && (au1Addr[4] == 0) && (au1Addr[5] == 0)))
    {
		ERR_ETHER("MAC address is illegal\n");
        return (NET_DRV_ERROR);
	}
   
    vHalSetMacAddr((MAC_ADDRESS_T *)pMacAddr);

    x_memcpy(aMiscMac, pMacAddr, sizeof(MAC_ADDRESS_T));
    aMiscMac[6] = ucCalChksumByte((UINT8*)pMacAddr, sizeof(MAC_ADDRESS_T));
        
    i4Ret = EEPNPTV_Write(EEP_ETH_MAC_ADDR_START, (UINT32)aMiscMac, MISC_MAC_ADDR_SIZE);
    if(i4Ret)
    {
        ERR_ETHER("set MAC address fail 0x%02x\n", i4Ret);
        return (NET_DRV_ERROR);
    }
    else
    {
        INFO_ETHER("Mini cli set 1st MAC to EEP\n"); 
    }
    i4Ret = EEPNPTV_Write(EEP_ETH_MAC_ADDR_START + MISC_MAC_ADDR_SIZE, (UINT32)aMiscMac, MISC_MAC_ADDR_SIZE);    
    if(i4Ret)
    {
        ERR_ETHER("set 2nd MAC address fail 0x%02x\n", i4Ret);
        return (NET_DRV_ERROR);
    }
    else
    {
        INFO_ETHER("Mini cli set 2nd MAC to EEP\n"); 
    }
    return (NET_DRV_OK);

}

/*******************************************************************************
**  i4MiniCliGetMacAddr
**
**  descriptions:
**      This routine is used for mini cli to get MAC address from EEP
**  parameters:
**      pMacAddr - the new MACAddress (a structure)
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
INT32 i4MiniCliGetMacAddr(MAC_ADDRESS_T *pMacAddr)
{
	
    UCHAR *pucMisc;
    UINT8 ucChksum;
    UINT32 i;
    INT32 i4Ret;

    /* pointer must be 32 byte alignment */
    pucMisc = (UINT8*)x_mem_alloc(MISC_MAC_ADDR_SIZE + MISC_MAC_ADDR_SIZE);
    ASSERT(pucMisc != NULL);

    i4Ret = EEPNPTV_Read(EEP_ETH_MAC_ADDR_START, (UINT32)pucMisc,MISC_MAC_ADDR_SIZE+MISC_MAC_ADDR_SIZE);
    if(i4Ret)
    {
        ERR_ETHER("get MAC address fail 0x%02x\n", i4Ret);
        x_mem_free((UINT8*)pucMisc);
        return (NET_DRV_ERROR);
    }
    //check 1st MAC
    ucChksum = 0;
    for(i = 0; i < MISC_MAC_ADDR_SIZE; i++)
    {
        ucChksum += (UINT8)*(pucMisc + i);
    }
    if(ucChksum)
    {
        NOTE_ETHER("1st MAC in EEP is invalid! (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
        //check 2nd MAC
        ucChksum = 0;
        for(i = 0; i < MISC_MAC_ADDR_SIZE; i++)
        {
            ucChksum += (UINT8)*(pucMisc + i + MISC_MAC_ADDR_SIZE);
        }
        if(ucChksum)
        {
            NOTE_ETHER("Both MAC in EEP is invalid! Need write by cli mac\n");
            NOTE_ETHER("1st : !(%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
            NOTE_ETHER("2nd : !(%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc + MISC_MAC_ADDR_SIZE));      
        }
        else
        {
            NOTE_ETHER("2nd MAC in EEP is valid (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
            x_memcpy(pMacAddr, pucMisc, sizeof(MAC_ADDRESS_T));
        }
    }
    else
    {
        NOTE_ETHER("1st MAC in EEP is valid (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
        x_memcpy(pMacAddr, pucMisc, sizeof(MAC_ADDRESS_T));
    }
    NOTE_ETHER("1st : !(%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
    NOTE_ETHER("2nd : !(%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc + MISC_MAC_ADDR_SIZE));      
    
    x_mem_free((UINT8*)pucMisc);

//    vHalGetMacAddr((MAC_ADDRESS_T *)pMacAddr);
//    NOTE_ETHER("MAC (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pMacAddr));
    return (NET_DRV_OK);
   
}

/*******************************************************************************
**  i4CmnEthSetMacAddrDefault
**
**  descriptions:
**      This routine is used Default Mac address in EEPROM.
**  parameters:
**      prDev - pointer to struct net_device
**      pMacAddr - the new MACAddress (a structure)
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthSetMacAddrDefault(const NET_DRV_IF_T *prDev)
{
	
    INT32 i4Ret1, i4Ret2;
    UINT8 aMiscMac[MISC_MAC_ADDR_SIZE];

    ASSERT(prDev != NULL);

    //read 2nd Mac addr and Checksum
    i4Ret1 = EEPNPTV_Read(EEP_ETH_MAC_ADDR_START+MISC_MAC_ADDR_SIZE, (UINT32)aMiscMac,MISC_MAC_ADDR_SIZE);
    //write 1st Mac addr and Checksum
    i4Ret2 = EEPNPTV_Write(EEP_ETH_MAC_ADDR_START, (UINT32)aMiscMac, MISC_MAC_ADDR_SIZE);

    if(i4Ret1 || i4Ret2)
    {
        ERR_ETHER("set MAC address fail 0x%02x\n", i4Ret1);
        return (NET_DRV_ERROR);
    }
    else
    {
        INFO_ETHER("set MAC to EEP default\n"); 
    }
    //set HW register also.
    vHalSetMacAddr((MAC_ADDRESS_T *)aMiscMac);
    return (NET_DRV_OK);
    
}

/*******************************************************************************
**  i4CmnEthSetMacAddr
**
**  descriptions:
**      This routine is used for upper layer to set MAC address to register 
**      but write it into EEP depends on flag ui4_SetToFlash.
**      And it only overwrite 1st MAC.
**  parameters:
**      prDev - pointer to struct net_device
**      pMacAddr - the new MACAddress (a structure)
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthSetMacAddr(NET_DRV_IF_T *prDev, NET_PHY_ADDRESS_T *pPhyAddr)
{

    INT32 i4Ret;
    MAC_ADDRESS_T *pMacAddr = &(pPhyAddr->t_MacAddress);
    UINT8 aMiscMac[MISC_MAC_ADDR_SIZE];

    ASSERT(prDev != NULL);
    ASSERT(pPhyAddr != NULL);

    vHalSetMacAddr((MAC_ADDRESS_T *)pMacAddr);
    if(pPhyAddr->ui4_SetToFlash)
    {
        INFO_ETHER("upper layer set MAC to EEPROM\n"); 
        x_memcpy(aMiscMac, pMacAddr, sizeof(MAC_ADDRESS_T));
        aMiscMac[6] = ucCalChksumByte((UINT8*)pMacAddr, sizeof(MAC_ADDRESS_T));
        i4Ret = EEPNPTV_Write(EEP_ETH_MAC_ADDR_START, (UINT32)aMiscMac, MISC_MAC_ADDR_SIZE);
        if(i4Ret)
        {
            ERR_ETHER("set MAC address fail 0x%02x\n", i4Ret);
            return (NET_DRV_ERROR);
        }
        else
        {
            INFO_ETHER("Mini cli set MAC to EEP\n"); 
        }
    }
    return (NET_DRV_OK);
  
}

/*******************************************************************************
**  i4CmnEthGetMacAddr
**
**  descriptions:
**      This routine is called to get current MAC address
**  parameters:
**      prDev - pointer to struct net_device
**      pMacAddr - the new MACAddress (a structure)
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthGetMacAddr(NET_DRV_IF_T *prDev, NET_PHY_ADDRESS_T *pPhyAddr)
{
    MAC_ADDRESS_T *pMacAddr = &(pPhyAddr->t_MacAddress);

    ASSERT(prDev != NULL);
    ASSERT(pPhyAddr != NULL);

    vHalGetMacAddr(pMacAddr);

    return (NET_DRV_OK);
}

/*******************************************************************************
**  i4EthDrvGetMacAddr
**
**  descriptions:
**      This routine is called to get current MAC address by driver from HW(EEP)
**  parameters:
**      prDev - pointer to struct net_device
**      pMacAddr - the new MACAddress (a structure)
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
INT32 i4EthDrvGetMacAddr(MAC_ADDRESS_T *pMacAddr)
{
    UCHAR *pucMisc;
    UINT8 ucChksum;
    UINT32 i;
    INT32 i4Ret;

    /* pointer must be 32 byte alignment */
    pucMisc = (UINT8*)x_mem_alloc(MISC_MAC_ADDR_SIZE + MISC_MAC_ADDR_SIZE);
    ASSERT(pucMisc != NULL);

    i4Ret = EEPNPTV_Read(EEP_ETH_MAC_ADDR_START, (UINT32)pucMisc,MISC_MAC_ADDR_SIZE+MISC_MAC_ADDR_SIZE);
    if(i4Ret)
    {
        ERR_ETHER("get MAC address fail 0x%02x\n", i4Ret);
        x_mem_free((UINT8*)pucMisc);
        return (NET_DRV_ERROR);
    }
    //check 1st MAC
    ucChksum = 0;
    for(i = 0; i < MISC_MAC_ADDR_SIZE; i++)
    {
        ucChksum += (UINT8)*(pucMisc + i);
    }
    if(ucChksum || ((pucMisc[0]==0) && (pucMisc[1]==0) && (pucMisc[2]==0) && (pucMisc[3]==0) && (pucMisc[4]==0) && (pucMisc[5]==0)))
    {
        NOTE_ETHER("1st MAC in EEP is invalid! (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
        //check 2nd MAC
        ucChksum = 0;
        for(i = 0; i < MISC_MAC_ADDR_SIZE; i++)
        {
            ucChksum += (UINT8)*(pucMisc + i + MISC_MAC_ADDR_SIZE);
        }
        if(ucChksum || ((pucMisc[MISC_MAC_ADDR_SIZE]==0) && (pucMisc[MISC_MAC_ADDR_SIZE+1]==0) && (pucMisc[MISC_MAC_ADDR_SIZE+2]==0) 
                            && (pucMisc[MISC_MAC_ADDR_SIZE+3]==0) && (pucMisc[MISC_MAC_ADDR_SIZE+4]==0) && (pucMisc[MISC_MAC_ADDR_SIZE+5]==0)))
        {
            NOTE_ETHER("Both MAC in EEP is invalid! Need write by cli mac\n");
            NOTE_ETHER("1st : (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
            NOTE_ETHER("2nd : (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc + MISC_MAC_ADDR_SIZE));      
            x_mem_free((UINT8*)pucMisc);
            return (NET_DRV_ERROR);
        }
        else
        {
            NOTE_ETHER("2nd MAC in EEP is valid (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
            x_memcpy(pMacAddr, pucMisc, sizeof(MAC_ADDRESS_T));
        }
    }
    else
    {
        NOTE_ETHER("1st MAC in EEP is valid (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
        x_memcpy(pMacAddr, pucMisc, sizeof(MAC_ADDRESS_T));
    }
    NOTE_ETHER("1st : (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc));
    NOTE_ETHER("2nd : (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pucMisc + MISC_MAC_ADDR_SIZE));      
    
    x_mem_free((UINT8*)pucMisc);

//    vHalSetMacAddr((MAC_ADDRESS_T *)pMacAddr);
//    NOTE_ETHER("MAC (%02x:%02x:%02x:%02x:%02x:%02x)\n", ETH_MAC2STR(pMacAddr));
    return (NET_DRV_OK);
}


/*******************************************************************************
**  i4WlanGetMtu
**
**  descriptions:
**      This routine is called to query the Maximum Transmission Unit (MTU)
**      in unit of byte.
**  parameters:
**      prDev - pointer to struct net_device
**      pu4Mtu - MTU bytes
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthGetMtu (NET_DRV_IF_T *prDev, UINT32 *pu4Mtu)
{
    ASSERT(prDev != NULL);
    ASSERT(pu4Mtu != NULL);

    *pu4Mtu = ETHER_VALID_PKT_LEN;

    return (NET_DRV_OK);
}

/*******************************************************************************
**  i4CmnEthSetLinkSpeed
**
**  descriptions:
**      This routine is called to set the link speed in units of Mbps.
**  parameters:
**      prDev - pointer to struct net_device
**      pu4Speed - link speed in units of Mbps
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthSetLinkSpeed(NET_DRV_IF_T *prDev, UINT32 *pu4Speed)
{
    ASSERT(prDev != NULL);
    ASSERT(pu4Speed != NULL);

    if((*pu4Speed == 10) || (*pu4Speed == 100))
    {
        vHalSetLinkSpeed(pu4Speed);
    }
    else
    {
        WARN_ETHER("not supported link speed %d\n", *pu4Speed);
        return(NET_DRV_INV_OID);
    }
    
    return NET_DRV_OK;
}

/*******************************************************************************
**  i4CmnEthGetLinkSpeed
**
**  descriptions:
**      This routine is called to query the link speed in units of Mbps.
**  parameters:
**      prDev - pointer to struct net_device
**      pu4Speed - link speed in units of Mbps
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthGetLinkSpeed(NET_DRV_IF_T *prDev, UINT32 *pu4Speed)
{
    ASSERT(prDev != NULL);
    ASSERT(pu4Speed != NULL);

    vHalGetLinkSpeed(pu4Speed);
    return NET_DRV_OK;
}

/*******************************************************************************
**  i4CmnEthSetMulticastList
**
**  descriptions:
**      This routine is called to add specific multicast list 
**  parameters:
**      prDev - pointer to struct net_device
**      pMcList - pointer to multicast list structure
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthSetMulticastList(NET_DRV_IF_T *prDev, NET_MAC_ADDRESS_LIST_T *pMcList)
{
    UINT32 i;
    UINT32 u4McItemNum, u4McIdx;
    MAC_ADDRESS_T *pMcAddr, rMcAddrTmp;
    PETHER_ADAPTER_T prMac;

    ASSERT(prDev != NULL);
    ASSERT(pMcList != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    pMcAddr = (MAC_ADDRESS_T *)&pMcList->t_MulticastList[0];
   	u4McItemNum = pMcList->ui4_NumberOfItems;

    for (i = prMac->u4McListNum; (u4McItemNum && (i < ETHER_MAX_MULTICAST_NUM)); i++)
    {
        u4McIdx = 0;
        while(fgHalIsMulticastSet(u4McIdx) && 
              (u4McIdx < ETHER_MAX_MULTICAST_NUM))
        {
            vHalGetMulticastAddr(&rMcAddrTmp, u4McIdx);
            if(x_memcmp(&rMcAddrTmp, pMcAddr, sizeof(MAC_ADDRESS_T)) == OSR_OK)
            {
                pMcAddr ++;
                u4McIdx = 0;
                INFO_ETHER("The multicast address is existed\n");
                if(--u4McItemNum)
                {
                    /* check next multicast address */
                    continue;
                }
                else
                {
                    return (NET_DRV_OK);
                }
            }
            else
            {
                /* check flag to get available MAC address 1-15 */
                u4McIdx++;
            }
        }
       
        vHalAddMulticastAddr(pMcAddr, u4McIdx);
        
        pMcAddr ++;
        u4McItemNum--;
        prMac->u4McListNum ++;
        INFO_ETHER("existed multicast address: %d\n", prMac->u4McListNum);
    }

    return (NET_DRV_OK);
}

/*******************************************************************************
**  i4CmnEthDelMulticastList
**
**  descriptions:
**      This routine is called to delete specific multicast list 
**  parameters:
**      prDev - pointer to struct net_device
**      pMcList - pointer to multicast list structure
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthDelMulticastList(NET_DRV_IF_T *prDev, NET_MAC_ADDRESS_LIST_T *pMcList)
{
    UINT32 u4McItemNum, u4McIdx;
    MAC_ADDRESS_T rGetMcAddr;
    MAC_ADDRESS_T *pMcAddr;
    PETHER_ADAPTER_T prMac;

    ASSERT(prDev != NULL);
    ASSERT(pMcList != NULL);
    pMcAddr = (MAC_ADDRESS_T *)&pMcList->t_MulticastList[0];
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
   	u4McItemNum = pMcList->ui4_NumberOfItems;

    while(u4McItemNum)
    {
            u4McIdx = 0;
        while(u4McIdx < ETHER_MAX_MULTICAST_NUM)
            {
            if(fgHalIsMulticastSet(u4McIdx))
            {
                vHalGetMulticastAddr(&rGetMcAddr, u4McIdx);
                if(x_memcmp(pMcAddr, &rGetMcAddr, ETHER_MAX_ADDR_LEN) == OSR_OK)
            {
                vHalDelMulticastAddr(u4McIdx);
                    ASSERT(prMac->u4McListNum);
                    prMac->u4McListNum --;
                break;
            }
            }
            u4McIdx++;
        }
        
        pMcAddr ++;
        u4McItemNum --;
        if(prMac->u4McListNum == 0)
        {
            /* All multicast lists are deleted */
            break;
        }
    }

    return (NET_DRV_OK);
}

/*******************************************************************************
**  i4CmnEthGetMulticastList
**
**  descriptions:
**      This routine is called to get multicast list 
**  parameters:
**      prDev - pointer to struct net_device
**      pMcList - pointer to multicast list structure
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthGetMulticastList(NET_DRV_IF_T *prDev, NET_MAC_ADDRESS_LIST_T *pMcList)
{
    UINT32 i;
    UINT32 u4McItemNum, u4McIdx;
    MAC_ADDRESS_T *pMcAddr;
    PETHER_ADAPTER_T prMac;

    ASSERT(prDev != NULL);
    ASSERT(pMcList != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    pMcAddr = (MAC_ADDRESS_T *)&pMcList->t_MulticastList[0];
    pMcList->ui4_NumberOfItems = prMac->u4McListNum;
   	u4McItemNum = pMcList->ui4_NumberOfItems;

    u4McIdx = 0;
    for (i = 0; (u4McItemNum && (i < prMac->u4McListNum)); i++)
    {
        while(!fgHalIsMulticastSet(u4McIdx) && 
              (u4McIdx < ETHER_MAX_MULTICAST_NUM))
        {
            /* check flag to get existed MAC address */
            u4McIdx++;
        }
        vHalGetMulticastAddr(pMcAddr, u4McIdx);
        
        pMcAddr ++;
        u4McIdx ++;
        u4McItemNum --;
    }

    return (NET_DRV_OK);
}

/*******************************************************************************
**  i4CmnEthSetMulticastAddr
**
**  descriptions:
**      This routine is called to set one multicast address
**  parameters:
**      prDev - pointer to struct net_device
**      pMcList - pointer to multicast address array
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthSetMulticastAddr(NET_DRV_IF_T *prDev, MAC_ADDRESS_T *pMcList)
{
    NET_MAC_ADDRESS_LIST_T rMcList;

    ASSERT(prDev != NULL);
    ASSERT(pMcList != NULL);

    rMcList.ui4_NumberOfItems = 1;
    x_memcpy(rMcList.t_MulticastList, pMcList, ETHER_MAX_ADDR_LEN);

    return(i4CmnEthSetMulticastList(prDev, (NET_MAC_ADDRESS_LIST_T*)&rMcList));
}

/*******************************************************************************
**  i4CmnEthSetPromiseMode
**
**  descriptions:
**      This routine is called to set ethernet promiscuous mode
**  parameters:
**      prDev - pointer to struct net_device
**      pu4Promise - pointer to promiscuous mode setting
**  result:
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthSetPromiseMode(NET_DRV_IF_T *prDev, UINT32 *pu4Promise)
{
    ASSERT(prDev != NULL);
    ASSERT(pu4Promise != NULL);

    vHalSetPromiseMode(pu4Promise);
	
    return (NET_DRV_OK);
}

/*******************************************************************************
**  i4CmnEthGetPromiseMode
**
**  descriptions:
**      This routine is called to get ethernet promiscuous mode
**  parameters:
**      prDev - pointer to struct net_device
**      pu4Promise - pointer to promiscuous mode setting
**  result:
**		1 - promiscuous mode is enabled
**		0 - promiscuous mode is enabled
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4CmnEthGetPromiseMode(NET_DRV_IF_T *prDev, UINT32 *pu4Promise)
{
    ASSERT(prDev != NULL);
    ASSERT(pu4Promise != NULL);

    vHalGetPromiseMode(pu4Promise);
    return NET_DRV_OK;
}

/*******************************************************************************
**  i4CmnEthGetLinkStatus
**
**  descriptions:
**      This routine is called to get ethernet linking status
**  parameters:
**      prDev - pointer to struct net_device
**      pStatus - pointer to link status
**  result:
**      NI_DRV_EV_ETHERNET_PLUGIN - connected
**      NI_DRV_EV_ETHERNET_UNPLUG - disconnected
**  return:
**      0 on success, others on failure
*******************************************************************************/
static INT32 i4CmnEthGetLinkStatus(NET_DRV_IF_T *prDev, UINT32 *pStatus)
{
    PETHER_ADAPTER_T prMac;
    INT32 i4Ret;

    ASSERT(prDev != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;

    i4Ret = i4HalPhyStatusChk(prMac);
    if(i4Ret == NET_DRV_OK)
    {
        if(GET_ETHER_INFO(prMac, ETHER_LINK_UP))
        {
           *pStatus = NI_DRV_EV_ETHERNET_PLUGIN; 
        }
        else
        {
           *pStatus = NI_DRV_EV_ETHERNET_UNPLUG; 
        }
    } 

    return(i4Ret);
}

/*******************************************************************************
**  i4EthSetDuplexMode
**
**  descriptions:
**      This routine is called to set ethernet duplex mode
**  parameters:
**      prDev - pointer to struct net_device
**      pu4Duplex - pointer to duplex mode 
**  result:
**		0 - half duplex mode
**		1 - full duplex mode
**		2 - unknown
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4EthSetDuplexMode(NET_DRV_IF_T *prDev, UINT32 *pu4Duplex)
{
    ASSERT(prDev != NULL);
    ASSERT(pu4Duplex != NULL);

    if(*pu4Duplex > (UINT32)FULL_DUPLEX)
    {
        return (NET_DRV_INV_OID);
    }
    else
    {
        vHalSetDuplexMode(pu4Duplex);
        return (NET_DRV_OK);
    }
}

/*******************************************************************************
**  i4EthGetDuplexMode
**
**  descriptions:
**      This routine is called to get ethernet duplex mode
**  parameters:
**      prDev - pointer to struct net_device
**      pu4Duplex - pointer to duplex mode 
**  result:
**		0 - half duplex mode
**		1 - full duplex mode
**		2 - unknown
**  return:
**      NET_DRV_OK
*******************************************************************************/
static INT32 i4EthGetDuplexMode(NET_DRV_IF_T *prDev, UINT32 *pu4Duplex)
{
    ASSERT(prDev != NULL);
    ASSERT(pu4Duplex != NULL);

    vHalGetDuplexMode(pu4Duplex);
    return NET_DRV_OK;
}

/*******************************************************************************
**  i4EtherOidSet
**
**  descriptions:
**      This routine is called to SET common OIDs => OID_ETHER_OID_START region
**  parameters:
**      prDev - pointer to net device structure
**      i4Cmd - common OID command
**      pvParam - pointer to OID parameter
**  result:
**  return:
**      0 if success
**      negative if fail 
*******************************************************************************/
INT32 i4EtherOidSet(NET_DRV_IF_T *prDev, INT32 i4Cmd, VOID *pvParam)
{
    INT32 i4Ret = 0;

    ASSERT(prDev != NULL);
    ASSERT(pvParam != NULL);

    switch(i4Cmd)
    {
        case OID_ETHER_DUPLEXSTATUS:
        	i4Ret = i4EthSetDuplexMode(prDev, (UINT32*)pvParam);
        	break;
        default:
            i4Ret = i4HalSetEtherStatInfo(i4Cmd, pvParam);
        	break;
    }

    return(i4Ret);
}

/*******************************************************************************
**  i4EtherOidGet
**
**  descriptions:
**      This routine is called to GET common OIDs => OID_ETHER_OID_START region
**  parameters:
**      prDev - pointer to net device structure
**      i4Cmd - common OID command
**      pvParam - pointer to OID parameter
**  result:
**  return:
**      0 if success
**      negative if fail 
*******************************************************************************/
INT32 i4EtherOidGet(NET_DRV_IF_T *prDev, INT32 i4Cmd, VOID *pvParam)
{
    INT32 i4Ret;

    ASSERT(prDev != NULL);
    ASSERT(pvParam != NULL);

    switch(i4Cmd)
    {
        case OID_ETHER_DUPLEXSTATUS:
        	i4Ret = i4EthGetDuplexMode(prDev, (UINT32*)pvParam);
        	break;
        default:
            i4Ret = i4HalGetEtherStatInfo(i4Cmd, pvParam);
        	break;
    }

    return(i4Ret);
}

/*******************************************************************************
**  i4CmnEtherOidSet
**
**  descriptions:
**      This routine is called to SET common OIDs => OID_GROUP_CMN_IF region
**  parameters:
**      prDev - pointer to net device structure
**      i4Cmd - common OID command
**      pvParam - pointer to OID parameter
**  result:
**  return:
**      0 if success
**      negative if fail 
*******************************************************************************/
INT32 i4CmnEtherOidSet(NET_DRV_IF_T *prDev, INT32 i4Cmd, VOID *pvParam)
{
    INT32 i4Ret = 0;
    PETHER_ADAPTER_T prMac;
    PMAC_STAT_T prStat;
    UINT64 *pu8Parm;

    ASSERT(prDev != NULL);
    ASSERT(pvParam != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    prStat = (PMAC_STAT_T)&prMac->rStat;
    pu8Parm = (UINT64 *)pvParam;

    switch(i4Cmd)
    {
        case OID_CMN_IF_SPEED:
        	i4Ret = i4CmnEthSetLinkSpeed(prDev, (UINT32*)pvParam);
        	break;
        case OID_CMN_IF_PHYADDRESS:
            i4Ret = i4CmnEthSetMacAddr(prDev, (NET_PHY_ADDRESS_T*)pvParam);
        	break;
        case OID_CMN_IF_PHYADDRESS_DEFAULT:
            i4Ret = i4CmnEthSetMacAddrDefault(prDev);
        	break;
        case OID_CMN_IF_MULTICASTADDRESSLIST:
        	i4Ret = i4CmnEthSetMulticastList(prDev, (NET_MAC_ADDRESS_LIST_T*)pvParam);
        	break;
        case OID_CMN_IF_MULTICASTADDRESS_DEL:
            i4Ret = i4CmnEthDelMulticastList(prDev, (NET_MAC_ADDRESS_LIST_T*)pvParam);
            break;
        case OID_CMN_IF_MULTICASTADDRESS:
            i4Ret = i4CmnEthSetMulticastAddr(prDev, (MAC_ADDRESS_T*)pvParam);
        	break;
        case OID_CMN_IF_PROMISCUOUS_MODE:
        	i4Ret = i4CmnEthSetPromiseMode(prDev, (UINT32*)pvParam);
        	break;

        /* set statistics information */ 
        case OID_CMN_IF_IN_OCTETS:        
            prStat->u8RxOctet = *pu8Parm;
            break;
        case OID_CMN_IF_IN_UCASTPKT: 
            prStat->u8RxFrame = *pu8Parm;
            break;
        case OID_CMN_IF_IN_DISCARDS:      
        case OID_CMN_IF_IN_ERRORS:        
        case OID_CMN_IF_IN_UNKNOWN_PROTOS:
        case OID_CMN_IF_OUT_OCTETS:       
        case OID_CMN_IF_IN_MULTICAST_PKTS:
        case OID_CMN_IF_IN_BROADCAST_PKTS:
        case OID_CMN_IF_OUT_MULTICAST_PKTS:
        case OID_CMN_IF_OUT_BROADCAST_PKTS:
        case OID_CMN_IF_OUT_UCASTPKTS:    
        case OID_CMN_IF_OUT_DISCARDS:     
        case OID_CMN_IF_OUT_ERRORS:       
        default:
            i4Ret = NET_DRV_INV_OID;
        	break;
    }

    return(i4Ret);
}

/*******************************************************************************
**  i4CmnEtherOidGet
**
**  descriptions:
**      This routine is called to GET common OIDs => OID_GROUP_CMN_IF region
**  parameters:
**      prDev - pointer to net device structure
**      i4Cmd - common OID command
**      pvParam - pointer to OID parameter
**  result:
**  return:
**      0 if success
**      negative if fail 
*******************************************************************************/
INT32 i4CmnEtherOidGet(NET_DRV_IF_T *prDev, INT32 i4Cmd, VOID *pvParam)
{
    INT32 i4Ret = 0;
    PETHER_ADAPTER_T prMac;
    PMAC_STAT_T prStat;
    UINT64 *pu8Parm;

    ASSERT(prDev != NULL);
    ASSERT(pvParam != NULL);
    prMac = (PETHER_ADAPTER_T)prDev->pv_priv;
    prStat = (PMAC_STAT_T)&prMac->rStat;
    pu8Parm = (UINT64 *)pvParam;
    
    switch(i4Cmd)
    {
        case OID_CMN_IF_MTU:
        	i4Ret = i4CmnEthGetMtu(prDev, (UINT32*)pvParam);
        	break;
        case OID_CMN_IF_SPEED:
        	i4Ret = i4CmnEthGetLinkSpeed(prDev, (UINT32*)pvParam);
        	break;
        case OID_CMN_IF_PHYADDRESS:
            i4Ret = i4CmnEthGetMacAddr(prDev, (NET_PHY_ADDRESS_T*)pvParam);
        	break;
        case OID_CMN_IF_MULTICASTADDRESSLIST:
        	i4Ret = i4CmnEthGetMulticastList(prDev, (NET_MAC_ADDRESS_LIST_T*)pvParam);
        	break;
        case OID_CMN_IF_PROMISCUOUS_MODE:
        	i4Ret = i4CmnEthGetPromiseMode(prDev, (UINT32*)pvParam);
        	break;
        case OID_CMN_IF_CONNECT_STATUS: /* robert */
            i4Ret = i4CmnEthGetLinkStatus(prDev, (UINT32*) pvParam);
            break;
        case OID_CMN_IF_NIC_STATUS:
            i4Ret = i4CmnEthGetNicTest(prDev, (INT32*)pvParam);
            break;

        /* get statistics information */ 
        case OID_CMN_IF_IN_OCTETS:        
            *pu8Parm = (UINT64)prStat->u8RxOctet;
            break;
        case OID_CMN_IF_IN_UCASTPKT:      
            *pu8Parm = (UINT64)prStat->u8RxFrame;
            break;

        case OID_CMN_IF_IN_DISCARDS:      
        case OID_CMN_IF_IN_ERRORS:        
        case OID_CMN_IF_IN_UNKNOWN_PROTOS:
        case OID_CMN_IF_OUT_OCTETS:       
        case OID_CMN_IF_IN_MULTICAST_PKTS:
        case OID_CMN_IF_IN_BROADCAST_PKTS:
        case OID_CMN_IF_OUT_MULTICAST_PKTS:
        case OID_CMN_IF_OUT_BROADCAST_PKTS:
        case OID_CMN_IF_OUT_UCASTPKTS:    
        case OID_CMN_IF_OUT_DISCARDS:     
        case OID_CMN_IF_OUT_ERRORS:       
        default:
            i4Ret = NET_DRV_INV_OID;
        	break;
    }

    return(i4Ret);
}



