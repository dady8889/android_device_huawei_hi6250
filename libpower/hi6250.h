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

#ifndef HI6250_H
#define HI6250_H

#define DDR_FREQ_MIN_PATH 	"/sys/class/devfreq/ddrfreq/min_freq"
#define DDR_FREQ_MAX_PATH 	"/sys/class/devfreq/ddrfreq/max_freq"
#define DDR_FREQ_POLL_PATH	"/sys/class/devfreq/ddrfreq/polling_interval"

#define GPU_FREQ_MIN_PATH 	"/sys/class/devfreq/gpufreq/min_freq"
#define GPU_FREQ_MAX_PATH 	"/sys/class/devfreq/gpufreq/max_freq"
#define GPU_FREQ_POLL_PATH 	"/sys/class/devfreq/gpufreq/polling_interval"
#define GPU_ANIM_BOOST_PATH	"/sys/class/devfreq/gpufreq/mali_ondemand/animation_boost"

#define CPU0_BOOST_PULSE_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/interactive/boostpulse"
#define CPU0_BOOST_P_DUR_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/interactive/boostpulse_duration"

#define CPU0_GOV_PATH	 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"
#define CPU0_FREQ_MIN_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define CPU0_FREQ_MAX_PATH 	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

#define CPU4_BOOST_PULSE_PATH 	"/sys/devices/system/cpu/cpu4/cpufreq/interactive/boostpulse"
#define CPU4_GOV_PATH	 	"/sys/devices/system/cpu/cpu4/cpufreq/scaling_governor"
#define CPU4_FREQ_MIN_PATH	"/sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq"
#define CPU4_FREQ_MAX_PATH	"/sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq"

/* Start profile power_save  */

#define DDR_FREQ_MAX_0	 	"360000000\n"
#define DDR_FREQ_BOOST_0 	"360000000\n"
#define DDR_FREQ_LOW_0	 	"120000000\n"

#define GPU_FREQ_MAX_0	 	"360000000\n"
#define GPU_FREQ_BOOST_0 	"360000000\n"
#define GPU_FREQ_LOW_0	 	"120000000\n"

#define CPU0_BOOST_P_DUR_DEF_0	1
#define CPU0_FREQ_MAX_0	 	"807000\n"
#define CPU0_FREQ_LOW_0	 	"480000\n"

#define CPU4_FREQ_MAX_0	 	"1402000\n"
#define CPU4_FREQ_LOW_0	 	"1402000\n"

/* end profile power_save */

/* Start profile balanced */

#define DDR_FREQ_MAX_1	 	"533000000\n"
#define DDR_FREQ_BOOST_1 	"533000000\n"
#define DDR_FREQ_LOW_1	 	"120000000\n"

#define GPU_FREQ_MAX_1	 	"680000000\n"
#define GPU_FREQ_BOOST_1 	"480000000\n"
#define GPU_FREQ_LOW_1	 	"120000000\n"

#define CPU0_BOOST_P_DUR_DEF_1	4000
#define CPU0_FREQ_MAX_1	 	"1709000\n"
#define CPU0_FREQ_LOW_1	 	"480000\n"

#define CPU4_FREQ_MAX_1	 	"2016000\n"
#define CPU4_FREQ_LOW_1	 	"1402000\n"

/* end profile balanced */

/* Start profile performance */

#define DDR_FREQ_MAX_2	 	"933000000\n"
#define DDR_FREQ_BOOST_2 	"800000000\n"
#define DDR_FREQ_LOW_2	 	"120000000\n"

#define GPU_FREQ_MAX_2	 	"900000000\n"
#define GPU_FREQ_BOOST_2 	"800000000\n"
#define GPU_FREQ_LOW_2	 	"120000000\n"

#define CPU0_BOOST_P_DUR_DEF_2	8000
#define CPU0_FREQ_MAX_2	 	"1709000\n"
#define CPU0_FREQ_LOW_2	 	"480000\n"

#define CPU4_FREQ_MAX_2	 	"2016000\n"
#define CPU4_FREQ_LOW_2	 	"1402000\n"

/* end profile performance */


struct power_profile {
    const char * cpu0_freq_max;
    const char * cpu0_freq_low;
    const int    cpu0_boost_p_dur_def;
    const int    cpu0_should_boost;
    const char * cpu4_freq_max;
    const char * cpu4_freq_low;
    const char * gpu_freq_max;
    const char * gpu_freq_boost;
    const char * gpu_freq_low;
    const int    gpu_should_boost;
    const char * ddr_freq_max;
    const char * ddr_freq_boost;
    const char * ddr_freq_low;
    const int	 ddr_should_boost;
};

struct power_profile power_save  = {
    .cpu0_freq_max = CPU0_FREQ_MAX_0,
    .cpu0_freq_low = CPU0_FREQ_LOW_0,
    .cpu0_boost_p_dur_def = CPU0_BOOST_P_DUR_DEF_0,
    .cpu0_should_boost = 0,
    .cpu4_freq_max = CPU4_FREQ_MAX_0,
    .cpu4_freq_low = CPU4_FREQ_LOW_0,
    .gpu_freq_max = GPU_FREQ_MAX_0,
    .gpu_freq_boost = GPU_FREQ_BOOST_0,
    .gpu_freq_low = GPU_FREQ_LOW_0,
    .gpu_should_boost = 0,
    .ddr_freq_max = DDR_FREQ_MAX_0,
    .ddr_freq_boost = DDR_FREQ_BOOST_0,
    .ddr_freq_low = DDR_FREQ_LOW_0,
    .ddr_should_boost = 0,
};

struct power_profile balanced = {
    .cpu0_freq_max = CPU0_FREQ_MAX_1,
    .cpu0_freq_low = CPU0_FREQ_LOW_1,
    .cpu0_boost_p_dur_def = CPU0_BOOST_P_DUR_DEF_1,
    .cpu0_should_boost = 1,
    .cpu4_freq_max = CPU4_FREQ_MAX_1,
    .cpu4_freq_low = CPU4_FREQ_LOW_1,
    .gpu_freq_max = GPU_FREQ_MAX_1,
    .gpu_freq_boost = GPU_FREQ_BOOST_1,
    .gpu_freq_low = GPU_FREQ_LOW_1,
    .gpu_should_boost = 1,
    .ddr_freq_max = DDR_FREQ_MAX_1,
    .ddr_freq_boost = DDR_FREQ_BOOST_1,
    .ddr_freq_low = DDR_FREQ_LOW_1,
    .ddr_should_boost = 1,
};

struct power_profile performance = {
    .cpu0_freq_max = CPU0_FREQ_MAX_2,
    .cpu0_freq_low = CPU0_FREQ_LOW_2,
    .cpu0_boost_p_dur_def = CPU0_BOOST_P_DUR_DEF_2,
    .cpu0_should_boost = 1,
    .cpu4_freq_max = CPU4_FREQ_MAX_2,
    .cpu4_freq_low = CPU4_FREQ_LOW_2,
    .gpu_freq_max = GPU_FREQ_MAX_2,
    .gpu_freq_boost = GPU_FREQ_BOOST_2,
    .gpu_freq_low = GPU_FREQ_LOW_2,
    .gpu_should_boost = 1,
    .ddr_freq_max = DDR_FREQ_MAX_2,
    .ddr_freq_boost = DDR_FREQ_BOOST_2,
    .ddr_freq_low = DDR_FREQ_LOW_2,
    .ddr_should_boost = 1,
};

#endif
