#include "basisu_transcoder.h"

#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/bf_gamelib_generated.h"

// #include "imgui.h"
// #include "imgui_impl_sdl3.h"
// #include "imgui_impl_sdlgpu3.h"
#include "bf_bgfx_imgui.cpp"

#define SDL_MAIN_USE_CALLBACKS
#include "bf_lib.cpp"

#if defined(SDL_PLATFORM_WIN32) || defined(SDL_PLATFORM_MACOS) \
  || defined(SDL_PLATFORM_LINUX)
#  define SDL_PLATFORM_DESKTOP
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "miniaudio.h"

#include "shaders/imgui_image_fs.bin.h"
#include "shaders/imgui_image_vs.bin.h"
#include "shaders/ocornut_imgui_fs.bin.h"
#include "shaders/ocornut_imgui_vs.bin.h"
#include "shaders/quad_fs.bin.h"
#include "shaders/quad_vs.bin.h"
#include "shaders/quad_tex_fs.bin.h"
#include "shaders/quad_tex_vs.bin.h"

#include <bgfx/bgfx.h>

#if !defined(SDL_PLATFORM_EMSCRIPTEN)
#  include "GameAnalytics/GameAnalytics.h"
#endif

#include "bf_version.cpp"
#include "hands/bf_codegen.cpp"
#include "engine/bf_engine_config.cpp"
#include "engine/bf_engine.cpp"
#include "game/bf_game.cpp"

struct EngineAppState {
  SDL_Window*    window     = {};
  SDL_GPUDevice* gpu_device = nullptr;
} g_appstate;

#if defined(SDL_PLATFORM_EMSCRIPTEN)
extern "C" {
#  ifdef BF_PLATFORM_WebYandex
EMSCRIPTEN_KEEPALIVE void mark_ysdk_loaded_from_js() {  ///
  ge.meta.ysdkLoaded = true;
}

EMSCRIPTEN_KEEPALIVE void pause_from_js() {  ///
  ge.meta.windowIsInactive = true;
}

EMSCRIPTEN_KEEPALIVE void resume_from_js() {  ///
  ge.meta.windowIsInactive = false;
}
#  endif

EMSCRIPTEN_KEEPALIVE void saved_from_js() {  ///
  ge.meta.previousSaveIsNotCompletedYet = false;
}

EMSCRIPTEN_KEEPALIVE void set_localization_from_js(int localization) {  ///
  ge.meta.localization = localization;
}

EMSCRIPTEN_KEEPALIVE void resize_from_js(int w, int h) {  ///
  if (g_appstate.window)
    SDL_SetWindowSize(g_appstate.window, w, h);
}

EMSCRIPTEN_KEEPALIVE void set_device_type_from_js(int type) {  ///
  ge.meta.device = (DeviceType)type;
  LOGI("Set device %d", type);
}
}

EM_JS(void, js_TriggerOnResize, (), { onResize(); });

EM_JS(void, js_LogWebGLVersion, (), {  ///
  let canvas = document.createElement('canvas');
  let gl     = canvas.getContext('webgl2') || canvas.getContext('webgl');
  if (gl) {
    console.log("GL_VERSION: " + gl.getParameter(gl.VERSION));
  }
  else {
    console.log("No WebGL context available.");
  }
});
#endif

class BGFXCallbackHandler : public bgfx::CallbackI {  ///
  public:
  void fatal(
    const char* filePath,
    uint16_t    line,
    bgfx::Fatal::Enum /* code */,
    const char* str
  ) override {
    LOGE("bgfx fatal [%s:%d]: %s\n", filePath, line, str);
    INVALID_PATH;
    exit(EXIT_FAILURE);
  }

  // Optional: override other methods if needed
  void traceVargs(
    const char* filePath,
    uint16_t    line,
    const char* format,
    va_list     argList
  ) override {}

  void profilerBegin(const char* name, uint32_t abgr, const char* filePath, uint16_t line)
    override {}

