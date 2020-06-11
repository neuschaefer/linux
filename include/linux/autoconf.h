/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.17.WB_WPCM450.1.3
 * Mon Mar 23 13:35:49 2009
 */
#define AUTOCONF_INCLUDED
#define CONFIG_ARM 1
#define CONFIG_MMU 1
#define CONFIG_RWSEM_GENERIC_SPINLOCK 1
#define CONFIG_GENERIC_HWEIGHT 1
#define CONFIG_GENERIC_CALIBRATE_DELAY 1
#define CONFIG_VECTORS_BASE 0xffff0000

/*
 * Code maturity level options
 */
#define CONFIG_EXPERIMENTAL 1
#define CONFIG_BROKEN_ON_SMP 1
#define CONFIG_INIT_ENV_ARG_LIMIT 32

/*
 * General setup
 */
#define CONFIG_LOCALVERSION ""
#define CONFIG_LOCALVERSION_AUTO 1
#undef CONFIG_SWAP
#define CONFIG_SYSVIPC 1
#define CONFIG_POSIX_MQUEUE 1
#define CONFIG_BSD_PROCESS_ACCT 1
#undef CONFIG_BSD_PROCESS_ACCT_V3
#define CONFIG_SYSCTL 1
#undef CONFIG_AUDIT
#undef CONFIG_IKCONFIG
#undef CONFIG_RELAY
#define CONFIG_INITRAMFS_SOURCE ""
#define CONFIG_UID16 1
#define CONFIG_CC_OPTIMIZE_FOR_SIZE 1
#define CONFIG_EMBEDDED 1
#undef CONFIG_KALLSYMS
#define CONFIG_HOTPLUG 1
#define CONFIG_PRINTK 1
#define CONFIG_BUG 1
#define CONFIG_ELF_CORE 1
#define CONFIG_BASE_FULL 1
#define CONFIG_FUTEX 1
#define CONFIG_EPOLL 1
#define CONFIG_SHMEM 1
#define CONFIG_SLAB 1
#undef CONFIG_TINY_SHMEM
#define CONFIG_BASE_SMALL 0
#undef CONFIG_SLOB

/*
 * Loadable module support
 */
#define CONFIG_MODULES 1
#define CONFIG_MODULE_UNLOAD 1
#undef CONFIG_MODULE_FORCE_UNLOAD
#define CONFIG_MODVERSIONS 1
#undef CONFIG_MODULE_SRCVERSION_ALL
#define CONFIG_KMOD 1

/*
 * Block layer
 */
#undef CONFIG_BLK_DEV_IO_TRACE

/*
 * IO Schedulers
 */
#define CONFIG_IOSCHED_NOOP 1
#undef CONFIG_IOSCHED_AS
#define CONFIG_IOSCHED_DEADLINE 1
#define CONFIG_IOSCHED_CFQ 1
#undef CONFIG_DEFAULT_AS
#define CONFIG_DEFAULT_DEADLINE 1
#undef CONFIG_DEFAULT_CFQ
#undef CONFIG_DEFAULT_NOOP
#define CONFIG_DEFAULT_IOSCHED "deadline"

/*
 * System Type
 */
#undef CONFIG_ARCH_CLPS7500
#undef CONFIG_ARCH_CLPS711X
#undef CONFIG_ARCH_CO285
#undef CONFIG_ARCH_EBSA110
#undef CONFIG_ARCH_EP93XX
#undef CONFIG_ARCH_FOOTBRIDGE
#undef CONFIG_ARCH_INTEGRATOR
#undef CONFIG_ARCH_IOP3XX
#undef CONFIG_ARCH_IXP4XX
#undef CONFIG_ARCH_IXP2000
#undef CONFIG_ARCH_IXP23XX
#undef CONFIG_ARCH_L7200
#undef CONFIG_ARCH_PXA
#undef CONFIG_ARCH_RPC
#undef CONFIG_ARCH_SA1100
#undef CONFIG_ARCH_S3C2410
#undef CONFIG_ARCH_SHARK
#undef CONFIG_ARCH_LH7A40X
#undef CONFIG_ARCH_OMAP
#undef CONFIG_ARCH_VERSATILE
#undef CONFIG_ARCH_REALVIEW
#undef CONFIG_ARCH_IMX
#undef CONFIG_ARCH_H720X
#undef CONFIG_ARCH_AAEC2000
#undef CONFIG_ARCH_AT91RM9200
#define CONFIG_MACH_WPCM450 1
#define CONFIG_CPU_WPCM450 1

