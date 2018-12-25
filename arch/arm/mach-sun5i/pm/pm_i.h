#ifndef _PM_I_H
#define _PM_I_H

/*
 * Copyright (c) 2011-2015 yanggq.young@newbietech.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#include <mach/platform.h>
#include <mach/ccmu_regs.h>
#include "pm.h"

#include "standby/super/super_clock.h"
#include "standby/super/super_power.h"
#include "standby/super/super_twi.h"


/* define register for interrupt controller */
typedef struct __MEM_INT_REG{

    volatile __u32   Vector;
    volatile __u32   BaseAddr;
    volatile __u32   reserved0;
    volatile __u32   NmiCtrl;

    volatile __u32   IrqPend[3];
    volatile __u32   reserved1;

    volatile __u32   FiqPend[3];
    volatile __u32   reserved2;

    volatile __u32   TypeSel[3];
    volatile __u32   reserved3;

    volatile __u32   IrqEn[3];
    volatile __u32   reserved4;

    volatile __u32   IrqMask[3];
    volatile __u32   reserved5;

    volatile __u32   IrqResp[3];
    volatile __u32   reserved6;

    volatile __u32   IrqForce[3];
    volatile __u32   reserved7;

    volatile __u32   IrqPrio[5];
} __mem_int_reg_t;


//define timer controller registers
typedef struct __MEM_TMR_REG
{
    // offset:0x00
    volatile __u32   IntCtl;
    volatile __u32   IntSta;
    volatile __u32   reserved0[2];
    // offset:0x10
    volatile __u32   Tmr0Ctl;
    volatile __u32   Tmr0IntVal;
    volatile __u32   Tmr0CntVal;
    volatile __u32   reserved1;
    // offset:0x20
    volatile __u32   Tmr1Ctl;
    volatile __u32   Tmr1IntVal;
    volatile __u32   Tmr1CntVal;
    volatile __u32   reserved2;
    // offset:0x30
    volatile __u32   Tmr2Ctl;
    volatile __u32   Tmr2IntVal;
    volatile __u32   Tmr2CntVal;
    volatile __u32   reserved3;
    // offset:0x40
    volatile __u32   Tmr3Ctl;
    volatile __u32   Tmr3IntVal;
    volatile __u32   reserved4[2];
    // offset:0x50
    volatile __u32   Tmr4Ctl;
    volatile __u32   Tmr4IntVal;
    volatile __u32   Tmr4CntVal;
    volatile __u32   reserved5;
    // offset:0x60
    volatile __u32   Tmr5Ctl;
    volatile __u32   Tmr5IntVal;
    volatile __u32   Tmr5CntVal;
    volatile __u32   reserved6[5];
    // offset:0x80
    volatile __u32   AvsCtl;
    volatile __u32   Avs0Cnt;
    volatile __u32   Avs1Cnt;
    volatile __u32   AvsDiv;
    // offset:0x90
    volatile __u32   DogCtl;
    volatile __u32   DogMode;
    volatile __u32   reserved7[2];
    // offset:0xa0
    volatile __u32   Cnt64Ctl;
    volatile __u32   Cnt64Lo;
    volatile __u32   Cnt64Hi;
    volatile __u32   reserved8[21];
    // offset:0x100
    volatile __u32   LoscCtl;
    volatile __u32   RtcYMD;
    volatile __u32   RtcHMS;
    volatile __u32   RtcDHMS;
    // offset:0x110
    volatile __u32   AlarmWHMS;
    volatile __u32   AlarmEn;
    volatile __u32   AlarmIrqEn;
    volatile __u32   AlarmIrqSta;
    // offset:0x120
    volatile __u32   TmrGpReg[4];

} __mem_tmr_reg_t;

typedef struct __MEM_TWIC_REG
{
    volatile __u32 reg_saddr;
    volatile __u32 reg_xsaddr;
    volatile __u32 reg_data;
    volatile __u32 reg_ctl;
    volatile __u32 reg_status;
    volatile __u32 reg_clkr;
    volatile __u32 reg_reset;
    volatile __u32 reg_efr;
    volatile __u32 reg_lctl;

} __mem_twic_reg_t;

#ifdef CONFIG_ARCH_SUN4I
#define INT_REG_LENGTH	((0x90+0x4)>>2)
#define GPIO_REG_LENGTH	((0x218+0x4)>>2)
#define SRAM_REG_LENGTH	((0x94+0x4)>>2)
#elif defined CONFIG_ARCH_SUN5I
#define INT_REG_LENGTH	((0x94+0x4)>>2)
#define GPIO_REG_LENGTH	((0x218+0x4)>>2)
#define SRAM_REG_LENGTH	((0x94+0x4)>>2)
#endif

struct int_state{
	//__u32    IrqEnReg[3], IrqMaskReg[3], IrqSelReg[3];
	__mem_int_reg_t  *IntcReg;
	__u32 int_reg_back[INT_REG_LENGTH];
};

struct clk_state{
	__ccmu_reg_list_t   *CmuReg;
	__u32    ccu_reg_back[15];
};

struct tmr_state{
	__mem_tmr_reg_t  *TmrReg;
	__u32 TmrIntCtl, Tmr0Ctl, Tmr0IntVal, Tmr0CntVal, Tmr1Ctl, Tmr1IntVal, Tmr1CntVal;
};

struct twi_state{
	__mem_twic_reg_t *twi_reg;
	__u32 twi_reg_backup[7];
};

struct gpio_state{
	__u32 gpio_reg_back[GPIO_REG_LENGTH];
};

struct sram_state{
	__u32 sram_reg_back[SRAM_REG_LENGTH];
};

//save module state
__s32 mem_int_save(struct int_state *pint_state);
__s32 mem_int_restore(struct int_state *pint_state);
__s32 mem_clk_save(struct clk_state *pclk_state);
__s32 mem_clk_restore(struct clk_state *pclk_state);
__s32 mem_tmr_save(struct tmr_state *ptmr_state);
__s32 mem_tmr_restore(struct tmr_state *ptmr_state);
__s32 mem_twi_save(struct twi_state *ptwi_state);
__s32 mem_twi_restore(struct twi_state *ptwi_state);
__s32 mem_gpio_save(struct gpio_state *pgpio_state);
__s32 mem_gpio_restore(struct gpio_state *pgpio_state);
__s32 mem_sram_save(struct sram_state *psram_state);
__s32 mem_sram_restore(struct sram_state *psram_state);
__s32 mem_ccu_save(__ccmu_reg_list_t *pReg);
__s32 mem_ccu_restore(__ccmu_reg_list_t *pReg);


#endif /*_PM_I_H*/
