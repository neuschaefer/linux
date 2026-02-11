/*****************************************************************************
* Copyright 2004 - 2008 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/




/****************************************************************************/
/**
*  @file    otpHw_reg.h
*
*  @brief   Definitions for low level OTP registers
*
*/
/****************************************************************************/
#ifndef OTPHW_REG_H
#define OTPHW_REG_H

/* Defines for config register */
#define otpHW_REG_CONFIG_GET(addr)                       ( *(volatile uint32_t *)( addr + OTP_CFG_OFFSET ) )
#define otpHW_REG_CONFIG_SET(addr, val)                  ( *(volatile uint32_t *)( addr + OTP_CFG_OFFSET ) = (val) )
#define otpHw_REG_CONFIG_OTP_HW_INIT_DONE(reg)           ( (reg)  & 0x00000001 )
#define otpHw_REG_CONFIG_USE_TEST_KEYS(reg)              ( (reg) |= 0x00000010 )
#define otpHw_REG_CONFIG_DO_NOT_USE_TEST_KEYS(reg)       ( (reg) &= (~0x00000010) )
#define otpHw_REG_CONFIG_OTP_ENABLE(reg)                 ( (reg) |= 0x00001000 )
#define otpHw_REG_CONFIG_OTP_DISABLE(reg)                ( (reg) &= (~0x00001000) )
#define otpHw_REG_CONFIG_OTP_RST(reg)                    ( (reg) |= 0x00004000 )
#define otpHw_REG_CONFIG_OTP_RST_CLEAR(reg)              ( (reg) &= (~0x00004000) )
#define otpHw_REG_CONFIG_OTP_CPU_MODE_ENABLE(reg)        ( (reg) |= 0x00008000 )
#define otpHw_REG_CONFIG_OTP_CPU_MODE_DISABLE(reg)       ( (reg) &= (~0x00008000) )
#define otpHw_REG_CONFIG_OTP_CLK_DIV_BY_4                0
#define otpHw_REG_CONFIG_OTP_CLK_DIV_BY_8                1
#define otpHw_REG_CONFIG_OTP_CLK_DIV_BY_16               2
#define otpHw_REG_CONFIG_OTP_CLK_DIV_BY_32               3
#define otpHw_REG_CONFIG_OTP_CLK_DIV_RATIO_SET(reg,ratio)   ( (reg) = ((reg) & (~0x00300000)) | (((ratio) & 0x03) << 20) )
#define otpHw_REG_CONFIG_OTP_CLK_DIV_ENABLE(reg)            ( (reg) |= 0x00400000 )
#define otpHw_REG_CONFIG_OTP_CLK_DIV_DISABLE(reg)           ( (reg) &= (~0x00400000) )

/* Defines for writeData registers */
#define otpHw_REG_WDATA_0_SET(addr, data_word)              ( *(volatile uint32_t *)(addr + OTP_WDATA_0_OFFSET) = (data_word) )
#define otpHw_REG_WDATA_1_SET(addr, data_word)              ( *(volatile uint32_t *)(addr + OTP_WDATA_1_OFFSET) = ((data_word) & 0x1FF ) )
#define otpHw_REG_WDATA_1_SET_FAIL(addr, fail)              ( *(volatile uint32_t *)(addr + OTP_WDATA_1_OFFSET) = \
                                                            ( *(volatile uint32_t *)(addr + OTP_WDATA_1_OFFSET) & 0x00000380 ) | (((fail) & 0x03) << 7) )
#define otpHw_REG_WDATA_1_SET_ECC(addr, ecc)                ( *(volatile uint32_t *)(addr + OTP_WDATA_1_OFFSET) = \
                                                            ( *(volatile uint32_t *)(addr + OTP_WDATA_1_OFFSET) & 0x0000007F ) | (((ecc) & 0x07F)) )

