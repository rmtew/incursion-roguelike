/* See the Incursion LICENSE file for copyright information.
 *
 * Possible Feat Prereq Elements:
 *
 * -> can it be duplicated (exotic weapon, weapon focus, skill focus,
 * school focus, weapon prof)
 * -> is it currently unimplemented (alluring, pain tolerance)
 * -> is it always available (sneaky, acrobatic, woodsman, imp str) 
 * -> it may require an element:
 *      another feat
 *      an ability ("guarded invocation" requires "ca_innate_spell")
 *      BAB[S_MELEE] >= 2       (e.g., "dirty fighting")
 *      BAB[S_ARCHERY] >= 3     (e.g., "rapid reload")
 *      SkillRanks[SK_...] >=   (e.g., "mantis leap, sk_jump >= 5") 
 *      ChallengeRating >= 9    (e.g., "Iron skin, CR >= 9")
 *      IAttr(x) >= y           (e.g., "loadbearer, a_str >= 13") 
 *      CasterLev >= 5          (e.g., "lore of acid, caster >= 5")
 *      Ability >= 5            (e.g., "bonded mount, ca_sacred_mount >=5")
 *      Wep_skill ...           (e.g., "weapon mastery, 
 *              HasStati(WEP_SKILL,WS_SPECIALIST)")
 *      HasStati(...)           (e.g., "weapon mastery, stati(ca_
 * -> conjunction of elements:
 *      "one body, one soul" requires "iron will" and "great fort" 
 * -> disjunction:
 *      "imp turning" requires "'ca_command" or "ca_turning"
 */
#include "Incursion.h"

/* IMPLICIT (handled elsewhere): 
 * all Metamagic Feats require Caster Level >= 1 */

/* Order:
   -- General Skill Feats
   -- Dual Skill Enhancers
   -- Second-Gen Skill Feats

*/

struct FeatInfoStruct FeatInfo[] = {

/******************************************************************************\
 *                           GENERAL SKILL FEATS                              *
\******************************************************************************/

  { FT_ALLURING,  0, "Alluring", 
    "You receive a +3 circumstance bonus on social checks against members "
                  "of the opposite gender and similar genotype to yourself;"
                  "demihuman and human characters can be attractive to other "
                  "humans and demihumans; reptilian characters can be "
                  "attractive to other kobolds, lizardfolk or dragons and "
                  "goblinoids can be attractive to other orcs, kobolds or "
                  "goblins." },
  { FT_SKILL_FOCUS, FF_MULTIPLE, "Skill Focus", 
    "You receive a +2 competence bonus to any one skill of your "
                  "choice. This bonus stacks with the bonuses from other skill "
                  "enhancer feats. Additionally, when you perform a skill check "
                  "for that skill, you roll twice and take the better result. If "
                  "the skill is normally rolled twice, you roll three times "
                  "instead." },
  { FT_NATURAL_APTITUDE, FF_MULTIPLE, "Natural Aptitude", 
     "__You may choose one additional skill, and this skill will be "
     "treated as if it were a racial skill; you can spend skill points "
     "gained from any class to advance it." }, 


/******************************************************************************\
 *                             DUAL SKILL FEATS                               *
\******************************************************************************/

  { FT_ALERTNESS, 0, "Alertness",
    "You are exceptionally alert, receiving a +3 circumstance bonus on "
    "all Spot and Listen checks." },
  { FT_ACROBATIC, 0, "Acrobatic",
    "You have a natural agility and grace in motion, and receive a +3 "
    "skill bonus to all Jump and Tumble checks. Additionally, you do not "
    "provoke attacks of opportunity by foes beside you when you jump."  },
  { FT_ATHLETIC,  0, "Athletic",
    "You are one fit and rugged adventurer, and your prowess gives you "
    "a +3 skill bonus on Athletics, Climb and Swim checks. In addition, you "
    "gain a +1 competence bonus to your Fortitude Saving Throw." },
  { FT_ARTIFICER, 0, "Artificer",
    "You have a natural talent for intricate craftwork, and receive a +3 "
    "skill bonus to Craft and Handle Device checks; you also require only "
    "2/3rds of the normal XP when crafting items that require spending "
    "experience." },
  { FT_EDUCATED,  0, "Educated",
    "Your diverse knowledge and scholarly acumen gives you a +3 skill bonus "
                  "to all Knowledge skill checks." },
  /*
  { FT_INSIGHTFUL, 0, "Insightful",
    "Your keen insight and good instincts about both people and things "
      "give you a +3 skill bonus to Appraise and Intuition checks." },
  */
  { FT_LARCENOUS, 0, "Larcenous", 
    "Your shady upbringing gives you a +3 skill bonus to Lockpicking "
      "and Pick Pocket checks. In addition, if you fail to disarm a "
      "trap you will never accidentally set it off." },
  { FT_CAPTIVATING, 0, "Captivating", 
    "You possess a natural social poise and a gift for showmanship, "
    "giving you a +3 bonus to Diplomacy and Perform, and gain 250 "
    "gp added starting wealth." },
  { FT_SNEAKY,    0, "Sneaky",
    "Stealth is second-nature to you, and you receive a +3 skill bonus to Hide "
    "and Move Silently checks. In addition, you can move at full speed (rather "
    "than half) while hiding in shadows." },
  { FT_WOODSMAN,  0, "Woodsman", "You have great experience in the wilderness, and this "
    "familiarity grants you a +3 skill bonus to all Wilderness Lore and Animal Empathy "
    "checks. In addition, you gain a +2 feat bonus to your total fatigue points." },
  
  { FT_GRACEFUL, 0, "Graceful", "You're naturally lithe and graceful, and gain a +3 "
    "feat bonus to your Balance, Escape Artist and Swim skill ratings." },
    
  { FT_DETECTIVE, 0, "Detective", "You've had training as a detective or investigator, "
    "and as such gain a +3 feat bonus to the Appraise, Search and Gather "
    "Information skills, and a +2 social modifier whenever dealing with city "
    "guardsmen or similar lawkeepers." },
  { FT_TALENTED, 0, "Talented", "You're got a natural gift for invoking magic, and "
    "thus gain a +3 bonus to the Decipher Script and Use Magic skills." },
  { FT_GUILDMAGE, 0, "Guildmage", "You've attended one of the large "
    "magic-guilds on Theyra, and your training has granted you a +3 bonus to "
    "the Metamagic and Spellcraft skills, as well as a bonus 1st-level spell "
    "slot.",
    {{{FP_CASTER_LEVEL,1,0}}}},
  { FT_PHYSICIAN, 0, "Physician", "You've completed formal training with the "
    "Physician's Guild of Theyra, and as a result have acquired a +3 feat bonus "
    "to Alchemy and Heal." },
  { FT_MURDEROUS, 0, "Murderous", "You're a real hardcase; your proficiency at "
    "ending others lives gives you a +3 feat bonus to Find Weakness and Poison "
    "Use, as well as a +2 bonus to the roll to confirm critical hits." },
  { FT_CLEAR_MINDED, 0, "Clear-Minded", "You've been trained in meditation and "
    "similar mental exercises, and as a result gain a +3 feat bonus to the "
    "Concentration and Intuition skills, as well as a +1 insight bonus to your "
    "Will saving throw." },
  { FT_LANDED_NOBLE, 0, "Landed Noble", "You grew up amidst the aristocracy, "
    "and thus gain a +3 feat bonus to Ride and Seneschal, as well as a +2 "
    "social modifier versus other aristocrats." },
  { FT_DECIEVER, 0, "Deciever", "You have a natural flair for deception, and "
    "gain a +3 feat bonus to Bluff, Disguise and Illusioncraft." },
  
  
/******************************************************************************\
 *                          NON-TREE COMBAT FEATS                             *
\******************************************************************************/

    
       
  { FT_SWARM,       0, "Swarm Tactics",
    "You have an affinity for swarm tactics. Whenever you make an attack and your "
    "victim is adjacent to you, you gain a +1 bonus to hit and damage for every "
    "creature within one square of you that is neither hostile to you nor larger "
    "than you. You gain a similar +1 bonus to defense when attacked by an adjacent "
    "foe for every non-hostile, non-larger creature within one square of you.",
    {{{FP_ATTR,A_WIS,13}}}},

  { FT_BACKSTAB, 0, "Backstab",
    "You are adept at making surprise attacks. You gain a +4 bonus on attack rolls when "
    "making a surprise attack (e.g., because the enemy is surprised, flat-footed, flanked, "
    "exposed, charging, grappled, stuck or stunned).",
    {{{FP_ABILITY,CA_SNEAK_ATTACK,0,
      FP_BAB, S_MELEE, 2},
     {FP_FEAT, FT_SNEAKY,0,
      FP_BAB, S_MELEE, 2}}}},

  { FT_IMPROVED_CRIT, FF_MULTIPLE, "Improved Critical", 
    "Your martial skill with a specific weapon allows you to land deadly blows "
    "more often than others. The threat range of one specific type of weapon "
    "you choose is doubled." ,
    {{{FP_BAB,S_MELEE,8}}}},


/******************************************************************************\
 *                          GENERAL/QUIRKY FEATS                              *
\******************************************************************************/

  { FT_LIGHT_SLEEPER, 0, "Light Sleeper",
    "Your incredible instincts allow you to remain aware of danger even as "
    "you sleep, waking on a hair trigger and ready to respond to threats. In "
    "game terms, if you have an encounter while resting, the monsters will "
    "not get free actions ahead of you, you will not be subject to the Coup "
    "de Grace maneuver and you will not be flat-footed.",
    {{{FP_FEAT,FT_ALERTNESS,0},
      {FP_ATTR,A_WIS,13}}}},

  { FT_ACUTE_SENSES, 0, "Acute Senses",
    "Your senses extend out beyond their normal ranges for your race. Your sight, "
    "shadow, infravision and scent ranges are increased by 50~.",
    {{{FP_FEAT,FT_ALERTNESS,0}}}},

  { FT_BRACHIATION, 0, "Brachiation",
    "You can move through the trees at your normal land speed by swinging from one branch to "
    "another with no risk of falling. Descending from a tree takes you no time. You automatically "
    "ascend a tree when you move into a square containing one. Ascending a tree takes you no time.",
    {{{FP_SKILL,SK_CLIMB,10,
       FP_SKILL,SK_JUMP,10,
       FP_ATTR,A_STR,13}}}},

  { FT_NECROPHYSIOLOGY,  0, "Necrophysiology",
    "Your understanding of the makeup of the undead allows you to score critical hits "
    "against them in combat. This allows you to use abilities that normally only work "
    "against creatures subject to critical hits, such as Coup de Grace and Sneak Attack. "
    "Note that undead always succeed at Fortitude saves against a Coup de Grace.",
    {{{FP_SKILL,SK_KNOW_UNDEAD,10}}}},

  { FT_LOADBEARER, 0, "Loadbearer",
    "You are well-practiced at carrying a burden without it overly "
      "damaging your capabilities. You suffer no penalties when at Light "
      "or Medium encumbrance, and half the normal penalties thereafter." ,
      {{{ FP_ATTR, A_STR, 13}}} },
  { FT_QUICK_DRAW, FF_WARRIOR, "Quick Draw",
    "__You are able to exchange items in and out of your weapon hand, ready "
    "hand, shoulder and belt slots very quickly -- it takes you only 1/4th "
    "the normal time to do so. It still takes time to remove items from containers "
    "or pick things up." },

  { FT_FASTER_THAN_THE_EYE, 0, "Faster Than The Eye",
    "You organize and improvise your inventory exceptionally well, and can get at anything "
    "you need quickly. This feat grants the following benefits:\n"
    "\n__Inventory actions "
    "(wielding a weapon, taking an item out of a container, putting an item "
    "into a container, dumping a container, picking an item up from the ground) "
    "take you 1/4th the normal time (even during combat) and never provoke attacks of "
    "opportunity.\n"
    "__When you insert an item into a container you treat the container's weight "
    "and item count capacity as if they were doubled. You may insert items one size "
    "larger than that container would normally allow.",
    {{{ FP_FEAT, FT_QUICK_DRAW, 0}}} },

  { FT_RUN,        0, "Run",
    "You receive a 20% feat bonus to your Movement Rate. In addition, you are not "
    "denied your Dexterity bonus to defense when charging and not mounted." },

