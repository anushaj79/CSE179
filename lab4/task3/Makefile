CC = gcc-15
CFLAGS = -Wall -lpthread -isysroot $(shell xcrun --show-sdk-path)
TARGET = rwlock

$(TARGET): $(TARGET).c rwlock.h
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET) *.o *~ *#