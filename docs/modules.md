# Creating a Module

## In-game Usage

Modules are loaded when the player chooses to create a new character, or
reincarnate an existing character from the main menu.  As well as in a
variety of other random places.

## The .irc File

A module is defined using a file with the suffix `.irc`.  At this time only
the top level of the `mod` subdirectory is searched for these files.

### Module

This is a required definition.  It specifies the module name, as presented to
the player.  If not provided, compilation of the given module will fail.

    Module "<module name>";

### File

This is a required definition.  It specifies the module file name, and it
specifies the file name which the compiled module will be written out as.
If not provided, compilation of the given module will fail.

    File "";

### Slot

This is an optional definition.  If not provided, the value defaults to
putting the module in whatever slot module loading order leaves it in.

	Slot <number>;

The slot number a given module appears in depends mainly on module loading
order.  It is possible for a module to explicitly specify a slot number
by defining a `Slot` entry, but module loading will error if this causes
a conflict, discarding the second of the two to be encountered.
