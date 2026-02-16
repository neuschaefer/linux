/*
 * vlynq_enum.h
 * Description:
 * has all vlynq enums
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


#ifndef _VLYNQ_ENUM_H_
#define _VLYNQ_ENUM_H_

#include <mach/generic/pal.h>
#include "pal_vlynq.h"
#include "pal_vlynqIoctl.h"

typedef struct vlynq_soc_t VLYNQ_SOC_T;
typedef struct vlynq_dev_t VLYNQ_DEV_T;

#define VL_PORTAL_SIZE 0x04000000

typedef struct
{
    Uint32       offset;
    Uint32       size;
    VLYNQ_DEV_T  *dev;

} VLYNQ_REGION_CFG_T;

typedef struct
{
    Int8        hw_intr_line;
    Uint8       irq;
    VLYNQ_DEV_T *dev;

} VLYNQ_IRQ_CFG_T;

typedef struct vlynq_module_t
{
    Uint32                phy_addr;
    VLYNQ_REGION_CFG_T    *region;
    VLYNQ_SOC_T           *soc;

} VLYNQ_MODULE_T;

struct vlynq_dev_t
{
    char               *name;
    Bool               import;
//    Uint32             ref_cnt;
    PAL_VLYNQ_DEV_HND  *vlynq_dev_hnd;

};

struct vlynq_soc_t
{
    char               *name;
    char               *class_name;
    Uint32             soc_id;
    VLYNQ_MODULE_T     *lo_vlynq;
    VLYNQ_MODULE_T     *hi_vlynq;
    VLYNQ_DEV_T        *dev_list;
    VLYNQ_REGION_CFG_T *region_list;
    VLYNQ_IRQ_CFG_T    *irq_list;
    Uint32             endian_map;
    
};

typedef VLYNQ_SOC_T* VLYNQ_SOC_LIST_T;

#define import_true    True
#define import_false   False

#define LITTLE_EN      0x1
#define BIG_EN         0x2


#define WLAN_CLASS_NAME     "wlan"
#define VDSP_CLASS_NAME     "vdsp"
#define HOST_CLASS_NAME     "host"

/*-----------------------------------------------------------------------------
 * 1130 specifications.
 *---------------------------------------------------------------------------*/
#define VLYNQ_ACX111_MEM_OFFSET 0xC0000000
#define VLYNQ_ACX111_MEM_SIZE   0x00040000
#define VLYNQ_ACX111_REG_OFFSET 0xF0000000
#define VLYNQ_ACX111_REG_SIZE   0x01000000

/* It is assumed that 1130 has just one VLYNQ device, even though the docs say
 * otherwise. At least, this is what I have been communicated. */
static VLYNQ_DEV_T 
wlan1130_dev [] = { 
	              {WLAN_CLASS_NAME, import_true,  NULL},
                      {"lo-1130vlynq",  import_true,  NULL},
                      {NULL,            import_false, NULL}
	          }; 

static VLYNQ_REGION_CFG_T 
wlan1130_region[] = {
                        { VLYNQ_ACX111_MEM_OFFSET, VLYNQ_ACX111_MEM_SIZE, &wlan1130_dev[0]},
                        { VLYNQ_ACX111_REG_OFFSET, VLYNQ_ACX111_REG_SIZE, &wlan1130_dev[0]},
			{ 0x00000000,              VL_PORTAL_SIZE,        &wlan1130_dev[1]},
                        { 0xFFFFFFFF,              0,                     NULL            }
                    };

static VLYNQ_IRQ_CFG_T 
wlan1130_irq[] = {
                     { 0, 0, &wlan1130_dev[0]},
                     {-1, 0, NULL}
                 };

VLYNQ_MODULE_T 
wlan1130_vlynqs[] = {
	                 {0xFFF00A00, &wlan1130_region[2], NULL}
/*	                 {0xFFF00B00, &wlan1130_region[2], NULL}   This is not correct. */
                    };

