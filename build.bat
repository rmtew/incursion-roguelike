<# : 
@echo off
setlocal EnableDelayedExpansion

REM TODO: INCURSION USER
REM TODO: FETCH
REM TODO: - The top level prepared dependencies should be bundled for release, as a quick cheat pack.
REM TODO: -- Add to make-release/package-release steps.
REM TODO: -- Also support downloading and using them with a command-line argument if bitbucket allows.
REM TODO: PREPARE
REM TODO: - The version control archived zip.  How well does it work?  Should I even bother to support it?
REM TODO: -- What if someone installs hg after downloading and using the snapshot?
REM TODO: RELEASE

REM Divert to the internal setup code, it will return to the user setup.
goto internal_function_setup

REM --- FUNCTION: user_function_setup ----------------------------------------
:user_function_setup

REM This function is provided for the user to define what resources this script should
REM fetch and help prepare.  How they are actually prepared, the user specifies by
REM adding support below in 'user_function_prepare_dependency'.
REM
REM __ LINKS entries are either URLs to ZIP archives, or a collection of version control (vcs) information.
REM      LINKS[n]=<url to zip>
REM      LINKS[n]=vcs <vcs-system> <name> <revision-id> <repo-path> <snapshot-url>
REM        <revision-id>: This can be a URL, or it can be a filesystem path to an existing local clone.

set LINKS[0]=http://jaist.dl.sf.net/pub/sourceforge/w/wi/winflexbison/win_flex_bison-latest.zip
set LINKS[1]=vcs hg SDL2 704a0bfecf75 http://hg.libsdl.org/SDL http://hg.libsdl.org/SDL/archive/REV.zip
set LINKS[2]=vcs hg libtcod 7a8b072365b5 https://bitbucket.org/jice/libtcod https://bitbucket.org/jice/libtcod/get/REV.zip
set LINKS[3]=http://jaist.dl.sf.net/pub/sourceforge/p/pd/pdcurses/pdcurses/3.4/pdcurs34.zip
set LINKS[4]=vcs git gyp dd831fd https://chromium.googlesource.com/external/gyp https://chromium.googlesource.com/external/gyp/REV.tar.gz
set LINKS[5]=vcs git google-breakpad 0a0ad99 https://chromium.googlesource.com/external/google-breakpad/src/ https://chromium.googlesource.com/external/google-breakpad/src/REV.tar.gz

REM TODO: Comment out local git repositories and replace with remote ones before committing.
REM set LINKS[1]=vcs hg SDL2 704a0bfecf75 C:\RMT\VCS\HG\libraries\SDL http://hg.libsdl.org/SDL/archive/
REM set LINKS[2]=vcs hg libtcod 7a8b072365b5 C:\RMT\VCS\HG\libraries\libtcod https://bitbucket.org/jice/libtcod/get/
REM set LINKS[4]=vcs git gyp dd831fd C:\RMT\VCS\GIT\Tools\gyp ???
REM set LINKS[5]=vcs git google-breakpad 0a0ad99 C:\RMT\VCS\GIT\Libraries\google-breakpad ???

REM __ MD5CHECKSUMS entries are the MD5 checksum for the download in the matching LINKS position
REM      To get the checksum for a newly added download to add to an entry here, simply run the script and when
REM      the download is processed, the calculated checksum will be printed and noted as unchecked.

set MD5CHECKSUMS[0]=BB-C5-10-F0-91-34-2F-CF-5B-A7-A2-BC-A3-97-FD-A1
set MD5CHECKSUMS[3]=D6-3A-85-FD-5D-E9-60-11-71-02-87-4C-AB-5A-EB-90

set UV_INCLUDE_PATH=!DEPENDENCY_PATH!\include

set UV_INCLUDE_COMMANDS[0]=
set /A UV_INCLUDE_COMMAND_COUNT=0

set UV_PACKAGES_DIRNAME=packages
set UV_PACKAGES_PATH=!BUILD_PATH!\packages

REM Set to yes to get the buggy curses executables packaged as well.
set UV_PACKAGE_CURSES=no

REM Allow the user to specify the path to their Git 'git' executable.  It may have to be accessed via absolute path.
if not defined PYTHON_EXE (
    set "PATH=C:\Python27;%PATH%"
    python.exe --help >nul 2>&1 && (set PYTHON_EXE=python.exe) || (set PYTHON_EXE=)
)

REM Process the user data, calling event functions when applicable.
goto internal_function_main

REM --- FUNCTION: user_function_prepare_dependency --------------------------------
:user_function_prepare_dependency
REM variable: %V_LINK_PARTS% - The link data.
REM variable: %DEPENDENCY_PATH% - The absolute path of the dependencies directory.
REM variable: %V_DIRNAME% - The relative directory name the dependency can be found in.
REM variable: %V_SKIPPED% - 'yes' or 'no', depending on whether the archive was already extracted.

cd !DEPENDENCY_PATH!

set /A L_SDL2_ATTEMPTS=0
:user_function_prepare_dependency_retry

