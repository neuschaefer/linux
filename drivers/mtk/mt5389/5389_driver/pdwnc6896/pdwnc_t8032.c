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
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#include "x_bim_mt82.h"
#include "x_ckgen_mt82.h"
//#include "x_os.h"
#else
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_os.h"
#endif
#include "drvcust_if.h"
#include "x_lint.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "pdwnc_config.h"
#include "pdwnc_debug.h"
#include "pdwnc_entry.h"
#include "pdwnc_t8032.h"
#include "pdwnc_gpio.h"
#include "pdwnc_drvif.h"
#include "ir_if.h"

#include "x_timer.h"

//#undef PDWNC_WRITE32
//#undef PDWNC_READ32

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifdef CC_UP8032_ATV
typedef UINT32  SIZE_T;
typedef UINT32  HANDLE_T;
#define CC_MTK_LOADER
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


#define T8032_ACK_TIMEOUT 2000
#define T8032_ALIVE_CHECK_RETRY 3// 100
#define T8032_ALIVE_CHECK_INTERVAL 10 // ms
#define T8032_MSGQ_CAPACITY 16

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
//extern void HAL_Delay_us(UINT32 u4Micros);
#ifdef CC_UP8032_ATV
extern UINT32 DRVCUST_IrrxGet(QUERY_TYPE_T eQryType);
#endif
//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
//INT32 _PDWNC_SetT8032(UINT32 u4State);
//INT32 PDWNC_InitT8032(UINT32 u4Addr, UINT32 u4Size);
//INT32 PDWNC_T8032Cmd(PDWNC_T8032_CMD_T *prCmd, PDWNC_T8032_RESPONSE_T *prResponse);
//INT32 PDWNC_ReadSwVersion(UINT32* pu4SwVersion);
#ifndef CC_MTK_LOADER
INT32 PDWNC_WaitLastTransactionFin(void);
INT32 PDWNC_SetLastTransactionFin(void);
INT32 PDWNC_CheckLastTransactionRequest(void);
INT32 PDWNC_SetLastTransactionRequest(void);
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static HANDLE_T _hCECLastTransactionFinMsgq;
static HANDLE_T _hCECStartLastTransactionMsgq;
static HANDLE_T _hT8032IsrMsgq;
/*static*/ HANDLE_T _hT8032CmdMutex;
#endif
///*static*/ UINT8 _au1CecData[CEC_XDATA_SIZE];


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef CC_UP8032_ATV //copy by risc
static INT32 _CopyToT8032(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{//for instruction ram copy
    UINT32 i;
  
    volatile UINT32* pu1Ptr = (volatile UINT32*)u4RiscAddr;
    vIO32Write4B(PDWNC_UP_ADDR, u4XAddr);
    for(i = 0; i < u4Size; i+=4)
    {
        vIO32Write4B(PDWNC_UP_DATA, *pu1Ptr);
        pu1Ptr++;
    }    
    return 0;
}

/*static*/ INT32 _CopyToT8032_XDATA(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{//for xdata copy
//though we can only write one byte data into xdata with bus operation, we still need to use DW access for the bus operation! 
    UINT32 i;
    volatile UINT8* pu4Ptr = (volatile UINT8*)u4RiscAddr;
    vIO32Write4B(PDWNC_UP_ADDR, u4XAddr);
    for(i = 0; i < u4Size; i++)
    {
        vIO32Write4B(PDWNC_UP_DATA, *pu4Ptr);
        pu4Ptr++;
    }
    return 0;
}




static INT32 _CopyFromT8032(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{
    UINT32 i;
    volatile UINT32* pu1Ptr = (volatile UINT32*)u4RiscAddr;

    vIO32Write4B(PDWNC_UP_ADDR, u4XAddr);

    for(i = 0; i < u4Size; i+=4)
    {
        *pu1Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        pu1Ptr++;  
    }   
    return 0;
}


/*static*/ INT32 _CopyFromT8032_XDATA(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{
    UINT32 i;
    volatile UINT8* pu1Ptr = (volatile UINT8*)u4RiscAddr;

    vIO32Write4B(PDWNC_UP_ADDR, u4XAddr);

    for(i = 0; i < u4Size; i++)
    {
//        *pu1Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        pu1Ptr++;  
    }   
    return 0;
}

static INT32 _LoadCode(UINT32 u4Addr, UINT32 u4Size)
{
    if(u4Addr == 0)
    {
        return -1;
    }
    return _CopyToT8032(0x0, u4Size, u4Addr);
}
#endif
static INT32 _T8032Cmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse)
{
    INT32 i4Ret = 0;
    UINT8 u1Cmd, u1SubCmd;
    UINT32 u4Data;
#ifndef CC_MTK_LOADER  
	UINT16 i2MsgNum;  
    UINT16 u2Idx = 0;
    SIZE_T u4Size = sizeof(UINT32);
    UINT32 u4Msg;
#endif    

    u4Data = DRVCUST_InitGet(eT8032uPOffset);
#if 0
    if (u4Data == 0)  // here needs to setup 8032uP.
    {
        Printf("Warning! T8032 is not active!");
        return 0;
    }
    //UNUSED(u4Data);
#endif
    VERIFY(prCmd != NULL);
	
    u1Cmd = prCmd->u1Cmd;
    u1SubCmd = prCmd->u1SubCmd;
    u4Data = prCmd->au1Data[0] | (prCmd->au1Data[1] << 8) | (prCmd->au1Data[2] << 16) | (prCmd->au1Data[3] << 24);    
     vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
#ifndef CC_MTK_LOADER
//	 vIO32Write4B(PDWNC_SHREG8, u1Cmd | (u1SubCmd << 8));
        PDWNC_T8032CMD_SET_CMD(u1Cmd | (u1SubCmd << 8));
//	 vIO32Write4B(PDWNC_SHREG9, u4Data);
        PDWNC_T8032CMD_SET_DATA(u4Data);
		do{
			VERIFY(x_msg_q_num_msgs(_hT8032IsrMsgq, &i2MsgNum) == OSR_OK);
			if(i2MsgNum != 0)
			{
				Printf("Warning! _hT8032IsrMsgq is not empty");
				VERIFY(x_msg_q_receive_timeout(&u2Idx, &u4Msg, &u4Size, &_hT8032IsrMsgq, 1, T8032_ACK_TIMEOUT) == OSR_OK);
				Printf("...flush Msg(%d)\n", u4Msg);				
			}
		}while(i2MsgNum != 0);        
    	vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);

    // If the command is not ENTER_STANDBY, we need ACK 
    if(u1Cmd != PDWNC_CMD_ARMT8032_ENTER_STANDBY)
    {
#ifndef CC_UP8032_ATV		
        if (x_msg_q_receive_timeout(&u2Idx, &u4Msg, &u4Size, &_hT8032IsrMsgq, 1, T8032_ACK_TIMEOUT) != OSR_OK)
        {
            Printf("T8032 interrupt error: ack timeout!(Cmd = 0x%x, SubCmd = 0x%x)\n", u1Cmd, u1SubCmd);

            // Ack timeout
            return -2;
        }
#endif
        if((u4Msg & 0xff) == u1Cmd)
        {
            if(prResponse != NULL)
            {
                prResponse->u1Ack = (UINT8)(u4Msg & 0xff);
                prResponse->au1Data[0] = (UINT8)((u4Msg >> 8) & 0xff);
                prResponse->au1Data[1] = (UINT8)((u4Msg >> 16) & 0xff);
                prResponse->au1Data[2] = (UINT8)((u4Msg >> 24) & 0xff);            
            }
        }
        else
        {      
            Printf("T8032 interrupt error: ack mismatch!(Cmd = 0x%x, SubCmd = 0x%x, Ack = %d)\n", u1Cmd, u1SubCmd, (UINT8)(u4Msg & 0xff));
            // Ack mismatch                    
            i4Ret = -1;                
        }
        
    }
#else    
//    vIO32Write4B(PDWNC_SHREG8, u1Cmd | ((UINT32)u1SubCmd << 8));
    PDWNC_T8032CMD_SET_CMD(u1Cmd | ((UINT32)u1SubCmd << 8));
//    vIO32Write4B(PDWNC_SHREG9, u4Data);
    PDWNC_T8032CMD_SET_DATA(u4Data);
    // If the command is not ENTER_STANDBY, we need ACK 
    if(u1Cmd != PDWNC_CMD_ARMT8032_ENTER_STANDBY)
    {           
        // Wait until T8032 enter non-xdata mode
        UINT32 u4Val = 0;        
        UINT32 i = 0, j = 0;
        
        while(1)
        {          
            vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
            
            while(i < 10000)
            {
                HAL_Delay_us(100);                  
                if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))
                {
                    u4Val = PDWNC_T8032CMD_GET_RESPONSE();
                    if((u4Val & 0xff) == u1Cmd)
                    {
                        if(prResponse != NULL)
                        {
                            prResponse->u1Ack = (UINT8)(u4Val & 0xff);
                            prResponse->au1Data[0] = (UINT8)((u4Val >> 8) & 0xff);
                            prResponse->au1Data[1] = (UINT8)((u4Val >> 16) & 0xff);
                            prResponse->au1Data[2] = (UINT8)((u4Val >> 24) & 0xff);            
                        }                            
                    }
                    else
                    {      
                        Printf("T8032 polling error: ack mismatch!(Cmd = 0x%x, SubCmd = 0x%x, Ack = %d)\n", u1Cmd, u1SubCmd,(UINT8)(u4Val & 0xff));
                        // Ack mismatch
                        i4Ret = -1;                
                    }
                                       
                    break;                
                }      
                i++;
            }
                                 
            if(i < 10000)
            {
                	vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
                break;
            }            
            else
            {
                	vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
                i = 0;
            }
            
            if(++j > 2 /*T8032_ACK_TIMEOUT*/)
            {
                Printf("T8032 polling error: ack timeout!(Cmd = 0x%x, SubCmd = 0x%x, Ack = %d)\n", u1Cmd, u1SubCmd,(UINT8)(u4Val & 0xff));
                // Ack timeout
                i4Ret = -2;
                break;
            }
          
        }
    }
    else
    {
        vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
    }
#endif    
    
    return i4Ret;    

}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**  is the interrupt routine for GPIO pins of PDWNC module.
 *  @param u2Gpio is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER  
