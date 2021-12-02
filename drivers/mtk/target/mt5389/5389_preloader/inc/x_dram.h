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
#ifndef X_DRAM_H
#define X_DRAM_H

//----------------------------------------------------------------------------
// DQS definitions
//----------------------------------------------------------------------------
#define MAX_DQS_DELAY       96
#define MAX_DQS_OUT_DELAY   16
#define MAX_DQ_DELAY        16
#define MAX_DQ_OUT_DELAY    16
#define CALIB_START         0x100000
#define CALIB_LEN           0x100

//----------------------------------------------------------------------------
// DRAM USE general purpose register to be the parameter passing at preloader.
// BIM (IO_VIRT + 0x8100) ~ (IO_VIRT + 0x810F)
//----------------------------------------------------------------------------
#define TCM_SRAM_ADDR       (IO_VIRT + 0x8100)
#define TCM_DRAM_FLAGS    		(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR - 0x8)))
#define TCM_DRAM_SIZE       (*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR - 0x4)))
#define TCM_DRAM_FLAGS1      	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + 0)))
#define TCM_DRAM_FLAGS2     	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + 0x4)))
#define TCM_DRAM_FLAGS3     	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + 0x8)))
#define TCM_DRAM_FLAGS4     	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + 0xC)))
#define TCM_DRAM_REGBASE		(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + 0x50)))
#define TCM_DRAM_DDRPHY_REGBASE	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + 0x54)))

#define DRAM_CLOCK_MASK     (0x00000FFFU)
#define DRAM_TYPE_MASK      (0x0000F000U)
#define TYPE_SHIFT          (12)
#define DRAM_DDR_CL_MASK    (0x000F0000U)
#define CL_SHIFT            (16)

#define CHB_SUPPORT_SHIFT       (20)
#define DRAM_CHANNELB_SUPPORT   (1U << CHB_SUPPORT_SHIFT)
#define CHB_ACTIVE_SHIFT        (21)
#define DRAM_CHANNELB_ACTIVE    (1U << CHB_ACTIVE_SHIFT)
#define CHA_FORCE32_SHIFT       (22)
#define DRAM_CHA_FORCE32        (1U << CHA_FORCE32_SHIFT)
#define CHB_FORCE32_SHIFT       (23)
#define DRAM_CHB_FORCE32        (1U << CHB_FORCE32_SHIFT)
#define QFP_SHIFT               (24)
#define DRAM_DDR_QFP            (1U << QFP_SHIFT)
#define SWAP16BIT_SHIFT         (25)
#define DRAM_SWAP16BIT          (1U << SWAP16BIT_SHIFT)
#define DMSSON_SHIFT            (26)
#define DRAM_DMSSON             (1U << DMSSON_SHIFT)
#define ASYNCON_SHIFT       	(27)
#define DRAM_ASYNCON        	(1U << ASYNCON_SHIFT)
#define CTSMODE_SHIFT       	(28)
#define DRAM_CTSMODE        	(1U << CTSMODE_SHIFT)
#define BUSX8_SHIFT       	(29)
#define DRAM_BUSX8        	(1U << BUSX8_SHIFT)

#define BASE_DDR_CLK        1000000
#define TCMGET_DDR_CLK()    ((TCM_DRAM_FLAGS & DRAM_CLOCK_MASK) * BASE_DDR_CLK)
#define TCMGET_DRAMTYPE()   ((TCM_DRAM_FLAGS & DRAM_TYPE_MASK) >> TYPE_SHIFT)
#define TCMGET_DDR_CL()     ((TCM_DRAM_FLAGS & DRAM_DDR_CL_MASK) >> CL_SHIFT)
#ifdef CC_MTK_PRELOADER
#define TCMSET_CHANNELA_ACTIVE()    (TCM_DRAM_FLAGS &= ~DRAM_CHANNELB_ACTIVE); \
                                    (TCM_DRAM_REGBASE = DRAM_BASE);\
                                    (TCM_DRAM_DDRPHY_REGBASE = DRAM_DDRPHY_BASE)
