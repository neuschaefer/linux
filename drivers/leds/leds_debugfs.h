/*
 * Copyright (c) 2014 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef LEDS_DEBUGFS_H
#define LEDS_DEBUGFS_H

#define MTK_LEDS_UT_CORE 		0x01
#define MTK_LEDS_UT_DRIVER		0x02
#define MTK_LEDS_UT_KMS			0x04
#define MTK_LEDS_UT_PRIME		0x08
#define MTK_LEDS_UT_ATOMIC		0x10
#define MTK_LEDS_UT_VBL			0x20
extern unsigned int mtk_leds_debug;

void mtk_lp5523_debugfs_init(void);
void mtk_lp5523_debugfs_deinit(void);
void mtk_lp5523_ut_debug_printk(const char *function_name, const char *format, ...);

#define MTK_LEDS_DEBUG_DRIVER(fmt, args...)					\
	do {								\
		if (unlikely(mtk_leds_debug & MTK_LEDS_UT_CORE))		\
			mtk_lp5523_ut_debug_printk(__func__, fmt, ##args);	\
	} while (0)


#endif /* MTK_DRM_DEBUGFS_H */
