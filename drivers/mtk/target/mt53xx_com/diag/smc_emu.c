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
#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"

#include "smc_emu.h"

extern void SmcWarmRst(void);

#define TIDE_TXMASK		    0x0f00
static void SmcSetTxTide(UINT16 u2TxTide)
{
    UINT16 u2Tide;

    u2Tide = SMC_READ16(REG_SMC_TIDE) & ~TIDE_TXMASK;
    u2Tide |= (u2TxTide << 8) & TIDE_TXMASK;
    SMC_WRITE16(REG_SMC_TIDE, u2Tide);
}

/******************************************************************************
 *  Local Definition
 *****************************************************************************/
BOOL _SmcRegRwTest(void);
BOOL _SmcEmuWaitMsg(UINT32 u4MsgWait);


/******************************************************************************
 *  Local Functions
 *****************************************************************************/
BOOL _SmcRegRwTest(void)
{
    SMC_CON     tTstSMC_CON   = {1, 1, 0};
    SMC_CNF     tTstSMC_CNF   = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    SMC_BRR     tTstSMC_BRR   = {3, 0x1FF};
    SMC_IRQEN   tTstSMC_IRQEN = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    SMC_RETRY   tTstSMC_RETRY = {7, 7};
    SMC_TIDE    tTstSMC_TIDE  = {0xF, 0xF};
    SMC_ATIME   tTstSMC_ATIME = {0xFFFF};
    SMC_DTIME   tTstSMC_DTIME = {0xFFF};
    SMC_WTIME   tTstSMC_WTIME = {0xFFFF};
    SMC_GTIME   tTstSMC_GTIME = {0xF};
    SMC_ETIME   tTstSMC_ETIME = {0x3F};
    SMC_INS     tTstSMC_INS   = {0xFF, 1};
    SMC_P3      tTstSMC_P3    = {0xFF};

    SMC_CON     tSMC_CON  ;
    SMC_CNF     tSMC_CNF  ;
    SMC_BRR     tSMC_BRR  ;
    SMC_IRQEN   tSMC_IRQEN;
    SMC_RETRY   tSMC_RETRY;
    SMC_TIDE    tSMC_TIDE ;
    SMC_ATIME   tSMC_ATIME;
    SMC_DTIME   tSMC_DTIME;
    SMC_WTIME   tSMC_WTIME;
    SMC_GTIME   tSMC_GTIME;
    SMC_ETIME   tSMC_ETIME;
    SMC_INS     tSMC_INS  ;
    SMC_P3      tSMC_P3   ;

    UINT32 u4Var;       //5391

    SMC_WRITE16(REG_SMC_CON  , *(UINT16*)&tTstSMC_CON  );
    SMC_WRITE16(REG_SMC_CNF  , *(UINT16*)&tTstSMC_CNF  );
    SMC_WRITE16(REG_SMC_BRR  , *(UINT16*)&tTstSMC_BRR  );
    SMC_WRITE16(REG_SMC_IRQEN, *(UINT16*)&tTstSMC_IRQEN);
    SMC_WRITE16(REG_SMC_RETRY, *(UINT16*)&tTstSMC_RETRY);
    SMC_WRITE16(REG_SMC_TIDE , *(UINT16*)&tTstSMC_TIDE );
    SMC_WRITE16(REG_SMC_ATIME, *(UINT16*)&tTstSMC_ATIME);
    SMC_WRITE16(REG_SMC_DTIME, *(UINT16*)&tTstSMC_DTIME);
    SMC_WRITE16(REG_SMC_WTIME, *(UINT16*)&tTstSMC_WTIME);
    SMC_WRITE16(REG_SMC_GTIME, *(UINT16*)&tTstSMC_GTIME);
    SMC_WRITE16(REG_SMC_ETIME, *(UINT16*)&tTstSMC_ETIME);
    SMC_WRITE16(REG_SMC_INS  , *(UINT16*)&tTstSMC_INS  );
    SMC_WRITE16(REG_SMC_P3   , *(UINT16*)&tTstSMC_P3   );
#if 0
    tSMC_CON   = *(SMC_CON*)   &SMC_READ16(REG_SMC_CON);
    tSMC_CNF   = *(SMC_CNF*)   &SMC_READ16(REG_SMC_CNF);
    tSMC_BRR   = *(SMC_BRR*)   &SMC_READ16(REG_SMC_BRR);
    tSMC_IRQEN = *(SMC_IRQEN*) &SMC_READ16(REG_SMC_IRQEN);
    tSMC_RETRY = *(SMC_RETRY*) &SMC_READ16(REG_SMC_RETRY);
    tSMC_TIDE  = *(SMC_TIDE*)  &SMC_READ16(REG_SMC_TIDE);
    tSMC_ATIME = *(SMC_ATIME*) &SMC_READ16(REG_SMC_ATIME);
    tSMC_DTIME = *(SMC_DTIME*) &SMC_READ16(REG_SMC_DTIME);
    tSMC_WTIME = *(SMC_WTIME*) &SMC_READ16(REG_SMC_WTIME);
    tSMC_GTIME = *(SMC_GTIME*) &SMC_READ16(REG_SMC_GTIME);
    tSMC_ETIME = *(SMC_ETIME*) &SMC_READ16(REG_SMC_ETIME);
    tSMC_INS   = *(SMC_INS*)   &SMC_READ16(REG_SMC_INS);
    tSMC_P3    = *(SMC_P3*)    &SMC_READ16(REG_SMC_P3);
#else       //5391
    u4Var= SMC_READ16(REG_SMC_CON); tSMC_CON   = *(SMC_CON*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_CNF); tSMC_CNF   = *(SMC_CNF*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_BRR); tSMC_BRR   = *(SMC_BRR*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_IRQEN); tSMC_IRQEN   = *(SMC_IRQEN*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_RETRY); tSMC_RETRY   = *(SMC_RETRY*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_TIDE); tSMC_TIDE   = *(SMC_TIDE*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_ATIME); tSMC_ATIME   = *(SMC_ATIME*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_DTIME); tSMC_DTIME   = *(SMC_DTIME*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_WTIME); tSMC_WTIME   = *(SMC_WTIME*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_GTIME); tSMC_GTIME   = *(SMC_GTIME*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_ETIME); tSMC_ETIME   = *(SMC_ETIME*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_INS); tSMC_INS   = *(SMC_INS*) &u4Var;
    u4Var= SMC_READ16(REG_SMC_P3); tSMC_P3   = *(SMC_P3*) &u4Var;
#endif
    // SMC_CON
    if(tSMC_CON.u2SMCON != tTstSMC_CON.u2SMCON ||
       tSMC_CON.u2CSTOP != tTstSMC_CON.u2CSTOP )
    {
        Printf("\n_SmcRegRwTest(): SMC_CON r/w value error!\n");
        return FALSE;
    }

    // SMC_CNF
    if(tSMC_CNF.u2RXACK  != tTstSMC_CNF.u2RXACK  ||
       tSMC_CNF.u2TXACK  != tTstSMC_CNF.u2TXACK  ||
       tSMC_CNF.u2CPOL   != tTstSMC_CNF.u2CPOL   ||
       tSMC_CNF.u2SINV   != tTstSMC_CNF.u2SINV   ||
       tSMC_CNF.u2SDIR   != tTstSMC_CNF.u2SDIR   ||
       tSMC_CNF.u2ODD    != tTstSMC_CNF.u2ODD    ||
       tSMC_CNF.u2SMCSEL != tTstSMC_CNF.u2SMCSEL ||
       tSMC_CNF.u2TOUT   != tTstSMC_CNF.u2TOUT   ||
       tSMC_CNF.u2T1EN   != tTstSMC_CNF.u2T1EN   ||
       tSMC_CNF.u2T0EN   != tTstSMC_CNF.u2T0EN   ||
       tSMC_CNF.u2HFEN   != tTstSMC_CNF.u2HFEN   )
    {
        Printf("\n_SmcRegRwTest(): SMC_CNF r/w value error!\n");
        return FALSE;
    }

    // SMC_BRR
    if(tSMC_BRR.u2SMCCLK != tTstSMC_BRR.u2SMCCLK  ||
       tSMC_BRR.u2ETU    != tTstSMC_BRR.u2ETU    )
    {
        Printf("\n_SmcRegRwTest(): SMC_BRR r/w value error!\n");
        return FALSE;
    }

    // SMC_IRQEN
    if(tSMC_IRQEN.u2TXTIDE != tTstSMC_IRQEN.u2TXTIDE ||
       tSMC_IRQEN.u2RXTIDE != tTstSMC_IRQEN.u2RXTIDE ||
       tSMC_IRQEN.u2OVRUN  != tTstSMC_IRQEN.u2OVRUN  ||
       tSMC_IRQEN.u2TOUT   != tTstSMC_IRQEN.u2TOUT   ||
       tSMC_IRQEN.u2TXERR  != tTstSMC_IRQEN.u2TXERR  ||
       tSMC_IRQEN.u2ATRERR != tTstSMC_IRQEN.u2ATRERR ||
       tSMC_IRQEN.u2SMCOFF != tTstSMC_IRQEN.u2SMCOFF ||
       tSMC_IRQEN.u2T0END  != tTstSMC_IRQEN.u2T0END  ||
       tSMC_IRQEN.u2RXERR  != tTstSMC_IRQEN.u2RXERR  ||
       tSMC_IRQEN.u2T1END  != tTstSMC_IRQEN.u2T1END  ||
       tSMC_IRQEN.u2EDCERR != tTstSMC_IRQEN.u2EDCERR )
    {
        Printf("\n_SmcRegRwTest(): SMC_IRQEN r/w value error!\n");
        return FALSE;
    }

    // SMC_RETRY
    if(tSMC_RETRY.u2RXRETRY != tTstSMC_RETRY.u2RXRETRY ||
       tSMC_RETRY.u2TXRETRY != tTstSMC_RETRY.u2TXRETRY )
    {
        Printf("\n_SmcRegRwTest(): SMC_RETRY r/w value error!\n");
        return FALSE;
    }

    // SMC_TIDE
    if(tSMC_TIDE.u2RXTIDE != tTstSMC_TIDE.u2RXTIDE ||
       tSMC_TIDE.u2TXTIDE != tTstSMC_TIDE.u2TXTIDE )
    {
        Printf("\n_SmcRegRwTest(): SMC_TIDE r/w value error!\n");
        return FALSE;
    }

    // SMC_ATIME
    if(tSMC_ATIME.u2ATIME != tTstSMC_ATIME.u2ATIME)
    {
        Printf("\n_SmcRegRwTest(): SMC_ATIME r/w value error!\n");
        return FALSE;
    }

    // SMC_DTIME
    if(tSMC_DTIME.u2DTIME != tTstSMC_DTIME.u2DTIME)
    {
        Printf("\n_SmcRegRwTest(): SMC_DTIME r/w value error!\n");
        return FALSE;
    }

    // SMC_WTIME
    if(tSMC_WTIME.u2WTIME != tTstSMC_WTIME.u2WTIME)
    {
        Printf("\n_SmcRegRwTest(): SMC_WTIME r/w value error!\n");
        return FALSE;
    }

    // SMC_GTIME
    if(tSMC_GTIME.u2GTIME != tTstSMC_GTIME.u2GTIME)
    {
        Printf("\n_SmcRegRwTest(): SMC_GTIME r/w value error!\n");
        return FALSE;
    }

    // SMC_ETIME
    if(tSMC_ETIME.u2ETIME != tTstSMC_ETIME.u2ETIME)
    {
        Printf("\n_SmcRegRwTest(): SMC_ETIME r/w value error!\n");
        return FALSE;
    }

    // SMC_INS
    if(tSMC_INS.u2SMCINS != tTstSMC_INS.u2SMCINS ||
       tSMC_INS.u2INSD   != tTstSMC_INS.u2INSD   )
    {
        Printf("\n_SmcRegRwTest(): SMC_INS r/w value error!\n");
        return FALSE;
    }

    // SMC_P3
    if(tSMC_P3.u2SMCP3 != tTstSMC_P3.u2SMCP3)
    {
        Printf("\n_SmcRegRwTest(): SMC_P3 r/w value error!\n");
        return FALSE;
    }

    return TRUE;
}


