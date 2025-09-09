// John Carmack on Inlined Code.
// http://number-none.com/blow/blog/programming/2014/09/26/carmack-on-inlined-code.html
//
// «The real enemy addressed by inlining is unexpected dependency and mutation of state.»
//
// If you are going to make a lot of state changes, having them all happen inline does
// have advantages; you should be made constantly aware of the full horror of what you are
// doing. When it gets to be too much to take, figure out how to factor blocks out into
// pure functions.
//
// To sum up:
//
// * If a function is only called from a single place, consider inlining it.
//
// * If a function is called from multiple places, see if it is possible to arrange
//   for the work to be done in a single place, perhaps with flags, and inline that.
//
// * If there are multiple versions of a function,
//   consider making a single function with more, possibly defaulted, parameters.
//
// * If the work is close to purely functional, with few references to global state,
//   try to make it completely functional.
//
// * Try to use const on both parameters and functions when the function
//   really must be used in multiple places.
//
// * Minimize control flow complexity and "area under ifs", favoring consistent
//   execution paths and times over "optimally" avoiding unnecessary work.

#pragma once

#include "box2d/box2d.h"

// Clay.
// ============================================================ {  ///
#define CLAY_IMPLEMENTATION
#include "clay.h"

Clay_Color ToClayColor(Color color) {
  return {
    .r = (f32)color.r,
    .g = (f32)color.g,
    .b = (f32)color.b,
    .a = (f32)color.a,
  };
}

#define BF_CLAY_SPACER_VERTICAL \
  CLAY({.layout{.sizing{CLAY_SIZING_FIXED(1), CLAY_SIZING_GROW(0)}}}) {}
#define BF_CLAY_SPACER_HORIZONTAL \
  CLAY({.layout{.sizing{CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1)}}}) {}

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

// g_gameVersion { ///
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
// }

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

struct Item {
  ItemType type = {};
};

struct CreatureController {
  Vector2 move = {};
  // Vector2 shoot = {};
  // bool    bomb  = false;
  // LogicalFrame startedMovingAt = {};
};

struct Creature {
  bool               active        = true;
  int                id            = {};
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
  bool                              active             = true;
  ProjectileType                    type               = {};
  int                               ownerCreatureIndex = {};
  Vector2                           pos                = {};
  Vector2                           dir                = {};
  f32                               damage             = {};
  LogicalFrame                      createdAt          = {};
  Array<int, PROJECTILE_MAX_PIERCE> piercedCreatureIds = {};
  int                               piercedCount       = 0;
  f32                               travelledDistance  = 0;
};

struct MakeProjectileData {
  ProjectileType type               = {};
  int            ownerCreatureIndex = {};
  Vector2        pos                = {};
  Vector2        dir                = {};
  f32            damage             = {};
};

struct Number {
  NumberType   type      = {};
  int          value     = {};
  Vector2      pos       = {};
  LogicalFrame createdAt = {};
};

struct Pickupable {
  PickupableType type       = {};
  Vector2        pos        = {};
  LogicalFrame   createdAt  = {};
  LogicalFrame   pickedUpAt = {};
};

constexpr int PLAYER_WEAPONS_COUNT = 6;

struct MakeNumberData {
  NumberType type  = {};
  int        value = {};
  Vector2    pos   = {};
};

lframe GetWaveDuration(int wave) {  ///
#if BF_RELEASE
  constexpr int durations_[]{20, 25, 30, 35, 40, 45, 50, 55, 60, 60,
                             60, 60, 60, 60, 60, 60, 60, 60, 60, 90};
  VIEW_FROM_ARRAY_DANGER(durations);
  const int seconds = durations[MIN(durations.count - 1, wave)];
#else
  const int seconds = 3 + wave * 5;
#endif
  return lframe::MakeUnscaled(seconds * FIXED_FPS);
}

enum StateType {
  StateType_GAMEPLAY,
  StateType_UPGRADES,
  StateType_SHOP,
};

struct ShopItem {
  WeaponType weapon = {};
  ItemType   item   = {};
  int        price  = {};
};

struct GameData {
  struct Meta {
    i64   frame      = 0;
    Arena arena      = {};
    Arena trashArena = {};
    Font  uiFont     = {};

    bool godMode = false;

    struct Touch {
      Array<Vector2, 4> pos      = {};  // lstarted, ltarget, rstarted, rtarget.
      Array<Vector2, 2> dir      = {};
      Array<TouchID, 2> touchIDs = {};
      // LogicalFrame      rightLastPressedAt = {};
    } touch;

    bool      reload            = false;
    bool      shopScheduled     = false;
    bool      nextWaveScheduled = false;
    StateType state             = StateType_GAMEPLAY;
  } meta;

  struct Level {
    Camera camera{
      .zoom          = METER_LOGICAL_SIZE,
      .texturesScale = 1.0f / METER_LOGICAL_SIZE,
    };

    b2WorldId    world           = {};
    int          nextCreatureId  = 0;
    LogicalFrame playerDiedAt    = {};
    int          toSpawn         = 3;
    LogicalFrame lastLifestealAt = {};

    f32 xp          = 0;
    f32 nextLevelXp = 10;
    int coins       = 0;
    int xpLevel     = 1;
    int pierceCount = 0;

    int          wave          = 0;
    LogicalFrame waveStartedAt = {};