void _PDWNC_T8032Isr(UINT16 u2Vector)
{
    UINT32 u4Val;
    UINT32 u4Size = sizeof(UINT32); 
    UINT16 u2Num;
    	
#ifndef CC_UP8032_ATV		
    x_msg_q_num_msgs (_hT8032IsrMsgq, &u2Num);
#endif    
    if(u2Num >= T8032_MSGQ_CAPACITY)
    {
        return;
    }
    
    u4Val = PDWNC_T8032CMD_GET_RESPONSE();//u4IO32Read4B(PDWNC_SHREG0);
#ifndef CC_UP8032_ATV		
    x_msg_q_send(_hT8032IsrMsgq, &u4Val, u4Size, 100);    
#endif    

    // Clear by PDWNC_HwIsr
    // PDWNC_WRITE32(REG_RW_INT, AUXINT_CLR);
    
}
//-----------------------------------------------------------------------------
/**  PDWNC_SetT8032 set T8032 to the specified state.
 *  @param u4State T8032 state to set
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_RegT8032Isr(void)
{
    void (* pfnOldIsr)(UINT16);
#ifndef CC_UP8032_ATV
    VERIFY(x_msg_q_create(&_hT8032IsrMsgq, "t8032msg", sizeof(UINT32), 
T8032_MSGQ_CAPACITY) == OSR_OK);    
    VERIFY(x_msg_q_create(&_hCECStartLastTransactionMsgq, "ceclststart", sizeof(UINT32), T8032_MSGQ_CAPACITY) == OSR_OK);
    VERIFY(x_msg_q_create(&_hCECLastTransactionFinMsgq, "ceclstfin", sizeof(UINT32), T8032_MSGQ_CAPACITY) == OSR_OK);    
    VERIFY(x_sema_create(&_hT8032CmdMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);    
    // Hook T8032 ISR
//    VERIFY(PDWNC_RegIsr((UINT16)PINTNO_T8032, _PDWNC_T8032Isr, &pfnOldIsr) == PDWNC_OK);    
#endif
    VERIFY(PDWNC_RegIsr((UINT16)PDWNC_INTNO_AUX_INT, _PDWNC_T8032Isr, &pfnOldIsr) == PDWNC_OK);    
    return 0;
}
#endif

/*
INT32 _PDWNC_TransferCECData(UINT32 u4Direction)
{    
#ifndef CC_UP8032_ATV // need to proting in risc
    if(u4Direction == PDWNC_CEC_DATA_TO_T8032)
    {
        return _CopyToT8032_XDATA(XDATA_ADDR + CEC_XDATA_OFFSET, CEC_XDATA_SIZE, (UINT32)_au1CecData);
    }
    else
    {
        return _CopyFromT8032_XDATA(XDATA_ADDR + CEC_XDATA_OFFSET, CEC_XDATA_SIZE, (UINT32)_au1CecData); 
    }
#else
	return TRUE;
#endif
	
}
*/
//-----------------------------------------------------------------------------
/**  PDWNC_SetupCEC to setup CEC wakeup before entering standby.
 *  @param void
 */
