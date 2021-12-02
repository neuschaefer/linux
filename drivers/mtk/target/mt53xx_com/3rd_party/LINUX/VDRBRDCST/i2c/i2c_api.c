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
#include "x_assert.h"
#include "x_debug.h"
#include "sif_if.h"
#include "drvcust_if.h"//Get Tuner/Demod I2C bus id

#define cI2C_TIMING             270
#define cMAX_I2C_LEN            8

EXTERN UINT8 ICtrlBus_I2cTunerWrite(UINT16 u2ClkDiv,UINT8 i2cAddr,UINT8 *data,UINT16 len);
EXTERN UINT8 ICtrlBus_I2cTunerRead(UINT16 u2ClkDiv,UINT8 i2cAddr,UINT8 *data,UINT16 len);

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
UINT8 ucI2cReadOnly_Gateway(UINT8 ucI2cAddr, UINT32 u4WordAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{
UINT32 u4DemodBusId=SIF_BUS_TUNER_I2C;//default
DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId);
	
    if (SIF_X_Read((UINT8)(u4DemodBusId&0xFF), cI2C_TIMING, ucI2cAddr, 2, u4WordAddr, pucBuffer, ucByteCount) == 0)
    {
        Printf("ucI2cRead failed: %02X-%02X!\n", ucI2cAddr, u4WordAddr);
        return 1;
    }
    return 0;
}
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
UINT32 u4DemodBusId=SIF_BUS_TUNER_I2C;//default
	
	DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId);
	
    if (SIF_X_Read((UINT8)(u4DemodBusId&0xFF), cI2C_TIMING, ucI2cAddr, 1, ucRegAddr, pucBuffer, ucByteCount) == 0)
    {
        Printf("ucI2cRead failed: %02X-%02X!\n", ucI2cAddr, ucRegAddr);
        return 1;
    }
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
     UINT32 u4DemodBusId=SIF_BUS_TUNER_I2C;//default
    if (ucByteCount > cMAX_I2C_LEN)
        Printf("%d exceed max I2C length! (%d)\n", ucByteCount, cMAX_I2C_LEN);
    
	DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId);	
	
    if (SIF_X_Write((UINT8)(u4DemodBusId&0xFF), cI2C_TIMING, ucI2cAddr, 1, ucRegAddr, pucBuffer, ucByteCount) == 0)       
    {
        Printf("u1I2cWrite failed: %02X-%02X!\n", ucI2cAddr, ucRegAddr);
        return 1;
    }

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
    UINT32 u4TunerBusId=SIF_BUS_TUNER_I2C;//default
   DRVCUST_OptQuery(eDemodI2cBusID,&u4TunerBusId);
   if (SIF_X_Read((UINT8)(u4TunerBusId&0xFF), cI2C_TIMING, ucI2cAddr, 0, 0, pucBuffer,ucByteCount) == 0)
    {
        Printf("SIF_X_Read failed\n");
        return 1;
    }


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
	UINT32 u4TunerBusId=0;

    if (ucByteCount > cMAX_I2C_LEN)
        Printf("%d exceed max I2C length! (%d)\n", ucByteCount, cMAX_I2C_LEN);
    
   u4TunerBusId=SIF_BUS_TUNER_I2C;//default
   DRVCUST_OptQuery(eDemodI2cBusID,&u4TunerBusId);
   
   if (SIF_X_Write((UINT8)(u4TunerBusId&0xFF), cI2C_TIMING, ucI2cAddr, 0, 0, pucBuffer, ucByteCount) == 0)
    {
        Printf("SIF_X_Write  failed\n");
        return 1;
    }

    return 0;                               // TRUE
}
UINT8 ucI2cWriteNonbreak(UINT8 ucI2cAddr, UINT16 u2RegAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{
	UINT32 u4TunerBusId=0;

   if (ucByteCount > cMAX_I2C_LEN)
        Printf("%d exceed max I2C length! (%d)\n", ucByteCount, cMAX_I2C_LEN);
    
   u4TunerBusId=SIF_BUS_TUNER_I2C;//default
   DRVCUST_OptQuery(eDemodI2cBusID,&u4TunerBusId);
   
   if (SIF_X_Write_SpecialDemod((UINT8)(u4TunerBusId&0xFF), cI2C_TIMING, ucI2cAddr, 2,u2RegAddr, pucBuffer, ucByteCount) == 0)
   {
        Printf("SIF_X_Write_NonBreak fail\n");
        return 1;
   }
   return 0;
}
UINT8 ucI2cReadNonbreak(UINT8 ucI2cAddr, UINT16 u2RegAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{	
	UINT32 u4TunerBusId=0;
   if (ucByteCount > cMAX_I2C_LEN)
        Printf("%d exceed max I2C length! (%d)\n", ucByteCount, cMAX_I2C_LEN);
    
   u4TunerBusId=SIF_BUS_TUNER_I2C;//default
   DRVCUST_OptQuery(eDemodI2cBusID,&u4TunerBusId);
   
   if (SIF_X_Read((UINT8)(u4TunerBusId&0xFF), cI2C_TIMING, ucI2cAddr, 2,u2RegAddr, pucBuffer, ucByteCount) == 0)
   {
        Printf("SIF_X_Read fail\n");
        return 1;
   }
   return 0;
}

//-------------------------------------------------------------------------
/**
 *  Write data without register address via SPI.
 *  @param  ucI2cAddr 	Serial interface device address.
 *  @param  pucBuffer   Pointer to user's buffer.
 *  @param  ucByteCount Number of byte to read. Max = 256 bytes.
 *  @retval 0           success
 *  @retval !=0         fail
 */
 //-------------------------------------------------------------------------

UINT8 ucSPIWriteOnly(UINT8 ucI2cAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{
	return ICtrlBus_I2cTunerWrite(cI2C_TIMING,ucI2cAddr,pucBuffer,ucByteCount);                             // TRUE
}

//-------------------------------------------------------------------------
/**
 *  Read data without register address via SPI.
 *  @param  ucI2cAddr 	Serial interface device address.
 *  @param  pucBuffer   Pointer to user's buffer.
 *  @param  ucByteCount Number of byte to read. Max = 256 bytes.
 *  @retval 0           success
 *  @retval !=0         fail
 */
 //-------------------------------------------------------------------------

UINT8 ucSPIReadOnly(UINT8 ucI2cAddr, UINT8 *pucBuffer, UINT16 ucByteCount)
{
	return ICtrlBus_I2cTunerRead(cI2C_TIMING,ucI2cAddr,pucBuffer,ucByteCount);                             // TRUE
}

