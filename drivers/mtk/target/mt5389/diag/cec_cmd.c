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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nptv_hdmicec_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
/** @file nptv_hdmicec_cmd.c
 *  This C file implements the HDMI CEC CLI test functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "cec_drv_if.h"
#include "cec_if.h"

#include "eeprom_if.h"
#include "x_pdwnc.h"

#include "x_stl_lib.h"
#include "x_os.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define CECTXNOACK     0x0001
#define CECRXOVERFLOW  0x0002
#define CECTOOLONGRXCOMMAND 0x0004
#define CECRXBUFFERSIZE      17
#define CEC_BASE          (IO_VIRT + 0x28500)

#define OPCODE_PING 0
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define CEC_READ32(offset)            IO_READ32(CEC_BASE, (offset))
#define CEC_WRITE32(offset, value) IO_WRITE32(CEC_BASE, (offset), (value))

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
EXTERN INT32 _PDWNC_SetupCEC(UINT32 u4XAddr, UINT32 u4Size, UINT8* pu1Data);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#ifndef NDEBUG
static CEC_FRAME_DESCRIPTION _rCECTxFrame;
static UINT32 _u4CECTxTag = 0;
static BOOL _fgCECTxFinish = FALSE;
static UINT32 _u4CECDstLogAddr = 0;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static void CecCli_Send(UINT8 u1Size, UINT8 u1Initiator, UINT8 u1Destination, 
    UINT8 u1Opcode, void *pvTag, UINT8 *pu1Data)
{
    UINT8 errcode;
    CEC_FRAME_DESCRIPTION *prFrame;
    UINT32 i;

    prFrame = &_rCECTxFrame;    
    // Polling next logical address.
    prFrame->size = u1Size;
    prFrame->sendidx = 0;
    prFrame->reTXcnt = 0;
    prFrame->txtag = pvTag;
    prFrame->blocks.header.initiator = u1Initiator; // TV
    prFrame->blocks.header.destination = u1Destination;

    if (u1Size > 1)
    {
        prFrame->blocks.opcode = u1Opcode;

        if ((u1Size > 2) && (u1Size <= 16))
        {
            for (i=0; i<(u1Size-2); i++)
            {
                prFrame->blocks.operand[i] = pu1Data[i];
            }
        }
    }

    do
    {
        errcode = CEC_EnqueueTx(prFrame);
        if (errcode)
        {
            // wait for a while and retry.
            x_thread_delay(100);

            Printf("CEC_EnqueueTx retry.\n");
        }
    }while(errcode);   

}

static void CecCli_TxResult(UINT8 result, void* txtag)
{
    Printf("Tx result=%d, txtag=0x%08X.\n", result, (UINT32)txtag);
    _fgCECTxFinish = TRUE;
}

static void CecCli_RxResult(CEC_FRAME_DESCRIPTION* frame)
{
    UINT32 i;
    
    Printf("Rx frame size=%d, txtag=0x%08X.\n", 
        frame->size, (UINT32)frame->txtag);
    Printf("Rx initiator=0x%X, destination=0x%X.\n", 
        frame->blocks.header.initiator, frame->blocks.header.destination);
    if (frame->size > 1)
    {
        Printf("Rx opcode=0x%X.\n", frame->blocks.opcode);

        if ((frame->size > 2) && (frame->size <= 16))
        {
            Printf("Rx operand= ");
            for (i=0;i<(frame->size-2); i++)
            {
                Printf("0x%02X, ", frame->blocks.operand[i]);
            }
            Printf("\n\n");
        }
    }
}

static INT32 CecCli_SetFrame(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 i;
    UINT8 operand[14];
    
    if (i4Argc < 3)
    {
        Printf("s [Initiator] [destination] [opcode] [operand 0 ~ 1].\n");
        return 0;        
    }    	

    // Set not finish. 
    _fgCECTxFinish = FALSE;
    
    _u4CECTxTag = 0;

    // Set tx result function.
    CEC_SetTxCallback(CecCli_TxResult);    

    if (i4Argc > 4)
    {
        for (i=0; i<(i4Argc-4); i++)
        {
            operand[i] = StrToInt(szArgv[4+i]);
        }
    }

    CecCli_Send((UINT8)(i4Argc -2), StrToInt(szArgv[1]), StrToInt(szArgv[2]), 
        StrToInt(szArgv[3]), (void*)(_u4CECTxTag ++), operand);    

    // Block to wait tx finish.
    while(!_fgCECTxFinish)
    {
        x_thread_delay(10);
    }

    return 0;
}

static INT32 CecCli_TxTestCmd(INT32 i4Argc, const CHAR **szArgv)    
{
    UINT8 u1Operand[14];
    UINT32 u4Dst;
    UINT32 i;
    UINT8 u1Size;

    // Set Rx result function.
    CEC_SetRxCallback(CecCli_RxResult);    

    // Set Tx result function.
    CEC_SetTxCallback(CecCli_TxResult);    

    for (i=0; i<14; i++)
    {
        u1Operand[i] = i;        
    }

    u4Dst = 15;
    Printf("Tx check destination=0x%X.\n", u4Dst);
    
    for (i=0; i<16; i++)
    {
        if (i == 0)
        {
            u1Size = 1;
            CecCli_Send(u1Size, CEC_LOG_TV, u4Dst, OPCODE_PING, 
                (void*)((UINT32)u1Size), NULL);
        }
        else
        {
            u1Size = i+1;
            CecCli_Send(u1Size, CEC_LOG_TV, u4Dst, OPCODE_IMAGE_VIEW_ON, 
                (void*)((UINT32)u1Size), u1Operand);
        }
    }

    u4Dst = 4;
    Printf("Tx check destination=0x%X.\n", u4Dst);
    
    for (i=0; i<16; i++)
    {
        if (i == 0)
        {
            u1Size = 1;
            CecCli_Send(u1Size, CEC_LOG_TV, u4Dst, OPCODE_PING, 
                (void*)((UINT32)u1Size), NULL);
        }
        else
        {
            u1Size = i+1;
            CecCli_Send(u1Size, CEC_LOG_TV, u4Dst, OPCODE_IMAGE_VIEW_ON, 
                (void*)((UINT32)u1Size), u1Operand);
        }
    }

    x_thread_delay(1000);

    return 0;
}

static INT32 CecCli_RxTestCmd(INT32 i4Argc, const CHAR **szArgv)    
{
    // Set Rx result function.
    CEC_SetRxCallback(CecCli_RxResult);    

    return 0;
}

static void CecCli_PingResult(UINT8 result, void* txtag)
{
    Printf("(%d) Ping Device=0x%X, result=%d.\n", _u4CECTxTag++, (UINT32)txtag , result);

    if ((_u4CECDstLogAddr+1) <= 15)
    {
        _u4CECDstLogAddr ++;

        CecCli_Send(1, CEC_LOG_TV, _u4CECDstLogAddr, OPCODE_PING, (void*)_u4CECDstLogAddr, NULL);
    }
    else
    {
        _u4CECDstLogAddr = 0;
    }
}

static INT32 CecCli_PingCmd(INT32 i4Argc, const CHAR **szArgv)    
{
    // Set tx result function.
    CEC_SetTxCallback(CecCli_PingResult);  

    _u4CECTxTag=0;

    CecCli_Send(1, CEC_LOG_TV, _u4CECDstLogAddr, OPCODE_PING, (void*)_u4CECDstLogAddr, NULL);    
    return 0;
}
#endif /* #ifndef NDEBUG */