//-----------------------------------------------------------------------------

INT32 _PDWNC_SetupCEC(UINT32 u4XAddr, UINT32 u4Size, UINT8* pu1Data)
{

    INT32 i4Ret = 0;

#ifndef CC_MTK_LOADER
    // Request CEC driver to stop CEC ISR handling after current transaction finish
/*    
    PDWNC_SetLastTransactionRequest();
    VERIFY(PDWNC_WaitLastTransactionFin() == OSR_OK);
*/    
#endif

#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
    x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif
#endif

    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM);

    // Copy CEC data to XData
    i4Ret |= _CopyToT8032_XDATA(u4XAddr, u4Size, (UINT32)pu1Data);//_PDWNC_TransferCECData(PDWNC_CEC_DATA_TO_T8032);

    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032);


#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
    x_sema_unlock(_hT8032CmdMutex);
#endif
#endif
    
    return i4Ret; 
}

#ifdef CC_MT8223
static UINT8 CEC_ARRAY[0x100];
INT32 _PDWNC_GetT8302CECData(UINT32 u4XAddr, UINT32 u4Size, UINT8* pu1Data)
{

    INT32 i4Ret = 0;
	PDWNC_T8032_CMD_T rCmd;
	PDWNC_T8032_RESPONSE_T rResp;
	UINT32 u4DramAddr;
	UNUSED(u4XAddr);
	UNUSED(pu1Data);
	
	//Printf("address of CEC_ARRAY is 0x%08x\n",CEC_ARRAY);
	//for(u4DramAddr=30;u4DramAddr<0x30;u4DramAddr++)
		//CEC_ARRAY[u4DramAddr] = 0xff;
	u4DramAddr = (UINT32)(CEC_ARRAY)&0x0fffffff;
	Printf("PHYSICAL(CEC_ARRAY) is 0x%08x\n",u4DramAddr);
    rCmd.u1Cmd = PDWNC_T8032_COPY_CEC_DATA_TO_DRAM;
	if(u4Size>=256)		
		rCmd.u1SubCmd = 0;
	rCmd.au1Data[0] = BYTE0(u4DramAddr); // will be byte3 in t8032
	rCmd.au1Data[1] = BYTE1(u4DramAddr);
	rCmd.au1Data[2] = BYTE2(u4DramAddr);
	rCmd.au1Data[3] = BYTE3(u4DramAddr);

	i4Ret |= PDWNC_T8032Cmd(&rCmd,&rResp);
	//Printf("Response is Ack: 0x%2x, Data0: 0x%2x, Data1: 0x%2x, Data2: 0x%2x\n", rResp.u1Ack, rResp.au1Data[0],rResp.au1Data[1],rResp.au1Data[2]);

	x_memcpy((void *)pu1Data,(void *)CEC_ARRAY,u4Size);
    return i4Ret; 
}
#else
INT32 _PDWNC_GetT8302CECData(UINT32 u4XAddr, UINT32 u4Size, UINT8* pu1Data)
{

    INT32 i4Ret = 0;
#if 0
david
#ifndef CC_MTK_LOADER
    // Request CEC driver to stop CEC ISR handling after current transaction finish
    PDWNC_SetLastTransactionRequest();
    VERIFY(PDWNC_WaitLastTransactionFin() == OSR_OK);
#endif
#endif

#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
    x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif
#endif
#ifdef CC_MT8223
	IO_WRITE8(PDWNC_BASE, 0x140, 0x7);//for 8223 load code to srm
#endif
    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM);
#ifdef CC_MT8223
				IO_WRITE8(PDWNC_BASE, 0x140, 0x7);//for 8223 load code to srm
