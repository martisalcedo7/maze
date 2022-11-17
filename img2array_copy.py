import numpy as np
import cv2 as cv
from array2header import array_to_header


def onMouse(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        # draw circle here (etc...)
        print('x = %d, y = %d' % (x, y))


def map_from_image(path):
    # reads an image in the BGR format
    img = cv.imread(path)
    # Convert image into grayscale
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    # Filter image
    gray = cv.GaussianBlur(gray, (3, 3), 0)
    # Get image edges
    ddepth = cv.CV_8U
    kernel_size = 3
    gray = cv.Laplacian(gray, ddepth, ksize=kernel_size)
    # Reverse image
    gray = np.max(gray) - gray
    # Scale image
    scale_percent = 60  # percent of original size
    width = int(img.shape[1] * scale_percent / 100)
    height = int(img.shape[0] * scale_percent / 100)
    dim = (width, height)
    gray = cv.resize(gray, dim, interpolation=3)
    # Binarize image
    ret, gray = cv.threshold(gray, 180, 255, cv.THRESH_BINARY)

    gray = cv.convertScaleAbs(gray)
    print(np.max(gray))

    cv.imshow("Display window", gray)
    cv.setMouseCallback("Display window", onMouse)
    cv.waitKey(0)

    print(gray)

    map = (np.max(gray) - gray) / np.max(gray)
    map = map.astype(np.uint8)

    print(np.max(map))

    print(map)

    cv.imshow("Display window", map)
    cv.setMouseCallback("Display window", onMouse)
    cv.waitKey(0)

    # t = 0.5
    # map = map < t
    # map = np.where(map < t, 1, 0)
    # # img = Image.fromarray(map, 'L')
    # # img.show()
    map[1021, 38] = 2
    map[24, 607] = 3
    # map[18, 404] = 3
    # map[691, 22] = 2
    return map


if __name__ == "__main__":
    map = map_from_image("test_2.png")
    array_to_header(map)
