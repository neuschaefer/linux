/* 

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2013-2017 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2013-2016 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* PUMA7 DOCSIS Boot Parameters definition file */

#ifndef   _DOCSIS_IP_BOOT_PARAMS_H_
#define   _DOCSIS_IP_BOOT_PARAMS_H_


/* Boot Params top address id fix to 8KB */

/* since this is used in various places, the #define needs to be an absolute address. it is calculated like so: (AVALANCHE_SRAM_BASE + 0x00004000) */
#define BOOT_PARAMS_TOP_OFFSET	0x4000

/* Boot Params fields */
#define BOOT_PARAM_VER                  (BOOT_PARAMS_TOP_OFFSET         - sizeof(unsigned int ))  /* 0xFFFF3FFC */
#define ARM11_BOOT_STATUS               (BOOT_PARAM_VER                 - sizeof(unsigned int ))  /* 0xFFFF3FF8 */
#define SILICON_STEPPING                (ARM11_BOOT_STATUS              - sizeof(unsigned int ))  /* 0xFFFF3FF4 */
#define APP_CPU_BIOS_VERSION            (SILICON_STEPPING               - sizeof(unsigned int ))  /* 0xFFFF3FF0 */
#define BOOT_MODE                       (APP_CPU_BIOS_VERSION           - sizeof(unsigned int ))  /* 0xFFFF3FEC */
#define BOARD_TYPE                      (BOOT_MODE                      - sizeof(unsigned int ))  /* 0xFFFF3FE8 */
#define BOARD_REVISION                  (BOARD_TYPE                     - sizeof(unsigned int ))  /* 0xFFFF3FE4 */
#define ARM11_DDR_OFFSET                (BOARD_REVISION                 - sizeof(unsigned int ))  /* 0xFFFF3FE0 */
#define ARM11_DDR_SIZE                  (ARM11_DDR_OFFSET               - sizeof(unsigned int ))  /* 0xFFFF3FDC */
#define ARM11_OS_OFFSET                 (ARM11_DDR_SIZE                 - sizeof(unsigned int ))  /* 0xFFFF3FD8 */
#define ARM11_OS_SIZE                   (ARM11_OS_OFFSET                - sizeof(unsigned int ))  /* 0xFFFF3FD4 */
#define ARM11_OS_COMMANDLINE_ADDR       (ARM11_OS_SIZE                  - sizeof(unsigned int ))  /* 0xFFFF3FD0 */
#define ARM11_OS_COMMANDLINE_SIZE       (ARM11_OS_COMMANDLINE_ADDR      - sizeof(unsigned int ))  /* 0xFFFF3FCC */
#define DEBUG_BOOT_LOADER_OFFSET        (ARM11_OS_COMMANDLINE_SIZE      - sizeof(unsigned int ))  /* 0xFFFF3FC8 */
#define DEBUG_PARAMETERS_ADDR           (DEBUG_BOOT_LOADER_OFFSET       - sizeof(unsigned int ))  /* 0xFFFF3FC4 */
#define NPCPU_ONLY_MODE                 (DEBUG_PARAMETERS_ADDR          - sizeof(unsigned int ))  /* 0xFFFF3FC0 */
#define RESET_TYPE                      (NPCPU_ONLY_MODE                - sizeof(unsigned int ))  /* 0xFFFF3FBC */
#define RESET_REASON                    (RESET_TYPE                     - sizeof(unsigned int ))  /* 0xFFFF3FB8 */
#define PLATFORM_TYPE                   (RESET_REASON                   - sizeof(unsigned int ))  /* 0xFFFF3FB4 */
#define SHARED_DDR_OFFSET               (PLATFORM_TYPE                  - sizeof(unsigned int ))  /* 0xFFFF3FB0 */
#define SHARED_DDR_SIZE                 (SHARED_DDR_OFFSET              - sizeof(unsigned int ))  /* 0xFFFF3FAC */
#define RESET_BUTTON_DURATION           (SHARED_DDR_SIZE                - sizeof(unsigned int ))  /* 0xFFFF3FA8 */
#define SECURE_BOOT_STATUS              (RESET_BUTTON_DURATION          - sizeof(unsigned int ))  /* 0xFFFF3FA4 */
#define BOOT_PARAMS_BOTTOM_OFFSET       (SECURE_BOOT_STATUS             - sizeof(unsigned int ))  /* 0xFFFF3FA0 */

