#pragma once

#include "box2d/box2d.h"

// Clay.
// ============================================================ {  ///
#define CLAY_IMPLEMENTATION
#include "clay.h"

#define BF_CLAY_PADDING_ALL(v)      \
  .padding {                        \
    (u16) v, (u16)v, (u16)v, (u16)v \
  }
#define BF_CLAY_PADDING_HORIZONTAL(v) \
  .padding {                          \
    (u16) v, (u16)v, 0, 0             \
  }
#define BF_CLAY_PADDING_VERTICAL(v) \
  .padding {                        \
    0, 0, (u16)v, (u16)v            \
  }
#define BF_CLAY_PADDING_LEFT(v) \
  .padding {                    \
    (u16) v, 0, 0, 0            \
  }
#define BF_CLAY_PADDING_RIGHT(v) \
  .padding {                     \
    0, (u16)v, 0, 0              \
  }
#define BF_CLAY_PADDING_TOP(v) \
  .padding {                   \
    0, 0, (u16)v, 0            \
  }
#define BF_CLAY_PADDING_BOTTOM(v) \
  .padding {                      \
    0, 0, 0, (u16)v               \
  }

#define BF_CLAY_CHILD_ALIGNMENT_LEFT_TOP          \
  .childAlignment {                               \
    .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP \
  }
#define BF_CLAY_CHILD_ALIGNMENT_CENTER_TOP          \
  .childAlignment {                                 \
    .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP \
  }
#define BF_CLAY_CHILD_ALIGNMENT_RIGHT_TOP          \
  .childAlignment {                                \
    .x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_TOP \
  }
#define BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER          \
  .childAlignment {                                  \
    .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER \
  }
#define BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER          \
  .childAlignment {                                    \
    .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER \
  }
#define BF_CLAY_CHILD_ALIGNMENT_RIGHT_CENTER          \
  .childAlignment {                                   \
    .x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_CENTER \
  }
#define BF_CLAY_CHILD_ALIGNMENT_LEFT_BOTTOM          \
  .childAlignment {                                  \
    .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_BOTTOM \
  }
#define BF_CLAY_CHILD_ALIGNMENT_CENTER_BOTTOM          \
  .childAlignment {                                    \
    .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_BOTTOM \
  }
#define BF_CLAY_CHILD_ALIGNMENT_RIGHT_BOTTOM          \
  .childAlignment {                                   \
    .x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_BOTTOM \
  }

#define BF_CLAY_TEXT_LOCALIZED_DANGER(locale_)               \
  {                                                          \
    auto        string = localization_strings->Get(locale_); \
    Clay_String text{                                        \
      .isStaticallyAllocated = true,                         \
      .length                = (i32)string->size(),          \
      .chars                 = string->c_str(),              \
    };                                                       \
    BF_CLAY_TEXT(text);                                      \
  }

struct Beautify {
  u16     alpha     = u16_max;
  Vector2 translate = {0, 0};
  Vector2 scale     = {1, 1};
};

#define BEAUTIFY(beautify_)                                                            \
  CLAY({                                                                               \
    .custom{                                                                           \
      .customData = PushClayCustomData({                                               \
        .type      = ClayCustomElementType_BEAUTIFIER_START,                           \
        .alpha     = (beautify_).alpha,                                                \
        .translate = (beautify_).translate,                                            \
        .scale     = (beautify_).scale,                                                \
      }),                                                                              \
    },                                                                                 \
  }) {}                                                                                \
  beautifiers[beautifiersCount++] = (beautify_);                                       \
                                                                                       \
  DEFER {                                                                              \
    CLAY({.custom{                                                                     \
      .customData = PushClayCustomData({.type = ClayCustomElementType_BEAUTIFIER_END}) \
    }}) {}                                                                             \
    beautifiersCount--;                                                                \
  };

#define FLOATING_BEAUTIFY FLOATING_BEAUTIFY_CONDITIONAL(true)

#define FLOATING_BEAUTIFY_CONDITIONAL(cond)                                          \
  f32     currentAlpha_ = 1;                                                         \
  Vector2 currentTranslate_{0, 0};                                                   \
  Vector2 currentScale_{1, 1};                                                       \
  if (cond) {                                                                        \
    FOR_RANGE (int, i, beautifiersCount) {                                           \
      auto& b = beautifiers[i];                                                      \
      currentAlpha_ *= (f32)b.alpha / 65535.0f;                                      \
      currentTranslate_ += b.translate;                                              \
      currentScale_ *= b.scale;                                                      \
    }                                                                                \
  }                                                                                  \
  Beautify currentBeautify_{                                                         \
    .alpha     = (u16)(currentAlpha_ * 65535.0f),                                    \
    .translate = currentTranslate_,                                                  \
    .scale     = currentScale_,                                                      \
  };                                                                                 \
                                                                                     \
  CLAY({.custom{                                                                     \
    .customData = PushClayCustomData({                                               \
      .type      = ClayCustomElementType_BEAUTIFIER_START,                           \
      .alpha     = currentBeautify_.alpha,                                           \
      .translate = currentBeautify_.translate,                                       \
      .scale     = currentBeautify_.scale,                                           \
    })                                                                               \
  }}) {}                                                                             \
  DEFER{CLAY({.custom{                                                               \
    .customData = PushClayCustomData({.type = ClayCustomElementType_BEAUTIFIER_END}) \
  }}){}};

struct ClayImageData {
  int     texId         = {};
  Margins sourceMargins = {0, 0};
  Color   color         = WHITE;
  // f32   scale     = {};
  // ImageFitType fitType   = {};
};

enum ClayCustomElementType : u16 {
  ClayCustomElementType_INVALID,
  ClayCustomElementType_DIALOGUE,
  ClayCustomElementType_BEAUTIFIER_START,
  ClayCustomElementType_BEAUTIFIER_END,
};

struct ClayCustomData {
  ClayCustomElementType type      = {};
  u16                   alpha     = u16_max;
  Vector2               translate = {0, 0};
  Vector2               scale     = {1, 1};
};

// ============================================================ }

#include "bf_constants.cpp"

b2Vec2 ToB2Vec2(Vector2 value) {  ///
  return {value.x, value.y};
}

Vector2 ToVector2(b2Vec2 value) {  ///
  return {value.x, value.y};
}

Vector2 ToVector2(const BFGame::Posf* value) {  ///
  return {value->x(), value->y()};
}

Vector2 ToVector2(const BFGame::Pos* value) {  ///
  return {(f32)value->x(), (f32)value->y()};
}

