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
 * $RCSfile: $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_demod.c
 *  DVBC demod related API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pi_demod.h"
#include "mt5135c_com_defs.h"
#include "pd_common.h"
#include "pd_API.h"
#include "code_img_8G80.h"


#include "ctrl_bus.h"  //add by liuqu,20090429
#include "spi_if.h"


extern UINT8	Reg_3A_15;
extern UINT8	Reg_3A_16;
extern UINT8	Reg_3A_17;

extern UINT8	Reg_3A_4B;
extern UINT8	Reg_3A_4C;
extern UINT8	Reg_3A_4D;
extern UINT8	Reg_3A_7E;
extern UINT8	Reg_3A_7F;

//-----------------------------------------------------------------------------
// Compile option define
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

static UINT8 Positive = 0;


#define fcLOAD_CODE_TIME        (1 && fcINTERNAL_TEST)
#define cLGS8G8X_DVBC_ADDRESS1  0x3A
#define cLGS8G8X_DVBC_ADDRESS2  0x3E
#define DVBC_SYNC_PER_THRESHOLD 2500

//-----------------------------------------------------------------------------
//static void  vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len);

// Configurations
//-----------------------------------------------------------------------------
#define fcMEAS_HOST_CMD_TIME    (0 && fcINTERNAL_TEST)
#define fcSKIP_LOAD_CODE        (0 && fcINTERNAL_TEST)
#define fcADD_HOSTCMD_CHKSUM    0

#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_DBG_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}\


UINT8 DVBC_ExDemodSetReg(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddr,UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
//UINT8 ucData;
  //  if(u1I2cAddr!=0x3E)
    {
		if(ICtrlBus_I2cDemodWrite(0x100, u1I2cAddr, u2RegAddr, pu1Buffer, u2ByteCount))
		{
			printf(" _@IF_IIC Bus Write Error: %02X-%03X!\n",
						u1I2cAddr, u2RegAddr);
	
			return 1;
		}
    }

	return 0;
}


UINT8 DVBC_ExDemodGetReg(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddr,UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
//UINT8 ucData;

   // if(u1I2cAddr!=0x3E)
	{
		if(ICtrlBus_I2cDemodRead(0x100, u1I2cAddr, u2RegAddr, pu1Buffer, u2ByteCount))
		{
			printf(" _@IF_IIC Bus Read Error: %02X-%03X!\n",
						u1I2cAddr, u2RegAddr);
	
			return 1;
		}
	}

	return 0;
}

/***********************************************************************/
/*                  Private (static) Function Prototypes                                                     */
/***********************************************************************/
//----------------------------------------------------------------------------- 
/*
 *  DVBC_ChipInit
 *  This function initialize DVB-C related registers in the chip.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_ChipInit(DEMOD_CTX_T *psDemodCtx)
{
////
    if (!psDemodCtx)
    {
       return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetNormalMode
 *  Set demod to normal mode.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_SetNormalMode(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 fgRet = 0;

    mcSHOW_DBG_MSG(("PI DVBC Set Normal Mode!!!\n"));
   
    return fgRet;
}
//----------------------------------------------------------------------------- 
/*
 *  DVBC_Connect
 *  Start demod module to acquire to a channel.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_Connect(DEMOD_CTX_T *psDemodCtx, UINT8 Mode, UINT16 Rate)
{
#if 0

    UINT8 fgRet = 0;
    mcSHOW_DBG_MSG(("PI DVBC Connect!!\n"));

    // Chip init
    DVBC_ChipInit(psDemodCtx);

    // stop driver
    DVBC_DriverStop(psDemodCtx);
    //mcDELAY_MS(10);

    fgRet = DVBC_SetNormalMode(psDemodCtx); 

    // stop driver
    //DVBT_DriverStop(psDemodCtx);   
    // set QAM Mode
    DVBC_SetQamMode(psDemodCtx, Mode);
	//set Symbol Rate
	DVBC_SetSymbolRate(psDemodCtx, Rate);
    // driver start
    DVBC_DriverStart(psDemodCtx); /*, REACQ_AUTO, FAST_SYNC_AUTO);*/

    //mcDELAY(5);
#endif

		UINT8 RegAddr, uValue;
		

		mcSHOW_DBG_MSG3(("PI DVBC Connect!!\n"));
		RegAddr = 0x14;
		ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
		uValue &= 0xFB;//enable GPIO1
		uValue &= 0xF7;//GPIO1 output login 0
		mcSHOW_DBG_MSG3(("Connect digital IF AGC GPIO1(0x14) = 0x%02x\n", uValue));
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
	

		
		//Enable IF AGC
		RegAddr = 0x07;
		if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 ))
		{
			mcSHOW_DBG_MSG3(("DVBC connect enable IFAGC read (0x07)fail!"));
			return 1;
		}
		else
		{
			mcSHOW_DBG_MSG1(("DVBC connect enable IFAGC read (0x07==0x%02x)!\n",uValue));
		}
	
		uValue |= 0x10;
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
		
		return 0;
		
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_DisConnect
 *  power down demod module.  
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 */
//-----------------------------------------------------------------------------
VOID DVBC_DisConnect(DEMOD_CTX_T *psDemodCtx)
{

UINT8 RegAddr, uValue;
	
	mcSHOW_DBG_MSG3(("PI DVBC Disconnect!!\n"));
	RegAddr = 0x14;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
	uValue &= 0xFB;//enable GPIO1
	uValue |= 0x08;//GPIO1 output login 1
	mcSHOW_DBG_MSG3(("Disconnect digital IF AGC GPIO1(0x14) = 0x%02x\n", uValue));
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );


	//disable IF AGC
	RegAddr = 0x07;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 ))
		mcSHOW_DBG_MSG3(("Disconnect disable IFAGC read (0x07)fail!");	)
	else
	{
		mcSHOW_DBG_MSG3(("Disconnect disable IFAGC read (0x07==0x%02x)!\n",uValue));
	}

	uValue &= 0xEF;
	mcSHOW_DBG_MSG3(("Disconnect disable IFAGC write (0x07==0x%02x)!\n",uValue));
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
			

}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_DemodCtxCreate
 *  This function creates demod context and returns the context pointer.
 *  @param  void.
 *  @retval   DEMOD_CTX_T * : pointer of demod context.   
 */
//-----------------------------------------------------------------------------
DEMOD_CTX_T *DVBC_DemodCtxCreate(void)
{
    DEMOD_CTX_T *p = NULL;

    CREATE_OBJECT(p, DEMOD_CTX_T);

    // error handling
    if (!p)
       return NULL;

    // zero memory
    memset(p, 0, sizeof(DEMOD_CTX_T));

    return p;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DemodCtxDestroy
 *  This function destroys context you have created and releases the memory.
 *  @param  p  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DemodCtxDestroy(DEMOD_CTX_T *p)
{
    DESTROY_OBJECT(p);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetDemodReg
 *  This function will read demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_GetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   ii = 0;

    if (!psDemodCtx)
        return 1;
    if ( (ucLen == 0) || (ucLen > 4) )
        return 1;
    if (!pucBuf)
        return 1;

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_REG;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucLen;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE( u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucLen - 1, ccOFST_REG1_SIZE+1))
    {
        for (ii = 0; ii < ucLen; ii++)
             *(pucBuf+ii) = mccGET_QUEUE(aucData, ccOFST_REG1_DATA+ii);
         return 0;
    }

    return 1;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetDemodReg
 *  This function will write demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_SetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   ii = 0;

    if (!psDemodCtx)
        return 1;
    if ( (ucLen == 0) || (ucLen > 4) )
        return 1;
    if (!pucBuf)
        return 1;

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_SET_REG;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucLen;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE( u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    for (ii = 0; ii < ucLen; ii++)
        mccSET_QUEUE2(aucData, ccOFST_REG1_DATA+ii) = *(pucBuf+ii);

    if (mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_REG1_SIZE+ucLen-1+1))
         return 0;

    return 1;
}

