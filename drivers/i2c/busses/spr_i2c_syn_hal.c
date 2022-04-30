/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by Rajeev Kumar  			*/
/*   rajeev-dlh kumar@st.com							*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/

/*------------------------------------------------------------------------------
* includes
-------------------------------------------------------------------------------*/
#include "spr_i2c_syn_hal.h"


/*-----------------------------------------------------------------------------
Name		: I2C_Init
Description	: Initialize the given I2C controller by default settings
		
In		: None
InOut		: None
Out		: None
Comments	: The controller is disabled.
			  Must be called before any other function.
-----------------------------------------------------------------------------*/

void I2C_Init (void)
{
	I2C_Configure();					
}

/*-----------------------------------------------------------------------------
Name		: I2C_Configure
Description	: Configure  the given I2C controller according to mode of operation
		
In		: None
InOut		: None
Out		: None
Comments	: The controller is enabled I2c  
			  Must be called before any other function.
--------------------------------------------------------------------------------*/
void I2C_Configure(void)
{
 
        I2C_Disable();
	
	I2C_set_CON( IC_CON_SD | IC_CON_SPF | IC_CON_MM );//Enable 7bit address, set fast mode, set master mode
	
	I2C_set_RX_TL( FIFO_SIZE );
	I2C_set_TX_TL( FIFO_SIZE );
	
	//Assume ic_clk = 10M(reality is 166M), accelerate I2C bus
	
	I2C_set_FS_SCL_HCNT(FS_SCL_HCNT);
	I2C_set_FS_SCL_LCNT(FS_SCL_LCNT);
	
	I2C_set_SS_SCL_HCNT(SS_SCL_HCNT);
	I2C_set_SS_SCL_LCNT(SS_SCL_LCNT);
	
	I2C_set_HS_SCL_HCNT(HS_SCL_HCNT);
	I2C_set_HS_SCL_LCNT(HS_SCL_LCNT);
	
	
	I2C_set_INTR_MASK(IC_M_STOP_DET);
	
	
	I2C_SAR(IC_SLAVE_ADDR);  	//if used as slave transmitter/Receiver,used to store slave address
	
	I2C_TAR(TAR_ADDR + IC_TAR_NB);	//set I2c device address 50
		
	I2C_Enable();
}
/*-----------------------------------------------------------------------------
Name		: I2C_SetStanderedFrequency
Description	: Set transfer frequency .
		
In	        : None 
--------------------------------------------------------------------------------*/

void I2C_SetStanderedFrequency(void )
{

	int HighCount; 
	int LowCount;
	
	HighCount =  ( IC_CLK * MIN_SS_SCL_HIGHTIME )/NANO_TO_MICRO;
	I2C_set_SS_SCL_HCNT(HighCount);
	
	LowCount =  ( IC_CLK * MIN_SS_SCL_LOWTIME )/NANO_TO_MICRO;
	I2C_set_SS_SCL_LCNT(LowCount);
}
/*-----------------------------------------------------------------------------
Name		: I2C_SetFastFrequency
Description	: Set transfer frequency .
		
In	        : None 
--------------------------------------------------------------------------------*/
void I2C_SetFastFrequency(void )
{
	
	unsigned int HighCount; 
	unsigned int LowCount;

	HighCount =   ( IC_CLK * MIN_FS_SCL_HIGHTIME)/NANO_TO_MICRO;
	I2C_set_FS_SCL_HCNT(HighCount);
	
	LowCount =  ( IC_CLK * MIN_FS_SCL_LOWTIME )/ NANO_TO_MICRO;
	I2C_set_FS_SCL_LCNT(LowCount);
}
/*-----------------------------------------------------------------------------
Name		: I2C_SetHighFrequency
Description	: Set transfer frequency .
		
In	        : None 
--------------------------------------------------------------------------------*/

void I2C_SetHighFrequency(void )
{
	
	int HighCount; 
	int LowCount;
//	if(HS_BUS_LOADING)
	
	HighCount =  ( IC_CLK * MIN_HS_SCL_HIGHTIME )/NANO_TO_MICRO;
	I2C_set_SS_SCL_HCNT(HighCount);
	
	LowCount =  ( IC_CLK * MIN_HS_SCL_LOWTIME )/NANO_TO_MICRO;
	I2C_set_SS_SCL_LCNT(LowCount);
}
/*-----------------------------------------------------------------------------
Name		: I2C_SpeedSelect
Description	: Set transfer speed .
		
In	        : None 
--------------------------------------------------------------------------------*/

