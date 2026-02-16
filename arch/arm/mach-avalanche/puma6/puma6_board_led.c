/*
  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution
  in the file called LICENSE.GPL.

  Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052
*/
/*
 *
 * puma6_board_led.c
 * Description:
 * LED configuration data for board
 *
 */

#include <linux/init.h>
#include <pal.h>
#include <puma6_boards.h>
#include <led_manager.h>
#include <sram_api.h>

#define SOC_DOMAIN          0

#define HW_VALUE_LED_OFF    1
#define HW_VALUE_LED_ON     0

#define NO_LED_BIT          0xFFFFFFFF

typedef struct
{
    unsigned int led_id;
    unsigned int io_number;
    unsigned int io_bitmask[ LED_HAL_BITMASK_REG_WIDTH ];
    char *       module_name;       /* Module name */

} PUMA_LED_CFG_T;

/* Puma6 list of all GPIOs that are used for LED (in intel boards) */
/* GPIO numbers for Intel HarborPark and HarborPark-MG boards */
#define    PUMA6_LED_GPIO_DS       (50)   /* GPIO number 50 is used as DS LED in HP and HP-MG boards */  
#define    PUMA6_LED_GPIO_POWER    (51)   
#ifdef CONFIG_INTEL_KERNEL_BBU_SUPPORT
#define    PUMA6_LED_GPIO_BATTERY  (53)     
#else
#define    PUMA6_LED_GPIO_BATTERY  (NO_LED_BIT)
#endif
#define    PUMA6_LED_GPIO_ONLINE   (58)
#define    PUMA6_LED_GPIO_LINK     (59)
#define    PUMA6_LED_GPIO_MOCA     (60)  
#define    PUMA6_LED_GPIO_US       (61)  
#define    PUMA6_LED_GPIO_LINE2    (69)  
#define    PUMA6_LED_GPIO_LINE4    (72)  
#define    PUMA6_LED_GPIO_LINE3    (73)  
#define    PUMA6_LED_GPIO_LINE1    (98)  

/* GPIO numbers for Intel Falconmine and Catisland boards - only the diffrances from HarborPark are listed */
#define    PUMA6_LED_GPIO_POWER_FM_CI  (52)   /* GPIO number 52 is used as Power LED in FM and CI boards */       
#define    PUMA6_LED_GPIO_DS_FM_CI     (54)     
#define    PUMA6_LED_GPIO_ONLINE_FM_CI (55)
#define    PUMA6_LED_GPIO_LINK_FM_CI   (56)  

/* GPIO numbers for Intel Golden Springs board - only the diffrances from HarborPark are listed */
#define    PUMA6_LED_GPIO_POWER_GS     (65)        
#define    PUMA6_LED_GPIO_DS_GS        (64)     
#define    PUMA6_LED_GPIO_ONLINE_GS    (44)
#define    PUMA6_LED_GPIO_LINK_GS      (45)  

/* GPIO numbers for Intel Baker Lake board - only the diffrences from Golden Springs are listed */
#define    PUMA6_LED_GPIO_MOCA_BL      (69)
#define    PUMA6_LED_GPIO_LINE1_BL     (74) 
#define    PUMA6_LED_GPIO_LINE2_BL     (53)
#define    PUMA6_LED_GPIO_US_BL        (64) /* BL: US share same GPIO as DS (64) */
                                   
