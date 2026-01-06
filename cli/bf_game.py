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
from itertools import groupby

import bf_lib as bf
import bf_swatch
from bf_typer import command, timing

# }

bf.game_settings.itch_target = "hulvdan/"
bf.game_settings.languages = ["russian", "english"]
bf.game_settings.generate_flatbuffers_api_for = ["bf_save.fbs"]
bf.game_settings.yandex_metrica_counter_id = 0


@command
def temp():
    pass


###
