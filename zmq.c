/*
 * Copyright (c) 2010 Aleksey Yeschenko <aleksey@yeschenko.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"

#include <zmq.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#define MT_ZMQ_CONTEXT "MT_ZMQ_CONTEXT"
#define MT_ZMQ_SOCKET  "MT_ZMQ_SOCKET"

typedef struct { void *ptr; } zmq_ptr;

static int Lzmq_version(lua_State *L)
{
    int major, minor, patch;

    zmq_version(&major, &minor, &patch);

    lua_createtable(L, 3, 0);

    lua_pushinteger(L, 1);
    lua_pushinteger(L, major);
    lua_settable(L, -3);

    lua_pushinteger(L, 2);
    lua_pushinteger(L, minor);
    lua_settable(L, -3);

    lua_pushinteger(L, 3);
    lua_pushinteger(L, patch);
    lua_settable(L, -3);

    return 1;
}

static int Lzmq_init(lua_State *L)
{
    int app_threads = luaL_checkint(L, 1);
    int io_threads = luaL_checkint(L, 2);
    int flags = luaL_optint(L, 3, 0);

    zmq_ptr *ctx = lua_newuserdata(L, sizeof(zmq_ptr));

    ctx->ptr = zmq_init(app_threads, io_threads, flags);

    if (!ctx->ptr) {
        return luaL_error(L, zmq_strerror(zmq_errno()));
    }

    luaL_getmetatable(L, MT_ZMQ_CONTEXT);
    lua_setmetatable(L, -2);

    return 1;
}

static int Lzmq_term(lua_State *L)
{
    zmq_ptr *ctx = luaL_checkudata(L, 1, MT_ZMQ_CONTEXT);
    assert(zmq_term(ctx->ptr) == 0);
    return 0;
}

static int Lzmq_socket(lua_State *L)
{
    zmq_ptr *ctx = luaL_checkudata(L, 1, MT_ZMQ_CONTEXT);
    int type = luaL_checkint(L, 2);

    zmq_ptr *s = lua_newuserdata(L, sizeof(zmq_ptr));

    s->ptr = zmq_socket(ctx->ptr, type);

    if (!s->ptr) {
        return luaL_error(L, zmq_strerror(zmq_errno()));
    }

    luaL_getmetatable(L, MT_ZMQ_SOCKET);
    lua_setmetatable(L, -2);

    return 1;
}

static int Lzmq_close(lua_State *L)
{
    zmq_ptr *s = luaL_checkudata(L, 1, MT_ZMQ_SOCKET);
    assert(zmq_close(s->ptr) == 0);
    return 0;
}

static int Lzmq_setsockopt(lua_State *L)
{
    zmq_ptr *s = luaL_checkudata(L, 1, MT_ZMQ_SOCKET);
    int option = luaL_checkint(L, 2);

    int rc = 0;

    switch (option) {
    case ZMQ_HWM:
    case ZMQ_LWM:
    case ZMQ_SWAP:
    case ZMQ_AFFINITY:
        {
            int64_t optval = (int64_t) luaL_checklong(L, 3);
            rc = zmq_setsockopt(s->ptr, option, (void *) &optval, sizeof(int64_t));
        }
        break;
    case ZMQ_IDENTITY:
    case ZMQ_SUBSCRIBE:
    case ZMQ_UNSUBSCRIBE:
        {
            size_t optvallen;
            const char *optval = luaL_checklstring(L, 3, &optvallen);
            rc = zmq_setsockopt(s->ptr, option, (void *) optval, optvallen);
        }
        break;
    case ZMQ_RATE:
    case ZMQ_RECOVERY_IVL:
    case ZMQ_MCAST_LOOP:
    case ZMQ_SNDBUF:
    case ZMQ_RCVBUF:
        {
            uint64_t optval = (uint64_t) luaL_checklong(L, 3);
            rc = zmq_setsockopt(s->ptr, option, (void *) &optval, sizeof(uint64_t));
        }
        break;
    default:
        rc = -1;
        errno = EINVAL;
    }

    if (rc != 0) {
        return luaL_error(L, zmq_strerror(zmq_errno()));
    }
    return 0;
}

static int Lzmq_getsockopt(lua_State *L)
{
    zmq_ptr *s = luaL_checkudata(L, 1, MT_ZMQ_SOCKET);
    int option = luaL_checkint(L, 2);

    size_t optvallen;

    int rc = 0;

    switch (option) {
    case ZMQ_HWM:
    case ZMQ_LWM:
    case ZMQ_SWAP:
    case ZMQ_AFFINITY:
        {
            int64_t optval;
            rc = zmq_getsockopt(s->ptr, option, (void *) &optval, &optvallen);
            if (rc == 0) {
                lua_pushinteger(L, (lua_Integer) optval);
                return 1;
            }
        }
        break;
    /* case ZMQ_IDENTITY:
     * case ZMQ_SUBSCRIBE:
     * case ZMQ_UNSUBSCRIBE:
     */
    case ZMQ_RATE:
    case ZMQ_RECOVERY_IVL:
    case ZMQ_MCAST_LOOP:
    case ZMQ_SNDBUF:
    case ZMQ_RCVBUF:
    case ZMQ_RCVMORE:
        {
            uint64_t optval;
            rc = zmq_getsockopt(s->ptr, option, (void *) &optval, &optvallen);
            if (rc == 0) {
                lua_pushinteger(L, (lua_Integer) optval);
                return 1;
            }
        }
        break;
    default:
        rc = -1;
        errno = EINVAL;
    }

    if (rc != 0) {
        return luaL_error(L, zmq_strerror(zmq_errno()));
    }
    return 0;
}

