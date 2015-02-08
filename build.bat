<# : 
@echo off
setlocal EnableDelayedExpansion
 
REM TODO: Executing this script with no arguments should do some evaluation and display a status and short help information.
REM TODO: potential argument <upto> - If present with a stage name, will do all stages leading up to that stage name.
REM TODO: potential argument <bootstrap|bs> - Fetch project source code if not already present with this script.
REM TODO: potential argument <fetch-dependencies|fd> - Fetch dependencies if not already done.
REM TODO: potential argument <prepare-dependencies|pd> - Extract and generally prepare fetched dependencies if not already done.
REM TODO: potential argument <all|a> - Attempt to build this project's source code.
REM TODO: potential argument <run|r> - Attempt to run this projects's source code.

REM Note: If a BAT script calls something else, which modifies environment variables
REM       then when that something else exits, the BAT script will still have the
REM       same variables it already had - as it gets them at startup and by direct
REM       execution.

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

REM Divert to the internal setup code, it will return to the user setup.
goto internal_function_setup

REM --- FUNCTION: user_function_setup ----------------------------------------
:user_function_setup

set LINKS[0]=http://jaist.dl.sf.net/pub/sourceforge/w/wi/winflexbison/win_flex_bison-latest.zip
REM set LINKS[1]=vcs hg SDL2 704a0bfecf75 http://hg.libsdl.org/SDL http://hg.libsdl.org/SDL/archive/
set LINKS[1]=vcs hg SDL2 704a0bfecf75 C:\RMT\VCS\HG\libraries\SDL http://hg.libsdl.org/SDL/archive/
REM set LINKS[2]=vcs hg libtcod 7a8b072365b5 https://bitbucket.org/jice/libtcod https://bitbucket.org/jice/libtcod/get/
set LINKS[2]=vcs hg libtcod 7a8b072365b5 C:\RMT\VCS\HG\libraries\libtcod https://bitbucket.org/jice/libtcod/get/

REM Process the user data, calling event functions when applicable.
goto internal_function_main

REM --- FUNCTION: user_function_prepare_dependency --------------------------------
:user_function_prepare_dependency
REM variable: %V_LINK_PARTS% - The link data.
REM variable: %DEPENDENCY_PATH% - The absolute path of the dependencies directory.
REM variable: %V_DIRNAME% - The relative directory name the dependency can be found in.
REM variable: %V_SKIPPED% - 'yes' or 'no', depending on whether the archive was already extracted.
set V_LABEL_RETURN_ifpd=!V_LABEL_RETURN!

cd !DEPENDENCY_PATH!

