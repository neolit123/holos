@echo off
rem --------- get time & date
set dir=h:\holos\
set srcdir=..\holos
set td=%date%%time%
set td=%td:,=%
set td=%td:.=%
set td=%td: =%
set td=%td::=%
set td0=%td:~0,8%
set td1=%td:~9%
set target=%dir%holos%td0%_%td1%.zip
echo %target%
rem --------- create archive
7za a -tzip %target% %srcdir% -mx9 -xr!*.exe -xr!.svn -xr!.git -xr!demo -xr!bin -xr!doxygen -xr!gal
:: copy %target% ..\
echo.
echo ***** created %target%
echo.
set td0=
set td1=
set target=
set dir=
echo ***** done
ping -n 1 -w 1000 0.0.0.1 > NUL
echo.
echo.

rem pause
