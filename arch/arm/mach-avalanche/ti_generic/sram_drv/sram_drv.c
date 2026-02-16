/*
 *
 * sram_drv.c
 * Description:
 * internal memory SRAM device driver
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2013-2017 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */
/*! \file sram_driver.c
    \brief Implementation of SRAM internal memory Driver. 
    \code.
*/
/**************************************************************************/
/*      INCLUDES:                                                       */
/**************************************************************************/
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/version.h>
#include <linux/module.h>   /* for modules */
#include <linux/fs.h>       /* file_operations */
#include <linux/uaccess.h>  /* copy_(to,from)_user */
#include <linux/init.h>     /* module_init, module_exit */
#include <linux/cdev.h>     /* cdev utilities */
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/semaphore.h>
#include <linux/ioctl.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "sram_api.h"
#ifdef CONFIG_MACH_PUMA6
#include <asm-arm/arch-avalanche/puma6/puma6.h>
#endif
#ifdef CONFIG_MACH_PUMA7
#include <asm-arm/arch-avalanche/puma7/puma7.h>
#endif

/**************************************************************************/
/*      LOCAL DECLARATIONS:                                               */
/**************************************************************************/
static int proc_snprintf(char **page, int *page_size, int file_offset, int *curr_offset, const char *format, ...);
static Int32 is_opertion_allowed(SectionId_e sectionId, Uint32 sectionOffset, Uint32 size);
static int sram_bootparams_read_proc(struct seq_file *m, void *v);
static long sram_ioctl(struct file *fd, unsigned int cmd, unsigned long arg);

/**************************************************************************/
/*      DEFINES AND STRUCTS                                               */
/**************************************************************************/

#define FIRST_MINOR 0
#define MINOR_CNT 1

#define SRAM_OK                         (0)
#define SRAM_FAIL                       (-1)

#if 0
#ifdef CONFIG_MACH_PUMA6
#define BOOT_PARAMS_TOP_ADDR           0x2000      /* Boot Params top address id fix to 8KB */
#endif

#ifdef CONFIG_MACH_PUMA7
#define BOOT_PARAMS_TOP_ADDR           0x4000      /* Boot Params top address id fix to Puma 7  */
#endif
#endif

#define BOOT_PARAM_MAX_NAME             65         /* The max length allowed for the parameter name. */
#define SECTION_MAX_NAME                65

/* Sections names */
#define    BOOT_PARAMS_SECTION_NAME            "BOOT_PARAMS_SECTION"             
#define    BOOT_RAM_DEBUG_SECTION_NAME         "BOOT_RAM_DEBUG_SECTION"
#define    MAILBOX_SECTION_NAME                "NP/APP CPU MAILBOX_SECTION"
#define    BBU_SECTION_NAME                    "BBU_SECTION"
#define    SPARE_SECTION_NAME                  "SPARE_SECTION"
#define    BOOT_RAM_SECTION_NAME               "BOOT_RAM_SECTION"

/* Boot Params names*/
#define PROC_BP_BOOT_PARAM_VERSION              "  Boot Params Version .......... "
#define PROC_BP_NP_CPU_BOOT_STATUS              "  NP_CPU Boot Status ........... "
#define PROC_BP_BOOT_MODE                       "  Boot Mode .................... "
#define PROC_BP_BOARD_TYPE                      "  Board Type ................... "
#define PROC_BP_BOARD_REVISION                  "  BOARD REVISION ............... "
#define PROC_BP_NPCPU_ONLY_MODE                 "  NPCPU ONLY MODE .............. "
#define PROC_BP_SILICON_STEPPING                "  Silicon stepping ............. "
#define PROC_BP_RAM_OFFSET                      "  RAM Offset ................... "
#define PROC_BP_RAM_SIZE                        "  RAM Size ..................... "

