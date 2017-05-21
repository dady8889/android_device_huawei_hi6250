# AOSPA N for the Huawei P9 Lite (hi6250)

### Kernel
This device tree is intended to be used with this kernel repo:

https://github.com/dady8889/android_kernel_hisi_hi6250

branch: patch
### Vendor
This device tree is intented to be used with this vendor repo

https://github.com/Meticulus/android_vendor_huawei_hi6250

branch: master
# How To Build

### Step 1: Setting up the Build Environment.

This "How to" is scoped for new builders with moderate PC skills.

You'll need Linux to be able to build AOSPA. You have three choices here; you can:

1. Install Linux natively on your computer.
2. Dual boot Linux and Windows.
3. Use virtual machine software ( virtual box, vmware ) to run linux.

NOTE: I recommend you use Ubuntu 15.04 to build. That's what I use.

Now read this: http://source.android.com/source/initializing.html

### Step 2: Downloading the Source.

NOTE: Some say that it is better to download the ROM source and put in your local manifest later. I don't know if that's best but that's what we are going to do.

BEFORE YOU BEGIN: You are about to start downloading 15 - 20 Gigs of data. That could take a very long time, so plan accordingly. I like to start just before I go to sleep and let it go overnight! If you have to work, perhaps start it before work and let it go through out the day.

Execute the following commands in a linux terminal:
```bash
mkdir ~/Desktop/AOSPA-N
cd ~/Desktop/AOSPA-N
repo init -u https://github.com/AOSPA/manifest -b nougat-mr2
repo sync -j4
```
WARNING: There may be times, towards the end when it seem like, the download is stuck and not making any progress because there are no updates on the screen. BE PATIENT!, open a program that will show how much bandwidth you are using to be sure!

### Step 3: Create device entry.

AOSPA does automatically download the needed repos, but we need to add the device entry first.

Execute the following commands in a linux terminal:
```bash
cd ~/Desktop/AOSPA-N/vendor/pa
curl https://gist.githubusercontent.com/dady8889/a4c9456432788faa12a065bf181e4d71/raw | git apply -
```

If there was no git response, the patch was succesful.

### Step 4: Building

Turn off Ninja  
Ninja is supposed to help make incremental updates more efficient but you aren't doing an incremental update; are you?
```bash
export USE_NINJA=false
```

Make sure there is enough memory for Jack (required). Without this you can get out-of-memory errors.
```bash
export ANDROID_JACK_VM_ARGS="-Xmx2g -Dfile.encoding=UTF-8 -XX:+TieredCompilation"
```

Now we need to apply our patches. There is no autopatch at the moment.
```bash
cd ~/Desktop/AOSPA-N/device/huawei/hi6250/patches
./patchtool -p
```
If there were no errors, we can continue.

NOTE: Build times depend on you PC performance specifications. It may take anywhere from 1 hours to 15 hours depending on system specs for a complete build.

Execute the following commands in a linux terminal:
```bash
cd ~/Desktop/AOSPA-N
./rom-build hi6250
```

### Step 5: Troubleshooting
In case it won't download the needed repos in ./rom-build, you need to download them manually.
```bash
cd ~/Desktop/AOSPA-N
repo sync vendor/huawei/hi6250
repo sync device/huawei/hi6250
repo sync kernel/huawei/hi6250
```

### Step 6: Submitting patches
If you would like to submit patch, do the necessary changes to specific repo and make patch with 
the bundled git-makepatch tool, or use git commands (not part of this readme). Necessary information about the tool is in its help command.
You can do 
```bash
~/Desktop/AOSPA-N/device/huawei/hi6250/patches/git-makepatch <patch-name>
```
while you are in target repository. You can add ~/Desktop/AOSPA-N/device/huawei/hi6250/patches to your PATH to shorten the command.
Then you can copy the patch from your desktop and test it in the device tree. You can send the patch by making an "Issue".

### Please let me know if there are mistakes, typos, mis/outdated - information in these instructions by creating an "issue".
