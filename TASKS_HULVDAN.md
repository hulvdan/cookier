x kenney ui button
x better ui frame
x palettized secondary text color
x generate palette for aseprite
x extracted localization.csv
x use localization.csv
x fix this wave damage
x sfx: ui: button hover small
x smaller font for weapon and item properties in shop
x brighter text colors
x fix smg life steal 100%
x fix componentStats button spacing on the right
x fix can't hover / click other buttons when weapon is selected in shop
x tier-colored shop item/weapon frames
x tiered hovered modal colors
x bigger font of prices
x combine button doesn't get displayed if weapon can't be combined
x fix zIndex of modal (stat icons don't get displayed over overlay)
x don't show empty weapon slots in shop
x items + weapons also make click / hover sfx
x don't show items/weapons labels in shop/end if player doesn't have any
x fix componentStats primary/secondary button sizes
x keyboard actions also trigger button sounds.
  don't make clicked / error sounds from buttons
x shop reroll button red if no money
x ui sfx has pitch variation
x lowered pitch of hover sfx
x upgrades / shop buy gold wiggling + weapons wiggling upon error
x tried to design waves progress bar
x everythere but in hud default font is not outlined
x engine: crisp fonts rendering
x engine fonts: correct placement on both resolutions
x engine fonts: scale fonts while they haven't been reloaded yet
x engine: don't load filedata of the same font file multiple times
x resetting all pickupables on starting next wave. not leaving even crates
x small cooldown variation
x small damage +1/-1 variation
x mob with crate: doesn't get aggroed
x engine: bulletproof `LoadFonts`: atlas scaling
x engine: `ReloadFonts` implementation. Not unloading -> loading back filedata
x creatures flashing on getting damaged
x gathered + organized references
x menu: continue button
x menu: restart button
x menu: exit game (PC)
x saving: flatbuffers schema
x saving: ref SDL_SaveFile
x saving: debounced saving
x saving: robust save&swap saving algorithm

* saving: dumping data to a binary file
* saving: loading data from a binary file
* saving:
* saving:
* saving:
* saving:
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

IRRELEVANT:
================================================================================
* draw melee weapon attack colliders
* when player can be burnt:
  make sure that burn ticks don't set player to be invincible
