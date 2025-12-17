CXX = g++
CXXFLAGS = -pthread -static
TARGET = sni-filter
SRC = sni-filter.cpp

all: $(TARGET)
$(TARGET): $(SRC)
	CGO_ENABLED=0; GOARCH=amd64; go build && mv sni-filter sni-filter-amd64
	CGO_ENABLED=0; GOARCH=arm64; go build && mv sni-filter sni-filter-arm64
clean:
	rm -f $(TARGET)

