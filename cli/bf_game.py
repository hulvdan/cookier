"""
USAGE:

    from bf_lib import data_values, gamelib_processor

    data_values.itch_target = "hulvdan/cult-boy"

    @gamelib_processor
    def process_tiles(_genline, gamelib) -> None:
        for tile in gamelib["tiles"]:
            tile.pop("codename")
"""
