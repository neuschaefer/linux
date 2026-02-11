/*******************************************************************************
 * Copyright 2010,2011 Broadcom Corporation.  All rights reserved.
 *
 *	@file	arch/arm/plat-kona/kona_avs.c
 *
 * Unless you and Broadcom execute a separate written software license agreement
 * governing use of this software, this software is licensed to you under the
 * terms of the GNU General Public License version 2, available at
 * http://www.gnu.org/copyleft/gpl.html (the "GPL").
 *
 * Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *****************************************************************************/


#include <linux/kernel.h>
#include <linux/err.h>
#ifdef CONFIG_KONA_OTP
#include <plat/bcm_otp.h>
#endif
#include <plat/kona_avs.h>
#include <linux/platform_device.h>
#include <linux/io.h>


/*#define KONA_AVS_DEBUG*/

/*Should we move this to avs_param ?? */
#define MONITOR_VAL_MASK	   0xFF
#define MONITOR_VAL0_SHIFT	 8
#define MONITOR_VAL1_SHIFT	 16
#define MONITOR_VAL2_SHIFT	 24
#define MONITOR_VAL3_SHIFT	 0

#define AVS_ATE_MONTH_MASK	(0xF)
#define AVS_ATE_YEAR_MASK	(0xF0)
#define AVS_ATE_VAL_MASK	(0xF00)
#define AVS_ATE_CRC_MASK	(0xF000)

#define AVS_ATE_MONTH_SHIFT	(0)
#define AVS_ATE_YEAR_SHIFT	(4)
#define AVS_ATE_VAL_SHIFT	(8)
#define AVS_ATE_CRC_SHIFT	(12)

#define avs_dbg(level, args...) \
	do { \
		if (debug_mask & level) { \
			pr_info(args); \
		} \
	} while (0)

enum {
	AVS_LOG_ERR  = 1 << 0,
	AVS_LOG_WARN = 1 << 1,
	AVS_LOG_INIT = 1 << 2,
	AVS_LOG_FLOW = 1 << 3,
	AVS_LOG_INFO = 1 << 4,
};

struct silicon_type {
	 u32 csr_type;
	 u32 msr_type;
	 u32 vsr_type;
};

struct avs_info {
	u32 monitor_val0;
	u32 monitor_val1;
	u32 monitor_val2;
	u32 monitor_val3;

	u32 silicon_type;
	u32 svt_silicon_type;
	u32 lvt_silicon_type;
	u32 silicon_type_csr;
	u32 silicon_type_msr;
	u32 silicon_type_vsr;


	u32 ate_silicon_type;
	u32 ate_bin_csr;
	u32 ate_bin_msr;
	u32 ate_bin_vsr;
	u32 ate_irdrop;

	u32 freq;
	u32 avs_ate_val;
	u32 ate_crc;
	u32 year;
	u32 month;
	struct kona_avs_pdata *pdata;
};

struct avs_info avs_info = {.silicon_type = SILICON_SS, };
static int debug_mask = AVS_LOG_ERR | AVS_LOG_WARN | AVS_LOG_INIT |\
			AVS_LOG_FLOW | AVS_LOG_INFO;

module_param_named(silicon_type, avs_info.silicon_type, int, S_IRUGO);
module_param_named(svt_silicon_type, avs_info.svt_silicon_type, int, S_IRUGO);
module_param_named(lvt_silicon_type, avs_info.lvt_silicon_type, int, S_IRUGO);


module_param_named(avs_mon_val0, avs_info.monitor_val0, int, S_IRUGO | S_IWUSR
			| S_IWGRP);
module_param_named(avs_mon_val1, avs_info.monitor_val1, int, S_IRUGO | S_IWUSR
			| S_IWGRP);
module_param_named(avs_mon_val2, avs_info.monitor_val2, int, S_IRUGO | S_IWUSR
			| S_IWGRP);
module_param_named(avs_mon_val3, avs_info.monitor_val3, int, S_IRUGO | S_IWUSR
			| S_IWGRP);

module_param_named(avs_ate_val, avs_info.avs_ate_val, int, S_IRUGO | S_IWUSR
			| S_IWGRP);
module_param_named(year, avs_info.year, int, S_IRUGO | S_IWUSR
			| S_IWGRP);
module_param_named(month, avs_info.month, int, S_IRUGO | S_IWUSR
			| S_IWGRP);
module_param_named(ate_crc, avs_info.ate_crc, int, S_IRUGO | S_IWUSR
			| S_IWGRP);

module_param_named(debug_mask, debug_mask, int, S_IRUGO | S_IWUSR |
		S_IWGRP);

