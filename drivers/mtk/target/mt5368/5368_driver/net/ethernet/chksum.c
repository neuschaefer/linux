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
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, USER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND 
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") 
* ARE PROVIDED TO USER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS 
* ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH 
* RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED 
* IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND USER AGREES TO LOOK ONLY TO 
* SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL 
* ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO USER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
* 
* USER HEREBY ACKNOWLEDGES THE CONFIDENTIALITY OF MEDIATEK SOFTWARE AND AGREES 
* NOT TO DISCLOSE OR PERMIT DISCLOSURE OF ANY MEDIATEK SOFTWARE TO ANY THIRD 
* PARTY OR TO ANY OTHER PERSON, EXCEPT TO DIRECTORS, OFFICERS, EMPLOYEES OF 
* USER WHO ARE REQUIRED TO HAVE THE INFORMATION TO CARRY OUT THE PURPOSE OF 
* OPENING OR USING THIS FILE.
*****************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: ChkSum.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   CheckSum unit test and debug source file
 *
 * Author:
 * -------
 *   Hs Huang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

//#include "x_lint.h"
//LINT_EXT_HEADER_BEGIN
#include "x_assert.h"

#include "x_bim.h"
//#include "x_timer.h"
//#include "x_hal_1176.h"

#include "../net_if.h"
#include "../net_debug.h"
#include "chksum.h"
#include "chksum_if.h"
#include "ether_drv.h"

/****************************************************************************
** Local definitions
****************************************************************************/
#undef VECTOR_TCPIP //FIX build error
#define VECTOR_TCPIP 52

#define CHKS_INTR_AUTOPOLLING
/****************************************************************************
** Local structures and enumerations
****************************************************************************/

/****************************************************************************
** Constant Data Declaration
****************************************************************************/

/****************************************************************************
** Function prototypes
****************************************************************************/

/****************************************************************************
** Local variable
****************************************************************************/

/****************************************************************************
** Global variable
****************************************************************************/
UINT32 u4ChkSumOk = 0;
UINT32 u4RxChkSumOk = 0;
UINT32 u4ChkSumCnt = 0;
UINT32 u4RxChkSumCnt = 0;

HANDLE_T _hEtherChkSumSema;
#if HW_CHKSUM_ON_THE_FLY
LIST_HEAD_T _rChksumList;     
#endif

/****************************************************************************
** External functions
****************************************************************************/
#if 0
static void vChkSumIrqHandler(UINT16 u2Vector)
{
    ASSERT(u2Vector == VECTOR_TCPIP);  

    /* clear BIM interrupt */
    if(!BIM_ClearIrq(u2Vector))
    {
        ERR_DTCP("Checksum irq clear fail\n");
    }
    u4ChkSumOk = 1;
    u4ChkSumCnt++;
    //clear interrupt status
#if HW_CHKSUM_ON_THE_FLY
    CHKSUM_SEMA_UNLOCK(_hEtherChkSumSema);
    vChksumDeQueue();
#endif
}
#endif
#ifndef CHKS_INTR_AUTOPOLLING
void vChkSumIrqHandler(UINT16 u2Vector)
{
	UINT32 u4Data, u4Data1; 
//    ASSERT(u2Vector == VECTOR_TCPIP);  

    NOTE_ETHER("Vector %d\n", u2Vector);
    /* clear BIM interrupt */
    #ifdef DBG_CHKSUM	
    UTIL_Printf("INT\n");
    #endif
    
    if(!BIM_ClearIrq(u2Vector))
    {
       // ERR_ETHER("Ether Cks irq clear fail\n");
       NOTE_ETHER("Ether Cks irq clear fail\n");
    } 
    
    u4Data= dReadChkSum(RW_CHKSUM_CTRL);
    
    
    if (u4Data & CHKSUM_INT)
    {
    	
    	SetBitChkSum(RW_CHKSUM_CTRL, CHKSUM_INT_CLR);
    	ClrBitChkSum(RW_CHKSUM_CTRL, CHKSUM_INT_CLR);
    	u4ChkSumOk = 1;
    	#ifdef DBG_CHKSUM	
    	NOTE_ETHER("Tx u4ChkSumOk = 1, u4ChkSumCnt=%d\n", u4ChkSumCnt);
    	#endif
    	
    	u4ChkSumCnt ++;
    }
    
    u4Data1= dReadChkSum(RW_RX_CHKSUM_CTRL);
    
    if (u4Data1 & RX_CHKSUM_INT)
    {
    	
    	SetBitChkSum(RW_RX_CHKSUM_CTRL, RX_CHKSUM_INT_CLR);
    	ClrBitChkSum(RW_RX_CHKSUM_CTRL, RX_CHKSUM_INT_CLR);
    	u4RxChkSumOk = 1;
    	#ifdef DBG_CHKSUM	
    	UTIL_Printf("Rx u4ChkSumOk = 1, u4RxChkSumCnt=%d\n", u4RxChkSumCnt);
    	#endif
    	u4RxChkSumCnt ++;
    }
    
    if((u4ChkSumOk ==0) &&(u4RxChkSumOk == 0)) 
    {
		ERR_ETHER("checksum status not ready \n");
    }
    #ifdef DBG_CHKSUM	
    UTIL_Printf("u4ChkSumCnt = %d, u4ChkSumOk = %d\n", u4ChkSumCnt, u4ChkSumOk);
    UTIL_Printf("u4RxChkSumCnt = %d, u4RxChkSumOk = %d\n", u4RxChkSumCnt, u4RxChkSumOk);
    #endif
    
}

