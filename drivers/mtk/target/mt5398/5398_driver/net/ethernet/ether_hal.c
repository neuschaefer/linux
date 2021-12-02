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
 *   $Workfile: ether_hal.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet MAC driver code
 *
 * Author: Hs Huang
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

#include "x_clksrc.h"
#include "x_pinmux.h"
#include "ether_com.h"
#include "ether_oid.h"
#include "ether_drv.h"
#include "drvcust_if.h"

// *********************************************************************
// Constant Data Declaration
// *********************************************************************
static const MAC_ADDRESS_T MAC_ADDR = {0x00, 0x0C, 0xE7, 0x06, 0x00, 0x00};

/* Destination address(DA) sample to test hash filter, refer to spec. p.208 */
//static const MAC_ADDRESS_T HASH_DA_SAMPLE1 = {0x1f, 0x52, 0x41, 0x9c, 0xb6, 0xaf};
//static const MAC_ADDRESS_T HASH_DA_SAMPLE2 = {0xa0, 0x0a, 0x98, 0x00, 0x00, 0x45};

// *********************************************************************
// Local function prototype
// *********************************************************************

// =====================================================================
// MAC register block handling functions
// =====================================================================

// *********************************************************************
// Function    : void vHalSetMacAddr(UINT32 *pu4Addr)
// Description : Set MAC destination address
// Parameter   : 6 byte Address point, pu4Addr
// Return      : None
// *********************************************************************
void vHalSetMacAddr(MAC_ADDRESS_T *pMacAddr)
{
    UINT32 u4AddrH, u4AddrL;
    UINT32 u4AddrH2, u4AddrL2;

    ASSERT(pMacAddr != NULL);

    u4AddrL = 0;
    u4AddrH = 0;
    x_memcpy((UINT8*)&u4AddrL, (UINT8*)pMacAddr, 4);
    x_memcpy((UINT8*)&u4AddrH, (UINT8*)pMacAddr + 4, 2);

    u4AddrH2 = (UINT32)(((u4AddrL & 0xFF) << 8) + ((u4AddrL & 0xFF00) >> 8));
    u4AddrL2 = (UINT32)(((u4AddrL & 0xFF0000) << 8) + ((u4AddrL & 0xFF000000) >> 8) +
    	                ((u4AddrH & 0xFF) << 8) + ((u4AddrH & 0xFF00) >> 8)) ;
    
    vWriteMAC(RW_MY_MAC_H, u4AddrH2);
    vWriteMAC(RW_MY_MAC_L, u4AddrL2);

    //vWriteMACPDWNC(RW_MY_MAC_H, u4AddrH2);
   // vWriteMACPDWNC(RW_MY_MAC_L, u4AddrL2);
}

// *********************************************************************
// Function    : void vHalGetMacAddr(UINT32 *pu4Addr)
// Description : Get MAC destination address
// Parameter   : 6 byte Address point, pu4Addr
// Return      : None
// *********************************************************************
void vHalGetMacAddr(MAC_ADDRESS_T *pMacAddr)
{
    UINT32 u4AddrH, u4AddrL;
    UINT32 u4AddrH2, u4AddrL2;

    ASSERT(pMacAddr != NULL);

    u4AddrH = dReadMAC(RW_MY_MAC_H);
    u4AddrL = dReadMAC(RW_MY_MAC_L);

	u4AddrH2 = (UINT32)(((u4AddrL & 0xFF) << 8) + ((u4AddrL & 0xFF00) >> 8));
    u4AddrL2 = (UINT32)(((u4AddrL & 0xFF0000) << 8) + ((u4AddrL & 0xFF000000) >> 8) +
    	                ((u4AddrH & 0xFF) << 8) + ((u4AddrH & 0xFF00) >> 8)) ;

    x_memcpy((UINT8*)pMacAddr, (UINT8*)&u4AddrL2, 4);
    x_memcpy((UINT8*)pMacAddr + 4, (UINT8*)&u4AddrH2, 2);
}

// *********************************************************************
// Function    : vHalAddMulticastAddr
// Description : Set MAC multicast address
// Parameter   : pMacAddr - pointer to MAC adress
//               u4AddrIdx - MAC address 1 - 15
// Return      : None
// *********************************************************************
void vHalAddMulticastAddr(MAC_ADDRESS_T *pMacAddr, UINT32 u4AddrIdx)
{
    UINT32 u4AddrH, u4AddrL;
    UINT32 u4AddrH2, u4AddrL2;

    ASSERT(pMacAddr != NULL);

    u4AddrL = 0;
    u4AddrH = 0;
    x_memcpy((UINT8*)&u4AddrL, (UINT8*)pMacAddr, 4);
    x_memcpy((UINT8*)&u4AddrH, (UINT8*)pMacAddr + 4, 2);

	u4AddrH2 = (UINT32)(((u4AddrL & 0xFF) << 8) + ((u4AddrL & 0xFF00) >> 8));
    u4AddrL2 = (UINT32)(((u4AddrL & 0xFF0000) << 8) + ((u4AddrL & 0xFF000000) >> 8) +
    	                ((u4AddrH & 0xFF) << 8) + ((u4AddrH & 0xFF00) >> 8)) ;
    
    u4AddrH2 |= ADDR_EN;
    u4AddrIdx = u4AddrIdx * 8;
    
    vWriteMAC(RW_MY_MAC1_H + u4AddrIdx, u4AddrH2);
    vWriteMAC(RW_MY_MAC1_L + u4AddrIdx, u4AddrL2);
}

