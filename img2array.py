import numpy as np
import cv2 as cv
from array2header import array_to_header
from matplotlib import pyplot as plt
import csv

selector = True
origin = []
target = []


def onclick(event):
    if event.xdata is None or event.ydata is None:
        return
    c = int(event.xdata)
    r = int(event.ydata)
    global selector
    global origin
    global target
    if selector:
        origin = [r, c]
        print('Origin: r = %d, c = %d' % (r, c))
    else:
        target = [r, c]
        print('Target: r = %d, c = %d' % (r, c))
    selector = not selector


def image_to_array(path):
    # Read image in BGR format
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
    # Scales, calculates absolute values, and converts the result to 8-bit.
    gray = cv.convertScaleAbs(gray)

    # Create a figure and a set of subplots
    fig, ax = plt.subplots()
    ax.imshow(gray, cmap='gray', vmin=0, vmax=255)
    # Bind the button_press_event with the onclick() method
    fig.canvas.mpl_connect('button_press_event', onclick)
    # Display the plot
    plt.show()
    # Save processed image
    cv.imwrite("image.png", gray)

    # Normalize it to zeros and ones (255 -> 1)
    map = (np.max(gray) - gray) / np.max(gray)
    map = map.astype(np.uint8)

    # Write the origin and the target
    global origin
    global target

    # map[origin[0], origin[1]] = 2
    # map[target[0], target[1]] = 3
    # map[1733, 55] = 2
    # map[46, 1013] = 3

    with open("map.csv", "w") as my_csv:
        csvWriter = csv.writer(my_csv, delimiter=',')
        csvWriter.writerows(map)

    return map, origin, target


if __name__ == "__main__":
    import tkinter as tk
    from tkinter import filedialog
    root = tk.Tk()
    root.withdraw()
    file_path = filedialog.askopenfilename()
    root.destroy()
    map, origin, target = image_to_array(file_path)
    array_to_header(map, "c/map.h")
