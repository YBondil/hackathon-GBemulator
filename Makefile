BUILD_DIR  = build
TEST_DIR   = build-tests
BIN        = $(BUILD_DIR)/gbemu
ROM        ?= roms/Tetris.gb

all: build

build:
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR) -j

run: build
	@./$(BIN) $(ROM)

test:
	@cmake -S tests -B $(TEST_DIR)
	@cmake --build $(TEST_DIR) -j
	@ctest --test-dir $(TEST_DIR) --output-on-failure

clean:
	@rm -rf $(BUILD_DIR) $(TEST_DIR)

.PHONY: all build run test clean