Vector2Int ToVector2Int(const BFGame::Pos* value) {  ///
  return {value->x(), value->y()};
}

Vector2 Vector2DirectionOrRandom(Vector2 from, Vector2 to) {  ///
  if (from == to)
    return Vector2Rotate({1, 0}, 2 * PI * GRAND.FRand());
  return Vector2Normalize(to - from);
}

Vector2 Vector2DirectionOrZero(Vector2 from, Vector2 to) {  ///
  if (from == to)
    return Vector2Zero();
  return Vector2Normalize(to - from);
}

const char* GetWindowTitle() {  ///
  return "The Game"
#if BF_DEBUG
         " [DEBUG]"
#endif
#if BF_PROFILING && defined(DOCTEST_CONFIG_DISABLE)
         " [PROFILING]"
#endif
#if BF_ENABLE_ASSERTS
         " [ASSERTS]"
#endif
    ;
}

static const char* const g_gameVersion = BF_VERSION
#if BF_DEBUG
  " [DEBUG]"
#endif
#if BF_PROFILING && defined(DOCTEST_CONFIG_DISABLE)
  " [PROFILING]"
#endif
#if BF_ENABLE_ASSERTS
  " [ASSERTS]"
#endif
  ;

struct LogicalFrame {
  i64 _value = i64_max;

  [[nodiscard]] static LogicalFrame Now() {  ///
    LogicalFrame frame{};
    frame.SetNow();
    return frame;
  }

  bool IsSet() const {  ///
    return _value != i64_max;
  }
  void   SetNow();
  lframe Elapsed() const;
  f32    Progress(lframe duration) const;
};

enum ShapeCategory : u32 {
  ShapeCategory_STATIC     = 0x1,
  ShapeCategory_PLAYER     = 0x2,
  ShapeCategory_CREATURE   = 0x4,
  ShapeCategory_PROJECTILE = 0x8,
  // ShapeCategory_ITEM       = 0x10,
};

enum BodyType : u32 {
  BodyType_INVALID,
  BodyType_STATIC,
  BodyType_CREATURE,
  // BodyType_ITEM,
};

enum ShapeUserDataType : u32 {
  ShapeUserDataType_INVALID,
  ShapeUserDataType_STATIC,
  ShapeUserDataType_CREATURE,  // Can use `index` to query `g.level.a.creatures`.
  // ShapeUserDataType_ITEM,      // Can use `index` to query `g.level.a.items`.
};

struct ShapeUserData {
  ShapeUserDataType type   = {};
  int               _index = {};

  int index() {  ///
    if (type == ShapeUserDataType_CREATURE)
      return _index;
    // if (type == ShapeUserDataType_TILE)
    //   return _index;
    INVALID_PATH;
    return 0;
  }

  static ShapeUserData Static() {  ///
    return {.type = ShapeUserDataType_STATIC};
  }

  static ShapeUserData Creature(int index) {  ///
    ASSERT(index >= 0);
    return {
      .type   = ShapeUserDataType_CREATURE,
      ._index = index,
    };
  }

  // static ShapeUserData Item(int index) {  ///
  //   ASSERT(index >= 0);
  //   return {
  //     .type   = ShapeUserDataType_ITEM,
  //     ._index = index,
  //   };
  // }
  //
  // static ShapeUserData Tile(int roomIndex, int index) {  ///
  //   ASSERT(roomIndex >= 0);
  //   ASSERT(roomIndex < LEVEL_ROOMS_TOTAL);
  //
  //   ASSERT(index >= 0);
  //   ASSERT(index < ROOM_TILES_TOTAL);
  //
  //   return {
  //     .type   = ShapeUserDataType_TILE,
  //     ._index = roomIndex * ROOM_TILES_TOTAL + index,
  //   };
  // }

  static ShapeUserData FromPointer(void* ptr) {  ///
    static_assert((sizeof(ShapeUserData) == 4) || (sizeof(ShapeUserData) == 8));

    if (sizeof(void*) == 8) {
      // 64-bit.
      return *(ShapeUserData*)&ptr;
    }
    else {
      // 32-bit. Little endian.
      auto p = (u8*)&ptr;
      u8   type[4]{};
      u8   index[4]{};

      type[0]  = p[0];
      index[0] = p[1];
      index[1] = p[2];
      index[2] = p[3];

      return {
        .type   = *(ShapeUserDataType*)type,
        ._index = *(int*)index,
      };
    }
  }

  void* ToPointer() {  ///
    if (sizeof(void*) == 8) {
      // 64-bit.
      return *(void**)this;
    }
    else {
      // 32-bit. Little endian.
      u8 value[4]{};
      value[0] = ((u8*)&type)[0];
      value[1] = ((u8*)&_index)[0];
      value[2] = ((u8*)&_index)[1];
      value[3] = ((u8*)&_index)[2];

      return *(void**)value;
    }
  }
};

struct Body {
  int      createdId = {};
  b2BodyId id        = {};
};

enum BodyShapeType {
  BodyShapeType_INVALID,
  BodyShapeType_CIRCLE,
  BodyShapeType_RECT,
};

struct BodyShape {
  bool          active = true;
  Body          body   = {};
  BodyShapeType type   = {};
  Color         color  = WHITE;

  union {
    struct {
      f32 radius = {};
    } _circle;

    struct {
      Vector2 size = {};
    } _rect;
  } _u;

  auto& DataCircle() {  ///
    ASSERT(type == BodyShapeType_CIRCLE);
    return _u._circle;
  }

  auto& DataRect() {  ///
    ASSERT(type == BodyShapeType_RECT);
    return _u._rect;
  }
};

struct MakeBodyData {
  BodyType      type     = {};
  bool          isSensor = false;
  f32           density  = 1.0f;
  ShapeUserData userData = {};
  bool          isPlayer = {};
};

struct Weapon {
  WeaponType   type              = {};
  Vector2      offset            = {};
  f32          rotation          = {};
  LogicalFrame startedShootingAt = {};
  LogicalFrame cooldownStartedAt = {};
  // f32 lastShotDirection = {};
};

struct CreatureController {
  Vector2 move = {};
  // Vector2 shoot = {};
  // bool    bomb  = false;
  // LogicalFrame startedMovingAt = {};
};

struct Creature {
  bool               active        = true;
  CreatureType       type          = {};
  f32                health        = {};
  f32                maxHealth     = {};
  Vector2            pos           = {};
  Vector2            dir           = {};
  Body               body          = {};
  CreatureController controller    = {};
  LogicalFrame       lastDamagedAt = {};
  LogicalFrame       diedAt        = {};
};

