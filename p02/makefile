COMPILER = clang
COMPILER_FLAGS = -g -c -Wall
LINK_COMPILER_FLAGS = -o

main:	main.o	graphs.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) $@
main.o:	main.c	graphs.h
	$(COMPILER) main.c $(COMPILER_FLAGS)
graphs.o: graphs.c	graphs.h
	$(COMPILER) graphs.c $(COMPILER_FLAGS)
out.pdf: out.dot
	dot -Tpdf -o $@ $^
clear:
	rm *.o
	rm main
	rm out.dot
