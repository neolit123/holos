:: compileall.cmd
:: ----------------------
@echo off
echo.
echo holos 'compileall' script for windows
echo ---------------------------------------------------------------------------
echo syntax: compileall [path (no \ at end)] [compile flags (see compile.cmd)]

if not exist "%1" goto nodir
for %%f in (%1\*.cpp) do @.\compile.cmd "%%f" %2 %3 %4 %5 %6 %7 %8 %9
echo.
echo ** all done!
echo.
goto exit

:nodir
echo.
echo ## ERR: path not found - '%1%\'
echo.

:exit
