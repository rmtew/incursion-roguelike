<# : 
@echo off
setlocal EnableDelayedExpansion

REM ** This script is intended to automate all parts of the following:
REM - Fetching, extracting and building all dependencies.
REM - Building the solution.
REM - Making and packaging the releases.

REM ** Dependencies:
REM - Git, if any of the dependencies are fetched from a git repository.
REM - Mercurial, if any of the dependencies are fetched from a mercurial repository.

REM TODO:
REM - fetch-dependencies:
REM -- Download the 'make-release'/'package-release' dependencies from bitbucket.
REM - prepare-dependencies:
REM -- Take the downloaded dependencies and extract them in the right location.

REM Divert to the internal setup code, it will return to the user setup.
goto internal_function_setup

REM --- FUNCTION: user_function_setup ----------------------------------------
REM This function allows the user to define global variables, check for their own
REM dependencies and set internal script values which need to be initialised before
REM the script starts work.
:user_function_setup

REM So I can have this fetch from local repositories, rather than use expensive bandwidth.
set UV_RMTEW_DEVMODE=no

REM Internal variable: LINKS
REM
REM This variable defines what dependencies the script should download on behalf the user.
REM The user is responsible for preparing these dependencies in 'user_function_prepare_dependency'.
REM
REM __ LINKS entries are either URLs to ZIP archives, or a collection of version control (vcs) information.
REM      LINKS[n]=<url to zip>
REM      LINKS[n]=vcs <vcs-system> <name> <revision-id> <repo-path> <snapshot-url>
REM        <revision-id>: This can be a URL, or it can be a filesystem path to an existing local clone.

set LINKS[0]=http://jaist.dl.sf.net/pub/sourceforge/w/wi/winflexbison/win_flex_bison-latest.zip
set LINKS[1]=vcs hg libtcod 7a8b072365b5 https://bitbucket.org/jice/libtcod https://bitbucket.org/jice/libtcod/get/REV.zip
set LINKS[2]=http://jaist.dl.sf.net/pub/sourceforge/p/pd/pdcurses/pdcurses/3.4/pdcurs34.zip
set LINKS[3]=vcs git gyp dd831fd https://chromium.googlesource.com/external/gyp https://chromium.googlesource.com/external/gyp/REV.tar.gz
set LINKS[4]=vcs git google-breakpad 0a0ad99 https://chromium.googlesource.com/external/google-breakpad/src/ https://chromium.googlesource.com/external/google-breakpad/src/REV.tar.gz
set LINKS[5]=0

if "!UV_RMTEW_DEVMODE!" equ "yes" (
	set LINKS[1]=vcs hg libtcod default C:\RMT\VCS\HG\libraries\libtcod-bitbucket INVALID
	set LINKS[3]=vcs git gyp dd831fd C:\RMT\VCS\GIT\Tools\gyp INVALID
	set LINKS[4]=vcs git google-breakpad 0a0ad99 C:\RMT\VCS\GIT\Libraries\google-breakpad INVALID
)

REM Internal variable: MD5CHECKSUMS
REM
REM This variable is used to specify known MD5 checksums for dependency file downloads.  If you do
REM not provide it, then when the downloaded file is checked, the calculated checksum will be
REM displayed and noted as unchecked.  The checksum can then be entered here, and it will ensure all
REM later copies of the file are the expected file.

set MD5CHECKSUMS[0]=BB-C5-10-F0-91-34-2F-CF-5B-A7-A2-BC-A3-97-FD-A1
set MD5CHECKSUMS[3]=D6-3A-85-FD-5D-E9-60-11-71-02-87-4C-AB-5A-EB-90

set UV_INCLUDE_PATH=!DEPENDENCY_PATH!\include

REM Our user handling of the 'prepare-dependencies' command indexes all the include files from
REM the range of dependencies we prepare, and as a post step in 'user_function_prepare_dependencies'
REM we copy them into our dependency folder.
set UV_INCLUDE_COMMANDS[0]=
set /A UV_INCLUDE_COMMAND_COUNT=0

