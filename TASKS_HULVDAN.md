x map walls for creatures
x mob pre-spawns
x drew weapons
x weapons shoot
x projectiles fly
x projectiles collide with mobs
x projectiles apply damage
x disable body collisions?
x health bar
x mobs do contact damage
x correct weapon look left don't rotate 180
x game reset on death
x damage numbers
x killing mobs drops coins
x gathering coins
x ui: coins
x ui: xp bar
x killing mobs drops xp
x projectiles can't damage same mobs
x projectiles int pierce
x projectiles max distance destroy
- mobs collide with each other?
  ranged mobs can go throug each other
x zoom in on playable area.
x move camera based on player's position
x going to corner shifts camera to center
x ui: showing «+1 / +2» for coins
x waves
x finishing wave opens level up upgrade
x can pickup upgrade and return to gameplay
x implement GetMouseLogicalPos()
x more stats
x map hard stop at edge
x stat effect: HP
x stat effect: REGEN
2025-09-10
x reroll button
x show current weapons
x show current items
x show stats
x correct hp stat
x don't show bought item/weapon empty slot
x price is red if not enough money
x fix: can't buy when `canBuy` is false
x rendering stable sort
x endgame screen gets shown + restart button works
x endgame screen: blacken background + show cursor
2025-09-11
x endgame screen: current weapons + items
x upgrades get shown only after level up
x correct player weapon angles
  2 weapons 180
  3 weapons 120
  4 weapons 90
  5 weapons 72
  6 weapons 60
x resetting weapon target when no mob is in range
x fix of it
x item / weapons tiers
x upgrades screen: upgrade name
x upgrades screen: stats
x upgrades screen: button
x upgrades screen: reroll
2025-09-12
x upgrades screen: show upgrade effects
x upgrades screen: show gameplay UI (so that coins are shown)
x 9slice: made test texture + added 9slice frame element in shop
2025-09-13
x 9slice: drawing of 9slice UI element
x 9slice: correct 9slice margins
x upgrading weapons via buying
x items stack
x melee weapons that get thrown
x internet shouldn't be required for building: check web
2025-09-14
x show item stacks in the end screen
  (ref extract items + weapons components)
x mob "tree": spawn
x mob "tree": doesn't do contact damage to player
x mob "tree": cross is green
x mob "tree": consumable pickupable that heals on pickup
x mob "tree": also has a chance to drop a crate instead of consumable
x mob "tree": picking up crate shows up "item pickup" screen before upgrades
x item pickup screen: draw it
x item pickup screen: pickup button
x item pickup screen: refund button
x mob "tree": show crate in UI top right
x melee weapon doesn't change rotation after starting shooting
- melee weapon detaches from player then lerps to player's hand?
  (player's movement doesn't affect detached weapon)
x increase xp on picking up coins
x «level up» number
x upgrade random stat on level up
x shop: show weapon detail upon hovering on weapon
x shop: clicking on weapon makes modal stick
x shop: cancel/recycle buttons work
x shop: set weapon recycling price
x remove playerWeaponsCount
x shop: implement recycling (remove weapon)
x shop: implement combining (remove weapon)
x shop: sticked weapon detail darkens UI behind it
  (through custom element attached to parent of frame)
x reset weapons shooting times & cooldowns upon starting a new wave
x finishing wave damages enemies to death + the don't drop pickupables
x fix melee weapons flying outside the world at the end of the wave
x ref stoopid `g.run.a`
2025-09-15
x don't create creature pre spawns right before finishing the wave
x wave completed: "WAVE COMPLETED" animation with mobs dying
  before entering upgrades / item pickup screens
x mob that spawns others on death
  (be sure not to make pre spawns if wave ends shortly)
x correct regen formula
x stat effect: DAMAGE
x stat effect: DAMAGE_MELEE
x stat effect: DAMAGE_RANGED
x stat effect: SPEED
x stat effect: ATTACK_SPEED
x stat effect: CRIT_CHANCE
x stat effect: DODGE
x stats frame
2025-09-16
x elemental damage type + stat effect: DAMAGE_ELEMENTAL
x INTERNET stat effect: RANGE: how does it work?
x stat effect: RANGE
x show `%` in upgrades of stats that are %
x stat effect: HARVESTING
x idle / run animations
2025-09-17
x ranged mob that shoots and backs up if player is too close
x mob that rushes

* fix player death wave lost
* stat effect: LUCK
  100 Luck doubles the chances of Fruit to drop from enemies, while -100 Luck means none drop at all
  (except guaranteed Crates from Loot Aliens, Elites, and Bosses).
  100 Luck also doubles the chances of turning Fruit into a Loot Crate
  — but because it also doubles the chance of Fruit, it effectively quadruples
  your chances of getting Crates from enemies
  (but only doubles it from Trees, as they have 100% base Fruit chance).
  Formula for loot crate drop chance:
  `Enemy_Box_Drop_Chance * (100% + Luck) / (1 + Box_Spawned_This_Wave)`
* spread mobs apart (use force?)
* mob that rotates when moving?
* more mobs: default mob - стандартная с шоколадными крошками
* more mobs: ranged mob стреляет шоколадными крошками
* more mobs: heavy rusher mob
* more mobs: heavy spawner mob - толстая шрек печенька
* more mobs: tiny mob
* more mobs: mob boss - галет (он сухой)
* more mobs: tree - cow / apple tree
*
*
*
* correct xp progression
* mobs armor characteristic
* mobs damage progression
* mob that shoots
* mob that dashes
* mobs can't spawn near edges.
  consider spawning them with padding = radius of collider
  + make the same thing with mobs that spawn others on death
* stat effect: RANGE
  10  range +0.01s striking weapon shooting duration
  100 range +0.05s striking weapon shooting duration
