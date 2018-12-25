/*
 ***************************************************************************************************
 *                                               LINUX-KERNEL
 *                                   ReuuiMlla Linux Platform Develop Kits
 *                                              Kernel Module
 *
 *                                  (c) Copyright 2006-2011, pannan China
 *                                           All Rights Reserved
 *
 * File    : pmu_dvfs.c
 * By      : pannan/porewel
 * Version : v1.0
 * Date    : 2011-09-05
 * Descript: pmu dvfs process for cpu frequency
 * Update  : date                auther      ver     notes
 ***************************************************************************************************
*/

#include <asm/io.h>
#include <linux/kernel.h>
#include <linux/delay.h>

#include "pmu_dvfs.h"
#include "pmu_regs.h"
#include "cpu-freq.h"

static __pmu_reg_list_t *PmuReg;

static __u32 reg_val = 0;

int VF_Table0[19]={10,12,13,14,240,300,432,480,576,600,
			       624,744,864,912,1008,1056,1104,1176,1200};
                //1.2/1.25/1.3/1.35/1.4/1.45/1.5/1.55/1.6

int VF_Table1[19];
int VF_Table2[19];
int VF_Table3[19];


/*
 ***************************************************************************************************
 *                           pmu_dvfs_vftable_init
 *
 *Description: init vf-table 0,1,2,3
 *
 *Arguments  : none
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int __init pmu_dvfs_vftable_init(void)
{
    int i;
    for(i=12;i<19;i++)
	{
		VF_Table1[i] = VF_Table0[i] + 48;
		VF_Table2[i] = VF_Table1[i] + 48;
		VF_Table3[i] = VF_Table0[i] - 48;
	}

	for(i=0;i<12;i++)
	{
		VF_Table1[i] = VF_Table0[i] + 1;
		VF_Table2[i] = VF_Table1[i] + 1;
		VF_Table3[i] = VF_Table0[i] - 2;
	}

    return 0;
}


/*
 ***************************************************************************************************
 *                           __ccu_pll6_enable
 *
 *Description: set pio apb clock pass and twi0 apb clock pass
 *
 *Arguments  : none
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int  __ccu_pll6_enable(void)
{
    /* enable ccu pll6, speed factor detector start when set bit31 to 1 */
    reg_val = readl(CCU_PLL6) | (1 << 31);
    writel(reg_val, CCU_PLL6);
    return 0;
}


