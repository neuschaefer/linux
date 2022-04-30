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
#include "sprplus_jpeg_hdr_decode.h"
#include <linux/slab.h>

static void read_two_bytes(struct image* p_img, unsigned int *value)
{
	unsigned char buf;
	buf = p_img->file_buf[p_img->location];
	*value = buf<<8;

	buf = p_img->file_buf[p_img->location+1];
	*value += buf;
}

static void read_frame_header(struct image* p_img)
{
	int i;
	unsigned char buf;

	/* reading the image precision*/
	p_img->precision = p_img->file_buf[p_img->location];
	p_img->location++;

	/*Reading the image height*/
	read_two_bytes(p_img,&p_img->height);
	p_img->location += 2;

	/*Reading the image width*/
	read_two_bytes(p_img,&p_img->width);
	p_img->location += 2;

	/*Reading the number of components*/
	p_img->number_of_components = p_img->file_buf[p_img->location];
	p_img->location++;

	p_img->blocks_in_mcu = 0;
	p_img->max_comp_mcu_width = 0;

	/*Reading the component information*/
	for(i=0;i<p_img->number_of_components;i++)
	{
		/*Reading the component identifier*/
		p_img->img_comp_info[i].comp_identifier = p_img->file_buf[p_img->location];
		p_img->location++;

		/*Reading the sampling factors*/
		buf = p_img->file_buf[p_img->location];
		p_img->location++;
		p_img->img_comp_info[i].h_samp_factor = buf  >> 4;
		p_img->img_comp_info[i].v_samp_factor = buf & 0x0F;


		p_img->img_comp_info[i].comp_mcu_width =  p_img->img_comp_info[i].h_samp_factor * p_img->img_comp_info[i].v_samp_factor;
		if(p_img->img_comp_info[i].comp_mcu_width > p_img->max_comp_mcu_width)
			p_img->max_comp_mcu_width = p_img->img_comp_info[i].comp_mcu_width;


		p_img->blocks_in_mcu += p_img->img_comp_info[i].comp_mcu_width;
		/*Reading the quantization selectors*/
		p_img->img_comp_info[i].q_dest_sel = p_img->file_buf[p_img->location];
		p_img->location++;
	}
	p_img->total_blocks = 0;
	p_img->height_in_MCU = p_img->height/(DCTSIZE * p_img->img_comp_info[0].v_samp_factor);
	p_img->width_in_MCU = p_img->width/(DCTSIZE * p_img->img_comp_info[0].h_samp_factor);

	for(i=0;i<p_img->number_of_components;i++)
	{
		p_img->img_comp_info[i].total_mcu_in_comp = ((p_img->height/DCTSIZE) * (p_img->width/DCTSIZE) * p_img->img_comp_info[i].comp_mcu_width)/p_img->max_comp_mcu_width;
		p_img->total_blocks += p_img->img_comp_info[i].total_mcu_in_comp;
	}
	p_img->total_mcu = p_img->total_blocks/p_img->blocks_in_mcu;
}

static void decode_file_header(struct image* p_img, uint FileSize)
{
	unsigned char buf;
	unsigned int header_size;

	while(p_img->location < FileSize)
	{
		buf = p_img->file_buf[p_img->location];

		p_img->location++;
		if(buf == MARKER_START)
		{
			buf = p_img->file_buf[p_img->location];
			p_img->location++;
			if((buf >= MARKER_FRAME_START_LOW && buf<=MARKER_FRAME_START_HIGH) && buf!=MARKER_HUFFMAN)
			{
				/*Reading the frame header size*/
				p_img->dct_type = buf;
				read_two_bytes(p_img,&header_size);
				/*p_img->location+=header_size;*/
				p_img->location += 2;
				read_frame_header(p_img);
				break;
			}
			if((buf == START_IMAGE) || (buf == MARKER_START))
			{
			}
			else
			{
				read_two_bytes(p_img,&header_size);
				p_img->location+=header_size;
			}
		}
	}
}

static uint FindImgSize(struct image* p_img)
{
	uint ImgSize, EveryCompSize, DivisionFactor;
	uint hFactor, vFactor;
	
	hFactor = p_img->img_comp_info[0].h_samp_factor * 8;
	vFactor = p_img->img_comp_info[0].v_samp_factor * 8;

	p_img->width = ((p_img->width + hFactor -1)/hFactor)*hFactor;
	p_img->height = ((p_img->height + vFactor -1)/vFactor)*vFactor;

	EveryCompSize = p_img->height * p_img->width;
	DivisionFactor = p_img->img_comp_info[0].h_samp_factor * p_img->img_comp_info[0].v_samp_factor;

	ImgSize = EveryCompSize + (EveryCompSize*2)/DivisionFactor;
	return ImgSize;
}

uint FindDecodedImgSize(const char *buffer, uint FileSize)
{
	struct image* p_img;
	uint ImgSize;

	p_img = (struct image*)kmalloc(sizeof(struct image), GFP_KERNEL);
	if( p_img == NULL)
	{
		printk("Decoding Image: Memory allocation failed\n");
		return 0;
	}

	p_img->file_buf = (unsigned char *)buffer;
	p_img->location = 0;
	decode_file_header(p_img, FileSize);
	ImgSize = FindImgSize(p_img);

	kfree(p_img);
	return ImgSize;
}
