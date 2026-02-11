/***************************************************************************
*
* (c)1999-2011 Broadcom Corporation
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2,
* available at http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
****************************************************************************/
#ifndef _PLATFORM_MCONFIG_SHARED_H_
#define _PLATFORM_MCONFIG_SHARED_H_

/* Memory map for Linux build */

#ifndef EXT_RAM_BASE_ADDR
#define EXT_RAM_BASE_ADDR  CONFIG_BCM_RAM_BASE
#endif


/* Shared memory defines; commonly used by RTOS and Linux builds */

/* DSP sharedmem */
#define DSP_SH_BASE                            EXT_RAM_BASE_ADDR
#define DSP_SH_SIZE                            0x00064000

#define CP_SH_BASE                             DSP_SH_BASE
/* 128k CP/DSP sharedmem */
#define CP_SH_SIZE                             0x00020000

#define AP_SH_BASE                             (DSP_SH_BASE+CP_SH_SIZE)
/* 128k  AP/DSP sharedmem */
#define AP_SH_SIZE                             0x00020000

/* 256kB IPC shared RAM */
#define IPC_OFFSET                             0x1E00000
#define IPC_BASE                               (EXT_RAM_BASE_ADDR + IPC_OFFSET)
#define IPC_SIZE                               0x00200000

#define PARM_DEP_RAM_OFFSET                    0x180000
#define PARM_DEP_RAM_ADDR                      \
			(EXT_RAM_BASE_ADDR + PARM_DEP_RAM_OFFSET)
#define PARM_DEP_SIZE                          0x00010000

#define MAIN_ADDRESS_OFFSET 0x30
#define INIT_ADDRESS_OFFSET 0x34
#define MODEM_ITCM_ADDRESS 0x3AC00000
#define MODEM_DTCM_ADDRESS 0x3AD00000
#define RESERVED_HEADER 0x400


#endif /*_PLATFORM_MCONFIG_SHARED_H_*/