struct trigger_avs {
	int dummy;
};

#define __param_check_trigger_avs(name, p, type) \
	static inline struct type *__check_##name(void) { return (p); }

#define param_check_trigger_avs(name, p) \
	__param_check_trigger_avs(name, p, trigger_avs)

static int param_set_trigger_avs(const char *val,
			const struct kernel_param *kp);
static struct kernel_param_ops param_ops_trigger_avs = {
	.set = param_set_trigger_avs,
};

static struct trigger_avs trigger_avs;
module_param_named(trigger_avs, trigger_avs, trigger_avs,
				S_IWUSR | S_IWGRP);


struct mon_val {

	u32 val0;
	u32 val1;
};

struct ate_val {
	u32 val0;
	u32 val1;
};

#if defined(KONA_AVS_DEBUG)

static int otp_read(int row, struct mon_val *mon_val)
{
	avs_dbg(AVS_LOG_INFO, "%s:row = %d\n", __func__, row);
	if (row < 0)
		return -EINVAL;
	mon_val->val0 =
		(146 << MONITOR_VAL0_SHIFT) | (180 << MONITOR_VAL1_SHIFT) |
			(95 << MONITOR_VAL2_SHIFT);
	mon_val->val1 = 170;

	return 0;
}
#endif

u32 kona_avs_get_solicon_type(void)
{
	BUG_ON(avs_info.pdata == NULL);
	return avs_info.silicon_type;
}
EXPORT_SYMBOL(kona_avs_get_solicon_type);

/**
 * converts interger to string radix 2 (binary
 * number string)
 */
static void int2bin(unsigned int num, char *str)
{
	int i = 0;
	int j = 0;
	char temp[33];

	while (num != 0) {
		temp[i] = num % 2 ? '1' : '0';
		num /= 2;
		i++;
	}
	str[i] = 0;
	temp[i] = 0;

	/* reverse the string */
	while (i) {
		i--;
		str[j] = temp[i];
		j++;
	}
}

/**
 * 4-bit Linear feeback shift register implementation
 * based on primitive polynomial x^4 + x + 1
 *
 * bitstring should be null terminate
 * bit stream (exp: "10111111")
 */
static void cal_crc(const char *bitstring, char *crc_res)
{
	char crc[4];
	int i;
	char do_invert;

	memset(crc, 0x0, sizeof(crc));

	for (i = 0; i < strlen(bitstring); ++i) {
		do_invert = ('1' == bitstring[i]) ^ crc[3];
		crc[3] = crc[2];
		crc[2] = crc[1];
		crc[1] = crc[0] ^ do_invert;
		crc[0] = do_invert;
	}

	for (i = 0; i < 4; ++i)
		crc_res[3-i] = crc[i] ? '1' : '0';
	crc_res[4] = 0; /* Null Terminated */
}

static int kona_avs_get_mon_val(struct avs_info *avs_inf_ptr)
{
	struct mon_val mon_val;
	int ret = -EINVAL;

	if (avs_inf_ptr->pdata->flags & AVS_TYPE_BOOT) {
		avs_dbg(AVS_LOG_ERR,
				"%s:AVS_TYPE_BOOT not supported !!!\n",
				__func__);
		return -EINVAL;
	}

	if (avs_inf_ptr->pdata->flags & AVS_READ_FROM_MEM) {
		void __iomem *mem_ptr;
		avs_dbg(AVS_LOG_INIT, "%s: AVS_READ_FROM_MEM => mem adr = %x\n",
				__func__,
				avs_inf_ptr->pdata->avs_mon_addr);
		BUG_ON(avs_inf_ptr->pdata->avs_mon_addr == 0);
		mem_ptr =
			ioremap_nocache(avs_inf_ptr->pdata->avs_mon_addr,
					sizeof(struct mon_val));
		avs_dbg(AVS_LOG_INIT,
				"%s: AVS_READ_FROM_MEM => virtual addr = %p\n",
				__func__, mem_ptr);
		if (mem_ptr) {
			memcpy(&mon_val, mem_ptr, sizeof(struct mon_val));
			iounmap(mem_ptr);
			ret = 0;
		} else {
			ret = -ENOMEM;
			BUG_ON(mem_ptr == NULL);
		}
	} else {
		avs_dbg(AVS_LOG_INIT,
				"%s: AVS_READ_FROM_OTP => row = %x\n",
				__func__,
				avs_inf_ptr->pdata->avs_mon_addr);
#if defined(KONA_AVS_DEBUG) || defined(CONFIG_KONA_OTP)
		ret = otp_read(avs_inf_ptr->pdata->avs_mon_addr, &mon_val);
#endif
	}

	if (!ret) {

		avs_dbg(AVS_LOG_INIT, "%s:opt:val0 = %x val1 = %x\n", __func__,
				mon_val.val0,
				mon_val.val1);
		avs_inf_ptr->monitor_val0 =
			(mon_val.val0 >> MONITOR_VAL0_SHIFT) & MONITOR_VAL_MASK;
		avs_inf_ptr->monitor_val1 =
			(mon_val.val0 >> MONITOR_VAL1_SHIFT) & MONITOR_VAL_MASK;
		avs_inf_ptr->monitor_val2 =
			(mon_val.val0 >> MONITOR_VAL2_SHIFT) & MONITOR_VAL_MASK;
		avs_inf_ptr->monitor_val3 =
			(mon_val.val1 >> MONITOR_VAL3_SHIFT) & MONITOR_VAL_MASK;

		avs_dbg(AVS_LOG_INIT,
				"%s:monitor_val0 = %d"
				"monitor_val1= %d"
				"monitor_val2 = %d"
				"monitor_val3 = %d\n", __func__,
				avs_inf_ptr->monitor_val0,
				avs_inf_ptr->monitor_val1,
				avs_inf_ptr->monitor_val2,
				avs_inf_ptr->monitor_val3);
	}
	return ret;
}

