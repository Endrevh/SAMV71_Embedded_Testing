/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_spi_base.h>
#include <hpl_usart_base.h>

struct spi_m_async_descriptor SPI_0;

struct usart_sync_descriptor TARGET_IO;

void SPI_0_PORT_init(void)
{

	gpio_set_pin_function(PD20, MUX_PD20B_SPI0_MISO);

	gpio_set_pin_function(PD21, MUX_PD21B_SPI0_MOSI);

	gpio_set_pin_function(PD22, MUX_PD22B_SPI0_SPCK);
}

void SPI_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_SPI0);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_async_set_func_ptr(&SPI_0, _spi_get_spi_m_async());
	spi_m_async_init(&SPI_0, SPI0);
	SPI_0_PORT_init();
}

void TARGET_IO_PORT_init(void)
{

	gpio_set_pin_function(PA21, MUX_PA21A_USART1_RXD1);

	gpio_set_pin_function(PB4, MUX_PB4D_USART1_TXD1);
}

void TARGET_IO_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_USART1);
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	TARGET_IO_PORT_init();
	usart_sync_init(&TARGET_IO, USART1, _usart_get_usart_sync());
}

void system_init(void)
{
	init_mcu();

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	SPI_0_init();

	TARGET_IO_init();
}