/*
 * WPCM450 Boot
 */
#define CONFIG_WPCM450_BOOT_ERROR_RESET 1

/*
 * WPCM450 Setup
 */
#undef CONFIG_WPCM450_DMA
#define CONFIG_WPCM450_LOWLEVEL_UART_PORT 0

/*
 * Processor Type
 */
#define CONFIG_CPU_32 1
#define CONFIG_CPU_ARM926T 1
#define CONFIG_CPU_32v5 1
#define CONFIG_CPU_ABRT_EV5TJ 1
#define CONFIG_CPU_CACHE_VIVT 1
#define CONFIG_CPU_COPY_V4WB 1
#define CONFIG_CPU_TLB_V4WBI 1

/*
 * Processor Features
 */
#define CONFIG_ARM_THUMB 1
#undef CONFIG_CPU_ICACHE_DISABLE
#undef CONFIG_CPU_DCACHE_DISABLE
#undef CONFIG_CPU_DCACHE_WRITETHROUGH
#undef CONFIG_CPU_CACHE_ROUND_ROBIN

/*
 * Bus support
 */

/*
 * PCCARD (PCMCIA/CardBus) support
 */
#undef CONFIG_PCCARD

/*
 * Kernel Features
 */
#undef CONFIG_PREEMPT
#undef CONFIG_NO_IDLE_HZ
#define CONFIG_HZ 100
#undef CONFIG_AEABI
#undef CONFIG_ARCH_DISCONTIGMEM_ENABLE
#define CONFIG_SELECT_MEMORY_MODEL 1
#define CONFIG_FLATMEM_MANUAL 1
#undef CONFIG_DISCONTIGMEM_MANUAL
#undef CONFIG_SPARSEMEM_MANUAL
#define CONFIG_FLATMEM 1
#define CONFIG_FLAT_NODE_MEM_MAP 1
#undef CONFIG_SPARSEMEM_STATIC
#define CONFIG_SPLIT_PTLOCK_CPUS 4096
#define CONFIG_ALIGNMENT_TRAP 1

/*
 * Boot options
 */
#define CONFIG_ZBOOT_ROM_TEXT 0x0
#define CONFIG_ZBOOT_ROM_BSS 0x0
#define CONFIG_CMDLINE "mem=96M console=ttyS0,115200n8 root=/dev/mtdblock2 rootfstype=cramfs noinitrd rw mtdparts=spi0.0:256k(bootloader),1280k(nvram),8192k(1stFS),2048k(kernel),4096k(2ndFS),15360k@256k(all_part) quiet"
#undef CONFIG_XIP_KERNEL

/*
 * Floating point emulation
 */

/*
 * At least one emulation must be selected
 */
#define CONFIG_FPE_NWFPE 1
#undef CONFIG_FPE_NWFPE_XP
#undef CONFIG_FPE_FASTFPE
#undef CONFIG_VFP

/*
 * Userspace binary formats
 */
#define CONFIG_BINFMT_ELF 1
#undef CONFIG_BINFMT_AOUT
#undef CONFIG_BINFMT_MISC
#undef CONFIG_ARTHUR

/*
 * Power management options
 */
#undef CONFIG_PM
#undef CONFIG_APM

/*
 * Networking
 */
#define CONFIG_NET 1

/*
 * Networking options
 */
