#pragma once

// Sizings + colliders.
// ============================================================
constexpr int        WORLD_X    = 20;
constexpr int        WORLD_Y    = 20;
constexpr Vector2Int WORLD_SIZE = {WORLD_X, WORLD_Y};

constexpr f32 PLAYER_HURTBOX_RADIUS    = 0.7f;
constexpr f32 MOB_HURTBOX_RADIUS       = 0.9f;
constexpr f32 CREATURE_COLLIDER_RADIUS = 0.6f;

// Other.
// ============================================================
constexpr f32 PLAYER_SPEED_FORCE = 100.0f;