#endif

    // Copy CEC data to XData
    i4Ret |= _CopyFromT8032_XDATA(u4XAddr, u4Size, (UINT32)pu1Data);//_PDWNC_TransferCECData(PDWNC_CEC_DATA_TO_ARM);

    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032);

#ifdef CC_MT8223
				IO_WRITE8(PDWNC_BASE, 0x140, 0x0);//for 8223 load code to srm
#endif
#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
    x_sema_unlock(_hT8032CmdMutex);
#endif
#endif
    
    return i4Ret; 
}

#endif


#ifdef CC_UP8032_ATV
INT32 PDWNC_StopT8032(void)
{

    INT32 i4Ret = 0;

    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM);

    return i4Ret; 
}
#endif
//-----------------------------------------------------------------------------
/**   _PDWNC_RequestT8032TurnOffSys request T8032 to entern standby mode
 */
//-----------------------------------------------------------------------------
static INT32 _PDWNC_RequestT8032TurnOffSys(void)
{
    INT32 i4Ret = 0;
    PDWNC_T8032_CMD_T rCmd;
	UINT32 i;
   // x_memset(&rCmd, 0, sizeoof(PDWNC_T8032_CMD_T));
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;
			if(IO32ReadFldAlign(PDWNC_UP_CFG, FLD_XDATA_ACC))
			{
				vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC); //let T8032 control xdata
			}
//david : Switch operation of  CEC, UART, ENGINE are not necessary in MT5387/63
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_VIR_EN);
        
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENTER_STANDBY;
#ifdef CC_MTK_LOADER
        rCmd.u1SubCmd = 0x1; // AC_ON->Standby
#else
        rCmd.u1SubCmd = 0x2; // AC_ON->Normal->Standby
#endif
        
        // Signal 8032 to enter standby
        i4Ret = _T8032Cmd(&rCmd, NULL);

        LOG(7, "Signal T8032 to write power down entry code!\n");     
		for(i=0;i<T8032_ACK_TIMEOUT;i++)
		{
       		 HAL_Delay_us(1000);
		}
  
        return i4Ret;
}

