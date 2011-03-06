:: buildall.cmd
:: ----------------------
@echo off
echo.
echo holos 'buildall' script for windows
echo ---------------------------------------------------------------------------
echo syntax: buildall [path (no \ at end)] [build flags (see build -h)]

if not exist "%1" goto nodir
for %%f in (%1\*.cpp) do @.\build.cmd "%%f" %2 %3 %4 %5 %6 %7 %8 %9
echo.
echo ** all done!
echo.
goto exit

:nodir
echo.
echo ## ERR: path not found - '%1%\'
echo.

:exit
