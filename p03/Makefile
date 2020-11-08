COMPILER = gcc
COMPILER_FLAGS = -g -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11
LINK_COMPILER_FLAGS = -o

# стандартная компиляция(без кавереджа, санитайзера и прочего)
RPN: test_rec_desc.o RPN.o stack.o rec_desc.o errors.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) RPN
test_rec_desc.o: test_rec_desc.c rec_desc.h
	$(COMPILER) $(COMPILER_FLAGS) -c test_rec_desc.c
RPN.o: RPN.c RPN.h stack.h
	$(COMPILER) $(COMPILER_FLAGS) -c RPN.c
stack.o: stack.c stack.h
	$(COMPILER) $(COMPILER_FLAGS) -c stack.c
rec_desc.o: rec_desc.c rec_desc.h stack.h RPN.h
	$(COMPILER) $(COMPILER_FLAGS) -c rec_desc.c
errors.o: errors.c errors.h
	$(COMPILER) $(COMPILER_FLAGS) -c errors.c

# генерация отчета по покрытию
code_coverage: graph_cov.gcda main_cov.gcda
	lcov -t "graph" --output-file code_coverage.info --capture --directory .
	genhtml --branch-coverage --highlight --legend -o code_coverage code_coverage.info

clear:
	rm *.o
	rm RPN

# цель очистки промежуточных файлов покрытия не входит общий clean
clear_code_coverage:
	rm *.gcda
	rm *.gcno
	rm *.info