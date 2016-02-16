#include "api_computer.h"
#include "api_component.h"

void api_computer_register(lua_State* L)
{
    lua_newtable(L);
    component_register(L, "computer");
}

