#include "ws2812.h"

/* WS2812 signal levels for SPI transmission */
#define WS2812_LOW_LEVEL    0xC0    /* Logic 0 signal level */
#define WS2812_HIGH_LEVEL   0xF0    /* Logic 1 signal level */

/* Color channel enumeration for LED control */
enum color {
	RED_H = 0,    /* Red channel high byte */
	GREEN_H = 1,  /* Green channel high byte */
	BLUE_H = 2,   /* Blue channel high byte */
	RED_L = 3,    /* Red channel low byte */
	GREEN_L = 4,  /* Green channel low byte */
	BLUE_L = 5,   /* Blue channel low byte */
};

/**
 * @brief Control WS2812 LED color using SPI
 * @param r Red color value (0-255)
 * @param g Green color value (0-255)
 * @param b Blue color value (0-255)
 */
void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t txbuf[24];
	uint8_t res = 0;

	/* Convert RGB values to WS2812 signal format */
	for (int i = 0; i < 8; i++) {
		txbuf[7 - i]  = (((g >> i) & 0x01) ? WS2812_HIGH_LEVEL : WS2812_LOW_LEVEL) >> 1;
		txbuf[15 - i] = (((r >> i) & 0x01) ? WS2812_HIGH_LEVEL : WS2812_LOW_LEVEL) >> 1;
		txbuf[23 - i] = (((b >> i) & 0x01) ? WS2812_HIGH_LEVEL : WS2812_LOW_LEVEL) >> 1;
	}

	/* Send color data via SPI */
	R_SPI_Write(&g_spi1_ctrl, txbuf, 24, SPI_BIT_WIDTH_8_BITS);

	/* Send reset signal */
	for (int i = 0; i < 100; i++) {
		R_SPI_Write(&g_spi1_ctrl, &res, 1, SPI_BIT_WIDTH_8_BITS);
	}
}

/**
 * @brief Initialize WS2812 LED control
 * Opens SPI module for LED control
 */
void ws_2812_init(void)
{
	R_SPI_Open(&g_spi1_ctrl, &g_spi1_cfg);
}

/**
 * @brief SPI callback function
 * @param p_args Pointer to callback arguments
 */
void spi1_callback(spi_callback_args_t *p_args)
{
	if (p_args->event == SPI_EVENT_TRANSFER_COMPLETE) {
		/* Handle transfer complete event if needed */
	}
}
