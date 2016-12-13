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

extern "C" int __system_property_add(char *key,int ksize,char * value,int vsize);

static void set_property(char *key, char *value) {
    int error;
    if(error = __system_property_add(key,strlen(key),value,strlen(value))) {
	klog_write(0, "libhuawei_init: Could not set %s to %s error %d\n",key,value,error);
    }
}
static char * read_string(char * path, int maxsize) {
    char var[255];
    sprintf(var,"%s","ERR");

    int fd = open(PRODUCT_PATH, O_RDONLY);
    if(fd < 0) {
	klog_write(0,"Could not open %s!\n", path);
    }
    if(read(fd,var,255) < 1) {
	klog_write(0,"Unable to read %s!\n", path);
    }
    return var;
}

static int read_int(char * path) {
     return atoi(read_string(path,255));
}

void vendor_load_properties() {
    char * model = read_string(PRODUCT_PATH,255);
    klog_write(0,"libhuawei_init: model is %s\n",model);
    if(!strcmp(model,"VNS-L21")) {
	set_property("ro.config.mulirild","false");
	set_property("ro.boardid.product","51312");
	set_property("ro.product.model","VNS-L21");
    }
    else if(!strcmp(model,"VNS-L31")) {
	set_property("ro.config.mulirild","true");
	set_property("ro.boardid.product","51312");
	set_property("ro.product.model","VNS-L31");
    }
    else if(!strcmp(model,"VNS-L22")) {
	set_property("ro.config.mulirild","true");
	set_property("ro.boardid.product","51312");
	set_property("ro.product.model","VNS-L22");
    }
    else if(!strcmp(model,"VNS")) {
	set_property("ro.config.mulirild","false");
	set_property("ro.boardid.product","51312");
	set_property("ro.product.model","VNS-GENERIC");
    }
    else if(!strcmp(model,"NEM-AL60")) {
	set_property("ro.config.mulirild","false");
	set_property("ro.boardid.product","4871");
	set_property("ro.product.model","NEM-AL10");
    }
    else if(!strcmp(model,"NEM-UL10")) {
	set_property("ro.config.mulirild","false");
	set_property("ro.boardid.product","4871");
	set_property("ro.product.model","NEM-UL10");
    }
    else if(!strcmp(model,"NEM-L21")) {
	set_property("ro.config.mulirild","true");
	set_property("ro.boardid.product","4871");
	set_property("ro.product.model","NEM-L21");
    }
    else if(!strcmp(model,"NEM")) {
	set_property("ro.config.mulirild","false");
	set_property("ro.boardid.product","4871");
	set_property("ro.product.model","NEM-GENERIC");
    }
    else if(!strcmp(model,"NMO-L21")) {
	set_property("ro.config.mulirild","false");
	set_property("ro.boardid.product","4871");
	set_property("ro.product.model","NMO-L21");
    }
    else if(!strcmp(model,"NMO")) {
	set_property("ro.config.mulirild","false");
	set_property("ro.boardid.product","4871");
	set_property("ro.product.model","NMO-GENERIC");
    }

    /* if a match is not found the values in the build.prop will be used.
     * ro.boardid.product will not be set so the camera will not work.
     */
}

int vendor_handle_control_message(const char *msg, const char *arg) { 
	klog_write(0, "libhuawei_init: control message '%s' '%s' arg is not implemented!\n",msg,arg);
	return -1;
}
