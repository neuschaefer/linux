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

  /******************************************************************************
 * FILE PURPOSE:     - LED HAL module source
 ******************************************************************************
 * FILE NAME:     led_hal.c
 *
 * DESCRIPTION:    LED HAL API's source.
 *
 *******************************************************************************/


/* Header Includes */

#include <asm-arm/arch-avalanche/generic/led_platform.h>
#include <asm-arm/arch-avalanche/generic/led_hal.h>
#include <linux/seq_file.h>
/* End of Header Includes */

#define REQUIRES_TIMER(x)  ((x) > LED_HAL_MODE_LED_ON)

#define TRUE              1
#define FALSE             0


struct led_obj_list;
struct state_obj;

typedef struct led_obj
{
    Int32 (*outVal)(Uint32 led_val, Uint32 pos_map, Uint32 index);
    struct led_obj      *next;
    struct led_obj_list *domain_list;
    struct state_obj    *curr_state;

    Uint32        pos_map       [ LED_HAL_BITMASK_REG_WIDTH ];
    Uint32        curr_led_val  [ LED_HAL_BITMASK_REG_WIDTH ];
    Uint32        off_led_val   [ LED_HAL_BITMASK_REG_WIDTH ];
    Uint32        module_map;      /* Not relevant for multicolored LED. */
    Uint32        timer_step;
    Uint32        is_single_color; /* Not multicolored LED; used to mux multiple modules on a single colored LED. */
    Uint32        ref_cnt;

} LED_OBJ_T;

typedef struct state_obj
{
    void (*handler)(struct state_obj *state);
    Uint32    id;
    Uint32    module_id;
    Uint32    mode;
    Uint32    timer_running;
    void      *os_timer;
    Uint32    param1;
    Uint32    param2;
    LED_OBJ_T *led;
    Uint32    prev_led_val  [ LED_HAL_BITMASK_REG_WIDTH ];
    Uint32    led_val       [ LED_HAL_BITMASK_REG_WIDTH ];
    Uint32    led_behaviour_id;

} STATE_OBJ_T;

typedef struct mod_obj
{
    Int8           name[50];
    Uint32         id;
    Uint32         instance;
    STATE_OBJ_T    *state[MAX_STATES_PER_MOD];
    struct mod_obj *next;
    Uint32         ref_cnt;

} MOD_OBJ_T;

typedef struct led_obj_list
{
    Uint32                 domain;
    LED_OBJ_T              *led;
    struct led_obj_list    *next;

} LED_OBJ_LIST_T;


static void led_assign_timer(STATE_OBJ_T *state);
static void led_assign_handler(STATE_OBJ_T *state);
static void led_goback_to_start_led_val(STATE_OBJ_T *state);
static inline void display_led(LED_OBJ_T *led, Uint32 * val);

static LED_OBJ_LIST_T *domain_led_collection = NULL; /* The LED collection across domains. */
static Uint32                      global_id = 0;    /* Assigns a run time sequential id to the modules being registered. */
static MOD_OBJ_T               *mod_obj_list = NULL; /* The module list. */
static Uint32 lock;
static Uint32 behaviour_id = 0; /* Free wheelign id to determine the behaviour of the LED,
                   used to take care of rapid LED triggers.*/

/*------------------------------------------------------------------------------
 * The private functions.
 *----------------------------------------------------------------------------*/
static LED_OBJ_LIST_T *find_led_obj_list(Uint32 domain)
{
    LED_OBJ_LIST_T *list;

    for(list = domain_led_collection; list; list = list->next)
        if(list->domain == domain)
            break;

    return (list);
}

static LED_OBJ_LIST_T *generate_led_obj_list(Uint32 domain)
{
    LED_OBJ_LIST_T   *new_list = find_led_obj_list(domain);

    if(new_list) return (new_list);

    if(PAL_osMemAlloc(0, sizeof(LED_OBJ_LIST_T), 4, (void *)&new_list))
        return (NULL);

    new_list->domain      = domain;
    new_list->led         = NULL;
    new_list->next        = domain_led_collection;

    domain_led_collection = new_list;

    return (new_list);
}