/* Defines for address register */
#define otpHw_REG_WORD_ADDR_SHIFT                        6
#define otpHw_REG_WORD_ADDR_MAX                          111
#define otpHw_REG_BIT_ADDR_MAX                           40
#define otpHw_REG_WORD_ADDR_SET(addr, word_offset)        ( *(volatile uint32_t *)(addr + OTP_ADDR_OFFSET) = (((word_offset)  & 0x0FF) << otpHw_REG_WORD_ADDR_SHIFT) )
#define otpHw_REG_BIT_ADDR_SET(addr, word_offset, bit_offset)  ( *(volatile uint32_t *)(addr + OTP_ADDR_OFFSET) = ((((word_offset) & 0x0FF) << otpHw_REG_WORD_ADDR_SHIFT) | \
		                                                                                                   ((bit_offset)  & 0x03F)) )

/* Defines for control register */
#define otpHw_REG_OTP_CONTROL_GET(addr)                  ( *(volatile uint32_t *)(addr + OTP_CTRL_OFFSET) )
#define otpHw_REG_OPT_CONTROL_SET(addr, val)             ( *(volatile uint32_t *)(addr + OTP_CTRL_OFFSET) = (val) )
#define otpHw_REG_OTP_COMMAND_READ_SET(reg)              ((reg) &= (~0x0000001F))
#define otpHw_REG_OTP_COMMAND_PROG_ENABLE_SET(reg)       ((reg)  = ((reg) & (~0x0000001F)) | 0x00000001 )
#define otpHw_REG_OTP_COMMAND_PROG_DISABLE_SET(reg)      ((reg)  = ((reg) & (~0x0000001F)) | 0x00000002 )
#define otpHw_REG_OTP_COMMAND_VERIFY_SET(reg)            ((reg)  = ((reg) & (~0x0000001F)) | 0x00000003 )
#define otpHw_REG_OTP_COMMAND_INIT_SET(reg)              ((reg)  = ((reg) & (~0x0000001F)) | 0x00000004 )
#define otpHw_REG_OTP_COMMAND_LOCK_SET(reg)              ((reg)  = ((reg) & (~0x0000001F)) | 0x00000005 )
#define otpHw_REG_OTP_COMMAND_STDBY_SET(reg)             ((reg)  = ((reg) & (~0x0000001F)) | 0x00000006 )
#define otpHw_REG_OTP_COMMAND_WAKEUP_SET(reg)            ((reg)  = ((reg) & (~0x0000001F)) | 0x00000007 )
#define otpHw_REG_OTP_COMMAND_PRESCREEN_SET(reg)         ((reg)  = ((reg) & (~0x0000001F)) | 0x00000009 )
#define otpHw_REG_OTP_COMMAND_WRITE_BIT_SET(reg)         ((reg)  = ((reg) & (~0x0000001F)) | 0x0000000A )
#define otpHw_REG_OTP_COMMAND_WRITE_WORD_SET(reg)        ((reg)  = ((reg) & (~0x0000001F)) | 0x0000000B )
#define otpHw_REG_OTP_COMMAND_BURNIN_SET(reg)            ((reg)  = ((reg) & (~0x0000001F)) | 0x0000000C )
#define otpHw_REG_OTP_COMMAND_AUTO_RELOAD_SET(reg)       ((reg)  = ((reg) & (~0x0000001F)) | 0x0000000D )
#define otpHw_REG_OTP_COMMAND_OVST_READ_SET(reg)         ((reg)  = ((reg) & (~0x0000001F)) | 0x0000000E )
#define otpHw_REG_OTP_COMMAND_OVST_PROG_SET(reg)         ((reg)  = ((reg) & (~0x0000001F)) | 0x0000000F )

