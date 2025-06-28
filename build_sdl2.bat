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
    REM This is not recommended and if you must, use an earlier platform SDK than 10.0.22621.0. No idea which..
    REM set CFLAGS=%CFLAGS% /D WINDOWS_IGNORE_PACKING_MISMATCH
    echo Win32 requires older platform SDKs. With the latest ones winnt.h will assert on packing mismatch.
    exit /b 1
    REM set ARCH=Win32
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
    set CFLAGS=/Od /D _DEBUG /Z7 /MTd
    set RCFLAGS=/D _DEBUG
    set LFLAGS=
) else if "%CONFIG%"=="Release" (
    set CFLAGS=/O2 /D NDEBUG /Zi /MT
    set RCFLAGS=/D NDEBUG
    set LFLAGS=/OPT:REF /OPT:ICF
) else (
    echo %0 ^<Debug^|Release^>
    exit /b 1
)

set START=%time%

if not exist "%BUILD_PATH%\%ARCH%" mkdir "%BUILD_PATH%\%ARCH%"
if not exist "%BUILD_PATH%\%ARCH%\%CONFIG%" mkdir "%BUILD_PATH%\%ARCH%\%CONFIG%"
if not exist "%OUT_DIR%\SDL2" mkdir "%OUT_DIR%\SDL2"