REM Where the 'make-release'/'package-release' command outputs are placed.
set UV_PACKAGES_DIRNAME=packages
set UV_PACKAGES_PATH=!BUILD_PATH!\packages

REM The pdcurses dependency is used to build a Curses-based UI for the game.  It generally works,
REM but the reason we do not distribute it is that the console font lacks the range of characters
REM which we currently use.  Until that can be rectified, this is sidelined.
set UV_PACKAGE_CURSES=no

REM Allow the user to specify the path to their Python 'python' executable.  It may have to be
REM accessed via absolute path.  This is required for running gyp, which is used to generate
REM solutions and projects for Google Breakpad.  Note that the building of Breakpad on Windows is
REM broken and has been for a long time, see the prepare code for details.
if not defined PYTHON_EXE (
    set "PATH=C:\Python27;%PATH%"
    python.exe --help >nul 2>&1 && (set PYTHON_EXE=python.exe) || (set PYTHON_EXE=)
)

REM Internal variable: UV_COMMANDS
REM
REM Entries in this list are used by the internal functions to handle user arguments to the
REM script.  We use it to have the script automate compiling the scripts.
REM
REM Long argument / short argument / internal argument / <before|after> / function name to call
set UV_COMMANDS[0]=compile-scripts -cs build-project after user_function_compile_scripts
set UV_COMMANDS[1]=

REM Internal variable: UV_COMMAND_DESCS
REM
REM Provides a short description for each user argument.  These are injected into the command
REM listing that is displayed when the user just types 'build'.
set UV_COMMAND_DESCS[compile-scripts]=compile Incursion script module

REM We are done.
goto internal_function_main

REM --- FUNCTION: user_function_fetch_dependencies --------------------------------
:user_function_fetch_dependencies
(
    setlocal EnableDelayedExpansion
	
	cd !DEPENDENCY_PATH!
	if "!UV_RMTEW_DEVMODE!" equ "yes" (
		set SDL2LINK=vcs hg SDL2 704a0bfecf75 C:\RMT\VCS\HG\libraries\SDL http://hg.libsdl.org/SDL/archive/
	)
	CALL libtcod\build.bat -fd
)
(
    endlocal
    exit /b
)

