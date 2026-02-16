/*
 *
 * puma7_reboot_hook.c
 * Description:
 * puma7 reboot hook.
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2016 Intel Corporation. All rights reserved.
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */


/* ------------------------------------------------------------------------- */

/*
 * Reboot Hook -
 * In case of graceful reboot, the userspace application sets a reboot hook
 * before calling to reboot() (before killing the init process).
 * To set a reboot hook the user needs to write to /proc/avalanche/reboot_hook.
 *
 * There are two types of reboot hook:
 * 1. REBOOT_REQUEST:
 *	'service request reply' hook, triggering a HW-MBOX message to APP-CPU.
 *	User will provide the content of the message.
 * 2. RSWARN:
 *	'RSWARN response' hook, RCE will trigger an IPC reply message to PMC in
 *	response to 'RSWARN' message.
 */

/* ------------------------------------------------------------------------- */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/uaccess.h>				/* copy_(to,from)_user */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs_macros.h>		/* proc fs helpers... */
#include <linux/kconfig.h>				/* for IS_ENABLED macro */

#include <hw_mbox_defs.h>				/* for HW_MBOX definations */
#include <hw_mbox_mem_access.h>			/* for HW_MBOX memory access */
#include <hw_mbox_drv.h>				/* for HW_MBOX Kernel API */
#include <puma7_npcpu_appcpu_defs.h>	/* for HW_MBOX tag value */

