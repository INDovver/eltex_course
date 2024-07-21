contacts_stat.out: main.c libcontacts.a
	gcc main.c -o contacts_stat.out -L. -lcontacts

libcontacts.a: contacts.o
	ar rc libcontacts.a contacts.o 

contacts.o: contacts.c 
	gcc -c contacts.c -o contacts.o

clean:
	rm contacts_stat.out *.o *.a