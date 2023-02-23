#!/usr/bin/python3

from src.python.image_processing_tools import binarize_image, get_origin_target, image_to_mapfile, load_image
from src.python.show_result import show_result
import tkinter as tk
from tkinter import filedialog
import os


def main():
    root = tk.Tk()
    root.withdraw()
    image_path = filedialog.askopenfilename(multiple=False)
    root.destroy()

    if not image_path:
        print("No image selected!")
        return

    map_path = "map.csv"
    result_path = "result.csv"

    if os.path.exists(map_path):
        os.remove(map_path)

    image = load_image(image_path)
    img_binarized = binarize_image(image)
    origin, target = get_origin_target(img_binarized)
    if not origin or not target:
        print("No origin or target given!")
        return
    image_to_mapfile(img_binarized, map_path)

    if os.path.exists(result_path):
        os.remove(result_path)

    launch_command = f"./src/c/solver {map_path} {result_path} {origin[0]} {origin[1]} {target[0]} {target[1]}"
    os.system(launch_command)

    show_result(result_path, img_binarized, origin, target)


if __name__ == "__main__":
    main()
