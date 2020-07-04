/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2009-2019, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_logdef.h
  版 本 号   : 初稿
  生成日期   : D2011_07_28
  最近修改   :

******************************************************************************/
#ifndef __HI_LOGDEF_H__
#define __HI_LOGDEF_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_LOG_OUTPUTMAXLEN    2048



typedef enum
{
    HI_LOG_SYSTRACE_DISABLE_E,
    HI_LOG_SYSTRACE_ENABLE_E, 
    HI_LOG_SYSTRACE_FAIL_E, 
    HI_LOG_SYSTRACE_SUCC_E,
    HI_LOG_SYSTRACE_CNT_E, 
}hi_log_systrace_flag_e; 



typedef enum  {
    HI_LOGTYPE_DEBUG_E = 0,
    HI_LOGTYPE_SYSTRACE_E,
    HI_LOGTYPE_PRINT_E,
    HI_LOGTYPE_PRINTMEM_E,
}hi_logtype_e;

/*最多八个类型,因为DEBUG为UCHAR8类型*/
typedef enum  {
    HI_LOG_LEVEL_DBG_E = 0, /* 调试 */
    HI_LOG_LEVEL_ALM_E,     /* 告警 */
    HI_LOG_LEVEL_WRN_E,     /* 提示 */
    HI_LOG_LEVEL_INFO_E,    /* 信息 */
    HI_LOG_LEVEL_MSG_E,     /* 消息 */
    HI_LOG_LEVEL_ERR_E,     /* 错误 */
    HI_LOG_LEVEL_EVT_E,     /* 事件 */
    HI_LOG_LEVEL_RESV_E,    /* 内存打印 */
}hi_dbglevel_e; 

typedef struct  {
    hi_uint32 ui_srcmodule;
    hi_uint32 ui_maskdbgflag;   /*bit0:31,每个比特对应hi_dbglevel_e枚举*/
    hi_uchar8 uc_systraceflag;  /* 0x0: 关闭不输出; 0x1: 输出所有信息; 0x2: 只输出失败信息; 
                                   0x3: 只输出成功信息;0x4: 按照配置返回值精确匹配*/
    hi_uchar8 uc_printflag;
    hi_uchar8 uc_flag;          /*bit0:1远程打印 bit0:0本地打印*/
    hi_uchar8 uc_resv;
    hi_uint32 ui_retcode;
}hi_log_cfg_s;


#define HI_BUG()              { hi_printk("\r\n%s(%d):",__func__,__LINE__); dump_stack();}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_LOGDEF_H__ */
