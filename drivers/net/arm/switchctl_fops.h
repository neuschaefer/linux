#ifndef _SWITCHCTL_FOPS_H_
#define _SWITCHCTL_FOPS_H_

/* Prototypes */
/* ---------- */
int           switchctl_open   (struct inode *,struct file *);
ssize_t       switchctl_read   (struct file  *,char *,size_t,loff_t *);
ssize_t       switchctl_write  (struct file  *,const char *,size_t,loff_t *);
int           switchctl_flush  (struct file  *);
loff_t        switchctl_lseek  (struct file  *,loff_t,int);
int           switchctl_readdir(struct file  *,void *,filldir_t);
int           switchctl_ioctl  (struct inode *,struct file *,unsigned int,unsigned long);
unsigned int  switchctl_poll   (struct file  *,struct poll_table_struct *);
int           switchctl_fsync  (struct inode *,struct dentry *,int);
int           switchctl_fasync (int,struct file *,int);
ssize_t       switchctl_readv  (struct file  *,const struct iovec *,unsigned long,loff_t *);
ssize_t       switchctl_writev (struct file  *,const struct iovec *,unsigned long,loff_t *);
int           switchctl_release(struct inode *,struct file *);
int           switchctl_mmap(struct file * file, struct vm_area_struct * vma);

#endif
