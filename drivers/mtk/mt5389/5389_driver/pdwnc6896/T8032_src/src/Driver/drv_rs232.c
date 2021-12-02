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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_rs232.c $ 
 *
 * Project:
 * --------
 *   MT8226
 *
 * Description:
 * ------------
 *   BIM register definition
 *   
 * Author:
 * -------
 *   Daniel Hsiao
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 10:47p $    
 *
 * $Revision: #1 $
****************************************************************************/
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "general.h"
#include "hw_bim.h"
//#include "drv_rs232.h"
#include "drv_kr.h"
#include "drv_pdwnc.h"

 
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define _RS232_HIGH_PRIORITY_ 1
#define RS232_Uart_Switch 1

////////////////////////////////////////////////////////////////////////////////
// *********************************************************************
// Global Variable Declaration
// *********************************************************************

//XDATA usage 28 bytes
#define DRS232_LOG_LEN 20

#if RS232_Uart_Switch
UINT8 xdata u1UartPDDbg _at_ 0xF803;
#endif

static UINT8 xdata  _pbRs232TBuf[DRS232_LOG_LEN]; //RS232 transmit buffer
UINT8 uRs232RealData[MAX_UART_BUF_LEN];

BIT _fgRs232ISREnable;
UINT8 xdata _bRs232SendNum;
#define vRs232SetSendNum(x) (_bRs232SendNum = (x))
#define bRs232GetSendNum() _bRs232SendNum
#define pbRs232GetSendBuf() _pbRs232TBuf

UART_DRV_OBJ* fgUartRxData;


////////////////////////////////////////////////////////////////////////////////

void vRs232PriorityProc(void);
PRIVATE void vRs232FlushSendBuf(void) reentrant;
PRIVATE BIT fgRs232WriteMode(void);
PRIVATE BIT fgRs232ReadMode(void);
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Reset RS232 driver variables
 */
PRIVATE void vRs232Reset(void)
{
	vRs232SetSendNum(0);
}

void SwitchBack_PORT0_Set(void)
{
	if((uRs232RealData[0]== 0x20) && \
		(uRs232RealData[1]== 0xdf) && \
		(uRs232RealData[2]== 0x90) && \
		(uRs232RealData[3]== 0x6f)
		)
		{		
		SwitchBack_PORT0();
		}
	#if RS232_Uart_Switch
	else if(u1UartPDDbg == 0x11)            
		{
		u1UartPDDbg = 0;
		SwitchBack_PORT0();
		}
    #endif

}

void vRs232PriorityProc(void)
{
	UINT8 bPriority,i,j;
	

	bPriority = FALSE;	

	for(i=1,j=0;i<=(fgUartRxData->uRs232Rxidx);i+=2)
	{
		(uRs232RealData[j])=(fgUartRxData->uRs232Data[i]);
		j++;

	}
	
	SwitchBack_PORT0_Set();
#if _RS232_HIGH_PRIORITY_
	if(fgRs232WriteMode())
	{
		bPriority = TRUE;
	}
	if(fgRs232ReadMode())
	{
		bPriority = TRUE;
	}
#endif

	if(bPriority != FALSE)
	{
		vRs232FlushSendBuf();

		vRs232Reset();
	}		
}


