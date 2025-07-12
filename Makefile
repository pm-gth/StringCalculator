CC = gcc
BUILD_DIR = build

HEADERS = -I include/ -I src/calculator/
SOURCEFILES = $(wildcard src/main.c src/calculator/*.c src/myNewStrings/*.c)
TARGET = $(BUILD_DIR)/StringCalculator

RELEASE_FLAGS =
DEBUG_FLAGS = -g -fsanitize=address

TEST_SRCS = $(wildcard test/*.c)
TEST_HDRS = -I include/ -I src/calculator/ -I third_party/
TEST_BIN = $(patsubst test/%.c, $(BUILD_DIR)/%, $(TEST_SRCS))

CALC_SRCS = $(wildcard src/calculator/*.c)
MYSTR_SRCS = $(wildcard src/myNewStrings/*.c)
UNITY_SRC = third_party/unity.c

.PHONY: release debug clean test run_tests

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

release: $(BUILD_DIR)
	$(CC) $(RELEASE_FLAGS) $(HEADERS) $(SOURCEFILES) -o $(TARGET)

debug: $(BUILD_DIR)
	$(CC) $(DEBUG_FLAGS) $(HEADERS) $(SOURCEFILES) -o $(TARGET)

test: $(BUILD_DIR) $(TEST_BIN)

$(BUILD_DIR)/%: test/%.c $(CALC_SRCS) $(MYSTR_SRCS) $(UNITY_SRC)
	$(CC) $(TEST_HDRS) $^ -o $@

run_tests: test
	@echo "Running tests..."
	@for bin in $(TEST_BIN); do \
		echo "==> $$bin"; \
		./$$bin || exit 1; \
	done

clean:
	rm -rf $(BUILD_DIR)
