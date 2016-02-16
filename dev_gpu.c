#include "dev_gpu.h"
#include "font.h"
#include "api_component.h"
#include <stdlib.h>
#include <string.h>
#include <lauxlib.h>

struct dev_gpu_s {
    uint32_t width;
    uint32_t height;

    uint32_t current_bg_color;
    uint32_t current_fg_color;

    char* framebuffer;
    uint32_t* colors_bg;
    uint32_t* colors_fg;
};

dev_gpu_t* dev_gpu_create(uint32_t width, uint32_t height)
{
    if (width == 0 || width > 1000 || height == 0 || height > 1000) return 0;

    dev_gpu_t* ctx = malloc(sizeof(dev_gpu_t));

    ctx->width = width;
    ctx->height = height;

    ctx->framebuffer = malloc(width * height);
    ctx->colors_bg = malloc(width * height * sizeof(uint32_t));
    ctx->colors_fg = malloc(width * height * sizeof(uint32_t));

    memset(ctx->framebuffer, 0x20, width*height);
    memset(ctx->colors_bg, 0, width*height*sizeof(uint32_t));
    memset(ctx->colors_fg, 0xff, width*height*sizeof(uint32_t));

    ctx->current_bg_color = 0x000000;
    ctx->current_fg_color = 0xffffff;

    return ctx;
}

static void draw_char(SDL_Surface* surface, int cx, int cy, char ch, uint32_t bg, uint32_t fg)
{
    uint16_t lines[FONT_HEIGHT];

    font_get_char(lines, ch);

    int sx = cx * FONT_WIDTH;
    int sy = cy * FONT_HEIGHT;

    int line, pos;
    for (line = 0; line < FONT_HEIGHT; line++)
    {
        int y = sy + line;
        for (pos = 0; pos < FONT_WIDTH; pos++)
        {
            int x = sx + pos;
            uint8_t pix = (lines[line] >> pos) & 1;
            uint8_t* ptr = (uint8_t*)surface->pixels + (y * surface->pitch) + (x * sizeof(uint32_t));
            *((uint32_t*)ptr) = pix ? fg:bg;
        }
    }
}

void dev_gpu_redraw(dev_gpu_t* ctx, SDL_Surface* surface)
{
    int x, y;

    for (y = 0; y < ctx->height; y++)
    {
        for (x = 0; x < ctx->width; x++)
        {
            int offset = y * ctx->width + x;

            draw_char(surface, x, y, ctx->framebuffer[offset], ctx->colors_bg[offset], ctx->colors_fg[offset]);
        }
    }
}

static int l_set(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int ch = luaL_checkinteger(L, 4);

    lua_getfield(L, 1, "ctx");
    dev_gpu_t* ctx = lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (y < 0 || y >= ctx->height) return 0;
    if (x < 0 || x >= ctx->width) return 0;

    int offset = y * ctx->width + x;
    ctx->framebuffer[offset] = ch;
    ctx->colors_bg[offset] = ctx->current_bg_color;
    ctx->colors_fg[offset] = ctx->current_fg_color;

    return 0;
}

static int l_get(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);

    lua_getfield(L, 1, "ctx");
    dev_gpu_t* ctx = lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (y < 0 || y >= ctx->height) return 0;
    if (x < 0 || x >= ctx->width) return 0;

    int offset = y * ctx->width + x;
    char buf[2];
    buf[1] = 0;
    buf[0] = ctx->framebuffer[offset];
    lua_pushstring(L, buf);
    lua_pushinteger(L, ctx->colors_fg[offset]);
    lua_pushinteger(L, ctx->colors_bg[offset]);

    return 3;
}

static int l_set_color(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int type = luaL_checkinteger(L, 2);
    int color = luaL_checkinteger(L, 3);

    lua_getfield(L, 1, "ctx");
    dev_gpu_t* ctx = lua_touserdata(L, -1);
    lua_pop(L, 1);

    uint32_t* ptr;
    if (type == 1)
    {
        ptr = &ctx->current_fg_color;
    }
    else
    {
        ptr = &ctx->current_bg_color;
    }
    lua_pushinteger(L, *ptr);
    *ptr = color;
    return 1;
}

static int l_get_color(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int type = luaL_checkinteger(L, 2);

    lua_getfield(L, 1, "ctx");
    dev_gpu_t* ctx = lua_touserdata(L, -1);
    lua_pop(L, 1);

    int color;
    if (type == 1)
    {
        color = ctx->current_fg_color;
    }
    else
    {
        color = ctx->current_bg_color;
    }

    lua_pushinteger(L, color);
    return 1;
}

static int l_copy(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int w = luaL_checkinteger(L, 4);
    int h = luaL_checkinteger(L, 5);
    int tx = luaL_checkinteger(L, 6);
    int ty = luaL_checkinteger(L, 7);

    lua_getfield(L, 1, "ctx");
    dev_gpu_t* ctx = lua_touserdata(L, -1);
    lua_pop(L, 1);

    char ok = 0;

    if (w <= 0 || h <= 0) goto end;

    int xi, yi;
    for (yi = 0; yi < h; yi++)
    {
        int y1s = y + yi;
        int y1t = ty + yi;
        if (ty > y)
        {
            y1s = y + h - 1 - yi;
            y1t = ty + h - 1 - yi;
        }
        if (y1s < 0 || y1s >= ctx->height || y1t < 0 || y1t >= ctx->height) continue;

        for (xi = 0; xi < w; xi++)
        {
            int x1s = x + xi;
            int x1t = tx + xi;
            if (tx > x)
            {
                x1s = x + w - 1 - xi;
                x1t = tx + w - 1 - xi;
            }
            if (x1s < 0 || x1s >= ctx->width || x1t < 0 || x1t >= ctx->width) continue;
            
            int so = y1s * ctx->width + x1s;
            int to = y1t * ctx->width + x1t;
            ctx->framebuffer[to] = ctx->framebuffer[so];
            ctx->colors_bg[to] = ctx->colors_bg[so];
            ctx->colors_fg[to] = ctx->colors_fg[so];
        }
    }
    ok = 1;

end:
    lua_pushboolean(L, ok);
    return 1;
}


void dev_gpu_register(lua_State* L, dev_gpu_t* ctx)
{
    lua_newtable(L);
 
    lua_pushcfunction(L, l_set);
    lua_setfield(L, -2, "set");
    
    lua_pushcfunction(L, l_get);
    lua_setfield(L, -2, "get");
    
    lua_pushcfunction(L, l_set_color);
    lua_setfield(L, -2, "set_color");
    
    lua_pushcfunction(L, l_get_color);
    lua_setfield(L, -2, "get_color");
    
    lua_pushcfunction(L, l_copy);
    lua_setfield(L, -2, "copy");

    lua_pushlightuserdata(L, ctx);
    lua_setfield(L, -2, "ctx");

    lua_setglobal(L, "int_gpu");
}

