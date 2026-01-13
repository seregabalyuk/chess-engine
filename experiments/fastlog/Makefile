CXX = g++
CXXFLAGS = -O3 -march=native -Wall

all: benchmark check_powers benchmark_powers

benchmark: main.cpp
	$(CXX) $(CXXFLAGS) -o benchmark main.cpp

check_powers: check_powers.cpp
	$(CXX) $(CXXFLAGS) -o check_powers check_powers.cpp

benchmark_powers: benchmark_powers.cpp
	$(CXX) $(CXXFLAGS) -o benchmark_powers benchmark_powers.cpp

clean:
	rm -f benchmark check_powers benchmark_powers