static int Lzmq_bind(lua_State *L)
{
    zmq_ptr *s = luaL_checkudata(L, 1, MT_ZMQ_SOCKET);
    const char *addr = luaL_checkstring(L, 2);

    if (zmq_bind(s->ptr, addr) != 0) {
        return luaL_error(L, zmq_strerror(zmq_errno()));
    }

    return 0;
}

static int Lzmq_connect(lua_State *L)
{
    zmq_ptr *s = luaL_checkudata(L, 1, MT_ZMQ_SOCKET);
    const char *addr = luaL_checkstring(L, 2);

    if (zmq_connect(s->ptr, addr) != 0) {
        return luaL_error(L, zmq_strerror(zmq_errno()));
    }

    return 0;
}

static int Lzmq_send(lua_State *L)
{
    zmq_ptr *s = luaL_checkudata(L, 1, MT_ZMQ_SOCKET);
    size_t msg_size;
    const char *data = luaL_checklstring(L, 2, &msg_size);
    int flags = luaL_optint(L, 3, 0);

    zmq_msg_t msg;
    assert(zmq_msg_init_size(&msg, msg_size) == 0);
    memcpy(zmq_msg_data(&msg), data, msg_size);

    int rc = zmq_send(s->ptr, &msg, flags);

    assert(zmq_msg_close(&msg) == 0);

    if (rc != 0 && zmq_errno() == EAGAIN) {
        lua_pushboolean(L, 0);
        return 1;
    }

    if (rc != 0) {
        return luaL_error(L, zmq_strerror(zmq_errno()));
    }

    lua_pushboolean(L, 1);

    return 1;
}

static int Lzmq_recv(lua_State *L)
{
    zmq_ptr *s = luaL_checkudata(L, 1, MT_ZMQ_SOCKET);
    int flags = luaL_optint(L, 2, 0);

    zmq_msg_t msg;
    assert(zmq_msg_init(&msg) == 0);

    int rc = zmq_recv(s->ptr, &msg, flags);

    if (rc != 0 && zmq_errno() == EAGAIN) {
        assert(zmq_msg_close(&msg) == 0);
        lua_pushnil(L);
        return 1;
    }

    if (rc != 0) {
        assert(zmq_msg_close(&msg) == 0);
        return luaL_error(L, zmq_strerror(zmq_errno()));
    }

    lua_pushlstring(L, zmq_msg_data(&msg), zmq_msg_size(&msg));

    assert(zmq_msg_close(&msg) == 0);

    return 1;
}

static const luaL_reg zmqlib[] = {
    {"version",    Lzmq_version},
    {"init",       Lzmq_init},
    {NULL,         NULL}
};

static const luaL_reg ctxmethods[] = {
    {"term",       Lzmq_term}, 
    {"socket",     Lzmq_socket},
    {NULL,         NULL}
};

static const luaL_reg sockmethods[] = {
    {"close",   Lzmq_close},
    {"setopt",  Lzmq_setsockopt},
    {"getopt",  Lzmq_getsockopt},
    {"bind",    Lzmq_bind},
    {"connect", Lzmq_connect},
    {"send",    Lzmq_send},
    {"recv",    Lzmq_recv},
    {NULL,      NULL}
};

#define set_zmq_const(s) lua_pushinteger(L,ZMQ_##s); lua_setfield(L, -2, #s);

LUALIB_API int luaopen_zmq(lua_State *L)
{
    /* context metatable. */
    luaL_newmetatable(L, MT_ZMQ_CONTEXT);
    lua_createtable(L, 0, sizeof(ctxmethods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, ctxmethods);
    lua_setfield(L, -2, "__index");

    /* socket metatable. */
    luaL_newmetatable(L, MT_ZMQ_SOCKET);
    lua_createtable(L, 0, sizeof(sockmethods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, sockmethods);
    lua_setfield(L, -2, "__index");

    luaL_register(L, "zmq", zmqlib);

    set_zmq_const(PAIR);
    set_zmq_const(PUB);
    set_zmq_const(SUB);
    set_zmq_const(REQ);
    set_zmq_const(REP);
    set_zmq_const(XREQ);
    set_zmq_const(XREP);
    set_zmq_const(UPSTREAM);
    set_zmq_const(DOWNSTREAM);

    set_zmq_const(HWM);
    set_zmq_const(LWM);
    set_zmq_const(SWAP);
    set_zmq_const(AFFINITY);
    set_zmq_const(IDENTITY);
    set_zmq_const(SUBSCRIBE);
    set_zmq_const(UNSUBSCRIBE);
    set_zmq_const(RATE);
    set_zmq_const(RECOVERY_IVL);
    set_zmq_const(MCAST_LOOP);
    set_zmq_const(SNDBUF);
    set_zmq_const(RCVBUF);
    set_zmq_const(RCVMORE);

    set_zmq_const(NOBLOCK);
    set_zmq_const(SNDMORE);

    return 1;
}