if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "http" (
    if "!V_LINK_PARTS[%HTTP_FILENAME%]!" EQU "win_flex_bison-latest.zip" (
        REM Extract the pre-built executable and put in the right location.
        if "%V_SKIPPED%" equ "no" (
            if exist "%BUILD_PATH%flex.exe" del %BUILD_PATH%flex.exe        
        )
        if not exist "%BUILD_PATH%flex.exe" (
            cd !V_DIRNAME!
            copy win_flex.exe %BUILD_PATH%flex.exe
        )
    ) else (
        echo ERROR.. !V_LINK_PARTS[%HTTP_FILENAME%]! not handled by user who wrote the build amendments.
    )
) else if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "vcs" (
    set L_VCS_NAME=!V_LINK_PARTS[%VCS_NAME%]!
    if "!L_VCS_NAME!" EQU "SDL2" (
        cd !V_DIRNAME!

        if not exist !DEPENDENCY_PATH!\SDL2_d.dll (
            echo Building: [SDL2/Debug]

            msbuild /nologo VisualC\SDL_VS2013.sln /p:Configuration=Debug /p:Platform=Win32 /t:SDL2,SDL2main
            copy VisualC\SDL\Win32\Debug\SDL2.dll !DEPENDENCY_PATH!\SDL2_d.dll
            copy VisualC\SDL\Win32\Debug\SDL2.lib !DEPENDENCY_PATH!\SDL2_d.lib
            copy VisualC\SDL\Win32\Debug\SDL2.pdb !DEPENDENCY_PATH!\SDL2_d.pdb
        ) else (
            echo Building: [SDL2/Debug] .. SKIPPED
        )

        if not exist !DEPENDENCY_PATH!\SDL2.dll (
            echo Building: [SDL2/Release]

            msbuild /nologo VisualC\SDL_VS2013.sln /p:Configuration=Release /p:Platform=Win32 /t:SDL2,SDL2main
            copy VisualC\SDL\Win32\Release\SDL2.dll !DEPENDENCY_PATH!\SDL2.dll
            copy VisualC\SDL\Win32\Release\SDL2.lib !DEPENDENCY_PATH!\SDL2.lib
            copy VisualC\SDL\Win32\Release\SDL2.pdb !DEPENDENCY_PATH!\SDL2.pdb
        ) else (
            echo Building: [SDL2/Release] .. SKIPPED
        )
    ) else if "!L_VCS_NAME!" EQU "libtcod" (
        cd !V_DIRNAME!

REM  C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\CL.exe /c /I"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\\..\src\png" /I"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\\..\src\zlib" /I"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\\..\include" /I"$SolutionDir)\..\..\..\sdl2\include" /ZI /nologo /W3 /WX- /Od /Oy- /D TCOD_SDL2 /D NO_OPENGL /D LIBTCOD_EXPORTS /D _WINDLL /D _MBCS /Gm /EHsc /RTC1 /MTd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Fo"Debug\\" /Fd"Debug\vc120.pdb" /Gd /TC /analyze- /errorReport:queue ..\src\bresenham_c.c ..\src\bsp_c.c ..\src\color_c.c ..\src\console_c.c ..\src\fov_c.c ..\src\fov_circular_raycasting.c ..\src\fov_diamond_raycasting.c ..\src\fov_permissive2.c ..\src\fov_recursive_shadowcasting.c ..\src\fov_restrictive.c ..\src\heightmap_c.c ..\src\image_c.c ..\src\lex_c.c ..\src\list_c.c ..\src\mersenne_c.c ..\src\namegen_c.c ..\src\noise_c.c ..\src\parser_c.c ..\src\path_c.c ..\src\sys_c.c ..\src\sys_opengl_c.c ..\src\sys_sdl2_c.c ..\src\sys_sdl_c.c ..\src\sys_sdl_img_bmp.c ..\src\sys_sdl_img_png.c ..\src\tree_c.c ..\src\txtfield_c.c ..\src\zip_c.c

REM   C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\link.exe /ERRORREPORT:QUEUE /OUT:"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\Debug\libtcod.dll" /NOLOGO /LIBPATH:"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\Debug\\" /LIBPATH:"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\..\..\sdl2\VisualC\SDL\Debug" libz.lib png.lib sdl2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NODEFAULTLIB:libcmtd /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /DEBUG /PDB:"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\Debug\libtcod.pdb" /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\Debug\libtcod.lib" /MACHINE:X86 /SAFESEH /DLL Debug\bresenham_c.obj

REM  C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\CL.exe /c /ZI /nologo /W3 /WX- /Od /Oy- /D _MBCS /Gm /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Fo"Debug\\" /Fd"Debug\vc120.pdb" /Gd /TC /analyze- /errorReport:queue ..\src\zlib\adler32.c ..\src\zlib\compress.c ..\src\zlib\crc32.c ..\src\zlib\deflate.c ..\src\zlib\gzclose.c ..\src\zlib\gzlib.c ..\src\zlib\gzread.c ..\src\zlib\gzwrite.c ..\src\zlib\infback.c ..\src\zlib\inffast.c ..\src\zlib\inflate.c ..\src\zlib\inftrees.c ..\src\zlib\trees.c ..\src\zlib\uncompr.c ..\src\zlib\zutil.c

REM  C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\CL.exe /c /ZI /nologo /W3 /WX- /Od /Oy- /D _MBCS /Gm /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Fo"Debug\\" /Fd"Debug\vc120.pdb" /Gd /TC /analyze- /errorReport:queue ..\src\png\lodepng.c
REM   C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\Lib.exe /OUT:"C:\RMT\VCS\GIT\Roguelike\incursion-roguelike\_dependencies\libtcod\makefiles\Debug\png.lib" /NOLOGO Debug\lodepng.obj
   
        REM msbuild /nologo VisualC\SDL_VS2013.sln /p:Configuration=Debug /p:Platform=Win32 /t:SDL2,SDL2main
        echo Build libtcod and copy the right shit into place.
    ) else (
        echo ERROR.. !V_LINK_PARTS[%HTTP_FILENAME%]! not handled by user who wrote the build amendments.
    )
)

:exit_from_internal_function_prepare_dependency
goto !V_LABEL_RETURN_ifpd!

REM User function: user_function_teardown
REM Description: Called as a final step before the script exits.
:user_function_teardown



goto return_from_user_function_teardown

REM --- Script defined values ------------------------------------------------

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

set BUILD_PATH=%~dp0
set BUILD_SCRIPT_FILENAME=%~nx0
set DEPENDENCY_DIRNAME=dependencies
set DEPENDENCY_PATH=%BUILD_PATH%%DEPENDENCY_DIRNAME%

