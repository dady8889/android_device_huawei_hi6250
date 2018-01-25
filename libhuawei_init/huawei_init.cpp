/*
 * Meticulus hi6250 libhuawei_init
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

/* Load variant specific props here */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <cutils/klog.h>
#include <sys/system_properties.h>
#include <unistd.h>
#include <stdio.h>
#define BASE "/sys/firmware/devicetree/base/"
#define PRODUCT_PATH BASE"hisi,product_name"
#define MODEM_ID_PATH BASE"hisi,modem_id"
#define PHONE_PROP_PATH "/vendor/phone.prop"
#define BOARDID_PRODUCT_PROP "ro.boardid.product"
#define PRODUCT_MODEL_PROP "ro.product.model"

static char *model = "hi6250";
static char dsdsorig[PROP_VALUE_MAX];

extern "C" int __system_property_add(char *key,int ksize,char * value,int vsize);
extern "C" const prop_info * __system_property_find(const char * name);
extern "C" int __system_property_update(prop_info * pi, char* value,int vsize);
extern "C" int property_get(char *key, char* value, const char *dvalue);

static void set_property(char *key, char *value) {
    int error;
    prop_info* pi = (prop_info*) __system_property_find(key);
    if(pi)	
        error = __system_property_update(pi,value,strlen(value));
    else
    	error = __system_property_add(key,strlen(key),value,strlen(value));
    if(error) {
	klog_write(0, "huawei_init: Could not update or set %s to %s error %d\n",key,value,error);
    }
}

static char * read_string(char * path) {
    char var[255];
    sprintf(var,"%s","ERR");

    int fd = open(PRODUCT_PATH, O_RDONLY);
    if(fd < 0) {
	klog_write(0,"huawei_init: Could not open %s!\n", path);
    }
    if(read(fd,var,255) < 1) {
	klog_write(0,"huawei_init: Unable to read %s!\n", path);
    }
    return strdup(var);
}

static int read_int(char * path) {
     return atoi(read_string(path));
}

void vendor_load_default_properties() {
    model = read_string(PRODUCT_PATH);
    klog_write(0,"huawei_init: model is %s\n",model);

    /* bail if model is NULL */
    if(!model)
	return;

    /* All P9-Lite needs this */
    if(!strncmp(model,"VNS", 3)) {
	set_property(BOARDID_PRODUCT_PROP,"51316");
    /* All Berlin needs this */
    } else if(!strncmp(model, "BLN", 3) || !strncmp(model, "BLL", 3)) {
	set_property(BOARDID_PRODUCT_PROP, "61202");
    /* All Prague needs this */
    } else if(!strncmp(model, "PRA", 3)) {
	set_property(BOARDID_PRODUCT_PROP, "61285");
	set_property("media.settings.xml", "/etc/media_profiles_was.xml");
    /* All Warsaw needs this */
    } else if(!strncmp(model, "WAS", 3)) {
	set_property(BOARDID_PRODUCT_PROP, "61457");
	set_property("media.settings.xml", "/etc/media_profiles_was.xml");
    /* All Honor 5c/Honor 7 lite needs this */
    } else if(!strncmp(model, "NEM", 3) || !strncmp(model, "NMO", 3)) {
	set_property(BOARDID_PRODUCT_PROP, "4873");
    /* Dallas? */
    } else if(!strncmp(model, "DAL", 3)) {
	set_property(BOARDID_PRODUCT_PROP, "6198");
    } else if(!strncmp(model, "BND", 3)) {
	set_property(BOARDID_PRODUCT_PROP, "4753");
    }
    /* if a match is not found the values in the build.prop will be used.
     * ro.boardid.product will not be set so the camera will not work.
     */
}

static int find_product_path(char *path) {
    char *tmodel = strdup(model);
    char *thex = strstr(tmodel, "X");
    char *post;
    int i = 0, ret = -1;
    if(thex) {
        thex[0] = '\0';
        post = thex + 1;
        for(i = 0; i < 10;i ++) {
            sprintf(path, "/product/hw_oem/%s%d%s/prop/local.prop",tmodel,i,post);
	    if(!access(path, O_RDONLY)) {
		ret = 0;
		goto end;
	    }
        }
	ret = -1;
    } else {
        sprintf(path, "/product/hw_oem/%s/prop/local.prop",tmodel);
	ret = 0;
    }
end:
    return ret;
}

