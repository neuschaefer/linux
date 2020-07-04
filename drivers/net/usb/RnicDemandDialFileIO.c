/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  �� �� ��   : RnicDemandDialFileIO.h
  ��������   : 3G���貦���ļ�����ʵ��

******************************************************************************/


/*****************************************************************************
1 ͷ�ļ�����
*****************************************************************************/
#include "RnicDemandDialFileIO.h"
#include "msg/kcmsmonitormsgtypes.h"


/*��¼WLAN������*/
char g_wlan_dev_name[IFNAMSIZ] = {0};
/*��¼eth0�Ƿ�Ϊlan�豸*/
char g_eth_is_lan[IFNAMSIZ] = {0};

RNIC_DIAL_MODE_STRU  g_DialMode;
/*������ⶨʱ��*/
RNIC_FLUX_TIMER  g_flux_timer;

static const struct file_operations g_stOnDemandFileOps       =
{
    .owner      = THIS_MODULE,
    .write       = RNIC_WriteOnDemandFile,
    .read        = RNIC_ReadOnDemandFile,
};

static const struct file_operations g_stIdleTimerOutFileOps   =
{
    .owner      = THIS_MODULE,
    .write       = RNIC_WriteIdleTimerOutFile,
    .read        = RNIC_ReadIdleTimerOutFile,
};

static const struct file_operations g_stDialEventReportFileOps =
{
    .owner      = THIS_MODULE,
    .write       = RNIC_WriteDialEventReportFile,
    .read        = RNIC_ReadDialEventReportFile,
};

static const struct file_operations g_stWlanDevNameOps =
{
    .owner      = THIS_MODULE,
    .write      = RNIC_WriteWlanDevNameFile,
    .read       = RNIC_ReadWlanDevNameFile,
};

static const struct file_operations g_stEthIsLanDevOps =
{
    .owner      = THIS_MODULE,
    .write      = RNIC_WriteEthIsLanDevFile,
    .read       = RNIC_ReadEthIsLanDevFile,
};



bool check_rnic_idle_timeout(void)
{
    bool bRet = false;

    bRet = g_DialMode.disconnect_time * HZ > (jiffies - g_DialMode.last_net_time) ?false:true;

    return bRet;
}

void set_rnic_last_net_time(unsigned long time)
{
    g_DialMode.last_net_time = time;
}


void rnic_idle_timer_handler(unsigned long lparam)
{
    long unsigned  int diffTimer = 0;

    if(!check_rnic_idle_timeout())
    {
        diffTimer = RNIC_NO_FLUX_EXPIRES;
        mod_timer(&g_DialMode.flux_timer, diffTimer);
    }
    else
    {     
        del_timer(&g_DialMode.flux_timer);
        if(ALLOW_EVENT_REPORT == g_DialMode.enEventReportFlag)
        {

            Rnic_SendDisonnectEvent();
        }

    }
}

void rnic_idle_timer_func(unsigned long lparam)
{
    rnic_idle_timer_handler(lparam);
}

static int rnic_idle_timer_start(unsigned int seconds)
{
    long unsigned  int diffTimer = 0;
    //reset timer
    set_rnic_last_net_time(jiffies);

    diffTimer = RNIC_NO_FLUX_EXPIRES;
    mod_timer(&g_DialMode.flux_timer, diffTimer);

    return 0;
}


void rnic_idle_timer_init(void)
{
    init_timer(&g_DialMode.flux_timer);
    g_DialMode.flux_timer.function = rnic_idle_timer_func; 
    g_DialMode.is_timer_init = true;

}

void rnic_idle_timer_delete(void)
{
    del_timer(&g_DialMode.flux_timer);
    g_DialMode.is_timer_init = false;
}

void rnic_flux_timer_handler(unsigned long lparam)
{
    if(g_DialMode.is_flow)
    {
        Rnic_SendConnectEvent();
        g_DialMode.is_flow = false;
    }
    mod_timer(&g_flux_timer.timer, jiffies + ONDEMAND_CONNECT_FLUX_CHECK_TIME * HZ);
}

void rnic_flux_timer_init(void)
{     
    init_timer(&g_flux_timer.timer);
    g_flux_timer.is_timer_init = true;
    g_flux_timer.timer.function = rnic_flux_timer_handler; 
}

