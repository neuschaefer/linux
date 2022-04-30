/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
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
#ifndef SPEAR_JPEG_H
#define SPEAR_JPEG_H

#include <linux/serial_core.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <asm/arch/spr_major.h>
#include "sprbasic_jpeg_syn_hal.h"

#define SPEAR_JPEG_NAME 	"spear_jpeg"
#define SPEAR_JPEG_MAJOR	MAJOR_JPEG

/*Timeout for sleep*/
#define JPEG_TIMEOUT 500

/*JPEG Device types*/
typedef enum
{
	JPEG_READ,
	JPEG_WRITE,
	DEV_TYPE_MAX
}t_JPEGDevType;

/*Define the type used for JPEG Device.*/
typedef struct
{
	void *CurrentOwner;
	uint DevNum[DEV_TYPE_MAX];
	struct cdev Cdev[DEV_TYPE_MAX];
	volatile uint FlagArrived[DEV_TYPE_MAX];
	wait_queue_head_t WaitQueue[DEV_TYPE_MAX];
	t_DMAChannelId ChannelId[DEV_TYPE_MAX];
	uint *RBuf[2];
	int CurrentDecodingBuf;
	uint *WBuf;
	uint NumPageReq[DEV_TYPE_MAX];
	uint ImgSize[DEV_TYPE_MAX];
	uint BufSize[DEV_TYPE_MAX];
	t_bool IsJpegOpen[DEV_TYPE_MAX];
	volatile t_bool IsDecodingOver;
}t_JPEGDev;

/***********************************************************************************
 * PUBLIC Functions
 ***********************************************************************************/
/*This API is used to enable JPEG interrupt.*/
static int __init SPEAR_JPEG_Init(void);
static void __exit SPEAR_JPEG_Exit(void);
#endif /* SPEAR_JPEG_H*/