/* The following table defines the LED positions for HarborPark board type */
static PUMA_LED_CFG_T puma6ModulesCfg [PUMA_LED_ID_NUM_LEDS] =
{
    {   .module_name =  "power",        .led_id = PUMA_LED_ID_POWER,    .io_number = PUMA6_LED_GPIO_POWER,   },
    {   .module_name =  "downstream",   .led_id = PUMA_LED_ID_DS,       .io_number = PUMA6_LED_GPIO_DS,      },
    {   .module_name =  "upstream",     .led_id = PUMA_LED_ID_US,       .io_number = PUMA6_LED_GPIO_US,      },
    {   .module_name =  "online",       .led_id = PUMA_LED_ID_ONLINE,   .io_number = PUMA6_LED_GPIO_ONLINE,  },
    {   .module_name =  "link",         .led_id = PUMA_LED_ID_LINK,     .io_number = PUMA6_LED_GPIO_LINK,    },
    {   .module_name =  "line1",        .led_id = PUMA_LED_ID_LINE1,    .io_number = PUMA6_LED_GPIO_LINE1,   },
    {   .module_name =  "line2",        .led_id = PUMA_LED_ID_LINE2,    .io_number = PUMA6_LED_GPIO_LINE2,   },
    {   .module_name =  "line3",        .led_id = PUMA_LED_ID_LINE3,    .io_number = PUMA6_LED_GPIO_LINE3,   },
    {   .module_name =  "line4",        .led_id = PUMA_LED_ID_LINE4,    .io_number = PUMA6_LED_GPIO_LINE4,   },
    {   .module_name =  "battery",      .led_id = PUMA_LED_ID_BATTERY,  .io_number = PUMA6_LED_GPIO_BATTERY, },
    {   .module_name =  "moca",         .led_id = PUMA_LED_ID_MOCA,     .io_number = PUMA6_LED_GPIO_MOCA,    },
};

typedef struct
{
    PUMA_LED_STATE_e    ledState;
    Uint32              polarity;
    STATE_CFG_MODE_T    ledMode;
    Uint32              param1;
    Uint32              param2;
}
PUMA_LED_STATE_PARAMS_T;

/* This table defines the diferent LED behaviours (states) */
static const PUMA_LED_STATE_PARAMS_T ledStateParamsCfg[] =
{
    /* LED STATE        HAl should set LED ON/OFF   HAL state name               Params  */

    { PUMA_LED_STATE_OFF,    HW_VALUE_LED_OFF,      LED_HAL_MODE_LED_OFF,         0, 0 },
    { PUMA_LED_STATE_ON,     HW_VALUE_LED_ON,       LED_HAL_MODE_LED_ON,          0, 0 },
    { PUMA_LED_STATE_FLASH,  HW_VALUE_LED_ON,       LED_HAL_MODE_LED_FLASH,     200, 200} /* Freq = 2.5 Hz, Duty cycle = 50% */
};

static Int32 puma6_soc_led_group_set(Uint32 val, Uint32 mask, Uint32 reg_index)
{
    int idx;

    for(idx = 0; mask; mask >>= 1, val >>= 1, idx++)
    {
        if (mask & 1)
        {
            PAL_sysGpioOutBit( reg_index*32 + idx, val & 1 );
        }
    }
    return (0);
}


static void puma6_soc_led_callback_install(void)
{
    LED_FUNCS_T led_funcs;
    MOD_CFG_T   led_module_cfg;
    int         led_mod_idx;
    int         led_state_idx;

    for ( led_mod_idx=0; led_mod_idx < PUMA_LED_ID_NUM_LEDS; led_mod_idx++ )
    {
        if (puma6ModulesCfg[led_mod_idx].led_id != led_mod_idx)
        {
            printk("%s:%d ERROR - The LED Configuration is incorrect. Bad line is %d  \n", __FUNCTION__,__LINE__, led_mod_idx);
            return ;
        }

        if (NO_LED_BIT == puma6ModulesCfg[led_mod_idx].io_number)
        {
            continue;
        }

        memset(&led_funcs, 0, sizeof(LED_FUNCS_T));

        led_funcs.domain     = SOC_DOMAIN;

        led_funcs.pos_map[ puma6ModulesCfg[led_mod_idx].io_number/32 ] =  1 << (puma6ModulesCfg[led_mod_idx].io_number%32);
        led_funcs.off_val[ puma6ModulesCfg[led_mod_idx].io_number/32 ] =  HW_VALUE_LED_OFF << (puma6ModulesCfg[led_mod_idx].io_number%32);

        led_funcs.outVal = puma6_soc_led_group_set;
        led_manager_install_callbacks(&led_funcs);
        
        for (led_state_idx = 0; led_state_idx < LED_ARR_LEN(ledStateParamsCfg); led_state_idx++)
        {
            memset(&led_module_cfg, 0, sizeof(MOD_CFG_T));

            /* The name is size limited, so protect against overflow */
            strncpy((char *)led_module_cfg.name, puma6ModulesCfg[led_mod_idx].module_name, sizeof(led_module_cfg.name) - 1);
            /* and NULL terminate just in case ... */
            led_module_cfg.name[sizeof(led_module_cfg.name) - 1] = '\0';
            led_module_cfg.instance = 0;

            led_module_cfg.state_cfg.id              = ledStateParamsCfg[ led_state_idx ].ledState;
            led_module_cfg.state_cfg.mode            = ledStateParamsCfg[ led_state_idx ].ledMode;
            led_module_cfg.state_cfg.param1          = ledStateParamsCfg[ led_state_idx ].param1;
            led_module_cfg.state_cfg.param2          = ledStateParamsCfg[ led_state_idx ].param2;
            led_module_cfg.state_cfg.led_cfg.domain  = 0;

            led_module_cfg.state_cfg.led_val
                [ puma6ModulesCfg[led_mod_idx].io_number/32 ] =
                    ledStateParamsCfg[ led_state_idx ].polarity << (puma6ModulesCfg[led_mod_idx].io_number%32) ;

            led_module_cfg.state_cfg.led_cfg.pos_map
                [ puma6ModulesCfg[led_mod_idx].io_number/32 ]  =
                    1 << (puma6ModulesCfg[led_mod_idx].io_number%32);

            led_manager_cfg_mod(&led_module_cfg);
        }
    }

    return;
}

