#pragma once

constexpr int MAX_PARTICLES_COUNT = 2048;
constexpr int MAX_NUMBERS_COUNT   = 1024;

// Sizings + colliders.
// ============================================================
constexpr int        WORLD_X                                  = 32;
constexpr int        WORLD_Y                                  = 32;
constexpr Vector2Int WORLD_SIZE                               = {WORLD_X, WORLD_Y};
constexpr Vector2    WORLD_SIZEf                              = (Vector2)WORLD_SIZE;
constexpr f32        ENEMIES_DECAY_SPAWNING_AROUND_PLAYER_MAX = 12;
constexpr f32        ENEMIES_DECAY_SPAWNING_AROUND_PLAYER_MIN = 4;
constexpr f32        WORLD_CORNER_MARGIN_X                    = 3.0f;
constexpr f32        WORLD_CORNER_MARGIN_Y                    = 3.0f * 9.0f / 16.0f;

constexpr f32 CREATURES_SPAWN_MARGIN = 0.75f;

constexpr Rect CREATURES_WORLD_SPAWN_BOUNDS{
  .pos{CREATURES_SPAWN_MARGIN, CREATURES_SPAWN_MARGIN},
  .size{WORLD_X - 2 * CREATURES_SPAWN_MARGIN, WORLD_Y - 2 * CREATURES_SPAWN_MARGIN},
};

constexpr f32 WORLD_RESOLUTION = 1450.0f;

constexpr f32 BOSS_SPAWN_OFFSET_METERS = 8;

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
// Using value that "feels" good.
constexpr f32 RANGE_TO_METER_SCALE = 0.02f;

#define SQR(v) ((v) * (v))

constexpr f32 PLAYER_HURTBOX_RADIUS     = 0.6f;
constexpr f32 MOB_HURTBOX_RADIUS        = 0.7f;
constexpr f32 CREATURE_COLLIDER_RADIUS  = 0.5f;
constexpr f32 LANDMINE_COLLIDER_RADIUS  = 0.5f;
constexpr f32 PICKUPABLE_HURTBOX_RADIUS = 3.0f;
constexpr f32 BURNING_SPREAD_DISTANCE   = MOB_HURTBOX_RADIUS * 3.5f;

constexpr auto GARDEN_FRUIT_SPAWNING_INTERVAL     = lframe::FromSeconds(10);
constexpr f32  GARDEN_PICKUPABLE_SPAWN_RADIUS_MIN = 1;
constexpr f32  GARDEN_PICKUPABLE_SPAWN_RADIUS_MAX = 2;

constexpr f32 MELEE_WEAPON_COLLIDER_PHANTOM_RANGE_METERS = 0.12f;

constexpr f32 WALKING_TUTORIAL_RADIUS_METERS = 4;

// Other.
// ============================================================
constexpr auto IDLE_OR_WALKING_BONUS_FRAMES = FIXED_FPS / 8;

constexpr auto ANIMATION_0_FRAMES = lframe::FromSeconds(0.2f);
constexpr auto ANIMATION_1_FRAMES = lframe::FromSeconds(0.5f);
constexpr auto ANIMATION_2_FRAMES = lframe::FromSeconds(1.2f);
constexpr auto ANIMATION_3_FRAMES = lframe::FromSeconds(3.2f);
constexpr auto ANIMATION_4_FRAMES = lframe::FromSeconds(4.5f);
constexpr auto ANIMATION_5_FRAMES = lframe::FromSeconds(7.0f);

constexpr auto MELEE_WEAPON_MAX_ATTACK_DURATION           = lframe::FromSeconds(0.5f);
constexpr auto WEAPON_BURNING_APPLICATION_DEBOUNCE_FRAMES = lframe::FromSeconds(0.5f);

constexpr auto DONT_SPAWN_RIGHT_BEFORE_WAVE_ENDS        = lframe::Scaled(10);
constexpr auto SPAWN_FRAMES                             = lframe::Scaled(60);
constexpr auto DIE_FRAMES                               = lframe::Scaled(10);
constexpr auto PLAYER_INVINCIBILITY_FRAMES              = lframe::Scaled(10);
constexpr auto DAMAGE_NUMBERS_FRAMES                    = lframe::Scaled(30);
constexpr auto DAMAGE_NUMBERS_FADE_FRAMES               = lframe::Scaled(8);
constexpr auto PICKUPABLE_FADE_FRAMES                   = lframe::Scaled(10);
constexpr auto LIFESTEAL_COOLDOWN_FRAMES                = lframe::Scaled(3);
constexpr auto WAVE_COMPLETED_FRAMES                    = lframe::FromSeconds(2);
constexpr auto WAVE_COMPLETED_COINS_FLYING_FRAMES       = lframe::FromSeconds(0.9f);
constexpr auto WAVE_COMPLETED_COINS_FLYING_FRAMES_RIGHT = lframe::FromSeconds(0.3f);

