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

map = image_to_array(image_path)
array_to_header(map, "c/map.h")
os.system("cd c; make clean; make; ./main_cpp")
show_result("c/result.csv", image_path)
