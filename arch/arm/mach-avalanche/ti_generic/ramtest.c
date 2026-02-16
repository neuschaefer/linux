/*
 *
 * ramtest.c
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
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
 *
 */


/*
 * ramtest.c: Generic RAM testing code. 
 */

#include <asm-arm/arch-avalanche/generic/pal.h>

static unsigned int testpat[] = { 
                                  0x55555555,0xaaaaaaaa,0x33333333,0xcccccccc,
                                  0x0f0f0f0f,0xf0f0f0f0,0x00ff00ff,0xff00ff00,
                                  0x0000ffff,0xffff0000,0x00000000,0xffffffff,
                                  0x00000000
                                };

/*
 * This test involves writing multiple data patterns to a
 * single RAM location and testing if the data is read back correctly.
 */
int avalanche_ram_basic_data_test(volatile unsigned int* base)
{
    volatile unsigned int* pat;
    unsigned int i, error = 0, expected, found;
              
    base = (volatile unsigned int*)((unsigned int)base & ~0x3); /* 32-bit align */

    pat = testpat;

    for(i = 0; i < (sizeof(testpat)/4); i++, pat++) 
    {
        expected = *pat;
        *base = expected;
        found = *base;
        error |= (expected ^ found);
    }

    if (error)
    {
        printk("ERROR: %s FAILURE!!!\n", __FUNCTION__);
        return AVALANCHE_RAMTEST_BASIC_DATA_FAIL;
    }
    return 0;
}

/*
 * This test involves writing a unique number to each of the addressble
 * locations and testing to see if the data read back is the same.
 * (No two addresses point to the same location)
 */
int avalanche_ram_address_test(volatile unsigned int* base, unsigned int size)
{
    unsigned int i, error = 0, expected, found;
    volatile unsigned int* ptr;
              
    base = (volatile unsigned int*)((unsigned int)base & ~0x3); /* 32-bit align */

    for(i = 0, ptr = base; i < size; i += sizeof(unsigned int), ptr++)
    {
        *ptr = i;
    }

    for(i = 0, ptr = base; i < size; i += sizeof(unsigned int), ptr++) 
    {
        found = *ptr;
        expected = i;
        error |= (expected ^ found);
    }

    if (error)
    {
        printk("ERROR: %s FAILURE!!!\n", __FUNCTION__);
        return AVALANCHE_RAMTEST_ADDRESS_FAIL;
    }
    return 0;
}

/*
 * Keeps writing all the words of the test pattern (each time in a different order)
 * to the RAM and checks to see if any discrepency is found.
 */
int avalanche_ram_data_test(volatile unsigned int* base, unsigned int size)
{
    unsigned int i, j, error = 0, expected, found, index;
    volatile unsigned int* ptr;
              
    base = (volatile unsigned int*)((unsigned int)base & ~0x3); /* 32-bit align */

    for(i = 0; (i < (sizeof(testpat)/4)) && !error; i++) 
    {
        for(j = 0, ptr = base, index = i; j < size; j += sizeof(unsigned int)) 
        {
            *ptr = testpat[index % (sizeof(testpat)/4)];
            index++;
            ptr++;                            
        }

        for(j = 0, ptr = base, index = i; j < size; j += sizeof(unsigned int)) 
        {
            found = *ptr;
            expected = testpat[index % (sizeof(testpat)/4)];
            index++;
            ptr++;
            error |= (expected ^ found);
        }
    }

    if (error)
    {
        printk("ERROR: %s FAILURE!!!\n", __FUNCTION__);
        return AVALANCHE_RAMTEST_DATA_FAIL;
    }
    return 0;
}

/*
 * The public function which performs the address and data
 * tests and retuns the result as as success or failure
 */
int avalanche_do_ram_test(volatile unsigned int* base, unsigned int size) 
{
    int ret;        

    if((ret = avalanche_ram_basic_data_test(base))) goto done;

    if((ret = avalanche_ram_address_test(base, size))) goto done;

    if((ret = avalanche_ram_data_test(base, size))) goto done;
    
    /* set the block back to zeros after the mess we created in it. */
    {
        Ptr start = (Ptr) base;
        PAL_osMemSet(start, 0, size);
    }

done:
    return ret;
}
EXPORT_SYMBOL(avalanche_do_ram_test);
