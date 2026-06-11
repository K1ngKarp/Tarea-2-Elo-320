# ===== COMPILADOR =====
CC = gcc

# ===== FLAGS =====
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = -lm

# ===== DIRECTORIOS =====
SRC_DIR = src
OBJ_DIR = obj

# ===== ARCHIVOS =====
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/avl.c \
      $(SRC_DIR)/utils.c \
	  $(SRC_DIR)/hash.c \
	  $(SRC_DIR)/usuario.c

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# ===== EJECUTABLE =====
TARGET = vibenodes

# ===== REGLA PRINCIPAL =====
all: $(TARGET)

# ===== LINK FINAL =====
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# ===== COMPILAR .c A .o =====
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ===== LIMPIAR =====
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# ===== RECOMPILAR TODO =====
rebuild: clean all