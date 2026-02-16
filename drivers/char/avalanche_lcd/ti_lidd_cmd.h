/*
 *  Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
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
 */

/**************************************************************************
 * FILE PURPOSE :   IOCTL commands for LCD LIDD controller.
 **************************************************************************
 * FILE NAME    :   ti_lidd_cmd.h
 *
 * DESCRIPTION  :
 *  IOCTL commands for LCD LIDD controller.
 *
 *************************************************************************/

#ifndef _TI_LIDD_CMD_H_
#define _TI_LIDD_CMD_H_

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

#define RIGHT                  1
#define LEFT                   0
#define ON                     1
#define OFF                    0

#ifndef NULL
#define NULL    0x0
#endif

#endif /* _TI_LIDD_CMD_H_ */    
