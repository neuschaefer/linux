/*
 *
 * cppi_server.c
 * Description:
 * cppi over MRPC server driver
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2017 Intel Corporation.
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

#define pr_fmt(fmt) KBUILD_MODNAME ":%s:%d " fmt "\n", __func__, __LINE__

/************************************************/
/**                    Includes                 */
/************************************************/
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/byteorder/generic.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/err.h>
#include <linux/netdevice.h>
#include <linux/ethtool.h>
#include <linux/mrpc/mrpc.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>

extern Int32 avalanche_cppi_init_pp_sanity_dma_channels(PAL_Handle palHandle, Cppi4Queue freeQ,
                                                         Cppi4BufPool freeBuf);
extern u32 cppi_get_pp_free_queues_count(void);
extern u32 cppi_get_pp_free_queue_info(u32 fqueue_idx, FDqueue_t *info);
extern u32 cppi_get_buffer_pool_bsmon_info(u32 idx, qsmonRal_t *info);

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif

/* driver private database */
struct cppi_private {
    struct platform_device *pdev;   /* platform device */
    struct mrpc_server *mrpc;       /* mrpc server handle */
};

struct mrpc_cppi_init_pp_sanity_dma_channels_args {
    Uint32 freeQMgr;
    Uint32 freeQNum;
    Uint32 freeBMgr;
    Uint32 freeBpool;
};

struct mrpc_cppi_pp_get_bpool_info_args {
    Uint32 bMgr;
    Uint32 bPool;
};