void rnic_flux_timer_start(void)
{
    mod_timer(&g_flux_timer.timer, jiffies + ONDEMAND_CONNECT_FLUX_CHECK_TIME * HZ);
}

void rnic_flux_timer_delete(void)
{
    del_timer(&g_flux_timer.timer);
    g_flux_timer.is_timer_init = false;
}


/*****************************************************************************
�� �� ��  : RNIC_TransferStringToInt
��������  : ��app������ִ�ת��Ϊ����
�������  : VOS_CHAR *pcString
�������  : ��
�� �� ֵ  : VOS_UINT32
���ú���  :
��������  :

 �޸���ʷ      :
 1.��    ��   : 2012��2��10��
 �޸�����   : �����ɺ���
 
*****************************************************************************/
VOS_UINT32 RNIC_TransferStringToInt(VOS_CHAR *pcString)
{
    VOS_UINT32 ulStrLen = 0;
    VOS_UINT32 i = 0;
    VOS_UINT32 ulRst = 0;

    ulStrLen = 0;
    ulRst = 0;

    ulStrLen = strlen(pcString);

    for ( i = 0; i < ulStrLen; i++ )
    {
        if ( (pcString[i] >= '0') && (pcString[i] <= '9') )
        {
            ulRst = (ulRst * 10) + (pcString[i] - '0');
        }
    }

    return ulRst;
}


/*�ַ���������ת��*/
typedef unsigned int dsat_num_item_type;
typedef  unsigned char      byte;  
typedef enum
{                       
  ATOI_OK,              /*  conversion complete             */
  ATOI_NO_ARG,          /*  nothing to convert              */
  ATOI_OUT_OF_RANGE     /*  character or value out of range */
} atoi_enum_type;
#define MAX_VAL_NUM_ITEM 0xFFFFFFFF /*  Max value of a numeric AT parm     */
#define  UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )
#define DEC 10
/*****************************************************************************
 �� �� ��  : dsatutil_atoi
 ��������  : ʵ���ַ���������ת��Ϊ����
 �������  : val_arg_ptr ----- ָ�����ڴ洢ת�����
             s----- ԭʼ���ַ���
             r------����
 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��

    �޸�����   : �����ɺ���

*****************************************************************************/
atoi_enum_type dsatutil_atoi
(
    dsat_num_item_type *val_arg_ptr,     
    const byte *s,     
    unsigned int r    
)
{
    atoi_enum_type err_ret = ATOI_NO_ARG;
    byte c;
    dsat_num_item_type val, val_lim, dig_lim;
    
    val = 0;
    val_lim = (dsat_num_item_type) ((unsigned int)MAX_VAL_NUM_ITEM / r);
    dig_lim = (dsat_num_item_type) ((unsigned int)MAX_VAL_NUM_ITEM % r);

    if (NULL == val_arg_ptr || NULL == s)
    {
        return ATOI_NO_ARG;
    }
    while ( (c = *s++) != '\0')
    {
        if (c != ' ')
        {
            c = (byte) UPCASE (c);
            if (c >= '0' && c <= '9')
            {
                c -= '0';
            }
            else if (c >= 'A')
            {
                c -= 'A' - DEC;
            }
            else
            {
                break;
            }
            
            if (c >= r || val > val_lim
                || (val == val_lim && c > dig_lim))
            {
                err_ret = ATOI_OUT_OF_RANGE;  /*  char code too large */
                break;
            }
            else
            {
                err_ret = ATOI_OK;            /*  arg found: OK so far*/
                val = (dsat_num_item_type) (val * r + c);
            }
        }
        *val_arg_ptr =  val;
    }
  
    return err_ret;
}

