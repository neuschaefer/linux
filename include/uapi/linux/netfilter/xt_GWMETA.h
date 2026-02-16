/* 
 * 
 *
 * Copyright (C) 2011, Intel Ltd.
 * All Rights Reserved.
 ** Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL").
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *
   You should have received a copy of the GNU General Public License 
   along with this program; if not, write to the Free Software 
   Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
   The full GNU General Public License is included in this distribution 
   in the file called LICENSE.GPL.

   Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052
 
 
 * Description:
 *   This module provide extends netfilter feature to set a bitmask on a packets.
 *   In a difference from netfilter this masks lasts till packet leaves gateway TCP stack 
 *
 *   The module follows the Netfilter framework, called extended packet
 *   matching modules.
 * Usage :
 *  iptables -t mangle -I POSTROUTING 1 -o rndbr1 -p tcp --dport 6800:6866 -j GWMETA --gwmeta-gwmask 0x00000020
 *  iptables -t mangle -I OUTPUT 1 -o rndbr1 -p tcp --dport 6800:6866 -j GWMETA --gwmeta-gwmask 0x00000002
 *   Packet will have resulted ti_gw_meta ORed mask 0x22 
 */

#ifndef __XT_GWMETA_H_
#define __XT_GWMETA_H_
               
enum xt_gw_skb
{
	XT_GW_META = 1,
	XT_GW_DIS_PP = 2,
	XT_GW_EN_PP = 3
};
               
struct xt_gw_skb_rule_info 
{
	u_int32_t  gwmask;
	enum xt_gw_skb type;
};

#endif /*_XT_GWMETA_H*/
