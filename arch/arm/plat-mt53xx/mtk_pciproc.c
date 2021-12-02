/*
 * arch/arm/mach-mt5365/mtk_pciproc.c
 *
 * PCI and PCIe functions for MTK System On Chip
 *
 * Maintainer: Max Liao <max.liao@mediatek.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#ifdef CONFIG_PCI

#include <asm/uaccess.h>
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <plat/mtk_pci.h>

static struct proc_dir_entry * _mtkpci_testentry;

static int MTK_TestProcRead(char *page, char **start, 
	off_t off, int count, int *eof, void *data) 
{
	return 0;
} 

static int MTK_TestProcWrite(struct file *file, const char *buffer,
			     unsigned long count, void *data)
{
    char cmd[32];

    memset(cmd, 0, sizeof(cmd));
    if ((unsigned long)sizeof(cmd) != copy_from_user(&cmd, buffer, min(count, (unsigned long)sizeof(cmd))))
    {
        return 0;
    }
    printk("MTK_TestProcWrite: get (%ld)'%s'\n",
        min(count, (unsigned long)sizeof(cmd)), cmd);
    
    return count;
} 

void MTK_CreateTestProcFs(void) 
{
    if (_mtkpci_testentry)
    {
        return;
    }

    // Create a proc entry "mptp" => mtk pci test proc. 
    _mtkpci_testentry = create_proc_entry("mptp", S_IFREG | S_IRUGO | S_IWUSR, NULL);
    if ( _mtkpci_testentry )
    {
	_mtkpci_testentry->data = NULL;
	_mtkpci_testentry->read_proc = MTK_TestProcRead;
	_mtkpci_testentry->write_proc = MTK_TestProcWrite;
	_mtkpci_testentry->size = 0;

    	LOG("Registered /proc/mptp\n");
    } 
    else {
        LOG ("Cannot create a valid proc file entry");    
    }
}

#endif /*#ifdef CONFIG_PCI */