/*****************************************************************************
�� �� ��  : RNIC_ReadOnDemandFile
��������  : OnDemand�����ļ���ʵ��
�������  : file --- �ļ����
buf  --- �û��ռ�
ppos --- �ļ�ƫ�ƣ�����δʹ��
�������  : ��
�� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
ssize_t RNIC_ReadOnDemandFile(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    VOS_CHAR  acModeTemp[RNIC_ONDEMAND_FILE_LEN] = {0};
    VOS_INT32 lRlst = 0;
    
    if (*ppos > 0)
    {
        printk("RNIC_ReadOnDemandFile:ppos is err!\n");
        return 0;
    }

    PS_MEM_SET(acModeTemp, 0x00, RNIC_ONDEMAND_FILE_LEN);
    snprintf((VOS_CHAR *)acModeTemp, RNIC_ONDEMAND_FILE_LEN, "%lu", g_DialMode.enDialMode);

    len = PS_MIN(len, strlen(acModeTemp));

    /*�����ں˿ռ����ݵ��û��ռ�����*/
    lRlst = copy_to_user(buf,(VOS_VOID *)acModeTemp, len);

    if (lRlst < 0)
    {
        printk("RNIC_ReadOnDemandFile:copy_to_user fail!\n");
        return -EPERM;
    }

    *ppos += len;

    return len;
}

/*****************************************************************************
�� �� ��  : RNIC_WriteOnDemandFile
��������  : OnDemand�����ļ�дʵ��
�������  : file ----- �ļ����
buf  ----- �û��ռ�����
lLength -- �û����ݳ���
ppos - ----�ļ�ƫ�ƣ�����δʹ��
�������  : ��
�� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
ssize_t RNIC_WriteOnDemandFile(struct file *file, const char __user *buf, size_t len,loff_t *ppos)
{
    VOS_CHAR  acModeTemp[RNIC_ONDEMAND_FILE_LEN] = {0};
    VOS_INT32 lRlst = 0;

    PS_MEM_SET(acModeTemp, 0x00, RNIC_ONDEMAND_FILE_LEN);
    
    if (len >= RNIC_ONDEMAND_FILE_LEN)
    {
        printk("RNIC_WriteOnDemandFile:len is %d!\n", len);
        return -ENOSPC;
    }

    /*�����û��ռ����ݵ��ں˿ռ�����*/
    lRlst = copy_from_user((VOS_VOID *)acModeTemp, (VOS_VOID *)buf, len);

    if (lRlst > 0)
    {
        printk("RNIC_WriteOnDemandFile:copy_from_user ERR!\n");
        return -EFAULT;
    }

    acModeTemp[len] = '\0';

    g_DialMode.enDialMode = RNIC_TransferStringToInt(acModeTemp);

    switch((RNIC_DIAL_MODE_ENUM)g_DialMode.enDialMode)
    {
        case DIAL_MODE_DEMAND_DISABLED:
        {
            if(g_DialMode.is_timer_init)
            {
                rnic_idle_timer_delete();
            }
            if(g_flux_timer.is_timer_init)
            {
                rnic_flux_timer_delete();
            }
            g_DialMode.is_flow = false;

            break;
        }
        
        case DIAL_MODE_DEMAND_CONNECT:
        {
            if(g_flux_timer.is_timer_init)
            {
                rnic_flux_timer_delete();
            }
            g_DialMode.is_flow = false;
            if(!g_DialMode.is_timer_init)
            {
                rnic_idle_timer_init();
            }
            rnic_idle_timer_start(g_DialMode.disconnect_time);
            break;
        }

        case DIAL_MODE_DEMAND_DISCONNECT:
        {
            if(g_DialMode.is_timer_init)
            {
                rnic_idle_timer_delete();
            }
            g_DialMode.is_flow = false;
            if(!g_flux_timer.is_timer_init)
            {
                rnic_flux_timer_init();
            }
            rnic_flux_timer_start();
            break;
        }

        default:
            printk(KERN_ERR"%sInvalid arguments g_sta_flux.wifi_ondemand_flag =%lu\n",\
                 __func__,g_DialMode.enDialMode  );
    }

    return len;
}