    Array<Weapon, PLAYER_WEAPONS_COUNT> playerWeapons = {};
    Array<int, StatType_COUNT>          playerStats   = {};

    struct {
      Array<StatType, 4> toPick = {};
    } upgrades;

    struct {
      Array<ShopItem, 4> toPick = {};
    } shop;

    struct Allocated {
      // Using "X-macros". ref: https://www.geeksforgeeks.org/c/x-macros-in-c/
#define VECTORS_TABLE                            \
  X(Creature, creatures)                         \
  X(CreatureSpawn, creatureSpawns)               \
  X(MakeCreatureSpawnData, creatureSpawnsToMake) \
  X(Projectile, projectiles)                     \
  X(int, projectilesToRemove)                    \
  X(BodyShape, bodyShapes)                       \
  X(int, justDamagedCreatures)                   \
  X(Number, numbers)                             \
  X(Pickupable, pickupables)                     \
  X(Item, playerItems)

#define X(type_, name_) Vector<type_> name_ = {};
      VECTORS_TABLE;
#undef X

      void Reset() {  ///
#define X(type_, name_) (name_).Reset();
        VECTORS_TABLE;
#undef X
      }
    } a;
  } level;
} g = {};

#define FRAMES_ELAPSED(value) (g.meta.frame - (value))
#define PROGRESS(elapsed, duration) ((f32)(elapsed) / (f32)(duration))

void MakeNumber(MakeNumberData data) {  ///
  ASSERT(data.type);
  Number number{
    .type  = data.type,
    .value = data.value,
    .pos   = data.pos,
  };
  number.createdAt.SetNow();
  *g.level.a.numbers.Add() = number;
}

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
    .id        = g.level.nextCreatureId++,
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
             .isPlayer = (data.type == CreatureType_PLAYER),
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
    .type               = data.type,
    .ownerCreatureIndex = data.ownerCreatureIndex,
    .pos                = data.pos,
    .dir                = data.dir,
    .damage             = data.damage,
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
  ge.meta.logicRand           = Random(SDL_GetPerformanceCounter());
  ge.settings.backgroundColor = {0x1d, 0x1b, 0x24, 0xff};
}

struct Line {
  Vector2 v1 = {};
  Vector2 v2 = {};
};

struct MakeWallsData {
  const View<Line> lines     = {};
  View<Body>       outBodies = {};
};

void MakeWalls(MakeWallsData data) {  ///
  ASSERT(data.lines.count > 0);
  if (data.outBodies.count)
    ASSERT(data.lines.count == data.outBodies.count);
  ASSERT(data.lines.base);

  FOR_RANGE (int, i, data.lines.count) {
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

#define PLAYER_CREATURE (g.level.a.creatures[0])

Vector2 GetCameraTargetPos() {  ///
  auto tpos = PLAYER_CREATURE.pos;

  tpos.x += Lerp(
    WORLD_CORNER_MARGIN_X, -WORLD_CORNER_MARGIN_X, PLAYER_CREATURE.pos.x / (f32)WORLD_X
  );
  tpos.y += Lerp(
    WORLD_CORNER_MARGIN_Y, -WORLD_CORNER_MARGIN_Y, PLAYER_CREATURE.pos.y / (f32)WORLD_Y
  );

  tpos.x = Clamp(tpos.x, CAMERA_MIN_X, CAMERA_MAX_X);
  tpos.y = Clamp(tpos.y, CAMERA_MIN_Y, CAMERA_MAX_Y);

  return tpos;
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
  });

  g.level.camera.pos = GetCameraTargetPos();

  struct {
    WeaponType type = {};
  } weapons_[]{
    {.type = WeaponType_GUN},
    {.type = WeaponType_BOW},
    {.type = WeaponType_GUN},
  };
  VIEW_FROM_ARRAY_DANGER(weapons);

  FOR_RANGE (int, i, 3) {
    auto& weapon = g.level.playerWeapons[i * 2];
    weapon.type  = weapons[i].type;
  }

  // Placing walls.
  {  ///
    Vector2Int p00{-1, -1};
    Vector2Int p11{WORLD_X, WORLD_Y};

    Line lines_[]{
      // Walls around.
      {{p11.x, p00.y}, {p11.x, p11.y}},  // right
      {{p00.x, p11.y}, {p11.x, p11.y}},  // up
      {{p00.x, p00.y}, {p00.x, p11.y}},  // left
      {{p00.x, p00.y}, {p11.x, p00.y}},  // down
    };
    VIEW_FROM_ARRAY_DANGER(lines);

    MakeWalls({.lines = lines});
  }

  g.level.waveStartedAt = {};
  g.level.waveStartedAt.SetNow();
}

