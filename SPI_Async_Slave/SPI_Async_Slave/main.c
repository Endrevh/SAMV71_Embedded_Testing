#include <atmel_start.h>
#include "hal_spi_s_async.h"

void SPI0_test_callback();



int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	spi_s_async_register_callback(&SPI_0, SPI_S_CB_RX, (FUNC_PTR)SPI0_test_callback);
	spi_s_async_enable(&SPI_0);
	
	/* Replace with your application code */
	while (1) {
	}
}




void SPI0_test_callback() {
	uint8_t message;
	
	struct io_descriptor *io;
	spi_s_async_get_io_descriptor(&SPI_0, &io);
	
	//io_read(io, &message, 1);
	
	message = (uint8_t)SPI0->SPI_RDR;

		
	printf("Message received %d \n", message);
}

