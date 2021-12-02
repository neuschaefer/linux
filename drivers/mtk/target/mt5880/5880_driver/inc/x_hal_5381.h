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
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifndef X_HAL_5381_H
#define X_HAL_5381_H

#include "x_typedef.h"
#include "x_hal_io.h"

//===========================================================================
// Constant definitions

#define MEMORY_ALIGNMENT            8

//===========================================================================
// Memory mapping

#define DRAM_B_BASE                 0x0
#define PBI_B_BASE                  0xE0000000
#define IO_BASE                     0xF0000000
#define IO_PHYS                     0xF0000000

#ifdef __KERNEL__
#if defined(CC_UBOOT) || defined(CC_MTK_LOADER)
#define IO_VIRT                     0xF0000000
#else /* CC_UBOOT */
#define IO_VIRT                     0xF0000000
#endif /* CC_UBOOT */
#else
#define IO_VIRT                     0xF0000000
#endif

#define PBI_A_BASE                  0xF8000000
#define DRAM_A_BASE                 0x80000000
#define SRAM_ITCM_BASE              SRAM_LZHS_BASE //itcm=0k
#define SRAM_OSD_BASE               0xFB000000 //osd=16k
#define SRAM_LZHS_BASE              (SRAM_OSD_BASE + 0x4000) //lzhs=4k
#define SRAM_DMX_BASE               (SRAM_LZHS_BASE + 0x1000) //dmx=28k
#define SRAM_DMX_END                (SRAM_DMX_BASE + 0x7000)
#define SRAM_START                  SRAM_OSD_BASE
#define SRAM_END                    SRAM_DMX_END
#define SRAM_CORE1_BASE             (SRAM_END - 0x100)

#ifdef CC_FPGA_SDR64B
    #define FPGA_DRAM_SIZE_MB           128
#else
#define FPGA_DRAM_SIZE_MB           256
#endif

#define IO_ADDR(base)               (IO_VIRT + base)

//===========================================================================
// IO register definitions
#ifndef CC_MT5391_AUD_SUPPORT
#define PARSER0_BASE                (IO_VIRT + 0x00000)
#endif
#define GDMA_BASE                   (IO_VIRT + 0x01000)
#define VLD0_BASE                   (IO_VIRT + 0x02000)
#define DV_BASE                     (IO_VIRT + 0x02200)
#define VLD1_BASE                   (IO_VIRT + 0x03000)     // obsolete
#define GRAPH_BASE                  (IO_VIRT + 0x04000)
#define AUDIO_BASE                  (IO_VIRT + 0x05000)

#ifdef CC_MT5880
#define VGRAPH_BASE                 (IO_VIRT + 0x5c000) 
#define VGRAPH_CMDQUE_BASE          (IO_VIRT + 0x5000) 

#define GRAPH_MMU_BASE              (IO_VIRT + 0x68000)   //graphic mmu
#define GRAPH_MMU_CMD_BASE          (IO_VIRT + 0x4180)   //graphic mmu cmdque mode,  risc 0x68000  = cmdque 0x4180

//IMGRZ_HW_V4
#define IMGRZ_MMU_BASE			 	(IO_VIRT + 0x680c0)
#define IMGRZ_MMU_BASE_2	 	 	(IO_VIRT + 0x68140)
#define JPEG_MMU_BASE			 	(IO_VIRT + 0x68080)
#endif

#ifdef  CC_MT5391_AUD_SUPPORT
#define PARSER0_BASE                (AUDIO_BASE + 0x00700) //added by ling
#define PARSER0_BASE2               (AUDIO_BASE + 0x00600) //added by ling
#endif

