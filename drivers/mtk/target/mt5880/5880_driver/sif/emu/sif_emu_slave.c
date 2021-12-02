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
 * $RCSfile: sif_hw_v1.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_hw_v1.c
 *  This file implement serial interface SIF function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "sif_debug.h"
#include "sif_hw.h"
#include "sif_if.h"
#include "drvcust_if.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_gpio.h"
LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
/// Emulator type
#define ENABLE_SIF_SLAVE_EMULATOR 1

/// Only for emulator validation
// #define SIMULATOR_SW_VALIDATION

/// Device address of the slave devive emulator
#define DEVICE_ADDRESS 0x14
#define SDA_GPIO GPIO(43) // U2RX
#define SCL_GPIO GPIO(44) // U2Tx

/// monitor pin for slave emulator's state machine
#define MONITOR_GPIO GPIO(63) // TUNER_CLK


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#ifndef SIMULATOR_SW_VALIDATION
#define SAMPLING_DELAY 
// HAL_Delay_us(1);
#define GET_SCL(x) (x=GPIO_GetIn(SCL_GPIO))
#define GET_SDA(x) (x=GPIO_GetIn(SDA_GPIO))
#define SET_SCL(x) GPIO_SetOut(SCL_GPIO, x)
#define SET_SDA(x) GPIO_SetOut(SDA_GPIO, x)
#else
#define SAMPLING_DELAY x_thread_delay(10);
#define GET_SCL(x) (x=_i4SimScl)
#define GET_SDA(x) (x=_i4SimSda)
#define SET_SCL(x) (_i4SimScl=x)
#define SET_SDA(x) (_i4SimSda=x)
#endif
#define SET_MONITOR(x) GPIO_SetOut(MONITOR_GPIO, x)


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/// State definition
#define STATE_IDLE 0
#define STATE_ADDRESS 1
#define STATE_ADDRESS_ON_CLK_FALLING 2
#define STATE_ACK_ADDRESS 3
#define STATE_ACK_ADDRESS_2 4  
#define STATE_WRITE_DATA 5
#define STATE_WRITE_DATA_ON_CLK_FALLING 6
#define STATE_READ_DATA 7
#define STATE_READ_DATA_ON_CLK_FALLING 8
#define STATE_WAIT_READ_DONE 9
#define STATE_ACK_DATA 10
#define STATE_ACK_DATA_2 11
#define STATE_WAIT_ACK 12
#define STATE_WAIT_ACK_ON_CLK_FALLING 13
#define STATE_WAIT_STOP 14


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef struct _SIF_SLAVE_DEV_STATE_T
{
    UINT32 u4State;
    UINT32 u4Address;
    BOOL fgRead;    
    UINT32 au4Data[256];
    UINT32 u4Idx;
    UINT32 u4Shift;
} SIF_SLAVE_DEV_STATE_T;


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#ifdef SIMULATOR_SW_VALIDATION
static INT32 _i4SimScl = 1;
static INT32 _i4SimSda = 1;
#endif

#ifndef CC_5391_LOADER
static BOOL _fgInit = FALSE;
#endif

static INT32 _i4SDALevel = 1;  
static INT32 _i4PrevSDALevel = 1;  
static INT32 _i4SCLLevel = 1;  
static INT32 _i4PrevSCLLevel = 1;  

static BOOL _fgSCLStretchOnAddr = FALSE;
static BOOL _fgSCLStretchOnData = FALSE;
static UINT32 _u4SCLStretchTime = 1000;
static UINT32 _u4SCLPrevStretchTime = 270;

static HANDLE_T _hStateMachineHandle = NULL;

