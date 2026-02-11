/*******************************************************************************
* Copyright 2012 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*******************************************************************************/

/*
 * Driver for controlling displays attached to VideoCore.
 *
 * This is a high level driver for controlling displays. It drives the display
 * output manager within VideoCore via a VCHIQ service.
 */

/* Include Files */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/lcd.h>
#include <linux/backlight.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif /* CONFIG_HAS_EARLYSUSPEND */

#ifdef CONFIG_ANDROID_POWER
#include <linux/android_power.h>
#endif

#include "interface/vchiq_arm/vchiq_if.h"
#include "interface/vchiq_arm/vchiq_connected.h"

#define DSPC_MAGIC_HEADER  (VCHIQ_MAKE_FOURCC('d', 's', 'p', 'c'))
#define DSPC_MAGIC_FOOTER  (~(DSPC_MAGIC_HEADER))

#define DSPC_REQUEST_PARAMS_COUNT  3

#define DSPC_POWER_LEVEL_UNKNOWN  0xffffffff
#define DSPC_POWER_LEVEL_OFF      0
#define DSPC_POWER_LEVEL_BLANK    1
#define DSPC_POWER_LEVEL_FULL     4

#define VC_DISPLAY_VER	    1
#define VC_DISPLAY_VER_MIN  0

typedef enum DSPC_COMMAND_tag {
	DSPC_COMMAND_INVALID,

	DSPC_COMMAND_SET_POWER,
	DSPC_COMMAND_GET_POWER,
	DSPC_COMMAND_SET_BRIGHTNESS,
	DSPC_COMMAND_GET_BRIGHTNESS,

	DSPC_COMMAND_MAX
} DSPC_COMMAND_T;

typedef struct DSPC_SERVER_REQ_tag {
	uint32_t magic_header;
	uint32_t response_tag;
	uint32_t command;	/* From DSPC_COMMAND_T */
	uint32_t display;
	uint32_t params[DSPC_REQUEST_PARAMS_COUNT];
	uint32_t magic_footer;
} DSPC_SERVER_REQ_T;

typedef struct DSPC_SERVER_RSP_tag {
	uint32_t result;
	uint32_t response_tag;
} DSPC_SERVER_RSP_T;

typedef struct DSPC_SERVER_MSG_RSP_tag {
	VCHIQ_HEADER_T header;
	DSPC_SERVER_RSP_T response;
} DSPC_SERVER_MSG_RSP_T;

struct vc_display_platform_data {
	void (*power_set) (void *context, int turn_on, int done);
	void (*brightness_set) (void *context, int brightness, int done);
	void *context;
};

struct vc_display_state {
	VCHIQ_INSTANCE_T vchiq_instance;
	VCHIQ_SERVICE_HANDLE_T service_handle;
	int connected;
	struct vc_display_platform_data *platform_data;
	uint32_t response_tag;
	uint32_t display;
	uint32_t current_power_level;
	uint32_t requested_power_level;
	struct lcd_device *ld;
	struct backlight_device *bd;
	struct platform_device *p_dev;
};

/* Private Constants and Types */
#undef ENABLE_LOG_DBG
#ifdef ENABLE_LOG_DBG
#define LOG_DBG(fmt, arg...)  printk(KERN_INFO "[D] " fmt "\n", ##arg)
#else
#define LOG_DBG(fmt, arg...)
#endif
#define LOG_INFO(fmt, arg...) printk(KERN_INFO "[I] " fmt "\n", ##arg)
#define LOG_ERR(fmt, arg...)  printk(KERN_ERR  "[E] " fmt "\n", ##arg)

/* Exported function prototypes. */
static int vc_display_probe(struct platform_device *p_dev);
static int __devexit vc_display_remove(struct platform_device *p_dev);

static int vc_display_set_power(struct lcd_device *ld, int power);
static int vc_display_get_power(struct lcd_device *ld);

static int vc_display_bl_get_brightness(struct backlight_device *bd);
static int vc_display_bl_update_status(struct backlight_device *bd);

#ifdef CONFIG_HAS_EARLYSUSPEND
static void vc_display_early_suspend(struct early_suspend *s);
static void vc_display_late_resume(struct early_suspend *s);
#endif /* CONFIG_HAS_EARLYSUSPEND */

/* Structures exporting functions. */
static struct platform_driver vc_display_driver = {
	.probe = vc_display_probe,
	.remove = __devexit_p(vc_display_remove),
	.driver = {
		   .name = "vc-display"}
};

