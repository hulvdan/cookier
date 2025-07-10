import os
import shutil
import tempfile
from dataclasses import dataclass, field
from enum import Enum
from math import radians
from pathlib import Path
from typing import Any, Callable, Iterable, TypeVar

import pyjson5 as json
import pytest
import yaml
from bf_lib import (
    ART_DIR,
    FLATBUFFERS_GENERATED_DIR,
    FLATBUFFERS_SRC_DIR,
    FLATC_PATH,
    GAME_DIR,
    HANDS_GENERATED_DIR,
    PIXEL_SCALE_MULTIPLIER,
    RESOURCES_DIR,
    SRC_DIR,
    TEMP_DIR,
    log,
    recursive_mkdir,
    replace_double_spaces,
    run_command,
    timing,
    timing_mark,
)
from bf_svg import (
    OutlineType_DEFAULT,
    OutlineType_JUST_OUTLINE,
    OutlineType_JUST_SCALE,
    OutlineType_JUST_SHADOW,
    make_smooth_outlined_version_of,
)
from PIL import Image, ImageEnhance

Breakable: TypeVar = list[dict]
Line: TypeVar = list[Breakable]


def sum_tuples(a, b):
    assert len(a) == 2
    assert len(b) == 2
    return (a[0] + b[0], a[1] + b[1])


def texture_ids_recursive_transform(
    gamelib_recursed, transform_texture_id, transform_texture_ids_list
) -> None:
    if not isinstance(gamelib_recursed, dict):
        return

    for key, value in gamelib_recursed.items():
        if isinstance(value, dict):
            texture_ids_recursive_transform(
                value, transform_texture_id, transform_texture_ids_list
            )
        elif isinstance(key, str) and ("texture_ids" in key or "texture_id" in key):
            if "texture_ids" in key:
                assert isinstance(value, list)
                for v in value:
                    assert isinstance(v, str)
                transform_texture_ids_list(gamelib_recursed, key)
            elif "texture_id" in key:
                assert isinstance(value, str)
                transform_texture_id(gamelib_recursed, key)
        elif isinstance(value, list):
            for v in value:
                if isinstance(v, dict):
                    texture_ids_recursive_transform(
                        v, transform_texture_id, transform_texture_ids_list
                    )


def recursive_replace_transform(
    gamelib_recursed,
    key_postfix_single: str,
    key_postfix_list: str,
    codename_to_index: dict[str, int],
    *,
    root=True,
) -> list[str] | None:
    errors = None

    if not isinstance(gamelib_recursed, dict):
        return None

    for key, value in gamelib_recursed.items():
        if isinstance(value, dict):
            more_errors = recursive_replace_transform(
                value, key_postfix_single, key_postfix_list, codename_to_index, root=False
            )
            if more_errors:
                if not errors:
                    errors = more_errors
                else:
                    errors.extend(more_errors)

        elif isinstance(key, str) and (
            key.endswith((key_postfix_single, key_postfix_list))
        ):
            if key.endswith(key_postfix_list):
                assert isinstance(value, list)
                for i in range(len(value)):
                    assert isinstance(value[i], str), f"value: {value[i]}"
                    v = value[i]
                    try:
                        value[i] = codename_to_index[v]
                    except KeyError:
                        if not errors:
                            errors = []
                        errors.append(v)
            else:
                assert isinstance(value, str), f"value: {value[i]}"
                try:
                    gamelib_recursed[key] = codename_to_index[value]
                except KeyError:
                    if not errors:
                        errors = []
                    errors.append(value)

        elif isinstance(value, list):
            for v in value:
                if isinstance(v, dict):
                    more_errors = recursive_replace_transform(
                        v,
                        key_postfix_single,
                        key_postfix_list,
                        codename_to_index,
                        root=False,
                    )
                    if more_errors:
                        if not errors:
                            errors = more_errors
                        else:
                            errors.extend(more_errors)

    if root and errors:
        message = "recursive_replace_transform({}, {}):\nNot found:\n{}".format(
            key_postfix_single, key_postfix_list, "\n".join(errors)
        )
        raise AssertionError(message)

    return errors


def degrees_to_radians_recursive_transform(gamelib_recursed) -> None:
    if not isinstance(gamelib_recursed, dict):
        return

    keys_to_replace = [key for key in gamelib_recursed if key.endswith("_deg")]

    for key in keys_to_replace:
        new_key = key.replace("_deg", "_rad")
        value = gamelib_recursed.pop(key)
        assert isinstance(value, (int, float))
        gamelib_recursed[new_key] = radians(value)

    del keys_to_replace

    for value in gamelib_recursed.values():
        if isinstance(value, dict):
            degrees_to_radians_recursive_transform(value)

        elif isinstance(value, list):
            for v in value:
                if isinstance(v, dict):
                    degrees_to_radians_recursive_transform(v)


