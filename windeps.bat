@echo off

:args
if "%~1"=="/owndeps" goto owndeps
goto die

:owndeps
REM sanish behaviour for variable assignment inside of for loops
REM requires local variables with delayed expansion enabled
setlocal ENABLEDELAYEDEXPANSION
REM using command extensions to just get the filename. Those should
REM be available NT4 onwards, so way older than we need to support
for %%s in (src/*.c); do set obj=!obj! $(OBJDIR)%%~ns.obj

echo IBAARD_DEPS=%obj% > dyn-own_deps.inc
endlocal
goto end

:end
exit 0

:die
echo "Invalid arguments"
exit 1
