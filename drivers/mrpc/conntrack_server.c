/*
 *
 * mrpc_conntrack_server.c
 * Description:
 * Connection tracking over MRPC - ATOM server module.
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

#define pr_fmt(fmt) KBUILD_MODNAME ":%s:%d " fmt "\n", __func__, __LINE__

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
#include <linux/mrpc/conntrack_server.h>
#include <linux/mrpc/hil_cmds.h>
#include <linux/ti_hil.h>
/*conntrack headers */
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_seqadj.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_acct.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_zones.h>
#include <net/netfilter/nf_conntrack_tuple.h>
#include <net/netfilter/nf_conntrack_timestamp.h>
#include <net/netfilter/nf_conntrack_timeout.h>
#include <net/netfilter/nf_conntrack_labels.h>
#include <net/netfilter/nf_conntrack_synproxy.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_helper.h>

#ifdef DEBUG
#define DBG(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#else
#define DBG(fmt, ...) pr_debug(fmt, ##__VA_ARGS__)
#endif
/* driver private database */
struct ct_server{
      struct mrpc_server *mrpc;       /* mrpc server handle */
      struct ct_pp_funcs *pp_dev_callbacks; /* pp_dev module callback register */
      struct hil_funcs *pp_hil_callbacks; /*pp_hil module callback register */
};

/*Tuple information of the timed out connection - received from ATOM ct_client*/
struct mrpc_conntrack_tuple_info{
        u32     atomTag;
        struct nf_conntrack_tuple tuple;
};
 
/*reply message from ct server. This will be used to extend the CT timer - reply to ATOM ct_client */
struct mrpc_conntrack_arm_reply{
        int  extend_timeout;
};

#define PP_PROC(proc, func)                                  \
[proc] = {                                                   \
        .procid   = proc,                                    \
        .p_func	  = func,                                    \
        .p_arglen = sizeof(struct mrpc_conntrack_tuple_info),\
        .p_replen = sizeof(struct mrpc_conntrack_arm_reply), \
        .p_name   = #func,                                   \
}

/*CT PROC id*/
enum {
    CT_DEATH_BY_TIMEOUT_PROC = 0,
    NPCPU_CONNTRACK_FLUSH_PROC = 1,
};

static int mrpc_ct_death_by_timeout (void *args, ssize_t arglen, void *rep, ssize_t replen);
static int mrpc_npcpu_conntrack_flush (void *args, ssize_t arglen, void *rep, ssize_t replen);

static struct mrpc_procinfo ct_server_procs[] = {
    PP_PROC(CT_DEATH_BY_TIMEOUT_PROC, mrpc_ct_death_by_timeout),
    PP_PROC(NPCPU_CONNTRACK_FLUSH_PROC, mrpc_npcpu_conntrack_flush)
};

/*conntrack MRPC server module - mrpc handler*/
struct ct_server *ct_module;

