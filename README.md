Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.  This file is intended for people viewing the project repository where the source code can be obtained.

At this time building is only supported on Windows, using Visual Studio 2010.  You should be able to substitute other versions of Visual Studio, or write a makefile if you plan to compile on another platform.

Support the development
-----------------------

Want to support development?  Want to encourage work on certain aspects?

You can donate money via: [Paypal](http://disinterest.org/donate.html)

You can donate bitcoin via: [16F6d1F82qrhXaR67NHczQbu4eUie7QJui](https://blockchain.info/address/16F6d1F82qrhXaR67NHczQbu4eUie7QJui)

Windows build instructions
--------------------------

*Recommended compiler:*

  * Visual Studio Express 2010: [web-based installer](http://www.visualstudio.com/en-us/downloads#d-2010-express).
  * Visual Studio Express 2010: [VS2010Express1.iso](http://download.microsoft.com/download/1/E/5/1E5F1C0A-0D5B-426A-A603-1798B951DDAE/VS2010Express1.iso) 694MB.

*Dependencies:*

  * libtcod.dll: [libtcod@bitbucket](https://bitbucket.org/jice/libtcod).
  * flex.exe: [winflexbison@sourceforge](http://sourceforge.net/projects/winflexbison/) 0.7 MB (extract and rename it).

*Compilation instructions:*

  1. Download and install Visual Studio Express 2010, if necessary.
  2. Extract 'flex.exe' directly into the project directory.
  3. Place libtcod source code under "_dependencies\libtcod" sub-directory.
  4. Build libtcod to obtain the libtcod.dll, libtcod.lib and SDL2.dll files.
  4. Build Incursion.
  4. Place "libtcod.dll" directly into the project directory.
  4. Place "SDL2.dll" directly into the project directory.

*Building modules:*

Note that the module is built by running the Incursion executable, and selecting the compile option from the main menu.  However, this option is only present in debug builds of Incursion.