#define TCMSET_CHANNELB_ACTIVE()    (TCM_DRAM_FLAGS |= DRAM_CHANNELB_ACTIVE); \
                                    (TCM_DRAM_REGBASE = DRAM_CHB_BASE);\
                                    (TCM_DRAM_DDRPHY_REGBASE = DRAM_DDRPHY_CHB_BASE)
#else
#define TCMSET_CHANNELA_ACTIVE()    (TCM_DRAM_FLAGS &= ~DRAM_CHANNELB_ACTIVE)
#define TCMSET_CHANNELB_ACTIVE()    (TCM_DRAM_FLAGS |= DRAM_CHANNELB_ACTIVE)
#endif /* #ifdef CC_MTK_PRELOADER */

#define IS_DRAM_CHANNELB_SUPPORT()    (0)
#define IS_DRAM_CHANNELB_ACTIVE()    (0)
#define TCMSET_CHANNELA_SIZE(size)    (TCM_DRAM_SIZE = (TCM_DRAM_SIZE & 0xffff0000) + (size & 0xffff))   // size in Mbytes.
#define TCMGET_CHANNELA_SIZE()         (TCM_DRAM_SIZE & 0xFFFF) // size in Mbytes. 
#define TCMSET_CHANNELB_SIZE(size)    (TCM_DRAM_SIZE = (TCM_DRAM_SIZE & 0xffff) + (size << 16))
#define TCMGET_CHANNELB_SIZE()         ((TCM_DRAM_SIZE >> 16) & 0xFFFF) // size in Mbytes.
#define IS_FORCE32()        (IS_DRAM_CHANNELB_ACTIVE() ? \
                                                ((TCM_DRAM_FLAGS & DRAM_CHB_FORCE32) ? 1 : 0) : \
                                                ((TCM_DRAM_FLAGS & DRAM_CHA_FORCE32) ? 1 : 0))
#define IS_DDR_QFP()        ((TCM_DRAM_FLAGS & DRAM_DDR_QFP) ? 1 : 0)
#define IS_DDR_SWAP16BIT()   (0)
#define IS_DDR_DMSSON()        ((TCM_DRAM_FLAGS & DRAM_DMSSON) ? 1 : 0)
#define IS_DDR_ASYNCON()        ((TCM_DRAM_FLAGS & DRAM_ASYNCON) ? 1 : 0)
#define IS_DDR_CTSMODE()        ((TCM_DRAM_FLAGS & DRAM_CTSMODE) ? 1 : 0)
#define IS_DDR_BUSX8()        ((TCM_DRAM_FLAGS & DRAM_BUSX8) ? 1 : 0)

//lint -emacro({717}, ADD_DDR_CLK)  to disable "do {...} while (0)" lint warning
#define ADD_DDR_CLK()                                                       \
            do {                                                            \
                UINT32 u4AddClk;                                            \
                u4AddClk = (TCM_DRAM_FLAGS & DRAM_CLOCK_MASK);              \
                TCM_DRAM_FLAGS &= ~(DRAM_CLOCK_MASK);                       \
                TCM_DRAM_FLAGS |= ((u4AddClk + 54) & DRAM_CLOCK_MASK);     \
            } while (0)

//lint -emacro({717}, SUB_DDR_CLK)  to disable "do {...} while (0)" lint warning
#define SUB_DDR_CLK()                                                       \
            do {                                                            \
                UINT32 u4SubClk;                                            \
                u4SubClk = (TCM_DRAM_FLAGS & DRAM_CLOCK_MASK);              \
                    TCM_DRAM_FLAGS &= ~(DRAM_CLOCK_MASK);                   \
                TCM_DRAM_FLAGS |= ((u4SubClk - 54) & DRAM_CLOCK_MASK);   \
            } while (0)

