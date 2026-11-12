MCU := M030
PART := DPM32M030
HAL_FOLDER_$(MCU) := $(HAL_FOLDER)/m030

MCU_$(MCU) := -mcpu=cortex-m0 -mthumb
$(MCU)_LDSCRIPT := Mcu/m030/ldscript_bl.ld
$(MCU)_LDSCRIPT_BLU := Mcu/m030/ldscript_blu.ld
CFLAGS_$(MCU) := -DDPM32M030 -IMcu/m030/Inc \
    -IMcu/m030/Drivers/CMSIS/Include -IMcu/m030/Drivers/Library/inc

# Application control sources copied into Src do not belong in this build.
SRC_$(MCU)_BL := Mcu/m030/Startup/GCC/startup_dpm32m030.s \
    Mcu/m030/Startup/system_dpm32m0xx.c Mcu/m030/Src/eeprom.c \
    Mcu/m030/Drivers/Library/src/dpm32m0xx_flash.c \
    Mcu/m030/Drivers/Library/src/dpm32m0xx_gpio.c \
    Mcu/m030/Drivers/Library/src/dpm32m0xx_rcc.c

BOOTLOADER_PINS_$(MCU) := PD0
# Do not create the STM32 debug bundle: use the named ELF and a DPM-aware probe.
NO_DEBUG_$(MCU) := 1
