/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See http://www.gnu.org/licenses/gpl-2.0.html for more details.
*/

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <net/netlink.h>
#include <linux/skbuff.h>
#include <net/genetlink.h>
#include "precomp.h"

#if CFG_CUSTOM_NETLINK_SUPPORT

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/
#define MAX_BIND_PROCESS           (1)
#define GL_CUSTOM_FAMILY_NAME      "WIFI_NL_CUSTOM"
#define MAX_CUSTOM_PKT_LENGTH      (2048)
enum {
	__GL_CUSTOM_ATTR_INVALID,
	GL_CUSTOM_ATTR_MSG,	/* message */
	__GL_CUSTOM_ATTR_MAX,
};
#define GL_CUSTOM_ATTR_MAX       (__GL_CUSTOM_ATTR_MAX - 1)

enum {
	__GL_CUSTOM_COMMAND_INVALID,
	GL_CUSTOM_COMMAND_BIND,	/* bind */
	GL_CUSTOM_COMMAND_SEND,	/* user -> kernel */
	GL_CUSTOM_COMMAND_RECV,	/* kernel -> user */
	__GL_CUSTOM_COMMAND_MAX,
};
#define GL_CUSTOM_COMMAND_MAX    (__GL_CUSTOM_COMMAND_MAX - 1)

/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/

/*******************************************************************************
*                   F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
static INT32 glCustomGenlBind(struct sk_buff *skb, struct genl_info *info);
static INT32 glCustomRecvFromUplayer(struct sk_buff *skb, struct genl_info *info);

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/
static struct genl_family glCustomGenlFamily = {
	.id = GENL_ID_GENERATE,
	.hdrsize = 0,
	.name = GL_CUSTOM_FAMILY_NAME,
	.version = 1,
	.maxattr = GL_CUSTOM_ATTR_MAX,
};

/* attribute policy */
static struct nla_policy glCustomGenlPolicy[GL_CUSTOM_ATTR_MAX + 1] = {
	[GL_CUSTOM_ATTR_MSG] = {.type = NLA_NUL_STRING},
};

/* operation definition */
static struct genl_ops glCustomGenlOpsArray[] = {
	{
		.cmd = GL_CUSTOM_COMMAND_BIND,
		.flags = 0,
		.policy = glCustomGenlPolicy,
		.doit = glCustomGenlBind,
		.dumpit = NULL,
	},
	{
		.cmd = GL_CUSTOM_COMMAND_SEND,
		.flags = 0,
		.policy = glCustomGenlPolicy,
		.doit = glCustomRecvFromUplayer,
		.dumpit = NULL,
	},
};

static INT32 gBindProcessNum = 0;
static pid_t gBindPid;

UINT_8 gScanIEBuf[MAX_IE_LENGTH];
UINT_32 gScanIELen = 0;

/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/
VOID glCustomGenlInit(VOID)
{
	if (genl_register_family_with_ops(&glCustomGenlFamily, glCustomGenlOpsArray) != 0)
		DBGLOG(INIT, ERROR, "%s(): GE_NELINK family registration fail\n", __func__);
}

VOID glCustomGenlDeinit(VOID)
{
	genl_unregister_family(&glCustomGenlFamily);
}

static INT32 glCustomGenlBind(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr *na;
	PINT8 pData = NULL;

	DBGLOG(INIT, ERROR, "%s(): ->\n", __func__);
	if (info == NULL)
		return -1;

	na = info->attrs[GL_CUSTOM_ATTR_MSG];
	if (na)
		pData = (PINT8) nla_data(na);

	if (strcmp(pData, "BIND") == 0) {
		if (gBindProcessNum < MAX_BIND_PROCESS) {
			gBindPid = info->snd_portid;
			gBindProcessNum++;
			DBGLOG(INIT, ERROR,"%s():-> pid  = %d\n", __func__, info->snd_portid);
		} else {
			gBindPid = info->snd_portid;
			DBGLOG(INIT, ERROR,"%s(): exceeding binding limit %d\n", __func__, MAX_BIND_PROCESS);
		}
	} else if (strcmp(pData, "UNBIND") == 0) {
		if (gBindProcessNum == 1) {
			gBindPid = 0;
			gBindProcessNum--;
			DBGLOG(INIT, ERROR,"%s():-> pid  = %d unbind\n", __func__, info->snd_portid);
		} else {
			DBGLOG(INIT, ERROR,"%s(): unbinding error %d\n", __func__, gBindProcessNum);
		}
	} else {
		DBGLOG(INIT, ERROR,"%s(): Unknown cmd %s\n", __func__, pData);
	}

	return 0;
}