  { FT_SCRIBE_SCROLL, 0, "Scribe Scroll",
    "Using this feat, a character can create magical scrolls. This requires "
    "a materials cost equal to a third of the scroll's market price in gold "
    "pieces, as well as an experience cost equal to 1/25th the market price "
    "of the scroll. The number of scroll formulas a character knows is equal "
    "to twice her Knowledge (Magic) skill; additionally, she can scribe a "
    "scroll of any spell she knows how to cast.",
    {{{FP_ABILITY,CA_SPELLCASTING,1},
      {FP_SKILL, SK_KNOW_MAGIC, 10}}}},
      
  { FT_BREW_POTION, 0, "Brew Potion",
    "Using this feat, a character can brew magical potions. This requires "
    "a materials cost equal to a third of the potion's market price in gold "
    "pieces, as well as an experience cost equal to 1/25th the market price "
    "of the potion. The number of potion formulas a character knows is equal "
    "to the sum of her Knowledge (Magic) and Alchemy skills. Additionally, "
    "knowing a given spell grants the ability to brew potions that have a "
    "very similar effect to it -- for example, being able to cast <9>cure "
    "light wounds<7> would allow a character to brew Potions of Healing, "
    "and knowing <9>scorcher<7> would allow a character to brew Potions of "
    "Fire Breath. These formulas are in addition to any the character may "
    "have because of her skill ratings.",
    {{{FP_ABILITY,CA_SPELLCASTING,1},
      {FP_SKILL, SK_ALCHEMY, 10}}}},
  

/******************************************************************************\
 *                               FEATS FOR MONSTERS                           *
\******************************************************************************/

  { FT_FLYBY_ATTACK, 0, "Flyby Attack",
    "You do not suffer attacks of opportunity from foes that are not "
      "airbourne, provided that you are aerial.",
    {{{FP_ABILITY, CA_FLIGHT, +1 }}}},
  { FT_HOVER,        0, "Hover",
    "You do not suffer the usual -4 penalty to ranged weapon attacks "
      "while flying.",
    {{{FP_ABILITY, CA_FLIGHT, +1 }}}},
  { FT_MULTIATTACK,  0, "Multiattack",
    "When making your second and further attacks in your natural attack "
      "sequence, you suffer only a -2 penalty, rather than a -5.",
    {{{FP_ATTACK, A_CLAW, 0}}}},
  { FT_POUNCE,       FF_MONSTER, "Pounce",
    "When you attack a flat-footed enemy, you may make all of the attacks "
    "in your natural attack sequence at your full base attack bonus. Normally "
    "the subsequent attacks in a natural attacks are at a -5 penalty (or a -2 penalty"
    " with the Multiattack feat)." },
  { FT_RAKE,         FF_MONSTER, "Rake",
    "If you hit with two or more claw attacks in one natural attack sequence, "
      "you can make one further claw attack against that creature at your full "
      "attack bonus and full Strength damage modifier using your hind legs." },
  { FT_NATURAL_GRAB, FF_MONSTER, "Natural Grapple",
    "The first time you hit in your natural (pre-grab) attack sequence you "
    "immediately initiate a free grapple attack against your foe. You do not "
    "provoke attacks of opportunity when making grapple attacks. You may make "
    "grapple attacks even if you lack hands. If you also have the <6>Multiattack<7> "
    "feat, the free grab attack applies after any successful hit in your natural "
    "attack sequence.", 
      {{{FP_BAB,S_BRAWL,3}}}},
  { FT_REND,         FF_MONSTER, "Rend",
    "If you have grabbed your target before attacking, or you have hit with one "
    "natural attack already this attack sequence, you add your full Strength modifier "
    "to all further natural attacks. Normally all subsequent attacks in a natural attack "
    "sequence gain only half your Strength bonus." },
  { FT_TRAMPLE,      FF_MONSTER, "Trample",
    "Whenever you attack a smaller creature who is prone at the end of your turn "
    "(whether as a result of your own attacks or otherwise) with your natural attack "
    "sequence, you may make a special trample attack as part of your natural attack "
    "sequence, striking at full attack bonus for 1d12 points of blunt damage, plus "
    "Strength modifier." },

/******************************************************************************\
 *                                 FEATS FOR MONKS                            *
\******************************************************************************/

  { FT_NON_STANDARD_FLURRY, 0, "Non-Standard Flurry", 
    "You come from a monastic tradition that uses non-standard weapons. You may make "
    "a Flurry of Blows with any weapon, not just a martial one. " ,
    {{{FP_BAB,S_MELEE,4,  FP_ABILITY,CA_FLURRY_OF_BLOWS}}}},
  { FT_BREAK_THE_BREATH, 0, "Break the Breath",
    "Your stunning attack is particularly potent and you add +2 to the save DC for "
    "your stunning blows.",
    {{{FP_ABILITY,CA_STUN_ATTACK,1,
       FP_ATTR,A_WIS,15}}}},
  { FT_LIGHTNING_FISTS, 0, "Lightning Fists",
    "You receive a 5% bonus to the Speed of your unarmed "
    "strikes for every point of your Dexterity bonus." ,
    {{{FP_ATTR,A_DEX,13,
       FP_ABILITY,CA_UNARMED_STRIKE,1},
      {FP_ATTR,A_DEX,13,
       FP_FEAT,FT_MARTIAL_MASTERY,1}}}},
  { FT_FISTS_OF_IRON, 0, "Fists of Iron",
    "You suffer only half damage from the act of attacking something with your "
    "natural attack sequence (e.g., you will be burned less by punching a fire "
    "elemental). Additionally, you receive a +1 bonus to damage with unarmed "
    "attacks.",
    {{{FP_BAB,S_BRAWL,2}}}},
  { FT_IRON_SKIN, 0, "Iron Skin", 
    "Your body and soul are strong enough to withstand the heaviest blows. You gain a "
    "+5 natural armour bonus.",
    {{{ FP_FEAT, FT_TOUGHNESS, 0,
        FP_ATTR, A_CON, 17,
        FP_ATTR, A_SAV_FORT, 10 },
      { FP_ABILITY, CA_UNARMED_STRIKE, 1,
        FP_FEAT, FT_FISTS_OF_IRON, 0,
        FP_ATTR, A_WIS, 15,
        FP_ATTR, A_SAV_FORT, 6 }}}},
        
  { FT_REED_IN_THE_WIND, FF_AGILE, "Reed in the Wind",
    "Whenever a non-prone humanoid character with this feat is about to "
    "take more than a third of their current hit points in damage "
    "from a single attack consisting of slashing, piercing or "
    "blunt damage, they will be asked if they want want to use "
    "Reed in the Wind, and thus suffer only 1/3rd damage, but "
    "also suffer automatic knockdown. A character must be wearing "
    "Light armour or be unarmoured to use this feat, and it is "
    "ineffective against piercing damage.",
    {{{ FP_ABILITY, CA_EVASION, 1,
        FP_SKILL, SK_TUMBLE, 12,
        FP_ATTR, A_DEX, 15,
        FP_FEAT, FT_DODGE, 0 }}}},
        
  { FT_POWER_KICK, FF_KICK, "Power Kick",
    "You gain a +2 bonus to all damage on kick attacks.",
    {{{ FP_ABILITY, CA_UNARMED_STRIKE, 1,
        FP_FEAT, FT_POWER_ATTACK, 0,
        FP_SKILL, SK_ATHLETICS, 7 },
      { FP_FEAT, FT_MARTIAL_MASTERY, 1,
        FP_FEAT, FT_POWER_ATTACK, 0,
        FP_SKILL, SK_ATHLETICS, 7 }}}},
    
  { FT_CIRCLE_KICK, FF_KICK, "Circle Kick",
    "At the cost of one fatigue point, you execute a circle kick "
    "striking a creature in the direction you choose, and also any "
    "creatures in the squares beside both it and you. Only creatures "
    "of Large size or smaller have enough maneuverability in the "
    "dungeon to perform this maneuver. You can perform a Circle Kick "
    "by selecting it from the Activate menu.",
    {{{ FP_SKILL, SK_JUMP, 10,
        FP_FEAT, FT_POWER_KICK, 0,
        FP_BAB,  S_BRAWL, +3
        }}}},
  { FT_FLYING_KICK, FF_KICK, "Flying Kick",
    "If you perform a kick attack at the end of a charge, you "
    "inflict double damage.",
    {{{ FP_SKILL, SK_JUMP, 13,
        FP_FEAT, FT_POWER_KICK, 0,
        FP_BAB,  S_BRAWL, +5
        }}}},        
        
  { FT_EARTHS_EMBRACE, 0, "Earth's Embrace",
    "Every turn you maintain a grapple on another creature, that "
    "creature suffers your unarmed damage plus your Strength "
    "modifier. This is automatic blunt damage and does not require "
    "an attack roll, nor does it cost you time.",
    {{{ FP_ABILITY, CA_UNARMED_STRIKE, 1,
        FP_FEAT, FT_MASTER_GRAPPLE, 0,
        FP_BAB, S_BRAWL, 4 },
      { FP_FEAT, FT_MARTIAL_MASTERY, 1,
        FP_FEAT, FT_MASTER_GRAPPLE, 0,
        FP_BAB, S_BRAWL, 5 }}}},
  { FT_CHOKE_HOLD, 0, "Choke Hold", 
    "For every turn a character with this feat maintains a grapple "
    "on a living humanoid with less than half their hit points, that "
    "creature must make a Fortitude save against the grappler's stunning "
    "fist DC or be rendered unconscious for an extended period. Characters "
    "receive experience for foes felled in this manner.",
    {{{ FP_ABILITY, CA_UNARMED_STRIKE, 1,
        FP_FEAT, FT_MASTER_GRAPPLE, 0,
        FP_BAB, S_BRAWL, 4 },
      { FP_FEAT, FT_MARTIAL_MASTERY, 1,
        FP_FEAT, FT_MASTER_GRAPPLE, 0,
        FP_BAB, S_BRAWL, 5 }}}},    
   
  { FT_DEFENSIVE_THROW, 0, "Defensive Throw",
    "When a humanoid foe swings at you and misses your Defense Class by 8 "
    "or more, and you are unarmed, you can execute a trip attack against "
    "that foe. This costs you one attack of opportunity for the round, "
    "but has no actual timecost for you.",
    {{{ FP_ABILITY, CA_UNARMED_STRIKE, 1,
        FP_FEAT, FT_MASTER_GRAPPLE, 0,
        FP_FEAT, FT_MASTER_TRIP, 0 },
      { FP_FEAT, FT_MARTIAL_MASTERY, 1,
        FP_FEAT, FT_MASTER_GRAPPLE, 0,
        FP_FEAT, FT_MASTER_TRIP, 0 }}}},    
  
  { FT_GREAT_THROW, 0, "Great Throw",
    "When your Defensive Throw feat is triggered and the foe is knocked "
    "prone successfully, they also suffer 1d3+[Str mod] squares of "
    "knockback.",
    {{{ FP_FEAT, FT_DEFENSIVE_THROW, 0,
        FP_ATTR, A_STR, 15,
        FP_BAB, S_BRAWL, +6 }}}},

