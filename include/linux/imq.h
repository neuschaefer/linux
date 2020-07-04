/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef _IMQ_H
#define _IMQ_H

#define IMQ_MAX_DEVS     16
/*Start of modified for qos function 2012-1-6 */
#define QOS_DEFAULT_MARK  0x100000
/*End of modified for qos function 2012-1-6 */

/* Added for downlink rate control 2012-9-8 */
#ifdef CONFIG_DOWN_RATE_CONTROL
	#define QOS_DOWNDEFAULT_MARK  0x400000
	#define IS_ETN_LAN(name)  (('e' == name[0]) || ('w' == name[0]))
	#define IS_DOWN_WIFI(name)  ('w' == name[0])
#endif

#define QOS_DEV_IMQ4      0x4
#define IS_DOWN_WAN(name)  ('e' == name[0])
/* End for downlink rate control 2012-9-8 */

#define QOS_DEV_IMQ0      0x0
#define QOS_DEV_IMQ1      0x1
#define QOS_DEV_IMQ2      0x2
#define QOS_DEV_IMQ3      0x3
#define QOS_DEV_IMQ5      0x5
/*Start add: keep the kernel and cms of qos consistent20110629*/
#define QOS_HIGHEST_PRI                  0x100
#define QOS_HIGH_PRI                     0x200
#define QOS_DEFAULT_PRI                  0x000
#define QOS_PRI_MASK                     0x00000F00
/*end add: keep the kernel and cms of qos consistent20110629*/
/*End of modified for qos function 2012-1-6 */
#define IS_ETN_WAN(name)  (('n' == name[0]) && ('0' == name[3]))

/* IFMASK (16 device indexes, 0 to 15) and flag(s) fit in 5 bits */
#define IMQ_F_BITS	5

#define IMQ_F_IFMASK	0x0f
#define IMQ_F_ENQUEUE	0x10
extern int qos_enable;
extern int downqos_enable;
extern int smartqos_enable;
extern int imq_nf_queue(struct sk_buff *skb);
//#define IMQ_MAX_DEVS	(IMQ_F_IFMASK + 1)

#endif /* _IMQ_H */

