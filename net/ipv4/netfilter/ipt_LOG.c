/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 * This is a module which is used for logging packets.
 */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <net/route.h>

#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ipt_LOG.h>
#include <net/netfilter/nf_log.h>
#ifdef CONFIG_FIREWALL_LOG
#include "atplogdef.h"
#include "msg/kcmsmonitormsgtypes.h"
#endif
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("Xtables: IPv4 packet logging to syslog");

/* Use lock to serialize, so printks don't overlap */
static DEFINE_SPINLOCK(log_lock);

/* One level of recursion won't kill us */
static void dump_packet(const struct nf_loginfo *info,
			const struct sk_buff *skb,
			unsigned int iphoff, const char* prefix)
{
	struct iphdr _iph;
	const struct iphdr *ih;
	unsigned int logflags;
#ifdef CONFIG_FIREWALL_LOG
	u_int8_t acLogBuf[ATP_LOG_SIZE];
	static u_int8_t acTmp[256];
	memset(acTmp, 0, sizeof(acTmp));
#endif
	
	if (info->type == NF_LOG_TYPE_LOG)
		logflags = info->u.log.logflags;
	else
		logflags = NF_LOG_MASK;

	ih = skb_header_pointer(skb, iphoff, sizeof(_iph), &_iph);
	if (ih == NULL) {
		printk("TRUNCATED");
		return;
	}
	
#ifdef CONFIG_FIREWALL_LOG
        memset(acLogBuf, 0,sizeof(acLogBuf));
#endif
	/* Important fields:
	 * TOS, len, DF/MF, fragment offset, TTL, src, dst, options. */
	/* Max length: 40 "SRC=255.255.255.255 DST=255.255.255.255 " */
	printk("SRC=%pI4 DST=%pI4 ",
	       &ih->saddr, &ih->daddr);
#ifdef CONFIG_FIREWALL_LOG
	if (NULL != prefix)
    {
		strcat(acLogBuf, prefix);
    }
    snprintf(acTmp, sizeof(acTmp), "SRC=%pI4 DST=%pI4 ", &ih->saddr, &ih->daddr);
	//strcat(acLogBuf, acTmp);
    snprintf(acLogBuf,sizeof(acLogBuf),"%s%s", acLogBuf, acTmp);

#endif
	/* Max length: 46 "LEN=65535 TOS=0xFF PREC=0xFF TTL=255 ID=65535 " */
	printk("LEN=%u TOS=0x%02X PREC=0x%02X TTL=%u ID=%u ",
	       ntohs(ih->tot_len), ih->tos & IPTOS_TOS_MASK,
	       ih->tos & IPTOS_PREC_MASK, ih->ttl, ntohs(ih->id));
#ifdef CONFIG_FIREWALL_LOG
         snprintf(acTmp, sizeof(acTmp), "LEN=%u TOS=0x%02X PREC=0x%02X TTL=%u ID=%u ",
	       ntohs(ih->tot_len), ih->tos & IPTOS_TOS_MASK,
	       ih->tos & IPTOS_PREC_MASK, ih->ttl, ntohs(ih->id));
	    snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);

#endif
	/* Max length: 6 "CE DF MF " */
	if (ntohs(ih->frag_off) & IP_CE){
		printk("CE ");		
#ifdef CONFIG_FIREWALL_LOG
		strcpy(acTmp, "CE");
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);

#endif
	}
	if (ntohs(ih->frag_off) & IP_DF){
		printk("DF ");
#ifdef CONFIG_FIREWALL_LOG
		strcpy(acTmp, "DF");
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);

#endif
	}
	if (ntohs(ih->frag_off) & IP_MF){
		printk("MF ");
#ifdef CONFIG_FIREWALL_LOG
		strcpy(acTmp, "MF");
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);

