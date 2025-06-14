/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_rtc.h"
#include "r_rtc_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
FSP_HEADER
/* RTC Instance. */
extern const rtc_instance_t g_rtc0;

/** Access the RTC instance using these structures when calling API functions directly (::p_api is not used). */
extern rtc_instance_ctrl_t g_rtc0_ctrl;
extern const rtc_cfg_t g_rtc0_cfg;

#ifndef rtc_callback
void rtc_callback(rtc_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart6_debug;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart6_debug_ctrl;
extern const uart_cfg_t g_uart6_debug_cfg;
extern const sci_uart_extended_cfg_t g_uart6_debug_cfg_extend;

#ifndef uart6_debug_callback
void uart6_debug_callback(uart_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
