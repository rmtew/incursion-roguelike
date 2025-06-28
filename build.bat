@echo off
setlocal EnableDelayedExpansion

REM Build script for Incursion (Release and Debug, Win32 and x64) using cl.exe
if not defined VSINSTALLDIR (
    echo Error: VSINSTALLDIR not defined. Run in Developer Command Prompt for Visual Studio.
    exit /b 1
)

if "%VSCMD_ARG_TGT_ARCH%"=="x64" (
    set ARCH=x64
) else if "%VSCMD_ARG_TGT_ARCH%"=="x86" (
    set ARCH=Win32
) else (
    echo Run 'vcvarsall x64' or 'vcvarsall x86' first.
    exit /b 1
)

set "CONFIG="
if /I "%1"=="debug" set "CONFIG=Debug"
if /I "%1"=="release" set "CONFIG=Release"
if "%CONFIG%"=="" (
    echo %0 ^<Debug^|Release^>
    exit /b 1
)

set START=%time%

set BUILD_PATH=%~dp0build
set DEPENDENCY_PATH=%BUILD_PATH%\dependencies
set DEPENDENCY_INC_PATH=%DEPENDENCY_PATH%\include
set INC_PATH=%~dp0\inc
set LIB_PATH=%~dp0\lib
set LIBTCOD_INC_PATH=
set CL_EXE=cl.exe
set LINK_EXE=link.exe

REM Compiler flags
set CFLAGS=/nologo /W3 /Zi /EHsc /I"%INC_PATH%" /I"%DEPENDENCY_INC_PATH%" /D_MBCS /D_CONSOLE /DWIN32
set LFLAGS=/nologo /SUBSYSTEM:CONSOLE

REM Create directories
for %%a in (Win32 x64) do (
    for %%b in (Release Debug) do (
        if not exist "%BUILD_PATH%\%%a\%%b" mkdir "%BUILD_PATH%\%%a\%%b"
        if not exist "%BUILD_PATH%\%%a\%%b\incursion" mkdir "%BUILD_PATH%\%%a\%%b\incursion"
        if not exist "%BUILD_PATH%\%%a\%%b\modaccent" mkdir "%BUILD_PATH%\%%a\%%b\modaccent"
        if not exist "%BUILD_PATH%\%%a\%%b\exe_libtcod" mkdir "%BUILD_PATH%\%%a\%%b\exe_libtcod"
        if not exist "%BUILD_PATH%\%%a\%%b\exe_curses" mkdir "%BUILD_PATH%\%%a\%%b\exe_curses"
    )
)

REM Build function
:build
set OUT_DIR=%BUILD_PATH%\%ARCH%\%CONFIG%
set LIB_DIR=%DEPENDENCY_PATH%\%ARCH%
set CFLAGS_LOCAL=%CFLAGS%
set LFLAGS_LOCAL=%LFLAGS%
if "%CONFIG%"=="Debug" (
    set CFLAGS_LOCAL=%CFLAGS% /DDEBUG /MTd
    set LFLAGS_LOCAL=/nologo /SUBSYSTEM:CONSOLE /LIBPATH:"%LIB_DIR%\Debug"
) else (
    set CFLAGS_LOCAL=%CFLAGS% /MT
    set LFLAGS_LOCAL=/nologo /SUBSYSTEM:CONSOLE /LIBPATH:"%LIB_DIR%\Release"
)

if "%CONFIG%"=="Debug" (
    echo ** Building modaccent ^(%ARCH% %CONFIG%^)...
    pushd modaccent\src
    for %%f in (*.cpp *.c) do (
        %CL_EXE% /c %CFLAGS_LOCAL% /I..\include /Fo"!OUT_DIR!\modaccent\%%~nf.obj" %%f
    )
    %LINK_EXE% %LFLAGS_LOCAL% /debug /pdb:"!OUT_DIR!\modaccent.pdb" /OUT:"!OUT_DIR!\modaccent.exe" "!OUT_DIR!\modaccent\*.obj" legacy_stdio_definitions.lib
    popd

    echo Generating tokens and grammar...
    if not exist "%DEPENDENCY_PATH%\flex.exe" (
        echo Error: flex.exe not found in %DEPENDENCY_PATH%
        exit /b
    )
    if not exist "!OUT_DIR!\modaccent.exe" (
        echo Error: modaccent.exe not found in !OUT_DIR!
        exit /b
    )
    "%DEPENDENCY_PATH%\flex.exe" --wincompat -B -i -osrc\Tokens.cpp lang\Tokens.Lex
    "!OUT_DIR!\modaccent.exe" lang\Grammar.acc src inc
)

