package = "lua-zmq"
version = "0.1.0-0"
source = {
   url = "http://example.org/package.tar.gz"
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
