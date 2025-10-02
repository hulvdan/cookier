x fix: weapons DrawZ
x button 9slice + указание color и flash
x fix padding статов
x can set weapon icon texture_id
x engine: multiple fonts loading that use the same atlas and can use different outlines, codepoints
x using smaller font to display stats
x fix: accurate contact damage hitbox

* upgrades also recalculate player stats
* save system
* rework wave remaining seconds like in brotato
* turrel
* coin pickup particles
* keyboard input prompts
* navigating in pickedup item / upgrades / shop / end with keyboard
* stats coloring
* weapon tags (primitive etc.) + set bonuses
* weapon effect: +dmg for a number of the same weapon
* on-player-equipped item layers (hats, decals, etc.) (isaac-like)
*
*
*
*
*
*
*
*
*
*
*
*
*
* melee weapons attack from player's center of player
* slashing melee attacks
* stats: % Burning Speed: Increases or decreases the rate of which Burn ticks for damage.
* pause: unpause on touch
* pause: touch support
* stat descriptions
* using `value_multiplier` with stat that has `is_percent=true` can confuse people.
  assert that value_multiplier && is_percent is false during codegen
* explosion: damage scalings (look at brotato)
* think about boss
  + shoots oooo oooo oooo rows of projectiles like in brotato?
  + what would cookie-boss do?
* is brotato arena squared?
* StatType_RANGE brotato's min range is 25 (how much is it in meters?)
*
*
* stat effect: ENGINEERING
  Engineering increases the Damage and healing of your Structures, such as Turrets and Landmines.
  Engineering also increases the attack damage of the Weapons Plank and Screwdriver.
* stats: % Structure Attack Speed: Increases or decreases the rate of turrets attacking, Landmines spawning, and Gardens spawning Consumables.
* stats: Structure Range: Increases or decreases the range of Structures.
* «upgraded stat» number (show stat's icon instead of text) upon levelup
*
*
* ZHEKA: can web build
* ZHEKA: total commander for working with sounds?
  instructions of how to work with sounds
  - finding them using total commander (alt+F7)
  - sending them for listening to foobar2000
    (total commander: «Feed to listbox» then drag'n drop)
  - fast editing with audacity (fade in, fade out, compression, export macroses)
* gather all weapon classes (e.g. primitive)
* weapon tag: primitive 3 6 9 12 15 hp
* weapon tag: ethereal 3 6 9 12 15 armor
* weapon tag: explosive: 5 10 15 20 25 explosion size
* weapon tag: elemental: 1 2 3 4 5 elemental damage
* weapons categorization. жидкое / твёрдое?
*
*
* aseprite show image center

QA:
================================================================================
* StatType_ENEMY_SPEED
* StatType_ARMOR
* StatType_COINS_HEAL
*
*

NOTES:
================================================================================
MOBS:
- default mob - стандартная с шоколадными крошками
- ranged mob стреляет шоколадными крошками
- heavy rusher mob
- heavy spawner mob - толстая шрек печенька
- tiny mob
- mob boss - галет (он сухой)
- tree - cow / apple tree
* mob that rotates when moving?

TECH DEBT
============================================================
* broken_strings: proper lines support
* engine: better fonts rendering
* engine: automatically scale fonts for current screenSize
* engine: different atlases for different resolutions
* engine: handle the case when there's too much textures to pack into 1 atlas
* engine: nine slice should work even if width/height of image is very small
* HTML game loading screen?
* record+replay system
* trim pngs of transparent pixels
* internet shouldn't be required for building:
  check web debug yandex build with mocked ysdk
* font shader that remaps fillColor and outlineColor
