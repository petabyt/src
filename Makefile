all:
	@cc *.c
	@./a.out > a.c
	@cc -w a.c
	@./a.out

	@rm -rf *.out a.c