#undef CONFIG_NETDEBUG
#define CONFIG_PACKET 1
#define CONFIG_PACKET_MMAP 1
#define CONFIG_UNIX 1
#undef CONFIG_NET_KEY
#define CONFIG_INET 1
#undef CONFIG_IP_MULTICAST
#undef CONFIG_IP_ADVANCED_ROUTER
#define CONFIG_IP_FIB_HASH 1
#define CONFIG_IP_PNP 1
#define CONFIG_IP_PNP_DHCP 1
#undef CONFIG_IP_PNP_BOOTP
#undef CONFIG_IP_PNP_RARP
#undef CONFIG_NET_IPIP
#undef CONFIG_NET_IPGRE
#undef CONFIG_ARPD
#undef CONFIG_SYN_COOKIES
#undef CONFIG_INET_AH
#undef CONFIG_INET_ESP
#undef CONFIG_INET_IPCOMP
#undef CONFIG_INET_XFRM_TUNNEL
#undef CONFIG_INET_TUNNEL
#define CONFIG_INET_DIAG 1
#define CONFIG_INET_TCP_DIAG 1
#undef CONFIG_TCP_CONG_ADVANCED
#define CONFIG_TCP_CONG_BIC 1
#undef CONFIG_IPV6
#undef CONFIG_INET6_XFRM_TUNNEL
#undef CONFIG_INET6_TUNNEL
#undef CONFIG_NETFILTER

/*
 * DCCP Configuration (EXPERIMENTAL)
 */
#undef CONFIG_IP_DCCP

/*
 * SCTP Configuration (EXPERIMENTAL)
 */
#undef CONFIG_IP_SCTP

/*
 * TIPC Configuration (EXPERIMENTAL)
 */
#undef CONFIG_TIPC
#undef CONFIG_ATM
#undef CONFIG_BRIDGE
#define CONFIG_VLAN_8021Q 1
#undef CONFIG_DECNET
#undef CONFIG_LLC2
#undef CONFIG_IPX
#undef CONFIG_ATALK
#undef CONFIG_X25
#undef CONFIG_LAPB
#undef CONFIG_NET_DIVERT
#undef CONFIG_ECONET
#undef CONFIG_WAN_ROUTER

/*
 * QoS and/or fair queueing
 */
#undef CONFIG_NET_SCHED

/*
 * Network testing
 */
#undef CONFIG_NET_PKTGEN
#undef CONFIG_HAMRADIO
#undef CONFIG_IRDA
#undef CONFIG_BT
#undef CONFIG_IEEE80211

/*
 * Device Drivers
 */

/*
 * Generic Driver Options
 */
#define CONFIG_STANDALONE 1
#define CONFIG_PREVENT_FIRMWARE_BUILD 1
#undef CONFIG_FW_LOADER
#undef CONFIG_DEBUG_DRIVER

/*
 * Connector - unified userspace <-> kernelspace linker
 */
#undef CONFIG_CONNECTOR

/*
 * Memory Technology Devices (MTD)
 */
#define CONFIG_MTD 1
#undef CONFIG_MTD_DEBUG
#undef CONFIG_MTD_CONCAT
#define CONFIG_MTD_PARTITIONS 1
#undef CONFIG_MTD_REDBOOT_PARTS
#define CONFIG_MTD_CMDLINE_PARTS 1
#undef CONFIG_MTD_AFS_PARTS

/*
 * User Modules And Translation Layers
 */
#define CONFIG_MTD_CHAR 1
#define CONFIG_MTD_BLOCK 1
#undef CONFIG_FTL
#undef CONFIG_NFTL
#undef CONFIG_INFTL
#undef CONFIG_RFD_FTL

/*
 * RAM/ROM/Flash chip drivers
 */
#undef CONFIG_MTD_CFI
#undef CONFIG_MTD_JEDECPROBE
#define CONFIG_MTD_MAP_BANK_WIDTH_1 1
#define CONFIG_MTD_MAP_BANK_WIDTH_2 1
#define CONFIG_MTD_MAP_BANK_WIDTH_4 1
#undef CONFIG_MTD_MAP_BANK_WIDTH_8
#undef CONFIG_MTD_MAP_BANK_WIDTH_16
#undef CONFIG_MTD_MAP_BANK_WIDTH_32
#define CONFIG_MTD_CFI_I1 1
#define CONFIG_MTD_CFI_I2 1
#undef CONFIG_MTD_CFI_I4
#undef CONFIG_MTD_CFI_I8
#undef CONFIG_MTD_RAM
#undef CONFIG_MTD_ROM
#undef CONFIG_MTD_ABSENT
#undef CONFIG_MTD_OBSOLETE_CHIPS

