x can specify range + projectile_type + knockback + damage scalings
x turrel: pierce / bounce
x ref move FrameGame, FrameVisual, lframe to engine
x turrel: remove gun texture
x turrel: proper spawning via item
x turrel: correct bullet spawn pos
x fix fonts outline advance
x fix getting more health from upgrade ADDS HP to player (not only to max HP)
x fix slingshot bouncing
x ref damage int
x fix mob spawning bug
x worked a bit on making lint work
x weapon effect same weapon more damage: shows up correctly in weapon's card
x weapon effect same weapon more damage: make effect actually apply on weapon's damage
x show numbers of damage that mobs deal to player
x stats: Structure Range
x stat structure atk speed: rate of turrets attacking
x garden: item
x garden: spawning in the world
x landmine: spawning object in the world
x landmine: detonation
x landmine: damage scalings, explosion size damage etc
x landmine: structure atk speed affects spawning
x landmine: draw on floor
x garden: spawns consumables
x garden: structure atk speed affects

* creature shadows (shadow offsets, sizes)
* turrel: show damage in item
* landmine: show damage in item
* landmine: show spawn rate "Gets spawned every %.1f seconds"
* garden: show interval of fruit spawning "Spawns fruit every %.1f seconds"
* ZHEKA
  Cheats?
* clay hovered modal shadows
* stat descriptions
* store texture "original size" disregarding atlas_d2 / atlas_d4
  + refactor use of atlas_d2 / ASSETS_TO_LOGICAL_RATIO
* save system
* coin pickup particles
* keyboard input prompts
* navigating in pickedup item / upgrades / shop / end with keyboard
* stats coloring
* weapon tags (primitive etc.) + set bonuses
* on-player-equipped item layers (hats, decals, etc.) (isaac-like)
*
*
* slashing melee attacks
* stats: % Burning Speed: Increases or decreases the rate of which Burn ticks for damage.
* pause: unpause on touch
* pause: touch support
* fb effects: using `value_multiplier` with stat that has `is_percent=true`
  will mislead players / developer.
  assert that `value_multiplier && is_percent` is false during codegen
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
*
*
* aseprite show image center

QA:
================================================================================
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
* engine: crisp fonts rendering
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
* clang-tidy to show f32 int / int f32 conversions

IRRELEVANT:
================================================================================
* draw melee weapon attack colliders
* when player can be burnt:
  make sure that burn ticks don't set player to be invincible