#ifdef CONFIG_MACH_PUMA6
#define PROC_BP_NUMBER_OF_FLASHES               "  Numebr of flashes ............ "
#define PROC_BP_ACTIVE_AID                      "  Active AID  .................. "
#define PROC_BP_AID_1_OFFSET                    "  AID 1 Offset ................. "
#define PROC_BP_AID_2_OFFSET                    "  AID 2 Offset ................. "
#define PROC_BP_UBOOT_OFFSET                    "  Uboot Offset ................. "
#define PROC_BP_UBOOT_SIZE                      "  Uboot Size ................... "
#define PROC_BP_UBOOT_ENV1_OFFSET               "  Uboot Env1 Offset ............ "
#define PROC_BP_UBOOT_ENV2_OFFSET               "  Uboot Env2 Offset ............ "
#define PROC_BP_UBOOT_ENV_SIZE                  "  Uboot Env Size ............... "
#define PROC_BP_NP_CPU_NVRAM_OFFSET             "  NP CPU NVRAM Offset .......... "
#define PROC_BP_NP_CPU_NVRAM_SIZE               "  NP CPU NVRAM Size ............ "
#define PROC_BP_NP_CPU_UBFI1_OFFSET             "  NP CPU UBFI1 Offset .......... "
#define PROC_BP_NP_CPU_UBFI1_SIZE               "  NP CPU UBFI1 Size ............ "
#define PROC_BP_NP_CPU_UBFI2_OFFSET             "  NP CPU UBFI2 Offset .......... "
#define PROC_BP_NP_CPU_UBFI2_SIZE               "  NP CPU UBFI2 Size ............ "
#define PROC_BP_APP_CPU_UBFI1_OFFSET            "  APP CPU UBFI1 Offset ......... "
#define PROC_BP_APP_CPU_UBFI1_SIZE              "  APP CPU UBFI1 Size ........... "
#define PROC_BP_APP_CPU_UBFI2_OFFSET            "  APP CPU UBFI2 Offset ......... "
#define PROC_BP_APP_CPU_UBFI2_SIZE              "  APP CPU UBFI2 Size ........... "
#define PROC_BP_NP_CPU_KERNEL_1_PARTITION       "  NP CPU Kernel 1 partition .... "
#define PROC_BP_NP_CPU_KERNEL_2_PARTITION       "  NP CPU Kernel 2 partition .... "
#define PROC_BP_NP_CPU_ROOT_FS_1_PARTITION      "  NP CPU Root FS 1 partition ... "
#define PROC_BP_NP_CPU_ROOT_FS_2_PARTITION      "  NP CPU Root FS 2 partition ... "
#define PROC_BP_NP_CPU_GW_FS_1_PARTITION        "  NP CPU GW FS 1 partition ..... "
#define PROC_BP_NP_CPU_GW_FS_2_PARTITION        "  NP CPU GW FS 2 partition ..... "
#define PROC_BP_NP_CPU_NVRAM_1_PARTITION        "  NP CPU NVRAM 1 partition ..... "
#define PROC_BP_NP_CPU_NVRAM_2_PARTITION        "  NP CPU NVRAM 2 partition ..... "
#define PROC_BP_APP_CPU_KERNEL_1_PARTITION      "  APP CPU Kernel 1 partition ... "
#define PROC_BP_APP_CPU_KERNEL_2_PARTITION      "  APP CPU Kernel 2 partition ... "
#define PROC_BP_APP_CPU_ROOT_FS_1_PARTITION     "  APP CPU Root FS 1 partition .. "
#define PROC_BP_APP_CPU_ROOT_FS_2_PARTITION     "  APP CPU Root FS 2 partition .. "
#define PROC_BP_CEFDK_VERSION                   "  CEFDK version ................ "
#define PROC_BP_SIGNATURE1_OFFSET               "  Signature 1 Offset ........... "
#define PROC_BP_SIGNATURE2_OFFSET               "  Signature 2 Offset ........... "
#define PROC_BP_SIGNATURE_SIZE                  "  Signature Size ............... "
#define PROC_BP_SIGNATURE_NUMBER                "  Signature number ............. "
#define PROC_BP_EMMC_FLASH_SIZE                 "  EMMC flash Size .............. "
#define PROC_BP_CEFDK_S1_OFFSET                 "  CEFDK S1 Offset .............. "
#define PROC_BP_CEFDK_S1_SIZE                   "  CEFDK S1 Size ................ "
#define PROC_BP_CEFDK_S2_OFFSET                 "  CEFDK S2 Offset .............. "
#define PROC_BP_CEFDK_S2_SIZE                   "  CEFDK S2 Size ................ "
#define PROC_BP_CEFDK_S3_OFFSET                 "  CEFDK S3 Offset .............. "
#define PROC_BP_CEFDK_S3_SIZE                   "  CEFDK S3 Size ................ "
#define PROC_BP_CEFDK_S1H_OFFSET                "  CEFDK S1H Offset ............. "
#define PROC_BP_CEFDK_S1H_SIZE                  "  CEFDK S1H Size ............... "
#define PROC_BP_CEFDK_S2H_OFFSET                "  CEFDK S2H Offset ............. "
#define PROC_BP_CEFDK_S2H_SIZE                  "  CEFDK S2H Size ............... "
#define PROC_BP_CEFDK_S3H_OFFSET                "  CEFDK S3H Offset ............. "
#define PROC_BP_CEFDK_S3H_SIZE                  "  CEFDK S3H Size ............... "
#define PROC_BP_AEP_MODE                        "  AEP MODE ..................... "
#define PROC_BP_AIDIDX_APP_KERNEL               "  AIDIDX APP KERNEL ............ "
#define PROC_BP_AIDIDX_APP_ROOT_FS              "  AIDIDX APP ROOT FS ........... "
#define PROC_BP_AIDIDX_APP_VGW_FS               "  AIDIDX APP VGW FS ............ "
#define PROC_BP_AIDIDX_NP_KERNEL                "  AIDIDX NP KERNEL ............. "
#define PROC_BP_AIDIDX_NP_ROOT_FS               "  AIDIDX NP ROOT FS ............ "
#define PROC_BP_AIDIDX_NP_GW_FS                 "  AIDIDX NP GW FS .............. "
#define PROC_BP_AIDIDX_RSVD_6                   "  AIDIDX RSVD 6 ................ "
#define PROC_BP_AIDIDX_RSVD_7                   "  AIDIDX RSVD 7 ................ "
#define PROC_BP_AIDIDX_RSVD_8                   "  AIDIDX RSVD 8 ................ "
#define PROC_BP_AIDIDX_RSVD_9                   "  AIDIDX RSVD 9 ................ "
#define PROC_BP_AIDIDX_RSVD_10                  "  AIDIDX RSVD 10 ............... "
#define PROC_BP_AIDIDX_RSVD_11                  "  AIDIDX RSVD 11 ............... "
#define PROC_BP_AIDIDX_RSVD_12                  "  AIDIDX RSVD 12 ............... "
#define PROC_BP_AIDIDX_RSVD_13                  "  AIDIDX RSVD 13 ............... "
#define PROC_BP_AIDIDX_RSVD_14                  "  AIDIDX RSVD 14 ............... "
#define PROC_BP_AIDIDX_RSVD_15                  "  AIDIDX RSVD 15 ............... "
#endif

#ifdef CONFIG_MACH_PUMA7
#define PROC_BP_APP_CPU_BIOS_VERSION            "  APP-CPU BIOS version.......... "
#define PROC_BP_NP_CPU_OS_OFFSET                "  ARM11 OS offset .............. "
#define PROC_BP_NP_CPU_OS_SIZE                  "  ARM11 OS size ................ "
#define PROC_BP_NP_CPU_OS_COMMANDLINE_ADDR      "  ARM11 OS command line address  "
#define PROC_BP_NP_CPU_OS_COMMANDLINE_SIZE      "  ARM11 OS command line size ... "
#define PROC_BP_DEBUG_BOOT_LOADER_OFFSET        "  Debug boot loader offset ..... "
#define PROC_BP_DEBUG_PARAMETERS_ADDR           "  Debug parameters address ..... "
#define PROC_BP_RESET_TYPE                      "  Reset Type ................... "
#define PROC_BP_RESET_REASON                    "  Reset Reason ................. "
#define PROC_BP_PLATFORM_TYPE                   "  Platform Type ................ "
#define PROC_BP_SHARED_DDR_OFFSET               "  Shared DDR offset ............ "
#define PROC_BP_SHARED_DDR_SIZE                 "  Shared DDR size .............. "
#define PROC_BP_RESET_BUTTON_DURATION           "  Reset button duration ........ "
#define PROC_BP_SECURE_BOOT_STATUS              "  Secure Boot Status ........... "
#endif


#define READ_UINT32(addr)        (*((volatile unsigned int *)(addr)))
#define WRITE_UINT32(addr,val)   (*((volatile unsigned int *)(addr)) = (val))

