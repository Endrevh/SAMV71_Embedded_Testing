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


/*Message IDs*/

/*BMS1*/
/*High-priority*/
#define ID_VOLTAGES_P1_0_14 0x324
#define ID_VOLTAGES_P1_15_29 0x325
#define ID_VOLTAGES_P1_30_44 0x326
#define ID_VOLTAGES_P1_45_59 0x327
#define ID_VOLTAGES_P1_60_74 0x328
#define ID_VOLTAGES_P1_75_89 0x329
#define ID_TEMP_P1_0_11 0x32A
#define ID_TEMP_P1_12_23 0x32B
#define ID_TEMP_P1_24_35 0x32C
#define ID_TEMP_P1_36_44 0x32D
#define ID_HV_BATTERY_CURRENT_P1 0x32E
#define ID_BMS_SAFETY_P1 0x22F
#define ID_BMS1_HEARTBEAT 0x130

/*BMS2*/
/*High-priority*/
#define ID_VOLTAGES_P2_0_14 0x338
#define ID_VOLTAGES_P2_15_29 0x339
#define ID_VOLTAGES_P2_30_44 0x33A
#define ID_VOLTAGES_P2_45_59 0x33B
#define ID_VOLTAGES_P2_60_74 0x33C
#define ID_VOLTAGES_P2_75_89 0x33D
#define ID_TEMP_P2_0_11 0x33E
#define ID_TEMP_P2_12_23 0x33F
#define ID_TEMP_P2_24_35 0x340
#define ID_TEMP_P2_36_44 0x341
#define ID_HV_BATTERY_CURRENT_P2 0x342
#define ID_BMS_SAFETY_P2 0x243
#define ID_BMS2_HEARTBEAT 0x144

/*Inverter1*/
/*High-priority*/
//#define ID_LEVEL_SHIFTERS_DISABLED 0x174 //removed because we have a separate error enum and message
#define ID_INVERTER_CURRENT_MEASUREMENTS 0x475
#define ID_DC_LINK_VOLTAGE_MEASUREMENT 0x776
#define ID_INVERTER1_HEARTBEAT 0x277

/*Inverter2*/
/*High-priority*/
#define ID_INVERTER2_HEARTBEAT 0x288

/*Sensor suite 1*/
/*High-priority*/
#define ID_TEST_STRUCT 0x79C
#define ID_SENSOR_SUITE1_HEARTBEAT 0x2A7
/*Low-priority*/
#define ID_TEMPERATURE_INV_BRAKE_LIM 0x19C
#define ID_SUITE_XS_FRONT_TEMP_PRES 0x5A0
#define ID_SUITE_XS_FRONT_POS 0x5A1

/*Sensor suite 2*/
/*High-priority*/
#define ID_SENSOR_SUITE2_HEARTBEAT 0x2B2
/*Low-priority*/
#define ID_IMU_PSA 0x1B0 //Position, speed and acceleration
#define ID_IMU_YPR 0x1B1 //Yaw, pitch, roll
#define ID_PE_SPEED_POS_ESTIMATE 0x1B2 //Speed and position estimate from Photoelectric sensors and track markers
#define ID_SUITE_XS_REAR_TEMP_PRES 0x5B3 //Temperature and pressure from Pod's rear quadrants
#define ID_SUITE_XS_REAR_POS 0x5B4 //X, Y and Z position from Pod's rear quadrants, with Origin on the PCB
#define ID_DIST_TEMP_PRES 0x1B5 //Distance to beam rear, Pressure in brake fluid front & rear, temp brake pad and inverter rear

/*VCU*/
/*High-priority*/
#define ID_VCU_HEARTBEAT 0x2CD
#define	ID_CHANGE_POD_STATE 0x0CE
#define ID_ERROR_CODE_MESSAGE 0x1CF
#define ID_START_SENSOR_SUITE1_HEARTBEAT 0x7D0
#define ID_START_SENSOR_SUITE2_HEARTBEAT 0x7D1
#define ID_START_BMS1_HEARTBEAT 0x7D2
#define ID_START_BMS2_HEARTBEAT 0x7D3
#define ID_START_INVERTER1_HEARTBEAT 0x7D4
#define ID_START_INVERTER2_HEARTBEAT 0x7D5
#define ID_START_TELEMETRY_HEARTBEAT 0x7D6
/*Low-priority*/
#define ID_SYSTEM_STATUS_REPORT 0x5C4
#define ID_SET_HEARTBEAT_INTERVAL 0x2C5 //be careful with changing interval during pod-run. All systems will ping at the same time after interval-change
#define ID_INVERTER_SPEED_SETPOINT 0x0C6 

/*Telemetry*/
/*High-priority*/
#define ID_TELEMTRY_HEARTBEAT 0x2D8



/* Example, not final */

/* etc... */

