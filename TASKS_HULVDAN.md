
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