static bool puma6_soc_is_led_gpios_enabled(void)
{
    int i;

    for ( i=0; i < PUMA_LED_ID_NUM_LEDS; i++ )
    {
        if (NO_LED_BIT != puma6ModulesCfg[i].io_number)
        {
            if (GPIO_OUTPUT_PIN != PAL_sysGpioCtrlGetDir(puma6ModulesCfg[i].io_number))
            {
                if(!(strcmp(puma6ModulesCfg[i].module_name,"battery")))
                {
                    PAL_sysGpioCtrlSetDir(puma6ModulesCfg[i].io_number, GPIO_OUTPUT_PIN);
                }
                else
                {
                    printk(KERN_WARNING "%s: Warning - The LED GPIO configuration is incorrect. GPIO %d is not an output! will try to fix it...\n", __FUNCTION__, puma6ModulesCfg[i].io_number);
                    if (PAL_sysGpioCtrlSetDir(puma6ModulesCfg[i].io_number, GPIO_OUTPUT_PIN))
                    {
                        printk(KERN_ERR "%s: ERROR - The LED GPIO configuration is incorrect. GPIO %d is not an output\n", __FUNCTION__, puma6ModulesCfg[i].io_number);
                        return false;
                    }
                    printk("%s: LED GPIO %d was fix to output!\n", __FUNCTION__, puma6ModulesCfg[i].io_number);
                }
            }
        }
    }
    return true;
}

static void puma6_soc_led_gpio_init(void)
{
    int i;

    for ( i=0; i < PUMA_LED_ID_NUM_LEDS; i++ )
    {
        if (NO_LED_BIT != puma6ModulesCfg[i].io_number)
        {
            PAL_sysGpioOutBit( puma6ModulesCfg[i].io_number, HW_VALUE_LED_OFF );
        }
    }

    return;
}

extern int led_manager_init(void);

