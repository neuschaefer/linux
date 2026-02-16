/*
 *
 * sgc_client.c
 * Description:
 * SGC mrpc client driver
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2016 Intel Corporation.
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
#include "ppsgc_db.h"

/* driver private database */
struct SgcClientPrivate {
	/* platform device */
	struct platform_device  *pdev;
	/* mrpc client handle */
	struct mrpc_client      *mrpc;
	/* number of successful MRPC calls made */
	unsigned int            callSuccessCount;
	/* number of failure  MRPC calls made */
	unsigned int            callFailureCount;
};

enum {
	SGC_ALLOC_GROUP_COUNTER_ID_PROC = 0,
	SGC_RELEASE_GROUP_COUNTER_ID_PROC,
	SGC_DEL_GROUP_COUNTER_SESSIONS_PROC,
	SGC_GET_GROUP_COUNTER_AMOUNT_PROC,
	SGC_GET_IF_COUNTERS_PROC,
	SGC_GET_TP_PORT_COUNTERS_PROC,
	SGC_GET_SF_COUNTERS_PROC,
	SGC_PRINT_TPPORT_COUNTERS_PROC,
	SGC_PRINT_IF_COUNTERS_PROC,
	SGC_PRINT_GROUP_COUNTERS_PROC,
};

static ssize_t status_show
	(struct device *dev, struct device_attribute *attr, char *buf);
static int sgc_mrpc_probe(struct platform_device *pdev);
static int sgc_mrpc_remove(struct platform_device *pdev);
static inline int sgc_mrpc_call
	(__u8 procid, void *args, ssize_t arg_len, void *rep, ssize_t rep_len);

static struct SgcClientPrivate  *this;
static DEVICE_ATTR(status, S_IRUGO, status_show, NULL);

static struct attribute *sgc_attrs[] = {
	&dev_attr_status.attr,
	NULL
};

static struct attribute_group sgc_attrs_group = {
	.attrs = sgc_attrs,
};

static struct platform_driver sgc_driver = {
	.driver = {
		.name = "sgc_mrpc_client",
	},
	.probe = sgc_mrpc_probe,
	.remove = sgc_mrpc_remove,
};

static struct platform_device *sgc_device;


