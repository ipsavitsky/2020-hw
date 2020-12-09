COMPILER = gcc
COMPILER_FLAGS =  -g -O2 -ftrapv -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11

all: copy gr dump

copy: ./cp/cp.c
	cd cp; $(COMPILER) $(COMPILER_FLAGS) cp.c

gr: ./grep/grep.c
	cd grep; $(COMPILER) $(COMPILER_FLAGS) grep.c

dump: ./od/od.c
	cd od; $(COMPILER) $(COMPILER_FLAGS) od.c