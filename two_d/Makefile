
CC = mpicc
CFLAGS = -O3 -fopenmp # -g -Wall -ggdb

SRC_DIR = src
INC_DIR = include

BLD_DIR = build
BIN_DIR = bin


TARGET = $(BIN_DIR)/poisson

all: $(TARGET)

SRC = $(SRC_DIR)/utils.c \
      $(SRC_DIR)/init_kernel.c \
      $(SRC_DIR)/source_kernel.c \
      $(SRC_DIR)/comp_kernel.c \
      $(SRC_DIR)/poisson.c \
      $(SRC_DIR)/solution_kernel.c

OBJ = $(BLD_DIR)/utils.o \
      $(BLD_DIR)/init_kernel.o \
      $(BLD_DIR)/source_kernel.o \
      $(BLD_DIR)/comp_kernel.o \
      $(BLD_DIR)/poisson.o \
      $(BLD_DIR)/solution_kernel.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -I$(INC_DIR)

$(OBJ): $(SRC)
	@for src in $(SRC); do \
		obj=$(BLD_DIR)/$$(basename $${src} .c).o; \
		echo "$(CC) $(CFLAGS) -c $$src -o $$obj -I$(INC_DIR)"; \
		$(CC) $(CFLAGS) -c $$src -o $$obj -I$(INC_DIR); \
	done

.PHONY: clean
clean:
	rm -rf $(BLD_DIR)/*.o $(BIN_DIR)/poisson

