/*
 * drv_timer.c
 *
 *  Created on: 2025年5月28日
 *      Author: DAMIAN_CHEN
 */
#include "drv_timer.h"

/* System time structure */
static user_time_t systime;
static uint32_t timer_freq_hz, timer_freq_hz_ms, timer_freq_hz_us;
static uint32_t CYCCNT_rount_count;
static uint32_t CYCCNT_last;
static uint64_t CYCCNT64;

/**
 * @brief Check if hardware register overflows and update CYCCNT_rount_count
 * @attention This function assumes the time interval between two calls is less than one overflow
 */
static void user_timer_cnt_update(void)
{
    static volatile uint8_t bit_locker = 0;
    if (!bit_locker) {
        bit_locker = 1;
        volatile uint32_t cnt_now = USER_TIMER_CYCCNT;
        if (cnt_now < CYCCNT_last) {
            CYCCNT_rount_count++;
        }
        CYCCNT_last = USER_TIMER_CYCCNT;
        bit_locker = 0;
    }
}

/**
 * @brief Timer callback function for GPT0
 * @param p_args Timer callback arguments
 */
void timer0_gpt_callback(timer_callback_args_t *p_args)
{
    /* Timer overflow event */
    if (TIMER_EVENT_CYCLE_END == p_args->event) {
        user_timer_cnt_update();
    }
}

/**
 * @brief Initialize timer hardware
 */
void user_timer_hw_init(void)
{
    /* Initialize GPT0 module */
    R_GPT_Open(&g_timer0_gpt_ctrl, &g_timer0_gpt_cfg);
    /* Start GPT0 timer */
    R_GPT_Start(&g_timer0_gpt_ctrl);
}

/**
 * @brief Initialize timer with specified frequency
 * @param timer_freq_mhz Timer frequency in MHz
 */
void user_timer_init(uint32_t timer_freq_mhz)
{
    user_timer_hw_init();

    timer_freq_hz = timer_freq_mhz * 1000000;
    timer_freq_hz_ms = timer_freq_hz / 1000;
    timer_freq_hz_us = timer_freq_hz / 1000000;
    CYCCNT_rount_count = 0;

    user_timer_cnt_update();
}

/**
 * @brief Calculate time delta between two calls in seconds
 * @param cnt_last Pointer to last count value
 * @return float Time delta in seconds
 */
float user_timer_get_delta(uint32_t *cnt_last)
{
    volatile uint32_t cnt_now = USER_TIMER_CYCCNT;
    float dt = (float)((uint32_t)(cnt_now - *cnt_last)) / ((float)(timer_freq_hz));
    *cnt_last = cnt_now;

    user_timer_cnt_update();

    return dt;
}

/**
 * @brief Calculate time delta between two calls in seconds (high precision)
 * @param cnt_last Pointer to last count value
 * @return double Time delta in seconds
 */
double user_timer_get_delta_64(uint32_t *cnt_last)
{
    volatile uint32_t cnt_now = USER_TIMER_CYCCNT;
    double dt = ((uint32_t)(cnt_now - *cnt_last)) / ((double)(timer_freq_hz));
    *cnt_last = cnt_now;

    user_timer_cnt_update();

    return dt;
}

/**
 * @brief Update system time
 */
void user_timer_systime_update(void)
{
    volatile uint32_t cnt_now = USER_TIMER_CYCCNT;
    static uint64_t CNT_TEMP1, CNT_TEMP2, CNT_TEMP3;

    user_timer_cnt_update();

    CYCCNT64 = (uint64_t)CYCCNT_rount_count * (uint64_t)TIMER_MAX_CNT_CYCL + (uint64_t)cnt_now;
    CNT_TEMP1 = CYCCNT64 / timer_freq_hz;
    CNT_TEMP2 = CYCCNT64 - CNT_TEMP1 * timer_freq_hz;
    systime.s = (uint32_t)CNT_TEMP1;
    systime.ms = (uint16_t)(CNT_TEMP2 / timer_freq_hz_ms);
    CNT_TEMP3 = CNT_TEMP2 - systime.ms * timer_freq_hz_ms;
    systime.us = (uint16_t)(CNT_TEMP3 / timer_freq_hz_us);
}

/**
 * @brief Get current time in seconds
 * @return float Current time in seconds
 */
float user_timer_get_time_s(void)
{
    user_timer_systime_update();
    float DWT_Timelinef32 = (float)systime.s + (float)systime.ms * 0.001f + (float)systime.us * 0.000001f;
    return DWT_Timelinef32;
}

/**
 * @brief Get current time in milliseconds
 * @return float Current time in milliseconds
 */
float user_timer_get_time_ms(void)
{
    user_timer_systime_update();
    float DWT_Timelinef32 = (float)systime.s * 1000 + (float)systime.ms + (float)systime.us * 0.001f;
    return DWT_Timelinef32;
}

/**
 * @brief Get current time in microseconds
 * @return uint64_t Current time in microseconds
 */
uint64_t user_timer_get_time_us(void)
{
    user_timer_systime_update();
    uint64_t DWT_Timelinef32 = systime.s * 1000000 + systime.ms * 1000 + systime.us;
    return DWT_Timelinef32;
}

/**
 * @brief Delay function in seconds
 * @param delay Delay time in seconds
 */
void user_timer_delay_s(float delay)
{
    uint32_t tickstart = USER_TIMER_CYCCNT;
    float wait = delay;

    while ((USER_TIMER_CYCCNT - tickstart) < wait * (float)timer_freq_hz);
}
