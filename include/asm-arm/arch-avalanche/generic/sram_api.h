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

  Copyright(c) 2013-2017 Intel Corporation. All rights reserved.

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


/*------------------------------------------------------------------------------
* File Name: sram_api.h
* Description:
* SRAM internal memory device common type declarations
*------------------------------------------------------------------------------
*/

#ifndef  SRAM_API_H
#define  SRAM_API_H
/**************************************************************************/
/*      INCLUDES:                                                         */
/**************************************************************************/
#include <linux/ioctl.h>

#include "_tistdtypes.h"
#if PUMA6_SOC_TYPE
#include <docsis_ip_boot_params.h>
#endif

#if PUMA7_SOC_TYPE
#include <docsis_ip_boot_params.h>
#endif

/**************************************************************************/
/*      INTERFACE  DEFINES AND STRUCTS                                    */
/**************************************************************************/
#define SRAM_DEV_NAME  "/dev/sram_drv0"
#define SRAM_BOOTRAM_PROC_NAME "bootparams"



#define GET_SRAM_ADDR(offset)          (AVALANCHE_SRAM_BASE  + offset)

/* This enum defines the ids sections in the SRAM, when adding a new section need to add an enum for it */
typedef enum
{
    BOOT_RAM_SECTION_ID,
    SPARE_SECTION_ID,
    BBU_SECTION_ID,
    MAILBOX_SECTION_ID,
    BOOT_RAM_DEBUG_SECTION_ID,
    BOOT_PARAMS_SECTION_ID,
    MAX_SECTIONS

} SectionId_e;

/* This enum defines the ids of each boot param, when adding a new boot param need to add an enum for it  */
#if PUMA7_SOC_TYPE

typedef enum
{
    BOOT_PARAM_VER_ID,
    NP_CPU_BOOT_STATUS_ID,
    SILICON_STEPPING_ID,
    APP_CPU_BIOS_VERSION_ID,
    BOOT_MODE_ID,
    BOARD_TYPE_ID,
    BOARD_REVISION_ID,
    NP_CPU_DDR_OFFSET_ID,
    NP_CPU_DDR_SIZE_ID,
    NP_CPU_OS_OFFSET_ID,
    NP_CPU_OS_SIZE_ID,
    NP_CPU_OS_COMMANDLINE_ADDR_ID,
    NP_CPU_OS_COMMANDLINE_SIZE_ID,
    DEBUG_BOOT_LOADER_OFFSET_ID,
    DEBUG_PARAMETERS_ADDR_ID,
    NPCPU_ONLY_MODE_ID,
    RESET_TYPE_ID,
    RESET_REASON_ID,
    PLATFORM_TYPE_ID,
    SHARED_DDR_OFFSET_ID,
    SHARED_DDR_SIZE_ID,
    RESET_BUTTON_DURATION_ID,
    SECURE_BOOT_STATUS_ID,
    MAX_BOOT_PARAMS
} BootParamId_e;

#endif

#if PUMA6_SOC_TYPE