/*
 * Mapping drivers for chip access
 */
#undef CONFIG_MTD_COMPLEX_MAPPINGS
#undef CONFIG_MTD_PLATRAM

/*
 * Self-contained MTD device drivers
 */
#undef CONFIG_MTD_DATAFLASH
#undef CONFIG_MTD_M25P80
#define CONFIG_MTD_WPCM450_SPI_FLASH 1
#undef CONFIG_MTD_SLRAM
#undef CONFIG_MTD_PHRAM
#undef CONFIG_MTD_MTDRAM
#undef CONFIG_MTD_BLOCK2MTD

/*
 * Disk-On-Chip Device Drivers
 */
#undef CONFIG_MTD_DOC2000
#undef CONFIG_MTD_DOC2001
#undef CONFIG_MTD_DOC2001PLUS

/*
 * NAND Flash Device Drivers
 */
#undef CONFIG_MTD_NAND

/*
 * OneNAND Flash Device Drivers
 */
#undef CONFIG_MTD_ONENAND

/*
 * Parallel port support
 */
#undef CONFIG_PARPORT

/*
 * Plug and Play support
 */

/*
 * Block devices
 */
#undef CONFIG_BLK_DEV_COW_COMMON
#define CONFIG_BLK_DEV_LOOP 1
#undef CONFIG_BLK_DEV_CRYPTOLOOP
#define CONFIG_BLK_DEV_NBD 1
#define CONFIG_BLK_DEV_RAM 1
#define CONFIG_BLK_DEV_RAM_COUNT 1
#define CONFIG_BLK_DEV_RAM_SIZE 7168
#define CONFIG_BLK_DEV_INITRD 1
#undef CONFIG_CDROM_PKTCDVD
#undef CONFIG_ATA_OVER_ETH

/*
 * SCSI device support
 */
#undef CONFIG_RAID_ATTRS
#undef CONFIG_SCSI

/*
 * Multi-device support (RAID and LVM)
 */
#undef CONFIG_MD

/*
 * Fusion MPT device support
 */
#undef CONFIG_FUSION

/*
 * IEEE 1394 (FireWire) support
 */

/*
 * I2O device support
 */

/*
 * Network device support
 */
#define CONFIG_NETDEVICES 1
#undef CONFIG_DUMMY
#undef CONFIG_BONDING
#undef CONFIG_EQUALIZER
#undef CONFIG_TUN

/*
 * PHY device support
 */
#undef CONFIG_PHYLIB

/*
 * Ethernet (10 or 100Mbit)
 */
#define CONFIG_NET_ETHERNET 1
#define CONFIG_MII 1
#define CONFIG_ETH_WPCM450 1
#undef CONFIG_SMC91X
#undef CONFIG_DM9000

/*
 * Ethernet (1000 Mbit)
 */

/*
 * Ethernet (10000 Mbit)
 */

/*
 * Token Ring devices
 */

/*
 * Wireless LAN (non-hamradio)
 */
#undef CONFIG_NET_RADIO

/*
 * Wan interfaces
 */
#undef CONFIG_WAN
#undef CONFIG_PPP
#undef CONFIG_SLIP
#undef CONFIG_SHAPER
#undef CONFIG_NETCONSOLE
#undef CONFIG_NETPOLL
#undef CONFIG_NET_POLL_CONTROLLER

/*
 * ISDN subsystem
 */
#undef CONFIG_ISDN

/*
 * Input device support
 */
#define CONFIG_INPUT 1

/*
 * Userland interfaces
 */
#undef CONFIG_INPUT_MOUSEDEV
#undef CONFIG_INPUT_JOYDEV
#undef CONFIG_INPUT_TSDEV
#undef CONFIG_INPUT_EVDEV
#undef CONFIG_INPUT_EVBUG

/*
 * Input Device Drivers
 */
