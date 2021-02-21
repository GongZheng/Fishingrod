test_standard:
	@ gcc -O2 -o a.out ./test/test_standard.c
	@ ./a.out
test_optimized:
	@ gcc -O2 -o a.out ./test/test_optimized.c
	@ ./a.out
test_fishingrodLED:
	@ gcc -O2 -o a.out ./test/test_fishingrodLED.c
	@ ./a.out
test_fishingrodLED_step:
	@ gcc -O2 -o a.out ./test/test_fishingrodLED-1step.c
	@ ./a.out
clean:
	@ rm -r *.out