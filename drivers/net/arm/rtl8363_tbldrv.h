#ifndef _RTL8363_TBLDRV_H_
#define _RTL8363_TBLDRV_H_
#endif

/*Vlan*/	
#define RTL8363_VLAN_FULL			-10		/*Vlan number is full*/
#define RTL8363_VLAN_VIDEXISTED	-11		
#define RTL8363_VLAN_VIDNOTEXISTS	-12
#define RTL8363_ACL_FULL -2
#define RTL8363_ACL_NOTEXIST -2



s8 rtl8363_initVlan(void);
s8 rtl8363_addVlan(u16 vid);
s8 rtl8363_delVlan(u16 vid);
s8 rtl8363_addVlanPortMember(u16 vid, u8 port);
s8 rtl8363_delVlanPortMember(u16 vid, u8 port);
s8 rtl8363_getVlanPortMember(u16 vid, u8 *portmask);
s8 rtl8363_setPvid(u8 port, u16 vid);
s8 rtl8363_getPvid(u8 port, u16 *vid);
s8 rtl8363_setIngressFilter(u8 enabled);
s8 rtl8363_setVlanTagOnly(u8 enabled);
s8 rtl8363_setVlanTagAware(u8 enabled);
s8 rtl8363_addAclRule(u8 phyport, u8 protocol, u16 data, u8 priority, u8 action) ;
s8 rtl8363_delAclRule(u8 phyport, u8 protocol, u16 data) ;