static Int32 destroy_led_obj_list(void)
{
    LED_OBJ_LIST_T *list = domain_led_collection;
    LED_OBJ_LIST_T *prev = NULL;

    /* Destroys the list which has no led_obj associated with it. */
    while(list)
    {
        LED_OBJ_LIST_T *this = list;
        list = list->next;

        if(this->led == NULL)
        {
            if(!prev)
            {
                domain_led_collection = this->next;
            }
            else
            {
                prev->next            = this->next;
            }

            PAL_osMemFree(0, this, sizeof(LED_OBJ_LIST_T));
        }
        else
        {
            prev = this;
        }
    }

    return (0);
}

static LED_OBJ_T *find_led_obj(Uint32 domain, Uint32 * position_map)
{
    LED_OBJ_LIST_T *list = find_led_obj_list(domain);
    LED_OBJ_T      *led;

    if(!list)
        return (NULL);

    for(led = list->led; led; led = led->next)
    {
        if(0 ==  memcmp(position_map, led->pos_map, LED_HAL_BITMASK_BYTE_WIDTH))
        break;
    }

    return (led);
}

#define is_single_color_map(m)         ((m & (m - 1)) ? FALSE : TRUE)

static LED_OBJ_T* generate_led_obj(Uint32 domain, Uint32 * position_map)
{
    LED_OBJ_T      *led_obj = find_led_obj(domain, position_map);
    LED_OBJ_LIST_T *list    = generate_led_obj_list(domain);


    if(led_obj) return (led_obj);

    if(!list)
        return (NULL);

    if(PAL_osMemAlloc(0, sizeof(LED_OBJ_T), 4, (void *)&led_obj))
        return (NULL);

    led_obj->outVal          = NULL;
    memcpy(led_obj->pos_map, position_map, LED_HAL_BITMASK_BYTE_WIDTH);
    led_obj->module_map      = 0;
    led_obj->domain_list     = list;
    {
        int idx;
        int single_color = 0;

        for(idx = 0; idx < LED_HAL_BITMASK_REG_WIDTH; idx++)
        {
            if( position_map[idx] )
            {
                if (is_single_color_map(position_map[idx]))
                {
                    single_color++;
                }
                else
                {
                    single_color+=2;
                }
            }
        }

        if (1 == single_color)
        {
            led_obj->is_single_color = TRUE;
        }
        else
        {
            led_obj->is_single_color = FALSE;
        }
    }
    led_obj->curr_state      = NULL;
    led_obj->ref_cnt         = 0;
    led_obj->next            = list->led;

    list->led                = led_obj;

    return (led_obj);
}

static Int32 destroy_led_obj(LED_OBJ_T *led_obj)
{
    LED_OBJ_LIST_T *list = led_obj->domain_list;
    LED_OBJ_T      *this = list->led;
    LED_OBJ_T      *prev = NULL;

    if(led_obj->ref_cnt)
        return (0);

    while(this)
    {
        if(this == led_obj)
    {
        if(!prev)
            list->led   = this->next;
        else
        prev->next  = this->next;

            PAL_osMemFree(0, this, sizeof(LED_OBJ_T));

        break;
    }

    prev = this;
    this = this->next;
    }

    destroy_led_obj_list();

    return (0);
}

static MOD_OBJ_T* find_module_obj(Int8*  name,
                                  Uint32 instance)
{
    MOD_OBJ_T *mod_obj;

    for(mod_obj = mod_obj_list; mod_obj; mod_obj = mod_obj->next)
        if(!strcmp(mod_obj->name, name) &&
       (mod_obj->instance == instance))
            break;

    return (mod_obj);
}

