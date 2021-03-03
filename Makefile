PREFIX?=arm-none-eabi-
CC=$(PREFIX)gcc
OBJCOPY=$(PREFIX)objcopy

SFLAGS= --static -nostartfiles -std=c11 -g3 -Os
SFLAGS+= -fno-common -ffunction-sections -fdata-sections
SFLAGS+= -I./libopencm3/include -L./libopencm3/lib
LFLAGS+=-Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

M4FH_FLAGS= $(SFLAGS) -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

LFLAGS_STM32=$(LFLAGS) stm32mp1.c -T ld.stm32mp1.basic

STM32MP1_CFLAGS=$(M4FH_FLAGS) -DSTM32MP1 $(LFLAGS_STM32) -lopencm3_stm32mp1

stm32mp1.elf: stm32mp1.c libopencm3/lib/libopencm3_stm32mp1.a
	$(CC) $(STM32MP1_CFLAGS) -o stm32mp1.elf

libopencm3/Makefile:
	@echo "Initializing libopencm3 submodule"
	git submodule update --init

libopencm3/lib/libopencm3_%.a: libopencm3/Makefile
	$(MAKE) -C libopencm3

%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.hex: %.elf
	@#printf "  OBJCOPY $(*).hex\n"
	$(OBJCOPY) -Oihex $(*).elf $(*).hex

clean:
	$(RM) stm32mp1.elf

.DEFAULT := stm32mp1.elf