VLYNQ_SOC_T 
wlan1130_soc [] = { 
                    { "wlan1130", WLAN_CLASS_NAME, 0x0009, 
	              &wlan1130_vlynqs[0], NULL, &wlan1130_dev[0],  /* 1130 has just one VLYNQ working. */
		      &wlan1130_region[0], &wlan1130_irq[0], LITTLE_EN | BIG_EN 
                    }
                  };

/*-----------------------------------------------------------------------------
 * 1350 specifications.
 *---------------------------------------------------------------------------*/
#define VLYNQ_1x50_MEM_OFFSET 0x00000000
#define VLYNQ_1x50_MEM_SIZE   0x00080000
#define VLYNQ_1x50_REG_OFFSET 0x00300000
#define VLYNQ_1x50_REG_SIZE   0x00100000

static VLYNQ_DEV_T
wlan1350_dev [] = {
                      {WLAN_CLASS_NAME,  import_true,  NULL},
		      {"lo-1350vlynq",   import_true,  NULL},
		      {"hi-1350vlynq",   import_true,  NULL},
		      {NULL,             import_false, NULL}
                  };

static VLYNQ_REGION_CFG_T
wlan1350_region [] = {
                         { VLYNQ_1x50_MEM_OFFSET, VLYNQ_1x50_MEM_SIZE, &wlan1350_dev[0] },
                         { VLYNQ_1x50_REG_OFFSET, VLYNQ_1x50_REG_SIZE, &wlan1350_dev[0] },
                         { 0x40000000,            VL_PORTAL_SIZE,      &wlan1350_dev[2] },
                         { 0x60000000,            VL_PORTAL_SIZE,      &wlan1350_dev[1] },
                         { 0xFFFFFFFF,            0,                   NULL             }
                     };

static VLYNQ_IRQ_CFG_T
wlan1350_irq [] = {
                      { 0, 0, &wlan1350_dev[0]},
		      {-1, 0, NULL}
                  };

static VLYNQ_MODULE_T
wlan1350_vlynq []  = {
                         {0x00308000, &wlan1350_region[3], NULL},
		         {0x00308800, &wlan1350_region[2], NULL}
                     };

VLYNQ_SOC_T 
wlan1350_soc  = {"wlan1350", WLAN_CLASS_NAME, 0x0029, 
	         &wlan1350_vlynq[0],  &wlan1350_vlynq[1], &wlan1350_dev[0],    
		 &wlan1350_region[0], &wlan1350_irq[0], LITTLE_EN};

/*-----------------------------------------------------------------------------
 * VDSP 921 specifications.
 *---------------------------------------------------------------------------*/
static VLYNQ_DEV_T
v921_dev [] = {
                  {"vdsp",          import_true,  NULL},
	          {"lo-v921vlynq",  import_true,  NULL},
	          {"hi-v921vlynq",  import_true,  NULL},
	          {NULL,            import_false, NULL}
              };

static VLYNQ_REGION_CFG_T 
v921_region [] = {
                     { 0x00000000, 0x02081500,              &v921_dev[0] },
                     { 0x04000000, VL_PORTAL_SIZE,          &v921_dev[1] },
		     { 0x08000000, VL_PORTAL_SIZE,          &v921_dev[2] },
		     { 0xFFFFFFFF, 0,                       NULL         }
                 };

static VLYNQ_IRQ_CFG_T
v921_irq [] = {
	          //            { 0, 8, &v921_dev[0]},
			      {-1, 0, NULL}
	      };

static VLYNQ_MODULE_T
v921_vlynq [] = {
                    { 0x02081200, &v921_region[1], NULL},
	            { 0x02081300, &v921_region[2], NULL}
                };

VLYNQ_SOC_T 
v921_soc  = {"vdsp-921", VDSP_CLASS_NAME, 0x0006, 
	     &v921_vlynq[0],  &v921_vlynq[1], &v921_dev[0],      
	     &v921_region[0], &v921_irq[0], LITTLE_EN | BIG_EN};

/*-----------------------------------------------------------------------------
 * Ohio specifications.
 *---------------------------------------------------------------------------*/
static VLYNQ_DEV_T
ohio_dev [] = {
                   {"ohio",          import_false, NULL},
                   {"lo-7200vlynq",  import_true,  NULL},
                   {NULL,            import_false, NULL}
              };