void I2C_SpeedSelect (int i2c_SPD)
{
	if(i2c_SPD==IC_MAX_SPEED)
	{
		 *IC_CON |= IC_CON_SPH;
	    	 *IC_CON |= IC_CON_SPL;
		 I2C_SetHighFrequency( );
        }
	else if(i2c_SPD==IC_SPEED_MODE_FAST)
	{
		*IC_CON |= IC_CON_SPH;
	   	 *IC_CON &= ~IC_CON_SPL;
		 I2C_SetFastFrequency( );
	}
	else if(i2c_SPD==IC_SPEED_MODE_STANDARD)
	{
		*IC_CON |= ~IC_CON_SPF;
	    	*IC_CON &=  IC_CON_SPL;
		I2C_SetStanderedFrequency( );
	} 
	    
}

/*-----------------------------------------------------------------------------
Name		: I2C_SetAddress
Description	: Setting slave address to the TAR register(lower 10 bit, 9:0) .
		
In	        : None 
--------------------------------------------------------------------------------*/


//Setup the destination address part of TAR register
void I2C_SetAddress (unsigned int i2c_addr)
{
	int IC_TAR_CON_BITS;
	IC_TAR_CON_BITS = *IC_TAR & IC_TAR_NB;/*Save 12:10 bits */
	*IC_TAR = (unsigned int)i2c_addr;  /*Load address */
        *IC_TAR |= IC_TAR_CON_BITS;        /*Restore 12:10 bits*/
}

/*-----------------------------------------------------------------------------
Name		: I2C_TransmitByte
Description	: set CMD bit to 0, writing data to the command register .
		
In	        : None 
--------------------------------------------------------------------------------*/

void I2C_TransmitByte (int i2c_cmd_data)
{
             *IC_CMD_DATA =  i2c_cmd_data;
}

/*-----------------------------------------------------------------------------
Name		: I2C_Set_Read
Description	: Setting command bit to 1 For read operation .
		
In	        : None 
--------------------------------------------------------------------------------*/

//set CMD bit to 1
int I2C_Set_Read (void )
{
	 unsigned int data=0;
	if ((*IC_STATUS & IC_STATUS_RFNE) ==8)
	{
		data=(unsigned char ) (*IC_CMD_DATA); 
	}
	else
	{
		return -1;
	}
	return data; 
}


/*-----------------------------------------------------------------------------
Name		: I2C_CheckStop
Description	: Check for the generation of stop condition.
		
In	        : None 
--------------------------------------------------------------------------------*/

void I2C_Stop_Detect( void )
{
	int ic_clr_stop_detect;
	mdelay(4);
	if((*IC_RAW_INTR_STAT & IC_R_STOP_DET))
    		ic_clr_stop_detect = *IC_CLR_STOP_DET;  //clear stop detected bit.
  
}


/*-----------------------------------------------------------------------------
Name		: I2C_TransmitBuffer
Description	: Transmit data to the slave.
		
In	        : t_I2C_Configure *config,
		  u8 *ptrToTxBuffer,
		  u32 NoOfBytes
		  u8 slave_offset	 
InOut		: None
Out	        : None
--------------------------------------------------------------------------------*/

	size_t
