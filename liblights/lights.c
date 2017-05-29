/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2016 Jonathan Jason Dennis [Meticulus]
 *				theonejohnnyd@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "Meticulus Lights"

#include <cutils/log.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <cutils/properties.h>
#include <hardware/lights.h>
#include <hardware/hardware.h>

#define DEBUG

#define STOCK_PROP "persist.sys.stock_lights_HAL"
#define LOWPOWER_PROP "persist.sys.lights_HAL_lp"
#define CHARGING_PROP "persist.sys.lights_HAL_c"
#define FULLPOWER_PROP "persist.sys.lights_HAL_fp"
#define NOTIFICATION_PROP "persist.sys.lights_HAL_n"
#define LOWPOWER ""
#define CHARGING ""
#define FULLPOWER ""
#define NOTIFICATION ""

extern int load_stock_lights(char *path, hw_module_t **pHmi);

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

/** Paths to light files **/
char const *const RED_BRIGHTNESS_FILE = "/sys/class/leds/red/brightness";
char const *const RED_DELAYON_FILE = "/sys/class/leds/red/delay_on";
char const *const RED_DELAYOFF_FILE = "/sys/class/leds/red/delay_off";

char const *const GREEN_BRIGHTNESS_FILE = "/sys/class/leds/green/brightness";
char const *const GREEN_DELAYON_FILE = "/sys/class/leds/green/delay_on";
char const *const GREEN_DELAYOFF_FILE = "/sys/class/leds/green/delay_off";

char const *const BLUE_BRIGHTNESS_FILE = "/sys/class/leds/blue/brightness";
char const *const BLUE_DELAYON_FILE = "/sys/class/leds/blue/delay_on";
char const *const BLUE_DELAYOFF_FILE = "/sys/class/leds/blue/delay_off";

char const *const BACKLIGHT_FILE = "/sys/class/leds/lcd_backlight0/brightness";

static int last_battery_color = 0xff000000;
static int last_noti_color = 0xff000000;

/** Write integer to file **/
static int write_int(char const *path, int value)
{
	int fd;
	static int already_warned = -1;
	fd = open(path, O_RDWR);
	if (fd >= 0) {
		char buffer[20];
		int bytes = sprintf(buffer, "%d\n", value);
		int amt = write(fd, buffer, bytes);
		close(fd);
		return amt == -1 ? -errno : 0;
	} else {
		if (already_warned == -1) {
			ALOGE("write_int failed to open %s\n", path);
			already_warned = 1;
		}
		return -errno;
	}
}

static int rgb_to_brightness(struct light_state_t const * state) {

	unsigned int brightness = ((77 * ((state->color >> 16) & 0x00ff))
	                           + (150 * ((state->color >> 8) & 0x00ff)) + (29 * (state->color & 0x00ff))) >> 8;
	return brightness;

}

/** Set LCD backlight **/
static int set_light_backlight(struct light_device_t *dev, struct light_state_t const *state)
{
	pthread_mutex_lock(&g_lock);
	int err = write_int(BACKLIGHT_FILE, rgb_to_brightness(state));
	pthread_mutex_unlock(&g_lock);
	return err;
}

static void stop_blink(void) {
	write_int(RED_DELAYON_FILE, 0);
	write_int(RED_DELAYOFF_FILE, 0);
	write_int(GREEN_DELAYON_FILE, 0);
	write_int(GREEN_DELAYOFF_FILE, 0);
	write_int(BLUE_DELAYON_FILE, 0);
	write_int(BLUE_DELAYOFF_FILE, 0);
}

static void int_to_argb(int color, int argb[]) {
	char scolor[255];
	char alpha[3], red[3], green[3], blue[3];
	sprintf(scolor, "%x", color);
#ifdef DEBUG
	ALOGD("color=%s", scolor);
#endif
	strncpy(alpha, scolor, 2);
	strncpy(red, scolor + 2, 2);
	strncpy(green, scolor + 4, 2);
	strncpy(blue, scolor + 6, 2);
	alpha[2] = red[2] = green[2] = blue[2] = '\0';
#ifdef DEBUG
	ALOGD("alpha=%s red=%s green=%s blue=%s", alpha, red, green, blue);
#endif
	argb[0] = (int)strtol(alpha, NULL, 16);
	argb[1] = (int)strtol(red, NULL, 16);
	argb[2] = (int)strtol(green, NULL, 16);
	argb[3] = (int)strtol(blue, NULL, 16);

}

static void set_light_color(int color) {

	int argb[4];
	stop_blink();

	int_to_argb(color, argb);

	write_int(RED_BRIGHTNESS_FILE, argb[1]);
	write_int(GREEN_BRIGHTNESS_FILE, argb[2]);
	write_int(BLUE_BRIGHTNESS_FILE, argb[3]);

}

