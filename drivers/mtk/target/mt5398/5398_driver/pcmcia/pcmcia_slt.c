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
/*-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pcmcia_slt.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
 
#ifndef CC_MTK_LOADER
 //-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_timer.h"
#include "x_drv_cli.h"
#include "dmx_if.h"
#include "x_ckgen.h"
#include "pi_dvbt_if.h"
LINT_EXT_HEADER_END

#include "pcmcia_if.h"
#include "pcmcia_ctrl.h"
#include "pcmcia_hw.h"

#include "x_tuner.h"
#include "tuner_if.h"

#ifdef PCMCIA_ENABLED

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define CC_PCMCIA_SLT_SEND_TS_ONLY

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T hMT8295DemoThread = (HANDLE_T)NULL;
static pcmcia_cam_connectivity_nfy pfnCamConnectivityCbk = NULL;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
void _PCMCIA_SetTsFromDemodToCam(void)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        CKGEN_WRITE32( 0x24c, ((CKGEN_READ32(0x24c)&0xFCFFFFFF)|0x01000000));
        PCMCIA_EnableTS(TRUE);
        PCMCIA_SetExtDemodOnOff(FALSE);
        LOG(0, "    Set d.sfe 3\n");
        DMX_SetFrontEnd((DMX_FRONTEND_T)3);
    }
    else
    {
        TunerSetTsSP(SP_PARALLEL);
        DMX_SetFrontEnd((DMX_FRONTEND_T)9);
    }

    if ( pfnCamConnectivityCbk != NULL )
    {
        pfnCamConnectivityCbk(TRUE);
    }
}

void _PCMCIA_SetTsFromDemodToDmx(void)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        PCMCIA_EnableTS(FALSE);
        PCMCIA_SetExtDemodOnOff(FALSE);
        LOG(0, "    Set d.sfe 1\n");
        DMX_SetFrontEnd((DMX_FRONTEND_T)1);
    }
    else
    {
        TunerSetTsSP(SP_SERIAL);   
        DMX_SetFrontEnd((DMX_FRONTEND_T)1);
    }

    if ( pfnCamConnectivityCbk != NULL )
    {
        pfnCamConnectivityCbk(FALSE);
    }
}

void _PCMCIA_DemoThread(void * pvArg)
{
    UINT32 dwCamInserted=FALSE;
    while(TRUE)
    {
        if ( PCMCIA_DetectCableCard() ) // CI CAM inserted.
        {
            if ( dwCamInserted == FALSE )
            {
                LOG(0, "CAM Inserted\n");
                _PCMCIA_SetTsFromDemodToCam();
            }
            dwCamInserted = TRUE;
        }
        else    // CI CAM not inserted.
        {
            if ( dwCamInserted == TRUE )
            {
                LOG(0, "CAM Not Inserted\n");
                _PCMCIA_SetTsFromDemodToDmx();
            }
            dwCamInserted = FALSE;
        }
        x_thread_delay(1000);
    }
}



void PCMCIA_DemoStart(pcmcia_cam_connectivity_nfy pfnCbk)
{
    CLI_Parser("p.i");

    pfnCamConnectivityCbk = pfnCbk;
    
    if ( hMT8295DemoThread == (HANDLE_T)NULL )
    {
        VERIFY( x_thread_create(&hMT8295DemoThread,
                            "MT8295_DemoThread",
                            1024,   //stack size
                            128,    //priority
                            _PCMCIA_DemoThread,
                            0,
                           NULL) == OSR_OK);
   }
}

void PCMCIA_SltStart(UINT8 u1TsIndex, UINT8 u1TsClkRate, UINT8 u1TsDataWidth)
{
#ifdef __MODEL_slt__
    UINT32 u4Reg;    
    DMX_TSOUT_DATAWIDTH_T eTsDataWidth;

    CLI_Parser("d.i");
#ifndef CC_PCMCIA_SLT_SEND_TS_ONLY
    CLI_Parser("p.i");

    if ( CLI_Parser("p.v.tc") != 0 )
    {
        LOG(0, "p.v.tc Failed !\n");
        goto PCMCIA_SLT_FAILED;
    }
#endif

    // DMX Set Route
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
//        PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET3, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET3) | 0xC000);
        
        if ( DMX_TsOut_SetRoute(u1TsIndex, DMX_TSOUT_ROUTE_EXTERNAL_CI) != TRUE )
        {
            LOG(0, "DMX_TsOut_SetRoute Failed !\n");
            goto PCMCIA_SLT_FAILED;
        }
    }
    else
    {
        if ( DMX_TsOut_SetRoute(u1TsIndex, DMX_TSOUT_ROUTE_INTERNAL_CI) != TRUE )
        {
            LOG(0, "DMX_TsOut_SetRoute Failed !\n");
            goto PCMCIA_SLT_FAILED;
        }
    }

    // DMX Set Endian
    if ( DMX_TsOut_SetEndianess(DMX_TSOUT_ENDIAN_MSB) != TRUE )
    {
        LOG(0, "DMX_TsOut_SetEndianess Failed !\n");
        goto PCMCIA_SLT_FAILED;
    }

    // DMX Set Sync Cycle
    if ( DMX_TsOut_SetSyncCycle(DMX_TSOUT_SYNC_CYCLE_8) != TRUE )
    {
        LOG(0, "DMX_TsOut_SetSyncCycle Failed !\n");
        goto PCMCIA_SLT_FAILED;
    }

    // DMX Set Valid Cycle
    if ( DMX_TsOut_SetValidCycle(0) != TRUE )
    {
        LOG(0, "DMX_TsOut_SetValidCycle Failed !\n");
        goto PCMCIA_SLT_FAILED;
    }

    // DMX Set Latch Edge
    if ( DMX_TsOut_SetLatchEdge(DMX_TSOUT_LATCH_EDGE_NEGATIVE) != TRUE )
    {
        LOG(0, "DMX_TsOut_SetLatchEdge Failed !\n");
        goto PCMCIA_SLT_FAILED;
    }

    // DMX Set Data Width
    switch ( u1TsDataWidth )
    {
        case 1: 
            u1TsDataWidth = DMX_TSOUT_DATAWIDTH_1;
            break;
        case 2: 
            u1TsDataWidth = DMX_TSOUT_DATAWIDTH_2;
            break;
        case 8: 
            u1TsDataWidth = DMX_TSOUT_DATAWIDTH_8;
            break;
        default: 
            LOG(0, "u1TsDataWidth should be 1 or 2 or 8 !\n");
            return;
    }
    if ( DMX_TsIn_SetDataWidth(u1TsIndex, eTsDataWidth) != TRUE )
    {
        LOG(0, "DMX_TsIn_SetDataWidth Failed !\n");
        goto PCMCIA_SLT_FAILED;
    }
    if ( DMX_TsOut_SetDataWidth(eTsDataWidth) != TRUE )
    {
        LOG(0, "DMX_TsOut_SetDataWidth Failed !\n");
        goto PCMCIA_SLT_FAILED;
    }

    // 0x2000d214 [2:0]        (MT5363)
    //000: xtal_ck      (54Mhz)(MT5363)
    //001: tvdpll_d9_ck (60MHz)(MT5363)
    //010: cpupll_d6_ck (66MHz)(MT5363)
    //011: demod54_ck   (54Mhz)(MT5363)
    //100: cpupll_d9_ck (45Mhz)(MT5363)
    //101: xtal_d4_ck   (13.5Mhz)(MT5363)
    //110: xtal_d8_ck   (6.75Mhz)(MT5363)
    //111: cpupll_d10_ck(40.5Mhz)(MT5363)
    // Update _DMX_PVRPlay_SetTSOut for this setting
    
    // MT5365
    //0000: xtal_ck (27Mhz)
    //0001: sawlesspll_d4_ck (108Mhz)
    //0010: sawlesspll_d6_ck (72Mhz)
    //0011: sawlesspll_d8_ck (54Mhz)
    //0100: sawlesspll_d9_ck (48Mhz)
    //0101: usbpll_d6_ck (80Mhz)
    //0110: usbpll_d8_ck (60Mhz)
    //0111: usbpll_d16_ck (30Mhz)
    //1000: sawlesspll_d16_ck (27Mhz)
    //1001: syspll_d8_ck (57.3Mhz)
    // Update _DMX_PVRPlay_SetPort for this setting
    u4Reg = CKGEN_READ32(0x214);
    u4Reg &= ~0x8F;
    switch ( u1TsClkRate )
    {
        case 30 : 
            u4Reg |= 0x7;
            LOG(0, "TS CLK Rate : 30Mhz\n");
            break;
        case 48 : 
            u4Reg |= 0x4;
            LOG(0, "TS CLK Rate : 48Mhz\n");
            break;
        case 54 : 
            u4Reg |= 0x3;
            LOG(0, "TS CLK Rate : 54Mhz\n");
            break;
        case 60 : 
            u4Reg |= 0x6;
            LOG(0, "TS CLK Rate : 60Mhz\n");
            break;
        case 72 : 
            u4Reg |= 0x2;
            LOG(0, "TS CLK Rate : 72Mhz\n");
            break;
        case 80 : 
            u4Reg |= 0x5;
            LOG(0, "TS CLK Rate : 80Mhz\n");
            break;
        case 108 : 
            u4Reg |= 0x1;
            LOG(0, "TS CLK Rate : 108Mhz\n");
            break;
        case 27 :
        default :
            LOG(0, "TS CLK Rate : 27Mhz\n");
            u4Reg |= 0x0;
            break;
    }
    CKGEN_WRITE32(0x214, u4Reg);
    
    if ( DMX_TsOut_TransmitData(u1TsIndex, 1024*1024) != TRUE )
    {
        LOG(0, "DMX_TsOut_TransmitData Failed !\n");
        goto PCMCIA_SLT_FAILED;
    }
    
    LOG(0, "SLT_Result:Pass\n");
    return;

PCMCIA_SLT_FAILED:
    LOG(0, "SLT_Result:Failed\n");
    return;
    
#endif
}

#endif
#endif