static void vCheckSumSetIsr(BOOL fgSet)
{
    void (* pfnOldIsr)(UINT16);

    if(fgSet) 
    {
        UINT32 i;
//        VERIFY(x_reg_isr(VECTOR_TCPIP, vChkSumIrqHandler, &pfnOldIsr) == OSR_OK);
        for (i=42; i<60; i++)
            VERIFY(x_reg_isr(i, vChkSumIrqHandler, &pfnOldIsr) == OSR_OK);
        BIM_ClearIrq(VECTOR_TCPIP);
        BIM_EnableIrq(VECTOR_TCPIP);
    }
    else
    {
        VERIFY(x_reg_isr(VECTOR_TCPIP, NULL, &pfnOldIsr) == OSR_OK);
        BIM_DisableIrq(VECTOR_TCPIP);
    }

}
#endif
/*******************************************************************************
**  vCheckSumInit
**
**  descriptions:
**      This rotinue is called to initialize 1's complement checksum module
**  parameters:
**      none
**  result:
*******************************************************************************/
void vCheckSumInit(void)
{
    VERIFY(x_sema_create(&_hEtherChkSumSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

    vWriteChkSum(RW_CHKSUM_CTRL, INTR_EN | OUT_INV);  // Enable interrupt
    vWriteChkSum(RW_RX_CHKSUM_CTRL, RX_OUT_INV | RX_INTR_EN);  // Enable interrupt
    vWriteChkSum(RW_CHKSUM_DMA_CTRL, 0x0020<<16|0x0020); //default burst length 32*64 = 2048 byte.
#ifndef CHKS_INTR_AUTOPOLLING
    vCheckSumSetIsr(TRUE);
#endif
}

#if 0
UINT16 u2Checksum16SW(UINT8* data, INT32 len)
{
    UINT32 sum = 0;
    UINT16 u2Data;
/*
    if ((len & 1) == 0)
        len = len >> 1;
    else
        len = (len >> 1) + 1;
*/        
    while (len > 0) 
    {
      //avoid access word in unalign address
        u2Data = *data << 8;
        if(len == 1)  //pad 0, if length is odd.
        {
          u2Data &= 0xFF00;
          sum += u2Data;
          break;
        }
        else
        {
          u2Data |= *(data+1);
          sum += u2Data;
        }
        data += sizeof(UINT16);
        len-=2;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    sum = ~sum;
    return(sum&0xFFFF);
}



#define CHKSUM_DEBUG

UINT16 u2Checksum16HW(UINT8* ptrData, UINT32 u4Len)
{
  UINT32 u4Data; 

#ifdef CHKSUM_DEBUG  
  u4Data = dReadChkSum(RW_CHKSUM_CTRL);
  while(u4Data & CHECKSUM_EN)
  {
    x_thread_delay(1);
    u4Data = dReadChkSum(RW_CHKSUM_CTRL);  
  }
#endif  
  vWriteChkSum(RW_CHKSUM_ADDR, (UINT32)ptrData);
  vWriteChkSum(RW_CHKSUM_LENGTH, u4Len);

  if((UINT32)ptrData&0x1) //start address is odd, reverse output
  {
    ClrBitChkSum(RW_CHKSUM_CTRL, OUT_REV);
  }
  else
  {
    SetBitChkSum(RW_CHKSUM_CTRL, OUT_REV);
  }
  u4ChkSumOk = 0;
  SetBitChkSum(RW_CHKSUM_CTRL, CHECKSUM_EN);

  while(!u4ChkSumOk);
  {
    x_thread_delay(1);
  }
#ifdef CHKSUM_DEBUG  
  u4Data = dReadChkSum(RW_CHKSUM_CTRL);
  if(u4Data & CHECKSUM_EN)
  {
     ERR_DTCP("Interrupt happen but checksum != 0\n");
     while(u4Data & CHECKSUM_EN)
     {
       x_thread_delay(1);
       u4Data = dReadChkSum(RW_CHKSUM_CTRL);  
     }
  }
#endif
  return dReadChkSum(RW_CHKSUM_RESULT) & 0xFFFF;
}
#endif

UINT16 u2Checksum16SW(UINT8* data, INT32 len)
{
    UINT32 sum = 0;
    UINT16 u2Data;
/*
    if ((len & 1) == 0)
        len = len >> 1;
    else
        len = (len >> 1) + 1;
*/        
    while (len > 0) 
    {
      //avoid access word in unalign address
        u2Data = *data << 8;
        if(len == 1)  //pad 0, if length is odd.
        {
          u2Data &= 0xFF00;
          sum += u2Data;
          break;
        }
        else
        {
          u2Data |= *(data+1);
          sum += u2Data;
        }
        data += sizeof(UINT16);
        len -= 2;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    sum = ~sum;
    return(sum&0xFFFF);
}

UINT16 u2RxChecksum16SW(UINT8* data, INT32 len)
{
    UINT32 sum = 0;
    UINT16 u2Data;
/*
    if ((len & 1) == 0)
        len = len >> 1;
    else
        len = (len >> 1) + 1;
*/        
    while (len > 0) 
    {
      //avoid access word in unalign address
        u2Data = *data << 8;
        if(len == 1)  //pad 0, if length is odd.
        {
          u2Data &= 0xFF00;
          sum += u2Data;
          break;
        }
        else
        {
          u2Data |= *(data+1);
          sum += u2Data;
        }
        data += sizeof(UINT16);
        len -= 2;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    sum = ~sum;
    return(sum&0xFFFF);
}

UINT16 u2Checksum16HW(UINT8* ptrData, UINT32 u4Len)
{
    UINT32 u4Data; 
    UINT32 u4TimeOut; 

    u4Data = dReadChkSum(RW_CHKSUM_CTRL);

    while(u4Data & CHECKSUM_EN)
    {
        x_thread_delay(1);
        u4Data = dReadChkSum(RW_CHKSUM_CTRL);  
    }


    vWriteChkSum(RW_CHKSUM_ADDR, (UINT32)ptrData);
    vWriteChkSum(RW_CHKSUM_LENGTH, u4Len);

    if((UINT32)ptrData & 0x1) //start address is odd, reverse output
    {
    	ClrBitChkSum(RW_CHKSUM_CTRL, OUT_REV);
    }
    else
    {
        SetBitChkSum(RW_CHKSUM_CTRL, OUT_REV);
    }

    u4ChkSumOk = 0;
    u4ChkSumCnt = 0;
    u4TimeOut =0 ;
    SetBitChkSum(RW_CHKSUM_CTRL, CHECKSUM_EN);

#ifndef CHKS_INTR_AUTOPOLLING
    do 
    {
        u4TimeOut ++;
        x_thread_delay(2);
        if(u4TimeOut == 100)
        {
            NOTE_ETHER("Tx u4TimeOut break, u4ChkSumOk=%d\n", u4ChkSumOk);
            // break;
        }  

    } while(!u4ChkSumOk);
    
#else
    do 
    {
        u4Data = dReadChkSum(RW_CHKSUM_CTRL);
        
        u4TimeOut ++;
        x_thread_delay(2);
        if(u4TimeOut == 100)
        {
            NOTE_ETHER("Tx u4TimeOut break, u4ChkSumOk=%d\n", u4ChkSumOk);
            // break;
        }  

    } while(!(u4Data & CHKSUM_INT));

    ClrBitChkSum(RW_CHKSUM_CTRL, CHKSUM_INT_CLR);
#endif
    
    u4Data = dReadChkSum(RW_CHKSUM_CTRL);
    if(u4Data & CHECKSUM_EN)
    {
         //ERR_ETHER("Interrupt happen but checksum != 0\n");
         NOTE_ETHER("Interrupt happen but checksum != 0\n");
         while(u4Data & CHECKSUM_EN)
         {
           x_thread_delay(1);
           u4Data = dReadChkSum(RW_CHKSUM_CTRL);  
         }
    }

    return dReadChkSum(RW_CHKSUM_RESULT) & 0xFFFF;
}


UINT16 u2RxChecksum16HW(UINT8* ptrData, UINT32 u4Len)
{
  UINT32 u4Data; 
  UINT32 u4TimeOut=0;
   
  u4Data = dReadChkSum(RW_RX_CHKSUM_CTRL);

  while(u4Data & RX_CHECKSUM_EN)
  {
    x_thread_delay(1);
    u4Data = dReadChkSum(RW_RX_CHKSUM_CTRL);  
  }

  
  vWriteChkSum(RW_RX_CHKSUM_ADDR, (UINT32)ptrData);
  vWriteChkSum(RW_RX_CHKSUM_LENGTH, u4Len);

  if((UINT32)ptrData & 0x1) //start address is odd, reverse output
  {
  	ClrBitChkSum(RW_RX_CHKSUM_CTRL, RX_OUT_REV);
  }
  else
  {
    SetBitChkSum(RW_RX_CHKSUM_CTRL, RX_OUT_REV);
  }

  u4RxChkSumOk = 0;
  u4RxChkSumCnt = 0;
  
  SetBitChkSum(RW_RX_CHKSUM_CTRL, RX_CHECKSUM_EN);

  
  //while(!u4ChkSumOk);
  
#ifndef CHKS_INTR_AUTOPOLLING
    do 
    {
        u4TimeOut ++;
        x_thread_delay(2);
        if(u4TimeOut == 100)
        {
            NOTE_ETHER("Rx u4TimeOut break, u4RxChkSumOk =%d\n", u4RxChkSumOk);
            //break;
        }  
    } while(!u4RxChkSumOk);
#else    
    do 
    {
        u4Data = dReadChkSum(RW_RX_CHKSUM_CTRL);
        u4TimeOut ++;
        x_thread_delay(2);
        if(u4TimeOut == 100)
        {
            NOTE_ETHER("Rx u4TimeOut break, u4ChkSumOk=%d\n", u4RxChkSumOk);
            // break;
        }  
    } while(!(u4Data & RX_CHKSUM_INT));

    ClrBitChkSum(RW_RX_CHKSUM_CTRL, RX_CHKSUM_INT_CLR);
#endif
  
   
  u4Data = dReadChkSum(RW_RX_CHKSUM_CTRL);
  
  
  if(u4Data & RX_CHECKSUM_EN)
  {
     //ERR_ETHER("RX Interrupt happen but checksum != 0\n");
     NOTE_ETHER("RX Interrupt happen but checksum != 0\n");
     while(u4Data & RX_CHECKSUM_EN)
     {
       x_thread_delay(1);
       u4Data = dReadChkSum(RW_RX_CHKSUM_CTRL);  
     }
  }
  
  
  return dReadChkSum(RW_RX_CHKSUM_RESULT) & 0xFFFF;
}

/*******************************************************************************
**  vChkSum_DrvInit
**
**  descriptions:
**      This rotinue is called to initialize Chksum driver 
**  parameters:
**      none
**  result:
**  return:
**      0 if success,
**      negative value if fail
*******************************************************************************/
void vChkSum_DrvInit(void)
{
    vCheckSumInit();
    INFO_DTCP("Checksum module init OK\n");
}

/*******************************************************************************
**  _u2CheckSumOffload
**
**  descriptions:
**      This rotinue is called to get 1's complement value, it's a blocking implementation
**  parameters:
**      ptrData: data pointer
**       u4Len : data length
**  return:
**      1's complement checksum of data. From ptrData --> ptrData+u4Len
**
*******************************************************************************/
UINT16 _u2CheckSumOffload(UINT8* ptrData, UINT32 u4Len)
{
  UINT16 u2Data;

  HalFlushInvalidateDCache();    

  CHKSUM_SEMA_LOCK(_hEtherChkSumSema);

  u2Data = u2Checksum16HW(ptrData, u4Len);

  CHKSUM_SEMA_UNLOCK(_hEtherChkSumSema);

  return u2Data;
}

/*******************************************************************************
**  _i4ChkSumTest
**
**  descriptions:
**      The function is called to test checksum module
**  parameters:
**      none
**  result:
**  return: CHKSUM_DRV_OK
*******************************************************************************/
#if 0
INT32 _i4ChkSumTest(SIZE_T zSize, UINT32 u4Count)
{
    UCHAR *pucSour;
    UINT32 *pucSourAddr;
    UINT32 u4Cnt, u4Align;
    UINT32 u4Size, u4TotalSize;
    UINT32 u4Seed = 0x1234abcd;
    INT32 i, i4Ret = 0;
    UINT32 u4SWChkSum;
    UINT32 u4HWChkSum;    

 /*generate test pattern*/
    u4TotalSize = zSize + 32;
    //allocate 32 bit aligned buffer, maybe larger then u4TotalSize, it's ok.
    pucSourAddr = (UINT32 *)x_mem_alloc(u4TotalSize + (4 - (u4TotalSize & 0x03)));
    if(pucSourAddr == NULL)
    {
      return;	
    }
    
    for(i = 0; i < (INT32)u4TotalSize/4; i++)                            // fixed size
    {
      *(pucSourAddr + i) = u4Seed;
      u4Seed += 0x01010101;
    }
    
//verify by sw result
    HalFlushInvalidateDCache();    
    u4Cnt = (u4Count == NULL) ? 1 : u4Count;
    u4Align = 0;
    for(i = 0; i < (INT32)u4Cnt; i++)                            
    {
      for(u4Align = 0; u4Align<=31; u4Align++)
      {
        for(u4Size = 1; u4Size <= zSize; u4Size++)        // size increase from 1 to specific value
        {
            /* add an offset to test if sour addresses can be byte alignment */
            u4Align = u4Align & 0x1F; //check 32 byte boundry
            pucSour = (UCHAR *)((UINT32)pucSourAddr + u4Align);

            u4HWChkSum= u2Checksum16HW((UINT8*)pucSour, u4Size);

            u4SWChkSum = u2Checksum16SW((UINT8*)pucSour, u4Size);
            
            if(u4HWChkSum != u4SWChkSum)
            {
               ERR_DTCP("checksum error HW: 0x%08x, SW: 0x%08x, Length:%d, align:%d\n", 
                                u4HWChkSum, u4SWChkSum, u4Size, u4Align);
               i4Ret = -1;
            }
        }
      }
    }
    x_mem_free(pucSourAddr);

    if(i4Ret == 0)
    {
        INFO_DTCP("CheckSum verify, length:from 1->%d, %d times ok.\n", zSize, u4Cnt);
    }
    else
    {
        INFO_DTCP("CheckSum verify fail.\n");
    }
    return(0);
}
#endif

/*******************************************************************************
**  i4ChkSum_Start
**
**  descriptions:
**      This rotinue is called to start ChkSum HW processing
**  parameters:
**      none
**  result:
**  return:
**      0 if success,
**      negative value if fail
*******************************************************************************/
void vChkSum_Start(UINT8* ptrData, UINT32 u4Len)
{
    //INT32 i4Ret = 0;
    //UINT32 u4Count = 0xffff;
    //protection is out of this function

    CHKSUM_SEMA_LOCK(_hEtherChkSumSema);

    vWriteChkSum(RW_CHKSUM_ADDR, (UINT32)ptrData);
    vWriteChkSum(RW_CHKSUM_LENGTH, u4Len);
    if((UINT32)ptrData&0x1) //start address is odd, reverse output
    {
      ClrBitChkSum(RW_CHKSUM_CTRL, OUT_REV);
    }
    else
    {
      SetBitChkSum(RW_CHKSUM_CTRL, OUT_REV);
    }
    u4ChkSumOk = 0;
    SetBitChkSum(RW_CHKSUM_CTRL, CHECKSUM_EN);

}

#if HW_CHKSUM_ON_THE_FLY

/*******************************************************************************
**  pvChkSum_Memcpy
**
**  descriptions:
**      The function is called to process DTCP HW checksum.
**  parameters:
**      pkt_buf - pointer of pcaket buffer
**  return:
**      none
*******************************************************************************/
void vTx_Chksum_Offload(IN PKT_BUFF_T *pkt_buf)
{
    pkt_buf->t_hw_chksum.b_is_completed = FALSE;
    MLIST_INSERT(&_rChksumList, (LIST_ENTRY_T *)&pkt_buf->t_link);

    if (!dReadChkSum(RW_CHKSUM_CTRL) & CHECKSUM_EN) // check chksum module is free
    {
      HalFlushInvalidateDCache();    
      vChkSum_Start(pkt_buf->t_hw_chksum.pui1_start, pkt_buf->t_hw_chksum.ui4_len);
    }
}

void vChksumDeQueue(void)
{
    LIST_ENTRY_T *pt_entry;
    PKT_BUFF_T *prPktBuf;
    UINT8 *pucChksum;
    UINT16 u2Chksum;
    //UINT16 *pu2Chksum;
//    UINT32 u4Offset;
    
    //HAL_GetTime(&_rChksumTimeEnd);
    //HAL_GetDeltaTime(&_rChksumTimeDelta, &_rChksumTimeStart, &_rChksumTimeEnd);

    //MLIST_REMOVE(&_rChksumList, (LIST_ENTRY_T *)prPktBuf);
    pt_entry = _rChksumList.pt_head;
    ASSERT(pt_entry != NULL);
    _rChksumList.pt_head = pt_entry->pt_next;
    pt_entry->pt_next = NULL;
    if(--_rChksumList.ui4_count == 0)
    {
        _rChksumList.pt_tail = NULL;
    }
    u2Chksum = dReadChkSum(RW_CHKSUM_RESULT) & 0xFFFF;
    pucChksum = (UINT8*)&u2Chksum;
    
    prPktBuf = (PKT_BUFF_T *)pt_entry;
    /* write checksum value to specific address. The purpose of using non-cachable address 
       is that driver is not necessary to flush cache again to gain performance */
    *((UINT8*)NONCACHE((UINT32)(prPktBuf->t_hw_chksum.pui1_chksum))) = *pucChksum;
    *((UINT8*)NONCACHE((UINT32)(prPktBuf->t_hw_chksum.pui1_chksum + 1))) = *(pucChksum + 1);
    //*((UINT16*)NONCACHE((UINT32)(prPktBuf->t_hw_chksum.pui1_chksum))) = u2Chksum;
    prPktBuf->t_hw_chksum.b_is_completed = TRUE;

    prPktBuf = (PKT_BUFF_T *)_rChksumList.pt_head;
    if(prPktBuf != NULL)
    {
        HalFlushInvalidateDCache();    
        vChkSum_Start(prPktBuf->t_hw_chksum.pui1_start, prPktBuf->t_hw_chksum.ui4_len);
        NOTE_DTCP("checksum de-queue and do again at irq service\n");
    }
}
#else
void vTx_Chksum_Offload(IN PKT_BUFF_T *pkt_buf)
{
  ERR_DTCP("Need to enable CONFIG_ETHER_HW_CHKSUM, and HW_CHKSUM_ON_THE_FLY in ether module\n");
}

void vChksumDeQueue(void)
{
  ERR_DTCP("Need to enable CONFIG_ETHER_HW_CHKSUM, and HW_CHKSUM_ON_THE_FLY in ether module\n");
}
#endif


/*******************************************************************************
**  _i4EtherTxChkSumVerify
**
**  descriptions:
**      The function is called to test checksum module
**  parameters:
**      none
**  result:
**  return: M6S56_DRV_OK
*******************************************************************************/
INT32 _i4EtherTxChkSumVerify(UINT32 u4Size, UINT32 u4Count, BOOL fgSingleSizeTest)
{
    UCHAR *pucSour;
#if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)     
    UCHAR *pucPhySour;
#endif    
    UINT32 *pucSourAddr;
    UINT32 u4Cnt, u4Align;
    UINT32 u4Sizetmp, u4TotalSize;
    UINT32 u4Seed = 0x1234abcd;
    INT32 i, i4Ret = 0;
    UINT32 u4SWChkSum;
    UINT32 u4HWChkSum;    
	//UINT32 u4Size, u4Count;
	
    NOTE_ETHER("_i4EtherTxChkSumVerify u4Size: %d, u4Count: %d\n", u4Size ,u4Count); 
 	/*generate test pattern*/
    u4TotalSize = u4Size + 32;
    //allocate 32 bit aligned buffer, maybe larger then u4TotalSize, it's ok.
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    #if CONFIG_SYS_MEM_PHASE2
    pucSourAddr = (UINT32 *)x_alloc_dma_mem(u4TotalSize + (4 - (u4TotalSize & 0x03)));
    #elif CONFIG_SYS_MEM_PHASE3
    pucSourAddr = (UINT32 *)x_mem_alloc(u4TotalSize + (4 - (u4TotalSize & 0x03)));
    #endif
    #else
    pucSourAddr = (UINT32 *)x_mem_alloc(u4TotalSize + (4 - (u4TotalSize & 0x03)));
    #endif
    
    
    for(i = 0; i < (INT32)u4TotalSize/4; i++)                            // fixed size
    {
      *(pucSourAddr + i) = u4Seed;
      u4Seed += 0x01020304;
      //UTIL_Printf("pucSourAddr: 0x%x, Value=0x%x\n", pucSourAddr,*(pucSourAddr + i) ); 
      
    }
	//verify by sw result
    HalFlushInvalidateDCache();    
    u4Cnt = (u4Count == 0) ? 1 : u4Count;
    u4Align = 0;
    for(i = 0; i < (INT32)u4Cnt; i++)                            
    {
    	if(fgSingleSizeTest)
    	{
    	   u4Align = u4Align+u4Size;
    	   u4Align = u4Align & 0x1F; //check 32 byte boundry
    	   u4Sizetmp = u4Size;
    	   
    	   pucSour = (UCHAR *)((UINT32)pucSourAddr + u4Align);

           u4SWChkSum = u2Checksum16SW((UINT8*)pucSour, u4Sizetmp);
           NOTE_ETHER("TxChecksum: 0%08x\n",u4SWChkSum);

#if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)             
            pucPhySour = (UCHAR *)PHYSICAL(((UINT32)pucSourAddr + u4Align));
            #ifdef DBG_CHKSUM	
            UTIL_Printf("pucPhySour =0x%x, u4Sizetmp=%d\n", pucPhySour,u4Sizetmp);
            #endif
            u4HWChkSum= u2Checksum16HW((UINT8*)pucPhySour, u4Sizetmp);
#else            
            u4HWChkSum= u2Checksum16HW((UINT8*)pucSour, u4Sizetmp);
#endif            

            //u4SWChkSum = u2Checksum16SW((UINT8*)pucSour, u4Sizetmp);
            
      
            
            if(u4HWChkSum != u4SWChkSum)
            {
               ERR_ETHER("Tx checksum error HW: 0x%08x, SW: 0x%08x, Length:%d, align:%d\n", 
                                u4HWChkSum, u4SWChkSum, u4Sizetmp, u4Align);
               i4Ret = -1;
            }
            	
    	}
        else
        {
          for(u4Sizetmp = 1; u4Sizetmp <= u4Size; u4Sizetmp++)        // size increase from 1 to specific value
          {
            /* add an offset to test if sour addresses can be byte alignment */
            u4Align = u4Align & 0x1F; //check 32 byte boundry
            pucSour = (UCHAR *)((UINT32)pucSourAddr + u4Align);

            u4SWChkSum = u2Checksum16SW((UINT8*)pucSour, u4Sizetmp);
            NOTE_ETHER("TxChecksum: 0%08x\n",u4SWChkSum);

#if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)             
            pucPhySour = (UCHAR *)PHYSICAL(((UINT32)pucSourAddr + u4Align));
            #ifdef DBG_CHKSUM	
            NOTE_ETHER("pucPhySour =0x%x, u4Sizetmp=%d\n", pucPhySour,u4Sizetmp);
            #endif
            u4HWChkSum= u2Checksum16HW((UINT8*)pucPhySour, u4Sizetmp);
#else            
            u4HWChkSum= u2Checksum16HW((UINT8*)pucSour, u4Sizetmp);
#endif            

            //u4SWChkSum = u2Checksum16SW((UINT8*)pucSour, u4Sizetmp);
            
      
            
            if(u4HWChkSum != u4SWChkSum)
            {
               ERR_ETHER("Tx checksum error HW: 0x%08x, SW: 0x%08x, Length:%d, align:%d\n", 
                                u4HWChkSum, u4SWChkSum, u4Sizetmp, u4Align);
               i4Ret = -1;
            }
            u4Align++;
          }
        }  
    }
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    #if CONFIG_SYS_MEM_PHASE2
    x_free_dma_mem(pucSourAddr);
    #elif CONFIG_SYS_MEM_PHASE3
    x_mem_free(pucSourAddr);
    #endif
    #else
    x_mem_free(pucSourAddr);
    #endif
    
 
    if(i4Ret == 0)
    {
    	if(fgSingleSizeTest)
    	{
    	NOTE_ETHER("Tx CheckSum verify, length: %d, %d times ok.\n", u4Size, u4Cnt);
        }
        else
        {
        NOTE_ETHER("Tx CheckSum verify, length:from 1->%d, %d times ok.\n", u4Size, u4Cnt);
        }
    }
    else
    {
        NOTE_ETHER("TX CheckSum verify fail.\n");
    }
    return(i4Ret);
}

/*******************************************************************************
**  _i4EtherRxChkSumVerify
**
**  descriptions:
**      The function is called to test checksum module
**  parameters:
**      none
**  result:
**  return: M6S56_DRV_OK
*******************************************************************************/
INT32 _i4EtherRxChkSumVerify(UINT32 u4Size, UINT32 u4Count, BOOL fgSingleSizeTest)
{
    UCHAR *pucSour;
#if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)     
    UCHAR *pucPhySour;
#endif    
    UINT32 *pucSourAddr;
    UINT32 u4Cnt, u4Align;
    UINT32 u4Sizetmp, u4TotalSize;
    UINT32 u4Seed = 0x1234abcd;
    INT32 i, i4Ret = 0;
    UINT32 u4SWChkSum;
    UINT32 u4HWChkSum;    
	//UINT32 u4Size, u4Count;
	
    NOTE_ETHER("_i4EtherRxChkSumVerify u4Size: %d, u4Count: %d\n", u4Size ,u4Count); 
 	/*generate test pattern*/
    u4TotalSize = u4Size + 32;
    //allocate 32 bit aligned buffer, maybe larger then u4TotalSize, it's ok.
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    #if CONFIG_SYS_MEM_PHASE2
    pucSourAddr = (UINT32 *)x_alloc_dma_mem(u4TotalSize + (4 - (u4TotalSize & 0x03)));
    #elif CONFIG_SYS_MEM_PHASE3
    pucSourAddr = (UINT32 *)x_mem_alloc(u4TotalSize + (4 - (u4TotalSize & 0x03)));
    #endif
    #else
    pucSourAddr = (UINT32 *)x_mem_alloc(u4TotalSize + (4 - (u4TotalSize & 0x03)));
    #endif
    
    
    for(i = 0; i < (INT32)u4TotalSize/4; i++)                            // fixed size
    {
      *(pucSourAddr + i) = u4Seed;
      u4Seed += 0x01020304;
      //NOTE_ETHER("pucSourAddr: 0x%x, Value=0x%x\n", pucSourAddr,*(pucSourAddr + i) ); 
      
    }
	//verify by sw result
    HalFlushInvalidateDCache();    
    u4Cnt = (u4Count == 0) ? 1 : u4Count;
    u4Align = 0;
    
    for(i = 0; i < (INT32)u4Cnt; i++)                            
    {
      if(fgSingleSizeTest)
      {
    	   u4Align = u4Align+u4Size;
    	   u4Align = u4Align & 0x1F; //check 32 byte boundry
    	   u4Sizetmp = u4Size;
    	   pucSour = (UCHAR *)((UINT32)pucSourAddr + u4Align);

            u4SWChkSum = u2RxChecksum16SW((UINT8*)pucSour, u4Sizetmp);
            NOTE_ETHER("RxChecksum: 0%08x\n",u4SWChkSum);
           
#if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)             
            pucPhySour = (UCHAR *)PHYSICAL(((UINT32)pucSourAddr + u4Align));
            #ifdef DBG_CHKSUM	
            NOTE_ETHER("pucPhySour =0x%x, u4Sizetmp=%d\n", pucPhySour,u4Sizetmp);
            #endif
            u4HWChkSum= u2RxChecksum16HW((UINT8*)pucPhySour, u4Sizetmp);
#else            
            u4HWChkSum= u2RxChecksum16HW((UINT8*)pucSour, u4Sizetmp);
#endif            

            //u4SWChkSum = u2RxChecksum16SW((UINT8*)pucSour, u4Sizetmp);
                       
            if(u4HWChkSum != u4SWChkSum)
            {
               ERR_ETHER("Rx checksum error HW: 0x%08x, SW: 0x%08x, Length:%d, align:%d\n", 
                                u4HWChkSum, u4SWChkSum, u4Sizetmp, u4Align);
               i4Ret = -1;
            }
    	   
    	   
      }   
      else
      {
        for(u4Sizetmp = 1; u4Sizetmp <= u4Size; u4Sizetmp++)        // size increase from 1 to specific value
        {
            /* add an offset to test if sour addresses can be byte alignment */
            u4Align = u4Align & 0x1F; //check 32 byte boundry
            pucSour = (UCHAR *)((UINT32)pucSourAddr + u4Align);

            u4SWChkSum = u2RxChecksum16SW((UINT8*)pucSour, u4Sizetmp);
            NOTE_ETHER("RxChecksum: 0%08x\n",u4SWChkSum);
            
#if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)             
            pucPhySour = (UCHAR *)PHYSICAL(((UINT32)pucSourAddr + u4Align));
            #ifdef DBG_CHKSUM	
            NOTE_ETHER("pucPhySour =0x%x, u4Sizetmp=%d\n", pucPhySour,u4Sizetmp);
            #endif
            u4HWChkSum= u2RxChecksum16HW((UINT8*)pucPhySour, u4Sizetmp);
#else            
            u4HWChkSum= u2RxChecksum16HW((UINT8*)pucSour, u4Sizetmp);
#endif            

            //u4SWChkSum = u2RxChecksum16SW((UINT8*)pucSour, u4Sizetmp);
            
      
            
            if(u4HWChkSum != u4SWChkSum)
            {
               ERR_ETHER("Rx checksum error HW: 0x%08x, SW: 0x%08x, Length:%d, align:%d\n", 
                                u4HWChkSum, u4SWChkSum, u4Sizetmp, u4Align);
               i4Ret = -1;
            }
            u4Align++;
        }
      }  
    }
    #if (CONFIG_DRV_LINUX_DATA_CONSISTENCY)
    #if CONFIG_SYS_MEM_PHASE2
    x_free_dma_mem(pucSourAddr);
    #elif CONFIG_SYS_MEM_PHASE3
    x_mem_free(pucSourAddr);
    #endif
    #else
    x_mem_free(pucSourAddr);
    #endif
    

    if(i4Ret == 0)
    {
    	if(fgSingleSizeTest)
    	{
    	NOTE_ETHER("Rx CheckSum verify, length: %d, %d times ok.\n", u4Size, u4Cnt);
        }
    	else
    	{
        NOTE_ETHER("Rx CheckSum verify, length:from 1->%d, %d times ok.\n", u4Size, u4Cnt);
        }
    }
    else
    {
        NOTE_ETHER("Rx CheckSum verify fail.\n");
    }
    return(i4Ret);
}


