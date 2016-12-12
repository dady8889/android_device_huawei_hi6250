#include "vendor_init.h"
#include <errno.h>

/* init vendor override stubs */

__attribute__ ((weak))
void vendor_load_properties()
{
}

__attribute__ ((weak))
int vendor_handle_control_message(const char *msg, const char *arg)
{
    return -ENOSYS;
}

int main(void) {
vendor_load_properties();
return 0;
}
