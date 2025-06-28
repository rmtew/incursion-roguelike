Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.  This file is intended for people viewing the project repository where the source code can be obtained.

At this time building is only supported on Windows using Visual Studio x64 native command-line tools.  You should be able to substitute other versions of Visual Studio, or write a makefile if you plan to compile on another platform.

The Incursion website can be [found here](http://incursion-roguelike.net).

Links
-----

 * Page on [Rogue Basin](http://www.roguebasin.com/index.php?title=Incursion).

This used to be the go to place for discussion:

 * Thread on [Bay12](http://bay12forums.com/smf/index.php?topic=139289).

Windows build instructions
--------------------------

Pre-compiled binaries are included for all dependencies. However if you need to re-compile them
or fix bugs then the included scripts allow you to:

* Run `build_sdl2.bat` to compile SDL2.
* Run `build_libtcod.bat` to compile libtcod.
* Run `build_pdcurses.bat` to compile pdcurses.

For recompiling Incursion itself:

* Run `build.bat` to compile Incursion.
   * `modaccent.exe` will be only be compiled for Debug configuration.
   * `IncursionLibtcod.exe` will be compiled with libtcod used for terminal UI.
   * `IncursionCurses.exe` will be compiled with pdcurses used for terminal UI.

Linux/MacOS build instructions
------------------------------

Incursion was developed a long time ago and for primarily for Windows. While it was coded
appropriately for the time and is an even more impressive project for that, the code is now dated.
And worse, the code it incorporated is even more dated. ACCENT dates from 1999 at best and
produces 1980's C code. The CPP compiler it includes is actually from the 1980's.

If you run `modaccent` you may find that it produces corrupt "# line" directives in `yygram.cpp`.
This is a sporadic problem and seems to relate to dated code and certain specific values.

I almost got it to build an Incursion executable. It just failed to find exported symbols it should
have been able to find (libtcod and SDL2), and symbols it couldn't know it wouldn't find (no
function declarations in CPP/modaccent code?).

If an executable were produced, then there are the other problems. Like Incursion was written with
code that never conceived of having to deal with a 64-bit architecture. But then, Windows has
this problem as well.

No save game, no bug fix
------------------------

Bug fixes to gameplay require a save game. Often players save regularly, and on encountering a
bug might load an earlier save and work out how to save again just before it occurs. This allows
the developer to immediately reproduce a problem they would have no hope of reproducing on their
own. However the build of Incursion uses needs to be the one the save game was made with. This is
a reason why we keep everything, binaries and source code.

* Even when it isn't because of some obscure game scenario, or something else happening in the
   background in some other part of the level where a goblin is fighting an orc or whatever
   debugging the save game can pick it up.
* Character creation choices can be very varied in Incursion. The player might not even remember
   what they selected, meaning that reproducing any problem involving it would be a wild goose
   chase.

Binaries and source code for dependencies
-----------------------------------------

Different programmers have different philosophies when it comes to managing dependencies. Some
prefer build tools and package managers. Incursion used to take this approach in one form
of another, and returning to it after many years the ability to compile it was lost.

This is especially problematic in the case of Incursion given the aspects described in the "No save
game, no bug fix" section above. We want to be able to debug a save game for an old game build
and know that we have both source code available for it, and all exact versions of dependencies,
and the ability to view source all the way down. The current build architecture ensures this,
with the sole point preventing us from having hermetic builds being inability to easily lock to
specific VS compiler and platform SDK versions without checking them in.

An example of this is the change in the platform SDKs to require 64 bit struct packing.

```c
#elif _MSC_VER >= 1300
#pragma warning(push)
#pragma warning(disable: 4116)
C_ASSERT(TYPE_ALIGNMENT(LARGE_INTEGER) == 8);
#pragma warning(pop)
#endif
#endif
```

Even with VS 2015 this was a problem and required platform SDK pinning, it isn't something new
with VS 2022.

Building the module
-------------------

Module building is built into the Debug build. It is provided as an option in the game menu
when running the Debug executable. Something something modaccent and the GPL license and how not
including it in the release build helps us with that.

The module is generally built into the with the debug build and included with the release build
when we distribute a version of the game.

Miscellaneous
-------------

* The font used by libtcod is more square and compact. The benefit in map visibility and fitting
  more on the screen is a good reason in using libtcod over curses.
* Modaccent was generated by ACCENT compiler compiler which uses Gentle. In one rare situation
  I had to track down the references source code in "gen.l", "gen.y" and "lex.yy.c" from the right
  distribution of one of ACCENT and Gentle. I should have checked them in.
