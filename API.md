# constants

ZMQ_CONSTANT_NAME in the C API corresponds to zmq.CONSTANT_NAME in Lua.


# zmq.init

Initialises ØMQ context.
See [zmq_init(3)](http://api.zeromq.org/zmq_init.3.html).

zmq.init(app_threads, io_threads)
zmq.init(app_threads, io_threads, flags)


# zmq.term

Terminates ØMQ context.
See [zmq_term(3)](http://api.zeromq.org/zmq_term.3.html).

zmq.term(context)


# zmq.socket

Creates ØMQ socket.
See [zmq_socket(3)](http://api.zeromq.org/zmq_socket.3.html).

zmq.socket(context, type)


# zmq.close

Destroys ØMQ socket.
See [zmq_close(3)](http://api.zeromq.org/zmq_close.3.html).

zmq.close(socket)


# zmq.setsockopt

Sets a specified option on a ØMQ socket.
See [zmq_setsockopt(3)](http://api.zeromq.org/zmq_setsockopt.3.html).

zmq.setsockopt(socket, option, optval)


# zmq.bind

Binds the socket to the specified address.
See [zmq_bind(3)](http://api.zeromq.org/zmq_bind.3.html).

zmq.bind(socket, addr)


# zmq.connect

Connect the socket to the specified address.
See [zmq_connect(3)](http://api.zeromq.org/zmq_connect.3.html).

zmq.connect(socket, addr)


# zmq.send

Sends a message.
See [zmq_send(3)](http://api.zeromq.org/zmq_send.3.html).

zmq.send(socket, msg)
zmq.send(socket, msg, flags)


# zmq.flush

Flushes unflushed messages to the socket.
See [zmq_flush(3)](http://api.zeromq.org/zmq_flush.3.html).

zmq.flush(socket)


# zmq.recv

Retrieves a message from the socket.
See [zmq_recv(3)](http://api.zeromq.org/zmq_recv.3.html).

zmq.recv(socket)
zmq.recv(socket, flags)
