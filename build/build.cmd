:: build.cmd
:: build script for lib holos / win32
::
:: Copyright (c) 2010 Tor-Helge Skei, Lubomir I. Ivanov et al
:: This file is part of the Holos Library.
:: http://holos.googlecode.com
:: the Holos Library is free software: you can redistribute it and/or modify
:: it under the terms of the Holos Library License, either version 1.0
:: of the License, or (at your option) any later version.
::
:: the Holos Library is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
:: See LICENSE_HOLOS for more details.
::
:: You should have received a copy of the Holos Library License
:: If not, see <http://holos.googlecode.com/>.

@echo off

:: ######### NOTE #########
:: set paths without the \ (or /) at the end

:: -----------------------------------------------------------------------------
:: *** user settings here
:: -----------------------------------------------------------------------------
:: set path to holos headers
:: e.g: c:\holos\ (or relative to the build.cmd) - location of Holos.h
set hpath=..

:: set path to vstsdk, e.g: c:\vstsdk24\ (or relative to build.cmd)
set vstpath=..\extern\vst

:: set path to vstsdk, e.g: c:\vstsdk24\ (or relative to build.cmd)
set ladspapath=..\extern\ladspa

:: set g++ path, e.g: c:\mingw\bin\g++
set mgwpath=g++

:: set windres path, e.g: c:\mingw\bin\windres
set windrespath=windres

:: set warning flags
set warn=-pedantic -fpermissive -W -Wall -Wextra -Wno-unused -Wno-long-long

:: set resource file (leave blank for no resources)
set resfile="rc_default.rc"

:: set optimization flags
:: you can add --whole-program --combine here for newer GCC
set opt=-O3

:: target & libraries
set tgtlib=-mwindows -lmsimg32 -lopengl32

:: linker options
set linker=-fstack-check -fdata-sections -ffunction-sections -Wl,-gc-sections -s

:: -----------------------------------------------------------------------------
:: *** end of user settings
:: -----------------------------------------------------------------------------

:: default values
set dstatus=OFF
set gccdstatus=OFF
set noguisx=
set dbgsuffix=
set nmv=
set nogui=
set dbg=
set res=
set gccdbg=
set cmdline=
set v=
set suffix=
set m64=
set ext_params=
set ext_p2=
set ext_p3=
set ext_p4=
set ext_p5=
set ext_p6=
set ext_p7=
set ext_p8=
set ext_p9=

:: check for file input
if [%1]==[] goto syntax
if [%1]==[-h] goto syntax

:: check for cpp file; replace / with \
set infile=%1
set infile=%infile:/=\%
set srcext=.cpp

:: remove quotes
set infile=###%infile%###
set infile=%infile:"###=%
set infile=%infile:###"=%
set infile=%infile:###=%

:: check extesion
if not "%infile:~-4%"=="%srcext%" goto nocpp

:: check includes
if not exist %hpath%\holos.h goto noholos

