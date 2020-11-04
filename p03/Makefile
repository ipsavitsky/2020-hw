COMPILER = gcc
COMPILER_FLAGS = -g -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11
LINK_COMPILER_FLAGS = -o

# стандартная компиляция(без кавереджа, санитайзера и прочего)
build: RPN_test.o RPN.o	stack.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) RPN
RPN_test.o: RPN_test.c RPN.h
	$(COMPILER) $(COMPILER_FLAGS) -c RPN_test.c
RPN.o: RPN.c RPN.h stack.h
	$(COMPILER) $(COMPILER_FLAGS) -c RPN.c
stack.o: stack.c
	$(COMPILER) $(COMPILER_FLAGS) -c stack.c


# генерация отчета по покрытию
code_coverage: graph_cov.gcda main_cov.gcda
	lcov -t "graph" --output-file code_coverage.info --capture --directory .
	genhtml --branch-coverage --highlight --legend -o code_coverage code_coverage.info


clear:
	rm *.o
	rm graph


# цель очистки промежуточных файлов покрытия не входит общий clean
clear_code_coverage:
	rm *.gcda
	rm *.gcno
	rm *.info