if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "http" (
    if "!V_LINK_PARTS[%HTTP_FILENAME%]!" EQU "win_flex_bison-latest.zip" (
        REM Extract the pre-built executable and put in the right location.
        if "%V_SKIPPED%" equ "no" (
            if exist flex.exe del flex.exe        
        )
        if not exist flex.exe (
            copy !V_DIRNAME!\win_flex.exe .\flex.exe
        )
    ) else if "!V_LINK_PARTS[%HTTP_FILENAME%]!" EQU "pdcurs34.zip" (
        cd !V_DIRNAME!\win32\
        
        if not exist "!DEPENDENCY_PATH!\pdcurses_d.lib" (
            echo Building: [pdcurses/Debug]
            nmake -f vcwin32.mak clean
            nmake -f vcwin32.mak DEBUG=1 WIDE=1 DLLOPT=/MTd pdcurses.lib
            copy pdcurses.lib "!DEPENDENCY_PATH!\pdcurses_d.lib"
        ) else (
            echo Building: [pdcurses/Debug] .. skipped
        )

        if not exist "!DEPENDENCY_PATH!\pdcurses.lib" (
            echo Building: [pdcurses/Release]
            nmake -f vcwin32.mak clean
            nmake -f vcwin32.mak WIDE=1 DLLOPT=/MT pdcurses.lib
            copy pdcurses.lib "!DEPENDENCY_PATH!\pdcurses.lib"
        ) else (
            echo Building: [pdcurses/Release] .. skipped
        )

        set UV_INCLUDE_COMMANDS[%UV_INCLUDE_COMMAND_COUNT%]=!V_DIRNAME!\*.h
        set /A UV_INCLUDE_COMMAND_COUNT=%UV_INCLUDE_COMMAND_COUNT%+1

        cd !DEPENDENCY_PATH!
    ) else (
        echo ERROR.. !V_LINK_PARTS[%HTTP_FILENAME%]! not handled by user who wrote the build amendments.
        goto internal_function_exit
    )
) else if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "vcs" (
    set L_VCS_NAME=!V_LINK_PARTS[%VCS_NAME%]!
    if "!L_VCS_NAME!" EQU "SDL2" (
        REM This variable is required by the libtcod makefile.
        set SDL2PATH=!DEPENDENCY_PATH!\!V_DIRNAME!

        cd !V_DIRNAME!

        if not exist "!DEPENDENCY_PATH!\SDL2_d.dll" (
            echo Building: [SDL2/Debug]

            REM SDL2 in theory requires the DirectX SDK to be installed, but in practice it's absence
            REM can be worked around by putting a dummy include file in place, and then only XAudio2
            REM support is lost.
            set L_ERROR_MSG=
            for /F "usebackq tokens=*" %%i in (`msbuild /nologo VisualC\SDL_VS2013.sln /p:Configuration^=Debug /p:Platform^=Win32 /t:SDL2^,SDL2main`) do (
                set L_LINE=%%i
                if "!L_LINE:fatal error=!" NEQ "!L_LINE!" set L_ERROR_MSG=%%i
            )
            set /A L_SDL2_ATTEMPTS=!L_SDL2_ATTEMPTS!+1

            if exist VisualC\SDL\Win32\Debug\SDL2.dll (
                copy VisualC\SDL\Win32\Debug\SDL2.dll "!DEPENDENCY_PATH!\SDL2_d.dll"
                copy VisualC\SDL\Win32\Debug\SDL2.lib "!DEPENDENCY_PATH!\SDL2_d.lib"
                copy VisualC\SDL\Win32\Debug\SDL2.pdb "!DEPENDENCY_PATH!\SDL2_d.pdb"
            ) else (
                REM Only try and recover from the DirectX problem, and if that don't work, give up.
                if "!L_ERROR_MSG!" NEQ "" (
                    if !L_SDL2_ATTEMPTS! EQU 1 (
                        if "!L_ERROR_MSG:dxsdkver=!" NEQ "!L_ERROR_MSG!" (
                            echo WARNING: Trying to make up for lack of installed DirectX SDK.
                            type nul > include\dxsdkver.h
                            cd !DEPENDENCY_PATH!
                            goto user_function_prepare_dependency_retry
                        )
                    )
                )
            
                echo ERROR.. SDL2.dll did not successfully build for some reason.
                goto internal_function_exit
            )
        ) else (
            echo Building: [SDL2/Debug] .. skipped
        )

        if not exist "!DEPENDENCY_PATH!\SDL2.dll" (
            echo Building: [SDL2/Release]

            msbuild /nologo VisualC\SDL_VS2013.sln /p:Configuration=Release /p:Platform=Win32 /t:SDL2,SDL2main
            if exist VisualC\SDL\Win32\Debug\SDL2.dll (
                copy VisualC\SDL\Win32\Release\SDL2.dll "!DEPENDENCY_PATH!\SDL2.dll"
                copy VisualC\SDL\Win32\Release\SDL2.lib "!DEPENDENCY_PATH!\SDL2.lib"
                copy VisualC\SDL\Win32\Release\SDL2.pdb "!DEPENDENCY_PATH!\SDL2.pdb"
            ) else (
                echo ERROR.. SDL2.dll did not successfully build for some reason.
                goto internal_function_exit
            )
        ) else (
            echo Building: [SDL2/Release] .. skipped
        )

        set UV_INCLUDE_COMMANDS[%UV_INCLUDE_COMMAND_COUNT%]=!V_DIRNAME!\include\*.h
        set /A UV_INCLUDE_COMMAND_COUNT=%UV_INCLUDE_COMMAND_COUNT%+1
    ) else if "!L_VCS_NAME!" EQU "libtcod" (
        set BASEPATH=!DEPENDENCY_PATH!\!V_DIRNAME!
        set BASEOBJPATH=!DEPENDENCY_PATH!\!V_DIRNAME!\build
        if not exist "!BASEOBJPATH!" mkdir "!BASEOBJPATH!"

        if not exist "!DEPENDENCY_PATH!\libtcod_d.dll" (
            echo Building: [libtcod/Debug]
            set CONFIG=Debug
            set BASENAME_SUFFIX=_d
            nmake /nologo -f ..\makefile-libtcod
            copy "!BASEOBJPATH!\!CONFIG!\libtcod_d.lib" libtcod_d.lib
            copy "!BASEOBJPATH!\!CONFIG!\libtcod_d.dll" libtcod_d.dll
            copy "!BASEOBJPATH!\!CONFIG!\libtcod_d.pdb" libtcod_d.pdb
        ) else (
            echo Building: [libtcod/Debug] .. skipped
        )

        if not exist "!DEPENDENCY_PATH!\libtcod.dll" (
            echo Building: [libtcod/Release]
            set CONFIG=Release
            nmake /nologo -f ..\makefile-libtcod
            copy "!BASEOBJPATH!\!CONFIG!\libtcod.lib" libtcod.lib
            copy "!BASEOBJPATH!\!CONFIG!\libtcod.dll" libtcod.dll
            copy "!BASEOBJPATH!\!CONFIG!\libtcod.pdb" libtcod.pdb
        ) else (
            echo Building: [libtcod/Release] .. skipped
        )

		set UV_INCLUDE_COMMANDS[%UV_INCLUDE_COMMAND_COUNT%]=!V_DIRNAME!\include\*.h
        set /A UV_INCLUDE_COMMAND_COUNT=%UV_INCLUDE_COMMAND_COUNT%+1
    ) else if "!L_VCS_NAME!" EQU "gyp" (
        REM gyp is built using Python, so no building is required.
        echo Building: [gyp] .. skipped
    ) else if "!L_VCS_NAME!" EQU "google-breakpad" (
        set L_LIBS=exception_handler.lib crash_generation_client.lib common.lib
        set L_LIBRELPATH=client\Windows\Release\lib
        set L_FAILED=no
        REM gyp is a bit flakey, and requires at least at this time, we be at the right directory level and provide a depth.
        cd !DEPENDENCY_PATH!\google-breakpad
        for %%A in (!L_LIBS!) do (
            if not exist !L_LIBRELPATH!\%%A (
                set L_FAILED=yes
            )
        )
        
        if "!L_FAILED!" EQU "yes" (
            REM Use gyp to produce VS2013 solution and project files.
            set GYP_MSVS_VERSION=2013
            CALL "!DEPENDENCY_PATH!\gyp\gyp.bat" --no-circular-check client\windows\breakpad_client.gyp --depth .

            REM TODO: The current revision breaks on missing files, and produces partial results, but enough.  Build them piecemeal.
            msbuild /nologo client\Windows\common.vcxproj /p:Configuration=Release /p:Platform=Win32
            msbuild /nologo client\Windows\breakpad_client.sln /p:Configuration=Release /p:Platform=Win32 /t:build_all

            REM Verify that the required static libraries were produced as a result.
            set L_FAILED=no
            for %%A in (!L_LIBS!) do (
                if not exist "!L_LIBRELPATH!\%%A" (
                    echo ERROR.. google-breakpad compilation failed to produce '%%A'
                    set L_FAILED=yes
                )
            )
            REM If one or more libraries were not produced, the user will have been told about each, so now exit.
            if "!L_FAILED!" EQU "yes" (
                echo ERROR.. Giving up as these libraries are required for google-breakpad to work.
                goto internal_function_exit
            )

            REM Copy all the required static libraries.
            for %%A in (!L_LIBS!) do (
                copy "!L_LIBRELPATH!\%%A" "!DEPENDENCY_PATH!\"
            )

            echo SUCCESS.. Enough of google-breakpad was compiled to produce the required static libraries.
            goto user_function_prepare_dependency_breakpad_includes
        ) else (
            echo Building: [google-breakpad] .. skipped
        )
    ) else (
        echo ERROR.. !V_LINK_PARTS[%VCS_NAME%]! not handled by user who wrote the build amendments.
        goto internal_function_exit
    )
)