/**************************************************************************/
/*! \fn int PPSGC_Db_AllocGroupCounterId(unsigned short *groupCounterId)
 **************************************************************************
 *  \brief Allocate Group Counter ID.
 *  \param[in/out] groupCounterId - Pointer to group counter ID value.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_AllocGroupCounterId(unsigned short *groupCounterId)
{
	int                 ret;
	unsigned int        tmpGroupCounterId;
	unsigned short      allocGroupCounterId;

	ret = sgc_mrpc_call(SGC_ALLOC_GROUP_COUNTER_ID_PROC, NULL, 0,
			&tmpGroupCounterId, sizeof(unsigned int));
	tmpGroupCounterId = ntohl(tmpGroupCounterId);
	allocGroupCounterId = tmpGroupCounterId;
	*groupCounterId = allocGroupCounterId;
	return ret;
}
EXPORT_SYMBOL(PPSGC_Db_AllocGroupCounterId);


/**************************************************************************/
/*! \fn int PPSGC_Db_ReleaseGroupCounterId(unsigned short groupCounterId)
 **************************************************************************
 *  \brief Release Group Counter ID.
 *  \param[in] groupCounterId - Group Counter ID to release.
 *  \param[out] no output.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_ReleaseGroupCounterId(unsigned short groupCounterId)
{
	unsigned int    relGroupCounterId = groupCounterId;

	relGroupCounterId = htonl(relGroupCounterId);
	return sgc_mrpc_call(SGC_RELEASE_GROUP_COUNTER_ID_PROC,
			&relGroupCounterId, sizeof(unsigned int), NULL, 0);
}
EXPORT_SYMBOL(PPSGC_Db_ReleaseGroupCounterId);


/**************************************************************************/
/*! \fn int PPSGC_Db_DeleteGroupCounterSessions(unsigned short groupCounterId)
 **************************************************************************
 *  \brief Send request to PP to delete sessions list of Group Counter ID.
 *  \param[in] groupCounterId - Group Counter ID to delete sessions.
 *  \param[out] no output.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_DeleteGroupCounterSessions(unsigned short groupCounterId)
{
	unsigned int    delGroupCounterId = groupCounterId;

	delGroupCounterId = htonl(delGroupCounterId);
	return sgc_mrpc_call(SGC_DEL_GROUP_COUNTER_SESSIONS_PROC,
			&delGroupCounterId, sizeof(unsigned int), NULL, 0);
}
EXPORT_SYMBOL(PPSGC_Db_DeleteGroupCounterSessions);


/**************************************************************************/
/*! \fn int PPSGC_Db_GetGroupCounterAmount
 *                      (struct PpsgcGroupCounters *totalPpCounters)
 **************************************************************************
 *  \brief Retreives the total counters amount of all session in specified
 *         Group Counter.
 *  \param[in,out] totalPpCounters - Total Group Counter counters amount.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_GetGroupCounterAmount(struct PpsgcGroupCounters *totalPpCounters)
{
	int                     ret;
	unsigned int            groupCounterId =
			totalPpCounters->groupCounterId;
	struct PpsgcPpCounters  repPpCounters;

	groupCounterId = htonl(groupCounterId);
	ret = sgc_mrpc_call(SGC_GET_GROUP_COUNTER_AMOUNT_PROC,
			&groupCounterId, sizeof(unsigned int),
			&repPpCounters, sizeof(struct PpsgcPpCounters));

	totalPpCounters->ppCounters.totalBytesFwd    =
		be64_to_cpu(repPpCounters.totalBytesFwd);
	totalPpCounters->ppCounters.totalPktsFwd     =
		be64_to_cpu(repPpCounters.totalPktsFwd);
	totalPpCounters->ppCounters.multicastPktsFwd =
		be64_to_cpu(repPpCounters.multicastPktsFwd);
	totalPpCounters->ppCounters.unicastPktsFwd   =
		be64_to_cpu(repPpCounters.unicastPktsFwd);

	return ret;
}
EXPORT_SYMBOL(PPSGC_Db_GetGroupCounterAmount);


/**************************************************************************/
/*! \fn int PPSGC_Db_GetIfCounters
 *                      (struct PpsgcIfCounters *ppsgcIfCounters)
 **************************************************************************
 *  \brief Get packet processor interface device counters according to
 *         interface name.
 *  \param[in,out] ppsgcIfCounters - Packet processor interface device
 *      counters structure.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_GetIfCounters(struct PpsgcIfCounters *ppsgcIfCounters)
{
	int                     ret;
	struct PpsgcIfCounters  repPpsgcIfCounter;

	ret = sgc_mrpc_call(SGC_GET_IF_COUNTERS_PROC, ppsgcIfCounters,
			sizeof(struct PpsgcIfCounters), &repPpsgcIfCounter,
			sizeof(struct PpsgcIfCounters));

	ppsgcIfCounters->ifCounters.rx_byte          =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.rx_byte);
	ppsgcIfCounters->ifCounters.rx_unicast_pkt   =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.rx_unicast_pkt);
	ppsgcIfCounters->ifCounters.rx_broadcast_pkt =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.rx_broadcast_pkt);
	ppsgcIfCounters->ifCounters.rx_multicast_pkt =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.rx_multicast_pkt);
	ppsgcIfCounters->ifCounters.rx_discard_pkt   =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.rx_discard_pkt);

	ppsgcIfCounters->ifCounters.tx_byte          =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.tx_byte);
	ppsgcIfCounters->ifCounters.tx_unicast_pkt   =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.tx_unicast_pkt);
	ppsgcIfCounters->ifCounters.tx_broadcast_pkt =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.tx_broadcast_pkt);
	ppsgcIfCounters->ifCounters.tx_multicast_pkt =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.tx_multicast_pkt);
	ppsgcIfCounters->ifCounters.tx_discard_pkt   =
		be64_to_cpu(repPpsgcIfCounter.ifCounters.tx_discard_pkt);

	return ret;
}
EXPORT_SYMBOL(PPSGC_Db_GetIfCounters);


/**************************************************************************/
/*! \fn int PPSGC_Db_GetTpPortCounters
 *                      (struct PpsgcTpPortCounters* ppsgcTpPortCounters)
 **************************************************************************
 *  \brief Get PP transparent port counters according to interface name.
 *  \param[in,out] ppsgcTpPortCounters-PP transparent port counters structure.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_GetTpPortCounters(struct PpsgcTpPortCounters *ppsgcTpPortCounters)
{
	int                         ret;
	struct PpsgcTpPortCounters  repPsgcTpPortCounters;

	ret = sgc_mrpc_call(SGC_GET_TP_PORT_COUNTERS_PROC, ppsgcTpPortCounters,
		sizeof(struct PpsgcTpPortCounters),
		&repPsgcTpPortCounters, sizeof(struct PpsgcTpPortCounters));

	ppsgcTpPortCounters->tpCounters.rx_byte          =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.rx_byte);
	ppsgcTpPortCounters->tpCounters.rx_unicast_pkt   =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.rx_unicast_pkt);
	ppsgcTpPortCounters->tpCounters.rx_broadcast_pkt =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.rx_broadcast_pkt);
	ppsgcTpPortCounters->tpCounters.rx_multicast_pkt =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.rx_multicast_pkt);
	ppsgcTpPortCounters->tpCounters.rx_discard_pkt   =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.rx_discard_pkt);

	ppsgcTpPortCounters->tpCounters.tx_byte          =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.tx_byte);
	ppsgcTpPortCounters->tpCounters.tx_unicast_pkt   =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.tx_unicast_pkt);
	ppsgcTpPortCounters->tpCounters.tx_broadcast_pkt =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.tx_broadcast_pkt);
	ppsgcTpPortCounters->tpCounters.tx_multicast_pkt =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.tx_multicast_pkt);
	ppsgcTpPortCounters->tpCounters.tx_discard_pkt   =
		be64_to_cpu(repPsgcTpPortCounters.tpCounters.tx_discard_pkt);

	return ret;
}
EXPORT_SYMBOL(PPSGC_Db_GetTpPortCounters);


/**************************************************************************/
/*! \fn int PPSGC_Db_GetSfCounters(struct PpsgcSfCounters *ppsgcSfCounters)
 **************************************************************************
 *  \brief Get PP service flow counters according to interface name.
 *  \param[in,out] ppsgcSfCounters - PP service flow counters structure.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_GetSfCounters(struct PpsgcSfCounters *ppsgcSfCounters)
{
	int                     ret;
	struct PpsgcSfCounters  repPpsgcSfCounters;

	ret = sgc_mrpc_call(SGC_GET_SF_COUNTERS_PROC, ppsgcSfCounters,
			sizeof(struct PpsgcSfCounters),
			&repPpsgcSfCounters, sizeof(struct PpsgcSfCounters));

	ppsgcSfCounters->sfCounters.fwd_pkts =
		ntohl(repPpsgcSfCounters.sfCounters.fwd_pkts);
	ppsgcSfCounters->sfCounters.drp_cnt  =
		ntohl(repPpsgcSfCounters.sfCounters.drp_cnt);

	return ret;
}
EXPORT_SYMBOL(PPSGC_Db_GetSfCounters);


/**************************************************************************/
/*! \fn int PPSGC_Db_PrintTpPortCounters(void)
 **************************************************************************
 *  \brief Print packet processor transparent port counters.
 *  \param[in] no input.
 *  \param[out] no output.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_PrintTpPortCounters(void)
{
	return sgc_mrpc_call(SGC_PRINT_TPPORT_COUNTERS_PROC, NULL, 0, NULL, 0);
}
EXPORT_SYMBOL(PPSGC_Db_PrintTpPortCounters);


/**************************************************************************/
/*! \fn int PPSGC_Db_PrintIfCounters(void)
 **************************************************************************
 *  \brief Print packet processor interface device counters.
 *  \param[in] no input.
 *  \param[out] no output.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_PrintIfCounters(void)
{
	return sgc_mrpc_call(SGC_PRINT_IF_COUNTERS_PROC, NULL, 0, NULL, 0);
}
EXPORT_SYMBOL(PPSGC_Db_PrintIfCounters);


/**************************************************************************/
/*! \fn int PPSGC_Db_PrintGroupCounters(void)
 **************************************************************************
 *  \brief Print allocated group counters statistics.
 *  \param[in] no input.
 *  \param[out] no output.
 *  \return OK or error status.
 **************************************************************************/
