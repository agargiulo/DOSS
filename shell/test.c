#include <disk.h>
#include <c_io.h>
#include <string.h>
#include <ofs.h>

uint8_t buff[512];

void run_test(int argc, char **argv) {
	int i;
	
	switch ( argc ) {
		// Two arguments: Read sector
		case 2:
			read_sector( atoi(argv[1]), buff );
			for ( i = 0; i < 512; ++i ) {
				c_printf("%c", buff[i]);
				buff[i] = 0;
			}
			c_printf("\n");
				   // (uint8_t*)(argv[2]), strlen(argv[2]) );
			break;
		// Three arguments: write sector
		case 3:
			disk_write( atoi(argv[1]), (uint8_t*)(argv[2]), strlen(argv[2]) );
			
			break;
		
	}
}
