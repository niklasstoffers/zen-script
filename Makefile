CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -MMD -MP -g
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build
TARGET = zenc

SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)

.PHONY: all clean