static SIF_SLAVE_DEV_STATE_T _rSifSlave =
{
    STATE_IDLE,
    0x0,
    FALSE,
    {0, 0, 0, 0, 0, 0, 0, 0},
    0,
    0
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static INT32 _SampleData(INT32 i4SDALevel, INT32 i4PrevSDALevel, INT32 i4SCLLevel, INT32 i4PrevSCLLevel)
{
    INT32 i4Ret = -1;
    if(i4SCLLevel == 1 && i4PrevSCLLevel == 0)
    {
        // de-bouncing
        // HAL_Delay_us(1);
        // GET_SCL(_i4SCLLevel);
        //if(_i4SCLLevel == 1)
        {
            // GET_SDA(i4Ret);
            i4Ret = i4SDALevel;            
        }
    }
    return i4Ret;
}

static INT32 _WaitClkFalling(INT32 i4SDALevel, INT32 i4PrevSDALevel, INT32 i4SCLLevel, INT32 i4PrevSCLLevel)
{
    INT32 i4Ret = -1;
    if(i4SCLLevel == 0 && i4PrevSCLLevel == 1)
    {
        // de-bouncing
        // HAL_Delay_us(1);
        // GET_SCL(_i4SCLLevel);    
        //if(_i4SCLLevel == 0)
        {        
            // GET_SDA(i4Ret);
            i4Ret = i4SDALevel;
        }
    }
    return i4Ret;
}

static INT32 _CheckStopCond(INT32 i4SDALevel, INT32 i4PrevSDALevel, INT32 i4SCLLevel, INT32 i4PrevSCLLevel)
{
    INT32 i4Ret = 0;
    if(i4SCLLevel == 1 && i4PrevSCLLevel == 1)
    {
        if(i4SDALevel == 1 && i4PrevSDALevel == 0)
        {
            i4Ret = 1;
        }
    }
    return i4Ret;
}

static INT32 _CheckStartCond(INT32 i4SDALevel, INT32 i4PrevSDALevel, INT32 i4SCLLevel, INT32 i4PrevSCLLevel)
{
    INT32 i4Ret = -1;
    if(i4SCLLevel == 1 && i4PrevSCLLevel == 1)
    {
        if(i4SDALevel == 0 && i4PrevSDALevel == 1)
        {
            i4Ret = 1;
        }
    }
    return i4Ret;
}

static void _SifSlaveStateMachine(void* pvArg)
{    
    INT32 i4Data = 0;
    INT32 i4DataOnClkFalling = 0;


    Printf("SIF emulation tool: slave emulator.\n");

#ifndef CC_5391_LOADER
    _fgInit = TRUE;
#endif
    GPIO_SetOut(62, 1);
    
    while(1)
    {
        
        switch(_rSifSlave.u4State)
        {
            case STATE_IDLE:
                GET_SCL(_i4SCLLevel);            	
                GET_SDA(_i4SDALevel);                
                // Check start cond
                if(_CheckStartCond(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel) == 1)
                {
                    _rSifSlave.u4Shift = 7;
                    _rSifSlave.u4State = STATE_ADDRESS;
                }
                break;
            case STATE_ADDRESS:
                GET_SCL(_i4SCLLevel);            	
                GET_SDA(_i4SDALevel);            	
                // Sampled on rising edge     
                i4Data = _SampleData(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel);
                if(i4Data >= 0)
                {
                
                    GPIO_SetOut(62, 0);

                    // Already last bit, ack here
                    if(_rSifSlave.u4Shift == 0)
                    {
                        SET_SDA(0);      
                    }
                
                    _rSifSlave.u4State = STATE_ADDRESS_ON_CLK_FALLING;
                }
                break;        
            case STATE_ADDRESS_ON_CLK_FALLING:
                GET_SCL(_i4SCLLevel);       
                if(_rSifSlave.u4Shift > 0)
                {
                    GET_SDA(_i4SDALevel);                 	
                }
                // Sampled on falling edge            	
                i4DataOnClkFalling = _WaitClkFalling(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel);            	                
                if(/*(i4Data == i4DataOnClkFalling) &&*/ i4DataOnClkFalling >= 0)
                {          

                    GPIO_SetOut(62, 1);

                    // Bit read
                    _rSifSlave.u4Address |= (((UINT32)i4Data & 1u) << _rSifSlave.u4Shift);
                    if(_rSifSlave.u4Shift-- == 0)
                    {                   
                        _rSifSlave.fgRead = (BOOL)(_rSifSlave.u4Address & 0x1);                          
                        // Read for processing next byte
                        _rSifSlave.u4Shift = 7;                    
                        if((_rSifSlave.u4Address & 0xFE) == DEVICE_ADDRESS)
                        {                            
                            _rSifSlave.u4State = STATE_ACK_ADDRESS;
                            SET_SDA(0);                            
                        }
                        else
                        {
                            _rSifSlave.u4State = STATE_IDLE;
                        }
                    }
                    else
                    {
                        _rSifSlave.u4State = STATE_ADDRESS;
                    }
                }             
                break;
            case STATE_ACK_ADDRESS:
                GET_SCL(_i4SCLLevel);               	
                // SET_SDA(0);    
                
                GPIO_SetOut(62, 0);
                
            	  // Ack until clock falling
                if(_i4SCLLevel == 1 && _i4PrevSCLLevel == 0)
                {
                    // HAL_Delay_us(1);
                    GET_SCL(_i4SCLLevel);               	
                    if(_i4SCLLevel == 1)
                    {
                        _rSifSlave.u4State = STATE_ACK_ADDRESS_2;
                    }
                    _i4PrevSCLLevel = _i4SCLLevel;
                }
                break;        	
            case STATE_ACK_ADDRESS_2:      
                GET_SCL(_i4SCLLevel);               	
                // SET_SDA(0);                  	
                if(_i4SCLLevel == 0 && _i4PrevSCLLevel == 1)
                {
                    // de-bouncing
                    // HAL_Delay_us(1);
                    GET_SCL(_i4SCLLevel);
                    if(_i4SCLLevel == 0)
                    {
                        if(_rSifSlave.fgRead)
                        {
                            // Send 0th bit here
                            SET_SDA((_rSifSlave.au4Data[_rSifSlave.u4Idx] >> _rSifSlave.u4Shift) & 0x1 );
                            _rSifSlave.u4Shift--;                    
                            _rSifSlave.u4State = STATE_READ_DATA;                                        
                        }
                        else
                        {
                            _rSifSlave.u4State = STATE_WRITE_DATA;                    
                            GET_SDA(_i4SDALevel);

                            GPIO_SetOut(62, 1);

                            if(_fgSCLStretchOnAddr)
                            {
                                // Stretch SCL
                                if(_u4SCLPrevStretchTime)
                                {
                                    HAL_Delay_us(_u4SCLPrevStretchTime);
                                }                                
                                SET_SCL(0);

                                HAL_Delay_us(_u4SCLStretchTime);

                                GPIO_SetOut(62, 0);

                                //GET_SCL(_i4SCLLevel);      

                                // Release SCL
                                GPIO_SetOut(62, 1);

                                // Master might feed clock soon, so here we ignore SCL rising edge
                                do
                                {
                                    GET_SCL(_i4SCLLevel);
                                } while(_i4SCLLevel == 0);
                                GET_SDA(_i4SDALevel);
                                i4Data = _i4SDALevel;
                                GPIO_SetOut(62, 0);                                
                                _rSifSlave.u4State = STATE_WRITE_DATA_ON_CLK_FALLING;                
                            }                                                                
                        }                    
                    }                    
                }
                break;
            case STATE_WRITE_DATA:
                GET_SCL(_i4SCLLevel);            	
                GET_SDA(_i4SDALevel);                 	
                // Sampled on rising edge                
                i4Data = _SampleData(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel);
                if(i4Data >= 0)
                {                

                    GPIO_SetOut(62, 0);
                
                    _rSifSlave.u4State = STATE_WRITE_DATA_ON_CLK_FALLING;                
                }
                break;                   
            case STATE_WRITE_DATA_ON_CLK_FALLING:
                GET_SCL(_i4SCLLevel);            	
                GET_SDA(_i4SDALevel);              	
                // Sampled on falling edge
                i4DataOnClkFalling = _WaitClkFalling(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel);
                if(/*(i4Data == i4DataOnClkFalling) &&*/ i4DataOnClkFalling >= 0)
                {

                    GPIO_SetOut(62, 1);
                
                    _rSifSlave.au4Data[_rSifSlave.u4Idx] |= (((UINT32)i4Data & 1u) << _rSifSlave.u4Shift);
                    if(_rSifSlave.u4Shift-- == 0)
                    {
                        _rSifSlave.u4Idx++;
                        _rSifSlave.u4Shift = 7;
                        _rSifSlave.u4State = STATE_ACK_DATA;
                        SET_SDA(0);                              
                    }
                    else
                    {
                        _rSifSlave.u4State = STATE_WRITE_DATA;
                    }                      
                }            
                else if(_CheckStopCond(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel) == 1)
                {
                    _rSifSlave.u4State = STATE_IDLE;                        
                }                  
                break;
            case STATE_READ_DATA:
                GET_SCL(_i4SCLLevel);            	
                // GET_SDA(_i4SDALevel);              	
                if(_i4SCLLevel == 0 && _i4PrevSCLLevel == 1)
                {
                    // Send after previous clk falling edge
                    SET_SDA((_rSifSlave.au4Data[_rSifSlave.u4Idx] >> _rSifSlave.u4Shift) & 0x1 );
                    if(_rSifSlave.u4Shift-- == 0)
                    {
                        _rSifSlave.u4Idx++;
                        _rSifSlave.u4Shift = 7;
                        _rSifSlave.u4State = STATE_WAIT_READ_DONE;
                    }
                }            	  
                break;

            case STATE_WAIT_READ_DONE:
                GET_SCL(_i4SCLLevel);            	
                // GET_SDA(_i4SDALevel);              	
                // Release SDA on falling
                if(_i4SCLLevel == 0 && _i4PrevSCLLevel == 1)
                {
                    // Set to input to wait ack
                    GET_SDA(_i4SDALevel);
                    _rSifSlave.u4State = STATE_WAIT_ACK;                    
                }
                break;
            case STATE_ACK_DATA:
                GET_SCL(_i4SCLLevel);               	
                // SET_SDA(0);    
                
                GPIO_SetOut(62, 0);
                
            	  // Ack until clock falling
                if(_i4SCLLevel == 1 && _i4PrevSCLLevel == 0)
                {
                    // HAL_Delay_us(1);
                    GET_SCL(_i4SCLLevel);               	
                    if(_i4SCLLevel == 1)
                    {
                        _rSifSlave.u4State = STATE_ACK_DATA_2;
                    }
                    _i4PrevSCLLevel = _i4SCLLevel;
                }
                break;  

            case STATE_ACK_DATA_2:
                GET_SCL(_i4SCLLevel);            	
                // GET_SDA(_i4SDALevel);              	      	
            	  // Ack until clock falling
                if(_i4SCLLevel == 0 && _i4PrevSCLLevel == 1)
                {
                    // de-bouncing
                    // HAL_Delay_us(1);
                    GET_SCL(_i4SCLLevel);
                    if(_i4SCLLevel == 0)
                    {
                        _rSifSlave.u4State = STATE_WRITE_DATA;
                        GET_SDA(_i4SDALevel);       

                        GPIO_SetOut(62, 1);

                        if(_fgSCLStretchOnData)
                        {                        
                            // Stretch SCL
                            /*
                            if(_u4SCLPrevStretchTime)
                            {
                                HAL_Delay_us(_u4SCLPrevStretchTime);
                            }  
                            */
                            SET_SCL(0);

                            GPIO_SetOut(62, 0);
                            
                            HAL_Delay_us(_u4SCLStretchTime);
                            // Release SCL
                            //GET_SCL(_i4SCLLevel);      

                            GPIO_SetOut(62, 1);

                            // Master might feed clock soon, so here we ignore SCL rising edge
                            do
                            {
                                GET_SCL(_i4SCLLevel);
                            } while(_i4SCLLevel == 0);
                            GET_SDA(_i4SDALevel);
                            i4Data = _i4SDALevel;
                            GPIO_SetOut(62, 0);                                
                            _rSifSlave.u4State = STATE_WRITE_DATA_ON_CLK_FALLING;   
                            
                        }
                        
                    }
                }
                break;
                
            case STATE_WAIT_ACK:
                GET_SCL(_i4SCLLevel);            	
                GET_SDA(_i4SDALevel);              	
                // Sampled on rising edge     
                i4Data = _SampleData(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel);
                if(i4Data >= 0)
                {
                    _rSifSlave.u4State = STATE_WAIT_ACK_ON_CLK_FALLING;
                }
                break;  
            case STATE_WAIT_ACK_ON_CLK_FALLING:
                GET_SCL(_i4SCLLevel);            	
                GET_SDA(_i4SDALevel);              	
                // Sampled on falling edge
                i4DataOnClkFalling = _WaitClkFalling(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel);
                if((i4Data == i4DataOnClkFalling) && i4Data >= 0)
                {
                    if(i4Data == 1)
                    {
                        _rSifSlave.u4State = STATE_WAIT_STOP;
                    }
                    else
                    {
                        // Send 0th bit here
                        SET_SDA((_rSifSlave.au4Data[_rSifSlave.u4Idx] >> _rSifSlave.u4Shift) & 0x1 );
                        _rSifSlave.u4Shift--;
                        _rSifSlave.u4State = STATE_READ_DATA;
                    }
                }
                else if(_CheckStopCond(_i4SDALevel, _i4PrevSDALevel, _i4SCLLevel, _i4PrevSCLLevel) == 1)
                {
                    _rSifSlave.u4State = STATE_IDLE;                        
                }                        
            	  break;
            case STATE_WAIT_STOP:
                GET_SCL(_i4SCLLevel);            	
                GET_SDA(_i4SDALevel);              	
            	  if(_i4SCLLevel == 1 && _i4PrevSCLLevel == 0)
                {
                    if(_i4SDALevel == 1)
                    {
                        _rSifSlave.u4State = STATE_IDLE;                    
                    }
                }
            	  break;
            	  
            default:
            	  break;
        }    

        _i4PrevSDALevel = _i4SDALevel;
        _i4PrevSCLLevel = _i4SCLLevel;

#ifndef CC_5391_LOADER
        LOG(3, "SDA: %d, SCL %d\n", _i4SDALevel, _i4SCLLevel);
#endif

        SAMPLING_DELAY;
    }
    
}

#ifndef CC_5391_LOADER
INT32 SIF_EMU_SLAVE_Init(void)
{
    INT32 i4Ret;
    if(_fgInit)
    {
        return 0;
    }
    
    i4Ret = x_thread_create(&_hStateMachineHandle, "SIF_EMU_SLAVE", 1024, 1, _SifSlaveStateMachine, 0, NULL);
    if(i4Ret != 0)
    {
        Printf("SIF slave simulator created failed!\n");
    }    
    return 0;
}
#else
INT32 SIF_EMU_SLAVE_Init(void)
{
#if ENABLE_SIF_SLAVE_EMULATOR
    _SifSlaveStateMachine(NULL);
#endif
    return 0;
}
#endif


