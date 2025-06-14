/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = sci_uart_rxi_isr, /* SCI6 RXI (Receive data full) */
            [1] = sci_uart_txi_isr, /* SCI6 TXI (Transmit data empty) */
            [2] = sci_uart_tei_isr, /* SCI6 TEI (Transmit end) */
            [3] = sci_uart_eri_isr, /* SCI6 ERI (Receive error) */
            [4] = r_icu_isr, /* ICU IRQ9 (External pin interrupt 9) */
            [5] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_SCI6_RXI,GROUP0), /* SCI6 RXI (Receive data full) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TXI,GROUP1), /* SCI6 TXI (Transmit data empty) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TEI,GROUP2), /* SCI6 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SCI6_ERI,GROUP3), /* SCI6 ERI (Receive error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ9,GROUP4), /* ICU IRQ9 (External pin interrupt 9) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_GPT0_COUNTER_OVERFLOW,GROUP5), /* GPT0 COUNTER OVERFLOW (Overflow) */
        };
        #endif
        #endif
