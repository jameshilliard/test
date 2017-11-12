#include <string.h>
#include "bsp.h"

volatile uint32_t* g_pu32Gpio;

void riscv_gpio_init(uint32_t u32GroupMap, riscv_iodir_t eDir)
{
  g_pu32Gpio = (void*)(uintptr_t)GPIO_BASE;

  if(RISCV_IODIR_OUTPUT == eDir)
    g_pu32Gpio[GPIO_OUTPUT_EN]   |= u32GroupMap;
  else
    g_pu32Gpio[GPIO_INPUT_EN]   |= u32GroupMap;
}

void riscv_gpio_outputGroup(uint32_t u32GroupMap, uint32_t u32Val)
{
  if(0 == u32Val)
    g_pu32Gpio[GPIO_OUTPUT_VAL] &= ~u32GroupMap;
  else
    g_pu32Gpio[GPIO_OUTPUT_VAL] |= u32GroupMap;
}

void riscv_gpio_output(uint32_t u32Idx, uint32_t u32Val)
{
  riscv_gpio_outputGroup((1u << u32Idx), u32Val);
}

uint32_t riscv_gpio_inputGroup(uint32_t u32GroupMap)
{
  return (g_pu32Gpio[GPIO_OUTPUT_VAL] & u32GroupMap);
}

uint32_t riscv_gpio_input(uint32_t u32Idx)
{
  return riscv_gpio_inputGroup(1<<u32Idx) >> u32Idx;
}


