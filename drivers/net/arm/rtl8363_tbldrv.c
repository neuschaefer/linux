    /*
    * Copyright c                  Realtek Semiconductor Corporation, 2007 
    * All rights reserved.
    * 
    * Program : RTL8363 switch high-level API
    * Abstract : 
    * Author : Robin Zheng-bei Xing                
    *  $Id: rtl8363_tbldrv.c,v 1.5 2008-03-20 01:45:21 robin_xing Exp $
    */

/*	@doc RTL8363_TBLDRV_API

	@module Rtl8363_tblAsicDrv.c - RTL8213M/RTL8203M/RTL8363H/RTL8303H/RTL8363S Switch Table Driver API documentation	|
	This document explains API interface of the asic. 
	@normal robin_xing <date>

	Copyright <cp>2008 Realtek<tm> Semiconductor Cooperation, All Rights Reserved.

 	@head3 List of Symbols |
 	Here is a list of all functions and variables in this module.

 	@index | RTL8363_ASICDRIVER_API
*/


#include "rtl_types.h"    
#include "rtl8363_asicdrv.h"
#include "rtl8363_tbldrv.h"

/*
@func int8 | rtl8363_initVlan | Initialize Vlan before use vlan
@rvalue SUCCESS 
@rvalue FAILED
@comm
	Because the vlan is disabled as the default configuration, user must call the function before using vlan.
	It will clear vlan table and set a default vlan(vid 1), which all ports are in the vlan,
	and set all ports vid to the default vlan.
*/
int8 rtl8363_initVlan(void)
{
	uint8 i;


	/*clear vlan table*/
	for(i = 0;i < 16; i++)
		rtl8363_setAsicVlan(i, 0, 0);

	/*set switch default configuration */
	rtl8363_setVlanTagAware(TRUE);	/*enable tag aware*/
	rtl8363_setIngressFilter(FALSE);		/*disable ingress filter*/
	rtl8363_setVlanTagOnly(FALSE);		/*disable vlan tag only*/

	/*add a default vlan which contains all ports*/
	rtl8363_addVlan(1);
	for(i = 0; i < 3; i++)
		rtl8363_addVlanPortMember(1, i);
	/*set all ports' vid to vlan 1*/
	for(i = 0; i < 3; i++)
		rtl8363_setPvid(i, 1);

	/*set vlan enabled*/
	rtl8363_setAsicVlanEnable(TRUE);
	return SUCCESS;
}

/*
@func int8 | rtl8363_addVlan | Add a Vlan by vid
@parm uint16 | vid | Specify the VLAN ID
@rvalue SUCCESS 
@rvalue FAILED
@rvalue RTL8363_VLAN_VIDEXISTED
@rvalue RTL8363_VLAN_FULL
@comm
	Totally there are 16 Vlans could be added. Vid should be 1~4094.
	User could not add a vid that has existed in the vlan,or it will return rtl8363_VLAN_VIDEXISTED error.
	If the vlan table is full, user can not add a vlan any more. It will return  rtl8363_VLAN_FULL. 
	User can delete a vlan using rtl8363_delVlan and then add a vlan again.
	Notice: the function does not set vlan port member, so remember call rtl8363_addVlanPortMember to set port members
*/
int8 rtl8363_addVlan(uint16 vid)
{
	uint16 vidData;
      uint8 memData,index;
	uint8 fullflag;
	int8 i;
    
	/*check vid*/
	if ( vid < 1 || vid > 4094 )
		return FAILED;

	/*check if vid exists and check if vlan is full*/
	fullflag = TRUE;
	index = 16;
	for(i = 15;i >= 0; i--)
	{
		rtl8363_getAsicVlan(i, &vidData, &memData);
		if (vidData == 0)	/*has empty entry*/
		{
			index = i;	/*set available Vlan Entry index*/
			fullflag = FALSE;
			continue;
		}
		if (vidData == vid)
			return RTL8363_VLAN_VIDEXISTED;
	}
	if (fullflag == TRUE)
		return RTL8363_VLAN_FULL;

	/*check over and add Vlan*/
	rtl8363_setAsicVlan(index, vid, 0);
	return SUCCESS;
	
}

