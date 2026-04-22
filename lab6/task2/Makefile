CC = mpicc
CFLAGS = -Wall 
TARGET = topology

$(TARGET): $(TARGET).c topology.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET) *.o *~ *#