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

#define BF_CLAY_SIZING_GROW_XY               \
  .sizing {                                  \
    CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) \
  }

#define BF_CLAY_SIZING_GROW_X    \
  .sizing {                      \
    .width = CLAY_SIZING_GROW(0) \
  }

#define BF_CLAY_SIZING_GROW_Y     \
  .sizing {                       \
    .height = CLAY_SIZING_GROW(0) \
  }

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

#define BF_CLAY_CUSTOM_NINE_SLICE(gamelibNineSlicePtr) \
  .custom {                                            \
    .customData = PushClayCustomData({                 \
      .type      = ClayCustomElementType_NINE_SLICE,   \
      .nineSlice = (gamelibNineSlicePtr),              \
    }),                                                \
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
  ClayCustomElementType_BEAUTIFIER_START,
  ClayCustomElementType_BEAUTIFIER_END,
  ClayCustomElementType_NINE_SLICE,
};

struct ClayCustomData {
  ClayCustomElementType    type           = {};
  u16                      alpha          = u16_max;
  Vector2                  translate      = {0, 0};
  Vector2                  scale          = {1, 1};
  const BFGame::NineSlice* nineSlice      = nullptr;
  Color                    nineSliceColor = WHITE;
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
  ShapeCategory_STATIC     = 1 << 0,
  ShapeCategory_PLAYER     = 1 << 1,
  ShapeCategory_CREATURE   = 1 << 2,
  ShapeCategory_PROJECTILE = 1 << 3,
  // ShapeCategory_ITEM       = 1 << 4,
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
  ShapeUserDataType_CREATURE,  // Can use `index` to query `g.run.a.creatures`.
  // ShapeUserDataType_ITEM,      // Can use `index` to query `g.run.a.items`.
};

struct ShapeUserData {
  ShapeUserDataType type   = {};
  int               _index = {};

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

  int GetCreatureIndex() const {  ///
    ASSERT(type == ShapeUserDataType_CREATURE);
    return _index;
  }

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
  Vector2      detachedPos       = {};
  Vector2      targetDir         = {};
  LogicalFrame startedShootingAt = {};
  LogicalFrame cooldownStartedAt = {};
  int          tier              = {};

  Array<int, WEAPON_MAX_PIERCE> piercedCreatureIds = {};
  int                           piercedCount       = 0;
  // f32 lastShotDirection = {};
};

struct Item {
  ItemType type  = {};
  int      count = {};
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
  f32                               range              = {};
  f32                               travelledDistance  = 0;
};

struct MakeProjectileData {
  ProjectileType type               = {};
  int            ownerCreatureIndex = {};
  Vector2        pos                = {};
  Vector2        dir                = {};
  f32            range              = {};
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

struct MakeNumberData {
  NumberType type  = {};
  int        value = {};
  Vector2    pos   = {};
};

lframe GetWaveDuration(int waveIndex) {  ///
#if BF_RELEASE
  constexpr int durations_[]{20, 25, 30, 35, 40, 45, 50, 55, 60, 60,
                             60, 60, 60, 60, 60, 60, 60, 60, 60, 90};
  VIEW_FROM_ARRAY_DANGER(durations);
  const int seconds = durations[MIN(durations.count - 1, waveIndex)];
#else
  const int seconds = 3 + waveIndex * 5;
#endif
  return lframe::MakeUnscaled(seconds * FIXED_FPS);
}

enum ScreenType {
  ScreenType_GAMEPLAY,
  ScreenType_PICKED_UP_ITEM,
  ScreenType_UPGRADES,
  ScreenType_SHOP,
  ScreenType_END,
};

struct ShopItem {
  WeaponType weapon = {};
  ItemType   item   = {};
  int        price  = {};
  int        tier   = {};
};

struct Upgrade {
  StatType stat = {};
  int      tier = {};
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
  } meta;

  struct Run {
    bool       reload = false;
    bool       won    = false;
    ScreenType screen = ScreenType_GAMEPLAY;

    bool scheduledUI            = false;
    bool scheduledPickedUpItems = false;
    bool scheduledUpgrades      = false;
    bool scheduledShop          = false;
    bool scheduledEnd           = false;
    bool scheduledNextWave      = false;

    Camera camera{
      .zoom          = METER_LOGICAL_SIZE,
      .texturesScale = 1.0f / METER_LOGICAL_SIZE,
    };

    b2WorldId    world           = {};
    int          nextCreatureId  = 0;
    LogicalFrame playerDiedAt    = {};
    int          toSpawn         = 3;
    LogicalFrame lastLifestealAt = {};

    f32 xp            = 0;
    f32 nextLevelXp   = 10;
    int coins         = 0;
    int crates        = 0;
    int xpLevel       = 1;
    int previousLevel = 1;
    int pierceCount   = 0;

    int          waveIndex     = 0;
    LogicalFrame waveStartedAt = {};

    Array<Weapon, PLAYER_WEAPONS_COUNT> playerWeapons      = {};
    int                                 playerWeaponsCount = 0;
    Array<int, StatType_COUNT>          playerStats        = {};

    struct {
      ItemType toPick         = {};
      int      recyclingPrice = {};
    } pickedUpItem;

    struct {
      Array<Upgrade, 4> toPick = {};

      int rerolledTimes = 0;
      int rerollPrice   = 0;
    } upgrades;

    struct {
      Array<ShopItem, 4> toPick = {};

      int rerolledTimes = 0;
      int rerollPrice   = 0;
    } shop;