REM --- FUNCTION: user_function_prepare_dependency --------------------------------
:user_function_prepare_dependency
REM variable: %V_LINK_PARTS% - The link data.
REM variable: %DEPENDENCY_PATH% - The absolute path of the dependencies directory.
REM variable: %V_DIRNAME% - The relative directory name the dependency can be found in.
REM variable: %V_SKIPPED% - 'yes' or 'no', depending on whether the archive was already extracted.
(
REM    setlocal EnableDelayedExpansion

	cd !DEPENDENCY_PATH!

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
			
			for %%P in (Win32) do (
				if not exist "!DEPENDENCY_PATH!\%%P" mkdir "!DEPENDENCY_PATH!\%%P"
				for %%C in (Debug Release) do (
					if not exist "!DEPENDENCY_PATH!\%%P\%%C" mkdir "!DEPENDENCY_PATH!\%%P\%%C"
					if not exist "!DEPENDENCY_PATH!\%%P\%%C\pdcurses.lib" (
						echo Building: [pdcurses^|%%C^|%%P]
						nmake -f vcwin32.mak clean
						if "%%C" equ "Debug" (
							nmake -f vcwin32.mak DEBUG=1 WIDE=1 DLLOPT=/MTd pdcurses.lib
						) else if "%%C" equ "Release" (
							nmake -f vcwin32.mak WIDE=1 DLLOPT=/MT pdcurses.lib
						) else (
							echo ERROR
						)
						copy pdcurses.lib "!DEPENDENCY_PATH!\%%P\%%C\pdcurses.lib"
					) else (
						echo Building: [pdcurses^|%%C^|%%P] .. skipped
					)
				)
			)

			set UV_INCLUDE_COMMANDS[!UV_INCLUDE_COMMAND_COUNT!]=!V_DIRNAME!\*.h
			set /A UV_INCLUDE_COMMAND_COUNT=!UV_INCLUDE_COMMAND_COUNT!+1

			cd !DEPENDENCY_PATH!
		) else (
			echo ERROR.. !V_LINK_PARTS[%HTTP_FILENAME%]! not handled by user who wrote the build amendments.
			goto internal_function_exit
		)
	) else if "!V_LINK_PARTS[%LINK_CLASSIFIER%]!" EQU "vcs" (
		set L_VCS_NAME=!V_LINK_PARTS[%VCS_NAME%]!
		if "!L_VCS_NAME!" EQU "libtcod" (
			REM Delegate libtcod compilation to it's 'build.bat' script.
			cd "!DEPENDENCY_PATH!"
			CALL libtcod\build.bat -pd
			CALL libtcod\build.bat -p

			REM Copy compiled litcod dependency binaries.
			REM Copy compiled litcod binaries.
			for %%P in (Win32 x64) do (
				if not exist %%P mkdir %%P
				for %%C in (Debug Release) do (
					if not exist %%P\%%C mkdir %%P\%%C
					for %%S in (dll pdb lib) do (
						copy >nul libtcod\build\dependencies\%%P\%%C\SDL2.%%S %%P\%%C\
						copy >nul libtcod\build\msvs\libtcod\%%P\%%C\libtcod.%%S %%P\%%C\
					)
				)
			)

			REM Mark the gathered libtcod dependency includes to be copied.
			set UV_INCLUDE_COMMANDS[!UV_INCLUDE_COMMAND_COUNT!]=libtcod\build\dependencies\include\*.h
			set /A UV_INCLUDE_COMMAND_COUNT=!UV_INCLUDE_COMMAND_COUNT!+1

			REM Mark the libtcod includes to be copied.
			set UV_INCLUDE_COMMANDS[!UV_INCLUDE_COMMAND_COUNT!]=libtcod\include\*.h
			set /A UV_INCLUDE_COMMAND_COUNT=!UV_INCLUDE_COMMAND_COUNT!+1
		) else if "!L_VCS_NAME!" EQU "gyp" (
			REM gyp is built using Python, so no building is required.
			echo Building: [gyp] .. skipped
		) else if "!L_VCS_NAME!" EQU "google-breakpad" (
			set L_LIBS=exception_handler.lib crash_generation_client.lib common.lib

			REM gyp is a bit flakey, and requires at least at this time, we be at the right directory level and provide a depth.
			cd !DEPENDENCY_PATH!\google-breakpad
			set L_FAILED=no
			for %%A in (!L_LIBS!) do (
				for %%P in (Win32 x64) do (			
					for %%C in (Debug Release) do (			
						if not exist "!DEPENDENCY_PATH!\%%P\%%C\%%A" set L_FAILED=yes
					)
				)
			)
			
			if "!L_FAILED!" EQU "yes" (
				REM Use gyp to produce VS2013 solution and project files.
				set GYP_MSVS_VERSION=2013
				CALL >nul "!DEPENDENCY_PATH!\gyp\gyp.bat" --no-circular-check client\windows\breakpad_client.gyp --depth .

				for %%P in (Win32 x64) do (			
					for %%C in (Debug Release) do (
						set L_FAILED=no
						for %%A in (!L_LIBS!) do (
							if not exist "!DEPENDENCY_PATH!\%%P\%%C\%%A" set L_FAILED=yes
						)
					
						if "!L_FAILED!" EQU "yes" (
							echo Building: [google-breakpad^|%%C^|%%P]

							REM TODO: The current revision breaks on missing files, and produces partial results, but enough.  Build them piecemeal.
							DEL >nul /S /Q client\Windows\%%C
							set L_ERROR_MSG=
							for /F "usebackq tokens=*" %%i in (`msbuild /nologo client\Windows\common.vcxproj /p:Configuration^=%%C /p:Platform^=%%P`) do (
								set L_LINE=%%i
								if "!L_LINE:fatal error=!" NEQ "!L_LINE!" set L_ERROR_MSG=%%i
							)
							set L_ERROR_MSG=
							for /F "usebackq tokens=*" %%i in (`msbuild /nologo client\Windows\breakpad_client.sln /p:Configuration^=%%C /p:Platform^=%%P /t:build_all`) do (
								set L_LINE=%%i
								if "!L_LINE:fatal error=!" NEQ "!L_LINE!" set L_ERROR_MSG=%%i
							)

							REM Verify that the required static libraries were produced as a result.
							set L_FAILED=no
							for %%A in (!L_LIBS!) do (
								if not exist "client\Windows\%%C\lib\%%A" (
									echo Checking [google-breakpad^|%%P]: .. ERROR, failed to produce '%%A'
									set L_FAILED=yes
								)
							)
							REM If one or more libraries were not produced, the user will have been told about each, so now exit.
							if "!L_FAILED!" EQU "yes" (
								echo ERROR.. Giving up as these libraries are required for google-breakpad to work.
								goto internal_function_exit
							)
							
							for %%A in (!L_LIBS!) do (
								if not exist "!DEPENDENCY_PATH!\%%P\%%C\%%A" copy >nul "client\Windows\%%C\lib\%%A" "!DEPENDENCY_PATH!\%%P\%%C\"
							)
						)
					)
				)
			) else (
				echo Building: [google-breakpad] .. skipped
			)
		) else (
			echo ERROR.. !V_LINK_PARTS[%VCS_NAME%]! not handled by user who wrote the build amendments.
			goto internal_function_exit
		)
	)
)
( 
    REM endlocal
    exit /b
)

