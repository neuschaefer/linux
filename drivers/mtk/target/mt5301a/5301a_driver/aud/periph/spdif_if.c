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
 * $RCSfile: spdif_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file spdif_if.c
 *  SPDIF interface setting, including SPDIF in and SPDIF out.
 *  
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  x        AK4114 Digital Audio Interface Transceiver                  x
  x--------------------------------------------------------------------x
  x        Device Addr : 0x10                                          x
  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN  
  
#include "x_typedef.h"
#include "x_assert.h"
#include "x_printf.h"
#include "aud_debug.h"

#include "sif_if.h"  // for i2c
#include "spdif_drvif.h"
#include "aud_cfg.h"
#include "adac_if.h"
#include "x_pinmux.h"

LINT_EXT_HEADER_END

/****************************************************************************
** Local definitions
****************************************************************************/
//#define USE_AK4114
#define USE_CS8415

#define CS8415A_DEV_ADDR    0x17

/****************************************************************************
** Local structures and enumerations
****************************************************************************/

/****************************************************************************
** Local function prototypes
****************************************************************************/

/****************************************************************************
** Local variable
****************************************************************************/

// Register map of CS8415A
#define REG_CTRL_1     0x01
#define REG_CTRL_2     0x02
    #define CH_SEL_MASK   (0x7 << 0)
#define REG_SRC_CTRL   0x04
#define REG_OUT_FMT    0x06
    #define MASTER_MODE_MASK   (0x1 << 7)
    #define MASTER_MODE        (0x1 << 7)
    #define SLAVE_MODE         (0x0 << 7)
    #define DATA_BITS_NUM_MASK (0x3 << 4)
    #define DATA_BITS_24       (0x0 << 4)
    #define DATA_BITS_20       (0x1 << 4)
    #define DATA_BITS_16       (0x2 << 4)
#define REG_RCV_STATUS 0x10
    #define SPDIF_UNLOCK  (0x1 << 4)
    #define SPDIF_LOCK    (0x0 << 4)
#define REG_ID_VER     0x7f

static UINT8 _u1RegCtl1;
static UINT8 _u1RegCtl2;
static UINT8 _u1RegSrcCtrl;
static UINT8 _u1RegOutFmt;
static UINT8 _u1RegStatus;
static UINT8 _u1RegIdVer;

// keep the value of above registers for configuration
UINT8 _u1PeriphCfg00; // configuration on AKM4114 00h
UINT8 _u1PeriphCfg01; // configuration on AKM4114 01h
UINT8 _u1PeriphCfg03; // configuration on AKM4114 03h
UINT8 _u1PeriphCfg04; // configuration on AKM4114 04h
UINT8 _u1PeriphCfg06; // configuraiton on AKM4114 06h

UINT8 _u1AinFlag;
SPDIF_IN_FLAG_T _rSpdifInFlag;

/****************************************************************************
** External functions prototypes
****************************************************************************/

/****************************************************************************
** Local functions
****************************************************************************/

static BOOL _ReadPeriphReg(UINT8 u1Addr, UINT8 * pu1Data)
{
    UINT32 u4ReadCount = 0;

#ifdef USE_AK4114      
    u4ReadCount = SIF_Read(SIF_CLK_DIV, AK4114_DEV_ADDR<<1, u1Addr, pu1Data, 1);
#else
    u4ReadCount = SIF_Read(SIF_CLK_DIV, CS8415A_DEV_ADDR<<1, u1Addr, pu1Data, 1);
#endif

    if (pu1Data != NULL)
    {
       LOG( 9,"I2C read (address,data) = (0x%x, 0x%x)\n",u1Addr, *pu1Data);
    }   

    if (u4ReadCount > 0)
    {
    	return TRUE;
    }	
    else
    {
    	return FALSE;
    }	
}

static BOOL _WritePeriphReg(UINT8 u1Addr, UINT8 u1Data)
{
    UINT32 u4WriteCount = 0; 

#ifdef USE_AK4114   
    u4WriteCount = SIF_Write(SIF_CLK_DIV, AK4114_DEV_ADDR<<1, u1Addr, &u1Data, 1);
#else
    u4WriteCount = SIF_Write(SIF_CLK_DIV, CS8415A_DEV_ADDR<<1, u1Addr, &u1Data, 1);
#endif

    LOG( 9,"I2C write (address,data) = (0x %x,0x %x)\n",u1Addr, u1Data);

    if (u4WriteCount > 0)
    {
    	return TRUE;
    }	
    else
    {
    	return FALSE;
    }	
}

