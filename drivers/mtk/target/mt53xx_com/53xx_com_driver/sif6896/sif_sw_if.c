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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: sif_sw_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_sw_if.c
 *  This file implement serial interface SIF function by software controlled GPIO pin.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "sif_debug.h"
#include "sif_hw.h"
#include "sif_sw_drvif.h"
#include "drvcust_if.h"

#include "x_lint.h"
#include "c_model.h"
LINT_EXT_HEADER_BEGIN

#include "x_pinmux.h"

#include "x_assert.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_timer.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define SIF_SW_SUPPORT_DELAY_SCL
#ifdef SIF_SW_SUPPORT_DELAY_SCL          //default: polling mode 
  //#define SIF_SW_DELAYSCL_INTERRUPT    //define interrupt mode here
#endif
#define STATIC      //debug
//#define lousy
//#define STATIC  static    //release
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// *********************************************************************
// Define I2C Read/Write Flag  for fgSeq_Read /vI2CRead
// *********************************************************************

#define SIF_FG_SEQREAD      1
#define SIF_FG_RANDREAD     0
#define SIF_WAITSLAVE_DELAY 3
// *********************************************************************
// Timing Definition for System Clock
// *********************************************************************
#define SIF_ACK_DELAY  200 // time out for acknowledge check
#define SIF_BUS_DELAY  255 // time out bus arbitration
#define SIF_SYNCTIMEOUT_MAX 1000        //1000ms
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define SIF_BASE           BIM_BASE
#define SIF_REG_LENGTH      0

#define SIF_HDCP_SIZE  320

// HDMI EDID 1 EERPOM GPIO pin configuration.
#ifndef SIF_EDID1_SDA
#define SIF_EDID1_SDA  (35)
#endif
#ifndef SIF_EDID1_SCL
#define SIF_EDID1_SCL  (36)
#endif

// HDMI EDID 2 EERPOM GPIO pin configuration.
#ifndef SIF_EDID2_SDA
#define SIF_EDID2_SDA  (37)
#endif
#ifndef SIF_EDID2_SCL
#define SIF_EDID2_SCL  (38)
#endif

// HDMI EDID 3 EERPOM GPIO pin configuration.
#ifndef SIF_EDID3_SDA
#define SIF_EDID3_SDA  (44)
#endif
#ifndef SIF_EDID3_SCL
#define SIF_EDID3_SCL  (45)
#endif

#define SIF_EDID_SIZE  256

#define SIF_EEP_DEVICE_ADDR  0xA0
#define TIMEOUT_SEMA_ACK    10
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static UINT32 _u4SIF_SDA;
static UINT32 _u4SIF_SCL;

// old function of SDA, SCL pin.
static INT32 _i4SIF_SDA_Func;
static INT32 _i4SIF_SCL_Func;
static INT32 _i4SIF_SDA_Pin;
static INT32 _i4SIF_SCL_Pin;

// mark initialization.
static UINT32 _u4SIF_Init;

//
static BOOL _fgOptionWaitSlaveDevice = FALSE;
static UINT16 _u2SIF_SYNC_TIMEOUT= 100;     //100ms
#ifdef SIF_SW_DELAYSCL_INTERRUPT
static HANDLE_T h_sema_int=NULL;
static HANDLE_T h_sema_int_sda=NULL;
#endif 
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** SIF_READSDA
 * read IIC SDA.
 *
 * @param VOID
 * @return 1 or 0.
 */
//-------------------------------------------------------------------------
static INT32 SIF_READSDA(void)
{
    return GPIO_GetIn((INT32)_u4SIF_SDA);
}

static INT32 SIF_READSCL(void)
{
    return GPIO_GetIn((INT32)_u4SIF_SCL);
}

//-------------------------------------------------------------------------
/** SIF_SDALOW
 * output IIC SDA low.
 *
 * @param VOID
 * @return VOID.
 */
//-------------------------------------------------------------------------
static void SIF_SDALOW(void)
{
    GPIO_SetOut( (INT32) _u4SIF_SDA , 0 );
}

//-------------------------------------------------------------------------
/** SIF_SDAHIGH
 * output IIC SDA high.
 *
 * @param VOID
 * @return VOID.
 */
//-------------------------------------------------------------------------
static INT32 SIF_SDAHIGH(void)
{
    UINT32 u4Val;
    INT32 i4Idx;
    GPIO_SetOut( (INT32) _u4SIF_SDA , 1 );
    // Read input value.
    i4Idx = ((UINT32)_u4SIF_SDA >> 5);
    u4Val = GPIO_IN_REG(i4Idx);    
    return ((u4Val & (1U << (_u4SIF_SDA & GPIO_INDEX_MASK))) ? 1U : 0);
}

//-------------------------------------------------------------------------
/** SIF_SCLLOW
 * output IIC SCL low.
 *
 * @param VOID
 * @return VOID.
 */
//-------------------------------------------------------------------------
static void SIF_SCLLOW(void)
{
    GPIO_SetOut( (INT32) _u4SIF_SCL , 0 );
}

//-------------------------------------------------------------------------
/** SIF_SCLHIGH
 * output IIC SCL high.
 *
 * @param VOID
 * @return VOID.
 */
//-------------------------------------------------------------------------
static INT32 SIF_SCLHIGH(void)
{
    UINT32 u4Val;
    INT32 i4Idx;
    GPIO_SetOut( (INT32) _u4SIF_SCL , 1 );
    // Read input value.
    i4Idx = ((UINT32)_u4SIF_SCL >> 5);
    u4Val = GPIO_IN_REG(i4Idx);    
    return ((u4Val & (1U << (_u4SIF_SCL & GPIO_INDEX_MASK))) ? 1U : 0);
}

//-------------------------------------------------------------------------
/** _Sifsw_Delay1us
 * Delay routine (local function)
 *
 * @param u4Value: u4Value = 1->1us, 2->2us
 * @return NONE
 */
//-------------------------------------------------------------------------
 void _Sifsw_Delay1us(UINT32 u4Value)
{
#if 0
    volatile UINT32 u4Cnt;

    volatile UINT32 u4Loop;

    for (u4Cnt = 0; u4Cnt < u4Value; u4Cnt++)
    {
        for (u4Loop = 0; u4Loop < 25/*10*/; u4Loop++)
        {
            ;
        }
    }
#else
    if( u4Value >0 )
    {
        HAL_Delay_us(u4Value);
    }
#endif
}

