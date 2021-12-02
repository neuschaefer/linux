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
 * $Date: 2012/09/06 $
 * $RCSfile: loader_upgrade.c,v $
 * $Revision: #14 $
 *
 *---------------------------------------------------------------------------*/

/** @file loader_upgrade.c
 *  This file provides upgrade check and execution mechanism functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "loader_if.h"
#include "drvcust_if.h"
#include "musb_if.h"
#include "x_pdwnc.h"
#include "x_pwr_ctrl.h"
#include "x_serial.h"

#ifdef CC_NAND_BOOT
#include "nand_if.h"
#endif

#ifdef CC_EMMC_BOOT
#include "msdc_if.h"
#endif

#include "nor_if.h"
#include "pdwnc_if.h"
#include "loader_imghdr.h"
#include "c_model.h"
#include "x_hal_5381.h"

#ifdef LOADER_USB_UPGRADE
#include "x_fm.h"
#include "x_fm_fat.h"
#include "fm_ufs.h"
#include "x_dlm.h"
#include "x_timer.h"
#include "x_ldr_env.h"
#include "x_bim.h"
#endif

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT8 _UsbKernelEN ;
extern UINT8 _BootNormal;
extern UINT8 _TryUpgradeAutoPkg;

EXTERN void MUSB_NoneRunBackground(void);
EXTERN INT32 handle_init (UINT16   ui2_num_handles,
                          VOID**   ppv_mem_addr,
                          SIZE_T*  pz_mem_size);

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    NAND = 0,
    NOR = 1,
} LDR_UPG_FLASH_TYPE_T;

typedef struct _LDR_UPG_CFG_
{
    CHAR    szTagName[5];
    LDR_UPG_FLASH_TYPE_T    eFlashType;
    UINT8     u1PartitionID;
} LDR_UPG_CFG_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef LOADER_USB_UPGRADE
#ifndef CC_USB_DISABLE
UINT32 _u4Upgraded = 0;
UCHAR* _pucFileBuf = NULL;
UINT32  u4UsbKernelSize=0;
static UINT32 _u4FileSize = 0;
static CHAR   _szFileName[64];
static UINT32 _u4BufSize;
static UINT32 _u4ImageStartAddr;
static UINT8 _u1CurrentPartitionId = 0xFF;
static UINT32 *_pu4NorBuffer = NULL;

#define MAX_UPG_CFG_NUM 32
static LDR_UPG_CFG_T _arUpgCfg[MAX_UPG_CFG_NUM];
static UINT32 _u4NoOfTags;

#if defined(CC_SECURE_BOOT) && defined(CC_LOADER_VERIFY_PKG)
static UINT32 _u4UsigCmd = USIG_VERIFY_FAIL;
#endif

#endif // CC_USB_DISABLE
#endif // LOADER_USB_UPGRADE

#define USB_ROOT_PATH "/mnt/usb_0"
#define BYTES_PER_READ 4096
#ifndef MAX_UPGRADE_IMAGE_FILE_SIZE
#define MAX_UPGRADE_IMAGE_FILE_SIZE 64 * 1024 * 1024
#endif
#define FIRMWARE_FLASH_OFFSET           LOADER_MAX_SIZE

#ifdef DEBUG
#undef DEBUG
//#define USB_UPGRADE_DEBUG
#endif

#ifdef USB_UPGRADE_DEBUG
#define DEBUG(fmt,args...)  Printf (fmt ,##args)
#else
#define DEBUG(fmt,args...)
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef LOADER_USB_UPGRADE
#ifndef CC_USB_DISABLE

static void _LdrLoaderUpgradeReboot(void)
{
    #define REG_RW_WATCHDOG_TMR 0x0104          // Watchdog Timer Register
    #define REG_RW_WATCHDOG_EN  0x0100          // Watchdog Timer Control Register
    #define PDWNC_WRITE32(offset, value)    IO_WRITE32(PDWNC_BASE, (offset), value)
    #define PDWNC_READ32(offset)            IO_READ32(PDWNC_BASE, (offset))

    if(PDWNC_READ32(REG_RW_WATCHDOG_EN)==0)
    {
        PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0xffff0000);
        PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0xffff);
    }
}

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
static INT32 _LdrFlashDynamicDataWrite(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    UINT32 u4Offset, u4Size = 0;
    VOID *pvMemPtr;
    
    if (e_flag != DLM_WRITE)
    {
        DEBUG("e_flag type error (DLM_WRITE only)\n");
        return DLM_OTHERS;
    }

    u4Size = *ui4_len;
    pvMemPtr = (VOID *)pauc_buffer;

    if ((_u4ImageStartAddr & (_u4BufSize-1)))
    {
        DEBUG("image write address is not %d Byte alignment\n", _u4BufSize);
        return -1;
    }

    u4Offset = _u4ImageStartAddr - DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));

#if defined(CC_NAND_BOOT)
    if (Loader_WriteNandFlash(_u1CurrentPartitionId, u4Offset, pvMemPtr, u4Size) != u4Size)
#else
    if (Loader_WriteMsdc(_u1CurrentPartitionId, u4Offset, pvMemPtr, u4Size) != u4Size)
#endif
    {
        Printf("Flash write fail: Offset:0x%08x, Size:0x%x\n", _u4ImageStartAddr, u4Size);
        while(1);
    }

    Printf("Flash write complete: Offset:0x%08x, Size:0x%x\n", _u4ImageStartAddr, u4Size);
    _u4ImageStartAddr += u4Size;
    
    return DLM_OK;
}

static INT32 _LdrFlashWrite(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    UNUSED(_pu4NorBuffer);
    return _LdrFlashDynamicDataWrite(e_flag, pauc_buffer, ui4_len);
}

#else // nand or emmc boot

static INT32 _LdrFlashWrite(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    INT32 i4Size = 0;
    UINT32 u4BlkSize, u4MemPtr, i;
    UINT32 *pu4Cmp;
    UINT32 cnt = 0;
    UINT32 u4FlashAddr = PBI_A_BASE + DRVCUST_InitGet(eNorFlashLoaderSize);

    if (e_flag != DLM_WRITE)
    {
        DEBUG("e_flag type error (DLM_WRITE only)\n");
        return DLM_OTHERS;
    }

    i4Size = *ui4_len;
    u4BlkSize = _u4BufSize;
    u4MemPtr = (UINT32)pauc_buffer;

    if(_pu4NorBuffer == NULL)
    {
        _pu4NorBuffer = x_mem_alloc(u4BlkSize);
    }

    if ((*ui4_len) != _u4BufSize)
    {
        x_memcpy(_pu4NorBuffer, (const void *)(u4FlashAddr+cnt), 0x10000);
    }

    if ( _u4ImageStartAddr & (u4BlkSize-1))
    {
        DEBUG("image write address is not %d Byte alignment\n", u4BlkSize);
        return -1;
    }

    do
    {
        if (NOR_EraseBlk(_u4ImageStartAddr))
        {
            Printf("Nor flash erase fail: Offset:0x%08x\n", _u4ImageStartAddr);
        }

        if (NOR_Write((UINT64)_u4ImageStartAddr, u4MemPtr, u4BlkSize) )
        {
            Printf("Nor flash write fail: Offset:0x%08x, BlkSize:0x%x\n", _u4ImageStartAddr, u4BlkSize);
        }

        if (NOR_Read((UINT64)_u4ImageStartAddr, (UINT32)_pu4NorBuffer, u4BlkSize) )
        {
            Printf("Nor flash read fail: Offset:0x%08x, BlkSize:0x%x\n", _u4ImageStartAddr, u4BlkSize);
        }

        pu4Cmp = (UINT32 *)u4MemPtr;
        for (i = 0; i < u4BlkSize/4; i++)
        {
            if (pu4Cmp[i] != _pu4NorBuffer[i])
            {
                _u4ImageStartAddr += u4BlkSize;
                DEBUG("compare fail\n");
                return DLM_WRITE_DATA_FAILED;
            }
        }

        Printf("Nor flash write complete: Offset:0x%08x, BlkSize:0x%x\n", _u4ImageStartAddr, u4BlkSize);
        u4MemPtr += u4BlkSize;
        i4Size -= u4BlkSize;
        _u4ImageStartAddr += u4BlkSize;
    }while(i4Size>0);

    return DLM_OK;
}
#endif //  nor boot

static INT32 _LdrAllUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
        _u4ImageStartAddr = 0;
        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
}

static INT32 _LdrFirmUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
        _u4ImageStartAddr = LOADER_MAX_SIZE;
#else
        _u1CurrentPartitionId = DRVCUST_InitGet(eNANDFlashPartIdSysImgA);
        _u4ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#endif

        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
}

static INT32 _LdrFir2Upgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
        _u4ImageStartAddr = LOADER_MAX_SIZE;
#else
        _u1CurrentPartitionId = DRVCUST_InitGet(eNANDFlashPartIdSysImgB);
        _u4ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#endif

        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
}

static INT32 _LdrMicpUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
        _u1CurrentPartitionId = DRVCUST_InitGet(eNANDFlashPartIdUpA);
        _u4ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
}

static INT32 _Ldr8281Upgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    UNUSED(e_flag);
    UNUSED(pauc_buffer);
    UNUSED(ui4_len);

#ifdef DRV_SUPPORT_EXTMJC
    static UINT32 u4SAddress = 0;
    UINT32 u4Len;
    UINT8 ui1_err;
    
    UNUSED(ui1_err);

    if (FALSE == drv_extmjc_is_enable())
    {
        return DLM_OK;
    }

    Printf("\n_Ldr8281Upgrade ............\n");
    if (e_flag != DLM_WRITE)
    {
        DEBUG("e_flag type error (DLM_WRITE only)\n");
        return DLM_OTHERS;
    }

    u4Len = (UINT32)(*ui4_len);

#ifdef DRV_SUPPORT_MT8283   
    drv_extmjc_i2c_access_enable_password(&ui1_err);
    u1ExtmjcWriteRegByI2C(0x81C8, 0);
#endif

    if(u4SAddress == 0x0)
    {
        d_custom_extmjc_upgrade_open();
    }

#ifdef DRV_SUPPORT_MT8283    
    d_custom_extmjc_register_test();
#endif

    Printf("\n_Ldr8282 Upgrade Address:%x, Size:%x\n", u4SAddress, u4Len);

#ifndef DRV_SUPPORT_MT8283
    d_custom_extmjc_upgrade_erase();
#else
    d_custom_extmjc_upgrade_erase(u4SAddress, u4Len);
#endif

    d_custom_extmjc_upgrade_write(u4SAddress, pauc_buffer, u4Len);
    u4SAddress = u4SAddress + u4Len;

    if(u4Len % 0x10000 != 0)
    {
        d_custom_extmjc_upgrade_close();
        Printf("\n_Ldr8282Upgrade Close");
        Printf("\n_Ldr8282 Reset");
        drv_extmjc_reset();
    }
#endif
    return DLM_OK;
}

static INT32 _LdrLoaderUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
        _u1CurrentPartitionId = 0;
        _u4ImageStartAddr = 0;
        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
    
#else // nor boot

    UINT32 cnt = 0;
    UINT32 u4FlashAddr = PBI_A_BASE + DRVCUST_InitGet(eNorFlashLoaderSize);
    INT32 i4Size = 0;
    UINT32 u4BlkSize, u4MemPtr, i;
    UINT32 *pu4Cmp;
    static UINT32 _u4LoaderImageStartAddr = 0;

    if (e_flag != DLM_WRITE)
    {
        DEBUG("e_flag type error (DLM_WRITE only)\n");
        return DLM_OTHERS;
    }

    i4Size = *ui4_len;
    u4BlkSize = _u4BufSize;
    u4MemPtr = (UINT32)pauc_buffer;
    
    if(_pu4NorBuffer == NULL)
    {
       _pu4NorBuffer = x_mem_alloc(u4BlkSize);
    }

    if ((*ui4_len) != _u4BufSize)
    {
        x_memcpy(_pu4NorBuffer, (const void *)(u4FlashAddr+cnt), 0x10000);
    }

    if ( _u4LoaderImageStartAddr & (u4BlkSize-1))
    {
        DEBUG("image write address is not %d Byte alignment\n", u4BlkSize);
        return -1;
    }

    do
    {
        if (NOR_EraseBlk(_u4LoaderImageStartAddr))
        {
            Printf("Nor flash erase fail: Offset:0x%08x\n", _u4LoaderImageStartAddr);
        }

        if (NOR_Write((UINT64)_u4LoaderImageStartAddr, u4MemPtr, u4BlkSize) )
        {
            Printf("Nor flash write fail: Offset:0x%08x, BlkSize:0x%x\n", _u4LoaderImageStartAddr, u4BlkSize);
        }

        if (NOR_Read((UINT64)_u4LoaderImageStartAddr, (UINT32)_pu4NorBuffer, u4BlkSize) )
        {
            Printf("Nor flash read fail: Offset:0x%08x, BlkSize:0x%x\n", _u4LoaderImageStartAddr, u4BlkSize);
        }

        pu4Cmp = (UINT32 *)u4MemPtr;
        for (i = 0; i < u4BlkSize/4; i++)
        {
            if (pu4Cmp[i] != _pu4NorBuffer[i])
            {
                _u4LoaderImageStartAddr += u4BlkSize;
                DEBUG("compare fail\n");
                return DLM_WRITE_DATA_FAILED;
            }
        }

        Printf("Nor flash write complete: Offset:0x%08x, BlkSize:0x%x\n", _u4LoaderImageStartAddr, u4BlkSize);
        u4MemPtr += u4BlkSize;
        i4Size -= u4BlkSize;
        _u4LoaderImageStartAddr += u4BlkSize;
    }while(i4Size>0);

    return DLM_OK;
#endif
}

static INT32 _LdrDynamicUpgrade(UCHAR* s_tag_name, DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *pi4_len)
{
    UINT32 u4Idx = 0;

    for (u4Idx = 0; u4Idx < _u4NoOfTags; u4Idx++)
    {
        if (x_strncmp(_arUpgCfg[u4Idx].szTagName, (const char *)s_tag_name, 4) == 0)
        {
            /* Tag matched, use _arUpgCfg[u4Idx].szTagName.eFlashType and
                         _arUpgCfg[u4Idx].u1PartitionID to do something here. */

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
            if (_arUpgCfg[u4Idx].eFlashType == NAND)
            {
                if (_u1CurrentPartitionId != _arUpgCfg[u4Idx].u1PartitionID)
                {
                    _u1CurrentPartitionId = _arUpgCfg[u4Idx].u1PartitionID;
                    _u4ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
                }
                
                _LdrFlashDynamicDataWrite(e_flag, pauc_buffer, pi4_len);
            }
