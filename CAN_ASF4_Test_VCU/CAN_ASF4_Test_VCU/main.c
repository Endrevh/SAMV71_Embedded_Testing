#include <atmel_start.h>

#include "shift_can.h"

#define PC0 GPIO(GPIO_PORTC, 0)
#define PC1 GPIO(GPIO_PORTC, 1)
#define PC2 GPIO(GPIO_PORTC, 2)
#define PC3 GPIO(GPIO_PORTC, 3)
#define PC4 GPIO(GPIO_PORTC, 4)
#define PC5 GPIO(GPIO_PORTC, 5)
#define PC6 GPIO(GPIO_PORTC, 6)
#define PC7 GPIO(GPIO_PORTC, 7)
#define PC8 GPIO(GPIO_PORTC, 8)
#define PC9 GPIO(GPIO_PORTC, 9)
#define PC10 GPIO(GPIO_PORTC, 10)
#define PC11 GPIO(GPIO_PORTC, 11)




int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	init_can_module();
	
	gpio_set_pin_direction(PC0, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC1, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC2, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC3, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC4, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC5, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC6, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC7, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC8, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC9, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC10, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(PC11, GPIO_DIRECTION_OUT);
	
	for(int i = 0; i < 12; i++)
	{
		gpio_set_pin_level(GPIO(GPIO_PORTC, i), true);

	}


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
	
	
	//add_single_filter_module_0(10, 1);

	add_range_filter_module_0(0, 21, 21);
	shift_can_message_t message0;

	/* Replace with your application code */
	while (1) {
		/*delay_ms(1000);
		for(int i = 1; i < 13; i++) {
			uint8_t data0 = 8;
			message0.id = i; 
			message0.data = &data0;
			message0.dataSize = 1;
			send_message_module_1(&message0);
			delay_ms(50);
		}
		//send_message_module_0(&message0);
		//send_message_module_1(&message1);
		//printf("Message sent \n");
	}
}


void message_received_CAN0(shift_can_message_t message) {
	uint8_t id = message.id;
	//printf("Data received: %d \n", data);

	//gpio_toggle_pin_level(PC11);
	switch (id)
	{
		case 1:
			gpio_toggle_pin_level(PC0);
			break;
		case 2:
			gpio_toggle_pin_level(PC1);
			break;
		case 3:
			gpio_toggle_pin_level(PC2);
			break;
		case 4:
			gpio_toggle_pin_level(PC3);
			break;
		case 5:
			gpio_toggle_pin_level(PC4);
			break;
		case 6:
			gpio_toggle_pin_level(PC5);
			break;
		case 7:
			gpio_toggle_pin_level(PC6);
			break;
		case 8:
			gpio_toggle_pin_level(PC7);
			break;
		case 9:
			gpio_toggle_pin_level(PC8);
			break;	
		case 10:				
			gpio_toggle_pin_level(PC9);
			break;
		case 11:
			gpio_toggle_pin_level(PC10);
			break;
		case 12:
			gpio_toggle_pin_level(PC11);
			break;
		default:
		/* Your code here */
		break;
	};
}

void message_received_CAN1(shift_can_message_t message) {
	uint8_t data = *message.data;
	printf("Data received: %d \n", data);
	gpio_toggle_pin_level(PC11);

}

/*void message_received(shift_can_message_t message) {
	uint8_t data = *message.data;
	printf("Data received: %d \n", data);
}*/