static int led_callback_init(void)
{
    extern unsigned long puma6_boardtype_id;
    Uint32 board_rev=0;

    switch (puma6_boardtype_id)
    {
    case PUMA6_FM_BOARD_ID:
        {
            if (SRAM_Get_Boot_Param(BOARD_REVISION_ID, &board_rev))
            {
                printk(KERN_ERR "%s: ERROR - The LED GPIO can't read BOARD_REVISION_ID!!\n", __FUNCTION__);
                return -1;
            }
            if (board_rev >= PUMA6_FM_20_BOARD_REVISION_ID)
            {
                /* In FM 2.0 the LINE1-4 and BATTERY LED are not used. */
                puma6ModulesCfg[ PUMA_LED_ID_LINE1    ].io_number = NO_LED_BIT;
                puma6ModulesCfg[ PUMA_LED_ID_LINE2    ].io_number = NO_LED_BIT;
                puma6ModulesCfg[ PUMA_LED_ID_LINE3    ].io_number = NO_LED_BIT;
                puma6ModulesCfg[ PUMA_LED_ID_LINE4    ].io_number = NO_LED_BIT;
                puma6ModulesCfg[ PUMA_LED_ID_BATTERY  ].io_number = NO_LED_BIT;
            }
        }
        /* no "break" The CI setting is also needed for FM... */
    case PUMA6_CI_BOARD_ID:
        puma6ModulesCfg[ PUMA_LED_ID_POWER  ].io_number = PUMA6_LED_GPIO_POWER_FM_CI;
        puma6ModulesCfg[ PUMA_LED_ID_DS     ].io_number = PUMA6_LED_GPIO_DS_FM_CI;
        puma6ModulesCfg[ PUMA_LED_ID_ONLINE ].io_number = PUMA6_LED_GPIO_ONLINE_FM_CI;
        puma6ModulesCfg[ PUMA_LED_ID_LINK   ].io_number = PUMA6_LED_GPIO_LINK_FM_CI;
        break;
    case PUMA6_GS_BOARD_ID:
        puma6ModulesCfg[ PUMA_LED_ID_POWER  ].io_number = PUMA6_LED_GPIO_POWER_GS;
        puma6ModulesCfg[ PUMA_LED_ID_DS     ].io_number = PUMA6_LED_GPIO_DS_GS;
        puma6ModulesCfg[ PUMA_LED_ID_ONLINE ].io_number = PUMA6_LED_GPIO_ONLINE_GS;
        puma6ModulesCfg[ PUMA_LED_ID_LINK   ].io_number = PUMA6_LED_GPIO_LINK_GS;
        puma6ModulesCfg[ PUMA_LED_ID_MOCA   ].io_number = NO_LED_BIT;
        puma6ModulesCfg[ PUMA_LED_ID_LINE3  ].io_number = NO_LED_BIT;
        puma6ModulesCfg[ PUMA_LED_ID_LINE4  ].io_number = NO_LED_BIT;
        break;
    case PUMA6_BL_BOARD_ID:
        puma6ModulesCfg[ PUMA_LED_ID_POWER  ].io_number = PUMA6_LED_GPIO_POWER_GS;
        puma6ModulesCfg[ PUMA_LED_ID_DS     ].io_number = PUMA6_LED_GPIO_DS_GS;
        puma6ModulesCfg[ PUMA_LED_ID_ONLINE ].io_number = PUMA6_LED_GPIO_ONLINE_GS;
        puma6ModulesCfg[ PUMA_LED_ID_US     ].io_number = PUMA6_LED_GPIO_US_BL;
        puma6ModulesCfg[ PUMA_LED_ID_MOCA   ].io_number = PUMA6_LED_GPIO_MOCA_BL;
        puma6ModulesCfg[ PUMA_LED_ID_LINE1  ].io_number = PUMA6_LED_GPIO_LINE1_BL;
        puma6ModulesCfg[ PUMA_LED_ID_LINE2  ].io_number = PUMA6_LED_GPIO_LINE2_BL;
        /* In BL the LINE3-4, LINK and BATTERY LED are not used. */
        puma6ModulesCfg[ PUMA_LED_ID_LINK   ].io_number = NO_LED_BIT;
        puma6ModulesCfg[ PUMA_LED_ID_LINE3  ].io_number = NO_LED_BIT;
        puma6ModulesCfg[ PUMA_LED_ID_LINE4  ].io_number = NO_LED_BIT;
        puma6ModulesCfg[ PUMA_LED_ID_BATTERY  ].io_number = NO_LED_BIT;
        break;
    }

    /* Enabling and Initialiasing the GPIO's mapped for the LED(s) */
    if ( !puma6_soc_is_led_gpios_enabled() )
    {
        printk(KERN_ERR "%s: ERROR - The LED GPIO configuration is incorrect. LED driver exit!!\n", __FUNCTION__);
        return -1;
    }
    puma6_soc_led_gpio_init();

    led_manager_init();

    puma6_soc_led_callback_install();

    /* Start POWER LED at ON */
    PAL_sysGpioOutBit( puma6ModulesCfg[PUMA_LED_ID_POWER].io_number, HW_VALUE_LED_ON );

    return(0);
}

__initcall(led_callback_init);
