/*
 *
 * puma5_intc.h 
 * Description:
 * puma5 intc platform data
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

#ifndef _PUMA5_INTC_H
#define _PUMA5_INTC_H

/* The following defines are  speciifc to PUMA5 RTL/SoC */

#define NUM_SYSTEM_PRIMERY_INTS      ( 64 )
#define NUM_SYSTEM_SECONDARY_INTS    ( 32 )
#define NUM_MAX_SYSTEM_INTS          ( NUM_SYSTEM_PRIMERY_INTS +\
                                       NUM_SYSTEM_SECONDARY_INTS )
#define HOST_MAX_INTERRUPTS          ( 2 )
#define HOST_ARM_IRQ_NUM             ( 1 )  /* ARM nIRQ Number */
/* Number of INTC Registers Mapped in the given RTL/SoC */
#define NUM_PACER_REGS               ( 4 )
#define NUM_RAW_STATUS_REGS          ( 3 )  
#define NUM_ENABLED_STATUS_REGS      ( 3 )
#define NUM_ENABLE_SET_REGS          ( 3 )
#define NUM_ENABLE_CLEAR_REGS        ( 3 )  
#define NUM_CHANNEL_MAP_REGS         ( 24 )
#define NUM_HOST_INT_MAP_REGS        ( 2 )
#define NUM_PRIORITY_HINT_REGS       ( 2 )
#define NUM_POLARITY_REGS            ( 3 ) 
#define NUM_TYPE_REGS                ( 3 )
#define NUM_SECURE_ENABLE_SET_REGS   ( 3 ) 
#define NUM_SECURE_ENABLE_CLEAR_REGS ( 3 )
#define NUM_NESTING_LVL_REGS         ( 2 ) 
#define NUM_ENABLE_HINT_REGS         ( 1 ) 
#endif /*_PUMA5_INTC_H */


