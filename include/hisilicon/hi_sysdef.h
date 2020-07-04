/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��˼�뵼�����޹�˾

 ******************************************************************************
  �� �� ��   : hi_sysdef.h
  ��������   : hi_sysdef.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : D2011_07_19
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __HI_SYSDEF_H__
#define __HI_SYSDEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*******************************************************
high level protocol system defined
-------------------------------------------
31    -   28   -  20  -  12   -    8   -     0
| precode(0xF) | system | mod  | submod | cmdtype | 
-------------------------------------------
global retcode system defined
-------------------------------------------
31    -   28   -  20  -  12   -    8   -     0
| precode(0xE) | system | mod  | submod | retcode | 
********************************************************/
#define     HI_PRECODE_MAX          0xF   /* ���ǰ������*/
#define     HI_SYSTEM_MAX           0xFF  /* ���ϵͳ�� */
#define     HI_SRCMODULE_MAX        0x100 /* ���ģ���� */
#define     HI_SUBMODULE_MAX        0xF   /* �����ģ���� */
#define     HI_RETCODE_MAX          0xFF  /* ��󷵻����� */
#define     HI_CMDTYPE_MAX          0xFFF /* ��������� */

#define     HI_PRECODE_MASK         0xF   /* ��ȡǰ��������*/
#define     HI_SYSTEM_MASK          0xFF  /* ��ȡϵͳ���� */
#define     HI_SRCMODULE_MASK       0xFF  /* ��ȡģ������ */
#define     HI_SUBMODULE_MASK       0xF   /* ��ȡ��ģ������ */
#define     HI_RETCODE_MASK         0xFF  /* ��ȡ����ֵ���� */
#define     HI_CMDTYPE_MASK         0xFFF /* ��ȡ���������� */

#define     HI_PRECODE_OFFSET       28    /* ��ȡǰ����ƫ��ֵ */
#define     HI_SYSTEM_OFFSET        20    /* ��ȡϵͳƫ��ֵ */
#define     HI_SRCMODULE_OFFSET     12    /* ��ȡģ��ƫ��ֵ */
#define     HI_SUBMODULE_OFFSET     8     /* ��ȡ��ģ��ƫ��ֵ */

typedef enum  {
    HI_SYSBASE_GLB                = 0xF0000000,     /*ȫ��ͨ��*/
    HI_SYSBASE_BASIC              = 0xF1000000,     /*�ں˻������ϵͳ*/
    HI_SYSBASE_DRIVER             = 0xF2000000,     /*�ں�����ϵͳ*/
    HI_SYSBASE_SOC                = 0xF3000000,     /*�ں�SOC����ϵͳ*/
    HI_SYSBASE_SAMPLE             = 0xF4000000,     /*�ں�����ϵͳ*/
    HI_SYSBASE_TESTSET            = 0xF5000000,     /*testsetϵͳ*/
    HI_SYSBASE_APPS               = 0xF6000000,     /*�û�̬ͨ��Ӧ��ϵͳ*/
    HI_SYSBASE_voice              = 0xF7000000,     /*voiceӦ��ϵͳ*/    
}hi_sysbase_e;

typedef enum  {
    HI_SRCMODULE_KBASIC_DEV       = ( HI_SYSBASE_BASIC | 0x00001000),
    HI_SRCMODULE_KBASIC_HLP       = ( HI_SYSBASE_BASIC | 0x00002000),
    HI_SRCMODULE_KBASIC_LOG       = ( HI_SYSBASE_BASIC | 0x00003000),
    HI_SRCMODULE_KBASIC_MSG       = ( HI_SYSBASE_BASIC | 0x00004000),
    HI_SRCMODULE_KBASIC_NETLINK   = ( HI_SYSBASE_BASIC | 0x00005000),
    HI_SRCMODULE_KBASIC_OS        = ( HI_SYSBASE_BASIC | 0x00006000),
    HI_SRCMODULE_KBASIC_QUEUE     = ( HI_SYSBASE_BASIC | 0x00007000),
    HI_SRCMODULE_KBASIC_TIMER     = ( HI_SYSBASE_BASIC | 0x00008000),
    HI_SRCMODULE_KBASIC_HSAH      = ( HI_SYSBASE_BASIC | 0x00009000),
}hi_kbasic_e;

