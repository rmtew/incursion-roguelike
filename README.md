Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.  This file is intended for people viewing the project repository where the source code can be obtained.

At this time building is only supported on Windows, using Visual Studio 2022.  You should be able to substitute other versions of Visual Studio, or write a makefile if you plan to compile on another platform.

The Incursion website used to be [found here](http://incursion-roguelike.net) once. Maybe the internet archive has it for now?

Recent Changes
--------------

Read 'Changelog.txt'.  The version accompanying any source code should reflect the changes relating to that source code. The version on Github will reflect the latest changes.

Support the development
-----------------------

Want to support development?  Want to encourage work on certain aspects? Offer me money if you really want, this is not a project I work on these days. Or chip in and help out with pull requests!

Links
-----

 * Page on [Rogue Basin](https://www.roguebasin.com/index.php?title=Incursion).

Windows build instructions
--------------------------

**Recommended compiler:**

  * Visual Studio Community 2022: [web-based installer](https://visualstudio.microsoft.com/downloads/#d-community).

**Dependencies:**

  * libtcod 1.7.0 binaries: [Github release page](https://github.com/libtcod/libtcod/releases/tag/1.7.0).

These are optional extras, for the unsupported pdcurses-based console version or making changes to the module language. The links may be stale.

  * `flex.exe`: [winflexbison@sourceforge](http://sourceforge.net/projects/winflexbison/) 660 KB (extract and rename `win_flex.exe` to `flex.exe`).
  * `pdcurses`: [pdcurses@sourceforge](http://pdcurses.sourceforge.net) 384 KB

**Compilation instructions:**

These instructions are intended to allow you to get Incursion to the point where you can debug it within Visual Studio.

A pull request will fill in the instructions, if you want to. But maybe look at [this commit](https://github.com/rmtew/incursion-roguelike/commit/d55c5ace9fc1e26350a2ab0d142a6060421196b9) for now.

At this point, you are ready to do some development, or just play the latest version of the source code by debugging it within Visual Studio.  Congratulations!

**Running the game:**

Note that all the files which `Incursion.exe` requires to run are scattered in a variety of directories, and you cannot just copy the executable somewhere and expect it to work.  If you wish to collect the correct set of files, look at the code for `build.bat -r`. Or keep running it and providing what it says it is missing! A pull request might define the files needed and where, if you want to make one.

**x64 support:**

You can compile an x64 build of Incursion. It won't be easy, but it will run without crashing.  Will it allow someone to play without crashing any more than the Win32 version does?  No idea!

Note that the build will fail unless you can work out a way to get it to use the Win32 build of modaccent, which crashes when run as an x64 binary.  Incursion gives a lot of compilation warnings when compiled for the x64 platform, as it is an older project.  But `modaccent` is even older source code which uses some arcane parts of the original C programming standard!  Anyone wanting to use x64 builds should not expect any bugs filed about it to be resolved.

Also, the unsupported `exe_curses` prototype has been disabled for x64.  `pdcurses` only provides a makefile for the Win32 platfom.