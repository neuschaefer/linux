#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/time.h>
#include <linux/kernel_stat.h>

extern unsigned char _esn_mac[10];
extern unsigned char _esn_mac_extra[32];

static int esn_proc_show(struct seq_file *m, void *v)
{
	int i;
	for (i=0;i<10;i++)
		seq_printf(m, "%02x", _esn_mac[i]);
	seq_printf(m, "\n");
	for (i=0;i<32;i++)
		seq_printf(m, "%02x", _esn_mac_extra[i]);
	seq_printf(m, "\n");
	return 0;
}

static int esn_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, esn_proc_show, NULL);
}

static const struct file_operations esn_proc_fops = {
	.open		= esn_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_esn_init(void)
{
	proc_create("esn", 0440, NULL, &esn_proc_fops);
	return 0;
}
module_init(proc_esn_init);