PRIVATE void vRs232FlushSendBuf(void) reentrant
{
	UINT8 bTmp;
	
	for(bTmp=0; bTmp<bRs232GetSendNum(); bTmp++)
	{
		while(!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; //Write buffer available
		vIO32Write1B(PDWNC_DATAW,pbRs232GetSendBuf()[bTmp]);
	}
	vRs232SetSendNum(0);
}


PRIVATE BIT fgRs232WriteMode(void)
{
	BIT bRet;

	bRet = TRUE;

	switch (uRs232RealData[0])
	{
		case 0x80: // Write 8032 XDATA
			XBYTE[*((UINT16 *)&(uRs232RealData[1]))] = (uRs232RealData[3]);
			break;
			
		case 0x83: // Write 8032 IDATA
			switch (uRs232RealData[1])
			{
				case 0xff:
					IE = (uRs232RealData[2]);
					break;

				case 0xfe:
					IP = (uRs232RealData[2]);
					break;

				case 0xfd:
					TCON = (uRs232RealData[2]);
					break;

				case 0xfc:
					P3 = (uRs232RealData[2]);
					break;

				default:
					DBYTE[(uRs232RealData[1])] = (uRs232RealData[2]);
					break;
			}
			break;

		case 0x86: //Write 8226 Direct Addr Data (32bits)
			XBYTE[*((UINT16*)&(uRs232RealData[1]))+3] = (uRs232RealData[3]);
			XBYTE[*((UINT16*)&(uRs232RealData[1]))+2] = (uRs232RealData[4]);
			XBYTE[*((UINT16*)&(uRs232RealData[1]))+1] = (uRs232RealData[5]);
			XBYTE[*((UINT16*)&(uRs232RealData[1]))+0] = (uRs232RealData[6]);
			break;
		default:
			bRet = FALSE;
			break;
	}

	return (bRet);
}

PRIVATE BIT fgRs232ReadMode(void)
{
	BIT bRet;

	bRet = TRUE;
	
	switch (uRs232RealData[0])
	{
		case 0x00: // Read 8032 XDATA
			pbRs232GetSendBuf()[1] = XBYTE[*((UINT16*)&(uRs232RealData[1]))];
			vRs232SetSendNum(1);
#if UART_DBG
			LogS("read");
#endif
			break;	
      
		case 0x03: // Read 8032 IDATA
			switch (uRs232RealData[1])
			{
				case 0xff:
					pbRs232GetSendBuf()[1] = IE;
					break;

				case 0xfe:
					pbRs232GetSendBuf()[1] = IP;
					break;

				case 0xfd:
					pbRs232GetSendBuf()[1] = TCON;
					break;

				case 0xfc:
					pbRs232GetSendBuf()[1] = P3;
					break;

				default:
					pbRs232GetSendBuf()[1] = DBYTE[(uRs232RealData[1])];
					break;                	
			}                
			vRs232SetSendNum(1);
			break;

		case 0x06: //Read 8226 Direct Addr Data (32bits)            
			pbRs232GetSendBuf()[1] = XBYTE[*((UINT16*)&(uRs232RealData[1]))+3];
			pbRs232GetSendBuf()[2] = XBYTE[*((UINT16*)&(uRs232RealData[1]))+2];
			pbRs232GetSendBuf()[3] = XBYTE[*((UINT16*)&(uRs232RealData[1]))+1];
			pbRs232GetSendBuf()[4] = XBYTE[*((UINT16*)&(uRs232RealData[1]))+0];
			vRs232SetSendNum(4);
			break;
		default:
			bRet = FALSE;
			break;
	}
	pbRs232GetSendBuf()[0] = bRs232GetSendNum();
	vRs232SetSendNum(bRs232GetSendNum()+1);

	return (bRet);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**  putchar is for printf function poring.
 *
 * @param c  output character
 *  @retval   return ouput character
 */
//-----------------------------------------------------------------------------

char putchar (char c)
{
    while(!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; //Write buffer available
        vIO32Write1B(PDWNC_DATAW, c);
	return c;
}


//-----------------------------------------------------------------------------
// Interface(exported) functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**  vRs232Init is to Initialize RS232
 *
 */
//-----------------------------------------------------------------------------
void vRs232Init(UINT8 u1BaudRate)
{
#ifdef CC_MT8223

#define RS232_2_DATA 0x0208
#define RS232_2_STA 0x0209
  #define RS232_2_INTR_EN (1<<2)
  #define RS232_2_USE_FIFO (1<<3)
  #define BAUD_RATE_2_MSK 0x70
  	#define BR2_115200 0x00
	#define BR2_230400 0x10
	#define BR2_460800 0x20
	#define BR2_921600 0x30
	#define BR2_57600 0x40
	#define BR2_28800 0x50
	#define BR2_9600 0x60
	#define BR2_6750000 0x70
	#define RS232_BAUD_TP 0x0202
  	#define USE_FIFO (1<<3)
	_fgRs232ISREnable = TRUE;

	vRegWrite1BOr(RS232_BAUD_TP, USE_FIFO);//ENABLE_1ST_RS232_FIFO();
	vRegWrite1BOr(RS232_2_STA, RS232_2_USE_FIFO);//ENABLE_2ND_RS232_FIFO();
	//ENABLE_2ND_R2322_INT();

	//Enable RS232 interrupt
	//vEnableSysInt(RS232_INT_EN);
	//vEnableSysInt(RS232_8225_INT_EN);
	vDisableSysInt(RS232_8225_INT_EN);

	u1BaudRate=0;

//vRegWriteFldAlign(PDWNC_CCR, u1BaudRate & 0x0F, FLD_BAUD_RATE);


#else
//    vEnableSysInt(RS232_INT_EN);
    vRegWrite1B(PDWNC_STAB, 0xe2); //enter transparent mode;
    vRegWriteFldAlign(PDWNC_BCR, 0x1, FLD_CLEAR_TBUF);
    vRegWriteFldAlign(PDWNC_BCR, 0x1, FLD_CLEAR_RBUF);
    vRegWrite1B(PDWNC_INT_ID, 0); //clear UART inerrupt
    vRegWriteFldAlign(PDWNC_BCR, 0x1, FLD_RXD_BUF_TRIG_LEVEL);
    vRegWriteFldAlign(PDWNC_UART_PD_INT_EN, 0x1, FLD_TOUTE);
    vRegWriteFldAlign(PDWNC_UART_PD_INT_EN, 0x0, FLD_RBUFE);
 //   vRegWrite1B(REG_BCR1, 0x8); //Rx buf time out cycle
    vRegWriteFldAlign(PDWNC_BCR, 0xff, FLD_TIME_OUT_CYCLE);
    vRegWriteFldAlign(PDWNC_T8032_INTEN, 0x1, FLD_UART_DBG_INTEN);    
    vRegWriteFldAlign(PDWNC_CCR, u1BaudRate & 0x0F, FLD_BAUD_RATE);	

    fgUartRxData = pGetUartDrvObj();
    
	#if RS232_Uart_Switch
    u1UartPDDbg = 0;
    #endif	

 #endif
}
// *********************************************************************
// Function : void LogS(char *pcStr)
// Description : send string to PC directly
// Parameter : pcStr - string to send
// Return    : None
// *********************************************************************
void LogS(const char *pcStr) reentrant
{
	UINT8 bTNum, bTCnt;

	//_fgRs232ISREnable = FALSE;

	if (pcStr[0] != 0)
	{
		for(bTNum=0; (pcStr[bTNum]); bTNum++){};
		bTCnt = 0;

		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
		vIO32Write1B(PDWNC_DATAW, 0xb0);
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
		vIO32Write1B(PDWNC_DATAW, bTNum);

		while (bTNum)
		{
			while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
			vIO32Write1B(PDWNC_DATAW, (unsigned char)pcStr[bTCnt++]);
			bTNum--;
		}
	}

	//_fgRs232ISREnable = TRUE;
}
void Log4B(UINT8 bV1, UINT8 bV2, UINT8 bV3, UINT8 bV4) reentrant
{
	UINT8 bTNum, bTCnt;

	//_fgRs232ISREnable = FALSE;

	bTNum = 6;
	bTCnt = 0;
	pbRs232GetSendBuf()[0] = 0xA0;
	pbRs232GetSendBuf()[1] = 4;
	pbRs232GetSendBuf()[2] = bV1;
	pbRs232GetSendBuf()[3] = bV2;
	pbRs232GetSendBuf()[4] = bV3;
	pbRs232GetSendBuf()[5] = bV4;

	while (bTNum)
	{
		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
		vIO32Write1B(PDWNC_DATAW, pbRs232GetSendBuf()[bTCnt++]);
		bTNum--;
	}

	//_fgRs232ISREnable = TRUE;
}

void LogB(UINT8 bVal)
{
	Log4B(0x55,0x55,0x55,bVal);
}

void LogW(UINT16 wVal)
{
	Log4B(0x55,0x55,bHiByte(wVal),bLoByte(wVal));
}

void LogD(UINT32 dVal) reentrant 
{
	Log4B(BYTE3(dVal), BYTE2(dVal), BYTE1(dVal), BYTE0(dVal));
}

void ISR_LogS(const char *pcStr) reentrant
{
	UINT8 bTNum, bTCnt;

	//_fgRs232ISREnable = FALSE;

	if (pcStr[0] != 0)
	{
		for(bTNum=0; (pcStr[bTNum]); bTNum++){};
		bTCnt = 0;

//		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
		vIO32Write1B(PDWNC_DATAW, 0xb0);
//		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
		vIO32Write1B(PDWNC_DATAW, bTNum);

		while (bTNum)
		{
//			while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
			vIO32Write1B(PDWNC_DATAW, (unsigned char)pcStr[bTCnt++]);
			bTNum--;
		}
	}

	//_fgRs232ISREnable = TRUE;
}
void ISR_Log4B(UINT8 bV1, UINT8 bV2, UINT8 bV3, UINT8 bV4) reentrant
{
	UINT8 bTNum, bTCnt;

	//_fgRs232ISREnable = FALSE;

	bTNum = 6;
	bTCnt = 0;
	pbRs232GetSendBuf()[0] = 0xA0;
	pbRs232GetSendBuf()[1] = 4;
	pbRs232GetSendBuf()[2] = bV1;
	pbRs232GetSendBuf()[3] = bV2;
	pbRs232GetSendBuf()[4] = bV3;
	pbRs232GetSendBuf()[5] = bV4;

	while (bTNum)
	{
//		while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
		vIO32Write1B(PDWNC_DATAW, pbRs232GetSendBuf()[bTCnt++]);
		bTNum--;
	}

	//_fgRs232ISREnable = TRUE;
}



void ISR_LogB(UINT8 bVal)
{
	ISR_Log4B(0x55,0x55,0x55,bVal);
}

void ISR_LogW(UINT16 wVal)
{
	ISR_Log4B(0x55,0x55,bHiByte(wVal),bLoByte(wVal));
}

void ISR_LogD(UINT32 dVal) reentrant
{
	ISR_Log4B(BYTE3(dVal), BYTE2(dVal), BYTE1(dVal), BYTE0(dVal));
}

#if RW_REG_INTERFACE

void _Print8bitsHex(UINT8 num) reentrant
{
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
     if( (num >> 4) > 9)
     {
         vIO32Write1B(PDWNC_DATAW, ( (num >> 4)-0xa) + 'a');
     }
     else
     {
         vIO32Write1B(PDWNC_DATAW, (num >> 4) + '0');
     }
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};     
     if((num & 0x0f) > 9)
     {
         vIO32Write1B(PDWNC_DATAW, ((num & 0x0f)-0xa) + 'a');
     }
     else
     {
         vIO32Write1B(PDWNC_DATAW, (num & 0x0f) + '0');
     }
}

void Print8bitsHex(UINT8 num) reentrant
{
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};     vIO32Write1B(PDWNC_DATAW,  '0');
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW,  'x');
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};     _Print8bitsHex(num);
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW, 0x0D);
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};    vIO32Write1B(PDWNC_DATAW, 0x0A);

}


void _LogS(const char *pcStr) reentrant
{
    UINT8 bTNum, bTCnt;

   // _fgRs232ISREnable = FALSE;

    if (pcStr[0] != 0)
    {
        for(bTNum=0; (pcStr[bTNum]); bTNum++){};
        bTCnt = 0;
/*
        while (!(u1RegRead1B(REG_STAB)&WR_ALLOW)){};
        vRegWrite1B(REG_DATAW,0xb0);
        while (!(u1RegRead1B(REG_STAB)&WR_ALLOW)){};
        vRegWrite1B(REG_DATAW,bTNum);
*/
        while (bTNum)
        {
            while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){};
            vIO32Write1B(PDWNC_DATAW,  (unsigned char)pcStr[bTCnt++]);
            bTNum--;
        }
    }
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; vIO32Write1B(PDWNC_DATAW,  0x0D);
    while (!(RegReadFldAlign(PDWNC_STAB, FLD_WR_ALLOW))){}; vIO32Write1B(PDWNC_DATAW,  0x0A);
   // _fgRs232ISREnable = TRUE;
}
#endif 

