local cmd = 'cmd.exe /C tasklist /FI "IMAGENAME eq game.exe"'
local output = reaper.ExecProcess(cmd, 1000)  -- 1-second timeout

-- Check if the process name appears in the result
if output:match("game.exe") then
  reaper.ExecProcess([[cmd /c "c:/Users/user/dev/home/cookier/cli/win_codegen_debug.bat"]], -1)
end
