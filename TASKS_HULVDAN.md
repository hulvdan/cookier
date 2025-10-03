x fix: weapons Z оружий, чтобы не "мелькали" в некоторых случаях
x кнопке можно указать 9slice + color + flash
x fix padding статов
x оружию можно указать иконку
x движок: можно загрузить несколько шрифтов, чтобы они использовали один и тот же атлас
  (минимизируя draw call-ы), но имели разные настройки размера, обводки и т.п.
x мелкий шрифт для отображения статов
x фикс расстояния получения контактного урона
x fix: увеличение HP при повышении уровня (выборе апгрейда) отображается в HP bar-е сразу
x оставшиеся до конца волны секунды отображаются, как в brotato
  (вверху "WAVE #", под этим - оставшиеся секунды)
x showing "run completed!" after completing the last wave
x fix: reset damage numbers / projectiles upon getting into shop
x melee weapons attack from player's center of player
x melee weapons have guaranteed collision detection disregarding ATTACK_SPEED and RANGE
x boss enemy that forces wave to finish upon getting killed
x can spawn certain enemies in groups
x fix ranged weapons shooting trajectory
x boss HP bar
x correct melee weapons damage registration
  + better easing of melee weapons attacking from player's center
x updated ui frame + button sprites

* turrel
  + can't burn (can specify ailment resistances for mobs (e.g. for burning / zap))
  + can't move
  + doesn't get hurt by player
  + targets enemies
  + forecasts enemy positions
  + damages enemies
  + doesn't get hurt by enemies
  * proper spawning (via items)
  * can specify range + proejctile_type
  * correct gun texture placement
  * correct bullet spawn pos
  * knockback / pierce / bounce
  * damage scaling
* stat descriptions
* draw melee weapon attack colliders
* store texture "original size" disregarding atlas_d2 / atlas_d4
  + refactor use of atlas_d2 / ASSETS_TO_LOGICAL_RATIO
* save system
* shadows (shadow offsets, sizes)
* coin pickup particles
* keyboard input prompts
* navigating in pickedup item / upgrades / shop / end with keyboard
* stats coloring
* weapon tags (primitive etc.) + set bonuses
* weapon effect: +dmg for a number of the same weapon
* on-player-equipped item layers (hats, decals, etc.) (isaac-like)
*
*
* slashing melee attacks
* stats: % Burning Speed: Increases or decreases the rate of which Burn ticks for damage.
* pause: unpause on touch
* pause: touch support
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
