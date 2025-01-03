/*
 * Copyright (C) 2015 Pengutronix, Steffen Trumtrar <kernel@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/init.h>
#include "compat.h"
#include "regs.h"
#include "intern.h"
#include "desc.h"
#include "desc_constr.h"
#include "error.h"
#include "jr.h"

enum access_rights {
	KERNEL,
	USERSPACE,
};

struct blob_priv {
	struct device *jrdev;
	struct device dev;
	u32 *desc;
	u8 __iomem *red_blob;
	u8 __iomem *modifier;
	u8 __iomem *output;
	bool busy;
	enum access_rights access;
	unsigned int payload_size;
	struct bin_attribute *blob;
	struct bin_attribute *payload;
	struct caam_drv_private *ctrlpriv;
};

struct blob_job_result {
	int err;
	struct completion completion;
};

static struct platform_device *pdev;

static void blob_job_done(struct device *dev, u32 *desc, u32 err, void *context)
{
	struct blob_job_result *res = context;

#ifdef DEBUG
	dev_err(dev, "%s %d: err 0x%x\n", __func__, __LINE__, err);
#endif

	if (err)
		caam_jr_strstatus(dev, err);

	res->err = err;

	complete(&res->completion);
}

/*
 * Upon completion, desc points to a buffer containing a CAAM job
 * descriptor which encapsulates data into an externally-storable
 * blob.
 */
#define INITIAL_DESCSZ		16
#define BLOB_OVERHEAD		(32 + 16)
#define KEYMOD_LENGTH		16
#define RED_BLOB_LENGTH		64
#define MAX_BLOB_LEN		4096

/*
 * Generate a blob with the following format:
 * Format: Normal format (Test format only for testing)
 * Contents: General data (aka red blob)
 * Security State: Secure State (Non-Secure for testing)
 * Memory Types: General memory
 */
static int blob_encap_blob(struct blob_priv *priv, u8 *keymod, u8 *input,
				u8 *output, u16 length)
{
	u32 *desc;
	struct device *jrdev = priv->jrdev;
	dma_addr_t dma_keymod;
	dma_addr_t dma_in;
	dma_addr_t dma_out;
	struct blob_job_result testres;
	int ret;

	desc = kmalloc(CAAM_CMD_SZ * 5 + CAAM_PTR_SZ * 5, GFP_KERNEL | GFP_DMA);
	if (!desc) {
		dev_err(jrdev, "unable to allocate desc\n");
		return -ENOMEM;
	}

	dma_keymod = dma_map_single(jrdev, keymod, KEYMOD_LENGTH, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, dma_keymod)) {
		dev_err(jrdev, "unable to map keymod buffer\n");
		ret = -ENOMEM;
		goto out_free;
	}

	dma_in = dma_map_single(jrdev, input, length - BLOB_OVERHEAD,
					DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, dma_in)) {
		dev_err(jrdev, "unable to map keymod buffer\n");
		ret = -ENOMEM;
		goto out_unmap_key;
	}

	dma_out = dma_map_single(jrdev, output, length,	DMA_FROM_DEVICE);
	if (dma_mapping_error(jrdev, dma_out)) {
		dev_err(jrdev, "unable to map output DMA buffer\n");
		ret = -ENOMEM;
		goto out_unmap_in;
	}

	/*
	 * A data blob is encrypted using a blob key (BK); a random number.
	 * The BK is used as an AES-CCM key. The initial block (B0) and the
	 * initial counter (Ctr0) are generated automatically and stored in
	 * Class 1 Context DWords 0+1+2+3. The random BK is stored in the	
	 * Class 1 Key Register. Operation Mode is set to AES-CCM.
	 */

	init_job_desc(desc, 0);
	/*
	 * The key modifier can be used to differentiate specific data.
	 * Or to prevent replay attacks.
	 */
	append_key(desc, dma_keymod, KEYMOD_LENGTH, CLASS_2 | KEY_DEST_CLASS_REG);
	append_seq_in_ptr(desc, dma_in, length - BLOB_OVERHEAD, 0);
	append_seq_out_ptr(desc, dma_out, length, 0);
	append_operation(desc, OP_TYPE_ENCAP_PROTOCOL | OP_PCLID_BLOB);

#ifdef DEBUG
	printk("%s: keymod %s\n", __func__, keymod);
	print_hex_dump(KERN_ERR, "data@"__stringify(__LINE__)": ",
			DUMP_PREFIX_ADDRESS, 16, 1, input,
			length - BLOB_OVERHEAD, false);
	print_hex_dump(KERN_ERR, "jobdesc@"__stringify(__LINE__)": ",
			DUMP_PREFIX_ADDRESS, 16, 1, desc,
			desc_bytes(desc), false);