static MOD_OBJ_T* generate_module_obj(Int8*        name,
                                      Uint32 instance)
{
    MOD_OBJ_T *mod_obj = find_module_obj(name, instance);
    Int32          index;

    if(mod_obj)
        return (mod_obj);

    if(PAL_osMemAlloc(0, sizeof(MOD_OBJ_T), 4,(void *) &mod_obj))
        return (NULL);

    strcpy(mod_obj->name, name);

    mod_obj->id       = global_id++;
    mod_obj->instance = instance;

    for(index = 0; index < MAX_STATES_PER_MOD; index++)
        mod_obj->state[index] = NULL;

    mod_obj->ref_cnt  = 0;
    mod_obj->next     = mod_obj_list;
    mod_obj_list      = mod_obj;

    return (mod_obj);
}

static Int32 destroy_mod_obj(MOD_OBJ_T *mod_obj)
{
    MOD_OBJ_T *this = mod_obj_list;
    MOD_OBJ_T *prev = NULL;

    if(mod_obj->ref_cnt)
        return (0);

    while(this)
    {
        if(this == mod_obj)
        {
        if(!prev)
                mod_obj_list = this->next;
            else
                prev->next   = this->next;

            PAL_osMemFree(0, this, sizeof(MOD_OBJ_T));

            break;
        }

    prev = this;
    this = this->next;
    }

    return (0);
}

static Int32 led_assign_behaviour(MOD_OBJ_T *mod, STATE_OBJ_T *state)
{
    Int32 index,matched=FALSE,ret_val=0;

    for(index = 0; index < MAX_STATES_PER_MOD; index++)
    {
        STATE_OBJ_T *mod_state = mod->state[index];

        if((!mod_state) || (mod_state == state))
            continue;

        if(mod_state->mode != state->mode)
            continue;

        if(mod_state->led != state->led)
            continue;

        if(0 != memcmp(mod_state->led_val, state->led_val, LED_HAL_BITMASK_BYTE_WIDTH))
            continue;

        matched = TRUE;

        switch(state->mode)
        {
            case LED_HAL_MODE_LED_ONESHOT_BACK:
            case LED_HAL_MODE_LED_ONESHOT_ON:
            case LED_HAL_MODE_LED_ONESHOT_OFF:
                if(mod_state->param1 != mod_state->param1)
                    matched = FALSE;
                break;

            case LED_HAL_MODE_LED_FLASH:
            case LED_HAL_MODE_LED_FLASH_BACK:
                if((mod_state->param1 != mod_state->param1) ||
                   (mod_state->param2 != mod_state->param2))
                    matched = FALSE;
                break;

            default:
                ret_val = -1;
                goto assign_behave_exit;
                break;
        }

        if (matched)
            break;

    }

    if (matched)
        state->led_behaviour_id = mod->state[index]->led_behaviour_id;
    else
        state->led_behaviour_id = behaviour_id++;

assign_behave_exit:
    return (ret_val);
}

static void destroy_mod_state(MOD_OBJ_T *mod_obj, STATE_OBJ_T *state)
{
    LED_OBJ_T *led = state->led;
    Int32       index;

    if(state->os_timer)
        os_timer_destroy(state->os_timer);

    for(index = 0; index < MAX_STATES_PER_MOD; index++)
        if(mod_obj->state[index] == state)
        mod_obj->state[index] = NULL;

     PAL_osMemFree(0, state, sizeof(STATE_OBJ_T));

    if((!(--led->ref_cnt)) && (!led->outVal))
        destroy_led_obj(led);
}