def index_default_minus_1(list_variable, value):
    try:
        return list_variable.index(value)
    except ValueError:
        return -1


def genenum(
    genline,
    name: str,
    values: Iterable[str],
    *,
    enum_type: str | None = None,
    add_count: bool = False,
    hex_values: bool = False,
    override_values: Iterable[Any] | None = None,
    enumerate_values: bool = False,
    add_to_string: bool = False,
    comments: list[str] | None = None,
) -> None:
    assert not (hex_values and enumerate_values)
    assert not (override_values and enumerate_values)

    if add_count or hex_values:
        assert add_count != hex_values

    string = f"enum {name}"
    if enum_type:
        string += f" : {enum_type}"
    string += " {"
    genline(string)

    def genline_with_comment(line: str, i: int) -> str:
        if comments and comments[i]:
            line += "  // " + comments[i]
        genline(line)

    if hex_values:
        for i, value in enumerate(values):
            genline_with_comment("  {}_{} = {},".format(name, value, hex(2**i)), i)
    elif override_values:
        i = 0
        for value, value2 in zip(values, override_values):
            genline_with_comment("  {}_{} = {},".format(name, value, value2), i)
            i += 1
    else:
        for i, value in enumerate(values):
            if enumerate_values:
                genline_with_comment("  {}_{} = {},".format(name, value, i), i)
            else:
                genline_with_comment("  {}_{},".format(name, value), i)

    if add_count:
        genline("  {}_COUNT,".format(name))

    genline("};\n")

    if add_to_string:
        genline(f"const char* {name}ToString({name} type) {{")
        genline("  ASSERT(type >= 0);")
        if add_count:
            genline(f"  ASSERT(type <= {len(values)});")
        else:
            genline(f"  ASSERT(type < {len(values)});")
        genline("  static constexpr const char* strings[] = {")
        for value in values:
            genline(f'    "{name}_{value}",')
        if add_count:
            genline(f'    "{name}_COUNT",')
        genline("  };")
        genline("  return strings[type];")
        genline("};\n")


@timing
def convert_gamelib_json_to_binary(
    texture_name_2_id: dict[str, int], genline, genline_common, atlas_data
) -> None:
    with open(GAME_DIR / "gamelib.yaml", encoding="utf-8") as in_file:
        gamelib = yaml.safe_load(in_file)

    gamelib["atlas"] = atlas_data

    degrees_to_radians_recursive_transform(gamelib)

    COLORS = [
        ("white", "0xffffff"),
        ("black", "0x000000"),
        ("red", "0xff0000"),
        ("textColor", "0xe0d3c8"),
        ("textDisabledColor", "0xa57b73"),
        ("textErrorColor", "0xf05b5b"),
        ("backgroundColorDark", "0x452b3f"),
        ("backgroundColor", "0x56546e"),
        ("backgroundHoveredColor", "0x8a5865"),
        ("backgroundSelectedColor", "0x5479b0"),
        ("backgroundDisabledColor", "0x56546e"),
        ("backgroundHighlightedColor", "0xe08d51"),
        ("backgroundSuccessColor", "0x2c5e3b"),
        ("palLight", "0xf5eeb0"),
        ("palYellowOrange", "0xfabf61"),
        ("palOrange", "0xe08d51"),
        ("palBrown", "0x8a5865"),
        ("palDark", "0x452b3f"),
        ("palDarkGreen", "0x2c5e3b"),
        ("palGreen", "0x609c4f"),
        ("palLime", "0xc6cc54"),
        ("palSkyblue", "0x78c2d6"),
        ("palBlue", "0x5479b0"),
        ("palGray", "0x56546e"),
        ("palSlateGray", "0x839fa6"),
        ("palLightGray", "0xe0d3c8"),
        ("palRed", "0xf05b5b"),
        ("palPlum", "0x8f325f"),
        ("palPink", "0xeb6c98"),
    ]
    COLOR_NAMES = [i[0] for i in COLORS]
    if 1:
        genline_common("#define COLORS_TABLE \\")
        for i, color in enumerate(COLORS):
            line = f"  X({color[0]}, {color[1]})"
            if i != len(COLORS) - 1:
                line += " \\"
            genline_common(line)
        genline_common("")

    # Создание `gamelib.bin`.
    intermediate_path = TEMP_DIR / "gamelib.intermediate.jsonc"
    intermediate_path.write_text(json.dumps(gamelib, ensure_ascii=True, indent=4))
    run_command(
        [
            FLATC_PATH,
            "-o",
            TEMP_DIR,
            "-b",
            FLATBUFFERS_SRC_DIR / "bf_gamelib.fbs",
            intermediate_path,
        ]
    )

    intermediate_binary_path = str(intermediate_path).rsplit(".", 1)[0] + ".bin"
    shutil.move(intermediate_binary_path, RESOURCES_DIR / "gamelib.bin")


