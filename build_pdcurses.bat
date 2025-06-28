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

set BUILD_PATH=%~dp0build
set DEPENDENCY_PATH=%BUILD_PATH%\dependencies
set DEPENDENCY_INC_PATH=%DEPENDENCY_PATH%\include
set DEPENDENCY_LIB_DIR=%DEPENDENCY_PATH%\%ARCH%\%CONFIG%
set OUT_DIR=%BUILD_PATH%\%ARCH%\%CONFIG%

if "%CONFIG%"=="Debug" (
    set CFLAGS=-Z7 -DPDCDEBUG   -DPDC_WIDE /MTd
) else if "%CONFIG%"=="Release" (
    set CFLAGS=-O2 -DPDC_WIDE /MT
) else (
    echo %0 ^<Debug^|Release^>
    exit /b 1
)

set START=%time%

if not exist "%BUILD_PATH%\%ARCH%" mkdir "%BUILD_PATH%\%ARCH%"
if not exist "%BUILD_PATH%\%ARCH%\%CONFIG%" mkdir "%BUILD_PATH%\%ARCH%\%CONFIG%"
if not exist "%OUT_DIR%\pdcurs34" mkdir "%OUT_DIR%\pdcurs34"

cl.exe /nologo /c /Ipdcurs34 %CFLAGS% /Fo:"%OUT_DIR%\pdcurs34\\" pdcurs34\pdcurses\*.c pdcurs34\win32\*.c
lib /nologo /out:"%OUT_DIR%\pdcurses.lib" "%OUT_DIR%\pdcurs34\*.obj"

set END=%time%
set /A "hours=(1%END:~0,2%-1%START:~0,2%)*3600"
set /A "mins=(1%END:~3,2%-1%START:~3,2%)*60"
set /A "secs=(1%END:~6,2%-1%START:~6,2%)"
set /A "elapsed=%hours%+%mins%+%secs%"
set /A "min=%elapsed%/60"
set /A "sec=%elapsed%%%60"
echo ** Build ^(%CONFIG% %ARCH%^) took %min%m%sec%s

endlocal