  void profilerBeginLiteral(
    const char* name,
    uint32_t    abgr,
    const char* filePath,
    uint16_t    line
  ) override {}

  void profilerEnd() override {}

  uint32_t cacheReadSize(uint64_t /* id */) override {
    return 0;
  }

  bool cacheRead(uint64_t /* id */, void* /* data */, uint32_t /* size */) override {
    return false;
  }

  void cacheWrite(uint64_t id, const void* data, uint32_t size) override {}

  void screenShot(
    const char* filePath,
    uint32_t    width,
    uint32_t    height,
    uint32_t    pitch,
    const void* data,
    uint32_t    size,
    bool        yflip
  ) override {}

  void captureBegin(
    uint32_t                  width,
    uint32_t                  height,
    uint32_t                  pitch,
    bgfx::TextureFormat::Enum format,
    bool                      yflip
  ) override {}

  void captureEnd() override {}

  void captureFrame(const void* data, uint32_t size) override {}
};

SDL_AppResult SDL_AppInit(void** /* appstate */, int /* argc */, char** /* argv */) {  ///
  ZoneScopedN("SDL_AppInit");

  GamePreInit();

#if defined(SDL_PLATFORM_EMSCRIPTEN)
  js_LogWebGLVersion();
#endif

  {
    ZoneScopedN("SDL. SDL_Init()");
    if (!SDL_Init(0)) {
      LOGE("SDL_Init failed!");
      return SDL_APP_FAILURE;
    }
  }

  SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;

  ge.meta.screenSize = LOGICAL_RESOLUTION;

  SDL_Window* window = nullptr;
  {
    ZoneScopedN("SDL. SDL_CreateWindow()");
    window = SDL_CreateWindow(
      GetWindowTitle(), ge.meta.screenSize.x, ge.meta.screenSize.y, flags
    );
    if (!window) {
      LOGE("SDL_CreateWindow failed!");
      return SDL_APP_FAILURE;
    }
    g_appstate.window = window;
  }

#if defined(SDL_PLATFORM_EMSCRIPTEN)
  SDL_SetWindowMouseGrab(window, true);
#endif

  {
    ZoneScopedN("bgfx. Initializing bgfx");
    bgfx::PlatformData pd{};

#if defined(SDL_PLATFORM_WIN32)
    pd.nwh = SDL_GetPointerProperty(
      SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr
    );
    pd.ndt = nullptr;
#elif defined(SDL_PLATFORM_MACOS)
    pd.nwh = SDL_GetPointerProperty(
      SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr
    );
    pd.ndt = nullptr;
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
      pd.ndt = SDL_GetPointerProperty(
        SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr
      );
      pd.nwh = (Window)SDL_GetNumberProperty(
        SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0
      );
    }
    else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
      pd.ndt = SDL_GetPointerProperty(
        SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr
      );
      pd.nwh = SDL_GetPointerProperty(
        SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr
      );
    }
#elif defined(SDL_PLATFORM_EMSCRIPTEN)
    pd.nwh = (void*)"#canvas";
    pd.ndt = nullptr;
#endif

    pd.context      = nullptr;
    pd.backBuffer   = nullptr;
    pd.backBufferDS = nullptr;

    bgfx::Init init{};
    for (auto disabled : ge.settings.bgfxDisabledCapabilities)
      init.capabilities &= ~disabled;

    static BGFXCallbackHandler bgfxCallbacks{};
    init.callback = &bgfxCallbacks;

    init.type              = bgfx::RendererType::OpenGL;
    init.vendorId          = BGFX_PCI_ID_NONE;
    init.platformData      = pd;
    init.resolution.width  = ge.meta.screenSize.x;
    init.resolution.height = ge.meta.screenSize.y;
    init.resolution.reset  = BGFX_RESET_VSYNC;

    {
      ZoneScopedN("bgfx. bgfx::init()");
      if (!bgfx::init(init)) {
        LOGE("bgfx::init(init) failed!");
        exit(EXIT_FAILURE);
      }
    }

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x000000ff, 1.0f, 0);
  }

  // ImGui.
  {
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

    ImGui_ImplSDL3_InitForSDLGPU(window);
    ImGui_ImplSDLGPU3_InitInfo initInfo{};
    initInfo.Device = g_appstate.gpu_device;
    initInfo.ColorTargetFormat
      = SDL_GetGPUSwapchainTextureFormat(g_appstate.gpu_device, window);
    initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;  // Only used in multi-viewports mode.
    initInfo.SwapchainComposition
      = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;  // Only used in multi-viewports mode.
    initInfo.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
    ImGui_ImplSDLGPU3_Init(&initInfo);
  }

  // Setup scaling
  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(ge.meta.imguiScale);
  style.FontScaleDpi = ge.meta.imguiScale;

