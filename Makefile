TARGET = vibenodes

# Directorios
SRC_DIR = src
INCLUDE_DIR = include

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -I$(INCLUDE_DIR)
LDFLAGS = -lm

# Archivos fuente
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Regla principal
all: $(TARGET)

# Enlazado (el ejecutable se crea en la raíz)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Compilación de cada archivo objeto
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Limpieza
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(TARGET)

# Limpieza profunda
distclean: clean
	rm -f usuarios_hashed*.csv
	rm -f grafo_similitud*.csv

# Reglas de ejecución rápida
run: all
	./$(TARGET) -lista -user data/

run-matriz: all
	./$(TARGET) -matriz -user data/

run-admin: all
	./$(TARGET) -lista -admin data/

# Valgrind
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) -lista -user data/

.PHONY: all clean distclean run run-matriz run-admin valgrind