goto exit_from_user_function_prepare_dependency

:user_function_prepare_dependency_breakpad_includes

cd "!DEPENDENCY_PATH!\google-breakpad"
set L_PATHS[0]=client\windows
set L_PATHS[1]=common
set L_PATHS[2]=google_breakpad
set /A L_PATH_COUNT=3
set /A L_IDX=0

:user_function_make_release_loop1
set L_DIR_PATH=!L_PATHS[%L_IDX%]!
for /F "delims=" %%W in ('dir /B "!L_DIR_PATH!\*.h" 2^>^&1') do (
    if "%%W" NEQ "File Not Found" (
        xcopy /Y /Q >nul "!L_DIR_PATH!\%%W" "!DEPENDENCY_PATH!\include\!L_DIR_PATH!\"
    )
)    
for /F %%V in ('dir /A:D /B "!L_DIR_PATH!"') do (
    if "%%V" NEQ "Release" (
        set L_SUBDIR_PATH=!L_DIR_PATH!\%%V
        set L_PATHS[!L_PATH_COUNT!]=!L_SUBDIR_PATH!
        set /A L_PATH_COUNT=!L_PATH_COUNT!+1
    )
)
set /A L_IDX=!L_IDX!+1
if !L_IDX! LSS !L_PATH_COUNT! goto user_function_make_release_loop1

:exit_from_user_function_prepare_dependency
goto:eof REM return

REM --- FUNCTION: user_function_prepare_dependencies -------------------------
:user_function_prepare_dependencies
REM description: This is called as a final step after all dependencies have been prepared.
REM variable: %DEPENDENCY_PATH% - The absolute path of the dependencies directory.

if exist !UV_INCLUDE_PATH! goto exit_from_user_function_prepare_dependencies

set /A L_COUNT=0
:loop_user_function_prepare_dependencies

if %L_COUNT% LSS %UV_INCLUDE_COMMAND_COUNT% (
    echo Copying includes: !UV_INCLUDE_COMMANDS[%L_COUNT%]!
    xcopy /Q /Y "!DEPENDENCY_PATH!\!UV_INCLUDE_COMMANDS[%L_COUNT%]!" "%UV_INCLUDE_PATH%\"

    set /A L_COUNT=%L_COUNT%+1
    goto loop_user_function_prepare_dependencies
)

:exit_from_user_function_prepare_dependencies
goto:eof REM return

REM --- FUNCTION: user_function_detect_incursion_version ---------------------
:user_function_detect_incursion_version

if "!UV_VERSION!" EQU "" (
    REM Set up the environment so that we can run Incursion piecemeal.
    REM TODO(rmtew): Do this locally, so it doesn't pollute the environment.
    set "PATH=%PATH%;!BUILD_PATH!\dependencies"
    cd "!BUILD_PATH!\run"

    for /F "usebackq tokens=*" %%M in (`..\Win32\Debug\exe_libtcod\Incursion.exe -version`) do (
        set UV_VERSION=%%M
    )

    if "!UV_VERSION!" EQU "" (
        echo ERROR: unable to auto-detect the version of Incursion.
        goto internal_function_teardown
    )
)

goto:eof REM return

REM --- FUNCTION: user_function_make_release ---------------------------------
:user_function_make_release

call :user_function_detect_incursion_version

echo Compiling 'Incursion.Mod' ...

cd "!BUILD_PATH!\run"

set INCURSIONPATH=!BUILD_PATH!\run\
set INCURSIONLIBPATH=!BUILD_PATH!\..\lib
for /F "usebackq tokens=*" %%M in (`..\Win32\Debug\exe_libtcod\Incursion.exe -compile`) do (
    set L_LINE=%%M
	echo %%M
)
set INCURSIONPATH=
set INCURSIONLIBPATH=

if !ERRORLEVEL! NEQ 0 (
    echo ERROR: 'Incursion.exe -compile' exited with error level !ERRORLEVEL!
    goto internal_function_teardown
)

cd "!BUILD_PATH!"
if not exist "run\mod\Incursion.mod" (
    echo "ERROR: Failed building 'Incursion.mod'"
    goto internal_function_teardown
)

if exist "!UV_PACKAGES_DIRNAME!" rmdir /S /Q "!UV_PACKAGES_DIRNAME!"
if not exist "!UV_PACKAGES_DIRNAME!" mkdir "!UV_PACKAGES_DIRNAME!"

REM The naming is important and is dynamically used to determine which files to copy.
for %%A in (release-with-pdbs debug-with-pdbs release) do (
    set L_ARCHIVENAME=Incursion-!UV_VERSION!
    set L_NAME=%%A
    echo Making '%%A' package directory
    set L_PDBS=!L_NAME:~-4,4!
    if "!L_NAME:~0,1!" EQU "r" (
        set L_CONFIG=Release
        if "!L_PDBS!" EQU "pdbs" set L_ARCHIVENAME=!L_ARCHIVENAME!-release-with-pdbs
    ) else (
        set L_CONFIG=Debug
        if "!L_PDBS!" EQU "pdbs" set L_ARCHIVENAME=!L_ARCHIVENAME!-debug-with-pdbs
    )
    set L_NAME=!L_ARCHIVENAME!

    mkdir "!UV_PACKAGES_DIRNAME!\!L_NAME!"
    mkdir "!UV_PACKAGES_DIRNAME!\!L_NAME!\logs"
    mkdir "!UV_PACKAGES_DIRNAME!\!L_NAME!\save"

    xcopy /I /E >nul "run\mod" "!UV_PACKAGES_DIRNAME!\!L_NAME!\mod\"
    xcopy /I /E >nul "!BUILD_SCRIPT_PATH!\fonts" "!UV_PACKAGES_DIRNAME!\!L_NAME!\fonts\"

    copy >nul "!BUILD_SCRIPT_PATH!\LICENSE" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
    copy >nul "!BUILD_SCRIPT_PATH!\Incursion.txt" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
    copy >nul "!BUILD_SCRIPT_PATH!\Changelog.txt" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
    copy >nul "!BUILD_SCRIPT_PATH!\README.md" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
    
    copy >nul "!DEPENDENCY_PATH!\libtcod\build\!L_CONFIG!\libtcod.dll" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
    if "!L_PDBS!" EQU "pdbs" copy >nul "!DEPENDENCY_PATH!\libtcod\build\!L_CONFIG!\libtcod.pdb" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"

    copy >nul "!DEPENDENCY_PATH!\SDL2\VisualC\SDL\Win32\!L_CONFIG!\SDL2.dll" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
    if "!L_PDBS!" EQU "pdbs" copy >nul "!DEPENDENCY_PATH!\SDL2\VisualC\SDL\Win32\!L_CONFIG!\SDL2.pdb" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
    
    if "!UV_PACKAGE_CURSES!" EQU "yes" (
        copy >nul "Win32\!L_CONFIG!\exe_curses\Incursion.exe" "!UV_PACKAGES_DIRNAME!\!L_NAME!\IncursionCurses.exe"
        if "!L_PDBS!" EQU "pdbs" copy >nul "Win32\!L_CONFIG!\exe_curses\Incursion.pdb" "!UV_PACKAGES_DIRNAME!\!L_NAME!\IncursionCurses.pdb"
    )

    copy >nul "Win32\!L_CONFIG!\exe_libtcod\Incursion.exe" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
    if "!L_PDBS!" EQU "pdbs" copy >nul "Win32\!L_CONFIG!\exe_libtcod\Incursion.pdb" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
)

REM Collect the dependencies
echo Making 'dependencies' package directory
set "L_DEPENDENCIES_PATH=!UV_PACKAGES_DIRNAME!\dependencies"
mkdir "!L_DEPENDENCIES_PATH!"
xcopy /I /E  >nul "!DEPENDENCY_PATH!\include" "!L_DEPENDENCIES_PATH!\include\"
copy >nul "!BUILD_SCRIPT_PATH!\LICENSE" "!L_DEPENDENCIES_PATH!\"
copy >nul "!DEPENDENCY_PATH!\*.lib" "!L_DEPENDENCIES_PATH!\"
copy >nul "!DEPENDENCY_PATH!\*.pdb" "!L_DEPENDENCIES_PATH!\"
copy >nul "!DEPENDENCY_PATH!\*.dll" "!L_DEPENDENCIES_PATH!\"
copy >nul "!DEPENDENCY_PATH!\*.exe" "!L_DEPENDENCIES_PATH!\"

:exit_from_user_function_make_release
goto:eof REM return

REM --- FUNCTION: user_function_package_release ------------------------------
:user_function_package_release

call :user_function_detect_incursion_version

cd "!BUILD_PATH!"

REM Verify that the directories exist.
set /A L_FLAG=0
for %%A in (release-with-pdbs debug-with-pdbs release) do (
    set L_ARCHIVENAME=Incursion-!UV_VERSION!
    set L_NAME=%%A
    set L_PDBS=!L_NAME:~-4,4!
    if "!L_PDBS!" EQU "pdbs" (
        if "!L_NAME:~0,1!" EQU "r" (
            set L_ARCHIVENAME=!L_ARCHIVENAME!-release-with-pdbs
        ) else (
            set L_ARCHIVENAME=!L_ARCHIVENAME!-debug-with-pdbs
        )
    )

    if not exist "!UV_PACKAGES_PATH!\!L_ARCHIVENAME!" (
        echo ERROR: Release directory '!L_ARCHIVENAME!' does not exist.
        set /A L_FLAG=1
    )
)
if !L_FLAG! EQU 1 (
    echo Aborting packaging process.
    goto internal_function_teardown
)

REM Archive
cd "!UV_PACKAGES_PATH!"
for %%A in (release-with-pdbs debug-with-pdbs release) do (
    set L_ARCHIVENAME=Incursion-!UV_VERSION!
    set L_NAME=%%A
    set L_PDBS=!L_NAME:~-4,4!
    if "!L_PDBS!" EQU "pdbs" (
        if "!L_NAME:~0,1!" EQU "r" (
            set L_ARCHIVENAME=!L_ARCHIVENAME!-release-with-pdbs
        ) else (
            set L_ARCHIVENAME=!L_ARCHIVENAME!-debug-with-pdbs
        )
    )

    !7Z_EXE! a -r -t7z -mx9 !L_ARCHIVENAME!.7z !L_ARCHIVENAME!
)

REM Archive the dependencies collection.
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set L_DATE=%%c%%a%%b)
!7Z_EXE! a -r -t7z -mx9 build_dependencies-!L_DATE!-only-needed-for-development.7z dependencies\*

cd "!BUILD_PATH!"

:exit_from_user_function_package_release
goto:eof REM return

REM --- FUNCTION: user_function_teardown -------------------------------------
:user_function_teardown
REM description: This is called just before a non-error exit.

goto internal_function_exit

REM --- FUNCTION: internal_function_setup ------------------------------------
:internal_function_setup

REM Ensure that we have a properly set up developer console with access to things like msbuild and devenv.
if not exist "%VS120COMNTOOLS%VsDevCmd.bat" (
    echo You do not appear to have Visual Studio 2013 installed.
    echo The community edition is free, download it and install it.
    pause & exit /b
)
if "%VisualStudioVersion%" EQU "" CALL "%VS120COMNTOOLS%VsDevCmd.bat"
if "%VisualStudioVersion%" NEQ "12.0" (
    echo Your console window has already run the setup for Visual Studio %VisualStudioVersion%.
    echo Open a fresh window and run this script there.  It will run the correct setup.
    pause & exit /b
)