@timing
def make_atlas(path: Path) -> tuple[dict[str, int], dict]:
    assert str(path).endswith(".ftpp")

    filename_wo_extension = path.name.rsplit(".", 1)[0]

    shutil.copytree(ART_DIR / "to_do_nothing", TEMP_DIR / "art", dirs_exist_ok=True)

    cache_filepath = TEMP_DIR / ".atlas.cache"

    old_cache_value = -1
    if cache_filepath.exists():
        old_cache_value = int(cache_filepath.read_text())

    cache_value = 0
    for filepath in Path(TEMP_DIR / "art").rglob("*.png"):
        cache_value = hash(cache_value + filepath.stat().st_mtime_ns)

    if cache_value == old_cache_value:
        log.info("Skipped atlas generation - no images changed")
        timing_mark("skipped png generation")
    else:
        # Генерируем атлас из .ftpp файла. Создаются .json спецификация и .png текстура.
        log.info("Generating atlas...")
        run_command("free-tex-packer-cli --project {}".format(path))

        cache_filepath.write_text(str(cache_value))

    # Подгоняем спецификацию под наш формат.
    json_path = TEMP_DIR / (filename_wo_extension + ".json")
    with open(json_path) as json_file:
        json_data = json.load(json_file)

    found_textures: set[str] = set()

    textures: list[Any] = []
    for name_, data in json_data["frames"].items():
        name = name_.removeprefix("art/")

        assert name not in found_textures
        found_textures.add(name)

        outlined = (ART_DIR / "to_outline" / (name + ".png")).exists() or (
            "__outlined" in name
        )
        baseline = data["frame"]["h"]
        if outlined:
            baseline -= 20

        texture_data = {
            "id": -1,
            "debug_name": name,
            "size_x": data["frame"]["w"],
            "size_y": data["frame"]["h"],
            "atlas_x": data["frame"]["x"],
            "atlas_y": data["frame"]["y"],
            "scaled": (
                outlined
                or (ART_DIR / "to_scale" / (name + ".png")).exists()
                or (ART_DIR / "decorations" / "tiles" / (name + ".png")).exists()
                or (
                    ART_DIR
                    / "to_bone"
                    / (
                        name.replace("__shadowed", "")
                        .replace("__outlined", "")
                        .replace("__scaled", "")
                        + ".png"
                    )
                ).exists()
            ),
            "baseline": baseline,
        }
        textures.append(texture_data)

    texture_name_2_id = {}
    textures.sort(key=lambda x: x["debug_name"])

    for i in range(len(textures)):
        textures[i]["id"] = i
        name = textures[i]["debug_name"]
        texture_name_2_id[name] = i

    # Копируем в resources.
    shutil.copyfile(
        TEMP_DIR / (filename_wo_extension + ".png"),
        RESOURCES_DIR / (filename_wo_extension + ".png"),
    )

    return texture_name_2_id, {
        "textures": textures,
        "size_x": json_data["meta"]["size"]["w"],
        "size_y": json_data["meta"]["size"]["h"],
    }


@timing
def remove_intermediate_generation_files() -> None:
    for filepath in TEMP_DIR.rglob("*.intermediate*"):
        filepath.unlink()


def transform_to_texture_indexes_list(
    data: dict[str, Any],
    key: str,
    texture_name_2_index: dict[str, int],
) -> None:
    textures = data[key]
    assert isinstance(textures, list)

    for i, texture_name in enumerate(textures):
        if texture_name.lower() == "undefined":
            textures[i] = -1
        else:
            assert texture_name in texture_name_2_index, (
                f"Texture '{texture_name}' not found in atlas!"
            )
            textures[i] = texture_name_2_index[texture_name]


