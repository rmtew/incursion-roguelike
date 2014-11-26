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

  * libtcod.dll: [libtcod@bitbucket](https://bitbucket.org/jice/libtcod/src) (note this is the default branch, not 1.5.x).
  * flex.exe: [winflexbison@sourceforge](http://sourceforge.net/projects/winflexbison/) 0.7 MB (extract and rename 'win_flex.exe' to 'flex.exe').
  * _dependencies-20141125.7z: Available from the bitbucket [project downloads page](https://bitbucket.org/rmtew/incursion-roguelike/downloads).

*Compilation instructions:*

  1. Download and install Visual Studio Express 2010, if necessary.
  1. Enter the Incursion project directory.
  1. Place 'flex.exe' directly into the project directory.
  1. Extract '_dependencies-20141125.7z' over the "_dependencies" sub-directory.  Check that you extracted it at the right level.
  1. Place "libtcod.dll" directly into the project directory (look in '_dependencies\libtcod\makefiles\Release', it comes from '_dependencies-20141125.7z').
  1. Place "SDL2.dll" directly into the project directory (look in '_dependencies\libtcod', it comes from the libtcod source).
  1. Open 'Incursion.sln' and do a debug build of Incursion.
  1. Run the debug build.
  1. Select the 'Compile Resources' main menu option.  Once completed, this will have created the 'mod\Incursion.Mod' file.

At this point, you are ready to do some development, or just play the latest version of the source code.  Congratulations!
  
*Building libtcod:*

It is not necessary to build libtcod.  The include files from the libtcod source code, and the static libraries built from it, are included within the provided '_dependencies-20141125.7z' archive.

But if you do wish to build it, be aware that libtcod does not provide a Visual Studio solution and projects.  These are created by me, and are included within the provided '_dependencies-20141125.7z' archive, within the '_dependencies\libtcod\makefiles' directory.  Also note that there are minor changes to the libtcod source code, to make it better work with Incursion.  These are included in the archive, and will extract over the libtcod source code into the right places.

The recommended approach to getting a libtcod compilation environment is:

  1. Enter the Incursion project directory.
  1. Place the [libtcod source files](https://bitbucket.org/jice/libtcod/src) into the '_dependencies/libtcod' sub-directory.  Ensure you get [this commit](https://bitbucket.org/jice/libtcod/commits/30646e7bc99ececb155dece7bf9821899bf0eab2).  It matches the minor changes to the libtcod source code mentioned above.
  1. Extract '_dependencies-20141125.7z' over the "_dependencies" sub-directory.  Check that you extracted it at the right level.  This will overwrite some parts of the libtcod source code.

This should be incorporated into the compilation instructions above, so that the last step here falls in the same place as in the compilation instructions.

*Building Incursion modules:*

Note that the module is built by running the Incursion executable, and selecting the compile option from the main menu.  However, this option is only present in debug builds of Incursion.
