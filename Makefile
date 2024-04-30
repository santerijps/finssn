cc := gcc

all: gen rand

gen: bin_dir src/finssn-gen.c
	$(cc) src/finssn-gen.c -Wall -Wextra -O3 -o bin/finssn-gen.exe

rand: bin_dir src/finssn-rand.c
	$(cc) src/finssn-rand.c -Wall -Wextra -O3 -o bin/finssn-rand.exe

valid: bin_dir src/finssn-valid.c
	$(cc) src/finssn-valid.c -Wall -Wextra -O3 -o bin/finssn-valid.exe

bin_dir:
	if not exist bin mkdir bin
