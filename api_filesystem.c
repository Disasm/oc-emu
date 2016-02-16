#include "api_filesystem.h"
#include "api_component.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <lauxlib.h>

static const char* get_path(lua_State* L, int selfIndex)
{
    lua_getfield(L, selfIndex, "__path");
    const char* path = lua_tostring(L, -1);
    lua_pop(L, 1);
    return path;
}

static int l_get_file(lua_State* L)
{
    const char* dirPath = get_path(L, 1);
    if (dirPath == 0)
    {
        fprintf(stderr, "Can't get path\n");
        lua_pushnil(L);
        return 1;
    }

    const char* fileName = luaL_checkstring(L, 2);
    
    char filePath[1024];
    sprintf(filePath, "%s/%s", dirPath, fileName);

    FILE* f = fopen(filePath, "r");
    if (f == 0)
    {
        fprintf(stderr, "Can't open file: %s\n", filePath);
        lua_pushnil(L);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* data = malloc(size);
    fread(data, 1, size, f);
    fclose(f);

    lua_pushlstring(L, data, size);
    return 1;
}

static int l_set_file(lua_State* L)
{
    const char* dirPath = get_path(L, 1);
    if (dirPath == 0) goto err;

    const char* fileName = luaL_checkstring(L, 2);

    int nil = lua_isnil(L, 3);

    const char* data = lua_tostring(L, 3);
    size_t size = lua_rawlen(L, 3);
    nil = size == 0;

    printf("filesystem: writing %d bytes (nil=%d)\n", (int)size, nil);

    char filePath[1024];
    sprintf(filePath, "%s/%s", dirPath, fileName);

    if (!nil)
    {
        FILE* f = fopen(filePath, "w");
        if (f == 0) goto err;
        fwrite(data, 1, size, f);
        fclose(f);
    }
    else
    {
        unlink(filePath);
    }

err:
    return 0;
}

static int l_read(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "__handle");
    if (lua_isnil(L, -1))
    {
        lua_pushnil(L);
        lua_pushstring(L, "invalid handle");
        return 2;
    }
    FILE* f = lua_touserdata(L, -1);
    lua_pop(L, 1);
   
    double dn = luaL_checknumber(L, 2);
    int n = (dn > 1000000)?1000000:dn;
    if (n > 0)
    {
        char* buf = malloc(n);
        n = fread(buf, 1, n, f);
        if (n == 0)
        {
            lua_pushnil(L);
            return 1;
        }
        if (n < 0)
        {
            lua_pushnil(L);
            lua_pushstring(L, "read error");
            return 2;
        }

        lua_pushlstring(L, buf, n);
        return 1;
    }
    else
    {
        lua_pushstring(L, "");
        return 1;
    }
}

static int l_close(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_getfield(L, 1, "__handle");
    if (lua_isnil(L, -1)) return 0;

    FILE* f = lua_touserdata(L, -1);
    lua_pop(L, 1);

    fclose(f);

    lua_pushnil(L);
    lua_setfield(L, 1, "__handle");

    return 0;
}

static int l_open(lua_State* L)
{
    const char* dirPath = get_path(L, 1);
    if (dirPath == 0) goto err;

    const char* fileName = luaL_checkstring(L, 2);
    if (strstr(fileName, "..")) goto err;
    
    const char* mode = luaL_checkstring(L, 3);

    char filePath[1024];
    snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, fileName);

    FILE* f = fopen(filePath, mode);
    if (f == 0) goto err;
    
    lua_newtable(L);
    
    lua_pushcfunction(L, l_read);
    lua_setfield(L, -2, "read");
    
    /*
    lua_pushcfunction(L, l_write);
    lua_setfield(L, -2, "write");
    
    lua_pushcfunction(L, l_seek);
    lua_setfield(L, -2, "seek");*/
    
    lua_pushcfunction(L, l_close);
    lua_setfield(L, -2, "close");

    lua_pushlightuserdata(L, f);
    lua_setfield(L, -2, "__handle");

    return 1;

err:
    printf("l_open error\n");
    return 0;
}

void api_filesystem_register(lua_State* L, const char* path, int eeprom)
{
    lua_newtable(L);

    lua_pushstring(L, path);
    lua_setfield(L, -2, "__path");

    lua_pushcfunction(L, l_get_file);
    lua_setfield(L, -2, "get_file");

    lua_pushcfunction(L, l_set_file);
    lua_setfield(L, -2, "set_file");

    lua_pushcfunction(L, l_open);
    lua_setfield(L, -2, "open");

    component_register(L, eeprom?"eeprom":"filesystem");
}

