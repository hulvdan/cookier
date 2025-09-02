import logging
import sys
from enum import Enum, unique
from typing import BinaryIO, NamedTuple

log = logging.getLogger("__name__")

message_handler = logging.StreamHandler(sys.stdout)
message_handler.setLevel(logging.DEBUG)
message_handler.addFilter(lambda record: record.levelno <= logging.INFO)
log.addHandler(message_handler)

error_handler = logging.StreamHandler(sys.stderr)
error_handler.setLevel(logging.WARNING)
log.addHandler(error_handler)


@unique
class ColorSpace(Enum):
    """Adobe Color Swatch - Color Space Ids."""

    RGB = (0, "RGB", True)
    HSB = (1, "HSB", True)
    CMYK = (2, "CMYK", True)
    PANTONE = (3, "Pantone matching system", False)
    FOCOLTONE = (4, "Focoltone colour system", False)
    TRUMATCH = (5, "Trumatch color", False)
    TOYO = (6, "Toyo 88 colorfinder 1050", False)
    LAB = (7, "Lab", False)
    GRAYSCALE = (8, "Grayscale", True)
    HKS = (10, "HKS colors", False)

    def __new__(cls, *args):
        obj = object.__new__(cls)
        obj._value_ = args[0]
        return obj

    def __init__(
        self,
        _: int,
        label: str | None = None,
        supported: bool = False,
    ):
        self.label = label
        self.supported = supported

    def __str__(self) -> str:
        return self.label if self.label is not None else "unknown"


class HexColor(NamedTuple):
    name: str
    color_space: ColorSpace
    color_hex: str


class RawColor(NamedTuple):
    name: str
    color_space: ColorSpace
    component_1: int = 0
    component_2: int = 0
    component_3: int = 0
    component_4: int = 0


def save_aco_file(colors_data: list[RawColor], file: BinaryIO) -> None:
    """Saves provided color data into a `.aco` file.

    Args:
        colors_data: list of `RawColor`s, were each of them contains the name,
            color space and four color components.
        file: handle to the `.aco` file to be saved.
    """
    try:
        # Version 1
        version = 1
        file.write(version.to_bytes(2, "big"))

        color_count = len(colors_data)
        file.write(color_count.to_bytes(2, "big"))

        for color_data in colors_data:
            color_space_id = color_data.color_space.value
            file.write(color_space_id.to_bytes(2, "big"))

            file.write(color_data.component_1.to_bytes(2, "big"))
            file.write(color_data.component_2.to_bytes(2, "big"))
            file.write(color_data.component_3.to_bytes(2, "big"))
            file.write(color_data.component_4.to_bytes(2, "big"))

        # Version 2
        version = 2
        file.write(version.to_bytes(2, "big"))

        color_count = len(colors_data)
        file.write(color_count.to_bytes(2, "big"))

        for color_data in colors_data:
            color_space_id = color_data.color_space.value
            file.write(color_space_id.to_bytes(2, "big"))

            file.write(color_data.component_1.to_bytes(2, "big"))
            file.write(color_data.component_2.to_bytes(2, "big"))
            file.write(color_data.component_3.to_bytes(2, "big"))
            file.write(color_data.component_4.to_bytes(2, "big"))

            # + 1 is for termination character
            color_name_length = len(color_data.name) + 1
            file.write(color_name_length.to_bytes(4, "big"))

            color_name_bytes = bytes(color_data.name, "utf-16-be")
            file.write(color_name_bytes)

            termination_char = 0
            file.write(termination_char.to_bytes(2, "big"))

    finally:
        file.close()