:: check for additional params
if not [%2]==[] (if not [%2]==[-v] (if not [%2]==[-d] (if not [%2]==[-g] (if not [%2]==[-ladspa] (if not [%2]==[-vst] (if not [%2]==[-exe] (if not [%2]==[-ts] (if not [%2]==[-nogui] (if not [%2]==[-nmv] (if not [%2]==[-64] (if not [%2]==[-32] set ext_p2= %2)))))))))))
if not [%3]==[] (if not [%3]==[-v] (if not [%3]==[-d] (if not [%3]==[-g] (if not [%3]==[-ladspa] (if not [%3]==[-vst] (if not [%3]==[-exe] (if not [%3]==[-ts] (if not [%3]==[-nogui] (if not [%3]==[-nmv] (if not [%3]==[-64] (if not [%3]==[-32] set ext_p3= %3)))))))))))
if not [%4]==[] (if not [%4]==[-v] (if not [%4]==[-d] (if not [%4]==[-g] (if not [%4]==[-ladspa] (if not [%4]==[-vst] (if not [%4]==[-exe] (if not [%4]==[-ts] (if not [%4]==[-nogui] (if not [%4]==[-nmv] (if not [%4]==[-64] (if not [%4]==[-32] set ext_p4= %4)))))))))))
if not [%5]==[] (if not [%5]==[-v] (if not [%5]==[-d] (if not [%5]==[-g] (if not [%5]==[-ladspa] (if not [%5]==[-vst] (if not [%5]==[-exe] (if not [%5]==[-ts] (if not [%5]==[-nogui] (if not [%5]==[-nmv] (if not [%5]==[-64] (if not [%5]==[-32] set ext_p5= %5)))))))))))
if not [%6]==[] (if not [%6]==[-v] (if not [%6]==[-d] (if not [%6]==[-g] (if not [%6]==[-ladspa] (if not [%6]==[-vst] (if not [%6]==[-exe] (if not [%6]==[-ts] (if not [%6]==[-nogui] (if not [%6]==[-nmv] (if not [%6]==[-64] (if not [%6]==[-32] set ext_p6= %6)))))))))))
if not [%7]==[] (if not [%7]==[-v] (if not [%7]==[-d] (if not [%7]==[-g] (if not [%7]==[-ladspa] (if not [%7]==[-vst] (if not [%7]==[-exe] (if not [%7]==[-ts] (if not [%7]==[-nogui] (if not [%7]==[-nmv] (if not [%7]==[-64] (if not [%7]==[-32] set ext_p7= %7)))))))))))
if not [%8]==[] (if not [%8]==[-v] (if not [%8]==[-d] (if not [%8]==[-g] (if not [%8]==[-ladspa] (if not [%8]==[-vst] (if not [%8]==[-exe] (if not [%8]==[-ts] (if not [%8]==[-nogui] (if not [%8]==[-nmv] (if not [%8]==[-64] (if not [%8]==[-32] set ext_p8= %8)))))))))))
if not [%9]==[] (if not [%9]==[-v] (if not [%9]==[-d] (if not [%9]==[-g] (if not [%9]==[-ladspa] (if not [%9]==[-vst] (if not [%9]==[-exe] (if not [%9]==[-ts] (if not [%9]==[-nogui] (if not [%9]==[-nmv] (if not [%9]==[-64] (if not [%9]==[-32] set ext_p9= %9)))))))))))
set ext_params=%ext_p2%%ext_p3%%ext_p4%%ext_p5%%ext_p6%%ext_p7%%ext_p8%%ext_p9%

:: check platform
if [%2]==[-64] set m64=yes
if [%3]==[-64] set m64=yes
if [%4]==[-64] set m64=yes
if [%5]==[-64] set m64=yes
if [%6]==[-64] set m64=yes
if [%7]==[-64] set m64=yes
if [%8]==[-64] set m64=yes
if [%9]==[-64] set m64=yes
if [%m64%]==[] ( 
  set mplt=-m32
  set mpltsx=x86
) else (
  set mplt=-m64
  set mpltsx=x64
)

:: check for 'not move'
if [%2]==[-nmv] set nmv=yes
if [%3]==[-nmv] set nmv=yes
if [%4]==[-nmv] set nmv=yes
if [%5]==[-nmv] set nmv=yes
if [%6]==[-nmv] set nmv=yes
if [%7]==[-nmv] set nmv=yes
if [%8]==[-nmv] set nmv=yes
if [%9]==[-nmv] set nmv=yes

if [%2]==[-nogui] set nogui=yes
if [%3]==[-nogui] set nogui=yes
if [%4]==[-nogui] set nogui=yes
if [%5]==[-nogui] set nogui=yes
if [%6]==[-nogui] set nogui=yes
if [%7]==[-nogui] set nogui=yes
if [%8]==[-nogui] set nogui=yes
if [%9]==[-nogui] set nogui=yes
if not [%nogui%]==[] set tgtlib=-DH_NOGUI && set noguisx=-nogui

:skipnogui
:: check for verbose
if [%2]==[-v] set v=yes
if [%3]==[-v] set v=yes
if [%4]==[-v] set v=yes
if [%5]==[-v] set v=yes
if [%6]==[-v] set v=yes
if [%7]==[-v] set v=yes
if [%8]==[-v] set v=yes
if [%9]==[-v] set v=yes

