#
# Copyright (c) 2012-2013, Ari Suutari <ari@stonepile.fi>.
# All rights reserved. 
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. The name of the author may not be used to endorse or promote
#     products derived from this software without specific prior written
#     permission. 
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
# INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.

#
# Compile OneWire library using Pico]OS library Makefile
#

RELROOT = ../picoos/
PORT ?= cortex-m
BUILD ?= RELEASE
EXTRA_CFLAGS += -Wno-cast-align

include $(RELROOT)make/common.mak

TARGET = picoos-lua
SRC_TXT =	task.c			\
		init.c			\
		pico.c			\
		lock.c			\
		lua/src/lapi.c		\
		lua/src/lauxlib.c	\
		lua/src/lbaselib.c	\
		lua/src/lbitlib.c	\
		lua/src/lcode.c		\
		lua/src/lcorolib.c	\
		lua/src/lctype.c	\
		lua/src/ldblib.c	\
		lua/src/ldebug.c	\
		lua/src/ldo.c		\
		lua/src/ldump.c		\
		lua/src/lfunc.c		\
		lua/src/lgc.c		\
		lua/src/liolib.c	\
		lua/src/llex.c		\
		lua/src/lmathlib.c	\
		lua/src/lmem.c		\
		lua/src/loadlib.c	\
		lua/src/lobject.c	\
		lua/src/lopcodes.c	\
		lua/src/lparser.c	\
		lua/src/lstate.c	\
		lua/src/lstring.c	\
		lua/src/lstrlib.c	\
		lua/src/ltable.c	\
		lua/src/ltablib.c	\
		lua/src/ltm.c		\
		lua/src/lundump.c	\
		lua/src/lutf8lib.c	\
		lua/src/lvm.c		\
		lua/src/lzio.c		

SRC_HDR =	lua/src/lapi.h		\
		lua/src/lauxlib.h	\
		lua/src/lcode.h		\
		lua/src/lctype.h	\
		lua/src/ldebug.h	\
		lua/src/ldo.h		\
		lua/src/lfunc.h		\
		lua/src/lgc.h		\
		lua/src/llex.h		\
		lua/src/llimits.h	\
		lua/src/lmem.h		\
		lua/src/lobject.h	\
		lua/src/lopcodes.h	\
		lua/src/lparser.h	\
		lua/src/lprefix.h	\
		lua/src/lstate.h	\
		lua/src/lstring.h	\
		lua/src/ltable.h	\
		lua/src/ltm.h		\
		lua/src/lua.h		\
		lua/src/luaconf.h	\
		lua/src/lualib.h	\
		lua/src/lundump.h	\
		lua/src/lvm.h		\
		lua/src/lzio.h

SRC_HDR =	
SRC_OBJ =
DIR_USRINC +=   lua/src

ifeq '$(strip $(DIR_OUTPUT))' ''
DIR_OUTPUT = $(CURRENTDIR)/bin
endif

include $(MAKE_LIB)

