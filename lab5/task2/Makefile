CC = mpicc
CFLAGS = -Wall 
TARGET = MPI_ring

$(TARGET): $(TARGET).c MPI_ring.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET) *.o *~ *#