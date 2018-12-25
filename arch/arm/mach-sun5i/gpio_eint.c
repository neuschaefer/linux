/*
 * gpio_eint.c
 */

#include <linux/string.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/slab.h>

#include <mach/sys_config.h>
#include <mach/platform.h>
#include <mach/gpio.h>
//hxm del for debug msg
/* debug log config */
//#define EINT_ERR_MSG
//#define EINT_INF_MSG
//#define EINT_DBG_MSG

//#define EINT_DEBUG_REG

#ifdef EINT_ERR_MSG
#define eint_err(fmt, ...) \
    do { printk("[eint err] " fmt, ##__VA_ARGS__); } while (0)
#else
#define eint_err(fmt, ...) \
    do { } while (0)
#endif /* EINT_ERR_MSG */

#ifdef EINT_INF_MSG
#define eint_inf(fmt, ...) \
    do { printk("[eint inf] " fmt, ##__VA_ARGS__); } while (0)
#else
#define eint_inf(fmt, ...) \
    do { } while (0)
#endif /* EINT_INF_MSG */

#ifdef EINT_DBG_MSG
#define eint_dbg(fmt, ...) \
    do { printk("[eint dbg] " fmt, ##__VA_ARGS__); } while (0)
#else
#define eint_dbg(fmt, ...) \
    do { } while (0)
#endif /* EINT_DBG_MSG */

#ifdef EINT_DEBUG_REG
#define eint_pr_reg(addr) \
    do { printk("[eint reg] [%s] 0x%08x: %x\n", __func__, addr, readl(addr)); } while (0)
#else
#define eint_pr_reg(fmt, ...) \
    do { } while (0)
#endif

struct eint_handle
{
    u32 gpio_handle;
    user_gpio_set_t gpio_status;
    int no;
    u32 conf_reg_addr;
    u32 conf_reg_offs;
};

#define EINT_CONTROL_REG_ADDR           (SW_VA_PORTC_IO_BASE + 0x210)
#define EINT_STATUS_REG_ADDR            (SW_VA_PORTC_IO_BASE + 0x214)
#define EINT_DEBOUNCE_REG_ADDR          (SW_VA_PORTC_IO_BASE + 0x218)

#define EINT_HANDLE_CHECK(hdle, retval) \
    do { \
        if (hdle == NULL) { \
            eint_err("[%s] invalid handle\n", __func__); \
            return retval; \
        } \
    } while (0)
#define EINT_NULL_CHECK(p, retval) \
    do { \
        if (p == NULL) { \
            eint_err("[%s] null pointer\n", __func__); \
            return retval; \
        } \
    } while (0)

static spinlock_t eint_lock;

static int __eint_no(user_gpio_set_t *gpio_status)
{
    switch (gpio_status->port) {
        case 1:
            if (gpio_status->port_num == 17)
                return 31;
            break;

        case 2:
            if (gpio_status->port_num >= 2 && 
                gpio_status->port_num <= 14)
                return gpio_status->port_num + 14;
            else if (gpio_status->port_num == 19)
                return 29;
            else if (gpio_status->port_num == 20)
                return 30;
            break;

        case 5:
            if (gpio_status->port_num == 0)
                return 14;
            else if (gpio_status->port_num == 1)
                return 15;
            break;

        case 7:
            if (gpio_status->port_num >= 0 &&
                gpio_status->port_num <= 13)
                return gpio_status->port_num;
            break;

        default:
            return -1;
    }

    return -1;
}

/*
 * enable or disable eint.
 *
 * \param handle eint handle requested by gpio_request_eint.
 * \param 0 - disable eint, 1 - enable eint.
 * \return 0 indicates enable success, otherwise fail.
 */
int sw_gpio_eint_set_enable(u32 handle, u32 enable)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(EINT_CONTROL_REG_ADDR);
    value = readl(EINT_CONTROL_REG_ADDR);
    value &= ~(1 << hdle->no);
    if (enable)
        value |= 1 << hdle->no;
    writel(value, EINT_CONTROL_REG_ADDR);
    eint_pr_reg(EINT_CONTROL_REG_ADDR);
    spin_unlock_irqrestore(&eint_lock, flags);

    return 0;
}
EXPORT_SYMBOL(sw_gpio_eint_set_enable);

/*
 * get eint enable/disable status.
 *
 * \param handle eint handle requested by gpio_request_eint.
 * \param enable enable/disable status, this buffer allocated by caller.
 * \return 0 indicates success, otherwise fail.
 */
int sw_gpio_eint_get_enable(u32 handle, u32 *enable)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);
    EINT_NULL_CHECK(enable, -1);

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(EINT_CONTROL_REG_ADDR);
    value = readl(EINT_CONTROL_REG_ADDR);
    *enable = (value >> hdle->no) & 0x01;
    spin_unlock_irqrestore(&eint_lock, flags);

    return 0;
}
EXPORT_SYMBOL(sw_gpio_eint_get_enable);

/*
 * get eint trig type.
 *
 * \param handle eint handle requested by gpio_request_eint.
 * \param trig_type the trig type of eint, this buffer allocated by caller.
 * \return 0 indicates success, otherwise fail.
 */
int sw_gpio_eint_get_trigtype(u32 handle, enum gpio_eint_trigtype *trig_type)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);
    EINT_NULL_CHECK(trig_type, -1);

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(hdle->conf_reg_addr);
    value = readl(hdle->conf_reg_addr);
    *trig_type = (value >> hdle->conf_reg_offs) & 0x0f;
    spin_unlock_irqrestore(&eint_lock, flags);

    return 0;
}
EXPORT_SYMBOL(sw_gpio_eint_get_trigtype);

/*
 * set eint trig type.
 *
 * \param handle eint handle requested by gpio_request_eint.
 * \param trig_type the trig type of eint.
 * \return 0 indicates success, otherwise fail.
 */
int sw_gpio_eint_set_trigtype(u32 handle, enum gpio_eint_trigtype trig_type)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);
    if (trig_type < 0 || trig_type >= TRIG_INALID) {
        eint_err("[%s] invalid trig type\n", __func__);
        return -1;
    }

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(hdle->conf_reg_addr);
    value = readl(hdle->conf_reg_addr);
    value &= ~(0x0f << hdle->conf_reg_offs);
    value |= trig_type << hdle->conf_reg_offs;
    writel(value, hdle->conf_reg_addr);
    eint_pr_reg(hdle->conf_reg_addr);
    spin_unlock_irqrestore(&eint_lock, flags);

    return 0;
}
EXPORT_SYMBOL(sw_gpio_eint_set_trigtype);

