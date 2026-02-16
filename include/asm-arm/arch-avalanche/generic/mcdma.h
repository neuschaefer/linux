/*
 * mcdma.h
 * Description:
 * See below.
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
/******************************************************************************
 * FILE PURPOSE:    Multi Channel Direct Memory Access (MC-DMA) Header
 ********************************************************************************
 * FILE NAME:       mcdma.h
 *
 * DESCRIPTION:     Platform and OS independent API for MCDMA Controller
 *
 * REVISION HISTORY:
 * 27 Nov 02 - PSP TII  
 *
 *******************************************************************************/

#ifndef __MCDMA_H__
#define __MCDMA_H__


typedef enum MCDMA_CHANNEL_tag
{
    MCDMA_CHANNEL_0 = 0,
    MCDMA_CHANNEL_1,
    MCDMA_CHANNEL_2,
    MCDMA_CHANNEL_3,
    
} MCDMA_CHANNEL_T;

typedef enum MCDMA_CTRL_tag
{
    MCDMA_STOP = 0,
    MCDMA_START
    
} MCDMA_CTRL_T;

typedef enum MCDMA_ADDR_MODE_tag
{
    MCDMA_INCREMENTING = 0,
    MCDMA_FIXED = 2
    
} MCDMA_ADDR_MODE_T;

typedef enum MCDMA_BURST_MODE_tag
{
    MCDMA_1_WORD_BURST = 0,
    MCDMA_2_WORD_BURST = 1,
    MCDMA_4_WORD_BURST = 2,
    
} MCDMA_BURST_MODE_T;


void mcdma_init(UINT32 base_addr);
void mcdma_control(MCDMA_CHANNEL_T mcdma_ch, MCDMA_CTRL_T mcdma_ctrl);
INT32 mcdma_setdmaparams (MCDMA_CHANNEL_T mcdma_ch,
                          UINT32 src_addr, 
                          UINT32 dst_addr, 
                          UINT32 length, 
                          MCDMA_BURST_MODE_T burst_mode, 
                          MCDMA_ADDR_MODE_T src_addr_mode,
                          MCDMA_ADDR_MODE_T dst_addr_mode);

#endif /* __MCDMA_H__ */
