"""
USAGE:

    from bf_lib import data_values, gamelib_processor

    data_values.itch_target = "hulvdan/cult-boy"
    data_values.languages = ["russian", "english"]

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
    recursive_replace_transform,
)

data_values.itch_target = "hulvdan/cookier"
data_values.languages = ["russian", "english"]


def _check_duplicates(values: list) -> None:
    for i in range(len(values)):
        for k in range(i + 1, len(values)):
            assert values[i] != values[k], f"Found duplicate value: {values[i]}"


@gamelib_processor
def _process_gamelib(genline, gamelib, localization_codepoints: set[int]) -> None:
    transforms: list[tuple[str, str, dict[str, int]]] = []

    # Items.
    # ============================================================
    for x in gamelib["items"][1:]:
        x["name_locale"] = "ITEM_" + x["type"].upper()

        mandatory_fields = [
            "price",
        ]
        for field in mandatory_fields:
            assert field in x, "Item {} has to have '{}' specified".format(
                x["type"], field
            )

    # Weapons.
    # ============================================================
    for x in gamelib["weapons"][1:]:
        x["name_locale"] = "WEAPON_" + x["type"].upper()

        mandatory_fields = [
            "max_price",
            "min_tier_index",
            "base_damage",
            "damage_scalings",
        ]
        for field in mandatory_fields:
            assert field in x, "Weapon {} has to have '{}' specified".format(
                x["type"], field
            )

        for scalings in x["damage_scalings"]:
            percents_count = len(scalings["percents_per_tier"])
            assert percents_count == 4 - x["min_tier_index"], (
                "Weapon {} must have {} `percents_per_tier` because it's `min_tier_index` is {}".format(
                    x["type"], 4 - x["min_tier_index"], x["min_tier_index"]
                )
            )
        assert len(x["base_damage"]) == 4 - x["min_tier_index"], (
            "Weapon {} must have {} `base_damage` because it's `min_tier_index` is {}".format(
                x["type"], 4 - x["min_tier_index"], x["min_tier_index"]
            )
        )

    # Stats.
    # ============================================================
    for i, x in enumerate(gamelib["stats"]):
        if i >= 1:
            x["name_locale"] = "STAT_" + x["type"].upper()
        if i >= 2 and not x.get("is_secondary"):
            x["upgrade_name_locale"] = "UPGRADE_NAME_" + x["type"].upper()

    # Creatures.
    # ============================================================
    for i, x in enumerate(gamelib["creatures"]):
        if i >= 2:
            mandatory_fields = [
                "appearing_wave_number",
                "spawn_factor",
                "health",
                "health_increase_per_wave",
                "contact_damage",
                "contact_damage_increase_per_wave",
                "knockback_resistance",
            ]
            for field in mandatory_fields:
                assert field in x, "Mob {} needs `{}` specified".format(x["type"], field)

            assert 0 <= x["knockback_resistance"] <= 1, (
                "Mob {} `knockback_resistance` must be in range [0; 1]".format(x["type"])
            )

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

        genline("int g_codepoints[]{")
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
