## constants

ZMQ_CONSTANT_NAME in the C API turns into zmq.CONSTANT_NAME in Lua.

## version

Reports 0MQ library version.
See [zmq_version(3)](http://api.zeromq.org/zmq_version.html).

zmq.version()

## init

Initialises ØMQ context.
See [zmq_init(3)](http://api.zeromq.org/zmq_init.html).

zmq.init(app_threads, io_threads)  
zmq.init(app_threads, io_threads, flags)

## term

Terminates ØMQ context.
See [zmq_term(3)](http://api.zeromq.org/zmq_term.html).

ctx:term()

## socket

Creates ØMQ socket.
See [zmq_socket(3)](http://api.zeromq.org/zmq_socket.html).

ctx:socket(type)

## close

Destroys ØMQ socket.
See [zmq_close(3)](http://api.zeromq.org/zmq_close.html).

s:close()

## setsockopt

Sets a specified option on a ØMQ socket.
See [zmq_setsockopt(3)](http://api.zeromq.org/zmq_setsockopt.html).

s:setsockopt(option, optval)

## bind

Binds the socket to the specified address.
See [zmq_bind(3)](http://api.zeromq.org/zmq_bind.html).

s:bind(addr)

## connect

Connect the socket to the specified address.
See [zmq_connect(3)](http://api.zeromq.org/zmq_connect.html).

s:connect(addr)

## send

Sends a message.
See [zmq_send(3)](http://api.zeromq.org/zmq_send.html).

s:send(msg)  
s:send(msg, flags)

## recv

Retrieves a message from the socket.
See [zmq_recv(3)](http://api.zeromq.org/zmq_recv.html).

s:recv()  
s:recv(flags)