struct MakeCreatureData {
  CreatureType type = {};
  Vector2      pos  = {};
  MakeBodyData body = {};
};

struct CreatureSpawn {
  CreatureType type      = {};
  Vector2      pos       = {};
  LogicalFrame createdAt = {};
};

struct MakeCreatureSpawnData {
  CreatureType type = {};
  Vector2      pos  = {};
  MakeBodyData body = {};
};

struct Projectile {
  bool           active    = true;
  bool           isPlayer  = {};
  ProjectileType type      = {};
  Vector2        pos       = {};
  Vector2        dir       = {};
  f32            damage    = {};
  LogicalFrame   createdAt = {};
};

struct MakeProjectileData {
  ProjectileType type     = {};
  bool           isPlayer = {};
  Vector2        pos      = {};
  Vector2        dir      = {};
  f32            damage   = {};
};

constexpr int PLAYER_WEAPONS_COUNT = 6;

struct GameData {
  struct Meta {
    i64   frame      = 0;
    Arena trashArena = {};
    Font  uiFont     = {};

    bool godMode = false;

    struct Touch {
      Array<Vector2, 4> pos      = {};  // lstarted, ltarget, rstarted, rtarget.
      Array<Vector2, 2> dir      = {};
      Array<TouchID, 2> touchIDs = {};
      // LogicalFrame      rightLastPressedAt = {};
    } touch;
  } meta;

  struct Level {
    b2WorldId    world        = {};
    LogicalFrame playerDiedAt = {};

    Weapon playerWeapons_[PLAYER_WEAPONS_COUNT] = {};
    VIEW_FROM_ARRAY_DANGER(playerWeapons);

    struct Allocated {
#define VECTORS_TABLE                            \
  X(Creature, creatures)                         \
  X(CreatureSpawn, creatureSpawns)               \
  X(MakeCreatureSpawnData, creatureSpawnsToMake) \
  X(Projectile, projectiles)                     \
  X(BodyShape, bodyShapes)                       \
  X(int, justDamagedCreatures)

#define X(type_, name_) Vector<type_> name_ = {};
      VECTORS_TABLE;
#undef X

      void Reset() {  ///
#define X(type_, name_) (name_).Reset();
        VECTORS_TABLE;
#undef X
      }
#undef VECTORS_TABLE
    } a;
  } level;
} g = {};

#define FRAMES_ELAPSED(value) (g.meta.frame - (value))
#define PROGRESS(elapsed, duration) ((f32)(elapsed) / (f32)(duration))

void LogicalFrame::SetNow() {  ///
  ASSERT_FALSE(IsSet());
  _value = g.meta.frame;
}

lframe LogicalFrame::Elapsed() const {  ///
  ASSERT(IsSet());
  return lframe::MakeUnscaled(FRAMES_ELAPSED(_value));
}

static BF_FORCE_INLINE Clay_Dimensions MeasureText(
  Clay_StringSlice        text,
  Clay_TextElementConfig* config,
  void*                   userData
) noexcept {  ///
  // TODO: fontId, fontSize, letterSpacing
  auto font  = &g.meta.uiFont;
  f32  width = 0;

  IterateOverCodepoints(
    text.chars,
    text.length,
    [&font, &width](u32 codepoint) BF_FORCE_INLINE_LAMBDA {
      if (!codepoint)
        return;

      stbtt_aligned_quad _q{};
      f32                _y{};

      auto glyphIndex
        = ArrayBinaryFind(font->codepoints, font->codepointsCount, (int)codepoint);
      ASSERT(glyphIndex >= 0);

      stbtt_GetPackedQuad(
        font->chars,
        font->atlasTexture.size.x,
        font->atlasTexture.size.y,
        glyphIndex,
        &width,
        &_y,
        &_q,
        1  // 1=opengl & d3d10+,0=d3d9
      );
    }
  );

  return {(f32)width, (f32)font->size};
}

void* PushClayImageData(ClayImageData data) {  ///
  auto result = ALLOCATE_FOR(&g.meta.trashArena, ClayImageData);
  *result     = data;
  return (void*)result;
}

void* PushClayCustomData(ClayCustomData data) {  ///
  auto result = ALLOCATE_FOR(&g.meta.trashArena, ClayCustomData);
  *result     = data;
  return (void*)result;
}

void BF_CLAY_IMAGE(ClayImageData data, auto innerLambda) {  ///
  const auto texture = glib->atlas_textures()->Get(data.texId);
  CLAY({
    .layout{
      .sizing{
        .width  = CLAY_SIZING_FIXED((f32)texture->size_x() * ASSETS_TO_LOGICAL_RATIO),
        .height = CLAY_SIZING_FIXED((f32)texture->size_y() * ASSETS_TO_LOGICAL_RATIO),
      },
      BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
    },
  })
  CLAY({
    .layout{
      .sizing{
        .width  = CLAY_SIZING_FIXED((f32)texture->size_x() * ASSETS_TO_LOGICAL_RATIO),
        .height = CLAY_SIZING_FIXED((f32)texture->size_y() * ASSETS_TO_LOGICAL_RATIO),
      },
    },
    .image{.imageData = PushClayImageData(data)},
  }) {
    innerLambda();
  }
}

void BF_CLAY_IMAGE(ClayImageData data) {  ///
  BF_CLAY_IMAGE(data, [] {});
}

void BF_CLAY_TEXT(Clay_String string) {  ///
  CLAY_TEXT(string, CLAY_TEXT_CONFIG({.textColor{255, 255, 255, 255}}));
}

void BF_CLAY_TEXT(const char* text) {  ///
  auto len           = strlen(text);
  auto allocatedText = ALLOCATE_ARRAY(&g.meta.trashArena, char, len);
  memcpy(allocatedText, text, len + 1);

  Clay_String string{
    .length = (i32)len,
    .chars  = allocatedText,
  };
  BF_CLAY_TEXT(string);
}

void DestroyBody(Body* body) {  ///
  b2DestroyBody(body->id);
  for (auto& shape : g.level.a.bodyShapes) {
    if (shape.body.createdId == body->createdId)
      shape.active = false;
  }
}

void AddBodyShape(BodyShape v) {  ///
  ASSERT(v.active);
  ASSERT(v.type);

  for (auto& shape : g.level.a.bodyShapes) {
    if (!shape.active) {
      shape = v;
      return;
    }
  }

  *g.level.a.bodyShapes.Add() = v;
}

struct MakeBodyResult {  ///
  Body       body     = {};
  b2ShapeDef shapeDef = {};
};

