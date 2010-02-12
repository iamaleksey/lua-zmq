#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <zmq.h>

static int Lzmq_init(lua_State *L) {
    int app_threads = luaL_checkint(L, 1);
    int io_threads = luaL_checkint(L, 2);
    int flags = luaL_optint(L, 3, 0);

    void *ctx = zmq_init(app_threads, io_threads, flags);
    if(!ctx) {
        return luaL_error(L, zmq_strerror(errno));
    }
    lua_pushlightuserdata(L, ctx);
    return 1;
}

static int Lzmq_term(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *ctx = lua_touserdata(L, 1);

    assert(zmq_term(ctx) == 0);
    return 0;
}

static int Lzmq_socket(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *ctx = lua_touserdata(L, 1);
    int type = luaL_checkint(L, 2);

    void *s = zmq_socket(ctx, type);
    if(!s) {
        return luaL_error(L, zmq_strerror(errno));
    }
    lua_pushlightuserdata(L, s);
    return 1;
}

static int Lzmq_close(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *s = lua_touserdata(L, 1);
    assert(zmq_close(s) == 0);
    return 0;
}

static int Lzmq_setsockopt(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *s = lua_touserdata(L, 1);
    int option = luaL_checkint(L, 2);

    int rc = 0;

    switch(option) {
    case ZMQ_HWM:
    case ZMQ_LWM:
    case ZMQ_SWAP:
    case ZMQ_AFFINITY:
        {
            int64_t optval = (int64_t) luaL_checklong(L, 3);
            rc = zmq_setsockopt(s, option, (void *) &optval, sizeof(int64_t));
        }
        break;
    case ZMQ_IDENTITY:
    case ZMQ_SUBSCRIBE:
    case ZMQ_UNSUBSCRIBE:
        {
            size_t optvallen;
            const char *optval = luaL_checklstring(L, 3, &optvallen);
            rc = zmq_setsockopt(s, option, (void *) optval, optvallen);
        }
        break;
    case ZMQ_RATE:
    case ZMQ_RECOVERY_IVL:
    case ZMQ_MCAST_LOOP:
    case ZMQ_SNDBUF:
    case ZMQ_RCVBUF:
        {
            uint64_t optval = (uint64_t) luaL_checklong(L, 3);
            rc = zmq_setsockopt(s, option, (void *) &optval, sizeof(uint64_t));
        }
        break;
    default:
        rc = -1;
        errno = EINVAL;
    }

    if(rc != 0) {
        return luaL_error(L, zmq_strerror(errno));
    }
    return 0;
}

static int Lzmq_bind(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *s = lua_touserdata(L, 1);
    const char *addr = luaL_checkstring(L, 2);

    if(zmq_bind(s, addr) != 0) {
        return luaL_error(L, zmq_strerror(errno));
    }
    return 0;
}

static int Lzmq_connect(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *s = lua_touserdata(L, 1);
    const char *addr = luaL_checkstring(L, 2);

    if(zmq_connect(s, addr) != 0) {
        return luaL_error(L, zmq_strerror(errno));
    }
    return 0;
}

static int Lzmq_send(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *s = lua_touserdata(L, 1);
    size_t msg_size;
    const char *data = luaL_checklstring(L, 2, &msg_size);
    int flags = luaL_optint(L, 3, 0);

    zmq_msg_t msg;
    assert(zmq_msg_init_size(&msg, msg_size) == 0);
    memcpy(zmq_msg_data(&msg), data, msg_size);

    int rc = zmq_send(s, &msg, flags);

    assert(zmq_msg_close(&msg) == 0);

    if (rc != 0 && errno == EAGAIN) {
        lua_pushboolean(L, 0);
        return 1;
    }

    if(rc != 0) {
        return luaL_error(L, zmq_strerror(errno));
    }

    lua_pushboolean(L, 1);
    return 1;
}

static int Lzmq_flush(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *s = lua_touserdata(L, 1);

    if(zmq_flush(s) != 0) {
        return luaL_error(L, zmq_strerror(errno));
    }
    return 0;
}