REM The top-level directory.
set BUILD_SCRIPT_PATH=%~dp0
set BUILD_SCRIPT_FILENAME=%~nx0
REM The build sub-directory.
set BUILD_DIRNAME=build
set BUILD_PATH=%BUILD_SCRIPT_PATH%%BUILD_DIRNAME%
REM The dependencies sub-directory.
set DEPENDENCY_DIRNAME=%BUILD_DIRNAME%\dependencies
set DEPENDENCY_PATH=%BUILD_SCRIPT_PATH%%DEPENDENCY_DIRNAME%

REM Allow the user to specify the path to their Mercurial 'hg' executable.  It may have to be accessed via absolute path.
if not defined HG_EXE (
    hg.exe >nul 2>&1 && (set HG_EXE=hg.exe) || (set HG_EXE=)
)

REM Allow the user to specify the path to their Git 'git' executable.  It may have to be accessed via absolute path.
if not defined GIT_EXE (
    REM This doesn't work directly, like hg.exe, so need where.
    where git.exe >nul 2>&1 && (set GIT_EXE=git.exe) || (set GIT_EXE=)
)

REM Allow the user to specify the path to their 7zip '7z' executable.  It may have to be accessed via absolute path.
if not defined 7Z_EXE (
    7z.exe >nul 2>&1 && (set 7Z_EXE=7z.exe) || (set 7Z_EXE=)
    if "!7Z_EXE!" EQU "" (
        if exist "c:\Program Files\7-Zip\7z.exe" set 7Z_EXE="c:\Program Files\7-Zip\7z.exe"
    )
)

if not exist "%DEPENDENCY_PATH%" mkdir "%DEPENDENCY_PATH%"
cd "%DEPENDENCY_PATH%"

REM These variables are used to index the LINKS array entries.
REM
REM               0    1     2      3          4          5
REM HTTP link:   HTTP <url> <NAME> <FILENAME>
REM VCS link:    VCS  <hg>  <NAME> <REVISION> <CLONEURL> <ZIPDLURL>
REM ............ Attempt 1: Identify presence of hg.exe or git.exe, and clone or pull <CLONEURL> <REVISION>
REM ............ Attempt 2: Download <ZIPDLURL><REVISION>.zip as a normal link.

set LINK_CLASSIFIER=0
set HTTP_URL=1
set HTTP_NAME=2
set HTTP_FILENAME=3
set VCS_SYSTEM=1
set VCS_NAME=2
set VCS_REVISION=3
set VCS_CLONEURL=4
set VCS_ZIPDLURL=5

goto user_function_setup 

REM --- FUNCTION: internal_function_main -------------------------------------
:internal_function_main
REM input argument:  V_LINKS   - The user defined links.

if "%1" EQU "" (
    echo Usage: !BUILD_SCRIPT_FILENAME! [OPTION]
    echo.
    echo     -fd, fetch-dependencies    download if necessary
    echo     -pd, prepare-dependencies  extract and/or build, ready for project build
    echo     -mr, make-release          construct release directory for packaging
    echo     -pr, package-release       compress and archive release directory
    echo.
    echo     -d, dependencies           fetch and prepare the dependencies
    echo     -p, project                build this project using the dependencies
    echo     -r, release                construct/compress/archive built project
    echo.
    if not defined HG_EXE (
    echo WARNING: 'hg.exe' cannot be located.  Mercurial may not be installed.
    echo   This script can operate without it, but that mode is less supported.
    echo   If Mercurial is not in PATH, you can set HG_EXE to full filename of 'hg.exe'
    echo.
    )
    if not defined GIT_EXE (
    echo WARNING: 'git.exe' cannot be located.  Git may not be installed.
    echo   This script can operate without it, but that mode is less supported.
    echo   If Git is not in PATH, you can set GIT_EXE to full filename of 'git.exe'
    echo.
    )

    goto internal_function_teardown
)

set V_COMMANDS[fetch-dependencies]=fetch-dependencies
set V_COMMANDS[prepare-dependencies]=prepare-dependencies
set V_COMMANDS[make-release]=make-release
set V_COMMANDS[package-release]=package-release
set V_COMMANDS[dependencies]=dependencies
set V_COMMANDS[project]=project
set V_COMMANDS[release]=release
set V_COMMANDS[-fd]=fetch-dependencies
set V_COMMANDS[-pd]=prepare-dependencies
set V_COMMANDS[-mr]=make-release
set V_COMMANDS[-pr]=package-release
set V_COMMANDS[-d]=dependencies
set V_COMMANDS[-p]=project
set V_COMMANDS[-r]=release

:parse_args
if "%~1" EQU "" goto parse_args_end
set L_COMMAND=!V_COMMANDS[%~1]!
if "!L_COMMAND!" EQU "" (
    echo !BUILD_SCRIPT_FILENAME!: command %1 unrecognised.
    echo.
    echo Type '!BUILD_SCRIPT_FILENAME!' to see valid options.
    goto internal_function_teardown
)
set V_COMMAND[!L_COMMAND!]=yes
shift
goto parse_args

:parse_args_end

REM Ensure sub-steps are selected for the more general commands.
if "!V_COMMAND[dependencies]!" EQU "yes" (
    set V_COMMAND[fetch-dependencies]=yes
    set V_COMMAND[prepare-dependencies]=yes
)

if "!V_COMMAND[release]!" EQU "yes" (
    set V_COMMAND[make-release]=yes
    set V_COMMAND[package-release]=yes
)

REM Do the selected general commands.
if "!V_COMMAND[fetch-dependencies]!" EQU "yes" (
    call :internal_function_fetch_dependencies
)

if "!V_COMMAND[prepare-dependencies]!" EQU "yes" (
    call :internal_function_prepare_dependencies
)

if "!V_COMMAND[make-release]!" EQU "yes" (
    call :internal_function_make_release
)

if "!V_COMMAND[package-release]!" EQU "yes" (
    call :internal_function_package_release
)

goto internal_function_teardown

REM --- FUNCTION: internal_function_make_release -----------------------------
:internal_function_make_release

cd "%DEPENDENCY_PATH%"
call :user_function_make_release

goto:eof REM return

REM --- FUNCTION: internal_function_package_release --------------------------
:internal_function_package_release

if "!7Z_EXE!" EQU "" (
    echo ERROR: Packaging a release requires 7zip to be installed.
    goto internal_function_teardown
)

call :user_function_package_release

goto:eof REM return

REM --- FUNCTION: internal_function_prepare_dependencies ---------------------
:internal_function_prepare_dependencies
REM input argument:  V_LINKS   - The user defined links.

