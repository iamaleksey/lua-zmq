## constants

ZMQ_CONSTANT_NAME in the C API turns into zmq.CONSTANT_NAME in Lua.

## init

Initialises ØMQ context.
See [zmq_init(3)](http://api.zeromq.org/zmq_init.3.html).

zmq.init(app_threads, io_threads)  
zmq.init(app_threads, io_threads, flags)

## term

Terminates ØMQ context.
See [zmq_term(3)](http://api.zeromq.org/zmq_term.3.html).

ctx:term()

## socket

Creates ØMQ socket.
See [zmq_socket(3)](http://api.zeromq.org/zmq_socket.3.html).

ctx:socket(type)

## close

Destroys ØMQ socket.
See [zmq_close(3)](http://api.zeromq.org/zmq_close.3.html).

s:close()

## setsockopt

Sets a specified option on a ØMQ socket.
See [zmq_setsockopt(3)](http://api.zeromq.org/zmq_setsockopt.3.html).

s:setsockopt(option, optval)

## bind

Binds the socket to the specified address.
See [zmq_bind(3)](http://api.zeromq.org/zmq_bind.3.html).

s:bind(addr)

## connect

Connect the socket to the specified address.
See [zmq_connect(3)](http://api.zeromq.org/zmq_connect.3.html).

s:connect(addr)

## send

Sends a message.
See [zmq_send(3)](http://api.zeromq.org/zmq_send.3.html).

s:send(msg)  
s:send(msg, flags)

## flush

Flushes unflushed messages to the socket.
See [zmq_flush(3)](http://api.zeromq.org/zmq_flush.3.html).

s:flush()

## recv

Retrieves a message from the socket.
See [zmq_recv(3)](http://api.zeromq.org/zmq_recv.3.html).

s:recv()  
s:recv(flags)
