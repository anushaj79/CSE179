CC = mpicc
CFLAGS = -O3
TARGET = matmul_mpi

all: $(TARGET)

$(TARGET): matmul_mpi.c
	$(CC) $(CFLAGS) -o $(TARGET) matmul_mpi.c

clean:
	rm -f $(TARGET)