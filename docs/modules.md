# User defined modules

TODO: RComp.cpp: Game::ResourceCompiler()
    if (FIND("mage") && FIND("shocker lizard") && !Errors) {

## Which modules are loaded

There are a number of ways to start playing from the main menu, and they
depend on modules in different ways.

If the player chooses to create a new character or to reincarnate an existing
character, any modules located on disk are loaded.

If the player chooses to load a saved game, the set of modules which were on
disk at the time the game's character was created, or reincarnated, are
expected to be present and will be the ones loaded.  If any are not present
the user will be shown an error dialog.

## Creating a Module

A module is defined using a file with the suffix `.irc`.  At this time only
the top level of the `mod` subdirectory is searched for these files.

### Keyword: Module

Required: If not provided, compilation of the given module will fail.

This is a required definition.  It specifies the module name, as presented to
the player.

    Module "<module name>";
	e.g. Module "Bong Bong's Magical Tower"

### Keyword: File

Required: If not provided, compilation of the given module will fail.

This is a required definition.  It specifies the module file name, and it
specifies the file name which the compiled module will be written out as.

    File "<file name>";
	e.g. File "BongBongsMagicalTower.Mod";

### Keyword: Slot

Optional: If not provided, the module will automatically be placed in the
first unoccupied slot.

	Slot <number>;
	e.g. Slot 4;

The slot a given module appears in depends mainly on module loading
order.  It is possible for a module to explicitly specify a slot number
by defining a `Slot` entry, but module loading will error if this causes
a conflict, discarding the second of the two to be encountered.