/* This structure defines each section in the SRAM */
typedef struct
{
    Uint8           name[SECTION_MAX_NAME];
    Uint32          size;
    Uint32          addr;

}section_info_t;

/* This structure defines each parameter that is saved in the boot_param section in the SRAM */
typedef struct 
{
    Uint8            name[BOOT_PARAM_MAX_NAME];
    Uint32           size;
    Uint32           addr;
}boot_param_t;

/**************************************************************************/
/*      LOCAL VARIABLES:                                                  */
/**************************************************************************/
static dev_t dev;
static struct cdev *c_dev;
static struct class *cl;

static struct proc_dir_entry *procFile;

/* Structure to map driver functions to kernel */
static struct file_operations sram_drv_fops = 
{

    .owner   = THIS_MODULE,
    .unlocked_ioctl   = sram_ioctl
};

/* BootParams hold the boot parameters info. */
#ifdef CONFIG_MACH_PUMA6
static boot_param_t BootParams[MAX_BOOT_PARAMS] = 
{
    [BOOT_PARAM_VER_ID] =             {PROC_BP_BOOT_PARAM_VERSION,          sizeof(int), GET_SRAM_ADDR(BOOT_PARAM_VER)},                
    [NP_CPU_BOOT_STATUS_ID] =         {PROC_BP_NP_CPU_BOOT_STATUS,          sizeof(int), GET_SRAM_ADDR(ARM11_BOOT_STATUS)},             
    [BOOT_MODE_ID] =                  {PROC_BP_BOOT_MODE,                   sizeof(int), GET_SRAM_ADDR(BOOT_MODE)},                     
    [BOARD_TYPE_ID] =                 {PROC_BP_BOARD_TYPE,                  sizeof(int), GET_SRAM_ADDR(BOARD_TYPE)},                    
    [NUMBER_OF_FLASHES_ID] =          {PROC_BP_NUMBER_OF_FLASHES,           sizeof(int), GET_SRAM_ADDR(NUMBER_OF_FLASHES)},             
    [NP_CPU_DDR_OFFSET_ID] =          {PROC_BP_RAM_OFFSET,                  sizeof(int), GET_SRAM_ADDR(ARM11_DDR_OFFSET)},              
    [NP_CPU_DDR_SIZE_ID] =            {PROC_BP_RAM_SIZE,                    sizeof(int), GET_SRAM_ADDR(ARM11_DDR_SIZE)},                
    [ACTIVE_AID_ID] =                 {PROC_BP_ACTIVE_AID,                  sizeof(int), GET_SRAM_ADDR(ACTIVE_AID)},                    
    [AID_1_OFFSET_ID] =               {PROC_BP_AID_1_OFFSET,                sizeof(int), GET_SRAM_ADDR(AID_1_OFFSET)},                  
    [AID_2_OFFSET_ID] =               {PROC_BP_AID_2_OFFSET,                sizeof(int), GET_SRAM_ADDR(AID_2_OFFSET)},                  
    [UBOOT_OFFSET_ID] =               {PROC_BP_UBOOT_OFFSET,                sizeof(int), GET_SRAM_ADDR(ARM11_UBOOT_OFFSET)},            
    [UBOOT_SIZE_ID] =                 {PROC_BP_UBOOT_SIZE,                  sizeof(int), GET_SRAM_ADDR(ARM11_UBOOT_SIZE)},              
    [UBOOT_ENV1_OFFSET_ID] =          {PROC_BP_UBOOT_ENV1_OFFSET,           sizeof(int), GET_SRAM_ADDR(ARM11_ENV1_OFFSET)},             
    [UBOOT_ENV2_OFFSET_ID] =          {PROC_BP_UBOOT_ENV2_OFFSET,           sizeof(int), GET_SRAM_ADDR(ARM11_ENV2_OFFSET)},             
    [UBOOT_ENV_SIZE_ID] =             {PROC_BP_UBOOT_ENV_SIZE,              sizeof(int), GET_SRAM_ADDR(ARM11_ENV_SIZE)},                
    [NP_CPU_NVRAM_OFFSET_ID] =        {PROC_BP_NP_CPU_NVRAM_OFFSET,         sizeof(int), GET_SRAM_ADDR(ARM11_NVRAM_OFFSET)},            
    [NP_CPU_NVRAM_SIZE_ID] =          {PROC_BP_NP_CPU_NVRAM_SIZE,           sizeof(int), GET_SRAM_ADDR(ARM11_NVRAM_SIZE)},              
    [NP_CPU_UBFI1_OFFSET_ID] =        {PROC_BP_NP_CPU_UBFI1_OFFSET,         sizeof(int), GET_SRAM_ADDR(ARM11_UBFI1_OFFSET)},            
    [NP_CPU_UBFI1_SIZE_ID] =          {PROC_BP_NP_CPU_UBFI1_SIZE,           sizeof(int), GET_SRAM_ADDR(ARM11_UBFI1_SIZE)},              
    [NP_CPU_UBFI2_OFFSET_ID] =        {PROC_BP_NP_CPU_UBFI2_OFFSET,         sizeof(int), GET_SRAM_ADDR(ARM11_UBFI2_OFFSET)},            
    [NP_CPU_UBFI2_SIZE_ID] =          { PROC_BP_NP_CPU_UBFI2_SIZE,           sizeof(int), GET_SRAM_ADDR(ARM11_UBFI2_SIZE)},             
    [APP_CPU_UBFI1_OFFSET_ID] =       { PROC_BP_APP_CPU_UBFI1_OFFSET,        sizeof(int), GET_SRAM_ADDR(ATOM_UBFI1_OFFSET)},            
    [APP_CPU_UBFI1_SIZE_ID] =         { PROC_BP_APP_CPU_UBFI1_SIZE,          sizeof(int), GET_SRAM_ADDR(ATOM_UBFI1_SIZE)},              
    [APP_CPU_UBFI2_OFFSET_ID] =       { PROC_BP_APP_CPU_UBFI2_OFFSET,        sizeof(int), GET_SRAM_ADDR(ATOM_UBFI2_OFFSET)},            
    [APP_CPU_UBFI2_SIZE_ID] =         { PROC_BP_APP_CPU_UBFI2_SIZE,          sizeof(int), GET_SRAM_ADDR(ATOM_UBFI2_SIZE)},              
    [NP_CPU_KERNEL_1_PARTITION_ID] =  { PROC_BP_NP_CPU_KERNEL_1_PARTITION,   sizeof(char),GET_SRAM_ADDR(ARM11_KERNEL_1_EMMC_PARTITION)},
    [NP_CPU_KERNEL_2_PARTITION_ID] =  { PROC_BP_NP_CPU_KERNEL_2_PARTITION,   sizeof(char),GET_SRAM_ADDR(ARM11_KERNEL_2_EMMC_PARTITION)},
    [NP_CPU_ROOTFS_1_PARTITION_ID] =  { PROC_BP_NP_CPU_ROOT_FS_1_PARTITION,  sizeof(char),GET_SRAM_ADDR(ARM11_ROOTFS_1_EMMC_PARTITION)},
    [NP_CPU_ROOTFS_2_PARTITION_ID] =  { PROC_BP_NP_CPU_ROOT_FS_2_PARTITION,  sizeof(char),GET_SRAM_ADDR(ARM11_ROOTFS_2_EMMC_PARTITION)},
    [NP_CPU_GW_FS_1_PARTITION_ID] =   { PROC_BP_NP_CPU_GW_FS_1_PARTITION,    sizeof(char),GET_SRAM_ADDR(ARM11_GW_FS_1_EMMC_PARTITION)}, 
    [NP_CPU_GW_FS_2_PARTITION_ID] =   { PROC_BP_NP_CPU_GW_FS_2_PARTITION,    sizeof(char),GET_SRAM_ADDR(ARM11_GW_FS_2_EMMC_PARTITION)}, 
    [NP_CPU_NVRAM_1_PARTITION_ID] =   { PROC_BP_NP_CPU_NVRAM_1_PARTITION,    sizeof(char),GET_SRAM_ADDR(ARM11_NVRAM_EMMC_PARTITION)},   
    [NP_CPU_NVRAM_2_PARTITION_ID] =   { PROC_BP_NP_CPU_NVRAM_2_PARTITION,    sizeof(char),GET_SRAM_ADDR(ARM11_NVRAM_2_EMMC_PARTITION)}, 
    [APP_CPU_KERNEL_1_PARTITION_ID] = { PROC_BP_APP_CPU_KERNEL_1_PARTITION,  sizeof(char),GET_SRAM_ADDR(ATOM_KERNEL_1_EMMC_PARTITION)}, 
    [APP_CPU_KERNEL_2_PARTITION_ID] = { PROC_BP_APP_CPU_KERNEL_2_PARTITION,  sizeof(char),GET_SRAM_ADDR(ATOM_KERNEL_2_EMMC_PARTITION)}, 
    [APP_CPU_ROOTFS_1_PARTITION_ID] = { PROC_BP_APP_CPU_ROOT_FS_1_PARTITION, sizeof(char),GET_SRAM_ADDR(ATOM_ROOTFS_1_EMMC_PARTITION)}, 
    [APP_CPU_ROOTFS_2_PARTITION_ID] = { PROC_BP_APP_CPU_ROOT_FS_2_PARTITION, sizeof(char),GET_SRAM_ADDR(ATOM_ROOTFS_2_EMMC_PARTITION)}, 
    [SILICON_STEPPING_ID] =           { PROC_BP_SILICON_STEPPING,            sizeof(int), GET_SRAM_ADDR(SILICON_STEPPING)},             
    [CEFDK_VERSION_ID] =              { PROC_BP_CEFDK_VERSION,               sizeof(int), GET_SRAM_ADDR(CEFDK_VERSION)},                
    [SIGNATURE1_OFFSET_ID] =          { PROC_BP_SIGNATURE1_OFFSET,           sizeof(int), GET_SRAM_ADDR(SIGNATURE1_OFFSET)},            
    [SIGNATURE2_OFFSET_ID] =          { PROC_BP_SIGNATURE2_OFFSET,           sizeof(int), GET_SRAM_ADDR(SIGNATURE2_OFFSET)},            
    [SIGNATURE_SIZE_ID] =             { PROC_BP_SIGNATURE_SIZE,              sizeof(int), GET_SRAM_ADDR(SIGNATURE_SIZE)},               
    [SIGNATURE_NUMBER_ID] =           { PROC_BP_SIGNATURE_NUMBER,            sizeof(int), GET_SRAM_ADDR(SIGNATURE_NUMBER)},             
    [EMMC_FLASH_SIZE_ID] =            { PROC_BP_EMMC_FLASH_SIZE,             sizeof(int), GET_SRAM_ADDR(EMMC_FLASH_SIZE)},              
    [CEFDK_S1_OFFSET_ID] =            { PROC_BP_CEFDK_S1_OFFSET,             sizeof(int), GET_SRAM_ADDR(CEFDK_S1_OFFSET)},              
    [CEFDK_S1_SIZE_ID] =              { PROC_BP_CEFDK_S1_SIZE,               sizeof(int), GET_SRAM_ADDR(CEFDK_S1_SIZE)},                
    [CEFDK_S2_OFFSET_ID] =            { PROC_BP_CEFDK_S2_OFFSET,             sizeof(int), GET_SRAM_ADDR(CEFDK_S2_OFFSET)},              
    [CEFDK_S2_SIZE_ID] =              { PROC_BP_CEFDK_S2_SIZE,               sizeof(int), GET_SRAM_ADDR(CEFDK_S2_SIZE)},                
    [CEFDK_S3_OFFSET_ID] =            { PROC_BP_CEFDK_S3_OFFSET,             sizeof(int), GET_SRAM_ADDR(CEFDK_S3_OFFSET)},              
    [CEFDK_S3_SIZE_ID] =              { PROC_BP_CEFDK_S3_SIZE,               sizeof(int), GET_SRAM_ADDR(CEFDK_S3_SIZE)},                
    [CEFDK_S1H_OFFSET_ID] =           { PROC_BP_CEFDK_S1H_OFFSET,            sizeof(int), GET_SRAM_ADDR(CEFDK_S1H_OFFSET)},             
    [CEFDK_S1H_SIZE_ID] =             { PROC_BP_CEFDK_S1H_SIZE,              sizeof(int), GET_SRAM_ADDR(CEFDK_S1H_SIZE)},               
    [CEFDK_S2H_OFFSET_ID] =           { PROC_BP_CEFDK_S2H_OFFSET,            sizeof(int), GET_SRAM_ADDR(CEFDK_S2H_OFFSET)},             
    [CEFDK_S2H_SIZE_ID] =             { PROC_BP_CEFDK_S2H_SIZE,              sizeof(int), GET_SRAM_ADDR(CEFDK_S2H_SIZE)},               
    [CEFDK_S3H_OFFSET_ID] =           { PROC_BP_CEFDK_S3H_OFFSET,            sizeof(int), GET_SRAM_ADDR(CEFDK_S3H_OFFSET)},             
    [CEFDK_S3H_SIZE_ID] =             { PROC_BP_CEFDK_S3H_SIZE,              sizeof(int), GET_SRAM_ADDR(CEFDK_S3H_SIZE)},               
    [AEP_MODE_ID] =                   { PROC_BP_AEP_MODE,                    sizeof(int), GET_SRAM_ADDR(AEP_MODE)},                     
    [AIDIDX_APP_KERNEL_ID] =          { PROC_BP_AIDIDX_APP_KERNEL,           sizeof(char),GET_SRAM_ADDR(AIDIDX_APP_KERNEL)},            
    [AIDIDX_APP_ROOT_FS_ID] =         { PROC_BP_AIDIDX_APP_ROOT_FS,          sizeof(char),GET_SRAM_ADDR(AIDIDX_APP_ROOT_FS)},           
    [AIDIDX_APP_VGW_FS_ID] =          { PROC_BP_AIDIDX_APP_VGW_FS,           sizeof(char),GET_SRAM_ADDR(AIDIDX_APP_VGW_FS)},            
    [AIDIDX_NP_KERNEL_ID] =           { PROC_BP_AIDIDX_NP_KERNEL,            sizeof(char),GET_SRAM_ADDR(AIDIDX_NP_KERNEL)},             
    [AIDIDX_NP_ROOT_FS_ID] =          { PROC_BP_AIDIDX_NP_ROOT_FS,           sizeof(char),GET_SRAM_ADDR(AIDIDX_NP_ROOT_FS)},            
    [AIDIDX_NP_GW_FS_ID] =            { PROC_BP_AIDIDX_NP_GW_FS,             sizeof(char),GET_SRAM_ADDR(AIDIDX_NP_GW_FS)},              
    [AIDIDX_RSVD_6_ID] =              { PROC_BP_AIDIDX_RSVD_6,               sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_6)},                
    [AIDIDX_RSVD_7_ID] =              { PROC_BP_AIDIDX_RSVD_7,               sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_7)},                
    [AIDIDX_RSVD_8_ID] =              { PROC_BP_AIDIDX_RSVD_8,               sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_8)},                
    [AIDIDX_RSVD_9_ID] =              { PROC_BP_AIDIDX_RSVD_9,               sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_9)},                
    [AIDIDX_RSVD_10_ID] =             { PROC_BP_AIDIDX_RSVD_10,              sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_10)},               
    [AIDIDX_RSVD_11_ID] =             { PROC_BP_AIDIDX_RSVD_11,              sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_11)},               
    [AIDIDX_RSVD_12_ID] =             { PROC_BP_AIDIDX_RSVD_12,              sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_12)},               
    [AIDIDX_RSVD_13_ID] =             { PROC_BP_AIDIDX_RSVD_13,              sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_13)},               
    [AIDIDX_RSVD_14_ID] =             { PROC_BP_AIDIDX_RSVD_14,              sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_14)},               
    [AIDIDX_RSVD_15_ID] =             { PROC_BP_AIDIDX_RSVD_15,              sizeof(char),GET_SRAM_ADDR(AIDIDX_RSVD_15)},               
    [BOARD_REVISION_ID] =             { PROC_BP_BOARD_REVISION,              sizeof(int ),GET_SRAM_ADDR(BOARD_REVISION)},               
    [NPCPU_ONLY_MODE_ID] =            { PROC_BP_NPCPU_ONLY_MODE,             sizeof(int ),GET_SRAM_ADDR(NPCPU_ONLY_MODE)},              
    };
