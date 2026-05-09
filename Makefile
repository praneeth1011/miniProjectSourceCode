CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = banking_system

SOURCES = banking.c auth.c fileops.c transactions.c admin.c utils.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = banking.h

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Clean everything including generated files
distclean: clean
	rm -f accounts.dat transactions.log sessions.log system.log accounts.txt

# Run the program
run: $(TARGET)
	./$(TARGET)

# Rebuild everything
rebuild: clean all

# Help
help:
	@echo "Available targets:"
	@echo "  make all       - Build the program"
	@echo "  make clean     - Remove object files and executable"
	@echo "  make distclean - Remove all generated files"
	@echo "  make run       - Build and run the program"
	@echo "  make rebuild   - Clean and build from scratch"

.PHONY: all clean distclean run rebuild help
