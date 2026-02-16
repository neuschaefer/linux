/* 

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2013-2015 Intel Corporation.

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

  Copyright(c) 2013-2015 Intel Corporation. All rights reserved.

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

/* PUMA6 DOCSIS Boot Parameters definition file */

#ifndef   _DOCSIS_IP_BOOT_PARAMS_H_
#define   _DOCSIS_IP_BOOT_PARAMS_H_

/* Boot Params top address id fix to 8KB */
#define BOOT_PARAMS_TOP_OFFSET    0x2000

/* Boot Paramas fields */
#define BOOT_PARAM_VER                  (BOOT_PARAMS_TOP_OFFSET         - sizeof(unsigned int ))  /* 0x1FFC */
#define ARM11_BOOT_STATUS               (BOOT_PARAM_VER                 - sizeof(unsigned int ))  /* 0x1FF8 */
#define BOOT_MODE                       (ARM11_BOOT_STATUS              - sizeof(unsigned int ))  /* 0x1FF4 */
#define BOARD_TYPE                      (BOOT_MODE                      - sizeof(unsigned int ))  /* 0x1FF0 */
#define NUMBER_OF_FLASHES               (BOARD_TYPE                     - sizeof(unsigned int ))  /* 0x1FEC */
#define ARM11_DDR_OFFSET                (NUMBER_OF_FLASHES              - sizeof(unsigned int ))  /* 0x1FE8 */
#define ARM11_DDR_SIZE                  (ARM11_DDR_OFFSET               - sizeof(unsigned int ))  /* 0x1FE4 */
#define AID_1_OFFSET                    (ARM11_DDR_SIZE                 - sizeof(unsigned int ))  /* 0x1FE0 */
#define AID_2_OFFSET                    (AID_1_OFFSET                   - sizeof(unsigned int ))  /* 0x1FDC */
#define ARM11_UBOOT_OFFSET              (AID_2_OFFSET                   - sizeof(unsigned int ))  /* 0x1FD8 */
#define ARM11_UBOOT_SIZE                (ARM11_UBOOT_OFFSET             - sizeof(unsigned int ))  /* 0x1FD4 */
#define ARM11_ENV1_OFFSET               (ARM11_UBOOT_SIZE               - sizeof(unsigned int ))  /* 0x1FD0 */
#define ARM11_ENV2_OFFSET               (ARM11_ENV1_OFFSET              - sizeof(unsigned int ))  /* 0x1FCC */
#define ARM11_ENV_SIZE                  (ARM11_ENV2_OFFSET              - sizeof(unsigned int ))  /* 0x1FC8 */
#define ARM11_NVRAM_OFFSET              (ARM11_ENV_SIZE                 - sizeof(unsigned int ))  /* 0x1FC4 */
#define ARM11_NVRAM_SIZE                (ARM11_NVRAM_OFFSET             - sizeof(unsigned int ))  /* 0x1FC0 */
#define ARM11_UBFI1_OFFSET              (ARM11_NVRAM_SIZE               - sizeof(unsigned int ))  /* 0x1FBC */
#define ARM11_UBFI1_SIZE                (ARM11_UBFI1_OFFSET             - sizeof(unsigned int ))  /* 0x1FB8 */
#define ARM11_UBFI2_OFFSET              (ARM11_UBFI1_SIZE               - sizeof(unsigned int ))  /* 0x1FB4 */
#define ARM11_UBFI2_SIZE                (ARM11_UBFI2_OFFSET             - sizeof(unsigned int ))  /* 0x1FB0 */
#define ATOM_UBFI1_OFFSET               (ARM11_UBFI2_SIZE               - sizeof(unsigned int ))  /* 0x1FAC */
#define ATOM_UBFI1_SIZE                 (ATOM_UBFI1_OFFSET              - sizeof(unsigned int ))  /* 0x1FA8 */
#define ATOM_UBFI2_OFFSET               (ATOM_UBFI1_SIZE                - sizeof(unsigned int ))  /* 0x1FA4 */
#define ATOM_UBFI2_SIZE                 (ATOM_UBFI2_OFFSET              - sizeof(unsigned int ))  /* 0x1FA0 */
#define ARM11_KERNEL_1_EMMC_PARTITION   (ATOM_UBFI2_SIZE                - sizeof(unsigned char))  /* 0x1F9F */
#define ARM11_KERNEL_2_EMMC_PARTITION   (ARM11_KERNEL_1_EMMC_PARTITION  - sizeof(unsigned char))  /* 0x1F9E */
#define ARM11_ROOTFS_1_EMMC_PARTITION   (ARM11_KERNEL_2_EMMC_PARTITION  - sizeof(unsigned char))  /* 0x1F9D */
#define ARM11_ROOTFS_2_EMMC_PARTITION   (ARM11_ROOTFS_1_EMMC_PARTITION  - sizeof(unsigned char))  /* 0x1F9C */
#define ARM11_GW_FS_1_EMMC_PARTITION    (ARM11_ROOTFS_2_EMMC_PARTITION  - sizeof(unsigned char))  /* 0x1F9B */
#define ARM11_GW_FS_2_EMMC_PARTITION    (ARM11_GW_FS_1_EMMC_PARTITION   - sizeof(unsigned char))  /* 0x1F9A */
#define ARM11_NVRAM_EMMC_PARTITION      (ARM11_GW_FS_2_EMMC_PARTITION   - sizeof(unsigned char))  /* 0x1F99 */
#define ATOM_KERNEL_1_EMMC_PARTITION    (ARM11_NVRAM_EMMC_PARTITION     - sizeof(unsigned char))  /* 0x1F98 */
#define ATOM_KERNEL_2_EMMC_PARTITION    (ATOM_KERNEL_1_EMMC_PARTITION   - sizeof(unsigned char))  /* 0x1F97 */
#define ATOM_ROOTFS_1_EMMC_PARTITION    (ATOM_KERNEL_2_EMMC_PARTITION   - sizeof(unsigned char))  /* 0x1F96 */
#define ATOM_ROOTFS_2_EMMC_PARTITION    (ATOM_ROOTFS_1_EMMC_PARTITION   - sizeof(unsigned char))  /* 0x1F95 */
#define ARM11_NVRAM_2_EMMC_PARTITION    (ATOM_ROOTFS_2_EMMC_PARTITION   - sizeof(unsigned char))  /* 0x1F94 */
#define ACTIVE_AID                      (ARM11_NVRAM_2_EMMC_PARTITION   - sizeof(unsigned int ))  /* 0x1F90 */
#define SILICON_STEPPING                (ACTIVE_AID                     - sizeof(unsigned int ))  /* 0x1F8C */
#define CEFDK_VERSION                   (SILICON_STEPPING               - sizeof(unsigned int ))  /* 0x1F88 */
#define SIGNATURE1_OFFSET               (CEFDK_VERSION                  - sizeof(unsigned int ))  /* 0x1F84 */
#define EMMC_FLASH_SIZE                 (SIGNATURE1_OFFSET              - sizeof(unsigned int ))  /* 0x1F80 */
#define SIGNATURE2_OFFSET               (EMMC_FLASH_SIZE                - sizeof(unsigned int ))  /* 0x1F7C */
#define SIGNATURE_SIZE                  (SIGNATURE2_OFFSET              - sizeof(unsigned int ))  /* 0x1F78 */
#define SIGNATURE_NUMBER                (SIGNATURE_SIZE                 - sizeof(unsigned int ))  /* 0x1F74 */
#define CEFDK_S1_OFFSET                 (SIGNATURE_NUMBER               - sizeof(unsigned int ))  /* 0x1F70 */
#define CEFDK_S1_SIZE                   (CEFDK_S1_OFFSET                - sizeof(unsigned int ))  /* 0x1F6C */
#define CEFDK_S2_OFFSET                 (CEFDK_S1_SIZE                  - sizeof(unsigned int ))  /* 0x1F68 */
#define CEFDK_S2_SIZE                   (CEFDK_S2_OFFSET                - sizeof(unsigned int ))  /* 0x1F64 */
#define CEFDK_S3_OFFSET                 (CEFDK_S2_SIZE                  - sizeof(unsigned int ))  /* 0x1F60 */
#define CEFDK_S3_SIZE                   (CEFDK_S3_OFFSET                - sizeof(unsigned int ))  /* 0x1F5C */
#define CEFDK_S1H_OFFSET                (CEFDK_S3_SIZE                  - sizeof(unsigned int ))  /* 0x1F58 */
#define CEFDK_S1H_SIZE                  (CEFDK_S1H_OFFSET               - sizeof(unsigned int ))  /* 0x1F54 */
#define CEFDK_S2H_OFFSET                (CEFDK_S1H_SIZE                 - sizeof(unsigned int ))  /* 0x1F50 */
#define CEFDK_S2H_SIZE                  (CEFDK_S2H_OFFSET               - sizeof(unsigned int ))  /* 0x1F4C */
#define CEFDK_S3H_OFFSET                (CEFDK_S2H_SIZE                 - sizeof(unsigned int ))  /* 0x1F48 */
#define CEFDK_S3H_SIZE                  (CEFDK_S3H_OFFSET               - sizeof(unsigned int ))  /* 0x1F44 */
#define AEP_MODE                        (CEFDK_S3H_SIZE                 - sizeof(unsigned int ))  /* 0x1F40 */
#define AIDIDX_APP_KERNEL               (AEP_MODE                       - sizeof(unsigned char )) /* 0x1F3F */
#define AIDIDX_APP_ROOT_FS              (AIDIDX_APP_KERNEL              - sizeof(unsigned char )) /* 0x1F3E */
#define AIDIDX_APP_VGW_FS               (AIDIDX_APP_ROOT_FS             - sizeof(unsigned char )) /* 0x1F3D */
#define AIDIDX_NP_KERNEL                (AIDIDX_APP_VGW_FS              - sizeof(unsigned char )) /* 0x1F3C */
#define AIDIDX_NP_ROOT_FS               (AIDIDX_NP_KERNEL               - sizeof(unsigned char )) /* 0x1F3B */
#define AIDIDX_NP_GW_FS                 (AIDIDX_NP_ROOT_FS              - sizeof(unsigned char )) /* 0x1F3A */
#define AIDIDX_RSVD_6                   (AIDIDX_NP_GW_FS                - sizeof(unsigned char )) /* 0x1F39 */
#define AIDIDX_RSVD_7                   (AIDIDX_RSVD_6                  - sizeof(unsigned char )) /* 0x1F38 */
#define AIDIDX_RSVD_8                   (AIDIDX_RSVD_7                  - sizeof(unsigned char )) /* 0x1F37 */
#define AIDIDX_RSVD_9                   (AIDIDX_RSVD_8                  - sizeof(unsigned char )) /* 0x1F36 */
#define AIDIDX_RSVD_10                  (AIDIDX_RSVD_9                  - sizeof(unsigned char )) /* 0x1F35 */
#define AIDIDX_RSVD_11                  (AIDIDX_RSVD_10                 - sizeof(unsigned char )) /* 0x1F34 */
#define AIDIDX_RSVD_12                  (AIDIDX_RSVD_11                 - sizeof(unsigned char )) /* 0x1F33 */
#define AIDIDX_RSVD_13                  (AIDIDX_RSVD_12                 - sizeof(unsigned char )) /* 0x1F32 */
#define AIDIDX_RSVD_14                  (AIDIDX_RSVD_13                 - sizeof(unsigned char )) /* 0x1F31 */
#define AIDIDX_RSVD_15                  (AIDIDX_RSVD_14                 - sizeof(unsigned char )) /* 0x1F30 */
#define BOARD_REVISION                  (AIDIDX_RSVD_15                 - sizeof(unsigned int ))  /* 0x1F2C */
#define NPCPU_ONLY_MODE                 (BOARD_REVISION                 - sizeof(unsigned int ))  /* 0x1F28 */
#define BOOT_PARAMS_BOTTOM_OFFSET       (NPCPU_ONLY_MODE                - sizeof(unsigned int ))  /* 0x1F24 */