#if defined(SDL_PLATFORM_EMSCRIPTEN)
  js_TriggerOnResize();
#endif
  InitEngine();

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* /* appstate */) {  ///
  SDL_AppResult result{};

#ifdef BF_PLATFORM_WebYandex
  static bool pr1 = false;
  static bool pr2 = false;
  if (!pr1) {
    pr1 = true;
    LOGI("Waiting for yandex sdk to initialize...");
  }
  if (!ge.meta.ysdkLoaded)
    return SDL_APP_CONTINUE;
  if (!pr2) {
    pr2 = true;
    LOGI("Waiting for yandex sdk to initialize... Done!");
  }
#endif

  {
    ZoneScopedN("SDL. SDL_AppIterate()");

    bgfx::setViewRect(0, 0, 0, (u16)ge.meta.screenSize.x, (u16)ge.meta.screenSize.y);
    bgfx::touch(0);
    bgfx::setViewMode(0, bgfx::ViewMode::Sequential);

    EngineOnFrameStart();

    bgfx::dbgTextClear(0, false);

    {
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

      ge.meta._drawing = true;

      // Background.
      DrawGroup_Begin(DrawZ_SCREEN_BACKGROUND);
      DrawGroup_SetSortY(0);
      {
        DrawGroup_CommandRect({
          .pos  = pos,
          .size = size,
          .anchor{},
          .color = ge.settings.backgroundColor,
        });
      }
      DrawGroup_End();

      // Fade.
      if (ge.settings.screenFade > 0) {
        DrawGroup_Begin(DrawZ_SCREEN_FADE);
        DrawGroup_SetSortY(0);
        {
          DrawGroup_CommandRect({
            .pos  = pos,
            .size = size,
            .anchor{},
            .color = Fade(ge.settings.screenFadeColor, ge.settings.screenFade),
          });
        }
        DrawGroup_End();
      }

      ge.meta._drawing = false;
    }

    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    result = EngineUpdate();
    if (result == SDL_APP_CONTINUE) {
      ZoneScopedN("bgfx. bgfx::frame()");
      bgfx::frame(false);

      {
        auto& io = ImGui::GetIO();

        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool  is_minimized
          = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

        SDL_GPUCommandBuffer* command_buffer
          = SDL_AcquireGPUCommandBuffer(g_appstate.gpu_device
          );  // Acquire a GPU command buffer

        SDL_GPUTexture* swapchain_texture;
        SDL_WaitAndAcquireGPUSwapchainTexture(
          command_buffer, g_appstate.window, &swapchain_texture, nullptr, nullptr
        );  // Acquire a swapchain texture

        if (swapchain_texture != nullptr && !is_minimized) {
          // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the
          // vertex/index buffer!
          ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

          // Setup and start a render pass
          SDL_GPUColorTargetInfo target_info = {};
          target_info.texture                = swapchain_texture;
          target_info.clear_color            = SDL_FColor{0, 0, 0, 1};
          target_info.load_op                = SDL_GPU_LOADOP_CLEAR;
          target_info.store_op               = SDL_GPU_STOREOP_STORE;
          target_info.mip_level              = 0;
          target_info.layer_or_depth_plane   = 0;
          target_info.cycle                  = false;
          SDL_GPURenderPass* render_pass
            = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

          // Render ImGui
          ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

          SDL_EndGPURenderPass(render_pass);
        }

        // Submit the command buffer
        SDL_SubmitGPUCommandBuffer(command_buffer);
      }
    }
  }

  FrameMark;
  return result;
}

