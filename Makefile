case  ?=uart


OPT   := -O2 -g 
OPT   := -march=rv64imac -mabi=lp64 -mcmodel=medany $(OPT)

BUILD     = build
BUILDDIR  = $(BUILD)/$(case)
OBJDIR    = $(BUILDDIR)/obj
LSTDIR    = $(BUILDDIR)/lst

project_name := riscv-$(case)
src_dir      := .
inc_dir      := .
c_srcs       :=
asm_srcs     :=
ld_script    := "linker.lds"
LIBS         :=

include util/util.mk
include platform/platform.mk
include bsp/bsp.mk
include case/$(case)/$(case).mk

OUTFILES = $(BUILDDIR)/$(project_name).elf \
           $(BUILDDIR)/$(project_name).hex \
           $(BUILDDIR)/$(project_name).bin \
           $(BUILDDIR)/$(project_name).bin.text \
           $(BUILDDIR)/$(project_name).dmp \
           $(BUILDDIR)/$(project_name).s

#-------------------------------------------------------------------------
# Programs and flags 
#-------------------------------------------------------------------------
OPT   += -ffunction-sections -fdata-sections -fno-common -fno-delete-null-pointer-checks
LDOPT += --data-sections -T $(ld_script)

CC            := riscv64-unknown-elf-gcc
READELF       := riscv64-unknown-elf-readelf
OBJCOPY       := riscv64-unknown-elf-objcopy
OBJDUMP       := riscv64-unknown-elf-objdump
AR            := riscv64-unknown-elf-ar
RANLIB        := riscv64-unknown-elf-ranlib
LD            := $(CC)

RUN           := @RUN@
RUNFLAGS      := @RUNFLAGS@

SRCPATHS   = $(sort $(dir $(c_srcs)) $(dir $(asm_srcs)))
IINC_DIR   = $(patsubst %,-I%,$(inc_dir))
COBJS      = $(addprefix $(OBJDIR)/, $(notdir $(c_srcs:.c=.o)))
ASMOBJS    = $(addprefix $(OBJDIR)/, $(notdir $(asm_srcs:.S=.o)))
OBJS       = $(COBJS) $(ASMOBJS)

CFLAGS        := -Wall -Werror -D__NO_INLINE__ -mcmodel=medany -std=gnu99 -Wno-unused -Wno-attributes $(OPT) $(CFLAGS) $(LDFLAGS) $(IINC_DIR) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"
ASFLAGS       := -x assembler-with-cpp $(CFLAGS) $(ASFLAGS) $(IINC_DIR) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"
LDFLAGS       :=  -nostartfiles -nostdlib $(OPT) $(LDFLAGS) $(LIBS) $(LDOPT)

VPATH     = $(SRCPATHS)

all: PRE_MAKE_ALL_RULE_HOOK $(OBJS) $(OUTFILES) POST_MAKE_ALL_RULE_HOOK

PRE_MAKE_ALL_RULE_HOOK: $(BUILDDIR) $(OBJDIR) $(LSTDIR)
	@echo start compile....

POST_MAKE_ALL_RULE_HOOK:
	@echo 
	@echo --------  Done  --------


$(BUILDDIR):
	@echo create $@
	@mkdir -p $(BUILDDIR)

$(OBJDIR):
	@echo create $@
	@mkdir -p $(OBJDIR)

$(LSTDIR):
	@echo create $@
	@mkdir -p $(LSTDIR)

$(ASMOBJS): $(OBJDIR)/%.o: %.S
	@echo Compiling $<
	@$(CC) $(ASFLAGS) $< -o $@

$(COBJS): $(OBJDIR)/%.o: %.c
	@echo Compiling $<
	@$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR)/$(project_name).elf: $(OBJS)
	@echo Linking $@ 
	@$(LD) $(OBJS) $(LDFLAGS) -o $@

%.hex: %.elf
	@echo Creating $@
	@$(OBJCOPY) -O ihex $< $@

%.bin: %.elf
	@echo Creating $@
	@$(OBJCOPY) -O binary $< $@

%.dmp: %.elf
	@echo Creating $@
	@$(OBJDUMP) -dx $< > $@

%.s: %.elf
	@echo Creating $@
	@$(OBJDUMP) -S $< > $@

%.bin.text: %.bin
	@echo Creating $@
	@hexdump -ve '"%08x\n"' $< >$@

clean:
	@rm -rf $(BUILDDIR) ./-MT
	@echo clean done


