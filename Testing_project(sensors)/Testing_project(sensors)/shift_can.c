/*
 * shift_can.c
 * This file contains implementations of prototypes in shift_can.h
 * Created: 05.06.2021 13:51:57
 *  Author: Shift Hyperloop Embedded
 */ 

#include "shift_can.h"

uint32_t data_size_to_dlc(uint32_t data_size)
{
	/* If 8 or below, same value */
	if (data_size <= 8) {
		return data_size;
		/* 0001100 -> 1001 */
		} else if (data_size <= 12) {
		return 0x9ul;
		/* 0010000 -> 1010 */
		} else if (data_size <= 16) {
		return 0xAul;
		/* 0010100 -> 1011 */
		} else if (data_size <= 20) {
		return 0xBul;
		/* 0011000 -> 1100 */
		} else if (data_size <= 24) {
		return 0xCul;
		/* 0100000 -> 1101 */
		} else if (data_size <= 32) {
		return 0xDul;
		/* 0110010 -> 1110 */
		} else if (data_size <= 48) {
		return 0xEul;
		/* 1000000 -> 1111 */
		} else if (data_size <= 64) {
		return 0xFul;
	}
	return 0;
}

uint32_t dlc_to_data_size(uint32_t dlc)
{
	/* Opposite of data_size_to_dlc() */
	switch(dlc)
	{
		case 0x9ul:
			return 12;
		case 0xAul:
			return 16;
		case 0xBul:
			return 20;
		case 0xCul:
			return 24;
		case 0xDul:
			return 32;
		case 0xEul:
			return 48;
		case 0xFul:
			return 64;
		default:
			return dlc;
	}
}

void configure_upll(void);

/* Change the mcan modules to be driven 
 * by the UPLL clock with a frequency 
 * of 480MHz, and 6 prescalar
 */
void configure_upll()
{
	
	/* TODO: Fin out how to switch PCK5 source to UPLL in Atmel Start
	pmc_disable_pck(PMC_PCK_5);
	pmc_switch_pck_to_upllck(PMC_PCK_5, PMC_PCK_PRES(6));
	pmc_enable_pck(PMC_PCK_5);
	pmc_enable_upll_clock();
	pmc_enable_periph_clk(ID_MCAN0);
	pmc_enable_periph_clk(ID_MCAN1);
	*/
}

void serialize_data(void* data_struct, uint8_t data_size, uint8_t* buffer)
{
	/*  Create a 64 byte array and copy contents of struct directly.
		This assumes endianness and bit stuffing is identical on all 
		connected devices. */
	memcpy(buffer, data_struct, data_size);
}

void init_can_module()
{		
	can_async_init(&module_0, MCAN0);
	can_async_init(&module_1, MCAN1);
	 
	/* Configure modules to use UPLL clock with 
	   480MHz, 6 prescaler meaning 80 MHz */
	//configure_upll();
	 	
 	/* Enable interrupts for  MCAN modules */
	can_async_register_callback(&module_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN0_RX_CALLBACK);
	can_async_register_callback(&module_1, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN1_RX_CALLBACK);
	
	can_async_register_callback(&module_0, CAN_ASYNC_IRQ_CB, (FUNC_PTR)ERROR_CALLBACK);
	can_async_register_callback(&module_1, CAN_ASYNC_IRQ_CB, (FUNC_PTR)ERROR_CALLBACK);
	
	can_async_enable(&module_0);
	can_async_enable(&module_1);
}

int checkMessage(shift_can_message_t* message)
{	
	/* Get message struct information */
	uint32_t id = message->id;
	uint8_t* data = message->data;
	uint8_t dataSize = message->dataSize;
	
	if(id && data && dataSize && dataSize <= 64 && id < 0x7FF)
	{
		return 1;
	}
	else
	{
		if(!data) LOG_ERROR("Struct is missing data!");
		if(!dataSize) LOG_ERROR("Struct is missing dataSize!");
		if(dataSize > 64) LOG_ERROR("Data size is too large!");
		if(id > 0x7FF) LOG_ERROR("ID is too large!");
	}
	return 0;
}

