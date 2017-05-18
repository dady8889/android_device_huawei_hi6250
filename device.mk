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

# Overlays
DEVICE_PACKAGE_OVERLAYS += $(LOCAL_PATH)/overlay

# DPI
PRODUCT_AAPT_CONFIG := normal xxhdpi
PRODUCT_AAPT_PREF_CONFIG := xxhdpi

# Ramdisk
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/root/init.hi6250.rc:root/init.hi6250.rc \
    $(LOCAL_PATH)/root/init.meticulus.rc:root/init.meticulus.rc \
    $(LOCAL_PATH)/root/init.platform.rc:root/init.platform.rc \
    $(LOCAL_PATH)/root/init.hisi.rc:root/init.hisi.rc \
    $(LOCAL_PATH)/root/init.connectivity.hisi.rc:root/init.connectivity.hisi.rc \
    $(LOCAL_PATH)/root/init.connectivity.bcm43455.rc:root/init.connectivity.bcm43455.rc \
    $(LOCAL_PATH)/root/fstab.hi6250:root/fstab.hi6250 \
    $(LOCAL_PATH)/root/resetFactory.cfg:root/resetFactory.cfg \
    $(LOCAL_PATH)/root/init.51312.rc:root/init.51312.rc \
    $(LOCAL_PATH)/root/init.post-fs-data.rc:root/init.post-fs-data.rc \
    $(LOCAL_PATH)/root/init.balong_modem.rc:root/init.balong_modem.rc \
    $(LOCAL_PATH)/root/init.tee.rc:root/init.tee.rc \
    $(LOCAL_PATH)/root/init.manufacture.rc:root/init.manufacture.rc \
    $(LOCAL_PATH)/root/init.huawei.rc:root/init.huawei.rc \
    $(LOCAL_PATH)/root/init.audio.rc:root/init.audio.rc \
    $(LOCAL_PATH)/root/init.4871.rc:root/init.4871.rc \
    $(LOCAL_PATH)/root/init.6193.rc:root/init.6193.rc \
    $(LOCAL_PATH)/root/init.connectivity.bcm43xx.rc:root/init.connectivity.bcm43xx.rc \
    $(LOCAL_PATH)/root/init.extmodem.rc:root/init.extmodem.rc \
    $(LOCAL_PATH)/root/ueventd.hi6250.rc:root/ueventd.hi6250.rc \
    $(LOCAL_PATH)/root/fstab.zram256m:root/fstab.zram256m \
    $(LOCAL_PATH)/root/ueventd.51312.rc:root/ueventd.51312.rc \
    $(LOCAL_PATH)/root/init.connectivity.gps.rc:root/init.connectivity.gps.rc \
    $(LOCAL_PATH)/root/init.connectivity.rc:root/init.connectivity.rc \
    $(LOCAL_PATH)/root/fstab.zram512m:root/fstab.zram512m \
    $(LOCAL_PATH)/root/init.hi6250.usb.rc:root/init.hi6250.usb.rc \
    $(LOCAL_PATH)/root/ueventd.4871.rc:root/ueventd.4871.rc \
    $(LOCAL_PATH)/root/init.device.rc:root/init.device.rc \
    $(LOCAL_PATH)/root/ueventd.6193.rc:root/ueventd.6193.rc \
    $(LOCAL_PATH)/root/lpm.rc:root/lpm.rc \
    $(LOCAL_PATH)/root/sbin/teecd:root/sbin/teecd \
    $(LOCAL_PATH)/root/sbin/oeminfo_nvm_server:root/sbin/oeminfo_nvm_server \
    $(LOCAL_PATH)/root/sbin/check_root:root/sbin/check_root \
    $(LOCAL_PATH)/root/sbin/kmsgcat:root/sbin/kmsgcat \
    $(LOCAL_PATH)/root/sbin/hw_crit_service_sys:root/sbin/hw_crit_service_sys \
    $(LOCAL_PATH)/root/sbin/hw_ueventd:root/sbin/hw_ueventd \
    $(LOCAL_PATH)/root/sbin/hdbd:root/sbin/hdbd \
    $(LOCAL_PATH)/root/sbin/volisnotd:root/sbin/volisnotd \
    $(LOCAL_PATH)/root/sbin/ntfs-3gd:root/sbin/ntfs-3gd \
    $(LOCAL_PATH)/root/sbin/logctl_service:root/sbin/logctl_service

# Recovery ramdisk
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/root/init.recovery.balong_modem.rc:root/init.recovery.balong_modem.rc \
    $(LOCAL_PATH)/root/init.recovery.huawei.rc:root/init.recovery.huawei.rc \
    $(LOCAL_PATH)/root/init.recovery.hisi.rc:root/init.recovery.hisi.rc

# Media
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/etc/media_profiles.xml:system/etc/media_profiles.xml \
    $(LOCAL_PATH)/configs/etc/media_codecs.xml:system/etc/media_codecs.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/etc/media_codecs_google_video.xml

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
    sensors.hi6250

# Shim Layer
PRODUCT_PACKAGES += \
    libshim

# ADB Hack
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/busybox:root/sbin/sh
#    $(LOCAL_PATH)/configs/etc/wifi/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
#    $(LOCAL_PATH)/configs/etc/wifi/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
#    $(LOCAL_PATH)/configs/etc/wifi/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf

# Bluetooth
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/etc/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf

# Fingerprint
PRODUCT_PACKAGES += \
    fingerprintd

# Wifi
PRODUCT_PACKAGES += \
    hostapd

# Nfc
#PRODUCT_PACKAGES += \
#    NQNfcNci \
#    libnqnfc-nci \
#    com.nxp.nfc.nq \
#    com.nxp.nfc.nq.xml \
#    nqnfcee_access.xml \
#    nqnfcse_access.xml \
#    Tag

# Device Features
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
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
    frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml \
    frameworks/native/data/etc/android.software.vr.xml:system/etc/permissions/android.software.vr.xml


# Non-device-specific Props
PRODUCT_PROPERTY_OVERRIDES += \
    ro.com.google.locationfeatures=1 \
    ro.setupwizard.mode=OPTIONAL \
    ro.setupwizard.enable_bypass=1 \
    ro.config.sync=yes \
    persist.sys.usb.config=mtp

# Device-specific Props
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    sys.usb.configfs=1 \
    sys.usb.controller=hisi-usb-otg

PRODUCT_PROPERTY_OVERRIDES += \
    ro.telephony.ril_class=HuaweiRIL
