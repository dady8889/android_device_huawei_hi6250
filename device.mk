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

# Common overlay
DEVICE_PACKAGE_OVERLAYS += $(LOCAL_PATH)/overlay/common

# Inherit from those products. Most specific first.
$(call inherit-product, build/target/product/full_base_telephony.mk)
$(call inherit-product, build/target/product/languages_full.mk)

# Blobs
$(call inherit-product, vendor/huawei/hi6250/vendor.mk)

# 64 bit
$(call inherit-product, build/target/product/core_64_bit.mk)

# Ramdisk
PRODUCT_COPY_FILES += \
	$(call find-copy-subdir-files,*,device/huawei/hi6250/rootdir/,root/)

# Recovery ramdisk, libraries and modules.
PRODUCT_COPY_FILES += \
        $(LOCAL_PATH)/rootdir/init.recovery.balong_modem.rc:root/init.recovery.balong_modem.rc \
        $(LOCAL_PATH)/rootdir/init.recovery.huawei.rc:root/init.recovery.huawei.rc \
        $(LOCAL_PATH)/rootdir/init.recovery.hisi.rc:root/init.recovery.hisi.rc \
        $(LOCAL_PATH)/recovery/init.recovery.hi6250.rc:root/init.recovery.hi6250.rc \
        $(LOCAL_PATH)/recovery/init.recovery.hi6250.usb.rc:root/init.recovery.hi6250.usb.rc \
        $(LOCAL_PATH)/recovery/init.recovery.meticulus.rc:root/init.recovery.meticulus.rc \
        $(LOCAL_PATH)/recovery/init.recovery.usb.rc:root/init.recovery.usb.rc

# Media
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/etc/media_profiles.xml:system/etc/media_profiles.xml \
    $(LOCAL_PATH)/configs/etc/media_profiles_was.xml:system/etc/media_profiles_was.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/etc/media_codecs_google_video.xml

ifeq ($(TARGET_PRODUCT), omni_hi6250)
    PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/etc/omni_media_codecs.xml:system/etc/media_codecs.xml
else ifeq ($(TARGET_PRODUCT), aosp_hi6250)
    PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/etc/omni_media_codecs.xml:system/etc/media_codecs.xml
else 
    PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/etc/media_codecs.xml:system/etc/media_codecs.xml
endif

# Audio
PRODUCT_PACKAGES += \
    libtinyalsa \
    libaudioroute \
    audio.a2dp.default \
    audio.r_submix.default

# Hardware
PRODUCT_PACKAGES += \
    power.hi6250 \
    hwcomposer.hi6250 \
    libcopybit_wrapper \
    lights.hi6250 \
    sensors.hi6250 \
    metiotgd \
    emui5_vendor_symlinks

# Shim libs
PRODUCT_PACKAGES += \
    libshim

# Wifi
PRODUCT_PACKAGES += \
    hostapd_cli

# NFC
PRODUCT_PACKAGES += \
    NQNfcNci \
    libnqnfc-nci \
    com.nxp.nfc.nq \
    com.nxp.nfc.nq.xml \
    nqnfcee_access.xml \
    nqnfcse_access.xml \
    Tag \
    com.android.nfc_extras \
    libp61-jcop-kit \
    nfc_nci.nqx.default

# Prebuilt
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/prebuilt/data_enc_resize:system/bin/data_enc_resize

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml \
    frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml \
    frameworks/native/data/etc/android.hardware.nfc.hce.xml:system/etc/permissions/android.hardware.nfc.hce.xml \
    frameworks/native/data/etc/com.android.nfc_extras.xml:system/etc/permissions/com.android.nfc_extras.xml \
    frameworks/native/data/etc/com.nxp.mifare.xml:system/etc/permissions/com.nxp.mifare.xml \
    frameworks/native/data/etc/android.hardware.fingerprint.xml:system/etc/permissions/android.hardware.fingerprint.xml \
    frameworks/native/data/etc/android.hardware.vulkan.version-1_0_3.xml:system/etc/permissions/android.hardware.vulkan.version-1_0_3.xml \
    frameworks/native/data/etc/android.hardware.vulkan.level-0.xml:system/etc/permissions/android.hardware.vulkan.level-0.xml \
    $(LOCAL_PATH)/configs/etc/permissions/android.hardware.huawei.xml:system/etc/permissions/android.hardware.huawei.xml


# Non-device-specific props
PRODUCT_PROPERTY_OVERRIDES += \
    ro.com.google.locationfeatures=1 \
    ro.setupwizard.mode=OPTIONAL \
    ro.setupwizard.enable_bypass=1 \
    ro.config.sync=yes


PRODUCT_PROPERTY_OVERRIDES += \
    ro.telephony.ril_class=HuaweiRIL