:: check for gcc debug mode
if [%2]==[-ts] set usets=yes
if [%3]==[-ts] set usets=yes
if [%4]==[-ts] set usets=yes
if [%5]==[-ts] set usets=yes
if [%6]==[-ts] set usets=yes
if [%7]==[-ts] set usets=yes
if [%8]==[-ts] set usets=yes
if [%9]==[-ts] set usets=yes

:: check for lib debug mode
if [%2]==[-d] goto setlibdebug
if [%3]==[-d] goto setlibdebug
if [%4]==[-d] goto setlibdebug
if [%5]==[-d] goto setlibdebug
if [%6]==[-d] goto setlibdebug
if [%7]==[-d] goto setlibdebug
if [%8]==[-d] goto setlibdebug
if [%9]==[-d] goto setlibdebug

:getgccdebug
:: check for gcc debug mode
if [%2]==[-g] goto setgccdebug
if [%3]==[-g] goto setgccdebug
if [%4]==[-g] goto setgccdebug
if [%5]==[-g] goto setgccdebug
if [%6]==[-g] goto setgccdebug
if [%7]==[-g] goto setgccdebug
if [%8]==[-g] goto setgccdebug
if [%9]==[-g] goto setgccdebug

:: get the tgt format
:getformat
if [%2]==[-ladspa] goto ladspatarget
if [%3]==[-ladspa] goto ladspatarget
if [%4]==[-ladspa] goto ladspatarget
if [%5]==[-ladspa] goto ladspatarget
if [%6]==[-ladspa] goto ladspatarget
if [%7]==[-ladspa] goto ladspatarget
if [%8]==[-ladspa] goto ladspatarget
if [%9]==[-ladspa] goto ladspatarget

if [%2]==[-vst] goto vsttarget
if [%3]==[-vst] goto vsttarget
if [%4]==[-vst] goto vsttarget
if [%5]==[-vst] goto vsttarget
if [%6]==[-vst] goto vsttarget
if [%7]==[-vst] goto vsttarget
if [%8]==[-vst] goto vsttarget
if [%9]==[-vst] goto vsttarget

if [%2]==[-exe] goto exetarget
if [%3]==[-exe] goto exetarget
if [%4]==[-exe] goto exetarget
if [%5]==[-exe] goto exetarget
if [%6]==[-exe] goto exetarget
if [%7]==[-exe] goto exetarget
if [%8]==[-exe] goto exetarget
if [%9]==[-exe] goto exetarget
goto :exetarget

:: set lib debug
:setlibdebug
set dbgsuffix=-debug
set dbg=-DH_DEBUG
set dstatus=ON
goto getgccdebug

:: set gcc debug
:setgccdebug
set dbgsuffix=-debug
set gccdbg=-DDEBUG -gstabs
set gccdstatus=ON
set resfile=
set linker=
set opt=-O3
goto getformat

:: format is ladspa
:ladspatarget
if not exist %ladspapath%\ladspa.h goto noladspa
if not [%v%]==[] echo ---------------------------------------------------------------------------
set suffix=-ladspa
set ext=.dll
set tgtlib=
set tgtformat=-DH_LADSPA -shared
set libfmt= LADSPA
goto begin

:: format is vst
:vsttarget
if not exist %vstpath%\aeffect.h goto novstsdk
if not [%v%]==[] echo ---------------------------------------------------------------------------
set suffix=-vst
set ext=.dll
set tgtformat=-DH_VST -shared
set libfmt= VST
goto begin

:: format is exe
:exetarget
if not [%v%]==[] echo ---------------------------------------------------------------------------
set libfmt=
set ext=.exe
set tgtformat=-DH_EXE
goto begin

:: echo syntax
:syntax
echo.
echo ---------------------------------------------------------------------------
echo * holos build script for windows
echo.
echo usage: build.cmd [file.cpp] [-h] [format] [other options] [...]
echo  -exe : create an executable (default)
echo  -vst : create a vst dll
echo  -ladspa : create a ladspa dll
echo  -nmv : do not move result to ..\bin
echo  -nogui : compile without gui
echo  -64 : target 64bit (default is 32bit)
echo  -ts : add time stamp to name e.g. plugin-vst-20153569.dll
echo  -d : enable library debug mode
echo  -g : enable gcc debug mode (-gstabs)
echo  -v : verbose
echo  -h : show this help message
echo  [...] : pass extra parameters e.g. -DSOME_DEF_1 -DSOME_DEF_2
echo ---------------------------------------------------------------------------
goto end

