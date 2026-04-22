CXX = g++-15
CXXFLAGS = -fopenmp -isysroot $(shell xcrun --show-sdk-path)
TARGET = prefix_sums

all: $(TARGET)

$(TARGET): prefix_sum.c
	$(CXX) $(CXXFLAGS) -o $(TARGET) prefix_sum.c

clean:
	rm -f $(TARGET)