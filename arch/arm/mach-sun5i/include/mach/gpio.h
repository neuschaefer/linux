/*
 * mach/gpio.h
 */

#ifndef _GPIO_H_
#define _GPIO_H_

/* gpio eint trig type */
enum gpio_eint_trigtype {
    TRIG_EDGE_POSITIVE = 0,
    TRIG_EDGE_NEGATIVE,
    TRIG_LEVL_HIGH,
    TRIG_LEVL_LOW,
    TRIG_EDGE_DOUBLE,   /* positive/negative */
    TRIG_INALID
};

/* gpio eint debounce para */
struct gpio_eint_debounce {
    u32 clk_sel;      /* pio interrupt clock select, 0-LOSC, 1-HOSC */
    u32 clk_pre_scl;  /* debounce clk pre-scale n, the select,
                       * clock source is pre-scale by 2^n.
                       */
};

/* api for external int */
int sw_gpio_eint_set_trigtype(u32 handle, enum gpio_eint_trigtype trig_type);
int sw_gpio_eint_get_trigtype(u32 handle, enum gpio_eint_trigtype *trig_type);
int sw_gpio_eint_get_enable(u32 handle, u32 *enable);
int sw_gpio_eint_set_enable(u32 handle, u32 enable);
int sw_gpio_eint_get_irqpd_sta(u32 handle);
int sw_gpio_eint_clr_irqpd_sta(u32 handle);
int sw_gpio_eint_get_debounce(u32 handle, struct gpio_eint_debounce *debounce);
int sw_gpio_eint_set_debounce(u32 handle, struct gpio_eint_debounce *debounce);
u32 sw_gpio_irq_request(char *main_name, char *sub_name, enum gpio_eint_trigtype trig_type);
int sw_gpio_irq_free(u32 handle);
void sw_gpio_set_trigger(u32 handle, enum gpio_eint_trigtype trig_type);


#endif /* _GPIO_H_ */
