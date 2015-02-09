@echo off

REM TODO: The libtcod and SDL2 pdbs from the relevant _dependency locations should be used if they are present.
REM       - If they are not present, then complain loudly.
REM       - The correct release and debug versions should be copied.
REM       - Also generate the _dependencies-YYYYMMDD.7z file.

SET TLD=INCURSION-RELEASES

PUSHD .

FOR %%I IN (exe_libtcod exe_curses) DO (
    FOR %%J IN (Release Debug) DO (
        IF NOT EXIST build\%%I\%%J (
            @ECHO The [%%I] project needs to be built in [%%J] configuration.    
            EXIT /b 1
        )
    )
)

IF NOT EXIST mod\Incursion.Mod (
    @ECHO [Incursion.Mod] needs to be built.  See the readme file.
    EXIT /b 1
)

@ECHO Making '%TLD%' directories.
IF EXIST %TLD% (
	RD /s /q %TLD%
    if ERRORLEVEL 1 (
        @echo Cannot remove %TLD%, it is locked
        exit /b 1
	)
)
MD %TLD%
if ERRORLEVEL 1 (
	@echo Cannot create %TLD% directory
	exit /b 1
)

CD %TLD%

@ECHO Making required directories and copying shared files.
FOR %%I IN (Incursion-VVV\ Incursion-VVV-release-with-pdbs\ Incursion-VVV-debug-with-pdbs\) DO (
	MD %%I
	FOR %%J IN (fonts logs mod save) DO (
		MD %%I%%J >NUL
	)
	FOR %%J IN (..\Changelog.txt ..\Incursion.txt ..\LICENSE ..\libtcod.dll ..\SDL2.dll) DO (
		COPY %%J %%I >NUL
	)
	FOR %%J IN (fonts mod) DO (
		COPY ..\%%J\* %%I\%%J\ >NUL
	)
)

@ECHO Copying executables.
COPY ..\build\exe_libtcod\Release\Incursion.exe Incursion-VVV\ >NUL
COPY ..\build\exe_curses\Release\IncursionCurses.exe Incursion-VVV\ >NUL

COPY ..\build\exe_libtcod\Release\Incursion.exe Incursion-VVV-release-with-pdbs\ >NUL
COPY ..\build\exe_libtcod\Release\Incursion.pdb Incursion-VVV-release-with-pdbs\ >NUL
COPY ..\build\exe_curses\Release\IncursionCurses.exe Incursion-VVV-release-with-pdbs\ >NUL
COPY ..\build\exe_curses\Release\IncursionCurses.pdb Incursion-VVV-release-with-pdbs\ >NUL

COPY ..\build\exe_libtcod\Debug\Incursion.exe Incursion-VVV-debug-with-pdbs\ >NUL
COPY ..\build\exe_libtcod\Debug\Incursion.pdb Incursion-VVV-debug-with-pdbs\ >NUL
COPY ..\build\exe_curses\Debug\IncursionCurses.exe Incursion-VVV-debug-with-pdbs\ >NUL
COPY ..\build\exe_curses\Debug\IncursionCurses.pdb Incursion-VVV-debug-with-pdbs\ >NUL

POPD
