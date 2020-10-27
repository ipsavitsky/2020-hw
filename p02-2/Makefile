

graph: graph.o main.o
	clang main.o graph.o -o graph

graph.o: graph.c graph.h
	clang -c -g graph.c

main.o: graph.h main.c
	clang -c -g main.c