//-----------------------------------------------------------------------------
/**  PDWNC_SetT8032 set T8032 to the specified state.
 *  @param u4State T8032 state to set
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_SetT8032(UINT32 u4Set)
{
    INT32 i4Ret = 0;
    UINT32 u4Val;   
    PDWNC_T8032_CMD_T rCmd;
	
	x_memset((void *)&rCmd, 0, sizeof(PDWNC_T8032_CMD_T));
	
    // PDWNC_T8032_RESPONSE_T rResponse;    
    if(u4Set == PDWNC_T8032_SET_STANDBY)
    {//david: this path will not go through OS API (x_sema_lock)
        return _PDWNC_RequestT8032TurnOffSys();
    }    
#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
    x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT);
#endif
#endif
    
    if(u4Set == PDWNC_T8032_SET_XDATA_ARM)    
    {       
        u4Val = u4IO32Read4B(PDWNC_UP_CFG);
        
        // If T8032 is inactive (reset)             
        if(u4Val & T8032_RST)
        {
            LOG(7, "XDATA belongs to ARM already!\n");                        
#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
            x_sema_unlock(_hT8032CmdMutex);
#endif
#endif             
            return 0;
        }
        // If T8032 is active (reset released)    
        else
        {        
          rCmd.u1Cmd = PDWNC_CMD_ARMT8032_XDATA_SWITCH;
          rCmd.u1SubCmd = 0x1;
#ifdef CC_MT8223
#else
          MSET_XDATA_TO_ARM_FLAG();
#endif
          //Printf("Set XDATA to ARM!\n");
          // Signal 8032 to change to non-xdata mode
          _T8032Cmd(&rCmd, NULL);
            // Really set xdata access right to ARM
#if 0  //david          
					vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_T8032_RST); //accert T8032 reset
					vIO32WriteFld(PDWNC_UP_CFG, 0x0, FLD_VIR_EN); //blocking T8032 to access code ram
#endif						
					vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
					LOG(7, "Switch XDATA to ARM!\n");            
        }

    }
    else if(u4Set == PDWNC_T8032_SET_XDATA_T8032)
    {
#if 0     
        u4Val = u4IO32Read4B(PDWNC_UP_CFG);

        // If T8032 is inactive (reset)        
        if(u4Val & T8032_RST)
        {
            PDWNC_T8032_CMD_T rCmd;
            rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;
						vIO32WriteFld(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to T8032
						vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_VIR_EN); //grant T8032 to access code ram
						vIO32WriteFld(PDWNC_UP_CFG, 0x0, FLD_T8032_RST); //deactive T8032 reset		
            // Set T8032 to normal state
            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENTER_NORMAL;
            rCmd.u1SubCmd = 2; // notmal stage
            _T8032Cmd(&rCmd, NULL);
            LOG(5, "T8032 change to normal stage...\n");                         
            LOG(7, "Switch XDATA to T8032!\n");            
        }
        // If T8032 is active (reset released)     
        else
        {
            // Because X-data and instrution sram cannot be assigned seperated
            LOG(7, "XDATA belongs to T8032 already!\n");                        
            return 0;
        }
#else
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to T8032
#ifdef CC_MT8223
#else
            MCLR_XDATA_TO_ARM_FLAG();
#endif
            LOG(7, "Switch XDATA to T8032!\n");    
#endif
        
    }    	
    else if(u4Set == PDWNC_T8032_SET_ENGINE_ARM)//useless in MT5387
    {
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENGINE_SWITCH;
        rCmd.u1SubCmd = 0x1;

        // Signal 8032 to release other engine control right
        if(_T8032Cmd(&rCmd, NULL) != PDWNC_OK)
        {
            LOG(5, "Switch other engine to ARM error: T8032 not response!\n");
            i4Ret = -1;
        }   
   	  // Disable 8032 engine access   	  

        u4Val = u4IO32Read4B(PDWNC_UP_CFG);
        u4Val &= ~(1U << 16);   	    	 
        vIO32Write4B(PDWNC_UP_CFG, u4Val);
        LOG(7, "Switch other engine to ARM successfully!\n");        
    }    
    else if(u4Set == PDWNC_T8032_SET_ENGINE_T8032)////useless in MT5387
    {
   	  // Disable 8032 engine access   	  
        u4Val = u4IO32Read4B(PDWNC_UP_CFG);
        u4Val |= (1U << 16);   	    	 
        vIO32Write4B(PDWNC_UP_CFG, u4Val);
        
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENGINE_SWITCH;
        rCmd.u1SubCmd = 0x2;    
        
        // Signal 8032 to get other engine control right        
        if(_T8032Cmd(&rCmd, NULL) != PDWNC_OK)
        {
            LOG(5, "Switch other engine to T8032 error: T8032 not response!\n");
            i4Ret = -1;
        }

        LOG(7, "Switch other engine to T8032 successfully!\n");        
    }        
    else if(u4Set == PDWNC_T8032_SET_NORMAL)
    {
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENTER_NORMAL;
        rCmd.u1SubCmd = 0x2;
        
        // Signal 8032 to enter normal mode
        _T8032Cmd(&rCmd, NULL);
        
        LOG(7, "Signal T8032 to switch to normal mode!\n"); 
    }
#if 0    
    else if(u4Set == PDWNC_T8032_SET_STANDBY)
    {         
			if(IO32ReadFldAlign(PDWNC_UP_CFG, FLD_XDATA_ACC))
			{
				vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC); //let T8032 control xdata
			}
//david : Switch operation of  CEC, UART, ENGINE are not necessary in MT5387/63
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_VIR_EN);
        
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENTER_STANDBY;
#ifdef CC_MTK_LOADER
        rCmd.u1SubCmd = 0x1; // AC_ON->Standby
#else
        rCmd.u1SubCmd = 0x2; // AC_ON->Normal->Standby
#endif
        
        // Signal 8032 to enter standby
        i4Ret = _T8032Cmd(&rCmd, NULL);

        LOG(7, "Signal T8032 to write power down entry code!\n");     

        HAL_Delay_us(T8032_ACK_TIMEOUT * 1000);        

    }
#endif        
    else
    {
        i4Ret = -1;
    }        

#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
    x_sema_unlock(_hT8032CmdMutex);
#endif
#endif
    
    return i4Ret;
}

//-----------------------------------------------------------------------------
/**  PDWNC_InitT8032 to initialize T8032.
 *  @param void.
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_InitT8032(UINT32 u4Addr, UINT32 u4Size)
{
    INT32 i4Ret = 0;
    UINT32 u4Val;    
    UINT32 u4IrrxType;
//#if defined(CC_MT5363)
//#ifndef LINUX_TURNKEY_SOLUTION	    
    UINT32 i = 0;
//#endif
//#endif    
    vIO32WriteFldAlign(PDWNC_ARM_INTEN, 0x1, FLD_AUX_INTEN);
#ifdef CC_MT5391
    // We need to make sure PDWNC run at correct clock
    if (IS_XTAL_60MHZ())
    {
        u4Val = PDCK_SEL_XTAL_D20;
    }
    else
    {
        u4Val = PDCK_SEL_XTAL_D9;    
    }    
    _PDWNC_WriteMT8297(REGSIF_RW_CKGEN, &u4Val);
#endif

    // Enable IR interrupt & wakeup
#ifdef CC_UP8032_ATV
	i4Ret = DRVCUST_IrrxGet(eIrrxType);
#else
    i4Ret = DRVCUST_InitQuery(eIrrxType, &u4IrrxType);   
#endif
    i4Ret = IRRX_InitMtkIrRegSetup(u4IrrxType);   

		vIO32WriteFldAlign(PDWNC_ARM_INTEN, 0x1, FLD_IR_INTEN);
    
    vIO32WriteFldAlign(PDWNC_WAKEN, 0x1, FLD_IR_WAKEN);
    // Set reset count from 16/27 seconds to 5/27 seconds
		vIO32Write4B(PDWNC_WKRSC, DRVCUST_InitGet(eSysResetTimer));
	Printf("IR DATA register : 0x%8x\n",u4IO32Read4B(PDWNC_IRDATA0));
    // If A/C on, load uCode to T8032 except for mt8223 
    u4Val = PDWNC_ReadWakeupReason();

    if((u4Val == PDWNC_WAKE_UP_REASON_AC_POWER) || (u4Val == PDWNC_WAKE_UP_REASON_WATCHDOG))

    {    
//#if defined(CC_MT5363)
//#ifndef LINUX_TURNKEY_SOLUTION
        UINT32 u4Val;
//#endif
//#endif        
#ifndef CC_UP8032_ATV // 8225 load uP code by risc
        UINT8* pu1Buf = x_mem_alloc(u4Size);
#endif
        
        Printf("T8032 init A/C on case loader stage...\n");      
#ifdef CC_MT8223
		//Printf("\n---------MT8223-----------\n");
		IO_WRITE8(PDWNC_BASE, 0x140, 0x7);//for 8223 load code to srm
#endif
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);	
	vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
#ifndef CC_UP8032_ATV // 8225 load uP code by risc
        // Load uP code              
        VERIFY(_LoadCode(u4Addr, u4Size) == 0);        
        
        _CopyFromT8032(0x0, u4Size, (UINT32)pu1Buf);
        if(x_memcmp((void*)u4Addr, (void*)pu1Buf, u4Size) != 0)
        {
            Printf("Load T8032 FW fail!!\n");
            return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
        }
        
        Printf("Load T8032 FW (addr: 0x%8x, size: %d)success!!\n", u4Addr, u4Size);
#endif

#if PDWNC_T8032_UART_PD_ENABLE
		//#ifdef CC_MT5396	//for 5368/96/89
		//Printf("Set VGA_SCL/SDA as UART PD function!\n");
		vIO32Write4B(PDWNC_PINMUX0, (u4IO32Read4B(PDWNC_PINMUX0) & 0xffff8fff) | 0x00003000);
		//#endif
		
        //Set UART transparent mode
        vIO32Write4B(PDWNC_STAB, 0xe2);//david: need to confirm...     
        //Printf("Set UART PD transparent OK!\n");
        //vIO32Write4B(PDWNC_RS232_MODE, 0x04);
        //Printf("Set UART PD pinmux OK!\n");   
#endif        
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_VIR_EN);
	
#ifdef CC_MT8223
		IO_WRITE8(PDWNC_BASE, 0x178, 0x7b);//switch 8032 run on srm
		IO_WRITE32(PDWNC_BASE, 4, 0x10006); // CLOCK FOR 8032 RUN NORMAL
//		BIM_WRITE32(0x0A4, 1); //switch pdwnc module to normal mode for DRAM access
		IO_WRITE8(PDWNC_BASE, 0x140, 0x5); //release load code to srm
#endif
	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_T8032_RST);
    vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
//#if defined(CC_MT5363)
//#ifndef LINUX_TURNKEY_SOLUTION	
        HAL_Delay_us(500); //wait T8032 init ready
        for(i = 0; i < T8032_ALIVE_CHECK_RETRY; i++)
        {
            if((i4Ret = PDWNC_ReadSwVersion(&u4Val)) == PDWNC_OK)
            {
                Printf("T8032 FW version: %d\n", u4Val);
                break;
            }
            Printf("T8032 FW version read error!\n");                        
            HAL_Delay_us(T8032_ALIVE_CHECK_INTERVAL);                                        
        }                     
//#endif
//#endif      
    }

//#if defined(CC_MT5363)
//#ifndef LINUX_TURNKEY_SOLUTION	    
    if( i == T8032_ALIVE_CHECK_RETRY)
    {
    	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
    	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);	
    	vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
        Printf("Read veriosn fail, set t8032 as reset state!\n");                        	
	    return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
    }
//#endif
//#endif
#ifndef CC_IR_POWER_BUTTON_RECEIVE_PRELOADER      
    else
    {

#if PDWNC_T8032_UART_PD_ENABLE
		//#ifdef CC_MT5396	//for 5368/96/89
		//Printf("Set VGA_SCL/SDA as UART PD function!\n");
		vIO32Write4B(PDWNC_PINMUX0, (u4IO32Read4B(PDWNC_PINMUX0) & 0xffff8fff) | 0x00003000);
		//#endif
		
        //Set UART transparent mode
        vIO32Write4B(PDWNC_STAB, 0xe2);//david: need to confirm...     
        //Printf("Set UART PD transparent OK!\n");
        //vIO32Write4B(PDWNC_RS232_MODE, 0x04);
        //Printf("Set UART PD pinmux OK!\n");     
#endif            
        PDWNC_T8032_CMD_T rCmd;
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENTER_NORMAL;
        rCmd.u1SubCmd = 1; // loader stage
        rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;  
        if(_T8032Cmd(&rCmd, NULL))
        {
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);	
            vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
            Printf("Enter normal state fail, set t8032 as reset state!\n");                        	
            return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
        }
        else
        {
            Printf("T8032 change to loader stage...\n");             
         }    
    }       
#endif

    return i4Ret;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/* PDWNC_InitT8032() To initial T8032 uP with 8032 binary info.
 *
 * @param u4Offset  8032 binary pointer offset.
 * @param u4Size    8032 binary size.
 * @retval 0        SUCCESS.
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_InitT8032(UINT32 u4Offset, UINT32 u4Size)
{
#ifdef CC_MT8223

/*			UNUSED(u4Offset);
			UNUSED(u4Size);
			vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
			vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST); //8032 not run at boot up (neither in ac on nor dc on)
			vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
			IO_WRITE8(PDWNC_BASE, 0x140, 0); //srm to normal clock
			Printf("-----PDWNC_InitT8032 28178=%08x\n",IO_READ32(PDWNC_BASE,0x178));
			Printf("-----PDWNC_InitT8032 28140=%08x\n",IO_READ32(PDWNC_BASE,0x140));
		
	Printf("200080a4 shall = 0x%08x\n",BIM_READ32(0x0A4));
	//BIM_WRITE32(0x0A4, 5); //switch pdwnc module to normal mode for DRAM access
	Printf("0x20028000= 0x%08x\n0x20028004= 0x%08x\n", IO_READ32(PDWNC_BASE,0),IO_READ32(PDWNC_BASE,4)); // CLOCK FOR 8032 RUN NORMAL
	IO_WRITE32(PDWNC_BASE, 4, 0x10606); // CLOCK FOR 8032 RUN NORMAL
	IO_WRITE32(PDWNC_BASE,0,0x40);
	Printf("0x20028000= 0x%08x\n0x20028004= 0x%08x\n", IO_READ32(PDWNC_BASE,0),IO_READ32(PDWNC_BASE,4)); // CLOCK FOR 8032 RUN NORMAL
	*/

	return 0;