/*
 ***************************************************************************************************
 *                           __ccu_ahb_from_pll6
 *
 *Description: set apb clock source from pll6
 *
 *Arguments  : none
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
int __ccu_ahb_from_pll6(void)
{
    reg_val = (readl(CCU_CPU_AHB_APB) & ~(0xF<<4)) | (0x9<<4);//AHB CLK SRC /div
    writel(reg_val,CCU_CPU_AHB_APB);

    reg_val = (readl(CCU_CPU_AHB_APB) & ~(0x3<<8)) | (0x0<<8);//APB0
    writel(reg_val,CCU_CPU_AHB_APB);

    return 0;
}


/*
 ***************************************************************************************************
 *                           __ccu_ahb_from_axi
 *
 *Description: set apb clock source from axi
 *
 *Arguments  : none
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
int __ccu_ahb_from_axi(void)
{
    reg_val = (readl(CCU_CPU_AHB_APB) & ~(0xF<<4)) | (0x0<<4);//AHB CLK SRC /div
    writel(reg_val,CCU_CPU_AHB_APB);

    reg_val = (readl(CCU_CPU_AHB_APB) & ~(0x3<<8)) | (0x0<<8);//APB0
    writel(reg_val,CCU_CPU_AHB_APB);

    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_enable
 *
 * Description: enable pmu dvfs.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
int pmu_dvfs_enable(void)
{
    PmuReg->PmuCtl0.DvfsEn = 1;
    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_disable
 *
 * Description: disable pmu dvfs.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
int pmu_dvfs_disable(void)
{
    PmuReg->PmuCtl0.DvfsEn = 0;
    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_mode_enable
 *
 * Description: enable twi dvfs mode.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
int pmu_dvfs_mode_enable(void)
{
    PmuReg->PmuCtl0.DvfsModEn = 1;
    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_mode_disable
 *
 * Description: disable twi dvfs mode.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
int pmu_dvfs_mode_disable(void)
{
    PmuReg->PmuCtl0.DvfsModEn = 0;
    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_axi_auto_swt_enable
 *
 * Description: enable axi auto switch
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int pmu_dvfs_axi_auto_swt_enable(void)
{
    PmuReg->PmuCtl0.AxiAutoSwtEn = 1;
    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_axi_auto_swt_disable
 *
 * Description: disable axi auto switch
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int pmu_dvfs_axi_auto_swt_disable(void)
{
    PmuReg->PmuCtl0.AxiAutoSwtEn = 0;
    return 0;
}


#if 0
/*
 ***************************************************************************************************
 *                                      pmu_dvfs_spd_det_enable
 *
 *Description: enable speed detect
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_spd_det_enable(void)
{
	PmuReg->PmuCtl0.SpdDetEn = 1;
	return 0;
}
#endif


/*
 ***************************************************************************************************
 *                                      pmu_dvfs_spd_det_disable
 *
 *Description: disable speed detect
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_spd_det_disable(void)
{
	PmuReg->PmuCtl0.SpdDetEn = 0;
	return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_volt_chg_enable
 *
 * Description: enable pmu dvfs voltage change.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int pmu_dvfs_volt_chg_enable(void)
{
    PmuReg->PmuCtl0.VtChgEn = 1;
    return 0;
}


#if 0
/*
 ***************************************************************************************************
 *                                     pmu_dvfs_volt_chg_disable
 *
 * Description: disable pmu dvfs voltage change.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int pmu_dvfs_volt_chg_disable(void)
{
    PmuReg->PmuCtl0.VtChgEn = 0;
    return 0;
}
#endif


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_smooth_enable
 *
 * Description: pmu dvfs smooth enable.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
int pmu_dvfs_smooth_enable(void)
{
    PmuReg->PmuCtl0.SmthEn = 1;
    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_smooth_disable
 *
 * Description: pmu dvfs smooth disable.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
int pmu_dvfs_smooth_disable(void)
{
    PmuReg->PmuCtl0.SmthEn = 0;
    return 0;
}


#if 0
/*
 ***************************************************************************************************
 *                                      pmu_vtchg_set
 *
 *Description: cpuvdd voltage set
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_vtchg_set(__u32 address, __u32 value)
{
    /*cpuvdd set*/
    PmuReg->PmuCpuvddAddr.CpuvddCtrlRegAddr = address;
    PmuReg->PmuCpuvddVal.CpuvddDefVal = value;
    PmuReg->PmuCtl2.VtSetEn = 1;

    /*wait for voltage setting finished*/
    while(PmuReg->PmuCtl2.VtSetEn == 1);

    return 0;
}
#endif


#if 0
/*
 ***************************************************************************************************
 *                                      pmu_vtchg_independent
 *
 *Description: corevdd and cpuvdd voltage change indepently
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_vtchg_independent(__u32 cpuvdd_value, __u32 corevdd_value)
{
    /* dvfs on for aw1623,voltage change disable */
    pmu_dvfs_enable();
    PmuReg->PmuCtl0.VtChgEn = 0;
    /* corevdd(intvdd) set addr:0x27 */
    pmu_vtchg_set(0x27,corevdd_value);
    /* cpuvdd set (switch to cpuvdd) addr:0x23 */
    pmu_vtchg_set(0x23,cpuvdd_value);
    pmu_dvfs_disable();

    return 0;
}
#endif


