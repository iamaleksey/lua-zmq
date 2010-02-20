require("zmq")

if not arg[3] then
    puts "usage: lua remote_thr.lua <connect-to> <message-size> <message-count>"
    os.exit()
end

local connect_to = arg[1]
local message_size = tonumber(arg[2])
local message_count = tonumber(arg[3])

local ctx = zmq.init(1, 1)
local s = zmq.socket(ctx, zmq.PUB)
zmq.connect(s, connect_to)

local msg = ""
for i = 1, message_size do msg = msg .. "0" end

for i = 1, message_count do
  zmq.send(s, msg)
end

os.execute("sleep " .. 10)

zmq.close(s)
zmq.term(ctx)
