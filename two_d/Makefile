
TARGET = poisson
CC = gcc
CFLAG = -O3 -g # -Wall -ggdb
OBJ = source_kernel.o comp_kernel.o poisson.o comm_kernel.o solution_kernel.o

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

.PHONY: clean
clean:
	rm *.o poisson