#undef CONFIG_INPUT_KEYBOARD
#undef CONFIG_INPUT_MOUSE
#undef CONFIG_INPUT_JOYSTICK
#undef CONFIG_INPUT_TOUCHSCREEN
#undef CONFIG_INPUT_MISC

/*
 * Hardware I/O ports
 */
#undef CONFIG_SERIO
#undef CONFIG_GAMEPORT

/*
 * Character devices
 */
#define CONFIG_VT 1
#undef CONFIG_VT_CONSOLE
#define CONFIG_HW_CONSOLE 1
#undef CONFIG_SERIAL_NONSTANDARD

/*
 * Serial drivers
 */
#undef CONFIG_SERIAL_8250

/*
 * Non-8250 serial port support
 */
#define CONFIG_SERIAL_WPCM450 1
#define CONFIG_SERIAL_WPCM450_CONSOLE 1
#define CONFIG_SERIAL_CORE 1
#define CONFIG_SERIAL_CORE_CONSOLE 1
#define CONFIG_UNIX98_PTYS 1
#define CONFIG_LEGACY_PTYS 1
#define CONFIG_LEGACY_PTY_COUNT 256

/*
 * IPMI
 */
#undef CONFIG_IPMI_HANDLER

/*
 * Watchdog Cards
 */
#define CONFIG_WATCHDOG 1
#undef CONFIG_WATCHDOG_NOWAYOUT

/*
 * Watchdog Device Drivers
 */
#define CONFIG_SOFT_WATCHDOG 1
#undef CONFIG_NVRAM
#undef CONFIG_DTLK
#undef CONFIG_R3964

/*
 * Ftape, the floppy tape device driver
 */
#undef CONFIG_RAW_DRIVER

/*
 * TPM devices
 */
#undef CONFIG_TCG_TPM
#undef CONFIG_TELCLOCK

/*
 * I2C support
 */
#undef CONFIG_I2C

/*
 * SPI support
 */
#define CONFIG_SPI 1
#undef CONFIG_SPI_DEBUG
#define CONFIG_SPI_MASTER 1

/*
 * SPI Master Controller Drivers
 */
#define CONFIG_SPI_BITBANG 1
#define CONFIG_SPI_WPCM450 1

/*
 * SPI Protocol Masters
 */

/*
 * Dallas's 1-wire bus
 */
#undef CONFIG_W1

/*
 * Hardware Monitoring support
 */
#undef CONFIG_HWMON
#undef CONFIG_HWMON_VID

/*
 * Misc devices
 */

/*
 * LED devices
 */
#undef CONFIG_NEW_LEDS

/*
 * LED drivers
 */

/*
 * LED Triggers
 */

/*
 * Multimedia devices
 */
#undef CONFIG_VIDEO_DEV
#define CONFIG_VIDEO_V4L2 1

/*
 * Digital Video Broadcasting Devices
 */
#undef CONFIG_DVB

/*
 * Graphics support
 */
#undef CONFIG_FB

/*
 * Console display driver support
 */
#undef CONFIG_VGA_CONSOLE
#define CONFIG_DUMMY_CONSOLE 1

/*
 * Sound
 */
#undef CONFIG_SOUND

/*
 * USB support
 */
#undef CONFIG_USB_ARCH_HAS_HCD
#undef CONFIG_USB_ARCH_HAS_OHCI
#undef CONFIG_USB_ARCH_HAS_EHCI

/*
 * NOTE: USB_STORAGE enables SCSI, and 'SCSI disk support'
 */

/*
 * USB Gadget Support
 */
#undef CONFIG_USB_GADGET

/*
 * MMC/SD Card support
 */
#undef CONFIG_MMC

/*
 * Real Time Clock
 */
#define CONFIG_RTC_LIB 1
#undef CONFIG_RTC_CLASS

/*
 * File systems
 */
#define CONFIG_EXT2_FS 1
#undef CONFIG_EXT2_FS_XATTR
#undef CONFIG_EXT2_FS_XIP
#undef CONFIG_EXT3_FS
#undef CONFIG_REISERFS_FS
#undef CONFIG_JFS_FS
#define CONFIG_FS_POSIX_ACL 1
#undef CONFIG_XFS_FS
#undef CONFIG_OCFS2_FS
#undef CONFIG_MINIX_FS
#undef CONFIG_ROMFS_FS
#define CONFIG_INOTIFY 1
#undef CONFIG_QUOTA
#define CONFIG_DNOTIFY 1
#undef CONFIG_AUTOFS_FS
#undef CONFIG_AUTOFS4_FS
#undef CONFIG_FUSE_FS