#endif
	}

	/* Max length: 11 "FRAG:65535 " */
	if (ntohs(ih->frag_off) & IP_OFFSET){
		printk("FRAG:%u ", ntohs(ih->frag_off) & IP_OFFSET);
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "FRAG:%u ", ntohs(ih->frag_off) & IP_OFFSET);
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
	}

	if ((logflags & IPT_LOG_IPOPT)
	    && ih->ihl * 4 > sizeof(struct iphdr)) {
		const unsigned char *op;
		unsigned char _opt[4 * 15 - sizeof(struct iphdr)];
		unsigned int i, optsize;

		optsize = ih->ihl * 4 - sizeof(struct iphdr);
		op = skb_header_pointer(skb, iphoff+sizeof(_iph),
					optsize, _opt);
		if (op == NULL) {
			printk("TRUNCATED");
#ifdef CONFIG_FIREWALL_LOG
			syswatch_sendLog(ATP_LOG_TYPE_FIREWALL, ATP_LOG_LEVEL_WARNING, 0, acLogBuf);
#endif
			return;
		}

		/* Max length: 127 "OPT (" 15*4*2chars ") " */
		printk("OPT (");
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "OPT ( ");
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);

#endif
		for (i = 0; i < optsize; i++){
			printk("%02X", op[i]);
#ifdef CONFIG_FIREWALL_LOG
			snprintf(acTmp+i*2, sizeof(acTmp), "%02X", op[i]);
#endif
		}
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);

#endif
		printk(") ");
#ifdef CONFIG_FIREWALL_LOG
		strcpy(acTmp, ")");
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);

#endif
	}

	switch (ih->protocol) {
	case IPPROTO_TCP: {
		struct tcphdr _tcph;
		const struct tcphdr *th;

		/* Max length: 10 "PROTO=TCP " */
		printk("PROTO=TCP ");
#ifdef CONFIG_FIREWALL_LOG
		strcpy(acTmp, "PROTO=TCP ");
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);

#endif

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		/* Max length: 25 "INCOMPLETE [65535 bytes] " */
		th = skb_header_pointer(skb, iphoff + ih->ihl * 4,
					sizeof(_tcph), &_tcph);
		if (th == NULL) {
			printk("INCOMPLETE [%u bytes] ",
			       skb->len - iphoff - ih->ihl*4);
#ifdef CONFIG_FIREWALL_LOG
			snprintf(acTmp, sizeof(acTmp),  "INCOMPLETE [%u bytes] ",
			       skb->len - iphoff - ih->ihl*4);
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
			break;
		}

		/* Max length: 20 "SPT=65535 DPT=65535 " */
		printk("SPT=%u DPT=%u ",
		       ntohs(th->source), ntohs(th->dest));
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "SPT=%u DPT=%u ",
		       ntohs(th->source), ntohs(th->dest));
		//strcat(acLogBuf, acTmp);
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
		/* Max length: 30 "SEQ=4294967295 ACK=4294967295 " */
		if (logflags & IPT_LOG_TCPSEQ){
			printk("SEQ=%u ACK=%u ",
			       ntohl(th->seq), ntohl(th->ack_seq));
#ifdef CONFIG_FIREWALL_LOG
			snprintf(acTmp, sizeof(acTmp), "SEQ=%u ACK=%u ",
			       ntohl(th->seq), ntohl(th->ack_seq));
		    snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
	         }
		/* Max length: 13 "WINDOW=65535 " */
		printk("WINDOW=%u ", ntohs(th->window));
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp),"WINDOW=%u ", ntohs(th->window));
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
		/* Max length: 9 "RES=0x3F " */
		printk("RES=0x%02x ", (u8)(ntohl(tcp_flag_word(th) & TCP_RESERVED_BITS) >> 22));
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp),"RES=0x%02x ", (u8)(ntohl(tcp_flag_word(th) & TCP_RESERVED_BITS) >> 22));
		//strcat(acLogBuf, acTmp);
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
		/* Max length: 32 "CWR ECE URG ACK PSH RST SYN FIN " */
		if (th->cwr){
			printk("CWR ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "CWR");
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, "CWR");
#endif
		}
		if (th->ece){
			printk("ECE ");
#ifdef CONFIG_FIREWALL_LOG            
			//strcat(acLogBuf, "ECE");
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "ECE");
#endif
		}
		if (th->urg){
			printk("URG ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "URG");
snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "URG");
#endif
		}
		if (th->ack){
			printk("ACK ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "ACK");
snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "ACK");

#endif
		}
		if (th->psh){
			printk("PSH ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "PSH");
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "PSH");

#endif
		}
		if (th->rst){
			printk("RST ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "RST");
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "RST");
#endif
		}
		if (th->syn){
			printk("SYN ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "SYN");
						snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "SYN");

#endif
		}
		if (th->fin){
			printk("FIN ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "FIN");
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "FIN");

#endif
		}
		/* Max length: 11 "URGP=65535 " */
		printk("URGP=%u ", ntohs(th->urg_ptr));
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "URGP=%u ", ntohs(th->urg_ptr));
		//strcat(acLogBuf, acTmp);
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "FIN");
#endif

		if ((logflags & IPT_LOG_TCPOPT)
		    && th->doff * 4 > sizeof(struct tcphdr)) {
			unsigned char _opt[4 * 15 - sizeof(struct tcphdr)];
			const unsigned char *op;
			unsigned int i, optsize;

			optsize = th->doff * 4 - sizeof(struct tcphdr);
			op = skb_header_pointer(skb,
						iphoff+ih->ihl*4+sizeof(_tcph),
						optsize, _opt);
			if (op == NULL) {
				printk("TRUNCATED");
#ifdef CONFIG_FIREWALL_LOG
				syswatch_sendLog(ATP_LOG_TYPE_FIREWALL, ATP_LOG_LEVEL_WARNING, 0, acLogBuf);
#endif
				return;
			}

			/* Max length: 127 "OPT (" 15*4*2chars ") " */
			printk("OPT (");
#ifdef CONFIG_FIREWALL_LOG
			strcat(acLogBuf, "OPT (");
#endif
			for (i = 0; i < optsize; i++){
				printk("%02X", op[i]);
#ifdef CONFIG_FIREWALL_LOG
				snprintf(acTmp+i*2, sizeof(acTmp), "%02X", op[i]);
#endif
			}
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, acTmp);
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  acTmp);
#endif
			printk(") ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, ") ");
                snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  ")");
