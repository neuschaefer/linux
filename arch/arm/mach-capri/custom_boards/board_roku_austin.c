/*****************************************************************************
* Copyright 2012 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#define CAPRI_BOARD_ID CAPRI_ROKU_AUSTIN
#include "../board_template.c"

#if defined (CONFIG_CAPRI_11130)

   #ifdef CONFIG_LITTLE_MODE
   #define CAPRI_BOARD_NAME "BCM911130_ROKU_AUSTIN"
//   #define CAPRI_BOARD_NAME "BCM911130_RAY"

   #else
   #error "Unsupported mode"
   #endif

#else 

   #error "Unsupported chip"

#endif

CREATE_BOARD_INSTANCE(CAPRI_BOARD_ID,CAPRI_BOARD_NAME)
