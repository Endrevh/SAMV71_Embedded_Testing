#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	/* Replace with your application code */
	while (1) {
		delay_ms(1);
		SPI_0_example();
	}
}
