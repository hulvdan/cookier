#pragma once

#include <bgfx/c99/bgfx.h>
#include "../bf_lib.cpp"

#if BF_ENGINE_IMPLEMENTATION
#  define BF_ENGINE_EXPORT
#else
#  define BF_ENGINE_EXPORT extern
#endif

///
struct Texture2D {
  int   w    = {};
  int   h    = {};
  void* data = {};

  bgfx_texture_handle_t tex = {};
};

BF_ENGINE_EXPORT Texture2D LoadTexture(const char* path);
BF_ENGINE_EXPORT void      UnloadTexture(Texture2D* texture);
BF_ENGINE_EXPORT u32       GetTicks();

#ifdef BF_ENGINE_IMPLEMENTATION

#  include <SDL3/SDL.h>

#  define STB_IMAGE_IMPLEMENTATION
#  include "stb_image.h"

///
BF_ENGINE_EXPORT Texture2D LoadTexture(const char* path) {
  Texture2D result{};
  int       channels = 0;
  result.data        = stbi_load(path, &result.w, &result.h, &channels, 4);
  ASSERT(result.data);

  auto memory = bgfx_copy(result.data, result.w * result.h * 4);

  result.tex = bgfx_create_texture_2d(
    result.w,
    result.h,
    false /* _hasMips*/,
    1,
    BGFX_TEXTURE_FORMAT_RGBA8,
    BGFX_SAMPLER_MIN_POINT      //
      | BGFX_SAMPLER_MAG_POINT  //
      | BGFX_SAMPLER_MIP_POINT  //
      | BGFX_SAMPLER_U_CLAMP    //
      | BGFX_SAMPLER_V_CLAMP,
    memory
  );

  return result;
}

///
BF_ENGINE_EXPORT void UnloadTexture(Texture2D* texture) {
  ASSERT(texture->data);
  bgfx_destroy_texture(texture->tex);
  texture->tex = {};
  stbi_image_free(texture->data);
  texture->data = nullptr;
}

///
BF_ENGINE_EXPORT u32 GetTicks() {
  return SDL_GetTicks();
}

#endif  // BF_ENGINE_IMPLEMENTATION

///