/***********************************************************************/
/*                  Private (static) Function Prototypes               */
/***********************************************************************/

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetReg
 *  This function set demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    //if (u1I2cDemWrite(psDemodCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))	 
    if(ICtrlBus_DemodWrite(psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG(("Demod Write Error: %02X!\n", u2RegAddr));
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetReg
 *  This function get demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    //if (u1I2cDemRead(psDemodCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    if(ICtrlBus_DemodRead(psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG((" Demod Read Error: %02X!\n", u2RegAddr));
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/** download image

 *  @param  
 *  @param  
 *  @retval   others : Error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_Loadcode(DEMOD_CTX_T *psDemodCtx, UINT8 *pu1Buffer, UINT16 u2CurRomLen)
{
	UINT16	u2CurI2cLen=1;// u2CurRamAdr=0;
		UINT32	ii;
		UINT8	uValue;
	
		uValue=0x00;
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x39, &uValue, u2CurI2cLen);//MCU off,MCU reset disable
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x3A, &uValue, u2CurI2cLen);//set address=0
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x3B, &uValue, u2CurI2cLen);
	
		for(ii=0; ii<u2CurRomLen; ii++)
		{	 
			uValue = (UINT8)ii;
			if(ICtrlBus_I2cDemodWrite(0x40, cLGS8G8X_DVBC_ADDRESS1, 0x3A, &uValue, u2CurI2cLen ))
			{
				/*fail case*/
				mcSHOW_DBG_MSG(("ucI2cWriteSpeed Download I2C fail(0x3A)!\n"));
				return 1;
			}
			
			uValue = (UINT8)(ii>>8);
			if(ICtrlBus_I2cDemodWrite(0x40, cLGS8G8X_DVBC_ADDRESS1, 0x3B, &uValue, u2CurI2cLen ))
			{
				/*fail case*/
				mcSHOW_DBG_MSG(("ucI2cWriteSpeed Download I2C fail(0x3B)!\n"));
				return 1;
			}
			if(ICtrlBus_I2cDemodWrite(0x40, cLGS8G8X_DVBC_ADDRESS1, 0x3C, pu1Buffer + ii, u2CurI2cLen ))
			{
				/*fail case*/
				mcSHOW_DBG_MSG(("ucI2cWriteSpeed Download I2C fail(0x3C)!\n"));
				return 1;
			}
			else
			{  //success
				//mcSHOW_USER_MSG((" %d value == 0x%02x\n", ii, *(pu1Buffer + ii)));
			}
			//u2CurRamAdr += u2CurI2cLen;
			//u2CurRomLen -= u2CurI2cLen;		
		}
	
		if(ii==u2CurRomLen)
		{
			printf("load code success @@@@@@@@@@@@@@@@@@@");
		}
		else
		{
			printf("load code fail ******************");
		}
	
		uValue=0x00;
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x39, &uValue, u2CurI2cLen);//MCU off,MCU reset disable
		uValue=0x02;
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x39, &uValue, u2CurI2cLen);//MCU on,MCU reset disable
		uValue=0x03;
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x39, &uValue, u2CurI2cLen);//MCU on,MCU reset enable


    return 0;
}

/************************************************************************/
#define cMAX_TRY_CNT            2
#define cMAX_WAIT_LOOP          10
//#define cWAIT_TIME_INTERVAL     200         // ms
#define cWAIT_TIME_INTERVAL     10         // ms

//----------------------------------------------------------------------------- 
/*
 *  fgHostCmd
 *  Host command interface API between host and demod.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  aucCmdData  Data buffer.
 *  @param  ucReadParacnt  Read parameter byte counts.
 *  @param  ucWriteCmdcnt  Write command byte counts.
 *  @retval   TRUE : Host command success.
 *  @retval   FALSE : Host command fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgHostCmd(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt)
{
UCHAR   ucWriteParacnt;
UINT16  ii;
UINT8   ucValue;
UCHAR   ucParaStart, ucRegSetOfst;
BOOL    fgRetStatus = TRUE;
#if fcMEAS_HOST_CMD_TIME
UINT32  u4TickStart;
UINT32  u4TickCnt;
#endif
UCHAR   ucReadChkCnt;
#if fcADD_HOSTCMD_CHKSUM
UCHAR   ucChkSum;
#endif
UCHAR   jj;

#if fcMEAS_HOST_CMD_TIME
    u4TickStart = mcGET_SYS_TICK();
#endif

    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucValue, 1))
    {
    //    psDemodCtx->u1I2cNakCnt ++;
        mcSHOW_HW_MSG((" _I2_ "));

        return FALSE;                       // I2C fail
    }
    if (!mcTEST_BIT(ucValue, ccpUP_ENABLE))
    {
        psDemodCtx->u1UpChkCnt ++;
    //    mcSHOW_HW_MSG(("uP not enabled!\n"));
        mcSHOW_HW_MSG((" _UE_ "));

        return FALSE;
    }
    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1))
    {
        return FALSE;                       // I2C fail
    }
    if (ucValue != ccFW_RUN_TAG)
    {
        psDemodCtx->u1UpChkCnt ++;
        if (ucValue == ccFW_WDT_TAG)
        {
        //    mcSHOW_HW_MSG(("Watchdog reset!\n"));
            mcSHOW_HW_MSG((" _WR_ "));
        }
        else
        {
        //    mcSHOW_HW_MSG(("Fw not running!\n"));
            mcSHOW_HW_MSG((" _FR_ "));
        }

        return FALSE;
    }   
    if (ucWriteCmdcnt)
    {
        ucWriteParacnt = ucWriteCmdcnt - 1;
        for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
        {
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
            // This while loop will cause dead-lock for Combo-Tuner (when I2C_Block = TRUE)
        #if 1
            jj = 0;
            while (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
            {
                DVBC_GetReg(psDemodCtx, DVBC_REG_RSICIF_RSP7, &ucValue, 1);
                DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
                jj ++;
                if (jj >= cMAX_WAIT_LOOP)
                    break;
            }
        #endif
            if (!mcTEST_BIT(ucValue, ccpUP_CMD_RX))
                break;
            mcDELAY_MS(cWAIT_TIME_INTERVAL);
        }

        #if fcMEAS_HOST_CMD_TIME
        u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
        mcSHOW_TIME_MSG((" ** Cmd%02X %d (%3u)", mccSET_CMD2(aucCmdData),
                                           ii, u4TickCnt * mcGET_TICK_PERIOD()));
        #endif

        if (ucWriteParacnt > ccCMD_REG_NUM - 1)
        {
            mcSHOW_HW_MSG(("Cmd %02X Para Cnt %d > %d!",
                                              mccSET_CMD2(aucCmdData), ucWriteParacnt, ccCMD_REG_NUM - 1));
            ucWriteParacnt = ccCMD_REG_NUM - 1;
        }
        #if fcADD_HOSTCMD_CHKSUM
        if (ucWriteParacnt == (ccCMD_REG_NUM - 1))
        {
            mcSHOW_HW_MSG(("Cmd %02X Para Cnt %d == %d!",
            mccSET_CMD2(aucCmdData), ucWriteParacnt, ccCMD_REG_NUM - 1));
        }
        else
        {
            ucChkSum = 0;
            for (ii = 0; ii < ccCMD_REG_NUM - 1; ii ++)
            {
                ucChkSum ^= aucCmdData[ccCMD_REG_NUM - 1 - ii + 1];
            }
            aucCmdData[1] = ucChkSum;
            ucWriteParacnt = ccCMD_REG_NUM - 1;
        }
        #endif

        if (ii == cMAX_WAIT_LOOP)
            psDemodCtx->u1UpNakCnt ++;

        ucParaStart = ccCMD_REG_NUM - ucWriteParacnt - 1;
        ucRegSetOfst = 1;

        DVBC_SetReg(psDemodCtx, (UINT16) (DVBC_REG_RSICIF_CMD0 + ucParaStart),
                                      aucCmdData + ucParaStart + ucRegSetOfst,
                                                          ucWriteParacnt + 1);
        #if fcMEAS_HOST_CMD_TIME
        u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
        mcSHOW_TIME_MSG((" Para %d (%3u)", ucWriteParacnt, u4TickCnt * mcGET_TICK_PERIOD()));
        #endif
        #if fcSHOW_HOST_CMD
        if (fgShowHostCmd)
        {
            //    mcSHOW_USER_MSG(("HostCmd: (%d) -->", ucParaStart));
            mcSHOW_USER_MSG(("HostCmd -->"));
            for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
            mcSHOW_USER_MSG(("\n           "));
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR00 + ucParaStart,
                                                         aucCmdData + ucParaStart + ucRegSetOfst,
                                                         ucWriteParacnt + 1);
            for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
            mcSHOW_USER_MSG(("\n"));
        }
        #endif
    }

    if (ucReadParacnt)
    {
        UCHAR   ucCmdId;

        ucCmdId = mccSET_CMD2(aucCmdData);
        fgRetStatus = FALSE;
        for (jj = 0; jj < cMAX_TRY_CNT; jj ++)
        {
            for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
            {
                DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
                if (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
                    break;
                mcDELAY_MS(cSHORT_SLEEP_TIME);
            }

            if (ucReadParacnt > ccCMD_REG_NUM - 1)
            {
                mcSHOW_HW_MSG(("Rsp %02X Para Cnt %d > %d!",
                                                  mccSET_CMD2(aucCmdData), ucReadParacnt, ccCMD_REG_NUM - 1));
                ucReadParacnt = ccCMD_REG_NUM - 1;
            }
            ucReadChkCnt = ucReadParacnt;
            #if fcADD_HOSTCMD_CHKSUM
            if (ucReadChkCnt < ccCMD_REG_NUM - 1)
                ucReadChkCnt ++;
            #endif
            if (ii < cMAX_WAIT_LOOP)
            {
                UINT16  u2AddrOfst = 0;

                #if fcMEAS_HOST_CMD_TIME
                u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
                mcSHOW_TIME_MSG((" Rsp %d_%d (%3u)", jj, ii, u4TickCnt * mcGET_TICK_PERIOD()));
                #endif
                ucParaStart = ccCMD_REG_NUM - ucReadChkCnt - 1;

                if (DVBC_GetReg(psDemodCtx,
                                            (UINT16) (DVBC_REG_RSICIF_RSP0 + u2AddrOfst + ucParaStart),
                                            aucCmdData + ucParaStart, ucReadChkCnt + 1))
                {
                    return FALSE;
                }

                #if fcMEAS_HOST_CMD_TIME
                u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
                mcSHOW_TIME_MSG((" Para %d (%3u)", ucReadChkCnt, u4TickCnt * mcGET_TICK_PERIOD()));
                #endif

                if (mccGET_CMD(aucCmdData) == ucCmdId)
                {
                    fgRetStatus = TRUE;
                    #if fcSHOW_HOST_CMD
                    if (fgShowHostCmd)
                    {
                        mcSHOW_USER_MSG(("HostRsp -->"));
                        for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                            mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                        mcSHOW_USER_MSG(("\n"));
                    }
                    #endif
                    
                    #if fcADD_HOSTCMD_CHKSUM
                    if (ucReadParacnt < ccCMD_REG_NUM - 1)
                    {
                        ucChkSum = 0;
                        for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                        {
                            ucChkSum ^= aucCmdData[ccCMD_REG_NUM - 1 - ii];
                        }
                        if (ucChkSum)
                        {
                            fgRetStatus = FALSE;
                            mcSHOW_HW_MSG((" _@CS_ "));
                            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_RSPR00 + ucParaStart,
                                                   aucCmdData + ucParaStart, ucReadChkCnt + 1);
                            mcSHOW_USER_MSG(("-->"));
                            for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                            mcSHOW_USER_MSG(("\n"));
                            psDemodCtx->u1UpMisCnt ++;
                        }
                    }
                    #endif
                    break;
                }
                else
                    psDemodCtx->u1UpMisCnt ++;
            }
            else
            {
                psDemodCtx->u1UpNakCnt ++;
                break;
            }
        }
        //    mcSHOW_HW_MSG(("Error uP Rsp!"));
    }

    #if fcMEAS_HOST_CMD_TIME
    u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
    //    mcSHOW_DBG_MSG((" %3uH ", u4TickCnt * mcGET_TICK_PERIOD()));
    mcSHOW_TIME_MSG((" %3uH ", u4TickCnt * mcGET_TICK_PERIOD()));
    mcSHOW_TIME_MSG(("\n"));
    #endif

    return fgRetStatus;
}

//----------------------------------------------------------------------------- 
/*
 *  fgDVBCGetFwVars
 *  Get firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgDVBCGetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_VAR;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucCnt;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    //mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = 0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucCnt - 1, ccOFST_REG1_SIZE + 1))
    {
        for (ii = 0; ii < ucCnt; ii ++)
            pucData[ii] = mccGET_QUEUE(aucData, ccOFST_REG1_DATA + ii);
    }
    else
        return FALSE;
    return TRUE;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetDvbtNotExist
 *  Check if really no DVB-C signal exists.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE  : It's confirmed that no DVB-C signal exists.   
 *  @retval   FALSE : DVB-C signal exists or it's not confirmed yet.
 */