// *********************************************************************
// Function    : vHalDelMulticastAddr
// Description : Delete MAC multicast address
// Parameter   : pMacAddr - pointer to MAC adress
//               u4AddrIdx - MAC address 1 - 15
// Return      : None
// *********************************************************************
void vHalDelMulticastAddr(UINT32 u4AddrIdx)
{
    u4AddrIdx = u4AddrIdx * 8;

    vWriteMAC(RW_MY_MAC1_H + u4AddrIdx, 0x0000ffff);
    vWriteMAC(RW_MY_MAC1_L + u4AddrIdx, 0xffffffff);
}

// *********************************************************************
// Function    : vHalGetMulticastAddr
// Description : Get MAC multicast address
// Parameter   : pMacAddr - pointer to MAC adress
//               u4AddrIdx - MAC address 1 - 15
// Return      : None
// *********************************************************************
void vHalGetMulticastAddr(MAC_ADDRESS_T *pMacAddr, UINT32 u4AddrIdx)
{
    UINT32 u4AddrH, u4AddrL;
    UINT32 u4AddrH2, u4AddrL2;

    ASSERT(pMacAddr != NULL);

    u4AddrIdx = u4AddrIdx * 8;
    u4AddrH = dReadMAC(RW_MY_MAC1_H + u4AddrIdx);
    u4AddrL = dReadMAC(RW_MY_MAC1_L + u4AddrIdx);

	u4AddrH2 = (UINT32)(((u4AddrL & 0xFF) << 8) + ((u4AddrL & 0xFF00) >> 8));
    u4AddrL2 = (UINT32)(((u4AddrL & 0xFF0000) << 8) + ((u4AddrL & 0xFF000000) >> 8) +
    	                ((u4AddrH & 0xFF) << 8) + ((u4AddrH & 0xFF00) >> 8)) ;

    x_memcpy((UINT8*)pMacAddr, (UINT8*)&u4AddrL2, 4);
    x_memcpy((UINT8*)pMacAddr + 4, (UINT8*)&u4AddrH2, 2);
}

// *********************************************************************
// Function    : fgHalIsMulticastSet
// Description : Check if the specific multicast list is enabled 
// Parameter   : u4AddrIdx - MAC address 1 - 15
// Return      : 1->enabled, 0->disabled 
// *********************************************************************
BOOL fgHalIsMulticastSet(UINT32 u4AddrIdx)
{
    UINT32 u4Tmp;

    u4Tmp = (UINT32)dReadMAC(RW_MY_MAC1_H + (u4AddrIdx * 8)) & ADDR_EN;
    return((u4Tmp == 0x00) ? FALSE : TRUE);
}

// *********************************************************************
// Function    : vHalGetLinkSpeed
// Description : Get MAC link speed
// Parameter   : pu4Speed - pointer of link speed in unit of Mbps
// Return      : none
// *********************************************************************
void vHalGetLinkSpeed(UINT32 *pu4Speed)
{
    UINT32 u4Speed;
    
    ASSERT(pu4Speed != NULL);

    u4Speed = dReadMAC(RW_PHY_CTRL1) & FORCE_SPEED;
    if (u4Speed == FORCE_SPEED_100M)
    {
        *pu4Speed = 100;
        INFO_ETHER("link speed: %dMbps\n", *pu4Speed);
    }
    else if (u4Speed == FORCE_SPEED_10M)
    {
        *pu4Speed = 10;
        INFO_ETHER("link speed: %dMbps\n", *pu4Speed);
    }
    else
    {
		*pu4Speed = 0;
        ERR_ETHER("link speed error");
        ASSERT(0);
    }
}

// *********************************************************************
// Function    : vHalSetLinkSpeed
// Description : Set MAC link speed
// Parameter   : pu4Speed - pointer of link speed in unit of Mbps
// Return      : none
// *********************************************************************
void vHalSetLinkSpeed(UINT32 *pu4Speed)
{
    ASSERT(pu4Speed != NULL);

    vWriteMAC(RW_PHY_CTRL1, dReadMAC(RW_PHY_CTRL1) | MI_DIS);
    if(*pu4Speed == 10)
    {
        SetBitMAC(RW_PHY_CTRL1, FORCE_SPEED_10M);
    }
    else
    if(*pu4Speed == 100)
    {
        SetBitMAC(RW_PHY_CTRL1, FORCE_SPEED_100M);
    }
}

// *********************************************************************
// Function    : vHalGetPromiseMode
// Description : Get MAC promise mode
// Parameter   : pu4Promise - pointer of promise mode
//               TRUE: promise mode; FALSE: non-promise mode
// Return      : none
// *********************************************************************
void vHalGetPromiseMode(UINT32 *pu4Promise)
{
    ASSERT(pu4Promise != NULL);

    if(dReadMAC(RW_ARL_CFG) & MISC_MODE)
    {
        *pu4Promise = TRUE;
    }
    else
    {
        *pu4Promise = FALSE;
    }
}

// *********************************************************************
// Function    : vHalSetPromiseMode
// Description : Set MAC promise mode
// Parameter   : pu4Promise - pointer of promise mode
//               TRUE: promise mode; FALSE: non-promise mode
// Return      : none
// *********************************************************************
void vHalSetPromiseMode(UINT32 *pu4Promise)
{
    ASSERT(pu4Promise != NULL);

    if(*pu4Promise == FALSE)
    {
        ClrBitMAC(RW_ARL_CFG, MISC_MODE);
    }
    else
    {
        SetBitMAC(RW_ARL_CFG, MISC_MODE);
    }
}

// *********************************************************************
// Function    : vHalGetDuplexMode
// Description : Get MAC promise mode
// Parameter   : pu4Duplex - pointer of duplex mode
//               0 - half duplex mode
//               1 - full duplex mode
// Return      : none
// *********************************************************************
void vHalGetDuplexMode(UINT32 *pu4Duplex)
{
    ASSERT(pu4Duplex != NULL);

    if(dReadMAC(RW_PHY_CTRL1) & FORCE_DUPLEX)
    {
        *pu4Duplex = (UINT32)(FULL_DUPLEX);
    }
    else
    {
        *pu4Duplex = (UINT32)(HALF_DUPLEX);
    }
}

// *********************************************************************
// Function    : vHalSetDuplexMode
// Description : Set MAC promise mode
// Parameter   : pu4Duplex - pointer of duplex mode
//               0 - half duplex mode
//               1 - full duplex mode
// Return      : none
// *********************************************************************
void vHalSetDuplexMode(UINT32 *pu4Duplex)
{
    ASSERT(pu4Duplex != NULL);

    vWriteMAC(RW_PHY_CTRL1, dReadMAC(RW_PHY_CTRL1) | MI_DIS);
    if(*pu4Duplex == 0x00)
    {
        SetBitMAC(RW_PHY_CTRL1, FORCE_DUPLEX_HALF);
    }
    else
    {
        SetBitMAC(RW_PHY_CTRL1, FORCE_DUPLEX_FULL);
    }
}

// *********************************************************************
// Function    : vMacHashTableSet
// Description : It's used to calculate upper 6 bits CRC value for hash table
// Parameter   : prHashAddr
// Return      : returns hash bit number for given MAC address
// *********************************************************************
#if 0
static void vMacHashTableSet(MAC_ADDRESS_T *prHashAddr)   
{
    INT16 i, bit;
    UCHAR data, result;
  UINT32 crc = 0xFFFFFFFF;
#if CONFIG_ETHER_CRC_NORMAL    
    UINT32 poly = 0x04c11db7;
#else
  UINT32 poly = 0xEDB88320;
#endif

    ASSERT(prHashAddr != NULL);
  
  for( i=0; i<6; i++ )
  {
    data = *((UCHAR *)prHashAddr + i);
    for( bit=0; bit<8; bit++ )
    {
    #if CONFIG_ETHER_CRC_NORMAL   
            /* normal CRC32 */
            if((crc^data) & 0x80000000)
            {
                crc = (crc << 1) ^ poly;
            }    
            else
            {
                crc = crc << 1;
            }    
            data <<= 1;
    #else
            /* inverted/reversed CRC32 */
            if((crc^data) & 0x01)
            {
                crc = (crc >> 1) ^ poly;
            }    
            else
            {
                crc = crc >> 1;
            }    
      data >>= 1;
    #endif
    }
  }

    /* invert and swap crc value according to HW sample code */
  result = 0;
  crc ^= 0xFFFFFFFF;
  for(bit=0; bit<6; bit++)
  {
      /* get the first 6 bits */
      if((crc & 0x01) != 0)
      {
           result = (result << 1) | 1;
      }	
      else
      {
          result <<= 1;
      }
      crc >>= 1;
  }
  result &= 0x3F;      

  /* set 64-bit hash table */
  if(result >= 32)
  {
      SetBitMAC(RW_MAC_R2_HASH_H, (1 << (result-32)));
  }    
  else 
  {
      SetBitMAC(RW_MAC_R3_HASH_L, (1 << result));
  }	
}
#endif

// *********************************************************************
// Function    : i4HalPhyStatusChk
// Description : MAC PHY link status check
// Parameter   : prMac
// Return      : None
// *********************************************************************
INT32 i4HalPhyStatusChk(PETHER_ADAPTER_T prMac)
{
    PMAC_MII_T prMii;
    INT32 i4Ret;
    static BOOL fgRegAlreadySet;
    	
    ASSERT(prMac != NULL);
                
    prMii = &prMac->rMii;

    // Init PHY and check link status
    i4Ret = i4MacPhyNegotitate(prMac);
    if(i4Ret != NET_DRV_OK)
    {
        return(i4Ret);
    }
    else
    {
        if(!GET_ETHER_INFO(prMac, ETHER_LINK_UP))
        {
            fgRegAlreadySet = FALSE;	
            /* cable is not conntected, no need to set mode */
            return(NET_DRV_OK);
        }
        else 
        {
            if(fgRegAlreadySet)
            {
                /* cable is conntected and MAC register is already set */
                return(NET_DRV_OK);
            }    
            fgRegAlreadySet = TRUE;	
        }
    }

    if(prMii->fgFullDup)
    {
        SetBitMAC(RW_PHY_CTRL1, FORCE_DUPLEX_FULL); 
    }    
    else
    {
        ClrBitMAC(RW_PHY_CTRL1, FORCE_DUPLEX_FULL); 
    }    

    /* 100/10 Mbps selection */
    if(prMii->fgSpeed100)
    {
        SetBitMAC(RW_PHY_CTRL1, FORCE_SPEED_100M); 
    }    
    else
    {
        SetBitMAC(RW_PHY_CTRL1, FORCE_SPEED_10M); 
    }    
    
    return(NET_DRV_OK);
}

