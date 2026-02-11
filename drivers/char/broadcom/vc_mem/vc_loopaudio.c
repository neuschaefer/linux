#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include <vc_vchi_audioserv_defs.h>
#include <vc_loopaudio.h>

#define DRIVER_NAME  "vc-loopaudio"

static dev_t vc_loopaudio_devnum;
static struct class *vc_loopaudio_class;
static struct cdev vc_loopaudio_cdev;
static int vc_loopaudio_inited = 0;
static VC_LOOPAUDIO_MAILBOX_T* vc_loopaudio_mailbox = 0;
static uint32_t vc_loopaudio_mailbox_phys = 0;

struct loopaudio {
    uint32_t total_read;
};

static int loopaudio_getflags(uint32_t* pflags)
{
    if (!vc_loopaudio_mailbox) return -ENOTCONN;
    *pflags = vc_loopaudio_mailbox->lflags;
    return 0;
}

static int loopaudio_setflags(uint32_t flags)
{
    if (!vc_loopaudio_mailbox) return -ENOTCONN;
    vc_loopaudio_mailbox->lflags = flags;
    return 0;
}

void vc_loopaudio_set_mailbox(void* p, uint32_t phys)
{
    vc_loopaudio_mailbox = p;
    vc_loopaudio_mailbox_phys = phys;
    loopaudio_setflags(VC_LOOPAUDIO_LOOP_MUTE);
}
EXPORT_SYMBOL(vc_loopaudio_set_mailbox);

static int vc_loopaudio_open(struct inode *inode, struct file *file)
{
    struct loopaudio* loopaudio;

    if (!vc_loopaudio_mailbox)
        return -ENOTCONN; // FIXME
    loopaudio = kmalloc(sizeof(struct loopaudio), GFP_KERNEL);
    if (!loopaudio)
        return -ENOMEM;
    loopaudio->total_read = vc_loopaudio_mailbox->written;
    //loopaudio_setflags(VC_LOOPAUDIO_HDMI_MUTE);
    file->private_data = loopaudio;
    return 0;
}

static int vc_loopaudio_release(struct inode *inode, struct file *file)
{
    struct loopaudio* loopaudio = (struct loopaudio*) file->private_data;
    //loopaudio_setflags(VC_LOOPAUDIO_LOOP_MUTE);
    kfree(loopaudio);
    return 0;
}

static ssize_t vc_loopaudio_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
    struct loopaudio* loopaudio = (struct loopaudio*) file->private_data;
    ssize_t written, num_read, avail_front, avail_back, index;

    // Wait for data to be available.
    for (;;) {
        if (signal_pending(current))
            return -EINTR;
        written = vc_loopaudio_mailbox->written;
        num_read = written - loopaudio->total_read;
        if (num_read > 0)
            break;
        if (file->f_flags & O_NONBLOCK)
            return -EAGAIN;
        msleep_interruptible(1);
    }
    // Copy available data to user buffer.
    if (num_read > count)
        num_read = count;
    index = loopaudio->total_read % VC_LOOPAUDIO_BUFFER_SIZE;
    avail_back = VC_LOOPAUDIO_BUFFER_SIZE - index;
    if (avail_back > num_read)
        avail_back = num_read;
    avail_front = (written < VC_LOOPAUDIO_BUFFER_SIZE) ? 0 :
                  (num_read > avail_back) ?  num_read - avail_back : 0;

    if (copy_to_user(ubuf, &vc_loopaudio_mailbox->buffer[index], avail_back))
        return -EFAULT;
    ubuf += avail_back;
    if (copy_to_user(ubuf, &vc_loopaudio_mailbox->buffer[0], avail_front))
        return -EFAULT;
    loopaudio->total_read += num_read;
    return num_read;
}

static uint32_t side_channel_total_read = 0;
void vc_side_channel_read(uint32_t *cnt, unsigned char** buf, uint32_t max_chunk)
{
    uint32_t max;
    if (vc_loopaudio_mailbox->written - side_channel_total_read > VC_LOOPAUDIO_BUFFER_SIZE/2)
        side_channel_total_read = vc_loopaudio_mailbox->written;

    *buf = &vc_loopaudio_mailbox->buffer[side_channel_total_read % VC_LOOPAUDIO_BUFFER_SIZE];
    max = VC_LOOPAUDIO_BUFFER_SIZE - (side_channel_total_read % VC_LOOPAUDIO_BUFFER_SIZE);
    *cnt = vc_loopaudio_mailbox->written - side_channel_total_read;
    if (*cnt > max) *cnt = max;
    if (*cnt > max_chunk) *cnt = max_chunk;
    side_channel_total_read += *cnt;
}
EXPORT_SYMBOL(vc_side_channel_read);

