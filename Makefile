CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -MMD -MP -g
TESTFLAGS = -fsanitize=address -fno-omit-frame-pointer
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
SRC_BUILD_DIR = $(BUILD_DIR)/$(SRC_DIR)
TEST_BUILD_DIR = $(BUILD_DIR)/$(TEST_DIR)

TARGET = zenc

SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(patsubst $(SRC_DIR)/%.c,$(SRC_BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

TEST_SRCS := $(shell find $(TEST_DIR) -name "*.c")
TEST_OBJS := $(patsubst $(SRC_DIR)/%.c,$(TEST_BUILD_DIR)/$(SRC_DIR)/%.o,$(filter-out $(SRC_DIR)/main.c,$(SRCS)))
TEST_TARGETS := $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR)/$(TEST_DIR)/%,$(TEST_SRCS))

all: $(TARGET)
tests: $(TEST_TARGETS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SRC_BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BUILD_DIR)/$(TEST_DIR)/%: $(TEST_DIR)/%.c $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TESTFLAGS) -Ideps $^ -o $@

$(TEST_BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(TESTFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)

.PHONY: all clean
