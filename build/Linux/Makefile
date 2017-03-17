CXX=g++
CFLAGS=-std=c++11 -Wall -I ../../include/ -I ../../_extern/

SRCPATH=../../src/
CPP_UTILS=$(SRCPATH)utils/utils.cpp $(SRCPATH)utils/xmlparser.cpp $(SRCPATH)utils/parsable_utils.cpp
CPPS=$(SRCPATH)main.cpp $(CPP_UTILS)

EXE=../../bin/linux/WorldCube.exe
MKDIR=mkdir -p ../../bin & mkdir -p ../../bin/linux
BUILD=$(MKDIR) & $(CXX) $(CFLAGS) $(CPPS) -o $(EXE)

build:
	$(BUILD)

debug:
	$(BUILD) -g

run:
	./$(EXE)
