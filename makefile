CC	:=	g++
CFLAGS	:=	-Wall -Isrc/network -Isrc/game
TARGET	:=	main

SRCS	:=	$(wildcard src/*.cpp) $(wildcard src/*/*.cpp)

.PHONY: all run server

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET).out

run: all
	./$(TARGET).out

server:
	java -Xmx1G -Xms1G -jar ~/minecraft/server.jar nogui