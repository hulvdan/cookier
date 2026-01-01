"""
USAGE:

    from bf_lib import game_settings, gamelib_processor

    game_settings.itch_target = "hulvdan/cult-boy"
    game_settings.languages = ["russian", "english"]
    game_settings.generate_flatbuffers_api_for = ["bf_save.fbs"]
    game_settings.yandex_metrica_counter_id = 1

    @gamelib_processor
    def process_gamelib(_genline, gamelib, _localization_codepoints: set[int]) -> None:
        for tile in gamelib["tiles"]:
            t = tile["type"]
"""

# Imports.  {  ###
import json
import subprocess
from itertools import groupby
from typing import Any, TypeAlias

import bf_image
import bf_swatch
from bf_lib import (
    ART_DIR,
    ART_TEXTURES_DIR,
    SRC_DIR,
    all_are_none,
    all_are_not_none,
    check_duplicates,
    game_settings,
    gamelib_processor,
    genenum,
    hex_to_rgb_floats,
    hex_to_rgb_ints,
    load_gamelib_cached,
    only_one_is_not_none,
    read_localization_csv,
    recursive_flattenizer,
    recursive_mkdir,
    recursive_replace_transform,
    replace_double_spaces,
    rgb_floats_to_hex,
    transform_color,
)
from bf_typer import command, timing
from PIL import Image, ImageDraw, ImageEnhance, ImageFont
from rich.console import Console
from rich.table import Table

# }

game_settings.itch_target = "hulvdan/cookier"
game_settings.languages = ["russian", "english"]
game_settings.generate_flatbuffers_api_for = ["bf_save.fbs"]
game_settings.yandex_metrica_counter_id = 105874717


ACHIEVEMENTS_X = 8
ACHIEVEMENTS_ORDER_FILEPATH = SRC_DIR / "game" / "gamelib_achievements_order.json"


def field_to_list(container, field: str, ensure_length: int | None = None) -> None:
    # {  ###
    if ensure_length is not None:
        assert ensure_length >= 0

    if field not in container:
        return
    if isinstance(container[field], list):
        return
    if isinstance(container[field], str):
        val = replace_double_spaces(container[field]).strip()
        try:
            container[field] = [int(v) for v in val.split(" ")]
        except ValueError:
            container[field] = [float(v) for v in val.split(" ")]
    elif isinstance(container[field], (int, float)):
        container[field] = [container[field]]
    else:
        assert False

    if ensure_length is not None:
        assert len(container[field]) == ensure_length
    # }


def does_require(effect_condition_name: str, v: str) -> bool:
    # {  ###
    requires = False
    if effect_condition_name.startswith(f"{v}__"):
        requires = True
    if effect_condition_name.endswith(f"__{v}"):
        requires = True
    if f"__{v}__" in effect_condition_name:
        requires = True
    return requires
    # }


scoped_processing_args = ["None", "None"]


@gamelib_processor
def process_gamelib(*args, **kwargs) -> None:
    # {  ###
    try:
        _process_gamelib(*args, **kwargs)
    except Exception:
        print("ERROR HAPPENED DURING PROCESSING:", ", ".join(scoped_processing_args))
        raise
    # }


def explode_achievements(gamelib: dict) -> None:
    # {  ###
    for i, x in enumerate(gamelib["achievements"]):
        if not i:
            continue

        if x["type"].startswith("FINISH_RUN_WITH_BUILD_"):
            build_name = x["type"].removeprefix("FINISH_RUN_WITH_BUILD_")
            x["build_type"] = build_name
            x["name_locale"] = f"BUILD_{build_name}"
            x["description_locale"] = "ACHIEVEMENT_DESCRIPTION_FINISH_RUN_WITH_BUILD"
            x["hide_progress"] = True

            for build in gamelib["builds"]:
                if build["type"] == build_name:
                    break
            assert build
            assert "steps" not in x
            v = {"value": 1}
            for f in ("unlocks_item_type", "unlocks_weapon_type"):
                if build.get(f):
                    v[f] = build.pop(f)
            x["steps"] = [v]
    # }


