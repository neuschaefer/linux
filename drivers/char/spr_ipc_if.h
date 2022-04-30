/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by Ashwini Pahuja  			*/
/*   ashwini.pahuja@st.com							*/
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

#ifndef SPLUS_IPC_IF_H
#define SPLUS_IPC_IF_H

#define MY_CORE_ID	ARM1

#if 0
#define MSGQD_BASE_ARM_1_2        0x2000000
#define MSGQD_ARM_2_1_OFFSET      0x30

#define MSGQ_ARM_1_2_OFFSET       0x60
#define MSGQ_ARM_2_1_OFFSET       0x320
#endif

#define IPC_MEMORY_SIZE           0x100000

#define MSGQD_BASE_ARM_1_2 	  0x2000000
#define MSGQD_ARM_2_1   	  MSGQD_BASE_ARM_1_2 + 0x30

#define MSGQ_ARM_1_2    	  MSGQD_BASE_ARM_1_2 + 0x60
#define MSGQ_ARM_2_1	       	  MSGQD_BASE_ARM_1_2 +0x320


#define MAX_PREV_MSGIRQ_TIMEOUT   10000
#define MAX_PREV_ACKIRQ_TIMEOUT   10000

#endif
