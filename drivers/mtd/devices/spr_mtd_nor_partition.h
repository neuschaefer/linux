/*
 * linux/drivers/char/mtd_partition.h
 *
 *  Copyright (C) 2006 ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
 * AUTHOR :  Ashwini Pahuja <ashwini.pahuja@st.com> */


#ifndef MTD_PARTITION_H
#define MTD_PARTITION_H


#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>


#define NUM_SF_PARTITIONS_I     5
#define NUM_SF_PARTITIONS_II    1 
#define NUM_SF_PARTITIONS_III   1 
#define NUM_SF_PARTITIONS_IV    1 
#define NUM_SF_PARTITIONS_MERGE 1


#ifndef CONFIG_MTD_STATIC_PARTITIONING
static struct mtd_partition stwsf_par_info[4][1];
#endif
/*
static struct mtd_partition stwsf_par_info_primary[] =
{
    {
	   name: "UBoot",
	   offset: 0x00000000,
	   size: 0x00050000,
	   mask_flags: MTD_WRITEABLE,  

    },

    {
	  name: "kernel",
	  offset: 0x0070000,
	  size: 0x00420000,
	  mask_flags: MTD_WRITEABLE,  
    },

    {
         name: "initrd",
         offset: 0x00470000,
         size :0x00280000,
    },

    {
        name:"JFFS2-I",
        offset:0x006f0000,
        size:0x00100000 ,
    }
};
*/

static struct mtd_partition stwsf_par_info_primary[] =
{
    {
	   name: "Xloader",
	   offset: 0x00000000,
	   size: 0x00010000,
//	   mask_flags: MTD_WRITEABLE,  
    },
    { 
	   name: "UBoot",
	   offset: 0x00010000,
	   size: 0x00040000,
//	   mask_flags: MTD_WRITEABLE,  

    },
    {
	  name: "kernel",
	  offset: 0x0050000,
	  size: 0x002c0000,
//	  mask_flags: MTD_WRITEABLE,  
    },

    {
         name: "Root File system",
         offset: 0x00310000,
         size :0x004e0000,
    },
    {
         name: "Temp",
         offset: 0x007F0000,
         size :0x00010000,
    }
};


/******** This is start of 2nd chip ****************/
static struct mtd_partition stwsf_par_info_II[] =
{
    {
        name:"JFFS2-II",
        offset:0x00000000,
        size:0x00800000 , 
    }


};
/******** This is start of 3rd chip ****************/
static struct mtd_partition stwsf_par_info_III[] =
{
    {
        name:"JFFS2-III",
        offset:0x00000000,
        size:0x00020000 , 
    }
};

/******** This is start of 4th chip ****************/
static struct mtd_partition stwsf_par_info_IV[] =
{
    {
        name:"JFFS2-IV",
        offset:0x00000000,
        size:0x00010000 , 
    }

};

static struct mtd_partition stwsf_par_info_merge[] =
{
    {
        name:"JFFS2-Merge",
        offset:0x006f0000,
        size:0x00900000 ,
    }

};

#endif

/* End of file - mtd_partition.h */
