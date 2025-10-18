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

from bf_lib import (
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
    if isinstance(container[field], str):
        try:
            container[field] = [int(v) for v in container[field].split(" ")]
        except ValueError:
            container[field] = [float(v) for v in container[field].split(" ")]
    elif isinstance(container[field], (int, float)):
        container[field] = [container[field]]
    else:
        assert False


@gamelib_processor
def _process_gamelib(genline, gamelib, localization_codepoints: set[int]) -> None:
    transforms: list[tuple[str, str, dict[str, int]]] = []

    # Effect conditions.
    # ============================================================
    if 1:
        for x in gamelib.pop("weapon_effect_conditions", []):
            gamelib["effect_conditions"].append({**x, "restrict": 1})

        genline("// Effect conditions. {  ///")
        params = "const BFGame::Effect* fb_effect, int tierOffset"
        genline("using ClayEffectConditionFunction = void(*)({});".format(params))
        for x in gamelib["effect_conditions"][1:]:
            genline("void ClayEffectCondition_{}({});".format(x["type"], params))
        genline("constexpr ClayEffectConditionFunction clayEffectConditionFunctions_[]{")
        for x in gamelib["effect_conditions"][1:]:
            genline("  ClayEffectCondition_{},".format(x["type"]))
        genline("};")
        genline("VIEW_FROM_ARRAY_DANGER(clayEffectConditionFunctions);")
        genline("// }")
        genline("")

    # Items.
    # ============================================================
    for x in gamelib["items"][1:]:
        for e in x.get("effects", []):
            field_to_list(e, "value")
            field_to_list(e, "value_multiplier")
            field_to_list(e, "condition_value")

        x["name_locale"] = "ITEM_" + x["type"].upper()
        assert 0 <= x["tier"] < 4

        mandatory_fields = [
            "price",
        ]
        for field in mandatory_fields:
            assert field in x, "Item {} has to have '{}' specified".format(
                x["type"], field
            )

    # Difficulties.
    # ============================================================
    for i, x in enumerate(gamelib["difficulties"]):
        if i > 0:
            for e in x.get("effects", []):
                field_to_list(e, "value")
                field_to_list(e, "value_multiplier")
                field_to_list(e, "condition_value")

            x["name_locale"] = f"DIFFICULTY_{i}"

    # Weapons.
    # ============================================================
    MAX_WEAPON_TIER = 4

    for x in gamelib["weapons"][1:]:
        x["name_locale"] = "WEAPON_" + x["type"].upper()

        requiredTierValues = MAX_WEAPON_TIER - x["min_tier_index"]
        assert requiredTierValues > 0
        assert requiredTierValues <= MAX_WEAPON_TIER

        mandatory_fields = [
            "max_price",
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

        field_to_list(x, "base_damage")
        field_to_list(x, "projectile_spawn_frames")
        for vv in x["damage_scalings"]:
            field_to_list(vv, "percents_per_tier")

        assert len(x["base_damage"]) == requiredTierValues, (
            "Weapon {} must have {} `base_damage` because it's `min_tier_index` is {}".format(
                x["type"], requiredTierValues, x["min_tier_index"]
            )
        )

        for scalings in x["damage_scalings"]:
            percents_count = len(scalings["percents_per_tier"])
            assert percents_count == requiredTierValues, (
                "Weapon {} must have {} `percents_per_tier` because it's `min_tier_index` is {}".format(
                    x["type"], requiredTierValues, x["min_tier_index"]
                )
            )

        for e in x.get("effects", []):
            field_to_list(e, "value")
            field_to_list(e, "value_multiplier")
            field_to_list(e, "condition_value")

            if "value" in e:
                assert "value_multiplier" not in e, x["type"]
            if "value_multiplier" in e:
                assert "value" not in e, x["type"]
            if "effectcondition_type" not in e:
                assert "condition_value" not in e, x["type"]

            if "value" in e:
                assert len(e["value"]) == requiredTierValues, x["type"]
            if "value_multiplier" in e:
                assert len(e["value_multiplier"]) == requiredTierValues, x["type"]
            if "condition_value" in e:
                assert len(e["condition_value"]) == requiredTierValues, x["type"]

    # Stats.
    # ============================================================
    for i, x in enumerate(gamelib["stats"]):
        if i >= 1:
            x["name_locale"] = "STAT_" + x["type"].upper()
        if i >= 3 and not x.get("is_secondary"):
            x["upgrade_name_locale"] = "UPGRADE_NAME_" + x["type"].upper()

    # Creatures.
    # ============================================================
    for i, x in enumerate(gamelib["creatures"]):
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
        for i, x in enumerate(gamelib["builds"]):
            for e in x.get("effects", []):
                field_to_list(e, "value")
                field_to_list(e, "value_multiplier")
                field_to_list(e, "condition_value")

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

        for i, x in enumerate(gamelib["achievements"]):
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

    # Tables.
    # ============================================================
    if 1:
        with open(SRC_DIR / "game" / "bf_gamelib.fbs", encoding="utf-8") as in_file:
            gamelib_fbs_lines = in_file.readlines()

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
