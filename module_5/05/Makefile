obj-m += kernel_space.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc user_space.c -o user_space.out

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm *.out