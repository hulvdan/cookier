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
x stat effect: ARMOR
x stat effect: DODGE

* stat effect: RANGE
* stat effect: CURSE
* stat effect: DAMAGE_ELEMENTAL
* stat effect: ENGINEERING
* stat effect: LUCK
* stat effect: HARVESTING
* show % in names of stats that are %
* wave completed: not picked up coins fly to "stash" during the "WAVE COMPLETED" animation
* wave completed: stashed coins drop during the next wave with x2 value
* «upgraded stat» number (show stat's icon instead of text) upon levelup
* mobs can't spawn near edges.
  consider spawning them with padding = radius of collider
  + make the same thing with mobs that spawn others on death
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
* upgrades screen: frame
* upgrades screen:
* upgrades screen:
* upgrades screen:
* upgrades screen:
* upgrades screen:
* combining weapons
* better mob spawn rate (ref brotato)
* stats frame
* item effects
* weapons categorization. жидкое / твёрдое?
* Экран загрузки для HTML?
* система записи / воспроизведения действий для облегчения отладки
* ranged mobs
* internet shouldn't be required for building:
  check web debug yandex build with mocked ysdk
* trim pngs of transparent pixels