#endif

	testres.err = 0;
	init_completion(&testres.completion);

	ret = caam_jr_enqueue(jrdev, desc, blob_job_done, &testres);
	if (!ret) {
		wait_for_completion(&testres.completion);
		ret = testres.err;
#ifdef DEBUG
		print_hex_dump(KERN_ERR, "output@"__stringify(__LINE__)": ",
			       DUMP_PREFIX_ADDRESS, 16, 1, output,
			       length, false);
#endif
	}

	dma_unmap_single(jrdev, dma_out, length,
					DMA_FROM_DEVICE);
out_unmap_in:
	dma_unmap_single(jrdev, dma_keymod, KEYMOD_LENGTH, DMA_TO_DEVICE);
out_unmap_key:
	dma_unmap_single(jrdev, dma_in, length - BLOB_OVERHEAD, DMA_TO_DEVICE);
out_free:
	kfree(desc);

	return ret;
}

static int blob_decap_blob(struct blob_priv *priv, u8 *keymod, u8 *input,
				u8 *output, u16 length)
{
	u32 *desc;
	struct device *jrdev = priv->jrdev;
	dma_addr_t dma_keymod;
	dma_addr_t dma_in;
	dma_addr_t dma_out;
	struct blob_job_result testres;
	int ret;

	desc = kzalloc(CAAM_CMD_SZ * 5 + CAAM_PTR_SZ * 5, GFP_KERNEL | GFP_DMA);
	if (!desc) {
		dev_err(jrdev, "unable to allocate desc\n");
		return -ENOMEM;
	}

	dma_keymod = dma_map_single(jrdev, keymod, KEYMOD_LENGTH, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, dma_keymod)) {
		dev_err(jrdev, "unable to map keymod buffer\n");
		ret = -ENOMEM;
		goto out_free;
	}

	dma_in = dma_map_single(jrdev, input, length, DMA_TO_DEVICE);
	if (dma_mapping_error(jrdev, dma_in)) {
		dev_err(jrdev, "unable to map keymod buffer\n");
		ret = -ENOMEM;
		goto out_unmap_key;
	}

	dma_out = dma_map_single(jrdev, output, length - BLOB_OVERHEAD, DMA_FROM_DEVICE);
	if (dma_mapping_error(jrdev, dma_out)) {
		dev_err(jrdev, "unable to map output DMA buffer\n");
		ret = -ENOMEM;
		goto out_unmap_in;
	}

	/*
	 * A data blob is encrypted using a blob key (BK); a random number.
	 * The BK is used as an AES-CCM key. The initial block (B0) and the
	 * initial counter (Ctr0) are generated automatically and stored in
	 * Class 1 Context DWords 0+1+2+3. The random BK is stored in the	
	 * Class 1 Key Register. Operation Mode is set to AES-CCM.
	 */

	init_job_desc(desc, 0);
	/*
	 * The key modifier can be used to differentiate specific data.
	 * Or to prevent replay attacks.
	 */
	append_key(desc, dma_keymod, KEYMOD_LENGTH, CLASS_2 | KEY_DEST_CLASS_REG);
	append_seq_in_ptr(desc, dma_in, length, 0);
	append_seq_out_ptr(desc, dma_out, length - BLOB_OVERHEAD, 0);
	append_operation(desc, OP_TYPE_DECAP_PROTOCOL | OP_PCLID_BLOB);

#ifdef DEBUG
	print_hex_dump(KERN_ERR, "data@"__stringify(__LINE__)": ",
			DUMP_PREFIX_ADDRESS, 16, 1, input,
			length, false);
	print_hex_dump(KERN_ERR, "jobdesc@"__stringify(__LINE__)": ",
			DUMP_PREFIX_ADDRESS, 16, 1, desc,
			desc_bytes(desc), false);
#endif

	testres.err = 0;
	init_completion(&testres.completion);

	ret = caam_jr_enqueue(jrdev, desc, blob_job_done, &testres);
	if (!ret) {
		wait_for_completion(&testres.completion);
		ret = testres.err;
#ifdef DEBUG
		print_hex_dump(KERN_ERR, "output@"__stringify(__LINE__)": ",
			       DUMP_PREFIX_ADDRESS, 16, 1, output,
			       length - BLOB_OVERHEAD, false);
#endif
	}

	dma_unmap_single(jrdev, dma_out, length - BLOB_OVERHEAD, DMA_FROM_DEVICE);
