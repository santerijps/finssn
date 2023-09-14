dev:
	tcc -run finssngen.c -h --from 2023

c_gcc: finssngen.c bin_dir
	gcc -O3 -s -fno-ident -fno-asynchronous-unwind-tables -o bin/main.gcc.exe finssngen.c
	upx --best -q bin/main.gcc.exe -q

c_clang: finssngen.c bin_dir
	clang -O3 -s -fno-ident -fno-asynchronous-unwind-tables -o bin/main.clang.exe finssngen.c
	upx -q --best bin/main.clang.exe -q

c_zig: finssngen.c bin_dir
	zig cc -O3 -s -fno-ident -fno-asynchronous-unwind-tables -o bin/main.zig.exe finssngen.c
	upx -q --best bin/main.zig.exe -q

c_tcc: finssngen.c bin_dir
	tcc -o bin/main.tcc.exe finssngen.c
	upx -q --best bin/main.tcc.exe -q

bin_dir: finssngen.c
	if not exist bin mkdir bin

all: c_gcc c_clang c_tcc c_zig