#endif
		}
		break;
	}
	case IPPROTO_UDP:
	case IPPROTO_UDPLITE: {
		struct udphdr _udph;
		const struct udphdr *uh;

		if (ih->protocol == IPPROTO_UDP){
			/* Max length: 10 "PROTO=UDP "     */
			printk("PROTO=UDP " );
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "PROTO=UDP ");
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "PROTO=UDP ");
#endif
		}
		else	/* Max length: 14 "PROTO=UDPLITE " */
		{
			printk("PROTO=UDPLITE ");
#ifdef CONFIG_FIREWALL_LOG
			//strcat(acLogBuf, "PROTO=UDPLITE ");
			snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "PROTO=UDPLITE ");
#endif
		}

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		/* Max length: 25 "INCOMPLETE [65535 bytes] " */
		uh = skb_header_pointer(skb, iphoff+ih->ihl*4,
					sizeof(_udph), &_udph);
		if (uh == NULL) {
			printk("INCOMPLETE [%u bytes] ",
			       skb->len - iphoff - ih->ihl*4);
			break;
		}

		/* Max length: 20 "SPT=65535 DPT=65535 " */
		printk("SPT=%u DPT=%u LEN=%u ",
		       ntohs(uh->source), ntohs(uh->dest),
		       ntohs(uh->len));
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "SPT=%u DPT=%u LEN=%u ",
		       ntohs(uh->source), ntohs(uh->dest),
		       ntohs(uh->len));
		//strcat(acLogBuf, acTmp);
            snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  acTmp);