#define TV_EOCODER_BASE             (IO_VIRT + 0x06000)     // obsolete
#define DRAM_BASE                   (IO_VIRT + 0x07000)
#define BIM_BASE                    (IO_VIRT + 0x08000)
#define I1394_BASE                  (IO_VIRT + 0x09000)     // obsolete
#define MC0_BASE                    (IO_VIRT + 0x0a000)
#define MC1_BASE                    (IO_VIRT + 0x0b000)     // obsolete
#define RS232_BASE                  (IO_VIRT + 0x0c000)
#define CKGEN_BASE                  (IO_VIRT + 0x0d000)
#define MBIST_BASE                  (IO_VIRT + 0x0e000)
#define DRAM_CHB_BASE               (IO_VIRT + 0x0F000)
#define TCMGPR0_BASE                (IO_VIRT + 0x10000)
#define TCMGPR1_BASE                (IO_VIRT + 0x11000)
#define FLASH_CARD_BASE             (IO_VIRT + 0x12000)
#define SMART_CARD_BASE             (IO_VIRT + 0x13000)
#define JPG_VLD_BASE                (IO_VIRT + 0x14000)     // obsolete
#define JPG_MC_BASE                 (IO_VIRT + 0x15000)     // obsolete
#define PCMCIA_BASE                 (IO_VIRT + 0x1F000)
#define AES_BASE                    (IO_VIRT + 0x16000)
#define SHA1_BASE                   (IO_VIRT + 0x16400)
#define DFAST_BASE                  (IO_VIRT + 0x16800)
#define DEMUX0_BASE                 (IO_VIRT + 0x17000)
#define DEMUX1_BASE                 (IO_VIRT + 0x18000)
#define DEMUX2_BASE                 (IO_VIRT + 0x19000)
#define DEMUX3_BASE                 (IO_VIRT + 0x1a000)
#define DEMUX4_BASE                 (IO_VIRT + 0x1b000)
#define DEMUX5_BASE                 (IO_VIRT + 0x1c000)
#define DEMUX6_BASE                 (IO_VIRT + 0x1d000)
#define PIDSWAP_BASE                (IO_VIRT + 0x1e000)
#define IDE_TEST_BASE               (IO_VIRT + 0x1f000)
#define GFX_SCALAR_BASE             (IO_VIRT + 0x20000)
#define IMGRZ_BASE                  (IO_VIRT + 0x20000)
#define OSD_BASE                    (IO_VIRT + 0x21000)
#define VIDEO_IN0_BASE              (IO_VIRT + 0x22000)
#define VIDEO_IN1_BASE              (IO_VIRT + 0x23000)
#define PSCAN_BASE                  (IO_VIRT + 0x24000)
#define SCPOS_BASE                  (IO_VIRT + 0x25000)
#define BLK2RS_BASE                 (IO_VIRT + 0x26000)
#define POST_PROC_BASE              (IO_VIRT + 0x27000)
#define PDWNC_BASE                  (IO_VIRT + 0x28000)
#define HPDS_BASE                   (IO_VIRT + 0x29000)
#define MUSB_BASE                   (IO_VIRT + 0x50000)
#define MUSB1_BASE                  (IO_VIRT + 0x51000)
#define MUSB2_BASE                  (IO_VIRT + 0x52000)
#define MUSB3_BASE                  (IO_VIRT + 0x53000)
    #define MUSB_COREBASE            (0)  
    #define MUSB_DMABASE             (0x200)
    #define MUSB_PHYBASE             (0x9800)
    #define MUSB_PORT0_PHYBASE       (0)
	#define MUSB_PORT1_PHYBASE       (0x100)
	#define MUSB_PORT2_PHYBASE       (0x200)
	#define MUSB_PORT3_PHYBASE       (0x300)	
#define ETHERNET_BASE               (IO_VIRT + 0x32000)
#define DTCP_BASE                   (IO_VIRT + 0x33000)
#define MJC_BASE                    (IO_VIRT + 0x34000)     // obsolete
#define DRAM_DDRPHY_BASE			(IO_VIRT + 0x58800)
#define DRAM_DDRPHY_CHB_BASE		(IO_VIRT + 0x58C00)
#define ANA_PLL_GROUP_BASE			(IO_VIRT + 0x61000)
#define IMGRZ_BASE2                  (IO_VIRT + 0x6c000)
#define L2C_BASE                    (IO_VIRT + 0x2000000)

//===========================================================================
// Macros for register read

#define IO_READ8(base, offset)      HAL_READ8((base) + (offset))
#define IO_READ16(base, offset)     HAL_READ16((base) + (offset))
#define IO_READ32(base, offset)     HAL_READ32((base) + (offset))
#define IO_READ32MSK(base, offset, msk) \
        ((UINT32)HAL_READ32((base) + (offset)) & (UINT32)(msk))
#define IO_READ32MSK_BOOL(base, offset, msk) \
       ( (IO_READ32MSK((base), (offset), (msk)) > 0)?TRUE:FALSE)

//===========================================================================
// Macros for register write

#define IO_WRITE8(base, offset, value)  HAL_WRITE8((base) + (offset), (value))
#define IO_WRITE16(base, offset, value) HAL_WRITE16((base) + (offset), (value))
#define IO_WRITE32(base, offset, value) HAL_WRITE32((base) + (offset), (value))
#define IO_WRITE32MSK(base, offset, value, msk) \
        IO_WRITE32((base), (offset), \
                   (IO_READ32((base), (offset)) & ~(msk)) | ((value) & (msk)))


//===========================================================================
// Macros for register read/write access