  { FT_PAIN_TOUCH, 0, "Pain Touch",
    "You are able to strike nerve clusters in such a manner as to cause "
    "intense pain. When you perform a successful stun attack, your foe "
    "also becomes nauseated for one round.",
    {{{ FP_ABILITY, CA_STUN_ATTACK, 0,
        FP_FEAT, FT_LIGHTNING_FISTS, 0,
        FP_SKILL, SK_FIND_WEAKNESS, 7,
        FP_ATTR, A_WIS, 19,
        FP_BAB, S_BRAWL, +2 }}}},
  { FT_FALLING_STAR_STRIKE, 0, "Falling Star Strike",
    "When a monk with this feat punches a victim with eyes, the creature has a "
    "chance of being blinded for 3d6 rounds. On a critical hit, great blow or "
    "1 in 3 normal attacks, the victim must make a Fortitude save "
    "(against stunning fist save DC) or be so blinded. Only living "
    "humanoids can be blinded.",
    {{{ FP_ABILITY, CA_UNARMED_STRIKE, 1,
        FP_FEAT, FT_LIGHTNING_FISTS, 0,
        FP_SKILL, SK_FIND_WEAKNESS, 10,
        FP_BAB, S_BRAWL, +4 },
      { FP_FEAT, FT_MARTIAL_MASTERY, 0,
        FP_FEAT, FT_LIGHTNING_FISTS, 0,
        FP_SKILL, SK_FIND_WEAKNESS, 10,
        FP_BAB, S_BRAWL, +4 }}}},  
  { FT_FREEZE_THE_LIFEBLOOD, 0, "Freeze the Lifeblood",
    "When a monk with this feat punches a victim whose CR is two or more lower than "
    "her own, the creature has a chance of being paralyzed for 1d4+1 rounds."
    " On a critical hit, great blow or 1 in 3 normal attacks, the victim must make a Fortitude save "
    "(against stunning fist save DC) or be rendered immobile.  Only living "
    "creatures can be paralyzed.",
    {{{ FP_ABILITY, CA_UNARMED_STRIKE, 1,
        FP_FEAT, FT_LIGHTNING_FISTS, 0,
        FP_SKILL, SK_FIND_WEAKNESS, 12,
        FP_BAB, S_BRAWL, +6 },
      { FP_FEAT, FT_MARTIAL_MASTERY, 0,
        FP_FEAT, FT_LIGHTNING_FISTS, 0,
        FP_SKILL, SK_FIND_WEAKNESS, 12,
        FP_BAB, S_BRAWL, +6 }}}},  
  { FT_CHI_STRIKE, 0, "Chi Strike",
    "When you hit a foe with a successful punch attack, they lose "
    "1d8 points of mana for every hit points of damage "
    "that you inflict. This mana does not regenerate. A Fortitude "
    "save (DC as per your stunning attack) negates the effect.",
    {{{ FP_ABILITY, CA_UNARMED_STRIKE, 1,
        FP_FEAT, FT_LIGHTNING_FISTS, 0,
        FP_SKILL, SK_FIND_WEAKNESS, 7,
        FP_BAB, S_BRAWL, +8 },
      { FP_FEAT, FT_MARTIAL_MASTERY, 0,
        FP_FEAT, FT_LIGHTNING_FISTS, 0,
        FP_SKILL, SK_FIND_WEAKNESS, 10,
        FP_BAB, S_BRAWL, +8 }}}},          

/******************************************************************************\
 *                               ATTRIBUTE ENHANCERS                          *
\******************************************************************************/

  { FT_IMPROVED_STRENGTH, 0, "Improved Strength", 
    "Your might is truly legendary -- you receive a +1 feat bonus to your Strength score." },
  { FT_IMPROVED_DEXTERITY, 0, "Improved Dexterity", 
    "Your agility is peerless; you receive a +1 feat bonus to your Dexterity score." },
  { FT_IMPROVED_CONSTITUTION, 0, "Improved Constitution", 
    "You are a peak physical specimen of your race, and you receive a +1 feat bonus to your Constitution score." },
  { FT_IMPROVED_INTELLIGENCE, 0, "Improved Intelligence", 
    "Your intellect is unparalleled, giving you a +1 feat bonus to your Intelligence score." },
  { FT_IMPROVED_WISDOM, 0, "Improved Wisdom", 
    "Your instincts have saved your life more times than you can count. Gain a +1 feat bonus to Wisdom." },
  { FT_IMPROVED_CHARISMA, 0, "Improved Charisma", 
    "You have a magnetic personality, and benefit from a +1 feat bonus to your Charisma." },
  { FT_IMPROVED_LUCK, 0, "Improved Luck", 
    "Fortune simply smiles on you, as represented by your +1 feat bonus to Luck." },

/******************************************************************************\
 *                             CLASS FEATURE BOOSTERS                         *
\******************************************************************************/

  { FT_INTENSIVE_STUDY, FF_MULTIPLE, "Intensive Study",
     "You've devoted an exceptional amount of time and effort to staying ahead "
     "of the curve in both (or all) or your chosen professions, and thus you have "
     "an edge over other multi-classed characters. Rangers, paladins and bards can "
     "also use it to advance their magical studies more rapidly.\n"
     "__This feat may be selected multiple times; each time you can selecte one of "
     "the following benefits. The benefits may also be selected more than once.\n"
     "__<9>Improve Caster Level:<7> You gain one spellcasting chart level, up to a "
     "maximum of either double your natural casting level or 4/5ths of your "
     "character level, whichever is less. This both increases your caster level "
     "and gives you access to the higher level spells on the spellcasting chart.\n"
     "__<9>Improve Stealth and Sneak Attack:<7> You gain +1d6 sneak attack and +2 "
     "maximum ranks in Hide in Shadows and Move Silently, up to a maximum of what "
     "a Rogue of either twice your rogue level or 4/5ths your character level, "
     "whichever is lower, would have.\n"
     "__<9>Improve Wild Shape:<7> Your effective druid level is increased by 2, "
     "only for the purposes of Wild Shapes, to a maximum of twice your real "
     "druid level, or 4/5ths your character level, whichever is lower.\n"
     "__<9>Improve Bardic Ability:<7> Your effective bard level is increased by 2, "
     "only for the purposes of Bardic Music, Legend Lore and Storycraft, to a "
     "maximum of twice your real bard level, or 4/5ths your character level, "
     "whichever is lower. You don't gain any additional Storycraft plusses, but "
     "the maximum level of item you can augment is increased.\n"
     "__<9>Improve BAB:<7> Your base attack bonus increases by +1, to a maximum "
     "of twice your sum total of levels in warrior, barbarian, paladin or ranger "
     "or your total character level, whichever is lower.\n"
     "__<9>Improve Turn/Command Undead:<7> Your effective priest level is increased by 2, "
     "only for the purposes of turning or commanmding undead, to a maximum of twice your real "
     "priest level, or 4/5ths your character level, whichever is lower.\n"
     "__<9>Improve Unarmed Attack:<7> Your effective monk level is increased by 2, "
     "only for the purposes of determining your base unarmed attack damage and stunning "
     "attack save DC, to a maximum of twice your real monk level, or 4/5ths your character "
     "level, whichever is lower.\n"
     "__<9>Improve Smite:<7> Your level in the smite ability (the paladin's "
     "Smite Evil, or the cleric of Destruction domain power) is increased by 4, "
     "to a maximum of twice your real smite level, or 4/5ths your character "
     "level, whichever is lower.\n"
     "__<9>Improve Sacred Mount:<7> Your effective paladin level is increased by 3, "
     "only for the purposes of your mount, to a maximum of twice your real "
     "paladin level, or 4/5ths your character level, whichever is lower." ,
     {{{FP_ATTR,A_INT,13}}}},

  { FT_ANIMAL_BOND, 0, "Animal Bond", 
    "You have a knack for bonding with animals that are paragons of "
      "their type. When computing Druid Animal Companion additional powers you "
      "add 2 to your level. Note that this does not affect the "
      "types of animals available to you, only the power of animals you receive." , 
    {{{ FP_ABILITY, CA_ANIMAL_COMP}}}},
    
  { FT_IMPROVED_MOUNT, 0, "Improved Mount", 
    "You have a knack for bonding with sacred mounts that are exceptional "
      "for your degree of skill. Your paladin level is treated as if it were "
      "two higher than it actually is when determining what kind of creatures "
      "you can summon as a bonded mount.",
    {{{ FP_ABILITY, CA_SACRED_MOUNT,5}}}},

  { FT_DOMAIN_FOCUS, 0, "Domain Focus",
    "Your domain spells are especially potent and the specially-granted magic of "
    "your god is difficult to resist. You add +2 to the Difficulty Class for all saving "
    "throws against spells that appear on your domain spell list.", 
    {{{FP_ABILITY,CA_DOMAINS}}}},
    
  { FT_DOMAIN_MASTERY, 0, "Domain Mastery",
    "Your domain spells are especially potent and the specially-granted magic of your "
    "god is difficult to resist. You add an additional +2 (for a total of +4) to the "
    "Difficult Class for all saving throws against spells that appear on your domain "
    "spell list.", 
    {{{FP_FEAT,FT_DOMAIN_FOCUS}}}},

  { FT_WILD_SHAPE_MASTERY, 0, "Wild Shape Mastery",
    "You have a superior understanding of druidic shapeshifting. You may assume Wild "
    "Shapes as if you had two additional Druid levels. This allows you access to more "
    "powerful forms earlier, and allows you to take a Wild Shape with only one Druid level." ,
    {{{ FP_ABILITY, CA_WILD_SHAPE}}}},

  { FT_WILD_SHAPE_SCENT, 0, "Wild Shape Scent",
    "You have a superior understanding of druidic canine shapeshifting. You gain a "
    "wolf-like ability to Scent creatures within a range of 30 feet (3 squares), "
    "regardless of your physical form.", 
    {{{ FP_ABILITY, CA_WILD_SHAPE, 0,
        FP_ATTR, A_WIS, 11}}}},

  { FT_FAST_WILD_SHAPE, 0, "Fast Wild Shape",
    "You have a superior understanding of druidic shapeshifting. Taking on a wild "
    "shapes requires no time for you (instead of a full round).",
    {{{ FP_ABILITY, CA_WILD_SHAPE}}}},

  { FT_WILD_SHAPE_PLANT, 0, "Wild Shape: Plant",
    "You have a superior understanding of druidic shapeshifting. You may assume "
    "Plant Wild Shapes. You are still limited by the standard challenge rating, "
    "hit dice and size requirements." ,
    {{{ FP_ABILITY, CA_WILD_SHAPE, 6}}}},

  { FT_WILD_SHAPE_MYTHIC, 0, "Wild Shape: Mythic Hybrids",
    "You have a superior understanding of druidic shapeshifting. You may assume "
    "Mythic Hybrid Wild Shapes. You are still limited by the standard challenge "
    "rating, hit dice and size requirements." ,
    {{{ FP_ABILITY, CA_WILD_SHAPE, 8}}}},

  { FT_WILD_SHAPE_VERMIN, 0, "Wild Shape: Vermin",
    "You have a superior understanding of druidic shapeshifting. You may assume "
    "Vermin Wild Shapes. You are still limited by the standard challenge rating, "
    "hit dice and size requirements." ,
    {{{ FP_ABILITY, CA_WILD_SHAPE, 10}}}},

  { FT_WILD_SHAPE_ELEMENTALKIN, 0, "Wild Shape: Elementalkin",
    "You have a superior understanding of druidic shapeshifting. You may assume "
    "Elementalkin Wild Shapes. You are still limited by the standard challenge rating, "
    "hit dice and size requirements." ,
    {{{ FP_ABILITY, CA_WILD_SHAPE, 16}}}},

/******************************************************************************\
 *                             CLERIC TURNING BOOSTERS                        *
\******************************************************************************/

  { FT_IMPROVED_TURNING, 0, "Improved Turning", "You have a knack for exercizing "
    "divine authority and channeling sacred energies. You gain a +4 bonus to all "
    "Turn or Command attempts." ,
    {{{FP_ABILITY,CA_COMMAND},{FP_ABILITY,CA_TURNING}}}
  },
  
  { FT_DIVINE_ARMOUR, 0, "Divine Armour", 
    "You can channel divine energy to protect yourself from certain attacks. You "
    "gain a resistance to aligned damage (Law, Chaos, Holy and Unholy) as well as "
    "necromatic damage equal to twice your Charisma modifier for [Charisma x 2] "
    "rounds after turning undead.",  
    {{{FP_ABILITY,CA_TURNING,0,
       FP_ATTR,A_CHA,13}}}
  }, 
  
  { FT_DIVINE_RESISTANCE, 0, "Divine Resistance", 
    "You can channel divine energy to protect yourself from certain elemental attacks. "
    "You gain a resistance equal to your Charisma modifier to fire, cold and lightning "
    "for [Charisma x 2] rounds after turning undead.", 
    {{{FP_ABILITY,CA_TURNING,0,FP_ATTR,A_CHA,13,
       FP_FEAT, FT_ENDURANCE, 0}}}
  }, 
  { FT_DIVINE_VIGOR, 0, "Divine Vigor", 
    "You can channel divine energy to power your physical actions. You gain a +2 sacred "
    "bonus to your Constitution and a 5~ increase to your base movement rate per point "
    "of your secondary Charisma modifier, for [Charisma x 2] rounds after turning undead.", 
    {{{FP_ABILITY,CA_TURNING,0,FP_ATTR,A_CHA,13}}}
  }, 