#define PRINTK(logLevel, fmt, args...) \
	printk(logLevel "reboot_hook[%s:%d]: " fmt "\n", \
		__func__, __LINE__, ## args)

//#define REBOOT_HOOK_KDBG 1 // uncomment for debug prints...

#define dbg(fmt, args...) \
	do { if(IS_ENABLED(REBOOT_HOOK_KDBG)) PRINTK("", fmt, ## args); } while (0)
#define info(fmt, args...) \
	PRINTK(KERN_INFO, fmt, ## args)
#define err(fmt, args...) \
	PRINTK(KERN_ERR, "ERROR - " fmt, ## args)

#define RCE_REPLY_TO_RSWARN_REG_ADDR	0xF3C212E4
/* This is the address of RCE bundle mailbox S/W interrupt register.
 * WARNING: hardcoded value from RCE memory space */

enum reboot_reply_type {
	REPLY_TYPE_NONE,
	REPLY_TYPE_REPLY_TO_RSWARN,
	REPLY_TYPE_REPLY_TO_REBOOT_REQUEST,

	/* do not add new values below this line!!! */
	REPLY_TYPE_COUNT
};

#define REBOOT_REPLY_IS_TYPE_VALID( _type ) \
	(((_type) >= REPLY_TYPE_NONE) && ((_type) < REPLY_TYPE_COUNT))

static const char
	*reboot_reply_type_str(enum reboot_reply_type type)
{
	int idx = -1;
	const char *strs[] = {
		"NONE",
		"RSWARN",
		"REBOOT_REQUEST"
	};

	switch (type) {
	case REPLY_TYPE_NONE:
		idx = 0;
		break;

	case REPLY_TYPE_REPLY_TO_RSWARN:
		idx = 1;
		break;

	case REPLY_TYPE_REPLY_TO_REBOOT_REQUEST:
		idx = 2;
		break;

	default:
		break;
	}

	if (idx != -1)
		return strs[idx];
	else
		return "";
}

#define MAX_REPLY_DATA_WORD_COUNT	10

struct reboot_reply_data {
	enum reboot_reply_type type;

	unsigned int regAddr;
	u32 regValue;

	unsigned int dataAddr;
	unsigned int dataWordCount;
	u32 dataWordBuf[MAX_REPLY_DATA_WORD_COUNT];
};

/*
 * Globals:
 */

static unsigned int reboot_hook_disabled = 0;
static struct reboot_reply_data	reboot_hook_reply_data;

/* ------------------------------------------------------------------------- */

static int reboot_hook_set(enum reboot_reply_type type,
		unsigned int dataWordCount, u32 *dataWordBuf)
{
	struct reboot_reply_data		tmpRrd;
	struct hw_mbox_channelConfig	chnConfig;
	struct hwMbox_opcode			opcode;
	unsigned int					dataSize;

	if (!REBOOT_REPLY_IS_TYPE_VALID(type)) {
		err("reboot type value %d is not supported", type);
		return -1;
	}

	if (dataWordCount != 0) {
		if (dataWordBuf == NULL) {
			err("(dataWordCount value (%d) > 0) but (dataWordBuf == NULL)",
				dataWordCount);
			return -1;
		}
		if (dataWordCount > MAX_REPLY_DATA_WORD_COUNT) {
			err("dataWordCount value (%d) > max allowed (%d)",
				dataWordCount, MAX_REPLY_DATA_WORD_COUNT);
			return -1;
		}
	}

	memset(&tmpRrd, 0, sizeof(struct reboot_reply_data));
	tmpRrd.type = type;

	switch (tmpRrd.type) {
	case REPLY_TYPE_NONE:
		if (dataWordCount != 0) {
			err("reboot type '%s' should not have any data (dataWordCount is %d)",
				reboot_reply_type_str(tmpRrd.type), dataWordCount);
			return-1;
		}
		break;

	case REPLY_TYPE_REPLY_TO_RSWARN:
		if (dataWordCount != 1) {
			err("reboot type '%s' should have 1 word data (dataWordCount is %d)",
				reboot_reply_type_str(tmpRrd.type), dataWordCount);
			return -1;
		}
		tmpRrd.regAddr = RCE_REPLY_TO_RSWARN_REG_ADDR;
		tmpRrd.regValue = dataWordBuf[0];
		tmpRrd.dataWordCount = 0;
		break;

	case REPLY_TYPE_REPLY_TO_REBOOT_REQUEST:
		memset(&chnConfig, 0, sizeof(struct hw_mbox_channelConfig));

		if (hwMbox_getChannelConfig(HW_MBOX_MASTER_APP_CPU,
									hw_mbox_opcode,
									False,
									&chnConfig) < 0) {
			err("failed to get HW_MBOX channel configuration");
			return -1;
		}

		dataSize = sizeof(u32) * dataWordCount;

		if (dataSize > chnConfig.memBuff.size) {
			err("reply data size (%u) > HW_MBOX channel SRAM size (%u)",
				dataSize, chnConfig.memBuff.size);
			return -1;
		}

		tmpRrd.regAddr = IO_VIRT2PHY(HW_MBOX_BASE +
			HW_MBOX_REGDEFS_CHN_COMMAND_OFFSET(chnConfig.cid));

		opcode.lenInBytes = dataSize;
		opcode.tag = NPCPU_APPCPU_HW_MBOX_TAG_PM;

		HW_MBOX_OPCODE_FIELDS_SET_FROM_STRUCT(tmpRrd.regValue, &opcode);

		tmpRrd.dataAddr = IO_VIRT2PHY(HW_MBOX_BASE +
			HW_MBOX_MEMDEFS_SRAM_OFFSET(chnConfig.memBuff.start));

		tmpRrd.dataWordCount = dataWordCount;
		memcpy(&tmpRrd.dataWordBuf, dataWordBuf, dataSize);
		break;

	default:
		/* should not happen since we already check for validity... */
		return -1;
	}

	/* all was successful, save the values! */
	memcpy(&reboot_hook_reply_data, &tmpRrd, sizeof(struct reboot_reply_data));
	return 0;
}

/*
 * reboot_hook write format:
 *
 * To set a reboot_hook (tokens delimitated by space ' '):
 *	"%s" - type: reboot reply type, value from reboot_reply_type_str
 *	"0x%08x" - buf[i]: buffer data, repeated for for n times
 *
 * To enable/disable the reboot_hook (initial value is enabled):
 *	"0" - to disable the reboot hook.
 *	"1" - to enable the reboot hook.
 */
static int reboot_hook_proc_write(struct file *fp, const char __user *buf,
		size_t count, loff_t *f_pos)
{
#define MAX_WRITE_SIZE	256
	int res;

	char inpBuf[MAX_WRITE_SIZE + 1];
	const char *delim = " ";
	char *p, *pTok;
	int tokIdx, tokLen, enDis;

	enum reboot_reply_type type;

	u32 dataWordBuf[MAX_REPLY_DATA_WORD_COUNT];
	int dataWordCount;

	tokIdx = 0;
	dataWordCount = 0;

	if (count > MAX_WRITE_SIZE) {
		err("input length (%d) > max allowed (%d)", count, MAX_WRITE_SIZE);
		return -EFAULT;
	}

	res = copy_from_user(inpBuf, buf, count);
	if (res > 0) {
		err("failed to copy from user");
		return -EFAULT;
	}

	/* make sure string is NULL terminated */
	inpBuf[count] = '\0';
	p = inpBuf;

	while (1) {
		/* check if the string is over... */
		if (p == NULL)
			break;

		pTok = strsep(&p, delim);
		if (pTok == NULL) {
			err("failed to parse input");
			return -EFAULT;
		}

		tokLen = strlen(pTok);

		dbg("idx=%d, tokLen=%d, tokStr='%s'", tokIdx, tokLen, pTok);

		/* skip empty tokens (consecutive spaces)... */
		if (tokLen == 0)
			continue;

		if (tokIdx == 0) {
			/* first token is one of 2:
			 * 1. enable/disable '[0/1]'
			 * 2. type string '%s'
			 */

			/* first check for enable/disable: */
			res = sscanf(pTok, "%d", &enDis);
			if (res == 1) {
				if (enDis == 0) {
					info("disabling reboot hook!!!");
					reboot_hook_disabled = 1;
				}
				else {
					info("enabling reboot hook...");
					reboot_hook_disabled = 0;
				}
				return count;
			}

			for (type = REPLY_TYPE_NONE;
				 type < REPLY_TYPE_COUNT;
				 type++) {

				dbg("type=%d str='%s'", type, reboot_reply_type_str(type));

				if (strncmp(pTok,
							reboot_reply_type_str(type),
							tokLen) == 0) {
					break;
				}
			}

			if (type == REPLY_TYPE_COUNT) {
				/* type not found... */
				err("reboot type '%s' is not supported", pTok);
				return -EFAULT;
			}
		}
		else
		{
			if (dataWordCount == MAX_REPLY_DATA_WORD_COUNT) {
				err("reply data word count (%d) > max allowed (%d)",
					dataWordCount, MAX_REPLY_DATA_WORD_COUNT);
				return -EFAULT;
			}

			/* all subsequent tokens format is "0x%08x" */
			res = sscanf(pTok, "0x%08x", &dataWordBuf[dataWordCount]);
			if (res != 1) {
				err("token %d '%s' does not have the expected format of '0x%%08x'",
					tokIdx, pTok);
				return -EFAULT;
			}

			dataWordCount++;
		}

		tokIdx++;
	}

	if (tokIdx == 0) {
		/* no tokens found (all consecutive spaces...) */
		err("failed to parse input");
		return -EFAULT;
	}

	if (reboot_hook_set(type, dataWordCount, dataWordBuf) != 0) {
		err("failed to set reboot hook to type '%s' (dataWordCount=%d)",
			reboot_reply_type_str(type), dataWordCount);
		return -EFAULT;
	}

	info("successfully set reboot hook to type '%s' (dataWordCount=%d)",
		reboot_reply_type_str(type), dataWordCount);

	return count;
}

static int reboot_hook_proc_read(struct seq_file *m, void *v)
{
	int i;
	if (reboot_hook_disabled)
		seq_printf(m, "\n*** reboot hook is disabled!!! ***\n\n");

	seq_printf(m, "========== REBOOT_HOOK ==========\n");
	seq_printf(m, " Type: %s\n", reboot_reply_type_str(reboot_hook_reply_data.type));
	seq_printf(m, " Register Address: 0x%08x\n", reboot_hook_reply_data.regAddr);
	seq_printf(m, " Register Value:   0x%08x\n", reboot_hook_reply_data.regValue);
	seq_printf(m, " Data Word count:  %u\n", reboot_hook_reply_data.dataWordCount);

	for (i = 0; i < reboot_hook_reply_data.dataWordCount; i++)
		seq_printf(m, " Data Word[%02d]:    0x%08x\n", i, reboot_hook_reply_data.dataWordBuf[i]);

	seq_printf(m, "========== REBOOT_HOOK ==========\n");
	return 0;
}

DECLARE_PROCFS_ENTRY(reboot_hook, reboot_hook_proc_read, reboot_hook_proc_write)

int reboot_hook_proc_init(void)
{
	proc_create("avalanche/reboot_hook", 0644, NULL, &reboot_hook_proc_fops);
	pr_info("PUMA7: /proc/avalanche/reboot_hook created\n");
	return 0;
}

int reboot_hook_is_set(void)
{
	if ((reboot_hook_disabled == 1) || (reboot_hook_reply_data.type == REPLY_TYPE_NONE))
		return 0;

	return 1;
}

int reboot_hook_run(void)
{
	int i;
	volatile u32 *addr;

	/* first write data (if there is any)... */
	for (i = 0; i < reboot_hook_reply_data.dataWordCount; i++) {
		if (i == 0) {
			printk("Reboot: writing reply data (wordCount=%d)...\n",
					reboot_hook_reply_data.dataWordCount);
			addr = (u32 *)IO_PHY2VIRT(reboot_hook_reply_data.dataAddr);
		}

		*addr = reboot_hook_reply_data.dataWordBuf[i];
		addr++;
		printk("Reboot: writing 0x%08x to 0x%p\n",
				reboot_hook_reply_data.dataWordBuf[i], addr);
	}

	/* write to kick register... */
	printk("Reboot: writing to kick register...\n");
	addr = (u32 *)IO_PHY2VIRT(reboot_hook_reply_data.regAddr);
	printk("Reboot: writing 0x%08x to 0x%p\n", reboot_hook_reply_data.regValue, addr);
	*addr = reboot_hook_reply_data.regValue;

	return 0;
}
