#if defined(CONFIG_BCM_KF_DPI) && defined(CONFIG_BRCM_DPI)
/*!
 * \warning Do not change this file.
 */
#ifndef __DPI_ENGINE_H__
#define __DPI_ENGINE_H__

#define FINE_GRAIN_APPCTRL 1 //!< DO NOT change.
#define IPS_FUNC_SWITCH 0 //!< DO NOT change.

#define DPI_FUNC_IPS    (1 << 0)
#define DPI_FUNC_APP_ID (1 << 1)
#define DPI_FUNC_AV     (1 << 2)
#define DPI_FUNC_DEV_ID (1 << 3)
#define DPI_FUNC (DPI_FUNC_DEV_ID | DPI_FUNC_APP_ID)

#define BW_ERR          -1
#define BW_OK           0
#define BW_QUEUE        1

#define MAX_NAME_LEN		128
#define MAX_DEVID_HOST_NAME_LEN 32 //!< Do not change.

#define SW_NONE  0x0000
#define SW_IPS   (1 << 0) // 1
#define SW_APP   (1 << 1) // 2
#define SW_AV    (1 << 2) // 4
#define SW_DEVID (1 << 3) // 8
#define SW_SELF  (1 << 4) // 16
#define SW_ADP   (1 << 5) // 32
#define SW_SSL_DECRYPTED (1 << 6) // 64

#define SW_FG_FINAL     0x0001
#define SW_FG_NOTIA     0x0002
#define SW_FG_NOMORE    0x0004

#define IS_FG_FINAL(p)	((p)->flags & SW_FG_FINAL)
#define IS_FG_NOTIA(p)	((p)->flags & SW_FG_NOTIA)
#define IS_FG_NOMORE(p)	((p)->flags & SW_FG_NOMORE)

struct sw_struct
{
	struct sk_buff *skb;

	u8 req_flag;
	u8 type;
#ifdef FINE_GRAIN_APPCTRL
	u16 flags;
#if IPS_FUNC_SWITCH
#define IPS_FUNC_TCP_REASM		0x01
#define IPS_FUNC_L7PROTO_NORM	0x02
	u16 ips_func;		// IPS function flags
	u16 rsv;
#endif
#endif

	union 
	{
		struct 
		{
			struct
			{
				char *domain;
				u32 domain_len;
				char *path;
				u32 path_len;
				char *referer;
				u32 referer_len;
			} url;

			struct 
			{
				char *rule_name;	// Rule name
				char *cat_name;		// Category name, DDoS/BOF/...
				u32 action;		// Action
				u32 rule_id;		// Rule ID
				u16 cat_id;		// IPS Category ID
				u8 ips_proto; 		// L4 protocol
				u8 rsv;			// Reserved
			} ips;

			struct
			{
				char *app_name;		// Application name
#ifdef FINE_GRAIN_APPCTRL
				char *cat_name;		// Category name
				char *beh_name;		// Behavior name
#endif
				u16 app_id;		// Application ID
#ifdef FINE_GRAIN_APPCTRL
				u8 cat_id;		// Category ID
				u8 beh_id;		// Behavior ID
#else
				u16 cat_id;
#endif
				u32 action;		// Action
				u32 fwmark;		// Firewall mark
			} app;

#if (DPI_FUNC & DPI_FUNC_DEV_ID)
			struct
			{
				u16 vendor_id; // OS vendor, e.g. Microsoft
				u16 name_id; // OS name, e.g. Windows 8
				u16 class_id; // OS class, e.g. Windows Series
				u16 cat_id; // OS device type, e.g. Phone
				u16 prio; // Priority of matched policy

				u8 host_name[MAX_DEVID_HOST_NAME_LEN]; // host name derived from DHCP
			} devid;
#endif
		} s;
	} u;

	void *sgi;

	struct sw_struct *(*async_prepare)(struct sw_struct *);
	int (*async_send)(struct sw_struct *);
};

/* BRCM */
//#define CC_DPI_SUPPORT_DEBUG
#ifndef DPI_NULL_STMT
#define DPI_NULL_STMT                   do { /* NULL BODY */ } while (0)
#endif

#endif // __DPI_ENGINE_H__
#endif
