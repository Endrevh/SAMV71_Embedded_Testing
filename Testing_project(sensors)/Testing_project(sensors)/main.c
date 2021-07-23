#include <atmel_start.h>
#include "shift_can.h"

#define HEARTBEAT_INTERVAL 10
#define LED_A GPIO(GPIO_PORTC, 2)

struct timer_task TIMER_0_sensors_suite1_heartbeat;

pod_state current_state;

void sensors_suite1_heartbeat_callback(void);
void start_heartbeat(uint16_t heartbeat_interval);
void subsystem_timer_setup(void);
void subsystem_setup(void);


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	subsystem_setup();
	
	gpio_set_pin_direction(LED_A, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(LED_A, true);	

	/* Replace with your application code */
	while (1) {
	}
}

void sensors_suite1_heartbeat_callback(void)
{
	shift_can_message_t heartbeat_message;
	
	pod_state_struct_t data_struct;
	data_struct.state = (uint8_t)current_state; //note that current_state has to be a variable in your code
	
	uint8_t* buff;
	uint8_t datasize = dlc_to_data_size(sizeof(data_struct));
	serialize_data(&data_struct, datasize, buff);
	
	heartbeat_message.data = buff;
	heartbeat_message.id = ID_SENSOR_SUITE1_HEARTBEAT; //use correct subsystem ID. Example: ID_BMS2_HEARTBEAT (defined in shift_can.h)
	heartbeat_message.dataSize = datasize;
	
	gpio_toggle_pin_level(LED_A);
	
	send_message_module_0(&heartbeat_message); //sent on high-pri bus
}

void start_heartbeat(uint16_t heartbeat_interval)
{
	TIMER_0_sensors_suite1_heartbeat.interval = heartbeat_interval; //naming example: TIMER_0_telemetry_heartbeat
	TIMER_0_sensors_suite1_heartbeat.mode = TIMER_TASK_REPEAT;
	TIMER_0_sensors_suite1_heartbeat.cb = sensors_suite1_heartbeat_callback; //the name of the callback-function
	timer_add_task(&TIMER_0, &TIMER_0_sensors_suite1_heartbeat);
}

void subsystem_timer_setup()
{
	/*Some timer setup code here*/
	
	start_heartbeat(HEARTBEAT_INTERVAL);
	
	/*Some more timer setup code here*/
	
	timer_start(&TIMER_0); //NOTE: this is the last line in the function, as it should be called after all other timer-configuration is done
}

void subsystem_setup()
{
	/*Some setup-code here*/
	
	init_can_module();
	
	subsystem_timer_setup();
	current_state = INITIALIZING_LV;
	/*Some more setup-code here*/
}

void message_received_CAN0(shift_can_message_t message)
{
	uint32_t msg_id = message.id;
	pod_state_struct_t* pod_state_struct_ptr;

	switch (msg_id)
	{
		case ID_CHANGE_POD_STATE:
			pod_state_struct_ptr = (pod_state_struct_t*) message.data;
			current_state = pod_state_struct_ptr->state;
			break;
		default:
		/* Your code here */
		break;
	}
}


void message_received_CAN1(shift_can_message_t message)
{
	
}