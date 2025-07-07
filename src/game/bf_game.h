#pragma once

#if BF_GAME_IMPLEMENTATION
#  define BF_GAME_EXPORT
#else
#  define BF_GAME_EXPORT extern
#endif

enum UpdateFunctionResult {
  UpdateFunctionResult_NO_ERROR_CONTINUE_RUNNING = -1,
  UpdateFunctionResult_FINISHED_SUCCESSFULLY     = 0,
  UpdateFunctionResult_ERR                       = 1,
};

#define Update_function(name_) UpdateFunctionResult name_() noexcept

extern "C" BF_GAME_EXPORT Update_function(GameUpdate);
