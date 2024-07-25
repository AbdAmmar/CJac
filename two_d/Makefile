
#HOSTNAME = MYPC
HOSTNAME = TURPAN

ifeq ($(HOSTNAME), MYPC)
	CC = /usr/bin/mpicc
else ifeq ($(HOSTNAME), TURPAN)
	CC = /usr/local/openmpi/arm/4.1.4-cpu/bin/mpicc
else
	CC = gcc
endif


TARGET = poisson

CFLAG = -O3 -g # -Wall -ggdb

OBJ = utils.o init_kernel.o source_kernel.o comp_kernel.o poisson.o solution_kernel.o

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

.PHONY: clean
clean:
	rm *.o poisson