/*
@func int8 | rtl8363_delVlan | delete a Vlan by vid
@parm uint16 | vid | Specify the VLAN ID
@rvalue SUCCESS 
@rvalue FAILED
@rvalue rtl8363_VLAN_VIDNOTEXIST
@comm
	 Vid should be 1~4094.
	 If the vid does not exist, it will return rtl8363_VLAN_VIDNOTEXIST.
*/
int8 rtl8363_delVlan(uint16 vid)
{
	uint16 vidData;
       uint8 memData;
	uint8 i,index;
    
	/*check vid*/
	if ( vid < 1 || vid > 4094 )
		return FAILED;
	
	/*search the vid*/
	index = 16;	/*init a invalid value*/
	for(i = 0; i  < 16; i++)
	{
		rtl8363_getAsicVlan(i, &vidData, &memData);
		if ( vidData == vid)
		{
			index = i;
			break;
		}			
	}
	
	if (index == 16)	/*vid not exists*/
		return RTL8363_VLAN_VIDNOTEXISTS;

	/*delete the vid*/
	rtl8363_setAsicVlan(index,0,0);
	return SUCCESS;
	
}

/*
@func int8 | rtl8363_addVlanPortMember | Add a port into an existed vlan
@parm uint16 | vid | Specify the VLAN ID
@parm uint16 | port | Port number to be added into the vlan
@rvalue SUCCESS 
@rvalue FAILED
@rvalue rtl8363_VLAN_VIDNOTEXIST
@comm
	 Make sure the vid has existed. Otherwise the Vlan must be added using rtl8363_addVlan() first.
	 Vid should be 1~4094. Port should be 0~2.
*/
int8 rtl8363_addVlanPortMember(uint16 vid, uint8 port)
{
	uint16 vidData;
      uint8 memData;
	uint8 i,index;

	/*check vid*/
	if ( vid < 1 || vid > 4094 )
		return FAILED;
	/*check port number*/
	if (port > 2)
		return FAILED;

	/*search the vid*/
	index = 16;	/*init a invalid value*/
	for(i = 0; i  < 16; i++)
	{
		rtl8363_getAsicVlan(i, &vidData, &memData);
		if ( vidData == vid)
		{
			index = i;
			break;
		}			
	}
	
	if (index == 16)	/*vid not exists*/
		return RTL8363_VLAN_VIDNOTEXISTS;

	/*add the port number to the Vlan*/
	memData |= 1 << port;
	rtl8363_setAsicVlan(index, vid, memData);
    
	return SUCCESS;
}

/*
@func int8 | rtl8363_delVlanPortMember | delete a port in an existed vlan
@parm uint16 | vid | Specify the VLAN ID
@parm uint16 | port | Port number to be deleted in the vlan
@rvalue SUCCESS 
@rvalue FAILED
@rvalue rtl8363_VLAN_INVALIDPORT
@rvalue rtl8363_VLAN_VIDNOTEXIST
@comm
	 The function does not check if the port was in the vlan member set.
	 But it guarantees the port will not in the vlan member set when this function is called successfully.
	 Vid should be 1~4094. Port should be 0~2.
*/
int8 rtl8363_delVlanPortMember(uint16 vid, uint8 port)
{
	uint16 vidData;
       uint8 memData;
	uint8 i,index;

	/*check vid*/
	if ( vid < 1 || vid > 4094 )
		return FAILED;
	/*check port number*/
	if (port > 2)
		return FAILED;

	/*search the vid*/
	index = 16;	/*init a invalid value*/
	for(i = 0; i  < 16; i++)
	{
		rtl8363_getAsicVlan(i, &vidData, &memData);
		if ( vidData == vid)
		{
			index = i;
			break;
		}			
	}
	
	if (index == 16)	/*vid not exists*/
		return RTL8363_VLAN_VIDNOTEXISTS;

	/*delete the port number in the Vlan*/
	memData &= ~(1 << port); /*set the port member maskbit to 0*/
	rtl8363_setAsicVlan(index, vid, memData);
    
	return SUCCESS;
	
}

