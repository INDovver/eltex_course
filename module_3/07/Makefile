msg_snd.out msg_rcv.out: msg_snd.c msg_rcv.c
	gcc msg_rcv.c -o msg_rcv.out -lrt
	gcc msg_snd.c -o msg_snd.out -lrt

clean:
	rm *.out 