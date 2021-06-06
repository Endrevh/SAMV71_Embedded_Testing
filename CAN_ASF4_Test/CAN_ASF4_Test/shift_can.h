/*!
 * @file shift_can.h
 *
 * @brief Header file for the main functionality of CAN FD implementation.
 *
 * This header includes functions and structs that simplify the implementation
 * of CAN-FD on Atmel V71 chips. Its main purpose is to ensure CAN-FD communication
 * can be achieved using only a few basic functions.
 *
 * @date 24.10.2019 18:33:30
 * @author Shift Hyperloop Embedded
 */ 

#ifndef SHIFT_CAN_H_
#define SHIFT_CAN_H_

#include "hal_can_async.h"
#include "hpl_mcan.h"
#include "hpl_pmc.h"
#include <string.h>

/* !\brief Using standard identifier, each address consists of 11 bits,
 * from 0x000 to 0x7FF in hexadecimal, 
 * meaning 2^11 different values*/


/* Example, not final 
 * 256 values per ECU */

#define ID_BASE 0x000
#define VCU_OFFSET 0x000
#define INV_OFFSET 0x100
#define SAFETY_OFFSET 0x200
#define TEL_OFFSET 0x300
#define SENSOR_OFFSET 0x400

/* Then define full database */

/* Example, not final */

#define VCU_ERROR_FATAL 0x000
#define VCU_SHUT_OFF_PLS 0x001

#define SCC_SDC_KILL 0x400
#define SCC_BDR_PLUS_STATUS 0x401
#define SCC_BDR_MINUS_STATUS 0x402 
#define SCC_HV_SENSOR_DATA 0x403
#define SCC_IM_DEAD 0x404
#define SCC_BMS_STATUS 0x405
#define SCC_STATE 0x406


#define INV_ERROR_FATAL 0x100

#define SENSOR_ACCELLEROMETER_ORIENTATION 0x40C

/* etc... */

/* Example of typedefs for datatypes */

typedef struct
{
	char msg[8];
} error_struct_t;

typedef struct
{
	uint32_t pitch;
	uint32_t yaw;
	uint32_t roll;
} orientation_struct_t;

typedef struct  
{
	bool status;
} bool_struct_t;

/* Variables we are going to need */

/* @brief The main shift_can message struct 
 *
 * This structs contains all the neccessary information for
 * a can message, and keeps the id and data in one place.
 */
typedef struct shift_can_message
{
	uint32_t id; /**< The id for the message */
	uint8_t* data; /**< The data for the message */
	uint8_t dataSize; /**< The size of the data, should be 8/12/16/20/24/32/48/64 */
} shift_can_message_t;

/* Methods we are going to need */

/**
 * @brief Initializes both mcan modules
 *
 * This function configures both mcan modules to enable
 * FD mode, set the correct baudrate and enable interrupts.
 *
 */
void init_can_module(void);

/**
 * @brief Sends message on CAN bus using module 0 
 * 
 * This function deconstructs the message
 * and sends it over the CAN bus using module 0.
 *
 * @note The data needs to be serialized before
 * it can be sent
 * @see serialize_data()
 * @see shift_can_message  Struct for CAN messages*/
void send_message_module_0(shift_can_message_t* message); 

/**
 * @brief Sends message on CAN bus using module 1
 *
 * This function deconstructs the message
 * and sends it over the CAN bus using module 1.
 *
 * @note The data needs to be serialized before
 * it can be sent
 * @see serialize_data()
 * @see shift_can_message  Struct for CAN messages*/
void send_message_module_1(shift_can_message_t* message); 

/**
 * @brief Add single filter to mcan module 0
 *
 * This function adds a single ID filter to 
 * module 0 at a given index.
 * @param index needs to be 0-63
 */
void add_single_filter_module_0(uint32_t id, uint8_t index); 

/**
 * @brief Add single filter to mcan module 1
 *
 * This function adds a single ID filter to
 * module 0 at a given index.
 * @param index needs to be 0-63
 */
void add_single_filter_module_1(uint32_t id, uint8_t index);

/**
 * @brief Add range filter to mcan module 0
 *
 * This function adds a ranged ID filter to
 * module 0 at a given index.
 * @param index needs to be 0-63
 */
void add_range_filter_module_0(uint8_t index, uint32_t id_start, uint32_t id_end);

/**
 * @brief Add range filter to mcan module 1
 *
 * This function adds a ranged ID filter to
 * module 1 at a given index.
 * @param index needs to be 0-63
 */
void add_range_filter_module_1(uint8_t index, uint32_t id_start, uint32_t id_end);


/**
 * @brief Called by interrupt handler when message is received 
 *
 * This function is called whenever a module receives a message.
 * You can retrieve the original data structure by casting
 * like this:
 * @code
 * struct_type_t* the_struct = (struct_type_t*) message.data;
 * @endcode
 */
void message_received(shift_can_message_t message); 


void CAN_RX_CALLBACK(struct can_async_descriptor *const descr);
void ERROR_CALLBACK(struct can_async_descriptor *const descr);

/** 
 * @brief Serialize struct into buffer to send over CAN bus.
 * 
 * This function takes in a struct and serializes it to a
 * byte array that can be sent over the CAN bus.
 * @return @c A pointer to a uint8_t array.
 */
void serialize_data(void* data_struct, uint8_t data_size, uint8_t* buffer);

/*! \brief Helper function to get correct DLC value */
uint32_t dlc_to_data_size(uint32_t dlc);

/*! \brief Helper function to get correct data size value from DLC */
uint32_t data_size_to_dlc(uint32_t data_size);

//void mcan_set_baudrate(uint32_t baudrate);

/* Defines we are going to need that should be settable for each ECU */

#define ID_BUFFER_INDEX	2
#define TRANSFER_BUFFER_INDEX	0
#define BAUDRATE 1000000

#define FILTER_TYPE_RANGE 0
#define FILTER_TYPE_DUAL 1
#define FILTER_TYPE_SINGLE 2

#define LOG_ERROR(msg) printf("SHIFT_CAN_ERROR: %s\n", msg)


/* The two mcan module structures, 
 * containing reference to hardware. 
 */

#define module_0 CAN_0
#define module_1 CAN_1

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
	can_async_register_callback(&module_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_RX_CALLBACK);
	can_async_register_callback(&module_1, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_RX_CALLBACK);
	
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

void CAN_RX_CALLBACK(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	
	shift_can_message_t message;
	message.data = msg.data;
	message.dataSize = msg.len;
	message.id = msg.id;
	message_received(message);
}

void ERROR_CALLBACK(struct can_async_descriptor *const descr)
{
	init_can_module();
	printf("ERROR");
}


#endif /* SHIFT_CAN_H_ */