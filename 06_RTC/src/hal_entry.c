#include "hal_data.h"
#include "bsp_uart_debug.h"
#include "stdio.h"

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER

fsp_err_t err = FSP_SUCCESS;

// RTC变量
typedef struct tm rtc_time_t;
rtc_time_t set_time = {
    .tm_sec  = 50,   // 秒，范围从 0 到 59
    .tm_min  = 59,   // 分，范围从 0 到 59
    .tm_hour = 23,   // 小时，范围从 0 到 23
    .tm_mday = 28,   // 一月中的第几天，范围从 1 到 31
    .tm_mon  = 1,    // 月份，范围从 0 到 11
    .tm_year = 125,  // 自 1900 起的年数，2025为125
    .tm_wday = 5,    // 一周中的第几天，范围从 0 到 6
    // .tm_yday=0,   // 一年中的第几天，范围从 0 到 365
    // .tm_isdst=0;  // 夏令时
};

// RTC闹钟变量
rtc_alarm_time_t set_alarm_time = {
    .time.tm_sec  = 10,   // 秒，范围从 0 到 59
    .time.tm_min  = 30,   // 分，范围从 0 到 59
    .time.tm_hour = 12,   // 小时，范围从 0 到 23
    .time.tm_mday = 1,    // 一月中的第几天，范围从 1 到 31
    .time.tm_mon  = 2,    // 月份，范围从 0 到 11
    .time.tm_year = 125,  // 自 1900 起的年数，2025为125
    .time.tm_wday = 6,    // 一周中的第几天，范围从 0 到 6

    .sec_match        = 1,
    .min_match        = 0,
    .hour_match       = 0,
    .mday_match       = 0,
    .mon_match        = 0,
    .year_match       = 0,
    .dayofweek_match  = 0,
};

// RTC回调函数
volatile bool rtc_flag = 0;       // RTC延时1s标志位
volatile bool rtc_alarm_flag = 0; // RTC闹钟

// Callback function
void rtc_callback(rtc_callback_args_t *p_args)
{
    if (p_args->event == RTC_EVENT_PERIODIC_IRQ)
    {
        rtc_flag = 1;
    }
    else if (p_args->event == RTC_EVENT_ALARM_IRQ)
    {
        rtc_alarm_flag = 1;
    }
}

/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{
    uart_debug_init();

    /********************** RTC开启 ***************************************/
    // Initialize the RTC module
    err = R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    assert(FSP_SUCCESS == err);

    // Set the RTC clock source. Can be skipped if "Set Source Clock in Open" property is enabled.
    R_RTC_ClockSourceSet(&g_rtc0_ctrl);
    // R_RTC_CalendarTimeSet must be called at least once to start the RTC
    R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &set_time);
    // Set the periodic interrupt rate to 1 second
    R_RTC_PeriodicIrqRateSet(&g_rtc0_ctrl, RTC_PERIODIC_IRQ_SELECT_1_SECOND);
    R_RTC_CalendarAlarmSet(&g_rtc0_ctrl, &set_alarm_time);

    uint8_t rtc_second = 0; // 秒
    uint8_t rtc_minute = 0; // 分
    uint8_t rtc_hour   = 0; // 时
    uint8_t rtc_day    = 0; // 日
    uint8_t rtc_month  = 0; // 月
    uint16_t rtc_year  = 0; // 年
    uint8_t rtc_week   = 0; // 周
    rtc_time_t get_time;

    while (1)
    {
        if (rtc_flag)
        {
            R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &get_time); // 获取RTC计数时间
            rtc_flag = 0;
            rtc_second = (uint8_t)get_time.tm_sec;
            rtc_minute = (uint8_t)get_time.tm_min;
            rtc_hour   = (uint8_t)get_time.tm_hour;
            rtc_day    = (uint8_t)get_time.tm_mday;
            rtc_month  = (uint8_t)get_time.tm_mon;
            rtc_year   = (uint8_t)get_time.tm_year;
            rtc_week   = (uint8_t)get_time.tm_wday;
            printf(" %d year %d mon %d day %d hour %d min %d sec %d week\n",
                   rtc_year + 1900, rtc_month + 1, rtc_day, rtc_hour, rtc_minute, rtc_second, rtc_week);
        }
        if (rtc_alarm_flag)
        {
            rtc_alarm_flag = 0;
            printf("/************************Alarm Clock********************************/\n");
        }
        R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
    }

#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0
        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;
        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */
        /* Configure pins. */
        R_IOPORT_Open(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
#if BSP_CFG_SDRAM_ENABLED
        /* Setup SDRAM and initialize it. Must configure pins first. */
        R_BSP_SdramInit(true);
#endif
    }
}

#if BSP_TZ_SECURE_BUILD

FSP_CPP_HEADER
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable();

/* Trustzone Secure Projects require at least one nonsecure callable function in order to build (Remove this if it is not required to build). */
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable()
{
}
FSP_CPP_FOOTER

#endif