/****************************************************************************
** Global functions
****************************************************************************/

/************************************************************************
     Function : void _SpdifDataFormat(UINT8 u1DataFormat)
  Description : Setting ADAC Data Format 
    Parameter :           
    Return    : NONE
************************************************************************/
static void _SpdifDataFormat(UINT8 u1DataFormat) 
{
#ifdef USE_AK4114

  UINT8 u1Target=_u1PeriphCfg01 & 0x8F;
  switch (u1DataFormat)
  {
    case RTJ_16BIT:
      u1Target = u1Target | 0;
      break;
    case RTJ_18BIT:
      u1Target = u1Target | 0x10;
      break;
    case RTJ_20BIT:
      u1Target = u1Target | 0x20;
      break;
    case I2S_24BIT:
      u1Target = u1Target | 0x50;
      break;
    case LTJ_24BIT:
      u1Target = u1Target | 0x40;
      break;
    case RTJ_24BIT: // default 24 bit Right Justified
    default:
      u1Target = u1Target | 0x30;
      break;
  }
  if(_u1PeriphCfg01 != u1Target) // if configuration keeps
  {

    if(!_WritePeriphReg(0x01,u1Target))
    {

      if(!_WritePeriphReg(0x01,u1Target))
      {

        return; // if error occurs twice we don't update the value
      }// write one more time    
    }
    _u1PeriphCfg01 = u1Target;
  }
#else

  UNUSED(u1DataFormat);

#endif  
}
/************************************************************************
     Function : void _SpdifChgMCLK(UINT8 bSampleRate)
  Description : Change master clk output with sampling rate 
    Parameter : bSampleRate : 0-> 48k, 1-> 96k, 2-> 192k          
    Return    : NONE
************************************************************************/
static void _SpdifChgMCLK(UINT8 u1SampleRate) 
{
#ifdef USE_AK4114
  UINT8 u1Target=_u1PeriphCfg00 & 0xF3;
  switch (u1SampleRate)
  {
    case AFS512_48K: //Max fs :48kHz
      u1Target = u1Target | 0x08;
      break;
    case AFS128_192K: //Max fs :192kHz
      u1Target = u1Target | 0x0C;
      break;
    case AFS256_96K: //Max fs :96kHz
    default:    
      u1Target = u1Target | 0x00;
      break;
  }
  if (u1Target != _u1PeriphCfg00) // if configuration keeps
  {
    if (!_WritePeriphReg(0x00, u1Target))
    {
      if (!_WritePeriphReg(0x00, u1Target))
      {
        return;
      }
    }
    _u1PeriphCfg00 = u1Target;
  }  
 #else
  UNUSED(u1SampleRate);
 #endif
}