static STATE_OBJ_T *configure_mod_state(MOD_OBJ_T *mod_obj, STATE_CFG_T *cfg)
{
    LED_OBJ_T    *led;
    STATE_OBJ_T  *state;

    if(cfg->id >= MAX_STATES_PER_MOD)
        return (NULL);

    if(mod_obj->state[cfg->id]) /* We are over-writing the current state info. */
        destroy_mod_state(mod_obj, mod_obj->state[cfg->id]);

    if(PAL_osMemAlloc(0, sizeof(STATE_OBJ_T), 4,(void *) &state))
        return (NULL);

    led = generate_led_obj(cfg->led_cfg.domain, cfg->led_cfg.pos_map);

    if(!led)
    {
        PAL_osMemFree(0, state, sizeof(STATE_OBJ_T));
        return (NULL);
    }

    state->id              = cfg->id;
    state->module_id       = mod_obj->id;
    state->mode            = cfg->mode;
    state->os_timer        = NULL;
    state->timer_running   = FALSE;
    state->param1          = 0;
    state->param2          = 0;

    state->led             = led;
    memcpy(state->led_val, cfg->led_val, LED_HAL_BITMASK_BYTE_WIDTH);

    led->ref_cnt++;

    led_assign_handler(state);

    if(REQUIRES_TIMER(state->mode))
    {
        led_assign_timer(state);

    if(state->os_timer)
    {
            state->param1  = cfg->param1;
            state->param2  = cfg->param2;
        }
    else
        {
            PAL_osMemFree(0, state, sizeof(STATE_OBJ_T));
        return (NULL);
        }
    }

    led_assign_behaviour(mod_obj, state);

    return (state);
}

static void led_hal_cleanup_mod_config(void)
{
    MOD_OBJ_T *mod_obj = mod_obj_list;

    while(mod_obj)
    {
        Int32 index;
    for(index = 0; index < MAX_STATES_PER_MOD; index++)
        if(mod_obj->state[index])
                destroy_mod_state(mod_obj, mod_obj->state[index]);

    mod_obj->ref_cnt--;

    mod_obj = mod_obj->next;
    }
}

/*-------------------------------------------------------------------------------
 * Exports to the driver modules.
 *-----------------------------------------------------------------------------*/
Int32 led_hal_configure_mod(MOD_CFG_T *cfg)
{
    MOD_OBJ_T   *mod_obj;
    STATE_OBJ_T *state;

    if(!cfg->name || cfg->instance > 255)
        return (-1);

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    mod_obj = generate_module_obj(cfg->name, cfg->instance);
    if(!mod_obj)
    {
        PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);
        return (-1);
    }

    mod_obj->ref_cnt++;

    state = configure_mod_state(mod_obj, &cfg->state_cfg);
    if(!state)
    {
        PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);
        return (-1);
    }

    mod_obj->state[state->id] = state;
    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

    return (0);
}

MOD_OBJ_HND* led_hal_register(Int8 *name, Uint32 instance)
{
    MOD_OBJ_T *mod_obj;

    if(!name || instance > 255)
        return (NULL);

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    mod_obj = generate_module_obj(name, instance);

    if(mod_obj)
        mod_obj->ref_cnt++;

    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

    return((MOD_OBJ_HND*)mod_obj);
}

void led_hal_unregister(MOD_OBJ_HND *mod)
{
    MOD_OBJ_T *mod_obj = (MOD_OBJ_T*)mod;

    if(!mod_obj)
        return;

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    if(!(--mod_obj->ref_cnt))
        destroy_mod_obj(mod_obj);

    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);
}

LED_OBJ_HND* led_hal_install_callbacks(LED_FUNCS_T *led_funcs)
{
    LED_OBJ_T *led_obj;

    if(!led_funcs)
        return (NULL);

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    led_obj = generate_led_obj(led_funcs->domain, led_funcs->pos_map);
    if(!led_obj)
    {
        PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);
        return (NULL);
    }

    led_obj->outVal          = led_funcs->outVal;
    memcpy(led_obj->off_led_val, led_funcs->off_val, LED_HAL_BITMASK_BYTE_WIDTH);

    display_led(led_obj, led_funcs->off_val);

    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

    return (led_obj);
}

