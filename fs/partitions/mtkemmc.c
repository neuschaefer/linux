/*
 *
 *  fs/partitions/mtkemmc.c
 *
 *  this partition system using for EMMC
 *
 *  maked by jun.wang
 *
 */

#include <linux/proc_fs.h>
#include <linux/mmc/card.h>

#include "check.h"
#include "mtkemmc.h"

/*
 * Many architectures don't like unaligned accesses, while
 * the nr_sects and start_sect partition table entries are
 * at a 2 (mod 4) address.
 */
#include <asm/unaligned.h>

mtk_partmap_info mtk_mmc_partmap;

/* the command line passed to mtdpart_setupd() */
static char *cmdline;

static char *newpart(char *s)
{
    char *retptr;
    unsigned long size = 0;
    unsigned long offset = 0;
    char *name;
    int name_len;
    unsigned char partno;
    mtk_part_info *part;

    /* fetch the partition size */
	size = memparse(s, &s);
    BUG_ON(size < 512);
    BUG_ON(size % 512);
    size /= 512;

    /* check for offset */
    if (*s == '@')
    {
        s++;
        offset = memparse(s, &s);
        BUG_ON(offset % 512);
    }
    
    /* now look for name */
    if (*s == '(')
    {
    	char *p;

        name = ++s;
    	p = strchr(name, ')');
    	BUG_ON(!p);
        
    	name_len = p - name;
    	s = p + 1;
    }
    else
    {
        name = NULL;
    	name_len = 8; /* Part_000 */
    }

    if (strncmp(s, "ro", 2) == 0)
    {
        s += 2;
    }

    if (strncmp(s, "lk", 2) == 0)
    {
        s += 2;
    }

    if (strncmp(s, "enc", 3) == 0)
    {
        s += 3;
    }

    /* test if more partitions are following */
    if (*s == ',')
    {
        /* return (updated) pointer command line string */
        retptr = ++s;
    }
    else
    {
        retptr = NULL;
    }
    
    mtk_mmc_partmap.nparts++;
    partno= mtk_mmc_partmap.nparts;
    part = &(mtk_mmc_partmap.partition[mtk_mmc_partmap.nparts]);
    
    if (name)
    {
    	strlcpy(part->name, name, name_len + 1);
    }
    else
    {
    	sprintf(part->name, "Part_%02d", partno);
    }

    if (partno > 1)
    {
        mtk_part_info *prevpart = &(mtk_mmc_partmap.partition[partno-1]);
        part->start_sect = offset + prevpart->start_sect + prevpart->nr_sects;
    }
    else
    {
        part->start_sect = offset;
    }
    
    part->nr_sects = size;

    return retptr;
}

static int mtdpart_setup_real(void)
{
    char *p;
    
    /* fetch <mtd-id> */
    if (!(p = strchr(cmdline, ':')))
    {
        printk(KERN_ERR "mtdpart_setup_real: no mtd-id!\n");
        return -1;
    }
    
    p++;

    while (p)
    {
        p = newpart(p);
    }

    return 0;
}

#ifdef CONFIG_PROC_FS
/*====================================================================*/

// Support for /proc/partinfo
static struct proc_dir_entry *proc_partinfo;

static int partinfo_read_proc (char *page, char **start, off_t off, int count,
			  int *eof, void *data_unused)
{
	int len, l, partno;
    off_t begin = 0;
    mtk_part_info *part;

	len = sprintf(page, "dev:    offset   size     name\n");

	for (partno = 1; partno <= mtk_mmc_partmap.nparts; partno++)
    {
        part = &(mtk_mmc_partmap.partition[partno]);

        l = sprintf(page + len, "part%02d: %8.8llx %8.8llx \"%s\"\n", partno,
				(unsigned long long)part->start_sect,
				(unsigned long long)part->nr_sects,
				part->name);
	    len += l;

        if (len+begin > off+count)
        {
            goto done;
        }
        
	    if (len+begin < off)
        {
            begin += len;
            len = 0;
	    }
	}

	*eof = 1;

done:
	if (off >= len+begin)
	{
	    return 0;
	}
    
	*start = page + (off-begin);
	return ((count < begin+len-off) ? count : begin+len-off);
}
#endif /* CONFIG_PROC_FS */

int mtkemmc_partition(struct parsed_partitions *state)
{
    int partno;
    mtk_part_info *part;

    memset(&mtk_mmc_partmap, 0, sizeof(mtk_mmc_partmap));
    
    BUG_ON(mtdpart_setup_real());
    BUG_ON(mtk_mmc_partmap.nparts >= state->limit);

    for(partno = 1; partno <= mtk_mmc_partmap.nparts; partno++)
    {
        part = &(mtk_mmc_partmap.partition[partno]);
        put_named_partition(state, partno, part->start_sect, part->nr_sects, part->name, strlen(part->name));
        
        printk(KERN_INFO "part %02d: startblk 0x%08x, blkcnt 0x%08x, <%s>\n",
             partno, part->start_sect, part->nr_sects, part->name);
    }
    
#ifdef CONFIG_PROC_FS
	if ((proc_partinfo = create_proc_entry("partinfo", 0, NULL)))
		proc_partinfo->read_proc = partinfo_read_proc;
#endif /* CONFIG_PROC_FS */

	return 1;
}

/*
 * This is the handler for our kernel parameter, called from
 * main.c::checksetup(). Note that we can not yet kmalloc() anything,
 * so we only save the commandline for later processing.
 *
 * This function needs to be visible for bootloaders.
 */
static int emmcpart_setup(char *s)
{
	cmdline = s;
	return 1;
}
__setup("mtdparts=", emmcpart_setup);

