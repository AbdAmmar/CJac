
TARGET = poisson
CC = gcc
CFLAG = -Wall -ggdb
OBJ = init_kernel.o source_kernel.o comp_kernel.o poisson.o

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

.PHONY: clean
clean:
	rm *.o poisson