Int32 led_hal_uninstall_callbacks(LED_OBJ_HND *led)
{
    LED_OBJ_T *led_obj = (LED_OBJ_T*)led;

    if(!led_obj)
        return (-1);

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    led_obj->outVal = NULL;

    if(!led_obj->ref_cnt)
        destroy_led_obj(led_obj);

    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

    return (0);
}

Int32 led_hal_exit()
{
    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    led_hal_cleanup_mod_config();
    destroy_mod_obj(mod_obj_list);

    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

    return (0);
}

Int32 led_hal_init()
{
    return (0);
}

Int32 led_hal_action(MOD_OBJ_HND *mod, Uint32 state_id)
{
    MOD_OBJ_T   *mod_obj = (MOD_OBJ_T*) mod;
    STATE_OBJ_T *state   = mod_obj->state[state_id];

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    if(state)
    {
        LED_OBJ_T          *led = state->led;
        STATE_OBJ_T *prev_state = led->curr_state;

        if(prev_state)
        {
            if(prev_state->timer_running == TRUE)
            {
                if(prev_state->led_behaviour_id == state->led_behaviour_id)
                {
                    /* If the timer is running for the previous state and that behaviour
                     * of the new state is same as that of the previous state, then, the
                     * actions are being repeated, so let us not meddle with the current
                     * timer action.  */
                    goto ret_from_action;
                }
                else
                {
                    /* We have a new action to perform, lets shut down the running timer
                     * for the previous state. Then, we work with the new state. */
                    prev_state->timer_running = FALSE;
                    os_timer_delete(prev_state->os_timer);
                    led_goback_to_start_led_val(prev_state);
                }
            }
            else if(!prev_state->os_timer)
            {
                /* The previous state did not have any timer associated with it, the new
                 * behaviour is same as that what has been reflected on the LED then, we
                 * do have to do anything, let us maintain status quo. */
                if(prev_state->led_behaviour_id == state->led_behaviour_id)
                    goto ret_from_action;
            }
            else
            {
                ;/* We need to work with the new state. */
            }
        }

        led->curr_state = state;
        state->handler(state);
    }

ret_from_action:

    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

    return (0);
}

/*-----------------------------------------------------------------------------
 * LED state handlers.
 *---------------------------------------------------------------------------*/
static inline void display_led(LED_OBJ_T *led, Uint32 * val)
{
    int idx;
    if(!led->outVal)
        return;

    memcpy(led->curr_led_val, val, LED_HAL_BITMASK_BYTE_WIDTH);

    for (idx = 0; idx <LED_HAL_BITMASK_REG_WIDTH; idx++)
    {
        if(led->pos_map[idx])
        {
            led->outVal(val[idx], led->pos_map[idx], idx);
        }
    }
}

static void led_on(STATE_OBJ_T * state)
{
    LED_OBJ_T *led = state->led;

    led->module_map |= (1 << state->module_id);
    display_led(led, state->led_val);

    return;
}

static void led_off(STATE_OBJ_T * state)
{
    LED_OBJ_T *led = state->led;

    led->module_map &= ~(1 << state->module_id);

    if(led->is_single_color && led->module_map)
        return; /* Modules are still driving this single color LED. */

    display_led(led, state->led_val);
}

static void led_toggle(STATE_OBJ_T * state)
{
    LED_OBJ_T *led = state->led;

    if(0 == memcmp(state->led_val, led->off_led_val, LED_HAL_BITMASK_BYTE_WIDTH))
        led_off(state);
    else
        led_on(state);
}

static void led_oneshot_back (STATE_OBJ_T * state)
{

    LED_OBJ_T *led = state->led;

    state->timer_running   = TRUE;
    led->timer_step        = 1;

    memcpy(state->prev_led_val, led->curr_led_val, LED_HAL_BITMASK_BYTE_WIDTH);

    display_led(led, state->led_val);

    /* We timeout after 80% of the stipulated time, otherwise we do not see any
     * flickering under high rate of dispatch of this function.
     * We use the ramaining 20% to indicate the previous LED state so that the
     * user see some flickering on the LED.
     */
     os_timer_add (state->os_timer, ( ( state->param1 << 3 ) / 10 ),(Int32) state);
}

