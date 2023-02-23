all: solver

FLAGS = $(shell pkg-config --cflags opencv4)
LIBS = $(shell pkg-config --libs opencv4)

solver: solver.cpp
	g++ -O3 solver.cpp utils.cpp \
	$(FLAGS) \
	$(LIBS) \
	-o solver

clean:
	rm -f main solver