/*
 * CD-ROM/DVD Filesystems
 */
#undef CONFIG_ISO9660_FS
#undef CONFIG_UDF_FS

/*
 * DOS/FAT/NT Filesystems
 */
#undef CONFIG_MSDOS_FS
#undef CONFIG_VFAT_FS
#undef CONFIG_NTFS_FS

/*
 * Pseudo filesystems
 */
#define CONFIG_PROC_FS 1
#define CONFIG_SYSFS 1
#define CONFIG_TMPFS 1
#undef CONFIG_HUGETLB_PAGE
#define CONFIG_RAMFS 1
#undef CONFIG_CONFIGFS_FS

/*
 * Miscellaneous filesystems
 */
#undef CONFIG_ADFS_FS
#undef CONFIG_AFFS_FS
#undef CONFIG_HFS_FS
#undef CONFIG_HFSPLUS_FS
#undef CONFIG_BEFS_FS
#undef CONFIG_BFS_FS
#undef CONFIG_EFS_FS
#undef CONFIG_YAFFS_FS
#undef CONFIG_JFFS_FS
#define CONFIG_JFFS2_FS 1
#define CONFIG_JFFS2_FS_DEBUG 0
#define CONFIG_JFFS2_FS_WRITEBUFFER 1
#undef CONFIG_JFFS2_SUMMARY
#undef CONFIG_JFFS2_COMPRESSION_OPTIONS
#define CONFIG_JFFS2_ZLIB 1
#define CONFIG_JFFS2_RTIME 1
#undef CONFIG_JFFS2_RUBIN
#define CONFIG_CRAMFS 1
#undef CONFIG_VXFS_FS
#undef CONFIG_HPFS_FS
#undef CONFIG_QNX4FS_FS
#undef CONFIG_SYSV_FS
#undef CONFIG_UFS_FS

/*
 * Network File Systems
 */
#define CONFIG_NFS_FS 1
#define CONFIG_NFS_V3 1
#define CONFIG_NFS_V3_ACL 1
#undef CONFIG_NFS_V4
#undef CONFIG_NFS_DIRECTIO
#undef CONFIG_NFSD
#define CONFIG_ROOT_NFS 1
#define CONFIG_LOCKD 1
#define CONFIG_LOCKD_V4 1
#define CONFIG_NFS_ACL_SUPPORT 1
#define CONFIG_NFS_COMMON 1
#define CONFIG_SUNRPC 1
#undef CONFIG_RPCSEC_GSS_KRB5
#undef CONFIG_RPCSEC_GSS_SPKM3
#undef CONFIG_SMB_FS
#undef CONFIG_CIFS
#undef CONFIG_NCP_FS
#undef CONFIG_CODA_FS
#undef CONFIG_AFS_FS
#undef CONFIG_9P_FS

/*
 * Partition Types
 */
#define CONFIG_PARTITION_ADVANCED 1
#undef CONFIG_ACORN_PARTITION
#undef CONFIG_OSF_PARTITION
#undef CONFIG_AMIGA_PARTITION
#undef CONFIG_ATARI_PARTITION
#undef CONFIG_MAC_PARTITION
#define CONFIG_MSDOS_PARTITION 1
#undef CONFIG_BSD_DISKLABEL
#undef CONFIG_MINIX_SUBPARTITION
#undef CONFIG_SOLARIS_X86_PARTITION
#undef CONFIG_UNIXWARE_DISKLABEL
#undef CONFIG_LDM_PARTITION
#undef CONFIG_SGI_PARTITION
#undef CONFIG_ULTRIX_PARTITION
#undef CONFIG_SUN_PARTITION
#undef CONFIG_KARMA_PARTITION
#undef CONFIG_EFI_PARTITION

/*
 * Native Language Support
 */