static void set_blink(struct light_state_t const* state) {
	char scolor[255];
	char alpha[3], red[3], green[3], blue[3];
	int ialpha, ired, igreen, iblue;
	sprintf(scolor, "%x", state->color);
#ifdef DEBUG
	ALOGD("color=%s", scolor);
#endif
	strncpy(alpha, scolor, 2);
	strncpy(red, scolor + 2, 2);
	strncpy(green, scolor + 4, 2);
	strncpy(blue, scolor + 6, 2);
	alpha[2] = red[2] = green[2] = blue[2] = '\0';
#ifdef DEBUG
	ALOGD("alpha=%s red=%s green=%s blue=%s", alpha, red, green, blue);
#endif
	ired = strtol(red, NULL, 16);
	igreen = strtol(green, NULL, 16);
	iblue = strtol(blue, NULL, 16);

#ifdef DEBUG
	ALOGD("flashMode=%d flashOnMS=%d flashOffMS=%d brightnessMode=%d",
	      state->flashMode,
	      state->flashOnMS,
	      state->flashOffMS,
	      state->brightnessMode);
#ifdef LEDSMODE
	ALOGD("ledsModes=%d", state->ledsModes);
#endif
#endif
	stop_blink();
	if (!state->flashMode) return;

	if (ired) {
		write_int(RED_DELAYON_FILE, state->flashOnMS);
		write_int(RED_DELAYOFF_FILE, state->flashOffMS);
	}
	if (igreen) {
		write_int(GREEN_DELAYON_FILE, state->flashOnMS);
		write_int(GREEN_DELAYOFF_FILE, state->flashOffMS);
	}
	if (iblue) {
		write_int(BLUE_DELAYON_FILE, state->flashOnMS);
		write_int(BLUE_DELAYOFF_FILE, state->flashOffMS);
	}
}



static int set_light_notifications(struct light_device_t* dev, struct light_state_t const* state)
{
	int err = 0;
	pthread_mutex_lock(&g_lock);
	last_noti_color = state->color;
	/* reset before changing */
	set_light_color(0xff000000);
	if (state->color == 0xff000000 || !state->color)
		set_light_color(last_battery_color);
	else
		set_light_color(property_get_int32(NOTIFICATION, state->color));
	set_blink(state);
	pthread_mutex_unlock(&g_lock);
	return err;
}

static int set_light_battery(struct light_device_t* dev, struct light_state_t const* state)
{
	int err = 0;
	int argb[4];
	int color = state->color;
	pthread_mutex_lock(&g_lock);
	last_battery_color = state->color;
	if (last_noti_color == 0xff000000 || !last_noti_color) {
		int_to_argb(color, argb);
		/* Yellow charging between 25% and 90% */
		if (argb[1] == 0xff && argb[2] == 0xff && argb[3] == 0x00)
			color = property_get_int64(CHARGING_PROP, color);
		/* Red below 25 % */
		else if (argb[1] == 0xff && argb[2] == 0x00 && argb[3] == 0x00)
			color = property_get_int64(LOWPOWER_PROP, color);
		/* Green 90% or above */
		else if (argb[1] == 0x00 && argb[2] == 0xff && argb[3] == 0x00)
			color = property_get_int64(FULLPOWER_PROP, color);
		set_light_color(color);
		set_blink(state);
	}
	pthread_mutex_unlock(&g_lock);
	return err;
}

/** Close the lights device */
static int close_lights(struct light_device_t *dev)
{
	if (dev)
		free(dev);
	return 0;
}

/** Open a new instance of a lights device using name */
static int open_lights(const struct hw_module_t *module, char const *name, struct hw_device_t **device)
{

	if (property_get_bool(STOCK_PROP, 0)) {
		ALOGI("%s is set. Loading Stock lights HAL", STOCK_PROP);
		if (!load_stock_lights("/system/lib64/hw/lights.default.so", &module)) {
			return module->methods->open(module, name, device);
		} else {
			ALOGE("%s is set but could not load Stock lights HAL!", STOCK_PROP);
			property_set(STOCK_PROP, "false");
		}
	}

	pthread_t lighting_poll_thread;

	int (*set_light) (struct light_device_t *dev,
	                  struct light_state_t const * state);

	if (0 == strcmp(LIGHT_ID_BACKLIGHT, name))
		set_light = set_light_backlight;
	else if (0 == strcmp(LIGHT_ID_NOTIFICATIONS, name))
		set_light = set_light_notifications;
	else if (0 == strcmp(LIGHT_ID_BATTERY, name))
		set_light = set_light_battery;
	else
		return -EINVAL;

	pthread_mutex_init(&g_lock, NULL);

	struct light_device_t *dev = malloc(sizeof(struct light_device_t));
	memset(dev, 0, sizeof(*dev));

	dev->common.tag = HARDWARE_DEVICE_TAG;
	dev->common.version = 0;
	dev->common.module = (struct hw_module_t *)module;
	dev->common.close = (int (*)(struct hw_device_t *))close_lights;
	dev->set_light = set_light;

	*device = (struct hw_device_t *)dev;

	return 0;
}

/** Method list **/
static struct hw_module_methods_t lights_methods =
{
	.open =  open_lights,
};

/** The backlight module **/
struct hw_module_t HAL_MODULE_INFO_SYM =
{
	.tag = HARDWARE_MODULE_TAG,
	.version_major = 1,
	.version_minor = 0,
	.id = LIGHTS_HARDWARE_MODULE_ID,
	.name = "Meticulus Lights",
	.author = "Meticulus Development",
	.methods = &lights_methods,
};