#ifdef SIF_SW_DELAYSCL_INTERRUPT
static void _SIF_SclFreeIsr(INT32 i4Gpio, BOOL fgStatus)
{
    ASSERT( i4Gpio == _u4SIF_SCL );
    ASSERT( fgStatus == TRUE );
    VERIFY( x_sema_unlock( h_sema_int )==OSR_OK);
    LOG(9 , "[_SIF_SclFreeIsr]\n");
}
#endif
/*
static void _SIF_SdaFreeIsr(INT32 i4Gpio, BOOL fgStatus)
{
    ASSERT( i4Gpio == _u4SIF_SDA );
    ASSERT( fgStatus == TRUE );
    VERIFY( x_sema_unlock( h_sema_int_sda )==OSR_OK);
    LOG(9 , "[_SIF_SdaFreeIsr]\n");
}
*/
/*return 1: OK, 0: fail */
static INT32 SIF_WaitSclFree( UINT32 u4Delay/*ms*/)
{
#ifndef SIF_SW_DELAYSCL_INTERRUPT    //polling mode,  suggested by demonz
    BOOL fgRet=FALSE;
    if(u4Delay>SIF_SYNCTIMEOUT_MAX)
    {
        u4Delay=SIF_SYNCTIMEOUT_MAX;        
    }else if (u4Delay==0)
    {
        u4Delay = 1;                //min:1 ms
    }

    u4Delay *= 1000; // ms to us

    while(u4Delay--)              //bounded waiting
    //while(u4Delay)                  //infinite loop  //demonz
    {
        if( SIF_READSCL()==1)
        {
            fgRet = TRUE;
            break;
        }
        _Sifsw_Delay1us(1);
    }

    return fgRet;
#else                               // interrupt mode
    BOOL fgRet=FALSE; INT32 i4Set=0;
    if(u4Delay>SIF_SYNCTIMEOUT_MAX)
    {
        u4Delay=SIF_SYNCTIMEOUT_MAX;        
    }else if (u4Delay==0)
    {
        u4Delay = 1;                //min:1 ms
    }

    VERIFY(GPIO_Reg( _u4SIF_SCL, GPIO_TYPE_INTR_RISE, _SIF_SclFreeIsr ) >= 0);  //enable IRQ

   if( x_sema_lock_timeout( h_sema_int , u4Delay )!=OSR_OK )   
    {   // !OSR_OK
        //chances are SCL became free during setup isr
        //lets poll scl again
        if( SIF_READSCL()==1)
        {
            fgRet = TRUE;
        }
    }
    else
    {   //OSR_OK
        fgRet = TRUE;
    }
    GPIO_Intrq( _u4SIF_SCL , &i4Set);       //disable IRQ

    if(!fgRet)
    {
        Printf("[SIF_WaitSclFree] Byte Synchronization timeout\n"); //fatal error
    }

    return fgRet;
#endif
}

/*return 1: OK, 0: fail */ /*
static INT32 SIF_WaitSdaFree(void)
{
#if 1
  #ifdef SIF_SW_DELAYSCL_INTERRUPT
    BOOL fgRet=FALSE; INT32 i4Set=0;
    VERIFY(GPIO_Reg( _u4SIF_SDA, GPIO_TYPE_INTR_RISE, _SIF_SdaFreeIsr ) >= 0);  //enable IRQ
    if( x_sema_lock_timeout( h_sema_int_sda , TIMEOUT_SEMA_ACK )!=OSR_OK )
    {   // !OSR_OK
        //chances are SCL became free during setup isr
        //lets poll scl again
        if( SIF_READSDA()==1)
        {
            fgRet = TRUE;
        }
    }
    else
    {   //OSR_OK
        fgRet = TRUE;
    }
    //VERIFY(GPIO_Reg( _u4SIF_SCL, GPIO_TYPE_INTR_RISE, NULL ) >= 0);   
    GPIO_Intrq( _u4SIF_SDA , &i4Set);       //disable IRQ

    ASSERT(fgRet);

    return fgRet;
  #endif
#else
  #define SIF_WAIT_DELAY (1000*100)    //100 ms
    INT32 i4data, i4DelaySlot=SIF_WAIT_DELAY;
    do
    {
        i4data = SIF_READSDA();
        if( i4data ==1 )
        {
            break;
        }
        _Sifsw_Delay1us(1);        
    }while( --i4DelaySlot && (i4data != 1) );
    if( i4DelaySlot ==0 )
    {
        LOG(9, "[SIF_WaitSdaFree] SIF_WaitSdaFree timeout\n");
    }
    else 
    {
        LOG(9, "[SIF_WaitSdaFree] SCL==high after %d us\n",SIF_WAIT_DELAY -i4DelaySlot  );
    }
    return (i4DelaySlot? TRUE:FALSE);
#endif
} */

/*
static void SIF_WaitSlave(void)
{*/
    /************************************************************/
    /*** IF SDA IS FREE(HIGH) AND SCL IS OCCUPIED (LOW)       ***/
    /*** BY SPEC CH.7 MASTER HAS TO WAIT TILL SCL FREE        ***/
    /*** FOR SANITY'S SAKE, WE WAIT SCL ONLY FOR MILI-SECONDS ***/
    /************************************************************/
/*    if( _fgOptionWaitSlaveDevice && (SIF_READSDA()==1)  )  
    {
        SIF_READSCL();      //release SCL
        _Sifsw_Delay1us(SIF_WAITSLAVE_DELAY);
        if( SIF_READSCL()==0)
        {
            SIF_WaitSclFree(_u2SIF_SYNC_TIMEOUT);
        }
    }
}
*/
//-------------------------------------------------------------------------
/** _Sifsw_PutByte
 * I2C send UINT8 routine (local function)
 *
 * @param bValue UINT8 to be sent out
 * @param bBitDelay delay between bit
 * @return 1 or 0
 * @retval = 1 send UINT8 success
 * @retval = 0 send UINT8 fail (no ACK)
 */
