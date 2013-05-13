#include <disk.h>
#include <c_io.h>
#include <string.h>

uint8_t buff[512];

void run_test(int argc, char **argv) {
	int mode = argv[1][0] - '0';
	int i;
	//c_printf("%s %s\n", argv[0], argv1[
	switch ( mode ) {
		case 0:
			print_devices();
			break;
		case 1:
			disk_write( 0, (uint8_t*)(argv[2]), strlen(argv[2]) );
			break;
		case 2:
			disk_read( 0, buff, 512 );
			for ( i = 0; i < 512; ++i ) {
				c_printf("%c", buff[i]);
				buff[i] = 0;
			}
			c_printf("\n");
			break;
			
	}
}