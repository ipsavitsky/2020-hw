COMPILER = gcc
DEBUG_COMPILER_FLAGS = -O0 -g -Wall -Werror -c
LINK_COMPILER_FLAGS = -o 


main:	main.o	graphs.o
	$(COMPILER) --coverage -fsanitize=address $^ $(LINK_COMPILER_FLAGS) $@
main.o:	main.c	graphs.h
	$(COMPILER) --coverage -fsanitize=address $(DEBUG_COMPILER_FLAGS) main.c
graphs.o: graphs.c	graphs.h
	$(COMPILER) --coverage -fsanitize=address $(DEBUG_COMPILER_FLAGS) graphs.c

out.pdf: out.dot
	dot -Tpdf -o $@ $^



code_coverage: main graphs.gcda main.gcda
	lcov -t "main" --output-file code_coverage.info --capture --directory .
	genhtml --branch-coverage --highlight --legend -o code_coverage code_coverage.info

clear: clear_regular clear_dot

clear_regular:
	rm *.o
	rm main

clear_dot:
	rm out.dot
	rm out.pdf

clear_code_coverage:
	rm *.gcda
	rm *.gcno
	rm *.info