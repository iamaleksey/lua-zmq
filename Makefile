zmq.so: zmq.c
	gcc -O2 -fpic -c -I /usr/include/lua5.1/ -o zmq.o zmq.c
	gcc -O `pkg-config --libs --cflags libzmq` -shared -fpic -o zmq.so zmq.o

clean:
	rm zmq.so zmq.o
