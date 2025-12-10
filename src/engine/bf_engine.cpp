#pragma once

#include "flatbuffers/bf_save_generated.h"

// Functions that MUST be implemented by the user of the engine in `bf_game.cpp`:
void                           GameInit();
void                           GameFixedUpdate();
void                           GameDraw();
flatbuffers::FlatBufferBuilder GameDumpStateForSaving();
void                           GameLoad(const BFSave::Save* save);

#if BF_DEBUG
#  define STB_IMAGE_WRITE_IMPLEMENTATION
#  include "stb_image_write.h"
#endif

struct Rect {
  Vector2 pos  = {};
  Vector2 size = {};

  constexpr bool ContainsInside(Vector2 v) const {
    if (v.x <= pos.x)
      return false;
    if (v.y <= pos.y)
      return false;
    if (v.x >= pos.x + size.x)
      return false;
    if (v.y >= pos.y + size.y)
      return false;
    return true;
  }

  Vector2 GetRandomGamePosInside() const;
  Vector2 GetRandomVisualPosInside() const;
};

constexpr Vector2Int ASSETS_REFERENCE_RESOLUTION = {1920, 1080};
constexpr Vector2Int LOGICAL_RESOLUTION          = {1280, 720};
constexpr f32        ASSETS_TO_LOGICAL_RATIO     = 1280.0f / 3840.0f;

struct Texture2D {
  Vector2Int          size   = {};
  bgfx::TextureHandle handle = {};
};

constexpr auto             GAMELIB_PATH = "resources/gamelib.bin";
void*                      glibFile     = nullptr;
const BFGame::GameLibrary* glib         = nullptr;
SDL_Time                   glibTime     = {};

struct _PosColorFlashTexVertex {  ///
  f32 x, y, z;
  u32 abgr;
  u32 abgrFlash;
  f32 u, v;

  static bgfx::VertexLayout layout;
};

bgfx::VertexLayout _PosColorFlashTexVertex::layout;

struct _PosColorVertex {  ///
  f32 x, y, z;
  u32 abgr;

  static bgfx::VertexLayout layout;
};

bgfx::VertexLayout _PosColorVertex::layout;

struct Shader {
  bgfx::ProgramHandle program              = {};
  bgfx::VertexLayout  vertexLayout         = {};
  u64                 additionalStateFlags = {};
};

enum DeviceType : int {
  DeviceType_DESKTOP,
  DeviceType_MOBILE,
  DeviceType_COUNT,
};

struct TouchID {
  SDL_TouchID  _touchID  = {};
  SDL_FingerID _fingerID = {};

  bool operator==(const TouchID& other) const {
    return (_touchID == other._touchID) && (_fingerID == other._fingerID);
  }
};

constexpr TouchID InvalidTouchID = {};

struct _TouchEvent {
  TouchID _id          = {};
  i64     _number      = {};
  Vector2 _screenPos   = {};
  Vector2 _screenDelta = {};
};

struct TouchData {
  i64     number      = {};
  Vector2 screenPos   = {};
  Vector2 screenDelta = {};
  u64     userData    = {};
};

enum _TouchDataState : u32 {
  _TouchDataState_DOWN     = 1 << 0,
  _TouchDataState_PRESSED  = 1 << 1,
  _TouchDataState_RELEASED = 1 << 2,
};

struct _TouchData {
  TouchData data  = {};
  u32       state = {};
};

struct Margins {
  f32 left, right, top, bottom = {};
};

struct DrawTextureData {
  int     texID         = -1;
  f32     rotation      = {};
  Vector2 pos           = {};
  Vector2 anchor        = Vector2Half();
  Vector2 scale         = {1, 1};
  Margins sourceMargins = {};
  Color   color         = WHITE;
  Color   flash         = TRANSPARENT_WHITE;
  // bgfx::ProgramHandle program = {};
  // int materialsBufferStart = -1;
};

struct DrawTextureNineSliceData {
  int     texID            = -1;
  f32     rotation         = {};
  Vector2 pos              = {};
  Vector2 anchor           = Vector2Half();
  Vector2 scale            = {1, 1};
  Color   color            = WHITE;
  Color   flash            = TRANSPARENT_WHITE;
  Margins nineSliceMargins = {};
  Vector2 nineSliceSize    = {};
};

struct DrawCircleData {
  // TODO: not only lines.
  Vector2 pos    = {};
  f32     radius = {};
  Color   color  = WHITE;
};

struct DrawRectData {
  Vector2 pos      = {};
  Vector2 size     = {};
  Vector2 anchor   = Vector2Half();
  Vector2 scale    = {1, 1};
  f32     rotation = 0;
  Color   color    = WHITE;
};

struct Font {
  bool loaded = false;

  int size = {};

  // Used for scaling font size.
  // Specifying `size` 30 doesn't guarantee
  // that letter `M` would be 30 px high.
  f32 FIXME_sizeScale = {};

  Texture2D atlasTexture = {};

  const u8*         fileData = {};
  stbtt_packedchar* chars    = {};  // It's length is `codepointsCount`.
  stbtt_fontinfo    info     = {};

  int* codepoints      = {};
  int  codepointsCount = {};
  int  outlineWidth    = {};
  f32  outlineAdvance  = {};

  f32 _scaleToFit = 1;
};

struct DrawTextData {
  Vector2     pos        = {};
  Vector2     scale      = {1, 1};
  Vector2     anchor     = Vector2Half();
  const Font* font       = {};
  const char* text       = {};
  int         bytesCount = {};
  Color       color      = WHITE;
  Color       flash      = TRANSPARENT_WHITE;
  // TODO: Color outlineColor = BLACK + shader.
};

enum DrawCommandType {
  DrawCommandType_INVALID,
  DrawCommandType_TEXTURE,
  DrawCommandType_TEXTURE_NINE_SLICE,
  DrawCommandType_CIRCLE,
  DrawCommandType_RECT,
  DrawCommandType_STRIPS,
  DrawCommandType_CIRCLE_LINES,
  DrawCommandType_RECT_LINES,
  DrawCommandType_TEXT,
};

struct DrawGroup {
  DrawZ z                  = DrawZ_DEFAULT;
  f32   sortY              = f32_inf;
  int   commandsCount      = 0;
  int   commandsStartIndex = -1;
};

struct DrawCommand {
  DrawCommandType type = DrawCommandType_INVALID;

  union {
    DrawTextureData          texture;
    DrawTextureNineSliceData textureNineSlice;
    DrawCircleData           circle;
    DrawRectData             rect;
    DrawTextData             text;
  } _u;

  auto& DataTexture() const {  ///
    ASSERT(type == DrawCommandType_TEXTURE);
    return _u.texture;
  }

  auto& DataTextureNineSlice() const {  ///
    ASSERT(type == DrawCommandType_TEXTURE_NINE_SLICE);
    return _u.textureNineSlice;
  }

  auto& DataCircle() const {  ///
    auto allowed
      = (type == DrawCommandType_CIRCLE) || (type == DrawCommandType_CIRCLE_LINES);
    ASSERT(allowed);
    return _u.circle;
  }

  auto& DataRect() const {  ///
    auto allowed = (type == DrawCommandType_RECT) || (type == DrawCommandType_RECT_LINES);
    ASSERT(allowed);
    return _u.rect;
  }

  auto& DataText() const {  ///
    ASSERT(type == DrawCommandType_TEXT);
    return _u.text;
  }
};

struct Camera {
  Vector2 pos           = {};
  f32     zoom          = 1;
  f32     texturesScale = 1;
};

enum SavedataLoadingType {
  SavedataLoadingType_NOT_LOADED,
  SavedataLoadingType_JUST_FISNIHED,
  SavedataLoadingType_FISNIHED,
};

enum LastEventType {
  LastEventType_NOT_SET,
  LastEventType_TOUCH,
  LastEventType_MOUSE,
  LastEventType_KEY,
  LastEventType_CONTROLLER,
};

struct _ReceivedEvents {  ///
  bool mousePressed  = false;
  bool mouseReleased = false;
  bool mouseWheeled  = false;
  bool mouseMoved    = false;

  bool touchPressed  = false;
  bool touchReleased = false;
  bool touchMoved    = false;

  bool keyPressed  = false;
  bool keyReleased = false;

  bool Mouse() const {
    return mousePressed || mouseReleased || mouseWheeled || mouseMoved;
  }

  bool Touch() const {
    return touchPressed || touchReleased || touchMoved;
  }

  bool Key() const {
    return keyPressed || keyReleased;
  }
};

struct lframe {  ///
  i64 value = i64_max;

  constexpr static lframe Scaled(i64 v) {
    return {.value = v * _BF_LOGICAL_FPS_SCALE};
  }

  constexpr static lframe Unscaled(i64 v) {
    return {.value = v};
  }

  constexpr static lframe FromSeconds(f32 seconds) {
    return {.value = (i64)(seconds * (f32)FIXED_FPS)};
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
    return lframe::Unscaled(value + other.value);
  }

  const lframe operator-(const lframe& other) const {
    return lframe::Unscaled(value - other.value);
  }

  void SetRand(lframe v);
  void SetRand(lframe v1, lframe v2);
  void SetRandSeconds(f32 v);
  void SetRandSeconds(f32 v1, f32 v2);
};

struct FrameGame {  ///
  i64 _value = i64_max;

  [[nodiscard]] static FrameGame Now() {
    FrameGame frame{};
    frame.SetNow();
    return frame;
  }

  bool IsSet() const {
    return _value != i64_max;
  }

  void SetNow();

  lframe Elapsed() const;
};

struct FrameVisual {  ///
  i64 _value = i64_max;

  [[nodiscard]] static FrameVisual Now() {
    FrameVisual frame{};
    frame.SetNow();
    return frame;
  }

  bool IsSet() const {
    return _value != i64_max;
  }

  void SetNow();

  lframe Elapsed() const;
};

struct _SoundVariation;
int _CmpSoundVariation(  ///
  const _SoundVariation* v1,
  const _SoundVariation* v2
);

struct _SoundVariation {  ///
  const char* filepath       = {};
  ma_sound    ma_sound       = {};
  u32         soundHashValue = {};
  int         variation      = {};
  u32         flags          = {};

  bool operator==(const _SoundVariation& other) const {
    return _CmpSoundVariation(this, &other) == 0;
  }

  bool operator<=(const _SoundVariation& other) const {
    return _CmpSoundVariation(this, &other) != 1;
  }
};

int _CmpSoundVariation(const _SoundVariation* v1,
                       const _SoundVariation* v2) {  ///
  if (v1->soundHashValue > v2->soundHashValue)
    return 1;
  if (v1->soundHashValue < v2->soundHashValue)
    return -1;
  if (v1->variation > v2->variation)
    return 1;
  if (v1->variation < v2->variation)
    return -1;
  return 0;
}

struct _SoundVariationRange {  ///
  int start = -1;
  int end   = -1;  // Sentinel. After the last element.
};

struct _LaunchedSound {  ///
  int         index           = {};
  int         loadedFileIndex = -1;
  int         boostCount      = 0;
  FrameVisual frame           = {};
};

struct _EndedSound {  ///
  ma_sound* ma_sound        = {};
  int       loadedFileIndex = {};
};

struct PlayingSound {  ///
  int index      = {};
  int generation = {};
};

struct EngineData {
  struct Meta {
    i64 frameGame   = 0;
    i64 frameVisual = 0;

    f32 imguiScale = 1.5f;

    Texture2D           atlas                 = {};
    Vector2Int          atlasSize             = {};
    bgfx::ProgramHandle programDefaultTexture = {};
    bgfx::ProgramHandle programDefaultQuad    = {};
    bgfx::UniformHandle uniformTexture        = {};

    Vector2Int screenSize = {};

    f32 screenToLogicalRatio = {};
    f32 screenScale          = {};

    View<const bool> _keyboardState         = {};
    bool*            _keyboardStatePressed  = {};
    bool*            _keyboardStateReleased = {};

    u32     _mouseState            = {};
    u32     _mouseStatePressed     = {};
    u32     _mouseStateReleased    = {};
    Vector2 _mousePos              = {};
    Vector2 _mouseOrLatestTouchPos = {};
    Vector2 _latestTouchPos        = {};
    int     _mouseWheel            = {};

    Vector<TouchID>    _touchIDs            = {};
    Vector<_TouchData> _touches             = {};
    TouchID            _latestActiveTouchID = {};

    i64 ticks         = {};
    f64 prevFrameTime = {};
    f64 frameTime     = {};

    Arena _arena = {};

    DeviceType device = DeviceType_DESKTOP;

    Vector2 _screenToLogicalScale = {};
    Vector2 _screenToLogicalAdd   = {};

    int localization = 1;  // 0 - ru. 1 - en.

    struct SoundManager {
      ma_sound_group groupSFX   = {};
      ma_sound_group groupMusic = {};

      ReaderWriterQueue<_EndedSound> soundsToUninitialize{};
      Vector<_SoundVariation>        soundVariationsLoadedFromFiles = {};
      Vector<_SoundVariationRange>   soundVariationRanges           = {};

      bool works = false;
      union {
        ma_engine     engine;
        ma_node_graph engineg;
      };

      f32                            volume                        = 0.75f;
      Array<ma_sound, BF_MAX_SOUNDS> playingSounds                 = {};
      ma_sound*                      playingMusic                  = nullptr;
      Array<int, BF_MAX_SOUNDS>      playingSoundsBoolsGenerations = {};
      int                            playingSoundsCount            = 0;

      ma_lpf_node musicLpf = {};

      Vector<_LaunchedSound> launchedSounds = {};
    } _soundManager = {};

    bool ysdkLoaded       = false;
    bool windowIsInactive = false;
    bool quitRequested    = false;
    bool quitScheduled    = false;

    bool debugEnabled = false;

    Random logicRand{0};
    Random visualRand{0};

    const Camera* _currentCamera = nullptr;

    bool previousSaveIsNotCompletedYet = false;

    SavedataLoadingType loading  = {};
    const u8*           savedata = {};

    // 0 - not loaded yet, 1 - savedata is empty (null), 2 - got savedata.
    int jsLoadedSavedata = 0;

    bool _drawing = false;
  } meta;

  struct {
    _ReceivedEvents prevFrame = {};
    _ReceivedEvents thisFrame = {};
    LastEventType   last      = {};
  } events;

  struct Settings {
    Color     backgroundColor = BLACK;
    Color     screenFadeColor = BLACK;
    f32       screenFade      = 0;
    View<u64> bgfxDisabledCapabilities{};

    struct {
      bool                     setup      = {};
      const char*              build      = {};
      const char*              gameKey    = {};
      const char*              gameSecret = {};
      std::vector<std::string> currencies = {};
      std::vector<std::string> resources  = {};
    } gameanalytics;

    size_t additionalArenaSize = 0;
  } settings;

  struct Draw {
    Vector<DrawCommand> commands          = {};
    Vector<DrawGroup>   groups            = {};
    int                 currentGroupIndex = -1;
    bool                flushedThisFrame  = false;
  } draw;
} ge = {};

#define GRAND (ge.meta.logicRand)
#define VRAND (ge.meta.visualRand)

void lframe::SetRand(lframe v) {  ///
  value = ge.meta.logicRand.Rand() % v.value;
}

void lframe::SetRand(lframe v1, lframe v2) {  ///
  ASSERT(v2.value >= v1.value);
  value = v1.value + ge.meta.logicRand.Rand() % (v2.value - v1.value);
}

void lframe::SetRandSeconds(f32 v) {  ///
  value = (f32)FIXED_FPS * v * GRAND.FRand();
}