#endif
		break;
	}
	case IPPROTO_ICMP: {
		struct icmphdr _icmph;
		const struct icmphdr *ich;
		static const size_t required_len[NR_ICMP_TYPES+1]
			= { [ICMP_ECHOREPLY] = 4,
			    [ICMP_DEST_UNREACH]
			    = 8 + sizeof(struct iphdr),
			    [ICMP_SOURCE_QUENCH]
			    = 8 + sizeof(struct iphdr),
			    [ICMP_REDIRECT]
			    = 8 + sizeof(struct iphdr),
			    [ICMP_ECHO] = 4,
			    [ICMP_TIME_EXCEEDED]
			    = 8 + sizeof(struct iphdr),
			    [ICMP_PARAMETERPROB]
			    = 8 + sizeof(struct iphdr),
			    [ICMP_TIMESTAMP] = 20,
			    [ICMP_TIMESTAMPREPLY] = 20,
			    [ICMP_ADDRESS] = 12,
			    [ICMP_ADDRESSREPLY] = 12 };

		/* Max length: 11 "PROTO=ICMP " */
		printk("PROTO=ICMP ");
#ifdef CONFIG_FIREWALL_LOG
		//strcat(acLogBuf, "PROTO=ICMP ");
            snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "PROTO=ICMP");
#endif

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		/* Max length: 25 "INCOMPLETE [65535 bytes] " */
		ich = skb_header_pointer(skb, iphoff + ih->ihl * 4,
					 sizeof(_icmph), &_icmph);
		if (ich == NULL) {
			printk("INCOMPLETE [%u bytes] ",
			       skb->len - iphoff - ih->ihl*4);
			break;
		}

		/* Max length: 18 "TYPE=255 CODE=255 " */
		printk("TYPE=%u CODE=%u ", ich->type, ich->code);
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "TYPE=%u CODE=%u ", ich->type, ich->code);
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
        
