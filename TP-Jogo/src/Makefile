# O nome do executável final
TARGET = snake

# Os arquivos-objeto necessários para criar o executável
OBJS = main.o snake.o -lm -lraylib

# O compilador que será usado
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra -std=c99

# Regra 'all': a regra padrão, que compila o projeto
all: $(TARGET)

# Regra para criar o executável final (TARGET)
# Depende dos arquivos-objeto (.o)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Regra para compilar o arquivo main.c em main.o
# Depende de main.c
main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

# Regra para compilar o arquivo snake.c em snake.o
# Depende de snake.c e snake.h
snake.o: snake.c snake.h
	$(CC) $(CFLAGS) -c snake.c

# Regra 'run': executa o programa
run: $(TARGET)
	./$(TARGET)

# Regra 'clean': remove todos os arquivos compilados (objetos e executável)
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run


