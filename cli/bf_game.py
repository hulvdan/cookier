"""
USAGE:

    from bf_lib import data_values, gamelib_processor

    data_values.itch_target = "hulvdan/cult-boy"
    data_values.languages = ["russian", "english"]
    data_values.generate_flatbuffers_api_for = ["bf_save.fbs"]

    @gamelib_processor
    def _process_gamelib(_genline, gamelib, _localization_codepoints: set[int]) -> None:
        for tile in gamelib["tiles"]:
            tile.pop("type")
"""

from itertools import groupby
from typing import Any

import bf_image
import bf_swatch
import yaml
from bf_lib import (
    ART_TEXTURES_DIR,
    GAME_DIR,
    SRC_DIR,
    data_values,
    gamelib_processor,
    genenum,
    hex_to_rgb_floats,
    hex_to_rgb_ints,
    recursive_replace_transform,
    rgb_floats_to_hex,
    transform_color,
)
from bf_typer import command, log, timing

data_values.itch_target = "hulvdan/cookier"
data_values.languages = ["russian", "english"]
data_values.generate_flatbuffers_api_for = ["bf_save.fbs"]


def _check_duplicates(values: list) -> None:
    for i in range(len(values)):
        for k in range(i + 1, len(values)):
            assert values[i] != values[k], f"Found duplicate value: {values[i]}"


def field_to_list(container, field: str) -> None:
    if field not in container:
        return
    if isinstance(container[field], list):
        return
    if isinstance(container[field], str):
        try:
            container[field] = [int(v) for v in container[field].split(" ")]
        except ValueError:
            container[field] = [float(v) for v in container[field].split(" ")]
    elif isinstance(container[field], (int, float)):
        container[field] = [container[field]]
    else:
        assert False


def does_require(effect_condition_name: str, v: str) -> bool:
    requires = False
    if effect_condition_name.startswith(f"{v}__"):
        requires = True
    if effect_condition_name.endswith(f"__{v}"):
        requires = True
    if f"__{v}__" in effect_condition_name:
        requires = True
    return requires


scoped_processing_args = ["None", "None"]


@gamelib_processor
def _process_gamelib(genline, gamelib, localization_codepoints: set[int]) -> None:
    try:
        __process_gamelib(genline, gamelib, localization_codepoints)
    except Exception:
        print("ERROR HAPPENED DURING PROCESSING:", ", ".join(scoped_processing_args))
        raise


