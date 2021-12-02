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
#include "pdwnc_entry.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "drvcust_if.h"
#include "x_lint.h"
#include "pdwnc_t8032.h"

#include "x_assert.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define XDATA_ADDR          0x4000
#define CEC_XDATA_OFFSET    0x400
#define CEC_XDATA_SIZE      0x400
#define CEC_RISC_ADDR       0x0
#define UCODE_SIZE          (16 * 1024)
#define UCODE_ADDR          0x0

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern void HAL_Delay_us(UINT32 u4Micros);

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
INT32 _PDWNC_SetT8032(UINT32 u4State);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hSetT8032Mutex = NULL;

static UINT8 _u1PowerDownFlag;
static BOOL _fgReadSwVersionReadyFlag;
static BOOL _fgEnterNormalReadyFlag;
static BOOL _fgXDataSwitchReadyFlag;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _CopyToT8032(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{
    UINT32 i;
    UINT8* pu1Ptr = (UINT8*)u4RiscAddr;

    PDWNC_WRITE32(REG_RW_UP_ADDR, u4XAddr);

    for(i = 0; i < u4Size; i++)
    {
        PDWNC_WRITE8(REG_RW_UP_DATA, *(pu1Ptr + i));    
    }    
    return 0;
}

static INT32 _CopyFromT8032(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{
    UINT32 i;
    UINT8* pu1Ptr = (UINT8*)u4RiscAddr;

    PDWNC_WRITE32(REG_RW_UP_ADDR, u4XAddr);

    for(i = 0; i < u4Size; i++)
    {
        *(pu1Ptr + i) = PDWNC_READ8(REG_RW_UP_DATA);    
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


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**  is the interrupt routine for GPIO pins of PDWNC module.
 *  @param u2Gpio is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
#ifndef CC_5391_LOADER  
void _PDWNC_T8032Isr(UINT16 u2Vector)
{
    CRIT_STATE_T rCrit;
    UINT32 u4Cmd, u4Val;
    
    u4Val = PDWNC_READ32(REG_RW_AUX_DATA);
    u4Cmd = u4Val & 0xff; 
    u4Val >>= 8;
    
    if(u4Cmd & PDWNC_CMD_T8032ARM_ENTER_NORMAL_READY)
    {
        rCrit = x_crit_start();
        _fgEnterNormalReadyFlag = TRUE;
        x_crit_end(rCrit);        
    }
    else if(u4Cmd & PDWNC_CMD_T8032ARM_RESPONSE_SW_VERSION)
    {
        rCrit = x_crit_start();
        _fgReadSwVersionReadyFlag = TRUE;
        x_crit_end(rCrit);       
    }
    else if(u4Cmd & PDWNC_CMD_T8032ARM_RESPONSE_XDATA_SWITCH)
    {
        rCrit = x_crit_start();
        _fgXDataSwitchReadyFlag = TRUE;
        x_crit_end(rCrit);       
    }    
}
#endif


//-----------------------------------------------------------------------------
/**  PDWNC_SetupCEC to setup CEC wakeup before entering standby.
 *  @param void
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_SetupCEC(void)
{
    UINT32 u4RiscAddr;
    
    INT32 i4Ret = 0;

    // Request CEC driver to stop CEC ISR handling after current transaction finish
    PDWNC_SetPowerDownFlag(PDWNC_WAIT_CEC_TRANSACTION);
    while(PDWNC_GetPowerDownFlag() != PDWNC_CEC_TRANSACTION_FINISH)
    {
        HAL_Delay_us(10);                   
    }

    _PDWNC_SetT8032(PDWNC_MSK_T8032_NO_XDATA);

    // Wait until T8032 enter normal mode ready
    while(!_fgXDataSwitchReadyFlag)
    {
        HAL_Delay_us(10);            
    }

    // TODO: Should set XData offset storing CEC data
    // Copy CEC data to XData
    u4RiscAddr = CEC_RISC_ADDR;
    i4Ret = _CopyToT8032(XDATA_ADDR + CEC_XDATA_OFFSET, CEC_XDATA_SIZE, u4RiscAddr);
    
    return i4Ret; 
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**  PDWNC_SetT8032 set T8032 to the specified state.
 *  @param u4State T8032 state to set
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_SetT8032(UINT32 u4State)
{
    INT32 i4Ret = 0;
    UINT32 u4Val;   
    
    if(u4State & PDWNC_MSK_T8032_NO_XDATA)    
    {       
        u4Val = PDWNC_READ32(REG_RW_UP_CFG);    
        u4Val &= ~XDATA_ACC;
        PDWNC_WRITE32(REG_RW_UP_CFG, u4Val);
    }
    else if(u4State & PDWNC_MSK_T8032_NORMAL)
    {
        // Signal 8032 to enter normal mode
        PDWNC_WRITE32(REG_RW_ADDR, PDWNC_CMD_ARMT8032_ENTER_NORMAL);
        PDWNC_WRITE32(REG_RW_DATA, 0x0);
        _fgEnterNormalReadyFlag = FALSE;
        PDWNC_WRITE32(REG_RW_INT, 0x1);     

        // Wait until T8032 enter normal mode ready
        while(!_fgEnterNormalReadyFlag)
        {
            HAL_Delay_us(10);            
        }
        PDWNC_WRITE32(REG_RW_INT, 0x0);     

    }
    else if(u4State & PDWNC_MSK_T8032_STANDBY)
    {
   	  // Enable 8032 Xdata access
        u4Val = PDWNC_READ32(REG_RW_UP_CFG);    
        u4Val |= XDATA_ACC;
        PDWNC_WRITE32(REG_RW_UP_CFG, u4Val);
        
        // Signal 8032 to enter standby
        PDWNC_WRITE32(REG_RW_ADDR, PDWNC_CMD_ARMT8032_ENTER_STANDBY);
        PDWNC_WRITE32(REG_RW_DATA, 0x0);
        PDWNC_WRITE32(REG_RW_INT, 0x1);              
        
        while(1);
    }    
    else
    {
        i4Ret = -1;
    }        
    
    return i4Ret;
}


//-----------------------------------------------------------------------------
/**  PDWNC_InitT8032 to initialize T8032.
 *  @param void.
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_InitT8032(void)
{
    INT32 i4Ret = 0;
    UINT32 u4T8032CodeAddr = 0;
    UINT32 u4RiscAddr;
    UINT32 u4Size;
    
#ifndef CC_5391_LOADER      
    i4Ret = x_sema_create(&_hSetT8032Mutex, X_SEMA_TYPE_MUTEX,
                        X_SEMA_STATE_UNLOCK);
#endif

    // If A/C on, load uCode to T8032
    if(PDWNC_ReadWakeupReason() == PDWNC_WAKE_UP_REASON_AC_POWER)
    {
        UINT32 u4Val;
        u4Val = PDWNC_READ32(REG_RW_UP_CFG);
        u4Val &= ~(UART_EN | FAST_EN | ENGINE_EN | CEC_EN | VIR_EN | XDATA_ACC);
        PDWNC_WRITE32(REG_RW_UP_CFG, u4Val);

        // TODO: Should set uCode size first
        // Load uP code      
        u4T8032CodeAddr = UCODE_ADDR;
        u4Size = UCODE_SIZE;        
        VERIFY(_LoadCode(u4T8032CodeAddr, u4Size) == 0);        

        PDWNC_WRITE32(REG_RW_UP_CFG, T8032_RST);
    }
    else
    {
        _PDWNC_SetT8032(PDWNC_MSK_T8032_NO_XDATA);

        // TODO: Should Set CEC data size and addr
        // Copy CEC data from XData
        u4Size = CEC_XDATA_SIZE;
        u4RiscAddr = CEC_RISC_ADDR;
        VERIFY(_CopyFromT8032(XDATA_ADDR + CEC_XDATA_OFFSET, u4Size, u4RiscAddr) == 0);   
    
        _PDWNC_SetT8032(PDWNC_CMD_ARMT8032_ENTER_NORMAL);
    }

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** PDWNC_ReadSwVersion reads SW version from T8032.
 *  @param u2Gpio is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_ReadSwVersion(void)
{
    INT32 i4Ret = 0;
#ifndef CC_5391_LOADER      
    // Avoid other thread's simultaneous write to INT
    i4Ret = x_sema_lock_timeout(_hSetT8032Mutex, 1000);    
#endif
    // Signal 8032 to enter normal mode
    PDWNC_WRITE32(REG_RW_ADDR, PDWNC_CMD_ARMT8032_QUERY_SW_VERSION);
    PDWNC_WRITE32(REG_RW_DATA, 0x0);
    _fgReadSwVersionReadyFlag = FALSE;
    PDWNC_WRITE32(REG_RW_INT, 0x1);     
    // Wait until T8032 enter normal mode ready
    while(!_fgReadSwVersionReadyFlag)
    {
        HAL_Delay_us(10);            
    }
    PDWNC_WRITE32(REG_RW_INT, 0x0);       
    
#ifndef CC_5391_LOADER      
    x_sema_unlock(_hSetT8032Mutex);         
#endif
    return i4Ret;
}


//-----------------------------------------------------------------------------
/** PDWNC_SetPowerDownFlag to set power down flag.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
void PDWNC_SetPowerDownFlag(UINT8 u1Flag)
{
    _u1PowerDownFlag = u1Flag;
}


//-----------------------------------------------------------------------------
/** PDWNC_GetPowerDownFlag to get power down flag.
 *  @param void.
 *  @retval the power down flag got
 */
//-----------------------------------------------------------------------------
UINT8 PDWNC_GetPowerDownFlag(void)
{
    return _u1PowerDownFlag;
}

