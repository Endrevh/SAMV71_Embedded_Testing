#include <atmel_start.h>

#define PC0 GPIO(GPIO_PORTC, 0)
#define PC1 GPIO(GPIO_PORTC, 1)
#define PC2 GPIO(GPIO_PORTC, 2)

#define PD1 GPIO(GPIO_PORTD, 1)
#define PD2 GPIO(GPIO_PORTD, 2)


bool stop = false;
bool did = false;

static void TIMER_0_task1_callback(const struct timer_task *const timer_task);
static void TIMER_0_task2_callback(const struct timer_task *const timer_task);

struct timer_task TIMER_0_task1, TIMER_0_task2;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	gpio_set_pin_direction(PD1, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PD2, GPIO_DIRECTION_OUT);

	TIMER_0_task1.interval = 100;
	TIMER_0_task1.cb       = TIMER_0_task1_callback;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 600;
	TIMER_0_task2.cb       = TIMER_0_task2_callback;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
	
	for(int i = 0; i < 12; i++)
	{
		gpio_set_pin_direction(GPIO(GPIO_PORTC, i), GPIO_DIRECTION_OUT);
		gpio_set_pin_level(GPIO(GPIO_PORTC, i), true);
	}
	
	/* Replace with your application code */
	while (1) {
		for(int i = 0; i < 12; i++)
		{
			gpio_toggle_pin_level(GPIO(GPIO_PORTC, i));
			delay_ms(20);
		}

	}
}

static void TIMER_0_task1_callback(const struct timer_task *const timer_task)
{
	gpio_toggle_pin_level(PD1);
}

static void TIMER_0_task2_callback(const struct timer_task *const timer_task)
{
	gpio_toggle_pin_level(PD2);
}