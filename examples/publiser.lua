require("zmq")

local ctx = zmq.init(1, 1, 0)
local s = zmq.socket(ctx, zmq.PUB)
zmq.bind(s, "tcp://lo:5555")
local msg_id = 1
while true do
    zmq.send(s, tostring(msg_id))
    msg_id = msg_id + 1
end