:user_function_prepare_dependency_breakpad_includes
(
    setlocal EnableDelayedExpansion

	cd "!DEPENDENCY_PATH!\google-breakpad"

	set L_PATHS[0]=client\windows
	set L_PATHS[1]=common
	set L_PATHS[2]=google_breakpad
	REM The code below will extend the array.
	set /A L_PATH_COUNT=3
	set /A L_IDX=0

	REM Ensure L_IDX is usable by jumping into the loop. o_O
	goto uf_loop_pdbi

	REM Recursively copy the subset of the directory tree that is include file locations (and of course the include files there).
:uf_loop_pdbi
	set "L_DIR_PATH=!L_PATHS[%L_IDX%]!"
	REM Copy this directory, if there are include files present.
	for /F "delims=" %%W in ('dir /B "!L_DIR_PATH!\*.h" 2^>^&1') do (
		if "%%W" NEQ "File Not Found" (
			xcopy /Y /Q >nul "!L_DIR_PATH!\%%W" "!DEPENDENCY_PATH!\include\!L_DIR_PATH!\"
		)
	)    
	REM Index subdirectories of this directory for subsequent processing.
	for /F %%V in ('dir /A:D /B "!L_DIR_PATH!"') do (
		if "%%V" NEQ "Release" (
			set L_SUBDIR_PATH=!L_DIR_PATH!\%%V
			set L_PATHS[!L_PATH_COUNT!]=!L_SUBDIR_PATH!
			set /A L_PATH_COUNT=!L_PATH_COUNT!+1
		)
	)
	
	set /A L_IDX=!L_IDX!+1
	if !L_IDX! LSS !L_PATH_COUNT! goto uf_loop_pdbi
)
( 
    endlocal
    exit /b
)