static struct lcd_ops vc_display_lcd_ops = {
	.set_power = vc_display_set_power,
	.get_power = vc_display_get_power,
};

static const struct backlight_ops vc_display_bl_ops = {
	.get_brightness = vc_display_bl_get_brightness,
	.update_status = vc_display_bl_update_status,
};

static struct vc_display_state *vcd_state_instance;

#ifdef CONFIG_HAS_EARLYSUSPEND
/* We want early suspend handler to be called after the
 * EARLY_SUSPEND_LEVEL_BLANK_SCREEN handler is called,
 * so we increase priority by 5 */
static struct early_suspend vc_display_early_suspend_desc = {
	.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 5,
	.suspend = vc_display_early_suspend,
	.resume = vc_display_late_resume,
};
#endif /* CONFIG_HAS_EARLYSUSPEND */

/*
 * Local internal functions.
 */

static VCHIQ_STATUS_T vc_display_send_message(struct vc_display_state *state,
					      uint32_t command,
					      uint32_t *params,
					      size_t params_count,
					      int use_until_response)
{
	LOG_DBG("Broadcom vc-display entered %s", __func__);

	if (state && state->connected) {
		DSPC_SERVER_REQ_T request;
		VCHIQ_ELEMENT_T elems[] = { {&request, sizeof request}
		};
		size_t i;

		request.display = state->display;
		request.command = command;
		if (use_until_response)
			request.response_tag = ++(state->response_tag);
		else {
			/* No response wanted. */
			request.response_tag = 0;
		}

		for (i = 0; i < params_count && i < DSPC_REQUEST_PARAMS_COUNT;
		     i++)
			request.params[i] = params[i];

		request.magic_header = DSPC_MAGIC_HEADER;
		request.magic_footer = DSPC_MAGIC_FOOTER;

		vchiq_use_service(state->service_handle);
		vchiq_queue_message(state->service_handle, elems, 1);
		if (!use_until_response) {
			vchiq_release_service(state->service_handle);
		} else {
			LOG_DBG
			    ("Broadcom vc-display: Holding until response %d.",
			     request.response_tag);
		}

		return 0;
	} else {
		return -1;
	}
}

static VCHIQ_STATUS_T vc_display_vchiq_callback(VCHIQ_REASON_T reason,
						VCHIQ_HEADER_T *header,
						VCHIQ_SERVICE_HANDLE_T
						service_user, void *bulk_user)
{
	LOG_DBG("Broadcom vc-display entered %s", __func__);

	if (reason == VCHIQ_MESSAGE_AVAILABLE) {
		if (header->size == sizeof(DSPC_SERVER_RSP_T)) {
			DSPC_SERVER_MSG_RSP_T *wrapped_message =
			    (DSPC_SERVER_MSG_RSP_T *) header;
			DSPC_SERVER_RSP_T *message = &wrapped_message->response;

			if (message->result != 0) {
				LOG_ERR
					("VideoCore Display: "\
					 "Error response received %d",
				    message->result);
			}

			if (message->response_tag) {
				LOG_DBG
				    ("VideoCore Display: "\
					  "Response %d received, releasing.",
				     message->response_tag);
				vchiq_release_service(service_user);
			} else {
				LOG_ERR
				    ("VideoCore Display: "\
					  "Unexpected response received");
			}
		} else {
			LOG_ERR
			    ("VideoCore Display: "\
				  "Unexpected message size %d, expected %d",
			     header->size, sizeof(DSPC_SERVER_RSP_T));
		}

		vchiq_release_message(service_user, header);
	}

	return VCHIQ_SUCCESS;
}

static void vc_display_power(struct vc_display_state *state,
			     uint32_t power_level)
{
	int turn_on = power_level != DSPC_POWER_LEVEL_OFF;
	void (*power_set_fn) (void *context, int turn_on, int done);
	void *context = NULL;

	LOG_DBG("Broadcom vc-display entered %s", __func__);

	if (power_level == state->current_power_level) {
		LOG_DBG("Broadcom vc-display: power level already set.");
		return;
	}

	if (state->platform_data && state->platform_data->power_set) {
		power_set_fn = state->platform_data->power_set;
		context = state->platform_data->context;
	} else {
		power_set_fn = NULL;
	}

	if (power_set_fn)
		power_set_fn(context, turn_on, 0);

	vc_display_send_message(state, DSPC_COMMAND_SET_POWER, &power_level, 1,
				1);

	if (power_set_fn)
		power_set_fn(context, turn_on, 1);

	state->current_power_level = power_level;
}

