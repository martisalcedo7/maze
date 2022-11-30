import cv2 as cv
from matplotlib import pyplot as plt
import numpy as np


def show_result(result_path, img_binarized, origin, target):

    try:
        with open(result_path, "r") as f:
            # img = cv.imread(image_path)  # b,g,r
            # if img is None:
            #     raise FileExistsError
            img = cv.cvtColor(img_binarized, cv.COLOR_GRAY2RGB)
            img_with_path = img.copy()

            # img_bin = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
            # img_bin = cv.GaussianBlur(img_bin, (3, 3), 0)
            # img_bin = cv.adaptiveThreshold(
            #     img_bin, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 25, 20)
            # img_bin = cv.cvtColor(img_bin, cv.COLOR_GRAY2RGB)

            # shape = img.shape
            # path_size = int(0.005*min(shape[0], shape[1]))
            # if path_size <= 0:
            #     path_size = 1
            # print(path_size)

            for line in f.readlines():
                row, col = line.split(",")
                row, col = int(row), int(col)
                colors = (255, 0, 0)
                img_with_path = cv.circle(
                    img_with_path, (col, row), 2, colors, -1)
                # for idx, color in enumerate(colors):
                #     img[row, col, idx] = color
                #     img[row+1, col, idx] = color
                #     img[row, col+1, idx] = color
                #     img[row-1, col, idx] = color
                #     img[row, col-1, idx] = color

            img_with_path = cv.circle(
                img_with_path, (origin[1], origin[0]), 5, (0, 255, 0), -1)
            img_with_path = cv.circle(
                img_with_path, (target[1], target[0]), 5, (0, 0, 255), -1)

            final_img = cv.bitwise_and(img_with_path, img)
            plt.imshow(final_img)
            plt.xticks([]), plt.yticks([])
            plt.show()
            cv.imwrite("result.png", cv.cvtColor(final_img, cv.COLOR_RGB2BGR))

    except FileNotFoundError:
        print("Result file not found!")
    except FileExistsError:
        print("Image not found!")


if __name__ == "__main__":
    show_result("c/result.csv", "test_2.png")
