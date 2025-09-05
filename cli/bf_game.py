"""
USAGE:

    from bf_lib import data_values, gamelib_processor

    data_values.itch_target = "hulvdan/cult-boy"
    data_values.languages = ["russian", "english"]

    @gamelib_processor
    def process_gamelib(_genline, gamelib, _localization_codepoints: set[int]) -> None:
        for tile in gamelib["tiles"]:
            tile.pop("type")
"""

from itertools import groupby

from bf_lib import data_values, gamelib_processor, genenum

data_values.itch_target = "hulvdan/NOT-SPECIFIED"
data_values.languages = ["russian", "english"]


@gamelib_processor
def process_gamelib(genline, gamelib, localization_codepoints: set[int]) -> None:
    # Creatures.
    # ============================================================
    if 1:
        CREATURE_TYPES = [x.pop("type") for x in gamelib["creatures"]]
        genenum(genline, "CreatureType", CREATURE_TYPES)

    # Projectiles.
    # ============================================================
    if 1:
        PROJECTILE_TYPES = [x.pop("type") for x in gamelib["projectiles"]]
        genenum(genline, "ProjectileType", PROJECTILE_TYPES)

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