static_assert(WAVE_COMPLETED_FRAMES.value > 0);
static_assert(WAVE_COMPLETED_COINS_FLYING_FRAMES.value > 0);
static_assert(WAVE_COMPLETED_COINS_FLYING_FRAMES_RIGHT.value > 0);
static_assert(
  WAVE_COMPLETED_FRAMES.value >= WAVE_COMPLETED_COINS_FLYING_FRAMES.value
                                   + WAVE_COMPLETED_COINS_FLYING_FRAMES_RIGHT.value
);

// Projectiles and weapons won't be able to damage
// the same creatures they've already damaged
// on previous frames of animation / projectile flying.
constexpr int PROJECTILE_MAX_PIERCE = 12;
constexpr int WEAPON_MAX_PIERCE     = 32;

constexpr f32 PLAYER_PICKUP_NUMBER_Y_OFFSET = -0.5f;

constexpr f32 SHOP_ITEM_RATIO = 0.4f;  // X% of items in shop, (1-X)% of weapons.

constexpr int TOTAL_WAVES = 20;

constexpr int PLAYER_WEAPONS_COUNT = 6;
constexpr f32 PLAYER_WEAPONS_STARTING_ANGLES_[PLAYER_WEAPONS_COUNT]{
  0,
  0,
  (f32)PI32 / 6.0f,
  (f32)PI32 / 4.0f,
  -(f32)PI32 / 2.0f + 2.0f * (f32)PI32 / 5.0f,
  0
};
VIEW_FROM_ARRAY_DANGER(PLAYER_WEAPONS_STARTING_ANGLES);
constexpr DrawZ PLAYER_WEAPONS_DRAW_Z_[PLAYER_WEAPONS_COUNT][PLAYER_WEAPONS_COUNT]{
  {DrawZ_WEAPONS_1,  // 1 weapon.
   DrawZ_INVALID,
   DrawZ_INVALID,
   DrawZ_INVALID,
   DrawZ_INVALID,
   DrawZ_INVALID},
  {DrawZ_WEAPONS_2,  // 2 weapons.
   DrawZ_WEAPONS_1,
   DrawZ_INVALID,
   DrawZ_INVALID,
   DrawZ_INVALID,
   DrawZ_INVALID},
  {DrawZ_WEAPONS_2,  // 3 weapons.
   DrawZ_WEAPONS_1,
   DrawZ_WEAPONS_3,
   DrawZ_INVALID,
   DrawZ_INVALID,
   DrawZ_INVALID},
  {DrawZ_WEAPONS_2,  // 4 weapons.
   DrawZ_WEAPONS_1,
   DrawZ_WEAPONS_3,
   DrawZ_WEAPONS_4,
   DrawZ_INVALID,
   DrawZ_INVALID},
  {DrawZ_WEAPONS_4,  // 5 weapons.
   DrawZ_WEAPONS_2,
   DrawZ_WEAPONS_1,
   DrawZ_WEAPONS_3,
   DrawZ_WEAPONS_5,
   DrawZ_INVALID},
  {DrawZ_WEAPONS_4,  // 6 weapons.
   DrawZ_WEAPONS_2,
   DrawZ_WEAPONS_1,
   DrawZ_WEAPONS_3,
   DrawZ_WEAPONS_5,
   DrawZ_WEAPONS_6},
};
VIEW_FROM_ARRAY_DANGER(PLAYER_WEAPONS_DRAW_Z);

constexpr int TOTAL_TIERS = 4;

constexpr Color MODAL_OVERLAY_COLOR      = BLACK;
constexpr f32   MODAL_OVERLAY_COLOR_FADE = 0.75f;

constexpr f32 CRIT_DAMAGE_MULTIPLIER = 1.5f;

constexpr auto MOB_RANGER_SHOOTING_FRAMES  = lframe::Scaled(40);
constexpr auto MOB_RANGER_SHOOTING_FRAME   = lframe::Scaled(36);
constexpr f32  MOB_RANGER_THRESHOLD_METERS = 0.5f;
constexpr f32  MOB_RANGER_SHOOT_METERS     = 8;

