/*
 *
 * avalanche_intc.h
 * Description:
 * intc header
 *
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _AVALANCHE_INTC_H
#define _AVALANCHE_INTC_H

#include <linux/irq.h>
#include <asm-arm/arch-avalanche/generic/pal.h>

/*****************************************************************************
 Avalanche Interrupt number
******************************************************************************/
#define AVINTNUM(x)     ( (x) )


/*******************************************************************************
 * Linux Interrupt number
 *******************************************************************************/

#define LNXINTNUM(x)                 ( x )
#define AVALANCHE_INT_END_PRIMARY    ( NUM_SYSTEM_PRIMERY_INTS )
#define AVALANCHE_INT_END_SECONDARY  ( AVALANCHE_INT_END_PRIMARY + NUM_SYSTEM_SECONDARY_INTS )  
#define AVALANCHE_INTC_END           ( AVALANCHE_INT_END_SECONDARY )


#if defined(CONFIG_ARM_AVALANCHE_VLYNQ)
  #define AVALANCHE_INT_END_LOW_VLYNQ  (AVALANCHE_INTC_END + 32)
  #ifdef AVALANCHE_VLYNQ_HIGH_CONTROL_BASE
    #define AVALANCHE_INT_END_VLYNQ      (AVALANCHE_INTC_END + 32 * 2)
  #else
    #define AVALANCHE_INT_END_VLYNQ      (AVALANCHE_INTC_END + 32 * 1)
  #endif
  #define AVALANCHE_INT_END             AVALANCHE_INT_END_VLYNQ
#else
  #define AVALANCHE_INT_END            ( AVALANCHE_INT_END_SECONDARY )
#endif

#define NUM_MAX_CHANNEL              ( NUM_CHANNELS_PER_REG * \
                                       NUM_CHANNEL_MAP_REGS )
#define NUM_SYS_INTS_PER_REG         ( 4 )
#define NUM_CHANNELS_PER_REG         ( 4 )
#ifndef NUM_INTS_PER_REG
#define NUM_INTS_PER_REG             ( 32 )
#endif
#define INTC_MAX_NUMBER_PACERS       ( 16 )
#define INTC_NUM_MAX_CHANNEL         ( 1024 )

struct avalanche_intc_vect_t
{
    irq_desc_t *irq_desc;
};


/****************************************************************************
 * Avalanche interrupt controller register base 
 ****************************************************************************/
#define AVALANCHE_ICTRL_REGS_BASE    AVALANCHE_INTC_BASE

/* NOTE: Read "ICTRL" as 'interrupt Controller' */
struct avalanche_ictrl_pacer_regs /* Avalanche Interrupt control pacer regs*/
{
    volatile unsigned long icpparamr; /* ICTRL Pacer Parameter Reg */
    volatile unsigned long icpdecr;   /* ICTRL Pacing Decrement Reg */
    volatile unsigned long icpmap;    /* ICTRL Pacer map Reg */
    volatile unsigned long reserved;  /* unused Reg */
};  
typedef struct avalanche_ictrl_pacer_regs ICTRL_PACER_REGS;

struct avalanche_ictrl_regs /* Avalanche Interrupt control registers */
{
   volatile unsigned long icrevr;      /* ICTRL Revision Reg 0x00*/
   volatile unsigned long iccntlr;     /* ICTRL Control Reg  0x04*/
   volatile unsigned long unused1;     /* 0x08 */
   volatile unsigned long ichcntlr;    /* ICTRL Host control Reg 0x0C */
   volatile unsigned long icglber;     /* ICTRL Global Enable Reg 0x10 */
   volatile unsigned long unused2[2];  /* 0x14  to 0x18*/
   volatile unsigned long icglbnlr;    /* ICTRL Global Nesting Level Reg 0x1C */      
   volatile unsigned long icsisr;      /* ICTRL Status index Set Reg 0x20 */
   volatile unsigned long icsicr;      /* ICTRL Status index Clear Reg 0x24 */
   volatile unsigned long iceisr;      /* ICTRL enable index Set Reg 0x28*/
   volatile unsigned long iceicr;      /* ICTRL enable  index Clear Reg 0x2C */
   volatile unsigned long icgwer;      /* ICTRL Global Wakeup Enable Reg 0x30 */
   volatile unsigned long ichinteisr;  /* ICTRL host interrupt enable index 
                                          Set  Reg 0x34 */
   volatile unsigned long ichinteicr;  /* ICTRL host interrupt enable index 
                                          Clear Reg 0x38 */
   volatile unsigned long unused4;     /* 0x3c */  
   volatile unsigned long icpprer;     /* ICTRL Pacer Prescale  Reg 0x40 */
   volatile unsigned long unused5[3];  /* 0x44  to 0x4C */  
   volatile unsigned long icvbr;       /* ICTRL Vector Base Reg 0x50 */
   volatile unsigned long icvszr;      /* ICTRL Vector Size Reg 0x54*/
   volatile unsigned long icvnullr;    /* ICTRL Vector Null Reg 0x58 */
   volatile unsigned long unused7[9];  /* 0x5c  to 0x7c */  
   volatile unsigned long icgpir;      /* ICTRL Global Priority Index Reg 0x80 */
   volatile unsigned long icgpvr;      /* ICTRL Global Priority vector Reg0x84 */
   volatile unsigned long unused17[2]; /* 0x88  to 0x8c */  
   volatile unsigned long icgsecinter; /* ICTRL Global security Interrupt 
                                          Enable Reg 0x90 */
   volatile unsigned long icsecpir;    /* ICTRL Secure prioritised Index Reg 0x94 */
   volatile unsigned long unused19[26];/* 0x098  to 0x0FC */  
   ICTRL_PACER_REGS  icpacer [16];        /* ICTRL pacing Regs array 0x100 */
   volatile unsigned long icrstar[32];    /* ICTRL Raw status  Reg 0x200 */
   volatile unsigned long icestar[32];    /* ICTRL Enabled Status Reg 0x280 */
   volatile unsigned long icenbsr[32];    /* ICTRL Enabler set Reg 0x300 */
   volatile unsigned long icenbcr[32];    /* ICTRL Enabler Clear Reg 0x380*/
   volatile unsigned long icchnlmpr[256]; /* ICTRL Channel Map Reg 0x400 */   
   volatile unsigned long ichmpr[64];     /* ICTRL Host Map Reg 0x800 */
   volatile unsigned long ichintpir[256]; /* ICTRL Host Interrupt
                                             Priotrized Index  Reg 0x900 */
   volatile unsigned long icpolr[32];     /* ICTRL polarity Reg 0xD00 */
   volatile unsigned long ictypr[32];     /* ICTRL type Reg 0xD80 */
   volatile unsigned long icwuper[64];    /* ICTRL Wakeup Enable Reg 0xE00 */
   volatile unsigned long icdbgsetr[64];  /* ICTRL Debug Set Reg 0xF00*/
   volatile unsigned long icsecer[64];    /* ICTRL secure Enble Reg 0x1000 */
   volatile unsigned long ichintnlr[256]; /* ICTRL Host Interrupt 
                                             Nesting level Reg 0x1100 */
   volatile unsigned long ichinter[8];    /* ICTRL Host Interrupt
                                              Enable Reg 0x1500 */
   volatile unsigned long unused45[184];  /* 0x1520  to 0x1800 */  
 };