/*
 * Name: mrpc_ct_death_by_timeout
 * Desc: mrpc_ct_death_by_timeout is the MRPC srever procedure invoked when there is a 
 *       connection tracking entry timeout happens at ATOM. It does,
 *       1: Parse the received tuple information and construct the local tuple record at ARM.
 *       2: Re-generate HASH index for the received tuple information.
 *       3: Look for corresponding CT record at ARM. If found, reply to ct_client module at ATOM
 *          to extend the timeout as there is a possible PP session available for this CT entry.
 * Input: args - tuple information from ATOM.
 *        arglen - tuple size.
 *        rep - reply message to ATOM.
 *        replen - reply message size.
 * Output: Return 0 to MRPC driver.
 *
 * CAUTION:conntrack tuple information is network byte ordered(_be32, _be16 types) and the HASH 
 *         index is genreated while keeping tuple information(ip and port info) in network byte 
 *         order at the nf_conntrack module. ntohl/s on the received IP and PORT number will impact
 *         the HASH index generation at ARM.
*/
static int mrpc_ct_death_by_timeout (void *args, ssize_t arglen, void *rep, ssize_t replen)
{
  struct mrpc_conntrack_tuple_info *ct_tuple_info = args; 
  struct mrpc_conntrack_arm_reply *reply_atom = rep;
  struct nf_conntrack_tuple tuple;
  struct nf_conntrack_tuple_hash *hash;
  struct nf_conn *ct;
  struct net_device *dev;
  struct net *netns;
  int  atomTag, hash_found=0;
  int  ip_mode = ntohs(ct_tuple_info->tuple.src.l3num);
  int  protoNum = ct_tuple_info->tuple.dst.protonum;

  atomTag = ntohl(ct_tuple_info->atomTag);

  memset(&tuple,0,sizeof(struct nf_conntrack_tuple));
  
  tuple.dst.protonum = ct_tuple_info->tuple.dst.protonum;

  /*Check the address family - IPV4 or IPV6*/
  if (ip_mode == AF_INET)
  {
     memcpy(tuple.src.u3.all, ct_tuple_info->tuple.src.u3.all,
                                      sizeof(struct in_addr));
     memcpy(tuple.dst.u3.all, ct_tuple_info->tuple.dst.u3.all,
                                      sizeof(struct in_addr));
  }
  else
  {
     memcpy(tuple.src.u3.all, ct_tuple_info->tuple.src.u3.all,
                                      sizeof(struct in6_addr));
     memcpy(tuple.dst.u3.all, ct_tuple_info->tuple.dst.u3.all,
                                      sizeof(struct in6_addr));
  }

  /* extract protocol specific PORT number */
  switch(protoNum) {
    case IPPROTO_TCP:
         tuple.src.u.tcp.port = ct_tuple_info->tuple.src.u.tcp.port;
         tuple.dst.u.tcp.port = ct_tuple_info->tuple.dst.u.tcp.port;
    break;
    case IPPROTO_UDP:
         tuple.src.u.udp.port = ct_tuple_info->tuple.src.u.udp.port;
         tuple.dst.u.udp.port = ct_tuple_info->tuple.dst.u.udp.port;
    break;
    case IPPROTO_DCCP:
         tuple.src.u.dccp.port = ct_tuple_info->tuple.src.u.dccp.port;
         tuple.dst.u.dccp.port = ct_tuple_info->tuple.dst.u.dccp.port;
    break;
    case IPPROTO_SCTP:
         tuple.src.u.sctp.port = ct_tuple_info->tuple.src.u.sctp.port;
         tuple.dst.u.sctp.port = ct_tuple_info->tuple.dst.u.sctp.port;
    break;
    case IPPROTO_GRE:
         tuple.src.u.gre.key = ct_tuple_info->tuple.src.u.gre.key;
         tuple.dst.u.gre.key = ct_tuple_info->tuple.dst.u.gre.key;
    break;
    default:
         DBG("Protocol %d not supported by mrpc_conntrack_server.\n",protoNum);
  }

  tuple.src.l3num = ntohs(ct_tuple_info->tuple.src.l3num);

  DBG("CT: %d :proto %d : TUPLE INFO: %pI4:%u->%pI4:%u/%u \n",
                                           ct_tuple_info->atomTag,
                                           ct_tuple_info->tuple.src.l3num,
                                           &ct_tuple_info->tuple.src.u3.ip,
                                           ntohs(ct_tuple_info->tuple.src.u.all),
                                           &ct_tuple_info->tuple.dst.u3.ip,
                                           ntohs(ct_tuple_info->tuple.dst.u.all),
                                           ct_tuple_info->tuple.dst.protonum);

  DBG("TAG: %d :proto %d : TUPLE INFO: %pI4:%u->%pI4:%u/%u \n",atomTag,
                                                                   tuple.src.l3num,
                                                                   &tuple.src.u3.ip, 
                                                                   ntohs(tuple.src.u.all),
                                                                   &tuple.dst.u3.ip, 
                                                                   ntohs(tuple.dst.u.all),
                                                                   tuple.dst.protonum);
  read_lock(&dev_base_lock);
  for_each_netdev(&init_net, dev){
    netns = dev_net(dev); /* get the network namespace info */
    hash = nf_conntrack_find_get(netns, NF_CT_DEFAULT_ZONE, &tuple);
    if(hash){
      DBG("Dev %7s: hash FOUND for the given tuple.",dev->name); 
      hash_found = 1;
      break;
    }
  }
  read_unlock(&dev_base_lock);
  /*default reply is 0. delete the CT record on ATOM*/
  reply_atom->extend_timeout = 0;
  if(hash_found){
    ct = nf_ct_tuplehash_to_ctrack(hash);
    if(ct) { /*ARM conntrack record found for the tuple info received from ATOM*/
      /*Generated ATOM specific event to check for the PP accleration*/
      ti_hil_pp_event (CT_APPCPU_DEATH_BY_TIMEOUT, (void *)ct);
      if((ct->pp_appcpu_status_flag & TI_PP_KILL_CONNTRACK) == 0){
        DBG("PP session exist. reply ATOM to extend timeout.\n"); 
        reply_atom->extend_timeout = 1;
      }
      /*decrement conntrack reference count*/
      nf_ct_put(ct);
    }
    else{
      DBG("Conntrack record NOT FOUND for the generated HASH. KILL CT at ATOM.\n"); 
    }
  } 
  else{
    DBG("conntrack HASH NOT FOUND. reply ATOM to KILL CT session.\n"); 
  }
  return 0;
}

