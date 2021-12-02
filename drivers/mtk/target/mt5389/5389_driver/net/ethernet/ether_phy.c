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
 *   $Workfile: ether_phy.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet PHY code
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

#define _PHY_

#include "ether_com.h"
#include "ether_drv.h"
#include "x_pinmux.h"
/****************************************************************************
** Constant definitions
****************************************************************************/
#define PHY_ID_NUM_LOW_WORD     0x5AA5


// *********************************************************************
// Function    : i4MacPhyRead
// Description : MII PHY register read
// Parameter   : u4PhyAddr - phy device address
//               u4RegAddr - phy register offset
//               pu4Data - output read data
// Return      : 0 if success
// *********************************************************************
INT32 i4MacPhyRead(UINT32 u4PhyAddr, UINT32 u4RegAddr, UINT32 *pu4Data)
{
    UINT32 u4Data, u4TimeOut;

    ASSERT(pu4Data != NULL);
    
   u4Data = 0;
   u4Data = (u4PhyAddr << PHY_ADDR_SHIFT) & PHY_ADDR;
   u4Data |= ((u4RegAddr << PHY_REG_SHIFT) & PHY_REG);
   u4Data |= RD_CMD;
   vWriteMAC(RW_PHY_CTRL0, u4Data);

    u4TimeOut = 0xffff;
    while((dReadMAC(RW_PHY_CTRL0) & RW_OK) == 0) // Wait for R/W command complete 
    { 
        u4TimeOut --;
        if(u4TimeOut == 0x00)
        {
            WARN_ETHER("MII read PHY timeout\n");
            return(NET_DRV_HW_ERROR);
        }
    }
  
     *pu4Data = (UINT32)(dReadMAC(RW_PHY_CTRL0) >> RW_DATA_SHIFT);        // Get PHY data
     SetBitMAC(RW_PHY_CTRL0, RW_OK);  //clear RW_OK

    return (NET_DRV_OK);          
}

// *********************************************************************
// Function    : void i4MacPhyWrite(UINT32 u4PhyAddr, UINT32 u4RegAddr, UINT32 u4RegData)
// Description : MII PHY register write
// Parameter   : u4PhyAddr, u4RegAddr, u4RegData
// Return      : none
// *********************************************************************
INT32 i4MacPhyWrite(UINT32 u4PhyAddr, UINT32 u4RegAddr, UINT32 u4RegData)
{
    UINT32 u4Data, u4TimeOut;
  
   u4Data = (u4RegData & 0xFFFF) << RW_DATA_SHIFT;
   u4Data |= ((u4PhyAddr << PHY_ADDR_SHIFT) & PHY_ADDR);
   u4Data |= ((u4RegAddr << PHY_REG_SHIFT) & PHY_REG);
   u4Data |= WT_CMD;
  
   vWriteMAC(RW_PHY_CTRL0, u4Data);
  
    u4TimeOut = 0xffff;
    while((dReadMAC(RW_PHY_CTRL0) & RW_OK) == 0) // Wait for R/W command complete 
    { 
        u4TimeOut --;
        if(u4TimeOut == 0x00)
        {
            WARN_ETHER("MII write PHY timeout\n");
            return(NET_DRV_HW_ERROR);
        }
    }
    SetBitMAC(RW_PHY_CTRL0, RW_OK);  //clear RW_OK
  
    return(NET_DRV_OK);
}

// *********************************************************************
// Function    : void i4MacPhyReset(UINT32 u4PhyAddr)
// Description : PHY device SW reset
// Parameter   : u4PhyAddr
// Return      : none
// *********************************************************************
INT32 i4MacPhyReset(UINT32 u4PhyAddr)
{
    INT32 i4Ret;
    
    i4Ret = i4MacPhyWrite(u4PhyAddr, MII_PHY_CTRL_REG, SW_RESET);
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("ethernet PHY reset fail\n");
    }

    return(i4Ret);
}

#if (INTERNAL_PHY)
// *********************************************************************
// Function    : void i4MacIntPhyInit(UINT32 u4PhyAddr)
// Description : PHY device SW init
// Parameter   : u4PhyAddr
// Return      : none
// *********************************************************************
INT32 i4MacIntPhyInit(UINT32 u4PhyAddr)
{
    INT32 i4Ret = 0;

#if 0
    //test page
    i4MacPhyWrite(u4PhyAddr, 0x1F, 0x2a30); 
    i4MacPhyWrite(u4PhyAddr, 0x03, 0x000c); //10M power saving
    i4MacPhyWrite(u4PhyAddr, 0x08, 0x0211); //disable gating of RCLK125 according to YT's suggest
    i4MacPhyWrite(u4PhyAddr, 0x0E, 0x35); //analog
    i4MacPhyWrite(u4PhyAddr, 0x18, 0xE402);  //analog
    #if 0 //(CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)
    i4MacPhyRead(u4PhyAddr, 0x0A, &u4Data);  //ADC clock INvert
    i4MacPhyWrite(u4PhyAddr, 0x0A, u4Data|(1<<13));
     
    #endif 
    //extend page
    i4MacPhyWrite(u4PhyAddr, 0x1F, 0x01);   
    i4MacPhyWrite(u4PhyAddr, 0x1b, 0x1a0); //enable auto-crossover as autoneg is disabled

    //main page
    i4MacPhyWrite(u4PhyAddr, 0x1F, 0x00);   
    i4MacPhyWrite(u4PhyAddr, 0x17, 0x0020); //ACTI_EN for power save when link down
    i4MacPhyWrite(u4PhyAddr, 0x18, 0x1f00); //phy control
    
    i4Ret = i4MacPhyWrite(u4PhyAddr, 0x1F, 0x0); //return to main page.
    if(i4Ret != NET_DRV_OK)
    {
        ERR_ETHER("ethernet internal PHY init fail\n");
    }
#endif

    /* config ethernet LED */
    //BSP_PinSet(PIN_GPIO0, PINMUX_FUNCTION6);
    //BSP_PinSet(PIN_GPIO1, PINMUX_FUNCTION6);
    // set LED
    i4MacPhyWrite(u4PhyAddr, 0x1F, 0x0002);  // misc page
    i4MacPhyWrite(u4PhyAddr, 0x00, 0x80f0);  // EXT_CTL([15]) = 1
    i4MacPhyWrite(u4PhyAddr, 0x03, 0xc000);  // LED0 on clean
    i4MacPhyWrite(u4PhyAddr, 0x04, 0x003f);  // LED0 blink for 10/100/1000 tx/rx
    i4MacPhyWrite(u4PhyAddr, 0x05, 0xc007);  // LED1 on for 10/100/1000M linkup
    i4MacPhyWrite(u4PhyAddr, 0x06, 0x0000);  // LED1 blink clean
    i4MacPhyWrite(u4PhyAddr, 0x1F, 0x2a30);  // test page
    i4MacPhyWrite(u4PhyAddr, 0x05, 0x1010);  //force MDI crossover
    i4MacPhyWrite(u4PhyAddr, 0x1F, 0x0000);  // main page

    UNUSED(i4Ret);
    return(i4Ret);
}
#endif

// *********************************************************************
// Function    : void i4MacPhyFullDup(UINT32 u4PhyAddr, BOOL fgFullDup)
// Description : Set PHY duplex mode
// Parameter   : fgFullDup, TRUE-> Full duplex, FALSE-> half duplex
// Return      : none
// *********************************************************************
INT32 i4MacPhyFullDup(UINT32 u4PhyAddr, BOOL fgFullDup)
{
    UINT32 u4Data;
    INT32 i4Ret;
    
    i4Ret = i4MacPhyRead(u4PhyAddr, MII_PHY_CTRL_REG, &u4Data);
    if(i4Ret == NET_DRV_OK)
    {
        u4Data &= (~AN_ENABLE);
    }
    else
    {
        return(i4Ret);
    }
    
    if(fgFullDup)
    {
        u4Data |= DUPLEX_FULL;
    }
    else
    {
        u4Data &= ~DUPLEX_FULL;
    }
    	
    i4Ret = i4MacPhyWrite(u4PhyAddr, MII_PHY_CTRL_REG, u4Data);
    
    return(i4Ret);
}

// *********************************************************************
// Function    : void i4MacPhyLoopback(UINT32 u4PhyAddr, BOOL fgLoopback)
// Description : Set PHY loopback mode
// Parameter   : fgLoopback, TRUE-> loopback mode, FALSE-> normal mode
// Return      : none
// *********************************************************************
INT32 i4MacPhyLoopback(UINT32 u4PhyAddr, BOOL fgLoopback)
{
    UINT32 u4Data;
    INT32 i4Ret;
    	
    i4Ret = i4MacPhyRead(u4PhyAddr, MII_PHY_CTRL_REG, &u4Data);
    if(i4Ret != NET_DRV_OK)
    {
        return(i4Ret);
    }
    
    if(fgLoopback)
    {
        u4Data |= LOOKBACK_EN;
    }
    else
    {
        u4Data &= ~LOOKBACK_EN;
    }
    	
    i4Ret = i4MacPhyWrite(u4PhyAddr, MII_PHY_CTRL_REG, u4Data);

    return(i4Ret);
}

// *********************************************************************
// Function    : i4MacPhyAddrGet
// Description : Get PHY physical address by r/w register result
// Parameter   : prMac
// Return      : 0 on success, others on failure
// *********************************************************************
INT32 i4MacPhyAddrGet(PETHER_ADAPTER_T prMac)
{
   #if (!INTERNAL_PHY) //kenny mark it
    UINT32 u4Data, u4PhyAddr;
    UINT32 u4DataTmp;
   #endif

    ASSERT(prMac != NULL);
    #if (!INTERNAL_PHY) //kenny mark it
    for(u4PhyAddr = 0; u4PhyAddr < 32; u4PhyAddr++)
    {
        // MII_PHY_ID0_REG can be r/w on SMSC8700, but read only for RTL8201
        i4MacPhyRead(u4PhyAddr, MII_PHY_ID0_REG, &u4DataTmp);
        i4MacPhyWrite(u4PhyAddr, MII_PHY_ID0_REG, PHY_ID_NUM_LOW_WORD);
        i4MacPhyRead(u4PhyAddr, MII_PHY_ID0_REG, &u4Data);
        if(u4Data == PHY_ID_NUM_LOW_WORD)
        {
            prMac->u4PhyAddr = u4PhyAddr;
            INFO_ETHER("PHY MII address: 0x%02x\n", u4PhyAddr);
            i4MacPhyWrite(u4PhyAddr, MII_PHY_ID0_REG, u4DataTmp);
            return (NET_DRV_OK);
        }
    }
    #else
    prMac->u4PhyAddr = 0;//kenny temply 
    #endif
    //INFO_ETHER("Get PHY MII address fail\n");
    return (NET_DRV_ERROR);          
}