BOOL _SmcEmuWaitMsg(UINT32 u4MsgWait)
{
    UINT32  u4Msg;
    UINT16  u2MsgIdx;
    SIZE_T  zMsgSize = sizeof(u4Msg);
    INT32   i4Ret;
    BOOL    bRet;

    while (TRUE)
    {
        i4Ret = x_msg_q_receive_timeout(&u2MsgIdx,
                                        &u4Msg,
                                        &zMsgSize,
                                        &hSmcEmuMsgQ,
                                        1,
                                        10000);
        if(i4Ret == OSR_TIMEOUT)
        {
            bRet = FALSE;
            break;
        }

        if (u4Msg == u4MsgWait)
        {
            bRet = TRUE;
            break;
        }
    }

    return bRet;
}


void _SmcEmuFlushMsgQ(void)
{
    UINT32  u4Msg;
    UINT16  u2MsgIdx;
    SIZE_T  zMsgSize = sizeof(u4Msg);

    while (x_msg_q_receive(&u2MsgIdx,
                           &u4Msg,
                           &zMsgSize,
                           &hSmcEmuMsgQ,
                           1,
                           X_MSGQ_OPTION_NOWAIT)==OSR_OK);
}


BOOL _SmcT0Cmd1Test(void)
{
    UINT16  SW1SW2;
	UINT8 txData[5];
	UINT8 result[100];
	UINT16 rcvSize;
	INT32 i4Index;  UNUSED(i4Index);
	UINT8 Error;

	txData[0] = 0xA0;  // Class
	txData[1] = 0xA4;  // INS
	txData[2] = 0x00;  // P1
	txData[3] = 0x00;  // P2
	txData[4] = 0x02;  // P3

	SW1SW2 = SmcT0Cmd(txData, 5, result, &rcvSize, &Error);
	if ((SW1SW2 & 0xFF00) == 0x6E00)
	{
	    return TRUE;
	}
	else
	{
	    return FALSE;
	}
}