/*
 * get eint pending status.
 *
 * \param handle eint handle requested by gpio_request_eint.
 * \return the status of eint, -1 indicates fail.
 */
int sw_gpio_eint_get_irqpd_sta(u32 handle)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(EINT_STATUS_REG_ADDR);
    value = readl(EINT_STATUS_REG_ADDR);
    value = (value >> hdle->no) & 0x01;
    spin_unlock_irqrestore(&eint_lock, flags);

    return value;
}
EXPORT_SYMBOL(sw_gpio_eint_get_irqpd_sta);

/*
 * clear eint pending status.
 *
 * \param handle eint handle request by gpio_request_eint.
 * \return 0 indicates sucess, otherwise fail.
 */
int sw_gpio_eint_clr_irqpd_sta(u32 handle)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(EINT_STATUS_REG_ADDR);
    value = readl(EINT_STATUS_REG_ADDR);
    value |= 1 << hdle->no;
    writel(value, EINT_STATUS_REG_ADDR);
    eint_pr_reg(EINT_STATUS_REG_ADDR);
    spin_unlock_irqrestore(&eint_lock, flags);

    return 0;
}
EXPORT_SYMBOL(sw_gpio_eint_clr_irqpd_sta);

/*
 * get eint debounce.
 *
 * \param handle eint handle request by gpio_request_eint.
 * \param debounce the debounce of eint, the memory of this pointer allocated by caller.
 * \return 0 indicates success, otherwise fail.
 */