    struct Allocated {
      // Using "X-macros". ref: https://www.geeksforgeeks.org/c/x-macros-in-c/
#define VECTORS_TABLE              \
  X(Creature, creatures)           \
  X(CreatureSpawn, creatureSpawns) \
  X(Projectile, projectiles)       \
  X(int, projectilesToRemove)      \
  X(BodyShape, bodyShapes)         \
  X(int, justDamagedCreatures)     \
  X(Number, numbers)               \
  X(Pickupable, pickupables)       \
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
  } run;
} g = {};

template <typename T>
void CheckCollisionsRect(
  ShapeCategory           ofWhat,
  /* ShapeCategory */ u32 withWhat,
  Vector2                 center,
  Vector2                 size,
  Vector2                 dir,
  bool (*onCollided)(b2ShapeId, T*),
  T* context
) {  ///
  ASSERT(ofWhat);
  ASSERT(withWhat);

  ASSERT(FloatEquals(Vector2Length(dir), 1));
  ASSERT(size.x > 0);
  ASSERT(size.y > 0);

  const auto         box = b2MakeBox(size.x / 2, size.y / 2);
  const b2ShapeProxy proxy
    = b2MakeOffsetProxy(box.vertices, box.count, 0, {center.x, center.y}, {dir.x, dir.y});
  b2World_OverlapShape(
    g.run.world,
    &proxy,
    b2QueryFilter{
      .categoryBits = ofWhat,
      .maskBits     = withWhat,
    },
    (bool (*)(b2ShapeId, void*))onCollided,
    context
  );
}

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
  *g.run.a.numbers.Add() = number;
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

void BF_CLAY_TEXT(Clay_String string, Color color = WHITE) {  ///
  CLAY_TEXT(string, CLAY_TEXT_CONFIG({.textColor = ToClayColor(color)}));
}

void BF_CLAY_TEXT(const char* text, Color color = WHITE) {  ///
  auto len           = strlen(text);
  auto allocatedText = ALLOCATE_ARRAY(&g.meta.trashArena, char, len);
  memcpy(allocatedText, text, len + 1);

  Clay_String string{
    .length = (i32)len,
    .chars  = allocatedText,
  };
  BF_CLAY_TEXT(string, color);
}

void DestroyBody(Body* body) {  ///
  b2DestroyBody(body->id);
  for (auto& shape : g.run.a.bodyShapes) {
    if (shape.body.createdId == body->createdId)
      shape.active = false;
  }
}

void AddBodyShape(BodyShape v) {  ///
  ASSERT(v.active);
  ASSERT(v.type);

  for (auto& shape : g.run.a.bodyShapes) {
    if (!shape.active) {
      shape = v;
      return;
    }
  }

  *g.run.a.bodyShapes.Add() = v;
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

  b2BodyId body = b2CreateBody(g.run.world, &bodyDef);

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
  FOR_RANGE (int, i, g.run.a.creatures.count) {
    if (!g.run.a.creatures[i].active) {
      auto ok = false;

      if ((i == 0) && (g.run.a.creatures.count == 0))
        ok = true;
      if ((i > 0) && (g.run.a.creatures.count > 0))
        ok = true;

      if (ok) {
        index = i;
        break;
      }
    }
  }

  if (index == -1) {
    index = g.run.a.creatures.count;
    g.run.a.creatures.Add();
  }

  auto hurtboxRadius = PLAYER_HURTBOX_RADIUS;
  if (data.type != CreatureType_PLAYER)
    hurtboxRadius = MOB_HURTBOX_RADIUS;

  const auto fb_creature = glib->creatures()->Get(data.type);
  auto       health      = (f32)fb_creature->health();
  if (data.type == CreatureType_PLAYER)
    health = (f32)g.run.playerStats[StatType_HP];

  ASSERT(health > 0);

  Creature creature{
    .id        = g.run.nextCreatureId++,
    .type      = data.type,
    .health    = health,
    .maxHealth = health,
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

  g.run.a.creatures[index] = creature;
}

void MakeProjectile(MakeProjectileData data) {  ///
  ASSERT(data.type);
  ASSERT(data.dir != Vector2Zero());

  Projectile projectile{
    .type               = data.type,
    .ownerCreatureIndex = data.ownerCreatureIndex,
    .pos                = data.pos,
    .dir                = data.dir,
    .damage             = data.damage,
    .range              = data.range,
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

  *g.run.a.projectiles.Add() = projectile;
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

#define PLAYER_CREATURE (g.run.a.creatures[0])

Vector2 GetCameraTargetPos() {  ///
  auto tpos = PLAYER_CREATURE.pos;

  tpos.x += Lerp(
    WORLD_CORNER_MARGIN_X, -WORLD_CORNER_MARGIN_X, PLAYER_CREATURE.pos.x / (f32)WORLD_X
  );
  tpos.y += Lerp(
    WORLD_CORNER_MARGIN_Y, -WORLD_CORNER_MARGIN_Y, PLAYER_CREATURE.pos.y / (f32)WORLD_Y
  );

  tpos.x = Clamp(tpos.x, CAMERA_MIN_WORLD_X, CAMERA_MAX_WORLD_X);
  tpos.y = Clamp(tpos.y, CAMERA_MIN_WORLD_Y, CAMERA_MAX_WORLD_Y);

  return tpos;
}

void RecalculatePlayerWeaponOffsets() {  ///
  // Checking that playerWeaponsCount is correct.
  int weaponsCount = 0;
  for (const auto& weapon : g.run.playerWeapons) {
    if (weapon.type)
      weaponsCount++;
  }
  ASSERT(g.run.playerWeaponsCount == weaponsCount);

  // Checkin that INVALID weapons are on the end of `playerWeapons`.
  for (int i = weaponsCount; i < g.run.playerWeapons.count; i++)
    ASSERT(!g.run.playerWeapons[i].type);

  // Recalculating offsets.
  const auto startingAngle = PLAYER_WEAPONS_STARTING_ANGLES[weaponsCount - 1];
  const auto angleDelta    = 2.0f * (f32)PI / (f32)weaponsCount;
  FOR_RANGE (int, i, g.run.playerWeaponsCount) {
    g.run.playerWeapons[i].offset
      = Vector2Rotate(Vector2(1, 0), i * angleDelta + startingAngle);
  }
}

void RunInit() {
  // Creating box2d world.
  {  ///
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = b2Vec2{0, 0};
    g.run.world         = b2CreateWorld(&worldDef);
  }

  // Initializing `playerStats`.
  g.run.playerStats[StatType_HP] = 20;

  // Making player.
  MakeCreature({
    .type = CreatureType_PLAYER,
    .pos  = (Vector2)WORLD_SIZE / 2.0f,
  });

  g.run.camera.pos = GetCameraTargetPos();

  struct {
    WeaponType type = {};
  } weapons_[]{
    {.type = WeaponType_SWORD},
    // {.type = WeaponType_BOW},
    // {.type = WeaponType_GUN},
  };
  VIEW_FROM_ARRAY_DANGER(weapons);

  FOR_RANGE (int, i, weapons.count) {
    auto& weapon = g.run.playerWeapons[i];
    weapon.type  = weapons[i].type;
    weapon.tier  = GRAND.Rand() % (TOTAL_TIERS - 1);
    g.run.playerWeaponsCount++;
  }
  RecalculatePlayerWeaponOffsets();

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

  g.run.waveStartedAt = {};
  g.run.waveStartedAt.SetNow();
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

  RunInit();
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
  auto& creature = g.run.a.creatures[creatureIndex];

  const auto fb = glib->creatures()->Get(creature.type);
  if (!fb->can_be_knocked_back())
    impulse = 0;

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
      if (GRAND.FRand() < GetLifestealChance(g.run.playerStats[StatType_LIFE_STEAL])) {
        bool canLifesteal = false;
        if (!g.run.lastLifestealAt.IsSet())
          canLifesteal = true;
        else if (g.run.lastLifestealAt.Elapsed() >= LIFESTEAL_COOLDOWN_FRAMES)
          canLifesteal = true;

        if (canLifesteal && (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth)) {
          PLAYER_CREATURE.health
            = MoveTowards(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth, 1);
          g.run.lastLifestealAt = {};
          g.run.lastLifestealAt.SetNow();
        }
      }
    }
    creature.health -= damage;

    creature.lastDamagedAt = {};
    creature.lastDamagedAt.SetNow();

    b2Body_ApplyLinearImpulseToCenter(
      creature.body.id, ToB2Vec2(direction * impulse), true
    );

    if (!g.run.a.justDamagedCreatures.Contains(creatureIndex))
      *g.run.a.justDamagedCreatures.Add() = creatureIndex;

    return true;
  }

  return false;
}

void RunReset() {  ///
  ge.settings.screenFade = 0;
  SDL_HideCursor();

  b2DestroyWorld(g.run.world);

  auto a = g.run.a;
  a.Reset();
  g.run = {.a = a};

  g.meta.arena.used = 0;
}

int GetRerollPrice(int waveIndex, int rerolledTimes) {  ///
  int rerollPricesPerWave_[]{1,  2,  3,  4,  5,  6,  7,  9,  9,  11,
                             12, 13, 14, 15, 17, 18, 18, 20, 21, 23};
  VIEW_FROM_ARRAY_DANGER(rerollPricesPerWave);

  int rerollPriceIncreasePerWave_[]{1, 1, 1, 1, 2, 2, 2, 3, 3, 4,
                                    4, 4, 5, 5, 6, 6, 6, 7, 7, 8};
  VIEW_FROM_ARRAY_DANGER(rerollPriceIncreasePerWave);

  return rerollPricesPerWave[waveIndex]
         + rerolledTimes * rerollPriceIncreasePerWave[waveIndex];
}

void RefillUpgradesToPick() {  ///
  const auto fb_stats = glib->stats();

  FOR_RANGE (int, i, g.run.upgrades.toPick.count) {
    while (1) {
      const auto newStat = (StatType)(GRAND.Rand() % fb_stats->size());
      if (!newStat)
        continue;

      const auto fb = fb_stats->Get(newStat);

      // Can't upgrade `curse`.
      if (!fb->upgrade_texture_id())
        continue;

      // Not showing same upgrade twice.
      bool contains = false;
      for (const auto& v : g.run.upgrades.toPick) {
        if (v.stat == newStat) {
          contains = true;
          break;
        }
      }
      if (contains)
        continue;

      // Setting upgrade.
      g.run.upgrades.toPick[i] = {
        .stat = newStat,
        .tier = (int)(GRAND.Rand() % TOTAL_TIERS),
      };
      break;
    }
  }
}

ItemType GenerateRandomItem() {  ///
  ItemType type{};
  while (!type)
    type = (ItemType)(GRAND.Rand() % (u32)ItemType_COUNT);
  return type;
}

int GenerateItemPrice() {  ///
  return 15 + (int)(GRAND.Rand() % 20);
}

void RefillShopToPick() {  ///
  const auto fb_items   = glib->items();
  const auto fb_weapons = glib->weapons();

  auto& toPick = g.run.shop.toPick;

  for (auto& v : toPick) {
    v = {.price = GenerateItemPrice()};

    const bool setToItem = (GRAND.FRand() <= SHOP_ITEM_RATIO);
    if (setToItem) {
      v.item = GenerateRandomItem();

      const auto fb = fb_items->Get(v.item);
      v.tier        = fb->tier();
    }
    else {
      while (!v.weapon) {
        v.weapon = (WeaponType)(GRAND.Rand() % fb_weapons->size());
        // Legendary weapons can't be sold in shop.
        v.tier = (int)(GRAND.Rand() % (TOTAL_TIERS - 1));
      }
    }
  }
}

void AddItem(ItemType v) {  ///
  bool increasedExistingItemCount = false;
  int  i                          = 0;
  for (auto& item : g.run.a.playerItems) {
    i++;
    if (item.type == v) {
      item.count++;
      increasedExistingItemCount = true;
      break;
    }
  }
  if (!increasedExistingItemCount) {
    Item item{.type = v, .count = 1};
    *g.run.a.playerItems.Add() = item;
  }
}

void DoUI(bool draw) {
  // NOTE: Logic must be executed only when `draw` is false!
  // e.g. updating mouse position, processing `clicked()`,
  // logically reacting to `Clay_Hovered()`, changing game's state, etc.

  const auto fb_items       = glib->items();
  const auto fb_weapons     = glib->weapons();
  const auto fb_pickupables = glib->pickupables();

  // Setup.
  // {  ///
  if (!draw) {
    // Updating clay mouse pos.
    // TODO: TOUCH!
    const auto pos = ScreenPosToLogical(GetMouseScreenPos());
    Clay_SetPointerState({pos.x, LOGICAL_RESOLUTION.y - pos.y}, false);
  }

  constexpr int MAX_BEAUTIFIERS      = 32;
  const auto    localization         = glib->localizations()->Get(ge.meta.localization);
  const auto    localization_strings = localization->strings();

  TEMP_USAGE(&g.meta.trashArena);

  Clay_BeginLayout();

  auto textures = glib->atlas_textures();

  Array<Beautify, MAX_BEAUTIFIERS> beautifiers{};
  int                              beautifiersCount = 0;
  // }

  LAMBDA (bool, clicked, ()) {  ///
    return !draw && Clay_Hovered() && IsMouseReleased(L);
  };

  // LAMBDA (bool, componentButton, (bool enabled, auto innerLambda)) { ///
  const u32 buttonColors_[]{
    glib->ui_button_hovered_color(),
    glib->ui_button_default_color(),
    glib->ui_button_disabled_color(),
  };
  VIEW_FROM_ARRAY_DANGER(buttonColors);

  LAMBDA (bool, componentButton, (bool enabled, auto innerLambda)) {
    bool result{};

    CLAY({
      .layout{BF_CLAY_PADDING_ALL(8)},
      .backgroundColor
      = ToClayColor(ColorFromRGB(buttonColors[(enabled ? (Clay_Hovered() ? 0 : 1) : 2)])),
    }) {
      result = enabled && clicked();
      innerLambda();
    }

    return result;
  };
  // }

  // LAMBDA (void, componentSlot, (bool enabled, int tier, auto innerLambda)) { ///
  const auto slotTexs   = glib->ui_item_slot_texture_ids();
  const auto slotColors = glib->ui_item_slot_colors();

  LAMBDA (void, componentSlot, (bool enabled, int tier, auto innerLambda)) {
    const int t = (enabled ? (Clay_Hovered() ? 0 : 1) : 2) + 3 * tier;
    BF_CLAY_IMAGE(
      {
        .texId = slotTexs->Get(t),
        .color = ColorFromRGB(slotColors->Get(t)),
      },
      innerLambda
    );
  };
  // }

  LAMBDA (void, componentStats, ()) {  ///
    CLAY({.layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
      // Stats label.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_stats_locale());
      }

      LAMBDA (void, statsEntry, (int iconTexId, int locale, int value)) {
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_X,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          BF_CLAY_IMAGE({.texId = iconTexId});
          BF_CLAY_TEXT(" ");
          BF_CLAY_TEXT_LOCALIZED_DANGER(locale);
          BF_CLAY_SPACER_HORIZONTAL;
          BF_CLAY_TEXT(TextFormat("%d", value));
        }
      };

      // Current level.
      statsEntry(
        glib->ui_shop_current_level_icon_texture_id(),
        glib->ui_current_level_locale(),
        g.run.xpLevel
      );

      // Stats.
      CLAY({.layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM}})
      FOR_RANGE (int, i, (int)StatType_COUNT - 1) {
        const auto type = (StatType)(i + 1);
        const auto fb   = glib->stats()->Get(type);
        statsEntry(fb->icon_texture_id(), fb->name_locale(), g.run.playerStats[type]);
      }
    }
  };

