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
 * $Author: jifeng.fan $
 * $Date: 2012/05/31 $
 * $RCSfile: loader_initenv.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file smp_sample.c
 *  Brief of file loader_initenv.c
 *  Details of file loader_initenv.c
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_os.h"
#include "u_irrc_btn_def.h"
#include "x_printf.h"
#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_serial.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_dram.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
#include "x_gpio.h"
#include "x_chip_id.h"
#include "nor_if.h"

#ifdef CC_NAND_BOOT
#include "nand_if.h"
#endif

#ifdef CC_EMMC_BOOT
#include "msdc_if.h"
#endif

#include "pdwnc_if.h"
#include "drvcust_if.h"
#include "musb_if.h"
#include "loader_if.h"
#include "c_model.h"
#include "x_ldr_env.h"
#include "pcmcia_if.h"

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif

#include "dmx_drm_if.h"

#if defined(CC_MT5396) || defined(CC_MT5398)
    #ifdef __GCC__
        extern UINT32 __core1_start;
        extern UINT32 __core1_end;
    #else
        extern UINT32 Load$$CORE1$$Base;
        extern UINT32 Image$$CORE1$$Length;
    #endif
#endif

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------
static void _LdrAudioReset()
{
    // AUD_CLR_BIT(REG_INT_SDADC2, ADC_R_PD);
    IO_WRITE32(AUDIO_BASE,0x0248, (IO_READ32(AUDIO_BASE,0x0248)) & (~((0x1 << 27))));
    // AUD_CLR_BIT(REG_INT_SDADC2, ADC_L_PD);
    IO_WRITE32(AUDIO_BASE,0x0248, (IO_READ32(AUDIO_BASE,0x0248)) & (~((0x1 << 26))));
    // AUD_WRITE32(REG_INT_DACCTL4,(AUD_READ32(REG_INT_DACCTL4) & ~PD_CTL_MASK)|PD_ON);
    IO_WRITE32(AUDIO_BASE,0x01d0, (IO_READ32(AUDIO_BASE,0x01d0))& ~(0xf << 4)|(0x0 << 4));
    // AUD_CLR_BIT(REG_INT_DACCTL4,PD_ISOURCE);
    IO_WRITE32(AUDIO_BASE,0x01d0, (IO_READ32(AUDIO_BASE,0x01d0)) & (~((0x1 << 3))));
    // AUD_CLR_BIT(REG_INT_DACCTL4,PD_REF2);
    IO_WRITE32(AUDIO_BASE,0x01d0, (IO_READ32(AUDIO_BASE,0x01d0)) & (~((0x1 << 1))));
    // AUD_CLR_BIT(REG_INT_DACCTL4,PD_REF1);
    IO_WRITE32(AUDIO_BASE,0x01d0, (IO_READ32(AUDIO_BASE,0x01d0)) & (~((0x1 << 0))));
}

static void _LdrEepromRead(LDR_DATA_T *prLdrData)
{
    INT32 i4Ret;
    UINT32 u4PowerGpio, u4OnVal, u4DelayMs;

    // Enable the power gpio for eeprom access.
    i4Ret = 0;
    i4Ret |= DRVCUST_InitQuery(ePowerCtrlGpio, &u4PowerGpio);
    i4Ret |= DRVCUST_InitQuery(ePowerCtrlOnVal, &u4OnVal);
    i4Ret |= DRVCUST_InitQuery(ePowerCtrlDelayMs, &u4DelayMs);

    if (i4Ret == 0)
    {
        i4Ret = (INT32)u4OnVal;
        VERIFY(i4Ret == GPIO_Output(u4PowerGpio, &i4Ret));

        HAL_Delay_us(u4DelayMs*1000);
    }

    // read eeprom to prLdrData->rDtvCfg;
    if (EEPDTV_GetCfg(&prLdrData->rDtvCfg))
    {
        Printf("### EEPROM read loader config failed!!!\n");
    }
#ifdef CC_LOAD_UBOOT
    x_memcpy((void*)0xf00000, (void*)&prLdrData->rDtvCfg, sizeof(prLdrData->rDtvCfg));
#endif
}