static void led_oneshot_on (STATE_OBJ_T * state)
{
    LED_OBJ_T *led = state->led;

    state->timer_running   = TRUE;
    led->timer_step        = 1;
    memcpy(state->prev_led_val, led->curr_led_val,LED_HAL_BITMASK_BYTE_WIDTH);

    display_led(led, state->led_val);

    /* We timeout after 80% of the stipulated time, otherwise we do not see any
     * flickering under high rate of dispatch of this function.
     * We use the ramaining 20% to indicate the previous LED state so that the
     * user see some flickering on the LED.
     */
    os_timer_add (state->os_timer, ( ( state->param1 << 3 ) / 10 ),(Int32) state);
}

static void led_flash (STATE_OBJ_T * state)
{
    LED_OBJ_T *led;

    led = state->led;

    state->timer_running   = TRUE;
    led->timer_step        = 1;
    memcpy(state->prev_led_val, led->curr_led_val,LED_HAL_BITMASK_BYTE_WIDTH);

    display_led(led, state->led_val);
    os_timer_add (state->os_timer, state->param1,(Int32) state);

}

static void led_oneshot_off (STATE_OBJ_T * state)
{

    LED_OBJ_T *led = state->led;

    state->timer_running   = TRUE;
    led->timer_step        = 1; //LED_HAL_MODE_LED_ONESHOT_OFF;
    memcpy(state->prev_led_val, led->curr_led_val, LED_HAL_BITMASK_BYTE_WIDTH);
    display_led(led, led->off_led_val);

    /* We timeout after 80% of the stipulated time, otherwise we do not see any
     * flickering under high rate of dispatch of this function.
     * We use the ramaining 20% to indicate the previous LED state so that the
     * user see some flickering on the LED.
     */
     os_timer_add (state->os_timer, ( ( state->param1 << 3 ) / 10 ),(Int32) state);
}

static void led_flash_back(STATE_OBJ_T *state)
{
    LED_OBJ_T *led = state->led;

    state->timer_running   = TRUE;
    led->timer_step        = 1;
    memcpy(state->prev_led_val, led->curr_led_val, LED_HAL_BITMASK_BYTE_WIDTH);
    state->prev_led_val[1] = led->curr_led_val[1];
    display_led(led, state->led_val);
    os_timer_add (state->os_timer, state->param1,(Int32) state);

}

static void led_oneshot_back_timer_func (Int32 arg)
{
    STATE_OBJ_T *state = (STATE_OBJ_T*) arg;
    LED_OBJ_T     *led = state->led;

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    if (state->timer_running == FALSE) /* Timer being shut down or false trigger. */
        goto ret_from_led_oneshot_back_timer;

    if (led->timer_step == 1)
    {
        led->timer_step = 2; /* Lets do the second step of the timer execution. */;
        display_led(led, state->prev_led_val);

    /* Lets us follow through the remaining 20% of the stipulated time. */
        os_timer_add ( state->os_timer, ( ( state->param1 << 1 ) / 10 ), (Int32)state);
    }
    else
    {
        /* This is step 2; No more steps for the timer, so the timer will now rest. */
        state->timer_running = FALSE;
    }

ret_from_led_oneshot_back_timer:
    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

}

