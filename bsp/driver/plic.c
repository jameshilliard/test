#include "bsp.h"


volatile uint32_t* g_pu32Plic;
static pfnPlic_handle_t g_pfnPlic_handle[PLIC_INT_NUM];

static void _plic_irqhandle(void)
{
  bm_plic_src_t eSrc = g_pu32Plic[PLIC_INT_CLAIM];

  if((eSrc >= PLIC_INT_NUM) || (!g_pfnPlic_handle[eSrc]))
    return ;

  g_pfnPlic_handle[eSrc]();
  g_pu32Plic[PLIC_INT_COMPLETE] = eSrc;
}

void bm_plic_init(void)
{
  uint32_t i;
  
  g_pu32Plic = (void*)(uintptr_t)GLIC_BASE;

  for(i=0; i<PLIC_INT_NUM; i++)
    g_pu32Plic[PLIC_SRC_PRIORITY + i] = 1;

  g_pu32Plic[PLIC_INT_ENABLE] = 0;
  g_pu32Plic[PLIC_PRIORITY_THRES] = 1;

  riscv_int_enable(INT_MH_EXTINT, _plic_irqhandle);
}

void bm_plic_irqenable(bm_plic_src_t eSrc, uint32_t u32Prioty, pfnPlic_handle_t pfnHandle)
{
  if(!g_pu32Plic)
    g_pu32Plic = (void*)(uintptr_t)GLIC_BASE;

  if(eSrc >= PLIC_INT_NUM)
    return ;

  g_pu32Plic[PLIC_SRC_PRIORITY + eSrc] = u32Prioty;
  g_pu32Plic[PLIC_INT_ENABLE + eSrc / 32] |= 1u << (eSrc % 32);

  g_pfnPlic_handle[eSrc] = pfnHandle;
}

void bm_plic_irqdisable(bm_plic_src_t eSrc)
{
  if(!g_pu32Plic)
    g_pu32Plic = (void*)(uintptr_t)GLIC_BASE;

  if(eSrc >= PLIC_INT_NUM)
    return ;

  g_pu32Plic[PLIC_SRC_PRIORITY + eSrc] = 0;
  g_pu32Plic[PLIC_INT_ENABLE + eSrc / 32] &= ~(1u << (eSrc % 32));
}