//-----------------------------------------------------------------------------
BOOL DVBC_GetDvbtNotExist(DEMOD_CTX_T *psDemodCtx)
{
	if (!psDemodCtx)
		return FALSE;	
	
	if ((psDemodCtx->u1ControlStatus > 4) && (psDemodCtx->ucMdFftMode == 3))
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------- 
/*
 *  vDVBCGetInfo
 *  Get demod information and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void vDVBCGetInfo(DEMOD_CTX_T *psDemodCtx)
{
UINT8	uValue;


	psDemodCtx->u2TotalPkt=0;

	//stop pkt counter
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );
	uValue &= 0xEF;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );

	//get total pkt count
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x2B, &uValue, 1 );
	psDemodCtx->u2TotalPkt |= uValue;
	psDemodCtx->u2TotalPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x2A, &uValue, 1 );
	psDemodCtx->u2TotalPkt |= uValue;
	psDemodCtx->u2TotalPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x29, &uValue, 1 );
	psDemodCtx->u2TotalPkt |= uValue;
	psDemodCtx->u2TotalPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x28, &uValue, 1 );
	psDemodCtx->u2TotalPkt |= uValue;

	//get error pkt count
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x2F, &uValue, 1 );
	psDemodCtx->u2ErrorPkt |= uValue; 
	psDemodCtx->u2ErrorPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x2D, &uValue, 1 );
	psDemodCtx->u2ErrorPkt |= uValue; 
	psDemodCtx->u2ErrorPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x2E, &uValue, 1 );
	psDemodCtx->u2ErrorPkt |= uValue; 
	psDemodCtx->u2ErrorPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x2C, &uValue, 1 );
	psDemodCtx->u2ErrorPkt |= uValue; 

	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );
	uValue |= 0x10;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );

    //clear BCH error count 
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );
	uValue |= 0x08;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );
	uValue &= 0xF7;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );
	
    mcSHOW_DBG_MSG2(("u2ErrorPkt = %d, u2TotalPkt=%d\n ", psDemodCtx->u2ErrorPkt,psDemodCtx->u2TotalPkt));
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_CheckLocked
 *  Check if DVB-C signal locked.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE  : It's confirmed that no DVB-C signal exists.   
 *  @retval   FALSE : DVB-C signal exists or it's not confirmed yet.
 */
//-----------------------------------------------------------------------------

//UINT8 DVBC_CheckLocked(UINT8* pPolarity, UINT32 Interval, UINT32 waitTimes _LGS_DEVICEHANDLE_)
UINT8 DVBC_CheckLocked(DEMOD_CTX_T *psDemodCtx)
{
UINT8 Reg07 = 0;
UINT8 isLocked = 0;

	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x07, &Reg07, 1 );

	if(!Positive)
	{
		Reg07 |= 0x04;
		Positive = 1;
	}
	else
	{
		Reg07 &= ~0x04;
		Positive = 0;
	}

	mcSHOW_USER_MSG(("Positive = %d\n",Positive));

	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x07, &Reg07, 1 );

	mcDELAY_MS(500);
	
	vDVBCGetInfo(psDemodCtx);
	
	if(DVBC_GetPER(psDemodCtx)<= DVBC_SYNC_PER_THRESHOLD)
	{
		isLocked = 1;
	}
	
	//mcSHOW_USER_MSG(("Locked check = %d\n",isLocked));

	if(isLocked==0)
	{
		return DVBC_DEMOD_STATE_UNLOCK;
	}
	
	return DVBC_DEMOD_STATE_LOCK;
	
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSync
 *  Get demod lock status.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   DVBC_DEMOD_STATE_UNLOCK : Demod unlock.
 *  @retval   DVBC_DEMOD_STATE_LOCK   : Demod locked.   
 *  @retval   DVBC_DEMOD_STATE_UNKNON : Demod status unknown.
 */
//-----------------------------------------------------------------------------

UINT8 DVBC_GetSync(DEMOD_CTX_T *psDemodCtx)
{
    #if 0
    UINT8 u1DvbcDemodState = DVBC_DEMOD_STATE_UNKNOWN;
    DVBC_UpdateInfo(psDemodCtx);    
    DVBC_UpdateStatus(psDemodCtx); // update SC state, FFT & TS lock
    if (DVBC_GetDvbtNotExist(psDemodCtx))
    {
        u1DvbcDemodState = DVBC_DEMOD_STATE_UNLOCK;
    }
    else if (DVBC_GetTsLock(psDemodCtx)) 
    {
        u1DvbcDemodState = DVBC_DEMOD_STATE_LOCK;
    }
	#endif


	UINT8 u1DvbcDemodState = DVBC_DEMOD_STATE_UNKNOWN;
#if 0
	UINT16 LoopCntManSwitch = 50;

		for( ; LoopCntManSwitch > 0; LoopCntManSwitch--)
		{
		    mcDELAY_MS(200);
			vDVBCGetInfo(psDemodCtx);
			if(DVBC_GetPER(psDemodCtx)<= DVBC_SYNC_PER_THRESHOLD)
			{
				u1DvbcDemodState = DVBC_DEMOD_STATE_LOCK;
				mcSHOW_DBG_MSG2(("PER is smaller than threshold\n"));
				break;
			}
			else
			{
				u1DvbcDemodState = DVBC_DEMOD_STATE_UNLOCK;
				mcSHOW_DBG_MSG2(("Current PER is heavy\n"));
			}						
		}	
#endif

	u1DvbcDemodState = DVBC_CheckLocked(psDemodCtx);
    mcSHOW_DBG_MSG2(("Get Sync Status = %d\n",u1DvbcDemodState));

	return u1DvbcDemodState;

}

//----------------------------------------------------------------------------- 
/*
 *  ucGetFwVar
 *  Get firmware one-byte variable.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @retval   1-byte data.   
 */
//-----------------------------------------------------------------------------
UINT8 ucGetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{
    UCHAR   ucValue = 0;

    fgDVBCGetFwVars(psDemodCtx, u2Addr, &ucValue, 1);
    return ucValue;
}
//----------------------------------------------------------------------------- 
/*
 *  fgDVBCSetFwVars
 *  Set firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgDVBCSetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_SET_VAR;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucCnt;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    //mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
        mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = 0;
    for (ii = 0; ii < ucCnt; ii ++)
        mccSET_QUEUE2(aucData, ccOFST_REG1_DATA + ii) = pucData[ii];
    //if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucCnt - 1, ccOFST_REG1_SIZE + 1))
    if (mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_REG1_SIZE + ucCnt -1 + 1))
    {
        return TRUE;
    }
    return FALSE;
}

//----------------------------------------------------------------------------- 
BOOL DVBC_FwSwitch(DEMOD_CTX_T *psDemodCtx)
{
#if defined(CC_MT5135) || defined(CC_MT5135DVBC)
    UCHAR ucValue;

    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG07, &ucValue, 1);
    if(0x01 != ucValue)
    {
        UCHAR aucData[cMAX_READ_NUM + 1], ii;

        mccSET_CMD2(aucData) = ccCMD_ID_DVBTC_SWITCH;
        mccSET_QUEUE2(aucData, ccOFST_DVBTC_SWITCH) = cCONN_TYPE_DVBC;//0x01:DVBC, 0x02: DVBT
        mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBTC_SWITCH_SIZE+1);

        // delay for waiting FW initialization done, may have I2C timeout at first
        for (ii = 0; ii < 5; ii++)
        {
            mcDELAY_MS(3);
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
            if (ucValue == ccFW_RUN_TAG)
                break;
        }
        mcSHOW_DBG_MSG2(("FW switch to DVBT!"));
    }
#endif

    return TRUE;
}

//-----------------------------------------------------------------------------
VOID DVBC_TSParallelSerialSwitch(DEMOD_CTX_T *psDemodCtx, BOOL IsSerial)
{
    UCHAR ucValue = 0x00;
    if(IsSerial)
    {
        /* Setting TS Output Serial Mode */
        DVBC_GetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucValue , 1);
        ucValue = ucValue & 0x0b;
        DVBC_SetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucValue , 1);
    }
    else
    {
        /* Setting TS Output Parallel Mode */
        DVBC_GetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucValue , 1);
        ucValue = ucValue | 0x04;
        DVBC_SetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucValue , 1);		
    }
}
/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************/
//----------------------------------------------------------------------------- 
/*
 *  s4LoadCode
 *  Download firmware code.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u4I2cClkDiv  Unused.
 *  @param  u1CodeImage  Pointer of fimware code image.
 *  @param  u2CurRomLen  Firmware code size in byte.
 *  @param  u2MaxI2cLen  Maximum I2C length per transfer.
 *  @param  fgClk50  Flag to switch if 48MHz or 24MHz clock is used.
 *  @retval   cUP_LOAD_OK : Load code OK.
 *  @retval   cUP_LOAD_ERR_I2C : I2C error.
 *  @retval   cUP_LOAD_ERR_HW_RDY : HW not ready.
 *  @retval   cUP_LOAD_ERR_CHKSUM_RDY : Checksum not ready.
 *  @retval   cUP_LOAD_ERR_CHKSUM_OK : Checksum not OK.   
 */
