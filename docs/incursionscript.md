# IncursionScript

## Events

### Event Modifiers

#### PRE

Before an specified event 'broadcast-event' is processed, 'PRE(broadcast-event)' is first processed.

Return values:

 * ABORT: Do not process 'broadcast-event'.
 * DONE: Do not process 'broadcast-event'.
 * Otherwise: Proceed to process 'broadcast-event'.

#### POST

If when 'broadcast-event' is processed ABORT is not the return value, 'POST(broadcast-event)' is next processed.

No return values required from post-events.

#### EVICTIM / ETARGET

When a creature receives an event:

 * If the creature is the victim the event will be propagated to the monster handling as 'EVICTIM(event)'. 
 * If the creature is the actor the event will be propagated to the monster handling as 'event'.
 
If the return value is not DONE, ERROR or ABORT, then what was propagated will be further propagated to all template statis.

#### EITEM

When an item receives an event:

 # The script class for the item gets the event propagated as 'EITEM(event)'.  But only if the script class for the event is not that of the item receiving the event.  If DONE or ERROR are returned, the item is done handling the event.
 # The script class for the item gets the event propagated as is.  If any result other than NOTHING is returned, the item is done handling the event.
 # Otherwise, the item falls back on standard hard-coded event handling.

#### META

For every event processed, a targeted further modified event 'META(event)' is sent to all involved objects.  Except for the victim/target which is sent 'META(EVICTIM(event))'.

#### GODWATCH

For every event processed, a targeted further modified event 'GODWATCH(event)' is sent to each god.  Next the process is repeated with 'GODWATCH(EVICTIM(event))' sent to each god.

### Event Types

All event types can be hooked into in a variety of ways, by most scripts.  However, some events are limited to certain script types.

#### EV_ACTIVATE
#### EV_ADAMAGE
#### EV_ADVANCE

Invoked in: Player.
Handled in: Various script classes.

Invoked when the player advances a level.

#### EV_ALIGNED

Invoked in: Character.
Handled in: Various script classes.

Invoked when a character peforms an action representative of a given alignment.

#### EV_ANGER_PULSE

Invoked in: Creature.
Handled in: Various god script classes.

Invoked during a creature's turn, as an indication of an angry god.

#### EV_APPLY
#### EV_ASCEND

Handled in: Creature.

Called to go up a level, climb onto the ceiling or climb a tree.

  * DONE: Success.
  * ABORT: Failure.

#### EV_ATTACKMSG
#### EV_ATTK
#### EV_BARTER
#### EV_BIRTH
#### EV_BLESSING
#### EV_BLOCK

Not implemented.

#### EV_BREAK
#### EV_BRIDGE
#### EV_BURN
#### EV_CALC_EFFECT
#### EV_CALC_FAVOUR
#### EV_CALC_VALUES
#### EV_CAST
#### EV_CHANGE_ALIGN
#### EV_CLEAN
#### EV_CLOSE

Handled in: Door.

At this time, it is only called to attempt to close a door.

  * DONE: Success.
  * ABORT: Failure.

#### EV_COMPULSION

Handled in: Script hook possibility.

Invoked prioritised forced action for a charmed and compelled player or monster, currently does nothing.

#### EV_CONDITION

Not implemented.

#### EV_CONVERT

Handled in: Creature.

Invoked when a praying user chooses to convert to follow whomever the given altar represents.

  * DONE: Success.
  * ABORT: Failure.

#### EV_CONVERT_ALTAR

Handled in: Creature.

Invoked when a praying user chooses to consecrate the given altar to whomever they follow.

  * DONE: Success.
  * ABORT: Failure.

#### EV_COW

Handled in: Creature.

Invoked when the user chooses to attempt to cow a creature, or a group of creatures.

  * DONE: Success.
  * ABORT: Failure.

#### EV_CRIT
#### EV_CRITERIA

Handled in: Script hook possibility.

If game logic wishes to determine if a script class is suitable for a given use, it invokes this event against that class.  There are three possible return values:

 * SHOULD_CAST_IT: valid.
 * CAN_CAST_IT: valid.
 * CANNOT_CAST_IT: invalid.

This event is used in the game engine for the selection of templates to apply to monsters, encounters to spawn, and also in user dialogs to narrow down a selection of script classes to choose from (e.g. when using the Oil of Transformation).

