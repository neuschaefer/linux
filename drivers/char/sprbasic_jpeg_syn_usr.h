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
#ifndef SPEAR_JPEG_USER_H
#define SPEAR_JPEG_USER_H

#include <linux/types.h>
#include <asm/io.h>

/*IOCTL Defines*/
#define SPEAR_JPEG_BASE 'J'
#define	JPEGIOC_GET_HDR					_IOR(SPEAR_JPEG_BASE, 1,t_JPEGHdrInfo)
#define	JPEGIOC_START_DECODE			_IOW(SPEAR_JPEG_BASE, 2, unsigned int)
#define	JPEGIOC_SET_SRC_IMG_SIZE		_IOW(SPEAR_JPEG_BASE,3, unsigned int)
#define	JPEGIOC_GET_DECODED_DATA_SIZE	_IOR(SPEAR_JPEG_BASE, 4,unsigned int)

/*Macro for shuffling b/w the two read bufs.*/
#define BUF_SHUFFLE(Buf) (Buf = (Buf==0?1:0))

/*Struct to define Hdr Info after Decoding.*/
typedef struct
{
		unsigned int NumOfColorCmp;
		unsigned int ColSpcType;
		unsigned int NumOfCmpForScanHdr;
		unsigned int XSize;
		unsigned int YSize;
		unsigned int NumMcu;
		unsigned int NumMcuInRST;
		unsigned int H1;
		unsigned int V1;
		unsigned int H2;
		unsigned int V2;
		unsigned int H3;
		unsigned int V3;
		unsigned int H4;
		unsigned int V4;
}t_JPEGHdrInfo;


/*Define the type used to describe the possible errors of that can occurin.*/
typedef enum
{
		JPEG_OK = 0,
		JPEG_BUSY,
		JPEG_NULL_POINTER,
		JPEG_IMAGE_SIZE_ZERO,
		JPEG_SRC_IMAGE_OVERFLOW,
		JPEG_SEM_ERROR,
		JPEG_SLEEP_ERROR,
		JPEG_DECODING_NOT_OVER,
		JPEG_INVALID_IMG,
		JPEG_NO_MEM_AVAILABLE,
		JPEG_DMA_ERROR,
		JPEG_ERROR
}t_JPEGError;

#endif /* SPEAR_JPEG_USER_H*/
