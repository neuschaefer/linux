/*
 * lcd.h
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
/*******************************************************************************   
 * FILE PURPOSE:    LCD Module Driver Header file                                     
 *******************************************************************************   
 * FILE NAME:       lcd.h                                                   
 *                                                                                 
 * DESCRIPTION:     Header file for Linux LCD Driver                             
 *                                                                                 
 * REVISION HISTORY:  
 *   
 * Date           Description                               Author
 *-----------------------------------------------------------------------------
 * 27 Aug 2003    Initial Creation                          Sharath Kumar  
 * 
 * 16 Dec 2003    Updates for 5.7                           Sharath Kumar                                                          
 *                                                                                 
 ******************************************************************************/   
#ifndef _TI_LCD_H_
#define _TI_LCD_H_

typedef struct lcd_pos {
	int row;
	int column;
} LCD_POS;

typedef struct lcd_pulse_arg {
	unsigned char is_up;
	unsigned char cnt;
} LCD_PULSE_ARG;

/* LCD configuration params  */
#define    MAX_ROWS                          (4)  
#define    MAX_COLS                         (40)  
#define    DEFAULT_ROWS                      (2)  
#define    DEFAULT_COLS                     (24)  
#define    NO_LCD_DEVICES                    (1)  

/* Defines for IOCTLs */
#define TI_LIDD_CLEAR_SCREEN   1               
#define TI_LIDD_CURSOR_HOME    2               
#define TI_LIDD_GOTO_XY        3               
#define TI_LIDD_DISPLAY        4               
#define TI_LIDD_BLINK          5               
#define TI_LIDD_CURSOR_STATE   6               
#define TI_LIDD_DISPLAY_SHIFT  7               
#define TI_LIDD_CURSOR_SHIFT   8               
#define TI_LIDD_CURSOR_MOVE    9               
#define TI_LIDD_DISPLAY_MOVE   10              
#define TI_LIDD_WR_CHAR        11              
#define TI_LIDD_RD_CHAR        12              
#define TI_LIDD_LINE_WRAP      13              
#define TI_LIDD_RD_CMD         14              
#define TI_LIDD_PULSE_CMD      15
                                               
#define RIGHT                  1               
#define LEFT                   0               
#define ON                     1               
#define OFF                    0               

#endif /* _TI_LCD_H_ */
