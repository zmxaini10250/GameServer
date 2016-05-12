#include <iostream>

extern "C" 
{
	#include "../../LuaTinker/lua.h"
	#include "../../LuaTinker/lualib.h"
	#include "../../LuaTinker/lauxlib.h"
};

#include "../../LuaTinker/lua_tinker.h"

int cpp_func(int arg1, int arg2)
{
	return arg1 + arg2;
}

int main()
{
	lua_State* L = lua_open();

	luaopen_base(L);

	lua_tinker::def(L, "cpp_func", cpp_func);

	lua_tinker::dofile(L, "sample1.lua");

	int result = lua_tinker::call<int>(L, "lua_func", 3, 4);

	printf("lua_func(3,4) = %d\n", result);

	lua_close(L);

	return 0;
}
