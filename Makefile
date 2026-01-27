CC=gcc
CFLAGS=-Wall -Wextra

SRC=src
OBJ=build
INC=inc

TARGET=redis


SRC_FILES:= $(wildcard $(SRC)/*.c)
OBJ_FILES:= $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

clean:
	rm -rf $(OBJ)/*.o $(TARGET)

.PHONY: all clean
