riscv_path = $(bsp_path)riscv/

inc_dir += $(riscv_path)

c_srcs   += $(riscv_path)interruption.c

asm_srcs += $(riscv_path)cpu_lib.S   \
            $(riscv_path)mentry.S 