  { FT_DIVINE_MIGHT, 0, "Divine Might", 
    "You can channel divine energy to aid your local attacks. You add you secondary "
    "Charisma modifier as a sacred bonus to your melee and brawl damage for [Charisma "
    "x 2] rounds after turning undead.",
    {{{FP_ABILITY,CA_TURNING,0,
       FP_ATTR,A_CHA,13,
       FP_FEAT,FT_POWER_ATTACK,0}}}
  }, 

/******************************************************************************\
 *                             SAVING THROW BOOSTERS                          *
\******************************************************************************/


  { FT_LIGHTNING_REFLEXES, 0, "Lightning Reflexes", 
    "You have unearthly reflexes, and thus receive a +3 feat bonus to all Reflex saving throws." },
  { FT_GREAT_FORTITUDE, 0, "Great Fortitude", 
    "You're tougher than nails, and thus receive a +3 feat bonus to all Fortitude saving throws." },
  { FT_IRON_WILL, 0, "Iron Will",
    "You have a resolute psyche, and thus gain a +3 feat bonus to all Will saving throws." },

  { FT_ONE_BODY_ONE_SOUL, 0, "One Body, One Soul", 
    "You have tapped into the innermost depths of your being, where the mental and "
    "physical become one. You can use the higher of your Wisdom and Constitution "
    "modifiers for the purposes of calculating your hit points, mana points, fatigue "
    "points, Fortitude and Will saving throws. Where applicable, this benefit is "
    "retroactive." ,
    {{{FP_FEAT,FT_IRON_WILL,0, 
       FP_FEAT,FT_GREAT_FORTITUDE,0}}}
  },

  { FT_LION_HEART, FF_WARRIOR, "Lionheart", 
    "Your courage in unequaled. You have a +2 bonus to saving throws vs. fear, "
    "morale penalties (as from <9>doom<7>) are halved for you, and you may attack as "
    "if <9>slowed<7> and at a -4 penalty when you have the Afraid Stati. Your "
    "movement is not restricted by being Afraid." ,
    {{{FP_ATTR,A_WIS,14}}}
     },


  { FT_PAIN_TOLERANCE, FF_WARRIOR, "Pain Tolerance",
    "You are naturally resistant to pain, and the modifiers that it "
      "causes you are all halved.",
	  {{{ FP_ATTR, A_CON, 13 }}}
  },

  { FT_STUBBORN_MIND, FF_UNIMP, "Stubborn Mind", 
    "You fight like a wild bull against mental coercion. The duration "
      "of all mental effects against you is halved." },




/******************************************************************************\
 *                      WEAPON PROFICIENCY & SPECIALIZATION                   *
\******************************************************************************/

  { FT_AP_LIGHT,  0, "Armour Proficiency: Light", 
    "You are able to wear Light armour without it severely impeding your ability to function.",
    {{{FP_NOT_PROF, WG_LARMOUR, 0}}} },
  { FT_AP_MEDIUM,  0, "Armour Proficiency: Medium",
    "You are able to wear Medium armour without it severely impeding your ability to function." ,
    {{{FP_PROF,WG_LARMOUR, 0,
       FP_NOT_PROF, WG_MARMOUR, 0}}} },
  { FT_AP_HEAVY,  0, "Armour Proficiency: Heavy",
    "You are able to wear Heavy armour without it severely impeding your ability to function." ,
    {{{FP_PROF,WG_MARMOUR, 0,
       FP_NOT_PROF,WG_HARMOUR, 0}}} },
  { FT_ARMOUR_OPTIMIZATION, FF_WARRIOR, "Armour Optimization",
    "You are adept at performing tasks (e.g., balancing, escaping from holds, "
    "hiding in shadows) while wearing heavy armour and using shields. You suffer "
    "only one-third of the normal Skill Check penalty (rounded up) for wearing "
    "armour or using a shield. You suffer only one-third (rounded up) of the "
    "movement rate penalty for wearing armour. You still suffer the full Arcane "
    "spell failure rate and maximum Dexterity modifier limit, however.", 
    {{{FP_PROF,WG_HARMOUR}}} },

  { FT_SHIELD_PROFICIENCY, 0, "Shield Proficiency",
    "You have been trained in the proper use of a shield, "
    "and do not suffer penalties as a result of wearing one.",
    {{{FP_NOT_PROF,WG_SHIELDS}}}  },
  { FT_SPEAR_AND_SHIELD, FF_WARRIOR, "Spear and Shield",
    "You have trained in the use of a large spear and shield together. You can "
    "use a spear together with a large sheild, despite the fact that such weapons "
    "cannot normally be used together. The shield is slung over the shoulder and "
    "maneuvered with the off-hand that simultaneously holds the spear. Effectively, "
    "spears, lances and polearms count as one size smaller for you when you are already "
    "wielding a shield.",
    {{{FP_FEAT,FT_SHIELD_PROFICIENCY}}} },

  { FT_SHIELD_FOCUS, FF_WARRIOR, "Shield Focus",
    "You are particularly adept at using shields. You gain a +2 increase to the "
    "defense bonus a shield grants.",
    {{{FP_FEAT,FT_SHIELD_PROFICIENCY}}} },

  { FT_SHIELD_EXPERT, FF_WARRIOR, "Shield Expert",
    "You are able to use your shield as a barrier, preventing foes from striking "
    "at the weak areas in your armour. Your shield grants you a bonus to coverage "
    "based on it's size -- +10 for a shield larger than you, +8 for a shield with "
    "the same size category, +4 for a shield one category smaller and +2 for a "
    "shield two categories smaller.",
    {{{FP_FEAT,FT_SHIELD_PROFICIENCY}}} },


  { FT_EXOTIC_WEAPON, FF_WARRIOR|FF_MULTIPLE, "Exotic Weapon",
    "You are proficient with one Exotic weapon of your choice. "
      "You must already be proficient in all the groups that weapon "
      "belongs to in order to choose this feat for it. For example, "
      "a character that lacks the Blades weapon group proficiency "
      "cannot buy Exotic Weapon: Two-Bladed Sword. Some weapons, like "
      "the bastard sword, are not exotic weapons proper, but require "
      "the Exotic Weapon feat to be used proficiently one-handed." 
      },

  { FT_MARTIAL_MASTERY, FF_WARRIOR|FF_UNIMP, "Martial Mastery",
    "You are trained in making deadly unarmed attacks without "
      "sacrificing your own safety -- when you punch or kick unarmed, "
      "your foe does not gain an attack of opportunity on you." },

  
  { FT_WEAPON_PROFICIENCY, FF_MULTIPLE|FF_WARRIOR, "Weapon Proficiency",
    "In addition to all the weapons you are proficient with "
      "naturally due to your class or classes, you can choose one "
      "additional non-Exotic weapon with which you are proficient." },
  { FT_WEAPON_FOCUS, FF_MULTIPLE|FF_WARRIOR, "Weapon Focus", 
    "You are particulairly skilled with a specific weapon, and receive "
      "a +1 skill bonus to hit when you use that weapon." },
  { FT_WEAPON_SPECIALIST, FF_MULTIPLE|FF_WARRIOR, "Weapon Specialist",
    "You have studied the use of a given weapon in great detail, "
      "and thus you receive a +1 skill bonus to hit, +2 skill bonus to "
      "damage and a +10% skill bonus to Speed when using it.",
    {{{FP_WEP_SKILL,WS_FOCUSED,0,
       FP_ABILITY,CA_WEAPON_SKILL, 1}}}
  },
  { FT_WEAPON_MASTERY, FF_MULTIPLE|FF_WARRIOR, "Weapon Mastery",
    "You have mastered every possible intricacy of a single weapon, "
      "and thus you receive a +2 skill bonus to hit, +2 skill bonus to "
      "damage and +20% skill bonus to Speed when using it. In addition, "
      "the weapon inflicts the next highest possible die type of damage "
      "-- for example, a long sword inflicts 1d10 base damage instead "
      "of 1d8. Finally, you gain a +2 skill bonus to your defence class "
      "if the weapon is your main- or off-hand melee weapon.",
    {{{FP_WEP_SKILL,WS_SPECIALIST,0,
       FP_ABILITY,CA_WEAPON_SKILL, 2}}}},
  { FT_WEAPON_HIGH_MASTERY, FF_MULTIPLE|FF_WARRIOR, "Weapon High Mastery",
    "Your skill with your chosen weapon is the stuff of legends. You receive a "
    "+2 skill bonus to hit, a +3 skill bonus to damage, and a +30~ skill bonus to "
    "speed when using it. You gain a +3 skill bonus to melee defense. Your critical "
    "hit damage multiplier for the weapon is increased by one. Your range increment "
    "with the weapon is doubled. Finally, the weapon inflicts the next highest "
    "possible die type of damage -- for example, a long sword inflicts 1d10 base "
    "damage instead of 1d8.",
    {{{FP_WEP_SKILL,WS_MASTERY,0,
       FP_ABILITY,CA_WEAPON_SKILL, 3}}}},
  { FT_WEAPON_GRAND_MASTERY, FF_MULTIPLE|FF_WARRIOR, "Weapon Grand Mastery",
    "Your skill with your chosen weapon is rumoured to make the gods envious. You "
    "receive a +3 skill bonus to hit, a +4 skill bonus to damage, and a +50~ skill "
    "bonus to speed when using it. You gain a +4 skill bonus to melee defense. Your "
    "critical hit damage multiplier for the weapon is increased by one and your "
    "critical threat range is doubled. Your range increment is tripled. Finally, the "
    "weapon inflicts the next highest possible die type of damage -- for example, a "
    "long sword inflicts 1d10 base damage instead of 1d8.",
    {{{FP_WEP_SKILL,WS_HIGH_MASTERY,0,
       FP_ABILITY,CA_WEAPON_SKILL, 4}}}},

  { FT_WEAPON_FINESSE, FF_WARRIOR|FF_AGILE, "Weapon Finesse",
    "When using a qualifying melee weapon, you modify your attack rolls in combat "
      "with your Dexterity instead of your Strength, if that gives a more "
      "favourable result. You also never suffer a penalty to damage due to "
      "Strength with a qualifying weapon. Suitable weapons must belong to "
      "one of the following groups: Daggers, Staves, Short Blades, Flexible "
      "Weapons, Martial Arts Weapons or Light Weapons.\n"
      "__Additionally, you can use your Dexterity modifier instead of Strength "
      "on unarmed attacks and grapple checks.",
    {{{FP_ATTR,A_DEX,13,  FP_BAB,S_MELEE,1},
      {FP_ABILITY,CA_UNARMED_STRIKE,1}}}},

/******************************************************************************\
 *                               METAMAGIC FEATS                              *
\******************************************************************************/

  { FT_AMPLIFY_SPELL, FF_META|FF_WIZARD, "Amplify Spell",
    "An amplified spell reduces a target creature's magic resistance by 25%." },
  { FT_AUGMENT_SUMMONING, FF_META|FF_WIZARD, "Augment Summoning",
    "Your summoned creatures are more powerful than most, gaining a +4 magic bonus "
    "to Strength and Constitution, and [10 + (Spell Level x 5)] additional hit points." }, 
  { FT_BIND_SPELL, FF_META|FF_WIZARD, "Bind Spell",
    "A bound spell has no effect on the caster, even if he would normally "
                  "be caught within the area of effect." },
  { FT_CONTROL_SPELL, FF_META|FF_WIZARD, "Control Spell",
    "A controlled spell that has a random effect (<9>polymorph self<7>, <9>monster "
    "summoning<7>, etc.) offers the caster a limited array of choices rather than " 
    "being completely random." },
  { FT_CONSECRATE_SPELL, FF_META|FF_WIZARD, "Consecrate Spell",
    "A consecrated spell inflicts half holy damage and half damage "
      "of whatever type it normally does, making it useful against evil "
      "creatures who resist elemental damage types." ,
    {{{FP_ABILITY,CA_TURNING}}}},
  { FT_DEFILING_SPELL, FF_UNIMP|FF_META|FF_WIZARD, "Defiling Spell",
    "A defiling spell powers itself from the life energy of those nearby "
      "when it is cast. All living creatures within a 5-square radius globe "
      "of the caster take [spell level] points of damage when it is cast "
      "(Fortitude save against the spell's DC for half)." },
  { FT_DEFENSIVE_SPELL, FF_META|FF_WIZARD, "Defensive Spell",
    "A defensive spell does not provoke an attack of opportunity when "
      "it is cast, even if there are hostile creatures threatening the "
      "caster." },
  { FT_JUDICIOUS_SPELL, FF_META|FF_WIZARD, "Judicious Spell",
    "A judicious spell only affects creatures who are hostile to the "
    "caster, leaving neutrals unharmed.",
    {{{FP_FEAT,FT_BIND_SPELL}}}},
  { FT_EMPOWER_SPELL, FF_META|FF_WIZARD, "Empower Spell",
    "An empowered spell inflicts 50% extra damage, or otherwise has its "
      "magnitude increased by 50%." },
  { FT_ENLARGE_SPELL, FF_META|FF_WIZARD, "Enlarge Spell",
    "An enlarged spell has its area of effect significantly increased." },
  { FT_EXTEND_SPELL, FF_META|FF_WIZARD, "Extend Spell",
    "An extended spell lasts twice as long as it normally would (to a "
    "maximum of one day), and has double the normal range." },
  
