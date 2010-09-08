package = "lua-zmq"
version = "scm-0"
source = {
   url = "git://github.com/iamaleksey/lua-zmq.git"
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