typedef enum
{
    BOOT_PARAM_VER_ID,
    NP_CPU_BOOT_STATUS_ID,
    BOOT_MODE_ID,
    BOARD_TYPE_ID,
    NUMBER_OF_FLASHES_ID,
    NP_CPU_DDR_OFFSET_ID,
    NP_CPU_DDR_SIZE_ID,
    ACTIVE_AID_ID,
    AID_1_OFFSET_ID,
    AID_2_OFFSET_ID,
    UBOOT_OFFSET_ID,
    UBOOT_SIZE_ID,
    UBOOT_ENV1_OFFSET_ID,
    UBOOT_ENV2_OFFSET_ID,
    UBOOT_ENV_SIZE_ID,
    NP_CPU_NVRAM_OFFSET_ID,
    NP_CPU_NVRAM_SIZE_ID,
    NP_CPU_UBFI1_OFFSET_ID,
    NP_CPU_UBFI1_SIZE_ID,
    NP_CPU_UBFI2_OFFSET_ID,
    NP_CPU_UBFI2_SIZE_ID,
    APP_CPU_UBFI1_OFFSET_ID,
    APP_CPU_UBFI1_SIZE_ID,
    APP_CPU_UBFI2_OFFSET_ID,
    APP_CPU_UBFI2_SIZE_ID,
    NP_CPU_KERNEL_1_PARTITION_ID,
    NP_CPU_KERNEL_2_PARTITION_ID,
    NP_CPU_ROOTFS_1_PARTITION_ID,
    NP_CPU_ROOTFS_2_PARTITION_ID,
    NP_CPU_GW_FS_1_PARTITION_ID,
    NP_CPU_GW_FS_2_PARTITION_ID,
    NP_CPU_NVRAM_1_PARTITION_ID,
    NP_CPU_NVRAM_2_PARTITION_ID,
    APP_CPU_KERNEL_1_PARTITION_ID,
    APP_CPU_KERNEL_2_PARTITION_ID,
    APP_CPU_ROOTFS_1_PARTITION_ID,
    APP_CPU_ROOTFS_2_PARTITION_ID,
    SILICON_STEPPING_ID,
    CEFDK_VERSION_ID,
    SIGNATURE1_OFFSET_ID,
    SIGNATURE2_OFFSET_ID,
    SIGNATURE_SIZE_ID,
    SIGNATURE_NUMBER_ID,
    EMMC_FLASH_SIZE_ID,
    CEFDK_S1_OFFSET_ID,
    CEFDK_S1_SIZE_ID,
    CEFDK_S2_OFFSET_ID,
    CEFDK_S2_SIZE_ID,
    CEFDK_S3_OFFSET_ID,
    CEFDK_S3_SIZE_ID,
    CEFDK_S1H_OFFSET_ID,
    CEFDK_S1H_SIZE_ID,
    CEFDK_S2H_OFFSET_ID,
    CEFDK_S2H_SIZE_ID,
    CEFDK_S3H_OFFSET_ID,
    CEFDK_S3H_SIZE_ID,
    AEP_MODE_ID,
    AIDIDX_APP_KERNEL_ID,
    AIDIDX_APP_ROOT_FS_ID,
    AIDIDX_APP_VGW_FS_ID,
    AIDIDX_NP_KERNEL_ID,
    AIDIDX_NP_ROOT_FS_ID,
    AIDIDX_NP_GW_FS_ID,
    AIDIDX_RSVD_6_ID,
    AIDIDX_RSVD_7_ID,
    AIDIDX_RSVD_8_ID,
    AIDIDX_RSVD_9_ID,
    AIDIDX_RSVD_10_ID,
    AIDIDX_RSVD_11_ID,
    AIDIDX_RSVD_12_ID,
    AIDIDX_RSVD_13_ID,
    AIDIDX_RSVD_14_ID,
    AIDIDX_RSVD_15_ID,
    BOARD_REVISION_ID,
    NPCPU_ONLY_MODE_ID,
    MAX_BOOT_PARAMS
} BootParamId_e;

#endif



/* This structer is used for the ioctl boot param args to read the boot param value to user space from this driver. */
typedef struct
{

    BootParamId_e       bootParamId;
    Uint32              bootParamVal;

}sram_boot_params_args_t;

/* This structer is used for the ioctl sections args to read and write from sections in the sram */
typedef struct
{
    SectionId_e         sectionId;
    Uint32              sectionOffset;
    Uint32              sectionOffsetVal;
    Uint8               *buff;
    Uint32              buffSize;
}sram_section_args_t;


/* Sections addr  and size */
#if PUMA7_SOC_TYPE

#define    BOOT_RAM_SECTION_OFFSET             0xFFFF0000
#define    BOOT_RAM_SECTION_SIZE               0x1C00

#define    SPARE_SECTION_OFFSET                0xFFFF1C00
#define    SPARE_SECTION_SIZE                  0x2200

