all:
	gcc -o sem sem.c

run: all
	./sem

cvcr: all
	./sem -c 4
	./sem -v
	./sem -c 3
	./sem -r