void lframe::SetRandSeconds(f32 v1, f32 v2) {  ///
  ASSERT(v2 >= v1);
  value = Round((f32)FIXED_FPS * Lerp(v1, v2, GRAND.FRand()));
}

Vector2 ScreenPosToLogical(Vector2 pos) {  ///
  return pos * ge.meta._screenToLogicalScale + ge.meta._screenToLogicalAdd;
}

Vector2 LogicalPosToScreen(Vector2 pos) {  ///
  return (pos - ge.meta._screenToLogicalAdd) / ge.meta._screenToLogicalScale;
}

Vector2 GetRandomPosInside(Random* random, const Rect* rect) {  ///
  Vector2 result{};

  f32 t{};

  do {
    t = random->FRand();
  } while (t == 0);
  result.x = rect->pos.x + t * rect->size.x;

  do {
    t = random->FRand();
  } while (t == 0);

  result.y = rect->pos.y + t * rect->size.y;

  return result;
}

// Points are contained inside. Not on edge.
Vector2 Rect::GetRandomGamePosInside() const {  ///
  return GetRandomPosInside(&GRAND, this);
}

// Points are contained inside. Not on edge.
Vector2 Rect::GetRandomVisualPosInside() const {  ///
  return GetRandomPosInside(&VRAND, this);
}

void BeginMode2D(const Camera* camera) {
  ASSERT(!ge.meta._currentCamera);
  ge.meta._currentCamera = camera;
}

void EndMode2D() {
  ASSERT(ge.meta._currentCamera);
  ge.meta._currentCamera = nullptr;
}

// clang-format off
#ifdef BF_PLATFORM_WebYandex
EM_JS(void, js_YandexReady, (), {
  window.ysdk.features.LoadingAPI.ready();
});
#endif
// clang-format on

void GameReady() {  ///
#ifdef BF_PLATFORM_WebYandex
  js_YandexReady();
#endif
}

void _OnSoundEnd(void* userData, ma_sound* sound) {  ///
  ASSERT(sound);

  auto& m = ge.meta._soundManager;
  ASSERT(m.works);

  const auto loadedFileIndex = *(size_t*)&userData;

  m.soundsToUninitialize.enqueue({
    .ma_sound        = sound,
    .loadedFileIndex = (int)loadedFileIndex,
  });
}

struct PlaySoundData {
  u64 delayMilliseconds = 0;
};

void SetVolumeMaster(f32 v) {  ///
  ASSERT(v >= 0);
  ASSERT(v <= 1);
  auto& m  = ge.meta._soundManager;
  m.volume = Clamp01(v);
  ma_engine_set_volume(&m.engine, m.volume);
}

void SetVolumeSFX(f32 v) {  ///
  ASSERT(v >= 0);
  ASSERT(v <= 1);
  ma_sound_set_volume(&ge.meta._soundManager.groupSFX, Clamp01(v));
}

void SetVolumeMusic(f32 v) {  ///
  ASSERT(v >= 0);
  ASSERT(v <= 1);
  ma_sound_set_volume(&ge.meta._soundManager.groupMusic, Clamp01(v));
}

// TODO: Options struct to support
// * increasing volume of recently started sounds
// * variations
// * ma_sound_set_stop_time_in_pcm_frames()
// * ma_sound_set_stop_time_in_milliseconds()
PlayingSound PlaySound(u32 soundHashValue, PlaySoundData data = {}) {  ///
  static int nextSoundGeneration = 1;

  if (!soundHashValue)
    return {};

  static f32 BF_BOOST_VOLUME_MAX = 0;
  if (BF_BOOST_VOLUME_MAX == 0)
    BF_BOOST_VOLUME_MAX = ma_volume_db_to_linear(-3) / ma_volume_db_to_linear(-6);

  ASSERT_FALSE(ge.meta._drawing);
  auto& m = ge.meta._soundManager;

  if (!m.works)
    return {};

  const auto fb_sounds = glib->sounds();
  const auto fb_sound  = fb_sounds->LookupByKey(soundHashValue);
  ASSERT(fb_sound);

  if (fb_sound->is_music())
    ASSERT_FALSE(ma_sound_is_playing(m.playingMusic));

  const int soundIndex = fb_sound->index();
  ASSERT(soundIndex >= 0);
  ASSERT(soundIndex < fb_sounds->size());

  // Playing the same sound during the next short time doesn't spawn a new sound.
  // We're boosting previously launched one.
  auto& launchedSound = m.launchedSounds[soundIndex];

  if (launchedSound.frame.IsSet()
      && (launchedSound.frame.Elapsed().value < BF_SOUND_VOLUME_BOOST_MAX_LATENCY_FRAMES))
  {
    launchedSound.frame = {};
    launchedSound.frame.SetNow();

    auto s = m.playingSounds.base + launchedSound.index;
    if (launchedSound.boostCount < BF_SOUND_VOLUME_BOOST_STEPS)
      launchedSound.boostCount++;
    const f32 p
      = (f32)MIN(1, launchedSound.boostCount) / (f32)BF_SOUND_VOLUME_BOOST_STEPS;
    ma_sound_set_volume(s, Lerp(1.0f, BF_BOOST_VOLUME_MAX, p));

    return {};
  }

  if (!fb_sound->is_music() && (m.playingSoundsCount >= m.playingSounds.count))
    return {};

  const auto& variationRange = m.soundVariationRanges[soundIndex];
  if (variationRange.start == -1)
    return {};

  ASSERT(variationRange.start >= 0);
  ASSERT(variationRange.end > variationRange.start);

  int loadedFileIndex = launchedSound.loadedFileIndex;
  ASSERT(loadedFileIndex >= -1);
  if (variationRange.start != variationRange.end - 1) {
    while (loadedFileIndex == launchedSound.loadedFileIndex)
      loadedFileIndex = VRAND.RandInt(variationRange.start, variationRange.end - 1);
  }
  if (loadedFileIndex <= -1)
    loadedFileIndex = variationRange.start;
  ASSERT(loadedFileIndex >= 0);
  const int variationIndex = loadedFileIndex - variationRange.start;
  ASSERT(variationIndex >= 0);
  auto& original = m.soundVariationsLoadedFromFiles[loadedFileIndex];

  ma_sound* s = nullptr;

  PlayingSound result{};

  if (fb_sound->is_music()) {
    s                             = &original.ma_sound;
    launchedSound.loadedFileIndex = loadedFileIndex;
  }
  else {
    // TODO:
    // * freelist + pool allocator?
    FOR_RANGE (int, i, m.playingSoundsBoolsGenerations.count) {
      auto& v = m.playingSoundsBoolsGenerations[i];
      if (v)
        continue;
      s = m.playingSounds.base + i;

      launchedSound = {
        .index           = i,
        .loadedFileIndex = loadedFileIndex,
      };
      launchedSound.frame.SetNow();

      v      = nextSoundGeneration++;
      result = {
        .index      = i,
        .generation = v,
      };
      break;
    }
    if (!s) {
      INVALID_PATH;
      return {};
    }

    if (ma_sound_init_copy(
          &m.engine,
          &original.ma_sound,
          original.flags | MA_SOUND_FLAG_WAIT_INIT,
          &m.groupSFX,
          s
        )
        != MA_SUCCESS)
    {
      INVALID_PATH;
      return {};
    }
  }

  ASSERT_FALSE(ma_sound_is_playing(s));

  static_assert(sizeof(void*) == sizeof(size_t));
  const auto soundEndUserData = (size_t)loadedFileIndex;

  if (ma_sound_set_end_callback(s, _OnSoundEnd, *(void**)&soundEndUserData) != MA_SUCCESS)
  {
    INVALID_PATH;
    return {};
  }

  if (fb_sound->pitch_min() != 1) {
    ASSERT(fb_sound->pitch_min() <= fb_sound->pitch_max());
    ma_sound_set_pitch(
      s, Lerp(fb_sound->pitch_min(), fb_sound->pitch_max(), VRAND.FRand11())
    );
  }

  auto fb_variation = fb_sound->variations()->Get(variationIndex);

  ASSERT_FALSE(strcmp(
    fb_variation->filepath()->c_str(),
    m.soundVariationsLoadedFromFiles[loadedFileIndex].filepath
  ));

  const u64 delayMilliseconds
    = fb_variation->delay_milliseconds() + data.delayMilliseconds;
  if (delayMilliseconds) {
    ma_sound_set_start_time_in_milliseconds(
      s, ma_engine_get_time_in_milliseconds(&m.engine) + delayMilliseconds
    );
  }

  if (ma_sound_start(s) == MA_SUCCESS) {
    if (fb_sound->is_music()) {
      ASSERT_FALSE(m.playingMusic);
      m.playingMusic = s;
    }
  }
  else {
    INVALID_PATH;
    return {};
  }

  if (!fb_sound->is_music())
    m.playingSoundsCount++;

  return result;
}

void StopSound(PlayingSound sound) {  ///
  auto& m = ge.meta._soundManager;
  if (!sound.generation)
    return;

  if (m.playingSoundsBoolsGenerations[sound.index] != sound.generation)
    return;

  ma_sound_stop(&m.playingSounds[sound.index]);
}

void PlaySound(Sound sound) {  ///
  PlaySound(SOUND_TO_HASH_VALUE[sound]);
}

BF_FORCE_INLINE void DrawGroup_Begin(DrawZ z) {  ///
  ASSERT(ge.meta._drawing);

  ASSERT(ge.draw.currentGroupIndex == -1);
  ge.draw.currentGroupIndex = ge.draw.groups.count;
  *ge.draw.groups.Add()     = {.z = z, .commandsStartIndex = ge.draw.commands.count};
}

BF_FORCE_INLINE void DrawGroup_SetSortY(f32 value) {  ///
  auto& group = ge.draw.groups[ge.draw.currentGroupIndex];

  if (group.sortY == f32_inf)
    group.sortY = value;
  else
    INVALID_PATH;
}

Vector2 WorldPosToLogical(Vector2 pos, Camera* camera) {  ///
  ASSERT(camera);
  ASSERT(camera->zoom > 0);
  pos -= camera->pos;
  pos *= camera->zoom;
  pos += (Vector2)LOGICAL_RESOLUTION / 2.0f;
  return pos;
}

void _ApplyCurrentCamera(Vector2* point, Vector2* size, bool isTexture = false) {  ///
  if (ge.meta._currentCamera) {
    ASSERT(ge.meta._currentCamera->zoom > 0);
    ASSERT(ge.meta._currentCamera->texturesScale > 0);

    *point -= ge.meta._currentCamera->pos;
    *point *= ge.meta._currentCamera->zoom;
    *point += (Vector2)LOGICAL_RESOLUTION / 2.0f;
    if (size) {
      *size *= ge.meta._currentCamera->zoom;
      if (isTexture)
        *size *= ge.meta._currentCamera->texturesScale;
    }
  }
}

void _ApplyCurrentCamera(Vector2* point, f32* size, bool isTexture = false) {  ///
  if (ge.meta._currentCamera) {
    ASSERT(ge.meta._currentCamera->zoom > 0);
    ASSERT(ge.meta._currentCamera->texturesScale > 0);

    *point -= ge.meta._currentCamera->pos;
    *point *= ge.meta._currentCamera->zoom;
    *point += (Vector2)LOGICAL_RESOLUTION / 2.0f;
    if (size) {
      *size *= ge.meta._currentCamera->zoom;
      if (isTexture)
        *size *= ge.meta._currentCamera->texturesScale;
    }
  }
}

enum DrawCommandSetSortY {  ///
  DrawCommandSetSortY_DO_NOTHING,
  DrawCommandSetSortY_SET_BASELINE,
};

BF_FORCE_INLINE void DrawGroup_CommandTexture(
  DrawTextureData     data,
  DrawCommandSetSortY setSortY = DrawCommandSetSortY_DO_NOTHING
) {  ///
  _ApplyCurrentCamera(&data.pos, &data.scale, true);

  ASSERT(data.sourceMargins.left >= 0);
  ASSERT(data.sourceMargins.right >= 0);
  ASSERT(data.sourceMargins.top >= 0);
  ASSERT(data.sourceMargins.bottom >= 0);
  ASSERT(data.sourceMargins.left <= 1);
  ASSERT(data.sourceMargins.right <= 1);
  ASSERT(data.sourceMargins.top <= 1);
  ASSERT(data.sourceMargins.bottom <= 1);

  auto mx = data.sourceMargins.left + data.sourceMargins.right;
  auto my = data.sourceMargins.bottom + data.sourceMargins.top;
  ASSERT(mx >= 0);
  ASSERT(my >= 0);

  if ((mx >= 1) || (my >= 1))
    return;

  if (setSortY == DrawCommandSetSortY_SET_BASELINE) {
    auto tex    = glib->atlas_textures()->Get(data.texID);
    auto height = (f32)tex->size_y();
    auto off    = glib->atlas_downscale_factor() * ASSETS_TO_LOGICAL_RATIO * data.scale.y
               * ((f32)tex->baseline() - (f32)tex->size_y() * data.anchor.y);
    DrawGroup_SetSortY(data.pos.y + off);
  }

  ge.draw.groups[ge.draw.currentGroupIndex].commandsCount++;
  *ge.draw.commands.Add() = {
    .type = DrawCommandType_TEXTURE,
    ._u{.texture = data},
  };
}

BF_FORCE_INLINE void DrawGroup_CommandTextureNineSlice(
  DrawTextureNineSliceData data,
  DrawCommandSetSortY      setSortY = DrawCommandSetSortY_DO_NOTHING
) {  ///
  _ApplyCurrentCamera(&data.pos, &data.scale, true);

  ASSERT(data.nineSliceMargins.left > 0);
  ASSERT(data.nineSliceMargins.right > 0);
  ASSERT(data.nineSliceMargins.top > 0);
  ASSERT(data.nineSliceMargins.bottom > 0);

  if ((data.nineSliceSize.x == 0) || (data.nineSliceSize.y == 0))
    return;

  if (setSortY == DrawCommandSetSortY_SET_BASELINE) {
    auto tex    = glib->atlas_textures()->Get(data.texID);
    auto height = (f32)tex->size_y();
    auto off    = glib->atlas_downscale_factor() * ASSETS_TO_LOGICAL_RATIO * data.scale.y
               * ((f32)tex->baseline() - (f32)tex->size_y() * data.anchor.y);
    DrawGroup_SetSortY(data.pos.y + off);
  }

  ge.draw.groups[ge.draw.currentGroupIndex].commandsCount++;
  *ge.draw.commands.Add() = {
    .type = DrawCommandType_TEXTURE_NINE_SLICE,
    ._u{.textureNineSlice = data},
  };
}

BF_FORCE_INLINE void DrawGroup_CommandCircle(
  DrawCircleData      data,
  DrawCommandSetSortY setSortY = DrawCommandSetSortY_DO_NOTHING
) {  ///
  ASSERT(data.radius >= 0);
  if (data.radius <= 0)
    return;

  _ApplyCurrentCamera(&data.pos, &data.radius);

  if (setSortY == DrawCommandSetSortY_SET_BASELINE)
    DrawGroup_SetSortY(data.pos.y - data.radius);

  ge.draw.groups[ge.draw.currentGroupIndex].commandsCount++;
  *ge.draw.commands.Add() = {
    .type = DrawCommandType_CIRCLE,
    ._u{.circle = data},
  };
}

