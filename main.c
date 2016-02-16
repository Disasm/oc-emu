#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "font.h"
#include "api_component.h"
#include "dev_gpu.h"
#include "api_filesystem.h"
#include "api_computer.h"
#include "lfs.h"

#define FPS 60

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

dev_gpu_t* gpu = 0;

#define MAX_SDL_EVENTS 16
SDL_Event events[MAX_SDL_EVENTS];
int events_cnt = 0;
SDL_mutex* events_mutex;

void dumpStack(lua_State* L)
{
    int top = lua_gettop(L);
    int i;
    for (i = -top; i <= -1; i++)
    {
        printf("[%d]=", i);
        if (lua_isnil(L, i))
        {
            printf("nil");
        }
        else if (lua_isstring(L, i))
        {
            printf("'%s'", lua_tostring(L, i));
        }
        else if (lua_isnumber(L, i))
        {
            double n = lua_tonumber(L, i);
            printf("%lf", n);
        }
/*        else if (lua_istable(L, i))
        {
            printf("<table>");
        }
        else if (lua_isfunction(L, i))
        {
            printf("<func>");
        }*/
        else if (lua_isboolean(L, i))
        {
            printf("%s", lua_toboolean(L, i)?"true":"false");
        }
        else
        {
            int type = lua_type(L, i);
            printf("<%s>", lua_typename(L, type));
        }
        printf(" ");
    }
    printf("\n");
}

#include "scancode.h"
int l_get_event(lua_State* L)
{
    int n = 0;
    SDL_mutexP(events_mutex);
    if (events_cnt > 0)
    {
        SDL_Event* event = &events[0];
        switch(event->type)
        {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                {
                SDL_KeyboardEvent *key = &event->key;

                int scancode = key->keysym.scancode;
                struct k_info info = keymap[scancode];
                char ch = 0;
                if (info.type == K_UNKNOWN)
                {
                    scancode = 0;
                }
                else
                {
                    scancode = info.code;
                    ch = info.ch;
                }
                printf("Scancode: 0x%02x\n", scancode);
                if (ch == 0)
                {
                    const char* name = SDL_GetKeyName(key->keysym.sym);
                    printf("Char: '%s'\n", name);
                    char ch = 0;
                    if (strlen(name) == 1) ch = tolower(name[0]);
                }

                lua_pushstring(L, (event->type == SDL_KEYDOWN)?"key_down":"key_up");
                lua_pushinteger(L, ch);
                lua_pushinteger(L, scancode);

                n = 3;

                break;
                }
        }
        memmove(event, event+1, sizeof(event)*(events_cnt-1));
        events_cnt--;
    }
    SDL_mutexV(events_mutex);

    return n;
}

int load_core_lua(lua_State* L)
{
    int error;

    error = luaL_loadfile(L, "core.lua");
    if (error) goto err;
    
    error = lua_pcall(L, 0, 0, 0);
    if (error) goto err;

err:
    if (error)
    {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);  // pop error message from the stack
    }
    return -1;
}

#include "machine.h"
#include "init.h"
int load_precompiled_code(lua_State* L)
{
    int error;

    error = luaL_loadbuffer(L, (char*)init_bin, init_bin_len, "init.lua") || lua_pcall(L, 0, 0, 0);
    if (error) goto err;

    //dumpGlobals(L);
    
    error = luaL_loadbuffer(L, (char*)machine_bin, machine_bin_len, "machine.lua");
    if (error) goto err;

#if 0
    error = lua_pcall(L, 0, 0, 0);
    if (error) goto err;
#else
    lua_getglobal(L, "coroutine");
    lua_getfield(L, -1, "create");
    lua_pushvalue(L, -3);
    error = lua_pcall(L, 1, 1, 0);
    if (error) goto err;

    while (1)
    {
        //printf("top1=%d\n", lua_gettop(L));
        //dumpStack(L);
        lua_getfield(L, -2, "resume");
        lua_pushvalue(L, -2);
        //printf("top2=%d\n", lua_gettop(L));
        //dumpStack(L);
        error = lua_pcall(L, 1, 2, 0);
        //printf("top3=%d\n", lua_gettop(L));
        //dumpStack(L);
        if (error) goto err;

        //printf("yield: ");
        /*int i;
        for (i = -2; i <= -1; i++)
        {
            if (i != -2) printf(", ");
            const char* s = lua_tostring(L, i);
            printf("%s", s);
        }
        printf("\n");*/
        lua_pop(L, 2);
        //printf("top4=%d\n", lua_gettop(L));
    }
#endif

err:
    if (error)
    {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);  // pop error message from the stack
        return -1;
    }

    return 0;
}