#endif

		/* Max length: 25 "INCOMPLETE [65535 bytes] " */
		if (ich->type <= NR_ICMP_TYPES
		    && required_len[ich->type]
		    && skb->len-iphoff-ih->ihl*4 < required_len[ich->type]) {
			printk("INCOMPLETE [%u bytes] ",
			       skb->len - iphoff - ih->ihl*4);
			break;
		}

		switch (ich->type) {
		case ICMP_ECHOREPLY:
		case ICMP_ECHO:
			/* Max length: 19 "ID=65535 SEQ=65535 " */
			printk("ID=%u SEQ=%u ",
			       ntohs(ich->un.echo.id),
			       ntohs(ich->un.echo.sequence));
#ifdef CONFIG_FIREWALL_LOG
			snprintf(acTmp, sizeof(acTmp), "ID=%u SEQ=%u ", ntohs(ich->un.echo.id),
			       ntohs(ich->un.echo.sequence));
		    snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
			break;

		case ICMP_PARAMETERPROB:
			/* Max length: 14 "PARAMETER=255 " */
			printk("PARAMETER=%u ",
			       ntohl(ich->un.gateway) >> 24);
#ifdef CONFIG_FIREWALL_LOG
			snprintf(acTmp, sizeof(acTmp), "PARAMETER=%u ",
			       ntohl(ich->un.gateway) >> 24);
		    snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
			break;
		case ICMP_REDIRECT:
			/* Max length: 24 "GATEWAY=255.255.255.255 " */
			printk("GATEWAY=%pI4 ", &ich->un.gateway);
			/* Fall through */
		case ICMP_DEST_UNREACH:
		case ICMP_SOURCE_QUENCH:
		case ICMP_TIME_EXCEEDED:
			/* Max length: 3+maxlen */
			if (!iphoff) { /* Only recurse once. */
				printk("[");
				dump_packet(info, skb,
					    iphoff + ih->ihl*4+sizeof(_icmph), NULL);
				printk("] ");
			}

			/* Max length: 10 "MTU=65535 " */
			if (ich->type == ICMP_DEST_UNREACH
			    && ich->code == ICMP_FRAG_NEEDED){
				printk("MTU=%u ", ntohs(ich->un.frag.mtu));
#ifdef CONFIG_FIREWALL_LOG
				snprintf(acTmp, sizeof(acTmp), "MTU=%u ", ntohs(ich->un.frag.mtu));
		        snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
			}
		}
		break;
	}
	/* Max Length */
	case IPPROTO_AH: {
		struct ip_auth_hdr _ahdr;
		const struct ip_auth_hdr *ah;

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		/* Max length: 9 "PROTO=AH " */
		printk("PROTO=AH ");
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, "PROTO=AH ");
#endif

		/* Max length: 25 "INCOMPLETE [65535 bytes] " */
		ah = skb_header_pointer(skb, iphoff+ih->ihl*4,
					sizeof(_ahdr), &_ahdr);
		if (ah == NULL) {
			printk("INCOMPLETE [%u bytes] ",
			       skb->len - iphoff - ih->ihl*4);
			break;
		}

		/* Length: 15 "SPI=0xF1234567 " */
		printk("SPI=0x%x ", ntohl(ah->spi));
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "SPI=0x%x ", ntohl(ah->spi));
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
		break;
	}
	case IPPROTO_ESP: {
		struct ip_esp_hdr _esph;
		const struct ip_esp_hdr *eh;

		/* Max length: 10 "PROTO=ESP " */
		printk("PROTO=ESP ");
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf,  "PROTO=ESP ");
#endif

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		/* Max length: 25 "INCOMPLETE [65535 bytes] " */
		eh = skb_header_pointer(skb, iphoff+ih->ihl*4,
					sizeof(_esph), &_esph);
		if (eh == NULL) {
			printk("INCOMPLETE [%u bytes] ",
			       skb->len - iphoff - ih->ihl*4);
			break;
		}

		/* Length: 15 "SPI=0xF1234567 " */
		printk("SPI=0x%x ", ntohl(eh->spi));
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "SPI=0x%x ", ntohl(eh->spi));
		//strcat(acLogBuf, acTmp);
        snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
		break;
	}
	/* Max length: 10 "PROTO 255 " */
	default:{
		printk("PROTO=%u ", ih->protocol);
#ifdef CONFIG_FIREWALL_LOG
		snprintf(acTmp, sizeof(acTmp), "PROTO=%u ", ih->protocol);
		//strcat(acLogBuf, acTmp);
		snprintf(acLogBuf,sizeof(acLogBuf), "%s%s", acLogBuf, acTmp);
#endif
	}
		
	}

	/* Max length: 15 "UID=4294967295 " */
	if ((logflags & IPT_LOG_UID) && !iphoff && skb->sk) {
		read_lock_bh(&skb->sk->sk_callback_lock);
		if (skb->sk->sk_socket && skb->sk->sk_socket->file)
			printk("UID=%u GID=%u ",
				skb->sk->sk_socket->file->f_cred->fsuid,
				skb->sk->sk_socket->file->f_cred->fsgid);
		read_unlock_bh(&skb->sk->sk_callback_lock);
	}
	
#ifdef CONFIG_FIREWALL_LOG
        syswatch_sendLog(ATP_LOG_TYPE_FIREWALL, ATP_LOG_LEVEL_WARNING, 0, acLogBuf);
#endif
	/* Max length: 16 "MARK=0xFFFFFFFF " */
	if (!iphoff && skb->mark)
		printk("MARK=0x%x ", skb->mark);

	/* Proto    Max log string length */
	/* IP:      40+46+6+11+127 = 230 */
	/* TCP:     10+max(25,20+30+13+9+32+11+127) = 252 */
	/* UDP:     10+max(25,20) = 35 */
	/* UDPLITE: 14+max(25,20) = 39 */
	/* ICMP:    11+max(25, 18+25+max(19,14,24+3+n+10,3+n+10)) = 91+n */
	/* ESP:     10+max(25)+15 = 50 */
	/* AH:      9+max(25)+15 = 49 */
	/* unknown: 10 */

	/* (ICMP allows recursion one level deep) */
	/* maxlen =  IP + ICMP +  IP + max(TCP,UDP,ICMP,unknown) */
	/* maxlen = 230+   91  + 230 + 252 = 803 */
}

