#
# Makefile for the IEEE 802.1d ethernet bridging layer.
#

ifneq ($(strip $(CONFIG_BCM_WLAN)),)
EXTRA_CFLAGS    += -DDSLCPE -DBCMDRIVER -DPKTC
EXTRA_CFLAGS    += -I$(BRCMDRIVERS_DIR)/broadcom/net/wl/impl$(CONFIG_BCM_WLAN_IMPL)/include
EXTRA_CFLAGS    += -I$(DRIVER_VENDOR_DIR)/wlan/bcm963138/include
endif

obj-$(CONFIG_BRIDGE) += bridge.o

bridge-y	:= br.o br_device.o br_fdb.o br_forward.o br_if.o br_input.o \
			br_ioctl.o br_notify.o br_stp.o br_stp_bpdu.o \
			br_stp_if.o br_stp_timer.o br_netlink.o

#ifdef BCM_KF # defined(CONFIG_BLOG)
bridge-y += br_flows.o
#endif # BCM_KF

bridge-$(CONFIG_SYSFS) += br_sysfs_if.o br_sysfs_br.o

#ifdef BCM_KF # defined(CONFIG_BCM_KF_IGMP)
bridge-$(CONFIG_BR_IGMP_SNOOP) += br_igmp_parse.o

bridge-$(CONFIG_BR_IGMP_SNOOP) += br_igmp_dev_list.o
bridge-$(CONFIG_BR_IGMP_SNOOP) += br_igmp.o
bridge-$(CONFIG_BR_MLD_SNOOP) += br_mld.o
#endif # BCM_KF

bridge-$(CONFIG_MLD_SNOOPING) += br_mld_snooping.o

bridge-$(CONFIG_BRIDGE_NETFILTER) += br_netfilter.o

bridge-$(CONFIG_BRIDGE_IGMP_SNOOPING) += br_multicast.o

obj-$(CONFIG_BRIDGE_NF_EBTABLES) += netfilter/

#ifdef BCM_KF # defined(CONFIG_BCM_KF_IGMP)
ifeq ($(strip $(CONFIG_BR_IGMP_SNOOP)),y)
bridge-$(CONFIG_BR_IGMP_SNOOP) += br_netlink_mcpd.o br_mcast.o
else
ifeq ($(strip $(CONFIG_BR_MLD_SNOOP)),y)
bridge-$(CONFIG_BR_MLD_SNOOP) += br_netlink_mcpd.o br_mcast.o
endif
endif
#endif # BCM_KF

#ifdef BCM_KF # defined(CONFIG_BCM_KF_NETFILTER)
bridge-y += br_notifier.o
#endif # BCM_KF

#ifdef BCM_KF # defined(CONFIG_BCM_KF_RUNNER)
ifdef CONFIG_BCM_RDPA_BRIDGE
bridge-y += br_fp.o
endif # CONFIG_BCM_RDPA_BRIDGE
#endif # BCM_KF
