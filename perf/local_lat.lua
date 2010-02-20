require("zmq")

if not arg[2] then
    print("usage: lua local_lat.lua <bind-to> <roundtrip-count>")
    os.exit()
end

local bind_to = arg[1]
local roundtrip_count = tonumber(arg[2])

local ctx = zmq.init(1, 1)
local s = zmq.socket(ctx, zmq.REP)
zmq.bind(s, bind_to)

local msg

for i = 1, roundtrip_count do
    msg = zmq.recv(s)
    zmq.send(s, msg)
end

zmq.close(s)
zmq.term(ctx)
