WORKING SINCE 2025-09-07

x yandex save
x weapon effects: BOUNCES_UP_TO_X_TIMES_ON_CRIT
x javelin: ranged weapon that shoots itself
x fix: guarantee input press
x show: EVERY__X__USE_CHANGES_CRIT_CHANCE_BY__Y
x show: X__CHANCE_TO_HEAL__Y__HP_UPON_DODGING
x show: X__CHANCE_TO_HEAL__Y__HP_UPON__PICKUPABLE
x show: X__CHANCE_TO_DEAL__Y__DAMAGE_UPON__PICKUPABLE
x show: X__PERCENT_MORE_COINS_FROM_RECYCLING
x placeholder-image scales with font size
x yandex: mark game ready after loading save file
x (this weapon only) / (other weapons only) localization strings
x EVERY__X__USE_CHANGES_CRIT_CHANCE_BY__Y
  should be able to target one weapon OR all weapons
  (This weapon)
x fix: flying coins stood on the floor after flying
x weapon effects: impl: X__CHANCE_TO_EXPLODE
x X__CHANCE_TO_EXPLODE
x X__CHANCE_OF_EXPLOSION_ON_HIT
x impl: universal: X__CHANCE_TO_GET__Y__COINS_UPON_KILLING_WITH_CRIT
x impl: X__CHANCE_TO_HEAL__Y__HP_UPON_DODGING
x impl: X__CHANCE_TO_HEAL__Y__HP_UPON__PICKUPABLE
x impl: X__CHANCE_TO_DEAL__Y__DAMAGE_UPON__PICKUPABLE
x impl: X__PERCENT_MORE_COINS_FROM_RECYCLING

* structures in a circle around center

* touch: pause button during gameplay
* touch: can play using finger
* на русском апгрейд "рукопашный урон" хреново смотрится
  мб "рукопашная"?
*
* стереть недопонимание PROPERY / Стат. "Именно ЭТО оружие получает урон +4"
*
*
*
* implement all weapon/item/build/difficulty effects
* weapon groups (primitive, medical, etc.) + set bonuses
* can take damage if dodged
* fix: web release: why pressing P doesn't show pause UI?
* keyboard UI nav: picked up item
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
* REWORK_ME_WEAPON_EFFECT_CHANCE_OF_EXPLOSION
* weapon groups
* fix: yandex: build not drawing
