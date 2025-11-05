import cv2
import numpy as np
from PIL import Image


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
    img_back = cv2.merge((one, one, one, alpha))

    return _cv2pil(img_front), _cv2pil(img_back)
