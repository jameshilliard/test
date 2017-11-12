#ifndef _RISCV_UART_H
#define _RISCV_UART_H

#include <stdint.h>

#define UART_REG_TXFIFO   0
#define UART_REG_RXFIFO   1
#define UART_REG_TXCTRL   2
#define UART_REG_RXCTRL   3
#define UART_REG_IE       4
#define UART_REG_IP       5
#define UART_REG_DIV      6

#define UART_TXEN         0x1
#define UART_RXEN         0x1

void riscv_uart_putchar(uint8_t ch);
int  riscv_uart_getchar(void);
void riscv_uart_init(uint32_t bandrate);

void riscv_uart_irqenable(pfnPlic_handle_t pfnHandle);
void riscv_uart_irqdisable(void);


#endif