static void led_oneshot_on_timer_func (Int32 arg)
{
    STATE_OBJ_T *state = (STATE_OBJ_T*) arg;
    LED_OBJ_T     *led = state->led;

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    if (state->timer_running == FALSE) /* Timer being shut down or false trigger. */
        goto ret_from_led_oneshot_on_timer;

    if (led->timer_step == 1)
    {
        led->timer_step = 2; /* Lets do the second step of the timer execution. */;
        display_led(led, led->off_led_val);

    /* Lets us follow through the remaining 20% of the stipulated time. */
        os_timer_add ( state->os_timer, ( ( state->param1 << 1 ) / 10 ), (Int32)state);
    }
    else
    {
        /* This is step 2; No more steps for the timer, so the timer will now rest. */
        state->timer_running = FALSE;
    }

ret_from_led_oneshot_on_timer:
    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

}

static void led_flash_timer_func (Int32 arg)
{
    STATE_OBJ_T *state = (STATE_OBJ_T*) arg;
    LED_OBJ_T     *led = state->led;

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    if (state->timer_running == FALSE) /* Timer being shut down or false trigger. */
        goto ret_from_led_flash_timer;

    if (led->timer_step == 1)
    {
        led->timer_step = 2; /* Lets do the second step of the timer execution. */;
        display_led(led, led->off_led_val);
        os_timer_add (state->os_timer, state->param2, (Int32)state);
    }
    else
    {
        led->timer_step = 1; /* We need to go back to the step of the timer. */
        display_led(led, state->led_val);
        os_timer_add (state->os_timer, state->param1, (Int32)state);
    }

ret_from_led_flash_timer:
    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

}

static void led_oneshot_off_timer_func (Int32 arg)
{
    STATE_OBJ_T *state = (STATE_OBJ_T*) arg;
    LED_OBJ_T     *led = state->led;

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    if (state->timer_running == FALSE) /* Timer being shut down or false trigger. */
        goto ret_from_led_oneshot_off_timer;

    if (led->timer_step == 1)
    {
        led->timer_step = 2; /* Lets do the second step of the timer execution. */;
        display_led(led, state->led_val);

    /* Lets us follow through the remaining 20% of the stipulated time. */
        os_timer_add ( state->os_timer, ( ( state->param1 << 1 ) / 10 ), (Int32)state);
    }
    else
    {
        /* This is step 2; No more steps for the timer, so the timer will now rest. */
         state->timer_running = FALSE;
    }

ret_from_led_oneshot_off_timer:
    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

}

static void led_flash_back_timer_func (Int32 arg)
{
    STATE_OBJ_T *state = (STATE_OBJ_T*) arg;
    LED_OBJ_T     *led = state->led;

    PAL_osProtectEntry (PAL_OSPROTECT_INTERRUPT, &lock);

    if (state->timer_running == FALSE) /* Timer being shut down or false trigger. */
        goto ret_from_led_flash_back_timer;

    if (led->timer_step == 1)
    {
        led->timer_step = 2; /* Lets do the second step of the timer execution. */;
        display_led(led, state->prev_led_val);
        os_timer_add (state->os_timer, state->param2, (Int32)state);
    }
    else
    {
        led->timer_step = 1; /* Lets go back to the step 1 of the timer. */
        display_led(led, state->led_val);
        os_timer_add (state->os_timer, state->param1, (Int32)state);
    }

ret_from_led_flash_back_timer:
    PAL_osProtectExit (PAL_OSPROTECT_INTERRUPT, lock);

}

static void led_goback_to_start_led_val(STATE_OBJ_T *state)
{
    LED_OBJ_T *led=state->led;

    switch(state->mode)
    {
        case LED_HAL_MODE_LED_ONESHOT_BACK:
             display_led(led, state->prev_led_val);
             break;

        case LED_HAL_MODE_LED_ONESHOT_ON:
             display_led(led, led->off_led_val);
             break;

        case LED_HAL_MODE_LED_FLASH:
             display_led(led, led->off_led_val);
             break;

        case LED_HAL_MODE_LED_ONESHOT_OFF:
             display_led(led, state->led_val);
             break;

        case LED_HAL_MODE_LED_FLASH_BACK:
             display_led(led, state->prev_led_val);
             break;

        default:
             break;
    }

}