typedef enum 
{
    HI_SRCMODULE_UBASIC_UBASIC    = ( HI_SYSBASE_BASIC | 0x0000A000),    
    HI_SRCMODULE_UBASIC_UOS       = ( HI_SYSBASE_BASIC | 0x0000B000),
    HI_SRCMODULE_UBASIC_CLI       = ( HI_SYSBASE_BASIC | 0x0000C000),
}hi_ubasic_e;

typedef enum  {
    HI_SRCMODULE_KDRIVER_LED      = ( HI_SYSBASE_DRIVER | 0x00001000),
    HI_SRCMODULE_KDRIVER_LSW      = ( HI_SYSBASE_DRIVER | 0x00002000),
    HI_SRCMODULE_KDRIVER_PHY      = ( HI_SYSBASE_DRIVER | 0x00004000),
    HI_SRCMODULE_KDRIVER_SCI      = ( HI_SYSBASE_DRIVER | 0x00003000),
    HI_SRCMODULE_KDRIVER_VOIP     = ( HI_SYSBASE_DRIVER | 0x00005000),
    HI_SRCMODULE_KDRIVER_CODEC    = ( HI_SYSBASE_DRIVER | 0x00006000),    
}hi_driver_e;

typedef enum  {
    HI_SRCMODULE_KSOC_ADAPTER       = ( HI_SYSBASE_SOC | 0x00001000),
    HI_SRCMODULE_KSOC_SDK           = ( HI_SYSBASE_SOC | 0x00002000),
}hi_ksoc_e;

typedef enum  {
    HI_SUBMODULE_KSOC_SDK_L0        = ( HI_SRCMODULE_KSOC_SDK | 0x00000800),  
    HI_SUBMODULE_KSOC_SDK_L1        = ( HI_SRCMODULE_KSOC_SDK | 0x00000600),
    HI_SUBMODULE_KSOC_SDK_L2        = ( HI_SRCMODULE_KSOC_SDK | 0x00000A00),
}hi_ksoc_submod_e;

typedef enum  {
    HI_SUBMODULE_KSOC_L3IF          = ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000001), 
    HI_SUBMODULE_KSOC_SDK_QOS       = ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000002),
    HI_SUBMODULE_KSOC_TBL_MNG       = ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000003),
    HI_SUBMODULE_KSOC_SDK_CHIP      = ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000004),
    HI_SUBMODULE_KSOC_SDK_PORT      = ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000005),
    HI_SUBMODULE_KSOC_SDK_SECURITY  = ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000006),
    HI_SUBMODULE_KSOC_SDK_FLOW      = ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000007),
    HI_SUBMODULE_KSOC_SDK_L2_IN_LEV1= ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000008),
    HI_SUBMODULE_KSOC_SDK_L3        = ( HI_SUBMODULE_KSOC_SDK_L1 | 0x00000009),   
    
}hi_ksoc_l1_submod_e;

typedef enum  {
    HI_SRCMODULE_KSOC_ADPT_EFUSE    = ( HI_SRCMODULE_KSOC_ADAPTER | 0x00000100),
    HI_SRCMODULE_KSOC_ADPT_ETH      = ( HI_SRCMODULE_KSOC_ADAPTER | 0x00000200),
    HI_SRCMODULE_KSOC_ADPT_GPIO     = ( HI_SRCMODULE_KSOC_ADAPTER | 0x00000300),
    HI_SRCMODULE_KSOC_ADPT_HW       = ( HI_SRCMODULE_KSOC_ADAPTER | 0x00000400),
    HI_SRCMODULE_KSOC_ADPT_MDIO     = ( HI_SRCMODULE_KSOC_ADAPTER | 0x00000500),
    HI_SRCMODULE_KSOC_ADPT_MISC     = ( HI_SRCMODULE_KSOC_ADAPTER | 0x00000600),
    HI_SRCMODULE_KSOC_ADPT_PHY      = ( HI_SRCMODULE_KSOC_ADAPTER | 0x00000700),
    HI_SRCMODULE_KSOC_ADPT_SPI      = ( HI_SRCMODULE_KSOC_ADAPTER | 0x00000800),
}hi_adapter_kspace_e;

typedef enum 
{
    HI_SRCMODULE_APPS_CLI         = ( HI_SYSBASE_APPS | 0x00001000),
    HI_SRCMODULE_APPS_SYSTEM      = ( HI_SYSBASE_APPS | 0x00002000),
    HI_SRCMODULE_APPS_XPON        = ( HI_SYSBASE_APPS | 0x00003000),
    HI_SRCMODULE_APPS_CFE         = ( HI_SYSBASE_APPS | 0x00004000),
}hi_apps_e;

