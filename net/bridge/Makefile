#
# Makefile for the IEEE 802.1d ethernet bridging layer.
#

obj-$(CONFIG_BRIDGE) += bridge.o

bridge-y	:= br.o br_device.o br_fdb.o br_forward.o br_if.o br_input.o \
			br_ioctl.o br_notify.o br_stp.o br_stp_bpdu.o \
			br_stp_if.o br_stp_timer.o br_netlink.o ti_br_notify.o

bridge-$(CONFIG_SYSFS) += br_sysfs_if.o br_sysfs_br.o

bridge-$(CONFIG_TI_L2_SELECTIVE_FORWARDER) += ti_br_sf.o
bridge-$(CONFIG_TI_L2_SELECTIVE_PACKET_HANDLING) += ti_br_sph.o
bridge-$(CONFIG_INTEL_L2VPN_L2CP_FORWARD) += intel_br_l2vpn.o


bridge-$(CONFIG_BRIDGE_NETFILTER) += br_netfilter.o

bridge-$(CONFIG_BRIDGE_IGMP_SNOOPING) += br_multicast.o br_mdb.o

bridge-$(CONFIG_BRIDGE_VLAN_FILTERING) += br_vlan.o

obj-$(CONFIG_BRIDGE_NF_EBTABLES) += netfilter/