static struct nf_loginfo default_loginfo = {
	.type	= NF_LOG_TYPE_LOG,
	.u = {
		.log = {
			.level    = 0,
			.logflags = NF_LOG_MASK,
		},
	},
};

static void
ipt_log_packet(u_int8_t pf,
	       unsigned int hooknum,
	       const struct sk_buff *skb,
	       const struct net_device *in,
	       const struct net_device *out,
	       const struct nf_loginfo *loginfo,
	       const char *prefix)
{
    /*Start for 限制内核打印 */
    if(!net_ratelimit())
    {
        return;
    }
	/*End for 限制内核打印 */
    
	if (!loginfo)
		loginfo = &default_loginfo;

	spin_lock_bh(&log_lock);
	printk("<%d>%sIN=%s OUT=%s ", loginfo->u.log.level,
	       prefix,
	       in ? in->name : "",
	       out ? out->name : "");
#ifdef CONFIG_BRIDGE_NETFILTER
	if (skb->nf_bridge) {
		const struct net_device *physindev;
		const struct net_device *physoutdev;

		physindev = skb->nf_bridge->physindev;
		if (physindev && in != physindev)
			printk("PHYSIN=%s ", physindev->name);
		physoutdev = skb->nf_bridge->physoutdev;
		if (physoutdev && out != physoutdev)
			printk("PHYSOUT=%s ", physoutdev->name);
	}
#endif

	if (in && !out) {
		/* MAC logging for input chain only. */
		printk("MAC=");
		if (skb->dev && skb->dev->hard_header_len
		    && skb->mac_header != skb->network_header) {
			int i;
			const unsigned char *p = skb_mac_header(skb);
			for (i = 0; i < skb->dev->hard_header_len; i++,p++)
				printk("%02x%c", *p,
				       i==skb->dev->hard_header_len - 1
				       ? ' ':':');
		} else
			printk(" ");
	}

	dump_packet(loginfo, skb, 0, prefix);
	printk("\n");
	spin_unlock_bh(&log_lock);
}

static unsigned int
log_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct ipt_log_info *loginfo = par->targinfo;
	struct nf_loginfo li;

	li.type = NF_LOG_TYPE_LOG;
	li.u.log.level = loginfo->level;
	li.u.log.logflags = loginfo->logflags;

	ipt_log_packet(NFPROTO_IPV4, par->hooknum, skb, par->in, par->out, &li,
		       loginfo->prefix);
	return XT_CONTINUE;
}

static int log_tg_check(const struct xt_tgchk_param *par)
{
	const struct ipt_log_info *loginfo = par->targinfo;

	if (loginfo->level >= 8) {
		pr_debug("LOG: level %u >= 8\n", loginfo->level);
		return -EINVAL;
	}
	if (loginfo->prefix[sizeof(loginfo->prefix)-1] != '\0') {
		pr_debug("LOG: prefix term %i\n",
			 loginfo->prefix[sizeof(loginfo->prefix)-1]);
		return -EINVAL;
	}
	return 0;
}

static struct xt_target log_tg_reg __read_mostly = {
	.name		= "LOG",
	.family		= NFPROTO_IPV4,
	.target		= log_tg,
	.targetsize	= sizeof(struct ipt_log_info),
	.checkentry	= log_tg_check,
	.me		= THIS_MODULE,
};

static struct nf_logger ipt_log_logger __read_mostly = {
	.name		= "ipt_LOG",
	.logfn		= &ipt_log_packet,
	.me		= THIS_MODULE,
};

static int __init log_tg_init(void)
{
	int ret;

	ret = xt_register_target(&log_tg_reg);
	if (ret < 0)
		return ret;
	nf_log_register(NFPROTO_IPV4, &ipt_log_logger);
	return 0;
}

static void __exit log_tg_exit(void)
{
	nf_log_unregister(&ipt_log_logger);
	xt_unregister_target(&log_tg_reg);
}

module_init(log_tg_init);
module_exit(log_tg_exit);
