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
#include "i2c_api.h"
#include "x_assert.h"
#include "x_debug.h"

#define cI2C_TIMING             270
#define cMAX_I2C_LEN            8

//-------------------------------------------------------------------------
/**
 *  Read data with register address.
 *  @param  ucI2cAddr 	Serial interface device address.
 *  @param  ucRegAddr 	register address
 *  @param  pucBuffer   Pointer to user's buffer.
 *  @param  ucByteCount Number of byte to read. Max = 256 bytes.
 *  @retval 0           success
 *  @retval 1           fail
 */
 //-------------------------------------------------------------------------
UINT8 ucI2cRead(UINT8 ucI2cAddr, UINT8 ucRegAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{
    if (SIF_X_Read(SIF_BUS_TUNER_I2C, cI2C_TIMING, ucI2cAddr, 1, ucRegAddr, pucBuffer, ucByteCount) == 0)
    {
        Printf("ucI2cRead failed: %02X-%02X!\n", ucI2cAddr, ucRegAddr);
        return 1;
    }
/*  //debug
    UINT8 ii;
    for (ii=0; ii<ucByteCount; ii++) {
        mcSHOW_DBG_MSG(("ucI2cRead[0x%02X] = 0x%02X\n", (ii+ucRegAddr), *(pucBuffer+ii)));
    }
*/
    return 0;
}

//-------------------------------------------------------------------------
/**
 *  Write data with register address.
 *  @param  ucI2cAddr 	Serial interface device address.
 *  @param  ucRegAddr 	register address
 *  @param  pucBuffer   Pointer to user's buffer.
 *  @param  ucByteCount Number of byte to read. Max = 256 bytes.
 *  @retval 0           success
 *  @retval 1           fail
 */
 //-------------------------------------------------------------------------
UINT8 ucI2cWrite(UINT8 ucI2cAddr, UINT8 ucRegAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{	
    if (ucByteCount > cMAX_I2C_LEN)
        Printf("%d exceed max I2C length! (%d)\n", ucByteCount, cMAX_I2C_LEN);
        
    if (SIF_X_Write(SIF_BUS_TUNER_I2C, cI2C_TIMING, ucI2cAddr, 1, ucRegAddr, pucBuffer, ucByteCount) == 0)
    {
        Printf("u1I2cWrite failed: %02X-%02X!\n", ucI2cAddr, ucRegAddr);
        return 1;
    }
/* //debug
    UINT8 ii;
    for (ii=0; ii<ucByteCount; ii++) {
        mcSHOW_DBG_MSG(("ucI2cWrite[0x%02X] = 0x%02X\n", (ucRegAddr+ii), *(pucBuffer+ii)));
    }
*/
    return 0;                               // TRUE
}

//-------------------------------------------------------------------------
/**
 *  Read data without register address.
 *  @param  ucI2cAddr 	Serial interface device address.
 *  @param  pucBuffer   Pointer to user's buffer.
 *  @param  ucByteCount Number of byte to read. Max = 256 bytes.
 *  @retval 0           success
 *  @retval 1           fail
 */
 //-------------------------------------------------------------------------
UINT8 ucI2cReadOnly(UINT8 ucI2cAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{
    if (SIF_TunerReadNoSubAddr(cI2C_TIMING, ucI2cAddr, pucBuffer, ucByteCount) == 0)
    {
        Printf("SIF_TunerReadNoSubAddr failed\n");
        return 1;
    }

/* //debug   UINT8 ii;
    for (ii=0; ii<ucByteCount; ii++) {
        Printf("ucI2cReadOnly[0x%02X] = 0x%02X\n", ii, *(pucBuffer+ii));
    } */
    return 0;
}

//-------------------------------------------------------------------------
/**
 *  Write data without register address.
 *  @param  ucI2cAddr 	Serial interface device address.
 *  @param  pucBuffer   Pointer to user's buffer.
 *  @param  ucByteCount Number of byte to read. Max = 256 bytes.
 *  @retval 0           success
 *  @retval !=0         fail
 */
 //-------------------------------------------------------------------------
UINT8 ucI2cWriteOnly(UINT8 ucI2cAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{
    if (ucByteCount > cMAX_I2C_LEN)
        Printf("%d exceed max I2C length! (%d)\n", ucByteCount, cMAX_I2C_LEN);

    if (SIF_TunerWriteNoSubAddr(cI2C_TIMING, ucI2cAddr, pucBuffer, ucByteCount) == 0)
    {
        Printf("SIF_TunerWriteNoSubAddr failed\n");
        return 1;
    }

/* //debug   UINT8 ii;
    for (ii=0; ii<ucByteCount; ii++) {
        mcSHOW_DBG_MSG(("SIF_TunerWriteNoSubAddr[0x%02X] = 0x%02X\n", (ii+ucI2cAddr), *(pucBuffer+ii)));
    }
*/ 
    return 0;                               // TRUE
}
