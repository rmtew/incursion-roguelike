<# : 
@echo off
setlocal EnableDelayedExpansion

REM Note: If a BAT script calls something else, which modifies environment variables
REM       then when that something else exits, the BAT script will still have the
REM       same variables it already had - as it gets them at startup and by direct
REM       execution.

REM Divert to the internal setup code, it will return to the user setup.
goto internal_setup

REM --- User event handler functions -----------------------------------------

:user_setup

set LINKS[0]=http://jaist.dl.sf.net/pub/sourceforge/w/wi/winflexbison/win_flex_bison-latest.zip

REM Process the user data, calling event functions when applicable.
goto return_from_user_setup

:user_zip_extracted
REM variable: %V_FILE% - The relative file name of the archive.
REM variable: %DEPENDENCY_PATH% - The absolute directory the archive is located in, and will be extracted within.
REM variable: %EXTRACT_DIRNAME% - The relative directory name the archive contains as it's sole top level entry.
REM variable: %V_EXTRACT_SKIPPED% - 'yes' or 'no', depending on whether the archive was already extracted.
cd !DEPENDENCY_PATH!

if "!V_FILE!" EQU "win_flex_bison-latest.zip" (
    if "%V_EXTRACT_SKIPPED%" equ "no" (
        if exist %BUILD_PATH%flex.exe del %BUILD_PATH%flex.exe        
    )
    if not exist %BUILD_PATH%flex.exe (
        cd !EXTRACT_DIRNAME!
        copy win_flex.exe %BUILD_PATH%flex.exe

        goto return_from_user_zip_extracted
    )
) else (
    echo ERROR.. !V_FILE! not handled by user who wrote the build amendments.

    goto return_from_user_zip_extracted
)

echo MSG: Already built.
goto return_from_user_zip_extracted

REM User function: user_teardown
REM Description: Called as a final step before the script exits.
:user_teardown



goto return_from_user_teardown

REM --- Script defined values ------------------------------------------------

:internal_setup

REM Ensure that we have a properly set up developer console with access to things like msbuild and devenv.
if not exist "%VS120COMNTOOLS%VsDevCmd.bat" (
    echo You do not appear to have Visual Studio 2013 installed.
    echo The community edition is free, download it and install it.
    pause & exit /b
)
if "%VisualStudioVersion%" EQU "" %VS120COMNTOOLS%VsDevCmd.bat
if "%VisualStudioVersion%" NEQ "12.0" (
    echo Your console window has already run the setup for Visual Studio %VisualStudioVersion%.
    echo Open a fresh window and run this script there.  It will run the correct setup.
    pause & exit /b
)

set BUILD_PATH=%~dp0
set BUILD_SCRIPT_FILENAME=%~nx0
set DEPENDENCY_DIRNAME=dependencies
set DEPENDENCY_PATH=%BUILD_PATH%%DEPENDENCY_DIRNAME%

if not exist %DEPENDENCY_PATH% mkdir %DEPENDENCY_PATH%
cd %DEPENDENCY_PATH%

REM --- Script defined values ------------------------------------------------

set FILES[0]=
set V_EXTRACT_DIRNAMES[0]=

goto user_setup
 
REM --- FUNCTION: Map lines to local filenames -------------------------------
:return_from_user_setup

REM Iterate over the links.
SET /A IDX=0
:loop1processlinks
set V_LINK=!LINKS[%IDX%]!
if "!V_LINK!" EQU "" goto downloadlinkedfiles

:loop2processlink
if "!V_LINK!" EQU "" goto loop0continue
REM Select the substring up to the first path separator.
for /f "delims=/" %%M in ("!V_LINK!") do set SUBSTRING=%%M

:loop3strip
REM Skip until the next path separator.
set CHAR=!V_LINK:~0,1!
set V_LINK=!V_LINK:~1!
if "!V_LINK!" EQU "" goto foundfile
if "!CHAR!" NEQ "/" goto loop3strip
goto loop2processlink

:foundfile
REM We have the trailing string after the last path separator, or the file name.
set FILES[%IDX%]=!SUBSTRING!
goto loop2processlink

:loop0continue
set /A IDX=!IDX!+1
goto loop1processlinks

REM --- DOWNLOAD DEPENDENCIES THAT HAVE NOT ALREADY BEEN DOWNLOADED ----------

:downloadlinkedfiles

set /A IDX=0
:loop_downloadlinkedfiles
set V_LINK=!LINKS[%IDX%]!
set V_FILE=!FILES[%IDX%]!
if "!V_LINK!" EQU "" goto processlinkedfiles

if not exist !V_FILE! (
    REM Download it through powershell (puts up console download progress GUI).
    echo Downloading: !V_FILE!
    powershell -c "Start-BitsTransfer -source !V_LINK!"
) else (
    echo Downloading: !V_FILE! [skipped]
)

set /A IDX=!IDX!+1
goto loop_downloadlinkedfiles

REM --- PROCESS  DEPENDENCIES THAT HAVE NOT ALREADY BEEN PROCESSED  ----------

:processlinkedfiles

set /A IDX_PLF=0
:loop_processlinkedfiles
set V_LINK=!LINKS[%IDX_PLF%]!
set V_FILE=!FILES[%IDX_PLF%]!
if "!V_LINK!" EQU "" goto internal_teardown

if "%V_FILE:~-4%" EQU ".zip" (    
    echo Decompressing: %V_FILE%
    set fn=Archive-Extract
    set fnp0=%DEPENDENCY_PATH%\%V_FILE%
    set fnp1=%DEPENDENCY_PATH%
    set EXTRACT_DIRNAME=
    set V_EXTRACT_DIRNAMES[%IDX_PLF%]=
    set V_EXTRACT_SKIPPED=no

    for /F "usebackq tokens=*" %%i in (`more %BUILD_PATH%%0 ^| powershell -c -`) do (
        set vi=%%i
        if "!vi:~0,12!" EQU "EXTRACTPATH:" (
            set EXTRACT_DIRNAME=!vi:~13!
            set V_EXTRACT_DIRNAMES[%IDX_PLF%]=!EXTRACT_DIRNAME!
        ) else if "!vi:~0,4!" EQU "MSG:" (
            set V_EXTRACT_MSG=!vi:~5!
            if "!V_EXTRACT_MSG!" EQU "Already extracted." set V_EXTRACT_SKIPPED=yes
            echo !vi!
        ) else (
            echo Unexpected result: !vi!
        )
    )

    goto user_zip_extracted
:return_from_user_zip_extracted
    REM Restore this just in case.  Is it necessary?
    REM cd !DEPENDENCY_PATH!
    REM This is here to get the label directly preceding it to parse correctly.
)

set /A IDX_PLF=!IDX_PLF!+1
goto loop_processlinkedfiles

REM --- Everything is done, exit back to the user ----------------------------

:internal_teardown
REM Now that processing is done, allow the user to do some steps before exiting.
goto user_teardown

:return_from_user_teardown
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
        Write-Host "MSG: Already extracted.";
        return 2; # Failure
    }
    
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
