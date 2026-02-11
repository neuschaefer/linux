/*****************************************************************************
*  Copyright 2007 - 2012 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/licenses/old-license/gpl-2.0.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/**
 * @file    chal_nand_cmd.h
 * @brief   CHAL NAND command definitions.
 * @note
 **/

#ifndef __CHAL_NAND_CMD_H__
#define __CHAL_NAND_CMD_H__

/* ONFI mandatory commands */
#define NAND_CMD_READ_1ST        (0x00)
#define NAND_CMD_READ_2ND        (0x30)

#define NAND_CMD_READ_RAND_1ST   (0x05)
#define NAND_CMD_READ_RAND_2ND   (0xE0)

#define NAND_CMD_BERASE_1ST      (0x60)
#define NAND_CMD_BERASE_2ND      (0xD0)

#define NAND_CMD_STATUS          (0x70)

#define NAND_CMD_PROG_1ST        (0x80)
#define NAND_CMD_PROG_2ND        (0x10)

#define NAND_CMD_PROG_RAND       (0x85)

#define NAND_CMD_ID              (0x90)

#define NAND_CMD_READ_PARAM      (0xEC)

#define NAND_CMD_RESET           (0xFF)

/* ONFI optional commands */
#define NAND_CMD_GET_FEATURE     (0xEE)
#define NAND_CMD_SET_FEATURE     (0xEF)

/* NAND status result masks */
#define NAND_STATUS_FAIL         (0x01)   /* last command failed */
#define NAND_STATUS_FAILC        (0x02)   /* command issued prior to the last command failed */
#define NAND_STATUS_CSP          (0x08)   /* command specific */
#define NAND_STATUS_VSP          (0x10)   /* vendor specific */
#define NAND_STATUS_AREADY       (0x20)   /* array is ready */
#define NAND_STATUS_READY        (0x40)   /* LUN or plane is ready */
#define NAND_STATUS_WP_N         (0x80)   /* not protected */

#endif

