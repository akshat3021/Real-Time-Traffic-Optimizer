CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -O2

SRC = src/main.c src/graph.c src/algorithm.c
OBJ = src/main.o src/graph.o src/algorithm.o
TARGET = traffic_optimizer.exe

# Clean up previous builds (use del for Windows compatibility)
clean:
	del src\*.o $(TARGET)

# Main build rule
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compile each .c file into a .o object file
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean
