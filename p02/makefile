COMPILER = clang
COMPILER_FLAGS = -g -c -Wall
FIN_COMPILER_FLAGS = -o

main:	main.o	graphs.o
	$(COMPILER) main.o graphs.o $(FIN_COMPILER_FLAGS) $@
main.o:	main.c	graphs.h
	$(COMPILER) main.c $(COMPILER_FLAGS)
graphs.o: graphs.c	graphs.h
	$(COMPILER) graphs.c $(COMPILER_FLAGS)
