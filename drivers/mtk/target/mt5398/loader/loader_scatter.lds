/*
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(INT_Vectors)
SECTIONS
{
    . = 0x00D00000;
    . = ALIGN(4);
    .text   :
    {
        gcc_int.o (.text)
        __core1_start = .;
        core1.o (.text)
        __core1_end = .;
        *(.text)
    }

    .rodata : 
    {
         *(.rodata) 
    }

    . = ALIGN(4);

    _data_in_tcm = .;

    .datacli :
    {
        __datacli_start = .;
        *(data.cli)
        . = ALIGN(4);
        __datacli_end = .;
    }
    . = ALIGN(4);
    .endcli :
    {
        *(end.cli)
    }
    . = ALIGN(4);
    .data :
    {
        _data_start = .;
        *(.data)

        . = ALIGN(4);
        _data_end = .;
    }
    . = ALIGN(4);
    .bss :
    {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }

}
