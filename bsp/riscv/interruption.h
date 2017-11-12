
#ifndef __RISCV_INTERRUPTION_H__
#define __RISCV_INTERRUPTION_H__


typedef void (*pfnInt_handle_t) (void);

typedef enum
{
  INT_UR_SW         = 0,
  INT_SV_SW         ,
  INT_RESER2        ,
  INT_MH_SW         ,
  
  INT_UR_TIMER      ,
  INT_SV_TIMER      ,
  INT_RESER6        ,
  INT_MH_TIMER      ,

  INT_UR_EXTINT     ,
  INT_SV_EXTINT     ,
  INT_RESER10       ,
  INT_MH_EXTINT     ,

  RISCV_INT_NUM
}riscv_int_src_t;

void riscv_int_init(void);
void riscv_int_enable(riscv_int_src_t eSrc, pfnInt_handle_t pfnHandle);
void riscv_int_disable(riscv_int_src_t eSrc);

#endif
