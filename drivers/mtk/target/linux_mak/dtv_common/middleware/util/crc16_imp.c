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
/*-----------------------------------------------------------------------------
 * $RCSfile: crc32_imp.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains API to generate CRC32 check-sum value
 *---------------------------------------------------------------------------*/


#include "inc/common.h"
#include "util/x_crc16.h"
#include "util/c_crc16.h"

UINT16 aui2_crc_16_tbl[16] = {
0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400 };


/*----------------------------------------------------------------------------
 * Function: x_crc32_gen()
 *
 * Description:
 *      generate a CRC16 check-sum value from an array of data pointed to
 *      by "pui1_data" over "z_size" bytes.
 *
 * Inputs:
 *      pui1_data:    an array of data to generate its CRC16
 *      z_size:       byte count of array of data "pui1_data"
 *      ui2_crc_seed: CRC16 seed value to generate the check sum value from
 *                    'pui1_data' buffer.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      a CRC16 value
 *      0xFFFF          If *pui1_data = NULL or z_size = 0.
 *---------------------------------------------------------------------------*/
UINT16  x_crc16_gen(
    const UINT8   *pui1_data,
    SIZE_T        z_size,
    UINT16        ui2_crc_seed )
{
    UINT16 ui2_crc = ui2_crc_seed;
    UINT16 ui2_ref;

    if ( (pui1_data == (UINT8*)NULL) ||
         (z_size == 0) )
    {
        return 0xFFFF;
    }

    while (z_size-- > 0) {

        /* Calculate checksum of lower four bits of *pui1_data */
        ui2_ref = aui2_crc_16_tbl[ui2_crc & 0xF];
        ui2_crc = (ui2_crc >> 4) & 0x0FFF;
        ui2_crc = ui2_crc ^ ui2_ref ^ aui2_crc_16_tbl[(*pui1_data) & 0xF];

        /* Calculate checksum of upper four bits of *pui1_data */
        ui2_ref = aui2_crc_16_tbl[ui2_crc & 0xF];
        ui2_crc = (ui2_crc >> 4) & 0x0FFF;
        ui2_crc = ui2_crc ^ ui2_ref ^ aui2_crc_16_tbl[(*pui1_data >> 4) & 0xF];

        pui1_data++;
    }

    return(ui2_crc);
}


/*----------------------------------------------------------------------------
 * Function: x_crc32_gen()
 *
 * Description:
 *      generate a CRC16 check-sum value from an array of data pointed to
 *      by "pui1_data" over "z_size" bytes.
 *
 * Inputs:
 *      pui1_data:    an array of data to generate its CRC16
 *      z_size:       byte count of array of data "pui1_data"
 *      ui2_crc_seed: CRC16 seed value to generate the check sum value from
 *                    'pui1_data' buffer.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      a CRC16 value
 *      0xFFFF          If *pui1_data = NULL or z_size = 0.
 *---------------------------------------------------------------------------*/
UINT16  c_crc16_gen(
    const UINT8   *pui1_data,
    SIZE_T        z_size,
    UINT16        ui2_crc_seed )
{
    return x_crc16_gen( pui1_data, z_size, ui2_crc_seed);
}
