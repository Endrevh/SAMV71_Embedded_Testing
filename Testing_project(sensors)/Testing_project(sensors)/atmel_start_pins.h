/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMV71 has 4 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3

#define PB2 GPIO(GPIO_PORTB, 2)
#define PB3 GPIO(GPIO_PORTB, 3)
#define PC12 GPIO(GPIO_PORTC, 12)
#define PC14 GPIO(GPIO_PORTC, 14)
#define SILENT0 GPIO(GPIO_PORTE, 1)
#define SILENT1 GPIO(GPIO_PORTE, 2)

#endif // ATMEL_START_PINS_H_INCLUDED