BOOL _SmcT0Cmd2Test(void)
{
    UINT16  SW1SW2;
	UINT8 txData[5];
	UINT8 result[100];
	UINT16 rcvSize;
	UINT8 Error;
	INT32 i4Index;  UNUSED(i4Index);

	txData[0] = 0xA0;  // Class
	txData[1] = 0xC0;  // INS
	txData[2] = 0x00;  // P1
	txData[3] = 0x00;  // P2
	txData[4] = 0x17;  // P3

	SW1SW2 = SmcT0Cmd(txData, 5, result, &rcvSize, &Error);
	if ((SW1SW2 & 0xFF00) == 0x6E00)
	{
	    return TRUE;
	}
	else
	{
	    return FALSE;
	}
}


#define T1BlockFrameLen(pu1Data) (*(pu1Data + 2))
BOOL _SmcT1Cmd1Test(void)
{
	UINT8 txData[7];
	UINT8 result[100];
	UINT16 rcvSize;
	UINT8 Error;
	const UINT8 rxGolden[] = {0x00, 0xE1, 0x01, 0x75};

	txData[0] = 0x00;  // NAD(node address)
	txData[1] = 0xC1;  // PCB(protocol control byte)
	txData[2] = 0x01;  // LEN(length)
	txData[3] = 0x75;  // Data

    // Clear result buffer
    x_memset(result, NULL, sizeof(rxGolden));

    // Send command
	SmcT1Cmd(txData, 4, result, &rcvSize, &Error);

    // Compare result
	if ((x_memcmp((const VOID *)(rxGolden+3),
	              (const VOID *)(result+3),
    	          T1BlockFrameLen(rxGolden)) != 0) ||
    	(Error == TRUE))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


BOOL _SmcT1Cmd2Test(void)
{
	UINT8 txData[8];
	UINT8 result[100];
	UINT16 rcvSize;
	UINT8 Error;
	const UINT8 rxGolden[] = {0x0,0x0,0x2e,0x0,0x1,0x20,0x9,0xee,
		                      0x7b,0x0,0x0,0x50,0x45,0x52,0x46,0x45,
		                      0x43,0x54,0x56,0x43,0x4f,0x4e,0x44,0x49,
		                      0x54,0x49,0x4f,0x4e,0x41,0x4c,0x41,0x43,
		                      0x43,0x45,0x53,0x53,0x53,0x59,0x53,0x54,
		                      0x45,0x4d,0x31,0x81,0x1,0x0,0x0,0x90,
		                      0x0};

	txData[0] = 0x00;  // NAD(node address)
	txData[1] = 0x00;  // PCB(protocol control byte)
	txData[2] = 0x05;  // LEN(length)
	txData[3] = 0x80;  // Data
	txData[4] = 0x30;  // Data
	txData[5] = 0x00;  // Data
	txData[6] = 0x00;  // Data
	txData[7] = 0x00;  // Data

    // Clear result buffer
    x_memset(result, NULL, sizeof(rxGolden));

    // Send command
	SmcT1Cmd(txData, 8, result, &rcvSize, &Error);

    // Compare result
	if ((x_memcmp((const VOID *)(rxGolden+3), (const VOID *)(result+3),
    	          T1BlockFrameLen(rxGolden)) != 0) ||
    	(Error == TRUE))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


BOOL _SmcT1Cmd4Test(void)
{
	UINT8 txData[8];
	UINT8 result[100];
	UINT16 rcvSize;
	UINT8 Error;
	UINT32  i;  UNUSED(i);
	const UINT8 rxGolden[] = {0x00, 0xE1, 0x01, 0xFC};

	txData[0] = 0x00;  // NAD(node address)
	txData[1] = 0xC1;  // PCB(protocol control byte)
	txData[2] = 0x01;  // LEN(length)
	txData[3] = 0xFC;  // Data

	// Clear result buffer
    x_memset(result, NULL, sizeof(result));

    // Send command
	SmcT1Cmd(txData, 4, result, &rcvSize, &Error);

    // Compare result
	if ((x_memcmp((const VOID *)(rxGolden), (const VOID *)result,
    	          T1BlockFrameLen(rxGolden)) != 0) ||
    	(Error == TRUE))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/******************************************************************************
 *  Exported Functions
 *****************************************************************************/
void SmcRegSetDfl(void)
{
    SMC_CON     tSMC_CON   = {0, 0, 0};
    SMC_CNF     tSMC_CNF   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    SMC_BRR     tSMC_BRR   = {1, 372};
    SMC_IRQEN   tSMC_IRQEN = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    SMC_RETRY   tSMC_RETRY = {3, 3};
    SMC_TIDE    tSMC_TIDE  = {0, 0};
    SMC_COUNT   tSMC_COUNT = {0};
    SMC_ATIME   tSMC_ATIME = {0xAFC7};
    SMC_DTIME   tSMC_DTIME = {0x3E7};
    SMC_WTIME   tSMC_WTIME = {0x983};
    SMC_GTIME   tSMC_GTIME = {10};
    SMC_ETIME   tSMC_ETIME = {15};
    SMC_INS     tSMC_INS   = {0, 0};
    SMC_P3      tSMC_P3    = {0};

    SMC_WRITE16(REG_SMC_CON  , *(UINT16*)&tSMC_CON  );
    SMC_WRITE16(REG_SMC_CNF  , *(UINT16*)&tSMC_CNF  );
    SMC_WRITE16(REG_SMC_BRR  , *(UINT16*)&tSMC_BRR  );
    SMC_WRITE16(REG_SMC_IRQEN, *(UINT16*)&tSMC_IRQEN);
    SMC_WRITE16(REG_SMC_RETRY, *(UINT16*)&tSMC_RETRY);
    SMC_WRITE16(REG_SMC_TIDE , *(UINT16*)&tSMC_TIDE );
    SMC_WRITE16(REG_SMC_COUNT, *(UINT16*)&tSMC_COUNT);
    SMC_WRITE16(REG_SMC_ATIME, *(UINT16*)&tSMC_ATIME);
    SMC_WRITE16(REG_SMC_DTIME, *(UINT16*)&tSMC_DTIME);
    SMC_WRITE16(REG_SMC_WTIME, *(UINT16*)&tSMC_WTIME);
    SMC_WRITE16(REG_SMC_GTIME, *(UINT16*)&tSMC_GTIME);
    SMC_WRITE16(REG_SMC_ETIME, *(UINT16*)&tSMC_ETIME);
    SMC_WRITE16(REG_SMC_INS  , *(UINT16*)&tSMC_INS  );
    SMC_WRITE16(REG_SMC_P3   , *(UINT16*)&tSMC_P3   );
}


BOOL SmcRegDflTest(void)
{
        UINT32 u4Var;       //5391
#if 0
    SMC_CON     tSMC_CON   = *(SMC_CON*)   &SMC_READ16(REG_SMC_CON);
    SMC_CNF     tSMC_CNF   = *(SMC_CNF*)   &SMC_READ16(REG_SMC_CNF);
    SMC_BRR     tSMC_BRR   = *(SMC_BRR*)   &SMC_READ16(REG_SMC_BRR);
    SMC_IRQEN   tSMC_IRQEN = *(SMC_IRQEN*) &SMC_READ16(REG_SMC_IRQEN);
    SMC_STA     tSMC_STA   = *(SMC_STA*)   &SMC_READ16(REG_SMC_STA);
    SMC_RETRY   tSMC_RETRY = *(SMC_RETRY*) &SMC_READ16(REG_SMC_RETRY);
    SMC_TIDE    tSMC_TIDE  = *(SMC_TIDE*)  &SMC_READ16(REG_SMC_TIDE);
    SMC_COUNT   tSMC_COUNT = *(SMC_COUNT*) &SMC_READ16(REG_SMC_COUNT);
    SMC_ATIME   tSMC_ATIME = *(SMC_ATIME*) &SMC_READ16(REG_SMC_ATIME);
    SMC_DTIME   tSMC_DTIME = *(SMC_DTIME*) &SMC_READ16(REG_SMC_DTIME);
    SMC_WTIME   tSMC_WTIME = *(SMC_WTIME*) &SMC_READ16(REG_SMC_WTIME);
    SMC_GTIME   tSMC_GTIME = *(SMC_GTIME*) &SMC_READ16(REG_SMC_GTIME);
    SMC_ETIME   tSMC_ETIME = *(SMC_ETIME*) &SMC_READ16(REG_SMC_ETIME);
    SMC_INS     tSMC_INS   = *(SMC_INS*)   &SMC_READ16(REG_SMC_INS);
    SMC_P3      tSMC_P3    = *(SMC_P3*)    &SMC_READ16(REG_SMC_P3);
    SMC_SW1     tSMC_SW1   = *(SMC_SW1*)   &SMC_READ16(REG_SMC_SW1);
    SMC_SW2     tSMC_SW2   = *(SMC_SW2*)   &SMC_READ16(REG_SMC_SW2);
#else
    SMC_CON     tSMC_CON   ;
    SMC_CNF     tSMC_CNF   ;
    SMC_BRR     tSMC_BRR   ;
    SMC_IRQEN   tSMC_IRQEN ;
    SMC_STA     tSMC_STA   ;
    SMC_RETRY   tSMC_RETRY ;
    SMC_TIDE    tSMC_TIDE  ;
    SMC_COUNT   tSMC_COUNT ;
    SMC_ATIME   tSMC_ATIME ;
    SMC_DTIME   tSMC_DTIME ;
    SMC_WTIME   tSMC_WTIME ;
    SMC_GTIME   tSMC_GTIME ;
    SMC_ETIME   tSMC_ETIME ;
    SMC_INS     tSMC_INS   ;
    SMC_P3      tSMC_P3    ;
    SMC_SW1     tSMC_SW1   ;
    SMC_SW2     tSMC_SW2   ;
    u4Var=SMC_READ16(REG_SMC_CON); tSMC_CON =*(SMC_CON*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_CNF); tSMC_CNF =*(SMC_CNF*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_BRR); tSMC_BRR =*(SMC_BRR*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_IRQEN); tSMC_IRQEN =*(SMC_IRQEN*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_STA); tSMC_STA =*(SMC_STA*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_RETRY); tSMC_RETRY =*(SMC_RETRY*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_TIDE); tSMC_TIDE =*(SMC_TIDE*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_COUNT); tSMC_COUNT =*(SMC_COUNT*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_ATIME); tSMC_ATIME =*(SMC_ATIME*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_DTIME); tSMC_DTIME =*(SMC_DTIME*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_WTIME); tSMC_WTIME =*(SMC_WTIME*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_GTIME); tSMC_GTIME =*(SMC_GTIME*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_ETIME); tSMC_ETIME =*(SMC_ETIME*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_INS); tSMC_INS =*(SMC_INS*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_P3); tSMC_P3 =*(SMC_P3*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_SW1); tSMC_SW1 =*(SMC_SW1*)&u4Var;
    u4Var=SMC_READ16(REG_SMC_SW2); tSMC_SW2 =*(SMC_SW2*)&u4Var;
#endif
    // SMC_CON
    if(tSMC_CON.u2SMCON != 0 ||
       tSMC_CON.u2CSTOP != 0 )
    {
        Printf("\nSmcRWRegDflTest(): SMC_CON default value error!\n");
        return FALSE;
    }

    // SMC_CNF
    if(tSMC_CNF.u2RXACK  != 0 ||
       tSMC_CNF.u2TXACK  != 0 ||
       tSMC_CNF.u2CPOL   != 0 ||
       tSMC_CNF.u2SINV   != 0 ||
       tSMC_CNF.u2SDIR   != 0 ||
       tSMC_CNF.u2ODD    != 0 ||
       tSMC_CNF.u2SMCSEL != 0 ||
       tSMC_CNF.u2TOUT   != 0 ||
       tSMC_CNF.u2T1EN   != 0 ||
       tSMC_CNF.u2T0EN   != 0 ||
       tSMC_CNF.u2HFEN   != 0 )
    {
        Printf("\nSmcRWRegDflTest(): SMC_CNF default value error!\n");
        return FALSE;
    }

    // SMC_BRR
    if(tSMC_BRR.u2SMCCLK != 1    ||
       tSMC_BRR.u2ETU    != 372 )
    {
        Printf("\nSmcRWRegDflTest(): SMC_BRR default value error!\n");
        return FALSE;
    }

    // SMC_IRQEN
    if(tSMC_IRQEN.u2TXTIDE != 0 ||
       tSMC_IRQEN.u2RXTIDE != 0 ||
       tSMC_IRQEN.u2OVRUN  != 0 ||
       tSMC_IRQEN.u2TOUT   != 0 ||
       tSMC_IRQEN.u2TXERR  != 0 ||
       tSMC_IRQEN.u2ATRERR != 0 ||
       tSMC_IRQEN.u2SMCOFF != 0 ||
       tSMC_IRQEN.u2T0END  != 0 ||
       tSMC_IRQEN.u2RXERR  != 0 ||
       tSMC_IRQEN.u2T1END  != 0 ||
       tSMC_IRQEN.u2EDCERR != 0 )
    {
        Printf("\nSmcRWRegDflTest(): SMC_IRQEN default value error!\n");
        return FALSE;
    }

    // SMC_STA
    if(tSMC_STA.u2TXTIDE != 0 ||
       tSMC_STA.u2RXTIDE != 0 ||
       tSMC_STA.u2OVRUN  != 0 ||
       tSMC_STA.u2TOUT   != 0 ||
       tSMC_STA.u2TXERR  != 0 ||
       tSMC_STA.u2ATRERR != 0 ||
       tSMC_STA.u2SMCOFF != 0 ||
       tSMC_STA.u2T0END  != 0 ||
       tSMC_STA.u2RXERR  != 0 ||
       tSMC_STA.u2T1END  != 0 ||
       tSMC_STA.u2EDCERR != 0 )
    {
        Printf("\nSmcRWRegDflTest(): SMC_STA default value error!\n");
        return FALSE;
    }

    // SMC_RETRY
    if(tSMC_RETRY.u2RXRETRY != 3 ||
       tSMC_RETRY.u2TXRETRY != 3 )
    {
        Printf("\nSmcRWRegDflTest(): SMC_RETRY default value error!\n");
        return FALSE;
    }

    // SMC_TIDE
    if(tSMC_TIDE.u2RXTIDE != 0 ||
       tSMC_TIDE.u2TXTIDE != 0 )
    {
        Printf("\nSmcRWRegDflTest(): SMC_TIDE default value error!\n");
        return FALSE;
    }

    // SMC_COUNT
    if(tSMC_COUNT.u2COUNT != 0)
    {
        Printf("\nSmcRWRegDflTest(): SMC_COUNT default value error!\n");
        return FALSE;
    }

    // SMC_ATIME
    if(tSMC_ATIME.u2ATIME != 0xAFC7)
    {
        Printf("\nSmcRWRegDflTest(): SMC_ATIME default value error!\n");
        return FALSE;
    }

    // SMC_DTIME
    if(tSMC_DTIME.u2DTIME != 0x3E7)
    {
        Printf("\nSmcRWRegDflTest(): SMC_DTIME default value error!\n");
        return FALSE;
    }

    // SMC_WTIME
    if(tSMC_WTIME.u2WTIME != 0x983)
    {
        Printf("\nSmcRWRegDflTest(): SMC_WTIME default value error!\n");
        return FALSE;
    }

    // SMC_GTIME
    if(tSMC_GTIME.u2GTIME != 10)
    {
        Printf("\nSmcRWRegDflTest(): SMC_GTIME default value error!\n");
        return FALSE;
    }

    // SMC_ETIME
    if(tSMC_ETIME.u2ETIME != 15)
    {
        Printf("\nSmcRWRegDflTest(): SMC_ETIME default value error!\n");
        return FALSE;
    }

    // SMC_INS
    if(tSMC_INS.u2SMCINS != 0 ||
       tSMC_INS.u2INSD   != 0 )
    {
        Printf("\nSmcRWRegDflTest(): SMC_INS default value error!\n");
        return FALSE;
    }

    // SMC_P3
    if(tSMC_P3.u2SMCP3 != 0)
    {
        Printf("\nSmcRWRegDflTest(): SMC_P3 default value error!\n");
        return FALSE;
    }

    // SMC_SW1
    if(tSMC_SW1.u2SMCSW1 != 0)
    {
        Printf("\nSmcRWRegDflTest(): SMC_SW1 default value error!\n");
        return FALSE;
    }

    // SMC_SW2
    if(tSMC_SW2.u2SMCSW2 != 0)
    {
        Printf("\nSmcRWRegDflTest(): SMC_SW2 default value error!\n");
        return FALSE;
    }

    return TRUE;
}


BOOL SmcRegRwTest(void)
{
    if(_SmcRegRwTest() == FALSE)
    {
        return FALSE;
    }

    SmcRegSetDfl();
    if(SmcRegDflTest() == FALSE)
    {
        Printf("\nSmcRWRegTest(): Set back to default value error!\n");
        return FALSE;
    }

    return TRUE;
}


#define SMC_CHK_INT()                                   \
    while (TRUE)                                        \
    {                                                   \
        if((x_msg_q_receive_timeout(&u2MsgIdx,          \
                                    &u4Msg,             \
                                    &zMsgSize,          \
                                    &hSmcEmuMsgQ,       \
                                    1,                  \
                                    1000)) == OSR_OK)   \
        {                                               \
            switch(u4Msg)                               \
            {                                           \
            case EMU_MSG_TXTIDE:                        \
                bIntTxTide = TRUE;                      \
                break;                                  \
            case EMU_MSG_RXTIDE:                        \
                bIntRxTide = TRUE;                      \
                break;                                  \
            case EMU_MSG_OVRUN :                        \
                bIntOvrun = TRUE;                       \
                break;                                  \
            case EMU_MSG_TOUT  :                        \
                bIntTout = TRUE;                        \
                break;                                  \
            case EMU_MSG_TXERR :                        \
                bIntTxErr = TRUE;                       \
                break;                                  \
            case EMU_MSG_ATRERR:                        \
                bIntAtrErr = TRUE;                      \
                break;                                  \
            case EMU_MSG_SIMOFF:                        \
                bIntSimOff = TRUE;                      \
                break;                                  \
            case EMU_MSG_T0END :                        \
                bIntT0End = TRUE;                       \
                break;                                  \
            case EMU_MSG_RXERR :                        \
                bIntRxErr = TRUE;                       \
                break;                                  \
            case EMU_MSG_T1END :                        \
                bIntT1End = TRUE;                       \
                break;                                  \
            case EMU_MSG_EDCERR:                        \
                bIntEdcErr = TRUE;                      \
                break;                                  \
            default:                                    \
                break;                                  \
            }                                           \
        }                                               \
        else                                            \
        {                                               \
            break;                                      \
        }                                               \
    }

BOOL SmcIntTest(void)
{
    UINT32  i;  UNUSED(i);

    BOOL bIntTxTide = FALSE,
         bIntRxTide = FALSE,
         bIntOvrun  = TRUE,     // Not yet tested
         bIntTout   = FALSE,
         bIntTxErr  = FALSE,
         bIntAtrErr = FALSE,
         bIntSimOff = FALSE,
         bIntT0End  = TRUE,     // Not yet tested
         bIntRxErr  = FALSE,
         bIntT1End  = FALSE,
         bIntEdcErr = TRUE;     // Not yet tested

    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  u4ATRLen;
    UINT32  u4Msg;
    UINT16  u2MsgIdx;
    SIZE_T  zMsgSize = sizeof(u4Msg);

    // SIMOFF, TXTIDE, RXTIDE, TOUT, T1END
    SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);
    SmcSetTxTide(1);
    bTestTxTide = TRUE;
    _SmcT1Cmd1Test();
    bTestTxTide = FALSE;
    SMC_CHK_INT();

    // ATR Error
    bTestAtrErr = TRUE;
    SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);
    bTestAtrErr = FALSE;
    SMC_CHK_INT();

    // TxErr, RxErr
    bTestTxRxErr = TRUE;
    SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);
    _SmcT1Cmd1Test();
    bTestTxRxErr = FALSE;
    SMC_CHK_INT();

    return (bIntTxTide && bIntRxTide && bIntOvrun && bIntTout &&
            bIntTxErr && bIntAtrErr && bIntSimOff && bIntT0End &&
            bIntRxErr && bIntT1End && bIntEdcErr);
}


