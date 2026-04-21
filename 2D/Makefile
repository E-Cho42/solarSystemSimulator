CXX = g++
CXXFLAGS = -std=c++20 -Wall -g -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = PhysEngine

# Added RigidBody.cpp and removed the comma
SRCS = main.cpp RidgidBody.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)