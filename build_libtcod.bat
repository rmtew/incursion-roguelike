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
    set CFLAGS_LOCAL=/Zi /JMC /Od /MTd /D _DEBUG /RTC1
    set LFLAGS_LOCAL=/INCREMENTAL /DEBUG /PDB:"%OUT_DIR%\libtcod.pdb"
) else if "%CONFIG%"=="Release" (
    set CFLAGS_LOCAL=/Zi /O2 /MT /D NDEBUG
    set LFLAGS_LOCAL=/INCREMENTAL:NO /DEBUG /PDB:"%OUT_DIR%\libtcod.pdb" /OPT:REF /OPT:ICF
) else (
    echo %0 ^<Debug^|Release^>
    exit /b 1
)

set START=%time%

if not exist "%BUILD_PATH%\%ARCH%" mkdir "%BUILD_PATH%\%ARCH%"
if not exist "%BUILD_PATH%\%ARCH%\%CONFIG%" mkdir "%BUILD_PATH%\%ARCH%\%CONFIG%"
if not exist "%OUT_DIR%"\libtcod mkdir "%OUT_DIR%"\libtcod

pushd libtcod
cl.exe /c /I"%DEPENDENCY_INC_PATH%\SDL2" /Iinclude /Izlib /Iinclude /nologo /W3 /WX- /D WIN32 /D _WINDOWS /D TCOD_SDL2 /D LIBTCOD_EXPORTS /D _CRT_SECURE_NO_WARNINGS /D NO_OPENGL /D _WINDLL /Gm- /EHsc /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /Fo"%OUT_DIR%\libtcod\\" /external:W3 /Gd /TC /FC /errorReport:prompt %CFLAGS_LOCAL% src\bresenham_c.c src\bsp_c.c src\color_c.c src\console_c.c src\fov_c.c src\fov_circular_raycasting.c src\fov_diamond_raycasting.c src\fov_permissive2.c src\fov_recursive_shadowcasting.c src\fov_restrictive.c src\heightmap_c.c src\image_c.c src\lex_c.c src\list_c.c src\mersenne_c.c src\namegen_c.c src\noise_c.c src\parser_c.c src\path_c.c src\png\lodepng.c src\sys_c.c src\sys_opengl_c.c src\sys_sdl2_c.c src\sys_sdl_c.c src\sys_sdl_img_bmp.c src\sys_sdl_img_png.c src\tree_c.c src\txtfield_c.c src\wrappers.c src\zip_c.c src\zlib\adler32.c src\zlib\compress.c src\zlib\crc32.c src\zlib\deflate.c src\zlib\gzclose.c src\zlib\gzlib.c src\zlib\gzread.c src\zlib\gzwrite.c src\zlib\infback.c src\zlib\inffast.c src\zlib\inflate.c src\zlib\inftrees.c src\zlib\trees.c src\zlib\uncompr.c src\zlib\zutil.c
popd

link.exe /OUT:"%OUT_DIR%\libtcod.dll" /NOLOGO /LIBPATH:"%DEPENDENCY_LIB_DIR%" /LIBPATH:"%OUT_DIR%" SDL2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /SUBSYSTEM:CONSOLE,"5.02" /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"%OUT_DIR%\libtcod.lib" /DLL %LFLAGS_LOCAL% "%OUT_DIR%\libtcod\*.obj"

set END=%time%
set /A "hours=(1%END:~0,2%-1%START:~0,2%)*3600"
set /A "mins=(1%END:~3,2%-1%START:~3,2%)*60"
set /A "secs=(1%END:~6,2%-1%START:~6,2%)"
set /A "elapsed=%hours%+%mins%+%secs%"
set /A "min=%elapsed%/60"
set /A "sec=%elapsed%%%60"
echo ** Build ^(%CONFIG% %ARCH%^) took %min%m%sec%s

endlocal