//-------------------------------------------------------------------------
UINT8 _Sifsw_PutByte(UINT8 bValue, UINT8 bBitDelay)
{
    UINT8 bBitMask = 0x80;

    UINT8 bData;

    BOOL fgAckDetected=FALSE;
    INT32 i;


    // step 1 : 8-bit data transmission
    while (bBitMask)
    {
        if (bBitMask & bValue)
        {
            SIF_SDAHIGH();
        }
        else
        {
            SIF_SDALOW();
        }

        _Sifsw_Delay1us(bBitDelay);
#ifndef SIF_SW_SUPPORT_DELAY_SCL
        SIF_SCLHIGH();
#else   //Byte Synchronization
        if( (SIF_READSCL()!=1) && (bBitMask == 0x80) && (_fgOptionWaitSlaveDevice==TRUE))   //0x80:1st bit
        {
            if( !SIF_WaitSclFree(_u2SIF_SYNC_TIMEOUT))
            {
                Printf("_Sifsw_PutByte: Byte Synchronization Timout\n");
            }
            //ASSERT( SIF_READSCL()==1 );
        }
#endif
        _Sifsw_Delay1us(bBitDelay);

        SIF_SCLLOW();
        _Sifsw_Delay1us(bBitDelay);

        bBitMask = bBitMask >> 1;
    }

    //workaround for SS DDP
    // DDP might pull SCL before SDA (not standard), have to check this case at end of byte before ack
    if( (SIF_SCLHIGH()!=1) && (_fgOptionWaitSlaveDevice==TRUE))  
    {
            if( !SIF_WaitSclFree(_u2SIF_SYNC_TIMEOUT))
            {
                Printf("_Sifsw_PutByte: Byte Synchronization Timout\n");
            }
    }

    // step 2 : slave acknowledge check
    //SIF_SDAHIGH();
    //SIF_SDALOW();
    SIF_READSDA();      //release SDA   //TC Wu
    _Sifsw_Delay1us(bBitDelay);

#if 0 // demonz
    i = 100; // 100us: GPIO status might be delayed
#else
    i = 100 + 10*bBitDelay;   //wait at least 10us
#endif
    
    do 
    {
        bData = (UINT8) SIF_READSDA();
        if( bData == 0 )    //SDA Low?
        {
            fgAckDetected= TRUE;
            break;
        }
        _Sifsw_Delay1us(1);
    } while ( !fgAckDetected && (--i>0) );

    //_Sifsw_Delay1us(bBitDelay);
#ifndef SIF_SW_SUPPORT_DELAY_SCL
    SIF_SCLHIGH();
#else //work around here
    if( SIF_SCLHIGH()!=1 && (_fgOptionWaitSlaveDevice==TRUE))
    {
            if( !SIF_WaitSclFree(_u2SIF_SYNC_TIMEOUT))
            {
                Printf("_Sifsw_PutByte: Byte Synchronization Timout\n");
            }
            //ASSERT( SIF_READSCL()==1 );
    }
#endif
    _Sifsw_Delay1us(bBitDelay);

    //_Sifsw_Delay1us(bDelay);
    SIF_SCLLOW(); // end ACK polling
    _Sifsw_Delay1us(bBitDelay);

    if (fgAckDetected)
    {
        return (TRUE); // return TRUE if ACK detected
    }
    else
    {
        //Sifsw_SendStop(bBitDelay);
        return (FALSE); // return FALSE if time out
    }
}

//-------------------------------------------------------------------------
/** _Sifsw_GetByte
 * I2C read UINT8 routine (local function)
 *
 * @param prValue pointer of data to be sent out
 * @param fgSeqRead sequential read
 * @param bBitDelay delay between bit
 * @return NONE
 */
//-------------------------------------------------------------------------
 void _Sifsw_GetByte(UINT8 *prValue, UINT8 fgSeqRead, UINT8 bBitDelay)
{
    UINT8 bBitMask = 0x80;

    UINT8 bData;

    ASSERT(prValue);

    *prValue = 0;
    SIF_SDAHIGH();
    _Sifsw_Delay1us(bBitDelay);

    // step 1 : 8-bit data reception
    while (bBitMask)
    {
#ifndef SIF_SW_SUPPORT_DELAY_SCL
        SIF_SCLHIGH();
#else   //Byte Synchronization
        if( (SIF_READSCL()!=1) && (bBitMask == 0x80) && (_fgOptionWaitSlaveDevice==TRUE))   
        {
            if( !SIF_WaitSclFree(_u2SIF_SYNC_TIMEOUT))
            {
                Printf("[_Sifsw_GetByte] Byte Synchronization timeout\n"
);
            }
            //ASSERT( SIF_READSCL()==1 );
        }
#endif

        _Sifsw_Delay1us(bBitDelay);
        bData = (UINT8) SIF_READSDA();

        if (bData)
        {
            *prValue = *prValue | bBitMask;
        }

        //_Sifsw_Delay1us(bBitDelay);
        SIF_SCLLOW();             // ready for next clock out
        _Sifsw_Delay1us(bBitDelay);

        bBitMask = bBitMask >> 1; // shift bit mask & clock delay
    }

    // step 2 : acknowledgement to slave
    if (fgSeqRead)
    {
        SIF_SDALOW();
    }
    else
    {
        SIF_SDAHIGH();
    }

    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLLOW();
    _Sifsw_Delay1us(bBitDelay);

    SIF_READSDA();  //SIF_SDAHIGH();    //TC Wu
    _Sifsw_Delay1us(bBitDelay);
}

//-------------------------------------------------------------------------
/** _Sifsw_SendStart
 * I2C send UINT8 with device address routine (local function)
 *
 * @param bDevAddr device address
 * @param bBitDelay delay between bit
 * @return 1 or 0
 * @retval = 1 send UINT8 success
 * @retval = 0 send UINT8 fail (no ACK)
 */
//-------------------------------------------------------------------------
 UINT8 _Sifsw_SendStart(UINT8 bDevAddr, UINT8 bBitDelay)
{
#ifndef SIF_SW_SUPPORT_DELAY_SCL
    SIF_SCLHIGH();
#else   //byte synchronization
    if( SIF_READSCL()!=1 && (_fgOptionWaitSlaveDevice==TRUE))
    {
            if( !SIF_WaitSclFree(_u2SIF_SYNC_TIMEOUT) )
            {
                Printf("_Sifsw_SendStart: Byte Synchronization timeout\n");
            }
            //ASSERT( SIF_READSCL()==1 );
    }
#endif
    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SDALOW();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLLOW();
    _Sifsw_Delay1us(bBitDelay);

    return (_Sifsw_PutByte(bDevAddr, bBitDelay));
}

