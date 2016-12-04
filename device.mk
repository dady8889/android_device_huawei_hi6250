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

DEVICE_PACKAGE_OVERLAYS += $(LOCAL_PATH)/overlay

# Inherit from those products. Most specific first.
$(call inherit-product, build/target/product/full_base_telephony.mk)
$(call inherit-product, build/target/product/languages_full.mk)

# Blobs
$(call inherit-product, vendor/huawei/hi6250/vendor.mk)

# 64 bit
$(call inherit-product, build/target/product/core_64_bit.mk)

# Ramdisk
PRODUCT_COPY_FILES += \
        $(LOCAL_PATH)/rootdir/init.hi6250.rc:root/init.hi6250.rc \
        $(LOCAL_PATH)/rootdir/init.meticulus.rc:root/init.meticulus.rc \
        $(LOCAL_PATH)/rootdir/init.platform.rc:root/init.platform.rc \
        $(LOCAL_PATH)/rootdir/init.hisi.rc:root/init.hisi.rc \
        $(LOCAL_PATH)/rootdir/init.connectivity.hisi.rc:root/init.connectivity.hisi.rc \
        $(LOCAL_PATH)/rootdir/init.connectivity.bcm43455.rc:root/init.connectivity.bcm43455.rc \
        $(LOCAL_PATH)/rootdir/fstab.hi6250:root/fstab.hi6250 \
        $(LOCAL_PATH)/rootdir/resetFactory.cfg:root/resetFactory.cfg \
        $(LOCAL_PATH)/rootdir/init.51312.rc:root/init.51312.rc \
        $(LOCAL_PATH)/rootdir/init.post-fs-data.rc:root/init.post-fs-data.rc \
        $(LOCAL_PATH)/rootdir/init.balong_modem.rc:root/init.balong_modem.rc \
        $(LOCAL_PATH)/rootdir/init.tee.rc:root/init.tee.rc \
        $(LOCAL_PATH)/rootdir/init.manufacture.rc:root/init.manufacture.rc \
        $(LOCAL_PATH)/rootdir/init.huawei.rc:root/init.huawei.rc \
        $(LOCAL_PATH)/rootdir/init.audio.rc:root/init.audio.rc \
        $(LOCAL_PATH)/rootdir/init.4871.rc:root/init.4871.rc \
        $(LOCAL_PATH)/rootdir/init.6193.rc:root/init.6193.rc \
        $(LOCAL_PATH)/rootdir/init.connectivity.bcm43xx.rc:root/init.connectivity.bcm43xx.rc \
        $(LOCAL_PATH)/rootdir/init.extmodem.rc:root/init.extmodem.rc \
        $(LOCAL_PATH)/rootdir/ueventd.hi6250.rc:root/ueventd.hi6250.rc \
        $(LOCAL_PATH)/rootdir/fstab.zram256m:root/fstab.zram256m \
        $(LOCAL_PATH)/rootdir/ueventd.51312.rc:root/ueventd.51312.rc \
        $(LOCAL_PATH)/rootdir/init.connectivity.gps.rc:root/init.connectivity.gps.rc \
        $(LOCAL_PATH)/rootdir/init.connectivity.rc:root/init.connectivity.rc \
        $(LOCAL_PATH)/rootdir/fstab.zram512m:root/fstab.zram512m \
        $(LOCAL_PATH)/rootdir/init.hi6250.usb.rc:root/init.hi6250.usb.rc \
        $(LOCAL_PATH)/rootdir/ueventd.4871.rc:root/ueventd.4871.rc \
        $(LOCAL_PATH)/rootdir/init.device.rc:root/init.device.rc \
        $(LOCAL_PATH)/rootdir/ueventd.6193.rc:root/ueventd.6193.rc \
        $(LOCAL_PATH)/rootdir/lpm.rc:root/lpm.rc \
        $(LOCAL_PATH)/rootdir/sbin:root/sbin \
        $(LOCAL_PATH)/rootdir/sbin/teecd:root/sbin/teecd \
        $(LOCAL_PATH)/rootdir/sbin/oeminfo_nvm_server:root/sbin/oeminfo_nvm_server \
        $(LOCAL_PATH)/rootdir/sbin/check_root:root/sbin/check_root \
        $(LOCAL_PATH)/rootdir/sbin/kmsgcat:root/sbin/kmsgcat \
        $(LOCAL_PATH)/rootdir/sbin/hw_crit_service_sys:root/sbin/hw_crit_service_sys \
        $(LOCAL_PATH)/rootdir/sbin/hw_ueventd:root/sbin/hw_ueventd \
        $(LOCAL_PATH)/rootdir/sbin/hdbd:root/sbin/hdbd \
        $(LOCAL_PATH)/rootdir/sbin/volisnotd:root/sbin/volisnotd \
        $(LOCAL_PATH)/rootdir/sbin/ntfs-3gd:root/sbin/ntfs-3gd \
        $(LOCAL_PATH)/rootdir/sbin/logctl_service:root/sbin/logctl_service

# Recovery ramdisk, libraries and modules.
PRODUCT_COPY_FILES += \
        $(LOCAL_PATH)/rootdir/init.recovery.balong_modem.rc:root/init.recovery.balong_modem.rc \
        $(LOCAL_PATH)/rootdir/init.recovery.huawei.rc:root/init.recovery.huawei.rc \
        $(LOCAL_PATH)/rootdir/init.recovery.hisi.rc:root/init.recovery.hisi.rc \
        $(LOCAL_PATH)/recovery/init.recovery.hi6250.rc:root/init.recovery.hi6250.rc \
	    $(LOCAL_PATH)/recovery/twrp.fstab:recovery/root/etc/twrp.fstab \
    	$(LOCAL_PATH)/recovery/stock.twrp.fstab:recovery/root/etc/stock.twrp.fstab \
	    $(LOCAL_PATH)/busybox:recovery/root/sbin/busybox \
	    $(LOCAL_PATH)/recovery/bbInstaller:recovery/root/sbin/bbInstaller \
	    $(LOCAL_PATH)/recovery/fstabSwitcher:recovery/root/sbin/fstabSwitcher \
	    $(LOCAL_PATH)/recovery/fstab.hi6250.stock:recovery/root/fstab.hi6250.stock

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
    lights.hi6250

# Hack for adb
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
    frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml


# Non-device-specific props
PRODUCT_PROPERTY_OVERRIDES += \
    ro.com.google.locationfeatures=1 \
    ro.setupwizard.mode=OPTIONAL \
    ro.setupwizard.enable_bypass=1 \
    ro.config.sync=yes \
    persist.sys.usb.config=mtp


PRODUCT_PROPERTY_OVERRIDES += \
    ro.telephony.ril_class=HuaweiRIL

# Kernel
ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := kernel/huawei/out/arch/arm64/boot/Image
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif
PRODUCT_COPY_FILES += \
	$(LOCAL_KERNEL):kernel

# Essential packages
PRODUCT_PACKAGES += \
    Stk \
    Browser \
    messaging \
    libemoji \
    Eleven
    
# NFC
PRODUCT_PACKAGES += \
    com.android.nfc_extras \
    NQNfcNci \
    libnqnfc-nci \
    com.nxp.nfc.nq \
    com.nxp.nfc.nq.xml \
    nqnfcee_access.xml \
    nqnfcse_access.xml \
    Tag


# Keyboard Swiping
PRODUCT_COPY_FILES += $(LOCAL_PATH)/lib64/libjni_latinime.so:system/lib64/libjni_latinime.so

# Google
PRODUCT_PACKAGES += \
    GoogleKeyboard
    
