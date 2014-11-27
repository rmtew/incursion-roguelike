@echo off

PUSHD .

FOR %%I IN (Release\Incursion.exe Debug\Incursion.exe mod\Incursion.mod) DO (
	IF NOT EXIST %%I (
		@ECHO Please build '%%I' and try again.
		EXIT /b 1
	)
)

@ECHO Making 'INCURSION-RELEASES' directories.
IF EXIST INCURSION-RELEASES (
	RD /s /q INCURSION-RELEASES
    if ERRORLEVEL 1 (
        @echo Cannot remove INCURSION-RELEASES, it is locked
        exit /b 1
	)
)
MD INCURSION-RELEASES
if ERRORLEVEL 1 (
	@echo Cannot create INCURSION-RELEASES directory
	exit /b 1
)
CD INCURSION-RELEASES

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