REM Allow the user to specify the path to their 'hg' executable.  It may have to be accessed via absolute path.
if not defined HG_EXE (
    hg.exe >nul 2>&1 && (set HG_EXE=hg.exe) || (set HG_EXE=)
)

if not exist "%DEPENDENCY_PATH%" mkdir %DEPENDENCY_PATH%
cd %DEPENDENCY_PATH%

goto user_function_setup 

REM --- FUNCTION: internal_function_main -------------------------------------
:internal_function_main
REM is this function re-entrant? No.
REM
REM input argument:  V_LINKS   - The user defined links.

REM fetch-dependencies
set V_LABEL_RETURN=return_to_internal_function_main_FD
goto internal_function_fetch_dependencies
:return_to_internal_function_main_FD

REM prepare-dependencies
set V_LABEL_RETURN=return_to_internal_function_main_PD
goto internal_function_prepare_dependencies
:return_to_internal_function_main_PD

goto internal_function_teardown

REM --- FUNCTION: internal_function_prepare_dependencies ---------------------

:internal_function_prepare_dependencies
REM is this function re-entrant? No.
REM
REM expectation: 
REM Process the user defined links.
set V_LABEL_RETURN_ifpds=!V_LABEL_RETURN!

set /A IDX_PDS=0
:loop_internal_function_prepare_dependencies
set V_LINK=!LINKS[%IDX_PDS%]!
if "!V_LINK!" EQU "" goto exit_from_internal_function_prepare_dependencies

REM function call: V_LINK_PARTS = split_link(V_LINK)
set V_LABEL_RETURN=return_to_internal_function_prepare_dependencies1
goto internal_function_split_link
:return_to_internal_function_prepare_dependencies1

REM function call: prepare_dependency(V_LINK_PARTS)
set V_LABEL_RETURN=return_to_internal_function_prepare_dependencies2
goto internal_function_prepare_dependency
:return_to_internal_function_prepare_dependencies2

set /A IDX_PDS=!IDX_PDS!+1
goto loop_internal_function_prepare_dependencies

:exit_from_internal_function_prepare_dependencies
goto !V_LABEL_RETURN_ifpds!

REM --- FUNCTION: internal_function_prepare_dependency -----------------------
:internal_function_prepare_dependency
REM is this function re-entrant? No.
REM
REM input argument:  V_LINK_PARTS   - The processed link parts to make use of.
REM input argument:  V_LABEL_RETURN - The name of the label to return to when finished.
REM output argument: V_LINK_PARTS   - The array of elements that make up the given link text.
set V_LABEL_RETURN_iffd=!V_LABEL_RETURN!

REM               0    1     2      3          4          5
REM HTTP link:   HTTP <url> <NAME> <FILENAME>
REM VCS link:    VCS  <hg>  <NAME> <REVISION> <CLONEURL> <ZIPDLURL>
REM ............ Attempt 1: Identify presence of hg.exe or git.exe, and clone or pull <CLONEURL> <REVISION>
REM ............ Attempt 2: Download <ZIPDLURL><REVISION>.zip as a normal link.

if "!V_LINK_PARTS[0]!" EQU "http" (
    REM Environment variables for powershell script.
    set fn=Archive-Extract
    set fnp0=%DEPENDENCY_PATH%\!V_LINK_PARTS[3]!
    set fnp1=%DEPENDENCY_PATH%
    REM Environment variables for function 'user_function_prepare_dependency'.
    set V_DIRNAME=
    set V_SKIPPED=no

    REM Iterate over the lines of output.
    for /F "usebackq tokens=*" %%i in (`more "%BUILD_PATH%%BUILD_SCRIPT_FILENAME%" ^| powershell -c -`) do (
        set L_LINE=%%i
        if "!L_LINE:~0,12!" EQU "EXTRACTPATH:" (
            set V_DIRNAME=!L_LINE:~13!
        ) else if "!L_LINE:~0,4!" EQU "MSG:" (
            if "!L_LINE:~5!" EQU "EXTRACTED" (
                echo Decompressing: [!V_LINK_PARTS[3]!] .. SKIPPED
                set V_SKIPPED=yes
            ) else if "!L_LINE:~5!" EQU "EXTRACTING" (
                echo Decompressing: [!V_LINK_PARTS[3]!]
            )
        ) else (
            echo Unexpected result: !L_LINE!
        )
    )
    REM Do we process the output as it arrives, or at the end?
    REM echo Decompressed?: [!V_LINK_PARTS[3]!]
) else if "!V_LINK_PARTS[0]!" EQU "vcs" (
    set V_DIRNAME=!V_LINK_PARTS[%VCS_NAME%]!
    REM Stop errors from nothing being here.
)
set V_LABEL_RETURN=return_to_internal_function_prepare_dependency
goto user_function_prepare_dependency
:return_to_internal_function_prepare_dependency
cd %DEPENDENCY_PATH%