#else
    return _PDWNC_InitT8032(u4Offset, u4Size);
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_T8032Cmd to issue cmd to T8032.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_T8032Cmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse)
{
    INT32 i4Ret = 0;
#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
    x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT);
#endif
#endif
    
    i4Ret = _T8032Cmd(prCmd, prResponse);

#ifndef CC_MTK_LOADER
#ifndef CC_UP8032_ATV
    x_sema_unlock(_hT8032CmdMutex);
#endif
#endif

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** PDWNC_T8032GpioCmd to issue cmd to T8032.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_T8032GpioCmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse)
{

    INT32 i4Ret = 0;
    UINT8 u1Cmd, u1SubCmd;
    UINT32 u4Data;
    UINT32 u4Val = 0;        
    UINT32 i = 0, j = 0;
#ifndef CC_MTK_LOADER
        CRIT_STATE_T rCrit;
#else /* CC_MTK_LOADER */
        //UINT32 u4State;
#endif 

#ifndef CC_MTK_LOADER
        rCrit = x_crit_start();
#endif /* CC_MTK_LOADER */

    UNUSED(u4Data);

    VERIFY(prCmd != NULL);

    u1Cmd = prCmd->u1Cmd;
    if((u1Cmd != PDWNC_CMD_ARMT8032_SET_GPIO_ENABLE) &&
        (u1Cmd != PDWNC_CMD_ARMT8032_SET_GPIO_OUTPUT) &&
        (u1Cmd != PDWNC_CMD_ARMT8032_SET_SERVO_GPIO_ENABLE) &&
        (u1Cmd != PDWNC_CMD_ARMT8032_SET_SERVO_GPIO_OUTPUT) )
        {
#ifndef CC_MTK_LOADER
            x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */
            return -1;
        }
    u1SubCmd = prCmd->u1SubCmd;
    u4Data = prCmd->au1Data[0] | (prCmd->au1Data[1] << 8) | (prCmd->au1Data[2] << 16) | (prCmd->au1Data[3] << 24);    

//    vIO32Write4B(PDWNC_SHREG8, u1Cmd | ((UINT32)u1SubCmd << 8));
    PDWNC_T8032CMD_SET_CMD(u1Cmd | ((UINT32)u1SubCmd << 8));
//    vIO32Write4B(PDWNC_SHREG9, u4Data);
        PDWNC_T8032CMD_SET_DATA(u4Data);
    // If the command is not ENTER_STANDBY, we need ACK 
        // Wait until T8032 enter non-xdata mode
        while(1)
        {          
            vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
            
            while(i < 15000)
            {
                HAL_Delay_us(100);                  
                if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))
                {
                    u4Val = PDWNC_T8032CMD_GET_RESPONSE();//u4IO32Read4B(PDWNC_SHREG0);
                    if((u4Val & 0xff) == u1Cmd)
                    {
                        if(prResponse != NULL)
                        {
                            prResponse->u1Ack = (UINT8)(u4Val & 0xff);
                            prResponse->au1Data[0] = (UINT8)((u4Val >> 8) & 0xff);
                            prResponse->au1Data[1] = (UINT8)((u4Val >> 16) & 0xff);
                            prResponse->au1Data[2] = (UINT8)((u4Val >> 24) & 0xff);            
                        }                            
                    }
                    else
                    {      
                        Printf("T8032 gpio cmd polling error: ack mismatch!(Cmd = 0x%x, SubCmd = 0x%x, Ack = %d)\n", u1Cmd, u1SubCmd,(UINT8)(u4Val & 0xff));
                        // Ack mismatch
                        i4Ret = -1;                
                    }
                                       
                    break;                
                }      
                i++;
            }
                                 
            if(i < 10000)
            {
                	vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
                break;
            }            
            else
            {
                	vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
                i = 0;
            }
            
            if(++j > 2 /*T8032_ACK_TIMEOUT*/)
            {
                Printf("T8032 gpio cmd polling error: ack timeout!(Cmd = 0x%x, SubCmd = 0x%x, Ack = %d)\n", u1Cmd, u1SubCmd,(UINT8)(u4Val & 0xff));
                // Ack timeout
                i4Ret = -2;
                break;
            }
          
        }

