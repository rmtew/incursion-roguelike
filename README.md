Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.  This file is intended for people viewing the project repository where the source code can be obtained.

At this time building is only supported on Windows, using Visual Studio 2013.  You should be able to substitute other versions of Visual Studio, or write a makefile if you plan to compile on another platform.

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

  * Visual Studio Express 2013: [web-based installer](http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs#d-community).

*Dependencies:*

The simple option is to download the following dependencies archive.  It contains all the pre-built files required to get Incursion to build.  It just needs to be downloaded, extracted in the right location within the source code, and the compilation instructions below detail this further.

  * `build_dependencies-20150210.7z`: Available from the bitbucket [project downloads page](https://bitbucket.org/rmtew/incursion-roguelike/downloads).

The advanced option, which someone who knows about debugging and stepping into linked library code would be interested in, is to use the `build.bat` batch script which is located with the source code.

A simple downloadable archive of pre-built dependencies is mentioned, and also an advanced script that will download them and compile them for you.  If you choose to ignore both of these and go your own way, it is going to be difficult for you to compile them yourself, unless you know what you are doing, and you are willing to do it without any help from me.  In you wish to do this to yourself, and by yourself without any help from me, you will need the following so that you can do it without any help from me.

  * `libtcod`: [libtcod@bitbucket](https://bitbucket.org/jice/libtcod/) (note this is the default branch, not 1.5.x).
  * `SDL2`: [SDL2@libsdl.org](https://www.libsdl.org/hg.php) (can be optional).
  * `flex.exe`: [winflexbison@sourceforge](http://sourceforge.net/projects/winflexbison/) 660 KB (extract and rename `win_flex.exe` to `flex.exe`).
  * `pdcurses`: [pdcurses@sourceforge](http://pdcurses.sourceforge.net) 384 KB

*Compilation instructions:*

These instructions are intended to allow you to get Incursion to the point where you can debug it within Visual Studio.

  1. Download and install Visual Studio 2013 Community Edition, if necessary.
  1. Download and extract the Incursion source code.
  1. Enter the `build` subdirectory within the source code.
  1. Ensure that you have the EditorConfig extension installed by doing the following:
    1. Run Visual Studio, perhaps by opening `Incursion.sln`.
    1. Select the menu option: Tools -> Extensions and Updates.
    1. Select the Online entry from the left-hand side list.
    1. Search for and install EditorConfig.
    1. Exit Visual Studio.
  1. If you are going to do take the simple approach to obtaining the dependencies:
    1. Ensure you have downloaded the build dependencies archive mentioned above, from the bitbucket [project downloads page](https://bitbucket.org/rmtew/incursion-roguelike/downloads).
    1. Open the archive and look inside it.  There is only one entry there, a directory named `dependencies`, and within that there is a selection of files and directories inside it like `include`, `libtcod.dll` and so forth.
    1. Enter the `build` subdirectory within the source code.
    1. Extract the contents of the archive here.
    1. You should now be able to locate the `build\dependencies\include` directory.  You should now be able to locate `build\dependencies\libtcod.lib`.  If this is not the case, then you did not extract the contents of the archive into the `build` subdirectory.  Try again.
  1. Otherwise if you are going to do take the advanced approach to obtaining the dependencies:
    1. Open a DOS console window, and ensure you are in the top-level directory of the Incursion source code.
    1. Type `build.bat -d` and hit enter.
    1. Lots of text will scroll past and eventually you will have compiled source code, or this script will error and you will have to ask for help.
  1. If you have reached here, you have obtained the dependencies.
  1. Open `Incursion.sln`.
  1. Ensure the `Debug` configuration is selected.
  1. Look at the Solution Explorer window.  You will see four entries: `exe_curses`, `exe_libtcod`, `lib_incursion` and `modaccent`.  One will be darker than the others.  This is the one which runs when you start debugging.  If the darker entry is not `exe_libtcod`, then right click on `exe_libtcod` and choose the `Set as Startup Project` option to make it darker.
  1. Start debugging Incursion.  The game window will open.
  1. In the Incursion main menu, select the 'Compile Resources' option.  Until this is done and the module is generated, few other options will work.  Once the module is generated, this will have created a `build\run\mod\Incursion.Mod` file.

At this point, you are ready to do some development, or just play the latest version of the source code within Visual Studio.  Congratulations!  But note that all the files which `Incursion.exe` requires to run are scattered in a variety of directories, and you cannot just copy it somewhere and expect it to work.  At some point `build.bat -r` will take care of that.  For now, you can read `build\make-release.bat` and try and work it out for yourself.  Or open an Incursion release and look at what files are needed where.
