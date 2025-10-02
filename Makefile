CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = lab1_program
SOURCE = main.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET) result.txt

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
