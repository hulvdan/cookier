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

#define BF_CLAY_CUSTOM_NINE_SLICE(gamelibNineSlicePtr) \
  .custom {                                            \
    .customData = PushClayCustomData({                 \
      .type      = ClayCustomElementType_NINE_SLICE,   \
      .nineSlice = (gamelibNineSlicePtr),              \
    }),                                                \
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
  ClayCustomElementType_OVERLAY,
};

struct ClayCustomData {
  ClayCustomElementType    type           = {};
  u16                      alpha          = u16_max;
  Vector2                  translate      = {0, 0};
  Vector2                  scale          = {1, 1};
  const BFGame::NineSlice* nineSlice      = nullptr;
  Color                    nineSliceColor = WHITE;
  Color                    overlayColor   = MAGENTA;
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
  WeaponType   type              = {};
  Vector2      offset            = {};
  Vector2      detachedPos       = {};
  Vector2      targetDir         = {};
  LogicalFrame startedShootingAt = {};
  LogicalFrame cooldownStartedAt = {};
  int          tier              = {};
  int          recyclePrice      = {};
  f32          didDamage         = 0;

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

struct AilmentData {
  CreatureType ownerCreatureType = {};
  int          weaponIndex       = {};

  LogicalFrame startedAt = {};
  lframe       duration  = {};
  // int          tier      = {};
  f32 value = {};
};

struct Creature {
  int                id                  = {};
  CreatureType       type                = {};
  f32                health              = {};
  f32                maxHealth           = {};
  Vector2            pos                 = {};
  Vector2            dir                 = {};
  Body               body                = {};
  CreatureController controller          = {};
  LogicalFrame       lastDamagedAt       = {};
  LogicalFrame       diedAt              = {};
  f32                speed               = {};
  f32                speedModifier       = 1;
  f32                movementAccumulator = {};
  LogicalFrame       idleStartedAt       = {};

  Array<AilmentData, AilmentType_COUNT - 1> ailments = {};

  int lastDamagedWeaponIndex = -1;

  union {
    struct {
      LogicalFrame startedShootingAt;
    } ranger;

    struct {
      LogicalFrame startedRushingAt;
      LogicalFrame finishedRushingAt;
      lframe       cooldown;
      Vector2      rushingDir;
    } rusher;
  } _mob;

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

struct CreaturePreSpawn {
  CreatureType type      = {};
  Vector2      pos       = {};
  LogicalFrame createdAt = {};
};

struct Projectile {
  ProjectileType                    type               = {};
  CreatureType                      ownerCreatureType  = {};
  int                               weaponIndex        = -1;
  Vector2                           pos                = {};
  Vector2                           dir                = {};
  f32                               damage             = {};
  f32                               crit               = {};
  int                               pierce             = {};
  int                               bounce             = {};
  LogicalFrame                      createdAt          = {};
  Array<int, PROJECTILE_MAX_PIERCE> damagedCreatureIds = {};
  int                               damagedCount       = {};
  int                               piercedCount       = 0;
  int                               bouncedCount       = 0;
  f32                               knockbackMeters    = {};
  f32                               range              = {};
  f32                               travelledDistance  = 0;
};

struct MakeProjectileData {
  ProjectileType type              = {};
  CreatureType   ownerCreatureType = {};
  int            weaponIndex       = -1;
  Vector2        pos               = {};
  Vector2        dir               = {};
  f32            range             = {};
  f32            damage            = {};
  f32            crit              = {};
  f32            knockbackMeters   = {};
  int            pierce            = {};
  int            bounce            = {};
};

struct Number {
  NumberType   type      = {};
  int          value     = {};
  Vector2      pos       = {};
  LogicalFrame createdAt = {};
};

struct MakePickupableData {
  PickupableType type        = {};
  Vector2        pos         = {};
  int            coin_amount = {};
};

struct Pickupable {
  PickupableType type       = {};
  Vector2        pos        = {};
  LogicalFrame   createdAt  = {};
  LogicalFrame   pickedUpAt = {};

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
  return lframe::MakeUnscaled(seconds * FIXED_FPS);
}

enum ScreenType {
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

struct StringPlaceholderValue {
  int         id    = {};
  const char* value = {};
};

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

  struct Run {
    Arena arena = {};

    bool       reload = false;
    bool       won    = false;
    ScreenType screen = ScreenType_GAMEPLAY;

    LogicalFrame scheduledWaveCompleted = {};
    bool         waveWon                = false;
    bool         recalculatePlayerStats = false;
    bool         scheduledUI            = false;
    bool         scheduledPickedUpItems = false;
    bool         scheduledUpgrades      = false;
    bool         scheduledShop          = false;
    bool         scheduledEnd           = false;
    bool         scheduledNextWave      = false;

    bool showingSecondaryStats = false;

    Camera camera{
      .zoom          = METER_LOGICAL_SIZE,
      .texturesScale = 1.0f / METER_LOGICAL_SIZE,
    };

    b2WorldId world          = {};
    int       nextCreatureId = 0;
    int       toSpawn        = 3;

    Array<ThisWaveMob, CreatureType_COUNT> thisWaveMobs      = {};
    int                                    thisWaveMobsCount = 0;

    LogicalFrame playerLastLifestealAt = {};
    LogicalFrame playerLastRegenAt     = {};

    int playerKilledEnemies   = 0;
    int xpLevel               = 1;
    f32 xp                    = 0;
    f32 nextLevelXp           = 0;
    int coins                 = 0;
    int crates                = 0;
    int cratesDroppedThisWave = 0;
    int previousLevel         = 1;

    int          waveIndex     = 0;
    LogicalFrame waveStartedAt = {};

    Array<Weapon, PLAYER_WEAPONS_COUNT> playerWeapons = {};

    Array<int, StatType_COUNT> playerStatsWithoutItems = {};
    Array<int, StatType_COUNT> playerStats             = {};

    // Using "X-macros". ref: https://www.geeksforgeeks.org/c/x-macros-in-c/
#define VECTORS_TABLE                    \
  X(Creature, creatures)                 \
  X(CreaturePreSpawn, creaturePreSpawns) \
  X(Projectile, projectiles)             \
  X(int, projectilesToRemove)            \
  X(BodyShape, bodyShapes)               \
  X(int, justDamagedCreatures)           \
  X(Number, numbers)                     \
  X(Pickupable, pickupables)             \
  X(Item, playerItems)

#define X(type_, name_) Vector<type_> name_ = {};
    VECTORS_TABLE;
#undef X

    // NOTE: Downwards goes data associated with different screens (ref: ScreenType).
    struct {
      ItemType toPick       = {};
      int      recyclePrice = {};
    } pickedUpItem;

    struct {
      Array<Upgrade, 4> toPick = {};
      Rerolls           rerolls;
    } upgrades;

    struct {
      Array<ShopItem, 4> toPick = {};
      Rerolls            rerolls;

      int selectedWeaponIndex = -1;
    } shop;
  } run;

  struct {
    bool active        = false;
    bool addedChildren = false;
    u16  childGap      = {};
    int  currentWidth  = 0;
    int  maxWidth      = {};