int PPSGC_Db_PrintGroupCounters(void)
{
	return sgc_mrpc_call(SGC_PRINT_GROUP_COUNTERS_PROC, NULL, 0, NULL, 0);
}
EXPORT_SYMBOL(PPSGC_Db_PrintGroupCounters);


/**************************************************************************/
/*      LOCAL FUNCTIONS                                                   */
/**************************************************************************/

/**************************************************************************/
/*! \fn static inline int sgc_mrpc_call
 *       (__u8 procid, void *args, ssize_t arg_len, void *rep, ssize_t rep_len)
 **************************************************************************
 *  \brief
 *   args must be in network byte order!
 *  \param[in]
 *  \param[out] None
 *  \0 for success, error code otherwise.
 **************************************************************************/
static inline int sgc_mrpc_call
	(__u8 procid, void *args, ssize_t arg_len, void *rep, ssize_t rep_len)
{
	struct SgcClientPrivate *priv = this;
	int                     ret, errcode;

	if (!priv) {
		pr_err("ERROR: mrpc sgc not initialized");
		return PPSGC_RC_FAILURE;
	}

	ret = mrpc_call(priv->mrpc, procid, args, arg_len,
			rep, rep_len, 60000, &errcode);

	if (ret || errcode) {
		pr_err("ERROR: ret=%d, errcode=%d", ret, errcode);
		priv->callFailureCount++;
		return PPSGC_RC_FAILURE;
	}

	/* count the number of successful MRPC calls */
	priv->callSuccessCount++;
	return PPSGC_RC_SUCCESS;
}

