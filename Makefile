V=1
SOURCE_DIR=src
BUILD_DIR=build
PROG_NAME=Retro Trophies 64

include $(N64_INST)/include/n64.mk

all: retro-trophies-64.z64
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
$(BUILD_DIR)/games/diddy_kong_racing.o \
$(BUILD_DIR)/games/super_mario_64.o \
$(BUILD_DIR)/games/super_smash_bros.o \
$(BUILD_DIR)/games/mario_kart_64.o \
$(BUILD_DIR)/games/zelda_oot.o

retro-trophies-64.z64: N64_ROM_TITLE="Retro Trophies 64"
retro-trophies-64.z64: $(BUILD_DIR)/retro-trophies-64.dfs

$(BUILD_DIR)/retro-trophies-64.elf: $(OBJS)
$(BUILD_DIR)/retro-trophies-64.dfs: $(wildcard filesystem/*)

clean:
	rm -f $(BUILD_DIR)/* *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)