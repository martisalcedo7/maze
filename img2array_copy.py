import numpy as np
import cv2 as cv
from array2header import array_to_header
from matplotlib import pyplot as plt

counter = 0
origin = []
target = []


def onMouse(event, c, r, flags, param):
    global counter
    global origin
    global target
    if event == cv.EVENT_LBUTTONDOWN:
        # draw circle here (etc...)
        if counter == 0:
            origin = [r, c]
            print('Origin: r = %d, c = %d' % (r, c))
        elif counter == 1:
            target = [r, c]
            print('Target: r = %d, c = %d' % (r, c))
        counter += 1


def map_from_image(path):
    # reads an image in the BGR format
    img = cv.imread(path)
    # Convert image into grayscale
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # Filter image
    gray = cv.GaussianBlur(gray, (3, 3), 0)
    # Scale image
    # scale_percent = 90  # percent of original size
    # width = int(img.shape[1] * scale_percent / 100)
    # height = int(img.shape[0] * scale_percent / 100)
    # dim = (width, height)
    # gray = cv.resize(gray, dim, interpolation=3)

    # Binarize image
    gray = cv.adaptiveThreshold(
        gray, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 25, 20)

    gray = cv.convertScaleAbs(gray)

    cv.imshow("Display window", gray)
    cv.setMouseCallback("Display window", onMouse)
    k = cv.waitKey(0)
    if k == ord("s"):
        cv.imwrite("image.png", gray)

    plt.imshow(gray, cmap='gray', vmin=0, vmax=255)
    plt.xticks([]), plt.yticks([])
    plt.show()

    # print(gray)

    map = (np.max(gray) - gray) / np.max(gray)
    map = map.astype(np.uint8)

    print(np.count_nonzero(map != 1))

    # t = 0.5
    # map = map < t
    # map = np.where(map < t, 1, 0)
    # # img = Image.fromarray(map, 'L')
    # # img.show()
    map[1722, 44] = 2
    map[41, 1008] = 3
    # global origin
    # global target
    # map[origin[0], origin[1]] = 2
    # map[target[0], target[1]] = 3
    return map


if __name__ == "__main__":
    map = map_from_image("test_2.png")
    # print(map)
    plt.imshow(map, cmap='gray', vmin=0, vmax=1)
    plt.xticks([]), plt.yticks([])
    plt.show()
    print(np.count_nonzero(map != 1))
    array_to_header(map)