BF_FORCE_INLINE void DrawGroup_CommandRect(
  DrawRectData        data,
  DrawCommandSetSortY setSortY = DrawCommandSetSortY_DO_NOTHING
) {  ///
  ASSERT(data.size.x >= 0);
  ASSERT(data.size.y >= 0);
  if ((data.size.x == 0) || (data.size.y == 0))
    return;

  _ApplyCurrentCamera(&data.pos, &data.size);

  if (setSortY == DrawCommandSetSortY_SET_BASELINE)
    DrawGroup_SetSortY(data.pos.y - data.size.y * (data.anchor.y - 0.5f));

  ge.draw.groups[ge.draw.currentGroupIndex].commandsCount++;
  *ge.draw.commands.Add() = {
    .type = DrawCommandType_RECT,
    ._u{.rect = data},
  };
}

BF_FORCE_INLINE void DrawGroup_CommandCircleLines(DrawCircleData data) {  ///
  ASSERT(data.radius >= 0);
  if (data.radius <= 0)
    return;

  _ApplyCurrentCamera(&data.pos, &data.radius);

  ge.draw.groups[ge.draw.currentGroupIndex].commandsCount++;
  *ge.draw.commands.Add() = {
    .type = DrawCommandType_CIRCLE_LINES,
    ._u{.circle = data},
  };
}

BF_FORCE_INLINE void DrawGroup_CommandRectLines(DrawRectData data) {  ///
  ASSERT(data.size.x >= 0);
  ASSERT(data.size.y >= 0);
  if ((data.size.x == 0) || (data.size.y == 0))
    return;

  _ApplyCurrentCamera(&data.pos, &data.size);

  ge.draw.groups[ge.draw.currentGroupIndex].commandsCount++;
  *ge.draw.commands.Add() = {
    .type = DrawCommandType_RECT_LINES,
    ._u{.rect = data},
  };
}

BF_FORCE_INLINE void DrawGroup_CommandText(DrawTextData data) {  ///
  if (FloatEquals(data.scale.x, 0) || FloatEquals(data.scale.y, 0))
    return;

  _ApplyCurrentCamera(&data.pos, (f32*)nullptr);

  ge.draw.groups[ge.draw.currentGroupIndex].commandsCount++;
  *ge.draw.commands.Add() = {
    .type = DrawCommandType_TEXT,
    ._u{.text = data},
  };
}

BF_FORCE_INLINE void DrawGroup_End() {  ///
  const auto& group         = ge.draw.groups[ge.draw.currentGroupIndex];
  ge.draw.currentGroupIndex = -1;

  if (!group.commandsCount) {
    ge.draw.groups.count--;
    return;
  }

  ASSERT(group.sortY != f32_inf);
}

BF_FORCE_INLINE void DrawGroup_OneShotTexture(DrawTextureData data, DrawZ z) {  ///
  DrawGroup_Begin(z);
  DrawGroup_CommandTexture(data, DrawCommandSetSortY_SET_BASELINE);
  DrawGroup_End();
}

BF_FORCE_INLINE void DrawGroup_OneShotCircle(DrawCircleData data, DrawZ z) {  ///
  DrawGroup_Begin(z);
  DrawGroup_CommandCircle(data, DrawCommandSetSortY_SET_BASELINE);
  DrawGroup_End();
}

BF_FORCE_INLINE void DrawGroup_OneShotCircleLines(DrawCircleData data) {  ///
  DrawGroup_Begin(DrawZ_GIZMOS);
  DrawGroup_SetSortY(0);
  DrawGroup_CommandCircleLines(data);
  DrawGroup_End();
}

BF_FORCE_INLINE void DrawGroup_OneShotRect(DrawRectData data, DrawZ z) {  ///
  DrawGroup_Begin(z);
  DrawGroup_CommandRect(data, DrawCommandSetSortY_SET_BASELINE);
  DrawGroup_End();
}

BF_FORCE_INLINE void DrawGroup_OneShotRectLines(DrawRectData data) {  ///
  DrawGroup_Begin(DrawZ_GIZMOS);
  DrawGroup_SetSortY(0);
  DrawGroup_CommandRectLines(data);
  DrawGroup_End();
}

// NOTE: Ignores SortY!
BF_FORCE_INLINE void DrawGroup_OneShotText(DrawTextData data, DrawZ z) {  ///
  ASSERT(data.bytesCount > 0);
  DrawGroup_Begin(z);
  DrawGroup_SetSortY(0);
  DrawGroup_CommandText(data);
  DrawGroup_End();
}

BF_FORCE_INLINE int _DrawGroupCmp(
  const DrawGroup* BF_RESTRICT v1,
  const DrawGroup* BF_RESTRICT v2
) {  ///
  ASSERT(v1->z > DrawZ_INVALID);
  ASSERT(v2->z > DrawZ_INVALID);
  ASSERT(v1->z < DrawZ_COUNT);
  ASSERT(v2->z < DrawZ_COUNT);

  const auto y1 = v1->sortY;
  const auto y2 = v2->sortY;

  auto cmp = 0;
  if (v1->z > v2->z)
    cmp = 1;
  else if (v1->z < v2->z)
    cmp = -1;
  else if (y1 > y2)
    cmp = -1;
  else if (y1 < y2)
    cmp = 1;

  if (cmp == 0) {
    const auto& cmd1 = ge.draw.commands[v1->commandsStartIndex];
    const auto& cmd2 = ge.draw.commands[v2->commandsStartIndex];
    if ((cmd1.type == DrawCommandType_TEXTURE)  //
        && (cmd2.type == DrawCommandType_TEXTURE))
      cmp = (cmd1.DataTexture().pos.x > cmd2.DataTexture().pos.x ? 1 : -1);
    else if (cmd1.type == DrawCommandType_TEXTURE)
      cmp = 1;
    else if (cmd2.type == DrawCommandType_TEXTURE)
      cmp = -1;
  }

  return cmp;
}

#define SORT_NAME draw_group
#define SORT_TYPE DrawGroup
#define SORT_CMP(x, y) (_DrawGroupCmp(&x, &y))
#include "sort.h"