/* Silicon stepping ID */
enum SiliconStepping {
    SILICON_STEPPING_ID_A_0 = 0,
    SILICON_STEPPING_ID_A_1,
    SILICON_STEPPING_ID_A_2,
    SILICON_STEPPING_ID_A_3,
	SILICON_STEPPING_ID_A_4,
	SILICON_STEPPING_ID_A_5,
	SILICON_STEPPING_ID_A_6,
	SILICON_STEPPING_ID_A_7,
	SILICON_STEPPING_ID_B_0,
    SILICON_STEPPING_ID_B_1,
    SILICON_STEPPING_ID_B_2,
    SILICON_STEPPING_ID_B_3,
	SILICON_STEPPING_ID_B_4,
	SILICON_STEPPING_ID_B_5,
	SILICON_STEPPING_ID_B_6,
	SILICON_STEPPING_ID_B_7,
	SILICON_STEPPING_ID_C_0,
    SILICON_STEPPING_ID_C_1,
    SILICON_STEPPING_ID_C_2,
    SILICON_STEPPING_ID_C_3,
    SILICON_STEPPING_ID_C_4,
    SILICON_STEPPING_ID_C_5,
    SILICON_STEPPING_ID_C_6,
    SILICON_STEPPING_ID_C_7,
	SILICON_STEPPING_ID_D_0,
	SILICON_STEPPING_ID_D_1,
	SILICON_STEPPING_ID_D_2,
	SILICON_STEPPING_ID_D_3
};
typedef enum SiliconStepping SiliconSteppingId_t;

/* Board revision ID */
enum BoardRevision {
	BOARD_REVISION_ID_1_0 = 0x10,
	BOARD_REVISION_ID_1_1 = 0x11,
	BOARD_REVISION_ID_1_2 = 0x12,
	BOARD_REVISION_ID_1_3 = 0x13,
	BOARD_REVISION_ID_2_0 = 0x20,
	BOARD_REVISION_ID_2_1 = 0x21,
	BOARD_REVISION_ID_2_2 = 0x22,
	BOARD_REVISION_ID_2_3 = 0x23,
	BOARD_REVISION_ID_3_0 = 0x30,
	BOARD_REVISION_ID_3_1 = 0x31,
	BOARD_REVISION_ID_3_2 = 0x32,
	BOARD_REVISION_ID_3_3 = 0x33,
	BOARD_REVISION_ID_4_0 = 0x40,
	BOARD_REVISION_ID_4_1 = 0x41,
	BOARD_REVISION_ID_4_2 = 0x42,
	BOARD_REVISION_ID_4_3 = 0x43
};
typedef enum BoardRevision BoardRevisionId_t;

/* Macros to read & write */
#define GET_BOOT_PARAM_REG(addr)       (*((volatile unsigned int *)(addr)))
#define SET_BOOT_PARAM_REG(addr,val)   (*((volatile unsigned int *)(addr)) = (val))

#define BOOT_PARAM_BYTE_READ(addr)     (*((volatile unsigned char *)(addr)))
#define BOOT_PARAM_SHORT_READ(addr)    (*((volatile unsigned short *)(addr)))
#define BOOT_PARAM_DWORD_READ(addr)    (*((volatile unsigned int *)(addr)))
#define BOOT_PARAM_LONG_READ(addr)     (*((volatile unsigned long *)(addr)))

/* Macros to use form boot ram - in order to delete these macros there is a need to change their name in the boot ram */
#define GET_BOOT_PARAM_DWORD_REG(addr)       GET_BOOT_PARAM_REG(addr)
#define SET_BOOT_PARAM_DWORD_REG(addr,val)   SET_BOOT_PARAM_REG(addr,val)

#define GET_BOOT_PARAM_BYTE_REG(addr)        BOOT_PARAM_BYTE_READ(addr)
#define SET_BOOT_PARAM_BYTE_REG(addr,val)    (*((volatile unsigned char *)(addr)) = (val))


/* Data definitions for Boot Mode */
#define BOOT_MODE_CONFIGURE_OS         (0)
#define BOOT_MODE_DEBUG_BOOT_LOADER    (1)

/* Data definitions for Reset Type and Reset Reason
 * are defined as part of the RESET_INFO PMC register
 */
