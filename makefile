# Compiler
CC := gcc

# Compiler flags for release mode
CFLAGS_RELEASE := -Wall -Wextra -O3

# Compiler flags for debug mode
CFLAGS_DEBUG := -Wall -Wextra -g

ifeq ($(OS), Windows_NT)	
	# Library paths
	LIBRARY_PATHS := -L./lib/windows
	# Linker flags
	LDFLAGS := -lglfw3dll -lopengl32
	# Target directory
	TARGET_DIR := build/windows
else
	# Library paths
	LIBRARY_PATHS := -L./lib/linux
	# Linker flags
	LDFLAGS := -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
	# Target directory
	TARGET_DIR := build/linux
endif


# Include directories
INCLUDES := -I./include

# Source files directory
SRC_DIR := src

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Object files directory
OBJ_DIR := obj

# Object files
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))



# Binary output
TARGET := main

# Default target
all: release

# Release mode
release: CFLAGS := $(CFLAGS_RELEASE)
release: $(TARGET)

# Debug mode
debug: CFLAGS := $(CFLAGS_DEBUG)
debug: $(TARGET)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link object files into the final binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LIBRARY_PATHS) $^ -o $(TARGET_DIR)/$@ $(LDFLAGS)

# Clean build artifacts
clean:
ifeq ($(OS),Windows_NT)
	@del /Q $(TARGET_DIR)/$(TARGET).exe 2>NUL || exit 0
	@del /Q $(OBJ_DIR)\* 2>NUL || exit 0
else
	rm -f $(OBJ_DIR)/*.o $(TARGET_DIR)/$(TARGET)
endif

bear: clean
	@bear -- make -n > /dev/null || true
	@bear -- make

.PHONY: all release debug clean bear