// *********************************************************************
// Function    : i4MacPhyNegotitate
// Description : MAC PHY link status check and negotiation
// Parameter   : prMac
// Return      : NET_DRV_OK - MII access PHY register OK
//             : negative - MII access PHY register fail
// *********************************************************************
INT32 i4MacPhyNegotitate(PETHER_ADAPTER_T prMac)
{
    UINT32 u4PhyAddr;
    UINT32 u4CtrlReg0, u4StatusReg1; 
    UINT32 u4AnlparReg5, u4AnerReg6; 
    PMAC_MII_T prMii;
    INT32 i4Ret;

    ASSERT(prMac != NULL);
    
    prMii = &prMac->rMii;
    prMii->fgSpeed1000 = FALSE;

    u4PhyAddr = prMac->u4PhyAddr;
    i4Ret = i4MacPhyRead(u4PhyAddr, MII_PHY_CTRL_REG, &u4CtrlReg0);
    if(i4Ret != NET_DRV_OK)
    {
        WARN_ETHER("MII access PHY register fail\n");
        return(i4Ret);
    }
    i4Ret = i4MacPhyRead(u4PhyAddr, MII_PHY_STATUS_REG, &u4StatusReg1);

#if 0
    if ((u4StatusReg1 & LINK) == 0)
    {
		NOTE_ETHER("Link Down \n");
		Printf("0x%04x  0x%04x \n", u4CtrlReg0,u4StatusReg1);
    }
#endif
    
    if(i4Ret != NET_DRV_OK)
    {
        WARN_ETHER("MII access PHY register fail\n");
        return(i4Ret);
    }
    i4Ret = i4MacPhyRead(u4PhyAddr, MII_PHY_ANER_REG, &u4AnerReg6);
    if(i4Ret != NET_DRV_OK)
    {
        WARN_ETHER("MII access PHY register fail\n");
        return(i4Ret);
    }

    prMii->fgLinkUp = (u4StatusReg1 & LINK) ? TRUE : FALSE;
    prMii->fgLpNwAble = (u4AnerReg6 & 0x01) ? TRUE : FALSE;

    if(u4CtrlReg0 & AN_ENABLE)
    {
        /* auto-negotiation is enabled and completed */
	    if(prMii->fgLinkUp)
	    {
	        /* cable is conntected and linked up */
	            if(u4StatusReg1 & AN_COMPLT)
	            {
	        		SET_ETHER_INFO(prMac, ETHER_LINK_UP);
	            }	
	            else
	            {
	                /* From experiment, this case is never occurred on RTL8201.
	                   It means that once mode is changed(may be speed, deplex or plug in/out),
	                   PHY status will be linked down, i.e. prMii->fgLinkUp = 0 */
	                DBG_ETHER("auto-negotiation is not completed\n");
	            }
	    }
	    else
	    {
	        /* cable is not conntected, no need to check other status */
	        CLR_ETHER_INFO(prMac, ETHER_LINK_UP);
	    	return(NET_DRV_OK);
		}
    }
    else
   	{
        /* auto negotiation is disabled */
        if(prMii->fgLinkUp)
      	{
            /* cable is conntected and linked up */
            SET_ETHER_INFO(prMac, ETHER_LINK_UP);
    	}
        else
    	{
            /* cable is not conntected, no need to check other status */
            CLR_ETHER_INFO(prMac, ETHER_LINK_UP);
            return(NET_DRV_OK);
      	}
  	}

	/* check speed and deplex mode */
    i4Ret = i4MacPhyRead(u4PhyAddr, MII_PHY_ANLPAR_REG, &u4AnlparReg5);
    if(i4Ret != NET_DRV_OK)
  	{
        WARN_ETHER("MII access PHY register fail\n");
        return(i4Ret);
    }
    if(u4AnlparReg5 & (PARTNER_100BASE | PARTNER_100BASE_FULL))
    {
        prMii->fgSpeed100 = TRUE;
        prMii->fgFullDup = (u4AnlparReg5 & PARTNER_100BASE_FULL) ? TRUE : FALSE;
    }
    else
    {
        prMii->fgSpeed100 = FALSE;
        prMii->fgFullDup = (u4AnlparReg5 & PARTNER_10BASE_FULL) ? TRUE : FALSE;
    }

    return (NET_DRV_OK);
}

