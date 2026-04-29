CC = nvcc
CFLAGS = -O3
TARGET = bio_sim

all: $(TARGET)

$(TARGET): $(TARGET).cu
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cu

clean:
	rm -f $(TARGET)