def transform_to_texture_index(
    data: dict[str, Any],
    key: str,
    texture_name_2_index: dict[str, int],
) -> None:
    texture_name = data[key]
    assert isinstance(texture_name, str)

    if texture_name.lower() == "undefined":
        data[key] = -1
        return

    assert texture_name in texture_name_2_index, (
        f"Texture '{texture_name}' not found in atlas!"
    )

    data[key] = texture_name_2_index[texture_name]


def listfiles_with_hashes_in_dir(path: str | Path) -> dict[str, int]:
    filepath_string_to_hash = {}

    for filepath in Path(path).rglob("*"):
        with open(filepath, "rb") as in_file:
            filepath_string_to_hash[str(filepath.relative_to(path))] = hash(
                in_file.read()
            )

    return filepath_string_to_hash


@timing
def generate_flatbuffer_files():
    hashes_for_msbuild = listfiles_with_hashes_in_dir(FLATBUFFERS_GENERATED_DIR)

    # Генерируем cpp файлы из FlatBuffer (.fbs) файлов.
    flatbuffer_files = list(SRC_DIR.rglob("*.fbs"))

    with tempfile.TemporaryDirectory() as td:

        def gen(filepaths, *args):
            command = [
                FLATC_PATH,
                *args,
                "-o",
                td,
                "--cpp",
                *list(filepaths),
            ]

            run_command(command)

        to_reflect = "bf_boner.fbs"
        gen(
            (i for i in flatbuffer_files if i.name == to_reflect),
            "--gen-object-api",
            "--reflect-names",
        )
        gen(i for i in flatbuffer_files if i.name != to_reflect)

        for file, file_hash in listfiles_with_hashes_in_dir(td).items():
            # Костыль, чтобы MSBuild не ребилдился каждый раз.
            if file_hash != hashes_for_msbuild.get(file):
                shutil.copyfile(Path(td) / file, FLATBUFFERS_GENERATED_DIR / file)


@timing
def scale_images(*filepaths) -> None:
    output_directory = TEMP_DIR / "art"

    skipped = 0

    for filepath in filepaths:
        save_to = output_directory / filepath.relative_to(filepath.parent.parent)

        s1 = filepath.stat()

        if save_to.exists():
            s2 = save_to.stat()
            if s1.st_mtime_ns == s2.st_mtime_ns:
                skipped += 1
                continue

        image = Image.open(filepath)
        if image.mode != "RGBA":
            image = image.convert("RGBA")

        image = image.resize(
            (
                PIXEL_SCALE_MULTIPLIER * image.size[0],
                PIXEL_SCALE_MULTIPLIER * image.size[1],
            ),
            Image.Resampling.NEAREST,
        )

        recursive_mkdir(save_to)
        image.save(save_to)
        os.utime(save_to, ns=(s1.st_atime_ns, s1.st_mtime_ns))

    if skipped:
        processed = len(filepaths) - skipped
        if processed:
            timing_mark("processed {}/{}".format(processed, len(filepaths)))

        timing_mark("skipped {}/{}".format(skipped, len(filepaths)))


@timing
def process_outline_images() -> None:
    cache_filepath = TEMP_DIR / ".outline.cache"

    cache = {}
    if cache_filepath.exists():
        with open(cache_filepath) as in_file:
            cache = json.load(in_file)

    files: list[tuple[Path, tuple[int, ...]]] = [
        (filepath, (OutlineType_DEFAULT,))
        for filepath in (ART_DIR / "to_outline").rglob("*.png")
    ]
    files.extend(
        (
            filepath,
            (OutlineType_JUST_SHADOW, OutlineType_JUST_OUTLINE, OutlineType_JUST_SCALE),
        )
        for filepath in (ART_DIR / "to_bone").rglob("*.png")
    )

    visited = set()

    processed = 0
    skipped = 0

    for filepath, outline_types in files:
        p = filepath.relative_to(ART_DIR)
        visited.add(str(p))

        s = filepath.stat()

        if str(p) in cache:
            if cache[str(p)] == s.st_mtime_ns:
                skipped += 1
                continue

        log.debug("Outlining '{}'...".format(p))
        make_smooth_outlined_version_of(filepath, outline_types)
        cache[str(p)] = s.st_mtime_ns
        os.utime(ART_DIR / p, ns=(s.st_atime_ns, s.st_mtime_ns))

        processed += 1

    total = processed + skipped
    if processed:
        timing_mark("processed {}/{}".format(processed, total))

    timing_mark("skipped {}/{}".format(skipped, total))

    cache = {k: v for k, v in cache.items() if k in visited}
    cache_filepath.write_text(json.dumps(cache))


