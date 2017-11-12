#include "bsp.h"
#include <string.h>

static uint32_t g_u32IntFlag;

static void _test_swhandle(void)
{
  g_u32IntFlag = 1;
  bm_clint_clrswt();
}

/* 
 * test an software-interruption, core-0
 */
void test_main(uintptr_t hartid, uintptr_t dtb)
{
  g_u32IntFlag = 0;
  
  riscv_int_init();
  bm_clint_init();
  bm_clint_enable(CLINT_SRC_MSW, _test_swhandle);

  riscv_test_start();

  /* begin to test */
  bm_clint_swtTriger();

  __delayCycles(0x10);
  riscv_test_end(g_u32IntFlag);
  
  while(1)
    __wfi();
}


