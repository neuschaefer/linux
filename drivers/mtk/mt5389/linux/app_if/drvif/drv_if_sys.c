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
 * $RCSfile: drv_if_sys.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sys.c
 *  Driver interface: System related interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "x_os.h"
#include "x_pdwnc.h"
#include "drvcust_if.h"
#include "storage_if.h"
#include "eeprom_if.h"
#include "nor_if.h"
#include "rtc_if.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT32 _u4NorPartSize = 0;
static UINT8 _u1NorPartNum = 0;
static BOOL _fgInit = FALSE;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static UINT32 _u4WakeupSetting;
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Exported functionss
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_SYS_Init(void)
{        
    UINT32 i;

    if(_fgInit)
    {
        return DRV_E_OK;
    }
    
    GPIO_Init();    
    SIF_Init();    
    VERIFY(RTC_Init()); 
    
    //NORHW_Init();
    //NOR_Init();
    
    VERIFY(STORGInit() == 0);

    for(i = 0; i < PART_MIDD_END; i++)
    {
        if( (_u4NorPartSize = NORPART_GetPartSize(i)) != 0)
        {
            _u1NorPartNum = i;    
            break;
        }
    }
    _fgInit = TRUE;
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_SYS_ReadEep(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    DRV_ERR_CODE_T rRetCode;
    INT32 i4Ret;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt;

    if(!_fgInit)
    {
        VERIFY(DRV_SYS_Init() == DRV_E_OK);
    }

    rRetCode = DRV_E_OK;
    // check boundary.
    if ((u8Offset + u4MemLen) > DRVCUST_OptGet(eSystemEepromSize))
    {
        return DRV_E_INV_ARG;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DRIVER_SIZE;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);

    if (prDev == NULL)
    {
        return DRV_E_FAIL;
    }

    // send to storage task and do sync IO.
    i4Ret = STORGSendReq(prDev, STORG_OP_READ,
    	NULL, NULL, NULL,
    	u8Offset, u4MemLen,
    	(void *)u4MemPtr, &u4XferCnt);

    if ((i4Ret) || (u4XferCnt != u4MemLen))
    {
        rRetCode = DRV_E_FAIL;
    }

    UNUSED(STORGCloseDev(prDev));   

    return rRetCode;
}

DRV_ERR_CODE_T DRV_SYS_WriteEep(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    DRV_ERR_CODE_T rRetCode;
    INT32 i4Ret;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt;

    if(!_fgInit)
    {
        VERIFY(DRV_SYS_Init() == DRV_E_OK);
    }

    rRetCode = DRV_E_OK;
    // check boundary.
    if ((u8Offset + u4MemLen) > DRVCUST_OptGet(eSystemEepromSize))
    {
        return DRV_E_INV_ARG;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DRIVER_SIZE;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);

    if (prDev == NULL)
    {
        return DRV_E_FAIL;
    }

    // send to storage task and do sync IO.
    i4Ret = STORGSendReq(prDev, STORG_OP_WRITE,
    	NULL, NULL, NULL, 
    	u8Offset, u4MemLen,
    	(void *)u4MemPtr, &u4XferCnt);

    if ((i4Ret) || (u4XferCnt != u4MemLen))
    {
        rRetCode = DRV_E_FAIL;
    }

    UNUSED(STORGCloseDev(prDev));
     
    return rRetCode;
}

DRV_ERR_CODE_T DRV_SYS_ReadNor(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    UINT64 u8Offset;
    STORG_DEV_T *prDev;

    if(!_fgInit)
    {
        VERIFY(DRV_SYS_Init() == DRV_E_OK);
    }
    
    if((u4Offset+u4MemLen) > _u4NorPartSize)
    {
        return DRV_E_INV_ARG;
    }
    if(u4MemPtr == 0)
    {
        return DRV_E_INV_ARG;
    }

    // NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
    u8Offset = (UINT64)u4Offset;
    u8Offset |= ((UINT64)_u1NorPartNum) << 32; 

    prDev = STORGOpenDev(STORG_FLAG_NOR);
    
    i4Ret = STORGSendReq(prDev, STORG_OP_READ, NULL, NULL, NULL,
                            u8Offset, u4MemLen, (void *)u4MemPtr, NULL);

    VERIFY(0 == STORGCloseDev(prDev));
    
    if(i4Ret != 0)
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_SYS_WriteNor(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    UINT64 u8Offset;
    STORG_DEV_T *prDev;

    if(!_fgInit)
    {
        VERIFY(DRV_SYS_Init() == DRV_E_OK);
    }
       
    if((u4Offset+u4MemLen) > _u4NorPartSize)
    {
        return DRV_E_INV_ARG;
    }
    if(u4MemPtr == 0)    
    {
        return DRV_E_INV_ARG;
    }

    // NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
    u8Offset = (UINT64)u4Offset;
    u8Offset |= ((UINT64)_u1NorPartNum) << 32; 

    prDev = STORGOpenDev(STORG_FLAG_NOR);
    
    i4Ret = STORGSendReq(prDev, STORG_OP_WRITE, NULL, NULL, NULL,
                            u8Offset, u4MemLen, (void *)u4MemPtr, NULL);

    VERIFY(0 == STORGCloseDev(prDev));

    if(i4Ret != 0)
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}


DRV_ERR_CODE_T DRV_SYS_EraseNor(void)
{
    if(!_fgInit)
    {
        VERIFY(DRV_SYS_Init() == DRV_E_OK);
    }

    if(NORSTG_Erase(_u1NorPartNum) != 0)
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_SYS_SetWakeup(DRV_SYS_WAKEUP_T* prWakeupSetting)
{
    if(prWakeupSetting->fgIRWakeup)
    {
        _u4WakeupSetting |= WAK_IRRX;
    }
    if(prWakeupSetting->fgRTCWakeup)
    {
        _u4WakeupSetting |= WAK_RTC;
    }
    if(prWakeupSetting->fgVGAWakeup)
    {
        _u4WakeupSetting |= WAK_VGA;
    }
    if(prWakeupSetting->fgRS232Wakeup)
    {
        _u4WakeupSetting |= WAK_RS232;
    }
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_SYS_SetTime(const UINT64 u8Utc)
{
    if(!_fgInit)
    {
        VERIFY(DRV_SYS_Init() == DRV_E_OK);
    }
    
    if(!RTC_SetTimeDate(&u8Utc))
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_SYS_GetTime(const UINT64 *pu8Utc)
{
    if(!_fgInit)
    {
        VERIFY(DRV_SYS_Init() == DRV_E_OK);
    }
    
    if(!RTC_GetTimeDate(pu8Utc))
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}

DRV_ERR_CODE_T DRV_SYS_SetAlarm(const UINT64 u8Utc)
{
    if(!_fgInit)
    {
        VERIFY(DRV_SYS_Init() == DRV_E_OK);
    }
    
    if(!RTC_SetAlertTime(u8Utc))
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}

void DRV_SYS_Reboot(void)
{   
    PDWNC_Reboot();
}

void DRV_SYS_Down(void)
{
    PDWNC_EnterPowerDown(_u4WakeupSetting);    
}

void* DRV_SYS_AllocDmaMemory(UINT32 u4Size)
{
    return BSP_AllocDmaMemory(u4Size);
}

void* DRV_SYS_AllocAlignedDmaMemory(UINT32 u4Size, UINT32 u4Alignment)
{
    return BSP_AllocAlignedDmaMemory(u4Size, u4Alignment);
}

BOOL DRV_SYS_FreeDmaMemory(void* pvBuf)
{
    return BSP_FreeDmaMemory(pvBuf);
}

BOOL DRV_SYS_FreeAlignedDmaMemory(void* pvBuf)
{
    return BSP_FreeAlignedDmaMemory(pvBuf);
}

void DRV_SYS_FlushInvalidateCache(void)
{
    HalFlushInvalidateDCache();   
}
