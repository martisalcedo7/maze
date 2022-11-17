import numpy as np
from copy import deepcopy
import cv2 as cv
from PIL import Image


def normalize(array):
    array = (array / array.max())
    array = 1 - array
    return array


def map_from_image(path):
    im = Image.open("test.bmp").convert("L")
    map = np.array(im)
    map = normalize(map)
    map = map.astype(np.uint8)
    # reads an image in the BGR format
    # img = cv.imread("test_2.png")
    # img_blue, img_green, img_red = cv.split(img)
    # gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # print(gray)
    # gray = normalize(gray)
    # r, c = gray.shape
    # left = gray[:, 0:(int(c/2)-1)]
    # right = gray[:, (c-int(c/2)):-1]
    # gray = np.where(gray > 0.5, 1.0, 0.0)
    # print(gray)
    # cv.imshow("Display window", left)
    # k = cv.waitKey(0)
    # cv.imshow("Display window", right)
    # k = cv.waitKey(0)
    # img = Image.fromarray(map_green, 'L')
    # img.show()
    # img = Image.fromarray(map_blue, 'L')
    # img.show()

    # t = 0.5
    # map = map < t
    # map = np.where(map < t, 1, 0)
    # # img = Image.fromarray(map, 'L')
    # # img.show()
    map[0, -1] = 3
    map[-1, 0] = 2
    return map


if __name__ == "__main__":
    map = map_from_image("test.bmp")
