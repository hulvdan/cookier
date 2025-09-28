x correct piggy bank coins scaling
x max dodge is 60
x range
  negative -50 halves range
  negative -100 quarters it (negative is `scale = -25/x`) !!! REWORK FORMULA
  positive 100  =x2 range
  positive 200  =x3 range (positive is linear `scale = (f32)(x + 100) / 100.0f`)
x using wheel for cheating
x fix StatType_ITEMS_PRICE, StatType_REROLL_PRICE
x weapon effects: giving +1 attack speed upon killing 20 enemies with this weapon
x codegen so that i won't fail upon adding new EffectConditionType

* buying item that gives StatType_FREE_REROLLS should give reroll in shop immediately
* fix: in SHOP when hovering right player's item,
  counters of left ones appear above it! they shouldn't!
* ref malloc/free -> ALLOC/FREE (lib_instrument)
* engine: better fonts rendering
* engine: automatically scale fonts for current screenSize
*
* weapon stats: bounces
* weapon stats: life steal
* weapon stats:
* weapon stats:
* end: can hover on weapon
* elemental weapon with bouncing projectile that slows down / burns mobs
  (chech other types of elemental damage)
*
*
* stat effect: ENGINEERING
  Engineering increases the Damage and healing of your Structures, such as Turrets and Landmines.
  Engineering also increases the attack damage of the Weapons Plank and Screwdriver.
* stats: % Explosion Damage: Increases or decreases the damage of Explosions
* stats: % Explosion Size: Increases or decreases the size of Explosions
* stats: Bounces: Gives projectiles additional Bounces
* stats: % Structure Attack Speed: Increases or decreases the rate of turrets attacking, Landmines spawning, and Gardens spawning Consumables.
* stats: Structure Range: Increases or decreases the range of Structures.
* stats: % Burning Speed: Increases or decreases the rate of which Burn ticks for damage.
* stats: Burning Spread: Increases the amount of times Burn spreads.
* spread mobs apart (use force?)
* mob that rotates when moving?
*
*
* engine: different atlases for different resolutions
* engine: handle the case when there's too much textures to pack into 1 atlas
* wave completed: not picked up coins fly to "stash" during the "WAVE COMPLETED" animation
* wave completed: stashed coins drop during the next wave with x2 value
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
* show melee weapon collider gizmos
* weapons DrawZ
* gather all weapon classes (e.g. primitive)
* primitive weapons bonus (3,6,9,12,15 HP)
* ethereal weapons bonus (3,6,9,12,15 ARMOR or 2,4,X or 1,2,X)
* weapons categorization. жидкое / твёрдое?
*
*
* aseprite show image center
* HTML game loading screen?
* record+replay system
* internet shouldn't be required for building: check web debug yandex build with mocked ysdk
* trim pngs of transparent pixels

QA:
================================================================================
* StatType_ENEMY_SPEED
* StatType_XP_GAIN
* StatType_ARMOR
* StatType_COINS_HEAL
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
