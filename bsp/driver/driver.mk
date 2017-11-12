driver_path = $(bsp_path)driver/

inc_dir += $(driver_path)

c_srcs += \
  $(driver_path)riscv_uart.c \
  $(driver_path)plic.c \
  $(driver_path)clint.c \
  $(driver_path)riscv_gpio.c 

asm_srcs +=