#### EV_CUT
#### EV_DAMAGE
#### EV_DEATH
#### EV_DESCEND

Handled in: Creature.

Called to go down a level, climb down from a tree, descend from levitation or climb down a chasm.

  * DONE: Success.
  * ABORT: Failure.

#### EV_DIG
#### EV_DIP
#### EV_DISARM
#### EV_DISGUISE
#### EV_DISMISS
#### EV_DISMOUNT
#### EV_DISPELLED
#### EV_DISSECT
#### EV_DISTRACT
#### EV_DIVIDE
#### EV_DODGE
#### EV_DRINK
#### EV_DROP
#### EV_EAT
#### EV_EFFECT
#### EV_ELAPSED
#### EV_ENBUILD_MON
#### EV_ENCHOOSE_MID
#### EV_ENCHOOSE_TEMP
#### EV_ENGEN
#### EV_ENGEN_ALIGN
#### EV_ENGEN_MOUNT
#### EV_ENGEN_PART
#### EV_ENLIST
#### EV_ENSELECT_TEMPS
#### EV_ENTER
#### EV_ENTERSTORE
#### EV_EXAMINE
#### EV_FAST_TALK
#### EV_FIELDOFF
#### EV_FIELDON
#### EV_FORCE
#### EV_FUMBLE
#### EV_GAIN_STATI
#### EV_GENITEM
#### EV_GEN_DUNGEON
#### EV_GEN_LEVEL
#### EV_GEN_PANEL
#### EV_GEN_ROOM
#### EV_GETNAME
#### EV_GIVE
#### EV_GIVE_AID
#### EV_GODPULSE

Invoked in: Creature.
Handled in: Various god script classes.

Invoked during a creature's turn, as an indication of an angry god.

#### EV_GOD_RAISE

Invoked in: Player.
Handled in: Various god script classes.

Invoked when a player dies, first against their god if they have one.  Otherwise if the result is NOTHING, then against all remaining gods while the result continues to be NOTHING.

#### EV_GREET
#### EV_HIDE
#### EV_HIT
#### EV_IBLESSING
#### EV_IMBUE
#### EV_INFECT
#### EV_INITIALIZE
#### EV_INIT_COMP
#### EV_INIT_FIELDS
#### EV_INIT_STATI
#### EV_INSCRIBE
#### EV_INSERT
#### EV_INSIGHT
#### EV_INVOKE
#### EV_ISDETECT
#### EV_ISTARGET

Handled in: Script hook possibility.

If game logic wishes to determine if a script class can be applied to a given object, it invokes this event against that class.  There are three possible return values:

 * SHOULD_CAST_IT: Autobuff and monster AI will do so.
 * CAN_CAST_IT: Player can, monsters won't.
 * CANNOT_CAST_IT: Invalid target.

It isn't just used for spell classes, but also encounter generation and validating character creation choices.
 
#### EV_ITERRAIN
#### EV_JAM
#### EV_JEALOUSY
#### EV_JUMP
#### EV_JUMP_OFF
#### EV_LAND
#### EV_LEAVE
#### EV_LOADXBOW
#### EV_LOSEITEM
#### EV_MACRO
#### EV_MAGIC_HIT

Handled in: Creature, Item.

Called by EV_MAGIC_STRIKE if the target is not immune to the strike, and fails to resist it.  Or if the thing is EF_PARTIAL, provided by non-magic or a magic blast (?).

 * ABORT: Stop the hit from being applied.
 * NOTHING / DONE:  Allow the hit to proceed.

#### EV_MAGIC_STRIKE
#### EV_MAGIC_XY
#### EV_MINE
#### EV_MISS
#### EV_MISSILE
#### EV_MIX
#### EV_MON_CONSIDER
#### EV_MOUNT
#### EV_MOVE
#### EV_NATTACK
#### EV_OATTACK
#### EV_OPEN

Handled in: Door.

At this time, it is only called to attempt to open a door.

  * DONE: Success.
  * ABORT: Failure.
 