#endif

#ifdef CONFIG_MACH_PUMA7
static boot_param_t BootParams[MAX_BOOT_PARAMS] = 
{
    [BOOT_PARAM_VER_ID] =             { PROC_BP_BOOT_PARAM_VERSION,          sizeof(int),GET_SRAM_ADDR(BOOT_PARAM_VER)},               
    [NP_CPU_BOOT_STATUS_ID] =         { PROC_BP_NP_CPU_BOOT_STATUS,          sizeof(int),GET_SRAM_ADDR(ARM11_BOOT_STATUS)},            
    [SILICON_STEPPING_ID] =           { PROC_BP_SILICON_STEPPING,            sizeof(int),GET_SRAM_ADDR(SILICON_STEPPING)},             
    [APP_CPU_BIOS_VERSION_ID] =       { PROC_BP_APP_CPU_BIOS_VERSION,        sizeof(int),GET_SRAM_ADDR(APP_CPU_BIOS_VERSION)},         
    [BOOT_MODE_ID] =                  { PROC_BP_BOOT_MODE,                   sizeof(int),GET_SRAM_ADDR(BOOT_MODE)},                    
    [BOARD_TYPE_ID] =                 { PROC_BP_BOARD_TYPE,                  sizeof(int),GET_SRAM_ADDR(BOARD_TYPE)},                   
    [BOARD_REVISION_ID] =             { PROC_BP_BOARD_REVISION,              sizeof(int),GET_SRAM_ADDR(BOARD_REVISION)},               
    [NP_CPU_DDR_OFFSET_ID] =          { PROC_BP_RAM_OFFSET,                  sizeof(int),GET_SRAM_ADDR(ARM11_DDR_OFFSET)},             
    [NP_CPU_DDR_SIZE_ID] =            { PROC_BP_RAM_SIZE,                    sizeof(int),GET_SRAM_ADDR(ARM11_DDR_SIZE)},               
    [NP_CPU_OS_OFFSET_ID] =           { PROC_BP_NP_CPU_OS_OFFSET,            sizeof(int),GET_SRAM_ADDR(ARM11_OS_OFFSET)},              
    [NP_CPU_OS_SIZE_ID] =             { PROC_BP_NP_CPU_OS_SIZE,              sizeof(int),GET_SRAM_ADDR(ARM11_OS_SIZE)},                
    [NP_CPU_OS_COMMANDLINE_ADDR_ID] = { PROC_BP_NP_CPU_OS_COMMANDLINE_ADDR,  sizeof(int),GET_SRAM_ADDR(ARM11_OS_COMMANDLINE_ADDR)},    
    [NP_CPU_OS_COMMANDLINE_SIZE_ID] = { PROC_BP_NP_CPU_OS_COMMANDLINE_SIZE,  sizeof(int),GET_SRAM_ADDR(ARM11_OS_COMMANDLINE_SIZE)},    
    [DEBUG_BOOT_LOADER_OFFSET_ID] =   { PROC_BP_DEBUG_BOOT_LOADER_OFFSET,    sizeof(int),GET_SRAM_ADDR(DEBUG_BOOT_LOADER_OFFSET)},     
    [DEBUG_PARAMETERS_ADDR_ID] =      { PROC_BP_DEBUG_PARAMETERS_ADDR,       sizeof(int),GET_SRAM_ADDR(DEBUG_PARAMETERS_ADDR)},        
    [NPCPU_ONLY_MODE_ID] =            { PROC_BP_NPCPU_ONLY_MODE,             sizeof(int),GET_SRAM_ADDR(NPCPU_ONLY_MODE)},              
    [RESET_TYPE_ID] =                 { PROC_BP_RESET_TYPE,                  sizeof(int),GET_SRAM_ADDR(RESET_TYPE)},                   
    [RESET_REASON_ID] =               { PROC_BP_RESET_REASON,                sizeof(int),GET_SRAM_ADDR(RESET_REASON)},                 
    [PLATFORM_TYPE_ID] =              { PROC_BP_PLATFORM_TYPE,               sizeof(int),GET_SRAM_ADDR(PLATFORM_TYPE)},                
    [SHARED_DDR_OFFSET_ID] =          { PROC_BP_SHARED_DDR_OFFSET,           sizeof(int),GET_SRAM_ADDR(SHARED_DDR_OFFSET)},
    [SHARED_DDR_SIZE_ID] =            { PROC_BP_SHARED_DDR_SIZE,             sizeof(int),GET_SRAM_ADDR(SHARED_DDR_SIZE)},
    [RESET_BUTTON_DURATION_ID] =      { PROC_BP_RESET_BUTTON_DURATION,       sizeof(int),GET_SRAM_ADDR(RESET_BUTTON_DURATION)},
    [SECURE_BOOT_STATUS_ID] =         { PROC_BP_SECURE_BOOT_STATUS,          sizeof(int),GET_SRAM_ADDR(SECURE_BOOT_STATUS)},
    };