/************************************************************************
     Function : UINT8 _SpdifDetSmpRate(void)
  Description : Sample Rate Detection
    Parameter : NONE            
    Return    : 0:44.1k 1:48k 2:32k 3:88.2k 4:96k 5:176.4k 6:192k
************************************************************************/
/*
static UINT8 _SpdifDetSmpRate(void) 
{
  UINT8 bData;
  UINT8 bRetVal;
  if(!_ReadPeriphReg(0x07,&bData))
  {
    if(!_ReadPeriphReg(0x07,&bData))
    {
      return (DSP_AIN_FREQ44_1K);
    }
  }
  bData = (bData & 0xf0) >> 4;
  switch (bData)
  {
    case 2: //48kHz
      bRetVal = DSP_AIN_FREQ48K;
      break;

    case 3: //32kHz
      bRetVal = DSP_AIN_FREQ32K;
      break;

    case 8: //88.2kHz
      bRetVal = DSP_AIN_FREQ88_2K;
      break;

    case 10: //96kHz
      bRetVal = DSP_AIN_FREQ96K;
      break;

    case 12: //176.4kHz
      bRetVal = DSP_AIN_FREQ176_4K;
      break;

    case 14: //192kHz
      bRetVal = DSP_AIN_FREQ192K;
      break;

    case 0: //44.1kHz
    case 1: //>192kHz, <32kHz, invalid
    default:
      bRetVal = DSP_AIN_FREQ44_1K;
      break;
  }

  return (bRetVal);
}
*/
/************************************************************************
     Function : void _SpdifDeEmphasisEN(BOOL fgEnable)   
  Description : Enisable DeEmphasis
    Parameter : NONE            
    Return    : NONE
************************************************************************/
/*
static void _SpdifDeEmphasisEN(BOOL fgEnable)   
{
  UINT8 u1Target=_u1PeriphCfg01;
  
  if(fgEnable)
  {
    u1Target = (u1Target & 0xF7) | 0x08;   
  }
  else
  {
    u1Target = (u1Target & 0xF0) | 0x02;
  }
  if(u1Target != _u1PeriphCfg01)
  {

    if(!_WritePeriphReg(0x01,u1Target))
    {

      if(!_WritePeriphReg(0x01,u1Target))
      {
        return;
      }
    }
    _u1PeriphCfg01 = u1Target;
  }
}
*/
/************************************************************************
     Function : void _SpdifPWN(BOOL fgEnable)
  Description : SPDIF Transciver Power Down
    Parameter : bEnable: 1-> Power down            
    Return    : NONE
************************************************************************/
static void _SpdifPWN(BOOL fgEnable) 
{
#ifdef USE_AK4114
  UINT8 u1Target = _u1PeriphCfg00;
 
  u1Target = u1Target & 0xFD;
  if(fgEnable)  //PowerOn
  {
    u1Target = u1Target |0x02;
  }
  if(_u1PeriphCfg00 != u1Target)
  {

    if(!_WritePeriphReg(0x00, u1Target))
    {

      if(!_WritePeriphReg(0x00,u1Target))
      {
        return;
      }
    }
    _u1PeriphCfg00 = u1Target;
  }
#else
  UNUSED(fgEnable);
#endif
}


/************************************************************************
     Function : void _SpdifChInSel(UINT8 bCh)
  Description : Iutput Channel Selection
    Parameter : u1Ch : 0->RX0, 1->RX1, 2->RX2, 3->RX3                
    Return    : NONE
************************************************************************/
static void _SpdifChInSel(UINT8 u1Ch) 
{
#ifdef USE_AK4114
    UINT8 u1Target = _u1PeriphCfg03 & 0xf8;
    u1Target = (u1Target & 0xF8) | u1Ch;
    if (u1Target != _u1PeriphCfg03)
    {
      if (!_WritePeriphReg(0x03,u1Target))
      {
        if (!_WritePeriphReg(0x03,u1Target))
        {
          return;     
        }
      }
      _u1PeriphCfg03=u1Target;
    }
#else
    UINT8 u1RegCtl2 = 0;
    u1RegCtl2 = (_u1RegCtl2 & (~CH_SEL_MASK)) | u1Ch;
    if (u1RegCtl2 != _u1RegCtl2)
    {
        if (!_WritePeriphReg(REG_CTRL_2, u1RegCtl2))
        {
            LOG(5, "_SpdifChInSel fail\n");
            return ;
        }
    }
    _u1RegCtl2 = u1RegCtl2;
#endif

}

/************************************************************************
     Function : void _SpdifClkMode(UINT8 bClkMode)
  Description : Clock Control Mode Selection(W/ data source selection)
    Parameter : bClkMode : 0->PLL(RX), 1->X'tal(DAUX), 
                           2->PLL(RX)/X'tal(DAUX) auto switch           
    Return    : NONE
************************************************************************/
static void _SpdifClkMode(UINT8 u1ClkMode) 
{
#ifdef USE_AK4114

  UINT8 u1Target = _u1PeriphCfg00 & 0xCF;
  switch (u1ClkMode)
  {
    case 0: //Use PLL for clk source
      break;
    case 1: //Use X'tal for clk source
      u1Target = u1Target | 0x10;
      break;
    case 2: //Use PLL(X'tal when PLL unlock) for clk source
      u1Target = u1Target | 0x20;
    default:
      break;
  }
  if (_u1PeriphCfg00 != u1Target)
  {
    if (!_WritePeriphReg(0x00, u1Target))
    {

      if (!_WritePeriphReg(0x00, u1Target))
      {
        return;
      }
    }
    _u1PeriphCfg00=u1Target;
  }
#else
 
  UNUSED(u1ClkMode);
#endif
}

