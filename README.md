This library contains [LUA interpreter][1] and integrates it into
Pico]OS. As LUA is an interpreted language, it is not suitable
for writing hard-realtime parts in application. But it could
be very useful in secondary tasks, like communication to
some external systems or user interfaces.

This is currently full LUA 5.3.0, except posix os module (pipes etc),
which is not suitable for Pico]OS.

[1]: http://lua.org