#endif

/* SramLayoutInfo hold the SRAM sections info. */
static section_info_t SramLayoutInfo[MAX_SECTIONS] = 
{
    [BOOT_RAM_SECTION_ID] =       { BOOT_RAM_SECTION_NAME,             BOOT_RAM_SECTION_SIZE,       GET_SRAM_ADDR(BOOT_RAM_SECTION_OFFSET)},       
    [SPARE_SECTION_ID] =          { SPARE_SECTION_NAME,                SPARE_SECTION_SIZE,          GET_SRAM_ADDR(SPARE_SECTION_OFFSET)},          
#if defined (CONFIG_MACH_PUMA6)
    [BBU_SECTION_ID] =            { BBU_SECTION_NAME,                  BBU_SECTION_SIZE,            GET_SRAM_ADDR(BBU_SECTION_OFFSET)},            
#endif
    [MAILBOX_SECTION_ID] =        { MAILBOX_SECTION_NAME,              MAILBOX_SECTION_SIZE,        GET_SRAM_ADDR(MAILBOX_SECTION_OFFSET)},        
    [BOOT_RAM_DEBUG_SECTION_ID] = { BOOT_RAM_DEBUG_SECTION_NAME,       BOOT_RAM_DEBUG_SECTION_SIZE, GET_SRAM_ADDR(BOOT_RAM_DEBUG_SECTION_OFFSET)}, 
    [BOOT_PARAMS_SECTION_ID] =    { BOOT_PARAMS_SECTION_NAME,          BOOT_PARAMS_SECTION_SIZE,    GET_SRAM_ADDR(BOOT_PARAMS_SECTION_OFFSET)},    
};
/***************************************************************************************/
/* LOCAL FUNCTIONS                                                                     */       
/***************************************************************************************/