static int Lzmq_recv(lua_State *L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    void *s = lua_touserdata(L, 1);
    int flags = luaL_optint(L, 2, 0);

    zmq_msg_t msg;
    assert(zmq_msg_init(&msg) == 0);

    int rc = zmq_recv(s, &msg, flags);

    if(rc != 0 && errno == EAGAIN) {
        assert(zmq_msg_close(&msg) == 0);
        lua_pushnil(L);
        return 1;
    }

    if(rc != 0) {
        assert(zmq_msg_close(&msg) == 0);
        return luaL_error(L, zmq_strerror(errno));
    }

    lua_pushlstring(L, zmq_msg_data(&msg), zmq_msg_size(&msg));
    assert(zmq_msg_close(&msg) == 0);
    return 1;
}

static const luaL_reg zmqlib[] = {
    {"init",       Lzmq_init},
    {"term",       Lzmq_term}, 
    {"socket",     Lzmq_socket},
    {"close",      Lzmq_close},
    {"setsockopt", Lzmq_setsockopt},
    {"bind",       Lzmq_bind},
    {"connect",    Lzmq_connect},
    {"send",       Lzmq_send},
    {"flush",      Lzmq_flush},
    {"recv",       Lzmq_recv},
    {NULL,         NULL}
};

LUALIB_API int luaopen_zmq(lua_State *L) {
    luaL_register(L, "zmq", zmqlib);
    // flags.
    lua_pushnumber(L, ZMQ_POLL);
    lua_setfield(L, -2, "POLL");
    lua_pushnumber(L, ZMQ_NOBLOCK);
    lua_setfield(L, -2, "NOBLOCK");
    lua_pushnumber(L, ZMQ_NOFLUSH);
    lua_setfield(L, -2, "NOFLUSH");
    // zmq.socket types.
    lua_pushnumber(L, ZMQ_P2P);
    lua_setfield(L, -2, "P2P");
    lua_pushnumber(L, ZMQ_PUB);
    lua_setfield(L, -2, "PUB");
    lua_pushnumber(L, ZMQ_SUB);
    lua_setfield(L, -2, "SUB");
    lua_pushnumber(L, ZMQ_REQ);
    lua_setfield(L, -2, "REQ");
    lua_pushnumber(L, ZMQ_REP);
    lua_setfield(L, -2, "REP");
    lua_pushnumber(L, ZMQ_XREQ);
    lua_setfield(L, -2, "XREQ");
    lua_pushnumber(L, ZMQ_XREP);
    lua_setfield(L, -2, "XREP");
    lua_pushnumber(L, ZMQ_UPSTREAM);
    lua_setfield(L, -2, "UPSTREAM");
    lua_pushnumber(L, ZMQ_DOWNSTREAM);
    lua_setfield(L, -2, "DOWNSTREAM");
    // zmq.setsockopt options.
    lua_pushnumber(L, ZMQ_HWM);
    lua_setfield(L, -2, "HWM");
    lua_pushnumber(L, ZMQ_LWM);
    lua_setfield(L, -2, "LWM");
    lua_pushnumber(L, ZMQ_SWAP);
    lua_setfield(L, -2, "SWAP");
    lua_pushnumber(L, ZMQ_AFFINITY);
    lua_setfield(L, -2, "AFFINITY");
    lua_pushnumber(L, ZMQ_IDENTITY);
    lua_setfield(L, -2, "IDENTITY");
    lua_pushnumber(L, ZMQ_SUBSCRIBE);
    lua_setfield(L, -2, "SUBSCRIBE");
    lua_pushnumber(L, ZMQ_UNSUBSCRIBE);
    lua_setfield(L, -2, "UNSUBSCRIBE");
    lua_pushnumber(L, ZMQ_RATE);
    lua_setfield(L, -2, "RATE");
    lua_pushnumber(L, ZMQ_RECOVERY_IVL);
    lua_setfield(L, -2, "RECOVERY_IVL");
    lua_pushnumber(L, ZMQ_MCAST_LOOP);
    lua_setfield(L, -2, "MCAST_LOOP");
    lua_pushnumber(L, ZMQ_SNDBUF);
    lua_setfield(L, -2, "SNDBUF");
    lua_pushnumber(L, ZMQ_RCVBUF);
    lua_setfield(L, -2, "RCVBUF");
    return 1;
}
