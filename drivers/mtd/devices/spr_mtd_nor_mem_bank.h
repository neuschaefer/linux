/*
 * linux/include/asm/arch/memory_banks.h
 *
 *  Copyright (C) 2006 ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
 * AUTHOR :  Siva Borra <siva.borra@st.com> */


#ifndef  MEMORY_BANKS_H
#define MEMORY_BANKS_H

#ifdef CONFIG_MTD_STATIC_MEMORY_BANKS 
struct memory_banks
{
 int num_banks;
 unsigned long mem_bank1;
 unsigned long mem_bank2;
 unsigned long mem_bank3;
 unsigned long mem_bank4;
};

static struct 
memory_banks memory_bank_info={
			       2,
			       SFLASH_DEVID_M25P64,
			       SFLASH_DEVID_M25P64,
			       0x00,
			       0x00
			      };
#endif
#endif

/* End of file - memory_banks.h */