@timing
def check_no_image_names_duplicated(folders: list[str]) -> None:
    existing_images = set()

    for folder_name in folders:
        base_dir = ART_DIR / folder_name

        for filepath in base_dir.rglob("*.png"):
            relative_filename = str(filepath.relative_to(base_dir))
            assert relative_filename not in existing_images, (
                "Found image with the same name! {}".format(relative_filename)
            )
            existing_images.add(relative_filename)


@timing
def check_images_in_temp_exist_in_to_process(folders: list[str]) -> None:
    temp_art_dir = TEMP_DIR / "art"
    temp_image_paths = set(
        filepath.relative_to(temp_art_dir) for filepath in temp_art_dir.rglob("*.png")
    )

    filepaths_per_folder = []
    for folder_name in folders:
        base_dir = ART_DIR / folder_name
        filepaths = set(
            filepath.relative_to(base_dir) for filepath in base_dir.rglob("*.png")
        )

        # NOTE: Файлы в `to_bone` преобразуются в outline и shadow.
        if folder_name == "to_bone":
            to_bone_filepaths = []
            for filepath in filepaths:
                for suffix in ("__shadowed.png", "__outlined.png", "__scaled.png"):
                    to_bone_filepaths.append(filepath.parent / (filepath.stem + suffix))

            filepaths_per_folder.append(to_bone_filepaths)
        else:
            filepaths_per_folder.append(filepaths)

    excessive_images = []
    for filepath in temp_image_paths:
        exists = False
        for filepaths in filepaths_per_folder:
            if filepath in filepaths:
                exists = True
                break

        if not exists:
            excessive_images.append(filepath.as_posix())

    if excessive_images:
        message = "Excessive images found!\n{}".format(
            "\n".join("{}) {}".format(i + 1, v) for i, v in enumerate(excessive_images))
        )
        raise AssertionError(message)


NOT_SET_STRING = "NOT SET"


@dataclass
class CutsceneNPCElementsParsed:
    npc: str
    elements: list[tuple[str, str]] = field(default_factory=list)


@dataclass
class CutsceneParsed:
    codename: str
    level_id: str = NOT_SET_STRING
    npcs: list[tuple[str, str, str]] = field(default_factory=list)
    npc_actions: list[CutsceneNPCElementsParsed] = field(default_factory=list)


class CutsceneLineType(Enum):
    REPLICA = 1
    CUTSCENE = 2
    NPC = 3
    OTHER = 4


def dummy_line_callback(_: CutsceneLineType, _2: str) -> None:
    pass


def parse_cutscenes(
    line_callback: Callable[[CutsceneLineType, str], None] = dummy_line_callback,
) -> list[CutsceneParsed]:
    result: list[CutsceneParsed] = []

    with open(CUTSCENES_TXT_PATH, encoding="utf-8") as in_file:
        lines = in_file.readlines()

    next_line_index = 0

    def pop_line() -> str:
        nonlocal next_line_index
        line = lines[next_line_index]
        next_line_index += 1
        return line

    current_cutscene: CutsceneParsed | None = None
    current_npc: CutsceneNPCElementsParsed | None = None
    current_cutscene_npc_codenames = []

    already_used_cutscene_codenames: list[str] = []

    line_callback(CutsceneLineType.OTHER, pop_line())
    line_callback(CutsceneLineType.OTHER, pop_line())

    while next_line_index < len(lines):
        original_line = pop_line()
        line = original_line

        # Cutscene.
        if line.startswith("["):
            line_callback(CutsceneLineType.CUTSCENE, original_line)

            cutscene_codename = line[1:].split("]", 1)[0]
            assert cutscene_codename not in already_used_cutscene_codenames, (
                "Cutscene codename duplicated '{}'!".format(cutscene_codename)
            )

            level_line = pop_line()
            line_callback(CutsceneLineType.OTHER, level_line)

            current_cutscene = CutsceneParsed(
                codename=cutscene_codename, level_id=level_line.strip()
            )
            result.append(current_cutscene)

            while True:
                cutscene_npc_line = pop_line()

                if not cutscene_npc_line.strip():
                    line_callback(CutsceneLineType.OTHER, cutscene_npc_line)
                    break

                line_callback(CutsceneLineType.NPC, cutscene_npc_line)

                npc, position_string, looks_at_string = replace_double_spaces(
                    cutscene_npc_line.strip()
                ).split(" ")

                current_cutscene.npcs.append((npc, position_string, looks_at_string))

            current_cutscene_npc_codenames.clear()
            current_cutscene_npc_codenames.extend(i[0] for i in current_cutscene.npcs)

            continue

        if not line.strip():
            line_callback(CutsceneLineType.OTHER, original_line)
            continue

        # NPC codename.
        if not line[0].isspace():
            npc = line.strip()
            assert npc in current_cutscene_npc_codenames, (
                "NPC '{}' not found in cutscene '{}' acting npcs {}!".format(
                    npc, current_cutscene.codename, current_cutscene_npc_codenames
                )
            )

            if current_npc is not None:
                assert current_npc.elements

            current_npc = CutsceneNPCElementsParsed(npc=npc)
            current_cutscene.npc_actions.append(current_npc)

            line_callback(CutsceneLineType.NPC, original_line)

            continue

        # NPC elements.
        else:
            line = replace_double_spaces(line.strip())

            # NPC game action.
            if line[0] == "@":
                action_codename, arguments = line.split(" ", 1)
                current_npc.elements.append((action_codename[1:], arguments))

                line_callback(CutsceneLineType.OTHER, original_line)

            # NPC replica.
            else:
                current_npc.elements.append(
                    ("REPLICA", line.rsplit("@id:", 1)[0].strip())
                )

                line_callback(CutsceneLineType.REPLICA, original_line)

            continue

    return result


