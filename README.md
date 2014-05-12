Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.  This file is intended for people viewing the project repository where the source code can be obtained.

At this time building is only supported on Windows, using Visual Studio 2008.  You should be able to substitute other versions of Visual Studio, or write a makefile if you plan to compile on another platform.

Support the development
-----------------------

If you wish to support the development..

You can donate money via:

  [Paypal](http://disinterest.org/donate.html)

You can donate bitcoin via:

  [16F6d1F82qrhXaR67NHczQbu4eUie7QJui](https://blockchain.info/address/16F6d1F82qrhXaR67NHczQbu4eUie7QJui)

Windows build instructions
--------------------------

*Recommended compiler:*

  * Visual Studio Express 2008: [VS2008ExpressWithSP1ENUX1504728.iso](http://download.microsoft.com/download/E/8/E/E8EEB394-7F42-4963-A2D8-29559B738298/VS2008ExpressWithSP1ENUX1504728.iso) 768.0MB.

*Dependencies:*

  * libtcod.dll: [bitbucket project](https://bitbucket.org/jice/libtcod).
  * flex.exe: [flex-2.5.4a-1-bin.zip](http://gnuwin32.sourceforge.net/downlinks/flex-bin-zip.php) 0.2 MB ([hosting web page](http://gnuwin32.sourceforge.net/packages/flex.htm)).
  * google breakpad (non-debug builds only):  [svn checkout](https://code.google.com/p/google-breakpad/).

*Compilation instructions:*

  1. Download and install Visual Studio Express 2008, if necessary.
  2. Extract 'flex.exe' directly into the project directory.
  3. Place libtcod source code under "_dependencies\libtcod" sub-directory.
  4. Build libtcod to obtain the libtcod.dll, libtcod.lib and SDL2.dll files.
  5. **If a debug build is being made, skip to step 12.**
  6. Place google breakpad source code under "_dependencies\google-breakpad" sub-directory.
  7. Enter the "_dependencies\google-breakpad" directory.
  8. Execute the DOS command "set GYP_MSVS_VERSION=2008", or if you are using an alternate compiler, use a modified command.
  9. Execute the DOS command "src\tools\gyp\gyp.bat --no-circular-check  src\client\windows\breakpad_client.gyp".
  10. The build files should be located in "_dependencies\google-breakpad\src\windows".
  11. Do a non-debug build of google breakpad.
  12. Build Incursion.
  13. Place "libtcod.dll" directly into the project directory.
  14. Place "SDL2.dll" directly into the project directory.

*Building modules:*

Note that the module is built by running the Incursion executable, and selecting the compile option from the main menu.  However, this option is only present in debug builds of Incursion.
