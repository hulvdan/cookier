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

// !banner: clay
//  ██████╗██╗      █████╗ ██╗   ██╗
// ██╔════╝██║     ██╔══██╗╚██╗ ██╔╝
// ██║     ██║     ███████║ ╚████╔╝
// ██║     ██║     ██╔══██║  ╚██╔╝
// ╚██████╗███████╗██║  ██║   ██║
//  ╚═════╝╚══════╝╚═╝  ╚═╝   ╚═╝

// {  ///
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

#define BF_CLAY_CUSTOM_BEGIN \
  .custom {                  \
    .customData = PushClayCustomData(

#define BF_CLAY_CUSTOM_END ) \
  }

#define BF_CLAY_CUSTOM_SHADOW(gamelibNineSlicePtr_, enabled_) \
  .shadow {                                                   \
    .set = (enabled_), .nineSlice = (gamelibNineSlicePtr_),   \
  }

struct Breathing {  ///
  bool        set            = false;
  FrameVisual bonusBreatheAt = {};
};

#define BF_CLAY_CUSTOM_NINE_SLICE(gamelibNineSlicePtr_, tier_, enabled_, breathing_) \
  .nineSlice {                                                                       \
    .set = enabled_, .breathing = breathing_, .nineSlice = (gamelibNineSlicePtr_),   \
    .nineSliceColor = slotColors[(tier_) * 2],                                       \
    .nineSliceFlash = slotColors[(tier_) * 2 + 1],                                   \
  }

#define BF_CLAY_CUSTOM_OVERLAY(color_) \
  .overlay {                           \
    .set = true, .color = (color_)     \
  }

#define BF_CLAY_CUSTOM_SCREEN_BACKGROUND \
  .screenBackground {                    \
    .set = true                          \
  }

struct Beautify {
  f32     alpha     = 1;
  Vector2 translate = {0, 0};
  Vector2 scale     = {1, 1};
};

#define BEAUTIFY(beautify_)                                                         \
  ASSERT((beautify_).alpha >= 0);                                                   \
  ASSERT((beautify_).alpha <= 1);                                                   \
                                                                                    \
  CLAY({                                                                            \
    BF_CLAY_CUSTOM_BEGIN{                                                           \
      .beautifierStart{                                                             \
        .set       = true,                                                          \
        .alpha     = (beautify_).alpha,                                             \
        .translate = (beautify_).translate,                                         \
        .scale     = (beautify_).scale,                                             \
      },                                                                            \
    } BF_CLAY_CUSTOM_END,                                                           \
  }) {}                                                                             \
  beautifiers[beautifiersCount++] = (beautify_);                                    \
                                                                                    \
  DEFER {                                                                           \
    CLAY({BF_CLAY_CUSTOM_BEGIN{.beautifierEnd{.set = true}} BF_CLAY_CUSTOM_END}) {} \
    beautifiersCount--;                                                             \
  };

#define FLOATING_BEAUTIFY FLOATING_BEAUTIFY_CONDITIONAL(true)

#define FLOATING_BEAUTIFY_CONDITIONAL(cond)    \
  f32     currentAlpha_ = 1;                   \
  Vector2 currentTranslate_{0, 0};             \
  Vector2 currentScale_{1, 1};                 \
  if (cond) {                                  \
    FOR_RANGE (int, i, beautifiersCount) {     \
      auto& b = beautifiers[i];                \
      currentAlpha_ *= b.alpha;                \
      currentTranslate_ += b.translate;        \
      currentScale_ *= b.scale;                \
    }                                          \
  }                                            \
  Beautify currentBeautify_{                   \
    .alpha     = currentAlpha_,                \
    .translate = currentTranslate_,            \
    .scale     = currentScale_,                \
  };                                           \
                                               \
  CLAY({BF_CLAY_CUSTOM_BEGIN{.beautifierStart{ \
    .set       = true,                         \
    .alpha     = currentBeautify_.alpha,       \
    .translate = currentBeautify_.translate,   \
    .scale     = currentBeautify_.scale,       \
  }} BF_CLAY_CUSTOM_END}) {}                   \
  DEFER{CLAY({BF_CLAY_CUSTOM_BEGIN{.beautifierEnd{.set = true}} BF_CLAY_CUSTOM_END}){}};

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
  int     texID         = {};
  Vector2 offset        = {};
  Vector2 scale         = {1, 1};
  Vector2 anchor        = {0.5f, 0.5f};
  Margins sourceMargins = {0, 0};
  Color   color         = WHITE;
  Color   flash         = TRANSPARENT_BLACK;

  bool    dontCareAboutScaleWhenCalculatingSize = false;
  Vector2 overriddenSize                        = {};  // Values must be > 0 to override.

  // f32   scale     = {};
  // ImageFitType fitType   = {};
};

struct ClayCustomData {
  struct {
    bool    set       = false;
    f32     alpha     = 1;
    Vector2 translate = {0, 0};
    Vector2 scale     = {1, 1};
  } beautifierStart = {};

  struct {
    bool set = false;
  } beautifierEnd = {};

  struct {
    bool  set   = false;
    Color color = MAGENTA;
  } overlay = {};

  struct {
    bool set = false;
  } screenBackground;

  struct {
    bool                     set       = false;
    const BFGame::NineSlice* nineSlice = nullptr;
  } shadow = {};

  struct {
    bool                     set            = false;
    Breathing                breathing      = {};
    const BFGame::NineSlice* nineSlice      = nullptr;
    Color                    nineSliceColor = WHITE;
    Color                    nineSliceFlash = TRANSPARENT_BLACK;
  } nineSlice = {};
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

f32 Vector2AngleOrRandom(Vector2 v) {  ///
  if (v == Vector2Zero())
    return GRAND.Angle();
  return Vector2Angle(v);
}

Vector2 Vector2DirectionOrRandom(Vector2 from, Vector2 to) {  ///
  if (from == to)
    return Vector2Rotate({1, 0}, GRAND.Angle());
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

enum ShapeCategory : u32 {  ///
  ShapeCategory_STATIC     = 1 << 0,
  ShapeCategory_PLAYER     = 1 << 1,
  ShapeCategory_CREATURE   = 1 << 2,
  ShapeCategory_PROJECTILE = 1 << 3,
};

enum BodyType : u32 {  ///
  BodyType_INVALID,
  BodyType_STATIC,
  BodyType_CREATURE,
};

enum ShapeUserDataType : u32 {  ///
  ShapeUserDataType_INVALID,
  ShapeUserDataType_STATIC,
  ShapeUserDataType_CREATURE,
};

struct ShapeUserData {  ///
  ShapeUserDataType type   = {};
  int               _value = {};

  static ShapeUserData Static() {
    return {.type = ShapeUserDataType_STATIC};
  }

  static ShapeUserData Creature(int value) {
    ASSERT(value >= 0);
    return {
      .type   = ShapeUserDataType_CREATURE,
      ._value = value,
    };
  }

  int GetCreatureID() const {
    ASSERT(type == ShapeUserDataType_CREATURE);
    return _value;
  }

  static ShapeUserData FromPointer(void* ptr) {
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

  void* ToPointer() {
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

struct Body {  ///
  int      createdID = {};
  b2BodyId id        = {};
};

enum BodyShapeType {  ///
  BodyShapeType_INVALID,
  BodyShapeType_CIRCLE,
  BodyShapeType_RECT,
};

struct BodyShape {  ///
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

  auto& DataCircle() {
    ASSERT(type == BodyShapeType_CIRCLE);
    return _u._circle;
  }

  const auto& DataCircle() const {
    ASSERT(type == BodyShapeType_CIRCLE);
    return _u._circle;
  }

  auto& DataRect() {
    ASSERT(type == BodyShapeType_RECT);
    return _u._rect;
  }

  const auto& DataRect() const {
    ASSERT(type == BodyShapeType_RECT);
    return _u._rect;
  }
};

struct MakeBodyData {  ///
  BodyType      type     = {};
  bool          isSensor = false;
  f32           density  = 1.0f;
  ShapeUserData userData = {};
  bool          isPlayer = {};
};

struct Weapon {  ///
  WeaponType type                               = {};
  Vector2    targetDir                          = {};
  FrameGame  startedShootingAt                  = {};
  FrameGame  cooldownStartedAt                  = {};
  int        tier                               = {};
  int        thisWaveDamage                     = 0;
  f32        lastCollisionCheckShootingProgress = 0;

  Array<int, WEAPON_MAX_PIERCE> piercedCreatureIDs = {};
  int                           piercedCount       = 0;

  int thisWaveKilledEnemies = 0;
  int thisWaveUseCount      = 0;

  FrameGame lastShotAt                   = {};
  i64       nextTrailSoundVisualFrame    = {};
  i64       nextUseTrailSoundVisualFrame = {};

  FrameVisual uiBouncedAt = {};
};

struct Item {  ///
  ItemType    type               = {};
  int         count              = {};
  int         thisWaveAddedCount = 0;
  FrameVisual uiBouncedAt        = {};
};

struct AvailableItem {  ///
  ItemType type  = {};
  int      count = {};
};

struct CreatureController {  ///
  Vector2 move = {};
};

struct AilmentCommonData {  ///
  AilmentType type            = AilmentType_INVALID;
  int         weaponIndex     = -1;
  int         initialTimes    = {};
  int         remainingTimes  = {};
  int         remainingSpread = {};
  int         damage          = {};
  FBFlattened damageScalings  = {};
};

struct Ailment {  ///
  AilmentCommonData c         = {};
  FrameGame         startedAt = {};
};

struct Creature;

struct ApplyAilmentData {  ///
  AilmentCommonData c        = {};
  Creature*         creature = {};
};

constexpr int MAX_SIMULTANEOUS_AILMENTS = 12;

constexpr f32 MOVEMENT_CYCLE_METERS = PI32 * 3 / 4;

struct Creature {  ///
  int                id                             = {};
  CreatureType       type                           = {};
  int                health                         = {};
  int                maxHealth                      = {};
  Vector2            pos                            = {};
  Vector2            dir                            = {1, 0};
  Body               body                           = {};
  CreatureController controller                     = {};
  FrameGame          lastDamagedAt                  = {};
  FrameGame          lastDamagedFlashAt             = {};
  FrameGame          spawnedAt                      = {};
  FrameGame          diedAt                         = {};
  f32                speed                          = {};
  f32                speedModifier                  = 1;
  f32                movementAccumulator            = {};
  f32                movementVisualFactor           = {};
  f32                movementAccumulatorVisual      = {};
  FrameGame          idleStartedAt                  = {};
  bool               killedBecauseOfTheEndOfTheWave = false;
  bool               aggroed                        = false;

  PushableArray<Ailment, MAX_SIMULTANEOUS_AILMENTS> ailments      = {};
  int                                               ailmentsCount = 0;

  int lastDamagedWeaponIndex = -1;

  union {
    struct {
      FrameGame   startedShootingAt;
      lframe      cooldown;
      Vector2     aimDirection;
      int         damage;
      FBFlattened damageScalings;
    } turret;

    struct {
      FrameGame startedShootingAt;
    } ranger;

    struct {
      FrameGame startedRushingAt;
      FrameGame finishedRushingAt;
      lframe    cooldown;
      Vector2   rushingDir;
    } rusher;

    struct {
      FrameGame startedShootingAt;
      int       shotTimes;
      lframe    cooldown;
      int       shootingPattern;
      f32       shootingRotationOffset;
    } boss;
  } _mob;

  auto& DataTurret() {
    ASSERT(type == CreatureType_TURRET);
    return _mob.turret;
  };

  const auto& DataTurret() const {
    ASSERT(type == CreatureType_TURRET);
    return _mob.turret;
  };

  auto& DataRanger() {
    ASSERT(type == CreatureType_RANGER);
    return _mob.ranger;
  };

  const auto& DataRanger() const {
    ASSERT(type == CreatureType_RANGER);
    return _mob.ranger;
  };

  auto& DataRusher() {
    ASSERT(type == CreatureType_RUSHER);
    return _mob.rusher;
  };

  const auto& DataRusher() const {
    ASSERT(type == CreatureType_RUSHER);
    return _mob.rusher;
  };

  auto& DataBoss() {
    ASSERT(type == CreatureType_BOSS);
    return _mob.boss;
  };

  const auto& DataBoss() const {
    ASSERT(type == CreatureType_BOSS);
    return _mob.boss;
  };
};

struct MakeCreatureData {  ///
  CreatureType type           = {};
  Vector2      pos            = {};
  int          damage         = {};
  FBFlattened  damageScalings = {};
};

struct PreSpawn {  ///
  PreSpawnType type                    = {};
  CreatureType typeCreature            = {};
  Vector2      pos                     = {};
  FrameGame    createdAt               = {};
  int          damage                  = {};
  FBFlattened  damageScalings          = {};
  f32          rotation                = {};
  lframe       garden_spawnsAppleEvery = {};
};

struct ProjectileFactoryCommonData {  ///
  ProjectileType type                             = {};
  CreatureType   ownerCreatureType                = {};
  int            weaponIndexOrMinus1              = -1;
  Vector2        pos                              = {};
  Vector2        dir                              = {};
  f32            range                            = {};
  int            damage                           = {};
  f32            critDamageMultiplier             = {};
  int            weaponCritChance                 = 0;
  f32            knockbackMeters                  = 0;
  int            pierce                           = 0;
  int            weaponPiercingDamageBonusPercent = 0;
  int            bounce                           = 0;
  bool           canExplode                       = true;
  bool           dontSpawnProjectilesOnHit        = false;
};

struct Projectile {  ///
  ProjectileFactoryCommonData       c                  = {};
  int                               id                 = {};
  f32                               rotationSpeed      = {};
  Array<int, PROJECTILE_MAX_PIERCE> damagedCreatureIDs = {};
  int                               damagedCount       = 0;

  f32       travelledDistance = 0;
  int       piercedCount      = 0;
  int       bouncedCount      = 0;
  FrameGame lastBouncedAt     = {};
  FrameGame createdAt         = {};
  int       effectCritPierce  = 0;
  int       effectCritBounce  = 0;

  i64 nextTrailSoundVisualFrame = {};
};

struct MakeProjectileData {  ///
  ProjectileFactoryCommonData c                        = {};
  int                         alreadyDamagedCreatureID = 0;
};

int ProjectileCmp(const Projectile* v1, const Projectile* v2) {  ///
  return IntCmp(&v1->id, &v2->id);
}

struct Number {  ///
  NumberType type      = {};
  int        value     = {};
  Vector2    pos       = {};
  FrameGame  createdAt = {};
};

struct MakePickupableData {  ///
  PickupableType type        = {};
  Vector2        pos         = {};
  int            coin_amount = {};
};

struct Pickupable {  ///
  PickupableType type            = {};
  u8             variation       = {};
  Vector2        pos             = {};
  f32            rotation        = 0;
  bool           mirrored        = false;
  FrameGame      createdAt       = {};
  FrameGame      pickedUpAt      = {};
  FrameVisual    startedFlyingAt = {};

  struct {
    struct {
      int amount;
    } coin;
  } _u;

  auto& DataCoin() {
    ASSERT(type == PickupableType_COIN);
    return _u.coin;
  }

  const auto& DataCoin() const {
    ASSERT(type == PickupableType_COIN);
    return _u.coin;
  }
};

struct MakeNumberData {  ///
  NumberType type  = {};
  int        value = {};
  Vector2    pos   = {};
};

struct RandomCumulativeChances {  ///
  f32 cumulativeChances[4] = {};
};

RandomCumulativeChances GetRandomCumulativeChances(int waveIndex, int luck) {  ///
  struct {
    int minWaveIndex   = {};
    f32 percentBase    = {};
    f32 percentPerWave = {};
    f32 percentMax     = {};
  } tierChances[TOTAL_TIERS]{
    // 0.
    {
      .minWaveIndex   = 0,
      .percentBase    = 100,
      .percentPerWave = 0,
      .percentMax     = 100,
    },
    // 1.
    {
      .minWaveIndex   = 1,
      .percentBase    = 0,
      .percentPerWave = 6,
      .percentMax     = 60,
    },
    // 2.
    {
      .minWaveIndex   = 3,
      .percentBase    = 0,
      .percentPerWave = 2,
      .percentMax     = 25,
    },
    // 3.
    {
      .minWaveIndex   = 7,
      .percentBase    = 0,
      .percentPerWave = 0.23f,
      .percentMax     = 8,
    },
  };

  RandomCumulativeChances result{};

  f32 prev = 0;
  for (int tierIndex = TOTAL_TIERS - 1; tierIndex >= 0; tierIndex--) {
    const auto& d = tierChances[tierIndex];

    if (waveIndex < d.minWaveIndex)
      continue;

    f32 v = (d.percentPerWave * (waveIndex - d.minWaveIndex) + d.percentBase)
            * (1 + (f32)luck / 100.0f);
    v = MIN(v, d.percentMax) / 100.0f;

    if (v > 0) {
      result.cumulativeChances[tierIndex] = v - prev;
      prev += v - prev;
    }
  }

  for (int i = TOTAL_TIERS - 2; i >= 0; i--) {
    for (int x = i + 1; x < TOTAL_TIERS; x++)
      result.cumulativeChances[x] += result.cumulativeChances[i];
  }

  result.cumulativeChances[TOTAL_TIERS - 1] = f32_inf;
  return result;
}

TEST_CASE ("GetRandomCumulativeChances") {  ///
  auto r1 = GetRandomCumulativeChances(0, 0);
  ASSERT(FloatEquals(r1.cumulativeChances[0], 1));
  ASSERT(FloatEquals(r1.cumulativeChances[1], 1));
  ASSERT(FloatEquals(r1.cumulativeChances[2], 1));
  ASSERT(r1.cumulativeChances[3] == f32_inf);

  auto r2 = GetRandomCumulativeChances(19, 0);
  ASSERT(FloatEquals(r2.cumulativeChances[0], 0.4f));
  ASSERT(FloatEquals(r2.cumulativeChances[1], 0.75f));
  ASSERT(FloatEquals(r2.cumulativeChances[2], 0.9724000096321106f));
  ASSERT(r2.cumulativeChances[3] == f32_inf);
}

enum ScreenType {  ///
  ScreenType_NEW_RUN,
  ScreenType_GAMEPLAY,
  ScreenType_WAVE_END_ANIMATION,
  ScreenType_PICKED_UP_ITEM,
  ScreenType_UPGRADES,
  ScreenType_SHOP,
  ScreenType_END,
  ScreenType_COUNT,
};

const ScreenType PAUSABLE_SCREENS_[]{ScreenType_GAMEPLAY};
VIEW_FROM_ARRAY_DANGER(PAUSABLE_SCREENS);

struct ShopItem {  ///
  WeaponType weapon = {};
  ItemType   item   = {};
  int        tier   = {};
};

struct Upgrade {  ///
  StatType stat = {};
  int      tier = {};
};

struct Rerolls {  ///
  int         rerolledFreeTimes = 0;
  int         rerolledTimes     = 0;
  FrameVisual lastRerolledAt    = {};

  void Roll();
  int  GetPrice() const;
};

struct ThisWaveMob {  ///
  CreatureType type              = {};
  f32          accumulatedFactor = {};
};

struct Placeholder {  ///
  PlaceholderType type = {};
  Placeholder*    next = nullptr;

  union {
    struct {
      const char* value;
      Color       color;
    } string;

    struct {
      int   value;
      Color color;
    } brokenLocale;

    struct {
      int texID;
    } image;
  } _u;

  const auto& string() const {
    ASSERT(type == PlaceholderType_STRING);
    return _u.string;
  }

  const auto& brokenLocale() const {
    ASSERT(type == PlaceholderType_BROKEN_LOCALE);
    return _u.brokenLocale;
  }

  const auto& image() const {
    ASSERT(type == PlaceholderType_IMAGE);
    return _u.image;
  }
};

struct PlaceholderGroup {  ///
  const char*       placeholder = {};
  PlaceholderGroup* next        = nullptr;

  Placeholder* first = nullptr;
  Placeholder* last  = nullptr;
};

struct Particle {  ///
  ParticleType type          = {};
  u16          variation     = {};
  Vector2      pos           = {};
  Vector2      velocity      = {};
  f32          rotation      = {};
  f32          rotationSpeed = 0;
  f32          scale         = 1;
  Color        color         = {};
  lframe       duration      = {};
  FrameGame    createdAt     = {};
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

struct Landmine {  ///
  Vector2     pos                 = {};
  FrameGame   startedDetonationAt = {};
  FrameGame   spawnedAt           = {};
  int         damage              = {};
  FBFlattened damageScalings      = {};
};

struct Garden {  ///
  Vector2   pos              = {};
  lframe    spawnsAppleEvery = {};
  FrameGame spawnedAt        = {};
};

struct UIButtonState {  ///
  Clay_ElementId id               = {};
  bool           thisFrameUpdated = false;
  bool           hovered          = false;
};

struct Build {  ///
  DifficultyType maxDifficultyBeaten = {};
};

struct Achievement {  ///
  i64 value = {};
};

struct JustUnlockedAchievement {  ///
  AchievementType type      = {};
  int             stepIndex = {};
  FrameVisual     shownAt   = {};
};

#define EFFECT_X_INT (fb_effect->placeholders()->Get(0)->ints()->Get(tierOffset))
#define EFFECT_Y_INT (fb_effect->placeholders()->Get(1)->ints()->Get(tierOffset))
#define EFFECT_Z_INT (fb_effect->placeholders()->Get(2)->ints()->Get(tierOffset))
#define EFFECT_W_INT (fb_effect->placeholders()->Get(3)->ints()->Get(tierOffset))
#define EFFECT_X_FLOAT (fb_effect->placeholders()->Get(0)->floats()->Get(tierOffset))
#define EFFECT_Y_FLOAT (fb_effect->placeholders()->Get(1)->floats()->Get(tierOffset))
#define EFFECT_Z_FLOAT (fb_effect->placeholders()->Get(2)->floats()->Get(tierOffset))
#define EFFECT_W_FLOAT (fb_effect->placeholders()->Get(3)->floats()->Get(tierOffset))

constexpr int MAX_BEAUTIFIERS = 32;

enum Direction {  ///
  Direction_NONE,
  Direction_RIGHT,
  Direction_UP,
  Direction_LEFT,
  Direction_DOWN,
};

using ControlsGroupID = int;

struct ControlsEntry {  ///
  Clay_ElementId id = {};

  ControlsEntry* next = {};
  ControlsEntry* prev = {};
};

struct ControlsDimension {  ///
  ControlsEntry* first = {};
  ControlsEntry* last  = {};

  ControlsDimension* next = {};
  ControlsDimension* prev = {};
};

struct ControlsGroup {  ///
  ControlsGroupID id = {};

  int currentDimension = {};

  ControlsDimension* first = {};
  ControlsDimension* last  = {};

  // NOTE: Use `(int)direction - 1` to index.
  ControlsGroupID connectionsPerDirection[4] = {};

  ControlsGroup* next = {};
  ControlsGroup* prev = {};
};

enum ControlsContext {  ///
  ControlsContext_INVALID,
  ControlsContext_ACHIEVEMENTS,
  ControlsContext_NEW_RUN,
  ControlsContext_PICKED_UP_ITEM,
  ControlsContext_UPGRADES,
  ControlsContext_SHOP,
  ControlsContext_END,
  ControlsContext_MODAL_PAUSE,
  ControlsContext_MODAL_SHOP_WEAPON_DETAILS,
  ControlsContext_MODAL_STATS,
  ControlsContext_MODAL_CONFIRM_RESTART,
  ControlsContext_MODAL_CONFIRM_NEW_RUN,
  ControlsContext_MODAL_CONFIRM_QUIT,
  ControlsContext_MODAL_CHEAT_WEAPON,
  ControlsContext_COUNT,
};

const ControlsContext CONTROLS_CONTEXT_MODALS_[]{
  // Lower = more priority.
  ControlsContext_MODAL_SHOP_WEAPON_DETAILS,
  ControlsContext_MODAL_STATS,
  ControlsContext_MODAL_PAUSE,
  ControlsContext_ACHIEVEMENTS,
  ControlsContext_MODAL_CONFIRM_RESTART,
  ControlsContext_MODAL_CONFIRM_NEW_RUN,
  ControlsContext_MODAL_CONFIRM_QUIT,
  ControlsContext_MODAL_CHEAT_WEAPON,
};
VIEW_FROM_ARRAY_DANGER(CONTROLS_CONTEXT_MODALS);

struct LockInfo {  ///
  AchievementType achievement = {};
  int             stepIndex   = -1;
};

enum ConfirmModalResultType {  ///
  ConfirmModalResultType_NONE,
  ConfirmModalResultType_CANCELLED,
  ConfirmModalResultType_CONFIRMED,
};

struct MakeParticlesData {  ///
  ParticleType type = {};

  int count = 1;

  Vector2 pos = {};

  f32 velocity          = 0;
  f32 velocityPlusMinus = 0;

  f32 velocityAngle          = 0;
  f32 velocityAnglePlusMinus = 0;

  f32               initialOffset          = 0;
  f32               initialOffsetPlusMinus = 0;
  Easing_function_t initialOffsetEasing    = EaseLinear;

  f32 scale          = 1;
  f32 scalePlusMinus = 0.2f;

  f32   rotationSpeedPlusMinus = PI32;
  Color color                  = WHITE;
};

struct Prop {  ///
  int     variation = {};
  bool    right     = {};
  Vector2 pos       = {};
  f32     rotation  = {};
};

struct TurretToSpawn {  ///
  int         baseDamage     = {};
  FBFlattened damageScalings = {};
};

struct GardenToSpawn {  ///
  lframe spawnsAppleEvery = {};
};

struct RotatedRect {  ///
  Vector2 pos      = {};
  Vector2 size     = {};
  f32     rotation = {};
};

struct GameData {
  struct Meta {  ///
    Arena trashArena         = {};
    Arena transientDataArena = {};

    // NOTE: Reorder loading upon reordering fonts.
    Font fontUI                 = {};
    Font fontUIOutlined         = {};
    Font fontUIBig              = {};
    Font fontUIBigOutlined      = {};
    Font fontStats              = {};
    Font fontPricesOutlined     = {};
    Font fontItemCountsOutlined = {};
    Font fontUIGiganticOutlined = {};
    Font fontUINextWave         = {};

    LoadFontsResult loadedFonts = {};

    Vector2 screenSizeUI       = {};
    Vector2 screenSizeUIMargin = {};

    struct {
      bool    controlling   = false;
      Vector2 startPos      = {};
      Vector2 targetPos     = {};
      Vector2 calculatedDir = {};
    } stickControl;

    bool        paused                             = false;
    bool        scheduledTogglePause               = false;
    bool        showingAchievements                = false;
    int         achievementsHoveredAchievement     = 0;
    int         achievementsHoveredAchievementStep = 0;
    FrameVisual showingStats                       = {};

    bool        scheduledSave = false;
    FrameVisual lastSaveAt    = {};

    bool playerUsesKeyboardOrController = false;
    f32  pauseButtonFadeProgress        = 1;

    bool confirmingRestart = false;
    bool confirmingNewRun  = false;

    f32 musicLowpassFactor = 1.0f;
  } meta;

  struct Player {  ///
    int runsWon  = 0;
    int runsLost = 0;

    DifficultyType difficulty = DifficultyType_D0;
    BuildType      build      = BuildType_DEFAULT;
    WeaponType     weapon     = {};
    BiomeType      biome      = BiomeType_INVALID;

    int volumeSFX   = 3;
    int volumeMusic = 2;

    Array<Achievement, AchievementType_COUNT> achievements = {};
    Array<Build, BuildType_COUNT>             builds       = {};

    Array<bool, TOTAL_ACHIEVEMENT_STEPS> lockedSteps   = {};
    Array<LockInfo, BuildType_COUNT>     lockedBuilds  = {};
    Array<LockInfo, ItemType_COUNT>      lockedItems   = {};
    Array<LockInfo, WeaponType_COUNT>    lockedWeapons = {};

    int achievementStepsCompleted = 0;
  } player;

  struct Run {  ///
    struct State {
      bool       won    = false;
      ScreenType screen = ScreenType_GAMEPLAY;

      int  waveIndex = 0;
      bool waveWon   = false;

      int levelOnStartOfTheWave = 1;
      f32 xp                    = 0;
      f32 xpOnStartOfTheWave    = 0;
      int previousCoins         = 0;

      int playerKilledEnemies = 0;
      int notPickedUpCoins    = 0;
      int chests              = 0;
      int toSpawn             = 3;

      Array<int, StatType_COUNT>          staticStats = {};
      Array<Weapon, PLAYER_WEAPONS_COUNT> weapons     = {};
      Vector<Item>                        items       = {};

      // NOTE: Downwards goes data associated with different screens (ref: ScreenType).
      struct {
        ItemType toPick = {};
      } pickedUpItem;

      struct {
        Array<Upgrade, 4> toPick = {};
        Rerolls           rerolls;
      } upgrades;

      struct {
        Array<ShopItem, SHOP_SELLING_ITEMS> toPick = {};
        Rerolls                             rerolls;
      } shop;
    } state = {};

    Array<int, StatType_COUNT> dynamicStats = {};

    int runRecycledWeapons   = 0;
    int runHealed            = 0;
    int runDodged            = 0;
    int turretsOnTheMapCount = 0;

    int shopActivatedModalWeaponIndex = -1;

    Arena arena = {};

    bool reload = false;

    FrameVisual scheduledWaveCompleted      = {};
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

    FrameVisual shownScreenAt_[ScreenType_COUNT]{};
    VIEW_FROM_ARRAY_DANGER(shownScreenAt);

    Camera camera{
      .zoom          = METER_LOGICAL_SIZE,
      .texturesScale = 1.0f / METER_LOGICAL_SIZE,
    };

    b2WorldId world          = {};
    int       nextCreatureID = 1;

    Array<ThisWaveMob, CreatureType_COUNT> thisWaveMobs      = {};
    int                                    thisWaveMobsCount = 0;

    Vector2 previousPlayerPos = {};

    FrameGame playerLastLifestealAt             = {};
    FrameGame playerLastRegenAt                 = {};
    f32       playerWalkedMetersCumulativeDelta = 0;
    int       playerWalkedMeters                = 0;
    int       playerContinuousIdleFrames        = 0;
    int       playerContinuousWalkingFrames     = 0;

    int  chestsDroppedThisWave = 0;
    bool cantHeal              = false;

    FrameGame waveStartedAt = {};

    int bossCreatureID = 0;

    bool hideSlotDetails  = false;
    int  cheatWeaponIndex = -1;

    int itemsScrollPause = 0;
    int itemsScrollShop  = 0;
    int itemsScrollEnd   = 0;

    f32 dangerHPLevelOverlayValue = 0;

    RandomCumulativeChances randomTiers = {};

    Array<AvailableItem, ITEMS_PER_TIER_[0]> itemPool0 = {};
    Array<AvailableItem, ITEMS_PER_TIER_[1]> itemPool1 = {};
    Array<AvailableItem, ITEMS_PER_TIER_[2]> itemPool2 = {};
    Array<AvailableItem, ITEMS_PER_TIER_[3]> itemPool3 = {};
    Array<View<AvailableItem>, TOTAL_TIERS>  itemPools = {};

    Array<Array<WeaponType, WeaponType_COUNT>, TOTAL_TIERS> weaponPoolsData = {};
    Array<View<WeaponType>, TOTAL_TIERS>                    weaponPools     = {};

    Array<Prop, PROPS_COUNT> propsData = {};
    View<Prop>               props     = {};

    Array<int, ProjectileType_COUNT> projectileCounts = {};

    bool schedulePlayerHurtSound = false;

    FrameVisual walkingTutorialCompletedAt = {};

    // Using "X-macros". ref: https://www.geeksforgeeks.org/c/x-macros-in-c/
    // These containers preserve allocated memory upon resetting state of the run.
#define VECTORS_TABLE                      \
  X(Creature, creatures)                   \
  X(Landmine, landmines)                   \
  X(Garden, gardens)                       \
  X(PreSpawn, preSpawns)                   \
  X(Projectile, projectiles)               \
  X(MakeProjectileData, projectilesToMake) \
  X(int, projectilesToRemove)              \
  X(BodyShape, bodyShapes)                 \
  X(int, justDamagedCreatures)             \
  X(Number, numbers)                       \
  X(Pickupable, pickupables)               \
  X(Particle, particles)                   \
  X(TurretToSpawn, turretsToSpawn)         \
  X(GardenToSpawn, gardensToSpawn)         \
  X(RotatedRect, meleeWeaponColliderGizmos)

#define X(type_, name_) Vector<type_> name_ = {};
    VECTORS_TABLE;
#undef X
  } run;

  struct UIFlex {  ///
    bool active        = false;
    bool addedChildren = false;
    u16  childGap      = {};
    int  currentWidth  = 0;
    int  maxWidth      = {};

    PlaceholderGroup* pgroupsFirst        = nullptr;
    PlaceholderGroup* pgroupsLast         = nullptr;
    bool              pgroupsLastIsActive = false;
  } uiFlex;

  struct UI {            ///
    int newRunStep = 0;  // 0 - difficulty, 1 - build, 2 - starting weapon.

    Vector2                   notPickedUpCoinsLogicalPos = {};
    const Font*               overriddenFont             = {};
    Array<UIButtonState, 128> buttonStates               = {};
    int                       buttonStatesCount          = 0;

    FrameVisual newRunErrorLocked          = {};
    FrameVisual newRunSelectedDifficultyAt = {};
    FrameVisual newRunSelectedBuildAt      = {};

    FrameVisual errorGold        = {};
    FrameVisual shopErrorWeapons = {};
    FrameVisual shopErrorBuild   = {};

    Vector<JustUnlockedAchievement> justUnlockedAchievements = {};

    i16                              clayZIndex = 0;
    Array<Beautify, MAX_BEAUTIFIERS> clayBeautifiers{};
    int                              clayBeautifiersCount = 0;

    f32 touchControlMaxLogicalOffset = {};

    ControlsGroup* controlsGroupsFirst = {};
    ControlsGroup* controlsGroupsLast  = {};

    FrameVisual changedCoinsAt            = {};
    FrameVisual changedNotPickedUpCoinsAt = {};
  } ui;
} g = {};

int GetStatValue(StatType stat) {  ///
  if (g.run.state.screen == ScreenType_GAMEPLAY)
    return g.run.dynamicStats[stat];
  else
    return g.run.state.staticStats[stat];
}

lframe GetWaveDuration(int waveIndex) {  ///
  constexpr int durations_[]{20, 25, 30, 35, 40, 45, 50, 55, 60, 60,
                             60, 60, 60, 60, 60, 60, 60, 60, 60, 90};
  VIEW_FROM_ARRAY_DANGER(durations);

  ASSERT(waveIndex >= 0);
  ASSERT(waveIndex < durations.count);

  int seconds = durations[MAX(0, MIN(durations.count - 1, waveIndex))];

  ASSERT(gdebug.waveDurationMode >= 0);
  ASSERT(gdebug.waveDurationMode <= 3);
  if (gdebug.waveDurationMode == 1)
    seconds /= 3;
  else if (gdebug.waveDurationMode == 2)
    seconds = 2;
  if (gdebug.waveDurationMode == 3)
    seconds = 9999;

  return lframe::Unscaled(seconds * FIXED_FPS);
}

void ResetDynamicStats() {  ///
  FOR_RANGE (int, i, StatType_COUNT)
    g.run.dynamicStats[i] = g.run.state.staticStats[i];
}

struct MakePreSpawnData {  ///
  PreSpawnType type                    = {};
  CreatureType typeCreature            = {};
  Vector2      pos                     = {};
  int          damage                  = {};
  FBFlattened  damageScalings          = {};
  lframe       garden_spawnsAppleEvery = {};
};

void MakePreSpawn(MakePreSpawnData data) {  ///
  PreSpawn spawn{
    .type                    = data.type,
    .typeCreature            = data.typeCreature,
    .pos                     = data.pos,
    .damage                  = data.damage,
    .damageScalings          = data.damageScalings,
    .rotation                = GRAND.Angle() * ((GRAND.Rand() % 2) ? 1 : -1),
    .garden_spawnsAppleEvery = data.garden_spawnsAppleEvery,
  };
  spawn.createdAt.SetNow();
  *g.run.preSpawns.Add() = spawn;
}

int GetRandomTier() {  ///
  f32 v = GRAND.FRand();
  FOR_RANGE (int, tierIndex, TOTAL_TIERS) {
    if (v < g.run.randomTiers.cumulativeChances[tierIndex])
      return tierIndex;
  }
  INVALID_PATH;
  return 0;
}

ControlsGroup* _GetControlsGroup(ControlsGroupID id) {  ///
  auto group = g.ui.controlsGroupsLast;
  while (group) {
    if (group->id == id)
      return group;
    group = group->prev;
  }
  INVALID_PATH;
  return nullptr;
}

ControlsEntry* _GetPreferredControlsEntry(ControlsEntry* first, int preferredIndex) {  ///
  ASSERT(preferredIndex >= 0);
  while ((preferredIndex > 0) && first->next) {
    first = first->next;
    preferredIndex--;
  }
  return first;
}

ControlsDimension*
_GetPreferredControlsDimension(ControlsDimension* first, int preferredIndex) {  ///
  ASSERT(preferredIndex >= 0);
  while ((preferredIndex > 0) && first->next) {
    first = first->next;
    preferredIndex--;
  }
  return first;
}

ControlsGroupID MakeControlsGroup() {  ///
  static ControlsGroupID nextID = 1;

  auto p = ALLOCATE_FOR(&g.meta.transientDataArena, ControlsGroup);
  *p     = {
        .id   = nextID++,
        .prev = g.ui.controlsGroupsLast,
  };

  if (p->prev)
    p->prev->next = p;
  if (!g.ui.controlsGroupsFirst)
    g.ui.controlsGroupsFirst = p;
  g.ui.controlsGroupsLast = p;

  return p->id;
}

void ControlsGroupNewRow(ControlsGroupID groupID) {  ///
  auto group = _GetControlsGroup(groupID);
  if (!group || (group->last && !group->last->first))
    return;

  auto dim = ALLOCATE_FOR(&g.meta.transientDataArena, ControlsDimension);
  *dim     = {
        .prev = group->last,
  };

  if (dim->prev)
    dim->prev->next = dim;
  if (!group->first)
    group->first = dim;
  group->last = dim;
}

void ControlsGroupAdd(ControlsGroupID groupID, Clay_ElementId id) {  ///
  ASSERT(id.id);

  auto group = _GetControlsGroup(groupID);
  if (!group)
    return;

  auto dim = group->last;
  if (!dim) {
    ControlsGroupNewRow(groupID);
    dim = group->last;
  }

  auto elem = ALLOCATE_FOR(&g.meta.transientDataArena, ControlsEntry);
  *elem     = {
        .id   = id,
        .prev = dim->last,
  };

  if (elem->prev)
    elem->prev->next = elem;
  if (!dim->first)
    dim->first = elem;
  dim->last = elem;
}

void ControlsGroupConnect(
  ControlsGroupID from,
  Direction       dir,
  ControlsGroupID to,
  bool            bidirectional = true
) {  ///
  ASSERT(dir);

  auto g1 = _GetControlsGroup(from);
  auto g2 = _GetControlsGroup(to);
  if (!g1 || !g2)
    return;

  // Can't connect groups where at least one of them is empty.
  if (!g1->first || !g1->first->first || !g2->first || !g2->first->first)
    return;

  const int d = (int)dir - 1;

  if (!g1->connectionsPerDirection[d])
    g1->connectionsPerDirection[d] = to;

  if (bidirectional) {
    auto opposite = (d + 2) % 4;

    if (!g2->connectionsPerDirection[opposite])
      g2->connectionsPerDirection[opposite] = from;
  }
}

#define PLAYER_CREATURE (g.run.creatures[0])

void Save() {  ///
  g.meta.scheduledSave = true;
}

void AchievementStepUnlock(
  AchievementType                achievement,
  int                            stepIndex,
  const BFGame::AchievementStep* fb_step
) {  ///
  ASSERT(glib->achievements()->Get(achievement)->steps()->Get(stepIndex) == fb_step);

  if (!g.player.lockedSteps[fb_step->global_index()])
    return;

  g.player.lockedSteps[fb_step->global_index()]          = false;
  g.player.lockedBuilds[fb_step->unlocks_build_type()]   = {};
  g.player.lockedItems[fb_step->unlocks_item_type()]     = {};
  g.player.lockedWeapons[fb_step->unlocks_weapon_type()] = {};

  g.player.achievementStepsCompleted++;

  ASSERT(g.player.achievementStepsCompleted >= 0);
  ASSERT(g.player.achievementStepsCompleted <= TOTAL_ACHIEVEMENT_STEPS);
}

void OnAchievementValueChanged(AchievementType type, int oldValue, int newValue) {  ///
  auto fb = glib->achievements()->Get(type);

  auto fb_steps = fb->steps();
  if (!fb_steps)
    return;

  int stepIndex = -1;
  for (auto fb_step : *fb_steps) {
    stepIndex++;
    if ((!fb->negative_is_good() && (oldValue < fb_step->value())
         && (fb_step->value() <= newValue))
        || (fb->negative_is_good() && (oldValue > fb_step->value()) && (fb_step->value() >= newValue)))
    {
      AchievementStepUnlock(type, stepIndex, fb_step);
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
  const int oldValue = g.player.achievements[type].value;
  if (value <= oldValue)
    return;
  g.player.achievements[type].value = value;
  OnAchievementValueChanged(type, oldValue, value);
  Save();
}

void AchievementMin(AchievementType type, int value) {  ///
  const int oldValue = g.player.achievements[type].value;
  if (value >= oldValue)
    return;
  g.player.achievements[type].value = value;
  OnAchievementValueChanged(type, oldValue, value);
  Save();
}

void SetWaveWonTrue() {  ///
  g.run.state.waveWon = true;
  if (PLAYER_CREATURE.health == 1)
    AchievementMax(AchievementType_FINISH_A_WAVE_WITH_1_HP, 1);
  if ((g.player.runsWon == 0) && (g.run.state.waveIndex == 0))
    Metric("g_Run0_Wave1_Won");
}

void TriggerWaveCompleted(bool instant) {  ///
  ASSERT_FALSE(g.run.scheduledWaveCompleted.IsSet());

  if (!g.run.walkingTutorialCompletedAt.IsSet())
    g.run.walkingTutorialCompletedAt.SetNow();
  if (!g.run.waveStartedAt.IsSet())
    g.run.waveStartedAt.SetNow();

  g.run.scheduledWaveCompleted.SetNow();

  if (instant)
    g.run.scheduledWaveCompleted._value -= WAVE_COMPLETED_FRAMES.value;
  else
    PlaySound(Sound_GAME_BIG_TEXT_WHOOSH);
  ResetDynamicStats();

  for (auto& item : g.run.state.items)
    item.thisWaveAddedCount = 0;
}

void ChangeDynamicStatBy(StatType stat, int value) {  ///
  if (g.run.state.screen == ScreenType_GAMEPLAY) {
    LOGD(
      "ChangeDynamicStatBy %s by %d (from %d to %d)",
      glib->stats()->Get(stat)->type()->c_str(),
      value,
      g.run.dynamicStats[stat],
      g.run.dynamicStats[stat] + value
    );
    g.run.dynamicStats[stat] += value;
  }
}

void ChangeStaticAndDynamicStatBy(StatType stat, int value) {  ///
  g.run.state.staticStats[stat] += value;
  ChangeDynamicStatBy(stat, value);

  const auto fb_stat = glib->stats()->Get(stat);
  if (fb_stat->reach_this_or_more_stat_achievement_type()) {
    AchievementMax(
      (AchievementType)fb_stat->reach_this_or_more_stat_achievement_type(),
      g.run.state.staticStats[stat]
    );
  }

  if (fb_stat->reach_this_or_less_stat_achievement_type()) {
    AchievementMin(
      (AchievementType)fb_stat->reach_this_or_less_stat_achievement_type(),
      g.run.state.staticStats[stat]
    );
  }

  Save();
}

int GetAchievementsCompletedPercent() {  ///
  ASSERT(g.player.achievementStepsCompleted <= TOTAL_ACHIEVEMENT_STEPS);
  ASSERT(g.player.achievementStepsCompleted >= 0);
  ASSERT(TOTAL_ACHIEVEMENT_STEPS > 0);

  int percent = g.player.achievementStepsCompleted * 100 / TOTAL_ACHIEVEMENT_STEPS;
  if (g.player.achievementStepsCompleted < TOTAL_ACHIEVEMENT_STEPS)
    percent = MIN(99, percent);
  if (g.player.achievementStepsCompleted > 0)
    percent = MAX(1, percent);

  return MAX(0, MIN(100, percent));
}

// NOTE: Doesn't apply `StatType_DAMAGE`.
int ApplyDamageScalings(
  int         baseDamage,
  int         tierOffset,
  FBFlattened damageScalings,
  int         times
) {  ///
  for (auto scalingIndex : damageScalings.Iter()) {
    auto       fb_scaling = glib->damage_scalings()->Get(scalingIndex);
    const auto stat       = (StatType)fb_scaling->stat_type();
    ASSERT(stat);
    auto statValue = GetStatValue(stat);
    auto percent   = fb_scaling->percents_per_tier()->Get(tierOffset) * times;
    baseDamage += Round((f32)statValue * (f32)percent / 100.0f);
  }
  return MAX(1, baseDamage);
}

int ApplyPlayerStatDamageMultiplier(int damage) {  ///
  f32 v = 1 + (f32)GetStatValue(StatType_DAMAGE) / 100.0f;
  return MAX(1, Round((f32)damage * v));
}

void IterateOverWeaponEffects(
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

void IterateOverItemEffects(
  Weapon*             w,
  int                 wi,
  EffectConditionType condition,
  ItemType            type,
  int                 count,
  int                 thisWaveAddedCount,
  /* void (Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int
     thisWaveAddedCount) */
  auto innerLambda
) {  ///
  const auto fb         = glib->items()->Get(type);
  const auto fb_effects = fb->effects();
  if (fb_effects) {
    for (const auto fb_effect : *fb_effects) {
      if (fb_effect->effectcondition_type() == condition)
        innerLambda(w, wi, fb_effect, 0, count, thisWaveAddedCount);
    }
  }
}

void IterateOverEffects(
  EffectConditionType condition,
  int                 weaponIndexOrMinus1,
  /* void (Weapon* w, int wi, auto fb_effect, int tierOffset, int times,
     int thisWaveAddedCount) */
  auto innerLambda
) {  ///
#if BF_ENABLE_ASSERTS
  auto fb_conditions = glib->effect_conditions();
  auto fb            = fb_conditions->Get(condition);
  ASSERT(fb->restrict() != 1);
#endif
  Weapon* w = nullptr;
  if (weaponIndexOrMinus1 >= 0) {
    const auto& weapon = g.run.state.weapons[weaponIndexOrMinus1];
    ASSERT(weapon.type);
    if (weapon.type)
      w = g.run.state.weapons.base + weaponIndexOrMinus1;
  }

  int wi = -1;
  for (auto& weapon : g.run.state.weapons) {
    wi++;

    if (!weapon.type)
      continue;

    auto fb         = glib->weapons()->Get(weapon.type);
    auto fb_effects = fb->effects();
    if (!fb_effects)
      continue;

    for (const auto fb_effect : *fb_effects) {
      if (fb_effect->effectcondition_type() != condition)
        continue;

      bool call = true;
      if (fb_effect->only_this_weapon())
        call = (weaponIndexOrMinus1 == wi);
      if (fb_effect->only_other_weapons())
        call = (weaponIndexOrMinus1 >= 0) && (weaponIndexOrMinus1 != wi);

      if (call) {
        innerLambda(
          w, weaponIndexOrMinus1, fb_effect, weapon.tier - fb->min_tier_index(), 1, 0
        );
      }
    }
  }

  // Iterating over items.
  for (auto& item : g.run.state.items) {
    IterateOverItemEffects(
      w,
      weaponIndexOrMinus1,
      condition,
      item.type,
      item.count,
      item.thisWaveAddedCount,
      innerLambda
    );
  }

  // Iterating over difficulty effects.
  {
    const auto fb         = glib->difficulties()->Get(g.player.difficulty);
    const auto fb_effects = fb->effects();
    if (fb_effects) {
      for (const auto fb_effect : *fb_effects) {
        if (fb_effect->effectcondition_type() == condition)
          innerLambda(w, weaponIndexOrMinus1, fb_effect, 0, 1, 0);
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
          innerLambda(w, weaponIndexOrMinus1, fb_effect, 0, 1, 0);
      }
    }
  }
}

int CalculateWeaponDamage(
  int        weaponIndexOrMinus1,
  WeaponType type,
  int        tier,
  bool       affectedByGame
) {  ///
  ASSERT(tier < 4);
  const auto fb = glib->weapons()->Get(type);
  ASSERT(tier >= fb->min_tier_index());

  int tierOffset = tier - fb->min_tier_index();

  int damage = fb->base_damage()->Get(tierOffset);
  damage     = ApplyDamageScalings(damage, tierOffset, fb->damage_scalings(), 1);
  damage     = ApplyPlayerStatDamageMultiplier(damage);
  LAMBDA (void, applyEffectToDamage, (auto fb_effect, int tierOffset2, int times)) {
    if (times < 0)
      return;

    auto value = fb_effect->value();
    if (value)
      damage += value->Get(tierOffset2) * times;

    auto value_multiplier = fb_effect->value_multiplier();
    if (value_multiplier)
      damage = Round((f32)damage * value_multiplier->Get(tierOffset2) * (f32)times);
  };

  IterateOverEffects(
    EffectConditionType_MORE_OF_THE_SAME_WEAPON_MORE__PROPERTY,
    weaponIndexOrMinus1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      if (fb_effect->weaponproperty_type() != WeaponPropertyType_DAMAGE)
        return;

      int sameWeapons = 0;
      int i           = -1;
      for (const auto& weapon : g.run.state.weapons) {
        i++;
        if ((weaponIndexOrMinus1 != i) && (weapon.type == type))
          sameWeapons++;
      }

      applyEffectToDamage(fb_effect, tierOffset, sameWeapons * times);
    }
  );

  IterateOverEffects(
    EffectConditionType_MORE_EMPTY_WEAPON_SLOTS_MORE__PROPERTY,
    weaponIndexOrMinus1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      if (fb_effect->weaponproperty_type() != WeaponPropertyType_DAMAGE)
        return;

      int emptySlots = 0;
      for (const auto& weapon : g.run.state.weapons) {
        if (!weapon.type)
          emptySlots++;
      }

      applyEffectToDamage(fb_effect, tierOffset, emptySlots * times);
    }
  );

  if ((g.run.state.screen == ScreenType_GAMEPLAY) && affectedByGame) {
    IterateOverEffects(
      EffectConditionType_PROPERTY__WHEN_IDLE,
      weaponIndexOrMinus1,
      [&](
        Weapon* w,
        int     wi,
        auto    fb_effect,
        int     tierOffset,
        int     times,
        int     thisWaveAddedCount
      ) BF_FORCE_INLINE_LAMBDA {
        if (fb_effect->weaponproperty_type() != WeaponPropertyType_DAMAGE)
          return;
        if (g.run.playerContinuousIdleFrames >= IDLE_OR_WALKING_BONUS_FRAMES)
          applyEffectToDamage(fb_effect, tierOffset, times);
      }
    );

    IterateOverEffects(
      EffectConditionType_PROPERTY__WHEN_WALKING,
      weaponIndexOrMinus1,
      [&](
        Weapon* w,
        int     wi,
        auto    fb_effect,
        int     tierOffset,
        int     times,
        int     thisWaveAddedCount
      ) BF_FORCE_INLINE_LAMBDA {
        if (fb_effect->weaponproperty_type() != WeaponPropertyType_DAMAGE)
          return;
        if (g.run.playerContinuousWalkingFrames >= IDLE_OR_WALKING_BONUS_FRAMES)
          applyEffectToDamage(fb_effect, tierOffset, times);
      }
    );
  }

  return MAX(1, damage);
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

#define PLAYER_COINS (g.run.state.staticStats[StatType_COINS])

void SanitizeCoins() {  ///
  if (PLAYER_COINS < 0)
    PLAYER_COINS = int_max;
}

f32 GetStatModificationScale(StatType stat) {  ///
  f32 scaleStat = 1;
  IterateOverEffects(
    EffectConditionType_STAT_2__MODS_CHANGED_BY__X__PERCENT,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      if (fb_effect->stat_type_2() == stat)
        scaleStat += (f32)EFFECT_X_INT * (f32)times / 100.0f;
    }
  );
  return scaleStat;
}

void ApplyStatEffect(const BFGame::Effect* fb_effect, int tierOffset, int times) {  ///
  ASSERT(times != 0);

  const auto statType = (StatType)fb_effect->stat_type();
  ASSERT(statType);
  if (!statType)
    return;

  int changeBy = 0;

  auto value = fb_effect->value();
  if (value)
    changeBy = value->Get(tierOffset) * times;

  int statValue = g.run.state.staticStats[statType];

  auto value_multiplier = fb_effect->value_multiplier();
  if (value_multiplier) {
    const f32 vm = value_multiplier->Get(tierOffset);
    if (!FloatEquals(vm, 1)) {
      f32 multiplier = (1 + (vm - 1) * fabs((f32)times));
      if (times < 0)
        multiplier = 1 / multiplier;

      int newStatValue{};

      const f32 newVal = (f32)statValue * multiplier;
      if (newVal > (f32)int_max)
        newStatValue = int_max;
      else if (newVal < (f32)int_min)
        newStatValue = int_min;
      else
        newStatValue = Round(newVal);

      changeBy += newStatValue - statValue;
    }
  }

  if (fb_effect->modification_scaleable()) {
    const auto scale = GetStatModificationScale((StatType)fb_effect->stat_type());
    if (scale != 1)
      changeBy = Round((f32)changeBy * scale);
  }

  if (changeBy)
    ChangeStaticAndDynamicStatBy(statType, changeBy);

  if (statType == StatType_COINS)
    SanitizeCoins();
}

f32 GetLuckFactor() {  ///
  return MAX(0, 1.0f + (f32)GetStatValue(StatType_LUCK) / 100.0f);
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

void UpdateHPPercentThresholdEffects(int prevHealth) {  ///
  IterateOverEffects(
    EffectConditionType_STAT__WHEN_BELOW__X__PERCENT_HP,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      const auto threshold
        = Round((f32)(PLAYER_CREATURE.maxHealth * EFFECT_X_INT) / 100.0f);

      int scale = 0;
      if ((prevHealth >= threshold) && (PLAYER_CREATURE.health < threshold))
        scale = 1;
      else if ((prevHealth < threshold) && (PLAYER_CREATURE.health >= threshold))
        scale = -1;

      const auto stat = (StatType)fb_effect->stat_type();
      ChangeDynamicStatBy(
        stat,
        GetStatModificationScale(stat) * scale * times
          * fb_effect->value()->Get(tierOffset)
      );
    }
  );
}

void OnWaveStarted() {  ///
  ResetDynamicStats();

  g.meta.stickControl = {};

  g.run.state.upgrades.rerolls = {};
  g.run.state.previousCoins    = g.run.state.staticStats[StatType_COINS];

  g.run.playerWalkedMetersCumulativeDelta = 0;
  g.run.playerWalkedMeters                = 0;
  g.run.playerContinuousIdleFrames        = 0;
  g.run.playerContinuousWalkingFrames     = 0;

  g.run.chestsDroppedThisWave = 0;

  g.run.turretsToSpawn.Reset();
  IterateOverEffects(
    EffectConditionType_TURRET_DEALING__X__DAMAGE,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      FOR_RANGE (int, i, times) {
        *g.run.turretsToSpawn.Add() = {
          .baseDamage     = EFFECT_X_INT,
          .damageScalings = fb_effect->damage_scalings(),
        };
      }
    }
  );
  GRAND.Shuffle(g.run.turretsToSpawn.base, g.run.turretsToSpawn.count);

  g.run.gardensToSpawn.Reset();
  IterateOverEffects(
    EffectConditionType_SPAWNS_GARDEN_THAT_SPAWNS_APPLE_EVERY__X__SECONDS,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      FOR_RANGE (int, i, times) {
        *g.run.gardensToSpawn.Add() = {
          .spawnsAppleEvery = lframe::FromSeconds(EFFECT_X_FLOAT),
        };
      }
    }
  );
  GRAND.Shuffle(g.run.gardensToSpawn.base, g.run.gardensToSpawn.count);

  g.run.waveStartedAt = {};
  if (g.run.walkingTutorialCompletedAt.IsSet())
    g.run.waveStartedAt.SetNow();

  g.run.bossCreatureID = 0;

  RecalculateThisWaveMobs();

  for (auto& weapon : g.run.state.weapons) {
    weapon.thisWaveDamage   = 0;
    weapon.thisWaveUseCount = 0;
  }

  g.run.randomTiers = GetRandomCumulativeChances(
    g.run.state.waveIndex, g.run.state.staticStats[StatType_LUCK]
  );

  g.run.turretsOnTheMapCount = 0;

  g.run.cantHeal = false;
  IterateOverEffects(
    EffectConditionType_CANT_HEAL,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA { g.run.cantHeal = true; }
  );

  IterateOverEffects(
    EffectConditionType_STAT__DURING_NEXT_WAVE,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      const auto stat = (StatType)fb_effect->stat_type();

      if (thisWaveAddedCount) {
        ChangeDynamicStatBy(
          stat,
          Round(
            GetStatModificationScale(stat) * (f32)fb_effect->value()->Get(tierOffset)
            * (f32)thisWaveAddedCount
          )
        );
      }
    }
  );

  if (PLAYER_CREATURE.health != PLAYER_CREATURE.maxHealth)
    UpdateHPPercentThresholdEffects(PLAYER_CREATURE.maxHealth);

  // Applying effects: START_OF_THE_WAVE_GET__STAT.
  IterateOverEffects(
    EffectConditionType_START_OF_THE_WAVE_GET__STAT,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA { ApplyStatEffect(fb_effect, tierOffset, times); }
  );
}

void SwitchScreen(ScreenType screen) {  ///
  g.run.state.screen          = screen;
  g.run.shownScreenAt[screen] = {};
  g.run.shownScreenAt[screen].SetNow();
}

void GameLoad(const BFSave::Save* save) {  ///
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

  g.player.runsWon        = save->runs_won();
  g.player.runsLost       = save->runs_lost();
  g.player.difficulty     = (DifficultyType)save->difficulty();
  g.player.build          = (BuildType)save->build();
  g.player.weapon         = (WeaponType)save->weapon();
  g.player.biome          = (BiomeType)save->biome();
  PLAYER_CREATURE.health  = save->health();
  s.won                   = save->won();
  s.screen                = (ScreenType)save->screen();
  s.waveIndex             = save->wave_index();
  s.waveWon               = save->wave_won();
  s.levelOnStartOfTheWave = save->level_on_start_of_the_wave();
  s.xp                    = save->xp();
  s.xpOnStartOfTheWave    = s.xp;
  s.playerKilledEnemies   = save->player_killed_enemies();
  s.notPickedUpCoins      = save->not_picked_up_coins();
  s.chests                = save->chests();
  s.toSpawn               = save->to_spawn();

  // Loading saved weapons.
  int weaponIndex = 0;
  for (auto x : *save->weapons()) {
    s.weapons[weaponIndex++] = {
      .type                  = (WeaponType)x->type(),
      .tier                  = x->tier(),
      .thisWaveKilledEnemies = x->killed_enemies(),
    };
  }

  // Loading saved items.
  auto fb_items = save->items();
  if (fb_items) {
    for (auto x : *fb_items) {
      *s.items.Add() = {
        .type               = (ItemType)x->type(),
        .count              = x->count(),
        .thisWaveAddedCount = x->this_wave_added_count(),
      };
    }

    // Removing player-equipped items from pools.
    for (const auto& x : s.items) {
      const auto fb   = glib->items()->Get(x.type);
      auto&      pool = g.run.itemPools[fb->tier()];

      int otherItemIndex = -1;
      for (auto& otherItem : pool) {
        otherItemIndex++;

        if (otherItem.type == x.type) {
          otherItem.count -= x.count;

          if (otherItem.count <= 0) {
            pool.UnstableRemoveAt(otherItemIndex);
            break;
          }
        }
      }
    }
  }

  // Resetting stats to default values.
  {
    auto fb_stats = glib->stats();
    int  i        = 0;
    for (auto& x : s.staticStats)
      x = fb_stats->Get(i++)->player_value();
  }

  // Loading values of previosly saved stats.
  int  statIndex = 0;
  auto fb_stats  = save->stats();
  if (fb_stats) {
    for (auto x : *fb_stats)
      s.staticStats[statIndex++] = x;
  }

  ResetDynamicStats();

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

  if (s.screen == ScreenType_GAMEPLAY)
    OnWaveStarted();
  else if (s.screen == ScreenType_WAVE_END_ANIMATION)
    TriggerWaveCompleted(true);
  else if (s.screen == ScreenType_PICKED_UP_ITEM)
    g.run.scheduledPickedUpItems = true;
  else if (s.screen == ScreenType_UPGRADES)
    g.run.scheduledUpgrades = true;
  else if (s.screen == ScreenType_SHOP)
    g.run.scheduledShop = true;
  else if (s.screen == ScreenType_END)
    g.run.scheduledEnd = true;

  g.player.lockedBuilds  = {};
  g.player.lockedWeapons = {};
  g.player.lockedItems   = {};

  g.player.volumeMusic = MAX(0, MIN(3, save->volume_music()));
  g.player.volumeSFX   = MAX(0, MIN(3, save->volume_sfx()));

  SwitchScreen(g.run.state.screen);
}

flatbuffers::FlatBufferBuilder GameDumpStateForSaving() {  ///
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

    fb_save.runs_won  = g.player.runsWon;
    fb_save.runs_lost = g.player.runsLost;

    fb_save.difficulty = (int)g.player.difficulty;
    fb_save.build      = (int)g.player.build;
    fb_save.weapon     = (int)g.player.weapon;
    fb_save.biome      = (int)g.player.biome;

    fb_save.random_state               = ge.meta.logicRand._state;
    fb_save.health                     = PLAYER_CREATURE.health;
    fb_save.won                        = s.won;
    fb_save.screen                     = s.screen;
    fb_save.wave_index                 = s.waveIndex;
    fb_save.wave_won                   = s.waveWon;
    fb_save.level_on_start_of_the_wave = s.levelOnStartOfTheWave;
    fb_save.xp                         = s.xp;
    fb_save.player_killed_enemies      = s.playerKilledEnemies;
    fb_save.not_picked_up_coins        = s.notPickedUpCoins;
    fb_save.chests                     = s.chests;
    fb_save.to_spawn                   = s.toSpawn;

    for (const auto& weapon : g.run.state.weapons) {
      fb_save.weapons.push_back(std::make_unique<BFSave::WeaponT>(BFSave::WeaponT{
        .type           = weapon.type,
        .tier           = weapon.tier,
        .killed_enemies = weapon.thisWaveKilledEnemies,
      }));
    }

    for (const auto& item : g.run.state.items) {
      fb_save.items.push_back(std::make_unique<BFSave::ItemT>(BFSave::ItemT{
        .type                  = item.type,
        .count                 = item.count,
        .this_wave_added_count = item.thisWaveAddedCount,
      }));
    }

    for (const auto& x : g.run.state.staticStats)
      fb_save.stats.push_back(x);

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
      fb_save.stats[StatType_COINS] = s.previousCoins;

      fb_save.level  = fb_save.level_on_start_of_the_wave;
      fb_save.xp     = s.xpOnStartOfTheWave;
      fb_save.chests = 0;
    }

    fb_save.volume_music = g.player.volumeMusic;
    fb_save.volume_sfx   = g.player.volumeSFX;
  }

  flatbuffers::FlatBufferBuilder fbb{};
  auto                           packed = BFSave::Save::Pack(fbb, &fb_save);
  fbb.Finish(packed);
  return fbb;
}

struct MakeLandmineData {  ///
  Vector2     pos            = {};
  int         damage         = {};
  FBFlattened damageScalings = {};
};

void MakeLandmine(MakeLandmineData data) {  ///
  ASSERT(data.damageScalings.start);
  ASSERT(data.damageScalings.start < data.damageScalings.end);
  Landmine v{
    .pos            = data.pos,
    .damage         = data.damage,
    .damageScalings = data.damageScalings,
  };
  v.spawnedAt.SetNow();
  *g.run.landmines.Add() = v;
}

struct MakeGardenData {  ///
  Vector2 pos              = {};
  lframe  spawnsAppleEvery = {};
};

void MakeGarden(MakeGardenData data) {  ///
  ASSERT(data.spawnsAppleEvery.value > 0);
  Garden v{
    .pos              = data.pos,
    .spawnsAppleEvery = data.spawnsAppleEvery,
  };
  v.spawnedAt.SetNow();
  *g.run.gardens.Add() = v;
}

void ApplyAilment(ApplyAilmentData data) {  ///
  ASSERT(data.creature);

  if (data.creature->ailments.count >= data.creature->ailments.maxCount)
    return;

  Ailment a{.c = data.c};
  a.startedAt.SetNow();
  *data.creature->ailments.Add() = a;
}

void ChangeCoins(int amount) {  ///
  if (amount) {
    g.ui.changedCoinsAt = {};
    g.ui.changedCoinsAt.SetNow();
  }

  if (amount > 0)
    AchievementAdd(AchievementType_COINS, amount);

  PLAYER_COINS += amount;
  if (PLAYER_COINS < 0) {
    if (amount >= 0)
      PLAYER_COINS = int_max;
    else
      PLAYER_COINS = MAX(0, PLAYER_COINS);
  }
  g.run.dynamicStats[StatType_COINS] = PLAYER_COINS;

  AchievementMax(AchievementType_HOLD_N_COINS, PLAYER_COINS);

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
  auto pp = g.uiFlex.pgroupsFirst;
  while (pp) {
    if (!strcmp(pp->placeholder, placeholder))
      return true;
    pp = pp->next;
  }
  return false;
}

void _AddPlaceholder(Placeholder p) {  ///
  ASSERT(g.uiFlex.pgroupsLastIsActive);

  auto pp = ALLOCATE_FOR(&g.meta.trashArena, Placeholder);
  *pp     = p;

  auto& group = *g.uiFlex.pgroupsLast;
  if (!group.first)
    group.first = pp;
  if (group.last)
    group.last->next = pp;
  group.last = pp;
}

// `placeholder` must be statically allocated or live in trashArena.
void PlaceholdGroupBegin(const char* placeholder) {  ///
  ASSERT_FALSE(IsAlreadyPlaceholded(placeholder));
  ASSERT_FALSE(g.uiFlex.pgroupsLastIsActive);
  g.uiFlex.pgroupsLastIsActive = true;

  ASSERT((bool)g.uiFlex.pgroupsFirst == (bool)g.uiFlex.pgroupsLast);

  auto pp = ALLOCATE_FOR(&g.meta.trashArena, PlaceholderGroup);
  *pp     = {.placeholder = placeholder};

  if (!g.uiFlex.pgroupsFirst)
    g.uiFlex.pgroupsFirst = pp;
  if (g.uiFlex.pgroupsLast)
    g.uiFlex.pgroupsLast->next = pp;
  g.uiFlex.pgroupsLast = pp;
}

void PlaceholdGroupEnd() {  ///
  ASSERT(g.uiFlex.pgroupsFirst);
  ASSERT(g.uiFlex.pgroupsLast);
  ASSERT(g.uiFlex.pgroupsLastIsActive);
  g.uiFlex.pgroupsLastIsActive = false;
}

// `value` must be statically allocated or live in trashArena.
void PlaceholdString(const char* value, Color color = palTextGreen) {  ///
  ASSERT(g.uiFlex.pgroupsLastIsActive);
  _AddPlaceholder({
    .type = PlaceholderType_STRING,
    ._u{.string{.value = value, .color = color}},
  });
}

void PlaceholdFormattedString(const char* value, Color color = palTextGreen) {  ///
  PlaceholdString(PushTextToArena(&g.meta.trashArena, value), color);
}

// `placeholder` and `value` must be statically allocated or live in trashArena.
void PlaceholdString(
  const char* placeholder,
  const char* value,
  Color       color = palTextGreen
) {  ///
  PlaceholdGroupBegin(placeholder);
  PlaceholdString(value, color);
  PlaceholdGroupEnd();
}

void PlaceholdBrokenLocale(int locale, Color color = palTextWhite) {  ///
  ASSERT(g.uiFlex.pgroupsLastIsActive);
  _AddPlaceholder({
    .type = PlaceholderType_BROKEN_LOCALE,
    ._u{.brokenLocale{
      .value = locale,
      .color = color,
    }},
  });
}

// `placeholder` must be statically allocated or live in trashArena.
void PlaceholdBrokenLocale(
  const char* placeholder,
  int         locale,
  Color       color = palTextWhite
) {  ///
  PlaceholdGroupBegin(placeholder);
  PlaceholdBrokenLocale(locale, color);
  PlaceholdGroupEnd();
}

void PlaceholdImage(int texID) {  ///
  ASSERT(g.uiFlex.pgroupsLastIsActive);
  _AddPlaceholder({
    .type = PlaceholderType_IMAGE,
    ._u{.image{.texID = texID}},
  });
}

// `placeholder` must be statically allocated or live in trashArena.
void PlaceholdImage(const char* placeholder, int texID) {  ///
  PlaceholdGroupBegin(placeholder);
  PlaceholdImage(texID);
  PlaceholdGroupEnd();
}

void MakeNumber(MakeNumberData data) {  ///
  if (g.run.numbers.count >= MAX_NUMBERS_COUNT)
    return;
  ASSERT(data.type);
  Number number{
    .type  = data.type,
    .value = data.value,
    .pos   = data.pos,
  };
  number.createdAt.SetNow();
  *g.run.numbers.Add() = number;
}

int GetNextLevelXp(int currentLevel) {  ///
  currentLevel = MAX(1, currentLevel);
  int value    = SQR(currentLevel + 3);
  f32 scale
    = 1 + g.run.state.staticStats[StatType_XP_PERCENT_REQUIRED_TO_LEVEL_UP] / 100.0f;
  value = Round((f32)value * scale);
  return MAX(1, value);
}

void AddXP(f32 xp) {  ///
  if (xp > 0)
    xp *= (f32)(MAX(1, g.run.dynamicStats[StatType_XP_GAIN] + 100)) / 100.0f;

  g.run.state.xp += xp;
  g.run.state.xp = MAX(0, g.run.state.xp);

  // Handling level up.

  auto nextLevelXp = GetNextLevelXp(g.run.state.staticStats[StatType_LEVEL]);

  int addedLevels = 0;

  while (g.run.state.xp >= nextLevelXp) {
    addedLevels++;
    g.run.state.xp -= nextLevelXp;
    g.run.state.staticStats[StatType_LEVEL]++;
    nextLevelXp = GetNextLevelXp(g.run.state.staticStats[StatType_LEVEL]);

    MakeNumber({.type = NumberType_LEVEL_UP, .pos = PLAYER_CREATURE.pos});
  }

  if (addedLevels > 0) {
    if (!gdebug.hideUIForVideo)
      PlaySound(Sound_GAME_LEVEL_UP);

    IterateOverEffects(
      EffectConditionType_STAT__UPON_LEVEL_UP,
      -1,
      [&](
        Weapon* w,
        int     wi,
        auto    fb_effect,
        int     tierOffset,
        int     times,
        int     thisWaveAddedCount
      ) BF_FORCE_INLINE_LAMBDA {
        ApplyStatEffect(fb_effect, tierOffset, times * addedLevels);
      }
    );
  }

  Save();
}

AilmentCommonData GetAilmentFromWeapon(int weaponIndex) {  ///
  auto& w = g.run.state.weapons[weaponIndex];
  ASSERT(w.type);

  auto fb = glib->weapons()->Get(w.type);
  if (!fb->burning_damage())
    return {};

  const int tierOffset = w.tier - fb->min_tier_index();
  const int damage     = fb->burning_damage()->Get(tierOffset);
  if (damage <= 0)
    return {};

  int times = 0;
  if (fb->burning_times())
    times = fb->burning_times()->Get(tierOffset);
  if (times <= 0)
    return {};

  AilmentCommonData result{
    .type            = AilmentType_BURN,
    .weaponIndex     = weaponIndex,
    .initialTimes    = times,
    .remainingTimes  = times,
    .remainingSpread = MAX(0, g.run.dynamicStats[StatType_BURNING_SPREAD]),
    .damage          = damage,
    .damageScalings{fb->burning_damage_scalings()},
  };
  return result;
}

struct TryApplyDamageData {  ///
  int          creatureIndex                      = {};
  int          damage                             = {};
  int          burningDamage                      = {};
  int          burningTimes                       = {};
  Vector2      directionOrZero                    = {0, 0};
  f32          knockbackMeters                    = 0;
  CreatureType damagerCreatureType                = CreatureType_INVALID;
  f32          critDamageMultiplier               = 1;
  int          weaponCritChance                   = 0;
  int          indexOfWeaponThatDidDamageOrMinus1 = -1;
  bool         weaponCanApplyAilment              = true;
  bool         weaponSound                        = false;
  bool         playerSound                        = true;
  bool*        outWasCrit                         = nullptr;
  bool*        outJustKilled                      = nullptr;
};
bool TryApplyDamage(TryApplyDamageData data);

void HealPlayer(int amount = 1) {  ///
  ASSERT(amount >= 0);
  if (amount <= 0)
    return;
  if (g.run.state.screen != ScreenType_GAMEPLAY)
    return;
  if (g.run.cantHeal)
    return;

  if (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth) {
    auto prevHealth = PLAYER_CREATURE.health;

    PLAYER_CREATURE.health
      = MoveTowardsI(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth, amount);

    g.run.runHealed += PLAYER_CREATURE.health - prevHealth;
    AchievementMax(AchievementType_HEAL_X_DURING_A_RUN, g.run.runHealed);

    UpdateHPPercentThresholdEffects(prevHealth);
  }

  IterateOverEffects(
    EffectConditionType_X__CHANCE_TO_DEAL__Y__DAMAGE_UPON_HEALING,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      const f32 chance = EFFECT_X_INT * (f32)times;
      if (GRAND.FRand() >= chance)
        return;

      FOR_RANGE (int, i, 12) {
        const int   creatureIndex = GRAND.Rand() % g.run.creatures.count;
        const auto& creature      = g.run.creatures[creatureIndex];

        if (creature.health <= 0)
          continue;
        if (creature.diedAt.IsSet())
          continue;

        auto fb = glib->creatures()->Get(creature.type);
        if (fb->hostility_type() == HostilityType_FRIENDLY)
          continue;

        f32 critDamageMultiplier = 1.5f;
        if (w) {
          critDamageMultiplier
            = glib->weapons()->Get(w->type)->crit_damage_multiplier()->Get(tierOffset);
        }

        int damage = EFFECT_Y_INT * times;
        damage     = ApplyDamageScalings(damage, 0, fb_effect->damage_scalings(), times);
        damage     = ApplyPlayerStatDamageMultiplier(damage);

        TryApplyDamage({
          .creatureIndex                      = creatureIndex,
          .damage                             = damage,
          .damagerCreatureType                = CreatureType_PLAYER,
          .critDamageMultiplier               = critDamageMultiplier,
          .indexOfWeaponThatDidDamageOrMinus1 = wi,
        });
        break;
      }
    }
  );

  Save();
}

f32 GetLifestealChance(
  WeaponType typeOrInvalid,
  int        tier,
  bool       affectedByGame = true
) {  ///
  auto fb = glib->weapons()->Get(typeOrInvalid);

  f32 lifesteal = 0;
  if (affectedByGame)
    lifesteal += (f32)GetStatValue(StatType_LIFE_STEAL) / 100.0f;

  if (typeOrInvalid) {
    auto fb_percents = fb->life_steal_percents();
    if (fb_percents)
      lifesteal += fb_percents->Get(tier - fb->min_tier_index()) / 100.0f;
  }

  return lifesteal;
}

void MakeParticles(MakeParticlesData data) {  ///
  ASSERT(data.type);

  ASSERT(data.velocityAnglePlusMinus >= 0);
  ASSERT(data.velocityAnglePlusMinus <= PI32);

  ASSERT(data.count >= 0);

  auto fb = glib->particles()->Get(data.type);

  auto color = ColorFromRGBA(fb->color());
  if (data.color != WHITE)
    color = ColorTint(color, data.color);

  const int maxCount = MAX(0, MAX_PARTICLES_COUNT - g.run.particles.count);
  data.count         = MIN(data.count, maxCount);

  if (data.count > 0)
    g.run.particles.Reserve(g.run.particles.count + data.count);

  FOR_RANGE (int, particleIndex, data.count) {
    const auto variation = (u16)(GRAND.Rand() % fb->variations()->size());

    const f32 vel   = data.velocity + data.velocityPlusMinus * GRAND.FRand11();
    const f32 angle = data.velocityAngle + data.velocityAnglePlusMinus * GRAND.FRand11();

    const f32 initialOffset = data.initialOffset
                              + Lerp(
                                -data.initialOffsetPlusMinus,
                                data.initialOffsetPlusMinus,
                                data.initialOffsetEasing(GRAND.FRand())
                              );

    f32 rotation = (fb->disable_rotation() ? 0 : GRAND.Angle());

    f32 rotationSpeed = data.rotationSpeedPlusMinus * GRAND.FRand11();

    const f32 scale = data.scale + data.scalePlusMinus * GRAND.FRand11();

    const f32 durationSeconds
      = fb->duration_seconds() + fb->duration_plus_minus() * GRAND.FRand11();

    ASSERT(durationSeconds > 0);
    if (durationSeconds <= 0)
      continue;

    Particle p{
      .type          = data.type,
      .variation     = variation,
      .pos           = data.pos + Vector2Rotate({initialOffset, 0}, angle),
      .velocity      = Vector2Rotate({vel, 0}, angle),
      .rotation      = rotation,
      .rotationSpeed = rotationSpeed,
      .scale         = scale,
      .color         = color,
      .duration      = lframe::FromSeconds(durationSeconds),
    };
    p.createdAt.SetNow();
    *g.run.particles.Add() = p;
  }
}

void MakePickupable(MakePickupableData data);

bool CanSpawnMoreCreatures() {  ///
  if (!g.run.waveStartedAt.IsSet())
    return false;

  const auto framesUntilTheEndOfTheWave
    = GetWaveDuration(g.run.state.waveIndex) - g.run.waveStartedAt.Elapsed();
  return (framesUntilTheEndOfTheWave > DONT_SPAWN_RIGHT_BEFORE_WAVE_ENDS + SPAWN_FRAMES);
}

bool TryApplyDamage(TryApplyDamageData data) {  ///
  if (data.outWasCrit)
    *data.outWasCrit = false;
  if (data.outJustKilled)
    *data.outJustKilled = false;

  ASSERT(data.creatureIndex >= 0);
  data.damage = MAX(1, data.damage);
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

  const auto fb         = glib->creatures()->Get(creature.type);
  const auto fb_damager = glib->creatures()->Get(data.damagerCreatureType);

  creature.aggroed = fb->can_aggro();

  if (data.damage == int_max) {
    creature.killedBecauseOfTheEndOfTheWave = true;
    creature.health                         = 0;
    if (data.outJustKilled)
      *data.outJustKilled = true;
  }
  else if (!creature.killedBecauseOfTheEndOfTheWave) {
    bool isCrit = false;

    if (data.creatureIndex) {
      f32 critChance
        = (f32)(g.run.dynamicStats[StatType_CRIT_CHANCE] + data.weaponCritChance)
          / 100.0f;

      if (data.indexOfWeaponThatDidDamageOrMinus1 >= 0) {
        IterateOverEffects(
          EffectConditionType_EVERY__X__USE_CHANGES_CRIT_CHANCE_BY__Y,
          data.indexOfWeaponThatDidDamageOrMinus1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA {
            if ((w->thisWaveUseCount % EFFECT_X_INT) == 0)
              critChance += (f32)(EFFECT_Y_INT * times) / 100.0f;
          }
        );
      }

      if (fb->is_boss()) {
        data.damage += Round(
          (f32)data.damage * (f32)g.run.dynamicStats[StatType_DAMAGE_AGAINST_BOSSES]
          / 100.0f
        );
      }

      IterateOverEffects(
        EffectConditionType_DEAL__X__PERCENT_OF_ENEMY_CURRENT_HP_BONUS_DAMAGE__Y__FOR_BOSSES,
        data.indexOfWeaponThatDidDamageOrMinus1,
        [&](
          Weapon* w,
          int     wi,
          auto    fb_effect,
          int     tierOffset,
          int     times,
          int     thisWaveAddedCount
        ) BF_FORCE_INLINE_LAMBDA {
          int percent = EFFECT_X_INT * times;
          if (fb->is_boss())
            percent = EFFECT_Y_INT * times;
          data.damage += Round(creature.health * (f32)percent / 100.0f);
        }
      );

      bool isBurning = false;
      for (auto& ailment : creature.ailments) {
        if (ailment.c.type == AilmentType_BURN) {
          isBurning = true;
          break;
        }
      }

      if (isBurning) {
        IterateOverEffects(
          EffectConditionType_HITTING_BURNING_ENEMIES_CHANGES_CRIT_CHANCE_BY__X,
          data.indexOfWeaponThatDidDamageOrMinus1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA { critChance += (f32)(EFFECT_X_INT * times); }
        );
      }

      {
        const f32 hpPercent = (f32)creature.health / (f32)creature.maxHealth;

        IterateOverEffects(
          EffectConditionType_X__PERCENT_MORE_DAMAGE_TO_ENEMIES_ABOVE__Y__PERCENT_HP,
          data.indexOfWeaponThatDidDamageOrMinus1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA {
            auto requiredPercent = EFFECT_Y_INT;
            if (hpPercent * 100 >= requiredPercent) {
              data.damage
                += Round((f32)data.damage * (f32)(EFFECT_X_INT * times) / 100.0f);
            }
          }
        );

        IterateOverEffects(
          EffectConditionType_X__PERCENT_MORE_DAMAGE_TO_ENEMIES_BELOW__Y__PERCENT_HP,
          data.indexOfWeaponThatDidDamageOrMinus1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA {
            auto requiredPercent = EFFECT_Y_INT;
            if (hpPercent * 100 <= requiredPercent) {
              data.damage
                += Round((f32)data.damage * (f32)(EFFECT_X_INT * times) / 100.0f);
            }
          }
        );
      }

      isCrit = GRAND.FRand() < critChance;
      if (isCrit)
        data.damage += Round((f32)data.damage * data.critDamageMultiplier);

      data.damage = MAX(1, data.damage);

      if (fb_damager->hostility_type() == HostilityType_FRIENDLY) {
        MakeNumber({
          .type  = (isCrit ? NumberType_DAMAGE_CRIT : NumberType_DAMAGE),
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
            (f32)MAX_DODGE_PERCENT / 100.0f,
            (f32)g.run.dynamicStats[StatType_DODGE] / 100.0f
          ))
      {
        MakeNumber({.type = NumberType_DODGE, .pos = creature.pos});

        PlaySound(Sound_GAME_PLAYER_DODGE);

        IterateOverEffects(
          EffectConditionType_X__CHANCE_TO_HEAL__Y__HP_UPON_DODGING,
          -1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA {
            if (GRAND.FRand() < (f32)EFFECT_X_INT / 100.0f)
              HealPlayer(EFFECT_Y_INT * times);
          }
        );

        // TODO: lastInvincibilityTriggeredAt?
        creature.lastDamagedAt = {};
        creature.lastDamagedAt.SetNow();

        g.run.runDodged++;
        AchievementMax(AchievementType_DODGE_N_ATTACKS_DURING_A_RUN, g.run.runDodged);

        return false;
      }

      // Applying player's armor.
      auto armor = (f32)g.run.dynamicStats[StatType_ARMOR];
      if (armor > 0)
        data.damage = Round((f32)data.damage * 1.0f / (1.0f + armor / 15.0f));
      else if (armor < 0)
        data.damage = Round((f32)data.damage * (15.0f - 2 * armor) / (15 - armor));

      // If damager is enemy -> applying StatType_ENEMY_ADDITIONAL_DAMAGE_PERCENT stat.
      if (fb_damager->hostility_type() == HostilityType_MOB) {
        auto percent = g.run.dynamicStats[StatType_ENEMY_ADDITIONAL_DAMAGE_PERCENT];
        data.damage  = Round((f32)data.damage * (1 + (f32)percent / 100.0f));
      }

      data.damage = MAX(1, data.damage);

      MakeNumber({
        .type  = NumberType_DAMAGE_MOB,
        .value = data.damage,
        .pos   = creature.pos,
      });

      IterateOverEffects(
        EffectConditionType_STAT__UPON_TAKING_DAMAGE_BONUS_GETS_LOST_AT_THE_END_OF_THE_WAVE,
        -1,
        [&](
          Weapon* w,
          int     wi,
          auto    fb_effect,
          int     tierOffset,
          int     times,
          int     thisWaveAddedCount
        ) BF_FORCE_INLINE_LAMBDA {
          const auto stat = (StatType)fb_effect->stat_type();
          ChangeDynamicStatBy(
            stat,
            Round(
              GetStatModificationScale(stat) * (f32)fb_effect->value()->Get(tierOffset)
              * (f32)times
            )
          );
        }
      );

      g.run.schedulePlayerHurtSound |= data.playerSound;
    }

    if (data.outWasCrit)
      *data.outWasCrit = isCrit;

    // Player lifesteals.
    if (data.damagerCreatureType == CreatureType_PLAYER) {
      auto weaponType = WeaponType_INVALID;
      int  tier       = 0;
      if (data.indexOfWeaponThatDidDamageOrMinus1 >= 0) {
        const auto& w = g.run.state.weapons[data.indexOfWeaponThatDidDamageOrMinus1];
        weaponType    = w.type;
        tier          = w.tier;
      }

      if (GRAND.FRand() < GetLifestealChance(weaponType, tier)) {
        bool canLifesteal = true;
        if (g.run.playerLastLifestealAt.IsSet()
            && (g.run.playerLastLifestealAt.Elapsed() < LIFESTEAL_COOLDOWN_FRAMES))
          canLifesteal = false;

        if (canLifesteal && (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth)) {
          PLAYER_CREATURE.health
            = MoveTowardsF(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth, 1);
          g.run.playerLastLifestealAt = {};
          g.run.playerLastLifestealAt.SetNow();
        }
      }
    }

    if (!data.creatureIndex) {
      g.run.dangerHPLevelOverlayValue
        += (f32)MIN(data.damage, creature.health) / (f32)creature.health;
    }

    auto prevHealth = creature.health;

    int damaged = MAX(0, MIN(creature.health, data.damage));

    creature.health -= data.damage;

    if (creature.type == CreatureType_PLAYER)
      UpdateHPPercentThresholdEffects(prevHealth);

    int effectDroppedCoins = 0;

    if (data.weaponSound && (data.indexOfWeaponThatDidDamageOrMinus1 >= 0)) {
      PlaySound(glib->weapons()
                  ->Get(g.run.state.weapons[data.indexOfWeaponThatDidDamageOrMinus1].type)
                  ->damage_sound_hash());
    }

    if (creature.health <= 0) {
      if (data.outJustKilled)
        *data.outJustKilled = true;

      if (isCrit) {
        IterateOverEffects(
          EffectConditionType_X__CHANCE_TO_GET__Y__COINS_UPON_KILLING_WITH_CRIT,
          data.indexOfWeaponThatDidDamageOrMinus1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA {
            if (GRAND.FRand() < (f32)EFFECT_X_INT / 100.0f)
              effectDroppedCoins += EFFECT_Y_INT * times;
          }
        );
      }
    }

    IterateOverEffects(
      EffectConditionType_DROP__X__COINS_ON_HIT_WITH__Y__CHANCE,
      data.indexOfWeaponThatDidDamageOrMinus1,
      [&](
        Weapon* w,
        int     wi,
        auto    fb_effect,
        int     tierOffset,
        int     times,
        int     thisWaveAddedCount
      ) BF_FORCE_INLINE_LAMBDA {
        if (GRAND.FRand() < (f32)EFFECT_Y_INT / 100.0f)
          effectDroppedCoins += EFFECT_X_INT * times;
      }
    );

    if (effectDroppedCoins > 0) {
      MakePickupable({
        .type        = PickupableType_COIN,
        .pos         = creature.pos,
        .coin_amount = effectDroppedCoins,
      });
    }

    if (data.indexOfWeaponThatDidDamageOrMinus1 >= 0) {
      auto& w = g.run.state.weapons[data.indexOfWeaponThatDidDamageOrMinus1];
      w.thisWaveDamage += damaged;
      AchievementMax(AchievementType_SINGLE_WEAPON_MAX_WAVE_DAMAGE, w.thisWaveDamage);
    }

    if (data.weaponCanApplyAilment) {
      AilmentCommonData ailment{};
      if (data.indexOfWeaponThatDidDamageOrMinus1 >= 0)
        ailment = GetAilmentFromWeapon(data.indexOfWeaponThatDidDamageOrMinus1);

      if (ailment.type) {
        bool ailmentCanBeApplied = true;

        auto resists = fb->resists_ailment_types();
        if (resists) {
          for (auto v : *resists) {
            if (v == ailment.type) {
              ailmentCanBeApplied = false;
              break;
            }
          }
        }

        // Same weapon can't apply ailment very fast.
        for (auto& a : creature.ailments) {
          if (a.c.type != ailment.type)
            continue;
          if (a.c.weaponIndex != ailment.weaponIndex)
            continue;
          if (a.startedAt.Elapsed() < WEAPON_BURNING_APPLICATION_DEBOUNCE_FRAMES) {
            ailmentCanBeApplied = false;
            break;
          }
        }

        if (ailmentCanBeApplied) {
          ApplyAilment({
            .c        = ailment,
            .creature = &creature,
          });
        }
      }
    }
  }

  creature.lastDamagedAt = {};
  creature.lastDamagedAt.SetNow();
  creature.lastDamagedFlashAt = {};
  creature.lastDamagedFlashAt.SetNow();
  creature.lastDamagedWeaponIndex = data.indexOfWeaponThatDidDamageOrMinus1;

  data.knockbackMeters *= b2Body_GetMass(creature.body.id) * BODY_LINEAR_DAMPING;
  data.knockbackMeters *= 1.0f - fb->knockback_resistance();

  b2Body_ApplyLinearImpulseToCenter(
    creature.body.id, ToB2Vec2(data.directionOrZero * data.knockbackMeters), true
  );

  // Making hit particles.
  if (!creature.killedBecauseOfTheEndOfTheWave) {
    f32 velocityAngle          = 0;
    f32 velocityAnglePlusMinus = PI32;
    if (!Vector2Equals(data.directionOrZero, Vector2Zero())) {
      velocityAngle          = Vector2Angle(data.directionOrZero),
      velocityAnglePlusMinus = PI32 / 6;
    }

    auto color = ColorFromRGBA(fb->hit_particles_color());
    if (creature.type == CreatureType_PLAYER) {
      color = ColorBrightness(
        ColorFromRGBA(glib->builds()->Get(g.player.build)->layer_colors()->Get(4)), 0.25f
      );
    }

    MakeParticles({
      .type                   = ParticleType_HIT,
      .count                  = GRAND.RandInt(3, 6),
      .pos                    = creature.pos,
      .velocity               = 8 * (3 / 2.0f),
      .velocityPlusMinus      = 3.0f,
      .velocityAngle          = velocityAngle,
      .velocityAnglePlusMinus = velocityAnglePlusMinus,
      .initialOffset          = 0.2f,
      .initialOffsetPlusMinus = 0.1f,
      .color                  = color,
    });
  }

  if (!g.run.justDamagedCreatures.Contains(data.creatureIndex))
    *g.run.justDamagedCreatures.Add() = data.creatureIndex;

  if (!data.creatureIndex)
    Save();

  if ((creature.health <= 0)                       //
      && !creature.killedBecauseOfTheEndOfTheWave  //
      && (creature.type != CreatureType_PLAYER))
  {
    // Mob died.

    // Spawning children if mob spawns them on death.
    if (fb->on_death_spawns_creature_type() && CanSpawnMoreCreatures()) {
      int toSpawn
        = GRAND.RandInt(fb->on_death_spawns_count_min(), fb->on_death_spawns_count_max());
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
                  GRAND.Angle()
                );
        } while (!CREATURES_WORLD_SPAWN_BOUNDS.ContainsInside(pos));

        MakePreSpawn({
          .type         = PreSpawnType_CREATURE,
          .typeCreature = (CreatureType)fb->on_death_spawns_creature_type(),
          .pos          = pos,
        });
      }
    }

    // Mob drops coin / apple / chest.
    {
      MakePickupableData data{
        .type        = PickupableType_COIN,
        .pos         = creature.pos,
        .coin_amount = fb->coins_dropped(),
      };
      MakePickupable(data);

      const auto luckFactor = GetLuckFactor();
      if (GRAND.FRand() <= fb->apple_drop_chance() * luckFactor) {
        data.type = PickupableType_APPLE;

        const auto chestChance = fb->chest_instead_of_apple_factor() * luckFactor
                                 / (f32)(1 + g.run.chestsDroppedThisWave);
        if (GRAND.FRand() <= chestChance) {
          data.type = PickupableType_CHEST;
          g.run.chestsDroppedThisWave++;
        }

        MakePickupable(data);
      }
    }

    // Counting mob as player killed.
    {
      g.run.state.playerKilledEnemies++;
      AchievementAdd(AchievementType_KILLER, 1);

      if (creature.lastDamagedWeaponIndex >= 0) {
        g.run.state.weapons[creature.lastDamagedWeaponIndex].thisWaveKilledEnemies++;
      }

      // Applying effects: STAT__EVERY__X__KILLED_ENEMIES.
      IterateOverEffects(
        EffectConditionType_STAT__EVERY__X__KILLED_ENEMIES,
        creature.lastDamagedWeaponIndex,
        [&](
          Weapon* w,
          int     wi,
          auto    fb_effect,
          int     tierOffset,
          int     times,
          int     thisWaveAddedCount
        ) BF_FORCE_INLINE_LAMBDA {
          if (w) {
            if ((w->thisWaveKilledEnemies % EFFECT_X_INT) == 0)
              ApplyStatEffect(fb_effect, tierOffset, 1);
          }
          else if ((g.run.state.playerKilledEnemies % EFFECT_X_INT) == 0)
            ApplyStatEffect(fb_effect, tierOffset, times);
        }
      );
    }

    // Wave gets set to completed upon killing boss.
    if (fb->is_boss() && !g.run.scheduledWaveCompleted.IsSet()) {
      TriggerWaveCompleted(false);
      SetWaveWonTrue();
      PlaySound(Sound_GAME_WAVE_WON);
      Save();
    }
  }

  return true;
}

void Pickup(int pickupableIndex) {  ///
  g.run.pickupables[pickupableIndex].pickedUpAt.SetNow();
}

void OnCoinFlewToStash(int amount) {  ///
  g.run.state.notPickedUpCoins += amount;
  g.ui.changedNotPickedUpCoinsAt = {};
  g.ui.changedNotPickedUpCoinsAt.SetNow();
}

void OnPickedUp(int pickupableIndex) {  ///
#define PICKUPABLE (g.run.pickupables[pickupableIndex])

  auto fb_creatures = glib->creatures();

  const int appleOrChestHeal = g.run.dynamicStats[StatType_APPLE_HEAL];
  IterateOverEffects(
    EffectConditionType_X__CHANCE_TO_DEAL__Y__DAMAGE_UPON__PICKUPABLE,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      if (fb_effect->pickupable_type() != PICKUPABLE.type)
        return;
      if (GRAND.FRand() >= (f32)EFFECT_X_INT / 100.0f)
        return;

      const auto fb_weapon = (w ? glib->weapons()->Get(w->type) : nullptr);

      FOR_RANGE (int, i, 5) {
        const auto  creatureIndex = GRAND.Rand() % g.run.creatures.count;
        const auto& creature      = g.run.creatures[creatureIndex];

        if (creature.diedAt.IsSet())
          continue;
        if (creature.health <= 0)
          continue;

        auto fb = fb_creatures->Get(creature.type);
        if (fb->hostility_type() == HostilityType_FRIENDLY)
          continue;

        int damage = EFFECT_Y_INT * times;
        damage     = ApplyDamageScalings(damage, 0, fb_effect->damage_scalings(), times);
        damage     = ApplyPlayerStatDamageMultiplier(damage);

        f32 critDamageMultiplier = 1.5f;
        int weaponCritChance     = 0;
        if (w) {
          const int tierOffset = w->tier - fb_weapon->min_tier_index();
          critDamageMultiplier = fb_weapon->crit_damage_multiplier()->Get(tierOffset);
          weaponCritChance     = fb_weapon->crit_chance()->Get(tierOffset);
        }

        TryApplyDamage({
          .creatureIndex                      = (int)creatureIndex,
          .damage                             = MAX(1, damage),
          .damagerCreatureType                = CreatureType_PLAYER,
          .critDamageMultiplier               = critDamageMultiplier,
          .weaponCritChance                   = weaponCritChance,
          .indexOfWeaponThatDidDamageOrMinus1 = wi,
        });

        break;
      }
    }
  );

  IterateOverEffects(
    EffectConditionType_X__CHANCE_TO_HEAL__Y__HP_UPON__PICKUPABLE,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      if (GRAND.FRand() < (f32)EFFECT_X_INT / 100.0f) {
        if (fb_effect->pickupable_type() == PICKUPABLE.type)
          HealPlayer(EFFECT_Y_INT * times);
      }
    }
  );

  IterateOverEffects(
    EffectConditionType_X__COINS_UPON__PICKUPABLE,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      if (fb_effect->pickupable_type() == PICKUPABLE.type) {
        const int amount = times * EFFECT_X_INT;
        ChangeCoins(amount);
      }
    }
  );

  switch (PICKUPABLE.type) {
  case PickupableType_COIN: {
    const auto& data    = PICKUPABLE.DataCoin();
    int         amount  = data.amount;
    int         xNumber = 1;

    ASSERT(amount > 0);

    if (g.run.scheduledWaveCompleted.IsSet())
      OnCoinFlewToStash(amount);
    else {
      if (g.run.state.notPickedUpCoins > 0) {
        int toAdd = amount;
        if (toAdd > g.run.state.notPickedUpCoins)
          toAdd = g.run.state.notPickedUpCoins;
        ASSERT(toAdd > 0);

        g.run.state.notPickedUpCoins -= toAdd;
        amount += toAdd;
        xNumber *= 2;
      }

      if (GRAND.FRand() < (f32)g.run.dynamicStats[StatType_DOUBLE_COIN_CHANCE] / 100.0f) {
        amount *= 2;
        xNumber *= 2;
      }

      if (xNumber > 1) {
        MakeNumber({
          .type  = NumberType_PICKUPABLE,
          .value = xNumber,
          .pos   = PLAYER_CREATURE.pos + Vector2(0, PLAYER_PICKUP_NUMBER_Y_OFFSET),
        });
      }

      ChangeCoins(amount);
      AddXP((f32)amount);

      auto healChance = (f32)g.run.dynamicStats[StatType_COINS_HEAL] / 100.0f;
      if (GRAND.FRand() < healChance)
        HealPlayer();
    }
  } break;

  case PickupableType_APPLE: {
    if (appleOrChestHeal > 0)
      HealPlayer(appleOrChestHeal);
  } break;

  case PickupableType_CHEST: {
    if (appleOrChestHeal > 0)
      HealPlayer(appleOrChestHeal);
    g.run.state.chests++;
  } break;

  default:
    INVALID_PATH;
  }

#undef PICKUPABLE
}

void MakePickupable(MakePickupableData data) {  ///
  ASSERT(data.coin_amount >= 0);
  ASSERT(data.type);

  auto       fb       = glib->pickupables()->Get(data.type);
  const bool mirrored = GRAND.Rand() % 2;

  Pickupable pickupable{
    .type      = data.type,
    .variation = (u8)(GRAND.Rand() % fb->variation_texture_ids()->size()),
    .pos       = data.pos,
    .rotation  = PI32 / 12 * GRAND.FRand11(),
    .mirrored  = mirrored,
  };

  switch (data.type) {
  case PickupableType_COIN: {
    auto& d  = pickupable.DataCoin();
    d.amount = data.coin_amount;
    ASSERT(d.amount > 0);
  } break;

  case PickupableType_APPLE:
  case PickupableType_CHEST: {
    // Intentionally left blank.
  } break;

  default:
    INVALID_PATH;
    break;
  }

  pickupable.createdAt.SetNow();

  f32 chanceToInstaPickup = 0;
  IterateOverEffects(
    EffectConditionType_X__CHANCE_TO_INSTANTLY_ATTRACT__PICKUPABLE,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      if (pickupable.type == fb_effect->pickupable_type())
        chanceToInstaPickup += (f32)(EFFECT_X_INT * times) / 100.0f;
    }
  );

  const int pickupableIndex = g.run.pickupables.count;
  *g.run.pickupables.Add()  = pickupable;

  if (GRAND.FRand() < chanceToInstaPickup)
    Pickup(pickupableIndex);
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

void ResetPlaceholders() {  ///
  ASSERT_FALSE(g.uiFlex.pgroupsLastIsActive);
  g.uiFlex.pgroupsFirst = nullptr;
  g.uiFlex.pgroupsLast  = nullptr;
}

void BF_CLAY_IMAGE(
  ClayImageData data,
  auto          innerLambda,
  bool          _resetPlaceholders = true
) {  ///
  if (!Vector2Equals(data.overriddenSize, Vector2Zero()))
    ASSERT_FALSE(data.dontCareAboutScaleWhenCalculatingSize);
  if (data.dontCareAboutScaleWhenCalculatingSize)
    ASSERT(Vector2Equals(data.overriddenSize, Vector2Zero()));

  const auto texture      = glib->atlas_textures()->Get(data.texID);
  const auto originalSize = glib->original_texture_sizes()->Get(data.texID);

  f32 w = (f32)originalSize->x() * ASSETS_TO_LOGICAL_RATIO;
  f32 h = (f32)originalSize->y() * ASSETS_TO_LOGICAL_RATIO;

  if (!data.dontCareAboutScaleWhenCalculatingSize) {
    w *= data.scale.x;
    h *= data.scale.y;
  }

  ASSERT(data.overriddenSize.x >= 0);
  ASSERT(data.overriddenSize.y >= 0);
  if (data.overriddenSize.x > 0)
    w = data.overriddenSize.x;
  if (data.overriddenSize.y > 0)
    h = data.overriddenSize.y;

  if (g.uiFlex.active)
    FlexAddRowForChildIfNeeded(Ceil(w));

  auto& beautifiers      = g.ui.clayBeautifiers;
  auto& beautifiersCount = g.ui.clayBeautifiersCount;

  CLAY({
    .layout{.sizing{.width = CLAY_SIZING_FIXED(w), .height = CLAY_SIZING_FIXED(h)}},
  }) {
    CLAY({
      .layout{.sizing{.width = CLAY_SIZING_FIXED(w), .height = CLAY_SIZING_FIXED(h)}},
      .floating{
        .zIndex = g.ui.clayZIndex,
        .attachPoints{
          .element = CLAY_ATTACH_POINT_CENTER_CENTER,
          .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
        },
        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
        .attachTo           = CLAY_ATTACH_TO_PARENT,
      },
    }) {
      FLOATING_BEAUTIFY;
      CLAY({
        .layout{.sizing{.width = CLAY_SIZING_FIXED(w), .height = CLAY_SIZING_FIXED(h)}},
        .image{.imageData = PushClayImageData(data)},
      }) {
        innerLambda();
      }
    }
  }

  if (_resetPlaceholders)
    ResetPlaceholders();
}

void BF_CLAY_IMAGE(ClayImageData data, bool _resetPlaceholders = true) {  ///
  BF_CLAY_IMAGE(data, [] {}, _resetPlaceholders);
}

struct ClayTextOptions {  ///
  Color color = palTextWhite;

  // Others: CLAY_TEXT_WRAP_NEWLINES, CLAY_TEXT_WRAP_NONE.
  Clay_TextElementConfigWrapMode wrapMode = CLAY_TEXT_WRAP_WORDS;
};

// NOTE: This overload DOESN'T SAVE string to trash arena.
void BF_CLAY_TEXT(Clay_String string, ClayTextOptions opts = {}) {  ///
  u16 fontID = 0;
  if (g.ui.overriddenFont)
    fontID = (UINT_FROM_PTR(g.ui.overriddenFont) - UINT_FROM_PTR(&g.meta.fontUI))
             / sizeof(Font);
  ASSERT(fontID >= 0);

  if (g.uiFlex.active) {
    Clay_StringSlice s{
      .length    = string.length,
      .chars     = string.chars,
      .baseChars = string.chars,
    };
    Clay_TextElementConfig cfg{
      .fontId   = fontID,
      .wrapMode = opts.wrapMode,
    };
    auto dim = MeasureText(s, &cfg, nullptr);

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
      .textColor = ToClayColor(opts.color),
      .fontId    = fontID,
      // fontSize fixes clay's incorrect MeasureText cache
      .fontSize = (u16)Hash32((u8*)&fontID, sizeof(fontID)),
      .wrapMode = opts.wrapMode,
    })
  );
}

// NOTE: This overload SAVES string to trash arena.
void BF_CLAY_TEXT(const char* text, ClayTextOptions opts = {}) {  ///
  int         len           = 0;
  const char* allocatedText = PushTextToArena(&g.meta.trashArena, text, &len);
  Clay_String string{
    .length = (i32)len,
    .chars  = allocatedText,
  };
  BF_CLAY_TEXT(string, opts);
}

void BF_CLAY_TEXT_BROKEN_LOCALIZED(
  int             locale_,
  ClayTextOptions opts               = {},
  bool            _resetPlaceholders = true
) {                                  ///
  const auto locale = (Loc)locale_;  // NOTE: For debug.

  const auto localization              = glib->localizations()->Get(ge.meta.localization);
  const auto localization_broken_lines = localization->broken_lines();

  if (BF_DEBUG) {
    // Checking that all required placeholders were set.
    for (auto line : *localization_broken_lines->Get(locale_)->lines()) {
      for (auto group : *line->groups()) {
        for (auto string : *group->strings()) {
          if (!string->placeholder())
            continue;

          const auto requiredPlaceholder = string->placeholder()->c_str();

          bool found = false;
          auto group = g.uiFlex.pgroupsFirst;
          while (group) {
            if (!strcmp(group->placeholder, requiredPlaceholder)) {
              found = true;
              break;
            }
            group = group->next;
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
          BF_CLAY_TEXT(text, opts);
          continue;
        }

        if (string->placeholder()) {
          const auto placeholderString = string->placeholder()->c_str();

          auto group = g.uiFlex.pgroupsFirst;
          bool found = false;
          while (group) {
            if (!strcmp(group->placeholder, placeholderString)) {
              found = true;
              break;
            }
            group = group->next;
          }
          ASSERT(found);

          auto pp = group->first;
          while (pp) {
            ASSERT(pp->type);
            clayPlaceholderFunctions[(int)pp->type - 1](pp);
            pp = pp->next;
          }
        }
        else {
          Clay_String text{
            .isStaticallyAllocated = true,
            .length                = (i32)string->string()->size(),
            .chars                 = string->string()->c_str(),
          };
          BF_CLAY_TEXT(text, opts);
        }
      }
    }
  }

  if (_resetPlaceholders)
    ResetPlaceholders();
}

void DestroyBody(Body* body) {  ///
  b2DestroyBody(body->id);
  for (auto& shape : g.run.bodyShapes) {
    if (shape.body.createdID == body->createdID)
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

  static int     lastCreatedID = 0;
  MakeBodyResult result{
    .body{.createdID = ++lastCreatedID, .id = body},
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

struct MakeRectBodyData {  ///
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

struct MakeCircleBodyData {  ///
  Vector2      pos           = {};
  f32          radius        = {};
  f32          hurtboxRadius = {};
  MakeBodyData bodyData      = {};
};

Body MakeCircleBody(MakeCircleBodyData data) {  ///
  ASSERT(data.radius > 0);

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
  if (index && gdebug.disableMobSpawns && !fb->is_boss())
    return -1;
  if (gdebug.disableBossSpawn && fb->is_boss())
    return -1;

  int health = fb->health()
               + Round(
                 ((g.run.state.waveIndex - fb->appearing_wave_number() + 1))
                 * fb->health_increase_per_wave()
               );

  if (fb->hostility_type() != HostilityType_FRIENDLY) {
    const f32 mobHpScale = 1 + (f32)g.run.dynamicStats[StatType_ENEMY_HP_SCALE] / 100.0f;
    health               = Round((f32)health * mobHpScale);
  }

  const auto creatureID = g.run.nextCreatureID++;

  Creature creature{
    .id        = creatureID,
    .type      = data.type,
    .health    = health,
    .maxHealth = health,
    .pos       = data.pos,
    .body      = MakeCircleBody({
           .pos           = data.pos,
           .radius        = CREATURE_COLLIDER_RADIUS * fb->collider_scale(),
           .hurtboxRadius = fb->hurtbox_radius(),
           .bodyData{
             .type     = BodyType_CREATURE,
             .userData = ShapeUserData::Creature(creatureID),
             .isPlayer = (data.type == CreatureType_PLAYER),
      },
    }),
    .speed     = fb->speed() + Lerp(-1.0f, 1.0f, GRAND.FRand()) * fb->speed_plus_minus(),
  };
  creature.idleStartedAt.SetNow();
  creature.spawnedAt.SetNow();

  if ((fb->aggro_distance() != f32_inf) || !fb->can_aggro()) {
    creature.controller.move = Vector2Rotate({1, 0}, GRAND.Angle());
    creature.speedModifier *= fb->not_aggroed_speed() / fb->speed();
  }

  switch (creature.type) {
  case CreatureType_TURRET: {
    creature.DataTurret() = {
      .aimDirection   = Vector2Rotate({1, 0}, GRAND.Angle()),
      .damage         = data.damage,
      .damageScalings = data.damageScalings,
    };
    g.run.turretsOnTheMapCount++;
    AchievementMax(AchievementType_TURRETS_ON_THE_MAP, g.run.turretsOnTheMapCount);
  } break;

  case CreatureType_RANGER: {
    creature.DataRanger() = {};
  } break;

  case CreatureType_RUSHER: {
    creature.DataRusher() = {};
  } break;

  case CreatureType_BOSS: {
    auto& d = creature.DataBoss();
    d       = {.shootingPattern = -1};
    d.startedShootingAt.SetNow();
    d.cooldown.SetRandSeconds(
      glib->boss_cooldown_min_seconds(), glib->boss_cooldown_max_seconds()
    );
  } break;

  case CreatureType_TREE: {
    if (GRAND.Rand() % 2)
      creature.dir.x *= -1;
  } break;

  default:
    break;
  }

  *slot = creature;
  return creature.id;
}

void MakeProjectile(MakeProjectileData data) {  ///
  ASSERT(data.c.type);
  ASSERT(data.c.dir != Vector2Zero());
  if (data.c.ownerCreatureType == CreatureType_PLAYER)
    ASSERT(data.c.weaponIndexOrMinus1 >= 0);

  *g.run.projectilesToMake.Add() = data;
}

void GamePreInit() {  ///
  ge.meta.logicRand           = Random(SDL_GetPerformanceCounter());
  ge.settings.backgroundColor = ColorFromRGBA(0x241207ff);
}

struct Line {  ///
  Vector2 v1 = {};
  Vector2 v2 = {};
};

struct MakeWallsData {  ///
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

  bool itemCanBeAdded = false;
  for (const auto& pool : g.run.itemPools) {
    if (pool.count > 0) {
      itemCanBeAdded = true;
      break;
    }
  }
  ASSERT(itemCanBeAdded);

  while (1) {
    const int tier             = GetRandomTier();
    int       currentItemCount = 0;

    auto& pool = g.run.itemPools[tier];
    if (pool.count <= 0)  // TODO: decrease tier by 1.
      continue;

    const ItemType type = pool[GRAND.Rand() % pool.count].type;

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

    if (g.player.lockedItems[type].achievement)
      continue;

    auto fb = fb_items->Get(type);
    if (fb->deprecated())
      continue;

    if (fb->limit() && (currentItemCount >= fb->limit()))
      continue;

    return type;
  }

  INVALID_PATH;
  return {};
}

f32 _ApplyStatPrice(StatType s, f32 price) {  ///
  const StatType allowed_[]{StatType_SHOP_PRICE, StatType_REROLL_PRICE};
  VIEW_FROM_ARRAY_DANGER(allowed);
  ASSERT(allowed.Contains(s));

  int stat = g.run.state.staticStats[s];
  if (stat == 0)
    return price;

  stat             = MAX(-100, stat);
  const f32 factor = 1 + (f32)stat / 100.0f;
  price            = (f32)price * factor;
  return price;
}

int GetNumberOfTreesToSpawn() {  ///
  // ref: https://brotato.wiki.spellsandguns.com/Trees#Spawn
  const int stat = g.run.dynamicStats[StatType_TREES];

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

enum ItemOrWeaponType {  ///
  ItemOrWeaponType_INVALID,
  ItemOrWeaponType_ITEM,
  ItemOrWeaponType_WEAPON,
};

f32 CalculateItemOrWeaponPrice(f32 price, int tier, ItemOrWeaponType type) {  ///
  ASSERT(type);
  ASSERT(tier >= 0);
  ASSERT(tier < TOTAL_TIERS);
  price *= PRICE_SCALINGS_PER_TIER[tier];

  const int wave = g.run.state.waveIndex + 1;
  price          = (price + wave + (price * wave * 0.1f));

  if (type == ItemOrWeaponType_ITEM)
    price *= 1 + (f32)g.run.state.staticStats[StatType_ITEMS_PRICE] / 100.0f;
  else if (type == ItemOrWeaponType_WEAPON)
    price *= 1 + (f32)g.run.state.staticStats[StatType_WEAPONS_PRICE] / 100.0f;

  price = MAX(price, 1);
  return price;
}

int ToRecyclePrice(f32 price) {  ///
  int percent = 0;
  IterateOverEffects(
    EffectConditionType_X__PERCENT_MORE_COINS_FROM_RECYCLING,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA { percent += EFFECT_X_INT * times; }
  );
  f32 scale = MAX(0, 1.0f + (f32)percent / 100.0f) / 3.0f;
  return MAX(1, Round(price * scale));
}

void AddItem(ItemType type) {  ///
  auto& items = g.run.state.items;

  int increasedExistingItemIndex = -1;

  int itemIndex = -1;
  for (auto& item : items) {
    itemIndex++;
    if (item.type == type) {
      item.count++;
      increasedExistingItemIndex = itemIndex;
      break;
    }
  }

  if (increasedExistingItemIndex >= 0) {
    items[increasedExistingItemIndex].uiBouncedAt = {};
    items[increasedExistingItemIndex].uiBouncedAt.SetNow();

    // Moving increased-count-item to the end.
    auto t = items[increasedExistingItemIndex];
    t.thisWaveAddedCount++;
    items.RemoveAt(increasedExistingItemIndex);
    *items.Add() = t;
  }
  else {
    Item item{
      .type               = type,
      .count              = 1,
      .thisWaveAddedCount = 1,
    };
    item.uiBouncedAt.SetNow();
    *items.Add() = item;
  }

  IterateOverItemEffects(
    nullptr,
    -1,
    {},
    type,
    1,
    0,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA { ApplyStatEffect(fb_effect, tierOffset, times); }
  );

  // Removing item from pool.
  {
    auto  fb   = glib->items()->Get(type);
    auto& pool = g.run.itemPools[fb->tier()];

    int poolItemIndex = -1;
    for (auto& poolItem : pool) {
      poolItemIndex++;

      if (poolItem.type == type) {
        poolItem.count -= 1;
        if (poolItem.count <= 0)
          pool.UnstableRemoveAt(poolItemIndex);

        break;
      }
    }
  }
}

void _UpdateImmediateWeaponEffects(int weaponIndex, int scale) {  ///
  int occupiedSlots = 0;
  int emptySlots    = 0;
  for (auto& w : g.run.state.weapons) {
    if (w.type)
      occupiedSlots++;
    else
      emptySlots++;
  }

  struct {
    EffectConditionType c;
    int                 times;
  } toUpdate_[]{
    {EffectConditionType_DEFAULT_GET__STAT, 1},
    {EffectConditionType_STAT__FOR_EVERY_OCCUPIED_WEAPON_SLOT, occupiedSlots},
    {EffectConditionType_STAT__FOR_EVERY_EMPTY_WEAPON_SLOT, emptySlots},
  };
  VIEW_FROM_ARRAY_DANGER(toUpdate);

  const auto& weapon = g.run.state.weapons[weaponIndex];
  const int   tierOffset
    = weapon.tier - glib->weapons()->Get(weapon.type)->min_tier_index();

  for (auto c : toUpdate) {
    IterateOverWeaponEffects(
      c.c,
      weapon.type,
      [&](auto fb_effect) BF_FORCE_INLINE_LAMBDA {
        ApplyStatEffect(fb_effect, tierOffset, c.times * scale);
      }
    );
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

  // Initializing `g.run.state.staticStats`.
  FOR_RANGE (int, stat, (int)StatType_COUNT) {  ///
    const auto fb                 = glib->stats()->Get(stat);
    g.run.state.staticStats[stat] = fb->player_value();
  }

  // Making player. {  ///
  auto playerPos = WORLD_SIZEf / 2.0f;
  MakeCreature({
    .type = CreatureType_PLAYER,
    .pos  = playerPos,
  });
  g.run.previousPlayerPos = playerPos;
  g.run.playerLastRegenAt.SetNow();
  // }

  g.run.camera.pos = GetCameraTargetPos();

  {
    if (!g.player.weapon) {
      g.player.weapon = (WeaponType)glib->builds()
                          ->Get(g.player.build)
                          ->starting_weapon_types()
                          ->Get(0);
    }
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

  IterateOverEffects(
    {},
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA { ApplyStatEffect(fb_effect, tierOffset, times); }
  );

  // Processing EffectConditionType_START_WITH__X__ITEM_OR_WEAPON.
  IterateOverEffects(  ///
    EffectConditionType_START_WITH__X__ITEM_OR_WEAPON,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      ASSERT(times > 0);
      ASSERT(tierOffset == 0);

      auto item   = (ItemType)fb_effect->item_type();
      auto weapon = (WeaponType)fb_effect->weapon_type();

      if (item)
        AddItem(item);

      if (weapon) {
        int  weaponIndex = -1;
        bool added       = false;

        for (auto& w : g.run.state.weapons) {
          ASSERT(times > 0);
          weaponIndex++;
          if (w.type)
            continue;

          added = true;
          w     = {.type = weapon};

          times--;
          if (times <= 0)
            break;
        }
        ASSERT(added);

        if (added)
          _UpdateImmediateWeaponEffects(weaponIndex, 1);
      }
    }
  );

  PLAYER_CREATURE.health    = MAX(1, g.run.state.staticStats[StatType_HP]);
  PLAYER_CREATURE.maxHealth = MAX(1, g.run.state.staticStats[StatType_HP]);

  // Refilling `itemPools` and `weaponPools`.
  {  ///
    g.run.itemPools[0] = {.count = ITEMS_PER_TIER[0], .base = g.run.itemPool0.base};
    g.run.itemPools[1] = {.count = ITEMS_PER_TIER[1], .base = g.run.itemPool1.base};
    g.run.itemPools[2] = {.count = ITEMS_PER_TIER[2], .base = g.run.itemPool2.base};
    g.run.itemPools[3] = {.count = ITEMS_PER_TIER[3], .base = g.run.itemPool3.base};

    int tieredCounts[TOTAL_TIERS]{};

    int itemIndex = -1;
    for (auto fb : *glib->items()) {
      itemIndex++;
      if (!itemIndex)
        continue;

      const int tier        = fb->tier();
      auto&     pool        = g.run.itemPools[tier];
      auto&     tieredCount = tieredCounts[tier];

      pool[tieredCount++] = {
        .type  = (ItemType)itemIndex,
        .count = (fb->limit() ? fb->limit() : int_max),
      };
    }

    FOR_RANGE (int, i, TOTAL_TIERS)
      ASSERT(tieredCounts[i] == ITEMS_PER_TIER[i]);

    FOR_RANGE (int, i, TOTAL_TIERS)
      g.run.weaponPools[i] = {.count = 0, .base = g.run.weaponPoolsData[i].base};

    int weaponIndex = -1;
    for (auto fb : *glib->weapons()) {
      weaponIndex++;
      if (!weaponIndex)
        continue;

      for (int tier = fb->min_tier_index(); tier < TOTAL_TIERS; tier++) {
        auto& pool         = g.run.weaponPools[tier];
        pool[pool.count++] = (WeaponType)weaponIndex;
      }
    }

    FOR_RANGE (int, i, TOTAL_TIERS)
      ASSERT(g.run.weaponPools[i].count <= g.run.weaponPoolsData[i].count);
  }

  // Selecting next biome.
  {  ///
    int biome = (int)g.player.biome + 1;
    if (biome >= BiomeType_COUNT)
      biome = 1;
    g.player.biome = (BiomeType)biome;
  }

  // Filling props.
  {  ///
    constexpr int  margin = 1.0f;
    constexpr Rect bounds{
      .pos{margin, margin}, .size{WORLD_X - 2 * margin, WORLD_Y - 2 * margin}
    };

    g.run.props = {.count = 0, .base = g.run.propsData.base};
    const int variations
      = (int)glib->biomes()->Get(g.player.biome)->prop_texture_ids()->size();

    int groupDataIndex = -1;
    int toSpawnInGroup = 0;
    for (const auto& toSpawnGroups : PROP_GROUP_COUNTS) {
      groupDataIndex++;
      toSpawnInGroup++;

      FOR_RANGE (int, _, toSpawnGroups) {
        while (1) {
          const auto pos = bounds.GetRandomVisualPosInside();

          bool shouldContinue = false;
          for (auto& prop : g.run.props) {
            if (Vector2DistanceSqr(prop.pos, pos) <= SQR(1.8f)) {
              shouldContinue = true;
              break;
            }
          }
          if (shouldContinue)
            continue;

          FOR_RANGE (int, propIndex, toSpawnInGroup) {
            const auto offset = Lerp(0.4f, 1.0f, VRAND.FRand())
                                * PROP_GROUP_OFFSET_SCALES[groupDataIndex];
            const auto rotatedOffset = Vector2Rotate(
              {offset, 0},
              (f32)propIndex * 2 * PI32 / (f32)toSpawnInGroup + VRAND.FRand11() * PI32 / 8
            );

            const auto propPos = pos + rotatedOffset;
            if (!bounds.ContainsInside(propPos))
              continue;

            g.run.props[g.run.props.count++] = {
              .variation = (int)(VRAND.Rand() % variations),
              .right     = (bool)(VRAND.Rand() % 2),
              .pos       = propPos,
              .rotation  = GRAND.FRand11() * PI32 / 24,
            };
          };
          break;
        }
      }
    }
  }

  OnWaveStarted();
}

void ReloadFontsIfNeeded() {  ///
  static bool       loaded             = false;
  static Vector2Int previousScreenSize = {};

  if ((ge.meta.screenSize.x <= 0) || (ge.meta.screenSize.y <= 0))
    return;

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
  static int  numberCodepoints[]{
    ' ', '+', '-', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'x'
  };

  static LoadFontData loadFontData_[]{
    // fontUI.
    {
      .filepath        = fontpath,
      .size            = 18,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
    },
    // fontUIOutlined.
    {
      .filepath        = fontpath,
      .size            = 18,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
      .outlineWidth    = 3,
      .outlineAdvance  = 1,
    },
    // fontUIBig.
    {
      .filepath        = fontpath,
      .size            = 22,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
    },
    // fontUIBigOutlined.
    {
      .filepath        = fontpath,
      .size            = 22,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
      .outlineWidth    = 3,
      .outlineAdvance  = 1,
    },
    // fontStats.
    {
      .filepath        = fontpath,
      .size            = 15,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
    },
    // fontPricesOutlined.
    {
      .filepath        = fontpath,
      .size            = 20,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = numberCodepoints,
      .codepointsCount = ARRAY_COUNT(numberCodepoints),
      .outlineWidth    = 3,
      .outlineAdvance  = 1,
    },
    // fontItemCountsOutlined.
    {
      .filepath        = fontpath,
      .size            = 20,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = numberCodepoints,
      .codepointsCount = ARRAY_COUNT(numberCodepoints),
      .outlineWidth    = 3,
      .outlineAdvance  = 1,
    },
    // fontUIGiganticOutlined.
    {
      .filepath        = fontpath,
      .size            = 40 * 5 / 4,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = g_codepoints,
      .codepointsCount = ARRAY_COUNT(g_codepoints),
      .outlineWidth    = 4,
      .outlineAdvance  = 0,
    },
    // fontUINextWave.
    {
      .filepath        = fontpath,
      .size            = 40,
      .FIXME_sizeScale = 45.0f / 30.0f,
      .codepoints      = numberCodepoints,
      .codepointsCount = ARRAY_COUNT(numberCodepoints),
      .outlineWidth    = 3,
      .outlineAdvance  = 0,
    },
  };
  VIEW_FROM_ARRAY_DANGER(loadFontData);

  loaded = true;
  g.meta.loadedFonts
    = LoadFonts({.count = loadFontData.count, .base = &g.meta.fontUI}, loadFontData);
}

void GameInit() {
  ZoneScoped;

  SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
  SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");

  // Checking gamelib.
  if (BF_ENABLE_ASSERTS) {  ///
    LAMBDA (
      void,
      checkEffects,
      (auto                  fb_effects,
       bool                  canHaveWeaponOnly = false,
       bool                  canHaveBuildOnly  = false,
       int                   tierValues        = 1,
       const BFGame::Weapon* fb_weapon         = nullptr)
    )
    {
      const bool allowed = canHaveWeaponOnly && canHaveBuildOnly;
      ASSERT_FALSE(allowed);

      if (!fb_effects)
        return;

      int effectIndex = -1;  // NOTE: For debug.
      for (auto fb_effect : *fb_effects) {
        effectIndex++;

        auto condType = (EffectConditionType)fb_effect->effectcondition_type();
        auto fb_cond  = glib->effect_conditions()->Get(condType);

        int allowedValues_[]{0, 0};
        VIEW_FROM_ARRAY_DANGER(allowedValues);
        if (canHaveBuildOnly)
          allowedValues[1] = 2;
        if (canHaveWeaponOnly)
          allowedValues[1] = 1;
        ASSERT(allowedValues.Contains(fb_cond->restrict()));

        if (!canHaveWeaponOnly) {
          ASSERT_FALSE(fb_effect->only_this_weapon());
          ASSERT_FALSE(fb_effect->only_other_weapons());
          ASSERT_FALSE(fb_effect->all_weapons());
        }

        // Max 1 can be specified.
        ASSERT(
          (int)fb_effect->only_this_weapon()        //
            + (int)fb_effect->only_other_weapons()  //
            + (int)fb_effect->all_weapons()
          <= 1
        );

        ASSERT(fb_cond->requires_stat() == (bool)fb_effect->stat_type());
        ASSERT(fb_cond->requires_stat_2() == (bool)fb_effect->stat_type_2());
        ASSERT(fb_cond->requires_property() == (bool)fb_effect->weaponproperty_type());
        ASSERT(fb_cond->requires_projectile() == (bool)fb_effect->projectile_type());

        const bool allowed = fb_cond->requires_stat() && fb_cond->requires_property();
        ASSERT_FALSE(allowed);

        if (fb_cond->requires_stat() || fb_cond->requires_property()) {
          auto       v                     = fb_effect->value();
          auto       vm                    = fb_effect->value_multiplier();
          const bool atLeastOneIsSpecified = v || vm;
          ASSERT(atLeastOneIsSpecified);
          if (v)
            ASSERT(v->size() == tierValues);
          if (vm)
            ASSERT(vm->size() == tierValues);
        }
        else
          ASSERT_FALSE(fb_effect->value());

        if (fb_cond->requires_projectile())
          ASSERT(fb_effect->projectile_range_meters() > 0);

        if (fb_cond->rectrict_only_melee_weapons())
          ASSERT(fb_cond->restrict() == 1);
        if (fb_cond->rectrict_only_ranged_weapons())
          ASSERT(fb_cond->restrict() == 1);
        if (fb_cond->restrict() == 1) {
          if (fb_cond->rectrict_only_melee_weapons())
            ASSERT_FALSE(fb_weapon->projectile_type());
          if (fb_cond->rectrict_only_ranged_weapons())
            ASSERT(fb_weapon->projectile_type());
        }

        {
          int required = 0;
          for (auto v : *fb_cond->placeholders()) {
            if (v->required())
              required++;
          }

          int  found = 0;
          auto fep   = fb_effect->placeholders();
          if (fep)
            found = fep->size();

          if (found > required)
            INVALID_PATH;  // You specified MORE entries in `placeholders` than needed.
          if (found < required)
            INVALID_PATH;  // You specified LESS entries in `placeholders` than needed.
        }

        if (fb_cond->requires_item_or_weapon()) {
          // Either item or weapon.
          auto i                     = fb_effect->item_type();
          auto w                     = fb_effect->weapon_type();
          auto atLeastOneIsSpecified = i || w;
          auto bothAreSpecified      = i && w;
          ASSERT(atLeastOneIsSpecified);
          ASSERT_FALSE(bothAreSpecified);
        }

        int reqIndex = -1;
        for (auto fb_req : *fb_cond->placeholders()) {
          reqIndex++;
          if (!fb_req->required())
            continue;

          auto placValues = fb_effect->placeholders()->Get(reqIndex);

          switch ((CondVarType)fb_req->condvar_type()) {
          case CondVarType_INTEGER: {
            ASSERT(placValues->ints());
            ASSERT(placValues->ints()->size() == tierValues);
          } break;

          case CondVarType_FLOAT: {
            ASSERT(placValues->floats());
            ASSERT(placValues->floats()->size() == tierValues);
          } break;

          case CondVarType_DAMAGE: {
            FOR_RANGE (int, i, tierValues)
              ASSERT(placValues->ints()->Get(i) >= 0);
          } break;

          default:
            break;
          }
        }
      }
    };

    for (auto fb : *glib->difficulties()) {
      // NOTE: For debug.
      auto id = glib->localization_debug_strings()->Get(fb->name_locale())->c_str();
      checkEffects(fb->effects(), false, false);
    }
    for (auto fb : *glib->builds()) {
      // NOTE: For debug.
      auto id = glib->localization_debug_strings()->Get(fb->name_locale())->c_str();
      checkEffects(fb->effects(), false, true);
    }
    for (auto fb : *glib->items()) {
      // NOTE: For debug.
      auto id = glib->localization_debug_strings()->Get(fb->name_locale())->c_str();
      checkEffects(fb->effects(), false, false);
    }
    for (auto fb : *glib->weapons()) {
      // NOTE: For debug.
      auto id = glib->localization_debug_strings()->Get(fb->name_locale())->c_str();
      checkEffects(fb->effects(), true, false, TOTAL_TIERS - fb->min_tier_index(), fb);
    }

    LAMBDA (bool, checkPlaceholder, (int locale_, const char* placeholder)) {
      const auto locale = (Loc)locale_;  // NOTE: For debug.

      int localizationIndex = -1;
      for (auto loc : *glib->localizations()) {
        localizationIndex++;

        for (auto line : *loc->broken_lines()->Get(locale)->lines()) {
          for (auto group : *line->groups()) {
            for (auto datum : *group->strings()) {
              if (datum->type() == BrokenStringDatumType_PLACEHOLDER) {
                if (!strcmp(placeholder, datum->placeholder()->c_str()))
                  return true;
              }
            }
          }
        }
      }
      return false;
    };

    for (auto fb_cond : *glib->effect_conditions()) {
      // NOTE: For debug.
      auto id = glib->localization_debug_strings()->Get(fb_cond->name_locale())->c_str();

      if (fb_cond->requires_stat()) {
        ASSERT(checkPlaceholder(fb_cond->name_locale(), "STAT"));
        ASSERT(checkPlaceholder(fb_cond->name_locale(), "MODIFIER"));
        ASSERT_FALSE(fb_cond->requires_property());
      }

      if (fb_cond->requires_stat_2()) {
        ASSERT(checkPlaceholder(fb_cond->name_locale(), "STAT_2"));
        ASSERT_FALSE(fb_cond->requires_property());
      }

      if (fb_cond->requires_property()) {
        ASSERT(checkPlaceholder(fb_cond->name_locale(), "PROPERTY"));
        ASSERT(checkPlaceholder(fb_cond->name_locale(), "MODIFIER"));
        ASSERT_FALSE(fb_cond->requires_stat());
      }

      if (fb_cond->requires_pickupable())
        ASSERT(checkPlaceholder(fb_cond->name_locale(), "PICKUPABLE"));

      const char* const letters_[]{"X", "Y", "Z", "W"};
      VIEW_FROM_ARRAY_DANGER(letters);
      FOR_RANGE (int, i, fb_cond->placeholders()->size()) {
        if (fb_cond->placeholders()->Get(i)->required()) {
          auto expectedLetter = letters[i];
          ASSERT(checkPlaceholder(fb_cond->name_locale(), expectedLetter));
        }
      }
    }

    for (auto fb : *glib->weapons()) {
      // NOTE: For debug.
      const auto id = glib->localization_debug_strings()->Get(fb->name_locale())->c_str();
      if (fb->shoots_itself())
        ASSERT(fb->projectile_type());

      if (fb->burning_times())
        ASSERT(fb->burning_damage());
      if (fb->burning_damage())
        ASSERT(fb->burning_times());
    }

    int particleIndex = -1;
    for (auto fb : *glib->particles()) {
      particleIndex++;
      // NOTE: For debug.
      const auto particleType = (ParticleType)particleIndex;
      ASSERT(fb->duration_seconds() >= fb->duration_plus_minus());
    }
  }

  // Setup. {  ///
  g.meta.trashArena         = MakeArena(128 * 1024);
  g.meta.transientDataArena = MakeArena(128 * 1024);
  g.run.arena               = MakeArena(4 * 1024);
  TEMP_USAGE(&g.meta.trashArena);

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
  // }

  ReloadFontsIfNeeded();  // TODO: remove this line?

  // Setting `g.ui.touchControlMaxLogicalOffset`.
  {  ///
    auto wBase = glib->original_texture_sizes()
                   ->Get(glib->ui_controls_touch_base_texture_id())
                   ->x();
    auto wHandle = glib->original_texture_sizes()
                     ->Get(glib->ui_controls_touch_handle_texture_id())
                     ->x();
    g.ui.touchControlMaxLogicalOffset
      = ((f32)wBase / 2 - (f32)wHandle / 2 - (f32)glib->ui_controls_touch_handle_margin())
        * ASSETS_TO_LOGICAL_RATIO;
  }

  RunInit();
}

void GameInitAfterLoadingSavedata() {
  LOGI("GameInitAfterLoadingSavedata...");
  DEFER {
    LOGI("GameInitAfterLoadingSavedata... Finished!");
  };

  // Recalculating unlocked builds, items and weapons based off achievements.
  {  ///
    g.player.achievementStepsCompleted = 0;

    // Locking all achievements.
    {
      int index = -1;
      for (auto fb : *glib->achievements()) {
        index++;

        auto fb_steps = fb->steps();
        if (!fb_steps)
          continue;

        int stepIndex = -1;
        for (auto fb_step : *fb_steps) {
          stepIndex++;
          LockInfo lock{
            .achievement = (AchievementType)index,
            .stepIndex   = stepIndex,
          };
          g.player.lockedSteps[fb_step->global_index()]          = true;
          g.player.lockedBuilds[fb_step->unlocks_build_type()]   = lock;
          g.player.lockedItems[fb_step->unlocks_item_type()]     = lock;
          g.player.lockedWeapons[fb_step->unlocks_weapon_type()] = lock;
        }
      }
    }

    int i = -1;
    for (const auto& x : g.player.achievements) {  ///
      i++;
      const auto achievementType = (AchievementType)i;

      auto fb       = glib->achievements()->Get(i);
      auto fb_steps = fb->steps();
      if (!fb_steps)
        continue;

      int stepIndex = -1;
      for (auto fb_step : *fb_steps) {
        bool locked = false;

        stepIndex++;
        if (!fb->negative_is_good() && (fb_step->value() > x.value))
          locked = true;
        if (fb->negative_is_good() && (fb_step->value() < x.value))
          locked = true;

        if (!locked)
          AchievementStepUnlock(achievementType, stepIndex, fb_step);
      }
    }

    ASSERT(g.player.achievementStepsCompleted >= 0);
    ASSERT(g.player.achievementStepsCompleted <= TOTAL_ACHIEVEMENT_STEPS);
  }

  g.run.randomTiers = GetRandomCumulativeChances(
    g.run.state.waveIndex, g.run.state.staticStats[StatType_LUCK]
  );
}

constexpr lframe GetRegenInterval(int regenLevel) {  ///
  if (regenLevel <= 0)
    return lframe::Unscaled(i64_max);
  const f32 regenPerSecond = (f32)regenLevel / 11.25f + 1.0f / 9.0f;
  return lframe::Unscaled((i64)((f32)FIXED_FPS / regenPerSecond));
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

  return MAX(1, Round(_ApplyStatPrice(StatType_REROLL_PRICE, price)));
}

void Rerolls::Roll() {  ///
  ChangeCoins(-GetPrice());
  ASSERT(PLAYER_COINS >= 0);
  if (rerolledFreeTimes < g.run.state.staticStats[StatType_FREE_REROLLS])
    rerolledFreeTimes++;
  else
    rerolledTimes++;

  lastRerolledAt = {};
  lastRerolledAt.SetNow();
}

int Rerolls::GetPrice() const {  ///
  if (rerolledFreeTimes < g.run.state.staticStats[StatType_FREE_REROLLS])
    return 0;
  return GetRerollPrice(g.run.state.waveIndex, rerolledTimes);
}

void RefillUpgradesToPick(bool rerolled) {  ///
  const auto fb_stats = glib->stats();

  int guaranteedTier = -1;

  if (!rerolled                                   //
      && (g.run.state.levelOnStartOfTheWave > 0)  //
      && (((g.run.state.levelOnStartOfTheWave + 1) % 5) == 0))
  {
    guaranteedTier = 1;
    if (g.run.state.levelOnStartOfTheWave >= 9)
      guaranteedTier = 2;
    if (g.run.state.levelOnStartOfTheWave >= 24)
      guaranteedTier = 3;
  }

  FOR_RANGE (int, i, g.run.state.upgrades.toPick.count) {
    while (1) {
      const auto newStat = (StatType)(GRAND.Rand() % fb_stats->size());

      // Can't get INVALID, LEVEL, CURSE or COINS from level up.
      if ((int)newStat <= 3)
        continue;

      const auto fb = fb_stats->Get(newStat);

      // Can't upgrade secondary stats.
      if (fb->is_secondary())
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

      int tier = guaranteedTier;
      if (tier < 0)
        tier = GetRandomTier();

      // Setting upgrade.
      g.run.state.upgrades.toPick[i] = {.stat = newStat, .tier = tier};
      break;
    }
  }
}

void RefillShopToPick() {  ///
  const auto fb_items   = glib->items();
  const auto fb_weapons = glib->weapons();

  for (auto& x : g.run.state.shop.toPick)
    x = {};

  struct {
    struct {
      ItemType   item   = {};
      WeaponType weapon = {};
    } filled[4]{};

    int filledCount = 0;

    bool Contains(ItemType item) const {
      FOR_RANGE (int, i, filledCount) {
        if (filled[i].item == item)
          return true;
      }
      return false;
    }

    bool Contains(WeaponType weapon) const {
      FOR_RANGE (int, i, filledCount) {
        if (filled[i].weapon == weapon)
          return true;
      }
      return false;
    }
  } filled{};

  Array<bool, g.run.state.shop.toPick.count> forceWeapons      = {};
  int                                        forceWeaponsCount = 0;

  IterateOverEffects(
    EffectConditionType_SHOP_SELLS_AT_LEAST__X__WEAPONS,
    -1,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA { forceWeaponsCount += EFFECT_X_INT * times; }
  );

  forceWeaponsCount = MIN(forceWeaponsCount, forceWeapons.count);

  while (forceWeaponsCount > 0) {
    auto& v = forceWeapons[GRAND.Rand() % forceWeapons.count];
    if (v)
      continue;
    v = true;
    forceWeaponsCount--;
  }

  int cardIndex = -1;
  for (auto& x : g.run.state.shop.toPick) {
    cardIndex++;
    bool setToItem = (GRAND.FRand() <= SHOP_ITEM_RATIO);
    if (forceWeapons[cardIndex])
      setToItem = false;

    x = {};

    if (setToItem) {
      do {
        x.item = GenerateRandomItem();
      } while (filled.Contains(x.item));

      filled.filled[filled.filledCount++] = {.item = x.item};

      x.tier = fb_items->Get(x.item)->tier();
    }
    else {
      while (!x.weapon) {
        const int   tier = GetRandomTier();
        const auto& pool = g.run.weaponPools[tier];
        ASSERT(pool.count > 0);

        WeaponType w{};
        do {
          w = pool[GRAND.Rand() % pool.count];
        } while (filled.Contains(w) || g.player.lockedWeapons[w].achievement);

        filled.filled[filled.filledCount++] = {.weapon = x.weapon};

        x = {.weapon = w, .tier = tier};
      }
    }
  }
}

void StableRemoveWeapon(int index) {  ///
  auto& weapon = g.run.state.weapons[index];

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
  return _AttackSpeedMultiplier(GetStatValue(StatType_ATTACK_SPEED));
}

f32 GetPlayerStatStructureAttackSpeedMultiplier() {  ///
  return _AttackSpeedMultiplier(GetStatValue(StatType_STRUCTURE_ATTACK_SPEED));
}

lframe ApplyAttackSpeedToDuration(lframe duration) {  ///
  return lframe::Unscaled(
    MAX(2, Round((f32)duration.value / GetPlayerStatAttackSpeedMultiplier()))
  );
}

lframe ApplyStructureAttackSpeedToDuration(lframe duration) {  ///
  return lframe::Unscaled(
    MAX(2, Round((f32)duration.value / GetPlayerStatStructureAttackSpeedMultiplier()))
  );
}

f32 GetExplosionDamageMultiplier() {  ///
  auto v = (f32)GetStatValue(StatType_EXPLOSION_DAMAGE);
  return MAX(0, 1.0f + v / 100.0f);
}

f32 GetExplosionSizeMultiplier() {  ///
  auto v = (f32)GetStatValue(StatType_EXPLOSION_SIZE);
  if (v >= 0)
    return 1 + v / 100.0f;
  return powf(2, v / 50);
}

// NOTE: Doesn't include melee weapon's size.
f32 CalculateWeaponRangeMeters(
  WeaponType type,
  int        tier,
  bool       affectedByGame = true
) {  ///
  const auto fb = glib->weapons()->Get(type);

  f32 rangeStat = 0;
  if (affectedByGame)
    rangeStat = (f32)GetStatValue(StatType_RANGE);

  f32 bonusRange = 0;

  if (rangeStat >= 0)
    bonusRange = rangeStat * RANGE_TO_METER_SCALE;
  else {
    bonusRange = (powf(2, rangeStat / -RANGE_GETS_HALVED_WHEN) - 1)
                 * fb->range_meters()->Get(tier - fb->min_tier_index());
  }

  // Divided by 2 because range stat is half as effective for melee weapons.
  // (don't confuse with weapons that have DamageType_MELEE).
  // TODO: better naming to differentiate
  // MELEE weapon from MELEE damage type. Ideas?
  if (!fb->projectile_type())
    bonusRange /= 2.0f;

  return fb->range_meters()->Get(tier - fb->min_tier_index()) + bonusRange;
}

int TryGetCreatureIndexByID(int id) {  ///
  int i = -1;
  for (const auto& creature : g.run.creatures) {
    i++;
    if (creature.id == id)
      return i;
  }
  return -1;
}

int GetCreatureIndexByID(int id) {  ///
  int index = TryGetCreatureIndexByID(id);
  ASSERT(index >= 0);
  return index;
}

f32 GetCreatureSpeed(const Creature& creature) {  ///
  f32 speed = creature.speed * creature.speedModifier;
  if (glib->creatures()->Get(creature.type)->hostility_type() == HostilityType_MOB) {
    speed *= 1 + (f32)g.run.dynamicStats[StatType_ENEMY_SPEED] / 100.0f;
    if (gdebug.mobsSlow)
      speed *= 0.2f;
  }
  return MAX(0, speed);
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

Vector2 CalculatePlayerProjectileBounceDirection(
  ProjectileType  type,
  Vector2         pos,
  f32             range,
  const View<int> damagedCreatureIDs
) {  ///
  ASSERT(type);

  auto fb                       = glib->projectiles()->Get(type);
  auto projectileColliderRadius = fb->collider_radius();
  auto fb_creatures             = glib->creatures();

  FOR_RANGE (int, i, 16) {
    const auto c = g.run.creatures.base + GRAND.Rand() % g.run.creatures.count;
    if (c->diedAt.IsSet())
      continue;

    if (damagedCreatureIDs.Contains(c->id))
      continue;

    auto fb_creature = fb_creatures->Get(c->type);
    if (fb_creature->hostility_type() == HostilityType_FRIENDLY)
      continue;

    const auto forecastedCreaturePos = ForecastWhereProjectileWillHitCreature(
      c->pos, c->controller.move * GetCreatureSpeed(*c), pos, fb->speed()
    );
    const auto d = Vector2DistanceSqr(forecastedCreaturePos, pos);

    if (d <= SQR(range + projectileColliderRadius + fb_creature->hurtbox_radius())) {
      return Vector2DirectionOrRandom(pos, forecastedCreaturePos);
    }
  }

  return Vector2Rotate({1, 0}, GRAND.Angle());
}

void EffectSpawnProjectilesOnHit(const Creature& creature, int weaponIndex) {  ///
  ASSERT(weaponIndex >= 0);

  const auto& weapon = g.run.state.weapons[weaponIndex];
  auto        fb     = glib->weapons()->Get(weapon.type);

  const int tierOffset = weapon.tier - fb->min_tier_index();

  IterateOverEffects(
    EffectConditionType_HIT_SPAWNS__X__PROJECTILES_DEALING__Y__DAMAGE,
    weaponIndex,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA {
      PlaySound(fb_effect->sound_hash());

      int damage = EFFECT_Y_INT * times;
      damage     = ApplyDamageScalings(damage, 0, fb_effect->damage_scalings(), times);
      damage     = ApplyPlayerStatDamageMultiplier(damage);

      const auto projectileType = (ProjectileType)fb_effect->projectile_type();
      ASSERT(projectileType);

      int piercingDamageBonusPercent = 0;
      if (fb->projectile_piercing_damage_bonus_percent())
        piercingDamageBonusPercent
          = fb->projectile_piercing_damage_bonus_percent()->Get(tierOffset);

      int pierce = g.run.dynamicStats[StatType_PIERCE];
      if (fb->projectile_pierce())
        pierce += fb->projectile_pierce()->Get(tierOffset);

      int bounce = g.run.dynamicStats[StatType_BOUNCE];
      if (fb->projectile_bounce())
        bounce += fb->projectile_bounce()->Get(tierOffset);

      const int toSpawn = EFFECT_X_INT * times;
      FOR_RANGE (int, i, toSpawn) {
        Vector2 dir{};

        if (fb_effect->spawned_projectile_shoots_directly_at_random_enemy()) {
          dir = CalculatePlayerProjectileBounceDirection(
            projectileType, creature.pos, fb_effect->projectile_range_meters(), {}
          );
        }
        else
          dir = Vector2Rotate({1, 0}, GRAND.Angle());

        MakeProjectile({
          .c{
            .type                 = projectileType,
            .ownerCreatureType    = CreatureType_PLAYER,
            .weaponIndexOrMinus1  = weaponIndex,
            .pos                  = creature.pos,
            .dir                  = dir,
            .range                = fb_effect->projectile_range_meters(),
            .damage               = damage,
            .critDamageMultiplier = fb->crit_damage_multiplier()->Get(tierOffset),
            .weaponCritChance     = fb->crit_chance()->Get(tierOffset),
            .pierce               = pierce,
            .weaponPiercingDamageBonusPercent = piercingDamageBonusPercent,
            .bounce                           = bounce,
            .canExplode                = fb_effect->spawned_projectile_can_explode(),
            .dontSpawnProjectilesOnHit = true,
          },
          .alreadyDamagedCreatureID = creature.id,
        });
      }
    }
  );
}

bool ShouldExplode(int weaponIndex) {  ///
  ASSERT(weaponIndex >= 0);
  f32 chanceToExplode = 0;

  IterateOverEffects(
    EffectConditionType_X__CHANCE_TO_EXPLODE,
    weaponIndex,
    [&](
      Weapon* w, int wi, auto fb_effect, int tierOffset, int times, int thisWaveAddedCount
    ) BF_FORCE_INLINE_LAMBDA { chanceToExplode += (f32)(EFFECT_X_INT * times) / 100.0f; }
  );
  return (GRAND.FRand() < chanceToExplode);
}

void MakeAOE(
  CreatureType damager,
  Vector2      pos,
  f32          baseRadius,
  int          baseDamage,
  f32          critDamageMultiplier,
  int          weaponCritChance,
  f32          knockbackMeters,
  int          weaponIndexOrMinus1
) {  ///
  const f32 sizeMultiplier = GetExplosionSizeMultiplier();

  const auto fb_damager = glib->creatures()->Get(damager);

  int killedCount = 0;

  int creatureIndex = -1;
  for (const auto& creature : g.run.creatures) {
    creatureIndex++;

    if (creature.diedAt.IsSet())
      continue;

    const auto fb_creature = glib->creatures()->Get(creature.type);
    if (fb_damager->hostility_type() == fb_creature->hostility_type())
      continue;

    if (Vector2DistanceSqr(creature.pos, pos)
        > SQR(baseRadius * sizeMultiplier + fb_creature->hurtbox_radius()))
      continue;

    int damage = Round((f32)baseDamage * GetExplosionDamageMultiplier());
    if (damager == CreatureType_PLAYER) {
      // Player damages mob.
      damage = ApplyPlayerStatDamageMultiplier(damage);
      damage = Round(
        damage * (1 + (f32)g.run.dynamicStats[StatType_EXPLOSION_DAMAGE] / 100.0f)
      );
    }

    auto hadHealth = (creature.health > 0);

    TryApplyDamage({
      .creatureIndex                      = creatureIndex,
      .damage                             = damage,
      .directionOrZero                    = Vector2DirectionOrRandom(pos, creature.pos),
      .knockbackMeters                    = knockbackMeters,
      .damagerCreatureType                = damager,
      .critDamageMultiplier               = critDamageMultiplier,
      .weaponCritChance                   = weaponCritChance,
      .indexOfWeaponThatDidDamageOrMinus1 = weaponIndexOrMinus1,
    });

    if ((creature.health <= 0) && hadHealth)
      killedCount++;
  }

  AchievementMax(AchievementType_EXPLODE_ENEMIES_IN_A_SINGLE_EXPLOSION, killedCount);

  MakeParticles({
    .type                   = ParticleType_EXPLOSION,
    .count                  = Ceil(SQR(baseRadius + 1) * sizeMultiplier * 3),
    .pos                    = pos,
    .velocity               = 0.4f,
    .velocityAnglePlusMinus = PI32,
    .initialOffset          = baseRadius * sizeMultiplier / 2.0f,
    .initialOffsetPlusMinus = baseRadius * sizeMultiplier / 2.0f,
    .initialOffsetEasing    = EaseOutQuart,
    .scale                  = 1.3f,
    .scalePlusMinus         = 0.15f,
  });
  PlaySound(Sound_GAME_EXPLOSION);
}

bool OnWeaponCollided(b2ShapeId shapeID, int* const weaponIndex) {  ///
  auto& weapon = g.run.state.weapons[*weaponIndex];

  const bool continueCollisions = true;
  const auto userData = ShapeUserData::FromPointer(b2Shape_GetUserData(shapeID));

  const auto  creatureIndex = GetCreatureIndexByID(userData.GetCreatureID());
  const auto& creature      = g.run.creatures[creatureIndex];

  ASSERT(creature.type);

  auto fb_creature = glib->creatures()->Get(creature.type);
  if (fb_creature->hostility_type() == HostilityType_FRIENDLY)
    return continueCollisions;

  const auto fb = glib->weapons()->Get(weapon.type);

  if (ArrayContains(weapon.piercedCreatureIDs.base, weapon.piercedCount, creature.id))
    return continueCollisions;

  const int  tierOffset = weapon.tier - fb->min_tier_index();
  const auto damage = CalculateWeaponDamage(*weaponIndex, weapon.type, weapon.tier, true);
  const auto knockbackMeters = fb->knockback_meters()->Get(tierOffset) * KNOCKBACK_SCALE;
  const auto critDamageMultiplier = fb->crit_damage_multiplier()->Get(tierOffset);
  const auto weaponCritChance     = fb->crit_chance()->Get(tierOffset);

  bool hit = false;

  if (weapon.piercedCount < weapon.piercedCreatureIDs.count) {
    weapon.piercedCreatureIDs[weapon.piercedCount++] = creature.id;

    hit = TryApplyDamage({
      .creatureIndex        = creatureIndex,
      .damage               = damage,
      .directionOrZero      = Vector2DirectionOrRandom(PLAYER_CREATURE.pos, creature.pos),
      .knockbackMeters      = knockbackMeters,
      .damagerCreatureType  = CreatureType_PLAYER,
      .critDamageMultiplier = critDamageMultiplier,
      .weaponCritChance     = weaponCritChance,
      .indexOfWeaponThatDidDamageOrMinus1 = *weaponIndex,
      .weaponSound                        = true,
    });
  }

  if (hit) {
    EffectSpawnProjectilesOnHit(creature, *weaponIndex);

    if (ShouldExplode(*weaponIndex)) {
      MakeAOE(
        CreatureType_PLAYER,
        creature.pos,
        glib->explosion_radius() * fb->explosion_size_scale(),
        damage,
        critDamageMultiplier,
        weaponCritChance,
        knockbackMeters,
        *weaponIndex
      );
    }
  }

  return continueCollisions;
}

Vector2 GetWeaponPos(int weaponIndex) {  ///
  auto& weapon = g.run.state.weapons[weaponIndex];
  ASSERT(weapon.type);
  const auto fb = glib->weapons()->Get(weapon.type);

  if (fb->projectile_type() || !weapon.startedShootingAt.IsSet())
    return PLAYER_CREATURE.pos + GetPlayerWeaponOffset(weaponIndex);

  const int tierOffset = weapon.tier - fb->min_tier_index();

  auto shootingDur = ApplyAttackSpeedToDuration(
    lframe::FromSeconds(fb->cooldown()->Get(tierOffset) * fb->attack_or_cooldown_ratio())
  );
  if (shootingDur.value > MELEE_WEAPON_MAX_ATTACK_DURATION.value)
    shootingDur.value = MELEE_WEAPON_MAX_ATTACK_DURATION.value;

  const auto e = weapon.startedShootingAt.Elapsed();
  auto       p = MIN(1, e.Progress(shootingDur));
  if (p >= 0.5) {
    p -= 0.5;
    p *= 2;
    p = EaseOutQuad(1 - p);
  }
  else
    p = EaseOutQuad(p * 2);

  const f32 movingDistance = MAX(1, CalculateWeaponRangeMeters(weapon.type, weapon.tier));
  const auto movedDistance = p * movingDistance;

  auto offset = weapon.targetDir * movedDistance;

  f32 t
    = InOutLerp(0, 1, (f32)e.value, (f32)shootingDur.value, (f32)shootingDur.value / 6);
  t      = Clamp01(t);
  offset = Vector2Lerp(GetPlayerWeaponOffset(weaponIndex), offset, t);

  return PLAYER_CREATURE.pos + offset;
}

void ClayPlaceholderFunction_STRING(const Placeholder* placeholder) {  ///
  const auto& string = placeholder->string();
  BF_CLAY_TEXT(string.value, {.color = string.color});
}

void ClayPlaceholderFunction_BROKEN_LOCALE(const Placeholder* placeholder) {  ///
  const auto& d = placeholder->brokenLocale();
  BF_CLAY_TEXT_BROKEN_LOCALIZED(d.value, {.color = d.color}, false);
}

void ClayPlaceholderFunction_IMAGE(const Placeholder* placeholder) {  ///
  const auto& d = placeholder->image();

  auto fb = glib->original_texture_sizes()->Get(d.texID);

  f32 h              = (f32)g.meta.fontUI.size;
  f32 fontScaleToFit = g.meta.fontUI._scaleToFit;
  if (g.ui.overriddenFont) {
    h              = (f32)g.ui.overriddenFont->size;
    fontScaleToFit = g.ui.overriddenFont->_scaleToFit;
  }

  f32 scale = h / ((f32)fb->y() * ASSETS_TO_LOGICAL_RATIO);
  BF_CLAY_IMAGE(
    {
      .texID = d.texID,
      .scale{scale, scale},
      .overriddenSize{0, h / fontScaleToFit},
    },
    false
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

void ApplyImmediateWeaponEffects() {  ///
  int weaponIndex = -1;
  for (const auto& weapon : g.run.state.weapons) {
    weaponIndex++;
    if (weapon.type)
      _UpdateImmediateWeaponEffects(weaponIndex, 1);
  }
}

void RemoveImmediateWeaponEffects() {  ///
  int weaponIndex = -1;
  for (const auto& weapon : g.run.state.weapons) {
    weaponIndex++;
    if (weapon.type)
      _UpdateImmediateWeaponEffects(weaponIndex, -1);
  }
}

#define SCOPED_CONTEXT(context_)                   \
  const auto _prevContext = currentContext;        \
  currentContext          = (context_);            \
  if (!draw) {                                     \
    controlsContexts[(context_)].thisFrame = true; \
    if (!controlsContexts[(context_)].prevFrame)   \
      controlsContexts[(context_)].focused = {};   \
  }                                                \
  DEFER {                                          \
    currentContext = _prevContext;                 \
  };

#define SCOPED_CONTEXT_IF(context_, enabled_)            \
  const auto _prevContext = currentContext;              \
  currentContext          = (context_);                  \
  if (!draw) {                                           \
    if (enabled_)                                        \
      controlsContexts[(context_)].thisFrame = true;     \
    controlsContexts[(context_)].disabled = !(enabled_); \
    if (!controlsContexts[(context_)].prevFrame)         \
      controlsContexts[(context_)].focused = {};         \
  }                                                      \
  DEFER {                                                \
    currentContext = _prevContext;                       \
  };

#define CURRENT_CONTEXT (controlsContexts[currentContext])

f32 ScaleDamageNumbersByProgress(f32 p) {  ///
  return Lerp(5 / 2.0f, 1, EaseOutQuad(MIN(1, p * 3)));
}

f32 GetScaleOfCoins(const FrameVisual& changedAt) {  ///
  f32 scale = 1.0f;
  if (changedAt.IsSet()) {
    const f32 s = ScaleDamageNumbersByProgress(
      MIN(1, changedAt.Elapsed().Progress(DAMAGE_NUMBERS_FRAMES))
    );
    scale += (s - 1) / 3.5f;
  }
  return scale;
}

void IterateOverBuildTextures(
  BuildType                                           build,
  /* (Vector2 anchor, Color color, int texID) */ auto innerLambda
) {  ///
  const auto texs       = glib->player_layer_texture_ids();
  const auto fb_colors  = glib->builds()->Get(build)->layer_colors();
  const auto fb_anchors = glib->player_layer_texture_anchors();

  ASSERT(texs->size() == fb_colors->size());
  ASSERT(texs->size() == fb_anchors->size());

  FOR_RANGE (int, i, texs->size()) {
    innerLambda(
      ToVector2(fb_anchors->Get(i)), ColorFromRGBA(fb_colors->Get(i)), texs->Get(i)
    );
  }
}

bool IsAchievementStepLocked(AchievementType type, int stepIndex) {  ///
  ASSERT(type);

  auto fb = glib->achievements()->Get(type);

  ASSERT(stepIndex >= 0);
  ASSERT(stepIndex < fb->steps()->size());

  auto       fb_step     = fb->steps()->Get(stepIndex);
  const auto playerValue = g.player.achievements[type].value;
  const bool isLocked    = (!fb->negative_is_good() && (playerValue < fb_step->value()))
                        || (fb->negative_is_good() && (playerValue > fb_step->value()));
  return isLocked;
}

struct BreatheColorData {  ///
  int       dur       = 3 * FIXED_FPS;
  Breathing breathing = {};
};

Color BreatheColor(Color color, BreatheColorData data) {  ///
  if (!data.breathing.set)
    return color;

  auto shownAt = g.run.shownScreenAt[g.run.state.screen];
  ASSERT(shownAt.IsSet());
  f32 p = (f32)(shownAt.Elapsed().value % data.dur) / (f32)data.dur;
  p     = (cosf(2 * PI32 * p) + 1) / 2.0f;
  p     = Lerp(p, EaseInQuad(p), 1);

  if (data.breathing.bonusBreatheAt.IsSet()) {
    p = EaseOutQuad(MAX(
      p,
      1 - data.breathing.bonusBreatheAt.Elapsed().Progress(lframe::Unscaled(data.dur / 3))
    ));
  }

  ASSERT(p >= 0);
  ASSERT(p <= 1);

  const auto v      = ColorToHSV(color);
  f32        factor = 0.75f;
  return ColorLerp(
    color, ColorFromHSV(v.x, Lerp(v.y, 0, factor / 2), Lerp(v.z, 1, factor)), p
  );
}

// NOTE: Logic must be executed only when `ge.meta._drawing` (`draw`) is false!
// e.g. updating mouse position, processing `clicked()`,
// logically reacting to `Clay_Hovered()`, changing game's state, etc.
void DoUI() {
  if ((ge.meta.screenSize.x <= 0) || (ge.meta.screenSize.y <= 0))
    return;

  ZoneScoped;

  enum UIZIndexOffset {  ///
    UIZIndexOffset_HOVER_DETAILS              = 2,
    UIZIndexOffset_STATS                      = 4,
    UIZIndexOffset_CONFIRM_MODAL              = 6,
    UIZIndexOffset_JUST_UNLOCKED_ACHIEVEMENTS = 8,
  };

  // Setup.
  // {  ///
  TEMP_USAGE(&g.meta.trashArena);
  TEMP_USAGE(&g.meta.transientDataArena);

  const auto draw = ge.meta._drawing;

  static ControlsContext lastFrameActiveControlsContext{};
  ControlsContext        currentContext{};

  static struct {
    bool           thisFrame = false;
    bool           prevFrame = false;
    Clay_ElementId focused   = {};
    bool           disabled  = false;
  } controlsContexts[ControlsContext_COUNT];

  static Direction uiElementSwitchDirectionPrevFrame = Direction_NONE;

  Direction uiElementSwitchDirection = Direction_NONE;
  if (!draw) {
    if (IsKeyPressed(SDL_SCANCODE_D) || IsKeyPressed(SDL_SCANCODE_RIGHT))
      uiElementSwitchDirection = Direction_RIGHT;
    if (IsKeyPressed(SDL_SCANCODE_W) || IsKeyPressed(SDL_SCANCODE_UP))
      uiElementSwitchDirection = Direction_UP;
    if (IsKeyPressed(SDL_SCANCODE_A) || IsKeyPressed(SDL_SCANCODE_LEFT))
      uiElementSwitchDirection = Direction_LEFT;
    if (IsKeyPressed(SDL_SCANCODE_S) || IsKeyPressed(SDL_SCANCODE_DOWN))
      uiElementSwitchDirection = Direction_DOWN;
  }

  DEFER {
    if (!draw)
      uiElementSwitchDirectionPrevFrame = uiElementSwitchDirection;
  };

  bool justFocusedDefaultControl = false;

  LAMBDA (void, markControlAsDefault, (Clay_ElementId id)) {
    if (draw)
      return;

    const bool allowed = (CURRENT_CONTEXT.thisFrame || CURRENT_CONTEXT.disabled);
    ASSERT(allowed);

    if ((uiElementSwitchDirection || g.meta.playerUsesKeyboardOrController)
        && !CURRENT_CONTEXT.focused.id  //
        && !CURRENT_CONTEXT.disabled)
    {
      CURRENT_CONTEXT.focused   = id;
      justFocusedDefaultControl = true;
    }

    if (!CURRENT_CONTEXT.prevFrame                //
        && g.meta.playerUsesKeyboardOrController  //
        && !CURRENT_CONTEXT.focused.id            //
        && !CURRENT_CONTEXT.disabled)
      CURRENT_CONTEXT.focused = id;
  };

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
  const auto fb_effectConditions    = glib->effect_conditions();
  const auto fb_creatures           = glib->creatures();
  const auto fb_damageScalings      = glib->damage_scalings();

  static int _disallowTouchNumber = {};

  LAMBDA (void, disallowTouch, ()) {
    ASSERT_FALSE(draw);
    if (GetTouchIDs().count) {
      ASSERT(ge.meta._latestActiveTouchID != InvalidTouchID);
      _disallowTouchNumber = GetTouchData(ge.meta._latestActiveTouchID).number;
    }
  };

  // Clay. Set UI dimensions + update mouse pos.
  if (!draw) {
    {
      auto res = ge.meta.scaledLogicalResolution;
      auto r   = res.x / res.y;
      if (r > UI_ASPECT_RATIO_MAX)
        res.x = res.y * UI_ASPECT_RATIO_MAX;
      else if (r < UI_ASPECT_RATIO_MIN)
        res.y = res.x / UI_ASPECT_RATIO_MIN;
      g.meta.screenSizeUI = res;
      Clay_SetLayoutDimensions({res.x, res.y});
    }

    auto pos = ScreenPosToLogical(GetMouseScreenPos());

    if (ge.events.last == LastEventType_TOUCH) {
      pos = Vector2Inf();

      if (ge.meta._latestActiveTouchID != InvalidTouchID) {
        const auto td = GetTouchData(ge.meta._latestActiveTouchID);
        if (_disallowTouchNumber != td.number)
          pos = ScreenPosToLogical(td.screenPos);
      }
    }

    g.meta.screenSizeUIMargin = {
      (g.meta.screenSizeUI.x - LOGICAL_RESOLUTION.x) / 2.0f,
      (g.meta.screenSizeUI.y - LOGICAL_RESOLUTION.y) / 2.0f,
    };

    pos = Vector2(pos.x, LOGICAL_RESOLUTION.y - pos.y) + g.meta.screenSizeUIMargin;
    Clay_SetPointerState({pos.x, pos.y}, false);
  }

  const auto localization              = glib->localizations()->Get(ge.meta.localization);
  const auto localization_strings      = localization->strings();
  const auto localization_broken_lines = localization->broken_lines();

  Clay_BeginLayout();

  auto& beautifiers      = g.ui.clayBeautifiers;
  auto& beautifiersCount = g.ui.clayBeautifiersCount;

#define GAP_FLEX (2)
#define GAP_SMALL (8)
#define GAP_BIG (20)
#define PADDING_FRAME (12)
#define PADDING_FRAME_SHADOW (94 * ASSETS_TO_LOGICAL_RATIO)
#define PADDING_OUTER_VERTICAL (10)
#define PADDING_OUTER_HORIZONTAL (12)

  int _wheel = 0;
  if (!draw) {
    _wheel = GetMouseWheel();
    if (IsKeyDown(SDL_SCANCODE_LSHIFT) || IsKeyDown(SDL_SCANCODE_RSHIFT))
      _wheel *= 10;
  }
  const int wheel = _wheel;

  auto& zIndex = g.ui.clayZIndex;

  const Color secondaryTextColor{0xef, 0xcb, 0x84, 255};

  const auto fb_slotColors = glib->ui_itemslot_colors();
  auto slotColors_ = ALLOCATE_ARRAY(&g.meta.trashArena, Color, fb_slotColors->size());
  FOR_RANGE (int, i, fb_slotColors->size())
    slotColors_[i] = ColorFromRGBA(fb_slotColors->Get(i));

  const View<Color> slotColors{.count = (int)fb_slotColors->size(), .base = slotColors_};

  constexpr int CARD_WIDTH          = 240;
  constexpr int CARD_HEIGHT         = 400;
  constexpr int UPGRADE_FRAME_WIDTH = 200;
  constexpr int ACHIEVEMENT_WIDTH   = CARD_WIDTH;

  const f32 ACHIEVEMENT_HEIGHT =                      //
    0                                                 //
    + g.meta.fontUI.size / g.meta.fontUI._scaleToFit  // Name.
    + GAP_SMALL                                       // Gap between name and description.
    + 2 * g.meta.fontStats.size / g.meta.fontUI._scaleToFit  // 2 lines of description.
    + GAP_FLEX;                                              // Gap between them.

  const auto slotSize
    = ToVector2(glib->original_texture_sizes()->Get(glib->ui_itemslot_texture_id()))
      * ASSETS_TO_LOGICAL_RATIO;
  // }

  LAMBDA (void, BF_CLAY_TEXT_LOCALIZED, (int locale, ClayTextOptions opts = {})) {  ///
    ASSERT((int)locale >= 0);
    ASSERT((int)locale < Loc_COUNT);
    auto        string = localization_strings->Get(locale);
    Clay_String text{
      .isStaticallyAllocated = true,
      .length                = (i32)string->size(),
      .chars                 = string->c_str(),
    };
    BF_CLAY_TEXT(text, opts);
  };

  LAMBDA (void, componentOverlay, (auto innerLambda, f32 fade = 1)) {  ///
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
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE * fade)
        ),
      } BF_CLAY_CUSTOM_END,
    }) {
      innerLambda();
    }
  };

  bool _alreadyHandledClickOrTouch = false;

  LAMBDA (bool, touched, (bool preventFutureDispatch = true)) {  ///
    if (draw)
      return false;
    if (_alreadyHandledClickOrTouch)
      return false;
    const bool result = !g.meta.playerUsesKeyboardOrController  //
                        && Clay_Hovered()                       //
                        && IsTouchPressed(ge.meta._latestActiveTouchID);
    if (result && preventFutureDispatch)
      _alreadyHandledClickOrTouch = true;
    return result;
  };

  LAMBDA (bool, clickOrTouchPressed, (bool preventFutureDispatch = true)) {  ///
    if (draw)
      return false;
    if (_alreadyHandledClickOrTouch)
      return false;
    const bool result
      = !g.meta.playerUsesKeyboardOrController  //
        && Clay_Hovered()                       //
        && (IsMousePressed(L) || IsTouchPressed(ge.meta._latestActiveTouchID));
    if (result && preventFutureDispatch)
      _alreadyHandledClickOrTouch = true;
    return result;
  };

  bool _alreadyHandledActivation = false;

  LAMBDA (bool, isCurrentContextActive, ()) {  ///
    return lastFrameActiveControlsContext == currentContext;
  };

  LAMBDA (bool, IsKeyPressedInCurrentContext, (SDL_Scancode key)) {  ///
    if (!isCurrentContextActive())
      return false;
    return IsKeyPressed(key);
  };

  LAMBDA (bool, didActivate, (SDL_Scancode key)) {  ///
    if (draw)
      return false;
    if (_alreadyHandledActivation)
      return false;
    bool result = IsKeyPressedInCurrentContext(key);
    if (result)
      _alreadyHandledActivation = true;
    return result;
  };

  LAMBDA (void, processShowingOrNotShowingSlotDetails, (Clay_ElementId slotID)) {  ///
    if (draw)
      return;

    if (CURRENT_CONTEXT.focused.id != slotID.id)
      return;

    if (uiElementSwitchDirectionPrevFrame)
      g.run.hideSlotDetails = false;
    else if (IsKeyPressedInCurrentContext(SDL_SCANCODE_ESCAPE) && !g.run.hideSlotDetails)
    {
      PlaySound(Sound_UI_CLICK);
      g.run.hideSlotDetails = true;
    }
    else if (IsKeyPressedInCurrentContext(SDL_SCANCODE_SPACE)) {
      PlaySound(Sound_UI_CLICK);
      g.run.hideSlotDetails = !g.run.hideSlotDetails;
    }
  };

  struct ComponentButtonData {  ///
    Clay_ElementId     id                = {};
    ControlsGroupID    group             = {};
    bool               growX             = false;
    bool               growY             = false;
    u16                paddingHorizontal = GAP_BIG;
    u16                paddingVertical   = GAP_SMALL;
    View<SDL_Scancode> keys              = {};
    int                tier              = 0;
    Breathing          breathing         = {};
    bool               hideBackground    = false;
  };

  LAMBDA (
    bool,
    componentButton,
    (ComponentButtonData                             data,
     auto /* void (bool hovered, Color textColor) */ innerLambda)
  )
  {  ///
    if (!draw)
      ASSERT(currentContext);

    ASSERT(data.id.id);
    ASSERT(data.group);
    bool result = false;

    Clay_Sizing sizing{};
    if (data.growX)
      sizing.width = CLAY_SIZING_GROW(0);
    if (data.growY)
      sizing.height = CLAY_SIZING_GROW(0);

    if (data.id.id && data.group)
      ControlsGroupAdd(data.group, data.id);

    auto& focused    = CURRENT_CONTEXT.focused;
    bool  isSelected = (focused.id && (focused.id == data.id.id));

    bool justSelected = false;

    const bool canShowAsSelected = (ge.events.last != LastEventType_TOUCH);

    CLAY({.id = data.id, .layout{.sizing{sizing}}}) {
      CLAY({
        .layout{
          BF_CLAY_SIZING_GROW_XY,
          .padding{
            .left   = data.paddingHorizontal,
            .right  = data.paddingHorizontal,
            .top    = data.paddingVertical,
            .bottom = data.paddingVertical,
          },
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        },
        BF_CLAY_CUSTOM_BEGIN{
          BF_CLAY_CUSTOM_NINE_SLICE(
            glib->ui_button_nine_slice(),
            (((!g.meta.playerUsesKeyboardOrController && Clay_Hovered() || isSelected)
              && canShowAsSelected)
               ? 6
               : data.tier),
            !data.hideBackground,
            data.breathing
          ),
        } BF_CLAY_CUSTOM_END,
      }) {
        bool hovered = !g.meta.playerUsesKeyboardOrController && Clay_Hovered();

        // MOUSE can focus buttons.
        if (hovered && (ge.events.last == LastEventType_MOUSE)) {
          if (!draw && (focused.id != data.id.id)) {
            focused      = data.id;
            justSelected = true;
          }
          isSelected = true;
        }

        CLAY({.layout{
          BF_CLAY_SIZING_GROW_XY,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          innerLambda(
            (hovered || isSelected) && canShowAsSelected,
            ((hovered || isSelected) && canShowAsSelected ? palTextBlack : palTextWhite)
          );
        }
      }

      if (justSelected)
        ButtonSFX(draw, data.id, true);

      if (!draw && isCurrentContextActive()) {
        result |= clickOrTouchPressed();
        if (result)
          disallowTouch();

        if (isSelected) {
          result |= didActivate(SDL_SCANCODE_SPACE);
          result |= didActivate(SDL_SCANCODE_RETURN);
        }
        for (auto k : data.keys)
          result |= didActivate(k);
      }
    }

    return result;
  };

  LAMBDA (
    bool,
    componentOutlinedTextButton,
    (ComponentButtonData                             data,
     auto /* void (bool hovered, Color textColor) */ innerLambda)
  )
  {  ///
    FontBegin(&g.meta.fontUIBigOutlined);
    const bool result = componentButton(data, [&](bool hovered, Color textColor) {
      innerLambda(hovered, palTextWhite);
    });
    FontEnd();
    return result;
  };

  const auto rerollID = CLAY_ID("button_reroll");

  LAMBDA (bool, componentButtonReroll, (ControlsGroupID group, int price)) {  ///
    ASSERT(price >= 0);

    SDL_Scancode keys_[]{SDL_SCANCODE_R};
    VIEW_FROM_ARRAY_DANGER(keys);

    if ((g.run.shopActivatedModalWeaponIndex >= 0) || g.meta.showingStats.IsSet())
      keys.count = 0;

    return componentButton(
      {
        .id    = rerollID,
        .group = group,
        .keys  = keys,
        .tier  = 7,
        .breathing{.set = (price <= PLAYER_COINS)},
      },
      [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
        CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
          const bool canReroll = (price <= PLAYER_COINS);
          BF_CLAY_IMAGE(
            {
              .texID = glib->ui_icon_refresh_texture_id(),
              .color = (canReroll ? WHITE : palGray),
            },
            [&]() BF_FORCE_INLINE_LAMBDA {
              if (price <= 0)
                return;

              CLAY({
                .layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER},
                .floating{
                  .zIndex = zIndex,
                  .attachPoints{
                    .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                    .parent  = CLAY_ATTACH_POINT_CENTER_BOTTOM,
                  },
                  .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                  .attachTo           = CLAY_ATTACH_TO_PARENT,
                },
              }) {
                FLOATING_BEAUTIFY;

                FontBegin(&g.meta.fontUIBigOutlined);
                BF_CLAY_TEXT(
                  TextFormat("%d", price),
                  {.color = (canReroll ? palTextWhite : palTextRed)}
                );
                BF_CLAY_IMAGE({.texID = glib->ui_coin_texture_id()});
                FontEnd();
              }
            }
          );
        }
      }
    );
  };

  struct ComponentButtonRecycleData {  ///
    Clay_ElementId  id        = {};
    ControlsGroupID group     = {};
    int             price     = {};
    Color           color     = WHITE;
    int             tier      = 0;
    Breathing       breathing = {};
  };

  LAMBDA (bool, componentButtonRecycle, (ComponentButtonRecycleData data)) {  ///
    ASSERT(data.price >= 0);
    ASSERT(data.group);
    ASSERT(data.id.id);

    return componentButton(
      {
        .id        = data.id,
        .group     = data.group,
        .tier      = data.tier,
        .breathing = data.breathing,
      },
      [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
        BF_CLAY_IMAGE(
          {.texID = glib->ui_icon_sell_texture_id(), .color = data.color},
          [&]() BF_FORCE_INLINE_LAMBDA {
            CLAY({
              .layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER},
              .floating{
                .zIndex = zIndex,
                .attachPoints{
                  .element = CLAY_ATTACH_POINT_CENTER_CENTER,
                  .parent  = CLAY_ATTACH_POINT_CENTER_BOTTOM,
                },
                .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                .attachTo           = CLAY_ATTACH_TO_PARENT,
              },
            }) {
              FLOATING_BEAUTIFY;

              FontBegin(&g.meta.fontUIBigOutlined);
              BF_CLAY_TEXT(TextFormat("%d", data.price), {.color = palTextWhite});
              BF_CLAY_IMAGE({
                .texID = glib->ui_coin_texture_id(),
                .scale = Vector2One() * 0.75f,
              });
              FontEnd();
            }
          }
        );
      }
    );
  };

  struct ComponentSlotData {  ///
    Clay_ElementId  id                      = {};
    ControlsGroupID group                   = {};
    bool            hidden                  = {};
    bool            canHover                = {};
    int             tier                    = {};
    Breathing       breathing               = {};
    f32             flashWhite              = 0;
    bool            showsDetails            = false;
    bool            disallowsTouch          = false;
    bool            touchPreservesSelection = false;
    FrameVisual     uiBouncedAt             = {};
  };

  bool touchedInsideSlot = false;

  LAMBDA (void, componentSlot, (ComponentSlotData data, auto innerLambda)) {  ///
    if (data.canHover)
      ASSERT(data.id.id);
    if (data.group)
      ControlsGroupAdd(data.group, data.id);

    const int texID        = glib->ui_itemslot_texture_id();
    auto      originalSize = original_texture_sizes->Get(texID);

    CLAY({.layout{.sizing{
      .width  = CLAY_SIZING_FIXED((f32)originalSize->x() * ASSETS_TO_LOGICAL_RATIO),
      .height = CLAY_SIZING_FIXED((f32)originalSize->y() * ASSETS_TO_LOGICAL_RATIO),
    }}})
    if (!data.hidden) {
      auto color = slotColors[2 * data.tier];
      if (draw && data.breathing.set)
        color = BreatheColor(color, {.breathing = data.breathing});

      auto flash = ColorLerp(slotColors[2 * data.tier + 1], palWhite, data.flashWhite);

      if (data.uiBouncedAt.IsSet()) {
        f32 p = data.uiBouncedAt.Elapsed().Progress(ANIMATION_2_FRAMES);
        p     = EaseOutQuad(MIN(1, p));
        color = ColorLerp(palWhite, color, p);
        flash = ColorLerp(palWhite, flash, p);
      }

      const bool hovered
        = data.canHover && Clay_Hovered() && !g.meta.playerUsesKeyboardOrController;

      auto& focused = controlsContexts[currentContext].focused;

      if (hovered && !draw) {
        if (ge.events.thisFrame.Mouse())
          g.run.hideSlotDetails = false;

        bool justTouched = touched(false);
        if (justTouched)
          touchedInsideSlot = true;
        bool justHidDetailsViaTouch = false;

        if (justTouched && data.showsDetails) {
          PlaySound(Sound_UI_CLICK);

          if (data.showsDetails)
            g.run.hideSlotDetails = !g.run.hideSlotDetails;

          if (!g.run.hideSlotDetails && !data.showsDetails) {
            focused                = {};
            justHidDetailsViaTouch = true;
          }
        }
        else if (justTouched && data.disallowsTouch && (focused.id == data.id.id))
          disallowTouch();

        if (focused.id != data.id.id) {
          if (!justHidDetailsViaTouch) {
            focused               = data.id;
            g.run.hideSlotDetails = false;
          }

          // Making it not possible to simultaneously (1) select and (2) activate slot
          // upon reacting to single touch press event.
          _alreadyHandledClickOrTouch = true;
          _alreadyHandledActivation   = true;
        }
      }

      bool showAsSelected = true;
      if (ge.events.last == LastEventType_TOUCH)
        showAsSelected = !g.run.hideSlotDetails;

      if (focused.id                     //
          && (focused.id == data.id.id)  //
          && (showAsSelected || data.touchPreservesSelection))
      {
        color = palWhite;
        flash = TRANSPARENT_BLACK;
      }

      BF_CLAY_IMAGE({.texID = texID, .color = color, .flash = flash}, innerLambda);
    }
  };

  // Closing slot details if touched outside of it.
  DEFER {                                                ///
    if (!draw                                            //
        && IsTouchPressed(ge.meta._latestActiveTouchID)  //
        && !touchedInsideSlot                            //
        && !g.run.hideSlotDetails)
    {
      PlaySound(Sound_UI_CLICK);
      g.run.hideSlotDetails = true;
    }
  };

  enum HiddenType {  ///
    HiddenType_HIDE_IF_EMPTY,
    HiddenType_SHOW_EMPTY_SLOT,
    HiddenType_SHOW_LOCK,
  };

  struct ComponentUniversalSlotData {  ///
    Clay_ElementId  id    = {};
    ControlsGroupID group = {};

    DifficultyType difficulty  = {};
    BuildType      build       = {};
    ItemType       item        = {};
    WeaponType     weapon      = {};
    int            weaponIndex = -1;

    HiddenType hidden = HiddenType_HIDE_IF_EMPTY;

    int       count     = 1;
    int       tier      = -1;
    bool      canHover  = true;
    Breathing breathing = {};

    bool showsDetails            = false;
    bool disallowsTouch          = false;
    bool touchPreservesSelection = false;

    FrameVisual uiBouncedAt = {};

    bool   unlocking     = false;
    lframe unlockElapsed = {};
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

  LAMBDA (bool, componentUniversalSlot, (ComponentUniversalSlotData data)) {  ///
    ASSERT(data.count > 0);
    if (data.canHover)
      ASSERT(data.id.id);

    // Checking that none or only one of build/item/weapon is specified.
    const int onlyOneOrNone = (int)(data.difficulty != 0) + (int)(data.build != 0)
                              + (int)(data.item != 0) + (int)(data.weapon != 0);
    ASSERT(onlyOneOrNone <= 1);

    int texID = 0;
    int tier  = 0;
    if (data.difficulty) {
      texID = fb_difficulties->Get(data.difficulty)->texture_id();
      tier  = (int)data.difficulty - 1;
    }
    if (data.build)
      tier = GetBuildTier(data.build);
    if (data.item) {
      auto fbx = fb_items->Get(data.item);
      texID    = fbx->texture_id();
      tier     = fbx->tier();
    }
    if (data.weapon) {
      auto fbx = fb_weapons->Get(data.weapon);
      texID    = fbx->icon_texture_id();
      tier     = fbx->min_tier_index();
    }
    if (data.tier >= 0)
      tier = data.tier;

    f32        unlockFadeLock             = 0;
    f32        unlockFadeItem             = 1;
    f32        unlockFlashWhiteLock       = 0;
    f32        unlockFlashWhiteItem       = 0;
    const auto unlockDelayBeforeUnlocking = ANIMATION_2_FRAMES;
    const auto unlockDuration             = ANIMATION_0_FRAMES;

    if (data.unlocking) {
      if (data.unlockElapsed > unlockDelayBeforeUnlocking + unlockDuration) {
        // Item is shown clearly.
      }
      else if (data.unlockElapsed > unlockDelayBeforeUnlocking) {
        // Unlock animation.
        f32 p = Clamp01(
          (data.unlockElapsed - unlockDelayBeforeUnlocking).Progress(unlockDuration)
        );
        unlockFadeItem       = Clamp01(p);
        unlockFadeLock       = Clamp01(1 - p);
        unlockFlashWhiteLock = Clamp01(1 - SQR(SQR(cosf(PI32 * p))));
        unlockFlashWhiteItem = Clamp01(1 - SQR(SQR(cosf(PI32 * p))));
      }
      else {
        unlockFadeLock = 1;
        unlockFadeItem = 0;
      }
    }

    bool result = false;

    componentSlot(
      {
        .id             = data.id,
        .group          = data.group,
        .hidden         = (!onlyOneOrNone && (data.hidden == HiddenType_HIDE_IF_EMPTY)),
        .canHover       = data.canHover,
        .tier           = tier,
        .breathing      = data.breathing,
        .flashWhite     = MAX(unlockFlashWhiteLock, unlockFlashWhiteItem),
        .showsDetails   = data.showsDetails,
        .disallowsTouch = data.disallowsTouch,
        .touchPreservesSelection = data.touchPreservesSelection,
        .uiBouncedAt             = data.uiBouncedAt,
      },
      [&]() BF_FORCE_INLINE_LAMBDA {
        // Cheat. Opening changing weapon modal.
        if (!draw  //
            && isCurrentContextActive()  //
            && (ge.meta.debugEnabled || BF_DEBUG)  //
            && (data.weaponIndex >= 0)  //
            && (
              IsKeyDown(SDL_SCANCODE_LSHIFT) //
              || IsKeyDown(SDL_SCANCODE_RSHIFT)
              || IsKeyDown(SDL_SCANCODE_LCTRL)
              || IsKeyDown(SDL_SCANCODE_RCTRL)
            )
            && clickOrTouchPressed())
          g.run.cheatWeaponIndex = data.weaponIndex;

        if (!onlyOneOrNone) {
          if (data.hidden == HiddenType_SHOW_LOCK)
            texID = glib->ui_item_locked_texture_id();
          else
            return;
        }

        if (data.canHover) {
          ButtonSFX(
            draw, data.id, !g.meta.playerUsesKeyboardOrController && Clay_Hovered()
          );

          if (isCurrentContextActive()) {
            result |= clickOrTouchPressed();

            const auto& focused = controlsContexts[currentContext].focused;
            if (focused.id && (focused.id == data.id.id)) {
              result |= didActivate(SDL_SCANCODE_SPACE);
              result |= didActivate(SDL_SCANCODE_RETURN);
            }
          }
        }

        // Cheat. Changing hovered player's tier / weapon's type using mouse wheel.
        if ((ge.meta.debugEnabled || BF_DEBUG)  //
            && (data.weaponIndex >= 0)          //
            && Clay_Hovered()                   //
            && wheel)
        {
          auto& w = g.run.state.weapons[data.weaponIndex];
          auto& t = w.type;

          if ((IsKeyDown(SDL_SCANCODE_LCTRL) || IsKeyDown(SDL_SCANCODE_RCTRL)))
            t = (WeaponType)(MIN((int)WeaponType_COUNT - 1, MAX(1, (int)t - wheel)));
          else
            w.tier += wheel;

          auto fb = fb_weapons->Get(t);

          if (w.tier < fb->min_tier_index())
            w.tier = fb->min_tier_index();
          else if (w.tier > TOTAL_TIERS - 1)
            w.tier = TOTAL_TIERS - 1;

          Save();
        }

        CLAY({.layout{BF_CLAY_SIZING_GROW_XY, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
          if (data.unlocking && ((unlockFadeLock > 0) || (unlockFlashWhiteLock > 0))) {
            componentCenterFloater([&]() BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_IMAGE({
                .texID = glib->ui_item_locked_texture_id(),
                .color = Fade(WHITE, Clamp01(unlockFadeLock)),
                .flash = Fade(WHITE, Clamp01(unlockFlashWhiteLock)),
              });
            });
          }

          if (data.build) {
            CLAY({.layout{BF_CLAY_SIZING_GROW_XY}}) {
              LAMBDA (void, oneMoreFloatingImageOfBuild, (auto innerLambda)) {
                CLAY({.floating{
                  .zIndex = zIndex,
                  .attachPoints{
                    .element = CLAY_ATTACH_POINT_CENTER_BOTTOM,
                    .parent  = CLAY_ATTACH_POINT_CENTER_BOTTOM,
                  },
                  .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                  .attachTo           = CLAY_ATTACH_TO_PARENT,
                }}) {
                  innerLambda();
                }
              };

              const auto fb_creaturePlayer = fb_creatures->Get(CreatureType_PLAYER);

              f32 off = 2.0;

              IterateOverBuildTextures(
                data.build,
                [&](Vector2 anchor, Color color, int texID) BF_FORCE_INLINE_LAMBDA {
                  oneMoreFloatingImageOfBuild([&]() BF_FORCE_INLINE_LAMBDA {
                    const auto sy = (f32)glib->original_texture_sizes()->Get(texID)->y();
                    BF_CLAY_IMAGE({
                      .texID = texID,
                      .offset{off, -sy * ASSETS_TO_LOGICAL_RATIO / 2.0f},
                      .anchor = anchor,
                      .color  = Fade(color, Clamp01(unlockFadeItem)),
                      .flash  = Fade(WHITE, Clamp01(unlockFlashWhiteItem)),
                    });
                  });
                }
              );

              const auto fb_build = fb_builds->Get(data.build);
              const auto fb_hat   = glib->hats()->Get(fb_build->hat_type());

              oneMoreFloatingImageOfBuild([&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_IMAGE({
                  .texID = fb_hat->texture_id(),
                  .offset{off, 100.0f * 179 / 256},
                  .anchor = ToVector2(fb_hat->anchor()) * Vector2(1, -1),
                  .color
                  = Fade(ColorFromRGBA(fb_build->hat_color()), Clamp01(unlockFadeItem)),
                  .flash = Fade(WHITE, Clamp01(unlockFlashWhiteItem)),
                });
              });
            }
          }
          else {
            BF_CLAY_IMAGE({
              .texID = texID,
              .scale = Vector2One() * GetScaleOfCoins(data.uiBouncedAt),
              .color = Fade(WHITE, Clamp01(unlockFadeItem)),
              .flash = Fade(WHITE, Clamp01(unlockFlashWhiteItem)),
              .dontCareAboutScaleWhenCalculatingSize = true,
            });
          }

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

  LAMBDA (
    void,
    componentEffectsExploded,
    (auto fb_effects, int tierOffset, int times, int thisWaveAddedCount, int maxWidth)
  )
  {  ///
    ASSERT(times > 0);
    ASSERT(tierOffset >= 0);
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

        if (fb_effect->only_this_weapon()       //
            || fb_effect->only_other_weapons()  //
            || fb_effect->all_weapons())
        {
          // auto color = secondaryTextColor;
          auto color = palTextWhite;
          // if (fb_effect->only_this_weapon()) {
          //   BF_CLAY_TEXT_BROKEN_LOCALIZED(Loc_UI_ONLY_THIS_WEAPON, {.color = color});
          //   BF_CLAY_TEXT(": ", {.color = color});
          // }
          if (fb_effect->only_other_weapons()) {
            BF_CLAY_TEXT_BROKEN_LOCALIZED(Loc_UI_ONLY_OTHER_WEAPONS, {.color = color});
            BF_CLAY_TEXT(": ", {.color = color});
          }
          if (fb_effect->all_weapons()) {
            BF_CLAY_TEXT_BROKEN_LOCALIZED(Loc_UI_ALL_WEAPONS, {.color = color});
            BF_CLAY_TEXT(": ", {.color = color});
          }
        }

        const auto cond                = fb_effect->effectcondition_type();
        auto       fb_cond             = fb_effectConditions->Get(cond);
        auto       fb_condPlaceholders = fb_cond->placeholders();

        if (fb_effect->stat_type()) {
          const auto fb_stat = fb_stats->Get(fb_effect->stat_type());

          PlaceholdGroupBegin("STAT");

          PlaceholdBrokenLocale(fb_stat->name_locale());

          PlaceholdGroupEnd();

          bool isPercent = fb_stat->is_percent();
          auto value     = fb_effect->value();
          if (value)
            v = (f32)value->Get(tierOffset);
          else {
            v         = (fb_effect->value_multiplier()->Get(tierOffset) - 1.0f) * 100.0f;
            isPercent = true;
          }

          if (fb_effect->modification_scaleable()) {
            v = Round(
              (f32)v * GetStatModificationScale((StatType)fb_effect->stat_type())
            );
          }

          if (fb_cond->stat_multiplied_by_times())
            v *= times;
          if (fb_cond->stat_multiplied_by_this_wave_added_count())
            v *= thisWaveAddedCount;

          const bool  isPositive = v >= 0;
          const char* format     = nullptr;
          if (isPercent)
            format = (isPositive ? "+%s%%" : "%s%%");
          else
            format = (isPositive ? "+%s" : "%s");

          PlaceholdGroupBegin("MODIFIER");

          PlaceholdFormattedString(
            TextFormat(format, StripLeadingZerosInFloat(TextFormat("%.1f", v))),
            ((isPositive == fb_stat->negative_is_good()) ? palTextRed : palTextGreen)
          );

          if (fb_stat->small_icon_texture_id()) {
            PlaceholdString(" ");
            PlaceholdImage(fb_stat->small_icon_texture_id());
          }

          PlaceholdGroupEnd();
        }

        if (fb_effect->stat_type_2()) {
          const auto fb_stat = fb_stats->Get(fb_effect->stat_type_2());
          PlaceholdGroupBegin("STAT_2");
          PlaceholdImage(fb_stat->small_icon_texture_id());
          PlaceholdString(" ");
          PlaceholdBrokenLocale(fb_stat->name_locale());
          PlaceholdGroupEnd();
        }

        if (fb_effect->weaponproperty_type()) {
          const auto fb_prop
            = fb_weapon_properties->Get(fb_effect->weaponproperty_type());
          PlaceholdBrokenLocale("PROPERTY", fb_prop->name_locale());

          bool isPercent = false;
          auto value     = fb_effect->value();
          if (value)
            v = (f32)value->Get(tierOffset);
          else {
            v         = (fb_effect->value_multiplier()->Get(tierOffset) - 1.0f) * 100.0f;
            isPercent = true;
          }
          v *= times;

          // TODO: correct colors.
          const char* format     = nullptr;
          bool        isPositive = (v >= 0);
          if (isPercent)
            format = (isPositive ? "+%s%%" : "%s%%");
          else
            format = (isPositive ? "+%s" : "%s");

          PlaceholdString(
            "MODIFIER",
            PushTextToArena(
              &g.meta.trashArena,
              TextFormat(format, StripLeadingZerosInFloat(TextFormat("%.1f", v)))
            ),
            palTextGreen
          );
        }

        if (fb_effect->pickupable_type()) {
          auto fb_pickupable = fb_pickupables->Get(fb_effect->pickupable_type());
          PlaceholdBrokenLocale("PICKUPABLE", fb_pickupable->name_locale(), palTextGreen);
        }

        if (fb_condPlaceholders) {
          char index = -1;
          for (auto fb_placeholder : *fb_condPlaceholders) {
            index++;

            if (!fb_placeholder->required())
              continue;

            auto fb_vals = fb_effect->placeholders()->Get(index);

            char placeholderName[2]{'X', '\0'};
            placeholderName[0] += index;

            PlaceholdGroupBegin(PushTextToArena(&g.meta.trashArena, placeholderName));

            switch ((CondVarType)fb_placeholder->condvar_type()) {
            case CondVarType_INTEGER: {
              auto cv = fb_vals->ints()->Get(tierOffset);

              if (fb_placeholder->multiplied_by_times())
                cv *= times;
              if (fb_placeholder->divided_by_times())
                cv /= times;

              auto formatFunc = FormatInt;
              if (fb_placeholder->signed_())
                formatFunc = FormatSignedInt;

              auto formatted = formatFunc(cv);
              if (fb_placeholder->is_percent())
                formatted = TextFormat("%s%%", formatted);

              auto color = palTextGreen;

              if (fb_placeholder->color_by_stat()) {
                bool green = fb_stats->Get(fb_effect->stat_type())->negative_is_good()
                             != (fb_effect->value()->Get(tierOffset) >= 0);
                if (!green)
                  color = palTextRed;
              }
              else if (fb_placeholder->color_by_stat_2()) {
                bool green = fb_stats->Get(fb_effect->stat_type_2())->negative_is_good()
                             != (cv >= 0);
                if (!green)
                  color = palTextRed;
              }

              PlaceholdFormattedString(formatted, color);
            } break;

            case CondVarType_FLOAT: {
              auto cv = fb_vals->floats()->Get(tierOffset);

              if (fb_placeholder->multiplied_by_times())
                cv *= times;
              if (fb_placeholder->divided_by_times())
                cv /= times;

              auto formatFunc = FormatFloatDot1WithoutLeadingZeros;
              if (fb_placeholder->signed_())
                formatFunc = FormatSignedFloatDot1WithoutLeadingZeros;

              auto formatted = formatFunc(cv);
              if (fb_placeholder->is_percent())
                formatted = TextFormat("%s%%", formatted);

              PlaceholdFormattedString(formatted);
            } break;

            case CondVarType_DAMAGE: {
              int baseDamage = fb_vals->ints()->Get(tierOffset);
              ASSERT(baseDamage >= 0);

              if (fb_placeholder->multiplied_by_times())
                baseDamage *= times;
              if (fb_placeholder->divided_by_times())
                baseDamage /= times;

              auto scalings = fb_effect->damage_scalings();

              int totalScalings = 0;
              if (scalings) {
                for (const int s : FBFlattened{scalings}.Iter()) {
                  const int v
                    = fb_damageScalings->Get(s)->percents_per_tier()->Get(tierOffset);
                  if (v)
                    totalScalings++;
                }
              }

              if (baseDamage) {
                PlaceholdFormattedString(FormatInt(baseDamage));

                if (totalScalings) {
                  PlaceholdString(" ");
                  PlaceholdString("+ ");
                }
              }

              if (scalings) {
                for (auto s : FBFlattened{scalings}.Iter()) {
                  auto fb_scaling = fb_damageScalings->Get(s);

                  int v = fb_scaling->percents_per_tier()->Get(tierOffset);
                  if (fb_placeholder->multiplied_by_times())
                    v *= times;
                  if (fb_placeholder->divided_by_times())
                    v /= times;

                  if (!v)
                    continue;

                  PlaceholdFormattedString(TextFormat("%d%%", v));
                  PlaceholdImage(
                    fb_stats->Get(fb_scaling->stat_type())->small_icon_texture_id()
                  );
                  totalScalings--;

                  if (totalScalings > 0) {
                    PlaceholdString(" ");
                    PlaceholdString("+ ");
                  }
                }
              }
            } break;

            case CondVarType_ITEM_OR_WEAPON: {
              auto item   = (ItemType)fb_effect->item_type();
              auto weapon = (WeaponType)fb_effect->weapon_type();
              int  locale = 0;
              if (item)
                locale = fb_items->Get(item)->name_locale();
              else if (weapon)
                locale = fb_weapons->Get(weapon)->name_locale();
              else
                INVALID_PATH;

              PlaceholdBrokenLocale(locale, palTextGreen);
            } break;

            default:
              INVALID_PATH;
              break;
            }

            PlaceholdGroupEnd();
          }
        }

        BF_CLAY_TEXT_BROKEN_LOCALIZED(fb_cond->name_locale());

        FlexEnd();
      }
    }
  };

  LAMBDA (void, ResetFocus, (ControlsContext c)) {  ///
    controlsContexts[c].focused = {};
  };

  LAMBDA (void, ChangeFocus, (ControlsContext c, Clay_ElementId to)) {  ///
    auto& context = controlsContexts[c];
    if (g.meta.playerUsesKeyboardOrController && to.id)
      context.focused = to;
  };

  LAMBDA (
    void, ChangeFocusFrom, (ControlsContext c, Clay_ElementId from, Clay_ElementId to)
  )
  {  ///
    auto& context = controlsContexts[c];
    if ((context.focused.id == from.id) && g.meta.playerUsesKeyboardOrController && to.id)
      context.focused = to;
  };

  LAMBDA (
    void,
    componentAchievementDescription,
    (AchievementType type,
     int             stepIndex,
     bool            requiresPreviousToBeCompletedToShowDetails = true)
  )
  {  ///
    ASSERT(type);
    ASSERT(stepIndex >= 0);

    const auto v = g.player.achievements[type].value;

    auto fb              = fb_achievements->Get(type);
    auto fb_step         = fb->steps()->Get(stepIndex);
    auto fb_previousStep = ((stepIndex > 0) ? fb->steps()->Get(stepIndex - 1) : nullptr);

    if (!requiresPreviousToBeCompletedToShowDetails
        || (!fb_previousStep || (!fb->negative_is_good() &&( v >= fb_previousStep->value())||fb->negative_is_good() &&( v <= fb_previousStep->value()))))
    {
      FlexBegin(ACHIEVEMENT_WIDTH, 0);
      if (type == AchievementType_DIFFICULTY) {
        PlaceholdString(
          "VALUE",
          localization_strings->Get(fb_difficulties->Get(stepIndex + 1)->name_locale())
            ->c_str()
        );
      }
      else {
        PlaceholdString(
          "VALUE", PushTextToArena(&g.meta.trashArena, TextFormat("%d", fb_step->value()))
        );
      }

      if (fb->stat_type()) {
        PlaceholdGroupBegin("STAT");
        auto fb_stat = fb_stats->Get(fb->stat_type());
        if (fb_stat->small_icon_texture_id()) {
          PlaceholdImage(fb_stat->small_icon_texture_id());
          PlaceholdString(" ");
        }
        PlaceholdBrokenLocale(fb_stat->name_locale());
        PlaceholdGroupEnd();
      }

      int description = fb->description_locale();

      if (fb->build_type()) {
        if (g.player.lockedBuilds[fb->build_type()].achievement)
          description = 0;
        else {
          PlaceholdBrokenLocale(
            "BUILD", fb_builds->Get(fb->build_type())->name_locale(), palTextGreen
          );
        }
      }

      if (description)
        BF_CLAY_TEXT_BROKEN_LOCALIZED(fb->description_locale());
      else
        BF_CLAY_TEXT("???");

      if (!fb->hide_progress()) {
        int percent = MIN(100, v * 100 / fb_step->value());
        if ((percent < 100) && (fb_step->value() != 1)) {
          BF_CLAY_TEXT(" ");
          BF_CLAY_TEXT(
            TextFormat("(%d / %d)", (int)v, (int)fb_step->value()),
            {
              .color    = palTextBezhevy,
              .wrapMode = CLAY_TEXT_WRAP_NONE,
            }
          );
        }
      }

      FlexEnd();
    }
    else
      BF_CLAY_TEXT("???");
  };

  struct ComponentUniversalCardData {  ///
    DifficultyType difficulty = {};
    BuildType      build      = {};
    ItemType       item       = {};
    WeaponType     weapon     = {};

    int count               = 1;
    int thisWaveAddedCount  = 0;
    int weaponIndexOrMinus1 = -1;

    HiddenType hidden         = HiddenType_SHOW_EMPTY_SLOT;
    LockInfo   lockInfo       = {};
    bool       affectedByGame = false;
    int        overrideTier   = -1;

    bool setFixedHeight = false;

    ControlsGroupID shopGroup                  = {};
    int             shopBuyingIndex            = -1;
    bool            shopDetailsContextIsActive = false;
    bool            shopSelling                = false;
    Clay_ElementId  shopFocusAfterBuying       = {};
    Clay_ElementId  shopFocusAfterRecycling    = {};
    bool            shadow                     = false;
  };

  const auto groupDetails = MakeControlsGroup();

  LAMBDA (Clay_ElementId, getIDFromShopBuyingIndex, (int index)) {  ///
    return CLAY_IDI("button_shop_buy", index);
  };

  LAMBDA (Clay_ElementId, getIDFromPlayerWeaponIndex, (int i)) {  ///
    return CLAY_IDI("player_weapon", i);
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

    f32 price_       = 0;
    int recyclePrice = 0;
    if (fb_weapon) {
      price_
        = CalculateItemOrWeaponPrice(fb_weapon->price(), tier, ItemOrWeaponType_WEAPON);
      recyclePrice = ToRecyclePrice(price_);
    }
    if (fb_item) {
      price_ = CalculateItemOrWeaponPrice(
        fb_item->price(), fb_item->tier(), ItemOrWeaponType_ITEM
      );
      recyclePrice = ToRecyclePrice(price_);
    }
    const int price = MAX(1, Round(_ApplyStatPrice(StatType_SHOP_PRICE, price_)));

    bool canBuy                     = (price <= PLAYER_COINS);
    bool canBuyErrorWeapon          = false;
    bool canBuyErrorBuild           = false;
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
    if (data.weapon) {
      auto fb        = fb_weapons->Get(data.weapon);
      auto is_ranged = fb->projectile_type();

      if (is_ranged) {
        IterateOverEffects(
          EffectConditionType_CANT_EQUIP_RANGED_WEAPONS,
          -1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA { canBuyErrorBuild = true; }
        );
      }
      else {
        IterateOverEffects(
          EffectConditionType_CANT_EQUIP_MELEE_WEAPONS,
          -1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA { canBuyErrorBuild = true; }
        );
      }

      if (canBuyErrorBuild)
        canBuy = false;
    }
    // }  ///

    CLAY(  ///
      {.layout{.sizing{
        .width = CLAY_SIZING_FIXED(CARD_WIDTH + 2 * PADDING_FRAME),
        .height
        = (data.setFixedHeight ? CLAY_SIZING_FIXED(CARD_HEIGHT) : CLAY_SIZING_FIT(0)),
      }}}
    )
    if ((data.hidden != HiddenType_HIDE_IF_EMPTY) || atLeastOneIsSpecified) {
      CLAY(
        ///
        {
          .layout{
            BF_CLAY_SIZING_GROW_XY,
            BF_CLAY_PADDING_ALL(PADDING_FRAME),
            .childGap        = GAP_SMALL,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
          BF_CLAY_CUSTOM_BEGIN{
            BF_CLAY_CUSTOM_SHADOW(glib->ui_frame_shadow_big_nine_slice(), data.shadow),
            BF_CLAY_CUSTOM_NINE_SLICE(
              glib->ui_frame_nine_slice(), tier, true, {.set = false}
            ),
          } BF_CLAY_CUSTOM_END,
        }
      ) {
        // Icon, name.
        CLAY({.layout{.childGap = GAP_SMALL}}) {
          // Icon. {  ///
          componentUniversalSlot({
            .difficulty = data.difficulty,
            .build      = data.build,
            .item       = data.item,
            .weapon     = data.weapon,
            .hidden     = data.hidden,
            .count      = data.count,
            .tier       = tier,
            .canHover   = false,
          });
          // }

          // Name, secondary stuff.
          CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            if (data.difficulty) {  ///
              BF_CLAY_TEXT_LOCALIZED(
                fb_difficulty->name_locale(), {.color = textColorsPerTier[tier]}
              );

              FontBegin(&g.meta.fontStats);
              BF_CLAY_TEXT_LOCALIZED(Loc_UI_DIFFICULTY, {.color = secondaryTextColor});
              FontEnd();
            }
            else if (data.build) {  ///
              BF_CLAY_TEXT_LOCALIZED(
                fb_build->name_locale(), {.color = textColorsPerTier[tier]}
              );
              FontBegin(&g.meta.fontStats);
              const auto d = g.player.builds[data.build].maxDifficultyBeaten;
              if (d > 0) {
                CLAY({}) {
                  auto dd = fb_difficulties->Get(d);
                  BF_CLAY_TEXT_LOCALIZED(
                    dd->name_locale(), {.color = secondaryTextColor}
                  );
                }
              }
              else
                BF_CLAY_TEXT_LOCALIZED(Loc_UI_BUILD, {.color = secondaryTextColor});
              FontEnd();
            }
            else if (data.item) {  ///
              BF_CLAY_TEXT_LOCALIZED(
                fb_item->name_locale(), {.color = textColorsPerTier[tier]}
              );

              FontBegin(&g.meta.fontStats);
              if (fb_item->limit() > 0) {
                if (fb_item->limit() > 1) {
                  CLAY({}) {
                    BF_CLAY_TEXT_LOCALIZED(Loc_UI_LIMITED, {.color = secondaryTextColor});

                    if (data.affectedByGame) {
                      int currentCount = 0;
                      for (auto& x : g.run.state.items) {
                        if (x.type == data.item) {
                          currentCount = x.count;
                          break;
                        }
                      }
                      BF_CLAY_TEXT(
                        TextFormat(" (%d/%d)", currentCount, fb_item->limit()),
                        {.color = secondaryTextColor}
                      );
                    }
                    else {
                      BF_CLAY_TEXT(
                        TextFormat(" (%d)", fb_item->limit()),
                        {.color = secondaryTextColor}
                      );
                    }
                  }
                }
                else
                  BF_CLAY_TEXT_LOCALIZED(Loc_UI_UNIQUE, {.color = secondaryTextColor});
              }
              else
                BF_CLAY_TEXT_LOCALIZED(Loc_UI_ITEM, {.color = secondaryTextColor});
              FontEnd();
            }
            else if (data.weapon) {  ///
              BF_CLAY_TEXT_LOCALIZED(
                fb_weapon->name_locale(), {.color = textColorsPerTier[tier]}
              );
              FontBegin(&g.meta.fontStats);
              BF_CLAY_TEXT_LOCALIZED(Loc_UI_WEAPON, {.color = secondaryTextColor});
              FontEnd();
            }
            // else {  ///
            //   BF_CLAY_TEXT("???");
            // }
          }
        }

        FontBegin(&g.meta.fontStats);

        // Difficulty stats.
        if (data.difficulty) {  ///
          componentEffectsExploded(
            fb_difficulty->effects(), 0, data.count, 0, CARD_WIDTH
          );
        }
        // Build stats.
        else if (data.build) {  ///
          componentEffectsExploded(fb_build->effects(), 0, data.count, 0, CARD_WIDTH);
        }
        // Item stats.
        else if (data.item) {  ///
          componentEffectsExploded(
            fb_items->Get(data.item)->effects(),
            0,
            data.count,
            (data.affectedByGame ? data.thisWaveAddedCount : 1),
            CARD_WIDTH
          );
        }
        // Weapon stats.
        else if (data.weapon) {  ///
          const auto fb = fb_weapons->Get(data.weapon);

          int tierOffset = 0;
          if (data.overrideTier >= 0)
            tierOffset = data.overrideTier - fb->min_tier_index();
          ASSERT(tierOffset >= 0);

          int thisWaveDamage = 0;
          if (data.weaponIndexOrMinus1 >= 0) {
            const auto& weapon = g.run.state.weapons[data.weaponIndexOrMinus1];
            tierOffset         = weapon.tier - fb->min_tier_index();
            ASSERT(tierOffset >= 0);
            thisWaveDamage = weapon.thisWaveDamage;
          }

          LAMBDA (void, componentWeaponStatEntry, (int labelLocale, auto&& innerLambda)) {
            CLAY({.layout{
              .childGap = GAP_FLEX,
              BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              FlexBegin(CARD_WIDTH, 0);
              BF_CLAY_TEXT_BROKEN_LOCALIZED(labelLocale, {.color = secondaryTextColor});
              BF_CLAY_TEXT(": ", {.color = secondaryTextColor});
              innerLambda();
              FlexEnd();
            }
          };

          LAMBDA (
            void,
            componentWeaponDamageWithScalings,
            (int         locale,
             int         baseDamage,
             int         actualDamage,
             int         times,
             FBFlattened damageScalings,
             int         tierOffset)
          )
          {
            CLAY({.layout{
              .childGap = GAP_FLEX,
              BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              FlexBegin(CARD_WIDTH, 0);

              PlaceholdGroupBegin("VALUE");

              LAMBDA (void, showPelletsCount, (Color color)) {
                if (times > 1)
                  PlaceholdFormattedString(TextFormat("x%d", times), color);
              };

              if (data.affectedByGame) {
                PlaceholdFormattedString(TextFormat("%d", actualDamage));
                showPelletsCount(palTextGreen);

                PlaceholdString(" ");
                PlaceholdString("| ", palTextWhite);
              }

              PlaceholdFormattedString(TextFormat("%d", baseDamage), palTextWhite);
              showPelletsCount(palTextWhite);

              for (const int scalingIndex : FBFlattened{damageScalings}.Iter()) {
                PlaceholdString(" ");
                PlaceholdString("+ ", palTextWhite);

                const auto fb_scaling = fb_damageScalings->Get(scalingIndex);
                const auto fb_stat    = fb_stats->Get(fb_scaling->stat_type());
                PlaceholdFormattedString(
                  TextFormat("%d%%", fb_scaling->percents_per_tier()->Get(tierOffset)),
                  palTextWhite
                );
                PlaceholdImage(fb_stat->small_icon_texture_id());
              }

              PlaceholdGroupEnd();

              BF_CLAY_TEXT_BROKEN_LOCALIZED(locale, {.color = palTextBezhevy});

              FlexEnd();
            }
          };

          // Damage.
          {
            int projectileCount = 1;
            if (fb->projectile_spawn_frame_factors())
              projectileCount *= fb->projectile_spawn_frame_factors()->size();
            if (fb->projectile_count())
              projectileCount *= fb->projectile_count()->Get(tierOffset);
            componentWeaponDamageWithScalings(
              Loc_UI_WEAPON_DAMAGE,
              fb->base_damage()->Get(tierOffset),
              CalculateWeaponDamage(
                data.weaponIndexOrMinus1, data.weapon, tier, data.affectedByGame
              ),
              projectileCount,
              fb->damage_scalings(),
              tierOffset
            );
          }

          // Burning damage.
          if (fb->burning_damage()) {
            componentWeaponDamageWithScalings(
              Loc_UI_BURNING_DAMAGE,
              fb->burning_damage()->Get(tierOffset),
              ApplyDamageScalings(
                fb->burning_damage()->Get(tierOffset),
                tierOffset,
                fb->burning_damage_scalings(),
                1
              ),
              fb->burning_times()->Get(tierOffset),
              fb->burning_damage_scalings(),
              tierOffset
            );
          }

          // Critical.
          {
            int chance = fb->crit_chance()->Get(tierOffset);
            if (data.affectedByGame)
              chance += GetStatValue(StatType_CRIT_CHANCE);

            chance = MIN(100, MAX(0, chance));

            componentWeaponStatEntry(Loc_UI_CRIT, [&]() BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT(TextFormat(
                "x%.1f (%d%%)", fb->crit_damage_multiplier()->Get(tierOffset), chance
              ));
            });
          }

          // Cooldown.
          componentWeaponStatEntry(Loc_UI_COOLDOWN, [&]() BF_FORCE_INLINE_LAMBDA {
            const f32 cooldownSeconds = fb->cooldown()->Get(tierOffset);
            BF_CLAY_TEXT(TextFormat("%.2f", cooldownSeconds), {.color = palTextGreen});
            BF_CLAY_TEXT_LOCALIZED(Loc_UI_SECONDS_SUFFIX, {.color = palTextGreen});
          });

          // Knockback.
          {
            f32 value = fb->knockback_meters()->Get(tierOffset) * KNOCKBACK_SCALE;
            if (data.affectedByGame)
              value *= 1 + (f32)GetStatValue(StatType_KNOCKBACK) / 100.0f;
            if (value > 0) {
              componentWeaponStatEntry(Loc_UI_KNOCKBACK, [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT(StripLeadingZerosInFloat(TextFormat("%.1f", value)));
              });
            }
          }

          // Range.
          componentWeaponStatEntry(Loc_UI_RANGE, [&]() BF_FORCE_INLINE_LAMBDA {
            const f32 rangeMeters
              = CalculateWeaponRangeMeters(data.weapon, tier, data.affectedByGame);
            if (fb->projectile_type())
              BF_CLAY_TEXT(TextFormat("%.1f", rangeMeters));
            else {
              const f32 meleeRangeMeters
                = (f32)glib->original_texture_sizes()->Get(fb->texture_ids()->Get(0))->x()
                  * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;
              BF_CLAY_TEXT(TextFormat("%.1f", meleeRangeMeters + rangeMeters));
            }
          });

          // Pierce.
          if (fb->projectile_type()
              || (fb->projectile_pierce() && fb->projectile_pierce()->Get(tierOffset)))
          {
            int value = 0;

            auto pierces = fb->projectile_pierce();
            if (pierces)
              value += pierces->Get(tierOffset);

            if (data.affectedByGame)
              value += GetStatValue(StatType_PIERCE);

            if (value > 0) {
              int piercingDamageBonusPercent = GetStatValue(StatType_PIERCE_DAMAGE);
              if (fb->projectile_piercing_damage_bonus_percent())
                piercingDamageBonusPercent
                  += fb->projectile_piercing_damage_bonus_percent()->Get(tierOffset);

              componentWeaponStatEntry(Loc_UI_PIERCE, [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT(TextFormat(
                  "%d (-%d%%)", value, MAX(0, MIN(100, 100 - piercingDamageBonusPercent))
                ));
              });
            }
          }

          // Bounce.
          if (fb->projectile_type()
              || (fb->projectile_bounce() && fb->projectile_bounce()->Get(tierOffset)))
          {
            int value = 0;

            auto bounces = fb->projectile_bounce();
            if (bounces)
              value += bounces->Get(tierOffset);

            if (data.affectedByGame)
              value += GetStatValue(StatType_BOUNCE);

            if (value > 0) {
              componentWeaponStatEntry(Loc_UI_BOUNCE, [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT(TextFormat("%d", value));
              });
            }
          }

          // Life Steal.
          {
            auto chance = GetLifestealChance(data.weapon, tier, data.affectedByGame);

            if (chance > 0) {
              componentWeaponStatEntry(
                fb_stats->Get(StatType_LIFE_STEAL)->name_locale(),
                [&]() BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT(
                    TextFormat(
                      "%s%%",
                      StripLeadingZerosInFloat(TextFormat("%.1f", chance * 100.0f))
                    ),
                    {.color = palTextGreen}
                  );
                }
              );
            }
          }

          componentEffectsExploded(fb->effects(), tierOffset, 1, 0, CARD_WIDTH);

          // This wave damage.
          ASSERT(thisWaveDamage >= 0);
          if (thisWaveDamage > 0) {
            componentWeaponStatEntry(
              Loc_UI_THIS_WAVE_DAMAGE,
              [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT(TextFormat("%d", thisWaveDamage), {.color = palTextWhite});
              }
            );
          };
        }
        // Locked info.
        else if (data.lockInfo.achievement && (data.lockInfo.stepIndex >= 0)) {  ///
          CLAY({.layout{
            BF_CLAY_SIZING_GROW_XY,
            .childGap = GAP_SMALL,
            BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          }}) {
            BEAUTIFY_WIGGLING_DANGER_SCOPED(
              g.ui.newRunErrorLocked,
              WEAPONS_WIGGLING_LOGICAL_AMPLITUDE,
              ERROR_WIGGLING_FRAMES,
              ERROR_WIGGLING_TIMES
            );

            componentAchievementDescription(
              data.lockInfo.achievement, data.lockInfo.stepIndex, false
            );
          }
        }

        FontEnd();

        BF_CLAY_SPACER_VERTICAL;

        if (data.shopBuyingIndex >= 0) {  ///
          CLAY({.layout{
            BF_CLAY_SIZING_GROW_X,
            BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
          }}) {
            // Buying item / weapon.
            const auto buyButtonID = getIDFromShopBuyingIndex(data.shopBuyingIndex);

            SDL_Scancode keys_[]{
              (SDL_Scancode)((int)SDL_SCANCODE_1 + data.shopBuyingIndex),
            };
            VIEW_FROM_ARRAY_DANGER(keys);

            if ((g.run.shopActivatedModalWeaponIndex >= 0) || g.meta.showingStats.IsSet())
              keys.count = 0;

            const bool bought = componentButton(
              {
                .id    = buyButtonID,
                .group = data.shopGroup,
                .keys  = keys,
                .tier  = tier,
                .breathing{
                  .set            = canBuy,
                  .bonusBreatheAt = g.run.state.shop.rerolls.lastRerolledAt,
                },
              },
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                CLAY({.layout{
                  BF_CLAY_SIZING_GROW_X,
                  BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                }}) {
                  FontBegin(&g.meta.fontPricesOutlined);
                  BF_CLAY_TEXT(
                    TextFormat("%d ", price),
                    {.color = ((price <= PLAYER_COINS) ? palTextWhite : palTextRed)}
                  );
                  FontEnd();
                  BF_CLAY_IMAGE({.texID = glib->ui_coin_texture_id()});
                }
              }
            );

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

                    RemoveImmediateWeaponEffects();
                    weapon.tier += 1;
                    ApplyImmediateWeaponEffects();

                    weapon.uiBouncedAt = {};
                    weapon.uiBouncedAt.SetNow();
                  }
                  else {
                    RemoveImmediateWeaponEffects();

                    // Filling empty weapon slot if exists.
                    weapon.type = data.weapon;
                    weapon.tier = tier;

                    ApplyImmediateWeaponEffects();

                    weapon.uiBouncedAt = {};
                    weapon.uiBouncedAt.SetNow();
                  }

                  Array<int, WeaponType_COUNT> weaponCounts{};
                  for (const auto& w : g.run.state.weapons)
                    weaponCounts[w.type]++;
                  for (int weaponCount : weaponCounts)
                    AchievementMax(AchievementType_HAVE_SAME_WEAPONS, weaponCount);
                }
                else if (data.item)
                  AddItem(data.item);
                else
                  INVALID_PATH;

                ChangeFocusFrom(
                  ControlsContext_SHOP, buyButtonID, data.shopFocusAfterBuying
                );

                g.run.state.shop.toPick[data.shopBuyingIndex] = {};
                Save();
              }
              else {
                if (canBuyErrorBuild) {
                  g.ui.shopErrorBuild = {};
                  g.ui.shopErrorBuild.SetNow();
                }
                else if (canBuyErrorWeapon) {
                  g.ui.shopErrorWeapons = {};
                  g.ui.shopErrorWeapons.SetNow();
                }
                else {
                  g.ui.errorGold = {};
                  g.ui.errorGold.SetNow();
                }
                PlaySound(Sound_UI_ERROR);
              }
            }
          }
        }

        if (data.shopSelling && data.weapon) {  ///
          SCOPED_CONTEXT_IF(
            ControlsContext_MODAL_SHOP_WEAPON_DETAILS, data.shopDetailsContextIsActive
          );

          auto alignX = CLAY_ALIGN_X_RIGHT;
          if (ge.events.last == LastEventType_TOUCH)
            alignX = CLAY_ALIGN_X_CENTER;

          CLAY({.layout{
            BF_CLAY_SIZING_GROW_X,
            .childGap = GAP_SMALL,
            .childAlignment{.x = alignX, .y = CLAY_ALIGN_Y_CENTER},
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
            bool combined  = false;
            auto combineID = CLAY_ID("button_weapon_combine");
            if (canCombineWithIndex >= 0) {
              combined = componentButton(
                {
                  .id    = combineID,
                  .group = groupDetails,
                  .tier  = 7,
                  .breathing{.set = true},
                },
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_IMAGE({.texID = glib->ui_icon_combine_texture_id()});
                }
              );
            }

            // Recycle button.
            const auto weaponRecycleID = CLAY_ID("button_weapon_recycle");
            const bool recycled        = componentButtonRecycle({
                     .id    = weaponRecycleID,
                     .group = groupDetails,
                     .price = recyclePrice,
                     .tier  = 4,
            });

            // Cancel button.
            auto cancelID  = CLAY_ID("button_weapon_cancel");
            bool cancelled = false;

            if (ge.events.last != LastEventType_TOUCH) {
              cancelled = componentButton(
                {
                  .id    = cancelID,
                  .group = groupDetails,
                  .keys  = KEYS_CANCEL,
                  .tier  = tier,
                },
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_IMAGE({.texID = glib->ui_icon_cancel_texture_id()});
                }
              );
              markControlAsDefault(cancelID);
            }

            if (combined) {
              ASSERT(canCombineWithIndex >= 0);

              RemoveImmediateWeaponEffects();
              weapon.tier += 1;

              weapon.thisWaveDamage        = 0;
              weapon.thisWaveKilledEnemies = MAX(
                weapon.thisWaveKilledEnemies,
                g.run.state.weapons[canCombineWithIndex].thisWaveKilledEnemies
              );

              weapon.uiBouncedAt = {};
              weapon.uiBouncedAt.SetNow();

              StableRemoveWeapon(canCombineWithIndex);
              ApplyImmediateWeaponEffects();
              Save();

              int focusIndex = data.weaponIndexOrMinus1;
              if (canCombineWithIndex < data.weaponIndexOrMinus1) {
                // Combined weapon to the left is removed -> our weapon moved left.
                focusIndex--;
              }

              ResetFocus(ControlsContext_SHOP);
              ChangeFocus(ControlsContext_SHOP, getIDFromPlayerWeaponIndex(focusIndex));
            }

            if (recycled) {
              ChangeCoins(recyclePrice);
              RemoveImmediateWeaponEffects();
              StableRemoveWeapon(data.weaponIndexOrMinus1);
              ApplyImmediateWeaponEffects();
              g.run.runRecycledWeapons++;
              AchievementMax(
                AchievementType_RECYCLE_X_WEAPONS_DURING_A_RUN, g.run.runRecycledWeapons
              );
              Save();

              ResetFocus(ControlsContext_SHOP);
              ChangeFocus(ControlsContext_SHOP, data.shopFocusAfterRecycling);
            }

            if (cancelled || recycled || combined) {
              PlaySound(Sound_UI_CLICK);
              g.run.shopActivatedModalWeaponIndex = -1;
            }
          }
        }
        else if (data.shopSelling)
          INVALID_PATH;
      }
    }
  };

  LAMBDA (void, componentAchievement, (AchievementType type, int stepIndex, bool shadow))
  {  ///
    auto fb      = fb_achievements->Get(type);
    auto fb_step = (type ? fb->steps()->Get(stepIndex) : nullptr);

    const bool isLocked = !type || IsAchievementStepLocked(type, stepIndex);

    int tier = (isLocked ? 0 : 3);

    CLAY({
      .layout{
        .sizing{
          .width  = CLAY_SIZING_FIXED(ACHIEVEMENT_WIDTH + 2 * PADDING_FRAME),
          .height = CLAY_SIZING_FIXED(ACHIEVEMENT_HEIGHT + 2 * PADDING_FRAME),
        },
        BF_CLAY_PADDING_ALL(PADDING_FRAME),
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_SHADOW(glib->ui_frame_shadow_small_nine_slice(), shadow),
        BF_CLAY_CUSTOM_NINE_SLICE(
          glib->ui_frame_nine_slice(), tier, true, {.set = false}
        ),
      } BF_CLAY_CUSTOM_END,
    }) {
      auto fb_previousStep
        = (type && (stepIndex > 0) ? fb->steps()->Get(stepIndex - 1) : nullptr);

      // Name.
      CLAY({}) {
        const auto nameColor = textColorsPerTier[tier];

        if (type) {
          if (IsAchievementStepLocked(type, stepIndex))
            BF_CLAY_TEXT("???", {.color = nameColor});
          else {
            BF_CLAY_TEXT_LOCALIZED(fb->name_locale(), {.color = nameColor});

            if (fb->steps()->size() > 1) {
              static const char* const romanNumbers_[]{
                "",    "I",    "II",  "III",  "IV",    "V",   "VI",
                "VII", "VIII", "IX",  "X",    "XI",    "XII", "XIII",
                "XIV", "XV",   "XVI", "XVII", "XVIII", "XIX", "XX",
              };
              VIEW_FROM_ARRAY_DANGER(romanNumbers);
              BF_CLAY_TEXT(" ", {.color = nameColor});
              BF_CLAY_TEXT(romanNumbers[stepIndex + 1], {.color = nameColor});
            }
          }
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
          componentAchievementDescription(type, stepIndex);
          FontEnd();
        }
      }
    }
  };

  struct GridEntryDetailsData {  ///
    Clay_ElementId id = {};

    DifficultyType difficulty = {};
    BuildType      build      = {};
    ItemType       item       = {};
    WeaponType     weapon     = {};

    int count               = 1;
    int thisWaveAddedCount  = 1;
    int weaponIndexOrMinus1 = -1;

    bool affectedByGame = false;
    int  overrideTier   = -1;

    bool           weAreInShop             = false;
    Clay_ElementId shopFocusAfterRecycling = {};

    int detailsRight = -1;
    int detailsBelow = -1;
  };

  bool _shownDetailsThisFrame = false;

  LAMBDA (void, gridEntryDetails, (GridEntryDetailsData data)) {  ///
    ASSERT(
      (int)((bool)data.difficulty) + (int)((bool)data.build) + (int)((bool)data.item)
        + (int)((bool)data.weapon)
      == 1
    );

    ASSERT(data.detailsRight >= 0);
    ASSERT(data.detailsRight <= 1);
    ASSERT(data.detailsBelow >= 0);
    ASSERT(data.detailsBelow <= 1);

    if (_shownDetailsThisFrame)
      return;

    const bool hovered = !g.meta.playerUsesKeyboardOrController && Clay_Hovered();

    const bool isSelected
      = (data.weaponIndexOrMinus1 >= 0)
        && (g.run.shopActivatedModalWeaponIndex == data.weaponIndexOrMinus1);

    if ((hovered && !g.run.hideSlotDetails)                                        //
        || (!g.run.hideSlotDetails && (CURRENT_CONTEXT.focused.id == data.id.id))  //
        || isSelected)
    {
      _shownDetailsThisFrame = true;

      f32                          offsetY{};
      Clay_FloatingAttachPointType attachElement{};
      Clay_FloatingAttachPointType attachParent{};

      ASSERT(data.detailsRight >= 0);
      ASSERT(data.detailsRight <= 1);
      ASSERT(data.detailsBelow >= 0);
      ASSERT(data.detailsBelow <= 1);

      if (data.detailsBelow) {
        offsetY = GAP_SMALL;

        if (data.detailsRight) {
          attachElement = CLAY_ATTACH_POINT_LEFT_TOP;
          attachParent  = CLAY_ATTACH_POINT_LEFT_BOTTOM;
        }
        else {
          attachElement = CLAY_ATTACH_POINT_RIGHT_TOP;
          attachParent  = CLAY_ATTACH_POINT_RIGHT_BOTTOM;
        }
      }
      else {
        offsetY = -GAP_SMALL;

        if (data.detailsRight) {
          attachElement = CLAY_ATTACH_POINT_LEFT_BOTTOM;
          attachParent  = CLAY_ATTACH_POINT_LEFT_TOP;
        }
        else {
          attachElement = CLAY_ATTACH_POINT_RIGHT_BOTTOM;
          attachParent  = CLAY_ATTACH_POINT_RIGHT_TOP;
        }
      }

      zIndex += UIZIndexOffset_HOVER_DETAILS;

      auto pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE;
      if (!draw) {
        if ((ge.meta._latestActiveTouchID != InvalidTouchID)
            && IsTouchDown(ge.meta._latestActiveTouchID)
            && !IsTouchPressed(ge.meta._latestActiveTouchID))
          pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH;
      }

      CLAY({
        .layout{
          .sizing{CLAY_SIZING_FIXED(CARD_WIDTH + 2 * PADDING_FRAME), CLAY_SIZING_FIT(0)},
        },
        .floating{
          .offset{0, offsetY},
          .zIndex = zIndex,
          .attachPoints{.element = attachElement, .parent = attachParent},
          .pointerCaptureMode = pointerCaptureMode,
          .attachTo           = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        if (isSelected) {
          componentOverlay([&]() BF_FORCE_INLINE_LAMBDA {
            if (clickOrTouchPressed())
              g.run.shopActivatedModalWeaponIndex = -1;
          });
        }

        componentUniversalCard(ComponentUniversalCardData{
          .difficulty                 = data.difficulty,
          .build                      = data.build,
          .item                       = data.item,
          .weapon                     = data.weapon,
          .count                      = data.count,
          .thisWaveAddedCount         = data.thisWaveAddedCount,
          .weaponIndexOrMinus1        = data.weaponIndexOrMinus1,
          .affectedByGame             = data.affectedByGame,
          .overrideTier               = data.overrideTier,
          .shopDetailsContextIsActive = isSelected,
          .shopSelling                = data.weAreInShop,
          .shopFocusAfterRecycling    = data.shopFocusAfterRecycling,
          .shadow                     = true,
        });
      }

      zIndex -= UIZIndexOffset_HOVER_DETAILS;
    }

    if ((CURRENT_CONTEXT.focused.id == data.id.id)  //
        && !hovered                                 //
        && ge.events.thisFrame.Mouse())
      g.run.hideSlotDetails = true;
  };

  struct ComponentItemsGridData {  ///
    ControlsGroupID group              = {};
    ControlsGroupID groupArrows        = {};
    int             itemsX             = {};
    int             itemsMaxY          = {};
    int*            scroll             = {};
    int             detailsRight       = -1;
    int             detailsBelow       = -1;
    bool            markFirstAsDefault = false;
    bool            affectedByGame     = true;
  };

  LAMBDA (void, componentItemsGrid, (ComponentItemsGridData data)) {  ///
    ASSERT(data.group);
    ASSERT(data.groupArrows);
    ASSERT(data.itemsX > 0);
    ASSERT(data.itemsMaxY > 0);
    ASSERT(data.scroll);

    ASSERT(data.detailsRight >= 0);
    ASSERT(data.detailsRight <= 1);
    ASSERT(data.detailsBelow >= 0);
    ASSERT(data.detailsBelow <= 1);

    CLAY({.layout{.childGap = GAP_SMALL}}) {
      const int ITEMS_X   = data.itemsX;
      const int totalRows = CeilDivision(g.run.state.items.count + 2, ITEMS_X);

      // const int maxScroll = MAX(0, ITEMS_Y - data.itemsMaxY);
      const int totalPages
        = CeilDivision(g.run.state.items.count + 2, ITEMS_X * data.itemsMaxY);
      const int maxScroll = totalPages - 1;

      CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
        FOR_RANGE (int, y, data.itemsMaxY) {
          ControlsGroupNewRow(data.group);

          bool shouldBreak = false;

          CLAY({.layout{.childGap = GAP_SMALL}})
          FOR_RANGE (int, x, ITEMS_X) {
            const int t = (y + (*data.scroll) * data.itemsMaxY) * ITEMS_X + x;
            if ((t >= g.run.state.items.count + 2) && !maxScroll) {
              shouldBreak = true;
              break;
            }

            CLAY({.layout{.sizing{
              .width  = CLAY_SIZING_FIXED(slotSize.x),
              .height = CLAY_SIZING_FIXED(slotSize.y),
            }}})
            if (t < g.run.state.items.count + 2) {
              DifficultyType difficultyType     = {};
              BuildType      buildType          = {};
              ItemType       itemType           = {};
              int            count              = 1;
              int            thisWaveAddedCount = 1;

              Clay_ElementId slotID{};

              if (t == 0) {
                BEAUTIFY_WIGGLING_DANGER_SCOPED(
                  g.ui.shopErrorBuild,
                  WEAPONS_WIGGLING_LOGICAL_AMPLITUDE,
                  ERROR_WIGGLING_FRAMES,
                  ERROR_WIGGLING_TIMES
                );

                slotID = CLAY_IDI("shop_player_item", t);
                componentUniversalSlot({
                  .id           = slotID,
                  .group        = data.group,
                  .build        = g.player.build,
                  .showsDetails = true,
                });
                buildType = g.player.build;
              }
              else if (t == 1) {
                slotID = CLAY_IDI("shop_player_item", t);
                componentUniversalSlot({
                  .id           = slotID,
                  .group        = data.group,
                  .difficulty   = g.player.difficulty,
                  .showsDetails = true,
                });
                difficultyType = g.player.difficulty;
              }
              else {
                auto& item = g.run.state.items[g.run.state.items.count - (t - 2) - 1];
                slotID     = CLAY_IDI("shop_player_item", t);
                componentUniversalSlot({
                  .id           = slotID,
                  .group        = data.group,
                  .item         = item.type,
                  .count        = item.count,
                  .showsDetails = true,
                  .uiBouncedAt  = item.uiBouncedAt,
                });
                itemType           = item.type;
                count              = item.count;
                thisWaveAddedCount = item.thisWaveAddedCount;
              }

              if (data.markFirstAsDefault && (t == 0))
                markControlAsDefault(slotID);

              processShowingOrNotShowingSlotDetails(slotID);

              gridEntryDetails(GridEntryDetailsData{
                .id                 = slotID,
                .difficulty         = difficultyType,
                .build              = buildType,
                .item               = itemType,
                .count              = count,
                .thisWaveAddedCount = thisWaveAddedCount,
                .affectedByGame     = data.affectedByGame,
                .detailsRight       = data.detailsRight,
                .detailsBelow       = data.detailsBelow,
              });
            }
          }

          if (shouldBreak)
            break;
        }
      }

      bool movedUp
        = (!g.meta.playerUsesKeyboardOrController && Clay_Hovered() && (wheel > 0));
      bool movedDown
        = (!g.meta.playerUsesKeyboardOrController && Clay_Hovered() && (wheel < 0));

      CLAY({.layout{
        BF_CLAY_SIZING_GROW_Y,
        .childGap        = GAP_SMALL,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        const auto moveUpID   = CLAY_ID("items_move_up");
        const auto moveDownID = CLAY_ID("items_move_down");

        if (*data.scroll > 0) {
          movedUp |= componentButton(
            {
              .id                = moveUpID,
              .group             = data.groupArrows,
              .growY             = true,
              .paddingHorizontal = GAP_SMALL,
              // FIXME: make item slot focused (if item was focues previously).
              // .keys              = KEYS_MOVE_UP,
            },
            [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_IMAGE({.texID = glib->ui_icon_up_texture_id()});
            }
          );
          // TODO: Keyboard items scrolling.
          // if (uiElementSwitchDirection == Direction_UP) {
          //   uiElementSwitchDirection = Direction_NONE;
          //   movedUp                  = true;
          // }
          ControlsGroupNewRow(data.groupArrows);
        }
        else {
          movedUp = false;
          BF_CLAY_SPACER_VERTICAL;
        }

        if (*data.scroll < maxScroll) {
          movedDown |= componentButton(
            {
              .id                = moveDownID,
              .group             = data.groupArrows,
              .growY             = true,
              .paddingHorizontal = GAP_SMALL,
              // FIXME: make item slot focused (if item was focues previously).
              // .keys              = KEYS_MOVE_DOWN,
            },
            [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_IMAGE({.texID = glib->ui_icon_down_texture_id()});
            }
          );
          // TODO: Keyboard items scrolling.
          // if (uiElementSwitchDirection == Direction_DOWN) {
          //   uiElementSwitchDirection = Direction_NONE;
          //   movedDown                = true;
          // }
        }
        else {
          movedDown = false;
          BF_CLAY_SPACER_VERTICAL;
        }

        if (movedUp) {
          *data.scroll = MAX(*data.scroll - 1, 0);
          if (*data.scroll == 0)
            ChangeFocusFrom(currentContext, moveUpID, moveDownID);
        }

        if (movedDown) {
          *data.scroll = MIN(*data.scroll + 1, maxScroll);
          if (*data.scroll == maxScroll)
            ChangeFocusFrom(currentContext, moveDownID, moveUpID);
        }

        if (movedUp || movedDown)
          PlaySound(Sound_UI_CLICK);
      }
    }
  };

  struct ComponentWeaponsGridData {  ///
    ControlsGroupID group          = {};
    int             weaponsX       = {};
    bool            detailsRight   = 1;
    bool            detailsBelow   = 0;
    bool            affectedByGame = true;
    bool            weAreInShop    = false;
  };

  LAMBDA (void, componentWeaponsGrid, (ComponentWeaponsGridData data)) {  ///
    ASSERT(data.weaponsX > 0);
    const int WEAPONS_Y = CeilDivision(g.run.state.weapons.count, data.weaponsX);

    int weaponsCount = 0;
    for (auto& x : g.run.state.weapons) {
      if (x.type)
        weaponsCount++;
    }

    CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}})
    FOR_RANGE (int, y, WEAPONS_Y) {
      ControlsGroupNewRow(data.group);

      CLAY({.layout{.childGap = GAP_SMALL}})
      FOR_RANGE (int, x, data.weaponsX) {
        const int weaponIndex = y * data.weaponsX + x;
        if (weaponIndex >= g.run.state.weapons.count)
          break;

        const auto& weapon = g.run.state.weapons[weaponIndex];

        int wouldCombineWith = -1;

        if (weapon.tier < TOTAL_TIERS - 1) {
          FOR_RANGE (int, combineIndex, g.run.state.weapons.count) {
            if (weaponIndex == combineIndex)
              continue;
            const auto& w = g.run.state.weapons[combineIndex];
            if (!w.type)
              break;
            if ((w.type == weapon.type) && (w.tier == weapon.tier)) {
              wouldCombineWith = combineIndex;
              break;
            }
          }
        }

        CLAY({.layout{.sizing{
          .width  = CLAY_SIZING_FIXED(slotSize.x),
          .height = CLAY_SIZING_FIXED(slotSize.y),
        }}})
        if (weapon.type) {
          int focusWeapon = weaponIndex;
          if (weaponIndex >= weaponsCount - 1)
            focusWeapon--;
          Clay_ElementId focusAfterRecycling = getIDFromPlayerWeaponIndex(focusWeapon);
          if (focusWeapon < 0) {
            focusAfterRecycling = rerollID;
            for (auto i : DEFAULT_BUYING_INDICES) {
              const auto& v = g.run.state.shop.toPick[i];
              if (v.item || v.weapon) {
                focusAfterRecycling = getIDFromShopBuyingIndex(i);
                break;
              }
            }
          }

          const auto slotID = getIDFromPlayerWeaponIndex(weaponIndex);

          // Weapon.
          const bool selectedWeapon = componentUniversalSlot({
            .id          = slotID,
            .group       = data.group,
            .weapon      = weapon.type,
            .weaponIndex = weaponIndex,
            .tier        = weapon.tier,
            .breathing{.set = data.weAreInShop && (wouldCombineWith >= 0)},
            .showsDetails = true,
            .uiBouncedAt  = weapon.uiBouncedAt,
          });

          if (data.weAreInShop && selectedWeapon && !ge.events.thisFrame.Touch()) {
            PlaySound(Sound_UI_CLICK);
            g.run.shopActivatedModalWeaponIndex = weaponIndex;
          }

          // Hovering modal.
          if (weapon.type) {
            processShowingOrNotShowingSlotDetails(slotID);

            gridEntryDetails(GridEntryDetailsData{
              .id                      = slotID,
              .weapon                  = weapon.type,
              .count                   = 1,
              .weaponIndexOrMinus1     = weaponIndex,
              .affectedByGame          = data.affectedByGame,
              .overrideTier            = weapon.tier,
              .weAreInShop             = data.weAreInShop,
              .shopFocusAfterRecycling = focusAfterRecycling,
              .detailsRight            = data.detailsRight,
              .detailsBelow            = data.detailsBelow,
            });
          }
        }
      }
    }
  };

  LAMBDA (void, componentStats2, ()) {  ///
    static int maxStatIconWidth = 0;
    if (!maxStatIconWidth) {
      maxStatIconWidth = glib->original_texture_sizes()
                           ->Get(glib->ui_shop_current_level_icon_texture_id())
                           ->x();
      for (auto fb_stat : *fb_stats) {
        if (fb_stat->small_icon_texture_id()) {
          maxStatIconWidth = MAX(
            maxStatIconWidth,
            glib->original_texture_sizes()->Get(fb_stat->small_icon_texture_id())->x()
          );
        }
      }
    }

    CLAY({.layout{.childGap = GAP_FLEX, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
      LAMBDA (
        void, entry, (StatType stat, int texID, int value, Color color = palTextWhite)
      )
      {
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_X,
          BF_CLAY_CHILD_ALIGNMENT_RIGHT_CENTER,
        }}) {
          // Increasing stat by clicking on it in debug mode.
          if (stat && (ge.meta.debugEnabled || BF_DEBUG)) {
            if (clickOrTouchPressed())
              ChangeStaticAndDynamicStatBy(stat, 1);
            if (wheel && Clay_Hovered())
              ChangeStaticAndDynamicStatBy(stat, wheel);
          }

          if (value)
            BF_CLAY_TEXT(TextFormat("%d", value), {.color = color});
          else {
            Beautify b{.translate{0, -GAP_FLEX}};
            BEAUTIFY(b);
            BF_CLAY_TEXT("-");
          }

          CLAY({.layout{.sizing{
            .width = CLAY_SIZING_FIXED(maxStatIconWidth * ASSETS_TO_LOGICAL_RATIO)
          }}}) {
            componentCenterFloater([&]() BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_IMAGE({.texID = texID});
            });
          }
        }
      };

      FontBegin(&g.meta.fontUIBig);

      int statIndex = -1;
      for (const auto fb_stat : *fb_stats) {
        statIndex++;
        if (fb_stat->is_secondary()  //
            || fb_stat->is_hidden()  //
            || !fb_stat->small_icon_texture_id())
          continue;

        int v = GetStatValue((StatType)statIndex);

        auto color = palTextWhite;
        if (v > 0)
          color = (fb_stat->negative_is_good() ? palTextRed : palTextGreen);
        else if (v < 0)
          color = (fb_stat->negative_is_good() ? palTextGreen : palTextRed);

        entry((StatType)statIndex, fb_stat->small_icon_texture_id(), v, color);
      }

      FontEnd();
    }
  };

  LAMBDA (
    void,
    componentStats2Wrapped,
    (Clay_FloatingAttachPoints attachPoints
     = {.element = CLAY_ATTACH_POINT_RIGHT_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_CENTER},
     Vector2 offset = {-GAP_SMALL, 0})
  )
  {  ///
    CLAY({.floating{
      .offset{offset.x, offset.y},
      .zIndex             = zIndex,
      .attachPoints       = attachPoints,
      .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
      .attachTo           = CLAY_ATTACH_TO_PARENT,
    }}) {
      componentStats2();
    }
  };

  const f32 topRowHeight
    = (f32)glib->original_texture_sizes()->Get(glib->ui_icon_stats_texture_id())->y()
        * ASSETS_TO_LOGICAL_RATIO
      + 2 * GAP_SMALL;

  struct ComponentButtonAchievementsData {  ///
    const char*     id    = {};
    ControlsGroupID group = {};
  };

  LAMBDA (Clay_ElementId, CurrentContextButtonId, (const char* id, int index = 0)) {  ///
    const Clay_String str{.length = (i32)strlen(id), .chars = id};
    const auto        value = CLAY_SIDI(str, index);
    ASSERT(currentContext);
    return {.id = value.id + Hash32((u8*)&currentContext, sizeof(currentContext))};
  };

  LAMBDA (void, componentButtonAchievements, (ComponentButtonAchievementsData data))
  {  ///
    const bool clicked = componentButton(
      {
        .id             = CurrentContextButtonId("button_achievements"),
        .group          = data.group,
        .hideBackground = true,
      },
      [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
        BF_CLAY_IMAGE(
          {
            .texID = glib->ui_icon_achievements_texture_id(),
            .color = (hovered ? WHITE : TOP_BUTTON_NOT_HOVERED_COLOR),
          },
          [&]() BF_FORCE_INLINE_LAMBDA {
            auto percent = GetAchievementsCompletedPercent();
            if (percent <= 0)
              return;

            auto color = palTextBezhevy;
            if (percent >= 100)
              color = palTextGreen;

            CLAY({.layout{
              BF_CLAY_SIZING_GROW_XY,
              BF_CLAY_CHILD_ALIGNMENT_CENTER_BOTTOM,
            }}) {
              FontBegin(&g.meta.fontUIBigOutlined);
              BF_CLAY_TEXT(
                TextFormat("%d%%", percent), {.color = (hovered ? WHITE : color)}
              );
              FontEnd();
            }
          }
        );
      }
    );

    if (clicked) {
      PlaySound(Sound_UI_CLICK);
      g.meta.showingAchievements = true;
    }
  };

  struct ComponentButtonPauseData {  ///
    ControlsGroupID group = {};
  };

  LAMBDA (void, componentButtonPause, (ComponentButtonPauseData data)) {  ///
    const bool clicked = componentButton(
      {.id = CurrentContextButtonId("button_pause"), .group = data.group},
      [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
        BF_CLAY_IMAGE({.texID = glib->ui_icon_pause_small_texture_id()});
      }
    );

    if (clicked) {
      PlaySound(Sound_UI_CLICK);
      g.meta.scheduledTogglePause = true;
    }
  };

  struct ComponentButtonBackData {  ///
    ControlsGroupID group                = {};
    bool            markControlAsDefault = false;
  };

  LAMBDA (bool, componentButtonBack, (ComponentButtonBackData data = {})) {  ///
    auto       id      = CurrentContextButtonId("button_back");
    const bool clicked = componentButton(
      {
        .id             = id,
        .group          = data.group,
        .keys           = KEYS_CANCEL,
        .hideBackground = true,
      },
      [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
        BF_CLAY_IMAGE({
          .texID = glib->ui_icon_back_big_texture_id(),
          .color = (hovered ? WHITE : TOP_BUTTON_NOT_HOVERED_COLOR),
        });
      }
    );
    if (data.markControlAsDefault)
      markControlAsDefault(id);
    if (clicked)
      PlaySound(Sound_UI_CLICK);
    return clicked;
  };

  struct ComponentVolumeButtonsData {  ///
    ControlsGroupID group = {};
  };

  LAMBDA (void, componentVolumeButtons, (ComponentVolumeButtonsData data)) {  ///
    int nextVolumeButtonNumber = 1;

    LAMBDA (void, componentButtonVolume, (int iconTexID, int* var)) {
      const bool clicked = componentButton(
        {
          .id    = CurrentContextButtonId("button_volume", nextVolumeButtonNumber),
          .group = data.group,
          .paddingHorizontal = (u16)((nextVolumeButtonNumber == 1) ? GAP_BIG : GAP_SMALL),
          .hideBackground    = true,
        },
        [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
          const auto color = (hovered ? WHITE : TOP_BUTTON_NOT_HOVERED_COLOR);
          BF_CLAY_IMAGE({.texID = iconTexID, .color = color});
          BF_CLAY_IMAGE({
            .texID = glib->ui_icon_volume_bands_texture_ids()->Get(MAX(0, *var - 1)),
            .color = Fade(color, (*var ? 1 : 0)),
          });
        }
      );
      if (clicked) {
        *var = *var - 1;
        if (*var < 0)
          *var = 3;
        PlaySound(Sound_UI_CLICK);
        Save();
      }
      nextVolumeButtonNumber++;
    };

    componentButtonVolume(glib->ui_icon_sfx_texture_id(), &g.player.volumeSFX);

    componentButtonVolume(glib->ui_icon_music_texture_id(), &g.player.volumeMusic);
  };

  LAMBDA (void, componentTopRow, (auto innerLambda)) {  ///
    CLAY({.layout{
      .sizing{
        .width  = CLAY_SIZING_GROW(0),
        .height = CLAY_SIZING_FIXED(topRowHeight),
      },
      .childGap = GAP_SMALL,
      BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
    }}) {
      innerLambda();
    }
  };

  LAMBDA (void, componentScreenName_floatingInTheCenter, (Loc locale, auto innerLambda))
  {  ///
    FontBegin(&g.meta.fontUIGiganticOutlined);
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
      BF_CLAY_TEXT_LOCALIZED((Loc)((int)locale + LOCALE_CAPS_OFFSET));
      innerLambda();
    }
    FontEnd();
  };

  LAMBDA (void, componentPlayerCoins, ()) {  ///
    BEAUTIFY_WIGGLING_DANGER_SCOPED(
      g.ui.errorGold,
      GOLD_WIGGLING_LOGICAL_AMPLITUDE,
      ERROR_WIGGLING_FRAMES,
      ERROR_WIGGLING_TIMES
    );

    CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
      BF_CLAY_IMAGE({.texID = glib->ui_coin_texture_id()});

      auto color = GetFlashingColor(
        palTextWhite,
        palTextRed,
        g.ui.errorGold,
        ERROR_WIGGLING_FRAMES,
        ERROR_GOLD_FLASHING_TIMES,
        ERROR_GOLD_FLASH_NOT_FLASH_RATIO,
        ERROR_GOLD_PRECALC_X
      );

      FontBegin(&g.meta.fontUIBig);
      BF_CLAY_TEXT(TextFormat(" %d", PLAYER_COINS), {.color = color});
      FontEnd();

      if (ge.meta.debugEnabled && Clay_Hovered() && wheel)
        ChangeCoins(wheel);
    }
  };

  LAMBDA (void, componentButtonStats, (ControlsGroupID group)) {  ///
    SDL_Scancode keys_[]{SDL_SCANCODE_TAB};
    VIEW_FROM_ARRAY_DANGER(keys);

    if (g.meta.showingStats.IsSet() || (g.run.shopActivatedModalWeaponIndex >= 0))
      keys.count = 0;

    const bool clickedStats = componentButton(
      {.id = CLAY_ID("button_stats"), .group = group, .keys = keys},
      [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
        BF_CLAY_IMAGE({.texID = glib->ui_icon_stats_texture_id()});
      }
    );

    if (clickedStats && !g.meta.showingStats.IsSet()) {
      PlaySound(Sound_UI_CLICK);
      g.meta.showingStats.SetNow();
    }
  };

  LAMBDA (void, componentWaveAndTimerInCenterTopOfTheScreen, ()) {  ///
    CLAY({
      .layout{
        .childGap = GAP_BIG, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        // .layoutDirection = CLAY_TOP_TO_BOTTOM,
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

      FontBegin(&g.meta.fontUIBigOutlined);

      if (g.run.state.waveIndex > 0) {
        BF_CLAY_IMAGE(
          {.texID = glib->ui_icon_current_wave_texture_id()},
          [&]() BF_FORCE_INLINE_LAMBDA {
            CLAY({.layout{
              BF_CLAY_SIZING_GROW_XY,
              .padding{.top = GAP_SMALL / 2},
              BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
            }}) {
              BF_CLAY_TEXT(TextFormat("%d", g.run.state.waveIndex + 1));
            }
          }
        );
      }

      if (g.run.waveStartedAt.IsSet()) {
        BF_CLAY_IMAGE(
          {.texID = glib->ui_icon_stopwatch2_texture_id()},
          [&]() BF_FORCE_INLINE_LAMBDA {
            CLAY({.layout{
              BF_CLAY_SIZING_GROW_XY,
              .padding{.top = GAP_SMALL / 2},
              BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
            }}) {
              const auto remainingFrames
                = GetWaveDuration(g.run.state.waveIndex) - g.run.waveStartedAt.Elapsed();
              int remainingSeconds
                = CeilDivision(MAX(1, remainingFrames.value), FIXED_FPS);

              if (g.run.state.screen != ScreenType_GAMEPLAY)
                remainingSeconds = 0;

              BF_CLAY_TEXT(TextFormat("%d", remainingSeconds));
            }
          }
        );
      }

      FontEnd();
    }
  };

  // !banner: SCREENS
  // ███████╗ ██████╗██████╗ ███████╗███████╗███╗   ██╗███████╗
  // ██╔════╝██╔════╝██╔══██╗██╔════╝██╔════╝████╗  ██║██╔════╝
  // ███████╗██║     ██████╔╝█████╗  █████╗  ██╔██╗ ██║███████╗
  // ╚════██║██║     ██╔══██╗██╔══╝  ██╔══╝  ██║╚██╗██║╚════██║
  // ███████║╚██████╗██║  ██║███████╗███████╗██║ ╚████║███████║
  // ╚══════╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═══╝╚══════╝

  // Gameplay.
  if (!gdebug.hideUIForVideo && (g.run.waveStartedAt.IsSet()  //
        && (g.run.state.screen == ScreenType_GAMEPLAY)
      || (g.run.state.screen == ScreenType_WAVE_END_ANIMATION)))
  {
    Beautify b{
      .alpha
      = MIN(1, g.run.walkingTutorialCompletedAt.Elapsed().Progress(ANIMATION_1_FRAMES))
    };
    BEAUTIFY(b);

    CLAY(  ///
      {
        .layout{
          BF_CLAY_SIZING_GROW_XY,
          BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
            PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
          ),
        },
        .floating{
          .zIndex             = zIndex,
          .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
          .attachTo           = CLAY_ATTACH_TO_PARENT,
        },
      }
    )
    CLAY({.layout{BF_CLAY_SIZING_GROW_XY}}) {
      FLOATING_BEAUTIFY;

      // (Health bar + pause button) + xp bar + coins + not picked up coins.
      // {  ///
      FontBegin(&g.meta.fontUIBigOutlined);

      const int texs[]{
        glib->ui_player_bar_back_texture_id(),
        glib->ui_player_bar_top_texture_id(),
      };
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

        // Health bar + XP bar + pause button.
        CLAY({.layout{
          .childGap        = GAP_SMALL,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          // Health bar.
          BF_CLAY_IMAGE({.texID = texs[0]}, [&]() BF_FORCE_INLINE_LAMBDA {
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
                .texID = texs[1],
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

          // XP.
          BF_CLAY_IMAGE({.texID = texs[0]}, [&]() BF_FORCE_INLINE_LAMBDA {
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
                .texID = texs[1],
                .sourceMargins{
                  .right
                  = 1
                    - (f32)g.run.state.xp
                        / (f32)GetNextLevelXp(g.run.state.staticStats[StatType_LEVEL])
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
                BF_CLAY_TEXT(TextFormat("%d", g.run.state.staticStats[StatType_LEVEL]));
              }
            }
          });
        }

        // Coins.
        {
          BEAUTIFY_WIGGLING_DANGER_SCOPED(
            g.ui.errorGold,
            GOLD_WIGGLING_LOGICAL_AMPLITUDE,
            ERROR_WIGGLING_FRAMES,
            ERROR_WIGGLING_TIMES
          );

          CLAY({.layout{
            .childGap = GAP_SMALL,
            BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
          }}) {
            BF_CLAY_IMAGE({
              .texID = glib->ui_coin_texture_id(),
              .scale = Vector2One() * GetScaleOfCoins(g.ui.changedCoinsAt),
              .dontCareAboutScaleWhenCalculatingSize = true,
            });
            BF_CLAY_TEXT(TextFormat("%d", PLAYER_COINS));
          }
        }

        // Not picked up coins.
        CLAY({.layout{
          .childGap = GAP_SMALL,
          BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        }}) {
          f32        fade = (g.run.state.notPickedUpCoins > 0 ? 1 : 0);
          const auto id   = CLAY_ID("notPickedUpCoins");
          CLAY({.id = id}) {
            BF_CLAY_IMAGE({
              .texID = glib->ui_coin_x2_texture_id(),
              .scale = Vector2One() * GetScaleOfCoins(g.ui.changedNotPickedUpCoinsAt),
              .color = Fade(WHITE, fade),
              .dontCareAboutScaleWhenCalculatingSize = true,
            });
          }
          const auto d = Clay_GetElementData(id);
          if (d.found) {
            const auto& bb = d.boundingBox;
            g.ui.notPickedUpCoinsLogicalPos
              = {bb.x + bb.width / 2, LOGICAL_RESOLUTION.y - bb.y - bb.height / 2};
          }

          BF_CLAY_TEXT(
            TextFormat("%d", g.run.state.notPickedUpCoins),
            {.color = Fade(palTextWhite, fade)}
          );
        }
      }

      // Pause button and picked up chests.
      CLAY({
        .floating{
          .zIndex = zIndex,
          .attachPoints{
            .element = CLAY_ATTACH_POINT_RIGHT_TOP,
            .parent  = CLAY_ATTACH_POINT_RIGHT_TOP,
          },
          .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
          .attachTo           = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        const bool canPause
          = (g.run.state.screen == ScreenType_GAMEPLAY) && !g.meta.paused;

        auto color = WHITE;
        if (canPause && Clay_Hovered())
          color = palGreen;

        BF_CLAY_IMAGE(
          {
            .texID = glib->ui_icon_pause_texture_id(),
            .color = Fade(color, EaseOutQuad(g.meta.pauseButtonFadeProgress)),
          },
          [&]() BF_FORCE_INLINE_LAMBDA {
            if (clickOrTouchPressed() && canPause) {
              g.meta.scheduledTogglePause = true;
              PlaySound(Sound_UI_CLICK);
            }
          }
        );
      }

      FontEnd();
      // }

      // Wave.
      componentWaveAndTimerInCenterTopOfTheScreen();

      // Picked up chests.
      // { ///
      CLAY({
        .layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM},
        .floating{
          .offset{0, GAP_SMALL},
          .zIndex = zIndex,
          .attachPoints{
            .element = CLAY_ATTACH_POINT_RIGHT_BOTTOM,
            .parent  = CLAY_ATTACH_POINT_RIGHT_BOTTOM,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        FOR_RANGE (int, i, MIN(11, g.run.state.chests)) {
          const auto fb = fb_pickupables->Get(PickupableType_CHEST);
          BF_CLAY_IMAGE({
            .texID
            = fb->variation_texture_ids()->Get(i % fb->variation_texture_ids()->size()),
            .color = Fade(WHITE, 0.4f),
          });
        }
      }
      // }
    }
  }

  // New run.
  if (g.run.state.screen == ScreenType_NEW_RUN) {  ///
    SCOPED_CONTEXT(ControlsContext_NEW_RUN);

    const auto groupTop = MakeControlsGroup();
    const auto group    = MakeControlsGroup();

    LAMBDA (void, componentNewRunSelections, (auto innerLambda)) {
      CLAY({.layout{
        .sizing{
          .height = CLAY_SIZING_FIXED(2 * slotSize.y + GAP_SMALL + 2 * PADDING_FRAME),
        },
        .childGap = GAP_SMALL,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }})
      CLAY({
        .layout{
          BF_CLAY_PADDING_ALL(PADDING_FRAME),
          .childGap        = GAP_SMALL,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        BF_CLAY_CUSTOM_BEGIN{
          BF_CLAY_CUSTOM_SHADOW(glib->ui_frame_shadow_small_nine_slice(), true),
          BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice(), 0, true, {.set = false}),
        } BF_CLAY_CUSTOM_END,
      }) {
        innerLambda();
      }
    };

    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_SCREEN_BACKGROUND,
      } BF_CLAY_CUSTOM_END,
    }) {
      auto& p = g.player;

      componentTopRow([&]() BF_FORCE_INLINE_LAMBDA {
        componentButtonAchievements({.id = "new_run_achievements", .group = groupTop});

        componentScreenName_floatingInTheCenter(Loc_UI_NEW_RUN, []() {});

        if (g.ui.newRunStep > 0) {
          SDL_Scancode keys_[]{SDL_SCANCODE_ESCAPE, SDL_SCANCODE_BACKSPACE};
          VIEW_FROM_ARRAY_DANGER(keys);

          const bool backed = componentButtonBack({.group = groupTop});

          if (backed) {
            PlaySound(Sound_UI_CLICK);
            g.ui.newRunStep--;
            ASSERT(g.ui.newRunStep >= 0);
            ResetFocus(currentContext);

            if (g.ui.newRunStep == 0)
              g.ui.newRunSelectedDifficultyAt = {};
            if (g.ui.newRunStep == 1)
              g.ui.newRunSelectedBuildAt = {};
          }
        }

        BF_CLAY_SPACER_HORIZONTAL;

        componentVolumeButtons({.group = groupTop});
      });

      BF_CLAY_SPACER_VERTICAL;

      const bool difficultyIsLocked
        = ((int)p.difficulty - 1) > p.achievements[AchievementType_DIFFICULTY].value;
      const bool buildIsLocked  = (bool)p.lockedBuilds[p.build].achievement;
      const bool weaponIsLocked = (bool)p.lockedWeapons[p.weapon].achievement;

      CLAY({.layout{BF_CLAY_SIZING_GROW_Y}}) {
        LAMBDA (
          void,
          floatingInCenter,
          (DifficultyType difficulty,
           BuildType      build,
           WeaponType     weapon,
           LockInfo       lockInfo,
           int            overrideTier)
        )
        {
          ASSERT((int)((bool)difficulty) + (int)((bool)build) + (int)((bool)weapon) <= 1);
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

            componentUniversalCard(ComponentUniversalCardData{
              .difficulty     = difficulty,
              .build          = build,
              .weapon         = weapon,
              .hidden         = HiddenType_SHOW_LOCK,
              .lockInfo       = lockInfo,
              .overrideTier   = overrideTier,
              .setFixedHeight = true,
              .shadow         = true,
            });
          }
        };

#define BEAUTIFY_TRANSLATE_SCOPED(startedAt_, amplitude_, duration_) \
  f32 translateP = 0;                                                \
  if (startedAt_.IsSet())                                            \
    translateP = MIN(1, (startedAt_).Elapsed().Progress(duration_)); \
  Beautify b{.translate{EaseOutQuad(translateP) * (amplitude_), 0}}; \
  BEAUTIFY(b);

        const f32 translateAmplitude = CARD_WIDTH + GAP_BIG * 2 + 2 * PADDING_FRAME;

        {
          BEAUTIFY_TRANSLATE_SCOPED(
            g.ui.newRunSelectedDifficultyAt, -translateAmplitude, ANIMATION_0_FRAMES
          );
          floatingInCenter(
            (difficultyIsLocked ? DifficultyType_INVALID : p.difficulty),
            {},
            {},
            {.achievement = AchievementType_DIFFICULTY, .stepIndex = (int)p.difficulty - 2
            },
            (difficultyIsLocked ? 0 : -1)
          );
        }

        if (g.ui.newRunStep >= 1) {
          BEAUTIFY_TRANSLATE_SCOPED(
            g.ui.newRunSelectedBuildAt, translateAmplitude, ANIMATION_0_FRAMES
          );
          floatingInCenter(
            {},
            (buildIsLocked ? BuildType_INVALID : p.build),
            {},
            p.lockedBuilds[p.build],
            (buildIsLocked ? 0 : -1)
          );
        }

        if (g.ui.newRunStep >= 2) {
          floatingInCenter(
            {},
            {},
            (weaponIsLocked ? WeaponType_INVALID : p.weapon),
            p.lockedWeapons[p.weapon],
            (weaponIsLocked ? 0 : -1)
          );
        }
      }

      BF_CLAY_SPACER_VERTICAL;

      // Selecting difficulty.
      if (g.ui.newRunStep == 0) {
        componentNewRunSelections([&]() BF_FORCE_INLINE_LAMBDA {
          CLAY({})
          for (int i_ = (int)DifficultyType_D0; i_ < DifficultyType_COUNT; i_++) {
            const auto i = (DifficultyType)i_;

            const bool isLocked
              = i_ > p.achievements[AchievementType_DIFFICULTY].value + 1;
            auto fb = fb_difficulties->Get(i);

            const auto slotID = CLAY_IDI("new_run_difficulty", i);

            if (p.difficulty == i) {
              markControlAsDefault(slotID);
              if (CURRENT_CONTEXT.focused.id == 0)
                CURRENT_CONTEXT.focused = slotID;
            }

            const bool selected = (CURRENT_CONTEXT.focused.id == slotID.id);
            if (selected)
              p.difficulty = i;

            const bool chosen = componentUniversalSlot({
              .id         = slotID,
              .group      = group,
              .difficulty = (DifficultyType)(isLocked ? 0 : i),
              .hidden     = HiddenType_SHOW_LOCK,
              .tier       = (isLocked ? 0 : (int)i - 1),
              .breathing{.set = !isLocked},
              .disallowsTouch          = true,
              .touchPreservesSelection = true,
            });

            if (isLocked && chosen) {
              PlaySound(Sound_UI_ERROR);
              g.ui.newRunErrorLocked = {};
              g.ui.newRunErrorLocked.SetNow();
            }
            else if (chosen) {
              PlaySound(Sound_UI_CLICK);
              p.difficulty = i;
              Save();

              ResetFocus(currentContext);
              g.ui.newRunStep++;

              g.ui.newRunSelectedDifficultyAt = {};
              g.ui.newRunSelectedDifficultyAt.SetNow();
            }

            if (i < (int)DifficultyType_COUNT - 1)
              CLAY({.layout{.sizing{.width = CLAY_SIZING_FIXED(GAP_SMALL)}}}) {}
          }
        });
      }
      // Selecting build.
      else if (g.ui.newRunStep == 1) {
        componentNewRunSelections([&]() BF_FORCE_INLINE_LAMBDA {
          const int BUILDS_X = 5;
          const int BUILDS_Y = CeilDivision((int)fb_builds->size() - 1, BUILDS_X);

          FOR_RANGE (int, y, BUILDS_Y) {
            ControlsGroupNewRow(group);

            CLAY({.layout{.childGap = GAP_SMALL}})
            FOR_RANGE (int, x, BUILDS_X) {
              const int t = y * BUILDS_X + x;
              if (t >= fb_builds->size() - 1)
                break;

              const auto build    = (BuildType)(t + 1);
              auto       fb       = fb_builds->Get(build);
              const bool isLocked = p.lockedBuilds[build].achievement;

              const auto slotID = CLAY_IDI("new_run_build", t);

              if (p.build == build) {
                markControlAsDefault(slotID);
                if (CURRENT_CONTEXT.focused.id == 0)
                  CURRENT_CONTEXT.focused = slotID;
              }

              const bool selected = (CURRENT_CONTEXT.focused.id == slotID.id);
              if (selected)
                p.build = build;

              const bool chosen = componentUniversalSlot({
                .id     = slotID,
                .group  = group,
                .build  = (isLocked ? BuildType_INVALID : build),
                .hidden = HiddenType_SHOW_LOCK,
                .tier   = (isLocked ? 0 : -1),
                .breathing{.set = !isLocked},
                .disallowsTouch          = true,
                .touchPreservesSelection = true,
              });

              if (isLocked && chosen) {
                PlaySound(Sound_UI_ERROR);
                g.ui.newRunErrorLocked = {};
                g.ui.newRunErrorLocked.SetNow();
              }
              else if (chosen) {
                PlaySound(Sound_UI_CLICK);

                p.build = build;

                WeaponType firstWeapon{};
                bool       sameAsPreviousWeaponFound = false;

                for (auto w : *fb->starting_weapon_types()) {
                  if (!firstWeapon)
                    firstWeapon = (WeaponType)w;

                  if (p.weapon == w) {
                    sameAsPreviousWeaponFound = true;
                    break;
                  }
                }

                if (!sameAsPreviousWeaponFound)
                  p.weapon = firstWeapon;

                Save();

                ResetFocus(currentContext);
                g.ui.newRunStep++;

                g.ui.newRunSelectedBuildAt = {};
                g.ui.newRunSelectedBuildAt.SetNow();
              }
            }
          }
        });
      }
      // Selecting weapon.
      else if (g.ui.newRunStep == 2) {
        componentNewRunSelections([&]() BF_FORCE_INLINE_LAMBDA {
          const int WEAPONS_X = 8;
          const int WEAPONS_Y = CeilDivision(MAX_BUILD_WEAPONS, WEAPONS_X);

          auto fb_buildWeapons = fb_builds->Get(p.build)->starting_weapon_types();

          FOR_RANGE (int, y, WEAPONS_Y) {
            ControlsGroupNewRow(group);

            CLAY({.layout{.childGap = GAP_SMALL}})
            FOR_RANGE (int, x, WEAPONS_X) {
              const int t = y * WEAPONS_X + x;

              if (t >= fb_buildWeapons->size())
                break;

              const auto weapon   = (WeaponType)fb_buildWeapons->Get(t);
              const bool isLocked = (bool)p.lockedWeapons[weapon].achievement;

              const auto slotID = CLAY_IDI("new_run_weapon", t);

              if (p.weapon == weapon) {
                markControlAsDefault(slotID);
                if (CURRENT_CONTEXT.focused.id == 0)
                  CURRENT_CONTEXT.focused = slotID;
              }

              const bool selected = (CURRENT_CONTEXT.focused.id == slotID.id);
              if (selected)
                p.weapon = weapon;

              const bool chosen = componentUniversalSlot({
                .id     = slotID,
                .group  = group,
                .weapon = (WeaponType)(isLocked ? 0 : fb_buildWeapons->Get(t)),
                .hidden = HiddenType_SHOW_LOCK,
                .tier   = (isLocked ? 0 : -1),
                .breathing{.set = !isLocked},
                .disallowsTouch          = true,
                .touchPreservesSelection = true,
              });

              if (isLocked && chosen) {
                PlaySound(Sound_UI_ERROR);
                g.ui.newRunErrorLocked = {};
                g.ui.newRunErrorLocked.SetNow();
              }
              else if (chosen) {
                PlaySound(Sound_UI_CLICK);
                p.weapon = weapon;

                if (g.player.runsWon)
                  Metric(TextFormat("g_Run%d_Started", g.player.runsWon));

                if (p.difficulty > DifficultyType_D0) {
                  Metric(TextFormat(
                    "g_Difficulty%d_Started", (int)p.difficulty - (int)DifficultyType_D0
                  ));
                }

                Save();

                ResetFocus(currentContext);
                g.ui.newRunStep                 = 0;
                g.ui.newRunSelectedDifficultyAt = {};
                g.ui.newRunSelectedBuildAt      = {};
                g.run.reload                    = true;
              }
            }
          }
        });
      }
      else
        INVALID_PATH;

      ControlsGroupConnect(groupTop, Direction_RIGHT, groupTop);
      ControlsGroupConnect(group, Direction_DOWN, groupTop);
      ControlsGroupConnect(groupTop, Direction_DOWN, group);
      ControlsGroupConnect(group, Direction_RIGHT, group);
    }
  }
  // Picked up item.
  else if (g.run.state.screen == ScreenType_PICKED_UP_ITEM) {  ///
    SCOPED_CONTEXT(ControlsContext_PICKED_UP_ITEM);

    const auto groupTop = MakeControlsGroup();
    const auto group    = MakeControlsGroup();

    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_SCREEN_BACKGROUND,
      } BF_CLAY_CUSTOM_END,
    }) {
      componentStats2Wrapped();

      componentTopRow([&]() BF_FORCE_INLINE_LAMBDA {
        componentButtonAchievements({.group = groupTop});
        componentPlayerCoins();
        BF_CLAY_SPACER_HORIZONTAL;
        // componentButtonStats(groupTop);
        // componentButtonPause();
        componentVolumeButtons({.group = groupTop});

        componentScreenName_floatingInTheCenter(Loc_UI_ITEM_FOUND, []() {});
      });

      BF_CLAY_SPACER_VERTICAL;

      // Picked up item.
      CLAY({.layout{
        .childGap        = GAP_SMALL,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        const auto type = g.run.state.pickedUpItem.toPick;
        const auto fb   = fb_items->Get(type);

        // Item.
        componentUniversalCard(ComponentUniversalCardData{
          .item           = type,
          .affectedByGame = true,
          .setFixedHeight = true,
          .shadow         = true,
        });

        // Take and Recycle buttons.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_X,
          .childGap = GAP_BIG,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          auto       tookID = CLAY_ID("button_picked_up_item_take");
          const bool took   = componentButton(
            {.id = tookID, .group = group, .tier = 7, .breathing{.set = true}},
            [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_IMAGE({.texID = glib->ui_icon_take_texture_id()});
            }
          );
          markControlAsDefault(tookID);

          const int recyclePrice = ToRecyclePrice(
            CalculateItemOrWeaponPrice(fb->price(), fb->tier(), ItemOrWeaponType_ITEM)
          );

          const bool recycled = componentButtonRecycle({
            .id    = CLAY_ID("button_picked_up_item_recycle"),
            .group = group,
            .price = recyclePrice,
            .tier  = 4,
            .breathing{.set = true},
          });

          if (took)
            AddItem(g.run.state.pickedUpItem.toPick);
          else if (recycled)
            ChangeCoins(recyclePrice);

          if (took || recycled) {
            PlaySound(Sound_UI_CLICK);

            g.run.state.pickedUpItem = {};

            g.run.state.chests--;
            if (g.run.state.chests) {
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

      BF_CLAY_SPACER_VERTICAL;
    }

    ControlsGroupConnect(groupTop, Direction_RIGHT, groupTop);
    ControlsGroupConnect(groupTop, Direction_DOWN, group);
    ControlsGroupConnect(groupTop, Direction_UP, group);
  }
  // Upgrades.
  else if (g.run.state.screen == ScreenType_UPGRADES) {  ///
    SCOPED_CONTEXT(ControlsContext_UPGRADES);

    const auto groupTop      = MakeControlsGroup();
    const auto groupUpgrades = MakeControlsGroup();
    const auto groupReroll   = MakeControlsGroup();

    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        .childGap = GAP_BIG,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_SCREEN_BACKGROUND,
      } BF_CLAY_CUSTOM_END,
    }) {
      componentStats2Wrapped();

      componentTopRow([&]() BF_FORCE_INLINE_LAMBDA {
        componentButtonAchievements({.group = groupTop});

        componentPlayerCoins();
        BF_CLAY_SPACER_HORIZONTAL;
        // componentButtonStats(groupTop);
        // componentButtonPause();
        componentVolumeButtons({.group = groupTop});

        componentScreenName_floatingInTheCenter(Loc_UI_LEVEL_UP, []() {});
      });

      BF_CLAY_SPACER_VERTICAL;
      BF_CLAY_SPACER_VERTICAL;

      // Upgrades.
      CLAY({.layout{.childGap = GAP_SMALL}}) {
        const auto fb_stats = glib->stats();

        FOR_RANGE (int, i, g.run.state.upgrades.toPick.count) {
          const auto upgrade = g.run.state.upgrades.toPick[i];
          const auto fb      = fb_stats->Get(upgrade.stat);
          CLAY({
            .layout{
              .sizing{
                CLAY_SIZING_FIXED(UPGRADE_FRAME_WIDTH + 2 * PADDING_FRAME),
                CLAY_SIZING_FIT(245),
              },
              BF_CLAY_PADDING_ALL(PADDING_FRAME),
              .childGap = GAP_SMALL,
              // BF_CLAY_CHILD_ALIGNMENT_CENTER_TOP,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            BF_CLAY_CUSTOM_BEGIN{
              BF_CLAY_CUSTOM_SHADOW(glib->ui_frame_shadow_small_nine_slice(), true),
              BF_CLAY_CUSTOM_NINE_SLICE(
                glib->ui_frame_nine_slice(), upgrade.tier, true, {.set = false}
              ),
            } BF_CLAY_CUSTOM_END,
          }) {
            CLAY({.layout{.childGap = GAP_SMALL}}) {
              // Slot with upgrade's image.
              componentSlot({.tier = upgrade.tier}, [&]() BF_FORCE_INLINE_LAMBDA {
                componentCenterFloater([&]() BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_IMAGE({.texID = fb->big_icon_texture_id()});
                });
              });

              CLAY({.layout{
                .childGap        = GAP_SMALL,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              }}) {
                // "Upgrade" label.
                BF_CLAY_TEXT_BROKEN_LOCALIZED(
                  Loc_UI_UPGRADE, {.color = textColorsPerTier[upgrade.tier]}
                );
              }
            }

            BF_CLAY_SPACER_VERTICAL;

            // Stat + amount.
            int amount = fb->upgrade_values()->Get(upgrade.tier);

            IterateOverEffects(
              EffectConditionType_X__PERCENT_MORE_STATS_GAINED_FROM_UPGRADES,
              -1,
              [&](
                Weapon* w,
                int     wi,
                auto    fb_effect,
                int     tierOffset,
                int     times,
                int     thisWaveAddedCount
              ) BF_FORCE_INLINE_LAMBDA {
                amount += Ceil(amount * (f32)(EFFECT_X_INT * times) / 100.0f);
              }
            );

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
                BF_CLAY_TEXT(TextFormat("+%d", amount), {.color = palTextGreen});

                // BF_CLAY_IMAGE({.texID = fb->small_icon_texture_id()});

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
                  BF_CLAY_TEXT_BROKEN_LOCALIZED(fb->name_locale());
                  FlexEnd();
                }
              }
            }

            BF_CLAY_SPACER_VERTICAL;

            // Choose button.
            CLAY({.layout{BF_CLAY_SIZING_GROW_X, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}
            ) {
              SDL_Scancode keys_[]{(SDL_Scancode)((int)SDL_SCANCODE_1 + i)};
              VIEW_FROM_ARRAY_DANGER(keys);

              if (g.meta.showingStats.IsSet())
                keys.count = 0;

              auto chooseButtonID = CLAY_IDI("button_upgrades_choose", i);
              bool chosen         = componentButton(
                {
                          .id    = chooseButtonID,
                          .group = groupUpgrades,
                          .keys  = keys,
                          .tier  = upgrade.tier,
                          .breathing{.set = true},
                },
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_IMAGE({.texID = glib->ui_icon_take_texture_id()});
                }
              );

              if (i == 1)
                markControlAsDefault(chooseButtonID);

              if (chosen) {
                PlaySound(Sound_UI_CLICK);

                if (g.run.state.levelOnStartOfTheWave
                    < g.run.state.staticStats[StatType_LEVEL])
                {
                  g.run.state.levelOnStartOfTheWave++;
                  g.run.scheduledUpgrades      = true;
                  g.run.scheduledUpgradesReset = true;
                }
                else {
                  g.run.scheduledShop      = true;
                  g.run.scheduledShopReset = true;
                }

                ChangeStaticAndDynamicStatBy(upgrade.stat, amount);

                if (upgrade.stat == StatType_HP)
                  PLAYER_CREATURE.health += amount;

                Save();
              }
            }
          }
        }
      }

      BF_CLAY_SPACER_VERTICAL;

      // Reroll button.
      const auto calculatedRerollPrice = g.run.state.upgrades.rerolls.GetPrice();
      const bool canReroll             = (calculatedRerollPrice <= PLAYER_COINS);
      bool       rerolled = componentButtonReroll(groupReroll, calculatedRerollPrice);
      if (rerolled) {
        if (canReroll) {
          PlaySound(Sound_UI_CLICK);

          g.run.state.upgrades.rerolls.Roll();
          RefillUpgradesToPick(true);
          Save();
        }
        else {
          g.ui.errorGold = {};
          g.ui.errorGold.SetNow();

          PlaySound(Sound_UI_ERROR);
        }
      }

      BF_CLAY_SPACER_VERTICAL;
      BF_CLAY_SPACER_VERTICAL;
    }

    ControlsGroupConnect(groupTop, Direction_RIGHT, groupTop);
    ControlsGroupConnect(groupTop, Direction_DOWN, groupUpgrades);
    ControlsGroupConnect(groupUpgrades, Direction_RIGHT, groupUpgrades);
    ControlsGroupConnect(groupUpgrades, Direction_DOWN, groupReroll);
    ControlsGroupConnect(groupReroll, Direction_DOWN, groupTop);
  }
  // Shop.
  else if (g.run.state.screen == ScreenType_SHOP) {  ///
    SCOPED_CONTEXT(ControlsContext_SHOP);

    ControlsGroupID groupsToBuy_[SHOP_SELLING_ITEMS]{};
    for (auto& x : groupsToBuy_)
      x = MakeControlsGroup();
    VIEW_FROM_ARRAY_DANGER(groupsToBuy);
    const auto groupGoNextWave = MakeControlsGroup();
    const auto groupWeapons    = MakeControlsGroup();
    const auto groupItems      = MakeControlsGroup();
    const auto groupItemArrows = MakeControlsGroup();
    const auto groupTop        = MakeControlsGroup();
    const auto groupTop2       = MakeControlsGroup();

    // Columns.
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        .childGap = GAP_BIG,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_SCREEN_BACKGROUND,
      } BF_CLAY_CUSTOM_END,
    }) {
      // Left column that contains:
      // 1. wave, coins, reroll.
      // 2. items to buy.
      // 3. player's items and weapons.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        // 1. Wave, coins, reroll.
        componentTopRow([&]() BF_FORCE_INLINE_LAMBDA {
          componentButtonAchievements({.group = groupTop2});

          // Coins.
          componentPlayerCoins();

          BF_CLAY_SPACER_HORIZONTAL;

          CLAY({.layout{.childGap = GAP_SMALL}}) {
            // componentButtonPause();
            componentVolumeButtons({.group = groupTop2});
          }

          // Reroll button.
          CLAY({.layout{.childGap = GAP_BIG}}) {
            const auto calculatedRerollPrice = g.run.state.shop.rerolls.GetPrice();
            const bool canReroll             = (calculatedRerollPrice <= PLAYER_COINS);

            bool rerolled = componentButtonReroll(groupTop, calculatedRerollPrice);
            if (rerolled) {
              if (canReroll) {
                PlaySound(Sound_UI_CLICK);
                g.run.state.shop.rerolls.Roll();

                AchievementMax(
                  AchievementType_REROLL_N_TIMES_IN_A_ROW_IN_SHOP,
                  g.run.state.shop.rerolls.rerolledFreeTimes
                    + g.run.state.shop.rerolls.rerolledTimes
                );

                RefillShopToPick();
                Save();
              }
              else {
                g.ui.errorGold = {};
                g.ui.errorGold.SetNow();
                PlaySound(Sound_UI_ERROR);
              }
            }

            // Stats.
            // componentButtonStats(groupTop);
            // componentStats2Wrapped(
            //   {
            //     .element = CLAY_ATTACH_POINT_RIGHT_TOP,
            //     .parent  = CLAY_ATTACH_POINT_RIGHT_BOTTOM,
            //   },
            //   {GAP_FLEX, 32}
            // );
          }

          componentScreenName_floatingInTheCenter(Loc_UI_SHOP, []() {});
        });

        BF_CLAY_SPACER_VERTICAL;

        // 2. Items to buy.
        CLAY({.layout{BF_CLAY_SIZING_GROW_X}}) {
          int toPickIndex = -1;

          const int selectNextIndices[SHOP_SELLING_ITEMS][3]{
            {1, 2, 3}, {2, 0, 3}, {3, 1, 0}, {2, 1, 0}
          };

          BF_CLAY_SPACER_HORIZONTAL;

          for (auto& x : g.run.state.shop.toPick) {
            toPickIndex++;

            Clay_ElementId changeToID = rerollID;
            FOR_RANGE (int, k, 3) {
              const int changeToIndex = selectNextIndices[toPickIndex][k];
              ASSERT(changeToIndex != toPickIndex);
              const auto& vv = g.run.state.shop.toPick[changeToIndex];
              if (vv.item || vv.weapon) {
                changeToID = getIDFromShopBuyingIndex(changeToIndex);
                break;
              }
            }

            componentUniversalCard(ComponentUniversalCardData{
              .item                 = x.item,
              .weapon               = x.weapon,
              .hidden               = HiddenType_HIDE_IF_EMPTY,
              .affectedByGame       = true,
              .overrideTier         = x.tier,
              .setFixedHeight       = true,
              .shopGroup            = groupsToBuy[toPickIndex],
              .shopBuyingIndex      = toPickIndex,
              .shopFocusAfterBuying = changeToID,
              .shadow               = true,
            });
            if (toPickIndex < g.run.state.shop.toPick.count - 1)
              CLAY({.layout{.sizing{.width = CLAY_SIZING_FIXED(GAP_BIG)}}}) {}
          }

          BF_CLAY_SPACER_HORIZONTAL;

          CLAY({.layout{.sizing{
            .width  = CLAY_SIZING_FIXED(GAP_BIG * 1.0f),
            .height = CLAY_SIZING_GROW(0),
          }}}) {
            componentStats2Wrapped(
              {
                .element = CLAY_ATTACH_POINT_RIGHT_CENTER,
                .parent  = CLAY_ATTACH_POINT_RIGHT_CENTER,
              },
              {}
            );
          }

          for (auto i : DEFAULT_BUYING_INDICES) {
            const auto& x = g.run.state.shop.toPick[i];
            if (x.weapon || x.item)
              markControlAsDefault(getIDFromShopBuyingIndex(i));
          }

          markControlAsDefault(rerollID);
        }

        BF_CLAY_SPACER_VERTICAL;

        // 3. Player's items and weapons.
        CLAY({.layout{BF_CLAY_SIZING_GROW_X, .childGap = GAP_BIG}}) {
          CLAY({.layout{
            .childGap        = GAP_SMALL,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          }}) {
            // Items label.
            BF_CLAY_TEXT_LOCALIZED(Loc_UI_ITEMS__CAPS);
            // Items.
            componentItemsGrid({
              .group        = groupItems,
              .groupArrows  = groupItemArrows,
              .itemsX       = 11,
              .itemsMaxY    = 2,
              .scroll       = &g.run.itemsScrollShop,
              .detailsRight = 1,
              .detailsBelow = 0,
            });
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

              BF_CLAY_TEXT_LOCALIZED(
                Loc_UI_WEAPONS__CAPS,
                {.color = (weaponsCount > 0 ? palTextWhite : TRANSPARENT_BLACK)}
              );

              // BF_CLAY_TEXT(
              //   TextFormat(" (%d/%d)", weaponsCount, g.run.state.weapons.count),
              //   (weaponsCount > 0 ? palTextWhite : TRANSPARENT_BLACK)
              // );
            }

            CLAY({.layout{.sizing{.height = CLAY_SIZING_FIXED(GAP_SMALL)}}}) {}

            // Weapons.
            CLAY({}) {
              BEAUTIFY_WIGGLING_DANGER_SCOPED(
                g.ui.shopErrorWeapons,
                WEAPONS_WIGGLING_LOGICAL_AMPLITUDE,
                ERROR_WIGGLING_FRAMES,
                ERROR_WIGGLING_TIMES
              );

              componentWeaponsGrid({
                .group        = groupWeapons,
                .weaponsX     = 3,
                .detailsRight = 0,
                .detailsBelow = 0,
                .weAreInShop  = 1,
              });
            }
          }

          // Advance to the next wave button.
          CLAY({.layout{
            BF_CLAY_SIZING_GROW_Y,
            BF_CLAY_CHILD_ALIGNMENT_CENTER_BOTTOM,
          }}) {
            SDL_Scancode keys_[]{SDL_SCANCODE_E, SDL_SCANCODE_END};
            VIEW_FROM_ARRAY_DANGER(keys);

            if (g.meta.showingStats.IsSet() || (g.run.shopActivatedModalWeaponIndex >= 0))
              keys.count = 0;

            const int  nextWaveNumber = g.run.state.waveIndex + 2;
            const bool nextIsBoss     = (nextWaveNumber == TOTAL_WAVES);

            const bool nextWavePressed = componentButton(
              {
                .id    = CLAY_ID("button_shop_next_wave"),
                .group = groupGoNextWave,
                .growX = true,
                .keys  = keys,
                .tier  = (nextIsBoss ? 4 : 7),
                .breathing{.set = true},
              },
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_IMAGE(
                  {.texID = glib->ui_icon_go_next_wave2_texture_id()},
                  [&]() BF_FORCE_INLINE_LAMBDA {
                    CLAY({
                      .layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER},
                      .floating{
                        .zIndex = zIndex,
                        .attachPoints{
                          .element = CLAY_ATTACH_POINT_CENTER_BOTTOM,
                          .parent  = CLAY_ATTACH_POINT_CENTER_BOTTOM,
                        },
                        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                        .attachTo           = CLAY_ATTACH_TO_PARENT,
                      },
                    }) {
                      FLOATING_BEAUTIFY;
                      FontBegin(&g.meta.fontUINextWave);

                      auto color = palTextPaleYellow;
                      BF_CLAY_TEXT(TextFormat("%d", nextWaveNumber), {.color = color});
                      BF_CLAY_TEXT(TextFormat("/%d", TOTAL_WAVES), {.color = color});
                      FontEnd();
                    }
                  }
                );
              }
            );

            if (nextWavePressed) {
              ShowInterAd();

              g.run.scheduledNextWave = true;
              PlaySound(Sound_UI_CLICK);
              PlaySound(Sound_GAME_WAVE_START);

              if (!g.player.runsWon)
                Metric(TextFormat("g_Run0_Shop_ToWave%d", g.run.state.waveIndex + 1));

              for (auto& x : g.run.state.shop.toPick)
                x = {};
            }
          }
        }
      }
    }

    ControlsGroupConnect(groupsToBuy[0], Direction_RIGHT, groupsToBuy[1]);
    ControlsGroupConnect(groupsToBuy[1], Direction_RIGHT, groupsToBuy[2]);
    ControlsGroupConnect(groupsToBuy[2], Direction_RIGHT, groupsToBuy[3]);
    ControlsGroupConnect(groupsToBuy[0], Direction_RIGHT, groupsToBuy[2]);
    ControlsGroupConnect(groupsToBuy[1], Direction_RIGHT, groupsToBuy[3]);
    ControlsGroupConnect(groupsToBuy[0], Direction_RIGHT, groupsToBuy[3]);

    ControlsGroupConnect(groupTop, Direction_RIGHT, groupTop2);
    ControlsGroupConnect(groupTop, Direction_LEFT, groupTop2);

    ControlsGroupConnect(groupsToBuy[3], Direction_UP, groupTop);
    ControlsGroupConnect(groupsToBuy[2], Direction_UP, groupTop);
    ControlsGroupConnect(groupsToBuy[1], Direction_UP, groupTop);
    ControlsGroupConnect(groupsToBuy[0], Direction_UP, groupTop);
    ControlsGroupConnect(groupsToBuy[3], Direction_UP, groupTop2);
    ControlsGroupConnect(groupsToBuy[2], Direction_UP, groupTop2);
    ControlsGroupConnect(groupsToBuy[1], Direction_UP, groupTop2);
    ControlsGroupConnect(groupsToBuy[0], Direction_UP, groupTop2);

    ControlsGroupConnect(groupsToBuy[3], Direction_RIGHT, groupsToBuy[0]);
    ControlsGroupConnect(groupsToBuy[3], Direction_RIGHT, groupsToBuy[1]);
    ControlsGroupConnect(groupsToBuy[3], Direction_RIGHT, groupsToBuy[2]);

    ControlsGroupConnect(groupItems, Direction_UP, groupsToBuy[0]);
    ControlsGroupConnect(groupItems, Direction_UP, groupsToBuy[1]);
    ControlsGroupConnect(groupItems, Direction_UP, groupsToBuy[2]);
    ControlsGroupConnect(groupWeapons, Direction_UP, groupsToBuy[3]);
    ControlsGroupConnect(groupItems, Direction_UP, groupsToBuy[3]);
    ControlsGroupConnect(groupWeapons, Direction_UP, groupsToBuy[2]);
    ControlsGroupConnect(groupWeapons, Direction_UP, groupsToBuy[1]);
    ControlsGroupConnect(groupWeapons, Direction_UP, groupsToBuy[0]);
    ControlsGroupConnect(groupTop, Direction_DOWN, groupGoNextWave);
    ControlsGroupConnect(groupTop2, Direction_DOWN, groupGoNextWave);
    ControlsGroupConnect(groupWeapons, Direction_UP, groupTop);
    ControlsGroupConnect(groupItems, Direction_UP, groupTop);
    ControlsGroupConnect(groupWeapons, Direction_UP, groupTop2);
    ControlsGroupConnect(groupItems, Direction_UP, groupTop2);
    ControlsGroupConnect(groupWeapons, Direction_RIGHT, groupGoNextWave);
    ControlsGroupConnect(groupItems, Direction_RIGHT, groupItemArrows);
    ControlsGroupConnect(groupItemArrows, Direction_RIGHT, groupWeapons);
    ControlsGroupConnect(groupItems, Direction_RIGHT, groupWeapons);
    ControlsGroupConnect(groupItems, Direction_RIGHT, groupGoNextWave);
    ControlsGroupConnect(groupsToBuy[3], Direction_DOWN, groupGoNextWave);
    ControlsGroupConnect(groupsToBuy[2], Direction_DOWN, groupGoNextWave);
    ControlsGroupConnect(groupsToBuy[1], Direction_DOWN, groupGoNextWave);
    ControlsGroupConnect(groupsToBuy[0], Direction_DOWN, groupGoNextWave);

    ControlsGroupConnect(groupItems, Direction_LEFT, groupGoNextWave);

    ControlsGroupConnect(groupTop, Direction_UP, groupGoNextWave);
    ControlsGroupConnect(groupTop2, Direction_UP, groupGoNextWave);
    ControlsGroupConnect(groupItems, Direction_DOWN, groupTop);
    ControlsGroupConnect(groupItems, Direction_DOWN, groupTop2);
    ControlsGroupConnect(groupWeapons, Direction_DOWN, groupTop);
    ControlsGroupConnect(groupWeapons, Direction_DOWN, groupTop2);

    ControlsGroupConnect(groupTop, Direction_RIGHT, groupTop);
    ControlsGroupConnect(groupTop2, Direction_RIGHT, groupTop2);
  }
  // End.
  else if (g.run.state.screen == ScreenType_END) {  ///
    SCOPED_CONTEXT(ControlsContext_END);

#define BF_SCREEN_END_STATS_COLUMN_STICK_TO_RIGHT 1

    const auto groupTop             = MakeControlsGroup();
    const auto groupWeaponsAndItems = MakeControlsGroup();
    const auto groupItemArrows      = MakeControlsGroup();
    const auto groupButtons         = MakeControlsGroup();

    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        .childGap = GAP_BIG,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_SCREEN_BACKGROUND,
      } BF_CLAY_CUSTOM_END,
    }) {
#if BF_SCREEN_END_STATS_COLUMN_STICK_TO_RIGHT
      componentStats2Wrapped();
#endif

      // Header. Run Won / Lost, Wave.
      componentTopRow([&]() BF_FORCE_INLINE_LAMBDA {
        const int locale = (g.run.state.won ? Loc_UI_WON : Loc_UI_LOST);

        componentScreenName_floatingInTheCenter(
          (Loc)locale,
          [&]() BF_FORCE_INLINE_LAMBDA {
            if (!g.run.state.won) {
              // Wave.
              BF_CLAY_TEXT(". ");
              BF_CLAY_TEXT_LOCALIZED(Loc_UI_WAVE__CAPS);
              BF_CLAY_TEXT(TextFormat(" %d", g.run.state.waveIndex + 1));
            }
          }
        );

        componentButtonAchievements({.group = groupTop});
        BF_CLAY_SPACER_HORIZONTAL;
        componentVolumeButtons({.group = groupTop});
        // componentButtonPause();
        // componentButtonStats(groupTop);
      });

      BF_CLAY_SPACER_VERTICAL;
      BF_CLAY_SPACER_VERTICAL;

      // Main. Weapons, items.
      CLAY({.layout{
        .childGap = GAP_BIG,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        // Player's weapons and items.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_XY,
          .childGap        = GAP_BIG,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          int weaponsCount = 0;
          for (auto& weapon : g.run.state.weapons) {
            if (weapon.type)
              weaponsCount++;
          }

          if (weaponsCount > 0) {
            CLAY({.layout{
              .childGap        = GAP_SMALL,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              // Weapons label.
              BF_CLAY_TEXT_LOCALIZED(Loc_UI_WEAPONS__CAPS);

              // Weapons.
              componentWeaponsGrid({
                .group        = groupWeaponsAndItems,
                .weaponsX     = 10,
                .detailsRight = 1,
                .detailsBelow = 1,
              });
            }
          }

          CLAY({.layout{
            .childGap        = GAP_SMALL,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          }}) {
            // Items label.
            BF_CLAY_TEXT_LOCALIZED(Loc_UI_ITEMS__CAPS);
            // Items.
            componentItemsGrid({
              .group        = groupWeaponsAndItems,
              .groupArrows  = groupItemArrows,
              .itemsX       = 10,
              .itemsMaxY    = 3,
              .scroll       = &g.run.itemsScrollEnd,
              .detailsRight = 1,
              .detailsBelow = 0,
            });
          }
        }
      }

#if !BF_SCREEN_END_STATS_COLUMN_STICK_TO_RIGHT
      componentStats2Wrapped(
        {
          .element = CLAY_ATTACH_POINT_LEFT_CENTER,
          .parent  = CLAY_ATTACH_POINT_RIGHT_CENTER,
        },
        {GAP_BIG * 3, 0}
      );
#endif

      BF_CLAY_SPACER_VERTICAL;

      // Footer. Restart.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        .childGap = GAP_SMALL,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        bool restarted = false;
        if (0 && !g.run.state.won) {
          restarted = componentOutlinedTextButton(
            {
              .id    = CLAY_ID("button_end_restart"),
              .group = groupButtons,
              .tier  = 7,
              .breathing{.set = true},
            },
            [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT_LOCALIZED(Loc_UI_RESTART__CAPS, {.color = textColor});
            }
          );
        }

        const auto newRunID = CLAY_ID("button_end_new_run");

        const bool newRun = componentOutlinedTextButton(
          {
            .id    = newRunID,
            .group = groupButtons,
            .tier  = 7,
            .breathing{.set = true},
          },
          [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED(Loc_UI_NEW_RUN__CAPS, {.color = textColor});
          }
        );

        markControlAsDefault(newRunID);

        if (restarted)
          g.run.reload = true;
        if (newRun)
          g.run.scheduledNewRun = true;

        if (restarted || newRun)
          PlaySound(Sound_UI_CLICK);
      }

      BF_CLAY_SPACER_VERTICAL;
    }

    ControlsGroupConnect(groupTop, Direction_RIGHT, groupTop);
    ControlsGroupConnect(groupTop, Direction_DOWN, groupWeaponsAndItems);
    ControlsGroupConnect(groupWeaponsAndItems, Direction_DOWN, groupButtons);
    ControlsGroupConnect(groupWeaponsAndItems, Direction_RIGHT, groupItemArrows);
    ControlsGroupConnect(groupItemArrows, Direction_RIGHT, groupWeaponsAndItems);
    ControlsGroupConnect(groupButtons, Direction_DOWN, groupTop);
  }
  else if (g.run.state.screen == ScreenType_WAVE_END_ANIMATION) {
    // NOTE: Intentionally left blank.
  }
  else if (g.run.state.screen == ScreenType_GAMEPLAY) {
    // NOTE: Intentionally left blank.
  }
  else
    INVALID_PATH;

  // Pause.
  if (g.meta.paused && !g.meta.showingAchievements) {  ///
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
      },
      .floating{
        .zIndex = zIndex,
        .attachPoints{
          .element = CLAY_ATTACH_POINT_CENTER_CENTER,
          .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
        },
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_SCREEN_BACKGROUND,
      } BF_CLAY_CUSTOM_END,
    }) {
      FLOATING_BEAUTIFY;

      SCOPED_CONTEXT(ControlsContext_MODAL_PAUSE);

      componentStats2Wrapped();

      const auto groupTop             = MakeControlsGroup();
      const auto groupButtons         = MakeControlsGroup();
      const auto groupWeaponsAndItems = MakeControlsGroup();
      const auto groupItemArrows      = MakeControlsGroup();

      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        componentTopRow([&]() BF_FORCE_INLINE_LAMBDA {
          componentWaveAndTimerInCenterTopOfTheScreen();

          componentButtonAchievements({.group = groupTop});
          BF_CLAY_SPACER_HORIZONTAL;
          componentVolumeButtons({.group = groupTop});
          // componentButtonStats(groupTop);
        });

        BF_CLAY_SPACER_VERTICAL;

        // Columns. Wave + buttons, weapons + items, stats.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_XY,
          .childGap = GAP_BIG * 2,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          // 1. Buttons.
          CLAY({.layout{
            .sizing{.width = CLAY_SIZING_FIXED(320)},
            .childGap = GAP_BIG,
            BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          }}) {
            CLAY({}) {
              BF_CLAY_TEXT_LOCALIZED(Loc_UI_WAVE, {.color = TRANSPARENT_BLACK});
              BF_CLAY_TEXT(
                TextFormat(" %d", g.run.state.waveIndex + 1), {.color = TRANSPARENT_BLACK}
              );
            }

            CLAY({.layout{
              BF_CLAY_SIZING_GROW_X,
              .childGap        = GAP_BIG,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }}) {
              const auto resumeButtonID = CLAY_ID("button_pause_resume");
              markControlAsDefault(resumeButtonID);
              if (!controlsContexts[currentContext].focused.id)
                controlsContexts[currentContext].focused = resumeButtonID;

              const bool resumed = componentOutlinedTextButton(
                {
                  .id    = resumeButtonID,
                  .group = groupButtons,
                  .growX = true,
                  .keys  = KEYS_PAUSE,
                  .tier  = 7,
                },
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED(Loc_UI_RESUME__CAPS, {.color = textColor});
                }
              );

              ControlsGroupNewRow(groupButtons);

              bool restarted = false;
#if 0
                restarted = componentOutlinedTextButton(
                  {
                    .id    = CLAY_ID("button_pause_restart"),
                    .group = groupButtons,
                    .growX = true,
                  },
                  [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                    BF_CLAY_TEXT_LOCALIZED(Loc_UI_RESTART__CAPS, {.color = textColor});
                  }
                );
#endif

              ControlsGroupNewRow(groupButtons);

              const bool newRun = componentOutlinedTextButton(
                {
                  .id    = CLAY_ID("button_pause_new_run"),
                  .group = groupButtons,
                  .growX = true,
                },
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED(Loc_UI_NEW_RUN__CAPS, {.color = textColor});
                }
              );

              ControlsGroupNewRow(groupButtons);

              bool achievements = false;
#if 0
                achievements = componentOutlinedTextButton(
                  {
                    .id    = CLAY_ID("button_pause_achievements"),
                    .group = groupButtons,
                    .growX = true,
                  },
                  [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                    BF_CLAY_TEXT_LOCALIZED(
                      Loc_UI_ACHIEVEMENTS__CAPS, {.color = textColor}
                    );
                    int percent = GetAchievementsCompletedPercent();
                    if (percent > 0)
                      BF_CLAY_TEXT(TextFormat(" %d%%", percent), {.color = textColor});
                  }
                );
#endif

              bool quit = false;
#if defined(SDL_PLATFORM_DESKTOP)
              ControlsGroupNewRow(groupButtons);

              quit = componentOutlinedTextButton(
                {
                  .id    = CLAY_ID("button_pause_quit"),
                  .group = groupButtons,
                  .growX = true,
                },
                [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED(Loc_UI_QUIT__CAPS, {.color = textColor});
                }
              );
#endif

              if (resumed)
                g.meta.scheduledTogglePause = true;
              if (restarted)
                g.meta.confirmingRestart = true;
              if (newRun)
                g.meta.confirmingNewRun = true;
              if (achievements)
                g.meta.showingAchievements = true;
              if (quit)
                ge.meta.quitRequested = true;

              if (resumed || restarted || newRun || achievements || quit)
                PlaySound(Sound_UI_CLICK);
            }
          }

          // Weapons + items.
          int weaponsCount = 0;
          for (auto& weapon : g.run.state.weapons) {
            if (weapon.type)
              weaponsCount++;
          }

          CLAY({.layout{
            .childGap        = GAP_BIG,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          }}) {
            if (weaponsCount > 0) {
              // Weapons label.
              BF_CLAY_TEXT_LOCALIZED(Loc_UI_WEAPONS__CAPS);

              // Weapons.
              componentWeaponsGrid({
                .group        = groupWeaponsAndItems,
                .weaponsX     = 6,
                .detailsRight = 1,
                .detailsBelow = 1,
              });
            }

            // Items label.
            BF_CLAY_TEXT_LOCALIZED(Loc_UI_ITEMS__CAPS);
            // Items.
            componentItemsGrid({
              .group        = groupWeaponsAndItems,
              .groupArrows  = groupItemArrows,
              .itemsX       = 6,
              .itemsMaxY    = 4,
              .scroll       = &g.run.itemsScrollPause,
              .detailsRight = 1,
              .detailsBelow = 0,
            });
          }
        }

        BF_CLAY_SPACER_VERTICAL;
      }

      ControlsGroupConnect(groupTop, Direction_RIGHT, groupTop);
      ControlsGroupConnect(groupTop, Direction_DOWN, groupButtons);
      ControlsGroupConnect(groupButtons, Direction_UP, groupTop);
      ControlsGroupConnect(groupButtons, Direction_DOWN, groupTop);
      ControlsGroupConnect(groupButtons, Direction_RIGHT, groupWeaponsAndItems);
      ControlsGroupConnect(groupWeaponsAndItems, Direction_UP, groupTop);
      ControlsGroupConnect(groupWeaponsAndItems, Direction_DOWN, groupTop);
      ControlsGroupConnect(groupWeaponsAndItems, Direction_RIGHT, groupItemArrows);
      ControlsGroupConnect(groupItemArrows, Direction_RIGHT, groupWeaponsAndItems);
    }
  }

  // Achievements.
  if (g.meta.showingAchievements) {  ///
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
      },
      .floating{
        .zIndex = zIndex,
        .attachPoints{
          .element = CLAY_ATTACH_POINT_CENTER_CENTER,
          .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
        },
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_SCREEN_BACKGROUND,
      } BF_CLAY_CUSTOM_END,
    }) {
      FLOATING_BEAUTIFY;
      SCOPED_CONTEXT(ControlsContext_ACHIEVEMENTS);

      const auto groupTop  = MakeControlsGroup();
      const auto groupGrid = MakeControlsGroup();

      CLAY({.layout{
        BF_CLAY_SIZING_GROW_XY,
        .childGap = GAP_BIG,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        const auto fb_order   = glib->achievements_order();
        int        totalSlots = fb_order->size();

        int shownSlots = 0;

        // "Achievements" label.
        componentTopRow([&]() BF_FORCE_INLINE_LAMBDA {
          if (componentButtonBack({.group = groupTop, .markControlAsDefault = true})) {
            g.meta.showingAchievements                = false;
            g.meta.achievementsHoveredAchievement     = 0;
            g.meta.achievementsHoveredAchievementStep = 0;
          }

          componentScreenName_floatingInTheCenter(
            Loc_UI_ACHIEVEMENTS,
            [&]() BF_FORCE_INLINE_LAMBDA {
              int percent = GetAchievementsCompletedPercent();

              if (percent > 0) {
                auto color = palTextBezhevy;
                if (percent >= 100)
                  color = palTextGreen;
                BF_CLAY_TEXT(TextFormat(" %d%%", percent), {.color = color});
              }
            }
          );
        });

        BF_CLAY_SPACER_VERTICAL;

        const int achievementsY = CeilDivision((int)totalSlots, ACHIEVEMENTS_X);

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
                BF_CLAY_PADDING_ALL(PADDING_FRAME),
                .childGap        = GAP_SMALL,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              },
              BF_CLAY_CUSTOM_BEGIN{
                BF_CLAY_CUSTOM_SHADOW(glib->ui_frame_shadow_small_nine_slice(), true),
                BF_CLAY_CUSTOM_NINE_SLICE(
                  glib->ui_frame_nine_slice(), 0, true, {.set = false}
                ),
              } BF_CLAY_CUSTOM_END,
            }) {
              g.meta.achievementsHoveredAchievement     = 0;
              g.meta.achievementsHoveredAchievementStep = 0;

              FOR_RANGE (int, y, achievementsY) {
                ControlsGroupNewRow(groupGrid);

                CLAY({.layout{.childGap = GAP_SMALL}})
                FOR_RANGE (int, x, ACHIEVEMENTS_X) {
                  if (shownSlots >= totalSlots)
                    break;

                  const auto t   = y * ACHIEVEMENTS_X + x;
                  const auto fb_ = fb_order->Get(t);
                  const auto currentAchievement
                    = (AchievementType)fb_->achievement_type();
                  const int currentStep = fb_->step_index();

                  const bool isLocked
                    = IsAchievementStepLocked(currentAchievement, currentStep);

                  const auto fb      = fb_achievements->Get(currentAchievement);
                  const auto fb_step = fb->steps()->Get(currentStep);

                  bool canHover = true;

                  CLAY({}) {
                    int texID = 0;
                    int tier  = 0;

                    if (isLocked)
                      texID = glib->ui_item_locked_texture_id();
                    else
                      tier = 3;

                    auto id = CLAY_IDI(
                      "paused_achievement", 100 * (int)currentAchievement + currentStep
                    );

                    componentUniversalSlot({
                      .id    = id,
                      .group = groupGrid,
                      .build = (BuildType)(isLocked ? 0 : fb_step->unlocks_build_type()),
                      .item  = (ItemType)(isLocked ? 0 : fb_step->unlocks_item_type()),
                      .weapon
                      = (WeaponType)(isLocked ? 0 : fb_step->unlocks_weapon_type()),
                      .hidden   = HiddenType_SHOW_LOCK,
                      .tier     = tier,
                      .canHover = canHover,
                    });

                    if ((canHover                                   //
                         && !g.meta.playerUsesKeyboardOrController  //
                         && Clay_Hovered())
                        || (id.id == controlsContexts[currentContext].focused.id))
                    {
                      g.meta.achievementsHoveredAchievement     = currentAchievement;
                      g.meta.achievementsHoveredAchievementStep = currentStep;
                    }
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

            if (g.meta.achievementsHoveredAchievement) {
              fb       = fb_achievements->Get(g.meta.achievementsHoveredAchievement);
              fb_step  = fb->steps()->Get(g.meta.achievementsHoveredAchievementStep);
              isLocked = IsAchievementStepLocked(
                (AchievementType)g.meta.achievementsHoveredAchievement,
                g.meta.achievementsHoveredAchievementStep
              );
            }

            // Achievement's name and description.
            componentAchievement(
              (AchievementType)g.meta.achievementsHoveredAchievement,
              g.meta.achievementsHoveredAchievementStep,
              false
            );

            // Achievement's reward.
            componentUniversalCard(ComponentUniversalCardData{
              .build
              = (BuildType)((fb_step && !isLocked) ? fb_step->unlocks_build_type() : 0),
              .item
              = (ItemType)((fb_step && !isLocked) ? fb_step->unlocks_item_type() : 0),
              .weapon
              = (WeaponType)((fb_step && !isLocked) ? fb_step->unlocks_weapon_type() : 0),
              .hidden         = HiddenType_SHOW_LOCK,
              .setFixedHeight = true,
              .shadow         = true,
            });
          }
        }

        BF_CLAY_SPACER_VERTICAL;
      }

      ControlsGroupConnect(groupTop, Direction_RIGHT, groupTop);
      ControlsGroupConnect(groupTop, Direction_UP, groupGrid);
      ControlsGroupConnect(groupGrid, Direction_UP, groupTop);
    }
  }

  auto groupStats = MakeControlsGroup();

  // Stats.
  if (g.meta.showingStats.IsSet()) {  ///
    SCOPED_CONTEXT(ControlsContext_MODAL_STATS);

    zIndex += UIZIndexOffset_STATS;

    bool closeStats = false;

    const f32 p = MIN(1, g.meta.showingStats.Elapsed().Progress(ANIMATION_0_FRAMES));

    // TODO: move inside component below? AND CHANGE `pointerCaptureMode`.
    componentOverlay(
      [&]() BF_FORCE_INLINE_LAMBDA {
        if (clickOrTouchPressed())
          closeStats |= true;
      },
      EaseOutQuad(p)
    );

    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        BF_CLAY_CHILD_ALIGNMENT_RIGHT_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      .floating{
        .zIndex = zIndex,
        .attachPoints{
          .element = CLAY_ATTACH_POINT_CENTER_CENTER,
          .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
        },
        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
        .attachTo           = CLAY_ATTACH_TO_PARENT,
      },
    }) {
      FLOATING_BEAUTIFY;

      componentTopRow([&]() BF_FORCE_INLINE_LAMBDA {
        BF_CLAY_SPACER_HORIZONTAL;

        SDL_Scancode keys_[]{SDL_SCANCODE_ESCAPE, SDL_SCANCODE_TAB};
        VIEW_FROM_ARRAY_DANGER(keys);

        // Close button.
        const auto cancelID = CLAY_ID("stats_cancel");
        closeStats |= componentButton(
          {.id = cancelID, .group = groupStats, .keys = keys},
          [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_IMAGE({.texID = glib->ui_icon_cancel_big_texture_id()});
          }
        );
        markControlAsDefault(cancelID);
      });

      BF_CLAY_SPACER_VERTICAL;

      CLAY({})
      CLAY({
        .floating{
          .zIndex = zIndex,
          .attachPoints{
            .element = CLAY_ATTACH_POINT_RIGHT_CENTER,
            .parent  = CLAY_ATTACH_POINT_RIGHT_CENTER,
          },
          .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
          .attachTo           = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        Beautify b{.alpha = p, .translate{40 * EaseInQuad(1 - p), 0}};
        BEAUTIFY(b);

        const int STATS_ENTRY_WIDTH = 240;

        CLAY({
          .layout{
            .sizing{
              .width
              = CLAY_SIZING_FIXED(STATS_ENTRY_WIDTH * 2 + GAP_BIG + 2 * PADDING_FRAME),
            },
            BF_CLAY_PADDING_ALL(PADDING_FRAME),
            .childGap        = GAP_BIG,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
          BF_CLAY_CUSTOM_BEGIN{
            BF_CLAY_CUSTOM_NINE_SLICE(
              glib->ui_frame_nine_slice(), 0, true, {.set = false}
            ),
          } BF_CLAY_CUSTOM_END,
        }) {
          // Stats label.
          CLAY({.layout{
            BF_CLAY_SIZING_GROW_X,
            BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
          }}) {
            BF_CLAY_TEXT_LOCALIZED(Loc_UI_STATS__CAPS);
          }

          LAMBDA (
            void,
            componentStatsEntry,
            (int iconTexID, int locale, int value, StatType stat)
          )
          {
            CLAY({.layout{
              BF_CLAY_SIZING_GROW_X,
              BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
            }}) {
              // Increasing stat by clicking on it in debug mode.
              if (stat && ge.meta.debugEnabled) {
                if (clickOrTouchPressed())
                  ChangeStaticAndDynamicStatBy(stat, 1);
                if (wheel && Clay_Hovered())
                  ChangeStaticAndDynamicStatBy(stat, wheel);
              }

              const auto fb = fb_stats->Get(stat);

              if (iconTexID) {
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
                    FLOATING_BEAUTIFY;
                    BF_CLAY_IMAGE({.texID = iconTexID});
                  }
                }

                BF_CLAY_TEXT(" ");
              }

              FontBegin(&g.meta.fontStats);

              if (fb->is_percent())
                BF_CLAY_TEXT("% ");
              BF_CLAY_TEXT_LOCALIZED(locale);
              BF_CLAY_SPACER_HORIZONTAL;
              BF_CLAY_TEXT(" ");
              BF_CLAY_TEXT(TextFormat("%d", value));

              FontEnd();
            }
          };

          // Stats.
          CLAY({.layout{
            BF_CLAY_SIZING_GROW_XY,
            .childGap = GAP_BIG,
          }})
          FOR_RANGE (int, columnIndex, 2) {
            CLAY({.layout{
              BF_CLAY_SIZING_GROW_XY,
              .childGap        = GAP_SMALL,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            }})
            FOR_RANGE (int, i, (int)StatType_COUNT - 2) {
              const auto type = (StatType)(i + 2);
              const auto fb   = glib->stats()->Get(type);
              if (!fb->is_hidden() && (fb->is_secondary() == (bool)columnIndex)) {
                componentStatsEntry(
                  fb->small_icon_texture_id(), fb->name_locale(), GetStatValue(type), type
                );
              }
            }
          }
        }
      }

      BF_CLAY_SPACER_VERTICAL;
    }

    zIndex -= UIZIndexOffset_STATS;

    if (closeStats) {
      PlaySound(Sound_UI_CLICK);
      g.meta.showingStats = {};
    }
  }

  // "Quit" / "Restart" / "New Run" confirmation modals.
  {  ///
    LAMBDA (
      ConfirmModalResultType,
      confirmationModal,
      (bool* requested, bool* setToTrueOnConfirm, ControlsContext c, Loc locale)
    )
    {
      if (!(*requested))
        return ConfirmModalResultType_NONE;

      auto group = MakeControlsGroup();

      SCOPED_CONTEXT(c);

      zIndex += UIZIndexOffset_CONFIRM_MODAL;

      auto confirmID = CLAY_IDI("confirm_confirm", (int)c);
      auto cancelID  = CLAY_IDI("confirm_cancel", (int)c);

      ConfirmModalResultType result{};

      CLAY({.floating{
        .zIndex = zIndex,
        .attachPoints{
          .element = CLAY_ATTACH_POINT_CENTER_CENTER,
          .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
        },
        .attachTo = CLAY_ATTACH_TO_PARENT,
      }}) {
        FLOATING_BEAUTIFY;

        CLAY({
          .layout{
            .sizing{
              .width  = CLAY_SIZING_FIXED(450),
              .height = CLAY_SIZING_FIXED(200),
            },
            BF_CLAY_PADDING_ALL(PADDING_FRAME),
            BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
          BF_CLAY_CUSTOM_BEGIN{
            BF_CLAY_CUSTOM_SHADOW(glib->ui_frame_shadow_small_nine_slice(), true),
            BF_CLAY_CUSTOM_NINE_SLICE(
              glib->ui_frame_nine_slice(), 0, true, {.set = false}
            ),
          } BF_CLAY_CUSTOM_END,
        }) {
          componentOverlay([&]() BF_FORCE_INLINE_LAMBDA {
            if (clickOrTouchPressed())
              result = ConfirmModalResultType_CANCELLED;
          });

          BF_CLAY_SPACER_VERTICAL;

          FontBegin(&g.meta.fontUIBig);
          BF_CLAY_TEXT_LOCALIZED(Loc_UI_ARE_YOU_SURE__CAPS);
          FontEnd();

          BF_CLAY_SPACER_VERTICAL;

          CLAY({.layout{
            .childGap        = GAP_SMALL,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          }}) {
            const bool confirmed = componentOutlinedTextButton(
              {.id = confirmID, .group = group, .growX = true},
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT_LOCALIZED(locale, {.color = textColor});
              }
            );

            ControlsGroupNewRow(group);

            const bool cancelled = componentOutlinedTextButton(
              {.id = cancelID, .group = group, .growX = true, .keys = KEYS_CANCEL},
              [&](bool hovered, Color textColor) BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT_LOCALIZED(Loc_UI_CANCEL__CAPS, {.color = textColor});
              }
            );

            markControlAsDefault(cancelID);

            if (confirmed)
              result = ConfirmModalResultType_CONFIRMED;
            if (cancelled)
              result = ConfirmModalResultType_CANCELLED;
            if (confirmed || cancelled)
              PlaySound(Sound_UI_CLICK);
          }
        }
      }

      zIndex -= UIZIndexOffset_CONFIRM_MODAL;

      if (result == ConfirmModalResultType_CONFIRMED) {
        if (setToTrueOnConfirm)
          *setToTrueOnConfirm = true;
        g.meta.scheduledTogglePause = true;
      }

      if (result)
        *requested = false;

      return result;
    };

    confirmationModal(
      &ge.meta.quitRequested,
      &ge.meta.quitScheduled,
      ControlsContext_MODAL_CONFIRM_QUIT,
      Loc_UI_QUIT__CAPS
    );

    confirmationModal(
      &g.meta.confirmingRestart,
      &g.run.reload,
      ControlsContext_MODAL_CONFIRM_RESTART,
      Loc_UI_RESTART__CAPS
    );

    confirmationModal(
      &g.meta.confirmingNewRun,
      &g.run.scheduledNewRun,
      ControlsContext_MODAL_CONFIRM_NEW_RUN,
      Loc_UI_NEW_RUN__CAPS
    );
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
      translate = 100 * (1 - EaseBounceSmallSmooth(p));
      alpha     = EaseInOutQuad(p);
    }
    if (e >= ACHIEVEMENT_TOTAL_FRAMES - ACHIEVEMENT_OUT_FRAMES) {
      f32 p = (e + ACHIEVEMENT_OUT_FRAMES - ACHIEVEMENT_TOTAL_FRAMES)
                .Progress(ACHIEVEMENT_OUT_FRAMES);
      p         = Clamp01(p);
      translate = 200 - 200 * EaseBounceSmallSmooth(1 - p);
      alpha     = Clamp01(1 - EaseOutCubic(p));
    }

    zIndex += UIZIndexOffset_JUST_UNLOCKED_ACHIEVEMENTS;

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

      Beautify b{.alpha = alpha, .translate{translate, 0}};
      BEAUTIFY(b);

      auto fb      = fb_achievements->Get(x.type);
      auto fb_step = fb->steps()->Get(x.stepIndex);

      CLAY({.layout{.childGap = GAP_SMALL, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
        const bool unlocksSomething = fb_step->unlocks_build_type()
                                      || fb_step->unlocks_weapon_type()
                                      || fb_step->unlocks_item_type();

        if (unlocksSomething) {
          CLAY({
            .layout{.childGap = GAP_SMALL},
            BF_CLAY_CUSTOM_BEGIN{
              BF_CLAY_CUSTOM_SHADOW(glib->ui_frame_shadow_small_nine_slice(), true),
            } BF_CLAY_CUSTOM_END,
          }) {
            componentUniversalSlot({
              .build         = (BuildType)fb_step->unlocks_build_type(),
              .item          = (ItemType)fb_step->unlocks_item_type(),
              .weapon        = (WeaponType)fb_step->unlocks_weapon_type(),
              .tier          = 3,
              .canHover      = false,
              .unlocking     = true,
              .unlockElapsed = e,
            });
          }
        }

        componentAchievement(x.type, x.stepIndex, true);
      }
    }

    zIndex -= UIZIndexOffset_JUST_UNLOCKED_ACHIEVEMENTS;
  }

  // Cheat. Selecting weapon.
  if ((BF_DEBUG || ge.meta.debugEnabled) && (g.run.cheatWeaponIndex >= 0)) {  ///
    SCOPED_CONTEXT(ControlsContext_MODAL_CHEAT_WEAPON);

    const int WEAPONS_X = 8;
    const int WEAPONS_Y = CeilDivision((int)WeaponType_COUNT - 1, WEAPONS_X);

    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        ),
        BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
      },
      .floating{
        .zIndex = zIndex,
        .attachPoints{
          .element = CLAY_ATTACH_POINT_CENTER_CENTER,
          .parent  = CLAY_ATTACH_POINT_CENTER_CENTER,
        },
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
      } BF_CLAY_CUSTOM_END,
    }) {
      componentOverlay([]() {});

      CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}})
      FOR_RANGE (int, y, WEAPONS_Y) {
        CLAY({.layout{.childGap = GAP_SMALL}})
        FOR_RANGE (int, x, WEAPONS_X) {
          const int t = y * WEAPONS_X + x;
          if (t >= (int)WeaponType_COUNT - 1)
            continue;

          CLAY({}) {
            const bool clicked = componentUniversalSlot({
              .id     = CLAY_IDI("cheat_weapon", t),
              .weapon = (WeaponType)(t + 1),
            });

            if (Clay_Hovered()) {
              auto& w = g.run.state.weapons[g.run.cheatWeaponIndex];
              w.type  = (WeaponType)(t + 1);

              auto fb = fb_weapons->Get(t + 1);
              if (w.tier < fb->min_tier_index())
                w.tier = fb->min_tier_index();
            }
            if (clicked) {
              g.run.cheatWeaponIndex = -1;
              Save();
            }
          }
        }
      }
    }
  }

  ASSERT_FALSE(currentContext);

  // Control groups navigation.
  if (!draw) {  ///
    ControlsGroupConnect(groupDetails, Direction_RIGHT, groupDetails);

    ControlsContext shownScreen{};
    ControlsContext shownModal{};
    for (int i = 1; i < ControlsContext_COUNT; i++) {
      auto& context = controlsContexts[i];

      if (context.prevFrame && !context.thisFrame)
        context.focused = {};

      if (!context.disabled) {
        auto c = (ControlsContext)i;
        if (context.thisFrame) {
          if (CONTROLS_CONTEXT_MODALS.Contains(c))
            shownModal = c;
          else {
            ASSERT_FALSE(shownScreen);
            shownScreen = c;
          }
        }
      }

      context.prevFrame = context.thisFrame;
      context.thisFrame = false;
    }

    // if (shownModal)
    //   ASSERT(shownScreen);

    ControlsContext currentContext = shownModal;
    if (!currentContext)
      currentContext = shownScreen;

    lastFrameActiveControlsContext = currentContext;

    if (uiElementSwitchDirection && currentContext && !justFocusedDefaultControl) {
      auto toSelect = controlsContexts[currentContext].focused;

      auto group = g.ui.controlsGroupsFirst;
      while (group) {
        ASSERT(group->next != group);
        ASSERT(group->prev != group);

        auto dim = group->first;

        int preferredDimIndex = -1;
        while (dim) {
          preferredDimIndex++;

          ASSERT(dim->next != dim);
          ASSERT(dim->prev != dim);

          auto elem = dim->first;

          int preferredElemIndex = -1;
          while (elem) {
            preferredElemIndex++;

            ASSERT(elem->next != elem);
            ASSERT(elem->prev != elem);

            if (elem->id.id == controlsContexts[currentContext].focused.id) {
              if (uiElementSwitchDirection == Direction_RIGHT) {
                if (elem->next)
                  toSelect = elem->next->id;
                else {
                  auto to = group->connectionsPerDirection[(int)Direction_RIGHT - 1];
                  if (to) {
                    auto gto = _GetControlsGroup(to);
                    toSelect = gto->first->first->id;
                  }
                }
              }
              else if (uiElementSwitchDirection == Direction_LEFT) {
                if (elem->prev)
                  toSelect = elem->prev->id;
                else {
                  auto to = group->connectionsPerDirection[(int)Direction_LEFT - 1];
                  if (to) {
                    auto gto = _GetControlsGroup(to);
                    toSelect = gto->first->last->id;
                  }
                }
              }
              else if (uiElementSwitchDirection == Direction_DOWN) {
                if (dim->next && dim->next->first) {
                  toSelect
                    = _GetPreferredControlsEntry(dim->next->first, preferredElemIndex)
                        ->id;
                }
                else {
                  auto to = group->connectionsPerDirection[(int)Direction_DOWN - 1];
                  if (to) {
                    auto gto = _GetControlsGroup(to);
                    toSelect = gto->first->first->id;
                  }
                }
              }
              else if (uiElementSwitchDirection == Direction_UP) {
                if (dim->prev) {
                  toSelect
                    = _GetPreferredControlsEntry(dim->prev->first, preferredElemIndex)
                        ->id;
                }
                else {
                  auto to = group->connectionsPerDirection[(int)Direction_UP - 1];
                  if (to) {
                    auto gto = _GetControlsGroup(to);
                    toSelect = gto->last->first->id;
                  }
                }
              }
              else
                INVALID_PATH;
            }

            elem = elem->next;
          }
          dim = dim->next;
        }
        group = group->next;
      }

      if (!draw && (controlsContexts[currentContext].focused.id != toSelect.id)) {
        controlsContexts[currentContext].focused = toSelect;
        PlaySound(Sound_UI_HOVER_SMALL);
      }
    }

    g.ui.controlsGroupsFirst = nullptr;
    g.ui.controlsGroupsLast  = nullptr;
  }

  // Window is inactive.
  if (ge.meta.shouldGameplayStop.ShouldStop()) {  ///
    CLAY({
      .floating{
        .zIndex   = zIndex,
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
      BF_CLAY_CUSTOM_BEGIN{
        BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)),
      } BF_CLAY_CUSTOM_END,
    }) {}
  }

  // Game's version.
  if (!gdebug.hideUIForVideo) {  ///
    bool showVersion = ge.meta.debugEnabled;
#if BF_SHOW_VERSION
    showVersion = true;
#endif

    FontBegin(&g.meta.fontUIOutlined);

    CLAY({
      .floating{
        .offset{PADDING_OUTER_HORIZONTAL, -PADDING_OUTER_VERTICAL},
        .zIndex = i16_max,
        .attachPoints{
          .element = CLAY_ATTACH_POINT_LEFT_BOTTOM,
          .parent  = CLAY_ATTACH_POINT_LEFT_BOTTOM,
        },
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
    }) {
      FLOATING_BEAUTIFY;

      if (showVersion)
        BF_CLAY_TEXT(g_gameVersion);

      if (IsEmulatingMobile())
        BF_CLAY_TEXT("[EMULATING MOBILE]");

      if (BUILD_WARNINGS)
        BF_CLAY_TEXT(TextFormat("[WARNINGS: %d]", BUILD_WARNINGS));
    }

    FontEnd();
  }

  ASSERT(g.ui.clayZIndex == 0);
  ASSERT_FALSE(g.ui.overriddenFont);
  auto drawCommands = Clay_EndLayout();

  // Drawing UI.
  if (draw) {  ///
    ZoneScopedN("Drawing UI");

    auto& beautifiers      = g.ui.clayBeautifiers;
    auto& beautifiersCount = g.ui.clayBeautifiersCount;
    ASSERT(beautifiersCount == 0);

    FOR_RANGE (int, mode, 2) {
      // 0 - drawing ui.
      // 1 - drawing gizmos.

      DrawGroup_Begin(mode ? DrawZ_GIZMOS : DrawZ_UI);
      DrawGroup_SetSortY(0);

      FOR_RANGE (int, i, drawCommands.length) {
        auto& cmd = drawCommands.internalArray[i];

        auto    beautifierAlpha = 1.0f;
        Vector2 beautifierTranslate{0, 0};
        Vector2 beautifierScale{1, 1};
        FOR_RANGE (int, k, beautifiersCount) {
          auto& beautifier = beautifiers[k];
          beautifierAlpha *= beautifier.alpha;
          beautifierTranslate += beautifier.translate;
          beautifierScale *= beautifier.scale;
        }
        ASSERT(beautifierAlpha <= 1.0f);

        cmd.boundingBox.x -= (g.meta.screenSizeUI.x - LOGICAL_RESOLUTION.x) / 2.0f;
        cmd.boundingBox.y -= (g.meta.screenSizeUI.y - LOGICAL_RESOLUTION.y) / 2.0f;

        cmd.boundingBox.x += beautifierTranslate.x;
        cmd.boundingBox.y += beautifierTranslate.y;

        auto bb = cmd.boundingBox;
        bb.y    = LOGICAL_RESOLUTION.y - bb.y - bb.height;

        if (!mode) {
          switch (cmd.commandType) {
          case CLAY_RENDER_COMMAND_TYPE_NONE:
            break;

          case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
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

          case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
            const auto& d    = cmd.renderData.image;
            const auto& data = *(ClayImageData*)d.imageData;
            DrawGroup_CommandTexture({
              .texID = data.texID,
              .pos{
                bb.x + bb.width / 2 + data.offset.x,
                bb.y + bb.height / 2 + data.offset.y,
              },
              .anchor        = data.anchor,
              .scale         = data.scale,
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

          case CLAY_RENDER_COMMAND_TYPE_TEXT: {
            // TODO: uint16_t text.fontSize
            // TODO: letterSpacing
            // TODO: lineHeight
            auto& d    = cmd.renderData.text;
            auto  font = &g.meta.fontUI + d.fontId;

            Color c{
              (u8)d.textColor.r,
              (u8)d.textColor.g,
              (u8)d.textColor.b,
              (u8)((f32)d.textColor.a * beautifierAlpha),
            };
            DrawGroup_CommandText({
              .pos{bb.x, bb.y},
              .anchor{},
              .font       = font,
              .text       = d.stringContents.chars,
              .bytesCount = d.stringContents.length,
              .color      = c,
            });
          } break;

          case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
            const auto& d    = cmd.renderData.custom;
            const auto& data = *(ClayCustomData*)d.customData;

            if (data.beautifierStart.set) {
              ASSERT_FALSE(data.beautifierEnd.set);
              const auto& d                   = data.beautifierStart;
              beautifiers[beautifiersCount++] = {
                .alpha     = d.alpha,
                .translate = d.translate,
                .scale     = d.scale,
              };
            }

            if (data.beautifierEnd.set) {
              ASSERT_FALSE(data.beautifierStart.set);
              ASSERT(beautifiersCount > 0);
              beautifiersCount--;
            }

            if (data.overlay.set) {
              const auto& d = data.overlay;

              DrawGroup_CommandRect({
                .pos   = LOGICAL_RESOLUTIONf / 2.0f,
                .size  = ge.meta.scaledLogicalResolution,
                .color = d.color,
              });
            }

            if (data.screenBackground.set) {
              DrawGroup_CommandRect({
                .pos   = LOGICAL_RESOLUTIONf / 2.0f,
                .size  = ge.meta.scaledLogicalResolution,
                .color = Darken({49, 25, 23, 255}, 0.3f),
              });

              const int  texID = glib->ui_background_rect_texture_id();
              const auto gap   = GAP_SMALL;
              const auto size  = ToVector2(glib->original_texture_sizes()->Get(texID))
                                * ASSETS_TO_LOGICAL_RATIO;

              const int rectsXToSide = 9;
              const int rectsYToSide = 5;

#define BF_BACKGROUND_CYCLE_ENABLED 1

#if BF_BACKGROUND_CYCLE_ENABLED
              const int cycleDur = 20 * FIXED_FPS;
              const f32 cycleP   = (f32)(ge.meta.frameVisual % cycleDur) / (f32)cycleDur;
#endif

              const Color rectColor{22, 16, 13, 255};

              FOR_RANGE (int, y, rectsYToSide) {
                FOR_RANGE (int, n, 2) {
                  FOR_RANGE (int, x, rectsXToSide) {
                    FOR_RANGE (int, m, 2) {
                      if (!x && m)
                        continue;

                      if ((x == rectsXToSide - 1) && (y >= rectsYToSide - 1))
                        continue;

                      f32 offY = (size.y + gap) / 2.0f;
                      if (x % 2)
                        offY = 0;

                      auto center = LOGICAL_RESOLUTIONf / 2.0f;
                      auto off = Vector2(x * (size.x + gap), offY + y * (size.y + gap));
                      if (n)
                        off.y *= -1;

                      f32 angle = -PI32 / 6;

                      f32 fade = 1;

#if BF_BACKGROUND_CYCLE_ENABLED
                      off.y += (((x + m) % 2) ? 1 : -1) * (size.y + gap) * 0.1f
                               * cosf(2 * PI32 * cycleP);

                      // if ((y == rectsYToSide - 1) && ((x + m + n) % 2)) {
                      //   const f32 p = 10 * cycleP - 9;
                      //   if (p > 0)
                      //     fade = 1 - p;
                      // }
#endif

                      DrawGroup_CommandTexture({
                        .texID    = texID,
                        .rotation = angle,
                        .pos      = center + Vector2Rotate(off, angle + (m ? PI32 : 0)),
                        .color    = Fade(rectColor, fade),
                      });
                    }
                  }
                }
              }
            }

            if (data.shadow.set) {
              const auto& d = data.shadow;

              const auto downscaleFactor = (f32)glib->atlas_downscale_factor();
              const auto fb              = d.nineSlice;

              DrawGroup_CommandTextureNineSlice({
                .texID = fb->texture_id(),
                .pos{
                  bb.x - (f32)fb->outer_left() / downscaleFactor,
                  bb.y - (f32)fb->outer_bottom() / downscaleFactor,
                },
                .anchor{},
                .color = Fade(WHITE, beautifierAlpha * 3.0f / 5.0f),
                .nineSliceMargins{
                  (f32)fb->left() / downscaleFactor,
                  (f32)fb->right() / downscaleFactor,
                  (f32)fb->top() / downscaleFactor,
                  (f32)fb->bottom() / downscaleFactor,
                },
                .nineSliceSize{
                  (f32)bb.width
                    + (f32)(fb->outer_left() + fb->outer_right()) / downscaleFactor,
                  (f32)bb.height
                    + (f32)(fb->outer_top() + fb->outer_bottom()) / downscaleFactor,
                },
              });
            }

            if (data.nineSlice.set) {
              const auto& d = data.nineSlice;

              const auto downscaleFactor = (f32)glib->atlas_downscale_factor();
              const auto fb              = d.nineSlice;

              Color color{
                d.nineSliceColor.r, d.nineSliceColor.g, d.nineSliceColor.b, 255
              };
              if (d.breathing.set)
                color = BreatheColor(color, {.breathing = d.breathing});
              color.a = (u8)((f32)d.nineSliceColor.a * beautifierAlpha);

              DrawGroup_CommandTextureNineSlice({
                .texID = fb->texture_id(),
                .pos{bb.x, bb.y},
                .anchor{},
                .color = color,
                .flash = d.nineSliceFlash,
                .nineSliceMargins{
                  (f32)fb->left() / downscaleFactor,
                  (f32)fb->right() / downscaleFactor,
                  (f32)fb->top() / downscaleFactor,
                  (f32)fb->bottom() / downscaleFactor,
                },
                .nineSliceSize{(f32)bb.width, (f32)bb.height},
              });
            }
          } break;
          }
        }
        else {
          // UI elements' gizmos.
          if (gdebug.gizmos && bb.width && bb.height) {
            DrawGroup_CommandRectLines({
              .pos{bb.x, bb.y},
              .size{bb.width, bb.height},
              .anchor{0, 0},
              .color = GREEN,
            });
          }
        }
      }

      DrawGroup_End();
    }
  }

#undef GAP_FLEX
#undef GAP_SMALL
#undef GAP_BIG
#undef PADDING_FRAME
#undef PADDING_FRAME_SHADOW
#undef PADDING_OUTER_VERTICAL
#undef PADDING_OUTER_HORIZONTAL
}

int GetMobDamage(CreatureType type) {  ///
  auto fb = glib->creatures()->Get(type);
  ASSERT(fb->hostility_type() == HostilityType_MOB);
  return fb->contact_damage()
         + Round(
           fb->contact_damage_increase_per_wave()
           * (f32)(g.run.state.waveIndex + 1 - fb->appearing_wave_number())
         );
}

struct EmitParticlesData {  ///
  const BFGame::ParticleEmitter* fb_emitter           = {};
  Vector2                        pos                  = {};
  Vector2                        offsetScale          = {1, 1};
  Vector2                        offsetPlusMinusScale = {1, 1};
  f32                            offsetRotation       = 0;
  f32                            velocity             = 0;
  f32                            velocityAngle        = 0;
};

void EmitParticles(EmitParticlesData data) {  ///
  if (!data.fb_emitter)
    return;

  const auto interval = lframe::FromSeconds(data.fb_emitter->interval_seconds()).value;
  if ((interval > 0) && ((ge.meta.frameGame % interval) != 0))
    return;

  Vector2 off{};
  if (data.fb_emitter->offset())
    off = ToVector2(data.fb_emitter->offset()) * data.offsetScale;
  data.pos += Vector2Rotate(off, data.offsetRotation);
  data.pos += ToVector2(data.fb_emitter->offset_plus_minus())
              * Vector2(GRAND.FRand11(), GRAND.FRand11()) * data.offsetPlusMinusScale;

  MakeParticles({
    .type           = (ParticleType)data.fb_emitter->particle_type(),
    .pos            = data.pos,
    .velocity       = data.velocity,
    .velocityAngle  = data.velocityAngle,
    .scale          = 1.0f,
    .scalePlusMinus = 0.1f,
    .color          = Fade(WHITE, data.fb_emitter->fade()),
  });
}

void UpdateTrailSound(i64* nextTrailSoundVisualFrame, int trailSoundType) {  ///
  if (!trailSoundType)
    return;
  if (*nextTrailSoundVisualFrame <= ge.meta.frameVisual) {
    const auto fb_sound = glib->trail_sounds()->Get(trailSoundType);
    *nextTrailSoundVisualFrame
      = ge.meta.frameVisual + lframe::FromSeconds(fb_sound->repeat_seconds()).value;
    PlaySound(fb_sound->sound_hash());
  }
}

void GameFixedUpdate() {
  ZoneScoped;

  TEMP_USAGE(&g.meta.trashArena);
  TEMP_USAGE(&g.meta.transientDataArena);

  // Waiting for completion of save data loading.
  {  ///
    auto loaded = LoadSaveDataOnce(&g.meta.trashArena);
    if (loaded == SavedataLoadingType_JUST_FISNIHED) {
      GameInitAfterLoadingSavedata();
      GameReady();
    }
    else if (loaded != SavedataLoadingType_FISNIHED)
      return;
  }

  SetVolume(VolumeType_MASTER, 0.85f);
  SetVolume(VolumeType_SFX, (f32)g.player.volumeSFX / 3.0f);
  SetVolume(VolumeType_MUSIC, (f32)g.player.volumeMusic / 3.0f);

  // Setup. {  ///
  ReloadFontsIfNeeded();

  const auto fb_pickupables   = glib->pickupables();
  const auto fb_preSpawns     = glib->pre_spawns();
  const auto fb_atlasTextures = glib->atlas_textures();
  const auto fb_creatures     = glib->creatures();
  const auto fb_items         = glib->items();
  const auto fb_weapons       = glib->weapons();
  const auto fb_projectiles   = glib->projectiles();
  const auto fb_particles     = glib->particles();
  const auto fb_difficulties  = glib->difficulties();
  const auto fb_achievements  = glib->achievements();
  const auto fb_builds        = glib->builds();
  const auto fb_trailSounds   = glib->builds();
  // }

  // `g.meta.playerUsesKeyboardOrController`.
  {  ///
    if ((ge.events.last == LastEventType_KEY)
        || (ge.events.last == LastEventType_CONTROLLER))
      g.meta.playerUsesKeyboardOrController = true;
    else
      g.meta.playerUsesKeyboardOrController = false;
  }

  g.meta.pauseButtonFadeProgress = MoveTowardsF(
    g.meta.pauseButtonFadeProgress,
    (g.meta.playerUsesKeyboardOrController ? 0 : 1),
    FIXED_DT * 2
  );

  // Save.
  if (g.meta.scheduledSave && !ge.meta.previousSaveIsNotCompletedYet) {  ///
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

#ifndef BF_PLATFORM_WebYandex
      LOGI("Saving...");
#endif

      ge.meta.previousSaveIsNotCompletedYet = true;
      _Save(&g.meta.trashArena);
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
    if (g.meta.playerUsesKeyboardOrController && !IsEmulatingMobile())
      SDL_HideCursor();
    else
      SDL_ShowCursor();
  }

  // Cheats.
  if (ge.meta.debugEnabled || BF_DEBUG) {
    // F5 - add 10 coins.
    if (IsKeyPressed(SDL_SCANCODE_F5)) {  ///
      if (IsKeyDown(SDL_SCANCODE_LSHIFT) || IsKeyDown(SDL_SCANCODE_RSHIFT))
        ChangeCoins(int_max);
      else
        ChangeCoins(10);
    }

    if (g.run.state.screen == ScreenType_GAMEPLAY) {
      // F6 - complete wave.
      if (IsKeyPressed(SDL_SCANCODE_F6)) {  ///
        TriggerWaveCompleted(false);
        SetWaveWonTrue();
        PlaySound(Sound_GAME_WAVE_WON);
      }

      // F7 - show end screen.
      if (IsKeyPressed(SDL_SCANCODE_F7)) {  ///
        g.run.scheduledUI  = true;
        g.run.scheduledEnd = true;
      }

      // F8 - add level.
      if (IsKeyPressed(SDL_SCANCODE_F8)) {  ///
        AddXP(GetNextLevelXp(g.run.state.staticStats[StatType_LEVEL]));
      }

      // F9 - add chest.
      if (IsKeyPressed(SDL_SCANCODE_F9)) {  ///
        g.run.state.chests++;
      }
    }

    // F10 - unlock all achievements.
    if (IsKeyPressed(SDL_SCANCODE_F10)) {  ///
      int achievementIndex = -1;
      for (const auto fb : *glib->achievements()) {
        achievementIndex++;

        const auto fb_steps = fb->steps();
        if (!fb_steps)
          continue;

        int stepIndex = -1;
        for (const auto fb_step : *fb_steps) {
          stepIndex++;

          if (fb->negative_is_good())
            AchievementMin((AchievementType)achievementIndex, fb_step->value());
          else
            AchievementMax((AchievementType)achievementIndex, fb_step->value());
        }
      }
      g.ui.justUnlockedAchievements.Reset();

      Save();
    }

    // N - increase wave. Shift N - decrease wave.
    if (IsKeyPressed(SDL_SCANCODE_N)) {  ///
      g.run.state.waveIndex = MoveTowardsI(
        g.run.state.waveIndex,
        ((IsKeyDown(SDL_SCANCODE_LSHIFT) || IsKeyDown(SDL_SCANCODE_RSHIFT))
           ? 0
           : TOTAL_WAVES - 2),
        1
      );
      RecalculateThisWaveMobs();
      g.run.randomTiers = GetRandomCumulativeChances(
        g.run.state.waveIndex, g.run.state.staticStats[StatType_LUCK]
      );
    }
  }

  // Updating player's maxHealth.
  {  ///
    PLAYER_CREATURE.maxHealth = MAX(1, g.run.state.staticStats[StatType_HP]);
    PLAYER_CREATURE.health    = MIN(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth);
  }

  // Picking up remaining chests at the end of the wave.
  if (g.run.scheduledWaveCompleted.IsSet()) {  ///
    int pickupableIndex = -1;

    for (auto& pickupable : g.run.pickupables) {
      pickupableIndex++;

      if (pickupable.type != PickupableType_CHEST)
        continue;

      if (!pickupable.pickedUpAt.IsSet())
        Pickup(pickupableIndex);
    }
  }

  // Advancing to UI after wave completion animation finishes.
  if (g.run.scheduledWaveCompleted.IsSet()) {  ///
    if ((g.run.state.screen != ScreenType_WAVE_END_ANIMATION) && g.run.state.waveWon) {
      // Applying effects: END_OF_THE_WAVE_GET__STAT.
      IterateOverEffects(
        EffectConditionType_END_OF_THE_WAVE_GET__STAT,
        -1,
        [&](
          Weapon* w,
          int     wi,
          auto    fb_effect,
          int     tierOffset,
          int     times,
          int     thisWaveAddedCount
        ) BF_FORCE_INLINE_LAMBDA { ApplyStatEffect(fb_effect, tierOffset, times); }
      );

      // Making coins fly.
      for (auto& pickupable : g.run.pickupables) {
        if (pickupable.pickedUpAt.IsSet())
          continue;
        if (pickupable.type != PickupableType_COIN)
          continue;

        const auto& data = pickupable.DataCoin();

        pickupable.startedFlyingAt.SetNow();

        const auto durPlusMinus = WAVE_COMPLETED_FRAMES.value
                                  - WAVE_COMPLETED_COINS_FLYING_FRAMES.value
                                  - WAVE_COMPLETED_COINS_FLYING_FRAMES_RIGHT.value;
        pickupable.startedFlyingAt._value += (i64)(GRAND.FRand() * (f32)durPlusMinus);
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
        auto& harvesting = g.run.dynamicStats[StatType_HARVESTING];
        ChangeCoins(harvesting);
        AddXP(harvesting);
        // Harvesting stat (if positive) grows by 5% upon finishing each wave.
        if (harvesting > 0) {
          int percent = 5;
          IterateOverEffects(
            EffectConditionType_HARVESTING_INCREASES_BY_ADDITIONAL__X__PERCENT_AT_THE_END_OF_THE_WAVE,
            -1,
            [&](
              Weapon* w,
              int     wi,
              auto    fb_effect,
              int     tierOffset,
              int     times,
              int     thisWaveAddedCount
            ) BF_FORCE_INLINE_LAMBDA { percent += EFFECT_X_INT; }
          );

          if (percent > 0)
            ChangeStaticAndDynamicStatBy(
              StatType_HARVESTING, Ceil((f32)(harvesting * percent) / 100.0f)
            );
        }
      }

      if ((g.run.state.waveIndex >= TOTAL_WAVES - 1) || !g.run.state.waveWon) {
        g.run.scheduledEnd = true;
        g.run.state.won    = g.run.state.waveWon;

        if (!g.player.runsWon)
          Metric(g.run.state.won ? "g_Run0_Won" : "g_Run0_Lost");

        if (g.run.state.won)
          g.player.runsWon++;
        else
          g.player.runsLost++;

        // Updating `Build.maxDifficultyBeaten` + achievement.
        if (g.run.state.won) {
          auto d = g.player.difficulty;

          AchievementMax(AchievementType_DIFFICULTY, d);

          auto fb_build = fb_builds->Get(g.player.build);
          AchievementMax((AchievementType)fb_build->finish_run_achievement_type(), 1);

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

    if (g.run.state.chests) {
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
    SwitchScreen(ScreenType_NEW_RUN);
    Save();
  }

  // Advancing to ScreenType_PICKED_UP_ITEM.
  if (g.run.scheduledPickedUpItems) {  ///
    g.run.scheduledPickedUpItems = false;

    SwitchScreen(ScreenType_PICKED_UP_ITEM);

    if (g.run.scheduledPickedUpItemsReset) {
      g.run.scheduledPickedUpItemsReset = false;
      g.run.state.pickedUpItem.toPick   = GenerateRandomItem();
    }

    Save();
  }

  // Advancing to ScreenType_UPGRADES.
  if (g.run.scheduledUpgrades) {  ///
    g.run.scheduledUpgrades = false;
    SwitchScreen(ScreenType_UPGRADES);

    if (g.run.scheduledUpgradesReset) {
      g.run.scheduledUpgradesReset = false;
      RefillUpgradesToPick(false);
    }

    if (g.run.state.staticStats[StatType_LEVEL] == g.run.state.levelOnStartOfTheWave) {
      g.run.scheduledShop      = true;
      g.run.scheduledShopReset = true;
    }

    Save();
  }

  // Advancing to ScreenType_SHOP.
  if (g.run.scheduledShop) {  ///
    g.run.scheduledShop = false;
    SwitchScreen(ScreenType_SHOP);

    g.run.projectiles.Reset();
    g.run.numbers.Reset();
    g.run.particles.Reset();
    g.run.landmines.Reset();
    g.run.gardens.Reset();

#if 1
    // Resetting all pickupables.
    g.run.pickupables.Reset();
#else
    // Resetting all pickupables except chests.
    {
      const int total = g.run.pickupables.count;
      int       off   = 0;
      FOR_RANGE (int, i, total) {
        const auto& pickupable = g.run.pickupables[i - off];
        if (pickupable.type != PickupableType_CHEST) {
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

      bool destroyWeapons = false;

      IterateOverEffects(
        EffectConditionType_WEAPONS_ARE_DESTROYED_UPON_ENTERING_SHOP,
        -1,
        [&](
          Weapon* w,
          int     wi,
          auto    fb_effect,
          int     tierOffset,
          int     times,
          int     thisWaveAddedCount
        ) BF_FORCE_INLINE_LAMBDA { destroyWeapons = true; }
      );

      if (destroyWeapons) {
        RemoveImmediateWeaponEffects();

        for (auto& w : g.run.state.weapons)
          w.type = {};

        ApplyImmediateWeaponEffects();
      }
    }

    Save();
  }

  // Advancing to ScreenType_END.
  if (g.run.scheduledEnd) {  ///
    if (g.run.state.won)
      PlaySound(Sound_GAME_RUN_WON);
    else
      PlaySound(Sound_GAME_RUN_LOST);

    g.run.scheduledEnd = false;
    SwitchScreen(ScreenType_END);

    Save();
  }

  // Advancing to the next wave (ScreenType_GAMEPLAY).
  if (g.run.scheduledNextWave) {  ///
    g.run.scheduledNextWave = false;
    SwitchScreen(ScreenType_GAMEPLAY);

    g.run.state.waveIndex++;
    g.run.state.xpOnStartOfTheWave = g.run.state.xp;
    OnWaveStarted();

    const int health          = g.run.state.staticStats[StatType_HP];
    PLAYER_CREATURE.health    = MAX(1, health);
    PLAYER_CREATURE.maxHealth = MAX(1, health);

    Save();
  }

  PLAYER_CREATURE.controller.move = {};

  // Handling toggle pause.
  if (g.meta.scheduledTogglePause) {  ///
    g.meta.scheduledTogglePause = false;

    g.meta.paused = !g.meta.paused;

    Save();
  }

  // Handling ESC or P to toggle pause.
  {  ///
    if (!PAUSABLE_SCREENS.Contains(g.run.state.screen)) {
      g.meta.paused               = false;
      g.meta.scheduledTogglePause = false;
    }
    else if (!g.meta.paused) {
      for (auto c : KEYS_PAUSE) {
        if (IsKeyPressed(c)) {
          g.meta.scheduledTogglePause = true;
          PlaySound(Sound_UI_CLICK);
          break;
        }
      }
    }
  }

  const auto gameplayOrWaveEndScreen
    = (g.run.state.screen == ScreenType_GAMEPLAY)
      || (g.run.state.screen == ScreenType_WAVE_END_ANIMATION);

  // Updating gameplay.
  if (!ge.meta.shouldGameplayStop.ShouldStop()  //
      && !g.meta.paused                         //
      && gameplayOrWaveEndScreen)
  {
    ZoneScopedN("Updating gameplay.");

    MarkGameplay();

    constexpr f32 CREATURES_MOVE_MARGIN = 2;

    if (g.run.state.screen == ScreenType_GAMEPLAY) {
      // Stick controls.
      {  ///
        auto& c = g.meta.stickControl;

        if (IsMousePressed(L)) {
          c.controlling = true;
          c.startPos    = ScreenPosToLogical(GetMouseScreenPos());
          c.targetPos   = c.startPos;
        }
        else if (ge.meta._latestActiveTouchID != InvalidTouchID) {
          const auto td = GetTouchData(ge.meta._latestActiveTouchID);

          if (IsTouchPressed(ge.meta._latestActiveTouchID)) {
            c.controlling = true;
            c.startPos    = ScreenPosToLogical(td.screenPos);
            c.targetPos   = c.startPos;
          }

          c.targetPos = ScreenPosToLogical(td.screenPos);
        }
        else if (IsMouseDown(L))
          c.targetPos = ScreenPosToLogical(GetMouseScreenPos());
        else
          c.controlling = false;

        if (c.controlling && (c.startPos != c.targetPos)) {
          c.calculatedDir
              = Vector2Normalize(c.targetPos - c.startPos)
                * (MIN(
                  g.ui.touchControlMaxLogicalOffset, Vector2Distance(c.startPos, c.targetPos)
                )
                / g.ui.touchControlMaxLogicalOffset);
        }
        else
          c.calculatedDir = {};
      }

#if BF_ENABLE_ASSERTS
      for (auto& creature : g.run.creatures) {  ///
        if ((creature.health <= 0) && !creature.diedAt.IsSet())
          INVALID_PATH;
      }
#endif

      // Finishing wave opens upgrades screen.
      if (g.run.waveStartedAt.IsSet()
          && (g.run.waveStartedAt.Elapsed() >= GetWaveDuration(g.run.state.waveIndex)))
      {  ///
        if (!g.run.scheduledWaveCompleted.IsSet()) {
          TriggerWaveCompleted(false);
          SetWaveWonTrue();
          PlaySound(Sound_GAME_WAVE_WON);
          Save();
        }
      }

      if (!PLAYER_CREATURE.diedAt.IsSet()) {
        // Player actions. Moving.
        {  ///
          Vector2 move{};

          if (g.meta.stickControl.controlling)
            move = g.meta.stickControl.calculatedDir;
          else {
            if (IsKeyDown(SDL_SCANCODE_D) || IsKeyDown(SDL_SCANCODE_RIGHT))
              move.x += 1;
            if (IsKeyDown(SDL_SCANCODE_W) || IsKeyDown(SDL_SCANCODE_UP))
              move.y += 1;
            if (IsKeyDown(SDL_SCANCODE_A) || IsKeyDown(SDL_SCANCODE_LEFT))
              move.x -= 1;
            if (IsKeyDown(SDL_SCANCODE_S) || IsKeyDown(SDL_SCANCODE_DOWN))
              move.y -= 1;

            if (move.x || move.y)
              move = Vector2Normalize(move);
          }

          PLAYER_CREATURE.controller.move = move;
        }

        // Player HP regen.
        if (!PLAYER_CREATURE.diedAt.IsSet()
            && (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth))
        {  ///
          const auto interval = GetRegenInterval(g.run.dynamicStats[StatType_REGEN]);
          const bool canRegen = (g.run.playerLastRegenAt.Elapsed() >= interval);

          if (canRegen) {
            HealPlayer(1);
            g.run.playerLastRegenAt = {};
            g.run.playerLastRegenAt.SetNow();
          }
        }
      }

      // Setting creatures' `dir`.
      {  ///
        ZoneScopedN("Setting creatures `dir`.");

        for (auto& creature : g.run.creatures) {
          if (creature.diedAt.IsSet())
            continue;

          bool thresholded = true;
          if (creature.type == CreatureType_PLAYER)
            thresholded = (abs(creature.controller.move.x) > 0.2f);
          if (!thresholded)
            continue;

          if (creature.controller.move.x > 0)
            creature.dir.x = 1;
          else
            creature.dir.x = -1;
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

            if ((creature.pos.x <= CREATURES_WORLD_SPAWN_BOUNDS.pos.x) && (move.x <= 0))
              move = Vector2Rotate({1, 0}, Lerp(-PI32 / 2, PI32 / 2, GRAND.FRand()));

            if ((creature.pos.y <= CREATURES_WORLD_SPAWN_BOUNDS.pos.y) && (move.y <= 0))
              move = Vector2Rotate({1, 0}, Lerp(0, PI32, GRAND.FRand()));

            if ((creature.pos.x >= CREATURES_WORLD_SPAWN_BOUNDS.pos.x
                                     + CREATURES_WORLD_SPAWN_BOUNDS.size.x)
                && (move.x >= 0))
              move = Vector2Rotate({1, 0}, Lerp(PI32 / 2, 3 * PI32 / 2, GRAND.FRand()));

            if ((creature.pos.y >= CREATURES_WORLD_SPAWN_BOUNDS.pos.y
                                     + CREATURES_WORLD_SPAWN_BOUNDS.size.y)
                && (move.y >= 0))
              move = Vector2Rotate({1, 0}, Lerp(PI32, 2 * PI32, GRAND.FRand()));
          }

          if (creature.aggroed) {
            if (fb->can_move()) {
              creature.controller.move
                = Vector2DirectionOrZero(creature.pos, PLAYER_CREATURE.pos);
            }
          }

          if (creature.aggroed && (creature.type == CreatureType_RANGER)) {
            const auto distSqr = Vector2DistanceSqr(creature.pos, PLAYER_CREATURE.pos);

            bool canShoot = true;

            if (distSqr < SQR(MOB_RANGER_SHOOT_METERS - MOB_RANGER_THRESHOLD_METERS))
              creature.controller.move *= -1.0f;
            else if (distSqr < SQR(MOB_RANGER_SHOOT_METERS + MOB_RANGER_THRESHOLD_METERS))
              creature.controller.move = {};
            else
              canShoot = false;

            auto& data = creature.DataRanger();
            if (data.startedShootingAt.IsSet()) {
              creature.dir.x = ((PLAYER_CREATURE.pos.x - creature.pos.x >= 0) ? 1 : -1);

              const auto e = data.startedShootingAt.Elapsed();
              if (e == MOB_RANGER_SHOOTING_FRAME) {
                MakeProjectile({
                  .c{
                    .type              = ProjectileType_MOB,
                    .ownerCreatureType = creature.type,
                    .pos               = creature.pos,
                    .dir    = Vector2DirectionOrRandom(creature.pos, PLAYER_CREATURE.pos),
                    .range  = SQR(WORLD_X + WORLD_Y) * 2,
                    .damage = GetMobDamage(creature.type),
                  },
                });
                PlaySound(fb->action_sound_hash());
              }
              if (e >= MOB_RANGER_SHOOTING_FRAMES) {
                creature.speedModifier /= fb->acting_speed_modifier_scale();
                data.startedShootingAt = {};
              }
            }
            else if (canShoot) {
              data.startedShootingAt.SetNow();
              creature.speedModifier *= fb->acting_speed_modifier_scale();
            }
          }
          else if (creature.aggroed && (creature.type == CreatureType_RUSHER)) {
            auto& data = creature.DataRusher();

            if (data.startedRushingAt.IsSet()) {
              creature.controller.move = {};

              creature.dir = data.rushingDir;

              const auto e = data.startedRushingAt.Elapsed();
              if ((MOB_RUSHER_RUSH_PRE_FRAMES < e)
                  && (e < MOB_RUSHER_RUSH_TOTAL_FRAMES - MOB_RUSHER_RUSH_POST_FRAMES))
                creature.controller.move = data.rushingDir;

              if (e >= MOB_RUSHER_RUSH_TOTAL_FRAMES) {
                data.startedRushingAt = {};
                data.finishedRushingAt.SetNow();
                data.rushingDir = {};
                data.cooldown.SetRand(MOB_RUSHER_COOLDOWN_MIN, MOB_RUSHER_COOLDOWN_MAX);
                creature.speedModifier /= fb->acting_speed_modifier_scale();
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
                                          * fb->acting_speed_modifier_scale()
                                          * durSeconds;

                if (dist <= rushDistance) {
                  data.startedRushingAt.SetNow();
                  data.finishedRushingAt = {};
                  data.rushingDir
                    = Vector2DirectionOrRandom(creature.pos, PLAYER_CREATURE.pos);
                  creature.speedModifier *= fb->acting_speed_modifier_scale();
                  PlaySound(fb->action_sound_hash());
                }
              }
            }
          }
          else if (creature.aggroed && (creature.type == CreatureType_BOSS)) {
            auto&      data = creature.DataBoss();
            const auto e    = data.startedShootingAt.Elapsed() - data.cooldown;

            const auto patterns = glib->boss_shooting_patterns();
            if ((data.shootingPattern < 0) || (data.shootingPattern >= patterns->size()))
              data.shootingPattern = GRAND.Rand() % patterns->size();

            const auto pattern       = patterns->Get(data.shootingPattern);
            auto       shootingFrame = lframe::FromSeconds(glib->boss_pre_shot_seconds());
            auto       stride        = lframe::FromSeconds(
              glib->boss_pre_shot_seconds()     //
              + glib->boss_post_shot_seconds()  //
              + pattern->cooldown_between_shots_in_seconds()
            );

            FOR_RANGE (int, i, pattern->shots_count()) {
              if (e > shootingFrame) {
                shootingFrame = shootingFrame + stride;
                continue;
              }

              if (e == shootingFrame) {
                auto projectilesToSpawn = pattern->projectiles_per_shot();
                FOR_RANGE (int, shotIndex, projectilesToSpawn) {
                  MakeProjectile({
                    .c{
                      .type              = ProjectileType_BOSS,
                      .ownerCreatureType = creature.type,
                      .pos               = creature.pos,
                      .dir               = Vector2Rotate(
                        {1, 0},
                        2 * PI32 * (f32)shotIndex / (f32)projectilesToSpawn
                          + data.shootingRotationOffset
                      ),
                      .range  = f32_inf,
                      .damage = GetMobDamage(creature.type),
                    },
                  });
                  PlaySound(fb->action_sound_hash());
                }
                data.shootingRotationOffset
                  += pattern->rotation_offset()
                     * ((data.startedShootingAt._value % 2) ? 1 : -1);
              }

              break;
            }

            const auto duration = lframe::FromSeconds(
              (f32)pattern->shots_count()
                * (glib->boss_pre_shot_seconds() + glib->boss_post_shot_seconds())
              + (f32)(pattern->shots_count() - 1)
                  * pattern->cooldown_between_shots_in_seconds()
            );

            if (e >= duration) {
              data.startedShootingAt = {};
              data.startedShootingAt.SetNow();
              data.cooldown.SetRandSeconds(
                glib->boss_cooldown_min_seconds(), glib->boss_cooldown_max_seconds()
              );

              int newPattern = data.shootingPattern;
              while ((newPattern == data.shootingPattern) && (patterns->size() > 1))
                newPattern = GRAND.Rand() % patterns->size();
              data.shootingPattern = newPattern;
            }
          }
          else if (creature.type == CreatureType_TURRET) {
            auto& data = creature.DataTurret();

            f32 rangeMeters = fb->turret_range_meters();
            rangeMeters *= 1 + (f32)g.run.dynamicStats[StatType_STRUCTURE_RANGE] / 100.0f;
            rangeMeters = MAX(STRUCTURE_MIN_RANGE_METERS, rangeMeters);

            auto originalSize
              = glib->original_texture_sizes()->Get(fb->texture_ids()->Get(0));
            const auto projectileSpawnPos
              = creature.pos
                + Vector2(
                  0,
                  (fb->turret_projectile_shoot_anchor_y() - 0.5f) * (f32)originalSize->y()
                    * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE
                );
            auto projectileType = (ProjectileType)fb->turret_projectile_type();
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
                = ApplyStructureAttackSpeedToDuration(MOB_TURRET_SHOOT_FRAME);
              if (e == shotFrame) {
                int damage = ApplyDamageScalings(data.damage, 0, data.damageScalings, 1);
                damage     = ApplyPlayerStatDamageMultiplier(damage);

                MakeProjectile({
                  .c{
                    .type                 = projectileType,
                    .ownerCreatureType    = creature.type,
                    .weaponIndexOrMinus1  = -1,
                    .pos                  = projectileSpawnPos,
                    .dir                  = data.aimDirection,
                    .range                = rangeMeters,
                    .damage               = damage,
                    .critDamageMultiplier = fb->turret_crit_damage_multiplier(),
                    .knockbackMeters      = fb->turret_knockback_meters(),
                    .pierce               = fb->turret_pierce(),
                    .bounce               = fb->turret_bounce(),
                  },
                });

                PlaySound(
                  fb->action_sound_hash(), {.pos{creature.pos.x, creature.pos.y, 2, 16}}
                );
              }

              const auto dur
                = ApplyStructureAttackSpeedToDuration(MOB_TURRET_SHOOTING_FRAMES);
              if (e >= dur)
                data.startedShootingAt = {};
            }
          }
        }
      }

      // Making pre spawn decals.
      {  ///
        LAMBDA (
          void,
          makePreSpawns,
          (CreatureType type, TurretToSpawn turret = {}, GardenToSpawn garden = {})
        )
        {
          ASSERT(type);

          Vector2 posToSpawn{};
          while (1) {
            constexpr f32 epsilon = 0.001f;
            posToSpawn            = CREATURES_WORLD_SPAWN_BOUNDS.GetRandomGamePosInside();
            ASSERT(CREATURES_WORLD_SPAWN_BOUNDS.ContainsInside(posToSpawn));
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
              p = posToSpawn + Vector2Rotate({off, 0}, GRAND.Angle());
            } while (!CREATURES_WORLD_SPAWN_BOUNDS.ContainsInside(p));

            MakePreSpawn({
              .type                    = PreSpawnType_CREATURE,
              .typeCreature            = type,
              .pos                     = p,
              .damage                  = turret.baseDamage,
              .damageScalings          = turret.damageScalings,
              .garden_spawnsAppleEvery = garden.spawnsAppleEvery,
            });
          }
        };

        int  spawnEnemiesEvery = FIXED_FPS;
        auto multiplier        = 1 + (f32)g.run.dynamicStats[StatType_ENEMIES] / 100.0f;
        multiplier             = MAX(0.001f, multiplier);
        constexpr int WAVE_MAX_ENEMIES = 8;
        multiplier
          *= Lerp(0.4f, 1, MIN(1, (f32)g.run.state.waveIndex / (f32)WAVE_MAX_ENEMIES));
        spawnEnemiesEvery
          = Round((f32)spawnEnemiesEvery * powf(2, -gdebug.mobSpawnRate) / multiplier);
        spawnEnemiesEvery = MAX(1, spawnEnemiesEvery);

        if (CanSpawnMoreCreatures()) {
          if (((g.run.waveStartedAt.Elapsed().value - 1) % spawnEnemiesEvery) == 0) {
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

          // Spawning turrets and gardens.
          if ((g.run.waveStartedAt.Elapsed().value + 1)
                % (FIXED_FPS * SPAWNING_TURRETS_OR_GARDENS_EVERY_N_SECONDS)
              == 0)
          {
            if (g.run.turretsToSpawn.count > 0)
              makePreSpawns(CreatureType_TURRET, g.run.turretsToSpawn.Pop());

            if (g.run.gardensToSpawn.count > 0) {
              auto t = g.run.gardensToSpawn.Pop();
              MakePreSpawn({
                .type = PreSpawnType_GARDEN,
                .pos  = CREATURES_WORLD_SPAWN_BOUNDS.GetRandomGamePosInside(),
                .garden_spawnsAppleEvery = t.spawnsAppleEvery,
              });
            }
          }

          // Spawning landmines.
          IterateOverEffects(
            EffectConditionType_SPAWNS_LANDMINE_EVERY__X__SECONDS_DEALING__Y__DAMAGE,
            -1,
            [&](
              Weapon* w,
              int     wi,
              auto    fb_effect,
              int     tierOffset,
              int     times,
              int     thisWaveAddedCount
            ) BF_FORCE_INLINE_LAMBDA {
              auto e        = g.run.waveStartedAt.Elapsed().value;
              auto interval = lframe::FromSeconds(EFFECT_X_FLOAT / (f32)times);
              if (((e + 1) % interval.value) != 0)
                return;

              MakePreSpawn({
                .type           = PreSpawnType_LANDMINE,
                .pos            = CREATURES_WORLD_SPAWN_BOUNDS.GetRandomGamePosInside(),
                .damage         = EFFECT_Y_INT,
                .damageScalings = fb_effect->damage_scalings(),
              });
            }
          );

          // Spawning gardens.
          IterateOverEffects(
            EffectConditionType_SPAWNS_GARDEN_EVERY__X__SECONDS_THAT_SPAWNS_APPLE_EVERY__Y__SECONDS,
            -1,
            [&](
              Weapon* w,
              int     wi,
              auto    fb_effect,
              int     tierOffset,
              int     times,
              int     thisWaveAddedCount
            ) BF_FORCE_INLINE_LAMBDA {
              auto e        = g.run.waveStartedAt.Elapsed().value;
              auto interval = lframe::FromSeconds(EFFECT_X_FLOAT);
              if (((e + 1) % interval.value) != 0)
                return;

              FOR_RANGE (int, i, times) {
                MakePreSpawn({
                  .type = PreSpawnType_GARDEN,
                  .pos  = CREATURES_WORLD_SPAWN_BOUNDS.GetRandomGamePosInside(),
                  .garden_spawnsAppleEvery = lframe::FromSeconds(EFFECT_Y_FLOAT),
                });
              }
            }
          );

          // Spawning trees every 10 seconds.
          int seconds = 10;
          if (gdebug.overriddenTreeSpawnIntervalSeconds)
            seconds = gdebug.overriddenTreeSpawnIntervalSeconds;
          if ((g.run.waveStartedAt.Elapsed().value + 1) % (FIXED_FPS * seconds) == 0) {
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
          auto& x = g.run.preSpawns[i - off];

          ASSERT(x.type);
          if (x.type == PreSpawnType_CREATURE)
            ASSERT(x.typeCreature);

          const bool shouldSpawn
            = x.createdAt.IsSet() && (x.createdAt.Elapsed() >= SPAWN_FRAMES);
          if (!shouldSpawn)
            continue;

          switch (x.type) {
          case PreSpawnType_CREATURE: {
            auto fb       = fb_creatures->Get(x.typeCreature);
            bool canSpawn = true;
            if (fb->hostility_type() == HostilityType_MOB) {
              if (Vector2DistanceSqr(x.pos, PLAYER_CREATURE.pos) <= SQR(
                    CREATURE_COLLIDER_RADIUS
                    * (fb_creatures->Get(CreatureType_PLAYER)->collider_scale() + fb->collider_scale())
                  ))
                canSpawn = false;
            }
            if (canSpawn) {
              MakeCreature({
                .type           = x.typeCreature,
                .pos            = x.pos,
                .damage         = x.damage,
                .damageScalings = x.damageScalings,
              });
            }
          } break;

          case PreSpawnType_LANDMINE: {
            MakeLandmine({
              .pos            = x.pos,
              .damage         = x.damage,
              .damageScalings = x.damageScalings,
            });
          } break;

          case PreSpawnType_GARDEN: {
            MakeGarden({
              .pos              = x.pos,
              .spawnsAppleEvery = x.garden_spawnsAppleEvery,
            });
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
      if ((g.run.state.waveIndex >= TOTAL_WAVES - 1)  //
          && !g.run.bossCreatureID                    //
          && g.run.walkingTutorialCompletedAt.IsSet())
      {  ///
        const auto worldCenter = WORLD_SIZEf / 2.0f;
        const auto dir       = Vector2DirectionOrRandom(PLAYER_CREATURE.pos, worldCenter);
        const auto bossPos   = worldCenter + dir * BOSS_SPAWN_OFFSET_METERS;
        g.run.bossCreatureID = MakeCreature({.type = CreatureType_BOSS, .pos = bossPos});
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
                fb_creatures->Get(CreatureType_PLAYER)->hurtbox_radius()
                + CREATURE_COLLIDER_RADIUS * fb->collider_scale()
              ))
          {
            TryApplyDamage({
              .creatureIndex       = 0,
              .damage              = GetMobDamage(creature.type),
              .damagerCreatureType = creature.type,
            });
          }
        }
      }

      // Picking up pickupables.
      if (!PLAYER_CREATURE.diedAt.IsSet()) {  ///
        ZoneScopedN("Picking up pickupables.");

        FOR_RANGE (int, pickupableIndex, g.run.pickupables.count) {
          const auto& pickupable = g.run.pickupables[pickupableIndex];

          if (pickupable.pickedUpAt.IsSet())
            continue;

          auto pickupRangeScale = 100.0f + (f32)g.run.dynamicStats[StatType_PICKUP_RANGE];
          pickupRangeScale      = MAX(30, pickupRangeScale);
          pickupRangeScale /= 100.0f;

          if (Vector2DistanceSqr(pickupable.pos, PLAYER_CREATURE.pos)
              <= SQR(PICKUPABLE_HURTBOX_RADIUS * pickupRangeScale))
            Pickup(pickupableIndex);
        }
      }

      // Achievements. Walker / Idler.
      {  ///
        const f32 deltaMeters
          = Vector2Distance(PLAYER_CREATURE.pos, g.run.previousPlayerPos);

        g.run.previousPlayerPos = PLAYER_CREATURE.pos;

        LAMBDA (void, giveBonus, (EffectConditionType effectConditionType, int sign)) {
          IterateOverEffects(
            effectConditionType,
            -1,
            [&](
              Weapon* w,
              int     wi,
              auto    fb_effect,
              int     tierOffset,
              int     times,
              int     thisWaveAddedCount
            ) {
              const auto stat = (StatType)fb_effect->stat_type();
              ChangeDynamicStatBy(
                stat,
                Round(
                  GetStatModificationScale(stat) * fb_effect->value()->Get(tierOffset)
                  * times * sign
                )
              );
            }
          );
        };

        if ((deltaMeters > 0) && !FloatEquals(deltaMeters, 0)) {
          g.run.playerWalkedMetersCumulativeDelta += deltaMeters;

          if (g.run.playerContinuousIdleFrames >= IDLE_OR_WALKING_BONUS_FRAMES)
            giveBonus(EffectConditionType_STAT__WHEN_IDLE, -1);
          g.run.playerContinuousIdleFrames = 0;

          g.run.playerContinuousWalkingFrames++;
          if (g.run.playerContinuousWalkingFrames == IDLE_OR_WALKING_BONUS_FRAMES)
            giveBonus(EffectConditionType_STAT__WHEN_WALKING, 1);
        }
        else {
          g.run.playerContinuousIdleFrames++;
          if (g.run.playerContinuousIdleFrames == IDLE_OR_WALKING_BONUS_FRAMES)
            giveBonus(EffectConditionType_STAT__WHEN_IDLE, 1);

          if (g.run.playerContinuousWalkingFrames >= IDLE_OR_WALKING_BONUS_FRAMES)
            giveBonus(EffectConditionType_STAT__WHEN_WALKING, -1);
          g.run.playerContinuousWalkingFrames = 0;

          IterateOverEffects(
            EffectConditionType_STAT__EVERY__X__IDLE_SECONDS,
            -1,
            [&](
              Weapon* w,
              int     wi,
              auto    fb_effect,
              int     tierOffset,
              int     times,
              int     thisWaveAddedCount
            ) BF_FORCE_INLINE_LAMBDA {
              const auto lf = lframe::FromSeconds(EFFECT_X_FLOAT);
              if ((g.run.playerContinuousIdleFrames % lf.value) == 0) {
                ChangeStaticAndDynamicStatBy(
                  (StatType)fb_effect->stat_type(),
                  fb_effect->value()->Get(tierOffset) * times
                );
              }
            }
          );

          if (g.run.walkingTutorialCompletedAt.IsSet()
              && ((g.run.playerContinuousIdleFrames % FIXED_FPS) == 0))
            AchievementAdd(AchievementType_IDLER, 1);
        }

        while (g.run.playerWalkedMetersCumulativeDelta >= 1) {
          g.run.playerWalkedMetersCumulativeDelta -= 1;
          g.run.playerWalkedMeters++;

          if (g.run.walkingTutorialCompletedAt.IsSet())
            AchievementAdd(AchievementType_WALKER, 1);

          IterateOverEffects(
            EffectConditionType_STAT__EVERY__X__WALKED_METERS,
            -1,
            [&](
              Weapon* w,
              int     wi,
              auto    fb_effect,
              int     tierOffset,
              int     times,
              int     thisWaveAddedCount
            ) BF_FORCE_INLINE_LAMBDA {
              if ((g.run.playerWalkedMeters % EFFECT_X_INT) == 0) {
                ChangeStaticAndDynamicStatBy(
                  (StatType)fb_effect->stat_type(),
                  fb_effect->value()->Get(tierOffset) * times
                );
              }
            }
          );
        }
      }

      // Processing EffectConditionType_STAT__EVERY__X__SECONDS.
      if (g.run.waveStartedAt.IsSet()) {  ///
        IterateOverEffects(
          EffectConditionType_STAT__EVERY__X__SECONDS,
          -1,
          [&](
            Weapon* w,
            int     wi,
            auto    fb_effect,
            int     tierOffset,
            int     times,
            int     thisWaveAddedCount
          ) BF_FORCE_INLINE_LAMBDA {
            const auto lf = lframe::FromSeconds(EFFECT_X_FLOAT);
            const auto e  = g.run.waveStartedAt.Elapsed();
            if (((e.value + 1) % lf.value) == 0) {
              ChangeStaticAndDynamicStatBy(
                (StatType)fb_effect->stat_type(),
                fb_effect->value()->Get(tierOffset) * times
              );
            }
          }
        );
      }

      // Finishing walking tutorial.
      if (!g.run.walkingTutorialCompletedAt.IsSet()) {  ///
        if (Vector2DistanceSqr(PLAYER_CREATURE.pos, WORLD_SIZEf / 2.0f)
            >= SQR(WALKING_TUTORIAL_RADIUS_METERS))
        {
          g.run.walkingTutorialCompletedAt.SetNow();

          if (!g.player.runsWon)
            Metric("g_Run0_TutorialCompleted");

          ASSERT_FALSE(g.run.waveStartedAt.IsSet());
          g.run.waveStartedAt.SetNow();
        }
      }
    }

    // Launching music after completing walking tutorial.
    {  ///
      static bool once = false;
      if (!once                                        //
          && g.run.walkingTutorialCompletedAt.IsSet()  //
          && ge.meta._soundManager.CanPlaySound())
      {
        once = true;
        PlaySound(Sound_MUSIC_BATTLE);
      }
    }

    // Removing old picked up pickupables.
    {  ///
      ZoneScopedN("Removing old picked up pickupables.");

      const auto total = g.run.pickupables.count;
      int        off   = 0;
      FOR_RANGE (int, i, total) {
        const auto& pickupable = g.run.pickupables[i - off];

        bool needsRemoving = false;

        const auto fb = fb_pickupables->Get(pickupable.type);

        if (pickupable.pickedUpAt.IsSet()
            && (pickupable.pickedUpAt.Elapsed() >= PICKUPABLE_FADE_FRAMES))
        {
          if (pickupable.type == PickupableType_COIN) {
            MakeParticles({
              .type                   = ParticleType_COIN,
              .count                  = GRAND.RandInt(3, 6),
              .pos                    = PLAYER_CREATURE.pos,
              .velocity               = 4.0f,
              .velocityPlusMinus      = 0.5f,
              .velocityAngle          = 0,
              .velocityAnglePlusMinus = PI32,
              .initialOffset          = 0.25f,
              .initialOffsetPlusMinus = 0.1f,
            });
          }

          needsRemoving = true;
        }
        else if (pickupable.startedFlyingAt.IsSet()
                 && (pickupable.startedFlyingAt.Elapsed() >= WAVE_COMPLETED_COINS_FLYING_FRAMES))
        {
          const auto& d = pickupable.DataCoin();
          OnCoinFlewToStash(d.amount);
          needsRemoving = true;
        }

        if (needsRemoving) {
          PlaySound(fb->pickup_sound_hash());
          if (!pickupable.startedFlyingAt.IsSet())
            OnPickedUp(i - off);
          g.run.pickupables.UnstableRemoveAt(i - off);
          off++;
        }
      }
    }

    // Cheat. Mobs burn.
    if (gdebug.mobsBurn) {  ///
      for (auto& c : g.run.creatures) {
        bool hasFire = false;

        if (fb_creatures->Get(c.type)->hostility_type() == HostilityType_FRIENDLY)
          continue;

        for (auto& ailment : c.ailments) {
          if (ailment.c.type == AilmentType_BURN) {
            hasFire = true;
            break;
          }
        }

        if (!hasFire) {
          ApplyAilment({
            .c{
              .type            = AilmentType_BURN,
              .initialTimes    = 1,
              .remainingTimes  = 1,
              .remainingSpread = 0,
              .damage          = 1,
            },
            .creature = &c,
          });
        }
      }
    }

    // Burning spread.
    {  ///
      ZoneScopedN("Burning spread.");

      for (auto& creature : g.run.creatures) {
        for (auto& ailment : creature.ailments) {
          ASSERT(ailment.c.type);
          ASSERT(ailment.c.type == AilmentType_BURN);

          if ((ailment.c.remainingSpread <= 0))
            continue;

          auto       fb         = fb_creatures->Get(creature.type);
          const auto isFriendly = (fb->hostility_type() == HostilityType_FRIENDLY);

          for (auto& otherCreature : g.run.creatures) {
            if (creature.id == otherCreature.id)
              continue;
            if (otherCreature.diedAt.IsSet())
              continue;

            auto fb_other = fb_creatures->Get(otherCreature.type);
            if (fb_other->hostility_type() == HostilityType_FRIENDLY)
              continue;

            bool sameWeaponAppliedAilment = false;

            for (auto& a : otherCreature.ailments) {
              if (a.c.weaponIndex == ailment.c.weaponIndex) {
                sameWeaponAppliedAilment = true;
                break;
              }
            }

            if (sameWeaponAppliedAilment)
              continue;

            bool canBurn = true;
            auto resists = fb_other->resists_ailment_types();
            if (resists) {
              for (auto x : *resists) {
                if (x == AilmentType_BURN) {
                  canBurn = false;
                  break;
                }
              }
            }
            if (!canBurn)
              continue;

            const auto d = Vector2DistanceSqr(creature.pos, otherCreature.pos);
            if (d > SQR(BURNING_SPREAD_DISTANCE))
              continue;

            AilmentCommonData otherAilment = ailment.c;
            otherAilment.remainingSpread   = 0;
            ApplyAilment({
              .c        = otherAilment,
              .creature = &otherCreature,
            });
            ailment.c.remainingSpread--;

            if (ailment.c.remainingSpread <= 0)
              break;
          }
        }
      }
    }

    // Creatures ailments.
    {  ///
      ZoneScopedN("Creatures ailments.");

      int burningEnemies = 0;

      int creatureIndex = -1;
      for (auto& creature : g.run.creatures) {
        creatureIndex++;

        const auto fb = fb_creatures->Get(creature.type);

        int ailmentsTotal = creature.ailments.count;
        int off           = 0;

        bool emitFire = false;

        FOR_RANGE (int, ailmentIndex, ailmentsTotal) {
          auto& a = creature.ailments[ailmentIndex - off];
          ASSERT(a.c.type);
          ASSERT(a.c.type == AilmentType_BURN);
          ASSERT(a.startedAt.IsSet());

          if (a.c.type == AilmentType_BURN)
            emitFire = true;

          // Burning DoT (damage over time).
          if ((a.c.type == AilmentType_BURN)
              && (((a.startedAt.Elapsed().value + 1) % BURNING_RATE.value) == 0))
          {
            a.c.remainingTimes--;
            bool burned = (creature.health > 0);

            auto fb = fb_creatures->Get(creature.type);

            if ((creature.health > 0)        //
                && !creature.diedAt.IsSet()  //
                && (fb->hostility_type() != HostilityType_FRIENDLY))
              burningEnemies++;

            int tierOffset = 0;
            if (a.c.weaponIndex >= 0) {
              const auto& w = g.run.state.weapons[a.c.weaponIndex];
              tierOffset    = w.tier - fb_weapons->Get(w.type)->min_tier_index();
            }

            TryApplyDamage({
              .creatureIndex = creatureIndex,
              .damage
              = ApplyDamageScalings(a.c.damage, tierOffset, a.c.damageScalings, 1),
              .damagerCreatureType                = CreatureType_PLAYER,
              .critDamageMultiplier               = BURNING_CRIT_DAMAGE_MULTIPLIER,
              .indexOfWeaponThatDidDamageOrMinus1 = a.c.weaponIndex,
              .weaponCanApplyAilment              = false,
            });

            if (creature.health > 0)
              burned = false;

            if (burned)
              AchievementAdd(AchievementType_ENEMIES_DIED_BECAUSE_OF_BURNING, 1);
          }

          // Decay.
          ASSERT(a.c.remainingTimes >= 0);
          if (a.c.remainingTimes <= 0) {
            creature.ailments.UnstableRemoveAt(ailmentIndex - off);
            off++;
          }
        }

        if (emitFire) {
          EmitParticles({
            .fb_emitter           = glib->burning_creature_particle_emitter(),
            .pos                  = creature.pos,
            .offsetPlusMinusScale = Vector2One() * MAX(0, fb->hurtbox_radius() - 0.4f),
            .velocity             = GetCreatureSpeed(creature),
            .velocityAngle        = Vector2AngleOrRandom(creature.controller.move),
          });
        }
      }

      AchievementMax(AchievementType_SIMULTANEOUSLY_BURNING_ENEMIES, burningEnemies);
    }

    // Creatures moving.
    {  ///
      ZoneScopedN("Creatures moving.");

      for (auto& creature : g.run.creatures) {
        if (creature.diedAt.IsSet())
          continue;

        const auto fb = fb_creatures->Get(creature.type);

        f32 speed = GetCreatureSpeed(creature);
        if (creature.type == CreatureType_PLAYER)
          speed *= MAX(0, 1 + (f32)g.run.dynamicStats[StatType_SPEED] / 100.0f);

        speed *= b2Body_GetMass(creature.body.id) * BODY_LINEAR_DAMPING_SPEED_SCALE;

        b2Body_ApplyLinearImpulseToCenter(
          creature.body.id, ToB2Vec2(creature.controller.move * (FIXED_DT * speed)), true
        );
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

          creature.movementVisualFactor
            = MoveTowardsF(creature.movementVisualFactor, 0, FIXED_DT * 5);
          if (creature.movementVisualFactor <= 0)
            creature.movementAccumulator = 0;
        }
        else {
          auto v = Vector2Length(velocity) * FIXED_DT;
          creature.movementAccumulator += v;
          creature.movementAccumulatorVisual += v;

          while (creature.movementAccumulatorVisual * MOVEMENT_CYCLE_METERS >= 2 * PI32) {
            creature.movementAccumulatorVisual -= 2 * PI32 / MOVEMENT_CYCLE_METERS;
            if (creature.type == CreatureType_PLAYER)
              PlaySound(Sound_GAME_STEPS_GRASS);
          }

          if (creature.idleStartedAt.IsSet())
            creature.idleStartedAt = {};

          creature.movementVisualFactor
            = MoveTowardsF(creature.movementVisualFactor, 1, FIXED_DT * 5);
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

        f32 minDist              = f32_inf;
        int closestCreatureIndex = -1;

        // Resetting cooldown.
        const int  tierOffset  = weapon.tier - fb->min_tier_index();
        const auto cooldownDur = ApplyAttackSpeedToDuration(lframe::FromSeconds(
          fb->cooldown()->Get(tierOffset) * (1 - fb->attack_or_cooldown_ratio())
        ));
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

            const auto dist = MAX(
              0, Vector2Distance(pos, creature.pos) - fb_creature->hurtbox_radius()
            );

            if (dist < minDist) {
              minDist              = dist;
              closestCreatureIndex = creatureIndex;
            }
          }

          bool targetSet = false;

          if (closestCreatureIndex >= 0) {
            const auto& closestCreature = g.run.creatures[closestCreatureIndex];
            auto        range = CalculateWeaponRangeMeters(weapon.type, weapon.tier);

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

            if (minDist < range) {
              // Melee weapons attack from center.
              // Ranged - from their offset positions.
              Vector2 targetFromPos = PLAYER_CREATURE.pos;
              if (fb->projectile_type())
                targetFromPos += GetPlayerWeaponOffset(weaponIndex);

              auto targetToPos = closestCreature.pos;

              if (fb->projectile_type()) {
                targetToPos = ForecastWhereProjectileWillHitCreature(
                  targetToPos,
                  closestCreature.controller.move * GetCreatureSpeed(closestCreature),
                  targetFromPos,
                  fb_projectiles->Get(fb->projectile_type())->speed()
                );
              }

              const auto dir = Vector2DirectionOrRandom(targetFromPos, targetToPos);

              // Only ranged weapons continue tracking target during shooting.
              if (fb->projectile_type())
                weapon.targetDir = dir;

              if (!weapon.startedShootingAt.IsSet()) {
                weapon.targetDir = dir;
                weapon.startedShootingAt.SetNow();
                weapon.thisWaveUseCount++;
                PlaySound(fb->use_sound_hash());
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

          const int tierOffset = weapon.tier - fb->min_tier_index();

          const auto shootingDur = ApplyAttackSpeedToDuration(lframe::FromSeconds(
            fb->cooldown()->Get(tierOffset) * fb->attack_or_cooldown_ratio()
          ));

          const auto projectileSpawnFrameFactors = fb->projectile_spawn_frame_factors();

          if (projectileType) {
            // It's a weapon that shoots projectiles (RANGED / ELEMENTAL damage types).
            ASSERT(projectileSpawnFrameFactors);

            bool       spawn     = false;
            const auto speedMult = GetPlayerStatAttackSpeedMultiplier();
            for (auto value : *projectileSpawnFrameFactors) {
              if (Ceil((f32)shootingDur.value * value / speedMult) == e.value) {
                spawn = true;
                break;
              }
            }

            if (spawn) {
              int pierce = 0;
              int bounce = 0;
              if (fb->projectile_pierce())
                pierce = fb->projectile_pierce()->Get(tierOffset);
              if (fb->projectile_bounce())
                bounce = fb->projectile_bounce()->Get(tierOffset);

              int toSpawn = 1;
              if (fb->projectile_count())
                toSpawn = fb->projectile_count()->Get(tierOffset);

              auto spawnPos = pos;
              if (fb->offset()) {
                spawnPos += Vector2Rotate(
                  {fb->offset()->x(), 0}, Vector2AngleOrZero(weapon.targetDir)
                );
              }

              FOR_RANGE (int, projectileIndex, toSpawn) {
                auto dir = weapon.targetDir;
                if (fb->accuracy_plus_minus() != 0)
                  dir = Vector2Rotate(dir, GRAND.FRand11() * fb->accuracy_plus_minus());

                int weaponPiercingDamageBonusPercent = 0;
                if (fb->projectile_piercing_damage_bonus_percent()) {
                  weaponPiercingDamageBonusPercent
                    = fb->projectile_piercing_damage_bonus_percent()->Get(tierOffset);
                }

                MakeProjectile({
                  .c{
                    .type                = projectileType,
                    .ownerCreatureType   = PLAYER_CREATURE.type,
                    .weaponIndexOrMinus1 = weaponIndex,
                    .pos                 = spawnPos,
                    .dir                 = dir,
                    .range = CalculateWeaponRangeMeters(weapon.type, weapon.tier),
                    .damage
                    = CalculateWeaponDamage(weaponIndex, weapon.type, weapon.tier, true),
                    .critDamageMultiplier = fb->crit_damage_multiplier()->Get(tierOffset),
                    .weaponCritChance     = fb->crit_chance()->Get(tierOffset),
                    .knockbackMeters
                    = fb->knockback_meters()->Get(tierOffset) * KNOCKBACK_SCALE,
                    .pierce                           = pierce,
                    .weaponPiercingDamageBonusPercent = weaponPiercingDamageBonusPercent,
                    .bounce                           = bounce,
                  },
                });
              }

              weapon.lastShotAt = {};
              weapon.lastShotAt.SetNow();
            }
          }
          else {
            // It's a weapon that gets "shot" itself (MELEE / ELEMENTAL damage types).
            // Not projectile.
            ASSERT(!projectileSpawnFrameFactors);

            auto dur = shootingDur;
            if (dur.value > MELEE_WEAPON_MAX_ATTACK_DURATION.value)
              dur.value = MELEE_WEAPON_MAX_ATTACK_DURATION.value;

            const auto p = e.Progress(dur);

            if ((0.125f <= p)
                && ((p <= 0.5f) || ((weapon.lastCollisionCheckShootingProgress < 0.5f) && (p >= 0.5f))))
            {
              weapon.lastCollisionCheckShootingProgress = p;

              const auto texID = fb->texture_ids()->Get(0);
              Vector2    colliderSize{
                (f32)glib->original_texture_sizes()->Get(texID)->x() / 2.0f
                  + MELEE_WEAPON_COLLIDER_PHANTOM_RANGE_METERS,
                (f32)fb->melee_collider_height_px(),
              };
              colliderSize *= ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;

              colliderSize.x += CalculateWeaponRangeMeters(weapon.type, weapon.tier);

              auto pos = PLAYER_CREATURE.pos + weapon.targetDir * colliderSize.x / 2.0f;

              if (gdebug.gizmos) {
                *g.run.meleeWeaponColliderGizmos.Add() = {
                  .pos      = pos,
                  .size     = colliderSize,
                  .rotation = Vector2AngleOrZero(weapon.targetDir),
                };
              }

              CheckCollisionsRect(
                ShapeCategory_STATIC,
                (u32)ShapeCategory_CREATURE,
                pos,
                colliderSize,
                weapon.targetDir,
                OnWeaponCollided,
                &weaponIndex
              );
            }
          }

          if (e >= shootingDur) {
            const auto variation      = (int)(cooldownDur.value / 10);
            int        cooldownFrames = GRAND.RandInt(-variation, variation);

            IterateOverEffects(
              EffectConditionType_X__COOLDOWN_SECONDS_EVERY__Y__HITS,
              weaponIndex,
              [&](
                Weapon* w,
                int     wi,
                auto    fb_effect,
                int     tierOffset,
                int     times,
                int     thisWaveAddedCount
              ) BF_FORCE_INLINE_LAMBDA {
                if ((w->thisWaveUseCount % EFFECT_Y_INT) == 0) {
                  cooldownFrames
                    += lframe::FromSeconds(EFFECT_X_FLOAT * (f32)times).value;
                }
              }
            );

            weapon.startedShootingAt = {};
            weapon.piercedCount      = 0;
            weapon.thisWaveUseCount++;
            weapon.cooldownStartedAt.SetNow();
            weapon.cooldownStartedAt._value += cooldownFrames;
            weapon.lastCollisionCheckShootingProgress = 0;
          }
        }
      }
    }

    // Making projectiles.
    {  ///
      for (auto& data : g.run.projectilesToMake) {
        static int nextProjectileId = 1;

        auto fb = fb_projectiles->Get(data.c.type);

        f32 rotationSpeed
          = fb->rotation_speed() + fb->rotation_speed_plus_minus() * GRAND.FRand11();

        PlaySound(fb->spawn_sound_hash());
        Projectile projectile{
          .c             = data.c,
          .id            = nextProjectileId++,
          .rotationSpeed = rotationSpeed,
        };
        projectile.createdAt.SetNow();

        if (data.alreadyDamagedCreatureID) {
          projectile.damagedCreatureIDs[projectile.damagedCount++]
            = data.alreadyDamagedCreatureID;
        }

        *g.run.projectiles.Add() = projectile;

        g.run.projectileCounts[data.c.type]++;

        if (fb->max_simultaneous_count_achievement_type()) {
          AchievementMax(
            (AchievementType)fb->max_simultaneous_count_achievement_type(),
            g.run.projectileCounts[data.c.type]
          );
        }
      }
      g.run.projectilesToMake.Reset();
    }

    // Updating projectiles:
    // - Movement.
    // - Marking to remove because of travel distance.
    // - Mob collisions.
    // - Marking to remove because of pierce count.
    // - Playing trail sounds.
    {  ///
      ZoneScopedN("Updating projectiles.");

      constexpr Rect projectilesAliveBounds{
        .pos{-2 * WORLD_X, -2 * WORLD_Y},
        .size{(f32)WORLD_X * 5, (f32)WORLD_Y * 5},
      };

      int projectileIndex = -1;
      for (auto& projectile : g.run.projectiles) {
        projectileIndex++;

        const auto fb       = fb_projectiles->Get(projectile.c.type);
        const auto distance = FIXED_DT * fb->speed();
        projectile.travelledDistance += distance;
        projectile.c.pos += projectile.c.dir * distance;

        UpdateTrailSound(&projectile.nextTrailSoundVisualFrame, fb->trailsound_type());

        bool    createAoe = false;
        Vector2 aoePos    = projectile.c.pos;

        // Removing projectiles that travelled far enough.
        if (projectile.travelledDistance >= projectile.c.range) {
          if (!g.run.projectilesToRemove.Contains(projectileIndex)) {
            *g.run.projectilesToRemove.Add() = projectileIndex;
            if (fb->aoe_on_travel_end())
              createAoe = true;
          }
        }

        // Removing projectiles that are outside the world.
        if (!projectilesAliveBounds.ContainsInside(projectile.c.pos)) {
          if (!g.run.projectilesToRemove.Contains(projectileIndex))
            *g.run.projectilesToRemove.Add() = projectileIndex;
        }

        int start = 0;
        int end   = g.run.creatures.count;

        auto fb_owner = fb_creatures->Get(projectile.c.ownerCreatureType);
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
                projectile.damagedCreatureIDs.base, projectile.damagedCount, creature.id
              ))
            continue;

          const auto fb_creature = fb_creatures->Get(creature.type);
          if (fb_owner->hostility_type() == fb_creature->hostility_type())
            continue;

          const auto distSqr = Vector2DistanceSqr(creature.pos, projectile.c.pos);

          const auto radius = fb->collider_radius() + fb_creature->hurtbox_radius();

          if (distSqr < SQR(radius)) {
            Vector2 knockbackDirection{};

            int damage = projectile.c.damage;
            if (projectile.c.ownerCreatureType == CreatureType_PLAYER) {
              // Player damages mob.
              knockbackDirection
                = Vector2DirectionOrRandom(PLAYER_CREATURE.pos, creature.pos);
            }

            f32 piercingDamageMultiplier
              = (f32)(g.run.dynamicStats[StatType_PIERCE_DAMAGE]
                      + projectile.c.weaponPiercingDamageBonusPercent)
                / 100.0f;
            piercingDamageMultiplier = Clamp01(piercingDamageMultiplier);

            f32 knockback = projectile.c.knockbackMeters;

            FOR_RANGE (int, i, projectile.piercedCount) {
              damage = Round((f32)damage * (piercingDamageMultiplier));
              knockback *= piercingDamageMultiplier;
            }

            bool wasCrit = false;

            if (TryApplyDamage({
                  .creatureIndex                      = creatureIndex,
                  .damage                             = damage,
                  .directionOrZero                    = knockbackDirection,
                  .knockbackMeters                    = knockback,
                  .damagerCreatureType                = projectile.c.ownerCreatureType,
                  .critDamageMultiplier               = projectile.c.critDamageMultiplier,
                  .indexOfWeaponThatDidDamageOrMinus1 = projectile.c.weaponIndexOrMinus1,
                  .outWasCrit                         = &wasCrit,
                }))
            {
              auto fb_weapon = ((projectile.c.weaponIndexOrMinus1 >= 0)
                ? fb_weapons->Get(g.run.state.weapons[projectile.c.weaponIndexOrMinus1].type)
                : nullptr);

              if (!projectile.c.dontSpawnProjectilesOnHit
                  && (projectile.c.weaponIndexOrMinus1 >= 0))
              {
                const auto& weapon
                  = g.run.state.weapons[projectile.c.weaponIndexOrMinus1];
                const int tierOffset = weapon.tier - fb_weapon->min_tier_index();

                EffectSpawnProjectilesOnHit(creature, projectile.c.weaponIndexOrMinus1);
              }

              // Applying:
              // - EffectConditionType_CRITS_PIERCE_UP_TO__X__TIMES.
              // - EffectConditionType_CRITS_BOUNCE_UP_TO__X__TIMES.
              if (wasCrit && (projectile.c.weaponIndexOrMinus1 >= 0)) {
                const auto& weapon
                  = g.run.state.weapons[projectile.c.weaponIndexOrMinus1];
                const int tierOffset = weapon.tier - fb_weapon->min_tier_index();

                IterateOverEffects(
                  EffectConditionType_CRITS_PIERCE_UP_TO__X__TIMES,
                  projectile.c.weaponIndexOrMinus1,
                  [&](
                    Weapon* w,
                    int     wi,
                    auto    fb_effect,
                    int     tierOffset,
                    int     times,
                    int     thisWaveAddedCount
                  ) BF_FORCE_INLINE_LAMBDA {
                    if (projectile.effectCritPierce < EFFECT_X_INT)
                      projectile.c.pierce += times * EFFECT_X_INT;
                  }
                );
                IterateOverEffects(
                  EffectConditionType_CRITS_BOUNCE_UP_TO__X__TIMES,
                  projectile.c.weaponIndexOrMinus1,
                  [&](
                    Weapon* w,
                    int     wi,
                    auto    fb_effect,
                    int     tierOffset,
                    int     times,
                    int     thisWaveAddedCount
                  ) BF_FORCE_INLINE_LAMBDA {
                    if (projectile.effectCritBounce < EFFECT_X_INT)
                      projectile.c.bounce += times;
                  }
                );

                projectile.effectCritPierce++;
                projectile.effectCritBounce++;
              }

              auto maxBounce = projectile.c.bounce;
              auto maxPierce = projectile.c.pierce;
              if (projectile.c.ownerCreatureType == CreatureType_PLAYER) {
                maxBounce += g.run.dynamicStats[StatType_BOUNCE];
                maxPierce += g.run.dynamicStats[StatType_PIERCE];
              }

              bool canBounce = fb->can_bounce() && (projectile.bouncedCount < maxBounce);
              bool canPierce = projectile.piercedCount < maxPierce;
              if (projectile.damagedCount >= projectile.damagedCreatureIDs.count) {
                canBounce = false;
                canPierce = false;
              }

              if (canBounce) {
                projectile.damagedCreatureIDs[projectile.damagedCount++] = creature.id;
                projectile.bouncedCount++;
                AchievementMax(
                  AchievementType_MAX_SINGLE_PROJECTILE_BOUNCED, projectile.bouncedCount
                );
                projectile.lastBouncedAt = {};
                projectile.lastBouncedAt.SetNow();
                projectile.travelledDistance = 0;

                auto v = projectile.damagedCreatureIDs.ToView();
                ASSERT(v.count >= projectile.damagedCount);
                v.count = projectile.damagedCount;

                projectile.c.dir = CalculatePlayerProjectileBounceDirection(
                  projectile.c.type, projectile.c.pos, projectile.c.range, v
                );
              }
              else if (canPierce) {
                projectile.damagedCreatureIDs[projectile.damagedCount++] = creature.id;
                projectile.piercedCount++;
                AchievementMax(
                  AchievementType_MAX_SINGLE_PROJECTILE_PIERCED, projectile.piercedCount
                );
              }
              else if (!g.run.projectilesToRemove.Contains(projectileIndex)) {
                *g.run.projectilesToRemove.Add() = projectileIndex;
                aoePos = Vector2Lerp(aoePos, creature.pos, 0.5f);
              }

              PlaySound(fb->damage_sound_hash());

              createAoe = true;
            }
          }
        }

        // AOE.
        if (createAoe                                   //
            && projectile.c.canExplode                  //
            && (projectile.c.weaponIndexOrMinus1 >= 0)  //
            && ShouldExplode(projectile.c.weaponIndexOrMinus1))
        {
          projectile.c.canExplode = false;
          MakeAOE(
            projectile.c.ownerCreatureType,
            aoePos,
            glib->explosion_radius() * fb->explosion_size_scale(),
            projectile.c.damage,
            projectile.c.critDamageMultiplier,
            projectile.c.weaponCritChance,
            projectile.c.knockbackMeters,
            projectile.c.weaponIndexOrMinus1
          );
        }

        // Emitting particles.
        if (fb->particle_emitter()) {
          EmitParticles({
            .fb_emitter     = fb->particle_emitter(),
            .pos            = projectile.c.pos,
            .offsetRotation = Vector2Angle(projectile.c.dir),
          });
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
          int damage = landmine.damage;
          damage     = ApplyDamageScalings(damage, 0, landmine.damageScalings, 1);
          MakeAOE(
            CreatureType_PLAYER,
            landmine.pos,
            glib->landmine_explosion_radius(),
            damage,
            glib->landmine_crit_damage_multiplier(),
            0,
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

      for (const auto& garden : g.run.gardens) {
        auto interval = garden.spawnsAppleEvery.value;
        interval /= GetPlayerStatStructureAttackSpeedMultiplier();
        interval = MAX(2, interval);

        if ((garden.spawnedAt.Elapsed().value + 1) % interval != 0)
          continue;

        Vector2 pos{};
        do {
          const f32 off = Lerp(
            GARDEN_PICKUPABLE_SPAWN_RADIUS_MIN,
            GARDEN_PICKUPABLE_SPAWN_RADIUS_MAX,
            GRAND.FRand()
          );
          pos = garden.pos + Vector2Rotate({off, 0}, GRAND.Angle());
        } while (!CREATURES_WORLD_SPAWN_BOUNDS.ContainsInside(pos));

        MakePickupable({
          .type = PickupableType_APPLE,
          .pos  = pos,
        });
      }
    }

    // Processing EffectConditionType_YOU_GET_HURT_BY__X__DAMAGE_PER_SECOND
    if (g.run.waveStartedAt.IsSet()) {  ///
      IterateOverEffects(
        EffectConditionType_YOU_GET_HURT_BY__X__DAMAGE_PER_SECOND,
        -1,
        [&](
          Weapon* w,
          int     wi,
          auto    fb_effect,
          int     tierOffset,
          int     times,
          int     thisWaveAddedCount
        ) BF_FORCE_INLINE_LAMBDA {
          auto v = EFFECT_X_INT * times;
          if (v <= 0)
            return;
          if (((g.run.waveStartedAt.Elapsed().value + 1) % FIXED_FPS) == 0) {
            TryApplyDamage({
              .creatureIndex = 0,
              .damage        = v,
              .playerSound   = false,
            });
          }
        }
      );
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
        auto& projectile = g.run.projectiles[projectileIndex];
        g.run.projectileCounts[projectile.c.type]--;
        g.run.projectiles.UnstableRemoveAt(projectileIndex);
      }
      g.run.projectilesToRemove.Reset();

      qsort(
        (void*)g.run.projectiles.base,
        g.run.projectiles.count,
        sizeof(*g.run.projectiles.base),
        (int (*)(const void*, const void*))ProjectileCmp
      );
    }

    if (g.run.state.screen == ScreenType_GAMEPLAY) {
      // 0 - die.
      if (IsKeyPressed(SDL_SCANCODE_0)) {  ///
        TryApplyDamage({
          .creatureIndex = 0,
          .damage        = int_max,
        });
      }
    }

    // Processing `justDamagedCreatures`.
    {  ///
      ZoneScopedN("Processing `justDamagedCreatures`.");

      bool mobHurt = false;

      for (const auto index : g.run.justDamagedCreatures) {
        auto& creature = g.run.creatures[index];

        const auto fb = fb_creatures->Get(creature.type);

        if (creature.type != CreatureType_PLAYER && (creature.health <= 0))
          mobHurt = true;

        if (creature.health <= 0) {
          DestroyBody(&creature.body);
          creature.diedAt.SetNow();

          if (creature.type == CreatureType_TREE)
            AchievementAdd(AchievementType_KILL_TREES, 1);

          if (!index) {
            // Player died.
            PlaySound(Sound_GAME_PLAYER_DIED);

            if (!g.run.scheduledWaveCompleted.IsSet()) {
              TriggerWaveCompleted(false);
              g.run.state.waveWon = false;
              AchievementAdd(AchievementType_DIE_FOR_THE_FIRST_TIME, 1);
              Save();
            }
          }
        }
      }

      if (g.run.schedulePlayerHurtSound) {
        PlaySound(Sound_GAME_PLAYER_HURT);
        PlaySound(Sound_GAME_PLAYER_HURT_KICK);
        g.run.schedulePlayerHurtSound = false;
      }
      if (mobHurt)
        PlaySound(Sound_GAME_MOB_HURT);

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

    ASSERT(g.run.state.notPickedUpCoins >= 0);

    // Player's weapons create particles and play trail sounds.
    if (!PLAYER_CREATURE.diedAt.IsSet()) {  ///
      FOR_RANGE (int, weaponIndex, g.run.state.weapons.count) {
        auto& weapon = g.run.state.weapons[weaponIndex];
        if (!weapon.type)
          continue;

        const auto fb = fb_weapons->Get(weapon.type);

        UpdateTrailSound(&weapon.nextTrailSoundVisualFrame, fb->trailsound_type());
        if (weapon.startedShootingAt.IsSet() || weapon.cooldownStartedAt.IsSet()) {
          UpdateTrailSound(
            &weapon.nextUseTrailSoundVisualFrame, fb->use_trailsound_type()
          );
        }

        auto fb_emitter = fb->particle_emitter();
        if (!fb_emitter)
          continue;

        Vector2 offsetScale{1, 1};
        if ((weapon.targetDir == Vector2Zero()) && (PLAYER_CREATURE.dir.x < 0))
          offsetScale.x *= -1;

        EmitParticles({
          .fb_emitter     = fb_emitter,
          .pos            = GetWeaponPos(weaponIndex),
          .offsetScale    = offsetScale,
          .offsetRotation = Vector2AngleOrZero(weapon.targetDir),
        });
      }
    }

    // Updating particles.
    for (auto& p : g.run.particles) {  ///
      p.pos += p.velocity * FIXED_DT;
      p.rotation += p.rotationSpeed * FIXED_DT;
    }

    // Removing old particles.
    {  ///
      ZoneScopedN("Removing old particles.");

      const auto total = g.run.particles.count;
      int        off   = 0;
      FOR_RANGE (int, i, total) {
        const auto& particle = g.run.particles[i - off];
        const auto  fb       = fb_particles->Get(particle.type);
        if (particle.createdAt.Elapsed() >= particle.duration) {
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

  DoUI();

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
    if (!x.shownAt.IsSet()) {
      x.shownAt.SetNow();
      PlaySound(Sound_GAME_ACHIEVEMENT);
    }
    else if (x.shownAt.Elapsed() >= ACHIEVEMENT_TOTAL_FRAMES)
      g.ui.justUnlockedAchievements.RemoveAt(0);
  }

  g.run.dangerHPLevelOverlayValue
    = MoveTowardsF(g.run.dangerHPLevelOverlayValue, 0, FIXED_DT / 3);

  g.meta.musicLowpassFactor = MoveTowardsF(
    g.meta.musicLowpassFactor,
    ((((g.run.state.screen == ScreenType_GAMEPLAY)
       || (g.run.state.screen == ScreenType_WAVE_END_ANIMATION))
      && !g.meta.paused)
       ? 1
       : 0.6f),
    FIXED_DT
  );

  if (ge.meta._soundManager.CanPlaySound())
    SetMusicLowpassFactor(g.meta.musicLowpassFactor);

  // Setting audio listener position to player's world pos.
  {  ///
    auto engine = &ge.meta._soundManager.engine;
    ma_engine_listener_set_position(
      engine, 0, PLAYER_CREATURE.pos.x, PLAYER_CREATURE.pos.y, 0
    );
    ma_engine_listener_set_world_up(engine, 0, 0, 0, 1);
  }

  // Cheat. Cycling stuff in shop.
  if ((g.run.state.screen == ScreenType_SHOP)  //
      && gdebug.cyclingStuffInShop             //
      && !(ge.meta.frameVisual % (FIXED_FPS / 6)))
  {  ///
    auto& v = g.run.state.shop.toPick[3];

    if (v.weapon) {
      v.weapon = (WeaponType)((int)v.weapon + 1);
      if (v.weapon >= WeaponType_COUNT) {
        v.weapon = {};
        v.item   = (ItemType)1;
      }
    }
    else if (v.item) {
      v.item = (ItemType)((int)v.item + 1);
      if (v.item >= ItemType_COUNT) {
        v.item   = {};
        v.weapon = (WeaponType)1;
      }
    }
    else
      v.weapon = (WeaponType)1;

    if (v.item)
      v.tier = fb_items->Get(v.item)->tier();
    else if (v.weapon)
      v.tier = fb_weapons->Get(v.weapon)->min_tier_index();
  }

  ge.meta.frameVisual++;
}

int GetTextureIDByProgress(const flatbuffers::Vector<int>* texs, f32 p) {  ///
  ASSERT(p >= 0);
  int index = p * texs->size();
  index     = MIN(index, texs->size() - 1);
  return texs->Get(index);
}

void GameDraw() {
  ZoneScoped;

  TEMP_USAGE(&g.meta.trashArena);
  TEMP_USAGE(&g.meta.transientDataArena);

  // Waiting for completion of save data loading.
  if (!ge.meta.loading)
    return;

  // Setup.
  // {  ///
  TEMP_USAGE(&g.meta.trashArena);

  const auto localization         = glib->localizations()->Get(ge.meta.localization);
  const auto localization_strings = localization->strings();

  const auto fb_atlasTextures = glib->atlas_textures();
  const auto fb_hostilities   = glib->hostilities();
  const auto fb_preSpawns     = glib->pre_spawns();
  const auto fb_creatures     = glib->creatures();
  const auto fb_weapons       = glib->weapons();
  const auto fb_projectiles   = glib->projectiles();
  const auto fb_pickupables   = glib->pickupables();
  const auto fb_numbers       = glib->numbers();
  const auto fb_particles     = glib->particles();
  const auto fb_builds        = glib->builds();
  // }

  BeginMode2D(&g.run.camera);

  // Drawing floor.
  {  ///
    auto fb_segments = glib->floor_segments();
    auto fb_corners  = glib->floor_corners();

    DrawGroup_Begin(DrawZ_FLOOR);
    DrawGroup_SetSortY(0);

    f32 floorOffsetY = -0.25f;

    const Color color = ColorFromRGBA(glib->biomes()->Get(g.player.biome)->floor_color());

    static_assert((WORLD_X % 4) == 0);
    static_assert((WORLD_Y % 4) == 0);

    constexpr int DEBUG_SEGMENT = -1;  // -1 = disable debugging.

    if (DEBUG_SEGMENT < 0) {
      DrawGroup_CommandRect({
        .pos{(f32)WORLD_X / 2, (f32)WORLD_Y / 2 + floorOffsetY},
        .size{(f32)WORLD_X - 1, (f32)WORLD_Y - 1},
        .color = color,
      });
    }
    else
      floorOffsetY = 0;

    FOR_RANGE (int, mode, 2) {  // 0 - back, 1 - front.
      Random floorRand{1};

      struct {
        Vector2 pos      = {};
        f32     rotation = {};
      } corners[]{
        {.pos{2, WORLD_Y - 2}, .rotation = 0},
        {.pos{2, 2}, .rotation = PI32 / 2},
        {.pos{WORLD_X - 2, 2}, .rotation = PI32},
        {.pos{WORLD_X - 2, WORLD_Y - 2}, .rotation = PI32 * 3 / 2},
      };

      for (const auto& c : corners) {
        auto fb = fb_corners->Get(floorRand.Rand() % fb_corners->size());
        auto offset
          = ToVector2(fb->offset()) * (ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE);

        DrawGroup_CommandTexture({
          .texID    = (mode ? fb->back_texture_id() : fb->front_texture_id()),
          .rotation = c.rotation,
          .pos   = c.pos + Vector2Rotate(offset, c.rotation) + Vector2(0, floorOffsetY),
          .color = (mode ? BLACK : color),
        });

        if (DEBUG_SEGMENT >= 0) {
          DrawGroup_CommandRect({
            .pos = c.pos,
            .size{4, 4},
            .color = Fade(RED, 0.15f),
          });
        }
      }

      int prevSegment = -1;
      LAMBDA (int, newSegmentIndex, ()) {
        while (1) {
          const int result = floorRand.Rand() % fb_segments->size();
          if ((result != prevSegment) || (fb_segments->size() == 1)) {
            prevSegment = result;
            return result;
          }
        }
        INVALID_PATH;
        return 0;
      };

      FOR_RANGE (int, x, (WORLD_X - 8) / 4) {
        auto fb = fb_segments->Get(newSegmentIndex());

        if (DEBUG_SEGMENT >= 0) {
          if (x == 0)
            continue;
          if (x == 2)
            continue;

          fb = fb_segments->Get(4);
        }

        auto offset
          = ToVector2(fb->offset()) * (ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE);

        int posX = 6 + x * 4;

        DrawGroup_CommandTexture({
          .texID = (mode ? fb->back_texture_id() : fb->front_texture_id()),
          .pos   = Vector2(posX, WORLD_Y) + offset + Vector2(0, floorOffsetY),
          .color = (mode ? BLACK : color),
        });

        DrawGroup_CommandTexture({
          .texID    = (mode ? fb->back_texture_id() : fb->front_texture_id()),
          .rotation = PI32,
          .pos
          = Vector2(posX, 0) + Vector2{offset.x, -offset.y} + Vector2(0, floorOffsetY),
          .color = (mode ? BLACK : color),
        });

        if (DEBUG_SEGMENT >= 0) {
          DrawGroup_CommandRect({
            .pos{posX, WORLD_Y - 1},
            .size{4, 2},
            .color = Fade(RED, 0.15f),
          });
        }
      }

      if (DEBUG_SEGMENT < 0) {
        FOR_RANGE (int, y, (WORLD_Y - 8) / 4) {
          auto fb = fb_segments->Get(newSegmentIndex());
          auto offset
            = ToVector2(fb->offset()) * (ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE);

          int posY = 6 + y * 4;

          DrawGroup_CommandTexture({
            .texID    = (mode ? fb->back_texture_id() : fb->front_texture_id()),
            .rotation = 3 * PI32 / 2,
            .pos      = Vector2(WORLD_X, posY) + Vector2Rotate(offset, 3 * PI32 / 2)
                   + Vector2(0, floorOffsetY),
            .color = (mode ? BLACK : color),
          });

          DrawGroup_CommandTexture({
            .texID    = (mode ? fb->back_texture_id() : fb->front_texture_id()),
            .rotation = PI32 / 2,
            .pos      = Vector2(0, posY) + Vector2Rotate(offset, PI32 / 2)
                   + Vector2(0, floorOffsetY),
            .color = (mode ? BLACK : color),
          });
        }
      }
    }

    const auto fb_prop_texture_ids
      = glib->biomes()->Get(g.player.biome)->prop_texture_ids();
    for (const auto& prop : g.run.props) {
      DrawGroup_CommandTexture({
        .texID    = fb_prop_texture_ids->Get(prop.variation),
        .rotation = prop.rotation,
        .pos      = prop.pos + Vector2(0, floorOffsetY),
        .scale{(prop.right ? 1 : -1), 1},
      });
    }

    DrawGroup_End();
  }

  // Drawing prespawns.
  {  ///
    const auto texID = glib->game_decal_pre_spawn_texture_id();

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

      auto e = spawn.createdAt.Elapsed();
      f32  p = Clamp01(
        InOutLerp(
          0, SPAWN_FRAMES.value, e.value, SPAWN_FRAMES.value, 10 * _BF_LOGICAL_FPS_SCALE
        )
        / (f32)SPAWN_FRAMES.value
      );

      auto scale = Vector2One() * EaseBounceSmallSmooth(p);
      if (spawn.rotation < 0)
        scale.x *= -1;

      DrawGroup_CommandTexture({
        .texID    = texID,
        .rotation = abs(spawn.rotation),
        .pos      = spawn.pos,
        .scale    = scale,
        .color    = Fade(ColorFromRGBA(color), p),
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

    f32 fade = MIN(
      1, creature.spawnedAt.Elapsed().Progress(lframe::FromSeconds(0.33f * 3.0f / 8.0f))
    );
    if (creature.diedAt.IsSet())
      fade *= Clamp01(1 - creature.diedAt.Elapsed().Progress(DIE_FRAMES));

    int texID = 0;
    f32 sy    = 0;
    if (creature.type == CreatureType_PLAYER)
      sy = fb->original_height_px();
    else {
      texID = fb->texture_ids()->Get(0);
      sy    = (f32)glib->original_texture_sizes()->Get(texID)->y();
    }
    sy *= ASSETS_TO_LOGICAL_RATIO / (f32)METER_LOGICAL_SIZE;
    ASSERT(sy > 0);

    Vector2 scale{1, 1};
    if (creature.dir.x < 0)
      scale.x *= -1;

    auto color = ColorFromRGBA(fb->color());
    if (creature.type == CreatureType_RANGER) {
      const auto& data = creature.DataRanger();

      if (data.startedShootingAt.IsSet()) {
        f32  t         = 0;
        auto scaleEase = EaseInQuad;

        auto e = data.startedShootingAt.Elapsed();
        if (e < MOB_RANGER_SHOOTING_FRAME)
          t = e.Progress(MOB_RANGER_SHOOTING_FRAME);
        else {
          t = 1
              - (e - MOB_RANGER_SHOOTING_FRAME)
                  .Progress(MOB_RANGER_SHOOTING_FRAMES - MOB_RANGER_SHOOTING_FRAME);
          scaleEase = EaseOutQuad;
        }

        t = Clamp01(t);
        scale *= Lerp(1, 1.2f, scaleEase(t));
        color = ColorLerp(color, palRed, t);
      }
    }
    else if (creature.type == CreatureType_RUSHER) {
      const auto& data = creature.DataRusher();
      if (data.startedRushingAt.IsSet()) {
        f32  t = 0;
        auto e = data.startedRushingAt.Elapsed();
        if (e < MOB_RUSHER_RUSH_PRE_FRAMES)
          t = e.Progress(MOB_RUSHER_RUSH_PRE_FRAMES);
        else {
          t = 1
              - (e - MOB_RUSHER_RUSH_TOTAL_FRAMES + MOB_RUSHER_RUSH_PRE_FRAMES)
                  .Progress(MOB_RUSHER_RUSH_POST_FRAMES);
        }
        t     = Clamp01(t);
        color = ColorLerp(color, palRed, t);
      }
    }
    else if (creature.type == CreatureType_TURRET) {
      const auto& data = creature.DataTurret();
      if (data.startedShootingAt.IsSet()) {
        scale *= Lerp(
          1.22f,
          1,
          EaseOutCubic(
            MIN(1, data.startedShootingAt.Elapsed().Progress(ANIMATION_1_FRAMES))
          )
        );
      }
    }

    auto flash = TRANSPARENT_WHITE;
    if (creature.lastDamagedFlashAt.IsSet()) {
      auto p
        = MIN(1, creature.lastDamagedFlashAt.Elapsed().Progress(DAMAGED_FLASHING_FRAMES));
      p     = EaseInQuad(p);
      flash = Fade(WHITE, 1 - p);
    }

    Vector2 movementScale{1, 1};
    f32     movementCycle = creature.movementAccumulatorVisual * MOVEMENT_CYCLE_METERS;
    f32     movementAmplitudeScale
      = 0.07f * EaseInQuad(creature.movementVisualFactor) * 3 / 4 / sy * 2;
    if (creature.type == CreatureType_PLAYER)
      movementAmplitudeScale *= 0.75f;
    movementScale += Vector2(sinf(movementCycle) * 1.1f, cosf(movementCycle))
                     * movementAmplitudeScale;

    DrawGroup_Begin(DrawZ_DEFAULT);

    const auto basePos
      = creature.pos + Vector2(0, fb->offset_y() + fb->shadow_offset_y());

    const auto creatureRotation = -sinf(movementCycle * SIGN(creature.dir.x)) * PI32
                                  / (40 / 1.0f) * creature.movementVisualFactor;

    bool setBaseline = false;

    const auto shadowPos
      = creature.pos
        + Vector2(fb->shadow_offset_x() * SIGN(scale.x), fb->shadow_offset_y());

    if (texID) {
      DrawGroup_CommandTexture(
        {
          .texID    = texID,
          .rotation = creatureRotation,
          .pos      = basePos,
          .anchor{0.5f, 0.5f + fb->shadow_offset_y() / sy},
          .scale = scale * movementScale,
          .color = Fade(color, fade),
          .flash = flash,
        },
        DrawCommandSetSortY_SET_BASELINE
      );
    }
    else
      DrawGroup_SetSortY(WorldPosToLogical(shadowPos, &g.run.camera).y);

    if (creature.type == CreatureType_PLAYER) {
      const auto fb_build = fb_builds->Get(g.player.build);

      IterateOverBuildTextures(
        g.player.build,
        [&](Vector2 anchor, Color color, int texID) BF_FORCE_INLINE_LAMBDA {
          if (scale.x < 0)
            anchor.x = 1 - anchor.x;
          DrawGroup_CommandTexture({
            .texID    = texID,
            .rotation = creatureRotation,
            .pos      = basePos,
            .anchor   = anchor,
            .scale    = scale * movementScale,
            .color    = Fade(color, fade),
            .flash    = flash,
          });
        }
      );

      const auto texs = glib->player_layer_texture_ids();
      FOR_RANGE (int, i, fb_build->layer_colors()->size()) {
      }

      const auto hat = glib->hats()->Get(fb_build->hat_type());

      auto anchor = ToVector2(hat->anchor());
      if (scale.x < 0)
        anchor.x = 1 - anchor.x;

      DrawGroup_CommandTexture({
        .texID    = hat->texture_id(),
        .rotation = creatureRotation,
        .pos      = basePos,
        .anchor   = anchor,
        .scale    = scale * movementScale,
        .color    = Fade(ColorFromRGBA(fb_build->hat_color()), fade),
        .flash    = flash,
      });
    }

    DrawGroup_End();

    {
      int texID       = 0;
      f32 shadowScale = 1;

      for (auto t : *glib->shadow_texture_ids()) {
        const f32 sx = (f32)glib->original_texture_sizes()->Get(t)->x()
                       * fb->shadow_scale() * ASSETS_TO_LOGICAL_RATIO
                       / (f32)METER_LOGICAL_SIZE;

        if ((!texID) || (sx >= CREATURE_COLLIDER_RADIUS * 2)) {
          texID       = t;
          shadowScale = CREATURE_COLLIDER_RADIUS * 2 / sx * fb->shadow_scale();
        }
        else
          break;
      }

      DrawGroup_OneShotTexture(
        {
          .texID = texID,
          .pos   = shadowPos,
          .scale = Vector2One() * (shadowScale * fb->shadow_scale()),
          .color = Fade(BLACK, fade * 0.33f),
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

        const auto fb         = fb_weapons->Get(weapon.type);
        const int  tierOffset = weapon.tier - fb->min_tier_index();

        auto pos = GetWeaponPos(i);

        f32     rotation = 0;
        Vector2 scale{0, 1};

        if (weapon.targetDir == Vector2Zero())
          scale.x = (creature.dir.x >= 0 ? 1 : -1);
        else {
          scale.x  = (weapon.targetDir.x >= 0 ? 1 : -1);
          rotation = Vector2Angle(weapon.targetDir) + ((scale.x < 0) ? (f32)PI32 : 0.0f);
        }

        if (fb->offset())
          pos += Vector2Rotate({0, fb->offset()->y() * scale.y}, rotation);

        if (weapon.lastShotAt.IsSet() && fb->projectile_type()) {
          auto p = weapon.lastShotAt.Elapsed().Progress(ANIMATION_0_FRAMES);
          p      = Clamp01(p);
          p      = EaseOutQuad(p);
          scale *= Lerp(1 + 0.4f * fb->shooting_feedback_scale(), 1.0f, p);
          pos -= Vector2Lerp(
            weapon.targetDir * (fb->shooting_feedback_scale() / 4.0f), {}, p
          );
        }

        f32 weaponFade = fade;

        if (fb->shoots_itself() && weapon.lastShotAt.IsSet()) {
          f32 t = MIN(1, weapon.lastShotAt.Elapsed().Progress(ANIMATION_1_FRAMES));
          weaponFade *= EaseInQuad(t);
        }

        DrawGroup_OneShotTexture(
          {
            .texID    = fb->texture_ids()->Get(0),
            .rotation = rotation,
            .pos      = pos,
            .scale    = scale,
            .color    = Fade(ColorFromRGBA(fb->color()), weaponFade),
          },
          PLAYER_WEAPONS_DRAW_Z[weaponsCount - 1][i]
        );
      }
    }
  }

  {
    f32 baseFade = 1;
    if (g.run.scheduledWaveCompleted.IsSet())
      baseFade *= MAX(0, 1 - g.run.scheduledWaveCompleted.Elapsed().Progress(DIE_FRAMES));

    // Drawing landmines.
    {  ///
      const auto texID = glib->game_landmine_texture_id();
      for (const auto& x : g.run.landmines) {
        f32 fade = baseFade;
        fade *= MIN(
          1, x.spawnedAt.Elapsed().Progress(lframe::FromSeconds(0.33f * 3.0f / 8.0f))
        );

        Vector2 scale{1, 1};
        if (x.startedDetonationAt.IsSet()) {
          f32 p = x.startedDetonationAt.Elapsed().Progress(LANDMINE_DETONATION_FRAMES);
          scale.x += 0.2f * sinf(2 * PI32 * p * 0.66f);
          scale.y -= 0.2f * sinf(2 * PI32 * p * 0.66f);

          fade *= 1 - EaseInQuad(p);
        }

        DrawGroup_OneShotTexture(
          {
            .texID = texID,
            .pos   = x.pos,
            .scale = scale,
            .color = Fade(WHITE, fade),
          },
          DrawZ_LANDMINES
        );
      }
    }

    // Drawing gardens.
    {  ///
      const auto texID = glib->game_garden_texture_id();

      constexpr int GARDEN_REVOLUTIONS = 8;

      Vector2 anchor{0.5f, 0.2f};
      Vector2 posOffset{
        0,
        -(f32)glib->original_texture_sizes()->Get(texID)->y() * ASSETS_TO_LOGICAL_RATIO
          / METER_LOGICAL_SIZE
      };
      posOffset *= anchor;

      for (const auto& x : g.run.gardens) {
        if (gdebug.gizmos) {
          DrawGroup_OneShotCircleLines({
            .pos    = x.pos,
            .radius = 0.4f,
            .color  = YELLOW,
          });
        }

        const auto e = x.spawnedAt.Elapsed();

        f32 fade = baseFade;
        fade *= MIN(1, e.Progress(lframe::FromSeconds(0.33f * 3.0f / 8.0f)));

        f32 p = (f32)(e.value % x.spawnsAppleEvery.value) / (f32)x.spawnsAppleEvery.value;
        p *= GARDEN_REVOLUTIONS;

        auto scale = Vector2(1, 1) * 1.2f;

        f32       ampl                 = 0.08f;
        const f32 spawnAppleAmplScale  = 5;
        const f32 spawnAppleScaleScale = 1;
        if (p < 1) {
          ampl *= Lerp(spawnAppleAmplScale, 1, p);
          scale *= Lerp(spawnAppleScaleScale, 1, p);
        }
        else if (p > GARDEN_REVOLUTIONS - 1) {
          ampl *= Lerp(1, spawnAppleAmplScale, p - (GARDEN_REVOLUTIONS - 1));
          scale *= Lerp(1, spawnAppleScaleScale, p - (GARDEN_REVOLUTIONS - 1));
        }

        p = fmodf(p, 1);

        auto breathing = sinf(2 * PI32 * (p + 0.55f));
        scale += Vector2(-breathing, breathing) * ampl;

        DrawGroup_OneShotTexture(
          {
            .texID  = texID,
            .pos    = x.pos + posOffset,
            .anchor = anchor,
            .scale  = scale,
            .color  = Fade(WHITE, fade),
          },
          DrawZ_DEFAULT
        );
      }
    }
  }

  // Drawing boss hp bar.
  if (bossCreatureIndex >= 0) {  ///
    const auto& creature = g.run.creatures[bossCreatureIndex];
    const int   texs[]{
      glib->ui_boss_bar_back_texture_id(),
      glib->ui_boss_bar_top_texture_id(),
    };

    DrawGroup_Begin(DrawZ_BOSS_HP);
    DrawGroup_SetSortY(0);

    FOR_RANGE (int, i, 2) {
      f32 rightMargin = 0;
      if (i)
        rightMargin = Clamp01(1 - (f32)creature.health / (f32)creature.maxHealth);
      DrawGroup_CommandTexture({
        .texID = texs[i],
        .pos   = creature.pos - Vector2(0, 1.35f),
        .sourceMargins{.right = rightMargin},
        .color = (i ? palRed : palWhite),
      });
    }

    DrawGroup_End();
  }

  // Drawing projectiles.
  {  ///
    DrawGroup_Begin(DrawZ_PROJECTILES);
    DrawGroup_SetSortY(0);

    for (const auto& projectile : g.run.projectiles) {
      const auto fb = fb_projectiles->Get(projectile.c.type);

      f32 rotation = Vector2Angle(projectile.c.dir);

      f32 drot = projectile.rotationSpeed
                 * projectile.createdAt.Elapsed().Progress(lframe::Unscaled(FIXED_FPS));
      if (Hash32((u8*)&projectile.id, sizeof(projectile.id)) % 2)
        drot *= -1;
      rotation += drot;

      auto createdOrBouncedAt = projectile.createdAt;
      if (projectile.lastBouncedAt.IsSet())
        createdOrBouncedAt = projectile.lastBouncedAt;

      Vector2 scale{fb->scale_x(), fb->scale_y()};
      if (projectile.c.dir.x < 0) {
        rotation += (f32)PI32;
        scale.x *= -1;
      }

      if (fb->disable_rotation())
        rotation = 0;

      if (fb->scales_in() != 1.0f) {
        scale *= Lerp(
          fb->scales_in(),
          1.0f,
          EaseInQuad(MIN(1, createdOrBouncedAt.Elapsed().Progress(ANIMATION_0_FRAMES)))
        );
      }

      f32 fade = 1;

      if (projectile.c.range != f32_inf) {
        const auto willDieIn = lframe::FromSeconds(
          (projectile.c.range - projectile.travelledDistance) / fb->speed()
        );
        fade *= EaseOutCubic(Clamp01(willDieIn.Progress(ANIMATION_0_FRAMES)));
      }

      if (fb->fades_in()) {
        auto p = createdOrBouncedAt.Elapsed().Progress(ANIMATION_0_FRAMES);
        fade *= EaseOutQuad(MIN(1, p));
      }

      if (fb->squashes_in()) {
        auto p = createdOrBouncedAt.Elapsed().Progress(ANIMATION_0_FRAMES);
        p      = MIN(1, p);
        p      = EaseOutCubic(p);
        scale.x *= Lerp(1.0f, 3.5f, p);
        scale.y *= Lerp(1, 0.4f, p);
      }

      int texID = GetTextureIDByProgress(
        fb->texture_ids(), Clamp01(projectile.travelledDistance / projectile.c.range)
      );

      DrawGroup_CommandTexture({
        .texID    = texID,
        .rotation = rotation,
        .pos      = projectile.c.pos,
        .scale    = scale,
        .color    = Fade(ColorFromRGBA(fb->color()), fade),
        .flash    = TRANSPARENT_WHITE,
      });
    }

    DrawGroup_End();
  }

  // Drawing projectiles gizmos.
  if (gdebug.gizmos) {  ///
    for (const auto& projectile : g.run.projectiles) {
      auto fb = fb_projectiles->Get(projectile.c.type);

      DrawGroup_OneShotCircleLines({
        .pos    = projectile.c.pos,
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

      if (gdebug.hideUIForVideo && fb->gets_hidden_by_debug_option())
        continue;

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

      auto color = Fade(ColorFromRGBA(fb->color()), EaseOutQuad(fade));

      DrawGroup_CommandText({
        .pos        = number.pos + Vector2(0, EaseABitUpThenDown(p) / 4.0f),
        .scale      = Vector2One() * ScaleDamageNumbersByProgress(p),
        .font       = &g.meta.fontUIBigOutlined,
        .text       = buffer,
        .bytesCount = (int)bytesCount,
        .color      = color,
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
      const auto fb = fb_particles->Get(particle.type);

      auto       e = particle.createdAt.Elapsed();
      const auto p = Clamp01(e.Progress(particle.duration));

      f32 fade = EaseOutQuad(1 - p);
      if (fb->fades_in())
        fade *= MIN(1, EaseOutQuad(e.Progress(ANIMATION_0_FRAMES)));

      if (fade < 0)
        continue;

      DrawGroup_CommandTexture({
        .texID = GetTextureIDByProgress(
          fb->variations()->Get(particle.variation)->texture_ids(), p
        ),
        .rotation = particle.rotation,
        .pos      = particle.pos,
        .scale    = Vector2(fb->scale_x(), fb->scale_y()) * particle.scale,
        .color    = Fade(particle.color, fade),
      });
    }
    DrawGroup_End();
  }

  f32 walkingTutorialFade = 1;
  if (g.run.walkingTutorialCompletedAt.IsSet()) {
    walkingTutorialFade = MAX(
      0, 1 - g.run.walkingTutorialCompletedAt.Elapsed().Progress(ANIMATION_1_FRAMES)
    );
  }

  // Drawing walking tutorial area.
  if (walkingTutorialFade > 0) {  ///
    const Color color = Fade({239, 203, 132, 255}, EaseOutQuad(walkingTutorialFade));

    const f32 breathingP
      = (sinf(2 * PI32 * (f32)(ge.meta.frameGame % (2 * FIXED_FPS)) / 2 / (f32)FIXED_FPS)
         + 1)
        / 2.0f;

    DrawGroup_Begin(DrawZ_TUTORIAL_AREA);
    DrawGroup_SetSortY(0);

    const auto c = WORLD_SIZEf / 2.0f;

    const f32 rotation = PI32 / 40;
    const f32 marginX  = (f32)WORLD_SIZE.x * 1.0f / 4.0f;

    if (ge.meta.device != DeviceType_MOBILE) {
      const int  keyTex     = glib->ui_input_key_texture_id();
      const int  arrowTex   = glib->ui_input_key_arrow_texture_id();
      const auto keyTexSize = ToVector2(glib->original_texture_sizes()->Get(keyTex))
                              * (ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE);

      struct {
        Vector2 offset        = {};
        f32     arrowRotation = 0;
      } keyboardKeyOffsets[]{
        {.offset{-1, -0.5f}, .arrowRotation = PI32},
        {.offset{0, -0.5f}, .arrowRotation = -PI32 / 2},
        {.offset{1, -0.5f}, .arrowRotation = 0},
        {.offset{0, 0.5f}, .arrowRotation = PI32 / 2},
      };

      for (auto o : keyboardKeyOffsets) {
        o.offset *= Lerp(1.05f, 1.15f, breathingP);
        auto off = Vector2Rotate(keyTexSize * o.offset, rotation);

        const Vector2 posKey{marginX + off.x, c.y + off.y};

        DrawGroup_CommandTexture({
          .texID    = keyTex,
          .rotation = rotation,
          .pos      = posKey,
          .color    = color,
        });

        DrawGroup_CommandTexture({
          .texID    = arrowTex,
          .rotation = rotation + o.arrowRotation,
          .pos      = posKey + Vector2Rotate({0, 0.09f}, rotation),
          .color    = BLACK,
        });
      }
    }

    const auto mouseDurPerStep = FIXED_FPS;

    // Steps:
    // 0. Mouse fast unfade
    // 1. Mouse hold
    // 2. Mouse move
    // 3. Mouse fade
    const int mouseSteps = 4;

    const auto fr = ge.meta.frameGame % (mouseSteps * mouseDurPerStep);

    const int step = fr / mouseDurPerStep;
    ASSERT(step >= 0);
    ASSERT(step < mouseSteps);

    const f32 stepP = (f32)(fr % mouseDurPerStep) / (f32)mouseDurPerStep;
    ASSERT(stepP >= 0);
    ASSERT(stepP <= 1);

    f32 fade = walkingTutorialFade;
    if (step == 0)
      fade *= EaseOutQuad(stepP);
    else if (step == 3)
      fade *= 1 - EaseInQuad(stepP);

    int mouseTexs[]{
      glib->ui_input_mouse_texture_id(),
      glib->ui_input_mouse_left_down_texture_id(),
    };

    if (ge.meta.device == DeviceType_MOBILE) {
      mouseTexs[0] = glib->ui_input_touch_texture_id();
      mouseTexs[1] = glib->ui_input_touch_down_texture_id();
    }

    int tex = mouseTexs[1];
    if (step == 0)
      tex = mouseTexs[0];

    Vector2       off{-1, 0.1f};
    const Vector2 offTarget{1, -0.1f};
    if (step == 2)
      off = Vector2Lerp(off, offTarget, EaseInOutQuad(stepP));
    else if (step == 3)
      off = offTarget;

    DrawGroup_CommandTexture({
      .texID    = tex,
      .rotation = -rotation,
      .pos      = Vector2(WORLD_SIZE.x - marginX, c.y) + off,
      .scale    = Vector2One(),
      .color    = Fade(color, fade),
    });

    DrawGroup_End();
  }

  // Gizmos. Colliders.
  if (gdebug.gizmos) {  ///
    DrawGroup_Begin(DrawZ_GIZMOS);
    DrawGroup_SetSortY(0);

    for (const auto& shape : g.run.bodyShapes) {
      if (!shape.active)
        continue;

      auto pos = ToVector2(b2Body_GetPosition(shape.body.id));

      auto color = shape.color;
      if (!b2Body_IsEnabled(shape.body.id))
        color = GRAY;

      switch (shape.type) {
      case BodyShapeType_CIRCLE: {
        DrawGroup_CommandCircleLines({
          .pos    = pos,
          .radius = shape.DataCircle().radius,
          .color  = color,
        });
      } break;

      case BodyShapeType_RECT: {
        DrawGroup_CommandRectLines({
          .pos  = pos,
          .size = shape.DataRect().size,
          .anchor{0.5f, 0.5f},
          .color = color,
        });
      } break;

      default:
        INVALID_PATH;
      }
    }

    for (const auto& rect : g.run.meleeWeaponColliderGizmos) {
      DrawGroup_CommandRectLines({
        .pos      = rect.pos,
        .size     = rect.size,
        .rotation = rect.rotation,
      });
    }

    DrawGroup_End();
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

    auto pos = pickupable.pos;

    if (pickupable.pickedUpAt.IsSet()) {
      const auto p
        = MIN(1, pickupable.pickedUpAt.Elapsed().Progress(PICKUPABLE_FADE_FRAMES));

      fade *= Clamp01(1 - p);

      pos = Vector2Lerp(pickupable.pos, PLAYER_CREATURE.pos, EaseInQuad(p));
    }

    fade = EaseOutCubic(fade);

    pos = WorldPosToLogical(pos, &g.run.camera);
    if (pickupable.startedFlyingAt.IsSet()) {
      auto p
        = pickupable.startedFlyingAt.Elapsed().Progress(WAVE_COMPLETED_COINS_FLYING_FRAMES
        );
      p   = Clamp01(p);
      p   = EaseInQuad(p);
      pos = Vector2Lerp(
        pos,
        g.ui.notPickedUpCoinsLogicalPos + Vector2(-1, 1) * g.meta.screenSizeUIMargin,
        p
      );
    }

    DrawGroup_Begin(DrawZ_PICKUPABLES);

    const bool isCoin = (pickupable.type == PickupableType_COIN);

    Vector2 scale{1, 1};
    if (pickupable.mirrored)
      scale.x *= -1;

    if (isCoin) {
      // Coin glows.
      DrawGroup_CommandTexture({
        .texID = glib->game_coin_glow_texture_id(),
        .pos   = pos,
        .scale = scale,
        .color = Fade(palYellow, fade / 6),
      });
    }

    DrawGroup_CommandTexture(
      {
        .texID    = fb->variation_texture_ids()->Get(pickupable.variation),
        .rotation = pickupable.rotation,
        .pos      = pos,
        .scale    = scale,
        .color    = Fade(WHITE, fade),
      },
      DrawCommandSetSortY_SET_BASELINE
    );

    DrawGroup_End();
  }

  // Drawing danger hp level vignette.
  if (g.run.dangerHPLevelOverlayValue > 0) {  ///
    DrawGroup_OneShotTexture(
      {
        .texID = glib->ui_vignette_danger_hp_level_texture_id(),
        .pos   = LOGICAL_RESOLUTIONf / 2.0f,
        .scale = Vector2One() * 4.2f,
        .color = Fade(palRed, MIN(1, g.run.dangerHPLevelOverlayValue)),
      },
      DrawZ_DANGER_HP_LEVEL_VIGNETTE
    );
  }

  // Drawing wave completion animation.
  if (g.run.scheduledWaveCompleted.IsSet()) {  ///
    auto p
      = Clamp01(g.run.scheduledWaveCompleted.Elapsed().Progress(WAVE_COMPLETED_FRAMES));

    auto textColor = palTextWhite;

    int locale = Loc_UI_WAVE_WON__CAPS;
    if (!g.run.state.waveWon) {
      locale    = Loc_UI_WAVE_LOST__CAPS;
      textColor = palTextRed;
    }
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

    int charsToShow
      = MIN(totalChars, Ceil((f32)totalChars * EaseOutQuad(MIN(1, p / (5 / 16.0f)))));

    f32 scale = 1.2f + 0.1f * sinf(2 * PI32 * p);
    if (p < 0.3f)
      scale *= Lerp(0.15f, 1, EaseOutQuad(Remap(p, 0, 0.3f, 0, 1)));
    if (p > 0.75f)
      scale *= Lerp(1, 7, EaseInQuad(Remap(p, 0.75f, 1, 0, 1)));

    f32 fade = EaseOutQuad(Clamp01(Remap(p, 0.7f, 0.9f, 1, 0)));

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
            (f32)LOGICAL_RESOLUTION.y / 2.0f,
          },
          .scale      = Vector2One() * scale,
          .font       = &g.meta.fontUIGiganticOutlined,
          .text       = text->c_str(),
          .bytesCount = bytesToShow,
          .color      = Fade(textColor, fade),
        },
        DrawZ_UI
      );
    }
  }

  // Drawing touch controls.
  if ((g.run.state.screen == ScreenType_GAMEPLAY)  //
      && !g.meta.paused                            //
      && g.meta.stickControl.controlling           //
      && !gdebug.hideUIForVideo)
  {  ///
    DrawGroup_Begin(DrawZ_TOUCH_CONTROLS);
    DrawGroup_SetSortY(0);

    const auto& c = g.meta.stickControl;

    const auto startPos  = c.startPos;
    auto       targetPos = c.targetPos;
    if (startPos != targetPos)
      targetPos = startPos + c.calculatedDir * g.ui.touchControlMaxLogicalOffset;

    const struct {
      Vector2 pos   = {};
      int     texID = {};
    } data[]{
      {.pos = startPos, .texID = glib->ui_controls_touch_base_texture_id()},
      {.pos = targetPos, .texID = glib->ui_controls_touch_handle_texture_id()},
    };
    for (auto& d : data) {
      DrawGroup_CommandTexture({.texID = d.texID, .pos = d.pos});
    }

    DrawGroup_End();
  }

  DoUI();

#if 0
  EngineApplyStrips();
  EngineApplyVignette();
#endif
  FlushDrawCommands();

  // Debug info.
  if (ge.meta.debugEnabled) {  ///
    if (0)
      IM::ShowDemoWindow();

    auto localizationEn = glib->localizations()->Get(1)->strings();

    if (IM::Begin("Debug") && IM::BeginTabBar("tabs")) {
      if (IM::BeginTabItem("info")) {
        IM::Text("Close debug menu: hold F1 -> press F2");

        IM::Text("");

        if (IM::Button("Reset Debug"))
          gdebug = {};

        IM::Checkbox("Gizmos", &gdebug.gizmos);
        IM::Checkbox("Emulating Mobile", &gdebug.emulatingMobile);
        IM::Checkbox("Hide UI For Video", &gdebug.hideUIForVideo);

        IM::Checkbox("Mobs Burn", &gdebug.mobsBurn);
        IM::Checkbox("Mobs Slow", &gdebug.mobsSlow);

        IM::Checkbox("Disable Mob Spawns", &gdebug.disableMobSpawns);
        IM::Checkbox("Disable Boss Spawn", &gdebug.disableBossSpawn);

        {
          IM::Text("Wave Duration: ");
          const char* waveDurationModes[]{"Default", "Short", "Very Short", "Long"};
          int         modeIndex = -1;
          for (const auto mode : waveDurationModes) {
            modeIndex++;
            IM::SameLine();
            if (IM::RadioButton(mode, gdebug.waveDurationMode == modeIndex))
              gdebug.waveDurationMode = modeIndex;
          }
        }

        IM::InputInt(
          "Tree Spawn Interval", &gdebug.overriddenTreeSpawnIntervalSeconds, 1, 5
        );
        gdebug.overriddenTreeSpawnIntervalSeconds
          = MIN(60, MAX(0, gdebug.overriddenTreeSpawnIntervalSeconds));

        IM::SliderFloat("Mob Spawn Rate", &gdebug.mobSpawnRate, -2, 2, "%.1f", 0);

        if (IM::Button("Next biome")) {
          g.player.biome = (BiomeType)((int)g.player.biome + 1);
          if (g.player.biome >= BiomeType_COUNT)
            g.player.biome = (BiomeType)((int)BiomeType_INVALID + 1);
          Save();
        }

        IM::Checkbox("Cycling Stuff In Shop", &gdebug.cyclingStuffInShop);

        ge.meta.device
          = (gdebug.emulatingMobile ? DeviceType_MOBILE : DeviceType_DESKTOP);

        IM::Text("");

        IM::Text("MarkGameplay: %d", ge.meta.markGameplay);

        IM::Text("F3 change localization");
        IM::Text("F4 change device");
        IM::Text("F5 +10 coins");

        if (g.run.state.screen == ScreenType_GAMEPLAY) {
          IM::Text("0 die");
          IM::Text("F6 complete wave");
          IM::Text("F7 show end screen");
          IM::Text("F8 add level");
          IM::Text("F9 add chest");
        }

        IM::Text("F10 unlock all achievements");
        IM::Text("N - increase wave. Shift N - decrease wave");

        LAMBDA (void, debugTextArena, (const char* name, const Arena& arena)) {
          IM::Text(
            "%s: %d %d (%.1f%%) (max: %d, %.1f%%)",
            name,
            (int)arena.used,
            (int)arena.size,
            100.0f * (f32)arena.used / (f32)arena.size,
            (int)arena.maxUsed,
            100.0f * (f32)arena.maxUsed / (f32)arena.size
          );
        };

        debugTextArena("ge.meta._arena", ge.meta._arena);
        debugTextArena("g.run.arena", g.run.arena);
        debugTextArena("g.meta.trashArena", g.meta.trashArena);
        debugTextArena("g.meta.transientDataArena", g.meta.transientDataArena);

#define X(type_, name_) \
  IM::Text("g.run." #name_ ": %d/%d", g.run.name_.count, g.run.name_.maxCount);
        VECTORS_TABLE;
#undef X
        IM::EndTabItem();
      }

      if (IM::BeginTabItem("items")) {
        if (IM::Button("Give all items")) {
          FOR_RANGE (int, i, ItemType_COUNT) {
            if (i)
              AddItem((ItemType)i);
          }
          Save();
        }

        IM::Text("");

        static char searchBuf[512]{};
        IM::InputTextWithHint(
          "##items-search", "Search by name or code", searchBuf, ARRAY_COUNT(searchBuf)
        );
        zpl_str_to_upper(searchBuf);

        FOR_RANGE (int, i, ItemType_COUNT) {
          if (!i)
            continue;

          auto fb = glib->items()->Get(i);

          TEMP_USAGE(&g.meta.trashArena);
          auto name = PushTextToArena(
            &g.meta.trashArena, localizationEn->Get(fb->name_locale())->c_str()
          );
          zpl_str_to_upper(name);

          // Search filtering.
          auto s = zpl_str_trim(searchBuf, false);
          if (s) {
            if (!(strstr(name, s) || strstr(fb->type()->c_str(), s)))
              continue;
          }

          if (IM::Button(TextFormat("%d: %s - %s", i, fb->type()->c_str(), name))) {
            AddItem((ItemType)i);
            Save();
          }
        }
        IM::EndTabItem();
      }

      if ((g.run.state.screen == ScreenType_SHOP) && IM::BeginTabItem("weapons")) {
        IM::Text("These buttons set shop's 2nd weapon");

        static char searchBuf[512]{};
        IM::InputTextWithHint(
          "##weapons-search", "Search by name or code", searchBuf, ARRAY_COUNT(searchBuf)
        );
        zpl_str_to_upper(searchBuf);

        FOR_RANGE (int, i, WeaponType_COUNT) {
          if (!i)
            continue;

          auto fb = fb_weapons->Get(i);

          TEMP_USAGE(&g.meta.trashArena);
          auto name = PushTextToArena(
            &g.meta.trashArena, localizationEn->Get(fb->name_locale())->c_str()
          );
          zpl_str_to_upper(name);

          // Search filtering.
          auto s = zpl_str_trim(searchBuf, false);
          if (s) {
            if (!(strstr(name, s) || strstr(fb->type()->c_str(), s)))
              continue;
          }

          if (IM::Button(TextFormat("%d: %s - %s", i, fb->type()->c_str(), name))) {
            g.run.state.shop.toPick[1] = {
              .weapon = (WeaponType)i,
              .tier   = fb->min_tier_index(),
            };
            Save();
          }
        }
        IM::EndTabItem();
      }

      if (IM::BeginTabItem("stats")) {
        FOR_RANGE (int, i, StatType_COUNT) {
          if (!i)
            continue;
          const auto stat = (StatType)i;
          IM::Text(
            "%d: %s %d (dyn: %d)",
            i,
            glib->stats()->Get(stat)->type()->c_str(),
            g.run.state.staticStats[stat],
            g.run.dynamicStats[stat]
          );
        }
        IM::EndTabItem();
      }

      IM::EndTabBar();
    }

    IM::End();
  }

  g.run.meleeWeaponColliderGizmos.Reset();
}

///
