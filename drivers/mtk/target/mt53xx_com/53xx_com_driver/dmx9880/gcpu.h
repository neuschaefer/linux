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
 * $RCSfile: dmx_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gcpu.h
 *  gcpu driver - common definitions
 */

#ifndef GCPU_H
#define GCPU_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "drv_common.h"

#include "gcpu_if.h"
#include "gcpu_debug.h"
#include "x_debug.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_os.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define GCPU_BASE           (AES_BASE)
#define IOMMU_GCPU_BASE     (IO_VIRT + 0x68040)

//
// GCPU Registers
//
#define GCPU_REG_CTL                0 //(0x0)
#define GCPU_REG_MSC                1 //(0x4)

#define GCPU_REG_PC_CTL             256 //(0x400)
#define GCPU_REG_MEM_ADDR           257 //(0x404)
#define GCPU_REG_MEM_DATA           258 //(0x408)

#define GCPU_REG_MONCTL             261 //(0x414)

#define GCPU_REG_DRAM_INST_BASE     264 //(0x420)

#define GCPU_REG_TRAP_START         272 // (0x440)
#define GCPU_REG_TRAP_END           286 // (0x478)

#define GCPU_REG_INT_SET            512 //(0x800)
#define GCPU_REG_INT_CLR            513 //(0x804)
#define GCPU_REG_INT_EN             514 //(0x808)

#define GCPU_REG_MEM_CMD            768 //(0xC00)
#define GCPU_REG_MEM_P0             769 //(0xC04)
#define GCPU_REG_MEM_P1             770 //(0xC08)
#define GCPU_REG_MEM_P2             771 //(0xC0C)
#define GCPU_REG_MEM_P3             772 //(0xC10)
#define GCPU_REG_MEM_P4             773 //(0xC14)
#define GCPU_REG_MEM_P5             774 //(0xC18)
#define GCPU_REG_MEM_P6             775 //(0xC1C)
#define GCPU_REG_MEM_P7             776 //(0xC20)
#define GCPU_REG_MEM_P8             777 //(0xC24)
#define GCPU_REG_MEM_P9             778 //(0xC28)
#define GCPU_REG_MEM_P10            779 //(0xC2C)
#define GCPU_REG_MEM_P11            780 //(0xC30)
#define GCPU_REG_MEM_P12            781 //(0xC34)
#define GCPU_REG_MEM_P13            782 //(0xC38)
#define GCPU_REG_MEM_P14            783 //(0xC3C)
#define GCPU_REG_MEM_Slot           784 //(0xC40)

#define REG_IOMMU_CFG0              0x0//0x000        // basic setting
#define REG_IOMMU_CFG1              0x1//0x004        // page table index
#define REG_IOMMU_CFG2              0x2//0x008        // agnet_0~1 setting
#define REG_IOMMU_CFG4              0x4//0x010        // interrupt, monitor and debug
#define REG_IOMMU_CFG5              0x5//0x010
#define REG_IOMMU_CFG6              0x6//0x010
#define REG_IOMMU_CFG7              0x7//0x010
#define REG_IOMMU_CFG8              0x8//0x010
#define REG_IOMMU_CFG9              0x9//0x010
#define REG_IOMMU_CFGA              0xA//0x010
#define REG_IOMMU_CFGB              0xb//0x010
#define REG_IOMMU_CFGC              0xc//0x010
#define REG_IOMMU_CFGD              0xd//0x010


//
// Interrupt masks
//
#define GCPU_INT_MASK               (0x1)

//
// GCPU command
//
#define CSS_DEC_DK                  (0x00)         // CSS Disk Key Decryption
#define CSS_DEC_TK                  (0x01)         // CSS Title Key Decryption
#define CSS_DSC_AV                  (0x02)         // CSS AV Descramble
#define CSS_AUTH_DRV                (0x03)         // CSS Drive Authentication
#define CSS_AUTH_DEC                (0x04)         // CSS Decoder Authentication
#define CSS_AUTH_BK                 (0x05)         // CSS Key Share Authentication

#define C2_D                        (0x08)         // CPPM/CPRM C2 Decryption
#define C2_E                        (0x09)         // CPPM/CPRM C2 Encryption
#define C2_G                        (0x0A)         // CPPM/CPRM C2 Generating Function
#define C2_H                        (0x0B)         // CPPM/CPRM C2 Hash Function
#define CPPM_DPAK                   (0x0C)         // CPPM Packet Decryption
#define CPRM_DPAK                   (0x0D)         // CPRM Packet Decryption
#define CPRM_EPAK                   (0x0E)         // CPRM Packet Encryption
#define CPRM_DCI_VFY                (0x0F)         // CPRM DCI CCI Verify

