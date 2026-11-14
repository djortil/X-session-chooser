CC = gcc
CFLAGS = -Wall -Wextra
SRC = ./src
BUILD = ./build

main.c:
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(BUILD)/program

clean:
	rm -rf build
