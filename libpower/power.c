/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2016 Jonathan Jason Dennis [Meticulus]
 *					theonejohnnyd@gmail.com
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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define LOG_TAG "Met-Dev Hisi PowerHAL"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "hi6250.h"

//#define DEBUG

#ifdef DEBUG
#define DEBUG_LOG(x...) ALOGD(x)
#else
#define DEBUG_LOG(x...) do {} while(0)
#endif

static int low_power = 0;
static int dt2w = 0;
static struct power_profile * profile = &performance; 

static void write_string(char * path, char * value) {
    int fd = open(path, O_WRONLY);
	if(!fd) { ALOGE("Unable to open to %s", path); return;}

	ssize_t bytes_written = write(fd, value, strlen(value));

	if (bytes_written < 1 || bytes_written < strlen(value)) {
		ALOGE("Unable to write to %s : %d",path, bytes_written);
	}

    close(fd);

}

static void power_init(struct power_module *module)
{

    DEBUG_LOG("init");
 
    write_string(CPU0_FREQ_MAX_PATH,(* profile).cpu0_freq_max);
    write_string(CPU0_FREQ_MIN_PATH,(* profile).cpu0_freq_low);
    write_string(GPU_FREQ_MIN_PATH,(* profile).gpu_freq_low);
    usleep(500);
    write_string(GPU_FREQ_MAX_PATH,(* profile).gpu_freq_max);
    write_string(GPU_FREQ_POLL_PATH,"50\n");
    write_string(DDR_FREQ_MIN_PATH,(* profile).ddr_freq_low);
    usleep(500);
    write_string(DDR_FREQ_MAX_PATH,(* profile).ddr_freq_max);
    write_string(DDR_FREQ_POLL_PATH,"50\n");
}

static void power_set_interactive(struct power_module *module, int on) {
	DEBUG_LOG("set_interactive %d", on);
	if(on && !low_power) {
	    write_string(GPU_FREQ_MIN_PATH,(* profile).gpu_freq_low);
	    write_string(DDR_FREQ_MIN_PATH,(* profile).ddr_freq_low);
	    write_string(GPU_FREQ_POLL_PATH,"3000\n");
	    write_string(DDR_FREQ_POLL_PATH,"3000\n");
	} else {
	    write_string(GPU_FREQ_MIN_PATH,(* profile).gpu_freq_low);
	    write_string(DDR_FREQ_MIN_PATH,(* profile).gpu_freq_low);
	    write_string(GPU_FREQ_POLL_PATH,"12000\n");
	    write_string(DDR_FREQ_POLL_PATH,"12000\n");
            if(dt2w) write_string(WAKE_ENABLE_PATH,"1\n");
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
	if(!(* profile).gpu_should_boost)
	    return;
	write_string(GPU_ANIM_BOOST_PATH,"1\n");

}

static void power_hint_vsync(int on) {
	if(on) {
	    if((* profile).gpu_should_boost) {
	    	write_string(GPU_FREQ_MIN_PATH, (* profile).gpu_freq_boost);
	    	write_string(GPU_FREQ_POLL_PATH,"1000\n");
	    }
	    if((* profile).ddr_should_boost) {
	    	write_string(DDR_FREQ_MIN_PATH, (* profile).ddr_freq_boost);
	    	write_string(DDR_FREQ_POLL_PATH,"1000\n");
	    }
	} else {
	    if((* profile).gpu_should_boost) {
	        write_string(GPU_FREQ_MIN_PATH, (* profile).gpu_freq_low);
	        write_string(GPU_FREQ_POLL_PATH,"3000\n");
	    }
	    if((* profile).ddr_should_boost) {
	        write_string(DDR_FREQ_MIN_PATH, (* profile).ddr_freq_low);
	        write_string(DDR_FREQ_POLL_PATH,"3000\n");
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
	write_string(GPU_FREQ_POLL_PATH,"0\n");
	write_string(DDR_FREQ_POLL_PATH,"0\n");
    } else {
	write_string(CPU0_FREQ_MAX_PATH,(* profile).cpu0_freq_max);
	write_string(CPU0_FREQ_MIN_PATH,(* profile).cpu0_freq_low);
	write_string(GPU_FREQ_MAX_PATH,(* profile).gpu_freq_max);
	write_string(GPU_FREQ_MIN_PATH,(* profile).gpu_freq_low);
	write_string(DDR_FREQ_MAX_PATH,(* profile).ddr_freq_max);
	write_string(DDR_FREQ_MIN_PATH,(* profile).ddr_freq_low);
	write_string(GPU_FREQ_POLL_PATH,"3000\n");
	write_string(DDR_FREQ_POLL_PATH,"3000\n");
    }
}

static void power_hint_set_profile(struct power_module *module, int p) {

    switch(p) {
	case 0:
	    profile = &power_save;
	    power_init(module);
	    ALOGI("Set power save profile.");
	    break;
	case 1:
	    profile = &balanced;
	    power_init(module);
	    ALOGI("Set balanced profile.");
	    break;
	case 2:
	    profile = &performance;
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
	/*
	case POWER_HINT_CPU_BOOST:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_CPU_BOOST %d", var);
		if(!low_power)
		    power_hint_cpu_boost(var);
		break;
	case POWER_HINT_LAUNCH_BOOST:
		packageName = ((launch_boost_info_t *)data)->packageName;
		pid = ((launch_boost_info_t *)data)->pid;

		 Meticulus: not quite sure what to do with this info?
		 * Set thread prio on the app???

		DEBUG_LOG("POWER_HINT_LAUNCH_BOOST app=%s pid=%d", packageName,pid);
		if(!low_power)
		    power_hint_interactive(0);
		break;
	
	case POWER_HINT_SET_PROFILE:
		if(data != NULL)
		    var = *(int *) data;
		DEBUG_LOG("POWER_HINT_PROFILE %d", var);	
                power_hint_set_profile(module,var);
		break;
        default:
		ALOGE("Unknown power hint %d", hint);
        	break;*/
    }
}

static void set_dt2w(int on) {
    dt2w = on;
    if(on)
	write_string(WAKE_CONF_PATH,"1\n");
    else 
	write_string(WAKE_CONF_PATH,"0\n");
}

static int get_feature(struct power_module *module, feature_t feature) {

    int retval = 0;
    switch(feature) {
	case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
	    retval = 1;
	    break;
	/*
	case POWER_FEATURE_SUPPORTED_PROFILES:
	    retval = 3;
	    break;
        default:
	    ALOGE("Unknown feature %d", feature);
            break;
    */
    }
    return retval;
}

static void set_feature(struct power_module *module, feature_t feature, int state) {
    
    switch(feature) {
	case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
	    set_dt2w(state);
	    break;
	/*
	case POWER_FEATURE_SUPPORTED_PROFILES:
	    ALOGI("POWER_FEATURE_SUPPORTED_PROFILES: %d",state);
	    break;
        default:
	    ALOGE("Unknown feature %d", feature);
            break;
    */
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
        .name = "Met-Dev Hisi Power HAL",
        .author = "Meticulus Development",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .setInteractive = power_set_interactive,
    .powerHint = power_hint,
    .setFeature = set_feature,
    //.getFeature = get_feature,
};
