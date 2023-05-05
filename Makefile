V=1
SOURCE_DIR=src
BUILD_DIR=build
PROG_NAME=N64 Trophies

include $(N64_INST)/include/n64.mk

all: n64-trophies.z64
.PHONY: all

OBJS = $(BUILD_DIR)/main.o \
$(BUILD_DIR)/controller_ext.o \
$(BUILD_DIR)/graphics_ext.o \
$(BUILD_DIR)/trophy.o \
$(BUILD_DIR)/game.o \
$(BUILD_DIR)/debug.o \
$(BUILD_DIR)/save_state_utils.o \
$(BUILD_DIR)/save_type.o \
$(BUILD_DIR)/list_selection.o \
$(BUILD_DIR)/games/1080_snowboarding.o \
$(BUILD_DIR)/games/super_mario_64.o \
$(BUILD_DIR)/games/super_smash_bros.o \
$(BUILD_DIR)/games/mario_kart_64.o \
$(BUILD_DIR)/games/zelda_oot.o

n64-trophies.z64: N64_ROM_TITLE="N64 Trophies"
n64-trophies.z64: $(BUILD_DIR)/n64-trophies.dfs

$(BUILD_DIR)/n64-trophies.elf: $(OBJS)
$(BUILD_DIR)/n64-trophies.dfs: $(wildcard filesystem/*)

clean:
	rm -f $(BUILD_DIR)/* *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)