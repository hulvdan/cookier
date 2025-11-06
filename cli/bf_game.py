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

from bf_lib import (
    ART_TEXTURES_DIR,
    SRC_DIR,
    data_values,
    gamelib_processor,
    genenum,
    log,
    recursive_replace_transform,
)

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
