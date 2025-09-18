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

constexpr f32 CREATURES_SPAWN_MARGIN = 0.5f;

constexpr f32 WORLD_RESOLUTION = 1600.0f;

constexpr f32 METER_LOGICAL_SIZE = WORLD_RESOLUTION / (f32)WORLD_X;
static_assert(METER_LOGICAL_SIZE * WORLD_X >= LOGICAL_RESOLUTION.x);

constexpr f32 WORLD_CAMERA_METERS_MARGIN = 2;

constexpr f32 CAMERA_MIN_WORLD_X
  = LOGICAL_RESOLUTION.x / METER_LOGICAL_SIZE / 2.0f - WORLD_CAMERA_METERS_MARGIN;
constexpr f32 CAMERA_MAX_WORLD_X
  = (WORLD_RESOLUTION - LOGICAL_RESOLUTION.x / 2.0f) / METER_LOGICAL_SIZE
    + WORLD_CAMERA_METERS_MARGIN;
constexpr f32 CAMERA_MIN_WORLD_Y
  = LOGICAL_RESOLUTION.y / METER_LOGICAL_SIZE / 2.0f - WORLD_CAMERA_METERS_MARGIN;
constexpr f32 CAMERA_MAX_WORLD_Y
  = (WORLD_RESOLUTION - LOGICAL_RESOLUTION.y / 2.0f) / METER_LOGICAL_SIZE
    + WORLD_CAMERA_METERS_MARGIN;

// As Brotato community in steam says:
// 900 range = can shoot to corner using slingshot.
// Half of our screen is {16 meters, 9 meters}.
// meter/range = sqrt(9**2 + 16**2) / 900 = 0.0204f
// Using higher value that "feels" good.
constexpr f32 RANGE_TO_METER_SCALE = 0.04f;

#define SQR(v) ((v) * (v))

constexpr f32 PLAYER_HURTBOX_RADIUS     = 0.6f;
constexpr f32 MOB_HURTBOX_RADIUS        = 0.7f;
constexpr f32 CREATURE_COLLIDER_RADIUS  = 0.5f;
constexpr f32 PICKUPABLE_HURTBOX_RADIUS = 3.0f;

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

constexpr auto DONT_SPAWN_RIGHT_BEFORE_WAVE_ENDS = lframe::MakeScaled(10);
constexpr auto SPAWN_FRAMES                      = lframe::MakeScaled(60);
constexpr auto DIE_FRAMES                        = lframe::MakeScaled(10);
constexpr auto PLAYER_INVINCIBILITY_FRAMES       = lframe::MakeScaled(10);
constexpr auto DAMAGE_NUMBERS_FRAMES             = lframe::MakeScaled(30);
constexpr auto DAMAGE_NUMBERS_FADE_FRAMES        = lframe::MakeScaled(10);
constexpr auto PICKUPABLE_FADE_FRAMES            = lframe::MakeScaled(10);
constexpr auto LIFESTEAL_COOLDOWN_FRAMES         = lframe::MakeScaled(3);
constexpr auto WAVE_COMPLETED_FRAMES             = lframe::MakeScaled(60);

// Projectiles and weapons won't be able to damage
// the same creatures they've already damaged
// on previous frames of animation / projectile flying.
constexpr int PROJECTILE_MAX_PIERCE = 12;
constexpr int WEAPON_MAX_PIERCE     = 32;

constexpr f32 PLAYER_PICKUP_NUMBER_Y_OFFSET = -0.5f;

constexpr f32 SHOP_ITEM_RATIO = 0.4f;  // X% of items in shop, (1-X)% of weapons.

constexpr int TOTAL_WAVES = (BF_DEBUG ? 5 : 20);

constexpr int PLAYER_WEAPONS_COUNT = 6;
constexpr f32 PLAYER_WEAPONS_STARTING_ANGLES_[]{
  0,
  0,
  -(f32)PI / 2.0f,
  (f32)PI / 4.0f,
  -(f32)PI / 2.0f + 2.0f * (f32)PI / 5.0f,
  0
};

VIEW_FROM_ARRAY_DANGER(PLAYER_WEAPONS_STARTING_ANGLES);

constexpr int TOTAL_TIERS = 4;

constexpr f32 CRATE_INSTEAD_OF_CONSUMABLE_FACTOR = 0.25f;

const Color MODAL_OVERLAY_COLOR = Fade(BLACK, 0.75f);

constexpr f32 CRIT_DAMAGE_MULTIPLIER = 1.5f;

constexpr auto MOB_RANGER_SHOOTING_FRAMES      = lframe::MakeScaled(40);
constexpr auto MOB_RANGER_SHOOTING_FRAME       = lframe::MakeScaled(36);
constexpr f32  MOB_RANGER_MOVEMENT_SPEED_SCALE = 0.3f;

constexpr auto MOB_RUSHER_RUSH_PRE_FRAMES   = lframe::MakeScaled(10);
constexpr auto MOB_RUSHER_RUSH_TOTAL_FRAMES = lframe::MakeScaled(60);
constexpr auto MOB_RUSHER_RUSH_POST_FRAMES  = lframe::MakeScaled(20);
constexpr auto MOB_RUSHER_RUSH_SPEED_SCALE  = 2.0f;

///