def __process_gamelib(genline, gamelib, localization_codepoints: set[int]) -> None:
    def enumerate_table(field: str):
        scoped_processing_args[0] = field

        for i, x in enumerate(gamelib[field]):
            scoped_processing_args[1] = x["type"]
            yield i, x

        scoped_processing_args[0] = "None"
        scoped_processing_args[1] = "None"

    transforms: list[tuple[str, str, dict[str, int]]] = []

    # Pickupables.
    # ============================================================
    for i, x in enumerate_table("pickupables"):
        if i > 0:
            x["name_locale"] = "PICKUPABLE_{}".format(x["type"])

    # Effect conditions.
    # ============================================================
    EFFECT_CONDITION_LETTERS = ("x", "y", "z", "w")

    if 1:
        for _, x in enumerate_table("effect_conditions"):
            x["name_locale"] = "EFFECT_{}".format(x["type"])

        for x in gamelib.pop("build_effect_conditions", []):
            x["name_locale"] = "BUILD_EFFECT_{}".format(x["type"])
            gamelib["effect_conditions"].append({**x, "restrict": 2})

        for x in gamelib.pop("weapon_effect_conditions", []):
            x["name_locale"] = "WEAPON_EFFECT_{}".format(x["type"])
            gamelib["effect_conditions"].append({**x, "restrict": 1})

        for _, x in enumerate_table("effect_conditions"):
            for cond in (
                *(l.upper() for l in EFFECT_CONDITION_LETTERS),
                "STAT",
                "PROPERTY",
                "PICKUPABLE",
                # "TIER",
                # "ITEM",
                # "WEAPON",
                "ITEM_OR_WEAPON",
                # "WEAPONGROUP",
            ):
                if does_require(x["type"], cond):
                    x["requires_{}".format(cond.lower())] = True

            placeholders: list[Any] = []
            x["placeholders"] = placeholders
            previous_is_required = True
            for letter in EFFECT_CONDITION_LETTERS:
                required = x.pop(f"requires_{letter}", False)
                if required:
                    assert previous_is_required
                else:
                    previous_is_required = False

                placeholders.append(
                    {
                        "required": required,
                        "condvar_type": x.pop(f"{letter}_condvar_type", "INTEGER"),
                        "signed_": x.pop(f"{letter}_signed", False),
                        "is_percent": x.pop(f"{letter}_is_percent", False),
                        "multiplied_by_times": x.pop(
                            f"{letter}_multiplied_by_times", False
                        ),
                        "divided_by_times": x.pop(f"{letter}_divided_by_times", False),
                    }
                )

    def process_effects_of(x, required_tier_values: int) -> None:
        for e in x.get("effects", []):
            field_to_list(e, "value")
            field_to_list(e, "value_multiplier")
            for letter in EFFECT_CONDITION_LETTERS:
                field_to_list(e, f"condition_{letter}")
            for vv in e.get("damage_scalings", []):
                field_to_list(vv, "percents_per_tier")

            if "value" in e:
                assert "value_multiplier" not in e, x["type"]
            if "value_multiplier" in e:
                assert "value" not in e, x["type"]

            if "value" in e:
                assert len(e["value"]) == required_tier_values, x["type"]
            if "value_multiplier" in e:
                assert len(e["value_multiplier"]) == required_tier_values, x["type"]

            for v in e.get("placeholders", []):
                field_to_list(v, "ints")
                field_to_list(v, "floats")

    # Items.
    # ============================================================
    TOTAL_TIERS = 4

    items_per_tier = [0] * TOTAL_TIERS

    for i, x in enumerate_table("items"):
        if i > 0:
            process_effects_of(x, 1)

            x["name_locale"] = "ITEM_" + x["type"].upper()
            assert 0 <= x["tier"] < 4

            mandatory_fields = [
                "price",
            ]
            for field in mandatory_fields:
                assert field in x, "Item {} has to have '{}' specified".format(
                    x["type"], field
                )

            items_per_tier[x.get("tier", 0)] += 1

    genline(
        "constexpr int ITEMS_PER_TIER_[] {{{}}};".format(
            ", ".join(str(x) for x in items_per_tier)
        )
    )
    genline("VIEW_FROM_ARRAY_DANGER(ITEMS_PER_TIER);\n")

    # Difficulties.
    # ============================================================
    for i, x in enumerate_table("difficulties"):
        if i > 0:
            process_effects_of(x, 1)
            x["name_locale"] = f"DIFFICULTY_{i}"

    # Weapons.
    # ============================================================
    MAX_WEAPON_TIER = 4

    for i, x in enumerate_table("weapons"):
        if i > 0:
            x["name_locale"] = "WEAPON_" + x["type"].upper()

            required_tier_values = MAX_WEAPON_TIER - x["min_tier_index"]
            assert required_tier_values > 0
            assert required_tier_values <= MAX_WEAPON_TIER

            mandatory_fields = [
                "price",
                "min_tier_index",
                "base_damage",
                "damage_scalings",
                "icon_texture_id",
                "texture_ids",
            ]
            for field in mandatory_fields:
                assert field in x, "Weapon {} has to have '{}' specified".format(
                    x["type"], field
                )

            field_to_list(x, "life_steal_percents")
            field_to_list(x, "base_damage")
            field_to_list(x, "projectile_spawn_frames")
            for vv in x["damage_scalings"]:
                field_to_list(vv, "percents_per_tier")

            for f in ("base_damage", "life_steal_percents"):
                value = x.get(f)
                if value is None:
                    continue

                assert len(value) == required_tier_values, (
                    "Weapon {} must have {} `{}` because it's `min_tier_index` is {}".format(
                        x["type"], required_tier_values, f, x["min_tier_index"]
                    )
                )

            for scalings in x["damage_scalings"]:
                percents_count = len(scalings["percents_per_tier"])
                assert percents_count == required_tier_values, (
                    "Weapon {} must have {} `percents_per_tier` because it's `min_tier_index` is {}".format(
                        x["type"], required_tier_values, x["min_tier_index"]
                    )
                )

            process_effects_of(x, required_tier_values)

    # Stats.
    # ============================================================
    for i, x in enumerate_table("stats"):
        if i >= 1:
            x["name_locale"] = "STAT_" + x["type"].upper()
            icon_texture = x.pop("icon_texture_id", None)
            if icon_texture is not None:
                x["big_icon_texture_id"] = f"{icon_texture}_big"
                if x["type"] != "COINS":
                    x["small_icon_texture_id"] = f"{icon_texture}_small"

        if i >= 3 and not x.get("is_secondary"):
            x["upgrade_name_locale"] = "UPGRADE_NAME_" + x["type"].upper()

    # Creatures.
    # ============================================================
    for i, x in enumerate_table("creatures"):
        if i >= 2:
            mob_mandatory_fields = [
                "spawn_factor",
                "spawn_factor",
                "health",
                "health_increase_per_wave",
                "contact_damage",
                "contact_damage_increase_per_wave",
                "knockback_resistance",
            ]
            if x.get("hostility_type", "MOB") == "MOB":
                for field in mob_mandatory_fields:
                    assert field in x, "Mob {} needs `{}` specified".format(
                        x["type"], field
                    )

                assert 0 <= x["knockback_resistance"] <= 1, (
                    "Mob {} `knockback_resistance` must be in range [0; 1]".format(
                        x["type"]
                    )
                )

    # Placeholders.
    # ============================================================
    if 1:
        genline("// Placeholders. {  ///")
        genline("struct Placeholder;")
        params = "const Placeholder* placeholder"
        genline("using ClayPlaceholderFunction = void(*)({});".format(params))
        for i, x in enumerate(gamelib["placeholders"]):
            if i > 0:
                genline("void ClayPlaceholderFunction_{}({});".format(x["type"], params))
        genline("ClayPlaceholderFunction clayPlaceholderFunctions_[]{")
        for i, x in enumerate(gamelib["placeholders"]):
            if i > 0:
                genline("  ClayPlaceholderFunction_{},".format(x["type"]))
        genline("};")
        genline("VIEW_FROM_ARRAY_DANGER(clayPlaceholderFunctions);")
        genline("// }")
        genline("")

    # Builds.
    # ============================================================
    if 1:
        max_weapons = 0
        for i, x in enumerate_table("builds"):
            process_effects_of(x, 1)
            if i > 0:
                x["name_locale"] = "BUILD_{}".format(x["type"])
                max_weapons = max(max_weapons, len(x["starting_weapon_types"]))
        genline("constexpr int MAX_BUILD_WEAPONS = {};\n".format(max_weapons))

    # Achievements.
    # ============================================================
    if 1:
        unlocked_builds: list[str] = []
        unlocked_items: list[str] = []
        unlocked_weapons: list[str] = []

        for i, x in enumerate_table("achievements"):
            if i > 0:
                x["name_locale"] = f"ACHIEVEMENT_NAME_{x['type']}"
                x["description_locale"] = f"ACHIEVEMENT_DESCRIPTION_{x['type']}"

                mandatory_fields = [
                    "steps",
                ]
                for field in mandatory_fields:
                    assert field in x, "Achievement {} has to have '{}' specified".format(
                        x["type"], field
                    )
                for stepIndex, step in enumerate(x["steps"]):
                    assert step["value"] > 0

                    unlock_fields = {
                        "unlocks_build_type": unlocked_builds,
                        "unlocks_weapon_type": unlocked_items,
                        "unlocks_item_type": unlocked_weapons,
                    }
                    c = sum(f in step for f in unlock_fields)
                    assert c <= 1
                    if c != 1:
                        log.warning(
                            "Achievement {}, stepIndex {} (value {}), should have either {} specified".format(
                                x["type"],
                                stepIndex,
                                step["value"],
                                " or ".join(f"`{f}`" for f in unlock_fields),
                            )
                        )

                    for field, container in unlock_fields.items():
                        if field in step:
                            v = step[field]
                            assert v not in container, "{} {} duplicated".format(field, v)
                            container.append(v)

    with open(SRC_DIR / "game" / "bf_gamelib.fbs", encoding="utf-8") as in_file:
        gamelib_fbs_lines = [l.strip() for l in in_file if l.strip()]

    # Props.
    # ============================================================
    if 1:
        for biome in gamelib["biomes"][1:]:
            prop_texture_ids: list[str] = []
            biome["prop_texture_ids"] = prop_texture_ids
            for file in ART_TEXTURES_DIR.glob(
                "game_prop_*_{}.png".format(biome["type"].lower())
            ):
                prop_texture_ids.append(file.stem)

        groups = [70, 52, 20]
        offsetScales = [0, 1, 1]
        assert len(offsetScales) == len(groups)

        genline(
            "constexpr int PROP_GROUP_COUNTS_[]{{{}}};".format(
                ", ".join(str(x) for x in groups)
            )
        )
        genline(
            "constexpr f32 PROP_GROUP_OFFSET_SCALES_[]{{{}}};".format(
                ", ".join(str(x) for x in offsetScales)
            )
        )
        genline("VIEW_FROM_ARRAY_DANGER(PROP_GROUP_COUNTS);")
        genline("VIEW_FROM_ARRAY_DANGER(PROP_GROUP_OFFSET_SCALES);")
        genline(
            "constexpr int PROPS_COUNT = {};\n".format(
                sum(i * g for i, g in enumerate(groups, 1))
            )
        )

    # Texture bind.
    # ============================================================
    if 1:
        start = -1
        end = -1
        for i, line in enumerate(gamelib_fbs_lines):
            if "AUTOMATIC_TEXTURE_BIND_START" in line:
                assert start == -1
                start = i + 1
            if "AUTOMATIC_TEXTURE_BIND_END" in line:
                assert end == -1
                end = i
                break
        assert start >= 0
        assert end > start

        for i in range(start, end):
            field = gamelib_fbs_lines[i].split(":", 1)[0]
            gamelib[field] = field.split("_texture_id", 1)[0]

    # Tables.
    # ============================================================
    if 1:
        start = -1
        end = -1
        for i, line in enumerate(gamelib_fbs_lines):
            if "TABLE_GEN_START" in line:
                start = i + 1
            if "TABLE_GEN_END" in line:
                end = i
                break
        assert start >= 0
        assert end > start

        # tuples of field_name + type_name
        # ex: [("hostilities", "Hostility")]
        table_transform_data: list[tuple[str, str]] = []
        for i in range(start, end):
            line = gamelib_fbs_lines[i]  # ex: `    hostilities: [Hostility] (required);`
            field_name = line.split(":", 1)[0].strip()  # ex: `hostilities`
            type_name = line.split("[", 1)[-1].split("]", 1)[0].strip()  # ex: `Hostility`
            table_transform_data.append((field_name, type_name))

        for field_name, type_name in table_transform_data:
            types = [x.pop("type") for x in gamelib[field_name]]
            for t in types:
                assert t.upper() == t, (
                    "{}. {}. `type` fields must be in CONSTANT_CASE".format(field_name, t)
                )
            _check_duplicates(types)
            genenum(genline, type_name + "Type", types, add_count=True)
            transforms.append(
                (
                    f"{type_name.lower()}_type",
                    f"{type_name.lower()}_types",
                    {v: i for i, v in enumerate(types)},
                )
            )

    # Codepoints.
    # ============================================================
    if 1:
        ranges = [
            (ord(" "), ord(" ") + 1),  # Space character
            (33, 127),  # ASCII
            (1040, 1104),  # Cyrillic
        ]

        for r in ranges:
            localization_codepoints.update(set(range(r[0], r[1])))

        codepoints_with_groups: list[tuple[int, int]] = []
        for c in sorted(localization_codepoints):
            codepoints_with_groups.append((c // 10, c))

        genline("int g_codepoints[] {  ///")
        for _, group in groupby(codepoints_with_groups, lambda x: x[0]):
            g = list(group)
            genline(
                "  {},  // {}.".format(
                    ", ".join(str(i[1]) for i in g),
                    ", ".join(chr(i[1]) for i in g),
                )
            )
        genline("};\n")

    # Transforms.
    # ============================================================
    for v in transforms:
        recursive_replace_transform(gamelib, *v)


@command
@timing
def process_images():
    # Outlining ui icons.
    bf_image.conveyor(
        "to_outline",
        "outlining",
        bf_image.conveyor_outline(stroke_size=8, color=(0, 0, 0, 255), is_shadow=False),
    )

    # Extracting white and black of floor sprites.
    for f in ART_TEXTURES_DIR.glob("game_floor_*.png"):
        f.unlink()
    bf_image.conveyor(
        "to_split",
        "extract white",
        bf_image.conveyor_extract_white(),
        bf_image.conveyor_suffix("front"),
    )
    bf_image.conveyor(
        "to_split",
        "extract_black",
        bf_image.conveyor_extract_black(),
        bf_image.conveyor_suffix("back"),
    )

    # Biomefying props.
    for f in ART_TEXTURES_DIR.glob("game_prop_*.png"):
        f.unlink()
    gamelib = yaml.safe_load((GAME_DIR / "gamelib.yaml").read_text(encoding="utf-8"))
    get_color = lambda biome, x: hex_to_rgb_ints(hex(biome[x])[2:-2])
    for biome in gamelib["biomes"][1:]:
        t = biome["type"]
        bf_image.conveyor(
            "to_biome",
            f"{t}: remapping",
            bf_image.conveyor_remap(
                get_color(biome, "outline_color"), get_color(biome, "fill_color")
            ),
            bf_image.conveyor_suffix(t.lower()),
        )

    # Transforming stat icons into big and small.
    for f in ART_TEXTURES_DIR.glob("ui_stat_icon_*.png"):
        f.unlink()
    bf_image.conveyor("stat_icons", "copying big", bf_image.conveyor_suffix("big"))
    bf_image.conveyor(
        "stat_icons",
        "downscaling small",
        bf_image.conveyor_scale(0.5),
        bf_image.conveyor_outline(stroke_size=1, color=(0, 0, 0, 255), is_shadow=False),
        bf_image.conveyor_suffix("small"),
    )


@command
def make_swatch():
    # result = bf_swatch.parse("c:/Users/user/Downloads/woodspark.ase")
    # print(result)
    colors = [
        "#ffffff",
        "#2c4941",
        "#66a650",
        "#b9d850",
        "#82dcd7",
        "#208cb2",
        "#253348",
        "#1d1b24",
        "#3a3a41",
        "#7a7576",
        "#b59a66",
        "#cec7b1",
        "#edefe2",
        "#d78b98",
        "#a13d77",
        "#6d2047",
        "#3c1c43",
        "#2c2228",
        "#5e3735",
        "#885a44",
        "#b8560f",
        "#dc9824",
        "#efcb84",
        "#e68556",
        "#c02931",
        "#000000",
    ]

    new_colors = ["#ffffff", "#000000"]

    for i in range(len(colors)):
        color = colors[i]
        if color in ("#000000", "#ffffff"):
            continue
        c = rgb_floats_to_hex(
            transform_color(
                hex_to_rgb_floats(color),
                saturation_scale=1.2,
                value_scale=0.52,
            )
        )
        new_colors.append(color)
        new_colors.append(c)
        colors.append(c)

    def process_color(color: str) -> dict:
        return {
            "name": color,
            "type": "Global",
            "data": {
                "mode": "RGB",
                "values": hex_to_rgb_floats(color),
            },
        }

    swatch_data = [process_color(c) for c in colors]
    bf_swatch.write(swatch_data, "aboba.ase")

    def process_color2(color: str) -> bf_swatch.RawColor:
        r, g, b = hex_to_rgb_ints(color)
        r = int(r * 65535 / 255)
        g = int(g * 65535 / 255)
        b = int(b * 65535 / 255)
        assert r < 65536, r
        assert g < 65536, g
        assert b < 65536, b
        assert r >= 0, r
        assert g >= 0, g
        assert b >= 0, b

        return bf_swatch.RawColor(
            name=color,
            color_space=bf_swatch.ColorSpace.RGB,
            component_1=r,
            component_2=g,
            component_3=b,
            component_4=65535,
        )

    with open("aboba.aco", "wb") as out_file:
        bf_swatch.save_aco_file([process_color2(c) for c in new_colors], out_file)

    with open("aboba.pal", "w") as out_file:
        out_file.write(
            "JASC-PAL\n0100\n{}\n".format(
                len(new_colors),
            )
        )
        color_lines = []
        for color in new_colors:
            r, g, b = hex_to_rgb_ints(color)
            color_lines.append(f"{r} {g} {b}")
        color_lines = color_lines[2:] + color_lines[:2]
        out_file.write("\n".join(color_lines))