//
void FlushDrawCommands() {
  ZoneScoped;

  ASSERT(ge.meta._drawing);
  ASSERT_FALSE(ge.draw.flushedThisFrame);
  ge.draw.flushedThisFrame = true;

  // Setup. {  ///
  ASSERT(ge.draw.currentGroupIndex == -1);

  draw_group_tim_sort(ge.draw.groups.base, ge.draw.groups.count);

  Shader shaders_[]{
    {
      .program              = ge.meta.programDefaultTexture,
      .vertexLayout         = _PosColorFlashTexVertex::layout,
      .additionalStateFlags = 0,
    },
    {
      .program              = ge.meta.programDefaultQuad,
      .vertexLayout         = _PosColorVertex::layout,
      .additionalStateFlags = 0,
    },
    {
      .program              = ge.meta.programDefaultQuad,
      .vertexLayout         = _PosColorVertex::layout,
      .additionalStateFlags = BGFX_STATE_PT_LINES,
    },
    {
      .program              = ge.meta.programDefaultTexture,
      .vertexLayout         = _PosColorFlashTexVertex::layout,
      .additionalStateFlags = 0,
    },
  };
  VIEW_FROM_ARRAY_DANGER(shaders);

  u16 drawCallIndicesCounts_[512];
  int drawCallVerticesCounts_[512];
  VIEW_FROM_ARRAY_DANGER(drawCallIndicesCounts);
  VIEW_FROM_ARRAY_DANGER(drawCallVerticesCounts);

  bgfx::TransientVertexBuffer tvb{};
  bgfx::TransientIndexBuffer  tib{};
  // }

  const auto logicalToScreenScale = ScaleToFit(LOGICAL_RESOLUTION, ge.meta.screenSize);

  FOR_RANGE (int, mode, 2) {
    // 0 - calculating size of index / vertex buffers.
    // 1 - allocating and filling them.

    int  drawCallIndicesCount  = 0;
    int  drawCallVerticesCount = 0;
    bool addedSomethingToDraw  = false;
    int  drawCallIndex         = 0;

    const Font*   font   = nullptr;
    const Shader* shader = nullptr;

    LAMBDA (void, endShader, (int mode)) {  ///
      if (!mode) {
        drawCallIndicesCounts[drawCallIndex]  = drawCallIndicesCount;
        drawCallVerticesCounts[drawCallIndex] = drawCallVerticesCount;
      }
      else {
        ASSERT(drawCallIndicesCounts[drawCallIndex] == drawCallIndicesCount);
        ASSERT(drawCallVerticesCounts[drawCallIndex] == drawCallVerticesCount);

        if (font)
          bgfx::setTexture(0, ge.meta.uniformTexture, font->atlasTexture.handle);
        else {
          // TODO: don't bind for shaders that don't need it.
          bgfx::setTexture(0, ge.meta.uniformTexture, ge.meta.atlas.handle);
        }

        bgfx::setVertexBuffer(0, &tvb);
        bgfx::setIndexBuffer(&tib);
        bgfx::setState(
          BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_ALPHA | shader->additionalStateFlags
        );
        bgfx::submit(0, shader->program);

        tvb = {};
        tib = {};
      }
    };

    for (auto& group : ge.draw.groups) {
      ASSERT(group.z < DrawZ_COUNT);
      ASSERT(group.commandsStartIndex >= 0);
      ASSERT(group.commandsCount > 0);

      FOR_RANGE (int, commandIndex, group.commandsCount) {
        // Setup. {  ///
        const auto& command = ge.draw.commands[group.commandsStartIndex + commandIndex];

        const Font*   newFont   = nullptr;
        const Shader* newShader = nullptr;

        if ((command.type == DrawCommandType_TEXTURE)
            || (command.type == DrawCommandType_TEXTURE_NINE_SLICE))
          newShader = &shaders[0];
        else if (command.type == DrawCommandType_RECT)
          newShader = &shaders[1];
        else if (command.type == DrawCommandType_STRIPS) {
          if (ge.meta.screenToLogicalRatio == 1)
            continue;
          newShader = &shaders[1];
        }
        else if (command.type == DrawCommandType_CIRCLE_LINES)
          newShader = &shaders[2];
        else if (command.type == DrawCommandType_RECT_LINES)
          newShader = &shaders[2];
        else if (command.type == DrawCommandType_TEXT) {
          newShader = &shaders[3];
          newFont   = command.DataText().font;
        }
        else
          INVALID_PATH;

        if ((shader != newShader) || (font != newFont)) {
          if (addedSomethingToDraw) {
            endShader(mode);
            drawCallIndicesCount  = 0;
            drawCallVerticesCount = 0;
            drawCallIndex++;
          }

          shader = newShader;
          font   = newFont;

          if (mode == 1) {
            bgfx::allocTransientVertexBuffer(
              &tvb, drawCallVerticesCounts[drawCallIndex], shader->vertexLayout
            );
            bgfx::allocTransientIndexBuffer(&tib, drawCallIndicesCounts[drawCallIndex]);
          }
        }
        // }

        switch (command.type) {
        case DrawCommandType_INVALID: {  ///
          INVALID_PATH;
        } break;

        case DrawCommandType_TEXTURE: {  ///
          if (!mode) {
            drawCallIndicesCount += 6;
            drawCallVerticesCount += 4;
          }
          else {
            const auto& data = command.DataTexture();
            ASSERT(data.texID >= 0);

            const auto tex = glib->atlas_textures()->Get(data.texID);

            const Rect sourceRec{
              .pos{
                (f32)tex->atlas_x() + (f32)tex->size_x() * data.sourceMargins.left,
                (f32)tex->atlas_y() + (f32)tex->size_y() * data.sourceMargins.bottom,
              },
              .size{
                (f32)tex->size_x()
                  * (1 - data.sourceMargins.right - data.sourceMargins.left),
                (f32)tex->size_y()
                  * (1 - data.sourceMargins.top - data.sourceMargins.bottom),
              },
            };
            Rect destRec{
              .pos{
                data.pos.x
                  + (f32)tex->size_x() * data.sourceMargins.left * abs(data.scale.x)
                      * ge.meta.screenScale * data.anchor.x,
                data.pos.y
                  + (f32)tex->size_y() * data.sourceMargins.bottom * abs(data.scale.y)
                      * ge.meta.screenScale * data.anchor.y,
              },
              .size{
                (f32)tex->size_x() * abs(data.scale.x) * ge.meta.screenScale,
                (f32)tex->size_y() * abs(data.scale.y) * ge.meta.screenScale,
              },
            };
            destRec.pos -= LOGICAL_RESOLUTION / 2;

            auto sx0 = sourceRec.pos.x;
            auto sx1 = sx0 + sourceRec.size.x;
            auto sy0 = sourceRec.pos.y;
            auto sy1 = sy0 + sourceRec.size.y;
            sx0 /= (f32)ge.meta.atlas.size.x;
            sx1 /= (f32)ge.meta.atlas.size.x;
            sy0 /= (f32)ge.meta.atlas.size.y;
            sy1 /= (f32)ge.meta.atlas.size.y;
            if (data.scale.x < 0) {
              auto t = sx0;
              sx0    = sx1;
              sx1    = t;
            }
            if (data.scale.y < 0) {
              auto t = sy0;
              sy0    = sy1;
              sy1    = t;
            }

            Vector2 topLeft{};
            Vector2 topRight{};
            Vector2 bottomLeft{};
            Vector2 bottomRight{};

            auto dsx
              = destRec.size.x * (1 - data.sourceMargins.left - data.sourceMargins.right);
            auto dsy
              = destRec.size.y * (1 - data.sourceMargins.bottom - data.sourceMargins.top);

            if (data.rotation == 0.0f) {
              auto dx0 = destRec.pos.x;
              auto dx1 = destRec.pos.x + dsx;
              auto dy0 = destRec.pos.y;
              auto dy1 = destRec.pos.y + dsy;

              dx0 -= data.anchor.x * destRec.size.x;
              dx1 -= data.anchor.x * destRec.size.x;
              dy0 -= data.anchor.y * destRec.size.y;
              dy1 -= data.anchor.y * destRec.size.y;

              topLeft     = {dx0, dy0};
              topRight    = {dx1, dy0};
              bottomLeft  = {dx0, dy1};
              bottomRight = {dx1, dy1};
            }
            else {
              auto sinRotation = sinf(data.rotation);
              auto cosRotation = cosf(data.rotation);

              auto dx = -data.anchor.x * destRec.size.x;
              auto dy = -data.anchor.y * destRec.size.y;

              topLeft.x = destRec.pos.x + dx * cosRotation - dy * sinRotation;
              topLeft.y = destRec.pos.y + dx * sinRotation + dy * cosRotation;

              topRight.x = destRec.pos.x + (dx + dsx) * cosRotation - dy * sinRotation;
              topRight.y = destRec.pos.y + (dx + dsx) * sinRotation + dy * cosRotation;

              bottomLeft.x = destRec.pos.x + dx * cosRotation - (dy + dsy) * sinRotation;
              bottomLeft.y = destRec.pos.y + dx * sinRotation + (dy + dsy) * cosRotation;

              bottomRight.x
                = destRec.pos.x + (dx + dsx) * cosRotation - (dy + dsy) * sinRotation;
              bottomRight.y
                = destRec.pos.y + (dx + dsx) * sinRotation + (dy + dsy) * cosRotation;
            }

            topLeft.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
            topRight.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
            bottomLeft.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
            bottomRight.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
            topLeft.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;
            topRight.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;
            bottomLeft.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;
            bottomRight.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;

            const auto r = ge.meta.screenToLogicalRatio;
            if (r >= 1) {  // Window is too wide.
              const auto c = 1 - 1 / r;
              topLeft.x -= topLeft.x * c;
              topRight.x -= topRight.x * c;
              bottomLeft.x -= bottomLeft.x * c;
              bottomRight.x -= bottomRight.x * c;
            }
            else {  // Window is too high.
              const auto c = 1 - r;
              topLeft.y -= topLeft.y * c;
              topRight.y -= topRight.y * c;
              bottomLeft.y -= bottomLeft.y * c;
              bottomRight.y -= bottomRight.y * c;
            }

            const int quadIndices[]{0, 1, 2, 1, 3, 2};
            for (const auto index : quadIndices) {
              ((u16*)tib.data)[drawCallIndicesCount++]
                = (u16)(index + drawCallVerticesCount);
            }

            const auto                    color = *(u32*)&data.color;
            const auto                    flash = *(u32*)&data.flash;
            const _PosColorFlashTexVertex quadVertices[]{
              {topLeft.x, topLeft.y, 0.0f, color, flash, sx0, sy1},
              {topRight.x, topRight.y, 0.0f, color, flash, sx1, sy1},
              {bottomLeft.x, bottomLeft.y, 0.0f, color, flash, sx0, sy0},
              {bottomRight.x, bottomRight.y, 0.0f, color, flash, sx1, sy0},
            };
            for (const auto& vertex : quadVertices)
              ((_PosColorFlashTexVertex*)tvb.data)[drawCallVerticesCount++] = vertex;
          }

          ASSERT_FALSE(drawCallIndicesCount % 6);
          ASSERT_FALSE(drawCallVerticesCount % 4);
        } break;

        case DrawCommandType_TEXTURE_NINE_SLICE: {  ///
          if (!mode) {
            drawCallIndicesCount += 6 * 9;
            drawCallVerticesCount += 16;
          }
          else {
            const auto& data = command.DataTextureNineSlice();
            ASSERT(data.texID >= 0);

            const auto tex = glib->atlas_textures()->Get(data.texID);

            Rect destRec{
              .pos{
                data.pos.x + data.nineSliceSize.x * abs(data.scale.x) * data.anchor.x,
                data.pos.y + data.nineSliceSize.y * abs(data.scale.y) * data.anchor.y,
              },
              .size{
                data.nineSliceSize.x * abs(data.scale.x),
                data.nineSliceSize.y * abs(data.scale.y),
              },
            };
            destRec.pos -= LOGICAL_RESOLUTION / 2;

            auto sx0 = (f32)tex->atlas_x();
            auto sx3 = sx0 + (f32)tex->size_x();
            auto sy0 = (f32)tex->atlas_y();
            auto sy3 = sy0 + (f32)tex->size_y();
            auto sx1 = sx0 + data.nineSliceMargins.left + 1;
            auto sx2 = sx3 - data.nineSliceMargins.right - 1;
            auto sy1 = sy0 + data.nineSliceMargins.top + 1;
            auto sy2 = sy3 - data.nineSliceMargins.bottom - 1;
            sx0 += 0.7f;
            sx3 -= 0.7f;
            sy0 += 0.7f;
            sy3 -= 0.7f;
            ASSERT(sx0 < sx1);
            ASSERT(sx1 < sx2);
            ASSERT(sx2 < sx3);
            ASSERT(sy0 < sy1);
            ASSERT(sy1 < sy2);
            ASSERT(sy2 < sy3);
            sx0 /= (f32)ge.meta.atlas.size.x;
            sx1 /= (f32)ge.meta.atlas.size.x;
            sx2 /= (f32)ge.meta.atlas.size.x;
            sx3 /= (f32)ge.meta.atlas.size.x;
            sy0 /= (f32)ge.meta.atlas.size.y;
            sy1 /= (f32)ge.meta.atlas.size.y;
            sy2 /= (f32)ge.meta.atlas.size.y;
            sy3 /= (f32)ge.meta.atlas.size.y;
            if (data.scale.x < 0) {
              auto t = sx0;
              sx0    = sx3;
              sx3    = t;
              t      = sx1;
              sx1    = sx2;
              sx2    = t;
            }
            if (data.scale.y < 0) {
              auto t = sy0;
              sy0    = sy3;
              sy3    = t;
              t      = sy1;
              sy1    = sy2;
              sy2    = t;
            }

            auto dsx = destRec.size.x;
            auto dsy = destRec.size.y;

            Array<Vector2, 16> points{};

            if (data.rotation == 0.0f) {
              auto dx0 = destRec.pos.x;
              auto dx3 = destRec.pos.x + dsx;
              auto dy0 = destRec.pos.y;
              auto dy3 = destRec.pos.y + dsy;

              dx0 -= data.anchor.x * dsx;
              dx3 -= data.anchor.x * dsx;
              dy0 -= data.anchor.y * dsy;
              dy3 -= data.anchor.y * dsy;

              const auto df = glib->atlas_downscale_factor();
              auto dx1 = dx0 + data.nineSliceMargins.left * ASSETS_TO_LOGICAL_RATIO * df;
              auto dx2 = dx3 - data.nineSliceMargins.right * ASSETS_TO_LOGICAL_RATIO * df;
              auto dy1
                = dy0 + data.nineSliceMargins.bottom * ASSETS_TO_LOGICAL_RATIO * df;
              auto dy2 = dy3 - data.nineSliceMargins.top * ASSETS_TO_LOGICAL_RATIO * df;

              f32 dx_[4]{dx0, dx1, dx2, dx3};
              f32 dy_[4]{dy0, dy1, dy2, dy3};
              FOR_RANGE (int, y, 4) {
                FOR_RANGE (int, x, 4) {
                  const auto t = y * 4 + x;
                  points[t]    = {dx_[x], dy_[y]};
                }
              }
            }
            else
              NOT_IMPLEMENTED;

            for (auto& point : points) {
              point.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
              point.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;
            }

            const auto r = ge.meta.screenToLogicalRatio;
            if (r >= 1) {  // Window is too wide.
              const auto c = 1 - 1 / r;
              for (auto& point : points)
                point.x -= point.x * c;
            }
            else {  // Window is too high.
              const auto c = 1 - r;
              for (auto& point : points)
                point.y -= point.y * c;
            }

            int            quadIndicesFilled = 0;
            Array<int, 54> quadIndices{};
            FOR_RANGE (int, y, 3) {
              FOR_RANGE (int, x, 3) {
                int baseIndices[]{0, 1, 4, 1, 5, 4};
                for (int index : baseIndices)
                  quadIndices[quadIndicesFilled++] = index + x + 4 * y;
              }
            }

            for (const auto index : quadIndices) {
              ((u16*)tib.data)[drawCallIndicesCount++]
                = (u16)(index + drawCallVerticesCount);
            }

            const auto color  = *(u32*)&data.color;
            const auto flash  = *(u32*)&data.flash;
            const f32  sx_[4] = {sx0, sx1, sx2, sx3};
            const f32  sy_[4] = {sy3, sy2, sy1, sy0};
            FOR_RANGE (int, y, 4) {
              FOR_RANGE (int, x, 4) {
                const auto t = y * 4 + x;
                ((_PosColorFlashTexVertex*)tvb.data)[drawCallVerticesCount++]
                  = {points[t].x, points[t].y, 0.0f, color, flash, sx_[x], sy_[y]};
              }
            }
          }
        } break;

        case DrawCommandType_CIRCLE: {  ///
          NOT_IMPLEMENTED;
        } break;

        case DrawCommandType_RECT: {  ///
          if (!mode) {
            drawCallIndicesCount += 6;
            drawCallVerticesCount += 4;
          }
          else {
            const auto& data = command.DataRect();

            auto    off = data.anchor * data.size;
            Vector2 points[]{
              {data.pos.x - off.x, data.pos.y + data.size.y - off.y},
              {data.pos.x + data.size.x - off.x, data.pos.y + data.size.y - off.y},
              {data.pos.x - off.x, data.pos.y - off.y},
              {data.pos.x + data.size.x - off.x, data.pos.y - off.y},
            };

            for (auto& point : points) {
              point -= (Vector2)(LOGICAL_RESOLUTION) / 2.0f;
              point /= (Vector2)(LOGICAL_RESOLUTION) / 2.0f;
            }

            const auto r = ge.meta.screenToLogicalRatio;
            if (r >= 1) {  // Window is too wide.
              const auto c = 1 - 1 / r;
              for (auto& point : points)
                point.x -= point.x * c;
            }
            else {  // Window is too high.
              const auto c = 1 - r;
              for (auto& point : points)
                point.y -= point.y * c;
            }

            const auto color = *(u32*)&data.color;

            const int quadIndices[]{0, 1, 2, 1, 3, 2};
            for (const auto index : quadIndices) {
              ((u16*)tib.data)[drawCallIndicesCount++]
                = (u16)(index + drawCallVerticesCount);
            }

            const _PosColorVertex quadVertices[]{
              {points[0].x, points[0].y, 0.0f, color},
              {points[1].x, points[1].y, 0.0f, color},
              {points[2].x, points[2].y, 0.0f, color},
              {points[3].x, points[3].y, 0.0f, color},
            };
            for (const auto& vertex : quadVertices)
              ((_PosColorVertex*)tvb.data)[drawCallVerticesCount++] = vertex;
          }
        } break;

        case DrawCommandType_CIRCLE_LINES: {  ///
          if (!mode) {
            drawCallIndicesCount += 16;
            drawCallVerticesCount += 8;
          }
          else {
            const auto& data = command.DataCircle();

            const auto& centerX = data.pos.x;
            const auto& centerY = data.pos.y;
            const auto& radius  = data.radius;

            const auto s = SQRT_2_OVER_2;
            Vector2    points[8]{
              {centerX, centerY - radius},
              {centerX + radius * s, centerY - radius * s},
              {centerX + radius, centerY},
              {centerX + radius * s, centerY + radius * s},
              {centerX, centerY + radius},
              {centerX - radius * s, centerY + radius * s},
              {centerX - radius, centerY},
              {centerX - radius * s, centerY - radius * s},
            };

            for (auto& point : points) {
              point -= (Vector2)(LOGICAL_RESOLUTION) / 2.0f;
              point /= (Vector2)(LOGICAL_RESOLUTION) / 2.0f;
            }

            const auto r = ge.meta.screenToLogicalRatio;
            if (r >= 1) {  // Window is too wide.
              const auto c = 1 - 1 / r;
              for (auto& point : points)
                point.x -= point.x * c;
            }
            else {  // Window is too high.
              const auto c = 1 - r;
              for (auto& point : points)
                point.y -= point.y * c;
            }

            const int quadIndices[]{0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 0};
            for (const auto index : quadIndices) {
              ((u16*)tib.data)[drawCallIndicesCount++]
                = (u16)(index + drawCallVerticesCount);
            }

            const auto            color = *(u32*)&data.color;
            const _PosColorVertex quadVertices[]{
              {points[0].x, points[0].y, 0.0f, color},
              {points[1].x, points[1].y, 0.0f, color},
              {points[2].x, points[2].y, 0.0f, color},
              {points[3].x, points[3].y, 0.0f, color},
              {points[4].x, points[4].y, 0.0f, color},
              {points[5].x, points[5].y, 0.0f, color},
              {points[6].x, points[6].y, 0.0f, color},
              {points[7].x, points[7].y, 0.0f, color},
            };
            for (const auto& vertex : quadVertices)
              ((_PosColorVertex*)tvb.data)[drawCallVerticesCount++] = vertex;
          }
        } break;

        case DrawCommandType_RECT_LINES: {  ///
          if (!mode) {
            drawCallIndicesCount += 8;
            drawCallVerticesCount += 4;
          }
          else {
            const auto& data = command.DataRect();

            Rect destRec{
              .pos{data.pos.x, data.pos.y},
              .size{data.size.x * abs(data.scale.x), data.size.y * abs(data.scale.y)},
            };
            destRec.pos -= LOGICAL_RESOLUTION / 2;

            Vector2 topLeft{};
            Vector2 topRight{};
            Vector2 bottomLeft{};
            Vector2 bottomRight{};

            auto dsx = destRec.size.x;
            auto dsy = destRec.size.y;

            if (data.rotation == 0.0f) {
              auto dx0 = destRec.pos.x;
              auto dx1 = destRec.pos.x + dsx;
              auto dy0 = destRec.pos.y;
              auto dy1 = destRec.pos.y + dsy;

              dx0 -= data.anchor.x * destRec.size.x;
              dx1 -= data.anchor.x * destRec.size.x;
              dy0 -= data.anchor.y * destRec.size.y;
              dy1 -= data.anchor.y * destRec.size.y;

              topLeft     = {dx0, dy0};
              topRight    = {dx1, dy0};
              bottomLeft  = {dx0, dy1};
              bottomRight = {dx1, dy1};
            }
            else {
              auto sinRotation = sinf(data.rotation);
              auto cosRotation = cosf(data.rotation);

              auto dx = -data.anchor.x * destRec.size.x;
              auto dy = -data.anchor.y * destRec.size.y;

              topLeft.x = destRec.pos.x + dx * cosRotation - dy * sinRotation;
              topLeft.y = destRec.pos.y + dx * sinRotation + dy * cosRotation;

              topRight.x = destRec.pos.x + (dx + dsx) * cosRotation - dy * sinRotation;
              topRight.y = destRec.pos.y + (dx + dsx) * sinRotation + dy * cosRotation;

              bottomLeft.x = destRec.pos.x + dx * cosRotation - (dy + dsy) * sinRotation;
              bottomLeft.y = destRec.pos.y + dx * sinRotation + (dy + dsy) * cosRotation;

              bottomRight.x
                = destRec.pos.x + (dx + dsx) * cosRotation - (dy + dsy) * sinRotation;
              bottomRight.y
                = destRec.pos.y + (dx + dsx) * sinRotation + (dy + dsy) * cosRotation;
            }

            topLeft.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
            topRight.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
            bottomLeft.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
            bottomRight.x /= (f32)LOGICAL_RESOLUTION.x / 2.0f;
            topLeft.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;
            topRight.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;
            bottomLeft.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;
            bottomRight.y /= (f32)LOGICAL_RESOLUTION.y / 2.0f;

            const auto r = ge.meta.screenToLogicalRatio;
            if (r >= 1) {  // Window is too wide.
              const auto c = 1 - 1 / r;
              topLeft.x -= topLeft.x * c;
              topRight.x -= topRight.x * c;
              bottomLeft.x -= bottomLeft.x * c;
              bottomRight.x -= bottomRight.x * c;
            }
            else {  // Window is too high.
              const auto c = 1 - r;
              topLeft.y -= topLeft.y * c;
              topRight.y -= topRight.y * c;
              bottomLeft.y -= bottomLeft.y * c;
              bottomRight.y -= bottomRight.y * c;
            }

            const int quadIndices[]{0, 1, 1, 2, 2, 3, 3, 0};
            for (const auto index : quadIndices) {
              ((u16*)tib.data)[drawCallIndicesCount++]
                = (u16)(index + drawCallVerticesCount);
            }

            const auto            color = *(u32*)&data.color;
            const _PosColorVertex quadVertices[]{
              {topLeft.x, topLeft.y, 0.0f, color},
              {topRight.x, topRight.y, 0.0f, color},
              {bottomRight.x, bottomRight.y, 0.0f, color},
              {bottomLeft.x, bottomLeft.y, 0.0f, color},
            };
            for (const auto& vertex : quadVertices)
              ((_PosColorVertex*)tvb.data)[drawCallVerticesCount++] = vertex;
          }
        } break;

        case DrawCommandType_TEXT: {  ///
          const auto& data = command.DataText();

          ASSERT(data.bytesCount > 0);
          ASSERT(font);
          if (!font->loaded) {
            LOGE("DrawCommandType_TEXT: font is not loaded!");
            INVALID_PATH;
            break;
          }

          if (!mode) {
            IterateOverCodepoints(
              data.text,
              data.bytesCount,
              [&font, &drawCallIndicesCount, &drawCallVerticesCount](
                u32 codepoint, u32 _codepointSize
              ) BF_FORCE_INLINE_LAMBDA {
                if (!codepoint)
                  return;
                if (codepoint == (u32)'\n')
                  return;

                drawCallIndicesCount += 6;
                drawCallVerticesCount += 4;
              }
            );
          }
          else {
            const auto& font = data.font;

            auto pos = LogicalPosToScreen(data.pos);

            const auto SSS = logicalToScreenScale / font->_scaleToFit;

            // Processing `anchor`.
            {
              f32 lineWidth    = 0;
              f32 maxLineWidth = 0;
              int height       = 1;
              IterateOverCodepoints(
                data.text,
                data.bytesCount,
                [&](u32 codepoint, u32 _codepointSize) BF_FORCE_INLINE_LAMBDA {
                  if (!codepoint) {
                    maxLineWidth = MAX(maxLineWidth, lineWidth);
                    return;
                  }
                  if (codepoint == (u32)'\n') {
                    maxLineWidth = MAX(maxLineWidth, lineWidth);
                    lineWidth    = 0;
                    height++;
                    return;
                  }

                  auto glyphIndex = ArrayBinaryFind(
                    font->codepoints, font->codepointsCount, (int)codepoint
                  );
                  ASSERT(glyphIndex >= 0);

                  f32                y_{};
                  stbtt_aligned_quad q{};
                  stbtt_GetPackedQuad(
                    font->chars,
                    font->atlasTexture.size.x,
                    font->atlasTexture.size.y,
                    glyphIndex,
                    &lineWidth,
                    &y_,
                    &q,
                    1  // 1=opengl & d3d10+,0=d3d9
                  );
                }
              );
              pos.x -= maxLineWidth * data.anchor.x * SSS;
              pos.y
                -= (f32)height
                   * ((f32)font->size * font->FIXME_sizeScale + 2 * (f32)font->outlineWidth)
                   * SSS * data.anchor.y / 2.0f;
            }

            auto y = ge.meta.screenSize.y - pos.y;

            const auto startPosX = pos.x;

            IterateOverCodepoints(
              data.text,
              data.bytesCount,
              [&](u32 codepoint, u32 _codepointSize) BF_FORCE_INLINE_LAMBDA {
                if (!codepoint)
                  return;
                if (codepoint == (u32)'\n') {
                  pos.x = startPosX;
                  y += ((f32)font->size * font->FIXME_sizeScale
                        + 2 * (f32)font->outlineWidth)
                       * SSS;
                  return;
                }

                auto glyphIndex = ArrayBinaryFind(
                  font->codepoints, font->codepointsCount, (int)codepoint
                );
                ASSERT(glyphIndex >= 0);

                stbtt_aligned_quad q{};
                f32                px       = 0.0f;
                auto               prevPosX = pos.x;
                auto               prevY    = y;
                stbtt_GetPackedQuad(
                  font->chars,
                  font->atlasTexture.size.x,
                  font->atlasTexture.size.y,
                  glyphIndex,
                  &pos.x,
                  // &pos.y,
                  &y,
                  &q,
                  1  // 1=opengl & d3d10+,0=d3d9
                );

                pos.x = prevPosX + (pos.x - prevPosX) * SSS;
                y     = prevY + (y - prevY) * SSS;
                q.x1  = q.x0 + (q.x1 - q.x0) * SSS;
                q.y0  = y + (q.y0 - y) * SSS;
                q.y1  = y + (q.y1 - y) * SSS;

                q.x0 -= font->outlineWidth * SSS;
                q.x1 += font->outlineWidth * SSS;
                q.y0 -= font->outlineWidth * SSS;
                q.y1 += font->outlineWidth * SSS;

                {
                  auto pos0 = ScreenPosToLogical({q.x0, q.y0});
                  auto pos1 = ScreenPosToLogical({q.x1, q.y1});
                  q.x0      = pos0.x;
                  q.y0      = pos0.y;
                  q.x1      = pos1.x;
                  q.y1      = pos1.y;
                }

                {
                  q.x0 = q.x0 + (q.x0 - data.pos.x) * (data.scale.x - 1);
                  q.x1 = q.x1 + (q.x1 - data.pos.x) * (data.scale.x - 1);
                  q.y0
                    += (q.y0 - (LOGICAL_RESOLUTION.y - data.pos.y)) * (data.scale.y - 1);
                  q.y1
                    += (q.y1 - (LOGICAL_RESOLUTION.y - data.pos.y)) * (data.scale.y - 1);
                }

                const f32 sx0 = q.s0;
                const f32 sx1 = q.s1;
                const f32 sy0 = q.t0;
                const f32 sy1 = q.t1;

                Vector2 topLeft{};
                Vector2 topRight{};
                Vector2 bottomLeft{};
                Vector2 bottomRight{};

                auto lrh    = (Vector2)LOGICAL_RESOLUTION / 2.0f;
                topLeft     = Vector2{q.x0, q.y1} / lrh - Vector2One();
                topRight    = Vector2{q.x1, q.y1} / lrh - Vector2One();
                bottomLeft  = Vector2{q.x0, q.y0} / lrh - Vector2One();
                bottomRight = Vector2{q.x1, q.y0} / lrh - Vector2One();

                topLeft.y     = -topLeft.y;
                topRight.y    = -topRight.y;
                bottomLeft.y  = -bottomLeft.y;
                bottomRight.y = -bottomRight.y;

                const auto r = ge.meta.screenToLogicalRatio;
                if (r >= 1) {  // Window is too wide.
                  const auto c = 1 - 1 / r;
                  topLeft.x -= topLeft.x * c;
                  topRight.x -= topRight.x * c;
                  bottomLeft.x -= bottomLeft.x * c;
                  bottomRight.x -= bottomRight.x * c;
                }
                else {  // Window is too high.
                  const auto c = 1 - r;
                  topLeft.y -= topLeft.y * c;
                  topRight.y -= topRight.y * c;
                  bottomLeft.y -= bottomLeft.y * c;
                  bottomRight.y -= bottomRight.y * c;
                }

                const int quadIndices[]{0, 1, 2, 1, 3, 2};
                for (const auto index : quadIndices) {
                  ((u16*)tib.data)[drawCallIndicesCount++]
                    = (u16)(index + drawCallVerticesCount);
                }

                const auto                    color = *(u32*)&data.color;
                const auto                    flash = *(u32*)&data.flash;
                const _PosColorFlashTexVertex quadVertices[]{
                  {topLeft.x, topLeft.y, 0.0f, color, flash, sx0, sy1},
                  {topRight.x, topRight.y, 0.0f, color, flash, sx1, sy1},
                  {bottomLeft.x, bottomLeft.y, 0.0f, color, flash, sx0, sy0},
                  {bottomRight.x, bottomRight.y, 0.0f, color, flash, sx1, sy0},
                };
                for (const auto& vertex : quadVertices)
                  ((_PosColorFlashTexVertex*)tvb.data)[drawCallVerticesCount++] = vertex;
              }
            );
          }
        } break;

        case DrawCommandType_STRIPS: {  ///
          const auto r = ge.meta.screenToLogicalRatio;

          if (!mode) {
            if (r > 1 || r < 1) {
              drawCallIndicesCount += 12;
              drawCallVerticesCount += 8;
            }
          }
          else {
            auto color = *(u32*)&ge.settings.backgroundColor;
            if (BF_DEBUG_VIGNETTE_AND_STRIPS)
              color = *(u32*)&GREEN;

            _PosColorVertex quadVertices[8]{};

            auto shouldDrawStrips = true;

            const auto    tex = glib->atlas_textures()->Get(glib->vignette_texture_id());
            constexpr f32 VIGNETTE_SIZE = 28.0f;

            if (r > 1) {
              // Window is too wide. Draw left and right strips.
              const auto vignetteSize = VIGNETTE_SIZE / (f32)tex->size_x() / r;
              const auto stripSize    = 1 - 1 / r + 0.002f - vignetteSize;

              // 1. Top-left.
              quadVertices[0] = {-1.001f, -1.001f, 0, color};
              // 1. Top-right.
              quadVertices[1] = {-1.001f + stripSize, -1.001f, 0, color};
              // 1. Bottom-left.
              quadVertices[2] = {-1.001f, 1.001f, 0, color};
              // 1. Bottom-right.
              quadVertices[3] = {-1.001f + stripSize, 1.001f, 0, color};

              // 2. Top-left.
              quadVertices[4] = {1.001f - stripSize, -1.001f, 0, color};
              // 2. Top-right.
              quadVertices[5] = {1.001f, -1.001f, 0, color};
              // 2. Bottom-left.
              quadVertices[6] = {1.001f - stripSize, 1.001f, 0, color};
              // 2. Bottom-right.
              quadVertices[7] = {1.001f, 1.001f, 0, color};
            }
            else if (r < 1) {
              // Window is too high. Draw bottom and top strips.
              const auto vignetteSize = VIGNETTE_SIZE / (f32)tex->size_y() * r;
              const auto stripSize    = 1 - r + 0.002f - vignetteSize;

              // 1. Top-left.
              quadVertices[0] = {-1.001f, -1.001f, 0, color};
              // 1. Top-right.
              quadVertices[1] = {1.001f, -1.001f, 0, color};
              // 1. Bottom-left.
              quadVertices[2] = {-1.001f, -1.001f + stripSize, 0, color};
              // 1. Bottom-right.
              quadVertices[3] = {1.001f, -1.001f + stripSize, 0, color};

              // 2. Top-left.
              quadVertices[4] = {-1.001f, 1.001f, 0, color};
              // 2. Top-right.
              quadVertices[5] = {1.001f, 1.001f, 0, color};
              // 2. Bottom-left.
              quadVertices[6] = {-1.001f, 1.001f - stripSize, 0, color};
              // 2. Bottom-right.
              quadVertices[7] = {1.001f, 1.001f - stripSize, 0, color};
            }
            else
              shouldDrawStrips = false;

            if (shouldDrawStrips) {
              const int quadIndices[]{0, 1, 2, 1, 3, 2, 4, 5, 6, 5, 7, 6};
              for (const auto index : quadIndices) {
                ((u16*)tib.data)[drawCallIndicesCount++]
                  = (u16)(index + drawCallVerticesCount);
              }

              for (const auto& vertex : quadVertices)
                ((_PosColorVertex*)tvb.data)[drawCallVerticesCount++] = vertex;
            }
          }
        } break;
        }

        addedSomethingToDraw = true;
      }
    }

    if (addedSomethingToDraw)
      endShader(mode);
  }

  ge.draw.commands.Reset();
  ge.draw.groups.Reset();
}

