/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2008 by Ashish Priyadarshi  			*/
/*   ashish.priyadarshi@st.com								*/
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

#include <asm/arch/hardware.h>
#include "spr_fsmc_st_hal.h"



/*--------------------------------------------------------------------------*/
/*		 NAME :	FSMC_SetSramNorControl()  			    */
/*--------------------------------------------------------------------------*/
/*   DESCRIPTION: This routine set SRAM_NOR control register of  FSMC.	    */
/*				                      			    */
/*   PARAMETERS :							    */
/*           IN : bank_no: FSMC Bank Number				    */
/*				  p_nor_control: Control information	    */
/*		    OUT : None						    */
/*	   RETURN :	FSMC_OK	: For normal operations			    */
/*   			FSMC_INVALID_BANK_NO : If bank_no is invalid	    */
/*   			FSMC_BAD_PARAM : if bank_ctrl is not for NOR flash  */
/*	   							 or SRAM.   */
/*	   	    						 	    */
/*  REENTRANCY: NA							    */
/*--------------------------------------------------------------------------*/

PUBLIC t_fsmc_error FSMC_SetSramNorControl( t_fsmc_nor_chip_index bank_no,
		t_struct_fsmc_pc_nor_control* p_nor_control)
{

	t_uint32 val=0;
	t_fsmc_error error=FSMC_OK;

	if( FSMC_BANK_ENABLE == p_nor_control->enable_bank)
		val |= BANKENABLE ;

	switch(p_nor_control->mem_type)
	{
		case FSMC_SRAM:
			val |= SRAM_MEM;
			break;
		case CELLULAR_RAM:
			val |= CELLULAR_RAM;
			break;
		case FSMC_NOR_FLASH:
			val |= NOR_FLASH;
			break;
		case FSMC_COSMO_RAM:
			val |= COSMO_RAM;
			break;
		default:
			if(bank_no == FSMC_BANK0)
				val |= NOR_FLASH;
			else
				val |= SRAM_MEM;
	}


	switch(p_nor_control->dev_width)
	{
		case  FSMC_ACCESS_8:
			val |=NAND_DEV_WIDTH_8;
			break;
		case FSMC_ACCESS_16:
			val |=NAND_DEV_WIDTH_16 ;
			break;
		case FSMC_ACCESS_32:
			val |=NAND_DEV_WIDTH_32;
			break;
		default:
			error = FSMC_INVALID_ACCESS_WIDTH;
	}

	if( FSMC_WRITE_PROTECTED  == p_nor_control->wprot)
		val |= FSMC_WRITE_PROTECTED ;


	switch(bank_no)
	{
		case FSMC_NOR_CHIP0:
			*GenMemCtrl0 |=val; /*ORING as FSMC_WriteProtectNor might have been called & wprot might be set*/
			break;
		case FSMC_NOR_CHIP1:
			*GenMemCtrl1 |=val;
			break;
		case FSMC_NOR_CHIP2:
			*GenMemCtrl2 |=val;
			break;
		case FSMC_NOR_CHIP3:
			*GenMemCtrl3 |=val;
			break;
		case FSMC_NOR_CHIP4:
			*GenMemCtrl4 |=val;
			break;
		case FSMC_NOR_CHIP5:
			*GenMemCtrl5 |=val;
			break;
		case FSMC_NOR_CHIP6:
			*GenMemCtrl6 |=val;
			break;
		case FSMC_NOR_CHIP7:
			*GenMemCtrl7 |=val;
			break;
		default:
			error = FSMC_INVALID_BANK_NO;
	}
	return error;
}

/*--------------------------------------------------------------------------------*/
/*		 NAME :	FSMC_SetNandControl()  					  */
/*--------------------------------------------------------------------------------*/
/* DESCRIPTION: This routine set PC_NAND control register of  FSMC  		  */
/*				                      				  */
/* PARAMETERS :									  */
/*         IN : bank_no 		: fsmc_bank number.			  */
/*         	    p_nand_control  : Control information.		          */
/*		OUT : None 							  */
/*	 RETURN :	FSMC_OK	: For normal operations			          */
/*			FSMC_INVALID_BANK_NO : If bank_no is invalid	          */
/*			FSMC_BAD_PARAM : if bank_ctrl is not for Nand flash       */
/*  REENTRANCY: NA       			          			  */
/*----------------------------------------------------------------------------*/