// DRAM data read/write macros
#define INIT_DRAM_B_BASE            0x80000000
#define INIT_DRAM_B_CHB_BASE        0x40000000

#define MEM_READ32(offset)              (IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                    IO_READ32(INIT_DRAM_B_CHB_BASE, (offset)) : \
                                                                    IO_READ32(INIT_DRAM_B_BASE, (offset)))
                                                                    
#define MEM_WRITE32(offset, value)      (IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                    IO_WRITE32(INIT_DRAM_B_CHB_BASE, (offset), (value)): \
                                                                    IO_WRITE32(INIT_DRAM_B_BASE, (offset), (value)))                                                                    

#ifdef CC_MTK_PRELOADER

#define DRAM_READ32(offset)            	IO_READ32(TCM_DRAM_REGBASE, (offset))
#if 1
#define DRAM_WRITE32(offset, value)     IO_WRITE32(TCM_DRAM_REGBASE, (offset), (value))
#else
#define DRAM_WRITE32(offset, value)     IO_WRITE32(TCM_DRAM_REGBASE, (offset), (value)); \
                                        Printf("DRAM W: 0x%08X = 0x%08X.\n", TCM_DRAM_REGBASE + offset, value)
#endif

#else
// DRAM controller register read/write macros
#define DRAM_READ32(offset)           (IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                   IO_READ32(DRAM_CHB_BASE, (offset)) : \
                                                                   IO_READ32(DRAM_BASE, (offset)))
#define DRAM_WRITE32(offset, value)     (IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                        IO_WRITE32(DRAM_CHB_BASE, (offset), (value)) : \
                                                                        IO_WRITE32(DRAM_BASE, (offset), (value)))
#endif /* #ifdef CC_MTK_PRELOADER */

#ifdef CC_MTK_PRELOADER

#define DRAM_DDRPHY_READ32(offset)            	IO_READ32(TCM_DRAM_DDRPHY_REGBASE, (offset))
#if 1
#define DRAM_DDRPHY_WRITE32(offset, value)     	IO_WRITE32(TCM_DRAM_DDRPHY_REGBASE, (offset), (value))
#else
#define DRAM_DDRPHY_WRITE32(offset, value)     IO_WRITE32(TCM_DRAM_DDRPHY_REGBASE, (offset), (value)); \
                                        				Printf("DRAM W: 0x%08X = 0x%08X.\n", TCM_DRAM_DDRPHY_REGBASE + offset, value)
#endif

#else
// DRAM controller register read/write macros
#define DRAM_DDRPHY_READ32(offset)           (IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                   IO_READ32(DRAM_DDRPHY_CHB_BASE, (offset)) : \
                                                                   IO_READ32(DRAM_DDRPHY_BASE, (offset)))
#define DRAM_DDRPHY_WRITE32(offset, value)   (IS_DRAM_CHANNELB_ACTIVE() ? \
	                                                                     IO_WRITE32(DRAM_DDRPHY_CHB_BASE, (offset), (value)) : \
	                                                                     IO_WRITE32(DRAM_DDRPHY_BASE, (offset), (value)))
#endif /* #ifdef CC_MTK_PRELOADER */


