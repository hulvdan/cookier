WORKING SINCE 2025-09-07

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
x ref: placeholders on trashArena
  remove BF_MAX_PLACEHOLDERS_IN_STRING
x weapon effects: show: MORE_EMPTY_WEAPON_SLOTS_MORE_PROPERTY
x weapon effects: show: X_PERCENT_MORE_DAMAGE_TO_ENEMIES_ABOVE_Y_PERCENT_HP
x weapon effects: show: X_PERCENT_MORE_DAMAGE_TO_ENEMIES_BELOW_Y_PERCENT_HP
x weapon effects: show: X_COOLDOWN_SECONDS_EVERY_Y_HITS
x weapon effects: placeholder `is_percent`
x weapon effects: show: X_CHANCE_TO_EXPLODE
x weapon effects: show: X_CHANCE_TO_GET_COIN_UPON_KILLING_WITH_CRIT
x weapon effects: show: DEAL_X_PERCENT_OF_ENEMY_CURRENT_HP_BONUS_DAMAGE_Y_FOR_BOSSES
x weapon effects: show: DROP_X_COINS_ON_HIT_WITH_Y_CHANCE
x weapon effects: show: GET_STAT_FOR_EVERY_OCCUPIED_WEAPON_SLOT
x weapon effects: show: GET_STAT_FOR_EVERY_EMPTY_WEAPON_SLOT
x weapon effects: show: GET_STAT_EVERY_X_SECONDS_DURING_THIS_WAVE
x weapon effects: show: GET_STAT_EVERY_X_WALKED_METERS_DURING_THIS_WAVE
x weapon effects: show: GET_STAT_EVERY_X_IDLE_SECONDS_DURING_THIS_WAVE
x weapon effects: show: PROPERTY_WHEN_IDLE
x weapon effects: show: PROPERTY_WHEN_WALKING
x weapon effects: show: SPAWNS_GARDEN_EVERY_X_SECONDS
x weapon effects: show: SPAWNS_LANDMINE_EVERY_X_SECONDS_DEALING_Y_DAMAGE
x weapon effects: show: ALWAYS_CRITS_WHEN_HITTING_BURNING_ENEMIES
x weapon effects: show: BURNING_SPREADS_TO_ADDITIONAL_X_ENEMIES
x weapon effects: impl: X_PERCENT_MORE_DAMAGE_TO_ENEMIES_ABOVE_Y_PERCENT_HP
x weapon effects: impl: X_PERCENT_MORE_DAMAGE_TO_ENEMIES_BELOW_Y_PERCENT_HP
x weapon effects: impl: DROP_X_COINS_ON_HIT_WITH_Y_CHANCE
x weapon effects: impl: DEAL_X_PERCENT_OF_ENEMY_CURRENT_HP_BONUS_DAMAGE_Y_FOR_BOSSES
x weapon effects: impl: ALWAYS_CRITS_WHEN_HITTING_BURNING_ENEMIES
x yandex/web load savedata
x fix: on start of the game player's health is 20.
  Should be 25 (because of Build's effect)
x weapon effects: impl: MORE_EMPTY_WEAPON_SLOTS_MORE_PROPERTY
x weapon effects: impl: GET_STAT_FOR_EVERY_OCCUPIED_WEAPON_SLOT
x weapon effects: impl: GET_STAT_FOR_EVERY_EMPTY_WEAPON_SLOT
x weapon effects: impl: PROPERTY_WHEN_IDLE
x weapon effects: impl: PROPERTY_WHEN_WALKING
x weapon effects: impl: GET_STAT_EVERY_X_WALKED_METERS_DURING_THIS_WAVE
x weapon effects: impl: GET_STAT_EVERY_X_IDLE_SECONDS_DURING_THIS_WAVE
x weapon effects: impl: GET_STAT_EVERY_X_SECONDS_DURING_THIS_WAVE
x weapon effects: impl: HIT_SPAWNS_X_PROJECTILES_DEALING_Y_DAMAGE
x weapon effects: impl: X_COOLDOWN_SECONDS_EVERY_Y_HITS
x weapon effects: show: (ranged weapon) PIERCES_UP_TO_X_TIMES_ON_CRIT
x weapon effects: impl: (ranged weapon) PIERCES_UP_TO_X_TIMES_ON_CRIT
x gathered most of the weapon effects

* START_WITH_X_ITEM
* START_WITH_X_WEAPON
* X_CHANCE_TO_INSTANTLY_ATTRACT_A_COIN (25%) (max 4 of this item)
* X_STAT_UPON_LEVEL_UP
* X_COINS_UPON_PICKING_UP_A_CRATE
* X_PERCENT_MORE_STATS_GAINED_FROM_UPGRADES
* GET_STAT_WHEN_WALKING
* GET_STAT_WHEN_IDLE
* HARVESTING_INCREASES_BY_ADDITIONAL_X_PERCENT_AT_THE_END_OF_THE_WAVE
* CANT_EQUIP_MELEE_WEAPONS
* CANT_EQUIP_RANGED_WEAPONS
*
* STAT_FOR_WEAPONGROUP
* PROPERTY_FOR_WEAPONGROUP
*
* weapon effects: show: (ranged weapon) BOUNCES_UP_TO_X_TIMES_ON_CRIT
* weapon effects: impl: (ranged weapon) BOUNCES_UP_TO_X_TIMES_ON_CRIT
*
* projectile weapon that shoots itself (javelin)
*
* gather other weapon/diffic/build/item effects
*
* slows down enemies in a radius around the projectile
* every Nth (5 4 3 2) has +100 crit chance
* SLOWS_ENEMIES_ON_HIT_BY_X_PERCENT_FOR_EVERY_STAT_YOU_HAVE (1%, 2%, engineering)
* pierces through enemies
* crits bounce
*
* X_CHANCE_TO_HEAL_Y_HP_UPON_DODGING (50%, 5 hp)
* X_CHANCE_TO_HEAL_Y_HP_UPON_PICKUPABLE (8%, 1 hp, coin)
* shoots 6 alien eyes aound you for every 3 seconds dealing 6 + 50% HP damage each
* random weapon is upgraded upon entering the shop, gain +2 armor when theres no weapon to upgrade
* X_CHANCE_TO_DEAL_Y_DAMAGE_UPON_PICKING_UP_A_COIN (25%, 1 + 25% luck)
* SPAWN_PROJECTILE_FROM_ENEMY_CORPSE_DEALING_Y_DAMAGE
* SPECIAL_ENEMIES_APPEAR_AT_THE_BEGINNING_OF_NEXT_WAVE
* WEAPONS_MINIMUM_COOLDOWN_IS_X_SECONDS
* X_SELF_DAMAGE_PER_Y_SECONDS (-X здоровье раз в Yс) (no invulnerability)
* GET_STAT_FOR_EVERY_STAT2_YOU_HAVE
* X_CHANCE_TO_GET_STAT_Y_CHANCE_TO_GET_STAT2_UPON_REROLLING
* GET_STAT_FOR_Y_SECONDS_UPON_PICKUPABLE
* GET_STAT_DURING_THE_NEXT_WAVE
* GET_STAT_FOR_EVERY_LIVING_ENEMY
* GET_STAT_EVERY_X_SECONDS_DURING_THIS_WAVE_BONUS_IS_LOST_UPON_TAKING_DAMAGE
* GET_STAT_UPON_PICKUPABLE_WHILE_AT_MAX_HP (1 max hp, consumable)
* GET_STAT_UPON_PICKUPABLE_WHILE_AT_MAX_HP_UNTIL_THE_END_OF_THE_WAVE (1 hp regen, consumable)
* PROJECTILES_GET_PIERCING_UPON_CRIT
* PROJECTILES_GET_BOUNCE_UPON_CRIT
* stat: accuracy [-PI32 / 4, PI32 / 4]
* STAT_FOR_EVERY_DIFFERENT_TIER_ITEM
* STAT_FOR_EVERY_BURNING_ENEMY
* STAT_FOR_EVERY_DIFFERENT_WEAPON
* DAMAGE_SCALES_WITH_ADDITIONAL_Y
* GARDEN_SPAWNS_EVERY_N_SECONDS (spawns a garden that creates apple every 15 seconds)
* CHANCE_TO_HEAL_X_HP_UPON_KILLING
* ITEMS_WILL_BE_X_TIER_HIGHER_UPON_THE_NEXT_REROLL
* CRITS_DEAL_X_PERCENT_OF_ENEMY_CURRENT_HP_BONUS_DAMAGE_Y_FOR_BOSSES
* BURNING_DEALS_X_PERCENT_OF_ENEMY_CURRENT_HP_BONUS_DAMAGE_Y_FOR_BOSSES
* STAT_IS_CAPPED
* ENEMIES_TAKE_X_PERCENT_MORE_DAMAGE_DURING_Y_SECONDS_UPON_ELEMENTAL_DAMAGE
  ("Elemental Damage" string is GREEN)
* CONSUMABLES_HEAL_OVER_X_SECONDS
* STRUCTURES_CAN_CRIT
* TURRENT_FROM_DEAD_TREE
* X_PERCENT_MORE_COINS_FROM_RECYCLING
* STAT_WHILE_LOWER_THAN_X_PERCENT_HP (hp regen x2, <=50% hp)
* STAT_WHILE_HIGHER_THAN_X_PERCENT_HP
* ON_DEATH_ENEMIES_EXPLODE_WITH_X_PERCENT_CHANCE_DEALING_Y_DAMAGE
* DEAL_X_DAMAGE_AFTER_DODGING_ENEMY_ATTACK (300% melee, one time to that enemy specifically)
* STARTING_WITH_X_PERCENT_LESS_HP
  !!! STARTING_WITH_1_HP
* START_THE_NEXT_WAVE_WITH_1_HP
  !!! STARTING_WITH_X_PERCENT_LESS_HP
* EVERY_RANGED_WEAPON_X_TH_SHOT_HAS_ADDITIONAL_X_PROJECTILES (5th, +3 projectiles)
* GET_STAT_EVERY_TIME_YOU_GET_ITEM_THAT_INCREASES_STAT2
* PICKUPABLE_EXPLODES_WITH_X_PERCENT_CHANCE_DEALING_Y_DAMAGE (apple, 50%, 10 + 100% Max HP)
* NULLIFIES_ONE_HIT_TAKEN_EVERY_WAVE
* X_PERCENT_CHANCE_TO_HEAL_Y_HP_UPON_KILLING
* X_PERCENT_CHANCE_TO_HEAL_Y_HP_UPON_KILLING_WITH_CRIT (20%, 1 hp)
* RESTORE_X_HP_PER_SECOND_WITHOUT_ABILITY_TO_HEAL_ANY_OTHER_WAY (5 hp per seconds. life steal / apples don't work anymore)
* TURRET_DEALING_X_DAMAGE
* EXPLOSIVE_TURRET_DEALING_X_DAMAGE
* INCENDIARY_TURRET_DEALING_X_DAMAGE
* LITTLE_DUDE_DEALING_X_DAMAGE_SPAWNING_Y_PROJECTILES (10 + 75% engineering + 75% elemental damage, 10 projectiles)
* SLOW_DOWN_ENEMY_BY_X_PERCENT_ON_HIT_MAX_IS_Y (5, 20)
* BOT_THAT_SLOWS_DOWN_ENEMIES_BY_X_PERCENT_IN_RADIUS_Y_METERS
* EXPLODE_ONCE_UPON_BEING_BELOW_X_PERCENT_HP_DEALING_Y_DAMAGE
* stat_type: EXTRA_ITEM_IN_A_CRATE
* stat_type: LOOT_ALIENS_CHANCE_SCALE
* stat_type: LOOT_ALIENS_SPEED
* STAT_MODIFICATION_CHANGED_BY_X_PERCENT
  !!! in localization: instead of increased / decreased use "'Max HP' modifications: +-50%"
* stat_type: XP_REQUIRED_TO_LEVEL_UP
* stat_type: MAX_N_MELEE_WEAPONS
* stat_type: MAX_N_RANGED_WEAPONS
* CANT_EQUIP_WEAPONS_ABOVE_TIER
* CANT_EQUIP_WEAPONS_BELOW_TIER
* X_COINS_AT_THE_END_OF_THE_WAVE_FOR_EVERY_ENEMY
* stat_type: WEAPONS_PRICE
* LOSE_N_PERCENT_COINS_AT_THE_START_OF_THE_WAVE
* stat_type: STRUCTURES_SPAWN_CLOSE_TO_EACH_OTHER
* stat_type: COINS_DROPPED
* stat_type: COINS_DROPPED_FROM_ENEMIES
* SHOP_ALWAYS_SELLS_AT_LEAST_X_WEAPON
* SHOP_ALWAYS_SELLS_AT_LEAST_X_ITEM
* WEAPONS_ARE_DESTROYED_UPON_ENTERING_SHOP
* X_MATERIALS_PER_SECOND_MAX_Y_WHILE_IDLE
* X_MATERIALS_PER_SECOND_MAX_Y_WHILE_WALKING
* GET_STAT_FOR_EVERY_X_COINS
* DEAL_Y_DAMAGE_TO_A_RANDOM_ENEMY_UPON_HEALING
* turrets don't attack trees
* CANT_ATTACK_WHILE_MOVING
* CANT_ATTACK_WHILE_IDLE
* CANT_USE_WEAPONS
* EXPLODE_FOR_X_DAMAGE_Y_CRIT_MULTIPLIER_UPON_TAKING_DAMAGE
* X_PERCENT_COINS_CONVERTED_INTO_MAX_HP_BUY_ITEMS_WITH_MAX_HP
* GAIN_WEAPON_SLOT_UPON_LEVEL_UP_INSTEAD_OF_UPGRADE_MAX_X
* START_WITH_X_WEAPON_SLOTS
* WEAPONS_MUST_BE_UNIQUE
* CONVERT_X_UNCOLLECTED_COINS_INTO_STAT
* WEAPON_DAMAGE_SCALES_WITH_N_PERCENT_STAT
*
*
*
*
*
*
*
* weapon effects: impl: X_CHANCE_TO_EXPLODE
* weapon effects: impl: BURNING_SPREADS_TO_ADDITIONAL_X_ENEMIES
  PROBABLY CAN USE JUST PROPERTY WITH AN ALREADY EXISTING EFFECT
* weapon effects: impl: SPAWNS_GARDEN_EVERY_X_SECONDS
* weapon effects: impl: SPAWNS_LANDMINE_EVERY_X_SECONDS_DEALING_Y_DAMAGE
*
*
* fix guarantee input press
* на русском апгрейд "рукопашный урон" хреново смотрится
  мб "рукопашная"?
* weapon effects: impl: X_CHANCE_TO_GET_COIN_UPON_KILLING_WITH_CRIT
*
* стереть недопонимание PROPERY / Стат. "Именно ЭТО оружие получает урон +4"
*
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: impl:
* weapon effects: show: HITTING_SPAWNS_X_PROJECTILES_THAT_BOUNCE_Y_TIMES_PIERCE_Z_TIMES_DEALING_W_DAMAGE
* weapon effects: show:
* weapon effects: show:
* weapon effects: show:
* weapon effects: show:
* weapon effects: show:
*
* weapon / non weapon effects: X_PERCENT_MORE_DAMAGE_TO_ENEMIES_ABOVE_Y_PERCENT_HP
* weapon / non weapon effects: X_PERCENT_MORE_DAMAGE_TO_ENEMIES_BELOW_Y_PERCENT_HP
* weapon / non weapon effects: DROP_X_COINS_ON_HIT_WITH_Y_CHANCE
* weapon / non weapon effects: DEAL_X_PERCENT_OF_ENEMY_CURRENT_HP_BONUS_DAMAGE_Y_FOR_BOSSES
* weapon / non weapon effects: ALWAYS_CRITS_WHEN_HITTING_BURNING_ENEMIES
*

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
- resets cooldown of offensive turrets upon attacking
- spawns turret / incendiary turret / laser turret / explosive turrent

- deals 8x5/12x6/15x7 (+100% elemental damage) burning damage
- deals 3x3/5x5/8x6/12x9 (+elemental) burning damage.
- burning spreads to additional -/-/1/1 nearby enemies

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
* emscripten: how to return string from localStorage from js function to c++?
