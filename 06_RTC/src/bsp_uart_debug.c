#include "hal_data.h"
#include "bsp_uart_debug.h"
#include "stdio.h"

/* UART transmission complete flag */
volatile bool uart_send_complete_flag = false;

/**
 * @brief UART callback function for debug UART
 * @param p_args UART callback arguments
 */
void uart6_debug_callback(uart_callback_args_t *p_args)
{
    if (p_args->event == UART_EVENT_TX_COMPLETE) {
        uart_send_complete_flag = true;
    }
}

/**
 * @brief Initialize debug UART
 */
void uart_debug_init(void)
{
    fsp_err_t err;
    err = R_SCI_UART_Open(g_uart6_debug.p_ctrl, g_uart6_debug.p_cfg);
    assert(FSP_SUCCESS == err);
}

#if defined __GNUC__ && !defined __clang__
/**
 * @brief Write function for GCC
 * @param fd File descriptor
 * @param pBuffer Buffer to write
 * @param size Size of buffer
 * @return int Number of bytes written
 */
int _write(int fd, char *pBuffer, int size);
int _write(int fd, char *pBuffer, int size)
{
    (void)fd;
    R_SCI_UART_Write(&g_uart6_debug_ctrl, (uint8_t *)pBuffer, (uint32_t)size);
    while (uart_send_complete_flag == false);
    uart_send_complete_flag = false;

    return size;
}
#else
/**
 * @brief Write function for other compilers
 * @param ch Character to write
 * @param f File pointer
 * @return int Character written
 */
int fputc(int ch, FILE *f)
{
    (void)f;
    R_SCI_UART_Write(&g_uart6_debug_ctrl, (uint8_t *)&ch, 1);
    while (uart_send_complete_flag == false);
    uart_send_complete_flag = false;

    return ch;
}
#endif
