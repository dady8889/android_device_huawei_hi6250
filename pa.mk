#
# Copyright (C) 2016 Jonathan Jason Dennis (theonejohnnyd@gmail.com)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Inherit AOSPA Stuff
TARGET_BOOT_ANIMATION_RES := 1080
$(call inherit-product-if-exists, vendor/pa/main.mk)

# Inherit device configuration
$(call inherit-product, device/huawei/hi6250/device.mk)

# Device identifier
PRODUCT_DEVICE := hi6250
PRODUCT_NAME := cm_hi6250
PRODUCT_BRAND := huawei
PRODUCT_MODEL := P9-Lite
PRODUCT_MANUFACTURER := huawei

TARGET_SCREEN_HEIGHT := 1920
TARGET_SCREEN_WIDTH := 1080

ADDITIONAL_DEFAULT_PROPERTIES += \
    ro.zygote=zygote64_32 \
    ro.adb.secure=0 \
    ro.secure=0