REM --- FUNCTION: user_function_prepare_dependencies -------------------------
:user_function_prepare_dependencies
REM description: This is called as a final step after all dependencies have been prepared.
REM variable: %DEPENDENCY_PATH% - The absolute path of the dependencies directory.
(
    setlocal EnableDelayedExpansion

	if not exist "!UV_INCLUDE_PATH!" (
		call :user_function_prepare_dependency_breakpad_includes

		set /A L_END=!UV_INCLUDE_COMMAND_COUNT!-1
		for /L %%I in (0, 1, !L_END!) do (
			echo Copying: includes #%%I "!UV_INCLUDE_COMMANDS[%%I]!"
			xcopy >nul /Q /Y "!DEPENDENCY_PATH!\!UV_INCLUDE_COMMANDS[%%I]!" "%UV_INCLUDE_PATH%\"
		)
	)
)
( 
    endlocal
    exit /b
)

REM --- FUNCTION: user_function_build_project --------------------------------
:user_function_build_project
(
    setlocal EnableDelayedExpansion

	REM Compile incursion.

	REM TODO: Add %%P entry x64
	for %%N in (exe_libtcod exe_curses) do (
		for %%P in (Win32) do (
			REM pdcurses only has Win32 support.
			if "%%P%%N" neq "x64exe_curses" (
				for %%C in (Debug Release) do (
					echo Building: [%%N^|%%C^|%%P]

					set L_ERROR_MSG=
					for /F "usebackq tokens=*" %%i in (`msbuild /nologo msvs\libtcod.sln /p:Configuration^=%%C /p:Platform^=%%P /t:%%N`) do (
						set L_LINE=%%i
						if "!L_LINE:fatal error=!" NEQ "!L_LINE!" set L_ERROR_MSG=%%i
					)

					if not exist "!BUILD_PATH!\%%P\%%C\%%N\Incursion.exe" (
						echo ERROR.. '%%P\%%C\%%N\Incursion.exe' did not successfully build for some reason.
						goto internal_function_exit
					)
				)
			)
		)
	)
)
( 
    endlocal
    exit /b
)

REM --- FUNCTION: user_function_compile_scripts ----------------------------------------
:user_function_compile_scripts
(   
    setlocal EnableDelayedExpansion

	REM Script compiling needs to know where to locate the dependency dlls.
	set "PATH=%PATH%;!BUILD_PATH!\dependencies\Win32\Debug"
	REM Script compiling will write to a 'mod' directory here.
	set "INCURSIONPATH=!BUILD_PATH!\run"
	REM Script compiling will read the scripts it needs to compile.
	set "INCURSIONLIBPATH=!BUILD_SCRIPT_PATH!lib"
	REM Not sure this is needed.
	cd "!BUILD_PATH!\run"

	echo Compiling Incursion scripts..

	set "L_MODFILE_PATH=!BUILD_PATH!\run\mod\Incursion.Mod"
	if exist "!L_MODFILE_PATH!" del >nul /Q "!L_MODFILE_PATH!"

	for /F "usebackq tokens=*" %%M in (`..\Win32\Debug\exe_libtcod\Incursion.exe -compile`) do (
		set LAST_LINE=%%M
		echo !LAST_LINE!
	)
	
	if not exist "!L_MODFILE_PATH!" (
		echo ERROR: failed compiling Incursion script files..
		goto internal_function_teardown
	)
)
( 
    endlocal
    exit /b
)

REM --- FUNCTION: user_function_detect_incursion_version ---------------------
:user_function_detect_incursion_version <returnVal>
(   
    setlocal EnableDelayedExpansion

	if "!UV_VERSION!" EQU "" (
		REM Set up the environment so that we can run Incursion piecemeal.
		set "PATH=%PATH%;!BUILD_PATH!\dependencies\Win32\Debug"
		cd "!BUILD_PATH!\run"

		for /F "usebackq tokens=*" %%M in (`..\Win32\Debug\exe_libtcod\Incursion.exe -version`) do (
			set UV_VERSION=%%M
		)

		if "!UV_VERSION!" EQU "" (
			echo ERROR: unable to auto-detect the version of Incursion.
			goto internal_function_teardown
		)
	)
)
( 
    endlocal
    set "%~1=%UV_VERSION%"
    exit /b
)