#else // nor boot
            if (_arUpgCfg[u4Idx].eFlashType == NOR)
            {
                if (_u1CurrentPartitionId != _arUpgCfg[u4Idx].u1PartitionID)
                {
                    _u1CurrentPartitionId = _arUpgCfg[u4Idx].u1PartitionID;
                    _u4ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNorFlashPartOffset0+_u1CurrentPartitionId));
                }
                
                _LdrFlashWrite(e_flag, pauc_buffer, pi4_len);
            }
#endif

            break;
        }
    }

    if (u4Idx < _u4NoOfTags)
    {
        return DLM_OK;
    }
    else
    {
        DEBUG("Tag not found!\n");
        return DLM_OTHERS;
    }
}

#if defined(CC_SECURE_BOOT) && defined(CC_LOADER_VERIFY_PKG)

#define ENCRYPTED_DIGEST_OFFSET   0
#define DIGEST_OFFSET    256
#define COMMAND_OFFSET   256+128

extern INT32 RSADecryption(UINT32 *pu1Signature, UINT32 *pu4PublicKey, UINT32 *pu4CheckSum);

static INT32 _LdrUsigUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    UINT32 u4BufIdx=0;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
    UINT32 au4Key[64]; // 2048bit key
    UINT32 au4Result[64]; // 2048bit key

    if (!pauc_buffer)
    {
        DEBUG("Memory buffer is NULL!\n");
        return DLM_OTHERS;
    }

    // get vendor public key
    x_memcpy((void*)au4Key, (void*)prLdrEnv->au4CustKey, 256);

    // decrypt diagest
    RSADecryption(pauc_buffer+ENCRYPTED_DIGEST_OFFSET, au4Key, au4Result);

    if (x_memcmp((void*)(pauc_buffer+DIGEST_OFFSET), au4Result, 255) == 0)
    {
        if (pauc_buffer[COMMAND_OFFSET] == USIG_DO_UPGRADE)
        {
            _u4UsigCmd = USIG_DO_UPGRADE;
        }
        else if (pauc_buffer[COMMAND_OFFSET] == USIG_RUN_ON_USB)
        {
            _u4UsigCmd = USIG_RUN_ON_USB;
            x_strncpy(prLdrEnv->szUenv, &pauc_buffer[COMMAND_OFFSET+2], 125);
            prLdrEnv->u4CfgFlag = USIG_RUN_ON_USB;
        }
        
        Printf("_LdrUsigUpgrade, verify key pass, command=\'%c\'(0x%x)\n",
            pauc_buffer[COMMAND_OFFSET], pauc_buffer[COMMAND_OFFSET]);
    }
    else
    {
        _u4UsigCmd = USIG_VERIFY_FAIL;
        Printf("_LdrUsigUpgrade, verify key fail\n");
    }

    return DLM_OK;
}
#endif