//-----------------------------------------------------------------------------
INT32 s4LoadCode(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage,
                 UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50)
{

#if fcLOAD_CODE_TIME
    TIME_TICK_T u4TickStart, u4TickCur;
    TIME_DIFF_T u4TickCnt;
#endif


// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    ii = 0;
#endif


    /*ucI2cAddr = psDemodCtx->I2cAddress;*/
    if (DVBC_Loadcode(psDemodCtx, u1CodeImage, u2CurRomLen))
    {
        mcSHOW_POP_MSG(("Download FW fail! (%d)\n"));
        return 1;
    }

#if fcLOAD_CODE_TIME
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
    u4TickCnt = mcCONV_SYS_TIME(u4TickCnt);
    mcSHOW_DBG_MSG((" (%3u ms) ", u4TickCnt));
    if (u4TickCnt > 0)
        return -((INT32) u4TickCnt);
    else
#endif

    return cUP_LOAD_OK;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DriverStart
 *  Start demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u1Mon1Sel  Monitor1 driver select. (0: None, 1: Auto-Reacquisition).
 *  @param  u1Mon2Sel  Monitor2 driver select. (0: None, 1: TPS sync, 2: SPS sync auto, 3: SPS sync manual
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DriverStart(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_REACQ_START;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_REACQ_START_RSIZE, 1);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DriverStop
 *  Stop demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DriverStop(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_REACQ_STOP;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_REACQ_STOP_RSIZE, 1);
}

UINT8 DVBC_SetIFFreq(DEMOD_CTX_T *psDemodCtx,UINT16 Ifreq)//(IF freq with KHz uint)
{
	return 1;//default setting is ok
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_Initialize
 *  Initialize demod context.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucI2cAddress  Demod I2C address.
 *  @param  fgLoadCode  Flag to check if need to downlaod firmware code. Unused.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 ucI2cAddress, BOOL fgLoadCode)
{

	//SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

	psDemodCtx->u2ErrorPkt = 0;
    psDemodCtx->u2TotalPkt = 0;
    psDemodCtx->u4CorrBit = 0;
    psDemodCtx->ucEnFpga = FALSE;
    psDemodCtx->I2cAddress = ucI2cAddress;
    psDemodCtx->ucConnType = PORT_CFG_MTK;
    psDemodCtx->ucTsIfFormat = 0x8C;   //changed from 0x0C to 0x8C due to MT5360B, and no impact to MT539x and MT5360A
    psDemodCtx->ucTsIfCtrl = 0x00;
    psDemodCtx->ucTsIfRate = 0x07;
    psDemodCtx->ucSramSize = 32;
    psDemodCtx->m_TunerMode = 0;
	psDemodCtx->i4BerTh = 430;   // JC, 090202


	#if 0
    DVBC_ChipInit(psDemodCtx);

    if (fgLoadCode)
    {
        if (s4LoadCode(psDemodCtx, ucDvbcCodeImage, sizeof(ucDvbcCodeImage), cMAX_I2C_LEN, TRUE))
        {
            return (DRVAPI_TUNER_ERROR);
        }

        // Set TS IF
        DVBC_SetTsIf(psDemodCtx, psDemodCtx->ucTsIfFormat, psDemodCtx->ucTsIfCtrl, psDemodCtx->ucTsIfRate);

        // AGC crrve setting
        ucPara[0] = ccCMD_ID_SET_TUNER_PARA0;
        for (ii = 0; ii < 4; ii++)
            ucPara[ii+1] = pEuTunerCtx->m_aucPara[ii+2];  // write m_aucPara[2-5]
        vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TUNER_PARA0_SIZE+1);

        // set agc parameters
        ucPara[0] = ccCMD_ID_SET_TUNER_PARA1;
        ucPara[1] = pEuTunerCtx->m_aucPara[7];   // IF taget level, 080826, JC
        ucPara[2] = 0x7F;//(psDemodCtx->sTunerCtx.m_aucPara[6] & 0x3F) | 0x40;            // kp, ki; LF_CTRL
        ucPara[3] = mcLOW_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);   // IF_FREQ_LOW
        ucPara[4] = mcHIGH_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);  // IF_FREQ_HIGH
        vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TUNER_PARA1_SIZE+1);

        // set to blind search mode
        //vSetChSearchMode(psDemodCtx); /* Zhang Chi mark, and will be added in the later time */

        // Ken, GetVersion for previous host command complete, 090114
        mccSET_CMD2(aucData) = ccCMD_ID_GET_VER;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_VER_SIZE, 1))
        {
            mcSHOW_USER_MSG(("DVBC_Initialize OK!\n"));
        }
    }     
	#endif

	UINT8 uValue;

    mcSHOW_USER_MSG(("Before Download FW!!!!!!!!!!##########%%%%%%%%%%\n"));
    if (fgLoadCode)
    {
    
        if (s4LoadCode(psDemodCtx, ucDvbcCodeImage, sizeof(ucDvbcCodeImage), cMAX_I2C_LEN, TRUE))
        {
            return (DRVAPI_TUNER_ERROR);
        }
        
    }  
    mcSHOW_USER_MSG(("After Download FW!!!!!!!!!!###############&&&&&&&&&&\n"));

	#if 1
    uValue=0x11;//seclect internal ADC type:
    if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x07, &uValue, 1 ))
	{
        /*fail case*/
		mcSHOW_USER_MSG(("ucI2cWriteSpeed Download I2C fail(0x07)!\n"));
		return 1;
    }
	#endif
    #if 1
	//set serial when open
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );
    uValue |= 0x01;
    ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x30, &uValue, 1 );
    #endif	
	
	//set IF freq
	if(!DVBC_SetIFFreq(psDemodCtx,36167))//IF freq is 36.167MHz
	{
		mcSHOW_USER_MSG(("Set IF freq fail!\n"));
		return 1;
	}
	
	return (DRVAPI_TUNER_OK);
	
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTotPkt
 *  Get total TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval  (UINT32)total TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DVBC_GetTotPkt(DEMOD_CTX_T *psDemodCtx)  
{
    return (UINT32)psDemodCtx->u2TotalPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetErrPkt
 *  Get error TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval  (UINT32)error TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DVBC_GetErrPkt(DEMOD_CTX_T *psDemodCtx)  
{
    return (UINT32)psDemodCtx->u2ErrorPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetPER
 *  Get TS packet error rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (INT32) PER*10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPER(DEMOD_CTX_T *psDemodCtx)  // return value = PER*10^5
{
    INT32 RSTotPkt, RSErrPkt;
        
    RSTotPkt = (INT32)psDemodCtx->u2TotalPkt;
    RSErrPkt = (INT32)psDemodCtx->u2ErrorPkt;

    if (RSTotPkt <= 0)
        return 100000;
    else
        return (INT32) ((RSErrPkt * 100000) / RSTotPkt);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTsLock
 *  Get TS lock.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL DVBC_GetTsLock(DEMOD_CTX_T *psDemodCtx)
{
    if (psDemodCtx->ucRSHState)
        psDemodCtx->fgTsLock = TRUE;
    else
        psDemodCtx->fgTsLock = FALSE;
    
    return psDemodCtx->fgTsLock;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSignalLevel
 *  Get signal level from 0 (lowest) to 100 (highest).
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (UINT8) 0~100.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetSignalLevel(DEMOD_CTX_T *psDemodCtx)
{
UINT8 uValue, s2SignalStrength;

	uValue = 0x0D;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS2, 0xFF, &uValue, 1 );
	uValue = 0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS2, 0x54, &uValue, 1 );
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS2, 0x55, &s2SignalStrength, 1 );


	if( (psDemodCtx->u2TotalPkt < 7000) || (psDemodCtx->u2ErrorPkt>100) )
		s2SignalStrength = 0;
	else if( (psDemodCtx->u2ErrorPkt!=0)&&(psDemodCtx->u2ErrorPkt< 30)&&(psDemodCtx->u2TotalPkt > 8000) )
	    s2SignalStrength = 2;
	else
		s2SignalStrength += 5;

	mcSHOW_DBG_MSG2(("DVBC SignalStrength = %d", s2SignalStrength));

	return s2SignalStrength;


}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSignalSNR
 *  Get signal SNR.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (UINT8) SNR in dB.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetSignalSNR(DEMOD_CTX_T *psDemodCtx)
{
    INT16 s2_data;

    if (psDemodCtx->fgTsLock == TRUE)
    {
        s2_data = (psDemodCtx->s2Snr) / 256;
        if (s2_data < 0)
            return 0;
        else
            return (UINT8) s2_data;
    }
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  vSetIntfPara
 *  Set host command interface API.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  para  Pointer of parameters buffer. (para[0] : CmdId, para[1~(len-1)] : parameters).
 *  @param  len  Length of parameters
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
#if 0
static void vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR   ii;
    
    mccSET_CMD2(aucData) = para[0];
    if (len > 1)
    {
        for (ii = 0; ii < (len-1); ii++)
            mccSET_QUEUE2(aucData, ii) = para[ii+1];
    }
    
    mcHOST_CMD(psDemodCtx, aucData, 0, len);
}
#endif
//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetTsIf
 *  Set TS interface parameters.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  tsif_format  TS interface format.
 *  TsIfFormat:
 *  b7  Reserve
 *  b6  VALID   Active polarity selection of tsif_val
 *      0 -> tsif_val is high active
 *      1 -> tsif_val is low active
 *  b5  ERR Active polarity selection of tsif_err
 *      0 -> tsif_err is high active
 *      1 -> tsif_err is low active
 *  b4  SYNC    Active polarity selection of tsif_sync
 *      0 -> tsif_sync is high active
 *      1 -> tsif_sync is low active
 *  b3  TEI Transport stream error indicator
 *      0 -> TEI bit will not be changed by tsif
 *      1 -> TEI bit will automatic set by tsif 
 *  b2  SP  Parrallel / Serial mode selection
 *      0 -> Serial mode
 *      1 -> Parralle mode
 *  b1  CLK_GATED   Output clock gated selection of tsif_clk
 *      0 -> tsif_clk is gated
 *      1 -> tsif_clk is non-gated
 *  b0  CLK_INV Output clock edge selection of tsif_clk
 *      0 -> tsif_data is valid at positive edge clock
 *      1 -> tsif_data is valid at negative edge clock
 *  @param  tsif_ctrl  TS interface control.
 *  TsIfControl:
 *  b7  Reserve
 *  b6  FRACTIONAL_PKT  Allow MPEG fractional packet generated
 *      0 -> Output packet is always 188 bytes
 *      1 -> Output packet may not be 188 bytes when channel changed
 *  b5  SERIAL_SYNC  Control the width of TSIF_SYNC signal. Only valid in serial mode
 *      0 -> SYNC duration = first 8 bit
 *      1 -> SYNC duration = first 1 bit
 *  b4  DISABLE_HDR  Disable output header 'h47
 *      0 -> Normal
 *      1 -> Disable header 'h47 in TS
 *  b3  DATA_FLIP  Control data flip of TSIF_DATA[7:0]
 *      0 -> Normal
 *      1 -> Data flip of TSIF_DATA[7:0]
 *  b2  ERR_DROP_PKT  Drop packet if error packet occur
 *      0 -> Normal
 *      1 -> Drop packet if TSIF_ERR = 1'b1
 *  b1  ERR_SHORT  Define TSIF_ERR duration
 *      0 -> Normal, TSIF_ERR duration is 188 bytes
 *      1 -> TSIF_ERR duration is 1 byte
 *  b0  ERR_GATED  Define TSIF_ERR gated
 *      0 -> TSIF_ERR is gated by data enable
 *      1 -> TSIF_ERR is not gated between packet
 *  @param  tsif_rate  TS interface clock rate. Output data rate = (48MHz/(TSIF_RATE+1)). The value should be odd number.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate)
{
}