:: --------------------------
:: begin
:: --------------------------
:begin

:: resources
if [%resfile%]==[] goto nores
echo * preparing resources...
if exist %resfile%.o del %resfile%.o
%windrespath% -i %resfile% -o %resfile%.o
if not exist %resfile%.o goto nores
set res=%resfile%.o
:nores

:: get time stamp
if [%usets%]==[] goto nots
set td=%date%%time%
set td=%td:,=%
set td=%td:.=%
set td=%td: =%
set td=%td::=%
set td0=%td:~0,8%
set td1=%td:~9%
set td1=-%td1%
goto settarget
:nots
if not [%usets%]==yes set td1=

:: set target
:settarget
call set target=%%infile:%srcext%=%%
set target=%target%%suffix%%noguisx%%dbgsuffix%-%mpltsx%%td1%%ext%
set cmdpath=%~p0

:: delete old target
if exist %target% del %target%

:: echo settings
for /f "tokens=*" %%i in ('%mgwpath% -dumpversion') do set gccversion=%%i
echo * compiling windows binary for '%infile%'...
if not [%v%]==[] (
  echo.
  echo * compiler is: GCC %gccversion%
  echo * target binary format is:%libfmt% %ext%
  echo * target platform is: %mpltsx%
  echo * lib debug is: %dstatus%
  echo * gcc debug is: %gccdstatus%
  if "%noguisx%" == "-nogui" echo * gui is: OFF
  echo * found holos main '%hpath%'
  if "%libfmt%" == " VST" echo * found vst sdk main '%vstpath%'
  if "%libfmt%" == " LADSPA" echo * found ladpsa sdk path '%ladspapath%'
)

:compile
if not [%v%]==[] echo.
set cmdline=%mgwpath% -I%hpath% -I%vstpath% -I%ladspapath% %tgtformat% %opt% %warn% %gccdbg% %dbg% "%infile%" %res% -o "%target%" %ext_params% %tgtlib% %mplt% %linker%
:: show cmdline
if not [%v%]==[] echo command line is: %cmdline% && echo.
:: call g++
%cmdline%

:: target missing -> error
if not exist "%target%" echo. && echo # ERR: not compiled! && goto done
if not [%gccdstatus%]==[] goto printsize

:: call strip (no need if '-s' is passed to g++)
rem echo striping...
rem if exist %target% %mgwpath%strip --strip-all %target%

:printsize
:: target file size
if not [%v%]==[] for %%I in ("%target%") do echo * filesize: %%~zI bytes

:: check if '-nmv'
if not [%nmv%]==[] goto done
if not exist "%~p0..\bin" md "%~p0..\bin"
if exist "%target%" if not [%v%]==[] echo. && echo moving '%target%' to '%~p0..\bin'
if exist "%target%"	move "%target%" "%~p0..\bin"
:: --------------------------
:: done
:done
if not [%v%]==[] echo. && echo * done
goto end

:: errors
:: ----------------------------------------------------------------------------
:nocpp
echo.
echo ### ERR: first argument has to be a .cpp file
goto end
:: ----------------------------------------------------------------------------
:noholos
echo.
echo ### ERR: cannot find holos main '%hpath%\holos.h'
goto end
:: ----------------------------------------------------------------------------
:novstsdk
echo.
echo ### ERR: cannot find vst sdk main '%vstpath%\pluginterfaces\vst2.x\aeffect.h'
goto end
:: ----------------------------------------------------------------------------
:noladspa
echo.
echo ### ERR: cannot find ladspa sdk main '%ladspapath%\ladspa.h'
goto end
:: ----------------------------------------------------------------------------
:nores
echo.
echo ### ERR: cannot create resource file '%resfile%.o'
goto end
:: ----------------------------------------------------------------------------
:end
if not [%v%]==[] echo ---------------------------------------------------------------------------
