/*
 * kernel/puma6_bootcfg_ctrl.c
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011-2012 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */

/** \file   puma6_bootcfg_ctrl.c
 *  \brief  PAL boot config control APIs
 *          The boot config unit contains the docsis io enable
 *          also it contains the unlock register           
 *          
 *  \author     Intel
 *
 *  \version    0.1     Uri Tal   		Created
 */


#include <pal.h>
#include <puma6.h>
#include <linux/kernel.h>
#include <puma6_bootcfg_ctrl.h>


/** \enum PAL_SYS_DOSCIS_IO_BITS_tag
    \brief Enum for DOCSIS_IO_ENABLE register bits
*/
typedef enum PAL_SYS_DOSCIS_IO_BITS_tag
{
    BBU_ENABLE = 0,
    IIC_ENABLE,
    CODEC0_ENABLE,
    CODEC1_ENABLE,
    TDM0_ENABLE,
    TDM1_ENABLE,
    UART0_ENABLE,
    UART1_ENABLE,
    UART2_ENABLE,
    ZDS_ENABLE,
    OC_ENABLE,            
    US_AMP_ENABLE,          
    SR_MPEG_ENABLE,         
    TUNER_PWM_ENABLE,       
    CODEC_RESET_ENABLE,
    C55_EMU0_ENABLE,
    C55_EMU1_ENABLE
}PAL_SYS_BOOTCFG_MODULE_T;


static struct     semaphore BootCfg_sem;

/* Start address 0x000D_0000                */
/* End address   0x000D_FFFF                */
#define BOOTCFG_MOD_BASE                        (AVALANCHE_BOOTCFG_BASE)
#define DOCSIS_IOs_ENABLE_BASE                  (BOOTCFG_MOD_BASE + 0x144)
#define UNLOCK_REGISTER0_BASE                   (BOOTCFG_MOD_BASE + 0x07C)
#define UNLOCK_REGISTER1_BASE                   (BOOTCFG_MOD_BASE + 0x080)
#define PUMA_BOOTCFG_KICK_0_VAL                 (0x20406080)
#define PUMA_BOOTCFG_KICK_1_VAL                 (0x10305070)
#define CTRL_DSP_PRIORITY_BASE                  (BOOTCFG_MOD_BASE + 0x044)
#define PRIORITIZE_DSP_TO_BUNIT_VAL             (0x00000001)

static bool  InitOK = false;

/*     */
/*  Docsis IOs enable(offset: 0x0144)                                                                                               */
/* Bits	       Field Name	   Type	      Default value	       Description                                                          */
/*  0	      bbu_enable	     RW	         0x0	             0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */ 
/*  1	       iic_enable	     RW	         0x0	             0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  2	       codec0_enable     RW	         0x0	             0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  3	       codec1_enable     RW	         0x0	             0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  4	       tdm0_enable	     RW	         0x0	             0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  5	       tdm1_enable	     RW	         0x0	             0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  6	       uart0_enable  	RW       	0x0          	     0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  7	       uart1_enable  	RW       	0x0          	     0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  8	       uart2_enable  	RW       	0x0          	     0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  9	       zds_enable	    RW	        0x0	                 0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  10	       oc_enable      	RW	        0x0	                 0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  11	       us_amp_enable  	RW	        0x0	                 0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  12	       sr_mpeg_enable 	RW	        0x0	                 0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  13	       tuner_pwm_enable RW	        0x0	                 0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  14	       codec_reset_enable RW        0x0	                 0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  15	       c55_emu0_enable  RW	        0x0	                 0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */
/*  16	       c55_emu1_enable  RW	        0x0	                 0 : disable, the module IO's are not selected for muxing out       */
/*                                                               1: enable the module IO's are selected for muxing out              */




/***************************************************************************************** */
/**************************************************************************/
/*! \fn static void PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(PAL_SYS_BOOTCFG_MODULE_T BitNum, PAL_SYS_ENABLE_CTRL_T op)
 **************************************************************************
 *  \brief This function is used to read modify write any docsis io control registers  
 *  \param[in]: PAL_SYS_BOOTCFG_MODULE_T BitNum - The bit number of the module
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T op - Enable/Disable the module's pin muxing
 *  \param[out] 
 **************************************************************************/