#if 0
/*
 ***************************************************************************************************
 *                                  pmu_dvfs_vftable_range
 *
 *Description: pmu vf table range
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_vftable_range(__u32 range0, __u32 range1, __u32 range2)
{
	PmuReg->PmuVFTRng.VfTableRng0 = range0;
	PmuReg->PmuVFTRng.VfTableRng1 = range1;
	PmuReg->PmuVFTRng.VfTableRng2 = range2;

	return 0;
}
#endif

/*
 ***************************************************************************************************
 *                           pmu_dvfs_set_vftable_index
 *
 *Description: set vf-table index 0,1,2,3
 *
 *Arguments  : none
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_set_vftable_index(void)
{
    /* set v-f table index 0 */
	PmuReg->PmuVFTIdx.VfTableIdx   = 0;
    PmuReg->PmuVFT0.CpuMaxFreq070  = VF_Table0[0];
    PmuReg->PmuVFT1.CpuMaxFreq075  = VF_Table0[1];
    PmuReg->PmuVFT2.CpuMaxFreq080  = VF_Table0[2];
    PmuReg->PmuVFT3.CpuMaxFreq085  = VF_Table0[3];
    PmuReg->PmuVFT4.CpuMaxFreq090  = VF_Table0[4];
    PmuReg->PmuVFT5.CpuMaxFreq095  = VF_Table0[5];
    PmuReg->PmuVFT6.CpuMaxFreq100  = VF_Table0[6];
    PmuReg->PmuVFT7.CpuMaxFreq105  = VF_Table0[7];
    PmuReg->PmuVFT8.CpuMaxFreq110  = VF_Table0[8];
    PmuReg->PmuVFT9.CpuMaxFreq115  = VF_Table0[9];
    PmuReg->PmuVFT10.CpuMaxFreq120 = VF_Table0[10];
    PmuReg->PmuVFT11.CpuMaxFreq125 = VF_Table0[11];
    PmuReg->PmuVFT12.CpuMaxFreq130 = VF_Table0[12];
    PmuReg->PmuVFT13.CpuMaxFreq135 = VF_Table0[13];
    PmuReg->PmuVFT14.CpuMaxFreq140 = VF_Table0[14];
    PmuReg->PmuVFT15.CpuMaxFreq145 = VF_Table0[15];
    PmuReg->PmuVFT16.CpuMaxFreq150 = VF_Table0[16];
    PmuReg->PmuVFT17.CpuMaxFreq155 = VF_Table0[17];
    PmuReg->PmuVFT18.CpuMaxFreq160 = VF_Table0[18];

    /* set v-f table index 1 */
	PmuReg->PmuVFTIdx.VfTableIdx   = 1;
    PmuReg->PmuVFT0.CpuMaxFreq070  = VF_Table1[0];
    PmuReg->PmuVFT1.CpuMaxFreq075  = VF_Table1[1];
    PmuReg->PmuVFT2.CpuMaxFreq080  = VF_Table1[2];
    PmuReg->PmuVFT3.CpuMaxFreq085  = VF_Table1[3];
    PmuReg->PmuVFT4.CpuMaxFreq090  = VF_Table1[4];
    PmuReg->PmuVFT5.CpuMaxFreq095  = VF_Table1[5];
    PmuReg->PmuVFT6.CpuMaxFreq100  = VF_Table1[6];
    PmuReg->PmuVFT7.CpuMaxFreq105  = VF_Table1[7];
    PmuReg->PmuVFT8.CpuMaxFreq110  = VF_Table1[8];
    PmuReg->PmuVFT9.CpuMaxFreq115  = VF_Table1[9];
    PmuReg->PmuVFT10.CpuMaxFreq120 = VF_Table1[10];
    PmuReg->PmuVFT11.CpuMaxFreq125 = VF_Table1[11];
    PmuReg->PmuVFT12.CpuMaxFreq130 = VF_Table1[12];
    PmuReg->PmuVFT13.CpuMaxFreq135 = VF_Table1[13];
    PmuReg->PmuVFT14.CpuMaxFreq140 = VF_Table1[14];
    PmuReg->PmuVFT15.CpuMaxFreq145 = VF_Table1[15];
    PmuReg->PmuVFT16.CpuMaxFreq150 = VF_Table1[16];
    PmuReg->PmuVFT17.CpuMaxFreq155 = VF_Table1[17];
    PmuReg->PmuVFT18.CpuMaxFreq160 = VF_Table1[18];

    /* set v-f table index 2 */
	PmuReg->PmuVFTIdx.VfTableIdx   = 2;
    PmuReg->PmuVFT0.CpuMaxFreq070  = VF_Table2[0];
    PmuReg->PmuVFT1.CpuMaxFreq075  = VF_Table2[1];
    PmuReg->PmuVFT2.CpuMaxFreq080  = VF_Table2[2];
    PmuReg->PmuVFT3.CpuMaxFreq085  = VF_Table2[3];
    PmuReg->PmuVFT4.CpuMaxFreq090  = VF_Table2[4];
    PmuReg->PmuVFT5.CpuMaxFreq095  = VF_Table2[5];
    PmuReg->PmuVFT6.CpuMaxFreq100  = VF_Table2[6];
    PmuReg->PmuVFT7.CpuMaxFreq105  = VF_Table2[7];
    PmuReg->PmuVFT8.CpuMaxFreq110  = VF_Table2[8];
    PmuReg->PmuVFT9.CpuMaxFreq115  = VF_Table2[9];
    PmuReg->PmuVFT10.CpuMaxFreq120 = VF_Table2[10];
    PmuReg->PmuVFT11.CpuMaxFreq125 = VF_Table2[11];
    PmuReg->PmuVFT12.CpuMaxFreq130 = VF_Table2[12];
    PmuReg->PmuVFT13.CpuMaxFreq135 = VF_Table2[13];
    PmuReg->PmuVFT14.CpuMaxFreq140 = VF_Table2[14];
    PmuReg->PmuVFT15.CpuMaxFreq145 = VF_Table2[15];
    PmuReg->PmuVFT16.CpuMaxFreq150 = VF_Table2[16];
    PmuReg->PmuVFT17.CpuMaxFreq155 = VF_Table2[17];
    PmuReg->PmuVFT18.CpuMaxFreq160 = VF_Table2[18];

    /* set v-f table index 3 */
	PmuReg->PmuVFTIdx.VfTableIdx   = 3;
    PmuReg->PmuVFT0.CpuMaxFreq070  = VF_Table3[0];
    PmuReg->PmuVFT1.CpuMaxFreq075  = VF_Table3[1];
    PmuReg->PmuVFT2.CpuMaxFreq080  = VF_Table3[2];
    PmuReg->PmuVFT3.CpuMaxFreq085  = VF_Table3[3];
    PmuReg->PmuVFT4.CpuMaxFreq090  = VF_Table3[4];
    PmuReg->PmuVFT5.CpuMaxFreq095  = VF_Table3[5];
    PmuReg->PmuVFT6.CpuMaxFreq100  = VF_Table3[6];
    PmuReg->PmuVFT7.CpuMaxFreq105  = VF_Table3[7];
    PmuReg->PmuVFT8.CpuMaxFreq110  = VF_Table3[8];
    PmuReg->PmuVFT9.CpuMaxFreq115  = VF_Table3[9];
    PmuReg->PmuVFT10.CpuMaxFreq120 = VF_Table3[10];
    PmuReg->PmuVFT11.CpuMaxFreq125 = VF_Table3[11];
    PmuReg->PmuVFT12.CpuMaxFreq130 = VF_Table3[12];
    PmuReg->PmuVFT13.CpuMaxFreq135 = VF_Table3[13];
    PmuReg->PmuVFT14.CpuMaxFreq140 = VF_Table3[14];
    PmuReg->PmuVFT15.CpuMaxFreq145 = VF_Table3[15];
    PmuReg->PmuVFT16.CpuMaxFreq150 = VF_Table3[16];
    PmuReg->PmuVFT17.CpuMaxFreq155 = VF_Table3[17];
    PmuReg->PmuVFT18.CpuMaxFreq160 = VF_Table3[18];

    return 0;
}


