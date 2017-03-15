CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: prog1

prog1: prog1.cpp
	g++ -o myProg1 prog1.cpp `pkg-config opencv --cflags --libs`


clean:
	\rm myProg1
