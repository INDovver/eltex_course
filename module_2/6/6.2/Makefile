contacts_stat.out: main.c libcontacts.so
	gcc main.c -o contacts_stat.out -L. -lcontacts

libcontacts.so: contacts.o
	gcc -shared contacts.o -o libcontacts.so 

contacts.o: contacts.c 
	gcc -c -fPIC contacts.c -o contacts.o

clean:
	rm -f contacts_stat.out *.o *.so
	