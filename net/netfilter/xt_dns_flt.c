/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/checksum.h>
#include <linux/udp.h>
#include <linux/in.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_dns_flt.h>

MODULE_AUTHOR("Kevert < >");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("iptables dns filter module");
MODULE_ALIAS("ipt_dns_flt");
MODULE_ALIAS("ip6t_dns_flt");

#if 0
static int match(const struct sk_buff *skb,
      const struct net_device *in,
      const struct net_device *out,
      const struct xt_match *match,
      const void *matchinfo,
      int offset,
      unsigned int protoff,
      int *hotdrop)
#else
static bool match (const struct sk_buff *skb, const struct xt_action_param *par)
#endif
{
    struct iphdr *iph = (struct iphdr *)skb_network_header(skb);
    struct udphdr  stUdp, *pstUdpHdr = NULL;
    struct stDnsMsgHdr *pstDnsHdr = NULL;
	const  URL_STRING_ST *pstinfo = (URL_STRING_ST *)par->matchinfo;
    unsigned char *ptr = NULL;
    unsigned int i = 0;
    unsigned char ucStrLen = 0;
    unsigned int ulBuffLen = 0;
    unsigned char acBuff[URL_STRING_LEN] = {0};

/*注掉，使之也能匹配ipv6的dns查询包,cms加规则时已确保是dns包才走到这里*/
#if 0
    if ((NULL == iph) || (iph->protocol != IPPROTO_UDP))
    {
        return 0;
    }

#endif
	pstUdpHdr = skb_header_pointer(skb, par->thoff, sizeof(stUdp), &stUdp);
    if (NULL == pstUdpHdr)
    {
        return 0;
    }


    if (ntohs(pstUdpHdr->len) - sizeof(stUdp) <= sizeof(struct stDnsMsgHdr))
    {
        return 0;
    }

    ptr = (unsigned char *)(pstUdpHdr + 1);
    pstDnsHdr = (struct stDnsMsgHdr *)ptr;
    ptr += sizeof(struct stDnsMsgHdr);
    /* 只处理查询报文 */
    if ((ntohs(pstDnsHdr->u16Coms) >> 15) != URL_FLT_DNS_QUERY_CODE)
    {
        DEBUGPC("no query packet......");
        return 0;
    }

    DEBUGPC("begin to lookup %s", pstinfo->acURL);

    for (i = 0; (i < ntohs(pstDnsHdr->u16QDcnt)) && (ptr < skb->tail); i++)
    {
        DEBUGPC("url in packet: %s", acBuff);
        memset(acBuff, 0, URL_STRING_LEN);
        do
        {
            ucStrLen = *ptr;        /* 指向长度 */
            ptr++;                  /* 指向域名 */
            
            if (0 == ucStrLen)
            {
                ulBuffLen = strlen(acBuff);
                if (ulBuffLen)
                {
                    acBuff[ulBuffLen - 1] = '\0';
                }
                DEBUGPC("Get dns name: %s, begin to fund: %s", acBuff, pstinfo->acURL);
                if (strstr(acBuff, pstinfo->acURL))
                {
                    DEBUGPC("found the url: %s\r\n\r\n", pstinfo->acURL);
                    return (1 ^ pstinfo->u16Inv);
                }
                ptr += (2 * sizeof(uint16_t));    /* 指向下一个记录 */
                break;
            }
            
            if ((ptr + ucStrLen) > skb->tail)
            {
                DEBUGPC("Get dns name: %s, begin to fund: %s", acBuff, pstinfo->acURL);
                if (strstr(acBuff, pstinfo->acURL))
                {
                    return (1 ^ pstinfo->u16Inv);
                }
                return (0 | pstinfo->u16Inv);
            }

            /* 防止认为恶意构造DNS查询名字总长度超过了缓冲区可能导致问题 */
            if ((URL_STRING_LEN - 1) < (strlen(acBuff) + ucStrLen))
            {
                DEBUGPC("Get dns name: %s, begin to fund: %s", acBuff, pstinfo->acURL);
                if (strstr(acBuff, pstinfo->acURL))
                {
                    return (1 ^ pstinfo->u16Inv);
                }
                return (0 | pstinfo->u16Inv);
            }
            
            strncat(acBuff, ptr, ucStrLen);
            acBuff[strlen(acBuff)] = '.';
            ptr += ucStrLen;
            
        }while (1);
    }

    DEBUGPC("can not found the url: %s", pstinfo->acURL);

    return (0 | pstinfo->u16Inv);
}



static struct xt_match xt_dns_flt_match[] = {
	{
		.name		= "dns_flt",
        .revision   = 0,
		.family		= AF_INET,
		.match		= match,
		.matchsize	= sizeof(URL_STRING_ST),
		.hooks      = (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_IN) | (1 << NF_INET_FORWARD),
		.me		    = THIS_MODULE,
	},
	{
		.name		= "dns_flt",
        .revision   = 0,
		.family		= AF_INET6,
		.match		= match,
		.matchsize	= sizeof(URL_STRING_ST),
		.hooks      = (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_IN) | (1 << NF_INET_FORWARD),
		.me		    = THIS_MODULE,
	},
};

static int __init xt_dns_flt_init(void)
{
    DEBUGPC("register the dns_flt kernel module.");
	return xt_register_matches(xt_dns_flt_match, ARRAY_SIZE(xt_dns_flt_match));
}

static void __exit xt_dns_flt_fini(void)
{
	xt_unregister_matches(xt_dns_flt_match, ARRAY_SIZE(xt_dns_flt_match));
}

module_init(xt_dns_flt_init);
module_exit(xt_dns_flt_fini);