def do_cutscenes(gamelib, ldtk_narrative_points, genline) -> dict[str, int]:
    cutscene_system = gamelib["cutscene_system"]

    all_game_npcs = [i.pop("codename") for i in cutscene_system["npcs"]]
    assert len(set(all_game_npcs)) == len(all_game_npcs)
    genenum(genline, "NPCType", all_game_npcs, add_count=True)

    parsed_cutscenes = parse_cutscenes()

    cutscenes = [i.codename for i in parsed_cutscenes]
    genenum(genline, "CutsceneType", cutscenes, add_count=True, enumerate_values=True)

    directions = {
        "RIGHT_BOTTOM": (10, 1),
        "RIGHT_TOP": (10, -1),
        "BOTTOM_RIGHT": (1, 10),
        "TOP_RIGHT": (1, -10),
        "LEFT_BOTTOM": (-10, 1),
        "LEFT_TOP": (-10, -1),
        "BOTTOM_LEFT": (-1, 10),
        "TOP_LEFT": (-1, -10),
        "RIGHT": (1, 0),
        "LEFT": (-1, 0),
    }

    ELEMENT_TYPE_TO_INDEX = {
        "REPLICA": 0,
        "MOVE_TO": 1,
        "ASYNC_MOVE_TO": 2,
        "ASYNC_LOOK_AT": 3,
        "CONSUME": 4,
        "ASYNC_WARLOCK_TELEPORT": 5,
        "WARLOCK_TELEPORT": 6,
        "TRANSITION": 7,
        "IDLE": 8,
        "ASYNC_IDLE": 9,
    }

    def parse_pos(level_id: str, string: str) -> tuple[int, int]:
        if string.startswith("["):
            return tuple(json.loads(string))

        return ldtk_narrative_points[level_id][string]

    cutscene_npc_codenames: list[str] = []
    npc_positions: dict[str, tuple[int, int]] = {}

    def parse_looks_at(
        level_id: str, string, looks_from: tuple[int, int]
    ) -> tuple[int, int]:
        if string.startswith("["):
            p = sum_tuples(looks_from, tuple(json.loads(string)))
        elif string in directions:
            p = sum_tuples(looks_from, directions[string])
        elif string in cutscene_npc_codenames:
            p = npc_positions[string]
        else:
            p = ldtk_narrative_points[level_id][string]

        return sum_tuples((0.5, 0.5), p)

    transformed_cutscenes = []
    gamelib["cutscene_system"]["cutscenes"] = transformed_cutscenes

    gamelib["localizations"] = [
        {
            "texture_id": "ui/lang_russian",
            "strings": gamelib.pop("localization"),
            "replicas": [],
        }
    ]

    next_replica_id = 0

    for cutscene in parsed_cutscenes:
        transformed_cutscene = {
            "codename": cutscene.codename,
            "level_id": cutscene.level_id,
            "camera_pos": ldtk_narrative_points[cutscene.level_id]["camera"],
            "elements": [],
        }

        transformed_cutscenes.append(transformed_cutscene)

        npcs_to_check = [i[0] for i in cutscene.npcs]
        npcs_to_check.extend(i.npc for i in cutscene.npc_actions)

        for npc in npcs_to_check:
            assert npc in all_game_npcs, "Not found NPC '{}' in all_game_npcs: {}".format(
                npc, all_game_npcs
            )

        for npc_actions in cutscene.npc_actions:
            for element in npc_actions.elements:
                assert element[0] in ELEMENT_TYPE_TO_INDEX, (
                    "Not found element '{}' in {}".format(
                        element[0], list(ELEMENT_TYPE_TO_INDEX.keys())
                    )
                )

        npc_positions.clear()
        cutscene_npc_codenames.clear()
        for npc_data in cutscene.npcs:
            npc_positions[npc_data[0]] = parse_pos(cutscene.level_id, npc_data[1])
            cutscene_npc_codenames.append(npc_data[0])

        transformed_cutscene["npcs"] = [
            {
                "npc": all_game_npcs.index(i[0]),
                "pos": parse_pos(cutscene.level_id, i[1]),
                "looks_at": parse_looks_at(cutscene.level_id, i[2], npc_positions[i[0]]),
            }
            for i in cutscene.npcs
        ]

        for npc_actions in cutscene.npc_actions:
            for element, arguments_ in npc_actions.elements:
                data = {
                    "npc": all_game_npcs.index(npc_actions.npc),
                    "type": ELEMENT_TYPE_TO_INDEX[element],
                }
                transformed_cutscene["elements"].append(data)

                if element == "REPLICA":
                    data["replica_id"] = next_replica_id
                    next_replica_id += 1
                    continue

                arguments = arguments_.split(" ")

                if element in ("MOVE_TO", "ASYNC_MOVE_TO"):
                    assert len(arguments) == 1
                    data["pos"] = parse_pos(cutscene.level_id, arguments[0])
                    npc_positions[npc_actions.npc] = data["pos"]

                elif element == "ASYNC_LOOK_AT":
                    assert len(arguments) == 1
                    data["pos"] = parse_looks_at(
                        cutscene.level_id, arguments[0], npc_positions[npc_actions.npc]
                    )

                elif element == "CONSUME":
                    assert len(arguments) == 1
                    data["item_id"] = arguments[0]

                elif element in ("ASYNC_WARLOCK_TELEPORT", "WARLOCK_TELEPORT"):
                    assert len(arguments) == 3
                    assert arguments[0] in cutscene_npc_codenames
                    data["acting_on_npc_type"] = all_game_npcs.index(arguments[0])
                    data["pos"] = parse_pos(cutscene.level_id, arguments[1])

                    data["pos2"] = parse_looks_at(
                        cutscene.level_id, arguments[0], data["pos"]
                    )
                    npc_positions[npc_actions.npc] = data["pos"]

                elif element == "TRANSITION":
                    assert len(arguments) == 2
                    assert arguments[0] in ("CUTSCENE", "GAMEPLAY")

                    is_cutscene = arguments[0] == "CUTSCENE"
                    if is_cutscene:
                        data["next_cutscene_id"] = arguments[1]
                    else:
                        data["next_level_id"] = arguments[1]

                elif element == "IDLE":
                    assert len(arguments) == 1
                    data["frames"] = int(arguments[0])

                elif element == "ASYNC_IDLE":
                    assert len(arguments) == 1
                    data["frames"] = int(arguments[0])

                else:
                    assert False

    return {codename: i for i, codename in enumerate(cutscenes)}