constexpr auto MOB_RUSHER_RUSH_PRE_FRAMES   = lframe::Scaled(10);
constexpr auto MOB_RUSHER_RUSH_TOTAL_FRAMES = lframe::Scaled(60);
constexpr auto MOB_RUSHER_RUSH_POST_FRAMES  = lframe::Scaled(20);
constexpr auto MOB_RUSHER_COOLDOWN_MIN      = lframe::Unscaled(2.5f * FIXED_FPS);
constexpr auto MOB_RUSHER_COOLDOWN_MAX      = lframe::Unscaled(3.5f * FIXED_FPS);

constexpr auto MOB_TURRET_SHOOTING_FRAMES = lframe::Scaled(10);
constexpr auto MOB_TURRET_SHOOT_FRAME     = lframe::Scaled(2);

constexpr f32 PRICE_SCALINGS_PER_TIER_[TOTAL_TIERS]{
  20.0f / 149.0f,
  39.0f / 149.0f,
  74.0f / 149.0f,
  149.0f / 149.0f
};
VIEW_FROM_ARRAY_DANGER(PRICE_SCALINGS_PER_TIER);

constexpr int MAX_DODGE_PERCENT      = 60;
constexpr f32 RANGE_GETS_HALVED_WHEN = -50;

constexpr auto BURNING_RATE    = lframe::Scaled(15);
constexpr auto ZAP_SPEED_SCALE = 0.4f;

// constexpr f32 BODY_LINEAR_DAMPING             = 10;
// constexpr f32 BODY_LINEAR_DAMPING_SPEED_SCALE = 7.0f / 6.78f * 10.0f;
constexpr f32 BODY_LINEAR_DAMPING             = 20;
constexpr f32 BODY_LINEAR_DAMPING_SPEED_SCALE = 7.0f / 3.29f * 10.0f;
// constexpr f32 BODY_LINEAR_DAMPING             = 30;
// constexpr f32 BODY_LINEAR_DAMPING_SPEED_SCALE = 7.0f / 2.13f * 10.0f;
// constexpr f32 BODY_LINEAR_DAMPING             = 40;
// constexpr f32 BODY_LINEAR_DAMPING_SPEED_SCALE = 7.0f / 1.55f * 10.0f;
// constexpr f32 BODY_LINEAR_DAMPING             = 50;
// constexpr f32 BODY_LINEAR_DAMPING_SPEED_SCALE = 7.0f / 1.2f * 10.0f;

constexpr f32 STRUCTURE_MIN_RANGE_METERS = 2;

constexpr int  SPAWNING_TURRETS_OR_GARDENS_EVERY_N_SECONDS = 2;
constexpr int  SPAWNING_GARDENS_EVERY_N_SECONDS            = 5;
constexpr auto SPAWNING_LANDMINES_INTERVAL_FRAMES          = lframe::FromSeconds(8);
constexpr auto LANDMINE_DETONATION_FRAMES                  = lframe::FromSeconds(0.5);

// Colors.
// ============================================================
#define PAL_COLORS_TABLE \
  X(Green, 0x66a650ff)   \
  X(Lime, 0xb9d850ff)    \
  X(Blue, 0xb9d850ff)    \
  X(Purple, 0xb9d850ff)  \
  X(Orange, 0xb9d850ff)  \
  X(Red, 0xc02931ff)     \
  X(Bezhevy, 0xb59a66ff) \
  X(White, 0xedefe2ff)   \
  X(Gray, 0x7a7576ff)    \
  X(Yellow, 0xdc9824ff)  \
  X(PaleYellow, 0xefcb84ff)

#define X(name_, value_) constexpr auto pal##name_ = ColorFromRGBA(value_);
PAL_COLORS_TABLE;
#undef X

Color TextifyColor(Color color) {  ///
  auto v = ColorToHSV(color);
  return ColorFromHSV(v.x, MIN(1, v.y * 1.0f), MIN(1, v.z * 1.3f));
}

#define X(name_, value_) const auto palText##name_ = TextifyColor(ColorFromRGBA(value_));
// #define X(name_, value_) \
//   constexpr auto palText##name_ = ColorContrast(ColorFromRGBA(value_), 0.5f);
PAL_COLORS_TABLE;
#undef X
constexpr auto palTextBlack = BLACK;

const Color textColorsPerTier_[]{
  WHITE,
  TextifyColor(ColorFromRGBA(0x208cb2ff)),
  TextifyColor(ColorFromRGBA(0xa13d77ff)),
  ColorFromRGBA(0xdc9824ff),
  TextifyColor(ColorFromRGBA(0xc02931ff)),
  TextifyColor(ColorFromRGBA(0x82dcd7ff)),
  WHITE,
  WHITE,
};
VIEW_FROM_ARRAY_DANGER(textColorsPerTier);