    Array<StringPlaceholderValue, BF_MAX_PLACEHOLDERS_IN_STRING> placeholders      = {};
    int                                                          placeholdersCount = {};
  } uiFlex;
} g = {};

#define PLAYER_COINS (g.run.playerStatsWithoutItems[StatType_COINS])

void AddCoins(int amount) {  ///
  PLAYER_COINS += amount;
  if (PLAYER_COINS < 0) {
    if (amount >= 0)
      PLAYER_COINS = int_max;
    else
      PLAYER_COINS = MAX(0, PLAYER_COINS);
  }
}

void SanitizeCoins() {  ///
  if (PLAYER_COINS < 0)
    PLAYER_COINS = int_max;
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

void FlexAddRowForChildIfNeeded(int childWidth) {  ///
  bool shouldAddRow = !g.uiFlex.addedChildren;

  if (g.uiFlex.addedChildren && (g.uiFlex.currentWidth + childWidth > g.uiFlex.maxWidth))
  {
    Clay__CloseElement();
    shouldAddRow = true;
  }

  if (shouldAddRow) {
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

void SetStringPlaceholder(int id, const char* value) {  ///
  g.uiFlex.placeholders[g.uiFlex.placeholdersCount++] = {
    .id    = id,
    .value = value,
  };
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
  *g.run.numbers.Add() = number;
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
    [&font, &width](u32 codepoint, u32 _codepointSize) BF_FORCE_INLINE_LAMBDA {
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

// TODO: ATLAS_D2.
void BF_CLAY_IMAGE(ClayImageData data, auto innerLambda) {  ///
  const auto texture = glib->atlas_textures()->Get(data.texId);

  if (g.uiFlex.active)
    FlexAddRowForChildIfNeeded(Ceil((f32)texture->size_x() * ASSETS_TO_LOGICAL_RATIO));

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

// NOTE: This overload DOESN'T SAVE string to trash arena.
void BF_CLAY_TEXT(Clay_String string, Color color = WHITE) {  ///
  if (g.uiFlex.active) {
    Clay_StringSlice s{
      .length    = string.length,
      .chars     = string.chars,
      .baseChars = string.chars,
    };
    auto dim = MeasureText(s, nullptr, nullptr);
    FlexAddRowForChildIfNeeded(dim.width);
  }

  CLAY_TEXT(string, CLAY_TEXT_CONFIG({.textColor = ToClayColor(color)}));
}

// NOTE: This overload SAVES string to trash arena.
void BF_CLAY_TEXT(const char* text, Color color = WHITE) {  ///
  int         len           = 0;
  const char* allocatedText = PushTextToArena(&g.meta.trashArena, text, &len);
  Clay_String string{
    .length = (i32)len,
    .chars  = allocatedText,
  };
  BF_CLAY_TEXT(string, color);
}

void BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(int locale_) {  ///
  const auto localization = glib->localizations()->Get(ge.meta.localization);
  const auto localization_broken_strings = localization->broken_strings();

  if (BF_DEBUG) {
    int requiredPlaceholders = 0;
    for (auto string : *localization_broken_strings->Get(locale_)->strings()) {
      if (string->placeholder_id())
        requiredPlaceholders++;
    }
    ASSERT(requiredPlaceholders == g.uiFlex.placeholdersCount);

    for (auto string : *localization_broken_strings->Get(locale_)->strings()) {
      if (string->placeholder_id()) {
        bool found = false;
        FOR_RANGE (int, i, g.uiFlex.placeholdersCount) {
          if (g.uiFlex.placeholders[i].id == string->placeholder_id()) {
            found = true;
            break;
          }
        }
        ASSERT(found);
      }
    }
  }

  for (auto string : *localization_broken_strings->Get(locale_)->strings()) {
    if (string->placeholder_id()) {
      int pl = -1;
      FOR_RANGE (int, i, g.uiFlex.placeholdersCount) {
        if (g.uiFlex.placeholders[i].id == string->placeholder_id()) {
          pl = i;
          break;
        }
      }

      BF_CLAY_TEXT(g.uiFlex.placeholders[pl].value, GREEN);
    }
    else {
      Clay_String text{
        .isStaticallyAllocated = true,
        .length                = (i32)string->string()->size(),
        .chars                 = string->string()->c_str(),
      };
      BF_CLAY_TEXT(text);
    }
  }

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

  auto index = g.run.creatures.count;
  auto slot  = g.run.creatures.Add();

  auto hurtboxRadius = PLAYER_HURTBOX_RADIUS;
  if (data.type != CreatureType_PLAYER)
    hurtboxRadius = MOB_HURTBOX_RADIUS;

  const auto fb     = glib->creatures()->Get(data.type);
  auto       health = (f32)fb->health()
                + (f32)((g.run.waveIndex - fb->appearing_wave_number() + 1))
                    * fb->health_increase_per_wave();
  if (data.type == CreatureType_PLAYER)
    health = (f32)g.run.playerStatsWithoutItems[StatType_HP];

  ASSERT(health > 0);

  const auto creatureId = g.run.nextCreatureId++;

  Creature creature{
    .id        = creatureId,
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
             .userData = ShapeUserData::Creature(creatureId),
             .isPlayer = (data.type == CreatureType_PLAYER),
      },
    }),
    .speed     = fb->speed() + Lerp(-1.0f, 1.0f, GRAND.FRand()) * fb->speed_variation(),
  };
  creature.idleStartedAt.SetNow();

  switch (creature.type) {
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
}

void MakeProjectile(MakeProjectileData data) {  ///
  ASSERT(data.type);
  ASSERT(data.dir != Vector2Zero());
  if (data.ownerCreatureType == CreatureType_PLAYER)
    ASSERT(data.weaponIndex >= 0);

  Projectile projectile{
    .type              = data.type,
    .ownerCreatureType = data.ownerCreatureType,
    .weaponIndex       = data.weaponIndex,
    .pos               = data.pos,
    .dir               = data.dir,
    .damage            = data.damage,
    .crit              = data.crit,
    .pierce            = data.pierce,
    .bounce            = data.bounce,
    .knockbackMeters   = data.knockbackMeters,
    .range             = data.range,
  };
  projectile.createdAt.SetNow();

  switch (data.type) {
  case ProjectileType_ARROW:
  case ProjectileType_BULLET:
  case ProjectileType_STONE:
  case ProjectileType_FIRE:
  case ProjectileType_LIGHTNING:
  case ProjectileType_MOB: {
    // Intentionally left blank.
  } break;

  default:
    INVALID_PATH;
  }

  *g.run.projectiles.Add() = projectile;
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

#define PLAYER_CREATURE (g.run.creatures[0])

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
  int weaponsCount = 0;
  for (const auto& weapon : g.run.playerWeapons) {
    if (weapon.type)
      weaponsCount++;
  }

  // Checkin that INVALID weapons are on the end of `playerWeapons`.
  for (int i = weaponsCount; i < g.run.playerWeapons.count; i++)
    ASSERT(!g.run.playerWeapons[i].type);

  // Recalculating offsets.
  if (weaponsCount > 0) {
    const auto startingAngle = PLAYER_WEAPONS_STARTING_ANGLES[weaponsCount - 1];
    const auto angleDelta    = 2.0f * (f32)PI / (f32)weaponsCount;
    FOR_RANGE (int, i, weaponsCount) {
      g.run.playerWeapons[i].offset
        = Vector2Rotate(Vector2(1, 0), i * angleDelta + startingAngle);
    }
  }
}

ItemType GenerateRandomItem() {  ///
  ItemType type{};
  while (!type)
    type = (ItemType)(GRAND.Rand() % (u32)ItemType_COUNT);
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
  FOR_RANGE (int, i, percentsCount) {
    total += percents[i];
  }

  f32 factors[3]{};
  FOR_RANGE (int, i, percentsCount) {
    factors[i] = percents[i] / total;
  }

  for (int i = 1; i < percentsCount; i++) {
    factors[i] += factors[i - 1];
  }

  const f32 t = GRAND.FRand();
  FOR_RANGE (int, i, percentsCount) {
    if (t < factors[i])
      return min + i;
  }

  INVALID_PATH;
  return 0;
}

void RecalculateThisWaveMobs() {  ///
  const auto fb_creatures = glib->creatures();

  g.run.thisWaveMobsCount = 0;

  f32 accumulatedFactor = 0;

  FOR_RANGE (int, i, fb_creatures->size()) {
    const auto fb     = fb_creatures->Get(i);
    const auto factor = fb->spawn_factor();
    if ((factor > 0) && (g.run.waveIndex + 1 >= fb->appearing_wave_number())) {
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

void RunInit() {
  ZoneScoped;

  // Creating box2d world.
  {  ///
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = b2Vec2{0, 0};
    g.run.world         = b2CreateWorld(&worldDef);
  }

  // Initializing `g.run.playerStatsWithoutItems`.
  FOR_RANGE (int, stat, (int)StatType_COUNT) {  ///
    const auto fb                       = glib->stats()->Get(stat);
    g.run.playerStatsWithoutItems[stat] = fb->player_value();
  }
  g.run.recalculatePlayerStats = true;

  // Making player.
  MakeCreature({
    .type = CreatureType_PLAYER,
    .pos  = (Vector2)WORLD_SIZE / 2.0f,
  });
  g.run.playerLastRegenAt.SetNow();

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
    auto& weapon        = g.run.playerWeapons[i];
    weapon.type         = weapons[i].type;
    weapon.tier         = GRAND.Rand() % (TOTAL_TIERS - 1);
    weapon.recyclePrice = GetWeaponRecyclePrice(weapon.type, weapon.tier);
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

  RecalculateThisWaveMobs();

  g.run.nextLevelXp = GetNextLevelXp(g.run.xpLevel);
}

void GameInit() {  ///
  ZoneScoped;

  SDL_HideCursor();

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

  g.meta.uiFont = LoadFont({
    .filepath        = "resources/correction_brush.ttf",
    .size            = 15,
    .FIXME_sizeScale = 45.0f / 30.0f,
    .codepoints      = g_codepoints,
    .codepointsCount = ARRAY_COUNT(g_codepoints),
    .outlineWidth    = 3,
    .outlineAdvance  = 1,
  });

  RunInit();
}

constexpr lframe GetFramesPerRegen(int regenLevel) {  ///
  if (regenLevel <= 0)
    return lframe::MakeUnscaled(i64_max);
  const f32 regenPerSecond = (f32)regenLevel / 11.25f + 1.0f / 9.0f;
  return lframe::MakeUnscaled((i64)((f32)FIXED_FPS / regenPerSecond));
}

f32 GetLifestealChance(WeaponType type) {  ///
  const auto statLifesteal   = (f32)g.run.playerStats[StatType_LIFE_STEAL] / 100.0f;
  const auto weaponLifesteal = glib->weapons()->Get(type)->life_steal_percent() / 100.0f;
  return statLifesteal + weaponLifesteal;
}

struct TryApplyDamageData {
  int          creatureIndex                = {};
  f32          damage                       = {};
  Vector2      directionOrZero              = {0, 0};
  f32          knockbackMeters              = 0;
  CreatureType damageApplicatorCreatureType = CreatureType_INVALID;
  bool         isCrit                       = false;
  int          indexOfWeaponThatDidDamage   = -1;
  AilmentType  ailment                      = AilmentType_INVALID;
  f32          ailmentChance                = 0;
  f32          ailmentValue                 = 0;
};

bool TryApplyDamage(TryApplyDamageData data) {  ///
  ASSERT(data.creatureIndex >= 0);
  if (data.damage <= 0)
    return false;
  if (FloatEquals(data.damage, 0))
    return false;

  if (data.directionOrZero != Vector2Zero())
    ASSERT(FloatEquals(Vector2Length(data.directionOrZero), 1));

  // Player can't take damage when finishing wave.
  if (!data.creatureIndex && g.run.scheduledWaveCompleted.IsSet())
    return false;

  auto& creature = g.run.creatures[data.creatureIndex];
  if (creature.health <= 0)
    return false;

  const auto fb = glib->creatures()->Get(creature.type);

  if (data.creatureIndex) {
    if (data.damageApplicatorCreatureType == CreatureType_PLAYER) {
      MakeNumber({
        .type  = (data.isCrit ? NumberType_DAMAGE_CRIT : NumberType_DAMAGE),
        .value = Round(data.damage),
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
  }

  // Player lifesteals.
  if (data.damageApplicatorCreatureType == CreatureType_PLAYER) {
    if (GRAND.FRand()
        < GetLifestealChance(g.run.playerWeapons[data.indexOfWeaponThatDidDamage].type))
    {
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
  creature.health -= data.damage;

  if (data.ailment && (GRAND.FRand() < data.ailmentChance)) {
    auto damage = data.ailmentValue;
    if (data.damageApplicatorCreatureType == CreatureType_PLAYER)
      damage += g.run.playerStats[StatType_DAMAGE_ELEMENTAL];

    if ((data.ailmentValue == 0) || (damage > 0)) {
      auto fb_ailment = glib->ailments()->Get(data.ailment);

      auto& a             = creature.ailments[data.ailment - 1];
      a.ownerCreatureType = data.damageApplicatorCreatureType;
      if ((data.ailment == AilmentType_ZAP) && !a.startedAt.IsSet())
        creature.speedModifier *= ZAP_SPEED_SCALE;
      a.startedAt = {};
      a.startedAt.SetNow();
      a.duration
        = lframe::MakeUnscaled((int)(FIXED_FPS * fb_ailment->duration_seconds()));
      a.value = MAX(a.value, damage);
    }
  }

  if (data.indexOfWeaponThatDidDamage >= 0)
    g.run.playerWeapons[data.indexOfWeaponThatDidDamage].didDamage += data.damage;

  creature.lastDamagedAt = {};
  creature.lastDamagedAt.SetNow();
  creature.lastDamagedWeaponIndex = data.indexOfWeaponThatDidDamage;

  data.knockbackMeters *= b2Body_GetMass(creature.body.id) * BODY_LINEAR_DAMPING;
  data.knockbackMeters *= 1.0f - fb->knockback_resistance();

  b2Body_ApplyLinearImpulseToCenter(
    creature.body.id, ToB2Vec2(data.directionOrZero * data.knockbackMeters), true
  );

  if (!g.run.justDamagedCreatures.Contains(data.creatureIndex))
    *g.run.justDamagedCreatures.Add() = data.creatureIndex;

  return true;
}

void RunReset() {  ///
  ZoneScoped;

  ge.settings.screenFade = 0;
  SDL_HideCursor();

  b2DestroyWorld(g.run.world);

  // Resetting `g.run` to a default value,
  // while preserving allocated memory of it's Vectors.
  struct {
#define X(type_, name_) Vector<type_> name_ = g.run.name_;
    VECTORS_TABLE;
#undef X
  } temp{};

#define X(type_, name_) temp.name_.Reset();
  VECTORS_TABLE;
#undef X

  g.run = {
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
  return GetRerollPrice(g.run.waveIndex, rerolledTimes);
}

void RefillUpgradesToPick() {  ///
  const auto fb_stats = glib->stats();

  FOR_RANGE (int, i, g.run.upgrades.toPick.count) {
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

void RefillShopToPick() {  ///
  const auto fb_items   = glib->items();
  const auto fb_weapons = glib->weapons();

  for (auto& v : g.run.shop.toPick) {
    v = {};

    const bool setToItem = (GRAND.FRand() <= SHOP_ITEM_RATIO);
    if (setToItem) {
      v.item = GenerateRandomItem();

      const auto fb = fb_items->Get(v.item);
      v.tier        = fb->tier();
      v.price       = fb->price();
    }
    else {
      while (!v.weapon) {
        v.weapon      = (WeaponType)(GRAND.Rand() % fb_weapons->size());
        const auto fb = fb_weapons->Get(v.weapon);
        // Legendary weapons can't be bought in shop.
        // TODO: Check if it's possible to buy legendary weapons in Brotato.
        v.tier  = (int)(GRAND.Rand() % (TOTAL_TIERS - 1));
        v.price = GetWeaponPrice(v.weapon, v.tier);
      }
    }
  }
}

void AddItem(ItemType v) {  ///
  bool increasedExistingItemCount = false;
  int  i                          = 0;
  for (auto& item : g.run.playerItems) {
    i++;
    if (item.type == v) {
      item.count++;
      increasedExistingItemCount = true;
      break;
    }
  }
  if (!increasedExistingItemCount) {
    Item item{.type = v, .count = 1};
    *g.run.playerItems.Add() = item;
  }

  g.run.recalculatePlayerStats = true;
}

void StableRemoveWeapon(int index) {  ///
  for (int i = index + 1; i < g.run.playerWeapons.count; i++)
    g.run.playerWeapons[i - 1] = g.run.playerWeapons[i];
  g.run.playerWeapons[g.run.playerWeapons.count - 1] = {};
  RecalculatePlayerWeaponOffsets();
}

f32 GetPlayerStatDamageMultiplier() {  ///
  return (f32)(100 + g.run.playerStats[StatType_DAMAGE]) / 100.0f;
}

f32 GetWeaponDamage(WeaponType type, int tier) {  ///
  ASSERT(tier < 4);
  const auto fb = glib->weapons()->Get(type);
  ASSERT(tier >= fb->min_tier_index());
  f32 damage = fb->base_damage()->Get(tier - fb->min_tier_index());
  for (auto scaling : *fb->damage_scalings()) {
    auto statValue = g.run.playerStats[scaling->stat_type()];
    auto percent   = scaling->percents_per_tier()->Get(tier - fb->min_tier_index());
    damage += (f32)statValue * (f32)percent / 100.0f;
  }
  damage *= GetPlayerStatDamageMultiplier();
  return damage;
}

f32 GetPlayerStatAttackSpeedMultiplier() {  ///
  return (f32)(100 + g.run.playerStats[StatType_ATTACK_SPEED]) / 100.0f;
}

lframe ApplyAttackSpeedToDuration(int duration) {  ///
  return lframe::MakeUnscaled(MAX(
    1,
    (int)((f32)(_BF_LOGICAL_FPS_SCALE * duration) / GetPlayerStatAttackSpeedMultiplier())
  ));
}

f32 GetWeaponRange(WeaponType type) {  ///
  const auto fb = glib->weapons()->Get(type);

  auto range = (f32)g.run.playerStats[StatType_RANGE];

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

bool OnWeaponCollided(b2ShapeId shapeId, int* const weaponIndex) {  ///
  auto& weapon = g.run.playerWeapons[*weaponIndex];

  const bool continueCollisions = true;
  const auto userData = ShapeUserData::FromPointer(b2Shape_GetUserData(shapeId));

  const auto  creatureIndex = GetCreatureIndexById(userData.GetCreatureId());
  const auto& creature      = g.run.creatures[creatureIndex];

  ASSERT(creature.type);

  if (creature.type == CreatureType_PLAYER)
    return continueCollisions;

  const auto fb = glib->weapons()->Get(weapon.type);

  if (ArrayContains(weapon.piercedCreatureIds.base, weapon.piercedCount, creature.id))
    return continueCollisions;

  if (weapon.piercedCount < weapon.piercedCreatureIds.count) {
    weapon.piercedCreatureIds[weapon.piercedCount++] = creature.id;

    f32  damage = GetWeaponDamage(weapon.type, weapon.tier);
    bool isCrit = IsCrit();
    if (isCrit)
      damage *= fb->critical_damage();

    damage = MAX(1, damage);

    TryApplyDamage({
      .creatureIndex   = creatureIndex,
      .damage          = damage,
      .directionOrZero = Vector2DirectionOrRandom(PLAYER_CREATURE.pos, creature.pos),
      .knockbackMeters = fb->knockback_meters(),
      .damageApplicatorCreatureType = CreatureType_PLAYER,
      .isCrit                       = isCrit,
      .indexOfWeaponThatDidDamage   = *weaponIndex,
      .ailment                      = {},
      .ailmentChance                = 0,
      .ailmentValue                 = 0,
    });
  }
  return continueCollisions;
}

Vector2 GetWeaponPos(const Weapon& weapon) {  ///
  const auto fb = glib->weapons()->Get(weapon.type);

  const auto basePos = PLAYER_CREATURE.pos + weapon.offset;
  if (fb->projectile_type() || !weapon.startedShootingAt.IsSet())
    return basePos;

  const auto e           = weapon.startedShootingAt.Elapsed();
  const auto shootingDur = ApplyAttackSpeedToDuration(fb->shooting_duration_frames());
  auto       p           = e.Progress(shootingDur);
  if (p > 0.5)
    p = 2.0f - p * 2;
  else
    p *= 2;
  const auto texId      = fb->texture_ids()->Get(0);
  const auto fb_texture = glib->atlas_textures()->Get(texId);
  const auto colliderSize
    = (f32)fb_texture->size_x() * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;

  const f32  movingDistance = MAX(1, GetWeaponRange(weapon.type));
  const auto movedDistance  = EaseInOutQuad(p) * movingDistance;

  return basePos + weapon.targetDir * movedDistance;
}

void AddXP(f32 xp) {  ///
  if (xp > 0)
    xp *= (f32)(MAX(1, g.run.playerStats[StatType_XP_GAIN] + 100)) / 100.0f;

  g.run.xp += xp;
  g.run.xp = MAX(0, g.run.xp);

  // Handling level up.
  while (g.run.xp >= g.run.nextLevelXp) {
    g.run.xp -= g.run.nextLevelXp;
    g.run.xpLevel++;
    g.run.nextLevelXp = GetNextLevelXp(g.run.xpLevel);

    MakeNumber({.type = NumberType_LEVEL_UP, .pos = PLAYER_CREATURE.pos});

    // Increasing random stat that has `upgrade_values`.
    while (1) {
      const auto stat = (StatType)(GRAND.Rand() % StatType_COUNT);
      const auto fb   = glib->stats()->Get(stat);
      const auto vals = fb->upgrade_values();
      if (!vals)
        continue;
      g.run.playerStatsWithoutItems[stat] += vals->Get(0);
      g.run.recalculatePlayerStats = true;
      break;
    }
  }
}

bool CanSpawnMoreCreatures() {  ///
  const auto framesUntilTheEndOfTheWave
    = GetWaveDuration(g.run.waveIndex) - g.run.waveStartedAt.Elapsed();
  return (framesUntilTheEndOfTheWave > DONT_SPAWN_RIGHT_BEFORE_WAVE_ENDS + SPAWN_FRAMES);
}

f32 ApplyPlayerArmorToIncomingDamage(f32 damage) {  ///
  auto armor = (f32)g.run.playerStats[StatType_ARMOR];

  if (armor > 0)
    damage *= 1.0f / (1.0f + armor / 15.0f);
  else if (armor < 0)
    damage *= (15.0f - 2 * armor) / (15 - armor);

  return damage;
}

f32 GetLuckFactor() {  ///
  return MAX(0, 1.0f + (f32)g.run.playerStats[StatType_LUCK] / 100.0f);
}

void HealPlayer(f32 amount = 1) {  ///
  if (PLAYER_CREATURE.health < PLAYER_CREATURE.maxHealth) {
    PLAYER_CREATURE.health
      = MoveTowards(PLAYER_CREATURE.health, PLAYER_CREATURE.maxHealth, amount);
  }
}

void ClayEffectCondition_KILL_N_ENEMIES_GET_STAT(const BFGame::Effect* fb_effect) {  ///
  SetStringPlaceholder(
    BF_PL__UI_LABEL_KILL_N_ENEMIES__ENEMIES,
    TextFormat("%d", fb_effect->condition_value())
  );
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(glib->ui_label_kill_n_enemies_locale());
}

void ClayEffectCondition_END_OF_THE_WAVE_GET_STAT(const BFGame::Effect* fb_effect) {  ///
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(glib->ui_label_at_the_end_of_the_wave_locale());
}

void ClayEffectCondition_START_OF_THE_WAVE_GET_STAT(const BFGame::Effect* fb_effect
) {  ///
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(glib->ui_label_at_the_start_of_the_wave_locale());
}

void ClayEffectCondition_KILL_N_ENEMIES_USING_THIS_WEAPON_GET_STAT(
  const BFGame::Effect* fb_effect
) {  ///
  SetStringPlaceholder(
    BF_PL__UI_LABEL_KILL_N_ENEMIES_USING_THIS_WEAPON__ENEMIES,
    TextFormat("%d", fb_effect->condition_value())
  );
  BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(
    glib->ui_label_kill_n_enemies_using_this_weapon_locale()
  );
}

void DoUI(bool draw) {
  ZoneScoped;

  // NOTE: Logic must be executed only when `draw` is false!
  // e.g. updating mouse position, processing `clicked()`,
  // logically reacting to `Clay_Hovered()`, changing game's state, etc.

  const auto fb_atlas_textures = glib->atlas_textures();
  const auto fb_items          = glib->items();
  const auto fb_stats          = glib->stats();
  const auto fb_weapons        = glib->weapons();
  const auto fb_pickupables    = glib->pickupables();

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
  const auto    localization_broken_strings = localization->broken_strings();

  TEMP_USAGE(&g.meta.trashArena);

  Clay_BeginLayout();

  Array<Beautify, MAX_BEAUTIFIERS> beautifiers{};
  int                              beautifiersCount = 0;

  constexpr u16 GAP_SMALL                = 8;
  constexpr u16 GAP_BIG                  = 20;
  constexpr u16 PADDING_NINE_SLICE       = 12;
  constexpr u16 PADDING_OUTER_VERTICAL   = 10;
  constexpr u16 PADDING_OUTER_HORIZONTAL = 12;

  int _wheel = GetMouseWheel();
  if (IsKeyDown(SDL_SCANCODE_LSHIFT))
    _wheel *= 10;
  if (draw)
    _wheel = 0;
  const int wheel = _wheel;
  // }

  LAMBDA (void, BF_CLAY_TEXT_LOCALIZED_DANGER, (int locale, Color color = WHITE)) {  ///
    auto        string = localization_strings->Get(locale);
    Clay_String text{
      .isStaticallyAllocated = true,
      .length                = (i32)string->size(),
      .chars                 = string->c_str(),
    };
    BF_CLAY_TEXT(text, color);
  };

  LAMBDA (bool, clicked, ()) {  ///
    return !draw && Clay_Hovered() && IsMouseReleased(L);
  };

  LAMBDA (bool, rightClicked, ()) {  ///
    return !draw && Clay_Hovered() && IsMouseReleased(R);
  };

  struct ComponentButtonData {
    bool enabled = false;
    bool growX   = false;
  };

  // LAMBDA (bool, componentButton, (ComponentButtonData data, auto innerLambda)) { ///
  const u32 buttonColors_[]{
    glib->ui_button_hovered_color(),
    glib->ui_button_default_color(),
    glib->ui_button_disabled_color(),
  };
  VIEW_FROM_ARRAY_DANGER(buttonColors);

  LAMBDA (bool, componentButton, (ComponentButtonData data, auto innerLambda)) {
    bool result = false;

    Clay_Sizing sizing{};
    if (data.growX)
      sizing.width = CLAY_SIZING_GROW(0);

    CLAY({
      .layout{
        .sizing{sizing},
        BF_CLAY_PADDING_ALL(8),
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      },
      .backgroundColor = ToClayColor(
        ColorFromRGB(buttonColors[(data.enabled ? (Clay_Hovered() ? 0 : 1) : 2)])
      ),
    }) {
      result = data.enabled && clicked();
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
    CLAY({
      .layout{
        BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE),
        .childGap        = GAP_SMALL,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      },
      BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice()),
    }) {
      // Stats label.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_stats_locale());
      }

      // Primary / secondary buttons.
      CLAY({.layout{.childGap = GAP_SMALL}}) {
        const bool clickedPrimary = componentButton(
          {.enabled = g.run.showingSecondaryStats},
          [&]() BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_stats_primary_locale());
          }
        );

        const bool clickedSecondary = componentButton(
          {.enabled = !g.run.showingSecondaryStats},
          [&]() BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_stats_secondary_locale());
          }
        );

        if (clickedPrimary)
          g.run.showingSecondaryStats = false;
        if (clickedSecondary)
          g.run.showingSecondaryStats = true;
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
              g.run.playerStatsWithoutItems[stat]++;
              g.run.recalculatePlayerStats = true;
            }
            if (rightClicked()) {
              g.run.playerStatsWithoutItems[stat] += 10;
              g.run.recalculatePlayerStats = true;
            }
            if (wheel && Clay_Hovered()) {
              g.run.playerStatsWithoutItems[stat] += wheel;
              g.run.recalculatePlayerStats = true;
            }
          }

          const auto fb = fb_stats->Get(stat);

          // Icon.
          CLAY({.floating{
            .attachPoints{
              .element = CLAY_ATTACH_POINT_LEFT_CENTER,
              .parent  = CLAY_ATTACH_POINT_LEFT_CENTER,
            },
            .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
            .attachTo           = CLAY_ATTACH_TO_PARENT,
          }}) {
            FLOATING_BEAUTIFY;
            BF_CLAY_IMAGE({.texId = iconTexId});
          }

          if (fb->is_percent())
            BF_CLAY_TEXT("   % ");
          else
            BF_CLAY_TEXT("   ");
          BF_CLAY_TEXT_LOCALIZED_DANGER(locale);
          BF_CLAY_SPACER_HORIZONTAL;
          BF_CLAY_TEXT(TextFormat("%d", value));
        }
      };

      // Current level.
      if (!g.run.showingSecondaryStats) {
        componentStatsEntry(
          glib->ui_shop_current_level_icon_texture_id(),
          glib->ui_current_level_locale(),
          g.run.xpLevel,
          StatType_INVALID
        );
      }

      // Stats.
      CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}})
      FOR_RANGE (int, i, (int)StatType_COUNT - 2) {
        const auto type = (StatType)(i + 2);
        const auto fb   = glib->stats()->Get(type);
        if (!fb->hidden() && (fb->is_secondary() == g.run.showingSecondaryStats)) {
          componentStatsEntry(
            fb->icon_texture_id(), fb->name_locale(), g.run.playerStats[type], type
          );
        }
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
            FLOATING_BEAUTIFY;
            BF_CLAY_TEXT(TextFormat("x%d", item.count));
          }
        }
      }
    });
  };

  LAMBDA (void, componentEffectsExploded, (auto fb_effects, int count, int maxWidth))
  {  ///
    ASSERT(count > 0);
    if (!fb_effects)
      return;
    for (const auto fb_effect : *fb_effects) {
      CLAY({.layout{
        BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        FlexBegin(maxWidth, 8);

        const auto fb_stat = fb_stats->Get(fb_effect->stat_type());
        if (fb_effect->value() != 0) {
          const auto format = ((fb_effect->value() >= 0) ? "+%d" : "%d");
          BF_CLAY_TEXT(
            TextFormat(format, fb_effect->value() * count),
            ((fb_effect->value() >= 0) ? GREEN : RED)
          );
        }
        else {
          const auto format  = ((fb_effect->value_multiplier() >= 0) ? "+%d%%" : "%d%%");
          const auto fb_stat = fb_stats->Get(fb_effect->stat_type());
          BF_CLAY_TEXT(
            TextFormat(
              format, Round((fb_effect->value_multiplier() - 1) * 100.0f * count)
            ),
            ((fb_effect->value_multiplier() >= 0) ? GREEN : RED)
          );
        }

        if (fb_stat->icon_texture_id())
          BF_CLAY_IMAGE({.texId = fb_stat->icon_texture_id()});
        BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(fb_stat->name_locale());

        const auto cond = fb_effect->effectcondition_type();
        if (cond)
          clayEffectConditionFunctions[cond - 1](fb_effect);

        FlexEnd();
      }
    }
  };

  LAMBDA (void, componentItemStatsExploded, (ItemType type, int count, int maxWidth))
  {  ///
    componentEffectsExploded(fb_items->Get(type)->effects(), count, maxWidth);
  };

  struct ComponentItemDetailsData {
    int detailsRight = {};
    int detailsBelow = {};
  };

  constexpr int ITEM_FRAME_WIDTH    = 219;
  constexpr int UPGRADE_FRAME_WIDTH = 190;

  LAMBDA (void, componentItemDetails, (const Item& item, ComponentItemDetailsData data))
  {  ///
    u16 padding[4]{8, 8, 8, 8};

    Clay_FloatingAttachPointType attachElement{};
    Clay_FloatingAttachPointType attachParent{};

    if (data.detailsRight) {
      padding[0] = 0;
      if (data.detailsBelow) {
        padding[3]    = 0;
        attachElement = CLAY_ATTACH_POINT_LEFT_TOP;
        attachParent  = CLAY_ATTACH_POINT_LEFT_BOTTOM;
      }
      else {
        padding[2]    = 0;
        attachElement = CLAY_ATTACH_POINT_LEFT_BOTTOM;
        attachParent  = CLAY_ATTACH_POINT_LEFT_TOP;
      }
    }
    else {
      padding[1] = 0;
      if (data.detailsBelow) {
        padding[3]    = 0;
        attachElement = CLAY_ATTACH_POINT_RIGHT_TOP;
        attachParent  = CLAY_ATTACH_POINT_RIGHT_BOTTOM;
      }
      else {
        padding[2]    = 0;
        attachElement = CLAY_ATTACH_POINT_RIGHT_BOTTOM;
        attachParent  = CLAY_ATTACH_POINT_RIGHT_TOP;
      }
    }

    CLAY({
      .layout{
        .sizing{
          CLAY_SIZING_FIXED(ITEM_FRAME_WIDTH + 2 * PADDING_NINE_SLICE), CLAY_SIZING_FIT(0)
        },
        .padding{padding[0], padding[1], padding[2], padding[3]},
      },
      .floating{
        .attachPoints{.element = attachElement, .parent = attachParent},
        .attachTo = CLAY_ATTACH_TO_PARENT,
      },
    }) {
      FLOATING_BEAUTIFY;
      CLAY({
        .layout{
          BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE),
          .childGap        = GAP_SMALL,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice()),
      }) {
        const auto fb_item = fb_items->Get(item.type);

        CLAY({.layout{.childGap = GAP_SMALL}}) {
          componentItem(item);
          BF_CLAY_TEXT_LOCALIZED_DANGER(fb_item->name_locale());
        }

        componentItemStatsExploded(item.type, item.count, ITEM_FRAME_WIDTH);
      }
    }
  };

  LAMBDA (void, componentWeapon, (int weaponIndex, bool weAreInShop)) {  ///
    auto& weapon = g.run.playerWeapons[weaponIndex];

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

        if (weAreInShop && clicked() && (g.run.shop.selectedWeaponIndex == -1))
          g.run.shop.selectedWeaponIndex = weaponIndex;
      }
    });
  };