@timing
def do_generate() -> None:
    with (
        open(
            HANDS_GENERATED_DIR / "bf_codegen.cpp", "w", encoding="utf-8"
        ) as codegen_file,
        open(
            HANDS_GENERATED_DIR / "bf_codegen_common.cpp", "w", encoding="utf-8"
        ) as codegen_common_file,
    ):
        for file in (codegen_file, codegen_common_file):
            file.write(
                """// automatically generated by cli.py, do not modify
#pragma once

"""
            )

        def genline(value):
            codegen_file.write(value)
            codegen_file.write("\n")

        def genline_common(value):
            codegen_common_file.write(value)
            codegen_common_file.write("\n")

        generate_flatbuffer_files()

        remove_intermediate_generation_files()

        scale_images(
            *(ART_DIR / "to_scale").rglob("*.png"),
            *(ART_DIR / "decorations").rglob("*.png"),
        )
        process_outline_images()

        for filepath in ART_DIR.glob("darken__*.png"):
            image = Image.open(filepath)
            if image.mode != "RGBA":
                image = image.convert("RGBA")
            image = ImageEnhance.Brightness(image).enhance(0.66667)
            image.save(RESOURCES_DIR / filepath.name.removeprefix("darken__"))

        check_no_image_names_duplicated(
            (
                "to_do_nothing",
                "to_outline",
                "to_scale",
                "decorations",
            )
        )
        check_images_in_temp_exist_in_to_process(
            (
                "to_do_nothing",
                "to_outline",
                "to_scale",
                "to_bone",
                "decorations",
            )
        )

        texture_name_2_id, atlas_data = make_atlas(
            Path("src") / "assets" / "art" / "atlas.ftpp"
        )

        convert_gamelib_json_to_binary(
            texture_name_2_id, genline, genline_common, atlas_data
        )