/*Error-code enumerator. 
Note: Error 0 is no error, 1-126 are critical errors, 127 is an unused separator and 128-255 are noncritical errors.
Fill in when new potential errors are found. If you don't know if it is critical or not, put it in critical */
#define FOREACH_CRITICAL_ERROR(ERROR)			\
	ERROR(NO_ERROR)								\
	ERROR(VCU_MISSING_HEARTBEAT)				\
	ERROR(SENSORS_SUITE1_MISSING_HEARTBEAT)		\
	ERROR(SENSORS_SUITE2_MISSING_HEARTBEAT)		\
	ERROR(STATE_INDICATION_MISSING_HEARTBEAT)	\
	ERROR(INVERTER1_MISSING_HEARTBEAT)			\
	ERROR(INVERTER2_MISSING_HEARTBEAT)			\
	ERROR(TELEMETRY_MISSING_HEARTBEAT)			\
	ERROR(BMS1_MISSING_HEARTBEAT)				\
	ERROR(BMS2_MISSING_HEARTBEAT)				\
	ERROR(STATE_INDICATION_WRONG_BITMASK)		\
	ERROR(SENSORS_SUITE1_WRONG_STATE)			\
	ERROR(SENSORS_SUITE2_WRONG_STATE)			\
	ERROR(STATE_INDICATION_WRONG_STATE)			\
	ERROR(INVERTER1_WRONG_STATE)				\
	ERROR(INVERTER2_WRONG_STATE)				\
	ERROR(TELEMETRY_WRONG_STATE)				\
	ERROR(BMS1_WRONG_STATE)						\
	ERROR(BMS2_WRONG_STATE)						\
	ERROR(LEVEL_SHIFTERS_DISABLED)				\
	ERROR(BMS1_CELL_SAFETY_ERROR)				\
	ERROR(BMS2_CELL_SAFETY_ERROR)				\
	ERROR(SENSOR_SUITE2_POS_ERROR)				\
	
#define FOREACH_NONCRITICAL_ERROR(ERROR)		\
	ERROR(TEST123) 								\

#define ERROR_ENUM_SEPARATOR_VALUE 127
#define FOREACH_ERROR_SEPARATOR(SEPARATOR) \
	SEPARATOR(ERROR_SEPARATOR, ERROR_ENUM_SEPARATOR_VALUE) \
	
#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_SEPARATOR(ENUM, X) ENUM = X,
#define GENERATE_STRING_SEPARATOR(STRING, X) [X] = #STRING,
#define GENERATE_STRING(STRING) #STRING,

typedef enum {
	FOREACH_CRITICAL_ERROR(GENERATE_ENUM)
	FOREACH_ERROR_SEPARATOR(GENERATE_SEPARATOR)
	FOREACH_NONCRITICAL_ERROR(GENERATE_ENUM)
}shift_error_code;

static const char* ERROR_STRING[] = {
	FOREACH_CRITICAL_ERROR(GENERATE_STRING)
	FOREACH_ERROR_SEPARATOR(GENERATE_STRING_SEPARATOR)
	FOREACH_NONCRITICAL_ERROR(GENERATE_STRING)	
};

/*Pod-state enumerator*/
typedef enum {
	INITIALIZING_LV,
	INITIALIZED_LV,
	PRECHARGE,
	READY,
	RUNNING,
	BRAKING,
	DISCHARGE,
	RUN_COMPLETE,
	CRITICAL_ERROR
}pod_state;

/* Example of typedefs for datatypes */

/*typedef struct
{
	char msg[8];
} error_struct_t;*/

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
	uint8_t error0;
	uint8_t error1;
	uint8_t error2;
	uint8_t error3;
	uint8_t error4;
	uint8_t error5;
	uint8_t error6;
	uint8_t error7;
} shift_error_struct_t;

//union for easier error accessing
typedef union {
	shift_error_struct_t error_struct;
	uint8_t error_array[8];
} error_union_t;

typedef struct {
	uint8_t state;
}pod_state_struct_t;

typedef struct
{
	bool Sensor_suit_1;
	bool Sensor_suit_2;
	bool State_indication;
	bool Inverter_1;
	bool Inverter_2;
	bool Telemetry;
	bool BMS_1;
	bool BMS_2;
	bool VCU;
	
	uint32_t latency_CAN_0;
	uint32_t latency_CAN_1;
} VCU_status_check_t;

typedef struct
{
	bool status_0;
	bool status_1;
	bool status_2;
	bool status_3;
	bool status_4;
	
	double value_0;
	double value_1;
} sensor_alive_report_t;

// Message from sensors, that sends speed in m/s
typedef struct
{
	float position;				// m
	float speed;				// m/s
	float acceleration;			// m/s^2
} vector_3f_psa_t;

//integer structs
typedef struct
{
	uint8_t value;
} uint8_struct_t;

typedef struct
{
	uint16_t value;
} uint16_struct_t;

// float structs
typedef struct
{
	float value;
} float_struct_t;

typedef struct
{
	float value_0;
	float value_1;
} vector_2f_t;

typedef struct
{
	float value_0;
	float value_1;
	float value_2;
} vector_3f_t;

typedef struct
{
	float value_0;
	float value_1;
	float value_2;
	float value_3;
} vector_4f_t;

typedef struct
{
	float value_0;
	float value_1;
	float value_2;
	float value_3;
	float value_4;
	float value_5;
} vector_6f_t;

typedef struct
{
	float value_0;
	float value_1;
	float value_2;
	float value_3;
	float value_4;
	float value_5;
	float value_6;
	float value_7;
} vector_8f_t;

typedef struct
{
	float value_0;
	float value_1;
	float value_2;
	float value_3;
	float value_4;
	float value_5;
	float value_6;
	float value_7;
	float value_8;
	float value_9;
	float value_10;
	float value_11;
	float value_12;
	float value_13;
	float value_14;
	float value_15;
} vector_16f_t;
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
 * These functions are called whenever a module receives a message.
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

/*! \brief Helper function to translate error mesasge to string
Buffer has to be big enough to store all generated errors */
void error_message_to_string(shift_error_struct_t error_struct, char* buffer);

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
