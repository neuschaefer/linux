/*
*********************************************************************************************************
*                                                    LINUX-KERNEL
*                                        newbie Linux Platform Develop Kits
*                                                   Kernel Module
*
*                                    (c) Copyright 2006-2011, kevin.z China
*                                             All Rights Reserved
*
* File    : standby_touch.h
* By      : kevin.z
* Version : v1.0
* Date    : 2011-5-31 14:34
* Descript:
* Update  : date                auther      ver     notes
*********************************************************************************************************
*/
#ifndef __STANDBY_TOUCH_H__
#define __STANDBY_TOUCH_H__

#include "standby_cfg.h"

#define TP_ADDR        (0x5d)
#define TP_IICBUS      (1)

extern __s32 standby_touch_init(__u32 wakeup_src);
extern __s32 standby_touch_exit(__u32 wakeup_src);
extern __u32 standby_get_data(enum touch_vol_type_e type);


#endif  /* __STANDBY_TOUCH_H__ */