/* Data definitions for Reset Type */
#define RESET_TYPE_COLD_BOOT               (0x00) /* Cold Boot (SOC G3 exit) */
#define RESET_TYPE_GLOBAL_RESET            (0x01) /* Global Reset (Type 7/8) */
#define RESET_TYPE_WARM_RESET              (0x02) /* Warm Reset (Type 1: Host Reset without power cycle) */
#define RESET_TYPE_COLD_RESET              (0x03) /* Cold Reset (Type 2: Host Reset Power Cycle) */
#define RESET_TYPE_EXIT_DEEP_STANDBY       (0x04)

/* Data definitions for Reset Reason */
#define RESET_REASON_NO_CAUSE               (0x00) /* No Reset Cause (SOC G3 exit) */
#define RESET_REASON_MISC                   (0x01) /* Misc */
#define RESET_REASON_PMC_FW_WDT             (0x02) /* PMC FW WDT */
#define RESET_REASON_GPIO_RST_BUTTON_PRESS  (0x03) /* GPIO Reset Button press (PMU_RESETBUTTON_B) */
#define RESET_REASON_ATOM_CF9               (0x04) /* ATOM - 0xCF9 */
#define RESET_REASON_TCO_WD                 (0x05) /* TCO watchdog (ATOM) */
#define RESET_REASON_RESERVED               (0x06) /* Reserved */
#define RESET_REASON_NETIP_WDT              (0x07) /* NETIP WDT */
#define RESET_REASON_BBU_C_WDT              (0x08) /* BBU-C WDT */
#define RESET_REASON_REBOOT_WDT             (0x09) /* REBOOT WDT */
#define RESET_REASON_PUNIT_WDT              (0x0A) /* Punit WDT */
#define RESET_REASON_CSE_WDT                (0x0B) /* CSE WDT */
#define RESET_REASON_STC WDT                (0x0C) /* STC WDT */
#define RESET_REASON_COREPWROK_LOSS         (0x0D) /* COREPWROK loss */
#define RESET_REASON_THERMTRIP              (0x0E) /* THERMTRIP */
#define RESET_REASON_PWRBTN_OVERRIDE        (0x0F) /* PwrButton Override */
#define RESET_REASON_SEC_PD                 (0x10) /* SEC_PD */
#define RESET_REASON_GLOBAL_RESET_PROM      (0x11) /* Global Reset Promotion */
#define RESET_REASON_CPU_SHUTDOWN_PUNIT     (0x12) /* CPU_Shutdown message from PUNIT */


/* Data definitions for ARM Boot Status */
#define ARM_STAT_BOOT_RAM_START    0x00000001
#define ARM_STAT_BOOT_RAM_END      0x00000002
#define ARM_STAT_BOOT_UBOOT_START  0x00000004
#define ARM_STAT_BOOT_UBOOT_END    0x00000008
#define ARM_STAT_BOOT_KERNEL_START 0x00000010

/* Data definitions for board types */
#define PUMA7_UNKNOWN_BOARD_ID    (0x0)  /* ERROR          */
#define PUMA7_CP_BOARD_ID         (0xE0) /* Cougar-park    */
#define PUMA7_CI_MG_BOARD_ID      (0xE1) /* Cougar-island  */
#define PUMA7_CM_BOARD_ID         (0xE2) /* Cougar-mine    */
#define PUMA7_CV_BOARD_ID         (0xE3) /* Cougar-valley  */
#define PUMA7_CP_BOARD_NAME       "Cougar-park"
#define PUMA7_CI_BOARD_NAME       "Cougar-island"
#define PUMA7_CM_BOARD_NAME       "Cougar-mine"
#define PUMA7_CV_BOARD_NAME       "Cougar-valley"

/* Data definitions to Platform type */
#define PLATFORM_TYPE_CABLE_MODEM             (0)
#define PLATFORM_TYPE_RESIDENTIAL_GATEWAY     (1)
#define PLATFORM_TYPE_MEDIA_GATEWAY           (2)

/* Data definitions for NPCPU_ONLY mode */
#define NPCPU_ONLY_DISABLE		0x00000000
#define NPCPU_ONLY_ENABLE		0x00000001

#endif /*_DOCSIS_IP_BOOT_PARAMS_H_*/
