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

from bf_lib import data_values, gamelib_processor, genenum, recursive_replace_transform

data_values.itch_target = "hulvdan/cookier"
data_values.languages = ["russian", "english"]


def _check_duplicates(values: list) -> None:
    for i in range(len(values)):
        for k in range(i + 1, len(values)):
            assert values[i] != values[k]


@gamelib_processor
def _process_gamelib(genline, gamelib, localization_codepoints: set[int]) -> None:
    transforms: list[tuple[str, str, dict[str, int]]] = []

    # Tables.
    # ============================================================
    if 1:
        table_keys_to_update: list[str] = []
        for key_ in gamelib:
            suffix = "__table"
            if key_.endswith(suffix):
                key = key_.removesuffix(suffix)
                table_keys_to_update.append(key)
                types = [x.pop("type") for x in gamelib[key_]]
                _check_duplicates(types)
                genenum(genline, key.title() + "Type", types)
                transforms.append(
                    (f"{key}_type", f"{key}_types", {v: i for i, v in enumerate(types)})
                )
        for key in table_keys_to_update:
            gamelib[f"{key}s"] = gamelib.pop(f"{key}__table")

    # Normalization of wave creature spawn_factors.
    # ============================================================
    for wave in gamelib["waves"]:
        creatures = wave["creatures_to_spawn"]
        spawn_factor_sum = sum(x["spawn_factor"] for x in creatures)

        prev_factor = 0
        for creature in creatures:
            creature["spawn_factor"] = creature["spawn_factor"] / spawn_factor_sum
            creature["spawn_factor"] += prev_factor
            prev_factor = creature["spawn_factor"]

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
