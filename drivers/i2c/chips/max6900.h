/*
 * drivers/i2c/chips/max6900.h
 *
 * Driver for Maxim MAX6900 RTC
 *
 * Copyright (c) 2005 DENX Software Engineering
 * Stefan Roese <sr@denx.de>
 *
 * Based on original work by
 *	Copyright (C) 2002-2004 Stefan Eletzhofer
 *	Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

struct rtc_tm {
	unsigned char	secs;
	unsigned char	mins;
	unsigned char	hours;
	unsigned char	mday;
	unsigned char	mon;
	unsigned short	year; /* xxxx 4 digits :) */
	unsigned char	wday;
	unsigned char	vl;
};

struct mem {
	unsigned int	loc;
	unsigned int	nr;
	unsigned char	*data;
};

#define RTC_GETDATETIME	0
#define RTC_SETTIME	1
#define RTC_SETDATETIME	2
#define RTC_GETCTRL	3
#define RTC_SETCTRL	4
#define MEM_READ	5
#define MEM_WRITE	6

/* Device registers */
#define MAX6900_REG_SEC		0x80
#define MAX6900_REG_MIN		0x82
#define MAX6900_REG_HOUR	0x84
#define MAX6900_REG_MDAY	0x86
#define MAX6900_REG_MON		0x88
#define MAX6900_REG_WDAY	0x8a
#define MAX6900_REG_YEAR	0x8c
#define MAX6900_REG_CENT	0x92

#define I2C_DRIVERID_MAX6900	63	/* MAX6900 real-time clock */

#define BCD_TO_BIN(val) (((val)&15) + ((val)>>4)*10)
#define BIN_TO_BCD(val) ((((val)/10)<<4) + (val)%10)

#ifdef DEBUG
# define _DBG(x, fmt, args...) do{ if (debug>=x) printk(KERN_DEBUG"%s: " fmt "\n", __FUNCTION__, ##args); } while(0);
#else
# define _DBG(x, fmt, args...) do { } while(0);
#endif

#define _DBGRTCTM(x, rtctm) if (debug>x) printk("%s: secs=%d, mins=%d, hours=%d, mday=%d, " \
			"mon=%d, year=%d, wday=%d\n", __FUNCTION__, \
			(rtctm).tm_sec, (rtctm).tm_min, (rtctm).tm_hour, (rtctm).tm_mday, \
			(rtctm).tm_mon, (rtctm).tm_year, (rtctm).tm_wday);

struct max6900_data {
	struct i2c_client client;
	struct list_head list;
	u16 ctrl;
};

static int max6900_read_mem(struct i2c_client *client, struct mem *mem);
static int max6900_write_mem(struct i2c_client *client, struct mem *mem);
unsigned long max6900_get_rtc_time(void);
int max6900_set_rtc_time(unsigned long now);