#define AES_CTR                     (0x1E)         // AES CTR Function
#define AES_OFB                     (0x1F)         // AES OFB Function
#define AES_D                       (0x20)         // AES Decryption Function
#define AES_E                       (0x21)         // AES Encryption Function
#define AES_G                       (0x22)         // AES Generating Function
#define AES_DPAK                    (0x23)         // AES Packet Decryption
#define AES_EPAK                    (0x24)         // AES Packet Encryption
#define AES_CMAC                    (0x25)         // AES CMAC Algorithm
#define AES_DCBC                    (0x26)         // AES Cipher Block Chaining Decryption
#define AES_ECBC                    (0x27)         // AES Cipher Block Chaining Encryption
#define AES_H                       (0x28)         // AES Hash

#define AES_D_CMP                   (0x36)         // AES Decryption Function with Result Compare
#define AESEK_D                     (0x74)         // AES Decryption with Enc key
#define AESEK_E                     (0x75)         // AES Encryption with Enc key
#define AESPK_EK_D                  (0x76)         // AES Decryption with Predetermined/Encrypted key
#define AESPK_EK_E                  (0x77)         // AES Encryption with Predetermined/Encrypted key
#define AESPK_D                     (0x78)         // AES Decryption with Predetermined Key
#define AESPK_E                     (0x79)         // AES Encryption with Predetermined Key
#define AESPK_DPAK                  (0x7A)         // AES Packet Decryption with Predetermined Key
#define AESPK_EPAK                  (0x7B)         // AES Packet Encryption with Predetermined Key
#define AESPK_DCBC                  (0x7C)         // AES Cipher Block Chaining Decryption with Predetermined Key
#define AESPK_ECBC                  (0x7D)         // AES Cipher Block Chaining Encryption with Predetermined Key
#define AESPK_EK_DCBC               (0x7E)         // AES Cipher Block Chaining Decryption with Predetermined/Encrypted Key

#define VCPS_H                      (0x28)         // VCPS Hash Function
#define VCPS_DPAK                   (0x29)         // VCPS Packet Decryption
#define VCPS_EPAK                   (0x2A)         // VCPS Packet Encryption
#define VCPS_DKBH                   (0x2B)         // VCPS DKB Hash Function
#define VCPS_DHDK                   (0x2C)         // VCPS Hardware Device Key Decryption
#define VCPS_DCBC                   (0x2D)         // VCPS AES Cipher Block Chaining Decryption
#define VCPS_ECBC                   (0x2E)         // VCPS AES Cipher Block Chaining Encryption

#define AACS_DBD                    (0x30)         // AACS Blu-ray AV Packet Decryption
#define AACS_EBD                    (0x31)         // AACS Blu-ray AV Packet Encryption
#define AACS_DTN                    (0x32)         // AACS Blu-ray Thumbnail Packet Decryption
#define AACS_ETN                    (0x33)         // AACS Blu-ray Thumbnail Packet Encryption
#define AACS_DHD                    (0x34)         // AACS HD-DVD Packet Decryption
#define AACS_EHD                    (0x35)         // AACS HD-DVD Packet Encryption
#define AACS_DV_CALC                (0x37)         // AACS DV Calculation

#define TDES_D                      (0x50)         // T-DES Decryption
#define TDES_E                      (0x51)         // T-DES Encryption
#define TDES_DMA_D                  (0x52)         // T-DES DMA Decryption
#define TDES_DMA_E                  (0x53)         // T-DES DMA Encryption
#define TDES_CBC_D                  (0x54)         // T-DES Cipher Block Chaining Decryption
#define TDES_CBC_E                  (0x55)         // T-DES Cipher Block Chaining Encryption

#define BDRE_DBD                    (0x58)         // BDRE AV Packet Decryption
#define BDRE_EBD                    (0x59)         // BDRE AV Packet Encryption
#define BDRE_DTN                    (0x5A)         // BDRE Thumbnail Packet Decryption
#define BDRE_ETN                    (0x5B)         // BDRE Thumbnail Packet Encryption
#define BDRE_BE                     (0x5C)         // BDRE BytePerm and ExtendKey

#define GCPU_WRITE                  (0x6e)
#define GCPU_READ                   (0x6f)

