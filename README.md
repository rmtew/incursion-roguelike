Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.  This file is intended for people viewing the project repository where the source code can be obtained.

![Title screen](https://bytebucket.org/rmtew/incursion-roguelike/raw/4f6afc9cc3235a56a584510e53804c3df061f936/0.9.6H/title-screen.png)

At this time building is only supported on Windows, using Visual Studio 2008.  You should be able to substitute other versions of Visual Studio, or write a makefile if you plan to compile on another platform.

Windows build instructions
--------------------------

Recommended compiler:

  * Visual Studio Express 2008: [VS2008ExpressWithSP1ENUX1504728.iso](http://download.microsoft.com/download/E/8/E/E8EEB394-7F42-4963-A2D8-29559B738298/VS2008ExpressWithSP1ENUX1504728.iso) 768.0MB.

Dependencies:

  * libtcod.dll: [bitbucket project](https://bitbucket.org/jice/libtcod).
  * flex.exe: [flex-2.5.4a-1-bin.zip](http://gnuwin32.sourceforge.net/downlinks/flex-bin-zip.php) 0.2 MB ([hosting web page](http://gnuwin32.sourceforge.net/packages/flex.htm)).
  * google breakpad (non-debug builds only):  [svn checkout](https://code.google.com/p/google-breakpad/).

Compilation instructions:

  1. Download and install Visual Studio Express 2008, if necessary.
  2. Extract 'flex.exe' directly into the project directory.
  3. Place libtcod source code under "_dependencies\libtcod" sub-directory.
  4. Apply "_dependencies\libtcod.patch".
  5. Build libtcod to obtain the libtcod.dll, libtcod.lib and SDL2.dll files.
  6. **If a debug build is being made, skip to step 13.**
  7. Place google breakpad source code under "_dependencies\google-breakpad" sub-directory.
  8. Enter the "_dependencies\google-breakpad" directory.
  9. Execute the DOS command "set GYP_MSVS_VERSION=2008", or if you are using an alternate compiler, use a modified command.
  10. Execute the DOS command "src\tools\gyp\gyp.bat --no-circular-check  src\client\windows\breakpad_client.gyp".
  11. The build files should be located in "_dependencies\google-breakpad\src\windows".
  12. Do a non-debug build of google breakpad.
  13. Build Incursion.
  14. Place "libtcod.dll" directly into the project directory.
  15. Place "SDL2.dll" directly into the project directory.
