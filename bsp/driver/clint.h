#ifndef __RISCV_CLINT_H
#define __RISCV_CLINT_H

#define CLINT_MSIP        0x0
#define CLINT_MTIMECMP    0x800
#define CLINT_MTIME       0x17ff

typedef void (*pfnClint_handle_t) (void);

typedef enum
{
  CLINT_SRC_MSW     = 0,
  CLINT_SRC_MTM     ,

  CLINT_SRC_NUM
}bm_clint_src_t;

void bm_clint_init(void);
void bm_clint_enable(bm_clint_src_t eSrc, pfnClint_handle_t pfnHandle);
void bm_clint_disable(bm_clint_src_t eSrc);

void bm_clint_swtTriger(void);
void bm_clint_clrswt(void);

#endif
