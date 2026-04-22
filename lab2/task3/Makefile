CXX = g++-15
CXXFLAGS = -fopenmp -isysroot $(shell xcrun --show-sdk-path)
TARGET = matrix_vec_mul

all: $(TARGET)

$(TARGET): matrix_vec_mul.c
	$(CXX) $(CXXFLAGS) -o $(TARGET) matrix_vec_mul.c

clean:
	rm -f $(TARGET)