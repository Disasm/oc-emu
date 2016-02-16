#ifndef DEV_GPU_H
#define DEV_GPU_H

#include <stdint.h>
#include <SDL.h>
#include <lua.h>

typedef struct dev_gpu_s dev_gpu_t;

dev_gpu_t* dev_gpu_create(uint32_t width, uint32_t height);
void dev_gpu_redraw(dev_gpu_t* ctx, SDL_Surface* surface);

void dev_gpu_register(lua_State* L, dev_gpu_t* ctx);

#endif /* DEV_SCREEN_H */

