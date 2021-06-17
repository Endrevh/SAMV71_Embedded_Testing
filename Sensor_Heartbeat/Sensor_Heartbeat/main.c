#include <atmel_start.h>
#include "shift_can.h"

#define LED_A GPIO(GPIO_PORTA, 26)

static struct timer_task TIMER_0_sensor_heartbeat;

void sensor_heartbeat();
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	init_can_module();
	
	//setup();
	gpio_set_pin_direction(LED_A, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(LED_A, true);
	
	//add_single_filter_module_0(21, 0);
	add_single_filter_module_0(111, 1);


	/* Replace with your application code */
	while (1) {
	}
}

void setup()
{
	gpio_set_pin_direction(LED_A, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(LED_A, true);
	TIMER_0_sensor_heartbeat.interval = 20;
	TIMER_0_sensor_heartbeat.mode = TIMER_TASK_REPEAT;
	TIMER_0_sensor_heartbeat.cb = sensor_heartbeat;
	
	timer_add_task(&TIMER_0, &TIMER_0_sensor_heartbeat);
	timer_start(&TIMER_0);
}

void message_received_CAN0(shift_can_message_t message) 
{
	uint32_t id = message.id;
	
	uint32_t data = *message.data;
	
	if(id == 21)
	{
		timer_cb_t cb_temp = TIMER_0_sensor_heartbeat.cb;
		
		timer_remove_task(&TIMER_0, &TIMER_0_sensor_heartbeat);
		
		TIMER_0_sensor_heartbeat.interval = data;
		TIMER_0_sensor_heartbeat.mode = TIMER_TASK_REPEAT;
		TIMER_0_sensor_heartbeat.cb = cb_temp;
		
		timer_add_task(&TIMER_0, &TIMER_0_sensor_heartbeat);
	}
	
	if(id == 111)
	{
		uint32_t dummy = 123;
		shift_can_message_t msg;
		msg.id = 111;
		msg.data = &dummy;
		msg.dataSize = 1;
		
		send_message_module_0(&msg);
		gpio_toggle_pin_level(LED_A);
	}
}

void message_received_CAN1(shift_can_message_t message) {}

void sensor_heartbeat()
{
	uint32_t dummydata = 1;

	shift_can_message_t message;
	message.id = 0x29D;
	message.data = &dummydata;
	message.dataSize = 1;
	
	send_message_module_0(&message);
}