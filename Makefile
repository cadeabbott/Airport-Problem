CXX = g++
CXXFLAGS = -Wall -g

# Target executable
TARGET = airport_program

# Source files
SRCS = main.cpp trim_list.cpp calculate_distances.cpp read_distances.cpp depth_search.cpp common.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJS)
