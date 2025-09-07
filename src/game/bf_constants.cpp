#pragma once

// Sizings + colliders.
// ============================================================
constexpr int        WORLD_X                              = 40;
constexpr int        WORLD_Y                              = 40;
constexpr Vector2Int WORLD_SIZE                           = {WORLD_X, WORLD_Y};
constexpr f32        RADIUS_OF_NOT_SPAWNING_AROUND_PLAYER = 10;
constexpr f32        SPEED_MULTIPLIER                     = 10;
constexpr f32        WORLD_CORNER_MARGIN_X                = 3.0f;
constexpr f32        WORLD_CORNER_MARGIN_Y                = 3.0f * 9.0f / 16.0f;

#define SQR(v) ((v) * (v))

constexpr f32 PLAYER_HURTBOX_RADIUS     = 0.6f;
constexpr f32 MOB_HURTBOX_RADIUS        = 0.7f;
constexpr f32 CREATURE_COLLIDER_RADIUS  = 0.5f;
constexpr f32 PICKUPABLE_HURTBOX_RADIUS = 3.0f;

constexpr f32 METER_LOGICAL_SIZE = 1600.0f / (f32)WORLD_X;

// Other.
// ============================================================
struct lframe {  ///
  i64 value = i64_max;

  constexpr static lframe MakeScaled(i64 v) {
    return {.value = v * _BF_LOGICAL_FPS_SCALE};
  }

  constexpr static lframe MakeUnscaled(i64 v) {
    return {.value = v};
  }

  f32 Progress(const lframe duration) const {
    return (f32)value / (f32)duration.value;
  }

  bool operator==(const lframe& other) const {
    return value == other.value;
  }

  bool operator>(const lframe& other) const {
    return value > other.value;
  }

  bool operator<(const lframe& other) const {
    return value < other.value;
  }

  bool operator>=(const lframe& other) const {
    return value >= other.value;
  }

  bool operator<=(const lframe& other) const {
    return value <= other.value;
  }

  const lframe operator+(const lframe& other) const {
    return lframe::MakeUnscaled(value + other.value);
  }

  const lframe operator-(const lframe& other) const {
    return lframe::MakeUnscaled(value - other.value);
  }

  void SetRand(lframe v) {
    value = ge.meta.logicRand.Rand() % v.value;
  }

  void SetRand(lframe v1, lframe v2) {
    ASSERT(v2.value > v1.value);
    value = v1.value + ge.meta.logicRand.Rand() % (v2.value - v1.value);
  }
};

constexpr auto SPAWN_FRAMES                = lframe::MakeScaled(90);
constexpr auto DIE_FRAMES                  = lframe::MakeScaled(10);
constexpr auto PLAYER_INVINCIBILITY_FRAMES = lframe::MakeScaled(10);
constexpr auto DAMAGE_NUMBERS_FRAMES       = lframe::MakeScaled(30);
constexpr auto DAMAGE_NUMBERS_FADE_FRAMES  = lframe::MakeScaled(10);
constexpr auto PICKUPABLE_FADE_FRAMES      = lframe::MakeScaled(10);

constexpr int PROJECTILE_MAX_PIERCE = 10;

///
