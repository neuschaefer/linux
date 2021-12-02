/*****************************************************************************************
 *
 * FILE NAME          : MxL601_OEM_Drv.c
 * 
 * AUTHOR             : Dong Liu 
 *
 * DATE CREATED       : 01/23/2011  
 *
 * DESCRIPTION        : This file contains I2C driver and Sleep functins that 
 *                      OEM should implement for MxL601 APIs
 *                             
 *****************************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ****************************************************************************************/

#include "MxL601_OEM_Drv.h"
#include "i2c_api.h"

#include "x_os.h"  // 3rd party's code ? 
#include "x_typedef.h"  // 3rd party's code ? 
/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_WriteRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C write operation.
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS Ctrl_WriteRegister(UINT8 I2cSlaveAddr, UINT8 RegAddr, UINT8 RegData)
{
  MXL_STATUS status = MXL_FALSE;
  UINT8 pucBuffer[8] = {0};
   
  // OEM should implement I2C write protocol that complies with MxL601 I2C
  // format.

  // 8 bit Register Write Protocol:
  // +------+-+-----+-+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |RegAddr   | |RegData(L)| |P|
  // +------+-+-----+-+-+----------+-+----------+-+-+
  // |SLAVE |         |A|          |A|          |A| |
  // +------+---------+-+----------+-+----------+-+-+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Ack), N(NACK), 
  // P(Stop condition)
  pucBuffer[0] = RegAddr;
  pucBuffer[1] = RegData;
  
  // Call ucI2cWrite, return 0 means success, 1 means error. 
// if (0 == ucI2cWrite(I2cSlaveAddr, RegAddr, pucBuffer, 1))
  if (0 == ICtrlBus_I2cTunerWrite(270,I2cSlaveAddr,pucBuffer,2)){

  	status = MXL_TRUE;
	}
  
  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_ReadRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C read operation.
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS Ctrl_ReadRegister(UINT8 I2cSlaveAddr, UINT8 RegAddr, UINT8 *DataPtr)
{
  MXL_STATUS status = MXL_TRUE;
  UINT8 pucBuffer[8] = {0};
  
  // OEM should implement I2C read protocol that complies with MxL601 I2C
  // format.

  // 8 bit Register Read Protocol:
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFB| |RegAddr   | |P|
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |SLAVE |         |A|    |A|          |A| |
  // +------+-+-----+-+-+----+-+----------+-+-+
  // +------+-+-----+-+-+-----+--+-+
  // |MASTER|S|SADDR|R| |     |MN|P|
  // +------+-+-----+-+-+-----+--+-+
  // |SLAVE |         |A|Data |  | |
  // +------+---------+-+-----+--+-+
  // Legends: SADDR(I2c slave address), S(Start condition), MA(Master Ack), MN(Master NACK), 
  // P(Stop condition)
  pucBuffer[0] = 0xFB;
  pucBuffer[1] = RegAddr;
 // if (0 == ucI2cWrite(I2cSlaveAddr, 0xFB, pucBuffer, 1))
 if (0 == ICtrlBus_I2cTunerWrite(270,I2cSlaveAddr,pucBuffer,2))
  	status = MXL_TRUE;
  else status = MXL_FALSE;
  
  if (status == MXL_TRUE) 
  {
    //status = ucI2cReadOnly(I2cSlaveAddr, pucBuffer, 1);
  	 status =ICtrlBus_I2cTunerRead(270,I2cSlaveAddr,pucBuffer,1);
    if (status == MXL_TRUE) 
     *DataPtr = pucBuffer[0];
  }

  return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : MXL_Sleep
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete sleep operation. WaitTime is in ms unit
--|
--| RETURN VALUE  : None
--|
--|-------------------------------------------------------------------------------------*/

void MxL_Sleep(UINT16 DelayTimeInMs)
{
  // OEM should implement sleep operation 
  x_thread_delay((UINT32)(DelayTimeInMs));
}