#define IO_REG8(base, offset)       HAL_REG8((base) + (offset))
#define IO_REG16(base, offset)      HAL_REG16((base) + (offset))
#define IO_REG32(base, offset)      HAL_REG32((base) + (offset))

#define IO_ADDR(base)               (IO_VIRT + base)

//===========================================================================
// Macros for address translation

#ifdef CPU_TEST_ENABLE
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5880)
        #define CACHE(addr)                                   ((addr) & 0x7fffffff)
        #define NONCACHE(addr)                                (CACHE(addr) | 0x80000000)
    #elif defined(CC_MT5395) || defined(CC_MT5365)
        #define CACHE(addr)                                   ((addr) & 0x3fffffff)
        #define NONCACHE(addr)                                (CACHE(addr) | 0x40000000)
    #else
        #define CACHE(addr)                                   ((addr) & 0x0fffffff)
        #if defined(CC_MT5363)
            #define NONCACHE(addr)                            (CACHE(addr) | 0x60000000)
        #else
            #define NONCACHE(addr)                            (CACHE(addr) | 0x30000000)
        #endif
    #endif
#endif //CPU_TEST_ENABLE

#ifdef __KERNEL__
#ifdef CC_UBOOT
#define PHYSICAL(addr)              ((addr) & 0x3fffffff)
#define VIRTUAL(addr)               (addr)
#else
extern UINT32 VIRTUAL(UINT32);
extern UINT32 PHYSICAL(UINT32);
#endif
#else
#define PHYSICAL(addr)              ((addr) & 0x3fffffff)
#define VIRTUAL(addr)               (addr)
#endif



//===========================================================================
// Type definitions

#define IC_VER_FPGA         0x00000000U         // FPGA

#define IS_IC_5389()        ((BSP_GetIcVersion() & 0xffff0000U)==0x53890000U)
#define IS_IC_5368()        ((BSP_GetIcVersion() & 0xffff0000U)==0x53680000U)
#define IS_IC_5396()        ((BSP_GetIcVersion() & 0xffff0000U)==0x53960000U)
#define IS_IC_5398()        ((BSP_GetIcVersion() & 0xffff0000U)==0x53980000U)
#define IS_IC_5395()        ((BSP_GetIcVersion() & 0xffff0000U)==0x53950000U)
#define IS_IC_5365()        ((BSP_GetIcVersion() & 0xffff0000U)==0x53650000U)
#define IS_IC_5363()        ((BSP_GetIcVersion() & 0xffff0000U)==0x53630000U)
#define IS_IC_5387()        ((BSP_GetIcVersion() & 0xffff0000U)==0x53870000U)
#define IS_IC_5880()        ((BSP_GetIcVersion() & 0xffff0000U)==0x58800000U)

#define IC_VER_5363_AA      0x53630000         // 5363 AA
#define IC_VER_5363_AB      0x53630001         // 5363 AB

#define IC_VER_5387_AA      0x53870000         // 5387 AA
#define IC_VER_5387_AB      0x53870001         // 5387 AB

#define IC_VER_5365_AA      0x53650000         // 5365 AA
#define IC_VER_5365_AB      0x53650001         // 5365 AB
#define IC_VER_5365_AC      0x53650002         // 5365 AC

#define IC_VER_5395_AA      0x53950000         // 5395 AA
#define IC_VER_5395_AB      0x53950001         // 5395 AB
#define IC_VER_5395_AC      0x53950002         // 5395 AC

#define IC_VER_5389_AA      0x53890000         // 5389 AA
#define IC_VER_5389_AB      0x53890001         // 5389 AB
#define IC_VER_5389_AC      0x53890002         // 5389 AC

#define IC_VER_5368_AA      0x53680000         // 5368 AA
#define IC_VER_5368_AB      0x53680001         // 5368 AB
#define IC_VER_5368_AC      0x53680002         // 5368 AC

#define IC_VER_5396_AA      0x53960000         // 5396 AA
#define IC_VER_5396_AB      0x53960001         // 5396 AB
#define IC_VER_5396_AC      0x53960002         // 5396 AC

#define IC_VER_5398_AA      0x53980000         // 5398 AA
#define IC_VER_5398_AB      0x53980001         // 5398 AB
#define IC_VER_5398_AC      0x53980002         // 5398 AC