out_unmap_in:
	dma_unmap_single(jrdev, dma_keymod, KEYMOD_LENGTH, DMA_TO_DEVICE);
out_unmap_key:
	dma_unmap_single(jrdev, dma_in, length, DMA_TO_DEVICE);
out_free:
	kfree(desc);

	return ret;
}

static ssize_t
blob_gen_blob_read(struct file *filp, struct kobject *kobj, struct bin_attribute *attr,
		char *buf, loff_t off, size_t size)
{
	struct device *dev = kobj_to_dev(kobj);
	struct blob_priv *priv = dev_get_drvdata(dev);
	int length = min((int)size, (int) (priv->payload_size + BLOB_OVERHEAD - off));
	int ret;

	if (length > 0) {
		priv->busy = true;

		ret = blob_encap_blob(priv, priv->modifier, priv->output, priv->red_blob, length);
		if (ret) {
			priv->busy = false;
			return -EINVAL;
		}

		memcpy(buf, priv->red_blob, length);

		priv->busy = false;
	}

	return length;
}

static ssize_t
blob_gen_blob_write(struct file *filp, struct kobject *kobj,
		struct bin_attribute *attr, char *buf, loff_t off, size_t size)
{
	struct device *dev = kobj_to_dev(kobj);
	struct blob_priv *priv = dev_get_drvdata(dev);
	int ret;

	if (size > 0) {
		priv->busy = true;

		memcpy(priv->red_blob, buf, size);

		memset(priv->output, 0, MAX_BLOB_LEN - BLOB_OVERHEAD);

		ret = blob_decap_blob(priv, priv->modifier, priv->red_blob,
					priv->output, size);
		if (ret) {
			priv->busy = false;
			return -EINVAL;
		}

		priv->payload_size = size - BLOB_OVERHEAD;

		priv->busy = false;
	}

	return size;
}

static ssize_t
blob_gen_payload_read(struct file *filp, struct kobject *kobj,
			struct bin_attribute *attr, char *buf, loff_t off,
			size_t size)
{
	struct device *dev = kobj_to_dev(kobj);
	struct blob_priv *priv = dev_get_drvdata(dev);
	int length = min((int) size, (int) (priv->payload_size - off));

	if (priv->payload_size == 0)
		return -EINVAL;

	if (priv->access != USERSPACE)
		return -EPERM;

	memcpy(buf, priv->output + off, length);

	return length;
}

static ssize_t
blob_gen_payload_write(struct file *filp, struct kobject *kobj,
			struct bin_attribute *attr, char *buf, loff_t off,
			size_t size)
{
	struct device *dev = kobj_to_dev(kobj);
	struct blob_priv *priv = dev_get_drvdata(dev);

	if (priv->busy)
		return -EPERM;

	memcpy(priv->output, buf, size);

	priv->payload_size = size;

	return size;
}

static ssize_t
blob_gen_show_modifier(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	struct blob_priv *priv = dev_get_drvdata(dev);

	memcpy(buf, priv->modifier, KEYMOD_LENGTH);

	return KEYMOD_LENGTH;
}

static ssize_t
blob_gen_set_modifier(struct device *dev, struct device_attribute *attr,
			const char *buf, size_t n)
{
	struct blob_priv *priv = dev_get_drvdata(dev);
	char *buf_ptr;

	if (n > KEYMOD_LENGTH)
		return -EINVAL;

	if (priv->busy)
		return -EPERM;

	buf_ptr = (char *)buf;
	if (!strncmp(buf_ptr, "kernel:", 7))
		priv->access = KERNEL;
	else if (!strncmp(buf_ptr, "user:", 5))
		priv->access = USERSPACE;

	memset(priv->output, 0, MAX_BLOB_LEN - BLOB_OVERHEAD);
	memset(priv->red_blob, 0, MAX_BLOB_LEN);

	memcpy(priv->modifier, buf, n);

	return n;
}
static DEVICE_ATTR(modifier, S_IRUSR | S_IWUSR,
		blob_gen_show_modifier, blob_gen_set_modifier);

static struct attribute *blob_attrs[] = {
	&dev_attr_modifier.attr,
	NULL,
};
ATTRIBUTE_GROUPS(blob);