static WLAN_STATUS glCustomProbeRespTxDone(IN P_ADAPTER_T prAdapter,
	IN P_MSDU_INFO_T prMsduInfo, IN ENUM_TX_RESULT_CODE_T rTxDoneStatus)
{
	do {
		ASSERT_BREAK((prAdapter != NULL) && (prMsduInfo != NULL));

		DBGLOG(TX, TRACE, "glCustomProbeRespTxDone: Status: %d, seq NO. %d\n",
			rTxDoneStatus, prMsduInfo->ucTxSeqNum);

	} while (FALSE);

	return WLAN_STATUS_SUCCESS;
}

static WLAN_STATUS glCustomSendProbeResponse(VOID *probe_resp, UINT_32 resp_len)
{
	P_ADAPTER_T prAdapter;
	P_MSDU_INFO_T prMsduInfo;
	P_GLUE_INFO_T prGlueInfo = NULL;

	if (!wlanExportGlueInfo(&prGlueInfo)) {
		DBGLOG(INIT, ERROR, "No glue info\n");
		return WLAN_STATUS_FAILURE;
	}

	prAdapter = prGlueInfo->prAdapter;

	/* 4 <1> Allocate a PKT_INFO_T for Probe Response Frame */
	/* Allocate a MSDU_INFO_T */
	prMsduInfo = cnmMgtPktAlloc(prAdapter, resp_len);
	if (prMsduInfo == NULL) {
		DBGLOG(BSS, WARN, "No PKT_INFO_T for sending Custom Frame\n");
		return WLAN_STATUS_RESOURCES;
	}

	/* 4 <2> Compose Probe Response frame header and fixed fields in MSDU_INfO_T. */
	/* Compose Header and Fixed Field */
	memcpy((PINT8)prMsduInfo->prPacket, (PINT8)probe_resp, resp_len);

	/* 4 <3> Update information of MSDU_INFO_T */
	prMsduInfo->eSrc = TX_PACKET_MGMT;
	prMsduInfo->ucPacketType = HIF_TX_PACKET_TYPE_MGMT;
	prMsduInfo->ucStaRecIndex = 0xFF;
	prMsduInfo->ucNetworkType = (UINT_8) 0;
	prMsduInfo->ucMacHeaderLength = WLAN_MAC_MGMT_HEADER_LEN;
	prMsduInfo->fgIs802_1x = FALSE;
	prMsduInfo->fgIs802_11 = TRUE;
	prMsduInfo->u2FrameLength = resp_len;
	prMsduInfo->ucTxSeqNum = nicIncreaseTxSeqNum(prAdapter);
	prMsduInfo->pfTxDoneHandler = glCustomProbeRespTxDone;
	prMsduInfo->fgIsBasicRate = TRUE;

	DBGLOG(TX, TRACE, "glCustomSendProbeResponse seq NO %d\n",
		prMsduInfo->ucTxSeqNum);

	/* TODO(Kevin): Also release the unused tail room of the composed MMPDU */

	/* 4 <6> Inform TXM  to send this Beacon /Probe Response frame. */
	nicTxEnqueueMsdu(prAdapter, prMsduInfo);

	return WLAN_STATUS_SUCCESS;
}

#if CFG_SUPPORT_TX_BEACON_STA_MODE
static WLAN_STATUS glCustomBeaconTxDone(IN P_ADAPTER_T prAdapter,
	IN P_MSDU_INFO_T prMsduInfo, IN ENUM_TX_RESULT_CODE_T rTxDoneStatus)
{
	do {
		ASSERT_BREAK((prAdapter != NULL) && (prMsduInfo != NULL));

		DBGLOG(TX, TRACE, "glCustomBeaconTxDone: Status: %d, seq NO. %d\n",
			rTxDoneStatus, prMsduInfo->ucTxSeqNum);

	} while (FALSE);

	return WLAN_STATUS_SUCCESS;
}

