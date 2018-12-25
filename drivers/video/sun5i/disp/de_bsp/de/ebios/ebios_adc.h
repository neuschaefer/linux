#ifndef __EBIOS_TP_H__
#define __EBIOS_TP_H__

#include "../../bsp_display.h"

#define TP_CTRL0               (0x00)
#define TP_CTRL1               (0x04)
#define TP_CTRL2               (0x08)
#define TP_CTRL3               (0x0c)
#define TP_INT_FIFOC           (0x10)
#define TP_INT_FIFOS           (0x14)
#define TP_TPR                 (0x18)
#define TP_CDAT                (0x1c)
#define TEMP_DATA              (0x20)
#define TP_DATA                (0x24)

#define T_MAX                   50  //温度-阻值映射表范围:0-49

extern __u32 tp_reg_base;

#define DE_BE_GET_REG_BASE    (tp_reg_base)

#define ADC_WUINT8(offset,value)             (*((volatile __u8 *)(DE_BE_GET_REG_BASE + offset))=(value))
#define ADC_RUINT8(offset)                   (*((volatile __u8 *)(DE_BE_GET_REG_BASE + offset)))
#define ADC_WUINT16(offset,value)            (*((volatile __u16 *)(DE_BE_GET_REG_BASE + offset))=(value))
#define ADC_RUINT16(offset)                  (*((volatile __u16 *)(DE_BE_GET_REG_BASE + offset)))
#define ADC_WUINT32(offset,value)            (*((volatile __u32 *)(DE_BE_GET_REG_BASE + offset))=(value))
#define ADC_RUINT32(offset)                  (*((volatile __u32 *)(DE_BE_GET_REG_BASE + offset)))

typedef struct {
    __bool              adc_used;
    
    struct task_struct *read_ADC_value_task;    
    spinlock_t          lock[1];                
    __u32               ADC_value;              //ADC读数
    __s32               R_div;                  //与热敏电阻串联的分压电阻阻值,单位:欧姆
    __s32               vcc_vol;                //VCC电压，单位:毫伏
    __s32               adc_compensate_vol;     //ADC补偿电压，需要根据测试数据进行调试
    
    __u32               TR_map_table[T_MAX];    //热敏电阻的温度-阻值映射表
    __s32               T_ref;                  //热敏电阻参考温度,单位:℃
    __s32               R_ref;                  //热敏电阻在参考温度下的阻值,单位:欧姆
    __s32               B_constant;             //热敏电阻B值
}adc_para_t;

__s32 ADC_Set_Reg_Base(__u32 address);
__s32 ADC_Get_Reg_Base(void);
__u32 ADC_init(void);
void  ADC_exit(void);
__u32 ADC_Get_temperature(void);

#endif
