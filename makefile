all:
	gcc -o control control.c

c: all
	./control -c

v: all
	./control -v

r: all
	./control -r
