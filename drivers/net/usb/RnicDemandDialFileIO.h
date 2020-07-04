/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文 件 名   : RnicDemandDialFileIO.h
  功能描述   : 3G按需拨号文件操作实现

******************************************************************************/

#ifndef __RNICDEMANDDIALFILEIO_H__
#define __RNICDEMANDDIALFILEIO_H__
#include <linux/timer.h>


#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kallsyms.h>
#include <asm/uaccess.h>
#include <asm-generic/errno-base.h>
/*****************************************************************************
  1 数据类型以及宏定义
*****************************************************************************/
#define VOS_OK  0
#define VOS_ERR 1
#define VOS_NULL_PTR   NULL
#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif
#define RNIC_ONDEMAND_FILE_LEN          (4)
#define RNIC_IDLETIMEROUT_FILE_LEN      (16)
#define RNIC_EVENTFLAG_FILE_LEN         (4)
#define RNIC_VFILE_CRT_LEVEL            (0777)

#define RNIC_NO_FLUX_EXPIRES              (g_DialMode.disconnect_time * HZ + g_DialMode.last_net_time)
#define DEFULT_RNIC_DISCONNECT_TIME       10
#define ONDEMAND_CONNECT_FLUX_CHECK_TIME   5

/*内存内容填充宏定义*/
#define PS_MEM_SET(pBuffer,ucData,Count)                    memset(pBuffer, ucData, Count)
#define PS_MIN(x, y)\
        (((x) < (y))? (x) : (y))



typedef void                   VOS_VOID;
typedef unsigned long          VOS_UINT32;
typedef signed   long          VOS_INT32;
typedef char                   VOS_CHAR;  


/*****************************************************************************
 枚举名    : RNIC_DIAL_MODE_ENUM
 枚举说明  : 设置拨号模式
 1.日    期   : 2011年12月06日
   修改内容   : 创建
*****************************************************************************/
typedef enum 
{
    DIAL_MODE_DEMAND_DISABLED,                     
    DIAL_MODE_DEMAND_CONNECT,                      /*Demand dial mode*/
    DIAL_MODE_DEMAND_DISCONNECT,                   /*Demand dial mode*/
    DIAL_MODE_BUTT
}RNIC_DIAL_MODE_ENUM;
 
/*****************************************************************************
 枚举名    : RNIC_DIAL_EVENT_REPORT_FLAG_ENUM
 枚举说明  : 事件上报给应用的标识
 1.日    期   : 2012年1月31日
   修改内容   : 创建
*****************************************************************************/
typedef enum 
{
    FORBID_EVENT_REPORT            = 0x0000,                 /*不给应用上报*/
    ALLOW_EVENT_REPORT             = 0X0001,                /*允许给应用上报*/
    RNIC_DIAL_EVENT_REPORT_FLAG_BUTT
}RNIC_DIAL_EVENT_REPORT_FLAG_ENUM;

/*****************************************************************************
 结构名    : RNIC_DIAL_MODE_STRU
 结构说明  : 拨号模式及按需拨号时间
 1.日    期   : 2011年12月06日
   修改内容   : 创建

*****************************************************************************/
typedef struct
{
    RNIC_DIAL_MODE_ENUM            enDialMode;         /* Dial模式 */
    RNIC_DIAL_EVENT_REPORT_FLAG_ENUM     enEventReportFlag;  /*是否给应用上报标识*/
    struct timer_list flux_timer; //timer
    unsigned int disconnect_time; //minutes
    unsigned long last_net_time;  //last rx/tx time
    bool is_timer_init;
    bool is_flow;
}RNIC_DIAL_MODE_STRU;

/*****************************************************************************
 结构名    : RNIC_FLUX_TIMER
 结构说明  : 流量定时上报

 修改内容   : 增加定时器,有流量时定时上报按需连接事件。

*****************************************************************************/
typedef struct
{  
    struct timer_list timer; //timer
    bool is_timer_init;
    
}RNIC_FLUX_TIMER;

/*********************函数声明**************************/
ssize_t RNIC_WriteOnDemandFile(
    struct file                        *file,
    const char __user                  *buf,
    size_t                              len,
    loff_t                             *ppos
);
ssize_t RNIC_ReadOnDemandFile(
    struct file                        *file,
    char __user                        *buf,
    size_t                             len,
    loff_t                             *ppos
);
ssize_t RNIC_WriteIdleTimerOutFile(
    struct file                        *file,
    const char __user                  *buf,
    size_t                              len,
    loff_t                             *ppos
);
ssize_t RNIC_ReadIdleTimerOutFile(
    struct file                        *file,
    char __user                        *buf,
    size_t                             len,
    loff_t                             *ppos
);
ssize_t RNIC_WriteDialEventReportFile(
    struct file                        *file,
    const char __user                  *buf,
    size_t                              len,
    loff_t                             *ppos
);
ssize_t RNIC_ReadDialEventReportFile(
    struct file                        *file,
    char __user                        *buf,
    size_t                             len,
    loff_t                             *ppos
);
ssize_t RNIC_ReadWlanDevNameFile(
    struct file                        *file,
    char __user                        *buf,
    size_t                              len,
    loff_t                             *ppos
);

ssize_t RNIC_WriteWlanDevNameFile(
    struct file                        *file,
    const char __user                  *buf,
    size_t                              len,
    loff_t                             *ppos
);

ssize_t RNIC_ReadEthIsLanDevFile(
    struct file                        *file,
    char __user*                        buf,
    size_t                              len,
    loff_t*                             ppos
);
ssize_t RNIC_WriteEthIsLanDevFile(
    struct file*                        file,
    const char __user*                  buf,
    size_t                              len,
    loff_t*                             ppos
);
VOS_UINT32 RNIC_InitOnDemandFile(struct proc_dir_entry *pstParentFileDirEntry);
VOS_UINT32 RNIC_InitIdleTimerOutFile(struct proc_dir_entry *pstParentFileDirEntry);
VOS_UINT32 RNIC_InitDialEventReportFile(struct proc_dir_entry *pstParentFileDirEntry);

VOS_UINT32 RNIC_InitWlanDevNameFile(struct proc_dir_entry *pstParentFileDirEntry);
VOS_UINT32 RNIC_InitEthIsLanDevFile(struct proc_dir_entry* pstParentFileDirEntry);


VOS_UINT32 RNIC_InitDemandDialFile(VOS_VOID);

VOS_VOID Rnic_SendConnectEvent(VOS_VOID);
VOS_VOID Rnic_SendDisonnectEvent(VOS_VOID);

extern RNIC_DIAL_MODE_STRU  g_DialMode;
extern void set_rnic_last_net_time(unsigned long time);


#endif