//-------------------------------------------------------------------------
/** _Sifsw_SendStop
 * I2C stop routine (local function)
 *
 * @param bBitDelay delay between bit
 * @return NONE
 */
//-------------------------------------------------------------------------
 void _Sifsw_SendStop(UINT8 bBitDelay)
{
    SIF_SDALOW();

    _Sifsw_Delay1us(bBitDelay);

#ifndef SIF_SW_SUPPORT_DELAY_SCL
    SIF_SCLHIGH();
#else   //byte synchronization
    if( SIF_READSCL()!=1 && (_fgOptionWaitSlaveDevice==TRUE))
    {
            if( !SIF_WaitSclFree(_u2SIF_SYNC_TIMEOUT) )
            {
                Printf("_Sifsw_SendStart: Byte Synchronization timeout\n");
            }
            //ASSERT( SIF_READSCL()==1 );
    }
#endif
	_Sifsw_Delay1us(bBitDelay);

    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SDAHIGH();
    _Sifsw_Delay1us(bBitDelay);
}

//-------------------------------------------------------------------------
/** _SifSWWriteEeprom
 *  SIF CLI SW IIC write EEPROM function.
 * @param u4SDA 	GPIO number as SDA.
 * @param u4SCL 	GPIO number as SCL.
 * @param pbData pointer of data to be sent out 
 *  @retval  1  SUCCESS.
 *  @retval  0  FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 SifSW_WriteEeprom(UINT32 u4SDA, UINT32 u4SCL, UINT8 *pu1Data, UINT8 u1DataCount)
{
    UINT8 u1Ret = 0;
    INT32 fgSet;
    UINT32 u4Index = 0;  
    UINT32 u4Cnt;   
    UINT32 u4DeviceAddr;   

    UINT32 u4EdidWP, u4EdidWPEnable, u4Flag=0;

    // disable write protect.
    if (0 == DRVCUST_InitQuery(eEdidEepromWPGpio, &u4EdidWP) && 
       (0 == DRVCUST_InitQuery(eEdidEepromWPEnablePolarity, &u4EdidWPEnable)))
    {
        u4Flag = 1;
        VERIFY(BSP_PinGpioSel((INT32)u4EdidWP) == 0);  
        fgSet = !(INT32)u4EdidWPEnable;
        VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
			
    }

    Printf("Write EDID EEPROM SDA = GPIO%d, SCL = GPIO%d.\n", (int) u4SDA, (int) u4SCL);

    // initial SIF sda, scl pin.
    SIFSW_Init(u4SDA, u4SCL);

    u4Index = 0;
    // EEPROM page length = 8 bytes.
    while (u4Index < u1DataCount /*SIF_EDID_SIZE*/)
    {
        u4Cnt = (UINT32) (((u1DataCount /*SIF_EDID_SIZE*/ -  u4Index) > 8) ? 8 : (u1DataCount /*SIF_EDID_SIZE*/ -  u4Index)); 

        // lower device address [b3, b2, b1] are from address [A10, A9, A8].
        u4DeviceAddr = (u4Index >> 7) & 0x0E;        
        u4DeviceAddr |= 0xA0;
        
        u1Ret = SIFSW_WriteData(0xA0, (UINT8) u4Index, (UINT8) u4Cnt, (UINT8 *)&pu1Data[u4Index], 0xFF);

        if (u1Ret == 0)
        {
            Printf("write fail.\n");

            // enable write protect.
            if (u4Flag)
            {
                VERIFY(BSP_PinGpioSel((INT32)u4EdidWP) == 0);  
                fgSet = (INT32)u4EdidWPEnable;
                VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
            }

            SIFSW_Remove();
            return 0;
        }
        // Wait EEPROM to program.
        x_thread_delay(20);
        
        u4Index += u4Cnt;
    }

    Printf("Write successfully !\n");

    // enable write protect.
    if (u4Flag)
    {
        VERIFY(BSP_PinGpioSel((INT32)u4EdidWP) == 0);  
        fgSet = (INT32)u4EdidWPEnable;
        VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
    }

    SIFSW_Remove();

    return 1;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** SIFSW_Init
 * I2C initialization routine
 *
 * @param u4SDA 	GPIO number as SDA.
 * @param u4SCL 	GPIO number as SCL.
 * @return NONE
 */
//-------------------------------------------------------------------------
void SIFSW_Init(UINT32 u4SDA, UINT32 u4SCL)
{
    if ((_u4SIF_SDA != u4SDA) || (_u4SIF_SCL != u4SCL))
    {
        // Get old pin number, function, and store it.
        _i4SIF_SDA_Func = BSP_PinGpioGet((INT32)u4SDA, (UINT32 *)&_i4SIF_SDA_Pin);
        _i4SIF_SCL_Func = BSP_PinGpioGet((INT32)u4SCL, (UINT32 *)&_i4SIF_SCL_Pin);

        _u4SIF_SDA = u4SDA;
        
        // set SDA as GPIO by select pin mux
        VERIFY(BSP_PinGpioSel((INT32)_u4SIF_SDA) == 0);

        _u4SIF_SCL = u4SCL;
        // set SCL as GPIO.
        VERIFY(BSP_PinGpioSel((INT32)_u4SIF_SCL) == 0);
    }

#ifdef SIF_SW_DELAYSCL_INTERRUPT
    //create semaphore
    VERIFY(GPIO_Init()==OSR_OK);
    VERIFY(x_sema_create( &h_sema_int, X_SEMA_TYPE_BINARY , X_SEMA_STATE_LOCK )==OSR_OK);
    VERIFY(x_sema_create( &h_sema_int_sda, X_SEMA_TYPE_BINARY , X_SEMA_STATE_LOCK )==OSR_OK);
#endif 

    _u4SIF_Init = TRUE;
/*    
    SIF_SCLHIGH();
    _Sifsw_Delay1us(2);
    SIF_SDAHIGH();
    _Sifsw_Delay1us(2); */  //this part causes Non-Stop option failed
}

//-------------------------------------------------------------------------
/** SIFSW_Remove
 * I2C deactivate routine: restore sda and scl pin attribute.
 *
 * @param u4SDA 	GPIO number as SDA.
 * @param u4SCL 	GPIO number as SCL.
 * @return NONE
 */
