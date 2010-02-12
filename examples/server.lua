require("zmq")

local ctx = zmq.init(1, 1)
local s = zmq.socket(ctx, zmq.REP)
zmq.bind(s, "tcp://lo:5555")
while true do
    print(string.format("Received query: '%s'", zmq.recv(s)))
    zmq.send(s, "OK")
end
