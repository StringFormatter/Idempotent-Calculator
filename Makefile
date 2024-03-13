# Makefile modified from https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean#3220288

sources = main.cpp idempotent.cpp squared.cpp graph.cpp
objects = main.o idempotent.o squared.o graph.o

.PHONY: all clean

all: $(sources) final

final : $(objects)
	g++ $(objects) -o $@

.cpp.o:
	g++ -g -c -Wall -O2 $< -o $@

clean :
	rm final $(objects)