static void load_product_props() {
    char prod_prop_path[255];
    char *linebuf = NULL;
    size_t size = PROP_NAME_MAX + PROP_VALUE_MAX + 2;

    if(find_product_path(prod_prop_path)) {
	klog_write(0, "huawei_init: Couldn't find product prop path.");
	return;
    } else {
	klog_write(0, "huawei_init: product path: %s\n", prod_prop_path);
    }

    FILE *fd = fopen(prod_prop_path, "r");
    if(!fd) {
	klog_write(0, "huawei_init: Couldn't read %s?\n", prod_prop_path);
	return;
    }
    while(getline(&linebuf, &size, fd) > -1) {
	/* Meticulus skip empty lines and comments */
	if(linebuf[0] == '\n' || linebuf[0] == '#')
	    continue;

        linebuf[strlen(linebuf) -1] = '\0';
	char *key = strtok(linebuf, "=");
	char *value = strtok(NULL,"=");
	/* Meticulus:
	 * According to VNS-L21 prop file, Real model name
	 * is stored in 'marketing_name' so use this for
	 * ro.product.model.
	 */
	if(!strcmp(key, PRODUCT_MODEL_PROP)) {
	   continue;
	} else if(!strcmp(key, "ro.frp.pst")) {
	   continue;
	} else if(!strcmp(key, "ro.config.marketing_name")) {
	   set_property(PRODUCT_MODEL_PROP, value);
	   klog_write(0, "huawei_init: key='%s' value='%s'\n", key, value);
	} else {
	   set_property(key,value);
	   klog_write(0, "huawei_init: key='%s' value='%s'\n", key, value);
	}
   }
}

static void load_modem_props() {
    int fd = -1,retval = 0,on = 0;
    FILE * pf;
    char buff[255] = {0};
    char modemid[255] = {0};
    fd = open(MODEM_ID_PATH, O_RDONLY);
    if(fd < 0 ) {
	klog_write(0, "huawei_init: Couldn't get modem id?\n");
	return;
    }
    /* Meticulus:
     * The modem id is not characters, so convert to hex.
     */
    size_t size = read(fd, buff, 254);
    sprintf(modemid, "0X%X%X%X%X%X", buff[0], buff[1],buff[2],buff[3],buff[4]);
    close(fd);
    klog_write(0,"huawei_init: modemid = %s\n", modemid);
    sprintf(buff, "[%s]:\n",modemid);
    sprintf(modemid, "%s", buff);

    pf = fopen(PHONE_PROP_PATH, "r");
    if(!pf) {
	klog_write(0, "huawei_init: Couldn't read phone.prop?\n");
	return;
    }
    char *linebuf = NULL;
    size = PROP_NAME_MAX + PROP_VALUE_MAX + 2;
    /* Meticulus:
     * Loop through the prop file and look for the modem id.
     * Once found, set all props until you get empty line
     */
    while(getline(&linebuf, &size, pf) > -1) {
	if(!strcmp(linebuf, modemid)) {
	    klog_write(0, "huawei_init: Found! %s\n",linebuf);
	    on = 1;
	    continue;	
	}
	if(on && linebuf[0] == '\n') {
	    /* Meticulus: Empty line so we are done */
	    break;
	}
	else if(on) {
            linebuf[strlen(linebuf) -1] = '\0';
	    char *key = strtok(linebuf, "=");
	    char *value = strtok(NULL,"=");
	    klog_write(0, "huawei_init: key='%s' value='%s'\n", key, value);
	    set_property(key, value);
	}
    }
    fclose(pf);
    if(!on) {
	klog_write(0, "huawei_init: modemid '%s' was not found in phone.prop\n",modemid);
    }
}

void vendor_load_system_properties() {
    load_modem_props();
    load_product_props();
    /* Meticulus:
     * Save this, in case we need to revert to 'whatever'
     * was in phone.prop
     */
    property_get("persist.dsds.enabled", dsdsorig,"dsds");
}

static void check_single_sim() {
    /* Meticulus:
     * If they have hit the switch for single sim
     * Set these additional single sim properties to
     * make ril work. Based on fix discovered by
     * Konstantin Krstic
     */
    char buff[PROP_VALUE_MAX];

    property_get("persist.radio.multisim.config", buff, "none");
    if(!strcmp(buff, "single")) {
	klog_write(0, "huawei_init: Manual switch to single sim.\n");
	set_property("persist.dsds.enabled", "false");
	set_property("ro.config.client_number", "1");
	set_property("ro.config.modem_number", "1");
    } else {
	set_property("persist.dsds.enabled", dsdsorig);	
    }

}

void vendor_load_persist_properties() {
    check_single_sim();
}
