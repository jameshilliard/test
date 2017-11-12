bsp_path = bsp/

inc_dir += $(bsp_path)

c_srcs +=
asm_srcs +=

include $(bsp_path)driver/driver.mk
include $(bsp_path)riscv/riscv.mk