int sw_gpio_eint_get_debounce(u32 handle, struct gpio_eint_debounce *debounce)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);
    EINT_NULL_CHECK(debounce, -1);

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(EINT_DEBOUNCE_REG_ADDR);
    value = readl(EINT_DEBOUNCE_REG_ADDR);
    debounce->clk_sel = value & 0x01;
    debounce->clk_pre_scl = (value >> 4) & 0x07;
    spin_unlock_irqrestore(&eint_lock, flags);

    return 0;
}
EXPORT_SYMBOL(sw_gpio_eint_get_debounce);

/*
 * set eint debounce.
 *
 * \param handle eint handle request by gpio_request_eint.
 * \param debounce the debounce of eint.
 * \return 0 indicates success, otherwise fail.
 */
int sw_gpio_eint_set_debounce(u32 handle, struct gpio_eint_debounce *debounce)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);
    EINT_NULL_CHECK(debounce, -1);

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(EINT_DEBOUNCE_REG_ADDR);
    value = readl(EINT_DEBOUNCE_REG_ADDR);
    value &= ~(0x01 << 0);
    value |= debounce->clk_sel & 0x01;
    value &= ~(0x07 << 4);
    value |= debounce->clk_pre_scl & 0x07;
    writel(value, EINT_DEBOUNCE_REG_ADDR);
    eint_pr_reg(EINT_DEBOUNCE_REG_ADDR);
    spin_unlock_irqrestore(&eint_lock, flags);

    return 0;
}
EXPORT_SYMBOL(sw_gpio_eint_set_debounce);

/* 
 * request gpio for eint.
 *
 * \param main_name main key name in script.
 * \param sub_name sub key name in script.
 * \param trig_type the trig type of eint.
 * \return 0 indicates fail, otherwise success.
 */
u32 sw_gpio_irq_request(char *main_name, char *sub_name, enum gpio_eint_trigtype trig_type)
{
    struct eint_handle *hdle;
    u32 value;
    unsigned long flags = 0;

    EINT_NULL_CHECK(main_name, 0);
    EINT_NULL_CHECK(sub_name, 0);
    if (trig_type < 0 || trig_type >= TRIG_INALID) {
        eint_err("[%s] invalid trig type\n", __func__);
        return -1;
    }
    
    eint_inf("[%s] main name: %s, sub name: %s, trig type: %d\n", __func__, main_name, sub_name, trig_type);
    hdle = kmalloc(sizeof(struct eint_handle), GFP_KERNEL);
    if (hdle == NULL) {
        eint_err("kmalloc for handle failed\n");
        goto out;
    }
    memset(hdle, 0, sizeof(struct eint_handle));

    hdle->gpio_handle = gpio_request_ex(main_name, sub_name);
    if (!hdle->gpio_handle) {
        eint_err("reqest gpio failed\n");
        goto kmalloc_out;
    }

    gpio_get_one_pin_status(hdle->gpio_handle, &hdle->gpio_status, sub_name, 1);
    // check if config as eint
    if (hdle->gpio_status.mul_sel != 6) {
        eint_err("gpio not config as eint\n");
        goto gpio_out;
    }
    hdle->no = __eint_no(&hdle->gpio_status);
    eint_inf("eint port:P%c%02d<%d>, no:%d\n", '@' + hdle->gpio_status.port, 
            hdle->gpio_status.port_num, hdle->gpio_status.mul_sel, hdle->no);
    if (hdle->no < 0) {
        eint_err("gpio cann't config as eint\n");
        goto gpio_out;
    }

    hdle->conf_reg_addr = SW_VA_PORTC_IO_BASE + 0x200 + ((hdle->no >> 3) << 2);
    hdle->conf_reg_offs = (hdle->no % 8) << 2;
    eint_inf("config register address: %x\n", hdle->conf_reg_addr);
    eint_inf("config register offset : %x\n", hdle->conf_reg_offs);

    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(hdle->conf_reg_addr);
    eint_pr_reg(EINT_CONTROL_REG_ADDR);
    eint_pr_reg(EINT_STATUS_REG_ADDR);
    
    // disable it firstly
    value = readl(EINT_CONTROL_REG_ADDR);
    value &= ~(1 << hdle->no);
    writel(value, EINT_CONTROL_REG_ADDR);

    // clear pending
    value = readl(EINT_STATUS_REG_ADDR);
    value |= 1 << hdle->no;
    writel(value, EINT_STATUS_REG_ADDR);

    sw_gpio_set_trigger((u32)hdle, trig_type);

    eint_pr_reg(hdle->conf_reg_addr);
    eint_pr_reg(EINT_CONTROL_REG_ADDR);
    eint_pr_reg(EINT_STATUS_REG_ADDR);
    spin_unlock_irqrestore(&eint_lock, flags);

    return (u32)hdle;

gpio_out:
    gpio_release(hdle->gpio_handle, 1);
kmalloc_out:
    kfree(hdle);
out:
    return 0;
}
EXPORT_SYMBOL(sw_gpio_irq_request);

