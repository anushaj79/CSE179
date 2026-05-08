CC = gcc
CFLAGS = -O3 -Wall
OMP_FLAGS = -fopenmp

SERIAL = prims_serial
PARALLEL = prims_parallel

all: $(SERIAL) $(PARALLEL)

$(SERIAL): prims_serial.c
	$(CC) $(CFLAGS) -o $(SERIAL) prims_serial.c

$(PARALLEL): prims_parallel.c
	$(CC) $(CFLAGS) $(OMP_FLAGS) -o $(PARALLEL) prims_parallel.c

clean:
	rm -f $(SERIAL) $(PARALLEL)