static void PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(PAL_SYS_BOOTCFG_MODULE_T BitNum, PAL_SYS_ENABLE_CTRL_T op)
{
    volatile UINT32 *docsis_io_ctrl = (UINT32 *)DOCSIS_IOs_ENABLE_BASE;
    UINT32 RegVal = *docsis_io_ctrl;

    if (op == BOOTCFG_IO_DISABLE)
    {
        RegVal &= ~(1 << BitNum);
    }
    else
    {
        RegVal |= (1 << BitNum);
    }

    *docsis_io_ctrl = RegVal;
}


/**************************************************************************/
/*! \fn static void PAL_sysBootCfgCtrl_UnlockKickReg(void)
 **************************************************************************
 *  \brief This function is used to unlock kicker register  
 *  \param[in] 
 *  \param[out] 
 **************************************************************************/
static void PAL_sysBootCfgCtrl_UnlockKickReg(void)
{
    volatile UINT32 *UnlockReg = (UINT32 *)UNLOCK_REGISTER0_BASE;
    *UnlockReg = PUMA_BOOTCFG_KICK_0_VAL;
    UnlockReg = (UINT32 *)UNLOCK_REGISTER1_BASE;
    *UnlockReg = PUMA_BOOTCFG_KICK_1_VAL;
}


/**************************************************************************/
/*! \fn static void PAL_sysBootCfgCtrl_lockKickReg(void)
 **************************************************************************
 *  \brief This function is used to lock kicker register 
 *  \param[in] 
 *  \param[out] 
 **************************************************************************/
static void PAL_sysBootCfgCtrl_lockKickReg(void)
{
    volatile UINT32 *UnlockReg = (UINT32 *)UNLOCK_REGISTER0_BASE;
    *UnlockReg = 0; /* can be any value other than PUMA_BOOTCFG_KICK_0_VAL*/
    UnlockReg = (UINT32 *)UNLOCK_REGISTER1_BASE;
    *UnlockReg = 0;  /* can be any value other than PUMA_BOOTCFG_KICK_1_VAL*/
}


