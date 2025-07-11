#pragma once

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;

#define LOGI(...) SDL_Log(__VA_ARGS__)
#define LOGW(...) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#define LOGE(...) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

struct Color {
  u8 r = u8_max;
  u8 g = u8_max;
  u8 b = u8_max;
  u8 a = u8_max;
};

struct Texture2D {
  int   w    = {};
  int   h    = {};
  void* data = {};

  bgfx_texture_handle_t tex = {};
};

const BFGame::GameLibrary* glib = nullptr;

struct EngineData {
  struct Meta {
    Texture2D atlas        = {};
    void*     gamelibBytes = {};
  } meta;
} ge = {};

///
Texture2D LoadTexture(const char* filepath) {
  Texture2D result{};

  int channels = 0;
  result.data  = stbi_load(filepath, &result.w, &result.h, &channels, 4);
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
  bgfx_destroy_texture(*(bgfx_texture_handle_t*)&texture->tex);
  stbi_image_free(texture->data);
  *texture = {};
}

void* LoadFileData(const char* filepath, int* out_size = nullptr) {
  auto fp = fopen(filepath, "rb");
  ASSERT(fp);
  if (!fp) {
    perror("Failed to open file");
    LOGE("Failed to open file %s", filepath);
    INVALID_PATH;
    return nullptr;
  }

  fseek(fp, 0, SEEK_END);
  auto size = ftell(fp);
  rewind(fp);

  auto buffer = malloc(size + 1);
  if (!buffer) {
    LOGE("Failed to allocate buffer while opening file");
    fclose(fp);
    INVALID_PATH;
    return nullptr;
  }

  auto read_size = fread(buffer, 1, size, fp);
  if (read_size != size) {
    LOGE("Failed to read entire file");
    free(buffer);
    fclose(fp);
    return nullptr;
  }

  ((u8*)buffer)[size] = '\0';
  fclose(fp);

  if (out_size)
    *out_size = size;

  return buffer;
}

void UnloadFileData(void* ptr) {
  free(ptr);
}

void InitializeEngine() {
  ge.meta.atlas        = LoadTexture("resources/atlas.png");
  ge.meta.gamelibBytes = LoadFileData("resources/gamelib.bin");
  LOGI("Initialized engine!");
  glib = BFGame::GetGameLibrary(ge.meta.gamelibBytes);
}

struct DrawTextureData {
  int     texId      = -1;
  f32     rotation   = {};
  Vector2 pos        = {};
  Vector2 anchor     = Vector2Half();
  Vector2 scale      = {1, 1};
  Vector2 sourceSize = {1, 1};
  Color   color      = WHITE;

  // Shader* shader               = nullptr;
  // int     materialsBufferStart = -1;
};

void DrawTexture(DrawTextureData data) {
  ASSERT(data.texId >= 0);

  auto tex = glib->atlas_textures()->Get(data.texId);

  Rectangle sourceRec{
    (f32)tex->atlas_x(),
    (f32)tex->atlas_y() + (f32)tex->size_y() * (1 - data.sourceSizeY),
    (f32)tex->size_x() * SIGN(data.scale.x),
    (f32)tex->size_y() * data.sourceSizeY * SIGN(data.scale.y),
  };
  Rectangle destRec{
    data.pos.x
      + (f32)tex->size_x() * (1 - data.sourceSizeX) * abs(data.scale.x) * data.anchor.x,
    data.pos.y
      + (f32)tex->size_y() * (1 - data.sourceSizeY) * abs(data.scale.y) * data.anchor.y,
    (f32)tex->size_x() * data.sourceSizeX * abs(data.scale.x),
    (f32)tex->size_y() * data.sourceSizeY * abs(data.scale.y),
  };

  DrawTexturePro(
    g->meta.atlasTexture,
    sourceRec,
    destRec,
    GetRectangleSize(destRec) * data.anchor,
    data.rotationDeg,
    data.color
  );
}

///
u64 GetTime() {
  return SDL_GetTicks();
}

enum UpdateFunctionResult {
  UpdateFunctionResult_NO_ERROR_CONTINUE_RUNNING = -1,
  UpdateFunctionResult_FINISHED_SUCCESSFULLY     = 0,
  UpdateFunctionResult_ERR                       = 1,
};

///