void _OnTouchDown(_TouchEvent touch) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  for (auto id : ge.meta._touchIDs)
    ASSERT(id != touch._id);

  _TouchData d{
    .data{
      .number      = touch._number,
      .screenPos   = touch._screenPos,
      .screenDelta = touch._screenDelta,
    },
    .state = _TouchDataState_PRESSED | _TouchDataState_DOWN,
  };
  *ge.meta._touchIDs.Add() = touch._id;
  *ge.meta._touches.Add()  = d;

  ge.meta._latestActiveTouchID = touch._id;
}

void _OnTouchUp(_TouchEvent touch) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  auto found = false;
  // Marking as released. It will be removed on calling `_ClearControlsCache`.
  FOR_RANGE (int, i, ge.meta._touches.count) {
    auto id = ge.meta._touchIDs[i];
    if (id == touch._id) {
      found            = true;
      auto& t          = ge.meta._touches[i];
      t.data.screenPos = touch._screenPos;
      t.state |= _TouchDataState_RELEASED;
      t.state &= ~_TouchDataState_DOWN;
      break;
    }
  }
  ASSERT(found);
}

void _OnTouchMoved(_TouchEvent touch) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  auto found = false;
  FOR_RANGE (int, i, ge.meta._touches.count) {
    if (ge.meta._touchIDs[i] == touch._id) {
      found              = true;
      auto& t            = ge.meta._touches[i];
      t.data.screenPos   = touch._screenPos;
      t.data.screenDelta = touch._screenDelta;
      break;
    }
  }
  ASSERT(found);
}

void _ClearControlsCache() {  ///
  FOR_RANGE (int, i, ge.meta._keyboardState.count) {
    ge.meta._keyboardStatePressed[i]  = false;
    ge.meta._keyboardStateReleased[i] = false;
  }

  ge.meta._mouseStatePressed  = 0;
  ge.meta._mouseStateReleased = 0;
  ge.meta._mouseWheel         = 0;

  // Removing previously released touches.
  {
    auto total = ge.meta._touches.count;
    auto off   = 0;
    FOR_RANGE (int, i, total) {
      if (ge.meta._touches[i - off].state & _TouchDataState_RELEASED) {
        auto id = ge.meta._touchIDs[i - off];
        ge.meta._touchIDs.UnstableRemoveAt(i - off);
        ge.meta._touches.UnstableRemoveAt(i - off);
        off++;
      }
    }
  }

  for (auto& t : ge.meta._touches)
    t.state &= ~_TouchDataState_PRESSED;

  ge.events.prevFrame = ge.events.thisFrame;
  ge.events.thisFrame = {};
}

bool IsTouchPressed(TouchID id) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  if (id == InvalidTouchID)
    return false;

  FOR_RANGE (int, i, ge.meta._touches.count) {
    if (ge.meta._touchIDs[i] == id) {
      auto& t = ge.meta._touches[i];
      return t.state & _TouchDataState_PRESSED;
    }
  }
  INVALID_PATH;  // Not found.
  return false;
}

bool IsTouchReleased(TouchID id) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  if (id == InvalidTouchID)
    return false;

  FOR_RANGE (int, i, ge.meta._touches.count) {
    if (ge.meta._touchIDs[i] == id) {
      auto& t = ge.meta._touches[i];
      return t.state & _TouchDataState_RELEASED;
    }
  }
  INVALID_PATH;  // Not found.
  return false;
}

bool IsTouchDown(TouchID id) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  FOR_RANGE (int, i, ge.meta._touches.count) {
    if (ge.meta._touchIDs[i] == id) {
      auto& t = ge.meta._touches[i];
      return t.state & _TouchDataState_DOWN;
    }
  }
  return false;
}

TEST_CASE ("_TouchEvent controls") {  ///
  LAMBDA (TouchID, id, (SDL_TouchID i)) {
    return {._touchID = i, ._fingerID = i};
  };
  LAMBDA (_TouchEvent, touch, (SDL_TouchID _id)) {
    return {._id = id(_id)};
  };

  {
    _OnTouchDown(touch(1));
    _OnTouchMoved(touch(1));

    ASSERT(IsTouchPressed(id(1)));
    ASSERT(IsTouchDown(id(1)));
    ASSERT(!IsTouchReleased(id(1)));
    _ClearControlsCache();

    ASSERT(!IsTouchPressed(id(1)));
    ASSERT(IsTouchDown(id(1)));
    ASSERT(!IsTouchReleased(id(1)));
    _ClearControlsCache();

    _OnTouchUp(touch(1));
    ASSERT(!IsTouchPressed(id(1)));
    ASSERT(!IsTouchDown(id(1)));
    ASSERT(IsTouchReleased(id(1)));
    _ClearControlsCache();

    ASSERT(ge.meta._touches.count == 0);
  }

  {
    _OnTouchDown(touch(3));
    _OnTouchDown(touch(4));
    ASSERT(ge.meta._touches.count == 2);
    ASSERT(ge.meta._touchIDs.count == 2);

    _ClearControlsCache();

    _OnTouchUp(touch(4));
    _OnTouchUp(touch(3));

    _ClearControlsCache();

    ASSERT(ge.meta._touches.count == 0);
    ASSERT(ge.meta._touchIDs.count == 0);
  }

  {
    _OnTouchDown(touch(5));
    _OnTouchDown(touch(6));

    _ClearControlsCache();

    _OnTouchUp(touch(5));
    _OnTouchUp(touch(6));

    _ClearControlsCache();

    ASSERT(ge.meta._touches.count == 0);
    ASSERT(ge.meta._touchIDs.count == 0);
  }
}

