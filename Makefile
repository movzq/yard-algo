compiler = gcc
main: yard-algo.o
	$(compiler) -o yardalgo yard-algo.o
	./yardalgo

rm:
	rm yardalgo *.o
