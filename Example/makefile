#this is just a simple makefile. no real need to add anything here

all: ./
	g++ -std=c++11 -g -pedantic -Wno-deprecated-declarations -Wno-c++11-extensions ./Example.cpp -o Example -I../include ../bin/libWindowAPI_D.a -lGLU -lGL -lX11 -lpthread -lstdc++ 2> errors.txt