#define otpHw_REG_OTP_ACC_RAW_SET(reg)                   ((reg)  = ((reg) & (~0x00000300)) | 0x00000000 )
#define otpHw_REG_OTP_ACC_MANU_AREA_SET(reg)             ((reg)  = ((reg) & (~0x00000300)) | 0x00000100 )
#define otpHw_REG_OTP_ACC_CONFIG_AREA_SET(reg)           ((reg)  = ((reg) & (~0x00000300)) | 0x00000200 )
#define otpHw_REG_OTP_ACC_RAM_REPAIR_AREA_SET(reg)       ((reg)  = ((reg) & (~0x00000300)) | 0x00000300 )

#define otpHw_REG_OTP_ECC_BY_HW_SET(reg)                 ((reg) |= 0x01000000)
#define otpHw_REG_OTP_ECC_BY_SW_SET(reg)                 ((reg) &= (~0x01000000))

#define otpHw_REG_OTP_ECC_CHECK_ENABLE_SET(reg)          ((reg) |= 0x02000000)
#define otpHw_REG_OTP_ECC_CHECK_DISABLE_SET(reg)         ((reg) &= (~0x02000000))

/* Defines for status register */
#define otpHw_REG_STATUS_GET(addr)                       ( (*(volatile uint32_t *)(addr + OTP_STAT_OFFSET)) & 0x3FFFF )
#define otpHw_REG_STATUS_SET(addr, reg)                  ( (*(volatile uint32_t *)(addr + OTP_STAT_OFFSET)) = ((reg) & 0x3FFFF) )
#define otpHW_REG_STATUS_BITMSK_CMD_DONE                 (0x00000001)
#define otpHW_REG_STATUS_BITMSK_WRP_DATA_READY           (0x00000002)
#define otpHW_REG_STATUS_BITMSK_WRP_BIT_DOUT             (0x00000004)
#define otpHW_REG_STATUS_BITMSK_WRP_BUSY                 (0x00000008)
#define otpHW_REG_STATUS_BITMSK_WRP_FAIL                 (0x00000010)
#define otpHW_REG_STATUS_BITMSK_INVALID_PROG_REQ         (0x00000020)
#define otpHW_REG_STATUS_BITMSK_PROG_BLOCKED             (0x00000040)
#define otpHW_REG_STATUS_BITMSK_WRP_FDONE                (0x00000080)
#define otpHW_REG_STATUS_BITMSK_WRP_STBY                 (0x00000100)
#define otpHW_REG_STATUS_BITMSK_INVALID_CMD              (0x00000200)
#define otpHW_REG_STATUS_BITMSK_WRP_ERR                  (0x00000400)
#define otpHW_REG_STATUS_BITMSK_REFOK                    (0x00000800)
#define otpHW_REG_STATUS_BITMSK_PROGOK                   (0x00001000)
#define otpHW_REG_STATUS_BITMSK_INVALID_ADDR             (0x00002000)
#define otpHW_REG_STATUS_BITMSK_INVALID_ACC_MODE         (0x00004000)
#define otpHW_REG_STATUS_BITMSK_INVALID_SEC_ACC          (0x00008000)
#define otpHW_REG_STATUS_BITMSK_ECC_COR                  (0x00010000)
#define otpHW_REG_STATUS_BITMSK_ECC_DET                  (0x00020000)

/* Defines for readData registers */
#define otpHw_REG_RDATA_0_GET(addr)                      ( *(volatile uint32_t *)(addr + OTP_RDATA_0_OFFSET))
#define otpHw_REG_RDATA_1_GET(addr)                      ( (*(volatile uint32_t *)(addr + OTP_RDATA_1_OFFSET)) & 0x000001FF )
#define otpHw_REG_RDATA_1_ECC_GET(addr)                  ( (*(volatile uint32_t *)(addr + OTP_RDATA_1_OFFSET)) & 0x0000007F )
#define otpHw_REG_RDATA_1_FAIL_GET(addr)                 ( ((*(volatile uint32_t *)(addr + OTP_RDATA_1_OFFSET)) >> 7) & 0x00000003 )

#endif
