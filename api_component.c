#include "api_component.h"
#include "uuid.h"

#define COMPONENT_TABLE "intcomponent"

static int l_gen_uuid(lua_State* L)
{
    lua_pushstring(L, uuid_gen());
    return 1;
}

void component_init(lua_State* L)
{
    lua_newtable(L);

    lua_pushstring(L, "gen_uuid");
    lua_pushcfunction(L, l_gen_uuid);
    lua_settable(L, -3);

    lua_pushstring(L, "list");
    lua_newtable(L);
    lua_settable(L, -3);

    lua_setglobal(L, COMPONENT_TABLE);
}

const char* component_register(lua_State* L, const char* type)
{
    lua_pushstring(L, "type");
    lua_pushstring(L, type);
    lua_settable(L, -3);

    lua_getglobal(L, COMPONENT_TABLE);

    lua_pushstring(L, "list");
    lua_gettable(L, -2);

    l_gen_uuid(L);
    const char* uuid = lua_tostring(L, -1);
    lua_pushvalue(L, -4);
    lua_settable(L, -3);
    lua_pop(L, 3);

    return uuid;
}