static int mrpc_npcpu_conntrack_flush (void *args, ssize_t arglen, void *rep, ssize_t replen)
{
  struct mrpc_conntrack_tuple_info *ct_tuple_info = args;
  struct net_device *dev;
  struct net *netns;
  printk(KERN_NOTICE "mrpc_npcpu_conntrack_flush invoked from ATOM\n");
  dev = first_net_device(&init_net); /* get the device info */
  netns = dev_net(dev); /* get the network namespace info */
  printk(KERN_NOTICE "conntrack_flush : disable PP");
  ct_module->pp_hil_callbacks->pp_hil_set_disable_state(True);
  printk(KERN_NOTICE "conntrack_flush : Flush PP sessions");
  ct_module->pp_dev_callbacks->avalanche_pp_flush_sessions(AVALANCHE_PP_MAX_VPID, 4);
  printk(KERN_NOTICE "conntrack_flush : Flush CT");
  nf_conntrack_flush_report(netns, 0, 0); /*flush the connection tracking records - requested by APPCPU */
  printk(KERN_NOTICE "conntrack_flush : enable PP");
  ct_module->pp_hil_callbacks->pp_hil_set_disable_state(False);
  printk(KERN_NOTICE "mrpc_npcpu_conntrack_flush complete.\n"); 
  return 0;
}

/**
 * mrpc_conntrack_server_unregister
 * 
 * unregister pp callbacks
 *
 * @return 0 for success, error code otherwise
 */
void mrpc_conntrack_server_unregister(int module)
{
    BUG_ON(!ct_module);
    /*mrpc callbacks unregistered by pp_drv module */
    if (module == PP_DEV)
    {
       BUG_ON(!ct_module->pp_dev_callbacks);
       kfree(ct_module->pp_dev_callbacks);
       ct_module->pp_dev_callbacks = NULL;
       pr_info("PP_DEV: callback functions unregistered\n");
    }
    /*mrpc callbacks unregistered by hil_drv module */
    else if (module == PP_HIL)
    {
       BUG_ON(!ct_module->pp_hil_callbacks);
       kfree(ct_module->pp_hil_callbacks);
       ct_module->pp_hil_callbacks = NULL;
       pr_info("PP_HIL: callback functions unregistered\n");
    }
}
EXPORT_SYMBOL(mrpc_conntrack_server_unregister);

/**
 * mrpc_conntrack_server_register
 *
 * register pp and hil callbacks
 *
 * @param pp_funcs   callback functions
 *
 * @return 0 for success, error code otherwise
 */
int mrpc_conntrack_server_register(void *callbacks, int module)
{
    BUG_ON(!ct_module);
    BUG_ON(!callbacks);

    /*mrpc callbacks registered by pp_dev module */ 
    if (module == PP_DEV)
    {
       BUG_ON(ct_module->pp_dev_callbacks);
       ct_module->pp_dev_callbacks = kzalloc(sizeof(struct ct_pp_funcs), GFP_KERNEL);
       if (!ct_module->pp_dev_callbacks) {
           pr_err("PP_DEV: Failed to allocate memory");
           return -ENOMEM;
       }
       memcpy(ct_module->pp_dev_callbacks,callbacks, sizeof(struct ct_pp_funcs));
       pr_info("PP_DEV: conntrack server callback functions registered");
    }
    /*mrpc callbacks registered by hil_dev module */
    else if (module == PP_HIL)
    {
       BUG_ON(ct_module->pp_hil_callbacks);
       ct_module->pp_hil_callbacks = kzalloc(sizeof(struct hil_funcs), GFP_KERNEL);
       if (!ct_module->pp_hil_callbacks) {
           pr_err("PP_HIL: Failed to allocate memory");
           return -ENOMEM;
       }
       memcpy(ct_module->pp_hil_callbacks, callbacks, sizeof(struct hil_funcs));
       pr_info("PP_HIL: conntrack server callback functions registered");
    }
    return 0;
}
EXPORT_SYMBOL(mrpc_conntrack_server_register);

/**
 * Function Name: conntrack_server_init
 * Desc: Initialize the conntrack server MRPC module
 * return : 0 for success, error code otherwise
 */
static int __init conntrack_server_init(void)
{
  int ret;
  ct_module = kzalloc(sizeof(*ct_module), GFP_KERNEL);
  if (!ct_module) {
     pr_err("memory allocation failed for ct_server_module!");
     ret = -ENOMEM;
     goto out_mrpc_unreg;
  }
  ct_module->mrpc = mrpc_server_register(MRPC_RESERVED_ID_CONNTRACK,
                                         "conntrack",
                                         ct_server_procs,
                                         ARRAY_SIZE(ct_server_procs));
  if (!ct_module->mrpc) {
     pr_err("failed to register conntrack mrpc module");
     ret = -EACCES;
     goto out_mrpc_unreg;
  }
  return 0;

  out_mrpc_unreg:
     mrpc_server_unregister(ct_module->mrpc);
     kfree(ct_module);
     return ret;
}

 /**
  * Function name: conntrack_server_exit
  * Desc: Remove the conntrack server module.
  * return: 0 for success, error code otherwise
  */
static void __exit conntrack_server_exit(void)
{
     mrpc_server_unregister(ct_module->mrpc);
     kfree(ct_module);
}

/******************************************************/
/**    Module Declarations                           **/
/******************************************************/
module_init(conntrack_server_init);
module_exit(conntrack_server_exit);
MODULE_AUTHOR("Intel Corporation");
MODULE_AUTHOR("Arunkumar Velayutham arunkumar.velayutham@intel.com");
MODULE_DESCRIPTION("Conntrack server RPC over Mailbox");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");