static INT32 CecCli_QueryCmd(INT32 i4Argc, const CHAR **szArgv)    
{
    // Print CEC system information
    Printf("CEC TX FSM=0x%08X, FAIL=0x%08X, EVENT=0x%08X.\n", 
        TX_FSM_STATUS(), TX_FAIL_RECORD(), TX_EVENT_STATUS());
    Printf("CEC RX FSM=0x%08X, FAIL=0x%08X, EVENT=0x%08X.\n", 
        RX_FSM_STATUS(), RX_FAIL_RECORD(), RX_EVENT_STATUS());

    Printf("_CEC_Status=0x%X\n", _CEC_Status);

    if (IsCECStatus(STATE_TX_WAIT_DATA_TAKEN))
    {
        Printf("STATE_TX_WAIT_DATA_TAKEN\n");
    }

    if (IsCECStatus(STATE_TX_RESEND))
    {
        Printf("STATE_TX_RESEND\n");
    }

    if (IsCECStatus(STATE_TX_SEND_FRAME))
    {
        Printf("STATE_TX_SEND_FRAME\n");
    }

    if (IsCECStatus(STATE_TX_SEND_FRAME_OK))
    {
        Printf("STATE_TX_SEND_FRAME_OK\n");
    }

    if (IsCECStatus(STATE_TX_RESEND))
    {
        Printf("STATE_TX_RESEND\n");
    }

    if (IsCECStatus(STATE_RX_GET_NEW_HEADER))
    {
        Printf("STATE_RX_GET_NEW_HEADER\n");
    }

    if (IsCECStatus(STATE_RX_WAIT_COMPLETE))
    {
        Printf("STATE_RX_WAIT_COMPLETE\n");
    }

    // dump CEC register.
    CLI_Parser("r 0x20028500 0x100");
    return 0;
}