REM --- FUNCTION: user_function_make_release ---------------------------------
:user_function_make_release
(   
    setlocal EnableDelayedExpansion

	call :user_function_detect_incursion_version UV_VERSION

	cd "!BUILD_PATH!"

	if not exist "!BUILD_PATH!\run\mod\Incursion.Mod" (
		echo ERROR: Please compile the Incursion scripts first..
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

		REM Set up the game directory structure.
		mkdir "!UV_PACKAGES_DIRNAME!\!L_NAME!"
		mkdir "!UV_PACKAGES_DIRNAME!\!L_NAME!\logs"
		mkdir "!UV_PACKAGES_DIRNAME!\!L_NAME!\save"

		xcopy /I /E >nul "run\mod" "!UV_PACKAGES_DIRNAME!\!L_NAME!\mod\"
		xcopy /I /E >nul "!BUILD_SCRIPT_PATH!\fonts" "!UV_PACKAGES_DIRNAME!\!L_NAME!\fonts\"

		copy >nul "!BUILD_SCRIPT_PATH!\LICENSE" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
		copy >nul "!BUILD_SCRIPT_PATH!\Incursion.txt" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
		copy >nul "!BUILD_SCRIPT_PATH!\Changelog.txt" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
		copy >nul "!BUILD_SCRIPT_PATH!\README.md" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"

		REM Copy built binaries and dependencies into place.
		copy >nul "!DEPENDENCY_PATH!\Win32\!L_CONFIG!\libtcod.dll" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
		if "!L_PDBS!" EQU "pdbs" copy >nul "!DEPENDENCY_PATH!\Win32\!L_CONFIG!\libtcod.pdb" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"

		copy >nul "!DEPENDENCY_PATH!\Win32\!L_CONFIG!\SDL2.dll" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
		if "!L_PDBS!" EQU "pdbs" copy >nul "!DEPENDENCY_PATH!\Win32\!L_CONFIG!\SDL2.pdb" "!UV_PACKAGES_DIRNAME!\!L_NAME!\"
		
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
	xcopy /I /E  >nul "!DEPENDENCY_PATH!\Win32" "!L_DEPENDENCIES_PATH!\Win32\"
	REM xcopy /I /E  >nul "!DEPENDENCY_PATH!\x64" "!L_DEPENDENCIES_PATH!\x64\"
	copy >nul "!BUILD_SCRIPT_PATH!\LICENSE" "!L_DEPENDENCIES_PATH!\"
	copy >nul "!DEPENDENCY_PATH!\*.exe" "!L_DEPENDENCIES_PATH!\"
)
( 
    endlocal
    exit /b
)

REM --- FUNCTION: user_function_package_release ------------------------------
:user_function_package_release
(   
    setlocal EnableDelayedExpansion

	call :user_function_detect_incursion_version UV_VERSION

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
	for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set L_DATE=%%c%%b%%a)
	!7Z_EXE! a -r -t7z -mx9 build_dependencies-!L_DATE!-only-needed-for-development.7z dependencies\*
)
( 
    endlocal
	cd "!BUILD_PATH!"
    exit /b
)

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
REM input argument:  V_LINKS     - The user defined links.
REM input argument:  UV_COMMANDS - The user defined script arguments / function handlers.

REM Unit commands.
set V_COMMANDS[fetch-dependencies]=fetch-dependencies
set V_COMMANDS[prepare-dependencies]=prepare-dependencies
set V_COMMANDS[build-project]=build-project
set V_COMMANDS[make-release]=make-release
set V_COMMANDS[package-release]=package-release
REM Composite commands.
set V_COMMANDS[dependencies]=dependencies
set V_COMMANDS[release]=release

