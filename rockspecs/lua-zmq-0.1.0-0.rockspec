package = "lua-zmq"
version = "0.1.0-0"
source = {
   url = "http://github.com/downloads/norman/lua-zmq/lua-zmq-0.1.0.tar.gz",
   md5 = "64a6fdaef9992bee145d18c458da2afb"
}
description = {
   summary = "Lua bindings to zeromq2",
   homepage = "http://github.com/iamaleksey/lua-zmq",
   license = "MIT/X11"
}
dependencies = {
   "lua >= 5.1"
}
build = {
   type = "builtin",
   modules = {
     zmq = {
       sources = {"zmq.c"},
       libraries = {"zmq"}
     }
   }
}
