# AOSPA for the Huawei P9 Lite (hi6250)

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

You'll need Linux to be able to build Cyanogenmod. You have three choices here; you can:

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
  <project name="Meticulus/android_kernel_huawei_hi6250" path="kernel/huawei/hi6250" remote="github" revision="master"/>
  <project name="dady8889/android_device_huawei_hi6250" path="device/huawei/hi6250" remote="github" revision="aospa"/>
  <project name="Meticulus/android_vendor_huawei_hi6250" path="vendor/huawei/hi6250" remote="github" revision="master"/>
</manifest>
```

Execute the following commands in a linux terminal:
```bash
cd ~/Desktop/AOSPA
repo sync -j4
```

NOTE: Yes we are syncing again and No, it shouldn't take quite as long. Every time you repo sync just new data is downloaded. So we are downloading the 3 repo's we just put in and any updates that may have occured to the repo's we already have.

### Step 4: Building

Now you will want to apply the repo patches. These patches modify code in the ROM to work with this device.
Execute the following commands in a linux terminal:
```bash
cd ~/Desktop/AOSPA/device/huawei/hi6250/patches
./patch.sh
```

Now we need to build kernel:
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
NOTE: For some reason, sometimes it shows that it cant find bacon makefile. To fix it, do this:
```bash
cd ~/Desktop/AOSPA
. build/envsetup.sh
breakfast hi6250
brunch hi6250
```
### Please let me know if there are mistakes,typos, mis/outdated - information in these instructions by creating an "issue".
