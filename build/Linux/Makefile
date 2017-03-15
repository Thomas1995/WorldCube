CXX=g++
CFLAGS=-std=c++11 -Wall -I ../../include/ -I ../../_extern/
CPPS=../../src/main.cpp
EXE=../../bin/linux/WorldCube.exe
MKDIR=mkdir -p ../../bin & mkdir -p ../../bin/linux
BUILD=$(MKDIR) & $(CXX) $(CFLAGS) $(CPPS) -o $(EXE)

build:
	$(BUILD)

debug:
	$(BUILD) -g

run:
	./$(EXE)