// *********************************************************************
// Function    : vHalWakeupFrameFilterSet
// Description : Set wakeup frame filter register 
// Parameter   : prWkFrame
// Return      : None
// *********************************************************************
void vHalWakeupFrameFilterSet(ETHER_WAKEUP_FRAME_T *prWkFrame)
{
	UNUSED(prWkFrame);
#if 0
    UINT32 i, u4Data;
    
    ASSERT(prWkFrame != NULL);
    /* fill wakeup frame filter register */
    for(i = 0; i < 8; i++)
    {
        u4Data = (UINT32)prWkFrame[i];
        vWriteMAC(RW_MAC_R10_WKFIL, u4Data);
    }
#endif    
}

// *********************************************************************
// Function    : vHalSetMacPwrDown
// Description : Ethernet driver power down. All received frame will be 
//               dropped after calling this API. Only Magic packet 
//               and wake-up frame can power on driver again.
// Parameter   : fgPwrDown - TRUE: power down; FALSE: power up
// Return      : None
// *********************************************************************
void vHalSetMacPwrDown(BOOL fgPwrDown)
{
    if(fgPwrDown)
    {
        SetBitMAC(RW_MAC_CFG, NIC_PD);               
    }
    else
    {
        ClrBitMAC(RW_MAC_CFG, NIC_PD);               
    }
}

// *********************************************************************
// Function    : fgHalIsPwrDownWakeup
// Description : Check whether wake up source is received
// Parameter   : None
// Return      : TRUE - wake-up source is received
//               FALSE - no wake-up source is received
// *********************************************************************
BOOL fgHalIsPwrDownWakeup(void)
{
    BOOL fgRet = FALSE;
    UINT32 u4PwrWakeup;

    u4PwrWakeup = (UINT32)dReadMAC(RW_DLY_INT_CFG);
    
    if(u4PwrWakeup & MAGIC_PKT_REC)
    {
        INFO_ETHER("Magic packet is received\n");
        fgRet = TRUE;
    }

    return (fgRet);
}

// *********************************************************************
// Function    : i4HalGetEtherStatInfo
// Description : The function is called to get ethernet RFC2665 statistic
// Parameter   : i4Cmd - statistic command
//               pvParam - pointer of parameter 
// Return      : 0 if success
//               negative if fail 
// *********************************************************************
INT32 i4HalGetEtherStatInfo(INT32 i4Cmd, VOID *pvParam)
{
    UINT32 *pu4Parm;
    INT32 i4Ret = 0;
    
    ASSERT(pvParam != NULL);
    pu4Parm = (UINT32 *)pvParam;	

    switch(i4Cmd)
    {
        case OID_ETHER_ALIGNMENT_ERRORS:
            *pu4Parm = 0;
        	break;
        case OID_ETHER_FCSERRORS:
            *pu4Parm = dReadMAC(RW_C_RXCRC);
        	break;
        case OID_ETHER_SINGLECOLLISIONFRAMES:
            *pu4Parm = 0;
        	break;
        case OID_ETHER_MULTIPLECOLLISIONFRAMES:
            *pu4Parm = 0;
        	break;
        case OID_ETHER_DEFERREDTRANSMISSIONS:
            *pu4Parm = 0;
        	break;
        case OID_ETHER_LATECOLLISIONS:
            *pu4Parm = 0;
        	break;
        case OID_ETHER_EXCESSIVECOLLISIONS:
            *pu4Parm = dReadMAC(RW_C_TXSKIP);
        	break;
        case OID_ETHER_INTERNALMACTRANSMITERRORS:
            *pu4Parm = 0;
        	break;
        case OID_ETHER_FRAMETOOLONGS:
            *pu4Parm = dReadMAC(RW_C_RXLONG);
        	break;
        case OID_ETHER_INTERNALMACRECEIVEERRORS:
            *pu4Parm = 0;
        	break;
        case OID_ETHER_SYMBOLERRORS:
            //*pu4Parm = dReadMMC();
            i4Ret = NET_DRV_INV_OID;
        	break;
        default:
            i4Ret = NET_DRV_INV_OID;
        	break;
    }

    return (i4Ret);
}