/**************************************************************************************/
/*! \fn static int proc_snprintf(char **page, int *page_size, int file_offset, 
 *                               int *curr_offset, const char *format, ...)
 **************************************************************************************
 *  \brief This function checks if the writing to the file is allowed writes 
 *         to the wanted buffer.
 *  \return return the number of bytes wirtten to the buffer or -1 if the 
 *          page_size isn't big enough.
 **************************************************************************************/
static int proc_snprintf(char **page, int *page_size, int file_offset, int *curr_offset, const char *format, ...)
{
    va_list args;
    int ret;
    char buff[500];

    /* va_start sets args to the first optional argument in the list of arguments passed to the function*/
    /* va_start must be used before args is used for the first time*/
    va_start(args, format);

    /* Write formatted output to buff where 500 is the Maximum number of characters to write
       using args pointer to a list of arguments, return the number of characters written if the number of characters to write is less than or equal to 500 */
    ret = vsnprintf (buff, 500, format, args);

    /* After all arguments have been retrieved, va_end resets the pointer to NULL*/
    va_end(args);

    /* If the number of characters to write is greater than count, these functions return -1 indicating that output has been truncated.*/
    if (ret == -1)
    {
        return -1;
    }

    *curr_offset += ret; 

    /* This was written already */
    if (*curr_offset <= file_offset) 
    {
        /* Do nothing */
        return 0;
    }

    if (ret > *page_size) 
    {
        /* Dump */
        return -1;
    }

    memcpy(*page,buff,ret);
    *page += ret;
    *page_size -= ret;

    return ret;
}
/**************************************************************************************/
/*! \fn static int sram_bootparams_read_proc (struct seq_file *m, void *v)
 **************************************************************************************
 *  \brief This function reads all the boot params.
 *  \return return the number of bytes wirtten to the buffer.
 **************************************************************************************/
static int sram_bootparams_read_proc (struct seq_file *m, void *v)
{
    BootParamId_e paramId = 0;
    Uint32 paramVal;

    /* Print all of the boot params */
    for (paramId = 0; (paramId < MAX_BOOT_PARAMS); paramId++) 
    {
        SRAM_Get_Boot_Param(paramId, &paramVal);
        switch (BootParams[paramId].size)
        {
        case (sizeof(char)):
            /* write the next boot param in the char format */
            seq_printf(m, "%s 0x%.2X\n", BootParams[paramId].name, paramVal);        
            break; 
        case (sizeof(int)):
            /* write the next boot param in the int format */
            seq_printf(m, "%s 0x%.8X\n", BootParams[paramId].name, paramVal);
            break;
        }
    }

    /* Return number of bytes writen */
    return 0;
}

