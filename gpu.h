#ifndef GPU_H
#define GPU_H

#include <stdint.h>
#include <SDL.h>
#include <lua.h>

typedef struct gpu_s gpu_t;

gpu_t* gpu_create(uint32_t width, uint32_t height);
void gpu_redraw(gpu_t* ctx, SDL_Surface* surface);

void gpu_register(lua_State* L, gpu_t* ctx);

#endif /* GPU_H */