static INT32 _LdrCfigUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    CHAR *szBuf, szTmp[8];
    INT32 i4Size = * ui4_len;
    UINT32 u4BufIdx = 0, u4TmpIdx;

    if (!pauc_buffer)
    {
        DEBUG("Memory buffer is NULL!\n");
        return DLM_OTHERS;
    }

    szBuf = (CHAR *)pauc_buffer;

    while(0 != (x_strncmp(&szBuf[u4BufIdx], "START", 5)))
    {
        if (u4BufIdx >= i4Size-5)
        {
            DEBUG("START not found at cfig\n");
            return DLM_OTHERS;
        }
        
        u4BufIdx++;
    }
    u4BufIdx+= 6;

    for ( ; u4BufIdx < i4Size; _u4NoOfTags++)
    {
        if (_u4NoOfTags >= MAX_UPG_CFG_NUM)
        {
            Printf("warning! cfg partition number is over limitation(=%d)!\n", MAX_UPG_CFG_NUM);
            break;
        }

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }
        
        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';

        x_strncpy(_arUpgCfg[_u4NoOfTags].szTagName, szTmp, 4);
        _arUpgCfg[_u4NoOfTags].szTagName[4] = '\0';

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }

        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';
        if (x_strcmp(szTmp, "nand") == 0)
        {
            _arUpgCfg[_u4NoOfTags].eFlashType = NAND;
        }
        else if (x_strcmp(szTmp, "nor") == 0)
        {
            _arUpgCfg[_u4NoOfTags].eFlashType = NOR;
        }
        else
        {
            DEBUG("FlashType not support\n");
            //return DLM_OTHERS;
        }

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }

        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';
        extern UINT32 StrToInt(const CHAR*);
        _arUpgCfg[_u4NoOfTags].u1PartitionID = (UINT8)StrToInt(szTmp);

        if (x_strncmp(&szBuf[u4BufIdx], "END", 3) == 0)
        {
            _u4NoOfTags++;
            break;
        }
    }
    
    return DLM_OK;
}

static void _LdrDlmTagVerChkFn(CHAR* szFileNameWithPath)
{
#ifdef LOADER_USB_UPGRADE

    PFN_DLM_GET_TAG_VER_CB_FUNC pfnGetVerCb = NULL;
    PFN_DLM_CMP_TAG_VER_CB_FUNC pfnCmpVerCb = NULL;
    Append_T* prAppendTag = NULL;
    CHAR szTagData[64];
    CHAR szUpgVer[32];
    CHAR szCurVer[32];
    UCHAR* szTag;
    UCHAR a_szTag[5];
    UINT32 u4VerOff, u4VerLen;

#if 0
    UTIN32 u4TagId = 0;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
#endif

    pfnGetVerCb = (PFN_DLM_GET_TAG_VER_CB_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmGetTagVerCbFunc);
    pfnCmpVerCb = (PFN_DLM_CMP_TAG_VER_CB_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmCmpTagVerCbFunc);
    u4VerOff = DRVCUST_InitGet(eLoaderUpgradeDlmVerDataOff);
    u4VerLen = DRVCUST_InitGet(eLoaderUpgradeDlmVerDataLen);

    // 0.check if there are callback functions, return this function if not define
    if ((pfnGetVerCb == NULL) || (pfnCmpVerCb == NULL))
    {
        return;
    }

    // 0.check if size of local variable is enough
    if (u4VerOff < 4) //must be after 4 bytes tag, e.g. "iMtk"
    {
        ASSERT(0);
        return;
    }
    
    if ((u4VerOff + u4VerLen) > sizeof(szTagData))
    {
        ASSERT(0);
        return;
    }
    
    if (u4VerLen > sizeof(szUpgVer))
    {
        ASSERT(0);
        return;
    }

#if 0
    // 0.reset loader env.
    x_memset((void*)prLdrEnv->arTagVer, 0, sizeof(prLdrEnv->arTagVer));
#endif

    // 1.get all tags from x_dlm_get_codefile_tag_list()
    prAppendTag = x_dlm_get_codefile_tag_list(szFileNameWithPath);

    // 2.for each tag
    while (prAppendTag != NULL)
    {
        szTag = prAppendTag->uc_type;

        x_memcpy((void*)a_szTag, (void*)szTag, 4);
        a_szTag[4] = 0;

        // 2.1 get PKG version by x_dlm_tag_test()
        x_dlm_tag_test(szFileNameWithPath, szTag, NULL, szTagData, sizeof(szTagData)) == DLM_OK;
        Printf("a_szTag=%s, szTagData=%s\n", a_szTag, szTagData);

        // 2.2 check if it starts with a DLM_APPEND_HDR_MAGIC, i.e."iMtk"
        if (x_strncmp(szTagData, "iMtK", 4) == 0)
        {
            x_memset(szUpgVer, 0, sizeof(szUpgVer));
            x_memset(szCurVer, 0, sizeof(szCurVer));

            u4VerLen = *(UINT32*)&szTagData[u4VerOff - 4];
            x_strncpy(szUpgVer, &szTagData[u4VerOff], u4VerLen);
            Printf("szUpgVer=%s", szUpgVer);

            // 2.2.1 get current version by (PFN_GET_TAG_VER_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmGetTagVersionCbFunc)
            pfnGetVerCb(a_szTag, szCurVer);
            Printf(", szCurVer=%s\n", szCurVer);

            // 2.2.2 save the current version to loader env.
#if 0
            if (u4TagId < CC_LDR_ENV_TAG_NUM)
            {
                x_strncpy(prLdrEnv->arTagVer[u4TagId].szTag, (CHAR*)szTag, 4);
                x_strncpy(prLdrEnv->arTagVer[u4TagId].szVersion, szCurVer, u4VerLen);
                u4TagId++;
            }
            else
            {
                ASSERT(0);
                return;
            }
#endif

            // 2.2.3 compare version by (PFN_CMP_TAG_VER_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmCmpTagVersionCbFunc)
            if (pfnCmpVerCb(a_szTag, szCurVer, szUpgVer) == 0)
            {
                Printf("->upgrade NOT ALLOW\n", szUpgVer);
                // 2.2.3.1 if not allow, call x_dlm_unreg_append_callback()
                x_dlm_unreg_append_callback(szTag);
            }
            else
            {
                Printf("->upgrade ALLOW\n", szUpgVer);
            }
        }

        prAppendTag = prAppendTag->pt_next;
    }
#endif
}

