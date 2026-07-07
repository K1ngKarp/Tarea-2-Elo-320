CC = gcc

CFLAGS = -Wall -g -Iinclude
CFLAGS = -Wall -g -Iinclude -std=c99

LDFLAGS = -lm

TARGET = VibeNodes

SRC_DIR = src

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/avl.c \
       $(SRC_DIR)/grafo.c \
       $(SRC_DIR)/hash.c \
       $(SRC_DIR)/reproductor.c \
       $(SRC_DIR)/historial.c \
       $(SRC_DIR)/usuario.c \
       $(SRC_DIR)/utils.c \


OBJS = $(SRCS:.c=.o)
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean