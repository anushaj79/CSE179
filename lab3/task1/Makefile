CXX = g++-15
CXXFLAGS = -fopenmp -isysroot $(shell xcrun --show-sdk-path)
TARGET = prod_cons

all: $(TARGET)

$(TARGET): prod_cons.c
	$(CXX) $(CXXFLAGS) -o $(TARGET) prod_cons.c

clean:
	rm -f $(TARGET)