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

#define LOG_TAG "lights"

#include <cutils/log.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include <hardware/lights.h>
#include <hardware/hardware.h>

#define DEBUG

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

    unsigned int brightness = ((77*((state->color>>16)&0x00ff))
    		+ (150*((state->color>>8)&0x00ff)) + (29*(state->color&0x00ff))) >> 8;
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

static void set_state(struct light_state_t const * state) {

    char scolor[255];
    char alpha[3], red[3], green[3], blue[3];
    int ialpha,ired,igreen,iblue;
    sprintf(scolor, "%x",state->color);
#ifdef DEBUG
    ALOGD("color=%s",scolor);
#endif
    strncpy(alpha,scolor,2);
    strncpy(red, scolor + 2,2);
    strncpy(green, scolor + 4,2);
    strncpy(blue, scolor + 6,2);
    alpha[2] = red[2] = green[2] = blue[2] = '\0';
#ifdef DEBUG
    ALOGD("alpha=%s red=%s green=%s blue=%s",alpha,red,green,blue);
#endif
    ired = strtol(red,NULL,16);
    igreen = strtol(green,NULL,16);
    iblue = strtol(blue,NULL,16);
    write_int(RED_BRIGHTNESS_FILE,ired);
    write_int(GREEN_BRIGHTNESS_FILE,igreen);
    write_int(BLUE_BRIGHTNESS_FILE,iblue);

#ifdef DEBUG
    ALOGD("flashMode=%d flashOnMS=%d flashOffMS=%d brightnessMode=%d ledsModes=%d",
				state->flashMode,
				state->flashOnMS,
				state->flashOffMS,
				state->brightnessMode,
				state->ledsModes);
#endif 
    write_int(RED_DELAYON_FILE, 0);
    write_int(RED_DELAYOFF_FILE, 0);
    write_int(GREEN_DELAYON_FILE, 0);
    write_int(GREEN_DELAYOFF_FILE, 0);
    write_int(BLUE_DELAYON_FILE, 0);
    write_int(BLUE_DELAYOFF_FILE, 0);

    if(!state->flashMode) return;

    if(ired > igreen && ired > iblue) {
	write_int(RED_DELAYON_FILE, state->flashOnMS);
	write_int(RED_DELAYOFF_FILE, state->flashOffMS);
    }
    else if(igreen > ired && igreen > iblue) {
	write_int(GREEN_DELAYON_FILE, state->flashOnMS);
	write_int(GREEN_DELAYOFF_FILE, state->flashOffMS);
    } else {
	write_int(BLUE_DELAYON_FILE, state->flashOnMS);
	write_int(BLUE_DELAYOFF_FILE, state->flashOffMS);
    }
}

static int set_light_notifications(struct light_device_t* dev, struct light_state_t const* state)
{
    int err = 0;
    pthread_mutex_lock(&g_lock);
    set_state(state);
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
	pthread_t lighting_poll_thread;

	int (*set_light) (struct light_device_t *dev,
			  struct light_state_t const *state);

	if (0 == strcmp(LIGHT_ID_BACKLIGHT, name))
		set_light = set_light_backlight;
	else if (0 == strcmp(LIGHT_ID_NOTIFICATIONS, name))
        	set_light = set_light_notifications;
	else if (0 == strcmp(LIGHT_ID_BATTERY, name))
        	set_light = set_light_notifications;
	else if (0 == strcmp(LIGHT_ID_ATTENTION, name))
        	set_light = set_light_notifications;
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
	.name = "Lights for hi6250",
	.author = "Meticulus Development",
	.methods = &lights_methods,
};
