
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

CFLAGS = -O3 -fopenmp # -g -Wall -ggdb

OBJ = utils.o init_kernel.o source_kernel.o comp_kernel.o poisson.o solution_kernel.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *.o poisson
