all : cpu

cpu: cpu.o process.o cache.o
	gcc -g -no-pie -o cpu cpu.o process.o cache.o

cpu.o: cpu.c process.h cache.h
	gcc -g -c -o cpu.o cpu.c

process.o: process.c process.h cache.h
	gcc -g -c -o process.o process.c

cache.o: cache.c cache.h
	gcc -g -c -o cache.o cache.c

clean:
	rm -f *.o
	
	