void sw_gpio_set_trigger(u32 phandle, enum gpio_eint_trigtype trig_type)
{
	struct eint_handle *handle = (struct eint_handle *)phandle;
	u32 value;

	printk("%s(): Enter %d\n", __func__, trig_type);

	value = readl(handle->conf_reg_addr);
	value &= ~(0x0f << handle->conf_reg_offs);
	value |= trig_type << handle->conf_reg_offs;
	writel(value, handle->conf_reg_addr);
}
EXPORT_SYMBOL(sw_gpio_set_trigger);

/* 
 * release eint.
 *
 * \param handle eint handle requested by gpio_request_eint.
 * \return 0 indicates enable success, otherwise fail.
 */
int sw_gpio_irq_free(u32 handle)
{
    struct eint_handle *hdle = (struct eint_handle *)handle;
    u32 value;
    unsigned long flags = 0;

    EINT_HANDLE_CHECK(hdle, -1);
    
    spin_lock_irqsave(&eint_lock, flags);
    eint_pr_reg(hdle->conf_reg_addr);
    eint_pr_reg(EINT_CONTROL_REG_ADDR);
    eint_pr_reg(EINT_STATUS_REG_ADDR);
    
    // disable it firstly
    value = readl(EINT_CONTROL_REG_ADDR);
    value &= ~(1 << hdle->no);
    writel(value, EINT_CONTROL_REG_ADDR);

    // clear pending
    value = readl(EINT_STATUS_REG_ADDR);
    value |= 1 << hdle->no;
    writel(value, EINT_STATUS_REG_ADDR);
    
    // config
    value = readl(hdle->conf_reg_addr);
    value &= ~(0x0f << hdle->conf_reg_offs);
    writel(value, hdle->conf_reg_addr);
    
    eint_pr_reg(hdle->conf_reg_addr);
    eint_pr_reg(EINT_CONTROL_REG_ADDR);
    eint_pr_reg(EINT_STATUS_REG_ADDR);
    spin_unlock_irqrestore(&eint_lock, flags);
    
    gpio_release(hdle->gpio_handle, 1);
    kfree(hdle);

    return 0;
}
EXPORT_SYMBOL(sw_gpio_irq_free);

int sw_gpio_irq_init(void)
{
    spin_lock_init(&eint_lock);
    return 0;
}

int sw_gpio_irq_exit(void)
{
    // ...
    return 0;
}