  { FT_FOCUS_SPELL, FF_META|FF_WIZARD, "Focus Spell",
    "A focused spell has an area of effect 1/3rd of the normal size." },
  { FT_FORTIFY_SPELL, FF_META|FF_WIZARD, "Fortify Spell",
    "A fortified spell is much more difficult to dispel -- your effective "
      "caster level receives a +5 bonus for purposes of the dispel check a "
      "hostile caster makes against it." },
  { FT_HEIGHTEN_SPELL, FF_META|FF_WIZARD, "Heighten Spell",
    "A heightened spell has the DC of its saving throw increased by "
      "+4, and the maximum Hit Dice it can affect increased by 50%." },
  { FT_INHERANT_SPELL, FF_META|FF_WIZARD, "Inherent Spell",
    "An inherent spell does not require a spellbook or holy symbol "
      "to cast, and thus has no 'material component'." },
  { FT_MAXIMIZE_SPELL, FF_META|FF_WIZARD, "Maximize Spell",
    "A maximized spell inflicts the maximum damage it possibly could, "
      "or otherwise benifits from a maximized dice roll." },
  { FT_PROJECT_SPELL, FF_META|FF_WIZARD, "Project Spell",
    "A projected touch spell becomes a ray, thus allowing the caster "
      "to strike foes at a range." },
  { FT_QUICKEN_SPELL, FF_META|FF_WIZARD, "Quicken Spell",
    "A quickened spell can be cast in half the time it takes to cast "
      "any other spell." },
  { FT_REPEAT_SPELL, FF_UNIMP|FF_META|FF_WIZARD, "Repeat Spell",
    "A repeated spell can be cast again immediately after the first casting, "
      "for as long as the mage desires, with no chance of failing and a reduced "
      "mana cost." },
  { FT_STILL_SPELL, FF_META|FF_WIZARD, "Still Spell",
    "A stilled spell requires no gestures, and thus can be cast without "
      "the -60% penalty that usually results from casting while grappled or "
      "ensnared, or the -20% penalty for casting while prone or underwater. "
      "Also, a stilled spell can be cast without one hand free. Finally, when "
      "casting a still spell, since gestures are not required a caster "
      "may retain her weapon parry bonus during the casting timeout." },
  { FT_SURE_SPELL, FF_META|FF_WIZARD, "Sure Spell",
    "A sure spell has a 100% success chance when cast, provided that "
      "it had at least a 70% success chance before the Sure Spell feat was added." },
  { FT_TRANSMUTE_SPELL, FF_META|FF_WIZARD, "Transmute Spell",
    "A transmuted spell is altered to inflict a different type of "
      "elemental damage (fire, cold, acid, lightning or poison) than it "
      "normally would." },  
  { FT_VOCALIZE_SPELL, FF_META|FF_WIZARD, "Vocalize Spell",
    "A vocalized spell has no incantation or other verbal component, "
      "and can thus be cast normally in areas of magical silence." },
  { FT_WARP_SPELL, FF_META|FF_WIZARD, "Warp Spell",
    "A warped spell produces a ray or blast that can pass through solid "
      "matter such as doors or walls, but affects corporeal targets normally." },

/******************************************************************************\
 *                            OTHER CASTING FEATS                             *
\******************************************************************************/

  { FT_SCHOOL_FOCUS, FF_MULTIPLE|FF_WIZARD, "School Focus",
    "You have augmented the power of your spells from one school "
      "of magic (Abjuration, Necromancy, etc). Spells of that school "
      "have the DC of their saving throws adjusted by +2.",
     {{{FP_CASTER_LEVEL,1,0}}} }, 
  { FT_SPELL_PENETRATION, FF_WIZARD, "Spell Penetration",
    "You add 4 to you caster level (or 20%) when trying to overcome magic resistance.",
     {{{FP_CASTER_LEVEL,1,0}}} }, 
  { FT_GUARDED_INVOCATION, 0, "Guarded Invocation",
      "You know how to use your innate spell-like abilities without "
      "provoking an attack of opportunity, even "
      "if there are hostile creatures in the squares surronding you." ,
      {{{FP_ABILITY,CA_INNATE_SPELL}}}},
  { FT_MYSTIC_PREPARATION, FF_WIZARD, "Mystic Preparation", "Your strategic use of "
    "preparatory magic is without peer. When you cast a persistant enhancement "
    "spell it costs two mana less than it normally would (minimum 1).",
    {{{FP_FEAT,FT_EXTEND_SPELL,0},
      {FP_FEAT,FT_DEFENSIVE_SPELL,0},
      {FP_FEAT,FT_GUILDMAGE,0}} 
    }  },

  { FT_BROAD_COUNTERSPELL, FF_WIZARD, "Broad Counterspell", 
     "__You are able to use your skill with counterspells more generally than most "
     "spellcasters, and can counter the invocation of innate spell-like abilities, "
     "the reading of scrolls and the blasting of wands with a counterspell, "
     "provided you meet all the other normal requirements.",
     {{{FP_SKILL,SK_SPELLCRAFT,8,
        FP_CASTER_LEVEL,1,0}}} }, 
  { FT_REFLECTIVE_COUNTERSPELL, FF_WIZARD, "Reflective Counterspell", 
     "__You are able to use a counterspell to turn magic back on its point of origin, "
     "rather than merely snuffing it out If the spell in question already targets its "
     "caster, it is counterspelled normally.",
     {{{FP_SKILL,SK_SPELLCRAFT,13,
        FP_CASTER_LEVEL,3,0}}} },   

  { FT_LORE_OF_ACID, FF_WIZARD, "Lore of Acid", "Your mastery of corrosive magics has yielded "
    "impressive results. All direct Acid damage done by your spell effects is increased "
    "by 20%.", 
    {{{FP_CASTER_LEVEL, 5}}}},
  { FT_LORE_OF_DEATH, FF_WIZARD, "Lore of Death", "Your mastery of necromancy has yielded "
    "impressive results. All direct Necromatic damage done by your spell effects is "
    "increased by 20%.", 
    {{{FP_CASTER_LEVEL, 5}}}},
  { FT_LORE_OF_FLAMES, FF_WIZARD, "Lore of Flames", "Your mastery of incendiary magics has "
    "yielded impressive results. All direct Fire damage done by your spell effects is "
    "increased by 20%.", {{{FP_CASTER_LEVEL, 5}}}},
  { FT_LORE_OF_RIME, FF_WIZARD, "Lore of Rime", "Your mastery of chilling magics has yielded "
    "impressive results. All direct Cold damage done by your spell effects is increased "
    "by 20%.", 
    {{{FP_CASTER_LEVEL, 5}}}},
  { FT_LORE_OF_STORMS, FF_WIZARD, "Lore of Storms", "Your mastery of shocking magics has yielded "
    "impressive results. All direct Lightning damage done by your spell effects is increased "
    "by 20%.", 
    {{{FP_CASTER_LEVEL, 5}}}},

  { FT_COMBAT_CASTING, FF_WIZARD, "Combat Casting", 
    "When you fail the percentile chance to cast a spell, or your "
      "spell is disrupted due to damage in combat, the spellcasting attempt "
      "costs you only 15 phases, as opposed to the 30 phases it would cost "
      "any other character. Additionally, for you, the penalty to Defense "
      "during a casting timeout is -2 rather than -4. You still lose your "
      "parry, dodge and insight bonuses to Defense during a casting timeout, "
      "however.",
      {{{FP_CASTER_LEVEL,1}}} },

/******************************************************************************\
 *                           THE TWO-WEAPON TREE                              *
\******************************************************************************/

  { FT_TWO_WEAPON_STYLE, FF_WARRIOR, "Two-Weapon Style",
    "Whatever penalties you receive for fighting with two weapons "
      "are reduced by two for each hand." },
  { FT_DOUBLE_WEAPON_FIGHTING, FF_WARRIOR, "Double Weapon Fighting",
    "You've learned to fight effectively with both ends of a double weapon. "
    "You fight with double weapons as though you had the Two-Weapon Style and "
    "Ambidexterity. Thus your total attack penalty is -2/-2."  ,
    {{{FP_ATTR,A_DEX,15,
      FP_BAB,S_MELEE,1}}}},
  { FT_POWER_DOUBLE_WEAPON, FF_WARRIOR, "Power Double Weapon",
    "You have learned to maximize the strength of your blows when wielding a double "
    "weapon. You add your full Strength bonus to damage rolls with your on-hand and "
    "your off-hand (rather than half of your Strength bonus for your off-hand).",
    {{{FP_FEAT,FT_DOUBLE_WEAPON_FIGHTING,0,
       FP_FEAT,FT_POWER_ATTACK,0,
         FP_BAB,S_MELEE,3}}}
  },
  { FT_AMBIDEXTERITY, FF_WARRIOR, "Ambidexterity",
    "You are equally proficient with both of your hands; you effectively "
      "do not have an \"off hand\", and thus you do not suffer a greater "
      "penalty to attack rolls made with the weapon in your Ready Hand when "
      "fighting with two weapons." ,
    {{{FP_ATTR,A_DEX,15}}}},
  { FT_TWIN_WEAPON_STYLE, FF_WARRIOR, "Florentine Style",
    "When fighting with two weapons, you receive no additional penalty "
      "if your offhand weapon is the same size as you are -- that is, a human "
      "could wield two long swords rather than a long sword and a dagger." ,
    {{{FP_FEAT,FT_TWO_WEAPON_STYLE}}}},
  { FT_DEFENSIVE_SYNERGY, FF_WARRIOR, "Defensive Synergy",
    "When you wield two weapons, you add half the parry modifier of the weapon "
      "with the lower parry modifier to your Defense score along with the "
      "higher of the two bonuses.",
    {{{FP_FEAT,FT_TWO_WEAPON_STYLE,0,
       FP_FEAT,FT_EXPERTISE,0,
       FP_BAB,S_MELEE,3},
      {FP_FEAT,FT_DOUBLE_WEAPON_FIGHTING,0,
       FP_FEAT,FT_EXPERTISE,0,
       FP_BAB,S_MELEE,3} }}},

  { FT_TWO_WEAPON_TEMPEST, FF_WARRIOR, "Two-Weapon Tempest",
    "When you are wielding two weapons, you gain a 50% skill bonus "
      "to your Speed.",
    {{{FP_FEAT,FT_TWO_WEAPON_STYLE,0,
       FP_BAB,S_MELEE,9}}}},

/******************************************************************************\
 *                             THE STRONG FIGHTER TREE                        *
\******************************************************************************/