REM Unit commands.
set V_COMMANDS[-fd]=fetch-dependencies
set V_COMMANDS[-pd]=prepare-dependencies
set V_COMMANDS[-bp]=build-project
set V_COMMANDS[-mr]=make-release
set V_COMMANDS[-pr]=package-release
REM Composite commands.
set V_COMMANDS[-d]=dependencies
set V_COMMANDS[-r]=release

REM Iterable list of unit command names.
set V_FUNCTION_COMMANDS=fetch-dependencies prepare-dependencies make-release package-release build-project

REM Map unit command names to function names.
set V_FUNCTIONS[fetch-dependencies]=internal_function_fetch_dependencies
set V_FUNCTIONS[prepare-dependencies]=internal_function_prepare_dependencies
set V_FUNCTIONS[make-release]=internal_function_make_release
set V_FUNCTIONS[package-release]=internal_function_package_release
set V_FUNCTIONS[build-project]=internal_function_build_project

REM Add in the user commands.
call :internal_func_index_user_commands

if "%1" EQU "" (
    echo Usage: !BUILD_SCRIPT_FILENAME! [OPTION]
    echo.
    echo     -fd, fetch-dependencies    download if necessary
    echo     -pd, prepare-dependencies  extract and/or build, ready for project build
    echo     -bp, build-project         build this project using the dependencies
    echo     -mr, make-release          construct release directory for packaging
    echo     -pr, package-release       compress and archive release directory
    echo.
    echo     -d, dependencies           fetch and prepare the dependencies
    echo     -r, release                construct/compress/archive built project
	REM List user defined commands last.
	if "!V_USER_COMMANDS!" NEQ "" (
	echo.
		for %%C in (!V_USER_COMMANDS!) do (
			set V_LONG=%%C
			set V_DESC=!UV_COMMAND_DESCS[%%C]!
			set V_SHORT=!V_USER_COMMAND_SHORT_NAME[%%C]!
			REM Everything following up to the echo is formatting the output line.
			call :internal_function_strlen V_DESCLEN V_DESC
			call :internal_function_strlen V_SHORTLEN V_SHORT
			call :internal_function_strlen V_LONGLEN V_LONG
			REM 27 is the width of the composite argument name column
			set /A "V_SPACE_USED=!V_SHORTLEN!+!V_LONGLEN!"
			set /A "V_SPACE_WASTED=27-2-!V_SPACE_USED!"
			set V_SPACER=
			for /L %%I in (1,1,!V_SPACE_WASTED!) do set V_SPACER= !V_SPACER!
	echo     !V_SHORT!, %%C!V_SPACER!!V_DESC!
		)
	)
    if not defined HG_EXE (
	echo.
	echo WARNING: 'hg.exe' cannot be located.  Mercurial may not be installed.
	echo   This script can operate without it, but that mode is less supported.
	echo   If Mercurial is not in PATH, you can set HG_EXE to full filename of 'hg.exe'
    )
    if not defined GIT_EXE (
	echo.
	echo WARNING: 'git.exe' cannot be located.  Git may not be installed.
	echo   This script can operate without it, but that mode is less supported.
	echo   If Git is not in PATH, you can set GIT_EXE to full filename of 'git.exe'
    )

    goto internal_function_teardown
)

REM Compare the user given arguments to those we have indexed.
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

REM Set up the user command handlers.
for %%C in (!V_USER_COMMANDS!) do (
	if "!V_COMMAND[%%C]!" NEQ "" (
		set V_LINK_KEY=!V_USER_COMMAND_LINK[%%C]!-!V_USER_COMMAND_WHEN[%%C]!
		set V_LINK_VALUE=!V_USER_COMMAND_FUNCTION[%%C]!
		if "!V_COMMAND_LINK[%V_LINK_KEY%]!" EQU "" (
			set V_COMMAND_LINK[!V_LINK_KEY!]=!V_LINK_VALUE!
		) else (
			set V_COMMAND_LINK[!V_LINK_KEY!]=!V_COMMAND_LINK[%V_LINK_KEY%]! !V_LINK_VALUE!
		)
	)
)

