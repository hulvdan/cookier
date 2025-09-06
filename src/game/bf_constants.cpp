#pragma once

// Sizings + colliders.
// ============================================================
constexpr int        WORLD_X                              = 20;
constexpr int        WORLD_Y                              = 20;
constexpr Vector2Int WORLD_SIZE                           = {WORLD_X, WORLD_Y};
constexpr f32        RADIUS_OF_NOT_SPAWNING_AROUND_PLAYER = 8;

#define SQR(v) ((v) * (v))

constexpr f32 PLAYER_HURTBOX_RADIUS    = 0.7f;
constexpr f32 MOB_HURTBOX_RADIUS       = 0.9f;
constexpr f32 CREATURE_COLLIDER_RADIUS = 0.6f;

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

///
