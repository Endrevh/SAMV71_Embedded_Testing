#include <atmel_start.h>
#include "shift_can.h"

#define PC9 GPIO(GPIO_PORTC, 9)

#define DEVKIT_LED_0 PC9

#define DEBUG_LED_0 DEVKIT_LED_0


bool sender = false;

void timer_setup();
static void TIMER_0_task1_callback(const struct timer_task *const timer_task);
static void TIMER_0_task2_callback(const struct timer_task *const timer_task);

static struct timer_task TIMER_0_task1, TIMER_0_task2;



int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	timer_setup();
	gpio_set_pin_direction(PC9, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PC9, GPIO_PIN_FUNCTION_OFF);
	
	shift_can_message_t msg;
	msg.id = 0x45A;
	msg.dataSize = 1;
	msg.data = 12345;
	
	init_can_module();
	add_single_filter_module_1(0x45A, 0);
	/* Replace with your application code */
	while (1) {
		if(sender) {
			delay_ms(1000);
			send_message_module_1(&msg);
			printf("Message sent \n");
		}
		
	}
}

void message_received(shift_can_message_t message) {
	 uint32_t data = message.data;
	 
	 printf("Received data: %d \n", data);
}

void timer_setup() {
	TIMER_0_task1.interval = 1000;
	TIMER_0_task1.cb       = TIMER_0_task1_callback;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 200;
	TIMER_0_task2.cb       = TIMER_0_task2_callback;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
}

static void TIMER_0_task1_callback(const struct timer_task *const timer_task)
{
	//printf("Timer value is %d \n", TIMER_0.time);
	/*shift_can_message_t message;
	if(sender) {
		message.id = 5;
		message.dataSize = 1;
		message.data = 12345;
		send_message_module_0(&message);
			printf("Message sent \n");

	}
	*/
}

static void TIMER_0_task2_callback(const struct timer_task *const timer_task)
{
}

