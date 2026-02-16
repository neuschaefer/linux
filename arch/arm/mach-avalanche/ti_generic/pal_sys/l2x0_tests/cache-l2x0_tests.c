/*
 *
 * l2x0_cache_tests.c
 * Description:
 * Datapipe driver implementation
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */


/************************************************/
/*                    Includes                  */
/************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include "pal_cppi41.h"
#include "puma7.h"

/************************************************/
/*                    Defines                   */
/************************************************/

#define DRV_NAME                    "L2X0 Cache Tests"
#define DRV_VERSION                 "1.0.0.0"
/* uncomment to enable debug prints */
#define L2X0_TESTS_DBG

#ifdef L2X0_TESTS_DBG
/* Debug print, also print function name and line number */
#  define DPRINTK(fmt, args...) printk("%s(%d): " fmt "\n", __FUNCTION__ , __LINE__, ## args)
#else
#  define DPRINTK(fmt, args...)
#endif
/* Error print, also print function name and line number */
#define EPRINTK(fmt, args...) printk(KERN_ERR "\n\n****** %s(%d): " fmt " ******\n\n", __FUNCTION__ , __LINE__, ## args)


#define DECLARE_WRITE_PROC(proc_name)                                               \
    struct proc_dir_entry * l2x0_test_ ## proc_name ## _proc_entry = NULL;          \
    static const struct file_operations l2x0_test_ ## proc_name ## _proc_ops =      \
    { .write = l2x0_test_ ## proc_name ## _proc };

#define DECLARE_READ_PROC(proc_name)                                                \
    struct proc_dir_entry * l2x0_test_ ## proc_name ## _proc_entry = NULL;          \
    static const struct file_operations l2x0_test_ ## proc_name ## _proc_ops =      \
    { .read = l2x0_test_ ## proc_name ## _proc };

#define CREATE_PROC(dir, proc_name)                                                             \
    if (NULL == (l2x0_test_ ## proc_name ## _proc_entry =                                       \
                 proc_create("" #proc_name, 0, dir, &l2x0_test_ ## proc_name ## _proc_ops )))   \
    { EPRINTK("Failed to create %s proc....\n", "" #proc_name); }

#define DECLARE_WRITE_PROC_FUNCTION(proc_name, file_v, user_v, count_v, loff_t_v)            \
    ssize_t l2x0_test_ ## proc_name ## _proc(struct file *file_v, const char __user *user_v, \
                                             size_t count_v, loff_t *loff_t_v)

#define DECLARE_READ_PROC_FUNCTION(proc_name, file_v, user_v, count_v, loff_t_v)             \
    ssize_t l2x0_test_ ## proc_name ## _proc(struct file *file_v, char __user *user_v, \
                                             size_t count_v, loff_t *loff_t_v)

#define REMOVE_PROC(proc_name)                      \
    proc_remove(l2x0_test_ ## proc_name ## _proc_entry)


/************************************************/
/*                 Structures                   */
/************************************************/


/************************************************/
/*                  Functions                   */
/************************************************/
static int  __init l2x0_tests_init_module       (void);
static void __exit l2x0_tests_cleanup_module    (void);

DECLARE_WRITE_PROC_FUNCTION(read_ddr,           file, buffer, count, f_pos);
DECLARE_WRITE_PROC_FUNCTION(write_ddr,          file, buffer, count, f_pos);
DECLARE_WRITE_PROC_FUNCTION(read_reg,           file, buffer, count, f_pos);
DECLARE_WRITE_PROC_FUNCTION(read_regs,          file, buffer, count, f_pos);
DECLARE_WRITE_PROC_FUNCTION(write_reg,          file, buffer, count, f_pos);
DECLARE_WRITE_PROC_FUNCTION(cache_write_back,   file, buffer, count, f_pos);
DECLARE_WRITE_PROC_FUNCTION(cache_invalidate,   file, buffer, count, f_pos);
DECLARE_READ_PROC_FUNCTION (get_ddr_addr,       file, buffer, count, f_pos);
DECLARE_READ_PROC_FUNCTION (performance,        file, buffer, count, f_pos);

/************************************************/
/*         Global and Static Variables          */
/************************************************/

struct proc_dir_entry * l2x0_test_proc_dir = NULL;

DECLARE_WRITE_PROC(read_ddr);
DECLARE_WRITE_PROC(write_ddr);
DECLARE_WRITE_PROC(read_reg);
DECLARE_WRITE_PROC(read_regs);
DECLARE_WRITE_PROC(write_reg);
DECLARE_WRITE_PROC(cache_write_back);
DECLARE_WRITE_PROC(cache_invalidate);
DECLARE_READ_PROC (get_ddr_addr);
DECLARE_READ_PROC (performance);


/*****************************************************************************************/
/*****************************************************************************************/
/**  ___  ___            _         _                                                    **/
/**  |  \/  |           | |       | |                                                   **/
/**  | .  . |  ___    __| | _   _ | |  ___                                              **/
/**  | |\/| | / _ \  / _` || | | || | / _ \                                             **/
/**  | |  | || (_) || (_| || |_| || ||  __/                                             **/
/**  \_|  |_/ \___/  \__,_| \__,_||_| \___|                                             **/
/**  ______                    _    _                                                   **/
/**  |  ___|                  | |  (_)                                                  **/
/**  | |_  _   _  _ __    ___ | |_  _   ___   _ __   ___                                **/
/**  |  _|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|                               **/
/**  | |  | |_| || | | || (__ | |_ | || (_) || | | |\__ \                               **/
/**  \_|   \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/                               **/
/*****************************************************************************************/

static int __init l2x0_tests_init_module (void)
{
    if (NULL == (l2x0_test_proc_dir = proc_mkdir("l2x0_tests", NULL))) 
    { 
        EPRINTK("Failed to create l2x0 tests procs folder....\n");
    }
    CREATE_PROC(l2x0_test_proc_dir, read_ddr);
    CREATE_PROC(l2x0_test_proc_dir, write_ddr);
    CREATE_PROC(l2x0_test_proc_dir, read_reg);
    CREATE_PROC(l2x0_test_proc_dir, read_regs);
    CREATE_PROC(l2x0_test_proc_dir, write_reg);
    CREATE_PROC(l2x0_test_proc_dir, cache_write_back);
    CREATE_PROC(l2x0_test_proc_dir, cache_invalidate);
    CREATE_PROC(l2x0_test_proc_dir, get_ddr_addr);
    CREATE_PROC(l2x0_test_proc_dir, performance);

    return 0;
}

static void __exit l2x0_tests_cleanup_module (void)
{
    REMOVE_PROC(read_ddr);        
    REMOVE_PROC(write_ddr);       
    REMOVE_PROC(read_reg);        
    REMOVE_PROC(read_regs);        
    REMOVE_PROC(write_reg);       
    REMOVE_PROC(cache_write_back);
    REMOVE_PROC(cache_invalidate);
    REMOVE_PROC(get_ddr_addr);    
    REMOVE_PROC(performance);    
    proc_remove(l2x0_test_proc_dir);
}

/*****************************************************************************************/
/**   _______          _                                                                **/
/**  |__   __|        | |                                                               **/
/**     | |  ___  ___ | |_  ___                                                         **/
/**     | | / _ \/ __|| __|/ __|                                                        **/
/**     | ||  __/\__ \| |_ \__ \                                                        **/
/**     |_| \___||___/ \__||___/                                                        **/
/**   ______                    _    _                                                  **/
/**  |  ____|                  | |  (_)                                                 **/
/**  | |__  _   _  _ __    ___ | |_  _   ___   _ __   ___                               **/
/**  |  __|| | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|                              **/
/**  | |   | |_| || | | || (__ | |_ | || (_) || | | |\__ \                              **/
/**  |_|    \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/                              **/
/**                                                                                     **/
/*****************************************************************************************/

unsigned int read_ddr_address32(unsigned int ddr_phy_addr)
{
    unsigned int *ddr_virt_addr;

    ddr_virt_addr = PAL_CPPI4_PHYS_2_VIRT(ddr_phy_addr);

    return *ddr_virt_addr;
}

void write_ddr_address32(unsigned int ddr_phy_addr, int value)
{
    unsigned int *ddr_virt_addr;

    ddr_virt_addr = PAL_CPPI4_PHYS_2_VIRT(ddr_phy_addr);

    *ddr_virt_addr = value;
}

unsigned int read_register32(unsigned int reg_phy_addr)
{
    unsigned int *reg_virt_addr;

    reg_virt_addr = (unsigned int *) IO_PHY2VIRT(reg_phy_addr);

    return *reg_virt_addr;
}

void write_register32(unsigned int reg_phy_addr, int value)
{
    unsigned int *reg_virt_addr;

    reg_virt_addr = (unsigned int *) IO_PHY2VIRT(reg_phy_addr);

    *reg_virt_addr = value;
}

DECLARE_WRITE_PROC_FUNCTION(write_ddr, file, buffer, count, f_pos)
{
    unsigned int ddr_addr;
             int value;

    char *local_buffer;
    char *ddr_addr_str, *value_str;

    local_buffer = kmalloc(count + 1, GFP_KERNEL);
    if (NULL == local_buffer)
    {
        EPRINTK("Failed to alloc memory");
        return -EFAULT;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        EPRINTK("Failed to read from user space");
        return -EFAULT;
    }

    local_buffer[count] = 0;
    DPRINTK("local_buffer = %s", local_buffer);

    ddr_addr_str = strsep(&local_buffer, " \t");
    value_str    = strsep(&local_buffer, " \t");
    DPRINTK("ddr_addr_str = %s, value_str = %s", ddr_addr_str, value_str);

    kstrtou32(ddr_addr_str, 0, &ddr_addr);
    kstrtou32(value_str   , 0, &value);
    DPRINTK("ddr_addr = 0x%08x, value = 0x%08x", ddr_addr, value);
    kfree(local_buffer);

    // TBD - check if DDR address is valid
    if (0)
    {
        EPRINTK("Address 0x%08x isn't a valid DDR address", ddr_addr);
        return count;
    }

    printk("0x%08x <== 0x%08x\n", ddr_addr, value);
    write_ddr_address32(ddr_addr, value);
    printk("0x%08x ==> 0x%08x\n", ddr_addr, read_ddr_address32(ddr_addr));

    return count;
}

DECLARE_WRITE_PROC_FUNCTION(read_ddr, file, buffer, count, f_pos)
{
    char *local_buffer;
    unsigned int ddr_addr;

    local_buffer = kmalloc(count + 1, GFP_KERNEL);
    if (NULL == local_buffer)
    {
        EPRINTK("Failed to alloc memory");
        return -EFAULT;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        EPRINTK("Failed to read from user space");
        return -EFAULT;
    }

    local_buffer[count] = 0;
    DPRINTK("local_buffer = %s", local_buffer);

    kstrtou32(local_buffer, 0, &ddr_addr);
    DPRINTK("ddr_addr = 0x%08x", ddr_addr);

    kfree(local_buffer);

    // TBD - check if DDR address is valid
    if (0)
    {
        EPRINTK("Address 0x%08x isn't a valid DDR address", ddr_addr);
        return count;
    }
    
    printk("0x%08x ==> 0x%08x\n", ddr_addr, read_ddr_address32(ddr_addr));

    return count;
}

DECLARE_WRITE_PROC_FUNCTION(write_reg, file, buffer, count, f_pos)
{
    unsigned int reg_addr;
             int value;

    char *local_buffer;
    char *reg_addr_str, *value_str;

    local_buffer = kmalloc(count + 1, GFP_KERNEL);
    if (NULL == local_buffer)
    {
        EPRINTK("Failed to alloc memory");
        return -EFAULT;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        EPRINTK("Failed to read from user space");
        return -EFAULT;
    }

    local_buffer[count] = 0;
    DPRINTK("local_buffer = %s", local_buffer);

    reg_addr_str = strsep((char **)&local_buffer, " \t");
    value_str    = strsep((char **)&local_buffer, " \t");
    DPRINTK("ddr_addr_str = %s, value_str = %s", reg_addr_str, value_str);

    kstrtou32(reg_addr_str, 0, &reg_addr);
    kstrtou32(value_str, 0, &value);
    DPRINTK("reg_addr = 0x%08x, value = 0x%08x", reg_addr, value);
    kfree(local_buffer);

    // TBD - check if register address is valid
    if (0)
    {
        EPRINTK("Address 0x%08x isn't a valid register address", reg_addr);
        return count;
    }

    printk("0x%08x <== 0x%08x\n", reg_addr, value);
    write_register32(reg_addr, value);
    printk("0x%08x ==> 0x%08x\n", reg_addr, read_register32(reg_addr));

    return count;
}

DECLARE_WRITE_PROC_FUNCTION(read_reg, file, buffer, count, f_pos)
{
    char *local_buffer;
    unsigned int reg_addr;

    local_buffer = kmalloc(count + 1, GFP_KERNEL);
    if (NULL == local_buffer)
    {
        EPRINTK("Failed to alloc memory");
        return -EFAULT;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        EPRINTK("Failed to read from user space");
        return -EFAULT;
    }

    local_buffer[count] = 0;
    DPRINTK("local_buffer = %s", local_buffer);

    kstrtou32(local_buffer, 0, &reg_addr);
    DPRINTK("reg_addr = 0x%08x", reg_addr);
    kfree(local_buffer);

    // TBD - check if register address is valid
    if (0)
    {
        EPRINTK("Address 0x%08x isn't a valid register address", reg_addr);
        return count;
    }

    printk("0x%08x ==> 0x%08x\n", reg_addr, read_register32(reg_addr));

    return count;
}

DECLARE_WRITE_PROC_FUNCTION(read_regs, file, buffer, count, f_pos)
{
    unsigned int reg__start_addr;
             int length;
    unsigned int i;

    char *local_buffer;
    char *reg_addr_str, *size_str;

    local_buffer = kmalloc(count + 1, GFP_KERNEL);
    if (NULL == local_buffer)
    {
        EPRINTK("Failed to alloc memory");
        return -EFAULT;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        EPRINTK("Failed to read from user space");
        return -EFAULT;
    }

    local_buffer[count] = 0;
    DPRINTK("local_buffer = %s", local_buffer);

    reg_addr_str = strsep((char **)&local_buffer, " \t");
    size_str    = strsep((char **)&local_buffer, " \t");
    DPRINTK("ddr_addr_str = %s, value_str = %s", reg_addr_str, size_str);

    kstrtou32(reg_addr_str, 0, &reg__start_addr);
    kstrtou32(size_str, 0, &length);
    DPRINTK("reg_addr = 0x%08x, length = %d", reg__start_addr, length);
    kfree(local_buffer);

    // TBD - check if register address is valid
    if (0)
    {
        EPRINTK("Address 0x%08x isn't a valid register address", reg__start_addr);
        return count;
    }

    for (i=0; i < length; i+=1)
    {
        printk("0x%08x ==> 0x%08x\n", reg__start_addr + i, read_register32(reg__start_addr + i));
    }
    printk("\n");

    return count;
}

DECLARE_WRITE_PROC_FUNCTION(cache_write_back, file, buffer, count, f_pos)
{
    unsigned int ddr_addr;
    unsigned int length;

    char *local_buffer;
    char *ddr_addr_str, *length_str;

    local_buffer = kmalloc(count + 1, GFP_KERNEL);
    if (NULL == local_buffer)
    {
        EPRINTK("Failed to alloc memory");
        return -EFAULT;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        EPRINTK("Failed to read from user space");
        return -EFAULT;
    }

    local_buffer[count] = 0;
    DPRINTK("local_buffer = %s", local_buffer);

    ddr_addr_str = strsep((char **)&local_buffer, " \t");
    length_str   = strsep((char **)&local_buffer, " \t");
    DPRINTK("ddr_addr_str = %s, value_str = %s", ddr_addr_str, length_str);

    kstrtou32(ddr_addr_str, 0, &ddr_addr);
    kstrtou32(length_str  , 0, &length);
    DPRINTK("ddr_addr = 0x%08x, length = %d", ddr_addr, length);
    kfree(local_buffer);

    // TBD - check if register address is valid
    if (0)
    {
        EPRINTK("Address 0x%08x isn't a valid register address", ddr_addr);
        return count;
    }

    PAL_CPPI4_CACHE_WRITEBACK(PAL_CPPI4_PHYS_2_VIRT(ddr_addr), length);
    
    return count;
}

DECLARE_WRITE_PROC_FUNCTION(cache_invalidate, file, buffer, count, f_pos)
{
    unsigned int ddr_addr;
    unsigned int length;

    char *local_buffer;
    char *ddr_addr_str, *length_str;

    local_buffer = kmalloc(count + 1, GFP_KERNEL);
    if (NULL == local_buffer)
    {
        EPRINTK("Failed to alloc memory");
        return -EFAULT;
    }

    if (copy_from_user(local_buffer, buffer, count))
    {
        EPRINTK("Failed to read from user space");
        return -EFAULT;
    }

    local_buffer[count] = 0;
    DPRINTK("local_buffer = %s", local_buffer);

    ddr_addr_str = strsep((char **)&local_buffer, " \t");
    length_str   = strsep((char **)&local_buffer, " \t");
    DPRINTK("ddr_addr_str = %s, value_str = %s", ddr_addr_str, length_str);

    kstrtou32(ddr_addr_str, 0, &ddr_addr);
    kstrtou32(length_str  , 0, &length);
    DPRINTK("ddr_addr = 0x%08x, length = %d", ddr_addr, length);
    kfree(local_buffer);

    // TBD - check if register address is valid
    if (0)
    {
        EPRINTK("Address 0x%08x isn't a valid register address", ddr_addr);
        return count;
    }

    PAL_CPPI4_CACHE_INVALIDATE(PAL_CPPI4_PHYS_2_VIRT(ddr_addr), length);
    
    return count;
}

DECLARE_READ_PROC_FUNCTION(get_ddr_addr, file, buffer, count, f_pos)
{
    unsigned int *ddr_addr;
    char buf[32];
    int len;

    if (*f_pos) { return 0; }

    ddr_addr = kmalloc(sizeof(*ddr_addr), GFP_KERNEL);
    
    len = sprintf(buf, "ddr_addr = 0x%08x\n\n", PAL_CPPI4_VIRT_2_PHYS(ddr_addr));

    copy_to_user(buffer, buf, len);

    //kfree(ddr_addr);
    *f_pos += len;

    return len;
}

DECLARE_READ_PROC_FUNCTION (performance, file, buffer, count, f_pos)
{
#define L2CACHE_SIZE_IN_BYTES   0x40000
#define NUM_OF_ITERATIONS       10
    unsigned int *ddr_buffer;
    unsigned int ddr_buf_len, temp;
    unsigned int k,m,n, len = 0;
    unsigned int val_to_write = 0x55555555;
    unsigned long long start, end;
    unsigned int iterationsRunTime[NUM_OF_ITERATIONS] = { 0 }, totalTime = 0;
    char buf[960];

    if (*f_pos) { return 0; }

    /* 
       the test uses a buffer that is slightly largher than the size of the l2cache memory
       in order to create a test that will cause 95% of l2cache hits and 5% of misses
    */
    ddr_buf_len = (L2CACHE_SIZE_IN_BYTES);    /* setting buffer length to l2cache memory size */
    ddr_buf_len += ddr_buf_len / 20;          /* increase the buffer length in 5% */

    ddr_buffer = kmalloc(ddr_buf_len, GFP_KERNEL);
    if (NULL == ddr_buffer)
    {
        len = sprintf(buf, "failed to alloc buffer size of ddr_buf_len=%d from ddr\n", ddr_buf_len);
        copy_to_user(buffer, buf, len);
        *f_pos += len;

        return len;
    }

    len += sprintf(buf + len, "Test start: ddr address 0x%08x, buffer size = %d(KB), number of iterations = %d\n",
                  PAL_CPPI4_VIRT_2_PHYS(ddr_buffer), ddr_buf_len, NUM_OF_ITERATIONS);

    ddr_buf_len /= sizeof(*ddr_buffer);

    for (m = 0; m < NUM_OF_ITERATIONS; m++)
    {
        start = get_jiffies_64();

        for (n=0; n<50; n++)
        {
            /* write to buffer */
            for (k = 0; k < ddr_buf_len; k++)
            {
                ddr_buffer[k] = val_to_write;
                val_to_write = ~val_to_write;
            }

            /* read from buffer */
            for (k = 0; k < ddr_buf_len; k += 4)
            {
                temp = ddr_buffer[k];
            }
        }

        end = get_jiffies_64();
        len += sprintf(buf + len, ".");

        iterationsRunTime[m] = jiffies_to_msecs(end - start);
        totalTime += iterationsRunTime[m];
    }

    len += sprintf(buf + len, "\nTest done: total run time of %d(ms), average run time of %d(ms)\n",
                   totalTime, totalTime/NUM_OF_ITERATIONS);
    
    for (m = 0; m < NUM_OF_ITERATIONS; m++)
    {
        len += sprintf(buf + len, "It %d: %d(ms)\n", m, iterationsRunTime[m]);
    }

    len += sprintf(buf + len, "\n\n");
    copy_to_user(buffer, buf, len);
    *f_pos += len;
    kfree(ddr_buffer);

    return len;
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(l2x0_tests_init_module);
module_exit(l2x0_tests_cleanup_module);

MODULE_AUTHOR ("Intel Corporation");
MODULE_DESCRIPTION (DRV_NAME);
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);

