CC = mpicc
CFLAGS = -Wall 
TARGET = parallel_io

$(TARGET): $(TARGET).c parallel_io.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET) *.o *~ *#