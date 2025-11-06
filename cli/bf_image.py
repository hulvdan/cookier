import cv2
import numpy as np
from PIL import Image, ImageChops


def _change_matrix_outline(input_mat, stroke_size: int):
    # stroke_size = stroke_size - 1
    mat = np.ones(input_mat.shape)
    check_size = stroke_size + 1.0
    mat[input_mat > check_size] = 0
    border = (input_mat > stroke_size) & (input_mat <= check_size)
    mat[border] = 1.0 - (input_mat[border] - stroke_size)
    return mat


def _change_matrix_shadow(input_mat, stroke_size: int):
    mat = np.ones(input_mat.shape)
    mat[input_mat > stroke_size] = 0
    border = input_mat <= stroke_size

    # mat[border] = (stroke_size - input_mat[border]) / stroke_size

    # NOTE: Squared easing of shadow decay.
    mat[border] = (
        (stroke_size - input_mat[border])
        * (stroke_size - input_mat[border])
        / stroke_size
        / stroke_size
    )
    return mat


def _cv2pil(cv_img):
    return Image.fromarray(cv_img.astype("uint8"))


def outline(
    *,
    image: Image.Image,
    stroke_size: int,
    color: tuple[int, int, int, int],
    is_shadow: bool,
    threshold: int = 0,
    blend_image_on_top: bool = True,
) -> Image.Image:
    assert threshold >= 0

    img = np.asarray(image)
    h, w, _ = img.shape
    padding = stroke_size
    alpha = img[:, :, 3]
    rgb_img = img[:, :, 0:3]
    bigger_img = cv2.copyMakeBorder(
        rgb_img,
        padding,
        padding,
        padding,
        padding,
        cv2.BORDER_CONSTANT,
        value=(0, 0, 0, 0),
    )
    alpha = cv2.copyMakeBorder(  #  type: ignore  # noqa
        alpha, padding, padding, padding, padding, cv2.BORDER_CONSTANT, value=0
    )
    bigger_img = cv2.merge((bigger_img, alpha))
    h, w, _ = bigger_img.shape

    _, alpha_without_shadow = cv2.threshold(
        alpha, threshold, 255, cv2.THRESH_BINARY
    )  # threshold=0 in photoshop
    alpha_without_shadow = 255 - alpha_without_shadow
    dist = cv2.distanceTransform(
        alpha_without_shadow, cv2.DIST_L2, cv2.DIST_MASK_5
    )  # dist l1 : L1 , dist l2 : l2

    if is_shadow:
        stroked = _change_matrix_shadow(dist, stroke_size)
    else:
        stroked = _change_matrix_outline(dist, stroke_size)

    stroke_b = np.full((h, w), color[2], np.uint8)
    stroke_g = np.full((h, w), color[1], np.uint8)
    stroke_r = np.full((h, w), color[0], np.uint8)
    stroke_alpha = (stroked * color[3]).astype(np.uint8)

    stroke = cv2.merge((stroke_b, stroke_g, stroke_r, stroke_alpha))
    stroke = _cv2pil(stroke)
    if blend_image_on_top:
        bigger_img = _cv2pil(bigger_img)
        stroke = Image.alpha_composite(stroke, bigger_img)  # type: ignore  # noqa
    return stroke


def extract_white(grayscale_image: Image.Image) -> Image.Image:
    img = np.asarray(grayscale_image)
    h, w, _ = img.shape
    one = np.full((h, w), 255, np.uint8)
    img_r = img[:, :, 0]
    img_alpha = img[:, :, 3]
    img_front = cv2.merge((one, one, one, cv2.min(img_r, img_alpha)))
    return _cv2pil(img_front)


def extract_black(grayscale_image: Image.Image) -> Image.Image:
    img = np.asarray(grayscale_image)
    h, w, _ = img.shape
    one = np.full((h, w), 255, np.uint8)
    img_r = img[:, :, 0]
    img_alpha = img[:, :, 3]
    alpha = cv2.min(255 - img_r, img_alpha)
    out_img = cv2.merge((one, one, one, alpha))
    return _cv2pil(out_img)


def white(image: Image.Image) -> Image.Image:
    img = np.asarray(image)
    h, w, _ = img.shape
    one = np.full((h, w), 255, np.uint8)
    alpha = img[:, :, 3]
    out_img = cv2.merge((one, one, one, alpha))
    return _cv2pil(out_img)


def black(image: Image.Image) -> Image.Image:
    img = np.asarray(image)
    h, w, _ = img.shape
    one = np.full((h, w), 0, np.uint8)
    alpha = img[:, :, 3]
    out_img = cv2.merge((one, one, one, alpha))
    return _cv2pil(out_img)


def invert(image: Image.Image) -> Image.Image:
    return ImageChops.invert(image)


def remap(
    grayscale_image: Image.Image,
    black_to: tuple[int, int, int],
    white_to: tuple[int, int, int],
) -> Image.Image:
    img = np.asarray(grayscale_image)
    h, w, _ = img.shape
    img_r = img[:, :, 0]
    img_g = img[:, :, 1]
    img_b = img[:, :, 2]
    out_img = cv2.merge(
        (
            (255 - img_r) * (black_to[0] / 255) + (img_r * (white_to[0] / 255)),
            (255 - img_g) * (black_to[1] / 255) + (img_g * (white_to[1] / 255)),
            (255 - img_b) * (black_to[2] / 255) + (img_b * (white_to[2] / 255)),
            img[:, :, 3] * 1.0,
        )
    )
    return _cv2pil(out_img)


def remap_grayscale(
    grayscale_image: Image.Image, black_to: int, white_to: int
) -> Image.Image:
    return remap(
        grayscale_image, (black_to, black_to, black_to), (white_to, white_to, white_to)
    )