/*
 * Exported functions.
 */

static int vc_display_set_power(struct lcd_device *ld, int power)
{
	uint32_t vc_power_level = DSPC_POWER_LEVEL_OFF;
	struct vc_display_state *state = dev_get_drvdata(&ld->dev);

	LOG_DBG("Broadcom vc-display entered %s", __func__);
	if (power != FB_BLANK_UNBLANK && power != FB_BLANK_POWERDOWN &&
	    power != FB_BLANK_NORMAL) {
		return -EINVAL;
	}

	switch (power) {
	case FB_BLANK_UNBLANK:
		vc_power_level = DSPC_POWER_LEVEL_FULL;
		break;

	case FB_BLANK_NORMAL:
		vc_power_level = DSPC_POWER_LEVEL_BLANK;
		break;

	case FB_BLANK_POWERDOWN:
		vc_power_level = DSPC_POWER_LEVEL_OFF;
		break;
	}

	state->requested_power_level = vc_power_level;
	vc_display_power(state, vc_power_level);
	return 0;
}

static int vc_display_get_power(struct lcd_device *ld)
{
	/* We should get this from the underlying driver. */
	LOG_DBG("Broadcom vc-display entered %s", __func__);
	return 0;
}

static int vc_display_bl_get_brightness(struct backlight_device *bd)
{
	/* We should get this from the underlying driver. */
	LOG_DBG("Broadcom vc-display entered %s", __func__);
	return bd->props.brightness;
}

