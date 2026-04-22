CXX = g++-15
CXXFLAGS = -fopenmp -isysroot $(shell xcrun --show-sdk-path) -O3

all: linked_omp_tasks linked_worksharing

linked_omp_tasks: linked_omp_tasks.c
	$(CXX) $(CXXFLAGS) -o linked_omp_tasks linked_omp_tasks.c

linked_worksharing: linked_worksharing.c
	$(CXX) $(CXXFLAGS) -o linked_worksharing linked_worksharing.c

clean:
	rm -f linked_omp_tasks linked_worksharing