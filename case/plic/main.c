#include "bsp.h"
#include <string.h>

static uint32_t g_u32IntFlag;

static void _test_irqhandle(void)
{
  g_u32IntFlag = 1;
  riscv_uart_irqdisable();
}

/* 
 * test an ext-interruption, uart
 */
void test_main(uintptr_t hartid, uintptr_t dtb)
{
  g_u32IntFlag = 0;
  
  riscv_int_init();
  bm_plic_init();

  riscv_uart_init(115200);
  riscv_uart_irqenable(_test_irqhandle);
  
  riscv_test_start();

  riscv_uart_putchar('t');  
  __delayCycles(0x10000);  
  
  riscv_test_end(g_u32IntFlag);
  while(1)
    __wfi();
}