:exit_from_internal_function_prepare_dependency
goto !V_LABEL_RETURN_iffd!

REM --- FUNCTION: internal_function_fetch_dependencies --------------------------

:internal_function_fetch_dependencies
REM is this function re-entrant? No.
REM
REM input argument:  V_LINKS        - The processed link parts to make use of.
REM XXXX input argument:  V_LINK_PARTS   - The array of elements that make up the given link text.
REM input argument:  V_LABEL_RETURN - The name of the label to return to when finished.
REM XXX output argument: V_LINK_PARTS   - The array of elements that make up the given link text.
set V_LABEL_RETURN_ifdls=!V_LABEL_RETURN!

set /A IDX_FD=0
:loop_internal_function_fetch_dependencies
set V_LINK=!LINKS[%IDX_FD%]!
if "!V_LINK!" EQU "" goto exit_from_internal_function_fetch_dependencies

REM function call: V_LINK_PARTS = split_link(V_LINK)
set V_LABEL_RETURN=return_to_internal_function_fetch_dependencies1
goto internal_function_split_link
:return_to_internal_function_fetch_dependencies1

REM function call: download_link(V_LINK_PARTS)
set V_LABEL_RETURN=return_to_internal_function_fetch_dependencies2
goto internal_function_fetch_dependency
:return_to_internal_function_fetch_dependencies2

set /A IDX_FD=!IDX_FD!+1
goto loop_internal_function_fetch_dependencies

:exit_from_internal_function_fetch_dependencies
goto !V_LABEL_RETURN_ifdls!

REM --- FUNCTION: internal_function_fetch_dependency ---------------------------

:internal_function_fetch_dependency
REM is this function re-entrant? No.
REM
REM input argument:  V_LINK_PARTS   - The processed link parts to make use of.
REM input argument:  V_LABEL_RETURN - The name of the label to return to when finished.
REM output argument: V_LINK_PARTS   - The array of elements that make up the given link text.
set V_LABEL_RETURN_iffd=!V_LABEL_RETURN!

:loop_internal_function_fetch_dependency
cd %DEPENDENCY_PATH%

if "!V_LINK_PARTS[0]!" EQU "vcs" (
    if "!V_LINK_PARTS[1]!" EQU "hg" (
        if defined HG_EXE (
            REM Has the repository already been cloned?  If so, reuse it.
            echo Fetching: [!V_LINK_PARTS[2]!] mercurial repository.
            if not exist "!V_LINK_PARTS[2]!" (
                REM Does not exist, fetch it.
                for /F "usebackq tokens=*" %%i in (`hg clone "!V_LINK_PARTS[4]!" !V_LINK_PARTS[2]!`) do (
                    echo .. %HG_EXE%: %%i
                )                
                cd !V_LINK_PARTS[2]!
            ) else (
                cd !V_LINK_PARTS[2]!
                for /F "usebackq tokens=*" %%i in (`hg pull`) do (
                    echo .. %HG_EXE%: %%i
                )
            )
            echo Updating: [!V_LINK_PARTS[2]!] mercurial repository to revision [!V_LINK_PARTS[3]!].
            for /F "usebackq tokens=*" %%i in (`hg update -r "!V_LINK_PARTS[3]!" -C`) do (
                echo .. %HG_EXE%: %%i
            )
            goto exit_from_internal_function_fetch_dependency
        ) else (
            REM Fall back to ZIP archive downloading.
            REM TODO: What if someone installs hg after downloading and using the snapshot?
            REM = name
            set V_LINK_PARTS[2]=!V_LINK_PARTS[2]! r!V_LINK_PARTS[3]! !V_LINK_PARTS[1]! snapshot
            rem = classifier
            set V_LINK_PARTS[0]=http
            REM = filename
            set V_LINK_PARTS[3]=!V_LINK_PARTS[3]!.zip
            REM = url
            set V_LINK_PARTS[1]=!V_LINK_PARTS[5]!!V_LINK_PARTS[3]!
        )
    )
)

