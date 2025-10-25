CC = gcc
CFLAGS = -Wall -I./lib/include
LDFLAGS = -L./lib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = src/main.c src/snake.c
OBJ = $(SRC:.c=.o)
TARGET = snake.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGET)