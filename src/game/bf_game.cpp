#pragma once

const char* GetWindowTitle() {
  return "The Game"
#if BF_DEBUG
         " [DEBUG]"
#endif
    ;
}

void GamePreInit() {}

SDL_AppResult GameUpdate() {
  return SDL_APP_CONTINUE;
}