static void _LdrInit8032uP(LDR_DATA_T *prLdrData)
{
    UINT32 u4Offset, u4Size;

    // Setup 8032 uP
    u4Offset = DRVCUST_InitGet(eT8032uPOffset);
    u4Size = DRVCUST_InitGet(eT8032uPSize);
    
    if (u4Offset != 0)  // here needs to setup 8032uP.
    {
        if ((DRVCUST_InitGet(eT8032uPDualOffset) != 0) &&
            (DTVCFG_FLAG2_8032UP_BANK & prLdrData->rDtvCfg.u1Flags2))
        {
            // reset 8032uP offset.
            u4Offset = DRVCUST_InitGet(eT8032uPDualOffset);
        }

        if (u4Size == 0)
        {
            u4Size = (16*1024);
        }

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
        if (u4Offset & ADDR_NAND_FLASH)
        {
            UINT32 u4Dest = (UINT32)x_mem_alloc(u4Size);
            if (u4Dest == 0)
            {
                Printf("mem alloc fail\n");
                return;
            }

        #ifdef CC_NAND_BOOT
            Loader_ReadNandFlash(0, (u4Offset &(~ADDR_NAND_FLASH)), (void *)u4Dest, u4Size);
        #else
            Loader_ReadMsdc(0, (u4Offset &(~ADDR_NAND_FLASH)), (void *)u4Dest, u4Size);
        #endif
            u4Offset = u4Dest;
        }
#endif /* CC_NAND_BOOT */

        PDWNC_InitT8032(u4Offset, u4Size);
    }
}
static void _LdrInit8280(LDR_DATA_T *prLdrData)
{
#ifdef DRV_SUPPORT_EXTMJC
    drv_extmjc_reset();
#endif
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

static void _LdrSetDdrPriority(VOID)
{
    UINT8 *pu1DramAgentPriorityList;
    UINT32 u4BurstLen;
    UINT8 u1ArbiterTime;

    // Set dram agent priority
    pu1DramAgentPriorityList = (UINT8 *)DRVCUST_InitGet(eDramAgentPriority);
    if (pu1DramAgentPriorityList)
    {
        DDR_SetAgentPriority(pu1DramAgentPriorityList);
    }

    // Set dram group 1 agent burst length
    u4BurstLen = (BOOL)DRVCUST_InitGet(eDramBurstLen);
    if (u4BurstLen > 15)
    {
        u4BurstLen = 15;
    }
    u4BurstLen |= u4BurstLen << 4;
    u4BurstLen |= u4BurstLen << 8;
    u4BurstLen |= u4BurstLen << 16;
    DDR_SetBurstLen(u4BurstLen, u4BurstLen);

    // Set dram group arbiter time
    u1ArbiterTime = (UINT8)DRVCUST_InitGet(eDramGroup1ArbiterTime);
    DDR_SetArbiterTime(1, u1ArbiterTime);
    u1ArbiterTime = (UINT8)DRVCUST_InitGet(eDramGroup2ArbiterTime);
    DDR_SetArbiterTime(2, u1ArbiterTime);
    u1ArbiterTime = (UINT8)DRVCUST_InitGet(eDramGroup3ArbiterTime);
    DDR_SetArbiterTime(3, u1ArbiterTime);

    // Enable CPU bandwidth limiter
#if defined(CC_MT5365) || defined(CC_MT5395)
    {
        UINT32 u4Reg;

        u4Reg = DRAM_READ32(0x108);
        u4Reg &= 0xF0FFFFFF;
        u4Reg |= 0x09000000;
        DRAM_WRITE32(0x108, u4Reg);

        u4Reg = DRAM_READ32(0x150);
        u4Reg |= 0x00010000;
        DRAM_WRITE32(0x150, u4Reg);

        DRAM_WRITE32(0x154, 0x8001);
    }
#endif
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void LDR_EEPWriteEnable (BOOL fgState)
{
#if !defined(CC_MTK_LOADER) || !defined(CC_LOADER_SHRINK_EEP) //loader shrink
    UINT32 u4SysWP, u4SysWPEnable;

    Printf("[yjdbg] LDR_EEPWriteEnable: %x\n", fgState);
    if (0 == DRVCUST_InitQuery(eSysEepromWPGpio, &u4SysWP) &&
       (0 == DRVCUST_InitQuery(eSysEepromWPEnablePolarity, &u4SysWPEnable)))
    {
        switch (fgState) {
        case TRUE:
            /* Enable write access */
            // disable write protect by GPIO.
            GPIO_SetOut( (INT32)u4SysWP , u4SysWPEnable?0:1 );
            break;
            
        case FALSE:
            /* Disable write access,  */
            // Enable write protect by GPIO.
            GPIO_SetOut( (INT32)u4SysWP , u4SysWPEnable?1:0 );
            break;
            
        default:
            break;
        }
    }
#endif
}

#if (defined(CC_MT5396) || defined(CC_MT5398)) && !defined(CC_FPGA_ARM11)
extern void BIM_Core1Stub(void);
void _LinkSymbol(UINT32 fgDisplay)
{
    if (fgDisplay)
    {
        BIM_Core1Stub();
    }
}
#endif

UINT32 LDR_PreInit(VOID)
{
    UINT32 i = 0;
    UINT8 au1Key[16] = {};

#if (defined(CC_MT5396) || defined(CC_MT5398)) && !defined(CC_FPGA_ARM11)
    #ifdef __GCC__
        UINT32 u4Core1StubAddr = (UINT32)&__core1_start;
        UINT32 u4Core1StubSize = (UINT32)&__core1_end - (UINT32)&__core1_start;
    #else
        UINT32 u4Core1StubAddr = (UINT32)&Load$$CORE1$$Base + 4; // skip thumb-interworking code
        UINT32 u4Core1StubSize = (UINT32)&Image$$CORE1$$Length;
    #endif

    for (i = 0; i < (u4Core1StubSize/4); i++)
    {
        ((volatile UINT32*)(SRAM_CORE1_BASE+4))[i] = ((UINT32*)(u4Core1StubAddr))[i];
        //Printf("sram[0x%08x]=0x%08x\n", &((volatile UINT32*)(SRAM_CORE1_BASE+4))[i], ((volatile UINT32*)(SRAM_CORE1_BASE+4))[i]);
    }

    //ASSERT(x_memcpy((VOID*)(SRAM_CORE1_BASE+4), (VOID*)u4Core1StubAddr, u4Core1StubSize)==0);
    ASSERT(x_memcmp((VOID*)(SRAM_CORE1_BASE+4), (VOID*)u4Core1StubAddr, u4Core1StubSize)==0);

    ARM_SendEvent();

    _LinkSymbol(0);
#endif


#ifndef CC_FPGA
    _LdrSetDdrPriority();
#else
    UNUSED(_LdrSetDdrPriority);
#endif //CC_FPGA

#if defined(CC_MT5365) || defined(CC_MT5395)
#if !defined(CC_MT5368) && !defined(CC_MT5396) && !defined(CC_MT5398) && !defined(CC_MT5880) && !defined(CC_MT5860)
    CI_GPIO_Init();
#endif
#endif

    for (i = 0; i < LDR_TIMESTAMP_SIZE; i++)
    {
        ((LDR_ENV_T*)CC_LDR_ENV_OFFSET)->au4TimeStamp[i] = 0;
    }
    LDR_SetDramTimeLog(0);
#ifdef LOADER_USB_UPGRADE
#ifndef CC_USB_DISABLE
    if (MUSB_Init(1))
    {
        Printf("Init USB fail\n");
        return 1;
    }
#endif
#endif
    LDR_SetDramTimeLog(1);
    //LDR_EEPWriteEnable(TRUE);

    if(BIM_IS_SECURE_BOOT)
    {
        x_memcpy(au1Key, (void *)BIM_MTK_KEY_ADDR, 16);
        for(i = 0; i < 16; i++)
        {
            au1Key[i] ^= 0x55;
        }
        DMX_NAND_AES_INIT(au1Key, 128);
    }
    else
    {
        for(i = 0; i < 16; i++)
        {
            au1Key[i] ^= 0x55;
        }
        DMX_NAND_AES_INIT(au1Key, 128);        
    }

    // enable SMC accessible in normal world
    BIM_WRITE32(0x2C, BIM_READ32(0x2C) | 0x10000000);

    LDR_SetDramTimeLog(2);
    return 0;
}

UINT32 LDR_EnvInit(LDR_DATA_T *prLdrData)
{
    PFN_LDR_ENVINIT_FUNC pfnLdrEnvInit = NULL;
    UINT32 u4LdrVerOffset;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T *)CC_LDR_ENV_OFFSET;
    LDR_ENV_T* prLdrSramEnv = (LDR_ENV_T *)(SRAM_DMX_BASE);

    #ifdef DRV_SUPPORT_MT8283
	SIF_Init();
    _LdrInit8280(prLdrData);
    #endif

    // call driver customization loader env init function.
    DRVCUST_InitQuery(eLoaderEnvInitFunc, (UINT32 *)(void *)&pfnLdrEnvInit);
    if (pfnLdrEnvInit != NULL)
    {
        pfnLdrEnvInit(prLdrData, 0);
    }

    LDR_SetDramTimeLog(4);

    _LdrAudioReset();
    LDR_SetDramTimeLog(5);

    _LdrEepromRead(prLdrData);
    LDR_SetDramTimeLog(6);

    _LdrInit8032uP(prLdrData);
    LDR_SetDramTimeLog(7);
#ifndef DRV_SUPPORT_MT8283
    _LdrInit8280(prLdrData);
#endif
    LDR_SetDramTimeLog(8);

    u4LdrVerOffset = prLdrEnv->u4LoaderLzhsSize + prLdrEnv->u4LoaderLzhsAddress;

#if defined(CC_SECURE_BOOT_ALL)
	// t_mtk padding
    u4LdrVerOffset = (u4LdrVerOffset + 0x10) & ~0xf;
	// image signature
    u4LdrVerOffset += 0x100;
#endif
#if defined(CC_SECURE_BOOT_CHECK_CUSTKEY)
    // customer key signature
    u4LdrVerOffset += 0x100;
#endif

    // refer to tools/mt5391_pack/mt5391_pack.c for 0x80 alignment
    u4LdrVerOffset = (u4LdrVerOffset + 0x7f) & ~0x7f;
    Printf("LDR_FlashCopy 0x%x 0x%x 0x%x\n", prLdrEnv->u.au1Raw, u4LdrVerOffset, sizeof(LDR_ENV_HDR_T));
    LDR_FlashCopy(prLdrEnv->u.au1Raw, u4LdrVerOffset, sizeof(LDR_ENV_HDR_T));
    // duplicate in sram for linux AP reference
    x_memcpy((void*)prLdrSramEnv->u.au1Raw, (void*)prLdrEnv->u.au1Raw, sizeof(LDR_ENV_HDR_T));
    LDR_SetDramTimeLog(9);

    if (pfnLdrEnvInit != NULL)
    {
        pfnLdrEnvInit(prLdrData, 1);
    }
    LDR_SetDramTimeLog(10);

#ifdef REPLACE_EEPROM_WITH_FLASH
    {
        INT32 i4Ret;

        // Download 4KB EEPROM data to DRAM offset: eEepromDataMemOffset
        i4Ret = EEPROM_Read(0, DRVCUST_InitGet(eEepromDataMemOffset), 0x1000);
        if (i4Ret != 0)
        {
            Printf("Dump EEPROM to DRAM fail\n");
        }
    }
#endif

    return 0;
}

