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
 * FILE PURPOSE :   HAL header for Hitachi HD44780 controller.
 **************************************************************************
 * FILE NAME    :   hd_44780_hal.h
 *
 * DESCRIPTION  :
 *  HAL header for Hitachi HD44780 controller`.
 *
 *************************************************************************/

#ifndef _HD47780_HAL_H_
#define _HD47780_HAL_H_

/* 
 * Define void* os_platform_malloc(unsigned int), os_platform_free(void*), void os_platform_cli(unsigned int),
 * unsigned int os_platform_sti(unsigned int)
 */
#include "ti_lidd_cmd.h"

/**********************************************************************
 * Returns: NULL in case of error, otherwise a handle to be used in sub-
 * sequent calls. 
 *********************************************************************/
TI_HD47780_T* ti_hd47780_init(unsigned int* cntl_reg, 
		              unsigned int* data_reg,
			      unsigned char row,
			      unsigned char col, 
			      unsigned int cpufreq);

/**********************************************************************
 * Returns: -1 for error otherwise 0 for success. 
 *********************************************************************/
int ti_hd47780_cleanup(TI_HD47780_T*);

/**********************************************************************
 * Returns: -1 for error, other 0 for success. 
 *
 *     cmd                                  val
 *
 *     TI_LIDD_CLEAR_SCREEN                 none
 *     TI_LIDD_CURSOR_HOME                  none
 *     TI_LIDD_DISPLAY                      0 - off, 1 - on
 *     TI_LIDD_GOTO_XY                      [row - 2 bytes][col - 2 bytes]
 *     TI_LIDD_BLINK                        0 - off, 1 - on 
 *     TI_LIDD_CURSOR_STATE                 0 - not visible, 1 - visible.
 *     TI_LIDD_SHIFT                        1 - Right shift, 0 - left shift.
 *     TI_LIDD_CURSOR_SHIFT                 1 - Right, 0 - left
 *     TI_LIDD_WR_CHAR                      character.
 *     TI_LIDD_RD_CHAR                      place holder for character. 
 *     TI_LIDD_CURSOR_MOVE                  1 - Right, 0 - Left 
 *     TI_LIDD_DISPLAY_MOVE                 1 - Right, 0 - Left.
 *     TI_LIDD_LINE_WRAP                    0 - off, 1 - on.
 *
 *********************************************************************/
int ti_hd47780_ioctl(TI_HD47780_T*, unsigned int cmd, unsigned int val);

#endif /* _HD47780_HAL_H_ */