/************************************************************************
     Function : void _SpdifINT0(UINT8 bUlockEn , UINT8 bPAREn)
  Description : INT0 Enable
    Parameter : bUlockEn : 1->Unlock Enable, bPAREn : 1->PAR Enable                
    Return    : NONE
************************************************************************/
static void _SpdifINT0(UINT8 bUlockEn , UINT8 bPAREn)
{
#ifdef USE_AK4114
  UINT8 u1Target=_u1PeriphCfg04;
  if(bUlockEn)
  {
    u1Target = u1Target & 0xEF;
  }
  else
  {
    u1Target = u1Target | 0x10;
  }
  if(bPAREn)
  {
    u1Target = u1Target & 0xFE;
  }
  else
  {
    u1Target = u1Target | 0x01;
  }
  if(u1Target != _u1PeriphCfg04)
  {

    if(!_WritePeriphReg(0x04,u1Target))
    {


      if(!_WritePeriphReg(0x04, u1Target))
      {
        return;
      }
    }
    _u1PeriphCfg04=u1Target;
  }
#else
  UNUSED(bUlockEn);
  UNUSED(bPAREn);
#endif
}

static BOOL _SpidfHwInit()
{
  BOOL fgRet = TRUE;

#ifdef USE_AK4114

  if(!_WritePeriphReg(0x00,0x00))
  {
    UNUSED(_WritePeriphReg(0x00,0x00)); // write one more time
  }; // Reset and pwn all registers
  if(!_WritePeriphReg(0x00,0x03))
  {
    UNUSED(_WritePeriphReg(0x00,0x03)); // write one more time
  }; // Normal operation
  if(!_ReadPeriphReg(0x00,&_u1PeriphCfg00))
  {
    UNUSED(_ReadPeriphReg(0x00,&_u1PeriphCfg00)); // read one more time
  }; // get initial value on 00h
  if(!_ReadPeriphReg(0x01,&_u1PeriphCfg01))
  {
    UNUSED(_ReadPeriphReg(0x01,&_u1PeriphCfg01)); // read one more time
  }; // get initial value on 01h
  if(!_ReadPeriphReg(0x03,&_u1PeriphCfg03))
  {
    UNUSED(_ReadPeriphReg(0x03,&_u1PeriphCfg03)); // read one more time
  }; // get initial value on 03h
  if(!_ReadPeriphReg(0x04,&_u1PeriphCfg04))
  {
    UNUSED(_ReadPeriphReg(0x04,&_u1PeriphCfg04)); // read one more time
  }; // get initial value on 04h  

  //FIXME
  {
	// 3a: 24-bit right justified    
    _WritePeriphReg(0x01,0x3a);

  }
#else
 
   // Reset 8415 ?? 
   //_WritePeriphReg();

   if (!_ReadPeriphReg(REG_CTRL_1, &_u1RegCtl1)) { fgRet = FALSE; };
   if (!_ReadPeriphReg(REG_CTRL_2, &_u1RegCtl2)) { fgRet = FALSE; };
   if (!_ReadPeriphReg(REG_SRC_CTRL, &_u1RegSrcCtrl)) { fgRet = FALSE; };
   if (!_ReadPeriphReg(REG_OUT_FMT, &_u1RegOutFmt)) { fgRet = FALSE; };
   if (!_ReadPeriphReg(REG_RCV_STATUS, &_u1RegStatus)) { fgRet = FALSE; };
   if (!_ReadPeriphReg(REG_ID_VER, &_u1RegIdVer)) { fgRet = FALSE; };

#endif

   return fgRet;
}

static BOOL _SpidfIsLock()
{
#ifdef USE_AK4114

  if (!_ReadPeriphReg(0x06,&_u1PeriphCfg06))
  {
    // device busy or no ack
    if (!_ReadPeriphReg(0x06,&_u1PeriphCfg06))
    {
      return (FALSE);
    }
  }
  if (_u1PeriphCfg06 & 0x11)
  {
    // unlocked
    return (FALSE);
  }
  else
  {
    // locked    
    return (TRUE);
  }
#else

  UNUSED(_ReadPeriphReg(REG_RCV_STATUS, &_u1RegStatus));

  if (_u1RegStatus & SPDIF_UNLOCK)
  {
      return FALSE;
  }
  else
  {
      return TRUE;
  }

#endif  
}

