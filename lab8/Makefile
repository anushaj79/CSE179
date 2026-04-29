CC = nvcc
CFLAGS = -O3
TARGET = biology_sim

all: $(TARGET)

$(TARGET): biology_sim.cu
	$(CC) $(CFLAGS) -o $(TARGET) biology_sim.cu

clean:
	rm -f $(TARGET)