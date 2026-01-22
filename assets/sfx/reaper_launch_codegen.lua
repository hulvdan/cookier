local cmd = 'cmd.exe /C tasklist /FI "IMAGENAME eq game.exe"'
local output = reaper.ExecProcess(cmd, 1000)  -- 1-second timeout

-- Check if the process name appears in the result
if out,put:match("game.exe") then
  reaper.ExecProcess(string.format([[cmd /c "%s/cli/win_codegen_debug.bat"]], os.getenv("REAPER_LAUNCH_CWD")), -1)
end
