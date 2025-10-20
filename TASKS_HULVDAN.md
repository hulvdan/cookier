x yandex save: pass data
x ref remove g.run.state.statsWithoutItems
x fix black flag trash memory effect in shop
  ("for every TRASH killed enemies")
  to index `condition_value` pass `tierOffset`
x fix player max HP
x fix starting wave with a lot of money + piggyBank sets coins to 0
x weapon effects: gamelib.py placeholder aliases
x weapon effects: effect `condition_x` `condition_y` etc to `condition_values` array
x placeholders groups
  can specify "hitting an enemy spawns 3/4/5/6 projectiles dealing 50/60/70/80% {ranged damage icon} damage"

* cheat: wheel on weapon changes tier
* weapon effects:
* weapon effects:
* weapon effects:
* weapon effects:
* weapon effects:
* weapon effects:
* weapon effects:
* ref: placeholders on trashArena
  remove BF_MAX_PLACEHOLDERS_IN_STRING
* ref: placeholderCompound -> group

melee weapon effects:
- hitting enemy spawns 3/4/5/6 projectiles that deal (50/60/70/80% of ranged damage)
- deal +25/+50 damage for every free weapon slot
- drop coins chance on hit (brick breaks) (10/30/60/120 coins per tier)
- cooldown is 1.83s/1.52s every 30/40 shots
- deal 10%/20% of enemy current health as bonus damage (1%/2% bosses + elites)
- +1/+1/+1/+2 healing from consumables
- melee weapon also can explode with some chance
- X chance to gain 1 coin upon killing an enemy with a crit
- +X stat every Y seconds until the end of the wave (+10% attack speed, every 5 seconds)
- -3 armor for every weapon you have
- deals 8x5/12x6/15x7 (+100% elemental damage) burning damage
- +X stat (tiered)
- +X stat for every Y steps during this wave
- +X stat for every Y killed enemies using this weapon TODO DURING THIS WAVE
- -X% damage while standing still
- hitting enemy spawns lightning projectile that bounces -/1/2/3 times
  and inflicts 5/6/8/11 + (80% elemental damage)
- damaged enemies take 10% more damage for 3 seconds. stacks 3/5/7/10 times
- spawns garden every 15/14/12/10 seconds
- landmine spawns every 12/9/6/3 seconds dealing 10 + 100% engineering damage
- +X% tiered damage against targets above/below Y% health
- always crits when hitting burning enemies
- stick: tiered bonus damage for each of the same weapon
- 50/56/62/80% to gain coin unpon killing with crit using this weapon
- deals 3x3/5x5/8x6/12x9 (+elemental) burning damage. burning spreads to additional -/-/1/1 nearby enemies
- resets cooldown of offensive turrets upon attacking
- spawns turret / incendiary turret / laser turret / explosive turrent

ranged weapon effects:
- cooldown is reset upon picking up coins
- cooldown is X every Y shots
- pierces X enemies dealing -Y damage with each pierce
- pierces up to X enemies on critical hit
- projectiles explode on hit
- hitting an enemy that has than 50/65/70/75% hp
  has 10/15/20/25% chance to CHARM it for 8 seconds
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-
-

item effects:
-
-

* implement all weapon/item/build/difficulty effects
* weapon tags (primitive etc.) + set bonuses
* can take damage if dodged
* fix: web release: why pressing P doesn't show pause UI?
* keyboard UI nav: pecked up item
* keyboard UI nav: upgrades
* keyboard UI nav: shop
* keyboard UI nav: end
* keyboard UI nav: pause
* keyboard UI nav: new run
* keyboard UI nav:
* keyboard UI nav:
* keyboard UI nav:
* keyboard UI nav:
* keyboard UI nav:
* keyboard UI nav:
* touch: pause button during gameplay
* touch: can play using finger
* yandex save: emscripten pass string from C++
* ach screen: animate unlocked weapon tiers (different damages / scalings)
* fix: shop: disable keyboard R1234 while weapon modal is opened
* fix multiple one-by-one sliding achievements translation
* save load: more tests!
* new run: showing selected difficulty/build/weapon name
* new run: button selection needs to be rewoked for PC
* new run: go button big green or rework in steps
* new run: build icons
* localization sorting
* mob with crate: guaranteed cooldown?
* hp bar black outline
* luck = подкова
* multiple level ups icons in the top right corner (like picked up crates)
* draw UI? writing code gets tiring
* turrel: show damage in item
* landmine: show damage in item
* landmine: show spawn rate "Gets spawned every %.1f seconds"
* garden: show interval of fruit spawning "Spawns fruit every %.1f seconds"
* stat descriptions
* coin pickup particles
* stats coloring
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
* StatType_RANGE brotato's min range is 25 (how much is it in meters?)
* «upgraded stat» number (show stat's icon instead of text) upon levelup
* gather all weapon classes (e.g. primitive)
* weapon tag: primitive 3 6 9 12 15 hp
* weapon tag: ethereal 3 6 9 12 15 armor
* weapon tag: explosive: 5 10 15 20 25 explosion size
* weapon tag: elemental: 1 2 3 4 5 elemental damage
* weapons categorization. жидкое / твёрдое?

ART:
================================================================================
* art: crate (as chest)
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:
* art:

SFX:
============================================================
* sfx: ui: button click
* sfx: ui: button hover big
* sfx: ui: buy
* sfx: ui: sell
* sfx: ui: weapon combine
* sfx: ui: reroll
* sfx: ui: go next wave
* sfx: ui: mob hurt
* sfx: ui: player hurt
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx: ui:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:
* sfx:

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
- моб с сундуком
- mob boss - галет (он сухой)
- tree - cow / apple tree
* mob that rotates when moving?

TECH DEBT
============================================================
* broken_strings: proper lines support
* engine: different atlases for different resolutions
* engine: handle the case when there's too much textures to pack into 1 atlas
* engine: nine slice should work even if width/height of image is very small
* HTML game loading screen?
* record+replay system
* trim pngs of transparent pixels
* internet shouldn't be required for building:
  check web debug yandex build with mocked ysdk
* font shader that remaps fillColor and outlineColor
* enable linting: clang-tidy to show f32 int / int f32 conversions
* engine fonts: fix scaling of not yet reloaded fonts (y position)
* fix sometimes sdl thinks window is out of focus -> sounds mutes
* engine: LoadFonts per localization

LATER:
================================================================================
* draw melee weapon attack colliders
* when player can be burnt:
  make sure that burn ticks don't set player to be invincible
* save: starting wave completed animation saves the game
* windows save in another thread (+profile dumping / saving to file)
* brotato gog
* shop item locks
* weapon detail icons? (combine / sell / cancel)
* clay hovered modal shadows
* keyboard input prompts
* is brotato arena squared?
* aseprite show image center
* yandex save: check it works
* REWORK_ME_WEAPON_EFFECT_CHANCE_OF_EXPLOSION
