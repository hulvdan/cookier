Привет, Жень
Думаю, было бы клёво вживую подвести итоги по игре и определить, что остаётся для её релиза в плане кода, гейдизайна. Почти месяц разработки накапал.
Что думаешь?


Если будет время накинуть тем перед встречей, полагаю, можешь заранее подготовить их.


Ниже - для информации.

Предварительно по темам:

GAMEDESIGN:
- Типы эффектов оружий
  - "Инженерка". Стоит реализовывать туррели / мины?
- Мб кастомные projectile-ы? Что у них было бы по эффектам?
  - Ускорение
  - Замедление
- Эффекты предметов
- Мобы
  - доп мобы (хилер)
  - доп механики мобам (в brotato некоторые мобы "блуждают" всё время или пока не заагрятся на игрока)
  - как будет работать босс

VISUAL:
- Как будет выглядеть начало игры (т.е. катсцена -> переход к гейплею). Есть ли что-то по коду, что можно начать делать

PRODUCTION:
- яндекс
  - сохранение во время игры
  - прикручивание аналитики, воронки
  - проверить скалирование на ВСЕХ устройствах / топ браузерах
-
-


PS шо сделал

```
GAMEPLAY
x melee атаки оружиями игрока исходят из центра игрока
x fix: melee оружия гарантированно нанесут урон даже при запределеных значениях RANGE / ATTACK SPEED
x можно указать, чтобы некоторые мобы спавнились группами
x враг-босс. его убийство сразу завершает последнюю волну
x boss HP bar
x моб-туррель из Brotato (дружественна к игроку)
  + не горит (мобам можно указать сопротивляемость ailment-ам (горение, молниевое замедление))
  + игрок и враги ей не наносят урон, а сама - таргетит врагов, стреляет, убивает
  (правильный её спавн через предметы не делал)

UI
x кнопке можно указать 9slice + color + flash.
x подредачил спрайты UI frame-ов и кнопок
x оружию можно указать иконку
x мелкий шрифт для отображения статов
x оставшиеся до конца волны секунды отображаются, как в Brotato
x победа в последней волне отображает "run completed!", а не wave completed

FIX
x fix: weapons Z оружий, чтобы не "мелькали" в некоторых случаях
x fix padding статов
x фикс расстояния получения контактного урона
x fix: увеличение HP при повышении уровня (выборе апгрейда) отображается в HP bar-е сразу
x fix: удаление всех damage numbers / projectiles на карте при переходе в магазин
```

* turrel
  * proper spawning (via items)
  * can specify range + projectile_type
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