/*****************************************************************************
�� �� ��  : RNIC_InitOnDemandFile
��������  : OnDemand�����ļ���ʼ��
�������  : proc_dir_entry *pstParentFileDirEntry -- ��Ŀ¼�����
�������  : ��
�� �� ֵ  : ��ʼ�����

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
VOS_UINT32 RNIC_InitOnDemandFile(struct proc_dir_entry *pstParentFileDirEntry)
{
    struct proc_dir_entry *pstOnDemandEntry = NULL;

    if (VOS_NULL_PTR == pstParentFileDirEntry)
    {
        /*����OnDemand�����ļ�*/
        pstOnDemandEntry = proc_create("dial/ondemand",
                                                          RNIC_VFILE_CRT_LEVEL,
                                                          pstParentFileDirEntry,
                                                          &g_stOnDemandFileOps);

        if (VOS_NULL_PTR == pstOnDemandEntry)
        {
            printk("RNIC_InitOnDemandFile: proc_create 1 Return NULL \n");
            return VOS_ERR;
        }
    }
    else
    {
        /*����OnDemand�����ļ�*/
        pstOnDemandEntry = proc_create("ondemand",
                                                          RNIC_VFILE_CRT_LEVEL,
                                                          pstParentFileDirEntry,
                                                          &g_stOnDemandFileOps);

        if (VOS_NULL_PTR == pstOnDemandEntry)
        {
            printk("RNIC_InitOnDemandFile: proc_create 2 Return NULL \n");
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/*****************************************************************************
�� �� ��  : RNIC_ReadIdleTimerOutFile
��������  : ideltimerout�����ļ���ʵ��
�������  : file --- �ļ����
buf  --- �û��ռ�
ppos --- �ļ�ƫ�ƣ�����δʹ��
�������  : ��
�� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
ssize_t RNIC_ReadIdleTimerOutFile(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    VOS_CHAR  acModeTemp[RNIC_IDLETIMEROUT_FILE_LEN] = {0};
    VOS_INT32 lRlst = 0;

    if (*ppos > 0)
    {
        printk("RNIC_ReadIdleTimerOutFile:ppos is err!\n");
        return 0;
    }

    PS_MEM_SET(acModeTemp, 0x00, RNIC_IDLETIMEROUT_FILE_LEN);

    snprintf((VOS_CHAR *)acModeTemp, RNIC_IDLETIMEROUT_FILE_LEN, "%d", g_DialMode.disconnect_time);

    len = PS_MIN(len, strlen(acModeTemp));

    /*�����ں˿ռ����ݵ��û��ռ�����*/
    lRlst = copy_to_user(buf,(VOS_VOID *)acModeTemp, len);

    if (lRlst < 0)
    {
        printk("RNIC_ReadIdleTimerOutFile:copy_to_user fail!\n");
        return -EPERM;
    }

    *ppos += len;

    return len;
}


/*****************************************************************************
�� �� ��  : RNIC_WriteIdleTimerOutFile
��������  : idel_timerout�����ļ�дʵ��
�������  : file ----- �ļ����
buf  ----- �û��ռ�����
lLength -- �û����ݳ���
ppos - ----�ļ�ƫ�ƣ�����δʹ��
�������  : ��
�� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
ssize_t RNIC_WriteIdleTimerOutFile(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{

    VOS_CHAR  acModeTemp[RNIC_IDLETIMEROUT_FILE_LEN] = {0};
    VOS_INT32 lRlst = 0;

    unsigned int tmp_disconnect_time = 0;
 
    PS_MEM_SET(acModeTemp, 0x00, RNIC_IDLETIMEROUT_FILE_LEN);
    if (len >= RNIC_IDLETIMEROUT_FILE_LEN)
    {
        printk("RNIC_WriteIdleTimerOutFile:len is %d!\n", len);
        return -ENOSPC;
    }

    /*�����û��ռ����ݵ��ں˿ռ�����*/
    lRlst = copy_from_user((VOS_VOID *)acModeTemp, (VOS_VOID *)buf, len);

    if (lRlst > 0)
    {
        printk("RNIC_WriteIdleTimerOutFile:copy_from_user ERR!\n");
        return -EFAULT;
    }
    acModeTemp[len] = '\0';

    lRlst = dsatutil_atoi(&tmp_disconnect_time, acModeTemp, DEC);
    if (ATOI_OK != lRlst)
    {
        return len;
    }

    if(0 != tmp_disconnect_time)
    {   
        g_DialMode.disconnect_time = tmp_disconnect_time;  
    }

    return len;
}

