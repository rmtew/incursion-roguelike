Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.

At this time building is only supported on Windows, using Visual Studio 2022.  You should be able to substitute other versions of Visual Studio, or write a makefile if you plan to compile on another platform.

The Incursion website is [found here](http://incursion-roguelike.net).

Frequently asked questions
==========================

 Q. When I press a movement key, it sends two keypresses?
 A. Turn off numlock. You are using a keypad on a full keyboard.  SDL2 does not support differentiating between number keypad presses, and number key presses. This means we cannot either.

Recent Changes
--------------

Read 'Changelog.txt'.  The version accompanying any source code should reflect the changes relating to that source code. The version on Github will reflect the latest changes.

Support the development
-----------------------

Want to support development?  Want to encourage work on certain aspects? Offer me money if you really want, this is not a project I work on these days. Or chip in and help out with pull requests! Or fork it and release something it with a name that does not confuse your builds with this project.

Links
-----

 * Page on [Rogue Basin](https://www.roguebasin.com/index.php?title=Incursion).
 * Discuss things with us in [our discussion area](https://github.com/rmtew/incursion-roguelike/discussions).
 * Report a bug on [our issues tracker](https://github.com/rmtew/incursion-roguelike/issues).
 * Find the source code in the [Incursion repository](https://github.com/rmtew/incursion-roguelike).

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

1. Download the following two binary releases for [libtcod 1.7.0](https://github.com/libtcod/libtcod/releases/tag/1.7.0):
   * `libtcod-1.7.0-x86-msvc.zip`.
   * `libtcod-1.7.0-x86_64-msvc.zip`.
2. Extract each to the top-level `dependencies` directory.
   * `dependencies\libtcod-1.7.0-x86-msvc`.
   * `dependencies\libtcod-1.7.0-x86_64-msvc`.
3. Open `build\Incursion.sln` in Visual Studio 2022.
4. Ensure `exe_libtcod` is the default project. If it is not, right click on it and select "Set as Startup Project".
5. Build a Debug solution.
6. Run the built `Incursion.exe` with the command-line `build/Win32/Debug/exe_libtcod/Incursion.exe -compile` with a current directory of the top-level source directory. The module compilation code is not built into Release builds. It should generate a `mod\Incursion.mod` file which provides the game rules, setting and other things.

Note that there are custom build steps in Debug configurations that update this resource compiler, under the "Language Files" folder within Visual Studio. `tokens.lex` is parsed by `flex.exe`. `grammar.acc` is parsed by `modaccent.exe`. These steps may need to be disabled to compile the x64 Debug build as `modaccent.exe` is based on old external code that is not 64-bit compatible at this time. The output files of both steps are checked in and only deleted by rerunning these steps, so running them is not necessary and if it is, it can be done in Win32 Debug configuration.

At this point, you are ready to do some development, or just play the latest version of the source code by debugging it within Visual Studio.  Congratulations!

**Running the game:**

The following directory structure is required to have a running game installation:

```
Incursion.exe
libtcod.dll
SDL2.dll
fonts\12x16.png
fonts\16x12.png
fonts\16x16.png
fonts\8x8.png
mod\Incursion.Mod
```


**x64 support:**

Yes, you can compile an x64 build of Incursion. It will runs and enters a new game without crashing.  Will it allow someone to play without crashing any more than the Win32 version does?  No idea!

Note that the build will fail unless you can work out a way to get it to use the Win32 build of modaccent, which crashes when run as an x64 binary.  Incursion gives a lot of compilation warnings when compiled for the x64 platform, as it is an older project.  But `modaccent` is even older source code which uses some arcane parts of the original C programming standard!  Anyone wanting to use x64 builds should not expect any bugs filed about it to be resolved.

Also, the unsupported `exe_curses` prototype has been disabled for x64.  `pdcurses` only provides a makefile for the Win32 platfom.