//-------------------------------------------------------------------------
void SIFSW_Remove(void)
{
    if (!_u4SIF_Init)
    {
        return;
    }
    _u4SIF_Init = FALSE;

    // restore old function of SDA, SCL.
    VERIFY(BSP_PinSet((INT32)_i4SIF_SDA_Pin, (INT32)_i4SIF_SDA_Func) == 0);
    VERIFY(BSP_PinSet((INT32)_i4SIF_SCL_Pin, (INT32)_i4SIF_SCL_Func) == 0);

    // clear variable.
    _u4SIF_SDA = 0;
    _u4SIF_SCL = 0;
    _i4SIF_SDA_Pin = 0;
    _i4SIF_SCL_Pin = 0;
    _i4SIF_SDA_Func = 0;
    _i4SIF_SCL_Func = 0;

#ifdef SIF_SW_DELAYSCL_INTERRUPT
    //create semaphore
    VERIFY(x_sema_delete( h_sema_int )==OSR_OK);
    VERIFY(x_sema_delete( h_sema_int_sda )==OSR_OK);
#endif 
    
    _fgOptionWaitSlaveDevice = FALSE;
    _u2SIF_SYNC_TIMEOUT = 100;  //ms
}

#if 0

//-------------------------------------------------------------------------
/** SIFSW_WaitAckOK
 * I2C ack testing routine
 *
 * @param bDevAddr device address
 * @param wDelayUs delay before next trial
 * @param bRetry retry times
 * @return 1 or 0
 * @retval = 1 device return ACK success
 * @retval = 0 device return ACK fail
 */
//-------------------------------------------------------------------------

BOOL SIFSW_WaitAckOK(UINT8 bDevAddr, UINT16 wDelayUs, UINT8 bRetry)
{
    bDevAddr &= 0xFE;

    do
    {
        if (!_Sifsw_SendStart(bDevAddr, 2))
        {
            _Sifsw_SendStop(2);

            bRetry--;

            if (wDelayUs != 0)
            {
                _Sifsw_Delay1us(wDelayUs *2);
            }
            if (bRetry != 0)
            {
                continue;
            }
        }

        _Sifsw_SendStop(2);
        break;
    } while (1);

    if (bRetry == 0)
    {
        return (FALSE);
    }

    return (TRUE);
}
#endif

//-------------------------------------------------------------------------
/** SIFSW_WriteData
 * I2C write command (general type)
 *
 * @param bDevAddr device address
 * @param bSubAddr sub-address
 * @param bDataCount numbber of bytes to be sent out
 * @param pbData pointer of data to be sent out
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 8: halt and wait for ACK (full implementation of spec ch.7.2)
 * @return 1 or 0
 * @retval = 1 write success
 * @retval = 0 write fail
 */
//-------------------------------------------------------------------------

UINT8 SIFSW_WriteData(UINT8 bDevAddr, UINT8 bSubAddr, UINT8 bDataCount, UINT8 *pbData,
                      UINT16 bOption)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;

    ASSERT(pbData);

    bDevAddr &= 0xFE;

    bBitDelay = (UINT8) (bOption & 0x07);
    bByteDelay = (UINT8) ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) >> 6);
    _fgOptionWaitSlaveDevice = ((bOption & 0x100) >> 8);
        
#if 0       //stop if stop bit fail
    if ((!_Sifsw_SendStart(bDevAddr, bBitDelay)) && (fgNeedACKCheck))
    {
        return (FALSE);
    }
#else
    _Sifsw_SendStart(bDevAddr, bBitDelay);
#endif

    _Sifsw_Delay1us(bByteDelay);

    //if ((fgNeedACKCheck) && (!_Sifsw_PutByte(bSubAddr, bBitDelay)))
    if ( (!_Sifsw_PutByte(bSubAddr, bBitDelay)) && (fgNeedACKCheck) )
    {
        return (FALSE);
    }

    _Sifsw_Delay1us(bByteDelay);

    while (bDataCount)
    {
        //if ((fgNeedACKCheck) && (!_Sifsw_PutByte(*(pbData++), bBitDelay))) // Data Content Write
        if ((!_Sifsw_PutByte(*(pbData++), bBitDelay))&& (fgNeedACKCheck)) // Data Content Write
        {
            return (FALSE);                                                // Device Address exceeds the range
        }

        _Sifsw_Delay1us(bByteDelay);
        bDataCount--;
    }

    _Sifsw_SendStop(bBitDelay);

    return (TRUE);
}

//-------------------------------------------------------------------------
/** SIFSW_ReadData
 * I2C read command (general type)
 *
 * @param bDevAddr device address
 * @param bSubAddr sub-address
 * @param bDataCount numbber of bytes to be read
 * @param pbData pointer of data to be read
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 8: halt and wait for ACK (full implementation of spec ch.7.2)
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
//-------------------------------------------------------------------------
UINT8 SIFSW_ReadData(UINT8 bDevAddr, UINT8 bSubAddr, UINT16 bDataCount, UINT8 *pbData,
                     UINT16 bOption)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;

    ASSERT(pbData);

    bDevAddr &= 0xFE;

    bBitDelay = (UINT8) (bOption & 0x07);
    bByteDelay = (UINT8) ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) >>6 );
    _fgOptionWaitSlaveDevice = ((bOption & 0x100) >> 8);
#if 0
    if ( (!_Sifsw_SendStart(bDevAddr, bBitDelay)) && (fgNeedACKCheck))
    {
        return (FALSE);
    }
#else
    _Sifsw_SendStart(bDevAddr, bBitDelay);
#endif
    _Sifsw_Delay1us(bByteDelay);

//    if ((fgNeedACKCheck) && (!_Sifsw_PutByte(bSubAddr, bBitDelay)))
    if ( (!_Sifsw_PutByte(bSubAddr, bBitDelay))&& (fgNeedACKCheck) )
    {
        return (FALSE);
    }

    _Sifsw_Delay1us(bByteDelay);
    bDevAddr |= 0x01;

//    if ((fgNeedACKCheck) && (!_Sifsw_SendStart(bDevAddr, bBitDelay)))
    if ((!_Sifsw_SendStart(bDevAddr, bBitDelay))&&(fgNeedACKCheck))
    {
        return (FALSE);
    }

    _Sifsw_Delay1us(bByteDelay);

    while (bDataCount)
    {
        if (bDataCount == 1)
        {
            _Sifsw_GetByte(pbData++, SIF_FG_RANDREAD, bBitDelay);
        }
        else
        {
            _Sifsw_GetByte(pbData++, SIF_FG_SEQREAD, bBitDelay);
        }

        _Sifsw_Delay1us(bByteDelay);
        bDataCount--;
    }

    _Sifsw_SendStop(bBitDelay);

    return (TRUE);
}

//-------------------------------------------------------------------------
/** SIFSW_WriteDataNoSubAddr
 * I2C write command (no ack type)
 *
 * @param bDevAddr device address
 * @param bDataCount numbber of bytes to be sent out
 * @param pbData pointer of data to be sent out
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 8: halt and wait for ACK (full implementation of spec ch.7.2)
 * @param bOption bit 9: Not to send stop bit at the end of transaction
 * @param bOption bit10: Not to send start bit at the beginning of transaction
 * @return 1 or 0
 * @retval > 0 write success, number of bytes sent successfully
 * @retval = 0 write fail
 */
