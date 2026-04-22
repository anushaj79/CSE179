CXX = gcc-15
CXXFLAGS = -Wall -fopenmp -isysroot $(shell xcrun --show-sdk-path)

TARGET = pi_estimation

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)