/* Silicon stepping Id
   Lower 4 bits of Stepping ID 
   Stepping ID {4'bxxyy}
   xx = Major Stepping {00=A, 01=B, 10=C, 11=D}
   yy = Minor Stepping {00 = 0, 01 = 1, 10 = 2, 11 = 3}
   The max stepping ID of silicon is D3                   */
enum SiliconStepping {
    SILICON_STEPPING_ID_A_0 = 0x0,
    SILICON_STEPPING_ID_A_1 = 0x1,
    SILICON_STEPPING_ID_A_2 = 0x2,
    SILICON_STEPPING_ID_A_3 = 0x3,
    SILICON_STEPPING_ID_B_0 = 0x4,
    SILICON_STEPPING_ID_B_1 = 0x5,
    SILICON_STEPPING_ID_B_2 = 0x6,
    SILICON_STEPPING_ID_B_3 = 0x7,
    SILICON_STEPPING_ID_C_0 = 0x8,
    SILICON_STEPPING_ID_C_1 = 0x9,
    SILICON_STEPPING_ID_C_2 = 0xA,
    SILICON_STEPPING_ID_C_3 = 0xB,
    SILICON_STEPPING_ID_D_0 = 0xC,
    SILICON_STEPPING_ID_D_1 = 0xD,
    SILICON_STEPPING_ID_D_2 = 0xE,
    SILICON_STEPPING_ID_D_3 = 0xF
};
typedef enum SiliconStepping SiliconSteppingId_t;

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
#define BOOT_MODE_SPI            (0)
#define BOOT_MODE_eMMC           (1)
#define BOOT_MODE_SKIP_LOADING   (2)