static int kona_avs_get_ate_val(struct avs_info *avs_inf_ptr)
{
	struct ate_val ate_val;
	int ret = -EINVAL;
	void __iomem *mem_ptr;

	avs_dbg(AVS_LOG_FLOW, "%s\n", __func__);

	if (avs_inf_ptr->pdata->flags & AVS_TYPE_BOOT) {
		avs_dbg(AVS_LOG_ERR, "%s:AVS_TYPE_BOOT not supported !!!\n",
				__func__);
		return -EINVAL;
	}

	if (avs_inf_ptr->pdata->flags & AVS_READ_FROM_MEM) {
		BUG_ON(avs_inf_ptr->pdata->avs_ate_addr == 0);
		avs_dbg(AVS_LOG_INIT,
				"%s: AVS_READ_FROM_MEM => mem adr = %x\n",
				__func__,
				avs_inf_ptr->pdata->avs_ate_addr);
		mem_ptr = ioremap_nocache(avs_inf_ptr->pdata->avs_ate_addr,
				sizeof(ate_val));
		avs_dbg(AVS_LOG_INIT,
				"%s:AVS_READ_FROM_MEM => virtual addr = %p\n",
				__func__, mem_ptr);
		if (mem_ptr) {
			memcpy(&ate_val, mem_ptr, sizeof(ate_val));
			iounmap(mem_ptr);
			ret = 0;
		} else {
			ret = -ENOMEM;
			BUG_ON(mem_ptr == NULL);
		}
	}
	if (!ret) {
		avs_dbg(AVS_LOG_INIT, "%s:ATE val0 = %x val1 = %x\n", __func__,
				ate_val.val0,
				ate_val.val1);
		avs_inf_ptr->year = ((ate_val.val0 & AVS_ATE_YEAR_MASK) >>
				AVS_ATE_YEAR_SHIFT);
		avs_inf_ptr->month = ((ate_val.val0 & AVS_ATE_MONTH_MASK) >>
				AVS_ATE_MONTH_SHIFT);
		avs_dbg(AVS_LOG_INFO, "AVS Year & Month of Manufacturing:"
				"%d %d\n",
				((avs_inf_ptr->year == 0) ? 2012 :
				 (2010 + avs_inf_ptr->year)),
				(avs_inf_ptr->month));

		avs_inf_ptr->avs_ate_val = ((ate_val.val0 & AVS_ATE_VAL_MASK) >>
				AVS_ATE_VAL_SHIFT);
		avs_inf_ptr->ate_crc = ((ate_val.val0 & AVS_ATE_CRC_MASK) >>
				AVS_ATE_CRC_SHIFT);
		avs_dbg(AVS_LOG_INIT, "ATE_AVS_BIN[3:0]=0x%x CRC[3:0]=0x%x\n",
				avs_inf_ptr->avs_ate_val,
				avs_inf_ptr->ate_crc);
	}
	return ret;
}

