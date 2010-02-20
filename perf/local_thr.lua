require("zmq")

if not arg[3] then
    print("usage: lua local_thr.lua <bind-to> <message-size> <message-count>")
    os.exit()
end

local bind_to = arg[1]
local message_size = tonumber(arg[2])
local message_count = tonumber(arg[3])

local ctx = zmq.init(1, 1)
local s = zmq.socket(ctx, zmq.SUB)
zmq.setsockopt(s, zmq.SUBSCRIBE, "");
zmq.bind(s, bind_to)

local msg = zmq.recv(s)

local start_time = os.time()

for i = 1, message_count - 1 do
    msg = zmq.recv(s)
end

local end_time = os.time()

zmq.close(s)
zmq.term(ctx)

local elapsed = os.difftime(end_time, start_time)
if elapsed == 0 then elapsed = 1 end

local throughput = message_count / elapsed
local megabits = throughput * message_size * 8

print(string.format("message size: %i [B]", message_size))
print(string.format("message count: %i", message_count))
print(string.format("mean throughput: %i [msg/s]", throughput))
print(string.format("mean throughput: %.3f [Mb/s]", megabits))
