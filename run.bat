@echo off
setlocal enabledelayedexpansion

if "%1"=="" (echo Usage: %0 [Win32^|x64] [Debug^|Release] & exit /b 1)
if "%2"=="" (echo Usage: %0 [Win32^|x64] [Debug^|Release] & exit /b 1)

set "ARCH=%1"
set "CONFIG=%2"
set "CWD=%~dp0"

if not exist %ARCH%_%CONFIG% mkdir %ARCH%_%CONFIG%
pushd %ARCH%_%CONFIG%
set "files[0]=%CWD%build\%ARCH%\%CONFIG%\IncursionLibtcod.exe"
set "files[1]=%CWD%build\%ARCH%\%CONFIG%\IncursionLibtcod.pdb"
set "files[2]=%CWD%build\%ARCH%\%CONFIG%\SDL2.dll"
set "files[3]=%CWD%build\%ARCH%\%CONFIG%\SDL2.pdb"
set "files[4]=%CWD%build\%ARCH%\%CONFIG%\libtcod.dll"
set "files[5]=%CWD%build\%ARCH%\%CONFIG%\libtcod.pdb"
set "dirs[0]=%CWD%fonts;fonts"
set "dirs[1]=%CWD%lib;lib"

set "missing="
set "all_exist=1"

:: Check files
for /L %%i in (0,1,5) do (
    if not exist "!files[%%i]!" (
        set "missing=!missing! "!files[%%i]!""
        set "all_exist=0"
    )
)

:: Check directories
for /L %%i in (0,1,1) do (
    for /F "tokens=1 delims=;" %%d in ("!dirs[%%i]!") do (
        if not exist "%%d" (
            set "missing=!missing! "%%d""
            set "all_exist=0"
        )
    )
)

if %all_exist%==0 (
    echo Missing:
    for %%m in (%missing%) do echo %%~m
    exit /b 1
)

:: Copy files
for /L %%i in (0,1,5) do (
    copy "!files[%%i]!" . >nul
)

:: Copy directories
for /L %%i in (0,1,1) do (
    for /F "tokens=1,2 delims=;" %%d in ("!dirs[%%i]!") do (
        xcopy /I /E /Y "%%d" "%%e" >nul
    )
)
IncursionLibtcod.exe
popd
endlocal