/*----------------------------------------------------------------------------*/
/*! @struct DRAM_CFG_T
 *  @brief
 *
 *  @code
 * typedef struct _DRAM_CFG_T
 * {
 *   UINT8       ui1_ssc_range;
 *   UINT8       ui1_ssc_modulation;
 *   UINT8       ui1_clk_driving;
 *   UINT8       ui1_clk_delay;
 *   UINT8       ui1_cmd_driving;
 *   UINT8       ui1_cmd_delay;
 *   UINT8       ui1_wdqs_driving;
 *   UINT8       ui1_wdqs_delay;
 *   UINT8       ui1_wdq_driving;
 *   UINT8       ui1_wdq_delay;    
 * } DRAM_CFG_T;
 *  @endcode
 *
 *  @li@c  ui1_ssc_range               - spread spectrum clock: range 0 ~ 100: 0% ~ +-1%. 0 = disable SSC.
 *  @li@c  ui1_ssc_modulation       - spread spectrum modulation: range 0 ~ 60: 0Khz ~ 60Khz. 0 = disable SSC.
 *  @li@c  ui1_clk_driving               - clock driving: range 0 ~ 15.
 *  @li@c  ui1_clk_delay                 - clock delay: range 0 ~ 15.
 *  @li@c  ui1_cmd_driving             - command driving: range 0 ~ 15.
 *  @li@c  ui1_cmd_delay               - command delay: range 0 ~ 15.
 *  @li@c  ui1_wdqs_driving            - write DQS driving: range 0 ~ 15.
 *  @li@c  ui1_wdqs0_delay             - write DQS0 delay: range 0 ~ 15.
 *  @li@c  ui1_wdqs1_delay             - write DQS1 delay: range 0 ~ 15.
 *  @li@c  ui1_wdqs2_delay             - write DQS2 delay: range 0 ~ 15.
 *  @li@c  ui1_wdqs3_delay             - write DQS3 delay: range 0 ~ 15. 
 *  @li@c  ui1_wdq_driving              - write DQ driving: range 0 ~ 15.
 *  @li@c  ui1_wdq0_delay                - write DQ bit  0 ~  7 delay: range 0 ~ 15.
 *  @li@c  ui1_wdq1_delay                - write DQ bit  8 ~ 15 delay: range 0 ~ 15.
 *  @li@c  ui1_wdq2_delay                - write DQ bit 16 ~ 23 delay: range 0 ~ 15.
 *  @li@c  ui1_wdq3_delay                - write DQ bit 24 ~ 31 delay: range 0 ~ 15.
 */
/*----------------------------------------------------------------------------*/
#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
typedef struct _DRAM_CFG_T
{
    UINT8       ui1_ssc_range;
    UINT8       ui1_ssc_modulation;
    UINT8       ui1_clk_driving;
    UINT8       ui1_clk_delay;
    UINT8       ui1_cmd_driving;
    UINT8       ui1_cmd_delay;
    UINT8       ui1_wdqs_driving;
    UINT8       ui1_wdqs0_delay;
    UINT8       ui1_wdqs1_delay;
    UINT8       ui1_wdqs2_delay;    
    UINT8       ui1_wdqs3_delay;        
    UINT8       ui1_wdq_driving;
    UINT8       ui1_wdq0_delay;    
    UINT8       ui1_wdq1_delay;    
    UINT8       ui1_wdq2_delay; 
    UINT8       ui1_wdq3_delay;        
} DRAM_CFG_T;
#endif /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */

// DDR related functions.
extern CHAR * DDR_DramTypeString(UINT32 u4Type);
extern UINT32 DDR_IsDqsFail(UINT32 u4DQSth, UINT32 u4DQSVal, UINT32 u4DQSType, UINT32 u4ChipNum);
extern UINT32 DDR_CalibrateDqs(void);
extern UINT32 DDR_CalibrateOutDqs(void);
extern void DDR_SetDramController(void);
extern void DDR_SetAgentPriority(const UINT8 *pu1Prio);
extern void DDR_SetBurstLen(UINT32 u4TimeSlot1, UINT32 u4TimeSlot2);
extern void DDR_SetArbiterTime(UINT8 u1Group, UINT8 u1Time);

/*
    DRAM configuration API for factory mode.
        res_mngr\drv\u_drv_cust.h
        mw_if\drv_cust_api.c
*/ 
#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
extern BOOL DDR_SetCustCfg(DRAM_CFG_T *prDdrCfg);
extern BOOL DDR_GetCustCfg(DRAM_CFG_T *prDdrCfg);
#endif /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */

#endif /* X_DRAM_H */
