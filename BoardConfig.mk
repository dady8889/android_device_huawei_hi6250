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

TARGET_SPECIFIC_HEADER_PATH := device/huawei/hi6250/include
TARGET_OTA_ASSERT_DEVICE := hi6250,VNS,nemo,berlin,venus,prague,bond

# Board
TARGET_BOOTLOADER_BOARD_NAME := hisi
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

# Platform 
TARGET_BOARD_PLATFORM := hi6250
TARGET_CPU_SMP := true
TARGET_USES_64_BIT_BINDER := true
TARGET_USES_HISI_DTIMAGE := true
TARGET_SUPPORTS_32_BIT_APPS := true
TARGET_SUPPORTS_64_BIT_APPS := true
ANDROID_64 :=true

# 1st Arch
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_VARIANT := cortex-a53
TARGET_CPU_ABI := arm64-v8a
TARGET_BOARD_GPU := mali-t830mp2

#2nd Arch
TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_VARIANT := cortex-a15
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi

# Partitions
BOARD_BOOTIMAGE_PARTITION_SIZE := 33554432
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 67108864
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 2080374784
BOARD_USERDATAIMAGE_PARTITION_SIZE := 11231219712
BOARD_FLASH_BLOCK_SIZE := 4096
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true

# Kernel
TARGET_KERNEL_CONFIG := merge_hi6250_defconfig
TARGET_KERNEL_SOURCE := kernel/huawei/hi6250
BOARD_KERNEL_CMDLINE := loglevel=4 coherent_pool=512K page_tracker=on slub_min_objects=12 androidboot.selinux=permissive
BOARD_KERNEL_BASE := 0x00478000
BOARD_KERNEL_PAGESIZE := 2048
BOARD_MKBOOTIMG_ARGS := --kernel_offset 0x00008000 --ramdisk_offset 0x07b88000 --tags_offset 0x07588000
HISI_TARGET_PRODUCT := hi6250
TARGET_ARM_TYPE := arm64
TARGET_KERNEL_CROSS_COMPILE_PREFIX := aarch64-linux-android-

# Init
TARGET_INIT_VENDOR_LIB := libhuawei_init

BERLIN_SHIMS := '/hwvendor/lib64/hwcam/hwcam.hi6250.m.BERLIN.so|libshim_libui.so:/hwvendor/lib64/hwcam/hwcam.hi6250.m.BERLIN.so|libshim.so'
BOND_SHIMS := '/hwvendor/lib64/hwcam/hwcam.hi6250.m.BOND.so|libshim_libui.so:/hwvendor/lib64/hwcam/hwcam.hi6250.m.BOND.so|libshim.so'
DALLAS_SHIMS := '/hwvendor/lib64/hwcam/hwcam.hi6250.m.DALLAS.so|libshim_libui.so:/hwvendor/lib64/hwcam/hwcam.hi6250.m.DALLAS.so|libshim.so'
NEMO_SHIMS := '/hwvendor/lib64/hwcam/hwcam.hi6250.m.NEMO.so|libshim_libui.so:/hwvendor/lib64/hwcam/hwcam.hi6250.m.NEMO.so|libshim.so'
PRAGUE_SHIMS := '/hwvendor/lib64/hwcam/hwcam.hi6250.m.PRAGUE.so|libshim_libui.so:/hwvendor/lib64/hwcam/hwcam.hi6250.m.PRAGUE.so|libshim.so'
VENUS_SHIMS := '/hwvendor/lib64/hwcam/hwcam.hi6250.m.VENUS.so|libshim_libui.so:/hwvendor/lib64/hwcam/hwcam.hi6250.m.VENUS.so|libshim.so'
WARSAW_SHIMS := '/hwvendor/lib64/hwcam/hwcam.hi6250.m.WARSAW.so|libshim_libui.so:/hwvendor/lib64/hwcam/hwcam.hi6250.m.WARSAW.so|libshim.so'

COMMON_SHIMS := '/hwvendor/lib/hw/audio.primary.hisi.so|libshim.so:/hwvendor/lib64/libcamera_algo.so|libshim_libui.so:/hwvendor/lib64/hw/fingerprint.hi6250.so|libshim.so'

# Linker
LD_SHIM_LIBS := $(COMMON_SHIMS):$(BERLIN_SHIMS):$(BOND_SHIMS):$(DALLAS_SHIMS):$(NEMO_SHIMS):$(PRAGUE_SHIMS):$(VENUS_SHIMS):$(WARSAW_SHIMS)

## Uncomment to see LD_SHIM_LIBS var
#$(shell echo $(LD_SHIM_LIBS) >&2)

# RIL
BOARD_RIL_CLASS := ../../../device/huawei/hi6250/ril/
SIM_COUNT := 2
BOARD_PROVIDES_RILD := true
BOARD_PROVIDES_LIBRIL := true
BOARD_PROVIDES_LIBREFERENCE_RIL := true

# Graphics
USE_OPENGL_RENDERER := true
TARGET_HARDWARE_3D := true
BOARD_EGL_CFG := device/huawei/hi6250/egl.cfg
HWUI_COMPILE_SYMBOLS := true

# Surfaceflinger
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 6

# NFC
BOARD_NFC_CHIPSET := pn548
TARGET_USES_NQ_NFC := true
NXP_CHIP_TYPE := 2

# OTA
# BLOCK_BASED_OTA := false

# WIFI
WPA_SUPPLICANT_VERSION := VER_0_8_X
BOARD_HOSTAPD_DRIVER := NL80211

# Screen
TARGET_SCREEN_HEIGHT := 1920
TARGET_SCREEN_WIDTH := 1080

# Audio
BOARD_USES_ALSA_AUDIO := true

# Define kind of DPI
PRODUCT_AAPT_CONFIG := normal xxhdpi
PRODUCT_AAPT_PREF_CONFIG := xxhdpi

# Charger 
BOARD_CHARGER_DISABLE_INIT_BLANK := true
BACKLIGHT_PATH := "/sys/class/leds/lcd_backlight0/brightness"
HEALTHD_BACKLIGHT_LEVEL := 102
HEALTHD_FORCE_BACKLIGHT_CONTROL := true

# Recovery
TARGET_RECOVERY_FSTAB := device/huawei/hi6250/rootdir/fstab.hi6250

# cmhw
BOARD_HARDWARE_CLASS := device/huawei/hi6250/cmhw/

# SELinux
BOARD_SEPOLICY_DIRS += \
	device/huawei/hi6250/sepolicy

ifneq ($(TARGET_PRODUCT), aosp_hi6250)
BOARD_SEPOLICY_DIRS += \
	device/huawei/hi6250/cm_sepolicy
endif

