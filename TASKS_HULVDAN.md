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
x touch: emulate touch controls in debug when device is set to mobile
x touch: can play using finger
x test on phone: can play
- test on phone: touching screen with another finger overrides old touch
- test on phone: untouching returns control to previous finger
x pause button works good
x touch: fix double press
x preserving hovered element state
x ui keyboard: pause
x ui keyboard: pause: stats
x ui keyboard: achievements
x ui keyboard: upgrades
x ui keyboard: shop
x ref clicked -> activated
x ref hovered -> hovered + (g.ui.selectedElement.id == XXX.id)
x new run 3 screens
x ui keyboard: picked up item
x ui keyboard: new run
x shop rework
x clean level up screen
x level up show PLAYER_COINS
x stats show all stats at the same time big
x stats to the right
x stats: sliding + alpha
x stats: pressing TAB button
x stats: button icon `<-|` so that it looks like TAB
x stats: closing stats by ESC or TAB
x stats: fade in overlay
x stats: can't open stats while weapon details is opened
x end: show stats
x ui keyboard: shop: SPACE / RETURN should work for weapons
x ui keyboard: shop: don't add hidden weapons to group
x ui keyboard: shop: when context menu is active, don't process shop context at all

* ui keyboard: SPACE / RETURN should work in weapon details
* ui keyboard:
* ui keyboard:
* ui keyboard:
* ui keyboard:
* ui keyboard:
* ui keyboard:
* ui keyboard:
* ui keyboard:
* ui keyboard:
* ui keyboard:
*
* shop: mark default: 2 3 4 1 reroll
* ui keyboard: after pressing WASD make default button focused
* ui keyboard: shop: change selected button after buying/recycling/combining
* fix: new run: can't click on buttons
* touch: releasing finger while showing details
  -> details are sticked (or weapon details are shown up)
* touch: selecting weapon details only when released touch.
  if pressed and hovered - just showing it (like on pc - hovering)
*
* shop: selecting weapon -> set close as a default button
* stats: opening stats panel changes keyboard ui nav focus to close button
* sparkles that highlight the weapons can be upgraded
* outline with offset down
* fix: shop: weapons details to the left
*
* modal: are you sure you want to quit?
* modal: are you sure you want to restart?
* modal: are you sure you want to start a new run?
*
* enemy that tanks 1-3 damage
* engine: make `Arena` growing (by default it should assert)
*
* show controls above strips
* bigger buttons (bigger button font)
* touch: pressing item again removes hover from it
* touch:
* mobile: sounds don't work
* mobile: looks like framebuffer didn't scale
* touch: pause button during gameplay
* structures in a circle around center
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
