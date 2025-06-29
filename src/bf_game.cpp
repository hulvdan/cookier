#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shaders/quad_vs_s_5_0.bin"
#include "shaders/quad_fs_s_5_0.bin"

///
struct Texture2D {
  int   w    = {};
  int   h    = {};
  void* data = {};

  bgfx_texture_handle_t tex = {};
};

///
Texture2D LoadTexture(const char* path) {
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
void UnloadTexture(Texture2D* texture) {
  ASSERT(texture->data);
  bgfx_destroy_texture(texture->tex);
  texture->tex = {};
  stbi_image_free(texture->data);
  texture->data = nullptr;
}

struct GameData {
  Texture2D tex = {};
} g = {};

void Draw() {
  const auto texturePath = "c:/Users/user/dev/home/isaac/assets/tex.png";

  LOCAL_PERSIST bool loaded = false;

  if (!loaded) {
    loaded = true;

    g.tex = LoadTexture(texturePath);

    // bgfx_shader_handle_t vsh
    //   = bgfx_create_shader(bgfx_make_ref(quad_vs_s_5_0, sizeof(quad_vs_s_5_0)));
    // bgfx_shader_handle_t fsh
    //   = bgfx_create_shader(bgfx_make_ref(quad_fs_s_5_0, sizeof(quad_fs_s_5_0)));
    // bgfx_program_handle_t program = bgfx_create_program(vsh, fsh, true);
  }
}

///
