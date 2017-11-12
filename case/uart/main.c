#include "bsp.h"
#include <string.h>

/* 
 * uart测试 uart的tx会环回到rx 用到两个gpio output
 * （一个代表done 一个代表pass） 上电后done=0 pass =0 
 * 然后done=1 pass=1 然后 done=0 pass=0 然后测试uart发收 
 * 测试结束后 done=1 pass=软件接收uart与发送比较的结果
 */
void test_main(uintptr_t hartid, uintptr_t dtb)
{
  int ch = 0;

  riscv_uart_init(115200);
  riscv_test_start();

  riscv_uart_putchar('t');
  while(ch <= 0)
    ch = riscv_uart_getchar();
  
  riscv_test_end('t' == ch);
  while(1);
}