static long vc_loopaudio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    // struct loopaudio* loopaudio = (struct loopaudio*) file->private_data;
    int err;

    switch (cmd) {
    case VC_IOCTL_LOOPAUDIO_SETFLAGS: {
        uint32_t flags;
        if (copy_from_user(&flags, (void __user*) arg, sizeof(flags)))
            return -EFAULT;
        return loopaudio_setflags(flags);
    }
    case VC_IOCTL_LOOPAUDIO_GETFLAGS: {
        uint32_t flags;
        err = loopaudio_getflags(&flags);
        if (err) return err;
        if (copy_to_user((void __user*) arg, &flags, sizeof(flags)))
            return -EFAULT;
        return 0;
    }
    default:
        return -ENOTTY;
    }
}

static int vc_loopaudio_mmap(struct file *file, struct vm_area_struct *vma)
{    
    unsigned long length = vma->vm_end - vma->vm_start;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    return remap_pfn_range(vma, vma->vm_start,
            (vc_loopaudio_mailbox_phys >> PAGE_SHIFT) + vma->vm_pgoff,
            length, vma->vm_page_prot);
}


static const struct file_operations vc_loopaudio_fops = {
    .owner           = THIS_MODULE,
    .open            = vc_loopaudio_open,
    .release         = vc_loopaudio_release,
    .unlocked_ioctl  = vc_loopaudio_ioctl,
    .read            = vc_loopaudio_read,
    .mmap            = vc_loopaudio_mmap
};

static int __init vc_loopaudio_init(void)
{
    int rc = -EFAULT;
    struct device *dev;

    printk(KERN_INFO "vc-loopaudio: Videocore loopback audio driver\n");

    rc = alloc_chrdev_region(&vc_loopaudio_devnum, 0, 1, DRIVER_NAME);
    if (rc < 0) {
        printk(KERN_ERR "%s: alloc_chrdev_region failed (rc=%d)", __func__, rc);
        goto out_err;
    }

    cdev_init(&vc_loopaudio_cdev, &vc_loopaudio_fops);
    rc = cdev_add(&vc_loopaudio_cdev, vc_loopaudio_devnum, 1);
    if (rc != 0) {
        printk(KERN_ERR "%s: cdev_add failed (rc=%d)", __func__, rc);
        goto out_unregister;
    }

    vc_loopaudio_class = class_create(THIS_MODULE, DRIVER_NAME);
    if (IS_ERR(vc_loopaudio_class)) {
        rc = PTR_ERR(vc_loopaudio_class);
        printk(KERN_ERR "%s: class_create failed (rc=%d)", __func__, rc);
        goto out_cdev_del;
    }

    dev = device_create(vc_loopaudio_class, NULL, vc_loopaudio_devnum, NULL,
                DRIVER_NAME);
    if (IS_ERR(dev)) {
        rc = PTR_ERR(dev);
        printk(KERN_ERR "%s: device_create failed (rc=%d)", __func__, rc);
        goto out_class_destroy;
    }

    vc_loopaudio_inited = 1;
    return 0;

out_class_destroy:
    class_destroy(vc_loopaudio_class);
    vc_loopaudio_class = NULL;

out_cdev_del:
    cdev_del(&vc_loopaudio_cdev);

out_unregister:
    unregister_chrdev_region(vc_loopaudio_devnum, 1);

out_err:
    return rc;
}

static void __exit vc_loopaudio_exit(void)
{
    if (vc_loopaudio_inited) {
        device_destroy(vc_loopaudio_class, vc_loopaudio_devnum);
        class_destroy(vc_loopaudio_class);
        cdev_del(&vc_loopaudio_cdev);
        unregister_chrdev_region(vc_loopaudio_devnum, 1);
    }
}

module_init(vc_loopaudio_init);
module_exit(vc_loopaudio_exit);
MODULE_LICENSE("GPL"); // ???FIXME
MODULE_AUTHOR("Roku");