bgfx::ShaderHandle _LoadShader(const u8* data, u32 size) {  ///
  return bgfx::createShader(bgfx::makeRef(data, size));
}

View<TouchID> GetTouchIDs() {  ///
  return {.count = ge.meta._touchIDs.count, .base = ge.meta._touchIDs.base};
}

bgfx::ProgramHandle
LoadProgram(const u8* vsh, u32 sizeVsh, const u8* fsh, u32 sizeFsh) {  ///
  return bgfx::createProgram(
    _LoadShader(vsh, sizeVsh),
    _LoadShader(fsh, sizeFsh),
    /* destroyShaders */ true
  );
}

Texture2D _LoadTexture(const char* filepath, Vector2Int size) {  ///
  ZoneScopedN("_LoadTexture()");
  LOGI("Loading texture '%s'...", filepath);

  Texture2D result{.size = size};

  int    channels = 0;
  size_t dataSize = 0;

  void* data = nullptr;
  {
    ZoneScopedN("LoadFile()");
    data = LoadFile(filepath, &dataSize);
  }
  ASSERT(data);

  struct {
    basist::transcoder_texture_format from                = {};
    bgfx::TextureFormat::Enum         to                  = {};
    int                               bytesPerPixel       = 1;
    int                               divideBytesPerPixel = 1;
  } formatPairs[]{
    {
      .from = basist::transcoder_texture_format::cTFBC7_RGBA,
      .to   = bgfx::TextureFormat::BC7,
    },
    {
      .from = basist::transcoder_texture_format::cTFBC3_RGBA,
      .to   = bgfx::TextureFormat::BC3,
    },
    {
      .from                = basist::transcoder_texture_format::cTFPVRTC1_4_RGBA,
      .to                  = bgfx::TextureFormat::PTC14A,
      .divideBytesPerPixel = 2,
    },
    {
      .from                = basist::transcoder_texture_format::cTFPVRTC1_4_RGBA,
      .to                  = bgfx::TextureFormat::PTC14A,
      .divideBytesPerPixel = 2,
    },
    {
      .from = basist::transcoder_texture_format::cTFETC2_RGBA,
      .to   = bgfx::TextureFormat::ETC2A,
    },
    {
      .from          = basist::transcoder_texture_format::cTFRGBA32,
      .to            = bgfx::TextureFormat::RGBA8,
      .bytesPerPixel = 4,
    },
  };

  auto success = false;
  {
    ZoneScopedN("Transcoding texture");

    for (const auto pair : formatPairs) {
      LOGI(
        "Trying to transcode '%s' to '%s' format...",
        filepath,
        basist::basis_get_format_name(pair.from)
      );

      {
        ZoneScopedN("bgfx::isTextureValid()");

        if (!bgfx::isTextureValid(1, false, 1, pair.to, 0)) {
          LOGI(
            "'%s' is not supported on this platform",
            basist::basis_get_format_name(pair.from)
          );
          continue;
        }
      }

      basist::ktx2_transcoder transcoder{};
      transcoder.init(data, dataSize);

      // if (!transcoder.validate_file_checksums(data, dataSize, false))
      //   INVALID_PATH;
      // if (!transcoder.validate_header(data, dataSize))
      //   INVALID_PATH;
      if (!transcoder.start_transcoding())
        INVALID_PATH;

      ASSERT_FALSE(transcoder.is_video());

      basist::ktx2_image_level_info levelInfo{};
      if (!transcoder.get_image_level_info(levelInfo, 0, 0, 0))
        INVALID_PATH;

      auto outDataSize = (size_t)levelInfo.m_total_blocks
                         * (size_t)levelInfo.m_block_width
                         * (size_t)levelInfo.m_block_height * (size_t)pair.bytesPerPixel
                         / (size_t)pair.divideBytesPerPixel;
      auto outData = (u8*)BF_ALLOC(outDataSize);

      bool ok = false;
      {
        ZoneScopedN("basist. transcoder.transcode_image_level()");
        ok = transcoder.transcode_image_level(
          0,  // level_index
          0,  // layer_index
          0,  // face_index
          (void*)outData,
          (u32)outDataSize,
          pair.from
        );
      }

      if (ok) {
        ZoneScopedN("bgfx. bgfx::createTexture2D()");

        result.handle = bgfx::createTexture2D(
          size.x,
          size.y,
          false /* _hasMips*/,
          1,
          pair.to,
          BGFX_SAMPLER_MIN_ANISOTROPIC      //
            | BGFX_SAMPLER_MAG_ANISOTROPIC  //
            | BGFX_SAMPLER_MIP_POINT        //
            | BGFX_SAMPLER_U_CLAMP          //
            | BGFX_SAMPLER_V_CLAMP,
          bgfx::copy(outData, outDataSize)
        );

        success = true;
        break;
      }
      else
        INVALID_PATH;

      BF_FREE(outData);
    }
  }

  ASSERT(success);

  UnloadFile(data);

  LOGI("Loaded texture '%s'!", filepath);

  return result;
}

void _UnloadTexture(Texture2D* texture) {  ///
  bgfx::destroy(texture->handle);
  *texture = {};
}

void SetMusicLowpassFactor(f32 factor) {  ///
  ASSERT(factor >= 0);
  ASSERT(factor <= 1);

  auto&      m          = ge.meta._soundManager;
  const auto sampleRate = ma_engine_get_sample_rate(&m.engine);

  const auto minFreq    = 40.0f;
  const auto maxFreq    = (f32)sampleRate / 2.01f;
  const auto cutoffFreq = minFreq * powf(maxFreq / minFreq, Clamp01(factor));

  ASSERT(cutoffFreq >= minFreq);
  ASSERT(cutoffFreq <= maxFreq);

  const auto cfg = ma_lpf_config_init(
    ma_format_f32, ma_engine_get_channels(&m.engine), sampleRate, cutoffFreq, 8
  );
  if (ma_lpf_node_reinit(&cfg, &m.musicLpf) != MA_SUCCESS)
    INVALID_PATH;
}

void ReloadSounds() {  ///
  auto& m = ge.meta._soundManager;

  ma_engine_uninit(&m.engine);
  m.engine                        = {};
  m.playingSoundsBoolsGenerations = {};
  m.playingSoundsCount            = 0;

  // Resetting soundsToUninitialize.
  _EndedSound sound_{};
  while (m.soundsToUninitialize.try_dequeue(sound_))
    continue;

  m.works = false;

  // Initializing audio only if there are sounds in project.
  const auto fb_sounds = glib->sounds();
  if (!fb_sounds)
    return;
  if (!fb_sounds->size())
    return;

  m.engine    = {};
  auto config = ma_engine_config_init();
  if (ma_engine_init(&config, &m.engine) != MA_SUCCESS) {
    LOGW("Failed to init miniaudio engine");
    INVALID_PATH;
    return;
  }

  if (ma_engine_set_volume(&m.engine, m.volume) != MA_SUCCESS)
    INVALID_PATH;

  LOGI("miniaudio engine initialized");

  m.soundVariationRanges.Reset();
  m.soundVariationRanges.Reserve(fb_sounds->size());

  m.launchedSounds.Reset();
  m.launchedSounds.Reserve(fb_sounds->size());
  FOR_RANGE (int, i, fb_sounds->size()) {
    *m.launchedSounds.Add() = {};
  }

  m.groupSFX   = {};
  m.groupMusic = {};
  ma_sound_group_init(
    &m.engine, MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT, nullptr, &m.groupSFX
  );
  ma_sound_group_init(
    &m.engine, MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT, nullptr, &m.groupMusic
  );

  ma_fence fence{};
  if (ma_fence_init(&fence) != MA_SUCCESS) {
    LOGW("Error during ma_fence_init");
    INVALID_PATH;
    return;
  }
  DEFER {
    ma_fence_uninit(&fence);
  };

  int filesToLoad = 0;
  for (auto fb : *fb_sounds)
    filesToLoad += fb->variations()->size();

  m.soundVariationsLoadedFromFiles.Reset();
  m.soundVariationsLoadedFromFiles.Reserve(filesToLoad);
  const auto oldBase = m.soundVariationsLoadedFromFiles.base;

  bool _errored = false;
  LAMBDA (bool, checkErr, (ma_result res)) {
    if (res != MA_SUCCESS) {
      _errored = true;
      INVALID_PATH;
      return true;
    }
    return false;
  };

  {
    {
      const auto cfg = ma_lpf_node_config_init(
        ma_engine_get_channels(&m.engine), ma_engine_get_sample_rate(&m.engine), 2000, 8
      );
      checkErr(ma_lpf_node_init(&m.engineg, &cfg, nullptr, &m.musicLpf));
    }

    checkErr(ma_node_attach_output_bus(&m.groupMusic, 0, &m.musicLpf, 0));
    checkErr(
      ma_node_attach_output_bus(&m.musicLpf, 0, ma_node_graph_get_endpoint(&m.engineg), 0)
    );
    checkErr(
      ma_node_attach_output_bus(&m.groupSFX, 0, ma_node_graph_get_endpoint(&m.engineg), 0)
    );
  }

  for (auto fb : *fb_sounds) {
    u32 customFlags
      = MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;
    if (fb->pitch_min() == fb->pitch_max())
      customFlags |= MA_SOUND_FLAG_NO_PITCH;

    static_assert(sizeof(MA_SOUND_FLAG_DECODE) == sizeof(u32));
    u32             flags = MA_SOUND_FLAG_ASYNC | customFlags;
    ma_sound_group* group = nullptr;

    if (fb->is_music()) {
      flags |= MA_SOUND_FLAG_STREAM;
      group = &m.groupMusic;
    }
    else {
      flags |= MA_SOUND_FLAG_DECODE;
      group = &m.groupSFX;
    }

    *m.soundVariationRanges.Add() = {
      .start = m.soundVariationsLoadedFromFiles.count,
      .end   = m.soundVariationsLoadedFromFiles.count + (int)fb->variations()->size(),
    };

    int variationIndex = -1;
    for (auto fb_variation : *fb->variations()) {
      variationIndex++;

      auto slot = m.soundVariationsLoadedFromFiles.Add();

      *slot = {
        .filepath       = fb_variation->filepath()->c_str(),
        .soundHashValue = fb->enum_value_id(),
        .variation      = variationIndex,
        .flags          = customFlags,
      };

      auto fencePtr = &fence;
      if (fb->is_music())
        fencePtr = nullptr;
      checkErr(ma_sound_init_from_file(
        &m.engine, slot->filepath, flags, group, fencePtr, &slot->ma_sound
      ));
    }
  }

  ASSERT(oldBase == m.soundVariationsLoadedFromFiles.base);

  checkErr(ma_fence_wait(&fence));

  m.works = !_errored;
}

void InitEngine() {  ///
  ZoneScopedN("InitEngine");

  ge.meta._keyboardState.base = SDL_GetKeyboardState(&ge.meta._keyboardState.count);

  size_t arenaSize = 3 * sizeof(bool) * ge.meta._keyboardState.count;
  ge.meta._arena   = MakeArena(arenaSize + ge.settings.additionalArenaSize);

#if !defined(SDL_PLATFORM_EMSCRIPTEN)
  if (ge.settings.gameanalytics.setup) {
    const auto& a = ge.settings.gameanalytics;
    gameanalytics::GameAnalytics::setEnabledInfoLog(true);
    gameanalytics::GameAnalytics::setEnabledVerboseLog(BF_DEBUG);
    gameanalytics::GameAnalytics::configureBuild(a.build);
    gameanalytics::GameAnalytics::configureAvailableResourceCurrencies(a.currencies);
    gameanalytics::GameAnalytics::configureAvailableResourceItemTypes(a.resources);
    gameanalytics::GameAnalytics::initialize(a.gameKey, a.gameSecret);
  }
#endif

  {
    ZoneScopedN("basist. basist::basisu_transcoder_init()");
    basist::basisu_transcoder_init();
  }

  ge.meta._touches.Reserve(8);
  ge.meta._touchIDs.Reserve(8);

#if BF_DEBUG
  ge.meta._keyboardStatePressed  = (bool*)BF_ALLOC(ge.meta._keyboardState.count);
  ge.meta._keyboardStateReleased = (bool*)BF_ALLOC(ge.meta._keyboardState.count);
#else
  ge.meta._keyboardStatePressed
    = ALLOCATE_ZEROS_ARRAY(&ge.meta._arena, bool, ge.meta._keyboardState.count);
  ge.meta._keyboardStateReleased
    = ALLOCATE_ZEROS_ARRAY(&ge.meta._arena, bool, ge.meta._keyboardState.count);
#endif

#ifdef SDL_PLATFORM_WINDOWS
#  if BF_DEBUG
  auto glibPeekResult = PeekFiletime(GAMELIB_PATH);
  if (glibPeekResult.success)
    glibTime = glibPeekResult.filetime;
#  endif
#endif

  glibFile = LoadFile(GAMELIB_PATH, nullptr);
  glib     = BFGame::GetGameLibrary(glibFile);

  ge.meta.atlas = _LoadTexture(
    "resources/atlas_d2.basis", {glib->atlas_size_x(), glib->atlas_size_y()}
  );
  ReloadSounds();

  ge.meta.programDefaultTexture = LoadProgram(
    quad_tex_vs, ARRAY_COUNT(quad_tex_vs), quad_tex_fs, ARRAY_COUNT(quad_tex_fs)
  );
  ge.meta.programDefaultQuad
    = LoadProgram(quad_vs, ARRAY_COUNT(quad_vs), quad_fs, ARRAY_COUNT(quad_fs));

  // Remove Z?
  _PosColorFlashTexVertex::layout.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
    .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    .end();

  _PosColorVertex::layout.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();

  ge.meta.uniformTexture = bgfx::createUniform("u_texture", bgfx::UniformType::Sampler);

  ge.meta.screenScale = ScaleToFit(ASSETS_REFERENCE_RESOLUTION, LOGICAL_RESOLUTION);

  LOGI("Initialized engine");
}

void DeinitEngine() {  ///
  ma_engine_uninit(&ge.meta._soundManager.engine);
}

struct LoadFontData {
  const char* filepath = {};

  int size            = {};
  f32 FIXME_sizeScale = {};

  // `codepoints` must be in the same memory spot throughout the lifetime of their font.
  int* codepoints      = {};
  int  codepointsCount = {};

  int outlineWidth   = 0;  // Optional.
  f32 outlineAdvance = 0;  // Optional.
};

