#ifndef __WS2812_H__
#define __WS2812_H__

#include "hal_data.h"

/**
 * @brief Control WS2812 LED color using SPI
 * @param r Red color value (0-255)
 * @param g Green color value (0-255)
 * @param b Blue color value (0-255)
 */
void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Initialize WS2812 LED control
 * Opens SPI module for LED control
 */
void ws_2812_init(void);

#endif /* __WS2812_H__ */