/*
@func int16 | rtl8363_getVlanPortMember | get a vlan's portmask
@parm uint16 | vid | Specify the VLAN ID
@parm uint8* | portmask | a point to get portmask
@rvalue SUCCESS 
@rvalue FAILED
@rvalue rtl8363_VLAN_VIDNOTEXIST
@comm
	 Vid should be 1~4094.
	 If the vid does not exist, it will return rtl8363_VLAN_VIDNOTEXIST.
	 The portmask bit0 means port0.(portmake 23 -- 010111 means port 0,1,2,4 in the member set)
*/
int8 rtl8363_getVlanPortMember(uint16 vid, uint8 *portmask)
{
	uint16 vidData;
      uint8 memData;
	uint8 i,index;

	/*check vid*/
	if ( vid < 1 || vid > 4094 )
		return FAILED;
	/*check portmask*/
	if (portmask == NULL)
		return FAILED;

	/*search the vid*/
	index = 16;	/*init a invalid value*/
	for(i = 0; i  < 16; i++)
	{
		rtl8363_getAsicVlan(i, &vidData, &memData);
		if ( vidData == vid)
		{
			index = i;
			break;
		}			
	}
	
	if (index == 16)	/*vid not exists*/
		return RTL8363_VLAN_VIDNOTEXISTS;

	/*get the portmask of the Vlan*/
	rtl8363_getAsicVlan(index, &vid, portmask);
	return SUCCESS;
}

/*
@func int8 | rtl8363_setPvid | set a given port's pvid
@parm uint8 | port | The given port.
@parm uint16 | vid |  vid
@rvalue SUCCESS 
@rvalue FAILED
@rvalue rtl8363_VLAN_VIDNOTEXISTS
@comm
	 Vid should be 1~4094. Port should be 0~2.
	 Make sure the vid has existed in the vlan table, or it will return a rtl8363_VLAN_VIDNOTEXISTS error.
*/
int8 rtl8363_setPvid(uint8 port, uint16 vid)
{
	uint16 vidData;
       uint8 memData;
	uint8 i,index;
	
	/*check vid*/
	if ( vid > 4094 || vid < 1)
		return FAILED;
	
	/*check port number*/
	if (port > 2)
		return FAILED;

	/*For init only*/
	if ( vid == 0)
		rtl8363_setAsicPortVlanIndex(port, 0);

	/*search the vid*/
	index = 16;	/*init a invalid value*/
	for(i = 0; i  < 16; i++)
	{
		rtl8363_getAsicVlan(i, &vidData, &memData);
		if ( vidData == vid)
		{
			index = i;
			break;
		}			
	}
	
	if (index == 16)	/*vid not exists*/
		return RTL8363_VLAN_VIDNOTEXISTS;

	/*set pvid*/
	rtl8363_setAsicPortVlanIndex(port,index);
    
	
	return SUCCESS;
}

/*
@func int8 | rtl8363_getPvid | get a given port's pvid
@parm uint8 | port | The given port.
@parm uint16* | vid | a point to fetch the port's pvid
@rvalue SUCCESS 
@rvalue FAILED
@comm
	 Port should be 0~2. Return value by the vid point. 
*/
int8 rtl8363_getPvid(uint8 port, uint16 *vid)
{
	uint8 index,memData;

	/*check port number*/
	if (port > 2)
		return FAILED;

	/*check vid*/
	if (vid == NULL)
		return FAILED;
		
	/*get the pvid*/
	rtl8363_getAsicPortVlanIndex(port ,&index);
	rtl8363_getAsicVlan(index,vid,&memData);

	return SUCCESS;
}

