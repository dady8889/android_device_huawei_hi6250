#include <linux/input.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>

#define VOLUMEINPUTPATH "/dev/input/event0"

int main(int argc, char *argv[]) {
	int fd = -1,retval = -2;
	fd_set read_set;
	struct input_event ev;
	struct timeval tv;
	int size = sizeof(struct input_event);
	/* Initialize the structures */
	memset(&ev, 0x00, sizeof(ev));
	memset(&tv, 0x00, sizeof(tv));

	/* open input device */
	if ((fd = open(VOLUMEINPUTPATH, O_RDONLY)) > 0) {
		while(retval == -2) {
		/* Intialize the read descriptor */
		FD_ZERO(&read_set);
		FD_SET(fd,&read_set);
		/* Wait up to 0.5 seconds. */
		tv.tv_sec = 0 ;
		tv.tv_usec = 500000;
		if(select(fd+1, &read_set, NULL, NULL, &tv) < 0) {
			return -1;
		}
		/* FD_ISSET(0, &rfds) will be true. */
		if (FD_ISSET(fd, &read_set)) {
			read(fd, &ev, size );
			if(ev.type ==  1) {
				switch (ev.code) {
					case 114:
						printf("down\n");
						retval = -1;
						break;
					case 115:
						printf("up\n");
						retval = 0;
						break;

				}	
	
			} 
	
		}
		} 
	close(fd);
	} else {
		printf("Could not open %s", VOLUMEINPUTPATH);
	}
		return retval;	

}


