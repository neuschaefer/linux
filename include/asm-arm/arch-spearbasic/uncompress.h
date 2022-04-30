/*
 * linux/include/asm/arch/uncompress.h
 *
 *  Copyright (C)2008  ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */


//#include<asm/arch/spearbasic.h>
#include<asm/arch/spear.h>

#define ASC_TX_BUFFER	(*(volatile unsigned short *)(SPEARBASIC_START_APB_UART0 + 0x00 ))
//#define ASC_TX_BUFFER	(*(volatile unsigned short *)(SPEAR_START_APB_UART0 + 0x00 ))
#define ASC_STATUS	(*(volatile unsigned short *)(SPEARBASIC_START_APB_UART0 + 0x18))	
//#define ASC_STATUS	(*(volatile unsigned short *)(SPEAR_START_APB_UART0 + 0x18))
/*
 * This does not append a newline
 */

#if 0

static void
putstr (const char *s)
{
    while (*s)
    {
         while (ASC_STATUS & (1 << 5))
	     barrier ();
	 ASC_TX_BUFFER = *s;
	 if (*s == '\n')
	 {
	     while (ASC_STATUS & (1 << 5))
	         barrier ();
	     ASC_TX_BUFFER = '\r';
	 }
	 s++;
    }
}
#endif



/*
 * This does not append a newline
 */
static inline void putc2(int c)
{
	 while (ASC_STATUS & (1 << 5))
             barrier ();

	 ASC_TX_BUFFER = c;
}

static inline void flush(void)
{
//        while (AMBA_UART_FR & (1 << 3))
  //              barrier();
}

/*
 * nothing to do
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()

/*End of file - uncompress.h */