//-------------------------------------------------------------------------
UINT8 SIFSW_WriteDataNoSubAddr(UINT8 bDevAddr, UINT8 bDataCount, UINT8 *pbData, UINT32 bOption, UINT16 u2Delay,UINT16 u2SyncTimeout)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;
    //BOOL fgNoStart;
    BOOL fgNoStop;
    UINT8 bDataLeftCount=bDataCount;
    if( u2SyncTimeout > SIF_SYNCTIMEOUT_MAX )
    {
        _u2SIF_SYNC_TIMEOUT = SIF_SYNCTIMEOUT_MAX;
    } else
    {
        _u2SIF_SYNC_TIMEOUT = u2SyncTimeout;
    }

    ASSERT(pbData);

    bDevAddr &= 0xFE;

    bBitDelay = (UINT8) u2Delay ; //bOption & 0x07; 
    bByteDelay = (UINT8) ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) >>6);
    _fgOptionWaitSlaveDevice = ((bOption & 0x100) >> 8);
    //fgNoStart = ((bOption&0x400)>>10);        //not used anymore
    fgNoStop =((bOption&0x200)>>9);
#ifdef lousy
    Printf("SIFSW_WriteDataNoSubAddr: bDevAddr=0x%x, bDataCount=%d, pbData[0]=0x%x, bOption=0x%x, u2Delay=%d\n",bDevAddr,  bDataCount ,pbData[0] ,bOption, u2Delay);    //lousy
#endif    

    if ( (!_Sifsw_SendStart(bDevAddr, bBitDelay)) && (fgNeedACKCheck))
    {
        _Sifsw_SendStop(bBitDelay);     //No ack and exit--> release bus first            
        return (0);                     //target device no response
    }
    _Sifsw_Delay1us(bByteDelay);

    while (bDataLeftCount)
    {
        if ( (!_Sifsw_PutByte(*(pbData++), bBitDelay)) && (fgNeedACKCheck)) // Data Content Write
        {
            _Sifsw_SendStop(bBitDelay);     //No ack and exit--> release bus first            
            return (bDataCount-bDataLeftCount);     //returns successul bytes
        }
        _Sifsw_Delay1us(bByteDelay);
        bDataLeftCount--;
    }

    if(!fgNoStop)                   //STOP BIT
    {
        _Sifsw_SendStop(bBitDelay);     //release bus
    }

    return (bDataCount-bDataLeftCount);    //returns successul bytes
}

//-------------------------------------------------------------------------
/** SIFSW_ReadDataNoSubAddr
 * I2C read command (no ack type)
 *
 * @param bDevAddr device address
 * @param bDataCount numbber of bytes to be rad
 * @param pbData pointer of data to be rad
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 8: halt and wait for ACK (full implementation of spec ch.7.2)
 * @param bOption bit 9: Reserve
 * @param bOption bit10: Not to send start bit at the beginning of transaction
 * @return 1 or 0
 * @retval > 0 write success, number of bytes sent successfully
 * @retval = 0 read fail
 */
//-------------------------------------------------------------------------
UINT8 SIFSW_ReadDataNoSubAddr(UINT8 bDevAddr, UINT16 bDataCount, UINT8 *pbData, UINT32 bOption, UINT16 u2Delay,UINT16 u2SyncTimeout)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;
    //BOOL fgNoStart;
    //BOOL fgNoStop;
    UINT32 bDataLeftCount=bDataCount;
    if( u2SyncTimeout > SIF_SYNCTIMEOUT_MAX )
    {
        _u2SIF_SYNC_TIMEOUT = SIF_SYNCTIMEOUT_MAX;
    } else
    {
        _u2SIF_SYNC_TIMEOUT = u2SyncTimeout;
    }

    ASSERT(pbData);

    bDevAddr &= 0xFE;

    bBitDelay = (UINT8) u2Delay ; //bOption & 0x07; 
    bByteDelay = (UINT8) ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) >>6);
    _fgOptionWaitSlaveDevice = ((bOption & 0x100) >> 8);
    //fgNoStart = ((bOption&0x400)>>10);        //not used anymore
    //fgNoStop =((bOption&0x200)>>9);           //not used anymore
    
    bDevAddr |= 0x01;
#ifdef lousy
    Printf("SIFSW_ReadDataNoSubAddr: bDevAddr=0x%x, bDataCount=%d, pbData[0]=0x%x, bOption=0x%x, u2Delay=%d\n",bDevAddr,  bDataCount ,pbData[0] ,bOption,u2Delay);    //lousy
#endif

    if ( (!_Sifsw_SendStart(bDevAddr, bBitDelay)) && (fgNeedACKCheck) )
    {
        _Sifsw_SendStop(bBitDelay);     //No ack and exit--> release bus first            
        return (0);
    }
    _Sifsw_Delay1us(bByteDelay);

    while (bDataLeftCount)
    {
        if (bDataLeftCount == 1)
        {
            _Sifsw_GetByte(pbData++, SIF_FG_RANDREAD, bBitDelay);
        }
        else
        {
            _Sifsw_GetByte(pbData++, SIF_FG_SEQREAD, bBitDelay);
        }

        _Sifsw_Delay1us(bByteDelay);
        bDataLeftCount--;
    }

    _Sifsw_SendStop(bBitDelay);     //release bus

    return (UINT8) (bDataCount-bDataLeftCount);
}

