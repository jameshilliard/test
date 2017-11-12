#ifndef _RISCV_GPIO_H
#define _RISCV_GPIO_H

#include <stdint.h>

#define GPIO_STATUS       0
#define GPIO_RESULT       1

#define STATUS_MASK       (0x1<< GPIO_STATUS)
#define RESULT_MASK       (0x1<< GPIO_RESULT)


#define GPIO_INPUT_VAL    (0x00u)          //(0x00)
#define GPIO_INPUT_EN     (0x01u)          //(0x04)
#define GPIO_OUTPUT_EN    (0x02u)          //(0x08)
#define GPIO_OUTPUT_VAL   (0x03u)          //(0x0C)
#define GPIO_PULLUP_EN    (0x04u)          //(0x10)
#define GPIO_DRIVE        (0x05u)          //(0x14)
#define GPIO_RISE_IE      (0x06u)          //(0x18)
#define GPIO_RISE_IP      (0x07u)          //(0x1C)
#define GPIO_FALL_IE      (0x08u)          //(0x20)
#define GPIO_FALL_IP      (0x09u)          //(0x24)
#define GPIO_HIGH_IE      (0x0au)          //(0x28)
#define GPIO_HIGH_IP      (0x0bu)          //(0x2C)
#define GPIO_LOW_IE       (0x0cu)          //(0x30)
#define GPIO_LOW_IP       (0x0du)          //(0x34)
#define GPIO_IOF_EN       (0x0eu)          //(0x38)
#define GPIO_IOF_SEL      (0x0fu)          //(0x3C)
#define GPIO_OUTPUT_XOR   (0x10u)          //(0x40)

typedef enum 
{
  RISCV_IODIR_INPUT      = 0,
  RISCV_IODIR_OUTPUT
}riscv_iodir_t;

void riscv_gpio_init(uint32_t u32GroupMap, riscv_iodir_t eDir);
void riscv_gpio_outputGroup(uint32_t u32GroupMap, uint32_t u32Val);
void riscv_gpio_output(uint32_t u32Idx, uint32_t u32Val);
uint32_t riscv_gpio_inputGroup(uint32_t u32GroupMap);
uint32_t riscv_gpio_input(uint32_t u32Idx);


#endif
