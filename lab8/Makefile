CC = nvcc
CFLAGS = -O3
TARGET = bio_sim

all: $(TARGET)

$(TARGET): bio_sim.cu
	$(CC) $(CFLAGS) -o $(TARGET) bio_sim.cu

clean:
	rm -f $(TARGET)