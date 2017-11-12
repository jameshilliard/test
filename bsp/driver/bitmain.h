#ifndef _RISCV_BM_PLATFORM_H
#define _RISCV_BM_PLATFORM_H

#include <stdint.h>
#include "cpu_lib.h"
#include "bsp.h"


#define ROM_BASE          0x80000000u
#define ROM_SIZE          (32 * 1024)

#define RAM_BASE          0x80100000u
#define RAM_SIZE          (32 * 1024)

#define CLINT_BASE        0x02000000u
#define GLIC_BASE         0x0c000000u

#define PCIE_BASE         0x50000000u
#define UART_BASE         0x54000000u
#define SPI_BASE          0x54001000u
#define GPIO_BASE         0x54002000u

#define DRAM_BASE         0x80000000u


#define TRES_ADDR         0xbFFFFFFCu
#define TRES_SUCCESS      0xaaaaaaaau
#define TRES_FAIL         0xffffffffu


static inline void __delayCycles(uint32_t u32Cyc)
{
  while(u32Cyc--) __nop();
}

static inline void __testSetResult(uint32_t u32Res)
{
  volatile uint32_t* pu32Point = (void*)(uintptr_t)TRES_ADDR;

  * pu32Point = u32Res == TRES_SUCCESS ? TRES_SUCCESS : TRES_FAIL;
}

static inline void riscv_test_start(void)
{
  riscv_gpio_init(STATUS_MASK | RESULT_MASK, RISCV_IODIR_OUTPUT);
  riscv_gpio_outputGroup(STATUS_MASK | RESULT_MASK, 0);

  __delayCycles(0x1000);
  riscv_gpio_outputGroup(RESULT_MASK, 1);

  __delayCycles(0x1000);
  riscv_gpio_outputGroup(RESULT_MASK, 0);
}

static inline void riscv_test_end(int res)
{
  if(res)
    riscv_gpio_output(GPIO_RESULT, 1);
  else
    riscv_gpio_output(GPIO_RESULT, 0);
  riscv_gpio_output(GPIO_STATUS, 1);  
}


#endif