void send_message_module_0(shift_can_message_t* message)
{
	if (checkMessage(message))
	{
		struct can_message msg;

		msg.id = message->id;
		msg.type = CAN_TYPE_DATA;
		msg.data = message->data;
		msg.len = message->dataSize;
		msg.fmt  = CAN_FMT_STDID;
		
		can_async_write(&module_0, &msg);
	}
}

void send_message_module_1(shift_can_message_t* message)
{	
	if (checkMessage(message))
	{
		struct can_message msg;

		msg.id = message->id;
		msg.type = CAN_TYPE_DATA;
		msg.data = message->data;
		msg.len = message->dataSize;
		msg.fmt  = CAN_FMT_STDID;
		
		can_async_write(&module_1, &msg);
	}
}

int checkIndex(uint8_t index)
{
	if (index < 64)
	{
		return 1;
	}
	else
	{
		LOG_ERROR("Index needs to be between 0-63!");
	}
	return 0;
}

void add_single_filter_module_0(uint32_t id, uint8_t index)
{
	if(checkIndex(index))
	{
		struct can_filter filter;
		filter.id = id;
		filter.mask = 0;
		can_async_set_filter(&module_0, index, CAN_FMT_STDID, &filter);
	}

}
void add_single_filter_module_1(uint32_t id, uint8_t index)
{
	if(checkIndex(index))
	{
		struct can_filter filter;
		filter.id = id;
		filter.mask = 0;
		can_async_set_filter(&module_1, index, CAN_FMT_STDID, &filter);
	}
}

void add_range_filter_module_0(uint8_t index, uint32_t id_start, uint32_t id_end)
{
	if(checkIndex(index)) 
	{
		struct _can_standard_message_filter_element *sf;

		sf = &((struct _can_context *)module_0.dev.context)->rx_std_filter[index];

		sf->S0.val = 0;
		sf->S0.bit.SFID1 = id_start;
		sf->S0.bit.SFID2 = id_end;
		sf->S0.bit.SFT = _CAN_SFT_RANGE;
		sf->S0.bit.SFEC = _CAN_SFEC_STF0M;
	}
}

void add_range_filter_module_1(uint8_t index, uint32_t id_start, uint32_t id_end)
{
	if(checkIndex(index))
	{
		struct _can_standard_message_filter_element *sf;

		sf = &((struct _can_context *)module_1.dev.context)->rx_std_filter[index];

		sf->S0.val = 0;
		sf->S0.bit.SFID1 = id_start;
		sf->S0.bit.SFID2 = id_end;
		sf->S0.bit.SFT   = _CAN_SFT_RANGE;
		sf->S0.bit.SFEC  = _CAN_SFEC_STF0M;
	}
}

void CAN0_RX_CALLBACK(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	
	shift_can_message_t message;
	message.data = msg.data;
	message.dataSize = msg.len;
	message.id = msg.id;
	message_received_CAN0(message);
}

void CAN1_RX_CALLBACK(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	
	shift_can_message_t message;
	message.data = msg.data;
	message.dataSize = msg.len;
	message.id = msg.id;
	message_received_CAN1(message);
}

void ERROR_CALLBACK(struct can_async_descriptor *const descr)
{
	init_can_module();
	LOG_ERROR("BUS ERROR");
}

void error_message_to_string(shift_error_struct_t error_struct, char* buffer)
{
	shift_error_code error = NO_ERROR;
	
	error_union_t error_union;
	error_union.error_struct = error_struct;
	for(int i = 0; i < 8; i++)
	{	
		error = (shift_error_code)error_union.error_array[i];
	
		if(error == NO_ERROR) continue;
		strcat(buffer, ERROR_STRING[error]);
		strcat(buffer, "\n");
	}
	strcat(buffer, "\0"); //terminate string
}