/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_SetDspPriority(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the OOB output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable set to prioritize DSP in the Bunit
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_SetDspPriority(PAL_SYS_ENABLE_CTRL_T Op)
{
    volatile UINT32 *dsp_ctrl = (UINT32 *)CTRL_DSP_PRIORITY_BASE;
    UINT32 val = 0;

    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    if (Op == BOOTCFG_IO_ENABLE)
    {
        val = PRIORITIZE_DSP_TO_BUNIT_VAL;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    *dsp_ctrl = val;
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_SetDspPriority);

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_OOB(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the OOB output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable OOB pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_OOB(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }
    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(OC_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_OOB);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_MPEG_out(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the mpeg out output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable MPEG_OUT pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_MPEG_out(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(SR_MPEG_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_MPEG_out);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_BBU(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the BBU output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable BBU pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_BBU(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(BBU_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_BBU);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_IIC(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the IIC output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable I2C pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_IIC(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(IIC_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_IIC);



/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_CODEC0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the CODEC0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable CODEC0 pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_CODEC0(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(CODEC0_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_CODEC0);


/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_CODEC1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the CODEC1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable CODEC1 pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_CODEC1(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(CODEC1_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_CODEC1);


/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_TDM0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the TDM0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable TDM0 pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_TDM0(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(TDM0_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_TDM0);


/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_TDM1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the TDM1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable TDM1 pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_TDM1(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(TDM1_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_TDM1);


/**************************************************************************/
/*! \fn  void PAL_sysBootCfgCtrl_DocsisIo_UART0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the UART0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable UART0 pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_UART0(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(UART0_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_UART0);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_UART1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the UART1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable UART1 pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_UART1(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(UART1_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_UART1);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_UART2(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the UART2 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable UART2 pin muxing 
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_UART2(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(UART2_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_UART2);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_ZDS(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the ZDS output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable ZDS pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_ZDS(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(ZDS_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_ZDS);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_US_AMP(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the US_AMP output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable US_AMP pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_US_AMP(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
	{
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(US_AMP_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_US_AMP);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the C55_EMU1 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable C55_EMU1 pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(C55_EMU1_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_C55_EMU1);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_TUNER_PWM(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the TUNER_PWM output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable TUNER_PWM pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_TUNER_PWM(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(TUNER_PWM_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_TUNER_PWM);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the C55_EMU0 output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable C55_EMU0 pin muxing
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(C55_EMU0_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_C55_EMU0);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_DocsisIo_CODEC_RESET(PAL_SYS_RESET_CTRL_T Op)
 **************************************************************************
 *  \brief This API is used to enable or disable the CODEC_RESET output
 *  \param[in]: PAL_SYS_ENABLE_CTRL_T Op - Enable/Disable CODEC_RESET pin muxing 
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_DocsisIo_CODEC_RESET(PAL_SYS_ENABLE_CTRL_T Op)
{
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();
    PAL_sysBootCfgCtrl_ReadModifyWriteDocsisIo(CODEC_RESET_ENABLE, Op);
    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_DocsisIo_CODEC_RESET);


/* --------------------------- General register operations --------------------------------*/

/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_ReadReg(UINT32 RegOffset)
 **************************************************************************
 *  \brief This API is used to Read a BootCfg register.
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address 
 *  \param[out]: UINT32 - the register content value
 **************************************************************************/
UINT32 PAL_sysBootCfgCtrl_ReadReg(UINT32 RegOffset)
{
    volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_MOD_BASE + RegOffset);
    return (*RegAddr) ;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_ReadReg);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_WriteReg(UINT32 RegOffset, UINT32 RegVal)
 **************************************************************************
 *  \brief This API is used to write a BootCfg register.
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address
 *  \param[in]: UINT32 RegVal - the register value to be written
 *  \param[out]
 **************************************************************************/
void PAL_sysBootCfgCtrl_WriteReg(UINT32 RegOffset, UINT32 RegVal)
{
    volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_MOD_BASE + RegOffset);
    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();

    *RegAddr = RegVal;

    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_WriteReg);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_ReadModifyWriteReg(UINT32 RegOffset, UINT32 BitMask, UINT32 BitValue)
 **************************************************************************
 *  \brief This API is used to Read modify write a BootCfg register
 *  \param[in]: UINT32 RegOffset - The register offset from BootCfg module base address
 *  \param[in]: UINT32 BitMask - The bit(s) mask of the bit(s) we want to change
 *  \param[in]: UINT32 BitValue - The bit(s) value we want to set at teh location of the bit(s) mask
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_ReadModifyWriteReg(UINT32 RegOffset, UINT32 BitMask, UINT32 BitValue)
{
    volatile UINT32 *RegAddr = (UINT32 *)(BOOTCFG_MOD_BASE + RegOffset);
    UINT32 RegVal;

    if (InitOK == false)
    {
        printk (KERN_WARNING "BootConfig init not called \n");
        return;
    }

    down(&BootCfg_sem);
    PAL_sysBootCfgCtrl_UnlockKickReg();

    RegVal =  *RegAddr;
    RegVal &= ~(BitMask);
    RegVal |= BitValue;
    *RegAddr = RegVal;

    PAL_sysBootCfgCtrl_lockKickReg();
    up(&BootCfg_sem);
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_ReadModifyWriteReg);


/**************************************************************************/
/*! \fn void PAL_sysBootCfgCtrl_init(void)
 **************************************************************************
 *  \brief This API is used to initialize the BootCfg internal data
 *  \param[in]: 
 *  \param[out] 
 **************************************************************************/
void PAL_sysBootCfgCtrl_init(void)
{
    sema_init(&BootCfg_sem, 1);
    InitOK = true;
}
EXPORT_SYMBOL(PAL_sysBootCfgCtrl_init);

























