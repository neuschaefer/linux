/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/input.h>
#include <linux/switch.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <sound/soc.h>
#include <sound/wm8903.h>
#include "nbx_jack.h"
#include "../codecs/wm8903.h"

enum nbx_jack_state_t {
	_NBX_JACK_STATE_START = 0,
	_NBX_JACK_STATE_SP,
	_NBX_JACK_STATE_HP,
	_NBX_JACK_STATE_HS,
	_NBX_JACK_STATE_HOOKSWITCH_PRESSING,
	_NBX_JACK_STATE_TRANSITION,
	_NBX_JACK_STATE_NUM
};

enum hook_switch_state_t {
	_HOOK_SWITCH_RELEASED = 0,
	_HOOK_SWITCH_PRESSED = 1,
};

/* These values are copied from Android WiredAccessoryObserver */
enum headset_state_t {
	BIT_NO_HEADSET = 0,
	BIT_HEADSET = (1 << 0),
	BIT_HEADSET_NO_MIC = (1 << 1),
};

#define  _HOOK_SWITCH_DEV_NAME  "h2w"
#define  _TIMER_TICK            (10)    /* ms */
#define  _COUNT_DEFAULT		(30)    /* ms */
#define  _COUNT_TRANSITION	(200)   /* ms */

struct nbx_jack_t {
	enum nbx_jack_state_t state;
	int is_plugged;
	int is_short;
	int ctr;

	struct snd_soc_codec *codec;
	struct input_dev *hook_switch;
	struct work_struct work;
	struct timer_list timer;
	struct switch_dev *h2w;
	int plug_det_gpio;
	int codec_hp_det_gpio;
	int codec_short_gpio;
	int plug_det_irq;
	int codec_hp_det_irq;
	int codec_short_irq;
};

static struct switch_dev _h2w = { .name = _HOOK_SWITCH_DEV_NAME };
static struct nbx_jack_t *nbx_jack;

inline void
_start_timer(void)
{
	mod_timer(&nbx_jack->timer,
		jiffies + msecs_to_jiffies(_TIMER_TICK));
}

static void
_update_h2w_state(enum headset_state_t h2w_state)
{
  //printk(KERN_ERR "XXX: nbx_jack: h2w: %d\n", (int)h2w_state);
	if ( !nbx_jack->h2w ) {
		printk(KERN_ERR "nbx_jack: h2w == NULL\n");
		return;
	}
	switch_set_state(nbx_jack->h2w, (int)h2w_state);
}

static void
_update_hook_switch_state(enum hook_switch_state_t hksw_state)
{
  //printk(KERN_ERR "XXX: nbx_jack: input: %d\n", (int)hksw_state);
	if ( !nbx_jack->hook_switch ) {
		printk(KERN_ERR "nbx_jack: hook_switch == NULL\n");
		return;
	}
	input_report_key(nbx_jack->hook_switch, KEY_MEDIA, (int)hksw_state);
	input_sync(nbx_jack->hook_switch);
}