void _OutlineFonts(
  u8*        oneChannelAtlasData,
  u8*        atlasData,
  Vector2Int atlasSize,
  View<Font> fonts
) {  ///
  ZoneScoped;

  bool shouldReturnIfNoOutliningIsNeeded = true;
  for (auto& font : fonts) {
    if (font.outlineWidth > 0) {
      shouldReturnIfNoOutliningIsNeeded = false;
      break;
    }
  }
  if (shouldReturnIfNoOutliningIsNeeded)
    return;

  int maxCodepointWidth  = 0;
  int maxCodepointHeight = 0;
  for (const auto& font : fonts) {
    FOR_RANGE (int, i, font.codepointsCount) {
      const auto& c      = font.chars[i];
      maxCodepointWidth  = MAX(maxCodepointWidth, c.x1 - c.x0 + 2 * font.outlineWidth);
      maxCodepointHeight = MAX(maxCodepointHeight, c.y1 - c.y0 + 2 * font.outlineWidth);
    }
  }

  auto      dist_ = (f32*)BF_ALLOC(sizeof(f32) * maxCodepointWidth * maxCodepointHeight);
  View<f32> dist{.count = maxCodepointWidth * maxCodepointHeight, .base = dist_};

  for (auto& font : fonts) {
    if (font.outlineWidth > 0) {
      FOR_RANGE (int, codepointIndex, font.codepointsCount) {
        auto& fontChar = font.chars[codepointIndex];
        fontChar.x0 -= font.outlineWidth;
        fontChar.x1 += font.outlineWidth;
        fontChar.y0 -= font.outlineWidth;
        fontChar.y1 += font.outlineWidth;
        fontChar.xadvance += font.outlineAdvance;
      }
    }
  }

  for (auto& font : fonts) {
    if (font.outlineWidth <= 0)
      continue;

    FOR_RANGE (int, codepointIndex, font.codepointsCount) {
      auto& fontChar = font.chars[codepointIndex];

      const Vector2Int codepointSize{
        fontChar.x1 - fontChar.x0, fontChar.y1 - fontChar.y0
      };

      for (int oy = 0; oy < codepointSize.y; oy++) {
        for (int ox = 0; ox < codepointSize.x; ox++) {
          int distT  = oy * maxCodepointWidth + ox;
          int atlasT = (fontChar.y0 + oy) * atlasSize.x + fontChar.x0 + ox;
          if (oneChannelAtlasData[atlasT])
            dist[distT] = 1 - (f32)oneChannelAtlasData[atlasT] / 255.0f;
          else
            dist[distT] = f32_inf;
        }
      }

#define INDEX(y_, x_) ((y_) * maxCodepointWidth + (x_))

      // First pass: top-left to bottom-right
      for (int y = 0; y < codepointSize.y; y++) {
        for (int x = 0; x < codepointSize.x; x++) {
          int idx = INDEX(y, x);
          // Check neighbor above
          if (y > 0) {
            auto t    = INDEX(y - 1, x);
            f32  val  = dist[t] + 1;
            dist[idx] = MIN(dist[idx], val);
          }
          // Check neighbor to the left
          if (x > 0) {
            auto t    = INDEX(y, x - 1);
            f32  val  = dist[t] + 1;
            dist[idx] = MIN(dist[idx], val);
          }
          // Check neighbor diagonally above-left
          if ((y > 0) && (x > 0)) {
            auto t    = INDEX(y - 1, x - 1);
            f32  val  = dist[t] + SQRT_2;
            dist[idx] = MIN(dist[idx], val);
          }
          // Check neighbor diagonally above-right
          if ((y > 0) && (x < codepointSize.x - 1)) {
            auto t    = INDEX(y - 1, x + 1);
            f32  val  = dist[t] + SQRT_2;
            dist[idx] = MIN(dist[idx], val);
          }
        }
      }

      // Second pass: bottom-right to top-left
      for (int y = codepointSize.y - 1; y >= 0; y--) {
        for (int x = codepointSize.x - 1; x >= 0; x--) {
          int idx = INDEX(y, x);
          // Check neighbor below
          if (y + 1 < codepointSize.y) {
            auto t    = INDEX(y + 1, x);
            f32  val  = dist[t] + 1;
            dist[idx] = MIN(dist[idx], val);
          }
          // Check neighbor to the right
          if (x + 1 < codepointSize.x) {
            auto t    = INDEX(y, x + 1);
            f32  val  = dist[t] + 1;
            dist[idx] = MIN(dist[idx], val);
          }
          // Check neighbor diagonally below-right
          if ((y + 1 < codepointSize.y) && (x + 1 < codepointSize.x)) {
            auto t    = INDEX(y + 1, x + 1);
            f32  val  = dist[t] + SQRT_2;
            dist[idx] = MIN(dist[idx], val);
          }
          // Check neighbor diagonally below-left
          if ((y + 1 < codepointSize.y) && (x > 0)) {
            auto t    = INDEX(y + 1, x - 1);
            f32  val  = dist[t] + SQRT_2;
            dist[idx] = MIN(dist[idx], val);
          }
        }
      }

#undef INDEX

      FOR_RANGE (int, y, codepointSize.y) {
        FOR_RANGE (int, x, codepointSize.x) {
          auto distT  = y * maxCodepointWidth + x;
          auto atlasT = (y + fontChar.y0) * atlasSize.x + x + fontChar.x0;

          auto v                    = oneChannelAtlasData[atlasT];
          atlasData[atlasT * 4 + 0] = v;
          atlasData[atlasT * 4 + 1] = v;
          atlasData[atlasT * 4 + 2] = v;

          if (dist[distT] > 0) {
            u8 alpha = 255;
            if (dist[distT] > (f32)font.outlineWidth - 1)
              alpha = (u8)(255.0f * MAX((f32)font.outlineWidth - dist[distT], 0));
            atlasData[atlasT * 4 + 3] = alpha;
          }
        }
      }
    }
  }

  BF_FREE(dist_);
}

struct LoadFontsResult {
  bool               loaded          = false;
  View<Font>         _fonts          = {};
  void*              _freeMeOnUnload = {};
  Texture2D          _atlasTexture   = {};
  View<LoadFontData> _loadFontData   = {};
};

void _UnloadFileDataFromFonts(View<Font> fonts, View<LoadFontData> data) {  ///
  for (int i = 0; i < fonts.count; i++) {
    auto& font = fonts[i];
    if (!font.fileData)
      continue;

    for (int k = i + 1; k < fonts.count; k++) {
      if (!strcmp(data[k].filepath, data[i].filepath))
        fonts[k].fileData = nullptr;
    }
    UnloadFile((void*)font.fileData);
  }
}

std::tuple<bool, LoadFontsResult> _ProcessFonts(
  View<Font>         outFonts,
  View<LoadFontData> data_
) {  ///
  ASSERT(outFonts.count > 0);
  ASSERT(outFonts.count == data_.count);

  const f32 scaleToFit
    = ScaleToFit((Vector2)LOGICAL_RESOLUTION, (Vector2)ge.meta.screenSize);

  int maxExtraPadding = 0;
  for (const auto& data : data_) {
    maxExtraPadding = MAX(maxExtraPadding, Ceil((f32)data.outlineWidth * scaleToFit) * 2);
  }

  Vector2Int atlasSize = {512, 512};

  while (atlasSize.x <= BF_MAX_FONT_ATLAS_SIZE) {
    DEFER {
      atlasSize *= 2;
    };

    auto tempArena = MakeArena(atlasSize.x * atlasSize.y * 5);
    DEFER {
      DeinitArena(&tempArena);
    };

    auto tempOneChannelAtlasData
      = ALLOCATE_ARRAY(&tempArena, u8, atlasSize.x * atlasSize.y * 1);
    auto tempAtlasData = ALLOCATE_ARRAY(&tempArena, u8, atlasSize.x * atlasSize.y * 4);

    stbtt_pack_context context{};

    if (!stbtt_PackBegin(
          &context,
          tempOneChannelAtlasData,
          atlasSize.x,
          atlasSize.y,
          0,
          1 + maxExtraPadding,
          nullptr
        ))
    {
      LOGE("stbtt_PackBegin failed");
      INVALID_PATH;
      break;
    }

    bool tryEnlargingAtlas = false;

    FOR_RANGE (int, fontIndex, outFonts.count) {
      auto&       font = outFonts[fontIndex];
      const auto& data = data_[fontIndex];

      font.size           = data.size * scaleToFit;
      font.outlineWidth   = Ceil((f32)data.outlineWidth * scaleToFit);
      font.outlineAdvance = (f32)data.outlineAdvance * scaleToFit;
      font._scaleToFit    = scaleToFit;

      stbtt_pack_range range{
        .font_size                   = (f32)font.size * font.FIXME_sizeScale,
        .array_of_unicode_codepoints = font.codepoints,
        .num_chars                   = font.codepointsCount,
        .chardata_for_range          = font.chars,
      };

      ZoneScopedN("stbtt_PackFontRanges()");

      if (!stbtt_PackFontRanges(&context, font.fileData, 0, &range, 1)) {
        LOGE("stbtt_PackFontRanges failed");
        tryEnlargingAtlas = true;
        break;
      }
    }

    if (tryEnlargingAtlas)
      continue;

    stbtt_PackEnd(&context);

    FOR_RANGE (int, i, atlasSize.x * atlasSize.y) {
      tempAtlasData[i * 4 + 0] = 255;
      tempAtlasData[i * 4 + 1] = 255;
      tempAtlasData[i * 4 + 2] = 255;
      tempAtlasData[i * 4 + 3] = tempOneChannelAtlasData[i];
    }

    _OutlineFonts(tempOneChannelAtlasData, tempAtlasData, atlasSize, outFonts);

    // TODO: Rework as bgfx::TextureFormat::A8 + appropriate fragment shader.
    const auto atlasTextureHandle = bgfx::createTexture2D(
      atlasSize.x,
      atlasSize.y,
      false /* _hasMips */,
      1,
      bgfx::TextureFormat::RGBA8,
      BGFX_SAMPLER_MIN_ANISOTROPIC      //
        | BGFX_SAMPLER_MAG_ANISOTROPIC  //
        | BGFX_SAMPLER_MIP_POINT        //
        | BGFX_SAMPLER_U_CLAMP          //
        | BGFX_SAMPLER_V_CLAMP,
      bgfx::copy(tempAtlasData, atlasSize.x * atlasSize.y * 4)
    );

#if BF_DEBUG & !defined(SDL_PLATFORM_EMSCRIPTEN)
    if (0) {
      ZoneScopedN("stbi_write_png");
      stbi_write_png("debugFontAtlas.png", atlasSize.x, atlasSize.y, 4, tempAtlasData, 0);
    }
#endif

    for (auto& font : outFonts) {
      font.loaded       = true;
      font.atlasTexture = {
        .size   = atlasSize,
        .handle = atlasTextureHandle,
      };
    }

    return {
      true,
      {
        .loaded = true,
        ._fonts = outFonts,
        ._atlasTexture{
          .size   = atlasSize,
          .handle = atlasTextureHandle,
        },
        ._loadFontData = data_,
      }
    };
  }

  INVALID_PATH;
  return {false, {}};
}

// `data_.base` must live throughout the lifetime
// of fonts until `UnloadFonts` is called!
LoadFontsResult LoadFonts(View<Font> outFonts, View<LoadFontData> data_) {  ///
  ZoneScoped;

  int totalAllocatedPackedChars = 0;
  for (const auto& d : data_)
    totalAllocatedPackedChars += d.codepointsCount;
  // NOTE: `arena` doesn't get freed on success.
  auto arena = MakeArena(sizeof(stbtt_packedchar) * totalAllocatedPackedChars);

  FOR_RANGE (int, fontIndex, outFonts.count) {
    auto& font = outFonts[fontIndex];
    auto& data = data_[fontIndex];

#if BF_ENABLE_ASSERTS
    FOR_RANGE (int, i, data.codepointsCount - 1) {
      // Checking that `codepoints` are sorted.
      ASSERT(data.codepoints[i] < data.codepoints[i + 1]);
    }
#endif

    ASSERT(data.filepath);
    ASSERT(data.size >= 0);
    ASSERT(data.codepoints);
    ASSERT(data.codepointsCount > 0);
    ASSERT(data.outlineWidth >= 0);

#if BF_ENABLE_ASSERTS
    // Checking no duplicate codepoints.
    FOR_RANGE (int, i, data.codepointsCount) {
      auto codepoint = data.codepoints[i];
      auto c
        = ArrayContains(data.codepoints + i + 1, data.codepointsCount - i - 1, codepoint);
      ASSERT_FALSE(c);
    }
#endif

    u8* fileData = nullptr;
    FOR_RANGE (int, previousFontIndex, fontIndex) {
      if (!strcmp(data_[previousFontIndex].filepath, data.filepath)) {
        fileData = (u8*)outFonts[previousFontIndex].fileData;
        break;
      }
    }
    if (!fileData)
      fileData = (u8*)LoadFile(data.filepath, nullptr);

    font = Font{
      .FIXME_sizeScale = data.FIXME_sizeScale,
      .fileData        = fileData,
      .chars           = ALLOCATE_ARRAY(&arena, stbtt_packedchar, data.codepointsCount),
      .codepoints      = data.codepoints,
      .codepointsCount = data.codepointsCount,
    };

    stbtt_InitFont(&font.info, font.fileData, 0);
  }

  auto [success, result] = _ProcessFonts(outFonts, data_);
  result._freeMeOnUnload = arena.base;
  if (success)
    return result;

  DeinitArena(&arena);
  _UnloadFileDataFromFonts(outFonts, data_);
  INVALID_PATH;
  return {};
}

void ReloadFonts(LoadFontsResult* loadedFonts) {  ///
  ASSERT(loadedFonts->loaded);
  _UnloadTexture(&loadedFonts->_atlasTexture);
  for (auto& font : loadedFonts->_fonts) {
    font.size           = 0;
    font.outlineWidth   = 0;
    font.outlineAdvance = 0;
    font._scaleToFit    = 0;
    font.atlasTexture   = {};
  }
  auto [success, result] = _ProcessFonts(loadedFonts->_fonts, loadedFonts->_loadFontData);
  ASSERT(success);

  auto t                       = loadedFonts->_freeMeOnUnload;
  *loadedFonts                 = result;
  loadedFonts->_freeMeOnUnload = t;
}

void UnloadFonts(LoadFontsResult* loadedFonts) {  ///
  ASSERT(loadedFonts->loaded);
  if (!loadedFonts->loaded)
    return;

  ZoneScoped;
  LOGI("Unloading fonts...");

  BF_FREE(loadedFonts->_freeMeOnUnload);
  _UnloadTexture(&loadedFonts->_atlasTexture);
  _UnloadFileDataFromFonts(loadedFonts->_fonts, loadedFonts->_loadFontData);
  for (auto& font : loadedFonts->_fonts)
    font = {};
  *loadedFonts = {};
}

i64 GetTicks() {  ///
  return ge.meta.ticks;
}

f64 GetTime() {  ///
  return (f64)ge.meta.ticks / 1000.0;
}

f32 FrameTime() {  ///
  return (f32)(ge.meta.frameTime - ge.meta.prevFrameTime);
}

