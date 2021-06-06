#include <atmel_start.h>

void SPI_Test();
static void SPI_callback_test();
void TIMER_0_Test();
static void TIMER_0_task1_cb(const struct timer_task *const timer_task);
static void TIMER_0_task2_cb(const struct timer_task *const timer_task);


static struct timer_task TIMER_0_task1, TIMER_0_task2;


#define PC0 GPIO(GPIO_PORTC, 0)
#define PC1 GPIO(GPIO_PORTC, 1)



int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();	
	
	//spi_m_async_set_baudrate(&SPI_0, 80000);
	TIMER_0_Test();
	gpio_set_pin_direction(PC0, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC1, GPIO_DIRECTION_OUT);

	//gpio_set_pin_direction(PD3, GPIO_DIRECTION_OUT); //cs
	gpio_set_pin_direction(PD25, GPIO_DIRECTION_OUT); //cs on spi header
	gpio_set_pin_direction(PD12, GPIO_DIRECTION_OUT); //cs on spi header

	
	//pio_set_pin_level(PD3, true); //initial cs high
	gpio_set_pin_level(PD25, true); //initial cs on spi header high
	gpio_set_pin_level(PD12, true); //initial cs on spi header high

	gpio_set_pin_level(PC0, true); //LED initial off
	gpio_set_pin_level(PC1, true); //LED initial off

	
	/* Replace with your application code */
	while (1) {
		//delay_ms(500);
		//gpio_toggle_pin_level(PC0);

		//SPI_Test();
	}
	
	
}

void SPI_Test() {
	struct io_descriptor *io;
	spi_m_async_get_io_descriptor(&SPI_0, &io);
	
	spi_m_async_set_data_order(&SPI_0, SPI_DATA_ORDER_MSB_1ST);
	spi_m_async_set_char_size(&SPI_0, SPI_CHAR_SIZE_8);
	spi_m_async_set_baudrate(&SPI_0, 589000);

	
	uint8_t message = 42;

	spi_m_async_register_callback(&SPI_0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)SPI_callback_test);
	spi_m_async_enable(&SPI_0);
	
	//gpio_set_pin_level(PD3, false); //set cs low. Needs to be done manually yes.
	gpio_set_pin_level(PD25, false);
	gpio_set_pin_level(PD12, false);

	
	io_write(io, &message, 1);

}

static void SPI_callback_test() {
	//printf("Time: %d \n", TIMER_0.time);
	//gpio_set_pin_level(PD3, true); //set cs high. Needs to be done manually yes.
	gpio_set_pin_level(PD25, true); //set cs on spi header high
	gpio_set_pin_level(PD12, true); //set cs on spi header high

	
	gpio_toggle_pin_level(PC0);
	
	uint8_t receive = SPI0->SPI_RDR;
	if(receive == 42) {
		gpio_toggle_pin_level(PC1);
	}
}

static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
	SPI_Test();	
}

static void TIMER_0_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_0_Test(void)
{
	TIMER_0_task1.interval = 100;
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 2000;
	TIMER_0_task2.cb       = TIMER_0_task2_cb;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
}