static INT32 _LdrDlmVerChkFn(char *ps_version)
{
    IMAGE_UPGRADE_HEADER_T *prImgUpHdr = NULL;
    UINT32 u4Checksum;
    UINT32 *pu4Val;
    PFN_VER_CMP_FUNC pfnVerCmp = NULL;

    Printf("Download firmware version - %s\n", ps_version);

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
    UNUSED(prImgUpHdr);
    UNUSED(u4Checksum);
    UNUSED(pu4Val);
#else

    prImgUpHdr = (IMAGE_UPGRADE_HEADER_T *)(PBI_A_BASE + FIRMWARE_FLASH_OFFSET);
    // How to check current version?
    // 1. If there is no 539P header in norflash, current version is NULL, always return DLM_OK.
    if (prImgUpHdr->u4Signature != IMAGE_SIGNATURE_UPGRADE)
    {
        return DLM_OK;  // zero to allow upgrade
    }

    // 2. check image checksum, if checksum is not equal to 5382P header.u4FirmwareChksum, always return DLM_OK.
    u4Checksum = 0;
    pu4Val = (UINT32 *)(PBI_A_BASE + FIRMWARE_FLASH_OFFSET + sizeof(IMAGE_UPGRADE_HEADER_T));
    while ((((UINT32)pu4Val) - (PBI_A_BASE + FIRMWARE_FLASH_OFFSET + sizeof(IMAGE_UPGRADE_HEADER_T))) < prImgUpHdr->u4Size)
    {
        u4Checksum += *pu4Val;
        pu4Val++;
    }

    if (u4Checksum != prImgUpHdr->u4FirmwareChksum)
    {
        Printf("Image Checksum is incorrect chk:0x%08x firm:0x%08x\n", u4Checksum, prImgUpHdr->u4FirmwareChksum);
        return DLM_OK;  // zero to allow upgrade
    }
#endif

    pfnVerCmp = (PFN_VER_CMP_FUNC)DRVCUST_InitGet(eLoaderUpgradeVerCmpFunc);
    if (pfnVerCmp == NULL)
    {
        return DLM_OK;  // zero to allow upgrade
    }

    // 3. There is a version string in 539P header.au1Version,
    //    pass this version string and ps_version to customization version compare function.
    //    return the result of the version compare function.
    //    zero to allow upgrade, otherwise to reject upgrade.
    return pfnVerCmp((char *)prImgUpHdr->au1Version, ps_version);
}

VOID _LdrDlmInstallCallBack(UINT32 u4Upgrade)
{
    PFN_DLM_CB_FUNC pfnDlmCb = NULL;

    // Set version compare callback function.
    x_dlm_reg_version_compare_callback((dlm_ver_chk_fn)_LdrDlmVerChkFn);

    // Check upgrade bit.
    if (u4Upgrade & PC_UPGRADE_CTRL_NOR_1)
    {
#if defined(CC_NAND_BOOT) // nand boot
        INT32 i4NandBlkSize;

        i4NandBlkSize = Loader_GetNandFlashBlockSize();
        if(i4NandBlkSize <= 0)
        {
            _u4BufSize = 16*1024;
        }
        else
        {
            _u4BufSize = (UINT32)i4NandBlkSize;
        }

        Printf("NAND Erase Unit:0x%x\n", _u4BufSize);
        
#elif defined(CC_EMMC_BOOT) // emmc boot

        INT32 i4MsdcEraseSize;

        i4MsdcEraseSize = Loader_GetMsdcEraseSize();
        if(i4MsdcEraseSize <= 0)
        {
            _u4BufSize = 128*1024;
        }
        else
        {
            _u4BufSize = (UINT32)i4MsdcEraseSize;
        }

        Printf("Msdc Erase Unit:0x%x\n", _u4BufSize);
        
#else // nor boot

        _u4BufSize = 64*1024;
        Printf("Nor Erase Unit:0x%x\n", _u4BufSize);
#endif

        x_dlm_reg_append_callback("all",  _u4BufSize, _LdrAllUpgrade);
        x_dlm_reg_append_callback("firm", _u4BufSize, _LdrFirmUpgrade);
        x_dlm_reg_append_callback("fir2", _u4BufSize, _LdrFir2Upgrade);
        x_dlm_reg_append_callback("micp", _u4BufSize, _LdrMicpUpgrade);
        x_dlm_reg_append_callback("nor1", _u4BufSize, _LdrFirmUpgrade);
        x_dlm_reg_append_callback("8281", _u4BufSize, _Ldr8281Upgrade);
        x_dlm_reg_append_callback("load", _u4BufSize, _LdrLoaderUpgrade);

        pfnDlmCb = (PFN_DLM_CB_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmCbFunc);
        if (pfnDlmCb != NULL)
        {
            pfnDlmCb(_u4BufSize);
        }
    }
}

static INT32 _LdrSetUpgradeState(UINT8 u1Ch)
{
#ifdef CC_CUST_USB_BREAK_HANDLE_SUPPORT
    UINT32 u4UpgradeGoAddr;
    UINT8  u1UpgradeState = 0;

    if (DRVCUST_InitQuery(eLoaderUpgradeBreakStateOffset, &u4UpgradeGoAddr) == 0)
    {
        UNUSED(EEPROM_Read(u4UpgradeGoAddr, (UINT32)(void *)&u1UpgradeState, sizeof(UINT8)));

        if (u1Ch == 0)
        {
            u1UpgradeState &= (UINT8)(~(DTVCFG_FLAG_USB_UPGRADE_ON));
        }
        else if (u1Ch == 1)
        {
            u1UpgradeState |= (DTVCFG_FLAG_USB_UPGRADE_ON);
        }

        UNUSED(EEPROM_Write(u4UpgradeGoAddr, (UINT32)(void *)&u1UpgradeState, sizeof(UINT8)));
    }
#else
    UNUSED(u1Ch);
#endif /* CC_CUST_USB_BREAK_HANDLE_SUPPORT */

    return 0;
}

static INT32 _LdrSetT8032Cmd(UINT8 u1Cmd, UINT8 u1SubCmd)
{
#ifndef CC_MT8223
    #if defined(CC_MTK_LOADER)
        PDWNC_T8032_CMD_T rCmd;

        rCmd.u1Cmd = u1Cmd;
        rCmd.u1SubCmd = u1SubCmd;

        return PDWNC_T8032Cmd(&rCmd, NULL);
    #else
        return 0;
    #endif
    
#else

    UNUSED(u1Cmd);
    UNUSED(u1SubCmd);
    return 0;
#endif
}

