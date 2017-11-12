


#ifndef __RISCV_LIB_H__
#define __RISCV_LIB_H__


static inline void __wfi(void)
{
  asm volatile ("wfi\n");
}

static inline void __nop(void)
{
  asm volatile ("nop\n");
}

static inline void __fence(void)
{
  asm volatile ("fence\n");
}

#endif
