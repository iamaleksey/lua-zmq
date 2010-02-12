require("zmq")

local ctx = zmq.init(1, 1)
local s = zmq.socket(ctx, zmq.REQ)
zmq.connect(s, "tcp://localhost:5555")
zmq.send(s, "SELECT * FROM mytable")
print(zmq.recv(s))
zmq.close(s)
zmq.term(ctx)