  { FT_POWER_ATTACK, FF_MIGHTY|FF_WARRIOR, "Power Attack",
    "Your blows have the full force of your strength behind them. For "
      "every three points above your foe's Defense that your attack roll "
      "scores, you add a +1 bonus to that attack's damage roll, to a maximum "
      "of your secondary Strength modifier. If you wield a weapon two-handed, "
      "the damage bonus increases to +1 per 2 points above the enemy's Defense "
      "score.",
    {{{FP_ATTR,A_STR,13}}}},
  { FT_MASTER_GREAT_BLOW, FF_MIGHTY|FF_WARRIOR, "Master Great Blow",
    "You are skilled in delivering Great Blows, and doing so only takes "
      "the same amount of time as a normal attack (as opposed to 150%) and only "
      "leaves you exposed for 10 phases (rather than 20)." ,
    {{{FP_FEAT,FT_POWER_ATTACK}}}},
  { FT_CLEAVE,     FF_MIGHTY|FF_WARRIOR, "Cleave",
    "When your melee attack drops (kills or knocks down) an enemy, you gain an "
      "immediate second attack against a different randomly chosen foe within "
      "your threatened area, if such a foe exists. This feat (and similar feats) "
      "have no effect when fighting unarmed or using ranged attacks.",
    {{{FP_FEAT,FT_POWER_ATTACK}}}},
  { FT_CLEAVE_ASUNDER,     FF_MIGHTY|FF_WARRIOR, "Cleave Asunder",
    "Your Sunder attacks are unusually effective. You are trained in following "
    "through when you break an opponent's weapon or shield. If you make a Sunder "
    "attack and the target object is broken, you immediate cleave through and make "
    "another attack against the same opponent.", 
    {{{FP_FEAT,FT_CLEAVE,0 ,
       FP_FEAT,FT_SUNDER,0}}}},
  { FT_GREAT_CLEAVE, FF_MIGHTY|FF_WARRIOR, "Great Cleave",
    "If the bonus attack gained from Cleave also drops a foe, you may "
      "make the third attack, and so on, until either you fail to kill an "
      "enemy, or there are no more enemies in your threatened area." ,
    {{{FP_FEAT,FT_CLEAVE,0,
       FP_BAB,S_MELEE,3}}}},
  { FT_SUPREME_CLEAVE, FF_MIGHTY|FF_WARRIOR, "Supreme Cleave",
    "Whenever you hit with a bonus attack gained from Cleave or Great "
      "Cleave, you inflict an extra 1d8 points of damage on that attack.",
    {{{FP_FEAT,FT_GREAT_CLEAVE,0,
       FP_BAB,S_MELEE,7}}}},
  { FT_MASTER_BULL_RUSH, FF_MIGHTY|FF_WARRIOR, "Master Bull Rush",
    "You are skilled in using the bull rush maneuver, and no longer "
      "provoke an attack of opportunity in so doing.",
    {{{FP_FEAT,FT_POWER_ATTACK}}}},
  { FT_SUNDER,     FF_MIGHTY|FF_WARRIOR, "Master Sunder",
    "You do not provoke an attack of opportunity when you try to damage a foe's "
    "equipment in combat. In addition, you deal double damage when sundering "
    "equipment. Finally, you deal double damage when attacking inanimate objects "
    "like chests and doors (but not golems or magical constructs)." ,
    {{{FP_FEAT,FT_POWER_ATTACK}}}},
  { FT_KNOCKBACK,  FF_MIGHTY|FF_WARRIOR|FF_UNIMP, "Knock Back",
    "Whenever you inflict 20% or more of a foe's total hit points of damage "
      "in a single attack, and the foe is your size "
      "or smaller, that foe must make an opposed Strength check with you to "
      "avoid being knocked back one or more squares, provided those squares "
      "are open." ,
    {{{FP_FEAT,FT_POWER_ATTACK}}}},
  { FT_POWER_CHARGE, FF_MIGHTY|FF_WARRIOR, "Power Charge",
    "In addition to adding your charge bonus to your attack and damage "
      "rolls when you charge, one third of it is subtracted from the threat range of your "
      "attack, thus making it more likely you will score a critical hit.",
    {{{FP_FEAT,FT_POWER_ATTACK}}}},
  { FT_KNOCK_PRONE,  FF_MIGHTY|FF_WARRIOR, "Knock Prone", 
    "When you strike a great blow or a critical hit against an enemy, "
      "or on 1 in 3 randomly chosen normal attacks, they must succeed in a "
      "Fortitude saving throw or be knocked prone.",
    {{{FP_FEAT,FT_POWER_ATTACK}}}},
  { FT_MONKEY_GRIP,  FF_MIGHTY|FF_WARRIOR, "Monkey Grip",
    "You can wield weapons and bows as if you were one size category larger than "
    "you actually are. For example, a medium-sized character with this feat could "
    "hold a greatsword in one hand. Wielding a weapon that is larger than you are "
    "inflicts a -2 circumstance penalty to your attack roll." ,
    {{{FP_FEAT,FT_POWER_ATTACK,
       FP_ATTR,A_STR,13,
       FP_BAB,S_MELEE,3}}}},

/******************************************************************************\
 *                             THE AGILE FIGHTER TREE                         *
\******************************************************************************/

  { FT_COMBAT_REFLEXES, FF_WARRIOR|FF_AGILE, "Combat Reflexes", 
    "Most characters can make only one attack of opportunity every "
      "10 phases, but you are able to make up to (1 + Dexterity modifier) "
      "total such attacks in this span.",
    {{{FP_ATTR, A_DEX, 13}}} },
  { FT_CLOSE_QUARTERS_FIGHTER, FF_WARRIOR|FF_AGILE, "Close Quarters Fighter",
      "You gain a +1 circumstance bonus to hit, damage and defense when there "
      "are at least four solid squares (e.g., walls, pillars) within one square "
      "of you and you are adjacent to your opponent. For example, standing against "
      "the wall of a large room would not trigger this feat, but standing in a corner "
      "or a narrow hallway would." ,
    {{{FP_ATTR, A_DEX, 13,
       FP_FEAT, FT_COMBAT_REFLEXES, 0}}} },
  { FT_MISSILE_PROOF, 0, "Missile Proof",
    "You may block projectiles easily with your shield. If you are attacked by a "
    "ranged weapon and you are not surprised or flat-footed, you may make a Reflex "
    "save against DC 20. The DC is lowered by the armour bonus of your shield and "
    "raised by the magical plus, if any, of the missile weapon. If you succeed, you "
    "deflect the attack. Deflecting a missile attack in this manner uses one of your "
    "attacks of opportunity. This feat will not allow you to block magical attacks "
    "like <9>acid arrow<7>.",
    {{{FP_FEAT,FT_COMBAT_REFLEXES,0,
       FP_PROF,WG_SHIELDS,0}}}},

  /*
  { FT_COMBAT_OPPORTUNIST, 0, "Combat Opportunist",
    "You have trained extensively in attacking openings in the defense of your opponents. "
    "When you make a successful attack of opportunity you may add your Sneak Attack damage.",
    {{{FP_ABILITY,CA_SNEAK_ATTACK,0,
       FP_FEAT,FT_COMBAT_REFLEXES,0}}}},
  */

  { FT_RAPID_RELOAD, 0, "Rapid Reload",
    "It takes you only 10 phases to reload a crossbow, not 30. Reloading a crossbow "
    "does not provoke an attack of opporunity for you. In addition, you are able to care "
    "for and string a crossbow so that it can remain cocked for twice as long without "
    "suffering damage." ,
    {{{FP_ATTR,A_DEX,15,  
       FP_BAB,S_ARCHERY,3,
       FP_FEAT, FT_COMBAT_REFLEXES, 0}}}},


  { FT_DODGE,      FF_AGILE|FF_WARRIOR, "Dodge",
    "You receive a +1 feat bonus to your Defense, or a +3 bonus if you "
      "wear Light or lighter armour and have Light or lighter encumbrance.",
    {{{FP_ATTR,A_DEX,13}}}},
    
  { FT_SLIPAWAY,   FF_AGILE|FF_WARRIOR, "Slipaway",
    "You are easily able to slip away from others' grabs and holds, and "
      "gain a +4 bonus to the maneuver check to break grapples.",
    {{{FP_FEAT,FT_DODGE,0,
       FP_ATTR,A_DEX,13}}}},
    
  { FT_PARTIAL_EVASION, FF_AGILE|FF_WARRIOR, "Partial Evasion",
    "When you make a successful Reflex saving throw against a "
      "damaging area effect, you take 1/3 (instead of 1/2) of the normal "
      "damage.",
    {{{FP_FEAT,FT_DODGE,0,
       FP_ATTR,A_DEX,13}}}},

    
  { FT_FEATHERFOOT, FF_AGILE|FF_WARRIOR, "Featherfoot", 
    "As long as you have found a trap, you can walk over the square "
      "that contains it without any chance of triggering it. Additionally, "
      "you can continue Hiding in Shadows while walking over water or mud.",
    {{{FP_FEAT,FT_DODGE,0,
       FP_ATTR,A_DEX,13}}}},

  { FT_INSTANT_STAND, FF_AGILE|FF_WARRIOR, "Instant Stand",
    "With a Reflex save or Balance check -- whichever favours you more -- "
    "against DC 15, you can regain your feel nearby instantly after being "
    "knocked prone. Failing the check means that you take the normal time "
    "to stand, and provoke an attack of opportunity.", 
    {{{FP_FEAT,FT_DODGE,0,
       FP_ATTR,A_DEX,13}}}},
       
  { FT_MOBILITY,   FF_AGILE|FF_WARRIOR, "Mobility",
    "You receive a +4 bonus to Defense versus attacks of opportunity caused by "
    "moving out of an enemy's threatened area.",
    {{{FP_FEAT,FT_DODGE}}}},
    
  { FT_SPRING_ATTACK, FF_AGILE|FF_WARRIOR, "Spring Attack",
    "You can make a melee attack against a creature two squares distant "
      "from you, as long as there is not a creature larger than your target "
      "directly between you and it." ,
    {{{FP_FEAT,FT_MOBILITY,0,   
       FP_BAB,S_MELEE,4}}}},
  
  { FT_MANTIS_LEAP, FF_AGILE|FF_WARRIOR, "Mantis Leap",
    "When you jump, you gain an attack bonus as if you charged for the "
      "number of squares covered to your next attack. You cannot charge "
      "further before you attack, however.",
    {{{FP_FEAT,FT_MOBILITY,0,   FP_SKILL,SK_JUMP,5}}}},

/******************************************************************************\
 *                             THE TOUGH FIGHTER TREE                         *
\******************************************************************************/
  { FT_ROLL_WITH_IT, FF_TOUGH|FF_WARRIOR, "Roll With It",
    "You are able to move with the impact and lessen the most severe "
    "effects of critical hits. You suffer only half the normal damage "
    "increase from a crit, so a weapon normally inflicting x2 damage "
    "on a crit does x1.5, a weapon normally doing x3 does x2, a weapon "
    "normally doing x4 does x2.5 and a weapon normally doing x5 does x3 "
    "instead.",
    {{{FP_ATTR,A_CON,13}}}},
  
  { FT_ENDURANCE, FF_TOUGH|FF_WARRIOR, "Endurance",
    "You are naturally hardy and inured to physical exertion. Your "
    "harsh lifestyle makes you well suited to the rigors of adventuring, "
    "giving you a +5 feat bonus to your total fatigue points.",
    {{{FP_ATTR,A_CON,13}}}},

  { FT_DIEHARD, FF_TOUGH|FF_WARRIOR|FF_UNIMP, "Diehard",
    "You are extraordinarily difficult to kill. You can survive at twice "
    "the normal number of negative hit points allowed to you based on your "
    "Constitution and level, and when you are within the normal threshold "
    "of negative hit points, you never fall unconscious and are not considered "
    "helpless.",
    {{{FP_ATTR,A_CON,13,
       FP_FEAT,FT_ENDURANCE, 0}}}},

  { FT_WEATHER_BLAST, FF_TOUGH|FF_WARRIOR, "Weather Blast", 
    "You can resist area effects using a Fortitude save rather than "
      "Reflex, assuming your Fortitude is better. However, you cannot use "
      "the Evasion ability with this Feat.", 
    {{{FP_ATTR,A_CON,13,
       FP_FEAT,FT_ENDURANCE, 0}}}},

  { FT_TOUGHNESS,  FF_TOUGH|FF_WARRIOR, "Toughness",
    "You are extraordinarily resilient, and receive a 25% bonus to your "
      "total hit points.",
    {{{FP_ATTR,A_CON,13,
       FP_FEAT,FT_ENDURANCE, 0,
       FP_FEAT,FT_ROLL_WITH_IT, 0}}}},
       
