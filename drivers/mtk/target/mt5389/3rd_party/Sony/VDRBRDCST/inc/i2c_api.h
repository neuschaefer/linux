/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
#ifndef _I2C_API_H_
#define _I2C_API_H_

#include "x_typedef.h"
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
UINT8 ucI2cReadOnly_Gateway(UINT8 ucI2cAddr, UINT32 u4WordAddr, UINT8 *pucBuffer, UINT16 ucByteCount);
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
EXTERN UINT8 ucI2cRead(UINT8 ucI2cAddr, UINT8 ucRegAddr, UINT8 *pucBuffer, UINT16 ucByteCount);

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
EXTERN UINT8 ucI2cWrite(UINT8 ucI2cAddr, UINT8 ucRegAddr, UINT8 *pucBuffer, UINT16 ucByteCount);

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
EXTERN UINT8 ucI2cReadOnly(UINT8 ucI2cAddr, UINT8 *pucBuffer, UINT16 ucByteCount);

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
EXTERN UINT8 ucI2cWriteOnly(UINT8 ucI2cAddr, UINT8 *pucBuffer, UINT16 ucByteCount);

#endif /*_I2C_API_H_*/