PUBLIC t_fsmc_error FSMC_SetNandControl(t_fsmc_bank_index bank_no,
		t_struct_fsmc_pc_nand_control* p_nand_control)
{
	t_uint32 val=0;
	t_fsmc_error error;
	error=FSMC_OK;

	if(p_nand_control->wait_on == FSMC_WAIT_ON)
		val |= WAIT_ON;

	if(p_nand_control->enable_bank == FSMC_BANK_ENABLE)
		val |= NAND_ENABLE;

	if(p_nand_control->mem_type == FSMC_NAND_MEM)
		val |= NAND_TYPE;

	switch(p_nand_control->dev_width)
	{
		case  FSMC_ACCESS_8:
			val |=NAND_DEV_WIDTH_8;
			break;
		case FSMC_ACCESS_16:
			val |=NAND_DEV_WIDTH_16 ;
			break;
		case FSMC_ACCESS_32:
			val |=NAND_DEV_WIDTH_32;
			break;
		default:
			error=FSMC_INVALID_ACCESS_WIDTH;
	}

	val |= ( ((p_nand_control->tclr << TCLR_SHIFT) & TCLR_MSK)  | ((p_nand_control->tar << TAR_SHIFT ) & TAR_MSK));

	switch(bank_no)
	{
		case FSMC_BANK0:
			*GenMemCtrl_PC0 = val;
			break;
		case FSMC_BANK1:
			*GenMemCtrl_PC1 = val;
			break;

		case FSMC_BANK2:
			*GenMemCtrl_PC2 = val;
			break;
		case FSMC_BANK3:
			*GenMemCtrl_PC3 = val;
			break;
		default:
			error = FSMC_INVALID_BANK_NO;
			return error;
	}

	return error;
}
/*---------------------------------------------------------------------------*/
/*		 NAME :	FSMC_SetNandTiming() 				     */
/*---------------------------------------------------------------------------*/
/* DESCRIPTION: This routine set PC_NAND timing register of  FSMC  	     */
/*				                      			     */
/* PARAMETERS :								     */
/*        IN : bank_no : FSMC bank number. 				     */
/*         	  p_bank_tmng: Timing information			     */
/*         	  mem_space_type : Timing for particular address space.	     */
/*		OUT : 							     */
/*	 RETURN : FSMC_OK	: Normal operation                           */
/*			  FSMC_INVALID_MEMORY_SPACE: Invalid memory space    */
/*			  FSMC_INVALID_BANK_NO : If bank_no is invalid	     */
/*  REENTRANCY: NA							     */
/*---------------------------------------------------------------------------*/
PUBLIC t_fsmc_error FSMC_SetNandTiming (t_fsmc_bank_index bank_no,
		t_fsmc_pc_nand_mem_space mem_space_type,
		t_struct_fsmc_pc_nand_tmng* p_nand_timing)
{
	t_uint32 nand_mem_timing=0;
	t_fsmc_error error;
	error=FSMC_OK;


	nand_mem_timing = ((p_nand_timing->data_bus_hiz_phase << Thiz_shift) &Thiz_msk ) | (( p_nand_timing->addr_hold_phase << Thold_shift)& Thold_msk) ;
	nand_mem_timing |= ((p_nand_timing->wait_phase	<< Twait_shift)& Twait_msk) | (( p_nand_timing->addr_setup_phase << Tset_shift)& Tset_msk) ;



	switch(mem_space_type)
	{
		case FSMC_PC_NAND_IO_SPACE:
			switch (bank_no)
			{
				case FSMC_BANK0:
					*GenMemCtrl_IO_ATA0 = nand_mem_timing ;
					break;

				case FSMC_BANK1:
					*GenMemCtrl_IO_ATA1 = nand_mem_timing ;
					break;

				case FSMC_BANK2:
				case FSMC_BANK3:
				default: 		error = FSMC_INVALID_BANK_NO;
			}
			break;


		case FSMC_PC_NAND_ATTR_MEM_SPACE:
			switch (bank_no)
			{
				case FSMC_BANK0:
					*GenMemCtrl_Attrib0	 = nand_mem_timing ;
					break;
				case FSMC_BANK1:
					*GenMemCtrl_Attrib1	= nand_mem_timing ;
					break;

				case FSMC_BANK2:
				case FSMC_BANK3:
				default: 		error = FSMC_INVALID_BANK_NO;
			}
			break;


		case FSMC_PC_NAND_COMMON_MEM_SPACE:
			switch (bank_no)
			{
				case FSMC_BANK0:
					*GenMemCtrl_Comm0 = nand_mem_timing ;
					break;
				case FSMC_BANK1:
					*GenMemCtrl_Comm1 = nand_mem_timing ;
					break;

				case FSMC_BANK2:
				case FSMC_BANK3:
				default: 		error = FSMC_INVALID_BANK_NO;
			}
			break;

		default:
			{
				error = FSMC_INVALID_MEMORY_SPACE;
				return error;
			}
	}

	return error;
}