//----------------------------------------------------------------------------- 
/** DVBC_SetTsOutputMode
 *  Set TS output mode
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel)
{
UINT8 RegAddr,uValue;
	//set TS serial/paral
	
	RegAddr=0x30;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
	mcSHOW_DBG_MSG(("DTMB_SetTsOutputMode: fgParallel = %d\n", fgParallel));
	mcSHOW_DBG_MSG(("DTMB_SetTsOutputMode: read 0x30 = 0x%02X\n", uValue));


	if (fgParallel)
	uValue &= 0xFE;//0 is parallel and 1 is serial
	else
	uValue |= 0x01; 

	mcSHOW_DBG_MSG(("DTMB_SetTsOutputMode: write 0x30 = 0x%02X\n", uValue));	
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );

}

//----------------------------------------------------------------------------- 
/** DVBC_SetTsPathMode
 *  Set TS path mode
 *  @param  fgEnable  set TS path to tri-state or pass-through. TRUE: pass-through, FALSE: tri-state
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTsPathMode(DEMOD_CTX_T *psDemodCtx, BOOL fgEnable)
{
UINT8 uValue, RegAddr;

	RegAddr=0x14;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
	
	mcSHOW_DBG_MSG(("DVBC_SetTsPathMode: fgEnable = %d\n", fgEnable));
	mcSHOW_DBG_MSG(("DVBC_SetTsPathMode: read 0x14 = 0x%08X\n", uValue));
	
	if (fgEnable)
		uValue &= 0x7F;
	else
		uValue |= 0x80;
	
	mcSHOW_DBG_MSG(("DVBC_SetTsPathMode: write 0x14 = 0x%08X\n", uValue));
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
//#endif


}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetPostVBer
 *  Get post Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Post Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPostVBer(DEMOD_CTX_T *psDemodCtx) // return value = PostVBer * 10^5
{
    INT32 RSTotPkt, RSErrPkt, RSCorrBit;

    if (!psDemodCtx)
        return -1;

    // PostVBer = (RSCorBit + RSErrPkt * ERR_BITS_PER_ERR_PKT) / (RSTotPkt *188*8)
    // 66 = 10^5 / (188*8)
    
    RSTotPkt = (INT32)psDemodCtx->u2TotalPkt;
    RSErrPkt = (INT32)psDemodCtx->u2ErrorPkt;
    RSCorrBit = (INT32)psDemodCtx->u4CorrBit;
    
    if (RSTotPkt <= 0)
        return (-1);
    else
        return ( (RSCorrBit + RSErrPkt*ERR_BITS_PER_ERR_PKT) * 66 / RSTotPkt );
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetConnType
 *  Set connection type.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  type  Connection type. (Default is 0).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DVBC_SetConnType(DEMOD_CTX_T *psDemodCtx, UINT8 type)
{
    psDemodCtx->ucConnType = type;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetPowerMode
 *  Set power mode.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucMode  Unused.
 *  @retval   0. 
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_SetPowerMode(DEMOD_CTX_T *psDemodCtx, UCHAR ucMode)
{
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DumpRegisters
 *  Dump Demod registers.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  bank_no  Register bank number.
 *  @param  max_i2c_len  Max I2C read byte at one time
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_DumpRegisters(DEMOD_CTX_T *psDemodCtx, UINT8 *buffer, UINT8 bank_no, UINT16 max_i2c_len)
{
UINT16  u2CurI2cLen, u2CurAddr, u2StartAddr = 0;
UINT16  u2CurLen = 256;

    if (!psDemodCtx)
        return 1;
    if (!buffer)
        return 1;
    if (bank_no > 2)
        return 1;

    u2CurAddr = 0;
    u2StartAddr = (UINT16) bank_no << 8;
    while (u2CurLen)
    {
        u2CurI2cLen = u2CurLen;
        if (u2CurI2cLen > max_i2c_len)
            u2CurI2cLen = max_i2c_len;

        if (DVBC_GetReg(psDemodCtx, u2StartAddr + u2CurAddr,
                                   buffer + u2CurAddr, u2CurI2cLen))
        {
            return 1;
        }
        u2CurAddr += u2CurI2cLen;
        u2CurLen  -= u2CurI2cLen;
    }

    return 0;
}


//----------------------------------------------------------------------------- 
/** DVBC_SetQamMode
 *  Set QAM Mode.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  Mode  QAM mode selection.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetQamMode(DEMOD_CTX_T *psDemodCtx, UINT8 Mode)
{
UINT8 Reg19, uValue;

	mcSHOW_USER_MSG(("Mode = %d\n",Mode));

	uValue = 0x20;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x19, &uValue, 1 );
	uValue = 0x80;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x1C, &uValue, 1 );
	uValue = 0xcb;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x08, &uValue, 1 );
	uValue = 0x6b;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x09, &uValue, 1 );
	uValue = 0x28;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x0a, &uValue, 1 );
	uValue = 0x2f;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x0b, &uValue, 1 );

	if(Mode == constellation_qam256_dvbc)
	{
		mcSHOW_USER_MSG(("enter 256QAM Mod\n"));
		//uValue = 0x50;
		uValue = 0x00;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x15, &uValue, 1 );
		//uValue = 0x43;
		uValue = 0x42;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x16, &uValue, 1 );
		uValue = 0x6a;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x17, &uValue, 1 );
		uValue = 0xa4;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x4b, &uValue, 1 );
		uValue = 0x00;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x4c, &uValue, 1 );
		uValue = 0x80;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x4d, &uValue, 1 );
		uValue = 0x01;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x7e, &uValue, 1 );
		uValue = 0x30;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x7f, &uValue, 1 );

		uValue = 0x0C;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xFF, &uValue, 1 );
		uValue = 0x9B;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x66, &uValue, 1 );
		uValue = 0x9B;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x68, &uValue, 1 );
		uValue = 0x35;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x70, &uValue, 1 );
		uValue = 0x35;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x72, &uValue, 1 );

	}
	else
	{
	    mcSHOW_USER_MSG(("enter other QAM Mod\n"));
		uValue = Reg_3A_15;
		//mcSHOW_USER_MSG(("Reg_3A_15=0x%02X\n",Reg_3A_15));
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x15, &uValue, 1 );
		uValue = Reg_3A_16;
		//mcSHOW_USER_MSG(("Reg_3A_16=0x%02X\n",Reg_3A_16));
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x16, &uValue, 1 );
		uValue = Reg_3A_17;
		//mcSHOW_USER_MSG(("Reg_3A_17=0x%02X\n",Reg_3A_17));
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x17, &uValue, 1 );
		uValue = Reg_3A_4B;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x4b, &uValue, 1 );
		uValue = Reg_3A_4C;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x4c, &uValue, 1 );
		uValue = Reg_3A_4D;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x4d, &uValue, 1 );
		uValue = Reg_3A_7E;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x7e, &uValue, 1 );
		uValue = Reg_3A_7F;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x7f, &uValue, 1 );

		uValue = 0x0C;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xFF, &uValue, 1 );
		uValue = 0x9B;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x66, &uValue, 1 );
		uValue = 0x9B;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x68, &uValue, 1 );
		uValue = 0x35;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x70, &uValue, 1 );
		uValue = 0x35;
		ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x72, &uValue, 1 );
	}

	uValue = 0x0a;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xff, &uValue, 1 );
	uValue = 0x02;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xb0, &uValue, 1 );
	uValue = 0x2c;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xb3, &uValue, 1 );
	uValue = 0x00;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xb2, &uValue, 1 );
	uValue = 0x2b;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xb4, &uValue, 1 );
	uValue = 0x2c;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xb7, &uValue, 1 );
	uValue = 0x00;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xb6, &uValue, 1 );
	uValue = 0x00;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xb9, &uValue, 1 );
	uValue = 0x0a;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xb8, &uValue, 1 );
	uValue = 0x03;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xbb, &uValue, 1 );
	uValue = 0x03;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xba, &uValue, 1 );

	uValue = 0x0c;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xff, &uValue, 1 );
	uValue = 0x38;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x22, &uValue, 1 );
	uValue = 0xe0;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x42, &uValue, 1 );
	uValue = 0xe0;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x4c, &uValue, 1 );
	uValue = 0x38;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x24, &uValue, 1 );
	uValue = 0xe0;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x44, &uValue, 1 );
	uValue = 0xe0;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x4e, &uValue, 1 );
	uValue = 0x0d;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x26, &uValue, 1 );

	uValue = 0xf0;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x46, &uValue, 1 );
	uValue = 0xf0;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x50, &uValue, 1 );
	uValue = 0xf0;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x48, &uValue, 1 );
	uValue = 0xf0;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x52, &uValue, 1 );

	uValue = 0x38;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x20, &uValue, 1 );
	uValue = 0x0a;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x2a, &uValue, 1 );
	uValue = 0x0a;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x2c, &uValue, 1 );
	uValue = 0x0a;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x2e, &uValue, 1 );
	uValue = 0x0a;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x30, &uValue, 1 );
	uValue = 0x64;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x1a, &uValue, 1 );
	uValue = 0x1f;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x1C, &uValue, 1 );
	uValue = 0x41;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x00, &uValue, 1 );

	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x19, &Reg19, 1 );
	Reg19 &= 0xC7;

	switch(Mode)
	{
		case constellation_qam16_dvbc:
		{
			Reg19 |= 0x10;
			break;
		}
		case constellation_qam32_dvbc:
		{
			Reg19 |= 0x18;
			break;
		}
		case constellation_qam64_dvbc:
		{
			Reg19 |= 0x20;
			break;
		}
		case constellation_qam128_dvbc:
		{
			Reg19 |= 0x28;
			break;
		}
		case constellation_qam256_dvbc:
		{
			Reg19 |= 0x30;
			
			uValue = 0x0c;
			ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0xff, &uValue, 1 );
			uValue = 0x0a;
			ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS2, 0x02, &uValue, 1 );
			break;
		}
		default:
		{
			break;
		}
	}

	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x19, &Reg19, 1 );

}
//----------------------------------------------------------------------------- 
/** DVBC_SoftReset
 *  Reset FW in MCU of demod.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SoftReset(DEMOD_CTX_T *psDemodCtx)
{
UINT8 uValue;

	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x02, &uValue, 1);
	uValue = uValue & 0xFE;

	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x02, &uValue, 1);
	uValue = uValue | 0x01;

	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, 0x02, &uValue, 1);

}
//----------------------------------------------------------------------------- 
/** DVBC_SetSymbolRate
 *  Set Symbol Rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  Rate  symbol rate in kSym/sec.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetSymbolRate(DEMOD_CTX_T *psDemodCtx, UINT16 Rate)
{
UINT8 Reg07, Reg10, Reg11, Reg12, Reg1C;
UINT32	RatioValue;

	mcSHOW_USER_MSG(("Rate = %d\n",Rate));

	if ( (Rate > 999) && (Rate <= 1899) )
	{
		RatioValue = (1900 - Rate) * 4194304;
		RatioValue /= Rate;

		Reg10 = (UINT8)((UINT32)RatioValue & 0xff);
		Reg11 = (UINT8)(((UINT32)RatioValue>>8) & 0xff);
		Reg12 = (UINT8)(((UINT32)RatioValue>>16) & 0xff);
		Reg12 |= 0x80;

		ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x1C, &Reg1C, 1 );
		Reg1C &= 0xFC;
		Reg1C |= 0x02;
	}
	else if ( (Rate > 1899) && (Rate <= 3799) )
	{
		RatioValue = (3800 - Rate) * 4194304;
		RatioValue /= Rate;

		Reg10 = (UINT8)((UINT32)RatioValue & 0xff);
		Reg11 = (UINT8)(((UINT32)RatioValue>>8) & 0xff);
		Reg12 = (UINT8)(((UINT32)RatioValue>>16) & 0xff);
		Reg12 |= 0x80;

		ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x1C, &Reg1C, 1 );
		Reg1C &= 0xFC;
		Reg1C |= 0x01;
	}
	else if ( (Rate > 3799) && (Rate <= 7600) )
	{
		RatioValue = (7600 - Rate) * 4194304;
		RatioValue /= Rate;

		Reg10 = (UINT8)((UINT32)RatioValue & 0xff);
		Reg11 = (UINT8)(((UINT32)RatioValue>>8) & 0xff);
		Reg12 = (UINT8)(((UINT32)RatioValue>>16) & 0xff);
		Reg12 |= 0x80;

		ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x1C, &Reg1C, 1 );
		Reg1C &= 0xFC;
	}
	else
	{

	}

	mcSHOW_DBG_MSG3(("Reg10 = 0x%02x, Reg11 = 0x%02x, Reg12 = 0x%02x, Reg1C = 0x%02x, \n",Reg10,Reg11,Reg12,Reg1C));
	
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x10, &Reg10, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x11, &Reg11, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x12, &Reg12, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x1C, &Reg1C, 1 );

	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_DVBC_ADDRESS1, 0x07, &Reg07, 1 );

	#if 0
	switch(Polarity)
	{
		case 0:
		{
			Reg07 |= 0x04;
			break;
		}
		case 1:
		{
			Reg07 &= ~0x04;
			break;
		}
		default:
		{
			break;
		}
	}
	#endif
	//ICtrlBus_I2cDemodWrite( 0x100, LGS8G8XSECADDR1, 0x07, Reg07, 1 );

	//LGS_Wait(1000);
	DVBC_SoftReset(psDemodCtx);


}

//----------------------------------------------------------------------------- 
/** DVBC_AcqStart
 *  Start Acquistion.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_AcqStart(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_ACQ_START;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_ACQ_START_RSIZE, 1);
}

//----------------------------------------------------------------------------- 
/** DVBC_TunerI2c
 *  Switch Tuner I2C.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  fgSwitchOn  Switch ON/OFF of tuner I2C
 *  @retval   S16.
 */