// *********************************************************************
// Function    : i4HalSetEtherStatInfo
// Description : The function is called to set ethernet RFC2665 statistic
// Parameter   : i4Cmd - statistic command
//               pvParam - pointer of parameter 
// Return      : 0 if success
//               negative if fail 
// *********************************************************************
INT32 i4HalSetEtherStatInfo(INT32 i4Cmd, VOID *pvParam)
{
    UNUSED(i4Cmd);
    UNUSED(pvParam);

    return(0);
#if 0 //MT8550
    UINT32 *pu4Parm;
    INT32 i4Ret = 0;
    
    ASSERT(pvParam != NULL);
    pu4Parm = (UINT32 *)pvParam;	

    switch(i4Cmd)
    {
        case OID_ETHER_ALIGNMENT_ERRORS:
            vWriteMMC(RW_MMC_RX_ALIGNMENT_ERR, *pu4Parm);
        	break;
        case OID_ETHER_FCSERRORS:
            vWriteMMC(RW_MMC_RX_CRC_ERR, *pu4Parm);
        	break;
        case OID_ETHER_SINGLECOLLISIONFRAMES:
            vWriteMMC(RW_MMC_TX_SINGLE_COL_G, *pu4Parm);
        	break;
        case OID_ETHER_MULTIPLECOLLISIONFRAMES:
            vWriteMMC(RW_MMC_TX_MULTI_COL_G, *pu4Parm);
        	break;
        case OID_ETHER_DEFERREDTRANSMISSIONS:
            vWriteMMC(RW_MMC_TX_DEFERED, *pu4Parm);
        	break;
        case OID_ETHER_LATECOLLISIONS:
            vWriteMMC(RW_MMC_TX_LATE_COL_ERR, *pu4Parm);
        	break;
        case OID_ETHER_EXCESSIVECOLLISIONS:
            vWriteMMC(RW_MMC_TX_EXESSIVE_COL_ERR, *pu4Parm);
        	break;
        case OID_ETHER_INTERNALMACTRANSMITERRORS:
            vWriteMMC(RW_MMC_TX_UNDERFLOW_ERR, *pu4Parm);
        	break;
        case OID_ETHER_FRAMETOOLONGS:
            vWriteMMC(RW_MMC_RX_OVERSIZE_G, *pu4Parm);
        	break;
        case OID_ETHER_INTERNALMACRECEIVEERRORS:
            vWriteMMC(RW_MMC_RX_OVERSIZE_G, *pu4Parm);
        	break;
        case OID_ETHER_SYMBOLERRORS:
            i4Ret = NET_DRV_INV_OID;
        	break;
        default:
            i4Ret = NET_DRV_INV_OID;
        	break;
    }

    return (i4Ret);
#endif
}

/*******************************************************************************
**  vRMiiInit
**
**  descriptions:
**      This routine is called to initial pinmux, clock source and phy reset pin
**  parameters:
**      none
**  result:
**      It will set pinmux, RMII clock source, phy reset signal release
**  return:
**      none
*******************************************************************************/
#if 0
void vRMiiInit(void)
{
	
    SetBitMAC(RW_EXT_CFG, RMII_MODE); //Set RMII mode
    SetBitMAC(RW_EXT_CFG, RMII_SRC_SEL_EXT_PAD); //RMII refer clock from PAD
    ClrBitMAC(RW_EXT_CFG, RMII_CLK_SEL); //RMII clock from TXCLK
    ClrBitMAC(RW_EXT_CFG, RMII_CLK_INV);
#if 0

    INT32 i4Data;

//#define MII_PHY_TXER						(18 + MX4)	// GPIO 142
        //BSP_PinSet(MII_PHY_TXER, PINMUX_FUNCTION0); //reset pin        

        i4Data = 0;        
        GPIO_Output(142, &i4Data); //output low
        i4Data = 1;
        GPIO_Enable(142, &i4Data); //set as output

#ifdef ETH_RMII_PHY
//        BSP_PinSet(MII_PHY_TXCLK, PINMUX_FUNCTION1);
        BSP_PinSet(MII_PHY_TXD0, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_TXD1, PINMUX_FUNCTION1);        
//        BSP_PinSet(MII_PHY_TXD2, PINMUX_FUNCTION1);        
//        BSP_PinSet(MII_PHY_TXD3, PINMUX_FUNCTION1);        
//        BSP_PinSet(MII_PHY_RXCLK, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_RXD0, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_RXD1, PINMUX_FUNCTION1);                
//        BSP_PinSet(MII_PHY_RXD2, PINMUX_FUNCTION1);                
//        BSP_PinSet(MII_PHY_RXD3, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_COL, PINMUX_FUNCTION1);                        
//        BSP_PinSet(MII_PHY_CRS, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_TXEN, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_RXDV, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_RXER, PINMUX_FUNCTION1);                        
        BSP_PinSet(MII_MDC_O, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_MDIO, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_REFCLK, PINMUX_FUNCTION1);                
#else
        /* config pin as ethernet mii pads (0x2000d404,0x2000d408), ref */
        BSP_PinSet(MII_PHY_TXCLK, PINMUX_FUNCTION1);
        BSP_PinSet(MII_PHY_TXD0, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_TXD1, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_TXD2, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_TXD3, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_RXCLK, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_RXD0, PINMUX_FUNCTION1);        
        BSP_PinSet(MII_PHY_RXD1, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_RXD2, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_RXD3, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_COL, PINMUX_FUNCTION1);                        
        BSP_PinSet(MII_PHY_CRS, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_TXEN, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_RXDV, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_RXER, PINMUX_FUNCTION1);                        
        BSP_PinSet(MII_MDC_O, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_MDIO, PINMUX_FUNCTION1);                
        BSP_PinSet(MII_PHY_REFCLK, PINMUX_FUNCTION1);                
#endif

        i4Data = 1;        
        GPIO_Output(142, &i4Data); //output high
//        CKGEN_WRITE32(0x28c, 0x04);
//        CKGEN_WRITE32(0x290, 0x05);
#ifdef ETH_RMII_PHY
        vCLKSRC_InitRMIIClock(FALSE, TRUE);
#else
        vCLKSRC_InitRMIIClock(TRUE, TRUE);
#endif
#endif
}
#endif

