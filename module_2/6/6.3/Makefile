calc.out: main.c libcalcadd.so libcalcsub.so libcalcdiv.so libcalcmult.so libcalco.so
	gcc main.c -o calc.out -L. -lcalco -ldl

libcalcadd.so libcalcsub.so libcalcdiv.so libcalcmult.so libcalco.so: calcadd.o calcsub.o calcdiv.o calcmult.o calco.o
	gcc -shared calco.o calcadd.o -o libcalcadd.so
	gcc -shared calco.o calcsub.o -o libcalcsub.so
	gcc -shared calco.o calcdiv.o -o libcalcdiv.so
	gcc -shared calco.o calcmult.o -o libcalcmult.so
	gcc -shared calco.o -o libcalco.so

calcadd.o calcsub.o calcdiv.o calcmult.o calco.o: calcadd.c calcsub.c calcdiv.c calcmult.c calco.c
	gcc -c -fPIC calcadd.c -o calcadd.o
	gcc -c -fPIC calcsub.c -o calcsub.o
	gcc -c -fPIC calcdiv.c -o calcdiv.o
	gcc -c -fPIC calcmult.c -o calcmult.o
	gcc -c -fPIC calco.c -o calco.o

clean:
	rm -f calc.out *.o *.so *.a
	