typedef enum 
{
    HI_SUBMODULE_CFE_FW   = ( HI_SRCMODULE_APPS_CFE | 0x00000100),
    HI_SUBMODULE_CFE_LRN  = ( HI_SRCMODULE_APPS_CFE | 0x00000200),
    HI_SUBMODULE_CFE_RES  = ( HI_SRCMODULE_APPS_CFE | 0x00000300),
    HI_SUBMODULE_CFE_SRV  = ( HI_SRCMODULE_APPS_CFE | 0x00000400),
} hi_cfe_e;


typedef enum 
{
    HI_SUBMODULE_CFE_RES_HOOK   = ( HI_SUBMODULE_CFE_RES | 0x00000010),
    HI_SUBMODULE_CFE_RES_INTF   = ( HI_SUBMODULE_CFE_RES | 0x00000020),
    HI_SUBMODULE_CFE_RES_LINK   = ( HI_SUBMODULE_CFE_RES | 0x00000030),
    HI_SUBMODULE_CFE_RES_MNG    = ( HI_SUBMODULE_CFE_RES | 0x00000040),
    HI_SUBMODULE_CFE_RES_PKT    = ( HI_SUBMODULE_CFE_RES | 0x00000050),   
} hi_res_e;

typedef enum 
{
    HI_SUBMODULE_CFE_LRN_BR     = ( HI_SUBMODULE_CFE_LRN | 0x00000010),
    HI_SUBMODULE_CFE_LRN_MC     = ( HI_SUBMODULE_CFE_LRN | 0x00000020),
    HI_SUBMODULE_CFE_LRN_NAPT   = ( HI_SUBMODULE_CFE_LRN | 0x00000030),
    HI_SUBMODULE_CFE_LRN_QOS    = ( HI_SUBMODULE_CFE_LRN | 0x00000040),
    HI_SUBMODULE_CFE_LRN_RT     = ( HI_SUBMODULE_CFE_LRN | 0x00000050),  
    HI_SUBMODULE_CFE_LRN_TNL    = ( HI_SUBMODULE_CFE_LRN | 0x00000060),  
    HI_SUBMODULE_CFE_LRN_IPSEC  = ( HI_SUBMODULE_CFE_LRN | 0x00000070),       
} hi_lrn_e;


typedef enum 
{
    HI_SUBMODULE_CFE_FW_DELIVER   = ( HI_SUBMODULE_CFE_FW | 0x00000010),
    HI_SUBMODULE_CFE_FW_SW_MC     = ( HI_SUBMODULE_CFE_FW | 0x00000020), 
    HI_SUBMODULE_CFE_FW_SW_NORMAL = ( HI_SUBMODULE_CFE_FW | 0x00000030), 
    HI_SUBMODULE_CFE_FW_SW_BONDING = ( HI_SUBMODULE_CFE_FW | 0x00000040), 
         
} hi_fw_e;

typedef enum 
{
    HI_SUBMODULE_CFE_SRV_USB  = ( HI_SUBMODULE_CFE_SRV | 0x00000010),
    HI_SUBMODULE_CFE_SRV_WIFI = ( HI_SUBMODULE_CFE_SRV | 0x00000020),
    HI_SUBMODULE_CFE_SRV_MARK = ( HI_SUBMODULE_CFE_SRV | 0x00000030),    
    HI_SUBMODULE_CFE_SRV_QOS  = ( HI_SUBMODULE_CFE_SRV | 0x00000040),    
} hi_srv_e;

typedef enum 
{
    HI_SUBMODULE_TESTSET_PORT   = ( HI_SYSBASE_TESTSET | 0x00001000),
    HI_SUBMODULE_TESTSET_POWER  = ( HI_SYSBASE_TESTSET | 0x00002000),
    HI_SUBMODULE_TESTSET_SDK    = ( HI_SYSBASE_TESTSET | 0x00003000),
} hi_testset_e;

typedef enum 
{
    HI_SRCMODULE_VENDOR_CODEC     = ( HI_SYSBASE_voice | 0x00001000),
    HI_SUBMODULE_VENDOR_SOC_VOIP  = ( HI_SYSBASE_voice | 0x00002000),
} hi_voice_e;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_SYSDEF_H__ */


