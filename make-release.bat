@echo off

REM TODO: The libtcod and SDL2 pdbs from the relevant _dependency locations should be used if they are present.
REM       - If they are not present, then complain loudly.
REM       - The correct release and debug versions should be copied.
REM       - Also generate the _dependencies-YYYYMMDD.7z file.

SET TLD=INCURSION-RELEASES

PUSHD .

FOR %%I IN (Release\Incursion.exe Debug\Incursion.exe mod\Incursion.mod) DO (
	IF NOT EXIST %%I (
		@ECHO Please build '%%I' and try again.
		EXIT /b 1
	)
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
COPY ..\Release\Incursion.exe Incursion-VVV\ >NUL
FOR %%I IN (..\Release\Incursion.exe ..\Release\Incursion.pdb) DO COPY %%I Incursion-VVV-release-with-pdbs\ >NUL
FOR %%I IN (..\Debug\Incursion.exe ..\Debug\Incursion.pdb) DO COPY %%I Incursion-VVV-debug-with-pdbs\ >NUL

POPD