SPECIAL_TOKENS = ("#",)


def consume_token(block: str, index: int) -> tuple[str, int]:
    assert " " not in block

    first_is_spec = False
    found_spec = False

    for i in range(index, len(block)):
        if block[i] in SPECIAL_TOKENS:
            found_spec = True
            if i == index:
                first_is_spec = True
            break

    if found_spec and not first_is_spec:
        i -= 1

    return block[index : i + 1], i + 1


@pytest.mark.parametrize(
    ("value", "result"),
    [
        (("aboba", 0), ("aboba", 5)),
        (("aboba.", 0), ("aboba.", 6)),
        (("#aboba#", 0), ("#", 1)),
        (("#aboba#", 1), ("aboba", 6)),
        (("#aboba#", 6), ("#", 7)),
        (("#aboba#.", 0), ("#", 1)),
        (("#aboba#.", 1), ("aboba", 6)),
        (("#aboba#.", 6), ("#", 7)),
        (("#aboba#.", 7), (".", 8)),
    ],
)
def test_consume_token(value, result):
    result_ = consume_token(*list(value))
    assert result_ == result


STYLE_ANGRY = 1
MAX_CHARS_PER_LINE = 128


def process_line(
    line: str,
    localization_codename_to_translation,
    *,
    use_single_characters: bool = False,
):
    assert "\n" not in line

    line = replace_double_spaces(line)
    line = line.replace("- ", "— ")

    blocks = []

    current_style = 0

    line = line.format(**localization_codename_to_translation)
    assert len(line) <= MAX_CHARS_PER_LINE

    blocks_to_process = line.split(" ")

    for block_to_process in blocks_to_process:
        block = []
        next_token_index = 0

        while next_token_index < len(block_to_process):
            token, next_token_index = consume_token(block_to_process, next_token_index)

            if token == "#":
                assert current_style in (0, STYLE_ANGRY)

                if current_style == 0:
                    current_style = STYLE_ANGRY
                elif current_style == STYLE_ANGRY:
                    current_style = 0

            else:
                if use_single_characters:
                    for character in token:
                        block.append({"style": current_style, "text": character})
                else:
                    block.append({"style": current_style, "text": token})

        blocks.append(block)

    return blocks


@pytest.mark.parametrize(
    ("value", "result"),
    [
        (
            "aboba",
            [[{"style": 0, "text": "aboba"}]],
        ),
        (
            "aboba.",
            [[{"style": 0, "text": "aboba."}]],
        ),
        (
            "#aboba#.",
            [
                [
                    {"style": 1, "text": "aboba"},
                    {"style": 0, "text": "."},
                ],
            ],
        ),
        (
            "this is good",
            [
                [{"style": 0, "text": "this"}],
                [{"style": 0, "text": "is"}],
                [{"style": 0, "text": "good"}],
            ],
        ),
        (
            "this. is good.",
            [
                [{"style": 0, "text": "this."}],
                [{"style": 0, "text": "is"}],
                [{"style": 0, "text": "good."}],
            ],
        ),
        (
            "this?? is good!!!?",
            [
                [{"style": 0, "text": "this??"}],
                [{"style": 0, "text": "is"}],
                [{"style": 0, "text": "good!!!?"}],
            ],
        ),
        (
            "this?? is #good#!!!?",
            [
                [{"style": 0, "text": "this??"}],
                [{"style": 0, "text": "is"}],
                [
                    {"style": 1, "text": "good"},
                    {"style": 0, "text": "!!!?"},
                ],
            ],
        ),
        (
            "{COMRADE1}",
            [
                [{"style": 0, "text": "Комрад1"}],
            ],
        ),
    ],
)
def test_process_line(value, result):
    result_ = process_line(value, {"COMRADE1": "Комрад1"})
    assert result_ == result