/****************************************************************************
 * Legacy Support APIs
 ****************************************************************************/
#ifdef  CONFIG_AVALANCHE_INTC_LEGACY_SUPPORT
#define INTC_TYPE_LEVEL             ( 0 )
#define INTC_TYPE_EDGE              ( 1 )
#define INTC_POLARITY_ACTIVE_LOW    ( 0 )
#define INTC_POLARITY_ACTIVE_HIGH   ( 1 )

/* Set Interrupt type  */
int avalanche_intc_set_interrupt_type( unsigned int irq ,
                                       unsigned char int_type);
int avalanche_intc_get_interrupt_type( unsigned int irq );
/* Set Interrupt polarity */
int avalanche_intc_set_interrupt_polarity( unsigned int irq,
                                           unsigned char int_polarity);
int avalanche_intc_get_interrupt_polarity( unsigned int irq );
#endif /* CONFIG_AVALANCHE_INTC_LEGACY_SUPPORT */

/*****************************************************************************
 * Pacing Support APIs
 ****************************************************************************/
#ifdef CONFIG_AVALANCHE_INTC_PACING

/* Pacing prescale Reg */
#define INTC_GLOBAL_PACER_TEST_MODE        ( 0x80000000 )
#define INTC_GLOBAL_PACER_BYPASSMODE       ( 0x10000000 )
#define INTC_GLOBAL_PACER_NORMALMODE       ( 0x00000FFF )

int avalanche_intc_get_glb_pacer_mode( void );
int avalanche_intc_set_glb_pacer_mode( unsigned int glb_pacer_mode );
int avalanche_intc_get_glb_pacer_prescale_count( void );
int avalanche_intc_set_glb_pacer_prescale_count( unsigned int prescale_cnt );

/* Pacing parametr Reg */
#define INTC_PACER_TEST_MODE               ( 0x40000000 )
#define INTC_PACER_FREQ_BASED              ( 0xCFFFFFFF )
#define INTC_PACER_COUNT_BASED             ( 0x10000000 )
#define INTC_PACER_TIME_BASED              ( 0x20000000 )
#define INTC_PACER_COUNT_AND_TIME_BASED    ( 0x30000000 )

/* Pacing Control Register */
int avalanche_intc_enable_pacer_bypass( unsigned int pacer_num );
int avalanche_intc_disable_pacer_bypass( unsigned int pacer_num );
int avalanche_intc_set_pacer_mode( unsigned int pacer_num,
                                   unsigned int mode );
int avalanche_intc_get_pacer_count( unsigned int pacer_num );
int avalanche_intc_set_pacer_count( unsigned int pacer_num,
                                    unsigned int cnt_val );
int avalanche_intc_set_pacer_restart_mode( unsigned int pacer_num  );
int avalanche_intc_clear_pacer_restart_mode( unsigned int pacer_num );
int avalanche_intc_get_pacer_max_val( unsigned int pacer_num );
int avalanche_intc_set_pacer_max_val( unsigned int pacer_num,
                                      unsigned int max_value );

/* Pacing Control Value */
int avalanche_intc_get_serviced_int_count( unsigned int pacer_num );
int avalanche_intc_set_serviced_int_count( unsigned int pacer_num,
                                           unsigned int cnt_val );

/* Pacing Map Register */
int avalanche_intc_map_interrupt_to_pacer( unsigned int irq,
                                           unsigned int pacer_num );
int avalanche_intc_unmap_interrupt_to_pacer( unsigned int irq,
                                             unsigned int pacer_num );
#endif /*CONFIG_AVALANCHE_INTC_PACING */
                                                               
#endif /* _AVALANCHE_INTC_H */