static int vc_display_bl_update_status(struct backlight_device *bd)
{
	int ret = 0;
	int brightness = bd->props.brightness;
	void (*brightness_set_fn) (void *context, int brightness, int done);
	void *context = NULL;
	struct vc_display_state *state = dev_get_drvdata(&bd->dev);

	LOG_DBG("Broadcom vc-display entered %s", __func__);

	if (brightness < 0 || brightness > bd->props.max_brightness)
		return -EINVAL;

	if (state->platform_data && state->platform_data->brightness_set) {
		brightness_set_fn = state->platform_data->brightness_set;
		context = state->platform_data->context;
	} else {
		brightness_set_fn = NULL;
	}

	if (brightness_set_fn)
		brightness_set_fn(context, brightness, 0);

	vc_display_send_message(state, DSPC_COMMAND_SET_BRIGHTNESS, &brightness,
				1, 1);

	if (brightness_set_fn)
		brightness_set_fn(context, brightness, 1);

	return ret;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void vc_display_early_suspend(struct early_suspend *s)
{
	LOG_DBG("Broadcom vc-display entered %s", __func__);

	/* Enter off state if we're not already in it. */
	if (vcd_state_instance &&
	    (vcd_state_instance->current_power_level >
		   DSPC_POWER_LEVEL_OFF)) {
		vc_display_power(vcd_state_instance, DSPC_POWER_LEVEL_OFF);
	}
	return;
}

static void vc_display_late_resume(struct early_suspend *s)
{
	LOG_DBG("Broadcom vc-display entered %s", __func__);

	/* Restore power level to how it was before. */
	if (vcd_state_instance &&
	    (vcd_state_instance->current_power_level !=
	     vcd_state_instance->requested_power_level)) {
		vc_display_power(vcd_state_instance,
				 vcd_state_instance->requested_power_level);
	}
	return;
}
#endif /* CONFIG_HAS_EARLYSUSPEND */

/* Registration and removal. */
static int vc_display_do_connect(struct vc_display_state *state)
{
	int ret = -1;

	VCHIQ_SERVICE_PARAMS_T params = {
		.fourcc = VCHIQ_MAKE_FOURCC('d', 's', 'p', 'c'),
		.callback = vc_display_vchiq_callback,
		.version = VC_DISPLAY_VER,
		.version_min = VC_DISPLAY_VER };

	LOG_DBG("Broadcom vc-display entered %s", __func__);

	if (vchiq_initialise(&state->vchiq_instance) != VCHIQ_SUCCESS) {
		LOG_ERR("vchiq_initialise failed");
	} else if (vchiq_connect(state->vchiq_instance) != VCHIQ_SUCCESS) {
		LOG_ERR("vchiq_connect failed");
		vchiq_shutdown(state->vchiq_instance);
	} else if (vchiq_open_service(state->vchiq_instance,
				      &params,
				      &state->service_handle) !=
		   VCHIQ_SUCCESS) {
		/* This may happen if VideoCore is not
		*  using display abstraction. */
		LOG_INFO("vchiq_open_service failed");
		vchiq_shutdown(state->vchiq_instance);
	} else {
		LOG_DBG("Broadcom vc-display connected OK");
		vchiq_release_service(state->service_handle);
		state->connected = 1;
		state->current_power_level = DSPC_POWER_LEVEL_UNKNOWN;
		/* Assume we want full power by default until told otherwise. */
		state->requested_power_level = DSPC_POWER_LEVEL_FULL;
		ret = 0;
	}

	return ret;
}

static int vc_display_do_disconnect(struct vc_display_state *state)
{
	vchiq_close_service(state->service_handle);
	vchiq_shutdown(state->vchiq_instance);
	state->connected = 0;

	return 0;
}

static int vc_display_do_register(struct vc_display_state *state)
{
	int ret = -1;

	state->ld = lcd_device_register("vc-display-lcd",
					&state->p_dev->dev, state,
					&vc_display_lcd_ops);

	if (IS_ERR(state->ld)) {
		ret = PTR_ERR(state->ld);
		goto register_error;
	} else {
		dev_set_drvdata(&state->ld->dev, state);
	}

	state->bd = backlight_device_register("vc-display-bl",
					      &state->p_dev->dev, state,
					      &vc_display_bl_ops, NULL);

	if (IS_ERR(state->bd)) {
		ret = PTR_ERR(state->bd);
		goto register_error;
	} else {
		dev_set_drvdata(&state->bd->dev, state);
		state->bd->props.max_brightness = 255;
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	register_early_suspend(&vc_display_early_suspend_desc);
#endif /* CONFIG_HAS_EARLYSUSPEND */

	return 0;

register_error:
	LOG_ERR("Broadcom vc-display : registration failed %d %d %d",
		state ? 1 : 0,
		(state && state->ld) ? 1 : 0, (state && state->bd) ? 1 : 0);

	if (state) {
		if (state->ld)
			lcd_device_unregister(state->ld);

		if (state->bd)
			backlight_device_unregister(state->bd);

		kfree(state);
	}

	return ret;
}

static void vc_display_connected_init(void)
{
	LOG_DBG("Broadcom vc-display entered %s", __func__);

	if (vcd_state_instance) {
		if (vc_display_do_connect(vcd_state_instance) != 0)
			;
		else if (vc_display_do_register(vcd_state_instance) != 0)
			vc_display_do_disconnect(vcd_state_instance);
	}
}

static int vc_display_probe(struct platform_device *p_dev)
{
	struct vc_display_state *state;

	LOG_DBG("Broadcom vc-display entered %s", __func__);

	state = kzalloc(sizeof *state, GFP_KERNEL);
	if (!state) {
		LOG_ERR("Broadcom vc-display : alloc failed");
		return -ENOMEM;
	} else {
		dev_set_drvdata(&p_dev->dev, state);
		vcd_state_instance = state;
		state->p_dev = p_dev;

		state->display = 0;

		vchiq_add_connected_callback(vc_display_connected_init);

		return 0;
	}
}

static int __devexit vc_display_remove(struct platform_device *p_dev)
{
	struct vc_display_state *state = dev_get_drvdata(&p_dev->dev);

	if (state) {
		if (state->ld)
			lcd_device_unregister(state->ld);

		if (state->bd)
			backlight_device_unregister(state->bd);

		if (state->connected)
			vc_display_do_disconnect(state);

		kfree(state);
	}

	return 0;
}

static int __init vc_display_init(void)
{
	int ret;

	ret = platform_driver_register(&vc_display_driver);
	if (ret) {
		LOG_ERR("%s : Unable to register VideoCore display driver",
			__func__);
	} else {
		LOG_INFO("%s : Registered VideoCore display driver", __func__);
	}

	return ret;
}

static void __exit vc_display_exit(void)
{
	/* Clean up .. */
	platform_driver_unregister(&vc_display_driver);

	LOG_DBG("VideoCore display exit OK");
}

late_initcall(vc_display_init);
module_exit(vc_display_exit);

MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("VideoCore Display Driver");