set /A IDX_PDS=0
:loop_internal_function_prepare_dependencies
set V_LINK=!LINKS[%IDX_PDS%]!
if "!V_LINK!" EQU "" goto loop_internal_function_prepare_dependencies_break

REM function call: V_LINK_PARTS = split_link(V_LINK)
call :internal_function_split_link

REM function call: prepare_dependency(V_LINK_PARTS)
call :internal_function_prepare_dependency

set /A IDX_PDS=!IDX_PDS!+1
goto loop_internal_function_prepare_dependencies

:loop_internal_function_prepare_dependencies_break
call :user_function_prepare_dependencies

goto:eof REM return

REM --- FUNCTION: internal_function_prepare_dependency -----------------------
:internal_function_prepare_dependency
REM input argument:  V_LINK_PARTS   - The processed link parts to make use of.
REM output argument: V_LINK_PARTS   - The array of elements that make up the given link text.

REM               0    1     2      3          4          5
REM HTTP link:   HTTP <url> <NAME> <FILENAME>
REM VCS link:    VCS  <hg>  <NAME> <REVISION> <CLONEURL> <ZIPDLURL>
REM ............ Attempt 1: Identify presence of hg.exe or git.exe, and clone or pull <CLONEURL> <REVISION>
REM ............ Attempt 2: Download <ZIPDLURL><REVISION>.zip as a normal link.

if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "http" (
    REM Environment variables for powershell script.
    set fn=Archive-Extract
    set "fnp0=%DEPENDENCY_PATH%\!V_LINK_PARTS[%HTTP_FILENAME%]!"
    set "fnp1=%DEPENDENCY_PATH%"
    REM Environment variables for function 'user_function_prepare_dependency'.
    set V_DIRNAME=
    set V_SKIPPED=no

    REM Iterate over the lines of output.
    for /F "usebackq tokens=*" %%i in (`more "%BUILD_SCRIPT_PATH%%BUILD_SCRIPT_FILENAME%" ^| powershell -c -`) do (
        set L_LINE=%%i
        if "!L_LINE:~0,12!" EQU "EXTRACTPATH:" (
            set V_DIRNAME=!L_LINE:~13!
        ) else if "!L_LINE:~0,4!" EQU "MSG:" (
            if "!L_LINE:~5!" EQU "EXTRACTED" (
                echo Decompressing: [!V_LINK_PARTS[%HTTP_FILENAME%]!] .. skipped
                set V_SKIPPED=yes
            ) else if "!L_LINE:~5!" EQU "EXTRACTING" (
                echo Decompressing: [!V_LINK_PARTS[%HTTP_FILENAME%]!]
            )
        ) else (
            echo Unexpected result: !L_LINE!
        )
    )
    REM Do we process the output as it arrives, or at the end?
    REM echo Decompressed?: [!V_LINK_PARTS[%HTTP_FILENAME%]!]
) else if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "vcs" (
    set V_DIRNAME=!V_LINK_PARTS[%VCS_NAME%]!
    REM Stop errors from nothing being here.
)
call :user_function_prepare_dependency
cd %DEPENDENCY_PATH%

goto:eof REM return

REM --- FUNCTION: internal_function_fetch_dependencies --------------------------
:internal_function_fetch_dependencies
REM input argument:  V_LINKS        - The processed link parts to make use of.

set /A V_IDX_FD=0
:loop_internal_function_fetch_dependencies
set V_LINK=!LINKS[%V_IDX_FD%]!
if "!V_LINK!" EQU "" goto exit_from_internal_function_fetch_dependencies

REM function call: V_LINK_PARTS = split_link(V_LINK)
call :internal_function_split_link

REM function call: download_link(V_LINK_PARTS)
call :internal_function_fetch_dependency

set /A V_IDX_FD=!V_IDX_FD!+1
goto loop_internal_function_fetch_dependencies

:exit_from_internal_function_fetch_dependencies
goto:eof REM return

REM --- FUNCTION: internal_function_fetch_dependency ---------------------------
:internal_function_fetch_dependency
REM input argument:  V_LINK_PARTS   - The processed link parts to make use of.
REM input argument:  V_IDX_FD       - The index into the links array of the current dependency.
REM output argument: V_LINK_PARTS   - The array of elements that make up the given link text.

:loop_internal_function_fetch_dependency
cd %DEPENDENCY_PATH%