static int kona_avs_ate_get_type(struct avs_info *avs_inf_ptr)
{
	struct kona_avs_pdata *pdata = avs_inf_ptr->pdata;
	char str[33];
	char pack[60];
	char crc[5];
	u32 temp1;
	u32 temp2;
	long crc_val;
	int err;

	memset(pack, 0, sizeof(pack));

	if ((avs_inf_ptr->avs_ate_val == 0) && (avs_inf_ptr->ate_crc == 0))
		return -ENODATA;

	BUG_ON(pdata->ate_lut[avs_inf_ptr->avs_ate_val].silicon_type ==
			ATE_FIELD_RESERVED);
	/**
	 * pack {ATE_AVS_BIN[3:0], Year[3:0], Month[3:0], VM3[7:0], VM2[7:0],
	 * VM1[7:0],VM0[7:0]} and calculate CRC
	 */
	temp1 = ((avs_inf_ptr->avs_ate_val << 8) | (avs_inf_ptr->year << 4) |
		(avs_inf_ptr->month));
	avs_dbg(AVS_LOG_INFO, "pack [ATE:YEAR:MONTH] = 0x%x\n", temp1);
	int2bin(temp1, str);
	strcat(pack, str);
	temp2 = ((avs_inf_ptr->monitor_val3 << 24) |
		(avs_inf_ptr->monitor_val2 << 16)|
		(avs_inf_ptr->monitor_val1 << 8) |
		(avs_inf_ptr->monitor_val0));
	avs_dbg(AVS_LOG_INFO, "pack [VM3:2:1:0] = 0x%x\n", temp2);
	int2bin(temp2, str);
	strcat(pack, str);
	avs_dbg(AVS_LOG_INFO, "packed [ATE:VM] string for CRC : %s\n", pack);

	cal_crc(pack, crc);
	/**
	 * if CRC fails, we will assume 850MHZ and slow silicon type
	 */
	err = kstrtol(crc, 2, &crc_val);
	if (err) {
		avs_dbg(AVS_LOG_ERR, "kstrtol returned error\n");
		return err;
	}

	avs_dbg(AVS_LOG_INIT, "Calcualted ATE CRC value = %x\n", (u32)crc_val);

	if (avs_inf_ptr->ate_crc != crc_val) {
		avs_dbg(AVS_LOG_ERR, "ATE CRC Failed"
				"Assuming default silicon type %d\n",
				pdata->ate_default_silicon_type);
		avs_inf_ptr->ate_silicon_type = pdata->ate_default_silicon_type;
		avs_inf_ptr->freq = pdata->ate_default_cpu_freq;
	} else {
		avs_inf_ptr->ate_silicon_type =
			pdata->ate_lut[avs_inf_ptr->avs_ate_val].silicon_type;
		avs_inf_ptr->freq =
			pdata->ate_lut[avs_inf_ptr->avs_ate_val].freq;
	}
	avs_dbg(AVS_LOG_INIT, "%s: silicon type %d freq %d\n",
			__func__,
			avs_inf_ptr->ate_silicon_type,
			avs_inf_ptr->freq);
	return 0;
}

static u32 kona_avs_get_svt_type(struct avs_info *avs_inf_ptr)
{
	int i;

	int svt_pmos_inx = -1;
	int svt_nmos_inx = -1;

	struct kona_avs_pdata *pdata = avs_inf_ptr->pdata;

	for (i = 0; i < pdata->pmos_bin_size; i++) {
		if (avs_inf_ptr->monitor_val3 >= pdata->svt_pmos_bin[i] &&
			avs_inf_ptr->monitor_val3 < pdata->svt_pmos_bin[i + 1]) {
			svt_pmos_inx = i;
			break;
		}
	}

	for (i = 0; i < pdata->nmos_bin_size; i++) {
		if (avs_inf_ptr->monitor_val2 >= pdata->svt_nmos_bin[i] &&
			avs_inf_ptr->monitor_val2 < pdata->svt_nmos_bin[i + 1]) {
			svt_nmos_inx = i;
			break;
		}
	}

	avs_dbg(AVS_LOG_INFO, "%s:svt_pmos = %d svt_nmos = %d\n",
		__func__, svt_pmos_inx, svt_nmos_inx);

	if (svt_nmos_inx == -1 || svt_pmos_inx == -1)
		return SILICON_SS;

	return pdata->svt_silicon_type_lut[svt_pmos_inx * pdata->nmos_bin_size +
					   svt_nmos_inx];
}

