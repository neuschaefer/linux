/*************************************************************************************************************/
/*                                  Silicon Laboratories                                                     */
/*                                  Broadcast Video Group                                                    */
/*                     Layer 0      Communication Functions                                                  */
/* FILE: Si2170_L0_TV_Chassis.c                                                                              */
/*-----------------------------------------------------------------------------------------------------------*/
/*   This source code contains all i2c functions to communicate with i2c components                          */
/*     All functions are declared in Si2170_L0_TV_Chassis.h                                                  */
/*************************************************************************************************************/
#include "si2156_L0_TV_Chassis.h"

#include "x_stl_lib.h"
#include "x_typedef.h"
#include "si2156_i2c_api.h"
#include "PD_Def.h"
/************************************************************************************************************************
	NAME: L0_Init function
	DESCRIPTION:layer 0 initialization function
              Used to sets the layer 0 context parameters to startup values.
			  The I2C address of the Si2173 is set in the Si2170_L1_API_Init procedure.
              It is automatically called by the Layer 1 init function.
	Parameter:	Pointer to L0 (I2C) Context -
	Porting:	In most cases, no modifications should be required.
	Returns:    void
************************************************************************************************************************/
void   L0_Init  (L0_Context *pContext)
{
	(pContext)->address       = 0;
    return;
};
 /************************************************************************************************************************
  L0_SetAddress function
  Use:        function to set the device address
              Used to set the I2C address of the component.
              It must be called only once at startup per Layer 1 instance, as the addresses are not expected to change over time.
  Returns:    1 if OK, 0 otherwise
************************************************************************************************************************/
int     L0_SetAddress        (L0_Context* i2c, unsigned int add, int addSize)
{
  i2c->address   = add;
  i2c->indexSize = addSize;
  return 0;
}

/************************************************************************************************************************
  NAME: system_wait
  DESCRIPTION:	Delay for time_ms (milliseconds)
  Porting:		Replace with embedded system delay function
  Returns:		nothing
************************************************************************************************************************/
void system_wait(int time_ms)
{
	mcDELAY_MS(time_ms);
	return;
}

/************************************************************************************************************************
  L0_ReadBytes function
  Use:        lowest layer read function
              Used to read a given number of bytes from the Layer 1 instance.
  Parameters: i2c, a pointer to the Layer 0 context.
              iI2CIndex, the index of the first byte to read.
              iNbBytes, the number of bytes to read.
              *pbtDataBuffer, a pointer to a buffer used to store the bytes.
  Returns:    the number of bytes read.
************************************************************************************************************************/
int    L0_ReadBytes              (L0_Context* i2c, unsigned int iI2CIndex, int iNbBytes, unsigned char *pucDataBuffer)
{

    int i=0;
    if (SI215X_ucI2cReadOnly(i2c->address,pucDataBuffer,iNbBytes)!=0)
    {
        mcSHOW_HW_MSG(("[Silabs]:<= I2C Read Error\n"));
    }

	mcSHOW_DBG_MSG4(("SI215X_ucI2cReadOnly:i2c->address =%x;iNbBytes=%d\n",i2c->address,iNbBytes));
	for(i=0;i<iNbBytes;i++)
	 {mcSHOW_DBG_MSG4(("pucDataBuffer[%x]\n",pucDataBuffer[i]));}

	return iNbBytes;
}

/************************************************************************************************************************
  L0_WriteBytes function
  Use:        lowest layer write function
              Used to write a given number of bytes from the Layer 1 instance.
  Parameters: i2c, a pointer to the Layer 0 context.
              iI2CIndex, the index of the first byte to write.
              iNbBytes, the number of bytes to write.
              *pbtDataBuffer, a pointer to a buffer containing the bytes to write.
  Returns:    the number of bytes read.
************************************************************************************************************************/
 int      L0_WriteBytes             (L0_Context* i2c, unsigned int iI2CIndex, int iNbBytes, unsigned char *pucDataBuffer)
 {
    int i=0;
    if (SI215X_ucI2cWriteOnly(i2c->address,pucDataBuffer,iNbBytes) !=0)
    {
         mcSHOW_HW_MSG(("[Silabs]:<= I2C Write Error\n"));
    }
// ICtrlBus_I2cTunerWrite(0x100,i2c->address,pucDataBuffer,iNbBytes);
	  mcSHOW_DBG_MSG4(("SI215X_ucI2cWriteOnly:i2c->address =%x;iI2CIndex=%x;iNbBytes=%d\n",i2c->address,iI2CIndex,iNbBytes));
	 for(i=0;i<iNbBytes;i++)
	  	{ mcSHOW_DBG_MSG4(("pucDataBuffer[%x]\n",pucDataBuffer[i]));}

  return iNbBytes;
}

/************************************************************************************************************************
  L0_ReadCommandBytes function
  Use:        'command mode' bytes reading function
              Used to read a given number of bytes from the Layer 1 instance in 'command mode'.
  Comment:    The 'command mode' is a specific mode where the indexSize is always 0 and the index is always 0x00
  Parameters: i2c, a pointer to the Layer 0 context.
              iNbBytes, the number of bytes to read.
              *pucDataBuffer, a pointer to a buffer used to store the bytes.
  Returns:    the number of bytes read.
************************************************************************************************************************/
 int     L0_ReadCommandBytes    (L0_Context* i2c, int iNbBytes, unsigned char *pucDataBuffer)
{ return L0_ReadBytes (i2c, 0x00, iNbBytes, pucDataBuffer); }

/************************************************************************************************************************
  L0_WriteCommandBytes function
  Use:        'command mode' bytes writing function
              Used to write a given number of bytes to the Layer 1 instance in 'command mode'.
  Comment:    The 'command mode' is a specific mode where the indexSize is always 0 and the index is always 0x00
  Parameters: i2c, a pointer to the Layer 0 context.
              iNbBytes, the number of bytes to write.
              *pucDataBuffer, a pointer to a buffer containing the bytes.
  Returns:    the number of bytes written.
************************************************************************************************************************/
 int      L0_WriteCommandBytes   (L0_Context* i2c, int iNbBytes, unsigned char *pucDataBuffer)
{ return L0_WriteBytes(i2c, 0x00, iNbBytes, pucDataBuffer); }


/************************************************************************************************************************/