/*---------------------------------------------------------------------------*/
/*		 NAME :	FSMC_GetNandTiming()       			     */
/*---------------------------------------------------------------------------*/
/* DESCRIPTION: This routine get PC_NAND timing register of  FSMC  	     */
/*				                      			     */
/* PARAMETERS :								     */
/*         IN : bank_no : FSMC bank number. 				     */
/*         	  p_bank_tmng: Timing information			     */
/*         	  mem_space_type : Timing for particular address space.	     */
/*		OUT : 							     */
/*	 RETURN : FSMC_OK	: Normal operation			     */
/*	          FSMC_INVALID_MEMORY_SPACE: Invalid memory space            */
/*			  FSMC_INVALID_BANK_NO : If bank_no is invalid	     */
/*  REENTRANCY: NA							     */
/*---------------------------------------------------------------------------*/

PUBLIC t_fsmc_error FSMC_GetNandTiming ( t_fsmc_bank_index bank_no,
		t_fsmc_pc_nand_mem_space mem_space_type,
		t_struct_fsmc_pc_nand_tmng* p_nand_timing)
{
	t_uint32 nand_mem_timing=0;
	t_fsmc_error error;
	error=FSMC_OK;
	switch(mem_space_type)
	{
		case FSMC_PC_NAND_IO_SPACE:
			switch (bank_no)
			{
				case FSMC_BANK0:
					nand_mem_timing = *GenMemCtrl_IO_ATA0 ;
					break;

				case FSMC_BANK1:
					nand_mem_timing = *GenMemCtrl_IO_ATA1 ;
					break;

				case FSMC_BANK2:
					nand_mem_timing = *GenMemCtrl_IO_ATA2;
					break;
				case FSMC_BANK3:
					nand_mem_timing = *GenMemCtrl_IO_ATA3;
					break;

				default: 		error = FSMC_INVALID_BANK_NO;
			}
			break;


		case FSMC_PC_NAND_ATTR_MEM_SPACE:
			switch (bank_no)
			{
				case FSMC_BANK0:
					nand_mem_timing = *GenMemCtrl_Attrib0 ;
					break;

				case FSMC_BANK1:
					nand_mem_timing = *GenMemCtrl_Attrib1 ;
					break;

				case FSMC_BANK2:
					nand_mem_timing = *GenMemCtrl_Attrib2;
					break;
				case FSMC_BANK3:
					nand_mem_timing = *GenMemCtrl_Attrib3;
					break;

				default: 		error = FSMC_INVALID_BANK_NO;
			}
			break;


		case FSMC_PC_NAND_COMMON_MEM_SPACE:
			switch (bank_no)
			{
				case FSMC_BANK0:
					nand_mem_timing = *GenMemCtrl_Comm0 ;
					break;

				case FSMC_BANK1:
					nand_mem_timing = *GenMemCtrl_Comm1;
					break;

				case FSMC_BANK2:
					nand_mem_timing = *GenMemCtrl_Comm2;
					break;
				case FSMC_BANK3:
					nand_mem_timing = *GenMemCtrl_Comm3;
					break;

				default: 		error = FSMC_INVALID_BANK_NO;
			}

			break;
	}



	p_nand_timing->data_bus_hiz_phase = (nand_mem_timing & Thiz_msk) >> Thiz_shift;
	p_nand_timing->addr_hold_phase = (nand_mem_timing & Thold_msk) >> Thold_shift;
	p_nand_timing->wait_phase =(nand_mem_timing & Twait_msk) >> Twait_shift ;
	p_nand_timing->addr_setup_phase = (nand_mem_timing & Tset_msk) >> Tset_shift;

	return error;
}


/*----------------------------------------------------------------------------*/
/*		 NAME :	FSMC_ReadEcc()			    		     */
/*----------------------------------------------------------------------------*/
/* DESCRIPTION: This routine returns the ECC value computed for last page     */
/*			  transaction.	                                      */
/* PARAMETERS :								      */
/*         IN : bank_no: FSMC Bank Number				      */
/*		OUT : 							      */
/*	 RETURN : t_uint32: Value of ECC register			      */
/*  REENTRANCY: NA             						      */
/*----------------------------------------------------------------------------*/