MakeBodyResult MakeBody(Vector2 pos, MakeBodyData data) {  ///
  ASSERT(data.type != BodyType_INVALID);
  ASSERT(data.userData.type != ShapeUserDataType_INVALID);
  if (data.isPlayer)
    ASSERT(data.type == BodyType_CREATURE);

  b2BodyDef bodyDef = b2DefaultBodyDef();
  if (data.type == BodyType_CREATURE)
    bodyDef.type = b2_dynamicBody;
  bodyDef.position      = ToB2Vec2(pos);
  bodyDef.linearDamping = 9;

  b2BodyId body = b2CreateBody(g.level.world, &bodyDef);

  b2ShapeDef shapeDef = b2DefaultShapeDef();
  shapeDef.userData   = data.userData.ToPointer();
  shapeDef.isSensor   = data.isSensor;

  auto& categoryBits = shapeDef.filter.categoryBits;
  auto& maskBits     = shapeDef.filter.maskBits;
  maskBits           = ShapeCategory_STATIC | ShapeCategory_PLAYER;

  switch (data.type) {
  case BodyType_STATIC: {
    categoryBits = ShapeCategory_STATIC;
    maskBits |= ShapeCategory_CREATURE;
  } break;

  case BodyType_CREATURE: {
    categoryBits = ShapeCategory_CREATURE;
    if (data.isPlayer)
      categoryBits |= ShapeCategory_PLAYER;
  } break;

  default:
    INVALID_PATH;
  }

  shapeDef.density = data.density;

  static int     lastCreatedId = 0;
  MakeBodyResult result{
    .body{.createdId = ++lastCreatedId, .id = body},
    .shapeDef = shapeDef,
  };

  return result;
}

void HandleClayErrors(Clay_ErrorData errorData) {  ///
  LOGE("%s", errorData.errorText.chars);

  switch (errorData.errorType) {
  default:
    INVALID_PATH;
  }
}

struct MakeRectBodyData {
  Vector2      pos      = {};
  Vector2      size     = {};
  Vector2      anchor   = Vector2Half();
  MakeBodyData bodyData = {};
};

Body MakeRectBody(MakeRectBodyData data) {  ///
  ASSERT(data.size.x > 0);
  ASSERT(data.size.y > 0);

  auto makeBodyResult
    = MakeBody(data.pos - (data.anchor - Vector2Half()) * data.size, data.bodyData);

  auto box = b2MakeBox(data.size.x / 2, data.size.y / 2);
  b2CreatePolygonShape(makeBodyResult.body.id, &makeBodyResult.shapeDef, &box);

  AddBodyShape({
    .body  = makeBodyResult.body,
    .type  = BodyShapeType_RECT,
    .color = YELLOW,
    ._u{._rect{.size = data.size}},
  });

  return makeBodyResult.body;
}

struct MakeCircleBodyData {
  Vector2      pos           = {};
  f32          radius        = {};
  f32          hurtboxRadius = {};
  MakeBodyData bodyData      = {};
};

Body MakeCircleBody(MakeCircleBodyData data) {  ///
  ASSERT(data.radius > 0);
  if (data.bodyData.type == BodyType_CREATURE)
    ASSERT(data.hurtboxRadius > 0);

  auto makeBodyResult = MakeBody(data.pos, data.bodyData);

  b2Circle circle{.radius = data.radius};
  b2CreateCircleShape(makeBodyResult.body.id, &makeBodyResult.shapeDef, &circle);

  if (data.bodyData.type == BodyType_CREATURE) {
    makeBodyResult.shapeDef.isSensor = true;
    circle.radius                    = data.hurtboxRadius;
    b2CreateCircleShape(makeBodyResult.body.id, &makeBodyResult.shapeDef, &circle);

    AddBodyShape({
      .body  = makeBodyResult.body,
      .type  = BodyShapeType_CIRCLE,
      .color = RED,
      ._u{._circle{.radius = data.hurtboxRadius}},
    });
  }

  AddBodyShape({
    .body  = makeBodyResult.body,
    .type  = BodyShapeType_CIRCLE,
    .color = YELLOW,
    ._u{._circle{.radius = data.radius}},
  });

  return makeBodyResult.body;
}

void MakeCreature(MakeCreatureData data) {  ///
  ASSERT(data.type);
  if (!data.type)
    return;

  int index = -1;
  FOR_RANGE (int, i, g.level.a.creatures.count) {
    if (!g.level.a.creatures[i].active) {
      auto ok = false;

      if ((i == 0) && (g.level.a.creatures.count == 0))
        ok = true;
      if ((i > 0) && (g.level.a.creatures.count > 0))
        ok = true;

      if (ok) {
        index = i;
        break;
      }
    }
  }

  if (index == -1) {
    index = g.level.a.creatures.count;
    g.level.a.creatures.Add();
  }

  auto hurtboxRadius = PLAYER_HURTBOX_RADIUS;
  if (data.type != CreatureType_PLAYER)
    hurtboxRadius = MOB_HURTBOX_RADIUS;

  const auto fb_creature = glib->creatures()->Get(data.type);

  Creature creature{
    .type      = data.type,
    .health    = fb_creature->health(),
    .maxHealth = fb_creature->health(),
    .pos       = data.pos,
    .body      = MakeCircleBody({
           .pos           = data.pos,
           .radius        = CREATURE_COLLIDER_RADIUS,
           .hurtboxRadius = hurtboxRadius,
           .bodyData{
             .type     = BodyType_CREATURE,
             .userData = ShapeUserData::Creature(index),
             .isPlayer = (g.level.a.creatures.count == 1),
      },
    }),
  };

  switch (data.type) {
  case CreatureType_PLAYER:
  case CreatureType_MOB1:
  case CreatureType_MOB2: {
    // Intentionally left blank.
  } break;

  default:
    INVALID_PATH;
  }

  g.level.a.creatures[index] = creature;
}

void MakeCreatureSpawn(MakeCreatureSpawnData data) {  ///
  ASSERT(data.type);
  NOT_IMPLEMENTED;
}

void MakeProjectile(MakeProjectileData data) {  ///
  ASSERT(data.type);

  Projectile projectile{
    .isPlayer = data.isPlayer,
    .type     = data.type,
    .pos      = data.pos,
    .dir      = data.dir,
    .damage   = data.damage,
  };
  projectile.createdAt.SetNow();

  switch (data.type) {
  case ProjectileType_ARROW:
  case ProjectileType_BULLET: {
    // Intentionally left blank.
  } break;

  default:
    INVALID_PATH;
  }

  *g.level.a.projectiles.Add() = projectile;
}