REM Handle the internal arguments and linked user command handlers.
for %%C in (!V_FUNCTION_COMMANDS!) do (
	REM Call linked user command handlers for before this internal argument stage.
	for %%L in (!V_COMMAND_LINK[%%C-before]!) do call :%%L
	REM If an internal argument is given, call it's associated function.
	if "!V_COMMAND[%%C]!" EQU "yes" call :!V_FUNCTIONS[%%C]!
	REM Call linked user command handlers for after this internal argument stage.
	for %%L in (!V_COMMAND_LINK[%%C-after]!) do call :%%L
)

goto internal_function_teardown

REM --- FUNCTION: internal_func_index_user_commands -----------------------------
:internal_func_index_user_commands

set /A IDX_UV=0
:loop_uv_commands

set V_UV_COMMAND_DEFN=!UV_COMMANDS[%IDX_UV%]!
if "!V_UV_COMMAND_DEFN!" EQU "" goto loop_uv_commands_break
call :internal_func_index_user_command  !UV_COMMANDS[%IDX_UV%]!
set /A IDX_UV=!IDX_UV!+1
goto loop_uv_commands

:loop_uv_commands_break
goto:eof REM return

REM --- FUNCTION: internal_func_index_user_command ------------------------------
:internal_func_index_user_command
REM %1: long argument
REM %2: short argument
REM %3: internal long argument
REM %4: whether to handle <before|after> internal long argument
REM %5: user function name to call

REM Detect if the user argument clashes with an internal one.
if "!V_COMMANDS[%1]!" NEQ "" (
	echo ERROR.. User command name %1 not suitable.
	goto internal_function_exit	
)

REM Enter the user arguments as valid arguments.
set V_COMMANDS[%1]=%1
set V_COMMANDS[%2]=%1

REM Construct an iterable list of user argument names.
if "!V_USER_COMMANDS!" EQU "" (
	set V_USER_COMMANDS=%1
) else (
	set V_USER_COMMANDS=!V_USER_COMMANDS! %1
)

REM Index the other pieces of information to the given user command name.
set V_USER_COMMAND_LINK[%1]=%3
set V_USER_COMMAND_WHEN[%1]=%4
set V_USER_COMMAND_FUNCTION[%1]=%5
set V_USER_COMMAND_SHORT_NAME[%1]=%2

goto:eof REM return

REM --- FUNCTION: internal_function_build_project ----------------------------
:internal_function_build_project
(   
    setlocal EnableDelayedExpansion

	cd "!BUILD_PATH!"
	call :user_function_build_project
)
( 
    endlocal
    exit /b
)

REM --- FUNCTION: internal_function_make_release -----------------------------
:internal_function_make_release
(   
    setlocal EnableDelayedExpansion

	cd "%DEPENDENCY_PATH%"
	call :user_function_make_release
)
( 
    endlocal
    exit /b
)

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

call :user_function_fetch_dependencies

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

REM --- FUNCTION: internal_function_strlen -------------------------------
REM Taken from http://stackoverflow.com/a/5841587/3954464
:internal_function_strlen <resultVar> <stringVar>
(   
    setlocal EnableDelayedExpansion
    set "s=!%~2!#"
    set "len=0"
    for %%P in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
        if "!s:~%%P,1!" NEQ "" ( 
            set /a "len+=%%P"
            set "s=!s:~%%P!"
        )
    )
)
( 
    endlocal
    set "%~1=%len%"
    exit /b
)

REM --- Everything is done, exit back to the user ----------------------------

:internal_function_teardown
REM Now that processing is done, allow the user to do some steps before exiting.
goto user_function_teardown

:internal_function_exit
REM Leave the user in the directory they were in to begin with.
cd "%BUILD_SCRIPT_PATH%"

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
