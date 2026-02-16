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
 * puma5_board_led.c
 * Description:
 * LED configuration data for board
 *
 */

#include <linux/init.h>
#include <pal.h>
#include <led_manager.h>

#define SOC_DOMAIN          0

#define HW_VALUE_LED_OFF    1
#define HW_VALUE_LED_ON     0


enum
{
    GPIO0_LED_POWER = 0,
    GPIO1_LED_ONLINE,
    GPIO2_LED_LINE1,
    GPIO3_LED_LINE2,
    GPIO4_LED_LINE3,
    GPIO5_LED_LINE4,
    GPIO8_LED_BAT = 8,
    GPIO12_LED_LINK_ACT = 12,
    GPIO16_LED_DS = 16,
    GPIO17_LED_US,

    NO_LED_BIT = 0xFFFFFFFF
};

typedef struct
{
    unsigned int led_id;
    unsigned int io_number;
    unsigned int io_bitmask[ LED_HAL_BITMASK_REG_WIDTH ];
    char *       module_name;       /* Module name */

} PUMA_LED_CFG_T;

static PUMA_LED_CFG_T puma5ModulesCfg [PUMA_LED_ID_NUM_LEDS] =
{
    {   .module_name =  "power",        .led_id = PUMA_LED_ID_POWER,    .io_number = GPIO0_LED_POWER,       },
    {   .module_name =  "downstream",   .led_id = PUMA_LED_ID_DS,       .io_number = GPIO16_LED_DS,         },
    {   .module_name =  "upstream",     .led_id = PUMA_LED_ID_US,       .io_number = GPIO17_LED_US,         },
    {   .module_name =  "online",       .led_id = PUMA_LED_ID_ONLINE,   .io_number = GPIO1_LED_ONLINE,      },
    {   .module_name =  "link",         .led_id = PUMA_LED_ID_LINK,     .io_number = GPIO12_LED_LINK_ACT,   },
    {   .module_name =  "line1",        .led_id = PUMA_LED_ID_LINE1,    .io_number = GPIO2_LED_LINE1,       },
    {   .module_name =  "line2",        .led_id = PUMA_LED_ID_LINE2,    .io_number = GPIO3_LED_LINE2,       },
    {   .module_name =  "line3",        .led_id = PUMA_LED_ID_LINE3,    .io_number = GPIO4_LED_LINE3,       },
    {   .module_name =  "line4",        .led_id = PUMA_LED_ID_LINE4,    .io_number = GPIO5_LED_LINE4,       },

#ifdef CONFIG_INTEL_KERNEL_BBU_SUPPORT
    {   .module_name =  "battery",      .led_id = PUMA_LED_ID_BATTERY,  .io_number = GPIO8_LED_BAT,         },
#else
    {   .module_name =  "battery",      .led_id = PUMA_LED_ID_BATTERY,  .io_number = NO_LED_BIT,            },
#endif
    {   .module_name =  "moca",         .led_id = PUMA_LED_ID_MOCA,     .io_number = NO_LED_BIT,            },
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

static void puma5_soc_led_callback_install(void)
{
    LED_FUNCS_T led_funcs;
    MOD_CFG_T   led_module_cfg;
    int         led_mod_idx;
    int         led_state_idx;

    for ( led_mod_idx=0; led_mod_idx < PUMA_LED_ID_NUM_LEDS; led_mod_idx++ )
    {
        if (puma5ModulesCfg[led_mod_idx].led_id != led_mod_idx)
        {
            printk("%s:%d ERROR - The LED Configuration is incorrect. Bad line is %d  \n", __FUNCTION__,__LINE__, led_mod_idx);
            return ;
        }

        if (NO_LED_BIT == puma5ModulesCfg[led_mod_idx].io_number)
        {
            continue;
        }

        memset(&led_funcs, 0, sizeof(LED_FUNCS_T));

        led_funcs.domain     = SOC_DOMAIN;

        led_funcs.pos_map[ puma5ModulesCfg[led_mod_idx].io_number/32 ] =  1 << (puma5ModulesCfg[led_mod_idx].io_number%32);
        led_funcs.off_val[ puma5ModulesCfg[led_mod_idx].io_number/32 ] =  HW_VALUE_LED_OFF << (puma5ModulesCfg[led_mod_idx].io_number%32);

        led_funcs.outVal     = PAL_sysGpioOutValue;
        led_manager_install_callbacks(&led_funcs);
        for (led_state_idx = 0; led_state_idx < LED_ARR_LEN(ledStateParamsCfg); led_state_idx++)
        {
            memset(&led_module_cfg, 0, sizeof(MOD_CFG_T));

            /* The name is size limited, so protect against overflow */
            strncpy((char *)led_module_cfg.name, puma5ModulesCfg[led_mod_idx].module_name, sizeof(led_module_cfg.name) - 1);
            /* and NULL terminate just in case ... */
            led_module_cfg.name[sizeof(led_module_cfg.name) - 1] = '\0';
            led_module_cfg.instance = 0;

            led_module_cfg.state_cfg.id              = ledStateParamsCfg[ led_state_idx ].ledState;
            led_module_cfg.state_cfg.mode            = ledStateParamsCfg[ led_state_idx ].ledMode;
            led_module_cfg.state_cfg.param1          = ledStateParamsCfg[ led_state_idx ].param1;
            led_module_cfg.state_cfg.param2          = ledStateParamsCfg[ led_state_idx ].param2;
            led_module_cfg.state_cfg.led_cfg.domain  = SOC_DOMAIN;

            led_module_cfg.state_cfg.led_val
                [ puma5ModulesCfg[led_mod_idx].io_number/32 ] =
                    ledStateParamsCfg[ led_state_idx ].polarity << (puma5ModulesCfg[led_mod_idx].io_number%32) ;

            led_module_cfg.state_cfg.led_cfg.pos_map
                [ puma5ModulesCfg[led_mod_idx].io_number/32 ]  =
                    1 << (puma5ModulesCfg[led_mod_idx].io_number%32);

            led_manager_cfg_mod(&led_module_cfg);
        }
    }

    return;
}

static void puma5_soc_led_gpio_enable(void)
{
    int i;

    for ( i=0; i < PUMA_LED_ID_NUM_LEDS; i++ )
    {
        if (NO_LED_BIT != puma5ModulesCfg[i].io_number)
        {
            PAL_sysGpioCtrl(puma5ModulesCfg[i].io_number, GPIO_PIN, GPIO_OUTPUT_PIN);
        }
    }
}

static void puma5_soc_led_gpio_init(void)
{
    int i;

    for ( i=0; i < PUMA_LED_ID_NUM_LEDS; i++ )
    {
        if (NO_LED_BIT != puma5ModulesCfg[i].io_number)
        {
            PAL_sysGpioOutBit(puma5ModulesCfg[i].io_number, HW_VALUE_LED_OFF);
        }
    }

    return;
}

extern int led_manager_init(void);

static int led_callback_init(void)
{
    extern char *puma5_boardtype;

    if ( strstr( puma5_boardtype, "tnetc958" ) )
    {
        /* Line 3 and Line 4 LEDs are not available in the tnetc958 design */
        puma5ModulesCfg[ PUMA_LED_ID_LINE3 ].io_number = NO_LED_BIT;
        puma5ModulesCfg[ PUMA_LED_ID_LINE4 ].io_number = NO_LED_BIT;
    }

    /* Enabling and Initialiasing the GPIO's mapped for the LED(s) */
    puma5_soc_led_gpio_enable();

    puma5_soc_led_gpio_init();

    led_manager_init();

    puma5_soc_led_callback_install();

    /* Start POWER LED at ON */
    PAL_sysGpioOutValue( HW_VALUE_LED_ON, 1 << GPIO0_LED_POWER, GPIO0_LED_POWER/32 );

    return(0);
}

__initcall(led_callback_init);
