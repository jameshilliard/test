#include <string.h>
#include "bsp.h"

volatile uint32_t* g_pu32Uart;

static void riscv_uart_irqhandle(void)
{
  riscv_gpio_output(GPIO_RESULT, 1);
}

void riscv_uart_putchar(uint8_t ch)
{
  volatile uint32_t *tx = g_pu32Uart + UART_REG_TXFIFO;
  while ((int32_t)(*tx) < 0);
  *tx = ch;
}

int riscv_uart_getchar()
{
  int32_t ch = g_pu32Uart[UART_REG_RXFIFO];
  if (ch < 0) return -1;
  return ch;
}

void riscv_uart_init(uint32_t bandrate)
{
  g_pu32Uart = (void*)(uintptr_t)UART_BASE;
  g_pu32Uart[UART_REG_TXCTRL] = UART_TXEN;
  g_pu32Uart[UART_REG_RXCTRL] = UART_RXEN;
}

void riscv_uart_irqenable(pfnPlic_handle_t pfnHandle)
{
  if(!g_pu32Uart)
    g_pu32Uart =  (void*)(uintptr_t)UART_BASE;

  if(!pfnHandle)
    pfnHandle = riscv_uart_irqhandle;

  g_pu32Uart[UART_REG_IE] = 3;
  bm_plic_irqenable(PLIC_SRC_SERIAL, 5, pfnHandle);
}

void riscv_uart_irqdisable(void)
{
  g_pu32Uart[UART_REG_IE] = 0;
  bm_plic_irqdisable(PLIC_SRC_SERIAL);
}

