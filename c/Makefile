CC = gcc
CFLAGS = -O2 -Wall -Wextra -g -Wpedantic
COFLAGS = $(CFLAGS) -c
SRC_DIR = ./src
TARGET_DIR = ./target

all: target objects
	$(CC) $(CFLAGS) $(TARGET_DIR)/*.o main.c -o $(TARGET_DIR)/brainfrick

objects: target
	for src_file in $$(find $(SRC_DIR) -name "*.c") ; do \
		module_name=$$(basename -- $$src_file .c) ; \
		$(CC) $(COFLAGS) $$src_file -o $(TARGET_DIR)/$$module_name.o ; \
	done

target:
	mkdir -p $(TARGET_DIR)

clean:
	rm -rf $(TARGET_DIR)/*
