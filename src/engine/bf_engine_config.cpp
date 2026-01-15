#pragma once

#define UI_PADDING_OUTER_VERTICAL (10)
#define UI_PADDING_OUTER_HORIZONTAL (12)

struct ClayScreenBackground {  ///
  bool set = false;
};

// Example:
// #define BF_ENGINE_EXTEND_CLAY_CUSTOM_DATA \
//   X(ClayScreenBackground, screenBackground)
//
// ClayScreenBackground would need to have `bool set = false;`
//
#define BF_ENGINE_EXTEND_CLAY_CUSTOM_DATA X(ClayScreenBackground, screenBackground)

#define BF_MAX_PARTICLES_COUNT (2048)

#define BF_MAX_PARTICLES_COUNT (2048)

///