PUBLIC t_uint32 FSMC_ReadEcc( t_fsmc_bank_index bank_no)
{
	t_uint32 ecc=0;
	t_fsmc_error error;
	error=FSMC_OK;

	switch(bank_no)
	{
		case FSMC_BANK0:
			ecc=*GenMemCtrl_ECCr0;
			break;
		case FSMC_BANK1:
			ecc=*GenMemCtrl_ECCr1;
			break;
		case FSMC_BANK2:
			ecc=*GenMemCtrl_ECCr2;
			break;
		case FSMC_BANK3:
			ecc=*GenMemCtrl_ECCr3;
			break;
		default:
			error = FSMC_INVALID_BANK_NO;
	}
	if(error != FSMC_OK)
		DEBUGOUT1("error:n Read ECC %d",error);
	return(ecc);
}

/*---------------------------------------------------------------------------*/
/*		 NAME :	FSMC_NandEccLogic()				 */
/*---------------------------------------------------------------------------*/
/* DESCRIPTION: This routine Enable or Disable the ECC logic		 */
/*              					                  */
/* PARAMETERS :								*/
/*       IN : bank_no: FSMC Bank Number					 */
/*         	: status : To Enable or Disable the ECC logic		*/
/*			: page_len : ECC 512/256			*/
/*	    OUT :							 */
/*   RETURN : FSMC_OK	: Normal operation				 */
/*			  FSMC_INVALID_BANK_NO : If bank_no is invalid	 */					        				
/*  REENTRANCY: NA							 */
/*---------------------------------------------------------------------------*/

