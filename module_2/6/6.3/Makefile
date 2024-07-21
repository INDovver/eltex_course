calc.out: main.c libcalco.a libcalcadd.so libcalcsub.so libcalcdiv.so libcalcmult.so
	gcc -rdynamic main.c -o calc.out -L. -lcalco -ldl

libcalco.a: calco.o
	ar rc libcalco.a calco.o

libcalcadd.so libcalcsub.so libcalcdiv.so libcalcmult.so: calcadd.o calcsub.o calcdiv.o calcmult.o
	gcc -shared calcadd.o -o libcalcadd.so
	gcc -shared calcsub.o -o libcalcsub.so
	gcc -shared calcdiv.o -o libcalcdiv.so
	gcc -shared calcmult.o -o libcalcmult.so

calcadd.o calcsub.o calcdiv.o calcmult.o calco.o: calcadd.c calcsub.c calcdiv.c calcmult.c calco.c
	gcc -c -fPIC calcadd.c -o calcadd.o
	gcc -c -fPIC calcsub.c -o calcsub.o
	gcc -c -fPIC calcdiv.c -o calcdiv.o
	gcc -c -fPIC calcmult.c -o calcmult.o
	gcc -c calco.c -o calco.o

clean:
	rm -f calc.out *.o *.so *.a
	