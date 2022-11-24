from img2array import image_to_array
from array2header import array_to_header
from show_result import show_result
import tkinter as tk
from tkinter import filedialog
import os

root = tk.Tk()
root.withdraw()
image_path = filedialog.askopenfilename()
root.destroy()

map, origin, target = image_to_array(image_path)
array_to_header(map, "c/map.h")
os.system(
    f"cd c; ./main_cpp {origin[0]} {origin[1]} {target[0]} {target[1]}")
show_result("c/result.csv", image_path)