PUBLIC t_fsmc_error FSMC_NandEccLogic(t_fsmc_bank_index bank_no, t_fsmc_ecc status,t_ecc_pagelen page_len)
{

	t_fsmc_error error=FSMC_OK;

	switch (bank_no)
	{   case FSMC_BANK0:
		if (FSMC_ECC_ENABLE == status)
		{
			//if (page_len == ECCPAGELEN_512 )
			if (page_len == FSMC_ECC_PAGELEN_512 )
				*GenMemCtrl_PC0 &= ECCPAGELEN_512;
			else if (page_len == FSMC_ECC_PAGELEN_256 )
				*GenMemCtrl_PC0 |= ECCPAGELEN_256;
			else
				error=FSMC_BAD_PARAM;

			*GenMemCtrl_PC0 &= ECCDISABLED;    /* reset ECC logic      */
			*GenMemCtrl_PC0 |=  ECCENABLED;
		}
		else
		{
			*GenMemCtrl_PC0 &= ECCDISABLED;
		}
		break;

		case FSMC_BANK1:
		if (FSMC_ECC_ENABLE == status)
		{
			//if (page_len == ECCPAGELEN_512 )
			if (page_len == FSMC_ECC_PAGELEN_512 )
				*GenMemCtrl_PC1 &= ECCPAGELEN_512;
			else if (page_len == FSMC_ECC_PAGELEN_256 )
				*GenMemCtrl_PC1 |= ECCPAGELEN_256;
			else
				error=FSMC_BAD_PARAM;

			*GenMemCtrl_PC1 &= ECCDISABLED;    /* reset ECC logic      */
			*GenMemCtrl_PC1 |=  ECCENABLED;
		}
		else
		{
			*GenMemCtrl_PC1 &= ECCDISABLED;
		}
		break;

		case FSMC_BANK2:
		if (FSMC_ECC_ENABLE == status)
		{
			//if (page_len == ECCPAGELEN_512 )
			if (page_len == FSMC_ECC_PAGELEN_512 )
				*GenMemCtrl_PC2 &= ECCPAGELEN_512;
			else if (page_len == FSMC_ECC_PAGELEN_256 )
				*GenMemCtrl_PC2 |= ECCPAGELEN_256;
			else
				error=FSMC_BAD_PARAM;

			*GenMemCtrl_PC2 &= ECCDISABLED;    /* reset ECC logic      */
			*GenMemCtrl_PC2 |=  ECCENABLED;
		}
		else
		{
			*GenMemCtrl_PC2 &= ECCDISABLED;
		}
		break;

		case FSMC_BANK3:
		if (FSMC_ECC_ENABLE == status)
		{
			//if (page_len == ECCPAGELEN_512 )
			if (page_len == FSMC_ECC_PAGELEN_512 )
				*GenMemCtrl_PC3 &= ECCPAGELEN_512;
			else if (page_len == FSMC_ECC_PAGELEN_256 )
				*GenMemCtrl_PC3 |= ECCPAGELEN_256;
			else
				error=FSMC_BAD_PARAM;

			*GenMemCtrl_PC3 &= ECCDISABLED;    /* reset ECC logic      */
			*GenMemCtrl_PC3 |=  ECCENABLED;
		}
		else
		{
			*GenMemCtrl_PC3 &= ECCDISABLED;
		}
		break;
		default: 		error=FSMC_INVALID_BANK_NO;
					DEBUGOUT1("error NANDEcclogic %d",error);
	}
	return error;          
}
/*--------------------------------------------------------------------------*/
/*	NAME		: FSMC_WriteProtectNor				    */
/*--------------------------------------------------------------------------*/
/*	DESCRIPTION : Write protect NOR bank 				    */
/*	IN			: bank_no,				    */
/*				  protect :Enable/Disable write protection  */
/*	INOUT		: None						    */
/*	OUT			: None					    */
/*	RETURN		: FSMC_OK:					    */
/*				  FSMC_INVALID_BANK_NO : If bank_no is invalid */
/*	REENTRANCY	: NA					            */
/*--------------------------------------------------------------------------*/
PUBLIC t_fsmc_error FSMC_WriteProtectNor(   t_fsmc_bank_index    bank_no,
		t_write_protected         protect)
{
	t_fsmc_error error = FSMC_OK;


	if(FSMC_WRITE_PROTECTED == protect)
	{
		switch(bank_no)
		{
			case FSMC_NOR_CHIP0:
				*GenMemCtrl0 |=WPROT; //ORING as FSMC_WriteProtectNor might have been called & wprot might be set
				break;
			case FSMC_NOR_CHIP1:
				*GenMemCtrl1 |=WPROT;
				break;
			case FSMC_NOR_CHIP2:
				*GenMemCtrl2 |=WPROT;
				break;
			case FSMC_NOR_CHIP3:
				*GenMemCtrl3 |=WPROT;
				break;
			case FSMC_NOR_CHIP4:
				*GenMemCtrl4 |=WPROT;
				break;
			case FSMC_NOR_CHIP5:
				*GenMemCtrl5 |=WPROT;
				break;
			case FSMC_NOR_CHIP6:
				*GenMemCtrl6 |=WPROT;
				break;
			case FSMC_NOR_CHIP7:
				*GenMemCtrl7 |=WPROT;
				break;
			default:
				error = FSMC_INVALID_BANK_NO;
		}
	}
	else
	{
		switch(bank_no)
		{
			case FSMC_NOR_CHIP0:
				*GenMemCtrl0 |=WPROT_DIS; //ORING as FSMC_WriteProtectNor might have been called & wprot might be set
				break;
			case FSMC_NOR_CHIP1:
				*GenMemCtrl1 |=WPROT_DIS;
				break;
			case FSMC_NOR_CHIP2:
				*GenMemCtrl2 |=WPROT_DIS;
				break;
			case FSMC_NOR_CHIP3:
				*GenMemCtrl3 |=WPROT_DIS;
				break;
			case FSMC_NOR_CHIP4:
				*GenMemCtrl4 |=WPROT_DIS;
				break;
			case FSMC_NOR_CHIP5:
				*GenMemCtrl5 |=WPROT_DIS;
				break;
			case FSMC_NOR_CHIP6:
				*GenMemCtrl6 |=WPROT_DIS;
				break;
			case FSMC_NOR_CHIP7:
				*GenMemCtrl7 |=WPROT_DIS;
				break;
			default:
				error = FSMC_INVALID_BANK_NO;

		}
	}
	return error;
}

/*------------------------------------------------------------------------*/
/* NAME 	fsmc_IsWaitEnabled                                     	  */
/*------------------------------------------------------------------------*/
/* DESCRIPTION :This function test the wait enable bit            */
/* PARAMETERS  :                                                          */
/*IN	 : bank_no: The FSMC bank number             */
/*      RETURN : TRUE/FALSE Status of wait biti				  */
/*      FSMC_INVALID_BANK_NO : If bank_no is invalid			  */
/*   REENTRANCY: Reentrant						  */
/*------------------------------------------------------------------------*/
PUBLIC t_fsmc_error SPEAR_FSMC_IsWaitEnabled(t_fsmc_bank_index bank_no)
{
	switch(bank_no)
	{
		case FSMC_BANK0:
			return (*GenMemCtrl_PC0 & WAIT_ON);

		case FSMC_BANK1:
			return (*GenMemCtrl_PC1 & WAIT_ON);
		case FSMC_BANK2:
			return (*GenMemCtrl_PC2 & WAIT_ON);
		case FSMC_BANK3:
			return (*GenMemCtrl_PC3 & WAIT_ON);
	}
	return (FSMC_INVALID_BANK_NO);
}
