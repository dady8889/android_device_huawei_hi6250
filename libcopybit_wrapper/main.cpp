#include <utils/Log.h>
#include <hardware/hardware.h>

static hw_module_t *real_module;
static hw_device_t *real_device;
extern int load_real_copybit(char * path, hw_module_t **pHmi);
extern int open_real_copybit(hw_module_t *module, hw_device_t **real_device);
extern int close_real_copybit(hw_device_t *real_device);

int main(void) {
load_real_copybit("/system/lib64/hw/copybit.hi6250.so", &real_module);
if(!open_real_copybit(real_module,&real_device))
    ALOGI("Meticulus copybit open!");
if(!close_real_copybit(real_device))
    ALOGI("Meticulus copybit closed!");
return 0;
}