static INT32 _LdrDlmHandler(UINT32 u4Upgrade)
{
    PFN_GPIO_LEDCTRL_FUNC pfnLedCtrlFunc = NULL;
    DEFAULT_CODE_FILE_HDR_T t_code_hdr;
    char szFileNameWithPath[96];
    UINT32 u4Idx = 0;
    INT32 i4Ret;

    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
    x_memcpy(t_code_hdr.uac_id, (void *)(prLdrEnv->u.rHdr.szCustomer), 4);
    x_memcpy(t_code_hdr.s_model, (void *)(prLdrEnv->u.rHdr.szModel), 32);
    x_strncpy((char *)t_code_hdr.s_version, (void *)(prLdrEnv->u.rHdr.szVersion), 64);

    x_strcpy(szFileNameWithPath, USB_ROOT_PATH);
    x_strcat(szFileNameWithPath, "/");
	
	if(_TryUpgradeAutoPkg==0)
	{
    x_strcat(szFileNameWithPath, _szFileName);
	}
    else
    {
    x_strcat(szFileNameWithPath, "autotest_upgrade_full.pkg");
    }
	
    /* register dlm callback API */
    _LdrDlmInstallCallBack(u4Upgrade);

    x_dlm_set_buffer(0x800000);
    x_dlm_boot_init((CHAR*)&t_code_hdr);

    _LdrSetUpgradeState(1);
    _LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 1);
    UNUSED(DRVCUST_InitQuery(eGpioLedCtrlFunc, (UINT32 *)(void *)&pfnLedCtrlFunc));
    if (pfnLedCtrlFunc != NULL)
    {
        pfnLedCtrlFunc(eLedDetectBlink);
    }

    if ((x_dlm_integrity_check((CHAR*)szFileNameWithPath, NULL)) == DLM_OK)
    {
        if (pfnLedCtrlFunc != NULL)
        {
            pfnLedCtrlFunc(eLedUpgradeBlink);
        }
        Printf("Do loader usb upgrade....");

#if defined(CC_SECURE_BOOT) && defined(CC_LOADER_VERIFY_PKG)
        if (BIM_IS_SECURE_BOOT)
        {
        prLdrEnv->u4CfgFlag = USIG_VERIFY_FAIL;
        x_dlm_reg_append_callback("usig", 512, _LdrUsigUpgrade);
        x_dlm_download_by_tag(szFileNameWithPath, "usig", NULL);
        x_dlm_unreg_append_callback("usig");

        if (_u4UsigCmd != USIG_DO_UPGRADE)
        {
                Printf("secure upgrade stop, usigcmd=\'%c\'(0x%x)\n", _u4UsigCmd, _u4UsigCmd);
            return 1;
        }
        }
#endif
        x_dlm_reg_append_callback("cfig", 256, _LdrCfigUpgrade);
        x_dlm_download_by_tag(szFileNameWithPath, "cfig", NULL);
        x_dlm_unreg_append_callback("cfig");

        for (u4Idx = 0; u4Idx < _u4NoOfTags; u4Idx++)
        {
            Printf("Dynamic Tag No: %d\n", u4Idx);
            Printf("\tTagName: %s\n", _arUpgCfg[u4Idx].szTagName);
            Printf("\tFlashType: %d\n", _arUpgCfg[u4Idx].eFlashType);
            Printf("\tPartitionID: %d\n", _arUpgCfg[u4Idx].u1PartitionID);

            i4Ret = x_dlm_reg_append_callback2((UCHAR *)_arUpgCfg[u4Idx].szTagName, _u4BufSize, _LdrDynamicUpgrade);
            if (i4Ret != DLM_OK)
            {
                DEBUG("Warning: x_dlm_reg_append_callback2 return %d\n", i4Ret);
            }
        }

        // version control of each tag
        _LdrDlmTagVerChkFn((CHAR*)szFileNameWithPath);

        // trigger loader partition first
        if (x_dlm_download_by_tag(szFileNameWithPath, "load", NULL) == DLM_OK)
        {
            x_dlm_unreg_append_callback("load");
       //     _LdrLoaderUpgradeReboot();
        }

        if ((x_dlm_download((CHAR*)szFileNameWithPath, NULL, 0)) != DLM_OK)
        {
            if (pfnLedCtrlFunc != NULL)
            {
                pfnLedCtrlFunc(eLedUpgradeFail);
            }

            Printf("Fail!\n");
            _LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
            return 1;
        }

        if (pfnLedCtrlFunc != NULL)
        {
            pfnLedCtrlFunc(eLedUpgradeFinish);
        }
        
        Printf("Success!\n");
        _u4Upgraded++;
    }
    else
    {
        Printf("No proper firmware exist, ");
        if (pfnLedCtrlFunc != NULL)
        {
            pfnLedCtrlFunc(eLedUpgradeFail);
        }
        
        _LdrSetUpgradeState(0);
        _LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
        return 1;
    }

    _LdrSetUpgradeState(0);
    _LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
    
    return 0;
}

static INT32 _LdrUsbFileReadKernel()
{
   char szkernelFileWithPath[96];
    HANDLE_T rImgFile = NULL;
    FM_FILE_INFO_T  rFileInfo;
    HANDLE_T    h_dir;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    BOOL fgExactTarget = FALSE;     
    CHAR* szUsbKernel="usb_kernel" ;

    if (x_fm_open_dir(FM_ROOT_HANDLE, USB_ROOT_PATH, &h_dir) != FMR_OK)
    {
        Printf("Open USB root directory fail, ");
        return 1;
    }
    
   x_memset(szkernelFileWithPath, 0, sizeof(szkernelFileWithPath));
    // list all files
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {
       
        //Printf("[_LdrUsbFileRead]==sxk===>%s \n", at_dir_entry[0].s_name);
        if (x_strcmp(at_dir_entry[0].s_name, szUsbKernel) == 0)
        {
            fgExactTarget = TRUE;
            break;
        } 
        
    }

    if (fgExactTarget)
    {        
       Printf("===>kernel image file %s was found\n", szUsbKernel);        
    } 
    else
   {
    Printf("===>kernel image file %s was NOT found !!!\n", szUsbKernel);   
   return 1;
   }
    /*------------------Copy kernel image from usb--------------------------------------*/

if (1==_UsbKernelEN)
{ 
  
   x_strcpy(szkernelFileWithPath, USB_ROOT_PATH);
   x_strcat(szkernelFileWithPath, "/");  
   x_strcat(szkernelFileWithPath, szUsbKernel);
	
	if (x_fm_open(FM_ROOT_HANDLE, szkernelFileWithPath, FM_READ_ONLY, 0777, FALSE, &rImgFile) != FMR_OK)
    {
        Printf("\nError: Open file %s fail, ", szUsbKernel);
        return 1;
    }

    // Get file info
    if(x_fm_get_info_by_handle(rImgFile, &rFileInfo) != FMR_OK)
    {
        Printf("\nError: Get file %s info fail, ", szUsbKernel);
        return 1;
    }
    _u4FileSize = (UINT32)rFileInfo.ui8_size;

    // Print file info
    Printf("FileName:(%s)  File size: %d\n", szUsbKernel,_u4FileSize);
   
    if (_u4FileSize > MAX_UPGRADE_IMAGE_FILE_SIZE)
    {
        Printf("\nError: File %s is over the maximum allowed size, ", szUsbKernel);
        return 1;
    }	


    if( (_pucFileBuf = x_mem_alloc(_u4FileSize)) == NULL)
    {
        Printf("\nError: Buffer allocated fail, ");
        return 1;
    }

    // Read file into buffer
    UINT32 u4ReadFileSize = 0;
    UCHAR* pu1Buf = _pucFileBuf;
    UINT32 u4Read = 0;
    INT32 i4_ret;

    while(u4ReadFileSize != _u4FileSize)
    {
        if ((i4_ret = x_fm_read(rImgFile, pu1Buf, BYTES_PER_READ, &u4Read)) != FMR_OK)
        {
            Printf("\nError: Read file %s fail, ", _szFileName);
            Printf("\nx_fm_read return %d\n", i4_ret);
            Printf("Read File Size: %d Bytes\n", u4ReadFileSize);
            x_fm_close(rImgFile);
            x_fm_close(h_dir);
            x_mem_free(_pucFileBuf);
            pu1Buf = NULL;
            return 1;
        }

        u4ReadFileSize += u4Read;
        pu1Buf += u4Read;
    }
    
    Printf("\n====>Read kenrel  File Size: %d Bytes\n", u4ReadFileSize);
    u4UsbKernelSize=u4ReadFileSize ;

x_fm_close(rImgFile);
x_fm_close(h_dir);
}    
    return 0;
}

