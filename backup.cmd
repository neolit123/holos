:: automatic backup script for 7za
:: athor lubomir i.ivanov (neolit123 [at] gmail)

@echo off

:: -----------------------------------------------------------------------------
:: *** user settings
::
:: location where to store backups (with \ at the end)
set target_path=h:\
:: source path; set to blank for directory where backup.cmd is
set source_path=
:: backup period in minutes; set to blank for no periodic backup
set period=
:: path to 7za.exe (with \ at the end); set to blank if 7za is included in PATH 
set path_7za=
:: 7za command line
set cmd_7za=a -tzip -mx3
:: exclude file mask or sub-folders
set exclude=-xr!*.exe -xr!.svn -xr!.git -xr!demo -xr!bin
::
:: end of user settings
:: -----------------------------------------------------------------------------

for %%I in ("%~dp0.") do set c_dir_name=%%~nI
set target_path=%target_path%%c_dir_name%\
set /a per_ms=period*60000
if [%source_path%]==[] (
  set source_path=%~d0%~p0
)

echo.
echo ===========================================================================
echo *** automatic backup script with 7za
echo.
echo working in directory: %source_path%
echo press ctrl+c to quit

:init
echo ===========================================================================
set td=%date%%time%
set td=%td:,=%
set td=%td:.=%
set td=%td: =%
set td=%td::=%
set td0=%td:~0,8%
set td1=%td:~9%
set target=%target_path%%c_dir_name%_%td0%_%td1%.zip
set cmd_7za_exec=%path_7za%7za %cmd_7za% %target% "%source_path%" %exclude%

%cmd_7za_exec%
echo.
if exist %target% (
  echo *** success 
) else (
  echo *** %target% failed  
  echo *** exit
  pause
  goto exit
)
echo.
if [%period%]==[] (
  ping -n 1 -w 1000 0.0.0.1 > NUL  
) else (
  echo *** next backup in %period% minute^(s^)
  ping -n 1 -w %per_ms% 0.0.0.1 > NUL
  goto init
)

:exit