  { FT_IGNORE_WOUND, FF_TOUGH|FF_WARRIOR, "Ignore Wound",
    "Once per day, you can ignore the damage from any one attack that would "
    "bring you to or below 0 hit points. This happens automatically when it "
    "would be needed, but does not protect you from effects that cause instant "
    "death by means other than hit point loss, such as a <9>slay living<7> "
    "spell.",
    {{{FP_ATTR,A_CON,13,
       FP_FEAT,FT_ENDURANCE, 0,
       FP_FEAT,FT_ROLL_WITH_IT, 0,
       FP_FEAT,FT_TOUGHNESS, 0},
      {FP_ABILITY,CA_SIZE_ADVANTAGE,1}}}},
       
  /* Iron Skin */
  
  { FT_LIVING_WALL, FF_TOUGH|FF_WARRIOR, "Living Wall",
    "If an enemy charges you and hits, and you take less than 25~ of "
    "your total hit points in damage, and are not knocked down, knocked back "
    "or stunned, that foe is automatically knocked down, and must save "
    "or be stunned for a number of turns equal to your Strength bonus.",
    {{{FP_ATTR,A_CON,13,
       FP_ATTR,A_STR,13,
       FP_FEAT,FT_ENDURANCE, 0,
       FP_FEAT,FT_ROLL_WITH_IT, 0,
       FP_FEAT,FT_TOUGHNESS, 0}}}},
  
  { FT_RESILIENT,  FF_TOUGH|FF_WARRIOR, "Resilient",
    "As long as you are not more severely damaged than [(Con modifier + 3) x 5%] of "
    "your HP, you recover damage at a rate of 1 point per 3 turns -- the "
    "injuries you received are easily recovered from, since they are assumed "
    "to just be scrapes and bruises once you get your breath back.",
    {{{FP_ATTR,A_CON,13,
       FP_FEAT,FT_ENDURANCE, 0,
       FP_FEAT,FT_ROLL_WITH_IT, 0,
       FP_FEAT,FT_TOUGHNESS, 0}}}},  
       
  { FT_HARDINESS,  FF_TOUGH|FF_WARRIOR, "Hardiness",
    "You have a hardy constitution and a developed resistance to toxins and diseases. "
    "You gain a +2 bonus to Fortitude saves versus poison and disease, and you require "
    "two fewer successful saving throws to fully overcome any given poison and three "
    "fewer to overcome any disease, to a minimum of one. You also gain an inherent toxin "
    "resistance (against direct toxic damage like a green dragon's breath weapon or a "
    "mage's <9>cloudkill<7> spell) of +5.",
    {{{FP_ATTR, A_CON, 13,
       FP_FEAT,FT_ENDURANCE, 0}}}},

  { FT_RESIST_DEATH, FF_TOUGH|FF_WARRIOR, "Resist Death",
    "You gain a limited resistance to instant necromatic death attacks (such as a "
    "banshee's wail, a nymph's beauty, a <9>slay living<7> or <9>finger of death<7> "
    "spell, etc.), allowing you to automatically ignore a number of such attacks "
    "per day equal to twice your Constitution modifier. This ability takes effect "
    "before the saving throw.",
    {{{FP_ATTR, A_CON, 13,
       FP_FEAT,FT_HARDINESS}}}},    
  
  { FT_RESIST_PARALYSIS, FF_TOUGH|FF_WARRIOR, "Resist Paralysis",
    "You gain the ability to shrug off a number of paralyzing attacks per day equal "
    "to twice your Constitution modifier. This effect stacks with that gained from a "
    "cleric's Travel domain.",
    {{{FP_ATTR, A_CON, 13,
       FP_FEAT,FT_HARDINESS}}}},    

/******************************************************************************\
 *                             THE SMART FIGHTER TREE                         *
\******************************************************************************/

  { FT_EXPERTISE, FF_SMART|FF_WARRIOR, "Expertise", 
    "You are skilled at using your weapon for defense as well as offence. "
      "You gain your weapon's parry modifier as a Defense bonus against all attacks, "
      "instead of simply against the attacks of foes with slashing weapons.",
    {{{FP_ATTR,A_INT,13}}}},
    
  { FT_DEFT_OPPORTUNIST, FF_SMART|FF_AGILE, "Deft Opportunist",
    "You are especially adept at making attacks of opportunity, and "
    "receive a +4 bonus to the attack roll on all such attacks.",
    {{{FP_FEAT, FT_EXPERTISE, 0},
      {FP_FEAT, FT_COMBAT_REFLEXES, 0}}}},

  { FT_EXPERT_TACTICIAN, FF_SMART|FF_WARRIOR, "Expert Tactician", 
    "When you attack an opponent who is surprised or denied her dexterity bonus to "
    "defense (for any reason except being flat-footed or not being able to see you), "
    "the attack takes no time. This only applies to normal melee, ranged and brawl "
    "attacks, as well as magical touch attacks. It does not apply to great blows or "
    "trip attempts (for example). You may only make one \"free\" Expert Tactician "
    "attack on a given adversary per turn.", 
    {{{FP_ATTR,A_INT,13,
       FP_FEAT,FT_COMBAT_REFLEXES,0,
       FP_BAB,S_MELEE,2}}}},

  { FT_MASTER_TRIP, FF_SMART|FF_WARRIOR, "Master Trip",
    "You are particularly skilled at using the vulnerability tripping "
      "an opponent causes to your advantage. You do not suffer an attack "
      "of opportunity when you attempt to trip a foe with the Trip "
      "maneuver. In addition, you gain a +4 bonus to "
      "all trip checks.",
    {{{FP_FEAT,FT_EXPERTISE}}}},
    
  { FT_MASTER_GRAPPLE, FF_SMART|FF_WARRIOR, "Master Grapple",
    "You do not provoke an attack of opportunity when yo umake a touch attack to start "
    "a grapple. In addition, you gain a +4 bonus on all grapple checks, regardless of "
    "whether you started the grapple.", 
    {{{FP_ATTR,A_DEX,13}}}},
    
  { FT_GUARDED_STANCE, FF_SMART|FF_WARRIOR, "Guarded Stance",
    "You are able to position yourself and your polearm intelligently in combat "
    "situations. If you are wielding a reach weapon, you are not flat-footed, and an "
    "opponent fails to close with you, you may make an attack of opportunity on that "
    "opponent. In addition, if you are making an attack of opportunity because an "
    "opponent is charging you, you gain a +8 bonus to hit." ,
    {{{FP_ATTR,A_INT,13,  FP_BAB, S_MELEE, 1}}}},
    
  { FT_WHIRLWIND_ATTACK, FF_SMART|FF_WARRIOR, "Whirlwind Attack",
    "You may execute a devastating flurry of attacks, striking "
      "once at every foe that is immediately adjacent to you.",
    {{{FP_FEAT,FT_EXPERTISE,0,
       FP_FEAT,FT_MOBILITY,0,
       FP_BAB,S_MELEE,4}}}
  },

  { FT_MASTER_DISARM, FF_SMART|FF_WARRIOR, "Master Disarm", 
    "You are skilled in disarming enemies, and you do not provoke "
      "an attack of opportunity when you use the Disarm tactical option.",
    {{{FP_FEAT,FT_EXPERTISE}}}},
    
  { FT_TACTICAL_WITHDRAWL, FF_SMART|FF_WARRIOR, "Tactical Withdrawl",
    "You receive a +8 bonus to the disengagement contest needed to leave "
      "an enemy's threatened area safety.",
    {{{FP_FEAT,FT_EXPERTISE}}}},
    
  { FT_RIPOSTE,  FF_SMART|FF_WARRIOR, "Riposte", 
    "After deflecting an opponent's blow you can quickly attack while the "
    "enemy is over-extended. When you are not surprised, at most lightly "
    "encumbered, and wearing at most light armour, you may make an attack of "
    "opportunity against an opponent who is beside you and tries to attack you "
    "in melee and misses.",
    {{{FP_FEAT,FT_WEAPON_FINESSE,0,
       FP_FEAT,FT_EXPERTISE,0,
       FP_BAB,S_MELEE,6}}}},

/******************************************************************************\
 *                           THE INTUITIVE FIGHTER TREE                       *
\******************************************************************************/

  { FT_BLIND_FIGHT, FF_INTUITIVE|FF_WARRIOR, "Blind Fight",
    "You are skilled at fighting blind. You do suffer a 25% miss chance (as "
      "opposed to 50%) for attacking foes you can't see, and similairly you are "
      "not denied your Dexterity bonus to AC, nor is a +2 toHit bonus granted to "
      "them, when you are attacked by foes you cannot see. You movement rate is "
      "also not reduced when blind or in areas of magical darkness.",
    {{{FP_ATTR,A_WIS,13}}}},
  { FT_BLINDSIGHT, FF_INTUITIVE|FF_WARRIOR, "Blindsight",
    "Using senses such as acute hearing and sensitivity to vibrations, you detect "
    "the location of opponents who are within one square (ten feet) of you. "
    "Invisibility and darkness are irrelevant, though it does not allow you to "
    "discern incorporeal beings. This blindsight range stacks with blindsight you "
    "may have from any other sources.",
    {{{FP_ATTR,A_WIS,19,
       FP_FEAT,FT_BLIND_FIGHT,0}}}},
  { FT_DEFLECT_ARROWS, FF_INTUITIVE|FF_WARRIOR, "Deflect Arrows",
    "You can deflect a missile shot or thrown at you by making a Reflex "
      "save against a DC of 15. This attempt costs you one of your attack of "
      "opportunity slots for the current round, and is ineffective against "
      "a surprise attack.",
    {{{FP_ATTR,A_DEX,13,
       FP_ATTR, A_WIS, 13,
       FP_FEAT,FT_BLIND_FIGHT,0}}}},
  { FT_DEFLECT_MAGIC, FF_INTUITIVE|FF_WARRIOR, "Deflect Magic",
    "You extend your ability to deflect arrows into deflecting bolts of magical "
    "energy. With a successful Reflex save against DC 15, you completely negate "
    "any one magical bolt or ray directed against you, such as the effects of "
    "a <9>magic missile<7>, <9>searing light<7>, <9>ray of enfeeblement<7> or "
    "<9>disintegrate<7> spell. This costs you one of your attacks of opportunity "
    "for the current round, and you must be able to perceive the attacker and "
    "must not be flat-footed.\n",
    {{{FP_ATTR, A_DEX, 13,
       FP_ATTR, A_WIS, 13,
       FP_FEAT, FT_DEFLECT_ARROWS, 0 }}}},
       
  { FT_ZEN_ARCHERY, FF_INTUITIVE|FF_WARRIOR, "Zen Archery",
    "You gain an insight bonus to your archery +toHit equal in magnitude "
      "to your Wisdom modifier. This bonus stacks with the normal Dexterity "
      "bonus. If you have the Aimed Shot feat, the maximum damage bonus for an "
      "aimed shot is increased by your secondary Wisdom modifier.",
    {{{FP_FEAT,FT_FAR_SHOT,0,
       FP_FEAT,FT_BLIND_FIGHT,0}}}},
       
  { FT_ZEN_DEFENSE, FF_INTUITIVE|FF_WARRIOR, "Zen Defense",
    "When unarmoured, you gain an insight bonus to your Defense Class equal in magnitude "
      "to your Wisdom modifier, if it is positive. This bonus stacks with the "
      "normal Dexterity bonus. If you wear light armour, you get half the normal "
      "bonus, rounding up. Characters in medium or heavy armour gain no benefit from "
      "this feat. Spells that duplicate the effect of armour count as armour for the "
      "purpose of this feat." ,
      {{{FP_FEAT,FT_BLIND_FIGHT,0,
         FP_FEAT,FT_EXPERTISE,0}}}},

  { FT_IMPROVED_INITIATIVE, FF_WARRIOR|FF_INTUITIVE, "Improved Initiative",
    "Others are rarely able to act before you are ready "
      "to respond to their action. If you are attacked while flat-footed, "
      "you may make a Dexterity check (DC 15). If successful, you negate "
      "the condition of flat-footedness and ready your defenses before "
      "the attack lands.",
      {{{FP_ATTR, A_WIS, 13}}} },

