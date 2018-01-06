#
# Meticulus hi6250 libpower
# Copyright (c) 2017 Jonathan Dennis [Meticulus]
#                               theonejohnnyd@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := power.hi6250
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES := power.c stock_power_interface.c
LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_MODULE_TAGS := optional

ifneq ($(TARGET_PRODUCT), omni_hi6250)
ifneq ($(TARGET_PRODUCT), aosp_hi6250)
ifneq ($(TARGET_PRODUCT), bliss_hi6250)
LOCAL_CFLAGS += -DCMEXTRAS
else
LOCAL_CFLAGS += -DAOSP
endif
endif
endif

include $(BUILD_SHARED_LIBRARY)