  LAMBDA (void, componentItem, (const Item& item)) {  ///
    const auto fb = glib->items()->Get(item.type);
    componentSlot(false, fb->tier(), [&]() BF_FORCE_INLINE_LAMBDA {
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        BF_CLAY_IMAGE({.texId = fb->texture_id()});

        // Showing count if there are multiple of the same item.
        if (item.count > 1) {
          CLAY({.floating{
            .attachPoints{
              .element = CLAY_ATTACH_POINT_RIGHT_BOTTOM,
              .parent  = CLAY_ATTACH_POINT_RIGHT_BOTTOM,
            },
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            .attachTo           = CLAY_ATTACH_TO_PARENT,
          }}) {
            BF_CLAY_TEXT(TextFormat("x%d", item.count));
          }
        }
      }
    });
  };

  LAMBDA (void, componentWeapon, (const Weapon& weapon)) {  ///
    componentSlot(false, weapon.tier, [&]() BF_FORCE_INLINE_LAMBDA {
      if (weapon.type) {
        const auto fb = fb_weapons->Get(weapon.type);
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_XY,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          BF_CLAY_IMAGE({
            .texId = fb->texture_ids()->Get(0),
            .color = ColorFromRGB(fb->color()),
          });
        }
      }
    });
  };

  // Gameplay.
  if ((g.run.screen == ScreenType_GAMEPLAY)           //
      || (g.run.screen == ScreenType_PICKED_UP_ITEM)  //
      || (g.run.screen == ScreenType_UPGRADES))
  {
    CLAY({
      .layout{BF_CLAY_SIZING_GROW_XY},
      .floating{
        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
        .attachTo           = CLAY_ATTACH_TO_PARENT,
      },
    }) {
      FLOATING_BEAUTIFY;

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
              .sourceMargins{.right = 1 - (f32)g.run.xp / (f32)g.run.nextLevelXp},
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
              BF_CLAY_TEXT(TextFormat("%d", g.run.xpLevel));
            }
          }
        });

        CLAY({.layout{
          .childGap = 8,
          BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        }}) {
          BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
          BF_CLAY_TEXT(TextFormat("%d", g.run.coins));
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

        BF_CLAY_TEXT(TextFormat("Wave %d", g.run.waveIndex + 1));

        if (g.run.screen == ScreenType_GAMEPLAY) {
          const auto remainingFrames
            = GetWaveDuration(g.run.waveIndex) - g.run.waveStartedAt.Elapsed();
          const int remainingSeconds
            = CeilDivision(MAX(1, remainingFrames.value), FIXED_FPS);
          BF_CLAY_TEXT(TextFormat(". %d...", remainingSeconds));
        }
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

        bool showVersion = ge.meta.debugEnabled;
#if BF_SHOW_VERSION
        showVersion = true;
#endif
        if (showVersion)
          BF_CLAY_TEXT(g_gameVersion);
      }
      // }

      // Picked up crates.
      // { ///
      CLAY({
        .layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM},
        .floating{
          .attachPoints{
            .element = CLAY_ATTACH_POINT_RIGHT_TOP,
            .parent  = CLAY_ATTACH_POINT_RIGHT_TOP,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        FOR_RANGE (int, i, g.run.crates) {
          const auto fb = fb_pickupables->Get(PickupableType_CRATE);
          BF_CLAY_IMAGE({.texId = fb->texture_id()});
        }
      }
      // }
    }
  }

  // Picked up item.
  if (g.run.screen == ScreenType_PICKED_UP_ITEM) {  ///
    CLAY({.layout{
      BF_CLAY_SIZING_GROW_XY,
      BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
    }}) {
      // Columns (1) picked up item, (2) stats
      CLAY({.layout{
        .childGap = 8,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        // Picked up item.
        CLAY({.layout{.layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
          // "Item Found" label.
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_item_found_locale());

          // Item.
          CLAY({
            .layout{
              .sizing{CLAY_SIZING_FIXED(235), CLAY_SIZING_FIXED(320)},
              BF_CLAY_PADDING_ALL(8),
              .childGap        = 8,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice()),
          }) {
            CLAY({.layout{.childGap = 8}}) {
              const auto type = g.run.pickedUpItem.toPick;
              const auto fb   = fb_items->Get(type);
              const Item item{.type = type, .count = 1};
              componentItem(item);

              BF_CLAY_TEXT_LOCALIZED_DANGER(fb->name_locale());
            }

            // TODO: Item stats.
          }

          // Take and Recycle buttons.
          CLAY({.layout{.childGap = 8}}) {
            const bool takeClicked = componentButton(true, [&]() BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_take_locale());
            });

            const bool recycleClicked
              = componentButton(true, [&]() BF_FORCE_INLINE_LAMBDA {
                  CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
                    BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_recycle_locale());
                    BF_CLAY_TEXT(TextFormat(" (+%d ", g.run.pickedUpItem.recyclingPrice));
                    BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
                    BF_CLAY_TEXT(")");
                  }
                });

            if (takeClicked)
              AddItem(g.run.pickedUpItem.toPick);
            else if (recycleClicked)
              g.run.coins += g.run.pickedUpItem.recyclingPrice;

            if (takeClicked || recycleClicked) {
              g.run.crates--;
              if (g.run.crates)
                g.run.scheduledPickedUpItems = true;
              else
                g.run.scheduledUpgrades = true;
            }
          }
        }

        // Stats.
        componentStats();
      }
    }
  }
  // Upgrades.
  else if (g.run.screen == ScreenType_UPGRADES) {  ///
    // Vertical columns with upgrades and stats;
    CLAY({.layout{
      BF_CLAY_SIZING_GROW_XY,
      BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
    }}) {
      // Upgrades.
      CLAY({.layout{
        .childGap = 20,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        // Level up label.
        BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_level_up_locale());

        // Upgrades.
        CLAY({.layout{.childGap = 20}}) {
          const auto fb_stats = glib->stats();

          FOR_RANGE (int, i, g.run.upgrades.toPick.count) {
            const auto upgrade = g.run.upgrades.toPick[i];
            const auto fb      = fb_stats->Get(upgrade.stat);
            CLAY({.layout{
              .childGap        = 8,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              CLAY({.layout{.childGap = 8}}) {
                // Slot with upgrade's image.
                componentSlot(false, upgrade.tier, [&]() BF_FORCE_INLINE_LAMBDA {
                  CLAY({
                    .layout{
                      BF_CLAY_SIZING_GROW_XY,
                      BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                    },
                  }) {
                    BF_CLAY_IMAGE({.texId = fb->upgrade_texture_id()});
                  }
                });

                // Name.
                BF_CLAY_TEXT_LOCALIZED_DANGER(fb->upgrade_name_locale());
              }

              // Amount.
              const auto amount = fb->upgrade_values()->Get(upgrade.tier);
              CLAY({}) {
                BF_CLAY_TEXT(TextFormat("+%d ", amount));
                BF_CLAY_TEXT_LOCALIZED_DANGER(fb->name_locale());
              }

              // Choose button.
              const auto clicked = componentButton(true, [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_choose_locale());
              });
              if (clicked) {
                if (g.run.previousLevel < g.run.xpLevel) {
                  g.run.previousLevel++;
                  g.run.scheduledUpgrades = true;
                }
                else
                  g.run.scheduledShop = true;

                g.run.playerStats[upgrade.stat] += amount;

                switch (upgrade.stat) {
                case StatType_HP: {
                  PLAYER_CREATURE.health += amount;
                  PLAYER_CREATURE.maxHealth += amount;
                } break;

                default:
                  break;
                }
              }
            }
          }
        }

        // Reroll button.
        const bool canReroll = (g.run.upgrades.rerollPrice <= g.run.coins);
        const bool rerolled  = componentButton(canReroll, [&]() BF_FORCE_INLINE_LAMBDA {
          CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_reroll_locale());
            BF_CLAY_TEXT(TextFormat(" - %d ", g.run.upgrades.rerollPrice));
            BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
          }
        });
        if (rerolled) {
          g.run.coins -= g.run.upgrades.rerollPrice;
          g.run.upgrades.rerollPrice
            = GetRerollPrice(g.run.waveIndex, ++g.run.upgrades.rerolledTimes);
          RefillUpgradesToPick();
        }
      }

      // Stats.
      componentStats();
    }
  }
  // Shop.
  else if (g.run.screen == ScreenType_SHOP) {  ///
    // Columns.
    CLAY({.layout{
      BF_CLAY_SIZING_GROW_XY,
      BF_CLAY_PADDING_ALL(8),
      .childGap = 8,
    }}) {
      // Left column that contains:
      // 1. wave, coins, reroll.
      // 2. items to buy.
      // 3. player's items and weapons.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        .childGap        = 8,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        // 1. Wave, coins, reroll.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_X,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          // Wave.
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_shop_locale());
          BF_CLAY_TEXT(" (");
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_wave_locale());
          BF_CLAY_TEXT(TextFormat(" %d)", g.run.waveIndex + 1));

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
            BF_CLAY_TEXT(TextFormat("%d ", g.run.coins));
            BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
          }

          BF_CLAY_SPACER_HORIZONTAL;

          // Reroll button.
          const bool canReroll = (g.run.coins >= g.run.shop.rerollPrice);
          const bool rerolled  = componentButton(canReroll, [&]() BF_FORCE_INLINE_LAMBDA {
            CLAY({.layout{
              BF_CLAY_PADDING_ALL(8),
              BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
            }}) {
              BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_reroll_locale());

              ASSERT(g.run.shop.rerollPrice >= 0);

              if (g.run.shop.rerollPrice > 0) {
                BF_CLAY_TEXT(TextFormat(" - %d ", g.run.shop.rerollPrice));
                BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
              }
            }
          });
          if (rerolled) {
            g.run.coins -= g.run.shop.rerollPrice;
            g.run.shop.rerollPrice
              = GetRerollPrice(g.run.waveIndex, ++g.run.shop.rerolledTimes);
            RefillShopToPick();
          }
        }

        BF_CLAY_SPACER_VERTICAL;

        // 2. Items to buy.
        CLAY({.layout{BF_CLAY_SIZING_GROW_X, .childGap = 8}}) {
          FLOATING_BEAUTIFY;

          BF_CLAY_SPACER_HORIZONTAL;

          for (auto& v : g.run.shop.toPick) {
            bool canBuy = ((v.item || v.weapon) && (v.price <= g.run.coins));
            int  emptyOrSameWeaponSlotIndex = -1;
            if (canBuy && v.weapon) {
              // Trying to find empty weapon slot.
              FOR_RANGE (int, i, g.run.playerWeapons.count) {
                const auto& weapon = g.run.playerWeapons[i];
                if (!weapon.type) {
                  emptyOrSameWeaponSlotIndex = i;
                  break;
                }
              }
              // If not found, trying to find the same player's weapon to upgrade.
              if (emptyOrSameWeaponSlotIndex == -1) {
                FOR_RANGE (int, i, g.run.playerWeapons.count) {
                  const auto& weapon        = g.run.playerWeapons[i];
                  const bool  canBeUpgraded = (weapon.type == v.weapon)  //
                                             && (weapon.tier == v.tier)  //
                                             && (v.tier < TOTAL_TIERS - 1);
                  if (canBeUpgraded) {
                    emptyOrSameWeaponSlotIndex = i;
                    break;
                  }
                }
              }
              if (emptyOrSameWeaponSlotIndex == -1)
                canBuy = false;
            }

            CLAY({.layout{.sizing{CLAY_SIZING_FIXED(235), CLAY_SIZING_FIXED(320)}}})
            if (v.item || v.weapon) {
              CLAY({
                .layout{
                  BF_CLAY_SIZING_GROW_XY,
                  BF_CLAY_PADDING_ALL(8),
                  .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice()),
              }) {
                const auto fb_item   = (v.item ? fb_items->Get(v.item) : nullptr);
                const auto fb_weapon = (v.weapon ? fb_weapons->Get(v.weapon) : nullptr);

                // TODO: Highlight price, not item's frame.

                // Item's image + name.
                CLAY({.layout{.childGap = 8}}) {
                  // Image.
                  componentSlot(false, v.tier, [&]() BF_FORCE_INLINE_LAMBDA {
                    CLAY({.layout{
                      BF_CLAY_SIZING_GROW_XY, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER
                    }}) {
                      if (v.item || v.weapon) {
                        int texId = 0;
                        if (v.item)
                          texId = fb_item->texture_id();
                        if (v.weapon)
                          texId = fb_weapon->texture_ids()->Get(0);
                        BF_CLAY_IMAGE({.texId = texId});
                      }
                    }
                  });

                  // Name.
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
                  BF_CLAY_SIZING_GROW_X,
                  BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                }}) {
                  // Buying item / weapon.
                  const auto bought
                    = componentButton(canBuy, [&]() BF_FORCE_INLINE_LAMBDA {
                        CLAY({.layout{
                          BF_CLAY_SIZING_GROW_X,
                          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                        }}) {
                          BF_CLAY_TEXT(
                            TextFormat("%d ", v.price),
                            (v.price <= g.run.coins ? WHITE : RED)
                          );
                          BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
                        }
                      });

                  if (bought) {
                    g.run.coins -= v.price;
                    if (v.weapon) {
                      auto& weapon = g.run.playerWeapons[emptyOrSameWeaponSlotIndex];
                      if (weapon.type) {
                        // Upgrading an existing weapon if no empty slot found.
                        ASSERT(weapon.type == v.weapon);
                        ASSERT(weapon.tier == v.tier);
                        weapon.tier += 1;
                      }
                      else {
                        // Filling empty weapon slot if exists.
                        weapon.type = v.weapon;
                        weapon.tier = v.tier;
                        g.run.playerWeaponsCount++;
                        RecalculatePlayerWeaponOffsets();
                      }
                    }
                    else if (v.item) {
                      AddItem(v.item);
                    }
                    else
                      INVALID_PATH;
                    v = {};
                  }
                }
              }
            }
          }

          BF_CLAY_SPACER_HORIZONTAL;
        }

        BF_CLAY_SPACER_VERTICAL;

        // 3. Player's items and weapons.
        CLAY({.layout{BF_CLAY_SIZING_GROW_X}}) {
          CLAY({.layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            // Items label.
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_items_locale());

            // Items.
            CLAY({.layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
              constexpr int ITEMS_X = 10;
              const int     ITEMS_Y = CeilDivision(g.run.a.playerItems.count, ITEMS_X);

              FOR_RANGE (int, y, ITEMS_Y) {
                CLAY({.layout{.childGap = 8}})
                FOR_RANGE (int, x, ITEMS_X) {
                  const int t = y * ITEMS_X + x;
                  if (t >= g.run.a.playerItems.count)
                    break;
                  componentItem(g.run.a.playerItems[t]);
                }
              }
            }
          }

          BF_CLAY_SPACER_HORIZONTAL;

          CLAY({.layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            // Weapons label.
            CLAY({}) {
              BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_weapons_locale());

              int weaponsCount = 0;
              for (const auto& weapon : g.run.playerWeapons) {
                if (weapon.type)
                  weaponsCount++;
              }

              BF_CLAY_TEXT(TextFormat(" (%d/%d)", weaponsCount, g.run.playerWeapons.count)
              );
            }

            // Weapons.
            constexpr int WEAPONS_X = 3;
            constexpr int WEAPONS_Y = 2;
            static_assert(WEAPONS_X * WEAPONS_Y == g.run.playerWeapons.count);

            CLAY({.layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM}})
            FOR_RANGE (int, y, 2) {
              CLAY({.layout{.childGap = 8}})
              FOR_RANGE (int, x, 3) {
                const int t = y * WEAPONS_X + x;
                componentWeapon(g.run.playerWeapons[t]);
              }
            }
          }
        }
      }

      // Right column that contains stats and next wave button.
      CLAY({.layout{BF_CLAY_SIZING_GROW_Y}}) {
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_Y,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          // Stats.
          componentStats();

          BF_CLAY_SPACER_VERTICAL;

          // Advance to the next wave button.
          g.run.scheduledNextWave = componentButton(true, [&]() BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_next_wave_locale());
            BF_CLAY_TEXT(" (");
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_wave_locale());
            BF_CLAY_TEXT(TextFormat(" %d)", g.run.waveIndex + 2));
          });
        }
      }
    }
  }
  // End.
  else if (g.run.screen == ScreenType_END) {  ///
    CLAY({.layout{
      BF_CLAY_SIZING_GROW_XY,
      BF_CLAY_PADDING_ALL(8),
      .childGap = 8,
      BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
    }}) {
      // Header. Run Won / Lost, Wave.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        // Run Won / Lost.
        {
          const int locale = (g.run.won ? glib->ui_won_locale() : glib->ui_lost_locale());
          BF_CLAY_TEXT_LOCALIZED_DANGER(locale);
        }

        BF_CLAY_TEXT(". ");

        // Wave.
        BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_wave_locale());
        BF_CLAY_TEXT(TextFormat(" %d", g.run.waveIndex + 1));
      }

      // Main. Player's stats, weapons, items.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        .childGap = 8,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        // Stats.
        componentStats();

        // Player's weapons and items.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_XY,
          .childGap        = 8,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          // Weapons label.
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_weapons_locale());

          // Weapons.
          CLAY({.layout{.childGap = 8}})
          for (const auto& weapon : g.run.playerWeapons) {
            if (weapon.type)
              componentWeapon(weapon);
          }

          // Items label.
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_items_locale());

          // Items.
          const auto& items = g.run.a.playerItems;

          constexpr int ITEMS_X = 10;
          const int     ITEMS_Y = CeilDivision(items.count, ITEMS_X);

          CLAY({.layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM}})
          FOR_RANGE (int, y, ITEMS_Y) {
            CLAY({.layout{.childGap = 8}})
            FOR_RANGE (int, x, ITEMS_X) {
              const auto t = y * ITEMS_X + x;
              if (t < items.count)
                componentItem(items[t]);
            }
          }
        }
      }

      // Footer. Restart.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        g.run.reload = componentButton(true, [&]() BF_FORCE_INLINE_LAMBDA {
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_restart_locale());
        });
      }
    }
  }
  else if (g.run.screen == ScreenType_GAMEPLAY) {
    // NOTE: Intentionally left blank.
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

            case ClayCustomElementType_NINE_SLICE: {
              const auto downscaleFactor = (f32)glib->atlas_downscale_factor();
              const auto fb              = data.nineSlice;
              RenderGroup_CommandTextureNineSlice({
                .texId = data.nineSlice->texture_id(),
                .pos{bb.x, bb.y},
                .anchor{},
                .color{
                  data.nineSliceColor.r,
                  data.nineSliceColor.g,
                  data.nineSliceColor.b,
                  (u8)((f32)data.nineSliceColor.a * beautifierAlpha)
                },
                .nineSliceMargins{
                  (f32)fb->left() / downscaleFactor,
                  (f32)fb->right() / downscaleFactor,
                  (f32)fb->top() / downscaleFactor,
                  (f32)fb->bottom() / downscaleFactor,
                },
                .nineSliceSize{(f32)bb.width, (f32)bb.height},
              });
            } break;

            default:
              INVALID_PATH;
            }
          } break;
          }
        }
        else {
          // UI elements' gizmos.
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

bool OnWeaponCollided(b2ShapeId shapeId, Weapon* weapon) {  ///
  const bool continueCollisions = true;
  const auto userData = ShapeUserData::FromPointer(b2Shape_GetUserData(shapeId));

  const auto  creatureIndex = userData.GetCreatureIndex();
  const auto& creature      = g.run.a.creatures[creatureIndex];

  ASSERT(creature.type);
  ASSERT(creature.active);

  if (creature.type == CreatureType_PLAYER)
    return continueCollisions;

  const auto fb = glib->weapons()->Get(weapon->type);

  if (ArrayContains(weapon->piercedCreatureIds.base, weapon->piercedCount, creature.id))
    return continueCollisions;

  if (weapon->piercedCount < weapon->piercedCreatureIds.count) {
    weapon->piercedCreatureIds[weapon->piercedCount++] = creature.id;
    TryApplyDamage(creatureIndex, fb->damage(), weapon->targetDir, fb->impulse(), 0);
  }
  return continueCollisions;
}

Vector2 GetWeaponPos(const Weapon& weapon) {  ///
  const auto fb = glib->weapons()->Get(weapon.type);

  const auto basePos = PLAYER_CREATURE.pos + weapon.offset;
  if (fb->projectile_type() || !weapon.startedShootingAt.IsSet())
    return basePos;

  const auto e           = weapon.startedShootingAt.Elapsed();
  const auto shootingDur = lframe::MakeScaled(fb->shooting_duration_frames());
  auto       p           = e.Progress(shootingDur);
  if (p > 0.5)
    p = 2.0f - p * 2;
  else
    p *= 2;
  const auto texId     = fb->texture_ids()->Get(0);
  const auto fbTexture = glib->atlas_textures()->Get(texId);
  const auto colliderSize
    = (f32)fbTexture->size_x() * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;
  const auto movingDistance = fb->range() - colliderSize;
  const auto movedDistance  = EaseInOutQuad(p) * movingDistance;

  return basePos + weapon.targetDir * movedDistance;
}

void GameFixedUpdate() {
  // Reloading game.
  if (g.run.reload) {  ///
    RunReset();
    RunInit();
  }

  // Cheats.
  if (ge.meta.debugEnabled) {
    // F5 - add 10 coins.
    if (IsKeyPressed(SDL_SCANCODE_F5)) {  ///
      g.run.coins += 10;
    }

    // F6 - add random item.
    if (IsKeyPressed(SDL_SCANCODE_F6)) {  ///
      Item item{.type = (ItemType)((GRAND.Rand() % (int)(ItemType_COUNT - 1)) + 1)};
      *g.run.a.playerItems.Add() = item;
    }

    // F7 - show end screen.
    if (IsKeyPressed(SDL_SCANCODE_F7) && (g.run.screen == ScreenType_GAMEPLAY)) {  ///
      g.run.scheduledUI  = true;
      g.run.scheduledEnd = true;
    }

    // F8 - add level.
    if (IsKeyPressed(SDL_SCANCODE_F8) && (g.run.screen == ScreenType_GAMEPLAY)) {  ///
      g.run.nextLevelXp *= 2;
      g.run.xpLevel++;
    }

    // F9 - add crate.
    if (IsKeyPressed(SDL_SCANCODE_F9) && (g.run.screen == ScreenType_GAMEPLAY)) {  ///
      g.run.crates++;
    }
  }

  // Applying UI settings.
  if (g.run.scheduledUI) {
    g.run.scheduledUI = false;

    ge.settings.screenFade = glib->ui_modal_fade();
    SDL_ShowCursor();

    if (g.run.crates)
      g.run.scheduledPickedUpItems = true;
    else
      g.run.scheduledUpgrades = true;
  }

  // Advancing to picked up items.
  if (g.run.scheduledPickedUpItems) {  ///
    g.run.scheduledPickedUpItems = false;

    g.run.screen                      = ScreenType_PICKED_UP_ITEM;
    g.run.pickedUpItem.toPick         = GenerateRandomItem();
    g.run.pickedUpItem.recyclingPrice = GenerateItemPrice();
  }

  // Advancing to upgrades.
  if (g.run.scheduledUpgrades) {  ///
    g.run.scheduledUpgrades = false;

    g.run.screen = ScreenType_UPGRADES;

    RefillUpgradesToPick();

    g.run.upgrades.rerolledTimes = 0;
    g.run.upgrades.rerollPrice   = GetRerollPrice(g.run.waveIndex, 0);

    if (g.run.xpLevel == g.run.previousLevel)
      g.run.scheduledShop = true;
  }

  // Advancing to shop.
  if (g.run.scheduledShop) {  ///
    g.run.scheduledShop = false;
    g.run.screen        = ScreenType_SHOP;

    g.run.shop.rerolledTimes = 0;
    g.run.shop.rerollPrice   = GetRerollPrice(g.run.waveIndex, 0);
    RefillShopToPick();
  }

  // Advancing to end screen.
  if (g.run.scheduledEnd) {  ///
    g.run.scheduledEnd = false;
    g.run.screen       = ScreenType_END;
  }

  // Advancing to the next wave.
  if (g.run.scheduledNextWave) {  ///
    g.run.scheduledNextWave = false;

    const auto health         = (f32)g.run.playerStats[StatType_HP];
    PLAYER_CREATURE.health    = health;
    PLAYER_CREATURE.maxHealth = health;

    g.run.screen           = ScreenType_GAMEPLAY;
    ge.settings.screenFade = 0;
    SDL_HideCursor();

    IncrementSetZeroOn(&g.run.waveIndex, (int)glib->waves()->size());
    g.run.waveStartedAt = {};
    g.run.waveStartedAt.SetNow();

    for (int i = 1; i < g.run.a.creatures.count; i++) {
      auto& creature = g.run.a.creatures[i];
      if (creature.active && !creature.diedAt.IsSet())
        DestroyBody(&creature.body);
    }
    g.run.a.creatures.count = 1;
    g.run.a.creatureSpawns.Reset();
    g.run.a.projectiles.Reset();
    g.run.a.projectilesToRemove.Reset();
    g.run.a.bodyShapes.Reset();
    g.run.a.justDamagedCreatures.Reset();
  }

  // Updating gameplay.
  if (g.run.screen == ScreenType_GAMEPLAY) {
    // Finishing wave opens upgrades screen.
    if (g.run.waveStartedAt.Elapsed() >= GetWaveDuration(g.run.waveIndex)) {  ///
      g.run.scheduledUI = true;
      if (g.run.waveIndex >= TOTAL_WAVES - 1) {
        g.run.scheduledUI  = true;
        g.run.scheduledEnd = true;
        g.run.won          = true;
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
          GetStatRegenPerFrame(g.run.playerStats[StatType_REGEN])
        );
      }
    }

    // Updating AI.
    for (int i = 1; i < g.run.a.creatures.count; i++) {  ///
      auto& creature = g.run.a.creatures[i];
      creature.controller.move
        = Vector2DirectionOrZero(creature.pos, PLAYER_CREATURE.pos);
    }

    // Creatures moving.
    {  ///
      const auto fb_creatures = glib->creatures();

      for (auto& creature : g.run.a.creatures) {
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
    b2World_Step(g.run.world, FIXED_DT, 4);

    // Updating body positions.
    for (auto& creature : g.run.a.creatures) {  ///
      if (!creature.active || creature.diedAt.IsSet())
        continue;

      creature.pos = ToVector2(b2Body_GetPosition(creature.body.id));
    }

    // Making pre spawns.
    if (g.run.waveStartedAt.Elapsed().value % (4 * FIXED_FPS) == 0) {  ///
      const auto wave = glib->waves()->Get(g.run.waveIndex);

      Vector2 posToSpawn{};

      FOR_RANGE (int, i, g.run.toSpawn) {
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
          *g.run.a.creatureSpawns.Add() = spawn;
        }
        else
          INVALID_PATH;
      }

      if (g.run.toSpawn < 5)
        g.run.toSpawn++;
    }

    // Spawning.
    {  ///
      const int total = g.run.a.creatureSpawns.count;
      int       off   = 0;
      FOR_RANGE (int, i, total) {
        auto& v = g.run.a.creatureSpawns[i - off];
        if (v.createdAt.IsSet() && (v.createdAt.Elapsed() >= SPAWN_FRAMES)) {
          MakeCreature({
            .type = v.type,
            .pos  = v.pos,
          });
          g.run.a.creatureSpawns.UnstableRemoveAt(i - off);
          off++;
        }
      }
    }

    // Player weapons shooting.
    if (PLAYER_CREATURE.active && !PLAYER_CREATURE.diedAt.IsSet()) {  ///
      for (auto& weapon : g.run.playerWeapons) {
        if (!weapon.type)
          continue;

        const auto fb  = glib->weapons()->Get(weapon.type);
        const auto pos = PLAYER_CREATURE.pos + weapon.offset;

        f32 minDistSqr           = f32_inf;
        int closestCreatureIndex = -1;

        // Resetting cooldown.
        const auto cooldownDur = lframe::MakeScaled(fb->cooldown_frames());
        if (weapon.cooldownStartedAt.IsSet()
            && (weapon.cooldownStartedAt.Elapsed() >= cooldownDur))
          weapon.cooldownStartedAt = {};

        if (!weapon.cooldownStartedAt.IsSet()) {
          int creatureIndex = -1;
          for (const auto& creature : g.run.a.creatures) {
            creatureIndex++;

            if (!creature.active            //
                || creature.diedAt.IsSet()  //
                || (creature.type == CreatureType_PLAYER))
              continue;

            const auto distSqr = Vector2DistanceSqr(pos, creature.pos);

            if (distSqr < minDistSqr) {
              minDistSqr           = distSqr;
              closestCreatureIndex = creatureIndex;
            }
          }

          bool targetSet = false;

          if (closestCreatureIndex >= 0) {
            const auto& closestCreature = g.run.a.creatures[closestCreatureIndex];
            if (minDistSqr < SQR(fb->range())) {
              const auto dir = Vector2DirectionOrRandom(pos, closestCreature.pos);

              // Only ranged weapons continue tracking target
              // in the middle of shooting.
              if (fb->projectile_type())
                weapon.targetDir = dir;

              if (!weapon.startedShootingAt.IsSet()) {
                weapon.targetDir   = dir;
                weapon.detachedPos = GetWeaponPos(weapon);
                weapon.startedShootingAt.SetNow();
              }
              targetSet = true;
            }
          }

          if (!targetSet && !weapon.startedShootingAt.IsSet())
            weapon.targetDir = {};
        }

        if (weapon.startedShootingAt.IsSet()) {
          const auto projectileType = (ProjectileType)fb->projectile_type();
          const auto e              = weapon.startedShootingAt.Elapsed();
          const auto shootingDur    = lframe::MakeScaled(fb->shooting_duration_frames());

          const auto projectileSpawnFrames = fb->projectile_spawn_frames();

          if (projectileType) {
            // It's a ranged weapon that spawns projectiles.
            ASSERT(projectileSpawnFrames);

            bool spawn = false;
            for (auto value : *projectileSpawnFrames) {
              if (value == e.value) {
                spawn = true;
                break;
              }
            }

            if (spawn) {
              MakeProjectile({
                .type               = projectileType,
                .ownerCreatureIndex = 0,
                .pos                = pos,
                .dir                = weapon.targetDir,
                .range              = fb->range(),
                .damage             = fb->damage(),
              });
            }
          }
          else {
            // It's a melee weapon that gets "shot" itself.
            ASSERT(!projectileSpawnFrames);

            auto p = e.Progress(shootingDur);

            const auto colliderActiveStart = 0.25f;
            const auto colliderActiveEnd   = 0.5f;

            const auto texId     = fb->texture_ids()->Get(0);
            const auto fbTexture = glib->atlas_textures()->Get(texId);
            const auto    colliderSize = Vector2{
              // TODO: update calculation of X upon starting
              // to target 4K / 0.25K resolutions.
              (f32)fbTexture->size_x(),
              // NOTE:
              // Y is divided by 2 because it's grabbed from a texture that targets 4K.
              // X already is divided by 2 when we use atlas_d2 (atlas divided by 2).
              (f32)fb->melee_collider_height_px() / 2.0f,
            } * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;

            if ((colliderActiveStart <= p) && (p <= colliderActiveEnd)) {
              CheckCollisionsRect(
                ShapeCategory_STATIC,
                (u32)ShapeCategory_CREATURE,
                GetWeaponPos(weapon),
                colliderSize,
                weapon.targetDir,
                OnWeaponCollided,
                &weapon
              );
            }
          }

          if (e >= shootingDur) {
            weapon.startedShootingAt = {};
            weapon.piercedCount      = 0;
            weapon.cooldownStartedAt.SetNow();
          }
        }
      }
    }

    // Mobs contact-damage player.
    if (PLAYER_CREATURE.active && !PLAYER_CREATURE.diedAt.IsSet()) {  ///
      const auto fb_creatures = glib->creatures();

      const auto playerPos = PLAYER_CREATURE.pos;
      for (int i = 1; i < g.run.a.creatures.count; i++) {
        const auto& creature = g.run.a.creatures[i];
        if (!creature.active || creature.diedAt.IsSet())
          continue;

        const auto fb = fb_creatures->Get(creature.type);
        if (fb->hostility_type() != HostilityType_MOB)
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
    // - Movement.
    // - Marking to remove because of travel distance.
    // - Mob collisions.
    // - Marking to remove because of pierce count.
    {  ///
      const auto fb_projectiles = glib->projectiles();

      int projectileIndex = -1;
      for (auto& projectile : g.run.a.projectiles) {
        projectileIndex++;

        const auto fb       = fb_projectiles->Get(projectile.type);
        const auto distance = FIXED_DT * fb->speed();
        projectile.travelledDistance += distance;
        projectile.pos += projectile.dir * distance;

        if (projectile.travelledDistance >= projectile.range) {
          if (!g.run.a.projectilesToRemove.Contains(projectileIndex))
            *g.run.a.projectilesToRemove.Add() = projectileIndex;
        }

        int start = 0;
        int end   = g.run.a.creatures.count;

        if (g.run.a.creatures[projectile.ownerCreatureIndex].type == CreatureType_PLAYER)
          start = 1;
        else
          end = 1;

        for (int i = start; i < end; i++) {
          auto& creature = g.run.a.creatures[i];
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
              if (g.run.a.creatures[projectile.ownerCreatureIndex].type
                  == CreatureType_PLAYER)
                maxPierce += g.run.pierceCount;

              if (projectile.piercedCount < maxPierce)
                projectile.piercedCreatureIds[projectile.piercedCount++] = creature.id;
              else if (!g.run.a.projectilesToRemove.Contains(projectileIndex))
                *g.run.a.projectilesToRemove.Add() = projectileIndex;
            }
          }
        }
      }
    }

    // Processing `projectilesToRemove`.
    if (g.run.a.projectilesToRemove.count) {  ///
      qsort(
        (void*)g.run.a.projectilesToRemove.base,
        g.run.a.projectilesToRemove.count,
        sizeof(*g.run.a.projectilesToRemove.base),
        (int (*)(const void*, const void*))IntCmp
      );
      FOR_RANGE (int, i, g.run.a.projectilesToRemove.count) {
        const auto projectileIndex
          = g.run.a.projectilesToRemove[g.run.a.projectilesToRemove.count - i - 1];
        g.run.a.projectiles.UnstableRemoveAt(projectileIndex);
      }
      g.run.a.projectilesToRemove.Reset();
    }

    // Processing `justDamagedCreatures`.
    {  ///
      // auto playerHurt = false;
      // auto mobHurt    = false;

      for (const auto index : g.run.a.justDamagedCreatures) {
        auto& creature = g.run.a.creatures[index];
        ASSERT(creature.active);

        // if (index == 0)
        //   playerHurt = true;
        // else
        //   mobHurt = true;

        if (creature.health <= 0) {
          DestroyBody(&creature.body);

          if (!index) {
            g.run.scheduledUI  = true;
            g.run.scheduledEnd = true;
          }
          else
            creature.diedAt.SetNow();

          if (!index)
            g.run.playerDiedAt.SetNow();

          Pickupable pickupable{
            .type = PickupableType_COIN,
            .pos  = creature.pos,
          };

          // Trees drop consumables or crates.
          if (creature.type == CreatureType_TREE) {
            pickupable.type = PickupableType_CONSUMABLE;

            if (GRAND.FRand() <= TREE_DROP_CRATE_FACTOR)
              pickupable.type = PickupableType_CRATE;
          }

          pickupable.createdAt.SetNow();
          *g.run.a.pickupables.Add() = pickupable;
        }
      }

      // if (playerHurt)
      //   PlaySound(Sound_GAME_PLAYER_HURT);
      // if (mobHurt)
      //   PlaySound(Sound_GAME_HURT);

      g.run.a.justDamagedCreatures.Reset();
    }

    // Picking up pickupables.
    if (PLAYER_CREATURE.active && !PLAYER_CREATURE.diedAt.IsSet()) {  ///
      for (auto& pickupable : g.run.a.pickupables) {
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
              g.run.coins++;

              g.run.xp++;
              if (g.run.xp >= g.run.nextLevelXp) {
                g.run.nextLevelXp *= 2;
                g.run.xp = 0;
                g.run.xpLevel++;
              }
            } break;

            case PickupableType_CONSUMABLE: {
              if (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth) {
                PLAYER_CREATURE.health
                  = MoveTowards(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth, 1);
              }
            } break;

            case PickupableType_CRATE: {
              g.run.crates++;
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

  // Unactivating old died creatures.
  for (auto& creature : g.run.a.creatures) {  ///
    if (creature.active                       //
        && creature.diedAt.IsSet()            //
        && (creature.diedAt.Elapsed() >= DIE_FRAMES))
      creature.active = false;
  }

  // Removing old damage numbers.
  {  ///
    int removed = 0;
    int left    = -1;
    FOR_RANGE (int, i, g.run.a.numbers.count) {
      const auto& number = g.run.a.numbers[i];
      if (number.createdAt.Elapsed() >= DAMAGE_NUMBERS_FRAMES) {
        if (left == -1)
          left = i;
        removed++;
      }
      else if (left >= 0)
        g.run.a.numbers[left++] = number;
    }
    g.run.a.numbers.count -= removed;
  }

  // Removing old picked up pickupables.
  {  ///
    const auto total = g.run.a.pickupables.count;
    int        off   = 0;
    FOR_RANGE (int, i, total) {
      const auto& pickupable = g.run.a.pickupables[i - off];
      if (pickupable.pickedUpAt.IsSet()
          && (pickupable.pickedUpAt.Elapsed() >= PICKUPABLE_FADE_FRAMES))
      {
        g.run.a.pickupables.UnstableRemoveAt(i - off);
        off++;
      }
    }
  }

  g.run.camera.pos = GetCameraTargetPos();
  g.meta.frame++;
}

void GameDraw() {
  TEMP_USAGE(&g.meta.trashArena);

  const auto fb_hostilities = glib->hostilities();
  const auto fb_creatures   = glib->creatures();
  const auto fb_weapons     = glib->weapons();
  const auto fb_projectiles = glib->projectiles();
  const auto fb_pickupables = glib->pickupables();
  const auto fb_numbers     = glib->numbers();

  BeginMode2D(&g.run.camera);

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

    for (auto& spawn : g.run.a.creatureSpawns) {
      const auto fb_creature  = fb_creatures->Get(spawn.type);
      const auto fb_hostility = fb_hostilities->Get(fb_creature->hostility_type());
      RenderGroup_CommandTexture({
        .texId = texId,
        .pos   = spawn.pos,
        .color = ColorFromRGB(fb_hostility->color()),
      });
    }

    RenderGroup_End();
  }

  // Drawing creatures.
  {  ///
    for (const auto& creature : g.run.a.creatures) {
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
        // Drawing player's weapons.
        FOR_RANGE (int, i, PLAYER_WEAPONS_COUNT) {
          const auto& weapon = g.run.playerWeapons[i];
          if (!weapon.type)
            continue;

          const auto fb = fb_weapons->Get(weapon.type);

          f32     rotation = 0;
          Vector2 scale{0, 1};

          if (weapon.targetDir == Vector2Zero())
            scale.x = (creature.dir.x >= 0 ? 1 : -1);
          else {
            scale.x  = (weapon.targetDir.x >= 0 ? 1 : -1);
            rotation = Vector2Angle(weapon.targetDir) + ((scale.x < 0) ? (f32)PI : 0.0f);
          }

          RenderGroup_OneShotTexture(
            {
              .texId    = fb->texture_ids()->Get(0),
              .rotation = rotation,
              .pos      = GetWeaponPos(weapon),
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
    for (const auto& projectile : g.run.a.projectiles) {
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

    for (const auto& number : g.run.a.numbers) {
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
    for (const auto& pickupable : g.run.a.pickupables) {
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
    for (auto& shape : g.run.a.bodyShapes) {
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
    DebugText("F3 change localization");
    DebugText("F4 change device");
    DebugText("F5 +10 coins");
    DebugText("F6 add item");
    if (g.run.screen == ScreenType_GAMEPLAY) {
      DebugText("F7 show end screen");
      DebugText("F8 add level");
      DebugText("F9 add crate");
    }

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

#define X(type_, name_) DebugText("g.run.a." #name_ ".count: %d", g.run.a.name_.count);
    VECTORS_TABLE;
#undef X
  }
}

//