static INT32 CecCli_8032Cmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 au1Data[(256+32)];
    UINT32 i;
    UINT32 u4Offset;
    UINT8 u1Val;
    
    if (i4Argc >= 3)
    {
        u4Offset = StrToInt(szArgv[1]);        
        u1Val = (UINT8)StrToInt(szArgv[2]);
        
        if (u4Offset >= 0x120)
        {
            Printf("u4Offset error=0x%X.\n", u4Offset);
            return 0;
        }

        _PDWNC_SetupCEC(((XDATA_ADDR + CEC_XDATA_OFFSET)+u4Offset), 1, &u1Val);
    }    	

    (void)x_memset((void *)au1Data, 0, (256+32));
    
    // Get data from XDATA with 8032 address 0xFA00.
    /*
                    ARM address <----> 8032 address
        5387    0x4000                      0xF800
        Other    0x8000                      0xF800
    */
    (void)_PDWNC_GetT8302CECData(XDATA_ADDR + CEC_XDATA_OFFSET, (256+32), au1Data);

    for (i=0; i<(256+32)/16; i++)
    {
        Printf("0x%04X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", 
            (0xFA00+(i*16)), 
            au1Data[(i*16)+0], au1Data[(i*16)+1], au1Data[(i*16)+2], au1Data[(i*16)+3],
            au1Data[(i*16)+4], au1Data[(i*16)+5], au1Data[(i*16)+6], au1Data[(i*16)+7], 
            au1Data[(i*16)+8], au1Data[(i*16)+9], au1Data[(i*16)+10], au1Data[(i*16)+11], 
            au1Data[(i*16)+12], au1Data[(i*16)+13], au1Data[(i*16)+14], au1Data[(i*16)+15]);
    }

    return 0;
}

static INT32 CecCli_Init(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("CecCli_Init.\n");

    CEC_SetMWReady();
    
    CEC_Init();

    return 0;
}

static INT32 CecCli_Enable(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("CecCli_Enable = %d.\n", StrToInt(szArgv[1]));

    if (i4Argc >= 2)
    {
        if (StrToInt(szArgv[1]) >= 1)
        {
            CEC_SetEnable(TRUE);
        }
        else
        {
            CEC_SetEnable(FALSE);
        }
    }

    return 0;
}
#if defined (CC_HDMI_ARC_SPDIF_CTRL)|| defined (CC_HDMI_ARC_SPDIF_CTRL_89)


#if defined(CC_MT5396) ||defined(CC_MT5368) || defined(CC_MT5389)
static INT32 CecCli_Arcctrl(INT32 i4Argc, const CHAR **szArgv)
	{
		Printf("CecCli_Arc_Enable = %d.\n", StrToInt(szArgv[1]));
	
		if (i4Argc >= 2)
		{
			if (StrToInt(szArgv[1]) >= 1)
			{
				CEC_SetArcEnable(TRUE);
			}
			else
			{
				CEC_SetArcEnable(FALSE);
			}
		}
	
		return 0;
	}
#endif
#endif