#define IC_VER_5360_AA      0x62360AA0U         // 5360 AA
#define IC_VER_5360_AB      0x62360AB0U         // 5360 AB
#define IC_VER_5360_AC      0x62360AC0U         // 5360 AC
#define IC_VER_5360_AD      0x62360AD0U         // 5360 AD
#define IC_VER_5360_AE      0x62360AE0U         // 5360 AE
#define IC_VER_5360_AF      0x62360AF0U         // 5360 AF
#define IC_VER_5360_BA      0x62360BA0U         // 5360 BA
#define IC_VER_5360_BB      0x62360BB0U         // 5360 BB
#define IC_VER_5360_BC      0x62360BC0U         // 5360 BC
#define IC_VER_5360_BD      0x62360BD0U         // 5360 BD
#define IC_VER_5360_CA      0x62360CA0U         // 5360 CA
#define IC_VER_5360_CB      0x62360CB0U         // 5360 CB
#define IC_VER_5360_CC      0x62360CC0U         // 5360 CC

#define IC_VER_5391_AA      0x62391AA0U         // 5391 AA
#define IC_VER_5391_AB      0x62391AB0U         // 5391 AB
#define IC_VER_5391_AC      0x62391AC0U         // 5391 AC
#define IC_VER_5391_AD      0x62391AD0U         // 5391 AD
#define IC_VER_5391_AE      0x62391AE0U         // 5391 AE
#define IC_VER_5391_AF      0x62391AF0U         // 5391 AF
#define IC_VER_5391_BA      0x62391BA0U         // 5391 BA
#define IC_VER_5391_BB      0x62391BB0U         // 5391 BB
#define IC_VER_5391_BC      0x62391BC0U         // 5391 BC
#define IC_VER_5391_CA      0x62391CA0U         // 5391 CA
#define IC_VER_5391_CB      0x62391CB0U         // 5391 CB
#define IC_VER_5391_CC      0x62391CC0U         // 5391 CC

#define IC_VER_5392B_AA     0x53930000U         // 5392B - 2008.09.05
#define IC_VER_5392B        0x53930000U         // 5392B - 2008.09.05
#define IC_VER_5392B_E1     0x53930001U         // 5392B 1st ECO - 2008.10.22
#define IC_VER_5392B_E2     0x53930002U         // 5392B 2nd ECO
#define IC_VER_5392B_E3     0x53930003U         // 5392B 3rd ECO
#define IC_VER_5392B_E4     0x53930004U         // 5392B 4th ECO
#define IC_VER_5392B_E5     0x53930005U         // 5392B 5th ECO
#define IC_VER_5392B_E6     0x53930006U         // 5392B 6th ECO
#define IC_VER_5392B_E7     0x53930007U         // 5392B 7th ECO

#define IC_VER_5880_AA      0x58800000         // 5880 AA
#define IC_VER_5880_AB      0x58800001         // 5880 AB
#define IC_VER_5880_AC      0x58800002         // 5880 AC

#define IC_VER_UNKNOWN      0xffffffffU         // Unknown version

typedef UINT32 IC_VERSION_T;

//===========================================================================
// Exported functions

EXTERN void* BSP_AllocateReserved(UINT32 u4Size);
EXTERN void* BSP_AllocateReservedAlign(UINT32 u4Size, UINT32 u4Align);
EXTERN void BSP_FreeReserved(void* p);
EXTERN void* BSP_MapReservedMem(void *pv_Phys, UINT32 z_Size);
EXTERN BOOL BSP_IsFPGA(void);
EXTERN UINT32 BSP_GetSystemClock(void);
EXTERN void BSP_HaltSystem(void);
EXTERN UINT32 BSP_AllocDmaMemory(UINT32 u4Size);
EXTERN BOOL BSP_FreeDmaMemory(UINT32 uAddr);
EXTERN UINT32 BSP_AllocAlignedDmaMemory(UINT32 u4Size, UINT32 u4Align);
EXTERN BOOL BSP_FreeAlignedDmaMemory(UINT32 uAddr);
EXTERN IC_VERSION_T BSP_GetIcVersion(void);
EXTERN UINT32 BSP_IsSystemAcOn(void);
EXTERN UINT32 BSP_GetFbmMemAddr(void);
EXTERN UINT32 BSP_QuerySetAbortRebootFlag(UINT32 *pu4Flag);
EXTERN UINT32 BSP_GetChBMemSize(void);
EXTERN UINT32 BSP_GetMemSize(void);
EXTERN UINT32 BSP_GetPanelTableOffset(void);
EXTERN UINT32 BSP_GetPQDataOffset(void);
EXTERN UINT32 BSP_GetPQSmartPicOffset(void);
EXTERN UINT32 BSP_GetAQDataOffset(void);

/// Dump system task state.
EXTERN void BspDumpSystemTasksState(void);

#endif  // X_HAL_5381_H