static void
_state_machine(void)
{
	int next_state;
	if ( !nbx_jack ) {
		printk(KERN_ERR "nbx_jack: _state_machine: nbx_jack == NULL\n");
		return;
	}
	next_state = nbx_jack->state;

	switch ( nbx_jack->state ) {
	case _NBX_JACK_STATE_START:
		next_state = _NBX_JACK_STATE_SP;
		/* go to next statement */
	case _NBX_JACK_STATE_SP:
		if ( nbx_jack->is_plugged ) { /* HP or HS */
			if ( nbx_jack->is_short ) {
				/* HP */
				next_state = _NBX_JACK_STATE_TRANSITION;
			} else {
				/* HS */
				next_state = _NBX_JACK_STATE_TRANSITION;
			}
		} else {
			/* SP */
		}
		break;
	case _NBX_JACK_STATE_HP:
		if ( nbx_jack->is_plugged ) { /* HP or HS */
			if ( nbx_jack->is_short ) {
				/* HP */
			} else {
				/* HS */
			}
		} else {
			/* SP */
			next_state = _NBX_JACK_STATE_SP;
		}
		break;
	case _NBX_JACK_STATE_HS:
		if ( nbx_jack->is_plugged ) { /* HP or HS */
			if ( nbx_jack->is_short ) {
				/* Hook Switch Pressed */
				next_state = _NBX_JACK_STATE_HOOKSWITCH_PRESSING;
			} else {
				/* HS */
			}
		} else {
			/* SP */
			next_state = _NBX_JACK_STATE_SP;
		}
		break;
	case _NBX_JACK_STATE_HOOKSWITCH_PRESSING:
		if ( nbx_jack->is_plugged ) { /* HP or HS */
			if ( nbx_jack->is_short ) {
				/* HP */
			} else {
				/* HS */
				next_state = _NBX_JACK_STATE_HS;
			}
		} else {
			/* SP */
			next_state = _NBX_JACK_STATE_SP;
		}
		break;
	case _NBX_JACK_STATE_TRANSITION:
		if ( nbx_jack->is_plugged ) { /* HP or HS */
			if ( nbx_jack->is_short ) {
				/* HP */
				next_state = _NBX_JACK_STATE_HP;
			} else {
				/* HS */
				next_state = _NBX_JACK_STATE_HS;
			}
		} else {
			/* SP */
			next_state = _NBX_JACK_STATE_SP;
		}
		break;
	default:
		printk(KERN_ERR "nbx_jack: unknown state: %d\n", nbx_jack->state);
		break;
	}

	if ( next_state != nbx_jack->state ) { /* state changed */
		printk(KERN_ERR "nbx_jack: state [%d -> %d]\n", nbx_jack->state, next_state);

		/* exit action */
		switch ( nbx_jack->state ) {
		case _NBX_JACK_STATE_START:
			break;
		case _NBX_JACK_STATE_SP:
			break;
		case _NBX_JACK_STATE_HP:
			break;
		case _NBX_JACK_STATE_HS:
			break;
		case _NBX_JACK_STATE_HOOKSWITCH_PRESSING:
			_update_hook_switch_state(_HOOK_SWITCH_RELEASED);
			break;
		case _NBX_JACK_STATE_TRANSITION:
			break;
		default:
			printk(KERN_ERR "nbx_jack: unknown state: %d\n", next_state);
			break;
		}

		/* entry action */
		switch ( next_state ) {
		case _NBX_JACK_STATE_START:
			printk(KERN_ERR "nbx_jack: cannot happen state: %d\n", next_state);
			break;
		case _NBX_JACK_STATE_SP:
			_update_h2w_state(BIT_NO_HEADSET);
			break;
		case _NBX_JACK_STATE_HP:
			_update_h2w_state(BIT_HEADSET_NO_MIC);
			break;
		case _NBX_JACK_STATE_HS:
			_update_h2w_state(BIT_HEADSET);
			break;
		case _NBX_JACK_STATE_HOOKSWITCH_PRESSING:
			_update_hook_switch_state(_HOOK_SWITCH_PRESSED);
			break;
		case _NBX_JACK_STATE_TRANSITION:
			/* required codec_short polling */
			_start_timer();
			break;
		default:
			printk(KERN_ERR "nbx_jack: unknown state: %d\n", next_state);
			break;
		}

		nbx_jack->state = next_state;
	}

}

static void
_polling_gpio(void)
{
	int new_is_plugged;
	int new_is_short = 0;
	int expired_time;
	if ( !nbx_jack ) {
		printk(KERN_ERR "nbx_jack: _polling_gpio: nbx_jack == NULL\n");
		return;
	}
	// printk(KERN_ERR "nbx_jack: _polling_gpio: GPIO (%d %d %d)\n", 
	// nbx_jack->plug_det_gpio, nbx_jack->codec_hp_det_gpio, nbx_jack->codec_short_gpio);
	new_is_plugged = (gpio_get_value(nbx_jack->plug_det_gpio) ? 0 : 1) |
			 (gpio_get_value(nbx_jack->codec_hp_det_gpio) ? 1 : 0);
	if ( new_is_plugged == nbx_jack->is_plugged ) {
		if ( new_is_plugged ) {
			new_is_short = (gpio_get_value(nbx_jack->codec_short_gpio) ? 1 : 0);
			if ( new_is_short == nbx_jack->is_short ) {
				nbx_jack->ctr++;
			} else {
				nbx_jack->ctr = 0;
			}
		} else {
			nbx_jack->ctr++;
		}
	} else {
		nbx_jack->ctr = 0;
	}

	if ( nbx_jack->ctr > 0 ) {
	  //printk(KERN_ERR "XXX: nbx_jack: _polling_gpio: ctr: %d\n", nbx_jack->ctr);
		expired_time = ( new_is_plugged && nbx_jack->state == _NBX_JACK_STATE_TRANSITION )
				? _COUNT_TRANSITION : _COUNT_DEFAULT;
		if ( nbx_jack->ctr >= (expired_time / _TIMER_TICK) ) {
		  //printk(KERN_ERR "XXX: nbx_jack: _polling_gpio: expired: %d\n", expired_time);
			nbx_jack->ctr = 0;
			_state_machine();
		} else {
			/* continue polling until ctr is expired */
			_start_timer();
		}
	} else {
		nbx_jack->is_plugged = new_is_plugged;
		nbx_jack->is_short   = new_is_short;
		_start_timer();
	}
}

