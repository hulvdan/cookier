#pragma once

constexpr int _BF_LOGICAL_FPS       = 30;
constexpr int _BF_LOGICAL_FPS_SCALE = 4;

constexpr int FIXED_FPS = _BF_LOGICAL_FPS * _BF_LOGICAL_FPS_SCALE;
constexpr f32 FIXED_DT  = 1.0f / (f32)FIXED_FPS;

// If player's PC gives <= _BF_MIN_TARGET_FPS FPS,
// then engine would skip simulation frames. Game would run slower.
constexpr int _BF_MIN_TARGET_FPS = 20;

constexpr int BF_MAX_FONT_ATLAS_SIZE = 4096;

constexpr int BF_MAX_SOUNDS = 200;

constexpr f32 BF_SOUND_VOLUME_BOOST_FROM_DB            = -6;
constexpr f32 BF_SOUND_VOLUME_BOOST_TO_DB              = -3;
constexpr int BF_SOUND_VOLUME_BOOST_STEPS              = 3;
constexpr int BF_SOUND_VOLUME_BOOST_MAX_LATENCY_FRAMES = 3;