#define MEMCPY                      (0x10)         // DRAM Data Moving
#define DMA                         (0x11)         // DRAM Direct Memory Access
#define SHA_1                       (0x40)         // SHA-1 Algorithm
#define SHA_256                     (0x41)         // SHA-256 Algorithm
#define MD5                         (0x42)         // MD5 Algorithm
#define SHA_224                     (0x43)         // SHA-224 Algorithm
#define ROM_BIST                    (0x5E)
#define RNG                         (0x6B)
#define MEM_XOR                     (0x71)         // Memory XOR
#define PTX                         (0x8c)
#define RC4DPAK                     (0x87)
#define RC4KSA                      (0x88)


#define GCPU_AACS_BDMV_PACKET_SIZE  (6144)
#define GCPU_AACS_BDTHUMB_PACKET_SIZE   (2048)
#define GCPU_AACS_HDVD_PACKET_SIZE  (2048)
#define GCPU_BDRE_PACKET_SIZE       (6144)
#define GCPU_BDRE_THUMB_PACKET_SIZE (2048)
#define GCPU_DES_PACKET_SIZE        (8)
#define GCPU_CSS_PACKET_SIZE        (2048)
#define GCPU_C2H_PACKET_SIZE        (8)
#define GCPU_CPPM_PACKET_SIZE       (2048)
#define GCPU_CPRM_PACKET_SIZE       (2048)
#define GCPU_VCPS_PACKET_SIZE       (2048)
#define GCPU_SHA_1_PACKET_SIZE      (64) //(512bits)
#define GCPU_MD_5_PACKET_SIZE       (64) //(512bits)

#define GCPU_TRAP_NUM               (14)
#define GCPU_INTR_NUM               (1)

#define GCPU_DMEM_SIZE              (48)
#define GCPU_IMEM_RAM_SIZE          (1024)
#define GCPU_IMEM_ROM_SIZE          (4096)

#define GCPU_SECURESLOT_PTR         (48)
#define GCPU_SECURESLOT_NUM         (48)

#define GCPU_PARAM_NUM              (48)
#define GCPU_S_SLOT_NUM             (48)

#define GCPU_PARAM_PTR              (16)
#define GCPU_PARAM_RET_PTR          (32)
#define GCPU_PARAM_RET_NUM          (GCPU_PARAM_NUM - GCPU_PARAM_RET_PTR)

// CPRM
#define GCPU_PARAM_CPRM_KEY_NUM      (2)      //key 2 slots
#define GCPU_PARAM_CPRM_KEY_PTR      (GCPU_PARAM_PTR)
#define GCPU_PARAM_CPRM_DAT_PTR      (GCPU_PARAM_PTR + GCPU_PARAM_CPRM_KEY_NUM)
#define GCPU_PARAM_CPRM_RET_PTR      (GCPU_PARAM_RET_PTR)

// AES
#define GCPU_PARAM_AES_KEY_NUM      (8)      //key 8 slots
#define GCPU_PARAM_AES_MAC_KEY_NUM  (4)      //key 8 slots
#define GCPU_PARAM_AES_IV_NUM       (4)      //iv 4 slots
#define GCPU_PARAM_AES_KEY_PTR      (GCPU_PARAM_PTR)
#define GCPU_PARAM_AES_IV_PTR       (GCPU_PARAM_PTR + GCPU_PARAM_AES_KEY_NUM)
#define GCPU_PARAM_AES_RET_IV_PTR   (GCPU_PARAM_RET_PTR)

// VCPS
#define GCPU_PARAM_VPCS_KEY_NUM      (4)      //key 2 slots
#define GCPU_PARAM_VPCS_KEY_PTR      (GCPU_PARAM_PTR)
#define GCPU_PARAM_VPCS_DAT_PTR      (GCPU_PARAM_PTR + GCPU_PARAM_VPCS_KEY_NUM)
#define GCPU_PARAM_VPCS_RET_PTR      (GCPU_PARAM_RET_PTR)

// AACS
#define GCPU_PARAM_AACS_KEY_NUM      (4)      //key 2 slots
#define GCPU_PARAM_AACS_KEY_PTR      (GCPU_PARAM_PTR)

// BDRE
#define GCPU_PARAM_BDRE_KEY_NUM      (4)      //key 2 slots
#define GCPU_PARAM_BDRE_KEY_PTR      (GCPU_PARAM_PTR)
#define GCPU_PARAM_BDRE_RET_PTR      (GCPU_PARAM_RET_PTR)