/*
 ***************************************************************************************************
 *                      V-F Table Register Valid control bit
 *
 * Description: enable pmu dvfs.
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int	pmu_dvfs_vftable_13_validctr(__u32 v)
{
	reg_val = readl(SW_VA_PMU_IO_BASE+0xcc);
    reg_val = (reg_val & ~(1 << 0)) | (v << 0) | (0x1623 << 16);
    writel(reg_val, SW_VA_PMU_IO_BASE+0xcc);

	return 0;
}


static int	pmu_dvfs_vftable_14_validctr(__u32 v)
{
	reg_val = readl(SW_VA_PMU_IO_BASE+0xcc);
    reg_val = (reg_val & ~(1 << 1)) | (v << 1) | (0x1623 << 16);
    writel(reg_val, SW_VA_PMU_IO_BASE+0xcc);

	return 0;
}


static int	pmu_dvfs_vftable_15_validctr(__u32 v)
{
	reg_val = readl(SW_VA_PMU_IO_BASE+0xcc);
    reg_val = (reg_val & ~(1 << 2)) | (v << 2) | (0x1623 << 16);
    writel(reg_val, SW_VA_PMU_IO_BASE+0xcc);

	return 0;
}


static int	pmu_dvfs_vftable_16_validctr(__u32 v)
{
    reg_val = readl(SW_VA_PMU_IO_BASE+0xcc);
    reg_val = (reg_val & ~(1 << 3)) | (v << 3) | (0x1623 << 16);
    writel(reg_val, SW_VA_PMU_IO_BASE+0xcc);

	return 0;
}


static int	pmu_dvfs_vftable_17_validctr(__u32 v)
{
	reg_val = readl(SW_VA_PMU_IO_BASE+0xcc);
    reg_val = (reg_val & ~(1 << 4)) | (v << 4) | (0x1623 << 16);
    writel(reg_val, SW_VA_PMU_IO_BASE+0xcc);

	return 0;
}


static int	pmu_dvfs_vftable_18_validctr(__u32 v)
{
	reg_val = readl(SW_VA_PMU_IO_BASE+0xcc);
    reg_val = (reg_val & ~(1 << 5)) | (v << 5) | (0x1623 << 16);
    writel(reg_val, SW_VA_PMU_IO_BASE+0xcc);

	return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_VF_cfg
 *
 * Description: init V-F table/index/range/valid bit
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int pmu_dvfs_VF_cfg(void)
{
	/* init vf-table */
    pmu_dvfs_vftable_init();
	/* set v-f table index0,1,2,3 */
    pmu_dvfs_set_vftable_index();
    /* use v-f table index0 simulation */
    PmuReg->PmuVFTIdx.VfTableIdx = 0;
    /* v-f table range */
	//pmu_dvfs_vftable_range(0,0,0);

	/* init V-F Table Register Valid control bit--> 0:valid;1:invalid */
	pmu_dvfs_vftable_13_validctr(0);
	pmu_dvfs_vftable_14_validctr(0);
	pmu_dvfs_vftable_15_validctr(1);
	pmu_dvfs_vftable_16_validctr(1);
	pmu_dvfs_vftable_17_validctr(1);
	pmu_dvfs_vftable_18_validctr(1);

    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_pll_stable_time_init
 *
 * Description: set pll stable time
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int pmu_dvfs_pll_stable_time_init(void)
{
    PmuReg->PmuCtl1.PllStaTime = 0x3E8;//1000

    return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_axi_clk_level_cfg
 *
 * Description: config axi clock level
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int pmu_dvfs_axi_clk_level_cfg(void)
{
    PmuReg->PmuAxiRng0.AxiClkLevel0 = 408;  //axi level 0
    PmuReg->PmuAxiRng0.AxiClkLevel1 = 816;  //axi level 1
    PmuReg->PmuAxiRng1.AxiClkLevel2 = 1200; //axi level 2

    return 0;
}


/*
 ***************************************************************************************************
 *                                  pmu_dvfs_irq_init
 *
 *Description: dvfs irq enable set
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_irq_init(void)
{
	/* dvfs irq disable */
	PmuReg->PmuIrqEn.DvfsFinIrqEn = 0;
	PmuReg->PmuIrqEn.DvfsSpdDetFinIrqEn = 0;
	PmuReg->PmuIrqEn.DvfsVtChgFinEn = 0;
	PmuReg->PmuIrqEn.DvfsClkSwtFinIrqEn = 0;
	PmuReg->PmuIrqEn.VtDetFinIrqEn = 0;
	PmuReg->PmuIrqEn.DvfsSpdDetErrIrqEn = 0;
	PmuReg->PmuIrqEn.DvfsVtChgErrEn = 0;
	PmuReg->PmuIrqEn.DvfsClkSwtErrIrqEn = 0;
	PmuReg->PmuIrqEn.DvfsVtDetErrIrqEn = 0;

	return 0;
}

