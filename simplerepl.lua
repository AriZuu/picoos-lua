--
-- Copyright (c) 2015, Ari Suutari <ari@stonepile.fi>.
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
--
--  1. Redistributions of source code must retain the above copyright
--     notice, this list of conditions and the following disclaimer.
--  2. Redistributions in binary form must reproduce the above copyright
--     notice, this list of conditions and the following disclaimer in the
--     documentation and/or other materials provided with the distribution.
--  3. The name of the author may not be used to endorse or promote
--     products derived from this software without specific prior written
--     permission.
--
-- THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
-- OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
-- ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
-- INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
-- SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
-- HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
-- STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
-- ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
-- OF THE POSSIBILITY OF SUCH DAMAGE.


-- 
-- Simple REPL class for lua "console".
--

SimpleREPL = {}
SimpleREPL.__index = SimpleREPL

local stdout = io.stdout
local stdin  = io.stdin

function SimpleREPL.new(prompt)
  local self = setmetatable({}, SimpleREPL)
  self.prompt = prompt
  return self
end

function SimpleREPL.fixRet(success, ...)
  local cnt = select('#', ...)
  return success, cnt, { ... }
end

function SimpleREPL:repl()

  stdout:write(self.prompt .. ' ')
  for line in stdin:lines() do

-- try with 'return'
    ch,err = load('return ' .. line, "=stdin")
    if err then
       ch,err = load(line, "=stdin")
       if err and string.match(err, "<eof>$") then
  -- try multiline
  
         buf = line
         stdout:write(self.prompt .. self.prompt:sub(#self.prompt) .. ' ')
         for line in stdin:lines() do
     
           buf = buf .. line
           ch,err = load(buf, "=stdin")
           if ch or not string.match(err, "<eof>$") then
             break
           end
           stdout:write(self.prompt .. self.prompt:sub(#self.prompt) .. ' ')
         end
       end
    end
  
    if (ch) then
      status, n, x = SimpleREPL.fixRet(xpcall(ch, debug.traceback))
   
      if status then
        if n > 0 then
         
          for i=1,n do
            if i > 1 then
              io.write("\t")
            end
            io.write (tostring(x[i]))
          end
          print()
        end
      else
        print(x[1])
      end
    else
      print ("error", err)
    end
  
    stdout:write(self.prompt .. ' ')
  end
end

return SimpleREPL
