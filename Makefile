CC := xc16-gcc
BIN2HEX := xc16-bin2hex
RM := rm -rf
MKDIR := mkdir -p
DEVICE := 24FJ128GB202
TARGET_NAME := boatcontroller

SRC_DIR ?= src
BUILD_DIR ?= build
DEP_DIR ?= $(BUILD_DIR)/deps
BIN_DIR ?= bin
TARGET_ELF := $(BIN_DIR)/$(TARGET_NAME).elf
TARGET := $(BIN_DIR)/$(TARGET_NAME).hex
CFLAGS += -Wall -Wextra -Werror
release: CFLAGS += -O1
debug: CFLAGS += -g
CFLAGS += -mcpu=$(DEVICE)
LDFLAGS := -Wl,-Map,$(BIN_DIR)/output.map,--report-mem -T p$(DEVICE).gld
DEPFLAGS = -MMD -MP -MF $(@:$(BUILD_DIR)/%.o=$(DEP_DIR)/%.d)

SRCS := fat16.c hal_sd.c main.c mbr.c mpu6050.c output.c radio.c rng.c \
		sd.c simple_controller.c status.c \
		mcc_generated_files/i2c1.c \
		mcc_generated_files/ic1.c \
		mcc_generated_files/ic2.c \
		mcc_generated_files/interrupt_manager.c \
		mcc_generated_files/mcc.c \
		mcc_generated_files/pin_manager.c \
		mcc_generated_files/spi1.c \
		mcc_generated_files/tmr2.c \
		mcc_generated_files/tmr3.c \
		mcc_generated_files/tmr4.c \
		mcc_generated_files/tmr5.c \
		mcc_generated_files/traps.c \
		mcc_generated_files/uart1.c
SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))

OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:$(BUILD_DIR)/%.o=$(DEP_DIR)/%.d)

.PHONY: all
all: release

.PHONY: release
release: $(TARGET)

.PHONY: debug
debug: $(TARGET)

.PHONY: build-dirs
build-dirs:
	$(MKDIR) $(sort $(addprefix $(BUILD_DIR)/, $(dir $(SRCS))))

.PHONY: dep-dirs
dep-dirs:
	$(MKDIR) $(sort $(addprefix $(DEP_DIR)/, $(dir $(SRCS))))

$(TARGET): build-dirs dep-dirs $(OBJS)
	$(if $(BIN_DIR),$(MKDIR) $(BIN_DIR),)
	$(CC) $(LDFLAGS) -o $(TARGET_ELF) $(OBJS)
	$(BIN2HEX) $(TARGET_ELF)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $(realpath $<) -o $@

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)
	$(RM) $(BIN_DIR)

-include $(DEPS)