#if 0
/*
 ***************************************************************************************************
 *                                      pmu_dvfs_idle_counter_init
 *
 *Description: init idle counter
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_idle_counter_init(void)
{
	/* cpu idle counter enable */
	PmuReg->PmuCpuIdlCtl.CpuIdleCntEn = 1;
    /* cpu idle counter clear enable:set 1 to clear the idle counter */
	PmuReg->PmuCpuIdlCtl.CpuIdleCntClrEn = 1;
	/* cpu idle enter/exit ,clk auto switch enalbe */
	//PmuReg->PmuCpuIdlCtl.CpuIdleAutoSwtEn = 1;

	return 0;
}
#endif

/*
 ***************************************************************************************************
 *                                      pmu_dvfs_mode_sel
 *
 *Description: pmu_dvfs mode select:0/1/2
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_mode_sel(__u32 mode)
{
	PmuReg->PmuCtl0.DvfsModeSel = mode;
	return 0;
}


/*
 ***************************************************************************************************
 *                                      pmu_dvfs_voltchg_mode
 *
 *Description: voltage change mode select:0(normal mode);1(maximum mode)
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int  pmu_dvfs_voltchg_mode(__u32 mode)
{
	PmuReg->PmuCtl0.VtChgMode = mode;
	return 0;
}


/*
 ***************************************************************************************************
 *                                          pmu_dvfs_clkchg_mode
 *
 *Description: clock change smooth mode select:0(divide mode);1(gating mode)
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_clkchg_mode(__u32 mode)
{
	PmuReg->PmuCtl0.ClkChgSmthMode = mode;
	return 0;
}


/*
 ***************************************************************************************************
 *                                     pmu_dvfs_config
 *
 * Description: init V-F table/index/range/valid bit
 *
 * Arguments  : none
 *
 * Returns    : 0;
 ***************************************************************************************************
*/
static int pmu_dvfs_config(void)
{
	/* set dvfs twi timeout cycles */
    PmuReg->PmuTOut.DvfsTiCyc = 0x3F;
    /* pmu_dvfs mode select:0/1/2 */
    pmu_dvfs_mode_sel(0);
    /* voltage change mode select:0(normal mode);1(maximum mode) */
    pmu_dvfs_voltchg_mode(0);
    /* clock change smooth mode select:0(divide mode);1(gating mode) */
    pmu_dvfs_clkchg_mode(1);
    /* smooth enable */
    pmu_dvfs_smooth_enable();
    /* enable voltage change */
    pmu_dvfs_volt_chg_enable();
    /* disable speed detect */
    pmu_dvfs_spd_det_disable();

	return 0;
}

