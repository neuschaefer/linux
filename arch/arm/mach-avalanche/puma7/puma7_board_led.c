/*
  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

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
 * puma7_board_led.c
 * Description:
 * LED configuration data for board
 *
 */

#include <linux/init.h>
#include <pal.h>
#include <puma7_boards.h>
#include <puma7_gpio_ctrl.h>
#include <led_manager.h>
#include <sram_api.h>

#define SOC_DOMAIN          0

#define HW_VALUE_LED_OFF    0
#define HW_VALUE_LED_ON     1

#define HW_VALUE_LED_DISABLE    0
#define HW_VALUE_LED_ENABLE     1

#define NO_LED_BIT          0xFFFFFFFF

typedef struct
{
    unsigned int led_id;
    unsigned int io_number;
    unsigned int io_bitmask[ LED_HAL_BITMASK_REG_WIDTH ];
    char *       module_name;       /* Module name */

} PUMA_LED_CFG_T;

                                   
/* The following table defines the LED positions for HarborPark board type */
static PUMA_LED_CFG_T puma7ModulesCfg [PUMA_LED_ID_NUM_LEDS] =
{
    {   .module_name =  "power",        .led_id = PUMA_LED_ID_POWER,    .io_number = PUMA7_GPIO_LED_POWER,   },
    {   .module_name =  "downstream",   .led_id = PUMA_LED_ID_DS,       .io_number = PUMA7_GPIO_LED_DS,      },
    {   .module_name =  "upstream",     .led_id = PUMA_LED_ID_US,       .io_number = PUMA7_GPIO_LED_US,      },
    {   .module_name =  "online",       .led_id = PUMA_LED_ID_ONLINE,   .io_number = PUMA7_GPIO_LED_ONLINE,  },
    {   .module_name =  "link",         .led_id = PUMA_LED_ID_LINK,     .io_number = PUMA7_GPIO_LED_LINK,    },
    {   .module_name =  "line1",        .led_id = PUMA_LED_ID_LINE1,    .io_number = PUMA7_GPIO_LED_LINE1,   },
    {   .module_name =  "line2",        .led_id = PUMA_LED_ID_LINE2,    .io_number = PUMA7_GPIO_LED_LINE2,   },
    {   .module_name =  "dect",         .led_id = PUMA_LED_ID_DECT,     .io_number = PUMA7_GPIO_LED_DECT,    },
    {   .module_name =  "battery",      .led_id = PUMA_LED_ID_BATTERY,  .io_number = PUMA7_GPIO_LED_BATTERY, },
    {   .module_name =  "moca",         .led_id = PUMA_LED_ID_MOCA,     .io_number = PUMA7_GPIO_LED_MOCA,    },
    {   .module_name =  "wifi2.4g",     .led_id = PUMA_LED_ID_WIFI2_4G, .io_number = PUMA7_GPIO_LED_WIFI2_4G,},
    {   .module_name =  "wifi5g",       .led_id = PUMA_LED_ID_WIFI5G,   .io_number = PUMA7_GPIO_LED_WIFI5G,  },
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


/****************************************************************************/
/*                               Local functions                            */
/****************************************************************************/

/*! \fn puma7_soc_led_group_set(Uint32 val, Uint32 mask, Uint32 reg_index) 
    \brief The function receive mask and reg_index and turn OFF/ON the 
           corresponding GPIO.
    \param val - value to set (received as bit mask (0-32) for corresponding GPIO)
    \param mask - mask of the GPIO number modulo 32.
    \param reg_index - position of the register (GPIO number devided by 32)
    \return Returns (OK/FAIL)
*/
static Int32 puma7_soc_led_group_set(Uint32 val, Uint32 mask, Uint32 reg_index)
{
    int idx;

    /*Extract the value and number from the received params*/
    for(idx = 0; mask; mask >>= 1, val >>= 1, idx++)
    {
        /*Found the GPIO number (modulo 32)*/
        if (mask & 1)
        {
            /*Calculate the corresponding GPIO and set the value 
              e.g. mask = 0x1, reg_index = 1 ===> GPIO = 1x32 + 1 = 33*/
            PAL_sysGpioOutBit( reg_index*32 + idx, val & 1 );
        }
    }
    return (0);
}

/*! \fn puma7_soc_led_callback_install(void 
    \brief The function register the led module with the led hal driver
*/
static void puma7_soc_led_callback_install(void)
{
    LED_FUNCS_T led_funcs;
    MOD_CFG_T   led_module_cfg;
    int         led_mod_idx;
    int         led_state_idx;

    /*Go over all the existing reported leds */
    for ( led_mod_idx=0; led_mod_idx < PUMA_LED_ID_NUM_LEDS; led_mod_idx++ )
    {
        /*make sure the the define are aligned*/
        if (puma7ModulesCfg[led_mod_idx].led_id != led_mod_idx)
        {
            printk("%s:%d ERROR - The LED Configuration is incorrect. Bad line is %d  \n", __FUNCTION__,__LINE__, led_mod_idx);
            return ;
        }
        /*incase the module is not represented with LED, skip it*/
        if (NO_LED_BIT == puma7ModulesCfg[led_mod_idx].io_number)
        {
            continue;
        }

        memset(&led_funcs, 0, sizeof(LED_FUNCS_T));

        led_funcs.domain     = SOC_DOMAIN;

        /*clculate the postion map (represented as 'mask' in the led_group_set)*/
        led_funcs.pos_map[ puma7ModulesCfg[led_mod_idx].io_number/32 ] =  1 << (puma7ModulesCfg[led_mod_idx].io_number%32);
        /*set the led OFF value (represented as 'val' in the led_group_set)*/
        led_funcs.off_val[ puma7ModulesCfg[led_mod_idx].io_number/32 ] =  HW_VALUE_LED_OFF << (puma7ModulesCfg[led_mod_idx].io_number%32);
        /*register the call-back function*/
        led_funcs.outVal = puma7_soc_led_group_set;
        /*install the callbacks*/
        led_manager_install_callbacks(&led_funcs);
        
        /*Go over all let states and register */
        for (led_state_idx = 0; led_state_idx < LED_ARR_LEN(ledStateParamsCfg); led_state_idx++)
        {
            memset(&led_module_cfg, 0, sizeof(MOD_CFG_T));

            /* The name is size limited, so protect against overflow */
            strncpy((char *)led_module_cfg.name, puma7ModulesCfg[led_mod_idx].module_name, sizeof(led_module_cfg.name) - 1);
            /* and NULL terminate just in case ... */
            led_module_cfg.name[sizeof(led_module_cfg.name) - 1] = '\0';
            led_module_cfg.instance = 0;

            led_module_cfg.state_cfg.id              = ledStateParamsCfg[ led_state_idx ].ledState;
            led_module_cfg.state_cfg.mode            = ledStateParamsCfg[ led_state_idx ].ledMode;
            led_module_cfg.state_cfg.param1          = ledStateParamsCfg[ led_state_idx ].param1;
            led_module_cfg.state_cfg.param2          = ledStateParamsCfg[ led_state_idx ].param2;
            led_module_cfg.state_cfg.led_cfg.domain  = 0;

            led_module_cfg.state_cfg.led_val
                [ puma7ModulesCfg[led_mod_idx].io_number/32 ] =
                    ledStateParamsCfg[ led_state_idx ].polarity << (puma7ModulesCfg[led_mod_idx].io_number%32) ;

            led_module_cfg.state_cfg.led_cfg.pos_map
                [ puma7ModulesCfg[led_mod_idx].io_number/32 ]  =
                    1 << (puma7ModulesCfg[led_mod_idx].io_number%32);

            led_manager_cfg_mod(&led_module_cfg);
        }
    }

    return;
}
/*! \fn  puma7_soc_is_led_gpios_enabled(void)
    \brief The function eanble all the operating LEDs
    \return Returns (OK/FAIL)
*/
static void puma7_soc_is_led_gpios_enabled(void)
{
    int i;

    for ( i=0; i < PUMA_LED_ID_NUM_LEDS; i++ )
    {
        if (NO_LED_BIT != puma7ModulesCfg[i].io_number)
        {
            /*Eanble the GPIO*/
            if (PAL_sysGpioSetStatus(puma7ModulesCfg[i].io_number, HW_VALUE_LED_ENABLE))
            {
                printk(KERN_ERR "%s: ERROR - Fail to enable GPIO %d\n", __FUNCTION__, puma7ModulesCfg[i].io_number);
            }
            /*Make sure the GPIO set fot output*/
            if (PAL_sysGpioCtrlSetDir(puma7ModulesCfg[i].io_number, GPIO_OUTPUT_PIN))
            {
                printk(KERN_ERR "%s: ERROR - Fail to set GPIO %d to output\n", __FUNCTION__, puma7ModulesCfg[i].io_number);
            }
        }
    }
}
/*! \fn  void puma7_soc_led_gpio_init(void)
    \brief The funciton init all LEDs for default off status.
    \return Returns (OK/FAIL)
*/
static void puma7_soc_led_gpio_init(void)
{
    int i;

    for ( i=0; i < PUMA_LED_ID_NUM_LEDS; i++ )
    {
        if (NO_LED_BIT != puma7ModulesCfg[i].io_number)
        {
            PAL_sysGpioOutBit( puma7ModulesCfg[i].io_number, HW_VALUE_LED_OFF );
        }
    }

    return;
}

extern int led_manager_init(void);

static int led_callback_init(void)
{

    /* Enabling and Initialiasing the GPIO's mapped for the LED(s) */
    puma7_soc_is_led_gpios_enabled();
    /* Put in default state - OFF*/
    puma7_soc_led_gpio_init();

    led_manager_init();

    puma7_soc_led_callback_install();

    /* Start POWER LED at ON */
    PAL_sysGpioOutBit( puma7ModulesCfg[PUMA_LED_ID_POWER].io_number, HW_VALUE_LED_ON );

    return(0);
}

__initcall(led_callback_init);