static INT32 CecCli_Eeprom(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4LangAddr;
    UINT32 u4EnableAddr;
    UINT32 u4OneTouchAddr;
    UINT8 au1Data[4];
    UINT32 u4RxEnable = 0;
    UINT32 u4OneTouchEnable = 0;
    PDWNC_T8032_CMD_T rCmd;

    u4LangAddr = DRVCUST_InitGet(eEepromCECMenuLanguageOffset);
    u4EnableAddr = DRVCUST_InitGet(eEepromCECEnableOffset);
    u4OneTouchAddr = DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset);

    au1Data[0] = 0;
    au1Data[1] = 0;
    au1Data[2] = 0;
    au1Data[3] = 0;
    
    (void)EEPROM_Read(u4LangAddr, (UINT32)au1Data, 4);        
    Printf("MenuLang: EEP addr=%d, Value=%X, %X, %X, %X.\n", 
        u4LangAddr, au1Data[0], au1Data[1], au1Data[2], au1Data[3]);
    (void)EEPROM_Read(u4EnableAddr, (UINT32)&u4RxEnable, 1);
    Printf("Enable: EEP addr=%d, Value=%X.\n", u4EnableAddr, u4RxEnable);
    (void)EEPROM_Read(u4OneTouchAddr, (UINT32)&u4OneTouchEnable, 1);
    Printf("OneTouch: EEP addr=%d, Value=%X.\n", u4OneTouchAddr, u4OneTouchEnable);

    if (i4Argc < 3)
    {
        Printf("cec.eep [RxEnable = 1 or 0] [OneTouch = 1 or 0].\n");
    }
    else
    {
        u4RxEnable = StrToInt(szArgv[1]);        
        u4OneTouchEnable = StrToInt(szArgv[2]);     

        (void)EEPROM_Write(u4EnableAddr, (UINT32)&u4RxEnable, 1);
        (void)EEPROM_Read(u4EnableAddr, (UINT32)&u4RxEnable, 1);
        Printf("Enable: EEP addr=%d, Value=%X.\n", u4EnableAddr, u4RxEnable);

        (void)EEPROM_Write(u4OneTouchAddr, (UINT32)&u4OneTouchEnable, 1);
        (void)EEPROM_Read(u4OneTouchAddr, (UINT32)&u4OneTouchEnable, 1);
        Printf("OneTouch: EEP addr=%d, Value=%X.\n", u4OneTouchAddr, u4OneTouchEnable);
    }    	
   
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC;
    rCmd.u1SubCmd = 0;
    x_memcpy(rCmd.au1Data, &u4RxEnable, 1);
    PDWNC_T8032Cmd(&rCmd, NULL);

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
    rCmd.u1SubCmd = 0;    
    x_memcpy(rCmd.au1Data, &u4OneTouchEnable, 1);
    PDWNC_T8032Cmd(&rCmd, NULL);         

    return 0;    
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
CLIMOD_DEBUG_FUNCTIONS(CEC)

// Table for CEC command entry

static CLI_EXEC_T _arCecCmdTbl [] =
{
    CLIMOD_DEBUG_CLIENTRY(CEC),

#ifndef NDEBUG
    { "CEC Send", "s", CecCli_SetFrame, NULL, "HDMI CEC send frame", CLI_GUEST },
    { "CEC Ping", "ping", CecCli_PingCmd, NULL, "CEC ping device 0 ~ 15", CLI_GUEST},
    { "CEC Tx Test", "txtest", CecCli_TxTestCmd, NULL, "CEC tx test command", CLI_GUEST},
    { "CEC Rx Test", "rxtest", CecCli_RxTestCmd, NULL, "CEC rx test command", CLI_GUEST},
#endif
    { "CEC query", "q", CecCli_QueryCmd, NULL, "CEC query command", CLI_GUEST},
    { "CEC 8032", "8032", CecCli_8032Cmd, NULL, "CEC 8032 command", CLI_GUEST},
    { "CEC Initial", "i", CecCli_Init, NULL, "HDMI CEC register initial", CLI_GUEST },
    { "CEC Enable", "e", CecCli_Enable, NULL, "HDMI CEC Enable/disable", CLI_GUEST },
    { "CEC Eeprom", "eep", CecCli_Eeprom, NULL, "HDMI CEC eeprom/power down", CLI_GUEST },
//arc control
#if defined (CC_HDMI_ARC_SPDIF_CTRL)|| defined (CC_HDMI_ARC_SPDIF_CTRL_89)
#if defined(CC_MT5396) ||defined(CC_MT5368) || defined(CC_MT5389)
    { "CEC Arc Enable", "arc", CecCli_Arcctrl, NULL, "HDMI CEC arc Enable/disable", CLI_GUEST },
#endif
#endif

    { NULL, NULL, NULL, NULL, NULL, CLI_GUEST }
};

CLI_MAIN_COMMAND_ITEM(Cec)
{
    "cec", NULL, NULL, _arCecCmdTbl, "HDMI CEC test", CLI_GUEST
};