void GameInit() {  ///
  SDL_HideCursor();

  g.meta.trashArena = MakeArena(4 * 1024);
  g.meta.arena      = MakeArena(4 * 1024);

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

constexpr f32 GetStatRegenPerSecond(int level) {  ///
  return (f32)level / 11.25f + 1.0f / 9.0f;
}

constexpr f32 GetStatRegenPerFrame(int level) {  ///
  const auto perSecond = GetStatRegenPerSecond(level);
  return perSecond / (f32)FIXED_FPS;
}

constexpr f32 GetLifestealChance(int level) {  ///
  return (f32)level / 100.0f;
}

bool TryApplyDamage(
  int     creatureIndex,
  f32     damage,
  Vector2 direction,
  f32     impulse,
  int     damageApplicatorCreatureIndex
) {  ///
  auto& creature = g.level.a.creatures[creatureIndex];

  if (creatureIndex) {
    MakeNumber({
      .type  = NumberType_DAMAGE,
      .value = Round(damage),
      .pos   = creature.pos,
    });
  }
  else {
    if (creature.lastDamagedAt.IsSet()
        && (creature.lastDamagedAt.Elapsed() <= PLAYER_INVINCIBILITY_FRAMES))
      return false;
  }

  if (creature.health > 0) {
    if (!damageApplicatorCreatureIndex) {
      if (GRAND.FRand() < GetLifestealChance(g.level.playerStats[StatType_LIFE_STEAL])) {
        bool canLifesteal = false;
        if (!g.level.lastLifestealAt.IsSet())
          canLifesteal = true;
        else if (g.level.lastLifestealAt.Elapsed() >= LIFESTEAL_COOLDOWN_FRAMES)
          canLifesteal = true;

        if (canLifesteal && (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth)) {
          PLAYER_CREATURE.health
            = MoveTowards(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth, 1);
          g.level.lastLifestealAt = {};
          g.level.lastLifestealAt.SetNow();
        }
      }
    }
    creature.health -= damage;

    creature.lastDamagedAt = {};
    creature.lastDamagedAt.SetNow();

    b2Body_ApplyLinearImpulseToCenter(
      creature.body.id, ToB2Vec2(direction * impulse), true
    );

    if (!g.level.a.justDamagedCreatures.Contains(creatureIndex))
      *g.level.a.justDamagedCreatures.Add() = creatureIndex;

    return true;
  }

  return false;
}

void ResetLevel() {  ///
  b2DestroyWorld(g.level.world);

  auto a = g.level.a;
  a.Reset();
  g.level = {.a = a};

  g.meta.arena.used = 0;
}

void DoUI(bool draw) {
  // NOTE: Logic must be executed only when not drawing!
  // e.g. updating mouse position, processing `clicked()`,
  // logically reacting to `Clay_Hovered()`, setting game state, etc.

  if (!draw) {
    // Updating clay mouse pos.
    // TODO: TOUCH!
    const auto pos = ScreenPosToLogical(GetMouseScreenPos());
    Clay_SetPointerState({pos.x, LOGICAL_RESOLUTION.y - pos.y}, false);
  }

  LAMBDA (bool, clicked, ()) {  ///
    return !draw && Clay_Hovered() && IsMouseReleased(L);
  };

  constexpr int MAX_BEAUTIFIERS      = 32;
  const auto    localization         = glib->localizations()->Get(ge.meta.localization);
  const auto    localization_strings = localization->strings();

  TEMP_USAGE(&g.meta.trashArena);

  Clay_BeginLayout();

  auto textures = glib->atlas_textures();

  Array<Beautify, MAX_BEAUTIFIERS> beautifiers{};
  int                              beautifiersCount = 0;

  // Gameplay.
  if (g.meta.state == StateType_GAMEPLAY) {
    CLAY({.layout{.sizing{CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}}}) {
      // Health bar + coins.
      // {  ///
      const auto  texs   = glib->ui_health_texture_ids();
      const auto& player = PLAYER_CREATURE;

      CLAY({
        .layout{
          .padding{.left = 32, .top = 32},
          .childGap        = 8,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
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
              BF_CLAY_TEXT(TextFormat(
                "%d / %d", Round(PLAYER_CREATURE.health), Round(PLAYER_CREATURE.maxHealth)
              ));
            }
          }
        });

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
              .sourceMargins{.right = 1 - (f32)g.level.xp / (f32)g.level.nextLevelXp},
              .color = GREEN,
            });

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
              BF_CLAY_TEXT(TextFormat("%d", g.level.xpLevel));
            }
          }
        });

        CLAY({.layout{
          .childGap = 8,
          BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        }}) {
          BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
          BF_CLAY_TEXT(TextFormat("%d", g.level.coins));
        }
      }
      // }

      // Wave.
      // { ///
      CLAY({
        .layout{.padding{.top = 32}},
        .floating{
          .attachPoints{
            .element = CLAY_ATTACH_POINT_CENTER_TOP,
            .parent  = CLAY_ATTACH_POINT_CENTER_TOP,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;
        const auto remainingFrames
          = GetWaveDuration(g.level.wave) - g.level.waveStartedAt.Elapsed();
        const int remainingSeconds = remainingFrames.value / FIXED_FPS + 1;
        BF_CLAY_TEXT(TextFormat("Wave %d. %d...", g.level.wave + 1, remainingSeconds));
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
  // Upgrades.
  else if (g.meta.state == StateType_UPGRADES) {  ///
    CLAY({.layout{.sizing{CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}}}) {
      CLAY({
        .layout{.childGap = 8},
        .floating{
          .attachPoints{
            .element = CLAY_ATTACH_POINT_CENTER_CENTER,
            .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        const auto fb_stats = glib->stats();

        FOR_RANGE (int, i, g.level.upgrades.toPick.count) {
          const auto stat = g.level.upgrades.toPick[i];
          const auto fb   = fb_stats->Get(stat);
          CLAY({}) {
            // Scheduling close of upgrade UI + applying selected stat upgrade.
            if (clicked()) {
              g.meta.shopScheduled = true;

              const int amount = 1;
              g.level.playerStats[stat] += amount;

              switch (stat) {
              case StatType_HP: {
                PLAYER_CREATURE.health += amount;
                PLAYER_CREATURE.maxHealth += amount;
              } break;

              default:
                break;
              }
            }

            const int slotTexId
              = (Clay_Hovered() ? glib->ui_item_slot_hovered_texture_id() : glib->ui_item_slot_default_texture_id());
            const auto slotColor = ColorFromRGB(
              Clay_Hovered() ? glib->ui_item_slot_hovered_color()
                             : glib->ui_item_slot_default_color()
            );

            BF_CLAY_IMAGE(
              {.texId = slotTexId, .color = slotColor},
              [&]() BF_FORCE_INLINE_LAMBDA {
                CLAY({
                  .layout{
                    .sizing{CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                    BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                  },
                }) {
                  BF_CLAY_IMAGE({.texId = fb->upgrade_texture_id()});
                }
              }
            );
          }
        }
      }
    }
  }
  // Shop.
  else if (g.meta.state == StateType_SHOP) {  ///
    // Columns.
    CLAY({.layout{
      .sizing{CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
      BF_CLAY_PADDING_ALL(8),
    }}) {
      // Left column that contains:
      // 1. wave, coins, reroll.
      // 2. items to buy.
      // 3. player's items and weapons.
      CLAY({.layout{
        .sizing{CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
        .childGap        = 8,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        // 1. Wave, coins, reroll.
        CLAY({.layout{.sizing{.width = CLAY_SIZING_GROW(0)}}}) {
          // CLAY({}) {
          // BF_CLAY_TEXT_LOCALIZED_DANGER(glib->shop_button_reroll_locale());
          // }
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->shop_label_shop_locale());
          BF_CLAY_TEXT(" (");
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->wave_locale());
          BF_CLAY_TEXT(TextFormat(" %d)", g.level.wave));

          BF_CLAY_SPACER_HORIZONTAL;

          // Coins.
          CLAY({
            .layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER},
            .floating{
              .attachPoints{
                .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
              },
              .attachTo = CLAY_ATTACH_TO_PARENT,
            },
          }) {
            FLOATING_BEAUTIFY;
            BF_CLAY_TEXT(TextFormat("%d ", g.level.coins));
            BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
          }

          BF_CLAY_SPACER_HORIZONTAL;

          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->shop_button_reroll_locale());
        }

        BF_CLAY_SPACER_VERTICAL;

        // 2. Items to buy.
        CLAY({.layout{
          .sizing{.width = CLAY_SIZING_GROW(0)},
          .childGap = 8,
        }}) {
          FLOATING_BEAUTIFY;

          BF_CLAY_SPACER_HORIZONTAL;

          for (auto& v : g.level.shop.toPick) {
            bool canBuy = ((v.weapon || v.item) && (v.price <= g.level.coins));
            int  emptyWeaponSlotIndex = -1;
            if (canBuy && v.weapon) {
              bool emptySlotExists = false;
              FOR_RANGE (int, i, g.level.playerWeapons.count) {
                const auto& weapon = g.level.playerWeapons[i];
                if (!weapon.type) {
                  emptyWeaponSlotIndex = i;
                  break;
                }
              }
              if (emptyWeaponSlotIndex == -1)
                canBuy = false;
            }

            CLAY({
              .layout{
                .sizing{
                  .width  = CLAY_SIZING_FIXED(235),
                  .height = CLAY_SIZING_FIXED(320),
                },
                BF_CLAY_PADDING_ALL(8),
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              },
              .backgroundColor
              = ToClayColor(ColorFromRGB(glib->ui_item_big_frame_color())),
            }) {
              const auto fb_item = (v.item ? glib->items()->Get(v.item) : nullptr);
              const auto fb_weapon
                = (v.weapon ? glib->weapons()->Get(v.weapon) : nullptr);

              const int texs_[]{
                glib->ui_item_slot_hovered_texture_id(),
                glib->ui_item_slot_default_texture_id(),
                glib->ui_item_slot_disabled_texture_id(),
              };
              VIEW_FROM_ARRAY_DANGER(texs);

              const u32 colors_[]{
                glib->ui_item_slot_hovered_color(),
                glib->ui_item_slot_default_color(),
                glib->ui_item_slot_disabled_color(),
              };
              VIEW_FROM_ARRAY_DANGER(colors);

              // TODO: Highlight price, not item's frame.
              const int type = (canBuy ? (Clay_Hovered() ? 0 : 1) : 2);

              // Item's image + name.
              CLAY({.layout{.childGap = 8}}) {
                // Item's image.
                BF_CLAY_IMAGE(
                  {.texId = texs[type], .color = ColorFromRGB(colors[type])},
                  [&]() BF_FORCE_INLINE_LAMBDA {
                    if (v.item || v.weapon) {
                      CLAY({
                        .layout{
                          .sizing{CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                        },
                      }) {
                        int texId = 0;
                        if (v.item)
                          texId = fb_item->texture_id();
                        if (v.weapon)
                          texId = fb_weapon->texture_ids()->Get(0);
                        BF_CLAY_IMAGE({.texId = texId});
                      }
                    }
                  }
                );

                int locale = 0;
                if (v.item)
                  locale = fb_item->name_locale();
                else if (v.weapon)
                  locale = fb_weapon->name_locale();
                if (locale)
                  BF_CLAY_TEXT_LOCALIZED_DANGER(locale);
              }

              BF_CLAY_SPACER_VERTICAL;

              // Item's price.
              CLAY({.layout{
                .sizing{.width = CLAY_SIZING_GROW(0)},
                BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
              }}) {
                BF_CLAY_TEXT(TextFormat("%d ", v.price));
                BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});

                // Buying item / weapon.
                if (canBuy && clicked()) {
                  g.level.coins -= v.price;
                  if (v.weapon)
                    g.level.playerWeapons[emptyWeaponSlotIndex].type = v.weapon;
                  else if (v.item)
                    *g.level.a.playerItems.Add() = {.type = v.item};
                  else
                    INVALID_PATH;
                  v = {};
                }
              }
            }
          }

          BF_CLAY_SPACER_HORIZONTAL;
        }

        BF_CLAY_SPACER_VERTICAL;

        // 3. Player's items and weapons.
        CLAY({.layout{.sizing{.width = CLAY_SIZING_GROW(0)}}}) {
          CLAY({.layout{.layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            // Items label.
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->shop_label_items_locale());

            // TODO: Items.
            CLAY({}) {}
          }

          BF_CLAY_SPACER_HORIZONTAL;

          CLAY({.layout{.layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            // Weapons label.
            CLAY({}) {
              BF_CLAY_TEXT_LOCALIZED_DANGER(glib->shop_label_weapons_locale());

              int weaponsCount = 0;
              for (const auto& weapon : g.level.playerWeapons) {
                if (weapon.type)
                  weaponsCount++;
              }

              BF_CLAY_TEXT(
                TextFormat(" (%d/%d)", weaponsCount, g.level.playerWeapons.count)
              );
            }

            // TODO: Weapons.
            CLAY({}) {}
          }
        }
      }

      // Right column that contains stats and next wave button.
      CLAY({.layout{.sizing{.height = CLAY_SIZING_GROW(0)}}}) {
        // Advance to the next wave button.
        CLAY({
          .layout{
            .sizing{.height = CLAY_SIZING_GROW(0)},
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
        }) {
          // TODO: stats

          BF_CLAY_SPACER_VERTICAL;

          CLAY({
            .layout{BF_CLAY_PADDING_ALL(8)},
            .backgroundColor = ToClayColor(Clay_Hovered() ? WHITE : GRAY),
          }) {
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->shop_button_next_wave_locale());
            BF_CLAY_TEXT(" (");
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->wave_locale());
            BF_CLAY_TEXT(TextFormat(" %d)", g.level.wave + 1));

            if (clicked())
              g.meta.nextWaveScheduled = true;
          }
        }
      }
    }
  }
  else
    INVALID_PATH;

  auto renderCommands = Clay_EndLayout();

  // Drawing UI.
  if (draw) {
    Array<Beautify, MAX_BEAUTIFIERS> beautifiers{};
    int                              beautifiersCount = 0;

    FOR_RANGE (int, mode, 2) {
      // 0 - drawing ui.
      // 1 - drawing gizmos.

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
          case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {  ///
            const auto& d = cmd.renderData.rectangle;
            RenderGroup_CommandRect({
              .pos{bb.x, bb.y},
              .size{bb.width, bb.height},
              .anchor{},
              .color{
                (u8)d.backgroundColor.r,
                (u8)d.backgroundColor.g,
                (u8)d.backgroundColor.b,
                (u8)((f32)d.backgroundColor.a * beautifierAlpha)
              },
            });
          } break;

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

void GameFixedUpdate() {
  // Reloading game.
  if (g.meta.reload) {  ///
    g.meta.reload = false;
    ResetLevel();
    LevelInit();
  }

  // Cheats.
  if (ge.meta.debugEnabled) {
    if (IsKeyPressed(SDL_SCANCODE_F5))
      g.level.coins += 10;
  }

  // Advancing to shop.
  if (g.meta.shopScheduled) {  ///
    g.meta.shopScheduled = false;
    g.meta.state         = StateType_SHOP;

    auto& toPick = g.level.shop.toPick;

    for (auto& v : toPick) {
      v = {.price = 15 + (int)(GRAND.Rand() % 20)};

      const bool setToItem = (GRAND.FRand() <= SHOP_ITEM_RATIO);
      if (setToItem) {
        while (!v.item)
          v.item = (ItemType)(GRAND.Rand() % glib->items()->size());
      }
      else {
        while (!v.weapon)
          v.weapon = (WeaponType)(GRAND.Rand() % glib->weapons()->size());
      }
    }
  }

  // Advancing to the next wave.
  if (g.meta.nextWaveScheduled) {  ///
    g.meta.nextWaveScheduled = false;

    g.meta.state           = StateType_GAMEPLAY;
    ge.settings.screenFade = 0;
    SDL_HideCursor();

    IncrementSetZeroOn(&g.level.wave, (int)glib->waves()->size());
    g.level.waveStartedAt = {};
    g.level.waveStartedAt.SetNow();

    for (int i = 1; i < g.level.a.creatures.count; i++) {
      auto& creature = g.level.a.creatures[i];
      if (creature.active && !creature.diedAt.IsSet())
        DestroyBody(&creature.body);
    }
    g.level.a.creatures.count = 1;
    g.level.a.creatureSpawns.Reset();
    g.level.a.creatureSpawnsToMake.Reset();
    g.level.a.projectiles.Reset();
    g.level.a.projectilesToRemove.Reset();
    g.level.a.bodyShapes.Reset();
    g.level.a.justDamagedCreatures.Reset();
  }

  // Updating gameplay.
  if (g.meta.state == StateType_GAMEPLAY) {
    // Finishing wave opens upgrades screen.
    if (g.level.waveStartedAt.Elapsed() >= GetWaveDuration(g.level.wave)) {  ///
      g.meta.state           = StateType_UPGRADES;
      ge.settings.screenFade = glib->ui_modal_fade();
      SDL_ShowCursor();

      const auto fb_stats = glib->stats();

      // Refilling `toPick`.
      FOR_RANGE (int, i, g.level.upgrades.toPick.count) {
        while (1) {
          const auto newStat = (StatType)(GRAND.Rand() % fb_stats->size());
          if (!newStat)
            continue;
          const auto fb = fb_stats->Get(newStat);
          if (!fb->upgrade_texture_id())
            continue;
          if (ArrayContains(g.level.upgrades.toPick.base, i, newStat))
            continue;
          g.level.upgrades.toPick[i] = newStat;
          break;
        }
      }
    }

    if (PLAYER_CREATURE.active && !PLAYER_CREATURE.diedAt.IsSet()) {
      // Player actions. Moving.
      {  ///
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

      // Player HP regen.
      if (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth) {  ///
        PLAYER_CREATURE.health = MoveTowards(
          PLAYER_CREATURE.health,
          PLAYER_CREATURE.maxHealth,
          GetStatRegenPerFrame(g.level.playerStats[StatType_REGEN])
        );
      }
    }

    // Updating AI.
    for (int i = 1; i < g.level.a.creatures.count; i++) {  ///
      auto& creature = g.level.a.creatures[i];
      creature.controller.move
        = Vector2DirectionOrZero(creature.pos, PLAYER_CREATURE.pos);
    }

    // Creatures moving.
    {  ///
      const auto fb_creatures = glib->creatures();

      for (auto& creature : g.level.a.creatures) {
        if (!creature.active || creature.diedAt.IsSet())
          continue;

        const auto fb = fb_creatures->Get(creature.type);

        auto speedScale = fb->speed_force() * SPEED_MULTIPLIER;
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
    if (g.level.waveStartedAt.Elapsed().value % (4 * FIXED_FPS) == 0) {  ///
      const auto wave = glib->waves()->Get(g.level.wave);

      Vector2 posToSpawn{};

      FOR_RANGE (int, i, g.level.toSpawn) {
        do {
          posToSpawn = {
            0.5f + GRAND.FRand() * (WORLD_X - 1),
            0.5f + GRAND.FRand() * (WORLD_Y - 1),
          };
        } while (Vector2DistanceSqr(PLAYER_CREATURE.pos, posToSpawn)
                 < SQR(RADIUS_OF_NOT_SPAWNING_AROUND_PLAYER));

        const auto   factor = GRAND.FRand();
        CreatureType spawnType{};
        for (const auto creature : *wave->creatures_to_spawn()) {
          if (factor <= creature->spawn_factor()) {
            spawnType = (CreatureType)creature->creature_type();
            break;
          }
        }

        if (spawnType) {
          CreatureSpawn spawn{
            .type = spawnType,
            .pos  = posToSpawn,
          };
          spawn.createdAt.SetNow();
          *g.level.a.creatureSpawns.Add() = spawn;
        }
        else
          INVALID_PATH;
      }

      if (g.level.toSpawn < 5)
        g.level.toSpawn++;
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
              .type               = (ProjectileType)fb->projectile_type(),
              .ownerCreatureIndex = 0,
              .pos                = pos,
              .dir                = Vector2Rotate(Vector2(1, 0), weapon.rotation),
              .damage             = fb->damage(),
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
            0,
            i
          );
        }
      }
    }

    // Updating projectiles:
    // - Movement
    // - Marking to remove because of travel distance
    // - Mob collisions
    // - Marking to remove because of pierce count
    {  ///
      const auto fb_projectiles = glib->projectiles();

      int projectileIndex = -1;
      for (auto& projectile : g.level.a.projectiles) {
        projectileIndex++;

        const auto fb       = fb_projectiles->Get(projectile.type);
        const auto distance = FIXED_DT * fb->speed();
        projectile.travelledDistance += distance;
        projectile.pos += projectile.dir * distance;

        if (projectile.travelledDistance >= fb->distance()) {
          if (!g.level.a.projectilesToRemove.Contains(projectileIndex))
            *g.level.a.projectilesToRemove.Add() = projectileIndex;
        }

        int start = 0;
        int end   = g.level.a.creatures.count;

        if (g.level.a.creatures[projectile.ownerCreatureIndex].type
            == CreatureType_PLAYER)
          start = 1;
        else
          end = 1;

        for (int i = start; i < end; i++) {
          auto& creature = g.level.a.creatures[i];
          if (!creature.active || creature.diedAt.IsSet())
            continue;

          // Not damaging already damaged creatures.
          if (ArrayContains(
                projectile.piercedCreatureIds.base, projectile.piercedCount, creature.id
              ))
            continue;

          const auto distSqr = Vector2DistanceSqr(creature.pos, projectile.pos);
          const auto radius  = fb->collider_radius();
          if (distSqr < SQR(radius)) {
            if (TryApplyDamage(
                  i,
                  projectile.damage,
                  Vector2DirectionOrRandom(projectile.pos, creature.pos),
                  fb->impulse(),
                  projectile.ownerCreatureIndex
                ))
            {
              auto maxPierce = fb->pierce();
              if (g.level.a.creatures[projectile.ownerCreatureIndex].type
                  == CreatureType_PLAYER)
                maxPierce += g.level.pierceCount;

              if (projectile.piercedCount < maxPierce)
                projectile.piercedCreatureIds[projectile.piercedCount++] = creature.id;
              else if (!g.level.a.projectilesToRemove.Contains(projectileIndex))
                *g.level.a.projectilesToRemove.Add() = projectileIndex;
            }
          }
        }
      }
    }

    // Processing `projectilesToRemove`.
    if (g.level.a.projectilesToRemove.count) {  ///
      qsort(
        (void*)g.level.a.projectilesToRemove.base,
        g.level.a.projectilesToRemove.count,
        sizeof(*g.level.a.projectilesToRemove.base),
        (int (*)(const void*, const void*))IntCmp
      );
      FOR_RANGE (int, i, g.level.a.projectilesToRemove.count) {
        const auto projectileIndex
          = g.level.a.projectilesToRemove[g.level.a.projectilesToRemove.count - i - 1];
        g.level.a.projectiles.UnstableRemoveAt(projectileIndex);
      }
      g.level.a.projectilesToRemove.Reset();
    }

    // Processing `justDamagedCreatures`.
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
          if (index) {
            g.level.xp++;
            if (g.level.xp >= g.level.nextLevelXp) {
              g.level.nextLevelXp *= 2;
              g.level.xp = 0;
              g.level.xpLevel++;
            }
          }
          else
            g.level.playerDiedAt.SetNow();

          Pickupable pickupable{
            .type = PickupableType_COIN,
            .pos  = creature.pos,
          };
          pickupable.createdAt.SetNow();
          *g.level.a.pickupables.Add() = pickupable;
        }
      }

      // if (playerHurt)
      //   PlaySound(Sound_GAME_PLAYER_HURT);
      // if (mobHurt)
      //   PlaySound(Sound_GAME_HURT);

      g.level.a.justDamagedCreatures.Reset();
    }

    // Picking up pickupables.
    if (PLAYER_CREATURE.active && !PLAYER_CREATURE.diedAt.IsSet()) {  ///
      for (auto& pickupable : g.level.a.pickupables) {
        if (pickupable.pickedUpAt.IsSet()) {
          pickupable.pos
            = Vector2ExponentialDecay(pickupable.pos, PLAYER_CREATURE.pos, 3, FIXED_DT);
        }
        else {
          if (Vector2DistanceSqr(pickupable.pos, PLAYER_CREATURE.pos)
              <= SQR(PICKUPABLE_HURTBOX_RADIUS))
          {
            pickupable.pickedUpAt.SetNow();
            MakeNumber({
              .type  = NumberType_PICKUPABLE,
              .value = 1,
              .pos   = PLAYER_CREATURE.pos + Vector2(0, PLAYER_PICKUP_NUMBER_Y_OFFSET),
            });

            switch (pickupable.type) {
            case PickupableType_COIN: {
              g.level.coins++;
            } break;

            default:
              INVALID_PATH;
            }
          }
        }
      }
    }
  }

  DoUI(false);

  // Removing old died creatures.
  {  ///
    const auto total = g.level.a.creatures.count;
    int        off   = 0;
    FOR_RANGE (int, i, total) {
      const auto& creature = g.level.a.creatures[i - off];
      if (creature.diedAt.IsSet() && (creature.diedAt.Elapsed() >= DIE_FRAMES)) {
        if (i) {
          g.level.a.creatures.UnstableRemoveAt(i - off);
          off++;
        }
        else if (!i)
          g.meta.reload = true;
      }
    }
  }

  // Removing old damage numbers.
  {  ///
    int removed = 0;
    int left    = -1;
    FOR_RANGE (int, i, g.level.a.numbers.count) {
      const auto& number = g.level.a.numbers[i];
      if (number.createdAt.Elapsed() >= DAMAGE_NUMBERS_FRAMES) {
        if (left == -1)
          left = i;
        removed++;
      }
      else if (left >= 0)
        g.level.a.numbers[left++] = number;
    }
    g.level.a.numbers.count -= removed;
  }

  // Removing old picked up pickupables.
  {  ///
    const auto total = g.level.a.pickupables.count;
    int        off   = 0;
    FOR_RANGE (int, i, total) {
      const auto& pickupable = g.level.a.pickupables[i - off];
      if (pickupable.pickedUpAt.IsSet()
          && (pickupable.pickedUpAt.Elapsed() >= PICKUPABLE_FADE_FRAMES))
      {
        g.level.a.pickupables.UnstableRemoveAt(i - off);
        off++;
      }
    }
  }

  g.level.camera.pos = GetCameraTargetPos();
  g.meta.frame++;
}

void GameDraw() {
  TEMP_USAGE(&g.meta.trashArena);

  BeginMode2D(&g.level.camera);

  // Drawing floor.
  {  ///
    RenderGroup_OneShotRect(
      {
        .pos   = (Vector2)WORLD_SIZE / 2.0f,
        .size  = (Vector2)WORLD_SIZE,
        .color = {0x3a, 0x3a, 0x41, 0xff},
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
        .pos   = spawn.pos,
        .color = {0xc0, 0x29, 0x31, 0xff},
      });
    }

    RenderGroup_End();
  }

  // Drawing creatures.
  {  ///
    const auto fb_creatures = glib->creatures();

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
          .pos   = creature.pos,
          .color = Fade(ColorFromRGB(fb->color()), fade),
        },
        RenderZ_DEFAULT
      );

      if (creature.type == CreatureType_PLAYER) {
        const auto fb_weapons = glib->weapons();

        FOR_RANGE (int, i, PLAYER_WEAPONS_COUNT) {
          const auto& weapon = g.level.playerWeapons[i];
          if (!weapon.type)
            continue;

          const auto    fb = fb_weapons->Get(weapon.type);
          const Vector2 r{1, 0};
          const Vector2 scale{SIGN(Vector2Dot(r, Vector2Rotate(r, weapon.rotation))), 1};

          RenderGroup_OneShotTexture(
            {
              .texId    = fb->texture_ids()->Get(0),
              .rotation = weapon.rotation + ((scale.x < 0) ? (f32)PI : 0.0f),
              .pos      = creature.pos + weapon.offset,
              .scale    = scale,
              .color    = Fade(ColorFromRGB(fb->color()), fade),
            },
            RenderZ_WEAPONS
          );
        }
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
          .pos      = projectile.pos,
          .color    = ColorFromRGB(fb->color()),
        },
        RenderZ_DEFAULT
      );

      // Gizmos.
      if (ge.meta.debugEnabled) {
        RenderGroup_OneShotCircleLines({
          .pos    = projectile.pos,
          .radius = fb->collider_radius(),
          .color  = YELLOW,
        });
      }
    }
  }

  // Drawing damage numbers.
  {  ///
    RenderGroup_Begin(RenderZ_DAMAGE_NUMBERS);
    RenderGroup_SetSortY(0);

    const auto fb_numbers = glib->numbers();

    for (const auto& number : g.level.a.numbers) {
      ASSERT(number.type);
      const auto fb = fb_numbers->Get(number.type);

      const char* format = "%d";
      if (number.type == NumberType_PICKUPABLE)
        format = "+%d";

      const auto text    = TextFormat(format, MAX(1, number.value));
      const auto textLen = strlen(text);
      const auto buffer  = ALLOCATE_ARRAY(&g.meta.trashArena, char, textLen + 1);
      memcpy(buffer, text, textLen + 1);

      const auto e    = number.createdAt.Elapsed();
      const auto p    = Clamp01(e.Progress(DAMAGE_NUMBERS_FRAMES));
      const auto fade = Clamp01(InOutLerp(
        0, 1, e.value, DAMAGE_NUMBERS_FRAMES.value, DAMAGE_NUMBERS_FADE_FRAMES.value
      ));

      RenderGroup_CommandText({
        .pos        = number.pos + Vector2(0, EaseABitUpThenDown(p) / 4.0f),
        .scale      = Vector2(1, 1) * (p * 2),
        .font       = &g.meta.uiFont,
        .text       = buffer,
        .bytesCount = (int)textLen,
        .color      = Fade(ColorFromRGB(fb->color()), fade),
      });
    }

    RenderGroup_End();
  }

  // Drawing pickupables.
  {  ///
    const auto fb_pickupables = glib->pickupables();
    for (const auto& pickupable : g.level.a.pickupables) {
      const auto fb = fb_pickupables->Get(pickupable.type);

      f32 fade = 1;
      {
        const auto e = pickupable.createdAt.Elapsed();
        fade *= Clamp01(e.Progress(PICKUPABLE_FADE_FRAMES));
      }
      if (pickupable.pickedUpAt.IsSet()) {
        const auto e = pickupable.pickedUpAt.Elapsed();
        fade *= Clamp01(1 - e.Progress(PICKUPABLE_FADE_FRAMES));
      }

      RenderGroup_OneShotTexture(
        {
          .texId = fb->texture_id(),
          .pos   = pickupable.pos,
          .color = Fade(WHITE, fade),
        },
        RenderZ_PICKUPABLES
      );
    }
  }

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
          .pos    = pos,
          .radius = shape.DataCircle().radius,
          .color  = color,
        });
      } break;

      case BodyShapeType_RECT: {
        RenderGroup_OneShotRectLines({
          .pos    = pos,
          .size   = shape.DataRect().size,
          .anchor = Vector2Half(),
          .color  = color,
        });
      } break;

      default:
        INVALID_PATH;
      }
    }
  }

  EndMode2D();

  DoUI(true);

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
    DebugText("Close debug menu: F3 change localization");
    DebugText("Close debug menu: F4 change device");
    DebugText("Close debug menu: F5 +10 coins");

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
    debugTextArena("g.meta.arena", g.meta.arena);
    debugTextArena("g.meta.trashArena", g.meta.trashArena);

#define X(type_, name_) \
  DebugText("g.level.a." #name_ ".count: %d", g.level.a.name_.count);
    VECTORS_TABLE;
#undef X
  }
}

///
