CXX = g++ #compiler is g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic #compiler is correct version of c++

TARGET = calculator #represents program name

SRCS = double_calc.cpp #represents source file

all: $(TARGET) #represents default target

$(TARGET): $(SRCS) #(lines 10-11) builds calculator program
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET) #(lines 13-22) runs target with numbers.txt file, if numbers.txt doesn't exist, creates numbers.txt with same values in numbers.txt
	@if [ -f numbers.txt ]; then \
		echo "Using existing numbers.txt file"; \
		./$(TARGET) numbers.txt; \
	else \
		echo "Creating numbers.txt with test cases..."; \
		echo -e "1\n12345\n1.0\n+1\n-1\n001\n+001\n-001\n+1.0\n-1.0\n001.0\n+001.0\n-001.0\nA\n25..02\n000025.20000\n+-21.1\n-1.2000\n+09.020\n351+" > numbers.txt; \
		echo "Running program with test cases..."; \
		./$(TARGET) numbers.txt; \
	fi

clean: #cleans up build
	rm -f $(TARGET) numbers.txt

.PHONY: all run clean #sets phony targets