static void led_assign_handler(STATE_OBJ_T *state)
{

    switch(state->mode)
    {
        case LED_HAL_MODE_LED_ON:
            state->handler = led_toggle;
            break;

        case LED_HAL_MODE_LED_OFF:
            state->handler = led_toggle;
            break;

        case LED_HAL_MODE_LED_ONESHOT_BACK:
             state->handler = led_oneshot_back;
             break;

        case LED_HAL_MODE_LED_ONESHOT_ON:
             state->handler = led_oneshot_on;
             break;

        case LED_HAL_MODE_LED_FLASH:
             state->handler = led_flash;
             break;

        case LED_HAL_MODE_LED_ONESHOT_OFF:
             state->handler = led_oneshot_off;
             break;

        case LED_HAL_MODE_LED_FLASH_BACK:
             state->handler = led_flash_back;
             break;

        default:
             state->handler = NULL;
             break;
    }

}

static void led_assign_timer(STATE_OBJ_T *state)
{

    if (state->os_timer)
    {
        os_timer_destroy(state->os_timer);
    }

    switch(state->mode)
    {
        case LED_HAL_MODE_LED_ONESHOT_BACK:
             state->os_timer=os_timer_init(led_oneshot_back_timer_func);
             break;

        case LED_HAL_MODE_LED_ONESHOT_ON:
             state->os_timer=os_timer_init(led_oneshot_on_timer_func);
             break;

        case LED_HAL_MODE_LED_FLASH:
             state->os_timer = os_timer_init(led_flash_timer_func);
             break;

        case LED_HAL_MODE_LED_ONESHOT_OFF:
             state->os_timer=os_timer_init(led_oneshot_off_timer_func);
             break;

        case LED_HAL_MODE_LED_FLASH_BACK:
             state->os_timer = os_timer_init(led_flash_back_timer_func);
             break;

        default:
             state->os_timer = NULL;
             break;
    }

}

/*-----------------------------------------------------------------------------
 * Dump function.
 *---------------------------------------------------------------------------*/

Int32 led_hal_dump_cfg_info(struct seq_file *m)
{
    MOD_OBJ_T *mod_obj = mod_obj_list;
    Int8   *mode_str[] ={"led off", "led on", "led blink back", "led blink on",
                     "led flash", "led blink off", "led flash back"};

    while(mod_obj)
    {
        Int32 index     = 0;
        Int32 index1    = 0;
        Int32 index2    = 0;

        seq_printf(m, "Module name: %10s, instance: %5d.\n\n", mod_obj->name, mod_obj->instance);

        for(index = 0; index < MAX_STATES_PER_MOD; index++)
        {
            STATE_OBJ_T *state = mod_obj->state[index];
            LED_OBJ_T   *led;
            Uint32 pos_map;

            if(!state)
                continue;

            seq_printf(m, "\tState#: %10d, mode: %15s", index, mode_str[state->mode]);

            if(state->param1)
                seq_printf(m, ", param1: %5d", state->param1);

            if(state->param2)
                seq_printf(m, ", param2: %5d", state->param2);
            else
                seq_printf(m, "\n");

            seq_printf(m, "\n");

            led = state->led;

            if(led)
                seq_printf(m, "\tLED Domain#: %5d, %5s colored,", led->domain_list->domain,
                                                 led->is_single_color ? "single":"multi");

            seq_printf(m, "\tbits:");

            for (index2 = 0; index2<LED_HAL_BITMASK_REG_WIDTH; index2++)
            {
                pos_map = led->pos_map[index2];
                for(index1 = 0; pos_map; pos_map >>= 1, index1++)
                {
                    if(pos_map & 0x1)
                        seq_printf(m, "%3d", index1 + index2*32);
                }
            }


            seq_printf(m, "\n");
        }

        seq_printf(m, "\n");

        mod_obj = mod_obj->next;
        if (m->count == m->size)
            break;
    }

    return 0;
}