if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "vcs" (
    set L_VCS_DESC=
    if "!V_LINK_PARTS[%VCS_SYSTEM%]!" EQU "hg" (
        set L_VCS_DESC=Mercurial
        set L_VCS_TEST_NAME=.hg
        set L_VCS_EXE=!HG_EXE!
        set L_VCS_CMD_CLONE=!HG_EXE! clone "!V_LINK_PARTS[%VCS_CLONEURL%]!" !V_LINK_PARTS[%VCS_NAME%]!
        set L_VCS_CMD_PULL=!HG_EXE! pull
        set L_VCS_CMD_UPDATE=!HG_EXE! update -r "!V_LINK_PARTS[%VCS_REVISION%]!" -C
    ) else if "!V_LINK_PARTS[%VCS_SYSTEM%]!" EQU "git" (
        set L_VCS_DESC=Git
        set L_VCS_TEST_NAME=.git
        set L_VCS_EXE=!GIT_EXE!
        set L_VCS_CMD_CLONE=!GIT_EXE! clone "!V_LINK_PARTS[%VCS_CLONEURL%]!" !V_LINK_PARTS[%VCS_NAME%]! --no-checkout -b master
        set L_VCS_CMD_PULL=!GIT_EXE! pull origin master
        set L_VCS_CMD_UPDATE=!GIT_EXE! checkout !V_LINK_PARTS[%VCS_REVISION%]! -q
    )

    if "!L_VCS_DESC!" NEQ "" (
        REM Compute whether to use the VCS system or not.
        if "!L_VCS_EXE!" NEQ "" (
            set L_USE_VCS=yes
            REM Presence of directory, but lack of tested sub-directory, indicates zip archive source.
            if exist "!V_LINK_PARTS[%VCS_NAME%]!" (
                if not exist "!V_LINK_PARTS[%VCS_NAME%]!\!L_VCS_TEST_NAME!" (
                    echo WARNING: While !L_VCS_DESC! is now installed, existing [!V_LINK_PARTS[%VCS_NAME%]!] directory does not use it.
                    set L_USE_VCS=no
                )
            )
        ) else (
            set L_USE_VCS=no
        )
        if "!L_USE_VCS!" EQU "yes" (
            REM Has the repository already been cloned?  If so, reuse it.
            echo Fetching: [!V_LINK_PARTS[%VCS_NAME%]!] !L_VCS_DESC! repository.
            if not exist "!V_LINK_PARTS[%VCS_NAME%]!" (
                REM Does not exist, fetch it.
                for /F "usebackq tokens=*" %%i in (`!L_VCS_CMD_CLONE!`) do (
                    echo .. %L_VCS_EXE%: %%i
                )
                REM The subsequent VCS update needs to be within the repository directory.
                cd !V_LINK_PARTS[%VCS_NAME%]!
            ) else (
                REM The VCS pull and subsequent update needs to be within the repository directory.
                cd !V_LINK_PARTS[%VCS_NAME%]!
                for /F "usebackq tokens=*" %%i in (`!L_VCS_CMD_PULL!`) do (
                    echo .. %L_VCS_EXE%: %%i
                )
            )
            echo Updating: [!V_LINK_PARTS[%VCS_NAME%]!] !L_VCS_DESC! repository to revision [!V_LINK_PARTS[%VCS_REVISION%]!].
            for /F "usebackq tokens=*" %%i in (`!L_VCS_CMD_UPDATE!`) do (
                echo .. %L_VCS_EXE%: %%i
            )
            goto exit_from_internal_function_fetch_dependency
        ) else (
            if exist "!V_LINK_PARTS[%VCS_NAME%]!" (
                if exist "!V_LINK_PARTS[%VCS_NAME%]!\!L_VCS_TEST_NAME!" (
                    echo WARNING: No !L_VCS_DESC! client installed yet [!V_LINK_PARTS[%VCS_NAME%]!] was originally created using one.
                    echo WARNING: Therefore [!V_LINK_PARTS[%VCS_NAME%]!] cannot be updated.
                    goto exit_from_internal_function_fetch_dependency
                )
            )
            REM Fall back to ZIP archive downloading.  Ordering of these assignments is important as some overwrite others.
            echo WARNING: This is untested code.  If it does not work, report it.
            set V_LINK_PARTS[%HTTP_NAME%]=!V_LINK_PARTS[%VCS_NAME%]!
            set V_LINK_PARTS[%LINK_CLASSIFIER%]=http
            echo Download of VCS repo snapshots not fully implemented, giving up.
            goto internal_function_exit
            REM TODO: Set this to !V_LINK_PARTS[%VCS_ZIPDLURL%]! with 'REV' replaced with !V_LINK_PARTS[%VCS_REVISION%]!
            set V_LINK_PARTS[%HTTP_URL%]=!V_LINK_PARTS[%VCS_ZIPDLURL%]!
            REM TODO: Set this to the extracted file name from the HTTP_URL we just constructed.
            set V_LINK_PARTS[%HTTP_FILENAME%]=
        )
    )
)

REM Skip logic to keep script flat, as not possible to have label inside if statement block.
if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" NEQ "http" goto exit_from_internal_function_fetch_dependency

set /A L_ATTEMPTS=0
:internal_function_fetch_dependency_retry

if not exist "!V_LINK_PARTS[%HTTP_NAME%]!" (
    echo Downloading: [!V_LINK_PARTS[%HTTP_NAME%]!] 
    powershell -c "Start-BitsTransfer -source !V_LINK_PARTS[%HTTP_URL%]!"
    
    if not exist !V_LINK_PARTS[%HTTP_FILENAME%]! (
        echo Failed to download !V_LINK_PARTS[%HTTP_FILENAME%]!
        goto internal_function_exit
    )
) else (
    echo Downloading: [!V_LINK_PARTS[%HTTP_NAME%]!] .. skipped
)
set /A L_ATTEMPTS=!L_ATTEMPTS!+1

call :internal_function_checksum

if "!V_PASSED!" EQU "yes" (
    echo .. MD5 checksum [!V_CHECKSUM!] correct
) else if "!V_PASSED!" EQU "no" (
    echo .. MD5 checksum [!V_CHECKSUM!] incorrect
    echo .. Expected: [!MD5CHECKSUMS[%V_IDX_FD%]!]

    if !L_ATTEMPTS! EQU 1 (
        set /p L_RESULT="Attempt to re-download the file [Y/n]?"
        if /I "!L_RESULT!" NEQ "n" (
            del !V_LINK_PARTS[%HTTP_FILENAME%]!
            goto internal_function_fetch_dependency_retry
        )
    )
    
    echo ERROR: Failed to obtain valid copy of [!V_LINK_PARTS[%HTTP_FILENAME%]!]
    goto internal_function_exit
) else (
    echo .. MD5 checksum [!V_CHECKSUM!] unknown
)

:exit_from_internal_function_fetch_dependency
goto:eof REM return

REM --- FUNCTION: internal_function_checksum ---------------------------------

:internal_function_checksum
REM input argument:  V_LINK_PARTS   - The array of elements relating to the link in question.
REM output argument: V_CHECKSUM     - The calculated checksum for the given file.
REM output argument: V_PASSED       - If there is one to match, "yes" for correct and "no" for incorrect.
REM                                   If there is not one to match, "".

set fn=MD5-Checksum
set fnp0=!V_LINK_PARTS[%HTTP_FILENAME%]!
set fnp1=discard

set V_PASSED=

REM Iterate over the lines of output.
for /F "usebackq tokens=*" %%i in (`more "%BUILD_SCRIPT_PATH%%BUILD_SCRIPT_FILENAME%" ^| powershell -c -`) do (
    set L_LINE=%%i
    if "!L_LINE:~0,4!" EQU "MSG:" (
        set V_CHECKSUM=!L_LINE:~5!
        set L_CHECKSUM=!MD5CHECKSUMS[%V_IDX_FD%]!
        if "!L_CHECKSUM!" NEQ "" (
            if "!L_CHECKSUM!" EQU "!V_CHECKSUM!" (
                set V_PASSED=yes
            ) else (
                set V_PASSED=no
            )
        )
    ) else (
        echo Unexpected result in checksum function: !L_LINE!
        goto internal_function_exit
    )
)

:exit_from_internal_function_checksum
goto:eof REM return

REM --- FUNCTION: internal_function_verify_link ------------------------------

:internal_function_verify_link
REM input argument:  V_LINK         - The link text to verify.
REM output argument: V_LINK_PARTS   - The array of elements that make up the given link text.

REM function call: V_LINK_PARTS = split_link(V_LINK)
call :internal_function_split_link

if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "http" (
    if /I "!V_LINK_PARTS[%HTTP_URL%]:~0,4!" NEQ "http" goto internal_function_verify_link__valid
    if /I "!V_LINK_PARTS[%HTTP_URL%]:~-4,4!" NEQ ".zip" goto internal_function_verify_link__valid
) else if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "vcs" (
    if "!V_LINK_PARTS[%VCS_SYSTEM%]!" EQU "hg" goto internal_function_verify_link__valid
)