BOOL SmcDeactTest(void)
{
    SmcDisableAllInt();
    SmcEnableInt(IRQ_SMCOFF);
    _SmcEmuFlushMsgQ();

    SmcDeactive();
    return _SmcEmuWaitMsg(EMU_MSG_SIMOFF);
}


BOOL SmcActTest(void)
{
    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  u4ATRLen;
    INT32   i4Ret;

    i4Ret = SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);
    UNUSED(i4Ret);

    return (pucATR[0]==0x3F || pucATR[0]==0x3B);
}


BOOL SmcToutTest(void)
{
    SmcDisableAllInt();
    SmcEnableInt(IRQ_TOUT);
    _SmcEmuFlushMsgQ();

    SmcActTest();

    return _SmcEmuWaitMsg(EMU_MSG_TOUT);
}


BOOL SmcDirTest(void)
{
    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  u4ATRLen;
    UINT8   ucGoldT0[12] = {0x3B, 0xB0, 0x33, 0x00, 0x91, 0x81, 0x31, 0x6B, 0x35, 0xFC};
    UINT8   ucGoldT1[12] = {0x3F, 0x77, 0x18, 0x25, 0x00, 0x29, 0x14, 0x00, 0x62, 0x68, 0x90, 0x00};

    SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);

    return (x_memcmp(pucATR, ucGoldT0, u4ATRLen)==0 ||
            x_memcmp(pucATR, ucGoldT1, u4ATRLen)==0);
}