static INT32 _LdrUsbFileReadAutoPkg()
{
   char szAutoPkgWithPath[96];
    HANDLE_T rImgFile = NULL;
    FM_FILE_INFO_T  rFileInfo;
    HANDLE_T    h_dir;
    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    BOOL fgExactTarget = FALSE;     
    CHAR* szUsbAutoPkgName="autotest_upgrade_full.pkg" ;
	UINT32 u4AutoPkgSize = 0;

    if (x_fm_open_dir(FM_ROOT_HANDLE, USB_ROOT_PATH, &h_dir) != FMR_OK)
    {
        Printf("Open USB root directory fail, ");
        return 1;
    }
    
   x_memset(szAutoPkgWithPath, 0, sizeof(szAutoPkgWithPath));
    // list all files
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {
       
        //Printf("[_LdrUsbFileRead]==sxk===>%s \n", at_dir_entry[0].s_name);
        if (x_strcmp(at_dir_entry[0].s_name, szUsbAutoPkgName) == 0)
        {
            fgExactTarget = TRUE;
            break;
        } 
        
    }

    if (fgExactTarget)
    {        
       Printf("===>auto test full pkg : %s was found\n", szUsbAutoPkgName);        
    } 
    else
   {
    Printf("===>auto test full pkg: %s was NOT found !!!\n", szUsbAutoPkgName);   
   return 1;
   }

   x_strcpy(szAutoPkgWithPath, USB_ROOT_PATH);
    x_strcat(szAutoPkgWithPath, "/");
    x_strcat(szAutoPkgWithPath, szUsbAutoPkgName);

    // Open upgrade image file
    if (x_fm_open(FM_ROOT_HANDLE, szAutoPkgWithPath, FM_READ_ONLY, 0777, FALSE, &rImgFile) != FMR_OK)
    {
        Printf("\nError: Open file %s fail, ", szUsbAutoPkgName);
        return 1;
    }

    // Get file info
    if(x_fm_get_info_by_handle(rImgFile, &rFileInfo) != FMR_OK)
    {
        Printf("\nError: Get file %s info fail, ", szUsbAutoPkgName);
        return 1;
    }
    u4AutoPkgSize = (UINT32)rFileInfo.ui8_size;

    // Print file info
    Printf("FileName:(%s)  File size: %d\n", szUsbAutoPkgName,u4AutoPkgSize);

    if (u4AutoPkgSize > MAX_UPGRADE_IMAGE_FILE_SIZE)
    {
        Printf("\nError: File %s is over the maximum allowed size, ", szUsbAutoPkgName);
        return 1;
    }

    x_fm_close(rImgFile);
    x_fm_close(h_dir);     
    return 0;
}

static INT32 _LdrUsbFileRead()
{
    char szFileNameWithPath[96];
    HANDLE_T rImgFile = NULL;
    FM_FILE_INFO_T  rFileInfo;
    HANDLE_T    h_dir;

    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    BOOL fgExactTarget = FALSE;
    UINT32 u4FuzzyTargetCount = 0;
    CHAR szFuzzyFileName[64];
    CHAR* szPrefix = "";
    CHAR* szSuffix = "";

    if (x_fm_open_dir(FM_ROOT_HANDLE, USB_ROOT_PATH, &h_dir) != FMR_OK)
    {
        Printf("Open USB root directory fail, ");
        return 1;
    }

    x_memset(szFileNameWithPath, 0, sizeof(szFileNameWithPath));
    x_memset(szFuzzyFileName, 0, sizeof(szFuzzyFileName));
#if 0 //!define(CC_LOADER_SHRINK_QUERY)//loader shrink
    if (DRVCUST_InitGet(eUsbUpgradeFileNamePrefix) != NULL)
    {
        szPrefix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNamePrefix);
    }

    if (DRVCUST_InitGet(eUsbUpgradeFileNameSuffix) != NULL)
    {
        szSuffix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNameSuffix);
    }
#else
    szPrefix = (CHAR *)"upgrade";
    szSuffix = (CHAR *)".pkg";
#endif
    // list all files
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {
        UINT32 len = x_strlen(at_dir_entry[0].s_name);
        //Printf("[_LdrUsbFileRead]%s\n", at_dir_entry[0].s_name);
        if (x_strcmp(at_dir_entry[0].s_name, _szFileName) == 0)
        {
            fgExactTarget = TRUE;
            break;
        }

        if (len < (x_strlen(szPrefix) + x_strlen(szSuffix)))
        {
            continue;
        }

        len -= x_strlen(szSuffix);

        // if filename starts with "upgrade" and ends with ".pkg"
        if ((x_strncmp(at_dir_entry[0].s_name, szPrefix, x_strlen(szPrefix)) == 0) &&
            (x_strcmp(&at_dir_entry[0].s_name[len], szSuffix) == 0))
        {
            if (u4FuzzyTargetCount == 0)
            {
                x_strncpy(szFuzzyFileName, at_dir_entry[0].s_name, x_strlen(at_dir_entry[0].s_name));
            }
            
            u4FuzzyTargetCount++;
        }
    }

    if (!fgExactTarget)
    {
        if (u4FuzzyTargetCount == 0)
        {
            x_fm_close(h_dir);
            Printf("Error: No %sxxx%s was found\n", szPrefix, szSuffix);
            return 1;
        }
        else if (u4FuzzyTargetCount > 1)
        {
            x_fm_close(h_dir);
            Printf("Error: Multiple %sxxx%s were found\n", szPrefix, szSuffix);
            return 1;
        }
        else
        {
            x_strncpy(_szFileName, szFuzzyFileName, x_strlen(szFuzzyFileName));
            Printf("upgrade file %s was found\n", _szFileName);
        }
    }
    // end

    x_strcpy(szFileNameWithPath, USB_ROOT_PATH);
    x_strcat(szFileNameWithPath, "/");
    x_strcat(szFileNameWithPath, _szFileName);

    // Open upgrade image file
    if (x_fm_open(FM_ROOT_HANDLE, szFileNameWithPath, FM_READ_ONLY, 0777, FALSE, &rImgFile) != FMR_OK)
    {
        Printf("\nError: Open file %s fail, ", _szFileName);
        return 1;
    }

    // Get file info
    if(x_fm_get_info_by_handle(rImgFile, &rFileInfo) != FMR_OK)
    {
        Printf("\nError: Get file %s info fail, ", _szFileName);
        return 1;
    }
    _u4FileSize = (UINT32)rFileInfo.ui8_size;

    // Print file info
    Printf("FileName:(%s)  File size: %d\n", _szFileName,_u4FileSize);

    if (_u4FileSize > MAX_UPGRADE_IMAGE_FILE_SIZE)
    {
        Printf("\nError: File %s is over the maximum allowed size, ", _szFileName);
        return 1;
    }

    x_fm_close(rImgFile);
    x_fm_close(h_dir);
    return 0;
}