void GamePreInit() {  ///
  ge.meta.logicRand = Random(SDL_GetPerformanceCounter());
}

struct Line {
  Vector2 v1 = {};
  Vector2 v2 = {};
};

struct MakeWallsData {
  const Line* lines     = {};
  View<Body>  outBodies = {};
  int         n         = {};
};

void MakeWalls(MakeWallsData data) {  ///
  ASSERT(data.n > 0);
  if (data.outBodies.count)
    ASSERT(data.n == data.outBodies.count);
  ASSERT(data.lines);

  FOR_RANGE (int, i, data.n) {
    const auto& line = data.lines[i];

    auto v1 = line.v1;
    auto v2 = line.v2;

    if ((v1.x != v2.x) && (v1.y != v2.y))
      INVALID_PATH;
    if (v1 == v2)
      INVALID_PATH;

    if (v1.x > v2.x) {
      auto t = v1;
      v1     = v2;
      v2     = t;
    }
    if (v1.y > v2.y) {
      auto t = v1;
      v1     = v2;
      v2     = t;
    }

    auto body = MakeRectBody({
      .pos    = v1,
      .size   = (v2 - v1) + Vector2One(),
      .anchor = Vector2Zero(),
      .bodyData{
        .type     = BodyType_STATIC,
        .userData = ShapeUserData::Static(),
      },
    });

    if (data.outBodies.count)
      data.outBodies[i] = body;
  }
}

void LevelInit() {
  FOR_RANGE (int, i, PLAYER_WEAPONS_COUNT) {
    g.level.playerWeapons[i].offset = Vector2Rotate(Vector2(1, 0), i * PI / 3.0f);
  }

  // Creating box2d world.
  {  ///
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = b2Vec2{0, 0};
    g.level.world       = b2CreateWorld(&worldDef);
  }

  // Making player.
  MakeCreature({
    .type = CreatureType_PLAYER,
    .pos  = (Vector2)WORLD_SIZE / 2.0f,
    .body{
      .type     = BodyType_CREATURE,
      .userData = ShapeUserData::Creature(0),
      .isPlayer = true,
    },
  });

  FOR_RANGE (int, i, 3) {
    auto& weapon = g.level.playerWeapons[i * 2];
    weapon.type  = WeaponType_GUN;
  }

  // Placing walls.
  {  ///
    Vector2Int p00{-1, -1};
    Vector2Int p11{WORLD_X, WORLD_Y};

    const Line lines[]{
      // Walls around.
      {{p11.x, p00.y}, {p11.x, p11.y}},  // right
      {{p00.x, p11.y}, {p11.x, p11.y}},  // up
      {{p00.x, p00.y}, {p00.x, p11.y}},  // left
      {{p00.x, p00.y}, {p11.x, p00.y}},  // down
    };

    MakeWalls({.lines = lines, .n = ARRAY_COUNT(lines)});
  }
}

void GameInit() {  ///
  SDL_HideCursor();

  g.meta.trashArena = MakeArena(4 * 1024);

  // Initializing Clay.
  {  ///
    auto size = Clay_MinMemorySize();
    Clay_Initialize(
      // TODO: remove malloc!
      Clay_CreateArenaWithCapacityAndMemory(size, malloc(size)),
      Clay_Dimensions{(f32)LOGICAL_RESOLUTION.x, (f32)LOGICAL_RESOLUTION.y},
      Clay_ErrorHandler{HandleClayErrors}
    );
    Clay_SetCullingEnabled(false);
    Clay_SetMeasureTextFunction(MeasureText, 0);
  }

  g.meta.uiFont = LoadFont({
    .filepath        = "resources/correction_brush.ttf",
    .size            = 22,
    .FIXME_sizeScale = 45.0f / 30.0f,
    .codepoints      = g_codepoints,
    .codepointsCount = ARRAY_COUNT(g_codepoints),
    .outlineWidth    = 5,
    .outlineAdvance  = 2,
  });

  LevelInit();
}

#define PLAYER_CREATURE (g.level.a.creatures[0])

Vector2 WorldPosToLogical(Vector2 pos) {  ///
  return pos * ((f32)LOGICAL_RESOLUTION.y / (f32)WORLD_SIZE.y)
         + Vector2(LOGICAL_RESOLUTION.x - LOGICAL_RESOLUTION.y, 0) / 2.0f;
}

f32 WorldSizeToLogical(f32 size) {  ///
  return size * ((f32)LOGICAL_RESOLUTION.y / (f32)WORLD_SIZE.y);
}

Vector2 WorldSizeToLogical(Vector2 size) {  ///
  return size * ((f32)LOGICAL_RESOLUTION.y / (f32)WORLD_SIZE.y);
}

void TryApplyDamage(int creatureIndex, f32 damage, Vector2 direction, f32 impulse) {  ///
  auto& creature = g.level.a.creatures[creatureIndex];

  if (!creatureIndex) {
    if (creature.lastDamagedAt.IsSet()
        && (creature.lastDamagedAt.Elapsed() <= PLAYER_INVINCIBILITY_FRAMES))
      return;
  }

  creature.health -= damage;

  creature.lastDamagedAt = {};
  creature.lastDamagedAt.SetNow();

  b2Body_ApplyLinearImpulseToCenter(
    creature.body.id, ToB2Vec2(direction * impulse), true
  );

  if (!g.level.a.justDamagedCreatures.Contains(creatureIndex))
    *g.level.a.justDamagedCreatures.Add() = creatureIndex;
}