/* Data definitions for ARM Boot Status */
#define ARM_STAT_BOOT_RAM_START    0x00000001
#define ARM_STAT_BOOT_RAM_END      0x00000002
#define ARM_STAT_BOOT_UBOOT_START  0x00000004
#define ARM_STAT_BOOT_UBOOT_END    0x00000008
#define ARM_STAT_BOOT_KERNEL_START 0x00000010
 
/* Data definitions for board types */
#define PUMA6_UNKNOWN_BOARD_ID    (0x0) /* ERROR          */ 
#define PUMA6_HP_BOARD_ID         (0x1) /* harborpark     */ 
#define PUMA6_HP_MG_BOARD_ID      (0x2) /* harborpark-mg  */ 
#define PUMA6_FM_BOARD_ID         (0x3) /* falconmine     */ 
#define PUMA6_CI_BOARD_ID         (0x4) /* cat-island     */ 
#define PUMA6_GS_BOARD_ID         (0x5) /* golden-springs */
#define PUMA6_CR_BOARD_ID         (0x6) /* cat-river      */
#define PUMA6_BL_BOARD_ID         (0x7) /* baker-lake     */
#define PUMA6_AP_BOARD_ID         (0x8) /* apollopark     */ 
#define PUMA6_AP_MG_BOARD_ID      (0x9) /* apollopark-mg  */


