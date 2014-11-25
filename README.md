Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.  This file is intended for people viewing the project repository where the source code can be obtained.

At this time building is only supported on Windows, using Visual Studio 2010.  You should be able to substitute other versions of Visual Studio, or write a makefile if you plan to compile on another platform.

Recent Changes
--------------

Click on the bitbucket Source button, and then click on 'Changelog.txt'.  This gives a more readable and relevant form of the Bitbucket project's recent activity listing.

Support the development
-----------------------

Want to support development?  Want to encourage work on certain aspects?

You can donate money via: [Paypal](http://disinterest.org/donate.html)

You can donate bitcoin via: [16F6d1F82qrhXaR67NHczQbu4eUie7QJui](https://blockchain.info/address/16F6d1F82qrhXaR67NHczQbu4eUie7QJui)

Links
-----

 * Page on [Rogue Basin](http://www.roguebasin.com/index.php?title=Incursion).
 * Thread on [Bay12](http://bay12forums.com/smf/index.php?topic=139289).

Windows build instructions
--------------------------

*Recommended compiler:*

  * Visual Studio Express 2010: [web-based installer](http://www.visualstudio.com/en-us/downloads#d-2010-express).
  * Visual Studio Express 2010: [VS2010Express1.iso](http://download.microsoft.com/download/1/E/5/1E5F1C0A-0D5B-426A-A603-1798B951DDAE/VS2010Express1.iso) 694MB.

*Dependencies:*

  * libtcod.dll: [libtcod@bitbucket](https://bitbucket.org/jice/libtcod).
  * flex.exe: [winflexbison@sourceforge](http://sourceforge.net/projects/winflexbison/) 0.7 MB (extract and rename it).
  * _dependencies-20141125.7z: Available from the bitbucket project downloads page.

*Compilation instructions:*

  1. Download and install Visual Studio Express 2010, if necessary.
  2. Extract 'flex.exe' directly into the project directory.
  3. Place [these libtcod source files and directories](https://bitbucket.org/jice/libtcod/src) into the "_dependencies\libtcod" sub-directory.  Note they are from the default branch, not the 1.5.x branch.
  4. Extract '_dependencies-20141125.7z' over the "_dependencies" sub-directory.  If extracted correctly, it should ask for permission to override files and directories.
  4. Build libtcod to obtain the libtcod.dll, libtcod.lib and SDL2.dll files.  Read the section below for help.
  4. Build Incursion.
  4. Place "libtcod.dll" directly into the project directory.
  4. Place "SDL2.dll" directly into the project directory.

*Building libtcod:*

The '_dependencies-20141125.7z' file contains a few minor source modifications, and the solution and project files required to build libtcod with Visual Studio 2010.  To build it, you simply build the solution.  However, this may not be necessary.  The built lib and dll files are also bundled in the '_dependencies-20141125.7z' file.  With them also extracted in place, it may be possible to build Incursion without needing to build libtcod at all.

*Building Incursion modules:*

Note that the module is built by running the Incursion executable, and selecting the compile option from the main menu.  However, this option is only present in debug builds of Incursion.
