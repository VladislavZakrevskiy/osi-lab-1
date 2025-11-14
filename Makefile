CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = lab1_program
SOURCES = main.c parent.c child.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) result.txt

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
