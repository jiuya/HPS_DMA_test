#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hps_0.h"
#include "led.h"
#include "seg7.h"
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

volatile unsigned long *h2p_lw_led_addr=NULL;
volatile unsigned long *h2p_lw_hex_addr=NULL;
volatile unsigned long *h2p_lw_7seg_addr=NULL;
volatile unsigned long *h2p_lw_ram_addr=NULL;
volatile unsigned long *h2p_lw_sw_addr=NULL;
void led_blink(void)
{
	int i=0;
	while(1){
	printf("LED ON \r\n");
	for(i=0;i<=10;i++){
			LEDR_LightCount(i);
			usleep(100*1000);
		}
	printf("LED OFF \r\n");
	for(i=0;i<=10;i++){
			LEDR_OffCount(i);
			usleep(100*1000);
		}
	}
}

int main(int argc, char **argv)
{
	time_t timer;
	struct tm *date;
	int outData;
	int i;
	pthread_t id;
	int ret;
	void *virtual_base;
	int fd;

	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );
	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return(1);
	}
	/*
	h2p_lw_led_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + LED_PIO_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_hex_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + SEG7_IF_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	ret=pthread_create(&id,NULL,(void *)led_blink,NULL);
	if(ret!=0){
		printf("Creat pthread error!\n");
		exit(1);
	}
	*/
	h2p_lw_7seg_addr = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + 0x0   ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_ram_addr  = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + 0x400 ) & ( unsigned long)( HW_REGS_MASK ) );
	h2p_lw_sw_addr   = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + 0x100c0 ) & ( unsigned long)( HW_REGS_MASK ) );
	while(1)
	{
		/*
		timer = time(NULL);
		date = localtime(&timer);
		outData =
			(((date->tm_hour / 10) << 20)&0xf00000) +
			(((date->tm_hour % 10) << 16)&0xf0000) +
			(((date->tm_min / 10) << 12)&0xf000) +
			(((date->tm_min % 10) <<  8)&0xf00) +
			((date->tm_sec / 10) << 4) +
			(date->tm_sec % 10);
		//SEG7_All_Number();
		alt_write_word(h2p_lw_7seg_addr,outData);
		*/
		/*
		for(i = 0;i < 8;i++){
			printf("0x%x,",alt_read_word(h2p_lw_ram_addr + i));
		}
		*/
		printf("0x%x",alt_read_word(h2p_lw_sw_addr));
		printf("\n");

	}

	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );

	}
	close( fd );
	return 0;
}
