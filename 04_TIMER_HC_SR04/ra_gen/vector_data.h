/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (6)
#endif
/* ISR prototypes */
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void sci_uart_eri_isr(void);
void r_icu_isr(void);
void gpt_counter_overflow_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_SCI6_RXI ((IRQn_Type) 0) /* SCI6 RXI (Receive data full) */
#define SCI6_RXI_IRQn          ((IRQn_Type) 0) /* SCI6 RXI (Receive data full) */
#define VECTOR_NUMBER_SCI6_TXI ((IRQn_Type) 1) /* SCI6 TXI (Transmit data empty) */
#define SCI6_TXI_IRQn          ((IRQn_Type) 1) /* SCI6 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI6_TEI ((IRQn_Type) 2) /* SCI6 TEI (Transmit end) */
#define SCI6_TEI_IRQn          ((IRQn_Type) 2) /* SCI6 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI6_ERI ((IRQn_Type) 3) /* SCI6 ERI (Receive error) */
#define SCI6_ERI_IRQn          ((IRQn_Type) 3) /* SCI6 ERI (Receive error) */
#define VECTOR_NUMBER_ICU_IRQ9 ((IRQn_Type) 4) /* ICU IRQ9 (External pin interrupt 9) */
#define ICU_IRQ9_IRQn          ((IRQn_Type) 4) /* ICU IRQ9 (External pin interrupt 9) */
#define VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW ((IRQn_Type) 5) /* GPT0 COUNTER OVERFLOW (Overflow) */
#define GPT0_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 5) /* GPT0 COUNTER OVERFLOW (Overflow) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
