Incursion
=========

Incursion is a roguelike developed by Julian Mensch.  He has kindly released the source he has for some of the later versions.

At this time building is only supported on Windows, using Visual Studio 2008.

Windows build instructions
--------------------------

Required dependencies:

  * Visual Studio Express 2008: [VS2008ExpressWithSP1ENUX1504728.iso](http://download.microsoft.com/download/E/8/E/E8EEB394-7F42-4963-A2D8-29559B738298/VS2008ExpressWithSP1ENUX1504728.iso) 768.0MB.
  * Allegro binaries: [allegro-msvc9-4.2.3.zip](http://cdn.allegro.cc/file/library/allegro-4.2.3/allegro-msvc9-4.2.3.zip) 11.0MB ([hosting web page](https://www.allegro.cc/files/?v=4.2)).
  * DirectX 7 SDK files (if required): [dx70_min.zip](http://alleg.sourceforge.net/files/dx70_min.zip) 0.3 MB ([hosting web page](http://alleg.sourceforge.net/wip.html)).
  * Flex: [flex-2.5.4a-1-bin.zip](http://gnuwin32.sourceforge.net/downlinks/flex-bin-zip.php) 0.2 MB ([hosting web page](http://gnuwin32.sourceforge.net/packages/flex.htm)).

Dependency instructions:

  1. Download and install Visual Studio Express 2008, if you do not already have it installed.
  2. Extract the Allegro zip archive into the project directory, and rename the 'allegro-msvc9-4.2.3' subdirectory to 'allegro'.
  3. Extract DirectX into the project directory, under a 'dx70_min' subdirectory you will have to create.
  4. Extract 'flex.exe' directly into the project directory.

At this point, Incursion should build successfully.
