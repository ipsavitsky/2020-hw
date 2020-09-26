COMPILER = gcc
COMPILER_FLAGS = -g -Wall -Werror -c
LINK_COMPILER_FLAGS = -o 


main:	main.o	graphs.o
	$(COMPILER) -fsanitize=address $^ $(LINK_COMPILER_FLAGS) $@
main.o:	main.c	graphs.h
	$(COMPILER) -fsanitize=address $(COMPILER_FLAGS) main.c
graphs.o: graphs.c	graphs.h
	$(COMPILER) -fsanitize=address $(COMPILER_FLAGS) graphs.c

out.pdf: out.dot
	dot -Tpdf -o $@ $^



code_coverage: main graphs.gcda main.gcda
	lcov -t "main" -o code_coverage.info -c -d .
	genhtml -o code_coverage code_coverage.info

clear: clear_regular clear_dot

clear_regular:
	rm *.o
	rm main

clear_dot:
	rm out.dot
	rm out.pdf

clear_code_coverage:
	rm *.info
	rm *.gcda
	rm *.gcno