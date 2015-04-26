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

#define linit_c
#define LUA_LIB

#include "lprefix.h"
#include <stddef.h>

#include <picoos-lua.h>
#include "lualib.h"
#include "lauxlib.h"

static const luaL_Reg libs[] = {

/*
 * Modules from lua distribution.
 */

  { "_G", luaopen_base },
  { LUA_LOADLIBNAME, luaopen_package },
  { LUA_COLIBNAME, luaopen_coroutine },
  { LUA_TABLIBNAME, luaopen_table },
  { LUA_IOLIBNAME, luaopen_io },
  { LUA_STRLIBNAME, luaopen_string },
  { LUA_MATHLIBNAME, luaopen_math },
  { LUA_UTF8LIBNAME, luaopen_utf8 },
  { LUA_DBLIBNAME, luaopen_debug },
/*
 * Pico]OS pico layer module.
 */
  { PLUA_POSNAME, luaopen_pos },
  { NULL, NULL }
};

/*
 * Initialize libraries.
 */
LUALIB_API void luaL_openlibs (lua_State *L)
{
  const luaL_Reg *lib;

  for (lib = libs; lib->func; lib++) {

    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);
  }

  pluaAddLibs(L);
}

LUALIB_API void __attribute__((weak)) pluaAddLibs(lua_State *L)
{
}
