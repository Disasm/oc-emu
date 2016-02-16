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
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                SDL_MouseButtonEvent *ev = &event->button;

                const char* button = 0;
                switch (ev->button)
                {
                    case SDL_BUTTON_LEFT:
                        button = "left";
                        break;
                    case SDL_BUTTON_RIGHT:
                        button = "right";
                        break;
                    case SDL_BUTTON_MIDDLE:
                        button = "middle";
                        break;
                }
                lua_pushstring(L, (event->type == SDL_MOUSEBUTTONDOWN)?"mouse_down":"mouse_up");
                lua_pushinteger(L, ev->x);
                lua_pushinteger(L, ev->y);
                if (button != 0) lua_pushstring(L, button);
                else lua_pushnil(L);

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

int load_core_lua(lua_State* L, char** args)
{
    int error;

    error = luaL_loadfile(L, "core.lua");
    if (error) goto err;

    int n = 0;
    while (*args)
    {
        lua_pushstring(L, *args);
        n++; args++;
    }
    
    error = lua_pcall(L, n, 0, 0);
    if (error) goto err;

err:
    if (error)
    {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);  // pop error message from the stack
    }
    return -1;
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

    load_core_lua(L, args);
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

    font_init();
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
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
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