#if 0
/*
 ***************************************************************************************************
 *                                  pmu_dvfs_spdfactor_init
 *
 *Description: init spdfactor
 *
 *Arguments  :
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
static int pmu_dvfs_spdfactor_init(void)
{
	/* spd div */
	PmuReg->PmuSpdDet0.SpdDetDelay = 1;
	/* spd factor0 */
	PmuReg->PmuSpdDet0.SpdDetMode = 0;//Speed Detect Mode
	/* spd factor1 */
	PmuReg->PmuSpdDet1.SpdDetMode = 0;//Speed Detect Mode
	/* spd factor2 */
	PmuReg->PmuSpdDet2.SpdDetMode = 0;//Speed Detect Mode

	return 0;
}
#endif

/*
 ***************************************************************************************************
 *                           pmu_dvfs_hw_init
 *
 *Description: init pmu dvfs hardware configuration
 *
 *Arguments  : none
 *
 *Return     : result,   0 : init hardware configuration successed
 *                      !0 : init hardware configuration failed
 *
 *Notes      :
 *
 ***************************************************************************************************
*/
int __init pmu_dvfs_hw_init(void)
{
    PmuReg = (__pmu_reg_list_t *)SW_VA_PMU_IO_BASE;

    /* dvfs off */
    pmu_dvfs_disable();
    /* enable ccu pll6 */
    __ccu_pll6_enable();
    /* set ahb clock source */
    __ccu_ahb_from_pll6();
    /* set pll stable time */
    pmu_dvfs_pll_stable_time_init();
    /* config axi clock level */
    pmu_dvfs_axi_clk_level_cfg();
    /* enable axi auto switch*/
    pmu_dvfs_axi_auto_swt_enable();
    /* debug clk output for pll1 */
    writel(0x3E008000,SW_VA_SRAM_IO_BASE+0x90); // PIOB4 OUTPUT=PLL1/8
    /* init V-F table/index/range/valid bit */
    pmu_dvfs_VF_cfg();

    /*
    **************************************
    *     cpu vdd value
    *
    *   0x08---------0.9v
    *   0x0c---------1.0v
    *   0x10---------1.1v
    *   0x14---------1.2v
    *   0x16---------1.25v
    *   0x18---------1.3v
    *   0x1c---------1.4v
    *   0x20---------1.5v
    *   0x24---------1.6v
    **************************************
    */
	/* change voltage independently; cpuvdd=0x1c, corevdd=0x16 */
    //pmu_vtchg_independent(0x1c,0x16);

	/* dvfs irq init */
	pmu_dvfs_irq_init();
	/* init idle counter */
	// pmu_dvfs_idle_counter_init();
    /* pmu_dvfs_config */
    pmu_dvfs_config();
    /* init speed factor:pmu_dvfs_spdfactor_init() */
	//pmu_dvfs_spdfactor_init();

    /* enable twi dvfs mode */
    pmu_dvfs_mode_enable();
    /* enable dvfs */
	pmu_dvfs_enable();
    /* clear pending */
    pmu_dvfs_clear_pending();
    /* sleep 50ms */
    msleep(50);

    DVFS_DBG("%s: finished\n", __func__);

    return 0;
}


