test_standard:
	@ gcc -O2 -o a.out ./test/test_standard.c
	@ ./a.out
test_optimized:
	@ gcc -O2 -o a.out ./test/test_optimized.c
	@ ./a.out
clean:
	@ rm -r *.out