BOOL SmcPpsTest(void)
{
    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  u4ATRLen;

    SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);
    SmcWarmRst();
    x_thread_delay(50);
    SmcSendPps(0, 0x11);
    return TRUE;
}


BOOL SmcStopClkTest(void)
{
    INT8 i;

    SmcDeactive();
    x_thread_delay(100);
    SmcActive();
    x_thread_delay(100);
    SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) | CNF_CPOL);
    x_thread_delay(100);
    SMC_WRITE16(REG_SMC_CON, SMC_READ16(REG_SMC_CON) | CSTOP);
    Printf("\n          ==> SIMCLK should be HIGH now!\n");

    for(i=10; i>=0; i--)
    {
        Printf("\r          ==> Will change the clock level in %d seconds ... ", i);
        x_thread_delay(1000);
    }
    Printf("\n");

    SMC_WRITE16(REG_SMC_CON, SMC_READ16(REG_SMC_CON) & ~CSTOP);
    x_thread_delay(100);
    SMC_WRITE16(REG_SMC_CNF, SMC_READ16(REG_SMC_CNF) & ~CNF_CPOL);
    x_thread_delay(100);
    SMC_WRITE16(REG_SMC_CON, SMC_READ16(REG_SMC_CON) | CSTOP);
    Printf("          ==> SIMCLK should be LOW now!\n");

    return TRUE;
}


BOOL SmcTimerTest(void)
{
    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  u4ATRLen;
    INT32   i4Ret;

    i4Ret = SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);
    if(i4Ret != SMCR_OK)
    {
        return FALSE;
    }

    bTestWTime = TRUE;
    i4Ret = SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);
    bTestWTime = FALSE;
    if(i4Ret == SMCR_OK)
    {
        return FALSE;
    }

    return TRUE;
}


BOOL SmcT0Test(void)
{
    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  u4ATRLen;
    BOOL bT1, bT2;

    SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);

    bT1 = _SmcT0Cmd1Test();
    bT2 = _SmcT0Cmd2Test();

    return (bT1 && bT2);
}


BOOL SmcT1Test(void)
{
    UINT8   pucATR[ATR_BUF_LEN];
    UINT32  u4ATRLen;
    BOOL bT1, bT2;

    SmcResetCard(pucATR, ATR_BUF_LEN, &u4ATRLen);

    bT1 = _SmcT1Cmd1Test();

    x_thread_delay(1000);

    bT2 = _SmcT1Cmd2Test();

    return (bT1 && bT2);
}

