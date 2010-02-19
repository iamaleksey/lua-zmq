CFLAGS = `pkg-config lua5.1 --cflags` -fPIC -O3 -Wall
LFLAGS = -shared `pkg-config --libs --cflags libzmq`
INSTALL_PATH = `pkg-config lua5.1 --variable=INSTALL_CMOD`


all: zmq.so


zmq.lo: zmq.c
	gcc -o zmq.lo -c $(CFLAGS) zmq.c


zmq.so: zmq.lo
	gcc -o zmq.so $(LFLAGS) zmq.lo


install: zmq.so
	install -D -s zmq.so $(INSTALL_PATH)/zmq.so


clean:
	rm -f zmq.so zmq.lo
