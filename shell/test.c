#include <disk.h>
#include <c_io.h>
#include <string.h>
#include <ofs.h>

uint8_t buff[512];

void run_test(int argc, char **argv) {
	int i;
	//c_printf("%s %s\n", argv[0], argv1[
	switch ( argc ) {
		case 1:
			_fs_init();
			break;
		case 2:
			read_sector( atoi(argv[1]), buff );
			for ( i = 0; i < 512; ++i ) {
				c_printf("%c", buff[i]);
				buff[i] = 0;
			}
			c_printf("\n");
				   // (uint8_t*)(argv[2]), strlen(argv[2]) );
			break;
		case 3:
			disk_write( atoi(argv[1]), (uint8_t*)(argv[2]), strlen(argv[2]) );
			
			break;
		
	}
}
