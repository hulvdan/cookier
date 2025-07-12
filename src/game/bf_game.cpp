#pragma once

const char* GetWindowTitle() {
  return "The Game"
#if BF_DEBUG
         " [DEBUG]"
#endif
    ;
}

SDL_AppResult GameUpdate() {
  auto texId = glib->background_texture_id();
  auto tex   = glib->atlas_textures()->Get(texId);

  Vector2 size{(f32)tex->size_x(), (f32)tex->size_y()};
  // auto    ratio = size * ;
  // ratio /= ge.meta.screenSize;

  auto scale = ScaleToFit(size, LOGICAL_RESOLUTION);
  // scale *= (f32)ge.meta.screenSize.x;
  // scale /= (f32)tex->size_x();

  DrawTexture({
    .texId = texId,
    .pos   = LOGICAL_RESOLUTION / 2,
    .scale = Vector2One() * scale,
  });
  // .rotation   =0,
  // .pos        ={},
  // .anchor     =Vector2Half(),
  // .scale      =,
  // .sourceSize =,
  // .color      =,
  return SDL_APP_CONTINUE;
}