SDL_AppResult SDL_AppEvent(void* /* appstate */, SDL_Event* event) {
  ImGui_ImplSDL3_ProcessEvent(event);

  static i64     nextTouchNumber     = 1;
  static bool    emulatedTouchIsDown = false;
  static TouchID emulatedTouchID{._touchID = 1, ._fingerID = 1};

  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;

  case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
    ge.meta.quitRequested = true;
  } break;

  case SDL_EVENT_WINDOW_RESIZED: {  ///
    auto window = SDL_GetWindowFromID(event->window.windowID);

    auto width  = event->window.data1;
    auto height = event->window.data2;

    bgfx::reset(width, height, BGFX_RESET_VSYNC);
    ge.meta.screenSize = {width, height};
  } break;

  case SDL_EVENT_KEY_DOWN: {  ///
    // if (event->key.repeat)
    //   break;

    if (event->key.scancode == SDL_SCANCODE_F11) {
      auto window     = SDL_GetWindowFromID(event->key.windowID);
      auto mode       = SDL_GetWindowFullscreenMode(window);
      auto fullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN;
      SDL_SetWindowFullscreen(window, !fullscreen);
    }

    ge.meta._keyboardStatePressed[event->key.scancode] = true;

    ge.events.thisFrame.keyPressed = true;
    ge.events.last                 = LastEventType_KEY;
  } break;

  case SDL_EVENT_KEY_UP: {  ///
    // if (event->key.repeat)
    //   break;

    ge.meta._keyboardStateReleased[event->key.scancode] = true;

    ge.events.thisFrame.keyReleased = true;
    ge.events.last                  = LastEventType_KEY;
  } break;

  case SDL_EVENT_MOUSE_BUTTON_DOWN: {  ///
    auto& e = event->button;
    auto  b = e.button;
    ASSERT(b > 0);
    ASSERT(b < 32);

    if (IsEmulatingMobile()) {
      // Emulating touch controls.
      emulatedTouchIsDown = true;
      _OnTouchDown({
        ._id     = emulatedTouchID,
        ._number = nextTouchNumber++,
        ._screenPos{e.x, ge.meta.screenSize.y - e.y},
      });

      ge.events.thisFrame.touchPressed = true;
      ge.events.last                   = LastEventType_TOUCH;
    }
    else {
      if ((b > 0) && (b < 32))
        MARK_BIT(&ge.meta._mouseStatePressed, b - 1);

      ge.events.thisFrame.mousePressed = true;
      ge.events.last                   = LastEventType_MOUSE;

      ge.meta._mouseOrLatestTouchPos = {e.x, ge.meta.screenSize.y - e.y};
      ge.meta._mousePos              = {e.x, ge.meta.screenSize.y - e.y};
    }
  } break;

  case SDL_EVENT_MOUSE_BUTTON_UP: {  ///
    auto& e = event->button;
    auto  b = e.button;
    ASSERT(b > 0);
    ASSERT(b < 32);

    if (IsEmulatingMobile()) {
      // Emulating touch controls.
      emulatedTouchIsDown = false;
      _OnTouchUp({
        ._id = emulatedTouchID,
        ._screenPos{e.x, ge.meta.screenSize.y - e.y},
      });

      ge.events.thisFrame.touchReleased = true;
      ge.events.last                    = LastEventType_TOUCH;
    }
    else {
      ASSERT(b > 0);
      ASSERT(b < 32);
      if ((b > 0) && (b < 32))
        MARK_BIT(&ge.meta._mouseStateReleased, b - 1);

      ge.events.thisFrame.mouseReleased = true;
      ge.events.last                    = LastEventType_MOUSE;

      ge.meta._mouseOrLatestTouchPos = {e.x, ge.meta.screenSize.y - e.y};
      ge.meta._mousePos              = {e.x, ge.meta.screenSize.y - e.y};
    }
  } break;

  case SDL_EVENT_MOUSE_WHEEL: {  ///
    const auto& e       = event->wheel;
    ge.meta._mouseWheel = MIN(1, MAX(-1, (e.direction ? -1 : 1) * e.integer_y));

    ge.events.thisFrame.mouseWheeled = true;
    ge.events.last                   = LastEventType_MOUSE;
  } break;

  case SDL_EVENT_MOUSE_MOTION: {  ///
    const auto& e = event->motion;

    if (IsEmulatingMobile()) {
      // Emulating touch controls.
      if (emulatedTouchIsDown) {
        _OnTouchMoved({
          ._id = emulatedTouchID,
          ._screenPos{e.x, ge.meta.screenSize.y - e.y},
          ._screenDelta{e.xrel, -e.yrel},
        });

        ge.events.thisFrame.touchMoved = true;
        ge.events.last                 = LastEventType_TOUCH;
      }
    }
    else {
      ge.events.thisFrame.mouseMoved = true;
      ge.events.last                 = LastEventType_MOUSE;

      ge.meta._mouseOrLatestTouchPos = {e.x, ge.meta.screenSize.y - e.y};
      ge.meta._mousePos              = {e.x, ge.meta.screenSize.y - e.y};
    }
  } break;

  case SDL_EVENT_FINGER_DOWN: {  ///
    const auto& e = event->tfinger;

    _OnTouchDown({
      ._id{._touchID = e.touchID, ._fingerID = e.fingerID},
      ._number    = nextTouchNumber++,
      ._screenPos = Vector2{e.x, 1 - e.y} * (Vector2)ge.meta.screenSize,
    });

    ge.events.thisFrame.touchPressed = true;
    ge.events.last                   = LastEventType_TOUCH;
  } break;

  case SDL_EVENT_FINGER_UP: {  ///
    const auto& e = event->tfinger;
    _OnTouchUp({
      ._id{._touchID = e.touchID, ._fingerID = e.fingerID},
      ._screenPos = Vector2{e.x, 1 - e.y} * (Vector2)ge.meta.screenSize,
    });

    ge.events.thisFrame.touchReleased = true;
    ge.events.last                    = LastEventType_TOUCH;
  } break;

  case SDL_EVENT_FINGER_MOTION: {  ///
    const auto& e = event->tfinger;
    _OnTouchMoved({
      ._id{._touchID = e.touchID, ._fingerID = e.fingerID},
      ._screenPos   = Vector2{e.x, 1 - e.y} * (Vector2)ge.meta.screenSize,
      ._screenDelta = Vector2{e.dx, -e.dy} * (Vector2)ge.meta.screenSize,
    });

    ge.events.thisFrame.touchMoved = true;
    ge.events.last                 = LastEventType_TOUCH;
  } break;

  case SDL_EVENT_WINDOW_FOCUS_LOST: {  ///
    // Required by yandex.
    ma_engine_set_volume(&ge.meta._soundManager.engine, 0);
  } break;

  case SDL_EVENT_WINDOW_FOCUS_GAINED: {  ///
    // Required by yandex.
    ma_engine_set_volume(&ge.meta._soundManager.engine, ge.meta._soundManager.volume);
  } break;

  default:
    break;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* /* appstate */, SDL_AppResult /* result */) {  ///
  DeinitEngine();

  SDL_WaitForGPUIdle(g_appstate.gpu_device);
  ImGui_ImplSDL3_Shutdown();
  ImGui_ImplSDLGPU3_Shutdown();
  ImGui::DestroyContext();

  bgfx::shutdown();

  SDL_DestroyWindow(g_appstate.window);
  SDL_Quit();
}

///