static VLYNQ_REGION_CFG_T
ohio_region [] = {
                     { AVALANCHE_SDRAM_BASE, VL_PORTAL_SIZE, &ohio_dev[0]},
                     { 0x04000000,           VL_PORTAL_SIZE, &ohio_dev[1]},
		     { 0xFFFFFFFF,           0,              NULL        }
                 };

static VLYNQ_MODULE_T
ohio_vlynq [] = {
                    { 0x08611800, &ohio_region[1], NULL},
	        };

VLYNQ_SOC_T
ohio_soc  = {"ohio-7200", HOST_CLASS_NAME, 0x0000002b, 
	     &ohio_vlynq[0],  NULL, &ohio_dev[0],
	     &ohio_region[0], NULL, LITTLE_EN | BIG_EN};

/*-----------------------------------------------------------------------------
 * Yamuna specifications.
 *---------------------------------------------------------------------------*/
static VLYNQ_DEV_T
yamuna_dev [] = {
                   {"yamuna",        import_false, NULL},
                   {"lo-8400vlynq",  import_true,  NULL},
                   {NULL,            import_false, NULL}
              };

static VLYNQ_REGION_CFG_T
yamuna_region [] = {
                     { AVALANCHE_SDRAM_BASE, VL_PORTAL_SIZE, &yamuna_dev[0]},
                     { 0x0C000000,           VL_PORTAL_SIZE, &yamuna_dev[1]},
		     { 0xFFFFFFFF,           0,              NULL        }
                 };

static VLYNQ_MODULE_T
yamuna_vlynq [] = {
                    { 0x08611800, &yamuna_region[1], NULL},
	        };

VLYNQ_SOC_T
yamuna_soc  = {"yamuna-8400", HOST_CLASS_NAME, 48, 
	     &yamuna_vlynq[0],  NULL, &yamuna_dev[0],
	     &yamuna_region[0], NULL, LITTLE_EN | BIG_EN};

/*-----------------------------------------------------------------------------
 * Puma5 specifications.
 *---------------------------------------------------------------------------*/
static VLYNQ_DEV_T
puma5_dev [] = {
                   {"puma5",        import_false, NULL},
                   {"lo-puma5vlynq",  import_true,  NULL},
                   {NULL,            import_false, NULL}
              };

static VLYNQ_REGION_CFG_T
puma5_region [] = {
                     { AVALANCHE_SDRAM_BASE, VL_PORTAL_SIZE, &puma5_dev[0]},
                     { 0x0C000000,           VL_PORTAL_SIZE, &puma5_dev[1]},
		     { 0xFFFFFFFF,           0,              NULL        }
                 };

static VLYNQ_MODULE_T
puma5_vlynq [] = {
                    { 0x08611800, &puma5_region[1], NULL},
	        };

static VLYNQ_SOC_T
puma5_soc  = {"puma5-4830", HOST_CLASS_NAME, 0xB7C7, 
	     &puma5_vlynq[0],  NULL, &puma5_dev[0],
	     &puma5_region[0], NULL, BIG_EN};

/*--------------------------------------*/
static VLYNQ_DEV_T
puma5_dev2 [] = {
                   {"puma5_2",          import_true,    NULL},
                   {"lo-puma5vlynq_2",  import_false,   NULL},
                   {NULL,               import_false,   NULL}
              };

/* The structure below represents the peer puma5 regions to be mapped. In the
 * default configuration set up below, the 1MB SDRAM region starting from
 * 0x80000100 and 2MB region starting from 0x03000000 are mapped in the VLYNQ
 * window. Thus the resulting default VLYNQ window for peer Puma5 is:
 * 
 * Offset 0 to 1MB-1    ---> 0x80000100 to 0x801000FF
 * Offset 1MB to 3MB-1  ---> 0x03000000 to 0x031FFFFF
 * 
 * Note that all the regions mentioned below are associated with device 0 entry.
 * While the device 1 region is mapped for Tx.
 * 
 * These mappings can be further appended or modified as per system
 * configuration.
 */
