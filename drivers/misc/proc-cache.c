#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

static struct proc_dir_entry *parent;

#define PROC_FUNC(entry)                                                \
    ssize_t proc_##entry(struct file *filp, const char __user *buff,    \
                  unsigned long len, void *data )                       \
    {                                                                   \
        __cpuc_##entry();                                               \
        return len;                                                     \
    }

#define PROC_ENTRY(entry)                                               \
    {                                                                   \
        struct proc_dir_entry *proc_entry = create_proc_entry(#entry, 0644, parent); \
        if (proc_entry == NULL) {                                       \
            printk(KERN_INFO "cache: Couldn't create proc entry %s\n", #entry); \
            return  -ENOMEM;                                            \
        } else {                                                        \
            proc_entry->read_proc = cache_read;                         \
            proc_entry->write_proc = proc_##entry;                      \
        }                                                               \
    }

PROC_FUNC(flush_icache_all);
PROC_FUNC(flush_kern_all);
PROC_FUNC(flush_user_all);

ssize_t proc_flush_tlb_all(struct file *filp, const char __user * buff,
			   unsigned long len, void *data)
{
	flush_tlb_all();
	return len;
}

int cache_read(char *page, char **start, off_t off,
	       int count, int *eof, void *data)
{
	int len = 0;
	printk(KERN_INFO "cache: read\n");
	return len;
}

static int __init proc_cache_init(void)
{
	parent = proc_mkdir("cache", NULL);
	if (!parent) {
		pr_err("cache: failed to create proc entry\n");
		return -1;
	}

	PROC_ENTRY(flush_icache_all);
	PROC_ENTRY(flush_kern_all);
	PROC_ENTRY(flush_user_all);
	PROC_ENTRY(flush_tlb_all);

	printk(KERN_INFO "cache: Module loaded.\n");
	return 0;
}

__initcall(proc_cache_init);