I2C_TransmitBuffer(t_I2C_Configure *config,u8 *ptrToTxBuffer,u32 NoOfBytes,u32 slave_offset )
{
	u32 address;

	t_I2C_addressing_mode  ModeOfAddressing; 
	unsigned int  BytesWrite;
	unsigned int  BytesTransfered=0;
	int data=0; 
	int ic_clr;
	int i2cSpeed;
	unsigned int Rbytes;

	//check for the buffer
	if(ptrToTxBuffer == NULL)
		return I2C_TX_BUFFER_NULL;

	// Check for the number of data to be write
	if(NoOfBytes<=0)
		return I2C_NO_TX_DATA;

	if(slave_offset<0)
		return I2C_OFFSET_ERROR;

	address = config->SlaveAddress;
	ModeOfAddressing = config->operation; //10 bit or 7 bit addressing
	i2cSpeed = config->Speed; 


	I2C_Disable();	
	I2C_SpeedSelect ( i2cSpeed);
	I2C_SetAddress(address);	
	I2C_Enable();	


	
	Rbytes=(slave_offset % BLK_SIZE);
	if(Rbytes!=0 && ((BLK_SIZE-Rbytes)<NoOfBytes))
	{
		if( slave_offset == MAX_OFFSET)
		{
			address  = address | 0x01 ;
			I2C_SetAddress(address);
			slave_offset =0;
		}

		do
		{
			ic_clr = *IC_CLR_TX_ABRT;
			I2C_TransmitByte(slave_offset);
			while (!(*IC_STATUS &IC_STATUS_TFE));
		}while(*IC_TXABRT_SOURCE & IC_ABRT_7B_ADDR_NOACK);

			
			
		for(BytesWrite=0; BytesWrite <(BLK_SIZE-Rbytes) ;) 
		{
				
			if((*IC_STATUS & IC_STATUS_TFNF)==2)
			{
				data= *(ptrToTxBuffer + BytesTransfered);
				I2C_TransmitByte(data);
				BytesTransfered++;
				BytesWrite++;
			}
			else
			{
			}
		}
		I2C_Stop_Detect(); 
		NoOfBytes 	 -=	(BLK_SIZE-Rbytes);
		slave_offset +=	(BLK_SIZE-Rbytes);
	}


	while(NoOfBytes>BOUNDRY_CONDITION)
	{

		if( slave_offset == MAX_OFFSET)
		{
			address  = address | 0x01 ;
			I2C_SetAddress(address);
			slave_offset =0;
		}

		do
		{
			ic_clr = *IC_CLR_TX_ABRT;
			I2C_TransmitByte(slave_offset);
			while (!(*IC_STATUS &IC_STATUS_TFE));
		}while(*IC_TXABRT_SOURCE & IC_ABRT_7B_ADDR_NOACK);
	
		
		
		for(BytesWrite=0; BytesWrite <=BOUNDRY_CONDITION ;)
		{
					
			if((*IC_STATUS & IC_STATUS_TFNF)==2)
			{
				data= *(ptrToTxBuffer + BytesTransfered);
				I2C_TransmitByte(data);
				BytesTransfered++;
				BytesWrite++;
			}
			else
			{
			}
		}

		I2C_Stop_Detect(); 
		NoOfBytes -= (BOUNDRY_CONDITION + 1);
		slave_offset += (BOUNDRY_CONDITION + 1);

	}


	if(NoOfBytes>0) 
	{

		if( slave_offset == MAX_OFFSET)
		{
			address  = address | 0x01 ;
			I2C_SetAddress(address);
			slave_offset =0;
		}

		do
		{
			ic_clr = *IC_CLR_TX_ABRT;
			I2C_TransmitByte(slave_offset);
			while (!(*IC_STATUS &IC_STATUS_TFE));
		}while(*IC_TXABRT_SOURCE & IC_ABRT_7B_ADDR_NOACK);
		for(BytesWrite=0; BytesWrite <NoOfBytes ;)
		{
			if((*IC_STATUS & IC_STATUS_TFNF)==2)
			{
				data= *(ptrToTxBuffer + BytesTransfered);
				I2C_TransmitByte(data);
				BytesTransfered++;
				BytesWrite++;
			}
			else
			{
			}
		}
		I2C_Stop_Detect(); 
	}

	return BytesTransfered;
}

//unsigned char I2C_ReceiveByte(void)
int I2C_ReceiveByte(void)
{
	int data;
	data = I2C_Set_Read();
	if(data<0)
	{
		return -1;
	}

	return data;

}

/*-----------------------------------------------------------------------------
Name		: I2C_ReceiveBuffer
Description	: Receive data from the Slave.

In	        : t_I2C_configure *config,
u8 *ptrToRxBuffer,
u32 NoOfBytes
u8 slave_offset	 
InOut		: None
Out	        : None
--------------------------------------------------------------------------------*/

	size_t
I2C_ReceiveBuffer(t_I2C_Configure *config,u8 *ptrToRxBuffer,u32 NoOfBytes,u32 slave_offset )
{
	u32 address;
	t_I2C_addressing_mode ModeOfAddressing; 
	int ic_clr;
	int i2cSpeed;
	int count =0;
	int data;
	int commandcounter=0;

	int ReceiveByte=0;
	//check for the buffer
	if(ptrToRxBuffer == NULL)
		return I2C_RX_BUFFER_NULL;

	// Check for the number of data to be write
	if(NoOfBytes<=0)
		return I2C_NO_RX_DATA;
	if(slave_offset<0)
		return I2C_OFFSET_ERROR;


	address = config->SlaveAddress;
	ModeOfAddressing = config->operation; //10 bit or 7 bit addressing

	i2cSpeed = config->Speed; 

	I2C_Disable();	
	I2C_SpeedSelect ( i2cSpeed );
	I2C_SetAddress(address);	
	I2C_Enable();

	if(NoOfBytes>0)
	{
/*		if( slave_offset == MAX_OFFSET)
		{
			address  = address | 0x01 ;
			I2C_SetAddress(address);
		}*/

		do
		{
			ic_clr = *IC_CLR_TX_ABRT;
			I2C_TransmitByte(slave_offset);
			I2C_Stop_Detect(); 
			while (!(*IC_STATUS &IC_STATUS_TFE));
		}while(*IC_TXABRT_SOURCE & IC_ABRT_7B_ADDR_NOACK);

		do
		{

			if(commandcounter<NoOfBytes)
			{
				if((*IC_STATUS & IC_STATUS_TFNF)==2)
				{
					*IC_CMD_DATA = IC_CMD ; 
					commandcounter++;
				}
			}
			data = I2C_ReceiveByte();
			if(data >= 0 )
			{
				*(ptrToRxBuffer)=data; 
				ptrToRxBuffer++;
				ReceiveByte++;
				count++;
			}
			else
			{
			}

		}while( ReceiveByte <NoOfBytes);
	}

	return count;
}