//-----------------------------------------------------------------------------
S16 DVBC_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn)
{
UINT8	uValue, RegAddr;

    if (!psDemodCtx)
        return -2;

	if (fgSwitchOn) // // Access Tuner
	{
		uValue = 0xE1;//tuner i2c addrss is 0xC2
		RegAddr = 0x01;
		if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 ))
		{
		    mcSHOW_DBG_MSG3(("Detach Write I2C fail(enable tuner)!\n"));	
		}
		else
		{
			mcSHOW_DBG_MSG3(("Detach Write I2C success(enable tuner)!\n"));	
		}
	}
	else
	{
		uValue = 0x61;
		RegAddr = 0x01;
		if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 ))
		{
		    mcSHOW_DBG_MSG3(("Detach Write I2C fail(disable tuner)!\n"));
		}
		else
		{
			mcSHOW_DBG_MSG3(("Detach Write I2C success(disable tuner)!\n"));	
		}	
	}

	return 0;
}

//----------------------------------------------------------------------------- 
/** DVBC_UpdateStatus
 *  Get Status.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateStatus(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   ucData[2];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_STATUS0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_STATUS0_SIZE, 1))
    {
        ucData[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS00);
        ucData[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS01);
        
        psDemodCtx->ucPraLock     = mcGET_FIELD(ucData[0], 0x03, 0);
        psDemodCtx->ucQNIRState   = mcGET_FIELD(ucData[0], 0x04, 2);
        psDemodCtx->ucQTRState    = mcGET_FIELD(ucData[0], 0x08, 3);
        psDemodCtx->ucQCRFftState = mcGET_FIELD(ucData[0], 0x10, 4);
        psDemodCtx->ucQCRState    = mcGET_FIELD(ucData[0], 0x20, 5);
        psDemodCtx->ucRSHState    = mcGET_FIELD(ucData[0], 0x40, 6);
    }
}

//----------------------------------------------------------------------------- 
/** DVBC_UpdateInfo
 *  Get Info.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateInfo(DEMOD_CTX_T *psDemodCtx)
{

	vDVBCGetInfo(psDemodCtx);

#if 0
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   uc_data[4];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO0_SIZE, 1))
    {
        //corrected error bits
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT2);
        psDemodCtx->u4CorrBit = uc_data[0];
        psDemodCtx->u4CorrBit += (U32)mcUNSIGN_2BYTE(uc_data[2], uc_data[1], 7, 8) * 256;
        //total error packets
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT1);
        psDemodCtx->u2ErrorPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8); 
        //total packets
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT1);
        psDemodCtx->u2TotalPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO1;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO1_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_AGC_GAIN_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_AGC_GAIN_H);
        psDemodCtx->s2AgcGain = mcSIGN_2BYTE(uc_data[1], uc_data[0], 6, 8);   // Gain * 2^13

        psDemodCtx->ucQamMode = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_QAM_MODE);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_H);
        psDemodCtx->u2SymbolRate = mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);

        psDemodCtx->ucDsmSel = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_DSM_SEL);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO2;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO2_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET2);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET3);

        psDemodCtx->s4Cfo = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        psDemodCtx->s4Cfo += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 5, 8) * 256 * 256;

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_EQ_SNR0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_EQ_SNR1);
        psDemodCtx->s2Snr = mcSIGN_2BYTE(uc_data[1], uc_data[0], 7, 8);   // SNR * 256

        psDemodCtx->u1Bw = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_QCR_DD_BW);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO3;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO3_SIZE, 1))
    {
        psDemodCtx->ucSysCtrlState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_SYSC);
        psDemodCtx->ucQcnState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QCN);
        psDemodCtx->ucQcrReturnRC = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QCR_RETURN_RC);
        psDemodCtx->ucFftSearchResult = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QTR_FFT_SEARCH_RESULT);
        psDemodCtx->ucPraState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_PRA);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_BN_MONITOR);
        psDemodCtx->ucBNflag = mcGET_FIELD(uc_data[0], 0x40, 6);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO4;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO4_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_1);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_2);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_3);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_4);

        psDemodCtx->s4TimingOffset = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        psDemodCtx->s4TimingOffset += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 2, 8) * 256 * 256;
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO5;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO5_SIZE, 1))
    {
        psDemodCtx->ucASR_CFOflag = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CONFIDENCE);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO2);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO3);

        psDemodCtx->s4ASR_CFO = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        psDemodCtx->s4ASR_CFO += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 2, 8) * 256 * 256;
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO6;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO6_SIZE, 1))
    {
        psDemodCtx->ucAQM_Seq[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_00);
        psDemodCtx->ucAQM_Seq[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_01);
        psDemodCtx->ucAQM_Seq[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_02);
        psDemodCtx->ucAQM_Seq[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_03);
        psDemodCtx->ucAQM_Seq[4] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_04);
        psDemodCtx->ucAQM_ID = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_Index);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO7;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO7_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_SpecInv_QCN_State);
        psDemodCtx->u1QSpecInv = mcGET_FIELD(uc_data[0], 0x80, 7);
        psDemodCtx->ucQcnState = uc_data[0] & 0x07;

        psDemodCtx->ucSysCtrlState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_SYSC_State);
        psDemodCtx->ucCurrentAcqState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_ucCurAcqState);
        psDemodCtx->ucDev_AWGN_PN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uDev_AWGN_PN);
        psDemodCtx->ucDev_AWGN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uDev_AWGN);
        psDemodCtx->ucCor_AWGN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uCor_AWGN);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO8;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO8_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_JTD_00);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_JTD_01);
        psDemodCtx->u2JTD_Result = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_LockTime_00);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_LockTime_01);
        psDemodCtx->u2AcqTime = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO9;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO9_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_STATUS_MONITOR);
        psDemodCtx->ucSldPraState = mcGET_FIELD(uc_data[0], 0x60, 5);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_RFAGC_CTRL_WORD_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_RFAGC_CTRL_WORD_L);
        psDemodCtx->s2SldPraRfagcCtrlWord = mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 5);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_IFAGC_CTRL_WORD_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_IFAGC_CTRL_WORD_L);
        psDemodCtx->s2SldPraIfagcCtrlWord = mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 5);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_PGA_GAIN_INDEX);
        psDemodCtx->ucSldPraGainIdx = mcGET_FIELD(uc_data[0], 0x1f, 0);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFOA;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFOA_SIZE, 1))
    {
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFOA_SLD_PRA_MAG_REF_OUT_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFOA_SLD_PRA_MAG_REF_OUT_L);
        psDemodCtx->s2SldPraOut = mcSIGN_2BYTE(uc_data[1], uc_data[0], 3, 8);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFOA_LEGACY_PRAD_Cur_Level_sf_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFOA_LEGACY_PRAD_Cur_Level_sf_L);
        psDemodCtx->s2LegPraOut = mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
    }
#endif
}

//----------------------------------------------------------------------------- 
/** DVBC_GetCFO
 *  Get CFO.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   S32 CFO in kHz * 10^5.
 */