static INT32 _LdrUsbMount(void)
{
    INT32 i4_ret;
    UINT32 ui4_part_ns, ui4_cnt;
    VOID*          pv_mem_addr;
    SIZE_T         z_mem_size;

    /*  1. dev point creation */
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/dev", 0666);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/dev is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/dev is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /dev fail\n");
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  2. mount point creation */
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt", 0666);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/mnt is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/mnt is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /mnt fail\n");
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  3. make entry for USB */
    i4_ret = x_fm_make_entry(FM_ROOT_HANDLE,
                             "/dev/usb",
                             FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0660,
                             FM_DRVT_USB_MASS_STORAGE,
                             0);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/dev/usb is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/dev/usb is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /dev/usb fail\n");
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  4. init handle library  */
#define MAX_NUM_HANDLES     1024
#define HANDLE_MEM_SIZE     64*1024

    pv_mem_addr = x_mem_calloc(1, HANDLE_MEM_SIZE);
    z_mem_size = HANDLE_MEM_SIZE;

    i4_ret = handle_init(MAX_NUM_HANDLES,
                              &pv_mem_addr,
                              &z_mem_size);

    if (i4_ret != FMR_OK)
    {
        DEBUG("init handle library error.");
        DEBUG("return value:%d\n", i4_ret);
        goto func_exit;
    }


    /*  5. parse drive  */
    i4_ret = x_fm_parse_drive(FM_ROOT_HANDLE,
                              "/dev/usb",
                              (VOID *) NULL);

    if (i4_ret != FMR_OK)
    {
        DEBUG("parse drive error.");
        DEBUG("return value:%d\n", i4_ret);
        goto func_exit;
    }

    /*  6. get partition ns */
    i4_ret = x_fm_get_part_ns(FM_ROOT_HANDLE,
                              (const char *) "/dev/usb",
                              &ui4_part_ns);

    if (i4_ret != FMR_OK)
    {
        DEBUG("get partition ns fail.\n");
        DEBUG("return value:%d\n", i4_ret);
        goto func_exit;
    }

    for (ui4_cnt = 0; ui4_cnt < ui4_part_ns; ui4_cnt++)
    {
        FM_PART_INFO_T t_part_info;

        char ps_dev_name[40];
        char ps_mnt_name[40];

        i4_ret = x_fm_get_part_info(FM_ROOT_HANDLE,
                                    (const char *) "/dev/usb",
                                    ui4_cnt,
                                    &t_part_info);

        if (i4_ret != FMR_OK)
        {
            DEBUG("x_fm_get_part_info fails.\n");
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }

        x_strcpy((char *) ps_dev_name, "/dev/");
        x_strcpy((char *) ps_mnt_name, "/mnt/");

        x_strcat((char *) ps_dev_name, (const char *) t_part_info.ps_part_name);
        x_strcat((char *) ps_mnt_name, (const char *) t_part_info.ps_part_name);

        i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_mnt_name, 0666);

        if (i4_ret == FMR_OK)
        {
            DEBUG("%s is created.\n", ps_mnt_name);
        }
        else
        {
            if (i4_ret == FMR_EXIST)
            {
                DEBUG("%s is already existed.  That is OK.\n", ps_mnt_name);
            }
            else
            {
                DEBUG("make %s fail.\n", ps_mnt_name);
                DEBUG("return value:%d\n", i4_ret);
                goto func_exit;
            }
        }

        i4_ret = x_fm_mount(FM_ROOT_HANDLE, ps_dev_name, FM_ROOT_HANDLE, ps_mnt_name);

        if (i4_ret == FMR_OK)
        {
            DEBUG("%s is mounted as %s.\n", ps_dev_name, ps_mnt_name);
        }
        else
        {
            DEBUG("ERR: mount %s as %s fail.\n", ps_dev_name, ps_mnt_name);
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }
    return 0;

func_exit:

    return 1;
}

extern UINT8 PDWNC_GetT8032IROKPressedStatus(void);
extern void PDWNC_SetT8032IROKPressedHookOrNot(UINT8 bEnable);
extern UINT8 LDR_IsOADUpgradeEnabled(void);
static INT32 _LdrUsbUpgrade(LDR_DATA_T *prLdrData, char *szFileName, UINT32 u4Upgrade)
{
    static UINT8 u1USBBlock[2048];
    HAL_TIME_T t0, t1, dt;
    UINT32 u4Diff;

    vIO32Write4B(PDWNC_WDTCTL, 0);// PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);

    if (_u4Upgraded)
    {
        Printf("\nUSB upgraded\n");
        return 0;
    }

    x_strcpy(_szFileName, szFileName);


     if(1 == LDR_IsOADUpgradeEnabled())
     {
         Printf("OAD upgrading............................................\n");
         PDWNC_SetT8032LEDFastBlinkOn(1);
         goto OAD_ENTRANCE;
     }

     if (_UsbKernelEN==0) //consider normal case boot up time
    {
        if(0 == PDWNC_GetT8032IROKPressedStatus())
        {
            return 1;
        }
    }
    // Check if usb device is inserted.
    if (MUSB_GetInsert() == 0)
    {
        Printf("\nNo USB device.\n");
        return 1;
    }

    HAL_GetTime(&t0);
    Printf("Wait USB medium to be mounted...\n");

    // wait for usb device to be processed by USB driver.
    while (MUSB_GetStatus() != HFI_STATUS_READY)
    {
        MUSB_NoneRunBackground();

        HAL_GetTime(&t1);
        HAL_GetDeltaTime(&dt, &t0, &t1);
        u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;

        // time out = 5000 ms.
        if ((u4Diff >= 5000) && (MUSB_GetStatus() != HFI_STATUS_READY))
        {
            Printf("\nNo USB Medium on Device.\n");
            return 1;
        }
    }

    HAL_GetTime(&t1);
    HAL_GetDeltaTime(&dt, &t0, &t1);
    u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;
    Printf("USB Medium on Device, Time = %d ms.\n", u4Diff);
    Printf("USB block size = %d bytes.\n", MUSB_GetBlockSize());
    PDWNC_SetT8032LEDFastBlinkOn(1);
    // try to read 1st block of data from USB.
    if (MUSB_Read(0, (UINT32)u1USBBlock, MUSB_GetBlockSize()) != 0)
    {
        Printf("\nError: USB Read fail, ");
        return 1;
    }

    if (fm_init() != FMR_OK)
    {
        Printf("\nError: Init FM fail, ");
        return 1;
    }

    if (x_fm_fat_init(NULL) != FMR_OK)
    {
        Printf("\nError: Init FAT fail, ");
        return 1;
    }

    if (_LdrUsbMount())
    {
        Printf("\nError: USB mount fail, ");
        return 1;
    }

if (0==_UsbKernelEN)
{
    if (_LdrUsbFileRead())
    {
        Printf("\nError: No valid upgrade file found, ");
        return 1;
    }
}
else
{
     if (_LdrUsbFileReadKernel())
      {
        Printf("\nError: No valid kernel image file in usb found, ");
        _BootNormal=1;		
      }

	 
if(_TryUpgradeAutoPkg==0)
  {
       if(0 == PDWNC_GetT8032IROKPressedStatus())
         {
           return 1;
         }
       else
         {
             if (_LdrUsbFileRead())
            {
             Printf("\nError: No valid upgrade file found, ");
             return 1;
            }
         }
	
   }
else
   {
         if (_LdrUsbFileReadAutoPkg())
            {
             Printf("\nError: No Auto Test upgrade file found, ");
             return 1;
            }
   }

}

OAD_ENTRANCE:

#ifdef CC_FAST_INIT
    prLdrData->rDtvCfg.u1Flags3 |= (UINT8)DTVCFG_FLAG3_NORMAL_BOOT_WITH_SUSPEND;
    prLdrData->rDtvCfg.u1Flags3 &= (UINT8)~DTVCFG_FLAG3_FAST_BOOT;
    prLdrData->rDtvCfg.u1Flags3 &= (UINT8)~DTVCFG_FLAG3_NORMAL_BOOT_WITHOUT_SUSPEND;
    
    if (EEPDTV_SetCfg(&prLdrData->rDtvCfg))
    {
        Printf("\nError: eeprom write fail, ");
        return 1;
    }
#endif

    // To display logo index 1 for start upgrading.
    LDR_LogoDisplay(prLdrData, 1);

    if (_LdrDlmHandler(u4Upgrade))
    {
        // To display logo index 3 for upgrade failed.
        LDR_LogoDisplay(prLdrData, 3);
        return 1;
    }

#ifdef USB_UPGRADE_DEBUG
    if (_pucFileBuf)
    {
        x_mem_free(_pucFileBuf);
        _pucFileBuf = NULL;
    }
#endif

    // To display logo index 4 for upgrade success.
    LDR_LogoDisplay(prLdrData, 4);
    return 0;
}