/*
@func int8 | rtl8363_setIngressFilter | enable or disable the vlan ingress filter
@parm uint8 | enabled | TRUE or FALSE
@rvalue SUCCESS 
@rvalue FAILED
@comm
	If enable the filter(set TRUE),the switch will drop the received frames if the ingress port is 
	not included in the matched VLAN member set. 
	The default set is FALSE.
*/
int8 rtl8363_setIngressFilter(uint8 enabled)
{
	if ( !(enabled == TRUE || enabled == FALSE) )
		return FAILED;
	rtl8363_setAsicVlanIngressFilter(enabled);
	return SUCCESS;
}

/*
@func int8 | rtl8363_setVlanTagOnly | set the switch is vlan taged only or not
@parm uint8 | enabled | TRUE or FALSE
@rvalue SUCCESS 
@rvalue FAILED
@comm
  	If enable the filter(set TRUE),the switch will only accept tagged frames 
  	and will drop untagged frames.
  	The default set is FALSE.
*/
int8 rtl8363_setVlanTagOnly(uint8 enabled)
{
	if ( !(enabled == TRUE || enabled == FALSE) )
		return FAILED;
	rtl8363_setAsicVlanTaggedOnly(enabled);
	return SUCCESS;
}

/*
@func int8 | rtl8363_setVlanTagAware | set the switch is vlan taged aware or not
@parm uint8 | enabled | TRUE or FALSE
@rvalue SUCCESS 
@rvalue FAILED
@comm
  	If enable the filter(set TRUE),the switch will check the tagged VID 
  	and then performs VLAN mapping, but still use port-based VLAN mapping 
  	for priority-tagged and untagged frames.
  	The default set is TRUE in rtl8363_initVlan.
*/
int8 rtl8363_setVlanTagAware(uint8 enabled)
{
	if ( !(enabled == TRUE || enabled == FALSE) )
		return FAILED;
	rtl8363_setAsicVlanTagAware(enabled);
	return SUCCESS;
}

/*
@func int8 | rtl8363_initAcl | Initialize ACL table
@rvalue SUCCESS | Initialize acl table successfully, the table is cleared
@rvalue FAILED | FAILED to initialize acl table
@comm
this function should be called when system init.
*/
int8 rtl8363_initAcl (void) {
    uint8 entaddr;

    /*enable acl*/
    rtl8363_setAsicEnableAcl(TRUE);
    /*empty the acl table*/
    for (entaddr = 0; entaddr < RTL8363_ACL_ENTRYNUM; entaddr ++ ) {
        if (rtl8363_setAsicAclEntry(entaddr, RTL8363_ACL_INVALID, RTL8363_ACT_DROP, RTL8363_ACL_ETHER, 0, 0) == FAILED)
            return FAILED;
    }
    
    return SUCCESS;
}


/*
@func int8 | rtl8363_addAclRule | Add an acl rule into acl table
@parm uint8 | phyport | the physical port where packet received. 
@parm uint8 | protocol | Acl protocol which is in the packet
@parm uint16 | data | 16bit ether type or TCP/UDP source port, destination port value
@parm uint8 | priority | Acl priority assigned to the packet.
@parm uint8 | action | How to deal with the packet.
@rvalue SUCCESS | Successfully add the acl rule
@rvalue FAILED | Access Acl table error or parameter error 
@rvalue RTL8363_ACL_FULL | The Acl table is full, you should delete one before adding 
@comm
phyport could be:<nl>
RTL8363_ACL_INVALID - entry invalid<nl>
RTL8363_ACL_PORT0   - port 0<nl>
RTL8363_ACL_PORT1   - port 1<nl>
RTL8363_ACL_PORT2   - port 2<nl>
RTL8363_ACL_PORT01 - port0 & port 1<nl>
RTL8363_ACL_PORT02 - port0 & port 2<nl>
RTL8363_ACL_PORT12 - port 1 & port 2<nl>
RTL8363_ACL_ANYPORT - anyport<nl>
protocol could be RTL8363_ACL_ETHER(ether type), RTL8363_ACL_TCP(TCP), RTL8363_ACL_UDP(UDP), RTL8363_ACL_TCPUDP(TCP or UDP);<nl>
prority could be RTL8363_PRIO0~RTL8363_PRIO3;<nl>
action could be RTL8363_ACT_DROP, RTL8363_ACT_PERMIT, RTL8363_ACT_TRAPCPU, RTL8363_ACT_MIRROR ;<nl>
*/

