import cv2 as cv
from matplotlib import pyplot as plt

img = cv.imread("test_2.png")  # b,g,r
img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
img = cv.GaussianBlur(img, (3, 3), 0)
img = cv.adaptiveThreshold(
    img, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 11, 10)

img = cv.cvtColor(img, cv.COLOR_GRAY2RGB)

result = []
with open("c/result.txt", "r") as f:
    for line in f.readlines():
        row = int(line[1:line.find(',')])
        col = int(line[line.find(',')+1:-2])
        # coordinates = [int(row), int(col)]
        # result.append(coordinates)

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

cv.imshow("Display window", img)
k = cv.waitKey(0)
if k == ord("s"):
    cv.imwrite("result.png", img)
