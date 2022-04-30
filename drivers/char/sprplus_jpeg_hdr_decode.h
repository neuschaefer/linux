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
#ifndef SPEAR_JPEG_HDR_DECODE_H
#define SPEAR_JPEG_HDR_DECODE_H

#include <linux/types.h>

#define MARKER_START 0xFF
#define MARKER_HUFFMAN 0xC4
#define MARKER_QUANT_TABLE 0xDB
#define MARKER_FRAME_START_LOW 0xC0
#define MARKER_FRAME_START_HIGH 0xC7
#define MARKER_SCAN_START 0XDA
#define STUFFED_BYTE 0x00
#define START_IMAGE	0xD8

#define DCTSIZE 8
#define DCTSIZE2 64

#define JPEG_SUCCESS 1
#define JPEG_FAILURE 0

#define JPEG_TRUE 1
#define JPEG_FALSE 0

#define MAX_COMPONENTS 4
#define MAX_HUFFMAN_TABLES 4
#define MAX_QUANT_TABLES 4

struct comp_info
{
	unsigned char comp_identifier;
	unsigned char h_samp_factor;
	unsigned char v_samp_factor;
	unsigned char q_dest_sel;
	unsigned char comp_mcu_width;
	unsigned long total_mcu_in_comp;
};

struct image
{
	unsigned char *file_buf;
	unsigned char dct_type;
	unsigned char precision;
	unsigned int height;
	unsigned int width;
	unsigned int height_in_MCU;
	unsigned int width_in_MCU;
	unsigned char number_of_components;
	unsigned long location;
	unsigned char blocks_in_mcu;
	unsigned long total_mcu;
	unsigned long total_blocks;
	unsigned char max_comp_mcu_width;
	struct comp_info img_comp_info[MAX_COMPONENTS];
};

typedef struct image* p_image;

uint FindDecodedImgSize(const char *buffer, uint FileSize);

#endif /*SPEAR_JPEG_HDR_DECODE_H*/

