import cv2
import numpy as np
from PIL import Image

# def _change_matrix_outline(input_mat, stroke_size: int):
#     # stroke_size = stroke_size - 1
#     mat = np.ones(input_mat.shape)
#     check_size = stroke_size + 1.0
#     mat[input_mat > check_size] = 0
#     border = (input_mat > stroke_size) & (input_mat <= check_size)
#     mat[border] = 1.0 - (input_mat[border] - stroke_size)
#     return mat
#
#
# def _change_matrix_shadow(input_mat, stroke_size: int):
#     mat = np.ones(input_mat.shape)
#     mat[input_mat > stroke_size] = 0
#     border = input_mat <= stroke_size
#
#     # mat[border] = (stroke_size - input_mat[border]) / stroke_size
#
#     # NOTE: Squared easing of shadow decay.
#     mat[border] = (
#         (stroke_size - input_mat[border])
#         * (stroke_size - input_mat[border])
#         / stroke_size
#         / stroke_size
#     )
#     return mat


def _cv2pil(cv_img):
    return Image.fromarray(cv_img.astype("uint8"))


def image_split(*, image: Image.Image) -> tuple[Image.Image, Image.Image]:
    img = np.asarray(image)
    h, w, _ = img.shape

    one = np.full((h, w), 255, np.uint8)

    r_img = img[:, :, 0:1]
    alpha = img[:, :, 3]
    front_alpha = cv2.min(r_img, alpha)

    img_front = cv2.merge((one, one, one, front_alpha))
    img_back = cv2.merge((one,one,one, alpha))

    return _cv2pil(img_front), _cv2pil(img_back)

    # padding = stroke_size
    # alpha = img[:, :, 3]
    # rgb_img = img[:, :, 0:3]
    # bigger_img = cv2.copyMakeBorder(
    #     rgb_img,
    #     padding,
    #     padding,
    #     padding,
    #     padding,
    #     cv2.BORDER_CONSTANT,
    #     value=(0, 0, 0, 0),
    # )
    # alpha = cv2.copyMakeBorder(  #  type: ignore  # noqa
    #     alpha, padding, padding, padding, padding, cv2.BORDER_CONSTANT, value=0
    # )
    # bigger_img = cv2.merge((bigger_img, alpha))
    # h, w, _ = bigger_img.shape

    # _, alpha_without_shadow = cv2.threshold(
    #     alpha, threshold, 255, cv2.THRESH_BINARY
    # )  # threshold=0 in photoshop
    # alpha_without_shadow = 255 - alpha_without_shadow
    # dist = cv2.distanceTransform(
    #     alpha_without_shadow, cv2.DIST_L2, cv2.DIST_MASK_5
    # )  # dist l1 : L1 , dist l2 : l2
    #
    # if is_shadow:
    #     stroked = _change_matrix_shadow(dist, stroke_size)
    # else:
    #     stroked = _change_matrix_outline(dist, stroke_size)
    #
    # stroke_b = np.full((h, w), color[2], np.uint8)
    # stroke_g = np.full((h, w), color[1], np.uint8)
    # stroke_r = np.full((h, w), color[0], np.uint8)
    # stroke_alpha = (stroked * color[3]).astype(np.uint8)
    #
    # stroke = cv2.merge((stroke_b, stroke_g, stroke_r, stroke_alpha))
    # stroke = _cv2pil(stroke)
    #
    # if blend_image_on_top:
    #     bigger_img = _cv2pil(bigger_img)
    #     stroke = Image.alpha_composite(stroke, bigger_img)  # type: ignore  # noqa
    #
    # return stroke