int8 rtl8363_addAclRule(uint8 phyport, uint8 protocol, uint16 data, uint8 priority, uint8 action) {
    uint8 entaddr;
    uint8 port, pro,  pri, act;  
    uint16 val;
    uint8 isFull = TRUE;


    if (phyport == RTL8363_ACL_INVALID )
        return FAILED;
    /*if  exist an  acl entry of the same rule according by phyport, protocol,data,
       only update priority and action*/
    for (entaddr = 0; entaddr < RTL8363_ACL_ENTRYNUM; entaddr ++ ) {
        if (rtl8363_getAsicAclEntry(entaddr, &port, &act, &pro, &val, &pri) == FAILED )
            return FAILED;

        if ((phyport == port) && (protocol == pro) && (data == val)) {
            if (rtl8363_setAsicAclEntry(entaddr, phyport, action, protocol, data, priority) == FAILED )
                 return FAILED;
            else
                 return SUCCESS;
        }
            
    }    

    /*if not exist the rule, find an invalid entry to write it , else return table full */
    for (entaddr = 0; entaddr < RTL8363_ACL_ENTRYNUM; entaddr ++ ) {
        if (rtl8363_getAsicAclEntry(entaddr, &port, &act, &pro, &val, &pri) == FAILED )
            return FAILED;
        if (port == RTL8363_ACL_INVALID) {
            if (rtl8363_setAsicAclEntry(entaddr, phyport, action, protocol, data, priority) == FAILED)
                 return FAILED;
            else {
                isFull = FALSE;
                break;
            }            
            
        }            

    }    

    if (isFull)
        return RTL8363_ACL_FULL;
    else
        return SUCCESS;
    
}



/*
@func int8 | rtl8363_delAclRule | delete an acl rule 
@parm uint8 | phyport | the physical port where packet received. 
@parm uint8 | protocol | Acl protocol which is in the packet
@parm uint16 | data | 16bit ether type or TCP/UDP source port, destination port value
@rvalue SUCCESS | Suceed to delete the entry
@rvalue FAILED   | Access Acl table error or parameter error 
@rvalue RTL8363_ACL_NOTEXIST | The rule does not exist in the acl table
@comm

*/
int8 rtl8363_delAclRule(uint8 phyport, uint8 protocol, uint16 data) {
    uint8 entaddr;
    uint8 port, pro,  pri, act;
    uint16 val;
    uint8 isHit = FALSE;

    if (phyport == RTL8363_ACL_INVALID)
        return RTL8363_ACL_NOTEXIST;
    
    for (entaddr = 0; entaddr < RTL8363_ACL_ENTRYNUM; entaddr ++ ) {
        if (rtl8363_getAsicAclEntry(entaddr, &port, &act, &pro, &val, &pri) == FAILED )
            return FAILED;
        if ((port == phyport) && (pro == protocol) && (val == data)) {
            if (rtl8363_setAsicAclEntry(entaddr, RTL8363_ACL_INVALID, RTL8363_ACT_DROP, RTL8363_ACL_ETHER, 0, 0) == FAILED)
                return FAILED;
            isHit = TRUE;
            break;
        }                

    }

    if (isHit)
        return SUCCESS;
    else
        return RTL8363_ACL_NOTEXIST;
            
}




