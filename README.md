# AOSPA (Paranoid Android) for the Huawei P9 Lite (hi6250)

### Kernel
This device tree is intended to be used with this kernel repo:

See: https://github.com/Meticulus/android_kernel_huawei_hi6250

branch: master
### Vendor
This device tree is intented to be used with this vendor repo

https://github.com/Meticulus/android_vendor_huawei_hi6250

branch: master
# How To Build

### Step 1: Setting up the Build Environment.

NOTE: Final size of downloaded + built files was around 120 GB. So I recommend allocating at least 150 GB for the drive.

You'll need Linux to be able to build AOSPA. You have three choices here; you can:

1. Install Linux natively on your computer.
2. Dual boot Linux and Windows.
3. Use virtual machine software ( virtual box, vmware ) to run linux.

NOTE: I recommend you use Ubuntu 14.04 LTS to build. That's what I use. I had problems with Debian so really use it.

Now read this: http://source.android.com/source/initializing.html

NOTE: When I say "read", I mean read and comprehend.

NOTE: Read through the topics here as well: http://wiki.cyanogenmod.org/w/Development

### Step 2: Downloading the Source.

NOTE: Some say that it is better to download the ROM source and put in your local manifest later. I don't know if that's best but that's what we are going to do.

BEFORE YOU BEGIN: You are about to start downloading 5 - 15 Gigs of data. That could take a very long time, so plan accordingly. I like to start just before I go to sleep and let it go overnight! If you have to work, perhaps start it before work and let it go through out the day.

Execute the following commands in a linux terminal:
```bash
mkdir ~/Desktop/AOSPA
cd ~/Desktop/AOSPA
repo init -u https://github.com/AOSPA/manifest -b marshmallow
repo sync -j4
```
WARNING: There may be times, towards the end when it seem like, the download is stuck and not making any progress because there are no updates on the screen. BE PATIENT!, open a program that will show how much bandwidth you are using to be sure!

### Step 3: Set up local manifest.

The local manifest is different for every device. It contains those repos that are device specific, where as the ROM code you just "repo sync'd" is code that is general to any device.

Execute the following commands in a linux terminal:
```bash
mkdir ~/Desktop/AOSPA/.repo/local_manifests
gedit ~/Desktop/AOSPA/.repo/local_manifests/hi6250.xml
```
Copy the following into hi6250.xml, save and close.
```bash
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
<!-- Device specific -->
  <project name="Meticulus/android_kernel_huawei_hi6250" path="kernel/huawei/hi6250" remote="github" revision="master"/>
  <project name="dady8889/android_device_huawei_hi6250" path="device/huawei/hi6250" remote="github" revision="aospa"/>
  <project name="Meticulus/android_vendor_huawei_hi6250" path="vendor/huawei/hi6250" remote="github" revision="master"/>

<!-- Removes -->
  <remove-project name="AOSPA/android_system_core"/>
  <remove-project name="platform/system/bt"/>
  <remove-project name="platform/system/media"/>
  <remove-project name="platform/system/keymaster"/>
  <remove-project name="AOSPA/android_frameworks_av"/>
  <remove-project name="AOSPA/android_frameworks_native"/>
  <remove-project name="platform/hardware/libhardware"/>
  <remove-project name="platform/system/extras"/>
  <remove-project name="platform/external/f2fs-tools"/>
  <remove-project name="platform/external/e2fsprogs"/>
  <remove-project name="platform/system/vold"/>
  <remove-project name="platform/packages/apps/Browser"/>
  <remove-project name="platform/packages/apps/Nfc"/>
  
<!-- CyanogenMod -->
  <project name="CyanogenMod/android_system_core" path="system/core" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_system_bt" path="system/bt" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_system_media" path="system/media" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_system_keymaster" path="system/keymaster" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_frameworks_av" path="frameworks/av" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_frameworks_native" path="frameworks/native" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_hardware_libhardware" path="hardware/libhardware" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_external_ffmpeg" path="external/ffmpeg" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_external_stagefright-plugins" path="external/stagefright-plugins" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_system_extras" path="system/extras" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_external_f2fs-tools" path="external/f2fs-tools" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_external_e2fsprogs" path="external/e2fsprogs" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_system_vold" path="system/vold" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_packages_apps_Browser" path="packages/apps/Browser" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_packages_apps_Eleven" path="packages/apps/Eleven" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_Nfc" path="vendor/nxp-nfc/opensource/Nfc" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_libnfc-nci" path="vendor/nxp-nfc/opensource/libnfc-nci" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_frameworks" path="vendor/nxp-nfc/opensource/frameworks" remote="github" revision="cm-13.0"/>
  
<!-- Extras -->
  <remove-project name="platform/packages/apps/DeskClock"/>
  <project name="platform/packages/apps/DeskClock" path="packages/apps/DeskClock" remote="aosp" revision="refs/tags/android-7.0.0_r21"/>
</manifest>
```

Execute the following commands in a linux terminal:
```bash
cd ~/Desktop/AOSPA
repo sync -j4
```

### Step 4: Building

Now you will want to apply the repo patches. These patches modify code in the ROM to work with this device.
Execute the following commands in a linux terminal:
```bash
cd ~/Desktop/AOSPA/device/huawei/hi6250/patches
./patch.sh
```

Now we need to build the kernel:
```bash
cd ~/Desktop/AOSPA/kernel/huawei/hi6250
```
Read README_Kernel.txt and build the kernel with those instructions.
After it's built, continue with building AOSPA.

NOTE: It may take anywhere from 5 hours to 15 hours depending on system specs for a complete build.
Execute the following commands in a linux terminal:
```bash
cd ~/Desktop/AOSPA
./rom-build.sh hi6250
```

### Step 5: What if I got an error?

#### Bacon Makefile Missing

For some reason, sometimes it shows that it cant find bacon makefile. 
Execute the following commands in a linux terminal:
```bash
cd ~/Desktop/AOSPA
. build/envsetup.sh
breakfast hi6250
brunch hi6250
```

#### make: *** [out/blabla/classes.dex] Error 41

Execute the following commands in a linux terminal:
```bash
chmod 777 ~/.jack
```

### Please let me know if there are mistakes,typos, mis/outdated - information in these instructions by creating an "issue".
