###############################################################################
# Makefile for F28335 Buck-Boost Controller
# Uses TI C2000 GCC toolchain
###############################################################################

# Project name
PROJECT = buckboost_f28335

# Toolchain prefix (adjust to your installation)
TOOLCHAIN_PREFIX ?= c2000-elf-

# Compiler and tools
CC = $(TOOLCHAIN_PREFIX)gcc
AS = $(TOOLCHAIN_PREFIX)as
LD = $(TOOLCHAIN_PREFIX)ld
AR = $(TOOLCHAIN_PREFIX)ar
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
SIZE = $(TOOLCHAIN_PREFIX)size

# Directories
ROOT_DIR = ..
FIRMWARE_DIR = $(ROOT_DIR)
APP_DIR = $(FIRMWARE_DIR)/app
DRIVER_DIR = $(FIRMWARE_DIR)/drivers
BSP_DIR = $(FIRMWARE_DIR)/bsp
LIB_DIR = $(FIRMWARE_DIR)/lib
HAL_STUB_DIR = $(LIB_DIR)/hal_stub
FREERTOS_DIR ?= $(LIB_DIR)/FreeRTOS
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Source files
APP_SOURCES = $(wildcard $(APP_DIR)/*.c)
DRIVER_SOURCES = $(wildcard $(DRIVER_DIR)/*.c)
HAL_STUB_SOURCES = $(wildcard $(HAL_STUB_DIR)/*.c)
STARTUP_SOURCES = startup.c

ALL_SOURCES = $(APP_SOURCES) $(DRIVER_SOURCES) $(HAL_STUB_SOURCES) $(STARTUP_SOURCES)

# Object files
APP_OBJECTS = $(patsubst $(APP_DIR)/%.c,$(OBJ_DIR)/app/%.o,$(APP_SOURCES))
DRIVER_OBJECTS = $(patsubst $(DRIVER_DIR)/%.c,$(OBJ_DIR)/drivers/%.o,$(DRIVER_SOURCES))
HAL_STUB_OBJECTS = $(patsubst $(HAL_STUB_DIR)/%.c,$(OBJ_DIR)/hal_stub/%.o,$(HAL_STUB_SOURCES))
STARTUP_OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(STARTUP_SOURCES))

ALL_OBJECTS = $(APP_OBJECTS) $(DRIVER_OBJECTS) $(HAL_STUB_OBJECTS) $(STARTUP_OBJECTS)

# Include directories
INCLUDES = -I$(APP_DIR) \
           -I$(DRIVER_DIR) \
           -I$(BSP_DIR) \
           -I$(HAL_STUB_DIR) \
           -I$(LIB_DIR) \
           -I$(LIB_DIR)/hal_stub

# Add FreeRTOSConfig.h path if it exists
ifneq ($(wildcard $(LIB_DIR)/FreeRTOSConfig.h),)
    INCLUDES += -I$(LIB_DIR)
endif

# FreeRTOS includes (if available)
FREERTOS_AVAILABLE = 0
ifneq ($(wildcard $(FREERTOS_DIR)),)
    FREERTOS_SRC_DIR = $(FREERTOS_DIR)/Source
    FREERTOS_PORT_DIR = $(FREERTOS_DIR)/Source/portable/GCC/C2000
    FREERTOS_MEM_DIR = $(FREERTOS_DIR)/Source/portable/MemMang
    INCLUDES += -I$(FREERTOS_DIR)/Source/include \
                -I$(FREERTOS_PORT_DIR)
    
    # FreeRTOS source files (if available)
    FREERTOS_SOURCES = $(wildcard $(FREERTOS_SRC_DIR)/*.c) \
                       $(wildcard $(FREERTOS_PORT_DIR)/*.c) \
                       $(wildcard $(FREERTOS_MEM_DIR)/heap_4.c)
    FREERTOS_OBJECTS = $(patsubst $(FREERTOS_DIR)/%.c,$(OBJ_DIR)/freertos/%.o,$(FREERTOS_SOURCES))
    ALL_SOURCES += $(FREERTOS_SOURCES)
    ALL_OBJECTS += $(FREERTOS_OBJECTS)
    
    FREERTOS_AVAILABLE = 1
endif

# Compiler flags
CFLAGS = -mcpu=28 -mbig-endian -ml -mregparm=0 \
         -Wall -Wextra -Wno-unused-parameter \
         -ffunction-sections -fdata-sections \
         -O2 -g \
         $(INCLUDES) \
         -DF28335

# Add FreeRTOS define if available
ifeq ($(FREERTOS_AVAILABLE),1)
    CFLAGS += -DFREERTOS_AVAILABLE
endif

# Assembler flags
ASFLAGS = -mcpu=28 -mbig-endian

# Linker flags
LDFLAGS = -T F28335.ld \
          -Wl,--gc-sections \
          -Wl,--print-map \
          -Wl,-Map=$(BIN_DIR)/$(PROJECT).map \
          -nostdlib \
          -lgcc

# Linker script
LDSCRIPT = F28335.ld

# Output files
ELF_FILE = $(BIN_DIR)/$(PROJECT).elf
HEX_FILE = $(BIN_DIR)/$(PROJECT).hex
BIN_FILE = $(BIN_DIR)/$(PROJECT).bin

# Default target
all: $(ELF_FILE) $(HEX_FILE) $(BIN_FILE)
	@echo "Build complete: $(ELF_FILE)"
	@$(SIZE) $(ELF_FILE)

# Create directories
$(OBJ_DIR)/app:
	@mkdir -p $(OBJ_DIR)/app

$(OBJ_DIR)/drivers:
	@mkdir -p $(OBJ_DIR)/drivers

$(OBJ_DIR)/hal_stub:
	@mkdir -p $(OBJ_DIR)/hal_stub

$(OBJ_DIR)/freertos:
	@mkdir -p $(OBJ_DIR)/freertos

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compile C files
$(OBJ_DIR)/app/%.o: $(APP_DIR)/%.c | $(OBJ_DIR)/app
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/drivers/%.o: $(DRIVER_DIR)/%.c | $(OBJ_DIR)/drivers
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/hal_stub/%.o: $(HAL_STUB_DIR)/%.c | $(OBJ_DIR)/hal_stub
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/freertos/%.o: $(FREERTOS_DIR)/%.c | $(OBJ_DIR)/freertos
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link
$(ELF_FILE): $(ALL_OBJECTS) $(LDSCRIPT) | $(BIN_DIR)
	@echo "LD $@"
	@$(CC) $(CFLAGS) $(ALL_OBJECTS) -o $@ $(LDFLAGS)

# Generate hex file
$(HEX_FILE): $(ELF_FILE)
	@echo "OBJCOPY $@"
	@$(OBJCOPY) -O ihex $< $@

# Generate binary file
$(BIN_FILE): $(ELF_FILE)
	@echo "OBJCOPY $@"
	@$(OBJCOPY) -O binary $< $@

# Clean
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

# Flash using OpenOCD (adjust interface and target as needed)
flash: $(ELF_FILE)
	@echo "Flashing $(ELF_FILE)..."
	@openocd -f interface/ti-icdi.cfg -f target/ti_f2833x.cfg \
	         -c "program $(ELF_FILE) verify reset exit" || \
	 echo "OpenOCD not found. Please install OpenOCD or use alternative flashing method."

# Flash using DSLite (TI's command-line tool)
flash-dslite: $(HEX_FILE)
	@echo "Flashing $(HEX_FILE) using DSLite..."
	@if [ -z "$(DSLITE)" ]; then \
		echo "Error: DSLITE environment variable not set"; \
		echo "Set it to: export DSLITE=/path/to/ccs_base/ccs_base/common/uscif/dslite"; \
		exit 1; \
	fi
	@if [ -z "$(CCXML)" ]; then \
		echo "Error: CCXML environment variable not set"; \
		echo "Set it to: export CCXML=/path/to/F28335.ccxml"; \
		exit 1; \
	fi
	@$(DSLITE) flash -e -f $(HEX_FILE) -c $(CCXML) || \
	 echo "DSLite flash failed. Check your CCXML configuration."

# Disassemble
disasm: $(ELF_FILE)
	@$(OBJDUMP) -d $(ELF_FILE) > $(BIN_DIR)/$(PROJECT).dis

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the project (default)"
	@echo "  clean        - Remove build files"
	@echo "  flash        - Flash using OpenOCD"
	@echo "  flash-dslite - Flash using TI DSLite"
	@echo "  disasm       - Generate disassembly"
	@echo "  help         - Show this help"
	@echo ""
	@echo "Environment variables:"
	@echo "  TOOLCHAIN_PREFIX - GCC toolchain prefix (default: c2000-elf-)"
	@echo "  FREERTOS_DIR     - Path to FreeRTOS directory (default: ../lib/FreeRTOS)"
	@echo "  DSLITE          - Path to DSLite tool (for flash-dslite)"
	@echo "  CCXML           - Path to CCXML config file (for flash-dslite)"
	@echo ""
	@if [ -z "$(wildcard $(FREERTOS_DIR))" ]; then \
		echo "Warning: FreeRTOS not found at $(FREERTOS_DIR)"; \
		echo "         Set FREERTOS_DIR to point to your FreeRTOS installation"; \
	fi

.PHONY: all clean flash flash-dslite disasm help

