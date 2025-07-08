#pragma once

UpdateFunctionResult GameUpdate() {
  return UpdateFunctionResult_FINISHED_SUCCESSFULLY;
}

const char* GetWindowTitle() {
  return "The Game"
#if BF_DEBUG
         " [DEBUG]"
#endif
    ;
}
