COMPILER = gcc
COMPILER_FLAGS = -O2 -ftrapv  -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11
LINK_COMPILER_FLAGS = -o


build:	main.o	graph.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) graph
main.o:	main.c	graph.h
	$(COMPILER) $(COMPILER_FLAGS) -c main.c
graph.o: graph.c	graph.h
	$(COMPILER) $(COMPILER_FLAGS) -c graph.c


build_coverage:	main_cov.o	graph_cov.o
	$(COMPILER) --coverage $^ $(LINK_COMPILER_FLAGS) graph
main_cov.o:	main.c	graph.h
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c main.c -o $@
graph_cov.o: graph.c	graph.h
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c graph.c -o $@


build_san:	main_san.o	graph_san.o
	$(COMPILER) -fsanitize=address,undefined -g $^ $(LINK_COMPILER_FLAGS) graph
main_san.o:	main.c	graph.h
	$(COMPILER) -fsanitize=address,undefined -g $(COMPILER_FLAGS) -c main.c -o $@
graph_san.o: graph.c	graph.h
	$(COMPILER) -fsanitize=address,undefined -g $(COMPILER_FLAGS) -c graph.c -o $@


graph.png: graph.dot
	dot -Tpng -o $@ $^

code_coverage: graph_cov.gcda main_cov.gcda
	lcov -t "graph" --output-file code_coverage.info --capture --directory .
	genhtml --branch-coverage --highlight --legend -o code_coverage code_coverage.info

clear: clear_regular clear_dot

clear_regular:
	rm *.o
	rm graph

clear_dot:
	rm graph.dot
	rm graph.png

clear_code_coverage:
	rm *.gcda
	rm *.gcno
	rm *.info