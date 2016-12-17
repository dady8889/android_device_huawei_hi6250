/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (c) 2016 Jonathan Jason Dennis [Meticulus]
 *				theonejohnnyd@gmail.com
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

/* Load variant specific props here */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <cutils/klog.h>
#include <unistd.h>
#define BASE "/sys/firmware/devicetree/base/"
#define PRODUCT_PATH BASE"hisi,product_name"
#define MULTI_RILD_PROP "ro.multi.rild"
#define BOARDID_PRODUCT_PROP "ro.boardid.product"
#define PRODUCT_MODEL_PROP "ro.product.model"

static char *model = "hi6250";

extern "C" int __system_property_add(char *key,int ksize,char * value,int vsize);

static void set_property(char *key, char *value) {
    int error;
    if(error = __system_property_add(key,strlen(key),value,strlen(value))) {
	klog_write(0, "libhuawei_init: Could not set %s to %s error %d\n",key,value,error);
    }
}
static char * read_string(char * path) {
    char var[255];
    sprintf(var,"%s","ERR");

    int fd = open(PRODUCT_PATH, O_RDONLY);
    if(fd < 0) {
	klog_write(0,"Could not open %s!\n", path);
    }
    if(read(fd,var,255) < 1) {
	klog_write(0,"Unable to read %s!\n", path);
    }
    return strdup(var);
}

static int read_int(char * path) {
     return atoi(read_string(path));
}

void vendor_load_properties() {
    model = read_string(PRODUCT_PATH);
    klog_write(0,"libhuawei_init: model is %s\n",model);

    /* bail if model is NULL */
    if(!model)
	return;

    /* All P9-Lite needs this */
    if(!strncmp(model,"VNS", 3)) 
	set_property(BOARDID_PRODUCT_PROP,"51312");
    /* All Honor 5c/Honor 7 lite needs this */
    if(!strncmp(model, "NEM", 3) || !strncmp(model, "NMO", 3))
	set_property(BOARDID_PRODUCT_PROP, "4871");

    /* These models need ro.multi.rild true */
    if(!strcmp(model, "VNS-L31") ||
       !strcmp(model, "VNS-L22") ||
       !strcmp(model, "NEM-L21")) {
	set_property(MULTI_RILD_PROP, "true");
    } else {
	set_property(MULTI_RILD_PROP, "false");	
    }

    /* These have renamed model */
    if(!strcmp(model, "NMO-L21"))
	set_property(PRODUCT_MODEL_PROP, "NEM-L51");
    else
    	set_property(PRODUCT_MODEL_PROP,model);
 

    /* if a match is not found the values in the build.prop will be used.
     * ro.boardid.product will not be set so the camera will not work.
     */
}

int vendor_handle_control_message(const char *msg, const char *arg) { 
	klog_write(0, "libhuawei_init: control message '%s' '%s' arg is not implemented!\n",msg,arg);
	return -1;
}
