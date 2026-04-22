CC = mpicc
CFLAGS = -Wall 
TARGET = collective

$(TARGET): $(TARGET).c collective.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET) *.o *~ *#