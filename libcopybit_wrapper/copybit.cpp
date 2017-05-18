/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (c) 2010 - 2014, The Linux Foundation. All rights reserved.
 *
 * Not a Contribution, Apache license notifications and license are retained
 * for attribution purposes only.
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

#include <cutils/log.h>

#include <linux/fb.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "copybit.h"

static hw_module_t *real_module;
static hw_device_t *real_device;

extern int load_real_copybit(char * path, hw_module_t **pHmi);
extern int open_real_copybit(hw_module_t *module, hw_device_t **real_device);
extern int close_real_copybit(hw_device_t *real_device);
extern int real_set_parameter(hw_device_t *real_device,int name, int value);
extern int real_get(hw_device_t *real_device,int name);
extern int real_blit(struct hw_device_t *real_device,
 		struct copybit_image_t const *dst,
                struct copybit_image_t const *src,
                struct copybit_region_t const *region);
extern int real_set_sync(hw_device_t *real_device,int acquireFenceFd);
extern int real_stretch(struct hw_device_t *real_device,
                struct copybit_image_t const *dst,
                struct copybit_image_t const *src,
                struct copybit_rect_t const *dst_rect,
                struct copybit_rect_t const *src_rect,
                struct copybit_region_t const *region);
extern int real_fill_color(struct hw_device_t *real_device,
                struct copybit_image_t const *dst,
                struct copybit_rect_t const *rect,
                uint32_t color);
extern int real_finish(struct hw_device_t *real_device);
extern int real_flush_get_fence(struct hw_device_t *real_device, int *fd);
extern int real_clear(struct hw_device_t *real_device, struct copybit_image_t const *buf, struct copybit_rect_t *rect);
extern void close_fake_copybit();

extern void dump_image(const copybit_image_t *image, char *varname);
extern void dump_rect(const copybit_rect_t *rect, char *varname);
extern void dump_parameter(int name, int value);

struct copybit_context_t {
    struct copybit_device_t device; 
};

//#define DEBUG

static int open_copybit(const struct hw_module_t* module, const char* name,
                        struct hw_device_t** device);

static struct hw_module_methods_t copybit_module_methods = {
open:  open_copybit
};

/*
 * The COPYBIT Module
 */
struct copybit_module_t HAL_MODULE_INFO_SYM = {
common: {
tag: HARDWARE_MODULE_TAG,
     version_major: 1,
     version_minor: 0,
     id: COPYBIT_HARDWARE_MODULE_ID,
     name: "Meticulus COPYBIT Wrapper",
     author: "Meticulus Development",
     methods: &copybit_module_methods
        }
};

/** Set a parameter to value */
static int set_parameter_copybit(
    struct copybit_device_t *dev,
    int name,
    int value)
{
#ifdef DEBUG
    ALOGD("%s enter",__func__);
    dump_parameter(name,value);
#endif
    return real_set_parameter(real_device,name,value);
}

/** Get a static info value */
static int get(struct copybit_device_t *dev, int name)
{
    int retval = real_get(real_device, name);
#ifdef DEBUG
    ALOGD("%s enter",__func__);
    dump_parameter(name,retval);
#endif
    return retval;
}

static int set_sync_copybit(struct copybit_device_t *dev,
    int acquireFenceFd)
{
    ALOGD("%s acquireFenceFd=%d",__func__,acquireFenceFd);
    return real_set_sync(real_device,acquireFenceFd);
    return 0;
}

/** do a stretch blit type operation */
static int stretch_copybit(
    struct copybit_device_t *dev,
    struct copybit_image_t const *dst,
    struct copybit_image_t const *src,
    struct copybit_rect_t const *dst_rect,
    struct copybit_rect_t const *src_rect,
    struct copybit_region_t const *region)
{
#ifdef DEBUG
    ALOGD("%s enter",__func__);
    dump_image(dst,"dst");
    dump_image(src,"src");
    dump_rect(dst_rect,"dst_rect");
    dump_rect(src_rect,"src_rect");
#endif
    return real_stretch(real_device,dst,src,dst_rect,src_rect,region);
}

/** Perform a blit type operation */
static int blit_copybit(
    struct copybit_device_t *dev,
    struct copybit_image_t const *dst,
    struct copybit_image_t const *src,
    struct copybit_region_t const *region)
{
#ifdef DEBUG
    ALOGD("%s enter",__func__);
    dump_image(dst,"dst");
    dump_image(src,"src");
#endif
    return real_blit(real_device, dst,src,region);
}

static int finish_copybit(struct copybit_device_t *dev)
{
#ifdef DEBUG
    ALOGD("%s enter",__func__);
#endif
    /* Meticulus: mitigate crash
     * by not calling the real_finish
     */ 
    return 0;
}
static int clear_copybit(struct copybit_device_t *dev,
                         struct copybit_image_t const *buf,
                         struct copybit_rect_t *rect)
{
#ifdef DEBUG
    ALOGD("%s enter",__func__);
#endif
    return real_clear(real_device,buf,rect);
}

/** Fill the rect on dst with RGBA color **/
static int fill_color(struct copybit_device_t *dev,
                      struct copybit_image_t const *dst,
                      struct copybit_rect_t const *rect,
                      uint32_t color)
{
#ifdef DEBUG
    ALOGD("%s enter",__func__);
#endif
    return real_fill_color(real_device,dst,rect,color);
}

/*****************************************************************************/

/** Close the copybit device */
static int close_copybit(struct hw_device_t *dev)
{
#ifdef DEBUG
    ALOGD("%s enter",__func__);
#endif
    return close_real_copybit(real_device);
}

static int flush_get_fence(struct copybit_device_t *dev, int* fd)
{
#ifdef DEBUG
    ALOGD("%s enter",__func__);
#endif
    return real_flush_get_fence(real_device,fd);
}

/** Open a new instance of a copybit device using name */
static int open_copybit(const struct hw_module_t* module, const char* name,
                        struct hw_device_t** device)
{
    int status = -EINVAL;
#ifdef DEBUG
    ALOGD("%s enter",__func__);
#endif

    if (strcmp(name, COPYBIT_HARDWARE_COPYBIT0)) {
        return -1;
    }
    copybit_context_t *ctx;
    ctx = (copybit_context_t *)malloc(sizeof(copybit_context_t));
    memset(ctx, 0, sizeof(*ctx));

    ctx->device.common.tag = HARDWARE_DEVICE_TAG;
    ctx->device.common.version = 1;
    ctx->device.common.module = const_cast<hw_module_t*>(module);
    ctx->device.common.close = close_copybit;
    ctx->device.set_parameter = set_parameter_copybit;
    ctx->device.get = get;
    ctx->device.blit = blit_copybit;
    ctx->device.set_sync = set_sync_copybit;
    ctx->device.stretch = stretch_copybit;
    ctx->device.finish = finish_copybit;
    ctx->device.fill_color = fill_color;
    ctx->device.flush_get_fence = flush_get_fence;
    ctx->device.clear = clear_copybit;
    *device = &ctx->device.common;
    if(status = load_real_copybit("/system/lib64/hw/copybit.real.so", &real_module))
	ALOGE("Could not load real copybit!");	
    if(status = open_real_copybit(real_module,&real_device))
	ALOGE("Could not open real copygit!");
    return status;
}