if "!V_LINK_PARTS[0]!" EQU "http" (
    if not exist "!V_LINK_PARTS[2]!" (
        echo Downloading: [!V_LINK_PARTS[2]!] 
        echo powershell -c "Start-BitsTransfer -source !V_LINK_PARTS[1]!"
    ) else (
        echo Downloading: [!V_LINK_PARTS[2]!] .. SKIPPED
    )
)

:exit_from_internal_function_fetch_dependency
goto !V_LABEL_RETURN_iffd!

REM --- FUNCTION: internal_function_verify_link ------------------------------

:internal_function_verify_link
REM is this function re-entrant? No.
REM
REM input argument:  V_LINK         - The link text to verify.
REM input argument:  V_LABEL_RETURN - The name of the label to return to when finished.
REM output argument: V_LINK_PARTS   - The array of elements that make up the given link text.
set V_LABEL_RETURN_ifvl=!V_LABEL_RETURN!

REM function call: V_LINK_PARTS = split_link(V_LINK)
set V_LABEL_RETURN=return_to_internal_function_verify_link
goto internal_function_split_link
:return_to_internal_function_verify_link

if %V_LINK_PARTS[0]% EQU "http" (
    if /I %V_LINK_PARTS[1]:~0,4% NEQ "http" goto internal_function_verify_link__valid
    if /I %V_LINK_PARTS[1]:~-4,4% NEQ ".zip" goto internal_function_verify_link__valid
) else if %V_LINK_PARTS[0]% EQU "vcs" (
    if %V_LINK_PARTS[1]% EQU "hg" goto internal_function_verify_link__valid
)

REM emergency exit with error message.
echo Invalid link: %V_LINK%
goto return_from_user_function_teardown

:internal_function_verify_link__valid

goto !V_LABEL_RETURN_ifvl!

REM --- FUNCTION: internal_function_split_link -------------------------------

:internal_function_split_link
REM is this function re-entrant? No.
REM
REM input argument:  V_LINK         - The link text to verify.
REM input argument:  V_LABEL_RETURN - The name of the label to return to when finished.
REM output argument: V_LINK_PARTS   - The array of elements that make up the given link text.
set V_LABEL_RETURN_ifsl=!V_LABEL_RETURN!

set V_LINK_PARTS[0]=
for /F "tokens=1,2,3,4,5,6" %%A in ("!V_LINK!") do (
    set V_LINK_PARTS[0]=%%A
    set V_LINK_PARTS[1]=%%B
    set V_LINK_PARTS[2]=%%C
    set V_LINK_PARTS[3]=%%D
    set V_LINK_PARTS[4]=%%E
    set V_LINK_PARTS[5]=%%F
)

if /I "!V_LINK_PARTS[0]:~0,7!" EQU "http://" (
    set V_LINK_PARTS[1]=!V_LINK_PARTS[0]!
    set V_LINK_PARTS[0]=http

    REM function call: V_LINK_PARTS[2] = get_urlfilename(V_LINK)
    set V_LABEL_RETURN=return_to_internal_function_split_link
    goto internal_function_get_urlfilename

) else (
    goto internal_function_split_link__loop_exit
)

:return_to_internal_function_split_link
set V_LINK_PARTS[2]=!V_RESULT!
set V_LINK_PARTS[3]=!V_RESULT!

:internal_function_split_link__loop_exit
goto !V_LABEL_RETURN_ifsl!

REM --- FUNCTION: internal_function_split_link -------------------------------

:internal_function_get_urlfilename
REM is this function re-entrant? No.
REM
REM input argument:  V_LINK         - The link text to verify.
REM input argument:  V_LABEL_RETURN - The name of the label to return to when finished.
REM output argument: V_RESULT       - The base filename the URL exposes for download.
set V_LABEL_RETURN_ifgu=!V_LABEL_RETURN!

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

goto !V_LABEL_RETURN_ifgu!

REM --- Everything is done, exit back to the user ----------------------------

:internal_function_teardown
REM Now that processing is done, allow the user to do some steps before exiting.
goto user_function_teardown

:return_from_user_function_teardown
REM Leave the user in the directory they were in to begin with.
cd %BUILD_PATH%

REM endlocal: Ensure environment variables are left the same as when the script started.
REM exit /b:  Exit the script, but do not close any DOS window it was run from within.
endlocal & exit /b
#>

function MD5-Checksum([string]$path) {
    // $fullPath = Resolve-Path $path;
    $md5 = new-object -TypeName System.Security.Cryptography.MD5CryptoServiceProvider;
    $file = [System.IO.File]::Open($fullPath,[System.IO.Filemode]::Open, [System.IO.FileAccess]::Read);
    [System.BitConverter]::ToString($md5.ComputeHash($file));
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
