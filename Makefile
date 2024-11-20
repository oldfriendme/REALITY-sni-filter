CXX = g++
CXXFLAGS = -pthread -static
TARGET = sni-filter
SRC = sni-filter.cpp

all: $(TARGET)
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
clean:
	rm -f $(TARGET)