static void
_timer_handler(unsigned long _data)
{
	struct nbx_jack_t *pdata = (struct nbx_jack_t *)_data;

	schedule_work(&pdata->work);
}

static irqreturn_t
nbx_jack_gpio_interrupt(int irq, void *dummy)
{
	// printk(KERN_ERR "nbx_jack: nbx_jack_gpio_interrupt(%d)\n", irq);
	if ( !nbx_jack ) {
		printk(KERN_ERR "nbx_jack: irq: nbx_jack == NULL\n");
		return IRQ_HANDLED;
	}
	_start_timer();
	return IRQ_HANDLED;
}

static void
nbx_jack_intr_work(struct work_struct *work)
{
	// printk("nbx_jack: timer fired\n");
	_polling_gpio();
}

static int
_register_gpio_interrupt(int _gpio, char* _name, irq_handler_t _interrupt, void *_dev)
{
	int error = -1;
	int irq = -1;
	int trigger = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;

	error = gpio_request(_gpio, _name);
	if ( error < 0 ) {
		printk("nbx_jack: Failed to request GPIO %d, error %d\n", _gpio, error);
		return -1;
	}

	error = gpio_direction_input(_gpio);
	if ( error < 0 ) {
		printk("nbx_jack: Failed to configure direction for GPIO %d, error %d\n", _gpio, error);
		return -1;
	}

	irq = gpio_to_irq(_gpio);
	if ( irq < 0 ) {
		printk("nbx_jack: Unable to get irq number for GPIO %d, error %d\n", _gpio, error);
		return -1;
	}

	if ( request_irq(irq, _interrupt, trigger, _name, _dev) ) {
		printk(KERN_ERR "nbx_jack: Can't allocate IRQ %d, GPIO %d\n", irq, _gpio);
		return -EBUSY;
	}

	return irq; /* success: irq >= 0 */
}

static void
_enable_codec_interrupt(void)
{
	if ( !nbx_jack ) {
		printk(KERN_ERR "nbx_jack: _enable_codec_interrupt: nbx_jack == NULL\n");
		return;
	}
	snd_soc_update_bits(nbx_jack->codec, WM8903_INTERRUPT_STATUS_1_MASK,
				WM8903_MICDET_EINT | WM8903_MICSHRT_EINT, 0);
	snd_soc_update_bits(nbx_jack->codec, WM8903_WRITE_SEQUENCER_0,
				WM8903_WSEQ_ENA, WM8903_WSEQ_ENA);
	snd_soc_update_bits(nbx_jack->codec, WM8903_MIC_BIAS_CONTROL_0,
				WM8903_MICDET_ENA, WM8903_MICDET_ENA);
}

