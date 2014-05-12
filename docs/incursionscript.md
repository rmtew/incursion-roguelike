# IncursionScript

## Events

### Event Types
#### EV_ACTIVATE
#### EV_ADAMAGE
#### EV_ADVANCE
#### EV_ALIGNED
#### EV_ANGER_PULSE
#### EV_APPLY
#### EV_ASCEND
#### EV_ATTACKMSG
#### EV_ATTK
#### EV_BARTER
#### EV_BIRTH
#### EV_BLESSING
#### EV_BLOCK
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
#### EV_COMPULSION
#### EV_CONDITION
#### EV_CONVERT
#### EV_CONVERT_ALTAR
#### EV_COW
#### EV_CRIT
#### EV_CRITERIA

If game logic wishes to determine if a script class is suitable for a given use, it invokes this event against that class.  There are three possible return values:

 * SHOULD_CAST_IT: valid.
 * CAN_CAST_IT: valid.
 * CANNOT_CAST_IT: invalid.

This event is used in the game engine for the selection of templates to apply to monsters, encounters to spawn, and also in user dialogs to narrow down a selection of script classes to choose from (e.g. when using the Oil of Transformation).

#### EV_CUT
#### EV_DAMAGE
#### EV_DEATH
#### EV_DESCEND
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
#### EV_GOD_RAISE
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

Applies to: Q_INV

Used to filter which inventory items are shown in a menu for the user to select from.

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
#### EV_SPECIAL
#### EV_STARTING_ITEM
#### EV_STRIKE
#### EV_SUNRISE
#### EV_SUNSET
#### EV_SURRENDER
#### EV_TAKEOFF
#### EV_TAKEOUT
#### EV_TALK
#### EV_TASTE
#### EV_TAUNT
#### EV_TDAMAGE
#### EV_TERMS
#### EV_TIE_TO
#### EV_TIE_UP
#### EV_TOUCH
#### EV_TURN
#### EV_TURNING
#### EV_TWIST
#### EV_UNLOCK
#### EV_VICTORY
#### EV_WALKON
#### EV_WATTACK
#### EV_WAVE
#### EV_WEAR
#### EV_WIELD
#### EV_WILLBUY
#### EV_ZAP
