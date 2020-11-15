COMPILER = gcc
COMPILER_FLAGS =  -g -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11
LINK_COMPILER_FLAGS = -fsanitize=address,undefined -o

# COMPILER = clang
# COMPILER_FLAGS = -Weverything
# LINK_COMPILER_FLAGS = -o


# стандартная компиляция(без кавереджа, санитайзера и прочего)
calc: calculator.o RPN.o stack.o rec_desc.o errors.o arithm_func.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) $@
calculator.o: calculator.c
	$(COMPILER) $(COMPILER_FLAGS) -c calculator.c
RPN.o: RPN.c RPN.h stack.h
	$(COMPILER) $(COMPILER_FLAGS) -c RPN.c
stack.o: stack.c stack.h
	$(COMPILER) $(COMPILER_FLAGS) -c stack.c
rec_desc.o: rec_desc.c rec_desc.h
	$(COMPILER) $(COMPILER_FLAGS) -c rec_desc.c
errors.o: errors.c errors.h
	$(COMPILER) $(COMPILER_FLAGS) -c errors.c
arithm_func.o: arithm_func.c arithm_func.h
	$(COMPILER) $(COMPILER_FLAGS) -c arithm_func.c
test_rec_desc.o: test_rec_desc.c rec_desc.h
	$(COMPILER) $(COMPILER_FLAGS) -c test_rec_desc.c

# компиляция с санитайзером в исполняемый файл calc_asan
calc_asan: calculator_asan.o RPN_asan.o stack_asan.o rec_desc_asan.o errors_asan.o arithm_func_asan.o
	$(COMPILER) $^ -fsanitize=address,undefined $(LINK_COMPILER_FLAGS) $@
calculator_asan.o: calculator.c
	$(COMPILER) -fsanitize=address,undefined $(COMPILER_FLAGS) -c calculator.c -o $@
RPN_asan.o: RPN.c RPN.h stack.h
	$(COMPILER) -fsanitize=address,undefined $(COMPILER_FLAGS) -c RPN.c -o $@
stack_asan.o: stack.c stack.h
	$(COMPILER) -fsanitize=address,undefined $(COMPILER_FLAGS) -c stack.c -o $@
rec_desc_asan.o: rec_desc.c rec_desc.h
	$(COMPILER) -fsanitize=address,undefined $(COMPILER_FLAGS) -c rec_desc.c -o $@
errors_asan.o: errors.c errors.h
	$(COMPILER) -fsanitize=address,undefined $(COMPILER_FLAGS) -c errors.c -o $@
arithm_func_asan.o: arithm_func.c arithm_func.h
	$(COMPILER) -fsanitize=address,undefined $(COMPILER_FLAGS) -c arithm_func.c -o $@
test_rec_desc_asan.o: test_rec_desc.c rec_desc.h
	$(COMPILER) -fsanitize=address,undefined $(COMPILER_FLAGS) -c test_rec_desc.c -o $@

# компиляция с отчетом о покрытии кода
calc_cov: calculator_cov.o RPN_cov.o stack_cov.o rec_desc_cov.o errors_cov.o arithm_func_cov.o
	$(COMPILER) $^ --coverage $(LINK_COMPILER_FLAGS) $@
calculator_cov.o: calculator.c
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c calculator.c -o $@
RPN_cov.o: RPN.c RPN.h stack.h
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c RPN.c -o $@
stack_cov.o: stack.c stack.h
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c stack.c -o $@
rec_desc_cov.o: rec_desc.c rec_desc.h
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c rec_desc.c -o $@
errors_cov.o: errors.c errors.h
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c errors.c -o $@
arithm_func_cov.o: arithm_func.c arithm_func.h
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c arithm_func.c -o $@
test_rec_desc_cov.o: test_rec_desc.c rec_desc.h
	$(COMPILER) --coverage $(COMPILER_FLAGS) -c test_rec_desc.c -o $@

# компиляция test_stack(не очень полезно, ну ладно)
test_stack: stack.o test_stack.o
	$(COMPILER) $^ $(LINK_COMPILER_FLAGS) test_stack
test_stack.o: test_stack.c
	$(COMPILER) $(COMPILER_FLAGS) -c test_stack.c


fuzz: fuzz.o RPN_f.o stack_f.o rec_desc_f.o errors_f.o arithm_func_f.o
	clang $^ -fsanitize=address,undefined,fuzzer -g -o $@
fuzz.o: fuzz.c
	clang -fsanitize=address,undefined,fuzzer -g -c fuzz.c -o $@
RPN_f.o: RPN.c RPN.h stack.h
	clang -fsanitize=address,undefined,fuzzer -g -c RPN.c -o $@
stack_f.o: stack.c stack.h
	clang -fsanitize=address,undefined,fuzzer -g -c stack.c -o $@
rec_desc_f.o: rec_desc.c rec_desc.h
	clang -fsanitize=address,undefined,fuzzer -g -c rec_desc.c -o $@
errors_f.o: errors.c errors.h
	clang -fsanitize=address,undefined,fuzzer -g -c errors.c -o $@
arithm_func_f.o: arithm_func.c arithm_func.h
	clang -fsanitize=address,undefined,fuzzer -g -c arithm_func.c -o $@
test_rec_desc_f.o: test_rec_desc.c rec_desc.h
	clang -fsanitize=address,undefined,fuzzer -g -c test_rec_desc.c -o $@

# генерация отчета по покрытию
code_coverage: test_rec_desc.gcda RPN.gcda stack.gcda rec_desc.gcda
	lcov -t "RPN" --output-file code_coverage.info --capture --directory .
	genhtml --branch-coverage --highlight --legend -o code_coverage code_coverage.info

clear:
	rm *.o сalc*

# цель очистки промежуточных файлов покрытия не входит общий clean
clear_code_coverage:
	rm *.gcda
	rm *.gcno
	rm *.info