/**************************************************************************/
/*! \fn static ssize_t status_show
*          (struct device *dev, struct device_attribute *attr, char *buf)
 **************************************************************************
 *  \brief sysfs for future use.
 *  \param[in]
 *  \param[out]
 *  \0 for success, error code otherwise.
 **************************************************************************/
static ssize_t status_show
		(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct SgcClientPrivate *priv = dev_get_drvdata(dev);

	if (!priv)
		return -EINVAL;

	return scnprintf(buf, PAGE_SIZE,
		"status ok.\nMRPC Success : %d\nMRPC Failure : %d\n",
		priv->callSuccessCount, priv->callFailureCount);
}

/**************************************************************************/
/*! \fn static int sgc_mrpc_probe(struct platform_device *pdev)
 **************************************************************************
 *  \brief
 *  \param[in] pdev - platform device.
 *  \param[out]
 *  \0 for success, error code otherwise.
 **************************************************************************/
static int sgc_mrpc_probe(struct platform_device *pdev)
{
	struct SgcClientPrivate *priv;
	int                     ret;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		pr_err("memory allocation failed");
		return -ENOMEM;
	}

	dev_set_drvdata(&pdev->dev, priv);
	priv->pdev = pdev;

	ret = sysfs_create_group(&priv->pdev->dev.kobj, &sgc_attrs_group);
	if (ret) {
		pr_err("sysfs_create_group failed (ret=%d)", ret);
		return ret;
	}

	priv->mrpc = mrpc_client_register
		(MRPC_RESERVED_ID_SGC, "Session Group Counter");
	if (!priv->mrpc) {
		pr_err("failed to register sgc");
		ret = -ENODEV;
		goto out_remove_group;
	}
	priv->callSuccessCount = 0;
	priv->callFailureCount = 0;
	this = priv;
	return 0;

out_remove_group:
	sysfs_remove_group(&priv->pdev->dev.kobj, &sgc_attrs_group);
	return 0;
}

/**************************************************************************/
/*! \fn static int sgc_mrpc_remove(struct platform_device *pdev)
 **************************************************************************
 *  \brief This function is called when the sgc mrpc driver is removed.
 *  \param[in] pdev - platform device
 *  \param[out] None
 *  \0 for success, error code otherwise.
 **************************************************************************/
static int sgc_mrpc_remove(struct platform_device *pdev)
{
	struct SgcClientPrivate *priv = platform_get_drvdata(pdev);

	mrpc_client_unregister(priv->mrpc);
	sysfs_remove_group(&priv->pdev->dev.kobj, &sgc_attrs_group);
	dev_set_drvdata(&pdev->dev, NULL);
	this = NULL;

	return 0;
}

/**************************************************************************/
/*! \fn static int __init sgc_mrpc_init(void)
 **************************************************************************
 *  \brief
 *  \param[in]
 *  \param[out] None
 *  \0 for success, error code otherwise.
 **************************************************************************/
static int __init sgc_mrpc_init(void)
{
	int ret;

	ret = platform_driver_register(&sgc_driver);
	if (ret < 0) {
		pr_err("Failed to register sgc platform driver: %d\n", ret);
		return ret;
	}

	sgc_device = platform_device_register_simple
					("sgc_mrpc_client", -1, NULL, 0);
	if (IS_ERR(sgc_device)) {
		pr_err("Failed to register sgc platform device\n");
		platform_driver_unregister(&sgc_driver);
		return PTR_ERR(sgc_device);
	}

	return 0;
}

/**************************************************************************/
/*! \fn static void __exit sgc_mrpc_exit(void)
 **************************************************************************
 *  \brief Exit function
 *  \param[in] None
 *  \param[out] None
 *  \0 for success, error code otherwise.
 **************************************************************************/
static void __exit sgc_mrpc_exit(void)
{
	platform_device_unregister(sgc_device);
	platform_driver_unregister(&sgc_driver);
}

module_init(sgc_mrpc_init);
module_exit(sgc_mrpc_exit);

MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION("PPSGC mrpc client");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