void GameFixedUpdate() {
  // Player actions.
  if (PLAYER_CREATURE.active) {  ///
    // Moving.
    {
      Vector2 move{};

      if (IsKeyDown(SDL_SCANCODE_W))
        move.y += 1;
      if (IsKeyDown(SDL_SCANCODE_A))
        move.x -= 1;
      if (IsKeyDown(SDL_SCANCODE_S))
        move.y -= 1;
      if (IsKeyDown(SDL_SCANCODE_D))
        move.x += 1;

      if (move.x || move.y) {
        move                = Vector2Normalize(move);
        PLAYER_CREATURE.dir = move;
      }

      if (move == Vector2Zero()) {
        if (g.meta.touch.touchIDs[0] != InvalidTouchID)
          move = g.meta.touch.dir[0];
      }

      PLAYER_CREATURE.controller.move = move;
    }
  }

  // Updating AI.
  for (int i = 1; i < g.level.a.creatures.count; i++) {  ///
    auto& creature           = g.level.a.creatures[i];
    creature.controller.move = Vector2DirectionOrZero(creature.pos, PLAYER_CREATURE.pos);
  }

  // Creatures moving.
  {  ///
    const auto fb_creatures = glib->creatures();

    for (auto& creature : g.level.a.creatures) {
      if (!creature.active || creature.diedAt.IsSet())
        continue;

      const auto fb = fb_creatures->Get(creature.type);

      auto speedScale = fb->speed_force();
      if ((creature.type == CreatureType_PLAYER) && g.meta.godMode)
        speedScale *= 1.5f;

      b2Body_ApplyLinearImpulseToCenter(
        creature.body.id,
        ToB2Vec2(creature.controller.move * (FIXED_DT * speedScale)),
        true
      );
    }
  }

  // Updating box2d world.
  b2World_Step(g.level.world, FIXED_DT, 4);

  // Updating body positions.
  for (auto& creature : g.level.a.creatures) {  ///
    if (!creature.active || creature.diedAt.IsSet())
      continue;

    creature.pos = ToVector2(b2Body_GetPosition(creature.body.id));
  }

  // Making pre spawns.
  static int toSpawn = 4;

  if (g.meta.frame % (4 * FIXED_FPS) == 0) {  ///
    const CreatureType types_[]{
      CreatureType_MOB1,
      CreatureType_MOB2,
    };
    VIEW_FROM_ARRAY_DANGER(types);

    Vector2 posToSpawn{};

    FOR_RANGE (int, i, toSpawn) {
      do {
        posToSpawn = {
          0.5f + GRAND.FRand() * (WORLD_X - 1),
          0.5f + GRAND.FRand() * (WORLD_Y - 1),
        };
      } while (Vector2DistanceSqr(PLAYER_CREATURE.pos, posToSpawn)
               < SQR(RADIUS_OF_NOT_SPAWNING_AROUND_PLAYER));

      CreatureSpawn spawn{
        .type = types[GRAND.Rand() % types.count],
        .pos  = posToSpawn,
      };
      spawn.createdAt.SetNow();
      *g.level.a.creatureSpawns.Add() = spawn;
    }

    if (toSpawn < 30)
      toSpawn++;
  }

  // Spawning.
  {  ///
    const int total = g.level.a.creatureSpawns.count;
    int       off   = 0;
    FOR_RANGE (int, i, total) {
      auto& v = g.level.a.creatureSpawns[i - off];
      if (v.createdAt.IsSet() && (v.createdAt.Elapsed() >= SPAWN_FRAMES)) {
        MakeCreature({
          .type = v.type,
          .pos  = v.pos,
          .body{
            .type     = BodyType_CREATURE,
            .userData = ShapeUserData::Creature(g.level.a.creatures.count),
          },
        });
        g.level.a.creatureSpawns.UnstableRemoveAt(i - off);
        off++;
      }
    }
  }

  // Player weapons shooting.
  if (PLAYER_CREATURE.active && !PLAYER_CREATURE.diedAt.IsSet()) {  ///
    for (auto& weapon : g.level.playerWeapons) {
      if (!weapon.type)
        continue;

      const auto fb  = glib->weapons()->Get(weapon.type);
      const auto pos = PLAYER_CREATURE.pos + weapon.offset;

      f32       minDistSqr      = f32_inf;
      Creature* closestCreature = nullptr;

      // Resetting cooldown.
      const auto cooldownDur = lframe::MakeScaled(fb->cooldown_frames());
      if (weapon.cooldownStartedAt.IsSet()
          && (weapon.cooldownStartedAt.Elapsed() >= cooldownDur))
        weapon.cooldownStartedAt = {};

      if (!weapon.cooldownStartedAt.IsSet()) {
        for (int i = 1; i < g.level.a.creatures.count; i++) {
          const auto creature = g.level.a.creatures.base + i;
          if (!creature->active || creature->diedAt.IsSet())
            continue;

          const auto distSqr = Vector2DistanceSqr(pos, creature->pos);

          if (distSqr < minDistSqr) {
            minDistSqr      = distSqr;
            closestCreature = creature;
          }
        }

        if (closestCreature) {
          if (minDistSqr < fb->distance() * fb->distance()) {
            const auto dir  = Vector2DirectionOrRandom(pos, closestCreature->pos);
            weapon.rotation = Vector2Angle(dir);
            if (!weapon.startedShootingAt.IsSet()) {
              weapon.startedShootingAt.SetNow();
            }
          }
        }
      }

      if (weapon.startedShootingAt.IsSet()) {
        const auto e           = weapon.startedShootingAt.Elapsed();
        const auto spawnFrames = fb->projectile_spawn_frames();

        bool spawn = false;
        if (spawnFrames) {
          for (auto value : *spawnFrames) {
            if (value == e.value) {
              spawn = true;
              break;
            }
          }
        }

        if (spawn) {
          MakeProjectile({
            .type     = (ProjectileType)fb->projectile_type(),
            .isPlayer = true,
            .pos      = pos,
            .dir      = Vector2Rotate(Vector2(1, 0), weapon.rotation),
            .damage   = fb->damage(),
          });
        }

        if (e >= lframe::MakeScaled(fb->shooting_duration_frames()))
          weapon.startedShootingAt = {};
      }
    }
  }

  // Mobs contact-damage player.
  if (PLAYER_CREATURE.active && !PLAYER_CREATURE.diedAt.IsSet()) {  ///
    const auto playerPos = PLAYER_CREATURE.pos;
    for (int i = 1; i < g.level.a.creatures.count; i++) {
      const auto& creature = g.level.a.creatures[i];
      if (!creature.active || creature.diedAt.IsSet())
        continue;

      if (Vector2DistanceSqr(playerPos, creature.pos)
          <= SQR(PLAYER_HURTBOX_RADIUS + MOB_HURTBOX_RADIUS))
      {
        TryApplyDamage(
          0,
          glib->creatures()->Get(creature.type)->damage(),
          Vector2DirectionOrRandom(creature.pos, PLAYER_CREATURE.pos),
          0
        );
      }
    }
  }

  // Updating projectiles.
  {  ///
    const auto fb_projectiles = glib->projectiles();

    for (auto& projectile : g.level.a.projectiles) {
      const auto fb = fb_projectiles->Get(projectile.type);
      projectile.pos += projectile.dir * (FIXED_DT * fb->speed());

      int start = 0;
      int end   = g.level.a.creatures.count;

      if (projectile.isPlayer)
        start = 1;
      else
        end = 1;

      for (int i = start; i < end; i++) {
        auto& creature = g.level.a.creatures[i];
        if (!creature.active || creature.diedAt.IsSet())
          continue;

        const auto distSqr = Vector2DistanceSqr(creature.pos, projectile.pos);
        const auto radius  = fb->collider_radius();
        if (distSqr < SQR(radius)) {
          TryApplyDamage(
            i,
            projectile.damage,
            Vector2DirectionOrRandom(projectile.pos, creature.pos),
            fb->impulse()
          );
        }
      }
    }
  }

  // Processing justDamagedCreatures.
  {  ///
    // auto playerHurt = false;
    // auto mobHurt    = false;

    for (const auto index : g.level.a.justDamagedCreatures) {
      auto& creature = g.level.a.creatures[index];
      ASSERT(creature.active);

      // if (index == 0)
      //   playerHurt = true;
      // else
      //   mobHurt = true;

      if (creature.health <= 0) {
        DestroyBody(&creature.body);

        creature.diedAt.SetNow();
        if (!index)
          g.level.playerDiedAt.SetNow();
      }
    }

    // if (playerHurt)
    //   PlaySound(Sound_GAME_PLAYER_HURT);
    // if (mobHurt)
    //   PlaySound(Sound_GAME_HURT);

    g.level.a.justDamagedCreatures.Reset();
  }

  // Removing old died creatures.
  {  ///
    const auto total = g.level.a.creatures.count;
    int        off   = 0;
    FOR_RANGE (int, i, total) {
      const auto& creature = g.level.a.creatures[i - off];
      if (i && creature.diedAt.IsSet() && (creature.diedAt.Elapsed() >= DIE_FRAMES)) {
        g.level.a.creatures.UnstableRemoveAt(i - off);
        off++;
      }
    }
  }

  g.meta.frame++;
}

