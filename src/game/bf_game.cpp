#pragma once

const char* GetWindowTitle() {
  return "The Game"
#if BF_DEBUG
         " [DEBUG]"
#endif
    ;
}

UpdateFunctionResult GameUpdate() {
  return UpdateFunctionResult_FINISHED_SUCCESSFULLY;
}