#define CONFIG_NLS 1
#define CONFIG_NLS_DEFAULT "iso8859-1"
#define CONFIG_NLS_CODEPAGE_437 1
#undef CONFIG_NLS_CODEPAGE_737
#undef CONFIG_NLS_CODEPAGE_775
#undef CONFIG_NLS_CODEPAGE_850
#undef CONFIG_NLS_CODEPAGE_852
#undef CONFIG_NLS_CODEPAGE_855
#undef CONFIG_NLS_CODEPAGE_857
#undef CONFIG_NLS_CODEPAGE_860
#undef CONFIG_NLS_CODEPAGE_861
#undef CONFIG_NLS_CODEPAGE_862
#undef CONFIG_NLS_CODEPAGE_863
#undef CONFIG_NLS_CODEPAGE_864
#undef CONFIG_NLS_CODEPAGE_865
#undef CONFIG_NLS_CODEPAGE_866
#undef CONFIG_NLS_CODEPAGE_869
#undef CONFIG_NLS_CODEPAGE_936
#undef CONFIG_NLS_CODEPAGE_950
#undef CONFIG_NLS_CODEPAGE_932
#undef CONFIG_NLS_CODEPAGE_949
#undef CONFIG_NLS_CODEPAGE_874
#undef CONFIG_NLS_ISO8859_8
#undef CONFIG_NLS_CODEPAGE_1250
#undef CONFIG_NLS_CODEPAGE_1251
#define CONFIG_NLS_ASCII 1
#define CONFIG_NLS_ISO8859_1 1
#undef CONFIG_NLS_ISO8859_2
#undef CONFIG_NLS_ISO8859_3
#undef CONFIG_NLS_ISO8859_4
#undef CONFIG_NLS_ISO8859_5
#undef CONFIG_NLS_ISO8859_6
#undef CONFIG_NLS_ISO8859_7
#undef CONFIG_NLS_ISO8859_9
#undef CONFIG_NLS_ISO8859_13
#undef CONFIG_NLS_ISO8859_14
#undef CONFIG_NLS_ISO8859_15
#undef CONFIG_NLS_KOI8_R
#undef CONFIG_NLS_KOI8_U
#undef CONFIG_NLS_UTF8

/*
 * Profiling support
 */
#undef CONFIG_PROFILING

/*
 * Kernel hacking
 */
#undef CONFIG_PRINTK_TIME
#define CONFIG_MAGIC_SYSRQ 1
#define CONFIG_DEBUG_KERNEL 1
#define CONFIG_LOG_BUF_SHIFT 14
#define CONFIG_DETECT_SOFTLOCKUP 1
#undef CONFIG_SCHEDSTATS
#undef CONFIG_DEBUG_SLAB
#undef CONFIG_DEBUG_MUTEXES
#undef CONFIG_DEBUG_SPINLOCK
#undef CONFIG_DEBUG_SPINLOCK_SLEEP
#undef CONFIG_DEBUG_KOBJECT
#undef CONFIG_DEBUG_BUGVERBOSE
#undef CONFIG_DEBUG_INFO
#undef CONFIG_DEBUG_FS
#undef CONFIG_DEBUG_VM
#define CONFIG_FRAME_POINTER 1
#undef CONFIG_UNWIND_INFO
#define CONFIG_FORCED_INLINING 1
#undef CONFIG_RCU_TORTURE_TEST
#undef CONFIG_DEBUG_USER
#undef CONFIG_DEBUG_WAITQ
#undef CONFIG_DEBUG_ERRORS
#undef CONFIG_DEBUG_LL

/*
 * Security options
 */
#undef CONFIG_KEYS
#undef CONFIG_SECURITY

/*
 * Cryptographic options
 */
#undef CONFIG_CRYPTO

/*
 * Hardware crypto devices
 */

/*
 * Library routines
 */
#undef CONFIG_CRC_CCITT
#undef CONFIG_CRC16
#define CONFIG_CRC32 1
#undef CONFIG_LIBCRC32C
#define CONFIG_ZLIB_INFLATE 1
#define CONFIG_ZLIB_DEFLATE 1