struct mrpc_cppi_pp_get_bpool_info_rep {
    PAL_Result result;
    Cppi4BufPoolInfo bp_info;
};
/* p_arglen is assigned the sizof(func_args), therefor each arguments struct name should begin with the function's name */
#define CPPI_PROC_32(proc, func)                      \
[proc] = {                                            \
        .procid   = proc,                             \
        .p_func   = func,                             \
        .p_arglen = sizeof(struct func ## _args),     \
        .p_replen = sizeof(Uint32),                   \
        .p_name   = #func,                            \
}

#define CPPI_PROC_BPOOL_INFO(proc, func)                               \
[proc] = {                                                             \
        .procid   = proc,                                              \
        .p_func   = func,                                              \
        .p_arglen = sizeof(struct func ## _args),                      \
        .p_replen = sizeof(struct mrpc_cppi_pp_get_bpool_info_rep),    \
        .p_name   = #func,                                             \
}

#define CPPI_PROC_BPOOL_BSMON_INFO(proc, func)                         \
[proc] = {                                                             \
        .procid   = proc,                                              \
        .p_func   = func,                                              \
        .p_arglen = sizeof(u32),                                       \
        .p_replen = sizeof(qsmonRal_t),                                \
        .p_name   = #func,                                             \
}

#define CPPI_PROC_PP_FREE_QUEUE_INFO(proc, func)                       \
[proc] = {                                                             \
        .procid   = proc,                                              \
        .p_func   = func,                                              \
        .p_arglen = sizeof(u32),                                       \
        .p_replen = sizeof(FDqueue_t),                                 \
        .p_name   = #func,                                             \
}

#define CPPI_PROC_PP_FREE_QUEUES_COUNT(proc, func)      \
[proc] = {                                              \
        .procid   = proc,                               \
        .p_func   = func,                               \
        .p_arglen = sizeof(u32),                        \
        .p_replen = sizeof(u32),                        \
        .p_name   = #func,                              \
}

enum {
	CPPI_GET_BPOOL_INFO = 0,
    CPPI_CONFIGURE_SANITY_DMA_CHANNELS,
    CPPI_GET_BPOOL_BSMON_INFO,
    CPPI_GET_PP_FREE_QUEUES_COUNT,
    CPPI_GET_PP_FREE_QUEUE_INFO,
};

static int mrpc_cppi_init_pp_sanity_dma_channels(void *arg, ssize_t arglen,
                          void *rep, ssize_t replen)
{
    PAL_Handle handle;
    Uint32 *p_rep = rep;
    struct mrpc_cppi_init_pp_sanity_dma_channels_args *p_arg = arg;

    Cppi4Queue freeQ =
    {
        .qMgr = ntohl(p_arg->freeQMgr),
        .qNum = ntohl(p_arg->freeQNum)
    };

    Cppi4BufPool freeBuf =
    {
        .bMgr = ntohl(p_arg->freeBMgr),
        .bPool = ntohl(p_arg->freeBpool)
    };

    handle = PAL_cppi4Init(NULL, CPPI41_DOMAIN_PP);
    if (handle == NULL)
        return -1;
    DBG("init_pp_sanity_dma_channels: fQMgr %d fQNum %d fBufMgr %d fBufPool %d\n",
        freeQ.qMgr, freeQ.qNum, freeBuf.bMgr, freeBuf.bPool);

    *p_rep = avalanche_cppi_init_pp_sanity_dma_channels(handle, freeQ, freeBuf);
    *p_rep = htonl(*p_rep);
    return 0;
}

static int mrpc_cppi_pp_get_bpool_info(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    PAL_Handle hnd;
    struct mrpc_cppi_pp_get_bpool_info_args *p_arg = arg;
    struct mrpc_cppi_pp_get_bpool_info_rep *p_rep = rep;


    Cppi4BufPool pool =
    {
        .bMgr =  be32_to_cpu(p_arg->bMgr),
        .bPool = be32_to_cpu(p_arg->bPool)
    };

    memset(p_rep, 0 , replen);

    hnd = PAL_cppi4Init(NULL, CPPI41_DOMAIN_PP);
    if (hnd == NULL)
        return -1;


    p_rep->result = PAL_cppi4GetBufferPoolInfo(hnd, pool, &p_rep->bp_info); 

    DBG("cppi_buf_info: bPoolPtr 0x%08x bPoolTopAddr 0x%08x bPoolOffset 0x%08x bPoolName %s\n", p_rep->bp_info.bPoolPhyAddr, p_rep->bp_info.bPoolTopPhyAddr, p_rep->bp_info.bPoolOffset, PalCppiPpbmgrNames[pool.bPool]); 

    p_rep->bp_info.bPoolOffset = cpu_to_be32(p_rep->bp_info.bPoolOffset);
    p_rep->bp_info.bPoolPhyAddr = cpu_to_be32(p_rep->bp_info.bPoolPhyAddr);
    p_rep->bp_info.bPoolSize = cpu_to_be32(p_rep->bp_info.bPoolSize);
    p_rep->bp_info.bPoolTopPhyAddr = cpu_to_be32(p_rep->bp_info.bPoolTopPhyAddr);
    p_rep->bp_info.bufSize = cpu_to_be32(p_rep->bp_info.bufSize);
    p_rep->bp_info.numBuf = cpu_to_be32(p_rep->bp_info.numBuf);

    return 0;
}

static int mrpc_cppi_get_bpool_bsmon_info(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    return cppi_get_buffer_pool_bsmon_info(*((u32 *) arg), (qsmonRal_t *) rep);
}

static int mrpc_cppi_get_pp_free_queues_count(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    u32 *cnt = rep;
    *cnt = cppi_get_pp_free_queues_count();
    return 0;
}

static int mrpc_cppi_get_pp_free_queue_info(void *arg, ssize_t arglen, void *rep, ssize_t replen)
{
    return cppi_get_pp_free_queue_info(*((u32 *) arg), (FDqueue_t *) rep);
}

static struct mrpc_procinfo cppi_server_procs[] = {
    CPPI_PROC_BPOOL_INFO(CPPI_GET_BPOOL_INFO, mrpc_cppi_pp_get_bpool_info),
    CPPI_PROC_32(CPPI_CONFIGURE_SANITY_DMA_CHANNELS, mrpc_cppi_init_pp_sanity_dma_channels),
    CPPI_PROC_BPOOL_BSMON_INFO(CPPI_GET_BPOOL_BSMON_INFO, mrpc_cppi_get_bpool_bsmon_info),
    CPPI_PROC_PP_FREE_QUEUES_COUNT(CPPI_GET_PP_FREE_QUEUES_COUNT, mrpc_cppi_get_pp_free_queues_count),
    CPPI_PROC_PP_FREE_QUEUE_INFO(CPPI_GET_PP_FREE_QUEUE_INFO, mrpc_cppi_get_pp_free_queue_info),
};

/* sysfs for future use */
static ssize_t status_show(struct device *dev,
                           struct device_attribute *attr, char *buf)
{
    struct cppi_private *priv = dev_get_drvdata(dev);

    if (!priv)
        return -EINVAL;

    return scnprintf(buf, PAGE_SIZE, "status ok");
}

static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *cppi_attrs[] = {
    &dev_attr_status.attr,
    NULL
};

static struct attribute_group cppi_attrs_group = {
    .attrs = cppi_attrs,
};

/**
 * cppi_probe
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int cppi_probe(struct platform_device *pdev)
{
    struct cppi_private *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        pr_err("memory allocation failed");
        return -ENOMEM;
    }

    dev_set_drvdata(&pdev->dev, priv);
    priv->pdev = pdev;

    ret = sysfs_create_group(&priv->pdev->dev.kobj, &cppi_attrs_group);
    if (ret) {
        pr_err("sysfs_create_group failed (ret=%d)", ret);
        return ret;
    }

    priv->mrpc = mrpc_server_register(MRPC_RESERVED_ID_CPPI, "cppi_server", 
                                      cppi_server_procs,
                                      ARRAY_SIZE(cppi_server_procs));
    if (!priv->mrpc) {
        pr_err("failed to register cppi");
        ret = -ENODEV;
        goto out_remove_group;
    }

    return 0;

out_remove_group:
    sysfs_remove_group(&priv->pdev->dev.kobj, &cppi_attrs_group);
    return 0;
}

/**
 * cppi_remove
 *
 * This function is called when the cppi driver is removed.
 *
 * @param pdev   platform device
 *
 * @return 0 for success, error code otherwise
 */
static int cppi_remove(struct platform_device *pdev)
{
    struct cppi_private *priv = platform_get_drvdata(pdev);

    mrpc_server_unregister(priv->mrpc);
    sysfs_remove_group(&priv->pdev->dev.kobj, &cppi_attrs_group);
    dev_set_drvdata(&pdev->dev, NULL);

    return 0;
}

static struct platform_driver cppi_driver = {
    .driver = {
        .name   = "cppi_server",
    },
    .probe = cppi_probe,
    .remove = cppi_remove,
};

static struct platform_device *cppi_device;

static int __init cppi_server_init(void)
{
    int ret;

    ret = platform_driver_register(&cppi_driver);
    if (ret < 0) {
        pr_err("Failed to register cppi platform driver: %d\n", ret);
        return ret;
    }

    cppi_device = platform_device_register_simple("cppi_server", -1, NULL, 0);
    if (IS_ERR(cppi_device)) {
        pr_err("Failed to register cppi platform device\n");
        platform_driver_unregister(&cppi_driver);
        return PTR_ERR(cppi_device);
    }

    return 0;
}

static void __exit cppi_server_exit(void)
{
    platform_device_unregister(cppi_device);
    platform_driver_unregister(&cppi_driver);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(cppi_server_init);
module_exit(cppi_server_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Yair Weiss yair.weiss@intel.com");
MODULE_DESCRIPTION("cppi mrpc server");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