/*************************************************************************************/
/*! \fn static Int32 is_opertion_allowed(SectionId_e sectionId, Uint32 sectionOffset, Uint32 size)
 ************************************************************************************
 *  \brief  This function checks if the offset is inside the section boundary.
 *  \param[in] section id     : The wanted section
 *  \param[in] section offset : The wanted offset
 *  \param[in] section offset : The wanted size to read
 *  \return SRAM_OK\SRAM_FAIL : Return SRAM_OK if the offset in the section boundary.
 *                              otherwise return SRAM_FAIL
 ************************************************************************************/
static Int32 is_opertion_allowed(SectionId_e sectionId, Uint32 sectionOffset, Uint32 size)
{    

    /* Get the wanted section addr in the SRAM read from it */
    Uint32  sectionSize = SramLayoutInfo[sectionId].size;

    /* Check if reading the wanted size at the wanted offset is in the section boundary, if not fail the opertion*/
    if ( (sectionOffset + size) > sectionSize   )
    {
        return SRAM_FAIL;
    }
    
    return SRAM_OK;
}



/**************************************************************************/
/*! \fn Int32 Get_Boot_Param(BootParamId_e bootParamId, Uint32 *paramVal)
 **************************************************************************
 *  \brief This function get the boot param id and returns its value 
 *  \param[in] bootParamId: The boot param id
 *  \param[out] paramVal: The bbot param value
 *  \return SRAM_OK if reading succeed
 **************************************************************************/
Int32 SRAM_Get_Boot_Param(BootParamId_e bootParamId, Uint32 *paramVal)
{
    
    /* Get the wanted bootParam addr and size */
    Uint32 addr = BootParams[bootParamId].addr;
    Uint32 size = BootParams[bootParamId].size;

    /* init the value given from the user */
    *paramVal = 0;

    /* Get the wanted value */
    switch (size)
    {   
    case sizeof(char):
        *paramVal = BOOT_PARAM_BYTE_READ(addr);
        break;

    case sizeof(int):
        *paramVal = GET_BOOT_PARAM_REG(addr); 
        break;
    default:
        return SRAM_FAIL;
    }

    return SRAM_OK;
}
EXPORT_SYMBOL(SRAM_Get_Boot_Param);


/***********************************************************************************************************/
/*! \fn SRAM_Read_Data_From_Section_At_Offset (SectionId_e sectionId, Uint32 sectionOffset, Uint32 buffSize, Uint8 *buff)
 ***********************************************************************************************************
 *  \brief  This function copy the SRAM section data at the wanted offset in the wanted size.
 *          Reading is done only if the wanted size at the wanted offset are inside sections boundary
 *  \param[in] sectionId     : The section id
 *  \param[in] sectionOffset : The section offset
 *  \param[in] buffSize      : The data size to read
 *  \param[out] buff         : The pointer to the data that was copyed from the SRAM.
 *  \return SRAM_OK \SRAM_FAIL
 ***********************************************************************************************************/
Int32 SRAM_Read_Data_From_Section_At_Offset (SectionId_e sectionId, Uint32 sectionOffset, Uint32 buffSize, Uint8 *buff)
{
    
    /* Check that the offset is in section boundary */
    if (is_opertion_allowed(sectionId, sectionOffset, buffSize) == SRAM_OK)
    {
        /* Reading the value in the section at the section offset */
         memcpy(buff,(void *)(SramLayoutInfo[sectionId].addr + sectionOffset),buffSize);
         return SRAM_OK;
    }
    else
    {
        printk(KERN_ERR "\nSRAM DRIVER Reading from section %s, at offset %x Failed\n reading outside of section boundary\n",SramLayoutInfo[sectionId].name, sectionOffset);
        return SRAM_FAIL;
    }
    
}
EXPORT_SYMBOL(SRAM_Read_Data_From_Section_At_Offset);

/*********************************************************************************************************/
/*! \fn Int32 SRAM_Write_Data_To_Section_At_Offset (SectionId_e sectionId, Uint32 sectionOffset, Uint32 buffSize, Uint8 *buff)
 *********************************************************************************************************
 *  \brief  This function writes the wanted buffer to the wanted section in the wanted offset.
 *          Writing is done only if the wanted size at the wanted offset are inside sections boundary
 *  \param[in] sectionId       : The section id
 *  \param[in] sectionOffset   : The section offset
 *  \param[in]sectionOffsetVal: The writing value
 *  \return SRAM_OK \SRAM_FAIL : SRAM_OK if writing succeed otherwise return SRAM_FAIL 
 ********************************************************************************************************/
Int32 SRAM_Write_Data_To_Section_At_Offset (SectionId_e sectionId, Uint32 sectionOffset, Uint32 buffSize, Uint8 *buff)
{
     /* Check that the offset is in section boundary */
     if (is_opertion_allowed(sectionId, sectionOffset, buffSize) == SRAM_OK)
      {
        /* Writing the value in the section at the section offset */
         memcpy((void *)(SramLayoutInfo[sectionId].addr + sectionOffset),buff,buffSize);
         return SRAM_OK;
      }
      else
      {
        printk(KERN_ERR "\nSRAM DRIVER Writing to section %s, at offset %x Failed \n Writing outside of section boundary\n",SramLayoutInfo[sectionId].name, sectionOffset);
        return SRAM_FAIL;
      }

}
EXPORT_SYMBOL(SRAM_Write_Data_To_Section_At_Offset);

/**************************************************************************/
/*      INTERFACE FUNCTIONS IMPLEMENTATION:                               */
/**************************************************************************/

/**************************************************************************/
/*! \fn static long sram_ioctl(struct file *fd, unsigned int cmd, unsigned long arg) 
 **************************************************************************
 *  \brief This function is called from the user space to oprate function in the driver.
 *  \param[in] fd - The character device of the sram device driver.
 *  \param[in] cmd - The wanted command.
 *  \param[in] arg - The arguments from and to the user space.
 *  \return    SRAM_OK  = ok, SRAM_FAIL = operation faild.
 *************************************************************************/