#define    MAILBOX_SECTION_OFFSET              0xFFFF3E00
#define    MAILBOX_SECTION_SIZE                0x100

#define    BOOT_RAM_DEBUG_SECTION_OFFSET       0xFFFF3F00
#define    BOOT_RAM_DEBUG_SECTION_SIZE         0x10

#define    BOOT_PARAMS_SECTION_OFFSET          0xFFFF3F10
#define    BOOT_PARAMS_SECTION_SIZE            0xF0

#endif

#if PUMA6_SOC_TYPE

#define    BOOT_RAM_SECTION_OFFSET             0x0000
#define    BOOT_RAM_SECTION_SIZE               0x1C00

#define    SPARE_SECTION_OFFSET                0x1C00
#define    SPARE_SECTION_SIZE                  0x100

#define    BBU_SECTION_OFFSET                  0x1D00
#define    BBU_SECTION_SIZE                    0x100

#define    MAILBOX_SECTION_OFFSET              0x1E00
#define    MAILBOX_SECTION_SIZE                0x100

#define    BOOT_RAM_DEBUG_SECTION_OFFSET       0x1F00
#define    BOOT_RAM_DEBUG_SECTION_SIZE         0x10

#define    BOOT_PARAMS_SECTION_OFFSET          0x1F10
#define    BOOT_PARAMS_SECTION_SIZE            0xF0

#endif



#ifdef __KERNEL__
/**************************************************************************/
/*! \fn Int32 SRAM_Get_Boot_Param(BootParamId_e bootParamId, Uint32 *paramVal)
 **************************************************************************
 *  \brief This function get the boot param id and returns its value
 *  \param[in] bootParamId: The boot param id
 *  \param[out] paramVal: The bbot param value
 *  \return SRAM_OK if reading succeed
 **************************************************************************/
Int32 SRAM_Get_Boot_Param(BootParamId_e bootParamId, Uint32 *paramVal);

/***********************************************************************************************************/
/*! \fn Int32 SRAM_Read_From_Section_At_Offset (SectionId_e sectionId, Uint32 sectionOffset, Uint32 *sectionOffsetVal )
 ***********************************************************************************************************
 *  \brief  This function reads to Uint32 from the wanted section in the wanted offset.
 *  \param[in] sectionId       : The section id
 *  \param[in] sectionOffset   : The section offset
 *  \param[out]sectionOffsetVal: The reading value
 *  \return SRAM_OK \SRAM_FAIL
 ***********************************************************************************************************/
Int32 SRAM_Read_From_Section_At_Offset (SectionId_e sectionId, Uint32 sectionOffset, Uint32 *sectionOffsetVal );

/*********************************************************************************************************/
/*! \fn Int32 SRAM_Write_To_Section_At_Offset (SectionId_e sectionId, Uint32 sectionOffset, Uint32 sectionOffsetVal)
 *********************************************************************************************************
 *  \brief  This function writes the wanted value to the wanted section in the wanted offset.
 *  \param[in] sectionId       : The section id
 *  \param[in] sectionOffset   : The section offset
 *  \param[in]sectionOffsetVal: The writing value
 *  \return SRAM_OK \SRAM_FAIL : SRAM_OK if writing succeed otherwise return SRAM_FAIL
 ********************************************************************************************************/
Int32 SRAM_Write_To_Section_At_Offset(SectionId_e sectionId, Uint32 sectionOffset, Uint32 sectionOffsetVal);
#endif

#define SRAM_DRIVER_ID                    (0xE2)

#define SRAM_GET_BOOT_PARAM             _IOWR(SRAM_DRIVER_ID, 1, sram_boot_params_args_t *)
#define SRAM_READ_SECTION_OFFSET        _IOWR(SRAM_DRIVER_ID, 2, sram_section_args_t *)
#define SRAM_WRITE_SECTION_OFFSET       _IOW(SRAM_DRIVER_ID,  3, sram_section_args_t *)


#endif

