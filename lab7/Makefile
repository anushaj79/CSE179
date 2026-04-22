CC = nvcc

CFLAGS = -O3

TARGET = findMax

$(TARGET): $(TARGET).cu
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cu

clean:
	rm -f $(TARGET)