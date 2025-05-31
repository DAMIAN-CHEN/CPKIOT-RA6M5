/*
 * drv_timer.h
 *
 *  Created on: 2025年5月28日
 *      Author: DAMIAN_CHEN
 */

#ifndef DRV_TIMER_H_
#define DRV_TIMER_H_


#include "hal_data.h"


//配置好定时器的相关设置
#define    USER_TIMER_CYCCNT       g_timer0_gpt_ctrl.p_reg->GTCNT
#define    TIMER_MAX_CNT_CYCL      0x100000000


typedef struct
{
    uint32_t s;
    uint16_t ms;
    uint16_t us;
} user_time_t;

/**
 * @brief 该宏用于计算代码段执行时间,单位为秒/s,返回值为float类型
 *        首先需要创建一个float类型的变量,用于存储时间间隔
 *        计算得到的时间间隔同时还会打印到日志终端,也可以将dt变量添加到查看
 */
#define TIME_ELAPSE(dt, code)                    \
    do                                           \
    {                                            \
        float tstart = dwt_get_time_s();      \
        code;                                    \
        dt = dwt_get_time_s() - tstart;       \
        LOG_I("" #dt " = %f s\r\n", dt); \
    } while (0)

/**
 * @brief 初始化定时器硬件
 *
 */
void user_timer_hw_init(void);

/**
 * @brief 初始化定时器,传入参数为定时器CNT递增频率,单位MHz
 *
 * @param cnt_freq_mhz RA6M5的GPT时钟总线最大为100MHz
 */
void user_timer_init(uint32_t cnt_freq_mhz);

/**
 * @brief 获取两次调用之间的时间间隔,单位为秒/s
 *
 * @param cnt_last 上一次调用的时间戳
 * @return float 时间间隔,单位为秒/s
 */
float user_timer_get_delta(uint32_t *cnt_last);

/**
 * @brief 获取两次调用之间的时间间隔,单位为秒/s,高精度
 *
 * @param cnt_last 上一次调用的时间戳
 * @return double 时间间隔,单位为秒/s
 */
double user_timer_get_delta_64(uint32_t *cnt_last);

/**
 * @brief 获取当前时间,单位为秒/s,即初始化后的时间
 *
 * @return float 时间轴
 */
float user_timer_get_time_s(void);

/**
 * @brief 获取当前时间,单位为毫秒/ms,即初始化后的时间
 *
 * @return float
 */
float user_timer_get_time_ms(void);

/**
 * @brief 获取当前时间,单位为微秒/us,即初始化后的时间
 *
 * @return uint64_t
 */
uint64_t user_timer_get_time_us(void);

/**
 * @brief DWT延时函数,单位为秒/s
 * @attention 该函数不受中断是否开启的影响,可以在临界区和关闭中断时使用
 * @note 禁止在__disable_irq()和__enable_irq()之间使用HAL_Delay()函数,应使用本函数
 *
 * @param delay 延时时间,单位为秒/s
 */
void user_timer_delay_s(float delay);

/**
 * @brief 用户自定义定时器更新时间轴函数,会被三个timeline函数调用
 * @attention 如果长时间不调用timeline函数,则需要手动调用该函数更新时间轴,否则CYCCNT溢出后定时和时间轴不准确
 */
void user_timer_systime_update(void);


#endif /* DRV_TIMER_H_ */
