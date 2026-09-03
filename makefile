CXX	:=	g++
CXXFLAGS	:=	-Wall -Isrc/network -Isrc/game -Isrc/world -Isrc/include -Isrc/player
LDFLAGS	:=	-Lsrc/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET	:=	main.out

SRCS	:=	$(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJS	:=	$(SRCS:.cpp=.o)

.PHONY: all run server

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

all: $(TARGET)

run:
	./$(TARGET)