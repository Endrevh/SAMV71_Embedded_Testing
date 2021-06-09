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
#include "driver_init.h"
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

/*Error-code enumerator*/
typedef enum 
{
	NONE,
	SENSORS_SUITE1_MISSING_HEARTBEAT,
	SENSORS_SUITE2_MISSING_HEARTBEAT,
	STATE_INDICATION_MISSING_HEARTBEAT,
	INVERTER_MISSING_HEARTBEAT,
	TELEMETRY_MISSING_HEARTBEAT,
	BMS1_MISSING_HEARTBEAT,
	BMS2_MISSING_HEARTBEAT
}SHIFT_ERROR_CODE;

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

/*typecast SHIFT_ERROR_CODE to uint8_t and send as an array. Use the error "NONE" on indexes not used.*/
typedef struct  
{
	uint8_t errors[8];
} shift_error_struct_t;

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
void message_received_CAN0(shift_can_message_t message); 
void message_received_CAN1(shift_can_message_t message);



void CAN0_RX_CALLBACK(struct can_async_descriptor *const descr);
void CAN1_RX_CALLBACK(struct can_async_descriptor *const descr);
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

#ifdef _STDIO_IO_H_INCLUDED
#define LOG_ERROR(msg) printf("SHIFT_CAN_ERROR: %s\n", msg)
#else
#define LOG_ERROR(msg) 
#endif


/* The two mcan module structures, 
 * containing reference to hardware. 
 */

#define module_0 CAN_0
#define module_1 CAN_1






#endif /* SHIFT_CAN_H_ */
