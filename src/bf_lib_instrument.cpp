// Other stuff.
// ============================================================

#define BF_PROFILING (BF_DEBUG && defined(SDL_PLATFORM_WIN32))

#define BF_ENABLE_ASSERTS (1 && BF_DEBUG)
#define BF_ENABLE_SLOW_ASSERTS (1 && BF_ENABLE_ASSERTS)

#define BF_SHOW_VERSION (BF_RELEASE && !defined(BF_PLATFORM_WebYandex))
#define BF_DEBUG_VIGNETTE_AND_STRIPS 0

// Unmapping allocator.
// ============================================================

// Set to 1 to enable testing buffer overruns (Windows only).
#if 1 && BF_DEBUG
#  define BF_ALLOC unmapped_alloc
#  define BF_FREE unmapped_free
#else
#  define BF_ALLOC malloc
#  define BF_FREE free
#endif

// To test buffer overruns on right side (eg `char buffer[10]; x = buffer[10]`), set to 1.
// To test on left side (eg buffer[-1]), set to 0.
#define UNMAPPING_ALLOCATOR_ERROR_ON_RIGHT 1

#define UNMAPPING_ALLOCATOR_PAGES_MARGIN 4
