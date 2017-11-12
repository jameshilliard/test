#ifndef __RISCV_PLIC_H
#define __RISCV_PLIC_H

#define PLIC_SRC_PRIORITY     0
#define PLIC_INT_PENDING      0x400
#define PLIC_INT_ENABLE       0x800
#define PLIC_PRIORITY_THRES   0x80000
#define PLIC_INT_CLAIM        0x80001
#define PLIC_INT_COMPLETE     0x80001

typedef void (*pfnPlic_handle_t) (void);

typedef enum
{
  PLIC_SRC_SERIAL         = 1,
  PLIC_SRC_SPI            ,
  PLIC_SRC_GPIO1          ,
  PLIC_SRC_GPIO2          ,
  PLIC_SRC_GPIO3          ,
  PLIC_SRC_GPIO4          ,
  PLIC_SRC_PCIE           ,

  PLIC_INT_NUM
}bm_plic_src_t;

void bm_plic_init(void);
void bm_plic_irqenable(bm_plic_src_t eSrc, uint32_t u32Prioty, pfnPlic_handle_t pfnHandle);
void bm_plic_irqdisable(bm_plic_src_t eSrc);


#endif
