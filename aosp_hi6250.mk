#
# Copyright 2014 The Android Open-Source Project
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

$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/aosp_base_telephony.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

PRODUCT_NAME := aosp_hi6250
PRODUCT_DEVICE := hi6250
PRODUCT_BRAND := Huawei
PRODUCT_MODEL := HUAWEI-P9-LITE
PRODUCT_MANUFACTURER := HUAWEI
PRODUCT_RESTRICT_VENDOR_FILES := false

$(call inherit-product, device/huawei/hi6250/device.mk)
$(call inherit-product-if-exists, vendor/huawei/hi6250/device-vendor.mk)
#$(call inherit-product-if-exists, vendor/huawei/hi6250/vendor-additions.mk)