void EngineOnFrameStart() {
  ge.meta.ticks         = (i64)SDL_GetTicks();
  ge.meta.prevFrameTime = ge.meta.frameTime;
  ge.meta.frameTime     = GetTime();

  constexpr auto ratioLogical  = (f32)LOGICAL_RESOLUTION.x / (f32)LOGICAL_RESOLUTION.y;
  auto           ratioActual   = (f32)ge.meta.screenSize.x / (f32)ge.meta.screenSize.y;
  ge.meta.screenToLogicalRatio = ratioActual / ratioLogical;

  // Caching ScreenToLogical data.
  {  ///
    const auto r = ge.meta.screenToLogicalRatio;
    const auto s = (Vector2)ge.meta.screenSize;

    if (r >= 1) {
      auto m = (f32)LOGICAL_RESOLUTION.x * (r - 1) / 2.0f;
      ge.meta._screenToLogicalScale
        = {(f32)(LOGICAL_RESOLUTION.x + 2 * m) / s.x, (f32)LOGICAL_RESOLUTION.y / s.y};
      ge.meta._screenToLogicalAdd = {-m, 0};
    }
    else {
      auto m = (f32)LOGICAL_RESOLUTION.y * (1.0f / r - 1) / 2.0f;
      ge.meta._screenToLogicalScale
        = {(f32)LOGICAL_RESOLUTION.x / s.x, ((f32)LOGICAL_RESOLUTION.y + 2 * m) / s.y};
      ge.meta._screenToLogicalAdd = {0, -m};
    }
  }
}

bool IsKeyDown(SDL_Scancode key) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  return ge.meta._keyboardState[key];
}

bool IsKeyPressed(SDL_Scancode key) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  return ge.meta._keyboardStatePressed[key];
}

bool IsKeyReleased(SDL_Scancode key) {  ///
  ASSERT_FALSE(ge.meta._drawing);

  return ge.meta._keyboardStateReleased[key];
}

// Values: L, M, R, X1, X2.
#define IsMouseDown(button_) (ge.meta._mouseState & (SDL_BUTTON_##button_##MASK))
#define IsMousePressed(button_) \
  (ge.meta._mouseStatePressed & (SDL_BUTTON_##button_##MASK))
#define IsMouseReleased(button_) \
  (ge.meta._mouseStateReleased & (SDL_BUTTON_##button_##MASK))

const TouchData GetTouchData(TouchID id) {  ///
  FOR_RANGE (int, i, ge.meta._touches.count) {
    if (ge.meta._touchIDs[i] == id) {
      const auto& t = ge.meta._touches[i];
      return TouchData{
        .number      = t.data.number,
        .screenPos   = t.data.screenPos,
        .screenDelta = t.data.screenDelta,
      };
    }
  }
  INVALID_PATH;  // Not found.
  return {};
}

void SetTouchUserData(TouchID id, u64 userData) {  ///
  FOR_RANGE (int, i, ge.meta._touches.count) {
    if (ge.meta._touchIDs[i] == id) {
      ge.meta._touches[i].data.userData = userData;
      return;
    }
  }
  INVALID_PATH;  // Not found.
}

Vector2 GetMouseScreenPos() {  ///
  return ge.meta._mousePos;
}

Vector2 GetMouseOrLatestTouchScreenPos() {  ///
  return ge.meta._mouseOrLatestTouchPos;
}

Vector2 GetLatestTouchScreenPos() {  ///
  return ge.meta._latestTouchPos;
}

int GetMouseWheel() {  ///
  ASSERT_FALSE(ge.meta._drawing);

  return ge.meta._mouseWheel;
}

void EngineApplyVignette() {  ///
  DrawGroup_OneShotTexture(
    {
      .texID = glib->vignette_texture_id(),
      .pos   = LOGICAL_RESOLUTION / 2,
      .scale{4.01f, 4.01f},
      .color = (BF_DEBUG_VIGNETTE_AND_STRIPS ? RED : ge.settings.backgroundColor),
    },
    DrawZ_VIGNETTE
  );
}

void EngineApplyStrips() {  ///
  DrawGroup_Begin(DrawZ_VIGNETTE);
  DrawGroup_SetSortY(0);
  ge.draw.groups[ge.draw.currentGroupIndex].commandsCount++;
  *ge.draw.commands.Add() = {.type = DrawCommandType_STRIPS};
  DrawGroup_End();
}

void FrameGame::SetNow() {  ///
  ASSERT_FALSE(IsSet());
  _value = ge.meta.frameGame;
}

lframe FrameGame::Elapsed() const {  ///
  ASSERT(IsSet());
  return lframe::Unscaled(ge.meta.frameGame - _value);
}

void FrameVisual::SetNow() {  ///
  ASSERT_FALSE(IsSet());
  _value = ge.meta.frameVisual;
}

lframe FrameVisual::Elapsed() const {  ///
  ASSERT(IsSet());
  return lframe::Unscaled(ge.meta.frameVisual - _value);
}

SDL_AppResult EngineUpdate() {  ///
  ZoneScoped;

  static bool initialized = false;
  if (!initialized) {
    initialized = true;
    GameInit();
  }

  static f32 frameTime = 0.0f;
  frameTime += FrameTime();

  if (IsKeyDown(SDL_SCANCODE_F1) && IsKeyPressed(SDL_SCANCODE_F2))
    ge.meta.debugEnabled = !ge.meta.debugEnabled;

  if (ge.meta.debugEnabled || BF_DEBUG) {
    if (IsKeyPressed(SDL_SCANCODE_F3))
      IncrementSetZeroOn(&ge.meta.localization, 2);

    if (IsKeyPressed(SDL_SCANCODE_F4))
      IncrementSetZeroOn((int*)&ge.meta.device, (int)DeviceType_COUNT);
  }

  int simulated = 0;

  while (frameTime >= FIXED_DT) {
    frameTime -= FIXED_DT;

    SDL_PumpEvents();

    // Controls. Mouse.
    ge.meta._mouseState = SDL_GetMouseState(nullptr, nullptr);

    // Setting `ge.meta._latestActiveTouchID`.
    {
      ge.meta._latestActiveTouchID = {};
      ge.meta._latestTouchPos      = {-1, -1};
      int highestNumber            = 0;
      FOR_RANGE (int, i, ge.meta._touches.count) {
        const auto& t = ge.meta._touches[i];
        if (highestNumber < t.data.number) {
          highestNumber                  = t.data.number;
          ge.meta._latestActiveTouchID   = ge.meta._touchIDs[i];
          ge.meta._mouseOrLatestTouchPos = t.data.screenPos;
          ge.meta._latestTouchPos        = t.data.screenPos;
        }
      }
    }

#ifdef SDL_PLATFORM_WINDOWS
#  if BF_DEBUG
    auto glibPeekResult = PeekFiletime(GAMELIB_PATH);
    if (glibPeekResult.success && (glibPeekResult.filetime != glibTime)) {
      auto newGlibFile = TryLoadFile(GAMELIB_PATH, nullptr);
      if (newGlibFile) {
        UnloadFile(glibFile);

        glibFile = newGlibFile;
        glibTime = glibPeekResult.filetime;

        glib = BFGame::GetGameLibrary(glibFile);
        _UnloadTexture(&ge.meta.atlas);
        ge.meta.atlas = _LoadTexture(
          "../../../resources/atlas_d2.basis",
          {glib->atlas_size_x(), glib->atlas_size_y()}
        );
        ReloadSounds();
        LOGI("Gamelib reloaded!");
      }
    }
#  endif
#endif

    GameFixedUpdate();

    // Uninitializing ended sounds.
    {
      auto& m = ge.meta._soundManager;

      _EndedSound sound{};

      while (m.soundsToUninitialize.try_dequeue(sound)) {
        const auto& s = m.soundVariationsLoadedFromFiles[sound.loadedFileIndex];

        const auto fb = glib->sounds()->LookupByKey(
          m.soundVariationsLoadedFromFiles[sound.loadedFileIndex].soundHashValue
        );
        ASSERT(fb);

        if (fb->is_music()) {
          m.playingMusic = nullptr;
          PlaySound(fb->enum_value_id(), {.delayMilliseconds = 20000});
        }
        else {
          ma_sound_uninit(sound.ma_sound);

          auto index = sound.ma_sound - m.playingSounds.base;
          ASSERT(sound.ma_sound == m.playingSounds.base + index);
          ASSERT(m.playingSoundsBoolsGenerations[index]);

          m.playingSoundsBoolsGenerations[index] = 0;
          m.playingSoundsCount--;
        }
      }
    }

    if (ge.meta.quitScheduled)
      return SDL_APP_SUCCESS;
    _ClearControlsCache();

    simulated++;

    // Skipping frames if there are too many of them to simulate.
    if (simulated >= CeilDivision(FIXED_FPS, _BF_MIN_TARGET_FPS)) {
      frameTime = 0;
      break;
    }
  }

  // Forbidding use of `logicRand` during drawing.
  // This way game's logic remains deterministic.
  // It allows re-simulation of game using prerecorded inputs
  // such that game's state is always the same during and after the simulation.
  // TODO: record / replay inputs.
  // TODO: mb IsKeyPressed and other functions should also raise.
  ge.meta.logicRand._raise = true;
  ge.meta._drawing         = true;
  ge.draw.flushedThisFrame = false;
  GameDraw();
  ge.meta._drawing         = false;
  ge.meta.logicRand._raise = false;

  return SDL_APP_CONTINUE;
}

#ifdef BF_PLATFORM_Win
// struct Timestamp {  ///
//   int year;
//   int month;
//   int day;
//   int hour;
//   int minute;
//   int second;
//   int msecond;
// };

// Timestamp GetTimestamp() {  ///
//   SYSTEMTIME t{};
//   GetSystemTime(&t);
//
//   return Timestamp{
//     .year    = t.wYear,
//     .month   = t.wMonth,
//     .day     = t.wDay,
//     .hour    = t.wHour,
//     .minute  = t.wMinute,
//     .second  = t.wSecond,
//     .msecond = t.wMilliseconds,
//   };
// }
#endif

// void _Save(Arena* arena) {  ///
#if defined(SDL_PLATFORM_WIN32)

void _Save(Arena* _) {
  ZoneScoped;

  DEFER {
    ge.meta.previousSaveIsNotCompletedYet = false;
  };

  const auto toSwapPath = "save_to_swap.bin";

  bool saved = false;
  {
    auto fbb = GameDumpStateForSaving();
    FOR_RANGE (int, i, 5) {
      if (SDL_SaveFile(toSwapPath, (u8*)fbb.GetBufferPointer(), fbb.GetSize())) {
        saved = true;
        break;
      }
    }
  }
  if (!saved) {
    LOGE("Temp save failed");
    return;
  }

  bool swapped = false;
  FOR_RANGE (int, i, 5) {
    if (SDL_RenamePath(toSwapPath, "save.bin")) {
      swapped = true;
      break;
    }
  }
  if (!swapped)
    LOGI("Save swap failed");
}

#elif defined(BF_PLATFORM_WebYandex) || defined(BF_PLATFORM_Web)

extern "C" {
EMSCRIPTEN_KEEPALIVE void mark_js_returned_savedata(int value) {  ///
  ge.meta.jsLoadedSavedata = value;
}
}

#  if defined(BF_PLATFORM_WebYandex)

// clang-format off
EM_JS(void, js_Save, (const char* data), {
  window.player
    .setData({save: UTF8ToString(data)}, /* flush */ true)
    .then(() => {
      Module.ccall('saved_from_js', null, [], []);
    })
    .catch(() => {
      Module.ccall('saved_from_js', null, [], []);
    });
});
// clang-format on

#  elif defined(BF_PLATFORM_Web)

// clang-format off
EM_JS(void, js_Save, (const char* data), {
  localStorage.setItem('save', UTF8ToString(data));
  Module.ccall('saved_from_js', null, [], []);
});
// clang-format on

#  else
#    error "Not implemented"
#  endif

// clang-format off

// NOTE: Returned string must be `free`-d when `ge.meta.jsLoadedSavedata == 2`
EM_JS(const char*, jsLoad, (), {
#  if defined(BF_PLATFORM_WebYandex)
  if (!window.jsLoadCalled) {
    window.jsLoadCalled = true;
    window.jsLoad_savedataLoaded = false;
    window.jsLoad_savedataLoadingStarted = false;
    window.jsLoad_savedata = null;
  }

  if (!window.jsLoad_savedataLoadingStarted) {
    window.jsLoad_savedataLoadingStarted = true;

    window.player.getData(['save']).then((obj) => {
      window.jsLoad_savedata = obj?.save;
      window.jsLoad_savedataLoaded = true;
    }).catch(() => {
      window.jsLoad_savedataLoaded = true;
    });
  }

  if (!window.jsLoad_savedataLoaded)
    return null;

#  elif defined(BF_PLATFORM_Web)
  window.jsLoad_savedata = localStorage.getItem('save');
#  else
#    error "Not implemented"
#  endif

  if (!window.jsLoad_savedata) {
    Module.ccall('mark_js_returned_savedata', null, ['number'], [1]);
    return null;
  }

  Module.ccall('mark_js_returned_savedata', null, ['number'], [2]);

  var len            = lengthBytesUTF8(window.jsLoad_savedata) + 1;
  var string_on_heap = _malloc(len);
  stringToUTF8(window.jsLoad_savedata, string_on_heap, len + 1);
  return string_on_heap;
});
// clang-format on

void _Save(Arena* arena) {
  ZoneScoped;
  TEMP_USAGE(arena);
  auto fbb     = GameDumpStateForSaving();
  auto encoded = EncodeToHex(fbb.GetBufferPointer(), fbb.GetSize(), arena);
  js_Save(encoded);
}

#else
#  error "_Save(Arena* arena) is not implemented for your platform"
#endif
// }

SavedataLoadingType LoadSaveDataOnce(Arena* arena) {  ///
  TEMP_USAGE(arena);

  if (ge.meta.loading == SavedataLoadingType_JUST_FISNIHED)
    ge.meta.loading = SavedataLoadingType_FISNIHED;
  if (ge.meta.loading == SavedataLoadingType_FISNIHED)
    return SavedataLoadingType_FISNIHED;

  if (ge.meta.loading == SavedataLoadingType_NOT_LOADED) {
#if defined(SDL_PLATFORM_DESKTOP)

    // Desktop loads immediately.
    ge.meta.savedata = (const u8*)TryLoadFile("save.bin", nullptr);
    ge.meta.loading  = SavedataLoadingType_JUST_FISNIHED;

#elif defined(BF_PLATFORM_WebYandex) || defined(BF_PLATFORM_Web)

    // JS loads asynchronously.
    const char* savedata = jsLoad();
    if (ge.meta.jsLoadedSavedata)
      ge.meta.loading = SavedataLoadingType_JUST_FISNIHED;
    else
      return SavedataLoadingType_NOT_LOADED;

    if (ge.meta.jsLoadedSavedata == 2) {
      ge.meta.savedata = DecodeFromHex(savedata, arena);
      free((void*)savedata);
    }

#else
#  error "Not implemented yet"
#endif
  }
  else
    INVALID_PATH;

  if (ge.meta.savedata) {
    GameLoad(BFSave::GetSave(ge.meta.savedata));

#if defined(SDL_PLATFORM_DESKTOP)
    UnloadFile((void*)ge.meta.savedata);
#endif

    ge.meta.savedata = nullptr;
  }

  ASSERT(ge.meta.loading == SavedataLoadingType_JUST_FISNIHED);
  return ge.meta.loading;
}

bool IsEmulatingMobile() {
  return BF_DEBUG && (ge.meta.device == DeviceType_MOBILE);
}

struct FBFlattened {  ///
  int start = 0;
  int end   = 0;  // Exclusive.

  FBFlattened() = default;

  FBFlattened(const BFGame::Flattened* flattened) {
    if (flattened) {
      start = flattened->start();
      end   = flattened->end();
    }
    else {
      start = 0;
      end   = 0;
    }
  }

  RangeIterator Iter() const {
    return {start, end};
  }
};

FBFlattened ToFBFlattened(const BFGame::Flattened* fb) {  ///
  FBFlattened result{};
  if (fb) {
    result.start = fb->start();
    result.end   = fb->end();
  }
  return result;
}

///
