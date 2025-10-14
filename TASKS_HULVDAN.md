x стилизовал UI под brotato
  (размеры шрифтов, обводки, цвета фреймов, цвета текстов, hover кнопок)
x локализация теперь вся в localization.csv
x sfx ui hover / click / error
x когда нет денег и игрок пытается купить - монетки туда-сюдакают
x когда нет свободного слота и игрок пытается купить оружие - оружия туда-сюдакают
x чёткие шрифты
x при старте след волны удаляю все поднимаемые предметы
x 10% вариация кулдауна оружий
x вариация урона +/- 1
x моб, что 100% дропает сундук
x существа загораются белым на небольшое время, когда получают урон
x норм меню (пока что кнопка P в браузере)
x save / load на ПК
x save / load health so that cheating doesn't work
x if health is 0 and it's gameplay -> show end screen
x Differentiate notPickedUpCoins/notPickedUpCoinsVisual
  so that saving during ScreenType_WAVE_END_ANIMATION would be fair
x reopening game during the wave resets coins and xp
  to the values at the start of the wave
x save/load difficulty, build, achievements
x difficulty levels
x new run: can select difficulty + build + weapon and start a run
x new run: explain button error through wiggling
x new run: don't show lock if weapon doesn't exist
x new run: save/load last run difficulty/build/weapon
x new run: starting a new run works without bugs
x achievements: show recently got achievements on screen and what they unlock
x achievements: remove build lock in gamelib.fbs
x achievements: locked items/weapons/classes can't be acquired before unlocking
x achievements: calculating WALKER / IDLER
x achievements: descriptions
x achievements screen: drew cells
x locale gen caps (New Run / NEW RUN)
x achievements screen: ACHIEVEMENTS label
x achievements screen: pressing ESCAPE returns us back to paused menu

* better pause menu
* achievements screen: showing reward description in right column
* achievements screen:
* achievements screen:
* achievements screen:
* achievements:
* achievements:
* completing difficulty can unlock stuff
* save load: more tests!
* new run: dim overlay on pressing `new run` in pause menu
* new run: showing selected difficulty/build/weapon name
* new run: showing hover weapon details
* new run: initial weapon recycle price
* new run: button selection needs to be rewoked for PC
* new run: go button big green or rework in steps
* new run: build icons
* yandex save
* ref extract reroll button
* classes
* achievements
* menu: change class and start new run
* menu: web when touch enabled: button that can be touched turing the gameplay to get into menu
* menu:
* menu:
* menu:
* menu:
* menu:
* localization sorting
* mob with crate: guaranteed cooldown?
* hp bar black outline
* fix shop items/weapons frame height
* design achievements
* weapon detail icons? (combine / sell / cancel)
* luck = подкова
* multiple level ups icons in the top right corner (like picked up crates)
* shop items locking
* draw UI? writing code gets tiring
* turrel: show damage in item
* landmine: show damage in item
* landmine: show spawn rate "Gets spawned every %.1f seconds"
* garden: show interval of fruit spawning "Spawns fruit every %.1f seconds"
* clay hovered modal shadows
* stat descriptions
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
* «upgraded stat» number (show stat's icon instead of text) upon levelup
*
*
* ZHEKA: Cheats?
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
