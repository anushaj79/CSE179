CXX = g++-15
CXXFLAGS = -fopenmp -isysroot $(shell xcrun --show-sdk-path)
TARGET = openmp_simple

all: $(TARGET)

$(TARGET): openmp_simple.c
	$(CXX) $(CXXFLAGS) -o $(TARGET) openmp_simple.c

clean:
	rm -f $(TARGET)