#define PUMA6_FM_BOARD_NAME       "falconmine"
#define PUMA6_CI_BOARD_NAME       "catisland"
#define PUMA6_HP_BOARD_NAME       "harborpark"
#define PUMA6_HP_MG_BOARD_NAME    "harborpark-mg"
#define PUMA6_GS_BOARD_NAME       "golden-springs"
#define PUMA6_CR_BOARD_NAME       "cat-river"
#define PUMA6_BL_BOARD_NAME       "baker-lake"
#define PUMA6_AP_BOARD_NAME       "apollopark"
#define PUMA6_AP_MG_BOARD_NAME    "apollopark-mg"

/* Data definitions for board revision */
#define PUMA6_FM_20_BOARD_REVISION_ID   (20) /* FM 2.0 board rev*/
#define PUMA6_GS_20_BOARD_REVISION_ID   (20) /* GS 2.0 board rev*/

/* Data definitions for AEP_MODE */
#define AEP_MODE_NOT_SUPPORT    0x00000000
#define AEP_MODE_NOT_ACTIVE     0x00000001
#define AEP_MODE_ACTIVE         0x00000002

/*Data definitions for NPCPU_ONLY mode*/
#define NPCPU_ONLY_DISABLE		0x00000000
#define NPCPU_ONLY_ENABLE		0x00000001

#endif /*_DOCSIS_IP_BOOT_PARAMS_H_*/

