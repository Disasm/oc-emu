#ifndef API_COMPONENT_H
#define API_COMPONENT_H

#include <lua.h>

void component_init(lua_State* L);

const char* component_register(lua_State* L, const char* type); // ( table -- )

#endif /* API_COMPONENT_H */

