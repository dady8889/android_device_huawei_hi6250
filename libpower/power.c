/*
 * Meticulus hi6250 libpower
 * Copyright (c) 2017 Jonathan Dennis [Meticulus]
 *                               theonejohnnyd@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define LOG_TAG "Meticulus Power"
#include <utils/Log.h>
#include <cutils/properties.h>
#include <hardware/hardware.h>
#include <hardware/power.h>

#include "venus.h"
#include "warsaw.h"

//#define DEBUG

#ifdef DEBUG
#define DEBUG_LOG(x...) ALOGD(x)
#else
#define DEBUG_LOG(x...) do {} while(0)
#endif

#define STOCK_PROP "persist.sys.stock_power_HAL"
#define BASE "/sys/firmware/devicetree/base/"
#define PRODUCT_PATH BASE"hisi,product_name"

static hw_module_t *stock_power_module;
extern int load_stock_power(char *path, hw_module_t **pHmi);

static char stock_p_path[255] = "/system/lib64/hw/power.default.so";

static int stock_power = 0;
static int low_power = 0;
static struct power_profile power_save;
static struct power_profile balanced;
static struct power_profile performance;
static struct power_profile * profile = &performance; 
static struct power_profile * sel_profile = &performance;
static int init = 0;
static void write_string(const char * path, const char * value) {
    int fd = open(path, O_WRONLY);
	if(fd == -1 ) { ALOGE("Unable to open to %s", path); return;}

	unsigned long bytes_written = write(fd, value, strlen(value));

	if (bytes_written < 1 || bytes_written < strlen(value)) {
		ALOGE("Unable to write to %s : %lu",path, bytes_written);
	}

    close(fd);

}

static void power_init(struct power_module *module)
{
    int fd = -1;
    char model[255];
    stock_power = property_get_bool(STOCK_PROP,false) ? 1 : 0;
    if(stock_power) {
	ALOGI("%s is set. Loading Stock Power HAL...", STOCK_PROP);
	if(load_stock_power(stock_p_path, &stock_power_module)) {
	    ALOGE("%sis set but can't load Stock Power HAL!", STOCK_PROP);
	    property_set(STOCK_PROP, "false");
	    stock_power = 0;
	}
    }

    if(stock_power && stock_power_module && ((power_module_t *)stock_power_module)->init) {
	ALOGI("->Stock Power HAL init");
	return ((power_module_t *)stock_power_module)->init((power_module_t *)stock_power_module);
    }
    ALOGI("init");

    fd = open(PRODUCT_PATH, O_RDONLY);
    if(fd < 0) {
        ALOGE("Could not read model! using VNS");
        sprintf(model,"VNS-L21");
    } else {
        if(read(fd,model,255) < 1) {
            ALOGE("Could not read model! using VNS");
            sprintf(model,"VNS-L21");
        }
    }
    if(!strncmp(model, "WAS", 3)) {
        power_save = warsaw_power_save;
        balanced = warsaw_balanced;
        performance = warsaw_performance;
    } else {
        power_save = venus_power_save;
        balanced = venus_balanced;
        performance = venus_performance;
    }
    if(!init) {
	sel_profile = &performance;
	profile = &performance;
	init = 1;
    }

    write_string(CPU0_FREQ_MAX_PATH,(*profile).cpu0_freq_max);
    write_string(CPU0_FREQ_MIN_PATH,(* profile).cpu0_freq_low);
    write_string(CPU4_FREQ_MAX_PATH,(*profile).cpu4_freq_max);
    write_string(CPU4_FREQ_MIN_PATH,(* profile).cpu4_freq_low);
    write_string(GPU_FREQ_MIN_PATH,(* profile).gpu_freq_low);
    write_string(GPU_FREQ_MAX_PATH,(* profile).gpu_freq_max);
    write_string(GPU_FREQ_POLL_PATH,"50\n");
    write_string(DDR_FREQ_MIN_PATH,(* profile).ddr_freq_low);
    write_string(DDR_FREQ_MAX_PATH,(* profile).ddr_freq_max);
    write_string(DDR_FREQ_POLL_PATH,"50\n");
    write_string(GPU_ANIM_BOOST_F_PATH,(*profile).gpu_freq_boost);
    write_string(GPU_HISPEED_F_PATH,(*profile).gpu_freq_max);
}

static void power_set_interactive(struct power_module *module, int on) {
    if(stock_power && stock_power_module && ((power_module_t *)stock_power_module)->setInteractive) {
        ALOGI("->Stock Power HAL: setInteractive %d", on);
	    return ((power_module_t *)stock_power_module)->setInteractive((power_module_t *)stock_power_module,on);
    } 

    ALOGI("setInteractive %d", on);
    if (low_power)
	return;

    if (!on) {
	profile = &power_save;
	power_init(module);
    } else {
	profile = sel_profile;
	power_init(module);
    }
}

static void power_hint_cpu_boost(int dur) {
    char sdur[255];

    if(!(* profile).cpu0_should_boost)
	return;

    if(!dur)
	dur = (* profile).cpu0_boost_p_dur_def;
	
    sprintf(sdur, "%d\n", dur);
    write_string(CPU0_BOOST_P_DUR_PATH,sdur); 
    write_string(CPU0_BOOST_PULSE_PATH,"1\n"); 
}

static void power_hint_interactive(int on) {
    if(on && (* profile).gpu_should_boost) {
	write_string(GPU_ANIM_BOOST_PATH,"1\n");
	power_hint_cpu_boost(on);
    } else {
	write_string(GPU_ANIM_BOOST_PATH,"0\n");
    }
}

static void power_hint_vsync(int on) {
	if(on) {
	    if((* profile).gpu_should_boost) {
	    	write_string(GPU_VSYNCUP_PATH, "50\n");
	    	write_string(GPU_NOVSYNCUP_PATH,"50\n");
	    }
	    if((* profile).ddr_should_boost) {
	    	write_string(DDR_FREQ_MAX_PATH, (* profile).ddr_freq_boost);
	    	write_string(DDR_FREQ_POLL_PATH,"100\n");
	    }
	} else {
	    if((* profile).gpu_should_boost) {
	    	write_string(GPU_VSYNCUP_PATH, "85\n");
	    	write_string(GPU_NOVSYNCUP_PATH,"85\n");
	    }
	    if((* profile).ddr_should_boost) {
	        write_string(DDR_FREQ_MAX_PATH, (* profile).ddr_freq_max);
	        write_string(DDR_FREQ_POLL_PATH,"300\n");
            }
	}
}

static void power_hint_low_power(int on) {
    low_power = on;
    if(on) {
	write_string(CPU0_FREQ_MAX_PATH,(* profile).cpu0_freq_low);
	write_string(CPU0_FREQ_MIN_PATH,(* profile).cpu0_freq_low);
	write_string(GPU_FREQ_MAX_PATH,(* profile).gpu_freq_low);
	write_string(GPU_FREQ_MIN_PATH,(* profile).gpu_freq_low);
	write_string(DDR_FREQ_MAX_PATH,(* profile).ddr_freq_low);
	write_string(DDR_FREQ_MIN_PATH,(* profile).ddr_freq_low);
	write_string(GPU_FREQ_POLL_PATH,"1200\n");
	write_string(DDR_FREQ_POLL_PATH,"1200\n");
    } else {
	write_string(CPU0_FREQ_MAX_PATH,(* profile).cpu0_freq_max);
	write_string(CPU0_FREQ_MIN_PATH,(* profile).cpu0_freq_low);
	write_string(GPU_FREQ_MAX_PATH,(* profile).gpu_freq_max);
	write_string(GPU_FREQ_MIN_PATH,(* profile).gpu_freq_low);
	write_string(DDR_FREQ_MAX_PATH,(* profile).ddr_freq_max);
	write_string(DDR_FREQ_MIN_PATH,(* profile).ddr_freq_low);
	write_string(GPU_FREQ_POLL_PATH,"300\n");
	write_string(DDR_FREQ_POLL_PATH,"300\n");
    }
}

static void power_hint_set_profile(struct power_module *module, int p) {

    switch(p) {
	case 0:
	    profile = &power_save;
	    sel_profile = &power_save;
	    power_init(module);
	    ALOGI("Set power save profile.");
	    break;
	case 1:
	    profile = &balanced;
	    sel_profile = &balanced;
	    power_init(module);
	    ALOGI("Set balanced profile.");
	    break;
	case 2:
	    profile = &performance;
	    sel_profile = &performance;
	    power_init(module);
	    ALOGI("Set performance profile.");
	    break;
        default:
	    ALOGE("Unknown power profile %d", p);
	    break;
    }
}

static void power_hint(struct power_module *module, power_hint_t hint,
                       void *data) {

    if(stock_power && stock_power_module && ((power_module_t *)stock_power_module)->powerHint) {
	DEBUG_LOG("->Stock Power HAL: power hint 0x%x",hint);
	return ((power_module_t *)stock_power_module)->powerHint((power_module_t *)stock_power_module,hint,data);
    }

    int var = 0;
    char * packageName;
    int pid = 0;
    switch (hint) {
	case POWER_HINT_VSYNC:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_VSYNC %d", var);
		if(!low_power)
		    power_hint_vsync(var);
		break;
	case POWER_HINT_INTERACTION:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_INTERACTION %d", var);
		if(!low_power)
		    power_hint_interactive(var);
		break;
	case POWER_HINT_LOW_POWER:
		var = data == NULL ? 0 : 1;
		DEBUG_LOG("POWER_HINT_LOW_POWER %d", var);
		power_hint_low_power(var);
		break;
	case POWER_HINT_LAUNCH:
		DEBUG_LOG("POWER_HINT_LAUNCH");
		if(!low_power)
		    power_hint_interactive(0);
		break;
#ifndef AOSP
	case POWER_HINT_CPU_BOOST:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_CPU_BOOST %d", var);
		if(!low_power)
		    power_hint_cpu_boost(var);
		break;
#endif
#ifdef CMEXTRAS
	case POWER_HINT_SET_PROFILE:
#else
#ifndef AOSP
	case POWER_HINT_POWER_PROFILE:
#endif
#endif
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_PROFILE %d", var);	
                power_hint_set_profile(module,var);
		break;
	case POWER_HINT_SUSTAINED_PERFORMANCE:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_SUSTAINED_PREFORMANCE %d", var);
		if(var)
		    profile = &performance;
		else
		    profile = sel_profile;
		power_init(module);
		break;
        default:
		ALOGE("Unknown power hint %d", hint);
        	break;
    }
}

static int get_feature(struct power_module *module, feature_t feature) {

    if(stock_power) return -1;

    int retval = 0;
    switch(feature) {
#ifdef CMEXTRAS
	case POWER_FEATURE_SUPPORTED_PROFILES:
	    retval = 3;
	    break;
#endif
        default:
	    ALOGE("Unknown feature %d", feature);
            break;
    }
    return retval;
}

static void set_feature(struct power_module *module, feature_t feature, int state) {

    if(stock_power) return;

    switch(feature) {
#ifdef CMEXTRAS
	case POWER_FEATURE_SUPPORTED_PROFILES:
	    ALOGI("POWER_FEATURE_SUPPORTED_PROFILES: %d",state);
	    break;
#endif
        default:
	    ALOGE("Unknown feature %d", feature);
            break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_3,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "Meticulus Power",
        .author = "Meticulus Development",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .setInteractive = power_set_interactive,
    .powerHint = power_hint,
    .setFeature = set_feature,
#ifdef CMEXTRAS
    .getFeature = get_feature,
#endif
};
