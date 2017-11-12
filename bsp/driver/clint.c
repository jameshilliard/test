#include "bsp.h"

volatile uint32_t* g_pu32Sw;
volatile uint64_t* g_pu64Tm;

static void _clint_swhandle(void)
{
  ;
}

static void _clint_tmhandle(void)
{
  ;
}

void bm_clint_init(void)
{
  uint32_t i;
  
  g_pu32Sw = (void*)(uintptr_t)CLINT_BASE;
  g_pu64Tm = (void*)(uintptr_t)CLINT_BASE;
}

void bm_clint_enable(bm_clint_src_t eSrc, pfnClint_handle_t pfnHandle)
{
  riscv_int_src_t eIntSrc;
  
  if(eSrc >= CLINT_SRC_NUM)
    return ;

  if(!pfnHandle)
    pfnHandle = eSrc == CLINT_SRC_MSW ? _clint_swhandle : _clint_tmhandle;

  eIntSrc = eSrc == CLINT_SRC_MSW ? INT_MH_SW : INT_MH_TIMER;
  riscv_int_enable(eIntSrc, pfnHandle);
}

void bm_clint_disable(bm_clint_src_t eSrc)
{
  riscv_int_src_t eIntSrc;
  
  if(eSrc >= CLINT_SRC_NUM)
    return ;

  eIntSrc = eSrc == CLINT_SRC_MSW ? INT_MH_SW : INT_MH_TIMER;
  riscv_int_disable(eIntSrc);
}

void bm_clint_swtTriger(void)
{
  if(!g_pu32Sw)
    g_pu32Sw = (void*)(uintptr_t)CLINT_BASE;

  g_pu32Sw[CLINT_MSIP] = 0x1;
}

void bm_clint_clrswt(void)
{
  if(!g_pu32Sw)
    g_pu32Sw = (void*)(uintptr_t)CLINT_BASE;

  g_pu32Sw[CLINT_MSIP] = 0x0;
}




