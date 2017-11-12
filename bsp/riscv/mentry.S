#include "bits.h"
#include "csr.h"

  .option norvc
  .section .text.init,"ax",@progbits
  .globl reset_vector
reset_vector:
  li x1, 0
  li x2, 0
  li x3, 0
  li x4, 0
  li x5, 0
  li x6, 0
  li x7, 0
  li x8, 0
  li x9, 0
// save a0 and a1; arguments from previous boot loader stage:
//  li x10, 0
//  li x11, 0
  li x12, 0
  li x13, 0
  li x14, 0
  li x15, 0
  li x16, 0
  li x17, 0
  li x18, 0
  li x19, 0
  li x20, 0
  li x21, 0
  li x22, 0
  li x23, 0
  li x24, 0
  li x25, 0
  li x26, 0
  li x27, 0
  li x28, 0
  li x29, 0
  li x30, 0
  li x31, 0
  csrw mscratch, x0

  # write mtvec and make sure it sticks
  la t0, trap_vector
  csrw mtvec, t0
  csrr t1, mtvec
1:bne t0, t1, 1b

  la sp, stacks
  csrr a3, mhartid
  slli a2, a3, RISCV_PGSHIFT
  add sp, sp, a2

  # Boot on the first unmasked hart
  la a4, platform__disabled_hart_mask
  LOAD a4, 0(a4)
  addi a5, a4, 1
  not a4, a4
  and a4, a4, a5
  srl a4, a4, a3
  andi a4, a4, 1
  beqz a4, other_cpu

  la a2, __DATA_LMA_START__
  la a3, __DATA_START__
  la a4, __DATA_SIZE__

data_cpy:
  lb a5, 0(a2)
  sb a5, 0(a3)
  addi a2, a2, 1
  addi a3, a3, 1
  addi a4, a4, -1
  bnez a4, data_cpy

  la a3, __BSS_START__
  la a4, __BSS_SIZE__
  li x1, 0

data_clear:
  sb x1, 0(a3)
  addi a3, a3, 1
  addi a4, a4, -1
  bnez a4, data_clear
  
  call test_main
  j die

other_cpu:
  # set MSIE bit to receive IPI
  li a2, MIP_MSIP
  csrw mie, a2

.LmultiHart:
#if MAX_HARTS > 1
  # wait for an IPI to signal that it is safe to boot
  wfi

  # masked harts never start
  la a4, platform__disabled_hart_mask
  LOAD a4, 0(a4)
  srl a4, a4, a3
  andi a4, a4, 1
  bnez a4, .LmultiHart

  # only start if mip is set
  csrr a2, mip
  andi a2, a2, MIP_MSIP
  beqz a2, .LmultiHart

  # make sure our hart id is within a valid range
  fence
  li a2, MAX_HARTS
  # bltu a3, a2, init_other_hart
  b .
#endif
  wfi
  j .LmultiHart


trap_vector:
  addi sp, sp, -32*REGBYTES

  STORE x1, 1*REGBYTES(sp)
  STORE x2, 2*REGBYTES(sp)
  STORE x3, 3*REGBYTES(sp)
  STORE x4, 4*REGBYTES(sp)
  STORE x5, 5*REGBYTES(sp)
  STORE x6, 6*REGBYTES(sp)
  STORE x7, 7*REGBYTES(sp)
  STORE x8, 8*REGBYTES(sp)
  STORE x9, 9*REGBYTES(sp)
  STORE x10, 10*REGBYTES(sp)
  STORE x11, 11*REGBYTES(sp)
  STORE x12, 12*REGBYTES(sp)
  STORE x13, 13*REGBYTES(sp)
  STORE x14, 14*REGBYTES(sp)
  STORE x15, 15*REGBYTES(sp)
  STORE x16, 16*REGBYTES(sp)
  STORE x17, 17*REGBYTES(sp)
  STORE x18, 18*REGBYTES(sp)
  STORE x19, 19*REGBYTES(sp)
  STORE x20, 20*REGBYTES(sp)
  STORE x21, 21*REGBYTES(sp)
  STORE x22, 22*REGBYTES(sp)
  STORE x23, 23*REGBYTES(sp)
  STORE x24, 24*REGBYTES(sp)
  STORE x25, 25*REGBYTES(sp)
  STORE x26, 26*REGBYTES(sp)
  STORE x27, 27*REGBYTES(sp)
  STORE x28, 28*REGBYTES(sp)
  STORE x29, 29*REGBYTES(sp)
  STORE x30, 30*REGBYTES(sp)
  STORE x31, 31*REGBYTES(sp)
  
  csrr a0, mcause
  bgez a0, exception_trap

  # This is an interrupt.  Discard the mcause MSB and decode the rest.
  sll a0, a0, 1
  srl a0, a0, 1
  csrr a1, mepc
  # jump to the interrupt handler
  call interrupt_trap

  # Remain in M-mode after mret
  li t0, MSTATUS_MPP
  csrs mstatus, t0

.Lmret:
  LOAD x1, 1*REGBYTES(sp)
  LOAD x2, 2*REGBYTES(sp)
  LOAD x3, 3*REGBYTES(sp)
  LOAD x4, 4*REGBYTES(sp)
  LOAD x5, 5*REGBYTES(sp)
  LOAD x6, 6*REGBYTES(sp)
  LOAD x7, 7*REGBYTES(sp)
  LOAD x8, 8*REGBYTES(sp)
  LOAD x9, 9*REGBYTES(sp)
  LOAD x10, 10*REGBYTES(sp)
  LOAD x11, 11*REGBYTES(sp)
  LOAD x12, 12*REGBYTES(sp)
  LOAD x13, 13*REGBYTES(sp)
  LOAD x14, 14*REGBYTES(sp)
  LOAD x15, 15*REGBYTES(sp)
  LOAD x16, 16*REGBYTES(sp)
  LOAD x17, 17*REGBYTES(sp)
  LOAD x18, 18*REGBYTES(sp)
  LOAD x19, 19*REGBYTES(sp)
  LOAD x20, 20*REGBYTES(sp)
  LOAD x21, 21*REGBYTES(sp)
  LOAD x22, 22*REGBYTES(sp)
  LOAD x23, 23*REGBYTES(sp)
  LOAD x24, 24*REGBYTES(sp)
  LOAD x25, 25*REGBYTES(sp)
  LOAD x26, 26*REGBYTES(sp)
  LOAD x27, 27*REGBYTES(sp)
  LOAD x28, 28*REGBYTES(sp)
  LOAD x29, 29*REGBYTES(sp)
  LOAD x30, 30*REGBYTES(sp)
  LOAD x31, 31*REGBYTES(sp)

  addi sp, sp, 32*REGBYTES
  mret


.weak exception_trap
exception_trap:
1:
  j 1b

.weak main
main:
1:
  j 1b

.weak interrupt_trap
interrupt_trap:
1:
  j 1b

die:
1:
  j 1b

  .bss
  .align RISCV_PGSHIFT
stacks:
  .skip RISCV_PGSIZE * MAX_HARTS * 2
