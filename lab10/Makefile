CC = gcc
CFLAGS = -O3 -Wall
OMP_FLAGS = -fopenmp

SERIAL_TARGET = prims_serial
PARALLEL_TARGET = prims_parallel

SERIAL_SRC = prims_serial.c
PARALLEL_SRC = prims_parallel.c

all: $(SERIAL_TARGET) $(PARALLEL_TARGET)

$(SERIAL_TARGET): $(SERIAL_SRC)
	$(CC) $(CFLAGS) -o $(SERIAL_TARGET) $(SERIAL_SRC)

$(PARALLEL_TARGET): $(PARALLEL_SRC)
	$(CC) $(CFLAGS) $(OMP_FLAGS) -o $(PARALLEL_TARGET) $(PARALLEL_SRC)

clean:
	rm -f $(SERIAL_TARGET) $(PARALLEL_TARGET) adj_matrix.txt