/** SifEDIDReadByte
 * EDID I2C read command (one type only)
 * @param u1EDID_ID device address
 * @param u1SubAddr sub-address
 * @param pbData pointer of data to be read
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
UINT8 SifEDIDReadByte(UINT8 u1EDID_ID, UINT8 u1SubAddr, UINT8 * pbData)
{
    INT32 i4Ret=0;
    switch( u1EDID_ID )
    {
    	case 1:
    		SIFSW_Init(SIF_EDID1_SDA, SIF_EDID1_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
    		SIFSW_Remove();		
    		break;
    	case 2:
    		SIFSW_Init(SIF_EDID2_SDA, SIF_EDID2_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
    		SIFSW_Remove();		
    		break;
    	case 3:
    		SIFSW_Init(SIF_EDID3_SDA, SIF_EDID3_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
    		SIFSW_Remove();		
    		break;
	default:
		LOG(3,"[SifEDIDReadByte] parameter u1EDID_ID wrong!\n");
		i4Ret = 0	;
    		break;
    }
    return (UINT8) i4Ret;
}

/** SifEDIDWriteByte
 * EDID I2C write command (one type only)
 * @param u1EDID_ID device address
 * @param u1SubAddr sub-address
 * @param pbData pointer of data to be read
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
 /*
UINT8 SifEDIDWriteByte(UINT8 u1EDID_ID, UINT8 u1SubAddr, UINT8 * pbData)
{
    INT32 i4Ret=0;
    switch( u1EDID_ID )
    {
    	case 1:
    		SIFSW_Init(SIF_EDID1_SDA, SIF_EDID1_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
     		SIFSW_Remove();		
   		break;
    	case 2:
    		SIFSW_Init(SIF_EDID2_SDA, SIF_EDID2_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
     		SIFSW_Remove();		
   		break;
    	case 3:
    		SIFSW_Init(SIF_EDID3_SDA, SIF_EDID3_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
     		SIFSW_Remove();		
   		break;
	default:
		LOG(3,"[SifEDIDWriteByte] parameter u1EDID_ID wrong!\n");
		i4Ret = 0	;
    		break;
    }
    return i4Ret;
}*/

/** SifEEDIDReadBytes
 * EDID I2C read command (one type only)
 * @param u1EDID_ID device address
 * @param u1SubAddr sub-address
 * @param pbData pointer of data to be read
 * @return an interger >0 or 0
 * @retval > 0 read success
 * @retval = 0 read fail
 */
 /*
UINT8 SifEEDIDReadBytes(UINT8 u1EDID_ID, UINT8 u1Offset, UINT8 u1DataCount, UINT8 *pu1Data)
{
    INT32 i4Ret=0;
    switch( u1EDID_ID )
    {
        case 1:
            SIFSW_Init(SIF_EDID1_SDA, SIF_EDID1_SCL);
            i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1Offset,u1DataCount ,pu1Data, 0xFF  );
            SIFSW_Remove();		
    		break;
        case 2:
            SIFSW_Init(SIF_EDID2_SDA, SIF_EDID2_SCL);
            i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1Offset,u1DataCount ,pu1Data, 0xFF  );
            SIFSW_Remove();		
            break;
    	case 3:
    		SIFSW_Init(SIF_EDID3_SDA, SIF_EDID3_SCL);
            i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1Offset,u1DataCount ,pu1Data, 0xFF  );
            SIFSW_Remove();		
            break;
        default:
            LOG(3,"[SifEEDIDReadBytes] parameter u1EDID_ID wrong!\n");
            i4Ret = 0	;
            break;
    }
    return i4Ret;
}
*/
/** SifEEDIDWriteBytes
 * EDID I2C read command (one type only)
 * @param u1EDID_ID device address
 * @param u1SubAddr sub-address
 * @param pbData pointer of data to be read
 * @return an interger >0 or 0
 * @retval > 0 success
 * @retval = 0 fail
 */
 /*
UINT8 SifEEDIDWriteBytes(UINT8 u1EDID_ID, UINT8 u1Offset, UINT8 u1DataCount, UINT8 *pu1Data)
{
    INT32 i4Ret=0;
    INT32 i4EdidWP;
    INT32 i4EdidWPEnable;
    UINT32 u4Val;
    INT32 fgSet;

    Printf( "parameter: u1EDID_ID=%d,  u1Offset=%x, u1DataCount=%d\n", u1EDID_ID , u1Offset , u1DataCount );

    if (0 != DRVCUST_OptQuery(eEdidEepromWPGpio, &u4Val))
    {
        Printf("Not yet config EDID WP GPIO !!!\n");
        return 0;
    }    
    i4EdidWP = (INT32)u4Val;

    if (0 != DRVCUST_OptQuery(eEdidEepromWPEnablePolarity, &u4Val))
    {
        Printf("Not yet config EDID WP GPIO polarity !!!\n");
        return 0;
    }    

    i4EdidWPEnable = (INT32)u4Val;

    // disable write protect.
    VERIFY(BSP_PinGpioSel(i4EdidWP) == 0);  
    fgSet = !i4EdidWPEnable;
    VERIFY(GPIO_Output(i4EdidWP, &fgSet) == fgSet);

    Printf("Disable WP = GPIO%d.\n", i4EdidWP);

    switch( u1EDID_ID )
    {
        case 1:
            SIFSW_Init(SIF_EDID1_SDA, SIF_EDID1_SCL);
            i4Ret = SIFSW_WriteData( SIF_EEP_DEVICE_ADDR,u1Offset,u1DataCount ,pu1Data, 0xFF  );
            SIFSW_Remove();		
    		break;
        case 2:
            SIFSW_Init(SIF_EDID2_SDA, SIF_EDID2_SCL);
            i4Ret = SIFSW_WriteData( SIF_EEP_DEVICE_ADDR,u1Offset,u1DataCount ,pu1Data, 0xFF  );
            SIFSW_Remove();		
            break;
    	case 3:
    		SIFSW_Init(SIF_EDID3_SDA, SIF_EDID3_SCL);
            i4Ret = SIFSW_WriteData( SIF_EEP_DEVICE_ADDR,u1Offset,u1DataCount ,pu1Data, 0xFF  );
            SIFSW_Remove();		
            break;
        default:
            LOG(3,"[SifEEDIDReadBytes] parameter u1EDID_ID wrong!\n");
            i4Ret = 0	;
            break;
    }
    return i4Ret;
}
*/

