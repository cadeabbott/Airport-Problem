CXX = g++
CXXFLAGS = -Wall -g

TARGET = airport_program
OBJS = main.o trim_list.o calculate_distances.o common.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

trim_list.o: trim_list.cpp
	$(CXX) $(CXXFLAGS) -c trim_list.cpp

calculate_distances.o: calculate_distances.cpp
	$(CXX) $(CXXFLAGS) -c calculate_distances.cpp

common.o: common.cpp
	$(CXX) $(CXXFLAGS) -c common.cpp

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