static long sram_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{
    sram_section_args_t     sectionArgs;
    sram_boot_params_args_t bootParamsArgs;
    Uint8 *ptr;
    if (fd == NULL)
    {
        printk(KERN_ERR "\nSRAM Error value\n");
        return SRAM_FAIL;
    }
    switch (cmd)
    {
    case  SRAM_GET_BOOT_PARAM:
       
        /* Gets the boot param id and return it's value */   
        if (copy_from_user(&bootParamsArgs, (void __user *)arg, sizeof(bootParamsArgs)))
        {
            printk(KERN_ERR"\n SRAM failed to copy from user data. IOCTL=%d\n", cmd);
            return -EFAULT;
        }
        
        SRAM_Get_Boot_Param(bootParamsArgs.bootParamId, &bootParamsArgs.bootParamVal);
        if (copy_to_user((sram_boot_params_args_t *)arg, &bootParamsArgs, sizeof(bootParamsArgs)))
        {
            printk(KERN_ERR"\n SRAM failed to copy to user data. IOCTL=%d\n", cmd);
            return -EFAULT;
        }
        break;
    

    case    SRAM_READ_SECTION_OFFSET:

        /* Gets the section id and reads to val from the wanted offset in the section in the wanted size*/
         if (copy_from_user(&sectionArgs, (void __user *)arg, sizeof(sectionArgs)))
        {
            printk(KERN_ERR "\n SRAM failed to copy from user data. IOCTL=%d\n", cmd);
            return -EFAULT;
        }
        
        /* Creating a buffer in the wanted size to copy to it the section data*/
        ptr = kmalloc(sectionArgs.buffSize,0);
        if (ptr == 0) 
        {
            printk(KERN_ERR"\n SRAM kmalloc failed\n", cmd);
            return -ENOMEM;
        }
        /* Copy from the SRAM to the kernel buffer */
        SRAM_Read_Data_From_Section_At_Offset(sectionArgs.sectionId, sectionArgs.sectionOffset,sectionArgs.buffSize,ptr);
        /* Copy the buffer to user space pointer */
        if (copy_to_user(sectionArgs.buff, ptr, sectionArgs.buffSize))
        {
            printk(KERN_ERR"\n SRAM failed to copy to user data. IOCTL=%d\n", cmd);
            return -EFAULT;
        }
        /* free the kerenel buffer */
        kfree(ptr);
        break;


    case    SRAM_WRITE_SECTION_OFFSET:
       
        /* Gets the section id and writes to val from the wanted offset in the section in the wanted size*/
         if (copy_from_user(&sectionArgs, (void __user *)arg, sizeof(sectionArgs)))
        {
            printk(KERN_ERR "\n SRAM failed to copy from user data. IOCTL=%d\n", cmd);
            return -EFAULT;
        }
        /* Creating a buffer in the wanted size to copy to it the section data*/
        ptr = kmalloc(sectionArgs.buffSize,0);
        if (ptr == 0) 
        {
            printk(KERN_ERR"\n SRAM kmalloc failed\n", cmd);
            return -ENOMEM;
        }
        /* Copy the buffer from user space */
         if (copy_from_user(ptr, sectionArgs.buff, sectionArgs.buffSize))
        {
            printk(KERN_ERR "\n SRAM failed to copy from user data2. IOCTL=%d\n", cmd);
            return -EFAULT;
        }
         /* Copy the wanted data to the SRAM in the wanted section at wanted offset */
        SRAM_Write_Data_To_Section_At_Offset (sectionArgs.sectionId, sectionArgs.sectionOffset, sectionArgs.buffSize, ptr);

        /* free the kerenel buffer */
        kfree(ptr);
        break;


    default:
        printk(KERN_ERR "%s:%d Invalid IOCTL(0x%08X) has been received \n",__FUNCTION__,__LINE__,cmd);
        return -ENOSYS;
    }


    return SRAM_OK;

}


static int sram_bootparams_open_proc(struct inode *inode, struct file *file)
{
    return single_open(file, sram_bootparams_read_proc, NULL); 
}

static const struct file_operations sram_bootparams_proc_fops = {
    .open           = sram_bootparams_open_proc,
    .read           = seq_read,
    .llseek         = seq_lseek,
    .release        = single_release,
};

/**************************************************************************/
/*! \fn static Int32 sram_drv_init(void)
 **************************************************************************
 *  \brief     driver init routine 
 *  \param[in] none
 *  \return    SRAM_OK = ok, other = not ok.
 *************************************************************************/
static int __init sram_drv_init(void)
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "SRAM: Initializing Intel(R) SRAM driver\n");

    /*Allocating the coe_driver */
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "sram_driver")) < 0)
    {
        return ret;
    }

    if (!(c_dev = cdev_alloc()))
    {
        printk(KERN_ERR "%s:%d Failed to allocate character device sram_driver\n",__FUNCTION__,__LINE__);
        unregister_chrdev_region(dev, MINOR_CNT);
        return (-1);
    }

    /* Init the driver */
    cdev_init(c_dev, &sram_drv_fops);

    if ((ret = cdev_add(c_dev, dev, MINOR_CNT)) < 0)
    {
        printk(KERN_ERR "%s:%d Failed to add character device sram_driver\n",__FUNCTION__,__LINE__);
        cdev_del(c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "sram_drv0")))
    {
        class_destroy(cl);
        cdev_del(c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }
    
    printk(KERN_INFO "SRAM: Create device: /dev/%s\n","sram_drv0");

    /* create the /proc file */
    procFile = proc_create(SRAM_BOOTRAM_PROC_NAME, S_IFREG | S_IRUGO, NULL, &sram_bootparams_proc_fops);
    
    if (procFile == NULL) {
        remove_proc_entry(SRAM_BOOTRAM_PROC_NAME, NULL);
        printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",SRAM_BOOTRAM_PROC_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "SRAM: Create proc file /proc/%s\n", SRAM_BOOTRAM_PROC_NAME);

    return SRAM_OK;;
}

/**************************************************************************/
/*! \fn Int32 sram_drv_exit(void)
 **************************************************************************
 *  \brief     driver exit routine 
 *  \param[in] none
 *  \return    SRAM_OK  = ok, other = not ok.
 *************************************************************************/
static void __exit sram_drv_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}


subsys_initcall(sram_drv_init);
module_exit(sram_drv_exit);

/* Driver identification */
MODULE_DESCRIPTION("SRAM Device Driver");
MODULE_LICENSE("GPL");