int
nbx_jack_dev_register(struct nbx_jack_device_t *pdev)
{
	int error = -1;
	//printk(KERN_ERR "XXX: nbx_jack: nbx_jack_dev_register\n");
	if ( !pdev->codec ) {
		printk(KERN_ERR "nbx_jack: nbx_jack_dev_register: codec == NULL\n");
		return -1;
	}

	/* initialize work area */
	nbx_jack = kzalloc(sizeof(struct nbx_jack_t), GFP_KERNEL);
	if ( !nbx_jack )
		return -ENOMEM;

	/* copy to local member */
	nbx_jack->codec = pdev->codec;
	nbx_jack->plug_det_gpio = pdev->plug_det_gpio;
	nbx_jack->codec_hp_det_gpio = pdev->codec_hp_det_gpio;
	nbx_jack->codec_short_gpio = pdev->codec_short_gpio;

	/* initialize other member */
	nbx_jack->plug_det_irq = -1;
	nbx_jack->codec_hp_det_irq = -1;
	nbx_jack->codec_short_irq = -1;
	nbx_jack->h2w = &_h2w;

	/* initialize timer and workqueue */
	setup_timer(&nbx_jack->timer, _timer_handler, (unsigned long)nbx_jack);
	INIT_WORK(&nbx_jack->work, nbx_jack_intr_work);

	/* initialize GPIO and IRQ */
	nbx_jack->codec_hp_det_irq = _register_gpio_interrupt(
						nbx_jack->codec_hp_det_gpio,
						"codec_hp_det",
						nbx_jack_gpio_interrupt,
						nbx_jack);
	if ( nbx_jack->codec_hp_det_irq < 0 ) {
		error = nbx_jack->codec_hp_det_irq;
		goto err_free_irq;
	}

	nbx_jack->codec_short_irq = _register_gpio_interrupt(
						nbx_jack->codec_short_gpio,
						"codec_short",
						nbx_jack_gpio_interrupt,
						nbx_jack);
	if ( nbx_jack->codec_short_irq < 0 ) {
		error = nbx_jack->codec_short_irq;
		goto err_free_irq;
	}

	nbx_jack->plug_det_irq = _register_gpio_interrupt(
						nbx_jack->plug_det_gpio,
						"plug_det",
						nbx_jack_gpio_interrupt,
						nbx_jack);
	if ( nbx_jack->plug_det_irq < 0 ) {
		error = nbx_jack->plug_det_irq;
		goto err_free_irq;
	}

	/* initialize hook switch (input class driver) */
	nbx_jack->hook_switch = input_allocate_device();
	if ( !nbx_jack->hook_switch ) {
		printk(KERN_ERR "nbx_jack: Not enough memory\n");
		error = -ENOMEM;
		goto err_free_irq;
	}
	nbx_jack->hook_switch->name = "hook_switch";
	set_bit(EV_KEY, nbx_jack->hook_switch->evbit);
	set_bit(KEY_MEDIA, nbx_jack->hook_switch->keybit);
	error = input_register_device(nbx_jack->hook_switch);
	if ( error ) {
		printk(KERN_ERR "nbx_jack: Failed to register device\n");
		goto err_free_input_device;
	}

	/* initialize h2w switch (switch class driver) */
	error = switch_dev_register(nbx_jack->h2w);
	if ( error < 0 )
		goto err_free_switch_device;
	
	_enable_codec_interrupt();

	return 0;

	/* error handling */
 err_free_switch_device:;
	switch_dev_unregister(nbx_jack->h2w);
 err_free_input_device:;
	if ( nbx_jack->hook_switch ) {
		input_free_device(nbx_jack->hook_switch);
		nbx_jack->hook_switch = NULL;
	}
 err_free_irq:;
	if ( nbx_jack->plug_det_irq >= 0 )
		free_irq(nbx_jack->plug_det_irq, nbx_jack_gpio_interrupt);
	if ( nbx_jack->codec_hp_det_irq >=0 )
		free_irq(nbx_jack->codec_hp_det_irq, nbx_jack_gpio_interrupt);
	if ( nbx_jack->codec_short_irq >= 0 )
		free_irq(nbx_jack->codec_short_irq, nbx_jack_gpio_interrupt);
	/* err_free_memory:; */
	kfree(nbx_jack);
	nbx_jack = NULL;

	printk(KERN_ERR "nbx_jack: error on initializing (%d), exit\n", error);
	return error;
}

void
nbx_jack_dev_unregister(void)
{
	if ( !nbx_jack ) {
		printk(KERN_ERR "nbx_jack: nbx_jack_dev_unregister: nbx_jack == NULL\n");
		return;
	}
	switch_dev_unregister(nbx_jack->h2w);
	if ( nbx_jack->hook_switch ) {
		input_unregister_device(nbx_jack->hook_switch);
		nbx_jack->hook_switch = NULL;
	}
	if ( nbx_jack->plug_det_irq >= 0 )
		free_irq(nbx_jack->plug_det_irq, nbx_jack_gpio_interrupt);
	if ( nbx_jack->codec_hp_det_irq >=0 )
		free_irq(nbx_jack->codec_hp_det_irq, nbx_jack_gpio_interrupt);
	if ( nbx_jack->codec_short_irq >= 0 )
		free_irq(nbx_jack->codec_short_irq, nbx_jack_gpio_interrupt);
	kfree( nbx_jack );
	nbx_jack = NULL;
}

int
nbx_jack_resume(void)
{
	if ( !nbx_jack ) {
		printk(KERN_ERR "nbx_jack: nbx_jack_resume: nbx_jack == NULL\n");
		return 0;
	}
	nbx_jack->ctr = 0;
	_start_timer();
	if ( nbx_jack->plug_det_irq >= 0 )
		enable_irq(nbx_jack->plug_det_irq);
	if ( nbx_jack->codec_hp_det_irq >=0 )
		enable_irq(nbx_jack->codec_hp_det_irq);
	if ( nbx_jack->codec_short_irq >= 0 )
		enable_irq(nbx_jack->codec_short_irq);
	return 0;
}

int
nbx_jack_suspend(void)
{
	if ( !nbx_jack ) {
		printk(KERN_ERR "nbx_jack: nbx_jack_suspend: nbx_jack == NULL\n");
		return 0;
	}
	if ( nbx_jack->plug_det_irq >= 0 )
		disable_irq(nbx_jack->plug_det_irq);
	if ( nbx_jack->codec_hp_det_irq >=0 )
		disable_irq(nbx_jack->codec_hp_det_irq);
	if ( nbx_jack->codec_short_irq >= 0 )
		disable_irq(nbx_jack->codec_short_irq);
	return 0;
}

MODULE_AUTHOR("Sony Corporation");
MODULE_DESCRIPTION("NBX Jack driver");
MODULE_LICENSE("GPL");