// *********************************************************************
// Function    : i4HalSyncDescIdx
// Description : The function is called to synchronize descriptor index 
//               with current host descriptor register R18 and R19.
//               The reason is that they will un-sync sometimes after 
//               cable is plugged out.
// Parameter   : prMac
// Return      : 0 if success
//               negative if fail 
// *********************************************************************
INT32 i4HalSyncDescIdx(PETHER_ADAPTER_T prMac)
{
    UINT32 u4TxIdx, u4RxIdx;
    
    if((dReadMAC(RW_TX_DPTR) < dReadMAC(RW_TX_BASE_ADDR)) ||
        (dReadMAC(RW_RX_DPTR) < dReadMAC(RW_RX_BASE_ADDR)))
    {
        ERR_ETHER("current host descriptor register error\n");
        return(NET_DRV_ERROR);
    }
    
    u4TxIdx = (dReadMAC(RW_TX_DPTR) - dReadMAC(RW_TX_BASE_ADDR)) / sizeof(MAC_TX_DESC_T);
    prMac->u4TxIdx = prMac->u4TxCompIdx = u4TxIdx;
    if(u4TxIdx != prMac->u4TxIdx)
    {
        INFO_ETHER("tx descriptor idx not sync, u4TxIdx=%d, reg=%d\n", prMac->u4TxIdx, u4TxIdx);
    }

    u4RxIdx = (dReadMAC(RW_RX_DPTR) - dReadMAC(RW_RX_BASE_ADDR)) / sizeof(MAC_RX_DESC_T);
    prMac->u4RxIdx = u4RxIdx;
    if(u4RxIdx != prMac->u4RxIdx)
    {
        INFO_ETHER("rx descriptor idx not sync, u4RxIdx=%d, reg=%d\n", prMac->u4RxIdx, u4RxIdx);
    }

    return(NET_DRV_OK);
}

// *********************************************************************
// Function    : vHalCalThroughput
// Description : The function is called to calculate ethernet driver throughput 
// Parameter   : prMac
// Return      : 0 if success
//               negative if fail 
// *********************************************************************
void vHalCalThroughput(PETHER_ADAPTER_T prMac)
{
    ETH_BIT_RATE_T *prBitRate;
    UINT32 u4RxOctetDelta, u4CurrRxOctet;
    UINT32 u4TxOctetDelta, u4CurrTxOctet;
    UINT32 u4MicorsSum;
    
    prBitRate = &prMac->rEthBitRate;
    
    HAL_GetTime(&prBitRate->rTime_End);
    HAL_GetDeltaTime(&prBitRate->rTime_Delta, &prBitRate->rTime_Start, &prBitRate->rTime_End);

    u4CurrRxOctet = dReadMAC(RW_C_RXOKBYTE);
    u4CurrTxOctet = dReadMAC(RW_C_TXOKBYTE);
    if(u4CurrRxOctet >= prBitRate->u4PrevRxOctet)
    {
        u4RxOctetDelta = u4CurrRxOctet - prBitRate->u4PrevRxOctet;
    }
    else
    {
        u4RxOctetDelta = (prBitRate->u4PrevRxOctet ^ 0xffffffff) + 1 + u4CurrRxOctet;
    }

    if(u4CurrTxOctet >= prBitRate->u4PrevTxOctet)
    {
        u4TxOctetDelta = u4CurrTxOctet - prBitRate->u4PrevTxOctet;
    }
    else
    {
        u4TxOctetDelta = (prBitRate->u4PrevTxOctet ^ 0xffffffff) + 1 + u4CurrTxOctet;
    }

    if(prBitRate->rTime_Delta.u4Seconds)
    {
        u4MicorsSum = (prBitRate->rTime_Delta.u4Seconds * 1000000) + prBitRate->rTime_Delta.u4Micros;
    }
    else
    {
        u4MicorsSum = prBitRate->rTime_Delta.u4Micros;
    }
    u4MicorsSum /= 1000; 
    if(u4MicorsSum)
    {
        prBitRate->u4RxBitRate = u4RxOctetDelta * 8 / (u4MicorsSum); // Kbps
        prBitRate->u4TxBitRate = u4TxOctetDelta * 8 / (u4MicorsSum); // Kbps
    }
    
    prBitRate->u4PrevRxOctet = u4CurrRxOctet;
    prBitRate->u4PrevTxOctet = u4CurrTxOctet;
    x_memcpy(&prBitRate->rTime_Start, &prBitRate->rTime_End, sizeof(prBitRate->rTime_Start));
}