// DES
#define GCPU_PARAM_DES_KEY_NUM      (6)      //key 6 slots
#define GCPU_PARAM_DES_IV_NUM       (2)      //iv 2 slots
#define GCPU_PARAM_DES_KEY_PTR      (GCPU_PARAM_PTR)
#define GCPU_PARAM_DES_IV_PTR       (6)
#define GCPU_PARAM_DES_DAT_PTR      (GCPU_PARAM_PTR + GCPU_PARAM_DES_KEY_NUM)
#define GCPU_PARAM_DES_RET_PTR      (GCPU_PARAM_RET_PTR)

// RC4
#define GCPU_PARAM_RC4_KEY_NUM      (2)
#define GCPU_PARAM_S_KEY_NUM        (64)
#define GCPU_PARAM_RC4_KEY_PTR      (16)
#define GCPU_PARAM_RC4_S_PTR        (GCPU_PARAM_RC4_KEY_PTR + GCPU_PARAM_RC4_KEY_NUM)

// IF
#define GCPU_PARAM_IF_PTR           (4)
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// GCPU HW CMD Struct
typedef struct
{
  UINT32 u4Param[GCPU_PARAM_NUM];
//  UINT32 u4NonSecureSlot[GCPU_NS_SLOT_NUM];
//  UINT32 u4NonSecureSlotSetMask;
} GCPU_HW_CMD_T;

// Slot information
typedef struct
{
  BOOL fgUsed;             // used or not
  UINT8 u1Size;            // Slot size, unit: 32bits
} GCPU_SECURE_SLOT_INFO;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
///
/// GCPU register access commands
///
#define GCPUCMD_READ32(offset)			IO_READ32(GCPU_BASE, ((offset) * 4))
#define GCPUCMD_WRITE32(offset, value)	IO_WRITE32(GCPU_BASE, ((offset) * 4), (value))
#define IOMMU_GCPU_READ32(offset)			IO_READ32(IOMMU_GCPU_BASE, ((offset) * 4))
#define IOMMU_GCPU_WRITE32(offset, value)	IO_WRITE32(IOMMU_GCPU_BASE, ((offset) * 4), (value))

#define SwitchValue(u4Value) \
              ((u4Value << 24) & 0xFF000000) + ((u4Value << 8) & 0x00FF0000) + \
              ((u4Value >> 8) & 0x0000FF00) + ((u4Value >> 24) & 0x000000FF)

#define _GCPU_IsInRegion(u4Addr, u4BufStart, u4BufEnd) \
    (((u4Addr >= u4BufStart) && (u4Addr < u4BufEnd))?TRUE:FALSE)

#define ADDR_INCR_IN_RING(addr, incr, ringstart, ringend)      \
    ((((addr) + (incr)) < (ringend)) ? ((addr) + (incr)) : (((addr) + (incr)) - ((ringend) - (ringstart))))

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------
EXTERN void _GCPU_Lock(void);
EXTERN void _GCPU_Unlock(void);
EXTERN UINT32 _GCPU_CopyRingBuffer(UINT32 u4Dst, UINT32 u4DstStart, UINT32 u4DstEnd,
    UINT32 u4Src, UINT32 u4SrcStart, UINT32 u4SrcEnd, UINT32 u4Size);

EXTERN INT32 _GCPU_Init(UINT32 u4Mode);
EXTERN UINT32 _GCPU_AllocSecureSlot(UINT32 u4Size);
EXTERN INT32 _GCPU_FreeSecureSlot(UINT32 u4SlotPtr);
EXTERN INT32 _GCPU_SetSecureSlot(UINT32 u4SlotPtr, UINT8* pu1Data, UINT32 u4Size);
EXTERN INT32 _GCPU_Cmd(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam);

EXTERN INT32 _GCPU_EnableIOMMU(UINT32 u4Src1Start, UINT32 u4Src1End, UINT32 u4Src2Start, UINT32 u4Src2End);
EXTERN INT32 _GCPU_GetRomCodeChecksum(void);
EXTERN INT32 _GCPU_Hw_Init(UINT32 u4Mode);
EXTERN INT32 _GCPU_Hw_SetSlot(UINT8 u1Idx, UINT32 data);
EXTERN INT32 _GCPU_Hw_CmdExe(GCPU_HW_CMD_T *prHwCmd);
EXTERN BOOL _GCPU_AES_SW_Ring(AES_PARAM_T *prLParam);

#endif  // GCPU_H