* secondary stats:
    Consumable Heal
        Increases or decreases the healing done by consumables.
    % Materials Healing
        Chance of being healed by 1 when picking up Materials. Currently only used by Cute Monkey.
    % XP Gain
        Increases or decreases all forms of Experience you earn.
    % Pickup Range
        Increases or decreases the range at which you pick up materials and consumables.
    % Items Price
        Increases or decreases Shop Prices.
    % Explosion Damage
        Increases or decreases the damage of Explosions
    % Explosion Size
        Increases or decreases the size of Explosions
    Bounces
        Gives projectiles additional Bounces
    Piercing
        Gives projectiles additional Piercing
    % Piercing Damage
        Increases or decreases the piercing damage falloff for projectile weapons.
    % Damage against Bosses
        Multiplies the damage you do against Bosses and Elites.
    % Structure Attack Speed
        Increases or decreases the rate of turrets attacking, Landmines spawning, and Gardens spawning Consumables.
    Structure Range
        Increases or decreases the range of Structures.
    % Burning Speed
        Increases or decreases the rate of which Burn ticks for damage.
    Burning Spread
        Increases the amount of times Burn spreads.
    Knockback
        Increases or decreases the distance enemies are knockbacked back when hit.
    % Double Material Chance
        Increases the chance of gaining double materials when picking up Materials
    Free Rerolls
        Provides an amount of free reroll at the beginning of each Shop
    Trees
        Increases the amount of Trees that spawn. See the Wiki page for details.
    % Enemies
        Increases or decreases the amount of Enemies that spawn during waves.
    % Enemy Speed
        Increases or decreases the speed of Enemies that spawn during waves.
    % Reroll Price
        Increases or decreases the price of rerolling in the [[Shop}]] and when choosing Upgrades
* stat effect: ARMOR
  % of damage received after being reduced by positive armor = 1 / ( 1 + ( ARMOR / 15 ) )
  % of damage received after being increased by negative armor = (15 - 2 * ARMOR) / (15 - ARMOR)
  Tooltip for Damage Reduction % = ROUND( ( 1 - ( DmgReceived% ) ) * 100 )
  Rounding:
    The Tooltip for the Damage Reduction from Armor rounds to the nearest whole %.
    However, the actual damage reduction the game uses does not round, and instead uses the precise number.
    Also note that enemy damage is rounded before armor is calculated.
    So if an enemy would deal 1.6 damage, that is rounded up to 2 damage, then armor is applied to the 2 damage.
    And then that number is rounded. So you need 6+ armor to reduce 2 damage down to 1,
    because you need to reduce it to under 1.5 damage for it to round down.
* stat effect: CURSE
    Enemies:
        Cursed enemies drop 33% additional materials on death.
        Cursed enemies have +25% Damage, +15% Speed, and +150% HP +2% * Curse stat.
        Enemy cursed chance = 50 * Curse / (50 + Curse)
    Items:
        Shop item curse chance = 15 * Curse / (50 + Curse)
        Your items and weapons may become cursed and get some enhancement. The enhancements (green texts) of an item will be multiplied by a randomed Curse Ratio, which is ranged from 1.1 ~ 2.1, and enhanced by wave. When you reach wave 20+, the Curse Ratio will reach the highest range from 1.5 ~ 2.1. More precisely:
        Curse Ratio = 1.4 + random_range(-0.3, 0.3) + 0.02 * min(current_wave,20)
        The downsides (red texts) of the item will be divided by the Curse Ratio. The neutral effects (white texts) such as +5% Enemies in Gentle Alien will not be affected by Curse.
        Every single effects (enhancements/downsides) of an item has independant random Curse Ratio.
        Some cursed enhancements will round to integer. For example, you may get +3 Dangerous Bunny, +2 Tardigrade or +5%  Retromation's Hoodie if the Curse Ratio > 2.0.
* stat effect: ENGINEERING
  Engineering increases the Damage and healing of your Structures, such as Turrets and Landmines.
  Engineering also increases the attack damage of the Weapons Plank and Screwdriver.
* engine: automatically scale fonts for current screenSize
* engine: different atlases for different resolutions
* engine: handle the case when there's too much textures to pack into 1 atlas
* wave completed: not picked up coins fly to "stash" during the "WAVE COMPLETED" animation
* wave completed: stashed coins drop during the next wave with x2 value
* «upgraded stat» number (show stat's icon instead of text) upon levelup
*
*
* ZHEKA: ^ vs code /// collapsing
* ZHEKA: ^ knows how to work with UI
* ZHEKA: can web build
* ZHEKA: total commander for working with sounds?
  instructions of how to work with sounds
  - finding them using total commander (alt+F7)
  - sending them for listening to foobar2000
    (total commander: «Feed to listbox» then drag'n drop)
  - fast editing with audacity (fade in, fade out, compression, export macroses)
* show melee weapon collider gizmos
* weapons render z
* ref draw/drawing -> render/rendering (for consistency)
* can mobs drop consumables?
* gather all weapon classes (e.g. primitive)
* primitive weapons bonus (3,6,9,12,15 HP)
* ethereal weapons bonus (3,6,9,12,15 ARMOR or 2,4,X or 1,2,X)
*
*
* aseprite show image center
* upgrades screen: fix width of upgrade's frame
* upgrades screen:
* upgrades screen:
* upgrades screen:
* upgrades screen:
* upgrades screen:
* combining weapons
* better mob spawn rate (ref brotato)
* item effects
* weapons categorization. жидкое / твёрдое?
* HTML game loading screen?
* record+replay system
* ranged mobs
* internet shouldn't be required for building:
  check web debug yandex build with mocked ysdk
* trim pngs of transparent pixels