// *********************************************************************
// Function    : void vMacInit(PETHER_ADAPTER_T prMac)
// Description : MAC block and PHY init
// Parameter   : prMac
// Return      : None
// *********************************************************************
void vMacInit(PETHER_ADAPTER_T prMac)
{
   #if (INTERNAL_PHY)
   UINT8 u1InterPhyAddr;

   UNUSED(u1InterPhyAddr);
//   UINT8 u4Data;
   #endif
	
    
    
      ASSERT(prMac != NULL);       


    //set IPLL (servi register)
    /* This IPLL setting is done by ATA already.*/
    #if (0)//(INTERNAL_PHY) //kenny temply for verification
    IO_WRITE32(0xF0024030, 0, 0x85); 
    IO_WRITE32(0xF0024034, 0, 0x01);
    IO_WRITE32(0xF005C704, 0, 0x801);
    u4Data = IO_READ32(0xF0090310, 0);
    u4Data = (u4Data&0xffffff00)|((2<<0)|(2 <<4)); 
    IO_WRITE32(0xF0090310, 0, u4Data);
    #endif


    //Initial Internal Phy
    #if (INTERNAL_PHY)
    
    u1InterPhyAddr = 0;//bit3~bit7
    #if 0
    u4Data= (dReadChkSum(RW_INTERNAL_PHY_CTRL)& (~INTERNAL_PHY_ADDRESS_MASK));
    vWriteChkSum(RW_INTERNAL_PHY_CTRL,u4Data|(u1InterPhyAddr <<INTERNAL_PHY_ADDRESS_POS));
    SetBitChkSum(RW_INTERNAL_PHY_CTRL, INTERNAL_PHY_ENABLE_MDC_MDIO);
    SetBitChkSum(RW_INTERNAL_PHY_CTRL, INTERNAL_PHY_ENABLE_MII_PIN);
    SetBitChkSum(RW_INTERNAL_PHY_CTRL1,SOFT_RESET_DISABLE);
    #endif
    
#ifdef CC_MT5396
    // select internal phy and release phy reset
    //SetBitMAC(RW_ETHSYS_CONFIG, INT_PHY_SEL);
    //SetBitMAC(RW_SW_RESET_CONTROL, PHY_RSTN);
    i4MacPhyWrite(0, 0x0, 0x8000);
    i4MacPhyWrite(0, 0x09, 0x0);
    ClrBitMAC(RW_PHY_CTRL1, AN_EN);
#else
    u4Data= (dReadMAC(RW_INTERNAL_PHY_CTRL)& (~INTERNAL_PHY_ADDRESS_MASK));
    vWriteMAC(RW_INTERNAL_PHY_CTRL,u4Data|(u1InterPhyAddr <<INTERNAL_PHY_ADDRESS_POS));
    SetBitMAC(RW_INTERNAL_PHY_CTRL, INTERNAL_PHY_ENABLE_MDC_MDIO);
    SetBitMAC(RW_INTERNAL_PHY_CTRL, INTERNAL_PHY_ENABLE_MII_PIN);
    SetBitMAC(RW_INTERNAL_PHY_CTRL1,SOFT_RESET_DISABLE);
#endif
      #endif
    // Init MAC and MII clock range  
/*
    vWriteMACPDWNC(RW_EXT_CFG, PDWNC_MAC_EXT_INIT);  //Little endian
	vWriteMAC(RW_EXT_CFG, MAC_EXT_INIT); //Little endian
    vWriteMACPDWNC(RW_CLK_CFG, MAC_CLK_INIT); 
	vWriteMAC(RW_CLK_CFG, MAC_CLK_INIT); 
*/
    #ifdef CC_MT5396
//    ClrBitMAC(MAC_MODE_CONFIG, BIG_ENDIAN);  //Little endian 2010/3/2
    #endif
	  
    /* Init MAC setting */
    vWriteMAC(RW_MAC_CFG, MAC_CFG_INIT);          
    SetBitMAC(RW_PHY_CTRL1, AUTO_POLL_DIS);         
      vHalSetMacAddr((MAC_ADDRESS_T *)MAC_ADDR);
    vWriteMAC(RW_ARL_CFG, MAC_FILTER_INIT);  
    

   // PHY software reset, it takes time  
//    i4Ret = i4MacPhyReset(prMac->u4PhyAddr);
//    if(i4Ret != NET_DRV_OK)
//    {
//        return;
//    }

      
    /* For first time to use the PHY */
    i4MacPhyAddrGet(prMac);
    vWriteMAC(RW_PHY_CTRL1, PHY_CTRL_INIT | (((prMac->u4PhyAddr) << PHY_ADDR_AUTO_SHIFT) & PHY_ADDR_AUTO));  
    i4MacIntPhyInit(u1InterPhyAddr);  //init internal PHY
#if 0    
    // Init PHY and check link status first 
    i4Ret = i4HalPhyStatusChk(prMac);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("PHY init fail\n");
    }
    
    if(!GET_ETHER_INFO(prMac, ETHER_LINK_UP))
    {
        WARN_ETHER("cable is disconnected\n");
    }
    else
    {
        //INFO_ETHER("cable is %s\n", prMii->fgLinkUp ? "connected" : "disconnected");
        INFO_ETHER("%s-duplex mode\n", prMii->fgFullDup ? "Full" : "Half");
        INFO_ETHER("1%s Mbps speed\n", prMii->fgSpeed100 ? "00" : "0");
    }
#endif
}


// =====================================================================
// MMC register block handling functions
// =====================================================================

// *********************************************************************
// Function    : void vMmcInit(void)
// Description : MMC block init
// Parameter   : None
// Return      : None
// *********************************************************************
void vMmcInit(void)
{
              
}


// =====================================================================
// DMA register block handling functions
// =====================================================================

// *********************************************************************
// Function    : void vDmaInit(void)
// Description : GMAC reset and DMA block init
// Parameter   : prMac
// Return      : None
// *********************************************************************