void dumpGlobals(lua_State * L)
{
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	lua_pushinteger(L, LUA_RIDX_GLOBALS);
	lua_gettable(L, -2);
    lua_remove(L, -2);

    //getGlobals(L);

    lua_pushnil(L);
    while (lua_next(L, -2))
    {
        if (lua_isstring(L, -2))
        {
            const char * key = lua_tostring(L, -2);
            if (key != 0) printf("==> '%s'\n", key);
        }
        if (lua_isnumber(L, -2))
        {
            int n = lua_tointeger(L, -2);
            printf("==> %d\n", n);
        }

        lua_pop(L, 1); // pop value
    }
    lua_pop(L, 1); // pop table
}

int l_mseconds(lua_State* L)
{
    double ticks = SDL_GetTicks();
    lua_pushnumber(L, ticks);

    return 1;
}

void lua_reg_stub_module(lua_State* L, const char* name)
{
    lua_newtable(L);
    component_register(L, name);
}

int lua_thread_terminated = 0;

int lua_thread(void* param)
{
    char** args = param;
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);

    lua_pushcfunction(L, l_mseconds);
    lua_setglobal(L, "mseconds");
    
    lua_pushcfunction(L, l_get_event);
    lua_setglobal(L, "get_event");
    
    component_init(L);
    dev_gpu_register(L, gpu);
    api_computer_register(L);
    lua_reg_stub_module(L, "screen");
    lua_reg_stub_module(L, "keyboard");

    int i;
    for (i = 0; args[i]; i++)
    {
        api_filesystem_register(L, args[i], (i==0)?1:0);
    }

    load_core_lua(L);
    //if (load_precompiled_code(L) < 0) goto end;

end:
    lua_close(L);
    lua_thread_terminated = 1;
    return 0;
}

int main(int argc, char* argv[])
{
    SDL_Window* window = 0;
    SDL_Surface* surface = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <eeprom_dir> <hdd_dir> [<hdd_dir>]\n", argv[0]);
        return -1;
    }

    char** args = malloc(argc * sizeof(char*));
    int i;
    for (i = 1; i < argc; i++) args[i-1] = argv[i];
    args[argc-1] = 0;

    font_load("fonts/koi8r-8x16");
    gpu = dev_gpu_create(SCREEN_WIDTH, SCREEN_HEIGHT);

    events_mutex = SDL_CreateMutex();

    SDL_Thread *thread = SDL_CreateThread(lua_thread, "Lua thread", args);
    if (!thread)
    {
        fprintf(stderr, "Can't create lua thread\n");
        goto end;
    }

    SDL_Init(SDL_INIT_VIDEO);           // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "OpenComputers Emulator",                  // window title
        SDL_WINDOWPOS_UNDEFINED,        // initial x position
        SDL_WINDOWPOS_UNDEFINED,        // initial y position
        SCREEN_WIDTH * FONT_WIDTH,      // width, in pixels
        SCREEN_HEIGHT * FONT_HEIGHT,    // height, in pixels
        0/*SDL_WINDOW_OPENGL*/          // flags - see below
    );

    // Check that the window was successfully created
    if (window == 0)
    {
        // In the case that the window could not be made...
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    surface = SDL_GetWindowSurface(window);

    uint32_t t0 = SDL_GetTicks();
    for(;;)
    {
        if (lua_thread_terminated) break;

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    goto end;

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    SDL_mutexP(events_mutex);
                    if (events_cnt < MAX_SDL_EVENTS)
                    {
                        memcpy(&events[events_cnt], &event, sizeof(event));
                        events_cnt++;
                    }
                    SDL_mutexV(events_mutex);
                    break;
            }
        }

        uint32_t t = SDL_GetTicks();
        uint32_t dt = t - t0;
        if (dt > (1000 / FPS))
        {
            t0 = t;

            dev_gpu_redraw(gpu, surface);
            SDL_UpdateWindowSurface(window);
        }
    }
end:
    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();

    return 0;
}
