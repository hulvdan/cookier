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
#include "flatbuffers/bf_save_generated.h"

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

#define BF_CLAY_PADDING_ALL(v)             \
  .padding {                               \
    (u16)(v), (u16)(v), (u16)(v), (u16)(v) \
  }
#define BF_CLAY_PADDING_HORIZONTAL_VERTICAL(h, v) \
  .padding {                                      \
    (u16)(h), (u16)(h), (u16)(v), (u16)(v)        \
  }
#define BF_CLAY_PADDING_HORIZONTAL(v) \
  .padding {                          \
    (u16)(v), (u16)(v), 0, 0          \
  }
#define BF_CLAY_PADDING_VERTICAL(v) \
  .padding {                        \
    0, 0, (u16)(v), (u16)(v)        \
  }
#define BF_CLAY_PADDING_LEFT(v) \
  .padding {                    \
    (u16)(v), 0, 0, 0           \
  }
#define BF_CLAY_PADDING_RIGHT(v) \
  .padding {                     \
    0, (u16)(v), 0, 0            \
  }
#define BF_CLAY_PADDING_TOP(v) \
  .padding {                   \
    0, 0, (u16)(v), 0          \
  }
#define BF_CLAY_PADDING_BOTTOM(v) \
  .padding {                      \
    0, 0, 0, (u16)(v)             \
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

#define BF_CLAY_CUSTOM_NINE_SLICE(gamelibNineSlicePtr_, color_, flash_) \
  .custom {                                                             \
    .customData = PushClayCustomData({                                  \
      .type           = ClayCustomElementType_NINE_SLICE,               \
      .nineSlice      = (gamelibNineSlicePtr_),                         \
      .nineSliceColor = (color_),                                       \
      .nineSliceFlash = (flash_),                                       \
    }),                                                                 \
  }

#define BF_CLAY_CUSTOM_OVERLAY(color)                \
  .custom {                                          \
    .customData = PushClayCustomData({               \
      .type         = ClayCustomElementType_OVERLAY, \
      .overlayColor = (color),                       \
    }),                                              \
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

#define BEAUTIFY_WIGGLING_DANGER_SCOPED(value_, amplitude_, frames_, times_)         \
  f32 _wigglingP = 0;                                                                \
  if ((value_).IsSet())                                                              \
    _wigglingP = (value_).Elapsed().Progress(frames_);                               \
  if ((_wigglingP >= 1) && !draw)                                                    \
    value_ = {};                                                                     \
  if (_wigglingP >= 1)                                                               \
    _wigglingP = 0;                                                                  \
  Beautify b{.translate{(amplitude_) * sinf((f32)(times_) * PI32 * _wigglingP), 0}}; \
  BEAUTIFY(b);

struct ClayImageData {
  int     texId         = {};
  Margins sourceMargins = {0, 0};
  Color   color         = WHITE;
  Color   flash         = TRANSPARENT_BLACK;
  // f32   scale     = {};
  // ImageFitType fitType   = {};
};

enum ClayCustomElementType : u16 {
  ClayCustomElementType_INVALID,
  ClayCustomElementType_BEAUTIFIER_START,
  ClayCustomElementType_BEAUTIFIER_END,
  ClayCustomElementType_NINE_SLICE,
  ClayCustomElementType_OVERLAY,
};

struct ClayCustomData {
  ClayCustomElementType    type           = {};
  u16                      alpha          = u16_max;
  Vector2                  translate      = {0, 0};
  Vector2                  scale          = {1, 1};
  const BFGame::NineSlice* nineSlice      = nullptr;
  Color                    nineSliceColor = WHITE;
  Color                    nineSliceFlash = TRANSPARENT_BLACK;
  Color                    overlayColor   = MAGENTA;
};

// ============================================================ }

#include "bf_constants.cpp"

b2Vec2 ToB2Vec2(Vector2 value) {  ///
  return {value.x, value.y};
}

Clay_Vector2 ToClayVector2(Vector2 value) {  ///
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
    return Vector2Rotate({1, 0}, 2 * PI32 * GRAND.FRand());
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

enum ShapeCategory : u32 {
  ShapeCategory_STATIC     = 1 << 0,
  ShapeCategory_PLAYER     = 1 << 1,
  ShapeCategory_CREATURE   = 1 << 2,
  ShapeCategory_PROJECTILE = 1 << 3,
};

enum BodyType : u32 {
  BodyType_INVALID,
  BodyType_STATIC,
  BodyType_CREATURE,
};

enum ShapeUserDataType : u32 {
  ShapeUserDataType_INVALID,
  ShapeUserDataType_STATIC,
  ShapeUserDataType_CREATURE,
};

struct ShapeUserData {
  ShapeUserDataType type   = {};
  int               _value = {};

  static ShapeUserData Static() {  ///
    return {.type = ShapeUserDataType_STATIC};
  }

  static ShapeUserData Creature(int value) {  ///
    ASSERT(value >= 0);
    return {
      .type   = ShapeUserDataType_CREATURE,
      ._value = value,
    };
  }

  int GetCreatureId() const {  ///
    ASSERT(type == ShapeUserDataType_CREATURE);
    return _value;
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
      u8   value[4]{};

      type[0]  = p[0];
      value[0] = p[1];
      value[1] = p[2];
      value[2] = p[3];

      return {
        .type   = *(ShapeUserDataType*)type,
        ._value = *(int*)value,
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
      value[1] = ((u8*)&_value)[0];
      value[2] = ((u8*)&_value)[1];
      value[3] = ((u8*)&_value)[2];

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
  WeaponType type                               = {};
  Vector2    targetDir                          = {};
  FrameGame  startedShootingAt                  = {};
  FrameGame  cooldownStartedAt                  = {};
  int        tier                               = {};
  int        calculatedDamage                   = 0;
  int        thisWaveDamage                     = 0;
  f32        lastCollisionCheckShootingProgress = 0;

  Array<int, WEAPON_MAX_PIERCE> piercedCreatureIds = {};
  int                           piercedCount       = 0;

  int killedEnemies = 0;
};

struct Item {
  ItemType type  = {};
  int      count = {};
};

struct CreatureController {
  Vector2 move = {};
};

struct Ailment {
  CreatureType ownerCreatureType   = {};
  int          weaponIndexOrMinus1 = {};

  int       spread    = {};
  FrameGame startedAt = {};
  lframe    duration  = {};

  int value = {};
};

struct ApplyAilmentData {
  AilmentType type   = AilmentType_INVALID;
  int         value  = 0;
  int         spread = {};
};

struct Creature {
  int                id                             = {};
  CreatureType       type                           = {};
  int                health                         = {};
  int                maxHealth                      = {};
  Vector2            pos                            = {};
  Vector2            dir                            = {};
  Body               body                           = {};
  CreatureController controller                     = {};
  FrameGame          lastDamagedAt                  = {};
  FrameGame          lastDamagedFlashAt             = {};
  FrameGame          diedAt                         = {};
  f32                speed                          = {};
  f32                speedModifier                  = 1;
  f32                movementAccumulator            = {};
  FrameGame          idleStartedAt                  = {};
  bool               killedBecauseOfTheEndOfTheWave = false;
  bool               aggroed                        = false;

  // WARN: Note -1! To get burning ailment,
  // index into it using `AilmentType_BURN - 1`.
  Array<Ailment, AilmentType_COUNT - 1> ailments = {};

  int lastDamagedWeaponIndex = -1;

  union {
    struct {
      FrameGame startedShootingAt;
      lframe    cooldown;
      Vector2   aimDirection;
    } turrel;

    struct {
      FrameGame startedShootingAt;
    } ranger;

    struct {
      FrameGame startedRushingAt;
      FrameGame finishedRushingAt;
      lframe    cooldown;
      Vector2   rushingDir;
    } rusher;
  } _mob;

  auto& DataTurrel() {  ///
    ASSERT(type == CreatureType_TURREL);
    return _mob.turrel;
  };

  const auto& DataTurrel() const {  ///
    ASSERT(type == CreatureType_TURREL);
    return _mob.turrel;
  };

  auto& DataRanger() {  ///
    ASSERT(type == CreatureType_RANGER);
    return _mob.ranger;
  };

  const auto& DataRanger() const {  ///
    ASSERT(type == CreatureType_RANGER);
    return _mob.ranger;
  };

  auto& DataRusher() {  ///
    ASSERT(type == CreatureType_RUSHER);
    return _mob.rusher;
  };

  const auto& DataRusher() const {  ///
    ASSERT(type == CreatureType_RUSHER);
    return _mob.rusher;
  };
};

struct MakeCreatureData {
  CreatureType type = {};
  Vector2      pos  = {};
};

struct PreSpawn {
  PreSpawnType type         = {};
  CreatureType typeCreature = {};
  Vector2      pos          = {};
  FrameGame    createdAt    = {};
};

struct Projectile {
  ProjectileType                    type                 = {};
  CreatureType                      ownerCreatureType    = {};
  int                               weaponIndexOrMinus1  = -1;
  Vector2                           pos                  = {};
  Vector2                           dir                  = {};
  int                               damage               = {};
  f32                               critDamageMultiplier = {};
  int                               pierce               = {};
  int                               bounce               = {};
  FrameGame                         createdAt            = {};
  Array<int, PROJECTILE_MAX_PIERCE> damagedCreatureIds   = {};
  int                               damagedCount         = {};
  int                               piercedCount         = 0;
  int                               bouncedCount         = 0;
  f32                               knockbackMeters      = {};
  f32                               range                = {};
  f32                               travelledDistance    = 0;
};

struct MakeProjectileData {
  ProjectileType type                 = {};
  CreatureType   ownerCreatureType    = {};
  int            weaponIndexOrMinus1  = -1;
  Vector2        pos                  = {};
  Vector2        dir                  = {};
  f32            range                = {};
  int            damage               = {};
  f32            critDamageMultiplier = {};
  f32            knockbackMeters      = {};
  int            pierce               = {};
  int            bounce               = {};
};

struct Number {
  NumberType type      = {};
  int        value     = {};
  Vector2    pos       = {};
  FrameGame  createdAt = {};
};

struct MakePickupableData {
  PickupableType type        = {};
  Vector2        pos         = {};
  int            coin_amount = {};
};

struct Pickupable {
  PickupableType type            = {};
  Vector2        pos             = {};
  FrameGame      createdAt       = {};
  FrameGame      pickedUpAt      = {};
  FrameVisual    startedFlyingAt = {};

  struct {
    struct {
      int amount;
    } coin;
  } _u;

  auto& DataCoin() {  ///
    ASSERT(type == PickupableType_COIN);
    return _u.coin;
  }

  const auto& DataCoin() const {  ///
    ASSERT(type == PickupableType_COIN);
    return _u.coin;
  }
};

struct MakeNumberData {
  NumberType type  = {};
  int        value = {};
  Vector2    pos   = {};
};

lframe GetWaveDuration(int waveIndex) {  ///
  constexpr int durations_[]{20, 25, 30, 35, 40, 45, 50, 55, 60, 60,
                             60, 60, 60, 60, 60, 60, 60, 60, 60, 90};
  VIEW_FROM_ARRAY_DANGER(durations);
  int seconds = durations[MIN(durations.count - 1, waveIndex)];
#if BF_DEBUG
  seconds /= 3;
#endif
  return lframe::Unscaled(seconds * FIXED_FPS);
}

enum ScreenType {
  ScreenType_NEW_RUN,
  ScreenType_GAMEPLAY,
  ScreenType_WAVE_END_ANIMATION,
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

struct Rerolls {
  int rerolledFreeTimes = 0;
  int rerolledTimes     = 0;

  void Roll();
  int  GetPrice() const;
};

struct ThisWaveMob {
  CreatureType type              = {};
  f32          accumulatedFactor = {};
};

struct Placeholder {
  PlaceholderType type        = {};
  const char*     placeholder = {};

  union {
    struct {
      const char* value;
      Color       color;
    } string;

    int brokenLocale_value;
  } _u;

  const auto& string() const {  ///
    ASSERT(type == PlaceholderType_STRING);
    return _u.string;
  }

  int brokenLocale_value() const {  ///
    ASSERT(type == PlaceholderType_BROKEN_LOCALE);
    return _u.brokenLocale_value;
  }
};

struct Particle {
  ParticleType type      = {};
  Vector2      pos       = {};
  f32          scale     = 1;
  FrameGame    createdAt = {};
};

int ParticleCmp(const Particle* v1, const Particle* v2) {  ///
  if (v1->createdAt._value > v2->createdAt._value)
    return 1;
  if (v1->createdAt._value < v2->createdAt._value)
    return -1;
  if (v1->pos.y > v2->pos.y)
    return -1;
  if (v1->pos.y < v2->pos.y)
    return 1;
  return 0;
}

struct Landmine {
  Vector2   pos                 = {};
  FrameGame startedDetonationAt = {};
};

struct Garden {
  Vector2   pos       = {};
  FrameGame createdAt = {};
};

struct UIButtonState {
  Clay_ElementId id               = {};
  bool           thisFrameUpdated = false;
  bool           hovered          = false;
};

struct Build {
  DifficultyType maxDifficultyBeaten = {};
};

struct Achievement {
  i64 value = {};
};

struct JustUnlockedAchievement {
  AchievementType type      = {};
  int             stepIndex = {};
  FrameVisual     shownAt   = {};
};

struct GameData {
  struct Meta {
    Arena trashArena = {};

    // NOTE: Reorder loading upon reordering fonts.
    Font fontUI                 = {};
    Font fontUIOutlined         = {};
    Font fontUIBig              = {};
    Font fontStats              = {};
    Font fontPrices             = {};
    Font fontItemCountsOutlined = {};
    Font fontWaveCompletion     = {};

    LoadFontsResult loadedFonts = {};

    bool godMode = false;

    struct Touch {
      Array<Vector2, 4> pos      = {};  // lstarted, ltarget, rstarted, rtarget.
      Array<Vector2, 2> dir      = {};
      Array<TouchID, 2> touchIDs = {};
      // FrameGame      rightLastPressedAt = {};
    } touch;

    bool paused                                   = false;
    bool scheduledTogglePause                     = false;
    bool pausedShowingAchievements                = false;
    int  pausedAchievementsHoveredAchievement     = 0;
    int  pausedAchievementsHoveredAchievementStep = 0;

    bool        scheduledSave                 = false;
    bool        previousSaveIsNotCompletedYet = false;
    FrameVisual lastSaveAt                    = {};
  } meta;

  struct {
    DifficultyType difficulty = DifficultyType_D0;
    BuildType      build      = BuildType_DEFAULT;
    WeaponType     weapon     = WeaponType_SWORD;

    Array<Achievement, AchievementType_COUNT> achievements = {};
    Array<Build, BuildType_COUNT>             builds       = {};

    Array<bool, BuildType_COUNT>  lockedBuilds  = {};
    Array<bool, ItemType_COUNT>   lockedItems   = {};
    Array<bool, WeaponType_COUNT> lockedWeapons = {};

    int achievementStepsTotal     = 0;
    int achievementStepsCompleted = 0;
  } player;

  struct Run {
    struct State {
      bool       won    = false;
      ScreenType screen = ScreenType_GAMEPLAY;

      int  waveIndex = 0;
      bool waveWon   = false;

      int level         = 1;
      int previousLevel = 1;
      f32 xp            = 0;
      f32 previousXp    = 0;
      int previousCoins = 0;

      int playerKilledEnemies    = 0;
      int notPickedUpCoins       = 0;
      int notPickedUpCoinsVisual = 0;
      int crates                 = 0;
      int toSpawn                = 3;

      Array<Weapon, PLAYER_WEAPONS_COUNT> weapons           = {};
      Vector<Item>                        items             = {};
      Array<int, StatType_COUNT>          statsWithoutItems = {};

      // NOTE: Downwards goes data associated with different screens (ref: ScreenType).
      struct {
        ItemType toPick = {};
      } pickedUpItem;

      struct {
        Array<Upgrade, 4> toPick = {};
        Rerolls           rerolls;
      } upgrades;

      struct {
        Array<ShopItem, 4> toPick = {};
        Rerolls            rerolls;
      } shop;
    } state = {};

    int shopSelectedWeaponIndex = -1;

    Arena arena = {};

    bool reload = false;

    FrameVisual scheduledWaveCompleted      = {};
    bool        recalculatePlayerStats      = false;
    bool        scheduledUI                 = false;
    bool        scheduledNewRun             = false;
    bool        scheduledPickedUpItems      = false;
    bool        scheduledPickedUpItemsReset = false;
    bool        scheduledUpgrades           = false;
    bool        scheduledUpgradesReset      = false;
    bool        scheduledShop               = false;
    bool        scheduledShopReset          = false;
    bool        scheduledEnd                = false;
    bool        scheduledNextWave           = false;

    bool showingSecondaryStats = false;

    Camera camera{
      .zoom          = METER_LOGICAL_SIZE,
      .texturesScale = 1.0f / METER_LOGICAL_SIZE,
    };

    b2WorldId world          = {};
    int       nextCreatureId = 1;

    Array<ThisWaveMob, CreatureType_COUNT> thisWaveMobs      = {};
    int                                    thisWaveMobsCount = 0;

    FrameGame playerLastLifestealAt = {};
    FrameGame playerLastRegenAt     = {};
    Vector2   previousPlayerPos     = {};
    f32       playerWalkedMeters    = 0;
    int       playerIdleFrames      = 0;

    int cratesDroppedThisWave = 0;
    int turrelsToSpawn        = 0;
    int gardensToSpawn        = 0;

    FrameGame waveStartedAt = {};

    int bossCreatureId = 0;

    Array<int, StatType_COUNT> playerStats = {};

    // Using "X-macros". ref: https://www.geeksforgeeks.org/c/x-macros-in-c/
    // These containers preserve allocated memory upon resetting state of the run.
#define VECTORS_TABLE          \
  X(Creature, creatures)       \
  X(Landmine, landmines)       \
  X(Garden, gardens)           \
  X(PreSpawn, preSpawns)       \
  X(Projectile, projectiles)   \
  X(int, projectilesToRemove)  \
  X(BodyShape, bodyShapes)     \
  X(int, justDamagedCreatures) \
  X(Number, numbers)           \
  X(Pickupable, pickupables)   \
  X(Particle, particles)

#define X(type_, name_) Vector<type_> name_ = {};
    VECTORS_TABLE;
#undef X
  } run;

  struct {
    bool active        = false;
    bool addedChildren = false;
    u16  childGap      = {};
    int  currentWidth  = 0;
    int  maxWidth      = {};

    Array<Placeholder, BF_MAX_PLACEHOLDERS_IN_STRING> placeholders      = {};
    int                                               placeholdersCount = {};
  } uiFlex;

  struct UI {
    Vector2                   notPickedUpCoinsLogicalPos = {};
    const Font*               overriddenFont             = {};
    Array<UIButtonState, 128> buttonStates               = {};
    int                       buttonStatesCount          = 0;

    FrameVisual newRunErrorAt = {};

    FrameVisual upgradesErrorGold = {};

    FrameVisual shopErrorGold    = {};
    FrameVisual shopErrorWeapons = {};

    Vector<JustUnlockedAchievement> justUnlockedAchievements = {};
  } ui;
} g = {};

int GetAchievementsCompletedPercent() {  ///
  ASSERT(g.player.achievementStepsCompleted <= g.player.achievementStepsTotal);
  ASSERT(g.player.achievementStepsCompleted >= 0);
  ASSERT(g.player.achievementStepsTotal > 0);

  int percent = g.player.achievementStepsCompleted * 100 / g.player.achievementStepsTotal;
  if (g.player.achievementStepsCompleted < g.player.achievementStepsTotal)
    percent = MIN(99, percent);
  if (g.player.achievementStepsCompleted > 0)
    percent = MAX(1, percent);

  return percent;
}

void Save() {  ///
  g.meta.scheduledSave = true;
}

// NOTE: Doesn't apply `StatType_DAMAGE`.
int ApplyDamageScalings(int baseDamage, int tier, auto fb_damageScalings) {  ///
  if (fb_damageScalings) {
    for (auto scaling : *fb_damageScalings) {
      auto statValue = g.run.playerStats[scaling->stat_type()];
      auto percent   = scaling->percents_per_tier()->Get(tier);
      baseDamage += Round((f32)statValue * (f32)percent / 100.0f);
    }
  }
  return baseDamage;
}

int ApplyPlayerStatDamageMultiplier(int damage) {  ///
  auto v = (f32)(100 + g.run.playerStats[StatType_DAMAGE]) / 100.0f;
  return Round((f32)damage * v);
}

void IterateOverSpecificWeaponEffects(
  EffectConditionType              condition,
  WeaponType                       type,
  auto /* void (auto fb_effect) */ innerLambda
) {  ///
  const auto fb_effects = glib->weapons()->Get(type)->effects();
  if (fb_effects) {
    for (const auto fb_effect : *fb_effects) {
      if (fb_effect->effectcondition_type() == condition)
        innerLambda(fb_effect);
    }
  }
}

int CalculateWeaponDamage(int weaponIndexOrMinus1, WeaponType type, int tier) {  ///
  ASSERT(tier < 4);
  const auto fb = glib->weapons()->Get(type);
  ASSERT(tier >= fb->min_tier_index());

  int damage = fb->base_damage()->Get(tier - fb->min_tier_index());
  damage
    = ApplyDamageScalings(damage, tier - fb->min_tier_index(), fb->damage_scalings());
  damage = ApplyPlayerStatDamageMultiplier(damage);

  IterateOverSpecificWeaponEffects(
    EffectConditionType_MORE_OF_THE_SAME_WEAPON_MORE_PROPERTY,
    type,
    [&](auto fb_effect) BF_FORCE_INLINE_LAMBDA {
      if (fb_effect->weaponproperty_type() != WeaponPropertyType_DAMAGE)
        return;

      int sameWeapons = 0;
      int i           = -1;
      for (const auto& weapon : g.run.state.weapons) {
        i++;
        if ((weaponIndexOrMinus1 != i) && (weapon.type == type))
          sameWeapons++;
      }
      if (sameWeapons > 0) {
        auto v = fb_effect->value();
        if (v == 0)
          damage = Round(damage * fb_effect->value_multiplier() * (f32)sameWeapons);
        else
          damage += v * sameWeapons;
      }
    }
  );

  damage = MAX(1, damage);
  return damage;
}

void RecalculatePlayerWeaponDamages() {  ///
  int weaponIndexOrMinus1 = -1;
  for (auto& weapon : g.run.state.weapons) {
    weaponIndexOrMinus1++;
    weapon.thisWaveDamage = 0;
    if (weapon.type) {
      weapon.calculatedDamage
        = CalculateWeaponDamage(weaponIndexOrMinus1, weapon.type, weapon.tier);
    }
  }
}

Vector2 GetPlayerWeaponOffset(int weaponIndex) {  ///
  int weaponsCount = 0;
  for (const auto& weapon : g.run.state.weapons) {
    if (weapon.type)
      weaponsCount++;
  }

  ASSERT(weaponIndex >= 0);
  ASSERT(weaponIndex < weaponsCount);

  // Checking that INVALID weapons are on the end of `state.weapons`.
  for (int i = weaponsCount; i < g.run.state.weapons.count; i++)
    ASSERT(!g.run.state.weapons[i].type);

  const auto startingAngle = PLAYER_WEAPONS_STARTING_ANGLES[weaponsCount - 1];
  const auto angleDelta    = 2.0f * (f32)PI32 / (f32)weaponsCount;
  return Vector2Rotate(Vector2(1, 0), weaponIndex * angleDelta + startingAngle);
}

#define PLAYER_CREATURE (g.run.creatures[0])
#define PLAYER_COINS (g.run.state.statsWithoutItems[StatType_COINS])

void SanitizeCoins() {  ///
  if (PLAYER_COINS < 0)
    PLAYER_COINS = int_max;
}

void ApplyEffect(const BFGame::Effect* fb_effect, int times = 1) {  ///
  ASSERT(times > 0);
  if (fb_effect->stat_type()) {
    g.run.state.statsWithoutItems[fb_effect->stat_type()] += fb_effect->value() * times;
    if ((StatType)fb_effect->stat_type() == StatType_COINS)
      SanitizeCoins();
    if (fb_effect->value_multiplier() != 1)
      g.run.state.statsWithoutItems[fb_effect->stat_type()]
        *= 1 + (fb_effect->value_multiplier() - 1) * times;
    if ((StatType)fb_effect->stat_type() == StatType_COINS)
      SanitizeCoins();
  }

  g.run.recalculatePlayerStats = true;
}

f32 GetLuckFactor() {  ///
  return MAX(0, 1.0f + (f32)g.run.playerStats[StatType_LUCK] / 100.0f);
}

void RecalculateThisWaveMobs() {  ///
  const auto fb_creatures = glib->creatures();

  g.run.thisWaveMobsCount = 0;

  f32 accumulatedFactor = 0;

  FOR_RANGE (int, i, fb_creatures->size()) {
    const auto fb         = fb_creatures->Get(i);
    const f32  luckFactor = (fb->luck_affects_spawn_factor() ? GetLuckFactor() : 1.0f);
    const auto factor
      = fb->spawn_factor() * luckFactor
        / ArithmeticSumAverage(fb->spawn_group_count_min(), fb->spawn_group_count_max());
    ASSERT(factor >= 0);
    if ((factor > 0) && (g.run.state.waveIndex + 1 >= fb->appearing_wave_number())) {
      accumulatedFactor += factor;
      g.run.thisWaveMobs[g.run.thisWaveMobsCount++] = {
        .type              = (CreatureType)i,
        .accumulatedFactor = accumulatedFactor,
      };
    }
  }

  // Normalization of `accumulatedFactor`.
  FOR_RANGE (int, i, g.run.thisWaveMobsCount)
    g.run.thisWaveMobs[i].accumulatedFactor /= accumulatedFactor;
}

void OnWaveStarted() {  ///
  g.run.state.upgrades.rerolls = {};
  g.run.state.previousCoins    = g.run.state.statsWithoutItems[StatType_COINS];

  g.run.bossCreatureId = 0;

  g.run.waveStartedAt = {};
  g.run.waveStartedAt.SetNow();
  g.run.cratesDroppedThisWave = 0;

  RecalculateThisWaveMobs();

  g.run.turrelsToSpawn = g.run.playerStats[StatType_TURRELS_COUNT];
  g.run.gardensToSpawn = g.run.playerStats[StatType_GARDENS_COUNT];

  RecalculatePlayerWeaponDamages();
}

void AchievementStepSetLock(const BFGame::AchievementStep* fb_step, bool locked) {  ///
  if (fb_step->unlocks_build_type())
    g.player.lockedBuilds[fb_step->unlocks_build_type()] = locked;
  if (fb_step->unlocks_item_type())
    g.player.lockedItems[fb_step->unlocks_item_type()] = locked;
  if (fb_step->unlocks_weapon_type())
    g.player.lockedWeapons[fb_step->unlocks_weapon_type()] = locked;

  if (locked)
    g.player.achievementStepsCompleted--;
  else
    g.player.achievementStepsCompleted++;

  ASSERT(g.player.achievementStepsCompleted >= 0);
  ASSERT(g.player.achievementStepsCompleted <= g.player.achievementStepsTotal);
}

void OnAchievementValueChanged(AchievementType type, int oldValue, int newValue) {  ///
  auto fb_steps = glib->achievements()->Get(type)->steps();
  if (!fb_steps)
    return;

  int stepIndex = -1;
  for (auto fb_step : *fb_steps) {
    stepIndex++;
    if ((oldValue < fb_step->value()) && (fb_step->value() <= newValue)) {
      AchievementStepSetLock(fb_step, false);
      *g.ui.justUnlockedAchievements.Add() = {.type = type, .stepIndex = stepIndex};
    }
  }
}

void AchievementAdd(AchievementType type, int value) {  ///
  ASSERT(value >= 0);
  if (value < 0)
    return;

  int oldValue = g.player.achievements[type].value;
  g.player.achievements[type].value += value;
  OnAchievementValueChanged(type, oldValue, g.player.achievements[type].value);
  Save();
}

void AchievementMax(AchievementType type, int value) {  ///
  ASSERT(value >= 0);
  int oldValue = g.player.achievements[type].value;
  if (value <= oldValue)
    return;
  g.player.achievements[type].value = value;
  OnAchievementValueChanged(type, oldValue, value);
  Save();
}

void RecalculatePlayerStats() {  ///
  ZoneScoped;

  FOR_RANGE (int, i, StatType_COUNT)
    g.run.playerStats[i] = g.run.state.statsWithoutItems[i];

  for (auto& item : g.run.state.items) {
    const auto fb         = glib->items()->Get(item.type);
    const auto fb_effects = fb->effects();
    if (fb_effects) {
      for (const auto fb_effect : *fb_effects) {
        if (!fb_effect->effectcondition_type())
          g.run.playerStats[fb_effect->stat_type()] += fb_effect->value() * item.count;
      }
    }
  }

  PLAYER_CREATURE.maxHealth = g.run.playerStats[StatType_HP];
}

void Load(void* saveData) {  ///
  const auto save = BFSave::GetSave(saveData);

  auto& s         = g.run.state;
  auto  tempItems = s.items;
  tempItems.Reset();
  s = {.items = tempItems};

  ge.meta.logicRand._state = save->random_state();
  auto fb_achievements     = save->achievements();
  if (fb_achievements) {
    int i = 0;
    for (const auto& x : *fb_achievements)
      g.player.achievements[i++] = {.value = x->value()};
  }
  auto fb_builds = save->builds();
  if (fb_builds) {
    int i = 0;
    for (const auto& x : *fb_builds) {
      g.player.builds[i++]
        = {.maxDifficultyBeaten = (DifficultyType)x->max_difficulty_beaten()};
    }
  }

  g.player.difficulty      = (DifficultyType)save->difficulty();
  g.player.build           = (BuildType)save->build();
  g.player.weapon          = (WeaponType)save->weapon();
  PLAYER_CREATURE.health   = save->health();
  s.won                    = save->won();
  s.screen                 = (ScreenType)save->screen();
  s.waveIndex              = save->wave_index();
  s.waveWon                = save->wave_won();
  s.level                  = save->level();
  s.previousLevel          = save->previous_level();
  s.xp                     = save->xp();
  s.previousXp             = s.xp;
  s.playerKilledEnemies    = save->player_killed_enemies();
  s.notPickedUpCoins       = save->not_picked_up_coins();
  s.notPickedUpCoinsVisual = s.notPickedUpCoins;
  s.crates                 = save->crates();
  s.toSpawn                = save->to_spawn();

  int weaponIndex = 0;
  for (auto x : *save->weapons()) {
    s.weapons[weaponIndex++] = {
      .type          = (WeaponType)x->type(),
      .tier          = x->tier(),
      .killedEnemies = x->killed_enemies(),
    };
  }

  auto fb_items = save->items();
  if (fb_items) {
    for (auto x : *fb_items) {
      *s.items.Add() = {
        .type  = (ItemType)x->type(),
        .count = x->count(),
      };
    }
  }

  for (auto& x : s.statsWithoutItems)
    x = 0;
  int  statIndex = 0;
  auto fb_stats  = save->stats_without_items();
  if (fb_stats) {
    for (auto x : *fb_stats)
      s.statsWithoutItems[statIndex++] = x;
  }

  s.pickedUpItem.toPick = (ItemType)save->screen_picked_up_item__to_pick();

  int  upgradesIndex     = 0;
  auto fb_upgradesToPick = save->screen_upgrades__to_pick();
  if (fb_upgradesToPick) {
    for (auto x : *fb_upgradesToPick) {
      s.upgrades.toPick[upgradesIndex++] = {
        .stat = (StatType)x->stat(),
        .tier = x->tier(),
      };
    }
  }
  s.upgrades.rerolls.rerolledFreeTimes
    = save->screen_upgrades__rerolls()->rerolled_free_times();
  s.upgrades.rerolls.rerolledTimes = save->screen_upgrades__rerolls()->rerolled_times();

  int  shopIndex     = 0;
  auto fb_shopToPick = save->screen_shop__to_pick();
  if (fb_shopToPick) {
    for (auto x : *fb_shopToPick) {
      s.shop.toPick[shopIndex++] = {
        .weapon = (WeaponType)x->weapon(),
        .item   = (ItemType)x->item(),
        .tier   = x->tier(),
      };
    }
  }
  s.shop.rerolls.rerolledFreeTimes = save->screen_shop__rerolls()->rerolled_free_times();
  s.shop.rerolls.rerolledTimes     = save->screen_shop__rerolls()->rerolled_times();

  if (save->health() <= 0)
    g.run.scheduledEnd = true;

  if (s.screen == ScreenType_GAMEPLAY) {
    g.meta.paused = true;
    OnWaveStarted();
  }
  else if (s.screen == ScreenType_WAVE_END_ANIMATION) {
    g.run.scheduledWaveCompleted.SetNow();
    g.run.scheduledWaveCompleted._value -= WAVE_COMPLETED_FRAMES.value;
  }
  else if (s.screen == ScreenType_PICKED_UP_ITEM)
    g.run.scheduledPickedUpItems = true;
  else if (s.screen == ScreenType_UPGRADES)
    g.run.scheduledUpgrades = true;
  else if (s.screen == ScreenType_SHOP)
    g.run.scheduledShop = true;
  else if (s.screen == ScreenType_END)
    g.run.scheduledEnd = true;

  RecalculatePlayerStats();
  RecalculatePlayerWeaponDamages();

  g.player.lockedBuilds  = {};
  g.player.lockedWeapons = {};
  g.player.lockedItems   = {};

  // Recalculating unlocked builds, items and weapons based off achievements.
  {
    g.player.achievementStepsTotal     = 0;
    g.player.achievementStepsCompleted = 0;

    int i = -1;
    for (const auto& x : g.player.achievements) {  ///
      i++;
      auto fb       = glib->achievements()->Get(i);
      auto fb_steps = fb->steps();
      if (!fb_steps)
        continue;

      g.player.achievementStepsTotal += fb_steps->size();
      g.player.achievementStepsCompleted += fb_steps->size();

      for (auto fb_step : *fb_steps) {
        if (fb_step->value() > x.value)
          AchievementStepSetLock(fb_step, true);
      }
    }

    ASSERT(g.player.achievementStepsCompleted >= 0);
    ASSERT(g.player.achievementStepsCompleted <= g.player.achievementStepsTotal);
  }
}

flatbuffers::FlatBufferBuilder DumpState() {  ///
  BFSave::SaveT fb_save{};

  {
    const auto& s = g.run.state;

    for (const auto& x : g.player.achievements) {
      fb_save.achievements.push_back(
        std::make_unique<BFSave::AchievementT>(BFSave::AchievementT{.value = x.value})
      );
    }
    for (const auto& x : g.player.builds) {
      fb_save.builds.push_back(std::make_unique<BFSave::BuildT>(
        BFSave::BuildT{.max_difficulty_beaten = x.maxDifficultyBeaten}
      ));
    }

    fb_save.difficulty = (int)g.player.difficulty;
    fb_save.build      = (int)g.player.build;
    fb_save.weapon     = (int)g.player.weapon;

    fb_save.random_state          = ge.meta.logicRand._state;
    fb_save.health                = PLAYER_CREATURE.health;
    fb_save.won                   = s.won;
    fb_save.screen                = s.screen;
    fb_save.wave_index            = s.waveIndex;
    fb_save.wave_won              = s.waveWon;
    fb_save.level                 = s.level;
    fb_save.previous_level        = s.previousLevel;
    fb_save.xp                    = s.xp;
    fb_save.player_killed_enemies = s.playerKilledEnemies;
    fb_save.not_picked_up_coins   = s.notPickedUpCoins;
    fb_save.crates                = s.crates;
    fb_save.to_spawn              = s.toSpawn;

    for (const auto& weapon : g.run.state.weapons) {
      fb_save.weapons.push_back(std::make_unique<BFSave::WeaponT>(BFSave::WeaponT{
        .type           = weapon.type,
        .tier           = weapon.tier,
        .killed_enemies = weapon.killedEnemies,
      }));
    }

    for (const auto& item : g.run.state.items) {
      fb_save.items.push_back(std::make_unique<BFSave::ItemT>(BFSave::ItemT{
        .type  = item.type,
        .count = item.count,
      }));
    }

    for (const auto& x : g.run.state.statsWithoutItems)
      fb_save.stats_without_items.push_back(x);

    fb_save.screen_picked_up_item__to_pick = s.pickedUpItem.toPick;

    for (const auto& x : g.run.state.upgrades.toPick) {
      fb_save.screen_upgrades__to_pick.push_back(
        std::make_unique<BFSave::UpgradeT>(BFSave::UpgradeT{
          .stat = (i32)x.stat,
          .tier = x.tier,
        })
      );
    }
    fb_save.screen_upgrades__rerolls
      = std::make_unique<BFSave::RerollsT>(BFSave::RerollsT{
        .rerolled_free_times = s.upgrades.rerolls.rerolledFreeTimes,
        .rerolled_times      = s.upgrades.rerolls.rerolledTimes,
      });

    for (const auto& x : g.run.state.shop.toPick) {
      fb_save.screen_shop__to_pick.push_back(
        std::make_unique<BFSave::ShopItemT>(BFSave::ShopItemT{
          .weapon = x.weapon,
          .item   = x.item,
          .tier   = x.tier,
        })
      );
    }
    fb_save.screen_shop__rerolls = std::make_unique<BFSave::RerollsT>(BFSave::RerollsT{
      .rerolled_free_times = s.shop.rerolls.rerolledFreeTimes,
      .rerolled_times      = s.shop.rerolls.rerolledTimes,
    });

    if (s.screen == ScreenType_GAMEPLAY) {
      fb_save.stats_without_items[StatType_COINS] = s.previousCoins;

      fb_save.level  = fb_save.previous_level;
      fb_save.xp     = s.previousXp;
      fb_save.crates = 0;
    }
  }

  flatbuffers::FlatBufferBuilder fbb{};
  auto                           packed = BFSave::Save::Pack(fbb, &fb_save);
  fbb.Finish(packed);
  return fbb;
}

struct MakeLandmineData {
  Vector2 pos = {};
};

void MakeLandmine(MakeLandmineData data) {  ///
  Landmine v{.pos = data.pos};
  *g.run.landmines.Add() = v;
}

struct MakeGardenData {
  Vector2 pos = {};
};

void MakeGarden(MakeGardenData data) {  ///
  Garden v{.pos = data.pos};
  v.createdAt.SetNow();
  *g.run.gardens.Add() = v;
}

void ApplyAilment(
  Creature*        creature,
  CreatureType     damagerCreatureType,
  ApplyAilmentData data
) {  ///
  ASSERT(data.type);

  auto fb_ailment = glib->ailments()->Get(data.type);

  auto& a             = creature->ailments[data.type - 1];
  a.ownerCreatureType = damagerCreatureType;
  if ((data.type == AilmentType_ZAP) && !a.startedAt.IsSet())
    creature->speedModifier *= ZAP_SPEED_SCALE;

  bool wasSetPreviously = a.startedAt.IsSet();

  a.startedAt = {};
  a.startedAt.SetNow();
  a.duration = lframe::Unscaled((int)(FIXED_FPS * fb_ailment->duration_seconds()));

  if (wasSetPreviously) {
    a.value  = MAX(a.value, data.value);
    a.spread = MAX(a.spread, data.spread);
  }
  else {
    a.value  = data.value;
    a.spread = data.spread;
  }
}

void ChangeCoins(int amount) {  ///
  if (amount > 0)
    AchievementAdd(AchievementType_COINS, amount);

  PLAYER_COINS += amount;
  if (PLAYER_COINS < 0) {
    if (amount >= 0)
      PLAYER_COINS = int_max;
    else
      PLAYER_COINS = MAX(0, PLAYER_COINS);
  }
  Save();
}

void FlexBegin(int maxWidth, u16 childGap) {  ///
  ASSERT_FALSE(g.uiFlex.active);
  g.uiFlex = {
    .active   = true,
    .childGap = childGap,
    .maxWidth = maxWidth,
  };
}

void FlexEnd() {  ///
  if (g.uiFlex.addedChildren)
    Clay__CloseElement();
  g.uiFlex = {};
}

bool FlexShouldAddRowForChild(int childWidth) {  ///
  bool shouldAddRow = !g.uiFlex.addedChildren;
  if (g.uiFlex.addedChildren && (g.uiFlex.currentWidth + childWidth > g.uiFlex.maxWidth))
    shouldAddRow = true;
  return shouldAddRow;
}

void FlexAddRowForChildIfNeeded(int childWidth) {  ///
  if (FlexShouldAddRowForChild(childWidth)) {
    if (g.uiFlex.addedChildren)
      Clay__CloseElement();

    Clay__OpenElement();
    Clay__ConfigureOpenElement(CLAY__CONFIG_WRAPPER(
      Clay_ElementDeclaration,
      {
        .layout{
          .childGap = g.uiFlex.childGap,
          BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        },
      }
    ));
    g.uiFlex.currentWidth = 0;
  }

  g.uiFlex.addedChildren = true;
  g.uiFlex.currentWidth += childWidth + g.uiFlex.childGap;
}

bool IsAlreadyPlaceholded(const char* placeholder) {  ///
  FOR_RANGE (int, i, g.uiFlex.placeholdersCount) {
    if (!strcmp(g.uiFlex.placeholders[i].placeholder, placeholder))
      return true;
  }
  return false;
}

// value must be statically allocated or live in trashArena.
void PlaceholdString(
  const char* placeholder,
  const char* value,
  Color       color = palTextGreen
) {  ///
  ASSERT_FALSE(IsAlreadyPlaceholded(placeholder));
  Placeholder p{
    .type        = PlaceholderType_STRING,
    .placeholder = placeholder,
    ._u{.string{.value = value, .color = color}},
  };
  g.uiFlex.placeholders[g.uiFlex.placeholdersCount++] = p;
}

void PlaceholdBrokenLocale(const char* placeholder, int locale) {  ///
  ASSERT_FALSE(IsAlreadyPlaceholded(placeholder));
  Placeholder p{
    .type        = PlaceholderType_BROKEN_LOCALE,
    .placeholder = placeholder,
    ._u{.brokenLocale_value = locale},
  };
  g.uiFlex.placeholders[g.uiFlex.placeholdersCount++] = p;
}

void MakePickupable(MakePickupableData data) {  ///
  ASSERT(data.type);

  Pickupable pickupable{
    .type = data.type,
    .pos  = data.pos,
  };

  switch (data.type) {
  case PickupableType_COIN: {
    auto& d  = pickupable.DataCoin();
    d.amount = data.coin_amount;
  } break;

  case PickupableType_CONSUMABLE:
  case PickupableType_CRATE: {
    // Intentionally left blank.
  } break;

  default:
    INVALID_PATH;
    break;
  }

  pickupable.createdAt.SetNow();
  *g.run.pickupables.Add() = pickupable;
}

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

void MakeNumber(MakeNumberData data) {  ///
  ASSERT(data.type);
  Number number{
    .type  = data.type,
    .value = data.value,
    .pos   = data.pos,
  };
  number.createdAt.SetNow();
  *g.run.numbers.Add() = number;
}

static BF_FORCE_INLINE Clay_Dimensions MeasureText(
  Clay_StringSlice        text,
  Clay_TextElementConfig* config,
  void*                   userData
) noexcept {  ///
  ASSERT(config);
  // TODO: fontSize, letterSpacing
  const auto font  = &g.meta.fontUI + config->fontId;
  f32        width = 0;

  IterateOverCodepoints(
    text.chars,
    text.length,
    [&font, &width](u32 codepoint, u32 _codepointSize) BF_FORCE_INLINE_LAMBDA {
      if (!codepoint)
        return;

      stbtt_aligned_quad _q{};
      f32                _y{};

      auto glyphIndex
        = ArrayBinaryFind(font->codepoints, font->codepointsCount, (int)codepoint);
      ASSERT(glyphIndex >= 0);

      f32 w{};

      stbtt_GetPackedQuad(
        font->chars,
        font->atlasTexture.size.x,
        font->atlasTexture.size.y,
        glyphIndex,
        &w,
        &_y,
        &_q,
        1  // 1=opengl & d3d10+,0=d3d9
      );

      width += w / font->_scaleToFit;
    }
  );

  return {(f32)width, (f32)font->size / font->_scaleToFit};
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
  const auto texture      = glib->atlas_textures()->Get(data.texId);
  const auto originalSize = glib->original_texture_sizes()->Get(data.texId);

  if (g.uiFlex.active)
    FlexAddRowForChildIfNeeded(Ceil((f32)originalSize->x() * ASSETS_TO_LOGICAL_RATIO));

  CLAY({
    .layout{
      .sizing{
        .width  = CLAY_SIZING_FIXED((f32)originalSize->x() * ASSETS_TO_LOGICAL_RATIO),
        .height = CLAY_SIZING_FIXED((f32)originalSize->y() * ASSETS_TO_LOGICAL_RATIO),
      },
      BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
    },
  })
  CLAY({
    .layout{
      .sizing{
        .width  = CLAY_SIZING_FIXED((f32)originalSize->x() * ASSETS_TO_LOGICAL_RATIO),
        .height = CLAY_SIZING_FIXED((f32)originalSize->y() * ASSETS_TO_LOGICAL_RATIO),
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

// NOTE: This overload DOESN'T SAVE string to trash arena.
void BF_CLAY_TEXT(Clay_String string, Color color = palTextWhite) {  ///
  u16 fontId = 0;
  if (g.ui.overriddenFont)
    fontId = (UINT_FROM_PTR(g.ui.overriddenFont) - UINT_FROM_PTR(&g.meta.fontUI))
             / sizeof(Font);
  ASSERT(fontId >= 0);

  if (g.uiFlex.active) {
    Clay_StringSlice s{
      .length    = string.length,
      .chars     = string.chars,
      .baseChars = string.chars,
    };
    Clay_TextElementConfig cfg{.fontId = fontId};
    auto                   dim = MeasureText(s, &cfg, nullptr);

    // In flex space shouldn't break the line.
    // It's only other elements (which need more width) break the line.
    if (FlexShouldAddRowForChild(dim.width)) {
      if ((string.length == 1) && (string.chars[0] == ' '))
        return;
    }

    FlexAddRowForChildIfNeeded(dim.width);
  }

  CLAY_TEXT(
    string,
    CLAY_TEXT_CONFIG({
      .textColor = ToClayColor(color),
      .fontId    = fontId,
    })
  );
}

// NOTE: This overload SAVES string to trash arena.
void BF_CLAY_TEXT(const char* text, Color color = palTextWhite) {  ///
  int         len           = 0;
  const char* allocatedText = PushTextToArena(&g.meta.trashArena, text, &len);
  Clay_String string{
    .length = (i32)len,
    .chars  = allocatedText,
  };
  BF_CLAY_TEXT(string, color);
}

void BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(
  int   locale_,
  Color color              = palTextWhite,
  bool  _resetPlaceholders = true
) {  ///
  const auto localization              = glib->localizations()->Get(ge.meta.localization);
  const auto localization_broken_lines = localization->broken_lines();

  if (BF_DEBUG) {
    // Checking that all required placeholders were set.
    for (auto line : *localization_broken_lines->Get(locale_)->lines()) {
      for (auto group : *line->groups()) {
        for (auto string : *group->strings()) {
          if (!string->placeholder())
            continue;
          bool found = false;
          FOR_RANGE (int, i, g.uiFlex.placeholdersCount) {
            if (!strcmp(
                  g.uiFlex.placeholders[i].placeholder, string->placeholder()->c_str()
                ))
            {
              found = true;
              break;
            }
          }
          ASSERT(found);
        }
      }
    }
  }

  for (auto line : *localization_broken_lines->Get(locale_)->lines()) {
    // TODO: Proper lines support!
    for (auto group : *line->groups()) {
      for (auto string : *group->strings()) {
        if (string->type() == BrokenStringDatumType_SPACE) {
          Clay_String text{.isStaticallyAllocated = true, .length = 1, .chars = " "};
          BF_CLAY_TEXT(text, color);
          continue;
        }

        if (string->placeholder()) {
          const auto placeholderString = string->placeholder()->c_str();
          int        pl                = -1;
          FOR_RANGE (int, i, g.uiFlex.placeholdersCount) {
            if (!strcmp(g.uiFlex.placeholders[i].placeholder, placeholderString)) {
              pl = i;
              break;
            }
          }

          const auto& placeholder = g.uiFlex.placeholders[pl];
          ASSERT(placeholder.type);
          clayPlaceholderFunctions[(int)placeholder.type - 1](&placeholder);
        }
        else {
          Clay_String text{
            .isStaticallyAllocated = true,
            .length                = (i32)string->string()->size(),
            .chars                 = string->string()->c_str(),
          };
          BF_CLAY_TEXT(text, color);
        }
      }
    }
  }

  if (_resetPlaceholders)
    g.uiFlex.placeholdersCount = 0;
}

void DestroyBody(Body* body) {  ///
  b2DestroyBody(body->id);
  for (auto& shape : g.run.bodyShapes) {
    if (shape.body.createdId == body->createdId)
      shape.active = false;
  }
}

void AddBodyShape(BodyShape v) {  ///
  ASSERT(v.active);
  ASSERT(v.type);

  for (auto& shape : g.run.bodyShapes) {
    if (!shape.active) {
      shape = v;
      return;
    }
  }

  *g.run.bodyShapes.Add() = v;
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
  bodyDef.linearDamping = BODY_LINEAR_DAMPING;

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
    break;
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

int MakeCreature(MakeCreatureData data) {  ///
  ASSERT(data.type);

  auto index = g.run.creatures.count;
  auto slot  = g.run.creatures.Add();

  const auto fb = glib->creatures()->Get(data.type);

  f32 hurtboxRadius = PLAYER_HURTBOX_RADIUS;
  if (data.type != CreatureType_PLAYER)
    hurtboxRadius = MOB_HURTBOX_RADIUS * fb->hurtbox_scale();

  int health = fb->health()
               + Round(
                 ((g.run.state.waveIndex - fb->appearing_wave_number() + 1))
                 * fb->health_increase_per_wave()
               );

  if (fb->hostility_type() != HostilityType_FRIENDLY) {
    const f32 mobHpScale
      = (f32)(100 + g.run.playerStats[StatType_ENEMY_HP_SCALE]) / 100.0f;
    health = Round((f32)health * mobHpScale);
  }

  const auto creatureId = g.run.nextCreatureId++;

  Creature creature{
    .id        = creatureId,
    .type      = data.type,
    .health    = health,
    .maxHealth = health,
    .pos       = data.pos,
    .body      = MakeCircleBody({
           .pos           = data.pos,
           .radius        = CREATURE_COLLIDER_RADIUS * fb->collider_scale(),
           .hurtboxRadius = hurtboxRadius,
           .bodyData{
             .type     = BodyType_CREATURE,
             .userData = ShapeUserData::Creature(creatureId),
             .isPlayer = (data.type == CreatureType_PLAYER),
      },
    }),
    .speed     = fb->speed() + Lerp(-1.0f, 1.0f, GRAND.FRand()) * fb->speed_variation(),
  };
  creature.idleStartedAt.SetNow();

  if ((fb->aggro_distance() != f32_inf) || !fb->can_aggro()) {
    creature.controller.move = Vector2Rotate({1, 0}, 2 * PI32 * GRAND.FRand());
    creature.speedModifier *= fb->not_aggroed_speed() / fb->speed();
  }

  switch (creature.type) {
  case CreatureType_TURREL: {
    creature.DataTurrel() = {
      .aimDirection = Vector2Rotate({1, 0}, GRAND.FRand() * 2 * PI32),
    };
  } break;

  case CreatureType_RANGER: {
    creature.DataRanger() = {};
  } break;

  case CreatureType_RUSHER: {
    creature.DataRusher() = {};
  } break;

  default:
    break;
  }

  *slot = creature;
  return creature.id;
}

void MakeProjectile(MakeProjectileData data) {  ///
  ASSERT(data.type);
  ASSERT(data.dir != Vector2Zero());
  if (data.ownerCreatureType == CreatureType_PLAYER)
    ASSERT(data.weaponIndexOrMinus1 >= 0);

  Projectile projectile{
    .type                 = data.type,
    .ownerCreatureType    = data.ownerCreatureType,
    .weaponIndexOrMinus1  = data.weaponIndexOrMinus1,
    .pos                  = data.pos,
    .dir                  = data.dir,
    .damage               = data.damage,
    .critDamageMultiplier = data.critDamageMultiplier,
    .pierce               = data.pierce,
    .bounce               = data.bounce,
    .knockbackMeters      = data.knockbackMeters,
    .range                = data.range,
  };
  projectile.createdAt.SetNow();

  *g.run.projectiles.Add() = projectile;
}

void GamePreInit() {  ///
  ge.meta.logicRand           = Random(SDL_GetPerformanceCounter());
  ge.settings.backgroundColor = ColorFromRGBA(0x241207ff);
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

ItemType GenerateRandomItem() {  ///
  auto fb_items = glib->items();

  ItemType type{};
  int      currentItemCount = 0;
  while (1) {
    type = (ItemType)((GRAND.Rand() % ((u32)ItemType_COUNT - 1)) + 1);
    ASSERT(type);

    for (auto& item : g.run.state.items) {
      if (item.type == type)
        currentItemCount += item.count;
    }
    if (g.run.state.pickedUpItem.toPick == type)
      currentItemCount++;
    for (auto& v : g.run.state.shop.toPick) {
      if (v.item == type)
        currentItemCount++;
    }

    if (g.player.lockedItems[type])
      continue;
    auto fb = fb_items->Get(type);
    if (!fb->included_in_random_pool())
      continue;
    if (fb->count_cap() <= 0)
      break;
    if (fb->count_cap() > currentItemCount)
      break;
  }
  return type;
}

int _ApplyStatPrice(StatType s, int price) {  ///
  const StatType allowed_[]{StatType_ITEMS_PRICE, StatType_REROLL_PRICE};
  VIEW_FROM_ARRAY_DANGER(allowed);
  ASSERT(allowed.Contains(s));

  int stat = g.run.playerStats[s];
  if (stat == 0)
    return price;
  stat        = MAX(-100, stat);
  auto factor = (f32)(stat + 100) / 100.0f;
  price       = Round((f32)price * factor);
  return price;
}

int ApplyStatItemsPrice(int price) {  ///
  return _ApplyStatPrice(StatType_ITEMS_PRICE, price);
}

int ApplyStatRerollPrice(int price) {  ///
  return _ApplyStatPrice(StatType_REROLL_PRICE, price);
}

int GetNumberOfTreesToSpawn() {  ///
  // ref: https://brotato.wiki.spellsandguns.com/Trees#Spawn
  const int stat = g.run.playerStats[StatType_TREES];

  int min = stat / 3;

  int percentsCount = 2;
  if ((stat + 1) % 3 == 0)
    percentsCount = 3;

  f32 percents[3]{};
  percents[0] = 49.5f - (f32)min;
  percents[1] = 83.5f;
  percents[2] = 16.0f - (f32)min / 2.0f;

  f32 total = 0;
  FOR_RANGE (int, i, percentsCount)
    total += percents[i];

  f32 factors[3]{};
  FOR_RANGE (int, i, percentsCount)
    factors[i] = percents[i] / total;

  for (int i = 1; i < percentsCount; i++)
    factors[i] += factors[i - 1];

  const f32 t = GRAND.FRand();
  FOR_RANGE (int, i, percentsCount) {
    if (t < factors[i])
      return min + i;
  }

  INVALID_PATH;
  return 0;
}

int GetWeaponPrice(WeaponType type, int tier) {  ///
  ASSERT(tier >= 0);
  ASSERT(tier < 4);
  const auto fb = glib->weapons()->Get(type);
  ASSERT(fb->min_tier_index() <= tier);
  return Round((f32)fb->max_price() * PRICE_SCALINGS_PER_TIER[tier]);
}

int GetWeaponRecyclePrice(WeaponType type, int tier) {  ///
  return GetWeaponPrice(type, tier) / RECYCLE_PRICE_FACTOR;
}

int GetNextLevelXp(int currentLevel) {  ///
  return SQR(currentLevel + 3);
}

void IterateOverWeaponEffects(
  EffectConditionType                                                       condition,
  auto /* void (int weaponIndexOrMinus1, Weapon& weapon, auto fb_effect) */ innerLambda
) {  ///
  const auto fb_weapons          = glib->weapons();
  int        weaponIndexOrMinus1 = -1;
  for (auto& weapon : g.run.state.weapons) {
    weaponIndexOrMinus1++;
    if (!weapon.type)
      continue;
    const auto fb         = fb_weapons->Get(weapon.type);
    const auto fb_effects = fb->effects();
    if (fb_effects) {
      for (const auto fb_effect : *fb_effects) {
        if (fb_effect->effectcondition_type() == condition)
          innerLambda(weaponIndexOrMinus1, weapon, fb_effect);
      }
    }
  }
}

void IterateOverEffects(
  EffectConditionType                         condition,
  auto /* void (auto fb_effect, int times) */ innerLambda
) {  ///
  IterateOverWeaponEffects(
    condition,
    [&](int _, Weapon& weapon, auto fb_effect)
      BF_FORCE_INLINE_LAMBDA { innerLambda(fb_effect, 1); }
  );

  // Iterating over items.
  {
    const auto fb_items = glib->items();
    for (auto& item : g.run.state.items) {
      const auto fb         = fb_items->Get(item.type);
      const auto fb_effects = fb->effects();
      if (fb_effects) {
        for (const auto fb_effect : *fb_effects) {
          if (fb_effect->effectcondition_type() == condition)
            innerLambda(fb_effect, item.count);
        }
      }
    }
  }

  // Iterating over difficulty effects.
  {
    const auto fb         = glib->difficulties()->Get(g.player.difficulty);
    const auto fb_effects = fb->effects();
    if (fb_effects) {
      for (const auto fb_effect : *fb_effects) {
        if (fb_effect->effectcondition_type() == condition)
          innerLambda(fb_effect, 1);
      }
    }
  }

  // Iterating over build effects.
  {
    const auto fb         = glib->builds()->Get(g.player.build);
    const auto fb_effects = fb->effects();
    if (fb_effects) {
      for (const auto fb_effect : *fb_effects) {
        if (fb_effect->effectcondition_type() == condition)
          innerLambda(fb_effect, 1);
      }
    }
  }
}

void RunInit() {
  ZoneScoped;

  g.meta.paused               = false;
  g.meta.scheduledTogglePause = false;

  // Creating box2d world.
  {  ///
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = b2Vec2{0, 0};
    g.run.world         = b2CreateWorld(&worldDef);
  }

  // Initializing `g.run.state.statsWithoutItems`.
  FOR_RANGE (int, stat, (int)StatType_COUNT) {  ///
    const auto fb                       = glib->stats()->Get(stat);
    g.run.state.statsWithoutItems[stat] = fb->player_value();
  }
  g.run.recalculatePlayerStats = true;

  // Making player.
  auto playerPos = (Vector2)WORLD_SIZE / 2.0f;
  MakeCreature({
    .type = CreatureType_PLAYER,
    .pos  = playerPos,
  });
  g.run.previousPlayerPos = playerPos;
  g.run.playerLastRegenAt.SetNow();
  PLAYER_CREATURE.health    = g.run.state.statsWithoutItems[StatType_HP];
  PLAYER_CREATURE.maxHealth = g.run.state.statsWithoutItems[StatType_HP];

  g.run.camera.pos = GetCameraTargetPos();

  {
    auto& weapon = g.run.state.weapons[0];
    weapon.type  = g.player.weapon;
    weapon.tier  = 0;
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

  IterateOverEffects({}, [&](auto fb_effect, int times) BF_FORCE_INLINE_LAMBDA {
    ApplyEffect(fb_effect, times);
  });

  OnWaveStarted();
}

void ReloadFontsIfNeeded() {  ///
  static bool       loaded             = false;
  static Vector2Int previousScreenSize = {};

  // Debounce.
  {
    static int debounce = 0;
    if (previousScreenSize == ge.meta.screenSize) {
      debounce = 0;
      return;
    }
    if (loaded) {
      Clay_ResetMeasureTextCache();
      static Vector2Int lastFrameScreenSize = {};
      if (lastFrameScreenSize != ge.meta.screenSize) {
        lastFrameScreenSize = ge.meta.screenSize;
        debounce            = 0;
        return;
      }
      debounce++;
      if (debounce < FIXED_FPS / 5)
        return;
    }
    previousScreenSize = ge.meta.screenSize;
  }

  if (loaded) {
    ReloadFonts(&g.meta.loadedFonts);
    return;
  }

  static auto fontpath = "resources/arialnb.ttf";
  static int  priceCodepoints[]{
    ' ', '+', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
  };
  static int itemCountCodepoints[]{
    ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'x'
  };

  static LoadFontData loadFontData_[]{
    // fontUI.
    {
      .filepath        = fontpath,
      .size            = 15,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
    },
    // fontUIOutlined.
    {
      .filepath        = fontpath,
      .size            = 15,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
      .outlineWidth    = 3,
      .outlineAdvance  = 1,
    },
    // fontUIBig.
    {
      .filepath        = fontpath,
      .size            = 20,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
    },
    // fontStats.
    {
      .filepath        = fontpath,
      .size            = 13,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
    },
    // fontPrices.
    {
      .filepath        = fontpath,
      .size            = 20,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = priceCodepoints,
      .codepointsCount = ARRAY_COUNT(priceCodepoints),
    },
    // fontItemCountsOutlined.
    {
      .filepath        = fontpath,
      .size            = 20,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = itemCountCodepoints,
      .codepointsCount = ARRAY_COUNT(itemCountCodepoints),
      .outlineWidth    = 3,
      .outlineAdvance  = 1,
    },
    // fontWaveCompletion.
    {
      .filepath        = fontpath,
      .size            = 40,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
      .outlineWidth    = 3,
      .outlineAdvance  = 0,
    },
  };
  VIEW_FROM_ARRAY_DANGER(loadFontData);

  loaded = true;
  g.meta.loadedFonts
    = LoadFonts({.count = loadFontData.count, .base = &g.meta.fontUI}, loadFontData);
}

void GameInit() {  ///
  ZoneScoped;

  g.meta.trashArena = MakeArena(16 * 1024);
  g.run.arena       = MakeArena(4 * 1024);

  // Initializing Clay.
  {
    auto size = Clay_MinMemorySize();
    Clay_Initialize(
      Clay_CreateArenaWithCapacityAndMemory(size, BF_ALLOC(size)),
      Clay_Dimensions{(f32)LOGICAL_RESOLUTION.x, (f32)LOGICAL_RESOLUTION.y},
      Clay_ErrorHandler{HandleClayErrors}
    );
    Clay_SetCullingEnabled(false);
    Clay_SetMeasureTextFunction(MeasureText, 0);
  }

  ReloadFontsIfNeeded();

  RunInit();

  auto saveData = SDL_LoadFile("save.bin", nullptr);
  if (saveData) {
    Load(saveData);
    SDL_free(saveData);
  }
}

constexpr lframe GetFramesPerRegen(int regenLevel) {  ///
  if (regenLevel <= 0)
    return lframe::Unscaled(i64_max);
  const f32 regenPerSecond = (f32)regenLevel / 11.25f + 1.0f / 9.0f;
  return lframe::Unscaled((i64)((f32)FIXED_FPS / regenPerSecond));
}

f32 GetLifestealChance(WeaponType typeOrInvalid, bool affectedByGame = true) {  ///
  f32 lifesteal = 0;
  if (affectedByGame)
    lifesteal += (f32)g.run.playerStats[StatType_LIFE_STEAL] / 100.0f;
  if (typeOrInvalid)
    lifesteal += glib->weapons()->Get(typeOrInvalid)->life_steal_percent() / 100.0f;
  return lifesteal;
}

struct TryApplyDamageData {
  int              creatureIndex                      = {};
  int              damage                             = {};
  Vector2          directionOrZero                    = {0, 0};
  f32              knockbackMeters                    = 0;
  CreatureType     damagerCreatureType                = CreatureType_INVALID;
  bool             isCrit                             = false;
  int              indexOfWeaponThatDidDamageOrMinus1 = -1;
  ApplyAilmentData ailment                            = {};
  f32              ailmentChance                      = 0;
};

bool TryApplyDamage(TryApplyDamageData data) {  ///
  ASSERT(data.creatureIndex >= 0);
  ASSERT(data.damage >= 0);
  if (data.damage != int_max) {
    auto t = GRAND.Rand() % 7;
    if ((t == 0) || (t == 1))
      data.damage += 1;
    else if (t == 2)
      data.damage -= 1;
  }

  data.damage = MAX(1, data.damage);

  if (data.directionOrZero != Vector2Zero())
    ASSERT(FloatEquals(Vector2Length(data.directionOrZero), 1));

  // Player can't take damage when finishing wave.
  if (!data.creatureIndex && g.run.scheduledWaveCompleted.IsSet())
    return false;

  auto& creature = g.run.creatures[data.creatureIndex];
  if (creature.health <= 0)
    return false;

  const auto fb = glib->creatures()->Get(creature.type);

  creature.aggroed = fb->can_aggro();

  if (data.creatureIndex) {
    auto fb_damager = glib->creatures()->Get(data.damagerCreatureType);
    if (fb_damager->hostility_type() == HostilityType_FRIENDLY) {
      MakeNumber({
        .type  = (data.isCrit ? NumberType_DAMAGE_CRIT : NumberType_DAMAGE),
        .value = data.damage,
        .pos   = creature.pos,
      });
    }
  }
  else {
    // Can't hurt player if he was recently damaged.
    if (creature.lastDamagedAt.IsSet()
        && (creature.lastDamagedAt.Elapsed() <= PLAYER_INVINCIBILITY_FRAMES))
      return false;

    // Dodge.
    if (GRAND.FRand() < MIN(
          (f32)MAX_DODGE_PERCENT / 100.0f, (f32)g.run.playerStats[StatType_DODGE] / 100.0f
        ))
    {
      MakeNumber({.type = NumberType_DODGE, .pos = creature.pos});

      // TODO: lastInvincibilityTriggeredAt?
      creature.lastDamagedAt = {};
      creature.lastDamagedAt.SetNow();
      return false;
    }

    // Applying player's armor.
    auto armor = (f32)g.run.playerStats[StatType_ARMOR];
    if (armor > 0)
      data.damage = Round((f32)data.damage * 1.0f / (1.0f + armor / 15.0f));
    else if (armor < 0)
      data.damage = Round((f32)data.damage * (15.0f - 2 * armor) / (15 - armor));
    data.damage = MAX(1, data.damage);

    MakeNumber({
      .type  = NumberType_DAMAGE_MOB,
      .value = data.damage,
      .pos   = creature.pos,
    });
  }

  // Player lifesteals.
  if (data.damagerCreatureType == CreatureType_PLAYER) {
    auto weaponType = WeaponType_INVALID;
    if (data.indexOfWeaponThatDidDamageOrMinus1 >= 0)
      weaponType = g.run.state.weapons[data.indexOfWeaponThatDidDamageOrMinus1].type;

    if (GRAND.FRand() < GetLifestealChance(weaponType)) {
      bool canLifesteal = true;
      if (g.run.playerLastLifestealAt.IsSet()
          && (g.run.playerLastLifestealAt.Elapsed() < LIFESTEAL_COOLDOWN_FRAMES))
        canLifesteal = false;

      if (canLifesteal && (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth)) {
        PLAYER_CREATURE.health
          = MoveTowards(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth, 1);
        g.run.playerLastLifestealAt = {};
        g.run.playerLastLifestealAt.SetNow();
      }
    }
  }

  if (data.damage == int_max) {
    creature.killedBecauseOfTheEndOfTheWave = true;
    creature.health                         = 0;
  }
  else
    creature.health -= data.damage;

  bool ailmentCanBeApplied = true;
  auto resists             = fb->resists_ailment_types();
  if (resists) {
    for (auto v : *resists) {
      if (v == data.ailment.type) {
        ailmentCanBeApplied = false;
        break;
      }
    }
  }

  if (ailmentCanBeApplied && data.ailment.type && (GRAND.FRand() < data.ailmentChance)) {
    auto damage = data.ailment.value;
    if (data.damagerCreatureType == CreatureType_PLAYER)
      damage += g.run.playerStats[StatType_DAMAGE_ELEMENTAL];

    if ((data.ailment.value == 0) || (damage > 0)) {
      data.ailment.value = damage;
      ApplyAilment(&creature, data.damagerCreatureType, data.ailment);
    }
  }

  if (data.indexOfWeaponThatDidDamageOrMinus1 >= 0)
    g.run.state.weapons[data.indexOfWeaponThatDidDamageOrMinus1].thisWaveDamage
      += data.damage;

  if (data.damage != int_max) {
    creature.lastDamagedAt = {};
    creature.lastDamagedAt.SetNow();
    creature.lastDamagedFlashAt = {};
    creature.lastDamagedFlashAt.SetNow();
  }

  creature.lastDamagedWeaponIndex = data.indexOfWeaponThatDidDamageOrMinus1;

  data.knockbackMeters *= b2Body_GetMass(creature.body.id) * BODY_LINEAR_DAMPING;
  data.knockbackMeters *= 1.0f - fb->knockback_resistance();

  b2Body_ApplyLinearImpulseToCenter(
    creature.body.id, ToB2Vec2(data.directionOrZero * data.knockbackMeters), true
  );

  if (!g.run.justDamagedCreatures.Contains(data.creatureIndex))
    *g.run.justDamagedCreatures.Add() = data.creatureIndex;

  if (!data.creatureIndex)
    Save();

  return true;
}

void RunReset() {  ///
  ZoneScoped;

  b2DestroyWorld(g.run.world);

  g.run.state.items.Reset();

  // Resetting `g.run` to a default value,
  // while preserving allocated memory of it's Vectors.
  struct {
    struct {
      Vector<Item> items;
    } state = {
      .items = g.run.state.items,
    };
#define X(type_, name_) Vector<type_> name_ = g.run.name_;
    VECTORS_TABLE;
#undef X
  } temp{};

#define X(type_, name_) temp.name_.Reset();
  VECTORS_TABLE;
#undef X

  g.run = {
    .state{.items = temp.state.items},
#define X(type_, name_) .name_ = temp.name_,
    VECTORS_TABLE
#undef X
  };

  g.run.arena.used = 0;
}

int GetRerollPrice(int waveIndex, int rerolledTimes) {  ///
  int rerollPricesPerWave_[]{1,  2,  3,  4,  5,  6,  7,  9,  9,  11,
                             12, 13, 14, 15, 17, 18, 18, 20, 21, 23};
  VIEW_FROM_ARRAY_DANGER(rerollPricesPerWave);

  int rerollPriceIncreasePerWave_[]{1, 1, 1, 1, 2, 2, 2, 3, 3, 4,
                                    4, 4, 5, 5, 6, 6, 6, 7, 7, 8};
  VIEW_FROM_ARRAY_DANGER(rerollPriceIncreasePerWave);

  auto price = rerollPricesPerWave[waveIndex]
               + rerolledTimes * rerollPriceIncreasePerWave[waveIndex];
  return price;
}

void Rerolls::Roll() {  ///
  PLAYER_COINS -= ApplyStatRerollPrice(GetPrice());
  ASSERT(PLAYER_COINS >= 0);
  if (rerolledFreeTimes < g.run.playerStats[StatType_FREE_REROLLS])
    rerolledFreeTimes++;
  else
    rerolledTimes++;
}

int Rerolls::GetPrice() const {  ///
  if (rerolledFreeTimes < g.run.playerStats[StatType_FREE_REROLLS])
    return 0;
  return GetRerollPrice(g.run.state.waveIndex, rerolledTimes);
}

void RefillUpgradesToPick() {  ///
  const auto fb_stats = glib->stats();

  FOR_RANGE (int, i, g.run.state.upgrades.toPick.count) {
    while (1) {
      const auto newStat = (StatType)(GRAND.Rand() % fb_stats->size());
      if (!newStat)
        continue;

      const auto fb = fb_stats->Get(newStat);

      // Can't upgrade secondary stats.
      if (fb->is_secondary())
        continue;

      // Can't upgrade `curse`.
      if (!fb->upgrade_texture_id())
        continue;

      // Not showing same upgrade twice.
      bool contains = false;
      for (const auto& v : g.run.state.upgrades.toPick) {
        if (v.stat == newStat) {
          contains = true;
          break;
        }
      }
      if (contains)
        continue;

      // Setting upgrade.
      g.run.state.upgrades.toPick[i] = {
        .stat = newStat,
        .tier = (int)(GRAND.Rand() % TOTAL_TIERS),
      };
      break;
    }
  }
}

void RefillShopToPick() {  ///
  const auto fb_items   = glib->items();
  const auto fb_weapons = glib->weapons();

  for (auto& x : g.run.state.shop.toPick)
    x = {};

  for (auto& x : g.run.state.shop.toPick) {
    const bool setToItem = (GRAND.FRand() <= SHOP_ITEM_RATIO);
    if (setToItem) {
      x.item = GenerateRandomItem();

      const auto fb = fb_items->Get(x.item);
      x.tier        = fb->tier();
    }
    else {
      while (!x.weapon) {
        auto w = (WeaponType)(GRAND.Rand() % fb_weapons->size());
        if (g.player.lockedWeapons[w])
          continue;
        x.weapon      = w;
        const auto fb = fb_weapons->Get(x.weapon);
        // Legendary weapons can't be bought in shop.
        // TODO: Check if it's possible to buy legendary weapons in Brotato.
        x.tier = (int)(GRAND.Rand() % (TOTAL_TIERS - 1));
      }
    }
  }
}

void RecalculateShopToPick() {  ///
  for (auto& x : g.run.state.shop.toPick) {
    if (x.item)
      x.price = glib->items()->Get(x.item)->price();
    else if (x.weapon)
      x.price = GetWeaponPrice(x.weapon, x.tier);
  }
}

void AddItem(ItemType type) {  ///
  bool increasedExistingItemCount = false;
  for (auto& item : g.run.state.items) {
    if (item.type == type) {
      item.count++;
      increasedExistingItemCount = true;
      break;
    }
  }
  if (!increasedExistingItemCount) {
    Item item{.type = type, .count = 1};
    *g.run.state.items.Add() = item;
  }

  g.run.recalculatePlayerStats = true;
}

void StableRemoveWeapon(int index) {  ///
  for (int i = index + 1; i < g.run.state.weapons.count; i++)
    g.run.state.weapons[i - 1] = g.run.state.weapons[i];
  g.run.state.weapons[g.run.state.weapons.count - 1] = {};
}

f32 _AttackSpeedMultiplier(int v) {  ///
  if (v >= 0)
    return (f32)(100 + v) / 100.0f;
  return -100.0f / (f32)(v - 100);
}

f32 GetPlayerStatAttackSpeedMultiplier() {  ///
  return _AttackSpeedMultiplier(g.run.playerStats[StatType_ATTACK_SPEED]);
}

f32 GetPlayerStatStructureAttackSpeedMultiplier() {  ///
  return _AttackSpeedMultiplier(g.run.playerStats[StatType_STRUCTURE_ATTACK_SPEED]);
}

lframe ApplyAttackSpeedToDuration(int duration) {  ///
  return lframe::Unscaled(MAX(
    2,
    (int)((f32)(_BF_LOGICAL_FPS_SCALE * duration) / GetPlayerStatAttackSpeedMultiplier())
  ));
}

lframe ApplyStructureAttackSpeedToDuration(int duration) {  ///
  return lframe::Unscaled(MAX(
    2,
    (int)((f32)(_BF_LOGICAL_FPS_SCALE * duration)
          / GetPlayerStatStructureAttackSpeedMultiplier())
  ));
}

f32 GetExplosionDamageMultiplier() {  ///
  auto v = (f32)g.run.playerStats[StatType_EXPLOSION_DAMAGE];
  return MAX(0, 1.0f + v / 100.0f);
}

f32 GetExplosionSizeMultiplier() {  ///
  auto v = (f32)g.run.playerStats[StatType_EXPLOSION_SIZE];
  if (v >= 0)
    return 1 + v / 100.0f;
  return powf(2, v / 50);
}

// NOTE: Doesn't include melee weapon's size.
f32 GetWeaponRangeMeters(WeaponType type, bool affectedByGame = true) {  ///
  const auto fb = glib->weapons()->Get(type);

  f32 range = 0;
  if (affectedByGame)
    range = (f32)g.run.playerStats[StatType_RANGE];

  f32 bonusRange = 0;

  if (range >= 0)
    bonusRange = range * RANGE_TO_METER_SCALE;
  else
    bonusRange = (powf(2, range / -RANGE_GETS_HALVED_WHEN) - 1) * fb->range_meters();

  // Divided by 2 because range stat is half as effective for melee weapons.
  // (don't confuse with weapons that have DamageType_MELEE).
  // TODO: better naming to differentiate
  // MELEE weapon from MELEE damage type. Ideas?
  if (!fb->projectile_type())
    bonusRange /= 2.0f;

  return fb->range_meters() + bonusRange;
}

bool IsCrit() {  ///
  const f32 chance = (f32)g.run.playerStats[StatType_CRIT_CHANCE] / 100.0f;
  return GRAND.FRand() < chance;
}

int TryGetCreatureIndexById(int id) {  ///
  int i = -1;
  for (const auto& creature : g.run.creatures) {
    i++;
    if (creature.id == id)
      return i;
  }
  return -1;
}

int GetCreatureIndexById(int id) {  ///
  int index = TryGetCreatureIndexById(id);
  ASSERT(index >= 0);
  return index;
}

bool OnWeaponCollided(b2ShapeId shapeId, int* const weaponIndexOrMinus1) {  ///
  auto& weapon = g.run.state.weapons[*weaponIndexOrMinus1];

  const bool continueCollisions = true;
  const auto userData = ShapeUserData::FromPointer(b2Shape_GetUserData(shapeId));

  const auto  creatureIndex = GetCreatureIndexById(userData.GetCreatureId());
  const auto& creature      = g.run.creatures[creatureIndex];

  ASSERT(creature.type);

  auto fb_creature = glib->creatures()->Get(creature.type);
  if (fb_creature->hostility_type() == HostilityType_FRIENDLY)
    return continueCollisions;

  const auto fb = glib->weapons()->Get(weapon.type);

  if (ArrayContains(weapon.piercedCreatureIds.base, weapon.piercedCount, creature.id))
    return continueCollisions;

  if (weapon.piercedCount < weapon.piercedCreatureIds.count) {
    weapon.piercedCreatureIds[weapon.piercedCount++] = creature.id;

    int  damage = weapon.calculatedDamage;
    bool isCrit = IsCrit();
    if (isCrit)
      damage = Round((f32)damage * fb->crit_damage_multiplier());

    TryApplyDamage({
      .creatureIndex       = creatureIndex,
      .damage              = damage,
      .directionOrZero     = Vector2DirectionOrRandom(PLAYER_CREATURE.pos, creature.pos),
      .knockbackMeters     = fb->knockback_meters(),
      .damagerCreatureType = CreatureType_PLAYER,
      .isCrit              = isCrit,
      .indexOfWeaponThatDidDamageOrMinus1 = *weaponIndexOrMinus1,
    });
  }
  return continueCollisions;
}

Vector2 GetWeaponPos(int weaponIndex) {  ///
  auto& weapon = g.run.state.weapons[weaponIndex];
  ASSERT(weapon.type);
  const auto fb = glib->weapons()->Get(weapon.type);

  if (fb->projectile_type() || !weapon.startedShootingAt.IsSet())
    return PLAYER_CREATURE.pos + GetPlayerWeaponOffset(weaponIndex);

  const auto e            = weapon.startedShootingAt.Elapsed();
  const auto shootingDur  = ApplyAttackSpeedToDuration(fb->shooting_duration_frames());
  auto       p            = MIN(1, e.Progress(shootingDur) * 2);
  const auto texId        = fb->texture_ids()->Get(0);
  const auto colliderSize = (f32)glib->original_texture_sizes()->Get(texId)->x()
                            * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;

  p = EaseInOutQuad(p);

  const f32  movingDistance = MAX(1, GetWeaponRangeMeters(weapon.type));
  const auto movedDistance  = p * movingDistance;

  return PLAYER_CREATURE.pos + weapon.targetDir * movedDistance;
}

void AddXP(f32 xp) {  ///
  if (xp > 0)
    xp *= (f32)(MAX(1, g.run.playerStats[StatType_XP_GAIN] + 100)) / 100.0f;

  g.run.state.xp += xp;
  g.run.state.xp = MAX(0, g.run.state.xp);

  // Handling level up.

  auto nextLevelXp = GetNextLevelXp(g.run.state.level);

  while (g.run.state.xp >= nextLevelXp) {
    g.run.state.xp -= nextLevelXp;
    g.run.state.level++;
    nextLevelXp = GetNextLevelXp(g.run.state.level);

    MakeNumber({.type = NumberType_LEVEL_UP, .pos = PLAYER_CREATURE.pos});

    // Increasing random stat that has `upgrade_values`.
    while (1) {
      const auto stat = (StatType)(GRAND.Rand() % StatType_COUNT);
      const auto fb   = glib->stats()->Get(stat);
      const auto vals = fb->upgrade_values();
      if (!vals)
        continue;
      g.run.state.statsWithoutItems[stat] += vals->Get(0);
      g.run.recalculatePlayerStats = true;
      break;
    }
  }

  Save();
}

bool CanSpawnMoreCreatures() {  ///
  const auto framesUntilTheEndOfTheWave
    = GetWaveDuration(g.run.state.waveIndex) - g.run.waveStartedAt.Elapsed();
  return (framesUntilTheEndOfTheWave > DONT_SPAWN_RIGHT_BEFORE_WAVE_ENDS + SPAWN_FRAMES);
}

void HealPlayer(f32 amount = 1) {  ///
  if (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth) {
    PLAYER_CREATURE.health
      = MoveTowards(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth, amount);
  }
  Save();
}

void ClayEffectCondition_KILL_N_ENEMIES_GET_STAT(const BFGame::Effect* fb_effect) {  ///
  PlaceholdString("ENEMIES", TextFormat("%d", fb_effect->condition_value()));
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(Loc_EFFECT_KILL_N_ENEMIES);
}

void ClayEffectCondition_END_OF_THE_WAVE_GET_STAT(const BFGame::Effect* fb_effect) {  ///
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(Loc_EFFECT_AT_THE_END_OF_THE_WAVE);
}

void ClayEffectCondition_START_OF_THE_WAVE_GET_STAT(const BFGame::Effect* fb_effect
) {  ///
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(Loc_EFFECT_AT_THE_START_OF_THE_WAVE);
}

void ClayEffectCondition_KILL_N_ENEMIES_USING_THIS_WEAPON_GET_STAT(
  const BFGame::Effect* fb_effect
) {  ///
  PlaceholdString("ENEMIES", TextFormat("%d", fb_effect->condition_value()));
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(Loc_WEAPON_EFFECT_KILL_N_ENEMIES_USING_THIS_WEAPON
  );
}

void ClayEffectCondition_MORE_OF_THE_SAME_WEAPON_MORE_PROPERTY(
  const BFGame::Effect* fb_effect
) {  ///
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(
    Loc_WEAPON_EFFECT_MORE_OF_THE_SAME_WEAPON_MORE_PROPERTY
  );
}

void ClayPlaceholderFunction_STRING(const Placeholder* placeholder) {  ///
  const auto& string = placeholder->string();
  BF_CLAY_TEXT(string.value, string.color);
}

void ClayPlaceholderFunction_BROKEN_LOCALE(const Placeholder* placeholder) {  ///
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(
    placeholder->brokenLocale_value(), palTextWhite, false
  );
}

void FontBegin(Font* font) {  ///
  ASSERT_FALSE(g.ui.overriddenFont);
  g.ui.overriddenFont = font;
}

void FontEnd() {  ///
  ASSERT(g.ui.overriddenFont);
  g.ui.overriddenFont = nullptr;
}

void ButtonSFX(bool draw, Clay_ElementId id, bool hovered) {  ///
  ASSERT(id.id);
  if (draw)
    return;

  int index = -1;

  FOR_RANGE (int, i, g.ui.buttonStatesCount) {
    auto& b = g.ui.buttonStates[i];
    if (b.id.id == id.id) {
      ASSERT_FALSE(b.thisFrameUpdated);
      index = i;
      break;
    }
  }

  if (index == -1)
    index = g.ui.buttonStatesCount++;

  auto& b = g.ui.buttonStates[index];
  ASSERT_FALSE(b.thisFrameUpdated);

  if (hovered && !b.hovered)
    PlaySound(Sound_UI_HOVER_SMALL);

  b.id               = id;
  b.hovered          = hovered;
  b.thisFrameUpdated = true;
}

int GetBuildTier(BuildType build) {  ///
  return MAX(0, (int)g.player.builds[build].maxDifficultyBeaten - 1);
}

void DoUI(bool draw) {
  ZoneScoped;

  // NOTE: Logic must be executed only when `draw` is false!
  // e.g. updating mouse position, processing `clicked()`,
  // logically reacting to `Clay_Hovered()`, changing game's state, etc.

  // Setup.
  // {  ///
  const auto fb_atlas_textures      = glib->atlas_textures();
  const auto original_texture_sizes = glib->original_texture_sizes();
  const auto fb_items               = glib->items();
  const auto fb_stats               = glib->stats();
  const auto fb_weapon_properties   = glib->weapon_properties();
  const auto fb_weapons             = glib->weapons();
  const auto fb_projectiles         = glib->projectiles();
  const auto fb_pickupables         = glib->pickupables();
  const auto fb_difficulties        = glib->difficulties();
  const auto fb_achievements        = glib->achievements();
  const auto fb_builds              = glib->builds();

  if (!draw) {
    // Updating clay mouse pos.
    // TODO: TOUCH!
    const auto pos = ScreenPosToLogical(GetMouseScreenPos());
    Clay_SetPointerState({pos.x, LOGICAL_RESOLUTION.y - pos.y}, false);
  }

  constexpr int MAX_BEAUTIFIERS      = 32;
  const auto    localization         = glib->localizations()->Get(ge.meta.localization);
  const auto    localization_strings = localization->strings();
  const auto    localization_broken_lines = localization->broken_lines();

  TEMP_USAGE(&g.meta.trashArena);

  Clay_BeginLayout();

  Array<Beautify, MAX_BEAUTIFIERS> beautifiers{};
  int                              beautifiersCount = 0;

  constexpr u16 GAP_FLEX                 = 2;
  constexpr u16 GAP_SMALL                = 8;
  constexpr u16 GAP_BIG                  = 20;
  constexpr u16 PADDING_NINE_SLICE_FRAME = 12;
  constexpr u16 PADDING_OUTER_VERTICAL   = 10;
  constexpr u16 PADDING_OUTER_HORIZONTAL = 12;

  int _wheel = GetMouseWheel();
  if (IsKeyDown(SDL_SCANCODE_LSHIFT))
    _wheel *= 10;
  if (draw)
    _wheel = 0;
  const int wheel  = _wheel;
  i16       zIndex = 0;

  const Color secondaryTextColor{0xef, 0xcb, 0x84, 255};

  const auto fb_slotColors = glib->ui_item_slot_colors();
  auto slotColors_ = ALLOCATE_ARRAY(&g.meta.trashArena, Color, fb_slotColors->size());
  FOR_RANGE (int, i, fb_slotColors->size()) {
    slotColors_[i] = ColorFromRGBA(fb_slotColors->Get(i));
  }
  const View<Color> slotColors{.count = (int)fb_slotColors->size(), .base = slotColors_};

  constexpr int CARD_WIDTH          = 219;
  constexpr int CARD_HEIGHT         = 320;
  constexpr int UPGRADE_FRAME_WIDTH = 190;
  constexpr int ACHIEVEMENT_WIDTH   = CARD_WIDTH;

  const f32 ACHIEVEMENT_HEIGHT =                      //
    0                                                 //
    + g.meta.fontUI.size / g.meta.fontUI._scaleToFit  // Name.
    + GAP_SMALL                                       // Gap between name and description.
    + 2 * g.meta.fontStats.size / g.meta.fontUI._scaleToFit  // 2 lines of description.
    + GAP_FLEX;                                              // Gap between them.
  // }

  LAMBDA (void, BF_CLAY_TEXT_LOCALIZED_DANGER, (int locale, Color color = palTextWhite))
  {  ///
    auto        string = localization_strings->Get(locale);
    Clay_String text{
      .isStaticallyAllocated = true,
      .length                = (i32)string->size(),
      .chars                 = string->c_str(),
    };
    BF_CLAY_TEXT(text, color);
  };

  LAMBDA (void, componentOverlay, ()) {  ///
    CLAY({
      .layout{
        .sizing{
          .width  = CLAY_SIZING_FIXED(LOGICAL_RESOLUTION.x * 2),
          .height = CLAY_SIZING_FIXED(LOGICAL_RESOLUTION.y * 2),
        },
      },
      .floating{
        .zIndex = (i16)(zIndex - 1),
        .attachPoints{
          .element = CLAY_ATTACH_POINT_CENTER_CENTER,
          .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
        },
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
      BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
    }) {}
  };

  LAMBDA (bool, clicked, ()) {  ///
    return !draw && Clay_Hovered() && IsMousePressed(L);
  };

  LAMBDA (bool, rightClicked, ()) {  ///
    return !draw && Clay_Hovered() && IsMousePressed(R);
  };

  struct ComponentButtonData {  ///
    Clay_ElementId id                = {};
    bool           selected          = false;
    bool           growX             = false;
    u16            paddingHorizontal = GAP_BIG;
    bool           enabled           = true;
  };

  LAMBDA (
    bool,
    componentButton,
    (ComponentButtonData                             data,
     auto /* void (bool hovered, Color textColor) */ innerLambda)
  )
  {  ///
    ASSERT(data.id.id);
    bool result = false;

    Clay_Sizing sizing{};
    if (data.growX)
      sizing.width = CLAY_SIZING_GROW(0);

    CLAY({.id = data.id, .layout{.sizing{sizing}}}) {
      CLAY({
        .layout{
          BF_CLAY_SIZING_GROW_XY,
          .padding{
            .left   = data.paddingHorizontal,
            .right  = data.paddingHorizontal,
            .top    = GAP_SMALL,
            .bottom = GAP_SMALL,
          },
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        },
        BF_CLAY_CUSTOM_NINE_SLICE(
          glib->ui_button_nine_slice(),
          (Clay_Hovered() && data.enabled
             ? palWhite
             : (data.selected ? TRANSPARENT_BLACK : slotColors[0])),
          (Clay_Hovered() && data.enabled
             ? TRANSPARENT_BLACK
             : (data.selected ? TRANSPARENT_BLACK : slotColors[1]))
        ),
      }) {
        bool hovered = Clay_Hovered() && data.enabled;
        CLAY({.layout{BF_CLAY_SIZING_GROW_XY, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
          innerLambda(hovered, (hovered ? palTextBlack : palTextWhite));
        }
      }

      ButtonSFX(draw, data.id, Clay_Hovered() && data.enabled);

      result = clicked();
    }

    return result;
  };

  LAMBDA (bool, componentButtonReroll, (int price)) {  ///
    return componentButton(
      {.id = CLAY_ID("button_shop_reroll")},
      [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
        CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
          BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_REROLL, textColor);

          ASSERT(price >= 0);

          if (price > 0) {
            BF_CLAY_TEXT(" - ", textColor);
            BF_CLAY_TEXT(
              TextFormat("%d ", price), ((price <= PLAYER_COINS) ? textColor : palTextRed)
            );
            BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
          }
        }
      }
    );
  };

  struct ComponentSlotData {  ///
    bool hidden     = {};
    bool canHover   = {};
    int  tier       = {};
    f32  flashWhite = 0;
  };

  LAMBDA (void, componentSlot, (ComponentSlotData data, auto innerLambda)) {  ///
    const int texId        = glib->ui_item_slot_texture_id();
    auto      originalSize = original_texture_sizes->Get(texId);

    CLAY({.layout{.sizing{
      .width  = CLAY_SIZING_FIXED((f32)originalSize->x() * ASSETS_TO_LOGICAL_RATIO),
      .height = CLAY_SIZING_FIXED((f32)originalSize->y() * ASSETS_TO_LOGICAL_RATIO),
    }}})
    if (!data.hidden) {
      auto color = slotColors[2 * data.tier];
      auto flash = ColorLerp(slotColors[2 * data.tier + 1], palWhite, data.flashWhite);
      if (data.canHover && Clay_Hovered()) {
        color = palWhite;
        flash = TRANSPARENT_BLACK;
      }

      BF_CLAY_IMAGE({.texId = texId, .color = color, .flash = flash}, innerLambda);
    }
  };

  LAMBDA (void, componentStats, ()) {  ///
    CLAY({
      .layout{
        .sizing{.width = CLAY_SIZING_FIXED(256)},
        BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE_FRAME),
        .childGap        = GAP_BIG,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_NINE_SLICE(
        glib->ui_frame_nine_slice(), slotColors[0], slotColors[1]
      ),
    }) {
      // Stats label.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_STATS);
      }

      // Primary / secondary buttons.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        .childGap = GAP_SMALL,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        FontBegin(&g.meta.fontStats);

        const bool clickedPrimary = componentButton(
          {
            .id                = CLAY_ID("button_stats_primary"),
            .selected          = !g.run.showingSecondaryStats,
            .paddingHorizontal = GAP_SMALL,
          },
          [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_STATS_PRIMARY, textColor);
          }
        );

        const bool clickedSecondary = componentButton(
          {
            .id                = CLAY_ID("button_stats_secondary"),
            .selected          = g.run.showingSecondaryStats,
            .paddingHorizontal = GAP_SMALL,
          },
          [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_STATS_SECONDARY, textColor);
          }
        );

        FontEnd();

        if (clickedPrimary && g.run.showingSecondaryStats) {
          PlaySound(Sound_UI_CLICK);
          g.run.showingSecondaryStats = false;
        }
        if (clickedSecondary && !g.run.showingSecondaryStats) {
          PlaySound(Sound_UI_CLICK);
          g.run.showingSecondaryStats = true;
        }
      }

      LAMBDA (
        void, componentStatsEntry, (int iconTexId, int locale, int value, StatType stat)
      )
      {
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_X,
          BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        }}) {
          // Increasing stat by clicking on it in debug mode.
          if (stat && ge.meta.debugEnabled) {
            if (clicked()) {
              g.run.state.statsWithoutItems[stat]++;
              g.run.recalculatePlayerStats = true;
            }
            if (rightClicked()) {
              g.run.state.statsWithoutItems[stat] += 10;
              g.run.recalculatePlayerStats = true;
            }
            if (wheel && Clay_Hovered()) {
              g.run.state.statsWithoutItems[stat] += wheel;
              g.run.recalculatePlayerStats = true;
            }
          }

          const auto fb = fb_stats->Get(stat);

          if (iconTexId) {
            // Icon.
            CLAY({.layout{.sizing{.width = CLAY_SIZING_FIXED(20)}}}) {
              CLAY({.floating{
                .zIndex = zIndex,
                .attachPoints{
                  .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                  .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
                },
                .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                .attachTo           = CLAY_ATTACH_TO_PARENT,
              }}) {
                BF_CLAY_IMAGE({.texId = iconTexId});
              }
            }

            BF_CLAY_TEXT(" ");
          }

          FontBegin(&g.meta.fontStats);

          if (fb->is_percent())
            BF_CLAY_TEXT("% ");
          BF_CLAY_TEXT_LOCALIZED_DANGER(locale);
          BF_CLAY_SPACER_HORIZONTAL;
          BF_CLAY_TEXT(" ");
          BF_CLAY_TEXT(TextFormat("%d", value));

          FontEnd();
        }
      };

      // Current level.
      if (!g.run.showingSecondaryStats) {
        componentStatsEntry(
          glib->ui_shop_current_level_icon_texture_id(),
          Loc_UI_CURRENT_LEVEL,
          g.run.state.level,
          StatType_INVALID
        );
      }

      // Stats.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        .childGap        = GAP_SMALL,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }})
      FOR_RANGE (int, i, (int)StatType_COUNT - 2) {
        const auto type = (StatType)(i + 2);
        const auto fb   = glib->stats()->Get(type);
        if (!fb->is_hidden() && (fb->is_secondary() == g.run.showingSecondaryStats)) {
          componentStatsEntry(
            fb->icon_texture_id(), fb->name_locale(), g.run.playerStats[type], type
          );
        }
      }
    }
  };

  enum ComponentUniversalSlotHiddenType {  ///
    ComponentUniversalSlotHiddenType_HIDE,
    ComponentUniversalSlotHiddenType_SHOW_EMPTY_SLOT,
    ComponentUniversalSlotHiddenType_SHOW_LOCK,
  };

  struct ComponentUniversalSlotData {  ///
    Clay_ElementId id = {};

    DifficultyType difficulty = {};
    BuildType      build      = {};
    ItemType       item       = {};
    WeaponType     weapon     = {};

    ComponentUniversalSlotHiddenType hidden = ComponentUniversalSlotHiddenType_HIDE;

    int  count    = 1;
    int  tier     = -1;
    bool canHover = true;
  };

  LAMBDA (bool, componentUniversalSlot, (ComponentUniversalSlotData data)) {  ///
    ASSERT(data.count > 0);
    if (data.canHover)
      ASSERT(data.id.id);

    // Checking that none or only one of build/item/weapon is specified.
    const int onlyOneOrNone = (int)(data.difficulty != 0) + (int)(data.build != 0)
                              + (int)(data.item != 0) + (int)(data.weapon != 0);
    ASSERT(onlyOneOrNone <= 1);

    int texId = 0;
    int tier  = 0;
    if (data.difficulty) {
      texId = fb_difficulties->Get(data.difficulty)->texture_id();
      tier  = (int)data.difficulty - 1;
    }
    if (data.build) {
      texId = fb_builds->Get(data.build)->texture_id();
      tier  = GetBuildTier(data.build);
    }
    if (data.item) {
      auto fbx = fb_items->Get(data.item);
      texId    = fbx->texture_id();
      tier     = fbx->tier();
    }
    if (data.weapon) {
      auto fbx = fb_weapons->Get(data.weapon);
      texId    = fbx->icon_texture_id();
      tier     = fbx->min_tier_index();
    }
    if (data.tier >= 0)
      tier = data.tier;

    bool result = false;

    componentSlot(
      {
        .hidden
        = (!onlyOneOrNone && (data.hidden == ComponentUniversalSlotHiddenType_HIDE)),
        .canHover = data.canHover,
        .tier     = tier,
      },
      [&]() BF_FORCE_INLINE_LAMBDA {
        if (data.canHover) {
          ButtonSFX(draw, data.id, Clay_Hovered());
          if (clicked())
            result = true;
        }

        if (!onlyOneOrNone) {
          if (data.hidden == ComponentUniversalSlotHiddenType_SHOW_LOCK)
            texId = glib->ui_item_locked_texture_id();
          else
            return;
        }

        CLAY({.layout{BF_CLAY_SIZING_GROW_XY, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
          BF_CLAY_IMAGE({.texId = texId});

          // Showing count if there are multiple of the same item.
          if (data.count > 1) {
            CLAY({.floating{
              .zIndex = zIndex,
              .attachPoints{
                .element = CLAY_ATTACH_POINT_RIGHT_BOTTOM,
                .parent  = CLAY_ATTACH_POINT_RIGHT_BOTTOM,
              },
              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
              .attachTo           = CLAY_ATTACH_TO_PARENT,
            }}) {
              FLOATING_BEAUTIFY;
              FontBegin(&g.meta.fontItemCountsOutlined);
              BF_CLAY_TEXT(TextFormat("x%d", data.count));
              FontEnd();
            }
          }
        }
      }
    );

    return result;
  };

  LAMBDA (void, componentEffectsExploded, (auto fb_effects, int count, int maxWidth))
  {  ///
    ASSERT(count > 0);
    if (!fb_effects)
      return;

    for (const auto fb_effect : *fb_effects) {
      CLAY({.layout{
        .childGap = GAP_FLEX,
        BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        FlexBegin(maxWidth, 0);

        f32 v = 0;

        if (fb_effect->stat_type()) {
          const auto fb_stat = fb_stats->Get(fb_effect->stat_type());

          // if (fb_stat->icon_texture_id())
          //   BF_CLAY_IMAGE({.texId = fb_stat->icon_texture_id()});

          PlaceholdBrokenLocale("STAT", fb_stat->name_locale());

          bool isPercent = fb_stat->is_percent();
          v              = (f32)fb_effect->value();
          if (v == 0) {
            v         = (fb_effect->value_multiplier() - 1.0f) * 100.0f;
            isPercent = true;
          }
          v *= count;

          const bool  isPositive = v >= 0;
          const char* format     = nullptr;
          if (isPercent)
            format = (isPositive ? "+%s%%" : "%s%%");
          else
            format = (isPositive ? "+%s" : "%s");
          PlaceholdString(
            "MODIFIER",
            TextFormat(format, StripLeadingZerosInFloat(TextFormat("%.1f", v))),
            (isPositive == fb_stat->negative_is_good() ? palTextRed : palTextGreen)
          );
        }
        else if (fb_effect->weaponproperty_type()) {
          const auto fb_prop
            = fb_weapon_properties->Get(fb_effect->weaponproperty_type());
          PlaceholdBrokenLocale("PROPERTY", fb_prop->name_locale());

          v              = (f32)fb_effect->value();
          bool isPercent = false;
          if (v == 0) {
            v         = (fb_effect->value_multiplier() - 1.0f) * 100.0f;
            isPercent = true;
          }
          v *= count;

          // TODO: correct colors.
          const char* format     = nullptr;
          bool        isPositive = (v >= 0);
          if (isPercent)
            format = (isPositive ? "+%s%%" : "%s%%");
          else
            format = (isPositive ? "+%s" : "%s");

          PlaceholdString(
            "MODIFIER",
            TextFormat(format, StripLeadingZerosInFloat(TextFormat("%.1f", v))),
            palTextGreen
          );
        }
        else
          INVALID_PATH;

        const auto cond = fb_effect->effectcondition_type();
        if (cond)
          clayEffectConditionFunctions[cond - 1](fb_effect);
        else if (!FloatEquals(v, 0))
          BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(Loc_EFFECT_DEFAULT_WEAPON_STAT);

        FlexEnd();
      }
    }
  };

  struct ComponentUniversalCardData {  ///
    DifficultyType difficulty = {};
    BuildType      build      = {};
    ItemType       item       = {};
    WeaponType     weapon     = {};

    int count               = 1;
    int weaponIndexOrMinus1 = -1;

    bool hideIfEmpty    = false;
    bool affectedByGame = false;
    int  overrideTier   = -1;

    bool setFixedHeight = false;

    int  shopBuyingIndex = -1;
    bool shopSelling     = false;
  };

  LAMBDA (void, componentUniversalCard, (ComponentUniversalCardData data)) {
    // Setup. {  ///
    const int atLeastOneIsSpecified
      = (int)(data.weapon != 0) + (int)(data.item != 0) + (int)(data.build != 0);
    ASSERT(atLeastOneIsSpecified <= 1);

    const auto fb_difficulty
      = (data.difficulty ? fb_difficulties->Get(data.difficulty) : nullptr);
    const auto fb_build  = (data.build ? fb_builds->Get(data.build) : nullptr);
    const auto fb_item   = (data.item ? fb_items->Get(data.item) : nullptr);
    const auto fb_weapon = (data.weapon ? fb_weapons->Get(data.weapon) : nullptr);

    int tier = data.overrideTier;
    if (tier == -1) {
      if (fb_difficulty)
        tier = (int)data.difficulty - 1;
      else if (fb_build)
        tier = GetBuildTier(data.build);
      else if (fb_item)
        tier = fb_item->tier();
      else if (fb_weapon)
        tier = fb_weapon->min_tier_index();
      else
        tier = 0;
    }

    int price        = 0;
    int recyclePrice = 0;
    if (fb_weapon) {
      price        = GetWeaponPrice(data.weapon, tier);
      recyclePrice = GetWeaponRecyclePrice(data.weapon, tier);
    }
    if (fb_item) {
      price        = fb_item->price();
      recyclePrice = price / RECYCLE_PRICE_FACTOR;
    }
    price = ApplyStatItemsPrice(price);

    bool canBuy                     = (price <= PLAYER_COINS);
    bool canBuyErrorWeapon          = false;
    int  emptyOrSameWeaponSlotIndex = -1;

    if (canBuy && data.weapon) {
      // Trying to find empty weapon slot.
      FOR_RANGE (int, i, g.run.state.weapons.count) {
        const auto& weapon = g.run.state.weapons[i];
        if (!weapon.type) {
          emptyOrSameWeaponSlotIndex = i;
          break;
        }
      }
      // If not found, trying to find the same player's weapon to upgrade.
      if (emptyOrSameWeaponSlotIndex == -1) {
        FOR_RANGE (int, i, g.run.state.weapons.count) {
          const auto& weapon        = g.run.state.weapons[i];
          const bool  canBeUpgraded = (weapon.type == data.weapon)  //
                                     && (weapon.tier == tier)       //
                                     && (tier < TOTAL_TIERS - 1);
          if (canBeUpgraded) {
            emptyOrSameWeaponSlotIndex = i;
            break;
          }
        }
      }
      if (emptyOrSameWeaponSlotIndex == -1) {
        canBuyErrorWeapon = true;
        canBuy            = false;
      }
    }
    // }  ///

    // CLAY(...) {  ///
    CLAY({.layout{.sizing{
      .width = CLAY_SIZING_FIXED(CARD_WIDTH + 2 * PADDING_NINE_SLICE_FRAME),
      .height
      = (data.setFixedHeight ? CLAY_SIZING_FIXED(CARD_HEIGHT) : CLAY_SIZING_FIT(0)),
    }}})
    // }
    if (!data.hideIfEmpty || atLeastOneIsSpecified) {
      // CLAY(...) {  ///
      CLAY({
        .layout{
          BF_CLAY_SIZING_GROW_XY,
          BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE_FRAME),
          .childGap        = GAP_SMALL,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        BF_CLAY_CUSTOM_NINE_SLICE(
          glib->ui_frame_nine_slice(), slotColors[2 * tier], slotColors[2 * tier + 1]
        ),
      })
      // }
      {
        // Icon, name.
        CLAY({.layout{.childGap = GAP_SMALL}}) {
          // Icon. {  ///
          componentUniversalSlot({
            .difficulty = data.difficulty,
            .build      = data.build,
            .item       = data.item,
            .weapon     = data.weapon,
            .count      = data.count,
            .tier       = tier,
            .canHover   = false,
          });
          // }

          // Name, secondary stuff.
          CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            if (data.difficulty) {  ///
              BF_CLAY_TEXT_LOCALIZED_DANGER(
                fb_difficulty->name_locale(), textColorsPerTier[tier]
              );

              FontBegin(&g.meta.fontStats);
              BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_DIFFICULTY, secondaryTextColor);
              FontEnd();
            }
            if (data.build) {  ///
              BF_CLAY_TEXT_LOCALIZED_DANGER(
                fb_build->name_locale(), textColorsPerTier[tier]
              );
              FontBegin(&g.meta.fontStats);
              const auto d = g.player.builds[data.build].maxDifficultyBeaten;
              if (d > 0) {
                CLAY({}) {
                  auto dd = fb_difficulties->Get(d);
                  BF_CLAY_TEXT_LOCALIZED_DANGER(dd->name_locale(), secondaryTextColor);
                }
              }
              else
                BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_BUILD, secondaryTextColor);
              FontEnd();
            }
            if (data.item) {  ///
              BF_CLAY_TEXT_LOCALIZED_DANGER(
                fb_item->name_locale(), textColorsPerTier[tier]
              );

              FontBegin(&g.meta.fontStats);
              if (fb_item->count_cap() > 0) {
                if (fb_item->count_cap() > 1) {
                  CLAY({}) {
                    BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_LIMITED, secondaryTextColor);

                    if (data.affectedByGame) {
                      int currentCount = 0;
                      for (auto& x : g.run.state.items) {
                        if (x.type == data.item) {
                          currentCount = x.count;
                          break;
                        }
                      }
                      BF_CLAY_TEXT(
                        TextFormat(" (%d/%d)", currentCount, fb_item->count_cap()),
                        secondaryTextColor
                      );
                    }
                    else {
                      BF_CLAY_TEXT(
                        TextFormat(" (%d)", fb_item->count_cap()), secondaryTextColor
                      );
                    }
                  }
                }
                else
                  BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_UNIQUE, secondaryTextColor);
              }
              else
                BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_ITEM, secondaryTextColor);
              FontEnd();
            }
            if (data.weapon) {  ///
              BF_CLAY_TEXT_LOCALIZED_DANGER(
                fb_weapon->name_locale(), textColorsPerTier[tier]
              );
              FontBegin(&g.meta.fontStats);
              BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_WEAPON, secondaryTextColor);
              FontEnd();
            }
          }
        }

        FontBegin(&g.meta.fontStats);

        // Difficulty stats.
        if (data.difficulty) {  ///
          componentEffectsExploded(fb_difficulty->effects(), data.count, CARD_WIDTH);
        }
        // Build stats.
        if (data.build) {  ///
          componentEffectsExploded(fb_build->effects(), data.count, CARD_WIDTH);
        }
        // Item stats.
        if (data.item) {  ///
          componentEffectsExploded(
            fb_items->Get(data.item)->effects(), data.count, CARD_WIDTH
          );
        }
        // Weapon stats.
        if (data.weapon) {  ///
          int thisWaveDamage = 0;
          if (data.weaponIndexOrMinus1 >= 0)
            thisWaveDamage = g.run.state.weapons[data.weaponIndexOrMinus1].thisWaveDamage;

          LAMBDA (void, componentWeaponStatEntry, (int labelLocale, auto&& innerLambda)) {
            CLAY({.layout{
              .childGap = GAP_FLEX,
              BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              FlexBegin(CARD_WIDTH, 0);
              BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(labelLocale, secondaryTextColor);
              BF_CLAY_TEXT(": ", secondaryTextColor);
              innerLambda();
              FlexEnd();
            }
          };

          const auto fb = fb_weapons->Get(data.weapon);

          // Damage.
          componentWeaponStatEntry(
            fb_stats->Get(StatType_DAMAGE)->name_locale(),
            [&]() BF_FORCE_INLINE_LAMBDA {
              const int baseDamage = fb->base_damage()->Get(tier - fb->min_tier_index());
              if (data.affectedByGame) {
                const int actualDamage
                  = CalculateWeaponDamage(data.weaponIndexOrMinus1, data.weapon, tier);
                BF_CLAY_TEXT(TextFormat("%d", actualDamage), palTextGreen);
                BF_CLAY_TEXT(TextFormat(" (%d)", baseDamage), secondaryTextColor);
              }
              else
                BF_CLAY_TEXT(TextFormat("%d", baseDamage), palTextGreen);

              // Scalings.
              const auto fb_scalings = fb->damage_scalings();
              if (fb_scalings && fb_scalings->size()) {
                BF_CLAY_TEXT(" (");
                FOR_RANGE (int, scalingIndex, fb_scalings->size()) {
                  const auto fb_scaling = fb_scalings->Get(scalingIndex);
                  const auto fb_stat    = fb_stats->Get(fb_scaling->stat_type());
                  BF_CLAY_TEXT(TextFormat(
                    "+%d%%",
                    fb_scaling->percents_per_tier()->Get(tier - fb->min_tier_index())
                  ));
                  BF_CLAY_IMAGE({.texId = fb_stat->icon_texture_id()});
                  if (scalingIndex < fb_scalings->size() - 1)
                    BF_CLAY_TEXT(", ");
                }
                BF_CLAY_TEXT(")");
              }
            }
          );

          // Critical.
          if (data.affectedByGame) {
            componentWeaponStatEntry(Loc_UI_CRIT, [&]() BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT(TextFormat(
                "x%.1f (%d%%)",
                fb->crit_damage_multiplier(),
                MIN(100, MAX(0, g.run.playerStats[StatType_CRIT_CHANCE]))
              ));
            });
          }
          else {
            componentWeaponStatEntry(Loc_UI_CRIT, [&]() BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT(TextFormat("x%.1f", fb->crit_damage_multiplier()));
            });
          }

          // Cooldown.
          componentWeaponStatEntry(Loc_UI_COOLDOWN, [&]() BF_FORCE_INLINE_LAMBDA {
            auto cooldownFrames_
              = (fb->shooting_duration_frames() + fb->cooldown_frames());
            lframe cooldownFrames{};
            if (data.affectedByGame)
              cooldownFrames = ApplyAttackSpeedToDuration(cooldownFrames_);
            else
              cooldownFrames = lframe::Unscaled(cooldownFrames_);
            const f32 cooldownSeconds = (f32)cooldownFrames.value / (f32)FIXED_FPS;
            BF_CLAY_TEXT(TextFormat("%.2fs", cooldownSeconds), palTextGreen);
          });

          // Knockback.
          {
            f32 value = fb->knockback_meters();
            if (data.affectedByGame)
              value *= (f32)(100 + g.run.playerStats[StatType_KNOCKBACK]) / 100.0f;
            componentWeaponStatEntry(Loc_UI_KNOCKBACK, [&]() BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT(StripLeadingZerosInFloat(TextFormat("%.1f", value)));
            });
          }

          // Range.
          componentWeaponStatEntry(Loc_UI_RANGE, [&]() BF_FORCE_INLINE_LAMBDA {
            const f32 rangeMeters
              = GetWeaponRangeMeters(data.weapon, data.affectedByGame);
            if (fb->projectile_type())
              BF_CLAY_TEXT(TextFormat("%.1f", rangeMeters));
            else {
              const f32 weaponRangeMeters
                = (f32)glib->original_texture_sizes()->Get(fb->texture_ids()->Get(0))->x()
                  * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;
              BF_CLAY_TEXT(TextFormat("%.1f + %.1f", weaponRangeMeters, rangeMeters));
            }
          });

          // Pierce.
          if (fb->projectile_type()) {
            int value = fb->projectile_pierce();
            if (data.affectedByGame)
              value += g.run.playerStats[StatType_PIERCING];
            if (value > 0) {
              componentWeaponStatEntry(Loc_UI_PIERCE, [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT(TextFormat("%d", value));
              });
            }
          }

          // Bounce.
          if (fb->projectile_type()) {
            int value = fb->projectile_bounce();
            if (data.affectedByGame)
              value += g.run.playerStats[StatType_BOUNCES];
            if (value > 0) {
              componentWeaponStatEntry(Loc_UI_BOUNCE, [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT(TextFormat("%d", value));
              });
            }
          }

          // Life Steal.
          {
            auto chance = GetLifestealChance(data.weapon, data.affectedByGame);
            if (chance > 0) {
              componentWeaponStatEntry(
                fb_stats->Get(StatType_LIFE_STEAL)->name_locale(),
                [&]() BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT(
                    TextFormat(
                      "%s%%",
                      StripLeadingZerosInFloat(TextFormat("%.1f", chance * 100.0f))
                    ),
                    palTextGreen
                  );
                }
              );
            }
          }

          // Explosion.
          {
            auto chance = fb_projectiles->Get(fb->projectile_type())->aoe_chance();
            if (chance > 0) {
              CLAY({.layout{
                .childGap        = GAP_FLEX,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              }}) {
                FlexBegin(CARD_WIDTH, 0);

                PlaceholdString(
                  "CHANCE",
                  TextFormat(
                    "+%s%%", StripLeadingZerosInFloat(TextFormat("%.1f", chance * 100.0f))
                  )
                );
                BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(Loc_WEAPON_EFFECT_CHANCE_OF_EXPLOSION
                );

                FlexEnd();
              }
            }
          }

          componentEffectsExploded(fb->effects(), 1, CARD_WIDTH);

          // This wave damage.
          ASSERT(thisWaveDamage >= 0);
          if (thisWaveDamage > 0) {
            componentWeaponStatEntry(
              Loc_UI_THIS_WAVE_DAMAGE,
              [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT(TextFormat("%d", thisWaveDamage), palTextWhite);
              }
            );
          };
        }

        FontEnd();

        BF_CLAY_SPACER_VERTICAL;

        if (data.shopBuyingIndex >= 0) {  ///
          CLAY({.layout{
            BF_CLAY_SIZING_GROW_X,
            BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
          }}) {
            // Buying item / weapon.
            auto bought = componentButton(
              {.id = CLAY_IDI("button_shop_buy", data.shopBuyingIndex)},
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                CLAY({.layout{
                  BF_CLAY_SIZING_GROW_X,
                  BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                }}) {
                  FontBegin(&g.meta.fontPrices);
                  BF_CLAY_TEXT(
                    TextFormat("%d ", price),
                    (price <= PLAYER_COINS ? textColor : palTextRed)
                  );
                  FontEnd();
                  BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
                }
              }
            );
            if (!draw
                && IsKeyPressed((SDL_Scancode)((int)SDL_SCANCODE_1 + data.shopBuyingIndex)
                ))
              bought = true;

            if (bought) {
              if (canBuy) {
                PlaySound(Sound_UI_CLICK);

                ChangeCoins(-price);
                if (data.weapon) {
                  auto& weapon = g.run.state.weapons[emptyOrSameWeaponSlotIndex];
                  if (weapon.type) {
                    // Upgrading an existing weapon if no empty slot found.
                    ASSERT(weapon.type == data.weapon);
                    ASSERT(weapon.tier == tier);
                    weapon.tier += 1;
                  }
                  else {
                    // Filling empty weapon slot if exists.
                    weapon.type = data.weapon;
                    weapon.tier = tier;
                  }
                }
                else if (data.item)
                  AddItem(data.item);
                else
                  INVALID_PATH;

                g.run.state.shop.toPick[data.shopBuyingIndex] = {};
                Save();
              }
              else {
                if (canBuyErrorWeapon) {
                  g.ui.shopErrorWeapons = {};
                  g.ui.shopErrorWeapons.SetNow();
                }
                else {
                  g.ui.shopErrorGold = {};
                  g.ui.shopErrorGold.SetNow();
                }
                PlaySound(Sound_UI_ERROR);
              }
            }
          }
        }

        if (data.shopSelling) {  ///
          CLAY({.layout{
            .childGap        = GAP_SMALL,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          }}) {
            int   canCombineWithIndex = -1;
            auto& weapon              = g.run.state.weapons[data.weaponIndexOrMinus1];
            for (int i = g.run.state.weapons.count - 1; i >= 0; i--) {
              if (i == data.weaponIndexOrMinus1)
                continue;
              auto& otherWeapon = g.run.state.weapons[i];
              if ((weapon.type == otherWeapon.type)     //
                  && (weapon.tier == otherWeapon.tier)  //
                  && (weapon.tier < TOTAL_TIERS - 1))
              {
                canCombineWithIndex = i;
                break;
              }
            }

            // Combine button.
            bool combined = false;
            if (canCombineWithIndex >= 0) {
              combined = componentButton(
                {.id = CLAY_ID("button_weapon_combine")},
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_COMBINE, textColor);
                }
              );
            }

            const int recyclePrice = GetWeaponRecyclePrice(weapon.type, weapon.tier);

            // Recycle button.
            const bool recycled = componentButton(
              {.id = CLAY_ID("button_weapon_recycle")},
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_RECYCLE, textColor);
                BF_CLAY_TEXT(TextFormat(" (+%d)", recyclePrice), textColor);
              }
            );

            // Cancel button.
            const bool cancelled = componentButton(
              {.id = CLAY_ID("button_weapon_cancel")},
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_CANCEL, textColor);
              }
            );

            if (combined) {
              ASSERT(canCombineWithIndex >= 0);
              weapon.tier += 1;
              weapon.thisWaveDamage = 0;
              weapon.killedEnemies  = MAX(
                weapon.killedEnemies,
                g.run.state.weapons[canCombineWithIndex].killedEnemies
              );
              StableRemoveWeapon(canCombineWithIndex);
              Save();
            }
            if (recycled) {
              ChangeCoins(recyclePrice);
              StableRemoveWeapon(data.weaponIndexOrMinus1);
              Save();
            }
            if (cancelled || recycled || combined) {
              PlaySound(Sound_UI_CLICK);
              g.run.shopSelectedWeaponIndex = -1;
            }
          }
        }
      }
    }
  };

  struct ComponentUniversalDetailsData {  ///
    DifficultyType difficulty = {};
    BuildType      build      = {};
    ItemType       item       = {};

    int  count          = 1;
    bool affectedByGame = true;
    int  overrideTier   = -1;

    bool detailsRight = {};
    bool detailsBelow = {};
  };

  LAMBDA (void, componentUniversalDetails, (ComponentUniversalDetailsData data)) {  ///
    ASSERT(data.count > 0);
    int s = (int)(data.difficulty > 0) + (int)(data.build > 0) + (int)(data.item > 0);
    ASSERT(s > 0);

    f32                          offsetY{};
    Clay_FloatingAttachPointType attachElement{};
    Clay_FloatingAttachPointType attachParent{};

    if (data.detailsBelow) {
      offsetY       = GAP_SMALL;
      attachElement = CLAY_ATTACH_POINT_LEFT_TOP;
      attachParent  = CLAY_ATTACH_POINT_LEFT_BOTTOM;
    }
    else {
      offsetY       = -GAP_SMALL;
      attachElement = CLAY_ATTACH_POINT_LEFT_BOTTOM;
      attachParent  = CLAY_ATTACH_POINT_LEFT_TOP;
    }

    zIndex += 2;

    CLAY({
      .layout{
        .sizing{
          CLAY_SIZING_FIXED(CARD_WIDTH + 2 * PADDING_NINE_SLICE_FRAME), CLAY_SIZING_FIT(0)
        },
      },
      .floating{
        .offset{0, offsetY},
        .zIndex = zIndex,
        .attachPoints{.element = attachElement, .parent = attachParent},
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
    }) {
      FLOATING_BEAUTIFY;

      componentUniversalCard({
        .difficulty     = data.difficulty,
        .build          = data.build,
        .item           = data.item,
        .count          = data.count,
        .affectedByGame = data.affectedByGame,
        .overrideTier   = data.overrideTier,
      });
    }

    zIndex -= 2;
  };

  LAMBDA (void, componentAchievement, (AchievementType type, int stepIndex)) {  ///
    auto fb      = fb_achievements->Get(type);
    auto fb_step = (type ? fb->steps()->Get(stepIndex) : nullptr);

    const bool isLocked = !type || (g.player.achievements[type].value < fb_step->value());
    bool       workingOnIt = type && true;
    if (stepIndex > 0) {
      workingOnIt
        = g.player.achievements[type].value >= fb->steps()->Get(stepIndex - 1)->value();
    }

    int tier = (isLocked ? 0 : 3);
    if (isLocked && workingOnIt)
      tier = 1;

    CLAY({
      .layout{
        .sizing{
          .width  = CLAY_SIZING_FIXED(ACHIEVEMENT_WIDTH + 2 * PADDING_NINE_SLICE_FRAME),
          .height = CLAY_SIZING_FIXED(ACHIEVEMENT_HEIGHT + 2 * PADDING_NINE_SLICE_FRAME),
        },
        BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE_FRAME),
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_NINE_SLICE(
        glib->ui_frame_nine_slice(), slotColors[tier * 2], slotColors[tier * 2 + 1]
      ),
    }) {
      auto fb_previousStep
        = (type && (stepIndex > 0) ? fb->steps()->Get(stepIndex - 1) : nullptr);

      // Name.
      CLAY({}) {
        const auto nameColor = textColorsPerTier[tier];

        if (type) {
          if (g.player.achievements[type].value >= fb_step->value()) {
            BF_CLAY_TEXT_LOCALIZED_DANGER(fb->name_locale(), nameColor);
            static const char* romanNumbers_[]{
              "",   "I",   "II",   "III", "IV", "V",   "VI",   "VII",   "VIII", "IX", "X",
              "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX",  "XX",
            };
            VIEW_FROM_ARRAY_DANGER(romanNumbers);
            BF_CLAY_TEXT(" ", nameColor);
            BF_CLAY_TEXT(romanNumbers[stepIndex + 1], nameColor);
          }
          else
            BF_CLAY_TEXT("???", nameColor);
        }
      }

      BF_CLAY_SPACER_VERTICAL;
      if (type) {
        // Description.
        CLAY({.layout{
          .sizing{.height = CLAY_SIZING_GROW(0)},
          .childGap = GAP_FLEX,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          FontBegin(&g.meta.fontStats);

          const auto v = g.player.achievements[type].value;

          if (!fb_previousStep || (v >= fb_previousStep->value())) {
            FlexBegin(ACHIEVEMENT_WIDTH, 0);
            if (type == AchievementType_DIFFICULTY) {
              PlaceholdString(
                "VALUE",
                localization_strings
                  ->Get(fb_difficulties->Get(stepIndex + 1)->name_locale())
                  ->c_str()
              );
            }
            else
              PlaceholdString("VALUE", TextFormat("%d", fb_step->value()));
            BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(fb->description_locale());
            int percent = MIN(100, v * 100 / fb_step->value());
            if ((percent < 100) && (fb_step->value() > 1)) {
              BF_CLAY_TEXT(" ");
              BF_CLAY_TEXT(TextFormat("(%d / %d)", v, fb_step->value()), palTextBezhevy);
            }
            FlexEnd();
          }
          else
            BF_CLAY_TEXT("???");
          FontEnd();
        }
      }
    }
  };

  LAMBDA (void, componentCenterFloater, (auto innerLambda)) {  ///
    CLAY({.floating{
      .zIndex = zIndex,
      .attachPoints{
        .element = CLAY_ATTACH_POINT_CENTER_CENTER,
        .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
      },
      .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
      .attachTo           = CLAY_ATTACH_TO_PARENT,
    }}) {
      FLOATING_BEAUTIFY;
      innerLambda();
    }
  };

  LAMBDA (
    void,
    componentWeaponDetails,
    (WeaponType type,
     int        weaponIndexOrMinus1,
     bool       weAreInShop,
     bool       detailsBelow,
     bool       affectedByGame = true)
  )
  {  ///
    ASSERT(type);

    int tier = 0;
    if (weaponIndexOrMinus1 >= 0) {
      auto& weapon = g.run.state.weapons[weaponIndexOrMinus1];
      ASSERT(type == weapon.type);
      tier = weapon.tier;
    }
    else
      tier = fb_weapons->Get(type)->min_tier_index();

    // Floating weapon details modal.
    // Gets shown upon hovering. Gets sticked upon clicking on weapon.
    if (Clay_Hovered() || (g.run.shopSelectedWeaponIndex == weaponIndexOrMinus1)) {
      if (weAreInShop && (g.run.shopSelectedWeaponIndex == weaponIndexOrMinus1)) {
        // Pressing ESC closes modal.
        if (IsKeyPressed(SDL_SCANCODE_ESCAPE))
          g.run.shopSelectedWeaponIndex = -1;
      }

      f32                          offsetY{};
      Clay_FloatingAttachPointType attachElement{};
      Clay_FloatingAttachPointType attachParent{};

      if (detailsBelow) {
        offsetY       = GAP_SMALL;
        attachElement = CLAY_ATTACH_POINT_LEFT_TOP;
        attachParent  = CLAY_ATTACH_POINT_LEFT_BOTTOM;
      }
      else {
        offsetY       = -GAP_SMALL;
        attachElement = CLAY_ATTACH_POINT_LEFT_BOTTOM;
        attachParent  = CLAY_ATTACH_POINT_LEFT_TOP;
      }

      zIndex += 2;

      CLAY({
        .layout{
          .sizing{
            CLAY_SIZING_FIXED(CARD_WIDTH + 2 * PADDING_NINE_SLICE_FRAME),
            CLAY_SIZING_FIT(0)
          },
        },
        .floating{
          .offset{0, offsetY},
          .zIndex = zIndex,
          .attachPoints{.element = attachElement, .parent = attachParent},
          .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE,
          .attachTo           = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        if (weAreInShop && (g.run.shopSelectedWeaponIndex == weaponIndexOrMinus1))
          componentOverlay();

        componentUniversalCard({
          .weapon              = type,
          .weaponIndexOrMinus1 = weaponIndexOrMinus1,
          .affectedByGame      = affectedByGame,
          .overrideTier        = tier,
          .shopSelling         = weAreInShop,
        });
      }

      zIndex -= 2;
    }
  };

  struct ComponentItemsGridData {  ///
    int  itemsX       = {};
    bool detailsBelow = false;
  };

  LAMBDA (void, componentItemsGrid, (ComponentItemsGridData data)) {  ///
    ASSERT(data.itemsX > 0);

    CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
      const int ITEMS_X = data.itemsX;
      const int ITEMS_Y = CeilDivision(g.run.state.items.count + 2, ITEMS_X);

      FOR_RANGE (int, y, ITEMS_Y) {
        CLAY({.layout{.childGap = GAP_SMALL}})
        FOR_RANGE (int, x, ITEMS_X) {
          const int t = y * ITEMS_X + x;
          if (t >= g.run.state.items.count + 2)
            break;
          CLAY({}) {
            int            itemCount      = 1;
            DifficultyType difficultyType = {};
            BuildType      buildType      = {};
            ItemType       itemType       = {};

            if (t == 0) {
              componentUniversalSlot({
                .id    = CLAY_IDI("shop_player_item", t),
                .build = g.player.build,
              });
              buildType = g.player.build;
            }
            else if (t == 1) {
              componentUniversalSlot({
                .id         = CLAY_IDI("shop_player_item", t),
                .difficulty = g.player.difficulty,
              });
              difficultyType = g.player.difficulty;
            }
            else {
              auto& item = g.run.state.items[t - 2];
              componentUniversalSlot({
                .id    = CLAY_IDI("shop_player_item", t),
                .item  = item.type,
                .count = item.count,
              });
              itemType  = item.type;
              itemCount = item.count;
            }

            if (Clay_Hovered()) {
              componentUniversalDetails({
                .difficulty   = difficultyType,
                .build        = buildType,
                .item         = itemType,
                .count        = itemCount,
                .detailsRight = 1,
                .detailsBelow = data.detailsBelow,
              });
              if (ge.meta.debugEnabled && itemType) {
                auto& item = g.run.state.items[t - 2];
                if (clicked())
                  item.count++;
                if (wheel && Clay_Hovered()) {
                  item.count += wheel;
                  item.count = MAX(1, item.count);
                }
              }
            }
          }
        }
      }
    }
  };

  // Gameplay.
  if ((g.run.state.screen == ScreenType_GAMEPLAY)               //
      || (g.run.state.screen == ScreenType_WAVE_END_ANIMATION)  //
      || (g.run.state.screen == ScreenType_PICKED_UP_ITEM)      //
      || (g.run.state.screen == ScreenType_UPGRADES))
  {
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        )
      },
      .floating{
        .zIndex             = zIndex,
        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
        .attachTo           = CLAY_ATTACH_TO_PARENT,
      },
    })
    CLAY({.layout{BF_CLAY_SIZING_GROW_XY}}) {
      FLOATING_BEAUTIFY;

      FontBegin(&g.meta.fontUIOutlined);

      // Health bar + coins + not picked up coins.
      // {  ///
      const auto  texs   = glib->ui_health_player_texture_ids();
      const auto& player = PLAYER_CREATURE;

      CLAY({
        .layout{
          .childGap        = GAP_SMALL,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .floating{
          .zIndex = zIndex,
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
              .zIndex = zIndex,
              .attachPoints{
                .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
              },
              .attachTo = CLAY_ATTACH_TO_PARENT,
            },
          }) {
            FLOATING_BEAUTIFY;

            const auto health = MAX(0, PLAYER_CREATURE.health);

            BF_CLAY_IMAGE({
              .texId = texs->Get(1),
              .sourceMargins{
                .right = Clamp01(1.0f - (f32)health / (f32)PLAYER_CREATURE.maxHealth)
              },
              .color = palRed,
            });

            CLAY({
              .floating{
                .zIndex = zIndex,
                .attachPoints{
                  .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                  .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
                },
                .attachTo = CLAY_ATTACH_TO_PARENT,
              },
            }) {
              FLOATING_BEAUTIFY;
              BF_CLAY_TEXT(
                TextFormat("%d / %d", Round(health), Round(PLAYER_CREATURE.maxHealth))
              );
            }
          }
        });

        BF_CLAY_IMAGE({.texId = texs->Get(0)}, [&]() BF_FORCE_INLINE_LAMBDA {
          CLAY({
            .floating{
              .zIndex = zIndex,
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
                .right = 1 - (f32)g.run.state.xp / (f32)GetNextLevelXp(g.run.state.level)
              },
              .color = palGreen,
            });

            CLAY({
              .floating{
                .zIndex = zIndex,
                .attachPoints{
                  .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                  .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
                },
                .attachTo = CLAY_ATTACH_TO_PARENT,
              },
            }) {
              FLOATING_BEAUTIFY;
              BF_CLAY_TEXT(TextFormat("%d", g.run.state.level));
            }
          }
        });

        {
          BEAUTIFY_WIGGLING_DANGER_SCOPED(
            g.ui.upgradesErrorGold,
            GOLD_WIGGLING_LOGICAL_AMPLITUDE,
            ERROR_WIGGLING_FRAMES,
            ERROR_WIGGLING_TIMES
          );

          CLAY({.layout{
            .childGap = GAP_SMALL,
            BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
          }}) {
            BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
            BF_CLAY_TEXT(TextFormat("%d", PLAYER_COINS));
          }
        }

        CLAY({.layout{
          .childGap = GAP_SMALL,
          BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        }}) {
          f32        fade = (g.run.state.notPickedUpCoinsVisual > 0 ? 1 : 0);
          const auto id   = CLAY_ID("notPickedUpCoinsVisual");
          CLAY({.id = id}) {
            BF_CLAY_IMAGE({
              .texId = glib->ui_coin_x2_texture_id(),
              .color = Fade(WHITE, fade),
            });
          }
          const auto d = Clay_GetElementData(id);
          if (d.found) {
            const auto& bb = d.boundingBox;
            g.ui.notPickedUpCoinsLogicalPos
              = {bb.x + bb.width / 2, LOGICAL_RESOLUTION.y - bb.y - bb.height / 2};
          }

          BF_CLAY_TEXT(
            TextFormat("%d", g.run.state.notPickedUpCoinsVisual), Fade(palTextWhite, fade)
          );
        }
      }
      // }

      // Wave.
      // { ///
      CLAY({
        .layout{
          .childGap = GAP_SMALL,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .floating{
          .zIndex = zIndex,
          .attachPoints{
            .element = CLAY_ATTACH_POINT_CENTER_TOP,
            .parent  = CLAY_ATTACH_POINT_CENTER_TOP,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        CLAY({}) {
          BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_WAVE);
          BF_CLAY_TEXT(TextFormat(" %d", g.run.state.waveIndex + 1));
        }

        if (g.run.state.screen == ScreenType_GAMEPLAY) {
          CLAY({}) {
            const auto remainingFrames
              = GetWaveDuration(g.run.state.waveIndex) - g.run.waveStartedAt.Elapsed();
            const int remainingSeconds
              = CeilDivision(MAX(1, remainingFrames.value), FIXED_FPS);
            BF_CLAY_TEXT(TextFormat("%d", remainingSeconds));
          }
        }
      }
      // }

      // Game's version.
      // {  ///
      CLAY({
        .floating{
          .zIndex = zIndex,
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
        .layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM},
        .floating{
          .zIndex = zIndex,
          .attachPoints{
            .element = CLAY_ATTACH_POINT_RIGHT_TOP,
            .parent  = CLAY_ATTACH_POINT_RIGHT_TOP,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        FOR_RANGE (int, i, g.run.state.crates) {
          const auto fb = fb_pickupables->Get(PickupableType_CRATE);
          BF_CLAY_IMAGE({.texId = fb->texture_id()});
        }
      }
      // }

      FontEnd();
    }
  }

  // New run.
  if (g.run.state.screen == ScreenType_NEW_RUN) {  ///
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        .childGap = GAP_BIG * 2,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
    }) {
      auto& p = g.player;

      // "NEW RUN" label.
      FontBegin(&g.meta.fontWaveCompletion);
      BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_NEW_RUN__CAPS);
      FontEnd();

      CLAY({.layout{
        .childGap        = GAP_BIG,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        // Difficulties.
        FontBegin(&g.meta.fontUIBig);
        BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_DIFFICULTY);
        FontEnd();

        CLAY({}) {
          FrameVisual errorAt{};
          if (!p.difficulty)
            errorAt = g.ui.newRunErrorAt;

          BEAUTIFY_WIGGLING_DANGER_SCOPED(
            errorAt,
            WEAPONS_WIGGLING_LOGICAL_AMPLITUDE,
            ERROR_WIGGLING_FRAMES,
            ERROR_WIGGLING_TIMES
          );

          CLAY({
            .layout{
              BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE_FRAME),
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            BF_CLAY_CUSTOM_NINE_SLICE(
              glib->ui_frame_nine_slice(), slotColors[0], slotColors[1]
            ),
          }) {
            CLAY({.layout{.childGap = GAP_SMALL}})
            FOR_RANGE (int, i, (int)DifficultyType_COUNT - 1) {
              auto isLocked = i > g.player.achievements[AchievementType_DIFFICULTY].value;
              auto fb       = fb_difficulties->Get(i + 1);

              const bool selected = ((i + 1) == (int)p.difficulty);

              CLAY({}) {
                componentUniversalSlot({
                  .id         = CLAY_IDI("new_run_difficulty", i),
                  .difficulty = (DifficultyType)(isLocked ? 0 : i + 1),
                  .hidden     = ComponentUniversalSlotHiddenType_SHOW_LOCK,
                  .tier       = (isLocked ? 0 : (selected ? 6 : i)),
                  .canHover   = !isLocked,
                });

                if (!isLocked) {
                  if (clicked()) {
                    PlaySound(Sound_UI_CLICK);
                    p.difficulty = (DifficultyType)(i + 1);
                    Save();
                  }

                  if (Clay_Hovered()) {
                    componentUniversalDetails({
                      .difficulty     = (DifficultyType)(i + 1),
                      .affectedByGame = false,
                      .detailsRight   = 1,
                      .detailsBelow   = 1,
                    });
                  }
                }
              }
            }
          }
        }

        // Builds.
        FontBegin(&g.meta.fontUIBig);
        BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_BUILD);
        FontEnd();

        auto buildsX = 6;
        auto buildsY = CeilDivision((int)fb_builds->size() - 1, buildsX);

        CLAY({}) {
          FrameVisual errorAt{};
          if (!p.build)
            errorAt = g.ui.newRunErrorAt;

          BEAUTIFY_WIGGLING_DANGER_SCOPED(
            errorAt,
            WEAPONS_WIGGLING_LOGICAL_AMPLITUDE,
            ERROR_WIGGLING_FRAMES,
            ERROR_WIGGLING_TIMES
          );

          CLAY({
            .layout{
              BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE_FRAME),
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            BF_CLAY_CUSTOM_NINE_SLICE(
              glib->ui_frame_nine_slice(), slotColors[0], slotColors[1]
            ),
          }) {
            FOR_RANGE (int, y, buildsY) {
              CLAY({.layout{.childGap = GAP_SMALL}}) {
                FOR_RANGE (int, x, buildsX) {
                  const int t = y * buildsX + x;
                  if (t >= fb_builds->size() - 1)
                    break;

                  const bool selected = ((int)p.build == (t + 1));

                  CLAY({}) {
                    auto       fb       = fb_builds->Get(t + 1);
                    const bool isLocked = g.player.lockedBuilds[t + 1];

                    const int tier = GetBuildTier((BuildType)(t + 1));

                    componentUniversalSlot({
                      .id       = CLAY_IDI("new_run_build", t),
                      .build    = (BuildType)(isLocked ? 0 : t + 1),
                      .hidden   = ComponentUniversalSlotHiddenType_SHOW_LOCK,
                      .tier     = (isLocked ? 0 : (selected ? 6 : tier)),
                      .canHover = !isLocked,
                    });

                    if (!isLocked) {
                      if (clicked()) {
                        PlaySound(Sound_UI_CLICK);
                        if (p.build != (BuildType)(t + 1)) {
                          p.build  = (BuildType)(t + 1);
                          p.weapon = {};
                        }
                        Save();
                      }

                      if (Clay_Hovered()) {
                        componentUniversalDetails({
                          .build          = (BuildType)(t + 1),
                          .affectedByGame = false,
                          .overrideTier   = tier,
                          .detailsRight   = 1,
                          .detailsBelow   = 1,
                        });
                      }
                    }
                  }
                }
              }
            }
          }
        }

        // Starting weapons.
        FontBegin(&g.meta.fontUIBig);
        BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_STARTING_WEAPON);
        FontEnd();

        CLAY({}) {
          FrameVisual errorAt{};
          if (!p.weapon)
            errorAt = g.ui.newRunErrorAt;

          BEAUTIFY_WIGGLING_DANGER_SCOPED(
            errorAt,
            WEAPONS_WIGGLING_LOGICAL_AMPLITUDE,
            ERROR_WIGGLING_FRAMES,
            ERROR_WIGGLING_TIMES
          );

          CLAY({
            .layout{
              BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE_FRAME),
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            BF_CLAY_CUSTOM_NINE_SLICE(
              glib->ui_frame_nine_slice(), slotColors[0], slotColors[1]
            ),
          }) {
            const int weaponsX = 6;
            const int weaponsY = CeilDivision(MAX_BUILD_WEAPONS, weaponsX);

            auto fb_buildWeapons = fb_builds->Get(p.build)->starting_weapon_types();

            FOR_RANGE (int, y, weaponsY) {
              CLAY({.layout{.childGap = GAP_SMALL}})
              FOR_RANGE (int, x, weaponsX) {
                const int t = y * weaponsX + x;

                const bool exists = fb_buildWeapons && (t < fb_buildWeapons->size());

                bool isLocked = false;

                bool selected = false;
                if (exists) {
                  const auto weaponType = (WeaponType)fb_buildWeapons->Get(t);
                  selected              = (p.weapon == weaponType);
                  isLocked              = g.player.lockedWeapons[weaponType];
                }

                CLAY({}) {
                  componentUniversalSlot({
                    .id = CLAY_IDI("new_run_weapon", t),
                    .weapon
                    = (WeaponType)(!isLocked && exists ? fb_buildWeapons->Get(t) : 0),
                    .hidden   = ComponentUniversalSlotHiddenType_SHOW_LOCK,
                    .tier     = (!exists || isLocked ? 0 : (selected ? 6 : 0)),
                    .canHover = exists && !isLocked,
                  });

                  // Hovering modal.
                  if (exists && !isLocked && Clay_Hovered()) {
                    componentWeaponDetails(
                      (WeaponType)fb_buildWeapons->Get(t), -1, false, false, false
                    );
                  }

                  if (exists && !isLocked) {
                    if (clicked()) {
                      PlaySound(Sound_UI_CLICK);
                      p.weapon = (WeaponType)fb_buildWeapons->Get(t);
                      Save();
                    }
                  }
                }
              }
            }
          }
        }
      }

      // Go.
      CLAY({.layout{.sizing{.width = CLAY_SIZING_FIXED(200)}}}) {
        const bool canGo = p.difficulty && p.build && p.weapon;
        const bool go    = componentButton(
          {.id = CLAY_ID("button_new_run_go"), .growX = true},
          [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_GO, textColor);
          }
        );

        if (go) {
          if (canGo) {
            PlaySound(Sound_UI_CLICK);
            g.run.reload = true;
          }
          else {
            PlaySound(Sound_UI_ERROR);
            g.ui.newRunErrorAt = {};
            g.ui.newRunErrorAt.SetNow();
          }
        }
      }
    }
  }
  // Picked up item.
  else if (g.run.state.screen == ScreenType_PICKED_UP_ITEM) {  ///
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      },
      BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
    }) {
      // Columns (1) picked up item, (2) stats
      CLAY({.layout{.childGap = GAP_BIG, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
        // Column 1. Picked up item.
        CLAY({.layout{
          .childGap        = GAP_SMALL,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          const auto type = g.run.state.pickedUpItem.toPick;
          const auto fb   = fb_items->Get(type);

          // "Item Found" label.
          BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_ITEM_FOUND);

          // Item.
          componentUniversalCard({
            .item           = type,
            .affectedByGame = true,
            .setFixedHeight = true,
          });

          // Take and Recycle buttons.
          CLAY({.layout{.childGap = GAP_SMALL}}) {
            const bool took = componentButton(
              {.id = CLAY_ID("button_picked_up_item_take")},
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_TAKE, textColor);
              }
            );

            int recyclePrice = fb->price() / RECYCLE_PRICE_FACTOR;

            const bool recycled = componentButton(
              {.id = CLAY_ID("button_picked_up_item_recycle")},
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_RECYCLE, textColor);
                  BF_CLAY_TEXT(TextFormat(" (+%d)", recyclePrice), textColor);
                }
              }
            );

            if (took)
              AddItem(g.run.state.pickedUpItem.toPick);
            else if (recycled)
              ChangeCoins(recyclePrice);

            if (took || recycled) {
              PlaySound(Sound_UI_CLICK);

              g.run.state.pickedUpItem = {};

              g.run.state.crates--;
              if (g.run.state.crates) {
                g.run.scheduledPickedUpItems      = true;
                g.run.scheduledPickedUpItemsReset = true;
              }
              else {
                g.run.scheduledUpgrades      = true;
                g.run.scheduledUpgradesReset = true;
              }

              Save();
            }
          }
        }

        // Column 2. Stats.
        componentStats();
      }
    }
  }
  // Upgrades.
  else if (g.run.state.screen == ScreenType_UPGRADES) {  ///
    // Vertical columns with upgrades and stats;
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        .childGap = GAP_BIG,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      },
      BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
    }) {
      // Upgrades.
      CLAY({.layout{
        .childGap = GAP_BIG,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        // Level up label.
        BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_LEVEL_UP);

        // Upgrades.
        CLAY({.layout{.childGap = GAP_SMALL}}) {
          const auto fb_stats = glib->stats();

          FOR_RANGE (int, i, g.run.state.upgrades.toPick.count) {
            const auto upgrade = g.run.state.upgrades.toPick[i];
            const auto fb      = fb_stats->Get(upgrade.stat);
            CLAY({
              .layout{
                .sizing{
                  CLAY_SIZING_FIXED(UPGRADE_FRAME_WIDTH + 2 * PADDING_NINE_SLICE_FRAME),
                  CLAY_SIZING_FIT(215),
                },
                BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE_FRAME),
                .childGap = GAP_SMALL,
                // BF_CLAY_CHILD_ALIGNMENT_CENTER_TOP,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              },
              BF_CLAY_CUSTOM_NINE_SLICE(
                glib->ui_frame_nine_slice(),
                slotColors[2 * upgrade.tier],
                slotColors[2 * upgrade.tier + 1]
              ),
            }) {
              CLAY({.layout{.childGap = GAP_SMALL}}) {
                // Slot with upgrade's image.
                componentSlot(
                  {.canHover = false, .tier = upgrade.tier},
                  [&]() BF_FORCE_INLINE_LAMBDA {
                    CLAY({
                      .layout{
                        BF_CLAY_SIZING_GROW_XY,
                        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                      },
                    }) {
                      BF_CLAY_IMAGE({.texId = fb->upgrade_texture_id()});
                    }
                  }
                );

                // Name.
                BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(
                  fb->upgrade_name_locale(), textColorsPerTier[upgrade.tier]
                );
              }

              BF_CLAY_SPACER_VERTICAL;

              // Stat + amount.
              const auto amount = fb->upgrade_values()->Get(upgrade.tier);
              CLAY({.layout{
                BF_CLAY_SIZING_GROW_X,
                .childGap = GAP_SMALL,
                BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
              }}) {
                const auto id = CLAY_IDI("upgrade", i);
                CLAY({
                  .id = id,
                  .layout{.childGap = GAP_SMALL, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER},
                }) {
                  BF_CLAY_TEXT(TextFormat("+%d", amount), palTextGreen);

                  BF_CLAY_IMAGE({.texId = fb->icon_texture_id()});

                  if (fb->is_percent())
                    BF_CLAY_TEXT("%");
                }

                const auto d = Clay_GetElementData(id);
                if (d.found) {
                  CLAY({.layout{
                    .childGap        = GAP_FLEX,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                  }}) {
                    FlexBegin(UPGRADE_FRAME_WIDTH - d.boundingBox.width, 0);
                    BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(fb->name_locale());
                    FlexEnd();
                  }
                }
              }

              BF_CLAY_SPACER_VERTICAL;

              // Choose button.
              CLAY({.layout{BF_CLAY_SIZING_GROW_X, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}
              ) {
                bool chosen = componentButton(
                  {.id = CLAY_IDI("button_upgrades_choose", i)},
                  [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                    BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_CHOOSE, textColor);
                  }
                );
                if (!draw && IsKeyPressed((SDL_Scancode)((int)SDL_SCANCODE_1 + i)))
                  chosen = true;
                if (chosen) {
                  PlaySound(Sound_UI_CLICK);

                  if (g.run.state.previousLevel < g.run.state.level) {
                    g.run.state.previousLevel++;
                    g.run.scheduledUpgrades      = true;
                    g.run.scheduledUpgradesReset = true;
                  }
                  else {
                    g.run.scheduledShop      = true;
                    g.run.scheduledShopReset = true;
                  }

                  g.run.state.statsWithoutItems[upgrade.stat] += amount;
                  g.run.recalculatePlayerStats = true;

                  if (upgrade.stat == StatType_HP)
                    PLAYER_CREATURE.health += amount;

                  Save();
                }
              }
            }
          }
        }

        // Reroll button.
        const auto calculatedRerollPrice
          = ApplyStatRerollPrice(g.run.state.upgrades.rerolls.GetPrice());
        const bool canReroll = (calculatedRerollPrice <= PLAYER_COINS);
        bool       rerolled  = componentButtonReroll(calculatedRerollPrice);
        if (!draw && IsKeyPressed(SDL_SCANCODE_R))
          rerolled = true;
        if (rerolled) {
          if (canReroll) {
            PlaySound(Sound_UI_CLICK);

            g.run.state.upgrades.rerolls.Roll();
            RefillUpgradesToPick();
            Save();
          }
          else {
            g.ui.upgradesErrorGold = {};
            g.ui.upgradesErrorGold.SetNow();
            PlaySound(Sound_UI_ERROR);
          }
        }
      }

      // Stats.
      componentStats();
    }
  }
  // Shop.
  else if (g.run.state.screen == ScreenType_SHOP) {  ///
    // Columns.
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        .childGap = GAP_BIG,
      },
      BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
    }) {
      // Left column that contains:
      // 1. wave, coins, reroll.
      // 2. items to buy.
      // 3. player's items and weapons.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        .childGap        = GAP_BIG,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        // 1. Wave, coins, reroll.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_X,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          // Wave.
          BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_SHOP);
          BF_CLAY_TEXT(" (");
          BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_WAVE);
          BF_CLAY_TEXT(TextFormat(" %d)", g.run.state.waveIndex + 1));

          BF_CLAY_SPACER_HORIZONTAL;

          // Coins.
          {
            BEAUTIFY_WIGGLING_DANGER_SCOPED(
              g.ui.shopErrorGold,
              GOLD_WIGGLING_LOGICAL_AMPLITUDE,
              ERROR_WIGGLING_FRAMES,
              ERROR_WIGGLING_TIMES
            );

            CLAY({
              .layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER},
              .floating{
                .zIndex = zIndex,
                .attachPoints{
                  .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                  .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
                },
                .attachTo = CLAY_ATTACH_TO_PARENT,
              },
            }) {
              FLOATING_BEAUTIFY;
              BF_CLAY_TEXT(TextFormat("%d ", PLAYER_COINS));
              BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});

              if (ge.meta.debugEnabled && Clay_Hovered() && wheel)
                ChangeCoins(wheel);
            }
          }

          BF_CLAY_SPACER_HORIZONTAL;

          // Reroll button.
          const auto calculatedRerollPrice
            = ApplyStatRerollPrice(g.run.state.shop.rerolls.GetPrice());
          const bool canReroll = (calculatedRerollPrice <= PLAYER_COINS);

          bool rerolled = componentButtonReroll(calculatedRerollPrice);
          if (!draw && IsKeyPressed(SDL_SCANCODE_R))
            rerolled = true;
          if (rerolled) {
            if (canReroll) {
              PlaySound(Sound_UI_CLICK);
              g.run.state.shop.rerolls.Roll();
              RefillShopToPick();
              RecalculateShopToPick();
              Save();
            }
            else {
              g.ui.shopErrorGold = {};
              g.ui.shopErrorGold.SetNow();
              PlaySound(Sound_UI_ERROR);
            }
          }
        }

        BF_CLAY_SPACER_VERTICAL;

        // 2. Items to buy.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_X,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          int toPickIndex = -1;
          for (auto& v : g.run.state.shop.toPick) {
            toPickIndex++;

            componentUniversalCard({
              .item            = v.item,
              .weapon          = v.weapon,
              .hideIfEmpty     = true,
              .affectedByGame  = true,
              .overrideTier    = v.tier,
              .setFixedHeight  = true,
              .shopBuyingIndex = toPickIndex,
            });

            if (toPickIndex < g.run.state.shop.toPick.count - 1)
              BF_CLAY_SPACER_HORIZONTAL;
          }
        }

        BF_CLAY_SPACER_VERTICAL;

        // 3. Player's items and weapons.
        CLAY({.layout{BF_CLAY_SIZING_GROW_X}}) {
          CLAY({.layout{.layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            // Items label.
            BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_ITEMS);

            CLAY({.layout{.sizing{.height = CLAY_SIZING_FIXED(GAP_SMALL)}}}) {}

            // Items.
            componentItemsGrid({.itemsX = 10});
          }

          BF_CLAY_SPACER_HORIZONTAL;

          CLAY({.layout{.layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            // Weapons label.
            CLAY({}) {
              int weaponsCount = 0;
              for (const auto& weapon : g.run.state.weapons) {
                if (weapon.type)
                  weaponsCount++;
              }

              BF_CLAY_TEXT_LOCALIZED_DANGER(
                Loc_UI_WEAPONS, (weaponsCount > 0 ? palTextWhite : TRANSPARENT_BLACK)
              );

              BF_CLAY_TEXT(
                TextFormat(" (%d/%d)", weaponsCount, g.run.state.weapons.count),
                (weaponsCount > 0 ? palTextWhite : TRANSPARENT_BLACK)
              );
            }

            CLAY({.layout{.sizing{.height = CLAY_SIZING_FIXED(GAP_SMALL)}}}) {}

            // Weapons.
            constexpr int WEAPONS_X = 3;
            constexpr int WEAPONS_Y = 2;
            static_assert(WEAPONS_X * WEAPONS_Y == g.run.state.weapons.count);

            {
              BEAUTIFY_WIGGLING_DANGER_SCOPED(
                g.ui.shopErrorWeapons,
                WEAPONS_WIGGLING_LOGICAL_AMPLITUDE,
                ERROR_WIGGLING_FRAMES,
                ERROR_WIGGLING_TIMES
              );

              CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}}
              )
              FOR_RANGE (int, y, 2) {
                CLAY({.layout{.childGap = GAP_SMALL}})
                FOR_RANGE (int, x, 3) {
                  const int weaponIndex = y * WEAPONS_X + x;
                  if (weaponIndex >= g.run.state.weapons.count)
                    break;

                  const auto& weapon = g.run.state.weapons[weaponIndex];

                  CLAY({}) {
                    // Weapon.
                    const bool selectedWeapon = componentUniversalSlot({
                      .id     = CLAY_IDI("shop_player_weapon", weaponIndex),
                      .weapon = weapon.type,
                      .tier   = weapon.tier,
                    });

                    if (selectedWeapon) {
                      PlaySound(Sound_UI_CLICK);
                      g.run.shopSelectedWeaponIndex = weaponIndex;
                    }

                    // Hovering modal.
                    if (weapon.type)
                      componentWeaponDetails(weapon.type, weaponIndex, true, false);
                  }
                }
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
          g.run.scheduledNextWave = componentButton(
            {.id = CLAY_ID("button_shop_next_wave"), .growX = true},
            [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_GO, textColor);
              BF_CLAY_TEXT(" (", textColor);
              BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_WAVE, textColor);
              BF_CLAY_TEXT(TextFormat(" %d)", g.run.state.waveIndex + 2), textColor);
            }
          );

          if (g.run.scheduledNextWave) {
            PlaySound(Sound_UI_CLICK);

            for (auto& v : g.run.state.shop.toPick)
              v = {};
          }
        }
      }
    }
  }
  // End.
  else if (g.run.state.screen == ScreenType_END) {  ///
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        .childGap = GAP_SMALL,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
    }) {
      // Header. Run Won / Lost, Wave.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        // Run Won / Lost.
        {
          const int locale = (g.run.state.won ? Loc_UI_WON : Loc_UI_LOST);
          BF_CLAY_TEXT_LOCALIZED_DANGER(locale);
        }

        BF_CLAY_TEXT(". ");

        // Wave.
        BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_WAVE);
        BF_CLAY_TEXT(TextFormat(" %d", g.run.state.waveIndex + 1));
      }

      // Main. Player's stats, weapons, items.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        .childGap = GAP_SMALL,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        // Stats.
        componentStats();

        // Player's weapons and items.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_XY,
          .childGap        = GAP_SMALL,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          int weaponsCount = 0;
          for (auto& weapon : g.run.state.weapons) {
            if (weapon.type)
              weaponsCount++;
          }

          if (weaponsCount > 0) {
            // Weapons label.
            BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_WEAPONS);

            // Weapons.
            CLAY({.layout{.childGap = GAP_SMALL}}) {
              int weaponIndex = -1;
              for (auto& weapon : g.run.state.weapons) {
                weaponIndex++;
                if (weapon.type) {
                  CLAY({}) {
                    // Weapon.
                    componentUniversalSlot({
                      .id     = CLAY_IDI("end_player_weapon", weaponIndex),
                      .weapon = weapon.type,
                      .tier   = weapon.tier,
                    });
                    // Hovering modal.
                    componentWeaponDetails(weapon.type, weaponIndex, false, true);
                  }
                }
              }
            }
          }

          // Items label.
          BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_ITEMS);
          // Items.
          componentItemsGrid({.itemsX = 10, .detailsBelow = 1});
        }
      }

      // Footer. Restart.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        .childGap = GAP_SMALL,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        const bool restarted = componentButton(
          {.id = CLAY_ID("button_end_restart")},
          [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_RESTART, textColor);
          }
        );

        const bool newRun = componentButton(
          {.id = CLAY_ID("button_end_new_run")},
          [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_NEW_RUN, textColor);
          }
        );

        if (restarted)
          g.run.reload = true;
        if (newRun)
          g.run.scheduledNewRun = true;

        if (restarted || newRun)
          PlaySound(Sound_UI_CLICK);
      }
    }
  }
  else if (g.run.state.screen == ScreenType_WAVE_END_ANIMATION) {
    // NOTE: Intentionally left blank.
  }
  else if (g.run.state.screen == ScreenType_GAMEPLAY) {
    // NOTE: Intentionally left blank.
  }
  else
    INVALID_PATH;

  // Window is inactive.
  if (ge.meta.windowIsInactive) {  ///
    CLAY({
      .floating{
        .zIndex   = zIndex,
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
      BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
    }) {}
  }

  // Pause / Achievements.
  if (g.meta.paused) {
    CLAY({
      .floating{
        .zIndex = zIndex,
        .attachPoints{
          .element = CLAY_ATTACH_POINT_CENTER_CENTER,
          .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
        },
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
      BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
    }) {
      FLOATING_BEAUTIFY;

      // Achievements.
      if (g.meta.pausedShowingAchievements) {  ///
        CLAY({.layout{
          .childGap = GAP_BIG,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          int totalSlots = 0;

          int currentAchievement = 1;
          int currentStep        = 0;
          int shownSlots         = 0;

          {
            int achievementIndex = -1;
            for (auto fb : *fb_achievements) {
              achievementIndex++;

              auto fb_steps = fb->steps();
              if (fb_steps) {
                auto stepsCount = (int)fb_steps->size();
                totalSlots += stepsCount;
              }
            }
          }

          // "Achievements" label.
          FontBegin(&g.meta.fontWaveCompletion);
          CLAY({}) {
            BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_ACHIEVEMENTS__CAPS);

            int percent = GetAchievementsCompletedPercent();

            if (percent > 0) {
              auto color = palTextBezhevy;
              if (percent >= 100)
                color = palTextGreen;
              BF_CLAY_TEXT(TextFormat(" %d%%", percent), color);
            }
          }
          FontEnd();

          const int achievementsX = 8;
          const int achievementsY = CeilDivision((int)totalSlots, achievementsX);

          // Columns.
          CLAY({.layout{
            .childGap = GAP_BIG,
            BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
          }}) {
            // Left column. Grid of achievements.
            CLAY({.layout{
              .childGap = GAP_BIG,
              BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              CLAY({
                .layout{
                  BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE_FRAME),
                  .childGap        = GAP_SMALL,
                  .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                BF_CLAY_CUSTOM_NINE_SLICE(
                  glib->ui_frame_nine_slice(), slotColors[0], slotColors[1]
                ),
              }) {
                g.meta.pausedAchievementsHoveredAchievement     = 0;
                g.meta.pausedAchievementsHoveredAchievementStep = 0;

                FOR_RANGE (int, y, achievementsY) {
                  CLAY({.layout{.childGap = GAP_SMALL}})
                  FOR_RANGE (int, x, achievementsX) {
                    if (shownSlots >= totalSlots)
                      break;

                    auto       fb      = fb_achievements->Get(currentAchievement);
                    auto       fb_step = fb->steps()->Get(currentStep);
                    const bool isLocked
                      = (g.player.achievements[currentAchievement].value < fb_step->value());

                    bool workingOnIt = true;
                    if (currentStep > 0) {
                      workingOnIt = g.player.achievements[currentAchievement].value
                                    >= fb->steps()->Get(currentStep - 1)->value();
                    }

                    bool canHover = true;

                    CLAY({}) {
                      int texId = 0;
                      int tier  = 0;

                      if (isLocked) {
                        texId = glib->ui_item_locked_texture_id();
                        if (workingOnIt)
                          tier = 1;
                      }
                      else
                        tier = 3;

                      componentUniversalSlot({
                        .id = CLAY_IDI(
                          "paused_achievement", 100 * currentAchievement + currentStep
                        ),
                        .build    = (BuildType)fb_step->unlocks_build_type(),
                        .item     = (ItemType)fb_step->unlocks_item_type(),
                        .weapon   = (WeaponType)fb_step->unlocks_weapon_type(),
                        .hidden   = ComponentUniversalSlotHiddenType_SHOW_LOCK,
                        .tier     = tier,
                        .canHover = canHover,
                      });

                      if (canHover && Clay_Hovered()) {
                        g.meta.pausedAchievementsHoveredAchievement = currentAchievement;
                        g.meta.pausedAchievementsHoveredAchievementStep = currentStep;
                      }
                    }

                    currentStep++;
                    if (currentStep >= fb->steps()->size()) {
                      currentAchievement++;
                      currentStep = 0;
                    }
                    shownSlots++;
                  }
                }
              }
            }

            // Right column. Hover data.
            CLAY({.layout{
              .childGap        = GAP_BIG,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              const BFGame::Achievement*     fb       = nullptr;
              const BFGame::AchievementStep* fb_step  = nullptr;
              bool                           isLocked = false;

              if (g.meta.pausedAchievementsHoveredAchievement) {
                fb = fb_achievements->Get(g.meta.pausedAchievementsHoveredAchievement);
                fb_step
                  = fb->steps()->Get(g.meta.pausedAchievementsHoveredAchievementStep);
                const auto achievementValue
                  = g.player.achievements[g.meta.pausedAchievementsHoveredAchievement];
                isLocked = fb_step && (achievementValue.value < fb_step->value());
              }

              // Achievement's name and description.
              componentAchievement(
                (AchievementType)g.meta.pausedAchievementsHoveredAchievement,
                g.meta.pausedAchievementsHoveredAchievementStep
              );

              // Achievement's reward.
              componentUniversalCard({
                .build
                = (BuildType)(fb_step && !isLocked ? fb_step->unlocks_build_type() : 0),
                .item
                = (ItemType)(fb_step && !isLocked ? fb_step->unlocks_item_type() : 0),
                .weapon
                = (WeaponType)(fb_step && !isLocked ? fb_step->unlocks_weapon_type() : 0),
                .setFixedHeight = true,
              });
            }
          }

          // Back button.
          const bool back = componentButton(
            {.id = CLAY_ID("button_pause_achievements_back")},
            [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_BACK, textColor);
            }
          );

          if (back) {
            PlaySound(Sound_UI_CLICK);
            g.meta.pausedShowingAchievements                = false;
            g.meta.pausedAchievementsHoveredAchievement     = 0;
            g.meta.pausedAchievementsHoveredAchievementStep = 0;
          }
        }
      }
      // Pause.
      else {  ///
        // Columns. Wave + buttons, weapons + items, stats.
        CLAY({.layout{
          .childGap = GAP_BIG,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          // 1. Wave + buttons.
          CLAY({.layout{
            .sizing{.width = CLAY_SIZING_FIXED(400)},
            .childGap = GAP_BIG,
            BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          }}) {
            CLAY({}) {
              BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_WAVE);
              BF_CLAY_TEXT(TextFormat(" %d", g.run.state.waveIndex + 1));
            }

            CLAY({.layout{
              BF_CLAY_SIZING_GROW_X,
              .childGap        = GAP_SMALL,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              const bool resumed = componentButton(
                {.id = CLAY_ID("button_pause_resume"), .growX = true},
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_RESUME, textColor);
                }
              );

              const bool restarted = componentButton(
                {.id = CLAY_ID("button_pause_restart"), .growX = true},
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_RESTART, textColor);
                }
              );

              const bool newRun = componentButton(
                {.id = CLAY_ID("button_pause_new_run"), .growX = true},
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_NEW_RUN, textColor);
                }
              );

              const bool achievements = componentButton(
                {.id = CLAY_ID("button_pause_achievements"), .growX = true},
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_ACHIEVEMENTS, textColor);
                  int percent = GetAchievementsCompletedPercent();
                  if (percent > 0)
                    BF_CLAY_TEXT(TextFormat(" %d%%", percent), textColor);
                }
              );

              bool exited = false;
#if defined(SDL_PLATFORM_DESKTOP)
              exited = componentButton(
                {.id = CLAY_ID("button_pause_exit"), .growX = true},
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_EXIT, textColor);
                }
              );
#endif

              if (resumed)
                g.meta.scheduledTogglePause = true;
              if (restarted)
                g.run.reload = true;
              if (newRun)
                g.run.scheduledNewRun = true;
              if (achievements)
                g.meta.pausedShowingAchievements = true;
              if (exited)
                ge.meta.exitScheduled = true;

              if (resumed || restarted || newRun || achievements || exited)
                PlaySound(Sound_UI_CLICK);
            }
          }

          // 2. Weapons + items.
          int weaponsCount = 0;
          for (auto& weapon : g.run.state.weapons) {
            if (weapon.type)
              weaponsCount++;
          }

          if ((weaponsCount > 0) || (g.run.state.items.count > 0)) {
            CLAY({.layout{
              .childGap        = GAP_BIG,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              if (weaponsCount > 0) {
                // Weapons label.
                BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_WEAPONS);

                // Weapons.
                CLAY({.layout{.childGap = GAP_SMALL}}) {
                  int weaponIndex = -1;
                  for (auto& weapon : g.run.state.weapons) {
                    weaponIndex++;
                    if (weapon.type) {
                      CLAY({}) {
                        // Weapon.
                        componentUniversalSlot({
                          .id     = CLAY_IDI("pause_player_weapon", weaponIndex),
                          .weapon = weapon.type,
                          .tier   = weapon.tier,
                        });
                        // Hovering modal.
                        componentWeaponDetails(weapon.type, weaponIndex, false, true);
                      }
                    }
                  }
                }
              }

              // Items label.
              BF_CLAY_TEXT_LOCALIZED_DANGER(Loc_UI_ITEMS);
              // Items.
              componentItemsGrid({.itemsX = 6});
            }
          }

          // 3. Stats.
          componentStats();
        }
      }
    }
  }

  // Just unlocked achievement.
  if (g.ui.justUnlockedAchievements.count > 0) {  ///
    const auto& x = g.ui.justUnlockedAchievements[0];
    lframe      e{};
    if (x.shownAt.IsSet())
      e = x.shownAt.Elapsed();

    f32 alpha     = 1;
    f32 translate = 0;

    if (e < ACHIEVEMENT_IN_FRAMES) {
      f32 p     = Clamp01(e.Progress(ACHIEVEMENT_IN_FRAMES));
      translate = 100 * (1 - EaseOutQuad(p));
      alpha     = EaseOutQuad(p);
    }
    if (e >= ACHIEVEMENT_TOTAL_FRAMES - ACHIEVEMENT_OUT_FRAMES) {
      f32 p = (e + ACHIEVEMENT_OUT_FRAMES - ACHIEVEMENT_TOTAL_FRAMES)
                .Progress(ACHIEVEMENT_OUT_FRAMES);
      alpha = Clamp01(1 - EaseOutQuad(p));
    }

    CLAY({.floating{
      .offset{-GAP_SMALL, GAP_SMALL},
      .zIndex = zIndex,
      .attachPoints{
        .element = CLAY_ATTACH_POINT_RIGHT_TOP,
        .parent  = CLAY_ATTACH_POINT_RIGHT_TOP,
      },
      .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
      .attachTo           = CLAY_ATTACH_TO_PARENT,
    }}) {
      FLOATING_BEAUTIFY;

      Beautify b{.alpha = (u16)(alpha * (f32)u16_max), .translate{translate, 0}};
      BEAUTIFY(b);

      auto fb      = fb_achievements->Get(x.type);
      auto fb_step = fb->steps()->Get(x.stepIndex);

      LAMBDA (void, componentAchievementReward, (int texId, int tier)) {
        f32 fadeLock       = 1;
        f32 fadeItem       = 0;
        f32 flashWhiteLock = 0;
        f32 flashWhiteItem = 0;

        const auto delayBeforeUnlock = ANIMATION_2_FRAMES;
        const auto unlockDuration    = ANIMATION_0_FRAMES;

        if (e > delayBeforeUnlock + unlockDuration) {
          // Item is shown clearly.
          fadeItem = 1;
          fadeLock = 0;
        }
        else if (e > delayBeforeUnlock) {
          // Animation.
          f32 p          = Clamp01((e - delayBeforeUnlock).Progress(unlockDuration));
          fadeItem       = p;
          fadeLock       = 1 - p;
          flashWhiteLock = 1 - SQR(SQR(cosf(PI32 * p)));
          flashWhiteItem = 1 - SQR(SQR(cosf(PI32 * p)));
        }

        componentSlot(
          {.tier = tier, .flashWhite = Clamp01(flashWhiteItem)},
          [&]() BF_FORCE_INLINE_LAMBDA {
            CLAY({.layout{
              BF_CLAY_SIZING_GROW_XY,
              BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
            }}) {
              componentCenterFloater([&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_IMAGE({
                  .texId = glib->ui_item_locked_texture_id(),
                  .color = Fade(WHITE, Clamp01(fadeLock)),
                  .flash = Fade(WHITE, Clamp01(flashWhiteLock)),
                });
              });

              componentCenterFloater([&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_IMAGE({
                  .texId = texId,
                  .color = Fade(WHITE, Clamp01(fadeItem)),
                  .flash = Fade(WHITE, Clamp01(flashWhiteItem)),
                });
              });
            }
          }
        );
      };

      CLAY({.layout{.childGap = GAP_SMALL, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
        CLAY({.layout{.childGap = GAP_SMALL}}) {
          if (fb_step->unlocks_build_type()) {
            auto fbx = fb_builds->Get(fb_step->unlocks_build_type());
            componentAchievementReward(fbx->texture_id(), 3);
          }
          if (fb_step->unlocks_weapon_type()) {
            auto fbx = fb_weapons->Get(fb_step->unlocks_weapon_type());
            componentAchievementReward(fbx->icon_texture_id(), 3);
          }
          if (fb_step->unlocks_item_type()) {
            auto fbx = fb_items->Get(fb_step->unlocks_item_type());
            componentAchievementReward(fbx->texture_id(), 3);
          }
        }

        componentAchievement(x.type, x.stepIndex);
      }
    }
  }

  ASSERT_FALSE(g.ui.overriddenFont);
  auto drawCommands = Clay_EndLayout();

  // Drawing UI.
  if (draw) {
    ZoneScopedN("Drawing UI");

    Array<Beautify, MAX_BEAUTIFIERS> beautifiers{};
    int                              beautifiersCount = 0;

    FOR_RANGE (int, mode, 2) {
      // 0 - drawing ui.
      // 1 - drawing gizmos.

      DrawGroup_Begin(mode ? DrawZ_GIZMOS : DrawZ_UI);
      DrawGroup_SetSortY(0);

      FOR_RANGE (int, i, drawCommands.length) {
        // Setup.
        // {  ///
        auto& cmd = drawCommands.internalArray[i];

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
            DrawGroup_CommandRect({
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
            DrawGroup_CommandTexture({
              .texId = data.texId,
              .pos{bb.x + bb.width / 2, bb.y + bb.height / 2},
              .sourceMargins = data.sourceMargins,
              .color{
                data.color.r,
                data.color.g,
                data.color.b,
                (u8)((f32)data.color.a * beautifierAlpha)
              },
              .flash = data.flash,
            });
          } break;

          case CLAY_RENDER_COMMAND_TYPE_TEXT: {  ///
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
            DrawGroup_CommandText({
              .pos{bb.x, bb.y + bb.height},
              .anchor{},
              .font       = &g.meta.fontUI + d.fontId,
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
              DrawGroup_CommandTextureNineSlice({
                .texId = data.nineSlice->texture_id(),
                .pos{bb.x, bb.y},
                .anchor{},
                .color{
                  data.nineSliceColor.r,
                  data.nineSliceColor.g,
                  data.nineSliceColor.b,
                  (u8)((f32)data.nineSliceColor.a * beautifierAlpha)
                },
                .flash = data.nineSliceFlash,
                .nineSliceMargins{
                  (f32)fb->left() / downscaleFactor,
                  (f32)fb->right() / downscaleFactor,
                  (f32)fb->top() / downscaleFactor,
                  (f32)fb->bottom() / downscaleFactor,
                },
                .nineSliceSize{(f32)bb.width, (f32)bb.height},
              });
            } break;

            case ClayCustomElementType_OVERLAY: {
              Vector2 size = (Vector2)LOGICAL_RESOLUTION;
              Vector2 pos{};
              if (ge.meta.screenToLogicalRatio > 1) {
                auto d = size.x * (ge.meta.screenToLogicalRatio - 1);
                size.x += d;
                pos.x -= d / 2;
              }
              else if (ge.meta.screenToLogicalRatio < 1) {
                auto d = size.y * (1.0f / ge.meta.screenToLogicalRatio - 1);
                size.y += d;
                pos.y -= d / 2;
              }

              DrawGroup_CommandRect({
                .pos  = pos,
                .size = size,
                .anchor{},
                .color = data.overlayColor,
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
            DrawGroup_CommandRectLines({
              .pos = Vector2(bb.x, bb.y) + Vector2(bb.width, bb.height) / 2.0f,
              .size{bb.width, bb.height},
              .color = GREEN,
            });
          }
        }
      }

      DrawGroup_End();
    }
  }
}

// NOTE: Current implementation assumes that `creatureSpeed` << `projectileSpeed`.
Vector2 ForecastWhereProjectileWillHitCreature(
  Vector2 creaturePos,
  Vector2 creatureSpeed,
  Vector2 projectilePos,
  f32     projectileSpeed
) {  ///
  ASSERT(projectileSpeed > 0);
  const auto dist = Vector2Distance(creaturePos, projectilePos);
  return creaturePos + creatureSpeed * (dist / projectileSpeed);
}

f32 GetCreatureSpeed(const Creature& creature) {  ///
  f32 speed = creature.speed * creature.speedModifier;
  if (glib->creatures()->Get(creature.type)->hostility_type() == HostilityType_MOB) {
    speed *= (f32)(g.run.playerStats[StatType_ENEMY_SPEED] + 100) / 100.0f;
    speed = MAX(0, speed);
  }
  return speed;
}

void MakeAOE(
  CreatureType damager,
  ParticleType particleType,
  Vector2      pos,
  f32          baseRadius,
  int          baseDamage,
  f32          critDamageMultiplier,
  f32          knockbackMeters,
  int          weaponIndexOrMinus1
) {  ///
  const f32 sizeMultiplier = GetExplosionSizeMultiplier();

  Particle p{
    .type  = particleType,
    .pos   = pos,
    .scale = sizeMultiplier,
  };
  p.createdAt.SetNow();

  const auto fb_damager = glib->creatures()->Get(damager);

  int creatureIndex = -1;
  for (const auto& creature : g.run.creatures) {
    creatureIndex++;

    if (creature.diedAt.IsSet())
      continue;

    const auto fb_creature = glib->creatures()->Get(creature.type);
    if (fb_damager->hostility_type() == fb_creature->hostility_type())
      continue;

    if (Vector2DistanceSqr(creature.pos, pos) > SQR(
          baseRadius * sizeMultiplier + MOB_HURTBOX_RADIUS * fb_creature->hurtbox_scale()
        ))
      continue;

    int  damage = Round((f32)baseDamage * GetExplosionDamageMultiplier());
    bool isCrit = false;
    if (damager == CreatureType_PLAYER) {
      // Player damages mob.
      damage = ApplyPlayerStatDamageMultiplier(damage);
      damage = Round(
        damage * ((f32)g.run.playerStats[StatType_EXPLOSION_DAMAGE] / 100.0f + 1)
      );

      if (damage > 0) {
        isCrit = IsCrit();
        if (isCrit)
          damage = Round((f32)damage * critDamageMultiplier);

        if (fb_creature->is_boss()) {
          damage = Round(
            (f32)damage
            * MAX(
              0, (f32)(g.run.playerStats[StatType_DAMAGE_AGAINST_BOSSES] + 100) / 100.0f
            )
          );
        }
      }
    }

    TryApplyDamage({
      .creatureIndex                      = creatureIndex,
      .damage                             = damage,
      .directionOrZero                    = Vector2DirectionOrRandom(pos, creature.pos),
      .knockbackMeters                    = knockbackMeters,
      .damagerCreatureType                = damager,
      .isCrit                             = isCrit,
      .indexOfWeaponThatDidDamageOrMinus1 = weaponIndexOrMinus1,
    });
  }
  *g.run.particles.Add() = p;
}

#if defined(SDL_PLATFORM_WIN32)

void _Save() {  ///
  ZoneScoped;

  DEFER {
    g.meta.previousSaveIsNotCompletedYet = false;
  };

  const auto toSwapPath = "save_to_swap.bin";

  bool saved = false;
  {
    auto fbb = DumpState();
    FOR_RANGE (int, i, 5) {
      if (SDL_SaveFile(toSwapPath, (u8*)fbb.GetBufferPointer(), fbb.GetSize())) {
        saved = true;
        break;
      }
    }
  }
  if (!saved) {
    LOGE("Temp save failed");
    return;
  }

  bool swapped = false;
  FOR_RANGE (int, i, 5) {
    if (SDL_RenamePath(toSwapPath, "save.bin")) {
      swapped = true;
      break;
    }
  }
  if (!swapped)
    LOGI("Save swap failed");
}

#elif defined(SDL_PLATFORM_EMSCRIPTEN)

void _Save() {
  LOGW("Save is not yet implemented for web");
  // auto fbb = DumpState();
}

#else
#  error "_Save() is not implemented for your platform"
#endif

void GameFixedUpdate() {
  ZoneScoped;

  ReloadFontsIfNeeded();

  const auto fb_preSpawns      = glib->pre_spawns();
  const auto fb_atlas_textures = glib->atlas_textures();
  const auto fb_creatures      = glib->creatures();
  const auto fb_items          = glib->items();
  const auto fb_weapons        = glib->weapons();
  const auto fb_projectiles    = glib->projectiles();
  const auto fb_particles      = glib->particles();
  const auto fb_difficulties   = glib->difficulties();
  const auto fb_achievements   = glib->achievements();
  const auto fb_builds         = glib->builds();

  // Save.
  if (g.meta.scheduledSave && !g.meta.previousSaveIsNotCompletedYet) {  ///
#ifdef BF_PLATFORM_WebYandex
    const int SAVE_FRAMES_PERIOD = 3 * FIXED_FPS;
#else
    const int SAVE_FRAMES_PERIOD = FIXED_FPS / 2;
#endif

    if (!g.meta.lastSaveAt.IsSet()
        || (g.meta.lastSaveAt.Elapsed().value >= SAVE_FRAMES_PERIOD))
    {
      g.meta.scheduledSave = false;
      g.meta.lastSaveAt    = {};
      g.meta.lastSaveAt.SetNow();
      LOGI("Saving...");

      g.meta.previousSaveIsNotCompletedYet = true;
      _Save();
    }
  }

  // Reloading game.
  if (g.run.reload) {  ///
    ZoneScopedN("Reloading game.");

    RunReset();
    RunInit();

    Save();
  }

  // Show / hide cursor.
  {  ///
    if ((g.run.state.screen == ScreenType_GAMEPLAY) && !g.meta.paused)
      SDL_HideCursor();
    else
      SDL_ShowCursor();
  }

  // Cheats.
  if (ge.meta.debugEnabled) {
    // F5 - add 10 coins.
    if (IsKeyPressed(SDL_SCANCODE_F5)) {  ///
      if (IsKeyDown(SDL_SCANCODE_LSHIFT))
        ChangeCoins(int_max);
      else
        ChangeCoins(10);
    }

    if (g.run.state.screen == ScreenType_GAMEPLAY) {
      // F7 - show end screen.
      if (IsKeyPressed(SDL_SCANCODE_F7)) {  ///
        g.run.scheduledUI  = true;
        g.run.scheduledEnd = true;
      }

      // F8 - add level.
      if (IsKeyPressed(SDL_SCANCODE_F8)) {  ///
        AddXP(GetNextLevelXp(g.run.state.level));
      }

      // F9 - add crate.
      if (IsKeyPressed(SDL_SCANCODE_F9)) {  ///
        g.run.state.crates++;
      }
    }

    // N - increase wave.
    if (IsKeyPressed(SDL_SCANCODE_N)) {  ///
      g.run.state.waveIndex++;
      RecalculateThisWaveMobs();
    }
  }

  // Recalculating player stats.
  if (g.run.recalculatePlayerStats) {  ///
    g.run.recalculatePlayerStats = false;
    RecalculatePlayerStats();
  }

  // Advancing to UI after wave completion animation finishes.
  if (g.run.scheduledWaveCompleted.IsSet()) {  ///
    if ((g.run.state.screen != ScreenType_WAVE_END_ANIMATION) && g.run.state.waveWon) {
      // Applying effects: END_OF_THE_WAVE_GET_STAT.
      IterateOverEffects(
        EffectConditionType_END_OF_THE_WAVE_GET_STAT,
        [&](auto fb_effect, int times)
          BF_FORCE_INLINE_LAMBDA { ApplyEffect(fb_effect, times); }
      );

      // Making coins fly.
      for (auto& pickupable : g.run.pickupables) {
        if (pickupable.pickedUpAt.IsSet())
          continue;
        if (pickupable.type != PickupableType_COIN)
          continue;

        const auto& data = pickupable.DataCoin();
        g.run.state.notPickedUpCoins += data.amount;

        pickupable.startedFlyingAt.SetNow();

        const auto durVariation = WAVE_COMPLETED_FRAMES.value
                                  - WAVE_COMPLETED_COINS_FLYING_FRAMES.value
                                  - WAVE_COMPLETED_COINS_FLYING_FRAMES_RIGHT.value;
        pickupable.startedFlyingAt._value += (i64)(GRAND.FRand() * (f32)durVariation);
      }
    }

    g.run.state.screen = ScreenType_WAVE_END_ANIMATION;

    for (int i = 1; i < g.run.creatures.count; i++) {
      auto& creature = g.run.creatures[i];
      if (!creature.diedAt.IsSet())
        TryApplyDamage({.creatureIndex = i, .damage = int_max});
    }
    g.run.preSpawns.Reset();

    if (g.run.scheduledWaveCompleted.Elapsed() >= WAVE_COMPLETED_FRAMES) {
      {
        // Applying StatType_HARVESTING.
        auto& harvesting = g.run.playerStats[StatType_HARVESTING];
        ChangeCoins(harvesting);
        AddXP(harvesting);
        // Harvesting stat (if positive) grows by 5% upon finishing each wave.
        if (harvesting > 0)
          harvesting = Ceil((f32)harvesting * 1.05f);
      }

      if ((g.run.state.waveIndex >= TOTAL_WAVES - 1) || !g.run.state.waveWon) {
        g.run.scheduledEnd = true;
        g.run.state.won    = g.run.state.waveWon;

        // Updating `Build.maxDifficultyBeaten` + achievement.
        if (g.run.state.won) {
          auto d = g.player.difficulty;

          AchievementMax(AchievementType_DIFFICULTY, d);

          auto& build               = g.player.builds[g.player.build];
          build.maxDifficultyBeaten = (DifficultyType)MAX(build.maxDifficultyBeaten, d);
        }
      }

      g.run.scheduledWaveCompleted = {};
      g.run.state.waveWon          = false;

      g.run.scheduledUI = true;
    }
  }

  // Applying UI settings (showing cursor + darkening gameplay).
  if (g.run.scheduledUI) {  ///
    g.run.scheduledUI = false;

    if (g.run.state.crates) {
      g.run.scheduledPickedUpItems      = true;
      g.run.scheduledPickedUpItemsReset = true;
    }
    else {
      g.run.scheduledUpgrades      = true;
      g.run.scheduledUpgradesReset = true;
    }
  }

  // Advancing to new run.
  if (g.run.scheduledNewRun) {  ///
    g.run.scheduledNewRun = false;
    g.run.state.screen    = ScreenType_NEW_RUN;
    Save();
  }

  // Advancing to ScreenType_PICKED_UP_ITEM.
  if (g.run.scheduledPickedUpItems) {  ///
    g.run.scheduledPickedUpItems = false;

    g.run.state.screen = ScreenType_PICKED_UP_ITEM;

    if (g.run.scheduledPickedUpItemsReset) {
      g.run.scheduledPickedUpItemsReset = false;
      g.run.state.pickedUpItem.toPick   = GenerateRandomItem();
    }

    Save();
  }

  // Advancing to ScreenType_UPGRADES.
  if (g.run.scheduledUpgrades) {  ///
    g.run.scheduledUpgrades = false;

    g.run.state.screen = ScreenType_UPGRADES;

    if (g.run.scheduledUpgradesReset) {
      g.run.scheduledUpgradesReset = false;
      RefillUpgradesToPick();
    }

    if (g.run.state.level == g.run.state.previousLevel) {
      g.run.scheduledShop      = true;
      g.run.scheduledShopReset = true;
    }

    Save();
  }

  // Advancing to ScreenType_SHOP.
  if (g.run.scheduledShop) {  ///
    g.run.scheduledShop = false;
    g.run.state.screen  = ScreenType_SHOP;

    g.run.projectiles.Reset();
    g.run.numbers.Reset();
    g.run.particles.Reset();
    g.run.landmines.Reset();
    g.run.gardens.Reset();

#if 1
    // Resetting all pickupables.
    g.run.pickupables.Reset();
#else
    // Resetting all pickupables except crates.
    {
      const int total = g.run.pickupables.count;
      int       off   = 0;
      FOR_RANGE (int, i, total) {
        const auto& pickupable = g.run.pickupables[i - off];
        if (pickupable.type != PickupableType_CRATE) {
          g.run.pickupables.UnstableRemoveAt(i - off);
          off++;
        }
      }
    }
#endif

    if (g.run.scheduledShopReset) {
      g.run.scheduledShopReset = false;
      g.run.state.shop.rerolls = {};
      RefillShopToPick();
    }
    RecalculateShopToPick();

    Save();
  }

  // Advancing to ScreenType_END.
  if (g.run.scheduledEnd) {  ///
    g.run.scheduledEnd = false;
    g.run.state.screen = ScreenType_END;

    Save();
  }

  // Advancing to the next wave (ScreenType_GAMEPLAY).
  if (g.run.scheduledNextWave) {  ///
    g.run.scheduledNextWave = false;

    g.run.state.screen = ScreenType_GAMEPLAY;

    g.run.state.waveIndex++;
    g.run.state.previousXp = g.run.state.xp;
    OnWaveStarted();

    const int health          = g.run.playerStats[StatType_HP];
    PLAYER_CREATURE.health    = health;
    PLAYER_CREATURE.maxHealth = health;

    IterateOverEffects(
      EffectConditionType_START_OF_THE_WAVE_GET_STAT,
      [&](auto fb_effect, int times)
        BF_FORCE_INLINE_LAMBDA { ApplyEffect(fb_effect, times); }
    );

    Save();
  }

  PLAYER_CREATURE.controller.move = {};

  // Handling ESC or P to toggle pause.
  {  ///
    if (g.run.state.screen == ScreenType_GAMEPLAY) {
      if (IsKeyPressed(SDL_SCANCODE_P) && !g.meta.pausedShowingAchievements) {
        g.meta.scheduledTogglePause = true;
        PlaySound(Sound_UI_CLICK);
      }

#if defined(SDL_PLATFORM_DESKTOP)
      if (IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        if (g.meta.pausedShowingAchievements)
          g.meta.pausedShowingAchievements = false;
        else
          g.meta.scheduledTogglePause = true;
        PlaySound(Sound_UI_CLICK);
      }
#endif
    }
    else
      g.meta.paused = false;
  }

  // Handling toggle pause.
  if (g.meta.scheduledTogglePause) {  ///
    g.meta.scheduledTogglePause = false;

    g.meta.paused = !g.meta.paused;
    Save();
  }

  const auto gameplayOrWaveEndScreen
    = (g.run.state.screen == ScreenType_GAMEPLAY)
      || (g.run.state.screen == ScreenType_WAVE_END_ANIMATION);

  // Updating gameplay.
  if (!ge.meta.windowIsInactive && !g.meta.paused && gameplayOrWaveEndScreen) {
    ZoneScopedN("Updating gameplay.");

    constexpr Rect creaturesWorldSpawnBounds{
      .pos{CREATURES_SPAWN_MARGIN, CREATURES_SPAWN_MARGIN},
      .size{WORLD_X - 2 * CREATURES_SPAWN_MARGIN, WORLD_Y - 2 * CREATURES_SPAWN_MARGIN},
    };

    constexpr f32 CREATURES_MOVE_MARGIN = 2;

    if (g.run.state.screen == ScreenType_GAMEPLAY) {
#if BF_ENABLE_ASSERTS
      for (auto& creature : g.run.creatures) {  ///
        if ((creature.health <= 0) && !creature.diedAt.IsSet())
          INVALID_PATH;
      }
#endif

      // Finishing wave opens upgrades screen.
      if (g.run.waveStartedAt.Elapsed() >= GetWaveDuration(g.run.state.waveIndex)) {  ///
        if (!g.run.scheduledWaveCompleted.IsSet()) {
          g.run.scheduledWaveCompleted.SetNow();
          g.run.state.waveWon = true;
          Save();
        }
      }

      if (!PLAYER_CREATURE.diedAt.IsSet()) {
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

          if (move.x || move.y)
            move = Vector2Normalize(move);

          if (move == Vector2Zero()) {
            if (g.meta.touch.touchIDs[0] != InvalidTouchID)
              move = g.meta.touch.dir[0];
          }

          PLAYER_CREATURE.controller.move = move;
        }

        // Player HP regen.
        if (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth) {  ///
          const bool canRegen = g.run.playerLastRegenAt.Elapsed()
                                >= GetFramesPerRegen(g.run.playerStats[StatType_REGEN]);
          if (canRegen) {
            PLAYER_CREATURE.health
              = MIN(PLAYER_CREATURE.health + 1, PLAYER_CREATURE.maxHealth);
            g.run.playerLastRegenAt = {};
            g.run.playerLastRegenAt.SetNow();
          }
        }
      }

      // Updating AI.
      {  ///
        ZoneScopedN("Updating AI.");

        for (int i = 1; i < g.run.creatures.count; i++) {
          auto& creature = g.run.creatures[i];
          if (creature.diedAt.IsSet())
            continue;

          const auto fb = fb_creatures->Get(creature.type);

          if (!creature.aggroed && fb->can_move()) {
            if (fb->can_aggro()) {
              if (fb->aggro_distance() == f32_inf)
                creature.aggroed = true;
              else if (Vector2DistanceSqr(PLAYER_CREATURE.pos, creature.pos)
                       <= SQR(fb->aggro_distance()))
              {
                creature.aggroed = true;
                creature.speedModifier /= fb->not_aggroed_speed() / fb->speed();
              }
            }

            auto& move = creature.controller.move;

            if ((creature.pos.x <= creaturesWorldSpawnBounds.pos.x) && (move.x <= 0))
              move = Vector2Rotate({1, 0}, Lerp(-PI32 / 2, PI32 / 2, GRAND.FRand()));

            if ((creature.pos.y <= creaturesWorldSpawnBounds.pos.y) && (move.y <= 0))
              move = Vector2Rotate({1, 0}, Lerp(0, PI32, GRAND.FRand()));

            if ((creature.pos.x
                 >= creaturesWorldSpawnBounds.pos.x + creaturesWorldSpawnBounds.size.x)
                && (move.x >= 0))
              move = Vector2Rotate({1, 0}, Lerp(PI32 / 2, 3 * PI32 / 2, GRAND.FRand()));

            if ((creature.pos.y
                 >= creaturesWorldSpawnBounds.pos.y + creaturesWorldSpawnBounds.size.y)
                && (move.y >= 0))
              move = Vector2Rotate({1, 0}, Lerp(PI32, 2 * PI32, GRAND.FRand()));
          }

          if (creature.aggroed) {
            if (fb->can_move()) {
              creature.controller.move
                = Vector2DirectionOrZero(creature.pos, PLAYER_CREATURE.pos);
            }
          }

          if (creature.controller.move.x >= 0)
            creature.dir = {1, 0};
          else
            creature.dir = {-1, 0};

          if (creature.aggroed && (creature.type == CreatureType_RANGER)) {
            constexpr f32 thresholdMeters = 0.5f;
            constexpr f32 shootMeters     = 8;
            const auto    distSqr = Vector2DistanceSqr(creature.pos, PLAYER_CREATURE.pos);

            bool canShoot = true;

            if (distSqr < SQR(shootMeters - thresholdMeters))
              creature.controller.move *= -1.0f;
            else if (distSqr < SQR(shootMeters + thresholdMeters))
              creature.controller.move = {};
            else
              canShoot = false;

            auto& data = creature.DataRanger();
            if (data.startedShootingAt.IsSet()) {
              creature.controller.move *= MOB_RANGER_MOVEMENT_SPEED_SCALE;

              const auto e = data.startedShootingAt.Elapsed();
              if (e == MOB_RANGER_SHOOTING_FRAME) {
                MakeProjectile({
                  .type              = ProjectileType_MOB,
                  .ownerCreatureType = creature.type,
                  .pos               = creature.pos,
                  .dir    = Vector2DirectionOrRandom(creature.pos, PLAYER_CREATURE.pos),
                  .range  = 12,
                  .damage = fb->projectile_damage(),
                });
              }
              if (e >= MOB_RANGER_SHOOTING_FRAMES)
                data.startedShootingAt = {};
            }
            else if (canShoot)
              data.startedShootingAt.SetNow();
          }
          else if (creature.aggroed && (creature.type == CreatureType_RUSHER)) {
            auto& data = creature.DataRusher();

            if (data.startedRushingAt.IsSet()) {
              creature.controller.move = {};

              const auto e = data.startedRushingAt.Elapsed();
              if ((MOB_RUSHER_RUSH_PRE_FRAMES < e)
                  && (e < MOB_RUSHER_RUSH_TOTAL_FRAMES - MOB_RUSHER_RUSH_POST_FRAMES))
                creature.controller.move = data.rushingDir;

              if (e >= MOB_RUSHER_RUSH_TOTAL_FRAMES) {
                data.startedRushingAt = {};
                data.finishedRushingAt.SetNow();
                data.rushingDir = {};
                data.cooldown.SetRand(MOB_RUSHER_COOLDOWN_MIN, MOB_RUSHER_COOLDOWN_MAX);
                creature.speedModifier /= MOB_RUSHER_RUSH_SPEED_SCALE;
              }
            }
            else {
              bool canRush = true;

              if (data.finishedRushingAt.IsSet()) {
                if (data.finishedRushingAt.Elapsed() < data.cooldown)
                  canRush = false;
              }

              if (canRush) {
                const auto dist = Vector2Distance(PLAYER_CREATURE.pos, creature.pos);
                const auto rushingDur = MOB_RUSHER_RUSH_TOTAL_FRAMES
                                        - MOB_RUSHER_RUSH_PRE_FRAMES
                                        - MOB_RUSHER_RUSH_POST_FRAMES;
                const auto durSeconds   = (f32)rushingDur.value / FIXED_FPS;
                const auto rushDistance = creature.speed * creature.speedModifier
                                          * MOB_RUSHER_RUSH_SPEED_SCALE * durSeconds;

                if (dist <= rushDistance) {
                  data.startedRushingAt.SetNow();
                  data.finishedRushingAt = {};
                  data.rushingDir
                    = Vector2DirectionOrRandom(creature.pos, PLAYER_CREATURE.pos);
                  creature.speedModifier *= MOB_RUSHER_RUSH_SPEED_SCALE;
                }
              }
            }
          }
          else if (creature.type == CreatureType_TURREL) {
            auto& data = creature.DataTurrel();

            f32 rangeMeters = fb->turrel_range_meters();
            rangeMeters *= (f32)g.run.playerStats[StatType_STRUCTURE_RANGE] / 100.0f + 1;
            rangeMeters = MAX(STRUCTURE_MIN_RANGE_METERS, rangeMeters);

            auto originalSize
              = glib->original_texture_sizes()->Get(fb->idle_texture_ids()->Get(0));
            const auto projectileSpawnPos
              = creature.pos
                + Vector2(
                  0,
                  (fb->turrel_projectile_shoot_anchor_y() - 0.5f) * (f32)originalSize->y()
                    * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE
                );
            auto projectileType = (ProjectileType)fb->turrel_projectile_type();
            ASSERT(projectileType);
            auto fb_projectile = fb_projectiles->Get(projectileType);

            f32     closestDist = f32_inf;
            Vector2 forecastedPos{};
            for (int otherCreatureIndex = 1;                  //
                 otherCreatureIndex < g.run.creatures.count;  //
                 otherCreatureIndex++)
            {
              const auto& otherCreature = g.run.creatures[otherCreatureIndex];
              if (otherCreature.id == creature.id)
                continue;

              auto fb_otherCreature = fb_creatures->Get(otherCreature.type);
              if (fb_otherCreature->hostility_type() == fb->hostility_type())
                continue;

              const auto forecastedCreaturePos = ForecastWhereProjectileWillHitCreature(
                otherCreature.pos,
                otherCreature.controller.move * GetCreatureSpeed(otherCreature),
                projectileSpawnPos,
                fb_projectile->speed()
              );

              auto d = Vector2DistanceSqr(forecastedCreaturePos, creature.pos);
              if (d <= SQR(rangeMeters)) {
                if (d < closestDist) {
                  closestDist   = d;
                  forecastedPos = forecastedCreaturePos;
                }
              }
            }

            if (closestDist != f32_inf) {
              data.aimDirection = Vector2DirectionOrRandom(creature.pos, forecastedPos);
              if (!data.startedShootingAt.IsSet())
                data.startedShootingAt.SetNow();
            }

            if (data.startedShootingAt.IsSet()) {
              const auto e = data.startedShootingAt.Elapsed();

              const auto shotFrame
                = ApplyStructureAttackSpeedToDuration(MOB_TURREL_SHOOT_FRAME.value);
              if (e == shotFrame) {
                int damage = fb->projectile_damage();
                damage     = ApplyDamageScalings(damage, 0, fb->turrel_damage_scalings());
                damage     = ApplyPlayerStatDamageMultiplier(damage);
                MakeProjectile({
                  .type                 = projectileType,
                  .ownerCreatureType    = creature.type,
                  .weaponIndexOrMinus1  = -1,
                  .pos                  = projectileSpawnPos,
                  .dir                  = data.aimDirection,
                  .range                = rangeMeters,
                  .damage               = damage,
                  .critDamageMultiplier = fb->crit_damage_multiplier(),
                  .knockbackMeters      = fb->turrel_knockback_meters(),
                  .pierce               = fb->turrel_pierce(),
                  .bounce               = fb->turrel_bounce(),
                });
              }

              const auto dur
                = ApplyStructureAttackSpeedToDuration(MOB_TURREL_SHOOTING_FRAMES.value);
              if (e >= dur)
                data.startedShootingAt = {};
            }
          }
        }
      }

      // Making pre spawn decals.
      {  ///

        LAMBDA (void, makePreSpawns, (CreatureType type)) {
          ASSERT(type);

          Vector2 posToSpawn{};
          while (1) {
            constexpr f32 epsilon = 0.001f;
            posToSpawn            = creaturesWorldSpawnBounds.GetRandomPosInside();
            ASSERT(creaturesWorldSpawnBounds.ContainsInside(posToSpawn));
            auto t = MIN(
              1,
              Unlerp(
                Vector2Distance(PLAYER_CREATURE.pos, posToSpawn),
                ENEMIES_DECAY_SPAWNING_AROUND_PLAYER_MIN,
                ENEMIES_DECAY_SPAWNING_AROUND_PLAYER_MAX
              )
            );
            if (t <= 0)
              continue;
            if (t > GRAND.FRand())
              break;
          };

          auto      fb = fb_creatures->Get(type);
          const int toSpawnCount
            = GRAND.RandInt(fb->spawn_group_count_min(), fb->spawn_group_count_max());

          FOR_RANGE (int, i, toSpawnCount) {
            Vector2 p{};
            do {
              const f32 off = Lerp(
                fb->spawn_group_radius_min(), fb->spawn_group_radius_max(), GRAND.FRand()
              );
              p = posToSpawn + Vector2Rotate({off, 0}, 2 * PI32 * GRAND.FRand());
            } while (!creaturesWorldSpawnBounds.ContainsInside(p));
            PreSpawn spawn{.type = PreSpawnType_CREATURE, .typeCreature = type, .pos = p};
            spawn.createdAt.SetNow();
            *g.run.preSpawns.Add() = spawn;
          }
        };

        int  spawnEnemiesEvery = FIXED_FPS;
        auto multiplier        = 1 + (f32)g.run.playerStats[StatType_ENEMIES] / 100.0f;
        multiplier             = MAX(0.001f, multiplier);
        spawnEnemiesEvery      = Round((f32)spawnEnemiesEvery / multiplier);
        spawnEnemiesEvery      = MAX(1, spawnEnemiesEvery);

        if (CanSpawnMoreCreatures()) {
          if (g.run.waveStartedAt.Elapsed().value % spawnEnemiesEvery == 0) {
            FOR_RANGE (int, toSpawnIndex, g.run.state.toSpawn) {
              const auto   factor = GRAND.FRand();
              CreatureType spawnType{};

              FOR_RANGE (int, k, g.run.thisWaveMobsCount) {
                const auto& c = g.run.thisWaveMobs[k];
                if (factor < c.accumulatedFactor) {
                  spawnType = c.type;
                  break;
                }
              }

              makePreSpawns(spawnType);
            }

            if (g.run.state.toSpawn < 3)
              g.run.state.toSpawn++;
          }

          // Spawning turrels every 3 seconds.
          if ((g.run.waveStartedAt.Elapsed().value + 1)
                % (FIXED_FPS * SPAWNING_TURRELS_EVERY_N_SECONDS)
              == 0)
          {
            if (g.run.turrelsToSpawn > 0) {
              g.run.turrelsToSpawn--;
              makePreSpawns(CreatureType_TURREL);
            }
          }

          // Spawning landmines.
          if (g.run.playerStats[StatType_LANDMINES] > 0) {
            auto landminesSpawnInterval = SPAWNING_LANDMINES_INTERVAL_FRAMES;
            landminesSpawnInterval.value /= g.run.playerStats[StatType_LANDMINES];
            landminesSpawnInterval.value /= GetPlayerStatStructureAttackSpeedMultiplier();
            landminesSpawnInterval.value = MAX(2, landminesSpawnInterval.value);

            if ((g.run.waveStartedAt.Elapsed().value + 1) % landminesSpawnInterval.value
                == 0)
            {
              PreSpawn v{
                .type = PreSpawnType_LANDMINE,
                .pos  = creaturesWorldSpawnBounds.GetRandomPosInside(),
              };
              v.createdAt.SetNow();
              *g.run.preSpawns.Add() = v;
            }
          }

          // Spawning gardens every 5 seconds.
          if ((g.run.waveStartedAt.Elapsed().value + 1)
                % (FIXED_FPS * SPAWNING_GARDENS_EVERY_N_SECONDS)
              == 0)
          {
            if (g.run.gardensToSpawn > 0) {
              g.run.gardensToSpawn--;
              PreSpawn v{
                .type = PreSpawnType_GARDEN,
                .pos  = creaturesWorldSpawnBounds.GetRandomPosInside(),
              };
              v.createdAt.SetNow();
              *g.run.preSpawns.Add() = v;
            }
          }

          // Spawning trees every 10 seconds.
          if ((g.run.waveStartedAt.Elapsed().value + 1) % (FIXED_FPS * 10) == 0) {
            const int toSpawn = GetNumberOfTreesToSpawn();
            FOR_RANGE (int, i, toSpawn)
              makePreSpawns(CreatureType_TREE);
          }
        }
      }

      // Spawning.
      {  ///
        const int total = g.run.preSpawns.count;
        int       off   = 0;
        FOR_RANGE (int, i, total) {
          auto& v = g.run.preSpawns[i - off];

          ASSERT(v.type);
          if (v.type == PreSpawnType_CREATURE)
            ASSERT(v.typeCreature);

          const bool shouldSpawn
            = v.createdAt.IsSet() && (v.createdAt.Elapsed() >= SPAWN_FRAMES);
          if (!shouldSpawn)
            continue;

          switch (v.type) {
          case PreSpawnType_CREATURE: {
            auto fb       = fb_creatures->Get(v.typeCreature);
            bool canSpawn = true;
            if (fb->hostility_type() == HostilityType_MOB) {
              if (Vector2DistanceSqr(v.pos, PLAYER_CREATURE.pos) <= SQR(
                    CREATURE_COLLIDER_RADIUS
                    * (fb_creatures->Get(CreatureType_PLAYER)->collider_scale() + fb->collider_scale())
                  ))
                canSpawn = false;
            }
            if (canSpawn)
              MakeCreature({.type = v.typeCreature, .pos = v.pos});
          } break;

          case PreSpawnType_LANDMINE: {
            MakeLandmine({.pos = v.pos});
          } break;

          case PreSpawnType_GARDEN: {
            MakeGarden({.pos = v.pos});
          } break;

          default:
            INVALID_PATH;
            break;
          }
          g.run.preSpawns.UnstableRemoveAt(i - off);
          off++;
        }
      }

      // Spawning boss during the last wave.
      if ((g.run.state.waveIndex >= TOTAL_WAVES - 1) && !g.run.bossCreatureId) {  ///
        const auto worldCenter = (Vector2)WORLD_SIZE / 2.0f;
        const auto dir       = Vector2DirectionOrRandom(PLAYER_CREATURE.pos, worldCenter);
        const auto bossPos   = worldCenter + dir * BOSS_SPAWN_OFFSET_METERS;
        g.run.bossCreatureId = MakeCreature({.type = CreatureType_BOSS, .pos = bossPos});
      }

      // Mobs contact-damage player.
      if (!PLAYER_CREATURE.diedAt.IsSet()) {  ///
        ZoneScopedN("Mobs contact-damage player.");

        const auto playerPos = PLAYER_CREATURE.pos;
        for (int i = 1; i < g.run.creatures.count; i++) {
          const auto& creature = g.run.creatures[i];
          if (creature.diedAt.IsSet())
            continue;

          const auto fb = fb_creatures->Get(creature.type);
          if (fb->hostility_type() != HostilityType_MOB)
            continue;

          if (Vector2DistanceSqr(playerPos, creature.pos) <= SQR(
                PLAYER_HURTBOX_RADIUS + CREATURE_COLLIDER_RADIUS * fb->collider_scale()
              ))
          {
            int damage
              = fb->contact_damage()
                + Round(
                  fb->contact_damage_increase_per_wave()
                  * (f32)(g.run.state.waveIndex + 1 - fb->appearing_wave_number())
                );
            TryApplyDamage({
              .creatureIndex       = 0,
              .damage              = damage,
              .damagerCreatureType = creature.type,
            });
          }
        }
      }

      // Picking up pickupables.
      if (!PLAYER_CREATURE.diedAt.IsSet()) {  ///
        ZoneScopedN("Picking up pickupables.");

        for (auto& pickupable : g.run.pickupables) {
          if (pickupable.pickedUpAt.IsSet()) {
            pickupable.pos
              = Vector2ExponentialDecay(pickupable.pos, PLAYER_CREATURE.pos, 3, FIXED_DT);
          }
          else {
            auto pickupRangeScale = (f32)(100 + g.run.playerStats[StatType_PICKUP_RANGE]);
            pickupRangeScale      = MAX(30, pickupRangeScale);
            pickupRangeScale /= 100.0f;

            if (Vector2DistanceSqr(pickupable.pos, PLAYER_CREATURE.pos)
                <= SQR(PICKUPABLE_HURTBOX_RADIUS * pickupRangeScale))
            {
              pickupable.pickedUpAt.SetNow();

              const int consumableOrCrateHeal
                = MIN(1, g.run.playerStats[StatType_CONSUMABLE_HEAL]);

              switch (pickupable.type) {
              case PickupableType_COIN: {
                const auto& data    = pickupable.DataCoin();
                int         amount  = data.amount;
                int         xNumber = 1;

                if (g.run.state.notPickedUpCoins > 0) {
                  int toAdd = amount;
                  if (toAdd > g.run.state.notPickedUpCoins)
                    toAdd = g.run.state.notPickedUpCoins;

                  g.run.state.notPickedUpCoins -= toAdd;
                  g.run.state.notPickedUpCoinsVisual -= toAdd;
                  amount += toAdd;
                  xNumber *= 2;
                }

                if (GRAND.FRand()
                    < (f32)g.run.playerStats[StatType_DOUBLE_MATERIAL_CHANCE] / 100.0f)
                {
                  amount *= 2;
                  xNumber *= 2;
                }

                if (xNumber > 1) {
                  MakeNumber({
                    .type  = NumberType_PICKUPABLE,
                    .value = xNumber,
                    .pos
                    = PLAYER_CREATURE.pos + Vector2(0, PLAYER_PICKUP_NUMBER_Y_OFFSET),
                  });
                }

                ChangeCoins(amount);
                AddXP((f32)amount);

                auto healChance = (f32)g.run.playerStats[StatType_COINS_HEAL] / 100.0f;
                if (GRAND.FRand() < healChance)
                  HealPlayer();
              } break;

              case PickupableType_CONSUMABLE: {
                if (consumableOrCrateHeal > 0)
                  HealPlayer(consumableOrCrateHeal);
              } break;

              case PickupableType_CRATE: {
                if (consumableOrCrateHeal > 0)
                  HealPlayer(consumableOrCrateHeal);
                g.run.state.crates++;
              } break;

              default:
                INVALID_PATH;
              }
            }
          }
        }
      }

      // Achievements. Walker / Idler.
      {  ///
        const f32 deltaMeters
          = Vector2Distance(PLAYER_CREATURE.pos, g.run.previousPlayerPos);

        g.run.previousPlayerPos = PLAYER_CREATURE.pos;

        if ((deltaMeters > 0) && !FloatEquals(deltaMeters, 0)) {
          g.run.playerWalkedMeters += deltaMeters;
          g.run.playerIdleFrames = 0;
        }
        else
          g.run.playerIdleFrames++;

        while (g.run.playerWalkedMeters > 1) {
          g.run.playerWalkedMeters -= 1;
          AchievementAdd(AchievementType_WALKER, 1);
        }
        while (g.run.playerIdleFrames >= FIXED_FPS * 3 / 2) {
          g.run.playerIdleFrames -= FIXED_FPS;
          AchievementAdd(AchievementType_IDLER, 1);
        }
      }
    }

    // Burning spread.
    for (auto& creature : g.run.creatures) {  ///
      ZoneScopedN("Burning spread.");

      auto& a = creature.ailments[AilmentType_BURN - 1];
      if (!a.startedAt.IsSet())
        continue;
      if ((a.spread <= 0))
        continue;

      auto       fb         = fb_creatures->Get(creature.type);
      const auto isFriendly = (fb->hostility_type() == HostilityType_FRIENDLY);

      for (auto& otherCreature : g.run.creatures) {
        if (creature.id == otherCreature.id)
          continue;
        if (otherCreature.diedAt.IsSet())
          continue;

        auto       fb_other = fb_creatures->Get(otherCreature.type);
        const auto otherIsFriendly
          = (fb_other->hostility_type() == HostilityType_FRIENDLY);
        if (isFriendly == otherIsFriendly)
          continue;

        bool canBurn = true;
        auto resists = fb_other->resists_ailment_types();
        if (resists) {
          for (auto v : *resists) {
            if (v == AilmentType_BURN) {
              canBurn = false;
              break;
            }
          }
        }
        if (!canBurn)
          continue;

        const auto& aa = otherCreature.ailments[AilmentType_BURN - 1];
        if (aa.startedAt.IsSet())
          continue;

        const auto d = Vector2DistanceSqr(creature.pos, otherCreature.pos);
        if (d > SQR(BURNING_SPREAD_DISTANCE))
          continue;

        ApplyAilment(
          &otherCreature,
          a.ownerCreatureType,
          {.type = AilmentType_BURN, .value = a.value, .spread = 0}
        );
        a.spread--;

        if (a.spread <= 0)
          break;
      }
    }

    // Creatures ailments.
    {  ///
      ZoneScopedN("Creatures ailments.");

      int creatureIndex = -1;
      for (auto& creature : g.run.creatures) {
        creatureIndex++;
        for (int i = 1; i < AilmentType_COUNT; i++) {
          auto& a = creature.ailments[i - 1];

          // Burning damage over time.
          if (i == AilmentType_BURN) {
            if (a.startedAt.IsSet()
                && (a.startedAt.Elapsed().value % BURNING_RATE.value == 0))
            {
              bool isCrit = false;
              if (a.ownerCreatureType == CreatureType_PLAYER)
                isCrit = IsCrit();
              TryApplyDamage({
                .creatureIndex                      = creatureIndex,
                .damage                             = a.value,
                .damagerCreatureType                = a.ownerCreatureType,
                .isCrit                             = isCrit,
                .indexOfWeaponThatDidDamageOrMinus1 = a.weaponIndexOrMinus1,
              });
            }
          }

          // Decay.
          if (a.startedAt.IsSet() && (a.startedAt.Elapsed() >= a.duration)) {
            if (i == AilmentType_ZAP)
              creature.speedModifier /= ZAP_SPEED_SCALE;
            a = {};
          }
        }
      }
    }

    // Creatures moving.
    {  ///
      ZoneScopedN("Creatures moving.");

      for (auto& creature : g.run.creatures) {
        if (creature.diedAt.IsSet())
          continue;

        const auto fb = fb_creatures->Get(creature.type);

        f32 speed = GetCreatureSpeed(creature);
        if (creature.type == CreatureType_PLAYER) {
          speed *= MAX(0, (f32)(100 + g.run.playerStats[StatType_SPEED]) / 100.0f);
          if (g.meta.godMode)
            speed *= 1.5f;
        }

        speed *= b2Body_GetMass(creature.body.id) * BODY_LINEAR_DAMPING_SPEED_SCALE;

        b2Body_ApplyLinearImpulseToCenter(
          creature.body.id, ToB2Vec2(creature.controller.move * (FIXED_DT * speed)), true
        );

        if (abs(creature.controller.move.x) > 0.2f) {
          if (creature.controller.move.x > 0)
            creature.dir.x = 1;
          else
            creature.dir.x = -1;
        }
      }
    }

    // Updating box2d world.
    {  ///
      ZoneScopedN("Updating box2d world.");
      b2World_Step(g.run.world, FIXED_DT, 4);
    }

    // Updating body positions.
    {  ///
      ZoneScopedN("Updating body positions.");

      for (auto& creature : g.run.creatures) {
        if (creature.diedAt.IsSet())
          continue;

        creature.pos = ToVector2(b2Body_GetPosition(creature.body.id));

        const auto velocity = ToVector2(b2Body_GetLinearVelocity(creature.body.id));

        const auto tolerance = 0.1f;

        if ((abs(velocity.x) < tolerance) && (abs(velocity.y) < tolerance)) {
          creature.movementAccumulator = 0;
          if (!creature.idleStartedAt.IsSet())
            creature.idleStartedAt.SetNow();
        }
        else {
          creature.movementAccumulator += Vector2Length(velocity) * FIXED_DT;
          if (creature.idleStartedAt.IsSet())
            creature.idleStartedAt = {};
        }

        const auto fb = fb_creatures->Get(creature.type);
        creature.movementAccumulator
          = fmodf(creature.movementAccumulator, fb->movement_accumulator_meters_cycle());
      }
    }

    // Player weapons shooting.
    if (!PLAYER_CREATURE.diedAt.IsSet()) {  ///
      ZoneScopedN("Player weapons shooting.");

      int weaponIndex = -1;
      for (auto& weapon : g.run.state.weapons) {
        weaponIndex++;
        if (!weapon.type)
          continue;

        const auto fb  = fb_weapons->Get(weapon.type);
        const auto pos = PLAYER_CREATURE.pos + GetPlayerWeaponOffset(weaponIndex);

        f32 minDistSqr           = f32_inf;
        int closestCreatureIndex = -1;

        // Resetting cooldown.
        const auto cooldownDur = ApplyAttackSpeedToDuration(fb->cooldown_frames());
        if (weapon.cooldownStartedAt.IsSet()
            && (weapon.cooldownStartedAt.Elapsed() >= cooldownDur))
          weapon.cooldownStartedAt = {};

        if (!weapon.cooldownStartedAt.IsSet()) {
          int creatureIndex = -1;
          for (const auto& creature : g.run.creatures) {
            creatureIndex++;

            if (creature.diedAt.IsSet())
              continue;

            auto fb_creature = fb_creatures->Get(creature.type);
            if (fb_creature->hostility_type() == HostilityType_FRIENDLY)
              continue;

            const auto distSqr = Vector2DistanceSqr(pos, creature.pos);

            if (distSqr < minDistSqr) {
              minDistSqr           = distSqr;
              closestCreatureIndex = creatureIndex;
            }
          }

          bool targetSet = false;

          if (closestCreatureIndex >= 0) {
            const auto& closestCreature = g.run.creatures[closestCreatureIndex];
            auto        range           = GetWeaponRangeMeters(weapon.type);

            if (fb->projectile_type()) {
              const auto fb_projectile = glib->projectiles()->Get(fb->projectile_type());
              range += fb_projectile->collider_radius();
            }
            else {
              const auto originalSize
                = glib->original_texture_sizes()->Get(fb->texture_ids()->Get(0));
              range += (f32)originalSize->x() * ASSETS_TO_LOGICAL_RATIO
                       / METER_LOGICAL_SIZE / 2.0f;
            }

            if (minDistSqr < SQR(range)) {
              // Melee weapons attack from center.
              // Ranged - from their offset positions.
              Vector2 targetFromPos = PLAYER_CREATURE.pos;
              if (fb->projectile_type())
                targetFromPos += GetPlayerWeaponOffset(weaponIndex);
              const auto dir
                = Vector2DirectionOrRandom(targetFromPos, closestCreature.pos);

              // Only ranged weapons continue tracking target during shooting.
              if (fb->projectile_type())
                weapon.targetDir = dir;

              if (!weapon.startedShootingAt.IsSet()) {
                weapon.targetDir = dir;
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

          const auto shootingDur
            = ApplyAttackSpeedToDuration(fb->shooting_duration_frames());

          const auto projectileSpawnFrames = fb->projectile_spawn_frames();

          if (projectileType) {
            // It's a weapon that shoots projectiles (RANGED / ELEMENTAL damage types).
            ASSERT(projectileSpawnFrames);

            bool       spawn     = false;
            const auto speedMult = GetPlayerStatAttackSpeedMultiplier();
            for (auto value : *projectileSpawnFrames) {
              if (Ceil((f32)value / speedMult) == e.value) {
                spawn = true;
                break;
              }
            }

            if (spawn) {
              MakeProjectile({
                .type                 = projectileType,
                .ownerCreatureType    = PLAYER_CREATURE.type,
                .weaponIndexOrMinus1  = weaponIndex,
                .pos                  = pos,
                .dir                  = weapon.targetDir,
                .range                = GetWeaponRangeMeters(weapon.type),
                .damage               = weapon.calculatedDamage,
                .critDamageMultiplier = fb->crit_damage_multiplier(),
                .knockbackMeters      = fb->knockback_meters(),
                .pierce               = fb->projectile_pierce(),
                .bounce               = fb->projectile_bounce(),
              });
            }
          }
          else {
            // It's a weapon that gets "shot" itself (MELEE / ELEMENTAL damage types).
            // Not projectile.
            ASSERT(!projectileSpawnFrames);

            const auto colliderActiveStart = shootingDur.value / 4;
            const auto colliderActiveEnd   = shootingDur.value / 2;

            const auto p = e.Progress(shootingDur);

            if ((0.25f <= p)
                && ((p <= 0.5f) || ((weapon.lastCollisionCheckShootingProgress < 0.5f) && (p >= 0.5f))))
            {
              weapon.lastCollisionCheckShootingProgress = p;

              const auto texId = fb->texture_ids()->Get(0);
              Vector2    colliderSize{
                (f32)glib->original_texture_sizes()->Get(texId)->x(),
                (f32)fb->melee_collider_height_px(),
              };
              colliderSize *= ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;

              colliderSize.x += GetWeaponRangeMeters(weapon.type);

              CheckCollisionsRect(
                ShapeCategory_STATIC,
                (u32)ShapeCategory_CREATURE,
                PLAYER_CREATURE.pos + weapon.targetDir * colliderSize.x / 2.0f,
                colliderSize,
                weapon.targetDir,
                OnWeaponCollided,
                &weaponIndex
              );
            }
          }

          if (e >= shootingDur) {
            weapon.startedShootingAt = {};
            weapon.piercedCount      = 0;
            weapon.cooldownStartedAt.SetNow();
            const auto variation = (int)(cooldownDur.value / 10);
            weapon.cooldownStartedAt._value += GRAND.RandInt(-variation, variation);
            weapon.lastCollisionCheckShootingProgress = 0;
          }
        }
      }
    }

    // Updating projectiles:
    // - Movement.
    // - Marking to remove because of travel distance.
    // - Mob collisions.
    // - Marking to remove because of pierce count.
    {  ///
      ZoneScopedN("Updating projectiles.");

      constexpr f32 projectilesAliveBoundsMargin = 3;

      constexpr Rect projectilesAliveBounds{
        .pos{-projectilesAliveBoundsMargin, -projectilesAliveBoundsMargin},
        .size{
          (f32)WORLD_X + 2 * projectilesAliveBoundsMargin,
          (f32)WORLD_Y + 2 * projectilesAliveBoundsMargin,
        },
      };

      int projectileIndex = -1;
      for (auto& projectile : g.run.projectiles) {
        projectileIndex++;

        const auto fb       = fb_projectiles->Get(projectile.type);
        const auto distance = FIXED_DT * fb->speed();
        projectile.travelledDistance += distance;
        projectile.pos += projectile.dir * distance;

        bool createAoe = false;

        // Removing projectiles that travelled far enough.
        if (projectile.travelledDistance >= projectile.range) {
          if (!g.run.projectilesToRemove.Contains(projectileIndex)) {
            *g.run.projectilesToRemove.Add() = projectileIndex;
            if (fb->aoe_particle_type() && fb->aoe_on_travel_end())
              createAoe = true;
          }
        }

        // Removing projectiles that are outside the world.
        if (!projectilesAliveBounds.ContainsInside(projectile.pos)) {
          if (!g.run.projectilesToRemove.Contains(projectileIndex))
            *g.run.projectilesToRemove.Add() = projectileIndex;
        }

        int start = 0;
        int end   = g.run.creatures.count;

        auto fb_owner = fb_creatures->Get(projectile.ownerCreatureType);
        if (fb_owner->hostility_type() == HostilityType_FRIENDLY)
          start = 1;
        else
          end = 1;

        for (int creatureIndex = start; creatureIndex < end; creatureIndex++) {
          auto& creature = g.run.creatures[creatureIndex];
          if (creature.diedAt.IsSet())
            continue;

          // Not damaging already damaged creatures.
          if (ArrayContains(
                projectile.damagedCreatureIds.base, projectile.damagedCount, creature.id
              ))
            continue;

          const auto fb_creature = fb_creatures->Get(creature.type);
          if (fb_owner->hostility_type() == fb_creature->hostility_type())
            continue;

          const auto distSqr = Vector2DistanceSqr(creature.pos, projectile.pos);
          const auto radius  = fb->collider_radius();
          if (distSqr < SQR(radius)) {
            Vector2 knockbackDirection{};

            int  damage = projectile.damage;
            bool isCrit = false;
            if (projectile.ownerCreatureType == CreatureType_PLAYER) {
              // Player damages mob.
              isCrit = IsCrit();
              if (isCrit)
                damage *= projectile.critDamageMultiplier;

              if (fb_creature->is_boss()) {
                damage *= MAX(
                  0,
                  (f32)(g.run.playerStats[StatType_DAMAGE_AGAINST_BOSSES] + 100) / 100.0f
                );
              }

              knockbackDirection
                = Vector2DirectionOrRandom(PLAYER_CREATURE.pos, creature.pos);
            }

            const f32 piercingDamageMultiplier
              = (f32)g.run.playerStats[StatType_PIERCING_DAMAGE] / 100.0f;

            f32 knockback = projectile.knockbackMeters;

            FOR_RANGE (int, i, projectile.piercedCount) {
              damage = Round((f32)damage * piercingDamageMultiplier);
              knockback *= piercingDamageMultiplier;
            }

            int ailmentSpread = 0;
            if (fb->ailment_type() == AilmentType_BURN)
              ailmentSpread = MAX(0, g.run.playerStats[StatType_BURNING_SPREAD]);

            if (TryApplyDamage({
                  .creatureIndex                      = creatureIndex,
                  .damage                             = damage,
                  .directionOrZero                    = knockbackDirection,
                  .knockbackMeters                    = knockback,
                  .damagerCreatureType                = projectile.ownerCreatureType,
                  .isCrit                             = isCrit,
                  .indexOfWeaponThatDidDamageOrMinus1 = projectile.weaponIndexOrMinus1,
                  .ailment{
                    .type   = (AilmentType)fb->ailment_type(),
                    .value  = fb->ailment_value(),
                    .spread = ailmentSpread,
                  },
                  .ailmentChance = fb->ailment_chance(),
                }))
            {
              auto maxBounce = projectile.bounce;
              auto maxPierce = projectile.pierce;
              if (projectile.ownerCreatureType == CreatureType_PLAYER) {
                maxBounce += g.run.playerStats[StatType_BOUNCES];
                maxPierce += g.run.playerStats[StatType_PIERCING];
              }

              bool canBounce = projectile.bouncedCount < maxBounce;
              bool canPierce = projectile.piercedCount < maxPierce;
              if (projectile.damagedCount >= projectile.damagedCreatureIds.count) {
                canBounce = false;
                canPierce = false;
              }

              if (canBounce) {
                projectile.damagedCreatureIds[projectile.damagedCount++] = creature.id;
                projectile.bouncedCount++;
                projectile.travelledDistance = 0;

                const Creature* found = nullptr;
                Vector2         forecastedPos{};

                FOR_RANGE (int, i, 16) {
                  const auto c
                    = g.run.creatures.base + GRAND.Rand() % g.run.creatures.count;
                  if (c->diedAt.IsSet())
                    continue;
                  if (ArrayContains(
                        projectile.damagedCreatureIds.base, projectile.damagedCount, c->id
                      ))
                    continue;

                  const auto ownerHost = (HostilityType)fb_owner->hostility_type();
                  ASSERT(ownerHost);
                  ASSERT(ownerHost != HostilityType_NEUTRAL);

                  const bool ownerIsHostile = (ownerHost == HostilityType_MOB);
                  bool       canDamage      = false;
                  canDamage |= ownerIsHostile && (c->type == CreatureType_PLAYER);
                  const auto otherIsFriendly
                    = fb_creatures->Get(c->type)->hostility_type()
                      == HostilityType_FRIENDLY;
                  canDamage |= !ownerIsHostile && !otherIsFriendly;

                  if (!canDamage)
                    continue;

                  const auto forecastedCreaturePos
                    = ForecastWhereProjectileWillHitCreature(
                      c->pos,
                      c->controller.move * GetCreatureSpeed(*c),
                      projectile.pos,
                      fb->speed()
                    );
                  const auto d
                    = Vector2DistanceSqr(forecastedCreaturePos, projectile.pos);
                  if (d <= SQR(projectile.range)) {
                    found         = c;
                    forecastedPos = forecastedCreaturePos;
                    break;
                  }
                }
                if (found)
                  projectile.dir
                    = Vector2DirectionOrRandom(projectile.pos, forecastedPos);
                else
                  projectile.dir = Vector2Rotate({1, 0}, 2 * PI32 * GRAND.FRand());
              }
              else if (canPierce) {
                projectile.damagedCreatureIds[projectile.damagedCount++] = creature.id;
                projectile.piercedCount++;
              }
              else if (!g.run.projectilesToRemove.Contains(projectileIndex)) {
                *g.run.projectilesToRemove.Add() = projectileIndex;
                if (fb->aoe_on_contact())
                  createAoe = true;
              }
            }
          }
        }

        // Creating AOE particle.
        if (createAoe && fb->aoe_particle_type() && (GRAND.FRand() < fb->aoe_chance())) {
          MakeAOE(
            projectile.ownerCreatureType,
            (ParticleType)fb->aoe_particle_type(),
            projectile.pos,
            fb->aoe_radius(),
            projectile.damage,
            projectile.critDamageMultiplier,
            projectile.knockbackMeters,
            projectile.weaponIndexOrMinus1
          );
        }
      }
    }

    // Detonating landmines.
    {  ///
      const int total = g.run.landmines.count;
      int       off   = 0;
      FOR_RANGE (int, i, total) {
        auto& landmine = g.run.landmines[i - off];

        if (!landmine.startedDetonationAt.IsSet()) {
          for (const auto& creature : g.run.creatures) {
            auto fb = fb_creatures->Get(creature.type);
            if (!fb->can_move())
              continue;

            const f32 distSqr = Vector2DistanceSqr(creature.pos, landmine.pos);
            if (distSqr < SQR(LANDMINE_COLLIDER_RADIUS + CREATURE_COLLIDER_RADIUS)) {
              landmine.startedDetonationAt.SetNow();
              break;
            }
          }
        }

        if (landmine.startedDetonationAt.IsSet()
            && (landmine.startedDetonationAt.Elapsed() >= LANDMINE_DETONATION_FRAMES))
        {
          MakeAOE(
            CreatureType_PLAYER,
            ParticleType_EXPLOSION,
            landmine.pos,
            glib->landmine_explosion_radius(),
            ApplyDamageScalings(1, 0, glib->landmine_damage_scalings()),
            glib->landmine_crit_damage_multiplier(),
            glib->landmine_knockback_meters(),
            -1
          );
          g.run.landmines.UnstableRemoveAt(i - off);
          off++;
        }
      }
    }

    // Gardens spawn fruits.
    {  ///
      auto interval = GARDEN_FRUIT_SPAWNING_INTERVAL;
      interval.value /= GetPlayerStatStructureAttackSpeedMultiplier();
      interval.value = MAX(2, interval.value);

      for (const auto& garden : g.run.gardens) {
        if ((garden.createdAt.Elapsed().value + 1) % interval.value == 0) {
          Vector2 pos{};
          do {
            const f32 off = Lerp(
              GARDEN_PICKUPABLE_SPAWN_RADIUS_MIN,
              GARDEN_PICKUPABLE_SPAWN_RADIUS_MAX,
              GRAND.FRand()
            );
            pos = garden.pos + Vector2Rotate({off, 0}, 2 * PI32 * GRAND.FRand());
          } while (!creaturesWorldSpawnBounds.ContainsInside(pos));

          MakePickupable({
            .type = PickupableType_CONSUMABLE,
            .pos  = pos,
          });
        }
      }
    }

    // Processing `projectilesToRemove`.
    if (g.run.projectilesToRemove.count > 0) {  ///
      ZoneScopedN("Processing `projectilesToRemove`.");

      qsort(
        (void*)g.run.projectilesToRemove.base,
        g.run.projectilesToRemove.count,
        sizeof(*g.run.projectilesToRemove.base),
        (int (*)(const void*, const void*))IntCmp
      );
      FOR_RANGE (int, i, g.run.projectilesToRemove.count) {
        const auto projectileIndex
          = g.run.projectilesToRemove[g.run.projectilesToRemove.count - i - 1];
        g.run.projectiles.UnstableRemoveAt(projectileIndex);
      }
      g.run.projectilesToRemove.Reset();
    }

    // Processing `justDamagedCreatures`.
    {  ///
      ZoneScopedN("Processing `justDamagedCreatures`.");

      // auto playerHurt = false;
      // auto mobHurt    = false;

      for (const auto index : g.run.justDamagedCreatures) {
        auto& creature = g.run.creatures[index];

        const auto fb = fb_creatures->Get(creature.type);

        // if (index == 0)
        //   playerHurt = true;
        // else
        //   mobHurt = true;

        if (creature.health <= 0) {
          DestroyBody(&creature.body);
          creature.diedAt.SetNow();

          if (!index) {
            // Player died.
            if (!g.run.scheduledWaveCompleted.IsSet()) {
              g.run.scheduledWaveCompleted.SetNow();
              g.run.state.waveWon = false;
              Save();
            }
          }
          else {
            // Mob died.

            // Spawning children if mob spawns them on death.
            if (fb->on_death_spawns_creature_type() && CanSpawnMoreCreatures()) {
              int toSpawn = GRAND.RandInt(
                fb->on_death_spawns_count_min(), fb->on_death_spawns_count_max()
              );
              FOR_RANGE (int, i, toSpawn) {
                Vector2 pos{};
                do {
                  const auto t1 = GRAND.FRand();
                  pos           = creature.pos
                        + Vector2Rotate(
                          Vector2(
                            Lerp(
                              fb->on_death_spawns_distance_min(),
                              fb->on_death_spawns_distance_max(),
                              t1
                            ),
                            0
                          ),
                          GRAND.FRand() * 2.0f * PI32
                        );
                } while (!creaturesWorldSpawnBounds.ContainsInside(pos));

                PreSpawn spawn{
                  .type         = PreSpawnType_CREATURE,
                  .typeCreature = (CreatureType)fb->on_death_spawns_creature_type(),
                  .pos          = pos,
                };
                spawn.createdAt.SetNow();
                *g.run.preSpawns.Add() = spawn;
              }
            }

            // Mob drops coin / consumable / crate.
            if (!creature.killedBecauseOfTheEndOfTheWave) {
              MakePickupableData data{
                .type        = PickupableType_COIN,
                .pos         = creature.pos,
                .coin_amount = fb->coins_dropped(),
              };
              MakePickupable(data);

              const auto luckFactor = GetLuckFactor();
              if (GRAND.FRand() <= fb->consumable_drop_chance() * luckFactor) {
                data.type = PickupableType_CONSUMABLE;

                const auto crateChance = fb->crate_instead_of_consumable_factor()
                                         * luckFactor
                                         / (f32)(1 + g.run.cratesDroppedThisWave);
                if (GRAND.FRand() <= crateChance) {
                  data.type = PickupableType_CRATE;
                  g.run.cratesDroppedThisWave++;
                }

                MakePickupable(data);
              }
            }

            // Counting mob as player killed.
            if (!creature.killedBecauseOfTheEndOfTheWave) {
              g.run.state.playerKilledEnemies++;
              AchievementAdd(AchievementType_KILLER, 1);

              if (creature.lastDamagedWeaponIndex >= 0)
                g.run.state.weapons[creature.lastDamagedWeaponIndex].killedEnemies++;

              IterateOverEffects(
                EffectConditionType_KILL_N_ENEMIES_GET_STAT,
                [&](auto fb_effect, int times) BF_FORCE_INLINE_LAMBDA {
                  if (g.run.state.playerKilledEnemies % fb_effect->condition_value() == 0)
                    ApplyEffect(fb_effect, times);
                }
              );
              IterateOverWeaponEffects(
                EffectConditionType_KILL_N_ENEMIES_USING_THIS_WEAPON_GET_STAT,
                [&](int _, Weapon& weapon, auto fb_effect) BF_FORCE_INLINE_LAMBDA {
                  if (weapon.killedEnemies % fb_effect->condition_value() == 0)
                    ApplyEffect(fb_effect, 1);
                }
              );
            }

            // Wave gets set to completed upon killing boss.
            if (fb->is_boss()) {
              if (!g.run.scheduledWaveCompleted.IsSet()) {
                g.run.scheduledWaveCompleted.SetNow();
                g.run.state.waveWon = true;
                Save();
              }
            }
          }
        }
      }

      // if (playerHurt)
      //   PlaySound(Sound_GAME_PLAYER_HURT);
      // if (mobHurt)
      //   PlaySound(Sound_GAME_HURT);

      g.run.justDamagedCreatures.Reset();
    }

    // Removing old died creatures.
    {  ///
      ZoneScopedN("Removing old died creatures.");

      const int total = g.run.creatures.count;
      int       off   = 0;

      FOR_RANGE (int, i, total) {
        auto& creature = g.run.creatures[i - off];
        if ((creature.type != CreatureType_PLAYER)  //
            && creature.diedAt.IsSet()              //
            && (creature.diedAt.Elapsed() >= DIE_FRAMES))
        {
          g.run.creatures.UnstableRemoveAt(i - off);
          off++;
        }
      }
    }

    // Removing old damage numbers.
    {  ///
      ZoneScopedN("Removing old damage numbers.");

      int removed = 0;
      int left    = -1;
      FOR_RANGE (int, i, g.run.numbers.count) {
        const auto& number = g.run.numbers[i];
        if (number.createdAt.Elapsed() >= DAMAGE_NUMBERS_FRAMES) {
          if (left == -1)
            left = i;
          removed++;
        }
        else if (left >= 0)
          g.run.numbers[left++] = number;
      }
      g.run.numbers.count -= removed;
    }

    if (g.run.state.screen != ScreenType_WAVE_END_ANIMATION)
      ASSERT(g.run.state.notPickedUpCoins == g.run.state.notPickedUpCoinsVisual);

    // Removing old picked up pickupables.
    {  ///
      ZoneScopedN("Removing old picked up pickupables.");

      const auto total = g.run.pickupables.count;
      int        off   = 0;
      FOR_RANGE (int, i, total) {
        const auto& pickupable = g.run.pickupables[i - off];
        if (pickupable.pickedUpAt.IsSet()
            && (pickupable.pickedUpAt.Elapsed() >= PICKUPABLE_FADE_FRAMES))
        {
          g.run.pickupables.UnstableRemoveAt(i - off);
          off++;
        }
        else if (pickupable.startedFlyingAt.IsSet()
                 && (pickupable.startedFlyingAt.Elapsed() >= WAVE_COMPLETED_COINS_FLYING_FRAMES))
        {
          const auto& d = pickupable.DataCoin();
          g.run.state.notPickedUpCoinsVisual += d.amount;
          g.run.pickupables.UnstableRemoveAt(i - off);
          off++;
        }
      }
    }

    // Removing old particles.
    {  ///
      ZoneScopedN("Removing old particles.");

      const auto total = g.run.particles.count;
      int        off   = 0;
      FOR_RANGE (int, i, total) {
        const auto& particle = g.run.particles[i - off];
        const auto  fb       = fb_particles->Get(particle.type);
        if (particle.createdAt.Elapsed() >= lframe::FromSeconds(fb->duration_seconds())) {
          g.run.particles.UnstableRemoveAt(i - off);
          off++;
        }
      }
    }

    // Sorting particles.
    {  ///
      ZoneScopedN("Sorting particles.");

      qsort(
        (void*)g.run.particles.base,
        g.run.particles.count,
        sizeof(*g.run.particles.base),
        (int (*)(const void*, const void*))ParticleCmp
      );
    }

    g.run.camera.pos = GetCameraTargetPos();
    ge.meta.frameGame++;
  }

  DoUI(false);

  // Removing excessive data from `g.ui.buttonStates`.
  {  ///
    const int total = g.ui.buttonStatesCount;
    int       off   = 0;
    FOR_RANGE (int, i, total) {
      if (g.ui.buttonStates[i - off].thisFrameUpdated)
        g.ui.buttonStates[i - off].thisFrameUpdated = false;
      else {
        if (i - off != g.ui.buttonStatesCount - 1)
          g.ui.buttonStates[i - off] = g.ui.buttonStates[g.ui.buttonStatesCount - 1];
        g.ui.buttonStatesCount--;
        g.ui.buttonStates[g.ui.buttonStatesCount] = {};
        off++;
      }
    }
  }

  // Removing old unlocked achievements.
  if (g.ui.justUnlockedAchievements.count > 0) {  ///
    auto& x = g.ui.justUnlockedAchievements[0];
    if (!x.shownAt.IsSet())
      x.shownAt.SetNow();
    else if (x.shownAt.Elapsed() >= ACHIEVEMENT_TOTAL_FRAMES)
      g.ui.justUnlockedAchievements.RemoveAt(0);
  }

  ge.meta.frameVisual++;
}

int GetTextureIdByProgress(const flatbuffers::Vector<int>* texs, f32 p) {  ///
  ASSERT(p >= 0);
  int index = p * texs->size();
  index     = MIN(index, texs->size() - 1);
  return texs->Get(index);
}

void GameDraw() {
  ZoneScoped;

  // Setup.
  // {  ///
  TEMP_USAGE(&g.meta.trashArena);

  const auto localization         = glib->localizations()->Get(ge.meta.localization);
  const auto localization_strings = localization->strings();

  const auto fb_atlas_textures = glib->atlas_textures();
  const auto fb_hostilities    = glib->hostilities();
  const auto fb_preSpawns      = glib->pre_spawns();
  const auto fb_creatures      = glib->creatures();
  const auto fb_weapons        = glib->weapons();
  const auto fb_projectiles    = glib->projectiles();
  const auto fb_pickupables    = glib->pickupables();
  const auto fb_numbers        = glib->numbers();
  const auto fb_particles      = glib->particles();
  // }

  BeginMode2D(&g.run.camera);

  // Drawing floor.
  {  ///
    DrawGroup_OneShotRect(
      {
        .pos   = (Vector2)WORLD_SIZE / 2.0f,
        .size  = (Vector2)WORLD_SIZE,
        .color = ColorFromRGBA(0x362316ff),
      },
      DrawZ_FLOOR
    );
  }

  // Drawing prespawns.
  {  ///
    const auto texId = glib->decal_pre_spawn_texture_id();

    DrawGroup_Begin(DrawZ_FLOOR_DECALS);
    DrawGroup_SetSortY(0);

    for (auto& spawn : g.run.preSpawns) {
      ASSERT(spawn.type);
      if (spawn.type == PreSpawnType_CREATURE)
        ASSERT(spawn.typeCreature);

      u32 color{};

      switch (spawn.type) {
      case PreSpawnType_CREATURE: {
        const auto fb_creature = fb_creatures->Get(spawn.typeCreature);
        color = fb_hostilities->Get(fb_creature->hostility_type())->color();
      } break;

      case PreSpawnType_LANDMINE:
      case PreSpawnType_GARDEN: {
        const auto fb = fb_preSpawns->Get(spawn.type);
        color         = fb_hostilities->Get(fb->hostility_type())->color();
      } break;

      default:
        INVALID_PATH;
        break;
      }

      DrawGroup_CommandTexture({
        .texId = texId,
        .pos   = spawn.pos,
        .color = ColorFromRGBA(color),
      });
    }

    DrawGroup_End();
  }

  // Drawing creatures.
  int bossCreatureIndex = -1;
  int creatureIndex     = -1;
  for (const auto& creature : g.run.creatures) {  ///
    creatureIndex++;
    const auto fb = fb_creatures->Get(creature.type);

    if (fb->is_boss())
      bossCreatureIndex = creatureIndex;

    f32 fade = 1;
    if (creature.diedAt.IsSet())
      fade = Clamp01(1 - creature.diedAt.Elapsed().Progress(DIE_FRAMES));

    int texId = 0;
    if (fb->move_texture_ids() && !creature.idleStartedAt.IsSet()) {
      texId = GetTextureIdByProgress(
        fb->move_texture_ids(),
        creature.movementAccumulator / fb->movement_accumulator_meters_cycle()
      );
    }
    else {
      f32 p = 0.0f;
      if (creature.idleStartedAt.IsSet()) {
        const auto idleDuration = lframe::Unscaled(fb->idle_seconds() * FIXED_FPS);
        p = fmodf(creature.idleStartedAt.Elapsed().Progress(idleDuration), 1);
      }
      texId = GetTextureIdByProgress(fb->idle_texture_ids(), p);
    }

    Vector2 scale{1, 1};
    if (creature.dir.x < 0)
      scale.x = -1;

    auto color = ColorFromRGBA(fb->color());
    if (creature.type == CreatureType_RANGER) {
      const auto& data = creature.DataRanger();
      f32         t    = 0;
      if (data.startedShootingAt.IsSet()) {
        auto e = data.startedShootingAt.Elapsed();
        if (e < MOB_RANGER_SHOOTING_FRAME)
          t = e.Progress(MOB_RANGER_SHOOTING_FRAME);
        else {
          t = 1
              - (e - MOB_RANGER_SHOOTING_FRAME)
                  .Progress(MOB_RANGER_SHOOTING_FRAMES - MOB_RANGER_SHOOTING_FRAME);
        }
      }
      t     = Clamp01(t);
      color = ColorLerp(color, palRed, t);
    }
    else if (creature.type == CreatureType_RUSHER) {
      const auto& data = creature.DataRusher();
      f32         t    = 0;
      if (data.startedRushingAt.IsSet()) {
        auto e = data.startedRushingAt.Elapsed();
        if (e < MOB_RUSHER_RUSH_PRE_FRAMES)
          t = e.Progress(MOB_RUSHER_RUSH_PRE_FRAMES);
        else {
          t = 1
              - (e - MOB_RUSHER_RUSH_TOTAL_FRAMES + MOB_RUSHER_RUSH_PRE_FRAMES)
                  .Progress(MOB_RUSHER_RUSH_POST_FRAMES);
        }
      }
      t     = Clamp01(t);
      color = ColorLerp(color, palRed, t);
    }

    auto flash = TRANSPARENT_BLACK;
    if ((DAMAGED_FLASHING_TIMES > 0) && creature.lastDamagedFlashAt.IsSet()) {
      const auto e_ = creature.lastDamagedFlashAt.Elapsed();

      if (e_ < DAMAGED_FLASHING_FRAMES) {
        auto e          = (f32)e_.value;
        bool isFlashing = true;
        FOR_RANGE (int, i, DAMAGED_FLASHING_TIMES * 2 - 1) {
          if (isFlashing)
            e -= DAMAGED_FLASH_PRECALC_X * DAMAGED_FLASH_NOT_FLASH_RATIO;
          else
            e -= DAMAGED_FLASH_PRECALC_X * (1 - DAMAGED_FLASH_NOT_FLASH_RATIO);

          isFlashing = !isFlashing;
          if (e < 0)
            break;
        }

        if (!isFlashing)
          flash = WHITE;
      }
    }

    DrawGroup_OneShotTexture(
      {
        .texId = texId,
        .pos   = creature.pos,
        .scale = scale,
        .color = Fade(color, fade),
        .flash = flash,
      },
      DrawZ_DEFAULT
    );

    {
      f32 scale = 1;
      int texId = glib->shadow_texture_ids()->Get(0);
      for (auto t : *glib->shadow_texture_ids()) {
        const f32 sx = glib->original_texture_sizes()->Get(t)->x()
                       * ASSETS_TO_LOGICAL_RATIO / (f32)METER_LOGICAL_SIZE;

        if (!texId) {
          texId = t;
          scale = CREATURE_COLLIDER_RADIUS * 2 / sx;
        }

        if (sx >= CREATURE_COLLIDER_RADIUS * 2) {
          texId = t;
          scale = CREATURE_COLLIDER_RADIUS * 2 / sx;
        }
        else
          break;
      }
      DrawGroup_OneShotTexture(
        {
          .texId = texId,
          .pos   = creature.pos + Vector2(0, fb->shadow_offset_y()),
          .scale = Vector2One() * scale,
          .color = Fade(WHITE, fade * 0.33f),
        },
        DrawZ_FLOOR_SHADOWS
      );
    }

    if (creature.type == CreatureType_PLAYER) {
      int weaponsCount = 0;
      for (const auto& weapon : g.run.state.weapons) {
        if (weapon.type)
          weaponsCount++;
      }

      // Drawing player's weapons.
      FOR_RANGE (int, i, PLAYER_WEAPONS_COUNT) {
        const auto& weapon = g.run.state.weapons[i];
        if (!weapon.type)
          continue;

        const auto fb = fb_weapons->Get(weapon.type);

        auto pos = GetWeaponPos(i);
        if (!fb->projectile_type() && weapon.startedShootingAt.IsSet()) {
          const auto dur = ApplyAttackSpeedToDuration(fb->shooting_duration_frames());
          const f32  t   = InOutLerp(
            0,
            1,
            (f32)weapon.startedShootingAt.Elapsed().value,
            (f32)dur.value,
            (f32)dur.value / 6
          );
          pos = Vector2Lerp(PLAYER_CREATURE.pos + GetPlayerWeaponOffset(i), pos, t);
        }

        f32     rotation = 0;
        Vector2 scale{0, 1};

        if (weapon.targetDir == Vector2Zero())
          scale.x = (creature.dir.x >= 0 ? 1 : -1);
        else {
          scale.x  = (weapon.targetDir.x >= 0 ? 1 : -1);
          rotation = Vector2Angle(weapon.targetDir) + ((scale.x < 0) ? (f32)PI32 : 0.0f);
        }

        DrawGroup_OneShotTexture(
          {
            .texId    = fb->texture_ids()->Get(0),
            .rotation = rotation,
            .pos      = pos,
            .scale    = scale,
            .color    = Fade(ColorFromRGBA(fb->color()), fade),
          },
          PLAYER_WEAPONS_DRAW_Z[weaponsCount - 1][i]
        );
      }
    }
  }

  // Drawing landmines.
  {  ///
    const auto texId = glib->landmine_texture_id();
    for (const auto& v : g.run.landmines)
      DrawGroup_OneShotTexture({.texId = texId, .pos = v.pos}, DrawZ_LANDMINES);
  }

  // Drawing gardens.
  {  ///
    const auto texId = glib->garden_texture_id();
    for (const auto& v : g.run.gardens)
      DrawGroup_OneShotTexture({.texId = texId, .pos = v.pos}, DrawZ_DEFAULT);
  }

  // Drawing boss hp bar.
  if (bossCreatureIndex >= 0) {  ///
    const auto& creature = g.run.creatures[bossCreatureIndex];
    auto        texs     = glib->ui_health_boss_texture_ids();

    DrawGroup_Begin(DrawZ_BOSS_HP);
    DrawGroup_SetSortY(0);

    FOR_RANGE (int, i, 2) {
      f32 rightMargin = 0;
      if (i)
        rightMargin = Clamp01(1 - (f32)creature.health / (f32)creature.maxHealth);
      DrawGroup_CommandTexture({
        .texId = texs->Get(i),
        .pos   = creature.pos - Vector2(0, 1),
        .sourceMargins{.right = rightMargin},
        .color = (i ? palRed : palWhite),
      });
    }

    DrawGroup_End();
  }

  // Drawing projectiles + their gizmos.
  for (const auto& projectile : g.run.projectiles) {  ///
    const auto fb = fb_projectiles->Get(projectile.type);

    f32     rotation = Vector2Angle(projectile.dir);
    Vector2 scale{1, 1};
    if (projectile.dir.x < 0) {
      rotation += (f32)PI32;
      scale.x = -1;
    }

    DrawGroup_OneShotTexture(
      {
        .texId    = fb->texture_ids()->Get(0),
        .rotation = rotation,
        .pos      = projectile.pos,
        .scale    = scale,
        .color    = ColorFromRGBA(fb->color()),
      },
      DrawZ_PROJECTILES
    );

    // Gizmos.
    if (ge.meta.debugEnabled) {
      DrawGroup_OneShotCircleLines({
        .pos    = projectile.pos,
        .radius = fb->collider_radius(),
        .color  = YELLOW,
      });
    }
  }

  // Drawing numbers.
  {  ///
    DrawGroup_Begin(DrawZ_DAMAGE_NUMBERS);
    DrawGroup_SetSortY(0);

    for (const auto& number : g.run.numbers) {
      ASSERT(number.type);
      const auto fb = fb_numbers->Get(number.type);

      const char* buffer     = nullptr;
      int         bytesCount = 0;

      if (number.type == NumberType_DODGE) {
        const auto fb = localization_strings->Get(Loc_UI_DODGE__CAPS);
        buffer        = fb->c_str();
        bytesCount    = fb->size();
      }
      else if (number.type == NumberType_LEVEL_UP) {
        const auto fb = localization_strings->Get(Loc_UI_LEVEL_UP_NUMBER__CAPS);
        buffer        = fb->c_str();
        bytesCount    = fb->size();
      }
      else {
        const char* format = "%d";
        if (number.type == NumberType_PICKUPABLE)
          format = "x%d";

        const auto text = TextFormat(format, MAX(1, number.value));
        bytesCount      = strlen(text);
        buffer          = ALLOCATE_ARRAY(&g.meta.trashArena, char, bytesCount + 1);
        memcpy((void*)buffer, text, bytesCount + 1);
      }

      const auto e    = number.createdAt.Elapsed();
      const auto p    = Clamp01(e.Progress(DAMAGE_NUMBERS_FRAMES));
      const auto fade = Clamp01(InOutLerp(
        0, 1, e.value, DAMAGE_NUMBERS_FRAMES.value, DAMAGE_NUMBERS_FADE_FRAMES.value
      ));

      DrawGroup_CommandText({
        .pos        = number.pos + Vector2(0, EaseABitUpThenDown(p) / 4.0f),
        .scale      = Vector2(1, 1) * (p * 2),
        .font       = &g.meta.fontUIOutlined,
        .text       = buffer,
        .bytesCount = (int)bytesCount,
        .color      = Fade(ColorFromRGBA(fb->color()), fade),
      });
    }

    DrawGroup_End();
  }

  // Drawing particles.
  {  ///
    DrawGroup_Begin(DrawZ_PARTICLES);
    DrawGroup_SetSortY(0);
    for (const auto& particle : g.run.particles) {
      ASSERT(particle.type);
      const auto fb  = fb_particles->Get(particle.type);
      const auto dur = lframe::FromSeconds(fb->duration_seconds());
      const auto p   = Clamp01(particle.createdAt.Elapsed().Progress(dur));
      DrawGroup_CommandTexture({
        .texId = fb->texture_ids()->Get(0),
        .pos   = particle.pos,
        .scale = Vector2One() * particle.scale,
        .color = Fade(WHITE, EaseOutQuad(1 - p)),
      });
    }
    DrawGroup_End();
  }

  // Gizmos. Colliders.
  if (ge.meta.debugEnabled) {  ///
    for (auto& shape : g.run.bodyShapes) {
      if (!shape.active)
        continue;

      auto pos = ToVector2(b2Body_GetPosition(shape.body.id));

      auto color = shape.color;
      if (!b2Body_IsEnabled(shape.body.id))
        color = GRAY;

      switch (shape.type) {
      case BodyShapeType_CIRCLE: {
        DrawGroup_OneShotCircleLines({
          .pos    = pos,
          .radius = shape.DataCircle().radius,
          .color  = color,
        });
      } break;

      case BodyShapeType_RECT: {
        DrawGroup_OneShotRectLines({
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

  // Drawing pickupables.
  for (const auto& pickupable : g.run.pickupables) {  ///
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

    auto pos = WorldToLogical(pickupable.pos, &g.run.camera);
    if (pickupable.startedFlyingAt.IsSet()) {
      auto p
        = pickupable.startedFlyingAt.Elapsed().Progress(WAVE_COMPLETED_COINS_FLYING_FRAMES
        );
      p   = Clamp01(p);
      p   = EaseInQuad(p);
      pos = Vector2Lerp(pos, g.ui.notPickedUpCoinsLogicalPos, p);
    }

    DrawGroup_OneShotTexture(
      {
        .texId = fb->texture_id(),
        .pos   = pos,
        .color = Fade(WHITE, fade),
      },
      DrawZ_PICKUPABLES
    );
  }

  // Drawing wave completion animation.
  if (g.run.scheduledWaveCompleted.IsSet()) {  ///
    auto p
      = Clamp01(g.run.scheduledWaveCompleted.Elapsed().Progress(WAVE_COMPLETED_FRAMES));

    int locale = Loc_UI_WAVE_WON__CAPS;
    if (!g.run.state.waveWon)
      locale = Loc_UI_WAVE_LOST__CAPS;
    if (g.run.state.waveWon && (g.run.state.waveIndex >= TOTAL_WAVES - 1))
      locale = Loc_UI_WON__CAPS;
    auto text = localization_strings->Get(locale);

    int totalChars = 0;
    IterateOverCodepoints(
      text->c_str(),
      text->size(),
      [&totalChars](u32 codepoint, u32 _codepointSize) BF_FORCE_INLINE_LAMBDA {
        if (codepoint)
          totalChars++;
      }
    );
    int bytesToShow = 0;

    p               = InOutLerp(0, 1, p, 1, 0.33f);
    int charsToShow = MIN(totalChars, Ceil((f32)totalChars * p));

    IterateOverCodepoints(
      text->c_str(),
      text->size(),
      [&charsToShow, &bytesToShow](u32 codepoint, u32 codepointSize)
        BF_FORCE_INLINE_LAMBDA {
          if (codepoint && charsToShow) {
            bytesToShow += codepointSize;
            charsToShow--;
          }
        }
    );

    if (bytesToShow) {
      DrawGroup_OneShotText(
        {
          .pos{
            (f32)LOGICAL_RESOLUTION.x / 2.0f,
            (f32)LOGICAL_RESOLUTION.y * 3.0f / 4.0f,
          },
          .font       = &g.meta.fontWaveCompletion,
          .text       = text->c_str(),
          .bytesCount = bytesToShow,
        },
        DrawZ_UI
      );
    }
  }

  DoUI(true);

  EngineApplyStrips();
  EngineApplyVignette();
  FlushDrawCommands();

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
    if (g.run.state.screen == ScreenType_GAMEPLAY) {
      DebugText("F7 show end screen");
      DebugText("F8 add level");
      DebugText("F9 add crate");
    }

    DebugText("N increase wave");

    DebugText(TextFormat("%.2f", b2Body_GetLinearVelocity(PLAYER_CREATURE.body.id).x));

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
    debugTextArena("g.run.arena", g.run.arena);
    debugTextArena("g.meta.trashArena", g.meta.trashArena);

#define X(type_, name_) DebugText("g.run." #name_ ".count: %d", g.run.name_.count);
    VECTORS_TABLE;
#undef X
  }
}

///
