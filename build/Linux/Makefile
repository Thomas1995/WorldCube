CXX=g++
CFLAGS=-std=c++11 -Wall -Werror -I ../../include/ -I ../../_extern/

SRCPATH=../../src/
CPP_UTILS=$(SRCPATH)utils/utils.cpp $(SRCPATH)utils/xmlparser.cpp $(SRCPATH)utils/random.cpp
CPPS=$(SRCPATH)main.cpp $(CPP_UTILS) $(SRCPATH)worldtime.cpp $(SRCPATH)person.cpp $(SRCPATH)world.cpp

EXE=../../bin/linux/WorldCube.exe
MKDIR=mkdir -p ../../bin & mkdir -p ../../bin/linux
BUILD=$(MKDIR) & $(CXX) $(CFLAGS) $(CPPS) -o $(EXE) -lpthread

build:
	$(BUILD)

debug:
	$(BUILD) -g

run:
	./$(EXE)
