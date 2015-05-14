/*
 * Copyright (c) 2015, Ari Suutari <ari@stonepile.fi>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
 
#include <picoos.h>
#include <picoos-lua.h>

#include "lauxlib.h"
 
#define POS_TASK "pos.task"

typedef struct {

  POSTASK_t task;
  int threadRef;

} LPOSTASK_t;

static const char* taskReg    = "posTaskReg";

static int ltask_sleep(lua_State *L)
{
  int ticks;
 
  ticks = luaL_checkinteger(L, 1);
  posTaskSleep(ticks);
  return 0;
}
 
/*
 * Save task object into registry.posTaskReg[picoo-os-task-handle]
 */
static void registerTask(lua_State* L)
{
// stk: utask

  lua_pushstring(L, taskReg);
  lua_gettable(L, LUA_REGISTRYINDEX);
  lua_insert(L, -2);
// stk: reg, utask

  lua_pushlightuserdata(L, posTaskGetCurrent());
  lua_insert(L, -2);
// stk: reg, taskhandle, utask

  lua_settable(L, -3); // set to reg
  lua_pop(L, 1);       // pop reg
}

/*
 * Remove task object from registry.posTaskReg[picoo-os-task-handle]
 */
static void deRegisterTask(lua_State* L)
{
  lua_pushstring(L, taskReg);
  lua_gettable(L, LUA_REGISTRYINDEX);

  lua_pushlightuserdata(L, posTaskGetCurrent());
  lua_gettable(L, -2);

  LPOSTASK_t* task = (LPOSTASK_t*) luaL_checkudata(L, -1, POS_TASK);

  luaL_unref(L, LUA_REGISTRYINDEX, task->threadRef);
  lua_pop(L, 1);

  lua_pushlightuserdata(L, posTaskGetCurrent());
  lua_pushnil(L);
  lua_settable(L, -3);
  lua_pop(L, 1);
}

/*
 * Main for new thread.
 */
static void taskMain(void* arg)
{
  lua_State* TL = (lua_State*)arg;

// stk: func, utask

  registerTask(TL);

//
// Now, call requested lua function.
//
  if (lua_pcall(TL, 0, 0, 0) != 0)
     nosPrintf("error in pcall\n");

  deRegisterTask(TL);
}

static int ltask_create(lua_State *L)
{
  LPOSTASK_t* task;
  int taskRef;

  if (lua_gettop(L) != 3)
    return luaL_error(L, "expecting exactly 3 arguments");

  int priority  = luaL_checkinteger(L, 1);
  int stackSize = luaL_checkinteger(L, 2);

  luaL_checktype(L, 3, LUA_TFUNCTION); 

// stk: pri, stk, func

  task = (LPOSTASK_t*)lua_newuserdata(L, sizeof(LPOSTASK_t));
  lua_pushvalue(L, -1);
  lua_insert(L, -3);
// stk: pri, stksiz, utask, func, utask

  lua_State* TL = lua_newthread(L);
  task->threadRef = luaL_ref(L, LUA_REGISTRYINDEX);
// stk: pri, stksiz, utask, func, utask

  luaL_getmetatable(L, POS_TASK);
  lua_setmetatable(L, -2);

//
// Move function and utask into task thread.
//
  lua_xmove(L, TL, 2);
// stk: pri, stksiz, utask

// 
// Create thread, pass lua thread as argument.
//
  task->task = posTaskCreate(taskMain, TL, priority, stackSize);
  return 1;
}
 
static int ltask_run(lua_State *L)
{
  return 0;
}
 
static const struct luaL_Reg ltask_methods[] = {

  { "run",         ltask_run       },
  { NULL,          NULL            },
};
 
static const struct luaL_Reg ltask_functions[] = {

  { "create",      ltask_create    },
  { "sleep",       ltask_sleep     },
  { NULL,          NULL            }
};
 
int luaopen_pua_task(lua_State *L)
{
// thread registry
  lua_pushstring(L, taskReg);
  lua_newtable(L);
  lua_settable(L, LUA_REGISTRYINDEX);

// task class
  luaL_newmetatable(L, POS_TASK);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, ltask_methods, 0);
  luaL_newlib(L, ltask_functions);

  return 1;
}
