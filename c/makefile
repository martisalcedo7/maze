all: solver

# main: main.c
# 	gcc -O3 main.c -o main 

solver: solver.cpp
	g++ -O3 solver.cpp utils.cpp -o solver

clean:
	rm -f main solver result.csv