constexpr f32 _PrecalcFlashingX(lframe dur, int times, f32 ratio) {
  return (f32)(dur.value / (times * ratio + (times - 1) * (1 - ratio)));
}

Color GetFlashingColor(
  Color  original,
  Color  flashColor,
  auto   frame,
  lframe dur,
  int    times,
  f32    ratio,
  f32    precalcX
) {  ///
  if (times <= 0)
    return original;
  if (!frame.IsSet())
    return original;

  const auto e_ = frame.Elapsed();

  if (e_ < dur) {
    auto e          = (f32)e_.value;
    bool isFlashing = true;
    FOR_RANGE (int, i, times * 2 - 1) {
      if (isFlashing)
        e -= precalcX * ratio;
      else
        e -= precalcX * (1 - ratio);

      isFlashing = !isFlashing;
      if (e < 0)
        break;
    }

    if (!isFlashing)
      return flashColor;
  }

  return original;
}

constexpr f32  GOLD_WIGGLING_LOGICAL_AMPLITUDE    = 10.0f;
constexpr f32  WEAPONS_WIGGLING_LOGICAL_AMPLITUDE = 15.0f;
constexpr auto ERROR_WIGGLING_FRAMES              = lframe::FromSeconds(0.3f);
constexpr int  ERROR_WIGGLING_TIMES               = 4;

constexpr f32 ERROR_GOLD_FLASHING_TIMES        = 3;
constexpr f32 ERROR_GOLD_FLASH_NOT_FLASH_RATIO = 0.75f;
constexpr f32 ERROR_GOLD_PRECALC_X             = _PrecalcFlashingX(
  ERROR_WIGGLING_FRAMES,
  ERROR_GOLD_FLASHING_TIMES,
  ERROR_GOLD_FLASH_NOT_FLASH_RATIO
);

//
// In case of:
//
//   | FLASH | NOT FLASH | FLASH | NOT FLASH | FLASH |
//
// DAMAGED_FLASHING_TIMES would be 2.
// Duration of FLASH     is `DAMAGED_FLASH_PRECALC_X * DAMAGED_FLASH_NOT_FLASH_RATIO`.
// Duration of NOT FLASH is `DAMAGED_FLASH_PRECALC_X * (1-DAMAGED_FLASH_NOT_FLASH_RATIO)`.
//
constexpr auto DAMAGED_FLASHING_FRAMES       = lframe::FromSeconds(0.15f);
constexpr int  DAMAGED_FLASHING_TIMES        = 1;
constexpr f32  DAMAGED_FLASH_NOT_FLASH_RATIO = 0.5f;
constexpr f32  DAMAGED_FLASH_PRECALC_X       = _PrecalcFlashingX(
  DAMAGED_FLASHING_FRAMES,
  DAMAGED_FLASHING_TIMES,
  DAMAGED_FLASH_NOT_FLASH_RATIO
);

constexpr auto ACHIEVEMENT_TOTAL_FRAMES = ANIMATION_4_FRAMES;
constexpr auto ACHIEVEMENT_IN_FRAMES    = ANIMATION_1_FRAMES;
constexpr auto ACHIEVEMENT_OUT_FRAMES   = ANIMATION_1_FRAMES;

constexpr f32 BURNING_CRIT_DAMAGE_MULTIPLIER = 1.5f;

SDL_Scancode KEYS_CANCEL_[]{SDL_SCANCODE_ESCAPE};
VIEW_FROM_ARRAY_DANGER(KEYS_CANCEL);

SDL_Scancode KEYS_PAUSE_[]{SDL_SCANCODE_ESCAPE, SDL_SCANCODE_P};
VIEW_FROM_ARRAY_DANGER(KEYS_PAUSE);

SDL_Scancode KEYS_MOVE_UP_[]{SDL_SCANCODE_PAGEUP};
VIEW_FROM_ARRAY_DANGER(KEYS_MOVE_UP);

SDL_Scancode KEYS_MOVE_DOWN_[]{SDL_SCANCODE_PAGEDOWN};
VIEW_FROM_ARRAY_DANGER(KEYS_MOVE_DOWN);

constexpr int SHOP_SELLING_ITEMS = 4;
const int     DEFAULT_BUYING_INDICES_[SHOP_SELLING_ITEMS]{1, 2, 3, 0};
VIEW_FROM_ARRAY_DANGER(DEFAULT_BUYING_INDICES);

constexpr f32 KNOCKBACK_SCALE = 1 / 7.0f;

///
