all: solver

FLAGS = $(shell pkg-config --cflags opencv)
LIBS = $(shell pkg-config --libs opencv)

solver: solver.cpp
	g++ -O3 solver.cpp utils.cpp \
	$(FLAGS) \
	$(LIBS) \
	-o solver

clean:
	rm -f main solver