static int __init blob_gen_init(void)
{
	struct device_node *dev_node;
	struct device *ctrldev;
	struct device_node *childdevnode;
	struct platform_device *childdev;
	struct caam_drv_private *ctrlpriv;
	struct blob_priv *priv;
	int ret;

	dev_node = of_find_compatible_node(NULL, NULL, "fsl,sec-v4.0");
	if (!dev_node) {
		dev_node = of_find_compatible_node(NULL, NULL, "fsl,sec4.0");
		if (!dev_node)
			return -ENODEV;
	}

	pdev = of_find_device_by_node(dev_node);
	if (!pdev) {
		of_node_put(dev_node);
		return -ENODEV;
	}

	ctrldev = &pdev->dev;
	ctrlpriv = dev_get_drvdata(ctrldev);

	pdev = platform_device_alloc("blob_gen", -1);
	if (!pdev)
		return -ENOMEM;
	pdev->dev.parent = ctrldev;
	pdev->dev.groups = blob_groups;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	priv->blob = devm_kzalloc(&pdev->dev, sizeof(struct bin_attribute),
					GFP_KERNEL);
	priv->payload = devm_kzalloc(&pdev->dev, sizeof(struct bin_attribute),
					GFP_KERNEL);

	ret = platform_device_add(pdev);
	if (ret)
		goto pdev_add_failed;

	priv->modifier = devm_kzalloc(&pdev->dev, sizeof(*priv->modifier)*KEYMOD_LENGTH,
			GFP_KERNEL | GFP_DMA);

	priv->red_blob = devm_kzalloc(&pdev->dev, sizeof(*priv->red_blob)*
					MAX_BLOB_LEN, GFP_KERNEL | GFP_DMA);

	priv->output = devm_kzalloc(&pdev->dev, sizeof(*priv->output)*
					MAX_BLOB_LEN - BLOB_OVERHEAD, GFP_KERNEL | GFP_DMA);

	priv->desc = devm_kzalloc(&pdev->dev, sizeof(*priv->desc), GFP_KERNEL | GFP_DMA);
	if (priv->desc == NULL)
		return -ENOMEM;

	dev_set_drvdata(&pdev->dev, priv);

	priv->busy = false;

	/* use first child (sec_jr0) as job ring */
	childdevnode = NULL;
	for_each_available_child_of_node(dev_node, childdevnode) {
		if (of_device_is_compatible(childdevnode,
				"fsl,sec-v4.0-job-ring") ||
				of_device_is_compatible(childdevnode,
				"fsl,sec4.0-job-ring")) {
			break;
		}
	}

	if (!of_node_is_initialized(childdevnode))
		return -ENODEV;

	childdev = of_find_device_by_node(childdevnode);
	priv->jrdev = &childdev->dev;

	priv->blob->attr.name = "blob";
	priv->blob->attr.mode = S_IRUSR | S_IWUSR;

	sysfs_bin_attr_init(priv->blob);
	priv->blob->read = blob_gen_blob_read;
	priv->blob->write = blob_gen_blob_write;

	ret = sysfs_create_bin_file(&pdev->dev.kobj, priv->blob);
	if (ret)
		dev_err(&pdev->dev, "unable to create sysfs file %s\n",
			priv->blob->attr.name);
	else
		dev_info(&pdev->dev, "added sysfs binary attribute\n");

	priv->payload->attr.name = "payload";
	priv->payload->attr.mode = S_IRUSR | S_IWUSR;

	sysfs_bin_attr_init(priv->payload);
	priv->payload->read = blob_gen_payload_read;
	priv->payload->write = blob_gen_payload_write;

	ret = sysfs_create_bin_file(&pdev->dev.kobj, priv->payload);
	if (ret)
		dev_err(&pdev->dev, "unable to create sysfs file %s\n",
			priv->payload->attr.name);
	else
		dev_info(&pdev->dev, "added sysfs binary attribute\n");

	return 0;

pdev_add_failed:
	platform_device_put(pdev);

	return ret;
}

static void __exit blob_gen_exit(void)
{
	struct blob_priv *priv;

	priv = dev_get_drvdata(&pdev->dev);
	if (!priv)
		return;

	sysfs_remove_bin_file(&pdev->dev.kobj, priv->payload);
	sysfs_remove_bin_file(&pdev->dev.kobj, priv->blob);

	platform_device_unregister(pdev);
}

module_init(blob_gen_init);
module_exit(blob_gen_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Blob Generator Example");
MODULE_AUTHOR("Steffen Trumtrar <s.trumtrar@pengutronix.de>");