static INT32 _LdrSetUpgradeBit(LDR_DATA_T *prLdrData, UINT8 u1Ch)
{
    UINT32 i4Ret;

    if (u1Ch == 0)
    {
        prLdrData->rDtvCfg.u1Flags &= (UINT8)(~(DTVCFG_FLAG_USB_UPGRADE_ON));
    }
    else if (u1Ch == 1)
    {
        prLdrData->rDtvCfg.u1Flags |= (DTVCFG_FLAG_USB_UPGRADE_ON);
    }
    
    i4Ret = EEPDTV_SetCfg(&prLdrData->rDtvCfg);
    if (i4Ret)
    {
        printf("eeprom write failed, return %d\n", i4Ret);
    }

    return 0;
}
#endif // CC_USB_DISABLE
#endif // LOADER_USB_UPGRADE


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/*
physical address of OAD pkg.
*/
UINT32 LDR_GetOADPkgPhyAddress(void)
{
    int i;
    UINT32 u4OADPkgPhyAddressOffset = 0;
    UINT32 u4OADPkgPhyAddress = 0;

    
    i = DRVCUST_InitQuery(eOADPkgPhyAddressOffset, (UINT32 *)(void *)&u4OADPkgPhyAddressOffset);
    if(1 ==EEPROM_Read(u4OADPkgPhyAddressOffset, (UINT32)&u4OADPkgPhyAddress, 4))
    {
       // printf("LDR_GetOADPkgPhyAddress: eep error#1\n");
        return 0; 
    }
    
    return u4OADPkgPhyAddress;
}

UINT32 LDR_GetOADPkgSize(void)
{
    UINT32 u4OADPkgSize = 0;
    UINT32 u4OADPkgPhyAddress = 0;

    u4OADPkgPhyAddress = LDR_GetOADPkgPhyAddress();
    
    if(u4OADPkgPhyAddress == 0)
    {
     //   Printf("LDR_GetOADPkgSize is 0 #1.\n");
        return 0;
    }

    u4OADPkgSize = *((UINT32 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SIZE));
    
    return u4OADPkgSize;
}


UINT8 LDR_IsOADUpgradeEnabled(void)
{
    UINT32 u4OADPkgPhyAddress = 0;
    static UINT8 * bLogOnce = 0;
    UINT32 i = 0;
    static UINT8 bPrintOnce = 0;
    u4OADPkgPhyAddress = LDR_GetOADPkgPhyAddress();
    if(u4OADPkgPhyAddress == 0)
    {
//        Printf("OAD upgrading isn't enabled #1.\n");
        return 0;
    }
#if 0//debug print
//    Printf("u4OADPkgPhyAddress=0x%x", u4OADPkgPhyAddress);


    if(bPrintOnce == 0)
    {
        bPrintOnce = 1;
        for(i = 0; i< 200; i+=4)
        {
            Printf("(%d, %d): 0x%x, 0x%x, 0x%x, 0x%x\n", i, i+4,\
                *((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+i)),\
                *((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+i+1)),\
                *((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+i+2)),\
                *((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+i+3)));
        }
    }
#endif
    if((*((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SYMBOL_0)) != 0x12)\
        ||(*((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SYMBOL_1)) != 0x23)\
        ||(*((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SYMBOL_2)) != 0x34)\
        ||(*((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SYMBOL_3)) != 0x45)\
        ||(*((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SYMBOL_4)) != 0x56)\
        ||(*((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SYMBOL_5)) != 0x67)\
        ||(*((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SYMBOL_6)) != 0x78)\
        ||(*((UINT8 *)(u4OADPkgPhyAddress-OFFSET_OAD_ATTRIBUTES_END+OFFSET_OAD_PKG_SYMBOL_7)) != 0x89))
    {
        if(bLogOnce == 0)
        {
            bLogOnce = 1;
         //   Printf("OAD upgrading isn't enabled #2.\n");
        }
        return 0;
    }
    else
    {
        return 1;
    }

}

INT32 LDR_FlashWrite(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len, UINT32 u4ResetStartAddr, UINT32 u4WriteStartAddr)
{
#ifdef LOADER_USB_UPGRADE

#ifndef CC_USB_DISABLE
    if (u4ResetStartAddr)
    {
        _u4ImageStartAddr = u4WriteStartAddr;
    }
    else
    {
        UNUSED(u4WriteStartAddr);
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
#else
    return DLM_OTHERS;
#endif /* CC_USB_DISABLE */

#else
    return DLM_OTHERS;
#endif /* LOADER_USB_UPGRADE */
}

UINT32 LDR_CheckUpgrade(LDR_DATA_T *prLdrData)
{
#ifdef LOADER_USB_UPGRADE
#ifndef CC_USB_DISABLE
#if 0 //def CC_LOADER_UPGRADE_FORCE_DISABLE_SUPPORT
    UINT32 u4UpgradeForceDisableAddr;
    if (DRVCUST_InitQuery(eUsbUpgradeForceDisable, &u4UpgradeForceDisableAddr) == 0)
    {
        UINT8 u1Disable = 0;
        UNUSED(EEPROM_Read(u4UpgradeForceDisableAddr,
                           (UINT32)((void *)&u1Disable), sizeof(UINT8)));
        if (u1Disable == 1)
        {
            // Force disable loader upgrade
            return 0;
        }
    }
    
#endif /* CC_LOADER_UPGRADE_FORCE_DISABLE_SUPPORT */

    // Check DTVCFG_FLAG_USB_UPGRADE_ON for usb upgrade
    if ((DRVCUST_InitGet(eAutoUsbUpgradeEnable)) ||
         ((DRVCUST_InitGet(eEepromUsbUpgradeEnable)) &&
            (prLdrData->rDtvCfg.u1Flags & (DTVCFG_FLAG_USB_UPGRADE_ON))))
    {
        Printf("\nDo USB upgrade\n");
        //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 1);
        HalEnableIRQ();
        
	    //for USB2/3 after mt5396
    	BIM_EnableIrq(VECTOR_MISC);
        if (_LdrUsbUpgrade(prLdrData, (char *)DRVCUST_InitGet(eUsbUpgradeFileName), PC_UPGRADE_CTRL_NOR_1))
        {
            Printf("USB upgrade stop\n");
#if 0
            //HalDisableIRQ();
            if (DRVCUST_InitGet(eAutoUsbUpgradeEnable))
            {
                _LdrSetUpgradeBit(prLdrData, 0);
            }
            else
            {
                _LdrSetUpgradeBit(prLdrData, 1);
            }
#endif         
#ifndef CC_MT8223
            //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
#endif
            return 1;
        }

        // Set to image bank 1 after loader usb upgrade complete
//        prLdrData->rDtvCfg.u1Flags2 &= (UINT8)(~(DTVCFG_FLAG2_IMAGE_BANK));
        //HalDisableIRQ();
        
#ifndef CC_MT8223
        //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
#endif
//        _LdrSetUpgradeBit(prLdrData, 0);
    }
#endif
#endif

    return 0;
}

UINT32 LDR_ForceUpgrade(LDR_DATA_T *prLdrData)
{
#ifdef LOADER_USB_UPGRADE
#ifndef CC_USB_DISABLE
#if 0
    if ((DRVCUST_InitGet(eAutoUsbUpgradeEnable)) ||
         ((DRVCUST_InitGet(eEepromUsbUpgradeEnable)) &&
            (prLdrData->rDtvCfg.u1Flags & (DTVCFG_FLAG_USB_UPGRADE_ON))))
    {
        return 1;
    }
#endif

    Printf("\nForce USB upgrade\n\n");

    // Enable the upgrades setting.
    _LdrSetUpgradeBit(prLdrData, 1);
    //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 1);
    HalEnableIRQ();
    
    //for USB2/3 after mt5396
    BIM_EnableIrq(VECTOR_MISC);
    if (_LdrUsbUpgrade(prLdrData, (char *)DRVCUST_InitGet(eUsbUpgradeFileName), PC_UPGRADE_CTRL_NOR_1))
    {
        Printf("USB upgrade stop\n");
        //HalDisableIRQ();
        //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
        return 1;
    }

    // Set to image bank 1 after loader usb upgrade complete
    prLdrData->rDtvCfg.u1Flags2 &= (UINT8)(~(DTVCFG_FLAG2_IMAGE_BANK));
    //HalDisableIRQ();
    //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
    _LdrSetUpgradeBit(prLdrData, 0);
#endif
#endif

    return 0;
}

