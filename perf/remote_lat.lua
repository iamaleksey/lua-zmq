require("zmq")

if not arg[3] then
    print("usage: lua remote_lat.lua <connect-to> <message-size> <roundtrip-count>")
    os.exit()
end

local connect_to = arg[1]
local message_size = tonumber(arg[2])
local roundtrip_count = tonumber(arg[3])

local ctx = zmq.init(1, 1)
local s = zmq.socket(ctx, zmq.REQ)
zmq.connect(s, connect_to)

local msg = ""
for i = 1, message_size do msg = msg .. "0" end

local start_time = os.time()

for i = 1, roundtrip_count do
    zmq.send(s, msg)
    msg = zmq.recv(s)
end

local end_time = os.time()

zmq.close(s)
zmq.term(ctx)

local elapsed = os.difftime(end_time, start_time)
local latency = elapsed * 1000000 / roundtrip_count / 2

print(string.format("message size: %i [B]", message_size))
print(string.format("roundtrip count: %i", roundtrip_count))
print(string.format("mean latency: %.3f [us]", latency))
