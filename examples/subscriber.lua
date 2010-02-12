require("zmq")

local ctx = zmq.init(1, 1, 0)
local s = zmq.socket(ctx, zmq.SUB)
zmq.setsockopt(s, zmq.SUBSCRIBE, "")
zmq.connect(s, "tcp://localhost:5555")
while true do
    local msg = zmq.recv(s)
    local msg_id = tonumber(msg)
    if math.mod(msg_id, 10000) == 0 then print(msg_id) end
end
