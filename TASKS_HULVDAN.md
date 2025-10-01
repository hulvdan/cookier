x fix скалирования выдачи монет копилкой
x max dodge 60%
x Stat RANGE экспоненциально делит дальность (-50 = 1/2 дальности, -100 1/4, -150 = 1/8 etc)
x колесо мыши для читинга (увеличение/уменьшение статов, кол-ва предметов)
x fix StatType_ITEMS_PRICE, StatType_REROLL_PRICE
x weapon effects: давание +1 скорости атаки при убийстве 20 врагов этим оружием
x покупка предмета, что даёт StatType_FREE_REROLLS сразу разрешает бесплатное перебрасывание в магазе
x fix: если игра тормозит, не симулируем слишком много фреймов
x больше оружий для разработки
  + огне/молние палки для элементального урона
  + рогатка для "bouncing" механики
x горение минимально работает
  (нужно сделать spread + tick-rate)
x projectile-ы "правильно смотрят" в направлении их полёта
x bouncing предсказывает позицию врага
x shader: "flashing"
  перевод всех пикселей спрайта, к примеру,
  целиком в белый цвет, как в brotato при нанесении урона врагу
x замедление врагов при использовании молние-палки
x можно указать доп. lifesteal оружию, отображение его в карточке
x можно ховерить на оружие в экране завершения
x взрывная пуля + "частица" взрыва
  (нужно прикрутить explosion size + explosion damage статы)
x клёвое форматирование / перенос слов в своём flexbox-е
x отображение инфы по взрыву в карточке оружия
x fix данные оружия: поверх неё них не рисуются теперь иконки статов из списка статов
x ref clay use floating.offset
x чем ближе моб спавнится к игроку, тем меньше вероятность его спавна (в опред радиусе)
x распространение горения
x stats: % Explosion Damage + Size
x StatType_ATTACK_SPEED correct scaling
x ref FrameGame / FrameVisual. Gameplay must use only FrameGame
x pause works

* stats: % Burning Speed: Increases or decreases the rate of which Burn ticks for damage.
* not giving stat on buying item if it has condition
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
*
*
* wave completed: not picked up coins fly to "stash" during the "WAVE COMPLETED" animation
* wave completed: stashed coins drop during the next wave with x2 value
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
* weapons DrawZ
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
* StatType_ENEMY_SPEED
* StatType_XP_GAIN
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
* engine: better fonts rendering
* engine: automatically scale fonts for current screenSize
* engine: different atlases for different resolutions
* engine: handle the case when there's too much textures to pack into 1 atlas
* HTML game loading screen?
* record+replay system
* trim pngs of transparent pixels
* internet shouldn't be required for building:
  check web debug yandex build with mocked ysdk
