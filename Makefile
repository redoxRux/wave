CXX = g++
CXXFLAGS = -std=c++11 -O2
TARGET = wave
SRC = wave.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "✓ Installed to /usr/local/bin/wave"
	@echo "Add 'wave' to your ~/.bashrc or ~/.zshrc"

clean:
	rm -f $(TARGET)

.PHONY: all install clean