  { FT_DIRTY_FIGHTING, FF_WARRIOR|FF_INTUITIVE, "Dirty Fighting",
    "You are skilled at the brutal and effective fighting tactics of "
      "the streets and back alleys. When you make a successful attack "
      "against an enemy with vital areas (i.e., vulnerable to critical "
      "hits), and the enemy's Wisdom is lower than your own, you are "
      "able to 'get the drop on' them once per fight, inflicting +1d8 "
      "untyped damage on the first qualifying hit, as well as blinding "
      "the foe for 1d4+1 rounds if he fails a Reflex save against DC (10 + "
      "1/2 your BAB + your Wisdom modifier).\n"
      "__The Noble Aegis ability protects against the effects of this feat.",
    {{{FP_BAB,S_MELEE,2,
       FP_FEAT,FT_IMPROVED_INITIATIVE, 0,
       FP_SKILL, SK_FIND_WEAKNESS, 10},
      {FP_BAB,S_BRAWL,2,
       FP_FEAT,FT_IMPROVED_INITIATIVE, 0,
       FP_SKILL, SK_FIND_WEAKNESS, 10}}} },

/******************************************************************************\
 *                           THE COMMANDING FIGHTER TREE                      *
\******************************************************************************/

  { FT_LEADERSHIP, FF_COMMANDING|FF_WARRIOR, "Leadership",
    "You are a born leader, and followers naturally flock to your cause. "
    "The maximum possible power of a group you can lead is tripled; effectively, "
    "you can have three times as many creatures or similar CR in your party, "
    "or a single creature of +3 greater CR. This is equivalent to a +10 bonus "
    "to Diplomacy, only for the purpose of calculating PCR, and it "
    "applies to allies recruited with Diplomacy, creatures summoned via "
    "magic spells or items and creatures that have been dominated. It "
    "does not apply to creatures that are neutral to you that you issue "
    "requests to with Bluff, Diplomacy or Intimidate.",
    {{{FP_ATTR, A_CHA, 13}}}},

  { FT_BATTLEFIELD_INSPIRATION, FF_COMMANDING|FF_WARRIOR, "Battlefield Inspiration",
    "You lead your allies by example. Whenever you slay a foe whose CR equals "
    "your own, all of your allies who witness this act gain a +1 morale bonus "
    "to hit and damage for the next [3d6 x 10] turns. If the foe's CR exceeds your own, "
    "this bonus rises by the difference between your CR's. Killing further foes "
    "does not increase the bonus (unless you kill one with a higher CR), but it "
    "does extend the duration of the inspiration effect.",
    {{{FP_ATTR, A_CHA, 13}}}},
    
  { FT_COORDINATED_TACTICS, FF_COMMANDING|FF_SMART|FF_WARRIOR, "Coordinated Tactics",
    "You guide your allies with a careful tactical foresight, placing them "
    "exactly where you need them to be. You compensate for any weakness they "
    "might have in terms of movement with your tactics and foresight. In game "
    "terms, this allows all the creatures in your party to use your movement "
    "rate as if it were their own. If some of your allies have a higher movement "
    "rate than you do, they can still use their own rate normally.",
    {{{FP_ATTR, A_CHA, 13,
       FP_FEAT, FT_BATTLEFIELD_INSPIRATION, 0},
      {FP_ATTR, A_INT, 13,
       FP_FEAT, FT_EXPERTISE, 0}}}},

  { FT_DANCE_OF_TUKIMA, FF_COMMANDING|FF_WARRIOR, "Dance of Tukima",
    "You have mastered the Dance of Tukima, an exotic style of fighting "
    "that blends swordplay with dancing to beguile and disorient foes. "
    "You must be in light armour and lightly encumbered to use the Dance "
    "of Tukima; doing so costs 1 fatigue and lasts a number of rounds "
    "equal to twice your Athletics rating. While dancing, you add your "
    "Charisma bonus to attack rolls, and half of your Perform skill "
    "rating to damage, provided that you are wielding a Light piercing "
    "or slashing weapon (or weapons).",
    {{{FP_ATTR, A_CHA, 13,
       FP_FEAT, FT_BATTLEFIELD_INSPIRATION, 0,
       FP_FEAT, FT_ALLURING, 0,
       FP_SKILL, SK_PERFORM, +10 }}}},
  
  { FT_SOULBLADE, FF_COMMANDING|FF_WARRIOR, "Soulblade",
    "When you wield a magical weapon, your force of personality "
    "is so awesome that it leaves a psychic imprint on the weapon, "
    "altering its mystical nature for so long as it is in your "
    "hands. The weapon's plus is increased by 1, to a maximum of "
    "your Charisma modifier.",
    {{{FP_ATTR, A_CHA, 15,
       FP_FEAT, FT_BATTLEFIELD_INSPIRATION, 0}}}},
  
  { FT_GREATER_SOULBLADE, FF_COMMANDING|FF_WARRIOR|FF_UNIMP, "Greater Soulblade",
    "You may select any one special weapon quality from a list "
    "determined by your personality archetype, with a maximum "
    "effective plus increase equal to or lower than your Charisma "
    "modifier, minus 1. Any weapon you wield gains that quality "
    "for as long as you wield it.",
    {{{FP_ATTR, A_CHA, 15,
       FP_FEAT, FT_SOULBLADE, 0 }}}},
  
  { FT_IMPROVED_DEMORALIZE, FF_COMMANDING|FF_WARRIOR, "Improved Demoralize",
    "Learning this feat augments the Demoralize function of the Intimidate "
    "skill. It now functions whenever the character kills a member of an "
    "opposing group, even if it wasn't done with a critical hit. Additionally, "
    "creatures of CR 5 (rather than CR 3) or lower become afraid rather than "
    "shaken.",
    {{{FP_ATTR, A_CHA, 13,
       FP_FEAT, FT_BATTLEFIELD_INSPIRATION, 0,
       FP_SKILL, SK_INTIMIDATE, +8}}}},
  
  { FT_WARCRY, FF_COMMANDING|FF_WARRIOR, "Warcry",
    "You can let out a great and terrifying shout, shocking your enemies "
    "and causing them to hesitate. Making a warcry costs you 15 segments; "
    "any enemy within 80 feet who fails a Will save (DC 10 + 1/2 your "
    "Intimidate skill) hesitates for 50 segments, and is thereafter shaken "
    "as per the Demoralize function of the Intimidate skill. Monks refer to "
    "this ability as a <3>kiai shout<7>. This feat only affects a given opponent "
    "once per day, regardless of success or failure.",
    {{{FP_ATTR, A_CHA, 13,
       FP_FEAT, FT_IMPROVED_DEMORALIZE, 0},
      {FP_ATTR, A_CHA, 13,
       FP_ABILITY, CA_KI_STRIKE, 1}}}},
  
  { FT_TERRIBLE_MEIN, FF_COMMANDING|FF_WARRIOR, "Terrible Mein",
    "Your presence is so very terrible that thinking foes instinctively "
    "flinch, pulling their blows. When you are struck by a sapient "
    "creature who is not immune to fear for damage in melee, you subtract "
    "your Charisma modifier from the total damage dealt before armour and "
    "resistances are taken into account. If the attacker has a saving "
    "throw bonus versus fear effects, your Charisma modifier is reduced "
    "by that amount for this purpose.",
    {{{FP_ATTR, A_CHA, 13,
       FP_FEAT, FT_BATTLEFIELD_INSPIRATION, 0,
       FP_SKILL, SK_INTIMIDATE, 12}}}},
       
/******************************************************************************\
 *                             THE MOUNTED FIGHTER TREE                       *
\******************************************************************************/

  { FT_MOUNTED_COMBAT, FF_WARRIOR, "Mounted Combat",
    "You are skilled at defending your mount in combat. When your mount "
      "is attacked, you automatically make a ride skill check. If your skill "
      "check result exceeds the attack roll's total, the attack is considered "
      "to be a miss." },
  { FT_MOUNTED_ARCHERY, FF_WARRIOR, "Mounted Archery",
    "You suffer only half the normal penalty for using ranged weapons "
      "while mounted -- -2 instead of -4 normally, or -4 instead of -8 when "
      "you are charging." ,
    {{{FP_FEAT,FT_MOUNTED_COMBAT}}}},
  { FT_OVERRUN,     FF_WARRIOR, "Overrun",
    "Any mount you ride gains a trip attack (similar to a wolf's) in "
      "addition to its normal attacks. Whenever your mount hits with one "
      "or more of the attacks in its natural attack sequence, the victim "
      "must make a Reflex save (DC 10 + 1/2 your Ride skill) or be "
      "rendered prone.",
    {{{FP_FEAT,FT_MOUNTED_COMBAT}}}},
  { FT_RIDE_BY_ATTACK, FF_WARRIOR, "Ride-By Attack",
    "You are able to attack while charging and mounted more proficiently than most. "
    "This conveys three benefits:\n__First, you keep "
    "charging if you strike and kill an opponent, potentially allowing you to continue "
    "in a straight line and attack more enemies behind the first one with the full "
    "momentum of your charge.\n__Second, you gain a +4 circumstance bonus to your "
    "Defense versus attacks of opportunity caused by leaving an enemy's theat range "
    "while charging.\n__Finally, you are not denied your Dexterity bonus to Defense when "
    "charging while mounted.",
    {{{FP_FEAT,FT_MOUNTED_COMBAT}}}},

  { FT_SPIRITED_CHARGE, FF_WARRIOR, "Spirited Charge",
    "When charging while mounted, you do triple damage with weapons that say they "
    "would normally do double damage (e.g., lances, longspears). You do double damage "
    "with all other weapons.", 
    {{{FP_FEAT,FT_RIDE_BY_ATTACK}}}},

/******************************************************************************\
 *                              THE RANGED COMBAT TREE                        *
\******************************************************************************/

  { FT_FAR_SHOT,    FF_WARRIOR, "Far Shot",
    "The range increment of your ranged weapons is doubled." },
#if 0
  { FT_SHOOT_FROM_COVER,    FF_WARRIOR, "Shoot From Cover",
    "You are adept at making ranged attacks from concealment and then ducking back "
    "behind cover. Making a ranged attack does not cause you to stop hiding (although "
    "it will dispel <9>invisibility<7>, <9>sanctuary<7> and similar effects) provided "
    "that you are not beside your target. However, the victim will still have a "
    "general idea of the source of the attack." ,
    {{{FP_BAB,S_ARCHERY,5,
       FP_SKILL,SK_HIDE,5,
       FP_ATTR,A_DEX,13}}} },
#endif
  { FT_POINT_BLANK_SHOT, FF_WARRIOR, "Point-Blank Shot",
    "You gain a +1 circumstance bonus to hit and damage when "
      "you make ranged attacks against foes within 3 squares of you." },
  { FT_DEFENSIVE_SHOT, FF_WARRIOR, "Defensive Shot",
    "You do not provoke an attack of opportunity from those foes " 
      "next to you when you fire a ranged weapon." ,
    {{{FP_FEAT,FT_POINT_BLANK_SHOT}}}},
  { FT_RAPID_SHOT,  FF_WARRIOR, "Rapid Shot",
    "Your Speed when using a ranged weapon is increased by 25%.",
    {{{FP_FEAT,FT_FAR_SHOT,0},{FP_FEAT,FT_POINT_BLANK_SHOT,0}}}},
  { FT_PRECISE_SHOT, FF_WARRIOR, "Precise Shot",
    "When you shoot at a target and there is another creature in "
      "the line of fire, there is only a 25~ chance of the game rolling "
      "your attack against that creature to see if you hit it instead "
      "of your target, instead of the normal 75~ chance.",
    {{{FP_FEAT,FT_POINT_BLANK_SHOT}}}},
  { FT_AIMED_SHOT, FF_WARRIOR, "Aimed Shot",
    "You can put an arrow exactly where you want it. When you attack with a "
    "ranged weapon, for every three points above your foe's Defense that your "
    "attack rolls scores you add a +1 bonus to the attack's damage roll, to a "
    "maximum of your secondary Dexterity modifier.",
    {{{FP_FEAT,FT_FAR_SHOT,0},{FP_FEAT,FT_POINT_BLANK_SHOT, 0}}}},

  /* The Weapon Expert Tree */


  { 0, } , // ending element

} ; // ww: endof: "FeatInfoStruct FeatInfo[]" 

