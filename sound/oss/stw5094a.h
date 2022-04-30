/*
===============================================================================
                Copyright (C) 2008-2009 by STMicroelectronics.
                              All rights reserved.

FILE:        sound/oss/stw5094a.h
VERSION:     1.0 (alpha)
UPDATED:     20/04/2009
AUTHOR(S):   STM, Computer Systems Division, Beijing/Italy VoIP Team
DESCRIPTION:
Header file for audio driver based on SPEAr Basic SoC and STw5094A codec.
===============================================================================
*/

#ifndef STW5094A_H
#define STW5094A_H

#define STW5094A_DRIVER_VERSION "2.0 (SNOM 1.2)"

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/soundcard.h>
#include <linux/interrupt.h>
#include <asm/arch/spear.h>
#include <linux/types.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/hcl_defs.h>
#include <linux/delay.h>
#include "sound_config.h"


volatile unsigned int *SPEARBASIC_TDM_IMASK = NULL;
volatile unsigned int *SPEARBASIC_RAS4_CLK_SYNT = NULL;
volatile unsigned int *SPEARBASIC_RAS_CLK_ENB = NULL;
volatile unsigned int *SPEARBASIC_CORE_CLK_CFG = NULL;

volatile unsigned int SPEARBASIC_TDM_CTL = 0;
volatile unsigned int SPEARBASIC_TDM_ACT = 0;
volatile unsigned int SPEARBASIC_TDM_DAT = 0;
volatile unsigned int SPEARBASIC_IT_TDM  = 0;


#define INTPTR (volatile unsigned int *)
#define SHORTPTR (volatile unsigned short *)
#define CHARPTR (volatile unsigned char *)

// macros for TLC/TDM register access
#define SPEARBASIC_START_STW5094A_TDM				0x50000000
#define VA_SPEARBASIC_START_STW5094A_TDM_CTR                    IO_ADDRESS(0x50000000)
#define VA_SPEARBASIC_START_STW5094A_TDM_ACT                    IO_ADDRESS(0x50010000)
#define VA_SPEARBASIC_START_STW5094A_TDM_DAT                    IO_ADDRESS(0x50030000)
#define VA_SPEARBASIC_IT_TDM					IO_ADDRESS(0x50060000)

//#define TDM_IMASK         (SHORTPTR(SPEARBASIC_TDM_CTL+0x54))
#define TDM_IMASK         (INTPTR(SPEARBASIC_TDM_CTL+0x54))
#define TLC_ISTATUS       (INTPTR(SPEARBASIC_TDM_CTL+0x58))
#define TDM_CONF          (INTPTR(SPEARBASIC_TDM_CTL+0x04))
#define TDM_TIMESLOT_NBR  (SHORTPTR(SPEARBASIC_TDM_CTL+0x38))
#define TDM_FRAME_NBR     (SHORTPTR(SPEARBASIC_TDM_CTL+0x3C))
#define TDM_SYNC_GEN      (INTPTR(SPEARBASIC_TDM_CTL+0x40))
#define I2S_CONF	      (INTPTR(SPEARBASIC_TDM_CTL+0x4C))
#define IT_GEN		      (INTPTR(SPEARBASIC_TDM_CTL+0x24))

#define TDM_ACTION_MEMORY (INTPTR(SPEARBASIC_TDM_ACT))
#define TDM_BUFFER        (INTPTR(SPEARBASIC_TDM_DAT))
#define TDM_BUFFER1       ((INTPTR(SPEARBASIC_TDM_DAT)) + 0x1000)
#define VA_ITtdm          (CHARPTR(SPEARBASIC_IT_TDM + 0x03))
#define VA_DMAtdm         (INTPTR(SPEARBASIC_IT_TDM + 0x08))
// examples:
// a register may be just written as *TDM_FRAME_NBR = <value>
// a register may be just read as <var> = *TDM_FRAME_NBR


/******************************************************************************
                            GENERAL DEFINITIONS
******************************************************************************/

#define IRQ_STW5094a_TDM (28)

//ioctl used to check if there was some real time violation,
//like buffer is read/write at wrong times
#define SNDCTL_DSP_GET_RTIME_VIOLATIONS 0xEC0F0B1A
#define SNDCTL_DSP_READREG  0xEC0F0B1B
#define SNDCTL_DSP_WRITEREG 0xEC0F0B1C

typedef enum 
{
  MODE_HANDSET=0,   // MIC1 (ODM) or MIC2 (EVB) + handset output
  MODE_HANDSFREE=1, // MIC2 (ODM) or MIC1 (EVB) + speaker output
  MODE_HEADSET=2,    // MIC3 + headset output
  MODE_HANDSET_LS=3,    // MIC1 + handset output + speaker output
  MODE_HEADSET_LS=4    // MIC1 + headset output + speaker output
} codec_mode_t;

typedef enum 
{
  RATE_NARROWBAND=0,
  RATE_WIDEBAND=1
} codec_rate_t;

typedef enum
{
	BL_10ms=0,
	BL_20ms=1,
	BL_30ms=3
} codec_buffer_lenght_t;

/******************************************************************************
                             PRIVATE VARIABLES
******************************************************************************/

struct dev_state 
{
	int    dsp;   // DSP device handle
	int    mixer; // MIXER device handle
	codec_mode_t mode;
	codec_rate_t rate;
	codec_buffer_lenght_t buf_len;
};

#endif