/*****************************************************************************
�� �� ��  : RNIC_InitIdleTimerOutFile
��������  : ideltimerout�����ļ���ʼ��
�������  : proc_dir_entry *pstParentFileDirEntry -- ��Ŀ¼�����
�������  : ��
�� �� ֵ  : ��ʼ�����

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
VOS_UINT32 RNIC_InitIdleTimerOutFile(struct proc_dir_entry *pstParentFileDirEntry)
{
    struct proc_dir_entry *pstIdleTimeOutEntry = NULL;

    if (VOS_NULL_PTR == pstParentFileDirEntry )
    {
        /*����IdleTimeOut�����ļ�*/
        pstIdleTimeOutEntry = proc_create("dial/idle_timeout",
                                                          RNIC_VFILE_CRT_LEVEL,
                                                          pstParentFileDirEntry,
                                                          &g_stIdleTimerOutFileOps);

        if (VOS_NULL_PTR == pstIdleTimeOutEntry)
        {
            printk("RNIC_InitIdleTimerOutFile:pstIdleTimeOutEntry 1 is null!\n");
            return VOS_ERR;
        }
    }
    else
    {
        /*����IdleTimeOut�����ļ�*/
        pstIdleTimeOutEntry = proc_create("idle_timeout",
                                                          RNIC_VFILE_CRT_LEVEL,
                                                          pstParentFileDirEntry,
                                                          &g_stIdleTimerOutFileOps);

        if (VOS_NULL_PTR == pstIdleTimeOutEntry)
        {
            printk("RNIC_InitIdleTimerOutFile:pstIdleTimeOutEntry 2 is null!\n");
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/*****************************************************************************
�� �� ��  : RNIC_ReadDialEventReportFile
��������  : dial_event_report�����ļ���ʵ��
�������  : file --- �ļ����
buf  --- �û��ռ�
ppos --- �ļ�ƫ�ƣ�����δʹ��
�������  : ��
�� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
ssize_t RNIC_ReadDialEventReportFile(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    VOS_CHAR   acModeTemp[RNIC_EVENTFLAG_FILE_LEN] = {0};
    VOS_INT32  lRlst = 0;

    if (*ppos > 0)
    {
        printk("RNIC_ReadDialEventReportFile:ppos is err!\n");
        return 0;
    }

    PS_MEM_SET(acModeTemp, 0x00, RNIC_EVENTFLAG_FILE_LEN);

    snprintf((VOS_CHAR *)acModeTemp, RNIC_EVENTFLAG_FILE_LEN, "%lu", g_DialMode.enEventReportFlag);

    len = PS_MIN(len, strlen(acModeTemp));

    /*�����ں˿ռ����ݵ��û��ռ�����*/
    lRlst = copy_to_user(buf,(VOS_VOID *)acModeTemp, len);

    if (lRlst < 0)
    {
        printk("RNIC_ReadDialEventReportFile:copy_to_user fail!\n");
        return -EPERM;
    }

    *ppos += len;

    return len;
}


/*****************************************************************************
�� �� ��  : RNIC_WriteDialEventReportFile
��������  : dialeventreport�����ļ�дʵ��
�������  : file ----- �ļ����
buf  ----- �û��ռ�����
lLength -- �û����ݳ���
ppos - ----�ļ�ƫ�ƣ�����δʹ��
�������  : ��
�� �� ֵ  : �ɹ���ʧ��

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
ssize_t RNIC_WriteDialEventReportFile(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{

    VOS_CHAR  acModeTemp[RNIC_EVENTFLAG_FILE_LEN] = {0};
    VOS_INT32  lRlst = 0;

    PS_MEM_SET(acModeTemp, 0x00, RNIC_EVENTFLAG_FILE_LEN);

    if (len >= RNIC_EVENTFLAG_FILE_LEN)
    {
        printk("RNIC_WriteDialEventReportFile:len is %d!\n", len);
        return -ENOSPC;
    }

    /*�����û��ռ����ݵ��ں˿ռ�����*/
    lRlst = copy_from_user((VOS_VOID *)acModeTemp, (VOS_VOID *)buf, len);

    if (lRlst > 0)
    {
        printk("RNIC_WriteDialEventReportFile:copy_from_user ERR!\n");
        return -EFAULT;
    }

    acModeTemp[len] = '\0';

    g_DialMode.enEventReportFlag = RNIC_TransferStringToInt(acModeTemp);

    return len;
}
/*****************************************************************************
�� �� ��  : RNIC_InitDialEventReportFiles
��������  : proc_dir_entry *pstParentFileDirEntry -- ��Ŀ¼�����
�������  : proc_dir_entry *pstParentFileDirEntry
�������  : ��
�� �� ֵ  : ��ʼ�����

 �޸���ʷ      :
 1.��    ��   : 2012��01��30��
 ��    ��   : Huawei
 �޸�����   : �����ɺ���
 
*****************************************************************************/
VOS_UINT32 RNIC_InitDialEventReportFile(struct proc_dir_entry *pstParentFileDirEntry)
{
    struct proc_dir_entry *pstDialEventReportEntry = NULL;

    if (VOS_NULL_PTR == pstParentFileDirEntry)
    {
        /*����dial_event_report�����ļ�*/
        pstDialEventReportEntry = proc_create("dial/dial_event_report",
                                                              RNIC_VFILE_CRT_LEVEL,
                                                              pstParentFileDirEntry,
                                                              &g_stDialEventReportFileOps);

        if (VOS_NULL_PTR == pstDialEventReportEntry)
        {
            printk("RNIC_InitDialEventReportFile:pstDialEventReportEntry 1 is null!\n");
            return VOS_ERR;
        }
    }
    else
    {
        /*����dial_event_report�����ļ�*/
        pstDialEventReportEntry = proc_create("dial_event_report",
                                                              RNIC_VFILE_CRT_LEVEL,
                                                              pstParentFileDirEntry,
                                                              &g_stDialEventReportFileOps);

        if (VOS_NULL_PTR == pstDialEventReportEntry)
        {
            printk("RNIC_InitDialEventReportFile:pstDialEventReportEntry 2 is null!\n");
            return VOS_ERR;
        }
    }

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : RNIC_ReadWlanDevNameFile
 ��������  : wlan_dev_name�����ļ���ʵ��
 �������  : file --- �ļ����
             buf  --- �û��ռ�
             ppos --- �ļ�ƫ�ƣ�����δʹ��
 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��

*****************************************************************************/
ssize_t RNIC_ReadWlanDevNameFile(
    struct file                        *file,
    char __user                        *buf,
    size_t                              len,
    loff_t                             *ppos
)
{
    VOS_INT32                           lRlst;

    if (*ppos > 0)
    {
        printk("RNIC_ReadWlanDevNameFile:ppos err!\n");
        return 0;
    }

    len             = PS_MIN(len, strlen(g_wlan_dev_name));

    /*�����ں˿ռ����ݵ��û��ռ�����*/
    lRlst          = copy_to_user(buf, (VOS_VOID *)g_wlan_dev_name, len);


    if (lRlst < 0)
    {
        printk("RNIC_ReadWlanDevNameFile:copy_to_user err!\n");
        return -EPERM;
    }

    *ppos += len;

    return len;
}

/*****************************************************************************
 �� �� ��  : RNIC_WriteWlanDevNameFile
 ��������  : dialeventreport�����ļ�дʵ��
 �������  : file ----- �ļ����
             buf  ----- �û��ռ�����
             lLength -- �û����ݳ���
             ppos - ----�ļ�ƫ�ƣ�����δʹ��
 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��

*****************************************************************************/
ssize_t RNIC_WriteWlanDevNameFile(
    struct file                        *file,
    const char __user                  *buf,
    size_t                              len,
    loff_t                             *ppos
)
{
    VOS_INT32                           lRlst;
    
    if (len >= IFNAMSIZ)
    {
        printk("RNIC_WriteWlanDevNameFile:len is %d!\n",len);
        return -ENOSPC;
    }

    /*�����û��ռ����ݵ��ں˿ռ�����*/
    lRlst = copy_from_user((VOS_VOID *)g_wlan_dev_name, (VOS_VOID *)buf, len);

    if (lRlst > 0)
    {
        printk("RNIC_WriteWlanDevNameFile:copy_from_user ERR!\n");
        return -EFAULT;
    }

    g_wlan_dev_name[len] = '\0';

    return len;
}


/*****************************************************************************
 �� �� ��  : RNIC_InitWlanDevNameFile
 ��������  : proc_dir_entry *pstParentFileDirEntry -- ��Ŀ¼�����
 �������  : proc_dir_entry *pstParentFileDirEntry
 �������  : ��
 �� �� ֵ  : ��ʼ�����

*****************************************************************************/
VOS_UINT32 RNIC_InitWlanDevNameFile(struct proc_dir_entry *pstParentFileDirEntry)
{
    struct proc_dir_entry              *pstWlanDevNameEntry;

    if (VOS_NULL_PTR == pstParentFileDirEntry)
    {
        /*����wlan_dev_name�����ļ�*/
        pstWlanDevNameEntry             = proc_create("dial/wlan_dev_name",
                                                          RNIC_VFILE_CRT_LEVEL,
                                                          pstParentFileDirEntry,
                                                          &g_stWlanDevNameOps);

        if (VOS_NULL_PTR == pstWlanDevNameEntry)
        {
            printk("RNIC_InitWlanDevNameFile:pstWlanDevNameEntry 1 is null!\n");

            return VOS_ERR;
        }

    }
    else
    {

        /*����wlan_dev_name�����ļ�*/
        pstWlanDevNameEntry             = proc_create("wlan_dev_name",
                                                          RNIC_VFILE_CRT_LEVEL,
                                                          pstParentFileDirEntry,
                                                          &g_stWlanDevNameOps);

        if (VOS_NULL_PTR == pstWlanDevNameEntry)
        {
            printk("RNIC_InitWlanDevNameFile:pstWlanDevNameEntry 2 is null!\n");

            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RNIC_ReadEthIsLanDevFile
 ��������  : dial_event_report�����ļ���ʵ��
 �������  : file --- �ļ����
             buf  --- �û��ռ�
             ppos --- �ļ�ƫ�ƣ�����δʹ��
 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��

*****************************************************************************/
ssize_t RNIC_ReadEthIsLanDevFile(
    struct file                        *file,
    char __user                        *buf,
    size_t                              len,
    loff_t                             *ppos
)
{
    VOS_INT32                           lRlst;

    if (*ppos > 0)
    {
        printk("RNIC_ReadEthIsLanDevFile:ppos err!\n");
        return 0;
    }

    len             = PS_MIN(len, strlen(g_eth_is_lan));

    /*�����ں˿ռ����ݵ��û��ռ�����*/
    lRlst          = copy_to_user(buf, (VOS_VOID *)g_eth_is_lan, len);


    if (lRlst < 0)
    {
        printk("RNIC_ReadEthIsLanDevFile:copy_to_user err!\n");
        return -EPERM;
    }

    *ppos += len;

    return len;
}


/*****************************************************************************
 �� �� ��  : RNIC_WriteEthIsLanDevFile
 ��������  : dialeventreport�����ļ�дʵ��
 �������  : file ----- �ļ����
             buf  ----- �û��ռ�����
             lLength -- �û����ݳ���
             ppos - ----�ļ�ƫ�ƣ�����δʹ��
 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��
*****************************************************************************/
ssize_t RNIC_WriteEthIsLanDevFile(
    struct file                        *file,
    const char __user                  *buf,
    size_t                              len,
    loff_t                             *ppos
)
{
    VOS_INT32                           lRlst;
    
    if (len >= IFNAMSIZ)
    {
        printk("RNIC_WriteEthIsLanDevFile:len is %d!\n",len);
        return -ENOSPC;
    }

    /*�����û��ռ����ݵ��ں˿ռ�����*/
    lRlst = copy_from_user((VOS_VOID *)g_eth_is_lan, (VOS_VOID *)buf, len);

    if (lRlst > 0)
    {
        printk("RNIC_WriteEthIsLanDevFile:copy_from_user ERR!\n");
        return -EFAULT;
    }

    g_eth_is_lan[len] = '\0';

    return len;
}


/*****************************************************************************
 �� �� ��  : RNIC_InitEthIsLanDevFile
 ��������  : proc_dir_entry *pstParentFileDirEntry -- ��Ŀ¼�����
 �������  : proc_dir_entry *pstParentFileDirEntry
 �������  : ��
 �� �� ֵ  : ��ʼ�����

*****************************************************************************/
VOS_UINT32 RNIC_InitEthIsLanDevFile(struct proc_dir_entry *pstParentFileDirEntry)
{
    struct proc_dir_entry              *pstEthIsLanDevEntry;

    if (VOS_NULL_PTR == pstParentFileDirEntry)
    {
        /*����eth_is_lan�����ļ�*/
        pstEthIsLanDevEntry             = proc_create("dial/eth_is_lan",
                                                          RNIC_VFILE_CRT_LEVEL,
                                                          pstParentFileDirEntry,
                                                          &g_stEthIsLanDevOps);

        if (VOS_NULL_PTR == pstEthIsLanDevEntry)
        {
            printk("RNIC_InitEthIsLanDevFile:pstEthIsLanDevEntry 1 is null!\n");           
            return VOS_ERR;
        }

    }
    else
    {

        /*����eth_is_lan�����ļ�*/
        pstEthIsLanDevEntry             = proc_create("eth_is_lan",
                                                          RNIC_VFILE_CRT_LEVEL,
                                                          pstParentFileDirEntry,
                                                          &g_stEthIsLanDevOps);

        if (VOS_NULL_PTR == pstEthIsLanDevEntry)
        {
            printk("RNIC_InitEthIsLanDevFile:pstEthIsLanDevEntry 2 is null!\n");
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/*****************************************************************************
�� �� ��  : RNIC_InitDemandDialFile
��������  : ��ʼ�����貦�ļ�
�������  : VOS_VOID
�������  : ��
�� �� ֵ  : VOS_UINT32
���ú���  :
��������  :

 �޸���ʷ      :
 1.��    ��   : 2012��2��1��
 �޸�����   : �����ɺ���
 
*****************************************************************************/
VOS_UINT32 RNIC_InitDemandDialFile(VOS_VOID)
{
    struct proc_dir_entry              *stProcDemandDialDir = NULL;
    VOS_UINT32                          ulRlst = 0;
    struct path                         path;
    int                                 error = 0;

    memset(&path, 0, sizeof(struct path));
    /*��ʼ������ģʽȫ������*/
    g_DialMode.enDialMode = DIAL_MODE_DEMAND_DISCONNECT;
    g_DialMode.enEventReportFlag = FORBID_EVENT_REPORT ;
    g_DialMode.disconnect_time = DEFULT_RNIC_DISCONNECT_TIME;
    g_DialMode.is_flow = false;
    g_DialMode.is_timer_init = false;
    g_flux_timer.is_timer_init = false;
       
    rnic_idle_timer_init();
    rnic_flux_timer_init();
    error = kern_path("/proc/dial", LOOKUP_FOLLOW, &path);
    if (error)
    {
        stProcDemandDialDir = proc_mkdir("dial", VOS_NULL_PTR);
        if (VOS_NULL_PTR == stProcDemandDialDir)
        {
            printk("RNIC_InitDemandDialFile:proc_mkdir Fail!\n");
            return VOS_ERR;
        }
    }
    else
    {
        stProcDemandDialDir = VOS_NULL_PTR;
    }

    ulRlst = RNIC_InitOnDemandFile(stProcDemandDialDir);

    if (VOS_ERR == ulRlst)
    {
        return VOS_ERR;
    }

    ulRlst = RNIC_InitIdleTimerOutFile(stProcDemandDialDir);

    if (VOS_ERR == ulRlst)
    {
        return VOS_ERR;
    }

    ulRlst = RNIC_InitDialEventReportFile(stProcDemandDialDir);

    if (VOS_ERR == ulRlst)
    {
        return VOS_ERR;
    }
    /* �����ڵ� ��ȡWLAN֮lan�豸��*/
    ulRlst = RNIC_InitWlanDevNameFile(stProcDemandDialDir);
    if (VOS_ERR == ulRlst)
    {
        return VOS_ERR;
    }
    /*����  �ڵ��ж�eth0 �Ƿ�ΪLAN*/
    ulRlst  = RNIC_InitEthIsLanDevFile(stProcDemandDialDir);

    if (VOS_ERR == ulRlst)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_VOID Rnic_SendConnectEvent(VOS_VOID)
{
    syswatch_nl_send(ATP_MSG_MONITOR_EVT_3GPP_ONDEMAND_CONNECT, NULL, 0);    
}

VOS_VOID Rnic_SendDisonnectEvent(VOS_VOID)
{
    syswatch_nl_send(ATP_MSG_MONITOR_EVT_3GPP_ONDEMAND_DISCONN, NULL, 0);      
}