static WLAN_STATUS glCustomSendBeacon(VOID *probe_resp, UINT_32 resp_len)
{
	P_ADAPTER_T prAdapter;
	P_MSDU_INFO_T prMsduInfo;
	P_GLUE_INFO_T prGlueInfo = NULL;

	if (!wlanExportGlueInfo(&prGlueInfo)) {
		DBGLOG(INIT, ERROR, "No glue info\n");
		return WLAN_STATUS_FAILURE;
	}

	prAdapter = prGlueInfo->prAdapter;

	/* 4 <1> Allocate a PKT_INFO_T for Probe Response Frame */
	/* Allocate a MSDU_INFO_T */
	prMsduInfo = cnmMgtPktAlloc(prAdapter, resp_len);
	if (prMsduInfo == NULL) {
		DBGLOG(BSS, WARN, "No PKT_INFO_T for sending Custom Frame\n");
		return WLAN_STATUS_RESOURCES;
	}

	/* 4 <2> Compose Probe Response frame header and fixed fields in MSDU_INfO_T. */
	/* Compose Header and Fixed Field */
	memcpy((PINT8)prMsduInfo->prPacket, (PINT8)probe_resp, resp_len);

	/* 4 <3> Update information of MSDU_INFO_T */
	prMsduInfo->eSrc = TX_PACKET_MGMT;
	prMsduInfo->ucPacketType = HIF_TX_PACKET_TYPE_MGMT;
	prMsduInfo->ucStaRecIndex = 0xFF;
	prMsduInfo->ucNetworkType = (UINT_8) 0;
	prMsduInfo->ucMacHeaderLength = WLAN_MAC_MGMT_HEADER_LEN;
	prMsduInfo->fgIs802_1x = FALSE;
	prMsduInfo->fgIs802_11 = TRUE;
	prMsduInfo->u2FrameLength = resp_len;
	prMsduInfo->ucTxSeqNum = nicIncreaseTxSeqNum(prAdapter);
	prMsduInfo->pfTxDoneHandler = glCustomBeaconTxDone;
	prMsduInfo->fgIsBasicRate = TRUE;

	DBGLOG(TX, TRACE, "glCustomSendBeacon seq NO %d\n",
		prMsduInfo->ucTxSeqNum);

	/* TODO(Kevin): Also release the unused tail room of the composed MMPDU */

	/* 4 <6> Inform TXM  to send this Beacon /Probe Response frame. */
	nicTxEnqueueMsdu(prAdapter, prMsduInfo);

	return WLAN_STATUS_SUCCESS;
}
#endif

static INT32 glCustomRecvFromUplayer(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr *na;
	PINT8 pData = NULL;
	INT32 u4DataLen = 0;

	if (info == NULL)
		goto out;

	na = info->attrs[GL_CUSTOM_ATTR_MSG];

	if (na) {
		pData = (PINT8) nla_data(na);
		u4DataLen = nla_len(na);
	}

	DBGLOG(INIT, TRACE, "glCustomRecvFromUplayer len=%d, data[0]=0x%2x\n",
		u4DataLen, (UINT_8)pData[0]);

	if(*pData == 0x50) {
		glCustomSendProbeResponse(pData, u4DataLen);
	} else if(*pData == 0x40) {
		if(u4DataLen > MAX_IE_LENGTH)
			u4DataLen = MAX_IE_LENGTH;
		memcpy(gScanIEBuf, pData+4, u4DataLen-4);
		gScanIELen = u4DataLen-4;
	}

#if CFG_SUPPORT_TX_BEACON_STA_MODE
	if((UINT_8)pData[0] == 0x80) {
		glCustomSendBeacon(pData, u4DataLen);
	}
#endif

out:
	return 0;
}

INT32 glCustomSendToUplayer(PINT8 aucMsg, UINT32 u4MsgLen)
{
	struct sk_buff *skb = NULL;
	PVOID msg_head = NULL;
	INT32 rc = -1;

	if (gBindProcessNum == 0) {
		/* no listening process */
		DBGLOG(INIT, TRACE, "%s(): the process is not invoked\n", __func__);
		return -1;
	}

	if(aucMsg == NULL || u4MsgLen > MAX_CUSTOM_PKT_LENGTH) {
		DBGLOG(INIT, ERROR, "%s(): Input value is invalid\n", __func__);
		return -1;
	}

	skb = genlmsg_new(MAX_CUSTOM_PKT_LENGTH, GFP_KERNEL);

	if (skb) {
		msg_head = genlmsg_put(skb, 0, 0, &glCustomGenlFamily, 0, GL_CUSTOM_COMMAND_RECV);
		if (msg_head == NULL) {
			nlmsg_free(skb);
			DBGLOG(INIT, ERROR, "%s(): genlmsg_put fail...\n", __func__);
			return -1;
		}

		rc = nla_put(skb, GL_CUSTOM_ATTR_MSG, u4MsgLen, aucMsg);
		if (rc != 0) {
			nlmsg_free(skb);
			DBGLOG(INIT, ERROR, "%s(): nla_put_string fail...: %d\n", __func__, rc);
			return rc;
		}

		/* finalize the message */
		genlmsg_end(skb, msg_head);

		/* sending message */
		rc = genlmsg_unicast(&init_net, skb, gBindPid);
		if (rc != 0) {
			DBGLOG(INIT, ERROR, "%s(): genlmsg_unicast fail...: %d\n", __func__, rc);
			return rc;
		}
	} else {
		DBGLOG(INIT, ERROR, "%s(): genlmsg_new fail...\n", __func__);
		return -1;
	}

	return 0;
}
#endif /* CFG_CUSTOM_NETLINK_SUPPORT */