  LAMBDA (void, componentWeaponStatEntry, (int labelLocale, auto&& innerLambda)) {  ///
    CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER}}) {
      Color color{0xef, 0xcb, 0x84, 255};
      BF_CLAY_TEXT_LOCALIZED_DANGER(labelLocale, color);
      BF_CLAY_TEXT(": ", color);
      innerLambda();
    }
  };

  LAMBDA (
    void,
    componentWeaponStatsExploded,
    (WeaponType type, int tier, f32 didDamage, int maxWidth)
  )
  {  ///
    const auto fb = fb_weapons->Get(type);

    // Damage.
    componentWeaponStatEntry(
      fb_stats->Get(StatType_DAMAGE)->name_locale(),
      [&]() BF_FORCE_INLINE_LAMBDA {
        BF_CLAY_TEXT(TextFormat("%d", (int)GetWeaponDamage(type, tier)), GREEN);

        // Scalings.
        const auto fb_scalings = fb->damage_scalings();
        if (fb_scalings && fb_scalings->size()) {
          BF_CLAY_TEXT(" (");
          FOR_RANGE (int, scalingIndex, fb_scalings->size()) {
            const auto fb_scaling = fb_scalings->Get(scalingIndex);
            const auto fb_stat    = fb_stats->Get(fb_scaling->stat_type());
            BF_CLAY_TEXT(TextFormat(
              "+%d%%", fb_scaling->percents_per_tier()->Get(tier - fb->min_tier_index())
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
    componentWeaponStatEntry(glib->ui_label_crit_locale(), [&]() BF_FORCE_INLINE_LAMBDA {
      BF_CLAY_TEXT(TextFormat(
        "x%.1f (%d%%)", fb->critical_damage(), g.run.playerStats[StatType_CRIT_CHANCE]
      ));
    });

    // Cooldown.
    componentWeaponStatEntry(
      glib->ui_label_cooldown_locale(),
      [&]() BF_FORCE_INLINE_LAMBDA {
        const auto cooldownFrames = ApplyAttackSpeedToDuration(
          fb->shooting_duration_frames() + fb->cooldown_frames()
        );
        const f32 cooldownSeconds = (f32)cooldownFrames.value / (f32)FIXED_FPS;
        BF_CLAY_TEXT(TextFormat("%.2fs", cooldownSeconds), GREEN);
      }
    );

    // Knockback.
    componentWeaponStatEntry(
      glib->ui_label_knockback_locale(),
      [&]() BF_FORCE_INLINE_LAMBDA {
        BF_CLAY_TEXT(StripLeadingZerosInFloat(TextFormat(
          "%.1f",
          fb->knockback_meters() * (f32)(100 + g.run.playerStats[StatType_KNOCKBACK])
            / 100.0f
        )));
      }
    );

    // Range.
    componentWeaponStatEntry(glib->ui_label_range_locale(), [&]() BF_FORCE_INLINE_LAMBDA {
      const f32 rangeMeters = GetWeaponRange(type);
      if (fb->projectile_type()) {
        BF_CLAY_TEXT(TextFormat("%.1f", rangeMeters));
      }
      else {
        const f32 weaponRangeMeters
          = (f32)fb_atlas_textures->Get(fb->texture_ids()->Get(0))->size_x()
            * ASSETS_TO_LOGICAL_RATIO / METER_LOGICAL_SIZE;
        BF_CLAY_TEXT(TextFormat("%.1f + %.1f", weaponRangeMeters, rangeMeters));
      }
    });

    // Pierce.
    if (fb->projectile_type()
        && (fb->projectile_pierce() + g.run.playerStats[StatType_PIERCING] > 0))
    {
      componentWeaponStatEntry(
        glib->ui_label_pierce_locale(),
        [&]() BF_FORCE_INLINE_LAMBDA {
          BF_CLAY_TEXT(TextFormat(
            "%d", fb->projectile_pierce() + g.run.playerStats[StatType_PIERCING]
          ));
        }
      );
    }

    // Bounce.
    if (fb->projectile_type()
        && (fb->projectile_bounce() + g.run.playerStats[StatType_BOUNCES] > 0))
    {
      componentWeaponStatEntry(
        glib->ui_label_bounce_locale(),
        [&]() BF_FORCE_INLINE_LAMBDA {
          BF_CLAY_TEXT(TextFormat(
            "%d", fb->projectile_bounce() + g.run.playerStats[StatType_BOUNCES]
          ));
        }
      );
    }

    // Life Steal.
    {
      auto chance = GetLifestealChance(type);
      if (chance > 0) {
        componentWeaponStatEntry(
          fb_stats->Get(StatType_LIFE_STEAL)->name_locale(),
          [&]() BF_FORCE_INLINE_LAMBDA {
            BF_CLAY_TEXT(
              TextFormat(
                "%s%%", StripLeadingZerosInFloat(TextFormat("%.1f", chance * 100.0f))
              ),
              GREEN
            );
          }
        );
      }
    }

    componentEffectsExploded(fb->effects(), 1, maxWidth);

    // Did damage.
    ASSERT(didDamage >= 0);
    if (didDamage > 0) {
      componentWeaponStatEntry(
        glib->ui_label_did_damage_locale(),
        [&]() BF_FORCE_INLINE_LAMBDA { BF_CLAY_TEXT(TextFormat("%d", (int)didDamage)); }
      );
    }
  };

  LAMBDA (
    void, componentWeaponDetails, (int weaponIndex, bool weAreInShop, bool detailsBelow)
  )
  {  ///
    auto& weapon = g.run.playerWeapons[weaponIndex];
    ASSERT(weapon.type);
    auto fb = fb_weapons->Get(weapon.type);

    // Floating weapon details modal.
    // Gets shown upon hovering. Gets sticked upon clicking on weapon.
    if ((Clay_Hovered() || (g.run.shop.selectedWeaponIndex == weaponIndex))
        && g.run.playerWeapons[weaponIndex].type)
    {
      if (weAreInShop && (g.run.shop.selectedWeaponIndex == weaponIndex)) {
        // Pressing ESC closes modal.
        if (IsKeyPressed(SDL_SCANCODE_ESCAPE))
          g.run.shop.selectedWeaponIndex = -1;
      }

      u16 padding[4]{0, 0, 8, 8};

      Clay_FloatingAttachPointType attachElement{};
      Clay_FloatingAttachPointType attachParent{};

      if (detailsBelow) {
        padding[3]    = 0;
        attachElement = CLAY_ATTACH_POINT_LEFT_TOP;
        attachParent  = CLAY_ATTACH_POINT_LEFT_BOTTOM;
      }
      else {
        padding[2]    = 0;
        attachElement = CLAY_ATTACH_POINT_LEFT_BOTTOM;
        attachParent  = CLAY_ATTACH_POINT_LEFT_TOP;
      }

      CLAY({
        .layout{
          .sizing{
            CLAY_SIZING_FIXED(ITEM_FRAME_WIDTH + 2 * PADDING_NINE_SLICE),
            CLAY_SIZING_FIT(0)
          },
          .padding{padding[0], padding[1], padding[2], padding[3]},
        },
        .floating{
          .attachPoints{.element = attachElement, .parent = attachParent},
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;
        if (weAreInShop && (g.run.shop.selectedWeaponIndex == weaponIndex))
          CLAY({BF_CLAY_CUSTOM_OVERLAY(Fade(MODAL_OVERLAY_COLOR, MODAL_OVERLAY_COLOR_FADE)
          )}) {}

        CLAY({
          .layout{
            .sizing{
              .width = CLAY_SIZING_FIXED(ITEM_FRAME_WIDTH + 2 * PADDING_NINE_SLICE)
            },
            BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE),
            .childGap        = GAP_SMALL,
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
          },
          BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice()),
        }) {
          CLAY({.layout{.childGap = GAP_SMALL}}) {
            componentSlot(false, weapon.tier, [&]() BF_FORCE_INLINE_LAMBDA {
              CLAY({.layout{
                BF_CLAY_SIZING_GROW_XY,
                BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
              }}) {
                BF_CLAY_IMAGE({
                  .texId = fb->texture_ids()->Get(0),
                  .color = ColorFromRGB(fb->color()),
                });
              }
            });

            BF_CLAY_TEXT_LOCALIZED_DANGER(fb->name_locale());
          }

          componentWeaponStatsExploded(
            weapon.type, weapon.tier, weapon.didDamage, ITEM_FRAME_WIDTH
          );

          if (weAreInShop) {
            int canCombineWithIndex = -1;
            for (int i = g.run.playerWeapons.count - 1; i >= 0; i--) {
              if (i == weaponIndex)
                continue;
              auto& otherWeapon = g.run.playerWeapons[i];
              if ((weapon.type == otherWeapon.type)     //
                  && (weapon.tier == otherWeapon.tier)  //
                  && (weapon.tier < TOTAL_TIERS - 1))
              {
                canCombineWithIndex = i;
                break;
              }
            }

            // Combine button.
            const bool combined = componentButton(
              {.enabled = canCombineWithIndex >= 0},
              [&]() BF_FORCE_INLINE_LAMBDA {
                BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_combine_locale());
              }
            );

            // Recycle button.
            const bool recycled
              = componentButton({.enabled = true}, [&]() BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_recycle_locale());
                  BF_CLAY_TEXT(TextFormat(" (+%d)", weapon.recyclePrice));
                });

            // Cancel button.
            const bool cancelled
              = componentButton({.enabled = true}, [&]() BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_cancel_locale());
                });

            if (combined) {
              weapon.tier += 1;
              weapon.recyclePrice  = GetWeaponRecyclePrice(weapon.type, weapon.tier);
              weapon.didDamage     = 0;
              weapon.killedEnemies = MAX(
                weapon.killedEnemies,
                g.run.playerWeapons[canCombineWithIndex].killedEnemies
              );
              StableRemoveWeapon(canCombineWithIndex);
            }
            if (recycled) {
              AddCoins(weapon.recyclePrice);
              StableRemoveWeapon(weaponIndex);
            }
            if (cancelled || recycled || combined)
              g.run.shop.selectedWeaponIndex = -1;
          }
        }
      }
    }
  };

  // Gameplay.
  if ((g.run.screen == ScreenType_GAMEPLAY)               //
      || (g.run.screen == ScreenType_WAVE_END_ANIMATION)  //
      || (g.run.screen == ScreenType_PICKED_UP_ITEM)      //
      || (g.run.screen == ScreenType_UPGRADES))
  {
    CLAY({
      .layout{
        BF_CLAY_SIZING_GROW_XY,
        BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
          PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
        )
      },
      .floating{
        .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
        .attachTo           = CLAY_ATTACH_TO_PARENT,
      },
    })
    CLAY({.layout{BF_CLAY_SIZING_GROW_XY}}) {
      FLOATING_BEAUTIFY;

      // Health bar + coins.
      // {  ///
      const auto  texs   = glib->ui_health_texture_ids();
      const auto& player = PLAYER_CREATURE;

      CLAY({
        .layout{
          .childGap        = GAP_SMALL,
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

            const auto health = MAX(0, PLAYER_CREATURE.health);

            BF_CLAY_IMAGE({
              .texId = texs->Get(1),
              .sourceMargins{
                .right = Clamp01(1.0f - (f32)health / (f32)PLAYER_CREATURE.maxHealth)
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
              BF_CLAY_TEXT(
                TextFormat("%d / %d", Round(health), Round(PLAYER_CREATURE.maxHealth))
              );
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
          .childGap = GAP_SMALL,
          BF_CLAY_CHILD_ALIGNMENT_LEFT_CENTER,
        }}) {
          BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
          BF_CLAY_TEXT(TextFormat("%d", PLAYER_COINS));
        }
      }
      // }

      // Wave.
      // { ///
      CLAY({
        .floating{
          .attachPoints{
            .element = CLAY_ATTACH_POINT_CENTER_TOP,
            .parent  = CLAY_ATTACH_POINT_CENTER_TOP,
          },
          .attachTo = CLAY_ATTACH_TO_PARENT,
        },
      }) {
        FLOATING_BEAUTIFY;

        BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_wave_locale());
        BF_CLAY_TEXT(TextFormat(" %d", g.run.waveIndex + 1));

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
        .layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM},
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
      CLAY({.layout{.childGap = GAP_BIG, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
        // Column 1. Picked up item.
        CLAY({.layout{
          .childGap        = GAP_SMALL,
          .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }}) {
          // "Item Found" label.
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_item_found_locale());

          // Item.
          CLAY({
            .layout{
              .sizing{
                CLAY_SIZING_FIXED(ITEM_FRAME_WIDTH + 2 * PADDING_NINE_SLICE),
                CLAY_SIZING_FIXED(320)
              },
              BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE),
              .childGap        = GAP_SMALL,
              .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
            BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice()),
          }) {
            const auto type = g.run.pickedUpItem.toPick;

            CLAY({.layout{.childGap = GAP_SMALL}}) {
              const auto fb = fb_items->Get(type);
              const Item item{.type = type, .count = 1};
              componentItem(item);
              BF_CLAY_TEXT_LOCALIZED_DANGER(fb->name_locale());
            }

            componentItemStatsExploded(type, 1, ITEM_FRAME_WIDTH);
          }

          // Take and Recycle buttons.
          CLAY({.layout{.childGap = GAP_SMALL}}) {
            const bool took
              = componentButton({.enabled = true}, [&]() BF_FORCE_INLINE_LAMBDA {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_take_locale());
                });

            const bool recycled
              = componentButton({.enabled = true}, [&]() BF_FORCE_INLINE_LAMBDA {
                  CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
                    BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_recycle_locale());
                    BF_CLAY_TEXT(TextFormat(" (+%d)", g.run.pickedUpItem.recyclePrice));
                    // BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
                    // BF_CLAY_TEXT(")");
                  }
                });

            if (took)
              AddItem(g.run.pickedUpItem.toPick);
            else if (recycled)
              AddCoins(g.run.pickedUpItem.recyclePrice);

            if (took || recycled) {
              g.run.crates--;
              if (g.run.crates)
                g.run.scheduledPickedUpItems = true;
              else
                g.run.scheduledUpgrades = true;
            }
          }
        }

        // Column 2. Stats.
        componentStats();
      }
    }
  }
  // Upgrades.
  else if (g.run.screen == ScreenType_UPGRADES) {  ///
    // Vertical columns with upgrades and stats;
    CLAY({.layout{
      BF_CLAY_SIZING_GROW_XY,
      .childGap = GAP_BIG,
      BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
    }}) {
      // Upgrades.
      CLAY({.layout{
        .childGap = GAP_BIG,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
      }}) {
        // Level up label.
        BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_level_up_locale());

        // Upgrades.
        CLAY({.layout{.childGap = GAP_SMALL}}) {
          const auto fb_stats = glib->stats();

          FOR_RANGE (int, i, g.run.upgrades.toPick.count) {
            const auto upgrade = g.run.upgrades.toPick[i];
            const auto fb      = fb_stats->Get(upgrade.stat);
            CLAY({
              .layout{
                .sizing{
                  CLAY_SIZING_FIXED(UPGRADE_FRAME_WIDTH + 2 * PADDING_NINE_SLICE),
                  CLAY_SIZING_FIT(215),
                },
                BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE),
                .childGap = GAP_SMALL,
                // BF_CLAY_CHILD_ALIGNMENT_CENTER_TOP,
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
              },
              BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice()),
            }) {
              CLAY({.layout{.childGap = GAP_SMALL}}) {
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
                BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(fb->upgrade_name_locale());
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
                  BF_CLAY_TEXT(TextFormat("+%d", amount), GREEN);

                  BF_CLAY_IMAGE({.texId = fb->icon_texture_id()});

                  if (fb->is_percent())
                    BF_CLAY_TEXT("%");
                }

                const auto d = Clay_GetElementData(id);
                if (d.found) {
                  CLAY({.layout{.childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
                    FlexBegin(UPGRADE_FRAME_WIDTH - d.boundingBox.width, 8);
                    BF_CLAY_TEXT_BROKEN_LOCALIZED_DANGER(fb->name_locale());
                    FlexEnd();
                  }
                }
              }

              BF_CLAY_SPACER_VERTICAL;

              // Choose button.
              CLAY({.layout{BF_CLAY_SIZING_GROW_X, BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}
              ) {
                const auto clicked
                  = componentButton({.enabled = true}, [&]() BF_FORCE_INLINE_LAMBDA {
                      BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_choose_locale());
                    });
                if (clicked) {
                  if (g.run.previousLevel < g.run.xpLevel) {
                    g.run.previousLevel++;
                    g.run.scheduledUpgrades = true;
                  }
                  else
                    g.run.scheduledShop = true;

                  g.run.playerStatsWithoutItems[upgrade.stat] += amount;
                  g.run.recalculatePlayerStats = true;

                  switch (upgrade.stat) {
                  case StatType_HP: {
                    PLAYER_CREATURE.health    = g.run.playerStats[StatType_HP];
                    PLAYER_CREATURE.maxHealth = g.run.playerStats[StatType_HP];
                  } break;

                  default:
                    break;
                  }
                }
              }
            }
          }
        }

        // Reroll button.
        const auto calculatedRerollPrice
          = ApplyStatRerollPrice(g.run.upgrades.rerolls.GetPrice());
        const bool canReroll = (calculatedRerollPrice <= PLAYER_COINS);
        const bool rerolled
          = componentButton({.enabled = canReroll}, [&]() BF_FORCE_INLINE_LAMBDA {
              CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
                BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_reroll_locale());
                BF_CLAY_TEXT(TextFormat(" - %d ", calculatedRerollPrice));
                BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
              }
            });
        if (rerolled) {
          g.run.upgrades.rerolls.Roll();
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
      BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
        PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
      ),
      .childGap = GAP_BIG,
    }}) {
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
            BF_CLAY_TEXT(TextFormat("%d ", PLAYER_COINS));
            BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});

            if (ge.meta.debugEnabled && Clay_Hovered() && wheel)
              AddCoins(wheel);
          }

          BF_CLAY_SPACER_HORIZONTAL;

          // Reroll button.
          const auto calculatedRerollPrice
            = ApplyStatRerollPrice(g.run.shop.rerolls.GetPrice());
          const bool canReroll = (calculatedRerollPrice <= PLAYER_COINS);
          const bool rerolled
            = componentButton({.enabled = canReroll}, [&]() BF_FORCE_INLINE_LAMBDA {
                CLAY({.layout{BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER}}) {
                  BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_reroll_locale());

                  ASSERT(calculatedRerollPrice >= 0);

                  if (calculatedRerollPrice > 0) {
                    BF_CLAY_TEXT(TextFormat(" - %d ", calculatedRerollPrice));
                    BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
                  }
                }
              });
          if (rerolled) {
            g.run.shop.rerolls.Roll();
            RefillShopToPick();
          }
        }

        BF_CLAY_SPACER_VERTICAL;

        // 2. Items to buy.
        CLAY({.layout{
          BF_CLAY_SIZING_GROW_X,
          .childGap = GAP_SMALL,
          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
        }}) {
          for (auto& v : g.run.shop.toPick) {
            const auto calculatedPrice = ApplyStatItemsPrice(v.price);
            bool canBuy = ((v.item || v.weapon) && (calculatedPrice <= PLAYER_COINS));
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

            CLAY({.layout{.sizing{
              CLAY_SIZING_FIXED(ITEM_FRAME_WIDTH + 2 * PADDING_NINE_SLICE),
              CLAY_SIZING_FIXED(320),
            }}})
            if (v.item || v.weapon) {
              CLAY({
                .layout{
                  BF_CLAY_SIZING_GROW_XY,
                  BF_CLAY_PADDING_ALL(PADDING_NINE_SLICE),
                  .childGap        = GAP_SMALL,
                  .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                BF_CLAY_CUSTOM_NINE_SLICE(glib->ui_frame_nine_slice()),
              }) {
                const auto fb_item   = (v.item ? fb_items->Get(v.item) : nullptr);
                const auto fb_weapon = (v.weapon ? fb_weapons->Get(v.weapon) : nullptr);

                // TODO: Highlight price, not item's frame.

                // Item's image + name.
                CLAY({.layout{.childGap = GAP_SMALL}}) {
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

                if (v.item)
                  componentItemStatsExploded(v.item, 1, ITEM_FRAME_WIDTH);
                else if (v.weapon)
                  componentWeaponStatsExploded(v.weapon, v.tier, 0, ITEM_FRAME_WIDTH);

                BF_CLAY_SPACER_VERTICAL;

                // Item's price.
                CLAY({.layout{
                  BF_CLAY_SIZING_GROW_X,
                  BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                }}) {
                  // Buying item / weapon.
                  const auto bought
                    = componentButton({.enabled = canBuy}, [&]() BF_FORCE_INLINE_LAMBDA {
                        CLAY({.layout{
                          BF_CLAY_SIZING_GROW_X,
                          BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
                        }}) {
                          BF_CLAY_TEXT(
                            TextFormat("%d ", calculatedPrice),
                            (calculatedPrice <= PLAYER_COINS ? WHITE : RED)
                          );
                          BF_CLAY_IMAGE({.texId = glib->ui_coin_texture_id()});
                        }
                      });

                  if (bought) {
                    AddCoins(-calculatedPrice);
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
                        RecalculatePlayerWeaponOffsets();
                      }
                      weapon.recyclePrice
                        = GetWeaponRecyclePrice(weapon.type, weapon.tier);
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
        }

        BF_CLAY_SPACER_VERTICAL;

        // 3. Player's items and weapons.
        CLAY({.layout{BF_CLAY_SIZING_GROW_X}}) {
          CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
            // Items label.
            BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_items_locale());

            // Items.
            CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}}
            ) {
              constexpr int ITEMS_X = 10;
              const int     ITEMS_Y = CeilDivision(g.run.playerItems.count, ITEMS_X);

              FOR_RANGE (int, y, ITEMS_Y) {
                CLAY({.layout{.childGap = GAP_SMALL}})
                FOR_RANGE (int, x, ITEMS_X) {
                  const int t = y * ITEMS_X + x;
                  if (t >= g.run.playerItems.count)
                    break;
                  CLAY({}) {
                    auto& item = g.run.playerItems[t];
                    componentItem(item);
                    if (Clay_Hovered()) {
                      componentItemDetails(item, {.detailsRight = 1, .detailsBelow = 0});
                      if (ge.meta.debugEnabled) {
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
          }

          BF_CLAY_SPACER_HORIZONTAL;

          CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
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

            CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}})
            FOR_RANGE (int, y, 2) {
              CLAY({.layout{.childGap = GAP_SMALL}})
              FOR_RANGE (int, x, 3) {
                const int weaponIndex = y * WEAPONS_X + x;
                if (weaponIndex >= g.run.playerWeapons.count)
                  break;

                const auto& weapon = g.run.playerWeapons[weaponIndex];

                CLAY({}) {
                  // Weapon.
                  componentWeapon(weaponIndex, true && weapon.type);

                  // Hovering modal.
                  if (weapon.type)
                    componentWeaponDetails(weaponIndex, true, false);
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
            {.enabled = true, .growX = true},
            [&]() BF_FORCE_INLINE_LAMBDA {
              BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_next_wave_locale());
              BF_CLAY_TEXT(" (");
              BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_wave_locale());
              BF_CLAY_TEXT(TextFormat(" %d)", g.run.waveIndex + 2));
            }
          );
        }
      }
    }
  }
  // End.
  else if (g.run.screen == ScreenType_END) {  ///
    CLAY({.layout{
      BF_CLAY_SIZING_GROW_XY,
      BF_CLAY_PADDING_HORIZONTAL_VERTICAL(
        PADDING_OUTER_HORIZONTAL, PADDING_OUTER_VERTICAL
      ),
      .childGap = GAP_SMALL,
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
          // Weapons label.
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_weapons_locale());

          // Weapons.
          CLAY({.layout{.childGap = GAP_SMALL}}) {
            int i = -1;
            for (auto& weapon : g.run.playerWeapons) {
              i++;
              if (weapon.type) {
                CLAY({}) {
                  // Weapon.
                  componentWeapon(i, false);
                  // Hovering modal.
                  componentWeaponDetails(i, false, true);
                }
              }
            }
          }

          // Items label.
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_label_items_locale());

          // Items.
          const auto& items = g.run.playerItems;

          constexpr int ITEMS_X = 10;
          const int     ITEMS_Y = CeilDivision(items.count, ITEMS_X);

          CLAY({.layout{.childGap = GAP_SMALL, .layoutDirection = CLAY_TOP_TO_BOTTOM}})
          FOR_RANGE (int, y, ITEMS_Y) {
            CLAY({.layout{.childGap = GAP_SMALL}})
            FOR_RANGE (int, x, ITEMS_X) {
              const auto t = y * ITEMS_X + x;
              if (t < items.count) {
                CLAY({}) {
                  componentItem(g.run.playerItems[t]);
                  if (Clay_Hovered()) {
                    componentItemDetails(
                      g.run.playerItems[t], {.detailsRight = 1, .detailsBelow = 1}
                    );
                  }
                }
              }
            }
          }
        }
      }

      // Footer. Restart.
      CLAY({.layout{
        BF_CLAY_SIZING_GROW_X,
        BF_CLAY_CHILD_ALIGNMENT_CENTER_CENTER,
      }}) {
        g.run.reload = componentButton({.enabled = true}, [&]() BF_FORCE_INLINE_LAMBDA {
          BF_CLAY_TEXT_LOCALIZED_DANGER(glib->ui_button_restart_locale());
        });
      }
    }
  }
  else if (g.run.screen == ScreenType_WAVE_END_ANIMATION) {
    // NOTE: Intentionally left blank.
  }
  else if (g.run.screen == ScreenType_GAMEPLAY) {
    // NOTE: Intentionally left blank.
  }
  else
    INVALID_PATH;

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
            DrawGroup_CommandText({
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

void IterateOverWeaponEffects(
  EffectConditionType                                              condition,
  auto /* void (int weaponIndex, Weapon& weapon, auto fb_effect)*/ innerLambda
) {  ///
  const auto fb_weapons  = glib->weapons();
  int        weaponIndex = -1;
  for (auto& weapon : g.run.playerWeapons) {
    weaponIndex++;
    if (!weapon.type)
      continue;
    const auto fb         = fb_weapons->Get(weapon.type);
    const auto fb_effects = fb->effects();
    if (fb_effects) {
      for (const auto fb_effect : *fb_effects) {
        if (fb_effect->effectcondition_type() == condition)
          innerLambda(weaponIndex, weapon, fb_effect);
      }
    }
  }
}

void IterateOverItemEffects(
  EffectConditionType                         condition,
  auto /* void (Item& item, auto fb_effect)*/ innerLambda
) {  ///
  const auto fb_items = glib->items();
  for (auto& item : g.run.playerItems) {
    const auto fb         = fb_items->Get(item.type);
    const auto fb_effects = fb->effects();
    if (fb_effects) {
      for (const auto fb_effect : *fb_effects) {
        if (fb_effect->effectcondition_type() == condition)
          innerLambda(item, fb_effect);
      }
    }
  }
}

void ApplyEffect(const BFGame::Effect* fb_effect, int itemCount) {  ///
  ASSERT(itemCount > 0);
  g.run.playerStatsWithoutItems[fb_effect->stat_type()] += fb_effect->value() * itemCount;
  if ((StatType)fb_effect->stat_type() == StatType_COINS)
    SanitizeCoins();
  if (fb_effect->value_multiplier() != 1)
    g.run.playerStatsWithoutItems[fb_effect->stat_type()]
      *= 1 + (fb_effect->value_multiplier() - 1) * itemCount;
  if ((StatType)fb_effect->stat_type() == StatType_COINS)
    SanitizeCoins();

  g.run.recalculatePlayerStats = true;
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

bool AreEnemies(CreatureType t1, CreatureType t2) {  ///
  ASSERT(t1);
  ASSERT(t2);
  if ((t1 == CreatureType_PLAYER) && (t2 == CreatureType_PLAYER))
    return false;
  if ((t1 == CreatureType_PLAYER) && (t2 != CreatureType_PLAYER))
    return true;
  if ((t2 == CreatureType_PLAYER) && (t1 != CreatureType_PLAYER))
    return true;
  return false;
}

void GameFixedUpdate() {
  ZoneScoped;

  const auto fb_creatures   = glib->creatures();
  const auto fb_items       = glib->items();
  const auto fb_weapons     = glib->weapons();
  const auto fb_projectiles = glib->projectiles();

  // Reloading game.
  if (g.run.reload) {  ///
    ZoneScopedN("Reloading game.");

    RunReset();
    RunInit();
  }

  // Cheats.
  if (ge.meta.debugEnabled) {
    // F5 - add 10 coins.
    if (IsKeyPressed(SDL_SCANCODE_F5)) {  ///
      if (IsKeyDown(SDL_SCANCODE_LSHIFT))
        AddCoins(int_max);
      else
        AddCoins(10);
    }

    // F6 - add random item.
    if (IsKeyPressed(SDL_SCANCODE_F6)) {  ///
      Item item{.type = (ItemType)((GRAND.Rand() % (int)(ItemType_COUNT - 1)) + 1)};
      *g.run.playerItems.Add() = item;
    }

    if (g.run.screen == ScreenType_GAMEPLAY) {
      // F7 - show end screen.
      if (IsKeyPressed(SDL_SCANCODE_F7)) {  ///
        g.run.scheduledUI  = true;
        g.run.scheduledEnd = true;
      }

      // F8 - add level.
      if (IsKeyPressed(SDL_SCANCODE_F8)) {  ///
        AddXP(g.run.nextLevelXp);
      }

      // F9 - add crate.
      if (IsKeyPressed(SDL_SCANCODE_F9)) {  ///
        g.run.crates++;
      }
    }

    // N - increase wave.
    if (IsKeyPressed(SDL_SCANCODE_N)) {  ///
      g.run.waveIndex++;
      RecalculateThisWaveMobs();
    }
  }

  // Recalculating player stats.
  if (g.run.recalculatePlayerStats) {  ///
    ZoneScopedN("Recalculating player stats.");

    g.run.recalculatePlayerStats = false;

    FOR_RANGE (int, i, StatType_COUNT) {
      g.run.playerStats[i] = g.run.playerStatsWithoutItems[i];
    }

    for (auto& item : g.run.playerItems) {
      const auto fb         = glib->items()->Get(item.type);
      const auto fb_effects = fb->effects();
      if (fb_effects) {
        for (const auto fb_effect : *fb_effects)
          g.run.playerStats[fb_effect->stat_type()] += fb_effect->value() * item.count;
      }
    }
  }

  // Advancing to UI after wave completion animation finishes.
  if (g.run.scheduledWaveCompleted.IsSet()) {  ///

    // Applying item effects: END_OF_THE_WAVE_GET_STAT.
    if ((g.run.screen != ScreenType_WAVE_END_ANIMATION) && g.run.waveWon) {
      IterateOverWeaponEffects(
        EffectConditionType_END_OF_THE_WAVE_GET_STAT,
        [&](int _, Weapon& weapon, auto fb_effect)
          BF_FORCE_INLINE_LAMBDA { ApplyEffect(fb_effect, 1); }
      );
      IterateOverItemEffects(
        EffectConditionType_END_OF_THE_WAVE_GET_STAT,
        [&](const Item& item, auto fb_effect)
          BF_FORCE_INLINE_LAMBDA { ApplyEffect(fb_effect, item.count); }
      );
    }

    g.run.screen = ScreenType_WAVE_END_ANIMATION;

    g.run.projectiles.Reset();
    for (int i = 1; i < g.run.creatures.count; i++) {
      auto& creature = g.run.creatures[i];
      if (!creature.diedAt.IsSet())
        TryApplyDamage({.creatureIndex = i, .damage = f32_inf});
    }
    g.run.creaturePreSpawns.Reset();

    if (g.run.scheduledWaveCompleted.Elapsed() >= WAVE_COMPLETED_FRAMES) {
      {
        // Applying StatType_HARVESTING.
        auto& harvesting = g.run.playerStats[StatType_HARVESTING];
        AddCoins(harvesting);
        AddXP(harvesting);
        // Harvesting stat (if positive) grows by 5% upon finishing each wave.
        if (harvesting > 0)
          harvesting = Ceil((f32)harvesting * 1.05f);
      }

      if ((g.run.waveIndex >= TOTAL_WAVES - 1) || !g.run.waveWon) {
        g.run.scheduledEnd = true;
        g.run.won          = g.run.waveWon;
      }

      g.run.scheduledWaveCompleted = {};
      g.run.waveWon                = false;

      g.run.scheduledUI = true;
    }
  }

  // Applying UI settings (showing cursor + darkening gameplay).
  if (g.run.scheduledUI) {  ///
    g.run.scheduledUI = false;

    ge.settings.screenFade = glib->ui_modal_fade();
    SDL_ShowCursor();

    if (g.run.crates)
      g.run.scheduledPickedUpItems = true;
    else
      g.run.scheduledUpgrades = true;

    g.run.upgrades.rerolls = {};
  }

  // Advancing to ScreenType_PICKED_UP_ITEM.
  if (g.run.scheduledPickedUpItems) {  ///
    g.run.scheduledPickedUpItems = false;

    g.run.screen              = ScreenType_PICKED_UP_ITEM;
    g.run.pickedUpItem.toPick = GenerateRandomItem();
    g.run.pickedUpItem.recyclePrice
      = fb_items->Get(g.run.pickedUpItem.toPick)->price() / RECYCLE_PRICE_FACTOR;
  }

  // Advancing to ScreenType_UPGRADES.
  if (g.run.scheduledUpgrades) {  ///
    g.run.scheduledUpgrades = false;

    g.run.screen = ScreenType_UPGRADES;

    RefillUpgradesToPick();

    if (g.run.xpLevel == g.run.previousLevel)
      g.run.scheduledShop = true;
  }

  // Advancing to ScrenType_SHOP.
  if (g.run.scheduledShop) {  ///
    g.run.scheduledShop = false;
    g.run.screen        = ScreenType_SHOP;

    g.run.shop.rerolls = {};
    RefillShopToPick();
  }

  // Advancing to ScreenType_END.
  if (g.run.scheduledEnd) {  ///
    g.run.scheduledEnd = false;
    g.run.screen       = ScreenType_END;
  }

  // Advancing to the next wave (ScreenType_GAMEPLAY).
  if (g.run.scheduledNextWave) {  ///
    g.run.scheduledNextWave = false;

    const auto health         = (f32)g.run.playerStats[StatType_HP];
    PLAYER_CREATURE.health    = health;
    PLAYER_CREATURE.maxHealth = health;

    g.run.screen           = ScreenType_GAMEPLAY;
    ge.settings.screenFade = 0;
    SDL_HideCursor();

    g.run.waveIndex++;
    RecalculateThisWaveMobs();
    g.run.cratesDroppedThisWave = 0;

    g.run.waveStartedAt = {};
    g.run.waveStartedAt.SetNow();

    for (auto& weapon : g.run.playerWeapons)
      weapon.didDamage = 0;

    IterateOverWeaponEffects(
      EffectConditionType_START_OF_THE_WAVE_GET_STAT,
      [&](int _, Weapon& weapon, auto fb_effect)
        BF_FORCE_INLINE_LAMBDA { ApplyEffect(fb_effect, 1); }
    );
    IterateOverItemEffects(
      EffectConditionType_START_OF_THE_WAVE_GET_STAT,
      [&](const Item& item, auto fb_effect)
        BF_FORCE_INLINE_LAMBDA { ApplyEffect(fb_effect, item.count); }
    );
  }

  PLAYER_CREATURE.controller.move = {};

  // Updating gameplay.
  if (g.run.screen == ScreenType_GAMEPLAY) {
    ZoneScopedN("Updating gameplay.");

    // Finishing wave opens upgrades screen.
    if (g.run.waveStartedAt.Elapsed() >= GetWaveDuration(g.run.waveIndex)) {  ///
      if (!g.run.scheduledWaveCompleted.IsSet()) {
        g.run.scheduledWaveCompleted.SetNow();
        g.run.waveWon = true;
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
        if (creature.type == CreatureType_TREE)
          continue;

        const auto fb = fb_creatures->Get(creature.type);

        creature.controller.move
          = Vector2DirectionOrZero(creature.pos, PLAYER_CREATURE.pos);

        if (creature.controller.move.x >= 0)
          creature.dir = {1, 0};
        else
          creature.dir = {-1, 0};

        if (creature.type == CreatureType_RANGER) {
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
        else if (creature.type == CreatureType_RUSHER) {
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
              const auto dist       = Vector2Distance(PLAYER_CREATURE.pos, creature.pos);
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
      }
    }

    // Making pre spawn decals.
    {  ///
      LAMBDA (void, makePreSpawn, (CreatureType type)) {
        ASSERT(type);

        Vector2 posToSpawn{};
        do {
          posToSpawn = {
            CREATURES_SPAWN_MARGIN
              + GRAND.FRand() * (WORLD_X - 2 * CREATURES_SPAWN_MARGIN),
            CREATURES_SPAWN_MARGIN
              + GRAND.FRand() * (WORLD_Y - 2 * CREATURES_SPAWN_MARGIN),
          };
        } while (Vector2DistanceSqr(PLAYER_CREATURE.pos, posToSpawn)
                 < SQR(RADIUS_OF_NOT_SPAWNING_AROUND_PLAYER));

        CreaturePreSpawn spawn{
          .type = type,
          .pos  = posToSpawn,
        };
        spawn.createdAt.SetNow();
        *g.run.creaturePreSpawns.Add() = spawn;
      };

      int  spawnEnemiesEvery = FIXED_FPS;
      auto multiplier        = 1 + (f32)g.run.playerStats[StatType_ENEMIES] / 100.0f;
      multiplier             = MAX(0.001f, multiplier);
      spawnEnemiesEvery      = Round((f32)spawnEnemiesEvery / multiplier);
      spawnEnemiesEvery      = MAX(1, spawnEnemiesEvery);

      if (CanSpawnMoreCreatures()
          && (g.run.waveStartedAt.Elapsed().value % spawnEnemiesEvery == 0))
      {
        FOR_RANGE (int, toSpawnIndex, g.run.toSpawn) {
          const auto   factor = GRAND.FRand();
          CreatureType spawnType{};

          FOR_RANGE (int, k, g.run.thisWaveMobsCount) {
            const auto& c = g.run.thisWaveMobs[k];
            if (factor < c.accumulatedFactor) {
              spawnType = c.type;
              break;
            }
          }
          makePreSpawn(spawnType);
        }

        if (g.run.toSpawn < 3)
          g.run.toSpawn++;
      }

      // Spawning trees every 10 seconds.
      if ((g.run.waveStartedAt.Elapsed().value + 1) % (FIXED_FPS * 10) == 0) {
        const int toSpawn = GetNumberOfTreesToSpawn();
        FOR_RANGE (int, i, toSpawn) {
          makePreSpawn(CreatureType_TREE);
        }
      }
    }

    // Spawning.
    {  ///
      const int total = g.run.creaturePreSpawns.count;
      int       off   = 0;
      FOR_RANGE (int, i, total) {
        auto& v = g.run.creaturePreSpawns[i - off];
        if (v.createdAt.IsSet() && (v.createdAt.Elapsed() >= SPAWN_FRAMES)) {
          MakeCreature({
            .type = v.type,
            .pos  = v.pos,
          });
          g.run.creaturePreSpawns.UnstableRemoveAt(i - off);
          off++;
        }
      }
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

        if (Vector2DistanceSqr(playerPos, creature.pos)
            <= SQR(PLAYER_HURTBOX_RADIUS + MOB_HURTBOX_RADIUS))
        {
          const f32 damage
            = fb->contact_damage()
              + fb->contact_damage_increase_per_wave()
                  * (f32)(g.run.waveIndex + 1 - fb->appearing_wave_number());
          TryApplyDamage({
            .creatureIndex                = 0,
            .damage                       = ApplyPlayerArmorToIncomingDamage(damage),
            .damageApplicatorCreatureType = creature.type,
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
            MakeNumber({
              .type  = NumberType_PICKUPABLE,
              .value = 1,
              .pos   = PLAYER_CREATURE.pos + Vector2(0, PLAYER_PICKUP_NUMBER_Y_OFFSET),
            });

            const int consumableOrCrateHeal
              = MAX(1, g.run.playerStats[StatType_CONSUMABLE_HEAL]);

            switch (pickupable.type) {
            case PickupableType_COIN: {
              const auto& data   = pickupable.DataCoin();
              int         amount = data.amount;

              if (GRAND.FRand()
                  < (f32)g.run.playerStats[StatType_DOUBLE_MATERIAL_CHANCE] / 100.0f)
                amount *= 2;

              AddCoins(amount);
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
              g.run.crates++;
            } break;

            default:
              INVALID_PATH;
            }
          }
        }
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
              bool crit = false;
              if (a.ownerCreatureType == CreatureType_PLAYER)
                crit = IsCrit();
              TryApplyDamage({
                .creatureIndex                = creatureIndex,
                .damage                       = a.value,
                .damageApplicatorCreatureType = a.ownerCreatureType,
                .isCrit                       = crit,
                .indexOfWeaponThatDidDamage   = a.weaponIndex,
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
  }

  // Creatures moving.
  {  ///
    ZoneScopedN("Creatures moving.");

    for (auto& creature : g.run.creatures) {
      if (creature.diedAt.IsSet())
        continue;

      const auto fb = fb_creatures->Get(creature.type);

      auto speedScale = creature.speed * creature.speedModifier * SPEED_MULTIPLIER;
      if (creature.type == CreatureType_PLAYER) {
        speedScale *= MAX(0, (f32)(100 + g.run.playerStats[StatType_SPEED]) / 100.0f);

        if (g.meta.godMode)
          speedScale *= 1.5f;
      }
      else {
        speedScale *= (f32)(g.run.playerStats[StatType_ENEMY_SPEED] + 100) / 100.0f;
        speedScale = MAX(0, speedScale);
      };

      speedScale *= b2Body_GetMass(creature.body.id) * BODY_LINEAR_DAMPING_SPEED_SCALE;

      b2Body_ApplyLinearImpulseToCenter(
        creature.body.id,
        ToB2Vec2(creature.controller.move * (FIXED_DT * speedScale)),
        true
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
    for (auto& weapon : g.run.playerWeapons) {
      weaponIndex++;
      if (!weapon.type)
        continue;

      const auto fb  = fb_weapons->Get(weapon.type);
      const auto pos = PLAYER_CREATURE.pos + weapon.offset;

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

          if (creature.diedAt.IsSet() || (creature.type == CreatureType_PLAYER))
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
          auto        range           = GetWeaponRange(weapon.type);

          if (fb->projectile_type()) {
            const auto fb_projectile = glib->projectiles()->Get(fb->projectile_type());
            range += fb_projectile->collider_radius();
          }
          else {
            const auto fb_texture
              = glib->atlas_textures()->Get(fb->texture_ids()->Get(0));
            // TODO: ATLAS_D2.
            range += (f32)fb_texture->size_x() * ASSETS_TO_LOGICAL_RATIO
                     / METER_LOGICAL_SIZE / 2.0f;
          }

          if (minDistSqr < SQR(range)) {
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

        const auto shootingDur
          = ApplyAttackSpeedToDuration(fb->shooting_duration_frames());

        const auto projectileSpawnFrames = fb->projectile_spawn_frames();

        if (projectileType) {
          // It's a weapon that shoots projectiles (RANGED / ELEMENTAL damage types).
          ASSERT(projectileSpawnFrames);

          bool       spawn     = false;
          const auto speedMult = GetPlayerStatAttackSpeedMultiplier();
          for (auto value : *projectileSpawnFrames) {
            if (CeilDivision(value, speedMult) == e.value) {
              spawn = true;
              break;
            }
          }

          if (spawn) {
            MakeProjectile({
              .type              = projectileType,
              .ownerCreatureType = PLAYER_CREATURE.type,
              .weaponIndex       = weaponIndex,
              .pos               = pos,
              .dir               = weapon.targetDir,
              .range             = GetWeaponRange(weapon.type),
              .damage            = GetWeaponDamage(weapon.type, weapon.tier),
              .crit              = fb->critical_damage(),
              .knockbackMeters   = fb->knockback_meters(),
              .pierce            = fb->projectile_pierce(),
              .bounce            = fb->projectile_bounce(),
            });
          }
        }
        else {
          // It's a weapon that gets "shot" itself (MELEE / ELEMENTAL damage types).
          ASSERT(!projectileSpawnFrames);

          auto p = e.Progress(shootingDur);

          const auto colliderActiveStart = 0.25f;
          const auto colliderActiveEnd   = 0.5f;

          const auto texId      = fb->texture_ids()->Get(0);
          const auto fb_texture = glib->atlas_textures()->Get(texId);
          const auto    colliderSize = Vector2{
              // TODO: ATLAS_D2.
              (f32)fb_texture->size_x(),
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
              &weaponIndex
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

  // Updating projectiles:
  // - Movement.
  // - Marking to remove because of travel distance.
  // - Mob collisions.
  // - Marking to remove because of pierce count.
  {  ///
    ZoneScopedN("Updating projectiles.");

    int projectileIndex = -1;
    for (auto& projectile : g.run.projectiles) {
      projectileIndex++;

      const auto fb       = fb_projectiles->Get(projectile.type);
      const auto distance = FIXED_DT * fb->speed();
      projectile.travelledDistance += distance;
      projectile.pos += projectile.dir * distance;

      if (projectile.travelledDistance >= projectile.range) {
        if (!g.run.projectilesToRemove.Contains(projectileIndex))
          *g.run.projectilesToRemove.Add() = projectileIndex;
      }

      int start = 0;
      int end   = g.run.creatures.count;

      if (projectile.ownerCreatureType == CreatureType_PLAYER)
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

        const auto distSqr = Vector2DistanceSqr(creature.pos, projectile.pos);
        const auto radius  = fb->collider_radius();
        if (distSqr < SQR(radius)) {
          Vector2 knockbackDirection{};

          f32  damage = projectile.damage;
          bool isCrit = false;
          if (projectile.ownerCreatureType == CreatureType_PLAYER) {
            // Player damages mob.
            damage *= GetPlayerStatDamageMultiplier();
            isCrit = IsCrit();
            if (isCrit)
              damage *= projectile.crit;

            if (fb_creature->is_boss()) {
              damage *= MAX(
                0, (f32)(g.run.playerStats[StatType_DAMAGE_AGAINST_BOSSES] + 100) / 100.0f
              );
            }

            knockbackDirection
              = Vector2DirectionOrRandom(PLAYER_CREATURE.pos, creature.pos);
          }
          else {
            // Mob damages player.
            damage *= ApplyPlayerArmorToIncomingDamage(damage);
          }

          const f32 piercingDamageMultiplier
            = (f32)g.run.playerStats[StatType_PIERCING_DAMAGE] / 100.0f;

          f32 knockback = projectile.knockbackMeters;

          FOR_RANGE (int, i, projectile.piercedCount) {
            damage *= piercingDamageMultiplier;
            knockback *= piercingDamageMultiplier;
          }

          if (TryApplyDamage({
                .creatureIndex                = creatureIndex,
                .damage                       = damage,
                .directionOrZero              = knockbackDirection,
                .knockbackMeters              = knockback,
                .damageApplicatorCreatureType = projectile.ownerCreatureType,
                .isCrit                       = isCrit,
                .indexOfWeaponThatDidDamage   = projectile.weaponIndex,
                .ailment                      = (AilmentType)fb->ailment_type(),
                .ailmentChance                = fb->ailment_chance(),
                .ailmentValue                 = fb->ailment_value(),
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

              FOR_RANGE (int, i, 8) {
                const auto c
                  = g.run.creatures.base + GRAND.Rand() % g.run.creatures.count;

                if (!AreEnemies(c->type, CreatureType_PLAYER))
                  continue;
                if (c->diedAt.IsSet())
                  continue;
                if (ArrayContains(
                      projectile.damagedCreatureIds.base, projectile.damagedCount, c->id
                    ))
                  continue;

                const auto forecastedCreaturePos = ForecastWhereProjectileWillHitCreature(
                  c->pos,
                  c->controller.move * c->speed * c->speedModifier,
                  projectile.pos,
                  fb->speed()
                );
                const auto d = Vector2DistanceSqr(forecastedCreaturePos, projectile.pos);
                if (d <= SQR(projectile.range)) {
                  found         = c;
                  forecastedPos = forecastedCreaturePos;
                  break;
                }
              }
              if (found)
                projectile.dir = Vector2DirectionOrRandom(projectile.pos, forecastedPos);
              else
                projectile.dir = Vector2Rotate({1, 0}, 2 * PI * GRAND.FRand());
            }
            else if (canPierce) {
              projectile.damagedCreatureIds[projectile.damagedCount++] = creature.id;
              projectile.piercedCount++;
            }
            else if (!g.run.projectilesToRemove.Contains(projectileIndex))
              *g.run.projectilesToRemove.Add() = projectileIndex;
          }
        }
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
            g.run.waveWon = false;
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
              constexpr Rect bounds{
                .pos{CREATURES_SPAWN_MARGIN, CREATURES_SPAWN_MARGIN},
                .size{
                  WORLD_X - 2 * CREATURES_SPAWN_MARGIN,
                  WORLD_Y - 2 * CREATURES_SPAWN_MARGIN
                },
              };

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
                        GRAND.FRand() * 2.0f * PI
                      );
              } while (!bounds.ContainsInside(pos));

              CreaturePreSpawn spawn{
                .type = (CreatureType)fb->on_death_spawns_creature_type(),
                .pos  = pos,
              };
              spawn.createdAt.SetNow();
              *g.run.creaturePreSpawns.Add() = spawn;
            }
          }

          // Mob drops coin / consumable / crate.
          if (creature.health != -f32_inf) {
            MakePickupableData data{
              .type        = PickupableType_COIN,
              .pos         = creature.pos,
              .coin_amount = fb->coins_dropped(),
            };
            MakePickupable(data);

            const auto luckFactor = GetLuckFactor();
            if (GRAND.FRand() <= fb->consumable_drop_chance() * luckFactor) {
              data.type = PickupableType_CONSUMABLE;

              const auto crateChance = CRATE_INSTEAD_OF_CONSUMABLE_FACTOR * luckFactor
                                       / (f32)(1 + g.run.cratesDroppedThisWave);
              if (GRAND.FRand() <= crateChance) {
                data.type = PickupableType_CRATE;
                g.run.cratesDroppedThisWave++;
              }

              MakePickupable(data);
            }
          }

          // Counting mob as player killed.
          if (creature.health != -f32_inf) {
            g.run.playerKilledEnemies++;
            if (creature.lastDamagedWeaponIndex >= 0)
              g.run.playerWeapons[creature.lastDamagedWeaponIndex].killedEnemies++;

            IterateOverWeaponEffects(
              EffectConditionType_KILL_N_ENEMIES_GET_STAT,
              [&](int _, Weapon& weapon, auto fb_effect) BF_FORCE_INLINE_LAMBDA {
                if (g.run.playerKilledEnemies % fb_effect->condition_value() == 0)
                  ApplyEffect(fb_effect, 1);
              }
            );
            IterateOverWeaponEffects(
              EffectConditionType_KILL_N_ENEMIES_USING_THIS_WEAPON_GET_STAT,
              [&](int _, Weapon& weapon, auto fb_effect) BF_FORCE_INLINE_LAMBDA {
                if (weapon.killedEnemies % fb_effect->condition_value() == 0)
                  ApplyEffect(fb_effect, 1);
              }
            );
            IterateOverItemEffects(
              EffectConditionType_KILL_N_ENEMIES_GET_STAT,
              [&](const Item& item, auto fb_effect) BF_FORCE_INLINE_LAMBDA {
                if (g.run.playerKilledEnemies % fb_effect->condition_value() == 0)
                  ApplyEffect(fb_effect, item.count);
              }
            );
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

  DoUI(false);

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
    }
  }

  g.run.camera.pos = GetCameraTargetPos();
  g.meta.frame++;
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

  const auto fb_hostilities = glib->hostilities();
  const auto fb_creatures   = glib->creatures();
  const auto fb_weapons     = glib->weapons();
  const auto fb_projectiles = glib->projectiles();
  const auto fb_pickupables = glib->pickupables();
  const auto fb_numbers     = glib->numbers();
  // }

  BeginMode2D(&g.run.camera);

  // Drawing floor.
  {  ///
    DrawGroup_OneShotRect(
      {
        .pos   = (Vector2)WORLD_SIZE / 2.0f,
        .size  = (Vector2)WORLD_SIZE,
        .color = {0x3a, 0x3a, 0x41, 0xff},
      },
      DrawZ_FLOOR
    );
  }

  // Drawing creature spawns.
  {  ///
    const auto texId = glib->decal_pre_spawn_texture_id();

    DrawGroup_Begin(DrawZ_FLOOR_DECALS);
    DrawGroup_SetSortY(0);

    for (auto& spawn : g.run.creaturePreSpawns) {
      const auto fb_creature  = fb_creatures->Get(spawn.type);
      const auto fb_hostility = fb_hostilities->Get(fb_creature->hostility_type());
      DrawGroup_CommandTexture({
        .texId = texId,
        .pos   = spawn.pos,
        .color = ColorFromRGB(fb_hostility->color()),
      });
    }

    DrawGroup_End();
  }

  // Drawing creatures.
  {  ///
    for (const auto& creature : g.run.creatures) {
      const auto fb = fb_creatures->Get(creature.type);

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
          const auto idleDuration = lframe::MakeUnscaled(fb->idle_seconds() * FIXED_FPS);
          p = fmodf(creature.idleStartedAt.Elapsed().Progress(idleDuration), 1);
        }
        texId = GetTextureIdByProgress(fb->idle_texture_ids(), p);
      }

      Vector2 scale{1, 1};
      if (creature.dir.x < 0)
        scale.x = -1;

      auto color = ColorFromRGB(fb->color());
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
        color = ColorLerp(color, RED, t);
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
        color = ColorLerp(color, RED, t);
      }

      DrawGroup_OneShotTexture(
        {
          .texId = texId,
          .pos   = creature.pos,
          .scale = scale,
          .color = Fade(color, fade),
        },
        DrawZ_DEFAULT
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

          DrawGroup_OneShotTexture(
            {
              .texId    = fb->texture_ids()->Get(0),
              .rotation = rotation,
              .pos      = GetWeaponPos(weapon),
              .scale    = scale,
              .color    = Fade(ColorFromRGB(fb->color()), fade),
            },
            DrawZ_WEAPONS
          );
        }
      }
    }
  }

  // Drawing projectiles + their gizmos.
  {  ///
    for (const auto& projectile : g.run.projectiles) {
      const auto fb = fb_projectiles->Get(projectile.type);

      f32     rotation = Vector2Angle(projectile.dir);
      Vector2 scale{1, 1};
      if (projectile.dir.x < 0) {
        rotation += (f32)PI;
        scale.x = -1;
      }

      DrawGroup_OneShotTexture(
        {
          .texId    = fb->texture_ids()->Get(0),
          .rotation = rotation,
          .pos      = projectile.pos,
          .scale    = scale,
          .color    = ColorFromRGB(fb->color()),
        },
        DrawZ_DEFAULT
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
        const auto fb = localization_strings->Get(glib->ui_dodge_locale());
        buffer        = fb->c_str();
        bytesCount    = fb->size();
      }
      else if (number.type == NumberType_LEVEL_UP) {
        const auto fb = localization_strings->Get(glib->ui_level_up_number_locale());
        buffer        = fb->c_str();
        bytesCount    = fb->size();
      }
      else {
        const char* format = "%d";
        if (number.type == NumberType_PICKUPABLE)
          format = "+%d";

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
        .font       = &g.meta.uiFont,
        .text       = buffer,
        .bytesCount = (int)bytesCount,
        .color      = Fade(ColorFromRGB(fb->color()), fade),
      });
    }

    DrawGroup_End();
  }

  // Drawing pickupables.
  {  ///
    for (const auto& pickupable : g.run.pickupables) {
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

      DrawGroup_OneShotTexture(
        {
          .texId = fb->texture_id(),
          .pos   = pickupable.pos,
          .color = Fade(WHITE, fade),
        },
        DrawZ_PICKUPABLES
      );
    }
  }

  // Gizmos. Colliders.
  {  ///
    if (ge.meta.debugEnabled) {
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
  }

  EndMode2D();

  // Drawing wave completion animation.
  {  ///
    if (g.run.scheduledWaveCompleted.IsSet()) {
      auto p
        = Clamp01(g.run.scheduledWaveCompleted.Elapsed().Progress(WAVE_COMPLETED_FRAMES));

      int locale = glib->ui_label_wave_won_locale();
      if (!g.run.waveWon)
        locale = glib->ui_label_wave_lost_locale();
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
            .font       = &g.meta.uiFont,
            .text       = text->c_str(),
            .bytesCount = bytesToShow,
            .color      = WHITE,
          },
          DrawZ_UI
        );
      }
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
    DebugText("F6 add item");
    if (g.run.screen == ScreenType_GAMEPLAY) {
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