static VLYNQ_REGION_CFG_T
puma5_region2 [] = {
                     { (AVALANCHE_SDRAM_BASE+0x100),    0x100000,       &puma5_dev2[0]},
                     { (0x03000000),                    0x200000,       &puma5_dev2[0]},
                     { 0x0C000000,                      VL_PORTAL_SIZE, &puma5_dev2[1]},
		     { 0xFFFFFFFF,                      0,              NULL        }
                 };

static VLYNQ_MODULE_T
puma5_vlynq2 [] = {
                    { 0x08611800, &puma5_region2[2], NULL},
	        };

/* TODO : Add IRQ configurations */

static VLYNQ_SOC_T
puma5_soc2  = {"puma5-4830_2", HOST_CLASS_NAME, 0xB7C7, 
	     &puma5_vlynq2[0],  NULL, &puma5_dev2[0],
	     &puma5_region2[0], NULL, BIG_EN};
/*-----------------------------------------------------------------------------
 * List of the SoC(s).
 *---------------------------------------------------------------------------*/
static VLYNQ_SOC_LIST_T
soc_list [ ] =  {
//	           &ohio_soc, 
//		   &v921_soc, 
//		   &wlan1350_soc, 
//	           &yamuna_soc, 
                   &puma5_soc,
                   &puma5_soc2,
		   NULL
		};



/*
 * Trick to generate data for multiple instances of the same SOC which may found 
 * in the chain. This is to be done for each of the SOC.
 *
 * #define CREATE_MY_DEV(my_dev)                                             \
 * static VLYNQ_DEV_T                                                        \ 
 * my_dev [ ] = {                                                            \
 *                  {"name1",     import_false,        NULL},                \
 *                  {"name2",     import_true,         NULL},                \
 *                  {"name3",     import_false,        NULL}                 \
 *              } 
 *
 * #define CREATE_MY_REGION(my_region, my_dev)                               \
 * static VLYNQ_REGION_CFG_T                                                 \
 * my_region[ ] = {                                                          \
 *                    {BASE1, SIZE1, &my_dev[0]},                            \
 *                    {BASE2, SIZE2, &my_dev[1]},                            \
 *                    {BASE3, SIZE3, &my_dev[2]}                             \
 *                }                   
 *                 
 * #define CREATE_MY_IRQ(my_irq, my_dev)                                     \
 * static VLYNQ_IRQ_CFG_T                                                    \
 * my_irq [] = {                                                             \
 *                 { 0, 8, &my_dev[0]},                                      \
 *                 {-1, 0, NULL}                                             \
 *             }
 *		
 * #define CREATE_MY_VLYNQ(my_vlynq, my_region)                              \
 * static VLYNQ_MODULE_T                                                     \
 * my_vlynq[ ] = {                                                           \
 *                   {VL_CNT_BASE1, &my_region[0], NULL},                    \
 *                   {VL_CNT_BASE2, &my_region[1], NULL}                     \
 *               }
 *
 * #define CREATE_MY_SOC(my_soc, my_dev, my_region, my_irq, my_vlynq)        \
 * CREATE_MY_DEV(my_dev);                                                    \
 * CREATE_MY_REGION(my_region, my_dev);                                      \
 * CREATE_MY_IRQ(my_irq, my_dev);                                            \
 * CREATE_MY_VLYNQ(my_vlynq, my_dev);                                        \
 *                                                                           \
 * static VLYNQ_SOC_T                                                        \
 * my_soc[ ] = {                                                             \
 *                   {"my_soc", CLASS_NAME, 0xdeadbeef, &my_vlynq[0],        \
 *                    &my_vlynq[1], &my_dev[0], &my_region[0],               \
 *                    &my_irq[0], LITTLE_EN | BIG_EN } 
 *                  
 * CREATE_MY_SOC(soc1, dev1, region1, irq1, vlynq1);         
 * CREATE_MY_SOC(soc2, dev2, region2, irq2, vlynq2); 
 *
 * soc_list [ ] = { 
 *                    &soc1,
 *                    &soc2,
 *                    NULL
 *                };
 */
#endif