//-----------------------------------------------------------------------------
// SPDIF_InRegisterDump
//
/** Rigister data dump.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void SPDIF_InRegisterDump(void) 
{
#ifdef USE_AK4114

	int i;
	UINT8 u1Data;
	for(i=0;i<0x20;i++)
	{
		UNUSED(_ReadPeriphReg(i, &u1Data));
		LOG( 3,"(%2x,%2x)",i,u1Data);
		if(!((i+1)%8)) LOG( 3,"\n");
	}
#else

#endif
}

//-----------------------------------------------------------------------------
// SPDIF_InInit
//
/** SPDIF Transciver Initial Routine.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void SPDIF_InInit(void) 
{

// Hardcode here !! Should remove later !!
#ifdef CC_AUD_CUST_SPECIFIC_GPIO_CTL
	INT32 i4GpioValue = 0;

    GPIO_Output(206, (const INT32 *)&i4GpioValue);    
    x_thread_delay(100);
    i4GpioValue = 1;
    GPIO_Output(206, (const INT32 *)&i4GpioValue);    
#endif

#ifdef CC_AUD_USE_SPDIF
if (BSP_GetIcVersion() >= IC_VER_5382P_AA)
{
  //Use GPIO 13,2,3,4
  PINMUX1_WRITE((PINMUX1_REG() & (~(0x7 << 12))) | (0x1 <<12));
  PINMUX1_WRITE((PINMUX1_REG() & (~(0x7 << 20))) | (0x6 <<20));
  GPIO_EN_WRITE(0,(GPIO_EN_REG(0) &(~(0x7 << 2))) |(0x000 << 2));
  GPIO_EN_WRITE(0,(GPIO_EN_REG(0) &(~(0x1 << 13))) |(0x0 << 13));
  PINMUX_MISC_WRITE((PINMUX_MISC_REG() &(~(0x1 << 10))) |(0x1 << 10));
}
else

{
  //Use GPIO 1,2,3,4
  PINMUX1_WRITE((PINMUX1_REG() & (~(0x7 << 12))) | (0x1 <<12));
  GPIO_EN_WRITE(0,(GPIO_EN_REG(0) &(~(0xf << 1))) |(0x0 << 1));
}
#endif


  if (!_SpidfHwInit()) 
  {
     LOG(5, "SPDIF_InInit fail\n");    
  }

  _u1AinFlag = DSP_AIN_DISABLE;
  // select SPDIF in
  _rSpdifInFlag.fgSpdifChk = TRUE;
  _rSpdifInFlag.fgSpdifLock = FALSE;
   
  _SpdifDataFormat(RTJ_24BIT); // set data format  
  _SpdifChInSel(AIN_SEL_D0 & 0x0f);
  _SpdifChgMCLK(AFS256_96K);  // 256Fs
  _SpdifINT0(TRUE, FALSE);
  _SpdifClkMode(2);

#ifdef USE_AK4114

#else


	// CS8415 setting
  UNUSED(_WritePeriphReg(2, 0));  // 256FS, RXP0
  UNUSED(_WritePeriphReg(4, 0x40)); // Trigger RUN !!
  UNUSED(_WritePeriphReg(6, 0x88));  //  Right-Justified
#if 0
//#else
	// CS8416 setting
  UNUSED(_WritePeriphReg(4, 0x80)); // Trigger RUN !!
  UNUSED(_WritePeriphReg(5, 0x84));  //  I2S

#endif

  #endif

}

//-----------------------------------------------------------------------------
// SPDIF_InLockCheck
//
/** SPDIF lock check.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL SPDIF_InLockCheck(void) 
{
    return _SpidfIsLock(); 
}

//-----------------------------------------------------------------------------
// SPDIF_InEnable
//
/** SPDIF in enable.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for power down.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void SPDIF_InEnable(BOOL fgEnable)
{
  // TODO
  if (fgEnable)
  {
      SPDIF_InInit(); 
  }
  else
  {
      _SpdifPWN(FALSE);
  }
}

//-----------------------------------------------------------------------------
// SPDIF_InChSel
//
/** SPDIF in channel select.
 *
 *  @param[in]     u1Ch   "0":RX0,"1":RX1,"2":RX2,"3":RX3.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------

void SPDIF_InChSel(UINT8 u1Ch) 
{
    _SpdifChInSel(u1Ch);
}

