#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1) {
		delay_ms(1000);
		SPI_Test();
	}
}

static void SPI_callback_test() {
	printf("Message sent \n");
}


void SPI_Test() {
	struct io_descriptor *io;
	spi_m_async_get_io_descriptor(&SPI_0, &io);
	
	uint8_t message = 42;

	spi_m_async_register_callback(&SPI_0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)SPI_callback_test);
	spi_m_async_enable(&SPI_0);
	io_write(io, &message, 1);
}
