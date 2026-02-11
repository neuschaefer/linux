/*****************************************************************************
* Copyright 2004 - 2012 Broadcom Corporation.  All rights reserved.
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

/**
*  @file    otpHw_def.h
*
*  @brief   Definitions for low level OTP API
*
*/
#ifndef OTPHW_DEF_H
#define OTPHW_DEF_H

/* Definition for OTP memory areas */
#define otpHw_MANU_START_ROW                                           0
#define otpHw_MANU_END_ROW                                             5

#define otpHw_BRCMCFG_START_ROW                                        8 

#define optHw_KEYS_START_ROW                                           28
#define optHw_KEYS_END_ROW                                             95

#define optHw_CUST_START_ROW                                           96
#define optHw_CUST_END_ROW                                             103

#define otpHw_RESERVED_START_ROW                                       104
#define otpHw_RESERVED_END_ROW                                         107

#define otpHw_REPAIR_START_ROW                                         108
#define otpHw_REPAIR_END_ROW                                           111

#define otpHw_BITS_PER_ROW                                             41

#define otpHw_MAX_ROW                                                  111

#define otpHw_CLOCK_FREQUENCY_Hz                                       6250000
#define otpHw_ROW_WRAPPER_LOCK_BITS                                    6
#define otpHw_REGION_HW_CONFIG_START                                   10 
#define otpHw_REGION_LOCK_ROW_START                                    16
#define otpHw_REGION_LOCK_ROW_END                                      19

enum WRAPPER_LOCK_BITS 
{
    MANU_DATA_LOCK_BIT_0_2 = 0,
    MANU_DATA_LOCK_BIT_3_5 = 1,
    CONFIG_DATA_LOCK_BIT   = 2,
    RAM_REPAIR_LOCK_BIT    = 3
};

/****************************************************************************/
/**
*  These error codes overlay the upper bits of the OTP status register to
*  indicate other error conditions that are not necessarily related to 
*  hardware.
*/
/****************************************************************************/
#define otpHW_REG_STATUS_OVERLAY_BAD_CALL_PARAMS                    0x80000000
#define otpHW_REG_STATUS_OVERLAY_BAD_WRITE_MODE                     0x40000000
#define otpHW_REG_STATUS_OVERLAY_WAIT_TIMEOUT                       0x20000000

#endif 