void vDmaInit(PETHER_ADAPTER_T prMac)
{

    ASSERT(prMac != NULL);
    vWriteMAC(RW_BURST_TYPE, DMA_BUSMODE_INIT);
    vWriteMAC(RW_FC_CFG, MAC_FLOWCTRL_INIT); 
    vWriteMAC(RW_EXTEND_CFG, SEND_PAUSE_RLS_1K); 
#if (CONFIG_DRV_LINUX_DATA_CONSISTENCY) 
    vWriteMAC(RW_RX_BASE_ADDR, PHYSICAL((UINT32)prMac->prRxDesc)); 	// Set RX Descriptor List Address
    vWriteMAC(RW_TX_BASE_ADDR, PHYSICAL((UINT32)prMac->prTxDesc)); 	// Set TX Descriptor List Address
    vWriteMAC(RW_RX_DPTR, PHYSICAL((UINT32)prMac->prRxDesc)); 		// Set RX Descriptor Pointer
    vWriteMAC(RW_TX_DPTR, PHYSICAL((UINT32)prMac->prTxDesc)); 		// Set TX Descriptor Pointer    
    #ifdef CC_MT5396
    vDmaTxStartAndResetTXDesc();
    vDmaRxStartAndResetRXDesc();
    #endif
#else   
    vWriteMAC(RW_RX_BASE_ADDR, (UINT32)prMac->prRxDesc); 	// Set RX Descriptor List Address
    vWriteMAC(RW_TX_BASE_ADDR, (UINT32)prMac->prTxDesc); 	// Set TX Descriptor List Address
    vWriteMAC(RW_RX_DPTR, (UINT32)prMac->prRxDesc); 		// Set RX Descriptor Pointer
    vWriteMAC(RW_TX_DPTR, (UINT32)prMac->prTxDesc); 		// Set TX Descriptor Pointer
#endif  

    vWriteMAC(RW_DMA_CFG, DMA_OPMODE_INIT);
    vDmaIntDisableAll();

}

// *********************************************************************
// Function    : void vDmaRxStart(void)
// Description : MAC RX start, data in RX fifo will enter DRAM buffer
// Parameter   : None
// Return      : None
// *********************************************************************
void vDmaRxStart(void)
{
    ClrBitMAC(RW_DMA_CFG, RX_SUSPEND);           	// Start Rx
}

// *********************************************************************
// Function    : void vDmaTxStart(void)
// Description : MAC TX start, data in DRAM buffer will sned to TX fifo
// Parameter   : None
// Return      : None
// *********************************************************************
void vDmaTxStart(void)
{
   	ClrBitMAC(RW_DMA_CFG, TX_SUSPEND);        	// Start Tx
}

// *********************************************************************
// Function    : void vDmaRxStop(PETHER_ADAPTER_T prMac)
// Description : MAC RX stop and clear descriptor
// Parameter   : prMac
// Return      : None
// *********************************************************************
void vDmaRxStop(PETHER_ADAPTER_T prMac)
{
    PMAC_RX_DESC_T prRxDesc;
    UINT32 i;
    
    ASSERT(prMac != NULL);
    
    SetBitMAC(RW_DMA_CFG, RX_SUSPEND);           	// Stop Rx
    for(i = 0; i < NUM_RX_DESC; i++ )
    {
        prRxDesc = prMac->prRxDesc + i; 
        if(prRxDesc->rRxCtrl.Bits.fgOwn == OWNED_BY_CPU)            // if descriptor is owned by DMA engine, take ownership
        {
            prRxDesc->rRxCtrl.Bits.u2BufSize = PKT_BUFFER_SIZE;
            prRxDesc->rRxCtrl.Bits.fgOwn = OWNED_BY_DMA;       		// clear DMA own bit 
        }
    }    
#if CONFIG_DRV_LINUX_DATA_CONSISTENCY
   	HalFlushInvalidateDCache(); 
#endif    
}

// *********************************************************************
// Function    : void vDmaTxStop(PETHER_ADAPTER_T prMac)
// Description : MAC TX stop and clear descriptor
// Parameter   : prMac
// Return      : None
// *********************************************************************
void vDmaTxStop(PETHER_ADAPTER_T prMac)
{
    PMAC_TX_DESC_T prTxDesc;
    UINT32 i;
    
    ASSERT(prMac != NULL);
    
    SetBitMAC(RW_DMA_CFG, TX_SUSPEND);             	// Stop Tx
    for(i = 0; i < NUM_TX_DESC; i++ )
    {
        prTxDesc = prMac->prTxDesc + i; 
        if(prTxDesc->rTxCtrl.Bits.fgOwn == OWNED_BY_DMA)            // if descriptor is owned by DMA engine, take ownership
        {
            prTxDesc->rTxCtrl.Bits.fgOwn = OWNED_BY_CPU;		       // clear DMA own bit 
        }
    }    
    Printf("!!!!vDmaTxStop \n");
#if CONFIG_DRV_LINUX_DATA_CONSISTENCY
   	HalFlushInvalidateDCache(); 
#endif 
}


/**********************************************************
 * DMA engine interrupt handling functions
 **********************************************************/
void vDmaIntEnableAll(void)
{
  vWriteMAC(RW_INT_MASK, ~DMA_INT_ENABLE_ALL);               	// enable interrupts
}

void vDmaIntDisableAll(void)
{
   vWriteMAC(RW_INT_MASK, DMA_INT_ENABLE_ALL);              	// disable interrupts
}

UINT32 u4DmaGetAndClrInt(UINT32 u4IntMask)
{
    UINT32 u4Status = dReadMAC(RW_DLY_INT_CFG);
    UINT32 u4Type;

    u4Type = u4Status & u4IntMask;
    vWriteMAC(RW_DLY_INT_CFG, u4Status );     /* clear all interrupt requests */

    return (u4Type);
}