//-----------------------------------------------------------------------------
S32 DVBC_GetCFO(DEMOD_CTX_T *psDemodCtx)
{
    S32 s4Cfo_value;

    s4Cfo_value = psDemodCtx->s4Cfo * 10;   // (reg/2^29*54*1000) * 10^5

    return s4Cfo_value;
}

//----------------------------------------------------------------------------- 
/** DVBC_GetTFO
 *  Get TFO.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   UINT32 TFO in ppm.
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetTFO(DEMOD_CTX_T *psDemodCtx)
{
#if 0
    UINT32 u4Tmp;
    INT32  s4TFO;

    u4Tmp = (U32)1 << psDemodCtx->ucDsmSel;
    s4TFO = (INT32)((double) psDemodCtx->s4TimingOffset / (1024.0*1024.0*2.0) * 
            (double) psDemodCtx->u2SymbolRate / 1000.0 * (double)u4Tmp / 27 * 1000000.0);
#endif

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTsFmt
 *  Get demod TS output format.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
TS_FMT_T DVBC_GetTsFmt(DEMOD_CTX_T *psDemodCtx)
{
TS_FMT_T fgTsFmt;
UINT8 RegAddr, uValue;

	RegAddr=0x30;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );

	if (uValue & 0x01)
		fgTsFmt.SerOrPar = SP_SERIAL;
	else
		fgTsFmt.SerOrPar = SP_PARALLEL;

	if (uValue & 0x04)
		fgTsFmt.fgGatedClock = BOOL_FALSE;
	else
		fgTsFmt.fgGatedClock = BOOL_TRUE;

	if (uValue & 0x02)
		fgTsFmt.ClockPolarity = POL_LOW_ACTIVE;
	else
		fgTsFmt.ClockPolarity = POL_HIGH_ACTIVE;

	RegAddr=0x14;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_DVBC_ADDRESS1, RegAddr, &uValue, 1 );
	if (uValue & 0x80)
		fgTsFmt.fgTristate = BOOL_TRUE;
	else
		fgTsFmt.fgTristate = BOOL_FALSE;

	//TS clock unknow now
	fgTsFmt.u4TsClockInKHz = 0;
#if 0
	DVBT_GetReg(psDemodCtx, 0x201, &ucClk, 1);
	if (ucData & 0x04)
	{
		/* Parallel */
		fgTsFmt.u4TsClockInKHz = 54000/(ucClk+1);
	}
	else
	{
		/* Serial */
		fgTsFmt.u4TsClockInKHz = 54000*8/(ucClk+1);
	}
#endif

	fgTsFmt.fgExternalDemod = BOOL_TRUE;
	mcSHOW_DBG_MSG3(("SerOrPar=%d, GatedClock=%d, Tristate=%d\n",fgTsFmt.SerOrPar,fgTsFmt.fgGatedClock,fgTsFmt.fgTristate))

	return fgTsFmt;

}