CL.exe /c /I"SDL2\include" /nologo /W3 /WX- /diagnostics:column /Ob1 %CFLAGS% /D _WINDOWS /D _WINDLL /Gm- /EHsc /GS- /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /Fo"%OUT_DIR%\SDL2\\" /external:W3 /Gd /TC /FC /Zl /errorReport:prompt SDL2\src\libm\e_atan2.c SDL2\src\libm\e_log.c SDL2\src\libm\e_pow.c SDL2\src\libm\e_rem_pio2.c SDL2\src\libm\e_sqrt.c SDL2\src\libm\k_cos.c SDL2\src\libm\k_rem_pio2.c SDL2\src\libm\k_sin.c SDL2\src\libm\k_tan.c SDL2\src\libm\s_atan.c SDL2\src\libm\s_copysign.c SDL2\src\libm\s_cos.c SDL2\src\libm\s_fabs.c SDL2\src\libm\s_floor.c SDL2\src\libm\s_scalbn.c SDL2\src\libm\s_sin.c SDL2\src\libm\s_tan.c SDL2\src\SDL.c SDL2\src\SDL_assert.c SDL2\src\atomic\SDL_atomic.c SDL2\src\audio\SDL_audio.c SDL2\src\audio\SDL_audiocvt.c SDL2\src\audio\SDL_audiodev.c SDL2\src\audio\SDL_audiotypecvt.c SDL2\src\render\software\SDL_blendfillrect.c SDL2\src\render\software\SDL_blendline.c SDL2\src\render\software\SDL_blendpoint.c SDL2\src\video\SDL_blit.c SDL2\src\video\SDL_blit_0.c SDL2\src\video\SDL_blit_1.c SDL2\src\video\SDL_blit_A.c SDL2\src\video\SDL_blit_auto.c SDL2\src\video\SDL_blit_copy.c SDL2\src\video\SDL_blit_N.c SDL2\src\video\SDL_blit_slow.c SDL2\src\video\SDL_bmp.c SDL2\src\video\SDL_clipboard.c SDL2\src\events\SDL_clipboardevents.c SDL2\src\cpuinfo\SDL_cpuinfo.c SDL2\src\render\SDL_d3dmath.c SDL2\src\haptic\windows\SDL_dinputhaptic.c SDL2\src\joystick\windows\SDL_dinputjoystick.c SDL2\src\audio\directsound\SDL_directsound.c SDL2\src\audio\disk\SDL_diskaudio.c SDL2\src\render\software\SDL_drawline.c SDL2\src\render\software\SDL_drawpoint.c SDL2\src\events\SDL_dropevents.c SDL2\src\audio\dummy\SDL_dummyaudio.c SDL2\src\dynapi\SDL_dynapi.c SDL2\src\video\SDL_egl.c SDL2\src\SDL_error.c SDL2\src\events\SDL_events.c SDL2\src\video\SDL_fillrect.c SDL2\src\joystick\SDL_gamecontroller.c SDL2\src\events\SDL_gesture.c SDL2\src\stdlib\SDL_getenv.c SDL2\src\haptic\SDL_haptic.c SDL2\src\SDL_hints.c SDL2\src\stdlib\SDL_iconv.c SDL2\src\joystick\SDL_joystick.c SDL2\src\events\SDL_keyboard.c SDL2\src\SDL_log.c SDL2\src\stdlib\SDL_malloc.c SDL2\src\audio\SDL_mixer.c SDL2\src\joystick\windows\SDL_mmjoystick.c SDL2\src\events\SDL_mouse.c SDL2\src\video\dummy\SDL_nullevents.c SDL2\src\video\dummy\SDL_nullframebuffer.c SDL2\src\video\dummy\SDL_nullvideo.c SDL2\src\video\SDL_pixels.c SDL2\src\power\SDL_power.c SDL2\src\stdlib\SDL_qsort.c SDL2\src\events\SDL_quit.c SDL2\src\video\SDL_rect.c SDL2\src\render\SDL_render.c SDL2\src\render\direct3d\SDL_render_d3d.c SDL2\src\render\direct3d11\SDL_render_d3d11.c SDL2\src\render\opengl\SDL_render_gl.c SDL2\src\render\opengles2\SDL_render_gles2.c SDL2\src\render\software\SDL_render_sw.c SDL2\src\video\SDL_RLEaccel.c SDL2\src\render\software\SDL_rotate.c SDL2\src\file\SDL_rwops.c SDL2\src\render\opengl\SDL_shaders_gl.c SDL2\src\render\opengles2\SDL_shaders_gles2.c SDL2\src\video\SDL_shape.c SDL2\src\atomic\SDL_spinlock.c SDL2\src\stdlib\SDL_stdlib.c SDL2\src\video\SDL_stretch.c SDL2\src\stdlib\SDL_string.c SDL2\src\video\SDL_surface.c SDL2\src\thread\generic\SDL_syscond.c SDL2\src\filesystem\windows\SDL_sysfilesystem.c SDL2\src\loadso\windows\SDL_sysloadso.c SDL2\src\thread\windows\SDL_sysmutex.c SDL2\src\power\windows\SDL_syspower.c SDL2\src\thread\windows\SDL_syssem.c SDL2\src\thread\windows\SDL_systhread.c SDL2\src\timer\windows\SDL_systimer.c SDL2\src\thread\windows\SDL_systls.c SDL2\src\thread\SDL_thread.c SDL2\src\timer\SDL_timer.c SDL2\src\events\SDL_touch.c SDL2\src\video\SDL_video.c SDL2\src\audio\SDL_wave.c SDL2\src\events\SDL_windowevents.c SDL2\src\core\windows\SDL_windows.c SDL2\src\video\windows\SDL_windowsclipboard.c SDL2\src\video\windows\SDL_windowsevents.c SDL2\src\video\windows\SDL_windowsframebuffer.c SDL2\src\haptic\windows\SDL_windowshaptic.c SDL2\src\joystick\windows\SDL_windowsjoystick.c SDL2\src\video\windows\SDL_windowskeyboard.c SDL2\src\video\windows\SDL_windowsmessagebox.c SDL2\src\video\windows\SDL_windowsmodes.c SDL2\src\video\windows\SDL_windowsmouse.c SDL2\src\video\windows\SDL_windowsopengl.c SDL2\src\video\windows\SDL_windowsopengles.c SDL2\src\video\windows\SDL_windowsshape.c SDL2\src\video\windows\SDL_windowsvideo.c SDL2\src\video\windows\SDL_windowswindow.c SDL2\src\audio\winmm\SDL_winmm.c SDL2\src\audio\xaudio2\SDL_xaudio2.c SDL2\src\core\windows\SDL_xinput.c SDL2\src\haptic\windows\SDL_xinputhaptic.c SDL2\src\joystick\windows\SDL_xinputjoystick.c SDL2\src\render\SDL_yuv_mmx.c SDL2\src\render\SDL_yuv_sw.c
rc.exe %RCFLAGS% /l"0x0409" /nologo /fo"%OUT_DIR%\SDL2_version.res" SDL2\src\main\windows\version.rc
link.exe /ERRORREPORT:PROMPT /OUT:"%OUT_DIR%\SDL2.dll" /NOLOGO winmm.lib imm32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NODEFAULTLIB /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /DEBUG /PDB:"%OUT_DIR%\SDL2.pdb" /SUBSYSTEM:WINDOWS %LFLAGS% /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"%OUT_DIR%\SDL2.lib" /DLL "%OUT_DIR%\SD2_version.res" "%OUT_DIR%\SDL2\*.obj"

set END=%time%
set /A "hours=(1%END:~0,2%-1%START:~0,2%)*3600"
set /A "mins=(1%END:~3,2%-1%START:~3,2%)*60"
set /A "secs=(1%END:~6,2%-1%START:~6,2%)"
set /A "elapsed=%hours%+%mins%+%secs%"
set /A "min=%elapsed%/60"
set /A "sec=%elapsed%%%60"
echo ** Build ^(%CONFIG% %ARCH%^) took %min%m%sec%s

endlocal