pushd src
echo ** Building incursion.lib ^(%ARCH% %CONFIG%^)...
if "%CONFIG%"=="Release" (
    set "SRC_FILES="
    for /f "delims=" %%i in ('dir *.cpp *.c /b ^| findstr /v /i "cpp.*\.c yygram.cpp tokens.cpp"') do set "SRC_FILES=!SRC_FILES! %%i"
    %CL_EXE% !SRC_FILES! /c %CFLAGS_LOCAL% /I"%LIB_PATH%" /Fo"!OUT_DIR!\incursion\\" 
) else (
    %CL_EXE% /c %CFLAGS_LOCAL% /I"%LIB_PATH%" /Fo"!OUT_DIR!\incursion\\" *.c *.cpp
)
lib /nologo /OUT:"!OUT_DIR!\incursion.lib" "!OUT_DIR!\incursion\*.obj"
popd

echo ** Building IncursionLibtcod.exe ^(%ARCH% %CONFIG%^)...
%CL_EXE% /c %CFLAGS_LOCAL% /I"libtcod\include" /D_UNICODE /DLIBTCOD_TERM /DTCOD_SDL2 /DUSE_BREAKPAD /Fo"!OUT_DIR!\exe_libtcod\Wlibtcod.obj" src\Wlibtcod.cpp
%LINK_EXE% /debug /pdb:"!OUT_DIR!\IncursionLibtcod.pdb" %LFLAGS_LOCAL% /OUT:"!OUT_DIR!\IncursionLibtcod.exe" "!OUT_DIR!\exe_libtcod\Wlibtcod.obj" "!OUT_DIR!\incursion.lib" "!OUT_DIR!\libtcod.lib" "!OUT_DIR!\common.lib" "!OUT_DIR!\exception_handler.lib" "!OUT_DIR!\crash_generation_client.lib"

if not exist "!OUT_DIR!\pdcurses.lib" (
    echo pdcurses.lib not found, skipping build.
    goto :skip_pdcurses
)

echo ** Building IncursionCurses.exe ^(%ARCH% %CONFIG%^)...
%CL_EXE% /c %CFLAGS_LOCAL% /I"pdcurs34" /D_UNICODE /DCURSES_TERM /DUSE_BREAKPAD /Fo"!OUT_DIR!\exe_curses\Wcurses.obj" src\Wcurses.cpp
%LINK_EXE% /debug /pdb:"!OUT_DIR!\IncursionCurses.pdb" %LFLAGS_LOCAL% user32.lib advapi32.lib /OUT:"!OUT_DIR!\IncursionCurses.exe" "!OUT_DIR!\exe_curses\Wcurses.obj" "!OUT_DIR!\incursion.lib" "!OUT_DIR!\pdcurses.lib" "!OUT_DIR!\common.lib" "!OUT_DIR!\exception_handler.lib" "!OUT_DIR!\crash_generation_client.lib"

:skip_pdcurses

set END=%time%
set /A "hours=(1%END:~0,2%-1%START:~0,2%)*3600"
set /A "mins=(1%END:~3,2%-1%START:~3,2%)*60"
set /A "secs=(1%END:~6,2%-1%START:~6,2%)"
set /A "elapsed=%hours%+%mins%+%secs%"
set /A "min=%elapsed%/60"
set /A "sec=%elapsed%%%60"
echo ** Build ^(%CONFIG% %ARCH%^) took %min%m%sec%s

goto :eof

endlocal