REM emergency exit with error message.
echo Invalid link: %V_LINK%
goto internal_function_exit

:internal_function_verify_link__valid
goto:eof REM return

REM --- FUNCTION: internal_function_split_link -------------------------------
:internal_function_split_link
REM input argument:  V_LINK         - The link text to verify.
REM output argument: V_LINK_PARTS   - The array of elements that make up the given link text.

set V_LINK_PARTS[%LINK_CLASSIFIER%]=
for /F "tokens=1,2,3,4,5,6" %%A in ("!V_LINK!") do (
    set L_FIRST=%%A
    if /I "!L_FIRST:~0,7!" EQU "http://" (
        set V_LINK_PARTS[%LINK_CLASSIFIER%]=http
        set V_LINK_PARTS[%HTTP_URL%]=!V_LINK_PARTS[%%A]!

        REM .. V_LINK_PARTS[%HTTP_NAME%], V_LINK_PARTS[%HTTP_FILENAME%] = get_urlfilename(V_LINK)
        call :internal_function_get_urlfilename
		
		set V_LINK_PARTS[%HTTP_NAME%]=!V_RESULT!
		set V_LINK_PARTS[%HTTP_FILENAME%]=!V_RESULT!
    ) else (
        set V_LINK_PARTS[%LINK_CLASSIFIER%]=%%A
        set V_LINK_PARTS[%VCS_SYSTEM%]=%%B
        set V_LINK_PARTS[%VCS_NAME%]=%%C
        set V_LINK_PARTS[%VCS_REVISION%]=%%D
        set V_LINK_PARTS[%VCS_CLONEURL%]=%%E
        set V_LINK_PARTS[%VCS_ZIPDLURL%]=%%F
    )
)

goto:eof REM return

REM --- FUNCTION: internal_function_get_urlfilename -------------------------------
:internal_function_get_urlfilename
REM input argument:  V_LINK         - The link text to verify.
REM output argument: V_RESULT       - The base filename the URL exposes for download.

set L_LINK=!V_LINK!
:loop_internal_function_get_urlfilename_1
if "!L_LINK!" EQU "" goto loop0continue
REM Select the substring up to the first path separator.
for /f "delims=/" %%M in ("!L_LINK!") do set L_SUBSTRING=%%M

:loop_internal_function_get_urlfilename_2
REM Skip until the next path separator.
set L_CHAR=!L_LINK:~0,1!
set L_LINK=!L_LINK:~1!
if "!L_LINK!" EQU "" goto loop_internal_function_get_urlfilename_3
if "!L_CHAR!" NEQ "/" goto loop_internal_function_get_urlfilename_2
goto loop_internal_function_get_urlfilename_1

:loop_internal_function_get_urlfilename_3
REM We have the trailing string after the last path separator, or the file name.
set V_RESULT=!L_SUBSTRING!

goto:eof REM return

REM --- Everything is done, exit back to the user ----------------------------

:internal_function_teardown
REM Now that processing is done, allow the user to do some steps before exiting.
goto user_function_teardown

:internal_function_exit
REM Leave the user in the directory they were in to begin with.
cd %BUILD_SCRIPT_PATH%

REM endlocal: Ensure environment variables are left the same as when the script started.
REM exit /b:  Exit the script, but do not close any DOS window it was run from within.
endlocal & exit /b
#>

function MD5-Checksum([string]$path, [string]$discard) {
    # $fullPath = Resolve-Path $path;
    $md5 = new-object -TypeName System.Security.Cryptography.MD5CryptoServiceProvider;
    $file = [System.IO.File]::Open($path, [System.IO.Filemode]::Open, [System.IO.FileAccess]::Read);
    $hash = [System.BitConverter]::ToString($md5.ComputeHash($file))
    Write-Host "MSG: $hash";
    $file.Dispose();
    return 0;
}

function Archive-Extract([string]$zipFilePath, [string]$destinationPath) {
    # This will get added when paths are joined, and path comparison will need it to be absent.
    $destinationPath = $destinationPath.TrimEnd("\");

    [Reflection.Assembly]::LoadWithPartialName('System.IO.Compression.FileSystem') > $null;
    $zipfile = [IO.Compression.ZipFile]::OpenRead($zipFilePath);

    # Determine how many top level entries there are.
    $ziplevel0files = @{};
    $zipfile.Entries | foreach {
        $s = ($_.FullName.TrimEnd("/") -split "/")[0];
        if ($ziplevel0files.ContainsKey($s)) {
            $ziplevel0files[$s] = $ziplevel0files[$s] + 1;
        } else {
            $ziplevel0files[$s] = 0;
        }
    }

    if ($ziplevel0files.count -ne 1) {
        $zipDirName = [io.path]::GetFileNameWithoutExtension($zipFilePath)
        $zipDirPath = $destinationPath = Join-Path -Path $destinationPath -ChildPath $zipDirName
    } else {
        $zipDirName = $ziplevel0files.Keys[0]
        $zipDirPath = Join-Path -Path $destinationPath -ChildPath $zipDirName;
    }

    Write-Host "EXTRACTPATH: $zipDirName";
    if (Test-Path -LiteralPath $zipDirPath) {
        Write-Host "MSG: EXTRACTED";
        return 2; # Failure
    }
    Write-Host "MSG: EXTRACTING";
    
    $zipfile.Entries | foreach {
        $extractFilePath = Join-Path -Path $destinationPath -ChildPath $_.FullName;
        $extractFileDirPath = Split-Path -Parent $extractFilePath;

        if (-not (Test-Path -LiteralPath $extractFileDirPath -PathType Container)) {
            New-Item -Path $extractFileDirPath -Type Directory | Out-Null;
        }

        # Sometimes a directory comes after a file within the directory (the latter causes it to be created implicitly above).
        if (-not $extractFilePath.EndsWith("\")) {
            try {
                [IO.Compression.ZipFileExtensions]::ExtractToFile($_, $extractFilePath, $true);
            } catch {
                Write-Host "MSG: Failed to extract file:" $extractFilePath;
                return 3; # Failure
            }
        }
    }
    return 0; # Success
}

# Anything that calls should execute the powershell and set the parameters.

$fn = (Get-ChildItem Env:fn).Value;
$arg0 = (Get-ChildItem Env:fnp0).Value;
$arg1 = (Get-ChildItem Env:fnp1).Value;
$err = & $fn $arg0 $arg1;
exit $err;
