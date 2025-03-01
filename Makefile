CC = clang
CFLAGS = -std=c18 -Wall -Wextra -Werror -pedantic
TARGET = c_vector
SRC = src/main.c src/vector/operations.c src/deamortized_vector/operations.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