void ResetLevel() {  ///
  b2DestroyWorld(g.level.world);

  auto a = g.level.a;
  a.Reset();
  g.level = {.a = a};
}

void DoUI() {
  constexpr int MAX_BEAUTIFIERS      = 32;
  const auto    localization         = glib->localizations()->Get(ge.meta.localization);
  const auto    localization_strings = localization->strings();

  TEMP_USAGE(&g.meta.trashArena);

  Clay_BeginLayout();

  auto textures = glib->atlas_textures();

  {
    Array<Beautify, MAX_BEAUTIFIERS> beautifiers{};
    int                              beautifiersCount = 0;

    CLAY({.layout{.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}}}) {
      // Health bar.
      // {  ///
      const auto  texs   = glib->ui_health_texture_ids();
      const auto& player = PLAYER_CREATURE;

      CLAY({
        .layout{.padding{.left = 32, .top = 32}},
        .floating{
          .attachPoints{
            .element = CLAY_ATTACH_POINT_LEFT_TOP,
            .parent  = CLAY_ATTACH_POINT_LEFT_TOP,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        BF_CLAY_IMAGE({.texId = texs->Get(0)}, [&]() BF_FORCE_INLINE_LAMBDA {
          CLAY({
            .floating{
              .attachPoints{
                .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
              },
              .attachTo = CLAY_ATTACH_TO_PARENT,
            },
          }) {
            FLOATING_BEAUTIFY;
            BF_CLAY_IMAGE({
              .texId = texs->Get(1),
              .sourceMargins{
                .right = Clamp01(
                  1.0f - (f32)PLAYER_CREATURE.health / (f32)PLAYER_CREATURE.maxHealth
                )
              },
              .color = RED,
            });
          }
        });
      }
      // }

      // Game's version.
      // {  ///
      CLAY({
        .layout{
          BF_CLAY_PADDING_ALL(4),
          .childGap = 0,
          BF_CLAY_CHILD_ALIGNMENT_LEFT_BOTTOM,
        },
        .floating{
          .attachPoints{
            .element = CLAY_ATTACH_POINT_LEFT_BOTTOM,
            .parent  = CLAY_ATTACH_POINT_LEFT_BOTTOM,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

#if BF_SHOW_VERSION
        BF_CLAY_TEXT(g_gameVersion);
#endif
      }
      // }
    }
  }

  auto renderCommands = Clay_EndLayout();

  // Rendering UI.
  {
    Array<Beautify, MAX_BEAUTIFIERS> beautifiers{};
    int                              beautifiersCount = 0;

    FOR_RANGE (int, mode, 2) {
      // 0 - drawing ui.
      // 1 - drawing gizmos

      RenderGroup_Begin(mode ? RenderZ_GIZMOS : RenderZ_UI);
      RenderGroup_SetSortY(0);

      FOR_RANGE (int, i, renderCommands.length) {
        // Setup.
        // {  ///
        auto& cmd = renderCommands.internalArray[i];

        auto    beautifierAlpha = 1.0f;
        Vector2 beautifierTranslate{0, 0};
        Vector2 beautifierScale{1, 1};
        FOR_RANGE (int, k, beautifiersCount) {
          auto& beautifier = beautifiers[k];
          beautifierAlpha *= (f32)beautifier.alpha / (f32)u16_max;
          beautifierTranslate += beautifier.translate;
          beautifierScale *= beautifier.scale;
        }
        ASSERT(beautifierAlpha <= 1.0f);

        cmd.boundingBox.x += beautifierTranslate.x;
        cmd.boundingBox.y += beautifierTranslate.y;

        auto bb = cmd.boundingBox;
        bb.y    = LOGICAL_RESOLUTION.y - bb.y - bb.height;
        // }

        if (!mode) {
          switch (cmd.commandType) {
          case CLAY_RENDER_COMMAND_TYPE_IMAGE: {  ///
            const auto& d    = cmd.renderData.image;
            const auto& data = *(ClayImageData*)d.imageData;
            RenderGroup_CommandTexture({
              .texId = data.texId,
              .pos{bb.x + bb.width / 2, bb.y + bb.height / 2},
              .sourceMargins = data.sourceMargins,
              .color{
                data.color.r,
                data.color.g,
                data.color.b,
                (u8)((f32)data.color.a * beautifierAlpha)
              },
            });
          } break;

          case CLAY_RENDER_COMMAND_TYPE_TEXT: {  ///
            // TODO: uint16_t text.fontId
            // TODO: uint16_t text.fontSize
            // TODO: letterSpacing
            // TODO: lineHeight
            auto& d = cmd.renderData.text;

            Color c{
              (u8)d.textColor.r,
              (u8)d.textColor.g,
              (u8)d.textColor.b,
              (u8)((f32)d.textColor.a * beautifierAlpha),
            };
            RenderGroup_CommandText({
              .pos{bb.x, bb.y + bb.height},
              .anchor{},
              .font       = &g.meta.uiFont,
              .text       = d.stringContents.chars,
              .bytesCount = d.stringContents.length,
              .color      = c,
            });
          } break;

          case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {  ///
            const auto& d    = cmd.renderData.custom;
            const auto& data = *(ClayCustomData*)d.customData;

            switch (data.type) {
            case ClayCustomElementType_BEAUTIFIER_START: {
              beautifiers[beautifiersCount++] = {
                .alpha     = data.alpha,
                .translate = data.translate,
                .scale     = data.scale,
              };
            } break;

            case ClayCustomElementType_BEAUTIFIER_END: {
              ASSERT(beautifiersCount > 0);
              beautifiersCount--;
            } break;

            default:
              INVALID_PATH;
            }
          } break;
          }
        }
        else {
          // UI elements gizmos.
          if (ge.meta.debugEnabled && bb.width && bb.height) {  ///
            RenderGroup_CommandRectLines({
              .pos = Vector2(bb.x, bb.y) + Vector2(bb.width, bb.height) / 2.0f,
              .size{bb.width, bb.height},
              .color = GREEN,
            });
          }
        }
      }

      RenderGroup_End();
    }
  }
}

void GameDraw() {
  // Drawing floor.
  {  ///
    RenderGroup_OneShotRect(
      {
        .pos   = WorldPosToLogical((Vector2)WORLD_SIZE / 2.0f),
        .size  = WorldSizeToLogical((Vector2)WORLD_SIZE),
        .color = Fade(WHITE, 0.2f),
      },
      RenderZ_FLOOR
    );
  }

  // Drawing creature spawns.
  {  ///
    const auto texId = glib->decal_pre_spawn_texture_id();

    RenderGroup_Begin(RenderZ_FLOOR_DECALS);
    RenderGroup_SetSortY(0);

    for (auto& spawn : g.level.a.creatureSpawns) {
      RenderGroup_CommandTexture({
        .texId = texId,
        .pos   = WorldPosToLogical(spawn.pos),
        .color = RED,
      });
    }

    RenderGroup_End();
  }

  // Drawing creatures.
  {  ///
    const auto fb_creatures = glib->creatures();

    bool isPlayer = true;

    for (const auto& creature : g.level.a.creatures) {
      if (!creature.active)
        continue;

      const auto fb = fb_creatures->Get(creature.type);

      f32 fade = 1;
      if (creature.diedAt.IsSet())
        fade = Clamp01(1 - creature.diedAt.Elapsed().Progress(DIE_FRAMES));

      RenderGroup_OneShotTexture(
        {
          .texId = fb->texture_ids()->Get(0),
          .pos   = WorldPosToLogical(creature.pos),
          .color = Fade(ColorFromRGB(fb->color()), fade),
        },
        RenderZ_DEFAULT
      );

      if (isPlayer) {
        const auto fb_weapons = glib->weapons();

        FOR_RANGE (int, i, PLAYER_WEAPONS_COUNT) {
          const auto& weapon = g.level.playerWeapons[i];
          if (!weapon.type)
            continue;

          const auto fb = fb_weapons->Get(weapon.type);

          RenderGroup_OneShotTexture(
            {
              .texId    = fb->texture_ids()->Get(0),
              .rotation = weapon.rotation,
              .pos      = WorldPosToLogical(creature.pos + weapon.offset),
              .color    = Fade(ColorFromRGB(fb->color()), fade),
            },
            RenderZ_WEAPONS
          );
        }

        isPlayer = false;
      }
    }
  }

  // Drawing projectiles + their gizmos.
  {  ///
    const auto fb_projectiles = glib->projectiles();

    for (const auto& projectile : g.level.a.projectiles) {
      if (!projectile.active)
        continue;

      const auto fb = fb_projectiles->Get(projectile.type);
      RenderGroup_OneShotTexture(
        {
          .texId    = fb->texture_ids()->Get(0),
          .rotation = Vector2Angle(projectile.dir),
          .pos      = WorldPosToLogical(projectile.pos),
          .color    = ColorFromRGB(fb->color()),
        },
        RenderZ_DEFAULT
      );

      // Gizmos.
      if (ge.meta.debugEnabled) {
        RenderGroup_OneShotCircleLines({
          .pos    = WorldPosToLogical(projectile.pos),
          .radius = WorldSizeToLogical(fb->collider_radius()),
          .color  = YELLOW,
        });
      }
    }
  }

  DoUI();

  // Gizmos. Colliders.
  if (ge.meta.debugEnabled) {  ///
    for (auto& shape : g.level.a.bodyShapes) {
      if (!shape.active)
        continue;

      auto pos = ToVector2(b2Body_GetPosition(shape.body.id));

      auto color = shape.color;
      if (!b2Body_IsEnabled(shape.body.id))
        color = GRAY;

      switch (shape.type) {
      case BodyShapeType_CIRCLE: {
        RenderGroup_OneShotCircleLines({
          .pos    = WorldPosToLogical(pos),
          .radius = WorldSizeToLogical(shape.DataCircle().radius),
          .color  = color,
        });
      } break;

      case BodyShapeType_RECT: {
        RenderGroup_OneShotRectLines({
          .pos    = WorldPosToLogical(pos),
          .size   = WorldSizeToLogical(shape.DataRect().size),
          .anchor = Vector2Half(),
          .color  = color,
        });
      } break;

      default:
        INVALID_PATH;
      }
    }
  }

  EngineApplyStrips();
  EngineApplyVignette();
  FlushRenderCommands();

  // Drawing debug text.
  if (ge.meta.debugEnabled) {  ///
    int y = 1;
    LAMBDA (void, DebugText, (const char* pattern, auto&&... args)) {
      bgfx::dbgTextPrintf(1, y++, 0x4f, pattern, args...);
    };

    DebugText("Close debug menu: hold F1 -> press F2");

    LAMBDA (void, debugTextArena, (const char* name, const Arena& arena)) {
      DebugText(
        "%s: %d %d (%.1f%%) (max: %d, %.1f%%)",
        name,
        arena.used,
        arena.size,
        100.0f * (f32)arena.used / (f32)arena.size,
        arena.maxUsed,
        100.0f * (f32)arena.maxUsed / (f32)arena.size
      );
    };

    debugTextArena("ge.meta._arena", ge.meta._arena);
  }
}

///
