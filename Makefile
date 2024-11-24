# Compiler and flags
CC = gcc
CFLAGS = -o -Wall -lpthread

# Source and output files
SRC = HW04.c
OBJ = HW04.o
EXEC = HW04

# Targets

# Default target, builds the executable
all: $(EXEC)

# Rule to build the executable from object files
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -lpthread

# Rule to compile source code into object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

# Clean up compiled files
clean:
	rm -f $(OBJ) $(EXEC)

# Rebuild everything
rebuild: clean all

# Target to run the program after compiling
run: $(EXEC)
	./$(EXEC)