#ifndef CC_MTK_LOADER
            x_crit_end(rCrit);         
#endif /* CC_MTK_LOADER */


    return i4Ret;
}

//-----------------------------------------------------------------------------
/** PDWNC_ReadSwVersion reads SW version from T8032.
 *  @param u2Gpio is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
#ifdef CC_MTK_LOADER
extern UINT8 bLoaderVersion;
#endif
INT32 PDWNC_ReadSwVersion(UINT32* pu4SwVersion)
{
    INT32 i4Ret = 0;
    PDWNC_T8032_CMD_T rCmd;
    PDWNC_T8032_RESPONSE_T rResponse;

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_QUERY_SW_VERSION;
#ifdef CC_PDWNCONOFF_PATCH
	rCmd.u1SubCmd = 0xaa;
#else
    rCmd.u1SubCmd = 0x0;
#endif
#ifdef CC_MTK_LOADER
    rCmd.au1Data[0] = bLoaderVersion;
#else
    rCmd.au1Data[0] = 0;
#endif
    rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;    
    // Signal 8032 to enter normal mode
    i4Ret = PDWNC_T8032Cmd(&rCmd, &rResponse);

    *pu4SwVersion = (UINT32)rResponse.au1Data[0];

    return i4Ret;
}

#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** PDWNC_SetPowerDownFlag to set power down flag.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_WaitLastTransactionFin(void)
{
#ifdef CC_UP8032_ATV
	return OSR_OK;
#else
    UINT32 u4Msg;
    SIZE_T u4Size = sizeof(UINT32);
    UINT16 u2Idx = 0;
    return x_msg_q_receive_timeout(&u2Idx, 
    	&u4Msg, &u4Size, &_hCECLastTransactionFinMsgq, 1, 1000);    
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_SetPowerDownFlag to set power down flag.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_SetLastTransactionFin(void)
{
#ifdef CC_UP8032_ATV
	return OSR_OK;
#else
    UINT32 u4Msg;
    SIZE_T u4Size = sizeof(UINT32);
    return x_msg_q_send(_hCECLastTransactionFinMsgq,
    	&u4Msg, u4Size, 200);    
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_SetPowerDownFlag to set power down flag.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_CheckLastTransactionRequest(void)
{
#ifdef CC_UP8032_ATV
	return OSR_OK;
#else
    UINT32 u4Msg;
    SIZE_T u4Size = sizeof(UINT32);
    UINT16 u2Idx = 0;
    return x_msg_q_receive(&u2Idx, 
    	&u4Msg, &u4Size, &_hCECStartLastTransactionMsgq, 1, X_MSGQ_OPTION_NOWAIT);    
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_SetPowerDownFlag to set power down flag.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_SetLastTransactionRequest(void)
{
#ifdef CC_UP8032_ATV
	return OSR_OK;
#else
    UINT32 u4Msg;
    SIZE_T u4Size = sizeof(UINT32);
    return x_msg_q_send(_hCECStartLastTransactionMsgq,
    	&u4Msg, u4Size, 200);        
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_SetPowerDownFlag to set power down flag.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
/*
INT32 PDWNC_GetCECDataFromT8032(UINT8* pu1Buf)
{    
    VERIFY(pu1Buf != NULL);

    x_memcpy(pu1Buf, _au1CecData, CEC_XDATA_SIZE);
    
    return 0;
}
*/
#endif

//-----------------------------------------------------------------------------
/** PDWNC_SetT8032ToggleLed to enable led blinking function (while lzhs decompressing).
 *  @param u1ErrorCode the error code.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_SetT8032ToggleLed(UINT32 u4Enable)
{
#if !defined(CC_LOADER_SHRINK_LED)  //loader shrink
    PDWNC_T8032_CMD_T rCmd;
    UINT32 i, u4Offset;
    u4Offset = DRVCUST_InitGet(eT8032uPOffset);
    if (u4Offset != 0)  // here needs to setup 8032uP.
    {            
        UINT32 u4BlinkingLedGpio, u4BlinkingLedOnVal, u4LedBlinkingPeriod, u4GpioNumber;      
        if ((!DRVCUST_InitGet(eLoaderLogoNumber)) && ((DRVCUST_InitQuery(eFlagBootBlinkingLedSupport, &i) == 0)) && (i))    
        {
            UNUSED(DRVCUST_InitQuery(eBootBlinkingLedOnVal, &u4BlinkingLedOnVal));
            UNUSED(DRVCUST_InitQuery(eBootBlinkingLed, &u4BlinkingLedGpio));            
            UNUSED(DRVCUST_InitQuery(eBootBlinkingLedPeriod, &u4LedBlinkingPeriod));
            
            rCmd.u1Cmd = PDWNC_CMD_ARMT8032_LED_BLINKING;
            rCmd.u1SubCmd = u4Enable;

           if(u4BlinkingLedGpio >= SERVO(0))
           {
                u4GpioNumber = u4BlinkingLedGpio - SERVO(0);
                rCmd.au1Data[0] = (UINT8)u4GpioNumber; //OFFSET GPIO INDEX
                rCmd.au1Data[1] = (UINT8)u4BlinkingLedOnVal; // POLARITY
				#if 1//defined(CC_MT5365) || defined(CC_MT5395) //change after 5365/95/...
                    rCmd.au1Data[2] =  (UINT8)(u4LedBlinkingPeriod / 20);
                #else
                    rCmd.au1Data[2] =  (UINT8)(u4LedBlinkingPeriod / 50);
                #endif                    
                rCmd.au1Data[3] =  1;             
           }
           else if(u4BlinkingLedGpio >= OPCTRL(0))
           {
                u4GpioNumber = u4BlinkingLedGpio - OPCTRL(0);
                rCmd.au1Data[0] = (UINT8)u4GpioNumber; //OFFSET GPIO INDEX
                rCmd.au1Data[1] = (UINT8)u4BlinkingLedOnVal; // POLARITY
				#if 1//defined(CC_MT5365) || defined(CC_MT5395) //change after 5365/95/...
                    rCmd.au1Data[2] =  (UINT8)(u4LedBlinkingPeriod / 20);
                #else
                    rCmd.au1Data[2] =  (UINT8)(u4LedBlinkingPeriod / 50);
                #endif                    
                rCmd.au1Data[3] =  0;             
           }
           else
           {
                Printf("Set t8032 led gpio error (gpio index error)!!!\n");
                return -1;
           }
            if(u4Enable)
            {
                GPIO_SetOut(u4BlinkingLedGpio, ~u4BlinkingLedOnVal);
                Printf("Enable Led blinking (gpio_idx %d, on_value %d, period %d, type: %s)\n", rCmd.au1Data[0],rCmd.au1Data[1],rCmd.au1Data[2],((rCmd.au1Data[3])? "SRV":"OPCTRL"));
            }
            else
            {
                Printf("Disable gpio %d Led blinking\n", u4BlinkingLedGpio);
            }
                               
           return PDWNC_T8032Cmd(&rCmd, NULL);
        }
        else
        {
            return 0;        
        }
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** PDWNC_SetUartPDBaudrate to set system error code.
 *  @param u1ErrorCode the error code.
 */
//-----------------------------------------------------------------------------

INT32 PDWNC_SetUartPDBaudrate(UINT8 u1baudrate)
{
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_UART_PD_BAUDRATE;
    rCmd.u1SubCmd = u1baudrate;
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;    
    return PDWNC_T8032Cmd(&rCmd, NULL);
}

