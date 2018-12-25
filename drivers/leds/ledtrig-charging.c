/*
 * LED Charging Trigger
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/power_supply.h>

#include <linux/mfd/axp-mfd.h>

static const uint64_t AXP20_NOTIFIER_ON = 	AXP20_IRQ_USBIN | AXP20_IRQ_USBRE |	AXP20_IRQ_ACIN |
				       						AXP20_IRQ_ACRE | AXP20_IRQ_CHAST | AXP20_IRQ_CHAOV ;

static int state_full = false;

static int ledtrig_charging_callback(struct notifier_block *nfb, unsigned long action, void *ignored);
static void charge_led_activate(struct led_classdev *led);

static struct led_trigger ledtrig_charging = {
	.name		= "charging",
	.activate	= charge_led_activate,
};

#ifdef CONFIG_LEDS_TRIGGER_CHARGING_USE_TIMER
static struct power_supply *ps = NULL;
static struct timer_list lt;

static void lt_check_cap (unsigned long data) {
	union power_supply_propval value;

	if (ps == NULL) return;

	ps->get_property(ps, POWER_SUPPLY_PROP_CAPACITY, &value);

	if (value.intval >= 100) {
		led_trigger_event(&ledtrig_charging, LED_OFF);
		//printk("[%s] LED OFF by timer\n",__func__);
	} else {
		mod_timer(&lt,jiffies + HZ * 2);
	}
}

static void charge_led_activate(struct led_classdev *led) {

	led_trigger_event(&ledtrig_charging, LED_FULL);

	if (ps != NULL) {
		union power_supply_propval value = { 0 };
		ps->get_property(ps, POWER_SUPPLY_PROP_ONLINE, &value);
		if (value.intval == 0 && !timer_pending(&lt)) {
			setup_timer(&lt,lt_check_cap,0);
			mod_timer(&lt,jiffies + HZ * 10);
			//printk("[%s] Start timer\n",__func__);
		}
	}
	//printk("[%s] LED activation\n",__func__);
}
#else
static void charge_led_activate(struct led_classdev *led) {}
#endif

static struct notifier_block ledtrig_charging_notifier = {
	.notifier_call = ledtrig_charging_callback,
	//.priority = 0,
};

static int ledtrig_charging_callback(struct notifier_block *nfb, unsigned long action, void *ignored) {
	//printk("[%s] event=%lx; %lu\n",__func__,action,action);

	if ((action & AXP20_IRQ_USBRE) || (action & AXP20_IRQ_ACRE)) {
		//printk("[%s] state_full = false\n",__func__);
		state_full = false;
#ifdef CONFIG_LEDS_TRIGGER_CHARGING_USE_TIMER
		if (timer_pending(&lt))
			del_timer(&lt);
#endif
	}

	if ((action & AXP20_IRQ_CHAST) && !state_full) {
		//printk("[%s] LED ON\n",__func__);
		led_trigger_event(&ledtrig_charging, LED_FULL);
	}

	if (action & AXP20_IRQ_CHAOV) {
		//printk("[%s] LED OFF\n",__func__);
		state_full = true;

#ifdef CONFIG_LEDS_TRIGGER_CHARGING_USE_TIMER
		if (!timer_pending(&lt)) {
			setup_timer(&lt,lt_check_cap,0);
			mod_timer(&lt,jiffies + HZ * 2);
			//printk("[%s] Start timer\n",__func__);
		}
#else
		led_trigger_event(&ledtrig_charging, LED_OFF);
#endif
	}

	return NOTIFY_DONE;
}

static int __init ledtrig_charging_init(void)
{
	printk("[%s] == \n",__func__);
	led_trigger_register(&ledtrig_charging);

#ifdef CONFIG_LEDS_TRIGGER_CHARGING_USE_TIMER
	ps = power_supply_get_by_name("battery");
#endif

	struct device *axp_dev = axp_get_dev();
	axp_register_notifier(axp_dev,&ledtrig_charging_notifier,AXP20_NOTIFIER_ON);
	return 0;
}

static void __exit ledtrig_charging_exit(void)
{
	printk("[%s] == \n",__func__);
#ifdef CONFIG_LEDS_TRIGGER_CHARGING_USE_TIMER
	if (timer_pending(&lt))
		del_timer(&lt);
#endif
	led_trigger_unregister(&ledtrig_charging);
	struct device *axp_dev = axp_get_dev();
	axp_unregister_notifier(axp_dev,&ledtrig_charging_notifier,AXP20_NOTIFIER_ON);
}

module_init(ledtrig_charging_init);
module_exit(ledtrig_charging_exit);

MODULE_AUTHOR("Dushes");
MODULE_DESCRIPTION("LED Charging Trigger");
MODULE_LICENSE("GPL");
