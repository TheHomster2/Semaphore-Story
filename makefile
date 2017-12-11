all:
	gcc -o control control.c
	gcc -o write write.c

c: all
	./control -c

v: all
	./control -v

r: all
	./control -r