UINT8 Sif_WriteEDIDData(UINT8 u1EDID_ID, UINT8 *pBuf, UINT8 size)
{
    INT32 i4Ret=0;
    switch( u1EDID_ID )
    {
        case 1:
            i4Ret = SifSW_WriteEeprom( SIF_EDID1_SDA ,  SIF_EDID1_SCL , pBuf , size);
    		break;
        case 2:
            i4Ret = SifSW_WriteEeprom( SIF_EDID2_SDA ,  SIF_EDID2_SCL , pBuf , size);
            break;
    	case 3:
            i4Ret = SifSW_WriteEeprom( SIF_EDID3_SDA ,  SIF_EDID3_SCL , pBuf , size);
            break;
        default:
            LOG(3,"[SifEEDIDReadBytes] parameter u1EDID_ID wrong!\n");
            i4Ret = 0	;
            break;
    }
    return (UINT8) i4Ret;
}

//-------------------------------------------------------------------------
/** SIFSW_ReadData
 * I2C read command (general type)
 *
 * @param bDevAddr device address
 * @param bSubAddr sub-address
 * @param bDataCount numbber of bytes to be read
 * @param pbData pointer of data to be read
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 8: halt and wait for ACK (full implementation of spec ch.7.2)
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
//-------------------------------------------------------------------------
UINT8 SIFSW_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                  UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;

    UINT8 au1SubAddr[4] = {0, 0, 0, 0};

    UINT32 i;

    ASSERT(pu1Buf);

    u1DevAddr &= 0xFE;

    bBitDelay = (UINT8) (u2ClkDiv / (27 * 3));
    bByteDelay = (UINT8) (u2ClkDiv / (27 * 3));
    fgNeedACKCheck = 1;
    _fgOptionWaitSlaveDevice = 1;

    if (u1WordAddrNum == 1)
    {
        au1SubAddr[0] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else if (u1WordAddrNum == 2)
    {
        au1SubAddr[0] = (UINT8) ((u4WordAddr & 0x0000FF00) >> 8);
        au1SubAddr[1] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else if(u1WordAddrNum == 3)
    {
        au1SubAddr[0] = (UINT8) ((u4WordAddr & 0x00FF0000) >> 16);
        au1SubAddr[1] = (UINT8) ((u4WordAddr & 0x0000FF00) >> 8);
        au1SubAddr[2] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else if (u1WordAddrNum > 3)
    {
        return (FALSE);
    }
    
    // If there is no sub addr, we read directly
    if(!u1WordAddrNum)
    {
        u1DevAddr |= 0x01;
    }

    // Send start cond and address   
    _Sifsw_SendStart(u1DevAddr, bBitDelay);

    _Sifsw_Delay1us(bByteDelay);

    // If there is sub addr, we need to write sub addr first.
    if(u1WordAddrNum)
    {
        // Write sub address
        for(i = 0; i < u1WordAddrNum; i++)
        {
            if((!_Sifsw_PutByte(au1SubAddr[i], bBitDelay))&& (fgNeedACKCheck)) 
            {
                return (FALSE);
            }
        }
    
        _Sifsw_Delay1us(bByteDelay);
        u1DevAddr |= 0x01;
    
        // Send start cond and address for read
        if ((!_Sifsw_SendStart(u1DevAddr, bBitDelay))&&(fgNeedACKCheck))
        {
            return (FALSE);
        }
    
        _Sifsw_Delay1us(bByteDelay);

    }
    
    // Read data
    while (u2ByteCnt)
    {
        if (u2ByteCnt == 1)
        {
            _Sifsw_GetByte(pu1Buf++, SIF_FG_RANDREAD, bBitDelay);
        }
        else
        {
            _Sifsw_GetByte(pu1Buf++, SIF_FG_SEQREAD, bBitDelay);
        }

        _Sifsw_Delay1us(bByteDelay);
        u2ByteCnt--;
    }

    // Send stop
    _Sifsw_SendStop(bBitDelay);

    return (TRUE);
}

//-------------------------------------------------------------------------
/** SIFSW_ReadData
 * I2C read command (general type)
 *
 * @param bDevAddr device address
 * @param bSubAddr sub-address
 * @param bDataCount numbber of bytes to be read
 * @param pbData pointer of data to be read
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 8: halt and wait for ACK (full implementation of spec ch.7.2)
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
//-------------------------------------------------------------------------
UINT8 SIFSW_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                  UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;

    UINT8 au1SubAddr[4] = {0, 0, 0, 0};

    UINT32 i;

    ASSERT(pu1Buf);

    u1DevAddr &= 0xFE;

    bBitDelay = (UINT8) (u2ClkDiv / (27 * 3));
    bByteDelay = (UINT8) (u2ClkDiv / (27 * 3));
    fgNeedACKCheck = 1;
    _fgOptionWaitSlaveDevice = 1;

    if (u1WordAddrNum == 1)
    {
        au1SubAddr[0] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else if (u1WordAddrNum == 2)
    {
        au1SubAddr[0] = (UINT8) ((u4WordAddr & 0x0000FF00) >> 8);
        au1SubAddr[1] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else if (u1WordAddrNum == 3)
    {
        au1SubAddr[0] = (UINT8) ((u4WordAddr & 0x00FF0000) >> 16);
        au1SubAddr[1] = (UINT8) ((u4WordAddr & 0x0000FF00) >> 8);
        au1SubAddr[2] = (UINT8) ((u4WordAddr & 0x000000FF));
    }
    else if (u1WordAddrNum > 3)
    {
        return (FALSE);
    }
    
    // Send start cond and addr
    _Sifsw_SendStart(u1DevAddr, bBitDelay);
    
    _Sifsw_Delay1us(bByteDelay);

    // Write sub address
    for(i = 0; i < u1WordAddrNum; i++)
    {
        if((!_Sifsw_PutByte(au1SubAddr[i], bBitDelay))&& (fgNeedACKCheck)) 
        {
            return (FALSE);
        }
    }

    _Sifsw_Delay1us(bByteDelay);

    // Write data
    while (u2ByteCnt)
    {
        //if ((fgNeedACKCheck) && (!_Sifsw_PutByte(*(pbData++), bBitDelay))) // Data Content Write
        if ((!_Sifsw_PutByte(*(pu1Buf++), bBitDelay))&& (fgNeedACKCheck)) // Data Content Write
        {
            return (FALSE);                                                // Device Address exceeds the range
        }

        _Sifsw_Delay1us(bByteDelay);
        u2ByteCnt--;
    }

    // Send stop
    _Sifsw_SendStop(bBitDelay);

    return (TRUE);
}