#### EV_ORDER
#### EV_PARRY
#### EV_PDAMAGE
#### EV_PHASE
#### EV_PICKLOCK
#### EV_PICKUP
#### EV_PICK_POCKET
#### EV_PLACE
#### EV_PLAY
#### EV_POISON
#### EV_POUR
#### EV_PRAY
#### EV_PREREQ
#### EV_PRESS
#### EV_PRINT_NAME
#### EV_PULL
#### EV_PUSH
#### EV_PUTON
#### EV_QUELL
#### EV_RATETARG

Handled in: Script hook possibility.

Applies to: Q_INV

Used to filter which inventory items are shown in a menu for the user to select from.

 * SHOULD_CAST_IT: Autobuff and monster AI will do so.
 * CAN_CAST_IT: Player can, monsters won't.
 * CANNOT_CAST_IT: Invalid target.

#### EV_RATE_SAC
#### EV_RATTACK
#### EV_READ
#### EV_REMOVE
#### EV_REMOVED
#### EV_REQUEST
#### EV_RESEARCH
#### EV_REST
#### EV_RESTING
#### EV_RETRIBUTION
#### EV_RUB
#### EV_SACRIFICE
#### EV_SATTACK
#### EV_SEARCH
#### EV_SHIFT
#### EV_SHOW
#### EV_SIT
#### EV_SMELL

Invoked in: Y use menu.
Handled in: Script hook possibility.

#### EV_SPECIAL

Invoked in: Creature.
Handled in: Terrain Script.

Invoked as terrain effect for a given creature, if the relevant terrain at the creature's location handles this event.

 * Each turn.
 * When the creature moves, or is moved, to that location.
 * Terrain type for the given location changes to a new type.

#### EV_STARTING_ITEM

Invoked in: Player.
Handled in: God, Race, and Class Scripts.

Notify various scripts about each starting item the player has.

#### EV_STRIKE

Invoked in: Various.
Handled in: Creature, Feature, Item.

Invoked to strike an object.

 * ABORT: ...
 * Otherwise: ...

#### EV_SUNRISE

Not implemented.

#### EV_SUNSET

Not implemented.

#### EV_SURRENDER

Invoked in: Player talk 'surrender' option.
Handled in: Creature.

Invoked when a player wishes to surrender to another creature.

#### EV_TAKEOFF

Invoked in: Monster
Handled in: Script hook possibility.

Invoked to give the chance that a worn object can be removed.

#### EV_TAKEOUT
#### EV_TALK
#### EV_TASTE
#### EV_TAUNT
#### EV_TDAMAGE
#### EV_TERMS

Invoked in: Player talk 'offer terms' option.
Handled in: Creature.

Invoked when a player is offering terms to another creature.

#### EV_TIE_TO

Invoked in: Y use menu.
Handled in: Script hook possiblity.

#### EV_TIE_UP

Invoked in: Y use menu.
Handled in: Script hook possiblity.

#### EV_TOUCH

Invoked in: Y use menu.
Handled in: Script hook possiblity.

#### EV_TURN

Invoked in: Creature.
Handled in: Creature, Various scripts.

Allow an object to play out a game turn.

#### EV_TURNING

Handled in: Creature.

Used to attempt to turn another creature (e.g. undead).

#### EV_TWIST

Invoked in: Y use menu.
Handled in: Script hook possiblity.

#### EV_UNLOCK

Not implemented.

#### EV_VICTORY

Invoked in: Feature script.
Handled in: Player.

Invoked to declare the player has won the game.

#### EV_WALKON

Handled in: Feature Script.

Invoked to verify whether a creature can walk on a map tile which holds a given feature.  For instance, a closed door cannot be walked on and will fail.  An open door however, can successfully be walked on.

 * ABORT: Disallow.
 * Otherwise: Allow.

#### EV_WATTACK

Handled in: Creature.
Invoked in: Various places.

Invoke character attack sequence.

#### EV_WAVE

Invoked in: Y use menu.
Handled in: Script hook possiblity.

#### EV_WEAR

Not implemented.

#### EV_WIELD

Handled in: Creature.
Invoked in: Various places.

Invoked to make the creature wield an item in a given slot.

#### EV_WILLBUY

Handled in: NPC Script.

Invoked on a barter attempt from the player.

 * DONE: Success.  Object is sold.
 * NOMSG: Abort but do not show a message.
 * Otherwise: Abort but show a default message.

#### EV_ZAP

Handled in: Item.

Invoked for use of wands and staffs.
