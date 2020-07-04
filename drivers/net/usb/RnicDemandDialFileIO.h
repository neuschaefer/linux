/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  �� �� ��   : RnicDemandDialFileIO.h
  ��������   : 3G���貦���ļ�����ʵ��

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
  1 ���������Լ��궨��
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

/*�ڴ��������궨��*/
#define PS_MEM_SET(pBuffer,ucData,Count)                    memset(pBuffer, ucData, Count)
#define PS_MIN(x, y)\
        (((x) < (y))? (x) : (y))



typedef void                   VOS_VOID;
typedef unsigned long          VOS_UINT32;
typedef signed   long          VOS_INT32;
typedef char                   VOS_CHAR;  


/*****************************************************************************
 ö����    : RNIC_DIAL_MODE_ENUM
 ö��˵��  : ���ò���ģʽ
 1.��    ��   : 2011��12��06��
   �޸�����   : ����
*****************************************************************************/
typedef enum 
{
    DIAL_MODE_DEMAND_DISABLED,                     
    DIAL_MODE_DEMAND_CONNECT,                      /*Demand dial mode*/
    DIAL_MODE_DEMAND_DISCONNECT,                   /*Demand dial mode*/
    DIAL_MODE_BUTT
}RNIC_DIAL_MODE_ENUM;
 
/*****************************************************************************
 ö����    : RNIC_DIAL_EVENT_REPORT_FLAG_ENUM
 ö��˵��  : �¼��ϱ���Ӧ�õı�ʶ
 1.��    ��   : 2012��1��31��
   �޸�����   : ����
*****************************************************************************/
typedef enum 
{
    FORBID_EVENT_REPORT            = 0x0000,                 /*����Ӧ���ϱ�*/
    ALLOW_EVENT_REPORT             = 0X0001,                /*�����Ӧ���ϱ�*/
    RNIC_DIAL_EVENT_REPORT_FLAG_BUTT
}RNIC_DIAL_EVENT_REPORT_FLAG_ENUM;

/*****************************************************************************
 �ṹ��    : RNIC_DIAL_MODE_STRU
 �ṹ˵��  : ����ģʽ�����貦��ʱ��
 1.��    ��   : 2011��12��06��
   �޸�����   : ����

*****************************************************************************/
typedef struct
{
    RNIC_DIAL_MODE_ENUM            enDialMode;         /* Dialģʽ */
    RNIC_DIAL_EVENT_REPORT_FLAG_ENUM     enEventReportFlag;  /*�Ƿ��Ӧ���ϱ���ʶ*/
    struct timer_list flux_timer; //timer
    unsigned int disconnect_time; //minutes
    unsigned long last_net_time;  //last rx/tx time
    bool is_timer_init;
    bool is_flow;
}RNIC_DIAL_MODE_STRU;

/*****************************************************************************
 �ṹ��    : RNIC_FLUX_TIMER
 �ṹ˵��  : ������ʱ�ϱ�

 �޸�����   : ���Ӷ�ʱ��,������ʱ��ʱ�ϱ����������¼���

*****************************************************************************/
typedef struct
{  
    struct timer_list timer; //timer
    bool is_timer_init;
    
}RNIC_FLUX_TIMER;

/*********************��������**************************/
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