def _process_gamelib(
    genline, gamelib, localization_codepoints: set[int], warning
) -> None:
    def enumerate_table(field: str):
        # {  ###
        scoped_processing_args[0] = field

        for i, x in enumerate(gamelib[field]):
            scoped_processing_args[1] = x["type"]
            yield i, x

        scoped_processing_args[0] = "None"
        scoped_processing_args[1] = "None"
        # }

    transforms: list[tuple[str, str, str, dict[str, int]]] = []

    STAT_TYPES = [x["type"] for x in gamelib["stats"]]
    PROJECTILE_TYPES = [x["type"] for x in gamelib["projectiles"]]

    # Pickupables.
    # ============================================================
    # {  ###
    for i, x in enumerate_table("pickupables"):
        if not i:
            continue
        x["name_locale"] = "PICKUPABLE_{}".format(x["type"])
    # }

    # Effect conditions.
    # ============================================================
    # {  ###
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
                "STAT_2",
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
                        "color_by_stat": x.pop(f"{letter}_color_by_stat", False),
                        "color_by_stat_2": x.pop(f"{letter}_color_by_stat_2", False),
                    }
                )
    # }

    non_lockable_items: list[str] = []
    non_lockable_weapons: list[str] = []

    def process_effects_of(x, required_tier_values: int) -> None:
        # {  ###
        for e in x.get("effects", []):
            field_to_list(e, "value")
            field_to_list(e, "value_multiplier")
            for letter in EFFECT_CONDITION_LETTERS:
                field_to_list(e, f"condition_{letter}")
            for f in ("damage_scalings", "burning_damage_scalings"):
                for vv in e.get(f, []):
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
                field_to_list(v, "ints", required_tier_values)
                field_to_list(v, "floats", required_tier_values)

            if e.get("effectcondition_type") == "START_WITH__X__ITEM_OR_WEAPON":
                for container, field in (
                    (non_lockable_items, "item_type"),
                    (non_lockable_weapons, "weapon_type"),
                ):
                    if e.get(field):
                        container.append(e[field])

        # }

    TOTAL_TIERS = 4

    # Trail sounds.
    # ============================================================
    # {  ###
    for i, x in enumerate_table("trail_sounds"):
        if not i:
            continue
        assert x["repeat_seconds"] > 0
    # }

    # Items.
    # ============================================================
    # {  ###
    items_per_tier = [0] * TOTAL_TIERS

    for i, x in enumerate_table("items"):
        if not i:
            continue

        assert x.get("price", 0) > 0, "All items must have price > 0!"
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
    # }

    # Difficulties.
    # ============================================================
    # {  ###
    for i, x in enumerate_table("difficulties"):
        if not i:
            continue
        process_effects_of(x, 1)
        x["name_locale"] = f"DIFFICULTY_{i}"
        x["texture_id"] = "ui_item_difficulty_{}".format(i)

        for effect in x.get("effects", []):
            effect["modification_scaleable"] = False
    # }

    # Weapons.
    # ============================================================
    # {  ###
    for i, x in enumerate_table("weapons"):
        assert "min_tier_index" not in x

        if not i:
            continue

        # All burning fields must be specified at the same time. Or none at all.
        burning_fields = [
            x.get("burning_damage"),
            x.get("burning_times"),
            x.get("burning_damage_scalings"),
        ]
        assert all_are_not_none(burning_fields) or all_are_none(burning_fields)

        weapon_type, min_tier_index, name = x["type"].split("_", 2)
        min_tier_index = int(min_tier_index)

        x["min_tier_index"] = min_tier_index
        x["name_locale"] = "WEAPON_" + x["type"].upper()

        required_tier_values = TOTAL_TIERS - min_tier_index
        assert required_tier_values > 0
        assert required_tier_values <= TOTAL_TIERS

        texture_ids = x.get("texture_ids", [])
        texture_ids.insert(0, f"game_weapon_{i:02}_" + name.lower())
        x["texture_ids"] = texture_ids

        x["icon_texture_id"] = f"ui_weapon_{i:02}_" + name.lower()

        if weapon_type == "MELEE":
            assert x.get("melee_collider_height_px", 0) > 0
            assert "projectile_spawn_frame_factors" not in x
            x["attack_or_cooldown_ratio"] = 0.5
        elif weapon_type == "RANGED":
            assert "projectile_type" in x
            assert "melee_collider_height_px" not in x
            if "projectile_spawn_frame_factors" not in x:
                x["projectile_spawn_frame_factors"] = [0]
        else:
            assert False

        mandatory_fields = [
            "price",
            "base_damage",
            "crit_damage_multiplier",
            "crit_chance",
            "knockback_meters",
            "range_meters",
            "icon_texture_id",
            "texture_ids",
        ]
        assert "cooldown_frames" not in x
        for field in mandatory_fields:
            assert field in x, "Weapon {} has to have '{}' specified".format(
                x["type"], field
            )

        field_to_list(x, "base_damage", required_tier_values)
        field_to_list(x, "burning_damage", required_tier_values)
        field_to_list(x, "burning_times", required_tier_values)
        field_to_list(x, "crit_damage_multiplier", required_tier_values)
        field_to_list(x, "crit_chance", required_tier_values)
        field_to_list(x, "knockback_meters", required_tier_values)
        field_to_list(x, "cooldown", required_tier_values)
        field_to_list(x, "projectile_count", required_tier_values)
        field_to_list(x, "projectile_pierce", required_tier_values)
        field_to_list(x, "projectile_piercing_damage_bonus_percent", required_tier_values)
        field_to_list(x, "projectile_bounce", required_tier_values)
        field_to_list(x, "range_meters", required_tier_values)
        field_to_list(x, "life_steal_percents", required_tier_values)
        field_to_list(x, "projectile_spawn_frame_factors")

        if "attack_or_cooldown_ratio" in x:
            assert 0 <= x["attack_or_cooldown_ratio"] <= 1
        for v in x.get("projectile_spawn_frame_factors", []):
            assert 0 <= v <= 1

        shooting_duration_factor = x.get("shooting_duration_factor", 0.8)
        assert shooting_duration_factor >= 0
        assert shooting_duration_factor <= 1

        for f in ("damage_scalings", "burning_damage_scalings"):
            for vv in x.get(f, []):
                field_to_list(vv, "percents_per_tier", required_tier_values)
            for scalings in x.get(f, []):
                percents_count = len(scalings["percents_per_tier"])
                assert percents_count == required_tier_values, (
                    "Weapon {} in field `{}` must have {} `percents_per_tier` because it's `min_tier_index` is {}".format(
                        x["type"], f, required_tier_values, min_tier_index
                    )
                )

        if "projectile_count" in x:
            assert "accuracy_plus_minus" in x

        if "emit_particle_type" in x:
            assert "emit_particle_seconds" in x
        else:
            assert "emit_particle_seconds" not in x

        process_effects_of(x, required_tier_values)
    # }

    # Stats.
    # ============================================================
    # {  ###
    for i, x in enumerate_table("stats"):
        if not i:
            continue

        x["name_locale"] = "STAT_" + x["type"].upper()
        icon_texture = x.pop("icon_texture_id", None)
        if icon_texture is not None:
            x["big_icon_texture_id"] = f"{icon_texture}_big"
            if x["type"] != "COINS":
                x["small_icon_texture_id"] = f"{icon_texture}_small"

        if i >= 4 and not x.get("is_secondary"):
            x["upgrade_name_locale"] = "UPGRADE_NAME_" + x["type"].upper()
    # }

    # Creatures.
    # ============================================================
    # {  ###
    for i, x in enumerate_table("creatures"):
        if i >= 1:
            mandatory_fields = ["hurtbox_radius"]
            for field in mandatory_fields:
                assert field in x, "Creature {} needs `{}` specified".format(
                    x["type"], field
                )
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
    # }

    # Placeholders.
    # ============================================================
    if 1:  # {  ###
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
    # }

    locked_builds: list[str] = []
    locked_items: list[str] = []
    locked_weapons: list[str] = []

    # Builds.
    # ============================================================
    if 1:  # {  ###
        build_achievements = [
            x["type"].removeprefix("FINISH_RUN_WITH_BUILD_")
            for x in gamelib["achievements"]
            if x["type"].startswith("FINISH_RUN_WITH_BUILD_")
        ]

        assert not non_lockable_items
        assert not non_lockable_weapons

        max_weapons = 0
        for i, x in enumerate_table("builds"):
            if not i:
                continue

            x["finish_run_achievement_type"] = f"FINISH_RUN_WITH_BUILD_{x['type']}"
            for effect in x.get("effects", []):
                effect["modification_scaleable"] = False

            process_effects_of(x, 1)
            x["name_locale"] = "BUILD_{}".format(x["type"])
            max_weapons = max(max_weapons, len(x["starting_weapon_types"]))
            check_duplicates(x["starting_weapon_types"])

            assert len(x["layer_colors"]) == len(gamelib["player_layer_texture_ids"])

        assert non_lockable_items
        assert non_lockable_weapons

        for i, x in enumerate_table("builds"):
            if not i:
                continue

            item = x.get("unlocks_item_type")
            weapon = x.get("unlocks_weapon_type")

            assert only_one_is_not_none((item, weapon)), (
                f"Build {x['type']} must have `unlocks_item_type` or `unlocks_weapon_type`"
            )

            for field, value, non_lockable, locked in (
                ("item", item, non_lockable_items, locked_items),
                ("weapon", weapon, non_lockable_weapons, locked_weapons),
            ):
                assert value not in non_lockable, f"{field}={value} can't be locked"
                assert value not in locked, f"{field}={value} is already locked"

        for i, x in enumerate_table("builds"):
            if not i:
                continue

            assert x["type"] in build_achievements, (
                "Build {} doesn't have respective achievemnt FINISH_RUN_WITH_BUILD_{}".format(
                    x["type"], x["type"]
                )
            )
            build_achievements.remove(x["type"])

        assert not build_achievements, (
            "Found extra achievements but not builds: achievements are {}".format(
                build_achievements
            )
        )

        genline("constexpr int MAX_BUILD_WEAPONS = {};\n".format(max_weapons))
    # }

    # Achievements.
    # ============================================================
    if 1:  # {  ###
        more_less = ("more", "less")

        genline(f"constexpr int ACHIEVEMENTS_X = {ACHIEVEMENTS_X};\n")

        explode_achievements(gamelib)

        gamelib["achievements_order"] = json.loads(
            ACHIEVEMENTS_ORDER_FILEPATH.read_text()
        )

        step_global_index = -1

        for i, x in enumerate_table("achievements"):
            if not i:
                continue

            can_have_build = False
            can_have_stat = False

            if x["type"].startswith("FINISH_RUN_WITH_BUILD_"):
                # NOTE: Gets filled during `explode_achievements`.
                can_have_build = True

            elif x["type"].startswith(
                tuple(f"REACH_THIS_OR_{x.upper()}_STAT_" for x in more_less)
            ):
                can_have_stat = True
                for more_or_less in more_less:
                    prefix = f"REACH_THIS_OR_{more_or_less.upper()}_STAT_"
                    if not x["type"].startswith(prefix):
                        continue

                    if more_or_less == "less":
                        x["negative_is_good"] = True

                    stat_type = x["type"].removeprefix(prefix)
                    assert stat_type in STAT_TYPES

                    x["stat_type"] = stat_type
                    x["description_locale"] = (
                        f"ACHIEVEMENT_DESCRIPTION_REACH_THIS_OR_{more_or_less.upper()}_STAT"
                    )

                    stat = gamelib["stats"][STAT_TYPES.index(stat_type)]
                    stat_field = f"reach_this_or_{more_or_less}_stat_achievement_type"
                    assert stat_field not in stat
                    stat[stat_field] = x["type"]

            elif x["type"].startswith("MAX_COUNT_OF_PROJECTILE_ON_SCREEN_"):
                projectile_type = x["type"].removeprefix(
                    "MAX_COUNT_OF_PROJECTILE_ON_SCREEN_"
                )
                projectile = gamelib["projectiles"][
                    PROJECTILE_TYPES.index(projectile_type)
                ]
                assert "max_simultaneous_count_achievement_type" not in projectile
                projectile["max_simultaneous_count_achievement_type"] = x["type"]

            if "name_locale" not in x:
                x["name_locale"] = f"ACHIEVEMENT_NAME_{x['type']}"
            if "description_locale" not in x:
                x["description_locale"] = f"ACHIEVEMENT_DESCRIPTION_{x['type']}"

            assert can_have_build == bool(x.get("build_type"))
            assert can_have_stat == bool(x.get("stat_type"))

            mandatory_fields = [
                "steps",
            ]
            for field in mandatory_fields:
                assert field in x, "Achievement {} has to have '{}' specified".format(
                    x["type"], field
                )
            for stepIndex, step in enumerate(x["steps"]):
                step_global_index += 1
                step["global_index"] = step_global_index

                assert step["value"] != 0
                # step["order"] = achievements_order[x["type"]][stepIndex]

                unlock_fields = {
                    "unlocks_build_type": locked_builds,
                    "unlocks_weapon_type": locked_items,
                    "unlocks_item_type": locked_weapons,
                }
                c = sum(f in step for f in unlock_fields if step.get(f))
                assert c <= 1, unlock_fields
                if c != 1:
                    warning(
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
                        assert v not in container, "{} {} is already locked".format(
                            field, v
                        )
                        container.append(v)

                        assert v not in {
                            "unlocks_item_type": non_lockable_items,
                            "unlocks_weapon_type": non_lockable_weapons,
                        }.get(field, [])

        genline(f"constexpr int TOTAL_ACHIEVEMENT_STEPS = {step_global_index + 1};\n")
    # }

    # Props.
    # ============================================================
    if 1:  # {  ###
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
    # }

    with open(SRC_DIR / "game" / "bf_gamelib.fbs", encoding="utf-8") as in_file:
        gamelib_fbs_lines = [l.strip() for l in in_file if l.strip()]

    # Texture bind.
    # ============================================================
    if 1:  # {  ###
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
    # }

    # Tables.
    # ============================================================
    if 1:  # {  ###
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
            types = [x["type"] for x in gamelib[field_name]]
            for t in types:
                assert t.upper() == t, (
                    "{}. {}. `type` fields must be in CONSTANT_CASE".format(field_name, t)
                )
            check_duplicates(types)
            genenum(genline, type_name + "Type", types, add_count=True)
            transforms.append(
                (
                    field_name,
                    f"{type_name.lower()}_type",
                    f"{type_name.lower()}_types",
                    {v: i for i, v in enumerate(types)},
                )
            )
    # }

    # Other stuff.
    # ============================================================
    # {  ###
    assert len(gamelib["player_layer_texture_anchors"]) == len(
        gamelib["player_layer_texture_ids"]
    )

    # Checking that all builds except DEFAULT (and INVALID) are locked.
    not_locked_builds = []
    for build in gamelib["builds"][2:]:
        if build["type"] not in locked_builds:
            not_locked_builds.append(build["type"])

    if not_locked_builds:
        warning(f"These builds must be locked: {not_locked_builds}")

    assert "DEFAULT" not in locked_builds
    assert "INVALID" not in locked_builds
    # }

    # Codepoints.
    # ============================================================
    if 1:  # {  ###
        ranges = [
            (ord(" "), ord(" ") + 1),  # Space character
            # (ord("−"), ord("−") + 1),  # Long minus character
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
    # }

    # Transforms.
    # ============================================================
    # {  ###
    for v in transforms:
        recursive_replace_transform(gamelib, *(v[1:]))

    recursive_flattenizer(gamelib, "damage_scaling", "damage_scalings", "damage_scalings")
    # }


@command
@timing
def process_images():
    # {  ###
    SLOT_SIZE = 200
    UI_SLOT_OR_FRAME_RADIUS = 30

    # `ui_itemslot`.
    slot_image = bf_image.rectangle(SLOT_SIZE, radius=UI_SLOT_OR_FRAME_RADIUS)
    slot_image.save(ART_TEXTURES_DIR / "ui_itemslot.png")

    # `ui_frame`.
    bf_image.rectangle(
        112,
        radius=UI_SLOT_OR_FRAME_RADIUS,
        width=10,
        outline=hex_to_rgb_ints("969696"),
        fill=hex_to_rgb_ints("5a5a5a"),
    ).save(ART_TEXTURES_DIR / "ui_frame.png")

    DEBUG_SHADOWS = 0

    # `ui_frame_shadow_small`.
    bf_image.outline(
        image=bf_image.red(slot_image),
        radius=60,
        color=(0, 0, 0, 255),
        is_shadow=True,
        blend_image_on_top=DEBUG_SHADOWS,
    ).save(ART_TEXTURES_DIR / "ui_frame_shadow_small.png")

    # `ui_frame_shadow_big`.
    bf_image.outline(
        image=bf_image.red(slot_image),
        radius=120,
        color=(0, 0, 0, 255),
        is_shadow=True,
        blend_image_on_top=DEBUG_SHADOWS,
    ).save(ART_TEXTURES_DIR / "ui_frame_shadow_big.png")

    # `ui_player_bar_back`.
    bf_image.rectangle((634, 92), radius=30).save(
        ART_TEXTURES_DIR / "ui_player_bar_back.png"
    )
    # `ui_player_bar_top`.
    bf_image.rectangle((604, 64), radius=20).save(
        ART_TEXTURES_DIR / "ui_player_bar_top.png"
    )
    # `ui_boss_bar_back`.
    bf_image.rectangle((280, 40), radius=14).save(
        ART_TEXTURES_DIR / "ui_boss_bar_back.png"
    )
    # `ui_boss_bar_top`.
    bf_image.rectangle((260, 22), radius=10).save(
        ART_TEXTURES_DIR / "ui_boss_bar_top.png"
    )

    # `game_shadow_*.png`.
    for i in range(3):
        size = (312 // (2**i), 96 // (2**i))
        bf_image.ellipse(size).save(ART_TEXTURES_DIR / f"game_shadow_{i + 1}.png")

    TOUCH_OUTLINE = 10

    # TOUCH_COLOR = (*hex_to_rgb_ints("efcb84"), 255)
    TOUCH_COLOR = (255, 255, 255, 255)

    # `ui_controls_touch_base`.
    bf_image.ellipse(
        460, width=TOUCH_OUTLINE, outline=(0, 0, 0, 255), fill=(0, 0, 0, 0)
    ).save(ART_TEXTURES_DIR / "ui_controls_touch_base.png")

    # `ui_controls_touch_handle`.
    bf_image.outline(
        bf_image.ellipse(128, fill=TOUCH_COLOR),
        radius=TOUCH_OUTLINE,
    ).save(ART_TEXTURES_DIR / "ui_controls_touch_handle.png")

    # Making volume band icons.
    for f in (ART_TEXTURES_DIR / "to_outline").glob("ui_icon_volume_band_*.png"):
        f.unlink()
    bf_image.conveyor(
        "to_volume_band",
        "Outlining",
        bf_image.conveyor_outline(radius=10, color=(255, 255, 255, 255)),
        bf_image.conveyor_scale(0.2127),
        out_dir=ART_TEXTURES_DIR / "to_outline",
    )

    # Outlining ui icons.
    bf_image.conveyor(
        "to_outline",
        "Outlining",
        bf_image.conveyor_outline(radius=8, color=(0, 0, 0, 255), is_shadow=False),
    )

    # Extracting white and black of floor sprites.
    for f in ART_TEXTURES_DIR.glob("game_floor_*.png"):
        f.unlink()
    bf_image.conveyor(
        "to_split",
        "Extracting white",
        bf_image.conveyor_extract_white(),
        bf_image.conveyor_suffix("front"),
    )
    bf_image.conveyor(
        "to_split",
        "Extracting black",
        bf_image.conveyor_extract_black(),
        bf_image.conveyor_suffix("back"),
    )

    # Making input images.
    bf_image.outline(
        bf_image.rectangle(160, radius=20, width=10),
        radius=1,
        color=(0, 0, 0, 0),
    ).save(ART_DIR / "src" / "ui_input_key.png")

    # Spritesheetifying props.
    for f in (ART_TEXTURES_DIR / "to_biome").glob("game_prop_*.png"):
        f.unlink()
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_003.png",
        gap=14,
        cell_size=100,
        size=(5, 5),
        out_filename_prefix="game_prop_",
        out_dir=ART_TEXTURES_DIR / "to_biome",
    )

    # Biomefying props.
    for f in ART_TEXTURES_DIR.glob("game_prop_*.png"):
        f.unlink()
    gamelib = load_gamelib_cached()
    get_color = lambda biome, x: hex_to_rgb_ints(hex(biome[x])[2:-2])
    for biome in gamelib["biomes"][1:]:
        t = biome["type"]
        bf_image.conveyor(
            "to_biome",
            f"{t}: Remapping",
            bf_image.conveyor_remap(
                get_color(biome, "outline_color"), get_color(biome, "fill_color")
            ),
            bf_image.conveyor_suffix(t.lower()),
        )

    # Spritesheetifying stat icons.
    for f in (ART_TEXTURES_DIR / "stat_icons").glob("*.png"):
        f.unlink()
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_005.png",
        cell_size=160,
        size=(5, 5),
        gap=10,
        out_filename_prefix="ui_stat_icon_",
        out_filenames=[
            "level",
            "hp",
            "regen",
            "life_steal",
            "damage",
            "damage_melee",
            "damage_ranged",
            "damage_elemental",
            "attack_speed",
            "crit_chance",
            "engineering",
            "range",
            "armor",
            "dodge",
            "speed",
            "luck",
            "harvesting",
        ],
        out_dir=ART_TEXTURES_DIR / "stat_icons",
    )

    # Transforming stat icons into big and small.
    for f in ART_TEXTURES_DIR.glob("ui_stat_icon_*.png"):
        f.unlink()
    bf_image.conveyor("stat_icons", "Copying Big", bf_image.conveyor_suffix("big"))
    bf_image.conveyor(
        "stat_icons",
        "Downscaling Small",
        bf_image.conveyor_scale(0.65),
        bf_image.conveyor_brightness(1 + 3 / 8),
        # bf_image.conveyor_outline(radius=1, color=(0, 0, 0, 255), is_shadow=False),
        # bf_image.conveyor_outline(radius=2, color=(255, 255, 255, 255), is_shadow=False),
        bf_image.conveyor_suffix("small"),
    )

    # Spritesheetifying player parts and hats.
    for f in ART_TEXTURES_DIR.glob("game_player_layer_*.png"):
        f.unlink()
    for f in ART_TEXTURES_DIR.glob("game_hat_*.png"):
        f.unlink()
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_005.png",
        cell_size=160,
        size=(5, 5),
        origin=(870, 0),
        gap=10,
        out_filename_prefix="game_",
        out_filenames=[
            "player_layer_head",
            "player_layer_feet",
            "player_layer_shirt",
            "player_layer_phartuk",
            "player_layer_beard",
            "player_layer_arms",
            "hat_gopnik",
            "hat_beanie",
            "hat_top",
            "hat_kepka",
            "hat_milker",
        ],
        out_dir=ART_TEXTURES_DIR,
    )

    # Spritesheetifying clip studio items.
    for f in ART_TEXTURES_DIR.glob("ui_item_*.png"):
        f.unlink()
    item_names = [
        "cloud_in_a_bottle",
        "mechanical_lens",
        "obsidian_rose",
        "obsidian_skull",
        "mithril_shield",
        "bowl_of_grechka_buckwheat",
        "2d4",
        "crown",
        "partizan_cookie",
        "falling_cookie_star",
        "bone",
        "highbrow_hat",
        "sugar",
        "implant_plate",
        "magnifier",
        "chest_without_gold",
        "claws",
        "flying_carpet",
        "jelly_eye",
        "thorns_bracelet",
        "devil_horns",
        "bread_knuckles",
        "lucky_horseshoe",
        "obsidian_horseshoe",
        "regeneration_potion",
        "golden_sapling",
        "smoke_bomb",
        "grand_tree_sapling",
        "leather_glove",
        "glove_of_pyromancer",
        "cog",
        "springed_boots",
        "pink_candle",
        "water_candle",
        "pepper",
        "steampunk_goggles",
        "bouncing_ball_blueprint",
        "chocolate_pillows_bowl",
        "clover_cracker",
        "chocolate_mushroom",
        "garden",
        "landmine",
        "turret",
        "heavy_bracelet",
        "glove_of_dexer",
        "brownie_belt",
        "white_chocolate_potion",
        "dark_chocolate_potion",
        "bath_bomb_grenade",
        "nail",
        "skewer",
        "watermelon_helmet",
        "bandage",
        "bread_slice",
        "banana",
        "bag_of_gold",
        "damage_book",
        "ninja_cucumber",
        "greek_nut",
        "hamburger",
        "honey_comb",
        "honey_bottle",
        "inspiring_banner",
        "strange_mushroom",
        "mechanical_flower",
        "chocolate_coin",
        "alluring_donut",
        "regen_cracker",
        "healthy_cracker",
        "concentrating_donut",
        "garlick",
        "book",
        "portable_rainbow",
        "soup_pot",
        "magnetic_d6",
        "coupon",
        "harvesting_pact",
        "vampire_teeth",
        "wd",
        "recycling_bin",
        "dark_chocolate",
        "white_chocolate",
        "cooravie",
        "cat_ring",
        "void_donut",
        "heavy_spring",
        "d6",
        "piercing_bullet_blueprint",
        "hat_of_cook",
        "red_witch_ring",
        "chandelier",
        "handcuffed_doll",
        "shiny_stone",
        "sapling",
        "flint",
        "donut",
        "empty_milk_barrel",
        "pumpkin_helmet",
        "bandaged_apple",
        "golden_ingot",
        "gunpowder",
        "pumpkin",
        "phartuk",
        "milk",
        "d12",
        "locked",
        "piggy_bank",
        *["difficulty_{}".format(i + 1) for i in range(len(gamelib["difficulties"]) - 1)],
    ]
    assert len(item_names) == 110, len(item_names)
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_002.png",
        cell_size=200,
        size=(15, 8),
        gap=24,
        out_filename_prefix="ui_item_",
        out_filenames=item_names,
        out_dir=ART_TEXTURES_DIR,
    )

    for f in ART_TEXTURES_DIR.glob("game_projectile_*.png"):
        f.unlink()

    other_filenames = [
        "game_creature_chaser",
        "game_decal_pre_spawn",
        "game_landmine",
        "ui_coin_x2",
        "game_projectile_mob",
        "game_pickupable_chest",
        "game_creature_tree",
        "game_pickupable_apple",
        "game_creature_ranger",
        "game_creature_spawner",
        "game_creature_chester",
        "game_creature_mob_small",
        "game_creature_rusher",
        "game_creature_boss",
        "game_projectile_boss",
        "game_creature_player",
    ]
    for f in other_filenames:
        fi = ART_TEXTURES_DIR / (f + ".png")
        if fi.exists():
            fi.unlink()
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_004.png",
        cell_size=440,
        size=(8, 4),
        gap=16,
        out_filenames=other_filenames,
        out_dir=ART_TEXTURES_DIR,
    )

    # Game weapons (not ui icons).
    for f in ART_TEXTURES_DIR.glob("game_weapon_*.png"):
        f.unlink()
    weapon_names = [
        "cacti_club",
        "chopper",
        "claw",
        "fist",
        "ghost_axe",
        "ghost_flint",
        "lance",
        "lightning_shiv",
        "plank",
        "pruner",
        "quaterstaff",
        "rock",
        "screwdriver",
        "sharp_tooth",
        "spiky_shield",
        "stick",
        "torch",
        "wrench",
        "circular_saw",
        "flaming_brass_knuckles",
        "hammer",
        "sword",
        "plasma_sledge",
        "power_fist",
        "thunder_sword",
        "dex_troyer",
        "excalibur",
        "scythe",
        "crossbow",
        "shotgun",
        "ghost_scepter",
        "icicle",
        "javelin",
        "laser_gun",
        "pistol",
        "shredder",
        "shuriken",
        "slingshot",
        "smg",
        "wand",
        "fireball",
        "flamethrower",
        "potato_thrower",
        "rocker_launcher",
        "minigun",
        "nuclear_launcher",
        "sniper_gun",
        "chain_gun",
        "gatling_laser",
    ]
    custom_weapon_parts = [
        "crossbow_string",
    ]
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_006.png",
        cell_size=300,
        size=(12, 6),
        gap=10,
        out_filename_prefix="game_weapon_",
        out_filenames=[
            f"{i + 1:02}_{x}" for i, x in enumerate(weapon_names + custom_weapon_parts)
        ],
        out_dir=ART_TEXTURES_DIR,
    )

    # Weapon icons.
    for f in ART_TEXTURES_DIR.glob("ui_weapon_*.png"):
        f.unlink()
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_007.png",
        cell_size=200,
        size=(17, 8),
        gap=24,
        out_filename_prefix="ui_weapon_",
        out_filenames=[f"{i + 1:02}_{x}" for i, x in enumerate(weapon_names)],
        out_dir=ART_TEXTURES_DIR,
    )

    # Spritesheetifying projectiles.
    for f in (ART_TEXTURES_DIR / "projectiles").glob("*.png"):
        f.unlink()
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_005.png",
        cell_size=160,
        size=(5, 5),
        origin=(1740, 0),
        gap=10,
        out_filename_prefix="game_projectile_",
        out_filenames=[
            "bullet",
            "laser",
            "needle",
            "bolt",
            "apple",
            "magical_bullet",
            "egg",
            "rocket",
            "rocket_milk",
            "lightning",
        ],
        out_dir=ART_TEXTURES_DIR / "projectiles",
    )
    bf_image.conveyor("projectiles", "Copying")

    bf_image.outline(
        bf_image.white(
            Image.open(
                ART_TEXTURES_DIR / "projectiles" / "game_projectile_magical_bullet.png"
            )
        ),
        radius=16,
        color=(255, 255, 255),
        is_shadow=True,
    ).save(ART_TEXTURES_DIR / "game_projectile_magical_bullet.png")

    bf_image.outline(
        Image.open(ART_TEXTURES_DIR / "projectiles" / "game_projectile_lightning.png"),
        radius=16,
        color=(255, 255, 72, int(255 * 2 / 5)),
        is_shadow=True,
    ).save(ART_TEXTURES_DIR / "game_projectile_lightning.png")

    bf_image.outline(
        bf_image.ellipse(80),
        radius=28,
        color=(207, 200, 178),
        is_shadow=True,
    ).save(ART_TEXTURES_DIR / "game_projectile_bullet.png")

    bf_image.outline(
        bf_image.ellipse(80),
        radius=28,
        color=(255, 123, 239),
        is_shadow=True,
    ).save(ART_TEXTURES_DIR / "game_projectile_laser.png")

    bf_image.outline(
        bf_image.ellipse(80, fill=(255, 255, 255)),
        radius=28,
        color=(255, 255, 255),
        is_shadow=True,
    ).save(ART_TEXTURES_DIR / "game_projectile_fireball.png")

    # Spritesheetifying fire.
    for f in ART_TEXTURES_DIR.glob("game_particle_fire_*.png"):
        f.unlink()
    for f in (ART_TEXTURES_DIR / "fire").glob("game_particle_fire_*.png"):
        f.unlink()
    bf_image.spritesheetify(
        ART_DIR / "src" / "main_005.png",
        cell_size=160,
        size=(5, 1),
        origin=(2610, 340),
        gap=10,
        out_filename_prefix="fire_",
        out_dir=ART_TEXTURES_DIR / "fire",
    )
    bf_image.conveyor(
        "fire",
        "Making particles",
        bf_image.conveyor_outline(radius=24, color=(255, 255, 255, 128), is_shadow=True),
        bf_image.conveyor_white,
        bf_image.conveyor_prefix("game_particle"),
    )

    if 1:
        banner = Image.new("RGBA", (1920, 1080))
        h = 185
        # margin = 20
        margin = 0
        outline_width = 7
        w = 1920 + 2 * outline_width
        rect = bf_image.rectangle(
            (w - margin * 2, h - margin),
            fill="white",
            # radius=80,
            width=outline_width,
            outline="black",
        )
        banner.paste(rect, ((1920 - w) // 2 + margin, 1080 - h + outline_width))
    else:
        banner = Image.open(ART_DIR / "src" / "screenshot_text_banner.png")

    banner = bf_image.outline(
        banner,
        radius=80,
        color=(0, 0, 0, int(255 * 5 / 16)),
        is_shadow=True,
        extend=False,
    )

    recursive_mkdir(ART_DIR / "src" / "screenshots_processed")
    banner_colors = [
        "e7ae4b",
        "b59a66",
        "66a650",
        "c95d9c",
    ]

    _result = read_localization_csv()
    screenshot_loc_id_indices = sorted(
        i for i, x in enumerate(_result.loc_ids) if x.startswith("YANDEX_SCREENSHOT_")
    )

    font = ImageFont.truetype(
        ART_DIR / "src" / "screenshots" / "SeymourOne-Regular.ttf", size=150
    )

    for language, texts in _result.loc_by_languages.items():
        out_dir = ART_DIR / "src" / "screenshots_processed" / language
        for f in out_dir.glob("*.png"):
            f.unlink()
        recursive_mkdir(out_dir)

        for banner_color_, loc_id_index, f in zip(
            banner_colors,
            screenshot_loc_id_indices,
            (ART_DIR / "src" / "screenshots").glob("*.png"),
            strict=True,
        ):
            banner_color = hex_to_rgb_ints(banner_color_)

            text_image = Image.new("RGBA", (3840, 1550))
            draw = ImageDraw.Draw(text_image)
            draw.text(
                (1920, 1478),
                texts[loc_id_index],
                fill=tuple(
                    int(x * 255)
                    for x in transform_color(
                        hex_to_rgb_floats(banner_color_),
                        saturation_scale=0.23,
                        value_scale=2.5,
                    )
                ),
                anchor="ms",
                font=font,
                stroke_width=14,
                stroke_fill="black",
            )
            text_image = bf_image.outline(
                text_image,
                radius=40,
                color=(0, 0, 0, 255 * 3 / 16),
                is_shadow=True,
                extend=False,
            )

            brightness = 1.0
            contrast = 1.0
            if 1:
                brightness = 1.06
                contrast = 1.1

            bf_image.draw_on_top(
                bf_image.draw_on_top(
                    ImageEnhance.Brightness(
                        ImageEnhance.Contrast(Image.open(f)).enhance(contrast)
                    ).enhance(brightness),
                    banner,
                    (*banner_color, 255),
                ),
                text_image.resize((1920, 1080)),
            ).save(out_dir / f.name)

    # }


@command
def make_swatch():
    # {  ###
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

    with open("aboba.pal", "w") as out_file_2:
        out_file_2.write(
            "JASC-PAL\n0100\n{}\n".format(
                len(new_colors),
            )
        )
        color_lines = []
        for color in new_colors:
            r, g, b = hex_to_rgb_ints(color)
            color_lines.append(f"{r} {g} {b}")
        color_lines = color_lines[2:] + color_lines[:2]
        out_file_2.write("\n".join(color_lines))
    # }


@command
def reorder_achievements():
    # {  ###
    gamelib = load_gamelib_cached()
    explode_achievements(gamelib)

    AchievementIndex: TypeAlias = int
    StepIndex: TypeAlias = int
    Entry: TypeAlias = tuple[AchievementIndex, StepIndex]
    entries: list[Entry] = []

    achs = gamelib["achievements"]
    for ach_index, ach in enumerate(achs[1:]):
        for step_index in range(len(ach.get("steps", []))):
            entries.append((ach_index, step_index))

    console = Console()
    process = None

    error = None
    ERROR_INCORRECT_VALUE_PROVIDED = "Incorrect value provided!"
    while True:
        # Printing all achievements.
        table = Table("I", "POS", "TYPE", "UNLOCKS")
        for entry_index, d in enumerate(entries):
            ach = achs[d[0] + 1]
            step_index = d[1]
            step = ach["steps"][step_index]

            t = ach["type"]
            if len(gamelib["achievements"][d[0] + 1]["steps"]) > 1:
                t += f" ({d[1]})"

            table.add_row(
                str(entry_index),
                "{}x{}".format(
                    entry_index % ACHIEVEMENTS_X, entry_index // ACHIEVEMENTS_X
                ),
                t,
                str(
                    step.get("unlocks_build_type")
                    or step.get("unlocks_weapon_type")
                    or step.get("unlocks_item_type")
                ),
            )
        console.print(table)

        if error:
            print(error)
            error = None

        # User inputs value.
        user_value = input("Enter q to quit, or 2 indices to swap them: ").strip()

        # Processing user's input.
        if user_value == "q":
            break

        value_strings = user_value.split()
        if len(value_strings) != 2:
            error = ERROR_INCORRECT_VALUE_PROVIDED
            continue

        for v in value_strings:
            if not v.isdigit():
                error = ERROR_INCORRECT_VALUE_PROVIDED
                break
        if error:
            continue

        values = [int(x) for x in value_strings]

        i1: int = min(values)
        i2: int = max(values)
        if (
            (i1 == i2)
            or (i1 < 0)
            or (i2 < 0)
            or (i1 >= len(entries))
            or (i2 >= len(entries))
        ):
            error = ERROR_INCORRECT_VALUE_PROVIDED
            continue

        # Swapping entries.
        entries.append(entries[i1])
        entries[i1] = entries[i2]
        entries[i2] = entries.pop()

        # Dumping to file.
        steps_per_achievement: list[int] = [0] * (len(gamelib["achievements"]) - 1)
        for ach_index, _ in entries:
            steps_per_achievement[ach_index] += 1
        data = []
        for ach_index, step_index in entries:
            data.append(
                {
                    "achievement_type": gamelib["achievements"][ach_index + 1]["type"],
                    "step_index": step_index,
                }
            )
        ACHIEVEMENTS_ORDER_FILEPATH.write_text(json.dumps(data, indent=4))

        if process is not None and process.returncode is None:
            process.kill()
        process = subprocess.Popen(
            r"uv run python cli\bf_cli.py codegen Win Debug",
            shell=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
    # }


@command
def temp():
    pass


###
