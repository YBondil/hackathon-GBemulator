BUILD_DIR = build
BIN       = $(BUILD_DIR)/gbemu
ROM       ?= roms/Tetris.gb

all: build

build:
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR) -j

run: build
	@./$(BIN) $(ROM)

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all build run clean