static u32 kona_avs_get_lvt_type(struct avs_info *avs_inf_ptr)
{
	int i;

	int lvt_pmos_inx = -1;
	int lvt_nmos_inx = -1;

	struct kona_avs_pdata *pdata = avs_inf_ptr->pdata;

	for (i = 0; i < pdata->pmos_bin_size; i++) {
		if (avs_inf_ptr->monitor_val1 >= pdata->lvt_pmos_bin[i] &&
			avs_inf_ptr->monitor_val1 < pdata->lvt_pmos_bin[i + 1]) {
			lvt_pmos_inx = i;
			break;
		}
	}

	for (i = 0; i < pdata->nmos_bin_size; i++) {
		if (avs_inf_ptr->monitor_val0 >= pdata->lvt_nmos_bin[i] &&
			avs_inf_ptr->monitor_val0 < pdata->lvt_nmos_bin[i + 1]) {
			lvt_nmos_inx = i;
			break;
		}
	}
	avs_dbg(AVS_LOG_INIT, "%s:lvt_pmos = %d lvt_nmos = %d\n",
		__func__, lvt_pmos_inx, lvt_nmos_inx);

	if (lvt_nmos_inx == -1 || lvt_pmos_inx == -1)
		return SILICON_SS;

	return pdata->lvt_silicon_type_lut[lvt_pmos_inx * pdata->nmos_bin_size +
					   lvt_nmos_inx];
}

static int avs_find_silicon_type(void)
{
	int ret = 0;
	int ate_enabled = 0;

	if (!avs_info.pdata)
		return  -EPERM;

	avs_info.svt_silicon_type = kona_avs_get_svt_type(&avs_info);
	avs_info.lvt_silicon_type = kona_avs_get_lvt_type(&avs_info);

	ate_enabled = avs_info.pdata->flags & AVS_ATE_FEATURE_ENABLE;

	if (ate_enabled)
		ret = kona_avs_ate_get_type(&avs_info);

	if (ate_enabled && !ret) {
		avs_info.silicon_type = min(avs_info.ate_silicon_type,
				min(avs_info.lvt_silicon_type,
				avs_info.svt_silicon_type));
	} else {
		avs_info.silicon_type = min(avs_info.lvt_silicon_type,
				avs_info.svt_silicon_type);
		avs_info.freq = -1;
	}

	if (avs_info.pdata->silicon_type_notify)
		avs_info.pdata->silicon_type_notify(avs_info.silicon_type,
				avs_info.freq);

	avs_dbg(AVS_LOG_INIT,
			"%s:svt type: %d lvt type: %d  silicon type: %d\n",
			__func__, avs_info.svt_silicon_type,
			avs_info.lvt_silicon_type,
			avs_info.silicon_type);

	return 0;
}

static int param_set_trigger_avs(const char *val, const struct kernel_param *kp)
{
	int trig;
	int ret = -1;

	avs_dbg(AVS_LOG_FLOW, "%s\n", __func__);
	if (!val)
		return -EINVAL;
	if (!avs_info.pdata) {
		avs_dbg(AVS_LOG_ERR,
				"%s : invalid paltform data !!\n", __func__);
		return  -EPERM;
	}
	ret = sscanf(val, "%d", &trig);
	avs_dbg(AVS_LOG_INFO, "%s, trig:%d\n", __func__, trig);
	if (trig == 1)
		avs_find_silicon_type();

	return 0;
}

static int kona_avs_drv_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct kona_avs_pdata *pdata = pdev->dev.platform_data;

	avs_dbg(AVS_LOG_INIT, "%s\n", __func__);

	if (!pdata) {
		avs_dbg(AVS_LOG_ERR,
				"%s : invalid paltform data !!\n", __func__);
		ret = -EPERM;
		goto error;
	}

	avs_info.pdata = pdata;

	BUG_ON((pdata->flags & AVS_TYPE_OPEN)
		   && (pdata->flags & AVS_TYPE_BOOT));
	BUG_ON((pdata->flags & AVS_READ_FROM_OTP)
		   && (pdata->flags & AVS_READ_FROM_MEM));

	ret = kona_avs_get_mon_val(&avs_info);
	if (ret)
		goto error;
	ret = kona_avs_get_ate_val(&avs_info);
	if (ret)
		goto error;

	avs_find_silicon_type();

error:
	return ret;
}

static int __devexit kona_avs_drv_remove(struct platform_device *pdev)
{
	return 0;
}

static struct platform_driver kona_avs_driver = {
	.probe = kona_avs_drv_probe,
	.remove = __devexit_p(kona_avs_drv_remove),
	.driver = {.name = "kona-avs",},
};

static int __init kona_avs_drv_init(void)
{
	return platform_driver_register(&kona_avs_driver);
}

subsys_initcall_sync(kona_avs_drv_init);

static void __exit kona_avs_drv_exit(void)
{
	platform_driver_unregister(&kona_avs_driver);
}

module_exit(kona_avs_drv_exit);

MODULE_ALIAS("platform:kona_avs_drv");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AVS driver for BRCM Kona based Chipsets");
