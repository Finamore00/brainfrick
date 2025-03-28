CC = gcc
CFLAGS = -std=c23 -O2 -Wall -Wextra -Wshadow -Wdouble-promotion -Wfloat-equal -Wpedantic
COFLAGS = $(CFLAGS) -c
SRC_DIR = ./src
TARGET_DIR = ./target
BINARY_NAME = brainfrick

all: target_dir
	$(CC) $(CFLAGS) $(TARGET_DIR)/*.o main.c -o $(TARGET_DIR)/$(BINARY_NAME)

target_dir:
	mkdir $(TARGET_DIR)

clean: target_dir
	rm -rf $(TARGET_DIR)/*
