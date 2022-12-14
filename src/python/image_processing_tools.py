import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt
import csv


def load_image(image_path):
    image = cv.imread(image_path)  # b,g,r
    if image is None:
        raise FileNotFoundError
    return image


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


def get_origin_target(img_binarized):
    # Create a figure and a set of subplots
    fig, ax = plt.subplots()
    ax.imshow(img_binarized, cmap='gray', vmin=0, vmax=255)
    # Bind the button_press_event with the onclick() method
    fig.canvas.mpl_connect('button_press_event', onclick)
    # Display the plot
    plt.show()

    # Write the origin and the target
    global origin
    global target

    return origin, target


def binarize_image(img):
    # Convert image into grayscale
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # Filter image
    gray = cv.GaussianBlur(gray, (3, 3), 0)
    # Binarize image
    img_binarized = cv.adaptiveThreshold(
        gray, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 25, 20)  # cv.ADAPTIVE_THRESH_GAUSSIAN_C ADAPTIVE_THRESH_MEAN_C
    # Scales, calculates absolute values, and converts the result to 8-bit.
    img_binarized = cv.convertScaleAbs(img_binarized)
    return img_binarized


def scale_image(img, percentage):
    # Scale image
    width = int(img.shape[1] * percentage / 100)
    height = int(img.shape[0] * percentage / 100)
    dim = (width, height)
    img_rescaled = cv.resize(img, dim, interpolation=3)
    return img_rescaled


def image_to_mapfile(img_binarized, map_path):
    # Normalize image between 0 and 1
    map = (np.max(img_binarized) - img_binarized) / np.max(img_binarized)
    map = map.astype(np.uint8)
    # Write map into a csv file
    with open(map_path, "w") as my_csv:
        csvWriter = csv.writer(my_csv, delimiter=',')
        csvWriter.writerows(map)


if __name__ == "__main__":
    import tkinter as tk
    from tkinter import filedialog
    root = tk.Tk()
    root.withdraw()
    file_path = filedialog.askopenfilename()
    root.destroy()
    map, origin, target = image_to_array(file_path)
