graph: graph.o main.o
	clang main.o graph.o -fsanitize=address -o graph

graph.o: graph.c graph.h
	clang -c -g -fsanitize=address graph.c

main.o: graph.h main.c
	clang -c -g -fsanitize=address main.c