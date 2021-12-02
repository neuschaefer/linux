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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: uart_modem.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file uart_modem.c
 *  Brief of file uart_modem.c.
 *  Details of file uart_modem.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "uart_drv.h"

//#ifdef UART_MODEM
#if 0

#include "x_serial.h"
#include "x_os.h"
#include "x_bim.h"
#include "uart_hw_drvif.h"
#include "x_assert.h"
#include "x_printf.h"
#include "uart_debug.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define TEST_CNT 1024
#define TEST_PORT UART_PORT_1
#define MODEM_PORT UART_PORT_1

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

/*
static char * _pucMInit_1 = "AT&F&C1E0&D2\r\0";
static char * _pucMInit_2 = "ATL2M1S64=13\r\0";
static char * _pucMInit_3 = "AT+FCLASS=0\r\0";
static char * _pucMInit_4 = "ATV1S2=128X1\r\0";
static char * _pucMInit_5 = "AT\\V8\\N3\r\0";
static char * _pucMInit_6 = "ATL0M0&G0%E1\r\0";
static char * _pucMInit_7 = "AT%L1%M0%C3\r\0";
*/

static const CHAR * _aszMInit[] = {"AT&F&C1E0&D2\r\0",
	                         "ATL2M1S64=13\r\0",
	                         "AT+FCLASS=0\r\0" };
static void _ModemNotify(VOID* pv_nfy_tag, RS_232_COND_T  e_nfy_cond, UINT32 ui4_data);	
static BOOL _fgModemInit = FALSE;
static RS_232_NFY_INFO_T _rRs232ModemNotify = {NULL, _ModemNotify};
static UINT8 _aucBuf[TEST_CNT];
//static UINT8 _aucTestStr[TEST_CNT];
//static UINT8 _aucReadStr[TEST_CNT];
//static INT32 i4ReadCnt = 0;
//static INT32 i4WriteCnt = 0;

//static HANDLE_T _hWaitSema;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static void _pfRs232ModemStatusChgNotify()
{
    UINT32 u4ModemStatus;

    u4ModemStatus = UART_READ32(REG_SER_MSR);
    LOG(7, "ModemStatus 0x%x\n", u4ModemStatus);

    if (u4ModemStatus & MODEM_DSR_CHG)
    {
        LOG(5, "MODEM_DSR_CHG, current DSR = %d\n", ((u4ModemStatus & MODEM_DSR) > 0) ? FALSE : TRUE);
    }
    if (u4ModemStatus & MODEM_CTS_CHG)
    {
        LOG(5, "MODEM_CTS_CHG, current CTS = %d\n", ((u4ModemStatus & MODEM_CTS) > 0) ? FALSE : TRUE);
    }
    if (u4ModemStatus & MODEM_RI_CHG)
    {
        LOG(5, "MODEM_RI_CHG, current RI = %d\n", ((u4ModemStatus & MODEM_RI) > 0) ? FALSE : TRUE);
    }
    if (u4ModemStatus & MODEM_DCD_CHG)
    {
        LOG(5, "MODEM_DCD_CHG, current DCD = %d\n", ((u4ModemStatus & MODEM_DCD) > 0) ? FALSE : TRUE);
    }
}

static void _ModemNotify(VOID* pv_nfy_tag, RS_232_COND_T  e_nfy_cond, UINT32 ui4_data)
{
    UINT32 u4Idx;
    UINT32 u4RxLen;

    UNUSED(ui4_data);
    UNUSED(pv_nfy_tag);
    
    switch (e_nfy_cond)
    {
        case RS_232_COND_REC_BUFFER:
        	u4RxLen = UART_GetRcvDataLen(MODEM_PORT);
        	UNUSED(UART_ReadRxFifo(MODEM_PORT, (UINT8 *)_aucBuf, u4RxLen));
            for (u4Idx = 0; u4Idx < u4RxLen; u4Idx++)
            {
        	    LOG(9, "(%c)\n",_aucBuf[u4Idx]);
            }
        	break;
        case RS_232_COND_XMT_EMPTY:
        	// Since we use polling mode to write, so ignore this notify
        	break;
        default:
        	LOG(7, "Invalid notify condition %x\n", (UINT8)e_nfy_cond);
        	break;	
    }    	
}

/*
static INT32 _DetachNotify(UINT8 u1Port)
{
    RS_232_NFY_INFO_T _rRs232Notify = {NULL, NULL};
    UART_SetNotifyFuc(u1Port, &_rRs232Notify);
    return TRUE;
}
*/

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
BOOL UART_ModemInit(void)
{
    UART_HwModemCtr(TRUE);
    
    if (!_fgModemInit)
    {
        UART_SetNotifyFuc(MODEM_PORT, &_rRs232ModemNotify);
        UNUSED(UART_SetModemStatusChgNotify(_pfRs232ModemStatusChgNotify));
        _fgModemInit = TRUE;
    }

    UNUSED(UART_ModemCtl(MODEM_PORT, FALSE, FALSE));
    return TRUE;
}

INT32 UART_WriteModem(const CHAR * pucData, UINT32 u4Len)
{
     UNUSED(UART_ModemInit());
     UNUSED(UART_Write(MODEM_PORT, (const UINT8 *)pucData, u4Len, TRUE));

     return TRUE;
}

INT32 UART_ModemCmd(UCHAR ucCmd)
{
    if (!_fgModemInit)
    {
        UART_SetNotifyFuc(MODEM_PORT, &_rRs232ModemNotify);
        _fgModemInit = TRUE;
    }    
    
    return UART_WriteModem(_aszMInit[ucCmd], x_strlen((const CHAR *)_aszMInit[ucCmd]));
}

/******************************************************************************
* Function: INT32 UART_ModemCtl(UINT8 u1DecId, BOOL fgRTS, BOOL fgDTR)
*
* Description:
*      RTS, CTS ouput control
*
* Inputs: -
*      u1Port:  Uart id (1: Uart 1, 2: Uart 2)
*      fgRTS:   RTS output (TRUE : high, FLASE: low)
*      fgDTR:   DTR output (TRUE : high, FLASE: low)
* 
* Returns: -
*      TRUE: Succeed
*      FALSE: Fail
******************************************************************************/

INT32 UART_ModemCtl(UINT8 u1Port, BOOL fgRTS, BOOL fgDTR)
{
    INT32 i4Ret = TRUE;

    if (u1Port != UART_PORT_1)
    {
        i4Ret = FALSE;
    }
    else
    {
        if (fgRTS)
        {
            UART_CLR_BIT(REG_SER_MSR, MODEM_RTS);
        }
        else
        {
            UART_SET_BIT(REG_SER_MSR, MODEM_RTS);
        }
        if (fgDTR)
        {
            UART_CLR_BIT(REG_SER_MSR, MODEM_DTR);
        }
        else
        {
            UART_SET_BIT(REG_SER_MSR, MODEM_DTR);
        }
    }

    return i4Ret;
}

#endif
