/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*****************************************************************************/

/**
 * @file HDMI customization file
 *
 *
 * @author chiachi_chiang@mtk.com.tw
 * @author darren_weng@mtk.com.tw
 * @author daniel_hsiao@mtk.com.tw
 */
//Change the following definition for gpio customization
#include "hw_vdoin.h"
#include "sv_const.h"
#include "source_table.h"
#include "drv_video.h"
#include "drv_hdmi.h"
#include "x_printf.h"
#include "sif_if.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "x_timer.h"

void vHDMIHPDHigh(UINT8 bSwitch)
{
}

void vHDMIHPDLow(UINT8 bSwitch)
{
}

UINT8 bHDMIPort5VStatus(UINT8 port)
{
	return 0;
}

/* 
@brief : When TV power on, FW will call this function once. 
*/

void vHDMISwitchInit(void)
{

}


/*
@brief :  FW will call this function to  change switch port 
@param  : bSwitch =  1 , 2 ,3 , 4     
*/

void vHDMISetSwitch(UINT8 bSwitch)
{

}

/* 
@brief : Get HDCP authentication status of different HDMI port
@param: port  = 1 , 2, 3, 4
*/
UINT8 bHDMISwitchHDCPStatus(UINT8 port) 
{
    return 1;
}

/* 
@brief : Get 5V status of different HDMI port
@param: port  = 1 , 2, 3, 4
*/
UINT8 bHDMISwitch5VStatus(UINT8 port)
{
    return 1;
}


/* 
@brief : Get TMDS CLK  status of different HDMI port
@param: port  = 1 , 2, 3, 4
*/
UINT8 bHDMISwitchCLKStatus(UINT8 port)
{
    return 1;
}

/* 
@brief : Get SCDT status 
*/
UINT8 bHDMISwitchSCDTStatus(void)
{
    return 1;
}

/* 
@brief : Get HPD status of different HDMI port
@param: port  = 1 , 2, 3, 4
*/
UINT8 bHDMISwitchHPDStatus(UINT8 port)
{
    return 1;
}

/* 
@brief : reset switch
*/
void bHDMISwitchReset(void)
{
}


