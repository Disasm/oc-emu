#ifndef API_FILESYSTEM_H
#define API_FILESYSTEM_H

#include <lua.h>

void api_filesystem_register(lua_State* L, const char* path, int eeprom);

#endif
