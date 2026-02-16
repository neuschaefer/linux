/*
 *
 * ticfg.h
 * Description:
 *
 *
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

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

  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.

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
#ifndef _INCLUDE_TICFG_H_
#define _INCLUDE_TICFG_H_

#include <linux/ioctl.h>

#define TICFG_MTD_OFFSET     (0)

#if defined CONFIG_MIPS_TNETV1050SDB
#define TICFG_MTD_SIZE       (128*1024)
#else
#define TICFG_MTD_SIZE       (64*1024)
#endif

#define ENV_VAR_MTD          (3)
#define ENV_VAR_MTD_OFFSET   (0)
#define ENV_VAR_MTD_SIZE     (FLASH_ENV_ENTRY_SIZE * MAX_ENV_ENTRY)

#define CFGMAN_MTD           (3)
#define CFGMAN_MTD_OFFSET    (ENV_VAR_MTD_SIZE)
#define CFGMAN_MTD_SIZE      (TICFG_MTD_SIZE - ENV_VAR_MTD_SIZE)

#define TICFG_IOCTL_MAGIC	0xde

//#define TICFG_IOCTL_ERASE    0
#define TICFG_IOCTL_ERASE		_IO(TICFG_IOCTL_MAGIC, 0)
#define TICFG_IOCTL_FLAG_READ_PRIMARY	_IOWR(TICFG_IOCTL_MAGIC, 1, unsigned int)
#define TICFG_IOCTL_FLAG_SET_PRIMARY	_IOWR(TICFG_IOCTL_MAGIC, 2, unsigned int)
#define TICFG_IOCTL_FLAG_READ_SECONDARY	_IOWR(TICFG_IOCTL_MAGIC, 3, unsigned int)
#define TICFG_IOCTL_FLAG_SET_SECONDARY	_IOWR(TICFG_IOCTL_MAGIC, 4, unsigned int)

#define MAX_ENV_DATA_LEN     (FLASH_ENV_ENTRY_SIZE)

extern int ticfg_erase_cfgman(struct mtd_info *mtd);
extern int ticfg_erase_env_vars(struct mtd_info *mtd);

extern void ticfg_lock_region(void);
extern void ticfg_unlock_region(void);

#endif
