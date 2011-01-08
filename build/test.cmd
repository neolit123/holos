@echo off
cls :: comment
:: echo comment
rem --------------------------------------------------
rem echo %0
rem echo %~d0
rem echo %~p0
rem echo %~n0
rem echo %~x0
rem echo %~f0


rem --------------------------------------------------

rem set PLATFORM=
rem if /I [%1]==[-linux] SET PLATFORM=H_LINUX
rem if /I [%1]==[-win32] SET PLATFORM=H_WIN32
rem if [%PLATFORM%]==[] (
rem   ECHO "Usage: xxx <-linux|-win32>"
rem ) 
rem ECHO Source dir is: %PLATFORM%

rem --------------------------------------------------

set H_PLATFORM=
set H_INTERFACE=
set H_FORMAT=

set H_OPTIMIZE_FLAGS=
set H_DEBUG_FLAGS=
set H_PLATFORM_FLAGS=

:argloop
rem echo %1
if "%1"=="" goto argloop_end
rem if "%1"=="-linux" set linux=1
  if /I %1==-linux  set H_PLATFORM=H_LINUX
  if /I %1==-win32  set H_PLATFORM=H_WIN32
  if /I %1==-exe    set H_FORMAT=H_EXE
  if /I %1==-vst    set H_FORMAT=H_VST
  if /I %1==-ladspa set H_FORMAT=H_LADSPA
shift
goto argloop
:argloop_end

rem --------------------------------------------------

echo platform:  %H_PLATFORM%
echo interface: %H_INTERFACE%
echo format:    %H_FORMAT%

