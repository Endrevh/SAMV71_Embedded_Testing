#include <atmel_start.h>

#include "shift_can.h"

#define PC0 GPIO(GPIO_PORTC, 0)

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	init_can_module();
	
	gpio_set_pin_direction(PC0, GPIO_DIRECTION_OUT);

	/*
	uint8_t data0 = 42;
	shift_can_message_t message0;
	message0.id = 0x5;
	message0.data = &data0;
	message0.dataSize = 1;
	
	uint8_t data1 = 19;
	shift_can_message_t message1;
	message1.id = 0x7;
	message1.data = &data1;
	message1.dataSize = 1;
	*/
	
	//add_single_filter_module_0(0x7, 0);

	add_range_filter_module_1(0, 5, 19);
	shift_can_message_t message0;

	/* Replace with your application code */
	while (1) {
		//delay_ms(1000);
		for(int i = 0; i < 100; i++) {
			uint8_t data0 = i;
			message0.id = i;
			message0.data = &data0;
			message0.dataSize = 1;
			send_message_module_0(&message0);
			delay_ms(50);
		}
		//send_message_module_0(&message0);
		//send_message_module_1(&message1);
		//printf("Message sent \n");
	}
}


void message_received_CAN0(shift_can_message_t message) {
	uint8_t data = *message.data;
	printf("Data received: %d \n", data);
} 

void message_received_CAN1(shift_can_message_t message) {
	uint8_t data = *message.data;
	printf("Data received: %d \n", data);
	gpio_toggle_pin_level(PC0);
}

/*void message_received(shift_can_message_t message) {
	uint8_t data = *message.data;
	printf("Data received: %d \n", data);
}*/