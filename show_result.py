import cv2 as cv
from matplotlib import pyplot as plt


def show_result(result_path, image_path):
    img = cv.imread(image_path)  # b,g,r
    # img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # img = cv.GaussianBlur(img, (3, 3), 0)
    # img = cv.adaptiveThreshold(
    #     img, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 11, 10)

    # img = cv.cvtColor(img, cv.COLOR_GRAY2RGB)

    with open(result_path, "r") as f:
        for line in f.readlines():
            row, col = line.split(",")
            row, col = int(row), int(col)
            colors = [0, 0, 255]
            for idx, color in enumerate(colors):
                img[row, col, idx] = color
                img[row+1, col, idx] = color
                img[row, col+1, idx] = color
                img[row-1, col, idx] = color
                img[row, col-1, idx] = color

    plt.imshow(img)
    plt.xticks([]), plt.yticks([])
    plt.show()

    cv.imwrite("result.png", img)


if __name__ == "__main__":
    show_result("c/result.csv", "image.png")
