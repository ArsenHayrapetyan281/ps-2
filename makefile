CC = g++
CFLAGS = -std=c++11 -Wall
TARGET = interactive-shell

all: $(TARGET)

$(TARGET): interactive-shell.cpp
	$(CC) $(CFLAGS) interactive-shell.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)

