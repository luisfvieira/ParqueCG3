all: *.cpp
	g++ -std=c++0x *.cpp -o exec -fopenmp -lGLU -lglut -lGL -lGLEW -lm

run:
	./exec

