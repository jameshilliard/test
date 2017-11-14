#include <stdint.h>
#include "interruption.h"
#include "csr.h"


static pfnInt_handle_t g_pfnInt_handle[RISCV_INT_NUM];

void interrupt_trap(riscv_int_src_t eSrc, uintptr_t epc)
{
  if((eSrc >= RISCV_INT_NUM) || (!g_pfnInt_handle[eSrc]))
    return ;

  g_pfnInt_handle[eSrc]();
}

void riscv_int_init(void)
{
  int i;

  for(i=0; i<RISCV_INT_NUM; i++)
    g_pfnInt_handle[i] = 0;

  write_csr(mie, 0);
  write_csr(medeleg, 0);
  write_csr(mideleg, 0);
  set_csr(mstatus, MSTATUS_MIE);
}

void riscv_int_enable(riscv_int_src_t eSrc, pfnInt_handle_t pfnHandle)
{
  if(eSrc >= RISCV_INT_NUM)
    return ;

  g_pfnInt_handle[eSrc] = pfnHandle;
  set_csr(mie, (1u << eSrc));
}

void riscv_int_disable(riscv_int_src_t eSrc)
{
  if(eSrc >= RISCV_INT_NUM)
    return ;
  
  clear_csr(mie, (1u << eSrc));
  g_pfnInt_handle[eSrc] = 0;
}