/*
 ***************************************************************************************************
 *                           pmu_dvfs_start_to_finish
 *
 *Description: start dvfs, wait for finish
 *
 *Arguments  : none
 *
 *Return     : none
 *
 ***************************************************************************************************
*/
void pmu_dvfs_start_to_finish(void)
{
    reg_val = readl(CCU_CPU_AHB_APB) | (1 << 31);
    writel(reg_val, CCU_CPU_AHB_APB);
    while(PmuReg->PmuSta.DvfsBusy == 1);
    DVFS_DBG("%s: finished\n", __func__);
}


/*
 ***************************************************************************************************
 *                           pmu_dvfs_clear_pending
 *
 *Description: pmu dvfs clear pending
 *
 *Arguments  : none
 *
 *Return     : none
 *
 ***************************************************************************************************
*/
void pmu_dvfs_clear_pending(void)
{
    writel(readl(SW_VA_PMU_IO_BASE+0x44),SW_VA_PMU_IO_BASE+0x44);
}


/*
 ***************************************************************************************************
 *                           pmu_dvfs_volt_chg_err
 *
 *Description: pmu dvfs voltage change error
 *
 *Arguments  : none
 *
 *Return     : dvfs voltage change error pending flag
 *
 ***************************************************************************************************
*/
int pmu_dvfs_volt_chg_err(void)
{
    return PmuReg->PmuIrqSta.DvfsVtChgErrPd;
}


/*
 ***************************************************************************************************
 *                           pmu_dvfs_clk_swt_err
 *
 *Description: pmu dvfs clock switch error
 *
 *Arguments  : none
 *
 *Return     : dvfs clock switch error pending flag
 *
 ***************************************************************************************************
*/
int pmu_dvfs_clk_swt_err(void)
{
    return PmuReg->PmuIrqSta.DvfsClkSwtErrIrqPd;
}


/*
 ***************************************************************************************************
 *                           pmu_dvfs_suspend
 *
 *Description: pmu dvfs suspend
 *
 *Arguments  : none
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
int pmu_dvfs_suspend(void)
{
    /* disable dvfs */
    pmu_dvfs_disable();
    /* disable twi dvfs mode */
    pmu_dvfs_mode_disable();
    /* disable axi auto switch */
    pmu_dvfs_axi_auto_swt_disable();
    /* set ahb clock source axi */
    __ccu_ahb_from_axi();

    return 0;
}


/*
 ***************************************************************************************************
 *                           pmu_dvfs_resume
 *
 *Description: pmu dvfs resume
 *
 *Arguments  : none
 *
 *Return     : 0
 *
 ***************************************************************************************************
*/
int pmu_dvfs_resume(void)
{
    /* set ahb clock source from pll6 */
    __ccu_ahb_from_pll6();
    /* enable axi auto switch */
    pmu_dvfs_axi_auto_swt_enable();
    /* enable twi dvfs mode */
    pmu_dvfs_mode_enable();
    /* enable dvfs */
    pmu_dvfs_enable();

    return 0;
}
