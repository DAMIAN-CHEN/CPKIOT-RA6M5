/*
 * drv_hc_sr04.c
 *
 *  Created on: 2025年5月28日
 *      Author: DAMIAN_CHEN
 */
#include "drv_hc_sr04.h"

/* HC-SR04 measurement variables */
static uint64_t cnt_us_start;
static uint64_t cnt_us_end;
float hc_sr04_length;

/**
 * @brief Initialize HC-SR04 ultrasonic sensor
 */
void hc_sr04_init(void)
{
    /* Open ICU module for echo pin interrupt */
    R_ICU_ExternalIrqOpen(&g_external_irq9_ctrl, &g_external_irq9_cfg);
    /* Enable interrupt */
    R_ICU_ExternalIrqEnable(&g_external_irq9_ctrl);
}

/**
 * @brief Generate trigger pulse for HC-SR04
 */
void hc_sr04_trig(void)
{
    uint64_t keep_start_us;
    if (R_BSP_PinRead(BSP_IO_PORT_00_PIN_03) == BSP_IO_LEVEL_LOW) {
        R_IOPORT_PinWrite(g_ioport.p_ctrl, BSP_IO_PORT_00_PIN_03,
                         BSP_IO_LEVEL_HIGH);

        keep_start_us = user_timer_get_time_us();

        while (!((user_timer_get_time_us() - keep_start_us) > 15));

        R_IOPORT_PinWrite(g_ioport.p_ctrl, BSP_IO_PORT_00_PIN_03,
                         BSP_IO_LEVEL_LOW);
    }
}

/**
 * @brief Process echo signal from HC-SR04
 */
void hc_sr04_echo(void)
{
    if (R_BSP_PinRead(BSP_IO_PORT_00_PIN_04) == BSP_IO_LEVEL_HIGH) {
        cnt_us_start = user_timer_get_time_us();
    } else {
        cnt_us_end = user_timer_get_time_us();
        /* Calculate distance: time * speed of sound / 2 */
        hc_sr04_length = (float)(cnt_us_end - cnt_us_start) / 58;
    }
}

/**
 * @brief External interrupt callback for echo pin
 * @param p_args Interrupt callback arguments
 */
void external_irq9_callback(external_irq_callback_args_t *p_args)
{
    if (9 == p_args->channel) {
        hc_sr04_echo();
    }
}

